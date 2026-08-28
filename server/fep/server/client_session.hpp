/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   17:32
	file base:	client_session
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _CLIENT_CONNECTION_SESSION_H_
#define _CLIENT_CONNECTION_SESSION_H_

#include <time.hpp>
#include <base.hpp>
#include "logic/type_def.hpp"
#include <Logic/char_def.hpp>
#include <Logic/login_def.hpp>
#include <Logic/gate_def.hpp>
#include <loop_int.hpp>
#include "login.pb.h"

namespace hld
{
	class client_session : private boost::noncopyable
	{
	public:	
		client_session();
		~client_session();	
		void clear_data();
		void update(const int64& new_time);
	public:
		bool&				is_vaild() { return m_data_is_use; }
		void				set_data_use(bool is_use) { m_data_is_use = is_use; }
		bool&				get_is_logout() { return m_is_logout; }
		void				set_is_logout(bool is_logout) { m_is_logout = is_logout; }
		void				set_conn_index(uint32 conn_index) { m_conn_index = conn_index; }
		uint32				get_conn_index() { return m_conn_index; }

		void				set_array_index(uint32 array_index);
		uint32				get_array_index() { return m_array_index; }

		xchar*				get_account()										{  return m_account;  }
		void				set_account(const xchar account[max_account_length + 1]);
		void				set_account(const xstring& account);
		void				set_is_create(bool is_create_new) { m_is_create_account = is_create_new; };

		xchar*				get_server_msg() { return m_server_msg; }
		void				set_server_msg(const xchar server_msg[max_server_msg_length + 1]);

		void				set_client_uid();
		s_client_uid&		get_client_uid()									{  return m_client_uid;  }

		uint32				get_cs_uid()										{  return m_cs_conn_index;  }
		void				set_cs_uid(const uint32 cs_uid) { m_cs_conn_index = cs_uid; }

		void				set_cs_array_index(const int32 array_index) { m_cs_array_index = array_index; }
		int32				get_cs_array_index() { return m_cs_array_index; }

		void				refresh_heart_beat();// 刷新心跳时间

		void				check_session(const int64& new_time);

		ui8					get_msg_index() { return m_msg_index++; }
	public:
		void				set_login_proto_login(const login_proto_login& login_msg) { m_login_proto_login = login_msg; }
		login_proto_login&	get_login_proto_login() { return m_login_proto_login; }
		void				response_login(int32 eresult, int32 queue_pos = 0, int32 left_time_in_sec = 0);
		void				send_to_ws_login(
			int32 login_type, 
			int32 login_type_plus,
			int32 jewel_num,
			const int64* ban_role_array, 
			const int64* ban_chat_array,
			const guid_64& role_guid,
			const guid_64& map_guid,
			const int32& war_index,
			const int32& server_id);
		
		void				send_to_ws_leave_queued();
		void				set_radio_host(int32 radio_host);
		void				set_online_duration(int32 online_duration);
		int32				get_online_duration();
		void				set_online_time(int32 online_time);
		int32				get_online_time();
		void				set_age(int32 age);
		int32				get_age();
		void				set_month_recharge_num(int32 month_recharge_num);
		int32				get_month_recharge_num();
		void				set_month_recharge_time(int32 month_recharge_time);
		int32				get_month_recharge_time();


	public:
		xchar				m_account[hld::max_account_length + 1];			//	account for login
		xchar				m_server_msg[hld::max_server_msg_length + 1];			//	server_msg for login
		xchar				m_ipaddr[hld::max_ip_address_length + 1];
		bool				m_is_create_account;
		int64				m_heart_beat_time;					//上次心跳时间(上次收到客户端消息时间)
		int64				m_heart_login_time;					//玩家在登陆界面的时间，时间过长就踢掉
		int32				m_client_send_msg_count;            //客户端请求计数
		bool				m_is_send_login_success;			//已经发送过登陆成功的消息
	private:
		bool				m_data_is_use;
		bool				m_is_logout;
		ui8					m_msg_index;
		uint32				m_conn_index;
		uint32				m_array_index;
		uint32				m_cs_conn_index;							//The cell server uid that the client session use
		int32				m_cs_array_index;					//cs unit_man player array index;
		s_client_uid		m_client_uid;
		//guid_64				m_optional_char_guids[max_enum_character_num];

		int64				m_client_send_msg_time;				//封包幾次時間
		int32				m_client_send_error_count;          //客户端错误请求计数
		int32				m_client_random_close_num;          //客户端错误请求计数

		login_proto_login	m_login_proto_login;

		int32				m_radio_host;
		int32				m_online_duration;
		int32				m_online_time;
		int32				m_age;
		int32				m_month_recharge_num;
		int32				m_month_recharge_time;

		int64				m_online_tick;	//	慢tick 1分钟 一次
	};

}

#endif//#define __CLIENT_CONNECTION_SESSION_H_
