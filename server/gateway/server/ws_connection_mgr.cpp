#include "ws_connection_mgr.hpp"

#include <boost/bind.hpp>
#include <rlog.hpp>

#include "game_cfg/servers_config.h"

namespace faith
{
	bool ws_connection_mgr::init()
	{
		m_listen_port = GATEWAYCONFIG->internal_port > 0
			? GATEWAYCONFIG->internal_port
			: (GATEWAYCONFIG->external_port > 0 ? GATEWAYCONFIG->external_port : 2200);

		if (!GATEWAYCONFIG->internal_host.empty() &&
			GATEWAYCONFIG->internal_host != "127.0.0.1")
		{
			m_listen_host = GATEWAYCONFIG->internal_host;
		}
		else
		{
			m_listen_host = "0.0.0.0";
		}

		_RLOG_(MINFO, "ws_connection_mgr init listen="
			<< m_listen_host << ":" << m_listen_port);

		if (!net_server_mgr::getInstance().init(
				e_server_type_gateway,
				m_listen_host,
				m_listen_port,
				GATEWAY_CLIENT_SEND_BUFF_SIZE,
				GATEWAY_CLIENT_RECV_BUFF_SIZE,
				INTERNAL_SERVER_MAX_PACKET_SIZE,
				SERVER_WS_COUNT + SERVER_CS_COUNT,
				SERVER_WS_COUNT + SERVER_CS_COUNT,
				boost::bind(&ws_connection_mgr::on_peer_closed, this, _1)))
		{
			_RLOG_(MERROR, "ws_connection_mgr net_server_mgr::init failed");
			return false;
		}
		return true;
	}

	bool ws_connection_mgr::start()
	{
		if (!net_server_mgr::getInstance().start())
		{
			_RLOG_(MERROR, "ws_connection_mgr net_server_mgr::start failed");
			return false;
		}
		_RLOG_(MINFO, "ws_connection_mgr started, waiting for WS");
		return true;
	}

	void ws_connection_mgr::stop()
	{
		net_server_mgr::getInstance().stop();
		m_ws_conn_index = -1;
	}

	bool ws_connection_mgr::is_ws_connected() const
	{
		return m_ws_conn_index.load() >= 0;
	}

	void ws_connection_mgr::send_to_ws(const void* data_ptr, size_t data_len)
	{
		const int32 conn = m_ws_conn_index.load();
		if (conn < 0 || data_ptr == nullptr || data_len == 0)
		{
			return;
		}
		net_server_mgr::getInstance().send_message(
			data_ptr, data_len, conn, e_server_type_ws);
	}

	void ws_connection_mgr::on_ws_login(uint32 conn_index, const s_server_info& info)
	{
		(void)info;
		m_ws_conn_index = static_cast<int32>(conn_index);
		_RLOG_(MINFO, "ws_connection_mgr WS logged in, conn_index=" << conn_index);
	}

	void ws_connection_mgr::on_ws_closed(const net_server* peer)
	{
		if (peer == nullptr)
		{
			return;
		}
		const int32 conn = static_cast<int32>(peer->get_conn_index());
		int32 expected = conn;
		m_ws_conn_index.compare_exchange_strong(expected, -1);
		_RLOG_(MWARN, "ws_connection_mgr WS closed, conn_index=" << conn);
	}

	void ws_connection_mgr::on_peer_closed(const net_server* peer)
	{
		if (peer == nullptr)
		{
			return;
		}
		if (peer->get_server_type() == e_server_type_ws)
		{
			on_ws_closed(peer);
		}
	}
}
