/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:56
	file base:	msgproc_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_WS_H_
#define _MSGPROC_WS_H_

#include <net/tcp_client.hpp>
#include <char_msg.hpp>
#include "db_manager.hpp"

namespace faith
{
	// 获取游戏配置信息给ws
	void get_game_info_ws(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_get_game_info_ws_proc(db_result_type result, uint32 connindex);
	void gm_reload_csv(uint32 connindex, const void *data_ptr, size_t data_len);
	void gm_set_open_time(uint32 connindex, const void *data_ptr, size_t data_len);
	void gm_set_new_password(uint32 connindex, const void *data_ptr, size_t data_len);
	void gm_set_begin_cross(uint32 connindex, const void *data_ptr, size_t data_len);
	void gm_set_cross_open_time(uint32 connindex, const void *data_ptr, size_t data_len);
	void send_to_cs_dp_connect_success(uint32 connindex, const void *data_ptr, size_t data_len);


	void gm_set_open_time_lua(uint32 connindex, uint32 open_time, uint32 listen_port, int64 handle_index);
	void gm_set_cross_open_time_lua(uint32 connindex, uint32 server_info_value, uint32 server_info_type,uint32 listen_port, int64 handle_index);
	void gm_set_new_password_lua(uint32 connindex, xstring account, xstring new_password, uint32 listen_port, int64 handle_index);
	void gm_set_begin_cross_lua(uint32 connindex, uint32 need_begin_cross, uint32 gm_type, int64 req_http_id);
	void save_server_last_server_list_lua(uint32 connindex, uint32 server_id, xstring last_server_list);
	void load_server_last_server_list_lua(uint32 connindex, uint32 server_id);

	void save_server_last_server_list(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_last_server_list_end(db_result_type result, uint32 connindex);
	void load_server_last_server_list(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_server_last_server_list_end(db_result_type result, uint32 connindex);

	// 获取游戏角色信息给ws
	void get_char_info_ws(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_get_char_info_ws_proc(db_result_type result, uint32 connindex, guid_64 role_guid, xstring role_account);

	//gm
	void get_char_info_ws_lua(uint32 connindex, int64 role_guid, xstring role_account);
	void gm_set_role_right_lua(uint32 connindex, xstring role_mark, uint32 right_type, uint32 right_value, uint32 right_time, uint32 listen_port, int64 handle_index);
	void gm_change_account_lua(uint32 connindex, xstring change_name, xstring target_name, uint32 listen_port, int64 handle_index);
	void gm_del_role_cache_lua(uint32 connindex, int64 role_guid);
	void gm_del_role_lua(uint32 connindex, int64 role_guid, uint32 is_del, uint32 listen_port, int64 handle_index);
	void gm_stop_login_lua(uint32 connindex, uint32 is_stop, uint32 listen_port, int64 handle_index);
	void gm_get_role_info_lua(uint32 connindex, xstring role_mark, uint32 get_type, uint32 listen_port, int64 handle_index, uint32 check_type);
	void gm_modify_role_info_lua(uint32 connindex, int64 role_guid, xstring sql_str, uint32 listen_port, int64 handle_index);
	void gm_get_role_items_info_lua(uint32 connindex, int64 role_guid, uint32 listen_port, int64 handle_index);

	void gm_set_role_right(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_gm_set_proc(db_result_type result, uint32 connindex, s_daemon_callback_info callback_info);
	void save_recharge(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_save_recharge_end(db_result_type result, uint32 connindex, ws2dp_save_recharge recharge_info);
	void save_recharge_end(uint32 connindex, const void *data_ptr, size_t data_len);
	
	void gm_change_account(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_gm_change_account(db_result_type result, uint32 connindex, s_daemon_callback_info callback_info);

	void gm_del_role_cache(uint32 connindex, const void *data_ptr, size_t data_len);

	void gm_del_role(uint32 connindex, const void *data_ptr, size_t data_len);

	void gm_stop_login(uint32 connindex, const void *data_ptr, size_t data_len);

	void save_server_attr_val_lua(uint32 connindex, uint32 attr_type, int64 attr_value);
	void load_server_attr_val_lua(uint32 connindex);
	void save_server_attr_val(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_server_attr_val_end(db_result_type result, uint32 connindex);
	void load_server_attr_val(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_server_attr_val_end(db_result_type result, uint32 connindex);
	void gm_get_role_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_gm_get_role_info_proc(db_result_type result, uint32 connindex, s_daemon_callback_info callback_info, int32 check_type, std::string	role_mark);
	void gm_modify_role_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_gm_modify_role_info_end(db_result_type result, uint32 connindex, guid_64 role_guid, s_daemon_callback_info callback_info);
	void gm_get_role_time(uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info);
	void handler_gm_get_role_time_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info);
	void gm_get_role_money(uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info);
	void handler_gm_get_role_money_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info);
	void gm_get_role_lock(uint32 connindex, xchar* account, guid_64& role_guid, xchar* role_name, s_daemon_callback_info callback_info);
	void handler_gm_get_role_lock_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info);
	void gm_get_role_items_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_gm_get_role_items_info_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info);

	void gm_get_server_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void handler_gm_get_server_info_proc(db_result_type result, uint32 connindex);

	//角色改名
	void ws2dp_req_check_player_name(uint32 connindex, const void* data_ptr, size_t data_len);
		void ws2dp_req_check_player_name_(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_check_player_name_end(db_result_type result, uint32 connindex, guid_64 role_guid, std::string role_name);
	void ws2dp_req_change_player_name(uint32 connindex, const void* data_ptr, size_t data_len);
		void ws2dp_req_change_player_name_(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_change_player_name_end(db_result_type result, uint32 connindex, guid_64 role_guid, std::string role_name);

	void save_ws_time_limit_func(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_ws_time_limit_func_end(db_result_type result, uint32 connindex);
	void load_ws_time_limit_func(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_ws_time_limit_func_end(db_result_type result, uint32 connindex);

	//保存账号当时在线时长
	void save_account_online_time(uint32 connindex, const void* data_ptr, size_t data_len);
	void save_account_online_time_end(db_result_type result, uint32 connindex);

	void set_all_player_check_guide_info(uint32 connindex);
	void set_all_player_check_guide_info_end(db_result_type result, uint32 connindex);
}

#endif