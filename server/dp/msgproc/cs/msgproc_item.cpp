/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   16:02
	file base:	msgproc_item
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_item.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <item_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "item.pb.h"

namespace faith
{
	void cs2dp_req_load_char_item(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index, int32 bag_type)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		packet_s2s* data = parse_msg::getInstance().get_server_msg();
		if (data_set_ptr && data_set_ptr->get_proto_info(e_data_flag_item + bag_type, data->google_data, data->google_data_len))
		{
			data->wheader = e_msgindex_dp2cs_load_char_item;
			data->logic_guid = role_guid;
			data->logic_index = unit_array_index;
			dbproxy_service::getInstance().send_message(connindex, data, data->get_packet_len());
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_item_load(") << up_role_guid.server_64 << "," << bag_type << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_item;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_item_end, _1, connindex, role_guid, unit_array_index, bag_type);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index, int32 bag_type)
	{
		s_item_info_db sql_data[All_MAX_ITEM_BAG_NUM];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);

		if (result.error || table_len > All_MAX_ITEM_BAG_NUM || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_item;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			item_proto_item_save_load load_msg;
			load_msg.set_bag_type(bag_type);
			for (int32 i = 0; i < table_len; ++i)
			{
				load_msg.add_item_guid(sql_data[i].data_info.item_guid.server_64);
				load_msg.add_param_str(sql_data[i].data_info.param_str);
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					load_msg.add_data_ary(sql_data[i].data_info.data_ary[j]);
				}
			}
			packet_s2s* data = parse_msg::getInstance().serialze_server(&load_msg, e_msgindex_dp2cs_load_char_item, role_guid, unit_array_index);
			if (nullptr == data)
			{
				dp2cs_load_data_error error;
				error.role_guid = role_guid;
				error.array_index = unit_array_index;
				error.error_code = e_data_flag_item;
				error.error_result = 1000;
				dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
				return;
			}
			dbproxy_service::getInstance().send_message(connindex, data, data->get_packet_len());
			
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_item + bag_type, data->google_data, data->get_google_data_len());
			}
			return ;
		}
	}

	void cs2dp_req_save_char_item( uint32 connindex, const void* data_ptr, size_t data_len )
	{
		item_proto_item_save_load save_proto;
		packet_s2s* save_data_ptr = parse_msg::getInstance().parse_message_server(&save_proto, data_ptr, data_len);
		if (nullptr == save_data_ptr)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(save_data_ptr->logic_guid);
		if (data_set_ptr)
		{
			if (save_proto.bag_type() > e_bag_type_none)
			{
				data_set_ptr->add_data_info(e_data_flag_item + save_proto.bag_type(), save_data_ptr->google_data, save_data_ptr->google_data_len);
			}
			if (save_data_ptr->logic_ex != e_save_data_type_exit_game && save_proto.bag_type() == e_bag_type_none)
			{
				cs2dp_req_send_info_end(true, connindex, save_data_ptr->logic_guid, save_data_ptr->logic_index, e_data_flag_item, save_data_ptr->logic_ex);
			}
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(save_data_ptr->logic_guid.server_64)<<_XTEXT(".sp_role_item_save(")<< save_data_ptr->logic_guid.server_64 << ","<< save_proto.bag_type() <<",'";
		int32 info_len = save_proto.item_guid_size();
		int32 login_len = e_item_info_max - 1;
		for (int32 i = 0; i < info_len; i++)
		{
			sql << "(" << save_data_ptr->logic_guid.server_64 << ","
				<< save_proto.item_guid(i) << ",";
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				sql << save_proto.data_ary(i*e_item_info_max + j) << ",";
			}
			sql << "\\\'" << save_proto.param_str(i) << "\\\'";
			if (i != info_len - 1)
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
		query.logic_id = e_data_flag_item;
		query.role_guid = save_data_ptr->logic_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if ((save_data_ptr->logic_ex == e_save_data_type_exit_game)&& save_proto.bag_type() == e_bag_type_none)
		//if(pdata->bag_type == e_bag_type_none)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, save_data_ptr->logic_guid, save_data_ptr->logic_index, e_data_flag_item, save_data_ptr->logic_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}