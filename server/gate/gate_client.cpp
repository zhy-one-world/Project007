/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 11:13
	
	File Name :	gate_client.cpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
//#include <winsock2.h>
#include "gate_client.hpp"
#include <internal.hpp>
#include <gate_msg.hpp>
#include "game_cfg/servers_config.h"
#include "utility/init_unit.h"
#include "log_db_proxy.hpp"
#include "def/app/app_server.hpp"
#include "net/net_client_mgr.hpp"
#include "server_log.hpp"
#include "net/message_manager.hpp"

namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Implement
	//
	//////////////////////////////////////////////////////////////////////////

	gate_client::gate_client(void)
	{
		xstring gate_ip = GATECONFIG->internal_host;
		memcpy(m_gate_info.ip_addr, gate_ip.c_str(), gate_ip.size());
		m_gate_info.port = GATECONFIG->internal_port;
		m_gate_info.server_type = e_server_type_gate;
		m_gate_conn_index = tcp_client::invalid_conn_index;
	}

	gate_client::~gate_client(void)
	{
	}

	bool gate_client::start()
	{
		std::string host_ip = init_unit::get_host_ip();
		if (host_ip == m_gate_info.ip_addr)
		{
			return false;
		}
		net_client_mgr::getInstance().start(m_gate_info,
			boost::bind(&gate_client::on_conn_status, &gate_client::getInstance(), _1),
			boost::bind(&gate_client::on_conn_closed, &gate_client::getInstance(), _1)
		);
		return true;
	}
	void gate_client::stop()
	{
		net_client_mgr::getInstance().stop();
	}
	void gate_client::on_conn_status(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_gate:
		{
			m_gate_conn_index = faith_client_ptr->get_array_index();
		}
		break;
		default:
			break;
		}
	}
	void gate_client::on_conn_closed(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_gate:
		{
			m_gate_conn_index = tcp_client::invalid_conn_index;
		}
		break;
		default:
			break;
		}
	}
}
