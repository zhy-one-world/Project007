/********************************************************************
  created: 2019/07/03
  file base: server_client_mgr
  file ext: cpp
  author: zhy
  
  purpose: 
*********************************************************************/
/************************************************************************/
/*                          	File Include                            */
/************************************************************************/
//#include <winsock2.h>
#include <net/scheduler.hpp>
#include "net/net_client_mgr.hpp"
#include "app/app_server.hpp"
#include "net_client_mgr.hpp"
#include "message_manager.hpp"
#include "game_cfg/servers_config.h"
#include "utility/guid_gen.h"
#include "server_log.hpp"
#include "../connection/daemon_client.hpp"

namespace faith
{

	net_client_mgr::net_client_mgr(void)
	{
		m_client_connmap = 0;
		m_client_num = 0;
		m_server_type = e_server_type_invalid;
	}

	net_client_mgr::~net_client_mgr(void)
	{
		delete[] m_client_connmap;
		m_client_connmap = nullptr;
		m_client_num = 0;
	}
	net_client* net_client_mgr::get_empty_client()
	{
		for (int32 i = 0; i < m_client_num; ++i)
		{
			if (m_client_connmap[i].get_server_status() == e_serverstatus_created)
			{
				return &(m_client_connmap[i]);
			}
		}
		return nullptr;
	}
	bool net_client_mgr::set_netpara_option(uint32 send_buf_size, uint32 recv_buf_size, uint32 max_packet_size, uint32 client_num)
	{
		faith::net::tcp_client& client = faith::net::tcp_client::get_instance();

		if (!client.set_option(faith::net::tcp_client::options::send_buffer_size(send_buf_size))) return false;
		if (!client.set_option(faith::net::tcp_client::options::recv_buffer_size(recv_buf_size))) return false;
		if (!client.set_option(faith::net::tcp_client::options::max_packet_size(max_packet_size))) return false;
		m_client_connmap = new net_client[client_num];
		m_client_num = client_num;
		for (int32 i =0; i < m_client_num; ++i)
		{
			m_client_connmap[i].set_array_index(i);
		}
		return true;
	}
	void net_client_mgr::on_data_received(const net_client* faith_client_ptr, const void *data_ptr, size_t data_len)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		if (faith_client_ptr->get_server_type() == e_server_type_deamon)
		{
			daemon_client::getInstance().on_data_received(faith_client_ptr, data_ptr, data_len);
		}
		else
		{
			message_manager::getInstance().on_data_received(faith_client_ptr->get_array_index(), data_ptr, data_len);
		}
	}
	bool net_client_mgr::start(const s_server_info& server_info,
		client_on_connect_handler_type onconnect_handler,
		client_on_closed_handler_type onclose_handler)
	{
		for (int32 i = 0; i < m_client_num; ++i)
		{
			const s_server_info& temp_server_info = m_client_connmap[i].get_server_info();
			if (strcmp(server_info.ip_addr, temp_server_info.ip_addr) == 0 && server_info.port == temp_server_info.port)
			{
				return m_client_connmap[i].get_array_index();
			}
		}
		net_client* server_client_ptr = get_empty_client();
		if (nullptr == server_client_ptr)
		{
			return false;
		}
		server_client_ptr->start(server_info,
			onconnect_handler, 
			onclose_handler,
			boost::bind(&net_client_mgr::on_data_received, this, _1, _2, _3)
		);
		server_client_ptr->connect_to();
		return server_client_ptr->get_array_index();
	}

	void net_client_mgr::stop()
	{ 
		for (int32 i =0; i < m_client_num; ++i)
		{
			m_client_connmap[i].stop();
		}
	}
	void net_client_mgr::stop(uint32 conn_index)
	{
		if (conn_index >= m_client_num)
		{
			return;
		}
		m_client_connmap[conn_index].stop();
	}
	int32 net_client_mgr::get_server_count(e_server_type server_type)
	{
		int32 server_num = 0;
		for (int32 i = 0; i < m_client_num; ++i)
		{
			if (m_client_connmap[i].get_server_type() == server_type && m_client_connmap[i].get_server_status() > e_serverstatus_created)
			{
				server_num++;
			}
		}
		return server_num;
	}
	net_client* net_client_mgr::get_client_by_index(uint32 conn_index)
	{
		if (conn_index >= m_client_num)
		{
			return nullptr;
		}
		return &(m_client_connmap[conn_index]);
	}
	void net_client_mgr::send_message(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (conn_index >= m_client_num)
		{
			return;
		}
		m_client_connmap[conn_index].send_message(data_ptr, data_len);
	}
	void net_client_mgr::broadcast_pak(const void* data_ptr, size_t data_len)
	{
		for (int32 i = 0; i < m_client_num; ++i)
		{
			net_client& server_client_ref = m_client_connmap[i];
			if (server_client_ref.get_server_status() > e_serverstatus_created) // 只广播给已登录的Peer
				server_client_ref.send_message(data_ptr, data_len);
		}
	}

	void net_client_mgr::broadcast_pak(const void* data_ptr, size_t data_len, e_server_type server_type)
	{
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			net_client& server_client_ref = m_client_connmap[i];
			if (server_client_ref.get_server_type() == server_type && server_client_ref.get_server_status() > e_serverstatus_created)
				server_client_ref.send_message(data_ptr, data_len);
		}
	}
	void net_client_mgr::broadcast_pak_out(const void* data_ptr, size_t data_len, e_server_type server_type)
	{
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			net_client& server_client_ref = m_client_connmap[i];
			if (server_client_ref.get_server_type() != server_type && server_client_ref.get_server_status() > e_serverstatus_created)
				server_client_ref.send_message(data_ptr, data_len);
		}
	}
	void net_client_mgr::broadcast_pak_out(const void* data_ptr, size_t data_len, int32 conn_index)
	{
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			net_client& server_client_ref = m_client_connmap[i];
			if (server_client_ref.get_array_index() != conn_index && server_client_ref.get_server_status() > e_serverstatus_created)
				server_client_ref.send_message(data_ptr, data_len);
		}
	}

}
