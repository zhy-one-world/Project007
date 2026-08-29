#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_demons_tower.hpp"
#include "data_manager/data_manager.hpp"
#include <map_msg.hpp>
#include <char_msg.hpp>

namespace faith
{
	void cs2dp_req_load_char_demons_tower(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		//data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		//dp2cs_load_char_demons_tower msgData;
		//int32 data_num = 1;
		//if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_demons_tower, &msgData.demons_tower_info, data_num))
		//{
		//	msgData.role_guid = role_guid;
		//	msgData.unit_array_index = unit_array_index;
		//	dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		//}
		//else
		//{
		//	sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//	sql << _XTEXT("CALL sp_role_demons_tower_load('") << role_guid.server_64 << _XTEXT("');");

		//	db_query_type query;
		//	query.sql_str.assign(sql.c_str(), sql.size());
		//	query.logic_id = e_data_flag_demons_tower;
		//	query.role_guid = role_guid;
		//	query.handler = boost::bind(&cs2dp_req_load_char_demons_tower_end, _1, connindex, role_guid, unit_array_index);
		//	db_manager::getInstance().get_db_link().game_db.add_query(query);
		//}
	}

	void cs2dp_req_load_char_demons_tower_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		//s_demons_tower_info_db sql_data[max_map_record_num];
		//int32 table_len = result.query.data_select.row_count;
		//int32 data_size = sizeof(s_demons_tower_info_db);
		//if (result.error || table_len > max_map_record_num || data_size != result.query.data_select.row_size)
		//{
		//	dp2cs_load_data_error error;
		//	error.role_guid = role_guid;
		//	error.array_index = unit_array_index;
		//	error.error_code = e_data_flag_demons_tower;
		//	error.error_result = result.error;
		//	dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
		//}
		//else
		//{
		//	memset(sql_data, 0, sizeof(sql_data));
		//	db_read_data(&result, &sql_data, data_size * table_len);

		//	dp2cs_load_char_demons_tower reply_data;
		//	reply_data.is_over = true;
		//	reply_data.role_guid = role_guid;
		//	reply_data.unit_array_index = unit_array_index;

		//	memcpy(&(reply_data.demons_tower_info.data_ary), &(sql_data[0].data_array), sizeof(sql_data[0].data_array));

		//	dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
		//	data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		//	if (data_set_ptr)
		//	{
		//		data_set_ptr->add_data_info(e_data_flag_demons_tower, &reply_data.demons_tower_info, sizeof(s_demons_tower_info), 1);
		//	}
		//}
	}

	void cs2dp_req_save_char_demons_tower(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		//const cs2dp_save_char_demons_tower* pdata = static_cast<const cs2dp_save_char_demons_tower*>(data_ptr);
		//if (nullptr == pdata)
		//{
		//	return;
		//}
		//data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		//if (data_set_ptr)
		//{
		//	data_set_ptr->add_data_info(e_data_flag_demons_tower, &pdata->demon_tower_info, sizeof(s_demons_tower_info), 1);

		//	if (pdata->save_type_ex != e_save_data_type_exit_game)
		//	{
		//		cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_demons_tower, pdata->save_type_ex);
		//	}
		//}
		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("CALL sp_role_demons_tower_save(") << pdata->role_guid.server_64 << ",'";


		//sql << "(" << pdata->role_guid.server_64 << ",";
		//for (int32 i = 0; i < e_demons_tower_info_max; ++i)
		//{
		//	if (i != 0)
		//	{
		//		sql << "," << pdata->demon_tower_info.data_ary[i];
		//	}
		//	else
		//	{
		//		sql << pdata->demon_tower_info.data_ary[i];
		//	}
		//}
		//sql << ")');";

		//db_query_type query;
		//query.logic_id = e_data_flag_demons_tower;
		//query.role_guid = pdata->role_guid;
		//query.sql_str.assign(sql.c_str(), sql.size());

		//if (pdata->save_type_ex == e_save_data_type_exit_game)
		//{
		//	query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_demons_tower, pdata->save_type_ex, sql.c_str());
		//}
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}