#pragma once

#include <singleton.hpp>
#include "service/server_service.hpp"

namespace faith
{
	// Process-level orchestrator for world server startup / shutdown.
	// Slim dependency order:
	// servers_config_manager -> template_manager -> config_center_client
	//   -> ws_main_service (gateway_connection_mgr starts inside on_start
	//      after net_client_mgr::set_netpara_option)
	class ws_main_service
		: public server_service
		, public singleton<ws_main_service>
	{
		friend class singleton<ws_main_service>;

	public:
		const char* service_name() const override { return "ws_main_service"; }

		// Requires servers_config_manager already started (for SERVERCONFIG).
		void wire_dependencies();

	protected:
		bool on_start() override;
		void on_stop() override;

	private:
		ws_main_service() = default;
		bool m_wired = false;
	};
}
