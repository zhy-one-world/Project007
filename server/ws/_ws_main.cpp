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
#include <net/scheduler.hpp>
#include "dump/dump.hpp"
#include <time.hpp>
#include "base/ecs_world.h"
#include "game_cfg/servers_config.h"
#include "app/app_server.hpp"
#include "server_log.hpp"
#include "ws_main_service.hpp"
#include "world_server.hpp"
#include <rlog.hpp>

namespace faith
{
	using net::scheduler;

	xstring	app_get_pgroduct_version_string(	);
	void		app_set_console_title( void );

	static bool init( )
	{
		auto& main_svc = ws_main_service::getInstance();
		main_svc.wire_dependencies();
		return main_svc.start();
	}

	static void release( )
	{
		ws_main_service::getInstance().stop();
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
	if (!faith::servers_config_manager::getInstance().start())
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
