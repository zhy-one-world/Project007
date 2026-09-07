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

		bool redis_registry::using_redis() const
		{
			return m_impl != nullptr && m_impl->redis != nullptr;
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
				conn.connect_timeout = std::chrono::milliseconds(1000);
				conn.socket_timeout = std::chrono::milliseconds(1000);

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

		std::string redis_registry::make_key(const std::string& server_type, std::int32_t game_id)
		{
			std::ostringstream oss;
			oss << "cc:inst:" << server_type << ":" << game_id;
			return oss.str();
		}

		std::string redis_registry::serialize_endpoint(const server_endpoint& endpoint)
		{
			Json::Value root;
			root["server_type"] = endpoint.server_type;
			root["game_id"] = endpoint.game_id;
			root["internal_host"] = endpoint.internal_host;
			root["internal_port"] = endpoint.internal_port;
			root["external_host"] = endpoint.external_host;
			root["external_port"] = endpoint.external_port;
			Json::StreamWriterBuilder writer;
			writer["indentation"] = "";
			return Json::writeString(writer, root);
		}

		bool redis_registry::deserialize_endpoint(const std::string& payload, server_endpoint& endpoint)
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
				!root.isMember("internal_host") || !root["internal_host"].isString() ||
				!root.isMember("internal_port") || !root["internal_port"].isInt())
			{
				return false;
			}

			int game_id = -1;
			if (root.isMember("game_id") && root["game_id"].isInt())
			{
				game_id = root["game_id"].asInt();
			}
			else if (root.isMember("server_index") && root["server_index"].isInt())
			{
				game_id = root["server_index"].asInt();
			}
			else
			{
				return false;
			}

			endpoint.server_type = root["server_type"].asString();
			endpoint.game_id = game_id;
			endpoint.internal_host = root["internal_host"].asString();
			endpoint.internal_port = root["internal_port"].asInt();
			if (root.isMember("external_host") && root["external_host"].isString())
			{
				endpoint.external_host = root["external_host"].asString();
			}
			if (root.isMember("external_port") && root["external_port"].isInt())
			{
				endpoint.external_port = root["external_port"].asInt();
			}
			return true;
		}

		void redis_registry::purge_expired_memory()
		{
			const auto now = std::chrono::steady_clock::now();
			for (auto it = m_memory.begin(); it != m_memory.end();)
			{
				if (it->second.expire_at <= now)
				{
					it = m_memory.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		bool redis_registry::memory_try_register(const server_endpoint& endpoint, std::string& error)
		{
			std::lock_guard<std::mutex> lock(m_memory_mutex);
			purge_expired_memory();
			const std::string key = make_key(endpoint.server_type, endpoint.game_id);
			if (m_memory.find(key) != m_memory.end())
			{
				error = "already registered";
				return false;
			}
			memory_entry entry;
			entry.endpoint = endpoint;
			entry.expire_at = std::chrono::steady_clock::now() + std::chrono::seconds(m_ttl_sec);
			m_memory.emplace(key, entry);
			return true;
		}

		bool redis_registry::memory_heartbeat(
			const std::string& server_type,
			std::int32_t game_id,
			std::string& error)
		{
			std::lock_guard<std::mutex> lock(m_memory_mutex);
			purge_expired_memory();
			const std::string key = make_key(server_type, game_id);
			auto it = m_memory.find(key);
			if (it == m_memory.end())
			{
				error = "not registered";
				return false;
			}
			it->second.expire_at = std::chrono::steady_clock::now() + std::chrono::seconds(m_ttl_sec);
			return true;
		}

		bool redis_registry::memory_unregister(
			const std::string& server_type,
			std::int32_t game_id,
			std::string& error)
		{
			(void)error;
			std::lock_guard<std::mutex> lock(m_memory_mutex);
			m_memory.erase(make_key(server_type, game_id));
			return true;
		}

		bool redis_registry::memory_list_all(std::vector<server_endpoint>& out, std::string& error)
		{
			(void)error;
			out.clear();
			std::lock_guard<std::mutex> lock(m_memory_mutex);
			purge_expired_memory();
			out.reserve(m_memory.size());
			for (const auto& item : m_memory)
			{
				out.push_back(item.second.endpoint);
			}
			return true;
		}

		bool redis_registry::try_register(const server_endpoint& endpoint, std::string& error)
		{
			if (!m_impl->redis)
			{
				return memory_try_register(endpoint, error);
			}
			try
			{
				const std::string key = make_key(endpoint.server_type, endpoint.game_id);
				if (m_impl->redis->exists(key) > 0)
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

		bool redis_registry::heartbeat(const std::string& server_type, std::int32_t game_id, std::string& error)
		{
			if (!m_impl->redis)
			{
				return memory_heartbeat(server_type, game_id, error);
			}
			try
			{
				const std::string key = make_key(server_type, game_id);
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

		bool redis_registry::unregister(const std::string& server_type, std::int32_t game_id, std::string& error)
		{
			if (!m_impl->redis)
			{
				return memory_unregister(server_type, game_id, error);
			}
			try
			{
				const std::string key = make_key(server_type, game_id);
				m_impl->redis->del(key);
				return true;
			}
			catch (const sw::redis::Error& ex)
			{
				error = ex.what();
				return false;
			}
		}

		bool redis_registry::list_all(std::vector<server_endpoint>& out, std::string& error)
		{
			out.clear();
			if (!m_impl->redis)
			{
				return memory_list_all(out, error);
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
					server_endpoint endpoint;
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

		bool redis_registry::list_by_game_id(
			std::int32_t game_id,
			std::vector<server_endpoint>& out,
			std::string& error)
		{
			std::vector<server_endpoint> all;
			if (!list_all(all, error))
			{
				return false;
			}
			out.clear();
			for (const auto& endpoint : all)
			{
				if (endpoint.game_id == game_id)
				{
					out.push_back(endpoint);
				}
			}
			return true;
		}
	}
}
