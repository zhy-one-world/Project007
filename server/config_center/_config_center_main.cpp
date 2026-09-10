#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <windows.h>
#include <locale.h>

#include <net/scheduler.hpp>
#include <rlog.hpp>
#include <xchar.hpp>

#include "app/app_server.hpp"
#include "cc_main_service.hpp"
#include "dump/dump.hpp"

namespace faith
{
	using net::scheduler;

	static void set_root_directory()
	{
		char szPath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szPath, MAX_PATH);
		for (int i = static_cast<int>(strlen(szPath)); i > 0; --i)
		{
			if (szPath[i] != '\\')
			{
				szPath[i] = '\0';
			}
			else
			{
				break;
			}
		}
		::SetCurrentDirectoryA(szPath);
	}

	static bool init()
	{
		cc_main_service::getInstance().configure("./config_center.xml");
		return cc_main_service::getInstance().start();
	}

	static void release()
	{
		cc_main_service::getInstance().stop();
	}
}

faith::int32 _XMAIN(faith::int32 argc, xchar* argv[])
{
	(void)argc;
	(void)argv;
	setlocale(LC_ALL, "utf8");
	faith::set_root_directory();

	faith::rlog::options rlog_options;
	rlog_options.basename = "config_center";
	if (!faith::rlog::init(rlog_options))
	{
		return -1;
	}
	_RLOG_(MINFO, "config_center process entered");

	faith::dump postmortem(_XTEXT("config_center.exe"), _XTEXT("config_center"));

	faith::scheduler::getInstance().set_option(
		faith::scheduler::options::thread_num(4));
	faith::app_server::getInstance().run(faith::init, faith::release, true);

	faith::postmortem::getInstance().release();
	faith::rlog::shutdown();
	return 0;
}
