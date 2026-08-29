local arena= {}

local arena_reward_template_start_id = 109000001

function arena:init()
	-- body
end

define_C[11901] = function(message_data, player_ref)

    local msg_data = decode("hld.arena_proto.get_arena_rank", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end

	arena:get_arena_rank(player_ref)

end

define_C[11903] = function (message_data, player_ref)
	local msg_data = decode("hld.arena_proto.get_match_player", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end

	arena:get_match_player(player_ref)
end

define_C[11905] = function (message_data, player_ref)
	local msg_data = decode("hld.arena_proto.get_top_three_player", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end

	arena:req_get_first_three_player(player_ref)
end

define_C[11907] = function (message_data, player_ref)
	local msg_data = decode("hld.arena_proto.req_challenge", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	local map_type = player_ref:get_cur_map_type()
	if map_type ~= hld.e_map_type_big_map then
		return;
	end

	arena:req_challenge(player_ref, msg_data)
end

define_C[11910] = function (message_data, player_ref)
	local msg_data = decode("hld.arena_proto.req_get_challenge_log", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	arena:get_challenge_logs(player_ref)
end

define_C[11912] = function (message_data, player_ref)
	local msg_data = decode("hld.arena_proto.get_arena_reward", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	arena:get_reward(player_ref)
end


function arena:get_arena_rank(player_ref)
	local arena_mgr = player_ref:get_arena_mgr()
	if arena_mgr == nil then
		return 
	end
	arena_mgr:get_arena_rank()
end

function arena:get_match_player(player_ref)
	local arena_mgr = player_ref:get_arena_mgr()
	if arena_mgr == nil then
		return 
	end
	arena_mgr:req_get_match_player()
end

function arena:req_get_first_three_player(player_ref)
	local arena_mgr = player_ref:get_arena_mgr()
	if arena_mgr == nil then
		return 
	end
	arena_mgr:req_get_first_three_player()
end


function arena:req_challenge_msg(player_ref, t_target_guid, n_result, n_current_rank)
     local msg_data = 
     {
        role_guid = {0,0},
        target_guid = { t_target_guid[1], t_target_guid[2]},
        req_end = n_result,
        new_pos = n_current_rank
     }
     MessageMgr:send_message_to_self(player_ref, "hld.arena_proto.req_challenge_end",msg_data, 11908)
end


function arena:req_challenge(player_ref, msg_data)




	local arena_mgr = player_ref:get_arena_mgr()
	if arena_mgr == nil then
		return 
	end

	if arena_mgr:is_requiring() then
		return
	end

	local cur_vip_level = player_ref:get_vip_level();
    local vip_template_ptr = template_manager:get_template_by_vip_level(cur_vip_level);

    local game_config_ptr = server_tool_mgr:get_game_config_template_ptr();

    if(vip_template_ptr == nil or game_config_ptr == nil or #game_config_ptr.ArenaClearChallengeCdCost < 2 or #game_config_ptr.ArenaPurchaseChallengeTimesCost < 2) then
       self:req_challenge_msg(player_ref, msg_data.target_guid, ConstDefine.e_item_string_unkown, arena_mgr:current_rank())
       return;
    end

    local is_no_cd = vip_template_ptr.IsArenaChallengeNoCD > 0;



	local money_cost_type = game_config_ptr.ArenaClearChallengeCdCost[1];
		--ArenaClearChallengeCdCost 和 ArenaClearChallengeCdCost 要花的钱ID应该是一样的，现在随便拿一个，以后改了再说
	local money_cost_value = 0;

	local colling_time = player_ref:get_time_data_sec(hld.e_time_type_arena_colling_time);


	delog("----colling_time--", colling_time);


	if ( not is_no_cd  and  colling_time > os.time() ) then
		 -- 有CD并且仍在冷却中
			if  msg_data.is_cost_money then
			 --“老子要花钱！”
				money_cost_value = money_cost_value + game_config_ptr.ArenaClearChallengeCdCost[2];
			
			else
			--讲真，应该到不了这，不花钱的话，客户端就拦下了
			    self:req_challenge_msg(player_ref, msg_data.target_guid, ConstDefine.e_arena_challenge_is_colling, arena_mgr:current_rank())	
				return;
			end
	end

	if (player_ref:get_unit_info(hld.e_role_info_arena_times) >= game_config_ptr.ArenaTimes) then
		if (player_ref:get_unit_info(hld.e_role_info_arena_vip_buy_times) < vip_template_ptr.ArenaBuyTimeLimit and msg_data.is_cost_money )then
			--能买，而且确实要买
			money_cost_value = money_cost_value + gameconfig_template_ptr.ArenaPurchaseChallengeTimesCost[2];
			
		else
			--讲真，应该到不了这，不花钱的话，客户端就拦下了
			self:req_challenge_msg(player_ref, msg_data.target_guid, ConstDefine.e_arena_challenge_times_not_enough, arena_mgr:current_rank())
			return;
		end
	end

	if (money_cost_value > 0) then
		
		if ( not player_ref:can_cut_money(money_cost_type, money_cost_value)) then
			self:req_challenge_msg(player_ref, msg_data.target_guid, ConstDefine.e_item_string_jewel, arena_mgr:current_rank())
			return;
		end
		player_ref:set_time_data(hld.e_time_type_arena_colling_time, 0);
		player_ref:send_time_one(hld.e_time_type_arena_colling_time);
	end


	local ret = arena_mgr:lua_req_challenge_to_ws(msg_data.target_guid[1], msg_data.target_guid[2], money_cost_type, money_cost_value, msg_data.target_cur_pos, msg_data.self_cur_pos)
	if ret == false then
		return false
	end
	hld.server_log:arena_challenge_log(player_ref:get_third_info(), player_ref:get_unit_info_inst());
	arena_mgr:set_is_requiring(true);
end


function arena:get_challenge_logs(player_ref)
	local arena_mgr = player_ref:get_arena_mgr()
	if arena_mgr == nil then
		return 
	end
	arena_mgr:get_challenge_logs()
end



function arena:get_size()
	local tRewardTab = template_manager:get_template("ArenaRankRewardTemplate");
	local count = 0;
	for k, v in pairs(tRewardTab) do
	   	   count = count + 1;
	end
	self.m_reward_row = count;
	return count;
end

function arena:get_reward_by_arena_pos(arena_pos)
	local row_size = self:get_size()
	local real_template_id = 0
	if arena_pos < 0 or arena_pos >= row_size then
		real_template_id = arena_reward_template_start_id + row_size - 1
	else 
		real_template_id = arena_reward_template_start_id + arena_pos
	end
	return template_manager:get_line("ArenaRankRewardTemplate", real_template_id) , real_template_id
end


function arena:get_reward_msg(player_ref, n_reward_template_id, n_result)
	local msg_data = {
	    reward_template_id = n_reward_template_id,
     	get_result = n_result
    }
    MessageMgr:send_message_to_self(player_ref, "hld.arena_proto.get_arena_reward_end",msg_data, 11913)
end

function arena:get_reward(player_ref)

	local cur_time = os.time();
	
	if (player_ref:get_time_data_sec(hld.e_time_type_arena_last_get_reward_time) > cur_time) then
		 return;
	end
	local arena_mgr = player_ref:get_arena_mgr()
	if arena_mgr == nil then
		return 
	end

	local cur_rank = arena_mgr:current_rank()

	local reward_template_ptr ,  real_template_id = self:get_reward_by_arena_pos(cur_rank)
	if reward_template_ptr == nil then
		return
	end

	local gameconfig_template_ptr = server_tool_mgr:get_game_config_template_ptr();
	if gameconfig_template_ptr == nil then
		return
	end


	local  money_reward_num = #reward_template_ptr.MoneyRewardArray// 2;

	local item_tuple_array = {}

	local item_reward_num = #reward_template_ptr.ItemRewardArray // 2;


	local empty_slot_req_num = 0;
	local element_empty_slot_req_num = 0;

	delog("------money_reward_num----", money_reward_num,item_reward_num)

	for i= 0, item_reward_num - 1  do	
	    delog("------money_reward_num---111-", money_reward_num,item_reward_num)	
		local item_id = reward_template_ptr.ItemRewardArray[i * 2 + 1];
		local item_num = reward_template_ptr.ItemRewardArray[i * 2 + 2];
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr  == nil then
            return
		end
		local max_pile_num = item_template_ptr.max_pile_num;
		if max_pile_num <= 0 then
			max_pile_num = 1;
		end
	    local slot_need_num = item_num // max_pile_num;
		if (item_num % max_pile_num ~= 0) then
			slot_need_num = slot_need_num + 1;
		end
		if item_template_ptr.item_type == hld.e_item_type_element_heart then
			element_empty_slot_req_num = element_empty_slot_req_num + slot_need_num;
		else		
			empty_slot_req_num = empty_slot_req_num + slot_need_num;
		end
	end
	delog("------money_reward_num----", money_reward_num,item_reward_num)

	if ( not player_ref:get_item_set():is_empty_slot_enough(hld.e_bag_type_bag, empty_slot_req_num) or 
	    not player_ref:get_item_set():is_empty_slot_enough(hld.e_bag_type_elemenet, element_empty_slot_req_num)) then
		self:get_reward_msg(player_ref, real_template_id,  ConstDefine.e_element_heart_bag_full)
		return;
	end

	local is_set_time = arena_mgr:lua_set_reward_time();

	if is_set_time == false then
	   return;
	end

	for i= 0, item_reward_num -1 do		
		local item_id = reward_template_ptr.ItemRewardArray[i * 2 + 1];
		local item_num = reward_template_ptr.ItemRewardArray[i * 2 + 2];
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr  == nil then
            return
		end
		local o_item_ptr = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_arena_reward, item_reward_num, item_id, item_num, 1,0,0,0);
        if o_item_ptr ~= nil then
             player_ref:get_item_set():put_item_into_bag(o_item_ptr)
             table.insert(item_tuple_array, item_id)
             table.insert(item_tuple_array, item_num)
             table.insert(item_tuple_array, o_item_ptr:get_data_info(hld.e_item_info_locked))
        end 
	end

	for i=0,money_reward_num-1 do
		local money_type = reward_template_ptr.MoneyRewardArray[i * 2 + 1];
		local money_num = reward_template_ptr.MoneyRewardArray[i * 2 + 2];

		money_num = self:get_vip_add(player_ref,money_num);

		player_ref:add_money_or_exp(money_type, money_num, hld.e_server_log_add_money_arena_rank, real_template_id);

		table.insert(item_tuple_array, money_type)
        table.insert(item_tuple_array, money_num)
        table.insert(item_tuple_array, 0)
	end

	welfare:get_item_send_promp_msg_to_client(player_ref, item_tuple_array,false)


	if (player_ref:is_need_daily_finish(hld.e_daily_must_do_typ_arena) > 0) then
			player_ref:set_must_do_count(hld.e_daily_must_do_typ_arena, player_ref:get_must_do_count(hld.e_daily_must_do_typ_arena) + 1);
			player_ref:send_daily_must_do_count_info_one(hld.e_daily_must_do_typ_arena);
			player_ref:set_daily_active_degree_info(hld.e_daily_must_do_typ_arena);
	end
	self:get_reward_msg(player_ref, real_template_id,  ConstDefine.e_item_string_succeed)
end

function arena:get_vip_add(player_ref, value)
    local cur_vip_level = player_ref:get_vip_level();
    local vip_template_ptr = template_manager:get_template_by_vip_level(cur_vip_level);
    if vip_template_ptr == nil then
       return value
    end
    if vip_template_ptr.ArenaBuyTimeLimit <= 0 then
       return value
    end

    local result = value *(100 + vip_template_ptr.ArenaBuyTimeLimit) / 100;

    return math.floor(result)
end

arena_lua_mgr = arena

return arena
