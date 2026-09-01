/********************************************************************
created:	2015年7月24日14:13:03
file base:	msgproc_buff
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_buff.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <buff_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "buff.pb.h"

namespace faith
{
	static void cs2dp_load_role_buff_send_lua(uint32 connindex, packet_s2s *p_db)
	{
		if (p_db == nullptr)
		{
			return;
		}
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(p_db->logic_guid.server_64);
		msg.set_unit_array_index(p_db->logic_index);
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_buff);


	}
	void cs2dp_req_load_char_buff(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		packet_s2s* data = parse_msg::getInstance().get_server_msg();
		if (data_set_ptr && data_set_ptr->get_proto_info(e_data_flag_buff, data->google_data, data->google_data_len))
		{
			data->wheader = e_msgindex_dp2cs_load_char_buff;
			data->logic_guid = role_guid;
			data->logic_index = unit_array_index;
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_buff);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, data, data->get_packet_len());
			}
			else
			{
				cs2dp_load_role_buff_send_lua(connindex, data);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_buff_load(") << role_guid.server_64 << _XTEXT(");");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_buff;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_buff_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		static s_buff_save_db sql_data[MAX_BUFF_DESC];
		g_clear_data(sql_data, MAX_BUFF_DESC);

		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_buff;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
		}
		else
		{
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			buff_proto_buff_save_load load_msg;

			for (int32 i = 0; i < table_len; ++i)
			{
				buff_proto_buff_info* buff_info = load_msg.add_data_ary();
				buff_info->set_buff_guid(sql_data[i].data_info.buff_guid.server_64);
				buff_info->set_sender_guid(sql_data[i].data_info.sender_guid.server_64);
				for (int32 j = 0; j < EBuffInfo_max; ++j)
				{
					buff_info->add_data_ary(sql_data[i].data_info.data_ary[j]);
				}
			}
			packet_s2s* data = parse_msg::getInstance().serialze_server(&load_msg, e_msgindex_dp2cs_load_char_buff, role_guid, unit_array_index);
			if (nullptr == data)
			{
				dp2cs_load_data_error error;
				error.role_guid = role_guid;
				error.array_index = unit_array_index;
				error.error_code = e_data_flag_buff;
				error.error_result = 1000;
				dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
				return;
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_buff);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, data, data->get_packet_len());
			}
			else
			{
				cs2dp_load_role_buff_send_lua(connindex, data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_buff, data->google_data, data->get_google_data_len());
			}
		}
	}

	void cs2dp_req_save_char_buff(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		buff_proto_buff_save_load save_proto;
		packet_s2s* save_data_ptr = parse_msg::getInstance().parse_message_server(&save_proto, data_ptr, data_len);
		if (nullptr == save_data_ptr)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(save_data_ptr->logic_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_buff, save_data_ptr->google_data, save_data_ptr->google_data_len);

			if (save_data_ptr->logic_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, save_data_ptr->logic_guid, save_data_ptr->logic_index, e_data_flag_buff, save_data_ptr->logic_ex);
			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(save_data_ptr->logic_guid.server_64)<<_XTEXT(".sp_role_buff_save(") << save_data_ptr->logic_guid.server_64 << ",'";
		int32 info_len = save_proto.data_ary_size();
		int32 login_len = EBuffInfo_max - 1;
		for (int32 i = 0; i < info_len; i++)
		{
			const buff_proto_buff_info& buff_info = save_proto.data_ary(i);
			sql << "("
				<< "" << save_data_ptr->logic_guid.server_64 << ","
				<< "" << buff_info.buff_guid() << ","
				<< "" << buff_info.sender_guid() << ",";
			for (int32 j = 0; j < EBuffInfo_max; ++j)
			{
				if (j != login_len)
				{
					sql << buff_info.data_ary(j) << ",";
				}
				else
				{
					sql << buff_info.data_ary(j);
				}
			}
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
		query.logic_id = e_data_flag_buff;
		query.role_guid = save_data_ptr->logic_guid;
		query.sql_str.assign(sql.c_str(), sql.size());

		if (save_data_ptr->logic_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, save_data_ptr->logic_guid, save_data_ptr->logic_index, e_data_flag_buff, save_data_ptr->logic_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}