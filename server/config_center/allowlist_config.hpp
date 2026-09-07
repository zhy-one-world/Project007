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
			std::int32_t game_id = 0;
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

		struct allowed_game
		{
			std::int32_t game_id = 0;
			bool is_open = true;
		};

		struct center_config
		{
			std::string app_key;
			std::string listen_host = "0.0.0.0";
			int listen_port = 19000;
			bool use_https = true;
			ssl_config ssl;
			redis_config redis;
			std::uint32_t heartbeat_ttl_sec = 30;
			std::vector<allowed_game> allowed_games;
		};

		class allowlist_config
		{
		public:
			bool load_from_file(const std::string& path, std::string& error);
			const center_config& get() const { return m_config; }

			bool match(const std::string& app_key, std::int32_t game_id, std::string& error) const;

		private:
			center_config m_config;
		};
	}
}
