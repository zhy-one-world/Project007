/********************************************************************
	created:	2019年4月4日12:59:26
	file base:	msgproc_cross_server_harry_ws
	file ext:	cpp
	author:		zhangshuo
	
	purpose:	
*********************************************************************/
#include "msgproc_cross_server_harry_ws.hpp"
#include <cross_server_harry_msg.hpp>
#include "dbproxy_service.hpp"
#include "utility/parse_msg.h"
namespace faith
{
	void save_cross_server_harry_ws_info_lua(uint32 connindex, int32 server_id, int32 server_money, int32 special_money, int64 first_player_guid, int64 second_player_guid, int64 third_player_guid, xstring first_player_name)
	{
		int32 init_special_money = 0;//特殊采集不存库
		if ( server_id <= 0)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_cross_server_harry_save(";
		sql << server_id << ",";
		sql << "'(";
		sql << server_id << ",";
		sql << server_money << ",";
		sql << init_special_money << ",";
		sql << first_player_guid << ",";
		sql << second_player_guid << ",";
		sql << third_player_guid << ",";
		sql << "\\\'" << first_player_name << "\\\')'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_cross_server_harry_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_cross_server_harry_ws_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_cross_server_harry_msg_all request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_cross_server_harry_ws_info_lua(connindex, request.server_id(), request.server_money(), request.special_money(), request.first_player_guid(), request.second_player_guid(), request.third_player_guid(), request.first_player_name());
	}

	void save_cross_server_harry_ws_info_end(db_result_type result, uint32 connindex)
	{

	}

	void del_one_server_msg_func_lua(uint32 connindex, int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_harry_info_server_delete(");
		sql << server_id << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&del_one_server_msg_func_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void del_one_server_msg_func(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_del_one_server_msg request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		del_one_server_msg_func_lua(connindex, request.server_id());
	}

	void del_one_server_msg_func_end(db_result_type result, uint32 connindex)
	{

	}

	void load_cross_server_harry_ws_info_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cross_server_harry_info_load_all();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_cross_server_harry_ws_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_cross_server_harry_ws_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_cross_server_harry_ws_info_lua(connindex);
	}

	void load_cross_server_harry_ws_info_end(db_result_type result, uint32 connindex)
	{
		s_server_harry_msg sql_data[dp_save_max_server_num];
		int32 data_size = sizeof(s_server_harry_msg);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || data_num > dp_save_max_server_num)
		{
			return;
		}
		else if ( result.query.data_select.row_count <= 0 )
		{
			dp2ws_load_cross_server_harry_msg_all reply_msg;
			reply_msg.is_empty_msg = true;
			reply_msg.data_num = 0;
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, reply_msg.get_pak_length());
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * data_num);
			dp2ws_load_cross_server_harry_msg_all reply_msg;
			reply_msg.data_num = data_num;
			reply_msg.is_empty_msg = false;
			for (int32 i = 0; i < data_num; i++)
			{
				memcpy(&(reply_msg.harry_msg[i]), &(sql_data[i]), sizeof(s_server_harry_msg));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, reply_msg.get_pak_length());
			return;
		}
	}

	void load_harry_player_detail_lua(uint32 connindex, int32 player_idex, int64 role_guid, int32 server_id)
	{
		guid_64 harry_player_guid = role_guid;
		int32 big_type = player_idex;
		ws2dp_req_load_harry_player_base_info(connindex, harry_player_guid, big_type, server_id);
		ws2dp_req_load_harry_player_att(connindex, harry_player_guid, big_type, server_id);
		//ws2dp_req_load_harry_player_skill(connindex, harry_player_guid, big_type, server_id);
		ws2dp_req_load_harry_player_item(connindex, harry_player_guid, big_type, server_id);
		ws2dp_req_load_harry_player_buff(connindex, harry_player_guid, big_type, server_id);
		ws2dp_req_load_harry_player_belief(connindex, harry_player_guid, big_type, server_id);
		ws2dp_req_load_harry_player_fight_att(connindex, harry_player_guid, big_type, server_id);
	}

	void load_harry_player_detail(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_harry_player_detail request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_harry_player_detail_lua(connindex, request.player_idex(), request.role_guid(), request.server_id());
	}

	void ws2dp_req_load_harry_player_base_info(uint32 connindex, const guid_64& role_guid, int32 big_type,int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call  ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_ws_big_player_info_load(") << role_guid.server_64 << _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_harry_player_base_info_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_harry_player_base_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		int32 table_len = result.query.data_select.row_count;
		if (result.error || table_len > 1 || sizeof(s_unit_info) != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			dp2ws_load_harry_player_base_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			db_read_data(&result, &(ret_msg.role_info), sizeof(ret_msg.role_info));

			dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
		}
	}

	void ws2dp_req_load_harry_player_att(uint32 connindex, const guid_64& role_guid, int32 big_type,int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_att_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_harry_player_att_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_harry_player_att_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
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

			dp2ws_load_harry_player_att_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			db_read_data(&result, &(ret_msg.att_info), sizeof(ret_msg.att_info));

			dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
		}
	}

	void ws2dp_req_load_harry_player_skill(uint32 connindex, const guid_64& role_guid, int32 big_type,int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_skill_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_harry_player_skill_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_harry_player_skill_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
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

			dp2ws_load_harry_player_skill_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(ret_msg.skill_data[i].data_ary), &(sql_data[i].data_ary), sizeof(sql_data[i].data_ary));
			}

			dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
		}
	}

	void ws2dp_req_load_harry_player_item(uint32 connindex, const guid_64& role_guid, int32 big_type,int32 server_id)
	{
		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_item_load(") << role_guid.server_64 << "," << e_bag_type_equip << _XTEXT(");");

		//db_query_type query;
		//query.sql_str.assign(sql.c_str(), sql.size());
		//query.role_guid = role_guid;
		//query.handler = boost::bind(&ws2dp_req_load_harry_player_item_end, _1, connindex, role_guid, big_type);
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_get_other_player_fake_item_info(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_harry_player_item_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_harry_player_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		s_item_info_db sql_data[fake_item_num];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len >= fake_item_num || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_harry_player_item_info_end reply_item;
			reply_item.role_guid = role_guid;
			reply_item.big_type = big_type;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_item.item_data[i]), &(sql_data[i].data_info), sizeof(s_item_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_item, sizeof(reply_item));
			return;
		}
	}

	void ws2dp_req_load_harry_player_buff(uint32 connindex, const guid_64& role_guid, int32 big_type,int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_buff_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_harry_player_buff_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_harry_player_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		s_buff_save_db sql_data[MAX_BUFF_DESC];
		int32	iNumber = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < iNumber * result.query.data_select.row_size || iNumber > MAX_BUFF_DESC)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, iNumber * result.query.data_select.row_size);
			dp2ws_load_harry_player_buff_end reply_data;
			reply_data.role_guid = role_guid;
			reply_data.big_type = big_type;
			reply_data.data_num = iNumber;
			for (int32 i = 0; i < iNumber; ++i)
			{
				memcpy(&(reply_data.buff_data[i]), &(sql_data[i].data_info), sizeof(s_buff_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void ws2dp_req_load_harry_player_belief(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_belief_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_harry_player_belief_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_harry_player_belief_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		s_belief_info_db sql_data[e_belief_series_type_max];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, data_size);
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2ws_load_harry_player_belief_end reply_data;
			reply_data.role_guid = role_guid;
			reply_data.big_type = big_type;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.data_list[i] = sql_data[i].data_info;
				reply_data.data_num++;
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2ws_load_harry_player_belief_end));
		}
	}


	void ws2dp_req_load_harry_player_fight_att(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_fight_att_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&ws2dp_req_load_harry_player_fight_att_end, _1, connindex, role_guid, big_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_req_load_harry_player_fight_att_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type)
	{
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_arena_char_fight_att);
		s_arena_char_fight_att sql_data;
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);

			dp2ws_load_harry_player_fight_att_info_end ret_msg;
			ret_msg.role_guid = role_guid;
			ret_msg.big_type = big_type;
			db_read_data(&result, &(ret_msg.att_info), sizeof(ret_msg.att_info));

			dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
		}
	}
	}
