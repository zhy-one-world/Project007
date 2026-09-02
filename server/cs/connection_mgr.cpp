/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 20:31
  file base: connection_mgr
  file ext: cpp
  author: lidongyang
  
  purpose: 
*********************************************************************/
/************************************************************************/
/*                          	File Include                            */
/************************************************************************/
//#include <winsock2.h>
#include "app/app_server.hpp"
#include "cell_server.hpp"
#include "connection/daemon_client.hpp"
#include "connection_mgr.hpp"
#include "game_cfg/servers_config.h"
#include "internal/internal.hpp"
#include "internal/gate_msg.hpp"
#include "net/message_manager.hpp"
#include "net/net_client_mgr.hpp"
#include "net/scheduler.hpp"
#include "server_log.hpp"
#include "utility/guid_gen.h"
#include "utility/parse_msg.h"
#include <rlog.hpp>

namespace faith
{
	connection_mgr::connection_mgr(void)
	{
		xstring ws_ip = WSCONFIG->internal_host;
		memcpy(m_ws_info.ip_addr, ws_ip.c_str(), ws_ip.size());
		m_ws_info.port = WSCONFIG->internal_port;
		m_ws_info.server_type = e_server_type_ws;
		m_ws_conn_index = tcp_client::invalid_conn_index;

		xstring dp_ip = DPCONFIG->internal_host;
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

	connection_mgr::~connection_mgr(void)
	{
	}

	void connection_mgr::send_to_ws(const void* data_ptr, size_t data_len, int32 server_id)
	{
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (nullptr == pPacket)
		{
			return;
		}
		if (server_id == 0 || cell_server::getInstance().get_is_self_server(server_id))
		{
			net_client_mgr::getInstance().send_message(m_ws_conn_index, data_ptr, data_len);
		}
		else
		{
			static server2gate_to_server msg;
			msg.recv_server_id = server_id;
			msg.recv_server_type = e_server_type_ws;
			msg.send_server_id = cell_server::getInstance().get_server_id();
			msg.send_server_index = net_server_mgr::getInstance().get_server_index();
			msg.send_server_type = e_server_type_cs;
			msg.header = pPacket->wheader;
			msg.dataLen = data_len;
			memcpy(msg.data, data_ptr, data_len);
			net_client_mgr::getInstance().send_message(get_gate_index(), &msg, msg.get_pak_length());
		}
	}
	void connection_mgr::send_to_ws_lua(const char * msg, int32 msg_len, uint32 header, int32 server_id)
	{
		if (msg_len > INTERNAL_SERVER_MAX_PACKET_SIZE || header <= 0)
		{
			CONSOLE_ERROR("send_to_ws_lua_error header :{} len:{}", header ,msg_len);
			return;
		}
		m_s2s_msg.wheader = header;
		memcpy(m_s2s_msg.google_data, msg, msg_len);
		m_s2s_msg.google_data_len = msg_len;
		send_to_ws((void*)&m_s2s_msg, m_s2s_msg.get_packet_len(), server_id);
	}
	void connection_mgr::send_to_ws_lua(google::protobuf::Message * net_pro, uint32 header, int32 server_id)
	{
		packet_s2s *data = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
		if (nullptr == data)
		{
			return;
		}
		send_to_ws(data, data->get_packet_len(), server_id);
	}
	void connection_mgr::send_to_dp(const void* data_ptr, size_t data_len, int32 server_id)
	{
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (nullptr == pPacket)
		{
			return;
		}
		if (server_id == 0 || cell_server::getInstance().get_is_self_server(server_id))
		{
			net_client_mgr::getInstance().send_message(m_dp_conn_index, data_ptr, data_len);
		}
		else
		{
			static server2gate_to_server msg;
			msg.recv_server_id = server_id;
			msg.recv_server_type = e_server_type_dp;
			msg.send_server_id = cell_server::getInstance().get_server_id();
			msg.send_server_index = net_server_mgr::getInstance().get_server_index();
			msg.send_server_type = e_server_type_cs;
			msg.header = pPacket->wheader;
			msg.dataLen = data_len;
			memcpy(msg.data, data_ptr, data_len);
			net_client_mgr::getInstance().send_message(get_gate_index(), &msg, msg.get_pak_length());
		}
	}
	void connection_mgr::send_to_dp_lua(const char* msg, int32 msg_len, uint32 header, int32 server_id)
	{
		if (msg_len > INTERNAL_SERVER_MAX_PACKET_SIZE || header <= 0)
		{
			CONSOLE_ERROR("send_to_ws_lua_error header :{} len:{}", header, msg_len);
			return;
		}
		m_s2s_msg.wheader = header;
		memcpy(m_s2s_msg.google_data, msg, msg_len);
		m_s2s_msg.google_data_len = msg_len;
		send_to_dp((void*)&m_s2s_msg, m_s2s_msg.get_packet_len(), server_id);
	
	}
	void connection_mgr::send_to_dp_lua(google::protobuf::Message* net_pro, uint32 header, int32 server_id)
	{
		packet_s2s *data = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
		if (nullptr == data)
		{
			return;
		}
		send_to_dp(data, data->get_packet_len(), server_id);
	}


	void connection_mgr::on_conn_status(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_ws:
		{
			m_ws_conn_index = faith_client_ptr->get_array_index();
			cell_server::getInstance().start();
			start_gate();
		}
		break;
		case e_server_type_dp:
		{
			m_dp_conn_index = faith_client_ptr->get_array_index();
			server_log::init_log_db_sender(boost::bind(&connection_mgr::send_to_dp, &connection_mgr::getInstance(), _1, _2, _3));
		}
		break;
		case e_server_type_gate:
		{
			m_gate_conn_array.push_back(faith_client_ptr->get_array_index());
			server2gate_req_register msg;
			msg.game_info.server_id = cell_server::getInstance().get_server_id();
			msg.server_type = e_server_type_cs;
			net_client_mgr::getInstance().send_message(faith_client_ptr->get_array_index(), &msg, sizeof(msg));
		}
		break;
		default:
			break;
		}
	}

	void connection_mgr::on_conn_closed(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_ws:
		{
			_RLOG_(MINFO, "on_conn_closed_ws");
			//req_stop server_stop_msg;
			//server_stop_msg.server_type = e_server_type_dp;
			//connection_mgr::getInstance().send_to_dp(&server_stop_msg, sizeof(server_stop_msg));

			m_ws_conn_index = tcp_client::invalid_conn_index;
			daemon_client::getInstance().set_server_close(true);
		}
		break;
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

	bool connection_mgr::start()
	{
		net_client_mgr::getInstance().start(m_ws_info,
			boost::bind(&connection_mgr::on_conn_status, &connection_mgr::getInstance(), _1),
			boost::bind(&connection_mgr::on_conn_closed, &connection_mgr::getInstance(), _1)
		); 
		net_client_mgr::getInstance().start(m_dp_info,
			boost::bind(&connection_mgr::on_conn_status, &connection_mgr::getInstance(), _1),
			boost::bind(&connection_mgr::on_conn_closed, &connection_mgr::getInstance(), _1)
		);
		return true;
	}
	bool connection_mgr::start_gate()
	{
		net_client_mgr::getInstance().start(m_gate_info,
			boost::bind(&connection_mgr::on_conn_status, &connection_mgr::getInstance(), _1),
			boost::bind(&connection_mgr::on_conn_closed, &connection_mgr::getInstance(), _1)
		);
		return true;
	}
	bool connection_mgr::start_gate_new(const s_server_info& gate_info)
	{
		net_client_mgr::getInstance().start(gate_info,
			boost::bind(&connection_mgr::on_conn_status, &connection_mgr::getInstance(), _1),
			boost::bind(&connection_mgr::on_conn_closed, &connection_mgr::getInstance(), _1)
		);
		return true;
	}

	void connection_mgr::stop()
	{ 
		net_client_mgr::getInstance().stop();
	}
	void connection_mgr::internal_rep_login(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const rep_login* pPacket= static_cast<const rep_login*>(data_ptr);
		if ( NULL == pPacket )
		{
			return;
		}

		if(connindex == m_ws_conn_index)
		{
			guid_gen::set_server_id(server_uid_level_num + cell_server::getInstance().get_server_id());
			cell_server::getInstance().set_open_time(pPacket->open_time);
		}
	}

	void connection_mgr::internal_gate_msg(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const server2gate_to_server* packet = static_cast<const server2gate_to_server*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		message_manager::getInstance().on_data_received(connindex, packet->data, packet->dataLen);
	}	
	void connection_mgr::gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message("gate2ws_rep_register  data_len:{}",  data_len));
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

	bool connection_mgr::broadcast_pak(const void* data_ptr, size_t data_len)
	{
		net_client_mgr::getInstance().broadcast_pak(data_ptr, data_len);
		return true;
	}

	bool connection_mgr::broadcast_pak(const void* data_ptr, size_t data_len, e_server_type server_type)
	{
		net_client_mgr::getInstance().broadcast_pak(data_ptr, data_len, server_type);
		return true;
	}
	uint32 connection_mgr::get_gate_index()
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
