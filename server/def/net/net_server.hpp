/*@@

Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved.

Created_datetime : 	2019/07/03

File Name :	net_server.hpp

Author : zhy

Description :

Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <singleton.hpp>
#include "server_def.hpp"

namespace faith
{
	using namespace net;

	//////////////////////////////////////////////////////////////////////////
	//	Class connection_server Declare
	//////////////////////////////////////////////////////////////////////////
	class net_server
	{
	public:
		net_server(void);
		~net_server(void);
		void clear_data();
	public:
		void set_conn_index(int32 conn_index) { m_conn_index = conn_index; }
		const int32 get_conn_index()const { return m_conn_index; }
		void set_server_status(e_server_status server_status) { m_server_status = server_status; }
		e_server_status get_server_status() { return m_server_status; }
		void set_server_info(const s_server_info& server_info) { m_server_info = server_info; }
		s_server_info get_server_info() { return m_server_info; }
		const e_server_type get_server_type()const { return m_server_info.server_type; }
		const int32 get_server_index()const { return m_server_info.server_index; }
		void set_cur_count(int32 cur_count) { m_cur_count = cur_count; }
		const int32 get_cur_count()const { return m_cur_count; }
		void set_max_count(int32 max_count) { m_max_count = max_count; }
		const int32 get_max_count()const { return m_max_count; }
		void send_message(const void *data_ptr, size_t data_len);
	private:
		s_server_info m_server_info;
		int32 m_conn_index;
		e_server_status m_server_status;
		int32 m_cur_count;
		int32 m_max_count;
	};

}
