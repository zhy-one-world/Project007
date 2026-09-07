#pragma once

#include <chrono>
#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "allowlist_config.hpp"

namespace faith
{
	namespace config_center
	{
		struct redis_options
		{
			std::string host = "127.0.0.1";
			int port = 6379;
			int db = 0;
			std::string password;
		};

		class redis_registry
		{
		public:
			redis_registry();
			~redis_registry();

			// Returns false if Redis is unavailable; memory fallback is still enabled.
			bool connect(const redis_options& options);
			bool using_redis() const;
			void set_heartbeat_ttl_sec(std::uint32_t ttl_sec);

			bool try_register(const server_endpoint& endpoint, std::string& error);
			bool heartbeat(const std::string& server_type, std::int32_t game_id, std::string& error);
			bool unregister(const std::string& server_type, std::int32_t game_id, std::string& error);
			bool list_all(std::vector<server_endpoint>& out, std::string& error);
			bool list_by_game_id(std::int32_t game_id, std::vector<server_endpoint>& out, std::string& error);

		private:
			struct memory_entry
			{
				server_endpoint endpoint;
				std::chrono::steady_clock::time_point expire_at;
			};

			static std::string make_key(const std::string& server_type, std::int32_t game_id);
			static std::string serialize_endpoint(const server_endpoint& endpoint);
			static bool deserialize_endpoint(const std::string& payload, server_endpoint& endpoint);

			void purge_expired_memory();
			bool memory_try_register(const server_endpoint& endpoint, std::string& error);
			bool memory_heartbeat(const std::string& server_type, std::int32_t game_id, std::string& error);
			bool memory_unregister(const std::string& server_type, std::int32_t game_id, std::string& error);
			bool memory_list_all(std::vector<server_endpoint>& out, std::string& error);

			struct impl;
			impl* m_impl;
			std::uint32_t m_ttl_sec;
			std::mutex m_memory_mutex;
			std::map<std::string, memory_entry> m_memory;
		};
	}
}
