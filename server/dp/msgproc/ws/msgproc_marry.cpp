/********************************************************************
file name:	msgproc_marry.hpp
created:	2017/12/07 23:32
author:		Allen_Lei
purpose:
*********************************************************************/
#include "msgproc_marry.hpp"
#include "dbproxy_service.hpp"
#include <marry_msg.hpp>
#include "parse_msg.h"
#include "md5/Base64.h"
#include "character.pb.h"

namespace faith
{
	void  req_load_marry_info(uint32 connindex, int32 start_pos)
	{ 
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_load_marry_info('")
			<< start_pos << ","
			<< max_load_num << ","
			<< "');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_marry_info_end, _1, connindex, start_pos);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}



	void ws2dp_req_load_marry_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_load_marry_info(")
			<< 0 << ","
			<< max_load_num
			<< ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_marry_info_end, _1, connindex, 0);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_marry_info_end(db_result_type result, uint32 connindex ,int32 start_pos)
	{
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(marry_record_data) ;
		if (result.error || table_len <= 0 || data_size != result.query.data_select.row_size)
		{  
			return;
		}
		else
		{ 
			int32 load_count = 0;
			bool is_load_end = false;
			if ( table_len >= max_load_num )
			{  
				load_count = max_load_num; 
			} 
			else
			{
				load_count = table_len;
				is_load_end = true;
			}

			dp2ws_load_marry_couple_info_end reply;
			memset(reply.load_data, 0, data_size * max_load_num);
			db_read_data(&result, reply.load_data, data_size * load_count);
			reply.load_num = load_count; 
			dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(dp2ws_load_marry_couple_info_end));
			if (!is_load_end)
			{
				req_load_marry_info(connindex, start_pos + load_count);
			}
			 
			return;
		}
	}

	void ws2dp_req_save_marry_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const faith::ws2dp_save_marry_couple_info* pdata = static_cast<const faith::ws2dp_save_marry_couple_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		if (pdata->is_divorce)
		{
			sql << _XTEXT("call sp_del_marry_info('") << pdata->data_one.main_couple_guid.server_64;
			sql << "');";
		}
		else
		{
			sql << _XTEXT("call sp_save_marry_info('") << pdata->data_one.main_couple_guid.server_64 << "','";
			sql << "(";
			sql << pdata->data_one.main_couple_guid.server_64 << ",\\\'"
				<< pdata->data_one.main_couple_name << "\\\',"
				<< pdata->data_one.sub_couple_guid.server_64 << ",\\\'"
				<< pdata->data_one.sub_couple_name << "\\\',"
				<< pdata->data_one.marry_state << ","
				<< pdata->data_one.save_event_time << ","
				<< pdata->data_one.main_class_template_id << ","
				<< pdata->data_one.main_level << ","
				<< pdata->data_one.sub_class_template_id << ","
				<< pdata->data_one.sub_level << ","
				<< pdata->data_one.marry_item_id << ","
				<< pdata->data_one.main_ring_level << ","
				<< pdata->data_one.sub_ring_level << ","
				<< pdata->data_one.heart_value << ")";
			sql << "');";
		}

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_save_marry_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_save_marry_info_end(db_result_type result, uint32 connindex)
	{

	}


	void ws2dp_req_load_couple_login_time(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const faith::ws2dp_marry_get_last_login_time* pdata = static_cast<const faith::ws2dp_marry_get_last_login_time*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->target_guid.server_64) << _XTEXT(".sp_role_data_load(") << pdata->target_guid.server_64 << ",'" << data_manager::get_table_name(e_data_flag_time).c_str() << _XTEXT("');");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_couple_login_time_end, _1, connindex, pdata->sender_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_couple_login_time_end(db_result_type result, uint32 connindex, const guid_64& sender_guid)
	{
		dp2ws_marry_get_last_login_time_end pak;
		pak.last_login = 0;
		int32 table_len = result.query.data_select.row_count;
		int32 table_size = result.query.data_select.row_size;
		if (result.error)
		{
			return;
		}
		else if (table_len <= 0)
		{
			//合服逻辑会删除掉一个月没有登录的玩家,防止找不到不能离婚
			pak.sender_guid = sender_guid;
			pak.last_login = 0;
			dbproxy_service::getInstance().send_message(connindex, &pak, sizeof(dp2ws_marry_get_last_login_time_end));
			return;
		}
		else
		{
			pak.sender_guid = sender_guid; 

			static s_db_data_array sql_data;
			sql_data.clear_data();
			db_read_data(&result, &sql_data, table_size);
			auto str_len = strlen(sql_data.data_array);
			xstring str_2 = Base64::Decode(sql_data.data_array, str_len);
			character_s2s_sl_time_data msg;
			msg.ParseFromArray(str_2.c_str(), str_2.size());
			if (msg.data_ary_size() > e_time_type_login_time)
			{
				pak.last_login = msg.data_ary(e_time_type_login_time);
			}
			dbproxy_service::getInstance().send_message(connindex, &pak, sizeof(dp2ws_marry_get_last_login_time_end));
			return;
		}
	}

	void ws2dp_load_marry_info_cross_server_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const faith::ws2dp_load_marry_info_cross_server* pdata = static_cast<const faith::ws2dp_load_marry_info_cross_server*>(data_ptr);
		if (nullptr == pdata)
			return;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_marry_load_marry_info_cross_server(")
			<< pdata->role_guid.server_64
			<< ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_load_marry_info_cross_server_proc_end, _1, connindex, pdata->role_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void ws2dp_load_marry_info_cross_server_proc_end(db_result_type result, uint32 connindex, const guid_64& sender_guid)
	{
		dp2ws_load_marry_info_cross_server_end msg;
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(msg.data_one);
		if (result.error || table_len != 1 || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			msg.role_guid = sender_guid;
			memset(&msg.data_one, 0, data_size);
			db_read_data(&result, &msg.data_one, data_size);
			dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_load_marry_info_cross_server_end));
			return;
		}
	}

	
}



