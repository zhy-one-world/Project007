#include "msgproc_appearance_info.h"
#include "dbproxy_service.hpp"
#include <appearance_info_msg.hpp>

namespace faith
{
	void save_appearance_info_to_id(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_save_appearance_info_to_id* pdata = static_cast<const ws2dp_save_appearance_info_to_id*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(ws2dp_save_appearance_info_to_id))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_save_appearance_info_to_id(";
		sql << "\'" << pdata->appearance_info << "\'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_appearance_info_to_id_end, _1, connindex, pdata->client_uid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_appearance_info_to_id_end(db_result_type result, uint32 connindex, s_client_uid client_uid)
	{
		struct
		{
			int32 appearance_id;
			xchar appearance_info[MAX_APPEARANCE_INFO_LENGTH+1];
		} sql_data;
		int32 data_size = sizeof(sql_data);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || result.query.data_select.row_count != 1)
		{
			return;
		}
		else
		{
			memset(&sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * data_num);
			dp2ws_save_appearance_info_to_id_end reply_msg;
			reply_msg.client_uid = client_uid;
			reply_msg.appearance_id = sql_data.appearance_id;
			memcpy(&reply_msg.appearance_info, &sql_data.appearance_info, MAX_APPEARANCE_INFO_LENGTH);
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, sizeof(reply_msg));
			return;
		}
	}

	void load_appearance_info_by_id(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_load_appearance_info_by_id* pdata = static_cast<const ws2dp_load_appearance_info_by_id*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(ws2dp_load_appearance_info_by_id))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_load_appearance_info_by_id(";
		sql << pdata->appearance_id;
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_appearance_info_by_id_end, _1, connindex, pdata->client_uid, pdata->appearance_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_appearance_info_by_id_end(db_result_type result, uint32 connindex, s_client_uid client_uid, int32 appearance_id)
	{
		struct
		{
			int32 appearance_id;
			xchar appearance_info[MAX_APPEARANCE_INFO_LENGTH+1];
		} sql_data;
		int32 data_size = sizeof(sql_data);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || result.query.data_select.row_count != 1)
		{
			memset(&sql_data, 0, sizeof(sql_data));
			dp2ws_load_appearance_info_by_id_end reply_msg;
			reply_msg.client_uid = client_uid;
			reply_msg.appearance_id = appearance_id;
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, sizeof(reply_msg));
			return;
		}
		else
		{
			memset(&sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * data_num);
			dp2ws_load_appearance_info_by_id_end reply_msg;
			reply_msg.client_uid = client_uid;
			reply_msg.appearance_id = sql_data.appearance_id;
			memcpy(&reply_msg.appearance_info, &sql_data.appearance_info, MAX_APPEARANCE_INFO_LENGTH);
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, sizeof(reply_msg));
			return;
		}
	}

}
