#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

#include <net/scheduler.hpp>
#include <singleton.hpp>
#include "service/server_service.hpp"
#include "server_def.hpp"

namespace faith
{
	class config_center_client
		: public server_service
		, public singleton<config_center_client>
	{
		friend class singleton<config_center_client>;

	public:
		struct register_params
		{
			std::string center_host = "127.0.0.1";
			int center_port = 19000;
			bool use_https = true;
			std::string app_key;
			std::string server_type;
			int32 game_id = 0;
			std::string internal_host;
			int32 internal_port = 0;
			std::string external_host;
			int32 external_port = 0;
			int timeout_ms = 10000;
			int heartbeat_interval_ms = 10000;
		};

		struct peer_endpoint
		{
			std::string server_type;
			int32 game_id = 0;
			std::string internal_host;
			int32 internal_port = 0;
			std::string external_host;
			int32 external_port = 0;
		};

		void configure(const register_params& params);
		const char* service_name() const override { return "config_center_client"; }

		// Query GET /v1/registry: gateway list with external endpoints only.
		bool query_registry_sync(
			const std::string& center_host,
			int center_port,
			bool use_https,
			std::vector<peer_endpoint>& out,
			std::string& error,
			int timeout_ms = 10000);

		bool is_registered() const { return m_registered.load(); }
		const register_params& params() const { return m_params; }
		const std::vector<peer_endpoint>& peers() const { return m_peers; }

		// Attempt register now (or refresh peers). Safe to call repeatedly.
		bool ensure_registered();

	protected:
		bool on_start() override;
		void on_stop() override;

	private:
		config_center_client() = default;

		static constexpr int k_retry_interval_ms = 10000;

		std::string make_base_url() const;
		std::string make_base_url(
			const std::string& center_host,
			int center_port,
			bool use_https) const;
		bool request_json(
			const std::string& url,
			int method,
			const std::string& body,
			std::string& response_body,
			std::string& error,
			int timeout_ms);
		bool post_json(
			const std::string& path,
			const std::string& body,
			std::string& response_body,
			std::string& error,
			int timeout_ms);
		bool register_sync(const register_params& params, std::string& error);
		void start_heartbeat();
		void on_heartbeat_timer(uint32 timer_index);
		void schedule_register_retry();
		void on_register_retry_timer(uint32 timer_index);
		void clear_register_retry_timer();
		void poll_until(std::chrono::steady_clock::time_point deadline);

		register_params m_params;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_wait_done = false;
		bool m_wait_ok = false;
		std::string m_wait_error;
		std::string m_wait_body;
		std::vector<peer_endpoint> m_peers;
		std::atomic<bool> m_registered{ false };
		std::atomic<bool> m_running{ false };
		uint32 m_heartbeat_timer = net::scheduler::scheduler_invalid_timer_index;
		uint32 m_register_retry_timer = net::scheduler::scheduler_invalid_timer_index;
	};
}
