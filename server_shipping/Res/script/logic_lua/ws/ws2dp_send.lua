local ws2dp_send_class={}

function ws2dp_send_class:init()

end

function ws2dp_send_class:send_gm_open_time_lua(s_open_time, s_listen_port, s_handle_index)
	print("send_gm_open_time_lua",s_open_time,s_listen_port,s_handle_index)
	local  t_ret_msg = {
          open_time = s_open_time,
		  listen_port = s_listen_port,
		  handle_index = s_handle_index,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_open_time",t_ret_msg, 1323, 0);
end

function ws2dp_send_class:send_gm_set_server_info_lua(s_server_info_value, s_server_info_type, s_listen_port, s_handle_index)
	print("send_gm_set_server_info_lua",s_server_info_value,s_server_info_type,s_listen_port,s_handle_index)
	local  t_ret_msg = {
          server_info_value = s_server_info_value,
		  server_info_type = s_server_info_type,
		  listen_port = s_listen_port,
		  handle_index = s_handle_index,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_set_server_info",t_ret_msg, 1331, 0);
end

function ws2dp_send_class:send_gm_set_new_password_lua(s_account, s_new_password, s_listen_port, s_handle_index)
	print("send_gm_set_new_password_lua",s_account,s_new_password,s_listen_port,s_handle_index)
	local  t_ret_msg = {
          account = s_server_info_value,
		  new_password = s_server_info_type,
		  listen_port = s_listen_port,
		  handle_index = s_handle_index,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_change_password",t_ret_msg, 1324, 0);
end

function ws2dp_send_class:send_gm_set_begin_cross_lua(s_need_begin_cross, s_gm_type, s_req_http_id)
	print("send_gm_set_begin_cross_lua",s_need_begin_cross,s_gm_type,s_listen_port)
	local  t_ret_msg = {
          need_begin_cross = s_need_begin_cross,
		  gm_type = s_gm_type,
		  req_http_id = s_req_http_id,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_need_begin_cross",t_ret_msg, 1325, 0);
end

function ws2dp_send_class:send_save_server_last_server_list_lua(s_server_id, s_last_server_list)
	local  t_ret_msg = {
          server_id = s_server_id,
		  last_server_list = s_last_server_list,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_last_server_list",t_ret_msg, 1333, 0);
end

function ws2dp_send_class:send_load_server_last_server_list_lua(s_server_id)
	local  t_ret_msg = {
          server_id = s_server_id,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_last_server_list",t_ret_msg, 1334, 0);
end

function ws2dp_send_class:send_gm_set_role_right_lua(s_role_mark, s_right_type, s_right_value, s_right_time, s_listen_port, s_handle_index)
	local  t_ret_msg = {
          role_mark = s_role_mark,
		  right_type = s_right_type,
		  right_value = s_right_value,
		  right_time = s_right_time,
		  listen_port = s_listen_port,
		  handle_index = s_handle_index,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_set_role_right",t_ret_msg, 1312, 0);
end

function ws2dp_send_class:send_gm_change_account_lua(s_change_name, s_target_name, s_listen_port, s_handle_index)
	local  t_ret_msg = {
          change_name = s_change_name,
		  target_name = s_target_name,
		  listen_port = s_listen_port,
		  handle_index = s_handle_index,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_change_account",t_ret_msg, 1326, 0);
end

function ws2dp_send_class:send_gm_del_role_cache_lua(s_role_guid)
	local  t_ret_msg = {
          role_guid = s_role_guid,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_del_cache",t_ret_msg, 1328, 0);
end

function ws2dp_send_class:send_gm_del_role_lua(s_role_guid,s_is_del, s_listen_port, s_handle_index)
	local  t_ret_msg = {
          role_guid = s_role_guid,
          is_del = s_is_del,		  
          listen_port = s_listen_port,
          handle_index = s_handle_index,		  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_del_role",t_ret_msg, 1329, 0);
end

function ws2dp_send_class:send_gm_stop_login_lua(s_is_stop, s_listen_port, s_handle_index)
	local  t_ret_msg = {
          is_stop = s_is_stop,		  
          listen_port = s_listen_port,
          handle_index = s_handle_index,		  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_stop_login",t_ret_msg, 1330, 0);
end

function ws2dp_send_class:send_get_char_info_ws_lua(s_role_guid, s_role_account, s_server_id)
	local  t_ret_msg = {  
          role_guid = s_role_guid,
          role_account = s_role_account,		  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_character",t_ret_msg, 1336, s_server_id);
end

function ws2dp_send_class:send_gm_get_role_info_lua(s_role_mark, s_get_type, s_listen_port, s_handle_index,s_check_type)
	local  t_ret_msg = {  
          role_mark = s_role_mark,
          get_type = s_get_type,
          listen_port = s_listen_port,
          handle_index = s_handle_index,
          check_type = s_check_type,		  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_get_role_info",t_ret_msg, 1313, 0);
end

function ws2dp_send_class:send_gm_modify_role_info_lua(s_role_guid, s_sql_str, s_listen_port, s_handle_index)
	local  t_ret_msg = {  
          role_guid = s_role_guid,
          sql_str = s_sql_str,
          listen_port = s_listen_port,
          handle_index = s_handle_index,	  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_modify_role_info",t_ret_msg, 1316, 0);
end

function ws2dp_send_class:send_gm_get_role_items_info_lua(s_role_guid, s_listen_port, s_handle_index)
	local  t_ret_msg = {  
          role_guid = s_role_guid,
          listen_port = s_listen_port,
          handle_index = s_handle_index,	  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_gm_get_role_items_info",t_ret_msg, 1322, 0);
end

function ws2dp_send_class:send_ws2dp_req_load_offline_other_relation_lua(s_sender_guid, s_list_type, s_other_guid)
	local  t_ret_msg = {  
          sender_guid = s_sender_guid,
          list_type = s_list_type,
          other_guid = s_other_guid,	  
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_relation_info",t_ret_msg, 1371, 0);
end

function ws2dp_send_class:send_ws2dp_req_load_player_relation_lua(s_sender_guid)
	local  t_ret_msg = {  
          sender_guid = s_sender_guid, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_req_load_relation_list",t_ret_msg, 1373, 0);
end

function ws2dp_send_class:send_ws2dp_req_del_relation_lua(s_role_guid,s_target_role_guid,s_relation_type)
	local  t_ret_msg = {  
          role_guid = s_role_guid, 
		  target_role_guid = s_target_role_guid, 
		  relation_type = s_relation_type, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_del_relation",t_ret_msg, 1379, 0);
end

function ws2dp_send_class:send_check_id_mail_proc_lua(s_role_guid,s_server_id,s_cur_time)
	local  t_ret_msg = {  
          role_guid = s_role_guid, 
		  server_id = s_server_id, 
		  cur_time = s_cur_time, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_check_id_mail",t_ret_msg, 32804, s_server_id);
end

function ws2dp_send_class:send_save_other_server_mail_record_lua(s_source_server_id,s_target_server_id,s_mail_guid,s_role_guid,s_server_id)
	local  t_ret_msg = {  
          source_server_id = s_source_server_id, 
		  target_server_id = s_target_server_id, 
		  mail_guid = s_mail_guid, 
		  role_guid = s_role_guid,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_other_server_mail_record",t_ret_msg, 32819, s_server_id);
end

function ws2dp_send_class:send_load_other_server_mail_lua(s_server_id,s_mail_guid,server_id)
	local  t_ret_msg = {  
          server_id = s_server_id, 
		  mail_guid = s_mail_guid, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_other_server_mail",t_ret_msg, 32814, server_id);
end

function ws2dp_send_class:send_del_other_server_mail_lua(s_source_server_id,s_target_server_id,s_mail_guid,s_role_guid,s_server_id)
	local  t_ret_msg = {  
          source_server_id = s_source_server_id, 
		  target_server_id = s_target_server_id, 
		  mail_guid = s_mail_guid, 
		  role_guid = s_role_guid,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_del_other_server_mail",t_ret_msg, 32818, s_server_id);
end

function ws2dp_send_class:send_load_all_ranking_info_lua(s_ranking_type)
	local  t_ret_msg = {  
          ranking_type = s_ranking_type, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_all_ranking_info",t_ret_msg, 33100, 0);
end

function ws2dp_send_class:send_load_all_service_rank_info_lua(s_ranking_type)
	local  t_ret_msg = {  
          rank_type = s_ranking_type, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_all_service_rank_info",t_ret_msg, 33115, 0);
end

function ws2dp_send_class:send_load_player_worship_value_lua(s_sender,s_target)
	local  t_ret_msg = {  
          sender = s_sender, 
		  target = s_target, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_get_worship_value",t_ret_msg, 33110, 0);
end

function ws2dp_send_class:send_delete_legion_info_lua(s_legion_guid)
	local  t_ret_msg = {  
          legion_guid = s_legion_guid, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_delete_legion_info",t_ret_msg, 33503, 0);
end

function ws2dp_send_class:send_delete_legion_member_info_lua(s_role_guid)
	local  t_ret_msg = {  
          role_guid = s_role_guid, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_delete_legion_member_info",t_ret_msg, 33504, 0);
end

function ws2dp_send_class:send_delete_legion_city_war_member_info_lua(s_del_type)
	local  t_ret_msg = {  
          del_type = s_del_type, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_delete_legion_city_war_member_info",t_ret_msg, 33505, 0);
end

function ws2dp_send_class:send_delete_legion_applicant_lua(s_role_guid)
	local  t_ret_msg = {  
          role_guid = s_role_guid, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_del_legion_applicant_one",t_ret_msg, 33515, 0);
end

function ws2dp_send_class:send_save_legion_boss_award_get_log_lua(s_role_guid,s_boss_map_id)
	local  t_ret_msg = {  
          role_guid = s_role_guid, 
		  boss_map_id = s_boss_map_id,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_legion_boss_award_get_log",t_ret_msg, 33522, 0);
end

function ws2dp_send_class:send_load_all_legion_boss_award_get_log_lua(s_role_guid)
	local  t_ret_msg = {  
          role_guid = s_role_guid, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_all_legion_boss_award_get_log",t_ret_msg, 33520, 0);
end

function ws2dp_send_class:send_save_city_info_lua(s_occupation_legion_guid,s_city_master_name)
	local  t_ret_msg = {  
          occupation_legion_guid = s_occupation_legion_guid, 
		  city_master_name = s_city_master_name, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_city_info",t_ret_msg, 33525, 0);
end

function ws2dp_send_class:send_clear_war_bid_info_lua(s_clear_type)
	local  t_ret_msg = {  
          clear_type = s_clear_type, 
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_clear_war_bid_info",t_ret_msg, 33529, 0);
end

function ws2dp_send_class:send_clear_city_war_reward_stuff_lua(s_del_stuff_type,s_target_guid)
	local  t_ret_msg = {  
          del_stuff_type = s_del_stuff_type, 
		  target_guid = s_target_guid,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_clear_overlord_legion_stuff",t_ret_msg, 33530, 0);
end

ws2dp_send = ws2dp_send_class
return ws2dp_send





















