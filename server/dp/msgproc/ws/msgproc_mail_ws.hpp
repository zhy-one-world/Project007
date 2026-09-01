/********************************************************************
	created:	2016年12月12日12:53:46
	file base:	msgproc_mail_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_MAIL_WS_H_
#define _MSGPROC_MAIL_WS_H_

#include <mail_msg.hpp>
#include "db_manager.hpp"
#include "Logic/event_def.hpp"


namespace faith
{
	void check_player_is_exist(uint32 connindex, const void *data_ptr, size_t data_len);
	void player_exist_check_end(db_result_type result, uint32 connindex, const faith::ws2dp_add_mail_to_sql mail_msg);

	void del_mail_item_batch_for_add_new(uint32 connindex, guid_64 role_guid, const faith::ws2dp_add_mail_to_sql mail_msg);
	void del_mail_item_batch_for_add_new_finish(db_result_type result, uint32 connindex, guid_64 role_guid, const faith::ws2dp_add_mail_to_sql mail_msg);
	void add_mail_item_batch(uint32 connindex, guid_64 role_guid, const faith::ws2dp_add_mail_to_sql mail_msg);
	void add_new_item_finish(db_result_type result, uint32 connindex, guid_64 role_guid, const faith::ws2dp_add_mail_to_sql mail_msg);
	void add_mail(uint32 connindex, guid_64 role_guid, const faith::ws2dp_add_mail_to_sql mail_msg);
	void add_mail_finish(db_result_type result, uint32 connindex, faith::ws2dp_add_mail_to_sql mail_msg, guid_64 role_guid);
	void resend_mail_to_sender(uint32 connindex, const faith::ws2dp_add_mail_to_sql mail_msg);

	void check_id_mail_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void check_id_mail_proc_end(db_result_type result, uint32 connindex, guid_64 role_guid, int32 server_id, std::string param_str);
	void del_id_mail_proc(uint32 connindex, std::string param_str, dp2ws_check_id_mail_end msg);
	void del_id_mail_proc_end(db_result_type result, uint32 connindex, dp2ws_check_id_mail_end msg);

	void check_id_mail_proc_lua(uint32 connindex, int64 role_guid, int32 server_id, int32 cur_time);
	void save_other_server_mail_record_lua(uint32 connindex, uint32 source_server_id, uint32 target_server_id, int64 mail_guid, int64 role_guid);
	void load_other_server_mail_lua(uint32 connindex, uint32 server_id, int64 mail_guid);
	void del_other_server_mail_lua(uint32 connindex, uint32 source_server_id, uint32 target_server_id, int64 mail_guid, int64 role_guid);

	void save_globel_mail_to_sql(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_globel_mail_end(db_result_type result);
	void save_globel_mail_item_to_sql(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_globel_mail_item_end(db_result_type result, uint32 connindex);
	void load_mail_event(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_mail_event_item(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_mail_event_end(db_result_type result, uint32 connindex);
	void load_mail_event_item_end(db_result_type result, uint32 connindex);
	void save_globel_mail_new_get_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	void save_other_server_mail_record(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_other_server_mail_record(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_other_server_mail_record_end(db_result_type result, uint32 connindex);

	void load_other_server_mail(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_other_server_mail_end(db_result_type result, uint32 connindex, int32 server_id);
	void load_other_server_mail_item_end(db_result_type result, uint32 connindex, s_other_server_mail_db mail_db, int32 server_id);
	void del_other_server_mail(uint32 connindex, const void *data_ptr, size_t data_len);
	
	void save_globel_mail_to_sql_lua(uint32 connindex, const char *data_ptr, size_t data_len);
	void save_globel_mail_item_to_lua(uint32 connindex, const char *data_ptr, size_t data_len);
	void load_mail_event_lua(uint32 connindex, bool is_over);
	void load_mail_event_item_lua(uint32 connindex, bool is_over);
	void save_globel_mail_new_get_info_proc_lua(uint32 connindex, int64 role_guid, int64 data_info[max_globel_mail_in_same_time]);
}

#endif