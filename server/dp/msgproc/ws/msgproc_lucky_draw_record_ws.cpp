/********************************************************************
created:	2016年12月12日13:06:30
file base:	msgproc_big_player_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_lucky_draw_record_ws.hpp"
#include "../../dbproxy_service.hpp"
#include <lucky_draw_record_msg.hpp>
#include "msgproc_big_player.hpp"

namespace faith
{
	
	void del_draw_info_from_db(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_del_draw_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		del_draw_info_from_db_lua(connindex, request.info_guid());
	}
	void add_draw_info_to_db(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_add_new_draw_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		add_draw_info_to_db_lua(connindex, request.info_guid(), request.role_guid(), request.role_name(), request.item_template_id(), request.get_type(), request.record_type(), request.record_time());
	}

	void load_draw_info_by_db(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_draw_info_by_db_lua(connindex);
	}

	void load_draw_rocord_info_end(db_result_type result, uint32 connindex)
	{
		s_hope_hunt_record_info sql_data[draw_record_total_num];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_hope_hunt_record_info);
		if (result.error || table_len > draw_record_total_num || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_draw_record_by_db_end reply_data;
			reply_data.data_num = table_len;

			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.data_info[i]), &(sql_data[i]), sizeof(s_hope_hunt_record_info));
			}

			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}
	void del_draw_info_from_db_lua(uint32 connindex, int64 info_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_lucky_draw_record_ws_del(") << info_guid << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void add_draw_info_to_db_lua(uint32 connindex, int64 info_guid, int64 role_guid, xstring role_name, int32 item_template_id, int32 get_type, int32 record_type, int64 record_time)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_lucky_draw_record_ws_add(") << info_guid << ",";

		sql << "'(" << info_guid << "," << role_guid << ",";
		sql << "\\\'" << role_name << "\\\',";
		sql << item_template_id << ",";
		sql << get_type << ",";
		sql << record_type << ",";
		sql << record_time;
		sql << ")";
		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_draw_info_by_db_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_lucky_draw_record_ws_load();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_draw_rocord_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}
