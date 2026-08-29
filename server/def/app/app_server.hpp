#pragma once

#include <singleton.hpp>
#include <boost/function.hpp>

namespace faith
{
	class app_server : public singleton<app_server>
	{
		friend class singleton<app_server>;
	public:
		typedef boost::function<bool ()> init_handler_type;
		typedef boost::function<void ()> release_handler_type;
	private:
		app_server();
	public:
		void					run(init_handler_type init_handler,release_handler_type uninit_handler);
		void					stop();
	private:
		void					call_init_handler();
		void					call_release_handler();
	private:
		bool					m_stopping;
		bool					m_running;
		init_handler_type		m_init_handler;
		release_handler_type	m_release_handler;
	};
}
