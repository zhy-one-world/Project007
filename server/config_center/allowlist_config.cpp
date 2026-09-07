#include "allowlist_config.hpp"

#include <cstring>

#include <tinyxml/tinyxml.h>

#include "game_cfg/xml_config_util.hpp"

namespace faith
{
	namespace config_center
	{
		namespace
		{
			bool parse_bool_attr(TiXmlElement* element, const char* name, bool default_value)
			{
				if (element == nullptr || name == nullptr)
				{
					return default_value;
				}
				const char* value = element->Attribute(name);
				if (value == nullptr || value[0] == '\0')
				{
					return default_value;
				}
				return std::strcmp(value, "1") == 0 ||
					std::strcmp(value, "true") == 0 ||
					std::strcmp(value, "TRUE") == 0 ||
					std::strcmp(value, "True") == 0;
			}
		}

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
			config.app_key = xml_child_text(root, "app_key", "");
			if (config.app_key.empty())
			{
				error = "missing <app_key>";
				return false;
			}

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

			for (TiXmlElement* game = allowed->FirstChildElement("game");
				game != nullptr;
				game = game->NextSiblingElement("game"))
			{
				int game_id = -1;
				if (game->Attribute("game_id"))
				{
					game->Attribute("game_id", &game_id);
				}
				else
				{
					game_id = xml_child_int(game, "game_id", -1);
				}
				if (game_id < 0)
				{
					error = "allowed <game> missing game_id attribute";
					return false;
				}

				allowed_game entry;
				entry.game_id = game_id;
				entry.is_open = parse_bool_attr(game, "is_open", true);
				config.allowed_games.push_back(entry);
			}

			if (config.allowed_games.empty())
			{
				error = "allowed list is empty (need <game game_id=\"...\" is_open=\"true\"/>)";
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

		bool allowlist_config::match(const std::string& app_key, std::int32_t game_id, std::string& error) const
		{
			if (app_key.empty() || app_key != m_config.app_key)
			{
				error = "app_key mismatch";
				return false;
			}
			if (game_id < 0)
			{
				error = "invalid game_id";
				return false;
			}
			for (const auto& game : m_config.allowed_games)
			{
				if (game.game_id != game_id)
				{
					continue;
				}
				if (!game.is_open)
				{
					error = "game_id is closed";
					return false;
				}
				return true;
			}
			error = "game_id not allowed";
			return false;
		}
	}
}
