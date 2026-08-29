/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   11:45
	file base:	msgproc_ls
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "msgproc_ls.hpp"
#include "../server/proxy_service_cli.hpp"
#include "../server/fep_client.hpp"
#include <login_msg.hpp>
#include <char_msg.hpp>
#include <core.hpp>
#include <Logic/login_def.hpp>
#include "utility/init_unit.h"
#include <Utility/serialize_msg.h>
#include <Utility/parse_msg.h>
#include "server_log.hpp"
#include "net.pb.h"
#include "character.pb.h"

namespace faith
{
	void ls2fep_rep_client_login(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ls2fep_client_login* pdata = static_cast<const ls2fep_client_login*>(data_ptr);
		if ( NULL == pdata )
		{
			CONSOLE_INFO("ls2fep_rep_client_login pdata is null data_len:{}", data_len);
			return;
		}

		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if(nullptr == client_session_ptr || client_session_ptr->is_vaild() == false || pdata->client_uid != client_session_ptr->get_client_uid())
		{
			CONSOLE_INFO("ls2fep_rep_client_login client_session_ptr is null pdata->client_uid.fepsession_uid:{}", pdata->client_uid.fepsession_uid);
			return;
		}

		client_session_ptr->set_account(pdata->account);
		client_session_ptr->set_server_msg(pdata->server_msg);
		client_session_ptr->set_online_duration(pdata->online_duration);
		client_session_ptr->set_online_time(pdata->online_time);
		client_session_ptr->set_age(pdata->age);
		client_session_ptr->set_month_recharge_num(pdata->month_recharge_num);
		client_session_ptr->set_month_recharge_time(pdata->month_recharge_time);

		if (pdata->eResult != e_error_code_success)
		{
			client_session_ptr->response_login(pdata->eResult);
			CONSOLE_INFO("ls2fep_rep_client_login send_to_ws_login fail pdata->eResult:{} ", pdata->eResult);
		}
		else
		{
			if (strlen(pdata->account) < min_account_length)
			{
				CONSOLE_INFO("ls2fep_rep_client_login account is limit, account:{} ", pdata->account);
			}
			
			client_session_ptr->set_radio_host(pdata->radio_host);
			client_session_ptr->set_is_create(pdata->is_create_new);
			client_session_ptr->send_to_ws_login(
				pdata->login_type,
				pdata->login_type_plus,
				pdata->jewel_num, 
				pdata->ban_role_array,
				pdata->ban_chat_array,
				guid_64(),
				guid_64(),
				0,
				0);
		}
	}

	void ls2fep_rep_client_enum_char(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid = parse_msg::getInstance().get_packet_connect_idx(data_ptr, data_len);

		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(client_uid.fepsession_uid);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false || client_session_ptr->get_client_uid() != client_uid)
		{
			return;
		}
		
		//character_proto_enum_character_end_info character_end_info_msg;
		//parse_msg::getInstance().parse_message_new(&character_end_info_msg, data_ptr, data_len);
		//client_session_ptr->insert_optional_guid(character_end_info_msg.role_guid());

		//传送客户端
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), data_ptr, data_len);
	}

	void ls2fep_rep_client_create_char(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ls2fep_create_role* pdata = static_cast<const ls2fep_create_role*>(data_ptr);
		if (!pdata || data_len != sizeof(ls2fep_create_role))
		{
			return;
		}
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(pdata->client_uid.fepsession_uid);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false || client_session_ptr->get_client_uid() != pdata->client_uid)
		{
			return;
		}
		character_proto_create_character_end cre_char;
		cre_char.set_result(pdata->eresult);
		cre_char.set_role_guid(pdata->role_guid.server_64);
		cre_char.set_create_time(time_helper::get_time());
		cre_char.set_role_name(pdata->role_info.role_name);
		for (int32 i = 0; i < e_role_info_max; ++i)
		{
			cre_char.add_role_info(pdata->role_info.data_ary[i]);
		}
		//传送客户端
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), &cre_char, e_msgindex_s2c_create_char);
	}

	void ls2fep_rep_client_del_char(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid = parse_msg::getInstance().get_packet_connect_idx(data_ptr, data_len);

		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(client_uid.fepsession_uid);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false || client_session_ptr->get_client_uid() != client_uid)
		{
			return;
		}
		
		//character_proto_delete_character_end del_character_end_msg;
		//parse_msg::getInstance().parse_message_new(&del_character_end_msg, data_ptr, data_len);
		//if (del_character_end_msg.del_type() == e_del_char_delete)
		//{
		//	client_session_ptr->remove_optional_guid(del_character_end_msg.role_guid());
		//}

		//传送客户端
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), data_ptr, data_len);
	}
}
