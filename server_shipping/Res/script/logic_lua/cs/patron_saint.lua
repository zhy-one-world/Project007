local patron_saint_class = {}
---------------------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 	初始化
--]]
function patron_saint_class:init()
end

--[[
函数说明: 	获取配置
--]]
function patron_saint_class:get_template_from_ProtectSpiritTemplate(item)
	if item == nil then
		return nil
	end

	local template_id = item:get_item_template_id()
	local item_template_ptr = template_manager:get_line("ItemTemplate", template_id)
	if item_template_ptr == nil then
		return nil
	end

	local template_level = item:get_data_info(hld.e_item_info_upgrade_count)

	return template_manager:get_line("ProtectSpiritTemplate", item_template_ptr.logic_id + template_level)
end

--[[
函数说明:	获取配置中的固定字段
--]]
function patron_saint_class:get_upgrade_cost_array_from_ProtectSpiritTemplate(item)
	if item == nil then
		return nil
	end

	local template_id = item:get_item_template_id()
	local item_template_ptr = template_manager:get_line("ItemTemplate", template_id)
	if item_template_ptr == nil then
		return nil
	end

	local template_level = item:get_data_info(hld.e_item_info_upgrade_count)

	return template_manager:get_field("ProtectSpiritTemplate", item_template_ptr.logic_id + template_level, "UpgradeCostArray")
end

--[[
函数说明:	获取配置中的固定字段
--]]
function patron_saint_class:get_skill_id_from_ProtectSpiritTemplate(item)
	if item == nil then
		return nil
	end

	local template_id = item:get_item_template_id()
	local item_template_ptr = template_manager:get_line("ItemTemplate", template_id)
	if item_template_ptr == nil then
		return nil
	end

	local template_level = item:get_data_info(hld.e_item_info_upgrade_count)

	return template_manager:get_field("ProtectSpiritTemplate", item_template_ptr.logic_id + template_level, "SkillId")
end

--[[
函数说明:	
--]]
function patron_saint_class:get_tinder_star_already_used(player_ref)

	local already_used_tinder_star = 0

	local item_set_ref = player_ref:get_item_set()

	for i = 0, ItemDef.max_proctect_mark_equip_slot_num -1, 1 do
		local temp_mark_ptr = item_set_ref:get_item_by_slot(hld.e_bag_type_equip_protect, i)
		if temp_mark_ptr ~= nil then
			local temp_mark_template_ptr = patron_saint_class:get_template_from_ProtectSpiritTemplate(temp_mark_ptr)
			if temp_mark_template_ptr ~= nil then
				already_used_tinder_star = already_used_tinder_star + temp_mark_template_ptr.NeedStarLevel
			end
		end
	end

	return already_used_tinder_star
end

--[[
函数说明:	
--]]
function patron_saint_class:send_tinder_info_one(player_ref, info_index)

	if player_ref == nil then
		return 
	end

	local value = player_ref:get_patron_saint_mgr():get_tinder_info(info_index)
	local MsgData = 
	{
		info_index = info_index,
		info_value = value,
	}
	MessageMgr:send_message_to_self(player_ref, "hld.pokedex_proto.update_tinder_info_one", MsgData, 11513)
end
-----------------------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 	
--]]
function patron_saint_class:purchase_mark_slot(temp_player)
	local GameConfigTemplate = server_tool_mgr:get_game_config_template_ptr()
	local protect_mark_slot_purchase_array = GameConfigTemplate.ProtectMarkSlotPurchaseArray
	if protect_mark_slot_purchase_array == nil then
		return ConstDefine.e_item_string_unkown
	end

	if  #(protect_mark_slot_purchase_array)  < ItemDef.max_proctect_mark_equip_slot_num then
		return ConstDefine.e_item_string_unkown
	end

	local cur_slot_open_num = temp_player:get_unit_info(hld.e_role_info_protect_mark_slot_open_num)
	if cur_slot_open_num >= ItemDef.max_proctect_mark_equip_slot_num then
		return 0
	end

	local tmp_value = temp_player:get_patron_saint_mgr():get_tinder_info(hld.e_tinder_info_total_star_level)
	if tonumber(protect_mark_slot_purchase_array[cur_slot_open_num + 1]) > tmp_value then
		return ConstDefine.e_item_string_unkown
	end

	
	temp_player:set_unit_info(hld.e_role_info_protect_mark_slot_open_num, cur_slot_open_num + 1)
	temp_player:send_info_one(hld.e_role_info_protect_mark_slot_open_num)

	return ConstDefine.e_item_string_succeed;
end

--[[
函数说明: 	激活技能
--]]
function patron_saint_class:mark_activate(temp_player, mark_guid)

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local target_mark = item_set_ref:get_item(tostring(mark_guid))
	if target_mark == nil then
		return ConstDefine.e_item_string_no_item
	end

	if target_mark:get_data_info(hld.e_item_info_activate) > 0 then
		return ConstDefine.e_item_string_unkown
	end

	local upgrade_cost_array = patron_saint_class:get_upgrade_cost_array_from_ProtectSpiritTemplate(target_mark)
	if upgrade_cost_array == nil or #(upgrade_cost_array) <= 0 or #(upgrade_cost_array) % 2 ~= 0  then
		return ConstDefine.e_item_string_unkown
	end

	if item_set_ref:count_item_by_template_id(tonumber(upgrade_cost_array[1]), hld.e_bag_type_bag, -1) < tonumber(upgrade_cost_array[2]) then
		return ConstDefine.e_item_string_unkown
	end

	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_activate_tinder_skill,0, tonumber(upgrade_cost_array[1]), tonumber(upgrade_cost_array[2]), hld.e_item_locked)
		
	target_mark:set_data_info(hld.e_item_info_activate, 1)

	item_set_ref:send_item_one(target_mark, true, 0)

	temp_player:get_achievement_mgr():player_changed(hld.eat_patron_saint_activation_num_count)

	local skill_id = patron_saint_class:get_skill_id_from_ProtectSpiritTemplate(target_mark)
	temp_player:get_passive_skill():passive_skill_special(skill_id, false)
	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_tinder_skill, 0);
	return ConstDefine.e_item_string_succeed
end

--[[
函数说明: 	装备解锁的火种技能
--]]
function patron_saint_class:mark_equip(temp_player, mark_guid, target_slot)

	local patron_saint_mgr = temp_player:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local new_slot_mark = item_set_ref:get_item(tostring(mark_guid))
	if new_slot_mark == nil then
		return ConstDefine.e_item_string_no_item
	end

	if new_slot_mark:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_protect or new_slot_mark:get_data_info(hld.e_item_info_activate) <= 0 then
		return ConstDefine.e_item_string_unkown
	end

	local cur_equip_slot_open_num = temp_player:get_unit_info(hld.e_role_info_protect_mark_slot_open_num)
	if cur_equip_slot_open_num > ItemDef.max_proctect_mark_equip_slot_num then
		cur_equip_slot_open_num = ItemDef.max_proctect_mark_equip_slot_num
	end

	if target_slot >= cur_equip_slot_open_num then
		return ConstDefine.e_item_string_unkown
	end

	if target_slot < 0 then

		target_slot = item_set_ref:get_empty_slot(hld.e_bag_type_equip_protect)

		--TODO这里待测试
		if item_set_ref:get_item_by_slot(hld.e_bag_type_equip_protect, target_slot) ~= nil then
			return ConstDefine.e_item_string_unkown
		end
	end

	if target_slot < 0 then
		return ConstDefine.e_item_tinder_skill_slot_not_enough
	end

	local mark_item_template_ptr = template_manager:get_line("ItemTemplate", new_slot_mark:get_item_template_id())
	if mark_item_template_ptr == nil or mark_item_template_ptr.item_type ~= hld.e_item_type_protect then
		return ConstDefine.e_item_string_unkown
	end

	local new_mark_template_ptr = patron_saint_class:get_template_from_ProtectSpiritTemplate(new_slot_mark)
	if new_mark_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end


	local already_used_tinder_star = patron_saint_class:get_tinder_star_already_used(temp_player)

	local old_slot_mark = item_set_ref:get_item_by_slot(hld.e_bag_type_equip_protect, target_slot)
	if old_slot_mark ~= nil then
		local old_mark_template_ptr = patron_saint_class:get_template_from_ProtectSpiritTemplate(old_slot_mark)
		local total_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_total_star_level)
		local add_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_add_star_level)
		if old_mark_template_ptr == nullptr or total_star_level + add_star_level < already_used_tinder_star - old_mark_template_ptr.NeedStarLevel + new_mark_template_ptr.NeedStarLevel then
			return ConstDefine.e_item_tinder_star_level_not_enough
		end

		local old_mark_go_to_slot = new_slot_mark:get_data_info(hld.e_item_info_slot)

		old_slot_mark.set_data_info(hld.e_item_info_container_type, hld.e_bag_type_protect)

		old_slot_mark.set_data_info(hld.e_item_info_slot, old_mark_go_to_slot)

		item_set_ref:set_target_slot_guid(hld.e_bag_type_protect, old_mark_go_to_slot, old_slot_mark:get_item_guid())

		temp_player:get_passive_skill():passive_skill_special(old_mark_template_ptr.SkillId, false)

		item_set_ref:send_item_one(old_slot_mark, true, 0)
	else
		local total_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_total_star_level)
		local add_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_add_star_level)
		if total_star_level + add_star_level < already_used_tinder_star + new_mark_template_ptr.NeedStarLevel then
			return ConstDefine.e_item_tinder_star_level_not_enough
		end
		item_set_ref:clear_target_slot(hld.e_bag_type_protect, new_slot_mark:get_data_info(hld.e_item_info_slot))
	end

	new_slot_mark:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_equip_protect)

	new_slot_mark:set_data_info(hld.e_item_info_slot, target_slot)

	item_set_ref:set_target_slot_guid(hld.e_bag_type_equip_protect, target_slot, new_slot_mark:get_item_guid())

	temp_player:get_passive_skill():passive_skill_special(new_mark_template_ptr.SkillId, true)

	item_set_ref:send_item_one(new_slot_mark, true, 0)
		
	return ConstDefine.e_item_string_succeed
end

--[[
函数说明: 	卸下火种技能
--]]
function patron_saint_class:mark_unequip(temp_player, mark_guid)

	local patron_saint_mgr = temp_player:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local target_mark = item_set_ref:get_item(tostring(mark_guid))
	if target_mark == nil then
		return ConstDefine.e_item_string_no_item
	end

	if target_mark:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_equip_protect then
		return ConstDefine.e_item_string_unkown
	end

	local target_mark_template_ptr = patron_saint_class:get_template_from_ProtectSpiritTemplate(target_mark)
	if target_mark_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end
	
	local target_operate_slot = item_set_ref:get_empty_slot(hld.e_bag_type_protect)
	if target_operate_slot < 0 or target_operate_slot >= ItemDef.max_proctect_mark_bag_slot_num then
		return ConstDefine.e_item_string_unkown
	end
	
	item_set_ref:clear_target_slot(hld.e_bag_type_equip_protect, target_mark:get_data_info(hld.e_item_info_slot))

	target_mark:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_protect)

	target_mark:set_data_info(hld.e_item_info_slot, target_operate_slot)

	item_set_ref:set_target_slot_guid(hld.e_bag_type_protect, target_operate_slot, target_mark:get_item_guid())

	temp_player:get_passive_skill():passive_skill_special(target_mark_template_ptr.SkillId, false)

	item_set_ref:send_item_one(target_mark, true, 0);

	return ConstDefine.e_item_string_succeed;
end

--[[
函数说明: 	
--]]
function patron_saint_class:purchase_prop_slot(temp_player)

	local patron_saint_mgr = temp_player:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local tempalte_id = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_current_unlock_template_id)
	local tinder_template_ptr_unlock = template_manager:get_line("TinderTemplate", tempalte_id)

	if tinder_template_ptr_unlock == nil then
		return ConstDefine.e_item_string_unkown
	end

	local cur_unlock_order = tinder_template_ptr_unlock.CurrentOrder
	local cur_equip_slot_open_num = cur_unlock_order / 6

	if cur_unlock_order > PokedexDef.tinder_old_num_max then
			cur_equip_slot_open_num = PokedexDef.tinder_item_max
	end
	
	local cur_slot_open_num = temp_player:get_unit_info(hld.e_role_info_tinder_prop_slot_open_num)
	if cur_slot_open_num >= cur_equip_slot_open_num then
		return ConstDefine.e_item_string_unkown
	end

	temp_player:set_unit_info(hld.e_role_info_tinder_prop_slot_open_num, cur_slot_open_num + 1)

	temp_player:send_info_one(hld.e_role_info_tinder_prop_slot_open_num)

	return ConstDefine.e_item_string_succeed
end

--[[
函数说明: 	
--]]
function patron_saint_class:prop_equip(temp_player, mark_guid, target_slot)

	local patron_saint_mgr = temp_player:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local new_slot_mark = item_set_ref:get_item(tostring(mark_guid))
	if new_slot_mark == nil then
		return ConstDefine.e_item_string_no_item
	end

	if new_slot_mark:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_bag then
		return ConstDefine.e_item_string_unkown
	end		

	local template_id = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_current_unlock_template_id)
	local tinder_template_ptr_unlock = template_manager:get_line("TinderTemplate", template_id)
	if tinder_template_ptr_unlock == nil then
		return ConstDefine.e_item_string_unkown;
	end

	local cur_unlock_order = tinder_template_ptr_unlock.CurrentOrder
	local cur_equip_slot_open_num = cur_unlock_order / 6

	if cur_unlock_order > PokedexDef.tinder_old_num_max then
			cur_equip_slot_open_num = PokedexDef.tinder_item_max
	end

	

	if target_slot >= cur_equip_slot_open_num then
		return ConstDefine.e_item_string_unkown
	end

	if target_slot < 0 then
		return ConstDefine.e_item_tinder_skill_slot_not_enough
	end

	local mark_item_template_ptr = template_manager:get_line("ItemTemplate", new_slot_mark:get_item_template_id())
	if mark_item_template_ptr == nil or mark_item_template_ptr.item_type ~= hld.e_item_type_stone or mark_item_template_ptr.sub_type ~= hld.e_stone_sub_type_tinder_prop then
		return ConstDefine.e_item_string_unkown
	end

	if mark_item_template_ptr.item_type ~= hld.e_item_type_stone or mark_item_template_ptr.sub_type ~= hld.e_stone_sub_type_tinder_prop then
		return ConstDefine.e_item_string_unkown
	end

	local old_slot_mark = item_set_ref:get_item_by_slot(hld.e_bag_type_equip_tinder_prop, target_slot)
	if old_slot_mark ~= nil then

		local old_item_template_ptr = template_manager:get_line("ItemTemplate", old_slot_mark:get_item_template_id())
		if old_item_template_ptr == nil then
			return ConstDefine.e_item_string_unkown
		end

		local already_used_tinder_star = patron_saint_class:get_tinder_star_already_used(temp_player)
		local total_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_total_star_level)
		local add_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_add_star_level)
		if (already_used_tinder_star > total_star_level + add_star_level - old_item_template_ptr.FashionId + mark_item_template_ptr.FashionId) then
			return ConstDefine.e_item_string_tinder_point_not_enough
		end

		local old_mark_go_to_slot = new_slot_mark:get_data_info(hld.e_item_info_slot)

		old_slot_mark:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_bag)

		old_slot_mark:set_data_info(hld.e_item_info_slot, old_mark_go_to_slot)

		item_set_ref:set_target_slot_guid(hld.e_bag_type_bag, old_mark_go_to_slot, old_slot_mark:get_item_guid())

		item_set_ref:send_item_one(old_slot_mark, true, 0)

		patron_saint_mgr:set_tinder_info(hld.e_tinder_info_add_star_level, add_star_level - old_item_template_ptr.FashionId)
	else

		item_set_ref:clear_target_slot(hld.e_bag_type_bag, new_slot_mark:get_data_info(hld.e_item_info_slot))

	end

	local tmp_add_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_add_star_level)
	patron_saint_mgr:set_tinder_info(hld.e_tinder_info_add_star_level, tmp_add_star_level + mark_item_template_ptr.FashionId)

	new_slot_mark:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_equip_tinder_prop)

	new_slot_mark:set_data_info(hld.e_item_info_slot, target_slot)

	item_set_ref:set_target_slot_guid(hld.e_bag_type_equip_tinder_prop, target_slot, new_slot_mark:get_item_guid())

	item_set_ref:send_item_one(new_slot_mark, true, 0)

	patron_saint_class:send_tinder_info_one(temp_player, hld.e_tinder_info_add_star_level)

	return ConstDefine.e_item_string_succeed
end

--[[
函数说明: 	
--]]
function patron_saint_class:prop_unequip(temp_player, mark_guid)

	local patron_saint_mgr = temp_player:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return ConstDefine.e_item_string_unkown
	end

	local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local new_slot_mark = item_set_ref:get_item(tostring(mark_guid))
	if new_slot_mark == nil then
		return ConstDefine.e_item_string_no_item
	end

	if new_slot_mark:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_equip_tinder_prop then
		return ConstDefine.e_item_string_unkown;
	end

	local mark_item_template_ptr = template_manager:get_line("ItemTemplate", new_slot_mark:get_item_template_id())
	if mark_item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
	end

	if mark_item_template_ptr.item_type ~= hld.e_item_type_stone or mark_item_template_ptr.sub_type ~= hld.e_stone_sub_type_tinder_prop then
		return ConstDefine.e_item_string_unkown
	end

	local target_operate_slot = item_set_ref:get_empty_slot(hld.e_bag_type_bag)
	if target_operate_slot < 0 or target_operate_slot >= ItemDef.MAX_ITEM_BAG_NUM then
		return ConstDefine.e_item_string_unkown
	end

	local already_used_tinder_star = patron_saint_class:get_tinder_star_already_used(temp_player)
	local total_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_total_star_level)
	local add_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_add_star_level)
	if already_used_tinder_star > total_star_level + add_star_level - mark_item_template_ptr.FashionId then
		return ConstDefine.e_item_string_tinder_point_not_enough;
	end

	local tmp_add_star_level = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_add_star_level)
	patron_saint_mgr:set_tinder_info(hld.e_tinder_info_add_star_level, tmp_add_star_level - mark_item_template_ptr.FashionId)

	item_set_ref:clear_target_slot(hld.e_bag_type_equip_tinder_prop, new_slot_mark:get_data_info(hld.e_item_info_slot))

	new_slot_mark:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_bag)

	new_slot_mark:set_data_info(hld.e_item_info_slot, target_operate_slot)
	
	item_set_ref:set_target_slot_guid(hld.e_bag_type_bag, target_operate_slot, new_slot_mark:get_item_guid())
		
	item_set_ref:send_item_one(new_slot_mark, true, 0)

	patron_saint_class:send_tinder_info_one(temp_player, hld.e_tinder_info_add_star_level)

	return ConstDefine.e_item_string_succeed;
end

--[[
函数说明: 	
--]]
function patron_saint_class:set_unit_skill_cpp(unit_array_index)
	local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return 0
	end
	return patron_saint_class:set_unit_skill(player_ref)
end
function patron_saint_class:set_unit_skill(player_ref)

	local patron_saint_mgr = player_ref:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return 0
	end
	local GameConfigTemplate = server_tool_mgr:get_game_config_template_ptr()
	local protect_mark_slot_purchase_array = GameConfigTemplate.PatronSaintSkillStartId

	if protect_mark_slot_purchase_array == nil or #(protect_mark_slot_purchase_array) <= player_ref:get_unit_info(hld.e_role_info_class_type) then
			return 0
	end

	local skill_set_ptr = hld.get_skill_set_by_index(player_ref:get_array_index())
	for i = 2, #(protect_mark_slot_purchase_array), 1  do

		local skill_ptr = skill_set_ptr:get_skill_inst_by_id(protect_mark_slot_purchase_array[i])
		if skill_ptr == nil then
			return 0
		end
		local tempalte_id = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_current_unlock_template_id)
		local tinder_template_ptr_unlock = template_manager:get_line("TinderTemplate", tempalte_id)
		if tinder_template_ptr_unlock == nil then
			return 0
		end
		local current_order = tinder_template_ptr_unlock.CurrentOrder
		if skill_ptr:get_data_info(hld.e_skill_info_activated) <= 0 and (current_order >= PokedexDef.tinder_order_num_one_canto * (i - 1) or current_order == PokedexDef.tinder_new_order_max) then

			skill_set_ptr:activate_skill(protect_mark_slot_purchase_array[i], true)
			return 1
		end
	end
	return 0
end

--[[
函数说明: 	
--]]
function patron_saint_class:tinder_upgrade(player_ref)

	local patron_saint_mgr = player_ref:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return 0
	end

	local old_template_id = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_current_level_template_id)
	local new_template_id = old_template_id + 1
	if old_template_id <= 0 then
		new_template_id = PokedexDef.tinder_level_start_id
	end
	local GameConfigTemplate = server_tool_mgr:get_game_config_template_ptr()
	local tinder_max_level = GameConfigTemplate.TinderMaxLevel
	if #(tinder_max_level) <= 0 or new_template_id >= PokedexDef.tinder_level_start_id + tinder_max_level[1] then
		return 0
	end

	local unlock_template_id = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_current_unlock_template_id)

	local unlock_current_order  = template_manager:get_field("TinderTemplate", unlock_template_id, "CurrentOrder")
	local upgrade_current_order = template_manager:get_field("TinderTemplate", new_template_id, "CurrentOrder")
	if tonumber(unlock_current_order) < tonumber(upgrade_current_order) then
		return 0
	end
		
	local new_tinder_template_ptr = template_manager:get_line("TinderTemplate", new_template_id)
	if new_tinder_template_ptr == nil then
		return 0
	end

	local money_consume = new_tinder_template_ptr.MoneyConsume
	if #(money_consume) <= 0 or #(money_consume) % 2 ~= 0 then
		return 0
	end

	local mission_template_ptr = template_manager:get_line("MissionTemplate", player_ref:get_mission_id(hld.e_mission_slot_main))
	if mission_template_ptr == nil then
		return 0
	end
	
	if new_tinder_template_ptr.NeedChapter > mission_template_ptr.CurChapterIndex and new_template_id <= PokedexDef.tinder_old_end_id then
		return 0
	end

	local money_req_num = #(money_consume)
	for i = 1, money_req_num, 2 do
		local cost_money_id  = money_consume[i]
		local cost_money_num = money_consume[i + 1]
		if not player_ref:can_cut_money(tonumber(cost_money_id), tonumber(cost_money_num)) then
			return 0
		end
	end

	for i = 1, money_req_num, 2 do
		local cost_money_id  = money_consume[i]
		local cost_money_num = money_consume[i + 1]
		player_ref:cut_money(tonumber(cost_money_id), tonumber(cost_money_num), hld.e_server_log_cut_money_patron_saint_tinder_upgrade, new_template_id, 0)
	end

	local cur_tinder_template_ptr = template_manager:get_line("TinderTemplate", old_template_id)
	if cur_tinder_template_ptr ~= nil then
		pawn_att_mgr:apply_att_change_by_array(player_ref, cur_tinder_template_ptr.AddAtt, false, 1)
	end

	pawn_att_mgr:apply_att_change_by_array(player_ref, new_tinder_template_ptr.AddAtt, true, 1)

	patron_saint_mgr:set_tinder_info(hld.e_tinder_info_current_level_template_id, new_template_id)

	local old_value = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_total_star_level)
	patron_saint_mgr:set_tinder_info(hld.e_tinder_info_total_star_level, old_value + new_tinder_template_ptr.AddStarLevel)

	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_tinder_total_level, 0 ,0)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_add_tinder, 0);
	player_ref:get_achievement_mgr():player_changed(hld.eat_patron_saint_grade_num_count);

	patron_saint_mgr:sync_tinder_data()

	patron_saint_class:send_tinder_info_one(player_ref, hld.e_tinder_info_current_level_template_id)
	patron_saint_class:send_tinder_info_one(player_ref, hld.e_tinder_info_total_star_level)

	return 1
end

--[[
函数说明: 	火种激活
--]]
function patron_saint_class:tinder_unlock(player_ref)

	local patron_saint_mgr = player_ref:get_patron_saint_mgr()
	if patron_saint_mgr == nil then
		return 0
	end

	local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

	local mission_template_ptr = template_manager:get_line("MissionTemplate", player_ref:get_mission_id(hld.e_mission_slot_main))
	if mission_template_ptr == nil then
		return 0
	end

	local old_template_id = patron_saint_mgr:get_tinder_info(hld.e_tinder_info_current_unlock_template_id)
	local new_template_id = PokedexDef.tinder_unlock_start_id
	if old_template_id > 0 then
		new_template_id = old_template_id + 1;
	end
		
	local new_tinder_template_ptr = template_manager:get_line("TinderTemplate", new_template_id)
	if new_tinder_template_ptr == nil then
		return 0
	end

	local attribute_id = new_template_id
	if new_tinder_template_ptr.NeedChapter > mission_template_ptr.CurChapterIndex and attribute_id <= PokedexDef.tinder_old_end_id then
		return 0
	end

	local money_consume = new_tinder_template_ptr.MoneyConsume
	local need_matiral = #(money_consume)

	local is_material = false
	if attribute_id <= PokedexDef.tinder_old_end_id then
		is_material = true
	else
		if need_matiral == 0 or need_matiral % 2 ~= 0 then
			is_material = false
		else
			is_material = true
			for i = 1, need_matiral, 2 do
				local matiral_id = money_consume[i];
				local matiral_num = money_consume[i + 1];
				if item_set_ref:count_item_by_template_id(matiral_id, hld.e_bag_type_bag, -1) < tonumber(matiral_num) then
					is_material = false
				end
			end
		end
	end

	if is_material == false then
		return 0
	end
	local GameConfigTemplate = server_tool_mgr:get_game_config_template_ptr()
	local tinder_max_level =  GameConfigTemplate.TinderMaxLevel
	if tinder_max_level == nil or #(tinder_max_level) <= 1 or new_template_id >= PokedexDef.tinder_unlock_start_id + tinder_max_level[2] then
		return 0
	end

	if attribute_id > PokedexDef.tinder_old_end_id then
		for i = 1, need_matiral, 2 do
			local matiral_id = money_consume[i]
			local matiral_num = money_consume[i + 1]
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_new_tinder_active, 0, matiral_id, matiral_num, hld.e_item_locked)
		end
	end

	local cur_tinder_template_ptr = template_manager:get_line("TinderTemplate", old_template_id)
	if cur_tinder_template_ptr ~= nil then
		pawn_att_mgr:apply_att_change_by_array(player_ref, cur_tinder_template_ptr.AddAtt, false, 1)
	end

	pawn_att_mgr:apply_att_change_by_array(player_ref, new_tinder_template_ptr.AddAtt, true, 1)

	patron_saint_mgr:set_tinder_info(hld.e_tinder_info_current_unlock_template_id, new_template_id)

	patron_saint_class:send_tinder_info_one(player_ref, hld.e_tinder_info_current_unlock_template_id)

	return 1
end
--------------------------------------------------------------------------------------------------------------------------------------------------

define_C[10730] = function(message_data, player_ref)

	local msg_data = decode("hld.item_proto.patron_saint_mark_operate", message_data)

	patron_saint_class:mark_operate(player_ref, msg_data.mark_guid_a, msg_data.mark_guid_b, msg_data.target_slot, msg_data.operate_type)

end

function patron_saint_class:mark_operate(temp_player, mark_guid_a, mark_guid_b, target_slot, operate_type)

	local mark_guid = hld.guid_transfer_to_string(mark_guid_a, mark_guid_b)
	local operate_result = ConstDefine.e_item_string_unkown

	if operate_type == hld.e_patron_saint_mark_operate_type_purchase_new_slot then
		operate_result = patron_saint_class:purchase_mark_slot(temp_player)

	elseif operate_type == hld.e_patron_saint_mark_operate_type_equip_mark then
		operate_result = patron_saint_class:mark_equip(temp_player, mark_guid, target_slot)

	elseif operate_type == hld.e_patron_saint_mark_operate_type_unequip_mark then
		operate_result = patron_saint_class:mark_unequip(temp_player, mark_guid)

	elseif operate_type == hld.e_patron_saint_mark_operate_type_upgrade then	
		operate_result = patron_saint_class:mark_activate(temp_player, mark_guid)

	end

	local MsgData = 
	{
		mark_guid_a = mark_guid_a,
		mark_guid_b = mark_guid_b,
		target_slot = target_slot,
		operate_type = operate_type,
		operate_result = operate_result,
	}

	MessageMgr:send_message_to_self(temp_player, "hld.item_proto.patron_saint_mark_operate_end", MsgData, 10731)
end

define_C[10745] = function(message_data, player_ref)

	--item_proto_tinder_prop_operate
	local msg_data = decode("hld.item_proto.tinder_prop_operate", message_data)

	patron_saint_class:tinder_operate(player_ref, msg_data.mark_guid[1], msg_data.mark_guid[2], msg_data.target_slot, msg_data.operate_type)

end

function patron_saint_class:tinder_operate(temp_player, mark_guid_a, mark_guid_b, target_slot, operate_type)

	local mark_guid = hld.guid_transfer_to_string(mark_guid_a, mark_guid_b)
	local operate_result = ConstDefine.e_item_string_unkown
	
	if operate_type == hld.e_patron_saint_mark_operate_type_equip_prop then
		--TODO
		operate_result = patron_saint_class:prop_equip(temp_player, mark_guid, target_slot)
		
	elseif operate_type == hld.e_patron_saint_mark_operate_type_unequip_prop then
		--TODO
		operate_result = patron_saint_class:prop_unequip(temp_player, mark_guid)

	elseif operate_type == hld.e_patron_saint_mark_operate_type_purchase_prop_new_slot then
		--TODO
		operate_result = patron_saint_class:purchase_prop_slot(temp_player)
	end

	local MsgData = 
	{
		mark_guid_a = mark_guid_a,
		mark_guid_b = mark_guid_b,
		target_slot = target_slot,
		operate_type = operate_type,
		operate_result = operate_result,
	}

	MessageMgr:send_message_to_self(temp_player, "hld.item_proto.tinder_prop_operate_end", MsgData, 10746)
end

define_C[11509] = function(message_data, player_ref)

	local msg_data = decode("hld.pokedex_proto.tinder_operate", message_data)

	patron_saint_class:operate_tinder(player_ref, msg_data.operation_type)

end

function patron_saint_class:operate_tinder(temp_player, operate_type)

	local operate_result = 0;
	if operate_type == hld.e_tinder_operation_type_levelup_tinder then
		operate_result = patron_saint_class:tinder_upgrade(temp_player)

	elseif operate_type == hld.e_tinder_operation_type_unlock_tinder then
		operate_result = patron_saint_class:tinder_unlock(temp_player)

	elseif operate_type == hld.e_tinder_operation_type_unlock_active_skill then
		operate_result = patron_saint_class:set_unit_skill(temp_player)
	end

	local MsgData = 
	{
		operation_type = operate_type,
		result = operate_result,
	}

	MessageMgr:send_message_to_self(temp_player, "hld.pokedex_proto.tinder_operate_end", MsgData, 11514)
end
--------------------------------------------------------------------------------------------------------------------------------------------------
patron_saint = patron_saint_class
return patron_saint