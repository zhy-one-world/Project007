local feather_heart_class = {}

--[[
函数说明: 	初始化
--]]
function feather_heart_class:init()
end
--[[
函数说明: 	获取ItemTemplate配置
--]]
function feather_heart_class:get_item_template_info(template_id)
	return template_manager:get_line("ItemTemplate", template_id)
end

--[[
函数说明: 	获取GodHeartTemplate配置
--]]
function feather_heart_class:get_feather_heart_template_by_template_id(template_id)

	local item_ptr = template_manager:get_line("ItemTemplate", template_id)
	if item_ptr == nil or item_ptr.logic_id == nil then
		return {}
	end
	return template_manager:get_line("GodHeartTemplate", item_ptr.logic_id)
end

--[[
函数说明: 	获取GodHeartUpgradeTemplate配置
--]]
function feather_heart_class:get_feather_heart_upgrade_tempalte_by_template_id(template_id)
	return template_manager:get_line("GodHeartUpgradeTemplate", template_id)
end

--[[
函数说明: 	获取GodHeartUpgradeTemplate配置
--]]
function feather_heart_class:get_feather_heart_upgrade_tempalte_by_item(p_item)
	
	local template_id = p_item:get_item_template_id()
	local equip_template_ptr = feather_heart_class:get_item_template_info(template_id)
	if equip_template_ptr == nil then
		return nil
	end

	local template_level = p_item:get_data_info(hld.e_item_info_upgrade_count)

	local god_template_ptr = feather_heart_class:get_feather_heart_template_by_template_id(template_id)
	if god_template_ptr == nil then
		return nil
	end

	return template_manager:get_line("GodHeartUpgradeTemplate", god_template_ptr.UpgradeId + template_level)
end

--[[
函数说明: 	获取核心提升普通等级
--]]
function feather_heart_class:get_slot_add_level_by_template_id(core_template_id, core_template_level)

	local god_ptr = feather_heart_class:get_feather_heart_template_by_template_id(core_template_id)
	if god_ptr == nil or god_ptr.ChangeLevel == nil then
		return 0;
	end

	local add_level = 0;

	local ChangeLevelTbl = god_ptr.ChangeLevel
	local num = #(ChangeLevelTbl)

	if num == 0 or num % 3 ~= 0 then
		return add_level;
	end

	for i = 1,num,3 do
		local start_level = tonumber(ChangeLevelTbl[i + 0]);
		local end_level   = tonumber(ChangeLevelTbl[i + 1]);
		if start_level <= core_template_level and core_template_level <= end_level then
			add_level = ChangeLevelTbl[i + 2];
			break;
		end
	end
	
	return add_level;
end

--[[
函数说明: 	获取核心提升普通等级
--]]
function feather_heart_class:get_slot_add_level_by_item(item_ptr)
	if item_ptr == nil then
		return 0
	end
	local core_template_id    = item_ptr:get_data_info(FeatherDef.feather_slot_start + FeatherDef.e_feather_heart_3 * 2)
	local core_template_level = item_ptr:get_data_info(FeatherDef.feather_slot_start + FeatherDef.e_feather_heart_3 * 2 + 1)

	return feather_heart_class:get_slot_add_level_by_template_id(core_template_id, core_template_level + 1)
end
------------------------------------------------------------------------------------------------------------------------

--[[
函数说明: 	神翎雕文强化
--]]
function feather_heart_class:feather_heart_slot_grade(player_ref, s_item_guid, slot)
	if slot < FeatherDef.e_feather_heart_0 or slot > FeatherDef.e_feather_heart_3 then
		return FeatherDef.feather_heart_error_unknow
	end
	if slot == FeatherDef.e_feather_heart_3 then
		return feather_heart_class:feather_heart_kernel_grade(player_ref, s_item_guid)
	end
	return feather_heart_class:feather_heart_grade(player_ref, s_item_guid, slot)
end

--[[
函数说明: 	神翎雕文核心强化
--]]
function feather_heart_class:feather_heart_kernel_grade(player_ref, s_item_guid)

	if player_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local feather_heart_mgr_ptr = player_ref:get_feather_heart_mgr()
	if feather_heart_mgr_ptr == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local item_ptr = item_set_ref:get_item(tostring(s_item_guid))
	if item_ptr == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if item_ptr:get_data_info(hld.e_item_info_activate) == 0 then
		return FeatherDef.feather_heart_error_unknow
	end

	if item_ptr:get_item_info_ptr() == nil then
        return FeatherDef.feather_heart_error_unknow
	end

	local equip_template_ptr = feather_heart_class:get_item_template_info(item_ptr:get_item_template_id())

	if equip_template_ptr.item_type ~= hld.e_item_type_wing or equip_template_ptr.sub_type ~= hld.e_wing_sub_type_feather then
		return FeatherDef.feather_heart_error_unknow
	end

	local template_id    = item_ptr:get_data_info(FeatherDef.feather_slot_start + FeatherDef.e_feather_heart_3 * 2)
	local template_level = item_ptr:get_data_info(FeatherDef.feather_slot_start + FeatherDef.e_feather_heart_3 * 2 + 1)

	local god_template_ptr = feather_heart_class:get_feather_heart_template_by_template_id(template_id)
	if god_template_ptr == nil or god_template_ptr == {} then
		return FeatherDef.feather_heart_error_unknow
	end

	if template_level >= god_template_ptr.MaxUpgradeLevel then
		return FeatherDef.feather_heart_error_max_upgrade
	end

	local p_grade_template = feather_heart_class:get_feather_heart_upgrade_tempalte_by_template_id(god_template_ptr.UpgradeId + template_level)
	if p_grade_template == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local UpgradeCostTbl = p_grade_template.UpgradeCost
	if UpgradeCostTbl == nil or #(UpgradeCostTbl) % 2 ~= 0 then
		return FeatherDef.feather_heart_error_unknow
	end

	for i=1,#(UpgradeCostTbl),2 do
        if not player_ref:can_cut_money(tonumber(UpgradeCostTbl[i]), tonumber(UpgradeCostTbl[i+1])) then
            return FeatherDef.feather_heart_error_not_money
        end
	end

	for i=1,#(UpgradeCostTbl),2 do
		player_ref:cut_money(tonumber(UpgradeCostTbl[i]), tonumber(UpgradeCostTbl[i+1]), hld.e_server_log_cut_money_feather_heart_upgrade, item_ptr:get_data_info(hld.e_item_info_info_id), 0)
	end

	feather_heart_mgr_ptr:set_feather_heart_attr_all(item_ptr, false)

	item_ptr:set_data_info(FeatherDef.feather_slot_start + FeatherDef.e_feather_heart_3 * 2 + 1, template_level + 1)

	feather_heart_mgr_ptr:set_feather_heart_attr_all(item_ptr, true);

	item_set_ref:send_item_one(item_ptr,true,0)

	feather_heart_mgr_ptr:sync_ranking_feather_heart_data()

	return FeatherDef.feather_heart_error_sucess
end

--[[
函数说明: 	神翎雕文普通强化
--]]
function feather_heart_class:feather_heart_grade(player_ref, s_item_guid, slot)

	if player_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local feather_heart_mgr_ptr = player_ref:get_feather_heart_mgr()
	if feather_heart_mgr_ptr == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if slot < FeatherDef.e_feather_heart_0 or slot > FeatherDef.e_feather_heart_2 then
		return FeatherDef.feather_heart_error_unknow
	end

	local p_main_item = item_set_ref:get_item(tostring(s_item_guid))
	if p_main_item == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if p_main_item:get_data_info(hld.e_item_info_activate) == 0 then
		return FeatherDef.feather_heart_error_unknow
	end

	if p_main_item:get_item_info_ptr() == nil then
        return FeatherDef.feather_heart_error_unknow
	end

	local equip_template_id  = p_main_item:get_item_template_id()
	local equip_template_ptr = feather_heart_class:get_item_template_info(equip_template_id)
	if equip_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end
	if equip_template_ptr.item_type ~= hld.e_item_type_wing or equip_template_ptr.sub_type ~= hld.e_wing_sub_type_feather then
		return FeatherDef.feather_heart_error_unknow
	end

	local template_id    = p_main_item:get_data_info(FeatherDef.feather_slot_start + slot * 2)
	local template_level = p_main_item:get_data_info(FeatherDef.feather_slot_start + slot * 2 + 1)

	local god_template_ptr = feather_heart_class:get_feather_heart_template_by_template_id(template_id)
	if god_template_ptr == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if template_level >= god_template_ptr.MaxUpgradeLevel then
		return FeatherDef.feather_heart_error_max_upgrade
	end

	local p_grade_template = feather_heart_class:get_feather_heart_upgrade_tempalte_by_template_id(god_template_ptr.UpgradeId + template_level)
	if p_grade_template == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local UpgradeCostTbl = p_grade_template.UpgradeCost
	if UpgradeCostTbl == nil or #(UpgradeCostTbl) % 2 ~= 0 then
		return FeatherDef.feather_heart_error_unknow
	end

	for i=1,#(UpgradeCostTbl),2 do
		if not player_ref:can_cut_money(tonumber(UpgradeCostTbl[i]), tonumber(UpgradeCostTbl[i+1])) then
            return FeatherDef.feather_heart_error_not_money
        end
	end

	for i=1,#(UpgradeCostTbl),2 do
		player_ref:cut_money(tonumber(UpgradeCostTbl[i]), tonumber(UpgradeCostTbl[i+1]), hld.e_server_log_cut_money_feather_heart_upgrade, p_main_item:get_data_info(hld.e_item_info_info_id), 0)
	end

	local core_add_level = feather_heart_class:get_slot_add_level_by_item(p_main_item)

	feather_heart_mgr_ptr:set_feather_heart_attr(template_id, template_level + core_add_level, false)

	feather_heart_mgr_ptr:dec_buff_att(template_id, template_level, core_add_level)

	p_main_item:set_data_info(FeatherDef.feather_slot_start + slot * 2 + 1, template_level + 1)

	feather_heart_mgr_ptr:set_feather_heart_attr(template_id, template_level + core_add_level + 1, true)

	feather_heart_mgr_ptr:add_buff_att(template_id, template_level + 1, core_add_level)

	item_set_ref:send_item_one(p_main_item,true,0)

	feather_heart_mgr_ptr:sync_ranking_feather_heart_data()

	return FeatherDef.feather_heart_error_sucess
end

--[[
函数说明: 	神翎雕文装备
--]]
function feather_heart_class:feather_heart_equip_on(player_ref, s_item_guid, slot, d_item_guid)

	if player_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local feather_heart_mgr_ptr = player_ref:get_feather_heart_mgr()
	if feather_heart_mgr_ptr == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if slot < FeatherDef.e_feather_heart_0 or slot > FeatherDef.e_feather_heart_3 then
		return FeatherDef.feather_heart_error_unknow
	end

	local p_main_item = item_set_ref:get_item(tostring(s_item_guid))
	if p_main_item == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if p_main_item:get_data_info(hld.e_item_info_activate) == 0 then
		return FeatherDef.feather_heart_error_unknow
	end

	if p_main_item:get_item_info_ptr() == nil then
        return FeatherDef.feather_heart_error_unknow
	end

	local equip_template_ptr = feather_heart_class:get_item_template_info(p_main_item:get_item_template_id())
	if equip_template_ptr.item_type ~= hld.e_item_type_wing or equip_template_ptr.sub_type ~= hld.e_wing_sub_type_feather then
		return FeatherDef.feather_heart_error_unknow
	end

	local p_skill_item = item_set_ref:get_item(tostring(d_item_guid))
	if p_skill_item == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	if p_skill_item:get_item_info_ptr() == nil then
        return FeatherDef.feather_heart_error_unknow
	end

	local skill_item_template_id  = p_skill_item:get_item_template_id()
	local skill_item_template_ptr = feather_heart_class:get_item_template_info(skill_item_template_id)
	if skill_item_template_ptr.item_type ~= hld.e_item_type_skill_book then
		return FeatherDef.feather_heart_error_unknow
	end

	local god_heart_ptr = feather_heart_class:get_feather_heart_template_by_template_id(skill_item_template_id)
	if god_heart_ptr == nil then
		return FeatherDef.feather_heart_error_unknow;
	end

	if god_heart_ptr.ItemType ~= p_main_item:get_item_template_id() then
		return FeatherDef.feather_heart_error_equip_on_type;
	end

	if slot == FeatherDef.e_feather_heart_3 and skill_item_template_ptr.sub_type ~= FeatherDef.e_feather_heart_max then
		return FeatherDef.feather_heart_error_equip_on_slot_type;
	end
	if slot ~= FeatherDef.e_feather_heart_3 and skill_item_template_ptr.sub_type ~= FeatherDef.e_feather_heart_0 then
		return FeatherDef.feather_heart_error_equip_on_slot_type;
	end

	local old_template_id    = p_main_item:get_data_info(FeatherDef.feather_slot_start + slot * 2)
	local old_template_level = p_main_item:get_data_info(FeatherDef.feather_slot_start + slot * 2 + 1)


	feather_heart_mgr_ptr:set_feather_heart_attr_all(p_main_item, false);


	local old_add_level = feather_heart_class:get_slot_add_level_by_item(p_main_item)

	if old_template_id ~= 0 then
		feather_heart_mgr_ptr:put_feather_heart_in_bag(old_template_id, old_template_level)
	end

	local new_template_id    = p_skill_item:get_data_info(hld.e_item_info_info_id)
	local new_template_level = p_skill_item:get_data_info(hld.e_item_info_upgrade_count)

	p_main_item:set_data_info(FeatherDef.feather_slot_start + slot * 2, new_template_id)
	p_main_item:set_data_info(FeatherDef.feather_slot_start + slot * 2 + 1, new_template_level)

	feather_heart_mgr_ptr:set_feather_heart_attr_all(p_main_item, true)
		
	item_set_ref:del_item(hld.e_server_log_del_item_set_skill_book, 0, p_skill_item , 1)
	
	feather_heart_mgr_ptr:sync_ranking_feather_heart_data()

	item_set_ref:send_item_one(p_main_item,true,0)

	return FeatherDef.feather_heart_error_sucess
end

--[[
函数说明: 	神翎雕文分解
--]]
function feather_heart_class:feather_heart_bag_piece(player_ref, feather_heart_guid, is_use_diamond)

	if player_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local processed_map = {}
	local money_reward_array = {}
	local del_array     = {}

	local cost_money_id  = 0
    local cost_money_num = 0

	for k, v  in pairs(feather_heart_guid) do
	
		if processed_map[v] == nil then
			processed_map[v] = 1

        	local p_skill_item = player_ref:get_item_set():get_item(tostring(v))
			if p_skill_item ~= nil then

				local p_grade_template = feather_heart_class:get_feather_heart_upgrade_tempalte_by_item(p_skill_item)
				local RecoveryRewardTbl = p_grade_template.RecoveryReward
				if p_grade_template ~= nil and #(RecoveryRewardTbl) % 2 == 0 then

					local DiamondRecoveryCostTbl = p_grade_template.DiamondRecoveryCost
					local DiamondRecoveryMoneyTbl = p_grade_template.DiamondRecoveryMoney
					if is_use_diamond and DiamondRecoveryCostTbl ~= nil and DiamondRecoveryMoneyTbl ~= nil and #(DiamondRecoveryCostTbl) >= 2 then
						if #(DiamondRecoveryCostTbl) % 2 == 0 then
							local money_id  = tonumber(DiamondRecoveryMoneyTbl[1])
							local money_num = tonumber(DiamondRecoveryMoneyTbl[2])
				
							cost_money_id  = tonumber(DiamondRecoveryCostTbl[1])
							cost_money_num = tonumber(DiamondRecoveryCostTbl[2])

							if money_reward_array[money_id] == nil then
								money_reward_array[money_id] = money_num
							else
								money_reward_array[money_id] = money_reward_array[money_id] + money_num
							end
							table.insert(del_array, v)
						end
					else
						local count = #(RecoveryRewardTbl) /  2 * 2
						for i = 1, count, 2 do
							local temp_money_id = tonumber(RecoveryRewardTbl[i])
							local temp_money_value = tonumber(RecoveryRewardTbl[i + 1])

							if money_reward_array[temp_money_id] == nil then
								money_reward_array[temp_money_id] = temp_money_value
							else
								money_reward_array[temp_money_id] = money_reward_array[temp_money_id] + temp_money_value
							end
						end
						table.insert(del_array, v)
					end
				end
			end
		end
	end
	
	if is_use_diamond > 0 and player_ref:can_cut_money(cost_money_id, cost_money_num) then
		return FeatherDef.feather_heart_error_not_money
	end

	if is_use_diamond and cost_money_num > 0 then
		player_ref:cut_money(cost_money_id, cost_money_num, hld.e_server_log_cut_money_skill_book_diamond_recovery, -1, 0)
	end

	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
		return FeatherDef.feather_heart_error_unknow
	end

	local bag_type = 0;
	for k,v in pairs(del_array) do
		local del_item_ptr = item_set_ref:get_item(tostring(v))
		if del_item_ptr ~= nil then
			bag_type = del_item_ptr:get_data_info(hld.e_item_info_container_type)
		end
		item_set_ref:del_item(hld.e_server_log_del_item_item_sell, 0, del_item_ptr, 1)
	end

	for k,v in pairs(money_reward_array) do
		player_ref:add_money_or_exp_with_string(tonumber(k), tostring(v), hld.e_server_log_add_money_skill_book_recovery, 0)
	end
	
	item_set_ref:tidy_item(bag_type)
	if bag_type == 2 then
		local operate_end_msg = {};
		operate_end_msg.role_guid = {}
		table.insert(operate_end_msg.role_guid, player_ref:get_unit_guid_A());
		table.insert(operate_end_msg.role_guid, player_ref:get_unit_guid_B());
		operate_end_msg.operation_type = 17
		operate_end_msg.operation_state = 0
		MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_operation_end",operate_end_msg, 10702)
	end
	return FeatherDef.feather_heart_error_sucess
end

-------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 	神翎解锁
--]]
function feather_heart_class:wing_feather_unlock(unit_array_index, wing_guid, item_slot)

	local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end
	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_ptr = item_set_ref:get_item(tostring(wing_guid))
	if item_ptr == nil then
		return ConstDefine.e_item_string_no_item
	end

	if item_ptr:get_data_info(hld.e_item_info_activate) > 0 then
		return ConstDefine.e_item_string_unkown
	end

	if item_ptr:get_item_info_ptr() == nil then
        return ConstDefine.e_item_string_unkown
	end
	local item_template_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id())
	if item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local feather_template_ptr = template_manager:get_line("FeatherTemplate", item_template_ptr.logic_id)
	if feather_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local unlock_need_material_id  = feather_template_ptr.UnlockMaterialId
	local unlock_need_material_num = feather_template_ptr.UnlockMaterialNum

	local cur_owned_material_num  = item_set_ref:count_item_by_template_id(unlock_need_material_id, hld.e_bag_type_bag, hld.e_item_locked)
	if cur_owned_material_num < unlock_need_material_num then
		return ConstDefine.e_item_string_matirial_not_enough
	end

	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_feather_unlock, 0, unlock_need_material_id, unlock_need_material_num, hld.e_item_locked);

	item_set_ref:set_wing_att(false);

	item_ptr:set_data_info(hld.e_item_info_activate, 1);

	item_set_ref:set_wing_att(true);

	if feather_template_ptr.SkillID > 0 then
		local OpenItemTbl =  template_manager:get_field("PassiveSkillTemplate", feather_template_ptr.SkillID, "OpenItem")
		if OpenItemTbl ~= nil and #(OpenItemTbl) < 2 then
			local upgrade_count = item_ptr:get_data_info(hld.e_item_info_upgrade_count)
			item_set_ref:star_skill_unlock(wing_guid, upgrade_count, false, false);
		end
	end

	item_set_ref:send_item_one(item_ptr,true,0)

	local feather_heart_mgr_ptr = player_ref:get_feather_heart_mgr()
	if feather_heart_mgr_ptr ~= nil then
		feather_heart_mgr_ptr:sync_ranking_feather_heart_data()
	end

	return hld.e_error_code_success;
end

--[[
函数说明: 	神翎升级
--]]
function feather_heart_class:feather_upgrade(unit_array_index, feather_guid, use_up_item)

	local temp_player = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end
	
	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local feather_ptr = item_set_ref:get_item(tostring(feather_guid))
	if feather_ptr == nil then
		return ConstDefine.e_item_string_no_item
	end

	if feather_ptr:get_data_info(hld.e_item_info_activate) ~= 1 then
		return ConstDefine.e_item_string_unkown
	end

	local equip_template_ptr = feather_heart_class:get_item_template_info(feather_ptr:get_item_template_id())
	if equip_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	if equip_template_ptr.item_type ~= hld.e_item_type_wing then
		return ConstDefine.e_item_string_unkown
	end

	if equip_template_ptr.sub_type ~= hld.e_wing_sub_type_feather then
		return ConstDefine.e_item_string_unkown
	end

	local region_template_ptr = template_manager:get_line("RegionTemplate", 150000000)
	if region_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local region_money_type_index = region_template_ptr.RegionCode * 2
	local region_money_num_index  = region_money_type_index + 1

	local now_feather_count   = feather_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local real_id             = equip_template_ptr.logic_id + now_feather_count
	local feather_upgrade_ptr = template_manager:get_line("FeatherTemplate", real_id)
	if feather_upgrade_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local cur_star_num  = feather_upgrade_ptr.AddStarNow;
	local max_star_num  = feather_upgrade_ptr.AddStarMax;
	local cur_level_num = feather_upgrade_ptr.UpgradeNow;
	local max_level_num = feather_upgrade_ptr.UpgradeMax;

	if cur_star_num == max_star_num and cur_level_num == max_level_num then
		return ConstDefine.e_item_string_full_level
	end

	local next_feather_upgrade_ptr = template_manager:get_line("FeatherTemplate", real_id + 1)
	if next_feather_upgrade_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local is_use_jewel_instead_matirial = false

	if cur_star_num == max_star_num then
		if item_set_ref:count_item_by_template_id(feather_upgrade_ptr.UpgradeMatirialId, hld.e_bag_type_bag, hld.e_item_locked) < feather_upgrade_ptr.UpgradeMatirialNum then
			if use_up_item == 0 then
				return ConstDefine.e_item_string_matirial_not_enough
			else			
				is_use_jewel_instead_matirial = true
			end
		end

		if is_use_jewel_instead_matirial == false then
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_feather_add_star , 0, feather_upgrade_ptr.UpgradeMatirialId, feather_upgrade_ptr.UpgradeMatirialNum, hld.e_item_locked);
		else
			local NewPriceTbl =  template_manager:get_field("GoodsTemplate", feather_upgrade_ptr.UpgradeShopId, "NewPrice")
			if NewPriceTbl ~= nil and #(NewPriceTbl) > region_money_num_index then
				local money_id  = NewPriceTbl[region_money_type_index];
				local money_num = NewPriceTbl[region_money_num_index] * feather_upgrade_ptr.UpgradeMatirialNum;
				if not temp_player:can_cut_money(money_id, money_num) then
					return ConstDefine.e_item_string_auto_use_money_not_enough
				end
				temp_player:cut_money(money_id, money_num, hld.e_server_log_cut_money_feather_upgrade_auto_buy, feather_upgrade_ptr.UpgradeShopId, 0)
			else
				return ConstDefine.e_item_string_no_item;
			end
		end

		item_set_ref:set_feather_att(feather_ptr, false)

		feather_ptr:set_data_info(hld.e_item_info_upgrade_count, now_feather_count + 1)

		item_set_ref:send_item_one(feather_ptr, true, 0)

		item_set_ref:set_feather_att(feather_ptr, true)

		item_set_ref:calcu_addition(hld.e_addition_buff_feather, true)

		temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_wing_upgrade, 0 ,0);
		--temp_player:target_check(hld.e_mission_end_type_wing_upgrade, 0 ,0)

		temp_player:get_achievement_mgr():item_changed(feather_ptr, hld.e_item_operation_featherupgrade_star)
		--temp_player:item_changed(feather_ptr, hld.e_item_operation_featherupgrade_star)

	else

		if item_set_ref:count_item_by_template_id(feather_upgrade_ptr.AddStarMatirialId, hld.e_bag_type_bag, hld.e_item_locked) < feather_upgrade_ptr.AddStarMatirialNum then
			if use_up_item == 0 then
				return ConstDefine.e_item_string_matirial_not_enough
			else
				is_use_jewel_instead_matirial = true
			end
		end

		if is_use_jewel_instead_matirial == false then
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_feather_add_star, 0, feather_upgrade_ptr.AddStarMatirialId, feather_upgrade_ptr.AddStarMatirialNum, hld.e_item_locked)
		else
			local NewPriceTbl =  template_manager:get_field("GoodsTemplate", feather_upgrade_ptr.StarShopId, "NewPrice")
			if NewPriceTbl ~= nil and #(NewPriceTbl) > region_money_num_index then
				local money_id  = NewPriceTbl[region_money_type_index]
				local money_num = NewPriceTbl[region_money_num_index] * feather_upgrade_ptr.AddStarMatirialNum
				if not temp_player:can_cut_money(money_id, money_num) then
						return ConstDefine.e_item_string_auto_use_money_not_enough
				end
				temp_player:cut_money(money_id, money_num, hld.e_server_log_cut_money_feather_add_star_auto_buy, feather_upgrade_ptr.AddStarMatirialNum)
			else
				return ConstDefine.e_item_string_no_item
			end
		end

		item_set_ref:set_feather_att(feather_ptr, false)

		feather_ptr:set_data_info(hld.e_item_info_upgrade_count, now_feather_count + 1)

		item_set_ref:send_item_one(feather_ptr, true, 0)

		item_set_ref:set_feather_att(feather_ptr, true)
	end

	if next_feather_upgrade_ptr.SkillID > 0 then
		local OpenItemTbl =  template_manager:get_field("PassiveSkillTemplate", next_feather_upgrade_ptr.SkillID, "OpenItem")
		if OpenItemTbl ~= nil and #(OpenItemTbl) < 2 then
			local upgrade_count = feather_ptr:get_data_info(hld.e_item_info_upgrade_count)
			item_set_ref:star_skill_unlock(feather_guid, upgrade_count, false, false);
		end
	end
			
	local feather_heart_mgr_ptr = temp_player:get_feather_heart_mgr()
	if feather_heart_mgr_ptr ~= nil then
		feather_heart_mgr_ptr:sync_ranking_feather_heart_data()
	end

	return hld.e_error_code_success
end
---------------------------------------------------------------------------------------------------------------------------------------
define_C[10749] = function(message_data, player_ref)

	local msg_data = decode("hld.feather_heart_proto.feather_heart_operate", message_data)
	feather_heart_class:deal_with_feather_oper_msg(msg_data, player_ref);

end

--[[
函数说明: 	处理神翎雕文消息
--]]
function feather_heart_class:deal_with_feather_oper_msg(message_data, player_ref)

	local temp_item_vec = {}

	local tmp_guid_string = ""
	local count = #(message_data.item_guid)
	for i = 1, count, 2 do
		tmp_guid_string = hld.guid_transfer_to_string(message_data["item_guid"][i], message_data["item_guid"][i+1])
		table.insert(temp_item_vec, tmp_guid_string)
	end

	local item_skill = hld.guid_transfer_to_string(message_data["skill_a"], message_data["skill_b"])

	feather_heart_class:feather_oper(player_ref, temp_item_vec, message_data["oper_type"],  message_data["slot"], item_skill);
end

--[[
函数说明: 处理神翎雕文消息子接口
--]]
function feather_heart_class:feather_oper(player_ref, s_item_guid, oper_type, slot, d_item_guid)
	if player_ref == nil then
		return
	end

	local result = 0

	if oper_type == FeatherDef.e_feather_heart_oper_grade then
		result = feather_heart_class:feather_heart_slot_grade(player_ref, s_item_guid[1], slot)
	elseif oper_type == FeatherDef.e_feather_heart_oper_equip_on then
		result = feather_heart_class:feather_heart_equip_on(player_ref, s_item_guid[1], slot, d_item_guid)
	elseif oper_type == FeatherDef.e_feather_heart_oper_piece then
		result = feather_heart_class:feather_heart_bag_piece(player_ref, s_item_guid, slot)
	end

	local tmp_guid_a = hld.get_guid_A(s_item_guid[1])
	local tmp_guid_b = hld.get_guid_B(s_item_guid[1])
	local MsgData = 
	{
		item_guid = {tonumber(tmp_guid_a),tonumber(tmp_guid_b)},
		oper_type = oper_type,
		slot      = slot,
		result    = result,
	}

	MessageMgr:send_message_to_self(player_ref, "hld.feather_heart_proto.feather_heart_operate_end", MsgData, 10750)
end
----------------------------------------------------------------------------------------------------------------------------------------

feather_heart = feather_heart_class
return feather_heart