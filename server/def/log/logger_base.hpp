/********************************************************************
	file base:	logger_base
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _LOGGER_H_
#define _LOGGER_H_

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT 1
#include "spdlog\spdlog.h"
#include "common\xchar.hpp"


namespace std
{
	std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const std::exception& val);
}

namespace hld
{
	class logger_base
	{
	public:
		logger_base();
		~logger_base();
	public:
		void	project_record_log(xstring name);
		void	server_runing_log(xstring name);

		std::shared_ptr<spdlog::logger> get_logger() { return m_logger; }
	private:
		std::shared_ptr<spdlog::logger> m_logger;
	};
	template<typename... Args>
	inline void console_info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
		spdlog::default_logger_raw()->info(fmt, std::forward<Args>(args)...);
	}
	template<typename... Args>
	inline void console_error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
		spdlog::default_logger_raw()->error(fmt, std::forward<Args>(args)...);
	}
#define PROJECT_RECORD_LOG_PRIVATE(OBJ, ...)			{OBJ.get_logger()->info(__VA_ARGS__);OBJ.get_logger()->flush();}
#define SERVER_RUNING_LOG_PRIVATE(OBJ, MSG, ...)		{OBJ.get_logger()->info(MSG, __VA_ARGS__); OBJ.get_logger()->flush();}
}

#endif
