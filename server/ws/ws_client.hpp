 /*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 11:06
	
	File Name :	ws_client.hpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#ifndef _WS_CONNECTION_MANAGER_H_
#define _WS_CONNECTION_MANAGER_H_

#include <singleton.hpp>
#include "server_def.hpp"
#include "server/client_session.hpp"
#include <gate_msg.hpp>
namespace faith
{
	using namespace net;

	class net_client;
	//////////////////////////////////////////////////////////////////////////
	//	Class ws_client Declare
	//////////////////////////////////////////////////////////////////////////
	class ws_client
	{
		friend class singleton<ws_client>;
	private:
		ws_client(void);
	public:	
		~ws_client(void);
	public:	
		/************************************************************************/
		/* ws tcp server                                                        */
		/************************************************************************/
		static ws_client& getInstance()
		{
			static ws_client s_ws_client;
			return s_ws_client;
		}
	public:	
		bool start();
		bool start_gate();
		bool start_gate_new(const s_server_info& gate_info);
		void stop();
		void send_to_dp(const void* data_ptr, size_t data_len, int32 server_id = 0);
		void send_to_dp_new(google::protobuf::Message* proto_ptr, uint32 header, int32 server_id = 0);
		void send_to_dp_lua(const char* msg, int32 msg_len, uint32 header, int32 server_id);
		void send_to_gate(const void* data_ptr, size_t data_len, int32 server_id = 0);
		void send_to_gate(const google::protobuf::Message* proto_ptr, uint32 header, int32 server_id = 0);
		void send_cross_msg(int32 recv_server_id = 0, uint32 gate_index = tcp_client::invalid_conn_index);
		void internal_gate_msg(uint32 connindex, const void* data_ptr, size_t data_len);
		void only_send_to_gate(const void* data_ptr, size_t data_len);
	private:
		void on_conn_status(const net_client* faith_client_ptr);
		void on_conn_closed(const net_client* faith_client_ptr);
		uint32 get_gate_index();
	private:	
		/************************************************************************/
		/* ws tcp server                                                        */
		/************************************************************************/
		s_server_info m_dp_info;
		uint32 m_dp_conn_index;
		s_server_info m_gate_info;
		int32 m_gate_index;
		std::vector<uint32> m_gate_conn_array;
		server2gate_to_server m_msg;

		packet_s2s		m_ws2dp_msg;
	};

}

#endif//#define __WS_CONNECTION_MANAGER_H_
