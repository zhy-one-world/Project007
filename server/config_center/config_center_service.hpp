#pragma once

#include <string>

#include <json/json.h>
#include <net/http_types.hpp>
#include <singleton.hpp>

#include "allowlist_config.hpp"
#include "redis_registry.hpp"

namespace faith
{
	namespace config_center
	{
		class config_center_service : public singleton<config_center_service>
		{
			friend class singleton<config_center_service>;

		public:
			bool init(const std::string& config_path);
			bool start();
			void stop();

		private:
			config_center_service() = default;

			void on_http_request(const http_inbound_request& request);
			void handle_register(long handle, const Json::Value& body);
			void handle_heartbeat(long handle, const Json::Value& body);
			void handle_unregister(long handle, const Json::Value& body);
			void handle_query(long handle);
			void reply_json(long handle, int status, const Json::Value& body);

			allowlist_config m_allowlist;
			redis_registry m_registry;
			bool m_http_inited = false;
		};
	}
}
