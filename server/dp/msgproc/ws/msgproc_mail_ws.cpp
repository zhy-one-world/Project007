/********************************************************************
created:	2016年12月12日12:54:24
file base:	msgproc_mail_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_mail_ws.hpp"
#include <event_msg.hpp>
#include "utility/guid_gen.h"
#include "utility/init_unit.h"
#include "dbproxy_service.hpp"
#include "utility/parse_msg.h"
#include "server_log.hpp"

namespace hld
{
	

	void check_player_is_exist(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const hld::ws2dp_add_mail_to_sql* pdata = static_cast<const hld::ws2dp_add_mail_to_sql*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		std::string role_guid_str(pdata->role_mark);
		guid_64 role_guid;
		role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);

		if (pdata->mail_info.data_ary[EMailInfo_ServerId] > 0)
		{
			std::string role_guid_str(pdata->role_mark);
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
			del_mail_item_batch_for_add_new(connindex, role_guid, *pdata);
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_gm_get_role_info('") << pdata->role_mark << _XTEXT("',") << pdata->mark_type << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&player_exist_check_end, _1, connindex, *pdata);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void player_exist_check_end(db_result_type result, uint32 connindex, const hld::ws2dp_add_mail_to_sql mail_msg)
	{
		if (!result.error && result.query.data_select.row_count > 0)
		{
			s_unit_info data[hld::max_character_num];
			//s_unit_info data[160];
			memset(&data, 0, sizeof(data));
			int32 data_num = max_character_num > result.query.data_select.row_count ? result.query.data_select.row_count : max_character_num;
			db_read_data(&result, &data, sizeof(s_unit_info) * data_num);
			for (int32 i = 0; i < data_num; ++i)
			{
				if (!data[i].role_guid.is_valid())
				{
					return;
				}
				ws2dp_add_mail_to_sql new_msg;
				new_msg = mail_msg;
				if (i > 0)
				{//一次发一个以上的邮件，需要重设各种GUID
					new_msg.mail_info.mail_guid = guid_gen::make_guid();
					for (int32 item_index = 0; item_index < new_msg.item_num; ++item_index)
					{
						if (item_index >= max_item_per_mail)
						{
							break;
						}
						guid_64 new_guid = guid_gen::make_guid();
						new_msg.item_list[item_index].item_guid = new_guid;
						new_msg.mail_info.data_ary[EMailInfo_ItemGuid11 + item_index * 2] = new_guid.A;
						new_msg.mail_info.data_ary[EMailInfo_ItemGuid12 + item_index * 2] = new_guid.B;
					}
				}
				del_mail_item_batch_for_add_new(connindex, data[i].role_guid, mail_msg);
			}
		}
		else
		{
			resend_mail_to_sender(connindex, mail_msg);
		}
	}

	void del_mail_item_batch_for_add_new(uint32 connindex, guid_64 role_guid, const hld::ws2dp_add_mail_to_sql mail_msg)
	{
		if (!role_guid.is_valid())
		{
			return;
		}

		if (mail_msg.item_num > 0)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_mail_item_delete_batch(") << "'";

			bool is_first_guid = true;
			for (int32 i = 0; i < mail_msg.item_num; ++i)
			{
				if (i >= max_item_per_mail)
				{
					break;
				}
				guid_64 item_guid = mail_msg.item_list[i].item_guid;
				if (!item_guid.is_valid())
				{
					continue;
				}

				if (!is_first_guid)
				{
					sql << ",";
				}

				sql << item_guid.server_64;

				is_first_guid = false;
			}

			sql << "');";

			db_query_type query;
			query.role_guid = role_guid;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&del_mail_item_batch_for_add_new_finish, _1, connindex, role_guid, mail_msg);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
		else
		{
			add_mail(connindex, role_guid, mail_msg);
		}
	}

	void del_mail_item_batch_for_add_new_finish(db_result_type result, uint32 connindex, guid_64 role_guid, const hld::ws2dp_add_mail_to_sql mail_msg)
	{
		//插入物品
		if (!result.error)
		{
			add_mail_item_batch(connindex, role_guid, mail_msg);
		}
		else
		{
			resend_mail_to_sender(connindex, mail_msg);
		}
	}

	void add_mail_item_batch(uint32 connindex, guid_64 role_guid, const hld::ws2dp_add_mail_to_sql mail_msg)
	{
		if (!role_guid.is_valid())
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_mail_item_add_batch('");

		bool is_first_item = true;

		int32 single_info_end_index = e_item_info_max - 1;
		for (int32 i = 0; i < mail_msg.item_num; i++)
		{
			const s_item_info& item_info = mail_msg.item_list[i];
			if (!item_info.item_guid.is_valid())
			{
				continue;
			}
			if (!is_first_item)
			{
				sql << ",";
			}

			sql << "(" << role_guid.server_64 << ","
				<< mail_msg.item_list[i].item_guid.server_64 << ",";

			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				//if (j != single_info_end_index)
				//{
				//	sql << item_info.data_ary[j] << ",";
				//}
				//else
				//{
				//	sql << item_info.data_ary[j];
				//}
				sql << item_info.data_ary[j] << ",";
			}
			sql << "\\\'" << item_info.param_str << "\\\'";

			sql << ")";

			is_first_item = false;
		}
		sql << "');";

		db_query_type query;
		query.role_guid = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&add_new_item_finish, _1, connindex, role_guid, mail_msg);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void add_new_item_finish(db_result_type result, uint32 connindex, guid_64 role_guid, const hld::ws2dp_add_mail_to_sql mail_msg)
	{

		CONSOLE_INFO("add_new_item_finish result = {} Mail Guid = {}", result.error, mail_msg.mail_info.mail_guid.server_64);

		//插入邮件本体
		if (!result.error)
		{
			add_mail(connindex, role_guid, mail_msg);
		}
		else
		{
			resend_mail_to_sender(connindex, mail_msg);
		}
	}

	void add_mail(uint32 connindex, guid_64 role_guid, const hld::ws2dp_add_mail_to_sql mail_msg)
	{
		if (!role_guid.is_valid())
		{
			CONSOLE_INFO("No Guid  Mail Guid = {}", mail_msg.mail_info.mail_guid.server_64);
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_mail_add(") << role_guid.server_64 << ","
			<< mail_msg.mail_info.mail_guid.server_64 << ",'";
		int32 sqlEnd = EMailInfo_max - 1;

		guid_64 item1_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_ItemGuid11], mail_msg.mail_info.data_ary[EMailInfo_ItemGuid12]);
		guid_64 item2_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_ItemGuid21], mail_msg.mail_info.data_ary[EMailInfo_ItemGuid22]);
		guid_64 item3_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_ItemGuid31], mail_msg.mail_info.data_ary[EMailInfo_ItemGuid32]);
		guid_64 item4_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_ItemGuid41], mail_msg.mail_info.data_ary[EMailInfo_ItemGuid42]);
		guid_64 item5_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_ItemGuid51], mail_msg.mail_info.data_ary[EMailInfo_ItemGuid52]);
		guid_64 sender_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_SenderGuid1], mail_msg.mail_info.data_ary[EMailInfo_SenderGuid2]);

		sql << "(" << role_guid.server_64 << ","
			<< mail_msg.mail_info.mail_guid.server_64 << ",";

		sql << mail_msg.mail_info.data_ary[EMailInfo_IsNeedDelete] << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_SendTime] << ",";
		sql << item1_guid.server_64 << ",";
		sql << item2_guid.server_64 << ",";
		sql << item3_guid.server_64 << ",";
		sql << item4_guid.server_64 << ",";
		sql << item5_guid.server_64 << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_MoneyTyp1] << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_MoneyNum1] << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_MoneyTyp2] << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_MoneyNum2] << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_ReadState] << ",";
		sql << sender_guid.server_64 << ",";
		sql << mail_msg.mail_info.data_ary[EMailInfo_ServerId];


		sql << ",\\\'" << mail_msg.mail_info.text_contents << "\\\',";
		sql << "\\\'" << mail_msg.mail_info.sender_name << "\\\',";
		sql << "\\\'" << mail_msg.mail_info.mail_title << "\\\'";

		sql << ")";
		sql << "');";

		//if (item1_guid.is_valid())
		//{
		//	CONSOLE_INFO("sql = {}", sql.c_str());
		//}
		db_query_type query;
		query.role_guid = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&add_mail_finish, _1, connindex, mail_msg, role_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void add_mail_finish(db_result_type result, uint32 connindex, hld::ws2dp_add_mail_to_sql mail_msg, guid_64 role_guid)
	{
		if (mail_msg.item_list[0].is_valid())
		{
			CONSOLE_INFO("result = {} mail_guid = {}", result.error, mail_msg.mail_info.mail_guid.server_64);
		}

		if (!result.error)
		{
		}
		else
		{
			resend_mail_to_sender(connindex, mail_msg);
		}
	}

	void resend_mail_to_sender(uint32 connindex, const hld::ws2dp_add_mail_to_sql mail_msg)
	{
		CONSOLE_INFO("Mail Guid = {}", mail_msg.mail_info.mail_guid.server_64);
		if (mail_msg.resend_times > 5)
		{
			//防错，防止数据错误反复回发
			return;
		}
		guid_64 sender_guid = guid_64(mail_msg.mail_info.data_ary[EMailInfo_SenderGuid1], mail_msg.mail_info.data_ary[EMailInfo_SenderGuid2]);
		hld::ws2dp_add_mail_to_sql resend_msg;
		memcpy(&resend_msg, &mail_msg, sizeof(ws2dp_add_mail_to_sql));
		std::string role_guid_str = init_unit::change_i64_to_string(sender_guid.server_64);
		memcpy(resend_msg.role_mark, role_guid_str.c_str(), role_guid_str.size());
		resend_msg.mark_type = e_unit_guid_role_guid;
		resend_msg.resend_times += 1;

		del_mail_item_batch_for_add_new(connindex, sender_guid, resend_msg);
	}

	void check_id_mail_proc_lua(uint32 connindex, int64 role_guid, int32 server_id, int32 cur_time)
	{
		guid_64 c_role_guid = role_guid;
		if (!c_role_guid.is_valid())
		{
			return;
		}
		std::string param_string;
		std::stringstream temp_stringstream;

		temp_stringstream << "(";
		temp_stringstream << role_guid << ",";
		temp_stringstream << cur_time << ",";
		temp_stringstream << check_id_mail_per_time;
		temp_stringstream << ");";
		temp_stringstream >> param_string;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_mail_id_mail_load");
		sql << param_string;

		db_query_type query;
		query.role_guid = (guid_64)role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&check_id_mail_proc_end, _1, connindex, (guid_64)role_guid, server_id, param_string);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void check_id_mail_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_check_id_mail request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		check_id_mail_proc_lua(connindex, request.role_guid(), request.server_id(), request.cur_time());
	}

	void check_id_mail_proc_end(db_result_type result, uint32 connindex, guid_64 role_guid, int32 server_id, std::string param_str)
	{
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_id_mail_info);
		if (result.error || table_len > check_id_mail_per_time || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else if (table_len > 0)
		{
			dp2ws_check_id_mail_end reply_mail;
			reply_mail.role_guid = role_guid;
			reply_mail.server_id = server_id;
			reply_mail.data_num = table_len;
			db_read_data(&result, &reply_mail.data_array, data_size * table_len);

			del_id_mail_proc(connindex, param_str, reply_mail);
		}
	}

	void del_id_mail_proc(uint32 connindex, std::string param_str, dp2ws_check_id_mail_end msg)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(msg.role_guid.server_64) << _XTEXT(".sp_mail_id_mail_del");
		sql << param_str;

		db_query_type query;
		query.role_guid = msg.role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&del_id_mail_proc_end, _1, connindex, msg);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void del_id_mail_proc_end(db_result_type result, uint32 connindex, dp2ws_check_id_mail_end msg)
	{
		dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(msg));
	}

	void save_globel_mail_to_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		packet_s2s* pPacket = (packet_s2s*)data_ptr;
		save_globel_mail_to_sql_lua(connindex, (char*)pPacket->google_data, pPacket->google_data_len);
	}

	void save_globel_mail_end(db_result_type result)
	{
		if (result.error)
		{

		}
		else
		{

		}

	}

	void save_globel_mail_item_to_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		packet_s2s* pPacket = (packet_s2s*)data_ptr;
		save_globel_mail_item_to_lua(connindex, (char*)pPacket->google_data, pPacket->google_data_len);
	}

	void save_globel_mail_item_end(db_result_type result, uint32 connindex)
	{
		if (result.error)
		{

		}
		else
		{

		}

	}

	void load_mail_event(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_mail_event_lua(connindex, false);
	}

	void load_mail_event_item(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_mail_event_item_lua(connindex, false);
	}

	void load_mail_event_end(db_result_type result, uint32 connindex)
	{
		s_mail_info_db sql_data[max_globel_mail_in_same_time];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_mail_info_db);
		if (result.error || table_len > max_globel_mail_in_same_time || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_mail_event_end reply_mail;
			reply_mail.is_over = true;

			// 群发邮件只作为模版存在ws上，并不属于任何人
			reply_mail.role_guid = 0;

			reply_mail.mail_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_mail.mail_info[i]), &(sql_data[i].data_info), sizeof(s_mail_info));
			}

			dbproxy_service::getInstance().send_message(connindex, &reply_mail, sizeof(reply_mail));
			return;
		}
	}

	void load_mail_event_item_end(db_result_type result, uint32 connindex)
	{
		s_item_info_db sql_data[max_item_per_mail * max_globel_mail_in_same_time];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len > max_item_per_mail * max_globel_mail_in_same_time || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_mail_event_item_end reply_item;
			//这些物品（即这条消息）不属于任何人，item中的role_guid为0，只是为了配合 s_item_info_db 的数据格式占位用
			reply_item.is_over = true;
			reply_item.item_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_item.item_list[i]), &(sql_data[i].data_info), sizeof(s_item_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_item, sizeof(reply_item));
			return;
		}
	}

	void save_globel_mail_new_get_info_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_new_globel_mail_had_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int64 data_info[max_globel_mail_in_same_time];
		for (int32 i = 0; i < max_globel_mail_in_same_time; i++)
		{
			if (request.data_info_size() <= i)
			{
				break;
			}
			data_info[i] = request.data_info(i);
		}
		save_globel_mail_new_get_info_proc_lua(connindex, request.role_guid(), data_info);
	}

	void save_other_server_mail_record_lua(uint32 connindex, uint32 source_server_id, uint32 target_server_id, int64 mail_guid, int64 role_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_save_other_server_mail_record(") << source_server_id << "," << target_server_id << ",'";
		sql << "(" << source_server_id << ",";
		sql << target_server_id << ",";
		sql << mail_guid << ",";
		sql << role_guid << ")')";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_other_server_mail_record(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_other_server_mail_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_other_server_mail_record_lua(connindex, request.source_server_id(), request.target_server_id(), request.mail_guid(), request.role_guid());
	}

	void load_other_server_mail_record(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_load_other_server_mail_record();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_other_server_mail_record_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_other_server_mail_record_end(db_result_type result, uint32 connindex)
	{
		s_other_server_mail_record sql_data[max_other_server_mail_record_num];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_other_server_mail_record);
		if (result.error || table_len > max_other_server_mail_record_num || (result.query.data_select.row_size > 0 && data_size != result.query.data_select.row_size))
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_other_server_mail_record_end reply;
			reply.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply.mail_record[i]), &(sql_data[i]), sizeof(s_other_server_mail_record));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply, reply.get_pak_length());
			return;
		}
	}

	void load_other_server_mail_lua(uint32 connindex, uint32 server_id, int64 mail_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_load_other_server_mail(") << server_id << "," << mail_guid << "," << max_other_server_mail_num << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_other_server_mail_end, _1, connindex, server_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_other_server_mail(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_other_server_mail request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_other_server_mail_lua(connindex, request.server_id(), request.mail_guid());
	}

	void load_other_server_mail_end(db_result_type result, uint32 connindex, int32 server_id)
	{
		s_other_server_mail_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_mail_info_db);
		if (result.error || table_len > max_other_server_mail_num || (result.query.data_select.row_size > 0 && data_size != result.query.data_select.row_size))
		{
			return;
		}
		else if (table_len <= 0)
		{
			return;
		}
		else
		{
			db_read_data(&result, &sql_data.mail_info_db, data_size * table_len);

			guid_64	mail_item_guid_array[max_other_server_mail_item_num];
			memset(mail_item_guid_array, 0, sizeof(guid_64) * max_other_server_mail_item_num);
			int32 mail_item_count = 0;
			for (int32 i = 0; i < table_len; ++i)
			{
				for (int32 j = 0; j < max_item_per_mail; ++j)
				{// 存储load下来的邮件附件物品的guid
					guid_64 temp_guid;
					temp_guid.A = sql_data.mail_info_db[i].data_info.data_ary[EMailInfo_ItemGuid11 + j * 2];
					temp_guid.B = sql_data.mail_info_db[i].data_info.data_ary[EMailInfo_ItemGuid11 + j * 2 + 1];
					if (temp_guid.is_valid())
					{
						mail_item_guid_array[mail_item_count] = temp_guid;
						mail_item_count++;
					}
				}
			}

			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call sp_role_mail_item_load('");

			int32 last_index = mail_item_count - 1;
			for (int32 i = 0; i < mail_item_count; ++i)
			{
				sql << mail_item_guid_array[i].server_64;

				if (i != last_index)
				{
					sql << ",";
				}
			}
			sql << "');";
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&load_other_server_mail_item_end, _1, connindex, sql_data, server_id);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
			return;
		}
	}

	void load_other_server_mail_item_end(db_result_type result, uint32 connindex, s_other_server_mail_db mail_db, int32 server_id)
	{
		s_item_info_db sql_data[max_other_server_mail_item_num];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len > max_other_server_mail_item_num || (result.query.data_select.row_size > 0 && data_size != result.query.data_select.row_size))
		{
			return;
		}
		else
		{
			if (table_len > 0)
			{
				memset(sql_data, 0, sizeof(sql_data));
				db_read_data(&result, &sql_data, data_size * table_len);
			}

			dp2ws_load_other_server_mail_end reply;
			reply.server_id = server_id;
			for (int32 i = 0; i < max_other_server_mail_num; i++)
			{
				if (mail_db.mail_info_db[i].role_guid.is_valid() == false)
				{
					break;
				}
				reply.data_num++;
				reply.mail_info[i].mail_info_db = mail_db.mail_info_db[i];

				int32 item_num = 0;
				for (int32 j = 0; j < max_other_server_mail_item_num; j++)
				{
					if (sql_data[j].role_guid.is_valid() == false)
					{
						break;
					}
					for (int32 k = 0; k < max_item_per_mail; k++)
					{
						guid_64 temp_guid;
						temp_guid.A = reply.mail_info[i].mail_info_db.data_info.data_ary[EMailInfo_ItemGuid11 + k * 2];
						temp_guid.B = reply.mail_info[i].mail_info_db.data_info.data_ary[EMailInfo_ItemGuid11 + k * 2 + 1];
						if (temp_guid.is_valid() && temp_guid == sql_data[j].data_info.item_guid)
						{
							reply.mail_info[i].item_list[item_num] = sql_data[j].data_info;
							item_num++;
							break;
						}
					}
					if (item_num >= max_item_per_mail)
					{
						break;
					}
				}
			}
			dbproxy_service::getInstance().send_message(connindex, &reply, reply.get_pak_length());
			return;
		}
	}

	void del_other_server_mail_lua(uint32 connindex, uint32 source_server_id, uint32 target_server_id, int64 mail_guid, int64 role_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_role_mail_delete_batch(");
		sql <<mail_guid;
		sql << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void del_other_server_mail(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_del_other_server_mail request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		del_other_server_mail_lua(connindex, request.source_server_id(), request.target_server_id(), request.mail_guid(), request.role_guid());
	}

	void save_globel_mail_to_sql_lua(uint32 connindex, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_mail_event_to_db request;
		if (!parse_msg::getInstance().parse_buffer_to_proto(&request, data_ptr, data_len))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_mail_globel_mail_save(") << "'";
		int32 tempLen = request.mail_info_size() - 1;
		int32 sqlEnd = EMailInfo_max - 1;

		for (int32 i = 0; i < request.mail_info_size(); i++)
		{
			guid_64 item1_guid = guid_64(request.mail_info(i).data_ary(EMailInfo_ItemGuid11), request.mail_info(i).data_ary(EMailInfo_ItemGuid12));
			guid_64 item2_guid = guid_64(request.mail_info(i).data_ary(EMailInfo_ItemGuid21), request.mail_info(i).data_ary(EMailInfo_ItemGuid22));
			guid_64 item3_guid = guid_64(request.mail_info(i).data_ary(EMailInfo_ItemGuid31), request.mail_info(i).data_ary(EMailInfo_ItemGuid32));
			guid_64 item4_guid = guid_64(request.mail_info(i).data_ary(EMailInfo_ItemGuid41), request.mail_info(i).data_ary(EMailInfo_ItemGuid42));
			guid_64 item5_guid = guid_64(request.mail_info(i).data_ary(EMailInfo_ItemGuid51), request.mail_info(i).data_ary(EMailInfo_ItemGuid52));
			guid_64 sender_guid = guid_64(request.mail_info(i).data_ary(EMailInfo_SenderGuid1), request.mail_info(i).data_ary(EMailInfo_SenderGuid2));


			// 此时邮件不属于任何人，所以“role_guid”位置无意义，单位了配合 s_mail_info_db 的数据类型，所以填0占位
			sql << "(" << 0 << ","
				<< request.mail_info(i).role_guid() << ",";

			sql << request.mail_info(i).data_ary(EMailInfo_IsNeedDelete) << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_SendTime) << ",";
			sql << item1_guid.server_64 << ",";
			sql << item2_guid.server_64 << ",";
			sql << item3_guid.server_64 << ",";
			sql << item4_guid.server_64 << ",";
			sql << item5_guid.server_64 << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_MoneyTyp1) << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_MoneyNum1) << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_MoneyTyp2) << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_MoneyNum2) << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_ReadState) << ",";
			sql << sender_guid.server_64 << ",";
			sql << request.mail_info(i).data_ary(EMailInfo_ServerId);

			sql << ",\\\'" << request.mail_info(i).text_contents() << "\\\',";
			sql << "\\\'" << request.mail_info(i).sender_name() << "\\\',";
			sql << "\\\'" << request.mail_info(i).mail_title() << "\\\'";

			if (i != tempLen)
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
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_globel_mail_end, _1);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_globel_mail_item_to_lua(uint32 connindex, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_mail_event_item_to_db request;
		if (!parse_msg::getInstance().parse_buffer_to_proto(&request, data_ptr, data_len))
		{
			return;
		}
		if (request.item_list_size() < 1 || request.item_list_size() >= max_item_per_mail * max_globel_mail_in_same_time)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_mail_globel_mail_item_save(") << "'";
		int32 tempLen = request.item_list_size() - 1;
		int32 sqlEnd = e_item_info_max - 1;
		for (int32 i = 0; i < request.item_list_size(); i++)
		{
			// 此时，物品的不属于任何人，role_guid无意义，但为了配合 s_item_info_db的数据格式，填0占位
			sql << "(" << 0 << ","
				<< request.item_list(i).item_guid() << ",";
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				sql << request.item_list(i).data_ary(j) << ",";
			}
			sql << "\\\'" << request.item_list(i).param_str() << "\\\'";
			if (i != tempLen)
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
		query.role_guid.server_64 = request.mail_guid();
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_globel_mail_item_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_mail_event_lua(uint32 connindex, bool is_over)
	{
		sql_builder sql_mail(db_manager::getInstance().get_db_link().game_db);
		sql_mail << _XTEXT("call sp_mail_globel_mail_load();");

		db_query_type query_mail;
		query_mail.sql_str.assign(sql_mail.c_str(), sql_mail.size());
		query_mail.handler = boost::bind(&load_mail_event_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query_mail);
	}

	void load_mail_event_item_lua(uint32 connindex, bool is_over)
	{
		sql_builder sql_mail_item(db_manager::getInstance().get_db_link().game_db);
		sql_mail_item << _XTEXT("call sp_mail_globel_mail_item_load();");

		db_query_type query_mail_item;
		query_mail_item.sql_str.assign(sql_mail_item.c_str(), sql_mail_item.size());
		query_mail_item.handler = boost::bind(&load_mail_event_item_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query_mail_item);
	}

	void save_globel_mail_new_get_info_proc_lua(uint32 connindex, int64 role_guid, int64 data_info[max_globel_mail_in_same_time])
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_role_event_save(") << role_guid << ",'";
		int32 sqlEnd = max_globel_mail_in_same_time - 1;

		sql << "(" << role_guid << ",";

		for (int32 j = 0; j < max_globel_mail_in_same_time; ++j)
		{
			if (j != sqlEnd)
			{
				sql << data_info[j] << ",";
			}
			else
			{
				sql << data_info[j];
			}
		}

		sql << ")";

		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_globel_mail_had;
		query.role_guid.server_64 = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

}
