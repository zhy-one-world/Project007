/********************************************************************
	created:	2017年11月30日15:37:47
	file base:	msgproc_log
	file ext:	cpp
	author:		allen wl
	
	purpose:	
*********************************************************************/
#include <stdio.h>
#if _MSC_VER
#define snprintf _snprintf
#endif
#include "msgproc_server_log.hpp"
#include "../../dbproxy_service.hpp"
#include <net/tcp_server.hpp>
#include <sstream>
#include <tchar.h>
#include "log_cache.hpp"
//#include "server_log.hpp"

namespace faith
{
	
	

	void dp_save_log_info(xstring& log_string, xstring& table_name)
	{
		if (log_string == "" || table_name == "")
		{
			return; 
		}

		if (log_cache::getInstance().push_log_to_cache(table_name, log_string))
		{
			return;
		}

		/*
		//解析字符串
		int32 protect_count = 0;
		int32 max_length = log_string.length();
		std::vector<xstring> value_array;
		value_array.clear();
		//boost::split(value_array, src_string, boost::is_any_of(server_log_separator));
		std::string::size_type pos1, pos2;
		pos2 = log_string.find(server_log_separator);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			value_array.push_back(log_string.substr(pos1, pos2 - pos1));

			pos1 = pos2 + sizeof(server_log_separator);
			pos2 = log_string.find(server_log_separator, pos1);
			if ((++protect_count) >= max_length)
			{
				break;
			}
		}
		if (pos1 != log_string.length())
			value_array.push_back(log_string.substr(pos1));

		int32 max_in_sql_size = value_array.size();
		sql_builder sql(db_manager::getInstance().get_db_link().log_db);
		sql << _XTEXT("call sp_save_log( '") << table_name.c_str() << "','(";

		for (int32 num = 0; num < max_in_sql_size; num++)
		{
			if (num == max_in_sql_size - 1)
			{
				sql << "\\\'" << value_array[num].c_str() << "\\\')'";
			}
			else
			{
				sql << "\\\'" << value_array[num].c_str() << "\\\',";
			}
		}
		sql << ")";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&dp_save_log_info_end,_1);
		db_manager::getInstance().get_db_link().log_db.add_query(query);
		*/
	}

	void dp_save_log_info_end(db_result_type result)
	{

	}

	void server2dp_req_save_log_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const server2dp_save_log* pdata = static_cast<const server2dp_save_log*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(server2dp_save_log))
		{
			return;
		}

		if (log_cache::getInstance().push_log_to_cache(pdata->sql_table_name, pdata->save_string))
		{
			return;
		}

		/*
		//解析字符串
		xstring src_string = pdata->save_string;
		int32 protect_count = 0;
		int32 max_length = src_string.length();
		std::vector<xstring> value_array;
		value_array.clear();
		//boost::split(value_array, src_string, boost::is_any_of(server_log_separator));
		std::string::size_type pos1, pos2;
		pos2 = src_string.find(server_log_separator);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			value_array.push_back(src_string.substr(pos1, pos2 - pos1));

			pos1 = pos2 + sizeof(server_log_separator);
			pos2 = src_string.find(server_log_separator, pos1);
			if ((++protect_count) >= max_length)
			{
				break;
			}
		}
		if (pos1 != src_string.length())
			value_array.push_back(src_string.substr(pos1));

		int32 max_in_sql_size = value_array.size();
		sql_builder sql(db_manager::getInstance().get_db_link().log_db);
		sql << _XTEXT("call sp_save_log( '") << pdata->sql_table_name << "','(";

		for (int32 num = 0; num < max_in_sql_size; num++)
		{
			if (num == max_in_sql_size - 1)
			{
				sql << "\\\'" << value_array[num].c_str() << "\\\')'";
			}
			else
			{
				sql << "\\\'" << value_array[num].c_str() << "\\\',";
			}

		}
		sql << ")";
		db_query_type query;
		query.logic_id = 13;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&server2dp_req_save_log_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().log_db.add_query(query);
		*/
	}

	void server2dp_req_save_log_info_end(db_result_type result, uint32 connindex)
	{ 
		static int32 a = 0;
		a++;
		return; 
	}

	void server2dp_req_save_chat_log_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const server2dp_save_chat_log* pdata = static_cast<const server2dp_save_chat_log*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(server2dp_save_chat_log))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().log_db);
		sql << _XTEXT("call sp_save_chat_log(");
		sql << pdata->server_id << ",";
		sql << "\'" << pdata->log_time << "\',";
		sql << "\'" << pdata->role_account << "\',";
		sql << pdata->role_guid.server_64 << ",";
		sql << "\'" << pdata->role_name << "\',";
		sql << pdata->role_level << ",";
		sql << pdata->target_guid.server_64 << ",";
		sql << pdata->chat_type << ",";
		sql << "\'" << pdata->chat_content << "\',";
		sql << pdata->login_type << ",";
		sql << "\'" << pdata->third_info.param14 << "\',";
		sql << "\'" << pdata->third_info.param6 << "\'";
		sql << ")";

		db_query_type query;
		query.logic_id = 13;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&server2dp_req_save_chat_log_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().log_db.add_query(query);
	}
	void server2dp_req_save_chat_log_info_end(db_result_type result, uint32 connindex)
	{
		static int32 a = 0;
		a++;
		return;
	}

	void server2dp_req_save_legion_log(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const server2dp_save_legion_log* pdata = static_cast<const server2dp_save_legion_log*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(server2dp_save_legion_log))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().log_db);
		sql << "call sp_save_legion_log(";
		sql << pdata->server_id << ",";
		sql << "\'" << pdata->log_time << "\',";
		sql << pdata->legion_guid.server_64 << ",";
		sql << pdata->role_guid.server_64 << ",";
		sql << "\'" << pdata->legion_name << "\',";
		sql << pdata->operate_type << ",";
		sql << "\'" << pdata->param_string << "\',";
		sql << pdata->login_type << ",";
		sql << "\'" << pdata->third_info.param14 << "\',";
		sql << "\'" << pdata->third_info.param6 << "\'";
		sql << ")";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&server2dp_req_save_legion_log_end, _1, connindex);
		db_manager::getInstance().get_db_link().log_db.add_query(query);
	}
	void server2dp_req_save_legion_log_end(db_result_type result, uint32 connindex)
	{
		static int32 a = 0;
		a++;
		return;
	}

}