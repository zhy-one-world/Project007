/********************************************************************
created:	2016/09/19
created:	19:9:2016   20:02
file base:	msgproc_worship_target
file ext:	cpp
author:		zhangminghai

purpose:
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_worship_target.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <ranking_msg.hpp>
#include <Logic/ranking_def.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"

namespace hld
{
	static void cs2dp_role_worship_send_lua(uint32 connindex, const dp2cs_load_worship_target & msgData)
	{

		hld::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		hld::cs2dp_proto::role_worship_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			hld::cs2dp_proto::role_worship_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_ranking_index(msgData.data_list[i].ranking_index);
			db_row->set_target_guid(msgData.data_list[i].target_guid.server_64);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_worship_target);


	}
	

	void cs2dp_req_load_char_worship_target(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_worship_target msgData;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_worship, msgData.data_list, msgData.data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_worship_target);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_role_worship_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_worship_target_load(") << role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_worship;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_worship_target_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_worship_target_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		worship_record_db sql_data[DaliyWorshipTime];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(worship_record_db);
		if (result.error || table_len > DaliyWorshipTime || data_size != result.query.data_select.row_size)
		{
			CONSOLE_ERROR("error role_guid:{} unit_array_index:{} result.error:{} table_len:{} data_size:{} row_size:{}", role_guid.server_64, unit_array_index, result.error, table_len, data_size, result.query.data_select.row_size);
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_worship;
			error.error_result = e_error_code_db_error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);
			dp2cs_load_worship_target reply_data;
			reply_data.is_over = true;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.data_list[i].ranking_index = sql_data[i].ranking_index;
				reply_data.data_list[i].target_guid = sql_data[i].target_guid;
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_worship_target);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				cs2dp_role_worship_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_worship, reply_data.data_list, reply_data.data_num * sizeof(worship_record), reply_data.data_num);
			}
		}
	}



	bool cs2dp_save_role_worship_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		hld::cs2dp_proto::role_worship_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(worship_record));
		if (p_data == nullptr)
		{
			return false;
		}

		worship_record *p_row = (worship_record *)p_data;

		for (int32 i = 0; i < row_count; i++)
		{
			hld::cs2dp_proto::role_worship_row one_row = msg.row_data(i);

			p_row->ranking_index = one_row.ranking_index();
			p_row->target_guid.server_64 = one_row.target_guid();
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_worship, p_data, row_count * sizeof(worship_record), row_count);

		return true;
	}

	void cs2dp_req_save_char_worship_target(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_worship_target* pdata = static_cast<const cs2dp_save_char_worship_target*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != (const_cast<cs2dp_save_char_worship_target*>(pdata))->get_pak_length())
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_worship, pdata->data_list, pdata->data_num * sizeof(worship_record), pdata->data_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_worship, pdata->save_type_ex);
			}
		}
		int32 data_num = pdata->data_num;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_worship_target_save(") << pdata->role_guid.server_64 << ",'";

		for (int32 i = 0; i < data_num;++i)
		{
			sql << _XTEXT("(") << pdata->role_guid.server_64 << ","
				<< pdata->data_list[i].ranking_index << ",";
			if (i == data_num - 1)
			{
				sql<< pdata->data_list[i].target_guid.server_64 << ")";
			}
			else
			{
				sql << pdata->data_list[i].target_guid.server_64 << "),";
			}
		}
		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_worship;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_worship, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	//void cs2dp_req_save_role_ranking(uint32 connindex, const void* data_ptr, size_t data_len)
	//{
	///*
	//		const cs2dp_save_role_ranking* pdata = static_cast<const cs2dp_save_role_ranking*>(data_ptr);
	//		if (NULL == pdata)
	//		{
	//			return;
	//		}
	//		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

	//		for (int32 i = 0; i < ERankingInfo_max; ++i)
	//		{
	//			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_ranking_save(" << pdata->role_guid.server_64 << ",'(") << pdata->role_guid.server_64 << ",\\\'" << pdata->role_name << "\\\'" << ",";
	//		}

	//		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_ranking_save("<< pdata->role_guid.server_64<<",'(") << pdata->role_guid.server_64 << ",\\\'"<<pdata->role_name<<"\\\'" <<",";
	//		int32 data_num = ERankingInfo_max * 2 - 1;

	//		for (int32 i = 0; i < ERankingInfo_max; ++i)
	//		{

	//		}

	//		for (int32 i = 0; i < data_num; ++i)
	//		{
	//			sql << pdata->ranking_value[i] << ",";
	//		}
	//		sql << pdata->ranking_value[data_num] << ")');";

	//		db_query_type query;
	//		query.sql_str.assign(sql.c_str(), sql.size());
	//		query.handler = boost::bind(&cs2dp_req_save_role_ranking_end, _1, connindex, pdata->role_guid);
	//		db_manager::getInstance().get_db_link().game_db.add_query(query);*/
	//}
	//void cs2dp_req_save_role_ranking_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	//{

	//}
}