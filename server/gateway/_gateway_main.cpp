/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:06
	file base:	_gateway_main
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <net/scheduler.hpp>
#include <time.hpp>
#include <boost/lexical_cast.hpp>
#include <rlog.hpp>

#include "app/app_server.hpp"
#include "dump/dump.hpp"
#include "game_cfg/servers_config.h"
#include "gw_main_service.hpp"
#include "server_log.hpp"

namespace faith
{
	using net::scheduler;

	xstring app_get_pgroduct_version_string();
	void app_set_console_title(void);

	static bool init()
	{
		auto& main_svc = gw_main_service::getInstance();
		main_svc.wire_dependencies();
		return main_svc.start();
	}

	static void release()
	{
		gw_main_service::getInstance().stop();
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
	faith::rlog::options rlog_options;
	rlog_options.basename = "gateway";
	if (!faith::rlog::init(rlog_options))
	{
		return -1;
	}
	_RLOG_(MINFO, "gateway process entered, argc=" << argc);
	srand( faith::utility::time() );
	faith::app_set_console_title();

	if(!faith::servers_config_manager::getInstance().start())
	{
		_RLOG_(MERROR, "servers.xml initialization failed");
		return -1;
	}
	_RLOG_(MINFO, "servers.xml initialized");
	faith::server_log::init_new_log(SERVERCONFIG->game_id, "gateway");
	_RLOG_(MINFO, "server log initialized");

	faith::int32 server_index = 0;
	if (argc >= 2)
	{
		xstring server_index_str = argv[1];
		server_index = boost::lexical_cast<faith::int32>(server_index_str);
	}
	faith::gw_main_service::getInstance().configure(server_index);

	faith::dump postmortem(_XTEXT("gateway.exe"), _XTEXT("gateway-v") + faith::app_get_pgroduct_version_string());

	faith::scheduler::getInstance().set_option(faith::scheduler::options::thread_num(GATEWAY_SERVER_SCHEDULER_THREAD_COUNT));
	_RLOG_(MINFO, "scheduler configured, worker threads="
		<< GATEWAY_SERVER_SCHEDULER_THREAD_COUNT << ", main dispatch enabled");

	faith::app_server::getInstance().run(faith::init, faith::release, true);

	faith::postmortem::getInstance().release();
	faith::rlog::shutdown();

	return 0;
}
