local c2ws_msg = {}

local red_package_mgr = RequireSingleton("red_package_mgr")

local arena_mgr = RequireSingleton("arena_mgr")

local define_CS = define_C

function c2ws_msg:init()
	
end



define_CS[33011] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2ws_proto.send_red_package", message_data)
    if msg_data == nil then
    	return;
    end

    if not is_red_use_lua() then
    	hld.lua_cs2ws_send_red_package_process(conn_index, message_data, string.len(message_data))
        return
    end 


    local send_content = msg_data.send_text;
	send_content = hld.invalid_ansi_word:replace_invalid_ansi_str(send_content)

	local send_guid_ptr = hld.get_guid_64(msg_data.sender_guid);

	local add_result = red_package_mgr:add_red_package(msg_data.red_package_template_id, msg_data.sender_guid, msg_data.sender_name, send_content);
	if (add_result ~= ConstDefine.e_red_bag_send_success and
		add_result ~= ConstDefine.e_red_bag_get_globel_success and
		add_result ~= ConstDefine.e_red_bag_get_legion_success and
		add_result ~= ConstDefine.e_red_bag_get_vip_success) then
	
		local red_pack_template_ptr = template_manager:get_line("RedPackageTemplate", msg_data.red_package_template_id);
		if red_pack_template_ptr ~= nil and #red_pack_template_ptr.CostMoneyArray >= 2 then
	
			local mail_context = hld.get_str_by_string_template_id(ConstDefine.e_red_bag_refund_mail_text, 0);
			local gift_item_ptr = hld.event_ws_mgr:get_instance():get_free_item();
			local item_list_ptr = hld.event_ws_mgr:get_instance():get_free_list();
			if red_pack_template_ptr.LogicID ~= 0 then
				
				local result = hld.init_unit:init_item_data(gift_item_ptr, red_pack_template_ptr.LogicID, -1, 1, hld.e_bag_type_bag, 1, 1,0,0);
				if result == true then	
					hld.event_ws_mgr:get_instance():add_free_item(gift_item_ptr)
				end
			end
			local mail_title_name = hld.get_str_by_string_template_id(90303013, 0);

			hld.event_ws_mgr:get_instance():send_mail_to_player_by_system(send_guid_ptr, 0, mail_title_name, "REFUND", mail_context
					,red_pack_template_ptr.CostMoneyArray[1], red_pack_template_ptr.CostMoneyArray[2],0,0, item_list_ptr);

		end
	end

	local session = hld.get_client_session_by_guid(send_guid_ptr);
	if session ~= nil and session:get_cs_conn_index() >= 0 then
	
		local t_cs_data = 
		{
		  sender_guid = msg_data.sender_guid,
		  red_package_template_id = msg_data.red_package_template_id,
		  send_result = add_result
	    }
	    MessageMgr:send_to_cs(session, "hld.ws2cs_proto.send_red_package_end", t_cs_data, 33012)	
	end
end


define_CS[33013] = function ( message_data, conn_index)
	local msg_data = decode("hld.cs2ws_proto.get_red_package", message_data)
    if msg_data == nil then
    	return;
    end

    if not is_red_use_lua() then
    	hld.lua_cs2ws_get_red_package_process(conn_index, message_data, string.len(message_data))
        return
    end 

    red_package_mgr:get_red_package(msg_data.red_package_guid, msg_data.role_guid, msg_data.role_name);
end
------------------------------------------------------------------------------------------------------------
 									 --arena---
------------------------------------------------------------------------------------------------------------

define_CS[1634] = function (message_data, conn_index)
	local msg_data =  decode("hld.cs2ws_proto.req_get_arena_rank", message_data)
	if msg_data == nil then
		return
	end
	if not is_arena_use_lua() then
		hld.lua_cs2ws_get_arena_rank(conn_index, message_data, string.len(message_data))
		return
	end
	arena_mgr:req_get_player_rank(msg_data.role_guid)
end


define_CS[1636] = function (message_data, conn_index)
	 local msg_data = decode("hld.cs2ws_proto.req_challenge_msg", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if not is_arena_use_lua() then
	 	hld.lua_cs2ws_req_challenge(conn_index, message_data, string.len(message_data))
	 	return
	 end

	 arena_mgr:req_challenge(msg_data.role_guid, msg_data.target_guid, msg_data.cost_money_type, msg_data.cost_money_num, msg_data.target_cur_pos, msg_data.self_cur_pos)

end

define_CS[1638] = function (message_data, conn_index)
	 local msg_data = decode("hld.cs2ws_proto.req_choose_list", message_data)
	 if msg_data == nil then
	 	return
	 end

	 if not is_arena_use_lua() then
	 	hld.lua_cs2ws_req_get_choose_list(conn_index, message_data, string.len(message_data))
	 	return
	 end

	 arena_mgr:get_choose_list(msg_data.role_guid, msg_data.cur_pos);
end


define_CS[1640] =  function (message_data, conn_index)
	 local msg_data = decode("hld.cs2ws_proto.first_three", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if not is_arena_use_lua() then
	 	hld.lua_cs2ws_req_get_first_three(conn_index, message_data, string.len(message_data))
	 	return
	 end

	 arena_mgr:get_first_three(msg_data.role_guid);
end


local e_arena_end_type_win = 1

define_CS[1642] = function (message_data, conn_index)
      local msg_data = decode("hld.cs2ws_proto.chellenge_over", message_data)
      if msg_data == nil then
      	 return
      end

      if not is_arena_use_lua() then
      	hld.lua_cs2ws_chellenge_over_req(conn_index, message_data, string.len(message_data))
	 	return
	 end

      if msg_data.chellenge_result == e_arena_end_type_win then
		
			arena_mgr:change_rank(msg_data.role_info, msg_data.target_guid, msg_data.target_rank);
		
	  else
		
			arena_mgr:reset_challenge_state(msg_data.role_guid, msg_data.target_guid, msg_data.target_rank, msg_data.role_info.rank_pos);
	  end
end


define_CS[1644] = function (message_data, conn_index)
	  local msg_data = decode("hld.cs2ws_proto.arena_sync_gs_value", message_data)
	  if msg_data == nil then
	  	return
	  end

	  if not is_arena_use_lua() then
	  	hld.lua_cs2ws_arena_sync_gs_value_proc(conn_index, message_data, string.len(message_data))
	 	return
	  end

	  arena_mgr:sync_gs_value(msg_data.role_guid, msg_data.cur_pos, msg_data.gs_value);

end



















return c2ws_msg