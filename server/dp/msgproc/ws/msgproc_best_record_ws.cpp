/********************************************************************
created:	2016Äê12ÔÂ12ÈÕ12:59:39
file base:	msgproc_best_record_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_best_record_ws.hpp"
#include <activity_msg.hpp>
#include "dbproxy_service.hpp"
#include "dbproxy_service.hpp"
#include "utility/parse_msg.h"

namespace hld
{
	

	void save_best_record(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_best_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int32 map_template_id = request.record_info().map_template_id();
		int64 role_guid[hld::single_map_record_num];
		xstring role_name[hld::single_map_record_num];
		int32 role_value[hld::single_map_record_num];
		for (int32 i = 0; i < hld::single_map_record_num; i++)
		{
			if (i >= request.record_info().role_guid_size())
			{
				break;
			}
			role_guid[i] = request.record_info().role_guid(i);
			role_name[i] = request.record_info().role_name(i);
			role_value[i] = request.record_info().role_value(i);
		}
		save_best_record_lua(connindex, map_template_id, role_guid, role_name, role_value);
	}

	void save_best_record_end(db_result_type result)
	{
		if (result.error) {} else {}
	}

	void load_all_best_record(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		/*server2dp_proto_ws2dp_load_all_best_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}*/
		load_all_best_record_lua(connindex);
	}

	void load_all_best_record_end(db_result_type result, uint32 connindex)
	{
		if (result.error || result.query.data_select.row_count <= 0)
		{
			return;
		}
		else
		{
			int32 data_num = result.query.data_select.row_count;
			int32 data_size = result.query.data_select.row_size * data_num;
			s_db_best_record sql_data[max_best_record_num_all];
			db_read_data(&result, sql_data, data_size);

			dp2ws_load_all_best_record load_best_record_msg;
			int32 num = 0;
			for (int32 i = 0; i < data_num; i++)
			{
				if (num < max_best_record_num)
				{
					load_best_record_msg.db_record_list[num] = sql_data[i];
					num++;
				}
				else
				{
					load_best_record_msg.db_record_num = num;
					dbproxy_service::getInstance().send_message(connindex, &load_best_record_msg, sizeof(load_best_record_msg));
					load_best_record_msg.clear_data();
					num = 0;
				}
			}
			if (num > 0)
			{
				load_best_record_msg.db_record_num = num;
				dbproxy_service::getInstance().send_message(connindex, &load_best_record_msg, sizeof(load_best_record_msg));
			}
		}
	}
	void save_best_record_lua(uint32 connindex, uint32 map_template_id, int64 role_guid[hld::single_map_record_num], xstring role_name[hld::single_map_record_num], int32 role_value[hld::single_map_record_num])
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		int32 best_record_num = -1;
		for (int32 i = 0; i < hld::single_map_record_num; i++)
		{
			if (role_guid[i] <= 0)
			{
				break;
			}
			best_record_num = i + 1;
		}
		if (best_record_num <= 0)
		{
			return;
		}

		sql << "call sp_best_record_info_save(" << map_template_id << ",'";
		for (int32 i = 0; i < best_record_num; i++)
		{
			sql << "(" << map_template_id << ",";
			sql << role_guid[i] << ",\\\'";
			sql << role_name[i] << "\\\',";
			sql << role_value[i];
			if (i == best_record_num - 1)
			{
				sql << ")";
			}
			else
			{
				sql << "),";
			}
		}
		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_best_record_end, _1);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_all_best_record_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_best_record_info_load_all();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_best_record_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}
