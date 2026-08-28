#include "log_file_mgr.h"
#include "game_cfg/servers_config.h"
namespace hld
{
	logger_base    g_default_logger;
	void log_file_mgr::init_server_id(int32 in_server_id)
	{
		m_server_id = int32_to_string(in_server_id);
		
		m_process_id = boost::lexical_cast<xstring>(boost::interprocess::ipcdetail::get_current_process_id());
		m_path_root = SERVERCONFIG->new_log_file_root;

		m_map_data.clear();

		m_cur_time.year = 0;
		m_cur_time.month_in_year = 0;
		m_cur_time.day_in_month = 0;
		m_cur_time.hour_in_day = 0;

		m_last_time.year = 0;
		m_last_time.month_in_year = 0;
		m_last_time.day_in_month = 0;
		m_last_time.hour_in_day = 0;

	}

	void log_file_mgr::register_logger(xstring name)
	{
		auto end_name = m_server_id + "_" + name;
		std::shared_ptr<logger_base> log_obj = std::make_shared<logger_base>();
		log_obj->project_record_log(end_name);
		m_map_data[name] = log_obj;
	}

	void log_file_mgr::init_file_path()
	{
		m_cur_time = time_helper::get_cur_time_new();

		refresh_file_name();

	}

	xstring log_file_mgr::get_day()
	{

		xchar temp[32] = { 0 };
		sprintf(temp, "%d-%02d-%02d"
				, m_cur_time.year, m_cur_time.month_in_year + 1, m_cur_time.day_in_month + 1);
		xstring time = temp;
		return time;
	}
	xstring log_file_mgr::get_day_hour()
	{
		xchar temp[32] = { 0 };
		sprintf(temp, "%d%02d%02d%02d"
			, m_cur_time.year, m_cur_time.month_in_year + 1, m_cur_time.day_in_month + 1 , m_cur_time.hour_in_day);
		xstring time = temp;
		return time;
	}

	xstring log_file_mgr::int32_to_string(int32 value_i32)
	{
		std::string res_value = "";
		std::stringstream ss;
		ss << value_i32;
		ss >> res_value;
		return res_value;
	}
	xstring log_file_mgr::get_file_name(xstring date_str, xstring name, xstring date_hour_str)
	{
		xstring path_name = m_path_root + date_str + "/";
		path_name += m_server_id + "_";
		path_name += name + "_";
		path_name += date_hour_str + "_";
		path_name += m_process_id + ".json";

		return path_name;
	}

	void log_file_mgr::refresh_file_name()
	{
		xstring date_str = get_day();
		xstring date_hour_str = get_day_hour();
		//for (auto it = m_map_data.begin(); it != m_map_data.end(); it++)
		//{
		//	xstring file_name = it->first;
		//	xstring file_path = get_file_name(date_str, file_name, date_hour_str);
		//	logger_base& log_obj = *it->second;
		//	log_obj.remove_appender(file_name);
		//	log_obj.add_file_appender(file_name, file_path, "", false, true);

		//}
		m_last_time = m_cur_time;
	}

	bool log_file_mgr::is_refresh()
	{
		m_cur_time = time_helper::get_cur_time_new();
		if (m_cur_time.year == m_last_time.year
			&& m_cur_time.month_in_year == m_last_time.month_in_year
			&& m_cur_time.day_in_month == m_last_time.day_in_month
			&& m_cur_time.hour_in_day == m_last_time.hour_in_day
			/*&& m_cur_time.minute_in_hour == m_last_time.minute_in_hour*/)
			return false;

		return true;

	}
	

	logger_base & log_file_mgr::get_logger_by_name(xstring name)
	{
		if (is_refresh())
		{
			refresh_file_name();
		}

		auto it = m_map_data.find(name);
		if (it == m_map_data.end())
		{
			return g_default_logger;
		}

		return *it->second;
	}


}