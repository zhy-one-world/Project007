/********************************************************************
created: 2016年9月22日13:56:28
file base: convert
file ext: cpp
author: zhangminghai

purpose:dp about role's convert
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_convert.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <convert_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"


namespace faith
{
	static  void cs2dp_load_role_convert_send_lua(uint32 connindex, const dp2cs_load_char_convert& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_convert_db msg_db;
		for (int32 i = 0; i < e_convert_type_max; i++)
		{
			msg_db.add_data_ary(msgData.data_list[i]);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_convert);

	}

	void cs2dp_req_load_char_convert(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_char_convert msgData;
		int32 data_num = e_convert_type_max;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_convert, msgData.data_list, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_convert);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_convert_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_convert_load(") << role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_convert;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_convert_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}


	void cs2dp_req_load_char_convert_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{

		s_convert_info_db sql_data[e_convert_type_max];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_convert_info_db);
		if (result.error || table_len > e_convert_type_max || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_convert;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size * table_len);
			dp2cs_load_char_convert reply_data;
			reply_data.is_over = true;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			for (int32 i = 0; i < table_len;++i)
			{
				reply_data.data_list[i] = sql_data[i].times;
			}
			
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_convert);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_char_convert));
			}
			else
			{
				cs2dp_load_role_convert_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_convert, reply_data.data_list, sizeof(reply_data.data_list), e_convert_type_max);
			}
		}

	}


	bool cs2dp_save_role_convert_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		int32 db_row[e_convert_type_max] = { 0 };

		faith::cs2dp_proto_role_convert_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0 ;i < msg.data_ary_size() && i < e_convert_type_max; i++)
		{
			db_row[i] = msg.data_ary(i);
		}


		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_convert, db_row, sizeof(db_row), e_convert_type_max);

		return true;
	}




	void cs2dp_req_save_char_convert(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_convert* pdata = static_cast<const cs2dp_save_char_convert*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_save_char_convert))
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_convert, pdata->data_list, sizeof(pdata->data_list), e_convert_type_max);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_convert, pdata->save_type_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_convert_save(") << pdata->role_guid.server_64 << ",'";

		
		for (int32 i = 0; i < e_convert_type_max - 1; ++i)
		{
			sql << "(" << pdata->role_guid.server_64 << ",";
			sql << i << _XTEXT(",");
			sql << pdata->data_list[i] << "),";
		}
		sql << "(" << pdata->role_guid.server_64 << ",";
		sql << e_convert_type_max - 1 << _XTEXT(",");
		sql<< pdata->data_list[e_convert_type_max - 1] << ")'";
		sql << ");";
		db_query_type query;
		query.logic_id = e_data_flag_convert;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_convert, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}