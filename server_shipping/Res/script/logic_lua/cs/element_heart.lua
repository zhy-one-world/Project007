local element_heart={}
local function_array ={}
function element_heart:init()
	function_array[2]= self.element_heart_upgrade
	function_array[3]= self.element_heart_equip_on
	function_array[8]= self.element_heart_recovery
	function_array[14]= self.element_merge
	function_array[15]= self.core_element_unlock
	function_array[16]= self.core_element_upgrade
end
define_C[11304] = function(message_data, player_ref)
	local msg_data = decode("hld.element_heart_proto.element_heart_operate", message_data)
	element_heart:element_heart_operate(player_ref, msg_data.operation_type, msg_data.element_heart_guid, msg_data.target_equip_slot, msg_data.use_up_item);
end
function element_heart:element_heart_operate(player_ref, operation_type, item_guid_array, target_equip_slot, use_up_item)
	if nil == player_ref then
		print("element_heart:element_heart_operate player_ref is null")
		return;
	end
	if nil == function_array[operation_type] then
		print("element_heart:element_heart_operate function_array is null operation_type:"..operation_type)
		return;
	end
	local operate_end_msg = {};
	operate_end_msg.old_drop_group = player_ref:get_unit_info(hld.e_role_info_element_heart_drop_group)
	operate_end_msg.highest_group = 0
	operate_end_msg.is_raise = 0
	operate_end_msg.operation_type = operation_type
	local end_state = function_array[operation_type](self, player_ref, item_guid_array, target_equip_slot, use_up_item);
	operate_end_msg.operation_end_state = end_state;
	--operate_end_msg.target_guid = 0;
	operate_end_msg.new_drop_group =  player_ref:get_unit_info(hld.e_role_info_element_heart_drop_group)
	MessageMgr:send_message_to_self(player_ref, "hld.element_heart_proto.element_heart_operate_end",operate_end_msg, 11303)
end
function element_heart:element_heart_upgrade(player_ref, item_guid_array)
	if #item_guid_array < 2 then
		print("element_heart:element_heart_upgrade param is error")
		return ConstDefine.e_item_string_unkown;
	end
	local item_set = player_ref:get_item_set();
	local element_heart_ptr = item_set:get_item(item_guid_array[1],item_guid_array[2]);
	if nil == element_heart_ptr then
		print("element_heart:element_heart_upgrade element_heart_ptr is error")
		return ConstDefine.e_item_string_unkown;
	end
	local is_equiping = element_heart_ptr:get_data_info(hld.e_item_info_container_type) == hld.e_bag_type_equip_elemenet;
	local core_forth_item = item_set:get_item_by_slot(hld.e_bag_type_core_element, 3);
	if nil == core_forth_item then
		print("element_heart:element_heart_upgrade core_forth_item is error")
		return ConstDefine.e_item_string_unkown;
	end
	local element_heart_template_ptr = self:get_element_heart_template(core_forth_item);
	if nil == element_heart_template_ptr then
		print("element_heart:element_heart_upgrade element_heart_template_ptr is error")
		return ConstDefine.e_item_string_unkown;
	end
	local extend_level = 0;
	if core_forth_item:get_data_info(hld.e_item_info_activate) > 0 then
		extend_level = (core_forth_item:get_data_info(hld.e_item_info_upgrade_count) + 1) * element_heart_template_ptr.UpgradeLevel;
	end
	local cur_upgrade_num = element_heart_ptr:get_data_info(hld.e_item_info_upgrade_count);
	local rune_template_ptr = self:get_element_heart_template(element_heart_ptr);
	if  nil == rune_template_ptr then
		print("element_heart:element_heart_upgrade rune_template_ptr is error")
		return ConstDefine.e_item_string_unkown;
	end
	if cur_upgrade_num >= rune_template_ptr.MaxUpgradeLevel + extend_level then
		print("element_heart:element_heart_upgrade cur_upgrade_num is error cur_upgrade_num:"..cur_upgrade_num.." MaxUpgradeLevel:"..rune_template_ptr.MaxUpgradeLevel.." extend_level:"..extend_level)
		return ConstDefine.e_item_string_full_level;
	end

	local rune_upgrade_template_ptr = template_manager:get_line("ElementHeartUpgradeTemplate", rune_template_ptr.UpgradeId + cur_upgrade_num);
	if nil == rune_upgrade_template_ptr then
		print("element_heart:element_heart_upgrade rune_upgrade_template_ptr is error UpgradeId:"..rune_template_ptr.UpgradeId.." cur_upgrade_num:"..cur_upgrade_num)
		return ConstDefine.e_item_string_unkown;
	end
	local updrade_cost = rune_upgrade_template_ptr.UpgradeCost
	local upgrade_cost_num = #(updrade_cost);
	if math.fmod(upgrade_cost_num, 2) ~= 0 then
		print("element_heart:element_heart_upgrade upgrade_cost_num is error")
		return ConstDefine.e_item_string_unkown;
	end

	local cost_money_num = upgrade_cost_num / 2;
	for i = 0, cost_money_num - 1 do
		local money_id = tonumber(updrade_cost[i * 2 + 1]);
		local money_value = tonumber(updrade_cost[i * 2 + 2]);
		if (player_ref:can_cut_money(money_id, money_value) == false) then
			print("element_heart:element_heart_upgrade money is error money_id:"..money_id.." money_value:"..money_value)
			return ConstDefine.e_item_string_money;
		end
	end

	for i = 0, cost_money_num - 1 do
		local money_id = tonumber(updrade_cost[i * 2 + 1]);
		local money_value = tonumber(updrade_cost[i * 2 + 2]);
		player_ref:cut_money(money_id, money_value, hld.e_server_log_cut_money_element_heart_upgrade, element_heart_ptr:get_item_template_id(), 0);
	end

	if (is_equiping) then
		self:set_element_heart_one_att(player_ref, element_heart_ptr, false);
	end

	element_heart_ptr:set_data_info(hld.e_item_info_upgrade_count, cur_upgrade_num + 1);

	if (is_equiping) then
		self:set_element_heart_one_att(player_ref, element_heart_ptr, true);
		player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_total_element_heart_equip_level, 0, 0);
		player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_rune_total_level, 0, 0);
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_rune_total_level, 0, 0);
		player_ref:get_element_heart_mgr():refresh_rune_buff();
		player_ref:get_element_heart_mgr():sync_rune_ranking_data();
	end

	self:send_one_element_heart(player_ref,element_heart_ptr);

	if (rune_upgrade_template_ptr.Notice_id > 0) then
		local notice_data = {};
		notice_data.notice_id = rune_upgrade_template_ptr.Notice_id
		notice_data.item_data = {}
		notice_data.num_type_data = {}
		table.insert(notice_data.item_data, element_heart_ptr:get_item_inst());
		table.insert(notice_data.num_type_data,element_heart_ptr:get_data_info(hld.e_item_info_upgrade_count) + 1);
		server_tool_mgr:send_notice(player_ref, notice_data);
	end

	local temp_active_degree_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_upgrade_one_element_heart);
	player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_upgrade_one_element_heart, temp_active_degree_value + 1);
	player_ref:get_achievement_mgr():player_changed(hld.eat_runc_level_count);
	player_ref:refresh_service_goal(hld.e_service_goal_type_element_heart_total_level);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_element_power_up, 0);
	return ConstDefine.e_item_string_succeed
end
function element_heart:element_heart_equip_on(player_ref, item_guid_array)
	if #item_guid_array < 2 then
		print("element_heart:element_heart_equip_on param is error")
		return ConstDefine.e_item_string_unkown;
	end
	if (player_ref == nil) then
		print("element_heart:element_heart_equip_on player is error")
		return ConstDefine.e_item_string_unkown;
	end

	local main_element_heart = player_ref:get_item_set():get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == main_element_heart) then
		print("element_heart:element_heart_equip_on main_element_heart is error")
		return ConstDefine.e_item_string_unkown;
	end
	local element_item_template_ptr = main_element_heart:get_item_info_ptr();
	if (element_item_template_ptr == nil) or (element_item_template_ptr.item_type ~= e_item_type_element_heart) then
		print("element_heart:element_heart_equip_on element_item_template_ptr is error")
		return ConstDefine.e_item_string_unkown;
	end

	local old_element_heart_go_to_slot = main_element_heart:get_data_info(hld.e_item_info_slot);
	local main_ptr = self:get_element_heart_template(main_element_heart);
	if (main_ptr == nil) then
		print("element_heart:element_heart_equip_on main_ptr is error")
		return ConstDefine.e_item_string_unkown;
	end
	local target_operate_slot = main_ptr.AttType

	if (main_element_heart:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_bag) then
		print("element_heart:element_heart_equip_on bag is error bag_type:"..main_element_heart:get_data_info(hld.e_item_info_container_type))
		return ConstDefine.e_item_string_unkown;
	end

	local cur_equip_slot_open_num = player_ref:get_unit_info(hld.e_role_info_element_heart_slot_open_num);
	if (cur_equip_slot_open_num > ConstDefine.element_heart_equip_num) then
		cur_equip_slot_open_num = ConstDefine.element_heart_equip_num;
	end

	if (target_operate_slot < 0 or target_operate_slot > cur_equip_slot_open_num) then
		print("element_heart:element_heart_equip_on target_operate_slot is error target_operate_slot:"..target_operate_slot.." cur_equip_slot_open_num:"..cur_equip_slot_open_num)
		return ConstDefine.e_element_heart_equip_full;
	end
	
	local old_element_heart = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_equip_elemenet, target_operate_slot);

	if (old_element_heart ~= nil) then
		self:set_element_heart_one_att(player_ref, old_element_heart, false);
	end
	
	if (old_element_heart ~= nil) then
		local old_element_heart_go_to_slot_main = main_element_heart:get_data_info(hld.e_item_info_slot);

		main_element_heart:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_equip_elemenet);
		main_element_heart:set_data_info(hld.e_item_info_slot, target_operate_slot);
		player_ref:get_item_set():set_target_slot_guid(hld.e_bag_type_equip_elemenet, target_operate_slot, main_element_heart:get_item_guid());
		self:send_one_element_heart(player_ref, main_element_heart);

		old_element_heart:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_elemenet);
		old_element_heart:set_data_info(hld.e_item_info_slot, old_element_heart_go_to_slot_main);
		player_ref:get_item_set():set_target_slot_guid(hld.e_bag_type_elemenet, old_element_heart_go_to_slot_main, old_element_heart:get_item_guid());
		self:send_one_element_heart(player_ref, old_element_heart);
	else
		main_element_heart:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_equip_elemenet);
		player_ref:get_item_set():clear_target_slot(hld.e_bag_type_elemenet, main_element_heart:get_data_info(hld.e_item_info_slot));
		main_element_heart:set_data_info(hld.e_item_info_slot, target_operate_slot);
		player_ref:get_item_set():set_target_slot_guid(hld.e_bag_type_equip_elemenet, target_operate_slot, main_element_heart:get_item_guid());
		self:send_one_element_heart(player_ref, main_element_heart);
	end
	
	self:set_element_heart_one_att(player_ref, main_element_heart, true);
	player_ref:get_item_set():tidy_item(hld.e_bag_type_elemenet);
	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_total_element_heart_equip_level,0,0);
	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_rune_total_level,0,0);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_rune_equip, 0, 0);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_rune_total_level, 0, 0);
	player_ref:get_achievement_mgr():player_changed(hld.eat_run_equip_num_count);
	player_ref:get_achievement_mgr():player_changed(hld.eat_runc_level_count);
	player_ref:get_element_heart_mgr():refresh_rune_buff();
	player_ref:get_element_heart_mgr():sync_rune_ranking_data();
	player_ref:refresh_service_goal(hld.e_service_goal_type_element_heart_total_level);
	player_ref:refresh_service_goal(hld.e_service_goal_type_equiped_element_heart_num);
	return ConstDefine.e_item_string_succeed;
end
function element_heart:element_heart_recovery(player_ref, item_guid_array, target_equip_slot, use_up_item)
	print("element_heart:element_heart_recovery begin")
	if (player_ref == nil) then
		print("element_heart:element_heart_recovery player_ref is null")
		return ConstDefine.e_item_string_unkown;
	end
	local processed_map = {};
	local money_reward_array = {};
	local del_array = {};

	local cost_money_id = 0;
	local cost_money_num = 0;
	local add_percent = 1.0;
	local guid_size = #item_guid_array;
	for i = 1, guid_size,2 do
		local guid_str = ""..item_guid_array[i]..item_guid_array[i+1];
		if (nil ~= processed_map[guid_str]) then
			print("element_heart:element_heart_recovery guid_str repeate")
			return ConstDefine.e_item_string_unkown;
		end

		local element_heart_ptr = player_ref:get_item_set():get_item(item_guid_array[i], item_guid_array[i+1]);
		if (element_heart_ptr == nil) then
			print("element_heart:element_heart_recovery element_heart_ptr is null")
			return ConstDefine.e_item_string_unkown;
		end
		local item_template_ptr = element_heart_ptr:get_item_info_ptr();
		if (nil == item_template_ptr)then
			print("element_heart:element_heart_recovery item_template_ptr is null")
			return ConstDefine.e_item_string_unkown;
		end
		local item_id = element_heart_ptr:get_data_info(hld.e_item_info_info_id);
		local item_type = template_manager:get_field("ItemTemplate", tonumber(item_id), "sub_type");
		
		if (item_type == hld.e_element_heart_sub_type_exp) then
			add_percent = 1 + self:get_core_first_addition(player_ref);
		end

		processed_map[guid_str] = 1;

		local recover_template_ptr = self:get_element_heart_upgrade_tempalte(element_heart_ptr);
		if nil == recover_template_ptr then
			print("element_heart:element_heart_recovery recover_template_ptr is null")
			return ConstDefine.e_item_string_unkown;
		end
		
		local recovery_reward = recover_template_ptr.RecoveryReward
		local recovery_reward_num = #(recovery_reward);
		if math.fmod(recovery_reward_num, 2) ~= 0 then
			print("element_heart:element_heart_recovery recovery_reward_num is error recovery_reward_num:"..recovery_reward_num)
			return ConstDefine.e_item_string_unkown;
		end
		
		local diamond_recovery_money = recover_template_ptr.DiamondRecoveryMoney
		local diamond_recovery_money_num = #(diamond_recovery_money);
		
		local diamond_recovery_cost = recover_template_ptr.DiamondRecoveryCost
		local diamond_recovery_cost_num = #(diamond_recovery_cost);

		if (use_up_item > 0 and diamond_recovery_cost_num >= 2) then
			cost_money_id = tonumber(diamond_recovery_cost[1]);
			cost_money_num = cost_money_num + tonumber(diamond_recovery_cost[2]);
			if diamond_recovery_money_num >= 2 then
				local money_id = tonumber(diamond_recovery_money[1]);
				local money_num = tonumber(diamond_recovery_money[2]) * add_percent;
				
				server_tool_mgr:merge_money_to_array(money_reward_array, money_id, money_num);
			end
		else
			for reward_index = 1,  recovery_reward_num, 2 do
				local temp_money_id = tonumber(recovery_reward[reward_index]);
				local temp_money_value = tonumber(recovery_reward[reward_index + 1]) * add_percent;
				server_tool_mgr:merge_money_to_array(money_reward_array, temp_money_id, temp_money_value);
			end
		end
		table.insert(del_array, element_heart_ptr);
	end

	if (use_up_item > 0 and false == player_ref:can_cut_money(cost_money_id, cost_money_num)) then
		print("element_heart:element_heart_recovery money not enough cost_money_id:"..cost_money_id.." cost_money_num:"..cost_money_num)
		return ConstDefine.e_item_string_jewel;
	end

	if (use_up_item > 0 and cost_money_num > 0) then
		player_ref:cut_money(cost_money_id, cost_money_num, hld.e_server_log_cut_money_element_diamond_recovery, 0, 0);
	end

	for k,v in pairs(money_reward_array) do
		local money_id = k;
		local money_value = v;
		player_ref:add_money_or_exp(money_id, money_value, hld.e_server_log_add_money_element_heart_recovery, 0);
	end
	for k,v in pairs(del_array) do
		player_ref:get_item_set():del_item(hld.e_server_log_del_item_item_sell, 0, v, v:get_data_info(hld.e_item_info_stack_count));
	end

	print("element_heart:element_heart_recovery end")
	return ConstDefine.e_item_string_succeed;
end
function element_heart:element_merge(player_ref, item_guid_array)

	local merge_result = 90029001;
	local item_set_ref = player_ref:get_item_set();
	local first_element_heart = item_set_ref:get_item(item_guid_array[1], item_guid_array[2]);
	local second_element_heart = item_set_ref:get_item(item_guid_array[3], item_guid_array[4]);
	if nil == first_element_heart or nil == second_element_heart then
		return merge_result;
	end
	
	local del_item_list = {}
	
	table.insert(del_item_list,first_element_heart)
	table.insert(del_item_list,second_element_heart)
	local item_id = first_element_heart:get_item_template_id();
	local element_id =  template_manager:get_field("ItemTemplate", item_id, "logic_id")
	local merge_id =  template_manager:get_field("ElementHeartTemplate", element_id, "MergeId")
	local merge_cost =  template_manager:get_field("ElementHeartTemplate", merge_id,"MergeCost");
	local merge_item_id =  template_manager:get_field("ElementHeartTemplate", merge_id,"ItemTemplateId");
	if #merge_cost >= 6 then
		local element_num = item_set_ref:count_item_by_template_id(merge_cost[5],hld.e_bag_type_bag,-1)
		if element_num < tonumber(merge_cost[6]) then
			return merge_result
		end
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_element_merge,0,tonumber(merge_cost[5]),tonumber(merge_cost[6]),1);
	end
	local merge_level = self:get_element_merge_level(player_ref,del_item_list,merge_id)
	player_ref:get_element_heart_mgr():put_merge_item_into_bag(merge_item_id,merge_level);
	return 90029002
end

function element_heart:core_element_unlock(player_ref, item_guid_array)
	local item_set_ref = player_ref:get_item_set();
	local core_item = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	if core_item == nil then
		return 90029001
	end	 
	local container_type = core_item:get_data_info(hld.e_item_info_container_type);
	local is_unlock = core_item:get_data_info(hld.e_item_info_activate);
	if container_type ~= tonumber(hld.e_bag_type_core_element) or is_unlock == 1 then
		return 90029001
	end
	local core_item_id = core_item:get_data_info(hld.e_item_info_info_id);
	local core_element_id =  template_manager:get_field("ItemTemplate", core_item_id, "logic_id")
	local unlock_level =  template_manager:get_field("ElementHeartTemplate", tonumber(core_element_id),"UnlockLevel");
	local unlock_cost_arr = template_manager:get_field("ElementHeartTemplate", tonumber(core_element_id),"UnlockCost");
	local cur_total_level = self:get_double_att_element_total_level(player_ref)
	if #unlock_cost_arr < 2 or cur_total_level < unlock_level then
		return 90029001
	end
	local has_num = item_set_ref:count_item_by_template_id(unlock_cost_arr[1],hld.e_bag_type_bag,-1)
	if has_num < tonumber(unlock_cost_arr[2]) then
		return 90029001
	end
	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_element_unlock,0,tonumber(unlock_cost_arr[1]),tonumber(unlock_cost_arr[2]),1);
	self:core_element_operate(player_ref, core_item,0)
	item_set_ref:send_item_one(core_item,true,0)
	return 90029002
end

function element_heart:core_element_upgrade(player_ref, item_guid_array)
	local item_set_ref = player_ref:get_item_set();
	local core_item = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	local container_type = core_item:get_data_info(hld.e_item_info_container_type);
	local is_unlock = core_item:get_data_info(hld.e_item_info_activate);
	if container_type ~= tonumber(hld.e_bag_type_core_element) or is_unlock == 0 then
		return 90029001
	end
	local cur_exp = core_item:get_data_info(hld.e_item_info_wing_exp)
	local cur_level = core_item:get_data_info(hld.e_item_info_upgrade_count)
	local core_item_id = core_item:get_data_info(hld.e_item_info_info_id);
	local core_element_id =  template_manager:get_field("ItemTemplate", core_item_id, "logic_id")
	local Max_level =  template_manager:get_field("ElementHeartTemplate", tonumber(core_element_id),"MaxUpgradeLevel");
	local upgrade_id = template_manager:get_field("ElementHeartTemplate", tonumber(core_element_id),"UpgradeId");
	local upgrade_cost_arr =  template_manager:get_field("ElementHeartUpgradeTemplate", tonumber(upgrade_id) + cur_level,"UpgradeCost");
	local upgrade_need_num = template_manager:get_field("ElementHeartUpgradeTemplate", tonumber(upgrade_id) + cur_level,"UpgradeNeedNum");
	if #upgrade_cost_arr < 2 or cur_level >= Max_level then
		return 90029001
	end	
	local has_num = item_set_ref:count_item_by_template_id(upgrade_cost_arr[1],hld.e_bag_type_bag,-1)
	if has_num < tonumber(upgrade_cost_arr[2]) then
		return 90029001
	end
	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_element_unlock,0,upgrade_cost_arr[1],upgrade_cost_arr[2],1);
	if cur_exp + tonumber(upgrade_cost_arr[2]) >=  tonumber(upgrade_need_num) then
		core_item:set_data_info(hld.e_item_info_wing_exp,cur_exp + tonumber(upgrade_cost_arr[2]) - tonumber(upgrade_need_num))
		self:core_element_operate(player_ref, core_item,1)
	else
		core_item:set_data_info(hld.e_item_info_wing_exp,cur_exp + tonumber(upgrade_cost_arr[2]))
	end
	item_set_ref:send_item_one(core_item,true,0)
	return 90029002
end

function element_heart:get_double_att_element_total_level(player_ref)
	local item_set_ref = player_ref:get_item_set();
	local total_level = 0;
	local core_item_second = item_set_ref:get_item_by_slot(hld.e_bag_type_core_element,1)
	if core_item_second ~= nil then
		local is_unlock = core_item_second:get_data_info(hld.e_item_info_activate);
		local item_id = core_item_second:get_data_info(hld.e_item_info_info_id);
		local element_id =  template_manager:get_field("ItemTemplate", item_id, "logic_id")
		local upgrade_level =  template_manager:get_field("ElementHeartTemplate", tonumber(element_id), "UpgradeLevel")
		local addition_level = 0;
		if is_unlock == 1 then
			addition_level = (core_item_second:get_data_info(hld.e_item_info_upgrade_count) + 1) * tonumber(upgrade_level);
		end
		for i = 0,9 do
			local temp_item = item_set_ref:get_item_by_slot(hld.e_bag_type_equip_elemenet,i)
			if temp_item ~= nil then
				local temp_item_id = temp_item:get_data_info(hld.e_item_info_info_id)
				local temp_logic_id = template_manager:get_field("ItemTemplate", temp_item_id, "logic_id")
				local temp_type =  template_manager:get_field("ElementHeartTemplate", tonumber(temp_logic_id),"Type");
				if tonumber(temp_type) == 1 then
					local cur_level = temp_item:get_data_info(hld.e_item_info_upgrade_count);
					total_level = total_level + cur_level + 1 + addition_level;
				end
			end
		end
	end
	return total_level
end
function element_heart:core_element_operate(player_ref, core_item,operate_index)
	local item_set_ref = player_ref:get_item_set();
	local item_id = core_item:get_data_info(hld.e_item_info_info_id);
	local element_id =  template_manager:get_field("ItemTemplate", item_id, "logic_id")
	local core_type =  template_manager:get_field("ElementHeartTemplate", element_id,"Type");
	
	if tonumber(core_type) == 0 or tonumber(core_type) == 3 then
		if operate_index == 0 then
			core_item:set_data_info(hld.e_item_info_activate,1);
		else
			local cur_level = core_item:get_data_info(hld.e_item_info_upgrade_count)
			core_item:set_data_info(hld.e_item_info_upgrade_count,cur_level+1);
		end
	end
		
	if tonumber(core_type) == 1 or tonumber(core_type) == 2 then
		player_ref:get_element_heart_mgr():set_element_heart_all_att(false)
		if operate_index == 0 then
			core_item:set_data_info(hld.e_item_info_activate,1);
		else
			local cur_level = core_item:get_data_info(hld.e_item_info_upgrade_count)
			core_item:set_data_info(hld.e_item_info_upgrade_count,cur_level+1);
		end
		player_ref:get_element_heart_mgr():set_element_heart_all_att(true)
		player_ref:get_element_heart_mgr():sync_rune_ranking_data();
	end
end
function element_heart:get_element_merge_level(player_ref,del_item_list,merge_id)
	local level = 0;
	local total_recovery = 0;
	local item_set_ref = player_ref:get_item_set();
	for k,v in pairs(del_item_list) do
		local item_id = v:get_data_info(hld.e_item_info_info_id);
		local upgrade_count = v:get_data_info(hld.e_item_info_upgrade_count);
		local element_id =  template_manager:get_field("ItemTemplate", item_id, "logic_id")
		local upgrade_id =  template_manager:get_field("ElementHeartTemplate", tonumber(element_id), "UpgradeId")
		local recovery_money = {};
		if upgrade_count > 0 then 
			recovery_money =  template_manager:get_field("ElementHeartUpgradeTemplate", tonumber(upgrade_id) + upgrade_count,"DiamondRecoveryMoney");
		else
			recovery_money =  {0,0};
		end
		total_recovery = total_recovery +tonumber(recovery_money[2]);
		local container_type = v:get_data_info(hld.e_item_info_container_type);
		if  container_type == 10 then
			player_ref:get_element_heart_mgr():set_element_heart_one_att(v,false);
		end
		item_set_ref:del_item(hld.e_server_log_del_item_element_merge,0,v,1);
	end
	local upgrade_template_id =  template_manager:get_field("ElementHeartTemplate", tonumber(merge_id),"UpgradeId");
	local upgrade_max =  template_manager:get_field("ElementHeartTemplate", merge_id,"MaxUpgradeLevel");
	local core_element_forth = item_set_ref:get_item_by_slot(hld.e_bag_type_core_element,3)
	local extend_level = 0
	if core_element_forth ~= nil then
		local is_unlock = core_element_forth:get_data_info(hld.e_item_info_activate);
		local item_id = core_element_forth:get_data_info(hld.e_item_info_info_id);
		local element_id =  template_manager:get_field("ItemTemplate", item_id, "logic_id")
		local upgrade_level =  template_manager:get_field("ElementHeartTemplate", tonumber(element_id), "UpgradeLevel")
		if is_unlock == 0 then
			extend_level = 0
		else
			extend_level = (core_element_forth:get_data_info(hld.e_item_info_upgrade_count) + 1) * tonumber(upgrade_level);
		end
	end
	local temp_money = 0;
	for i = 0,(tonumber(upgrade_max) - 1 + extend_level) do --加上核心4的等级
		local  upgrade_cost_arr =  template_manager:get_field("ElementHeartUpgradeTemplate", tonumber(upgrade_template_id) + i,"UpgradeCost");
		temp_money = temp_money + tonumber(upgrade_cost_arr[2]);
		if total_recovery >= temp_money then
			level = i + 1;
		else
			break;
		end
	end
	return level
end
function element_heart:get_core_first_addition(player_ref)
	if (player_ref == nil) then
		return add_percent;
	end
	local add_percent = 0.0;

	local core_item_first = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_core_element, 0);
	if (nil == core_item_first)then
		return add_percent;
	end
	if (core_item_first:get_data_info(hld.e_item_info_activate) <= 0) then
		return add_percent;
	end
	--百分比 = 已穿戴双属性符文个数*核心等级*0.01
	local equip_num = player_ref:get_item_set():get_double_att_element_num();
	add_percent = (core_item_first:get_data_info(hld.e_item_info_upgrade_count) + 1)* equip_num * 0.01;
	return add_percent;
end
function element_heart:send_one_element_heart(player_ref, my_element_heart)
	if (nil == player_ref or nil == my_element_heart) then
		return;
	end
	player_ref:get_item_set():send_item_one(my_element_heart, true, hld.e_update_item_info_mode_default);
end
function element_heart:set_element_heart_one_att(player_ref, element_heart, is_add)
	if (player_ref == nil or nil == element_heart) then
		return false;
	end
	if (nil == element_heart:get_item_info_ptr()) then
		return false;
	end
	local element_heart_ptr = self:get_element_heart_template(element_heart);
	if (nil == element_heart_ptr) then
		return false;
	end
	local core_element_second = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_core_element, 1);
	local core_element_third = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_core_element, 2);
	if (nil == core_element_second or nil == core_element_third) then
		return false;
	end
	local addition_level = 0;
	local add_percent = 1 + self:get_core_third_addition(player_ref);
	local element_template_ptr = self:get_element_heart_template(core_element_second);
	if (nil == element_template_ptr)then
		return false;
	end
	if (core_element_second:get_data_info(hld.e_item_info_activate) > 0)then
		addition_level = (core_element_second:get_data_info(hld.e_item_info_upgrade_count) + 1) * element_template_ptr.UpgradeLevel;
	end
	--print("e_item_info_upgrade_count = ", element_heart:get_data_info(hld.e_item_info_upgrade_count))
	--print("addition_level = ", addition_level)
	--print("add_percent = ", add_percent)
	
	pawn_att_mgr:apply_att_change_by_array(player_ref, element_heart_ptr.BasicAtt, is_add, add_percent);
	pawn_att_mgr:apply_att_change_by_array(player_ref,element_heart_ptr.UpgradeAttEach, is_add, (element_heart:get_data_info(hld.e_item_info_upgrade_count) + addition_level) * add_percent);
	return true;
end
function element_heart:get_core_third_addition_cpp(player_index)
	local player_ref = hld.get_player_by_index(player_index)
	local add_percent = 0.0;
	if player_ref == nil then
		return add_percent;
	end
	add_percent = element_heart:get_core_third_addition(player_ref);
	return add_percent
end
function element_heart:get_core_third_addition(player_ref)
	local add_percent = 0.0;
	if (player_ref == nil) then
		return add_percent;
	end
	local core_item_third = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_core_element, 2);
	if (nil == core_item_third) then
		return add_percent;
	end
	if (core_item_third:get_data_info(hld.e_item_info_activate) <= 0) then
		return add_percent;
	end
	local element_coefficient  =  template_manager:get_field("GameConfigTemplate", 99000001, "ElementHeartComputingCoefficients")
	--百分比 = 总等级*核心等级/系数
	if tonumber(element_coefficient) <= 0 then
		return add_percent;
	end
	local total_level = self:get_equip_element_total_level(player_ref);
	add_percent = (core_item_third:get_data_info(hld.e_item_info_upgrade_count) + 1) * total_level / tonumber(element_coefficient);
	return add_percent;
end
function element_heart:get_equip_element_total_level(player_ref)
	local total_level = 0;
	local addition_level = 0;
	if (player_ref == nil) then
		return total_level;
	end
	local core_item_first = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_core_element, 1);
	if (core_item_first == nil) then
		return total_level;
	end
	local element_template_ptr = self:get_element_heart_template(core_item_first);
	if (nil == element_template_ptr) then
		return total_level;
	end
	if (core_item_first:get_data_info(hld.e_item_info_activate) > 0) then
		addition_level = (core_item_first:get_data_info(hld.e_item_info_upgrade_count) + 1) * element_template_ptr.UpgradeLevel;
	end
	for slot = 0, ConstDefine.element_heart_equip_num do
		local temp_element_heart_ptr = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_equip_elemenet, slot);
		if (temp_element_heart_ptr ~= nil) then
			local temp = temp_element_heart_ptr:get_data_info(hld.e_item_info_upgrade_count) + 1 + addition_level;
			total_level = total_level + temp;
		end
	end
	return total_level;
end
function element_heart:get_element_heart_template(element_heart)
	local item_template_ptr = server_tool_mgr:get_item_template(element_heart);
	if (nil == item_template_ptr) then
		print("element_heart:get_element_heart_template  item_template_ptr is null item_id:"..element_heart:get_item_template_id())
		return nil;
	end
	print("element_heart:get_element_heart_template logic_id:"..item_template_ptr.logic_id)
	return template_manager:get_line("ElementHeartTemplate", item_template_ptr.logic_id);
end
function element_heart:get_element_heart_upgrade_tempalte(element_heart)
	local element_heart_template = self:get_element_heart_template(element_heart);
	if (nil == element_heart_template) then
		print("element_heart:get_element_heart_upgrade_tempalte element_heart_template is null")
		return nullptr;
	end
	print("element_heart:get_element_heart_template UpgradeId:"..element_heart_template.UpgradeId.." upgrade_count:"..element_heart:get_data_info(hld.e_item_info_upgrade_count))
	return template_manager:get_line("ElementHeartUpgradeTemplate", element_heart_template.UpgradeId + element_heart:get_data_info(hld.e_item_info_upgrade_count));
end
element_heart_mgr = element_heart
return element_heart_mgr






