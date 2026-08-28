/********************************************************************
created:	2016Äê12ÔÂ12ÈÕ13:06:30
file base:	msgproc_big_player_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_big_player_ws.hpp"
#include "../../dbproxy_service.hpp"
#include <big_player_msg.hpp>
#include "msgproc_big_player.hpp"
#include "utility/parse_msg.h"

namespace hld
{
	
	void load_big_player(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_big_player_lua(connindex);
	}

	void load_big_player_detail(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_big_player_detail request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_big_player_detail_lua(connindex, request.big_type(), request.role_guid());
	}

	void save_big_player_to_sql_lua(uint32 connindex, int32 player_type[e_big_player_type_max], int64 role_guid[e_big_player_type_max])
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_big_player_save(") << "'";
		int32 tempLen = e_big_player_type_max - 1;

		for (int32 i = 0; i < e_big_player_type_max; i++)
		{
			sql << "(" << player_type[i] << ",";
			sql << role_guid[i];

			if (i != tempLen)
			{
				sql << "),";
			}
			else
			{
				sql << ")";
			}
		}
		sql << "');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_big_player_lua(uint32 connindex)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_big_player_load();");

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&load_big_player_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_big_player_detail_lua(uint32 connindex, int32 player_type, int64 role_guid)
	{
		guid_64 big_player_guid = role_guid;
		ws2dp_req_load_big_player_base_info(connindex, big_player_guid, player_type);
		ws2dp_req_load_big_player_att(connindex, big_player_guid, player_type);
		ws2dp_req_load_big_player_skill(connindex, big_player_guid, player_type);
		ws2dp_req_load_big_player_item(connindex, big_player_guid, player_type);
		ws2dp_req_load_big_player_buff(connindex, big_player_guid, player_type);
	}

	void load_big_player_end(db_result_type result, uint32 connindex)
	{
		s_big_player_db sql_data[e_big_player_type_max];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_big_player_db);
		if (result.error || table_len > e_big_player_type_max || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_big_player_end reply_data;

			for (int32 i = 0; i < e_big_player_type_max; ++i)
			{
				reply_data.data_info[i].role_guid = sql_data[i].role_guid;
				reply_data.data_info[i].player_type = sql_data[i].player_type;
			}

			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2ws_load_big_player_end));
			return;
		}
	}
	void save_big_player_to_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_big_player_to_db request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int32 	player_type[e_big_player_type_max];
		int64 	role_guid[e_big_player_type_max];

		for (int32 i = 0; i < e_big_player_type_max; i++)
		{
			if (request.player_type_size() > i)
			{
				player_type[i] = request.player_type(i);
			}
			if (request.role_guid_size() > i)
			{
				role_guid[i] = request.role_guid(i);
			}
		}
		save_big_player_to_sql_lua(connindex, player_type, role_guid);
	}
}
