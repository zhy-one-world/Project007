local dp_msgproc_lua_class={}

function dp_msgproc_lua_class:init()
	--dump("----0------------")
end

--功能消息入口
define_C[1323] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_open_time", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_open_time----")
	hld.gm_set_open_time_lua(conn_index, msg_data.open_time,msg_data.listen_port,msg_data.handle_index);
end

define_C[1331] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_set_server_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_set_server_info----")
	hld.gm_set_cross_open_time_lua(conn_index, msg_data.server_info_value,msg_data.server_info_type,msg_data.listen_port,msg_data.handle_index);
end

define_C[1324] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_change_password", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_change_password----")
	hld.gm_set_new_password_lua(conn_index, msg_data.account,msg_data.new_password,msg_data.listen_port,msg_data.handle_index);
end

define_C[1325] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_need_begin_cross", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_need_begin_cross----")
	hld.gm_set_begin_cross_lua(conn_index, msg_data.need_begin_cross,msg_data.gm_type,msg_data.req_http_id);
end

define_C[1333] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_last_server_list", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_last_server_list----")
	hld.save_server_last_server_list_lua(conn_index, msg_data.server_id,msg_data.last_server_list);
end

define_C[1334] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_last_server_list", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_last_server_list----")
	hld.load_server_last_server_list_lua(conn_index, msg_data.server_id);
end

define_C[1312] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_set_role_right", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_set_role_right----")
	hld.gm_set_role_right_lua(conn_index, msg_data.role_mark, msg_data.right_type, msg_data.right_value, msg_data.right_time, msg_data.listen_port, msg_data.handle_index);
end

define_C[1326] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_change_account", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_change_account----")
	hld.gm_change_account_lua(conn_index, msg_data.change_name, msg_data.target_name, msg_data.listen_port, msg_data.handle_index);
end

define_C[1328] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_del_cache", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_del_cache----")
	hld.gm_del_role_cache_lua(conn_index, msg_data.role_guid);
end

define_C[1329] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_del_role", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_del_role----")
	hld.gm_del_role_lua(conn_index,msg_data.role_guid,msg_data.is_del,msg_data.listen_port,msg_data.handle_index);
end

define_C[1330] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_stop_login", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_stop_login----")
	hld.gm_del_role_lua(conn_index,msg_data.is_stop,msg_data.listen_port,msg_data.handle_index);
end

define_C[1336] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_character", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_character----",msg_data.role_guid)
	hld.get_char_info_ws_lua(conn_index,msg_data.role_guid,msg_data.role_account);
end

define_C[1313] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_get_role_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_get_role_info----")
	hld.gm_get_role_info_lua(conn_index,msg_data.role_mark,msg_data.get_type,msg_data.listen_port,msg_data.handle_index,msg_data.check_type);
end

define_C[1316] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_modify_role_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_modify_role_info----")
	hld.gm_modify_role_info_lua(conn_index,msg_data.role_guid,msg_data.sql_str,msg_data.listen_port,msg_data.handle_index);
end

define_C[1322] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_gm_get_role_items_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_gm_get_role_items_info----")
	hld.gm_get_role_items_info_lua(conn_index,msg_data.role_guid,msg_data.listen_port,msg_data.handle_index);
end

define_C[1371] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_relation_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_relation_info----")
	hld.ws2dp_req_load_offline_other_relation_lua(conn_index,msg_data.sender_guid,msg_data.list_type,msg_data.other_guid);
end

define_C[1373] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_req_load_relation_list", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_req_load_relation_list----")
	hld.ws2dp_req_load_player_relation_lua(conn_index,msg_data.sender_guid);
end

define_C[1377] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_add_relation", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_add_relation----")
	
	hld.ws2dp_req_add_relation_lua(conn_index,msg_data.role_guid,msg_data.relation_guid,msg_data.relation_name,msg_data.relation_gs_value,msg_data.data_array);
end

define_C[1379] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_del_relation", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_del_relation----")
	
	hld.ws2dp_req_del_relation_lua(conn_index,msg_data.role_guid,msg_data.target_role_guid,msg_data.relation_type);
end

define_C[32804] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_check_id_mail", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_check_id_mail----")
	
	hld.check_id_mail_proc_lua(conn_index,msg_data.role_guid,msg_data.server_id,msg_data.cur_time);
end

define_C[32904] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_arena_rank_change", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_arena_rank_change----")
	local new_msg_data = msg_data["my_new_data"]
	local target_msg_data = msg_data["target_new_data"]
	
	hld.save_arena_rank_first_lua(conn_index,new_msg_data.rank_pos,new_msg_data.role_guid,new_msg_data.role_name,new_msg_data.gs_value
	,new_msg_data.role_template_id,new_msg_data.robot_template_id,new_msg_data.military_rank_level,new_msg_data.vip_level,new_msg_data.game_channel
	,target_msg_data.rank_pos,target_msg_data.role_guid,target_msg_data.role_name,target_msg_data.gs_value
	,target_msg_data.role_template_id,target_msg_data.robot_template_id,target_msg_data.military_rank_level,target_msg_data.vip_level,target_msg_data.game_channel);
end

define_C[32819] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_other_server_mail_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_other_server_mail_record----")

	hld.save_other_server_mail_record_lua(conn_index,msg_data.source_server_id,msg_data.target_server_id,msg_data.mail_guid,msg_data.role_guid);
end

define_C[32814] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_other_server_mail", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_other_server_mail----")

	hld.load_other_server_mail_lua(conn_index,msg_data.server_id,msg_data.mail_guid);
end

define_C[32818] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_del_other_server_mail", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_del_other_server_mail----")

	hld.del_other_server_mail_lua(conn_index,msg_data.source_server_id,msg_data.target_server_id,msg_data.mail_guid,msg_data.role_guid);
end

define_C[33100] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_all_ranking_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_all_ranking_info----")

	hld.load_all_ranking_info_lua(conn_index,msg_data.ranking_type);
end

define_C[33108] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_ranking_info_one", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_ranking_info_one----")
	local player_info = msg_data["ranking_info"]

	hld.save_ranking_info_one_lua(conn_index,player_info.role_guid,player_info.role_name,player_info.ranking_type,player_info.ranking_value
	,player_info.save_time,player_info.supporting_guid,player_info.supporting_name,player_info.vip_level,player_info.peak,player_info.server_id
	,player_info.game_channel,player_info.standby_parameter);
end

define_C[33115] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_all_service_rank_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_all_service_rank_info----")

	hld.save_one_ranking_list_lua(conn_index,msg_data.rank_type);
end

define_C[33102] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_ranking_worship", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_ranking_worship----")
	local player_info = msg_data["target_data"]

	hld.save_ranking_worship_lua(conn_index,msg_data.sender_guid,player_info.role_guid,player_info.role_name,player_info.ranking_type,player_info.ranking_value
	,player_info.save_time,player_info.supporting_guid,player_info.supporting_name,player_info.vip_level,player_info.peak,player_info.server_id
	,player_info.game_channel,player_info.standby_parameter,msg_data.worship_add_value,msg_data.worship_type);
end

define_C[33110] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_get_worship_value", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_get_worship_value----")

	hld.load_player_worship_value_lua(conn_index,msg_data.sender,msg_data.target);
end

define_C[33502] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_info----")
	local legion_info = msg_data["legion_info"]
	

	hld.save_legion_info_lua(conn_index,legion_info.legion_guid,legion_info.legion_name,legion_info.announcement,legion_info.gs_value,legion_info.data_ary);
end

define_C[33510] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_member_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_member_info----")
	local member_info = msg_data["legion_member_info"]
	

	hld.save_legion_member_info_lua(conn_index,msg_data.legion_guid,member_info.role_guid,member_info.role_name,member_info.gs_value,member_info.data_ary);
end

define_C[33511] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_city_war_member_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_city_war_member_info----")
	local member_info = msg_data["legion_member_info"]
	

	hld.save_legion_city_war_member_info_lua(conn_index,msg_data.legion_guid,member_info.role_guid,member_info.use_type,member_info.role_name,member_info.player_temp_id,member_info.week_contribution,member_info.job_title);
end

define_C[33503] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_delete_legion_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_delete_legion_info----")

	hld.delete_legion_info_lua(conn_index,msg_data.legion_guid);
end

define_C[33504] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_delete_legion_member_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_delete_legion_member_info----")

	hld.delete_legion_member_info_lua(conn_index,msg_data.role_guid);
end

define_C[33505] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_delete_legion_city_war_member_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_delete_legion_city_war_member_info----")

	hld.delete_legion_city_war_member_info_lua(conn_index,msg_data.del_type);
end

define_C[33515] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_del_legion_applicant_one", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_del_legion_applicant_one----")

	hld.delete_legion_applicant_lua(conn_index,msg_data.role_guid);
end

define_C[33514] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_applicant", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_applicant----")
	hld.save_legion_applicant_lua(conn_index,msg_data.legion_guid,msg_data.role_guid,msg_data.role_name,msg_data.exist_time,msg_data.gs_value,msg_data.data_ary);
end

define_C[33522] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_boss_award_get_log", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_boss_award_get_log----")

	hld.save_legion_boss_award_get_log_lua(conn_index,msg_data.role_guid,msg_data.boss_map_id);
end

define_C[33520] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_all_legion_boss_award_get_log", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_all_legion_boss_award_get_log----")

	hld.load_all_legion_boss_award_get_log_lua(conn_index,msg_data.role_guid);
end

define_C[33525] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_city_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_city_info----")

	hld.save_city_info_lua(conn_index,msg_data.occupation_legion_guid,msg_data.city_master_name);
end

define_C[33528] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_war_bid_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_war_bid_info----")
	local bid_info = msg_data["bid_info"];

	hld.save_war_bid_info_lua(conn_index,bid_info.territory_id,bid_info.index_in_terr,bid_info.legion_guid,bid_info.cur_bid_price
	,bid_info.is_prior_maintain,bid_info.occupy_time_stamp,bid_info.server_id,bid_info.legion_name);
end

define_C[33529] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_clear_war_bid_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_clear_war_bid_info----")

	hld.clear_war_bid_info_lua(conn_index,msg_data.clear_type);
end

define_C[33530] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_clear_overlord_legion_stuff", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_clear_overlord_legion_stuff----")

	hld.clear_city_war_reward_stuff_lua(conn_index,msg_data.del_stuff_type,msg_data.target_guid);
end

define_C[33533] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_event", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_event----")
	local event_info = msg_data["event_data"];

	hld.save_legion_event_lua(conn_index,event_info.legion_guid,event_info.event_idx,event_info.extra_data,event_info.happen_time);
end

define_C[33535] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_clear_legion_event", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_clear_legion_event----")

	hld.clear_legion_event_lua(conn_index,msg_data.legion_guid);
end

define_C[33536] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_legion_bonus_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_legion_bonus_info----")

	hld.save_legion_bonus_info_lua(conn_index,msg_data.legion_guid,msg_data.mission_id,msg_data.finish_count);
end

define_C[33539] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_clear_legion_bonus_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_clear_legion_bonus_info----")

	hld.clear_legion_bonus_info_lua(conn_index,msg_data.legion_guid);
end

define_C[1695] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_server_attr_val", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_server_attr_val----")

	hld.save_server_attr_val_lua(conn_index,msg_data.attr_type,msg_data.attr_val);
end

define_C[1693] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_server_attr_val", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_server_attr_val----")

	hld.load_server_attr_val_lua(conn_index);
end

define_C[33548] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_cross_city_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_cross_city_info----")

	hld.load_cross_city_info_lua(conn_index);
end

define_C[33552] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_cross_city_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_cross_city_info----")
	local event_info = msg_data["city_info_msg"];
	hld.save_cross_city_info_lua(conn_index, msg_data["rank_idex"]. event_info["legion_guid"], event_info["server_id"], event_info["chief_guid"], event_info["legion_name"], event_info["chief_name"]);
end

define_C[33555] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_clear_cross_city_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_clear_cross_city_info----")

	hld.clear_cross_city_info_lua(conn_index);
end

define_C[33902] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_best_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_best_record----")
	local event_info = msg_data["record_info"];
	hld.save_best_record_lua(conn_index, event_info["map_template_id"], event_info["role_guid"], event_info["role_name"], event_info["role_value"]);
end

define_C[33900] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_all_best_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_all_best_record----")

	hld.load_all_best_record_lua(conn_index);
end

define_C[35102] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_all_gain_treasure_record_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_all_gain_treasure_record_info----")

	hld.load_all_gain_treasure_record_info_lua(conn_index, msg_data["boss_spawn_point_id"]);
end

define_C[35104] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_gain_treasure_record_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_gain_treasure_record_info----")

	hld.save_gain_treasure_record_info_lua(conn_index, msg_data["del_stamp"], msg_data["boss_spawn_point_template_id"], msg_data["killed_stamp"], msg_data["role_name"], msg_data["gain_item_template_id"], msg_data["role_guid"]);
end

define_C[35800] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_cross_server_pk_msg_all", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_cross_server_pk_msg_all----")

	hld.save_cross_server_pk_ws_info_lua(conn_index, msg_data["pk_state"],msg_data["war_idex"],msg_data["player_guid"],msg_data["role_name"],msg_data["is_winner"],msg_data["server_id"],msg_data["win_num"]);
end

define_C[35801] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_cross_server_pk_msg_all", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_cross_server_pk_msg_all----")

	hld.load_cross_server_pk_ws_info_lua(conn_index)
end

define_C[35802] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_clear_cross_server_pk_msg_all", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_clear_cross_server_pk_msg_all----")

	hld.clear_cross_server_pk_ws_info_lua(conn_index)
end

define_C[36000] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_cross_server_harry_msg_all", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_cross_server_harry_msg_all----")

	hld.save_cross_server_harry_ws_info_lua(conn_index, msg_data["server_id"], msg_data["server_money"], msg_data["special_money"], msg_data["first_player_guid"], msg_data["second_player_guid"], msg_data["third_player_guid"], msg_data["first_player_name"])
end

define_C[36001] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_cross_server_harry_msg_all", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_cross_server_harry_msg_all----")

	hld.load_cross_server_harry_ws_info_lua(conn_index)
end

define_C[36025] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_del_one_server_msg", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_del_one_server_msg----")

	hld.del_one_server_msg_func_lua(conn_index, msg_data["server_id"])
end

define_C[36012] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_harry_player_detail", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_harry_player_detail----")

	hld.load_harry_player_detail_lua(conn_index, msg_data["player_idex"], msg_data["role_guid"], msg_data["server_id"])
end

define_C[33700] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_role_pk_top", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_role_pk_top----")

	hld.load_role_pk_top_lua(conn_index)
end

define_C[33702] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_role_pk_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_role_pk_info----")

	hld.load_role_pk_info_lua(conn_index, msg_data.role_guid, msg_data.template_id, msg_data.role_gs, msg_data.role_name);
end

define_C[33705] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_role_pk_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_role_pk_info----")

	hld.save_role_pk_info_lua(conn_index, msg_data.role_guid, msg_data.target_name_1, msg_data.target_name_2, msg_data.target_name_3, msg_data.ladder_score, msg_data.win_result, msg_data.cross_honor);
end

define_C[33706] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.save_role_pk_honor_refresh", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class save_role_pk_honor_refresh----")

	hld.save_role_pk_honor_refresh_lua(conn_index, msg_data["role_guid"], msg_data["refresh_time"])
end

define_C[33707] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_role_pk_log", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_role_pk_log----")

	hld.load_role_pk_log_lua(conn_index, msg_data["role_guid"], msg_data["server_id"])
end

define_C[33002] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_mail_event_to_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_mail_event_to_db----")

	hld.save_globel_mail_to_sql_lua(conn_index, message_data, string.len(message_data))
end

define_C[33015] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_red_package", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_red_package----")

	hld.save_red_package_to_sql_lua(conn_index, message_data, string.len(message_data))
end

define_C[33016] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_red_package", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_red_package----")

	hld.load_red_package_by_sql_lua(conn_index)
end

define_C[33017] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_red_package_receiver", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_red_package_receiver----")

	hld.load_red_package_receiver_by_sql_lua(conn_index, msg_data["red_package_guid"])
end

define_C[33018] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_add_red_package_to_sql", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_add_red_package_to_sql----")
	local event_info = msg_data["red_info"];
	hld.add_red_package_to_sql_proc_lua(conn_index, event_info["red_package_guid"], event_info["red_package_template_id"], event_info["sender_guid"], event_info["create_time"], 
	event_info["sender_name"], event_info["text"], event_info["sender_vip_level"], event_info["remain_times"], event_info["remain_money_num"])
end

define_C[33019] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_add_red_package_receiver_to_sql", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_add_red_package_receiver_to_sql----")
	local event_info = msg_data["red_info"];
	hld.add_red_package_receiver_to_sql_proc_lua(conn_index, event_info["red_package_guid"], event_info["receiver_guid"], event_info["role_name"], event_info["money_num"])
end

define_C[33020] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_del_red_package_by_sql", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_del_red_package_by_sql----")

	hld.del_red_package_by_sql_proc_lua(conn_index, msg_data["red_package_guid"])
end

define_C[33003] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_mail_event_item_to_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_mail_event_item_to_db----")

	hld.save_globel_mail_item_to_lua(conn_index, message_data, string.len(message_data))
end

define_C[33004] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_mail_event", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_mail_event----")

	hld.load_mail_event_lua(conn_index, msg_data["is_over"])
end

define_C[33005] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_mail_event_item", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_mail_event_item----")

	hld.load_mail_event_item_lua(conn_index, msg_data["is_over"])
end

define_C[33009] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_new_globel_mail_had_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_new_globel_mail_had_info----")

	hld.save_globel_mail_new_get_info_proc_lua(conn_index, msg_data["role_guid"], msg_data["data_info"])
end

define_C[33800] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_world_boss_to_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_world_boss_to_db----")

	hld.save_world_boss_to_sql_lua(conn_index, msg_data["npc_index"], msg_data["template_id"], msg_data["is_alive"], msg_data["line_id"], msg_data["last_respawn_time"], 
	msg_data["respawn_phase"], msg_data["killer_name"], msg_data["first_killer_name"], msg_data["first_killed_time"], msg_data["is_get_prize"])
end

define_C[33801] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_world_boss", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_world_boss----")

	hld.load_world_boss_lua(conn_index)
end

define_C[34501] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_del_draw_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_del_draw_record----")

	hld.del_draw_info_from_db_lua(conn_index, msg_data["info_guid"])
end

define_C[34500] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_add_new_draw_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_add_new_draw_record----")

	hld.add_draw_info_to_db_lua(conn_index, msg_data["info_guid"], msg_data["role_guid"], msg_data["role_name"], 
	msg_data["item_template_id"], msg_data["get_type"], msg_data["record_type"], msg_data["record_time"])
end

define_C[34502] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_draw_record_by_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_draw_record_by_db----")

	hld.load_draw_info_by_db_lua(conn_index)
end

define_C[34000] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_save_big_player_to_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_save_big_player_to_db----")

	hld.save_big_player_to_sql_lua(conn_index, msg_data["player_type"], msg_data["role_guid"])
end

define_C[34001] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_big_player_by_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_big_player_by_db----")

	hld.load_big_player_lua(conn_index)
end
define_C[34002] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_load_big_player_detail", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_load_big_player_detail----")

	hld.load_big_player_detail_lua(conn_index, msg_data["big_type"], msg_data["role_guid"])
end

define_C[34701] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_add_item_into_db", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_add_item_into_db----")
	hld.auction_add_item_into_db_proc_lua(conn_index, message_data, string.len(message_data))
end

define_C[34705] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_find_buy_target", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_find_buy_target----")

	hld.auction_find_buy_target_proc_lua(conn_index, msg_data["role_guid"], msg_data["item_guid"])
end

define_C[34714] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_cancel_sell", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class auction_cancel_sell----")

	hld.auction_find_cancel_target_proc_lua(conn_index, msg_data["role_guid"], msg_data["item_guid"])
end

define_C[34725] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_find_bid_target", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_find_bid_target----")

	hld.auction_find_bid_target_proc_lua(conn_index, msg_data["role_guid"], msg_data["item_guid"], msg_data["price_money_type"], msg_data["price_money_value"])
end

define_C[34730] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_bid_success", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_bid_success----")
	local t_db_row = msg_data.auction_info
	local s_buffer = encode("hld.server2dp_proto.s_auction_info", t_db_row)
	hld.auction_bid_success_proc_lua(conn_index, msg_data["new_record_id"], msg_data["old_buyer_guid"], s_buffer, string.len(s_buffer))
end

define_C[34732] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.auction_clear_old_item", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class auction_clear_old_item----")

	hld.auction_get_and_clear_old_item_proc_lua(conn_index, msg_data["trade_type"], msg_data["cur_time"])
end

define_C[34734] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_add_share_list", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_add_share_list----")

	hld.auction_add_share_list_proc_lua(conn_index, msg_data["item_guid"], msg_data["role_guid"], msg_data["share_rate"], msg_data["share_mail_title"], msg_data["share_mail_content"])
end

define_C[34710] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_add_purchase_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_add_purchase_record----")
	local t_db_row = msg_data.auction_info
	local s_buffer = encode("hld.server2dp_proto.s_auction_info", t_db_row)
	hld.auction_add_trade_record_proc_lua(conn_index, msg_data["record_guid"], msg_data["role_guid"], msg_data["is_sell"], s_buffer, string.len(s_buffer))
end

define_C[34735] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_req_share_list", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_req_share_list----")

	hld.auction_req_share_list_proc_lua(conn_index, msg_data["item_guid"], msg_data["item_template_id"], msg_data["buyer_guid"], msg_data["money_type"], msg_data["money_value"])
end

define_C[34738] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_check_legion_selling", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_check_legion_selling----")

	hld.auction_check_legion_selling_proc_lua(conn_index, msg_data["legion_guid"], msg_data["cur_time"])
end

define_C[34741] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_del_bid_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_del_bid_record----")

	hld.auction_del_bid_record_proc_lua(conn_index, msg_data["item_guid"])
end

define_C[34742] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_clear_old_bid_record", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_clear_old_bid_record----")
	hld.auction_clear_old_bid_record_proc_lua(conn_index)
end

define_C[34745] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_show_sell", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_show_sell----")
	local t_db_row = msg_data.data_info
	local s_buffer = encode("hld.server2dp_proto.s_auction_info", t_db_row)
	hld.auction_update_sell_show_proc_lua(conn_index, msg_data["end_time"], s_buffer, string.len(s_buffer))
end

define_C[34747] = function(message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.ws2dp_auction_time_out", message_data)
	if msg_data == nil then
     	return
    end
	print("-------dp_msgproc_lua_class ws2dp_auction_time_out----")

	hld.auction_time_out_proc_lua(conn_index, msg_data["cur_mil_sec"], msg_data["sell_time"], msg_data["deal_time"], msg_data["is_first"])
end

dp_msgproc_lua = dp_msgproc_lua_class
return dp_msgproc_lua
















