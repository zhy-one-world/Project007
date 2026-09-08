#pragma once

#include <atomic>
#include <mutex>
#include <singleton.hpp>
#include "server_def.hpp"
#include <net/net_server_mgr.hpp>

namespace faith
{
	// Manages inbound WS connections on gateway internal TCP (net_server_mgr).
	class ws_connection_mgr : public singleton<ws_connection_mgr>
	{
		friend class singleton<ws_connection_mgr>;

	public:
		bool init();
		bool start();
		void stop();

		bool is_ws_connected() const;
		int listen_port() const { return m_listen_port; }
		const std::string& listen_host() const { return m_listen_host; }

		void send_to_ws(const void* data_ptr, size_t data_len);
		void on_ws_login(uint32 conn_index, const s_server_info& info);
		void on_ws_closed(const net_server* peer);

	private:
		ws_connection_mgr() = default;

		void on_peer_closed(const net_server* peer);

		std::string m_listen_host = "0.0.0.0";
		int m_listen_port = 0;
		std::atomic<int32> m_ws_conn_index{ -1 };
		mutable std::mutex m_mutex;
	};
}
