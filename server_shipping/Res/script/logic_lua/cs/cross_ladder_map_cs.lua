local  cross_ladder_map_cs = {}


local e_cross_ladder_buy_ticket_ret = 
{
	success = 0,				--购买成功
	failed = 1,				--购买失败
	jewl = 2,					--钻石不足
	limit = 3,				--购买次数限制
    not_open = 4,				--活动未开放
};


local e_cross_ladder_mission_type = 
{
	join_num = 0,				--每日参与要求
	win_num = 1,				--每日胜场要求
};

local first_activity_common_template_id = 120000000;


function cross_ladder_map_cs:init()
	-- body
end


define_C[14905] = function(message_data, player_ref)     
    local msg_data = decode("hld.cross_ladder.request_buy_join_ticket", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	cross_ladder_map_cs:request_buy_join_ticket(player_ref, msg_data)
end

define_C[14908] = function(message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.req_mission_reward", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end

	cross_ladder_map_cs:req_mission_reward(player_ref, msg_data)
end


define_C[14911] = function(message_data, player_ref)
	local msg_data = decode("hld.cross_ladder.req_mission_reward", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	--if player_ref:is_self_server() == false then
	--	return;
	--end

	cross_ladder_map_cs:return_to_source_zone_proc(player_ref, msg_data)
end



function cross_ladder_map_cs:request_buy_join_ticket(player_ref, msg_data)
	
	local region_code = hld.globle_data:get_instance():get_region_code();
	if region_code == -1 then
		return;
	end

	local ret = e_cross_ladder_buy_ticket_ret.success ;
	local act_cfg_ptr = template_manager:get_line("ActivityCommonConfigTemplate", first_activity_common_template_id + hld.e_activity_type_cross_ladder);
	if act_cfg_ptr == nil then
	   self:request_buy_end_msg(player_ref, e_cross_ladder_buy_ticket_ret.failed);
	   return;
	end

	local cost_money = -1;
	local cost_money_type = -1;
	if #act_cfg_ptr.ParamIntArr5 <= 0 or #act_cfg_ptr.ParamIntArr5 % 3 ~= 0 then
		self:request_buy_end_msg(player_ref, e_cross_ladder_buy_ticket_ret.failed);
		return
	end


	for i=1,#act_cfg_ptr.ParamIntArr5, 3 do
		if act_cfg_ptr.ParamIntArr5[i] == region_code  then
			cost_money_type = act_cfg_ptr.ParamIntArr5[i + 1]
			cost_money  = act_cfg_ptr.ParamIntArr5[i + 2]
		    break
		end
	end

	if  player_ref:can_cut_money(cost_money_type, cost_money) == false then
		self:request_buy_end_msg(player_ref, e_cross_ladder_buy_ticket_ret.jewl);
		return
	end

	if player_ref:get_cross_ladder_can_buy_ticket_num() <= 0 then
		self:request_buy_end_msg(player_ref, e_cross_ladder_buy_ticket_ret.limit);
		return
	end


	player_ref:cut_money(cost_money_type, cost_money, hld.e_server_log_cut_money_cross_ladder_buy_ticket,0,0);

	local msg_ptr =  hld.cs2ws_cross_ladder_req_buy_ticket:new_local()
	msg_ptr.role_guid = player_ref:get_unit_guid();

	player_ref:send_message_to_ws_lua(msg_ptr, msg_ptr:get_len(), 0)
end

function cross_ladder_map_cs:request_buy_end_msg(player_ref, n_result)
	  local msg_data = {
	     result  = n_result,
	     --ticket_num = 0,
	     --can_buy_num = 0,
	  }
	  MessageMgr:send_message_to_self(player_ref, "hld.cross_ladder.respond_buy_join_ticket",msg_data, 14906)

end


function cross_ladder_map_cs:req_mission_reward(player_ref, msg_data)

    local act_cfg_ptr = template_manager:get_line("ActivityCommonConfigTemplate", first_activity_common_template_id + hld.e_activity_type_cross_ladder);
	if act_cfg_ptr == nil or #act_cfg_ptr.ParamIntArr4 % 6 ~= 0  then
	   return;
	end
	local max_mission_num = #act_cfg_ptr.ParamIntArr4 // 6;
	if msg_data.index >= max_mission_num then
	   return;
	end
	local mission_type	 = act_cfg_ptr.ParamIntArr4[msg_data.index * 6 + 1];
    local mission_condition  = act_cfg_ptr.ParamIntArr4[msg_data.index * 6 + 2];
    local mission_reward	 = act_cfg_ptr.ParamIntArr4[msg_data.index * 6 + 3];
    local mission_reward_num = act_cfg_ptr.ParamIntArr4[msg_data.index * 6 + 4];

    local my_cur_num = 0;
	local reward_tag = player_ref:get_logic_data(hld.e_role_logic_info_cross_ladder_reward_tag);
	if mission_type == e_cross_ladder_mission.join_num then
		my_cur_num = player_ref:get_logic_data(hld.e_role_logic_info_cross_ladder_join_num);
	else
		my_cur_num = player_ref:get_logic_data(hld.e_role_logic_info_cross_ladder_win_num);
	end

	if  my_cur_num < mission_condition then
		self:req_mission_reward_end(player_ref, msg_data.index, 0)
		return 
	end

	local is_get = reward_tag & (1 << msg_data.index)

	if is_get ~= 0 then
		self:req_mission_reward_end(player_ref, msg_data.index, 0)
		return 
	end

	player_ref:add_money_or_exp(mission_reward, mission_reward_num, hld.e_server_log_add_money_cross_ladder_task, 0);

	local t_msg = {
	    money_type_array = {},
	    money_num_array = {}
    }
    table.insert(t_msg.money_num_array, mission_reward)
    table.insert(t_msg.money_num_array, tostring(mission_reward_num))

    MessageMgr:send_message_to_self(player_ref, "hld.character_proto.character_get_money",t_msg, 10495)


    player_ref:set_logic_data(hld.e_role_logic_info_cross_ladder_reward_tag, reward_tag | 1 << msg_data.index)

    player_ref:send_logic_one(hld.e_role_logic_info_cross_ladder_reward_tag);

    self:req_mission_reward_end(player_ref, msg_data.index, 1)

end

function cross_ladder_map_cs:req_mission_reward_end(player_ref, n_index, n_result)
	local  msg_data = {
	    index = n_index,
	    result = n_result
	}
	MessageMgr:send_message_to_self(player_ref, "hld.cross_ladder.ret_mission_reward",msg_data, 14906)
end


function cross_ladder_map_cs:return_to_source_zone_proc(player_ref, msg_data)
	if (player_ref:is_self_server() == false and base_map_system:get_map_type(player_ref:get_map_guid()) == hld.e_map_type_cross_ladder) then
		local map_template_id = player_ref:get_unit_info(hld.e_role_info_main_map_id);
		local map_line_id = player_ref:get_main_line_id();
		local guid_ptr = hld.get_guid_64();
		player_ref:transfer_by_template(map_template_id, map_line_id, player_ref:get_unit_info(hld.e_role_info_server_id), guid_ptr, 0);
    end
end

g_cross_ladder_map_cs = cross_ladder_map_cs

return cross_ladder_map_cs 

