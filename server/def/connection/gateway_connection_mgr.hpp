#pragma once

#include <atomic>
#include <string>

#include <boost/function.hpp>
#include <singleton.hpp>
#include "connection/config_center_client.hpp"
#include "server_def.hpp"

namespace faith
{
	class net_client;

	// Shared outbound gateway TCP client for WS / CS (reversed topology).
	class gateway_connection_mgr : public singleton<gateway_connection_mgr>
	{
		friend class singleton<gateway_connection_mgr>;

	public:
		using conn_handler = boost::function<void(uint32 conn_index)>;

		struct start_params
		{
			config_center_client::register_params registry;
			conn_handler on_connected;
			conn_handler on_closed;
		};

		// Register to config_center, find same-game_id gateway peer, then connect.
		bool register_and_start(const start_params& params);
		bool connect(const s_server_info& gateway_info);
		void stop();

		bool is_connected() const;
		int32 conn_index() const { return m_conn_index.load(); }

		void send(const void* data_ptr, size_t data_len);

	private:
		gateway_connection_mgr() = default;

		void on_conn_status(const net_client* peer);
		void on_conn_closed(const net_client* peer);

		s_server_info m_gateway_info;
		std::atomic<int32> m_conn_index{ -1 };
		conn_handler m_on_connected;
		conn_handler m_on_closed;
	};
}
