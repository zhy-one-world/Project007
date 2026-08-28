/********************************************************************
created:	2016年12月12日12:54:24
file base:	msgproc_mail_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_red_package_ws.hpp"
#include "utility/guid_gen.h"
#include "utility/init_unit.h"
#include "dbproxy_service.hpp"
#include "utility/parse_msg.h"
#include "template/template_manager.h"
namespace hld
{
	
	void save_red_package_to_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		/*const ws2dp_save_red_package* pdata = static_cast<const ws2dp_save_red_package*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != (const_cast<ws2dp_save_red_package*>(pdata))->get_pak_length())
		{
			return;
		}
		sub_save_red_package_base_info(pdata->red_package_list, pdata->data_num);
		sub_save_red_package_receiver(pdata->red_package_list, pdata->data_num);*/
		packet_s2s* pPacket = (packet_s2s*)data_ptr;
		save_red_package_to_sql_lua(connindex, (char*)pPacket->google_data, pPacket->google_data_len);
	}

	void sub_save_red_package_base_info(server2dp_proto_s_red_package data_list[red_package_total_num], int32 data_num)
	{
		sql_builder base_info_sql(db_manager::getInstance().get_db_link().game_db);
		base_info_sql << _XTEXT("call sp_redpack_save(") << "'";
		int32 tempLen = data_num - 1;
		for (int32 i = 0; i < data_num && i < red_package_total_num; i++)
		{
			base_info_sql << "(" << data_list[i].base_info().red_package_guid() << ",";
			base_info_sql << data_list[i].base_info().red_package_template_id() << ",";
			base_info_sql << data_list[i].base_info().sender_guid() << ",";
			base_info_sql << data_list[i].base_info().create_time() << ",";
			base_info_sql << "\\\'" << data_list[i].base_info().sender_name() << "\\\',";
			base_info_sql << "\\\'" << data_list[i].base_info().text() << "\\\',";
			base_info_sql << data_list[i].base_info().sender_vip_level() << ",";
			base_info_sql << data_list[i].base_info().remain_times() << ",";
			base_info_sql << data_list[i].base_info().remain_money_num();

			if (i != tempLen)
			{
				base_info_sql << "),";
			}
			else
			{
				base_info_sql << ")";
			}
		}
		base_info_sql << "');";
		db_query_type base_query;
		base_query.sql_str.assign(base_info_sql.c_str(), base_info_sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(base_query);
	}

	void sub_save_red_package_receiver(server2dp_proto_s_red_package data_list[red_package_total_num], int32 data_num)
	{
		sql_builder receiver_info_sql(db_manager::getInstance().get_db_link().game_db);
		receiver_info_sql << _XTEXT("call sp_redpack_receiver_save(") << "'";

		int32 receiver_total_count = 0;
		for (int32 i = 0; i < data_num && i < red_package_total_num; i++)
		{
			//将此红包中的各个领奖人存档
			int32 reveiver_len = data_list[i].info_list_size() - 1;
			if (reveiver_len > red_package_max_receiver_num)
			{
				continue;
			}
			for (int32 receiever_index = 0; receiever_index < red_package_max_receiver_num; ++receiever_index)
			{
				if (receiver_total_count > 0)
				{
					receiver_info_sql << ",";
				}
				receiver_info_sql << "(" << data_list[i].base_info().red_package_guid() << ",";
				receiver_info_sql << data_list[i].info_list(receiever_index).receiver_guid() << ",";
				receiver_info_sql << "\\\'" << data_list[i].info_list(receiever_index).role_name() << "\\\',";
				receiver_info_sql << data_list[i].info_list(receiever_index).money_num();

				receiver_info_sql << ")";

				receiver_total_count++;

				if (receiever_index == reveiver_len)
				{
					break;
				}
			}
		}
		receiver_info_sql << "');";
		db_query_type receiver_query;
		receiver_query.sql_str.assign(receiver_info_sql.c_str(), receiver_info_sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(receiver_query);
	}

	void load_red_package_by_sql_lua(uint32 connindex)
	{
		sql_builder base_info_sql(db_manager::getInstance().get_db_link().game_db);
		base_info_sql << _XTEXT("call sp_redpack_load();");

		db_query_type query;
		query.sql_str.assign(base_info_sql.c_str(), base_info_sql.size());
		query.handler = boost::bind(&load_red_package_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_red_package_receiver_by_sql_lua(uint32 connindex, int64 role_guid)
	{
		//没有任何信息，直接load
		sql_builder receiver_info_sql(db_manager::getInstance().get_db_link().game_db);
		receiver_info_sql << _XTEXT("call sp_redpack_receiver_load(") << role_guid << ");";
		db_query_type query;
		query.sql_str.assign(receiver_info_sql.c_str(), receiver_info_sql.size());
		query.handler = boost::bind(&load_red_package_receiver_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void add_red_package_to_sql_proc_lua(uint32 connindex, int64 role_guid, int32 template_id, int64 sender_guid, int32 create_time, xstring sender_name, xstring red_text, int32 sender_vip_level, int32 remain_times, int32 remain_money_num)
	{
		sql_builder base_info_sql(db_manager::getInstance().get_db_link().game_db);
		base_info_sql << _XTEXT("call sp_redpack_add(") << role_guid << ",";

		base_info_sql << "'(";

		base_info_sql << role_guid << ",";
		base_info_sql << template_id  << ",";
		base_info_sql << sender_guid << ",";
		base_info_sql << create_time << ",";
		base_info_sql << "\\\'" << sender_name << "\\\',";
		base_info_sql << "\\\'" << red_text << "\\\',";
		base_info_sql << sender_vip_level << ",";
		base_info_sql << remain_times << ",";
		base_info_sql << remain_money_num;

		base_info_sql << ")');";

		db_query_type base_info_query;
		base_info_query.sql_str.assign(base_info_sql.c_str(), base_info_sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(base_info_query);
	}

	void add_red_package_receiver_to_sql_proc_lua(uint32 connindex, int64 role_guid, int64 receiver_guid, xstring role_name, int32 money_num)
	{
		sql_builder receiver_info_sql(db_manager::getInstance().get_db_link().game_db);
		receiver_info_sql << _XTEXT("call sp_redpack_receiver_add(") << role_guid << ",";
		receiver_info_sql << receiver_guid << ",";

		receiver_info_sql << "'(";

		receiver_info_sql << role_guid << ",";
		receiver_info_sql << receiver_guid << ",";
		receiver_info_sql << "\\\'" << role_name << "\\\',";
		receiver_info_sql << money_num;

		receiver_info_sql << ")');";

		db_query_type receiver_query;
		receiver_query.sql_str.assign(receiver_info_sql.c_str(), receiver_info_sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(receiver_query);
	}

	void del_red_package_by_sql_proc_lua(uint32 connindex, int64 red_package_guid)
	{
		sql_builder del_info_sql(db_manager::getInstance().get_db_link().game_db);
		del_info_sql << _XTEXT("call sp_redpack_del(") << red_package_guid << ");";

		db_query_type del_info_query;
		del_info_query.sql_str.assign(del_info_sql.c_str(), del_info_sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(del_info_query);
	}

	void load_red_package_by_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_red_package_by_sql_lua(connindex);
	}

	void load_red_package_end(db_result_type result, uint32 connindex)
	{
		s_red_package_base sql_data[red_package_total_num];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_red_package_base);
		if (result.error || table_len > red_package_total_num || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_red_package_end reply_data;

			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.red_package_list[i]), &(sql_data[i]), sizeof(s_red_package_base));
			}
			bool is_use = proto_by_lua(e_msgindex_dp2ws_load_red_pack_end);
			if (!is_use)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				dp2s_proto::load_red_package_end pro_msg;
				reply_data.to_proto(pro_msg);
				dbproxy_service::getInstance().send_message_lua(connindex, &pro_msg, e_msgindex_dp2ws_load_red_pack_end);
			}
		}
	}

	void load_red_package_receiver_by_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_red_package_receiver request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_red_package_receiver_by_sql_lua(connindex, request.red_package_guid());
	}

	void load_red_package_receiver_end(db_result_type result, uint32 connindex)
	{
		s_red_package_receiver sql_data[red_package_max_receiver_num];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_red_package_receiver);
		if (result.error || table_len > red_package_max_receiver_num || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_red_package_receiver_end reply_data;

			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.receiver_list[i]), &(sql_data[i]), sizeof(s_red_package_receiver));
			}
			bool is_use = proto_by_lua(e_msgindex_dp2ws_load_red_pack_receiver_end);
			if (!is_use)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				dp2s_proto::load_red_package_receiver_end pro_msg;
				reply_data.to_proto(pro_msg);
				dbproxy_service::getInstance().send_message_lua(connindex, &pro_msg, e_msgindex_dp2ws_load_red_pack_receiver_end);
			}
		}
	}

	void add_red_package_to_sql_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_add_red_package_to_sql request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		add_red_package_to_sql_proc_lua(connindex, request.red_info().red_package_guid(), request.red_info().red_package_template_id(), request.red_info().sender_guid(), request.red_info().create_time(), 
			request.red_info().sender_name(), request.red_info().text(), request.red_info().sender_vip_level(), request.red_info().remain_times(), request.red_info().remain_money_num());
	}
	void add_red_package_receiver_to_sql_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_add_red_package_receiver_to_sql request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		add_red_package_receiver_to_sql_proc_lua(connindex, request.red_info().red_package_guid(), request.red_info().receiver_guid(), request.red_info().role_name(), request.red_info().money_num());
	}

	void del_red_package_by_sql_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_del_red_package_by_sql request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		del_red_package_by_sql_proc_lua(connindex, request.red_package_guid());
	}
	void save_red_package_to_sql_lua(uint32 connindex, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_red_package request;
		if (!parse_msg::getInstance().parse_buffer_to_proto(&request, data_ptr, data_len))
		{
			return;
		}
		server2dp_proto_s_red_package red_package_list[red_package_total_num];
		int32 data_num = 0;
		for (int32 i = 0; i < request.data_ary_size() &&  i < red_package_total_num; ++i)
		{
			red_package_list[i] = request.data_ary(i);
			data_num++;
		}
		sub_save_red_package_base_info(red_package_list, data_num);
		sub_save_red_package_receiver(red_package_list, data_num);
	}
}
