/********************************************************************
created: 2019年5月31日
file base: msgproc_cross_ladder_info_ws
file ext: cpp
author: pengbiao

purpose: 跨服天梯
*********************************************************************/

#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_cross_ladder_info_ws.hpp"
#include <cross_ladder_msg.hpp>
#include <char_msg.hpp>

namespace faith
{
	void ws2dp_cross_ladder_load_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_load_info* pdata = static_cast<const ws2dp_cross_ladder_load_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		int32 cur_num = 0;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_cross_ladder_load_role_info(")
			<< cur_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_load_info_proc_end, _1, connindex, cur_num);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_cross_ladder_load_info_proc_end(db_result_type result, uint32 connindex, int32 cur_num)
	{
		if (result.error) // || result.query.data_select.row_count < 0)
			return;
		else
		{
			int32 data_num = result.query.data_select.row_count;
			dp2ws_cross_ladder_load_role_info msg;
			msg.cur_data_num = data_num;
			if (sizeof(s_cross_ladder_role_info) == result.query.data_select.row_size)
			{
				if (data_num > 0)
				{
					int32 data_size = result.query.data_select.row_size * data_num;
					db_read_data(&result, &(msg.data_array), data_size);
				}
				if (data_num < 500)
					msg.is_load_all = true;
				dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());

				if (data_num >= 500)
				{
					cur_num += 500;
					sql_builder sql(db_manager::getInstance().get_db_link().game_db);
					sql << _XTEXT("CALL sp_cross_ladder_load_role_info(")
						<< cur_num
						<< _XTEXT(");");

					db_query_type query;
					query.sql_str.assign(sql.c_str(), sql.size());
					query.handler = boost::bind(&ws2dp_cross_ladder_load_info_proc_end, _1, connindex, cur_num);
					db_manager::getInstance().get_db_link().game_db.add_query(query);
				}
			}
		}
	}

	void ws2dp_cross_ladder_add_role_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_add_role_info* pdata = static_cast<const ws2dp_cross_ladder_add_role_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_ladder_add_role_info(")
			<< pdata->role_info.role_guid.server_64
			<< _XTEXT(",'")
			<< pdata->role_info.role_name
			<< _XTEXT("','")
			<< pdata->role_info.server_name
			<< _XTEXT("',")
			<< pdata->role_info.gs_value;
		for (int i = 0; i < e_cross_ladder_role_info_max; ++i)
		{
			sql << _XTEXT(",") << pdata->role_info.data_array[i];
		}
		sql << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_cross_ladder_update_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_update_info* pdata = static_cast<const ws2dp_cross_ladder_update_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_ladder_update_info(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",")
			<< pdata->total_win_num
			<< _XTEXT(",")
			<< pdata->last_win_num
			<< _XTEXT(",")
			<< pdata->ladder_score
			<< _XTEXT(",")
			<< pdata->ticket_num
			<< _XTEXT(",")
			<< pdata->history_score
			<< _XTEXT(",")
			<< pdata->cur_season_join_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_cross_ladder_update_ticket_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_update_ticket_info* pdata = static_cast<const ws2dp_cross_ladder_update_ticket_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_ladder_update_ticket_info(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",")
			<< pdata->ticket_num
			<< _XTEXT(",")
			<< pdata->can_buy_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_cross_ladder_update_role_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_update_role_info* pdata = static_cast<const ws2dp_cross_ladder_update_role_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_ladder_update_role_info(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",'")
			<< pdata->role_name
			<< _XTEXT("','")
			<< pdata->server_name
			<< _XTEXT("',")
			<< pdata->role_gs
			<< _XTEXT(",")
			<< pdata->vip_level
			<< _XTEXT(",")
			<< pdata->server_id
			<< _XTEXT(",")
			<< pdata->template_id
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_cross_ladder_update_refresh_ticket_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_refresh_ticket_info* pdata = static_cast<const ws2dp_cross_ladder_refresh_ticket_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_ladder_refresh_ticket_info(")
			<< pdata->ticket_num
			<< _XTEXT(",")
			<< pdata->can_buy_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_cross_ladder_update_refresh_season_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_cross_ladder_refresh_season_info* pdata = static_cast<const ws2dp_cross_ladder_refresh_season_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_ladder_refresh_season_info(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",")
			<< pdata->ladder_score
			<< _XTEXT(",")
			<< pdata->history_score
			<< _XTEXT(",")
			<< pdata->last_score
			<< _XTEXT(",")
			<< pdata->season_reward
			<< _XTEXT(",")
			<< pdata->cur_season_join_num
			<< _XTEXT(",")
			<< pdata->last_season_join_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_cross_ladder_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_cross_ladder_common_end(db_result_type result, uint32 connindex)
	{

	}
}