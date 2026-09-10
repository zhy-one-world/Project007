#include "connection/gateway_connection_mgr.hpp"

#include <algorithm>
#include <boost/bind.hpp>
#include <rlog.hpp>

#include "net/net_client.hpp"
#include "net/net_client_mgr.hpp"

namespace faith
{
	void gateway_connection_mgr::configure(
		int32 game_id,
		conn_handler on_connected,
		conn_handler on_closed)
	{
		m_game_id = game_id;
		m_on_connected = on_connected;
		m_on_closed = on_closed;
	}

	bool gateway_connection_mgr::on_start()
	{
		m_running = true;
		if (!try_connect())
		{
			_RLOG_(MWARN, "gateway_connection_mgr connect deferred, retry every "
				<< k_retry_interval_ms << "ms");
			schedule_reconnect();
		}
		return true;
	}

	void gateway_connection_mgr::on_stop()
	{
		m_running = false;
		clear_reconnect_timer();
		const int32 conn = m_conn_index.exchange(-1);
		if (conn >= 0)
		{
			net_client_mgr::getInstance().stop(static_cast<uint32>(conn));
		}
		m_on_connected.clear();
		m_on_closed.clear();
	}

	bool gateway_connection_mgr::try_connect()
	{
		if (is_connected())
		{
			return true;
		}

		auto& cc = config_center_client::getInstance();
		if (!cc.ensure_registered())
		{
			_RLOG_(MWARN, "gateway_connection_mgr waiting for config_center register");
			return false;
		}

		const int32 game_id = m_game_id > 0 ? m_game_id : cc.params().game_id;
		const config_center_client::peer_endpoint* gateway_peer = nullptr;
		for (const auto& peer : cc.peers())
		{
			if (peer.server_type == "gateway" && peer.game_id == game_id)
			{
				gateway_peer = &peer;
				break;
			}
		}
		if (gateway_peer == nullptr)
		{
			_RLOG_(MWARN, "gateway_connection_mgr no gateway peer for game_id="
				<< game_id << ", will retry");
			return false;
		}
		if (gateway_peer->internal_host.empty() || gateway_peer->internal_port <= 0)
		{
			_RLOG_(MWARN, "gateway_connection_mgr gateway peer missing internal endpoint");
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

	void gateway_connection_mgr::schedule_reconnect()
	{
		if (!m_running.load() || is_connected())
		{
			return;
		}
		if (m_reconnect_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			return;
		}
		m_reconnect_timer = net::scheduler::getInstance().add_timer(
			static_cast<uint32>(k_retry_interval_ms),
			boost::bind(&gateway_connection_mgr::on_reconnect_timer, this, _1));
	}

	void gateway_connection_mgr::clear_reconnect_timer()
	{
		if (m_reconnect_timer != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_reconnect_timer);
			m_reconnect_timer = net::scheduler::scheduler_invalid_timer_index;
		}
	}

	void gateway_connection_mgr::on_reconnect_timer(uint32)
	{
		if (!m_running.load())
		{
			return;
		}
		if (is_connected())
		{
			clear_reconnect_timer();
			return;
		}
		if (try_connect())
		{
			// TCP connect is async; keep timer until on_conn_status, or clear if
			// start was accepted (net_client will retry TCP itself).
			clear_reconnect_timer();
			return;
		}
		// Timer auto-repeats; leave it armed.
	}

	bool gateway_connection_mgr::connect(const s_server_info& gateway_info)
	{
		m_gateway_info = gateway_info;
		m_gateway_info.server_type = e_server_type_gateway;
		_RLOG_(MINFO, "gateway_connection_mgr connecting "
			<< m_gateway_info.ip_addr << ":" << m_gateway_info.port);
		if (!net_client_mgr::getInstance().start(m_gateway_info,
			boost::bind(&gateway_connection_mgr::on_conn_status, this, _1),
			boost::bind(&gateway_connection_mgr::on_conn_closed, this, _1)))
		{
			_RLOG_(MERROR, "gateway_connection_mgr net_client_mgr::start failed "
				<< "(was set_netpara_option called?)");
			return false;
		}
		return true;
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
		clear_reconnect_timer();
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
			_RLOG_(MWARN, "gateway_connection_mgr closed, conn_index=" << conn
				<< "; net_client retries TCP every 10s");
			if (m_on_closed)
			{
				m_on_closed(peer->get_array_index());
			}
			// Do not call net_client_mgr::start again — same peer slot already
			// owns the reconnect timer. Only re-resolve if start never happened.
		}
	}
}
