#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config_center.pb.h"

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

		struct registered_instance
		{
			ServerEndpoint endpoint;
		};

		class redis_registry
		{
		public:
			redis_registry();
			~redis_registry();

			bool connect(const redis_options& options);
			void set_heartbeat_ttl_sec(std::uint32_t ttl_sec);

			// Returns false if already registered (key exists) or redis error.
			bool try_register(const ServerEndpoint& endpoint, std::string& error);
			bool heartbeat(const std::string& server_type, std::int32_t server_index, std::string& error);
			bool unregister(const std::string& server_type, std::int32_t server_index, std::string& error);
			bool list_all(std::vector<ServerEndpoint>& out, std::string& error);

		private:
			static std::string make_key(const std::string& server_type, std::int32_t server_index);
			static std::string serialize_endpoint(const ServerEndpoint& endpoint);
			static bool deserialize_endpoint(const std::string& payload, ServerEndpoint& endpoint);

			struct impl;
			impl* m_impl;
			std::uint32_t m_ttl_sec;
		};
	}
}
