local legion = {}


local first_legion_template_id = 38000001;		-- 军团表里第一个条目的ID
local e_legion_error_already_get_occupation_daily_award = 28
local e_legion_daily_rwd_flag_daily_everyone = 30


local max_name_size = 100

function legion:init()
	-- body
end


define_C[12123] = function(message_data, player_ref)
    local msg_data = decode("hld.legion_proto.convenient_join_legion", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	legion:convenient_join_legion(player_ref, msg_data)
end



define_C[12059] = function ( message_data, player_ref)
	local msg_data = decode("hld.legion_proto.draw_mem_daily_award", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	if player_ref:get_legion_guid():is_valid() == false then
		return;
	end


	legion:draw_mem_daily_award(player_ref, msg_data)
end

define_C[12188] = function (message_data, player_ref)
	local msg_data = decode("hld.legion_proto.legion_territory_buff_operate", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	if player_ref:get_legion_guid():is_valid() == false then
		return;
	end

	legion:legion_territory_buff_operate(player_ref, msg_data)

end


define_C[12190] = function (message_data, player_ref)
	local msg_data = decode("hld.legion_proto.goto_next_stage_of_bonfire_map", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	if player_ref:get_legion_guid():is_valid() == false then
		return;
	end

	legion:goto_next_stage_of_bonfire_map(player_ref, msg_data)

end


define_C[12057] = function (message_data, player_ref)
	local msg_data = decode("hld.legion_proto.invite_player", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end
	legion:invite_player(player_ref, msg_data)
end


define_C[12116] = function (message_data, player_ref)
	local msg_data = decode("hld.legion_proto.apply_to_join_legion", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end
	legion:apply_to_join_legion(player_ref, msg_data)
end



define_C[15401] = function (message_data, player_ref)
	local msg_data = decode("hld.game_proto.add_exp_raid_buff", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	legion:c2cs_add_damage_buff_in_legion_boss(player_ref, msg_data)
end






function legion:convenient_join_legion(player_ref, msg_data)
	delog("-----convenient_join_legion--------")
	if player_ref:get_legion_cs_mgr():can_convenient_join_legion() == false then
		return;
	end
	player_ref:get_legion_cs_mgr():set_last_convenient_join_legion_time()

	local msg_ptr = hld.cs2ws_convenient_join_legion:new_local()

	msg_ptr.role_guid = player_ref:get_unit_guid();

    player_ref:send_message_to_ws_lua(msg_ptr, msg_ptr:get_len(), 0)
end


function legion:draw_mem_daily_award(player_ref, msg_data)
	delog(msg_data)
    local  award_level = player_ref:get_player_legion_level_3();
	local  legion_template_id = first_legion_template_id + award_level - 1;
	local  legion_template_ptr = template_manager:get_line("LegionConstructionsTemplate", legion_template_id);
	if legion_template_ptr == nil or  #legion_template_ptr.MemDailyAwards == 0 or #legion_template_ptr.MemDailyAwards % 2 ~= 0 then
		return;
	end
	
	local legion_daily_rwd_flag_data = player_ref:get_time_data(hld.e_time_type_legion_daily_award_flags);
	local mask_flag  =  1 << (e_legion_daily_rwd_flag_daily_everyone - 1)
	local bit_ret =  legion_daily_rwd_flag_data & mask_flag;

	if  bit_ret ~=  0 then
	
		-- 今天已经领取过就不能再领取了
		local  t_ret_msg = {
           error_type = e_legion_error_already_get_occupation_daily_award,
	    }
		MessageMgr:send_message_to_self(player_ref, "hld.legion_proto.legion_error",t_ret_msg, 12176)
		return;
	end
	local get_bit =  legion_daily_rwd_flag_data | mask_flag;
	player_ref:set_time_data(hld.e_time_type_legion_daily_award_flags, get_bit);
	player_ref:send_time_one(hld.e_time_type_legion_daily_award_flags);
	
	local award_item_arr = legion_template_ptr.MemDailyAwards;




	local item_tuple_array = {}

	for i=1,#award_item_arr, 2 do
		local item_id = award_item_arr[i];
		local item_num = award_item_arr[i + 1];
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr == nil then
            return
		end
		local o_item_ptr = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_activate_code, 0, item_id, item_num, 1,0,0,0);
        if o_item_ptr ~= nil then
             player_ref:get_item_set():put_item_into_bag(o_item_ptr)
             table.insert(item_tuple_array, item_id)
             table.insert(item_tuple_array, item_num)
             table.insert(item_tuple_array, o_item_ptr:get_data_info(hld.e_item_info_locked))
        end 
	end
	welfare:get_item_send_promp_msg_to_client(player_ref, item_tuple_array,false)
end


function legion:legion_territory_buff_operate(player_ref, msg_data)
    local legion_cs_ref = player_ref:get_legion_cs_mgr();

	local operate_type = msg_data.operate_type;
	if  operate_type == 0 then
		legion_cs_ref:del_territory_buff(player_ref:get_array_index());
	elseif operate_type == 1 then	
		legion_cs_ref:add_territory_buff(player_ref:get_array_index(), msg_data.territory_id);
	end
end



function legion:goto_next_stage_of_bonfire_map(player_ref, msg_data)
  	local  map_object_ptr = hld.world_cs:get_instance():get_map_by_guid(player_ref:get_map_guid());
	if map_object_ptr == nil then
		return;
	end

	if map_object_ptr:get_map_template_type() ~= hld.e_map_type_legion_bonfire then
	
		return;
	end
	local bonfire_map_ptr = hld.bonfire_map_cs:cast(map_object_ptr);
	if bonfire_map_ptr == nil then
		return;
	end
	bonfire_map_ptr:goto_next_stage();
end


function legion:invite_player(player_ref, msg_data)

   delog(msg_data)

   local invite_player_ref = nil;
   if msg_data.invite_guid ~= nil or msg_data.invite_guid ~= 0 then
   	  local guid_ptr = hld.get_guid_64(msg_data.invite_guid)
   	  invite_player_ptr = hld.unit_man():get_player(guid_ptr);
   elseif msg_data.new_member_name ~= nil then
   	    local  name_len = string.len(msg_data.new_member_name);
		if name_len <= 0 or name_len > max_name_size then
			return;
		end 
	   invite_player_ptr = hld.get_player_by_name(msg_data.new_member_name)
   end

   if invite_player_ptr == nil then
   	  return
   end
   if invite_player_ptr:get_func_unlock_mgr():is_func_unlock("Legion", 0) == false then
	
		player_ref:send_notice("90095314"); --被邀请者未解锁
		return;
    end

	local msg_ptr = hld.cs2ws_invite_to_join_legion:new_local()
	msg_ptr.client_uid = player_ref:get_client_uid();
	msg_ptr.invite_client_uid = invite_player_ptr:get_client_uid();
	player_ref:send_message_to_ws_lua(msg_ptr, msg_ptr:get_len(), 0)
end


function legion:apply_to_join_legion(player_ref, msg_data)
	delog("-----apply_to_join_legion-----")
    if  player_ref:get_func_unlock_mgr():is_func_unlock("Legion", 0) == false then
		player_ref:send_notice("90202450"); --未解锁
		return;
	end

	local msg_ptr = hld.cs2ws_apply_to_join_legion:new_local()
	msg_ptr.client_uid = player_ref:get_client_uid();
	msg_ptr.legion_guid = hld.get_guid_64(msg_data.legion_guid);
	player_ref:send_message_to_ws_lua(msg_ptr, msg_ptr:get_len(), 0)
end


function legion:c2cs_add_damage_buff_in_legion_boss(player_ref, msg_data)
	 local map_object_ptr = hld.world_cs:get_instance():get_map_by_guid(player_ref:get_map_guid());
	if map_object_ptr == nil then
		
		return;
	end
	local  legion_map_cs_ptr = hld.legion_station_map_cs:cast(map_object_ptr);
	if legion_map_cs_ptr == nil then
		return;
	end
	local add_time_all = legion_map_cs_ptr:add_legion_boss_damage(player_ref:get_unit_guid(), msg_data.add_count);
	if add_time_all < 0 then
		return;
	end
	local n_demon_count = legion_map_cs_ptr:get_player_demon_count(player_ref:get_unit_guid());
	local n_gold_count = legion_map_cs_ptr:get_player_gold_count(player_ref:get_unit_guid());

	local t_ret_msg = {
	   add_count = add_time_all,
	   demon_count = n_demon_count,
	   gold_count = n_gold_count,
    }
    MessageMgr:send_message_to_self(player_ref, "hld.game_proto.add_legion_boss_buff_end",t_ret_msg, 15402)
end

g_legion = legion

return legion

