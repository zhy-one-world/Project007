#include "connection/gateway_connection_mgr.hpp"

#include <algorithm>
#include <boost/bind.hpp>
#include <rlog.hpp>

#include "http/http_access_mgr.hpp"
#include "net/net_client.hpp"
#include "net/net_client_mgr.hpp"

namespace faith
{
	bool gateway_connection_mgr::register_and_start(const start_params& params)
	{
		m_on_connected = params.on_connected;
		m_on_closed = params.on_closed;

		http_access_mgr::get_instance().init(false);

		const auto& cc_params = params.registry;
		std::string cc_error;
		_RLOG_(MINFO, "gateway_connection_mgr config_center register begin, type="
			<< cc_params.server_type
			<< " game_id=" << cc_params.game_id
			<< " internal=" << cc_params.internal_host << ":" << cc_params.internal_port);
		if (!config_center_client::getInstance().register_sync(cc_params, cc_error))
		{
			_RLOG_(MERROR, "gateway_connection_mgr register failed: " << cc_error);
			return false;
		}
		config_center_client::getInstance().start_heartbeat();

		const auto& peers = config_center_client::getInstance().peers();
		const config_center_client::peer_endpoint* gateway_peer = nullptr;
		for (const auto& peer : peers)
		{
			if (peer.server_type == "gateway" && peer.game_id == cc_params.game_id)
			{
				gateway_peer = &peer;
				break;
			}
		}
		if (gateway_peer == nullptr)
		{
			_RLOG_(MERROR, "gateway_connection_mgr no gateway peer for game_id="
				<< cc_params.game_id);
			return false;
		}
		if (gateway_peer->internal_host.empty() || gateway_peer->internal_port <= 0)
		{
			_RLOG_(MERROR, "gateway_connection_mgr gateway peer missing internal endpoint");
			return false;
		}

		s_server_info gateway_info;
		gateway_info.clear_data();
		const std::string& gw_host = gateway_peer->internal_host;
		memcpy(gateway_info.ip_addr, gw_host.c_str(),
			std::min(gw_host.size(), sizeof(gateway_info.ip_addr) - 1));
		gateway_info.port = gateway_peer->internal_port;
		gateway_info.server_type = e_server_type_gateway;
		gateway_info.server_index = 0;

		return connect(gateway_info);
	}

	bool gateway_connection_mgr::connect(const s_server_info& gateway_info)
	{
		m_gateway_info = gateway_info;
		m_gateway_info.server_type = e_server_type_gateway;
		_RLOG_(MINFO, "gateway_connection_mgr connecting "
			<< m_gateway_info.ip_addr << ":" << m_gateway_info.port);
		net_client_mgr::getInstance().start(m_gateway_info,
			boost::bind(&gateway_connection_mgr::on_conn_status, this, _1),
			boost::bind(&gateway_connection_mgr::on_conn_closed, this, _1));
		return true;
	}

	void gateway_connection_mgr::stop()
	{
		const int32 conn = m_conn_index.exchange(-1);
		if (conn >= 0)
		{
			net_client_mgr::getInstance().stop(static_cast<uint32>(conn));
		}
		config_center_client::getInstance().stop();
		m_on_connected.clear();
		m_on_closed.clear();
	}

	bool gateway_connection_mgr::is_connected() const
	{
		return m_conn_index.load() >= 0;
	}

	void gateway_connection_mgr::send(const void* data_ptr, size_t data_len)
	{
		const int32 conn = m_conn_index.load();
		if (conn < 0 || data_ptr == nullptr || data_len == 0)
		{
			return;
		}
		net_client_mgr::getInstance().send_message(
			static_cast<uint32>(conn), data_ptr, data_len);
	}

	void gateway_connection_mgr::on_conn_status(const net_client* peer)
	{
		if (peer == nullptr || peer->get_server_type() != e_server_type_gateway)
		{
			return;
		}
		m_conn_index = static_cast<int32>(peer->get_array_index());
		_RLOG_(MINFO, "gateway_connection_mgr connected, conn_index="
			<< m_conn_index.load());
		if (m_on_connected)
		{
			m_on_connected(peer->get_array_index());
		}
	}

	void gateway_connection_mgr::on_conn_closed(const net_client* peer)
	{
		if (peer == nullptr || peer->get_server_type() != e_server_type_gateway)
		{
			return;
		}
		const int32 conn = static_cast<int32>(peer->get_array_index());
		int32 expected = conn;
		if (m_conn_index.compare_exchange_strong(expected, -1))
		{
			_RLOG_(MWARN, "gateway_connection_mgr closed, conn_index=" << conn);
			if (m_on_closed)
			{
				m_on_closed(peer->get_array_index());
			}
		}
	}
}
