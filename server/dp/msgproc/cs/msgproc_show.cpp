/********************************************************************
	created:	2016Äê9ÔÂ8ÈÕ11:18:14
	file base:	msgproc_show
	file ext:	cpp
	author:		lxy
	
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_show.hpp"
#include <item_msg.hpp>
#include <char_msg.hpp>

namespace hld
{
	

	void cs2dp_req_load_char_show(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_show_info_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_char_show_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_char_show_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_char_show_info sql_data;
		//s_item_info_db sql_data[MAX_ITEM];
		int32 data_size = sizeof(s_char_show_info);
		if (result.error || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);

			dp2cs_load_char_show show_info;
			show_info.unit_array_index = unit_array_index;
			memcpy(&show_info.show_info, &sql_data, sizeof(s_char_show_info));

			dbproxy_service::getInstance().send_message(connindex, &show_info, sizeof(show_info));
			return ;
		}
	}

	void cs2dp_req_save_char_show( uint32 connindex, const void* data_ptr, size_t data_len )
	{
		const cs2dp_save_char_show* pdata = static_cast<const cs2dp_save_char_show*>(data_ptr);
		if ( NULL == pdata )
		{
			return;
		}

		if(data_len != sizeof(cs2dp_save_char_show))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->show_info.role_guid.server_64)<<_XTEXT(".sp_role_show_info_save(")<< pdata->show_info.role_guid.server_64 << ",'";
		sql << "(" << pdata->show_info.role_guid.server_64 << ",";
		sql << pdata->show_info.showed_mount_template_id << ",";
		sql << pdata->show_info.showed_wing_template_id << ",";
		sql << pdata->show_info.showed_mount_guid.server_64 << ",";
		sql << pdata->show_info.showed_wing_guid.server_64 << ",";

		sql << pdata->show_info.quick_call_mount_guid_array[0].server_64 << ",";
		sql << pdata->show_info.quick_call_mount_guid_array[1].server_64 << ",";
		sql << pdata->show_info.quick_call_mount_guid_array[2].server_64 << ",";

		sql << pdata->show_info.showed_quickly_hp_guid.server_64;

		sql << ")";
		sql << "');";

		db_query_type query;
		query.role_guid = pdata->show_info.role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}