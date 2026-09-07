#include "allowlist_config.hpp"

#include <cstring>

#include <tinyxml/tinyxml.h>

#include "game_cfg/xml_config_util.hpp"

namespace faith
{
	namespace config_center
	{
		bool allowlist_config::load_from_file(const std::string& path, std::string& error)
		{
			TiXmlDocument doc;
			if (!doc.LoadFile(path.c_str()))
			{
				error = std::string("failed to load ") + path + ": " + (doc.ErrorDesc() ? doc.ErrorDesc() : "unknown");
				return false;
			}

			TiXmlElement* root = doc.RootElement();
			if (root == nullptr || std::strcmp(root->Value(), "config_center") != 0)
			{
				error = "root element must be <config_center>";
				return false;
			}

			center_config config;
			config.listen_host = xml_child_text(root, "listen_host", config.listen_host.c_str());
			config.listen_port = xml_child_int(root, "listen_port", config.listen_port);
			config.use_https = xml_child_bool(root, "use_https", config.use_https);
			config.heartbeat_ttl_sec = static_cast<std::uint32_t>(
				xml_child_int(root, "heartbeat_ttl_sec", static_cast<int>(config.heartbeat_ttl_sec)));

			if (TiXmlElement* ssl = xml_child(root, "ssl"))
			{
				config.ssl.cert_file = xml_child_text(ssl, "cert_file", config.ssl.cert_file.c_str());
				config.ssl.key_file = xml_child_text(ssl, "key_file", config.ssl.key_file.c_str());
			}
			if (TiXmlElement* redis = xml_child(root, "redis"))
			{
				config.redis.host = xml_child_text(redis, "host", config.redis.host.c_str());
				config.redis.port = xml_child_int(redis, "port", config.redis.port);
				config.redis.db = xml_child_int(redis, "db", config.redis.db);
				config.redis.password = xml_child_text(redis, "password", config.redis.password.c_str());
			}

			TiXmlElement* allowed = xml_child(root, "allowed");
			if (allowed == nullptr)
			{
				error = "missing <allowed>";
				return false;
			}

			for (TiXmlElement* item = allowed->FirstChildElement("server");
				item != nullptr;
				item = item->NextSiblingElement("server"))
			{
				allowlist_entry entry;
				entry.server_type = xml_child_text(item, "server_type", "");
				entry.server_index = xml_child_int(item, "server_index", -1);
				entry.internal_host = xml_child_text(item, "internal_host", "");
				entry.internal_port = xml_child_int(item, "internal_port", 0);
				entry.external_host = xml_child_text(item, "external_host", "");
				entry.external_port = xml_child_int(item, "external_port", 0);

				if (entry.server_type.empty())
				{
					error = "allowed entry missing server_type";
					return false;
				}
				if (entry.server_index < 0)
				{
					error = "allowed entry missing server_index";
					return false;
				}
				if (entry.internal_port <= 0)
				{
					error = "allowed entry missing internal_port";
					return false;
				}
				config.allowed.push_back(entry);
			}

			if (config.allowed.empty())
			{
				error = "allowed list is empty";
				return false;
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
