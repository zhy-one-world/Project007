/***********************************************
* created	: 2020年8月20日
* file base : msgproc_element_war_info_ws
* file ext  : cpp
* author    : 赵玉明
* purpose   : 元素争霸
************************************************/
#include "msgproc_element_war_info_ws.hpp"
#include <element_heart_msg.hpp>
#include "dbproxy_service.hpp"
namespace faith
{
	void save_element_war_add_role_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_add_role_info* pdata = static_cast<const ws2dp_element_war_add_role_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_element_war_add_role_info(")
			<< pdata->role_info.role_guid.server_64
			<< _XTEXT(",'")
			<< pdata->role_info.role_name
			<< _XTEXT("','")
			<< pdata->role_info.server_name
			<< _XTEXT("',")
			<< pdata->role_info.gs_value;
		for (int i = 0; i < e_element_war_role_info_max; ++i)
		{
			sql << _XTEXT(",") << pdata->role_info.data_array[i];
		}
		sql << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}


	void save_element_war_update_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{	
		const ws2dp_element_war_update_info* pdata = static_cast<const ws2dp_element_war_update_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
			
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_element_war_update_info(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",")
			<< pdata->total_win_num
			<< _XTEXT(",")
			<< pdata->last_win_num
			<< _XTEXT(",")
			<< pdata->score_num
			<< _XTEXT(",")
			<< pdata->ticket_num
			<< _XTEXT(",")
			<< pdata->history_score
			<< _XTEXT(",")
			<< pdata->cur_season_join_num
			<< _XTEXT(",")
			<< pdata->have_buy_num
			<< _XTEXT(",")
			<< pdata->join_num
			<< _XTEXT(");");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}


	void save_element_war_refresh_season_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_refresh_season_info* pdata = static_cast<const ws2dp_element_war_refresh_season_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_element_war_refresh_season_info(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",")
			<< pdata->score_num
			<< _XTEXT(",")
			<< pdata->history_score
			<< _XTEXT(",")
			<< pdata->last_score
			<< _XTEXT(",")
			<< pdata->last_season
			<< _XTEXT(",")
			<< pdata->season_reward
			<< _XTEXT(",")
			<< pdata->cur_season_join_num
			<< _XTEXT(",")
			<< pdata->last_season_join_num
			<< _XTEXT(");");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_element_war_update_role_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_update_role_info* pdata = static_cast<const ws2dp_element_war_update_role_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_element_war_update_role_info(")
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
			<< pdata->role_level
			<< _XTEXT(",")
			<< pdata->server_id
			<< _XTEXT(",")
			<< pdata->template_id
			<< _XTEXT(",")
			<< pdata->can_buy_num
			<< _XTEXT(");");


		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_element_war_refresh_ticket_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_refresh_ticket_info* pdata = static_cast<const ws2dp_element_war_refresh_ticket_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_element_war_refresh_ticket_info(")
			<< pdata->ticket_num
			<< _XTEXT(",")
			<< pdata->have_buy_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_element_war_refresh_mission_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_refresh_mission_info* pdata = static_cast<const ws2dp_element_war_refresh_mission_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_element_war_refresh_mission(")
			<< pdata->role_guid.server_64
			<< _XTEXT(",")
			<< pdata->mission1
			<< _XTEXT(",")
			<< pdata->mission2
			<< _XTEXT(",")
			<< pdata->mission3
			<< _XTEXT(",")
			<< pdata->mission4
			<< _XTEXT(",")
			<< pdata->mission1_have_num
			<< _XTEXT(",")
			<< pdata->mission2_have_num
			<< _XTEXT(",")
			<< pdata->mission3_have_num
			<< _XTEXT(",")
			<< pdata->mission4_have_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_element_war_refresh_clear_mission_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_clear_mission_info* pdata = static_cast<const ws2dp_element_war_clear_mission_info*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_element_war_clear_mission");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_element_war_common_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
		
	}

	void ws2dp_element_war_common_end(db_result_type result, uint32 connindex)
	{
	}

	void Load_element_war_load_info(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_element_war_load_info* pdata = static_cast<const ws2dp_element_war_load_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		
		int32 cur_num = 0;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_element_war_load_role_info(")
			<< cur_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&Load_element_war_load_info_end, _1, connindex, cur_num);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	

	void Load_element_war_load_info_end(db_result_type result, uint32 connindex, int32 cur_num)
	{
		if (result.error)
		{
			return;
		}		
		else
		{
			int32 data_num = result.query.data_select.row_count;
			dp2ws_element_war_load_role_info msg;
			if (sizeof(s_element_war_role_info) == result.query.data_select.row_size)
			{
				if (data_num > 0)
				{
					int32 data_size = result.query.data_select.row_size * data_num;
					db_read_data(&result, &(msg.data_array), data_size);
					msg.cur_data_num = data_num;
				}
				if (data_num < 500)
				{
					msg.is_load_all = true;
				}
				dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());
				if (data_num >= 500)
				{
					cur_num += 500;
					sql_builder sql(db_manager::getInstance().get_db_link().game_db);
					sql << _XTEXT("CALL sp_element_war_load_role_info(")
						<< cur_num
						<< _XTEXT(");");

					db_query_type query;
					query.sql_str.assign(sql.c_str(), sql.size());
					query.handler = boost::bind(&Load_element_war_load_info_end, _1, connindex, cur_num);
					db_manager::getInstance().get_db_link().game_db.add_query(query);
				}
			}
		}
	}

}