#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace faith
{
	namespace config_center
	{
		struct server_endpoint
		{
			std::string server_type;
			std::int32_t server_index = 0;
			std::string internal_host;
			std::int32_t internal_port = 0;
			std::string external_host;
			std::int32_t external_port = 0;
		};

		struct allowlist_entry
		{
			std::string server_type;
			std::int32_t server_index = 0;
			std::string internal_host;
			std::int32_t internal_port = 0;
			std::string external_host;
			std::int32_t external_port = 0;
		};

		struct redis_config
		{
			std::string host = "127.0.0.1";
			int port = 6379;
			int db = 0;
			std::string password;
		};

		struct ssl_config
		{
			std::string cert_file = "cert.pem";
			std::string key_file = "key.pem";
		};

		struct center_config
		{
			std::string listen_host = "0.0.0.0";
			int listen_port = 19000;
			bool use_https = true;
			ssl_config ssl;
			redis_config redis;
			std::uint32_t heartbeat_ttl_sec = 30;
			std::vector<allowlist_entry> allowed;
		};

		class allowlist_config
		{
		public:
			bool load_from_file(const std::string& path, std::string& error);
			const center_config& get() const { return m_config; }

			std::optional<allowlist_entry> match(
				const std::string& server_type,
				std::int32_t server_index,
				const std::string& internal_host,
				std::int32_t internal_port,
				const std::string& external_host,
				std::int32_t external_port,
				std::string& error) const;

		private:
			center_config m_config;
		};
	}
}
