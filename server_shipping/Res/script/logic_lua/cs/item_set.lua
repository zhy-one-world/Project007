local item_set={}
local function_array ={}
function item_set:init()
	function_array[5]= self.item_upgrade
	function_array[8]= self.item_addon
	function_array[25]= self.item_succinct
	function_array[26]= self.item_succinct
	function_array[27]= self.item_save_succinct_oper
	function_array[57]= self.fashion_upgrade
	function_array[59]= self.item_succinct_n_times
	function_array[60]= self.item_succinct_n_times
	function_array[61]= self.item_enchant
	function_array[62]= self.item_unenchant
	function_array[50]= self.set_jewel_on
	function_array[66]= self.spirit_upgrade_promote
	function_array[67]= self.break_will_activate
	function_array[68]= self.awaken_fetter_activate
	function_array[69]= self.awaken_fetter_skill_upgrade
end
define_C[10701] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_operation", message_data)
	item_set:item_set_operate(player_ref, msg_data.operation_type, msg_data.item_guid, msg_data.item_slot, msg_data.use_up_item, msg_data.first_use_lock, msg_data.item_num, msg_data.force_do);
end
define_C[10739] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_forge", message_data)
	if msg_data.is_save_operate == true then
		item_set:item_save_forge(player_ref, msg_data.item_guid);
	else
		item_set:item_forge(player_ref, msg_data.item_guid, msg_data.forge_lock_flag);
	end
end
define_C[10741] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_forge", message_data)
	item_set:item_forge_begin(player_ref, msg_data.item_guid);
end
function item_set:item_set_operate(player_ref, operation_type, item_guid_array, item_slot, use_up_item, first_use_lock, item_num, force_do)
	if nil == player_ref then
		return;
	end
	if nil == function_array[operation_type] then
		return;
	end
	local operate_end_msg = {};
	operate_end_msg.role_guid = {}
	table.insert(operate_end_msg.role_guid, player_ref:get_unit_guid_A());
	table.insert(operate_end_msg.role_guid, player_ref:get_unit_guid_B());
	operate_end_msg.item_guid = item_guid_array
	operate_end_msg.item_slot = item_slot
	delog(item_guid_array)
	local end_state = function_array[operation_type](self, player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do);

	if operation_type == 59 then
		operation_type = 25
	elseif operation_type == 60 then
		operation_type = 26
	end
	operate_end_msg.operation_type = operation_type
	operate_end_msg.operation_state = end_state

	MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_operation_end",operate_end_msg, 10702)
end
function item_set:item_upgrade(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)
	if (player_ref == nil)then
		return hld.e_error_code_item_upgrade;
	end
	local item_ptr = player_ref:get_item_set():get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == item_ptr)then
		return hld.e_error_code_item_upgrade;
	end--没有物品

	if (nil == item_ptr:get_item_info_ptr())then
		return hld.e_error_code_item_upgrade;
	end
	local equip_template_ptr = template_manager:get_line("EquipTemplate", item_ptr:get_item_logic_id());
	if (nil == equip_template_ptr)then
		return hld.e_error_code_item_upgrade;
	end
	local item_upgrade_ptr = template_manager:get_line("ItemUpgradeTemplate", equip_template_ptr.UpgradeTemplateId + item_ptr:get_data_info(hld.e_item_info_upgrade_count));
	if (nil == item_upgrade_ptr)then
		return hld.e_error_code_item_upgrade;
	end
	local game_config_template_ptr = server_tool_mgr:get_game_config_template_ptr();
	if (game_config_template_ptr == nil)then
		return hld.e_error_code_item_upgrade;
	end

	local upgrade_num_now = item_ptr:get_data_info(hld.e_item_info_upgrade_count);

	--在这做个判断  判断强化的限时活动是否开启   开启了就设置开启的值
	local upgrade_max_num = 0;
	if (player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_add_upgrade_level))then
		upgrade_max_num = equip_template_ptr.limit_activity_equip_upgrade_num;
	else
		upgrade_max_num = equip_template_ptr.equip_upgrade_num;
	end
	if (upgrade_num_now >= upgrade_max_num)then--满级
		return hld.e_error_code_item_full_level;
	end
	local MoneyCostList = item_upgrade_ptr.MoneyCostList
	local MoneyCostList_num = #(MoneyCostList);

	--记录货币id和数量
	local money_type = 0;
	local money_num = 0;
	if (MoneyCostList_num >= 2)then
		money_type = tonumber(MoneyCostList[1]);
		money_num = tonumber(MoneyCostList[2]);
		for i = 1, MoneyCostList_num, 2 do --金钱判断
			if (player_ref:can_cut_money(tonumber(MoneyCostList[i]), tonumber(MoneyCostList[i + 1])) == false)then
				return hld.e_error_code_item_money;
			end
		end
	end
	local ItemIdList = item_upgrade_ptr.ItemIdList
	local ItemIdList_num = #(ItemIdList);
	local ItemNumList = item_upgrade_ptr.ItemNumList
	local ItemNumList_num = #(ItemNumList);
	for i = 1, ItemIdList_num do--材料判断
		if (player_ref:get_item_set():count_item_by_template_id(tonumber(ItemIdList[i]), hld.e_bag_type_bag, -1) < tonumber(ItemNumList[i]))then
			return hld.e_error_code_item_matirial_not_enough;
		end
	end

	--强化道具判断
	--记录使用的道具
	local protect_material_id = 0;
	local protect_material_num = 0;
	local max_down_num = hld.e_upgrade_result_max;
	local success_rate_add_value = 0;
	if (use_up_item >= 0)then
		local protect_use_state = self:upgrade_protect_item_can_use(player_ref, item_upgrade_ptr, upgrade_num_now, use_up_item);
		--dump(protect_use_state)
		if (protect_use_state ~= hld.e_error_code_success)then
			return protect_use_state;
		end
		local ProtectItemInfo = item_upgrade_ptr.ProtectItemInfo
		local ProtectItemInfo_num = #(ProtectItemInfo);
		if math.fmod(ProtectItemInfo_num, 2) ~= 0 then
			return hld.e_error_code_item_upgrade;
		end
		protect_material_id = tonumber(ProtectItemInfo[use_up_item * 2 + 1]);
		protect_material_num = tonumber(ProtectItemInfo[use_up_item * 2 + 2]);

		local protect_item_template_ptr = template_manager:get_line("ItemTemplate", protect_material_id);
		if (nil == protect_item_template_ptr)then
			return hld.e_error_code_item_upgrade;
		end
		local protect_props_template_ptr = template_manager:get_line("PropBasicAttributeTemplate", protect_item_template_ptr.logic_id);
		if (nil == protect_props_template_ptr)then
			return hld.e_error_code_item_upgrade;
		end

		local UpgradeProtectInfoArray = protect_props_template_ptr.UpgradeProtectInfoArray
		local UpgradeProtectInfoArray_num = #(UpgradeProtectInfoArray);
		if (UpgradeProtectInfoArray_num >= 2)then
			local protect_type = tonumber(UpgradeProtectInfoArray[1])
			if protect_type == hld.e_item_upgrade_protect_type_low_level_undown or protect_type == hld.e_item_upgrade_protect_type_high_level_undown then
				max_down_num = tonumber(UpgradeProtectInfoArray[2]);
			else
				success_rate_add_value = tonumber(UpgradeProtectInfoArray[2]);
			end
		end
	end
	--可以升级
	if MoneyCostList_num >= 2 then
		for i = 1, MoneyCostList_num, 2 do --花钱
			player_ref:cut_money(tonumber(MoneyCostList[i]), tonumber(MoneyCostList[i + 1]), hld.e_server_log_cut_money_item_upgrade, item_ptr:get_data_info(hld.e_item_info_info_id), 0);
		end
	end
	for i = 1, ItemIdList_num do--所有材料
		local item_id = tonumber(ItemIdList[i]);
		local item_num = tonumber(ItemNumList[i]);
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if (item_template_ptr.item_type == 5 and item_template_ptr.sub_type == 1)then
			if (first_use_lock == 1 and player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag, 1) > 0)then
				item_ptr:set_data_info(hld.e_item_info_locked, 1);
			elseif (player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag, 0) < item_num)then--使用到了绑定素材
				item_ptr:set_data_info(hld.e_item_info_locked, 1);
			end
			player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_upgrade, item_ptr:get_data_info(hld.e_item_info_info_id) , item_id, item_num, first_use_lock);
		end
	end
	--记录消耗的材料
	local material_one_id = 0;
	local material_two_id = 0;
	local material_one_num = 0;
	local material_two_num = 0;
	if (ItemIdList_num >= 1)then
		material_one_id = tonumber(ItemIdList[1]);
		material_one_num = tonumber(ItemNumList[1]);
	end
	if (ItemIdList_num >= 2)then
		material_two_id = tonumber(ItemIdList[2]);
		material_two_num = tonumber(ItemNumList[2]);
	end

	if (protect_material_id > 0)then
		player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_upgrade, item_ptr:get_data_info(hld.e_item_info_info_id) , protect_material_id, protect_material_num, first_use_lock);
	end

	local cur_upgrade_active_degree_num = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_three_upgrade);
	player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_three_upgrade, cur_upgrade_active_degree_num + 1);
	local new_upgrage_level = upgrade_num_now;
	local upgrade_res = 0;

	local result_type = hld.e_upgrade_result_max;
	if (player_ref:get_first_time_do_mgr():is_first_time_do(hld.e_first_time_do_type_equip_upgrade))then
		result_type = hld.e_upgrade_result_success;
		player_ref:get_first_time_do_mgr():add_do_record(hld.e_first_time_do_type_equip_upgrade);
	else
		local upgrade_rate = hld.random_gen:get_random(0, 100);
		local cur_vip_level = player_ref:get_vip_level();
		local vip_template_ptr = template_manager:get_template_by_vip_level(cur_vip_level);
		if (vip_template_ptr ~= nil)then
			--值越小越容易成功（因为比较函数是 真实值<目标值 算中），所以所谓加成功率其实逻辑应该是减法
			upgrade_rate = upgrade_rate - vip_template_ptr.UpgradeSuccessRatioBonuse;
		end
		upgrade_rate = upgrade_rate - success_rate_add_value;
		upgrade_rate = upgrade_rate/100;

		result_type = self:get_item_upgrade_result(item_upgrade_ptr.Rate, upgrade_rate);

		if (result_type >= hld.e_upgrade_result_failed_0)then
			local down_num = result_type - hld.e_upgrade_result_failed_0;
			if (down_num > max_down_num)then
				down_num = max_down_num;
				result_type = hld.e_upgrade_result_failed_0 + down_num;
			end
		end
	end
	if(result_type == hld.e_upgrade_result_success ) then
		upgrade_res = hld.e_error_code_success;
		new_upgrage_level = new_upgrage_level + 1;
	else
		local level_sub_value = result_type - hld.e_upgrade_result_failed_0;
		upgrade_res = hld.e_error_code_item_upgrade + level_sub_value;
		new_upgrage_level =new_upgrage_level - level_sub_value;
		if (new_upgrage_level < 0)then
			new_upgrage_level = 0;
		end

		local FailGetItemList = item_upgrade_ptr.FailGetItemList
		local FailGetItemList_num = #(FailGetItemList);
		if (FailGetItemList_num > 0 and math.fmod(FailGetItemList_num, 2) == 0)then--给失败补偿
			for i = 1, FailGetItemList_num, 2 do
				local item_id = tonumber(FailGetItemList[i]);
				local item_num = tonumber(FailGetItemList[i + 1]);
				local item_ptr = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_upgrade_fail_offset, 0, item_id, item_num, 1, 0, 0, 0);
				if (item_ptr ~= nil)then
					player_ref:get_item_set():put_item_into_bag(item_ptr);
				end
			end
		end
	end

	if (upgrade_num_now ~= new_upgrage_level)then
		if (item_ptr:get_data_info(hld.e_item_info_container_type) == hld.e_bag_type_equip and self:is_equipment_effective(player_ref, item_ptr) == true)then
			player_ref:get_item_set():equip_off_upgrade_show_buff();	--去装备光效buff
			player_ref:get_item_set():set_equip_upgrade_att(item_ptr:get_equip_info_ptr(), upgrade_num_now, false);

			item_ptr:set_data_info(hld.e_item_info_upgrade_count, new_upgrage_level);

			player_ref:get_item_set():set_equip_upgrade_att(item_ptr:get_equip_info_ptr(), new_upgrage_level, true);
			player_ref:get_item_set():calcu_addition(hld.e_addition_buff_upgrade, true);
			player_ref:get_item_set():equip_on_upgrade_show_buff();	--加装备光效buff
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_equip_upgrade, 0);
		else
			item_ptr:set_data_info(hld.e_item_info_upgrade_count, new_upgrage_level);
		end
		if (result_type == hld.e_upgrade_result_success)then
			--成功公告
			player_ref:get_ranking_mgr():set_and_sync_single_equip_ranking_data(item_ptr);

			local new_item_upgrade_ptr = template_manager:get_line("ItemUpgradeTemplate", equip_template_ptr.UpgradeTemplateId + new_upgrage_level);
			if (nil ~= new_item_upgrade_ptr)then
				if (new_item_upgrade_ptr.NoticeId > 0)then
					local notice_data = {};
					notice_data.notice_id = new_item_upgrade_ptr.NoticeId
					notice_data.item_data = {}
					notice_data.num_type_data = {}
					table.insert(notice_data.item_data, item_ptr:get_item_inst());
					table.insert(notice_data.num_type_data, item_ptr:get_data_info(hld.e_item_info_upgrade_count));
					server_tool_mgr:send_notice(player_ref, notice_data);
				end
			end

			player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_single_equip_upgrade, new_upgrage_level, 0);
			player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_total_equip_upgrade_num, 0, 0);
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_equip_upgrade, new_upgrage_level, 0);
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_upgrade_total_level, 0, 0);
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_upgrade_level_num,0,0);
			player_ref:refresh_service_goal(hld.e_service_goal_type_strength_any);
		end
	end
	--不成功也调用
	player_ref:get_achievement_mgr():item_changed(item_ptr, hld.e_item_operation_levelup);
	player_ref:get_item_set():send_item_one(item_ptr, true, 0);

	if (player_ref:get_write_log())then
		local demotion = item_ptr:get_data_info(hld.e_item_info_upgrade_count) - upgrade_num_now;

		local item_template_ptr = server_tool_mgr:get_item_template(item_ptr);
		hld.server_log:upgrade_equip_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(),
			equip_template_ptr.EquipSlot, item_template_ptr.logic_id, item_template_ptr.item_color, equip_template_ptr.quality_level,
			player_ref:get_highest_record(hld.e_role_history_highest_record_total_equip_upgrade_num), demotion, money_type, money_num,
			player_ref:get_item_set():get_bind_material_num(), player_ref:get_item_set():get_no_bind_material_num(),material_one_id, material_one_num,
			material_two_id, material_two_num, protect_material_id, protect_material_num, new_upgrage_level);

		hld.server_log:serverPropUpgrade(player_ref:get_share_log_head(),  tostring(item_template_ptr.logic_id), operation_type , tostring(equip_template_ptr.quality_level),tostring(item_template_ptr.item_color), tostring(new_upgrage_level));

		if (player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_add_upgrade_level) and upgrade_num_now >= equip_template_ptr.equip_upgrade_num)then
			local activity_obj_ptr = player_ref:get_time_limit_activity_mgr():get_new_act_obj_by_act_type(hld.e_time_limit_activity_type_add_upgrade_level);
			if (activity_obj_ptr:is_valid())then
				hld.server_log:time_limit_activity_log(player_ref:get_unit_info_inst(), player_ref:get_third_info(), activity_obj_ptr:get_branch_template_id(), hld.e_time_limit_activity_type_add_upgrade_level, upgrade_num_now, item_ptr:get_data_info(hld.e_item_info_upgrade_count), item_ptr:get_item_template_id(), 1, player_ref:get_login_type(),money_type,money_num,activity_obj_ptr:get_branch_template_id());
			end
		end
	end
	return upgrade_res;
end
function item_set:item_addon(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)
	if (player_ref == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_ptr = player_ref:get_item_set():get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == item_ptr)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr);
	if (nil == item_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end

	local region_template_ptr = server_tool_mgr:get_region_template_ptr();
	if (region_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local region_money_type_index = region_template_ptr.RegionCode * 2;
	local region_money_num_index = region_money_type_index + 1;

	local equip_template_ptr = template_manager:get_line("EquipTemplate", item_ptr:get_item_logic_id());
	if (nil == equip_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_addon_template_id = equip_template_ptr.AddOnTemplateId;
	local item_addon_num_old = item_ptr:get_data_info(hld.e_item_info_add_on);
	if (item_addon_num_old >= equip_template_ptr.equip_addon_num)then
		return ConstDefine.e_item_string_unkown;
	end

	local item_addon_template_ptr = template_manager:get_line("ItemAddonTemplate", equip_template_ptr.AddOnTemplateId + item_addon_num_old);
	if (nil == item_addon_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end

	local MoneyCostList = item_addon_template_ptr.MoneyCostList
	local MoneyCostList_num = #(MoneyCostList);

	if (math.fmod(MoneyCostList_num, 2) ~= 0)then
		return ConstDefine.e_item_string_unkown;
	end
	for i = 1, MoneyCostList_num,  2 do
		if (false == player_ref:can_cut_money(tonumber(MoneyCostList[i]), tonumber(MoneyCostList[i + 1])))then
			return ConstDefine.e_fund_money_not_enough;
		end
	end--can cut money
	local use_money_instead_matirial = false;
	local locked_state = item_ptr:get_data_info(hld.e_item_info_locked);
	local ItemIdList = item_addon_template_ptr.ItemIdList
	local ItemIdList_num = #(ItemIdList);
	local ItemNumList = item_addon_template_ptr.ItemNumList
	local ItemNumList_num = #(ItemNumList);
	for i = 1, ItemIdList_num  do
		if (tonumber(ItemNumList[i]) > player_ref:get_item_set():count_item_by_template_id(tonumber(ItemIdList[i]), hld.e_bag_type_bag, -1))then
			if (use_up_item == 0)then
				return ConstDefine.e_item_string_matirial_not_enough;--
			else
				use_money_instead_matirial = true;
			end
		else
			if (locked_state == 0)then
				if (first_use_lock == 1)then
					if (player_ref:get_item_set():count_item_by_template_id(tonumber(ItemIdList[i]), hld.e_bag_type_bag, 1) > 0)then
						locked_state = 1;
					end
				else
					if (player_ref:get_item_set():count_item_by_template_id(tonumber(ItemIdList[i]), hld.e_bag_type_bag, 0) < tonumber(ItemNumList[i]))then
						locked_state = 1;
					end
				end
			end
		end
	end--material enough

	 --the item can be added on
	 --记录绑定材料消耗金币
	local money_type = 0;
	local money_num = 0;
	for i = 1, MoneyCostList_num, 2 do
		player_ref:cut_money(tonumber(MoneyCostList[i]), tonumber(MoneyCostList[i + 1]), hld.e_server_log_cut_money_item_add_on, item_ptr:get_data_info(hld.e_item_info_info_id), 0);

		money_type = tonumber(MoneyCostList[i]);
		money_num = tonumber(MoneyCostList[i + 1]);
	end--cut money

	 --记录消耗材料ID和数量
	local material_id = 0;
	local material_num = 0;
	if (use_money_instead_matirial == true)then
		local goods_template_ptr = template_manager:get_line("GoodsTemplate", item_addon_template_ptr.AddonShopId);
		if nil == goods_template_ptr then
			return ConstDefine.e_item_string_no_item;
		end

		local NewPrice = goods_template_ptr.NewPrice
		local NewPrice_num = #(NewPrice);
		if (NewPrice_num > region_money_num_index and ItemNumList_num == 1)then
			local money_id = tonumber(NewPrice[region_money_type_index + 1]);
			local money_num = tonumber(NewPrice[region_money_num_index + 1]) * tonumber(ItemNumList[1]);
			if (false == player_ref:can_cut_money(money_id, money_num))then
				return ConstDefine.e_item_string_auto_use_money_not_enough;
			end
			player_ref:cut_money(money_id, money_num, hld.e_server_log_cut_money_item_add_on, item_ptr:get_data_info(hld.e_item_info_info_id), 0);
			material_id = money_id;
			material_num = money_num;
		else
			return ConstDefine.e_item_string_no_item;
		end
	else
		if (ItemIdList_num == 1 and ItemNumList_num == 1)then
			local item_id = tonumber(ItemIdList[1]);
			local item_num = tonumber(ItemNumList[1]);
			player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_add_on, 0 , item_id, item_num, first_use_lock);--cast material

			material_id = item_id;
			material_num = item_num;
		end
	end
	if (item_ptr:get_data_info(hld.e_item_info_locked) ~= locked_state)then
		item_ptr:set_data_info(hld.e_item_info_locked, locked_state);
	end

	local cur_addon_num = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_three_addon);--活跃度
	player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_three_addon, cur_addon_num + 1);

	--不成功也调用
	player_ref:get_achievement_mgr():item_changed(item_ptr, hld.e_item_operation_addon);
	if (item_addon_template_ptr.SuccessRate <= 0)then
		return ConstDefine.e_item_add_on_failed;
	elseif (item_addon_template_ptr.SuccessRate < 100)then
		local temp_random_value = hld.random_gen:get_random(0, 100);
		if (temp_random_value > item_addon_template_ptr.SuccessRate)then
			return ConstDefine.e_item_add_on_failed;
		end
	end

	if (item_ptr:get_data_info(hld.e_item_info_container_type) == hld.e_bag_type_equip and self:is_equipment_effective(player_ref, item_ptr) == true)then
		player_ref:get_item_set():set_equip_addon_att(item_ptr:get_equip_info_ptr(), item_addon_num_old, false);
		player_ref:get_item_set():equip_off_addon_show_buff();

		item_ptr:set_data_info(hld.e_item_info_add_on, item_addon_num_old + 1);

		player_ref:get_item_set():calcu_addition(hld.e_addition_buff_addon, true);
		player_ref:get_item_set():equip_on_addon_show_buff();
		player_ref:get_item_set():set_equip_addon_att(item_ptr:get_equip_info_ptr(), item_addon_num_old + 1, true);
	else
		item_ptr:set_data_info(hld.e_item_info_add_on, item_addon_num_old + 1);
	end

	player_ref:get_achievement_mgr():item_changed(item_ptr, hld.e_item_operation_addon);

	player_ref:get_item_set():send_item_one(item_ptr, true , 0);

	player_ref:get_ranking_mgr():set_and_sync_single_equip_ranking_data(item_ptr);

	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_total_equip_addon_num, 0, 0);

	local is_success = item_ptr:get_data_info(hld.e_item_info_add_on) - item_addon_num_old;
	hld.server_log:addon_equip_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), equip_template_ptr.EquipSlot,
		item_template_ptr.logic_id, equip_template_ptr.quality_level, item_template_ptr.item_color, item_ptr:get_data_info(hld.e_item_info_add_on),
		is_success, money_type, money_num, player_ref:get_item_set():get_bind_material_num(), player_ref:get_item_set():get_no_bind_material_num(), material_id, material_num);

	
	hld.server_log:serverPropUpgrade(player_ref:get_share_log_head(),tostring(item_template_ptr.logic_id), operation_type , tostring(equip_template_ptr.quality_level),tostring(item_template_ptr.item_color), tostring(item_ptr:get_data_info(hld.e_item_info_add_on)));


	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_addon_equip, 0, 0);

	--check mission
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_equip_addon, item_addon_num_old + 1, 0);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_add_on_total_level, 0, 0);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_addon_level_num, 0, 0);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_equip_upgrade, 0);


	if (ConstDefine.min_add_on_notice > item_ptr:get_data_info(hld.e_item_info_add_on))then
		return ConstDefine.e_item_string_succeed;
	end

	-- 装备追加公告
	local new_item_addon_ptr = template_manager:get_line("ItemAddonTemplate", equip_template_ptr.AddOnTemplateId + item_ptr:get_data_info(hld.e_item_info_add_on));
	local notice_id = new_item_addon_ptr.Noticeld;
	if (notice_id > 0)then
		local notice_data = {};
		notice_data.notice_id = notice_id
		notice_data.item_data = {}
		notice_data.num_type_data = {}
		table.insert(notice_data.item_data, item_ptr:get_item_inst());
		table.insert(notice_data.num_type_data, item_ptr:get_data_info(hld.e_item_info_add_on));
		server_tool_mgr:send_notice(player_ref, notice_data);
	end
	player_ref:refresh_service_goal(hld.e_service_goal_type_addition_the_body);
	return ConstDefine.e_item_string_succeed;
end

function item_set:get_item_upgrade_result(rate_array, real_rate)
	local rate_array = rate_array
	local rate_array_num = #(rate_array);
	if (rate_array_num == 0)then
		return hld.e_upgrade_result_failed_0;
	end
	local return_value = rate_array_num - 1;
	local temp_value = 0.0;
	for i = 1, rate_array_num do
		temp_value = temp_value + tonumber(rate_array[i]);
		if (real_rate <= temp_value)then
			return_value = i - 1;
			break;
		end
	end
	if (return_value >= hld.e_upgrade_result_max)then
		return_value = hld.e_upgrade_result_max - 1;
	end
	return return_value;
end
function item_set:upgrade_protect_item_can_use(player_ref, item_upgrade_ptr, cur_upgrade_num, protect_item_index)
	if (protect_item_index < 0)then
		return hld.e_error_code_item_upgrade;
	end
	if nil == item_upgrade_ptr then
		return hld.e_error_code_item_upgrade;
	end
	local ProtectItemInfo = item_upgrade_ptr.ProtectItemInfo
	local ProtectItemInfo_num = #(ProtectItemInfo);
	if (math.fmod(ProtectItemInfo_num, 2) ~= 0)then
		return hld.e_error_code_item_upgrade;
	end
	local data_num = math.floor(ProtectItemInfo_num / 2);
	if (protect_item_index >= data_num)then
		return hld.e_error_code_item_upgrade;
	end
	local item_id = tonumber(ProtectItemInfo[protect_item_index * 2 + 1]);
	local item_num = tonumber(ProtectItemInfo[protect_item_index * 2 + 2]);
	if (item_id <= 0 or item_num <= 0)then
		return hld.e_error_code_item_upgrade;
	end

	if (player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag, -1) < item_num)then
		return hld.e_error_code_item_matirial_not_enough;
	end

	local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
	if (nil == item_template_ptr)then
		return hld.e_error_code_item_upgrade;
	end
	local props_template_ptr = template_manager:get_line("PropBasicAttributeTemplate", item_template_ptr.logic_id);
	if (nil == props_template_ptr)then
		return hld.e_error_code_item_upgrade;
	end
	--dump(props_template_ptr.FuncLevelLimit)
	--如果使用小于作用下限或者大于作用上限，则不能使用
	local FuncLevelLimit = props_template_ptr.FuncLevelLimit
	local FuncLevelLimit_num = #(FuncLevelLimit);
	if (FuncLevelLimit_num >= 2)then
		if (cur_upgrade_num < tonumber(FuncLevelLimit[1]) or cur_upgrade_num > tonumber(FuncLevelLimit[2])) then
			return hld.e_error_code_item_upgrade;
		end
	end
	return hld.e_error_code_success;
end
function item_set:is_can_add_att(player_ref, item_ptr)
	if (nil == item_ptr) then
		return true;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr)
	if (nil == item_template_ptr) then
		return true;
	end
	if (item_template_ptr.item_type ~= hld.e_item_type_weapon and item_template_ptr.item_type ~= hld.e_item_type_armor) then
		return true;
	end
	local EquipOnNeedAtt = item_template_ptr.EquipOnNeedAtt
	local EquipOnNeedAtt_num = #(EquipOnNeedAtt);
	if (EquipOnNeedAtt_num ~= 8) then
		return true;
	end
	local player_att = {};
	local equip_need_att = {};
	for i = 0, 3 do
		player_att[i] = player_ref:get_pawn_att():get_unit_base_att(i + hld.e_base_att_info_strength) + player_ref:get_pawn_att():get_unit_base_att(i + hld.e_base_att_info_body_strength);
	end
	for i = 1, EquipOnNeedAtt_num, 2 do
		local need_att_index = tonumber(EquipOnNeedAtt[i]) - 1;
		equip_need_att[need_att_index] = tonumber(EquipOnNeedAtt[i + 1]);
	end
	local can_add_att = true
	for k,v in pairs(player_att) do
		if (equip_need_att[k] ~= nil and v < equip_need_att[k]) then
			can_add_att = false;
		end
	end
	return can_add_att;
end
function item_set:is_in_duration(item_ptr)
	if (nil == item_ptr)then
		return true;
	end
	if (item_ptr:get_data_info(hld.e_item_info_over_time) > 0 and item_ptr:get_data_info(hld.e_item_info_over_time) <= os.time()) then
		return false;
	end
	return true;
end
function item_set:is_equipment_effective(player_ref, item_ptr)
	return self:is_can_add_att(player_ref, item_ptr) and self:is_in_duration(item_ptr);
end
function item_set:send_forge_message(player_ref, item_ptr, is_save_operate, in_is_enabled)
	--发送消息
	local msg = {};
	msg.property_name = {}
	msg.property_value = {}
	msg.property_index = {}
	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property6 do
		local forge_property = item_ptr:get_data_info(i);
		if forge_property > 0 then
			local forge_name_flag_att = item_ptr:get_forge_name_flag_att(i);
			local forge_name_att = item_ptr:get_forge_name_att(i);
			local forge_att = item_ptr:get_forge_att(i);
			table.insert(msg.property_name, forge_name_att);
			table.insert(msg.property_value, forge_att);
			table.insert(msg.property_index, forge_name_flag_att);
		end
	end
	msg.is_save_operate = is_save_operate;
	msg.in_is_enabled = in_is_enabled;
	MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_forge_end",msg, 10740)
end
function item_set:item_forge_begin(player_ref, item_guid)
	if (nil == player_ref)then
		return hld.e_error_code_no_template;
	end
	local item_ptr = player_ref:get_item_set():get_item(item_guid[1],item_guid[2]);
	if (item_ptr == nil)then
		return hld.e_error_code_no_template;
	end
	local forge_not_save_num = item_ptr:have_not_save_forge_att_num();
	local in_is_enabled = (item_ptr:have_not_save_forge_att_num()>0);

	--发送消息
	self:send_forge_message(player_ref, item_ptr, false, in_is_enabled);
	return hld.e_error_code_success;
end
function item_set:rand_excellent_att_color(equip_template_ptr)
	if (equip_template_ptr == nil) then
		return hld.e_item_color_max;
	end

	local color_rand_value = hld.random_gen:get_random(1, 100);
	--不能从0开始

	local color_rand_target_value = 0;
	local final_excellent_color = 0;
	local ForgeExcellentAttQualityWeight = equip_template_ptr.ForgeExcellentAttQualityWeight
	local ForgeExcellentAttQualityWeight_num = #(ForgeExcellentAttQualityWeight)
	for i = 1, ForgeExcellentAttQualityWeight_num do
		color_rand_target_value = color_rand_target_value + tonumber(ForgeExcellentAttQualityWeight[i]);
		if (color_rand_value <= color_rand_target_value)then
			return i - 1;
		end
	end
	return hld.e_item_color_max;
end
function item_set:item_forge(player_ref, item_guid, forge_lock_flag, use_lock_first)
	if (nil == player_ref)then
		return hld.e_error_code_no_template;
	end
	local item_ptr = player_ref:get_item_set():get_item(item_guid[1],item_guid[2]);
	if (item_ptr == nil)then
		return hld.e_error_code_no_template;
	end
	local forge = {};
	local lock_forge_mum = 0;
	for i = 0, hld.item_temp_forge_property_max-1 do
		forge[i] = (forge_lock_flag >> i) & 1;
		if (forge[i] > 0) then
			lock_forge_mum = lock_forge_mum + 1;
		end
	end
	local temp_color_rate_array;
	local att_num_rand_value = hld.random_gen:get_random(1, 100);
	local att_num_rand_target_value = 0;
	local rand_final_att_num = 0;--需要几条卓越属性
	local equip_template_ptr = template_manager:get_line("EquipTemplate", item_ptr:get_item_logic_id());
	if (nil == equip_template_ptr)then
		return hld.e_error_code_no_template;
	end

	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr);
	if (nil == item_template_ptr)then
		return hld.e_error_code_no_template;
	end
	if (item_ptr:get_data_info(hld.e_item_info_slot) > hld.e_role_equip_slot_amulet or equip_template_ptr.quality_level < hld.e_quality_level_five or item_template_ptr.item_color < hld.e_item_color_orange)then
		return hld.e_error_code_no_template;
	end
	local BaseExcellentAtt = equip_template_ptr.BaseExcellentAtt
	local BaseExcellentAtt_num = #(BaseExcellentAtt)
	local GreenExcellentRatio = equip_template_ptr.GreenExcellentRatio
	local GreenExcellentRatio_num = #(GreenExcellentRatio)
	local BlueExcellentRatio = equip_template_ptr.BlueExcellentRatio
	local BlueExcellentRatio_num = #(BlueExcellentRatio)
	local PurpleExcellentRatio = equip_template_ptr.PurpleExcellentRatio
	local PurpleExcellentRatio_num = #(PurpleExcellentRatio)
	local OrangeExcellentRatio = equip_template_ptr.OrangeExcellentRatio
	local OrangeExcellentRatio_num = #(OrangeExcellentRatio)
	local RedExcellentRatio = equip_template_ptr.RedExcellentRatio
	local RedExcellentRatio_num = #(RedExcellentRatio)
	local total_att_num = BaseExcellentAtt_num // hld.e_att_one_max;
	if (total_att_num ~= GreenExcellentRatio_num // 2
		or total_att_num ~= BlueExcellentRatio_num // 2
		or total_att_num ~= PurpleExcellentRatio_num // 2
		or total_att_num ~= OrangeExcellentRatio_num // 2
		or total_att_num ~= RedExcellentRatio_num // 2
		or total_att_num <= 0)then
		return hld.e_error_code_no_template;
	end
	if (player_ref:get_item_set():count_item_by_template_id(equip_template_ptr.MaterialID, hld.e_bag_type_bag, -1) < equip_template_ptr.MaterialCost)then
		--没材料
		return hld.e_error_code_item_matirial_not_enough;
	end
	local LockMaterialID = equip_template_ptr.LockMaterialID
	local LockMaterialID_num = #(LockMaterialID)
	local LockMaterialCost = equip_template_ptr.LockMaterialCost
	if (lock_forge_mum > 0 and player_ref:get_item_set():count_item_by_template_id(tonumber(LockMaterialID[lock_forge_mum]), hld.e_bag_type_bag, -1) < tonumber(LockMaterialCost[lock_forge_mum]))then
		--没材料
		return hld.e_error_code_item_matirial_not_enough;
	end
	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property6 do
		item_ptr:set_data_info(i, 0);
	end
	for i = 0, hld.item_temp_forge_property_max - 1 do
		forge[i] = (forge_lock_flag >> i) & 1;
		if (forge[i] > 0) then
			item_ptr:set_forge_property(i);
		end
	end

	local att_index_array = {};
	for i = 1, BaseExcellentAtt_num, 5 do
		table.insert(att_index_array, tonumber(BaseExcellentAtt[i + 1]));
	end

	local ExcellentAttNumGenRate = equip_template_ptr.ExcellentAttNumGenRate
	local ExcellentAttNumGenRate_num = #(ExcellentAttNumGenRate)
	for i = 1, ExcellentAttNumGenRate_num do
		att_num_rand_target_value = att_num_rand_target_value + tonumber(ExcellentAttNumGenRate[i]);
		if (att_num_rand_value <= att_num_rand_target_value)then
			rand_final_att_num = i - 1;
			break;
		end
	end
	local max_att_num = ExcellentAttNumGenRate_num - 1;
	local final_att_num = 0;
	if (rand_final_att_num <= lock_forge_mum)then
		local att_num = lock_forge_mum + rand_final_att_num
		if att_num > max_att_num then
			final_att_num = max_att_num
		else
			final_att_num = att_num
		end
	else
		final_att_num = rand_final_att_num;
	end
	local get_att_index_array = {}
	local temp_att_index_lib_array = {}
	local in_temp_att_index_lib_array = {};

	for i = 0, hld.item_temp_forge_property_max - 1 do
		if (forge[i] ~= nil and forge[i] > 0)then
			local forge_property = item_ptr:get_data_info(i + hld.e_item_info_forge_property1);
			local forge_name_flag_att = (forge_property >> 24);
			in_temp_att_index_lib_array[forge_name_flag_att] = true;
		end
	end
	for i = 0, total_att_num -1 do
		if (in_temp_att_index_lib_array[i] == nil) then
			table.insert(temp_att_index_lib_array, i);
		end
	end
	for i = 0, final_att_num - lock_forge_mum - 1 do
		local size_temp_att_index_lib = #temp_att_index_lib_array;
		if (size_temp_att_index_lib <= 0)then
			break;
		end
		local key = server_tool_mgr:read_table_rand(temp_att_index_lib_array);
		table.insert(get_att_index_array, temp_att_index_lib_array[key]);
		temp_att_index_lib_array[key] = nil;
	end
	local get_att_index_array_num = #get_att_index_array
	local j = 0
	for i = 1, get_att_index_array_num do
		if (get_att_index_array[i] >= 0 and get_att_index_array[i] < 32) then
			while (forge[j] ~= nil and forge[j] > 0)
			do
				j = j + 1;
			end
			if (j > final_att_num) then
				break;
			end
			local att_index = get_att_index_array[i];
			local final_excellent_color = self:rand_excellent_att_color(equip_template_ptr);
			local temp_color_rate_array = template_manager:get_excellent_target_color_array(equip_template_ptr, final_excellent_color);
			if nil == temp_color_rate_array then
				return hld.e_error_code_no_template;
			end
			temp_color_rate_array = temp_color_rate_array
			local min_limit = tonumber(temp_color_rate_array[att_index * 2 + 1]);
			local max_limit = tonumber(temp_color_rate_array[att_index * 2 + 2]);
			local random_limit = hld.random_gen:get_random(min_limit, max_limit);

			local forge_property = 0;
			forge_property = forge_property | random_limit;
			forge_property = forge_property |(att_index_array[att_index + 1] << 16);
			forge_property = forge_property |(att_index << 24);
			item_ptr:set_data_info(j + hld.e_item_info_forge_property1, forge_property);
			j = j + 1;
		end
	end
	local have_att_idex = {} ;
	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property6 do
		if (item_ptr:get_data_info(i) ~= 0)then
			table.insert(have_att_idex, i);
		end
	end
	local have_att_idex_num = #have_att_idex;
	for i = 1, have_att_idex_num do
		item_ptr:set_data_info(hld.e_item_info_forge_property1 + i - 1, item_ptr:get_data_info(have_att_idex[i]));
	end
	for i = have_att_idex_num, hld.item_temp_forge_property_max do
		item_ptr:set_data_info(hld.e_item_info_forge_property1 + i, 0);
	end
	--扣除物品
	player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_forge, 0, equip_template_ptr.MaterialID, equip_template_ptr.MaterialCost, 1);
	if (lock_forge_mum > 0)then
		player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_forge, 0, tonumber(LockMaterialID[lock_forge_mum]), tonumber(LockMaterialCost[lock_forge_mum ]), 1);
	end
	--发送消息
	self:send_forge_message(player_ref, item_ptr, false, true);
	return hld.e_error_code_success;
end
function item_set:item_save_forge(player_ref, item_guid)
	if (nil == player_ref)then
		return hld.e_error_code_no_template;
	end
	local item_ptr = player_ref:get_item_set():get_item(item_guid[1],item_guid[2]);
	if (item_ptr == nil)then
		return hld.e_error_code_no_template;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr);
	if (item_template_ptr == nil)then
		return hld.e_error_code_no_template;
	end

	local equip_template_ptr = template_manager:get_line("EquipTemplate", item_ptr:get_item_logic_id());
	if (equip_template_ptr == nil)then
		return hld.e_error_code_no_template;
	end

	player_ref:get_item_set():equip_off_forge_show_buff()
	pawn_att_mgr:set_excellent_att_array(player_ref, equip_template_ptr.BaseExcellentAtt, item_ptr, false, 1);

	local temp_forge_att_flag = 0;
	local final_att_num = item_ptr:have_not_save_forge_att_num();
	if final_att_num <= 0 then
		return hld.e_error_code_no_template;
	end
	local item_temp_forge_block = {};
	local item_temp_forge_sort_block = {};
	local item_temp_forge_name_flag_block = {};
	for i = 0, final_att_num - 1 do
		local forge_name_flag_att = item_ptr:get_forge_name_flag_att(i + hld.e_item_info_forge_property1);
		local forge_name_att = item_ptr:get_forge_name_att(i + hld.e_item_info_forge_property1);
		local forge_att = item_ptr:get_forge_att(i + hld.e_item_info_forge_property1);
		item_temp_forge_block[i] = forge_att;
		item_temp_forge_name_flag_block[i] = forge_name_flag_att;
		temp_forge_att_flag = temp_forge_att_flag | (1 << item_temp_forge_name_flag_block[i]);
	end
	item_ptr:set_data_info(hld.e_item_info_random_had_flag, temp_forge_att_flag);
	for i = 0, final_att_num - 2 do
		local min_index = i;
		for j = i + 1, final_att_num - 1 do
			if (item_temp_forge_name_flag_block[j] < item_temp_forge_name_flag_block[min_index]) then
				min_index = j;
			end
		end
		item_temp_forge_sort_block[i] = item_temp_forge_block[min_index];
		if (min_index ~= i)then
			item_temp_forge_name_flag_block[min_index] = item_temp_forge_name_flag_block[i];
			item_temp_forge_block[min_index] = item_temp_forge_block[i];
		end
	end
	item_temp_forge_sort_block[final_att_num - 1] = item_temp_forge_block[final_att_num - 1];
	for i = 0, hld.item_temp_forge_property_max - 1 do
		if item_temp_forge_sort_block[i] ~= nil then
			item_ptr:set_data_info(hld.e_item_info_random_property1 + i, item_temp_forge_sort_block[i]);
		end
	end
	player_ref:get_item_set():send_item_one(item_ptr, true, 0);
	--发送消息
	self:send_forge_message(player_ref, item_ptr, true, false);

	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property6 do
		item_ptr:set_data_info(i, 0);
	end
	player_ref:get_ranking_mgr():set_and_sync_single_equip_ranking_data(item_ptr);
	player_ref:get_item_set():equip_on_forge_show_buff()
	pawn_att_mgr:set_excellent_att_array(player_ref, equip_template_ptr.BaseExcellentAtt, item_ptr, true, 1);
	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_max_forge_level, 0, 0);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_equip_upgrade, 0);
	return hld.e_error_code_success;
end

function item_set:fashion_upgrade(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do )
		local use_money_instead_matirial = false;
		local fashion_ptr = player_ref:get_item_set():get_item(item_guid_array[1], item_guid_array[2]);
		local free_upgrade = false; --免费升级
		if fashion_ptr == nil then
			return ConstDefine.e_item_string_no_item;
		end
		local item_ptr_template_ptr  = template_manager:get_line("ItemTemplate", fashion_ptr:get_item_template_id());
		if item_ptr_template_ptr == nil then
			return ConstDefine.e_item_string_unkown;
		end
		if nil == fashion_ptr:get_item_info_ptr() then
			return ConstDefine.e_item_string_unkown;
		end
		local temp_fashion_template_ptr = template_manager:get_line("FasionTemplate", item_ptr_template_ptr.logic_id);
		if temp_fashion_template_ptr == nil then
			return ConstDefine.e_item_string_unkown;
		end
		local game_config_tmpl_ptr = server_tool_mgr:get_game_config_template_ptr();
		if game_config_tmpl_ptr == nil then
			return ConstDefine.e_item_string_unkown;
		end

		--选择时装任务免费激活时装如果激活了则奖励一件  first_use_lock为奖励时装id
		if force_do == 1 then
			if fashion_ptr:get_data_info(hld.e_item_info_upgrade_count) == 0 then
				if player_ref:get_logic_data(hld.e_role_logic_info_choose_fashion_id) == 0 then
					free_upgrade = true;
					player_ref:set_logic_data(hld.e_role_logic_info_choose_fashion_id, first_use_lock)
					player_ref:send_logic_one(hld.e_role_logic_info_choose_fashion_id)
				end	
				player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_choose_a_fashion, 1, 0);
			else
				if player_ref:get_logic_data(hld.e_role_logic_info_choose_fashion_id) == 0 then
					local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_add_item_to_bag, 0, first_use_lock, 1, 1, 0, 0, 0)
					if temp_item ~= nil then
						player_ref:get_item_set():put_item_into_bag(temp_item)
						player_ref:set_logic_data(hld.e_role_logic_info_choose_fashion_id, first_use_lock)
						player_ref:send_logic_one(hld.e_role_logic_info_choose_fashion_id)
					end
				end	
				player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_choose_a_fashion, 1, 0);
				return hld.e_error_code_success;
			end
		end

		if fashion_ptr:get_data_info(hld.e_item_info_upgrade_count) == 5 and player_ref:get_unit_info(hld.e_role_info_exp_level) < game_config_tmpl_ptr.FashionNextUpgradeLevel then
			return ConstDefine.e_item_string_level;
		end

		local fashion_real_id = item_ptr_template_ptr.logic_id + fashion_ptr:get_data_info(hld.e_item_info_upgrade_count);
		local real_fashion_template_ptr = template_manager:get_line("FasionTemplate", fashion_real_id);

		if real_fashion_template_ptr == nil then
			return ConstDefine.e_item_string_unkown;
		end
		if real_fashion_template_ptr.GradeLimit > player_ref:get_grade_num() then
			return ConstDefine.e_item_string_unkown;
		end

		local is_big_upgrade = false;
		--等级不等于最高等级，升级

		local t_template_ptr_UpgradeMaterial = Common:StrToTable(real_fashion_template_ptr.UpgradeMaterial)

		if #t_template_ptr_UpgradeMaterial ~= 0 and t_template_ptr_UpgradeMaterial[1] > 0 then
			local item_id = t_template_ptr_UpgradeMaterial[1];
			local need_num = t_template_ptr_UpgradeMaterial[2];
			local have_num = player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag, -1);
			if have_num < item_num then
				if use_up_item == 0 then
					if free_upgrade == false then
						return ConstDefine.e_item_string_matirial_not_enough;
					end
				else
					use_money_instead_matirial = true;
				end
			end
			
			if use_money_instead_matirial then
				return ConstDefine.e_item_string_no_item;
			else
				if free_upgrade == false then
					player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_upgrade, 0, item_id, item_num, hld.e_item_locked);
				end
			end
			
			local new_exp = fashion_ptr:get_data_info(hld.e_item_info_wing_exp) + item_num;
			fashion_ptr:set_data_info(hld.e_item_info_wing_exp, new_exp);
			
			if new_exp >= need_num then
				fashion_ptr:set_data_info(hld.e_item_info_wing_exp, new_exp - need_num);
				if real_fashion_template_ptr ~= nil then
					pawn_att_mgr:apply_att_change_by_array(player_ref,real_fashion_template_ptr.AttArray,false,1.0)
					local fashion_upgrade_now = fashion_ptr:get_data_info(hld.e_item_info_upgrade_count);
					fashion_ptr:set_data_info(hld.e_item_info_upgrade_count, fashion_upgrade_now + 1);
					real_fashion_template_ptr ={}
					fashion_real_id = 0
					fashion_real_id = item_ptr_template_ptr.logic_id + fashion_ptr:get_data_info(hld.e_item_info_upgrade_count);
					real_fashion_template_ptr = template_manager:get_line("FasionTemplate", fashion_real_id);
					if real_fashion_template_ptr ~= nil then
						pawn_att_mgr:apply_att_change_by_array(player_ref,real_fashion_template_ptr.AttArray,true,1.0)
					else
						fashion_ptr:set_data_info(hld.e_item_info_upgrade_count, fashion_upgrade_now);
						pawn_att_mgr:apply_att_change_by_array(player_ref,real_fashion_template_ptr.AttArray,true,1.0)
					end
				end
				local temp_addition_buff_template_ptr = template_manager:get_line("AdditionBuffTemplate", 58080001);
				local i = 0;
				player_ref:get_item_set():add_fashion_star_num();
				local m_fashion_star_num = player_ref:get_item_set():get_fashion_star_num()
				while temp_addition_buff_template_ptr ~= nil do
					if temp_addition_buff_template_ptr.UpgradeNum >= m_fashion_star_num then
						if temp_addition_buff_template_ptr.UpgradeNum == m_fashion_star_num then
							pawn_att_mgr:apply_att_change_by_array(player_ref,temp_addition_buff_template_ptr.AdditionValue,true,1.0)
							if i > 0 then
								temp_addition_buff_template_ptr = template_manager:get_line("AdditionBuffTemplate", 58080000 + i);
								pawn_att_mgr:apply_att_change_by_array(player_ref,temp_addition_buff_template_ptr.AdditionValue,false,1.0)
							end
						end
						break;
					end
					i = i + 1
					temp_addition_buff_template_ptr = template_manager:get_line("AdditionBuffTemplate", 58080001 + i);
				end
			end
			if fashion_ptr:get_data_info(hld.e_item_info_activate) == 0 then
				fashion_ptr:set_data_info(hld.e_item_info_activate, 1);
			end
			player_ref:get_phantom_mgr():on_item_oper(fashion_ptr, fashion_ptr:get_data_info(hld.e_item_info_info_id));
			player_ref:get_item_set():send_item_one(fashion_ptr,true,0);
		else
			is_big_upgrade = true;
			return ConstDefine.e_item_string_full_level;
		end

		local effect_msg={}
		if real_fashion_template_ptr ~= nil then
			effect_msg.show_type =  hld.e_item_upgrade_effect_type_fashion_upgrade
			effect_msg.show_value = fashion_ptr:get_data_info(hld.e_item_info_upgrade_count)
			MessageMgr:send_message_to_self(player_ref, "hld.item_proto.show_item_upgrade_effect",effect_msg, 10723)
		end
		return hld.e_error_code_success;
end
------------------------------------------------------------------------------------------------------------------------------------------------
--辅助接口
function item_set:get_succinc_info_ptr(item_ptr)

	local equip_template_ptr = template_manager:get_line("EquipTemplate", item_ptr:get_item_logic_id())
	if equip_template_ptr == nil then
		return nil
	end
	return template_manager:get_line("ItemSuccinctTemplate", equip_template_ptr.SuccinctTemplateID)
end

--辅助接口
function item_set:get_property_max_limit(item_ptr)

	local result = {}

	local temp_succinct_template = item_set:get_succinc_info_ptr(item_ptr)
	if temp_succinct_template == nil then
		return result
	end
	local property_max_limit = temp_succinct_template.PropertyMaxLimit
	local property_max_limit_len = #(property_max_limit)
	local property_ID = temp_succinct_template.PropertyID
	local property_ID_len = #(property_ID)
	if property_max_limit_len <= 0 or property_ID_len <= 0 or property_max_limit_len % property_ID_len ~= 0 then
		return result
	end


	local max_succinct_buff_level = property_max_limit_len / property_ID_len;
	local max_limit_begin_index   = property_ID_len * (max_succinct_buff_level - 1);
	for i = 1, property_ID_len do
		table.insert(result, property_max_limit[max_limit_begin_index + i])
	end
	return result
end

--辅助接口
function item_set:random_property_value(up_chance_s, max_des_s, min_des_s, min_ins_s, max_ins_s)
	
	up_chance = tonumber(up_chance_s)
	max_des = tonumber(max_des_s)
	min_des = tonumber(min_des_s)
	min_ins = tonumber(min_ins_s)
	max_ins = tonumber(max_ins_s)

	if max_des < min_des then
		local t = max_des
		max_des = min_des
		min_des = t
	end

	if max_ins < min_ins then
		local t = max_ins
		max_ins = min_ins
		min_ins = t
	end

	local result = 0
	local offset = 0
	local rand_value = hld.random_gen:get_random(0, 100)
	if rand_value <= up_chance then
		if min_ins < 0 then
			offset 	= 0 - min_ins
			mis_ins = mis_ins + offset
			max_ins = max_ins + offset
		end
		result = hld.random_gen:get_random(min_ins, max_ins)
	else
		if min_des < 0 then
			offset 	= 0 - min_des
			min_des = min_des + offset
			max_des = max_des + offset
		end
		result = hld.random_gen:get_random(min_des, max_des)
	end
	result = result - offset
	return result
end

--原来的接口
--int32 item_set::item_succinct(const guid_64& item_guid, int32 money_cost_typ, int32 first_use_lock, int32 is_auto, int32 is_use_jewel)
--参数对应关系：
--item_guid      == item_guid_array[1][2]
--money_cost_typ == operation_type
--first_use_lock == first_use_lock
--is_auto        == use_up_item
--is_use_jewel	 == force_do
function item_set:item_succinct(temp_player, item_guid_array, item_slot, operation_type, is_auto, first_use_lock, item_num, is_use_jewel)

	if temp_player == nil then
		return hld.e_error_code_no_template
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
    end

	local temp_money_cost_typ = 0
	local temp_money_cost_num = 0
	local temp_item = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	if temp_item == nil then
		return hld.e_error_code_no_template
	end

	local is_change = false
	local vec_property_max_limit = item_set:get_property_max_limit(temp_item)

	local equip_template_ptr = template_manager:get_line("EquipTemplate", temp_item:get_item_logic_id())
	if nullptr == equip_template_ptr then
		return hld.e_error_code_no_template
	end

	for i = 0, hld.item_temp_succinct_property_max - 1 do
		temp_item:set_temp_succinct_att(i, 0)
	end

	local temp_succinct_template = item_set:get_succinc_info_ptr(temp_item)
	if temp_succinct_template == nil then
		return hld.e_error_code_no_template
	end

	local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return hld.e_error_code_no_template
	end

	local region_money_type_index = region_template_ptr.RegionCode * 2
	local region_money_num_index  = region_money_type_index + 1

	local cost_type = hld.e_money_type_jewel
	if operation_type == hld.e_item_operation_succinct_gold then
		cost_type = hld.e_money_type_silver_bind
	end
	local cost_num = tonumber(temp_succinct_template.DiamondCost)
	if operation_type == hld.e_item_operation_succinct_gold then
		cost_num = tonumber(temp_succinct_template.GoldCost)
	end

	local material_id  = 0
	local material_num = 0

	local GoldPropertyChangeRange = temp_succinct_template.GoldPropertyChangeRange
	local DiamondPropertyChangeRange = temp_succinct_template.DiamondPropertyChangeRange
	local PropertyID = temp_succinct_template.PropertyID

	if #GoldPropertyChangeRange ~= (#PropertyID * hld.item_succinct_change_max) then
		return hld.e_error_code_no_template
	end
	if #DiamondPropertyChangeRange ~= (#PropertyID * hld.item_succinct_change_max) then
		return hld.e_error_code_no_template
	end


	local GoldSuccinctEffectChance = {}
	local GoldSuccinctAttUpChance = {}
	local GoldPropertyChangeRange = {}
	if operation_type == hld.e_item_operation_succinct_gold then
		temp_money_cost_typ = hld.e_money_type_silver_bind
		temp_money_cost_num = temp_succinct_template.GoldCost
		GoldSuccinctEffectChance = temp_succinct_template.GoldSuccinctEffectChance
		GoldSuccinctAttUpChance  = temp_succinct_template.GoldSuccinctAttUpChance
		GoldPropertyChangeRange  = temp_succinct_template.GoldPropertyChangeRange
	else
		temp_money_cost_typ = hld.e_money_type_jewel
		temp_money_cost_num = temp_succinct_template.DiamondCost
		GoldSuccinctEffectChance = temp_succinct_template.DiamondSuccinctEffectChance
		GoldSuccinctAttUpChance  = temp_succinct_template.DiamondSuccinctAttUpChance
		GoldPropertyChangeRange  = temp_succinct_template.DiamondPropertyChangeRange
	end

	if not temp_player:can_cut_money(temp_money_cost_typ, temp_money_cost_num) then
		return hld.e_error_code_item_money
	end
		
	if item_set_ref:count_item_by_template_id(temp_succinct_template.MaterialID, hld.e_bag_type_bag, -1) < temp_succinct_template.MaterialCost then
		--没材料
		if is_use_jewel == 0 then
			return hld.e_error_code_item_matirial_not_enough
		else
			local NewPrice = template_manager:get_field("GoodsTemplate", temp_succinct_template.MaterialShopID, "NewPrice")
			material_id =  NewPrice[region_money_type_index + 1]
			material_num = NewPrice[region_money_num_index + 1] * temp_succinct_template.MaterialCost
			is_change = true
		end
	end

	if is_change == true then
		if cost_type == material_id and not temp_player:can_cut_money(cost_type, cost_num + material_num) then
			return hld.e_error_code_item_money
		end
		if cost_type ~= material_id and not temp_player:can_cut_money(material_id, material_num) then
			return hld.e_error_code_item_money
		end
	end

	local effect_block = {}
	--已满的属性不参与随机，也不占用“保底”影响名额,存储可被影响属性的下标
	for i = 1, #vec_property_max_limit do
		if temp_item:get_data_info(i - 1  + hld.e_item_info_succinct_property1) < tonumber(vec_property_max_limit[i]) then
			table.insert(effect_block,i) -- 从1开始的
		end
	end

	if #effect_block <= 0 then
		return hld.e_error_code_item_full_level
	end

	temp_player:cut_money(temp_money_cost_typ, temp_money_cost_num, hld.e_server_log_cut_money_item_succinct, temp_item:get_data_info(hld.e_item_info_info_id), 0)
	if is_change == true then
		temp_player:cut_money(material_id, material_num, hld.e_server_log_cut_money_item_succinct, temp_item:get_data_info(hld.e_item_info_info_id), 0)
	else
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_succinct, 0, temp_succinct_template.MaterialID, temp_succinct_template.MaterialCost, first_use_lock);
	end

	----------经过对比钻石洗练和金币洗练基本上都差不多所以下面使用相同的代码了---------------------------------------------


	local total_chance = 0
	local GoldSuccinctEffectChance = temp_succinct_template.GoldSuccinctEffectChance
	for i = 1, #(GoldSuccinctEffectChance) do
		--确定影响属性数随机总权重
		total_chance = total_chance + tonumber(GoldSuccinctEffectChance[i])
	end

	local effect_random_value = hld.random_gen:get_random(1, total_chance)
	local effect_num = 0
	total_chance = 0
	for i = 1, #GoldSuccinctEffectChance do
		--确定此次洗练要影响几条属性（条数权重0的不考虑）
		if GoldSuccinctEffectChance[i] == 0 then
			effect_num = effect_num + 1
		else
			total_chance = total_chance + GoldSuccinctEffectChance[i]
			if effect_random_value <= total_chance then
				break
			end
			effect_num = effect_num + 1
		end
	end

	for i = 1, effect_num do
		if #effect_block == 0 then
			break
		end

		local size_effect_block  = #effect_block
		local effect_block_index = hld.random_gen:get_random(1, size_effect_block)
		local real_index = effect_block[effect_block_index] - 1

		local property_value = 0
		local cur_property_value = 0
		
		cur_property_value = temp_item:get_data_info(real_index + hld.e_item_info_succinct_property1)

		--被影响的属性当前值为0时，只能出现上升结果
		if #GoldSuccinctAttUpChance > real_index and #GoldPropertyChangeRange > real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_max then
			if cur_property_value == 0 then
				property_value = item_set:random_property_value(100, 0, 0,
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_min + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_max + 1])
			else
				property_value = item_set:random_property_value(
					GoldSuccinctAttUpChance[effect_block[effect_block_index]],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_des_max + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_des_min + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_min + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_max + 1])
			end
		end

		--保证随机值与当前值的和不超过上下限
		if property_value + cur_property_value < 0 then
			property_value = 0 - cur_property_value
		end

		local max_value = 0
		if #vec_property_max_limit >= effect_block[effect_block_index] then
			max_value = vec_property_max_limit[effect_block[effect_block_index]];
		end
		if property_value + cur_property_value > tonumber(max_value) then
			property_value = tonumber(max_value) - cur_property_value
		end

		temp_item:set_temp_succinct_att(real_index, property_value)
		table.remove(effect_block, effect_block_index)
	end

	if first_use_lock > 0 then
		temp_item:set_data_info(hld.e_item_info_locked, 1)
	end

	if is_auto > 0 then
		item_set:item_save_succinct(temp_player, temp_item)
	else
		local MsgData = { item_guid = {} , property_value = {} }
		table.insert(MsgData.item_guid, item_guid_array[1])
		table.insert(MsgData.item_guid, item_guid_array[2])
		MsgData.is_save_operate = false
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_1))
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_2))
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_3))
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_4))
		MessageMgr:send_message_to_self(temp_player, "hld.item_proto.item_succinct_temp_value", MsgData, 10709)
		item_set_ref:send_item_one(temp_item, true, 0)
	end

	local cur_succinct_num = 0
	for i = hld.e_item_info_succinct_property1, hld.e_item_info_succinct_property6 do
		if temp_item:get_data_info(i) > 0 then
			cur_succinct_num = cur_succinct_num + temp_item:get_data_info(i)
		end
	end
	local Max_succinct_num = 0
	for i = 1, #vec_property_max_limit do
		if tonumber(vec_property_max_limit[i]) > 0 then
			Max_succinct_num = Max_succinct_num + vec_property_max_limit[i]
		end
	end
	local culturing_percent = 0.0
	if Max_succinct_num > 0 then
		culturing_percent = cur_succinct_num / Max_succinct_num
	end

	local item_template_ptr = server_tool_mgr:get_item_template(temp_item);
	hld.server_log:succinct_equip_log(temp_player:get_third_info(), temp_player:get_unit_info_inst(), equip_template_ptr.EquipSlot,
		temp_item:get_data_info(hld.e_item_info_info_id), equip_template_ptr.quality_level, item_template_ptr.item_color, culturing_percent,
		temp_money_cost_typ, temp_money_cost_num, 0, 0, temp_succinct_template.MaterialID, temp_succinct_template.MaterialCost)

	hld.server_log:serverPropUpgrade(temp_player:get_share_log_head(), tostring(temp_item:get_data_info(hld.e_item_info_info_id)), operation_type , tostring(equip_template_ptr.quality_level),tostring(item_template_ptr.item_color), tostring(temp_item:get_data_info(hld.e_item_info_succinct_level)));


	return hld.e_error_code_success
end

--辅助接口
function item_set:get_att_gs(att_index)
	local att_template_ptr = template_manager:get_line("AttributeTemplate", att_index + 20000000)
	if att_template_ptr == nil then
		return 0
	end
	return att_template_ptr.AttGs
end
--辅助接口
function item_set:item_succinct_is_fight_add(temp_item)

	local equip_template_ptr = template_manager:get_line("EquipTemplate", temp_item:get_item_logic_id())
	if nullptr == equip_template_ptr then
		return hld.e_error_code_no_template
	end

	local temp_succinct_template = item_set:get_succinc_info_ptr(temp_item)
	if temp_succinct_template == nil then
		return hld.e_error_code_no_template
	end

	local PropertyID = temp_succinct_template.PropertyID
	local max_index = hld.item_temp_succinct_property_max
	if #(PropertyID) < hld.item_temp_succinct_property_max then
		max_index = #(PropertyID)
	end
	local gs = 0
	for i = 1, max_index do
		local property_idx   = PropertyID[i]
		local property_value = temp_item:get_temp_succinct_att(i - 1)
		gs = gs + item_set:get_att_gs(property_idx) * property_value
	end
	if gs <= 0 then
		return false
	end
	return  true
end
--原来的接口
--int32 item_set::item_succinct_n_times(const guid_64& item_guid, int32 money_cost_type, int32 first_use_lock, int32 is_auto, int32 level, int32 is_use_jewel)
function item_set:item_succinct_n_times(temp_player, item_guid_array, item_slot, operation_type, is_auto, first_use_lock, level, is_use_jewel)

	if temp_player == nil then
		return hld.e_error_code_no_template
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
    end

	local temp_money_cost_typ = 0
	local temp_money_cost_num = 0
	local temp_item = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	if temp_item == nil then
		return hld.e_error_code_no_template
	end

	local is_change = false
	local vec_property_max_limit = item_set:get_property_max_limit(temp_item)

	local equip_template_ptr = template_manager:get_line("EquipTemplate", temp_item:get_item_logic_id())
	if nullptr == equip_template_ptr then
		return hld.e_error_code_no_template
	end

	for i = 0, hld.item_temp_succinct_property_max - 1 do
		temp_item:set_temp_succinct_att(i, 0)
	end

	local temp_succinct_template = item_set:get_succinc_info_ptr(temp_item)
	if temp_succinct_template == nil then
		return hld.e_error_code_no_template
	end

	local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return hld.e_error_code_no_template
	end

	local region_money_type_index = region_template_ptr.RegionCode * 2
	local region_money_num_index  = region_money_type_index + 1

	local cost_type = hld.e_money_type_jewel
	if operation_type == hld.e_item_operation_n_succinct_gold then
		cost_type = hld.e_money_type_silver_bind
	end
	local cost_num = tonumber(temp_succinct_template.DiamondCost)
	if operation_type == hld.e_item_operation_n_succinct_diamond then
		cost_num = tonumber(temp_succinct_template.GoldCost)
	end

	local material_id  = 0
	local material_num = 0

	local GoldPropertyChangeRange = temp_succinct_template.GoldPropertyChangeRange
	local PropertyID = temp_succinct_template.PropertyID
	local DiamondPropertyChangeRange = temp_succinct_template.DiamondPropertyChangeRange

	if #GoldPropertyChangeRange ~= (#PropertyID * hld.item_succinct_change_max) then
		return hld.e_error_code_no_template
	end
	if #DiamondPropertyChangeRange ~= (#PropertyID * hld.item_succinct_change_max) then
		return hld.e_error_code_no_template
	end


	local GoldSuccinctEffectChance = {}
	local GoldSuccinctAttUpChance = {}
	local GoldPropertyChangeRange = {}
	if operation_type == hld.e_item_operation_n_succinct_gold then
		temp_money_cost_typ = hld.e_money_type_silver_bind
		temp_money_cost_num = temp_succinct_template.GoldCost
		GoldSuccinctEffectChance = temp_succinct_template.GoldSuccinctEffectChance
		GoldSuccinctAttUpChance  = temp_succinct_template.GoldSuccinctAttUpChance
		GoldPropertyChangeRange  = temp_succinct_template.GoldPropertyChangeRange
	else
		temp_money_cost_typ = hld.e_money_type_jewel
		temp_money_cost_num = temp_succinct_template.DiamondCost
		GoldSuccinctEffectChance = temp_succinct_template.DiamondSuccinctEffectChance
		GoldSuccinctAttUpChance  = temp_succinct_template.DiamondSuccinctAttUpChance
		GoldPropertyChangeRange  = temp_succinct_template.DiamondPropertyChangeRange
	end

	if not temp_player:can_cut_money(temp_money_cost_typ, temp_money_cost_num) then
		return hld.e_error_code_item_money
	end
		
	if item_set_ref:count_item_by_template_id(temp_succinct_template.MaterialID, hld.e_bag_type_bag, -1) < temp_succinct_template.MaterialCost then
		--没材料
		if is_use_jewel == 0 then
			return hld.e_error_code_item_matirial_not_enough
		else
			local NewPrice = template_manager:get_field("GoodsTemplate", temp_succinct_template.MaterialShopID, "NewPrice")
			material_id =  NewPrice[region_money_type_index + 1]
			material_num = NewPrice[region_money_num_index + 1] * temp_succinct_template.MaterialCost
			is_change = true
		end
	end

	if is_change == true then
		if cost_type == material_id and not temp_player:can_cut_money(cost_type, cost_num + material_num) then
			return hld.e_error_code_item_money
		end
		if cost_type ~= material_id and not temp_player:can_cut_money(material_id, material_num) then
			return hld.e_error_code_item_money
		end
	end

	local effect_block = {}
	--已满的属性不参与随机，也不占用“保底”影响名额,存储可被影响属性的下标
	for i = 1, #vec_property_max_limit do
		if temp_item:get_data_info(i - 1  + hld.e_item_info_succinct_property1) < tonumber(vec_property_max_limit[i]) then
			table.insert(effect_block, i) -- 从1开始的
		end
	end

	if #effect_block <= 0 then
		return hld.e_error_code_item_full_level
	end

	temp_player:cut_money(temp_money_cost_typ, temp_money_cost_num, hld.e_server_log_cut_money_item_succinct, temp_item:get_data_info(hld.e_item_info_info_id), 0)
	if is_change == true then
		temp_player:cut_money(material_id, material_num, hld.e_server_log_cut_money_item_succinct, temp_item:get_data_info(hld.e_item_info_info_id), 0)
	else
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_succinct, 0, temp_succinct_template.MaterialID, temp_succinct_template.MaterialCost, first_use_lock);
	end

	----------经过对比钻石洗练和金币洗练基本上都差不多所以下面使用相同的代码了---------------------------------------------

	local total_chance = 0
	local GoldSuccinctEffectChance = temp_succinct_template.GoldSuccinctEffectChance
	for i = 1, #(GoldSuccinctEffectChance) do
		--确定影响属性数随机总权重
		total_chance = total_chance + GoldSuccinctEffectChance[i]
	end

	local effect_random_value = hld.random_gen:get_random(1, total_chance)
	local effect_num = 1
	total_chance = 0
	for i = 1, #GoldSuccinctEffectChance do
		--确定此次洗练要影响几条属性（条数权重0的不考虑）
		if GoldSuccinctEffectChance[i] == 0 then
			effect_num = effect_num + 1
		else
			total_chance = total_chance + GoldSuccinctEffectChance[i]
			if effect_random_value <= total_chance then
				break
			end
			effect_num = effect_num + 1
		end
	end

	for i = 1, effect_num do
		if #effect_block == 0 then
			break
		end

		local size_effect_block  = #effect_block
		local effect_block_index = hld.random_gen:get_random(1, size_effect_block)
		local real_index = effect_block[effect_block_index] - 1

		local property_value = 0
		local cur_property_value = 0
		
		cur_property_value = temp_item:get_data_info(real_index + hld.e_item_info_succinct_property1)

		--被影响的属性当前值为0时，只能出现上升结果
		if #GoldSuccinctAttUpChance > real_index and #GoldPropertyChangeRange > real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_max then
			if cur_property_value == 0 then
				property_value = item_set:random_property_value(100, 0, 0,
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_min + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_max + 1])
			else
				property_value = item_set:random_property_value(
					GoldSuccinctAttUpChance[effect_block[effect_block_index]],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_des_max + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_des_min + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_min + 1],
					GoldPropertyChangeRange[real_index * hld.item_succinct_change_max + hld.item_succinct_change_ins_max + 1])
			end
		end

		--保证随机值与当前值的和不超过上下限
		if property_value + cur_property_value < 0 then
			property_value = 0 - cur_property_value
		end

		local max_value = 0
		if #vec_property_max_limit >= effect_block[effect_block_index] then
			max_value = vec_property_max_limit[effect_block[effect_block_index]];
		end
		if property_value + cur_property_value > tonumber(max_value) then
			property_value = tonumber(max_value) - cur_property_value
		end

		temp_item:set_temp_succinct_att(real_index, property_value)
		table.remove(effect_block, effect_block_index)
	end

	if first_use_lock > 0 then
		temp_item:set_data_info(hld.e_item_info_locked, 1)
	end

	if item_set:item_succinct_is_fight_add(temp_item) then

		item_set:item_save_succinct(temp_player, temp_item)

		local MsgData = { role_guid = {}, item_guid = {}}
		MsgData.operation_state = hld.e_error_code_success
		MsgData.operation_type = hld.e_item_operation_save_succinct_result
		MsgData.item_slot = 0
		table.insert(MsgData.role_guid, temp_player:get_unit_guid_A())
		table.insert(MsgData.role_guid, temp_player:get_unit_guid_B())
		table.insert(MsgData.item_guid, item_guid_array[1])
		table.insert(MsgData.item_guid, item_guid_array[2])
		MessageMgr:send_message_to_self(temp_player, "hld.item_proto.item_operation_end", MsgData, 10702)
	else
		local MsgData = { item_guid = {} , property_value = {} }
		table.insert(MsgData.item_guid, item_guid_array[1])
		table.insert(MsgData.item_guid, item_guid_array[2])
		MsgData.is_save_operate = false
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_1))
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_2))
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_3))
		table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_4))
		MessageMgr:send_message_to_self(temp_player, "hld.item_proto.item_succinct_temp_value", MsgData, 10709)
		item_set_ref:send_item_one(temp_item, true, 0)
	end

	local cur_succinct_num = 0
	for i = hld.e_item_info_succinct_property1, hld.e_item_info_succinct_property6 do
		if temp_item:get_data_info(i) > 0 then
			cur_succinct_num = cur_succinct_num + temp_item:get_data_info(i)
		end
	end
	local Max_succinct_num = 0
	for i = 1, #vec_property_max_limit do
		if tonumber(vec_property_max_limit[i]) > 0 then
			Max_succinct_num = Max_succinct_num + vec_property_max_limit[i]
		end
	end
	local culturing_percent = 0.0
	if Max_succinct_num > 0 then
		culturing_percent = cur_succinct_num / Max_succinct_num
	end

	local item_template_ptr = server_tool_mgr:get_item_template(temp_item);
	hld.server_log:succinct_equip_log(temp_player:get_third_info(), temp_player:get_unit_info_inst(), equip_template_ptr.EquipSlot,
		temp_item:get_data_info(hld.e_item_info_info_id), equip_template_ptr.quality_level, item_template_ptr.item_color, culturing_percent,
		temp_money_cost_typ, temp_money_cost_num, 0, 0, temp_succinct_template.MaterialID, temp_succinct_template.MaterialCost)


	hld.server_log:serverPropUpgrade(temp_player:get_share_log_head(), tostring(temp_item:get_data_info(hld.e_item_info_info_id)), operation_type , tostring(equip_template_ptr.quality_level),tostring(item_template_ptr.item_color), tostring(temp_item:get_data_info(hld.e_item_info_succinct_level)));

	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_equip_upgrade, 0);
	return hld.e_error_code_success

end

--辅助函数
function item_set:set_equip_succinct_att(player_ref, item_ptr, is_add)

	local succinct_template_ptr = item_set:get_succinc_info_ptr(item_ptr)
	if succinct_template_ptr == nil then
		return 
	end
	
	local PropertyID = succinct_template_ptr.PropertyID
	local max_index = hld.item_temp_succinct_property_max
	if #(PropertyID) < hld.item_temp_succinct_property_max then
		max_index = #(PropertyID)
	end

	for i = 1, max_index do
		local real_item_info_index = hld.e_item_info_succinct_property1 + i - 1
		if real_item_info_index > hld.e_item_info_succinct_property6 then
			break
		end
		player_ref:get_pawn_att():apply_att_change_effect(4.0, PropertyID[i], item_ptr:get_data_info(real_item_info_index), 0.0, 1.0, is_add)
	end

end
function item_set:item_save_succinct_oper(temp_player, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)

	if temp_player == nil then
		return hld.e_error_code_no_template
	end
	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end
	
	local temp_item = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	if temp_item == nil then
		return hld.e_error_code_no_template
	end

	return item_set:item_save_succinct(temp_player, temp_item)
end
function item_set:item_save_succinct(temp_unit, temp_item)

	if temp_unit == nil then
		return hld.e_error_code_no_template
	end
	
	local item_set_ref = temp_unit:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
    end

	if temp_item == nil then
		return hld.e_error_code_no_template
	end

	local item_template_ptr = server_tool_mgr:get_item_template(temp_item)
	if item_template_ptr == nil then
		return hld.e_error_code_no_template
	end

	local temp_succinct_template = item_set:get_succinc_info_ptr(temp_item)
	if temp_succinct_template == nil then
		return hld.e_error_code_no_template
	end

	if item_set_ref:is_equipment_effective(temp_item:get_item_guid()) == true then
		item_set:set_equip_succinct_att(temp_unit, temp_item, false)
		item_set_ref:refresh_succinct_buff(false)
	end

	local vec_property_max_limit = item_set:get_property_max_limit(temp_item)
	local property_size = #vec_property_max_limit
	if property_size > hld.item_temp_succinct_property_max then
		property_size =  hld.item_temp_succinct_property_max
	end

	for i = 0, property_size - 1 do
		local temp_succinct_property_value = 0
		temp_succinct_property_value = temp_item:get_temp_succinct_att(i)

		local cur_succinct_property_value = 0
		cur_succinct_property_value = temp_item:get_data_info(hld.e_item_info_succinct_property1 + i)

		temp_succinct_property_value = temp_succinct_property_value + cur_succinct_property_value
		if temp_succinct_property_value < 0 then
			temp_succinct_property_value = 0
		end
		if temp_succinct_property_value > tonumber(vec_property_max_limit[i + 1]) then
			temp_succinct_property_value = vec_property_max_limit[i + 1]
		end
		temp_item:set_temp_succinct_att(i, 0)
		temp_item:set_data_info(hld.e_item_info_succinct_property1 + i, temp_succinct_property_value)
	end

	item_set_ref:send_item_one(temp_item, true, 0)

	if item_set_ref:is_equipment_effective(temp_item:get_item_guid()) == true then
		item_set:set_equip_succinct_att(temp_unit, temp_item, true)
	end

	local MsgData = {item_guid = {}, property_value={}}
	table.insert(MsgData.item_guid, temp_item:get_item_guid_A())
	table.insert(MsgData.item_guid, temp_item:get_item_guid_B())
	MsgData.is_save_operate = true
	table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_1))
	table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_2))
	table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_3))
	table.insert(MsgData.property_value, temp_item:get_temp_succinct_att(hld.item_temp_succinct_property_4))
	MessageMgr:send_message_to_self(temp_unit, "hld.item_proto.item_succinct_temp_value", MsgData, 10709)

	--if temp_item->is_succinct_full()
	local is_succinct_full = true
	local item_info_check_start = hld.e_item_info_succinct_property1;
	for i = 1, #vec_property_max_limit do
		local real_data_index = item_info_check_start + i - 1
		if real_data_index > hld.e_item_info_succinct_property6 then
			break
		end
		if temp_item:get_data_info(real_data_index) < tonumber(vec_property_max_limit[i]) then
			is_succinct_full = false
		end
	end
	if is_succinct_full == true then
		temp_unit:check_and_set_highest_record(hld.e_role_history_highest_record_item_succinct_reach_mark_level, temp_item:get_item_grade(), item_template_ptr.item_color)
		-- 物品洗练至满级公告
		local notice_id = temp_succinct_template.Noticeld
		if notice_id > 0 then
			local notice_data = {};
			notice_data.notice_id = notice_id
			notice_data.item_data = {}
			table.insert(notice_data.item_data, temp_item:get_item_inst())
			server_tool_mgr:send_notice(temp_unit, notice_data)
		end
	end
	temp_item:set_data_info(hld.e_item_info_succinct_level, temp_item:get_item_succinct_level())

	if item_set_ref:is_equipment_effective(temp_item:get_item_guid()) == true then
		item_set_ref:calcu_addition(hld.e_addition_buff_succinct, true)
		item_set_ref:refresh_succinct_buff(true)
	end

	temp_unit:get_ranking_mgr():set_and_sync_single_equip_ranking_data(temp_item)

	--武器
	if item_template_ptr.item_type == hld.e_item_type_weapon then
		temp_unit:check_and_set_highest_record(hld.e_role_history_highest_record_weapon_full_succinct_grade, 0, 0)
	end
	--装备
	if item_template_ptr.item_type == hld.e_item_type_armor then
		temp_unit:check_and_set_highest_record(hld.e_role_history_highest_record_equip_full_succinct_grade, 0, 0)
	end

	temp_unit:get_mission_mgr():target_check(hld.e_mission_end_type_succinct_level_num,0,0);

	item_set_ref:sync_whole_euip_fighting_power()
	hld.server_log:succinct_equip_save_log(temp_unit:get_third_info(), temp_unit:get_unit_info_inst(), temp_item:get_data_info(hld.e_item_info_info_id))

	return hld.e_error_code_success
end

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--faith::int32 item_set::item_enchant(const guid_64& item_guid, int32 enchant_template_id, int32 first_use_lock)
function item_set:item_enchant(temp_player, item_guid_array, enchant_template_id, operation_type, use_up_item, first_use_lock, item_num, force_do)

	--条件判断 模板数据 是否是下一级 材料足够
	--扣除物品
	--减去原有数值加成 删除所有套装buff
	--增加装备附魔级数
	--加上数值加成 增加所有套装buff
	--返回成功

	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
	end
	
	local item_ptr = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	if item_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr)
	if item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local equip_template_ptr = template_manager:get_line("EquipTemplate", item_ptr:get_item_logic_id())
	if equip_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local enchant_template_ptr = template_manager:get_line("EquipEnchantTemplate", enchant_template_id);
	if enchant_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	--模板数据
	local enchant_num = item_ptr:get_data_info(hld.e_item_info_illusion_had_byte)
	if enchant_num // ItemDef.max_enchant_type_num ~= (enchant_template_ptr.Level - 1) then
		return ConstDefine.e_item_string_unkown
	end

	if enchant_num ~= 0 and enchant_num % ItemDef.max_enchant_type_num ~= enchant_template_ptr.Type then
		return ConstDefine.e_item_string_unkown
	end

	if tonumber(item_ptr:get_data_info(hld.e_item_info_slot) - 1) ~= enchant_template_ptr.Slot then
		return ConstDefine.e_item_string_unkown
	end

	--装备品质
	if item_template_ptr.item_color < hld.e_item_color_purple then
		return ConstDefine.e_item_string_unkown
	end

	if equip_template_ptr.equip_enchant_num < enchant_template_ptr.Level then
		return ConstDefine.e_item_string_unkown
	end

	--材料足够
	local ItemConsume = enchant_template_ptr.ItemConsume
	if #ItemConsume % 2 ~= 0 then
		return ConstDefine.e_item_string_unkown
	end

	for i = 0, #ItemConsume / 2 - 1, 1 do

		if ItemConsume[i * 2 + 1] == 0 then
			break
		end
		if tonumber(ItemConsume[i * 2 + 2]) > item_set_ref:count_item_by_template_id(ItemConsume[i * 2 + 1], hld.e_bag_type_bag, -1) then
			return ConstDefine.e_item_string_matirial_not_enough
		end
	end

	--扣除物品
	for i = 0, #ItemConsume / 2 - 1 do
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_enchant, 0, ItemConsume[i * 2 + 1], ItemConsume[i * 2 + 2], first_use_lock)
	end

	item_set_ref:equip_off_enchant_show_buff()
	--删除数值加成
	local enchant_table = template_manager:get_template("EquipEnchantTemplate")
	if enchant_table == nil then
		return ConstDefine.e_item_string_unkown
	end
	local title_count = 0
	for k , v in pairs(enchant_table) do
		local temp_enchant_template_ptr = v
		if temp_enchant_template_ptr ~= nil then
			if temp_enchant_template_ptr.Level == enchant_template_ptr.Level - 1 and temp_enchant_template_ptr.Type == enchant_template_ptr.Type and temp_enchant_template_ptr.Slot == enchant_template_ptr.Slot then
				pawn_att_mgr:apply_att_change_by_array(temp_player, temp_enchant_template_ptr.AttArray, false, 1.0)
			end
		end
	end
	--增加装备附魔级数
	item_ptr:set_data_info(hld.e_item_info_illusion_had_byte, enchant_template_ptr.Level * ItemDef.max_enchant_type_num + enchant_template_ptr.Type)

	--加上数值加成 增加所有套装buff
	item_set_ref:calcu_addition(hld.e_addition_buff_enchant, true)
	pawn_att_mgr:apply_att_change_by_array(temp_player, enchant_template_ptr.AttArray, true, 1.0)
	
	if item_set_ref:get_equip_smallest_enchant_level_by_type(enchant_template_ptr.Type) > 0 then
		temp_player:set_unit_info(hld.e_role_info_enchant_show_type, enchant_template_ptr.Type);
		temp_player:send_info_one(hld.e_role_info_enchant_show_type);
	end
	
	item_set_ref:equip_on_enchant_show_buff()

	item_set_ref:send_item_one(item_ptr, true, 0)

	temp_player:get_ranking_mgr():set_and_sync_single_equip_ranking_data(item_ptr)

	item_set_ref:sync_whole_euip_fighting_power()
	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_equip_upgrade, 0);
	temp_player:check_and_set_highest_record(hld.e_role_history_highest_record_max_equip_enchant_level, 0, 0)
	temp_player:check_and_set_highest_record(hld.e_role_history_highest_record_max_jewelry_enchant_level, 0, 0)


	--返回成功
	return ConstDefine.e_item_string_succeed
end

--faith::int32 item_set::item_unenchant(const guid_64& item_guid)
function item_set:item_unenchant(temp_player, item_guid_array, enchant_template_id, operation_type, use_up_item, first_use_lock, item_num, force_do)

	--条件判断 是否是附魔物品
	--减去原有数值加成 删除所有套装buff
	--减少装备附魔级数
	--返还物品
	--增加所有套装buff
	--返回成功
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
	end
	
	local item_ptr = item_set_ref:get_item(item_guid_array[1], item_guid_array[2])
	if item_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr)
	if item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local enchant_value = item_ptr:get_data_info(hld.e_item_info_illusion_had_byte)
	if enchant_value <= 0 then
		return ConstDefine.e_item_string_unkown
	end
	local enchant_level = enchant_value // ItemDef.max_enchant_type_num
	local enchant_type  = enchant_value % ItemDef.max_enchant_type_num
	local enchant_slot  = tonumber(item_ptr:get_data_info(hld.e_item_info_slot) - 1)

	local enchant_template_id = -1

	local enchant_table = template_manager:get_template("EquipEnchantTemplate")
	if enchant_table == nil then
		return ConstDefine.e_item_string_unkown
	end
	for k , v in pairs(enchant_table) do
		local temp_enchant_template_ptr = v
		if temp_enchant_template_ptr ~= nil then
			if temp_enchant_template_ptr.Level == enchant_level and temp_enchant_template_ptr.Type == enchant_type and temp_enchant_template_ptr.Slot == enchant_slot then
				enchant_template_id = k
				break
			end
		end
	end

	if enchant_template_id < 0 then
		return ConstDefine.e_item_string_unkown
	end

	local enchant_template_ptr = template_manager:get_line("EquipEnchantTemplate", enchant_template_id)
	if enchant_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local ItemGet = enchant_template_ptr.ItemGet
	if #ItemGet % 2 ~= 0 then
		return ConstDefine.e_item_string_unkown
	end
	
	item_set_ref:equip_off_enchant_show_buff()
	--减去数值加成
	pawn_att_mgr:apply_att_change_by_array(temp_player, enchant_template_ptr.AttArray, false, 1.0)

	--减少装备附魔级数
	item_ptr:set_data_info(hld.e_item_info_illusion_had_byte, 0)
	item_set_ref:send_item_one(item_ptr, true, 0)

	--返还物品
	local data_array = {}
	for i = 0, #ItemGet / 2 -1 do
		local temp_item = item_set_ref:create_item_by_template(hld.e_server_log_add_item_unenchant, 0, ItemGet[i * 2 + 1], ItemGet[i * 2 + 2], 1, 0, 0, 0)
		if temp_item ~= nil then
			item_set_ref:put_item_into_bag(temp_item)
			table.insert(data_array, ItemGet[i * 2 + 1])
			table.insert(data_array, ItemGet[i * 2 + 2])
			table.insert(data_array, 1)
		end
	end
	welfare:get_item_send_promp_msg_to_client(temp_player, data_array, false);
	
	if temp_player:get_unit_info(hld.e_role_info_enchant_show_type) == enchant_template_ptr.Type then
		temp_player:set_unit_info(hld.e_role_info_enchant_show_type, 0)
		temp_player:send_info_one(hld.e_role_info_enchant_show_type);
	end
	--重置所有套装buff
	item_set_ref:calcu_addition(hld.e_addition_buff_enchant, true)
	item_set_ref:equip_on_enchant_show_buff()

	temp_player:get_ranking_mgr():set_and_sync_single_equip_ranking_data(item_ptr)

	item_set_ref:sync_whole_euip_fighting_power()

	return ConstDefine.e_item_string_succeed
end
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

define_C[11402] = function(message_data, player_ref)
	local msg_data = decode("hld.special_name_proto.special_name_operate", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	item_set:operate_special_name(player_ref,msg_data.special_name_type,msg_data.special_name_level,msg_data.operation_type)
end

function item_set:upgrade_sepcial_name(player_ref,name_type, name_level)
	if (name_type < 0) or (name_type >= hld.e_special_name_type_max) or (0 ~= name_type % 2) then
		return false;
	end
	if (player_ref:get_special_name_mgr():get_name_level_by_type(name_type) ~= name_level) then
		return false;
	end
	local m_template_id,special_name_template_ptr = template_manager:get_template_by_type_and_level_and_star(name_type,name_level, player_ref:get_special_name_mgr():get_value_by_id(name_type+1));
	if nil == special_name_template_ptr then
		return false;
	end
	local UpgradeCostList = special_name_template_ptr.UpgradeCost
	local UpgradeCost_num = #(UpgradeCostList);

	if UpgradeCost_num ~= hld.e_item_tuple_max then
		return false;
	end
	--/*该称号是否到达最高级*/
	if special_name_template_ptr.NextLevelId == 0 then
		return false;
	end
	if player_ref:can_cut_money(UpgradeCostList[hld.e_item_tuple_id + 1], UpgradeCostList[hld.e_item_tuple_num + 1]) == false then
		return false;
	end
	player_ref:cut_money(UpgradeCostList[hld.e_item_tuple_id + 1],UpgradeCostList[hld.e_item_tuple_num + 1],hld.e_server_log_cut_money_sepcial_name_upgrade,tonumber(m_template_id),0);

	--修改称号等级
	local next_name_ptr = template_manager:get_line("SpecialNameTemplate", special_name_template_ptr.NextLevelId);
	if next_name_ptr == nil then
		return false;
	end
	hld.buff_man:get_instance():del_buff_by_template_id(player_ref:get_array_index(), player_ref:get_array_index(), tonumber(special_name_template_ptr.BuffLogicId),false);
	hld.buff_man:get_instance():add_buff_inst(player_ref:get_array_index(), player_ref:get_array_index(), next_name_ptr.BuffLogicId,nil,false,-1);
	if (player_ref:get_special_name_mgr():set_value_by_id(name_type, next_name_ptr.NameLevel) == false) then
		return false;
	end
	if (hld.e_special_name_type_acheivement == name_type) then
		if (player_ref:get_special_name_mgr():set_value_by_id(name_type + 1, 0) == false) then
			return false;
		end
		return true;
	elseif (hld.e_special_name_type_military_rank == name_type) then
		if (player_ref:get_special_name_mgr():set_value_by_id(name_type + 1, 1) == false) then
			return false;
		end
		return true;
	end
	return false;
end

function item_set:promote_special_name(player_ref,name_type)
		if ((name_type < 0) or (name_type >= hld.e_special_name_type_max)) then
			return false;
		end
		if (ConstDefine.max_num_of_star <= player_ref:get_special_name_mgr():get_value_by_id(name_type + 1)) then
			return false;
		end
		local m_id,special_name_template_ptr = template_manager:get_template_by_type_and_level_and_star(name_type,player_ref:get_special_name_mgr():get_value_by_id(name_type), player_ref:get_special_name_mgr():get_value_by_id(name_type + 1));
		if (nil == special_name_template_ptr) then
			return false;
		end
		local UpgradeCostList = special_name_template_ptr.UpgradeCost
		local UpgradeCost_num = #(UpgradeCostList);
		if (hld.e_item_tuple_max ~= UpgradeCost_num) then
			return false;
		end
		if (player_ref:can_cut_money(UpgradeCostList[hld.e_item_tuple_id+1], UpgradeCostList[hld.e_item_tuple_num + 1]) == false) then
			return false;
		end
		player_ref:cut_money(UpgradeCostList[hld.e_item_tuple_id+1], UpgradeCostList[hld.e_item_tuple_num + 1], hld.e_server_log_cut_money_sepcial_name_upgrade,tonumber(m_id),0);
		local next_name_ptr = template_manager:get_line("SpecialNameTemplate", special_name_template_ptr.NextLevelId);
		if (next_name_ptr == nil) then
			return false;
		end
		hld.buff_man:get_instance():del_buff_by_template_id(player_ref:get_array_index(), player_ref:get_array_index(), special_name_template_ptr.BuffLogicId,false);
		hld.buff_man:get_instance():add_buff_inst(player_ref:get_array_index(), player_ref:get_array_index(), next_name_ptr.BuffLogicId,nil,false,-1);
		if (player_ref:get_special_name_mgr():set_value_by_id(name_type + 1, next_name_ptr.NameStar) == false) then
			return false;
		end
		return true;
end

function item_set:operate_special_name(player_ref,name_type,name_level,operate_type)
	if 0 > name_type or name_type >= hld.e_special_name_type_max then
		return;
	end
	local bSuccess = false;
	if operate_type == hld.e_special_name_operate_upgrade then
		bSuccess = item_set:upgrade_sepcial_name(player_ref,name_type, name_level)
		if bSuccess == true then
			player_ref:get_special_name_mgr():sync_military_rank_to_ws();
			player_ref:get_achievement_mgr():player_changed(hld.eat_military_rank_check);
		end
	elseif operate_type == hld.e_special_name_operate_promote then
		bSuccess = item_set:promote_special_name(player_ref,name_type);
		if bSuccess == true then
			player_ref:get_special_name_mgr():sync_military_rank_to_ws();
		end
	end
	if bSuccess == true then
		player_ref:get_special_name_mgr():sync_ranking_special_name(name_type);
	end


	player_ref:get_special_name_mgr():send_special_name_operate_end_msg(bSuccess,name_type,operate_type)

	player_ref:refresh_service_goal(hld.e_service_goal_type_soldier_rank_level);
	player_ref:refresh_service_goal(hld.e_service_goal_type_achievement_title);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_arena_military_rank,0,0)
	if name_type == hld.e_special_name_type_acheivement or  name_type == hld.e_special_name_type_acheivement_star then
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs,hld.e_power_up_prompt_raise_achievement,0)
	end
	if name_type == hld.e_special_name_type_military_rank or  name_type == hld.e_special_name_type_military_rank_star then
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs,hld.e_power_up_prompt_raise_rank,0)
	end
	if hld.e_special_name_operate_upgrade ~= operate_type then
		return;
	end
	if (bSuccess) then
		player_ref:get_special_name_mgr():send_special_name_upgrade_notice(name_type);
	end
end

function item_set:set_jewel_on(temp_player, item_guid_array, target_slot, operation_type, use_up_item, first_use_lock, item_num, force_do )
	if target_slot < hld.e_item_info_jewel_slot_0
	 or target_slot > hld.e_item_info_jewel_vip_slot_1 then
		return ConstDefine.e_item_string_unkown;
	 end
	local target_equip_ptr = temp_player:get_item_set():get_item(item_guid_array[1],item_guid_array[2]);
	if target_equip_ptr == nil then
		return ConstDefine.e_item_string_unkown;
	end
	local equip_template_ptr = template_manager:get_line("EquipTemplate",target_equip_ptr:get_item_logic_id())
	if equip_template_ptr == nil then
		return ConstDefine.e_item_string_unkown;
	end
	if target_slot < hld.e_item_info_jewel_vip_slot_0 then
		if hld.init_unit:is_can_use_this_jewel_slot_on_this_equip_level(equip_template_ptr.quality_level, target_slot) == false  then
			return ConstDefine.e_item_string_unkown;
		end
	else
		if hld.init_unit:is_can_use_this_jewel_slot_on_this_vip_level(temp_player:get_vip_level(), target_slot) == false then
			return ConstDefine.e_item_string_unkown;
		end
	end
	local jewel_ptr = temp_player:get_item_set():get_item(item_guid_array[3],item_guid_array[4]);
	if jewel_ptr == nil then
		return ConstDefine.e_item_string_unkown;
	end
	if jewel_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_unkown;
	end
	local jewel_template_ptr = template_manager:get_line("JewelTemplate", jewel_ptr:get_item_logic_id());
	if jewel_template_ptr == nil then
		return ConstDefine.e_item_string_unkown;
	end
	if equip_template_ptr.CanSettingJewelType ~= jewel_template_ptr.JewelType then
		return ConstDefine.e_item_string_unkown;
	end
	local old_jewel_id = target_equip_ptr:get_data_info(target_slot);
	if old_jewel_id > 0 then
		local old_jewel_ptr = temp_player:get_item_set():create_item_by_template(hld.e_server_log_add_item_set_jewel_off,0,old_jewel_id,1,1,0, 0, 0);
		if old_jewel_ptr == nil then
			return ConstDefine.e_item_string_unkown;
		end
		local send_mail_title = hld.globle_data:get_instance():get_mail_common_text(mail_def.e_mail_common_text_jewel_send_mail_title);
		local send_mail_content = hld.globle_data:get_instance():get_mail_common_text(mail_def.e_mail_common_text_jewel_send_mail_content);
		local send_mail_sender_name = hld.globle_data:get_instance():get_mail_common_text(mail_def.e_mail_common_text_sender_system);
		--temp_player:get_item_set():put_item_into_bag(old_jewel_ptr, 0, hld.c_mail_info:new(send_mail_sender_name, send_mail_title, send_mail_content));
		mail_info = hld.c_mail_info:new_local(send_mail_sender_name, send_mail_title, send_mail_content)
		local item_array={}
		table.insert(item_array,old_jewel_ptr)
		welfare:put_item_into_bag(temp_player, item_array,0, mail_info)
	end
	temp_player:get_item_set():set_equip_jewel_att(target_equip_ptr, false);
	target_equip_ptr:set_data_info(target_slot, jewel_ptr:get_data_info(hld.e_item_info_info_id));
	temp_player:get_item_set():set_equip_jewel_att(target_equip_ptr, true);
	temp_player:get_item_set():send_item_one(target_equip_ptr,true,0);
	temp_player:get_ranking_mgr():set_and_sync_single_equip_ranking_data(target_equip_ptr);
	temp_player:get_item_set():del_item(hld.e_server_log_del_item_set_jewel, old_jewel_id, jewel_ptr, 1);
	
	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_inset_genstone,0,0);
	
	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_jewel_set, 0);
	return ConstDefine.e_item_string_succeed;
end

function item_set:spirit_upgrade_promote(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)
	--判断角色是否有效
	if (player_ref == nil)then						
		return hld.e_error_code_no_template;
	end
	
	--判断角色物品管理是否有效
	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end
	
	--判断物品是否有效
	local item_ptr = item_set_ref:get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == item_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--判断物品表格信息是否有效
	if (nil == item_ptr:get_item_info_ptr())then	
		return hld.e_error_code_item_no_item;
	end
	
	--获取物品表格
	local item_tem_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_data_info(hld.e_item_info_info_id));
	if (nil == item_tem_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--判断物品对应精灵表格是否有效
	local spirit_template_id = item_tem_ptr.logic_id + item_ptr:get_data_info(hld.e_item_info_upgrade_count);
	local spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id);
	if (nil == spirit_template_ptr)then				
		return hld.e_error_code_item_no_item;
	end
	
	--判断是否到达升格等级上限
	if spirit_template_ptr.UpgradePromoteLevel >= spirit_template_ptr.UpgradePromoteMaxLevel then
		return hld.e_error_code_item_spirit_promote_max_level;	
	end
	
	--精灵升格材料数组二元组参数1是物品id参数2是数量
	local upgrade_promote_materials = spirit_template_ptr.UpgradePromoteMaterials;
	
	--检查材料是否满足
	local need_item_id_list;
	local need_item_num_list;
	local materials_list_num = #(upgrade_promote_materials);				
	if (materials_list_num >= 2)then
		for i = 1, materials_list_num, 2 do
			local need_item_id = tonumber(upgrade_promote_materials[i]);			--需要物品id
			local need_item_num = tonumber(upgrade_promote_materials[i + 1]);		--需要物品数量
			local have_item_num = item_set_ref:count_star_map_spirit(need_item_id)	--背包中拥有的物品数量
			if have_item_num < need_item_num then									--判断物品数量是否足够
				return hld.e_error_code_item_spirit_promote_not_enough
			end
		end
	end
	
	--检查下一级表格是否有效
	local next_spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id + 1);
	if (nil == next_spirit_template_ptr)then	
		return hld.e_error_code_no_template;
	end
	
	--扣除物品
	if (materials_list_num >= 2)then
		for i = 1, materials_list_num, 2 do
			local item_id = tonumber(upgrade_promote_materials[i]);					--需要物品id
			local item_num = tonumber(upgrade_promote_materials[i + 1]);			--需要物品数量
			item_set_ref:cost_star_map_spirit_item_states(hld.e_server_log_del_item_spirit_promote, 0, item_id, item_num)
		end
	end

	--增加属性
	item_ptr:set_data_info(hld.e_item_info_upgrade_count, next_spirit_template_ptr.UpgradePromoteLevel)	--设置精灵升格等级
	item_set_ref:refresh_spirit_att(spirit_template_id, tonumber(spirit_template_id + 1));		--刷新属性

	--刷新物品信息
	item_set_ref:send_item_one(item_ptr, true, 0);
	
	--检查羁绊
	player_ref:get_spirit_mgr():check_star_map();
	
	--同步精灵战力
	item_set_ref:sync_ranking_spirit();
	
	return hld.e_error_code_success;
end

function item_set:break_will_activate(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)
	--判断角色是否有效
	if (player_ref == nil)then						
		return hld.e_error_code_no_template;
	end
	
	--判断角色物品管理是否有效
	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end
	
	--判断物品是否有效
	local item_ptr = item_set_ref:get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == item_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--判断物品是否激活
	local is_activate = item_ptr:get_data_info(hld.e_item_info_activate)
	if (is_activate == 1)then						
		return hld.e_error_code_item_break_will_active;
	end
	
	--判断物品表格信息是否有效
	if (nil == item_ptr:get_item_info_ptr())then	
		return hld.e_error_code_item_no_item;
	end
	
	--获取物品表格
	local item_tem_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_data_info(hld.e_item_info_info_id));
	if (nil == item_tem_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--获取心魔意志属性表格
	local break_will_ptr = template_manager:get_line("BreakWillTemplate", item_tem_ptr["logic_id"]);
	if (nil == break_will_ptr)then				
		return hld.e_error_code_item_no_item;
	end
	
	if break_will_ptr.UnLockLevel > player_ref:get_grade_num() * 5 then
		return hld.e_error_code_item_break_will_no_level;
	end
	
	--获取消耗物品列表
	local consume_item_list = break_will_ptr.NeedItemId;
	
	--检查材料是否满足
	local need_item_id_list;
	local need_item_num_list;
	local materials_list_num = #(consume_item_list);				
	if (materials_list_num >= 2)then
		for i = 1, materials_list_num, 2 do
			local need_item_id = tonumber(consume_item_list[i]);				--需要物品id
			local need_item_num = tonumber(consume_item_list[i + 1]);			--需要物品数量
			local have_item_num = item_set_ref:count_item_by_template_id(need_item_id, hld.e_bag_type_bag, -1) 	--背包中拥有的物品数量
			if have_item_num < need_item_num then									--判断物品数量是否足够
				return hld.e_error_code_item_matirial_not_enough
			end
		end
	end
	
	--扣除物品
	if (materials_list_num >= 2)then
		for i = 1, materials_list_num, 2 do
			local need_item_id2 = tonumber(consume_item_list[i]);					--需要物品id
			local need_item_num2 = tonumber(consume_item_list[i + 1]);			--需要物品数量
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_break_will, item_tem_ptr["logic_id"], need_item_id2, need_item_num2, 1)
		end
	end
	
	--设置物品激活
	item_ptr:set_data_info(hld.e_item_info_activate, 1)
	--增加属性
	pawn_att_mgr:apply_att_change_by_array(player_ref, break_will_ptr.AttArray, true, 1.0)
	--刷新技能
	item_set_ref:refresh_break_will_skill_by_id(break_will_ptr["MainTemId"]);
	--刷新物品信息
	item_set_ref:send_item_one(item_ptr, true, 0);
	--刷新任务
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_active_break_will,0,0);
	return hld.e_error_code_success;
end


function item_set:awaken_fetter_activate(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)
	--判断角色是否有效
	if (player_ref == nil)then						
		return hld.e_error_code_no_template;
	end

	--判断角色物品管理是否有效
	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end
	
	--判断物品是否有效
	local item_ptr = item_set_ref:get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == item_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--获取物品表格
	local item_tem_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_data_info(hld.e_item_info_info_id));
	if (nil == item_tem_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--获取当前装备觉醒总等级
	local equip_level = item_set_ref:get_equip_num_awaken_level();
	
	--获取当前觉醒羁绊等级
	local cur_level = item_ptr:get_data_info(hld.e_item_info_upgrade_count);
	
	--设置下一级
	local next_level = cur_level + 1;
	
	--获取表格
	local template_ptr = template_manager:get_line("AwakenFetterTemplate", tonumber(item_tem_ptr.logic_id) + cur_level);
	if (nil == template_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	--判断等级是否满足
	if template_ptr.NeedLevel > equip_level or next_level > template_ptr["MaxNum"] then
		return hld.e_error_code_item_awaken_fetter_no_level;
	end
	
	-- 关闭觉醒Buff
	player_ref:get_item_set():equip_off_awaken_show_buff()
	
	--增加属性
	pawn_att_mgr:apply_att_change_by_array(player_ref, template_ptr.AttArray, true, 1.0)
	
	--增加等级
	item_ptr:set_data_info(hld.e_item_info_upgrade_count, next_level)
	
	if template_ptr.Type == 1 and #template_ptr.UnLockSkillList > 0 then
		--获取技能表格
		local skill_template_ptr = template_manager:get_line("AwakenFetterTemplate", template_ptr.UnLockSkillList[1]);
		if nil ~= skill_template_ptr then						
			--设置被动技能等级
			item_ptr:set_data_info(hld.e_item_info_random_property1 + skill_template_ptr["Index"], 1)
			player_ref:get_passive_skill():passive_skill_special(skill_template_ptr.UnLockSkillList[1], true)
		end
	end
	
	-- 刷新觉醒Buff
    player_ref:get_item_set():equip_on_awaken_show_buff()
    
	-- 刷新最高级
    player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_max_awaken_level, 0, 0)

	--刷新物品信息
	item_set_ref:send_item_one(item_ptr, true, 0);
	return hld.e_error_code_success;
end

function item_set:awaken_fetter_skill_upgrade(player_ref, item_guid_array, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do)
	
	--判断角色是否有效
	if (player_ref == nil)then						
		return hld.e_error_code_no_template;
	end
	
	--判断角色物品管理是否有效
	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end

	--判断物品是否有效
	local item_ptr = item_set_ref:get_item(item_guid_array[1], item_guid_array[2]);
	if (nil == item_ptr)then						
		return hld.e_error_code_item_no_item;
	end

	--获取物品表格
	local item_tem_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_data_info(hld.e_item_info_info_id));
	if (nil == item_tem_ptr)then						
		return hld.e_error_code_item_no_item;
	end
	
	local skill_template_id = 0;
	for i = 0, item_ptr:get_data_info(hld.e_item_info_upgrade_count) - 1 do
		--获取表格
		local tem_template_ptr = template_manager:get_line("AwakenFetterTemplate", tonumber(item_tem_ptr.logic_id) + i);
		if nil ~= tem_template_ptr then						
			if tem_template_ptr["SubIndex"] == item_slot then						
				skill_template_id = tonumber(item_tem_ptr.logic_id) + i;
				break
			end
		end
	end
	
	if skill_template_id == 0 then
		return hld.e_error_code_item_no_item;
	end
	
	--获取当前觉醒羁绊等级
	local cur_level = item_ptr:get_data_info(hld.e_item_info_random_property1 + item_slot);
	
	--设置下一级
	local next_level = cur_level + 1;
	
	--获取表格
	local template_ptr = template_manager:get_line("AwakenFetterTemplate", skill_template_id);
	if (nil == template_ptr)then						
		return hld.e_error_code_item_no_item;
	end

	--获取技能表格
	local skill_template_ptr = template_manager:get_line("AwakenFetterTemplate", template_ptr.UnLockSkillList[next_level]);
	if nil == skill_template_ptr then						
		return hld.e_error_code_item_no_item;
	end

	--获取消耗物品列表
	local consume_item_list = skill_template_ptr.NeedItem;
	local need_item_id = tonumber(consume_item_list[1]);				--需要物品id
	local need_item_num = tonumber(consume_item_list[2]);				--需要物品数量
	
	--背包中拥有的物品数量
	local have_item_num = item_set_ref:count_item_by_template_id(need_item_id, hld.e_bag_type_bag, -1)
	
	--判断物品数量是否足够
	if have_item_num < need_item_num then									
		return hld.e_error_code_item_matirial_not_enough
	end
			
	print("zym del_od = ", need_item_id)
	print("zym del_num = ", need_item_num)		
	--扣除物品
	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_awaken_skill, 0, need_item_id, need_item_num, 1)
	
	--增加等级
	item_ptr:set_data_info(hld.e_item_info_random_property1 + item_slot, next_level)
	
	--设置被动技能
	player_ref:get_passive_skill():passive_skill_special(skill_template_ptr.UnLockSkillList[1], true)
	
	--刷新物品信息
	item_set_ref:send_item_one(item_ptr, true, 0);
	return hld.e_error_code_success;
end

item_set_mgr = item_set
return item_set_mgr






