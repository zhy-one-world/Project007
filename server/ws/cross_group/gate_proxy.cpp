/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-26 9:40
	
	File Name :	gate_proxy.cpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////

#include "gate_proxy.hpp"
#include "world_server.hpp"
#include "game_cfg/servers_config.h"
#include "server_log.hpp"

namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Macro And Struct Define
	//
	//////////////////////////////////////////////////////////////////////////
	enum
	{
		INVALID_CONN_INDEX			=	0xFFFFFFFF,
		MAX_PACKET_SIZE				=	64		*1024,
		RECV_BUFFER_SIZE			=	1024	*1024,
		SEND_BUFFER_SIZE			=	1024	*1024,
		INTERVAL_RETRY_CONNECT2SAS	=	5000,
		INTERVAL_FEP_STATUS			=	5000,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Implement
	//
	//////////////////////////////////////////////////////////////////////////
	gate_proxy::gate_proxy():
		m_connIndex(INVALID_CONN_INDEX),
		m_status(E_GATE_INITED)
	{
		m_server_info.server_type = e_server_type_gate;
		m_server_info.port = GATECONFIG->internal_port;
		xstring gate_ip = GATECONFIG->internal_host;
		memcpy(m_server_info.ip_addr, gate_ip.c_str(), gate_ip.size());
	}
	bool gate_proxy::start()
	{
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross))
		{
			net_client_mgr::getInstance().start(m_server_info, 
				boost::bind(&gate_proxy::on_conn_status, this, _1),
				boost::bind(&gate_proxy::on_conn_closed, this, _1)
			);
		}
		return true;
	}

	void gate_proxy::send_message(const void *data_ptr, size_t data_len)
	{
		net_client_mgr::getInstance().send_message(m_connIndex, data_ptr, data_len);
	}

	void gate_proxy::on_conn_status(const net_client* faith_client_ptr)
	{
		m_connIndex = faith_client_ptr->get_array_index();
	}

	void gate_proxy::on_conn_closed(const net_client* faith_client_ptr)
	{
		CONSOLE_INFO("connindex:{}", faith_client_ptr->get_array_index());
		m_connIndex = INVALID_CONN_INDEX;
		set_status(E_GATE_CLOSED);
	}
	void gate_proxy::set_status(int32 status)
	{
		m_status = status;
	}
}
