/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   17:25
	file base:	msgproc_client
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "msgproc_client.hpp"
#include <gate_msg.hpp>
#include <login_msg.hpp>
#include <char_msg.hpp>
#include <core.hpp>
#include "../server/proxy_service_cli.hpp"
#include "../server/fep_client.hpp"
#include "game_cfg/servers_config.h"
#include <time.hpp>
#include <Utility/serialize_msg.h>
#include <Utility/parse_msg.h>
#include <Utility/cs_date.hpp>
#include "server_log.hpp"
#include <Utility/init_unit.h>
#include "game.pb.h"
#include "cross.pb.h"
#include "net.pb.h"
#include "character.pb.h"

namespace faith
{
	void fep2c_ping(client_session* client_session_ptr, const int64& client_time)
	{
		if (nullptr == client_session_ptr)
		{
			return;
		}
		game_proto_fep2client_ping server_ping;
		server_ping.set_client_time(client_time);
		server_ping.set_server_time(utility::get_tick_count());
		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), &server_ping, e_msgindex_fep2c_ping);
	}

	void c2fep_ping(uint32 array_index,const void *data_ptr,size_t data_len)
	{
		game_proto_client2fep_ping ping_msg;
		parse_msg::getInstance().parse_message_new(&ping_msg, data_ptr, data_len);
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}
		
		client_session_ptr->refresh_heart_beat();
		fep2c_ping(client_session_ptr, ping_msg.client_time());
	}
	void c2ls_req_login_reconnect(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			string str = "get_session error or session is vaild, id: ";
			str += to_string(array_index);
			server_log::reconnect_game_log("fep", __FUNCTION__, str.c_str());
			return;
		}
		login_proto_login_reconnect login_reconnect;
		parse_msg::getInstance().parse_message_new(&login_reconnect, data_ptr, data_len);
		int32 size_account = login_reconnect.account().size();
		// 验证登陆使用的账户名和密码
		if (size_account == 0 || size_account > max_account_length)
		{
			client_session_ptr->response_login(e_error_code_login_invalid_reconnect_account);
			client_session_ptr->set_is_logout(true);

			string str = " the login account_size is not in the rule account:";
			str += login_reconnect.account();
			server_log::reconnect_game_log("fep", __FUNCTION__, str.c_str());

			return;
		}

		client_session_ptr->set_account(login_reconnect.account());
		fep2ws_client_reconnect msg;
		msg.client_uid = client_session_ptr->get_client_uid();
		msg.cur_map_id = login_reconnect.cur_map_id();
		msg.cur_map_line_id = login_reconnect.cur_map_line_id();
		memcpy(msg.account, login_reconnect.account().c_str(), size_account);
		fep_client::getInstance().send_message_to_ws(&msg, sizeof(msg));
		return;
	}
	void c2ls_req_login(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			CONSOLE_INFO("c2ls_req_login client_session_ptr is null array_index:{} data_len:{}", array_index, data_len);
			return;
		}

		packet_c2s_s2c* packet = (packet_c2s_s2c*)data_ptr;
		if (NULL == packet)
		{
			CONSOLE_INFO("c2ls_req_login array_index:{} data_len:{}", array_index, data_len);
			return;
		}
		login_proto_login login;
		parse_msg::getInstance().parse_message_new(&login, data_ptr, data_len);

		std::string app_key = "10000";
		auto SdkDataPtr = login.mutable_sdk_data();
		SdkDataPtr->set_app_key(app_key);
		if (login.login_type() == e_login_type_new_account_and_password || login.login_type() == e_login_type_new_korea)
		{
			int32 size_account = login.logic_account().size();
			int32 size_password = login.password().size();
			// 验证登陆使用的账户名和密码
			if (size_account < min_account_length 
				|| size_account > max_account_length 
				|| size_password < min_account_length 
				|| size_password > max_password_length)
			{
				client_session_ptr->response_login(e_error_code_login_invalid_reconnect_account);
				return;
			}
			if (login.logic_account()[0] == '\0' || login.password()[0] == '\0')
			{
				client_session_ptr->response_login(e_error_code_login_invalid_reconnect_account);
				return;
			}

			if (init_unit::account_is_right(login.logic_account()) == false)
			{
				client_session_ptr->response_login(e_error_code_login_invalid_reconnect_account);
				return;
			}
			if (init_unit::account_is_right(login.password()) == false)
			{
				client_session_ptr->response_login(e_error_code_login_invalid_reconnect_account);
				return;
			}
		}
		client_session_ptr->set_login_proto_login(login);
		client_session_ptr->get_login_proto_login().set_ip(client_session_ptr->m_ipaddr);

		login_proto_login login_pro;
		login_pro.set_client_account(login.client_account());
		login_pro.set_logic_account(login.logic_account());
		login_pro.set_password(login.password());
		login_pro.set_login_type(login.login_type());
		login_pro.set_client_version(login.client_version());
		login_pro.set_buildtimes(login.buildtimes());
		login_pro.set_switch_cluster(login.switch_cluster());
		login_pro.set_captcha_str(login.captcha_str());
		login_pro.set_ip(client_session_ptr->m_ipaddr);
		login_pro.set_server_id(login.server_id());
		login_pro.set_device_type(login.device_type());
		login_pro.set_custom_info(login.custom_info());
		auto sdk_pro = login_pro.mutable_sdk_data();
		sdk_pro->set_data(login.sdk_data().data());
		sdk_pro->set_data_size(login.sdk_data().data_size());
		sdk_pro->set_app_key(login.sdk_data().app_key());
		sdk_pro->set_app_secret(login.sdk_data().app_secret());
		sdk_pro->set_channel_id(login.sdk_data().channel_id());
		sdk_pro->set_device_id(login.sdk_data().device_id());
		sdk_pro->set_opcode(login.sdk_data().opcode());
		sdk_pro->set_model(login.sdk_data().model());
		sdk_pro->set_idfa(login.sdk_data().idfa());
		sdk_pro->set_imei(login.sdk_data().imei());
		sdk_pro->set_android_id(login.sdk_data().android_id());
		sdk_pro->set_mac(login.sdk_data().mac());
		sdk_pro->set_sn(login.sdk_data().sn());

		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &login_pro, client_session_ptr->get_client_uid(), packet->wheader);
		fep_client::getInstance().send_message_to_ws( &msg, msg.get_packet_len());
		//xstring log_string = "";
		//log_string = server_log::login_role_log(login.server_id(), login.sdk_data().app_key(), login.client_version(), login.sdk_data().channel_id(),
		//	login.sdk_data().app_secret(), login.account(), login.sdk_data().device_id(), login.client_version(), login.sdk_data().model());
		//xstring table_name = "login_role_log";
	}
	void c2ws_token_login(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}

		cross_proto_cross_transfer_to_server clientReq;
		parse_msg::getInstance().parse_message_new(&clientReq, data_ptr, data_len);
		
		xchar req_account[max_account_length + 1] = { 0 };
		int32 account_len = clientReq.account().size() > max_account_length ? max_account_length : clientReq.account().size();
		memcpy(req_account, clientReq.account().c_str(), account_len);
		client_session_ptr->set_login_proto_login(clientReq.login_info());
		client_session_ptr->set_account(req_account);

		int64 ban_role_array[faith::max_character_num];
		int64 ban_chat_array[faith::max_character_num];

		memset(ban_role_array, 0, sizeof(ban_role_array));
		memset(ban_chat_array, 0, sizeof(ban_chat_array));
		client_session_ptr->send_to_ws_login(
			e_login_type_new_token, 
			0, 
			0, 
			ban_role_array, 
			ban_chat_array,
			clientReq.role_guid(), 
			clientReq.map_guid(),
			clientReq.war_index(),
			clientReq.server_id());
	}
	void c2fep_logout(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}
		proxy_service_cli::getInstance().on_conn_closed(client_session_ptr->get_conn_index());
	}

	void c2fep_enum_character(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		character_proto_enum_character enum_info;
		parse_msg::getInstance().parse_message_new(&enum_info, data_ptr, data_len);
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}

		enum_info.set_account(client_session_ptr->m_account);
		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &enum_info, client_session_ptr->get_client_uid(), e_msgindex_c2s_enum_char);

		fep_client::getInstance().send_message_to_ws( &msg, msg.get_packet_len());
	}

	void c2fep_select_character(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		game_proto_enter_game select_info;
		parse_msg::getInstance().parse_message_new(&select_info, data_ptr, data_len);
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}

		//if (false == client_session_ptr->is_selected_guid_valid(select_info.role_guid()))
		//{
		//	return;
		//}

		fep_client::getInstance().send_message_to_ws( data_ptr, data_len);
	}

	void c2fep_create_character(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		character_proto_create_character create_info;
		parse_msg::getInstance().parse_message_new(&create_info, data_ptr, data_len);
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}
		create_info.set_account(client_session_ptr->m_account);
		fep2ls_create_character msg;
		msg.client_uid = client_session_ptr->get_client_uid();
		create_info.SerializeToArray((void*)msg.create_character_msg, sizeof(msg.create_character_msg));
		msg.create_character_len = create_info.ByteSize();
		client_session_ptr->get_login_proto_login().SerializeToArray((void*)msg.login_proto_msg, sizeof(msg.login_proto_msg));
		msg.login_msg_len = client_session_ptr->get_login_proto_login().ByteSize();
		fep_client::getInstance().send_message_to_ws( &msg, sizeof(msg));
	}

	void c2fep_del_character(uint32 array_index, const void *data_ptr, size_t data_len)
	{
		character_proto_delete_character del_info;
		parse_msg::getInstance().parse_message_new(&del_info, data_ptr, data_len);
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}

		del_info.set_account(client_session_ptr->m_account);
		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &del_info, client_session_ptr->get_client_uid(), e_msgindex_c2s_del_char);

		fep_client::getInstance().send_message_to_ws( &msg, msg.get_packet_len());
	}
}
