/********************************************************************
created:	2021Äê10ÔÂ09ÈÕ16:04:35
file base:	msgproc_attack_city_ws
file ext:	cpp
author:		zhaoyuming

purpose:
*********************************************************************/
#include "msgproc_attack_city_ws.hpp"
#include <attack_city_msg.hpp>
#include "dbproxy_service.hpp"
namespace hld
{
	void save_attack_city_ws_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_attack_city_save_legion_info* pdata = static_cast<const ws2dp_attack_city_save_legion_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_attack_city_save_legion_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_attack_city_info_save( ";
		sql << pdata->legion_info.legion_guid.server_64<< ",";
		sql << "'("; 
		sql << pdata->legion_info.group_level << ",";
		sql << pdata->legion_info.legion_guid.server_64 << ",";
		sql << pdata->legion_info.server_id << ",";
		sql << "\\\'" << pdata->legion_info.legion_name << "\\\',";
		sql << pdata->legion_info.winning_streak_num << ",";
		sql << pdata->legion_info.is_check <<" )');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_attack_city_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);


	}
	void save_attack_city_ws_info_end(db_result_type result, uint32 connindex)
	{
	}
	void load_attack_city_ws_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_attack_city_load_legion_info* pdata = static_cast<const ws2dp_attack_city_load_legion_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_attack_city_load_legion_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_attack_city_info_load();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_attack_city_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_attack_city_ws_info_end(db_result_type result, uint32 connindex)
	{
		s_attack_city_legion_info	sql_data[attack_city_save_dp_max_num];
		int32 data_size = sizeof(s_attack_city_legion_info);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size || data_num > attack_city_save_dp_max_num)
		{
			return;
		}
		memset(sql_data, 0, sizeof(sql_data));
		db_read_data(&result, &sql_data, data_num*data_size);

		dp2ws_attack_city_load_legion_info_end ret_msg;
		ret_msg.data_num = data_num;
		for (int32 i = 0; i < data_num; ++i)
		{
			memcpy(&ret_msg.info_list[i], &sql_data[i], sizeof(s_attack_city_legion_info));
		}
		dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
	}
	void save_attack_city_rank_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_attack_city_save_rank_info* pdata = static_cast<const ws2dp_attack_city_save_rank_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_attack_city_save_rank_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_attack_city_rank_info_save('";
		sql << pdata->rank_info.role_guid.server_64 << "',";
		sql << "'(";
		sql << pdata->rank_info.group_level << ",";
		sql << pdata->rank_info.group_rank << ",";
		sql << pdata->rank_info.legion_guid.server_64 << ",";
		sql << pdata->rank_info.role_guid.server_64 << ",";
		sql << "\\\'" << pdata->rank_info.role_name<< "\\\',";
		for (int32 i = 0; i < e_attack_city_rank_info_max; ++i)
		{
			if (i + 1 >= e_attack_city_rank_info_max)
			{
				sql << pdata->rank_info.role_info[i];
				break;
			}
			sql << pdata->rank_info.role_info[i] << ",";
		}
		sql <<" )');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_attack_city_rank_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
		
	}
	void save_attack_city_rank_info_end(db_result_type result, uint32 connindex)
	{
	}

	void load_attack_city_rank_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_attack_city_load_rank_info* pdata = static_cast<const ws2dp_attack_city_load_rank_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_attack_city_load_rank_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_attack_city_rank_info_load();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_attack_city_rank_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_attack_city_rank_info_end(db_result_type result, uint32 connindex)
	{
		s_attack_city_rank_info sql_data[TopMaxNum];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_attack_city_rank_info);

		if (result.error || table_len > TopMaxNum || result.query.data_select.row_size != data_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_attack_city_load_rank_info_end msg;
			msg.data_num = table_len;
			for (int32 i = 0; i < table_len; i++)
			{
				msg.info_list[i] = sql_data[i];
			}
			dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_attack_city_load_rank_info_end));
			return;
		}
	}
	void clear_attack_city_legion_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_attack_city_clear_legion_info* pdata = static_cast<const ws2dp_attack_city_clear_legion_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_attack_city_clear_legion_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_attack_city_info_clear();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_attack_city_legion_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void clear_attack_city_legion_info_end(db_result_type result, uint32 connindex)
	{
	}
	void clear_attack_city_rank_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_attack_city_clear_rank_info* pdata = static_cast<const ws2dp_attack_city_clear_rank_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_attack_city_clear_rank_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_attack_city_rank_info_clear();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_attack_city_rank_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void clear_attack_city_rank_info_end(db_result_type result, uint32 connindex)
	{
	}
}