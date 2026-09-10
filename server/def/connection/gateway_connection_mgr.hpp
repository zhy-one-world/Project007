#pragma once

#include <atomic>
#include <string>

#include <boost/function.hpp>
#include <singleton.hpp>
#include <net/scheduler.hpp>
#include "service/server_service.hpp"
#include "connection/config_center_client.hpp"
#include "server_def.hpp"

namespace faith
{
	class net_client;

	// Shared outbound gateway TCP client for WS / CS (reversed topology).
	// Starts immediately; if config_center / gateway is not ready, retries every 10s.
	// TCP layer also retries every 10s via net_client.
	class gateway_connection_mgr
		: public server_service
		, public singleton<gateway_connection_mgr>
	{
		friend class singleton<gateway_connection_mgr>;

	public:
		using conn_handler = boost::function<void(uint32 conn_index)>;

		void configure(
			int32 game_id,
			conn_handler on_connected = conn_handler(),
			conn_handler on_closed = conn_handler());
		const char* service_name() const override { return "gateway_connection_mgr"; }

		bool is_connected() const;
		int32 conn_index() const { return m_conn_index.load(); }
		void send(const void* data_ptr, size_t data_len);

	protected:
		bool on_start() override;
		void on_stop() override;

	private:
		gateway_connection_mgr() = default;

		static constexpr int k_retry_interval_ms = 10000;

		bool try_connect();
		bool connect(const s_server_info& gateway_info);
		void schedule_reconnect();
		void clear_reconnect_timer();
		void on_reconnect_timer(uint32 timer_index);
		void on_conn_status(const net_client* peer);
		void on_conn_closed(const net_client* peer);

		int32 m_game_id = 0;
		s_server_info m_gateway_info;
		std::atomic<int32> m_conn_index{ -1 };
		std::atomic<bool> m_running{ false };
		conn_handler m_on_connected;
		conn_handler m_on_closed;
		uint32 m_reconnect_timer = net::scheduler::scheduler_invalid_timer_index;
	};
}
