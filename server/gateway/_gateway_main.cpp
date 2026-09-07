/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:06
	file base:	_gateway_main
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <net/scheduler.hpp>
#include <singleton.hpp>
#include "dump/dump.hpp"
#include <time.hpp>
#include "server/gateway_client.hpp"
#include "server/config_center_client.hpp"
#include "net/net_server_mgr.hpp"
#include "server/proxy_service_cli.hpp"
#include "game_cfg/servers_config.h"
#include "app/app_server.hpp"
#include "server/msg_dispatch_wrap.hpp"
#include "server_log.hpp"
#include "connection/daemon_client.hpp"
#include "http/http_access_mgr.hpp"
#include "net/message_manager.hpp"
#include "utility/init_unit.h"
#include <boost/bind.hpp>
#include <rlog.hpp>
//#include <windows.h>

namespace faith
{
	using net::scheduler;
	int32 g_server_index = 0;
	uint32 g_main_alive_timer_index = scheduler::scheduler_invalid_timer_index;

	static void main_thread_alive(uint32)
	{
		_RLOG_(MINFO, "gateway main thread alive, scheduler thread id="
			<< scheduler::getInstance().get_current_thread_id());
	}

	static bool init()
	{
		_RLOG_(MINFO, "gateway initialization started, server index=" << g_server_index);

		if (SERVERCONFIG == nullptr || GATEWAYCONFIG == nullptr)
		{
			_RLOG_(MERROR, "gateway config not loaded");
			return false;
		}

		http_access_mgr::get_instance().init(false);
		_RLOG_(MINFO, "HTTP access manager initialized");

		config_center_client::register_params cc_params;
		cc_params.center_host = SERVERCONFIG->config_center_host;
		cc_params.center_port = SERVERCONFIG->config_center_port;
		cc_params.use_https = true;
		cc_params.app_key = SERVERCONFIG->app_key;
		cc_params.server_type = "gateway";
		cc_params.game_id = SERVERCONFIG->game_id;

		const char* local_ip = init_unit::get_host_ip();
		const std::string lan_ip = (local_ip && local_ip[0] != '\0') ? local_ip : "127.0.0.1";
		const int internal_port = GATEWAYCONFIG->internal_port > 0
			? GATEWAYCONFIG->internal_port
			: (GATEWAYCONFIG->external_port > 0 ? GATEWAYCONFIG->external_port : 2200);
		const int external_port = GATEWAYCONFIG->external_port > 0
			? GATEWAYCONFIG->external_port
			: internal_port;

		cc_params.internal_host = lan_ip;
		cc_params.internal_port = internal_port;
		cc_params.external_host = GATEWAYCONFIG->external_host.empty()
			? lan_ip
			: GATEWAYCONFIG->external_host;
		cc_params.external_port = external_port;

		std::string cc_error;
		_RLOG_(MINFO, "config_center register begin, host="
			<< cc_params.center_host << " port=" << cc_params.center_port
			<< " app_key=" << cc_params.app_key
			<< " type=" << cc_params.server_type
			<< " game_id=" << cc_params.game_id
			<< " internal=" << cc_params.internal_host << ":" << cc_params.internal_port
			<< " external=" << cc_params.external_host << ":" << cc_params.external_port);
		if (!config_center_client::getInstance().register_sync(cc_params, cc_error))
		{
			_RLOG_(MERROR, "config_center register failed: " << cc_error
				<< "; gateway will not start");
			return false;
		}
		config_center_client::getInstance().start_heartbeat();
		_RLOG_(MINFO, "config_center register succeeded, continue gateway start");

		const int32 instance_id = cc_params.game_id;
		message_manager::getInstance().set_server_type(e_server_type_gateway);
		if( !net_client_mgr::getInstance().set_netpara_option(GATEWAY_CLIENT_SEND_BUFF_SIZE, GATEWAY_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, GATEWAY_NEED_CLIENT_COUNT))
		{
			_RLOG_(MERROR, "main(): set_netpara_option error");
			return false;
		}
		_RLOG_(MINFO, "internal network parameters initialized");
		net_server_mgr::getInstance().set_server_type(e_server_type_gateway);
		net_server_mgr::getInstance().set_server_index(instance_id);
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
		g_main_alive_timer_index = scheduler::getInstance().add_timer(
			5000,
			0,
			boost::bind(&main_thread_alive, _1));
		_RLOG_(MINFO, "main-thread heartbeat timer initialized");

		daemon_client::getInstance().init(e_server_type_gateway, SERVERCONFIG->game_id,
			boost::bind(&gateway_client::handler_daemon_onrecv, &gateway_client::getInstance(), _1, _2));
		_RLOG_(MINFO, "daemon client initialized");
		_RLOG_(MINFO, "main(): gateway started");

		return true;
	}
	static void release()
	{
		_RLOG_(MINFO, "main(): main-thread leave loop");
		if (g_main_alive_timer_index != scheduler::scheduler_invalid_timer_index)
		{
			scheduler::getInstance().remove_timer(g_main_alive_timer_index);
			g_main_alive_timer_index = scheduler::scheduler_invalid_timer_index;
		}

		config_center_client::getInstance().stop();
		_RLOG_(MINFO, "main(): config_center client stopped");

		gateway_client::getInstance().stop();
		_RLOG_(MINFO, "main(): gateway stopped");

		proxy_service_cli::getInstance().stop();
		_RLOG_(MINFO, "main(): proxy_service_cli stopped");

		gateway_client::getInstance().stop();
		_RLOG_(MINFO, "main(): gateway_client stopped");
	}

	static void set_root_directory()
	{
		char szPath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szPath, MAX_PATH);
		for (int i = strlen(szPath); i > 0; i--) {
			if (szPath[i] != '\\') {
				szPath[i] = '\0';
			}
			else {
				break;
			}
		}
		::SetCurrentDirectoryA(szPath);
	}

	xstring app_get_pgroduct_version_string();
	void app_set_console_title(void);
}

faith::int32 _XMAIN(faith::int32 argc, xchar* argv[])
{
	setlocale(LC_ALL, "utf8");
	faith::set_root_directory();
	faith::rlog::options rlog_options;
	rlog_options.basename = "gateway";
	if (!faith::rlog::init(rlog_options))
	{
		return -1;
	}
	_RLOG_(MINFO, "gateway process entered, argc=" << argc);
	srand( faith::utility::time() );
	faith::app_set_console_title();

	if(!faith::servers_config_manager::getInstance().init_config())
	{
		_RLOG_(MERROR, "servers.xml initialization failed");
		return -1;
	}
	_RLOG_(MINFO, "servers.xml initialized");
	//====Add for logger====
	faith::server_log::init_new_log(SERVERCONFIG->game_id, "gateway");
	_RLOG_(MINFO, "server log initialized");
	//====Add for logger====
	if (argc >= 2)
	{
		xstring server_index_str = argv[1];
		{
			using namespace faith;
			g_server_index = boost::lexical_cast<int32>(server_index_str);
		}
	}

	//	setup postmortem module
	faith::dump postmortem(_XTEXT("gateway.exe"), _XTEXT("gateway-v") + faith::app_get_pgroduct_version_string());

	faith::scheduler::getInstance().set_option(faith::scheduler::options::thread_num(GATEWAY_SERVER_SCHEDULER_THREAD_COUNT));
	_RLOG_(MINFO, "scheduler configured, worker threads="
		<< GATEWAY_SERVER_SCHEDULER_THREAD_COUNT << ", main dispatch enabled");

	faith::app_server::getInstance().run(faith::init, faith::release, true);

	faith::postmortem::getInstance().release();
	faith::rlog::shutdown();

	return 0;
}
