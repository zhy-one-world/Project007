/********************************************************************
	created:	2016年10月18日21:38:42
	file base:	map_object
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "server_log.hpp"
#include "connection_mgr.hpp"
#include "time.hpp"
#include "map_object.h"
#include "template/template_manager.h"
#include "Logic/map_def.hpp"
#include "Logic/activity_def.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "cell_server.hpp"
#include "../npc.hpp"
#include "gm_order_def.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "internal/core.hpp"
#include "internal/char_msg.hpp"

namespace faith
{
	map_object::map_object()
	{
		m_player_index_list.clear();
		m_npc_index_list.clear();
		clear_data();
	}

	map_object::~map_object()
	{
	}
	
	void map_object::clear_data()
	{
		m_map_guid.clear_data();
		m_map_template_ptr = NULL;
		m_map_template_id = 0;
		m_map_type = kInvalidId;
		m_line_id = 0;
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			unit_index_map_it temp_it = it++;
			del_player(temp_it->first);
		}

		for (unit_index_map_it it = m_npc_index_list.begin(); it != m_npc_index_list.end(); )
		{
			unit_index_map_it temp_it = it++;
			unit_man::remove_npc(temp_it->first, false);
		}
		m_npc_index_list.clear();
		m_is_win = false;
		m_end_time = 0;
		m_begin_map_time = utility::get_tick_count();
		m_first_player_join_time = 0;
		m_map_state = e_map_state_join;
		m_score_rank_indicator.clear_data();
		m_map_all_time = 0;
		m_map_state_time = 0;
		m_is_double_line = false;
		m_wave_trig_message_set.clear();
		m_map_pk_mode = -1;
		m_map_own_legion.clear_data();
		m_player_avg_lv = 0;
		m_legion_average_lv = 0;
		m_dead_player_map.clear();
		m_now_player_avg_lv = 0;
		m_enter_map_array.clear();
		m_map_target_show_info.clear();
	}

	bool map_object::init_map_data(const guid_64 map_guid, int32 template_id, int32 line_id, int32 map_all_time)
	{
		ZoneScoped;
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, template_id);
		if (nullptr == map_template_ptr)
		{
			return false;
		}
		set_map_guid(map_guid);
		set_map_template_id(template_id);
		set_line_id(line_id);
		set_map_template_ptr(map_template_ptr);
		m_map_all_time = map_all_time;
		//aoi_manager::get_instance().create_aoi(map_guid,
		//	map_template_ptr->MapStartX,
		//	map_template_ptr->MapEndX,
		//	map_template_ptr->MapStartY,
		//	map_template_ptr->MapEndY,
		//	map_template_ptr->TowerWidth,
		//	map_template_ptr->TowerLength
		//	);
		
		//lua_module_mgr::get_instance().call_table_func("map_object_lua", "init_map_data", "33");
		return true;
	}

	bool map_object::is_can_revive(e_revive_type revive_type, int32 player_idx)
	{
		if (nullptr == m_map_template_ptr)
		{
			return false;
		}
		int32 map_rule_check = false;
		switch (m_map_template_ptr->MapReviveType)
		{
		case e_revive_type_transfer:
		case e_revive_type_born_pos:
			map_rule_check = revive_type == m_map_template_ptr->MapReviveType || revive_type == e_revive_type_same_pos;
			break;

		case e_revive_type_free_revive_by_CD:
			map_rule_check = revive_type == m_map_template_ptr->MapReviveType;
			break;

		case e_revive_type_cant_revive:
			map_rule_check = revive_type == e_revive_type_transfer;
			break;

		default:
			break;
		}
		if (revive_type == e_revive_type_same_pos)
		{
			if (m_map_template_ptr->NotCanRevive == 1)
			{
				return false;
			}
		}
		int32 can_fuhuo_cd = m_map_template_ptr->DeadToLiveCD;

		if (map_rule_check && player_idx > 0 && revive_type == e_revive_type_born_pos)
		{
			unit_guid_map_it dead_stamp_it = get_player_dead_stamp(player_idx);
			if (dead_stamp_it != m_dead_player_map.end())
			{
				int64 now_stamp = utility::get_tick_count();
				map_rule_check = (now_stamp >= (dead_stamp_it->second + can_fuhuo_cd * second_tick_time));
				if (map_rule_check)
				{
					m_dead_player_map.erase(dead_stamp_it);
				}
			}
		}

		return map_rule_check;
	}

	bool map_object::is_can_change_pk_mode()
	{
		switch (get_map_type())
		{
		case e_map_type_city_war:
		case e_map_type_king_of_pk:
		case e_map_type_crystak_dreamland:
		case e_map_type_attack_city:
			return false;
		default:
			return true;
		}
	}

	int64 map_object::get_map_life_time()
	{
		if (m_first_player_join_time <= 0)
		{
			return 0;
		}
		int64 cur_time = utility::get_tick_count();
		int64 life_time = cur_time - m_first_player_join_time;
		return life_time;
	}

	void map_object::send_map_target_list_to_all()
	{
		game_proto_map_target_show_info_list msg;
		for (int32 i = 0; i < m_map_target_show_info.size(); ++i)
		{
			game_proto_map_target_show_info* show_info = msg.add_info_list();
			if (nullptr == show_info)
			{
				continue;
			}
			show_info->set_guid_a(m_map_target_show_info[i].role_guid.A);
			show_info->set_guid_b(m_map_target_show_info[i].role_guid.B);
			show_info->set_image_index(m_map_target_show_info[i].image_index);
		}
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& temp_player = unit_man::get_player(temp_it->first);
			if (temp_player.is_valid() && temp_player.get_map_guid() == get_map_guid() && temp_player.get_session_state() == e_session_status_in_gaming)
			{
				temp_player.send_message_to_self(&msg, e_mgsindex_s2c_send_map_target_show_info);
			}
		}
	}

	void map_object::send_map_target_list(guid_64 role_guid)
	{
		player& temp_player = unit_man::get_player(role_guid);
		if (false == temp_player.is_valid() || temp_player.get_map_guid() != get_map_guid() || temp_player.get_session_state() != e_session_status_in_gaming)
		{
			return;
		}
		game_proto_map_target_show_info_list msg;
		for (int32 i = 0; i < m_map_target_show_info.size(); ++i)
		{
			game_proto_map_target_show_info* show_info = msg.add_info_list();
			if (nullptr == show_info)
			{
				continue;
			}
			show_info->set_guid_a(m_map_target_show_info[i].role_guid.A);
			show_info->set_guid_b(m_map_target_show_info[i].role_guid.B);
			show_info->set_image_index(m_map_target_show_info[i].image_index);
		}
		temp_player.send_message_to_self(&msg, e_mgsindex_s2c_send_map_target_show_info);
	}

	void map_object::add_map_target_info(s_map_target_show_info target_info, bool is_send)
	{
		bool is_change = true;
		for (int32 i = 0; i < m_map_target_show_info.size(); ++i)
		{
			if (m_map_target_show_info[i].role_guid == target_info.role_guid)
			{
				m_map_target_show_info[i] = target_info;
				is_change = false;
				break;
			}
		}
		if (is_change)
		{
			m_map_target_show_info.push_back(target_info);
		}
		if (is_send)
		{
			send_map_target_list_to_all();
		}
	}

	void map_object::add_aoi(const fvector& unit_location, const int32& unit_index)
	{
		//aoi_manager::get_instance().add_object(m_map_guid, unit_location, unit_index);
	}

	void map_object::remove_aoi(const fvector& unit_location, const int32& unit_index, bool is_dead)
	{
		//aoi_manager::get_instance().remove_object(m_map_guid, unit_location, unit_index, is_dead);
	}

	void map_object::update_aoi(const fvector& old_location, const fvector& new_location, const int32& unit_index)
	{
		//aoi_manager::get_instance().update_object(m_map_guid, old_location, new_location, unit_index);
	}

	void map_object::send_game_state(e_map_state map_state)
	{
		cs2ws_map_state msg;
		msg.map_guid = m_map_guid;
		msg.map_state = map_state;
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void map_object::send_wave_trigger_message_to_all_player(int32 message)
	{
		game_proto_wave_trigger_message wave_trigger_message_msg;
		wave_trigger_message_msg.set_message(message);
		send_message_to_all_player(&wave_trigger_message_msg, e_msgindex_s2c_wave_trigger_message);
		
		m_wave_trig_message_set.insert(message);
	}

	void map_object::send_boss_state_message_to_all_player(int32 boss_id, e_boss_state boss_state, bool is_final_boss)
	{
		game_proto_boss_state boss_state_msg;
		boss_state_msg.set_boss_id(boss_id);
		boss_state_msg.set_boss_state_code(boss_state);
		boss_state_msg.set_is_final_boss(is_final_boss);

		send_message_to_all_player(&boss_state_msg, e_msgindex_s2c_boss_state);
	}

	bool map_object::is_fuben_boss(const int32& unit_index)
	{
		npc& temp_npc = unit_man::get_npc(unit_index);
		if (NULL == m_map_template_ptr)
		{
			return false;
		}
		if (m_map_template_ptr->BossID == temp_npc.get_unit_info(e_role_info_template_id))
		{
			return true;
		}
		return false;
	}
	void map_object::game_over()
	{
		remove_all_monster();
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end(); ++it)
		{
			buff_man::del_buff_when_map_end(it->first);
		}
	}
	void map_object::finish_log(player& player_ref)
	{
		if (player_ref.is_valid() == false || player_ref.get_write_log() == false)
		{
			return;
		}
		if (m_map_template_ptr != nullptr)
		{
			int32 map_template_id = m_map_template_ptr->attribute_id;
			server_log::raid_fight_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), map_template_id, 1, m_is_win, map_template_id, 0, m_map_template_ptr->Difficulty, m_map_template_ptr->Type, player_ref.get_relive_times());

			set_log_var(log_head);
			player_ref.get_log_common_head_info(log_head);

			server_log::serverDungeon(log_head, init_unit::change_i32_to_string(map_template_id), init_unit::change_i32_to_string(m_map_template_ptr->Type), m_map_template_ptr->Difficulty, 0, m_is_win, 0, get_map_life_time());

		}
	}
	void map_object::finish_log(player& player_ref, int32 temp_is_win)
	{
		if (player_ref.is_valid() == false || player_ref.get_write_log() == false)
		{
			return;
		}
		if (m_map_template_ptr != nullptr)
		{
			int32 map_template_id = m_map_template_ptr->attribute_id;
			server_log::raid_fight_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), map_template_id, 1, temp_is_win, map_template_id, 0, m_map_template_ptr->Difficulty, m_map_template_ptr->Type, player_ref.get_relive_times());

			set_log_var(log_head);
			player_ref.get_log_common_head_info(log_head);

			server_log::serverDungeon(log_head, init_unit::change_i32_to_string(map_template_id), init_unit::change_i32_to_string(m_map_template_ptr->Type), m_map_template_ptr->Difficulty, 0, temp_is_win, 0, get_map_life_time()/second_tick_time);

		}
	}

	void map_object::send_team_game_over()
	{
		MapTemplate* map_temp_ptr = get_map_template_ptr();
		if (nullptr == map_temp_ptr)
		{
			return;
		}
		if (map_temp_ptr->Order != e_map_order_type_multiplayer_raid)
		{
			return;
		}

		cs2ws_team_game_over team_game_over_msg;
		team_game_over_msg.raid_map_guid = m_map_guid;
		connection_mgr::getInstance().send_to_ws( &team_game_over_msg, sizeof(team_game_over_msg));

		//如果完成，且不是助战 则增加好友度
		cs2ws_fuben_add_friendliness_value pak;
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;

			player& temp_player = unit_man::get_player(temp_it->first);
			if (false == temp_player.is_valid())
			{
				continue;
			}
			if (pak.team_num >= max_team_member_num)
			{
				break;
			}
			pak.team_member_guid[pak.team_num] = temp_player.get_unit_guid();
			pak.team_num++;
		}
		connection_mgr::getInstance().send_to_ws( &pak, sizeof(cs2ws_fuben_add_friendliness_value));
		
	}

	bool map_object::have_info_panel(e_map_info_panel_type info_panel_type)
	{
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return false;
		}
		int32 len = map_template_ptr->InfoPanelType.size();
		for (int32 i = 0; i < len; ++i)
		{
			if ((int32)info_panel_type == map_template_ptr->InfoPanelType[i])
			{
				return true;
			}
		}

		return false;
	}	
	void map_object::set_game_state_to_player(e_map_state game_state, int32 state_time, int32 unit_index)
	{
		m_map_state = game_state;
		m_map_state_time = state_time;
		send_cur_game_state_to_player(unit_index);
	}

	void map_object::send_cur_game_state_to_player(int32 unit_index /* = -1 */)
	{
		int32 cur_time = time_helper::get_cur_time_new().second;
		game_proto_game_state state_msg;
		state_msg.set_state(m_map_state);
		state_msg.set_state_time(get_time_left(unit_index));
		state_msg.set_cur_server_time(cur_time);
		if (unit_index < 0)
		{
			send_message_to_all_player(&state_msg, e_msgindex_s2c_map_game_state);
		}
		else
		{
			send_message_to_one_player(&state_msg, e_msgindex_s2c_map_game_state, unit_index);
		}
	}

	void map_object::send_cur_wave_trig_state_to_player(int32 unit_index /* = -1 */)
	{
		if (m_wave_trig_message_set.size() <= 0)
		{
			return;
		}

		for (std::set<int32>::iterator iter = m_wave_trig_message_set.begin(); iter != m_wave_trig_message_set.end(); ++iter)
		{
			game_proto_wave_trigger_message wave_trigger_message_msg;
			wave_trigger_message_msg.set_message(*iter);

			if (unit_index < 0)
			{
				send_message_to_all_player(&wave_trigger_message_msg, e_msgindex_s2c_wave_trigger_message);
			}
			else
			{
				send_message_to_one_player(&wave_trigger_message_msg, e_msgindex_s2c_wave_trigger_message, unit_index);
			}
		}
	}
	bool map_object::is_in_map_win(e_map_win_cond win_cond)
	{
		if (nullptr == m_map_template_ptr)
		{
			return false;
		}
		for (int32 i = 0; i < m_map_template_ptr->WinCond.size(); ++i)
		{
			if (win_cond == m_map_template_ptr->WinCond[i])
			{
				return true;
			}
		}
		return false;
	}
	float map_object::get_time_passed()
	{
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return 0;
		}
		
		float passed_time = 0.f;
		switch (m_map_state)
		{
		case faith::e_map_state_join:
		case faith::e_map_state_game:
			break;
		case faith::e_map_state_in_game:
		{
			float left_time = get_time_left();
			passed_time = map_template_ptr->TimeRequire - left_time;
			break;
		}
		case faith::e_map_state_over:
		case faith::e_map_state_return_data:
		{
			passed_time = map_template_ptr->TimeRequire;
			break;
		}
		default:
			break;
		}

		if (passed_time < 0)
		{
			passed_time = 0;
		}
		return passed_time;
	}

	s_map_pos map_object::get_born_pos(int32 unit_index)
	{
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return s_map_pos();
		}
		if (map_template_ptr->BornLocation.size() < 3 || map_template_ptr->BornRotation.size() < 3)
		{
			return s_map_pos();
		}
		float pos_x = map_template_ptr->BornLocation[0];
		float pos_y = map_template_ptr->BornLocation[1];
		float pos_z = map_template_ptr->BornLocation[2];
		float roll  = map_template_ptr->BornRotation[0];
		float pitch = map_template_ptr->BornRotation[1];
		float yaw   = map_template_ptr->BornRotation[2];
		s_map_pos born_pos(pos_x, pos_y, pos_z, roll, pitch, yaw);

		return born_pos;
	}

	bool map_object::add_player(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return false;
		}
		m_player_index_list[unit_index] = 1;

		MapTemplate* map_temp_ptr = get_map_template_ptr();
		if (nullptr == map_temp_ptr)
		{
			return false;
		}
		if (map_temp_ptr->Type == faith::e_map_type_big_map)
		{
			player_ref.set_main_line_id(get_line_id());
		}

		if (false == m_map_own_legion.is_valid())
		{
			m_map_own_legion = player_ref.get_legion_guid();
		}
		return on_player_add(unit_index);
	}
	int32 map_object::get_player_one()
	{
		auto it = m_player_index_list.begin();
		if (it != m_player_index_list.end())
		{
			return it->first;
		}
		return 0;
	}
	void map_object::del_player(const int32& unit_index)
	{
		auto it = m_player_index_list.find(unit_index);
		if (it == m_player_index_list.end())
		{
			return;
		}
		player& player_ref = unit_man::get_player(unit_index);
		remove_aoi(player_ref.get_old_map_pos().unit_location, unit_index, false);
		m_player_index_list.erase(it);
		
		//if (player_ref.get_unit_guid().server_64 == 75488071822154807)
		//{
		//	std::string log_str = "_1.3_ --del_player-- lv:" 
		//		+ template_manager::get_instance().int_to_string(player_ref.m_role_info.data_ary[5])
		//		+ " guid:" + template_manager::get_instance().int_to_string(player_ref.get_unit_guid().server_64)
		//		+ " arr_idx:" + template_manager::get_instance().int_to_string(player_ref.get_array_index());
		//	server_log::server_debug_log(log_str);
		//}

		// on_player_delete在m_player_map清除掉了玩家后调用
		on_player_delete(unit_index);
	}
	
	int32 map_object::get_player_num()
	{
		return m_player_index_list.size();
	}

	void map_object::player_enter_scene(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		add_aoi(player_ref.get_new_map_pos().unit_location, unit_index);
		
		on_player_enter_scene(unit_index);
		player_ref.summon_all_pet();
		
		
		if (nullptr != m_map_template_ptr)
		{
			int32 cur_player_vip = player_ref.get_vip_level(false);
			if (m_map_template_ptr->VipEnterSceneBuff.size() > cur_player_vip)
			{
				buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), m_map_template_ptr->VipEnterSceneBuff[cur_player_vip]);
			}
		}
		

		if (e_map_type_big_map != get_map_type()) //非大世界的逻辑
		{
			send_cur_game_state_to_player(unit_index);
			check_default_pk_mode(unit_index);
		}
		if (false == init_unit::get_map_public(get_map_type())) //非公共地图的逻辑
		{
			calcu_player_avg_level();
			send_cur_wave_trig_state_to_player(unit_index);		
		}

		{
			int32 map_type = get_map_type();
			int32 map_sub_type = m_map_template_ptr != nullptr ? m_map_template_ptr->SubType : e_map_type_ui;
			server_log::map_change_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_login_type(), get_map_template_id(), map_type, map_sub_type, faith::e_map_change_enter, m_map_template_ptr->Difficulty);

			set_log_var(log_head);
			player_ref.get_log_common_head_info(log_head);
			server_log::serverMapChange(log_head, get_map_template_id(), map_type, map_sub_type, faith::e_map_change_enter, m_map_template_ptr->Difficulty);
		}
		if (m_first_player_join_time <= 0)
		{
			m_first_player_join_time = utility::get_tick_count();
		}
	}

	void map_object::player_leave_scene(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		handle_remaining_drop_item_by_email(unit_index);
		player_ref.map_in_out_hp_process();
		on_player_leave_scene(unit_index);
		player_ref.kill_all_pet();

		{
			int32 map_type = get_map_type();
			int32 map_sub_type = m_map_template_ptr != nullptr ? m_map_template_ptr->SubType : e_map_type_ui;
			server_log::map_change_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_login_type(), get_map_template_id(), map_type, map_sub_type, faith::e_map_change_leave, m_map_template_ptr->Difficulty);

			set_log_var(log_head);
			player_ref.get_log_common_head_info(log_head);
			server_log::serverMapChange(log_head, get_map_template_id(), map_type, map_sub_type, faith::e_map_change_leave, m_map_template_ptr->Difficulty);
		}
	}
	
	void map_object::player_logout(const int32& unit_index)
	{
		player_leave_scene(unit_index);
	}
	
	void map_object::player_disconnect(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		remove_aoi(player_ref.get_old_map_pos().unit_location, unit_index, false);
		on_player_disconnect(unit_index);
	}
	
	void map_object::player_reconnect(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		add_aoi(player_ref.get_new_map_pos().unit_location, unit_index);
		
		if (false == init_unit::get_map_public(get_map_type()))
		{
			send_cur_game_state_to_player(unit_index);
			send_cur_wave_trig_state_to_player(unit_index);
		}

		on_player_reconnect(unit_index);
	}

	void map_object::on_player_reconnect(const int32& unit_index)
	{
	}

	void map_object::add_npc(const int32& unit_index)
	{
		npc& temp_npc = unit_man::get_npc(unit_index);
		temp_npc.set_show_map(true);
		int32 empty_index = -1;
		m_npc_index_list[unit_index] = 1;
		const s_map_pos& unit_pos = temp_npc.get_new_map_pos();
		add_aoi(unit_pos.unit_location, unit_index);

		on_npc_add(unit_index);
	}

	void map_object::del_npc(const int32& unit_index, bool is_dead)
	{
		auto it = m_npc_index_list.find(unit_index);
		if (it == m_npc_index_list.end())
		{
			return;
		}
		
		m_npc_index_list.erase(it);
		on_npc_delete(unit_index, is_dead);
		
		npc& temp_npc = unit_man::get_npc(unit_index);
		remove_aoi(temp_npc.get_old_map_pos().unit_location, temp_npc.get_array_index(), is_dead);
	}

	void map_object::unit_dead(const int32 & unit_index)
	{
		if (unit_index >= npc_arrary_index_begin)
		{
			npc_dead(unit_index);
		}
		else
		{
			player_dead(unit_index);
		}
	}

	void map_object::npc_dead(const int32 & unit_index)
	{
		on_npc_dead(unit_index);
	}

	void map_object::player_dead(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		if (e_map_type_big_map != get_map_type())
		{
			m_dead_player_map[player_ref.get_unit_guid().server_64] = utility::get_tick_count();
		}
		
		on_player_dead(unit_index);
	}

	unit_guid_map_it map_object::get_player_dead_stamp(int32 unit_index)
	{
		unit_guid_map_it tmp_it = m_dead_player_map.end();
		if (m_dead_player_map.size() <= 0)
		{
			return tmp_it;
		}

		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return tmp_it;
		}

		tmp_it = m_dead_player_map.find(player_ref.get_unit_guid().server_64);
		if (tmp_it == m_dead_player_map.end())
		{
			return tmp_it;
		}

		return tmp_it;
	}
	
	player* map_object::get_first_valid_player()
	{
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			player& player_ref = unit_man::get_player(it->first);
			if (player_ref.is_valid())
			{
				return &player_ref;
			}
		}
		return nullptr;
	}

	npc* map_object::get_first_valid_npc(int32 npc_template_id)
	{
		auto it = m_npc_index_list.begin();
		for (; it != m_npc_index_list.end(); ++it)
		{
			int32 temp_idx = it->first;
			npc& temp_npc = unit_man::get_npc(temp_idx);
			if (temp_npc.is_valid() && temp_npc.get_npc_template_id() == npc_template_id)
			{
				return &temp_npc;
			}
		}
		return nullptr;
	}

	npc* map_object::get_npc_one()
	{
		auto it = m_npc_index_list.begin();
		if (it != m_npc_index_list.end())
		{
			npc& npc_ref = unit_man::get_npc(it->first);
			if (npc_ref.is_valid())
			{
				return &npc_ref;
			}
		}
		return nullptr;
	}

	void map_object::caclu_now_player_avg_level() 
	{
		int32 max_player_num = 0;
		int32 max_exp_level_num = 0;
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& temp_player = unit_man::get_player(temp_it->first);
			if (!temp_player.is_valid())
			{
				continue;
			}
			max_player_num++;
			max_exp_level_num += temp_player.get_unit_info(e_role_info_exp_level);
		}
		int32 average_exp_level = 0;
		if (max_player_num > 0)
		{
			average_exp_level = max_exp_level_num / max_player_num;
			if (average_exp_level < 0)
			{
				average_exp_level = 0;
			}
		}
		m_now_player_avg_lv = average_exp_level;
	}
	
	void map_object::calcu_player_avg_level()
	{
		int32 max_player_num = 0;
		int32 max_exp_level_num = 0;
		for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& temp_player = unit_man::get_player(temp_it->first);
			if (!temp_player.is_valid())
			{
				continue;
			}
			max_player_num++;
			max_exp_level_num += temp_player.get_unit_info(e_role_info_exp_level);
		}
		int32 average_exp_level = 0;
		if (max_player_num > 0)
		{
			average_exp_level = max_exp_level_num / max_player_num;
			if (average_exp_level < 0)
			{
				average_exp_level = 0;
			}
		}
		m_player_avg_lv = average_exp_level;
	}

	int32 map_object::get_npc_num()
	{
		return m_npc_index_list.size();
	}

	void map_object::kill_all_monster(const int32& unit_array_index, int32 buff_damage_num)
	{
		for (auto it = m_npc_index_list.begin(); it != m_npc_index_list.end(); )
		{
			auto temp_it = it++;
			buff_ex_env_param penv;
			penv.damage_num = buff_damage_num;
			penv.buff_level = 1;
			npc& temp_npc = unit_man::get_npc(temp_it->first);
			if (temp_npc.get_unit_type() != e_unit_type_monster)
			{
				continue;
			}

			//会走被玩家杀掉的逻辑
			buff_man::add_buff_inst(unit_array_index, temp_npc.get_array_index(), GM_DAMAGE_BUFF_ID, &penv);
		}
	}

	void map_object::remove_all_monster()
	{
		for (auto it = m_npc_index_list.begin(); it != m_npc_index_list.end(); )
		{
			auto temp_it = it++;
			int32 npc_index = temp_it->first;
		
			//友方NPC不删除
			npc& temp_npc = unit_man::get_npc(npc_index);
			if (temp_npc.get_unit_type() == e_unit_type_friend_npc || e_unit_type_drop_bag == temp_npc.get_unit_type()
				|| (temp_npc.get_unit_type() == e_unit_type_npc && temp_npc.get_unit_sub_type() == e_npc_type_cross_pk_chests)
				|| (temp_npc.get_unit_type() == e_unit_type_npc && temp_npc.get_unit_sub_type() == e_npc_type_world_boss_chests))//防止掉落包被清掉
			{
				continue;
			}

			unit_man::remove_npc(npc_index, false);
		}
	}
	
	npc* map_object::spawn_npc(int32 spawn_point_id, int32 dynamic_lv)
	{
		return world_cs::spawn_npc(spawn_point_id, get_entity(get_map_guid()));
	}

	void map_object::send_message_to_all_player(google::protobuf::Message* net_pro, uint32 header)
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& temp_player = unit_man::get_player(temp_it->first);
			if (temp_player.is_valid())
			{
				temp_player.send_message_to_self(net_pro, header);
			}
		}
	}
	
	void map_object::send_message_to_one_player(google::protobuf::Message* net_pro, uint32 header, int32 unit_index)
	{
		player& temp_player = unit_man::get_player(unit_index);
		if (temp_player.is_valid())
		{
			temp_player.send_message_to_self(net_pro, header);
		}
	}
	
	void map_object::transfer_all_player(int32 map_template_id)
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			//unit_index_map_it& _it = it;
			auto temp_it = it++;
			
			player& temp_player = unit_man::get_player(temp_it->first);
			if (temp_player.is_valid())
			{
				int32 map_line_id = 0;
				if (0 == map_template_id)
				{
					map_template_id = temp_player.get_unit_info(e_role_info_main_map_id);
					map_line_id = temp_player.get_main_line_id();
				}
				// 如果是个人打宝地图离开时复原Pk模式
				if (get_map_type() == e_map_type_single_land_boss)
				{
					int32 main_pk_mode = temp_player.get_logic_data(e_role_logic_info_main_pk_mode);
					temp_player.get_pk_community_mgr().set_entire_pk_mode(main_pk_mode);
				}
				temp_player.transfer_by_template(map_template_id, map_line_id, temp_player.get_unit_info(e_role_info_server_id), guid_64(), 0);
			}
		}
	}

	void map_object::send_notice_to_all_player(std::string notice_str)
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& player_ref = unit_man::get_player(temp_it->first);
			if (player_ref.is_valid())
			{
				player_ref.send_notice(notice_str);
			}
		}
	}

	void map_object::send_notice_to_all_player(int32 notice_id, std::string notice_str)
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& player_ref = unit_man::get_player(temp_it->first);
			if (player_ref.is_valid())
			{
				player_ref.get_chat_mgr().send_notice(notice_id, notice_str);
			}
		}
	}
	
	void map_object::check_default_pk_mode(const int32& unit_index)
	{
		//现在的逻辑需要部分进入地图改pk模式 比如打宝地图
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return;
		}
		
		if (map_template_ptr->DefaultPkMode < 0 || map_template_ptr->DefaultPkMode >= e_pk_mode_max)
		{
			return;
		}

		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		if (map_template_ptr->DefaultPkMode == e_pk_mode_family && !player_ref.get_legion_guid().is_valid())
		{
			return;
		}
		player_ref.get_pk_community_mgr().set_pk_mode(map_template_ptr->DefaultPkMode);
	}

	e_pk_mode_type map_object::get_map_pk_mode()
	{
		if (m_map_pk_mode >= e_pk_mode_type_cant && m_map_pk_mode < e_pk_mode_type_max)
		{
			return (e_pk_mode_type)m_map_pk_mode;
		}
		
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return e_pk_mode_type_cant;
		}
		int32 pk_type = map_template_ptr->IsAllowPK;
		return (e_pk_mode_type)pk_type;
	}

	void map_object::set_map_pk_mode(e_pk_mode_type mode_type)
	{
		m_map_pk_mode = mode_type;
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& player_ref = unit_man::get_player(temp_it->first);
			if (player_ref.is_valid())
			{
				player_ref.get_pk_community_mgr().init_map_pk_type();
			}
		}
	}

	void map_object::handle_remaining_drop_item_by_email(const int32 unit_index)
	{
		if (e_map_type_big_map == get_map_type())
		{
			return;
		}
		player& temp_player = unit_man::get_player(unit_index);
		std::vector<s_item_template_info> drop_list_vec;
		drop_list_vec.clear();
		for (auto it = m_npc_index_list.begin(); it != m_npc_index_list.end(); )
		{
			auto temp_it = it++;
			npc& temp_npc = unit_man::get_npc(temp_it->first);
			if (false == temp_npc.is_valid() || temp_npc.get_unit_type() != e_unit_type_drop_bag)
			{
				continue;
			}
			const guid_64& player_guid = temp_npc.get_owner().unit_guid;
			if (player_guid != temp_player.get_unit_guid())
			{
				continue;
			}

			s_item_template_info item_drop_list_with_num = temp_player.get_item_set().get_const_att_item_by_id(temp_npc.get_item_id(), 1, 0, 0, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));
			globle_data::get_instance().add_to_drop_list(drop_list_vec, item_drop_list_with_num);
			unit_man::remove_npc(temp_it->first, false);
		}

		if (drop_list_vec.size() <= 0)
		{
			return;
		}

		std::vector<std::string> content_params_title;
		content_params_title.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_get_remaining_title));
		std::string title = init_unit::implode(content_params_title);

		std::vector<std::string> content_params_contenttext;
		content_params_contenttext.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_get_remaining_context));
		std::string contenttext = init_unit::implode(content_params_contenttext);
		globle_data::get_instance().send_mail_with_item(temp_player.get_unit_guid(), temp_player.get_unit_info(e_role_info_server_id), drop_list_vec, title, contenttext);
		
	}

	bool map_object::have_be_assisted_mem(guid_64 team_guid)
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& player_ref = unit_man::get_player(temp_it->first);
			if (false == player_ref.is_valid())
			{
				continue;
			}
			if (player_ref.get_team_guid() == team_guid && true == player_ref.get_team_cs_mgr().is_assist_in_raid())
			{
				return true;
			}
		}
		return false;
	}

	int32 map_object::get_activity_map_sec_left(int32 act_map_type, int32 player_idx)
	{
		e_activity_type _activity_type = e_activity_type_max;
		if (act_map_type == e_map_type_crystak_dreamland)
		{
			_activity_type = e_activity_type_crystal_fairyland;
		}
		else if (act_map_type == e_map_type_field)
		{
			// _activity_type = e_activity_type_ancient_field;
			player& player_ref = unit_man::get_player(player_idx);
			if (false == player_ref.is_valid())
			{
				return 0;
			}
			return player_ref.get_time_data(e_time_type_ancient_battelfield) / second_tick_time; //消耗完每日时间后为0
		}
		else if (act_map_type == e_map_type_broken_sky)
		{
			_activity_type = e_activity_type_broken_sky;
		}
		else if (act_map_type == e_map_type_king_of_pk)
		{
			_activity_type = e_activity_type_pk_king;
		}
		else
		{
			return 1;
		}

		return cell_server::getInstance().get_activity_sec_left(_activity_type, e_activity_time_get_gaming);
	}

	void map_object::add_buff_to_all_player(int32 buff_temp_id)
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& temp_player = unit_man::get_player(temp_it->first);
			if (!temp_player.is_valid())
			{
				continue;
			}
			buff_man::add_buff_inst(temp_player.get_array_index(), temp_player.get_array_index(), buff_temp_id);
		}
	}

	void map_object::get_transfer_map_pos(int32 unit_index, s_map_pos& tem_pos, int32 war_index)
	{
	}

	void map_object::relive_all_player()
	{
		for (auto it = m_player_index_list.begin(); it != m_player_index_list.end();)
		{
			auto temp_it = it++;
			player& temp_player = unit_man::get_player(temp_it->first);
			if (!temp_player.is_valid())
			{
				continue;
			}

			if (temp_player.is_dead())
			{
				temp_player.back_to_life_with_map_end();
			}
		}
	}
}