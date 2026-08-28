/********************************************************************
	created:	2024\05\04
	file base:	logger_base
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "logger_base.hpp"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <string.h>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>

namespace std
{
	std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const std::exception& val)
	{
		return os << val.what();
	}
}

namespace hld 
{
	logger_base::logger_base()
	{
	}

	logger_base::~logger_base() 
	{
		if (m_logger)
		{
			m_logger->flush();
		}
	}

	void logger_base::project_record_log(xstring name)
	{
		xstring process_id = boost::lexical_cast<xstring>(boost::interprocess::ipcdetail::get_current_process_id());
		xstring end_name = name + "_" + process_id;
		m_logger = spdlog::daily_logger_st(name, _XTEXT("logs/project/") + end_name + _XTEXT(".json"), 0, 5);
		m_logger->set_pattern("%v");
	}

	void logger_base::server_runing_log(xstring name)
	{
		xstring process_id = boost::lexical_cast<xstring>(boost::interprocess::ipcdetail::get_current_process_id());
		xstring end_name = name + "_" + process_id;
		m_logger = spdlog::daily_logger_st(name, _XTEXT("logs/game/") + end_name + _XTEXT(".log"), 0, 10);
		m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
	}
}
