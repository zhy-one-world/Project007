#include "logic/type_def.hpp"
#include "Logic/guide_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_guide.hpp"
#include "msgproc_cs.hpp"
#include <guide_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"

namespace faith
{
	void cs2dp_req_load_char_guide_record(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_guide_record_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_guide;
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_char_guide_record_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	static void cs2dp_load_role_guide_send_lua(uint32 connindex, const dp2cs_load_guide_record& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::db_proto::load_role_guide_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::db_proto::load_role_guide_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_guide_trigger_id(msgData.data_list[i].guide_trigger_id);
			db_row->set_state(msgData.data_list[i].state);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}

		msg.set_db_data(p_db->google_data, p_db->google_data_len);
		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_guide_record);

	}

	void cs2dp_req_load_char_guide_record_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		struct guide_record_db_row
		{
			guid_64 role_guid;
			s_record_info	guide_info;
		}sql_data[faith::max_guide_record_num];

		int32 row_count = result.query.data_select.row_count;
		int32 row_size = sizeof(guide_record_db_row);
		if (result.error || row_count > faith::max_guide_record_num || row_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_guide;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, row_size * row_count);

			dp2cs_load_guide_record load_guide_record_end_msg;
			load_guide_record_end_msg.role_guid = role_guid;
			load_guide_record_end_msg.unit_array_index = unit_array_index;
			load_guide_record_end_msg.data_num = 0;
			for (int32 i = 0; i < row_count; ++i)
			{
				load_guide_record_end_msg.data_list[i] = sql_data[i].guide_info;
				++(load_guide_record_end_msg.data_num);
			}

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_guide_record);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &load_guide_record_end_msg, sizeof(load_guide_record_end_msg));
			}
			else
			{
				cs2dp_load_role_guide_send_lua(connindex, load_guide_record_end_msg);
			}
		}
	}

	void cs2dp_req_save_char_guide_record(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_guide_record* pdata = static_cast<const cs2dp_save_guide_record*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_save_guide_record))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_guide_record_save(") << pdata->role_guid.server_64 << ",";
		sql << pdata->record_info.guide_trigger_id << ",'";
		sql << "(" << pdata->role_guid.server_64 << "," << pdata->record_info.guide_trigger_id << "," << pdata->record_info.state << ")";
		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_guide;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}