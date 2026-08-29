/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   16:02
	file base:	msgproc_element_heart
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_element_heart.hpp"
#include <element_heart_msg.hpp>
#include <char_msg.hpp>

namespace faith
{
	void cs2dp_req_load_char_element_heart(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_element_heart_load(") << up_role_guid.server_64 << _XTEXT(");");

		//db_query_type query;
		//query.sql_str.assign(sql.c_str(), sql.size());
		//query.logic_id = e_data_flag_element_heart;
		//query.role_guid = role_guid;
		//query.handler = boost::bind(&cs2dp_req_load_char_element_heart_end, _1, connindex, role_guid, unit_array_index);
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_char_element_heart_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		//s_item_info_db sql_data[element_heart_bag_num + element_heart_equip_num];
		//int32	table_len = result.query.data_select.row_count;
		//int32 data_size = sizeof(s_item_info_db);
		//if (result.error || table_len > element_heart_bag_num + element_heart_equip_num || data_size != result.query.data_select.row_size)
		//{
		//	dp2cs_load_data_error error;
		//	error.role_guid = role_guid;
		//	error.error_code = e_data_flag_element_heart;
		//	error.error_result = result.error;
		//	dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
		//	return;
		//}
		//else
		//{
		//	memset(sql_data, 0, sizeof(sql_data));
		//	db_read_data(&result, &sql_data, data_size * table_len);

		//	dp2cs_load_char_element_heart reply_element_heart;
		//	reply_element_heart.is_over = true;
		//	reply_element_heart.role_guid = role_guid;
		//	reply_element_heart.data_num = table_len;
		//	for (int32 i = 0; i < table_len; ++i)
		//	{
		//		memcpy(&(reply_element_heart.element_heart_data[i]), &(sql_data[i].data_info), sizeof(s_item_info));
		//	}
		//	dbproxy_service::getInstance().send_message(connindex, &reply_element_heart, reply_element_heart.get_pak_length());
		//	return ;
		//}
	}

	void cs2dp_req_save_char_element_heart( uint32 connindex, const void* data_ptr, size_t data_len )
	{
		//const cs2dp_save_char_element_heart* pdata = static_cast<const cs2dp_save_char_element_heart*>(data_ptr);
		//if ( NULL == pdata )
		//{
		//	return;
		//}

		//if(data_len != (const_cast<cs2dp_save_char_element_heart*>(pdata))->get_pak_length())
		//{
		//	return;
		//}
		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_element_heart_save(")<< pdata->role_guid.server_64 << ",'";
		//int32 tempLen = pdata->data_num - 1;
		//int32 sqlEnd = e_item_info_max - 1;
		//for (int32 i = 0; i < pdata->data_num; i++)
		//{
		//	sql << "("<< pdata->role_guid.server_64 << ","
		//		<< pdata->data_list[i].item_guid.server_64 << ",";
		//	for (int32 j = 0; j < e_item_info_max; ++j)
		//	{
		//		if (j != sqlEnd)
		//		{
		//			sql << pdata->data_list[i].data_ary[j] << ",";
		//		}
		//		else
		//		{
		//			sql << pdata->data_list[i].data_ary[j];
		//		}
		//	}
		//	if (i != tempLen)
		//	{
		//		sql << "),";
		//	}
		//	else
		//	{
		//		sql << ")";
		//	}
		//}
		//sql << "');";
		//db_query_type query;
		//query.logic_id = e_data_flag_element_heart;
		//query.role_guid = pdata->role_guid;
		//query.sql_str.assign(sql.c_str(), sql.size());
		//query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_element_heart, pdata->save_type_ex, sql.c_str());
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}