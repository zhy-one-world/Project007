/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:35
	file base:	faith_app
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <net/scheduler.hpp>
#include "app_server.hpp"
#include "server_log.hpp"
#include <rlog.hpp>

namespace faith
{
	app_server::app_server():
		m_running(false),
		m_stopping(false),
		m_main_thread_dispatch(false)
	{
	}

	static bool faith_app_extend_init(app_server::init_handler_type init_handler)
	{
		return init_handler();
	}

	static void faith_app_extend_release(app_server::release_handler_type release_handler)
	{
		release_handler();
	}

	void app_server::run(init_handler_type init_handler,release_handler_type release_handler, bool main_thread_dispatch)
	{
		m_init_handler = boost::bind(faith_app_extend_init,init_handler);
		m_release_handler = boost::bind(faith_app_extend_release,release_handler);
		m_main_thread_dispatch = main_thread_dispatch;
		net::scheduler::getInstance().post(boost::bind(&app_server::call_init_handler,this));

		m_running = true;
		net::scheduler::getInstance().startup(m_main_thread_dispatch);
		if (m_main_thread_dispatch)
		{
			net::scheduler::getInstance().run_current_thread();
		}
		else
		{
			while(m_running)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}
		}
		net::scheduler::getInstance().shutdown();
	}

	void app_server::stop()
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message(" m_stopping :{}",  m_stopping));
		if(m_stopping)
		{
			return;
		}
		m_stopping = true;

		faith::net::scheduler::getInstance().post(boost::bind(&app_server::call_release_handler,this));
	}

	void app_server::call_init_handler()
	{
		if(!m_init_handler())
		{
			stop();
		}
	}

	void app_server::call_release_handler()
	{
		m_release_handler();
		m_running = false;
		if (m_main_thread_dispatch)
		{
			net::scheduler::getInstance().request_stop();
		}
	}

}
