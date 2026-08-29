/********************************************************************
	created:	2014/07/25
	created:	25:7:2014   19:27
	file base:	log_db_proxy
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _LOGDBPROXY_HPP_
#define _LOGDBPROXY_HPP_

#include "logic/type_def.hpp"
#include <xchar.hpp>
#include <singleton.hpp>

namespace faith
{

	class clog_db_proxy : public singleton< clog_db_proxy >
	{
	private:
		friend class singleton< clog_db_proxy >;
	public:
		typedef boost::function<int32(const void*,size_t)>	sender_handler_type;
		typedef boost::function<void(xstring&)>				execute_handler_type;
	public:
		void init_sender(sender_handler_type sender);
		void init_executer(execute_handler_type executer);
		void execute_sql(xstring& str_sql);
		bool is_inited();
		void send(const void* data, size_t length);
	private:
		sender_handler_type		m_sender;
		execute_handler_type	m_execute;
	};

}

#endif