#include "assist_fight_mgr.h"
#include "template/template_manager.h"
#include "../ws/server/client_session_mgr.hpp"
#include <world_boss_msg.hpp> 
#include "../ws/server/legion/legion_ws_mgr.h"
#include "../ws/server/team/team_ws_mgr.h"
#include "../ws/server/mail/event_ws_mgr.h"
#include "utility/parse_msg.h"
#include "utility/init_unit.h"

#include "system/scene/cs_map_system.h"
#include "assist_fight.pb.h"
#include "net.pb.h"
namespace faith
{
	assist_fight_mgr::assist_fight_mgr()
	{
	}

	assist_fight_mgr::~assist_fight_mgr()
	{
	}
	void assist_fight_mgr::start_assist_fight(const s_assist_fight_info & m_in_info)
	{
		if (is_use_lua())
		{
			faith::st_proto::st_assist_fight_info st_info;
			s_assist_fight_info& tmp = const_cast<s_assist_fight_info &>(m_in_info);
			tmp.to_proto(&st_info);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&st_info);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_start_assist_fight", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}


		
		if (m_assist_fight_list.find(m_in_info.asssit_fight_guid) != m_assist_fight_list.end())
		{
			s_assist_fight_info &m_old_info = m_assist_fight_list[m_in_info.asssit_fight_guid];
			m_old_info.set_info(m_in_info);
			m_old_info.start_assist_time = time_helper::get_time();
		}
		else
		{
			s_assist_fight_info m_info;
			m_info.set_info(m_in_info);
			m_info.start_assist_time = time_helper::get_time();
			m_assist_fight_list.insert(std::pair<guid_64, s_assist_fight_info>(m_in_info.asssit_fight_guid, m_info));
			if (m_in_info.assist_fight_type == e_assist_fight_type_legion)
			{
				send_create_assist_fight_notic(m_in_info);
			}	
		}
		if (m_in_info.assist_fight_type == e_assist_fight_type_legion)
		{
			broadcast_to_legion(m_in_info.legion_guid, m_in_info.asssit_fight_guid);
		}
		else if(m_in_info.assist_fight_type == e_assist_fight_type_server)
		{
			broadcast_to_server();
		}			
	}
	void assist_fight_mgr::req_assist_fight(guid_64 role_guid, guid_64 assist_fight_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_req_assist_fight", 0, false, "%l%l", role_guid.server_64, assist_fight_guid.server_64);
			return;

		}
		client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);

		if (role_session == nullptr)
		{
			return;
		}
		if (role_guid == assist_fight_guid)
		{
			role_session->send_notice("90204183");
			return;
		}
		if (m_assist_fight_list.find(role_guid) != m_assist_fight_list.end())
		{
			role_session->send_notice("90204328");
			return;
		}
		if (m_assist_fight_list.find(assist_fight_guid) == m_assist_fight_list.end())
		{
			return;
		}

		s_assist_fight_info& m_info = m_assist_fight_list[assist_fight_guid];
		if (!m_info.asssit_fight_guid.is_valid())
		{
			return;
		}
		if (role_session->m_is_in_assist)
			{
				return;
			}

		auto map_template_ptr = cs_map_system::get_map_template(role_session->get_map_guid());
		if (nullptr == map_template_ptr)
		{
			CONSOLE_ERROR("map_template_ptr is nullptr map_guid:{}", role_session->get_map_guid().server_64);
			return;
		}
		if (map_template_ptr->Type != e_map_type_boss_home &&
			map_template_ptr->Type != e_map_type_boss_island &&
			map_template_ptr->Type != e_map_type_boss_vip_home &&
			map_template_ptr->Type != e_map_type_belief_cloister &&
			map_template_ptr->Type != e_map_type_field &&
			map_template_ptr->Type != e_map_type_big_map)
		{
			role_session->send_notice("90204165");
			return;
		}
		if (m_info.assist_fight_tag == 1)
		{
			client_session* main_session = client_session_mgr::getInstance().get_session(assist_fight_guid);
			if (nullptr == main_session)
			{
				return;
			}
			if (!main_session->get_team_guid().is_valid())
			{
				return;
			}
			team_ws * m_team = team_ws_mgr::get_instance().get_team(main_session->get_team_guid());
			if (m_team == nullptr)
			{
				return;
			}
			if (m_team->get_member_num() >= max_team_member_num)
			{
				team_proto_team_error team_error_msg;
				team_error_msg.set_error_type(e_team_error_team_member_is_full);
				role_session->send_to_client(&team_error_msg, e_msgindex_s2c_team_error);
				return;
			}
		}

		bool m_check_result = check_can_assist_fight(role_guid,m_info.assist_fight_tag, m_info.map_id, m_info.map_guid);
		if (!m_check_result)
		{
			return;
		}
		if (m_info.assist_fight_tag == 0)
		{
			if (!m_info.insert_assist_list(role_guid))
			{
				role_session->send_notice("90204164");
				return;
			}
		}
		sync_assist_fight_info_to_cs(role_guid, assist_fight_guid);

		if (m_info.assist_fight_tag == 0)
		{
			role_session->m_is_in_assist = true;
			role_session->assit_fight_guid = assist_fight_guid;
		}
	}
	void assist_fight_mgr::check_assist_fight_legion_team(guid_64 role_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_check_assist_fight_legion_team", 0, false, "%l", role_guid.server_64);
			return;
		}
		client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);
		if (role_session == nullptr)
		{
			return;
		}
		if (m_assist_fight_list.find(role_guid) == m_assist_fight_list.end())
		{
			return;
		}
		MapTemplate *map_temp_ptr =  role_session->get_map_temp_ptr();
		if (map_temp_ptr != nullptr)
		{
			if (nullptr != map_temp_ptr && map_temp_ptr->Order == e_map_order_type_multiplayer_raid)
			{
				cancel_assist_fight(0, role_guid, role_guid);
			}
		}
	}
	void assist_fight_mgr::on_player_logout(guid_64 role_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_on_player_logout", 0, false, "%l", role_guid.server_64);
			return;

		}
		client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);

		if (role_session == nullptr)
		{
			return;
		}
		if (m_assist_fight_list.find(role_guid) != m_assist_fight_list.end())
		{
			s_assist_fight_info &m_assist_info = m_assist_fight_list[role_guid];
			if (m_assist_info.assist_fight_tag == 1)
			{
				cancel_assist_fight(0, role_guid, role_guid);
				broadcast_to_legion(role_session->get_legion_guid());
			}
		}
		
	}
	void assist_fight_mgr::send_create_assist_fight_notic(const s_assist_fight_info & m_in_info)
	{
		client_session* session = client_session_mgr::getInstance().get_session(m_in_info.asssit_fight_guid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		NpcTemplate* _npc_template_ptr = GET_TEMPLATE(NpcTemplate, m_in_info.boss_id);
		if (nullptr == _npc_template_ptr)
		{
			return;
		}
		int32 notice_id = 93000428;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		std::vector<std::string> content_text_arr;
		std::vector<int32> param_arr;
		std::string content_text;
		content_text_arr.push_back(notice_str);
		content_text_arr.push_back(session->get_role_name());
		content_text_arr.push_back(init_unit::change_i64_to_string(_npc_template_ptr->NpcName));
		content_text = init_unit::implode(content_text_arr);

		event_ws_mgr::get_instance().send_notice_with_param(session->get_role_guid(), "", content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion);

	}
	void assist_fight_mgr::sync_assist_fight_info_to_cs_login(guid_64 role_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_sync_assist_fight_info_to_cs_login", 0, false, "%l", role_guid);
			return;

		}
		client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);

		if (role_session == nullptr)
		{
			return;
		}
		if (m_assist_fight_list.find(role_guid) != m_assist_fight_list.end())
		{
			return;
		}
		if (role_session->m_is_in_assist)
		{
			sync_assist_fight_info_to_cs(role_guid, role_session->assit_fight_guid);
		}
	}
	bool assist_fight_mgr::check_can_assist_fight(guid_64 role_guid, int32 assist_fight_tag, int32 map_template_id, guid_64 map_guid)
	{
		client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);

		if (role_session == nullptr)
		{
			return false; 
		}

		if (assist_fight_tag == 0)
		{
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
			if (map_template_ptr == nullptr)
			{
				return false;
			}
			if (role_session->get_role_info_data(e_role_info_exp_level) < map_template_ptr->EnterLevel)
			{
				return false;
			}

			int32 player_class = role_session->get_role_info_data(e_role_info_class_type);
			if (player_class < 1 || player_class > GAMECONFIG->InitRoleAry.size())
				return false;

			int32 player_init_template_id = GAMECONFIG->InitRoleAry[player_class - 1];
			int32 player_cur_template_id = role_session->get_role_info_data(e_role_info_template_id);
			if (player_cur_template_id - player_init_template_id < map_template_ptr->EnterGradeLevel)
				return false;

			if (map_template_ptr->Type == e_map_type_boss_home ||
				map_template_ptr->Type == e_map_type_boss_island ||
				map_template_ptr->Type == e_map_type_boss_vip_home ||
				map_template_ptr->Type == e_map_type_belief_cloister ||
				map_template_ptr->Type == e_map_type_field ||
				map_template_ptr->Type == e_map_type_big_map)
			{
				return true;
			}
			else
			{
				role_session->send_notice("90204165");
			}
		}
		return true;
	}
	void assist_fight_mgr::sync_assist_fight_info(guid_64 role_guid, bool is_login, guid_64 create_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_sync_assist_fight_info", 0, false, "%l%b%l", role_guid.server_64, is_login, create_guid.server_64);
			return;

		}
		client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == role_session)
		{
			return;
		}
		assist_fight_proto_sycn_assist_fight_info msg;
		msg.set_is_finish(false);
		std::map<guid_64, s_assist_fight_info>::iterator iterter = m_assist_fight_list.begin();
		for (;iterter != m_assist_fight_list.end();iterter++)
		{
			s_assist_fight_info m_info_data = iterter->second;
			if (m_info_data.assist_fight_type == e_assist_fight_type_legion)
			{
				if (role_session->get_legion_guid() != m_info_data.legion_guid)
				{
					continue;
				}
			}
			faith::assist_fight_proto_assist_fight_info *m_info_msg = msg.add_assist_fight_list();
			m_info_msg->set_assist_fight_guid_a(m_info_data.asssit_fight_guid.A);
			m_info_msg->set_assist_fight_guid_b(m_info_data.asssit_fight_guid.B);
			m_info_msg->set_assist_fight_type(m_info_data.assist_fight_type);
			m_info_msg->set_role_name(m_info_data.role_name);
			m_info_msg->set_role_lv(m_info_data.role_lv);
			m_info_msg->set_role_professional(m_info_data.role_professional);
			m_info_msg->set_head_icon(m_info_data.role_head_icon);
			m_info_msg->set_boss_id(m_info_data.boss_id);
			m_info_msg->set_map_id(m_info_data.map_id);
			m_info_msg->set_start_assist_time(m_info_data.start_assist_time);

			int32 pak_size = msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				role_session->send_to_client(&msg, e_msgindex_s2c_sycn_assist_fight_info);
				msg.clear_assist_fight_list();
			}
		}
		if (create_guid.is_valid() && create_guid == role_guid)
		{
			msg.set_is_sync(false);
		}
		else
		{
			msg.set_is_sync(true);
		}
		msg.set_is_finish(true);
		msg.set_is_login(is_login);
		role_session->send_to_client(&msg, e_msgindex_s2c_sycn_assist_fight_info);
		
	}
	void assist_fight_mgr::req_assist_fight_end(guid_64 assist_fight_guid, guid_64 role_guid,bool check_result)
	{
		//if (check_result == false)
		//{
		//	return;
		//}
		//if (m_assist_fight_list.find(assist_fight_guid) == m_assist_fight_list.end())
		//{
		//	return;
		//}
		//s_assist_fight_info &m_info = m_assist_fight_list[assist_fight_guid];
		//m_info.insert_assist_list(role_guid);
		//sync_assist_fight_info_to_cs(role_guid, assist_fight_guid);

		//if (m_info.boss_id > 0)
		//{
		//	NpcTemplate* _npc_template_ptr = GET_TEMPLATE(NpcTemplate, m_info.boss_id);
		//	if (nullptr == _npc_template_ptr)
		//	{
		//		return;
		//	}
		//	std::string notice_str_id = "90204169";
		//	std::vector<std::string> notice_str_params_vec;
		//	notice_str_params_vec.push_back(notice_str_id);
		//	notice_str_params_vec.push_back(role_session->get_role_name());
		//	const std::string& boss_name = template_manager::get_instance().get_str_by_string_template_id(_npc_template_ptr->NpcName);
		//	notice_str_params_vec.push_back(boss_name);
		//	std::string notice_str = init_unit::implode(notice_str_params_vec);
		//	main_session->send_notice(notice_str);
		//	notice_str_params_vec.clear();
		//	notice_str_id = "90204170";
		//	notice_str_params_vec.push_back(notice_str_id);
		//	notice_str_params_vec.push_back(main_session->get_role_name());
		//	notice_str_params_vec.push_back(boss_name);
		//	notice_str = init_unit::implode(notice_str_params_vec);
		//	role_session->send_notice(notice_str);
		//}
		//role_session->send_notice("90204170");
	}
	void assist_fight_mgr::cancel_assist_fight(int32 cancel_type,guid_64 assist_fight_guid,guid_64 role_guid)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "assist_fight_mgr_cancel_assist_fight", 0, false, "%d%l%l", cancel_type, assist_fight_guid.server_64, role_guid.server_64);
			return;

		}
		
		guid_64 normal_fight_guid;
		if (cancel_type == 0)
		{
			if (m_assist_fight_list.find(role_guid) == m_assist_fight_list.end())
			{
				return;
			}
			s_assist_fight_info &m_info = m_assist_fight_list[assist_fight_guid];
			for (int32 i = 0 ; i < assist_fight_max_num;i++)
			{	
				guid_64 &helper_guid = m_info.assist_guid_list[i];
				if (helper_guid.is_valid())
				{
					helper_guid.clear_data();
				}
			}
			m_info.clear_data();
			m_assist_fight_list.erase(role_guid);
		}
		else if (cancel_type == 1)
		{
			client_session* role_session = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr != role_session)
			{
				role_session->m_is_in_assist = false;
				role_session->assit_fight_guid.clear_data();
			}
			if (m_assist_fight_list.find(assist_fight_guid) == m_assist_fight_list.end())
			{
				return;
			}
			s_assist_fight_info &m_info = m_assist_fight_list[assist_fight_guid];
			m_info.remove_assist_list(role_guid);
		}
		sync_assist_fight_info(role_guid);
	}
	void assist_fight_mgr::sync_assist_fight_info_to_cs(guid_64 role_guid,guid_64 assist_fight_guid)
	{
		client_session* helper_session = client_session_mgr::getInstance().get_session(role_guid);
		if (helper_session != nullptr)
		{
			if (m_assist_fight_list.find(assist_fight_guid) == m_assist_fight_list.end())
			{
				return;
			}
			team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(helper_session->m_team_guid);
			if (nullptr != team_ws_ptr)
			{
				team_ws_ptr->del_member(role_guid);
			}
			ws2cs_sync_assist_fight_info cs_msg;
			cs_msg.role_guid = role_guid;
			cs_msg.m_assist_fight_info = m_assist_fight_list[assist_fight_guid];
			bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_assist_fight_info);
			if (is_use == false)
			{
				helper_session->send_to_cs(&cs_msg, sizeof(ws2cs_sync_assist_fight_info));
			}
			else
			{
				faith::ws2cs_proto::sync_assist_fight_info pro_msg;
				cs_msg.to_proto(pro_msg);
				helper_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sync_assist_fight_info);
			}
		}
	}
	void assist_fight_mgr::broadcast_to_legion(guid_64 legion_guid, guid_64 create_guid)
	{
		legion_ws * m_legion = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (m_legion == nullptr)
		{
			return;
		}
		for (s_legion_member_info& member_info : m_legion->get_member_list())
		{
			if (member_info.is_valid() == false || member_info.data_ary[e_legion_member_info_is_online] <= 0)
			{
				continue;
			}

			guid_64 member_guid = member_info.role_guid;
			sync_assist_fight_info(member_guid, false, create_guid);
		}
	}
	void assist_fight_mgr::broadcast_to_server()
	{
		assist_fight_proto_sycn_assist_fight_info msg;
		std::map<guid_64, s_assist_fight_info>::iterator iterter = m_assist_fight_list.begin();
		for (; iterter != m_assist_fight_list.end(); iterter++)
		{
			s_assist_fight_info m_info_data = iterter->second;
			faith::assist_fight_proto_assist_fight_info* m_info_msg = msg.add_assist_fight_list();
			m_info_msg->set_assist_fight_guid_a(m_info_data.asssit_fight_guid.A);
			m_info_msg->set_assist_fight_guid_b(m_info_data.asssit_fight_guid.B);
			m_info_msg->set_assist_fight_type(m_info_data.assist_fight_type);
			m_info_msg->set_role_name(m_info_data.role_name);
			m_info_msg->set_role_lv(m_info_data.role_lv);
			m_info_msg->set_role_professional(m_info_data.role_professional);
			m_info_msg->set_boss_id(m_info_data.boss_id);
			m_info_msg->set_map_id(m_info_data.map_id);
		}
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_sycn_assist_fight_info);
	}
	bool assist_fight_mgr::is_use_lua()
	{
		bool bRet = false;
		script_mgr::get_instance().call_func(nullptr, "is_assist_use_lua", 1, false, ">%b", &bRet);
		return bRet;
	}
}