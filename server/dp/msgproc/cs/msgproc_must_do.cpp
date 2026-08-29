/********************************************************************
	created:	2016年9月8日11:18:14
	file base:	msgproc_show
	file ext:	cpp
	author:		lxy
	
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_must_do.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <item_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"

namespace faith
{
	static void cs2dp_role_yesterday_must_do_remain_send_lua(uint32 connindex, const dp2cs_load_character_yesterday_remain_must_do & msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_yesterday_must_do_remain_db msg_db;
		msg_db.set_row_count(resource_find_back_day);
	
		for (int32 i  = 0; i < resource_find_back_day; i++)
		{
			faith::cs2dp_proto::role_yesterday_must_do_remain_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_role_guid(msgData.data_info[i].role_guid.server_64);
			db_row->set_role_exp_level_yesterday(msgData.data_info[i].role_exp_level_yesterday);
			db_row->set_save_date(msgData.data_info[i].save_date);
			db_row->set_role_vip_level_yesterday(msgData.data_info[i].role_vip_level_yesterday);
			db_row->set_distance_today_day_count(msgData.data_info[i].distance_today_day_count);
			db_row->set_role_arena_rank(msgData.data_info[i].role_arena_rank);
			for (int32 j = 0 ; j < e_daily_must_do_typ_max; j++)
			{
				db_row->add_data_ary(msgData.data_info[i].data_ary[j]);
			}
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_yesterday_must_do);

	}
	

	void cs2dp_req_load_char_yesterday_must_do_remain(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_character_yesterday_remain_must_do msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_yesterday_must_do_remain, &msgData.data_info, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_yesterday_must_do);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_role_yesterday_must_do_remain_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_yesterday_must_do_remain_load(") << role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.role_guid = role_guid;
			query.logic_id = e_data_flag_yesterday_must_do_remain;
			query.handler = boost::bind(&cs2dp_req_load_char_yesterday_must_do_remain_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_yesterday_must_do_remain_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_unit_yesterday_must_do_remain sql_data[3];
		//s_item_info_db sql_data[MAX_ITEM];
		int32 data_cout = result.query.data_select.row_count;
		int32 data_size = sizeof(s_unit_yesterday_must_do_remain);
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_result = result.error;
			error.error_code = e_data_flag_yesterday_must_do_remain;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size * data_cout);

			dp2cs_load_character_yesterday_remain_must_do reply_data;

			memcpy(&reply_data.data_info, &sql_data,data_size * data_cout);
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_yesterday_must_do);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			}
			else
			{
				cs2dp_role_yesterday_must_do_remain_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_yesterday_must_do_remain, &reply_data.data_info, sizeof(s_unit_yesterday_must_do_remain) * resource_find_back_day, resource_find_back_day);
			}
		}
	}

	bool cs2dp_save_role_yesterday_must_do_remain_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		faith::cs2dp_proto::role_yesterday_must_do_remain_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_unit_yesterday_must_do_remain));
		if (p_data == nullptr)
		{
			return false;
		}

		s_unit_yesterday_must_do_remain *p_row = (s_unit_yesterday_must_do_remain *)p_data;

		for (int32 i = 0; i < row_count; i++)
		{
			faith::cs2dp_proto::role_yesterday_must_do_remain_row one_row = msg.row_data(i);
			p_row->role_guid.server_64 = one_row.role_guid();
			p_row->role_exp_level_yesterday = one_row.role_exp_level_yesterday();
			p_row->save_date = one_row.save_date();
			p_row->role_vip_level_yesterday = one_row.role_vip_level_yesterday();
			p_row->distance_today_day_count = one_row.distance_today_day_count();
			p_row->role_arena_rank = one_row.role_arena_rank();
			for (int32 j = 0; j < one_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = one_row.data_ary(j);
			}
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_yesterday_must_do_remain, p_data, row_count * sizeof(s_unit_yesterday_must_do_remain), row_count);

		return true;
	}

	void cs2dp_req_save_yesterday_must_do_remain(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_save_character_yesterday_remain_must_do))
		{
			return;
		}
		const cs2dp_save_character_yesterday_remain_must_do* pdata = static_cast<const cs2dp_save_character_yesterday_remain_must_do*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_yesterday_must_do_remain, &pdata->data_info, sizeof(s_unit_yesterday_must_do_remain) * resource_find_back_day, resource_find_back_day);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_yesterday_must_do_remain, pdata->save_type_ex);
			}
		}
		int32 tempcount = 0;
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			if (!pdata->data_info[i].role_guid.is_valid())
			{
				continue;
			}
			tempcount++;
		}
		// 更新角色信息表
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_yesterday_must_do_remain_save(")
			<< pdata->role_guid.server_64 << ",'";
		

		for (int32 find_back_day = 0; find_back_day < resource_find_back_day; find_back_day++)
		{
			if (!pdata->data_info[find_back_day].role_guid.is_valid())
			{
				continue;
			}
			tempcount--;
			sql << "(";
			sql << pdata->data_info[find_back_day].role_guid.server_64 << ","
				<< pdata->data_info[find_back_day].role_exp_level_yesterday << ","
				<< pdata->data_info[find_back_day].save_date << ","
				<< pdata->data_info[find_back_day].role_vip_level_yesterday << ","
				<< pdata->data_info[find_back_day].distance_today_day_count << ","
			    << pdata->data_info[find_back_day].role_arena_rank << ",";

			int32 data_num = e_daily_must_do_typ_max - 1;
			for (int32 i = 0; i < e_daily_must_do_typ_max; ++i)
			{
				sql << pdata->data_info[find_back_day].data_ary[i];
				if (i != data_num)
				{
					sql << ",";
				}
				else
				{
					sql << ")";
				}
			}
			if (find_back_day < resource_find_back_day - 1 && tempcount != 0)
			{
				sql << ",";
			}
			
		}
			sql << "');";
			db_query_type query;
			query.logic_id = e_data_flag_yesterday_must_do_remain;
			query.role_guid = pdata->role_guid;
			query.sql_str.assign(sql.c_str(), sql.size());

			if (pdata->save_type_ex == e_save_data_type_exit_game)
			{
				query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_yesterday_must_do_remain, pdata->save_type_ex, sql.c_str());
			}
			db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	static void cs2dp_load_role_daily_must_do_count_send_lua(uint32 connindex, const dp2cs_load_character_daily_must_do_count & msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_daily_must_do_count_db msg_db;
		for (int32 i = 0; i < e_daily_must_do_typ_max; i++)
		{
			msg_db.add_data_ary(msgData.data_info.data_ary[i]);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_daily_must_do_count);

	}

	void cs2dp_req_load_char_daily_must_do_count(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_character_daily_must_do_count msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_daily_must_do_count, &msgData.data_info, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_daily_must_do_count);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_daily_must_do_count_send_lua(connindex, msgData);

			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_daily_must_do_count_load(") << role_guid.server_64 << _XTEXT(");");


			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.role_guid = role_guid;
			query.logic_id = e_data_flag_daily_must_do_count;
			query.handler = boost::bind(&cs2dp_req_load_char_daily_must_do_count_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}
	void cs2dp_req_load_char_daily_must_do_count_end(db_result_type result, uint32 connindex, const guid_64 & role_guid, const int32& unit_array_index)
	{
		s_role_daily_must_do_count sql_data;

		int32 data_size = sizeof(s_role_daily_must_do_count);
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_result = result.error;
			error.error_code = e_data_flag_daily_must_do_count;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);

			dp2cs_load_character_daily_must_do_count reply_data;

			memcpy(&reply_data.data_info, &sql_data, sizeof(s_role_daily_must_do_count));
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_daily_must_do_count);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_character_daily_must_do_count));
			}
			else
			{
				cs2dp_load_role_daily_must_do_count_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_daily_must_do_count, &reply_data.data_info, sizeof(reply_data.data_info), 1);
			}
		}
	}

	bool cs2dp_save_role_daily_must_do_count_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}

		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_role_daily_must_do_count db_row_info;

		faith::cs2dp_proto_role_daily_must_do_count_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_row_info.data_ary[i] = msg.data_ary(i);
		}
		db_row_info.role_guid.server_64 = role_id;

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_daily_must_do_count, &db_row_info, sizeof(s_role_daily_must_do_count), 1);

		return true;
	}
	void cs2dp_req_save_daily_must_do_count(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2dp_save_character_daily_must_do_count))
		{
			return;
		}
		const cs2dp_save_character_daily_must_do_count* pdata = static_cast<const cs2dp_save_character_daily_must_do_count*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_daily_must_do_count, &pdata->data_info, sizeof(pdata->data_info), 1);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_daily_must_do_count, pdata->save_type_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_daily_must_do_count_save(") << pdata->role_guid.server_64 << ",";

		sql << "'(";
		sql << pdata->role_guid.server_64 << ",";
		int32 data_num = e_daily_must_do_typ_max - 1;
		for (int32 i = 0; i < e_daily_must_do_typ_max; ++i)
		{
			sql << pdata->data_info.data_ary[i];
			if (i != data_num)
			{
				sql << ",";
			}
			else
			{
				sql << ")'";
			}
		}

		sql << ")";
		db_query_type query;
		query.logic_id = e_data_flag_daily_must_do_count;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_daily_must_do_count, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}