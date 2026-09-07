#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

#include <net/scheduler.hpp>
#include <singleton.hpp>
#include "server_def.hpp"

namespace faith
{
	class config_center_client : public singleton<config_center_client>
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

		bool register_sync(const register_params& params, std::string& error);
		void start_heartbeat();
		void stop();

		bool is_registered() const { return m_registered.load(); }
		const std::vector<peer_endpoint>& peers() const { return m_peers; }

	private:
		config_center_client() = default;

		std::string make_base_url() const;
		bool post_json(
			const std::string& path,
			const std::string& body,
			std::string& response_body,
			std::string& error,
			int timeout_ms);
		void on_heartbeat_timer(uint32 timer_index);
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
		uint32 m_heartbeat_timer = net::scheduler::scheduler_invalid_timer_index;
	};
}
