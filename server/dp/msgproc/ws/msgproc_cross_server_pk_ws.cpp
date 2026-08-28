/********************************************************************
	created:	2019Äê1ÔÂ17ÈÕ12:59:26
	file base:	msgproc_cross_server_pk_ws
	file ext:	cpp
	author:		zhangshuo
	
	purpose:	
*********************************************************************/
#include "msgproc_cross_server_pk_ws.hpp"
#include <cross_server_pk_msg.hpp>
#include "dbproxy_service.hpp"
#include "utility/parse_msg.h"

namespace hld
{
	void save_cross_server_pk_ws_info_lua(uint32 connindex, int32 pk_state, int32 war_index, int64 player_guid[max_island_player_num], xstring role_name[max_island_player_num], int32 is_winner[max_island_player_num], int32 server_id[max_island_player_num], int32 win_num[max_island_player_num])
	{
		if (war_index < 0 || war_index >= max_cross_server_all_class_war_count)
		{
			return;
		}
		if (player_guid[0] <= 0 || pk_state < e_cross_pk_state_16 || pk_state > e_cross_pk_state_2)
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_cross_server_pk_save(";
		sql << war_index << ",";
		sql << "'(";
		sql << war_index << ",";
		sql << player_guid[0] << ",";
		sql << pk_state << ",";
		sql << "\\\'" << role_name[0]<< "\\\',";
		sql << win_num[0] << ",";
		sql << server_id[0] << ",";
		sql << is_winner[0] << ",";
		sql << player_guid[1] << ",";
		sql << "\\\'" << role_name[1] << "\\\',";
		sql << win_num[1] << ",";
		sql << server_id[1] << ",";
		sql << is_winner[1] << ")'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_cross_server_pk_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_cross_server_pk_ws_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_cross_server_pk_msg_all request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int64 player_guid[max_island_player_num];
		xstring role_name[max_island_player_num];
		int32 is_winner[max_island_player_num];
		int32 server_id[max_island_player_num];
		int32 win_num[max_island_player_num];
	
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (i >= request.player_guid_size())
			{
				break;
			}
			player_guid[i] = request.player_guid(i);
			role_name[i] = request.role_name(i);
			is_winner[i] = request.is_winner(i);
			server_id[i] = request.server_id(i);
			win_num[i] = request.win_num(i);
		}
		save_cross_server_pk_ws_info_lua(connindex, request.pk_state(), request.war_idex(), player_guid, role_name, is_winner, server_id, win_num);
	}
	void save_cross_server_pk_ws_info_end(db_result_type result, uint32 connindex)
	{
		
	}

	void clear_cross_server_pk_ws_info_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_cross_server_pk_info_clear();";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_cross_server_pk_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_cross_server_pk_ws_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		clear_cross_server_pk_ws_info_lua(connindex);
	}

	void clear_cross_server_pk_ws_info_end(db_result_type result, uint32 connindex)
	{

	}

	void load_cross_server_pk_ws_info_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cross_server_pk_info_load_all();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_cross_server_pk_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_cross_server_pk_ws_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_cross_server_pk_ws_info_lua(connindex);
	}

	void load_cross_server_pk_ws_info_end(db_result_type result, uint32 connindex)
	{
		s_dp2ws_island_simple_msg	sql_data[max_cross_server_all_class_war_count];
		int32 data_size = sizeof(s_dp2ws_island_simple_msg);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || result.query.data_select.row_count <= 0 || data_num > max_cross_server_all_class_war_count)
		{
			dp2ws_load_cross_server_pk_msg_all reply_msg;
			reply_msg.data_num = 0;
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, reply_msg.get_pak_length());
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * data_num);
			dp2ws_load_cross_server_pk_msg_all reply_msg;
			reply_msg.data_num = data_num;
			for (int32 i = 0; i < data_num; i++)
			{
				memcpy(&(reply_msg.simple_msg[i]), &(sql_data[i]), sizeof(s_dp2ws_island_simple_msg));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, reply_msg.get_pak_length());
			return;
		}
	}
}
