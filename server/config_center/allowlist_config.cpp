#include "allowlist_config.hpp"

#include <fstream>

#include <json/json.h>

namespace faith
{
	namespace config_center
	{
		bool allowlist_config::load_from_file(const std::string& path, std::string& error)
		{
			std::ifstream in(path);
			if (!in)
			{
				error = "failed to open " + path;
				return false;
			}

			Json::CharReaderBuilder builder;
			Json::Value root;
			std::string parse_error;
			if (!Json::parseFromStream(builder, in, &root, &parse_error))
			{
				error = "json parse failed: " + parse_error;
				return false;
			}

			center_config config;
			if (root.isMember("listen_host") && root["listen_host"].isString())
			{
				config.listen_host = root["listen_host"].asString();
			}
			if (root.isMember("listen_port") && root["listen_port"].isInt())
			{
				config.listen_port = root["listen_port"].asInt();
			}
			if (root.isMember("heartbeat_ttl_sec") && root["heartbeat_ttl_sec"].isUInt())
			{
				config.heartbeat_ttl_sec = root["heartbeat_ttl_sec"].asUInt();
			}
			else if (root.isMember("heartbeat_ttl_sec") && root["heartbeat_ttl_sec"].isInt())
			{
				config.heartbeat_ttl_sec = static_cast<std::uint32_t>(root["heartbeat_ttl_sec"].asInt());
			}

			if (root.isMember("redis") && root["redis"].isObject())
			{
				const Json::Value& redis = root["redis"];
				if (redis.isMember("host") && redis["host"].isString())
				{
					config.redis.host = redis["host"].asString();
				}
				if (redis.isMember("port") && redis["port"].isInt())
				{
					config.redis.port = redis["port"].asInt();
				}
				if (redis.isMember("db") && redis["db"].isInt())
				{
					config.redis.db = redis["db"].asInt();
				}
				if (redis.isMember("password") && redis["password"].isString())
				{
					config.redis.password = redis["password"].asString();
				}
			}

			if (root.isMember("use_https") && root["use_https"].isBool())
			{
				config.use_https = root["use_https"].asBool();
			}
			if (root.isMember("ssl") && root["ssl"].isObject())
			{
				const Json::Value& ssl = root["ssl"];
				if (ssl.isMember("cert_file") && ssl["cert_file"].isString())
				{
					config.ssl.cert_file = ssl["cert_file"].asString();
				}
				if (ssl.isMember("key_file") && ssl["key_file"].isString())
				{
					config.ssl.key_file = ssl["key_file"].asString();
				}
			}

			if (!root.isMember("allowed") || !root["allowed"].isArray())
			{
				error = "missing allowed array";
				return false;
			}

			for (const auto& item : root["allowed"])
			{
				if (!item.isObject())
				{
					error = "allowed entry is not object";
					return false;
				}
				allowlist_entry entry;
				if (!item.isMember("server_type") || !item["server_type"].isString())
				{
					error = "allowed entry missing server_type";
					return false;
				}
				entry.server_type = item["server_type"].asString();
				if (!item.isMember("server_index") || !item["server_index"].isInt())
				{
					error = "allowed entry missing server_index";
					return false;
				}
				entry.server_index = item["server_index"].asInt();
				if (!item.isMember("internal_host") || !item["internal_host"].isString())
				{
					error = "allowed entry missing internal_host";
					return false;
				}
				entry.internal_host = item["internal_host"].asString();
				if (!item.isMember("internal_port") || !item["internal_port"].isInt())
				{
					error = "allowed entry missing internal_port";
					return false;
				}
				entry.internal_port = item["internal_port"].asInt();
				if (item.isMember("external_host") && item["external_host"].isString())
				{
					entry.external_host = item["external_host"].asString();
				}
				if (item.isMember("external_port") && item["external_port"].isInt())
				{
					entry.external_port = item["external_port"].asInt();
				}
				config.allowed.push_back(entry);
			}

			if (config.listen_port <= 0 || config.heartbeat_ttl_sec == 0)
			{
				error = "invalid listen_port or heartbeat_ttl_sec";
				return false;
			}

			m_config = std::move(config);
			return true;
		}

		std::optional<allowlist_entry> allowlist_config::match(
			const std::string& server_type,
			std::int32_t server_index,
			const std::string& internal_host,
			std::int32_t internal_port,
			const std::string& external_host,
			std::int32_t external_port,
			std::string& error) const
		{
			const allowlist_entry* found = nullptr;
			for (const auto& entry : m_config.allowed)
			{
				if (entry.server_type == server_type && entry.server_index == server_index)
				{
					found = &entry;
					break;
				}
			}
			if (found == nullptr)
			{
				error = "server not in allowlist";
				return std::nullopt;
			}
			if (internal_host.empty() || internal_port <= 0)
			{
				error = "missing internal endpoint";
				return std::nullopt;
			}
			if (found->internal_port != internal_port)
			{
				error = "internal port mismatch";
				return std::nullopt;
			}
			// 0.0.0.0 / * / empty allowlist host = accept any reported LAN IP.
			const bool internal_host_wildcard =
				found->internal_host.empty() ||
				found->internal_host == "*" ||
				found->internal_host == "0.0.0.0";
			if (!internal_host_wildcard && found->internal_host != internal_host)
			{
				error = "internal host mismatch";
				return std::nullopt;
			}

			if (external_host.empty() || external_port <= 0)
			{
				error = "missing external endpoint";
				return std::nullopt;
			}
			if (found->external_port != 0 && found->external_port != external_port)
			{
				error = "external port mismatch";
				return std::nullopt;
			}
			const bool external_host_wildcard =
				found->external_host.empty() ||
				found->external_host == "*" ||
				found->external_host == "0.0.0.0";
			if (!external_host_wildcard && found->external_host != external_host)
			{
				error = "external host mismatch";
				return std::nullopt;
			}
			return *found;
		}
	}
}
