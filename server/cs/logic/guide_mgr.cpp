#include "guide_mgr.h"
#include "internal/guide_msg.hpp"
#include "internet/character.pb.h"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "logic/player.hpp"
#include "logic/skill_manager.h"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "skill_set.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"

namespace faith
{
	guide_mgr::guide_mgr()
	{
		m_skill_template_ptr = GET_TEMPLATE(SkillTemplate, warning_guide_skill_template_id);
		m_unit_index = 0;
		clear_data();
	}

	void guide_mgr::clear_data()
	{
		for (int32 i = 0; i < faith::max_guide_record_num; i++)
		{
			m_record_info_list[i].clear_data();
		}
		m_record_num = 0;
	}

	void guide_mgr::save_guide_record(s_record_info& record_info)
	{
		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (record_info.state > 0)
		{
			server_log::new_stages_role_log(player_ref.get_third_info(), player_ref.m_role_info, record_info.guide_trigger_id, player_ref.get_login_type());
			set_log_var(log_head);
			player_ref.get_log_common_head_info(log_head);
			server_log::serverNewbie(log_head, init_unit::change_i32_to_string(record_info.guide_trigger_id));
		}

		if (if_exist_guide_record(record_info))
		{
			for (int32 i = 0; i < m_record_num; i++)
			{
				if (m_record_info_list[i].guide_trigger_id == record_info.guide_trigger_id)
				{
					m_record_info_list[i].state = record_info.state;
					break;
				}
			}
		}
		else
		{
			add_once_guide_recond(record_info);
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_guide_record);
		if (is_use == false)
		{

			cs2dp_save_guide_record save_guide_record_msg;
			save_guide_record_msg.role_guid = player_ref.get_unit_guid();
			save_guide_record_msg.unit_array_index = player_ref.get_array_index();
			save_guide_record_msg.record_info = record_info;
			player_ref.send_message_to_dp(&save_guide_record_msg, sizeof(save_guide_record_msg));
		}
		else
		{
			faith::cs2dp_proto::save_role_guide msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(player_ref.get_array_index());
			faith::cs2dp_proto::role_guide_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			db_data->set_guide_trigger_id(record_info.guide_trigger_id);
			db_data->set_state(record_info.state);
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_guide_record);
		}

		if (m_skill_template_ptr && m_skill_template_ptr->TriggerGuide == record_info.guide_trigger_id)
		{
			//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
			//if (map_object_ptr)
			//{
			//	npc* npc_ptr = map_object_ptr->get_npc_one();
			//	if (npc_ptr)
			//	{
			//		npc_ptr->relieve_stop_time(warning_guide_skill_template_id);
			//	}
			//}
		}
	}

	void guide_mgr::save_guide_record_lua(int32 guide_trigger_id)
	{
		s_record_info record_info;
		record_info.guide_trigger_id = guide_trigger_id;
		record_info.state = 1;
		save_guide_record(record_info);
	}

	void guide_mgr::load_guide_record(const int32 record_num, const s_record_info record_info_list[faith::max_guide_record_num])
	{
		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		load_guide_record_reset(record_num, record_info_list);
		if (player_ref.get_unit_info(e_role_info_class_type) == e_class_type_assassinator)
		{
			add_skip_begin_guide_record(e_class_type_assassinator);
		}

		send_all_guide_record();
	}
	bool guide_mgr::load_guide_record_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::db_proto::load_role_guide_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_record_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_record_info *p_row = (s_record_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::db_proto::load_role_guide_row db_row = msg.row_data(i);
			p_row->guide_trigger_id = db_row.guide_trigger_id();
			p_row->state = db_row.state();
			p_row++;
		}
		load_guide_record(msg.row_count(),(const s_record_info *)p_data);
		return true;


	}

	void guide_mgr::load_guide_record_reset(const int32 record_num, const s_record_info record_info_list[faith::max_guide_record_num])
	{
		if (record_num >= faith::max_guide_record_num)
		{
			return;
		}
		for (int32 i = 0; i < record_num; i++)
		{
			add_once_guide_recond(record_info_list[i]);
		}

		s_record_info add_record_info_arr[faith::max_guide_record_num];
		int32 add_record_info_num = 0;
		get_add_guide_record(add_record_info_num, add_record_info_arr);
		if (add_record_info_num >= faith::max_guide_record_num)
		{
			return;
		}
		for (int32 i = 0; i < add_record_info_num; i++)
		{
			add_once_guide_recond(add_record_info_arr[i], true);
		}
	}

	void guide_mgr::send_all_guide_record()
	{
		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		check_guide();
		character_proto_update_guide_record update_guide_record_msg;
		for (int32 i = 0; i < m_record_num && i < faith::max_guide_record_num; ++i)
		{
			character_proto_guide_record_one* record_one = update_guide_record_msg.add_record_list();
			if (record_one == nullptr)
			{
				return;
			}
			record_one->set_guide_trigger_id(m_record_info_list[i].guide_trigger_id);
			record_one->set_state(m_record_info_list[i].state);
		}
		player_ref.send_message_to_self(&update_guide_record_msg, e_msgindex_s2c_update_guide_record);
	}

	void guide_mgr::get_add_guide_record(int32& add_record_info_num, s_record_info add_record_info_arr[faith::max_guide_record_num])
	{
		for (int32 i = 0; i < m_record_num; i++)
		{
			int32 guide_trigger_template_id = m_record_info_list[i].guide_trigger_id / 100 * 100;
			int32 record_state = m_record_info_list[i].state;
			for (int32 j = 0; j < faith::guide_interval; j++)
			{
				GuideTriggerTemplate* guide_trigger_template_ptr = GET_TEMPLATE(GuideTriggerTemplate, guide_trigger_template_id + j);
				if (guide_trigger_template_ptr == nullptr)
				{
					break;
				}
				else
				{
					if (add_record_info_num < faith::max_guide_record_num)
					{
						add_record_info_arr[add_record_info_num].guide_trigger_id = guide_trigger_template_id + j;
						add_record_info_arr[add_record_info_num].state = record_state;
						add_record_info_num++;
					}
				}
			}
		}
	}

	bool guide_mgr::if_exist_guide_record(s_record_info record_info)
	{
		if (m_record_num >= faith::max_guide_record_num)
		{
			return true;
		}
		for (int32 i = 0; i < m_record_num; i++)
		{
			if (m_record_info_list[i].guide_trigger_id == record_info.guide_trigger_id)
			{
				return true;
			}
		}
		return false;
	}

	bool guide_mgr::if_guide_triggered(int32 guide_trigger_id)
	{
		for (int32 i = 0; i < m_record_num; i++)
		{
			if (m_record_info_list[i].guide_trigger_id == guide_trigger_id)
			{
				if (m_record_info_list[i].state > 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}

	void guide_mgr::logout_save_guide_record()
	{
		//player& player_ref = unit_man::get_player(m_unit_index);
		//if (false == player_ref.is_valid())
		//{
		//	return;
		//}
		//cmission* main_mission_ptr = player_ref.get_mission_mgr().find_mission_by_index(e_mission_slot_main);
		//if (main_mission_ptr == nullptr)
		//{
		//	return;
		//}
		//int32 cur_mission_id = main_mission_ptr->get_mission_id();

		//faith::template_manager::template_type* table = template_manager::get_instance().get_templates(e_GuideTriggerTemplate);
		//if (nullptr == table)
		//{
		//	return;
		//}

		//faith::template_manager::template_type::iterator ite;
		//for (ite = table->begin(); ite != table->end(); ++ite)
		//{
		//	GuideTriggerTemplate* guide_trigger_template_ptr = (GuideTriggerTemplate*)(ite->second);
		//	if (nullptr == guide_trigger_template_ptr)
		//	{
		//		continue;
		//	}
		//	if (guide_trigger_template_ptr->TriggerGuideID == 100000000)
		//	{
		//		s_record_info record_info;
		//		record_info.guide_trigger_id = guide_trigger_template_ptr->attribute_id;
		//		record_info.state = 1;
		//		save_guide_record(record_info);
		//		continue;
		//	}
		//	if (nullptr == guide_trigger_template_ptr || guide_trigger_template_ptr->NeedMission.size() < 3)
		//	{
		//		continue;
		//	}
		//	int32 need_mission_id = guide_trigger_template_ptr->NeedMission[1];
		//	if (cur_mission_id > need_mission_id)
		//	{
		//		GuideTemplate* guide_template_ptr = GET_TEMPLATE(GuideTemplate, guide_trigger_template_ptr->TriggerGuideID);
		//		if (guide_template_ptr == nullptr)
		//		{
		//			continue;
		//		}
		//		if (guide_template_ptr->GuideType == 4)
		//		{
		//			s_record_info record_info;
		//			record_info.guide_trigger_id = guide_trigger_template_ptr->attribute_id;
		//			record_info.state = 1;
		//			save_guide_record(record_info);
		//		}
		//	}
		//}
	}
	void guide_mgr::add_skip_begin_guide_record(e_class_type class_type)
	{
		int32 profession_index = class_type - 1;
		template_manager::template_type* guide_trigger_template_table = template_manager::get_instance().get_templates(e_GuideTriggerTemplate);
		if (nullptr == guide_trigger_template_table || GAMECONFIG->InitRoleAry.size() <= profession_index || profession_index < 0)
		{
			return;
		}
		PlayerInitTemplate* player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, GAMECONFIG->InitRoleAry[profession_index]);
		if (nullptr == player_init_template_ptr)
		{
			return;
		}
		for (auto ite : *guide_trigger_template_table)
		{
			//只有等级限制，或者只有解锁功能限制的引导，除去这两个属性其他都应为空
			GuideTriggerTemplate* guide_trigger_template_ptr = (GuideTriggerTemplate*)(ite.second);
			if (nullptr == guide_trigger_template_ptr || guide_trigger_template_ptr->NeedLevel > player_init_template_ptr->InitLevel || guide_trigger_template_ptr->NeedMission.size() > 0 
				|| guide_trigger_template_ptr->NeedItem.size() > 0 || guide_trigger_template_ptr->NeedMoney.size() > 0 || guide_trigger_template_ptr->RoleState != 0 
				|| guide_trigger_template_ptr->OpenWidget != "" || guide_trigger_template_ptr->FirstTimeDo != 0 || guide_trigger_template_ptr->DeadCanGuide != 0 || guide_trigger_template_ptr->TriggerGuideID == 0
				|| guide_trigger_template_ptr->IsHitFirstGuide != 0)
			{
				continue;
			}

			//只有等级限制的引导
			if (guide_trigger_template_ptr->NeedLevel > 0 && guide_trigger_template_ptr->NeedFuncUnlock == "")
			{
				s_record_info add_record_info;
				add_record_info.guide_trigger_id = guide_trigger_template_ptr->attribute_id;
				add_record_info.state = 1;
				add_once_guide_recond(add_record_info);
				continue;
			}

			//功能解锁的限制等级不到初始等级的引导
			if (guide_trigger_template_ptr->NeedFuncUnlock != "")
			{
				FuncUnlockTemplate* func_unlock_template_ptr = func_unlock_mgr::get_func_unlock_template_by_func_name_of_all_func(guide_trigger_template_ptr->NeedFuncUnlock);
				if (nullptr == func_unlock_template_ptr || func_unlock_template_ptr->UnlockNeedLevel <= 0 || func_unlock_template_ptr->UnlockNeedLevel > player_init_template_ptr->InitLevel
					|| func_unlock_template_ptr->UnlockNeedMissionID != 0 || func_unlock_template_ptr->IsSpecialLogic != 0)
				{
					continue;
				}
				s_record_info add_record_info;
				add_record_info.guide_trigger_id = guide_trigger_template_ptr->attribute_id;
				add_record_info.state = 1;
				add_once_guide_recond(add_record_info);
			}
		}
	}

	void guide_mgr::add_once_guide_recond(s_record_info record_info, bool need_save)
	{
		if (if_exist_guide_record(record_info) || m_record_num >= faith::max_guide_record_num)
		{
			return;
		}
		m_record_info_list[m_record_num] = record_info;
		m_record_num++;
		if (need_save == true)
		{
			save_guide_record(record_info);
		}
	}

	void guide_mgr::guide_trigger_start_proc(int32 guide_trigger_template_id)
	{
		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		GuideTriggerTemplate* guide_trigger_template_ptr = GET_TEMPLATE(GuideTriggerTemplate, guide_trigger_template_id);
		if (nullptr == guide_trigger_template_ptr || if_guide_triggered(guide_trigger_template_id))
		{
			return;
		}

		if (guide_trigger_template_ptr->NeedMission.size() >= need_mission_tuple_max)
		{
			if (player_ref.get_mission_mgr().get_mission_id(e_mission_slot_main) > guide_trigger_template_ptr->NeedMission[need_mission_tuple_id])
			{
				return;
			}
		}

		if (guide_trigger_template_ptr->ResetSkillType > 0)
		{
			auto& skill_set_ref = skill_manager::get_skill_set(m_unit_index);
			skill_set_ref.set_skill_cd_by_type((e_skill_type)guide_trigger_template_ptr->ResetSkillType, 0, true);
		}

		if (guide_trigger_template_ptr->attribute_id == first_exp_fuben_guide_trigger_id)
		{
			player_ref.m_is_begin_exp_raid = false;
			player_ref.m_is_need_add_exp_buff = false;
		}
	}

	void guide_mgr::check_guide()
	{
		player& player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (player_ref.get_unit_info(e_role_info_need_check_guide) == 0)
		{
			return;
		}

		cmission* main_mission_ptr = player_ref.get_mission_mgr().find_mission_by_index(e_mission_slot_main);
		if (main_mission_ptr == nullptr)
		{
			return;
		}
		int32 mission_id = main_mission_ptr->get_mission_id();
		int32 mission_state = main_mission_ptr->get_mission_state();

		template_manager::template_type* guide_trigger_template_table = template_manager::get_instance().get_templates(e_GuideTriggerTemplate);
		if (nullptr == guide_trigger_template_table)
		{
			return;
		}
		for (auto ite : *guide_trigger_template_table)
		{
			GuideTriggerTemplate* guide_trigger_template_ptr = (GuideTriggerTemplate*)(ite.second);
			if (nullptr == guide_trigger_template_ptr || guide_trigger_template_ptr->LoginCheckGuideMission.size() < 3)
			{
				continue;
			}
			if (if_guide_triggered(guide_trigger_template_ptr->attribute_id))
			{
				continue;
			}
			if (mission_id > guide_trigger_template_ptr->LoginCheckGuideMission[1])
			{
				s_record_info record_info;
				record_info.guide_trigger_id = guide_trigger_template_ptr->attribute_id;
				record_info.state = 1;
				save_guide_record(record_info);
			}
			else if (mission_id = guide_trigger_template_ptr->LoginCheckGuideMission[1] && mission_state >= guide_trigger_template_ptr->LoginCheckGuideMission[2])
			{
				s_record_info record_info;
				record_info.guide_trigger_id = guide_trigger_template_ptr->attribute_id;
				record_info.state = 1;
				save_guide_record(record_info);
			}
		}
		player_ref.set_unit_info(e_role_info_need_check_guide, 0);
		player_ref.send_info_one(e_role_info_need_check_guide);
	}

}