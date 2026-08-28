#ifndef _LOG_FILE_MGR_H_
#define _LOG_FILE_MGR_H_

#include "logic/type_def.hpp"
#include "common/singleton.hpp"
#include "logger_base.hpp"
#include "utility/cs_date.hpp"

namespace hld
{
	typedef std::unordered_map<xstring, std::shared_ptr<logger_base>>  file_log_map;

	class log_file_mgr : public singleton< log_file_mgr>
	{
	public:
		log_file_mgr() {}
		~log_file_mgr() {}
	public:
		void    init_server_id(int32 in_server_id);
		void    register_logger(xstring name);
		void    init_file_path();
		xstring int32_to_string(int32 value_i32);

		void    refresh_file_name();
		xstring get_day();
		xstring get_day_hour();

		xstring get_file_name(xstring date_str, xstring name, xstring date_hour_str);

		bool    is_refresh();

		logger_base& get_logger_by_name(xstring name);


	 private:
		 xstring  m_path_root;
		 xstring  m_server_id;
		 xstring  m_process_id;
		 time_info m_last_time;
		 time_info m_cur_time;

		 file_log_map m_map_data; 

		// logger_base default_obj;
	};
}








#endif