/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019/07/03
	
	File Name :	net_server_mgr.cpp
	
	Author : zhy
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
//#include <winsock2.h>
#include "net_server_mgr.hpp"
#include "server_log.hpp"
#include "message_manager.hpp"
#include "utility/random.h"
#include <rlog.hpp>

namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//	Class Implement
	//////////////////////////////////////////////////////////////////////////

	net_server_mgr::net_server_mgr(void)
	:m_tcpserver_ptr(NULL)
	{
		m_gate_array.clear();
		m_gate_count = 0;
		m_server_type = e_server_type_invalid;
		m_server_index = 0;
		m_server_port = 0;
		m_conn_map = nullptr;
		m_conn_num = 0;
	}
	net_server_mgr::~net_server_mgr(void)
	{
		delete[] m_conn_map;
		m_conn_map = nullptr;
		m_conn_num = 0;
	}
	bool net_server_mgr::init(e_server_type server_type, xstring server_ip, int32 server_port,
		uint32 send_buf_size, uint32 recv_buf_size, uint32 max_packet_size, uint32 server_num, uint32 init_num,
		server_on_closed_handler_type onclosed_handler)
	{
		m_tcpserver_ptr = new tcp_server(
			boost::bind(&net_server_mgr::handler_serverstatus,this,_1),
			boost::bind(&net_server_mgr::handler_onconnected,this,_1),
			boost::bind(&net_server_mgr::handler_onclose,this,_1),
			boost::bind(&net_server_mgr::handler_onrecv,this,_1,_2,_3),
			server_ip,server_port
			);

		if(!m_tcpserver_ptr)
			return false;

		if (!m_tcpserver_ptr->set_option(tcp_server::options::send_buffer_size(send_buf_size))) return false;
		if (!m_tcpserver_ptr->set_option(tcp_server::options::recv_buffer_size(recv_buf_size))) return false;
		if (!m_tcpserver_ptr->set_option(tcp_server::options::max_packet_size(max_packet_size))) return false;
		if (!m_tcpserver_ptr->set_option(tcp_server::options::connections_num_limit(server_num))) return false;

		m_server_type = server_type;
		m_server_ip = server_ip;
		m_server_port = server_port;
		m_external_onclose_handler = onclosed_handler;
		m_conn_num = server_num;
		m_conn_map = new net_server[server_num];
		for (int32 i = 0; i < m_conn_num; ++i)
		{
			m_conn_map[i].set_conn_index(i);
		}
		return true;
	}
	bool net_server_mgr::start()
	{
		if (!m_tcpserver_ptr->start())
		{
			return false;
		}
		return true;
	}
	std::vector<uint32> net_server_mgr::get_server_conn_array(e_server_type server_type)
	{
		std::vector<uint32> res;
		for (int32 i = 0; i < m_conn_num; ++i)
		{
			if (m_conn_map[i].get_server_type() == server_type)
			{
				res.push_back(m_conn_map[i].get_conn_index());
			}
		}
		return res;
	}
	int32 net_server_mgr::get_server_count(e_server_type server_type)
	{
		int32 server_num = 0;
		for (int32 i= 0; i< m_conn_num; ++i)
		{
			if (m_conn_map[i].get_server_type() == server_type)
			{
				server_num++;
			}
		}
		return server_num;
	}
	net_server* net_server_mgr::get_peer_by_conn_index(uint32 conn_index)
	{
		if (conn_index >= m_conn_num)
		{
			return nullptr;
		}
		return &(m_conn_map[conn_index]);
	}
	net_server* net_server_mgr::get_peer_by_type(e_server_type server_type)
	{
		if (server_type <= e_server_type_invalid || server_type >= e_server_type_max)
		{
			return nullptr;
		}
		for (int32 i = 0; i < m_conn_num; ++i)
		{
			if (m_conn_map[i].get_server_type() == server_type)
			{
				return &(m_conn_map[i]);
			}
		}
		return nullptr;
	}

	bool net_server_mgr::send_message_by_index(const void* data_ptr, size_t data_len, int32 conn_index)
	{
		if (nullptr == data_ptr || data_len == 0)
		{
			return false;
		}
		if (conn_index < 0 || conn_index >= m_conn_num)
		{
			return false;
		}
		m_tcpserver_ptr->send(conn_index, data_ptr, data_len);
		return true;
	}
	void net_server_mgr::send_message_by_type(const void* data_ptr, size_t data_len, e_server_type server_type)
	{
		if (server_type >= e_server_type_max)
		{
			return;
		}
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			if (server_type == e_server_type_invalid || m_conn_map[i].get_server_type() == server_type)
			{
				m_tcpserver_ptr->send(i, data_ptr, data_len);
			}
		}
	}
	bool net_server_mgr::send_by_gate(const void* data_ptr, size_t data_len)
	{
		if (nullptr == m_tcpserver_ptr)
		{
			const packet_c2s_s2c* packet_ptr = (packet_c2s_s2c*)data_ptr;
			_RLOG_(MINFO, ::faith::log_detail::format_message("connection_server::send_by_gate tcp_server is null  header = {}",  packet_ptr->wheader));
			return false;
		}
		if (m_gate_count <= 0)
		{
			const packet_c2s_s2c* packet_ptr = (packet_c2s_s2c*)data_ptr;
			_RLOG_(MINFO, ::faith::log_detail::format_message("connection_server::send_by_gate m_gate_count is 0 m_gate_count = {} header = {}",  m_gate_count,  packet_ptr->wheader));
			return false;
		}
		int32 gate_rand = random_gen::get_random(0, m_gate_count);
		m_tcpserver_ptr->send(m_gate_array[gate_rand], data_ptr, data_len);
		return true;
	}
	void net_server_mgr::send_message(const void* data_ptr, size_t data_len, int32 conn_index, e_server_type server_type)
	{
		packet_base* pPacket = (packet_base*)data_ptr;
		//CONSOLE_INFO("wheader:{}, data_len:{}", pPacket->wheader, data_len);
		if (conn_index >= m_conn_num)
		{
			return;
		}
		if (server_type >= e_server_type_max)
		{
			return;
		}
		if (conn_index < 0)
		{
			send_message_by_type(data_ptr, data_len, server_type);
		}
		else
		{
			send_message_by_index(data_ptr, data_len, conn_index);
		}
	}

	void net_server_mgr::send_message_out(const void* data_ptr, size_t data_len, int32 conn_index, e_server_type server_type)
	{
		if (conn_index >= m_conn_num)
		{
			return;
		}
		if (server_type >= e_server_type_max)
		{
			return;
		}
		if (server_type == e_server_type_invalid)
		{
			for (int32 i = 0; i < m_conn_num; ++i)
			{
				if (i != conn_index)
				{
					m_tcpserver_ptr->send(i, data_ptr, data_len);
				}
			}
		}
		else
		{
			for (int32 i = 0; i < m_conn_num; ++i)
			{
				if (i != conn_index && m_conn_map[i].get_server_type() != server_type)
				{
					m_tcpserver_ptr->send(i, data_ptr, data_len);
				}
			}
		}
	}

	void net_server_mgr::handler_serverstatus(tcp_server::e_server_status_type status)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message(" status = {}",  (int32)status));
		if(status == tcp_server::e_ss_all_connection_closed)
		{
			delete m_tcpserver_ptr;
			m_tcpserver_ptr = NULL;
		}
	}

	void net_server_mgr::handler_onconnected(uint32 conn_index)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message(" conn_index = {}",  conn_index));
		if (conn_index >= m_conn_num)
		{
			return;
		}
		net_server& faith_server_ref = m_conn_map[conn_index];
		faith_server_ref.set_server_status(e_serverstatus_initialized);
	}

	void net_server_mgr::handler_onclose(uint32 conn_index)
	{
		if (conn_index >= m_conn_num)
		{
			return;
		}
		net_server& faith_server_ref = m_conn_map[conn_index];

		if (faith_server_ref.get_server_status() == e_serverstatus_created)
		{
			return;
		}
		_RLOG_(MINFO, ::faith::log_detail::format_message(" conn_index = {} type = {}",  conn_index,  (int32)faith_server_ref.get_server_type()));
		m_external_onclose_handler(&faith_server_ref);
		faith_server_ref.clear_data();
	}

	void net_server_mgr::handler_onrecv(uint32 conn_index,const void* data_ptr,size_t data_len)
	{
		if (conn_index >= m_conn_num)
		{
			return;
		}
		net_server& faith_server_ref = m_conn_map[conn_index];

		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (NULL == pPacket)
		{
			return;
		}
		if (pPacket->wheader < 0 || pPacket->wheader >= e_msg_base_max)
		{
			return;
		}
		message_manager::getInstance().on_data_received(conn_index, data_ptr, data_len);
	}
}
