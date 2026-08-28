/********************************************************************
	created:	2014/07/25
	created:	25:7:2014   19:28
	file base:	log_db_proxy
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "log_db_proxy.hpp"
#include "internal/dp.hpp"

namespace hld
{

	void clog_db_proxy::init_sender(sender_handler_type sender)
	{
		m_sender = sender;
	}

	void clog_db_proxy::init_executer(execute_handler_type executer)
	{
		m_execute = executer;
	}

	bool clog_db_proxy::is_inited()
	{
		if (m_execute.empty() && m_sender.empty())
			return false;
		return true;
	}

	void clog_db_proxy::execute_sql(xstring& str_sql)
	{
		if(str_sql.size()*sizeof(xchar) >= req_query_log_db::max_limit)
			return;

		if (m_execute.empty())
		{
			if (m_sender.empty())
				return;
			req_query_log_db msg;
			msg.data_size = str_sql.size()*sizeof(xchar);
			memcpy(msg.data, str_sql.c_str(), msg.data_size);
			m_sender(&msg, msg.get_pak_length());
			return;
		}
		m_execute(str_sql);
	}

	void clog_db_proxy::send(const void* data, size_t length)
	{
		if (m_execute.empty() && !m_sender.empty())
			m_sender(data, length);
	}
}
