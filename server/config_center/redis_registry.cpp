#include "redis_registry.hpp"

#include <chrono>
#include <memory>
#include <sstream>

#include <json/json.h>
#include <sw/redis++/redis++.h>

namespace faith
{
	namespace config_center
	{
		struct redis_registry::impl
		{
			std::unique_ptr<sw::redis::Redis> redis;
		};

		redis_registry::redis_registry()
			: m_impl(new impl())
			, m_ttl_sec(30)
		{
		}

		redis_registry::~redis_registry()
		{
			delete m_impl;
			m_impl = nullptr;
		}

		bool redis_registry::connect(const redis_options& options)
		{
			try
			{
				sw::redis::ConnectionOptions conn;
				conn.host = options.host;
				conn.port = static_cast<int>(options.port);
				conn.db = options.db;
				if (!options.password.empty())
				{
					conn.password = options.password;
				}
				conn.connect_timeout = std::chrono::milliseconds(3000);
				conn.socket_timeout = std::chrono::milliseconds(3000);

				m_impl->redis = std::make_unique<sw::redis::Redis>(conn);
				m_impl->redis->ping();
				return true;
			}
			catch (const sw::redis::Error&)
			{
				m_impl->redis.reset();
				return false;
			}
		}

		void redis_registry::set_heartbeat_ttl_sec(std::uint32_t ttl_sec)
		{
			m_ttl_sec = ttl_sec == 0 ? 30 : ttl_sec;
		}

		std::string redis_registry::make_key(const std::string& server_type, std::int32_t server_index)
		{
			std::ostringstream oss;
			oss << "cc:inst:" << server_type << ":" << server_index;
			return oss.str();
		}

		std::string redis_registry::serialize_endpoint(const ServerEndpoint& endpoint)
		{
			Json::Value root;
			root["server_type"] = endpoint.server_type();
			root["server_index"] = endpoint.server_index();
			root["internal_host"] = endpoint.internal_host();
			root["internal_port"] = endpoint.internal_port();
			root["external_host"] = endpoint.external_host();
			root["external_port"] = endpoint.external_port();
			Json::StreamWriterBuilder writer;
			writer["indentation"] = "";
			return Json::writeString(writer, root);
		}

		bool redis_registry::deserialize_endpoint(const std::string& payload, ServerEndpoint& endpoint)
		{
			Json::CharReaderBuilder builder;
			Json::Value root;
			std::string error;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			if (!reader->parse(payload.data(), payload.data() + payload.size(), &root, &error))
			{
				return false;
			}
			if (!root.isMember("server_type") || !root["server_type"].isString() ||
				!root.isMember("server_index") || !root["server_index"].isInt() ||
				!root.isMember("internal_host") || !root["internal_host"].isString() ||
				!root.isMember("internal_port") || !root["internal_port"].isInt())
			{
				return false;
			}
			endpoint.set_server_type(root["server_type"].asString());
			endpoint.set_server_index(root["server_index"].asInt());
			endpoint.set_internal_host(root["internal_host"].asString());
			endpoint.set_internal_port(root["internal_port"].asInt());
			if (root.isMember("external_host") && root["external_host"].isString())
			{
				endpoint.set_external_host(root["external_host"].asString());
			}
			if (root.isMember("external_port") && root["external_port"].isInt())
			{
				endpoint.set_external_port(root["external_port"].asInt());
			}
			return true;
		}

		bool redis_registry::try_register(const ServerEndpoint& endpoint, std::string& error)
		{
			if (!m_impl->redis)
			{
				error = "redis not connected";
				return false;
			}
			try
			{
				const std::string key = make_key(endpoint.server_type(), endpoint.server_index());
				const bool exists = m_impl->redis->exists(key) > 0;
				if (exists)
				{
					error = "already registered";
					return false;
				}
				const std::string value = serialize_endpoint(endpoint);
				const auto ttl = std::chrono::milliseconds(
					static_cast<std::int64_t>(m_ttl_sec) * 1000);
				const bool ok = m_impl->redis->set(
					key,
					value,
					ttl,
					sw::redis::UpdateType::NOT_EXIST);
				if (!ok)
				{
					error = "already registered";
					return false;
				}
				return true;
			}
			catch (const sw::redis::Error& ex)
			{
				error = ex.what();
				return false;
			}
		}

		bool redis_registry::heartbeat(const std::string& server_type, std::int32_t server_index, std::string& error)
		{
			if (!m_impl->redis)
			{
				error = "redis not connected";
				return false;
			}
			try
			{
				const std::string key = make_key(server_type, server_index);
				if (m_impl->redis->exists(key) == 0)
				{
					error = "not registered";
					return false;
				}
				m_impl->redis->expire(key, std::chrono::seconds(m_ttl_sec));
				return true;
			}
			catch (const sw::redis::Error& ex)
			{
				error = ex.what();
				return false;
			}
		}

		bool redis_registry::unregister(const std::string& server_type, std::int32_t server_index, std::string& error)
		{
			if (!m_impl->redis)
			{
				error = "redis not connected";
				return false;
			}
			try
			{
				const std::string key = make_key(server_type, server_index);
				m_impl->redis->del(key);
				return true;
			}
			catch (const sw::redis::Error& ex)
			{
				error = ex.what();
				return false;
			}
		}

		bool redis_registry::list_all(std::vector<ServerEndpoint>& out, std::string& error)
		{
			out.clear();
			if (!m_impl->redis)
			{
				error = "redis not connected";
				return false;
			}
			try
			{
				std::vector<std::string> keys;
				long long cursor = 0;
				do
				{
					cursor = m_impl->redis->scan(
						cursor,
						"cc:inst:*",
						100,
						std::back_inserter(keys));
				} while (cursor != 0);

				for (const auto& key : keys)
				{
					auto value = m_impl->redis->get(key);
					if (!value)
					{
						continue;
					}
					ServerEndpoint endpoint;
					if (!deserialize_endpoint(*value, endpoint))
					{
						continue;
					}
					out.push_back(endpoint);
				}
				return true;
			}
			catch (const sw::redis::Error& ex)
			{
				error = ex.what();
				return false;
			}
		}
	}
}
