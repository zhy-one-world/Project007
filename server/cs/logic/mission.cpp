/********************************************************************
created: 2016年5月5日11:14:26
file base: mission
author: 张宇翔
purpose: 单个任务
*********************************************************************/

#include "aoi/aoi_system.h"
#include "att_def.hpp"
#include "logic/buff_man.h"
#include "logic/drop.h"
#include "logic/item_set.h"
#include "logic/player.hpp" 
#include "logic/skill_manager.h"
#include "logic/skill_set.h"
#include "logic/unit_man.h"
#include "mission.hpp"
#include "internet/mission.pb.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/item/item_upgrade_system.h"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "world_cs.h"
#include "internet/net.pb.h"
#include "npc.hpp"
#include "utility/random.h"
#include "internet/aoi.pb.h"

namespace faith
{
	cmission::cmission()
	{
		m_player_ptr = nullptr;
		memset(m_mission_create_npc, 0, sizeof(m_mission_create_npc));
		clear_data();
	}

	cmission::~cmission()
	{}

	void cmission::heart_tick(const int64& new_time)
	{
		//check_mission_state();
		//check_self_see_npc();
	}

	void cmission::clear_data()
	{ 
		m_mission_info.clear_data();
		m_mission_ptr = nullptr;
		clear_mission_create_npcs();
	}

	void cmission::init_mission_by_template(int32 mission_id, int32 mission_slot, player* player_ptr)
	{
		if (nullptr == player_ptr)
		{
			return;
		}
		MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, mission_id);
		if (nullptr == mission_ptr)
		{
			return;
		}

		int32 old_mission_id = m_mission_info.data_ary[e_mission_inst_data_id];

		//初始化
		m_mission_info.clear_data();
		m_mission_info.data_ary[e_mission_inst_data_id] = mission_id;
		m_mission_info.data_ary[e_mission_inst_type] = mission_ptr->MissionShowType;
		m_mission_info.data_ary[e_mission_inst_get_time] = time_helper::get_cur_time_new().second;
		switch (mission_ptr->MissionShowType)
		{
		case e_mission_type_main:
			m_mission_info.data_ary[e_mission_inst_slot] = e_mission_slot_main;
			break;
		case e_mission_type_daily:
			m_mission_info.data_ary[e_mission_inst_slot] = e_mission_slot_daily;
			break;
		case e_mission_type_against:
			m_mission_info.data_ary[e_mission_inst_slot] = e_mission_slot_against;
			break;
		case e_mission_type_side:
			m_mission_info.data_ary[e_mission_inst_slot] = mission_slot;
			break;
		case e_mission_type_grade_up:
			m_mission_info.data_ary[e_mission_inst_slot] = e_mission_slot_grade_up;
			break;
		case e_mission_type_marry:
			m_mission_info.data_ary[e_mission_inst_slot] = e_mission_slot_extra_side_0;
			break;
		default:
			break;
		}
		
		if (player_ptr->get_write_log())
		{
			server_log::gettask_role_log(player_ptr->get_third_info(), player_ptr->get_unit_info_inst(), mission_id, 1, mission_ptr->MissionShowType, player_ptr->get_login_type());

			set_log_var(log_head);

			player_ptr->get_log_common_head_info(log_head);

			server_log::serverTask(log_head, init_unit::change_i32_to_string(mission_id), init_unit::change_i32_to_string(mission_ptr->MissionShowType), 0, 0);
		}

		init_mission_by_info(m_mission_info, player_ptr);
		if (m_mission_ptr == nullptr)
		{
			return;
		}

		switch (m_mission_ptr->MissionShowType)
		{
		case e_mission_type_daily:
		case e_mission_type_against:
			m_player_ptr->get_team_cs_mgr().check_update_team_mission();
			break;
		default:
			break;
		}
		if (m_mission_ptr->TemporaryActivateWing.size() > 0)
		{
			active_temporary_wing(true);
		}
		check_self_see_npc();
	}

	void cmission::init_mission_by_info(s_mission_info mission_info, player* player_ptr)
	{
		if (player_ptr == nullptr)
		{
			return;
		}
		for (int32 info_index = 0; info_index < e_mission_inst_data_max; info_index++)
		{
			if (set_inst_data(info_index, mission_info.data_ary[info_index]) == false)
			{
				return;
			}
		}
		MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, mission_info.data_ary[e_mission_inst_data_id]);
		if (nullptr == mission_ptr)
		{
			return;
		}
		//初始化
		m_player_ptr = player_ptr;
		m_mission_ptr = mission_ptr;
		clear_mission_create_npcs();
		can_accept_mission();
		accept_mission();

		if (get_mission_state() < e_mission_state_done)
		{
			switch (m_mission_ptr->MissionEndType)
			{
			case e_mission_end_type_dialog:
			case e_mission_end_type_transfer:
				set_mission_state(e_mission_state_done);
				break;
			}
		}

		std::vector<s_item_template_info> temp_list_with_num = cdrop::mission_drop(mission_ptr, m_player_ptr);
		for (int32 item_index = 0; item_index < temp_list_with_num.size();item_index ++)
		{
			if (item_index > MAX_AWARD_ITEM )
			{
				break;
			}
			set_inst_data(e_mission_inst_data_finish_item_id1 + item_index * 2, temp_list_with_num[item_index].m_item_id);
			set_inst_data(e_mission_inst_data_finish_item_num1 + item_index * 2, temp_list_with_num[item_index].m_item_num);
		}

		//check mission state
		target_check((e_mission_end_type)m_mission_ptr->MissionEndType, -1, -1, false);
		//check_special_mission_history_record();
		check_mission_state();
		check_self_see_npc();
	}

	bool cmission::accept_mission()
	{
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		if (nullptr == m_mission_ptr)
		{
			return false;
		}
		//s_map_pos now_position = m_player_ptr->get_old_map_pos();
		//if (m_mission_ptr->MissionShowType == e_mission_show_type_main
		//	&& init_unit::get_distance(now_position.posX, now_position.posY, now_position.posZ
		//		, m_mission_ptr->GetMissionNpcLocation[0]
		//		, m_mission_ptr->GetMissionNpcLocation[1]
		//		, m_mission_ptr->GetMissionNpcLocation[2]) > MIN_DISTENCE_TO_DONE_MISSION)
		//{//主线任务距离过远
		//	return false;
		//}
		if (get_mission_state() == e_mission_state_can_accept)
		{//接取
			set_mission_state(e_mission_state_accepted);
			return true;
		}
		return false;
	}

	int32 cmission::finish_mission(int32 npc_id, e_mission_finish_type finish_type)
	{
		if (nullptr == m_player_ptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}
		if (nullptr == m_mission_ptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}
		int32 end_state = e_mission_operate_end_state_system_wrong;
		end_state = finish_pre_proc(finish_type);
		if (end_state != e_mission_operate_end_state_succeed)
		{
			return end_state;
		}

		std::vector<s_item_template_info> item_get_array;
		std::vector<s_item_template_info> money_get_array;
		std::vector<s_item_template_info> money_cost_array;

		fill_mission_reward_and_cost_array(item_get_array, money_get_array, money_cost_array, finish_type);

		end_state = can_finish_judge(item_get_array, money_get_array, money_cost_array, npc_id);
		if (end_state != e_mission_operate_end_state_succeed)
		{
			return end_state;
		}
		end_state = get_reward_and_cost(item_get_array, money_get_array, money_cost_array);
		if (end_state != e_mission_operate_end_state_succeed)
		{
			return end_state;
		}
		end_state = finish_end_proc(finish_type, item_get_array);


		if (m_mission_ptr->MissionShowType == e_mission_slot_main)
		{
			cmission_mgr& mission_mgr_ref = m_player_ptr->get_mission_mgr();
			mission_mgr_ref.target_check(e_mission_end_type_unlock_skill_by_mission);
		}
		if (m_mission_ptr->TemporaryActivateWing.size() > 0)
		{
			active_temporary_wing(false);
		}
		return end_state;
	}

	int32 cmission::finish_pre_proc(int32 finish_type)
	{
		if (m_mission_ptr == nullptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		switch (finish_type)
		{
		case e_mission_finish_type_double:
		{

		}
			break;
		case e_mission_finish_type_endall:
		{
			if (m_mission_ptr->MissionShowType == e_mission_type_daily)
			{
				set_mission_state(e_mission_state_can_finish);
			}	set_inst_data(e_mission_inst_data_star_level, GAMECONFIG->DailyMissionMaxStarLevel);
		}
			break;
		default:
			break;
		}
		return e_mission_operate_end_state_succeed;
	}

	int32 cmission::fill_mission_reward_and_cost_array(std::vector<s_item_template_info>& item_reward_array,
		std::vector<s_item_template_info>& money_reward_array,
		std::vector<s_item_template_info>& money_cost_array,
		int32 finish_type)
	{
		if (m_player_ptr == nullptr
			|| m_mission_ptr == nullptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		if (GAMECONFIG->DailyMissionEndAllCost.size() < 2
			|| GAMECONFIG->DailyMissionDoubleCost.size() < 2)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		item_reward_array.clear();
		money_reward_array.clear();
		money_cost_array.clear();

		int32 remain_daily_mission_num = DAILY_MISSION_MAX - m_player_ptr->get_unit_info(e_role_info_daily_done_num);
		int32 mission_finish_times = 1;
		if (m_mission_ptr->MissionShowType == e_mission_type_daily
			&& finish_type == e_mission_finish_type_endall)
		{
			mission_finish_times = remain_daily_mission_num;
		}


		// 正常任务奖励
		for (int32 item_index = 0; item_index < MAX_AWARD_ITEM * 2; item_index += TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN)
		{
			int32 award_item_id = get_inst_data(e_mission_inst_data_finish_item_id1 + item_index);
			int32 award_item_num = get_inst_data(e_mission_inst_data_finish_item_num1 + item_index);
			award_item_num *= mission_finish_times;
			if (award_item_id <= 0
				|| award_item_num <= 0)
			{
				continue;
			}
			//任务给的东西都是绑定的
			init_unit::merge_item(item_reward_array, award_item_id, award_item_num);
		}
		int32 world_base_exp = m_player_ptr->get_cur_player_world_level_exp();

		init_unit::get_money_array_by_mission_id(
			get_inst_data(e_mission_inst_data_id), 
			m_player_ptr->get_unit_info(e_role_info_upgrade_id), 
			get_inst_data(e_mission_inst_data_star_level), 
			mission_finish_times,
			money_reward_array, world_base_exp);
	
		switch (finish_type)
		{
		case e_mission_finish_type_double:
		{
			if (get_inst_data(e_mission_inst_type) == e_mission_type_daily)
			{
				int32 cost_money_id = GAMECONFIG->DailyMissionDoubleCost[0];
				int32 cost_money_value = GAMECONFIG->DailyMissionDoubleCost[1];
				init_unit::merge_money_to_two_tuples(money_cost_array, cost_money_id, cost_money_value);
				//for (int32 i = 0; i < money_reward_array.size(); i += e_money_tuple_max)
				//{
				//	if (money_reward_array[i + e_money_tuple_id] == e_money_type_exp)
				//	{
				//		money_reward_array[i + e_money_tuple_num] *= 2;
				//	}
				//}
			}
		}
			break;
		case e_mission_finish_type_endall:
			if (get_inst_data(e_mission_inst_type) == e_mission_type_daily)
			{
				int32 cost_money_id = GAMECONFIG->DailyMissionEndAllCost[0];
				int32 cost_money_value = GAMECONFIG->DailyMissionEndAllCost[1] * remain_daily_mission_num;
				init_unit::merge_money_to_two_tuples(money_cost_array, cost_money_id, cost_money_value);
				//for (int32 i = 0; i < money_reward_array.size(); i += e_money_tuple_max)
				//{
				//	if (money_reward_array[i + e_money_tuple_id] == e_money_type_exp)
				//	{
				//		money_reward_array[i + e_money_tuple_num] *= 2;
				//	}
				//}
			}
			break;
		default:
			break;
		}

		//以下为日常、讨伐全部完成奖励
		if (!is_last_daily_or_against()
			&& finish_type != e_mission_finish_type_endall)
		{
			return e_mission_operate_end_state_succeed;
		}
		int32 all_done_reward_id = 0;
		PlayerUpgradeTemplate* player_upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, m_player_ptr->get_unit_info(e_role_info_upgrade_id));
		if (player_upgrade_template_ptr == nullptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}
		
		if (get_inst_data(e_mission_inst_type) == e_mission_type_daily)
		{
			all_done_reward_id = player_upgrade_template_ptr->DailyMissionLibrary;
		}
		else if(get_inst_data(e_mission_inst_type) == e_mission_type_against)
		{
			all_done_reward_id = player_upgrade_template_ptr->AgainstMissionLibrary;
		}

		MissionLibraryTemplate* mission_library_template_ptr = GET_TEMPLATE(MissionLibraryTemplate, all_done_reward_id);
		if (mission_library_template_ptr == nullptr
			|| mission_library_template_ptr->AllDoneitemShow.size() % TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN != 0)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		for (int32 i = 0; i < mission_library_template_ptr->AllDoneitemShow.size(); i += TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN)
		{
			int32 item_id = mission_library_template_ptr->AllDoneitemShow[i];
			int32 item_num = mission_library_template_ptr->AllDoneitemShow[i + 1];
			init_unit::merge_item(item_reward_array, item_id, item_num);
		}

		init_unit::get_money_array_by_mission_library_id(all_done_reward_id, m_player_ptr->get_unit_info(e_role_info_upgrade_id), money_reward_array, world_base_exp);
		return e_mission_operate_end_state_succeed;
	}

	bool cmission::is_last_daily_or_against()
	{
		if (m_player_ptr == nullptr)
		{
			return false;
		}
		switch (get_inst_data(e_mission_inst_type))
		{
		case e_mission_type_daily:
			return (DAILY_MISSION_MAX - m_player_ptr->get_unit_info(e_role_info_daily_done_num) == 1);
			break;
		case e_mission_type_against:
			return (AGAINST_MISSION_MAX - m_player_ptr->get_unit_info(e_role_info_against_done_num) == 1);
			break;
		case e_mission_type_side:
			break;
		default:
			return false;
			break;
		}
		return false;
	}

	int32 cmission::can_finish_judge(const std::vector<s_item_template_info>& item_reward_array,
		const std::vector<s_item_template_info>& money_reward_array,
		const std::vector<s_item_template_info>& money_cost_array, int32 npc_id)
	{
		if (nullptr == m_player_ptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}
		if (nullptr == m_mission_ptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		if (get_mission_state() != e_mission_state_can_finish && get_mission_state() != e_mission_state_chat
			&& m_mission_ptr->MissionEndType != e_mission_end_type_transfer && m_mission_ptr->MissionEndType != e_mission_end_type_dialog)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		//距离检测
		const s_map_pos& now_position = m_player_ptr->get_old_map_pos();
		if (m_mission_ptr->FinishMissionNpcId > 0)
		{
			//主线操作的npc不是任务对应的npc
			NpcSpawnPointTemplate* finish_spawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, m_mission_ptr->FinishMissionNpcId);
			if (finish_spawn_template_ptr == nullptr)
			{
				return e_mission_operate_end_state_system_wrong;
			}
			if (finish_spawn_template_ptr->TemplateId != npc_id)
			{
				return e_mission_operate_end_state_system_wrong;
			}
			if (m_mission_ptr->MissionShowType == e_mission_type_main
				&& m_mission_ptr->NeedLevel > m_player_ptr->get_unit_info(e_role_info_exp_level))
			{//判断是否达到主线任务要求等级
				return -1;//不提示错误 直接显示等级不足无法完成
			}

			fvector npc_location(finish_spawn_template_ptr->PosX, finish_spawn_template_ptr->PosY, finish_spawn_template_ptr->PosZ);
			if (m_mission_ptr->MissionEndType == e_mission_end_type_transfer && m_mission_ptr->TransferPos.size() > 0 && m_mission_ptr->TransferPos.size() % 3 == 0)
			{
				const s_map_pos& cur_pos = m_player_ptr->get_new_map_pos();
				if ((m_mission_ptr->TransferPos[0] - 200 <= cur_pos.unit_location.x && cur_pos.unit_location.x <= m_mission_ptr->TransferPos[0] + 200)
					&& (m_mission_ptr->TransferPos[1] - 200 <= cur_pos.unit_location.y && cur_pos.unit_location.y <= m_mission_ptr->TransferPos[1] + 200)
					&& (m_mission_ptr->TransferPos[2] - 200 <= cur_pos.unit_location.z && cur_pos.unit_location.z <= m_mission_ptr->TransferPos[2] + 200))
				{
				}
				else
				{
					return e_mission_operate_end_state_too_far;
				}
			}
			else
			{
				if (m_mission_ptr->MissionShowType == e_mission_type_main && (now_position.unit_location - npc_location).length_2d() > MIN_DISTENCE_TO_DONE_MISSION)
				{//是主线任务但是距离完成npc距离不够近
					return e_mission_operate_end_state_too_far;
				}
			}
		}

		//只有主线任务必须背包有格子
		if (m_mission_ptr->MissionShowType == e_mission_type_main)
		{
			int32 item_bag_slot_req_num = 0;
			int32 rune_bag_slot_req_num = 0;
			for (int32 i = 0; i < item_reward_array.size(); i++)
			{
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_reward_array[i].m_item_id);
				if (nullptr == item_template_ptr)
				{
					continue;
				}
				if (item_template_ptr->item_type == e_item_type_element_heart)
				{
					rune_bag_slot_req_num++;
				}
				else
				{
					item_bag_slot_req_num++;
				}
			}
			if (!m_player_ptr->get_item_set().is_empty_slot_enough(e_bag_type_bag, item_bag_slot_req_num))
			{
				return e_mission_operate_end_state_bag_full;
			}
			if (!m_player_ptr->get_item_set().is_empty_slot_enough(e_bag_type_elemenet, rune_bag_slot_req_num))
			{
				return e_mission_operate_end_state_bag_full;
			}
		}

		//花钱判断
		for (auto& it : money_cost_array)
		{
			if (!m_player_ptr->can_cut_money(e_money_type(it.m_item_id), it.m_item_num))
			{
				return e_mission_operate_end_state_not_enough_money;
			}
		}

		return e_mission_operate_end_state_succeed;
	}
	int32 cmission::get_reward_and_cost(
		const std::vector<s_item_template_info>& item_reward_array,
		const std::vector<s_item_template_info>& money_reward_array,
		const std::vector<s_item_template_info>& money_cost_array)
	{
		if (nullptr == m_player_ptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}
		if (nullptr == m_mission_ptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}

		//扣钱
		for (auto& it : money_cost_array)
		{
			if (!m_player_ptr->can_cut_money((e_money_type)it.m_item_id, it.m_item_num))
			{
				return e_mission_operate_end_state_not_enough_money;
			}
		}

		//扣钱
		for (auto& it : money_cost_array)
		{
			m_player_ptr->cut_money((e_money_type)it.m_item_id, it.m_item_num, e_server_log_cut_money_finish_mission, get_inst_data(e_mission_inst_data_id));
		}

		//给钱
		for (auto& it : money_reward_array)
		{
			m_player_ptr->add_money_or_exp(e_money_type(it.m_item_id), it.m_item_num, e_server_log_add_money_finish_mission, get_inst_data(e_mission_inst_data_id));
		}

		//给物品
		std::string add_item_cause_id;
		std::stringstream add_item_cause_id_stream;
		add_item_cause_id_stream << int32(e_server_log_add_item_finish_mission);
		add_item_cause_id_stream >> add_item_cause_id;

		std::string add_item_log_location;
		std::stringstream add_item_location_stream;
		add_item_location_stream << int32(get_inst_data(e_mission_inst_data_id));
		add_item_location_stream >> add_item_log_location;

		const char* mail_title_char = nullptr;
		const char* mail_content_char = nullptr;
		switch (m_mission_ptr->MissionShowType)
		{
		case e_mission_type_daily:
			mail_title_char = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_daily_mission_bag_full_title);
			mail_content_char = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_daily_mission_bag_full_context);
			break;
		case e_mission_type_against:
			mail_title_char = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_against_mission_bag_full_title);
			mail_content_char = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_against_mission_bag_full_context);
			break;
		default:
			mail_title_char = "90303013";
			mail_content_char = "90303013";
			break;
		}
		item_system::put_item_into_bag(m_player_ptr, item_reward_array);
		m_player_ptr->send_promp_msg_to_client(item_reward_array, money_reward_array);
		return e_mission_operate_end_state_succeed;
	}
	int32 cmission::finish_end_proc(int32 finish_type, const std::vector<s_item_template_info>& item_reward_array)
	{
		if (m_player_ptr == nullptr
			|| m_mission_ptr == nullptr)
		{
			return e_mission_operate_end_state_system_wrong;
		}
		//要先设置，后续逻辑有依赖，比如 func_unlock
		set_mission_state(e_mission_state_finished);

		if (m_player_ptr->get_write_log())
		{
			server_log::finishtask_role_log(m_player_ptr->get_third_info(), m_player_ptr->get_unit_info_inst(), get_mission_id(), 1, m_mission_ptr->MissionShowType, m_player_ptr->get_login_type());

			set_log_var(log_head)

			m_player_ptr->get_log_common_head_info(log_head);

			server_log::serverTask(log_head, init_unit::change_i32_to_string(get_mission_id()), init_unit::change_i32_to_string(m_mission_ptr->MissionShowType), 1,0);
			
		}

		switch (m_mission_ptr->MissionShowType)
		{
		case e_mission_type_main:
		{
			//map_record_mgr::get_instance().unlock_map(m_player_ptr->get_array_index(), get_mission_id()); //map表的Preconditions只是地图id
			m_player_ptr->get_achievement_mgr().mission_finish(this);
			m_player_ptr->get_func_unlock_mgr().func_unlock_trigger_activate();
		}
			break;
		case e_mission_type_daily:
		{
			if (finish_type == e_mission_finish_type_endall)
			{
				m_player_ptr->set_unit_info(e_role_info_daily_done_num, DAILY_MISSION_MAX);

				m_player_ptr->get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_ten_daily_mission, DAILY_MISSION_MAX);
				m_player_ptr->set_must_do_count(e_daily_must_do_typ_raid_daily, DAILY_MISSION_MAX);
				m_player_ptr->get_recycle_mgr().on_event(e_recycle_task_type_hunter_3, DAILY_MISSION_MAX);

			}
			else
			{
				m_player_ptr->set_unit_info(e_role_info_daily_done_num, m_player_ptr->get_unit_info(e_role_info_daily_done_num) + 1);

				int32 cur_daily_mission_num = m_player_ptr->get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_ten_daily_mission);
				m_player_ptr->get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_ten_daily_mission, cur_daily_mission_num + 1);
				m_player_ptr->set_must_do_count(e_daily_must_do_typ_raid_daily, m_player_ptr->get_must_do_count(e_daily_must_do_typ_raid_daily) + 1);
				m_player_ptr->get_recycle_mgr().on_event(e_recycle_task_type_hunter_3);
			}
			m_player_ptr->send_info_one(e_role_info_daily_done_num);
			m_player_ptr->send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_daily);
			m_player_ptr->set_daily_active_degree_info(e_daily_must_do_typ_raid_daily);
			//m_player_ptr->get_team_cs_mgr().give_captain_reward();
			m_player_ptr->send_promp_msg_to_client(item_reward_array);
			//special_mission_update(e_mission_end_type_clear_daily_mission, -1, -1);
		}
			break;
		case e_mission_type_against:
		{
			if (finish_type == e_mission_finish_type_endall)
			{
				m_player_ptr->set_unit_info(e_role_info_against_done_num, AGAINST_MISSION_MAX);
				m_player_ptr->set_must_do_count(e_daily_must_do_typ_crusade_mission, AGAINST_MISSION_MAX);
				m_player_ptr->get_recycle_mgr().on_event(e_recycle_task_type_crusade_4, AGAINST_MISSION_MAX);

			}
			else
			{
				m_player_ptr->set_unit_info(e_role_info_against_done_num, m_player_ptr->get_unit_info(e_role_info_against_done_num) + 1);
				m_player_ptr->set_must_do_count(e_daily_must_do_typ_crusade_mission, m_player_ptr->get_must_do_count(e_daily_must_do_typ_crusade_mission) + 1);
				m_player_ptr->get_recycle_mgr().on_event(e_recycle_task_type_crusade_4);

			}
			m_player_ptr->set_daily_active_degree_info(e_daily_must_do_typ_crusade_mission);
			m_player_ptr->send_info_one(e_role_info_against_done_num);
			m_player_ptr->send_daily_must_do_count_info_one(e_daily_must_do_typ_crusade_mission);
			//m_player_ptr->get_team_cs_mgr().give_captain_reward();
			m_player_ptr->send_promp_msg_to_client(item_reward_array);
		}
			break;
		case e_mission_type_side:
		{

		}
			break;
		case e_mission_type_grade_up:
		{
			m_player_ptr->check_can_done_grade_mission(get_inst_data(e_mission_inst_data_id));
		}
			break;
		case e_mission_type_marry:
		{
			if (finish_type == e_mission_finish_type_endall)
			{
				m_player_ptr->set_unit_info(e_role_info_marry_done_num, MARRY_MISSION_MAX);
			}
			else
			{
				m_player_ptr->set_unit_info(e_role_info_marry_done_num, m_player_ptr->get_unit_info(e_role_info_marry_done_num) + 1);
			}
			m_player_ptr->send_info_one(e_role_info_marry_done_num);
			m_player_ptr->get_item_set().get_item_send_promp_msg_to_client(item_reward_array);
		}
			break;
		default:
			break;
		}

		// 任何任务完成都有可能解锁新的一条龙目标 日常或讨伐任务完成可能改变新的一条龙目标
		m_player_ptr->get_team_cs_mgr().check_one_stop_finish_flags();
		m_player_ptr->get_team_cs_mgr().check_new_aim_for_one_stop(false);

		mission_proto_mission_finish mission_finish_msg;
		mission_finish_msg.set_mission_id(m_mission_ptr->attribute_id);
		m_player_ptr->send_message_to_self(&mission_finish_msg, e_msgindex_s2c_mission_finish);

		for (int32 i = 0; i < m_mission_ptr->FinishTriggerBuffArray.size(); ++i)
		{
			buff_man::add_buff_inst(m_player_ptr->get_array_index(), m_player_ptr->get_array_index(), m_mission_ptr->FinishTriggerBuffArray[i]);
		}

		return e_mission_operate_end_state_succeed;
	}

	int32 cmission::get_mission_end_type()
	{
		if (nullptr == m_mission_ptr)
		{
			return -1;
		}
		else
		{
			return m_mission_ptr->MissionEndType;
		}
	}

	int32 cmission::get_inst_data(int32 index)
	{
		if (index >= e_mission_inst_data_id &&  index < e_mission_inst_data_max)
		{
			return m_mission_info.data_ary[index];
		}
		else return -1;
	}

	bool cmission::set_inst_data(int32 index, int32 num)
	{
		if (index >= e_mission_inst_data_id && index < e_mission_inst_data_max)
		{
			m_mission_info.data_ary[index] = num;
			return true;
		}
		return false;
	}

	bool cmission::set_mission_state(e_mission_state mission_state)
	{
		check_self_see_npc();
		if (mission_state < e_mission_state_max)
		{
			m_mission_info.data_ary[e_mission_inst_data_mission_state] = mission_state;
			if (mission_state == e_mission_state_done
				|| mission_state == e_mission_state_chat
				|| mission_state == e_mission_state_can_finish)
			{
				operate_npc_show_by_state();
			}
			return true;
		}
		return false;
	}

	void cmission::create_npc()
	{
		if (nullptr == m_mission_ptr || nullptr == m_player_ptr || m_player_ptr->is_valid() == false)
		{
			return;
		}

		// 已经达成了交付任务的条件就不再刷NPC
		if (is_finish_mission_when_have_mission_create_npc() == true)
		{
			return;
		}

		const int32 len = m_mission_ptr->CreateNpc.size();
		if (len <= 0 || len % TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN)
		{
			return;
		}
		
		int32 create_npc_count = 0;
		int32 spawn_id = 0;
		int32 need_num = 0;
		for (int32 i = 0; i < len; i += TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN)
		{
			spawn_id = m_mission_ptr->CreateNpc[i];
			need_num = m_mission_ptr->CreateNpc[i + 1];

			// 检查表格数据是否有配置错误
			NpcSpawnPointTemplate* spawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_id);
			if (nullptr == spawn_template_ptr)
			{
				continue;
			}
			
			// 已经达成的目标就不再刷对应的NPC
			const int32 goal_index = i / TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN;
			const int32 cur_mission_goal_num = get_cur_mission_goal_num(goal_index);
			if (cur_mission_goal_num >= need_num)
			{
				continue;
			}

			// 已经有刷出的NPC就不再重复刷
			if (exist_mission_create_npc_by_goal_index(goal_index) == true)
			{
				continue;
			}

			// 刷出需要的NPC
			auto map_type = base_map_system::get_map_type(m_player_ptr->get_map_ent());
			if (map_type != faith::e_map_type_big_map)
			{
				continue;
			}

			npc* spawn_npc_ptr = world_cs::spawn_npc(spawn_id, m_player_ptr->get_map_ent());
			if (nullptr == spawn_npc_ptr)
			{
				continue;
			}
			

			const int32 spawn_npc_array_index = spawn_npc_ptr->get_array_index();
			add_mission_create_npc(spawn_npc_array_index);

			++create_npc_count;
		}

		if (create_npc_count > 0)
		{
			send_mission_create_npcs_info();
		}
	}

	int32 cmission::get_mission_id()
	{
		return get_inst_data(e_mission_inst_data_id);
	}

	bool cmission::can_accept_mission()
	{
		if (nullptr == m_mission_ptr)
		{
			return false;
		}
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		if (get_mission_state() == e_mission_state_can_accept)
		{//已判断完成
			return true;
		}
		if (get_mission_state() > e_mission_state_can_accept)
		{
			return false;
		}
		set_mission_state(e_mission_state_can_accept);
		return true;
	}

	bool cmission::check_mission_state()
	{
		if (nullptr == m_mission_ptr)
		{
			return false;
		}
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		if (get_mission_state() >= e_mission_state_can_finish)
		{//已可交付
			return true;
		}
		if (m_mission_ptr->MissionShowType >= e_mission_type_main && get_mission_state() == e_mission_state_done)
		{
			set_mission_state(e_mission_state_chat);
		}
		if (m_mission_ptr->MissionShowType == e_mission_type_main
			&& m_player_ptr->get_unit_info(e_role_info_exp_level) < m_mission_ptr->NeedLevel)
		{//主线任务查查等级
			return false;
		}
		if (get_mission_state() == e_mission_state_chat)
		{//对话完成
			set_mission_state(e_mission_state_can_finish);
		}

		switch (m_mission_ptr->MissionEndType)
		{
		case e_mission_end_type_transfer: //传送
		case e_mission_end_type_dialog://对话
		case e_mission_end_type_mission://章节
		{
			if (get_mission_state() > e_mission_state_done)
			{
				set_mission_state(e_mission_state_can_finish);
				return true;
			}
			return false;
		}
		break;
		case e_mission_end_type_reach_level://达到下一个任务等级
		{
			MissionTemplate* next_mission_ptr = GET_TEMPLATE(MissionTemplate, m_mission_ptr->NextMissionId);
			if (nullptr == next_mission_ptr)
			{
				return false;
			}
			if (m_player_ptr->get_unit_info(e_role_info_exp_level) >= next_mission_ptr->NeedLevel)
			{
				set_mission_state(e_mission_state_can_finish);
				if (m_mission_ptr->attribute_id == first_recharge_time_mission_id)
				{
					m_player_ptr->set_time_data(e_time_type_first_recharge_open_time, time_helper::get_cur_time_new().second);//这个地方需要写死
					m_player_ptr->send_time_one(e_time_type_first_recharge_open_time);
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
		case e_mission_end_type_unlock_skill_by_mission:
		{
			if (m_mission_ptr->GoalNpcKill.size() < 11)
			{
				return false;
			}
			if (m_mission_ptr->GoalNpcKill[0] < m_mission_ptr->GoalNpcKill[10])
			{
				return false;
			}
			int32 mission_id = m_mission_ptr->GoalNpcKill[10];
			for (int32 i = 0; i < m_mission_ptr->GoalNpcKill[0] - m_mission_ptr->GoalNpcKill[10]; i++)
			{
				if (mission_id == m_mission_ptr->GoalNpcKill[0])
				{
					if (get_inst_data(e_mission_inst_data_goal1_num) >= i)
					{
						set_mission_state(e_mission_state_can_finish);
						return true;
					}
					return false;
				}
				MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, mission_id);
				if (nullptr != mission_ptr)
				{
					mission_id = mission_ptr->NextMissionId;
				}
			}
		}
		break;
		default:
		{
			if (m_mission_ptr->GoalNpcKill.size() < 2)
			{
				return false;
			}
			if (get_inst_data(e_mission_inst_data_goal1_num) >= m_mission_ptr->GoalNpcKill[1])
			{
				set_mission_state(e_mission_state_can_finish);
				return true;
			}
			return false;
		}
		break;
		}
		return false;
	}

	const bool cmission::is_mission_target(const int32 npc_template_id)
	{
		const int32 goal_index = get_goal_index(npc_template_id);
		if (is_valid_goal_index(goal_index) == false)
		{
			return false;
		}
		return true;
	}

	const int32 cmission::get_mission_goal_npc_kill_id(const int32 goal_index /*= 0*/)
	{
		if (nullptr == m_mission_ptr)
		{
			return -1;
		}
		const int array_size = m_mission_ptr->GoalNpcKill.size();
		if (0 != array_size % 2)
		{
			return -1;
		}
		const int32 array_index = goal_index * 2;
		if (array_index >= array_size)
		{
			return -1;
		}
		return m_mission_ptr->GoalNpcKill[array_index];
	}

	void cmission::change_mission(const int32 mission_template_id)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, mission_template_id);
		if (nullptr == mission_ptr
			|| mission_ptr->MissionShowType != get_inst_data(e_mission_inst_type))
		{
			return;
		}

		//const int32 current_schedule = get_inst_data(e_mission_inst_data_goal1_num);
		//const int32 current_star = get_inst_data(e_mission_inst_data_star_level);
		set_inst_data(e_mission_inst_data_id, mission_template_id);
		m_mission_ptr = mission_ptr;
		std::vector<s_item_template_info> temp_list_with_num = cdrop::mission_drop(mission_ptr, m_player_ptr);
		for (int32 item_index = 0; item_index < temp_list_with_num.size(); item_index++)
		{
			if (item_index > MAX_AWARD_ITEM)
			{
				break;
			}
			set_inst_data(e_mission_inst_data_finish_item_id1 + item_index * 2, temp_list_with_num[item_index].m_item_id);
			set_inst_data(e_mission_inst_data_finish_item_num1 + item_index * 2, temp_list_with_num[item_index].m_item_num);
		}

	}

	bool cmission::is_valid_goal_index(const int32 goal_index)
	{
		if (goal_index < 0 || MAX_MISSION_LINE_NUM <= goal_index)
		{
			return false;
		}
		return true;
	}

	const int32 cmission::get_goal_index(const int32 killed_npc_template_id)
	{
		if (nullptr == m_mission_ptr)
		{
			return -1;
		}

		std::vector<int32>* goal_arr = nullptr;
		const e_mission_end_type mission_end_type = (e_mission_end_type)(m_mission_ptr->MissionEndType);
		switch (mission_end_type)
		{
		case e_mission_end_type_npc_kill:
		case e_mission_end_type_kill_monster_get_item:
		case e_mission_end_type_get_item:
		case e_mission_end_type_npc_attacked_num:
		case e_mission_end_type_kill_home_boss:
			goal_arr = &(m_mission_ptr->GoalNpcKill);
			break;
		case e_mission_end_type_beat_npc:
			goal_arr = &(m_mission_ptr->CreateNpc);
			break;
		}

		if (nullptr == goal_arr)
		{
			return -1;
		}

		const int32 arr_len = goal_arr->size();
		if (arr_len <= 0 || arr_len % e_mission_goal_tuple_max != 0)
		{
			return -1;
		}

		int32 goal_id = 0;
		for (int32 i = 0; i < arr_len; i += e_mission_goal_tuple_max)
		{
			goal_id = (*goal_arr)[i + e_mission_goal_tuple_id];
			if (mission_end_type == e_mission_end_type_beat_npc)
			{
				NpcSpawnPointTemplate* spawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, goal_id);
				if (nullptr == spawn_template_ptr)
				{
					return -1;
				}
				goal_id = spawn_template_ptr->TemplateId;
			}

			if (killed_npc_template_id == goal_id)
			{
				return (i / e_mission_goal_tuple_max);
			}
		}

		return -1;
	}

	void cmission::set_cur_mission_goal_num(const int32 goal_index, const int32 value)
	{
		if (is_valid_goal_index(goal_index) == false)
		{
			return;
		}

		const int32 real_goal_index = goal_index + e_mission_inst_data_goal1_num;
		set_inst_data(real_goal_index, value);
	}

	int32 cmission::get_cur_mission_goal_num(const int32 goal_index)
	{
		if (is_valid_goal_index(goal_index) == false)
		{
			return 0;
		}

		int32 goal_cur_num = get_inst_data(goal_index + e_mission_inst_data_goal1_num);
		return goal_cur_num;
	}

	int32 cmission::get_cur_reached_num(int32 target_info_0, int32 target_info_1)
	{
		int32 cur_reached_num = 0;
		if (m_mission_ptr == nullptr
			|| m_player_ptr == nullptr)
		{
			return cur_reached_num;
		}
		switch (m_mission_ptr->MissionEndType)
		{
		case e_mission_end_type_equip_levelup:
		case e_mission_end_type_add_friend:
		case e_mission_end_type_dialog:
		case e_mission_end_type_transfer:
		case e_mission_end_type_join_legion_boss_map:
		case e_mission_end_type_boss_field:
		case e_mission_end_type_marry_dance:
		case e_mission_end_type_spirit_baptism:
		case e_mission_end_type_mount_upgrade_num:
		case e_mission_end_type_spirit_upgrade_num:
		//case e_mission_end_type_auction_goods:
		case e_mission_end_type_assis_fight_num:
		//case e_mission_end_type_auction_shopping:
		case e_mission_end_type_transform:
		case e_mission_end_type_kill_any_home_boss:
		case e_mission_end_type_kill_any_single_boss:
		case e_mission_end_type_add_dot:
		case e_mission_end_type_intensify_add_gs:
		{
			//调用一次固定+1
			cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num) + 1;
		}
		break;
		case e_mission_end_type_equip_upgrade:
		case e_mission_end_type_equip_addon:
		case e_mission_end_type_choose_a_fashion:
		{
			//传啥是啥
			cur_reached_num = target_info_0;
		}
		break;
		case e_mission_end_type_clear_demon_tower:
		{
			cur_reached_num = m_player_ptr->get_highest_record(e_role_history_highest_record_demon_tower_highest_level);
		}
		break;
		case e_mission_end_type_npc_kill:
		case e_mission_end_type_beat_npc:
		case e_mission_end_type_get_item:
		case e_mission_end_type_mission:
		case e_mission_end_type_npc_attacked_num:
		case e_mission_end_type_kill_home_boss:
		case e_mission_end_type_new_map_finish:
		{
			//匹配上了就+1
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				if (target_info_0 == m_mission_ptr->GoalNpcKill[0])
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num) + 1;
				}
				else
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num);
				}
			}
		}
		break;
		case e_mission_end_type_map:
		{
			//大于等于当前地图就认为任务完成
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				if (target_info_0 >= m_mission_ptr->GoalNpcKill[0])
				{
					cur_reached_num = 1;
				}
			}
		}
		break;
		case e_mission_end_type_kill_monster_get_item:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				int32 item_get_random_value = random_gen::get_random(1, 100);
				if (target_info_0 == m_mission_ptr->GoalNpcKill[0]
					&& item_get_random_value <= m_mission_ptr->FinishItemGetRate)
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num) + 1;
				}
				else
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num);
				}
			}
		}
		break;
		case e_mission_end_type_purchase_goods:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				if (target_info_1 <= 0)
				{
					target_info_1 = 1;
				}
				if (target_info_0 == m_mission_ptr->GoalNpcKill[0])
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num) + target_info_1;
				}
				else
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num);
				}

				int32 buy_good_num = m_player_ptr->get_goods_helper_mgr().get_goods_buy_count(m_mission_ptr->GoalNpcKill[0]);

				cur_reached_num = cur_reached_num > buy_good_num ? cur_reached_num : buy_good_num;
			}
		}
		break;
		case e_mission_end_type_rune_equip:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				//cur_reached_num = m_player_ptr->get_element_heart_mgr().get_current_equipped_heart_num(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_map_clear_history:
		{
			//if (m_mission_ptr->GoalNpcKill.size() >= 2)
			//{
			//	map_record& temp_record = map_record_mgr::get_instance().get_map_record(m_player_ptr->get_array_index(), m_mission_ptr->GoalNpcKill[0], 1);
			//	if (temp_record.is_valid())
			//	{
			//		cur_reached_num = temp_record.get_data_info(e_map_record_info_extra_enter_count) + temp_record.get_data_info(e_map_record_info_all_enter_count);
			//	}
			//}
		}
		break;
		case e_mission_end_type_map_clear_current:
		case e_mission_end_type_activity_clear_current:
		case e_mission_end_type_reach_level:
		{
			//if (m_mission_ptr->GoalNpcKill.size() >= 2)
			//{
			//	map_record& temp_record = map_record_mgr::get_instance().get_map_record(m_player_ptr->get_array_index(), m_mission_ptr->GoalNpcKill[0], 1);
			//	if (temp_record.is_valid())
			//	{
			//		cur_reached_num = temp_record.get_data_info(e_map_record_info_extra_enter_count) + temp_record.get_data_info(e_map_record_info_enter_count);
			//	}
			//}
		}
		break;
		case e_mission_end_type_mount_unlock:
		{
			cur_reached_num = m_player_ptr->get_item_set().get_total_mount_activate_num();
		}
		break;
		case e_mission_end_type_wing_upgrade:
		{
			cur_reached_num = m_player_ptr->get_item_set().get_wing_and_total_feather_grade_num();
		}
		break;
		case e_mission_end_type_wing_all_star:
		{
			cur_reached_num = m_player_ptr->get_item_set().get_wing_and_total_feather_star_num();
		}
		break;
		case e_mission_end_type_pokedex_activation:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cpokedex* cpokedex_ptr = m_player_ptr->get_pokedex_mgr().get_pokedex_by_templateid(m_mission_ptr->GoalNpcKill[0]);
				if (cpokedex_ptr != nullptr)
				{
					if (cpokedex_ptr->get_data_info(e_pokedex_Info_state) == 1)
					{
						cur_reached_num = 1;
					}
				}
			}
		}
		break;
		case e_mission_end_type_sprite_activation:
		{
			//cur_reached_num = m_player_ptr->get_spirit_mgr().num_of_jiban();
		}
		break;
		case e_mission_end_type_character_level:
		{
			cur_reached_num = m_player_ptr->get_unit_info(e_role_info_exp_level);
		}
		break;
		case e_mission_end_type_clear_daily_mission:
		{
			cur_reached_num = m_player_ptr->get_unit_info(e_role_info_daily_done_num);
		}
		break;
		case e_mission_end_type_clear_against_mission:
		{
			cur_reached_num = m_player_ptr->get_unit_info(e_role_info_against_done_num);
		}
		break;
		case e_mission_end_type_cheer_worship:
		{
			cur_reached_num = m_player_ptr->get_unit_info(e_role_info_worship_other_time);
		}
		break;
		/*case e_mission_end_type_transform:
		{
			cur_reached_num = m_player_ptr->get_convert_mgr().get_convert_time(e_convert_type_bind_siliver);
			cur_reached_num += m_player_ptr->get_convert_mgr().get_convert_time(e_convert_type_exp);
			cur_reached_num += m_player_ptr->get_convert_mgr().get_convert_time(e_convert_type_bind_diamond);
		}
		break;*/
		case e_mission_end_type_join_legion:
		{
			if (m_player_ptr->get_legion_guid().is_valid())
			{
				cur_reached_num = 1;
			}
			else
			{
				cur_reached_num = 0;
			}
		}
		break;
		case e_mission_end_type_rune_total_level:
		{
			//cur_reached_num = m_player_ptr->get_highest_record(e_role_history_highest_record_rune_total_level);
			//cur_reached_num = m_player_ptr->get_element_heart_mgr().get_rune_total_level();
		}
		break;
		case e_mission_end_type_upgrade_total_level:
		{
			cur_reached_num = item_upgrade_system::get_upgrade_all_count(m_player_ptr);
		}
		break;
		case e_mission_end_type_add_on_total_level:
		{
			cur_reached_num = m_player_ptr->get_highest_record(e_role_history_highest_record_total_equip_addon_num);
		}
		break;
		case e_mission_end_type_mount_total_grade:
		{
			cur_reached_num = m_player_ptr->get_item_set().get_total_mount_grade();
		}
		break;
		case e_mission_end_type_tinder_total_level:
		{
			cur_reached_num = m_player_ptr->get_patron_saint_mgr().get_tinder_total_upgrade_num();
		}
		break;
		case e_mission_end_type_equip_target_color:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_color(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_clear_arena:
		{
			cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num) + target_info_0;
		}
		break;
		case e_mission_end_type_legion_donate:
		{
			cur_reached_num = m_player_ptr->get_unit_info(e_role_info_legion_donate_num); //get_inst_data(e_mission_inst_data_goal1_num) + target_info_0;
		}
		break;
		case e_mission_end_type_grade_levelup:
		{
			cur_reached_num = m_player_ptr->get_grade_num();
		}
		break;
		case e_mission_end_type_upgrade_level_num:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_strength_limit(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_addon_level_num:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_addition_limie(m_mission_ptr->GoalNpcKill[0]);
			}

		}
		break;
	
		case e_mission_end_type_awaken_level_num:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_awaken_limit(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_succinct_level_num:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_succinct_limit(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_learning_skill:
		{
			if (m_mission_ptr->GoalNpcKill.size() > 2)
			{
				int32 skill_id = 0;
				for (int32 i = 1; i < m_mission_ptr->GoalNpcKill.size() / 2; ++i)
				{
					if (m_player_ptr->get_unit_info(e_role_info_class_type) == m_mission_ptr->GoalNpcKill[i * 2])
					{
						skill_id = m_mission_ptr->GoalNpcKill[i * 2 + 1];
						break;
					}
				}
				skill_set& skill_ref = skill_manager::get_skill_set(m_player_ptr->get_array_index());
				skill* skill_ptr = skill_ref.get_skill_inst_by_id(skill_id);
				if (skill_ptr != nullptr)
				{
					cur_reached_num = skill_ptr->get_data_info(e_skill_info_activated);
				}
				
			}
		}
		break;
		case e_mission_end_type_arena_military_rank:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 4)
			{
				if (m_player_ptr->get_special_name_mgr().get_name_level_by_type(e_special_name_type_military_rank) > m_mission_ptr->GoalNpcKill[2] ||
					(m_player_ptr->get_special_name_mgr().get_name_level_by_type(e_special_name_type_military_rank) >= m_mission_ptr->GoalNpcKill[2] &&
					m_player_ptr->get_special_name_mgr().get_name_level_by_type(e_special_name_type_military_rank_star) >= m_mission_ptr->GoalNpcKill[3]))
				{
					cur_reached_num = 1;
				}
				else
				{
					cur_reached_num = 0;
				}
			}
		}
		break;
		case e_mission_end_type_spirit_baptism_color:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_spirit_baptism_color(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_equip_spirit:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_spirit_color(m_mission_ptr->GoalNpcKill[0]);
			}
		}
		break;
		case e_mission_end_type_inset_genstone:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				cur_reached_num = m_player_ptr->get_item_set().get_equip_num_by_inset_genstone();
			}
		}
		break;
		case e_mission_end_type_belief_all_num:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 4)
			{
				cur_reached_num = m_player_ptr->get_belief_mgr().get_equip_num_by_belief_level(m_mission_ptr->GoalNpcKill[2], m_mission_ptr->GoalNpcKill[3]);
			}
		}
		break;
		case e_mission_end_type_use_item:
		case e_mission_end_type_compound_item:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				if (target_info_1 > 0)
				{
					if (target_info_0 == m_mission_ptr->GoalNpcKill[0])
					{
						cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num) + target_info_1;
					}
				}
				else
				{
					cur_reached_num = get_inst_data(e_mission_inst_data_goal1_num);
				}
				
			}
		}
		break;
		case e_mission_end_type_install_equip:
		{
			if (m_mission_ptr->GoalNpcKill.size() > 2)
			{
				int32 item_id = 0;
				for (int32 i = 1; i < m_mission_ptr->GoalNpcKill.size() / 2; ++i)
				{
					if (m_player_ptr->get_unit_info(e_role_info_class_type) == m_mission_ptr->GoalNpcKill[i * 2])
					{
						item_id = m_mission_ptr->GoalNpcKill[i * 2 + 1];
						break;
					}
				}
				if (m_player_ptr->get_item_set().check_equip_by_item_id(item_id))
				{
					cur_reached_num = 1;
				}
				else
				{
					cur_reached_num = 0;
				}
			}
		}
		break;
		case e_mission_end_type_unlock_skill_by_mission:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 11)
			{
				cmission_mgr& mission_mgr_ref = m_player_ptr->get_mission_mgr();
				cmission* main_mission_ptr = mission_mgr_ref.find_mission_by_index(e_mission_slot_main);
				if (nullptr != main_mission_ptr)
				{
					int32 mission_id = m_mission_ptr->GoalNpcKill[10];
					if (main_mission_ptr->get_mission_id() >= m_mission_ptr->GoalNpcKill[10])
					{
						for (int32 i = 0; i < main_mission_ptr->get_mission_id() - m_mission_ptr->GoalNpcKill[10]; i++)
						{
							if (mission_id == main_mission_ptr->get_mission_id())
							{
								if (main_mission_ptr->get_mission_state() >= e_mission_state_finished)
								{
									cur_reached_num = i;
									break;
								}
							}
							MissionTemplate* mission_ptr = GET_TEMPLATE(MissionTemplate, mission_id);
							if (nullptr != mission_ptr)
							{
								mission_id = mission_ptr->NextMissionId;
							}
						}
					}
				}
			}
		}
		break;
		case e_mission_end_type_service_goal:
		{
			cur_reached_num = m_player_ptr->get_seven_day_goal_mgr().get_has_been_received();
		}
		break;
		case e_mission_end_type_total_login:
		{
			cur_reached_num = m_player_ptr->get_welfare_mgr().get_welfare_has_num((int32)e_welfare_type_total_login);
		}
		break;
		case e_mission_end_type_operatrion_raid:
		{
			if (m_mission_ptr->GoalNpcKill.size() >= 2)
			{
				//map_record& temp_record = map_record_mgr::get_instance().get_map_record(m_player_ptr->get_array_index(), m_mission_ptr->GoalNpcKill[0]);
				//if (temp_record.is_valid())
				//{
				//	cur_reached_num = temp_record.get_data_info(e_map_record_info_mopping_num);
				//}
			}
		}
		break;
		case  e_mission_end_type_gs_welfare_num:
		{
			cur_reached_num = m_player_ptr->get_welfare_mgr().get_welfare_has_num((int32)e_welfare_type_gs_reward);
		}
		break;
		case e_mission_end_type_level_welfare_num:
		{
			cur_reached_num = m_player_ptr->get_welfare_mgr().get_welfare_has_num((int32)e_welfare_type_exp_level_reward);
		}
		break;
		case e_mission_end_type_auction_goods:
		{
			cur_reached_num = m_player_ptr->get_first_time_do_mgr().exist_do_record(e_first_time_do_type_auction_sell) == true ? 1 : 0;
		}
		break;
		case e_mission_end_type_auction_shopping:
		{
			cur_reached_num = m_player_ptr->get_first_time_do_mgr().exist_do_record(e_first_time_do_type_auction_buy) == true ? 1 : 0;
		}
		break;
		case e_mission_end_type_by_assis_fight_num:
		{
			cur_reached_num = m_player_ptr->get_logic_data(e_role_logic_info_help_tip_send_times);
		}
		break;
		case e_mission_end_type_active_break_will:
		{
			cur_reached_num = m_player_ptr->get_item_set().get_break_will_level();
		}
		break;
		case e_mission_end_type_search_count:
		{
			cur_reached_num = m_player_ptr->get_logic_data(e_role_logic_info_search_value);
		}
		break;
		case e_mission_end_type_active_skill_count:
		{
			skill_set& skill_set_ref = skill_manager::get_skill_set(m_player_ptr->get_array_index());
			cur_reached_num = skill_set_ref.get_skill_equip_num();
		}
		break;
		case e_mission_end_type_passive_skill_count:
		{
			cur_reached_num = m_player_ptr->get_passive_skill().get_passive_skill_equip_count();
		}
		break;
		case e_mission_end_type_active_skill_level:
		{
			cur_reached_num = m_player_ptr->get_logic_data(e_role_logic_info_skill_level);
		}
		break;
		default:
			break;
		}
		return cur_reached_num;
	}

	int32 cmission::get_target_num()
	{
		if (m_mission_ptr == nullptr
			|| m_mission_ptr->GoalNpcKill.size() < 2)
		{
			return 0;
		}
		return m_mission_ptr->GoalNpcKill[1];
	}

	bool cmission::is_mission_goal_finish(const std::vector<int32>& template_goal_arr)
	{
		if (nullptr == m_mission_ptr)
		{
			return false;
		}

		const int32 goal_arr_len = template_goal_arr.size();
		if (goal_arr_len <= 0)
		{
			return true;
		}
		if (goal_arr_len % e_mission_goal_tuple_max != 0)
		{
			// 表格数据配置
			return false;
		}

		int32 goal_need_num = 0;
		int32 goal_cur_num  = 0;
		int32 mission_goat_inst_value = 0;
		for (int32 goal_level = 0; goal_level < goal_arr_len; goal_level += e_mission_goal_tuple_max)
		{ 
			goal_need_num = template_goal_arr[goal_level + e_mission_goal_tuple_num];
			goal_cur_num  = get_cur_mission_goal_num(goal_level / e_mission_goal_tuple_max);
			if (goal_cur_num < goal_need_num)
			{
				return false;
			}
		}
		
		return true;
	}

	bool cmission::exist_mission_create_npc_by_unit_index(const int32 npc_unit_array_index)
	{
		if (npc_unit_array_index <= 0)
		{
			return false;
		}
		for (int32 i = 0; i < MAX_MISSION_LINE_NUM; ++i)
		{
			if (npc_unit_array_index == m_mission_create_npc[i])
			{
				return true;
			}
		}

		return false;
	}

	bool cmission::exist_mission_create_npc_by_id(const int32 npc_template_id)
	{
		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);
		if (nullptr == npc_template_ptr)
		{
			return false;
		}
		
		int32 npc_array_index = 0;
		for (int32 i = 0; i < MAX_MISSION_LINE_NUM; ++i)
		{
			npc_array_index = m_mission_create_npc[i];
			if (npc_array_index <= 0)
			{
				continue;
			}
			npc& npc_ref = unit_man::get_npc(npc_array_index);
			if (npc_ref.is_valid() == false)
			{
				continue;
			}

			const int32 temp_npc_id = npc_ref.get_unit_info(e_role_info_template_id);
			if (temp_npc_id == npc_template_id)
			{
				return true;
			}
		}

		return false;
	}

	bool cmission::exist_mission_create_npc_by_goal_index(const int32 goal_index)
	{
		if (goal_index < 0 || MAX_MISSION_LINE_NUM <= goal_index)
		{
			return false;
		}

		const int32 unit_index = m_mission_create_npc[goal_index];
		if (unit_index > 0)
		{
			npc& npc_ref = unit_man::get_npc(unit_index);
			if (npc_ref.is_valid() == true)
			{
				return true;
			}
		}

		return false;
	}

	bool cmission::add_mission_create_npc(const int32 npc_unit_array_index)
	{
		if (npc_unit_array_index <= 0)
		{
			return false;
		}
		if (exist_mission_create_npc_by_unit_index(npc_unit_array_index) == true)
		{
			return false;
		}

		for (int32 i = 0; i < MAX_MISSION_LINE_NUM; ++i)
		{
			if (m_mission_create_npc[i] <= 0)
			{
				m_mission_create_npc[i] = npc_unit_array_index;
				return true;
			}
		}

		return false;
	}

	bool cmission::is_finish_mission_when_have_mission_create_npc()
	{
		if (nullptr == m_mission_ptr)
		{
			return false;
		}

		if (is_mission_goal_finish(m_mission_ptr->CreateNpc) == true)
		{
			return true;
		}

		return false;
	}

	void cmission::clear_mission_create_npcs()
	{
		for (int32 i = 0; i < MAX_MISSION_LINE_NUM; ++i)
		{
			const int32 npc_unit_array_index = m_mission_create_npc[i];
			if (npc_unit_array_index <= 0)
			{
				continue;
			}
			unit_man::remove_npc(npc_unit_array_index, false);
			m_mission_create_npc[i] = 0;
		}
	}

	void cmission::send_mission_create_npcs_info()
	{
		if (nullptr == m_player_ptr || nullptr == m_mission_ptr)
		{
			return;
		}

		mission_proto_mission_create_npc_info ission_create_npc_info_msg;

		int32 npc_count = 0;
		for (int32 i = 0; i < MAX_MISSION_LINE_NUM; ++i)
		{
			const int32 npc_unit_array_index = m_mission_create_npc[i];
			if (npc_unit_array_index <= 0)
			{
				continue;
			}

			npc& npc_ref = unit_man::get_npc(npc_unit_array_index);
			if (npc_ref.is_valid() == false)
			{
				continue;
			}

			const int64 npc_guid_64 = npc_ref.get_unit_guid().server_64;
			ission_create_npc_info_msg.add_npc_guid(npc_guid_64);
			++npc_count;
		}

		if (npc_count <= 0)
		{
			return;
		}

		ission_create_npc_info_msg.set_npc_count(npc_count);
		ission_create_npc_info_msg.set_mission_type(m_mission_ptr->MissionShowType);
		m_player_ptr->send_message_to_self(&ission_create_npc_info_msg, e_msgindex_s2c_mission_create_npc_info);
	}

	void cmission::operate_npc_show_by_state()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		int32 cur_mission_id = get_inst_data(e_mission_inst_data_id);
		int32 cur_mission_state = get_inst_data(e_mission_inst_data_mission_state);

		if (cur_mission_id <= 0)
		{
			return;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(m_player_ptr->get_map_ent(), m_player_ptr->get_old_map_pos().unit_location);
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
			if (npc_template_ptr->EndShowMissionId == cur_mission_id
				&& npc_template_ptr->EndShowMissionState > 0)
			{
				if (cur_mission_state == npc_template_ptr->EndShowMissionState)
				{
					//强行同步一下数据，防止数据不同步造成的aoi被客户端拦截
					m_player_ptr->get_mission_mgr().send_one_mission(*this);

					aoi_proto_unit_aoi_out unit_aoi_out_msg;
					unit_aoi_out_msg.add_unit_guid(npc_ref.get_unit_guid().server_64);
					unit_aoi_out_msg.set_is_dissolve(true);
					m_player_ptr->send_message_to_self(&unit_aoi_out_msg, e_msgindex_s2c_aoi_out);
				}
			}

			if (npc_template_ptr->StartShowMissionId == cur_mission_id
				&& npc_template_ptr->StartShowMissionState > 0)
			{
				if (cur_mission_state == npc_template_ptr->StartShowMissionState)
				{
					//强行同步一下数据，防止数据不同步造成的aoi被客户端拦截
					m_player_ptr->get_mission_mgr().send_one_mission(*this);

					aoi_proto_unit_aoi_all unit_aoi_enter_msg;
					npc_ref.get_aoi_msg(unit_aoi_enter_msg);
					unit_aoi_enter_msg.set_can_show_head(npc_ref.check_can_show_head(m_player_ptr->get_unit_guid()));
					m_player_ptr->send_message_to_self(&unit_aoi_enter_msg, e_msgindex_s2c_aoi_simple_one);
				}
			}
		}
	}

	void cmission::active_temporary_wing(bool is_add)
	{
		if (nullptr == m_player_ptr || false == m_player_ptr->is_valid())
		{
			return;
		}
		if (nullptr == m_mission_ptr)
		{
			return;
		}

		if (m_mission_ptr->TemporaryActivateWing.size() > 0 && m_mission_ptr->TemporaryActivateWing.size() >= m_player_ptr->get_unit_info(e_role_info_class_type))
		{
			int32 wing_id = m_mission_ptr->TemporaryActivateWing[(m_player_ptr->get_unit_info(e_role_info_class_type) - 1)]; 
			citem* item_ptr = m_player_ptr->get_item_set().get_item_by_template_id(wing_id);
			if (nullptr == item_ptr)
			{
				return;
			}
			if (is_add)
			{
				item_ptr->set_data_info(e_item_info_activate, 1);
				m_player_ptr->get_item_set().send_item_one(item_ptr);
				m_player_ptr->get_item_set().show_this_wing(item_ptr, -1, true);
				m_player_ptr->set_unit_info(e_role_info_wing_showd_template_id, wing_id);
				m_player_ptr->send_info_one(e_role_info_wing_showd_template_id, true);
				item_proto_show_this_wing msg;
				guid_64 role_guid = m_player_ptr->get_unit_guid();
				msg.add_role_guid(role_guid.A);
				msg.add_role_guid(role_guid.B);
				msg.set_wing_template_id(wing_id);
				m_player_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_show_this_whig);
				//m_player_ptr->get_pawn_att().set_game_att(e_unit_game_att_movement, e_move_ment_wing_walk, true);
				//m_player_ptr->get_phantom_mgr().on_item_oper(item_ptr, item_ptr->get_data_info(e_item_info_info_id));
			}
			else
			{
				item_ptr->set_data_info(e_item_info_activate, 0);
				m_player_ptr->get_item_set().send_item_one(item_ptr);
				m_player_ptr->set_unit_info(e_role_info_wing_showd_template_id, 0);
				m_player_ptr->send_info_one(e_role_info_wing_showd_template_id, true);
				if (m_player_ptr->get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_wing_walk)
				{
					m_player_ptr->get_pawn_att().set_game_att(e_unit_game_att_movement, e_move_ment_walk, true);
				}
				item_proto_show_this_wing msg;
				guid_64 role_guid = m_player_ptr->get_unit_guid();
				msg.add_role_guid(role_guid.A);
				msg.add_role_guid(role_guid.B);
				msg.set_wing_template_id(0);
				m_player_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_show_this_whig);
			}
		}
	}

	bool cmission::is_can_see_npc_guid(guid_64 npc_guid)
	{
		for (int32 i = 0; i < m_npc_guid_arr.size(); i++)
		{
			if (m_npc_guid_arr[i] == npc_guid)
			{
				return true;
			}
		}
		return false;
	}

	void cmission::check_mission_state_with_aoi_npc()
	{
		if (nullptr == m_mission_ptr)
		{
			return;
		}
		switch (m_mission_ptr->MissionEndType)
		{
		case e_mission_end_type_dialog:
		case e_mission_end_type_transfer:
			break;
		default:
			return; //只有传送和对话任务需要在这里检查
		}

		if (m_player_ptr == nullptr)
		{
			return;
		}
		player& cur_player_ref = *m_player_ptr;
		
		NpcSpawnPointTemplate* finish_spawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, m_mission_ptr->FinishMissionNpcId);
		if (finish_spawn_template_ptr == nullptr)
		{
			return;
		}
		int32 finish_npc_id = finish_spawn_template_ptr->TemplateId;
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(cur_player_ref.get_map_ent(), cur_player_ref.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return;
		}
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			npc& aoi_npc_ref = unit_man::get_npc(unit_index);
			if (aoi_npc_ref.is_valid() == false)
			{
				continue;
			}
			
			if (finish_npc_id != aoi_npc_ref.get_npc_template_id())
			{
				continue;
			}
			
			if (get_mission_state() < e_mission_state_done)
			{
				set_mission_state(e_mission_state_done);
			}

			check_mission_state();
			break;
		}
	}
	bool cmission::marry_mission_can_check()
	{
		bool is_can = false;
		//夫妻任务特殊处理
		if (!m_player_ptr->is_player_in_team())
		{
			return is_can;
		}
		if (m_player_ptr->get_player_team_member_num() != 2)
		{	
			m_player_ptr->set_notice_id(90096775);
			return is_can;
		}
		for (int32 i = 0; i < m_player_ptr->get_team_info().member_num; i++)
		{
			guid_64 &mem_guid = m_player_ptr->get_team_info().team_mems_guid[i];
			if (m_player_ptr->get_couple_guid() == mem_guid)
			{
				player& couple_player = unit_man::get_player(mem_guid);
				
				if (false == couple_player.is_valid() || couple_player.get_session_state() != e_session_status_in_gaming)
				{
					return is_can;
				}
				if (m_player_ptr->aoi_is_watch_all_data(couple_player.m_array_index))
				{
					//在AOI范围内
					is_can = true;
				}
				else
				{
					m_player_ptr->set_notice_id(90096816);
					couple_player.set_notice_id(90096816);
				}
				break;
			}
		}
		return is_can;
	}
	void cmission::check_self_see_npc()
	{
		if (nullptr == m_mission_ptr || nullptr == m_player_ptr || m_player_ptr->is_valid() == false)
		{
			return;
		}
		if (m_mission_ptr->HimSelfSeeNpcIdArr.size() <= 0)
		{
			if (m_npc_guid_arr.size() > 0)
			{
				m_npc_guid_arr.clear();
			}
			return;
		}
		if (get_mission_state() >= e_mission_state_can_finish)
		{
			return;
		}
		if (m_npc_guid_arr.size() > 0)
		{
			for (int32 i = 0; i < m_npc_guid_arr.size();)
			{
				npc& npc_ptr = unit_man::get_npc(m_npc_guid_arr[i]);
				if (npc_ptr.is_valid() == false || npc_ptr.is_mission_see(m_player_ptr->get_array_index()) == false)
				{
					m_npc_guid_arr.erase(m_npc_guid_arr.begin() + i);
					continue;
				}
				i++;
			}
		}
		else
		{
			if (m_mission_ptr->HimSelfSeeNpcIdArr.size() <= 0)
			{
				return;
			}
			for (int32 i = 0; i < m_mission_ptr->HimSelfSeeNpcIdArr.size(); i++)
			{
				NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, m_mission_ptr->HimSelfSeeNpcIdArr[i]);
				if (npc_spawn_point_template_ptr == nullptr)
				{
					continue;
				}
				auto map_type = base_map_system::get_map_type(m_player_ptr->get_map_ent());

				if (map_type != faith::e_map_type_big_map)
				{
					return;
				}

				npc* spawn_npc_ptr = world_cs::spawn_npc(m_mission_ptr->HimSelfSeeNpcIdArr[i], m_player_ptr->get_map_ent());
				if (nullptr == spawn_npc_ptr)
				{
					return;
				}
				spawn_npc_ptr->set_can_see_player_guid(m_player_ptr->get_unit_guid());
				m_npc_guid_arr.push_back(spawn_npc_ptr->get_unit_guid());

				world_cs::add_aoi(m_player_ptr->get_map_ent(), spawn_npc_ptr->get_array_index(), spawn_npc_ptr->get_new_map_pos().unit_location);
			}
		}
	}
	bool cmission::is_can_check(e_mission_end_type target_type, int32 target_info_0, int32 target_info_1,bool need_send_is_self)
	{
		bool is_can = false;
		if (m_mission_ptr == nullptr)
		{
			return is_can;
		}
		if (m_player_ptr == nullptr)
		{
			return is_can;
		}
		if ((target_type == e_mission_end_type_npc_kill || target_type == e_mission_end_type_get_item || target_type == e_mission_end_type_marry_dance) && m_mission_ptr->MissionShowType == e_mission_type_marry)
		{
			//夫妻任务特殊处理
			is_can = marry_mission_can_check();
			if (!is_can)
			{
				return is_can;
			}
		}
		switch (target_type)
		{
		case e_mission_end_type_npc_kill:
		case e_mission_end_type_kill_monster_get_item:
		case e_mission_end_type_beat_npc:
		case e_mission_end_type_kill_home_boss:
		{//这几个逻辑相同
			if (m_mission_ptr->MissionEndType == e_mission_end_type_npc_kill
				|| m_mission_ptr->MissionEndType == e_mission_end_type_kill_monster_get_item
				|| m_mission_ptr->MissionEndType == e_mission_end_type_beat_npc
				|| m_mission_ptr->MissionEndType == e_mission_end_type_kill_home_boss)
			{
				is_can = true;
			}
			else
			{
				is_can = false;
			}
		}
			break;
		case e_mission_end_type_map:
		case e_mission_end_type_map_clear_current:
		case e_mission_end_type_activity_clear_current:
		case e_mission_end_type_map_clear_history:
		case e_mission_end_type_reach_level:
		case e_mission_end_type_new_map_finish:
		{//这几个逻辑相同
			if (m_mission_ptr->MissionEndType == e_mission_end_type_map
				|| m_mission_ptr->MissionEndType == e_mission_end_type_map_clear_current
				|| m_mission_ptr->MissionEndType == e_mission_end_type_activity_clear_current
				|| m_mission_ptr->MissionEndType == e_mission_end_type_map_clear_history
				|| m_mission_ptr->MissionEndType == e_mission_end_type_reach_level
				|| m_mission_ptr->MissionEndType == e_mission_end_type_new_map_finish)
			{
				is_can = true;
			}
			else
			{
				is_can = false;
			}
		}
		break;
		default:
		{
			is_can = m_mission_ptr->MissionEndType == target_type;
		}
			break;
		}

		if (!is_can)
		{
			return false;
		}

		switch (target_type)
		{
		case e_mission_end_type_npc_kill:
		case e_mission_end_type_kill_monster_get_item:
		case e_mission_end_type_beat_npc:
		case e_mission_end_type_get_item:
		case e_mission_end_type_mission:
		case e_mission_end_type_npc_attacked_num:
		case e_mission_end_type_equip_upgrade:
		case e_mission_end_type_equip_addon:
		//case e_mission_end_type_legion_donate:
		case e_mission_end_type_clear_arena:
		case e_mission_end_type_use_item:
		case e_mission_end_type_compound_item:
		case e_mission_end_type_kill_home_boss:
		case e_mission_end_type_kill_any_home_boss:
		case e_mission_end_type_kill_any_single_boss:
		case e_mission_end_type_choose_a_fashion:
		{
			//这些类型需要依赖外部传入的
			is_can = (target_info_0 > 0);
		}
		break;
		case e_mission_end_type_purchase_goods:
		{
			//有些限购商品之前可能买过，依赖购买记录
			is_can = true;
		}
		break;
		case e_mission_end_type_equip_levelup:
		case e_mission_end_type_add_friend:
		case e_mission_end_type_dialog:
		case e_mission_end_type_transfer:
		case e_mission_end_type_boss_field:
		case e_mission_end_type_marry_dance:
		case e_mission_end_type_spirit_baptism:
		case e_mission_end_type_mount_upgrade_num:
		case e_mission_end_type_spirit_upgrade_num:
		//case e_mission_end_type_auction_goods:
		case e_mission_end_type_assis_fight_num:
		//case e_mission_end_type_auction_shopping:
		case e_mission_end_type_transform:
		case e_mission_end_type_add_dot:
		{
			//这些类型需要外部传入一个0（非默认参数），表示其是由真的玩家行为触发的，而不是自动检测之类的
			is_can = (target_info_0 == 0);
		}
		break;
		case e_mission_end_type_join_legion_boss_map:
		{
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, target_info_0);
			if (map_template_ptr == nullptr)
			{
				is_can = false;
			}
			else
			{
				is_can = (map_template_ptr->Type == e_map_type_legion_station);
			}
		}
		break;
		case e_mission_end_type_intensify_add_gs:
		{
			is_can = (target_info_0 >= 0);
		}
		break;
		default:
		{
			//这些类型不需要外部参数，可以通过调用函数取得数据
			is_can = true;
		}
		break;
		}
		return is_can;
	}
	bool cmission::target_check(e_mission_end_type mission_end_type, int32 target_info_0, int32 target_info_1,bool need_send_is_self)
	{
		bool check_result = false;
		if (m_mission_ptr == nullptr
			|| m_player_ptr == nullptr)
		{
			return check_result;
		}
		if (get_mission_state() == e_mission_state_none
			|| get_mission_state() == e_mission_state_finished
			|| (m_mission_ptr->MissionShowType == e_mission_type_main
				&& m_mission_ptr->NeedLevel > m_player_ptr->get_unit_info(e_role_info_exp_level)))
		{
			return check_result;
		}
		if (!is_can_check(mission_end_type, target_info_0, target_info_1, need_send_is_self))
		{
			return check_result;
		}
		int32 cur_reached_num = get_cur_reached_num(target_info_0, target_info_1);
		int32 target_num = get_target_num();
		int32 old_reached_num = get_inst_data(e_mission_inst_data_goal1_num);
		if (cur_reached_num > old_reached_num)
		{
			set_inst_data(e_mission_inst_data_goal1_num, cur_reached_num);
			check_mission_state();
			check_result = true;
		}

		return check_result;
	}

	bool cmission::start_dialog(int32 npc_id)
	{
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		if (nullptr == m_mission_ptr)
		{
			return false;
		}

		// 不是对话的npc
		//if (m_mission_ptr->FinishMissionNpcId != npc_id)
		//{
		//	return false;
		//}

		if (get_mission_state() != e_mission_state_done)
		{
			return false;
		}

		set_mission_state(e_mission_state_chat);
		check_mission_state();

		return true;
	}

	bool cmission::is_valid(bool is_check_state)
	{
		if (m_mission_info.data_ary[e_mission_inst_data_id] <= 0)
		{
			return false;
		}
		if (is_check_state)
		{
			if (m_mission_info.data_ary[e_mission_inst_data_mission_state] <= e_mission_state_none
				|| m_mission_info.data_ary[e_mission_inst_data_mission_state] >= e_mission_state_finished)
			{
				return false;
			}
		}
		return true;
	}
}

