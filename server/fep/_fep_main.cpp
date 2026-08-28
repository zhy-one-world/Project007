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
//#include <windows.h>

namespace hld
{
	using net::scheduler;
	int32 g_server_index = 0;

	static bool init()
	{
		message_manager::getInstance().set_server_type(e_server_type_fep);
		if( !net_client_mgr::getInstance().set_netpara_option(FEP_CLIENT_SEND_BUFF_SIZE, FEP_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, FEP_NEED_CLIENT_COUNT))
		{
			CONSOLE_INFO("main(): set_netpara_option error");
			return false;
		}
		http_access_mgr::get_instance().init(false);
		net_server_mgr::getInstance().set_server_type(e_server_type_fep);
		net_server_mgr::getInstance().set_server_index(g_server_index);
		if (!proxy_service_cli::getInstance().init())
		{
			CONSOLE_INFO("proxy_service_cli::getInstance().init error");
			return false;
		}
		message_from_server_dispatcher::getInstance().init();
		fep_client::getInstance().start();

		daemon_client::getInstance().init(e_server_type_fep, SERVERCONFIG->game_id,
			boost::bind(&fep_client::handler_daemon_onrecv, &fep_client::getInstance(), _1, _2));
		CONSOLE_INFO("main(): fep started");

		return true;
	}
	static void release()
	{
		CONSOLE_INFO("main(): main-thread leave loop");

		fep_client::getInstance().stop();
		CONSOLE_INFO("main(): fep stopped");

		proxy_service_cli::getInstance().stop();
		CONSOLE_INFO("main(): proxy_service_cli stopped");

		fep_client::getInstance().stop();
		CONSOLE_INFO("main(): fep_client stopped");
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

hld::int32 _XMAIN(hld::int32 argc, xchar* argv[])
{
	setlocale(LC_ALL, "utf8");
	hld::set_root_directory();
	srand( hld::utility::time() );
	hld::app_set_console_title();

	if(!hld::servers_config_manager::getInstance().init_config())
	{
		return -1;
	}
	//====Add for logger====
	hld::server_log::init_new_log(SERVERCONFIG->game_id, "fep");
	//====Add for logger====
	if (argc >= 2)
	{
		xstring server_index_str = argv[1];
		{
			using namespace hld;
			g_server_index = boost::lexical_cast<int32>(server_index_str);
		}
	}

	//	setup postmortem module
	hld::dump postmortem(_XTEXT("fep.exe"), _XTEXT("fep-v") + hld::app_get_pgroduct_version_string());

	hld::scheduler::getInstance().set_option(hld::scheduler::options::thread_num(FEP_SERVER_SCHEDULER_THREAD_COUNT));

	hld::app_server::getInstance().run(hld::init,hld::release);

	hld::postmortem::getInstance().release();

	return 0;
}
