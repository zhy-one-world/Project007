

local recharge = {}
function recharge:init()
end

local  fund_enum = 
{
	e_grow_up_fund_type_reborn = 0,					--重生基金
	e_grow_up_fund_type_time  = 1,					--时光基金
	e_grow_up_fund_type_pride = 2,					--充值基金
	e_grow_up_fund_type_max   = 3,
}

define_C[13001] = function(message_data, player_ref) 
     local msg_data = decode("hld.character_proto.get_growup_fund", message_data)
     if msg_data == nil then
     	return
     end
	 recharge:get_grow_up_fund(player_ref, msg_data.fund_template_id)
end

define_C[13003] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.purchase_growup_fund", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	recharge:purchase_growup_fund(player_ref, msg_data.fund_type)
end

define_C[12501] = function (message_data, player_ref)
	local msg_data = decode("hld.character_proto.get_vip_item", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	recharge:get_vip_item_by_vip_level(player_ref, msg_data.vip_level)
end



function recharge:sendmsg(player_ref, i_fund_template_id, i_result)
	local msg_data = {
	    role_guid = { player_ref:get_unit_guid_A(), player_ref:get_unit_guid_B() },
	    fund_template_id = i_fund_template_id,
	    get_result = i_result
	}
	MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_growup_fund_end",msg_data, 13002)
end

function recharge:is_grow_up_fund_purchased(player_ref,fund_type)
	  local cur_value =  player_ref:get_unit_info(hld.e_role_info_fund_payment_flag)
	  local offset_bit = hld.init_unit:get_left_shift_operation_result(1, fund_type)
	  local result = hld.init_unit:get_bitwise_and_operation_result(cur_value, offset_bit)
	  if result ~= 0 then
	  	return true
	  end
	  return false
end


function recharge:is_grow_up_fund_geted(player_ref,fund_type, index)
	if ((fund_type ~= fund_enum.e_grow_up_fund_type_reborn) and 
	    (fund_type ~= fund_enum.e_grow_up_fund_type_time) and 
	    (fund_type ~= fund_enum.e_grow_up_fund_type_pride)) then
	    return true
	end
	local offset_bit =  hld.init_unit:get_left_shift_operation_result(1, index)
	local cur_flag = 0;
	if fund_type == fund_enum.e_grow_up_fund_type_reborn then
		cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_reborn_get_flag)
	elseif fund_type == fund_enum.e_grow_up_fund_type_time then
		cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_time_get_flag) 
	else
		cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_pride_get_flag)
	end
	local result  = hld.init_unit:get_bitwise_and_operation_result(cur_flag, offset_bit)
	if result ~= 0 then
		return true
	end
	return false
end


function recharge:get_reward(player_ref, treward,fund_template_id)
	local Num  = #treward / 2 - 1
	for i= 0,Num do
		local id =  treward[i * 2 + 1]
		local num  = treward[i * 2 + 2]
		player_ref:add_money_or_exp(id, num, hld.e_server_log_add_money_grow_up_fund, fund_template_id)
	end
end

function recharge:set_flag(player_ref,fund_type, index)
	if ((fund_type ~= fund_enum.e_grow_up_fund_type_reborn) and 
	    (fund_type ~= fund_enum.e_grow_up_fund_type_time) and 
	    (fund_type ~= fund_enum.e_grow_up_fund_type_pride)) then
	    return;
	end
	local offset_bit =  hld.init_unit:get_left_shift_operation_result(1, index)
	local cur_flag = 0;
	local result = 0
	if fund_type == fund_enum.e_grow_up_fund_type_reborn then
       cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_reborn_get_flag)
       result   = hld.init_unit:get_bitwise_or_operation_result(cur_flag, offset_bit)
       player_ref:set_unit_info(hld.e_role_info_fund_reborn_get_flag, result)
       player_ref:send_info_one(hld.e_role_info_fund_reborn_get_flag)
       return;
    end
    if fund_type == fund_enum.e_grow_up_fund_type_time then
       cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_time_get_flag)
       result   = hld.init_unit:get_bitwise_or_operation_result(cur_flag, offset_bit)
       player_ref:set_unit_info(hld.e_role_info_fund_time_get_flag, result)
       player_ref:send_info_one(hld.e_role_info_fund_time_get_flag)
       return
    end
    cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_pride_get_flag)
    result   = hld.init_unit:get_bitwise_or_operation_result(cur_flag, offset_bit)
    player_ref:set_unit_info(hld.e_role_info_fund_pride_get_flag, result)
    player_ref:send_info_one(hld.e_role_info_fund_pride_get_flag)
end


function recharge:get_grow_up_fund(player_ref, fund_template_id)
	if player_ref == nil then
		return;
	end
	local tconfig = template_manager:get_line("GrowUpFundTemplate", fund_template_id)
	if tconfig == nil then
		self:sendmsg(player_ref, fund_template_id, ConstDefine.e_item_string_unkown)
		return;
	end
	local tmoney_rwd = Common:StrToTable(tconfig.MoneyRewardArray)
	if #tmoney_rwd == 0 or #tmoney_rwd % 2 ~= 0 then
	   self:sendmsg(player_ref, fund_template_id, ConstDefine.e_item_string_unkown)
	   return;
	end
	if not self:is_grow_up_fund_purchased(player_ref, tconfig.Type) then
	   self:sendmsg(player_ref, fund_template_id, ConstDefine.e_fund_not_purchase)
	   return;
	end

	if not player_ref:is_grow_up_fund_enable(fund_template_id) then
	   self:sendmsg(player_ref, fund_template_id, ConstDefine.e_fund_cant_get)
	   return;
	end

	if self:is_grow_up_fund_geted(player_ref, tconfig.Type, tconfig.Index) then
	   self:sendmsg(player_ref, fund_template_id, ConstDefine.e_fund_already_geted)
	   return;
	end

	self:get_reward(player_ref, tmoney_rwd, fund_template_id)

	self:set_flag(player_ref, tconfig.Type, tconfig.Index)

	self:sendmsg(player_ref, fund_template_id, 0)

end

---------------------------------------------------------------------------------------------




---------------------------------------------------------------------------------------------
function recharge:purchase_send_msg_to_client(player_ref, i_fund_type, i_result)
    local  msg_data = {
	    role_guid = { player_ref:get_unit_guid_A(), player_ref:get_unit_guid_B()},
	    fund_type = i_fund_type,
	    purchase_result = i_result
	}
	MessageMgr:send_message_to_self(player_ref, "hld.character_proto.purchase_growup_fund_end",msg_data, 13004)
end


function recharge:get_cost_by_type(i_fund_type)
	 local g_config = template_manager:get_line("GameConfigTemplate",ConstDefine.game_config_template_id)
	 if g_config == nil then
	 	return 0, 0, 0
	 end
	 local s_cost = ''
	 if i_fund_type == fund_enum.e_grow_up_fund_type_pride then
	 	 s_cost = g_config.FundPrideCost
	 elseif i_fund_type == fund_enum.e_grow_up_fund_type_time then
	 	 s_cost = g_config.FundTimeCost
	 elseif i_fund_type == fund_enum.e_grow_up_fund_type_reborn then
	 	 s_cost = g_config.FundRebornCost
	 end
	 if  s_cost == '' then
	    return 0,0,0
	 end
	 local t_cost = Common:StrToTable(s_cost)
	 if #t_cost ~= 2 then
	    return 0,0,0
	 end
	 return 1, t_cost[1], t_cost[2]
end

function recharge:set_purchase_flag(player_ref, i_fund_type)
	local cur_flag = player_ref:get_unit_info(hld.e_role_info_fund_payment_flag)
	local offset_bit = hld.init_unit:get_left_shift_operation_result(1, i_fund_type)
	local result  = hld.init_unit:get_bitwise_or_operation_result(cur_flag, offset_bit)
	player_ref:set_unit_info(hld.e_role_info_fund_payment_flag, result)
	player_ref:send_info_one(hld.e_role_info_fund_payment_flag)
end


function recharge:purchase_growup_fund(player_ref, i_fund_type)
	  if i_fund_type <  fund_enum.e_grow_up_fund_type_reborn  or i_fund_type >= fund_enum.e_grow_up_fund_type_max then
	  	 return;
	  end
	  if self:is_grow_up_fund_purchased(player_ref,i_fund_type) then
	  	 self:purchase_send_msg_to_client(player_ref, i_fund_type, ConstDefine.e_fund_already_purchased)
	  	 return;
	  end
	  local i_result, i_cost_id, i_cost_value = self:get_cost_by_type(i_fund_type)
	  if i_result ~= 1 then
	  	 self:purchase_send_msg_to_client(player_ref, i_fund_type, ConstDefine.e_item_string_unkown)
	  	 return;
	  end
	  if not player_ref:can_cut_money(i_cost_id, i_cost_value)  then
	     self:purchase_send_msg_to_client(player_ref, i_fund_type, ConstDefine.e_fund_money_not_enough)
	     return;
	  end

	  player_ref:cut_money(i_cost_id, i_cost_value, hld.e_server_log_cut_money_grow_up_fund,0,0)


	  self:set_purchase_flag(player_ref, i_fund_type)

	  self:purchase_send_msg_to_client(player_ref, i_fund_type, 0)

	  hld.server_log:grow_up_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), player_ref:get_login_type(), i_fund_type, i_cost_value)

	  hld.server_log:serverGrowthFund(player_ref:get_share_log_head(), i_fund_type, i_cost_id, i_cost_value);
end



--------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------

function recharge:send_vip_level_to_client(player_ref, i_vip_level, i_result)
	 local  msg_data = {
	    role_guid = { player_ref:get_unit_guid_A(), player_ref:get_unit_guid_B()},
	    vip_level = i_vip_level,
	    get_result = i_result
	}
	MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_vip_item_end",msg_data, 12502)
end

function recharge:is_same_class_type(player_ref, s_class_type)
	 local my_class = player_ref:get_unit_info(hld.e_role_info_class_type)
	 local t_class_type = Common:StrToTable(s_class_type)
	 if #t_class_type == 0 then
	 	return true
	 end
	 for i=1,#t_class_type do
	 	if my_class == t_class_type[i] then
	 		return true
	 	end
	 end
	 return false
end


function recharge:get_vip_item_by_vip_level(player_ref,  vip_level)
	  local  cur_vip_level = player_ref:get_vip_level(false);
	  local vip_item_geted_level = player_ref:get_unit_info(hld.e_role_info_vip_item_geted_level);

	  if vip_level > cur_vip_level then	
	    	self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_vip_this_level_not_open);
			return;
	  end
	  local  vip_item_get_flag = hld.init_unit:get_left_shift_operation_result(1, vip_level);

	  if hld.init_unit:get_bitwise_and_operation_result(vip_item_geted_level, vip_item_get_flag) ~= 0 then
		   self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_vip_item_alread_get);
		   return;
	  end
		
	  local t_vip_template = template_manager:get_template_by_vip_level(vip_level);

	  if t_vip_template == nil then
	   	  self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_item_string_unkown);
	   	  return;
	  end

	  local t_item_get_array = Common:StrToTable(t_vip_template.ItemGetArray)
	  if ( #t_item_get_array % 2 ~=0  ) then
	   	  self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_item_string_unkown);
	   	  return
	  end
	  local  total_item_num = #t_item_get_array / 2;
	  local temp_item_set = player_ref:get_item_set();
	  if (not temp_item_set:is_empty_slot_enough(hld.e_bag_type_bag, total_item_num)) then
		   self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_item_string_bag_full);
		   return;
	  end
	  local t_PurchaseCostArray = Common:StrToTable(t_vip_template.PurchaseCostArray)
      if (#t_PurchaseCostArray >= 2) then	
			if (not player_ref:can_cut_money(t_PurchaseCostArray[1], t_PurchaseCostArray[2])) then
			    self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_item_string_jewel);
				return;
			end
			player_ref:cut_money(t_PurchaseCostArray[1], t_PurchaseCostArray[2], hld.e_server_log_cut_money_vip_get_item,0,0);
	  end

	   local  t_final_item_array = {};

		for i=1, #t_item_get_array, 2 do
			local i_item_id = t_item_get_array[i];
			local i_item_num = t_item_get_array[i + 1];
			local t_item_template = template_manager:get_line("ItemTemplate", i_item_id);
			if self:is_same_class_type(player_ref,t_item_template.class_type) then
               local o_item_ptr = temp_item_set:create_item_by_template(hld.e_server_log_add_item_get_vip_item, vip_level, i_item_id, i_item_num, 1,0,0,0);
               if o_item_ptr ~= nil then
               	  temp_item_set:put_item_into_bag(o_item_ptr)

               	  table.insert(t_final_item_array, i_item_id)
               	  table.insert(t_final_item_array, i_item_num)
               	  table.insert(t_final_item_array, o_item_ptr:get_data_info(hld.e_item_info_locked))
               end 
			end
		end
		welfare:get_item_send_promp_msg_to_client(player_ref, t_final_item_array,false)

		local i_result_vip_flag = hld.init_unit:get_bitwise_or_operation_result(vip_item_geted_level, vip_item_get_flag)
		player_ref:set_unit_info(hld.e_role_info_vip_item_geted_level, i_result_vip_flag);
		player_ref:send_info_one(hld.e_role_info_vip_item_geted_level);
		self:send_vip_level_to_client(player_ref, vip_level, ConstDefine.e_vip_item_get_sucess);
end




return recharge