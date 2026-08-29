/********************************************************************
created: 2016年7月15日
file base: achievement
author: 张宇翔
purpose: 成就管理系统
*********************************************************************/

#include "server_log.hpp"
#include"item.h"
#include"logic/player.hpp"
#include"achievement_def.h"
#include"achievement_mgr.hpp"
#include "template/template_manager.h"
#include "logic/unit_man.h"
#include "npc.hpp"
#include "internet/cs2dp.pb.h"
#include "utility/parse_msg.h"
#include "internal/achievement_msg.hpp"
#include "internet/net.pb.h"
#include "internet/achievement.pb.h"
#include "utility/init_unit.h"
#include "skill.h"
#include "system/item/item_system.h"

namespace faith
{
	cachievement_mgr::cachievement_mgr()
	{
		m_array_index = 0;
		clear_data();
	}
	cachievement_mgr::~cachievement_mgr()
	{
	}
	//初始化
	void cachievement_mgr::clear_data()
	{
		m_achievement_array.clear();

		m_achievement_about_npc.clear();
		m_achievement_about_item.clear();
		m_achievement_about_self.clear();
		m_achievement_about_skill.clear();
		m_achievement_about_mission.clear();
		m_achievement_about_map.clear();
		m_achievement_about_team.clear();
		m_achievement_about_legion.clear();
		if (m_achievement_about_npc.capacity() == 0)
		{
			m_achievement_about_npc.reserve(30);
			m_achievement_about_item.reserve(60);
			m_achievement_about_self.reserve(160);
			m_achievement_about_skill.reserve(30);
			m_achievement_about_mission.reserve(30);
			m_achievement_about_map.reserve(30);
			m_achievement_about_team.reserve(30);
			m_achievement_about_legion.reserve(30);
		}
	}
	void cachievement_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}
	void cachievement_mgr::send_one_achievement(cachievement& my_achievement)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid())
		{
			achievement_proto_achievement_one achievement_msg_one;
			make_one_achievement_msg(achievement_msg_one, my_achievement);
			temp_player.send_message_to_self(&achievement_msg_one, e_msgindex_s2c_achievement_one);
		}
	}
	void cachievement_mgr::send_all_achievement()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid())
		{
			achievement_proto_achievement_all achievement_msg_all;
			achievement_msg_all.add_role_guid(temp_player.get_unit_guid().A);
			achievement_msg_all.add_role_guid(temp_player.get_unit_guid().B);

			std::map<int32, cachievement>::iterator it = m_achievement_array.begin();
			for (; it != m_achievement_array.end(); it++)
			{
				cachievement& temp_achieve = it->second;
				if (!temp_achieve.get_achievement_template_ptr())
				{
					break;
				}

				achievement_proto_achievement_one* achievement_msg_one = achievement_msg_all.add_achievement_array();
				make_one_achievement_msg(*achievement_msg_one, temp_achieve);

				if (achievement_msg_all.ByteSize() > packege_length_max)
				{
					temp_player.send_message_to_self(&achievement_msg_all, e_msgindex_s2c_achievement_all);
					achievement_msg_all.clear_achievement_array();
				}
			}

			if (achievement_msg_all.achievement_array_size() > 0)
			{
				temp_player.send_message_to_self(&achievement_msg_all, e_msgindex_s2c_achievement_all);
			}
		}
	}

	void cachievement_mgr::make_one_achievement_msg(achievement_proto_achievement_one& achievement_msg_one, const cachievement& my_achievement)
	{
		for (int32 i = eaid_id; i < eaid_max; ++i)
		{
			achievement_msg_one.add_achievement_info(my_achievement.get_inst_data(i));
		}
	}
	//存档
	void cachievement_mgr::save_achievement_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_achievement);
		if (is_use == false)
		{
			cs2dp_save_char_achievement req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;

			int32 data_count = 0;
			std::map<int32, cachievement>::iterator it = m_achievement_array.begin();
			for (; it != m_achievement_array.end(); it++)
			{
				if (data_count >= MAX_ACHIEVEMENT_NUM)
				{
					break;
				}
				cachievement& temp_achieve = it->second;
				if (temp_achieve.get_inst_data(eaid_id) <= 0)
				{
					continue;
				}
				req.data_list[data_count] = temp_achieve.get_achievement_info_all();
				++data_count;
			}
			req.data_num = data_count;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_achievement msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type);
			faith::cs2dp_proto::role_achievement_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}

			int32 row_count = 0;
			std::map<int32, cachievement>::iterator it = m_achievement_array.begin();
			for (; it != m_achievement_array.end(); it++)
			{
				if (row_count >= MAX_ACHIEVEMENT_NUM)
				{
					break;
				}
				cachievement& temp_achieve = it->second;
				if (temp_achieve.get_inst_data(eaid_id) <= 0)
				{
					continue;
				}
				s_achievement_info& tmp = temp_achieve.get_achievement_info_all();
				faith::cs2dp_proto::role_achievement_row *db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				for (int32 i = 0; i < eaid_max ; i++)
				{
					db_row->add_data_ary(tmp.data_ary[i]);
				}
				++row_count;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_achievement);
		}
	}

	void cachievement_mgr::load_achievement_by_db(const s_achievement_info* achievement_data, int32 achievement_num)
	{
		for (int32 i = 0; i < achievement_num; ++i)
		{
			const s_achievement_info& temp_info = achievement_data[i];
			if (temp_info.data_ary[eaid_id] <= 0)
			{
				return;
			}
			create_achievement_by_info(temp_info);
		}

		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_AchievementTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 template_id = it->first;
			if (find_achievement(template_id) != nullptr)
			{
				continue;
			}
			create_achievement_by_template(template_id);
		}
		send_all_achievement();
	}

	bool cachievement_mgr::load_achievement_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_achievement_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_achievement_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_achievement_info *p_row = (s_achievement_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_achievement_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_achievement_by_db((const s_achievement_info *)p_data, msg.row_count());
		return true;

	}

	cachievement*	cachievement_mgr::find_achievement(int32 achievement_id)
	{
		std::map<int32, cachievement>::iterator it = m_achievement_array.find(achievement_id);
		if (it == m_achievement_array.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	std::vector <cachievement*> cachievement_mgr::get_all_can_receive_achievement()
	{
		std::vector <cachievement*> cachievement_can_receive_array;

		std::map<int32, cachievement>::iterator it = m_achievement_array.begin();
		for (; it != m_achievement_array.end(); it++)
		{
			cachievement& temp_achieve = it->second;
			if (temp_achieve.get_inst_data(eaid_id) <= 0)
			{
				continue;
			}
			if (temp_achieve.get_inst_data(eaid_current_state) == eas_can_finish)
			{
				cachievement_can_receive_array.push_back(&temp_achieve);
			}
		}
		return cachievement_can_receive_array;
	}

	cachievement* cachievement_mgr::create_achievement_by_template(int32 achievement_id)
	{
		s_achievement_info temp_info;
		AchievementTemplate* achievement_template_ptr = GET_TEMPLATE(AchievementTemplate, achievement_id);
		if (achievement_template_ptr == nullptr)
		{
			return nullptr;
		}
		temp_info.data_ary[eaid_id] = achievement_id;
		cachievement* temp = create_achievement_by_info(temp_info);
		if (temp == nullptr)
		{
			return nullptr;
		}
		return temp;
	}

	cachievement* cachievement_mgr::create_achievement_by_info(const s_achievement_info& temp_info)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return nullptr;
		}
		if (m_achievement_array.size() >= MAX_ACHIEVEMENT_NUM)
		{
			return nullptr;
		}
		
		cachievement new_achievement;
		new_achievement.init_achievement_by_info(temp_info, &temp_player);
		std::pair<std::map<int32, cachievement>::iterator, bool> it = m_achievement_array.insert({ new_achievement.get_inst_data(eaid_id) ,new_achievement });
		cachievement* new_ptr = &it.first->second;
		if (new_ptr->get_achievement_template_ptr() == nullptr)
		{
			return nullptr;
		}
		switch (new_ptr->get_achievement_template_ptr()->AchievementType)
		{
		case eat_npc_kill_count:
		case eat_boss_kill_count:
		case eat_legion_boss:
		{
			m_achievement_about_npc.push_back(new_ptr);
		}
		break;
		case eat_compose_count:
		case eat_item_upgrade_count:
		case eat_item_upgrade_state_check:
		case eat_item_addon_count:
		case eat_item_addon_state_check:
		case eat_wing_grade_count:
		case eat_mount_type:
		case eat_mount_illusion_count:
		{
			m_achievement_about_item.push_back(new_ptr);
		}
		break;
		case eat_role_level_count:
		case eat_role_transmigration_level_count:
		case eat_current_money_check:
		case eat_all_money_count:
		case eat_belief_grade_count:
		case eat_patron_saint_activation_num_count:
		case eat_patron_saint_grade_num_count:
		case eat_illustration_finish_num_count:
		case eat_spirit_num_count:
		case eat_spirit_level_count:
		case eat_run_equip_num_count:
		case eat_runc_level_count:
		case eat_mount_grade_count:
		case eat_multiple_fuben_has_finished_count:
		case eat_add_friend_count:
		case eat_login_all_count:
		case eat_military_rank_check:
		case eat_role_marry:
		{
			m_achievement_about_self.push_back(new_ptr);
		}
		break;
		case eat_skill_level_check:
		case eat_all_skill_level_count:
		{
			m_achievement_about_skill.push_back(new_ptr);
		}
		break;
		case eat_main_mission_check:
		{
			m_achievement_about_mission.push_back(new_ptr);
		}
		break;
		case eat_easy_fuben_count:
		case eat_medium_fuben_count:
		case eat_hard_fuben_count:
		{
			m_achievement_about_map.push_back(new_ptr);
		}
		break;
		case eat_add_or_create_group_team:
		{
			m_achievement_about_team.push_back(new_ptr);
		}
		break;
		case eat_add_or_create_group_legion:
		{
			m_achievement_about_legion.push_back(new_ptr);
		}
		break;
		default:
			break;
		}
		
		return new_ptr;
	}

	void cachievement_mgr::npc_killed(npc* const dead_npc_ptr)
	{
		if (dead_npc_ptr == nullptr || dead_npc_ptr->get_npc_template() == nullptr)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		for (int32 i = 0; i < m_achievement_about_npc.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_npc[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			switch(temp_achieve_ptr->get_achievement_template_ptr()->AchievementType)
			{
			case eat_npc_kill_count:
			{
				//都行，不判断
			}
			break;
			case eat_boss_kill_count:
			{//杀boss
				if (dead_npc_ptr->get_npc_template()->NpcType != e_unit_type_monster
					|| init_unit::is_world_boss(dead_npc_ptr->get_npc_template()->SubType) == false)
				{
					continue;
				}
			}
			break;
			case eat_legion_boss:
			{//军团BOSS
				if (dead_npc_ptr->get_npc_template()->NpcType != e_unit_type_monster)
				{
					continue;
				}
			}
			break;
			default:
			{
				continue;
			}
				break;
			}

			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal.size() >= e_item_tuple_max
				&& temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id] > 0
				&& temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id] != dead_npc_ptr->get_unit_info(e_role_info_template_id))
			{
				//有ID要求还不匹配的
				continue;
			}

			if (temp_achieve_ptr->inc_count())
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}
	void cachievement_mgr::item_changed(citem* changed_item_ptr, e_item_operation operation_type)
	{
		if (changed_item_ptr == nullptr
			|| changed_item_ptr->get_item_info_ptr() == nullptr)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		for (int32 i = 0; i < m_achievement_about_item.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_item[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}

			int32 new_current_num = temp_achieve_ptr->get_inst_data(eaid_current_num);
			switch (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType)
			{
			case eat_compose_count:
			{
				if (operation_type != e_item_operation_synthesis)
				{
					continue;
				}
				new_current_num++;
			}
			break;
			case eat_item_upgrade_count:
			{
				if (operation_type != e_item_operation_levelup)
				{
					continue;
				}
				new_current_num++;
			}
			break;
			case eat_item_upgrade_state_check:
			{
				if (operation_type != e_item_operation_levelup)
				{
					continue;
				}
				new_current_num = changed_item_ptr->get_data_info(e_item_info_upgrade_count);
			}
			break;
			case eat_item_addon_count:
			{
				if (operation_type != e_item_operation_addon)
				{
					continue;
				}
				new_current_num++;
			}
			break;
			case eat_item_addon_state_check:
			{
				if (operation_type != e_item_operation_addon)
				{
					continue;
				}
				new_current_num = changed_item_ptr->get_data_info(e_item_info_add_on);
			}
			break;
			case eat_wing_grade_count:
			{
				if (operation_type != e_item_operation_featherupgrade_grade)
				{
					continue;
				}
				new_current_num++;
			}
			break;
			case eat_mount_type:
			{
				if (operation_type != e_item_operation_mount_unlock)
				{
					continue;
				}
				new_current_num++;
			}
			break;
			case eat_mount_illusion_count:
			{
				if (operation_type != e_item_operation_mount_get_illusion)
				{
					continue;
				}
				new_current_num++;
			}
			break;
			default:
			{
				continue;
			}
				break;
			}

			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal.size() >= e_item_tuple_max
				&& temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id] > 0
				&& temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id] != changed_item_ptr->get_data_info(e_item_info_info_id))
			{
				//有ID要求还不匹配的
				continue;
			}
			if (temp_achieve_ptr->replace_count(new_current_num))
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::player_changed(e_achievement_type operate_type)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		for (int32 i = 0; i < m_achievement_about_self.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_self[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			if (operate_type != temp_achieve_ptr->get_achievement_template_ptr()->AchievementType)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			int32 new_current_num = temp_achieve_ptr->get_inst_data(eaid_current_num);
			switch (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType)
			{
			case eat_role_level_count:
			{
				new_current_num = temp_player.get_unit_info(e_role_info_exp_level);
			}
			break;
			case eat_role_transmigration_level_count:
			{
				new_current_num = temp_player.get_grade_num_and_level_num().grade_num;
			}
			break;
			case eat_current_money_check:
			{
				if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal.size() < e_item_tuple_max)
				{
					continue;
				}
				new_current_num = temp_player.get_money_data((e_money_type)temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id]);
			}
			break;
			case eat_all_money_count:
			{
				if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal.size() < e_item_tuple_max)
				{
					continue;
				}
				new_current_num = temp_player.get_all_money_data((e_money_type)temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id]);
			}
			break;
			case eat_belief_grade_count:
			{
				new_current_num = temp_player.get_belief_mgr().get_total_rank();
			}
			break;
			case eat_patron_saint_activation_num_count:
			{
				//new_current_num = temp_player.get_item_set().get_mark_activated_num();
			}
			break;
			case eat_patron_saint_grade_num_count:
			{
				new_current_num = temp_player.get_patron_saint_level_num();
			}
			break;
			case eat_illustration_finish_num_count:
			{
				new_current_num = temp_player.get_pokedex_finished_num();
			}
			break;
			case eat_spirit_num_count:
			{
				//new_current_num = temp_player.get_spirit_mgr().num_of_spirit_equiped(0);
			}
			break;
			case eat_spirit_level_count:
			{
				new_current_num = temp_player.get_highest_record(e_role_history_highest_record_total_sprite_equip_level);
			}
			break;
			case eat_run_equip_num_count:
			{
				new_current_num = temp_player.get_element_heart_equip_num();
			}
			break;
			case eat_runc_level_count:
			{
				//new_current_num = temp_player.get_item_set().get_total_element_heart_level();
			}
			break;
			case eat_mount_grade_count:
			{
				new_current_num = temp_player.get_highest_mount_level();
			}
			break;
			case eat_multiple_fuben_has_finished_count:
			case eat_role_marry:
			{
				new_current_num++;
			}
			break;
			case eat_add_friend_count:
			{
				new_current_num = temp_player.get_highest_record(e_role_history_highest_record_higest_friend_num);
			}
			break;
			case eat_login_all_count:
			{
				new_current_num = temp_player.get_unit_info(e_role_info_total_login_days);
			}
			break;
			case eat_military_rank_check:
			{
				new_current_num = temp_player.get_special_name_mgr().get_name_level_by_type(e_special_name_type_military_rank);
			}
			break;
			default:
				continue;
				break;
			}
			if (temp_achieve_ptr->replace_count(new_current_num))
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}
	void cachievement_mgr::skill_level_up(skill* upgrade_skill)
	{
		if (upgrade_skill == nullptr
			|| upgrade_skill->get_skill_template_ptr() == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < m_achievement_about_skill.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_skill[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType != eat_skill_level_check)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			if (temp_achieve_ptr->replace_count(upgrade_skill->get_data_info(e_skill_info_level)))
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::skill_level_count(int32 new_value)
	{
		for (int32 i = 0; i < m_achievement_about_skill.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_skill[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType != eat_all_skill_level_count)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			if (temp_achieve_ptr->replace_count(new_value))
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::mission_finish(cmission* finished_mission)
	{
		if (finished_mission == nullptr)
		{
			return;
		}
		if (finished_mission->get_mission_template_ptr() == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < m_achievement_about_mission.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_mission[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal.size() < e_item_tuple_max)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType != eat_main_mission_check)
			{
				continue;
			}
			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id] > 0
				&& temp_achieve_ptr->get_achievement_template_ptr()->AchievementGoal[e_item_tuple_id] > finished_mission->get_inst_data(e_mission_inst_data_id))
			{
				continue;
			}
			if (temp_achieve_ptr->inc_count())
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::fubenfinish(map_record* new_record)
	{
		if (new_record == nullptr
			/*|| new_record->get_map_template_ptr() == nullptr*/)
		{
			return;
		}
		for (int32 i = 0; i < m_achievement_about_map.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_map[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			switch (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType)
			{
			//case eat_easy_fuben_count:
			//{
			//	if (new_record->get_map_template_ptr()->Difficulty != e_map_difficulty_easy)
			//	{
			//		continue;
			//	}
			//}
			//break;
			//case eat_medium_fuben_count:
			//{
			//	if (new_record->get_map_template_ptr()->Difficulty != e_map_difficulty_normal)
			//	{
			//		continue;
			//	}
			//}
			//break;
			//case eat_hard_fuben_count:
			//{
			//	if (new_record->get_map_template_ptr()->Difficulty != e_map_difficulty_heard)
			//	{
			//		continue;
			//	}
			//}
			//break;
			default:
				continue;
				break;
			}
			if (temp_achieve_ptr->inc_count())
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::create_team()
	{
		for (int32 i = 0; i < m_achievement_about_team.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_team[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType != eat_add_or_create_group_team)
			{
				continue;
			}
			if (temp_achieve_ptr->inc_count())
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::create_legion()
	{
		for (int32 i = 0; i < m_achievement_about_legion.size(); ++i)
		{
			cachievement* temp_achieve_ptr = m_achievement_about_legion[i];
			if (temp_achieve_ptr == nullptr
				|| temp_achieve_ptr->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			int32 old_state = temp_achieve_ptr->get_inst_data(eaid_current_state);
			if (old_state != eas_accept)
			{
				continue;
			}
			if (temp_achieve_ptr->get_achievement_template_ptr()->AchievementType != eat_add_or_create_group_legion)
			{
				continue;
			}
			if (temp_achieve_ptr->inc_count())
			{
				if (old_state != temp_achieve_ptr->get_inst_data(eaid_current_state))
				{
					save_achievement_log(temp_achieve_ptr->get_inst_data(eaid_id));
				}
			}
			send_one_achievement(*temp_achieve_ptr);
		}
	}

	void cachievement_mgr::save_achievement_log(int32 achievement_id)
	{
		if (achievement_id <= 0)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && player_ref.get_write_log())
		{
			server_log::achievement_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), achievement_id);
		}
		
	}

	void cachievement_mgr::set_achievement_success(int32 achievement_id, int32 achievement_state)
	{
		std::vector<s_item_template_info> get_money_tuple_array;
		cachievement* achievement = find_achievement(achievement_id);
		if (achievement == nullptr)
		{
			return;
		}
		if (achievement->get_achievement_template_ptr() == nullptr)
		{
			return;
		}
		if (achievement_state != eas_can_finish)
		{
			return;
		}

		if (achievement->get_achievement_state() != eas_can_finish)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (achievement->finish_achievement(get_money_tuple_array) == eaet_success)
		{
			item_system::send_promp_msg_to_client(&temp_player, get_money_tuple_array);
			send_one_achievement(*achievement);
			save_achievement_log(achievement->get_inst_data(eaid_id));
		}
	}

	void cachievement_mgr::set_can_receive_achievement_success()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		std::vector<s_item_template_info> get_money_tuple_array;
		std::vector<cachievement*> can_receive_cachievement_array = get_all_can_receive_achievement();
		for (int32 i = 0; i < can_receive_cachievement_array.size(); i++)
		{
			if (can_receive_cachievement_array[i] == nullptr)
			{
				continue;
			}
			if (can_receive_cachievement_array[i]->get_inst_data(eaid_id) == 0)
			{
				continue;
			}
			
			if (can_receive_cachievement_array[i]->get_achievement_template_ptr() == nullptr)
			{
				continue;
			}
			if (can_receive_cachievement_array[i]->get_achievement_state() != eas_can_finish)
			{
				continue;
			}

			if (can_receive_cachievement_array[i]->finish_achievement(get_money_tuple_array) == eaet_success)
			{
				send_one_achievement(*(can_receive_cachievement_array[i]));
				save_achievement_log(can_receive_cachievement_array[i]->get_inst_data(eaid_id));
			}
		}

		if (get_money_tuple_array.size() > 0)
		{
			item_system::send_promp_msg_to_client(&temp_player, get_money_tuple_array);
		}	
	}

}