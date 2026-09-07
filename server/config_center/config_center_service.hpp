#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

#include <net/tcp_server.hpp>
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

			void on_serverstatus_changed(net::tcp_server::e_server_status_type status);
			void on_conn_created(net::tcp_server_session_ptr session);
			void on_conn_closed(net::tcp_server_session_ptr session);
			void on_data_received(net::tcp_server_session_ptr session, const void* data, std::size_t data_len);

			void handle_message(const net::tcp_server_session_ptr& session, const CcMessage& request);
			bool send_message(const net::tcp_server_session_ptr& session, const CcMessage& message);

			void fill_peers(google::protobuf::RepeatedPtrField<ServerEndpoint>* peers);

			allowlist_config m_allowlist;
			redis_registry m_registry;
			std::unique_ptr<net::tcp_server> m_tcp_server;
			std::mutex m_session_mutex;
			std::unordered_map<net::tcp_server_session*, net::tcp_server_session_ptr> m_sessions;
		};
	}
}
