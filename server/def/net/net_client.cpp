/********************************************************************
  created: 2019/07/03
  file base: server_client
  file ext: cpp
  author: zhy
  
  purpose: 
*********************************************************************/
/************************************************************************/
/*                          	File Include                            */
/************************************************************************/
#include "internal/internal.hpp"
#include "net_client.hpp"
#include "net_server_mgr.hpp"
#include "server_log.hpp"
#include "utility/init_unit.h"
#include <net/scheduler.hpp>

namespace faith
{
	net_client::net_client(void)
	{
		m_timerindex_connect = scheduler::scheduler_invalid_timer_index;
		clear_data();
	}

	net_client::~net_client(void)
	{
	}

	void net_client::clear_data()
	{
		if (m_timerindex_connect != scheduler::scheduler_invalid_timer_index)
		{
			scheduler::getInstance().remove_timer(m_timerindex_connect);
			m_timerindex_connect = scheduler::scheduler_invalid_timer_index;
		}
		m_server_status = e_serverstatus_created;
		m_conn_index = tcp_client::invalid_conn_index;
		m_server_info.clear_data();
	}
	int32 net_client::send_message(const void* data_ptr, size_t data_len)
	{
		if (m_server_status > e_serverstatus_created)
		{
			return tcp_client::get_instance().send(m_conn_index, data_ptr, data_len);
		}
		else
		{
			return 0;
		}
	}

	void net_client::retry_connect(uint32 timer_index)
	{
		if(m_timerindex_connect != scheduler::scheduler_invalid_timer_index)
		{
			scheduler::getInstance().remove_timer(m_timerindex_connect);
			m_timerindex_connect = scheduler::scheduler_invalid_timer_index;
		}
		connect_to();
	}

	void net_client::on_conn_status(uint32 connindex, tcp_client::e_connect_info status, xstring info)
	{
		CONSOLE_INFO(" info = {} status = {}", info, (int32)status);
		switch (status)
		{
		case faith::net::tcp_client::e_ci_common_error:
		case faith::net::tcp_client::e_ci_addr_resovle_failed:
		case faith::net::tcp_client::e_ci_connection_failed:
		{
			m_timerindex_connect = scheduler::getInstance().add_timer(10000, boost::bind(&net_client::retry_connect, this, _1));
		}
			break;
		case faith::net::tcp_client::e_ci_addr_resovle_successed:
			break;
		case faith::net::tcp_client::e_ci_connection_successed:
		{
			m_server_status = e_serverstatus_working;

			req_login req;
			req.server_info.server_type = net_server_mgr::getInstance().get_server_type();
			req.server_info.server_index = net_server_mgr::getInstance().get_server_index();
			xstring ip = net_server_mgr::getInstance().get_server_ip();
			if (m_server_info.server_type == e_server_type_gate)
			{
				ip = init_unit::get_host_ip();
			}
			memcpy(req.server_info.ip_addr, ip.c_str(), ip.size());
			req.server_info.port = net_server_mgr::getInstance().get_server_port();
			send_message(&req, sizeof(req));
			m_onconnect_handler(this);
		}
			break;
		default:
			break;
		}
	}

	void net_client::on_conn_closed( uint32 connindex )
	{
		if (connindex >= e_server_type_max)
		{
			return;
		}
		m_onclose_handler(this);
		m_server_status = e_serverstatus_initialized;
		m_timerindex_connect = scheduler::getInstance().add_timer(10000, boost::bind(&net_client::retry_connect, this, _1));
	}

	void net_client::on_data_received( uint32 connindex,const void *data_ptr,size_t data_len )
	{
		m_onrecved_handler(this, data_ptr, data_len);
	}

	uint32 net_client::connect_to()
	{
		xstring ip = m_server_info.ip_addr;
		xstring port = boost::lexical_cast<xstring>(m_server_info.port);
		uint32 connindex = net::tcp_client::get_instance().connect_to(m_conn_index,
			ip, port,
			boost::bind(&net_client::on_conn_status,this,_1,_2,_3),
			boost::bind(&net_client::on_conn_closed,this,_1),
			boost::bind(&net_client::on_data_received,this,_1,_2,_3)
			);
		m_server_status = e_serverstatus_initialized;
		CONSOLE_INFO(" server_type = {} ipaddr = {} port_name = {} ci = {}", (int32)m_server_info.server_type, m_server_info.ip_addr, m_server_info.port, m_conn_index);
		return true;
	} 
	bool net_client::start(const s_server_info& server_info,
		client_on_connect_handler_type onconnect_handler,
		client_on_closed_handler_type onclose_handler,
		client_on_recved_handler_type onrecved_handler)
	{
		m_onconnect_handler = onconnect_handler;
		m_onclose_handler = onclose_handler;
		m_onrecved_handler = onrecved_handler;
		m_server_info = server_info;
		return true;
	}

	void net_client::stop()
	{ 
		tcp_client::get_instance().disconnect(m_conn_index);
		clear_data();
	}
}
