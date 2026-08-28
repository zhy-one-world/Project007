/********************************************************************
	file name:	msgproc_relation.cpp
	created:	2018/09/15 23:32
	author:		dengronggen
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "dbproxy_service.hpp"
#include "dbproxy_service.hpp"
#include "msgproc_chat_ws.hpp"
#include <char_msg.hpp>
#include <chat_msg.hpp>
namespace hld
{
	void save_chat_with_offline(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_save_chat_with_offline* pdata = static_cast<const ws2dp_save_chat_with_offline*>(data_ptr);

		if (nullptr == pdata)
		{
			return;
		}
		
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->address_guid.server_64)<<_XTEXT(".sp_role_offline_chat_save(");
		sql << pdata->address_guid.server_64;
		sql << ",";
		sql << pdata->sender_guid.server_64;
		sql << ",'(";
		sql << pdata->chat_guid;
		sql << ",";
		sql << pdata->address_guid.server_64;
		sql << ",";
		sql << pdata->sender_guid.server_64;
		sql << ",\\'";
		sql << pdata->sender_name;
		sql << "\\',";
		sql << pdata->chat_type;
		sql << ",\\'";
		sql << pdata->content_text;
		sql << "\\',";
		sql << pdata->sender_templete_id;
		sql << ",";
		sql << pdata->relation_template_id;
		sql << ",";
		sql << pdata->relation_level;
		sql << ",";
		sql << pdata->vip_title_template_id;
		sql << ")');";

		db_query_type query;
		query.logic_id = e_data_flag_chat;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_chat_with_offline(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_load_chat_with_offline* pdata = static_cast<const ws2dp_load_chat_with_offline*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_offline_chat_load(");
		sql << pdata->role_guid.server_64;
		sql << ");";

		db_query_type query;
		query.handler = boost::bind(&load_chat_with_offline_end, _1, connindex, pdata->role_guid);
		query.logic_id = e_data_flag_chat;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_chat_with_offline_end(db_result_type result,uint32 connindex, const guid_64& role_guid)
	{
		int32 table_len = result.query.data_select.row_count;
		int32 chat_info_size = sizeof(e_chat_offline_info);
		dp2ws_load_chat_with_offline msg;
		msg.role_guid = role_guid;

		if (result.error || table_len > chat_offline_num_max || chat_info_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());
		}
		else
		{
			memset(msg.chat_info_list, 0, sizeof(msg.chat_info_list));
			db_read_data(&result, msg.chat_info_list, chat_info_size * table_len);
			msg.data_num = table_len;
			msg.role_guid = role_guid;

			dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());
		}
	}

	void eliminate_chat_with_offline(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_eliminate_chat_with_offline* pdata = static_cast<const ws2dp_eliminate_chat_with_offline*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_offline_chat_eliminate(");
		sql << pdata->role_guid.server_64;
		sql << ");";

		db_query_type query;
		query.logic_id = e_data_flag_chat;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}



