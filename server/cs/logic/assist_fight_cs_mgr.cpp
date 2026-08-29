#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "assist_fight_cs_mgr.h"
#include "server_log.hpp"
#include "logic/world_cs.h"
#include "base/ecs_world.h"
#include "components/scene/base_map_component.h"
#include "system/scene/base_map_system.h"
#include "cell_server.hpp"
#include "internal/world_boss_msg.hpp"
#include "npc.hpp"
#include "internet/assist_fight.pb.h"
#include "internet/net.pb.h"

namespace faith
{
	assist_fight_cs_mgr::assist_fight_cs_mgr()
	{
		m_array_index = 0;
		m_assist_fight_info.clear_data();
	}
	assist_fight_cs_mgr::~assist_fight_cs_mgr()
	{
	}
	void assist_fight_cs_mgr::clear_data()
	{
		m_assist_fight_info.clear_data();
	}
	//bool assist_fight_cs_mgr::check_can_assist_fight(int32 assist_fight_tag, int32 map_template_id, guid_64 map_guid)
	//{
	//	player& player_ref = unit_man::get_player(m_array_index);
	//	if (player_ref.is_lock_map_mission(map_template_id) == false)
	//	{
	//		return false;
	//	}
	//	if (get_assist_fight_guid().is_valid())
	//	{
	//		return false;//正在助战中
	//	}
	//	if (assist_fight_tag == 0)
	//	{
	//		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
	//		if (map_template_ptr == nullptr)
	//		{
	//			return false;
	//		}
	//		if (player_ref.get_unit_info(e_role_info_exp_level) < map_template_ptr->EnterLevel)
	//		{
	//			return false;
	//		}
	//		if (!player_ref.check_grade_level_enough(map_template_ptr->EnterGradeLevel, map_template_ptr->EnterLevel, false))
	//		{
	//			return false;
	//		}
	//		if (map_template_ptr->Type == e_map_type_boss_home || //地宫boss
	//			map_template_ptr->Type == e_map_type_boss_island || //天空岛
	//			map_template_ptr->Type == e_map_type_boss_vip_home || //boss之家
	//			map_template_ptr->Type == e_map_type_belief_cloister ||//信仰回廊
	//			map_template_ptr->Type == e_map_type_field ||//上古遗迹   
	//			map_template_ptr->Type == e_map_type_big_map)
	//		{
	//			return false;
	//		}
	//		else
	//		{
	//			player_ref.send_notice("90204165");
	//		}
	//	}
	//	return true;
	//}

	void assist_fight_cs_mgr::start_assist_fight(int32 assist_fight_type,int32 assist_fight_tag)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.m_status != e_session_status_in_gaming)
		{
			return;
		}
		if (is_assist_helper())
		{
			return;
		}
		//if (m_assist_fight_info.asssit_fight_guid.is_valid())
		//{
		//	cs2ws_start_assist_fight re_msg;
		//	re_msg.m_assist_fight_info = m_assist_fight_info;
		//	if (player_ref.is_self_server())
		//	{
		//		connection_mgr::getInstance().send_to_ws(&re_msg, sizeof(re_msg));
		//	}
		//	else
		//	{
		//		connection_mgr::getInstance().send_to_ws(&re_msg, sizeof(re_msg), player_ref.get_unit_info(e_role_info_server_id));
		//	}
		//	player_ref.send_notice("90204194");
		//	return;
		//}
		//只要发起请求就算完成 不需要判断是否可以发送
		player_ref.set_logic_data(e_role_logic_info_help_tip_send_times, player_ref.get_logic_data(e_role_logic_info_help_tip_send_times) + 1);
		player_ref.get_mission_mgr().target_check(e_mission_end_type_by_assis_fight_num);
		if (assist_fight_type == e_assist_fight_type_legion)
		{
			if (!player_ref.get_legion_guid().is_valid())
			{
				player_ref.send_notice("90204162");
				return;
			}
		}
		
		int32 map_id = 0;
		guid_64 map_guid = player_ref.get_map_guid();
		int32 boss_id = 0;
		guid_64 boss_guid;
		faith::s_map_pos m_pos;

		auto map_type = base_map_system::get_map_type(player_ref.get_map_ent());
		auto map_template_id = base_map_system::get_map_template_id(player_ref.get_map_ent());

		if (assist_fight_tag == 0)
		{
			//boss
			map_id = map_template_id;

			if (map_type == e_map_type_boss_home || //地宫boss
				map_type == e_map_type_boss_island || //天空岛
				map_type == e_map_type_boss_vip_home || //boss之家
				map_type == e_map_type_single_assist_boss // 个人助战boss
				)
			{
				if (!m_assist_fight_info.boss_guid.is_valid())
				{
					return;
				}
				npc &aim_boss = unit_man::get_npc(get_assist_fight_boss_id());
				boss_id = aim_boss.get_npc_template_id();
				NpcTemplate * npc_temp_ptr = aim_boss.get_npc_template();
				if (npc_temp_ptr != nullptr && npc_temp_ptr->PathFindLocation.size() >= 3)
				{
					m_pos.set_location(player_ref.get_unit_info(faith::e_role_info_move_pos_x), player_ref.get_unit_info(faith::e_role_info_move_pos_y), player_ref.get_unit_info(faith::e_role_info_move_pos_z));
				}
				boss_guid = aim_boss.get_unit_guid();
				if (map_type == e_map_type_boss_island)
				{
					map_guid.clear_data();
				}
			}
			else
			{
				//其他副本不可发起助战
				return;
			}
		}
		else if (assist_fight_tag == 1)
		{
			if (map_type != e_map_type_big_map)
			{
				player_ref.send_notice("90300078");
				//只有大世界地图可以发起军团副本求助
				return;
			}
			s_player_team_info& m_team_info = player_ref.get_team_info();
			MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, m_team_info.team_sub_type_id);
			if (nullptr == map_temp_ptr)
			{
				return;
			}
			if (map_temp_ptr->Order != e_map_order_type_multiplayer_raid)
			{
				return;
			}
			map_id = m_team_info.team_sub_type_id;
		}
		//存储主要助战信息
		m_assist_fight_info.asssit_fight_guid = player_ref.get_unit_guid();
		memcpy(m_assist_fight_info.role_name, player_ref.get_name(), max_name_size);
		m_assist_fight_info.role_lv = player_ref.get_unit_info(e_role_info_exp_level);
		m_assist_fight_info.role_professional = player_ref.get_unit_info(e_role_info_class_type);
		m_assist_fight_info.role_head_icon = player_ref.get_unit_info(e_role_info_head_frame);
		m_assist_fight_info.assist_fight_type = assist_fight_type;
		m_assist_fight_info.boss_id = boss_id;
		m_assist_fight_info.boss_guid = boss_guid;
		m_assist_fight_info.map_id = map_id;
		m_assist_fight_info.map_guid = map_guid;
		m_assist_fight_info.map_pos = m_pos;
		m_assist_fight_info.assist_fight_tag = assist_fight_tag;
		m_assist_fight_info.legion_guid = player_ref.get_legion_guid();

		cs2ws_start_assist_fight msg;
		msg.m_assist_fight_info = m_assist_fight_info;
		if (player_ref.is_self_server())
		{
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		}
		else
		{
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), player_ref.get_unit_info(e_role_info_server_id));
		}
		sync_assist_fight_state();
		player_ref.send_notice("90204168");
	}
	//void assist_fight_cs_mgr::req_assist_fight(guid_64 assist_fight_guid)
	//{
	//	player& player_ref = unit_man::get_player(m_array_index);
	//	if (!player_ref.is_self_server())
	//	{
	//		return;
	//	}
	//	player& player_main = unit_man::get_player(assist_fight_guid);
	//	if (player_main.m_status != e_session_status_in_gaming)
	//	{
	//		return;
	//	}
	//	s_assist_fight_info& m_info = player_main.get_assist_fight_mgr().get_assist_fight_info();
	//	if (!m_info.asssit_fight_guid.is_valid())
	//	{
	//		return;
	//	}
	//	bool m_check_result = check_can_assist_fight(m_info.assist_fight_tag, m_info.map_id, m_info.map_guid);
	//	if (m_info.assist_fight_tag == 1)
	//	{
	//		if (!player_main.get_team_info().is_valid())
	//		{
	//			return;
	//		}
	//		if (player_main.get_team_info().member_num >= max_team_member_num)
	//		{
	//			team_proto_team_error team_error_msg;
	//			team_error_msg.set_error_type(e_team_error_team_member_is_full);
	//			player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
	//			return;
	//		}
	//	}
	//	if (!m_check_result)
	//	{
	//		return;
	//	}
	//	if (!m_info.insert_assist_list(player_ref.get_unit_guid()))
	//	{
	//		return;
	//	}
	//	cs2ws_check_can_assist_fight_end msg;
	//	msg.assist_fight_guid = assist_fight_guid;
	//	msg.check_result = true;
	//	msg.role_guid = player_ref.get_unit_guid();
	//	connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
	//}
	void assist_fight_cs_mgr::cancel_assist_fight(bool need_transfer)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (!get_assist_fight_guid().is_valid())
		{
			return;
		}
		cs2ws_cancel_assist_fight msg;
		int32 cancel_type = 0;
		if (get_assist_fight_guid() == player_ref.get_unit_guid())
		{
			msg.cancel_type = 1;
			msg.assist_fight_guid = m_assist_fight_info.asssit_fight_guid;
			s_assist_fight_info &m_info = get_assist_fight_info();
			for (int32 i = 0; i < assist_fight_max_num; i++)
			{
				if (m_info.assist_guid_list[i].is_valid())
				{
					msg.role_guid = m_info.assist_guid_list[i];
					player& helper_ref = unit_man::get_player(m_info.assist_guid_list[i]);
					helper_ref.get_assist_fight_mgr().clear_data();
					helper_ref.get_assist_fight_mgr().sync_assist_fight_state();
					connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
				}
			}
		}
		else
		{
			cancel_type = 1;
		}
		//if (need_transfer)
		//{
		//	//传出地图
		//	player_ref.transfer_by_template(player_ref.get_unit_info(faith::e_role_info_main_map_id), player_ref.get_main_line_id(), 0, guid_64(), 0);
		//}
		msg.cancel_type = cancel_type;
		msg.role_guid = player_ref.get_unit_guid();
		msg.assist_fight_guid = m_assist_fight_info.asssit_fight_guid;
		if (player_ref.is_self_server())
		{
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		}
		else
		{
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), player_ref.get_unit_info(e_role_info_server_id));
		}
		guid_64 tmp_boss_guid = m_assist_fight_info.boss_guid;

		npc& boss_ref = unit_man::get_npc(tmp_boss_guid);
		if (cancel_type == 1)
		{
			if (boss_ref.is_valid())
			{
				boss_ref.transfer_damage_to_master(player_ref.get_unit_guid(), player_ref.get_array_index());
			}
			player& main_player = unit_man::get_player(m_assist_fight_info.asssit_fight_guid);
			if (main_player.is_valid())
			{
				main_player.get_assist_fight_mgr().remove_assist_helper(player_ref.get_unit_guid());
				main_player.get_assist_fight_mgr().sync_assist_fight_state_to_all_mem();
			}
		}
		else
		{
			if (boss_ref.is_valid())
			{
				boss_ref.clear_all_damage(player_ref.get_unit_guid(), true);
			}
		}
		clear_data();
		sync_assist_fight_state();
	}
	void assist_fight_cs_mgr::remove_assist_helper(guid_64 helper_guid) 
	{
		m_assist_fight_info.remove_assist_list(helper_guid);
		sync_assist_fight_state();
	}
	void assist_fight_cs_mgr::assist_fight_end_to_show_reward(std::vector<guid_64>& assist_fight_helper_list, int32 reward_id1, std::vector<int32>& reward_id2, const std::vector< s_item_template_info >& drop_id_list, xstring first_name)
	{

		player& player_ref = unit_man::get_player(m_array_index);
		faith::assist_fight_proto_assist_fight_end_to_show_reward msg;
		faith::assist_fight_proto_assist_fight_end_to_show_thank_reward thank_msg;
		
		thank_msg.set_role_guid_a(m_assist_fight_info.asssit_fight_guid.A);
		thank_msg.set_role_guid_b(m_assist_fight_info.asssit_fight_guid.B);
		thank_msg.set_role_name(player_ref.get_name());
		thank_msg.set_role_lv(player_ref.get_unit_info(e_role_info_exp_level));
		thank_msg.set_role_professional(player_ref.get_unit_info(e_role_info_class_type));
		//thank_msg.set_reward_id(reward_id2);
		thank_msg.set_head_icon(player_ref.get_unit_info(e_role_info_head_frame));
		thank_msg.set_first_name(first_name);
		
		int32 m_size = assist_fight_helper_list.size();
		for (int32 i = 0; i < m_size;i++)
		{
			if (i >= assist_fight_max_num)
			{
				break;
			}
			guid_64 help_guid = assist_fight_helper_list[i];
			if (!help_guid.is_valid())
			{
				continue;
			}
			player& helper_ref = unit_man::get_player(help_guid);
			if (helper_ref.m_status == e_session_status_in_gaming)
			{
				faith::assist_fight_proto_assist_helper_info *help_info = msg.add_helper_list();
				help_info->set_role_guid_a(help_guid.A);
				help_info->set_role_guid_b(help_guid.B);
				help_info->set_role_name(helper_ref.get_name());
				help_info->set_role_lv(helper_ref.get_unit_info(e_role_info_exp_level));
				help_info->set_role_professional(helper_ref.get_unit_info(e_role_info_class_type));
				help_info->set_head_icon(helper_ref.get_unit_info(e_role_info_head_frame));

				thank_msg.set_reward_id(reward_id2[i]);
				helper_ref.send_message_to_self(&thank_msg, e_msgindex_s2c_assist_fight_end_to_show_thank_reward);
			}
		}
		msg.set_role_guid_a(m_assist_fight_info.asssit_fight_guid.A);
		msg.set_role_guid_b(m_assist_fight_info.asssit_fight_guid.B);
		msg.set_reward_id1(reward_id1);
		//msg.set_reward_id2(reward_id2);
		msg.set_reward_id2(0);
		msg.set_map_id(m_assist_fight_info.map_id);
		msg.set_first_name(first_name);
		int32 m_drop_size = drop_id_list.size();
		for (int32 i = 0 ; i < m_drop_size;i++)
		{
			msg.add_drop_id_list(drop_id_list[i].m_item_id);
		}
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_assist_fight_end_to_show_reward);
		//cancel_assist_fight(); 
	}
	//干掉
	void assist_fight_cs_mgr::update_assist_fight_info(s_assist_fight_info m_info)
	{
		//下面只走本服逻辑
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() && m_assist_fight_info.asssit_fight_guid.is_valid() == false && m_assist_fight_info.boss_guid.is_valid())
		{
			//对于之前在打boss的玩家，如果他去助战了就从伤害列表中清除
			if (m_info.asssit_fight_guid.is_valid() && m_info.asssit_fight_guid != player_ref.get_unit_guid())
			{
				npc& boss_ref = unit_man::get_npc(m_assist_fight_info.boss_guid);
				if (boss_ref.is_valid())
				{
					boss_ref.transfer_damage_to_master(player_ref.get_unit_guid(), player_ref.get_array_index());
				}
			}
		}
		m_assist_fight_info = m_info;
		if (get_assist_type() == e_assist_fight_type_server)
		{
			player_ref.get_competition_mgr().get_competition_info().assist_fight_guid = m_info.asssit_fight_guid;
		}
		else
		{
			if (m_info.assist_fight_tag == 1)
			{
				//军团求助不走助战关系
				return;
			}
			player& player_main_ref = unit_man::get_player(m_info.asssit_fight_guid);
			s_assist_fight_info & m_main_info = player_main_ref.get_assist_fight_mgr().get_assist_fight_info();
			if (m_main_info.asssit_fight_guid.is_valid())
			{
				m_main_info.insert_assist_list(player_ref.get_unit_guid());
				player_main_ref.get_assist_fight_mgr().sync_assist_fight_state();
				player_main_ref.get_assist_fight_mgr().sync_assist_fight_state_to_all_mem();
			}
		}
		sync_assist_fight_state();
	}
	void assist_fight_cs_mgr::check_enter_assist_map()
	{
		if (is_assist_helper())
		{
			player& player_ref = unit_man::get_player(m_array_index);
			if (m_assist_fight_info.boss_id > 0)
			{
				NpcTemplate* _npc_template_ptr = GET_TEMPLATE(NpcTemplate, m_assist_fight_info.boss_id);
				if (nullptr == _npc_template_ptr)
				{
					return;
				}
				const std::string& boss_name = template_manager::get_instance().get_str_by_string_template_id(_npc_template_ptr->NpcName);
				//被助战者提示
				/*std::string notice_str_id = "90204169";
				std::vector<std::string> notice_str_params_vec;
				notice_str_params_vec.push_back(notice_str_id);
				notice_str_params_vec.push_back(player_ref.get_name());
				
				notice_str_params_vec.push_back(boss_name);
				std::string notice_str = init_unit::implode(notice_str_params_vec);*/
				player& main_ref = unit_man::get_player(m_assist_fight_info.asssit_fight_guid);
				if (false == main_ref.is_valid())
				{
					return;
				}
				/*if (m_assist_fight_info.assist_fight_type == e_assist_fight_type_legion)
				{
					main_ref.send_notice(notice_str);
				}*/
				//助战者提示
				/*notice_str_params_vec.clear();
				notice_str_id = "90204170";
				notice_str_params_vec.push_back(notice_str_id);
				notice_str_params_vec.push_back(main_ref.get_name());
				notice_str_params_vec.push_back(boss_name);
				notice_str = init_unit::implode(notice_str_params_vec);
				player_ref.send_notice(notice_str);
				*/
				if (main_ref.get_legion_guid().is_valid())
				{
					int32 notice_id = 93000429;
					std::vector<std::string> vec_notice_str;
					vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
					vec_notice_str.push_back(player_ref.get_name());
					vec_notice_str.push_back(main_ref.get_name());
					vec_notice_str.push_back(boss_name);
					std::string new_notice_str = init_unit::implode(vec_notice_str);
					std::vector<int32> param_arr;
					world_cs::send_notice_with_param(main_ref.get_unit_guid(), "", new_notice_str, param_arr, e_notice_param_open_widget, e_chat_type_legion);
				}
			}
		}
	}
	void assist_fight_cs_mgr::check_leave_assist_map()
	{
		if (is_assist_helper())
		{
			player& player_ref = unit_man::get_player(m_array_index);
			auto map_template_id = base_map_system::get_map_template_id(player_ref.get_map_ent());
			if (map_template_id == m_assist_fight_info.map_id)
			{
				cancel_assist_fight(false);
			}
		}
	}
	void assist_fight_cs_mgr::check_assist_fight_boss_island()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		auto map_entity = player_ref.get_map_ent();
		if (nullptr == map_entity)
		{
			CONSOLE_ERROR("map_entity is nullptr");
			return;
		}
		auto base_map_cp = map_entity->get_component<base_map_component>();
		if (false == base_map_cp.isValid())
		{
			CONSOLE_ERROR("base_map_cp is nullptr");
			return;
		}
		if (base_map_cp->m_map_template->Type == e_map_type_boss_island)
		{
			player_ref.set_logic_data(e_role_logic_info_main_pk_mode, e_pk_mode_peace);
			//如果当前是天空岛，去尝试获取助战信息
			guid_64  &assist_fight_guid = player_ref.get_competition_mgr().get_competition_info().assist_fight_guid;
			if (assist_fight_guid.is_valid())
			{
				player& main_player_ref = unit_man::get_player(assist_fight_guid);
				s_assist_fight_info &m_info = main_player_ref.get_assist_fight_mgr().get_assist_fight_info();
				if (m_info.asssit_fight_guid.is_valid())
				{
					m_assist_fight_info.asssit_fight_guid = m_info.asssit_fight_guid;
					memcpy(m_assist_fight_info.role_name, m_info.role_name, max_name_size);
					m_assist_fight_info.role_lv = m_info.role_lv;
					m_assist_fight_info.role_professional = m_info.role_professional;
					m_assist_fight_info.role_head_icon = m_info.role_head_icon;
					m_assist_fight_info.assist_fight_type = m_info.assist_fight_type;
					m_assist_fight_info.boss_id = m_info.boss_id;
					m_assist_fight_info.boss_guid = m_info.boss_guid;
					m_assist_fight_info.map_id = m_info.map_id;
					m_assist_fight_info.map_guid = m_info.map_guid;
					m_assist_fight_info.map_pos = m_info.map_pos;
					m_assist_fight_info.assist_fight_tag = m_info.assist_fight_tag;
					m_assist_fight_info.legion_guid = m_info.legion_guid;

					m_info.insert_assist_list(player_ref.get_unit_guid());
					main_player_ref.get_assist_fight_mgr().sync_assist_fight_state();
					main_player_ref.get_assist_fight_mgr().sync_assist_fight_state_to_all_mem();
					player_ref.transfer_local_pos(m_info.map_pos);
					//被助战者提示
					std::string notice_str_id = "90204169";
					std::vector<std::string> notice_str_params_vec;
					notice_str_params_vec.push_back(notice_str_id);
					notice_str_params_vec.push_back(player_ref.get_name());
					NpcTemplate* _npc_template_ptr = GET_TEMPLATE(NpcTemplate, m_info.boss_id);
					if (nullptr != _npc_template_ptr)
					{
						const std::string& boss_name = template_manager::get_instance().get_str_by_string_template_id(_npc_template_ptr->NpcName);
						notice_str_params_vec.push_back(boss_name);
						std::string notice_str = init_unit::implode(notice_str_params_vec);
						main_player_ref.send_notice(notice_str);
					}
					
					
				}
			}
			assist_fight_guid.clear_data();
		}
	}
	void assist_fight_cs_mgr::go_to_assist()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (!player_ref.is_self_server())
		{
			return;
		}
		if (!m_assist_fight_info.asssit_fight_guid.is_valid())
		{
			return;
		}
		if (m_assist_fight_info.assist_fight_tag == 0)
		{
			int32 group_id = 0;
			if (get_assist_type() == e_assist_fight_type_server)
			{
				group_id = cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross);
				s_map_pos m_pos;
				player_ref.transfer_to_assist_fight(m_assist_fight_info.map_id, m_assist_fight_info.map_guid, group_id, m_pos);
				clear_data();
			}
			else
			{
				group_id = player_ref.get_unit_info(e_role_info_server_id);
				player_ref.transfer_to_assist_fight(m_assist_fight_info.map_id, m_assist_fight_info.map_guid, group_id, m_assist_fight_info.map_pos);
				if (player_ref.get_map_guid().server_64 == m_assist_fight_info.map_guid)
				{
					check_enter_assist_map();
				}
			}
		}
		else if(m_assist_fight_info.assist_fight_tag == 1)
		{
			//加入队伍
			player& aim_player = unit_man::get_player(m_assist_fight_info.asssit_fight_guid);
			if (aim_player.is_valid() && aim_player.is_player_in_team())
			{
				cs2ws_join_team_assist_fight join_team_msg;
				join_team_msg.team_guid = aim_player.get_team_guid();
				player_ref.get_team_cs_mgr().gen_player_team_member_info(join_team_msg.member_info);
				connection_mgr::getInstance().send_to_ws(&join_team_msg, sizeof(join_team_msg));	
			}
			cancel_assist_fight();
		}
		
	}
	bool assist_fight_cs_mgr::is_assist_helper()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		return m_assist_fight_info.asssit_fight_guid.is_valid() && m_assist_fight_info.asssit_fight_guid != player_ref.get_unit_guid();
	}
	bool assist_fight_cs_mgr::check_cant_join_team()
	{
		return is_assist_helper() && m_assist_fight_info.assist_fight_tag == 0;
	}
	bool assist_fight_cs_mgr::is_assist_main_player()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		return m_assist_fight_info.asssit_fight_guid.is_valid() && m_assist_fight_info.asssit_fight_guid == player_ref.get_unit_guid();
	}

	void assist_fight_cs_mgr::sync_assist_fight_state(bool is_login)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		int32 state = 0;
		faith::assist_fight_proto_sync_assist_fight_state m_info_msg;
		m_info_msg.set_assist_fight_guid(0);
		m_info_msg.set_boss_id(0);
		m_info_msg.set_map_id(0);
		m_info_msg.set_role_lv(0);
		m_info_msg.set_role_name("");
		m_info_msg.set_role_professional(0);
		if (m_assist_fight_info.assist_fight_tag == 1)
		{
			state = e_assist_fight_state_type_none;
		}
		else if(m_assist_fight_info.asssit_fight_guid.is_valid())
		{
			if (player_ref.get_unit_guid() == m_assist_fight_info.asssit_fight_guid)
			{	
				state = e_assist_fight_state_type_main_player;

			}
			else
			{
				state = e_assist_fight_state_type_helper;
			}
			for (int32 i = 0; i < assist_fight_max_num; i++)
			{
				if (m_assist_fight_info.assist_guid_list[i].is_valid())
				{
					m_info_msg.add_helper_list(m_assist_fight_info.assist_guid_list[i]);
				}
			}
			m_info_msg.set_assist_fight_guid(m_assist_fight_info.asssit_fight_guid);
			m_info_msg.set_boss_id(m_assist_fight_info.boss_id);
			m_info_msg.set_map_id(m_assist_fight_info.map_id);
			m_info_msg.set_role_lv(m_assist_fight_info.role_lv);
			m_info_msg.set_role_name(m_assist_fight_info.role_name);
			m_info_msg.set_role_professional(m_assist_fight_info.role_professional);
			m_info_msg.set_head_icon(m_assist_fight_info.role_head_icon);
		}
		else
		{
			state = e_assist_fight_state_type_none;
		}
		m_info_msg.set_state(state);
		player_ref.send_message_to_self(&m_info_msg, e_msgindex_s2c_sync_assist_fight_state);
	}
	void assist_fight_cs_mgr::check_assist_fight_legion_team()
	{
		if (m_assist_fight_info.assist_fight_tag == 1)
		{
			cancel_assist_fight();
		}
	}

	void assist_fight_cs_mgr::sync_assist_fight_state_to_all_mem()
	{
		for (int32 i = 0; i < assist_fight_max_num; i++)
		{
			if (m_assist_fight_info.assist_guid_list[i].is_valid())
			{
				player& helper_ref = unit_man::get_player(m_assist_fight_info.assist_guid_list[i]);
				helper_ref.get_assist_fight_mgr().m_assist_fight_info = m_assist_fight_info;
				helper_ref.get_assist_fight_mgr().sync_assist_fight_state();
			}
		}
	}
}