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

#include "net/message_manager.hpp"
#include "net/net_client_mgr.hpp"
#include "net/tcp_client.hpp"
#include "template/template_manager.h"
#include <net/scheduler.hpp>
#include <singleton.hpp>
#include <time.hpp>
#include <rlog.hpp>

namespace faith
{
 	static bool init()
	{
		message_manager::getInstance().set_server_type(e_server_type_dp);
		net_client_mgr::getInstance().set_netpara_option(DP_CLIENT_SEND_BUFF_SIZE, DP_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, e_server_type_max);

		http_access_mgr::get_instance().init(false); 
		db_manager::getInstance().start();
		if (false == dbproxy_service::getInstance().init_db_conn())
		{
			_RLOG_(MINFO, "init_db_conn init false ");
			return false;
		}
		script_mgr::get_instance().call_func(nullptr, "dp_main", 0, false, "");
		daemon_client::getInstance().init(e_server_type_dp, SERVERCONFIG->game_id,
			boost::bind(&dbproxy_service::handler_daemon_onrecv, &dbproxy_service::getInstance(), _1, _2));
		template_manager::get_instance().init_for_dp();
 		_RLOG_(MINFO, "main(): scheduler started");
 		_RLOG_(MINFO, "main(): dbproxy_service started");		
 		_RLOG_(MINFO, "main(): dbproxy started");
 		_RLOG_(MINFO, "main(): main-thread enter loop");

 		return true;
 	}
 	static void release()
 	{
 		_RLOG_(MINFO, "main(): main-thread leave loop");
 
 		dbproxy_service::getInstance().stop();
 		_RLOG_(MINFO, "main(): dbproxy_service stopped");
 
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
	faith::set_root_directory();

 	using faith::net::scheduler;	
 
 	srand( faith::utility::time() );
 	faith::app_set_console_title();
 

 	if(!faith::servers_config_manager::getInstance().init_config())
 	{
 		return -1;
 	}

	//====Add for logger====
	faith::server_log::init_new_log(SERVERCONFIG->game_id, "dp");
	//====Add for logger====

 	//	setup postmortem module
	faith::dump postmortem(_XTEXT("dp.exe"), _XTEXT("dp-v") + faith::app_get_pgroduct_version_string());

	using faith::net::scheduler;
	scheduler::getInstance().set_option(scheduler::options::thread_num(DP_SERVER_SCHEDULER_THREAD_COUNT));
 
 	faith::app_server::getInstance().run(faith::init,faith::release);	
 
 	faith::postmortem::getInstance().release();
 
 	return 0;
 }
