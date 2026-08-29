/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   16:02
	file base:	msgproc_special_name
	file ext:	cpp
	author:		lucifer~yu
	
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_special_name.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <special_name_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"

namespace faith
{
	static void  cs2dp_load_role_special_name_send_lua(uint32 connindex, const dp2cs_load_char_special_name & msgData)
	{
		dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		cs2dp_proto::role_special_name_db msg_db;
		for (int32 i = 0; i < e_special_name_type_max; i++)
		{
			msg_db.add_data_ary(msgData.special_name_data.data_ary[i]);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_special_name);

		


	}
	void cs2dp_req_load_char_special_name(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_char_special_name msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_special_name, &msgData.special_name_data, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_special_name);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_special_name_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_special_name_load(") << role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_special_name;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_special_name_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_special_name_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_special_name_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_special_name_info_db);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			CONSOLE_ERROR("error role_guid:{} unit_array_index:{} result.error:{} table_len:{} data_size:{} row_size:{}", role_guid.server_64, unit_array_index, result.error, table_len, data_size, result.query.data_select.row_size);
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_special_name;
			error.error_result = e_error_code_db_error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size * table_len);
			dp2cs_load_char_special_name reply_data;
			reply_data.is_over = true;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.special_name_data = sql_data.data_info;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_special_name);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_char_special_name));
			}
			else
			{
				cs2dp_load_role_special_name_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_special_name, &reply_data.special_name_data, sizeof(s_special_name_info), 1);
			}
		}
	}


	bool cs2dp_save_role_special_name_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_special_name_info db_row_info;

		cs2dp_proto::role_special_name_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_row_info.data_ary[i] = msg.data_ary(i);
		}


		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_special_name, &db_row_info, sizeof(s_special_name_info), 1);

		return true;
	}

	void cs2dp_req_save_char_special_name( uint32 connindex, const void* data_ptr, size_t data_len )
	{
		const cs2dp_save_char_special_name* pdata = static_cast<const cs2dp_save_char_special_name*>(data_ptr);
		if ( NULL == pdata )
		{
			return;
		}

		if(data_len != (const_cast<cs2dp_save_char_special_name*>(pdata))->get_pak_length())
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_special_name, &pdata->name_data, sizeof(s_special_name_info), 1);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_special_name, pdata->save_type_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_special_name_save(") << pdata->role_guid.server_64 << ",'";
		int32 sqlEnd = e_special_name_type_max - 1;
		sql << "(" << pdata->role_guid.server_64 << ",";
		for (int32 j = 0; j < e_special_name_type_max; ++j)
		{
			
			if (j != sqlEnd)
			{
				sql <<pdata->name_data.data_ary[j] << ",";
			}
			else
			{
				sql << pdata->name_data.data_ary[j]<<")";
			}
		}
		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_special_name;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_special_name, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}