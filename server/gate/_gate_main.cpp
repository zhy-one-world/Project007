/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved.

	Created_datetime : 	2012-3-15 14:02

	File Name :	_gait_main.cpp

	Author : zhangming

	Description :

	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include <net/Scheduler.hpp>
#include <time.hpp>
#include "game_mgr.h"
#include "gate_client.hpp"
#include "app/app_server.hpp"
#include "game_cfg/servers_config.h"
#include "connection/daemon_client.hpp"
#include "server_log.hpp"
#include "http/http_access_mgr.hpp"
#include "net/message_manager.hpp"
#include "dump/dump.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	Free Functions
//
//////////////////////////////////////////////////////////////////////////
//	global logger obj.
//hld::logger	server_log::get_game_log()(_XTEXT("gait"));

namespace hld
{

	xstring app_get_pgroduct_version_string();
	void app_set_console_title(void);

	static bool init()
	{
		message_manager::getInstance().set_server_type(e_server_type_gate);
		if (!net_client_mgr::getInstance().set_netpara_option(GATE_CLIENT_SEND_BUFF_SIZE, GATE_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, GATE_NEED_CLIENT_COUNT))
		{
			CONSOLE_INFO("net_client_mgr init error");
			return false;
		}
		http_access_mgr::get_instance().init(false);
		if (!game_mgr::getInstance().init())
		{
			CONSOLE_INFO("game_mgr init error");
			return false;
		}
		game_mgr::getInstance().start();
		gate_client::getInstance().start();

		daemon_client::getInstance().init(e_server_type_gate, SERVERCONFIG->game_id,
			boost::bind(&game_mgr::HandlerDaemonOnrecv, &game_mgr::getInstance(), _1, _2));
		return true;
	}
	static void release()
	{
		CONSOLE_INFO("main(): release");

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

hld::int32 _XMAIN(hld::int32 argc, xchar* argv[])
{
	setlocale(LC_ALL, "utf8");
	hld::set_root_directory();
	srand(hld::utility::time());
	hld::app_set_console_title();

	if (!hld::servers_config_manager::getInstance().init_config())
	{
		return -1;
	}

	//====Add for logger====
	hld::server_log::init_new_log(SERVERCONFIG->game_id, "gate");
	//====Add for logger====
	//	setup postmortem module
	hld::dump postmortem(_XTEXT("gate.exe"), _XTEXT("gate-v") + hld::app_get_pgroduct_version_string());

	using hld::net::scheduler;

	hld::scheduler::getInstance().set_option(scheduler::options::thread_num(GATE_SERVER_SCHEDULER_THREAD_COUNT));

	hld::app_server::getInstance().run(hld::init, hld::release);

	return 0;
}