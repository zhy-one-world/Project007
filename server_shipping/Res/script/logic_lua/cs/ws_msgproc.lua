local ws_msgproc = {}

local define_WS = define_C


local  harry_map_id = 71211107;                --跨服掠夺地图ID


function ws_msgproc:init(  )
	delog("---####---ws_msgproc--init----")
end


define_WS[1004] = function (message_data, conn_index)
	
	 hld.get_cell_server():set_reload_csv();
end

define_WS[1328] = function ( message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.gm_del_cache", message_data)
    if msg_data == nil then
    	return;
    end
    hld.ws2cs_req_del_unit_lua(msg_data.role_guid)
end

define_WS[1687] = function ( message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.del_unit_item", message_data)
    if msg_data == nil then
    	return;
    end
    local player_ref = hld.get_player_by_index(msg_data.cs_unit_index);
	if player_ref:is_valid() == true then
		player_ref:get_item_set():del_item(hld.e_server_log_del_item_gm_order, player_ref:get_unit_info(hld.e_role_info_move_map_id), msg_data.bag_type, msg_data.bag_slot_begin, msg_data.bag_slot_end);
    end
end

define_WS[1688] = function(message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.change_unit_vip", message_data)
    if msg_data == nil then
    	return;
    end
    local player_ref = hld.get_player_by_index(msg_data.cs_unit_index);
    if player_ref:is_valid() == false then
    	return
    end
		
	local new_value = player_ref:get_unit_info(hld.e_role_info_recharge_num) + msg_data.vip_value;
	if new_value < 0 then	
	   new_value = 0;
	end	
	player_ref:gm_set_vip_point(new_value);
end


define_WS[1689] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.del_unit_specified_item", message_data)
    if msg_data == nil then
    	return;
    end
    local player_ref = hld.get_player_by_index(msg_data.cs_unit_index);
    if player_ref:is_valid() == false then
    	return
    end
    player_ref:get_item_set():del_item(hld.e_server_log_del_item_gm_order, player_ref:get_unit_info(e_role_info_move_map_id), msg_data.bag_type, msg_data.item_id);
end

define_WS[1689] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.add_guide_record", message_data)
    if msg_data == nil then
    	return;
    end
    local player_ref = hld.get_player_by_index(msg_data.cs_unit_index);
    if player_ref:is_valid() == false then
    	return
    end
    player_ref:get_guide_mgr():save_guide_record_lua(msg_data.guide_trigger_id)
 end


 define_WS[1503] = function (message_data, conn_index)
 	local msg_data = decode("hld.ws2cs_proto.time_out", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data);
    local player_ref = hld.get_player_by_index(msg_data.cs_unit_index);
    if player_ref:is_valid() == false then
    	return
    end
    player_ref:on_player_disconnect();
 end

 define_WS[1504] = function (message_data, conn_index)
 	local msg_data = decode("hld.ws2cs_proto.reconnect_game", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data);
    hld.ws2cs_req_reconnect_game_lua(msg_data.client_uid,msg_data.role_guid,msg_data.cur_map_id, msg_data.cur_map_line_id)
 end


 define_WS[1507] = function (message_data, conn_index)
 	local msg_data = decode("hld.ws2cs_proto.enter_game", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_req_enter_game(conn_index,message_data, string.len(message_data))
 end

 ---------------------------------------------------------------------------------------------------
define_WS[1214] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.client_logout", message_data)
    if msg_data == nil then
    	return;
    end
    --delog(msg_data)
    hld.ws2cs_req_logout_client_lua(msg_data.role_guid, msg_data.need_send_save_end);
end

define_WS[1517] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.enter_scene", message_data)
    if msg_data == nil then
    	return;
    end
    --delog(msg_data)
    hld.ws2cs_req_enter_scene_lua(msg_data.role_guid);
end

define_WS[1520] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.server_config", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)
    hld.lua_ws2cs_req_server_config(conn_index,message_data, string.len(message_data));
end

define_WS[1521] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.clear_server_config", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)

    hld.get_cell_server():clear_all_server_config();
end

-------------------------------------------------------------------------------------------------

define_WS[1586] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.get_map_info_from_cs", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)
    hld.lua_ws2cs_get_map_info_from_cs_func(conn_index, message_data, string.len(message_data))
end


define_WS[1323] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.cs_gm_open_time", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)
    hld.get_cell_server():set_open_time(msg_data.open_time);
end

define_WS[1510] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.server_need_cross", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)
    hld.get_cell_server():set_need_begin_cross_gm_common(msg_data.gm_type, msg_data.need_begin_cross);
end

define_WS[1511] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.server_info_arr", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)
    hld.get_cell_server():set_server_info_arr(msg_data.server_info_type, msg_data.server_info_value);
end

define_WS[1512] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.server_info_all", message_data)
    if msg_data == nil then
    	return;
    end
    delog(msg_data)
    for i=1,8 do
    	local idx = i-1
    	hld.get_cell_server():set_server_info_arr(idx, msg_data.server_info_arr[i]);
    end
    --hld.get_cell_server():set_server_info_arr(msg_data.gm_type, msg_data.need_begin_cross);
end

-------------------------------------------------------------------------------------------
define_WS[1551] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.broken_sky_creat_boss", message_data)
    if msg_data == nil then
    	return;
    end

    hld.ws2cs_broken_sky_sync_creat_boss_func_lua(msg_data.boss_level, msg_data.all_damage, msg_data.from_map_guid)
end

define_WS[1549] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.broken_sky_info_sync", message_data)
    if msg_data == nil then
    	return;
    end

    hld.ws2cs_broken_sky_sync_game_info_func_lua(msg_data.map_template_id, msg_data.broken_sky_inst_guid, msg_data.damage)
end


define_WS[1553] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.broken_sky_kill_boss", message_data)
    if msg_data == nil then
    	return;
    end
    hld.ws2cs_broken_sky_sync_kill_boss_func_lua(msg_data.map_template_id, msg_data.killer_guid, msg_data.killer_unit_index)
end


define_WS[1554] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.chest_arrivai_spawn_chest_wave", message_data)
    if msg_data == nil then
    	return;
    end
    hld.ws2cs_chest_arrival_spawn_chests_func_lua(msg_data.chest_disappear_time)
end


define_WS[1556] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.clear_big_player_title", message_data)
    if msg_data == nil then
    	return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end

    player_ptr:change_big_player_title(msg_data.title_type);
end


--------------------------------------------------------------------------------------------------------
define_WS[1563] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.apply_to_join_team", message_data)
    if msg_data == nil then
    	return;
    end
    hld.team_cs_mgr:lua_ws2cs_apply_to_join_team_func(conn_index, message_data, string.len(message_data))
end


define_WS[1557] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.set_player_team_info", message_data)
    if msg_data == nil then
    	return;
    end
    hld.team_cs_mgr:lua_ws2cs_set_player_team_info_func(conn_index, message_data, string.len(message_data))
end

define_WS[1558] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.clear_player_team_info", message_data)
    if msg_data == nil then
    	return;
    end
    hld.team_cs_mgr:lua_ws2cs_clear_player_team_info_func(conn_index, message_data, string.len(message_data))
end

define_WS[1567] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.transfer_team_member_to_scene", message_data)
    if msg_data == nil then
    	return;
    end
    hld.team_cs_mgr:lua_ws2cs_transfer_team_member_to_scene_func(conn_index, message_data, string.len(message_data))
end

define_WS[1570] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.team_common_check_in_cs", message_data)
    if msg_data == nil then
    	return;
    end
    hld.team_cs_mgr:lua_ws2cs_team_common_check_in_cs_func(conn_index, message_data, string.len(message_data))
end


define_WS[1572] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.sync_team_start_aim", message_data)
    if msg_data == nil then
    	return;
    end
    hld.team_cs_mgr:lua_ws2cs_sync_team_start_aim_func(conn_index, message_data, string.len(message_data))
end


------------------------------------------------------------------------------------------------------------

define_WS[1575] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.set_player_legion_info", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_set_player_legion_guid_func(conn_index, message_data, string.len(message_data))
end

define_WS[1576] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.clear_player_legion_info", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end	
	player_ptr:get_legion_cs_mgr():ws2cs_clear_legion_info();
end

-------------------------------------------------------------------------------------------------------------

define_WS[1577] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.create_legion", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_create_legion_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1579] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.donate_legion_money", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_legion_donate_money_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1581] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.donate_legion_item", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_legion_donate_item_fun(conn_index, message_data, string.len(message_data))
end


define_WS[1583] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.enter_legion_boss_map", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_enter_legion_boss_map_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1585] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.get_legion_boss_award", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_get_legion_boss_award_fun(conn_index, message_data, string.len(message_data))
end


------------------------------------------------------

define_WS[1595] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.set_get_legion_boss_award_log", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_set_get_legion_boss_award_log_fun(conn_index, message_data, string.len(message_data))
end



define_WS[1603] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.bonfire_add_fuel", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_add_fuel_to_bonfire_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1607] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.legion_remove_recruit_cd", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_legion_rm_recruit_cd_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1609] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.draw_bonfire_daily_rwd", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_draw_bonfire_daily_rwd_fun(conn_index, message_data, string.len(message_data))
end

----------------------------------------------------------------------------------------
define_WS[1614] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.level_up_legion_skill", message_data)
    if msg_data == nil then
    	return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end	
    player_ptr:get_legion_skill():lua_level_up_legion_skill_func(message_data, string.len(message_data))
end

define_WS[1627] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.confirm_change_legion_name", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_confirm_change_legion_name_func(conn_index, message_data, string.len(message_data))
end

define_WS[1629] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.rem_item_change_legion_name_care", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_rem_item_change_legion_name_care_func(conn_index, message_data, string.len(message_data))
end

define_WS[1630] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.change_gate_legion_name", message_data)
    if msg_data == nil then
    	return;
    end

    hld.legion_cs_mgr:lua_ws2cs_change_gate_legion_name_func(conn_index, message_data, string.len(message_data))
end

-----------------------------------------------------------------------------------------------------------------


define_WS[1590] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.begin_city_war", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_begin_city_war_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1591] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.enter_city_war_map", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_enter_city_war_map_fun(conn_index, message_data, string.len(message_data))
end


define_WS[1592] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.city_war_operate", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_city_war_operate_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1597] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.get_occupation_daily_award", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_get_occupation_daily_award_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1599] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.get_city_master_stuff", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_get_city_war_master_stuff_fun(conn_index, message_data, string.len(message_data))
end

---------------------------------------------------------------------------------
define_WS[1600] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.del_city_master_stuff", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_del_city_war_master_stuff_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1610] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.add_city_war_winner_stuff", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_add_city_war_winner_stuff_fun(conn_index, message_data, string.len(message_data))
end


define_WS[1611] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.del_city_war_winner_stuff", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_del_city_war_winner_stuff_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1618] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.get_bonus_award", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_get_bonus_award_fun(conn_index, message_data, string.len(message_data))
end

define_WS[1621] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.legion_answer_question_right", message_data)
    if msg_data == nil then
    	return;
    end
    hld.legion_cs_mgr:lua_ws2cs_answer_question_right_func(conn_index, message_data, string.len(message_data))
end

----------------------------------------------------------

define_WS[1626] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.set_legion_average_lv_to_map", message_data)
    if msg_data == nil then
    	return;
    end
    if msg_data.legion_average_lv <= 0 then
		return;
	end
	local guid_ptr = hld.get_guid_64(msg_data.map_guid)
	local map_ptr =  hld.get_map(guid_ptr)
	if  map_ptr == nil then
		return;
	end
	map_ptr:set_legion_average_lv(msg_data.legion_average_lv)
end


-----------------------------------------------------------------------------------------------------


define_WS[1635] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.rep_get_arena_rank", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end
    player_ptr:get_arena_mgr():get_arena_rank_end(msg_data.arena_rank, msg_data.is_request);
end 

define_WS[1637] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.req_challenge_msg", message_data)
    if msg_data == nil then
    	return;
    end
    delog("----start----")
    delog(msg_data)
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local aim_guid_ptr = hld.get_guid_64(msg_data.target_guid)

    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end
    delog(msg_data)
    player_ptr:get_arena_mgr():do_challenge(aim_guid_ptr
    	                          ,msg_data.robot_id,msg_data.req_result,msg_data.cost_money_type, msg_data.cost_money_value
				                  ,msg_data.target_cur_pos, msg_data.self_cur_pos)
end 


define_WS[1639] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.req_choose_list", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)


    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end
    player_ptr:get_arena_mgr():lua_req_match_player_end(message_data, string.len(message_data))
end 
--------------------------------------------------------------------------------------------------------------
define_WS[1646] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.req_worship_player_end", message_data)
    if msg_data == nil then
    	return;
    end
    local target_guid_ptr = hld.get_guid_64(msg_data.target_guid)
 
    local target_player_ptr = hld.get_player_by_guid(target_guid_ptr)
	if  target_player_ptr ~= nil then
		target_player_ptr:get_ranking_mgr():set_worship_value(msg_data.target_worship_value);
	end

	local guid_ptr = hld.get_guid_64(msg_data.role_guid)
	local player_ptr = hld.get_player_by_guid(guid_ptr)
	if player_ptr == nil then
		return;
	end
	delog(msg_data)
	player_ptr:get_worship_target():add_new_record(target_guid_ptr, msg_data.ranking_type);
	player_ptr:get_worship_target():req_worship_player_end(msg_data.ranking_type, msg_data.worship_type, target_guid_ptr, msg_data.target_worship_value, msg_data.worship_result);
end 

define_WS[1647] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.sync_first_rank_end", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil or false == player_ptr:is_self_server() then
    	return;
    end
    player_ptr:get_title_mgr():ranking_first_changed(msg_data.ranking_type, msg_data.is_first, msg_data.is_notice);

end 


define_WS[1648] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.sync_server_avg_lv", message_data)
    if msg_data == nil then
    	return;
    end
    if msg_data.srv_avg_lv >= 0 then
		hld.get_cell_server():set_server_avg_level(msg_data.srv_avg_lv);
	end
	if msg_data.world_lv_cur >= 0 then
		hld.get_cell_server():set_world_level_cur(msg_data.world_lv_cur);
	end
	if msg_data.world_lv_last >= 0 then
		hld.get_cell_server():set_world_level_last(msg_data.world_lv_last);
	end
end 



define_WS[1652] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.req_service_rank_info_end", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local player_ptr = hld.get_player_by_guid(guid_ptr)
    if player_ptr == nil then
    	return;
    end
    player_ptr:get_service_rank_mgr():receive_reward_by_type_end(msg_data.rank_type, msg_data.rank_num);
end 


-----------------------------------------------------------------



define_WS[34905] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.check_item_enough", message_data)
    if msg_data == nil then
    	return;
    end
    local main_guid_ptr = hld.get_guid_64(msg_data.main_guid)
    local sub_guid_ptr = hld.get_guid_64(msg_data.sub_guid)

    local main_player_ptr = hld.get_player_by_guid(guid_ptr)
    if main_player_ptr == nil then
    	return;
    end
    if not msg_data.is_divorce then
		main_player_ptr:check_can_marry(sub_guid_ptr, msg_data.item_id, msg_data.is_cost_item);
	else
		main_player_ptr:check_can_divorce(msg_data.is_cost_item);
	end
end 

define_WS[34906] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.marry_result_end", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_marry_result_end_proc(conn_index, message_data, string.len(message_data))
end 


define_WS[34911] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.marry_get_reward", message_data)
    if msg_data == nil then
    	return;
    end
    local main_guid_ptr = hld.get_guid_64(msg_data.sender_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end

	main_player_ptr:get_new_married_reward_end(msg_data.old_marry_state);
end 

define_WS[34913] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.update_wedding_ring_state", message_data)
    if msg_data == nil then
    	return;
    end
    local main_guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end
	main_player_ptr:get_marry_mgr():refresh_wedding_ring_info(msg_data.is_marry, msg_data.marry_item_id, msg_data.mate_ring_level, msg_data.heart_value);
end 

define_WS[34915] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.update_wedding_ring_level", message_data)
    if msg_data == nil then
    	return;
    end
    local main_guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end
	main_player_ptr:get_marry_mgr():refresh_mate_wedding_ring_level(msg_data.mate_ring_level);
end 

--------------------------------------------------------------------------------------------------

define_WS[34921] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.refresh_heart_value", message_data)
    if msg_data == nil then
    	return;
    end
    local main_guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end
	main_player_ptr:get_marry_mgr():refresh_heart_value(msg_data.heart_value);
end 

define_WS[34922] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.req_cost_bless_money", message_data)
    if msg_data == nil then
    	return;
    end
    local main_guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end
	local target_guid_ptr = hld.get_guid_64(msg_data.target_guid)
	main_player_ptr:get_marry_mgr():cost_bless_money(target_guid_ptr);
end 


define_WS[34925] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.return_special_item_times", message_data)
    if msg_data == nil then
    	return;
    end

     local main_guid_ptr = hld.get_guid_64(msg_data.target_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end

	local has_use_time = main_player_ptr:get_logic_data(hld.e_role_logic_info_daily_lucky_times);
	if has_use_time == 0 then 
		return;
	end
	main_player_ptr:set_logic_data(hld.e_role_logic_info_daily_lucky_times, has_use_time - 1);
	main_player_ptr:send_logic_one(hld.e_role_logic_info_daily_lucky_times);
end 

define_WS[34918] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.marry_get_marry_task", message_data)
    if msg_data == nil then
    	return;
    end

    local main_guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end

	local  is_receive = main_player_ptr:get_mission_mgr():random_marry_mission(true, 0);
	if is_receive == true then
		local notice_str = hld.get_str_by_string_template_id(90096815, 0);
		main_player_ptr:send_notice(notice_str);
	end
end 

define_WS[34919] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.sync_marry_task", message_data)
    if msg_data == nil then
    	return;
    end

    local main_guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local main_player_ptr = hld.get_player_by_guid(main_guid_ptr)
	if main_player_ptr == nil then
		return;
	end
	if not msg_data.is_marry then	
		main_player_ptr:get_mission_mgr():clear_marry_mission();
	end
end 
------------------------------------------------------------------------------------------------------------------


define_WS[1664] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.send_mail", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_receive_mail(conn_index, message_data, string.len(message_data))
end 


define_WS[33008] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.had_new_globel_mail", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_refresh_globel_mail_sign_state(conn_index, message_data, string.len(message_data))
end 

define_WS[1669] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.receive_notice", message_data)
    if msg_data == nil then
    	return;
    end
    local addressee_guid_ptr = hld.get_guid_64(msg_data.addressee_guid)
    local  receive_player_ptr = hld.get_player_by_guid(addressee_guid_ptr);
    if receive_player_ptr == nil then
    	return;
    end
	local  sender_guid_ptr = hld.get_guid_64(msg_data.sender_guid);
	local  crate_time = msg_data.create_time;
	receive_player_ptr:get_chat_mgr():receive_notice(msg_data.notice_id, sender_guid_ptr, crate_time);
end 


define_WS[33301] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.send_chat_end", message_data)
    if msg_data == nil then
    	return;
    end
    local  guid_ptr = hld.get_guid_64(msg_data.sender_guid)
    local  player_ptr = hld.get_player_by_guid(guid_ptr);
	if player_ptr == nil then
		return;
	end
	player_ptr:get_chat_mgr():send_failed_process(msg_data.send_result, msg_data.chat_type);
end 
------------------------------------------------------------------------------------------------------


define_WS[33805] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.create_world_boss", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_create_world_boss_proc(conn_index, message_data, string.len(message_data))
end 

define_WS[33807] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.remove_world_boss", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_remove_world_boss_proc(conn_index, message_data, string.len(message_data))
end 

define_WS[33809] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.create_first_kill_prize", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.first_killer_guid);
    local self_player = hld.get_player_by_guid(guid_ptr);
	if self_player == nil then
        return;
    end
	self_player:get_first_kill_world_boss_prize_func(msg_data.first_kill_welfare_template_id);
end 


------------------------------------------------------------------------------------------------------

define_WS[1670] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.role_right", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid);
 	local player_ref = hld.get_player_by_guid(guid_ptr)
	if player_ref == nil then
		return;
	end
	player_ref:set_role_right(msg_data.right_type, msg_data.right_value);
end 



define_WS[1671] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.recharge", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_recharge_proc(conn_index, message_data, string.len(message_data))
end 


define_WS[1674] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.role_info_change", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid);
    local player_ref = hld.get_player_by_guid(guid_ptr)
	if player_ref == nil then
		return;
	end
	local role_type = msg_data.role_type;
	local role_value = msg_data.role_value;

	player_ref:set_unit_info(role_type, role_value); 
end

define_WS[1675] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.buy_good", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid);
  	local player_ref = hld.get_player_by_guid(guid_ptr);
	if player_ref == nil then
		return;
	end
	player_ref:buy_goods(msg_data.goods_id, msg_data.goods_num, msg_data.store_id, msg_data.is_auto_buy, msg_data.back_string);
end


define_WS[33014] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.get_red_package_end", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.receiver_guid);
  	local player_ref = hld.get_player_by_guid(guid_ptr);
	if player_ref == nil then
		return;
	end
	local package_guid_ptr = hld.get_guid_64(msg_data.red_package_guid);
	player_ref:get_red_pack_end(package_guid_ptr, msg_data.get_result, msg_data.money_type, msg_data.money_num);
end



define_WS[33012] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.send_red_package_end", message_data)
    if msg_data == nil then
    	return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.sender_guid);
  	local player_ref = hld.get_player_by_guid(guid_ptr);
	if player_ref == nil then
		return;
	end

	player_ref:send_red_package_end_proc(msg_data.red_package_template_id, msg_data.send_result);
end




define_WS[1375] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.full_friend_guid", message_data)
    if msg_data == nil then
    	return;
    end
  
  	local player_ref =  hld.get_player_by_index(msg_data.player_index)
	if player_ref == nil then
		return;
	end
	local  friend_num = msg_data.friend_num;
	player_ref:upgrade_highest_record(hld.e_role_history_highest_record_higest_friend_num, friend_num);
	player_ref:upgrade_highest_record(hld.e_role_history_highest_record_higest_friendliness_value, msg_data.highest_firendliness_value);
	player_ref:refresh_service_goal(hld.e_service_goal_type_friendliness);
	player_ref:get_achievement_mgr():player_changed(hld.eat_add_friend_count);
end


define_WS[1380] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.req_relation_end", message_data)
    if msg_data == nil then
    	return;
    end
  
  	local player_ref =  hld.get_player_by_index(msg_data.self_index)
	if player_ref == nil then
		return;
	end
	local target_guid_ptr = hld.get_guid_64(msg_data.target_guid);
	player_ref:req_relation_end(msg_data.relation_type, target_guid_ptr);
end
-------------------------------------------------------------------------------------------------------

define_WS[34707] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.auction_purchase_success", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_auction_purchase_success_proc(conn_index, message_data, string.len(message_data));
end

define_WS[34712] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.auction_send_sell_success_info", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_auction_sell_success_proc(conn_index, message_data, string.len(message_data));
end


define_WS[34716] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.auction_cancel_sell", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_auction_cancel_sell_end_proc(conn_index, message_data, string.len(message_data));
end


define_WS[34703] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.auction_sell_end", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_auction_sell_end_proc(conn_index, message_data, string.len(message_data));
end

define_WS[34727] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.auction_find_bid_target_end", message_data)
    if msg_data == nil then
        return
    end
    hld.lua_ws2cs_auction_find_bid_target_end_proc(conn_index, message_data, string.len(message_data))
end

define_WS[34752] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.auction_time_out_del", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_time_out_del_proc(conn_index, message_data, string.len(message_data));
end

----------------------------------------------------------------------------------------------------------

define_WS[35304] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.cloud_shop_buy_req", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_cloud_shop_buy_proc(conn_index, message_data, string.len(message_data));
end


define_WS[1390] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.sub_player_rename_item", message_data)
    if msg_data == nil then
    	return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.role_guid);
  	local player_ref = hld.get_player_by_guid(guid_ptr);
	if player_ref == nil then
		return;
	end
	player_ref:sub_rename_card(msg_data.item_templete_id);
end

define_WS[1389] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.change_player_name", message_data)
    if msg_data == nil then
    	return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.role_guid);
  	local player_ref = hld.get_player_by_guid(guid_ptr);
	if player_ref == nil then
		return;
	end
	if string.len(msg_data.role_name) <= 0 then
		return;
	end

	player_ref:change_name(msg_data.role_name);

	local  couple_guid_ptr = player_ref:get_couple_guid();
	local  player_couple_ref = hld.get_player_by_guid(couple_guid_ptr);
	if player_couple_ref == nil then
		return;
	end

	player_couple_ref:set_couple_name(msg_data.role_name);
end

define_WS[1690] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.spawn_npc", message_data)
    if msg_data == nil then
    	return;
    end
    hld.lua_ws2cs_gm_spawn_npc_func(conn_index, message_data, string.len(message_data));
end

define_WS[1692] = function (message_data, conn_index)
	local msg_data = decode("hld.ws2cs_proto.modify_money", message_data)
    if msg_data == nil then
    	return;
    end

    local player_ref = hld.get_player_by_index(msg_data.cs_unit_index);
  
	if player_ref == nil then
		return;
	end
	if msg_data.is_add_money  then
		player_ref:add_money_or_exp(msg_data.money_type, msg_data.money_value, hld.e_server_log_add_money_gm_order, 0);
	else
		player_ref.cut_money(msg_data.money_type, msg_data.money_value,hld.e_server_log_cut_money_gm_order, -1,0);
	end
end
--------------------------------------------------------------------------------------------------------------------
define_WS[36008] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.send_cur_server_harry_msg", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_set_cur_server_harry_func(conn_index, message_data, string.len(message_data))
end


define_WS[36021] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.send_create_normal_player", message_data)
    if msg_data == nil then
        return;
    end

    local  num = msg_data.crate_num;
    local  map_obj_ptr = hld.world_cs:get_instance():get_map_by_template_id(harry_map_id);
    if  map_obj_ptr == nil then
        return;
    end
    local map_cs_ptr = hld.map_cs:cast(map_obj_ptr);
    if map_cs_ptr == nil then
        return;
    end
    map_cs_ptr:create_harry_map_normal_npc(num);
end

define_WS[36008] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.create_harry_player_statue", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_set_cur_server_harry_func(conn_index, message_data, string.len(message_data))
end

define_WS[36100] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.cross_ladder_refresh_ticket_info", message_data)
    if msg_data == nil then
        return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local  player_ref = hld.get_player_by_guid(guid_ptr);
    if player_ref == nil then
        return;
    end
    player_ref:set_cross_ladder_can_buy_ticket_num(msg_data.can_buy_ticket_num);
end

define_WS[36120] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.cross_ladder_send_last_score", message_data)
    if msg_data == nil then
        return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local  player_ref = hld.get_player_by_guid(guid_ptr);
    if player_ref == nil then
        return;
    end
    player_ref:receive_cross_ladder_last_score(msg_data.last_score);
    player_ref:receive_cross_ladder_last_ranking(msg_data.last_ranking);
end

----------------------------------------------------------------------------------------

define_WS[36301] = function (message_data, conn_index)
    local msg_data = decode("hld.ws2cs_proto.cross_server_world_boss_creat_boss", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_cross_server_world_boss_creat_boss_func(conn_index, message_data, string.len(message_data))
end


define_WS[1680] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.check_time_limit_activity_common_data", message_data)
    if msg_data == nil then
        return;
    end

    local guid_ptr = hld.get_guid_64(msg_data.user_guid)
    local  player_ref = hld.get_player_by_guid(guid_ptr);
    if player_ref == nil then
        return
    end
    player_ref:get_time_limit_activity_mgr():check_time_limit_activity_common_data_to_ws_resp(msg_data.activity_id, msg_data.is_achieve);
end

define_WS[35410] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.end_act_type", message_data)
    if msg_data == nil then
        return;
    end

    local act_type = msg_data.act_type;
    local is_need_refresh = msg_data.is_need_send_to_client;
    hld.time_limit_activity_temp_mgr:get_instance():end_act_with_act_type(act_type, is_need_refresh);
end


define_WS[35408] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.act_limit_temp", message_data)
    if msg_data == nil then
        return;
    end
    --delog(msg_data)
    hld.lua_ws2cs_time_limit_act_temp(conn_index , message_data, string.len(message_data));
end

define_WS[35409] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.act_limit_branch_temp", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_time_limit_act_branch_temp(conn_index , message_data, string.len(message_data));
end

-------------------------------------------------------------------------------------------------------


define_WS[36802] = function (message_data, conn_index )  -- 废弃
    local msg_data = decode("hld.ws2cs_proto.send_bonfire_level", message_data)
    if msg_data == nil then
        return;
    end

    local  map_guid_ptr = hld.get_guid_64(msg_data.map_guid)

    local  map_ptr = hld.world_cs:get_instance():get_map_by_guid(map_guid_ptr);

    if  map_ptr == nil or map_ptr:get_map_template_type() ~= hld.e_map_type_legion_station then
        return;
    end
    local  map_cs_ptr = fath.legion_station_map_cs:cast(map_ptr);
    if  map_cs_ptr == nil then     
        return;
    end
    map_cs_ptr:set_bonfire_level(msg_data.bonfire_level);
end

define_WS[1631] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.set_legion_guid_to_map", message_data)
    if msg_data == nil then
        return;
    end

    local  map_guid_ptr = hld.get_guid_64(msg_data.map_guid)

    local  map_ptr = hld.world_cs:get_instance():get_map_by_guid(map_guid_ptr);

    if  map_ptr == nil then
        return;
    end
    local  legion_guid_ptr = hld.get_guid_64(msg_data.legion_guid)
    map_ptr:set_map_own_legion(legion_guid_ptr);
end

define_WS[1632] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.set_legion_guid_to_map", message_data)
    if msg_data == nil then
        return;
    end

    local  map_guid_ptr = hld.get_guid_64(msg_data.map_guid)

    local  map_ptr = hld.world_cs:get_instance():get_map_by_guid(map_guid_ptr);

    if  map_ptr == nil then
        return;
    end
    map_ptr:transfer_all_player(0);
    map_ptr:remove_all_monster();
end

define_WS[33816] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.sync_assist_fight_info", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_sync_assist_fight_info_proc(conn_index, message_data, string.len(message_data))
end


define_WS[37001] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.create_pk_king_combat_map_end", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_create_pk_king_combat_map_end_proc(conn_index, message_data, string.len(message_data))
end


define_WS[37112] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.element_war_send_create_robot", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_element_war_map_init_proc(conn_index, message_data, string.len(message_data))
end

define_WS[37121] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.element_war_check_can_get_mission_reward_end", message_data)
    if msg_data == nil then
        return;
    end
    hld.lua_ws2cs_element_war_get_mission_reward(conn_index, message_data, string.len(message_data))
end

define_WS[37123] = function (message_data, conn_index )
    local msg_data = decode("hld.ws2cs_proto.element_war_send_last_score", message_data)
    if msg_data == nil then
        return;
    end
    local guid_ptr = hld.get_guid_64(msg_data.role_guid)
    local player_ref = hld.get_player_by_guid(guid_ptr);
    if player_ref == nil then
        return
    end
    player_ref:receive_element_ladder_last_score(msg_data.last_score);
    player_ref.receive_element_ladder_last_ranking(msg_data.last_ranking);

end

 g_ws_msgproc = ws_msgproc

 return ws_msgproc





