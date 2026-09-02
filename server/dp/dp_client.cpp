/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 11:13
	
	File Name :	dp_client.cpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "dp_client.hpp"
#include <internal.hpp>
#include <gate_msg.hpp>
#include "game_cfg/servers_config.h"
#include "Logic/time_def.hpp"
#include "time.hpp"
#include "server_log.hpp"
#include "dbproxy_service.hpp"
#include "net/message_manager.hpp"
#include <rlog.hpp>

namespace faith
{
	//////////////////////////////////////////////////////////////////////////
	//	Class Implement
	//////////////////////////////////////////////////////////////////////////

	dp_client::dp_client(void)
	{
		xstring gate_ip = GATECONFIG->internal_host;
		memcpy(m_gate_info.ip_addr, gate_ip.c_str(), gate_ip.size());
		m_gate_info.port = GATECONFIG->internal_port;
		m_gate_info.server_type = e_server_type_gate;

		m_begin_message_index = DP_NEED_SERVER_COUNT;
		m_gate_message_map.clear();
	}

	dp_client::~dp_client(void)
	{
	}
	void dp_client::tick(const int64& new_time)
	{
		for (gate_message_map_it it = m_gate_message_map.begin(); it != m_gate_message_map.end();)
		{
			gate_message_map_it temp_it = it++;
			if (temp_it->second.msg_tick < new_time)
			{
				m_gate_message_map.erase(temp_it);
			}
		}
	}
	bool dp_client::start()
	{
		net_client_mgr::getInstance().start(m_gate_info,
			boost::bind(&dp_client::on_conn_status, &dp_client::getInstance(), _1),
			boost::bind(&dp_client::on_conn_closed, &dp_client::getInstance(), _1)
		);
		return true;
	}
	bool dp_client::start_gate_new(const s_server_info& gate_info)
	{
		net_client_mgr::getInstance().start(gate_info,
			boost::bind(&dp_client::on_conn_status, &dp_client::getInstance(), _1),
			boost::bind(&dp_client::on_conn_closed, &dp_client::getInstance(), _1)
		);
		return true;
	}
	void dp_client::stop()
	{
		net_client_mgr::getInstance().stop();
	}
	void dp_client::on_conn_status(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_gate:
		{
			server2gate_req_register msg;
			msg.game_info.server_id = dbproxy_service::getInstance().get_server_id();
			msg.server_type = e_server_type_dp;
			net_client_mgr::getInstance().send_message(faith_client_ptr->get_array_index(), &msg, sizeof(msg));
		}
		break;
		default:
			break;
		}
	}

	void dp_client::on_conn_closed(const net_client* faith_client_ptr)
	{
		m_gate_message_map.clear();
		if (faith_client_ptr->get_server_type() == e_server_type_gate)
		{
			//net_client_mgr::getInstance().stop(faith_client_ptr->get_array_index());
		}
	}
	void dp_client::send_to_gate(const void* data_ptr, size_t data_len, uint32 conn_index)
	{
		gate_message_map_it it = m_gate_message_map.find(conn_index);
		if (it == m_gate_message_map.end())
		{
			return;
		}
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (nullptr == pPacket)
		{
			return;
		}
		s_gate_message& gate_message_ref = it->second;
		static server2gate_to_server msg;
		msg.recv_server_id = gate_message_ref.send_server_id;
		msg.recv_server_index = gate_message_ref.send_server_index;
		msg.recv_server_type = gate_message_ref.send_server_type;
		msg.send_server_id = dbproxy_service::getInstance().get_server_id();
		msg.send_server_index = net_server_mgr::getInstance().get_server_index();
		msg.send_server_type = e_server_type_dp;
		msg.header = pPacket->wheader;
		msg.dataLen = data_len;
		memcpy(msg.data, data_ptr, data_len);
		net_client_mgr::getInstance().send_message(gate_message_ref.gate_conn_index, &msg, msg.get_pak_length());
	}
	void dp_client::internal_gate_msg(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_to_server* packet = static_cast<const server2gate_to_server*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		s_gate_message gate_msg;
		gate_msg.msg_tick = utility::get_tick_count() + minute_tick_time;
		gate_msg.send_server_type = packet->send_server_type;
		gate_msg.send_server_id = packet->send_server_id;
		gate_msg.send_server_index = packet->send_server_index;
		gate_msg.gate_conn_index = connindex;
		m_gate_message_map[++m_begin_message_index] = gate_msg;
		message_manager::getInstance().on_data_received(m_begin_message_index, packet->data, packet->dataLen);
		if (m_begin_message_index >= 0X0FFFFFFF)
		{
			m_begin_message_index = DP_NEED_SERVER_COUNT;
		}
	}	
	void dp_client::gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message(" gate2ws_rep_register  data_len : {}",  data_len));
		if (data_len != sizeof(gate2gate_req_new))
		{
			return;
		}
		const gate2gate_req_new* packet = (const gate2gate_req_new*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		start_gate_new(packet->server_info);
	}
}
