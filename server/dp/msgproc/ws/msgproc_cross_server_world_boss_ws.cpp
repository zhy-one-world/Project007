/********************************************************************
created:	2019年5月28日15:44:18
file base:	msgproc_cross_server_world_boss_ws
file ext:	cpp
author:		zhaoyuming

purpose:
*********************************************************************/
#include "msgproc_cross_server_world_boss_ws.hpp"
#include <cross_server_world_boss_msg.hpp>
#include "dbproxy_service.hpp"
namespace faith
{
	void save_cross_server_world_boss_ws_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_save_cross_server_world_boss_msg_all* pdata = static_cast<const ws2dp_save_cross_server_world_boss_msg_all*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(ws2dp_save_cross_server_world_boss_msg_all))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_cross_world_boss_save(";
		sql << pdata->boss_template_id << ",";
		sql << "'(";
		sql << pdata->boss_template_id << ",";
		sql << pdata->boss_is_dead << ",";
		sql << pdata->kill_boss_time << ",";
		sql << pdata->kill_boss_player_server_id << ",";
		sql << pdata->kill_boss_player_guid.server_64 << ",";
		sql << "\\\'" << pdata->kill_boss_player_name << "\\\',";	
		sql << pdata->kill_boss_legion_guid.server_64 << ",";
		sql << "\\\'" << pdata->kill_boss_legion_name << "\\\')'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_cross_server_world_boss_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_cross_server_world_boss_ws_info_end(db_result_type result, uint32 connindex)
	{
	}


	void load_cross_server_world_boss_ws_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_load_cross_server_world_boss_msg_all* pdata = static_cast<const ws2dp_load_cross_server_world_boss_msg_all*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(ws2dp_load_cross_server_world_boss_msg_all))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cross_world_boss_info_load();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_cross_server_world_boss_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}
	void load_cross_server_world_boss_ws_info_end(db_result_type result, uint32 connindex)
	{
		cross_world_boss_info_to_db sql_data[m_boss_max_num];
		int32 data_size = sizeof(cross_world_boss_info_to_db);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size || data_num > m_boss_max_num)
		{
			return;
		}
		memset(sql_data, 0, sizeof(sql_data));
		db_read_data(&result, &sql_data, data_num*data_size);

		dp2ws_load_cross_server_world_boss_msg_all ret_msg;
		ret_msg.data_num = data_num;
		for (int32 i = 0; i < data_num; ++i)
		{
			memcpy(&ret_msg.dp_info[i], &sql_data[i], sizeof(cross_world_boss_info_to_db));
		}
		dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
	}
}