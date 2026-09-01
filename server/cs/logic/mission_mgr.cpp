/********************************************************************
created: 2016年5月10日11:44:26
file base: mission
author: 张宇翔
purpose: 任务管理系统
*********************************************************************/

#include "mission_mgr.hpp"
#include "server_log.hpp"
#include "mission.hpp"
#include "logic/unit_man.h"
#include "mission_def.hpp"
#include "template/template_manager.h"
#include "aoi/aoi_system.h"
#include "logic/player.hpp"
#include "logic/npc.hpp"
#include "internet/aoi.pb.h"
#include "internet/net.pb.h"
#include "internet/mission.pb.h"
#include "utility/parse_msg.h"
#include "internet/cs2dp.pb.h"
#include "internal/mission_msg.hpp"

namespace faith
{
	cmission_mgr::cmission_mgr()
	{
		m_array_index = 0;
		clear_data();
	}

	cmission_mgr::~cmission_mgr()
	{}

	void cmission_mgr::clear_data()
	{
		for (int32 mission_index = 0; mission_index < e_mission_slot_max; mission_index++)
		{
			m_got_mission_array[mission_index].clear_data();
		}
		m_cur_mission_chapter_id = 0;
		for (int32 j = 0; j < e_power_up_prompt_max; ++j)
		{
			m_power_up_array[j] = false;
		}
	}

	void cmission_mgr::heart_tick(const int64& new_time)
	{
		//根据时间刷新任务
		for (int32 mission_index = e_mission_slot_main; mission_index < e_mission_slot_max; mission_index++)
		{
			m_got_mission_array[mission_index].heart_tick(new_time);
		}
		//根据完成的主线任务自动完成章节任务并接取下一章任务
		//if (m_got_mission_array[e_mission_show_type_chapter].finish_mission(0,e_mission_finish_type_normal))
		//{
		//	player& temp_player = unit_man::get_player(m_array_index);
		//	m_got_mission_array[e_mission_show_type_chapter].init_mission_by_template(m_got_mission_array[e_mission_show_type_chapter].get_mission_template_ptr()->NextMissionId, &temp_player);
		//	send_one_mission(m_got_mission_array[e_mission_show_type_chapter]);
		//}
		check_refresh_time();
	}

	void cmission_mgr::init_daily_mission()
	{
		player& temp_player = unit_man::get_player(m_array_index);

		if (m_got_mission_array[e_mission_slot_main].get_mission_id() < GAMECONFIG->DailyMissionNeedId)
		{//开启主线未完成
			return;
		}
		temp_player.set_unit_info(e_role_info_daily_done_num, 0);
		temp_player.set_must_do_count(e_daily_must_do_typ_raid_daily, 0);

		temp_player.send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_daily);
		temp_player.set_daily_active_degree_info(e_daily_must_do_typ_raid_daily);
		temp_player.send_info_one(e_role_info_daily_done_num);
		random_daily_mission();

	}
	void cmission_mgr::init_against_mission()
	{
		player& temp_player = unit_man::get_player(m_array_index);

		if (m_got_mission_array[e_mission_slot_main].get_mission_id() < GAMECONFIG->AgainstMissionNeedId)
		{//开启主线未完成
			return;
		}
		temp_player.set_unit_info(e_role_info_against_done_num, 0);
		temp_player.set_must_do_count(e_daily_must_do_typ_crusade_mission, 0);

		temp_player.send_info_one(e_role_info_daily_done_num);
		temp_player.send_info_one(e_role_info_against_done_num);
		random_against_mission();
	}
	void cmission_mgr::init_marry_mission()
	{
		player& temp_player = unit_man::get_player(m_array_index);

		if (m_got_mission_array[e_mission_slot_main].get_mission_id() < GAMECONFIG->MarryMissionNeedId)
		{//开启主线未完成
			return;
		}
		clear_marry_mission();
		temp_player.set_unit_info(e_role_info_marry_done_num, 0);
		temp_player.send_info_one(e_role_info_marry_done_num);
		//random_marry_mission();
	}
	void cmission_mgr::check_and_send_all_mission()
	{
		//刷新升级后的任务状态
		for (int32 i = 0; i < e_mission_slot_max; i++)
		{
			if (m_got_mission_array[i].get_inst_data(e_mission_inst_data_id) <= 0)
			{
				continue;
			}
			m_got_mission_array[i].check_mission_state();
		}
		send_all_mission();
	}
	cmission* cmission_mgr::find_mission(int32 mission_id)
	{
		if (mission_id <= 0)
		{
			return nullptr;
		}
		for (int32 mission_index = e_mission_slot_main; mission_index < e_mission_slot_max; mission_index++)
		{
			if (!m_got_mission_array[mission_index].is_valid(false))
			{
				continue;
			}
			if (m_got_mission_array[mission_index].get_inst_data(e_mission_inst_data_id) == mission_id)
			{
				return &m_got_mission_array[mission_index];
			}
		}
		return nullptr;
	}

	cmission* cmission_mgr::find_mission_by_index(int32 mission_index)
	{
		if (mission_index < e_mission_slot_main
			|| mission_index >= e_mission_slot_max)
		{//支线任务复数个 不能用这个找
			return nullptr;
		}
		if (m_got_mission_array[mission_index].get_inst_data(e_mission_inst_data_id) > 0)
		{
			return &m_got_mission_array[mission_index];
		}
		return nullptr;
	}

	cmission* cmission_mgr::find_mission_by_type(int32 mission_type)
	{
		if (mission_type < 0
			|| mission_type >= e_mission_type_side)
		{//支线任务复数个 不能用这个找
			return nullptr;
		}
		if (m_got_mission_array[mission_type].get_inst_data(e_mission_inst_data_id) > 0)
		{
			return &m_got_mission_array[mission_type];
		}
		return nullptr;
	}

	void cmission_mgr::gm_mission(int32 mission_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, mission_id);
		if (mission_template_ptr == nullptr)
		{
			return;
		}
		int32 target_slot = get_can_set_mission_slot(mission_template_ptr);
		if (target_slot < 0 || target_slot >= e_mission_slot_max)
		{
			if (mission_template_ptr->MissionShowType == e_mission_type_side)
			{
				target_slot = e_mission_slot_side_0;
			}
			else
			{
				return;
			}
		}
		int32 old_mission_id = m_got_mission_array[target_slot].get_inst_data(e_mission_inst_data_id);
		switch (mission_template_ptr->MissionShowType)
		{
		case faith::e_mission_type_main:
		{
			m_got_mission_array[target_slot].init_mission_by_template(mission_id, e_mission_slot_main, &temp_player);
			temp_player.get_service_goal_mgr().refresh_over_time_all();

			if (old_mission_id <= GAMECONFIG->DailyMissionNeedId
				&& mission_id > GAMECONFIG->DailyMissionNeedId)
			{
				init_daily_mission();
			}
			if (old_mission_id <= GAMECONFIG->AgainstMissionNeedId
				&& mission_id > GAMECONFIG->AgainstMissionNeedId)
			{
				init_against_mission();
			}			
		}
			break;
		case faith::e_mission_type_daily:
		{
			int32 cur_done_num = temp_player.get_unit_info(e_role_info_daily_done_num);
			if (cur_done_num >= DAILY_MISSION_MAX)
			{
				temp_player.set_unit_info(e_role_info_daily_done_num, DAILY_MISSION_MAX - 1);
				temp_player.send_info_one(e_role_info_daily_done_num);
				temp_player.set_must_do_count(e_daily_must_do_typ_raid_daily, DAILY_MISSION_MAX - 1);
				temp_player.send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_daily);
				temp_player.set_daily_active_degree_info(e_daily_must_do_typ_raid_daily);
			}
			random_daily_mission(mission_id);
		}
			break;
		case faith::e_mission_type_against:
		{
			int32 cur_done_num = temp_player.get_unit_info(e_role_info_against_done_num);
			if (cur_done_num >= AGAINST_MISSION_MAX)
			{
				temp_player.set_unit_info(e_role_info_against_done_num, AGAINST_MISSION_MAX - 1);
				temp_player.send_info_one(e_role_info_against_done_num);

				temp_player.set_must_do_count(e_daily_must_do_typ_crusade_mission, mission_id);
				temp_player.send_daily_must_do_count_info_one(e_daily_must_do_typ_crusade_mission);
				temp_player.set_daily_active_degree_info(e_daily_must_do_typ_crusade_mission);
			}
			random_against_mission(mission_id);
		}
			break;
		case faith::e_mission_type_side:
		case faith::e_mission_type_grade_up:
		{
			m_got_mission_array[target_slot].init_mission_by_template(mission_id, target_slot, &temp_player);
			send_one_mission(m_got_mission_array[target_slot]);
		}
			break;
		case faith::e_mission_type_marry:
			{
				int32 cur_done_num = temp_player.get_unit_info(e_role_info_marry_done_num);
				if (cur_done_num >= MARRY_MISSION_MAX)
				{
					temp_player.set_unit_info(e_role_info_marry_done_num, MARRY_MISSION_MAX - 1);
					temp_player.send_info_one(e_role_info_marry_done_num);
				}
				random_marry_mission(false,mission_id);
			}
			break;
		default:
			break;
		}
		send_one_mission(m_got_mission_array[target_slot]);
	}

	void cmission_mgr::operate_npc_show(MissionTemplate* OldTemplatePtr, MissionTemplate* NewTemplatePtr)
	{
		//if (OldTemplatePtr == nullptr || NewTemplatePtr == nullptr)
		//{
		//	return;
		//}
		//这两个都可以为空

		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(temp_player.get_map_ent(), temp_player.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return;
		}
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			npc& npc_ref = unit_man::get_npc(unit_index);
			if (npc_ref.is_valid() == false)
			{
				continue;
			}
			NpcTemplate* npc_template_ptr = npc_ref.get_npc_template();
			if (npc_template_ptr == nullptr)
			{
				continue;
			}

			if (npc_template_ptr->EndShowMissionId > 0
				&& npc_template_ptr->EndShowMissionState == 0)
			{
				if (OldTemplatePtr != nullptr
					&& OldTemplatePtr->attribute_id >= npc_template_ptr->EndShowMissionId)
				{//用旧任务判断要不要移除
					aoi_proto_unit_aoi_out unit_aoi_out_msg;
					unit_aoi_out_msg.add_unit_guid(npc_ref.get_unit_guid().server_64);
					//unit_aoi_out_msg.set_is_dissolve(true);
					temp_player.send_message_to_self(&unit_aoi_out_msg, e_msgindex_s2c_aoi_out);
				}
			}

			if (npc_template_ptr->StartShowMissionId > 0
				&& npc_template_ptr->StartShowMissionState == 0)
			{
				if (NewTemplatePtr != nullptr
					&& NewTemplatePtr->attribute_id <= npc_template_ptr->StartShowMissionId)
				{//用新任务判断要不要加入
					aoi_proto_unit_aoi_all unit_aoi_enter_msg;
					npc_ref.get_aoi_msg(unit_aoi_enter_msg);
					unit_aoi_enter_msg.set_can_show_head(npc_ref.check_can_show_head(temp_player.get_unit_guid()));
					temp_player.send_message_to_self(&unit_aoi_enter_msg, e_msgindex_s2c_aoi_simple_one);
				}
			}
		}
	}

	bool cmission_mgr::random_daily_mission(int32 force_mission_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		PlayerUpgradeTemplate* player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, temp_player.get_unit_info(e_role_info_upgrade_id));
		if (nullptr == player_upgrade_ptr)
		{
			return false;
		}
		if (m_got_mission_array[e_mission_slot_main].get_inst_data(e_mission_inst_data_id) <= GAMECONFIG->DailyMissionNeedId)
		{//开启主线未完成
			return false;
		}
		if (temp_player.get_unit_info(e_role_info_daily_done_num) >= DAILY_MISSION_MAX)
		{//全部做完了

			return false;
		}

		int32 new_mission_id = 0;
		{
			MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, force_mission_id);
			if (mission_template_ptr == nullptr
				|| mission_template_ptr->MissionShowType != e_mission_type_daily)
			{
				MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->DailyMissionLibrary);
				if (nullptr == daily_mission_library_ptr)
				{
					return false;
				}
				new_mission_id = daily_mission_library_ptr->MissionLibraryArray[rand() % daily_mission_library_ptr->MissionLibraryArray.size()];
			}
			else
			{
				new_mission_id = force_mission_id;
			}
		}
		MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, new_mission_id);
		if (mission_template_ptr == nullptr
			|| mission_template_ptr->MissionShowType != e_mission_type_daily)
		{
			return false;
		}

		m_got_mission_array[e_mission_slot_daily].init_mission_by_template(new_mission_id, e_mission_slot_daily, &temp_player);
		//int32 temp_rate = rand() % 100;
		//int32 star_num = 0;
		//for (star_num = 0; star_num < GAMECONFIG->DailyMissionMaxStarLevel; star_num++)
		//{
		//	temp_rate -= GAMECONFIG->DailyMissionStarRate[star_num];
		//	if (temp_rate < 0)
		//	{
		//		break;
		//	}
		//}

		m_got_mission_array[e_mission_slot_daily].set_inst_data(e_mission_inst_data_star_level, GAMECONFIG->DailyMissionMaxStarLevel);

		send_one_mission(m_got_mission_array[e_mission_slot_daily]);
		return true;
	}
	bool cmission_mgr::random_against_mission(int32 force_mission_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);

		if (m_got_mission_array[e_mission_slot_main].get_inst_data(e_mission_inst_data_id) <= GAMECONFIG->AgainstMissionNeedId)
		{//开启主线未完成
			return false;
		}
		PlayerUpgradeTemplate* player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, temp_player.get_unit_info(e_role_info_upgrade_id));
		if (nullptr == player_upgrade_ptr)
		{
			return false;
		}
		if (temp_player.get_unit_info(e_role_info_against_done_num) >= AGAINST_MISSION_MAX)
		{
			return false;
		}

		int32 new_mission_id = 0;
		{
			MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, force_mission_id);
			if (mission_template_ptr == nullptr
				|| mission_template_ptr->MissionShowType != e_mission_type_against)
			{
				MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->AgainstMissionLibrary);
				if (nullptr == daily_mission_library_ptr)
				{
					return false;
				}
				new_mission_id = daily_mission_library_ptr->MissionLibraryArray[rand() % daily_mission_library_ptr->MissionLibraryArray.size()];
			}
			else
			{
				new_mission_id = force_mission_id;
			}
		}
		MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, new_mission_id);
		if (mission_template_ptr == nullptr
			|| mission_template_ptr->MissionShowType != e_mission_type_against)
		{
			return false;
		}

		m_got_mission_array[e_mission_slot_against].init_mission_by_template(new_mission_id, e_mission_slot_against, &temp_player);

		send_one_mission(m_got_mission_array[e_mission_slot_against]);
		return true;
	}
	void cmission_mgr::clear_marry_mission()
	{
		m_got_mission_array[e_mission_slot_extra_side_0].set_mission_state(e_mission_state_cannot_accept);//设置条件不足以接取
		send_one_mission(m_got_mission_array[e_mission_slot_extra_side_0]);
		m_got_mission_array[e_mission_slot_extra_side_0].clear_data();
	}
	bool cmission_mgr::random_marry_mission(bool is_operate, int32 force_mission_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);

		if (m_got_mission_array[e_mission_slot_main].get_inst_data(e_mission_inst_data_id) <= GAMECONFIG->MarryMissionNeedId)
		{//开启主线未完成
			return false;
		}
		player& couple_player = unit_man::get_player(temp_player.get_couple_guid());
		int32 min_level = temp_player.get_unit_info(e_role_info_upgrade_id);
		int32 couple_level = couple_player.get_unit_info(e_role_info_upgrade_id);
		if (couple_level < min_level)
		{
			min_level = couple_level;
		}
		//用夫妻两人中等级较小的计算
		PlayerUpgradeTemplate* player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, min_level);
		if (nullptr == player_upgrade_ptr)
		{
			return false;
		}
		if (m_got_mission_array[e_mission_slot_extra_side_0].get_inst_data(e_mission_inst_data_mission_state) == e_mission_state_accepted)
		{
			if (is_operate)
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90096817);
				temp_player.send_notice(notice_str);
			}
			return false;
		}
		if (temp_player.get_unit_info(e_role_info_marry_done_num) >= MARRY_MISSION_MAX)
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90096778);
			temp_player.send_notice(notice_str);
			return false;
		}

		int32 new_mission_id = 0;
		{
			MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, force_mission_id);
			if (mission_template_ptr == nullptr
				|| mission_template_ptr->MissionShowType != e_mission_type_marry)
			{
				MissionLibraryTemplate* marry_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->MarryMissionLibrary);
				if (nullptr == marry_mission_library_ptr)
				{
					return false;
				}
				//不随机
				int32 marry_mission_done_num = temp_player.get_unit_info(e_role_info_marry_done_num);
				if (marry_mission_done_num >= marry_mission_library_ptr->MissionLibraryArray.size())
				{
					new_mission_id = marry_mission_library_ptr->MissionLibraryArray[marry_mission_library_ptr->MissionLibraryArray.size() - 1];
				}
				else
				{
					new_mission_id = marry_mission_library_ptr->MissionLibraryArray[marry_mission_done_num];
				}
			}
			else
			{
				new_mission_id = force_mission_id;
			}
		}
		MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, new_mission_id);
		if (mission_template_ptr == nullptr
			|| mission_template_ptr->MissionShowType != e_mission_type_marry)
		{
			return false;
		}
		m_got_mission_array[e_mission_slot_extra_side_0].init_mission_by_template(new_mission_id, e_mission_slot_extra_side_0, &temp_player);
		send_one_mission(m_got_mission_array[e_mission_slot_extra_side_0]);
		return true;
	}

	void cmission_mgr::set_marry_mission(int32 new_mission_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		m_got_mission_array[e_mission_slot_extra_side_0].init_mission_by_template(new_mission_id, e_mission_slot_extra_side_0, &temp_player);
		send_one_mission(m_got_mission_array[e_mission_slot_extra_side_0]);
	}

	void cmission_mgr::send_one_mission(cmission& my_mission)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (my_mission.get_mission_template_ptr() == nullptr)
		{
			return;
		}

		mission_proto_mission_info mission_msg_one;
		for (int32 goal_num = e_mission_inst_data_id; goal_num < e_mission_inst_data_max; goal_num++)
		{
			mission_msg_one.add_mission_info(my_mission.get_inst_data(goal_num));
		}
		temp_player.send_message_to_self(&mission_msg_one, e_msgindex_s2c_mission_one);
	}
	void cmission_mgr::send_all_mission()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		mission_proto_mission_all mission_msg_all;
		mission_msg_all.add_role_guid(temp_player.get_unit_guid().A);
		mission_msg_all.add_role_guid(temp_player.get_unit_guid().B);
		mission_msg_all.set_daily_mission_done_num(temp_player.get_unit_info(e_role_info_daily_done_num));
		mission_msg_all.set_against_mission_done_num(temp_player.get_unit_info(e_role_info_against_done_num));
		mission_msg_all.set_is_finish(false);
		for (int32 mission_index = e_mission_slot_main; mission_index < e_mission_slot_max; mission_index++)
		{
			mission_proto_mission_info *mission_msg_one = mission_msg_all.add_mission_array();
			if (mission_msg_one == nullptr)
			{
				continue;
			}
			for (int32 info_index = e_mission_inst_data_id; info_index < e_mission_inst_data_max; info_index++)
			{
				mission_msg_one->add_mission_info(m_got_mission_array[mission_index].get_inst_data(info_index));
			}

			int32 pak_size = mission_msg_all.ByteSize();
			if (pak_size > packege_length_max)
			{
				temp_player.send_message_to_self(&mission_msg_all, e_msgindex_s2c_mission_all);
				mission_msg_all.clear_mission_array();
			}
		}
		mission_msg_all.set_is_finish(true);
		temp_player.send_message_to_self(&mission_msg_all, e_msgindex_s2c_mission_all);
	}
	void cmission_mgr::save_mission_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		cs2dp_save_char_mission req;
		req.save_type_ex = save_type;
		req.role_guid = player_ref.get_unit_guid();
		req.unit_array_index = m_array_index;
		int32 mission_count = 0;
		for (int32 mission_index = e_mission_slot_main; mission_index < e_mission_slot_max; mission_index++) // 所有物品的数据
		{
			if (m_got_mission_array[mission_index].get_inst_data(e_mission_inst_data_id) == 0)
			{
				continue;
			}
			req.data_list[mission_count] = m_got_mission_array[mission_index].get_ins_data_array();
			mission_count++;
		}
		req.data_num = mission_count;
		player_ref.send_message_to_dp(&req, req.get_pak_length());
	}
	void cmission_mgr::load_mission_by_db(const s_mission_info* mission_data, int32 mission_num)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (mission_num == 0)
		{
			PlayerInitTemplate* template_data_ptr = GET_TEMPLATE(PlayerInitTemplate, temp_player.get_unit_info(e_role_info_template_id));
			if (!template_data_ptr)
			{
				return;
			}
			for (int32 i = 0; i < template_data_ptr->InitMission.size(); i++)
			{
				if (i >= e_mission_slot_max)
				{
					break;
				}
				if (template_data_ptr->InitMission[i] == 0)
				{
					continue;
				}
				MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, template_data_ptr->InitMission[i]);
				if (nullptr == mission_ptr)
				{
					continue;
				}
				int32 target_slot = get_can_set_mission_slot(mission_ptr);
				s_mission_info temp_mission_info;
				temp_mission_info.data_ary[e_mission_inst_data_id] = template_data_ptr->InitMission[i];
				temp_mission_info.data_ary[e_mission_inst_type] = mission_ptr->MissionShowType;
				temp_mission_info.data_ary[e_mission_inst_slot] = target_slot;
				m_got_mission_array[target_slot].init_mission_by_info(temp_mission_info, &temp_player);
			}
		}
		else
		{
			for (int32 i = 0; i < mission_num; i++)
			{
				int32 target_slot = mission_data[i].data_ary[e_mission_inst_slot];
				if (mission_data[i].data_ary[e_mission_inst_data_id] == 0
					|| target_slot < 0
					|| target_slot >= e_mission_slot_max)
				{
					continue;
				}
				MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, mission_data[i].data_ary[e_mission_inst_data_id]);
				if (nullptr == mission_ptr)
				{
					continue;
				}
				target_slot = mission_data[i].data_ary[e_mission_inst_slot];
				if (!can_set_in_this_slot(target_slot, mission_ptr))
				{
					target_slot = get_can_set_mission_slot(mission_ptr);
				}
				if (target_slot < 0
					|| target_slot >= e_mission_slot_max)
				{//这里有变化的可能，所以要再检测一次
					continue;
				}

				s_mission_info temp_info = mission_data[i];
				temp_info.data_ary[e_mission_inst_slot] = target_slot;
				m_got_mission_array[target_slot].init_mission_by_info(temp_info, &temp_player);
			}
		}

		send_all_mission();
		reset_mission_chapter(false);
	}
	bool cmission_mgr::load_mission_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_mission_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_mission_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_mission_info *p_row = (s_mission_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_mission_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_mission_by_db((const s_mission_info *)p_data, msg.row_count());
		return true;

	}
	void cmission_mgr::check_refresh_time()
	{
		//改由 player 的按天刷新，下面 gameconfig 的任务刷新时间实际上是 24

		//player& player_ref = unit_man::get_player(m_array_index);
		//tm* now_time = time_helper::get_localtime();
		//if (nullptr == now_time)
		//{
		//	return;
		//}
		//int32 now_day = now_time->tm_year * 10000 + now_time->tm_mon * 100 + now_time->tm_mday;
		//int32 now_hour = now_time->tm_hour * 10000 + now_time->tm_min * 100 + now_time->tm_sec;

		//if (now_day > player_ref.get_time_data(e_time_type_last_refresh_time) && now_hour > GAMECONFIG->MissionRefreshTime)
		//{
		//	player_ref.set_time_data(e_time_type_last_refresh_time, now_day);
		//	init_against_mission();
		//	init_daily_mission();
		//	send_all_mission();
		//}
	}

	void cmission_mgr::change_mission(const e_mission_slot mission_index, const int32 mission_template_id)
	{
		if (mission_index != e_mission_slot_daily
			&& mission_index != e_mission_slot_against)
		{
			return;
		}

		cmission* mission_ptr = find_mission_by_index(mission_index);
		if (nullptr == mission_ptr)
		{
			return;
		}
		MissionTemplate* mission_template_ptr = GET_TEMPLATE(MissionTemplate, mission_template_id);
		if (nullptr == mission_template_ptr)
		{
			return;
		}
		if (mission_ptr->get_inst_data(e_mission_inst_type) != mission_template_ptr->MissionShowType)	// 防止刷新错任务id
		{
			return;
		}
		if (mission_ptr->get_mission_id() == mission_template_id)
		{
			return;
		}
		mission_ptr->change_mission(mission_template_id);
		send_one_mission(*mission_ptr);
	}

	int32 cmission_mgr::get_mission_id(const e_mission_slot mission_index)
	{
		cmission* mission_ptr = find_mission_by_index(mission_index);
		if (nullptr == mission_ptr)
		{
			return 0;
		}
		return mission_ptr->get_inst_data(e_mission_inst_data_id);
	}

	int32 cmission_mgr::get_new_mission_id(const e_mission_slot mission_index)
	{
		int32 new_mission_id = 0;
		player& temp_player = unit_man::get_player(m_array_index);
		PlayerUpgradeTemplate* player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, temp_player.get_unit_info(e_role_info_upgrade_id));
		if (nullptr == player_upgrade_ptr)
		{
			return 0;
		}
		switch (mission_index)
		{
		case e_mission_slot_daily:
		{
			MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->DailyMissionLibrary);
			if (nullptr == daily_mission_library_ptr)
			{
				return 0;
			}
			new_mission_id = daily_mission_library_ptr->MissionLibraryArray[rand() % daily_mission_library_ptr->MissionLibraryArray.size()];
			break;
		}
		case e_mission_slot_against:
		{
			MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->AgainstMissionLibrary);
			if (nullptr == daily_mission_library_ptr)
			{
				return false;
			}
			new_mission_id = daily_mission_library_ptr->MissionLibraryArray[rand() % daily_mission_library_ptr->MissionLibraryArray.size()];
			break;
		}
		default:
			break;
		}
		return new_mission_id;
	}

	bool cmission_mgr::is_cur_mission_accord_level(const e_mission_slot mission_index)
	{
		const int32 cur_mission_id = get_mission_id(mission_index);
		player& temp_player = unit_man::get_player(m_array_index);
		PlayerUpgradeTemplate* player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, temp_player.get_unit_info(e_role_info_upgrade_id));
		if (nullptr == player_upgrade_ptr)
		{
			return true;
		}
		switch (mission_index)
		{
		case e_mission_slot_daily:
		{
			MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->DailyMissionLibrary);
			if (nullptr == daily_mission_library_ptr)
			{
				return true;
			}
			const int32 mission_library_array_size = daily_mission_library_ptr->MissionLibraryArray.size();
			for (int32 i = 0; i < mission_library_array_size; i++)
			{
				if (cur_mission_id == daily_mission_library_ptr->MissionLibraryArray[i])
				{
					return true;
				}
			}
			return false;
		}
			break;
		case e_mission_slot_against:
		{
			MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->AgainstMissionLibrary);
			if (nullptr == daily_mission_library_ptr)
			{
				return true;
			}
			const int32 mission_library_array_size = daily_mission_library_ptr->MissionLibraryArray.size();
			for (int32 i = 0; i < mission_library_array_size; i++)
			{
				if (cur_mission_id == daily_mission_library_ptr->MissionLibraryArray[i])
				{
					return true;
				}
			}
			return false;
		}
			break;
		default:
			break;
		}

		return true;
	}

	bool cmission_mgr::is_cur_mission_high_level(const e_mission_slot mission_index)
	{
		const int32 cur_mission_id = get_mission_id(mission_index);
		player& temp_player = unit_man::get_player(m_array_index);
		PlayerUpgradeTemplate* player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, temp_player.get_unit_info(e_role_info_upgrade_id));
		if (nullptr == player_upgrade_ptr)
		{
			return false;
		}
		switch (mission_index)
		{
		case e_mission_slot_daily:
		{
			MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->DailyMissionLibrary);
			if (nullptr == daily_mission_library_ptr)
			{
				return false;
			}
			const int32 mission_library_array_size = daily_mission_library_ptr->MissionLibraryArray.size();

			for (int32 mission_library_array_index = 0; mission_library_array_index < mission_library_array_size; mission_library_array_index++)
			{
				if (cur_mission_id <= daily_mission_library_ptr->MissionLibraryArray[mission_library_array_index])
				{
					return false;
				}
			}
			return true;
		}
			break;
		case e_mission_slot_against:
		{
			MissionLibraryTemplate* daily_mission_library_ptr = GET_TEMPLATE(MissionLibraryTemplate, player_upgrade_ptr->AgainstMissionLibrary);
			if (nullptr == daily_mission_library_ptr)
			{
				return false;
			}
			const int32 mission_library_array_size = daily_mission_library_ptr->MissionLibraryArray.size();

			for (int32 mission_library_array_index = 0; mission_library_array_index < mission_library_array_size; mission_library_array_index++)
			{
				if (cur_mission_id <= daily_mission_library_ptr->MissionLibraryArray[mission_library_array_index])
				{
					return false;
				}
			}
			return true;
		}
			break;
		default:
			break;
		}

		return false;
	}

	void cmission_mgr::reset_mission_chapter(bool is_need_send)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		int32 cur_main_mission_id = m_got_mission_array[e_mission_slot_main].get_inst_data(e_mission_inst_data_id);
		MissionChapterTemplate*	new_chapter_template = template_manager::get_instance().get_cur_chapter_template_by_main_mission_id(cur_main_mission_id);
		if (new_chapter_template != nullptr)
		{
			if (m_cur_mission_chapter_id != new_chapter_template->attribute_id)
			{//有变化
				MissionChapterTemplate* new_finish_chapter_template = template_manager::get_instance().get_template_by_mission_chapter_index(new_chapter_template->ChapterNum - 1);

				MissionChapterTemplate*	old_finish_chapter_template = nullptr;
				MissionChapterTemplate*	old_in_chapter_template = GET_TEMPLATE(MissionChapterTemplate, m_cur_mission_chapter_id);
				if (old_in_chapter_template != nullptr)
				{
					old_finish_chapter_template = template_manager::get_instance().get_template_by_mission_chapter_index(old_in_chapter_template->ChapterNum - 1);;
				}

				if (old_finish_chapter_template != nullptr)
				{
					unit_man::operate_unit_att(m_array_index, false, old_finish_chapter_template->AttValueArray);
				}
				if (new_finish_chapter_template != nullptr)
				{
					unit_man::operate_unit_att(m_array_index, true, new_finish_chapter_template->AttValueArray);
				}
				m_cur_mission_chapter_id = new_chapter_template->attribute_id;

				if (is_need_send && nullptr != new_finish_chapter_template)
				{
					mission_proto_show_chapter_over chap_finish_msg;
					chap_finish_msg.set_chapter_template_id(new_finish_chapter_template->attribute_id);
					temp_player.send_message_to_self(&chap_finish_msg, e_msgindex_s2c_mission_show_chapter_finish);
				}
			}
		}
		else
		{
			m_cur_mission_chapter_id = 0;
		}
	}

	void cmission_mgr::refresh_main_mission()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		cmission* cur_grade_mission = find_mission_by_index(e_mission_slot_main);
		if (cur_grade_mission == nullptr
			|| cur_grade_mission->get_inst_data(e_mission_inst_data_mission_state) != e_mission_state_finished)
		{//不相等说明没做完
			return;
		}
		MissionTemplate* cur_grade_mission_ptr = cur_grade_mission->get_mission_template_ptr();
		if (cur_grade_mission_ptr == nullptr
			|| cur_grade_mission_ptr->NextMissionId <= 0)
		{//next为0就说明做完了
			return;
		}
		int32 target_slot = cur_grade_mission->get_inst_data(e_mission_inst_slot);
		m_got_mission_array[target_slot].init_mission_by_template(cur_grade_mission_ptr->NextMissionId, target_slot, &temp_player);
		send_one_mission(*cur_grade_mission);
	}

	void  cmission_mgr::refresh_missions()
	{
		//处理新加的主线任务
		refresh_main_mission();
		//处理新加的转职任务
		refresh_grade_mission();
	}

	void cmission_mgr::refresh_side_mission(int32 trigger_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		MissionTemplate* trigger_template_ptr = GET_TEMPLATE(MissionTemplate, trigger_id);
		if (trigger_template_ptr == nullptr)
		{
			return;
		}
		if (trigger_template_ptr->TriggerSideMissionArray.size() == 0)
		{
			if (trigger_id == temp_player.get_unit_info(e_role_info_next_side_mission_main_trigger_id))
			{//防止下面的递归死循环，以及理论上，这个位置存储的任务ID，一定是有支线任务的
				temp_player.set_unit_info(e_role_info_next_side_mission_main_trigger_id, 0);
				temp_player.send_info_one(e_role_info_next_side_mission_main_trigger_id);
				return;
			}

			if (temp_player.get_unit_info(e_role_info_next_side_mission_main_trigger_id) > 0)
			{
				refresh_side_mission(temp_player.get_unit_info(e_role_info_next_side_mission_main_trigger_id));
			}
			else
			{
				return;
			}
		}

		cmission* cur_main_mission = find_mission_by_index(e_mission_slot_main);
		if (cur_main_mission == nullptr)
		{
			return;
		}
		if (trigger_template_ptr->MissionShowType == e_mission_type_main
			&& trigger_id >= cur_main_mission->get_mission_id())
		{//主线支线触发器必须是已完成的主线任务
			return;
		}

		bool is_success_refresh = false;
		for (int32 i = 0; i < trigger_template_ptr->TriggerSideMissionArray.size(); ++i)
		{
			int32 new_side_mission_id = trigger_template_ptr->TriggerSideMissionArray[i];
			if (find_mission(new_side_mission_id))
			{
				continue;
			}
			MissionTemplate* new_side_mission_ptr = GET_TEMPLATE(MissionTemplate, new_side_mission_id);
			if (nullptr == new_side_mission_ptr
				|| new_side_mission_ptr->SideMissionReqMainMissionId >= cur_main_mission->get_mission_id()
				|| is_has_same_group_misison(new_side_mission_ptr->MissionGroupId))
			{
				continue;
			}

			int32 empty_slot = get_empty_side_mission_slot(trigger_id);
			if (empty_slot < 0
				|| empty_slot >= e_mission_slot_max)
			{
				break;
			}
			m_got_mission_array[empty_slot].init_mission_by_template(new_side_mission_id, empty_slot, &temp_player);
			send_one_mission(m_got_mission_array[empty_slot]);
			is_success_refresh = true;
		}

		if (is_success_refresh
			&& trigger_template_ptr->MissionShowType == e_mission_type_main)
		{
			temp_player.set_unit_info(e_role_info_next_side_mission_main_trigger_id, trigger_template_ptr->NextSideMissionTriggerId);
			temp_player.send_info_one(e_role_info_next_side_mission_main_trigger_id);
		}
	}

	void cmission_mgr::refresh_grade_mission()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		cmission* cur_grade_mission = find_mission_by_index(e_mission_slot_grade_up);
		if (cur_grade_mission == nullptr
			|| cur_grade_mission->get_inst_data(e_mission_inst_data_mission_state) != e_mission_state_finished)
		{//不相等说明没做完
			return;
		}
		MissionTemplate* cur_grade_mission_ptr = cur_grade_mission->get_mission_template_ptr();
		if (cur_grade_mission_ptr == nullptr
			|| cur_grade_mission_ptr->NextMissionId <= 0)
		{//next为0就说明做完了
			return;
		}
		int32 target_slot = cur_grade_mission->get_inst_data(e_mission_inst_slot);
		m_got_mission_array[target_slot].init_mission_by_template(cur_grade_mission_ptr->NextMissionId, target_slot, &temp_player);
		send_one_mission(*cur_grade_mission);
	}

	bool cmission_mgr::is_has_same_group_misison(int32 check_group_id)
	{
		if (check_group_id <= 0)
		{
			return false;
		}
		for (int32 i = 0; i < e_mission_slot_max; ++i)
		{
			if (m_got_mission_array[i].get_mission_template_ptr() == nullptr
				|| m_got_mission_array[i].get_inst_data(e_mission_inst_data_id) <= 0
				|| m_got_mission_array[i].get_inst_data(e_mission_inst_data_mission_state) <= e_mission_state_none
				|| m_got_mission_array[i].get_inst_data(e_mission_inst_data_mission_state) >= e_mission_state_finished)
			{
				continue;
			}

			if (m_got_mission_array[i].get_mission_template_ptr()->MissionGroupId == check_group_id)
			{
				return true;
			}
		}
		return false;
	}

	bool cmission_mgr::can_set_in_this_slot(int32 target_slot, MissionTemplate* mission_template_ptr)
	{
		if (mission_template_ptr == nullptr
			|| target_slot < 0
			|| target_slot >= e_mission_slot_max)
		{
			return false;
		}
		
		switch (target_slot)
		{
		case e_mission_slot_main:
		{
			if (mission_template_ptr->MissionShowType != e_mission_type_main)
			{
				return false;
			}
		}
			break;
		case e_mission_slot_daily:
		{
			if (mission_template_ptr->MissionShowType != e_mission_type_daily)
			{
				return false;
			}
		}
			break;
		case e_mission_slot_against:
		{
			if (mission_template_ptr->MissionShowType != e_mission_type_against)
			{
				return false;
			}
		}
			break;
		case e_mission_slot_grade_up:
		{
			if (mission_template_ptr->MissionShowType != e_mission_type_grade_up)
			{
				return false;
			}
		}
			break;
		case e_mission_slot_extra_side_0:
		{
			if (mission_template_ptr->MissionShowType != e_mission_type_marry)
			{
				return false;
			}
		}
			break;
		default:
		{
			if (m_got_mission_array[target_slot].get_mission_id() != 0
				&& m_got_mission_array[target_slot].get_mission_id() != mission_template_ptr->attribute_id
				&& m_got_mission_array[target_slot].get_mission_state() != e_mission_state_finished)
			{
				return false;
			}
		}
			break;
		}
		return true;
	}

	int32 cmission_mgr::get_can_set_mission_slot(MissionTemplate* mission_template_ptr)
	{
		int32 return_value = -1;
		if (mission_template_ptr == nullptr)
		{
			return return_value;
		}
		switch (mission_template_ptr->MissionShowType)
		{
		case e_mission_type_main:
			return_value = e_mission_slot_main;
			break;
		case e_mission_type_daily:
			return_value = e_mission_slot_daily;
			break;
		case e_mission_type_against:
			return_value = e_mission_slot_against;
			break;
		case e_mission_type_side:
			return_value = get_empty_side_mission_slot(mission_template_ptr->attribute_id);
			break;
		case e_mission_type_grade_up:
			return_value = e_mission_slot_grade_up;
			break;
		case e_mission_type_marry:
			return_value = e_mission_slot_extra_side_0;
			break;
		default:
			break;
		}
		return return_value;
	}

	int32 cmission_mgr::get_empty_side_mission_slot(int32 trigger_mission_id)
	{
		int32 return_value = -1;
		for (int32 i = e_mission_slot_side_0; i < e_mission_slot_max; ++i)
		{
			if (i == e_mission_slot_extra_side_0)
			{
				continue;
			}
			if (m_got_mission_array[i].get_mission_id() == 0
				|| m_got_mission_array[i].get_mission_id() == trigger_mission_id
				|| m_got_mission_array[i].get_mission_state() == e_mission_state_finished)
			{
				return_value = i;
				break;
			}
		}
		return return_value;
	}

	void cmission_mgr::target_check(e_mission_end_type mission_end_type, int32 target_info_0, int32 target_info_1)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		player_ref.get_star_trip_mgr().target_mission_by_mission_type(mission_end_type, target_info_0, target_info_1);
		//变强任务需要检查是否可以完成
		if (mission_end_type == e_mission_end_type_intensify_add_gs && false == get_power_up_is_can_check(target_info_0))
		{
			return;
		}
		for (int32 i = e_mission_slot_main; i < e_mission_slot_max; ++i)
		{
			if (m_got_mission_array[i].target_check(mission_end_type, target_info_0, target_info_1))
			{
				send_one_mission(m_got_mission_array[i]);
				//if (e_mission_slot_main == i && m_got_mission_array[i].get_mission_id() == 15000632)//增加特例，该任务处于已完成待交付状态，则进行下面的操作
				//{
				//	player_ref.get_item_set().create_item_by_template(31040130);//当把龙杀死以后，则给予翅膀物品
				//}
			}
		}
	}

	bool cmission_mgr::start_dialog(int32 mission_id, int32 npc_id)
	{
		for (int32 mission_index = e_mission_slot_main; mission_index < e_mission_slot_max; mission_index++)
		{
			if (m_got_mission_array[mission_index].start_dialog(npc_id) == true)
			{//对话有效
				send_one_mission(m_got_mission_array[mission_index]);
				return true;
			}
		}
		return false;
	}

	void  cmission_mgr::gm_set_grade_mission_id(int32 mission_id)
	{
		cmission& grade_mission = m_got_mission_array[e_mission_slot_grade_up];
		if (grade_mission.is_valid())
		{
			grade_mission.set_inst_data(e_mission_inst_data_id, mission_id);
			player& temp_player = unit_man::get_player(m_array_index);
			m_got_mission_array[e_mission_slot_grade_up].init_mission_by_info(grade_mission.get_ins_data_array(), &temp_player);
			grade_mission.set_mission_state(e_mission_state_can_finish);

			send_one_mission(grade_mission);
		}
	}

	void cmission_mgr::operate_npc_show_by_lua(int32 old_mission_id, int32 new_mission_id)
	{

		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(temp_player.get_map_ent(), temp_player.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return;
		}
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			npc& npc_ref = unit_man::get_npc(unit_index);
			if (npc_ref.is_valid() == false)
			{
				continue;
			}
			NpcTemplate* npc_template_ptr = npc_ref.get_npc_template();
			if (npc_template_ptr == nullptr)
			{
				continue;
			}

			if (npc_template_ptr->EndShowMissionId > 0
				&& npc_template_ptr->EndShowMissionState == 0)
			{
				if (old_mission_id != 0
					&& old_mission_id >= npc_template_ptr->EndShowMissionId)
				{//用旧任务判断要不要移除
					aoi_proto_unit_aoi_out unit_aoi_out_msg;
					unit_aoi_out_msg.add_unit_guid(npc_ref.get_unit_guid().server_64);
					unit_aoi_out_msg.set_is_dissolve(true);
					temp_player.send_message_to_self(&unit_aoi_out_msg, e_msgindex_s2c_aoi_out);
				}
			}

			if (npc_template_ptr->StartShowMissionId > 0
				&& npc_template_ptr->StartShowMissionState == 0)
			{
				if (new_mission_id != 0
					&& new_mission_id <= npc_template_ptr->StartShowMissionId)
				{//用新任务判断要不要加入
					aoi_proto_unit_aoi_all unit_aoi_enter_msg;
					npc_ref.get_aoi_msg(unit_aoi_enter_msg);
					unit_aoi_enter_msg.set_can_show_head(npc_ref.check_can_show_head(temp_player.get_unit_guid()));
					temp_player.send_message_to_self(&unit_aoi_enter_msg, e_msgindex_s2c_aoi_simple_one);
				}
			}
		}

	}
	void cmission_mgr::set_power_up_be_use(int32 power_type)
	{
		if (power_type < 0 || power_type >= e_power_up_prompt_max)
		{
			return;
		}
		m_power_up_array[power_type] = true;
	}
	bool cmission_mgr::get_power_up_is_can_check(int32 power_type)
	{
		if (power_type < 0 || power_type >= e_power_up_prompt_max)
		{
			return false;
		}
		return m_power_up_array[power_type];
	}
}
