/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019/07/03
	
	File Name :	faith_server.cpp
	
	Author : zhy
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "net_server.hpp"
#include "server_log.hpp"
#include "net_server_mgr.hpp"

namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Implement
	//
	//////////////////////////////////////////////////////////////////////////

	net_server::net_server(void)
	{
		m_conn_index = 0;
		clear_data();
	}
	net_server::~net_server(void)
	{
	}
	void net_server::clear_data()
	{
		m_server_info.clear_data();
		m_cur_count = 0;
		m_max_count = 0;
		m_server_status = e_serverstatus_created;
	}
	void net_server::send_message(const void *data_ptr, size_t data_len)
	{
		net_server_mgr::getInstance().send_message(data_ptr, data_len, m_conn_index);
	}
}
