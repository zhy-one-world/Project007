/********************************************************************
created:	2015Äê7ÔÂ24ÈÕ14:13:03
file base:	msgproc_treasure
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/
#include "../../dbproxy_service.hpp"
#include "msgproc_treasure.hpp"
#include "data_manager/data_manager.hpp"
#include <treasure_msg.hpp>
#include <char_msg.hpp>


namespace faith
{
	
	void cs2dp_req_load_char_treasure(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_treasure_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&cs2dp_req_load_char_treasure_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_char_treasure_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_treasure_info_db sql_data[treasure_prize_record_num];
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
			dp2cs_load_char_treasure reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.data_list[i] = sql_data[i].data_info;
				reply_data.data_num++;
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
		}
	}

	void cs2dp_req_save_char_treasure(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_treasure* pdata = static_cast<const cs2dp_save_char_treasure*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_treasure_save(") << pdata->role_guid.server_64 << ",'";
		int32 tempLen = pdata->data_num - 1;
		int32 sqlEnd = e_treasure_record_max - 1;
		for (int32 i = 0; i < pdata->data_num; i++)
		{
			sql << "(" << pdata->role_guid.server_64 << ",";
			for (int32 j = 0; j < e_treasure_record_max; ++j)
			{
				if (j != sqlEnd)
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
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}