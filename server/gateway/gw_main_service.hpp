#pragma once

#include <singleton.hpp>
#include <net/scheduler.hpp>
#include "service/server_service.hpp"
#include "server_def.hpp"

namespace faith
{
	// Process-level orchestrator for gateway startup / shutdown.
	// Dependency order (gateway has no outbound gateway_connection_mgr):
	// servers_config_manager -> template_manager -> config_center_client
	//   -> gw_main_service
	class gw_main_service
		: public server_service
		, public singleton<gw_main_service>
	{
		friend class singleton<gw_main_service>;

	public:
		void configure(int32 server_index);
		// Requires servers_config_manager already started (for SERVERCONFIG).
		void wire_dependencies();
		const char* service_name() const override { return "gw_main_service"; }
		int32 server_index() const { return m_server_index; }

	protected:
		bool on_start() override;
		void on_stop() override;

	private:
		gw_main_service() = default;

		void main_thread_alive(uint32 timer_index);

		int32 m_server_index = 0;
		uint32 m_main_alive_timer_index = net::scheduler::scheduler_invalid_timer_index;
		bool m_wired = false;
	};
}
