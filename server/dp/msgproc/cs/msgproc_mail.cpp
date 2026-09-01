/********************************************************************
created:	2016/08/16
file base:	msgproc_mail
file ext:	cpp
author:		lxy

purpose:
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_mail.hpp"
#include <mail_msg.hpp>
#include <char_msg.hpp>
#include "utility/parse_msg.h"
#include "template/template_manager.h"
#include "dp2cs.pb.h"

namespace faith
{
	

	void cs2dp_req_load_char_mail(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_mail_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_mail;
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_char_mail_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	static void cs2dp_load_role_mail_send_lua(uint32 connindex, const dp2cs_load_char_mail& msgData, const int32&  unit_array_index)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(unit_array_index);

		faith::db_proto::role_mail_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::db_proto::role_mail_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_mail_guid(msgData.data_list[i].mail_guid.server_64);
			for (int32 j =0; j < EMailInfo_max; j++)
			{
				db_row->add_data_ary(msgData.data_list[i].data_ary[j]);
			}
			db_row->set_text_contents(msgData.data_list[i].text_contents);
			db_row->set_sender_name(msgData.data_list[i].sender_name);
			db_row->set_mail_title(msgData.data_list[i].mail_title);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_mail);


	}

	void cs2dp_req_load_char_mail_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_mail_info_db sql_data[MAX_MAIL_NUM];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_mail_info_db);
		if (result.error || table_len > MAX_MAIL_NUM || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_mail;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));

			dp2cs_operate_load_lock_mail operate_load_lock_mail;
			operate_load_lock_mail.unit_guid = role_guid;
			operate_load_lock_mail.unit_index = unit_array_index;
			operate_load_lock_mail.operate_value = -2;
			dbproxy_service::getInstance().send_message(connindex, &operate_load_lock_mail, sizeof(operate_load_lock_mail));
			return;
		}
		else if(result.query.data_select.row_count > 0)
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2cs_load_char_mail reply_mail;
			reply_mail.is_over = true;
			reply_mail.role_guid = role_guid;
			reply_mail.data_num = table_len;
			reply_mail.unit_index = unit_array_index;

			guid_64	mail_item_guid_array[max_mail_item_num];
			memset(mail_item_guid_array, 0, sizeof(guid_64) * max_mail_item_num);
			int32 mail_item_count = 0;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_mail.data_list[i]), &(sql_data[i].data_info), sizeof(s_mail_info));

				for (int32 j = 0; j < max_item_per_mail; ++j)
				{// 存储load下来的邮件附件物品的guid
					guid_64 temp_guid;
					temp_guid.A = sql_data[i].data_info.data_ary[EMailInfo_ItemGuid11 + j * 2];
					temp_guid.B = sql_data[i].data_info.data_ary[EMailInfo_ItemGuid11 + j * 2 + 1];
					if (temp_guid. is_valid())
					{
						mail_item_guid_array[mail_item_count] = temp_guid;
						mail_item_count++;
					}
				}
			}

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_mail);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_mail, reply_mail.get_pak_length());
			}
			else
			{
				cs2dp_load_role_mail_send_lua(connindex, reply_mail, unit_array_index);
			}

			//没有物品也要调用 保证数据链完整
			cs2dp_req_load_char_mail_item(mail_item_guid_array, mail_item_count, connindex, role_guid, unit_array_index);
			return;
		}
		else
		{
			dp2cs_operate_load_lock_mail operate_load_lock_mail;
			operate_load_lock_mail.unit_guid = role_guid;
			operate_load_lock_mail.unit_index = unit_array_index;
			operate_load_lock_mail.operate_value = -2;
			dbproxy_service::getInstance().send_message(connindex, &operate_load_lock_mail, sizeof(operate_load_lock_mail));

			dp2cs_load_char_mail_item reply_mail;
			reply_mail.role_guid = role_guid;
			reply_mail.unit_array_index = unit_array_index;
			reply_mail.data_num = 0;
			dbproxy_service::getInstance().send_message(connindex, &reply_mail, reply_mail.get_pak_length());
			return;
		}
	}

	void cs2dp_req_load_char_mail_item(guid_64* item_guid, int32 item_num, int32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_mail_item_load('");

		int32 last_index = item_num - 1;
		for (int32 i = 0; i < item_num; ++i)
		{
			sql << item_guid[i].server_64;

			if (i != last_index)
			{
				sql << ",";
			}
		}
		sql << "');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_char_mail_item_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	static void cs2dp_load_role_mail_item_send_lua(uint32 connindex, const  dp2cs_load_char_mail_item& msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::db_proto::role_mail_item_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::db_proto::item_info_db *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}

			db_row->set_item_guid(msgData.mail_item_data[i].item_guid.server_64);
			for (int32 j = 0; j < e_item_info_max;j++)
			{
				db_row->add_data_ary(msgData.mail_item_data[i].data_ary[j]);
			}
			db_row->set_param_str(msgData.mail_item_data[i].param_str);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_mail_item);

	}

	void cs2dp_req_load_char_mail_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_item_info_db sql_data[MAX_MAIL_NUM*max_item_per_mail];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len > MAX_MAIL_NUM*max_item_per_mail || (result.query.data_select.row_size > 0 && data_size != result.query.data_select.row_size))
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_mail;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));

			dp2cs_operate_load_lock_mail operate_load_lock_mail;
			operate_load_lock_mail.unit_guid = role_guid;
			operate_load_lock_mail.unit_index = unit_array_index;
			operate_load_lock_mail.operate_value = -1;
			dbproxy_service::getInstance().send_message(connindex, &operate_load_lock_mail, sizeof(operate_load_lock_mail));
			return;
		}
		else /*if (result.query.data_select.row_count > 0)*/  // 可以为0
		{
			if (table_len > 0)
			{
				memset(sql_data, 0, sizeof(sql_data));
				db_read_data(&result, &sql_data, data_size * table_len);
			}

			dp2cs_load_char_mail_item reply_mail;
			reply_mail.role_guid = role_guid;
			reply_mail.unit_array_index = unit_array_index;
			reply_mail.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_mail.mail_item_data[i]), &(sql_data[i].data_info), sizeof(s_item_info));
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_mail_item);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_mail, reply_mail.get_pak_length());
			}
			else
			{
				cs2dp_load_role_mail_item_send_lua(connindex, reply_mail);
			}
			return;
		}
	}

	void cs2dp_req_auto_load_character_mail(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auto_load_mail_by_db* pdata = static_cast<const cs2dp_auto_load_mail_by_db*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		cs2dp_req_load_char_mail(connindex, pdata->role_guid, pdata->role_guid, pdata->unit_array_index);
	}
}