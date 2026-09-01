/********************************************************************
	file name:	msgproc_relation.cpp
	created:	2017/06/07 23:32
	author:		zhy
	purpose:	
*********************************************************************/
#include "dbproxy_service.hpp"
#include "msgproc_relation.hpp"
#include <char_msg.hpp>
#include "Logic/relation_def.h"
#include "utility/parse_msg.h"
namespace faith
{
	void ws2dp_req_load_player_relation_lua(uint32 connindex, int64 sender_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(sender_guid) << _XTEXT(".sp_role_relation_load('") << sender_guid << _XTEXT("');");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_relation;
		query.role_guid = (guid_64)sender_guid;
		query.handler = boost::bind(&ws2dp_req_load_player_relation_end, _1, connindex, (guid_64)sender_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_player_relation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_req_load_relation_list request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		ws2dp_req_load_player_relation_lua(connindex, request.sender_guid());
	}

	void ws2dp_req_load_player_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid)
	{
		s_relation_info_db sql_data[MAX_FRIEND_NUM * e_relationlist_type_max];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_relation_info_db);
		dp2ws_load_player_relation reply_relation;
		reply_relation.role_guid = role_guid;
		if (result.error || table_len > (MAX_FRIEND_NUM * e_relationlist_type_max) || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex,  &reply_relation, reply_relation.get_pak_length());
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);
			reply_relation.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_relation.relation_array[i].relation_guid = sql_data[i].data_info.relation_guid;
				strncpy_s(reply_relation.relation_array[i].relation_name, sql_data[i].data_info.relation_name, max_name_size);
				
				//关系数据中的战力数据更换字段 增加新字段为0时将新字段设置为旧字段
				if (sql_data[i].data_info.gs_value <= 0)
				{
					reply_relation.relation_array[i].gs_value = sql_data[i].data_info.data_array[e_relation_data_gs];
				}
				else
				{
					reply_relation.relation_array[i].gs_value = sql_data[i].data_info.gs_value;
				}

				for (int32 j = 0; j < e_relation_data_max; j++)
				{
					reply_relation.relation_array[i].data_array[j] = sql_data[i].data_info.data_array[j];
				}
			}
			dbproxy_service::getInstance().send_message(connindex,  &reply_relation, reply_relation.get_pak_length());
			return;
		}
	}

	void ws2dp_req_save_player_relation( uint32 connindex, const void* data_ptr, size_t data_len )
	{
		const ws2dp_save_player_relation* pdata = static_cast<const ws2dp_save_player_relation*>(data_ptr);
		if ( NULL == pdata )
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_relation_save('")<< pdata->role_guid.server_64 << "','";

		for (int32 i = 0; i < pdata->data_num; i++)
		{
			if (pdata->data_list[i].relation_guid.is_valid())
			{
				sql << "(";
				sql << pdata->role_guid.server_64 << ",";
				sql << pdata->data_list[i].relation_guid.server_64 << ",";
				sql << "\\\'" << pdata->data_list[i].relation_name << "\\\'" << ",";
				sql <<  pdata->data_list[i].gs_value << ",";
				for (int32 j = 0; j < e_relation_data_max; ++j)
				{
					if (j < e_relation_data_max - 1)
					{
						sql << pdata->data_list[i].data_array[j] << ",";
					}
					else
					{
						sql << pdata->data_list[i].data_array[j];
					}
				}
				if (i != (pdata->data_num - 1))
				{
					sql << "),";
				}
				else
				{
					sql << ")";
				}
			}
		}
		sql << "');";
		db_query_type query;
		query.role_guid = pdata->role_guid;
		query.logic_id = e_data_flag_relation;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_offline_other_relation_lua(uint32 connindex, int64 sender_guid, int32 list_type, int64 other_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(other_guid) << _XTEXT(".sp_role_info_load('") << other_guid << _XTEXT("');");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = (guid_64)sender_guid;
		query.handler = boost::bind(&ws2dp_req_load_offline_other_relation_end, _1, connindex, (guid_64)sender_guid, list_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_offline_other_relation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_relation_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		ws2dp_req_load_offline_other_relation_lua(connindex, request.sender_guid(), request.list_type(), request.other_guid());
	}

	void ws2dp_req_load_offline_other_relation_other_server(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const ws2dp_load_relation_info_other_server* pdata = static_cast<const ws2dp_load_relation_info_other_server*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
 
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->other_guid.server_64) << _XTEXT(".sp_role_info_load('") << pdata->other_guid.server_64 << _XTEXT("');");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->sender_guid;
		query.handler = boost::bind(&ws2dp_req_load_offline_other_relation_other_server_end, _1, connindex, pdata->sender_guid, pdata->sender_server_id, pdata->list_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}


	void ws2dp_req_load_offline_other_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& relation_type)
	{
		s_unit_info				data_info;
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_unit_info);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(&data_info, 0, sizeof(data_info));
			db_read_data(&result, &data_info, data_size);

			dp2ws_load_relation_info reply_relation;
			reply_relation.sender_guid = role_guid;
			reply_relation.other_relation.relation_guid.server_64 = data_info.role_guid.server_64;
			strncpy_s(reply_relation.other_relation.relation_name, data_info.role_name, max_name_size);
			reply_relation.other_relation.gs_value = data_info.get_gs_value();
			reply_relation.other_relation.data_array[e_relation_data_type] = relation_type;
			reply_relation.other_relation.data_array[e_relation_data_level] = data_info.data_ary[e_role_info_exp_level];
			reply_relation.other_relation.data_array[e_relation_data_template_id] = data_info.data_ary[e_role_info_template_id];
			reply_relation.other_relation.data_array[e_relation_data_relation_state] = 0;
			reply_relation.other_relation.data_array[e_relation_data_last_login_time] = 0;
			reply_relation.other_relation.data_array[e_relation_data_friendliness] = 0;

			dbproxy_service::getInstance().send_message(connindex,  &reply_relation, sizeof(dp2ws_load_relation_info));
			return;
		}

	}

	void ws2dp_req_load_offline_other_relation_other_server_end(db_result_type result, uint32 connindex, const guid_64 & role_guid, const int32 & server_id, const int32 & relation_type)
	{
		s_unit_info				data_info;
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_unit_info);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(&data_info, 0, sizeof(data_info));
			db_read_data(&result, &data_info, data_size);

			dp2ws_load_relation_info_other_server reply_relation;
			reply_relation.sender_guid = role_guid;
			reply_relation.other_relation.relation_guid.server_64 = data_info.role_guid.server_64;
			reply_relation.sender_server_id = server_id;
			strncpy_s(reply_relation.other_relation.relation_name, data_info.role_name, max_name_size);
			reply_relation.other_relation.gs_value = data_info.get_gs_value();
			reply_relation.other_relation.data_array[e_relation_data_type] = relation_type;
			reply_relation.other_relation.data_array[e_relation_data_level] = data_info.data_ary[e_role_info_exp_level];
			reply_relation.other_relation.data_array[e_relation_data_template_id] = data_info.data_ary[e_role_info_template_id];
			reply_relation.other_relation.data_array[e_relation_data_relation_state] = 0;
			reply_relation.other_relation.data_array[e_relation_data_last_login_time] = 0;
			reply_relation.other_relation.data_array[e_relation_data_friendliness] = 0;

			dbproxy_service::getInstance().send_message(connindex, &reply_relation, sizeof(dp2ws_load_relation_info_other_server));
			return;
		}

	}

	void ws2dp_req_add_relation_lua(uint32 connindex, int64 role_guid, int64 relation_guid, xstring relation_name, int64 relation_gs_value, int32 data_array[e_relation_data_max])
	{
		xchar		t_relation_name[max_name_size + 1];
		memset(t_relation_name, 0, sizeof(t_relation_name));
		memcpy(t_relation_name, relation_name.c_str(), relation_name.size() > sizeof(t_relation_name) ? sizeof(t_relation_name) : relation_name.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_role_relation_add(") << role_guid << "," << relation_guid << "," << data_array[e_relation_data_type];
		sql << ",'(";
		sql << role_guid;
		sql << ",";
		sql << relation_guid;
		sql << ", \\'";
		sql << t_relation_name;
		sql << "\\', \\'";
		sql << relation_gs_value;
		sql << "\\'";
		for (int i = 0; i < e_relation_data_max; ++i)
		{
			sql << ",";
			sql << data_array[i];
		}
		sql << ")');";

		db_query_type query;
		query.role_guid = (guid_64)role_guid;
		query.logic_id = e_data_flag_relation;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_add_relation_end, _1, connindex, (guid_64)role_guid, (guid_64)relation_guid, data_array[e_relation_data_type]);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_add_relation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_add_relation request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int32 t_data_array[e_relation_data_max];
		for (int32 i = 0; i < request.data_array_size(); i++)
		{
			if (i >= e_relation_data_max)
			{
				break;
			}
			t_data_array[i] = request.data_array(i);
		}

		ws2dp_req_add_relation_lua(connindex, request.role_guid(), request.relation_guid(), request.relation_name(), request.relation_gs_value(), t_data_array);
	}

	void ws2dp_req_add_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const int32 relation_type)
	{
		if (!result.error)
		{
			dp2ws_add_relation msg;
			db_read_data(&result, &msg.result, sizeof(int32));
			msg.role_guid = role_guid;
			msg.relation_guid = target_guid;
			msg.relation_type = relation_type;
			dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_add_relation));
		}
		else
		{
			dp2ws_add_relation msg;
			msg.result = 1;
			msg.role_guid = role_guid;
			msg.relation_guid = target_guid;
			msg.relation_type = relation_type;
			dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_add_relation));
		}
	}

	void ws2dp_req_del_relation_lua(uint32 connindex, int64 role_guid, int64 target_role_guid, int32 relation_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_role_relation_del(");
		sql << role_guid;
		sql << ",";
		sql << target_role_guid;
		sql << ",";
		sql << relation_type;
		sql << ");";

		db_query_type query;
		query.role_guid = role_guid;
		query.logic_id = e_data_flag_relation;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_del_relation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_del_relation request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		ws2dp_req_del_relation_lua(connindex, request.role_guid(), request.target_role_guid(), request.relation_type());
	}

}



