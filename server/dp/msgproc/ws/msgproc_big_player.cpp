/********************************************************************
created:	2016/10/27
file base:	msgproc_big_player
file ext:	cpp
author:		lxy

purpose:
*********************************************************************/
#include "logic/type_def.hpp"
#include "dbproxy_service.hpp"
#include "dbproxy_service.hpp"
#include "msgproc_big_player.hpp"
#include <big_player_msg.hpp>

namespace hld
{
	

	void ws2dp_req_load_big_player_base_info(uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_ws_big_player_info_load(") << role_guid.server_64 << _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_big_player_base_info_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_big_player_base_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		int32 table_len = result.query.data_select.row_count;
		if (result.error || table_len > 1 || sizeof(s_unit_info) != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			dp2ws_load_big_player_base_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			db_read_data(&result, &(ret_msg.role_info), sizeof(ret_msg.role_info));

			dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
		}
	}

	void ws2dp_req_load_big_player_att(uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_att_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_big_player_att_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_big_player_att_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_att_info_db);
		s_att_info_db sql_data;
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);

			dp2ws_load_big_player_att_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			db_read_data(&result, &(ret_msg.att_info), sizeof(ret_msg.att_info));

			dbproxy_service::getInstance().send_message(connindex,  &ret_msg, sizeof(ret_msg));
		}
	}

	void ws2dp_req_load_big_player_skill(uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_skill_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_big_player_skill_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_big_player_skill_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		s_skill_info_db sql_data[MAX_SKILL_NUM];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_skill_info_db);
		if (result.error || table_len > MAX_SKILL_NUM || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_big_player_skill_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(ret_msg.skill_data[i].data_ary), &(sql_data[i].data_ary), sizeof(sql_data[i].data_ary));
			}

			dbproxy_service::getInstance().send_message(connindex,  &ret_msg, sizeof(ret_msg));
		}
	}

	void ws2dp_req_load_big_player_item(uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_item_load(") << role_guid.server_64<<","<< e_bag_type_equip << _XTEXT(");");

		//db_query_type query;
		//query.sql_str.assign(sql.c_str(), sql.size());
		//query.role_guid = role_guid;
		//query.handler = boost::bind(&ws2dp_req_load_big_player_item_end, _1, connindex, role_guid, big_type);
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_get_other_player_equiping_info(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_big_player_item_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_big_player_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		s_item_info_db sql_data[role_show_item_num];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len > role_show_item_num || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_big_player_item_info_end reply_item;
			reply_item.role_guid = role_guid;
			reply_item.big_type = big_type;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_item.item_data[i]), &(sql_data[i].data_info), sizeof(s_item_info));
			}
			dbproxy_service::getInstance().send_message(connindex,  &reply_item, sizeof(reply_item));
			return;
		}
	}

	void ws2dp_req_load_big_player_buff(uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_buff_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_big_player_buff_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_big_player_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		s_buff_save_db sql_data[MAX_BUFF_DESC];
		int32	iNumber = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < iNumber * result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, iNumber * result.query.data_select.row_size);
			dp2ws_load_big_player_buff_end reply_data;
			reply_data.role_guid = role_guid;
			reply_data.big_type = big_type;
			reply_data.data_num = iNumber;
			for (int32 i = 0; i < iNumber; ++i)
			{
				memcpy(&(reply_data.buff_data[i]), &(sql_data[i].data_info), sizeof(s_buff_info));
			}
			dbproxy_service::getInstance().send_message(connindex,  &reply_data, reply_data.get_pak_length());
			return;
		}
	}
}