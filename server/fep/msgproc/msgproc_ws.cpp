/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   11:54
	file base:	msgproc_ws
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "msgproc_ws.hpp"
#include <rlog.hpp>
#include "logic/type_def.hpp"
#include <Logic/login_def.hpp>
#include <gate_msg.hpp>
#include <login_msg.hpp>
#include <core.hpp>
#include "../server/fep_client.hpp"
#include "../server/proxy_service_cli.hpp"
#include "../server/fep_client.hpp"
#include "../server/security_communication_layer.hpp"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include <Utility/serialize_msg.h>
#include "server_log.hpp"
#include "net.pb.h"
#include "game.pb.h"

namespace faith
{
	void ws2fep_broadcast_msg_fun(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2fep_broadcast_msg* pdata = static_cast<const ws2fep_broadcast_msg*>(data_ptr);
		if (NULL == pdata)
			return;
		proxy_service_cli::getInstance().broadcast(pdata->data, pdata->data_size, pdata->header);
	}
	void ws2fep_client_login_queue_status(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2fep_login_queue_status* pdata = static_cast<const ws2fep_login_queue_status*>(data_ptr);
		if ( NULL == pdata )
			return;

		auto client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if (nullptr == client_session_ptr || pdata->client_uid != client_session_ptr->get_client_uid())
		{
			return;
		}

		login_proto_login_queue_status login_queue;
		login_queue.set_left_time_in_sec(pdata->left_time_in_sec);
		login_queue.set_position(pdata->queue_pos);
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), &login_queue, e_msgindex_s2c_login_queue_status);
	}

	void ws2fep_client_loginned(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2fep_client_logined* pdata = static_cast<const ws2fep_client_logined*>(data_ptr);
		if (NULL == pdata)
		{
			_RLOG_(MERROR, "pdata is null");
			return;
		}

		auto client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if (nullptr == client_session_ptr || pdata->client_uid != client_session_ptr->get_client_uid())
		{
			_RLOG_(MERROR, "client_session_ptr is null pdata->client_uid.fepsession_uid="
				<< pdata->client_uid.fepsession_uid);
			return;
		}

		client_session_ptr->response_login(pdata->eResult, pdata->queue_pos, pdata->left_time_in_sec);
	}

	void ws2fep_reconnect_game_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2fep_reconnect_game* pdata = static_cast<const ws2fep_reconnect_game*>(data_ptr);
		if (NULL == pdata)
		{
			string str = " the pdata is null";
			server_log::reconnect_game_log("fep", __FUNCTION__, str.c_str());
			return;
		}

		auto client_session_ptr = proxy_service_cli::getInstance().get_session_by_account(pdata->client_uid.fepsession_uid, pdata->account);
		if (nullptr == client_session_ptr)
		{
			string str = " the client_session_ptr is null";
			server_log::reconnect_game_log("fep", __FUNCTION__, str.c_str());
			return;
		}
		login_proto_login_reconnect_end reconnect_game_end;
		reconnect_game_end.set_reconnect_res(pdata->reconnect_res);
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), &reconnect_game_end, e_msgindex_s2c_login_reconnect);

		if (pdata->reconnect_res == e_reconnect_result_win)
		{
			client_session_ptr->set_cs_uid(pdata->cellserver_id);
			client_session_ptr->set_cs_array_index(pdata->array_index);
		}
		else
		{
			client_session_ptr->set_is_logout(true);
			//proxy_service_cli::getInstance().disconn_session(client_session_ptr->get_conn_index(), e_logout_result_connect_dis);
		}
	}
	void ws2fep_enter_game_func(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2fep_enter_game* pdata = static_cast<const ws2fep_enter_game*>(data_ptr);
		if ( NULL == pdata )
			return;

		auto client_session_ptr = proxy_service_cli::getInstance().get_session_by_account(pdata->client_uid.fepsession_uid, pdata->account);
		if (nullptr == client_session_ptr || client_session_ptr->get_client_uid() != pdata->client_uid)
		{
			return;
		}
		if (pdata->e_result == e_error_code_success)
		{
			if (pdata->server_type == e_server_type_cs)
			{
				client_session_ptr->set_cs_uid(connindex);
				client_session_ptr->set_cs_array_index(pdata->cs_array_index);
			}
		}
		else
		{
			_RLOG_(MINFO, "eResult:" << (int32)pdata->e_result);
			client_session_ptr->set_is_logout(true);
		}
		game_proto_enter_game_end enter_game_end;
		enter_game_end.set_result(pdata->e_result);
		enter_game_end.set_server_type(pdata->server_type);
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), &enter_game_end, e_msgindex_s2c_enter_game);
	}

	void ws2fep_kickout_account_func( uint32 connindex, const void* data_ptr, size_t data_len )
	{
		const ws2fep_kickout_account* pdata = static_cast<const ws2fep_kickout_account*>(data_ptr);
		if (NULL == pdata)
		{
			string str = " the pdata is null";
			server_log::reconnect_game_log("fep", __FUNCTION__, str.c_str());
			return;
		}
		auto client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if(nullptr == client_session_ptr || pdata->client_uid != client_session_ptr->get_client_uid())
		{
			string str = " the client_session_ptr is null or vaild or client_uid != client_session_ptr->get_client_uid";
			server_log::reconnect_game_log("fep", __FUNCTION__, str.c_str());
			return;
		}
		_RLOG_(MINFO, "reason:" << (int32)pdata->reason);
		login_proto_logout_end logout_msg;
		logout_msg.set_result(pdata->reason);
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), &logout_msg, e_msgindex_s2c_logout);
		client_session_ptr->set_is_logout(true);
	}
}
