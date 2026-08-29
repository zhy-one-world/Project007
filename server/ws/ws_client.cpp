/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 11:13
	
	File Name :	ws_client.cpp
	
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
#include "ws_client.hpp"
#include <gate_msg.hpp>
#include "game_cfg/servers_config.h"
#include "net/net_client_mgr.hpp"
#include "server_log.hpp"
#include "world_server.hpp"
#include "cross_group/gate_proxy.hpp"
#include "net/message_manager.hpp"
#include "utility/serialize_msg.h"

namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Implement
	//
	//////////////////////////////////////////////////////////////////////////

	ws_client::ws_client(void)
	{
		m_dp_info.clear_data();
		xstring dp_ip = DPCONFIG->internal_host;
		std::cout << dp_ip.size() << std::endl;
		memcpy(m_dp_info.ip_addr, dp_ip.c_str(), dp_ip.size());
		m_dp_info.port = DPCONFIG->internal_port;
		m_dp_info.server_type = e_server_type_dp;
		m_dp_conn_index = tcp_client::invalid_conn_index;

		xstring gate_ip = GATECONFIG->internal_host;
		memcpy(m_gate_info.ip_addr, gate_ip.c_str(), gate_ip.size());
		m_gate_info.port = GATECONFIG->internal_port;
		m_gate_info.server_type = e_server_type_gate;
		m_gate_index = 0;
		m_gate_conn_array.clear();
	}

	ws_client::~ws_client(void)
	{
	}

	bool ws_client::start()
	{
		net_client_mgr::getInstance().start(m_dp_info,
			boost::bind(&ws_client::on_conn_status, &ws_client::getInstance(), _1),
			boost::bind(&ws_client::on_conn_closed, &ws_client::getInstance(), _1)
		);
		return true;
	}
	bool ws_client::start_gate()
	{
		net_client_mgr::getInstance().start(m_gate_info,
			boost::bind(&ws_client::on_conn_status, &ws_client::getInstance(), _1),
			boost::bind(&ws_client::on_conn_closed, &ws_client::getInstance(), _1)
		);
		return true;
	}
	bool ws_client::start_gate_new(const s_server_info& gate_info)
	{
		net_client_mgr::getInstance().start(gate_info,
			boost::bind(&ws_client::on_conn_status, &ws_client::getInstance(), _1),
			boost::bind(&ws_client::on_conn_closed, &ws_client::getInstance(), _1)
		);
		return true;
	}
	void ws_client::stop()
	{
		net_client_mgr::getInstance().stop();
	}
	void ws_client::on_conn_status(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_dp:
		{
			m_dp_conn_index = faith_client_ptr->get_array_index();
			server_log::init_log_db_sender(boost::bind(&ws_client::send_to_dp, &ws_client::getInstance(), _1, _2, _3));
		}
		break;
		case e_server_type_gate:
		{
			m_gate_conn_array.push_back(faith_client_ptr->get_array_index());
			send_cross_msg(0, faith_client_ptr->get_array_index());
			gate_proxy::getInstance().set_status(E_GATE_REGISTERED);
			world_server::getInstance().gate_server_func_when_game_info_load_end();
			world_server::getInstance().send_to_gate_with_flag_load_end();
		}
		break;
		default:
			break;
		}
	}
	void ws_client::internal_gate_msg(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_to_server* packet = static_cast<const server2gate_to_server*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		message_manager::getInstance().on_data_received(connindex, packet->data, packet->dataLen);
	}
	void ws_client::send_cross_msg(int32 recv_server_id, uint32 gate_index)
	{
		server2gate_req_register msg;
		xstring& out_ip = world_server::getInstance().get_out_ip();
		memcpy(msg.game_info.out_ip, out_ip.c_str(), out_ip.size());
		msg.game_info.out_port = world_server::getInstance().get_out_port();
		xstring& server_name = world_server::getInstance().get_server_name();
		memcpy(msg.game_info.server_name, server_name.c_str(), server_name.size());
		msg.game_info.server_id = world_server::getInstance().get_server_id();
		msg.game_info.cross_id = world_server::getInstance().get_cross_id();
		msg.server_type = e_server_type_ws;
		msg.recv_server_id = recv_server_id;
		if (gate_index == tcp_client::invalid_conn_index)
		{
			net_client_mgr::getInstance().send_message(get_gate_index(), &msg, sizeof(msg));
		}
		else
		{
			net_client_mgr::getInstance().send_message(gate_index, &msg, sizeof(msg));
		}
	}
	void ws_client::on_conn_closed(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_dp:
		{
			m_dp_conn_index = tcp_client::invalid_conn_index;
		}
		break;
		case e_server_type_gate:
		{
			for (std::vector<uint32>::iterator it = m_gate_conn_array.begin(); it != m_gate_conn_array.end(); ++it)
			{
				if (*it == faith_client_ptr->get_array_index())
				{
					m_gate_conn_array.erase(it);
					break;
				}
			}
			//net_client_mgr::getInstance().stop(faith_client_ptr->get_array_index());
		}
		break;
		default:
			break;
		}
	}
	void ws_client::send_to_dp( const void* data_ptr,size_t data_len, int32 server_id)
	{
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (nullptr == pPacket)
		{
			return;
		}
		if (server_id == 0 || server_id == world_server::getInstance().get_server_id())
		{
			net_client_mgr::getInstance().send_message(m_dp_conn_index, data_ptr, data_len);
		}
		else
		{
			static server2gate_to_server msg;
			msg.recv_server_id = server_id;
			msg.recv_server_type = e_server_type_dp;
			msg.send_server_id = world_server::getInstance().get_server_id();
			msg.send_server_index = net_server_mgr::getInstance().get_server_index();
			msg.send_server_type = e_server_type_ws;
			msg.header = pPacket->wheader;
			msg.dataLen = data_len;
			memcpy(msg.data, data_ptr, data_len);
			net_client_mgr::getInstance().send_message(get_gate_index(), &msg, msg.get_pak_length());
		}
	}

	void ws_client::send_to_dp_lua(const char* msg, int32 msg_len, uint32 header, int32 server_id)
	{
		if (msg_len > INTERNAL_SERVER_MAX_PACKET_SIZE || header <= 0)
		{
			CONSOLE_INFO("header:{} len:{}", header, msg_len);
			return;
		}
		m_ws2dp_msg.wheader = header;
		memcpy(m_ws2dp_msg.google_data, msg, msg_len);
		m_ws2dp_msg.google_data_len = msg_len;
		send_to_dp((void*)&m_ws2dp_msg, m_ws2dp_msg.get_packet_len(), server_id);
	}

	void ws_client::send_to_dp_new(google::protobuf::Message* proto_ptr, uint32 header, int32 server_id)
	{
		serialize_msg::get_instance().set_serialize_msg_new(m_ws2dp_msg, proto_ptr, header);
		send_to_dp((void*)&m_ws2dp_msg, m_ws2dp_msg.get_packet_len(), server_id);
	}

	void ws_client::only_send_to_gate(const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr || data_len == 0)
		{
			return;
		}
		net_client_mgr::getInstance().send_message(get_gate_index(), data_ptr, data_len);
	}
	void ws_client::send_to_gate(const void* data_ptr, size_t data_len, int32 server_id)
	{
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (nullptr == pPacket)
		{
			return;
		}
		//server2gate_to_server msg;
		if (server_id == 0)
		{
			m_msg.recv_server_id = world_server::getInstance().get_cross_id();
			m_msg.recv_server_index = 0;
		}
		else if (server_id == -1)
		{
			m_msg.recv_server_id = 0;
			m_msg.recv_server_index = -1;
		}
		else
		{
			m_msg.recv_server_id = server_id;
			m_msg.recv_server_index = 1;
		}
		m_msg.recv_server_type = e_server_type_gate;
		m_msg.send_server_id = world_server::getInstance().get_server_id();
		m_msg.send_server_index = net_server_mgr::getInstance().get_server_index();
		m_msg.send_server_type = e_server_type_ws;
		m_msg.header = pPacket->wheader;
		m_msg.dataLen = data_len;
		memcpy(m_msg.data, data_ptr, data_len);
		net_client_mgr::getInstance().send_message(get_gate_index(), &m_msg, m_msg.get_pak_length());
	}
	void ws_client::send_to_gate(const google::protobuf::Message* proto_ptr, uint32 header, int32 server_id)
	{
		//static server2gate_to_server msg;
		if (server_id == 0)
		{
			m_msg.recv_server_id = world_server::getInstance().get_cross_id();
			m_msg.recv_server_index = 0;
		}
		else if (server_id == -1)
		{
			m_msg.recv_server_id = 0;
			m_msg.recv_server_index = -1;
		}
		else
		{
			m_msg.recv_server_id = server_id;
			m_msg.recv_server_index = 1;
		}
		m_msg.recv_server_type = e_server_type_gate;
		m_msg.send_server_id = world_server::getInstance().get_server_id();
		m_msg.send_server_index = net_server_mgr::getInstance().get_server_index();
		m_msg.send_server_type = e_server_type_ws;
		m_msg.header = header;
		bool ret = proto_ptr->SerializeToArray((void*)m_msg.data, sizeof(m_msg.data));
		if (!ret)
		{
			return;
		}
		m_msg.dataLen = proto_ptr->ByteSize();
		net_client_mgr::getInstance().send_message(get_gate_index(), &m_msg, m_msg.get_pak_length());
	}
	uint32 ws_client::get_gate_index()
	{
		if (m_gate_conn_array.empty())
		{
			return tcp_client::invalid_conn_index;
		}
		m_gate_index--;
		if (m_gate_index < 0)
		{
			m_gate_index = m_gate_conn_array.size() - 1;
		}
		if (m_gate_index >= 0)
		{
			return m_gate_conn_array[m_gate_index];
		}
		else
		{
			return tcp_client::invalid_conn_index;
		}
	}
}
