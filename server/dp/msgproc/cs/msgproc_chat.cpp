#include "msgproc_chat.hpp"
#include "data_manager/data_manager.hpp"
#include <chat_msg.hpp>
#include "../../dbproxy_service.hpp"
#include <char_msg.hpp>
#include "msgproc_cs.hpp"
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"

namespace faith
{
	static void  cs2dp_load_role_chat_record_send_lua(uint32 connindex, const dp2cs_load_chat_record& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_chat_record_db db_msg;
		db_msg.set_row_count(msgData.data_num);
		for (int32 i = 0; i < db_msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_chat_record_row *db_row = db_msg.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_pos(msgData.data_list[i].pos);
			db_row->set_record(msgData.data_list[i].record);
		}
		
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&db_msg);
		if (p_db == nullptr)
		{
			return;
		}

		msg.set_db_data(p_db->google_data, p_db->get_google_data_len());

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_chat_record);

	}
	void cs2dp_req_load_chat_record(uint32 connindex, const guid_64 & role_guid, const guid_64 & up_role_guid, const int32 & unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_load_chat_record msgData;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_chat_record, msgData.data_list, msgData.data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_chat_record);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_chat_record_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_chat_record_load('") << up_role_guid.server_64 << _XTEXT("');");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_chat_record;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_chat_record_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_chat_record_end(db_result_type result, uint32 connindex, const guid_64 & role_guid, const int32 & unit_array_index)
	{
		s_chat_record_db sql_data[chat_record_max_num];
		int32  table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_chat_record_db);
		if (result.error || table_len > chat_record_max_num || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_chat_record;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2cs_load_chat_record reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(reply_data.data_list + i, (xchar *)(&sql_data[i].chat_record), sizeof(s_chat_record));
			}

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_chat_record);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				cs2dp_load_role_chat_record_send_lua(connindex, reply_data);

			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_chat_record, reply_data.data_list, reply_data.data_num * sizeof(s_chat_record), reply_data.data_num);
			}
		}
	}

	bool cs2dp_save_role_chat_record_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		faith::cs2dp_proto::role_chat_record_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_chat_record));
		if (p_data == nullptr)
		{
			return false;
		}

		s_chat_record *p_row = (s_chat_record *)p_data;

		for (int32 i = 0; i < row_count; i++)
		{
			faith::cs2dp_proto::role_chat_record_row one_row = msg.row_data(i);

			p_row->pos = one_row.pos();
			parse_msg::getInstance().my_memcopy(p_row->record, chat_record_max_len, one_row.record().c_str(), one_row.record().size());
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_chat_record, p_data, row_count * sizeof(s_chat_record), row_count);

		return true;


	}


	void cs2dp_req_save_chat_record(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const cs2dp_save_chat_record* pdata = static_cast<const cs2dp_save_chat_record*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != (const_cast<cs2dp_save_chat_record*>(pdata))->get_pak_length())
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_chat_record, pdata->data_list, pdata->data_num * sizeof(s_chat_record), pdata->data_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_chat_record, pdata->save_type_ex);

			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_chat_record_save('") << pdata->role_guid.server_64 << "','";
		int32 tempLen = pdata->data_num - 1;

		for (int32 i = 0; i < pdata->data_num; i++)
		{
			sql << "(" << pdata->role_guid.server_64 << ",";

			///////////////////////////
			sql << pdata->data_list[i].pos << ",";
			sql << "\\'";
			sql << pdata->data_list[i].record << "";
			sql << "\\'";

		
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
		query.logic_id = e_data_flag_chat_record;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_chat_record, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}
