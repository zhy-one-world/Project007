/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:19
	file base:	mysql_worker_thread
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MYSQL_WORKER_THREAD_H_
#define _MYSQL_WORKER_THREAD_H_

#include "mysql_async_mgr_impl.hpp"

#define INTERACTIVE_TIMEOUT	30000

namespace faith
{
	struct db_query_type;
	struct mysql_conn_info;
	class mysql_async_mgr_impl;

	class worker_thread : public boost::noncopyable
	{
	public:
		explicit worker_thread( mysql_async_mgr_impl* ptr, int32 thread_no );
		~worker_thread(	);
	public:
		void					operator()( mysql_conn_info* conn_info );
		void					stop(	);
	private:
		mysql_async_mgr_impl*	m_mgr_ptr;
		db_query_type			m_current_query;
		boost::thread			m_thread;
		int32						m_no;
		volatile bool			m_run;
		ui64					m_pre_time;
	};
}

#endif
