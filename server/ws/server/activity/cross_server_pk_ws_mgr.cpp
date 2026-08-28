/********************************************************************
  created: 2019年1月15日
  file base: cross_server_pk_ws_mgr
  file ext: cpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#include "../big_player_ws_mgr.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "cross_server_harry_ws_mgr.h"
#include "cross_server_pk_msg.hpp"
#include "cross_server_pk_ws_mgr.h"
#include "game.pb.h"
#include "Logic/cross_server_harry_def.hpp"
#include "Logic/time_def.hpp"
#include "net.pb.h"
#include "server_log.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "world_server.hpp"
#include "ws_client.hpp"
namespace hld
{
	cross_server_pk_ws_mgr::cross_server_pk_ws_mgr()
	{
		clear_data();
	}

	void cross_server_pk_ws_mgr::clear_data()
	{
		for (int32 i = 0; i < max_branch_type; i++)
		{
			m_cross_pk_map_guid[i].clear_data();
			m_cross_pk_map_id_arr[i] = (i + (int32)e_pk_map_id_barserker);
			m_is_cur_map_end[i] = false;
			m_pk_player_msg[i].clear_data();
			m_is_load_all_fake_player[i] = false;
		}
		m_island_all_msg.clear_data();
		m_last_pk_king_stamp = 0;
		m_timer = 0;
		m_last_tick_time = 0;
		m_is_clear_db = false;
		m_last_3sec_tick_time = 0;
		m_empty_player_msg.clear_data();
		m_last_load_fake_player_time = 0;
	}

	void cross_server_pk_ws_mgr::init_manager()
	{
		req_load_island_msg();
		m_can_craete_fake_player = true;
	}

	void cross_server_pk_ws_mgr::tick_3sec(int64 time_new)
	{

	}

	void cross_server_pk_ws_mgr::tick(int64 time_new)
	{
		m_timer += time_new;
		if (m_timer - m_last_tick_time > second_tick_time)
		{
			m_last_tick_time = m_timer;
			if (true == world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_pk))
			{
				if (true == is_in_game_time() && false == is_have_pk_map())
				{
					init_cross_server_pk_map();
					if (false == m_is_clear_db)//进入到游戏时间则清理一次库
					{
						clear_all_big_player_and_title();
						m_is_clear_db = true;
						clear_all_island_msg();
						send_clear_all_island_msg_to_client();
					}
				}
				if (false == is_in_game_time())
				{
					if (true == m_is_clear_db)//离开游戏时间则存一次库
					{
						for (int32 i = 0; i < max_branch_type; i++)
						{
							m_is_cur_map_end[i] = false;
							m_cross_pk_map_guid[i].clear_data();
						}
						m_is_clear_db = false;
						ws2dp_save_island_msg();
						set_winner_fake_player_gate_msg();
						send_winner_fake_player_to_other_server();
						//send_island_msg_all_to_client();
					}
				}
			}
			if (m_can_craete_fake_player && m_recv_gate_msg && m_need_create_fake_player)
			{
				m_recv_gate_msg = false;
				cross_server_harry_ws_mgr::get_instance().load_fake_data(e_activity_type_cross_server_pk);
				m_last_load_fake_player_time = time_helper::get_cur_time_new().second + load_fake_player_time;
			}
			if (m_last_load_fake_player_time > 0)
			{
				if (time_helper::get_cur_time_new().second >= m_last_load_fake_player_time)
				{
					if (get_is_load_all_fake_player())
					{
						m_last_load_fake_player_time = 0;
					}
					else
					{
						m_last_load_fake_player_time = time_helper::get_cur_time_new().second + load_fake_player_time;
						load_fake_player_with_last_time();
					}
				}
			}
		}
		if (m_timer - m_last_3sec_tick_time > second_tick_time * 3)
		{
			m_last_3sec_tick_time = m_timer;
			tick_3sec(time_new);
		}
	}

	void cross_server_pk_ws_mgr::send_to_gate_transfer_map(int32 class_type, guid_64 role_guid, int32 activity_type, int32 server_id, guid_64 legion_guid)
	{
		ws2ws_transfer_cross_pk_map msg;
		msg.class_type = class_type;
		msg.role_guid = role_guid;
		msg.server_id = world_server::getInstance().get_server_id();
		msg.activity_type = activity_type;
		msg.legion_guid = legion_guid;
		if (server_id <= 0)
		{
			server_id = world_server::getInstance().get_cross_id();
		}
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_state_end, &msg, sizeof(msg));
	}

	void cross_server_pk_ws_mgr::recv_other_ws_transger_gate_map(int32 class_type, guid_64 role_guid, int32 server_id)
	{
		auto ret = is_can_join_with_cross_gate(class_type);
		int32 error_ret = ret;
		int32 war_idex = 0;
		int32 map_temp_id = get_map_temp_id_with_class_type(class_type);
		guid_64 map_guid = guid_64();
		get_pk_king_map_guid(class_type, map_guid, war_idex);		
		ws2ws_transfer_cross_pk_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idex;
		msg.activity_type = (int32)e_activity_type_cross_server_pk;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_map_island_state_msg, &msg, sizeof(msg));
	}

	void cross_server_pk_ws_mgr::transfer_to_map_with_gate_msg(guid_64 role_guid, int32 error_ret, int32 map_temp_id, guid_64 map_guid, int32 war_idex, int32 server_id)
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
		case hld::e_error_code_success:
			break;
		case hld::e_error_code_map_init_map_err:
			notice_id = "90090579";
			break;
		case hld::e_error_code_map_max_players:
			break;
		case hld::e_error_code_map_enter_map_cd:
			break;
		case hld::e_error_code_map_pk_king_not_begin:
			break;
		case hld::e_error_code_map_pk_king_no_eligibility:
			break;
		case hld::e_error_code_map_broken_sky_not_begin:
			break;
		case hld::e_error_code_map_broken_sky_game_over:
			break;
		case hld::e_error_code_map_cross_server_pk_not_begin:
			notice_id = "90090360";
			break;
		case hld::e_error_code_map_cross_server_pk_session_error:
			break;
		case hld::e_error_code_map_cross_server_pk_end:
			notice_id = "90201845";
			break;
		case hld::e_error_code_map_cross_server_pk_not_start:
			notice_id = "90305015";
			break;
		case hld::e_error_code_map_cross_city_war_not_begin:
			notice_id = "90201806";
			break;
		case hld::e_error_code_map_cross_city_war_map_error:
			notice_id = "90209394";
			break;
		case hld::e_error_code_map_cross_city_war_map_not_find:
			notice_id = "90203651";
			break;
		case hld::e_error_code_map_cross_city_war_map_end:
			notice_id = "90203604";
			break;
		case hld::e_error_code_map_cross_territory_war_not_begin:
			notice_id = "90203650";
			break;
		case hld::e_error_code_map_no_territory_war_qualification:
			notice_id = "90203651";
			break;
		case hld::e_error_code_map_pk_king_ready_not_join_time:
			notice_id = "90305000";
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
		else if (map_temp_id > 0)
		{
			if (server_id <= 0)
			{
				server_id = world_server::getInstance().get_cross_id();
			}
			cs_map_mgr_system::transfer_to_map(session_ptr, map_temp_id, map_guid, war_idex, server_id);
		}
	}

	e_error_code cross_server_pk_ws_mgr::is_can_join_with_cross_gate(int32 class_type)
	{
		if (is_in_game_time() == false)
		{
			return e_error_code_map_cross_server_pk_not_begin;
		}

		if (class_type < e_class_type_barserker || class_type > e_class_type_assassinator)
		{
			return e_error_code_map_cross_server_pk_end;
		}

		int32 war_idex = 0;
		guid_64 map_guid;
		get_pk_king_map_guid(class_type, map_guid, war_idex);

		if (cs_map_system::is_full(map_guid))
		{
			return e_error_code_map_max_players;
		}

		if (true == m_is_cur_map_end[class_type - 1])
		{
			return e_error_code_map_cross_server_pk_end;
		}
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_pk))
		{
			return e_error_code_map_cross_server_pk_not_begin;
		}
		return e_error_code_success;
	}

	e_error_code cross_server_pk_ws_mgr::is_can_join_with_cross(guid_64 role_guid)
	{
		client_session* cur_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == cur_session_ptr)
		{
			return e_error_code_map_cross_server_pk_session_error;
		}
		if (is_in_game_time() == false)
		{
			cur_session_ptr->send_notice("90090360");
			return e_error_code_map_cross_server_pk_not_begin;
		}
		int32 class_type = cur_session_ptr->get_role_info_data(e_role_info_class_type);
		if (class_type<e_class_type_barserker || class_type>e_class_type_assassinator)
		{
			return e_error_code_map_cross_server_pk_end;
		}
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_pk))
		{
			cur_session_ptr->send_notice("90305015");
			return e_error_code_map_cross_server_pk_not_start;
		}
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, (int32)e_pk_map_id_barserker);
		if (map_ptr == nullptr || map_ptr->EnterLevel > cur_session_ptr->get_role_info_data(e_role_info_exp_level))
		{
			cur_session_ptr->send_notice("90090228");
			return e_error_code_map_max_players;
		}
		return e_error_code_success;
	}

	e_error_code cross_server_pk_ws_mgr::is_can_join(guid_64 role_guid)
	{
		client_session* cur_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == cur_session_ptr)
		{
			return e_error_code_map_cross_server_pk_session_error;
		}
		if (is_in_game_time() == false)
		{
			cur_session_ptr->send_notice("90090360");
			return e_error_code_map_cross_server_pk_not_begin;
		}

		int32 class_type = cur_session_ptr->get_role_info_data(e_role_info_class_type);
		if (class_type<e_class_type_barserker || class_type>e_class_type_assassinator)
		{
			return e_error_code_map_cross_server_pk_end;
		}

		int32 war_idex = 0;
		guid_64 map_guid;
		get_pk_king_map_guid(class_type, map_guid, war_idex);

		if (cs_map_system::is_full(map_guid))
		{
			cur_session_ptr->send_notice("90202267");
			return e_error_code_map_max_players;
		}
		
		if (true == m_is_cur_map_end[class_type - 1])
		{
			cur_session_ptr->send_notice("90201845");
			return e_error_code_map_cross_server_pk_end;
		}
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_pk))
		{
			cur_session_ptr->send_notice("90305015");
			return e_error_code_map_cross_server_pk_not_start;
		}
		auto map_ptr =cs_map_system::get_map_template(map_guid);
		if (map_ptr == nullptr || map_ptr->EnterLevel > cur_session_ptr->get_role_info_data(e_role_info_exp_level))
		{
			cur_session_ptr->send_notice("90090228");
			return e_error_code_map_max_players;
		}
		return e_error_code_success;
	}

	bool cross_server_pk_ws_mgr::is_have_pk_map()
	{
		bool is_have = true;
		for (int32 i = 0; i < max_branch_type; i++)
		{
			if (false == m_cross_pk_map_guid[i].is_valid())
			{
				is_have = false;
			}
		}
		return is_have;
	}

	bool cross_server_pk_ws_mgr::is_in_game_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_pk, e_activity_time_get_all);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	void cross_server_pk_ws_mgr::get_pk_king_map_guid(int32 class_type,guid_64& map_guid, int32& war_idex)
	{
		map_guid = guid_64();
		if (class_type < e_class_type_barserker || class_type > e_class_type_assassinator)
		{
			return;
		}

		if (m_cross_pk_map_guid[class_type - 1].is_valid())
		{
			auto map_ent = get_entity(m_cross_pk_map_guid[class_type - 1]);
			if (map_ent)
			{
				map_guid = map_ent->getEntityId();
				war_idex = cs_map_system::get_unit_count(map_ent) % 4 + 1;
			}
		}
	}

	void cross_server_pk_ws_mgr::init_cross_server_pk_map()
	{
		for (int32 i = 0; i < max_branch_type; i++)
		{
			auto map_ent = cs_map_mgr_system::init_map_by_battle(m_cross_pk_map_id_arr[i]);
			if (map_ent)
			{
				m_cross_pk_map_guid[i] = map_ent->getEntityId();
				m_last_pk_king_stamp = time_helper::get_cur_time_new().second;
				server_log::elite_boss_battle_from_boss_log(m_cross_pk_map_guid[i].A, m_cross_pk_map_guid[i].B, i, m_cross_pk_map_id_arr[i], time_helper::get_cur_time_new().second);
			}
		}
	}

	void cross_server_pk_ws_mgr::save_cs2ws_island_msg(const s_pk_island_all_msg& island_all_msg, int32 war_count, guid_64 winner_guid, int32 is_end_war, bool is_need_send_to_client, int32 pk_count)
	{
		if ( war_count >= max_cross_server_war_count || war_count <= 0)
		{
			return;
		}

		for (int32 i = 0; i < war_count; i++)
		{
			s_pk_island_simple_msg simple_msg = island_all_msg.island_msg[i];
			int32 war_idex = simple_msg.war_idex;
			if (war_idex < 0 || war_idex >= max_cross_server_all_class_war_count)
			{
				continue;
			}
			m_island_all_msg.island_msg[war_idex] = simple_msg;
			if (true == is_need_send_to_client)
			{
				send_island_msg_one_to_client(simple_msg, winner_guid, is_end_war, pk_count);
			}
		}
	}

	void cross_server_pk_ws_mgr::save_dp2ws_island_msg(const s_pk_island_simple_msg& island_simple_msg)
	{
		int32 war_idex = island_simple_msg.war_idex;

		if (war_idex < 0 || war_idex >= max_cross_server_all_class_war_count)
		{
			return;
		}
		m_island_all_msg.island_msg[war_idex] = island_simple_msg;
	}

	void cross_server_pk_ws_mgr::ws2dp_save_island_msg()
	{
		s_pk_island_simple_msg island_simple_msg;
		for (int32 i = 0; i < max_cross_server_all_class_war_count; i++)
		{
			if (false == m_island_all_msg.island_msg[i].is_vaild())
			{
				continue;
			}
			island_simple_msg.clear_data();
			island_simple_msg = m_island_all_msg.island_msg[i];
			ws2dp_save_simple_island_msg(island_simple_msg);
		}
	}

	void cross_server_pk_ws_mgr::ws2dp_save_simple_island_msg(const s_pk_island_simple_msg& island_msg)
	{
		server2dp_proto_ws2dp_save_cross_server_pk_msg_all save_msg;
		save_msg.set_pk_state(island_msg.pk_state);
		save_msg.set_war_idex(island_msg.war_idex);
		for (int32 i = 0; i < max_island_player_num; ++i)
		{
			save_msg.add_player_guid(island_msg.island_player_msg[i].player_guid.server_64);
			save_msg.add_role_name(island_msg.island_player_msg[i].role_name);
			save_msg.add_is_winner(island_msg.island_player_msg[i].is_winner);
			save_msg.add_server_id(island_msg.island_player_msg[i].server_id);
			save_msg.add_win_num(island_msg.island_player_msg[i].win_num);
		}
		ws_client::getInstance().send_to_dp_new(&save_msg, e_msg_ws2dp_save_cross_server_pk_info);
		/*ws2dp_save_cross_server_pk_msg_all save_msg;
		save_msg.island_simple_msg = island_msg;
		ws_client::getInstance().send_to_dp(&save_msg, sizeof(save_msg));*/
	}

	void cross_server_pk_ws_mgr::clear_all_island_msg()
	{
		m_island_all_msg.clear_data();
		ws2dp_clear_cross_server_pk_msg_all clear_msg;
		ws_client::getInstance().send_to_dp(&clear_msg, sizeof(clear_msg));
	}

	void cross_server_pk_ws_mgr::clear_all_big_player_and_title()
	{
		for (int32 i = e_big_player_type_best_barserker; i <= e_big_player_type_best_assassinator; i++)
		{
			big_player_ws_mgr::get_instance().set_big_player_guid((e_big_player_type)i, 0);
			if (init_unit::is_send_clear_title_with_clear_big_player(i))
			{
				big_player_ws_mgr::get_instance().clear_title_with_big_player_clear(i);
			}
		}
		big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(false);

	}

	void cross_server_pk_ws_mgr::send_clear_all_island_msg_to_client()
	{
		game_proto_cross_server_pk_map_transfer msg;
		client_session_mgr::getInstance().send_message_to_all_server_client(&msg, e_msgindex_s2c_clear_pk_msg_all, e_msgindex_ws2ws_clear_all_pk_msg);
	}

	void cross_server_pk_ws_mgr::send_all_msg_to_client(const void* data_ptr, size_t data_len, client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		session->send_to_client(data_ptr, data_len, e_msgindex_s2c_cross_server_pk_msg_info_all);
	}

	void cross_server_pk_ws_mgr::send_pk_to_all_player_ws(const void* data_ptr, size_t data_len)
	{
		client_session_mgr::getInstance().send_message_to_all_client_data(data_ptr, data_len, e_msgindex_s2c_cross_server_pk_msg_info_one);
	}

	void cross_server_pk_ws_mgr::send_ws2ws_all_pk_msg(int32 server_id, guid_64 role_guid)
	{
		game_proto_cross_server_all_msg all_msg;
		for (int32 i = 0; i < max_cross_server_all_class_war_count; i++)
		{
			if (!m_island_all_msg.island_msg[i].is_vaild())
			{
				continue;
			}
			game_proto_cross_server_one_island_msg* island_msg = all_msg.add_island_msg();
			fill_one_island_msg(*island_msg, m_island_all_msg.island_msg[i]);
			if (all_msg.ByteSize() > packege_length_max)
			{
				cross::send_msg_to_ws(role_guid, server_id, e_msgindex_s2c_cross_server_pk_msg_info_one, &all_msg);
				all_msg.clear_island_msg();
			}

		}
		cross::send_msg_to_ws(role_guid, server_id, e_msgindex_s2c_cross_server_pk_msg_info_one, &all_msg);
	}

	void cross_server_pk_ws_mgr::send_island_msg_all_to_client(client_session* session_ptr)
	{
		if (nullptr == session_ptr)
		{
			return;
		}
		if (false == gate_proxy::getInstance().is_gate_run()
			|| (true == gate_proxy::getInstance().is_gate_run() && world_server::getInstance().is_sky_island_server()))
		{
			game_proto_cross_server_all_msg all_msg;
			for (int32 i = 0; i < max_cross_server_all_class_war_count; i++)
			{
				if (!m_island_all_msg.island_msg[i].is_vaild())
				{
					continue;
				}
				game_proto_cross_server_one_island_msg* island_msg = all_msg.add_island_msg();
				fill_one_island_msg(*island_msg, m_island_all_msg.island_msg[i]);
				if (all_msg.ByteSize() > packege_length_max)
				{
					session_ptr->send_to_client(&all_msg, e_msgindex_s2c_cross_server_pk_msg_info_all);
					all_msg.clear_island_msg();
				}

			}
			session_ptr->send_to_client(&all_msg, e_msgindex_s2c_cross_server_pk_msg_info_all);
		}
		else if (true == gate_proxy::getInstance().is_gate_run() )
		{
			gate2ws_server_id msg;
			msg.server_id = session_ptr->get_role_info_data(e_role_info_server_id);
			cross::send_msg_to_ws(session_ptr->get_role_guid(), world_server::getInstance().get_cross_id(), e_msgindex_s2c_cross_server_pk_msg_info_all, &msg, sizeof(msg));
		}
		
	}

	void cross_server_pk_ws_mgr::send_island_msg_one_to_client(const s_pk_island_simple_msg& island_simple_msg, guid_64 winner_guid, int32 is_end_war, int32 pk_count)
	{		
		if (false == gate_proxy::getInstance().is_gate_run()
			|| (true == gate_proxy::getInstance().is_gate_run() && world_server::getInstance().is_sky_island_server()))
		{
			game_proto_cross_server_one_island_msg one_msg;
			fill_one_island_msg(one_msg, island_simple_msg);
			if (winner_guid.is_valid())
			{
				one_msg.set_winner_guida(winner_guid.A);
				one_msg.set_winner_guidb(winner_guid.B);
			}
			one_msg.set_is_end_war(is_end_war);
			one_msg.set_pk_count(pk_count);
			client_session_mgr::getInstance().send_message_to_all_client(&one_msg, e_msgindex_s2c_cross_server_pk_msg_info_one);
		}
		if (true == gate_proxy::getInstance().is_gate_run() && world_server::getInstance().is_sky_island_server())
		{
			game_proto_cross_server_one_island_msg one_msg;
			fill_one_island_msg(one_msg, island_simple_msg);
			if (winner_guid.is_valid())
			{
				one_msg.set_winner_guida(winner_guid.A);
				one_msg.set_winner_guidb(winner_guid.B);
			}
			one_msg.set_is_end_war(is_end_war);
			one_msg.set_pk_count(pk_count);
			cross::send_msg_to_ws(guid_64(), 0, e_msgindex_s2c_clear_pk_msg_all, &one_msg, sizeof(one_msg));
		}
	}

	void cross_server_pk_ws_mgr::fill_one_island_msg(game_proto_cross_server_one_island_msg& island_msg, const s_pk_island_simple_msg& island_simple_msg)
	{
		island_msg.set_war_idex(island_simple_msg.war_idex);
		island_msg.set_island_pk_state(island_simple_msg.pk_state);
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			game_proto_pk_one_player_msg* player_msg = island_msg.add_player_msg();
			fill_pk_one_player_msg(*player_msg, island_simple_msg.island_player_msg[i]);
		}
	}

	void cross_server_pk_ws_mgr::fill_pk_one_player_msg(game_proto_pk_one_player_msg& player_msg, const s_pk_island_simple_player_msg& sim_player_msg)
	{
		player_msg.set_player_guida(sim_player_msg.player_guid.A);
		player_msg.set_player_guidb(sim_player_msg.player_guid.B);
		player_msg.set_win_num(sim_player_msg.win_num);
		player_msg.set_server_id(sim_player_msg.server_id);
		player_msg.set_player_name(sim_player_msg.role_name);
		player_msg.set_is_winner(sim_player_msg.is_winner);
	}

	int32 cross_server_pk_ws_mgr::get_map_temp_id_with_class_type(int32 class_type)
	{
		if (class_type < e_class_type_barserker || class_type > e_class_type_assassinator)
		{
			return 0;
		}
		return m_cross_pk_map_id_arr[class_type - 1];
	}

	void cross_server_pk_ws_mgr::set_cross_server_pk_map_end(int32 class_type)
	{
		class_type++;
		if (class_type < e_class_type_barserker || class_type > e_class_type_assassinator)
		{
			return;
		}
		m_is_cur_map_end[class_type - 1] = true;
	}

	void cross_server_pk_ws_mgr::req_load_island_msg()
	{
		ws2dp_load_cross_server_pk_msg_all load_msg;
		ws_client::getInstance().send_to_dp(&load_msg, sizeof(load_msg));
	}

	void cross_server_pk_ws_mgr::recv_dp_msg_to_ws(s_dp2ws_island_simple_msg& simple_msg)
	{
		if (simple_msg.is_vaild() == false)
		{
			return;
		}
		guid_64 winner_guid;
		int32 winner_serverid = 0;
		s_pk_island_simple_msg island_simple_msg;
		island_simple_msg.clear_data();
		island_simple_msg.war_idex = simple_msg.war_idex;
		island_simple_msg.pk_state = simple_msg.island_pk_state;
		island_simple_msg.island_player_msg[0].player_guid = simple_msg.winner_player_guid;
		island_simple_msg.island_player_msg[0].server_id = simple_msg.winner_player_server_id;
		island_simple_msg.island_player_msg[0].win_num = simple_msg.winner_player_win_num;
		island_simple_msg.island_player_msg[0].set_role_name(simple_msg.winner_player_name);
		island_simple_msg.island_player_msg[0].is_winner = simple_msg.one_player_is_winner;


		island_simple_msg.island_player_msg[1].player_guid = simple_msg.lose_player_guid;
		island_simple_msg.island_player_msg[1].server_id = simple_msg.lose_player_server_id;
		island_simple_msg.island_player_msg[1].win_num = simple_msg.lose_player_win_num;
		island_simple_msg.island_player_msg[1].set_role_name(simple_msg.lose_player_name);
		island_simple_msg.island_player_msg[1].is_winner = simple_msg.other_player_is_winner;

		if (simple_msg.one_player_is_winner > 0)
		{
			winner_guid = island_simple_msg.island_player_msg[0].player_guid;
			winner_serverid = island_simple_msg.island_player_msg[0].server_id;
		}
		else
		{
			winner_guid = island_simple_msg.island_player_msg[1].player_guid;
			winner_serverid = island_simple_msg.island_player_msg[1].server_id;
		}

		for (int32 i = 0; i < max_branch_type; i++)
		{
			if ((i * max_cross_server_war_count + max_cross_server_war_count - 1) == island_simple_msg.war_idex)
			{
				m_pk_player_msg[i].role_guid = winner_guid;
				m_pk_player_msg[i].server_id = winner_serverid;
			}
		}

		save_dp2ws_island_msg(island_simple_msg);
	}

	void cross_server_pk_ws_mgr::send_winner_fake_player_to_other_server(int32 server_id /* = 0 */)
	{
		if (false == world_server::getInstance().is_sky_island_server() || false == gate_proxy::getInstance().is_gate_run() || false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_pk))
		{
			return;
		}
		int32 data_num = 0;
		ws2ws_pk_harry_player fake_msg;
		for (int32 i = 0; i < max_branch_type; i++)
		{
			fake_msg.pk_player_msg[i] = m_pk_player_msg[i];
			data_num++;
		}
		fake_msg.data_num = data_num;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_pk_fake_player_msg, &fake_msg, sizeof(fake_msg));
	}

	void cross_server_pk_ws_mgr::set_winner_fake_player_gate_msg()
	{
		for (int32 i = 0; i < max_branch_type; i++)
		{
			m_pk_player_msg[i].clear_data();
		}
		for (int32 i = 0; i < max_cross_server_all_class_war_count; i++)
		{
			guid_64 winner_guid;
			int32	winner_serverid = 0;
			if (false == m_island_all_msg.island_msg[i].is_vaild())
			{
				continue;
			}
			int32 is_winner = m_island_all_msg.island_msg[i].island_player_msg[1].is_winner;

			winner_guid = m_island_all_msg.island_msg[i].island_player_msg[is_winner].player_guid;
			winner_serverid = m_island_all_msg.island_msg[i].island_player_msg[is_winner].server_id;

			for (int32 j = 0; j < max_branch_type; j++)
			{
				if ((j * max_cross_server_war_count + max_cross_server_war_count - 1) == i)
				{
					m_pk_player_msg[j].role_guid = winner_guid;
					m_pk_player_msg[j].server_id = winner_serverid;
				}
			}

		}
	}

	void cross_server_pk_ws_mgr::set_load_all_fake_player(int32 idex)
	{
		if (idex < 0 || idex >= max_branch_type)
		{
			return;
		}
		m_is_load_all_fake_player[idex] = true;
	}

	bool cross_server_pk_ws_mgr::get_is_load_all_fake_player()
	{
		bool is_load = true;
		for (int32 i = 0; i < max_branch_type; i++)
		{
			if (!m_is_load_all_fake_player[i])
			{
				is_load = false;
				break;
			}
		}
		return is_load;
	}

	s_pk_player_msg& cross_server_pk_ws_mgr::get_pk_player_msg(int32 idex)
	{
		if (idex < 0 || idex >= max_branch_type)
		{
			return m_empty_player_msg;
		}
		return m_pk_player_msg[idex];
	}

	void cross_server_pk_ws_mgr::load_fake_player_with_last_time()
	{
		for (int32 i = 0; i < max_branch_type; i++)
		{
			if (!m_is_load_all_fake_player[i])
			{
				cross_server_harry_ws_mgr::get_instance().load_fake_data_with_fake_type(i + e_fake_type_best_barserker);
			}
		}
	}

	void cross_server_pk_ws_mgr::set_winner_fake_player_with_gate_msg(int32 data_num, const s_pk_player_msg * pk_player_msg)
	{
		if (nullptr == pk_player_msg)
		{
			m_recv_gate_msg = true;
			return;
		}
		for (int32 i = 0; i < data_num; i++)
		{
			if (data_num > max_branch_type)
			{
				break;
			}
			m_pk_player_msg[i] = pk_player_msg[i];
		}
		m_recv_gate_msg = true;
	}

}
