/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   16:23
	file base:	_dp_main
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "./dbproxy_service.hpp"
#include "app/app_server.hpp"
#include "connection/daemon_client.hpp"
#include "db_config.hpp"
#include "db_manager.hpp"
#include "dump/dump.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "log/log_db_proxy.hpp"
#include "lua/script_mgr.h"
#include "net/message_manager.hpp"
#include "net/net_client_mgr.hpp"
#include "net/tcp_client.hpp"
#include "template/template_manager.h"
#include <net/scheduler.hpp>
#include <singleton.hpp>
#include <time.hpp>

namespace hld
{
 	static bool init()
	{
		message_manager::getInstance().set_server_type(e_server_type_dp);
		net_client_mgr::getInstance().set_netpara_option(DP_CLIENT_SEND_BUFF_SIZE, DP_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, e_server_type_max);

		http_access_mgr::get_instance().init(false); 
		db_manager::getInstance().start();
		if (false == dbproxy_service::getInstance().init_db_conn())
		{
			CONSOLE_INFO("init_db_conn init false ");
			return false;
		}
		script_mgr::get_instance().call_func(nullptr, "dp_main", 0, false, "");
		daemon_client::getInstance().init(e_server_type_dp, SERVERCONFIG->game_id,
			boost::bind(&dbproxy_service::handler_daemon_onrecv, &dbproxy_service::getInstance(), _1, _2));
		template_manager::get_instance().init_for_dp();
 		CONSOLE_INFO("main(): scheduler started" );
 		CONSOLE_INFO("main(): dbproxy_service started" );		
 		CONSOLE_INFO("main(): dbproxy started" );
 		CONSOLE_INFO("main(): main-thread enter loop");

 		return true;
 	}
 	static void release()
 	{
 		CONSOLE_INFO("main(): main-thread leave loop");
 
 		dbproxy_service::getInstance().stop();
 		CONSOLE_INFO("main(): dbproxy_service stopped");
 
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
	hld::set_root_directory();

 	using hld::net::scheduler;	
 
 	srand( hld::utility::time() );
 	hld::app_set_console_title();
 

 	if(!hld::servers_config_manager::getInstance().init_config())
 	{
 		return -1;
 	}

	//====Add for logger====
	hld::server_log::init_new_log(SERVERCONFIG->game_id, "dp");
	//====Add for logger====

 	//	setup postmortem module
	hld::dump postmortem(_XTEXT("dp.exe"), _XTEXT("dp-v") + hld::app_get_pgroduct_version_string());

	using hld::net::scheduler;
	scheduler::getInstance().set_option(scheduler::options::thread_num(DP_SERVER_SCHEDULER_THREAD_COUNT));
 
 	hld::app_server::getInstance().run(hld::init,hld::release);	
 
 	hld::postmortem::getInstance().release();
 
 	return 0;
 }
