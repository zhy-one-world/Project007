local dungeon = {}


function dungeon:init()
	-- body
end

define_C[14250] = function(message_data, player_ref)
	local msg_data = decode("hld.game_proto.buy_raid_count", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	dungeon:buy_raid_count(player_ref, msg_data)

end

define_C[10356] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.get_first_raid_award", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	dungeon:get_first_raid_award(player_ref, msg_data)
end


define_C[10447] = function (message_data, player_ref)
    local msg_data = decode("hld.character_proto.mopping_up", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end
	dungeon:mopping_up(player_ref, msg_data)
end

define_C[10450] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.mopping_up_demons_tower", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	dungeon:mopping_up_demons_tower(player_ref, msg_data)
end

define_C[10451] = function ( message_data, player_ref)
	local msg_data = decode("hld.character_proto.mopping_up_demons_tower_get_award", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	dungeon:mopping_up_demons_tower_get_award(player_ref, msg_data)
end

define_C[10377] = function (message_data, player_ref)
	local msg_data = decode("hld.game_proto.buy_simple_boss_num", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	dungeon:buy_simple_boss_num(player_ref, msg_data)
end


function dungeon:buy_raid_count(player_ref, msg_data)
	 delog(msg_data)

	 local n_map_temp_id = msg_data.map_temp_id;
	 local n_buy_count = msg_data.buy_count;
	 local is_success_buy_count = player_ref:buy_count_with_map_id(n_map_temp_id, n_buy_count)
	 if  is_success_buy_count  == false then
		 return;
	 end

	 delog("------buy---raid---count---")

	local t_ret_msg = {
	    map_temp_id = n_map_temp_id,
	    buy_count = n_buy_count,
	}

	MessageMgr:send_message_to_self(player_ref, "hld.game_proto.buy_raid_count_end",msg_data, 14251)
end


function dungeon:get_first_raid_award(player_ref, msg_data)

	local  map_id = msg_data.map_temp_id;

    local map_record_set_ref = hld.get_map_record_set_by_index(player_ref:get_array_index());

    delog("----map_record_set_ref", map_record_set_ref)
	if map_record_set_ref:is_valid() == false then
		return;
	end
	local can_get = map_record_set_ref:can_get_first_award(map_id);
	if  can_get == false then
		return;
	end
	local success_get = map_record_set_ref:get_first_award(map_id);
	if success_get == false then
		return;
	end
	local map_template_ptr = template_manager:get_line("MapTemplate", map_id);
	if map_template_ptr == nil then
		return;
	end
	local award_item_arr = map_template_ptr.FirstWinAwardItems;
	local award_item_num_arr = map_template_ptr.FirstWinAwardItemsNum;

	if  #award_item_arr == 0 or #award_item_arr ~= #award_item_num_arr  then
		return
	end

	local item_tuple_array = {}

	for i=1,#award_item_arr do
		local item_id = award_item_arr[i];
		local item_num = award_item_num_arr[i];
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr == nil then
            return
		end
		local o_item_ptr = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_raid_reward, map_id, item_id, item_num, 1,0,0,0);
        if o_item_ptr ~= nil then
             player_ref:get_item_set():put_item_into_bag(o_item_ptr)
             table.insert(item_tuple_array, item_id)
             table.insert(item_tuple_array, item_num)
             table.insert(item_tuple_array, o_item_ptr:get_data_info(hld.e_item_info_locked))
        end 
	end

	welfare:get_item_send_promp_msg_to_client(player_ref, item_tuple_array,false)

end


function dungeon:mopping_up(player_ref, msg_data)
	 delog(msg_data)
	 local  raid_template_id = msg_data.raid_template_id;
	 local  mopping_up_count = msg_data.mopping_up_count;

	 local auto_convert_equip_to_magic_crystal = msg_data.auto_convert_equip_to_magic_crystal > 0 and true  or false;

	 delog("----auto_convert_equip_to_magic_crystal-", auto_convert_equip_to_magic_crystal)

     local map_record_set_ref = hld.get_map_record_set_by_index(player_ref:get_array_index());
	 if map_record_set_ref:is_valid() == false then
		 return;
	 end
	 map_record_set_ref:mopping_up(raid_template_id, auto_convert_equip_to_magic_crystal, mopping_up_count);
end


function dungeon:mopping_up_demons_tower(player_ref, msg_data)
	 local  map_record_set_ref = hld.get_map_record_set_by_index(player_ref:get_array_index());
     if map_record_set_ref:is_valid() == false then
		 return;
	 end
	 map_record_set_ref:start_mopping_up_demons_tower();
end


function dungeon:mopping_up_demons_tower_get_award(player_ref, msg_data)
	local  map_record_set_ref = hld.get_map_record_set_by_index(player_ref:get_array_index());
	if map_record_set_ref:is_valid() == false then
		return;
	end
	map_record_set_ref:mopping_up_demons_tower_get_award(msg_data.is_resolve);
end



function dungeon:buy_simple_boss_num(player_ref, msg_data)
	delog(msg_data)
	if msg_data.map_type == hld.e_map_type_boss_single then
		player_ref:get_gain_treasure_mgr():buy_single_boss_num();
	end
end


g_dungeon = dungeon

return dungeon