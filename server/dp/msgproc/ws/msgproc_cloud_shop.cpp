/********************************************************************
file name:	msgproc_cloud_shop.hpp
created:	2018/07/18 18:02
author:		shangxiaoliang
purpose:
*********************************************************************/
#include "msgproc_cloud_shop.hpp"
#include "dbproxy_service.hpp"
#include "dbproxy_service.hpp"
#include <cloud_shop_msg.hpp>

namespace hld
{
	void  req_load_cloud_shop_role_record(uint32 connindex, int32 start_pos)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cloud_shop_role_record_load(")
			<< start_pos << ","
			<< max_load_record_num
			<< ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_cloud_role_record_end, _1, connindex, start_pos);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_cloud_role_record(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const hld::ws2dp_load_cloud_shop_players_info* pdata = static_cast<const hld::ws2dp_load_cloud_shop_players_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cloud_shop_role_record_load(")
			<< 0 << ","
			<< max_load_record_num 
			<< ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_cloud_role_record_end, _1, connindex, 0);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_cloud_role_record_end(db_result_type result, uint32 connindex, int32 start_pos)
	{
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(cloud_shop_role_record_data);
		if (result.error || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 load_count = 0;
			bool is_load_end = false;
			if (table_len >= max_load_record_num)
			{
				load_count = max_load_record_num;
			}
			else
			{
				load_count = table_len;
				is_load_end = true;
			}

			dp2ws_load_cloud_shop_role_record_end reply;
			memset(&reply.load_data, 0, data_size*max_load_record_num);
			db_read_data(&result, &reply.load_data, data_size*load_count);
			reply.load_num = load_count;
			reply.is_end = is_load_end;
			dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(dp2ws_load_cloud_shop_role_record_end));

			if (!is_load_end)
			{
				req_load_cloud_shop_role_record(connindex, start_pos + load_count);
			}

			return;
		}
	}

	void ws2dp_req_save_cloud_role_record(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const hld::ws2dp_save_cloud_shop_role_info* pdata = static_cast<const hld::ws2dp_save_cloud_shop_role_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cloud_shop_role_record_save(");
		sql << pdata->player_role_info.role_guid.server_64 << ",";
		sql << pdata->player_role_info.get_data(e_cloud_shop_role_info_activity_type) << ",'(";
		sql << pdata->player_role_info.role_guid.server_64 << ",";
		sql << "\\\'" << pdata->player_role_info.role_name << "\\\',";
		for (int32 i = 0; i < e_cloud_shop_role_info_max; i++)
		{
			sql << pdata->player_role_info.info_data[i];
			if (i != e_cloud_shop_role_info_max - 1)
			{
				sql << ",";
			}
		}
		sql << ")');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_save_cloud_role_record_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_save_cloud_role_record_end(db_result_type result, uint32 connindex)
	{
		//do nothing
	}
	
	void ws2dp_req_clear_cloud_shop_player_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const hld::ws2dp_clear_cloud_shop_player_info* pdata = static_cast<const hld::ws2dp_clear_cloud_shop_player_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cloud_shop_clear_player_info(");
		sql << pdata->clear_type;
		sql << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_clear_cloud_shop_player_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_clear_cloud_shop_player_info_end(db_result_type result, uint32 connindex)
	{
		//do nothing
	}

}



