/********************************************************************
created:	2016Äê8ÔÂ16ÈÕ 11:08:08
file base:	msgproc_pokedex
file ext:	cpp
author:		XingLong

purpose:
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_pokedex.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <pokedex_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"

namespace faith
{
	static void cs2dp_load_role_pokedex_send_lua(uint32 connindex, const dp2cs_load_char_pokedex & msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_pokedex_db msg_db;
		msg_db.set_row_count(msgData.data_num);

		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::cs2dp_proto::role_pokedex_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			for (int32 j = 0;  j < msgData.data_num; j++)
			{
				db_row->add_data_ary(msgData.data_list[i].data_ary[j]);
			}
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_pokedexs);

	}
	void cs2dp_req_load_char_pokedex(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_load_char_pokedex msgData;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_pokedex, msgData.data_list, msgData.data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_pokedexs);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_pokedex_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_pokedex_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_pokedex;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_pokedex_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_pokedex_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		char_pokedex_db_info sql_data[POKEDEX_NUM_MAX];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_pokedex;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, data_size /** table_len*/);
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2cs_load_char_pokedex reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			for (int32 i = 0; i < table_len; ++i)
			{
				for (int32 j = 0; j < e_pokedex_Info_max; ++j)
				{
					reply_data.data_list[i].data_ary[j] = sql_data[i].pokedex_data[j];
				}
			}
			reply_data.data_num = table_len;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_pokedexs);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_char_pokedex));
			}
			else
			{
				cs2dp_load_role_pokedex_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_pokedex, reply_data.data_list, reply_data.data_num * sizeof(s_pokedex_info), reply_data.data_num);
			}
		}
	}


	bool cs2dp_save_role_pokedex_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		faith::cs2dp_proto::role_pokedex_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_pokedex_info));
		if (p_data == nullptr)
		{
			return false;
		}

		s_pokedex_info *p_row = (s_pokedex_info *)p_data;

		for (int32 i = 0; i < row_count; i++)
		{
			faith::cs2dp_proto::role_pokedex_row one_row = msg.row_data(i);
			for (int32 j = 0; j < one_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = one_row.data_ary(j);
			}
			for (int32 k = 0; k < one_row.collect_num_size(); k++)
			{
				p_row->collect_num[k] = one_row.collect_num(k);
			}
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_pokedex, p_data, row_count * sizeof(s_pokedex_info), row_count);

		return true;
	}




	void cs2dp_req_save_char_pokedex(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_pokedex* pdata = static_cast<const cs2dp_save_char_pokedex*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_pokedex, pdata->data_list, pdata->data_num * sizeof(s_pokedex_info), pdata->data_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_pokedex, pdata->save_type_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_pokedex_save(") << pdata->role_guid.server_64 << ",'";
		int32 tempLen = pdata->data_num - 1;
		int32 sql_pro_End = e_pokedex_Info_max -1;

		for (int32 i = 0; i < pdata->data_num; i++)
		{
			sql << "(" << pdata->role_guid.server_64 << ",";
			for (int32 j = 0; j < e_pokedex_Info_max; ++j)
			{
				if (j != sql_pro_End)
				{
					sql << pdata->data_list[i].data_ary[j] << ",";
				}
				else
				{
					sql << pdata->data_list[i].data_ary[j];
				}
			}
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
		query.logic_id = e_data_flag_pokedex;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_pokedex, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	static void cs2dp_load_role_tinder_send_lua(uint32 connindex, const dp2cs_load_char_tinder& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_tinder_db  msg_db;
		for (int32 i = 0; i < e_tinder_info_max; i++)
		{
			msg_db.add_data_ary(msgData.tinder_data.data_ary[i]);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_tinder);


	}
	void cs2dp_req_load_char_tinder(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_load_char_tinder msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_tinder, &msgData.tinder_data, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_tinder);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_tinder_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_tinder_load(") << up_role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());

			query.role_guid = role_guid;
			query.logic_id = e_data_flag_tinder;
			query.handler = boost::bind(&cs2dp_req_load_char_tinder_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_tinder_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_tinder_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_tinder;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{

			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2cs_load_char_tinder reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.tinder_data = sql_data.data_info;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_tinder);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_char_tinder));
			}
			else
			{
				cs2dp_load_role_tinder_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_tinder, &reply_data.tinder_data, sizeof(s_tinder_info), 1);
			}
		}
	}

	bool cs2dp_save_role_tinder_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_tinder_info db_row_info;

		faith::cs2dp_proto::role_tinder_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}
		for (int32 i =0 ; i < msg.data_ary_size();i++)
		{
			db_row_info.data_ary[i] = msg.data_ary(i);
		}

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_tinder, &db_row_info, sizeof(s_tinder_info), 1);

		return true;
	}

	void cs2dp_req_save_char_tinder(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_tinder* pdata = static_cast<const cs2dp_save_char_tinder*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_tinder, &pdata->tinder_data, sizeof(s_tinder_info), 1);
			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_tinder, pdata->save_type_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_tinder_save(") << pdata->role_guid.server_64 << ",'";
		int32 sqlEnd = e_tinder_info_max - 1;

		sql << "(" << pdata->role_guid.server_64 << ",";
		for (int32 j = 0; j < e_tinder_info_max; ++j)
		{
			if (j != sqlEnd)
			{
				sql << pdata->tinder_data.data_ary[j] << ",";
			}
			else
			{
				sql << pdata->tinder_data.data_ary[j];
			}
		}
		sql << ")";
		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_tinder;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_tinder, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

}