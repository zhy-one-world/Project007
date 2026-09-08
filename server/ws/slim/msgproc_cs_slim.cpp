/* slim CS handlers: session / map / enter / logout / transfer */
#include "msgproc/msgproc_cs.hpp"
#include "server/client_session.hpp"
#include "server/client_session_mgr.hpp"
#include "world_server.hpp"
#include "cross_group/gate_proxy.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "business_stubs.hpp"
#include "server_log.hpp"
#include "system/scene/cs_map_system.h"
#include "system/scene/cs_map_mgr_system.h"
#include "game.pb.h"
#include "net.pb.h"
#include <login_msg.hpp>
#include <char_msg.hpp>
#include <core.hpp>
#include <rlog.hpp>

namespace faith
{
	void cs2ws_rep_reconnect_game(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_reconnect_game* packet = static_cast<const cs2ws_reconnect_game*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == client_session_ptr)
		{
			ws2cs_client_logout	req_logout;
			req_logout.role_guid = packet->role_guid;
			req_logout.client_uid = packet->client_uid;
			world_server::getInstance().broadcast(&req_logout, sizeof(req_logout), e_server_type_cs);
			return;
		}
		if (packet->reconnect_res == e_reconnect_result_win)
		{
			client_session_ptr->m_logout_begin = false;
			client_session_ptr->m_logout_time = 0;
			marry_mgr_ws::get_instance().session_online_marry_handle(client_session_ptr->get_role_guid());
			team_ws_mgr::get_instance().on_player_online(client_session_ptr);
			legion_ws_mgr::get_instance().on_player_reconnect(client_session_ptr);

			ws2gateway_reconnect_game gateway_msg;
			gateway_msg.client_uid = packet->client_uid;
			gateway_msg.cellserver_id = client_session_ptr->get_cs_conn_index();
			gateway_msg.array_index = client_session_ptr->get_cs_array_index();
			gateway_msg.reconnect_res = e_reconnect_result_win;
			memcpy(gateway_msg.account, client_session_ptr->m_account, sizeof(gateway_msg.account));
			client_session_ptr->send_to_gateway(&gateway_msg, sizeof(gateway_msg));
		}
		else
		{
			ws2gateway_reconnect_game gateway_msg;
			gateway_msg.client_uid = packet->client_uid;
			gateway_msg.reconnect_res = e_reconnect_result_cs_no_find;
			memcpy(gateway_msg.account, client_session_ptr->m_account, sizeof(gateway_msg.account));
			client_session_ptr->send_to_gateway(&gateway_msg, sizeof(gateway_msg));
			client_session_mgr::getInstance().logout_client(client_session_ptr);
		}
	}

	void cs2ws_rep_enter_game(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_enter_game* packet = static_cast<const cs2ws_enter_game*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(packet->char_info_to_ws.role_guid);
		if (session == NULL)
		{
			ws2cs_client_logout	req_logout;
			req_logout.client_uid = packet->client_uid;
			req_logout.role_guid = packet->char_info_to_ws.role_guid;
			world_server::getInstance().send_by_uid(conn_index, &req_logout, sizeof(req_logout));
			return;
		}
		switch (packet->eResult)
		{
		case cs2ws_enter_game::e_success:
		{
			session->m_status = client_session::e_ss_ingame;
			session->set_cs_array_index(packet->char_info_to_ws.array_index);
			session->m_step_num = client_session::e_session_step_cs_enter_game;
			ws2gateway_enter_game rep_gateway;
			rep_gateway.client_uid = session->get_client_uid();
			rep_gateway.e_result = e_error_code_success;
			rep_gateway.cs_array_index = packet->char_info_to_ws.array_index;
			rep_gateway.server_type = e_server_type_ws;
			memcpy(rep_gateway.account, session->m_account, sizeof(rep_gateway.account));
			session->send_to_gateway(&rep_gateway, sizeof(rep_gateway));
		}
			break;
		case cs2ws_enter_game::e_failed_repeat_player_id:
		default:
		{
			client_session_mgr::getInstance().kickout_account(session->m_account, e_logout_result_enter_scene_failed);
		}
			break;
		}
	}

	void cs2ws_rep_enter_scene(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_enter_scene* packet = static_cast<const cs2ws_enter_scene*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (session == NULL)
		{
			return;
		}

		switch (packet->eResult)
		{
		case cs2ws_enter_scene::e_failed_unknow_error:
		case cs2ws_enter_scene::e_enter_ret_player_err:
		{
			client_session_mgr::getInstance().kickout_account(session->m_account, e_logout_result_enter_scene_failed);
		}
		break;
		case cs2ws_enter_scene::e_enter_ret_already_in:
		{
			game_proto_enter_scene_end enter_scene_end;
			enter_scene_end.set_result(faith::e_error_code_repeated);
			session->send_to_client(&enter_scene_end, e_msgindex_s2c_enter_scene);
		}
		break;
		case cs2ws_enter_scene::e_enter_ret_success:
		{
			game_proto_enter_scene_end enter_scene_end;
			enter_scene_end.set_result(faith::e_error_code_success);
			session->send_to_client(&enter_scene_end, e_msgindex_s2c_enter_scene);
			session->enter_scene_logic_proc();
		}
		break;
		}
	}

	void cs2ws_rep_logout(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_client_logout* packet = static_cast<const cs2ws_client_logout*>(data_ptr);
		if (packet == NULL)
			return;

		client_session* session = client_session_mgr::getInstance().get_session(packet->player_guid);
		if (session == NULL)
			return;
		session->set_cs_conn_index(e_invalid_server_uid);

		if (session->m_status != client_session::e_ss_logout)
		{
			client_session_mgr::getInstance().kickout_account(session->m_account, packet->logout_result);
		}
		else
		{
			client_session_mgr::getInstance().logout_complete(session);
		}
	}

	void cs2ws_rep_role_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const faith::cs2ws_role_info* packet = static_cast<const cs2ws_role_info*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (session == NULL)
		{
			return;
		}
		if (packet->role_index >= e_role_info_max)
		{
			return;
		}
		session->m_role_info.data_ary[packet->role_index] = packet->role_value;
	}

	void cs2ws_rep_map_state(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const faith::cs2ws_map_state* packet = static_cast<const cs2ws_map_state*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		auto map_ent = get_entity(packet->map_guid);
		cs_map_system::set_map_state(map_ent, packet->map_state);
	}

	void cs2ws_req_transfer_player(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const faith::cs2ws_tansfer_player* packet = static_cast<const cs2ws_tansfer_player*>(data_ptr);
		if (packet == NULL)
		{
			CONSOLE_ERROR("cs2ws_req_transfer_player packet is null");
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (session == NULL)
		{
			CONSOLE_ERROR("cs2ws_req_transfer_player session is null role_guid:{}", packet->role_guid.server_64);
			return;
		}

		guid_64 now_map_guid = session->get_map_guid();
		session->set_last_map_guid(now_map_guid);
		int32 unit_num_after_rm = cs_map_mgr_system::remove_unit_from_map(now_map_guid, session->get_role_guid());
		team_ws_mgr::get_instance().on_unit_leave_map(session, unit_num_after_rm);

		if (packet->dest_group_id != 0 && gate_proxy::getInstance().is_gate_run() && packet->dest_group_id != world_server::getInstance().get_server_id())
		{
			cross::transfer_player(session, packet->dest_group_id, packet->transfer_info);
			return;
		}
		session->set_transfer_info(packet->transfer_info);
		session->m_status = client_session::e_ss_map_transfer;
		session->set_cs_conn_index(-1);
		memcpy(&session->m_role_info, &packet->role_unit_info, sizeof(session->m_role_info));

		session->login_try_enter_scene();
	}

	void cs2ws_rep_get_map_info_from_cs_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_get_map_info_from_cs_end* packet = static_cast<const cs2ws_get_map_info_from_cs_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_get_map_info_from_cs_end))
		{
			return;
		}
		int64 map_info[map_info_arr_max_num];
		for (int32 i = 0; i < map_info_arr_max_num; i++)
		{
			map_info[i] = packet->map_info[i];
		}
		cs_map_mgr_system::send_one_map_info_to_client(packet->role_guid, packet->map_template_id, map_info);
	}

	void cs2ws_reload_csv_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		world_server::getInstance().set_cs_reload_csv_end();
	}
}
