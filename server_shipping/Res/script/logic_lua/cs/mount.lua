local mount_class = {}

function mount_class:init()
end

----------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 获取坐骑表
--]]

function mount_class:get_mount_template_ptr(mount_item)
    local item_template_ptr = server_tool_mgr:get_item_template(mount_item)
	if nil == item_template_ptr then
		return nil
    end
	return template_manager:get_line("MountTemplate", item_template_ptr.logic_id + mount_item:get_data_info(hld.e_item_info_upgrade_count))
end
----------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 启用外形
--]]
function mount_class:show_this_mount(unit_array_index, mount_guid, item_slot, is_buff)

    local temp_unit = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_unit == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = temp_unit:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

    local mount_ptr = item_set_ref:get_item(tostring(mount_guid))
    if mount_ptr == nil then
        return ConstDefine.e_item_string_no_item
    end

    local temp_mount_item_template_ptr = server_tool_mgr:get_item_template(mount_ptr)
	if temp_mount_item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local temp_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
	if temp_mount_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local MsgData = {}
    MsgData.role_guid = {}
    --item_proto_change_mount_shape msg;
    table.insert(MsgData.role_guid, temp_unit:get_unit_guid_A());
	table.insert(MsgData.role_guid, temp_unit:get_unit_guid_B());
	MsgData.is_buff = is_buff
	if item_slot < 0 then
		if mount_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
			return ConstDefine.e_item_string_cant_show_unlock_mount
        end

		temp_unit:set_unit_info(hld.e_role_info_mount_showd_template_id, mount_ptr:get_data_info(hld.e_item_info_info_id))
        temp_unit:send_info_one(hld.e_role_info_mount_showd_template_id)
        
        item_set_ref:set_showed_mount()
        
		mount_ptr:set_data_info(hld.e_item_info_showing_illusion_idex, item_slot)
        item_set_ref:send_item_one(mount_ptr, true, 0)
        
		MsgData.mount_template_id = mount_ptr:get_data_info(hld.e_item_info_info_id)
        MessageMgr:send_message_to_aoi_lua(temp_unit, "hld.item_proto.change_mount_shape", MsgData, 10712, true, false)
    else

        local illusion_list = temp_mount_template_ptr.IllusionList
		if item_slot > #(illusion_list) then
            return ConstDefine.e_item_string_unkown
        end

		local mount_illusion_ptr = item_set_ref:get_item_by_template_id(illusion_list[item_slot + 1])
		if mount_illusion_ptr == nil then
            return ConstDefine.e_item_string_no_item
        end

		if mount_illusion_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
			return ConstDefine.e_item_string_cant_show_unlock_mount
        end

		temp_unit:set_unit_info(hld.e_role_info_mount_showd_template_id, mount_illusion_ptr:get_data_info(hld.e_item_info_info_id))
        temp_unit:send_info_one(hld.e_role_info_mount_showd_template_id)
        
        item_set_ref:set_showed_mount()
        
		mount_ptr:set_data_info(hld.e_item_info_showing_illusion_idex, item_slot)
        item_set_ref:send_item_one(mount_ptr, true, 0)
        
		MsgData.mount_template_id = mount_illusion_ptr:get_data_info(hld.e_item_info_info_id)
        MessageMgr:send_message_to_aoi_lua(temp_unit, "hld.item_proto.change_mount_shape", MsgData, 10712, true, false)
    end

	return hld.e_error_code_success
end

--[[
函数说明: 坐骑解锁
--]]
function mount_class:mount_unlock(unit_array_index,  mount_guid)
    
    local temp_player = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local temp_mount_item = item_set_ref:get_item(tostring(mount_guid))
	if temp_mount_item == nil or temp_mount_item:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

	if temp_mount_item:get_data_info(hld.e_item_info_activate) > 0 and temp_mount_item:get_data_info(hld.e_item_info_over_time) == 0 then
		return ConstDefine.e_item_string_cant_unlock_again
    end

	local temp_mount_template_ptr = mount_class:get_mount_template_ptr(temp_mount_item)
	if temp_mount_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local matirial_done = false
	local bool money_done = false
	local pre_mount_done = false

	local matirial_cost = false
	local money_cost = false

    local unlock_money_requirement = temp_mount_template_ptr.UnlockMoneyRequirement
    if unlock_money_requirement == nil then
        return ConstDefine.e_item_string_unkown
    end
	if #(unlock_money_requirement) > 1 then
		money_cost = true
		if not temp_player:can_cut_money(tonumber(unlock_money_requirement[1]), tonumber(unlock_money_requirement[2])) then
            return ConstDefine.e_item_string_money
        end
		money_done = true
	else
		money_done = true
    end

    local unlock_matirial_requirement = temp_mount_template_ptr.UnlockMatirialRequirement
    if unlock_matirial_requirement == nil then
        return  ConstDefine.e_item_string_unkown
    end

	if #(unlock_matirial_requirement) > 1 then
		matirial_cost = true;
		local cur_matirial_num = item_set_ref:count_item_by_template_id(tonumber(unlock_matirial_requirement[1]), hld.e_bag_type_bag, -1)
		if cur_matirial_num < tonumber(unlock_matirial_requirement[2]) then
			return ConstDefine.e_item_string_matirial_not_enough
        end
        matirial_done = true
	else
		matirial_done = true
    end

    local pre_mount_requirement = temp_mount_template_ptr.PreMountRequirement
    if pre_mount_requirement == nil then
        return ConstDefine.e_item_string_unkown
    end
	if #(pre_mount_requirement) > 1 then
		local pre_mount = item_set_ref:get_item_by_template_id(tonumber(pre_mount_requirement[1]))
			if pre_mount == nil then
				return ConstDefine.e_item_string_unkown
            end
            local pre_mount_template_ptr = mount_class:get_mount_template_ptr(pre_mount)
			if tonumber(pre_mount_template_ptr.ClassNow) >= tonumber(pre_mount_requirement[2]) then
				pre_mount_done = true
            end
	else
        pre_mount_done = true
    end

	if matirial_done and money_done and pre_mount_done then
        
        if matirial_cost then
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_unlock, 0, tonumber(unlock_matirial_requirement[1]), tonumber(unlock_matirial_requirement[2]), hld.e_item_locked)
		end
        
        if money_cost then
			temp_player:cut_money(tonumber(unlock_money_requirement[1]), tonumber(unlock_money_requirement[2]), hld.e_server_log_cut_money_mount_unlock, temp_mount_item:get_data_info(hld.e_item_info_info_id), 0)
		end

		temp_mount_item:set_data_info(hld.e_item_info_activate, 1)
        temp_mount_item:set_data_info(hld.e_item_info_over_time, 0)
        
        pawn_att_mgr:apply_att_change_by_array(temp_player, temp_mount_template_ptr.AttArray, true, 1);
            
        item_set_ref:send_item_one(temp_mount_item, true, 0)

        local temp_guid_string = hld.guid_transfer_to_string(temp_mount_item:get_item_guid_A(), temp_mount_item:get_item_guid_B())
        mount_class:show_this_mount(temp_player:get_array_index(), temp_guid_string, -1, true)

        if temp_mount_template_ptr.NoticeId > 0 then
            local notice_data = {}
            notice_data.notice_id = temp_mount_template_ptr.NoticeId
            notice_data.item_name = {}
            local temp_string = hld.get_str_by_string_template_id(temp_mount_template_ptr.MountNameId, 1)
            table.insert(notice_data.item_name, temp_string)
            server_tool_mgr:send_notice(temp_player, notice_data)
        end

		temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_mount_unlock, 0, 0)
		temp_player:get_achievement_mgr():item_changed(temp_mount_item, hld.e_item_operation_mount_unlock)
        temp_player:refresh_service_goal(hld.e_service_goal_type_mount_num)
        
        item_set_ref:add_new_quick_mount_call(temp_mount_item:get_item_guid())
        
		temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_mount_total_grade, 0, 0)
        temp_player:get_phantom_mgr():on_item_oper(temp_mount_item, temp_mount_item:get_data_info(hld.e_item_info_info_id))
        
		return hld.e_error_code_success
    end

	return ConstDefine.e_item_string_unlock_requirement_not_reach;
end

--[[
函数说明: 坐骑升级
--]]
function mount_class:mount_upgrade(unit_array_index, mount_guid, use_up_item)

    local temp_player = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local mount_ptr = item_set_ref:get_item(tostring(mount_guid))
	if mount_ptr == nil or mount_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

	local use_money_instead_matirial = false
	local matirial_count = 0

	if mount_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
        return ConstDefine.e_item_string_mount_unlock
    end

	local temp_item_template_ptr = server_tool_mgr:get_item_template(mount_ptr)
	if temp_item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_money_type_index = region_template_ptr.RegionCode * 2
	local region_money_num_index  = region_money_type_index + 1

	local real_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
	if real_mount_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if real_mount_template_ptr.Type == ItemDef.mount_type_warlord or real_mount_template_ptr.Type == ItemDef.mount_type_lordlegionmem then
		return ConstDefine.e_item_string_auto_use_money_not_enough
    end

	if real_mount_template_ptr.ClassNow >= real_mount_template_ptr.MountClassMax and real_mount_template_ptr.GradeNow >= real_mount_template_ptr.MountGradeMax then
		return ConstDefine.e_item_string_full_level
    end

	if real_mount_template_ptr.GradeLimit > temp_player:get_grade_num() then
		return ConstDefine.e_item_string_unkown
    end

	local temp_cur_value = temp_player:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount)
	temp_player:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1)

	local is_big_upgrade = false
	if real_mount_template_ptr.GradeNow ~= real_mount_template_ptr.MountGradeMax then
		matirial_count = item_set_ref:count_item_by_template_id(real_mount_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, -1)
		if matirial_count < real_mount_template_ptr.UpgradeMatirialNum then
			if use_up_item == 0 then
				return ConstDefine.e_item_string_matirial_not_enough
			else
				use_money_instead_matirial = true
            end
        end

		if use_money_instead_matirial then
            local new_price_array = template_manager:get_line("GoodsTemplate", real_mount_template_ptr.UpgradeMatirialShopId, "NewPrice")
			if new_price_array ~= nil and #new_price_array() > region_money_num_index then
				local money_id = new_price_array[region_money_type_index + 1];
				local money_num = new_price_array[region_money_num_index] * real_mount_template_ptr.UpgradeMatirialNum
				if not temp_player:can_cut_money(money_id, money_num) then
					return ConstDefine.e_item_string_auto_use_money_not_enough
                end
				temp_player:cut_money(money_id, money_num, hld.e_server_log_cut_money_mount_upgrade_auto_buy, real_mount_template_ptr.UpgradeMatirialNum, 0)
			else
				return ConstDefine.e_item_string_no_item
            end
		else
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_upgrade, 0, real_mount_template_ptr.UpgradeMatirialId, real_mount_template_ptr.UpgradeMatirialNum, hld.e_item_locked)
        end

        if real_mount_template_ptr ~= nil then

            pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, false, 1)

            local mount_upgrade_now = mount_ptr:get_data_info(hld.e_item_info_upgrade_count)   
            mount_ptr:set_data_info(hld.e_item_info_upgrade_count, mount_upgrade_now + 1)

            new_real_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
            if new_real_mount_template_ptr ~= nil then
                pawn_att_mgr:apply_att_change_by_array(temp_player, new_real_mount_template_ptr.AttArray, true, 1)
				if new_real_mount_template_ptr.StarBuffId > 0 then
					local PassiveSkillTemplate = template_manager:get_line("PassiveSkillTemplate", new_real_mount_template_ptr.StarBuffId)
					local open_item_array = PassiveSkillTemplate.OpenItem
					if open_item_array ~= nil and #(open_item_array) < 2 then
						item_set_ref:star_skill_unlock(mount_guid, mount_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
                    end
				end
			else
                mount_ptr:set_data_info(hld.e_item_info_upgrade_count, mount_upgrade_now)
                pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, true, 1)
            end
		end
		temp_player:get_mount_power_mgr():set_core_is_active(mount_ptr)
		item_set_ref:send_item_one(mount_ptr, true, 0)
	else
		is_big_upgrade = true
		matirial_count = item_set_ref:count_item_by_template_id(real_mount_template_ptr.LiftingClassMatirialId, hld.e_bag_type_bag, -1)
		if matirial_count < real_mount_template_ptr.LiftingClassMatirialNum then
			if use_up_item == 0 then
				return ConstDefine.e_item_string_matirial_not_enough
			else
				use_money_instead_matirial = true
            end
        end

        if use_money_instead_matirial then
            local new_price_array = template_manager:get_line("GoodsTemplate", real_mount_template_ptr.LiftingClassShopId, "NewPrice")
		    if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
				local money_id = new_price_array[region_money_type_index + 1];
				local money_num = new_price_array[region_money_num_index + 1] * real_mount_template_ptr.LiftingClassMatirialNum
				if not temp_player:can_cut_money(money_id, money_num) then
                    return ConstDefine.e_item_string_auto_use_money_not_enough
                end
				temp_player:cut_money(money_id, money_num, hld.e_server_log_cut_money_mount_upgrade, mount_real_id, 0)
			else
				return ConstDefine.e_item_string_no_item
            end
		else
            item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_upgrade, 0, real_mount_template_ptr.LiftingClassMatirialId, real_mount_template_ptr.LiftingClassMatirialNum, hld.e_item_locked)
        end

        if real_mount_template_ptr ~= nil then

            pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, false, 1)

            local mount_upgrade_now = mount_ptr:get_data_info(hld.e_item_info_upgrade_count)   
            mount_ptr:set_data_info(hld.e_item_info_upgrade_count, mount_upgrade_now + 1)

            new_real_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
            if new_real_mount_template_ptr ~= nil then
                pawn_att_mgr:apply_att_change_by_array(temp_player, new_real_mount_template_ptr.AttArray, true, 1)
				if new_real_mount_template_ptr.StarBuffId > 0 then
					local PassiveSkillTemplate = template_manager:get_line("PassiveSkillTemplate", new_real_mount_template_ptr.StarBuffId)
					local open_item_array = PassiveSkillTemplate.OpenItem
					if open_item_array ~= nil and #(open_item_array) < 2 then
						item_set_ref:star_skill_unlock(mount_guid, mount_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
                    end
				end
			else
                mount_ptr:set_data_info(hld.e_item_info_upgrade_count, mount_upgrade_now)
                pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, true, 1)
            end
		end
		temp_player:get_mount_power_mgr():set_core_is_active(mount_ptr)
		item_set_ref:send_item_one(mount_ptr, true, 0)
    end

    local MsgData = {}
	--item_proto_show_item_upgrade_effect effect_msg;
	local final_template_ptr =  mount_class:get_mount_template_ptr(mount_ptr)
    if final_template_ptr ~= nil then
		if is_big_upgrade then
			MsgData.show_type = hld.e_item_upgrade_effect_type_mount_upgrade_class
			MsgData.show_value = final_template_ptr.ClassNow
        else
			MsgData.show_type = hld.e_item_upgrade_effect_type_mount_upgrade
			MsgData.show_value = final_template_ptr.GradeNow
        end
        MessageMgr:send_message_to_self(temp_player, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)
	end

	if is_big_upgrade then
		local current_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
        if current_mount_template_ptr ~= nil and current_mount_template_ptr.NoticeId > 0 then
            local notice_data = {}
            notice_data.notice_id = current_mount_template_ptr.NoticeId
            notice_data.item_name = {}
            notice_data.num_type_data = {}
            local temp_string = hld.get_str_by_string_template_id(current_mount_template_ptr.MountNameId, 1)
            table.insert(notice_data.item_name, temp_string)
            table.insert(notice_data.num_type_data, current_mount_template_ptr.ClassNow)
            server_tool_mgr:send_notice(temp_player, notice_data)
        end

		temp_player:get_achievement_mgr():player_changed(hld.eat_mount_grade_count);
		temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_mount_total_grade, 0, 0)
    end
    temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_mount_upgrade_num, 0, 0)
	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_mount_levelup, 0)
    temp_player:refresh_service_goal(hld.e_service_goal_type_mount_level)



	return hld.e_error_code_success
end

--[[
函数说明: 坐骑升级
--]]
function mount_class:mount_starupgrade(unit_array_index, mount_guid, use_up_item)

    local temp_player = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local mount_ptr = item_set_ref:get_item(tostring(mount_guid))
	if mount_ptr == nil or mount_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

	local use_money_instead_matirial = false
	local matirial_count = 0

	if mount_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
		return ConstDefine.e_item_string_mount_unlock
    end

	local temp_item_template_ptr = server_tool_mgr:get_item_template(mount_ptr)
	if temp_item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_money_type_index = region_template_ptr.RegionCode * 2
    local region_money_num_index  = region_money_type_index + 1

	local mount_real_id = temp_item_template_ptr.logic_id + mount_ptr:get_data_info(hld.e_item_info_stargrade_count) + 1000
	local real_mount_template_ptr = template_manager:get_line("MountTemplate", mount_real_id)
	if real_mount_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end

	if real_mount_template_ptr.Type == ItemDef.mount_type_warlord or real_mount_template_ptr.Type == ItemDef.mount_type_lordlegionmem then
		return ConstDefine.e_item_string_auto_use_money_not_enough
    end

	if real_mount_template_ptr.StarGradeNow >= real_mount_template_ptr.StarGradeMax then
		return ConstDefine.e_item_string_full_level
    end

	local mount_grade_id = temp_item_template_ptr.logic_id + mount_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local grade_ptr = template_manager:get_line("MountTemplate", mount_grade_id)
	if grade_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if real_mount_template_ptr.UpgradeLimit > grade_ptr.ClassNow then
		return ConstDefine.e_item_string_unkown
    end

	local temp_cur_value = temp_player:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount)
	temp_player:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1)

	matirial_count = item_set_ref:count_item_by_template_id(real_mount_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, -1)
	if matirial_count < real_mount_template_ptr.UpgradeMatirialNum then
		if use_up_item == 0 then
			return ConstDefine.e_item_string_matirial_not_enough
		else
            use_money_instead_matirial = true
        end
    end

    if use_money_instead_matirial then
        local new_price_array = template_manager:get_line("GoodsTemplate", real_mount_template_ptr.UpgradeMatirialShopId, "NewPrice")
		if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
			local money_id = new_price_array[region_money_type_index + 1];
			local money_num = new_price_array[region_money_num_index + 1] * real_mount_template_ptr.UpgradeMatirialNum
			if not temp_player:can_cut_money(money_id, money_num) then
                 return ConstDefine.e_item_string_auto_use_money_not_enough
            end
			temp_player:cut_money(money_id, money_num, hld.e_server_log_cut_money_mount_upgrade_auto_buy, real_mount_template_ptr.UpgradeMatirialShopId, 0)
		else
			return ConstDefine.e_item_string_no_item
        end
	else
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_starupgrade, 0, real_mount_template_ptr.UpgradeMatirialId, real_mount_template_ptr.UpgradeMatirialNum, hld.e_item_locked)
    end

	local mount_starupgrade_now = mount_ptr:get_data_info(hld.e_item_info_stargrade_count)
	mount_ptr:set_data_info(hld.e_item_info_stargrade_count, mount_starupgrade_now + 1)

    pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, false, 1)

	local next_mount_real_id = temp_item_template_ptr.logic_id + mount_ptr:get_data_info(hld.e_item_info_stargrade_count) + 1000
	local next_real_mount_template_ptr = template_manager:get_line("MountTemplate", next_mount_real_id)
	if next_real_mount_template_ptr then       
        pawn_att_mgr:apply_att_change_by_array(temp_player, next_real_mount_template_ptr.AttArray, true, 1)
        if next_real_mount_template_ptr.StarBuffId > 0 then
			local PassiveSkillTemplate = template_manager:get_line("PassiveSkillTemplate", next_real_mount_template_ptr.StarBuffId)
            local open_item_array = PassiveSkillTemplate.OpenItem
            if open_item_array ~= nil and #(open_item_array) < 2 then
                item_set_ref:star_skill_unlock(mount_guid, mount_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
            end
        end
	else
		mount_ptr:set_data_info(hld.e_item_info_stargrade_count, mount_starupgrade_now)
        pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, true, 1)
    end

	item_set_ref:send_item_one(mount_ptr, true, 0)

	return hld.e_error_code_success
end

--[[
函数说明: 坐骑幻化
--]]
function mount_class:get_mount_illusion(unit_array_index, mount_guid, illusion_slot)

	local temp_player = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local temp_mount_ptr = item_set_ref:get_item(tostring(mount_guid))
	if temp_mount_ptr == nil or temp_mount_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

	local temp_mount_template_ptr = mount_class:get_mount_template_ptr(temp_mount_ptr)
	if temp_mount_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local illusion_list = temp_mount_template_ptr.IllusionList
	if illusion_slot > #(illusion_list) then
        return ConstDefine.e_item_string_have_no_illusion
    end


	if item_set_ref:get_item_by_template_id(illusion_list[illusion_slot+1]) ~= nil then
		return ConstDefine.e_item_string_cant_unlock_again
    end

	local temp_illusion_mount_item_ptr = template_manager:get_line("ItemTemplate", illusion_list[illusion_slot+1])
	if temp_illusion_mount_item_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local temp_illusion_template_ptr = template_manager:get_line("MountTemplate", temp_illusion_mount_item_ptr.logic_id)
	if temp_illusion_template_ptr == nil then
		return ConstDefine.e_item_string_have_no_illusion
    end
    
    local pre_mount_requirement = temp_illusion_template_ptr.PreMountRequirement
    if #(pre_mount_requirement) > 1 then
		if tonumber(temp_mount_template_ptr.ClassNow) < tonumber(pre_mount_requirement[2]) then
            return hld.e_error_code_item_mount_class_not_reach
        end
    end

    local unlock_money_requirement = temp_illusion_template_ptr.UnlockMoneyRequirement
	if #(unlock_money_requirement) >= 2 then
		if not temp_player:can_cut_money(unlock_money_requirement[1], unlock_money_requirement[2]) then
			return ConstDefine.e_item_string_money
        end
    end
    
    local unlock_matirial_requirement = temp_illusion_template_ptr.UnlockMatirialRequirement
	if #(unlock_matirial_requirement) >= 2 then
		local cur_matirial_num = item_set_ref:count_item_by_template_id(unlock_matirial_requirement[1], hld.e_bag_type_bag, -1)
		if cur_matirial_num < tonumber(unlock_matirial_requirement[2]) then
			return ConstDefine.e_item_string_matirial_not_enough
        end
	end

	local illusion_item = item_set_ref:create_item_by_template(hld.e_server_log_add_item_mount_illusion, 0, illusion_list[illusion_slot+1], 1, 1, 0, 0, 0)
	if illusion_item == nil then
        return ConstDefine.e_item_string_unkown
    end

	if #(unlock_money_requirement) >= 2 then
		temp_player:cut_money(unlock_money_requirement[1], unlock_money_requirement[2], hld.e_server_log_cut_money_get_mount_illusion, illusion_list[illusion_slot + 1])
    end

	if #(unlock_matirial_requirement) >= 2 then
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_illusion, 0, unlock_matirial_requirement[1], unlock_matirial_requirement[2], hld.e_item_locked)
    end

	illusion_item:set_data_info(hld.e_item_info_activate, 1)
    pawn_att_mgr:apply_att_change_by_array(temp_player, temp_illusion_template_ptr.AttArray, true, 1)
	item_set_ref:send_item_one(illusion_item, true, 0)

    local temp_guid_string = hld.guid_transfer_to_string(temp_mount_ptr:get_item_guid_A(), temp_mount_ptr:get_item_guid_B())
    mount_class:show_this_mount(temp_player:get_array_index(), temp_guid_string, illusion_slot, true)

    temp_player:get_achievement_mgr():item_changed(temp_mount_ptr, hld.e_item_operation_mount_get_illusion)
    
	return hld.e_error_code_success
end

--[[
函数说明: 坐骑幻化升级
--]]
function mount_class:mount_illusionupgrade(unit_array_index, mount_guid, use_up_item)

    local temp_player = hld.get_player_by_index(tonumber(unit_array_index))
	if temp_player == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local mount_ptr = item_set_ref:get_item(tostring(mount_guid))
	if mount_ptr == nil or mount_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end
    
    local use_money_instead_matirial = false
    local matirial_count = 0


	if mount_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
        return ConstDefine.e_item_string_mount_unlock
    end

	local temp_item_template_ptr = server_tool_mgr:get_item_template(mount_ptr)
	if temp_item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_money_type_index = region_template_ptr.RegionCode * 2
	local region_money_num_index  = region_money_type_index + 1

	local real_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
	if real_mount_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if real_mount_template_ptr.Type ~= ItemDef.mount_type_illusion then
		return ConstDefine.e_item_string_auto_use_money_not_enough
    end

	if real_mount_template_ptr.GradeNow >= real_mount_template_ptr.MountGradeMax then
		return ConstDefine.e_item_string_full_level
    end

    local pre_mount_requirement = real_mount_template_ptr.PreMountRequirement
	if #(pre_mount_requirement) > 1 then       
        local pre_mount = item_set_ref:get_item_by_template_id(pre_mount_requirement[1])
		if pre_mount == nil then
			return ConstDefine.e_item_string_unkown
        end
        local pre_mount_template_ptr = mount_class:get_mount_template_ptr(pre_mount)
		if pre_mount_template_ptr.ClassNow < pre_mount_requirement[2] then
			return ConstDefine.e_item_string_unkown
        end
    end
    
	local temp_cur_value = temp_player:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount)
	temp_player:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1)

	matirial_count = item_set_ref:count_item_by_template_id(real_mount_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, -1)
	if matirial_count < real_mount_template_ptr.UpgradeMatirialNum then
		if use_up_item == 0 then
			return ConstDefine.e_item_string_matirial_not_enough
		else
            use_money_instead_matirial = true
        end
    end

    if use_money_instead_matirial then
        local new_price_array = template_manager:get_line("GoodsTemplate", real_mount_template_ptr.UpgradeMatirialShopId, "NewPrice")
		if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
			local money_id = new_price_array[region_money_type_index + 1];
			local money_num = new_price_array[region_money_num_index + 1] * real_mount_template_ptr.UpgradeMatirialNum
			if not temp_player:can_cut_money(money_id, money_num) then
                 return ConstDefine.e_item_string_auto_use_money_not_enough
            end
			temp_player:cut_money(money_id, money_num, hld.e_server_log_cut_money_mount_upgrade_auto_buy, real_mount_template_ptr.UpgradeMatirialShopId, 0)
		else
			return ConstDefine.e_item_string_no_item
        end
	else
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mount_illusionupgrade, 0, real_mount_template_ptr.UpgradeMatirialId, real_mount_template_ptr.UpgradeMatirialNum, hld.e_item_locked)
    end

    pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, false, 1)

	local mount_upgrade_now = mount_ptr:get_data_info(hld.e_item_info_upgrade_count)
    mount_ptr:set_data_info(hld.e_item_info_upgrade_count, mount_upgrade_now + 1)

	local next_real_mount_template_ptr = mount_class:get_mount_template_ptr(mount_ptr)
	if next_real_mount_template_ptr then       
        pawn_att_mgr:apply_att_change_by_array(temp_player, next_real_mount_template_ptr.AttArray, true, 1)
        if next_real_mount_template_ptr.StarBuffId > 0 then
            item_set_ref:star_skill_unlock(mount_guid, mount_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
        end
	else
		mount_ptr:set_data_info(hld.e_item_info_stargrade_count, mount_starupgrade_now)
        pawn_att_mgr:apply_att_change_by_array(temp_player, real_mount_template_ptr.AttArray, true, 1)
    end
    
    item_set_ref:send_item_one(mount_ptr, true, 0)

    local MsgData = {}
	local final_template_ptr =  mount_class:get_mount_template_ptr(mount_ptr)
	if final_template_ptr ~= nil then
		MsgData.show_type = hld.e_item_upgrade_effect_type_mount_upgrade
		MsgData.show_value = final_template_ptr.GradeNow
        MessageMgr:send_message_to_self(temp_player, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)
    end


	return hld.e_error_code_success
end
----------------------------------------------------------------------------------------------------------------------------------------

mount = mount_class
return mount