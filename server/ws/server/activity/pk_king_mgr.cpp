#include "../big_player_ws_mgr.h"
#include "../ranking/ranking_mgr_ws.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "cross_server_pk_msg.hpp"
#include "game.pb.h"
#include "net.pb.h"
#include "pk_king_mgr.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "world_server.hpp"

namespace faith
{
	pk_king_mgr::pk_king_mgr()
	{
		clear_data();
	}

	void pk_king_mgr::clear_data()
	{
		m_pk_king_player.clear_data();
		m_pk_king_map_guid.clear_data();
		m_timer = 0;
		m_last_tick_time = 0;
		m_is_clear_rank = true;
		m_break_line_login_list.clear();
	}

	void pk_king_mgr::clear_rank_list_data()
	{
		ranking_mgr_ws::clear_data_by_ranking_type(e_RankingIndex_pk_king);
		big_player_ws_mgr::get_instance().clear_title_with_big_player_clear(e_big_player_type_lord_or_war);
		big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(false);
	}

	void pk_king_mgr::add_rank_list_data(guid_64 role_guid)
	{
		s_ranking_player_info list_info;
		list_info.role_guid = role_guid;
		list_info.ranking_value = 0;
		list_info.save_time = time_helper::get_cur_time_new().second;
		list_info.server_id = world_server::getInstance().get_server_id();
		list_info.ranking_type = e_RankingIndex_pk_king;
		ranking_mgr_ws::sync_player_info(list_info);
	}

	void pk_king_mgr::tick(int64 time_new)
	{
		m_timer += time_new;
		if (m_timer - m_last_tick_time > 1000)
		{
			m_last_tick_time = m_timer;
			if (true == is_in_game_time())
			{
				check_break_line_player();
				if (false == m_pk_king_map_guid.is_valid())
				{

					if (m_is_clear_rank)
					{
						clear_rank_list_data();
						m_is_clear_rank = false;
					}
					if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity) && false == world_server::getInstance().is_sky_island_server())
					{
						return;
					}
					int32 war_idx = 0;
					init_pkking_map();
				}
			}	
			int64 time_now = utility::get_tick_count();
			if (false == is_in_game_time() && true == m_pk_king_map_guid.is_valid())
			{
				m_is_clear_rank = true;
				m_break_line_login_list.clear();
				m_pk_king_map_guid.clear_data();
			}
		}
	}

	e_error_code pk_king_mgr::is_can_join(guid_64 role_guid)
	{
		ZoneScoped;
		if (is_in_notice_time())
		{
			return e_error_code_map_pk_king_ready_not_join_time;
		}
		if (is_in_game_time() == false)
		{
			return e_error_code_map_pk_king_not_begin;
		}
		if (is_in_ready_time() == false)
		{
			return e_error_code_map_pk_king_ready_end;
		}
		
		if (cs_map_system::is_full(m_pk_king_map_guid))
		{
			return e_error_code_map_max_players;
		}
		
		// 新增玩家进入地图cd
		//if (false == cs_map_mgr_system::check_unit_can_enter_map(pk_king_map_template_id, role_guid))
		//{
		//	return e_error_code_map_enter_map_cd;
		//}
		return e_error_code_success;
	}

	bool pk_king_mgr::is_in_game_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_pk_king, e_activity_time_get_all, false, false);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	bool pk_king_mgr::is_in_ready_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_pk_king, e_activity_time_get_ready);
		//pk之王剩10秒就不允许进入了 防止进入地图中活动开启
		if (act_left < 10)
		{
			return false;
		}
		return true;
	}

	bool pk_king_mgr::is_in_notice_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_pk_king, e_activity_time_get_notice);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	void pk_king_mgr::set_pk_king_player(guid_64 role_guid)
	{
		m_pk_king_player = role_guid;
	}

	void pk_king_mgr::init_pkking_map()
	{
		m_pk_king_map_guid.clear_data();
		auto map_ent = cs_map_mgr_system::init_map_by_battle(pk_king_map_template_id);
		if (map_ent)
		{
			m_pk_king_map_guid = map_ent->getEntityId();
		}
	}

	void pk_king_mgr::get_valid_pkking_map(int32& map_war_idx, guid_64& map_guid)
	{
		map_guid.clear_data();
		auto map_ent = get_entity(m_pk_king_map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("pk_king_mgr::get_valid_pkking_map map_ent is nullptr m_pk_king_map_guid:{}", m_pk_king_map_guid.server_64);
			return;
		}

		map_war_idx = cs_map_system::get_unit_count(map_ent) % 4 + 1;
		map_guid = m_pk_king_map_guid;
	}

	void pk_king_mgr::send_player_num_in_map_to_client(guid_64 role_guid)
	{
		if (is_in_ready_time() == false)
		{
			return;
		}

		int32 player_num = 0;
		int64 map_info[map_info_arr_max_num];
		auto map_ent = get_entity(m_pk_king_map_guid);
		if (nullptr == map_ent)
		{
			map_info[e_map_info_player_num] = player_num;
			cs_map_mgr_system::send_one_map_info_to_client(role_guid, pk_king_map_template_id, map_info);
			return;
		}

		player_num = cs_map_system::get_unit_count(map_ent);
		map_info[e_map_info_player_num] = player_num;
		cs_map_mgr_system::send_one_map_info_to_client(role_guid, pk_king_map_template_id, map_info);
	}

	void pk_king_mgr::recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id)
	{
		e_error_code ret = is_can_join(role_guid);
		int32 error_ret = ret;
		int32 war_idex = 0;
		int32 map_temp_id = pk_king_map_template_id;
		guid_64 map_guid = guid_64();
		if (ret == e_error_code_success)
		{
			int32 war_idx = 0;
			get_valid_pkking_map(war_idx, map_guid);
		}
		if (false == map_guid.is_valid())
		{
			ret = e_error_code_map_init_map_err;
		}
		ws2ws_transfer_cross_pk_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idex;
		msg.activity_type = (int32)e_activity_type_pk_king;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_map_island_state_msg, &msg, sizeof(msg));
	}

	void pk_king_mgr::add_break_line_player(s_break_line_login_info break_info)
	{
		bool is_change = true;
		for (int32 i = 0; i < m_break_line_login_list.size(); ++i)
		{
			if (m_break_line_login_list[i].role_guid == break_info.role_guid)
			{
				m_break_line_login_list[i] = break_info;
				is_change = false;
				break;
			}
		}
		if (is_change)
		{
			m_break_line_login_list.push_back(break_info);
		}
	}

	void pk_king_mgr::check_break_line_player()
	{
		int64 game_time = utility::get_tick_count();
		vector<s_break_line_login_info>::iterator ite = m_break_line_login_list.begin();
		for (; ite != m_break_line_login_list.end(); ite++)
		{
			s_break_line_login_info& other_info = *ite;
			if (game_time >= other_info.expiry_time)
			{
				//不要删除防止接近时间的时候重新连接导致获取不到数据
				other_info.is_send_msg = true;
				continue;
			}
			if (other_info.is_send_msg)
			{
				continue;
			}
			client_session* session_ptr = client_session_mgr::getInstance().get_session(other_info.role_guid);
			if (nullptr == session_ptr || session_ptr->m_online_state != client_session::e_os_online || session_ptr->m_status != client_session::e_ss_ingame)
			{
				continue;
			}
			other_info.is_send_msg = true;
			send_break_line_msg(other_info.role_guid, other_info.map_template_id);
		}
	}

	void pk_king_mgr::send_break_line_msg(guid_64 role_guid, int32 map_template_id)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		game_proto_send_need_break_line_login msg;
		msg.set_map_template_id(map_template_id);
		msg.set_active_type(e_activity_type_pk_king);
		session_ptr->send_to_client(&msg, e_mgsindex_s2c_send_need_break_line_login);
	}

	void pk_king_mgr::break_line_login_msg(guid_64 role_guid, bool is_login)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		if (is_login)
		{
			s_break_line_login_info role_login_info = get_player_break_login_info(role_guid);
			if (role_login_info.role_guid.is_valid() == false)
			{
				//发送活动已结束
				session->send_notice("90201845");
				return;
			}
			send_to_break_login_transfer_map(session->get_role_guid(), role_login_info.map_guid, e_activity_type_pk_king);
		}
	}

	void pk_king_mgr::send_to_break_login_transfer_map(guid_64 role_guid, guid_64 map_guid, int32 activity_type, int32 server_id)
	{
		ws2ws_break_login_transfer_map msg;
		msg.role_guid = role_guid;
		msg.server_id = world_server::getInstance().get_server_id();
		msg.activity_type = activity_type;
		msg.map_guid = map_guid;
		if (server_id <= 0)
		{
			server_id = world_server::getInstance().get_cross_id();
		}
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_break_login_transfer_map, &msg, sizeof(msg));
	}

	void pk_king_mgr::check_can_break_login_map(guid_64 role_guid, guid_64 map_guid, int32 server_id)
	{
		e_error_code ret = check_login_map_can_join(role_guid, map_guid);
		int32 error_ret = ret;
		int32 war_idex = 0;
		if (false == map_guid.is_valid())
		{
			ret = e_error_code_map_init_map_err;
		}
		ws2ws_break_login_transfer_map_result msg;
		msg.map_template_id = cs_map_system::get_map_template_id(map_guid);
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.war_idex = war_idex;
		msg.activity_type = (int32)e_activity_type_pk_king;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_break_login_transfer_map_end, &msg, sizeof(msg));
	}

	e_error_code pk_king_mgr::check_login_map_can_join(guid_64 role_guid, guid_64 map_guid)
	{
		if (is_in_game_time() == false)
		{
			return e_error_code_map_pk_king_not_begin;
		}

		if (cs_map_system::is_full(map_guid))
		{
			return e_error_code_map_max_players;
		}
		return e_error_code_success;
	}

	void pk_king_mgr::transfer_break_login_map(guid_64 role_guid, guid_64 map_guid, int32 error_ret, int32 map_template_id, int32 war_idex, int32 server_id)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		e_error_code ret = (e_error_code)error_ret;
		std::string notice_id = "";
		switch (ret)
		{
		case faith::e_error_code_success:
			break;
		case faith::e_error_code_map_init_map_err:
			notice_id = "90090579";
			break;
		case faith::e_error_code_map_max_players:
			break;
		case faith::e_error_code_map_enter_map_cd:
			break;
		case faith::e_error_code_map_pk_king_not_begin:
			break;
		case faith::e_error_code_map_pk_king_no_eligibility:
			break;
		case faith::e_error_code_map_broken_sky_not_begin:
			break;
		case faith::e_error_code_map_broken_sky_game_over:
			break;
		case faith::e_error_code_map_cross_server_pk_not_begin:
			notice_id = "90090360";
			break;
		case faith::e_error_code_map_cross_server_pk_session_error:
			break;
		case faith::e_error_code_map_cross_server_pk_end:
			notice_id = "90201845";
			break;
		case faith::e_error_code_map_cross_server_pk_not_start:
			notice_id = "90305015";
			break;
		case faith::e_error_code_map_cross_city_war_not_begin:
			notice_id = "90201806";
			break;
		case faith::e_error_code_map_cross_city_war_map_error:
			notice_id = "90209394";
			break;
		case faith::e_error_code_map_cross_city_war_map_not_find:
			notice_id = "90203651";
			break;
		case faith::e_error_code_map_cross_city_war_map_end:
			notice_id = "90203604";
			break;
		case faith::e_error_code_map_cross_territory_war_not_begin:
			notice_id = "90203650";
			break;
		case faith::e_error_code_map_no_territory_war_qualification:
			notice_id = "90203651";
			break;
		default:
			break;
		}
		if (ret != e_error_code_success)
		{
			if (notice_id != "")
			{
				session_ptr->send_notice(notice_id);
				return;
			}
			cs_map_mgr_system::send_map_error_to_client(session_ptr, ret);
			return;
		}
		else if (map_template_id > 0)
		{
			if (server_id <= 0)
			{
				server_id = world_server::getInstance().get_cross_id();
			}
			cs_map_mgr_system::transfer_to_map(session_ptr, map_template_id, map_guid, war_idex, server_id);
		}
	}

	bool pk_king_mgr::player_is_pk_king(guid_64 role_guid)
	{
		s_ranking_player_info* player_ranking = ranking_mgr_ws::get_ranking_player_info_by_type(e_RankingIndex_pk_king, role_guid);
		if (player_ranking == nullptr)
		{
			return false;
		}
		return true;
	}

	s_break_line_login_info pk_king_mgr::get_player_break_login_info(guid_64 role_guid)
	{
		s_break_line_login_info login_info;
		for (int32 i = 0; i < m_break_line_login_list.size(); ++i)
		{
			if (m_break_line_login_list[i].role_guid == role_guid)
			{
				login_info = m_break_line_login_list[i];
				break;
			}
		}
		return login_info;
	}

	void pk_king_mgr::create_pk_king_combat_map(guid_64 from_map_guid, int32 map_num)
	{
		ws2cs_create_pk_king_combat_map_end msg;
		msg.map_guid = m_pk_king_map_guid;
		msg.map_num = 0;
		for (int32 i = 0; i < map_num; ++i)
		{
			auto map_ent = cs_map_mgr_system::init_map_by_battle(pk_king_combat_map_template_id);
			if (map_ent)
			{
				guid_64 map_guid = map_ent->getEntityId();
				if (i < pk_king_max_map_num)
				{
					msg.map_array[i] = map_guid;
					msg.map_num++;
				}	
			}
		}
		bool is_use = proto_by_lua(e_msgindex_ws2cs_create_pk_king_combat_map_end);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::create_pk_king_combat_map_end pro_msg;
			msg.to_proto(pro_msg);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_create_pk_king_combat_map_end, e_server_type_cs);
		}
	}
}
