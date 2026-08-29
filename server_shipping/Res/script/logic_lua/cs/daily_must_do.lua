local daily_must_do = {}



function daily_must_do:init( )
	
end



define_C[12302] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.find_back_must_do_resource", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end
	delog(msg_data)

	if msg_data.only_find_back_several ~= -1 then
		daily_must_do:find_back_serveral_resource_process(player_ref, msg_data);
		--player_ref.find_back_serveral_resource_process(request.must_do_typ(), request.is_diamond_find(), request.only_find_back_several());	
	else	
		player_ref:find_back_must_do_resource_process(msg_data.must_do_typ, msg_data.is_diamond_find);
	end
end




function daily_must_do:find_back_msg_end(player_ref, in_must_do_type, in_operate_stat, in_is_diamond_find, t_money, t_item)
	  local msg_data = {
	      money_reward = {},
	      must_do_typ = in_must_do_type,
	      operate_stat = in_operate_stat,
	      is_diamond_find = in_is_diamond_find,
	      item_reward = {},
	  }
	  if t_money ~= nil then
	  	 msg_data.money_reward = t_money
	  end
	  if t_item ~= nil then
	  	msg_data.item_reward = t_item
	  end
	  MessageMgr:send_message_to_self(player_ref, "hld.character_proto.find_back_must_do_resource_end",msg_data, 12303)

end


function daily_must_do:get_find_count_by_type(player_ref, type) 
	 return player_ref:get_find_count_by_type(type)
end


function daily_must_do:get_yesterday_info(player_ref)
	    local three_ptr =   player_ref:get_find_back_info()
		local yesterday_exp_level = three_ptr.role_exp_level_yesterday;
		local yesterday_vip_level = three_ptr.role_vip_level_yesterday;
		local yesterday_arena_level = three_ptr.role_arena_rank;

		local t_yester_info = {
		   [1] = yesterday_exp_level,
		   [2] = yesterday_vip_level,
		   [3] = yesterday_arena_level,
	    }
	    return t_yester_info
end

function daily_must_do:get_find_back_template_ptr(player_ref, must_do_typ, yesterday_vip_level)
	  local template_id = player_ref:get_find_back_template_id(must_do_typ, yesterday_vip_level)

	  return template_manager:get_line("DailyActivitiesTemplate", template_id);
end



function daily_must_do:get_dyn_money(t_money_ret, template_ptr, upgrade_template_ptr, find_back_count, find_back_coefficient)
      local dynamic_money_num = #template_ptr.DynamicFindBackInfo // 2 - 1
	    for money_index = 0 , dynamic_money_num  do
	    	local money_typ = template_ptr.DynamicFindBackInfo[money_index * 2 + 1];
			local money_coefficient = template_ptr.DynamicFindBackInfo[money_index * 2 + 2];
			local money_num = 0;

			if  money_typ == hld.e_money_type_exp then
				money_num = money_coefficient * upgrade_template_ptr.BaseExp;		
			elseif money_typ == hld.e_money_type_silver_bind then
				money_num = money_coefficient * upgrade_template_ptr.BaseCoin;
			end
			local real_money_num =  money_num * find_back_count * find_back_coefficient;

		    table.insert(t_money_ret, money_typ)
		    table.insert(t_money_ret, real_money_num)
	   end
	   	delog("---t_BackInfo--111111----")
		delog(template_ptr.DynamicFindBackInfo)

end


function daily_must_do:get_static_money(t_money_ret, t_BackInfo, find_back_count, find_back_coefficient)
		local  static_money_num = #t_BackInfo // 2 - 1;
		for money_index = 0, static_money_num do
			local money_typ = t_BackInfo[money_index * 2 + 1];
			local money_num = t_BackInfo[money_index * 2 + 2];
			local real_money_num = money_num * find_back_count * find_back_coefficient;
			table.insert(t_money_ret, money_typ)
		    table.insert(t_money_ret, real_money_num)	
		end
		delog("---t_BackInfo----")
		delog(t_BackInfo)
end



function daily_must_do:get_item_id_num(t_item_ret, t_BackArray, find_back_count, find_back_coefficient)
       local  item_find_back_num = #t_BackArray // 2 - 1
	   for item_index = 0 , item_find_back_num do
	   	   repeat
		   	    local  item_id = t_BackArray[item_index * 2 + 1];
				local  item_num = t_BackArray[item_index * 2 + 2] * find_back_count * find_back_coefficient;
				item_num = math.floor(item_num)

				local temp_template_ptr = template_manager:get_line("ItemTemplate", item_id);
				if temp_template_ptr == nil then
					break;
				end
				if item_num <= temp_template_ptr.max_pile_num then
					table.insert(t_item_ret, item_id)
					table.insert(t_item_ret, item_num)
					break
				end
				local max_pile_num = 0;
				if temp_template_ptr.max_pile_num <= 0 then
					max_pile_num = 1;
				else
					max_pile_num = temp_template_ptr.max_pile_num;
				end
				local item_piles_count = item_num // max_pile_num;
				if item_num % max_pile_num ~= 0 then
					item_piles_count = item_piles_count + 1;
				end
				for i= 1,item_piles_count do
					local enter_count = 0
					if item_num > max_pile_num then
			            enter_count = max_pile_num
						item_num = item_num - max_pile_num;
					else
					    enter_count = item_num
					end	
					table.insert(t_item_ret, item_id)
					table.insert(t_item_ret, enter_count)
				end
		   until true
	   end	
end


function daily_must_do:find_back_serveral_resource_process(player_ref, msg_data)
	 local must_do_typ = msg_data.must_do_typ
	 local is_diamond_find = msg_data.is_diamond_find
	 local find_back_count = msg_data.only_find_back_several
	 if must_do_typ < 0 and must_do_typ >= hld.e_daily_must_do_typ_max then
		return;
	 end
	 local  final_find_back_count = self:get_find_count_by_type(player_ref,must_do_typ);

	 if find_back_count <= 0 or find_back_count > final_find_back_count then	
		return;
	 end

	 -----------------------------------------------------------------------------

	 local game_config_template_ptr = server_tool_mgr:get_game_config_template_ptr();

     if game_config_template_ptr == nullptr then
     	self:find_back_msg_end(player_ref, must_do_typ, ConstDefine.e_item_string_unkown,is_diamond_find)
		return;
	 end
	 local  find_back_coefficient = 0;
	 if is_diamond_find then
		 find_back_coefficient = game_config_template_ptr.DiamondFindBackPercentage / 100.0;
     else
		
		 find_back_coefficient = game_config_template_ptr.SiliverFindBackPercentage / 100.0;
	 end

	 -------------------------------------------------------------------------------------------
	 local t_yester_info = self:get_yesterday_info(player_ref)
	 local yesterday_exp_level = t_yester_info[1]
	 local yesterday_vip_level = t_yester_info[2]
	 local yesterday_arena_level = t_yester_info[3]

	 delog(t_yester_info)
	 --------------------------------------------------------------------------------------------


	 local  upgrade_template_id = player_ref:get_unit_info(hld.e_role_info_upgrade_id);

     local  upgrade_template_ptr = template_manager:get_line("PlayerUpgradeTemplate", upgrade_template_id);
	 if upgrade_template_ptr == nil then
		self:find_back_msg_end(player_ref, must_do_typ, ConstDefine.e_item_string_unkown,is_diamond_find)
		return;
	 end
	 if upgrade_template_ptr.Level ~= yesterday_exp_level then
	 	local delta = upgrade_template_ptr.Level - yesterday_exp_level;

		upgrade_template_id = upgrade_template_id - delta;
	 end
	 upgrade_template_ptr = template_manager:get_line("PlayerUpgradeTemplate", upgrade_template_id);

	 -------------------------------------------------------------------------------------------------

	 local  template_ptr = self:get_find_back_template_ptr(player_ref, must_do_typ, yesterday_exp_level);
	 if template_ptr == nil  then
	 	return
	 end

	 if template_ptr.IsCanFindBack == 0 then
		return;
	 end

	 local  is_unlock = player_ref:get_func_unlock_mgr():is_func_unlock(template_ptr.FuncUnlockName, 0);
	 if  is_unlock == false then
		return;
	 end

     ------------------------------------------------------------------------------------------------

	 local find_back_cost = 0;
	 local find_back_cost_typ = -1;
	 if is_diamond_find then
		find_back_cost_typ = hld.e_money_type_jewel_bind;
		find_back_cost = template_ptr.DiamondBackCost * find_back_count;
	  
	 else		
		find_back_cost_typ = hld.e_money_type_silver_bind;
		find_back_cost = template_ptr.SiliverBackCost * find_back_count;
	 end
		--m_yesterday_back_type = find_back_cost_typ;
	 delog("-----find-back_cost_typ--", find_back_cost_typ, find_back_cost);
     if  not player_ref:can_cut_money(find_back_cost_typ, find_back_cost) then
		
		local ret  = is_diamond_find and ConstDefine.e_item_string_jewel or ConstDefine.e_item_string_money
		self:find_back_msg_end(player_ref, must_do_typ, ret,is_diamond_find)
		return;
	 end


	 local  item_back_Array_num = #template_ptr.ItemFindBackArray;
	 local  dynamic_back_array_num = #template_ptr.DynamicFindBackInfo;
	 local  static_back_array_num = #template_ptr.StaticFindBackInfo;
	 if item_back_Array_num % 2 ~= 0 or dynamic_back_array_num % 2 ~= 0 or static_back_array_num % 2 ~= 0 then
		 return;
	 end

	 -----------------------------------------------------------------------------------------
	local item_list = {};

	local money_list = {};


	if must_do_typ ~= hld.e_daily_must_do_typ_arena then

		self:get_dyn_money(money_list, template_ptr,  upgrade_template_ptr, find_back_count, find_back_coefficient)

		self:get_static_money(money_list, template_ptr.StaticFindBackInfo, find_back_count, find_back_coefficient)

		self:get_item_id_num(item_list, template_ptr.ItemFindBackArray, find_back_count, find_back_coefficient)
    else  
		local reward_template_ptr ,  real_template_id = arena_lua_mgr:get_reward_by_arena_pos(yesterday_arena_level)
		if reward_template_ptr == nil or #reward_template_ptr.ItemRewardArray % 2 ~= 0 or #reward_template_ptr.MoneyRewardArray % 2  ~= 0 then
			return
		end
		local gameconfig_template_ptr = server_tool_mgr:get_game_config_template_ptr();
		if gameconfig_template_ptr == nil then
			return
		end

		self:get_static_money(money_list, reward_template_ptr.MoneyRewardArray, find_back_count, find_back_coefficient)
		self:get_item_id_num(item_list, reward_template_ptr.ItemRewardArray, find_back_count, find_back_coefficient)

    end
	local money_list_size = #money_list;
	if money_list_size % 2 ~= 0 then
		return;
	end

	local item_array_size = #item_list / 2;

	if  not player_ref:get_item_set():is_empty_slot_enough(hld.e_bag_type_bag, item_array_size) then	  
		self:find_back_msg_end(player_ref, must_do_typ, ConstDefine.e_item_string_bag_full,is_diamond_find)
		return;
	end
		
	player_ref:cut_money(find_back_cost_typ, find_back_cost, hld.e_server_log_cut_money_resource_back, 0,0);

	--------------------------------------------------------------------------------------------
	local  item_tuple_array = {}

	for i= 1, #item_list, 2 do			
		local item_id = item_list[i];
		local item_num = item_list[i + 1];
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr  == nil then
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

	delog("----money_list--")
	delog(money_list)

	for i= 1, #money_list, 2 do
		local money_type = money_list[i];
		local money_num = money_list[i + 1];

		--delog("---faith--", hld.e_server_log_add_money_resource_back);

		player_ref:add_money_or_exp(money_type, money_num, hld.e_server_log_add_money_resource_back, 0);
		table.insert(item_tuple_array, money_type)
        table.insert(item_tuple_array, money_num)
        table.insert(item_tuple_array, 0)
	end

	welfare:get_item_send_promp_msg_to_client(player_ref, item_tuple_array,false)

	-------------------------------------------------------------------------------------------

	player_ref:update_find_back_count(must_do_typ, find_back_count);


	hld.server_log:resource_find_back_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), must_do_typ, find_back_cost_typ, find_back_count);


	player_ref:update_three_day_must_do_remain();

	self:find_back_msg_end(player_ref, must_do_typ, ConstDefine.e_item_string_succeed,is_diamond_find, money_list, item_list)

end

g_daily_must_do = daily_must_do

return daily_must_do




