/********************************************************************
	created:	2014/08/18
	created:	18:8:2014   17:59
	file base:	_cs_main
	file ext:	cpp
	author:		zhy

	purpose:
*********************************************************************/
#include "app/app_server.hpp"
#include "base/ecs_world.h"
#include "cell_server.hpp"
#include "connection/daemon_client.hpp"
#include "dump/dump.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "invalid_word/CCharLib.hpp"
#include "invalid_word/CInvalidWord.hpp"
#include "logic/buff_man.h"
#include "logic/cache_mgr.hpp"
#include "logic/item_manager.h"
#include "logic/skill_manager.h"
#include "Logic/store_mgr.hpp"
#include "logic/unit_man.h"
#include "msg_dispatch.hpp"
#include "msgproc/msgproc_gm.hpp"
#include "net/message_manager.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"
#include <net/scheduler.hpp>
#include <time.hpp>
#include "utility/globle_data.h"

namespace faith
{
	xstring app_get_pgroduct_version_string();
	void app_set_console_title(void);

	static bool init()
	{
		CONSOLE_INFO("main(): main-thread init");

		if (!net_client_mgr::getInstance().set_netpara_option(CS_CLIENT_SEND_BUFF_SIZE, CS_CLIENT_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, CS_NEED_CLIENT_COUNT))
		{
			CONSOLE_ERROR("net_client_mgr init error");
			return false;
		}
		http_access_mgr::get_instance().init(false);
		if (false == cell_server::getInstance().init())
		{
			CONSOLE_ERROR("cell_server init error");
			return false;
		}
		message_manager::getInstance().set_server_type(e_server_type_cs);
		CONSOLE_INFO("begin template");
		template_manager::get_instance().init();
		CONSOLE_INFO("begin CCharLib");
		CCharLib::getInstance().LoadResource();
		CONSOLE_INFO("begin CInvalidWord");
		CInvalidWord::getInstance().LoadResource();
		CONSOLE_INFO("begin globle_data");
		globle_data::get_instance().init_manager();
		CONSOLE_INFO("begin init_send_ws");
		globle_data::get_instance().init_send_ws(boost::bind(&connection_mgr::send_to_ws, &connection_mgr::getInstance(), _1, _2, _3));
		CONSOLE_INFO("begin msg");
		msg_dispatch::getInstance().init();
		buff_man::init_manager();
		skill_manager::init_manager();
		item_manager::init_manager();
		CONSOLE_INFO("begin unit");
		if (false == unit_man::init_manager())
		{
			CONSOLE_ERROR("unit_man init error");
			return false;
		}
		cache_mgr::get_instance().init();
		store_mgr::get_instance().init_manager();
		init_gm_function_map();
		connection_mgr::getInstance().start();
		daemon_client::getInstance().init(e_server_type_cs, SERVERCONFIG->game_id,
			boost::bind(&cell_server::handler_daemon_onrecv, &cell_server::getInstance(), _1, _2));
		CONSOLE_INFO("main(): scheduler started");
		CONSOLE_INFO("main(): cell_server started");
		CONSOLE_INFO("main(): main-thread enter loop");

		return true;
	}
	static void release()
	{
		CONSOLE_INFO("main(): main-thread release");
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
	setlocale(LC_ALL, "utf8");
	faith::set_root_directory();
	srand(faith::utility::time());
	faith::app_set_console_title();
	if (!faith::servers_config_manager::getInstance().init_config())
	{
		std::cout << "server config fail!!" << std::endl;
		return -1;
	}

	//====Add for logger====
	faith::server_log::init_new_log(SERVERCONFIG->game_id, "cs");
	//====Add for logger====

	//	setup postmortem module
	faith::dump postmortem(_XTEXT("cs.exe"), _XTEXT("cs-v") + faith::app_get_pgroduct_version_string());

	faith::ecs_world::get_instance().init();

	using faith::net::scheduler;

	faith::scheduler::getInstance().set_option(scheduler::options::thread_num(CS_SERVER_SCHEDULER_THREAD_COUNT));

	faith::app_server::getInstance().run(faith::init, faith::release);

	faith::postmortem::getInstance().release();
	return 0;
}