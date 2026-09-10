#include "gw_main_service.hpp"

#include <boost/bind.hpp>
#include <net/host_ip.hpp>
#include <net/scheduler.hpp>
#include <rlog.hpp>

#include "connection/config_center_client.hpp"
#include "connection/daemon_client.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "net/message_manager.hpp"
#include "net/net_server_mgr.hpp"
#include "server/gateway_client.hpp"
#include "server/msg_dispatch_wrap.hpp"
#include "server/proxy_service_cli.hpp"
#include "server/ws_connection_mgr.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"

namespace faith
{
	using net::scheduler;

	void gw_main_service::configure(int32 server_index)
	{
		m_server_index = server_index;
	}

	void gw_main_service::wire_dependencies()
	{
		if (m_wired)
		{
			return;
		}

		auto& scm = servers_config_manager::getInstance();
		auto& tm = template_manager::get_instance();
		auto& cc = config_center_client::getInstance();

		if (!scm.is_started())
		{
			_RLOG_(MERROR, "gw_main_service::wire_dependencies requires servers_config_manager started");
			return;
		}
		if (SERVERCONFIG == nullptr || GATEWAYCONFIG == nullptr)
		{
			_RLOG_(MERROR, "gw_main_service::wire_dependencies missing SERVERCONFIG/GATEWAYCONFIG");
			return;
		}

		http_access_mgr::get_instance().init(false);

		tm.configure(template_manager::load_mode::full);
		tm.depend_on(scm);

		config_center_client::register_params cc_params;
		cc_params.center_host = SERVERCONFIG->config_center_host;
		cc_params.center_port = SERVERCONFIG->config_center_port;
		cc_params.use_https = true;
		cc_params.app_key = SERVERCONFIG->app_key;
		cc_params.server_type = "gateway";
		cc_params.game_id = SERVERCONFIG->game_id;

		const char* local_ip = net::get_host_ip();
		const std::string lan_ip = (local_ip && local_ip[0] != '\0') ? local_ip : "127.0.0.1";
		const int internal_port = GATEWAYCONFIG->internal_port > 0
			? GATEWAYCONFIG->internal_port
			: 2210;
		const int external_port = GATEWAYCONFIG->external_port > 0
			? GATEWAYCONFIG->external_port
			: internal_port;

		// Prefer XML hosts for local compound (127.0.0.1). Only fall back to
		// discovered LAN IP when XML left the field empty / wildcard.
		const std::string& xml_internal = GATEWAYCONFIG->internal_host;
		const std::string& xml_external = GATEWAYCONFIG->external_host;
		cc_params.internal_host =
			(xml_internal.empty() || xml_internal == "0.0.0.0") ? lan_ip : xml_internal;
		cc_params.internal_port = internal_port;
		cc_params.external_host =
			(xml_external.empty() || xml_external == "0.0.0.0") ? lan_ip : xml_external;
		cc_params.external_port = external_port;

		cc.configure(cc_params);
		cc.depend_on(tm);

		depend_on(cc);
		m_wired = true;
		_RLOG_(MINFO, "gw_main_service dependencies wired: "
			"servers_config_manager -> template_manager -> config_center_client "
			"-> gw_main_service");
	}

	void gw_main_service::main_thread_alive(uint32)
	{
		_RLOG_(MINFO, "gateway main thread alive, scheduler thread id="
			<< scheduler::getInstance().get_current_thread_id());
	}

	bool gw_main_service::on_start()
	{
		_RLOG_(MINFO, "gw_main_service on_start begin, server index=" << m_server_index);

		if (SERVERCONFIG == nullptr || GATEWAYCONFIG == nullptr)
		{
			_RLOG_(MERROR, "gateway config not loaded");
			return false;
		}

		message_manager::getInstance().set_server_type(e_server_type_gateway);
		net_server_mgr::getInstance().set_server_type(e_server_type_gateway);
		net_server_mgr::getInstance().set_server_index(SERVERCONFIG->game_id);

		if (!ws_connection_mgr::getInstance().init() ||
			!ws_connection_mgr::getInstance().start())
		{
			_RLOG_(MERROR, "ws_connection_mgr start failed");
			return false;
		}

		const int32 instance_id = SERVERCONFIG->game_id;
		if (!net_client_mgr::getInstance().set_netpara_option(
			GATEWAY_CLIENT_SEND_BUFF_SIZE,
			GATEWAY_CLIENT_RECV_BUFF_SIZE,
			INTERNAL_SERVER_MAX_PACKET_SIZE,
			GATEWAY_NEED_CLIENT_COUNT))
		{
			_RLOG_(MERROR, "gw_main_service: set_netpara_option error");
			return false;
		}
		_RLOG_(MINFO, "internal network parameters initialized");
		_RLOG_(MINFO, "server identity initialized, game_id=" << instance_id);

		if (!proxy_service_cli::getInstance().init())
		{
			_RLOG_(MERROR, "proxy_service_cli::getInstance().init error");
			return false;
		}
		_RLOG_(MINFO, "proxy service initialized");

		message_from_server_dispatcher::getInstance().init();
		_RLOG_(MINFO, "message dispatcher initialized");

		gateway_client::getInstance().start();
		_RLOG_(MINFO, "gateway internal client start requested");

		m_main_alive_timer_index = scheduler::getInstance().add_timer(
			5000,
			0,
			boost::bind(&gw_main_service::main_thread_alive, this, _1));
		_RLOG_(MINFO, "main-thread heartbeat timer initialized");

		daemon_client::getInstance().init(
			e_server_type_gateway,
			SERVERCONFIG->game_id,
			boost::bind(&gateway_client::handler_daemon_onrecv, &gateway_client::getInstance(), _1, _2));
		_RLOG_(MINFO, "daemon client initialized");
		_RLOG_(MINFO, "gw_main_service on_start end");
		return true;
	}

	void gw_main_service::on_stop()
	{
		_RLOG_(MINFO, "gw_main_service on_stop begin");
		if (m_main_alive_timer_index != scheduler::scheduler_invalid_timer_index)
		{
			scheduler::getInstance().remove_timer(m_main_alive_timer_index);
			m_main_alive_timer_index = scheduler::scheduler_invalid_timer_index;
		}

		gateway_client::getInstance().stop();
		proxy_service_cli::getInstance().stop();
		gateway_client::getInstance().stop();

		config_center_client::getInstance().stop();
		template_manager::get_instance().stop();
		servers_config_manager::getInstance().stop();
		_RLOG_(MINFO, "gw_main_service on_stop end");
	}
}
