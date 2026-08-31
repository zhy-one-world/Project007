/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:06
	file base:	_fep_main
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <net/scheduler.hpp>
#include <singleton.hpp>
#include "dump/dump.hpp"
#include <time.hpp>
#include "server/fep_client.hpp"
#include "net/net_server_mgr.hpp"
#include "server/proxy_service_cli.hpp"
#include "game_cfg/servers_config.h"
#include "app/app_server.hpp"
#include "server/msg_dispatch_wrap.hpp"
#include "server_log.hpp"
#include "connection/daemon_client.hpp"
#include "http/http_access_mgr.hpp"
#include "net/message_manager.hpp"
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
		_RLOG_(MINFO, "fep main thread alive, scheduler thread id="
			<< scheduler::getInstance().get_current_thread_id());
	}

	static bool init()
	{
		_RLOG_(MINFO, "fep initialization started, server index=" << g_server_index);
		message_manager::getInstance().set_server_type(e_server_type_fep);
		if( !net_client_mgr::getInstance().set_netpara_option(FEP_CLIENT_SEND_BUFF_SIZE, FEP_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, FEP_NEED_CLIENT_COUNT))
		{
			_RLOG_(MERROR, "main(): set_netpara_option error");
			return false;
		}
		_RLOG_(MINFO, "internal network parameters initialized");
		http_access_mgr::get_instance().init(false);
		_RLOG_(MINFO, "HTTP access manager initialized");
		net_server_mgr::getInstance().set_server_type(e_server_type_fep);
		net_server_mgr::getInstance().set_server_index(g_server_index);
		_RLOG_(MINFO, "server identity initialized");
		if (!proxy_service_cli::getInstance().init())
		{
			_RLOG_(MERROR, "proxy_service_cli::getInstance().init error");
			return false;
		}
		_RLOG_(MINFO, "proxy service initialized");
		message_from_server_dispatcher::getInstance().init();
		_RLOG_(MINFO, "message dispatcher initialized");
		fep_client::getInstance().start();
		_RLOG_(MINFO, "fep internal client start requested");
		g_main_alive_timer_index = scheduler::getInstance().add_timer(
			5000,
			0,
			boost::bind(&main_thread_alive, _1));
		_RLOG_(MINFO, "main-thread heartbeat timer initialized");

		daemon_client::getInstance().init(e_server_type_fep, SERVERCONFIG->game_id,
			boost::bind(&fep_client::handler_daemon_onrecv, &fep_client::getInstance(), _1, _2));
		_RLOG_(MINFO, "daemon client initialized");
		_RLOG_(MINFO, "main(): fep started");

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

		fep_client::getInstance().stop();
		_RLOG_(MINFO, "main(): fep stopped");

		proxy_service_cli::getInstance().stop();
		_RLOG_(MINFO, "main(): proxy_service_cli stopped");

		fep_client::getInstance().stop();
		_RLOG_(MINFO, "main(): fep_client stopped");
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
	rlog_options.basename = "fep";
	if (!faith::rlog::init(rlog_options))
	{
		return -1;
	}
	_RLOG_(MINFO, "fep process entered, argc=" << argc);
	srand( faith::utility::time() );
	faith::app_set_console_title();

	if(!faith::servers_config_manager::getInstance().init_config())
	{
		_RLOG_(MERROR, "servers.json initialization failed");
		return -1;
	}
	_RLOG_(MINFO, "servers.json initialized");
	//====Add for logger====
	faith::server_log::init_new_log(SERVERCONFIG->game_id, "fep");
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
	faith::dump postmortem(_XTEXT("fep.exe"), _XTEXT("fep-v") + faith::app_get_pgroduct_version_string());

	faith::scheduler::getInstance().set_option(faith::scheduler::options::thread_num(FEP_SERVER_SCHEDULER_THREAD_COUNT));
	_RLOG_(MINFO, "scheduler configured, worker threads="
		<< FEP_SERVER_SCHEDULER_THREAD_COUNT << ", main dispatch enabled");

	faith::app_server::getInstance().run(faith::init, faith::release, true);

	faith::postmortem::getInstance().release();
	faith::rlog::shutdown();

	return 0;
}
