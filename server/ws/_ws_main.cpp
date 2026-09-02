/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-2-25 13:35
	
	File Name :	_ws_main.cpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
//#include <winsock2.h>
#include <net/scheduler.hpp>
#include "dump/dump.hpp"
#include <time.hpp>
#include "server/client_session_mgr.hpp"
#include "world_server.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "ws_client.hpp"
#include "connection/daemon_client.hpp"
#include "app/app_server.hpp"
#include "msg_dispatch.hpp"
#include "utility/globle_data.h"
#include "invalid_word/CInvalidWord.hpp"
#include "template/template_manager.h"
#include "server/web_client.hpp"
#include "server_log.hpp"
#include "net/message_manager.hpp"
#include "server/cache_ws_mgr.hpp"

#include "system/scene/cs_map_mgr_system.h"
#include <rlog.hpp>

namespace faith
{
	using net::scheduler;

	xstring	app_get_pgroduct_version_string(	);
	void		app_set_console_title( void );

	static bool init( )
	{
		_RLOG_(MINFO, "main(): init begin");
		message_manager::getInstance().set_server_type(e_server_type_ws);
		//	setup log-level
		scheduler::getInstance().set_option(scheduler::options::thread_num( WS_SERVER_SCHEDULER_THREAD_COUNT ) );

		if (!net_client_mgr::getInstance().set_netpara_option(WS_CLIENT_SEND_BUFF_SIZE, WS_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, WS_NEED_CLIENT_COUNT))
		{
			return false;
		}
		http_access_mgr::get_instance().init(false);
		http_access_mgr::get_instance().start_listen_http(WSCONFIG->http_port,
			boost::bind(&web_client::handler_http_onrecv, &web_client::get_instance(), _1)
		);

		if (false == world_server::getInstance().init())
		{
			_RLOG_(MINFO, "world_server init error");
			return false;
		}
		cs_map_mgr_system::start_up();
		template_manager::get_instance().init();
		client_session_mgr::getInstance().init();
		globle_data::get_instance().init_manager();
		msg_dispatch::getInstance().init();
		CInvalidWord::getInstance().LoadResource();
		CCharLib::getInstance().LoadResource();
		ws_client::getInstance().start();
		cache_ws_mgr::get_instance().init();
		script_mgr::get_instance().call_func(nullptr, "ws_main", 0, false, "");
		daemon_client::getInstance().init(e_server_type_ws, SERVERCONFIG->game_id, 
			boost::bind(&world_server::handler_daemon_onrecv, &world_server::getInstance(), _1, _2));

		_RLOG_(MINFO, "main(): init end");
		return true;
	}

	static void release( )
	{
		_RLOG_(MINFO, "main(): main-thread leave loop");
		world_server::getInstance().stop();
		_RLOG_(MINFO, "main(): world_server stopped");
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
}

faith::int32 _XMAIN(faith::int32 argc, xchar* argv[])
{
	srand(faith::utility::time());
	setlocale(LC_ALL, "utf8");
	faith::set_root_directory();
	if (!faith::servers_config_manager::getInstance().init_config())
	{
		std::cout << "servers_config init fail!" << std::endl;
		return -1;
	}

	faith::app_set_console_title();

	faith::server_log::init_new_log(SERVERCONFIG->game_id, "ws");

	//	setup postmortem module
	faith::dump postmortem(_XTEXT("ws.exe"), _XTEXT("ws-v") + faith::app_get_pgroduct_version_string());

	faith::ecs_world::get_instance().init();

	using faith::net::scheduler;

	faith::scheduler::getInstance().set_option(scheduler::options::thread_num(WS_SERVER_SCHEDULER_THREAD_COUNT));

	faith::app_server::getInstance().run(faith::init,faith::release);

	faith::postmortem::getInstance().release();

	return 0;
}
