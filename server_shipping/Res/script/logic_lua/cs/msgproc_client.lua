local msgproc_client={}
function msgproc_client:init()
end

define_C[10004] = function(message_data, player_ref)
	local msg_data = decode("hld.game_proto.client2cs_ping", message_data);
	msgproc_client:c2cs_ping(player_ref, msg_data.trace_time);
end

define_C[10320] = function(message_data, player_ref)
	local msg_data = decode("hld.game_proto.get_temple_over_gift", message_data);
	msgproc_client:c2cs_req_temple_gift_num(player_ref, msg_data.get_over_num);
end

define_C[10418] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.map_tigger_volume", message_data);
	msgproc_client:c2cs_req_map_tigger_volume(player_ref, msg_data.tigger_volume_id);
end

function msgproc_client:c2cs_ping(player_ref, trace_time)
	if nil == player_ref then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	local ping_msg = {};
	ping_msg.trace_time = trace_time;
	MessageMgr:send_message_to_self(player_ref, "hld.game_proto.cs2client_ping",ping_msg, 10005)
end

function msgproc_client:c2cs_req_temple_gift_num(player_ref, over_num)
	if nil == player_ref then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	local map_object = hld.get_map(player_ref:get_map_guid())
	if nil == map_object then
		return;
	end
	map_object:get_game_over_gift(player_ref:get_array_index(),over_num)
end

function msgproc_client:c2cs_req_map_tigger_volume(player_ref, tigger_volume_id)
	if nil == player_ref then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	local map_guid = player_ref:get_map_guid()
	local raid_map_cs = hld.get_raid_map_cs(map_guid)
	if nil == raid_map_cs then
		return;
	end
	raid_map_cs:map_tigger_volume(tigger_volume_id,0,true,player_ref:get_array_index());
end

-------------------------------------------------------------------------------------------------
define_C[10813] = function(message_data, player_ref)
    local msg_data = decode("hld.aoi_proto.unit_location", message_data);
    if msg_data == nil then
    	return 
    end

	if player_ref:is_valid() == false   or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:get_loading_flag_all_finish() == false or
	   player_ref:get_saveing_flag_all_finish() == false or
	   player_ref:is_during_transfer() == true then
	   return; 
	end
	hld.c2cs_req_aoi_location_lua(player_ref, message_data, string.len(message_data))

end

define_C[10814] = function(message_data, player_ref)
    local msg_data = decode("hld.aoi_proto.unit_rotation", message_data);
    if msg_data == nil then
    	return 
    end
    
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	hld.c2cs_req_aoi_rotation_lua(player_ref, message_data, string.len(message_data))
end

define_C[10815] = function(message_data, player_ref)
     
    local msg_data = decode("hld.aoi_proto.show_player", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    player_ref:aoi_change_show_player(req.show_num());
end

define_C[10811] = function(message_data, player_ref)
    local msg_data = decode("hld.aoi_proto.simple_unit_pos", message_data);
    if msg_data == nil then
    	return 
    end
    
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	hld.c2cs_req_aoi_simple_pos_lua(player_ref, message_data, string.len(message_data))
end

define_C[14607] = function(message_data, player_ref)
    local msg_data = decode("hld.game_proto.cross_server_pk_map_transfer", message_data);
    if msg_data == nil then
    	return 
    end
    
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	hld.c2cs_back_to_self_server_lua(player_ref, message_data, string.len(message_data))
end

-----------------------------------------------------------------------------------------------------------------------
define_C[10305] = function(message_data, player_ref)
    
    local msg_data = decode("hld.game_proto.transfer_scene", message_data);
    if msg_data == nil then
    	return 
    end
 
    
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog(msg_data)

	hld.c2cs_req_transfer_scene_lua(player_ref, message_data, string.len(message_data))
end

define_C[10310] = function(message_data, player_ref)
    
    local msg_data = decode("hld.game_proto.transfer_scene_pos", message_data);
    if msg_data == nil then
    	return 
    end
 
    
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog(msg_data)

	hld.c2cs_req_transfer_scene_pos_lua(player_ref, message_data, string.len(message_data))
end


define_C[10339] = function(message_data, player_ref)
    
    local msg_data = decode("hld.game_proto.transfer_scene_born", message_data);
    if msg_data == nil then
    	return 
    end
 
    
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog(msg_data)

	hld.c2cs_req_transfer_scene_born_lua(player_ref, message_data, string.len(message_data))
end


define_C[10445] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.lucky_draw", message_data);
    if msg_data == nil then
    	return 
    end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog(msg_data)

	player_ref:lucky_draw(msg_data.fuben_template_id);
end



define_C[10432] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.bag_unlock_slot_req", message_data);
    if msg_data == nil then
    	return 
    end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog(msg_data)

	player_ref:unlock_bag_slot(msg_data.bag_slot_index_want_to_unlock, msg_data.storage_slot_index_want_to_unlock);
	
end


define_C[10421] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.character_fuhuo", message_data);
    if msg_data == nil then
    	return 
    end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog(msg_data)
	
	player_ref:back_to_life(msg_data.mode ,false);
end

define_C[10480] = function(message_data, player_ref)
    local msg_data = decode("hld.character_proto.get_func_unlock_award", message_data);
    if msg_data == nil then
    	return 
    end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	local func_unlock_id = msg_data.func_unlock_template_id;
	if func_unlock_id <= 0 then
		return;
	end
	delog(msg_data)
	player_ref:get_func_unlock_award(func_unlock_id);
end

define_C[10334] = function(message_data, player_ref)
    local msg_data = decode("hld.game_proto.leave_map", message_data);
    if msg_data == nil then
    	return 
    end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end


	local  cur_map_guid = player_ref:get_map_guid();
	if cur_map_guid:is_valid() == false then	
		return;
	end

	player_ref:get_assist_fight_mgr():check_leave_assist_map();

	local guid_ptr = hld.get_guid_64();

	player_ref:transfer_by_template(player_ref:get_unit_info(hld.e_role_info_main_map_id), player_ref:get_main_line_id(), 0, guid_ptr, 0);

end

define_C[10486] = function(message_data, player_ref)
    local msg_data = decode("hld.character_proto.req_hit_and_get", message_data);
    if msg_data == nil then
    	return 
    end

    delog(msg_data)

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end


	local target_npc_guid_ptr = hld.get_guid_64(msg_data.npc_guid)
	player_ref:req_hit_and_get_npc_proc(target_npc_guid_ptr);
end


define_C[10410] = function(message_data, player_ref)
    local msg_data = decode("hld.character_proto.character_one_att_change", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	local  change_index = msg_data.attack_att_index;
	local  change_value = msg_data.attack_att_value;

	if (change_index <  ( hld.e_unit_attack_att_max + hld.e_unit_game_att_max)) then

		if ((change_index - hld.e_unit_attack_att_max == hld.e_unit_game_att_fight_lock) 
			or (change_index - hld.e_unit_attack_att_max == hld.e_unit_game_att_jump)
			or (change_index - hld.e_unit_attack_att_max == hld.e_unit_game_att_interaction)) then			
			  player_ref:get_pawn_att():set_game_att(change_index - hld.e_unit_attack_att_max, change_value, true);
		end
    end
end



define_C[10491] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.grade_up_level_ani_finish", message_data);
    if msg_data == nil then
    	return 
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	player_ref:grade_up_level_ani_end(msg_data.old_equip_buff_id, msg_data.new_equip_buff_id);
end



define_C[10498] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.unit_info_body_att", message_data);
    if msg_data == nil then
    	return 
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	hld.c2cs_req_base_body_att_lua(player_ref, message_data, string.len(message_data))
end

define_C[14200] = function (message_data, player_ref)
	local msg_data = decode("hld.game_proto.add_exp_raid_buff", message_data);
    if msg_data == nil then
    	return 
    end

    delog(msg_data)
    if player_ref:is_valid() == false  then
	 	return;
	end


	local map_object_ptr = hld.world_cs:get_instance():get_map_by_guid(player_ref:get_map_guid());
	if map_object_ptr == nil then
		return;
	end
	local  raid_map_cs_ptr = hld.raid_map_cs:cast(map_object_ptr);
	if  raid_map_cs_ptr == nil then
		return;
	end
	local damage_buff_count = raid_map_cs_ptr:add_exp_raid_attack_buff(player_ref, msg_data.add_count);
	if damage_buff_count < 0 then
		return;
	end




	local end_msg = 
	{
	  add_count = damage_buff_count
	}

	MessageMgr:send_message_to_self(player_ref, "hld.game_proto.add_exp_raid_buff_end",end_msg, 14201)
end

define_C[10508] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.change_role_class", message_data);
    if msg_data == nil then
    	return 
    end
     if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog(msg_data)

	if player_ref:change_role_class(msg_data.class_change) then		   
		 local end_msg = 
		 {
		    result = 1
		 }
		 MessageMgr:send_message_to_self(player_ref, "hld.character_proto.change_role_class_end",end_msg, 10509)
	end
end


define_C[10516] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.auto_use_hp_medicine", message_data);
    if msg_data == nil then
    	return 
    end
     if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end


	player_ref:set_auto_use_hp_pot(msg_data.health_percent, msg_data.is_auto_buy_medicine);

end


-------------------------------------------------------------------------------------------------------------------------
define_C[11703] = function (message_data, player_ref)
    delog("-----------create---team---------")
	local msg_data, err = decode("hld.team_proto.create_team", message_data);
    if msg_data == nil then
    	delog(err)
    	return 
    end
    if player_ref:is_valid() == false then
    	return
    end

    delog(msg_data)

    hld.team_cs_mgr:c2cs_create_team_lua(player_ref, message_data, string.len(message_data))
end

define_C[11705] = function (message_data, player_ref)
	local msg_data = decode("hld.team_proto.join_team", message_data);
    if msg_data == nil then
    	return 
    end
    if player_ref:is_valid() == false then
    	return
    end
    delog(msg_data)
    hld.team_cs_mgr:c2cs_join_team_lua(player_ref, message_data, string.len(message_data)) 
end

define_C[11740] = function (message_data, player_ref)
	local msg_data = decode("hld.team_proto.update_team_attribute", message_data);
    if msg_data == nil then
    	return 
    end
    if player_ref:is_valid() == false then
    	return
    end
    delog(msg_data)
    hld.team_cs_mgr:c2cs_change_team_type_lua(player_ref, message_data, string.len(message_data))
end


---------------------------------------------------------------------------------------------------------------

define_C[10736] = function (message_data, player_ref)
	local msg_data = decode("hld.item_proto.show_fashion", message_data);
    if msg_data == nil then
    	return 
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog(msg_data)



	local  is_show_fashion= msg_data.is_show_fashion;
	if is_show_fashion < 0 or  is_show_fashion > 1 then
		player_ref:set_unit_info(hld.e_role_info_show_fashion, 1);
	else
		player_ref:set_unit_info(hld.e_role_info_show_fashion, msg_data.is_show_fashion);
	end
	local  item_ref = player_ref:get_item_set();
	item_ref:equip_off_equip_show_buff();	--去装备光效buff

	player_ref:send_info_one(hld.e_role_info_show_fashion, true);

    local guid_a =  player_ref:get_unit_guid_A();
    local guid_b =  player_ref:get_unit_guid_B();
	local t_client_msg = 
	{
	   is_show_fashion = player_ref:get_unit_info(hld.e_role_info_show_fashion),
	   role_guid = { guid_a, guid_b},
    }

    MessageMgr:send_message_to_aoi_lua(player_ref, "hld.item_proto.show_fashion",t_client_msg, 10735, true, false)

	
	item_ref:set_show_fashion_buff();
	item_ref:equip_on_equip_show_buff();
end

define_C[10715] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_quick_call_mount", message_data);
    if msg_data == nil then
    	return 
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    hld.item_set_quick_call_mount_proc_lua(player_ref, message_data, string.len(message_data))
end


define_C[10717] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_hope_item_get", message_data);
    if msg_data == nil then
    	return 
    end

    local  get_typ = msg_data.get_typ;
	if get_typ < 0  or  get_typ >= hld.e_item_get_hope_item_typ_max then
		return;
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	player_ref:get_item_set():get_hope_item(get_typ);
end


-------------------------------------------------------------------------------------------------------------------------
local sky_equip_buff_id = 10992053;															--天空套装显示buffId
local skygod_equip_buff_id = 10992125;														--天神套装显示buffId
local supreme_equip_buff_id = 10992150;	

define_C[10520] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.change_sky_suit_shape", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end



	local  show_state = msg_data.operate_state;
	local  suit_type = msg_data.operate_type;
	if show_state < 0 or show_state > 3 then
		return;
	end
	if show_state == 1 then

		if player_ref:get_item_set():can_show_sky_shape(suit_type) == false then
			return;
		end
		hld.buff_man:get_instance():add_buff_inst(player_ref:get_array_index(), player_ref:get_array_index(), sky_equip_buff_id,nil,false,-1);

	else
		hld.buff_man:get_instance():del_buff_by_template_id(player_ref:get_array_index(), player_ref:get_array_index(), sky_equip_buff_id, false);
	end
	if show_state == 2 then	
		if player_ref:get_item_set():can_show_skygod_shape(suit_type) == false then
			return;
		end
		hld.buff_man:get_instance():add_buff_inst(player_ref:get_array_index(), player_ref:get_array_index(), skygod_equip_buff_id,nil,false,-1);
	else
		hld.buff_man:get_instance():del_buff_by_template_id(player_ref:get_array_index(), player_ref:get_array_index(), skygod_equip_buff_id, false);
	end
	if show_state == 3 then
		
		if player_ref.get_item_set().can_show_supreme_shape(suit_type) == false then
			
				return;
		end
		hld.buff_man:get_instance():add_buff_inst(player_ref:get_array_index(), player_ref:get_array_index(), supreme_equip_buff_id,nil,false,-1);
		
	else
		hld.buff_man:get_instance():del_buff_by_template_id(player_ref:get_array_index(), player_ref:get_array_index(), supreme_equip_buff_id, false);
	end
	player_ref:set_unit_info(hld.e_role_info_show_sky_suit_shape, show_state);
	player_ref:send_info_one(hld.e_role_info_show_sky_suit_shape, true);

	MessageMgr:send_message_to_self(player_ref, "hld.item_proto.change_sky_suit_shape", msg_data, 10521)
end

-------------------------------------------------------------------------------------------------

define_C[10601] = function (message_data, player_ref)
	local msg_data = decode("hld.skill_proto.skill_operation", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog("------hld.skill_proto.skill_operation----")
	hld.skill_req_message_lua(player_ref, message_data, string.len(message_data))
end

define_C[14101] = function (message_data, player_ref)
	local msg_data = decode("hld.buff_proto.buff_operation", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog("------hld.buff_proto.buff_operation---")
	hld.buff_req_message_lua(player_ref, message_data, string.len(message_data))
end


define_C[10433] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.character_gmorder_message", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog("-----hld.character_proto.character_gmorder_message--")
	hld.c2s_player_gmorder_message_lua(player_ref, message_data, string.len(message_data))
end


define_C[10435] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.pick_drop_box", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    local guid_ptr = hld.get_guid_64(msg_data.drop_box_guid)
    local unit_idf_ptr = hld.s_unit_identifier:new_local(guid_ptr, msg_data.runtime_id);

    delog(msg_data)

	player_ref:on_req_pick_drop_box(unit_idf_ptr);
end




define_C[10454] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.pk_mode_change", message_data);
    if msg_data == nil then
    	return 
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog(msg_data)

    player_ref:get_pk_community_mgr():change_pk_info(msg_data.pk_info_type, msg_data.pk_info_value, false);
	
end


define_C[10463] = function (message_data, player_ref)
	local  msg_data = decode("hld.character_proto.start_meditation", message_data)
	if msg_data == nil then
		return
	end


    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog(msg_data)

	player_ref:get_meditation_mgr():start_mediation(true);
end




define_C[10462] = function (message_data, player_ref)
	local  msg_data = decode("hld.character_proto.stop_meditation", message_data)
	if msg_data == nil then
		return
	end


    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog(msg_data)

	player_ref:get_meditation_mgr():stop_mediation();
end



define_C[10506] = function (message_data, player_ref)
	local  msg_data = decode("hld.character_proto.treasure_notice", message_data)
	if msg_data == nil then
		return
	end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
    delog("---hld.character_proto.treasure_notice-----")

    delog(msg_data)

	player_ref:get_treasure_mgr():send_treasure_notice(msg_data.item_id, msg_data.item_quality, msg_data.operate_type);



end



define_C[10476] = function (message_data, player_ref)
	local  msg_data = decode("hld.character_proto.add_guide_record", message_data)
	if msg_data == nil then
		return
	end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	 delog("---hld.character_proto.add_guide_record-----")

    delog(msg_data)

	hld.c2s_add_guide_record_lua(player_ref, message_data, string.len(message_data))
end







define_C[10505] = function (message_data, player_ref)
	local  msg_data = decode("hld.character_proto.guide_trigger_start", message_data)
	if msg_data == nil then
		return
	end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    
	delog("---hld.character_proto.guide_trigger_start-----")
    
    delog(msg_data)


	player_ref:get_guide_mgr():guide_trigger_start_proc(msg_data.guide_trigger_id);
end



----------------------------------------------------------------------------------------------------------------

define_C[12003] = function ( message_data , player_ref)
	local msg_data = decode("hld.ranking_proto.worship_player", message_data)
	if msg_data == nil then
	   return
	end


    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	local guid_ptr = hld.get_guid_64(msg_data.target_guid)

	delog("-----hld.character_proto.worship_player-------")
	delog(msg_data)
	player_ref:get_worship_target():req_worship_player(msg_data.ranking_index, msg_data.worship_type, guid_ptr);
end

define_C[12014] = function (message_data, player_ref)
	local msg_data = decode("hld.ranking_proto.service_rank_recevice_reward", message_data)
	if msg_data == nil then
		return 
	end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog("-----hld.ranking_proto.service_rank_recevice_reward-------")
	player_ref:get_service_rank_mgr():receive_reward_by_type(msg_data.service_rank_type);
end


define_C[11801] = function (message_data, player_ref)
	local msg_data = decode("hld.mail_proto.mail_operation", message_data)
	if msg_data == nil then
	 	return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
    delog("-----hld.mail_proto.mail_operation-------")
	delog(msg_data)

	hld.c2s_operate_mail_lua(player_ref, message_data, string.len(message_data))
end


define_C[12207] = function (message_data, player_ref)
	local msg_data = decode("hld.chat_proto.notice_info", message_data)
	if msg_data == nil then
		return 
	end

	if #msg_data.role_guid ~= 2 then	
		return;
    end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    delog("-----chat_proto.notice_info-------")
	delog(msg_data)

	player_ref:get_chat_mgr():send_notice(msg_data.notice_id, msg_data.final_string);
end


define_C[12223] = function (message_data, player_ref)
	local msg_data = decode("hld.chat_proto.notice_info_with_param", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    delog("-----hld.chat_proto.notice_info_with_param-------")


	hld.c2s_send_notice_with_param_lua(player_ref, message_data, string.len(message_data))
end


define_C[12212] = function ( message_data, player_ref)
	local msg_data = decode("hld.chat_proto.chat_content",  message_data)
	if msg_data == nil then
		return 
	end

	if #msg_data.addressee_guid ~= 2 then	
		return;
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    --delog("-----hld.chat_proto.chat_content-------")
	--delog(msg_data)

	hld.c2s_send_chat_new_lua(player_ref, message_data, string.len(message_data))
end


define_C[12503] = function ( message_data, player_ref )
	local msg_data = decode("hld.character_proto.recharge", message_data)
	if msg_data == nil then
		return 
	end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	if hld.get_cell_server():get_gm_state() == false and not player_ref:get_is_robot() then
		return;
	end
	delog("-----hld.character_proto.recharge-------")
	delog(msg_data)
	player_ref:recharge_by_template_id(msg_data.recharge_typ, "", 0, hld.e_payment_method_test, 0, 1);
end

define_C[12505] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.recharge_widget_type", message_data)
	if msg_data == nil then
		return
	end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	if hld.get_cell_server():get_gm_state() == false and not player_ref:get_is_robot() then
		return;
	end


	delog("-----hld.character_proto.recharge_widget_type-------")
	delog(msg_data)

	player_ref:set_recharget_widget_type(msg_data.is_dead_recharge);
end

define_C[12611] = function (message_data, player_ref)
    local msg_data = decode("hld.welfare_proto.get_first_recharge_time", message_data)
    if msg_data == nil then
    	return
    end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("-----hld.welfare_proto.get_first_recharge_time-------")
	delog(msg_data)
	player_ref:get_welfare_mgr():get_first_recharge_welfare_by_id();
end


define_C[13207] = function ( message_data, player_ref)
	local msg_data = decode("hld.title_proto.title_set_title_no_need_see", message_data)
    if msg_data == nil then
    	return
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("-----hld.title_proto.title_set_title_no_need_see-------")
	delog(msg_data)
	player_ref:get_title_mgr():set_title_no_need_see();
end


define_C[13302] = function ( message_data, player_ref)
	local msg_data = decode("hld.character_proto.worship", message_data)
    if msg_data == nil then
    	return
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("-----hld.character_proto.worship------")
	delog(msg_data)
	player_ref:worship_by_type(msg_data.worship_type, (msg_data.is_cost_diamond > 0));
end

define_C[14158] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.find_other_unit_lua", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	local server_id = player_ref:get_unit_info(hld.e_role_info_server_id);
	if  player_ref:is_self_server() == false then
		server_id = hld.get_cell_server():get_server_id();
	end

	target_guid_ptr = hld.get_guid_64(msg_data.target_guid_a, msg_data.target_guid_b)
	local  other_player_ref = hld.get_player_by_guid(target_guid_ptr);
	if  other_player_ref ~= nil and other_player_ref:is_valid() == true then
		server_id = other_player_ref:get_unit_info(hld.e_role_info_server_id);
	else
		server_id = msg_data.server_id;
	end
	delog("-----hld.character_proto.find_other_unit_lua-------")
	delog(msg_data)
	player_ref:find_other_player_info(target_guid_ptr, server_id);
end


define_C[13401] = function (message_data, player_ref)
	local  msg_data = decode("hld.character_proto.send_red_package", message_data)
	if msg_data == nil then
		return 
	end

	if #msg_data.item_guid < 2 then
	    return 
	end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("----hld.character_proto.send_red_package-------")
	delog(msg_data)

    local item_guid_ptr = hld.get_guid_64(msg_data.item_guid[1], msg_data.item_guid[2])

    player_ref:send_red_package(msg_data.red_package_id, msg_data.send_text, item_guid_ptr);
end

define_C[13405] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.get_red_package", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("-----hld.character_proto.get_red_package-------")
	delog(msg_data)

	local  red_package_guid_ptr = hld.get_guid_64(msg_data.red_package_guid[1], msg_data.red_package_guid[2])

	player_ref:get_red_package(red_package_guid_ptr);
end


define_C[10479] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.transfer_buff", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("-----hld.character_proto.transfer_buff-------")
	delog(msg_data)

	player_ref:transfer_buff(msg_data.operate_type);
end

define_C[13503] = function ( message_data, player_ref)
	local msg_data = decode("hld.first_time_do_proto.add_first_time_do_record", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("----hld.first_time_do_proto.add_first_time_do_record-------")
	delog(msg_data)


	local  do_type = msg_data.do_type;

	local first_time_do_mgr_ref = player_ref:get_first_time_do_mgr();
	first_time_do_mgr_ref:add_do_record(do_type);
end

---------------------------------------------------------------------------------------------------------------


define_C[11642] = function (message_data, player_ref)
	local msg_data = decode("hld.chat_proto.chat_record_oper", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog("------msg_data.pos---", msg_data.pos)

	delog("----hld.chat_proto.chat_record_oper-------")
	delog(msg_data)

	--delog("------msg_data.pos---", msg_data.pos)

	player_ref:get_chat_mgr():chat_oper(msg_data.pos, msg_data.record);
end


define_C[11658] = function (message_data, player_ref)
	local msg_data = decode("hld.recycle_proto.recycle_invited_reward", message_data)
	if msg_data == nil then
		return 
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("---hld.recycle_proto.recycle_invited_reward-------")
	delog(msg_data)


    player_ref:get_recycle_mgr():on_process_invited_reward(msg_data.inviter_guid);
end




define_C[10515] = function ( message_data, player_ref)  -- 无参数-c2s_first_look_legion
	local msg_data = decode("hld.character_proto.jump_raid_animation", message_data)
	if msg_data == nil then
	 	return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end


	delog("---c2s_first_look_legion-------")
	delog(msg_data)


	player_ref:set_logic_data(hld.e_role_logic_info_first_look_legion, 1);
	player_ref:send_logic_one(hld.e_role_logic_info_first_look_legion);
end


define_C[14804] = function ( message_data, player_ref)
	local msg_data = decode("hld.game_proto.give_server_item_msg", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if not player_ref:get_server_harry_mgr():is_have_item() then
		return;
	end


	delog("---hld.game_proto.give_server_item_msg-------")
	delog(msg_data)

	player_ref:get_server_harry_mgr():give_server_item_to_npc();
	player_ref:change_harry_buff();
end



define_C[15108] = function (message_data, player_ref)
	local msg_data = decode("hld.game_proto.get_map_boss_is_dead", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	hld.c2s_cross_boss_get_boss_is_dead_lua(player_ref, message_data,  string.len(message_data));
end



define_C[15110] = function (message_data , player_ref)
	local msg_data = decode("hld.game_proto.get_boss_god_time", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end


    hld.c2s_get_boss_god_time_lua(player_ref, message_data,  string.len(message_data));
end

define_C[13653] = function (message_data, player_ref)
	local msg_data = decode("hld.relation_proto.c2s_req_use_fireworks", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

   	delog("---hld.relation_proto.c2s_req_use_fireworks-------")
	delog(msg_data)
    
    local guid_ptr = hld.get_guid_64(msg_data.obj_guid_a, msg_data.obj_guid_b)

	player_ref:get_marry_mgr():req_use_fireworks(guid_ptr, msg_data.target_name, msg_data.target_blessing);
end


define_C[13654] = function (message_data, player_ref)
	local msg_data = decode("hld.relation_proto.c2s_req_lucky_info", message_data)
	if msg_data == nil then
	   return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	player_ref:get_marry_mgr():req_lucky_operator(msg_data.req_type);

end


define_C[10523] = function (message_data, player_ref)
   local msg_data = decode("hld.character_proto.get_main_person_information", message_data)
   if msg_data == nil then
   	  return
   end
   if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
   end
    delog("---hld.character_proto.get_main_person_information------")
	delog(msg_data)

   player_ref:get_person_infor_mgr():sync_to_client();
end


define_C[10526] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.get_other_person_information", message_data)
	if  msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.character_proto.get_other_person_information------")
	delog(msg_data)

    local guid_ptr = hld.get_guid_64(msg_data.role_guid_a, msg_data.role_guid_b)

    player_ref:get_person_infor_mgr():get_other_person_information_to_dp(guid_ptr, false);

end


define_C[10550] = function ( message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.get_role_competition", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.cross_ladder.get_role_competition------")
	delog(msg_data)
    player_ref:get_competition_mgr():sync_competition_info();

end


define_C[10552] = function (message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.get_competition_lv_reward", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.cross_ladder.get_competition_lv_reward------")
	delog(msg_data)

    player_ref:get_competition_mgr():get_reward(msg_data.manual_id, msg_data.manual_type);

end

define_C[10554] = function (message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.get_competition_lv_reward_all", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end


    delog("---hld.cross_ladder.get_competition_lv_reward_all------")
	delog(msg_data)

    player_ref:get_competition_mgr():get_reward_all(hld.e_competition_manual_get_reward_type_all);

end

define_C[10555] = function (message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.buy_competition_lv", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.cross_ladder.buy_competition_lv------")
	delog(msg_data)

    player_ref:get_competition_mgr():buy_competition_level(msg_data.level);

end

define_C[10557] = function ( message_data, player_ref)
	 local msg_data = decode("hld.cross_ladder.buy_speical_manual", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
     end

     delog("---hld.cross_ladder.buy_speical_manual------")
	 delog(msg_data)

     player_ref:get_competition_mgr():buy_speical_manual(msg_data.manual_type);
end

define_C[10560] = function ( message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.get_role_competition", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.cross_ladder.get_role_competition------")
	delog(msg_data)
    player_ref:get_competition_mgr():sync_competition_info();

end


define_C[10562] = function (message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.get_competition_lv_reward", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.cross_ladder.get_competition_lv_reward------")
	delog(msg_data)

    player_ref:get_competition_mgr():get_reward(msg_data.manual_id, msg_data.manual_type);

end

define_C[10564] = function (message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.get_competition_lv_reward_all", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end


    delog("---hld.cross_ladder.get_competition_lv_reward_all------")
	delog(msg_data)

    player_ref:get_competition_mgr():get_reward_all(hld.e_competition_manual_get_reward_type_all);

end

define_C[10565] = function (message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.buy_competition_lv", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.cross_ladder.buy_competition_lv------")
	delog(msg_data)

    player_ref:get_competition_mgr():buy_competition_level(msg_data.level);

end

define_C[10567] = function ( message_data, player_ref)
	 local msg_data = decode("hld.cross_ladder.buy_speical_manual", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
     end

     delog("---hld.cross_ladder.buy_speical_manual------")
	 delog(msg_data)

     player_ref:get_competition_mgr():buy_speical_manual(msg_data.manual_type);
end


define_C[15500] = function (message_data, player_ref)
	local msg_data = decode("hld.legion_proto.get_map_legion_player", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end
    local map_object_ptr = hld.world_cs:get_instance():get_map_by_guid(player_ref:get_map_guid())
	if  map_object_ptr == nil  or  map_object_ptr:get_map_type() ~=  hld.e_map_type_legion_station then
		return;
	end
	local  map_cs_ptr = hld.legion_station_map_cs:cast(map_object_ptr);
	if  map_cs_ptr == nil then
		return;
	end


    delog("---hld.legion_proto.get_map_legion_player-----")
	delog(msg_data)
	map_cs_ptr:send_map_all_play_info(player_ref:get_unit_guid());
end


define_C[15601] = function (message_data, player_ref)
	local msg_data = decode("hld.assist_fight_proto.start_assist_fight", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("---hld.assist_fight_proto.start_assist_fight-----")
	delog(msg_data)
    player_ref:get_assist_fight_mgr():start_assist_fight(msg_data.assist_fight_type, msg_data.assist_fight_tag);
end


define_C[15605] = function ( message_data, player_ref)
	local msg_data = decode("hld.assist_fight_proto.cancel_assist_fight", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end
    delog("---hld.assist_fight_proto.cancel_assist_fight-----")
	delog(msg_data)

    player_ref:get_assist_fight_mgr():cancel_assist_fight();
end

-------------------------------------------------------------------------------------------------------------------

define_C[15609] = function (message_data, player_ref)
	local msg_data = decode("hld.assist_fight_proto.create_assist_fight_npc", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog("-------hld.assist_fight_proto.create_assist_fight_npc------")
    delog(msg_data)

    hld.c2s_create_assist_fight_npc_proc_lua(player_ref, message_data, string.len(message_data))

end


define_C[10532] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.save_customize_appearance_info", message_data)
	if msg_data == nil then
		return
	end

	if player_ref:is_valid() == false then
		return
	end

	delog(msg_data)

	hld.c2s_save_appearance_info_lua(player_ref, message_data, string.len(message_data))
end


define_C[10537] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.show_vip", message_data)
	if msg_data == nil then
		return
	end

	if player_ref:is_valid() == false then
		return
	end

	delog(msg_data)

	player_ref:show_vip(msg_data.is_show_vip);
end


define_C[11668] = function (message_data, player_ref)
	local msg_data = decode("hld.mount_power_proto.mount_power_operate", message_data)
	if msg_data == nil then
		return
	end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog(msg_data)

	hld.c2s_mountpower_oper_lua(player_ref, message_data, string.len(message_data))
end


define_C[10755] = function (message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_jewel_carve_oper", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end


    delog(msg_data)


	local guid_ptr = hld.get_guid_64(msg_data.item_guid[1], msg_data.item_guid[2])

    player_ref:get_jewel_carve_mgr():on_oper(guid_ptr, msg_data.oper_type, msg_data.param);
end



define_C[16116] = function (message_data, player_ref)
	 local msg_data = decode("hld.game_proto.element_war_buy_ticket", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
     end

     hld.c2s_element_war_buy_ticket_lua(player_ref, message_data, string.len(message_data))
end


define_C[16122] = function (message_data, player_ref)
	local msg_data = decode("hld.game_proto.get_map_target_show_info_list", message_data)
	if msg_data == nil then
	   return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    local  map_ptr = hld.world_cs:get_instance():get_map_by_guid(player_ref:get_map_guid());
	if map_ptr == nil then
		return;
	end
	map_ptr:send_map_target_list(player_ref:get_unit_guid());
end


define_C[13658] = function (message_data, player_ref)
	local msg_data = decode("hld.mission_proto.use_power_up", message_data)
	if msg_data == nil then
	  return
	end

	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

	player_ref:get_mission_mgr():set_power_up_be_use(msg_data.power_up_type);
end



define_C[16201] = function (message_data , player_ref)
    local msg_data = decode("hld.skytreasure_proto.skytreasure_operate", message_data)
    if msg_data == nil then
    	return
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
    end

    delog(msg_data)

    player_ref:get_skytreasure_mgr():on_operate(msg_data.operation_type, msg_data.operation_param1, msg_data.operation_param2);

end


define_C[16204] = function (message_data, player_ref)
	 local msg_data = decode("hld.starark_proto.starark_operate", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
     end


     delog(msg_data)


     player_ref:get_starark_mgr():on_operate(msg_data.oper_type, msg_data.oper_param1, msg_data.oper_param2, msg_data.oper_param3);

end





































return msgproc_client






