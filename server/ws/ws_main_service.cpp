#include "ws_main_service.hpp"

#include <net/scheduler.hpp>
#include <net/host_ip.hpp>
#include <rlog.hpp>

#include "connection/daemon_client.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "msg_dispatch.hpp"
#include "net/message_manager.hpp"
#include "net/net_server_mgr.hpp"
#include "server/client_session_mgr.hpp"
#include "server_log.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "template/template_manager.h"
#include "utility/globle_data.h"
#include "world_server.hpp"
#include "ws_client.hpp"

#ifdef WS_SLIM_BUILD
#include "connection/config_center_client.hpp"
#include "connection/gateway_connection_mgr.hpp"
#endif

#ifndef WS_SLIM_BUILD
#include "invalid_word/CCharLib.hpp"
#include "invalid_word/CInvalidWord.hpp"
#include "server/cache_ws_mgr.hpp"
#include "server/web_client.hpp"
#endif

namespace faith
{
	using net::scheduler;

	void ws_main_service::wire_dependencies()
	{
		if (m_wired)
		{
			return;
		}

		auto& scm = servers_config_manager::getInstance();
		auto& tm = template_manager::get_instance();

		if (!scm.is_started())
		{
			_RLOG_(MERROR, "ws_main_service::wire_dependencies requires servers_config_manager started");
			return;
		}
		if (SERVERCONFIG == nullptr || WSCONFIG == nullptr)
		{
			_RLOG_(MERROR, "ws_main_service::wire_dependencies missing SERVERCONFIG/WSCONFIG");
			return;
		}

		tm.configure(template_manager::load_mode::full);
		tm.depend_on(scm);

#ifdef WS_SLIM_BUILD
		http_access_mgr::get_instance().init(false);

		auto& cc = config_center_client::getInstance();
		auto& gwm = gateway_connection_mgr::getInstance();

		config_center_client::register_params registry;
		registry.center_host = SERVERCONFIG->config_center_host;
		registry.center_port = SERVERCONFIG->config_center_port;
		registry.use_https = true;
		registry.app_key = SERVERCONFIG->app_key;
		registry.server_type = "ws";
		registry.game_id = SERVERCONFIG->game_id;

		const char* local_ip = net::get_host_ip();
		const std::string lan_ip = (local_ip && local_ip[0] != '\0') ? local_ip : "127.0.0.1";
		registry.internal_host = lan_ip;
		registry.internal_port = WSCONFIG->internal_port > 0 ? WSCONFIG->internal_port : 2300;
		registry.external_host = lan_ip;
		registry.external_port = registry.internal_port;

		cc.configure(registry);
		cc.depend_on(tm);

		gwm.configure(
			registry.game_id,
			[](uint32)
			{
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_gateway_connect);
			});
		gwm.depend_on(cc);

		// Depend on CCC only here. GWM must start AFTER set_netpara_option in on_start,
		// otherwise net_client_mgr has no connection slots and connect is a no-op.
		depend_on(cc);
		_RLOG_(MINFO, "ws_main_service dependencies wired: "
			"servers_config_manager -> template_manager -> config_center_client "
			"-> ws_main_service (gateway_connection_mgr starts in on_start)");
#else
		depend_on(tm);
		_RLOG_(MINFO, "ws_main_service dependencies wired: "
			"servers_config_manager -> template_manager -> ws_main_service");
#endif
		m_wired = true;
	}

	bool ws_main_service::on_start()
	{
		_RLOG_(MINFO, "ws_main_service on_start begin");
		message_manager::getInstance().set_server_type(e_server_type_ws);
		scheduler::getInstance().set_option(scheduler::options::thread_num(WS_SERVER_SCHEDULER_THREAD_COUNT));

		if (!net_client_mgr::getInstance().set_netpara_option(
			WS_CLIENT_SEND_BUFF_SIZE,
			WS_CLIENT_RECV_BUFF_SIZE,
			INTERNAL_SERVER_MAX_PACKET_SIZE,
			WS_NEED_CLIENT_COUNT))
		{
			return false;
		}

#ifdef WS_SLIM_BUILD
		// Soft-fail + 10s retry inside gateway_connection_mgr.
		(void)gateway_connection_mgr::getInstance().start();
#endif

#ifndef WS_SLIM_BUILD
		http_access_mgr::get_instance().init(false);
		http_access_mgr::get_instance().start_listen_http(
			WSCONFIG->http_port,
			boost::bind(&web_client::handler_http_onrecv, &web_client::get_instance(), _1));
#endif

		if (!world_server::getInstance().init())
		{
			_RLOG_(MINFO, "world_server init error");
			return false;
		}
		net_server_mgr::getInstance().set_server_type(e_server_type_ws);
		net_server_mgr::getInstance().set_server_index(0);

		cs_map_mgr_system::start_up();
		client_session_mgr::getInstance().init();
		globle_data::get_instance().init_manager();
		msg_dispatch::getInstance().init();
#ifndef WS_SLIM_BUILD
		CInvalidWord::getInstance().LoadResource();
		CCharLib::getInstance().LoadResource();
#endif
		ws_client::getInstance().start();
#ifndef WS_SLIM_BUILD
		cache_ws_mgr::get_instance().init();
		script_mgr::get_instance().call_func(nullptr, "ws_main", 0, false, "");
#endif
		daemon_client::getInstance().init(
			e_server_type_ws,
			SERVERCONFIG->game_id,
			boost::bind(&world_server::handler_daemon_onrecv, &world_server::getInstance(), _1, _2));

		_RLOG_(MINFO, "ws_main_service on_start end");
		return true;
	}

	void ws_main_service::on_stop()
	{
		_RLOG_(MINFO, "ws_main_service on_stop begin");
		world_server::getInstance().stop();
#ifdef WS_SLIM_BUILD
		gateway_connection_mgr::getInstance().stop();
		config_center_client::getInstance().stop();
#endif
		template_manager::get_instance().stop();
		servers_config_manager::getInstance().stop();
		_RLOG_(MINFO, "ws_main_service on_stop end");
	}
}
