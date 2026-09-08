#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include <net/scheduler.hpp>
#include <net/tcp_client.hpp>

namespace gateway_test_client
{
	struct options
	{
		std::string host = "127.0.0.1";
		std::uint16_t port = 2200;
		std::uint32_t connections = 1000;
		std::uint32_t duration_seconds = 60;
		std::uint32_t interval_milliseconds = 1000;
		std::uint32_t scheduler_threads = 4;
		// Connected hold duration before timed disconnect; max 0 disables.
		std::uint32_t hold_min_seconds = 10;
		std::uint32_t hold_max_seconds = 30;
		// Delay after timed disconnect before reconnect.
		std::uint32_t reconnect_delay_milliseconds = 200;
		// "load" = ping stress; "login" = discover gateway via config_center then login
		std::string mode = "load";
		std::string account = "test_account";
		std::string password = "123456";
		std::int32_t server_id = 10200;
		// config_center discovery (login mode)
		std::string config_center_host = "127.0.0.1";
		std::uint16_t config_center_port = 19000;
		bool config_center_https = true;
		std::uint32_t gateway_index = 0;
	};

	class load_client
	{
	public:
		explicit load_client(const options& config);
		~load_client();

		bool start();
		void stop();
		void print_summary() const;

	private:
		using clock_type = std::chrono::steady_clock;
		using time_point = clock_type::time_point;

		struct connection_state
		{
			bool connected = false;
			bool connecting = false;
			bool handshake_received = false;
			bool login_sent = false;
			bool intentional_disconnect = false;
			bool disconnecting = false;
			std::uint64_t session_epoch = 0;
			time_point next_retry = time_point::max();
			time_point next_disconnect = time_point::max();
			std::deque<time_point> pending_pings;
		};

		class connection_object
		{
		public:
			connection_object(std::uint32_t connection_index,
				std::uint32_t scheduler_thread_id)
				: connection_index(connection_index)
				, scheduler_thread_id(scheduler_thread_id)
			{
			}

			std::uint32_t connection_index;
			std::uint32_t scheduler_thread_id;
			faith::net::tcp_client_session_ptr session;
			connection_state state;
		};

		struct statistics
		{
			std::uint64_t connection_attempts = 0;
			std::uint64_t connection_successes = 0;
			std::uint64_t connection_failures = 0;
			std::uint64_t connections_closed = 0;
			std::uint64_t intentional_disconnects = 0;
			std::uint64_t handshakes = 0;
			std::uint64_t login_requests = 0;
			std::uint64_t login_responses = 0;
			std::uint64_t requests = 0;
			std::uint64_t responses = 0;
			std::uint64_t malformed_responses = 0;
			std::uint64_t send_failures = 0;
			std::uint64_t timeouts = 0;
			std::uint64_t unexpected_messages = 0;
			std::uint64_t latency_microseconds = 0;
			std::uint64_t min_latency_microseconds = 0;
			std::uint64_t max_latency_microseconds = 0;
		};

		void connect_one(std::uint32_t connection_index);
		void disconnect_one(std::uint32_t connection_index);
		void schedule_next_disconnect(
			connection_state& connection,
			time_point now);
		// Caller must hold m_mutex.
		std::uint32_t random_hold_seconds();
		void handle_connection(
			std::uint32_t connection_index,
			faith::net::tcp_client_session_ptr session,
			faith::net::tcp_client::e_connect_info status,
			xstring info);
		void on_connection(
			std::uint32_t connection_index,
			faith::net::tcp_client_session_ptr session,
			faith::net::tcp_client::e_connect_info status,
			xstring info);
		void handle_closed(
			std::uint32_t connection_index,
			faith::net::tcp_client_session_ptr session);
		void on_closed(
			std::uint32_t connection_index,
			faith::net::tcp_client_session_ptr session);
		void handle_received(
			std::uint32_t connection_index,
			faith::net::tcp_client_session_ptr session,
			const void* data,
			size_t data_length);
		void on_received(
			std::uint32_t connection_index,
			faith::net::tcp_client_session_ptr session,
			const void* data,
			size_t data_length);
		void on_timer(unsigned int timer_index);
		void on_maintenance_timer(unsigned int timer_index);
		void send_pings();
		void send_ping_for_connection(
			std::uint32_t connection_index,
			time_point now);
		void send_login_for_connection(std::uint32_t connection_index);
		void retry_connections(time_point now);
		void cycle_disconnects(time_point now);
		void expire_timeouts(connection_state& connection, time_point now);
		void record_latency(std::uint64_t latency_microseconds);

		options m_options;
		faith::net::tcp_client& m_tcp_client;
		std::vector<std::unique_ptr<connection_object>> m_connections;
		mutable std::mutex m_mutex;
		statistics m_statistics;
		std::atomic<bool> m_stopping{ false };
		bool m_started = false;
		unsigned int m_timer_index = faith::net::scheduler::scheduler_invalid_timer_index;
		unsigned int m_maintenance_timer_index =
			faith::net::scheduler::scheduler_invalid_timer_index;
		time_point m_next_status_log;
		std::uint32_t m_next_ramp_connection = 0;
		std::mt19937 m_rng;
	};
}
