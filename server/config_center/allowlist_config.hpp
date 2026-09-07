#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace faith
{
	namespace config_center
	{
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

		struct center_config
		{
			std::string listen_host = "0.0.0.0";
			int listen_port = 19000;
			redis_config redis;
			std::uint32_t heartbeat_ttl_sec = 30;
			std::vector<allowlist_entry> allowed;
		};

		class allowlist_config
		{
		public:
			bool load_from_file(const std::string& path, std::string& error);
			const center_config& get() const { return m_config; }

			// Match (type,index) and require internal host/port equal.
			// If request provides non-empty external_host or non-zero external_port,
			// those must also match the allowlist entry.
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
