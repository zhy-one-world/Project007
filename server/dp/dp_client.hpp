 /*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 11:06
	
	File Name :	dp_client.hpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#ifndef _DP_CLIENT_H_
#define _DP_CLIENT_H_

#include <singleton.hpp>
#include <net/net_client_mgr.hpp>

namespace faith
{
	using namespace net;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Class dp_client Declare
	//
	//////////////////////////////////////////////////////////////////////////
	struct s_gate_message
	{
		s_gate_message()
		{
			memset(this, 0, sizeof(*this));
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		int64 msg_tick;
		e_server_type send_server_type;
		int32 send_server_id;
		int32 send_server_index;
		uint32 gate_conn_index;
	};
	typedef std::map<int32, s_gate_message> gate_message_map;
	typedef gate_message_map::iterator gate_message_map_it;
	class dp_client :	public singleton<dp_client>
	{
		friend class singleton<dp_client>;
	private:
		dp_client(void);
	public:	
		~dp_client(void);
	public:
		void tick(const int64& new_time);
	public:	
		bool start();
		bool start_gate_new(const s_server_info& gate_info);
		void stop();
		void send_to_gate(const void* data_ptr, size_t data_len, uint32 conn_index);
		void internal_gate_msg(uint32 connindex, const void* data_ptr, size_t data_len);
		void gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len);
	private:
		void on_conn_status(const net_client* faith_client_ptr);
		void on_conn_closed(const net_client* faith_client_ptr);
	private:
		s_server_info m_gate_info;
		gate_message_map m_gate_message_map;
		int32 m_begin_message_index;
	};

}

#endif//#define __WS_CONNECTION_MANAGER_H_
