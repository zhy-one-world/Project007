local wing_class = {}

function wing_class:init()
end
----------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 羽翼、神翼 升星、升代
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]
function wing_class:wing_upgrade_rate(bless, wing_template_ptr)

    if wing_class == nil then
        return false
    end

    if bless < wing_template_ptr.MinUpgradeBless then
        return false
    end

    if bless >= wing_template_ptr.MaxUpgradeBless then
        return true
    end

    if wing_template_ptr.EachUpgradeBless <= 0 then
        return false
    end

    local m_random = hld.random_gen:get_random(1, 100)
    local template_random = 100 * (wing_template_ptr.BasicRate + ((bless - wing_template_ptr.MinUpgradeBless) / wing_template_ptr.EachUpgradeBless) * wing_template_ptr.RateChange)
    if m_random <= template_random then
        return true
    end
    return false
end
----------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 羽翼、神翼 升星、升代
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]
function wing_class:wing_upgrade(unit_array_index, wing_guid, use_up_item)

	local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local wing_ptr = item_set_ref:get_item(tostring(wing_guid))
	if wing_ptr == nil or wing_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

	if wing_ptr:get_data_info(hld.e_item_info_activate) == 0 or wing_ptr:get_data_info(hld.e_item_info_over_time) ~= 0 then
		return ConstDefine.e_item_string_unkown
    end

    local item_template_ptr = template_manager:get_line("ItemTemplate", wing_ptr:get_item_template_id())
	if item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
    
	if item_template_ptr.item_type ~= hld.e_item_type_wing then
		return ConstDefine.e_item_string_unkown
    end

	local use_money_instead_matirial = false

    local real_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
    
	local wing_template_ptr = template_manager:get_line("WingTemplate", real_id)
	if wing_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local region_template_ptr = server_tool_mgr:get_region_template_ptr()
    if region_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local region_money_type_index = region_template_ptr.RegionCode * 2
    local region_money_num_index  = region_money_type_index + 1;

    local is_cost_money = false
    local upgrade_money_array = wing_template_ptr.UpgradeMoney
	if #(upgrade_money_array) > 1 then
		is_cost_money = true
    end

	if is_cost_money and #(upgrade_money_array) % 2 ~= 0 then
        return ConstDefine.e_item_string_unkown
    end

	if wing_template_ptr.GradeLimit > player_ref:get_grade_num() then
		return ConstDefine.e_item_string_unkown
    end

	local cur_wing_star_num = wing_template_ptr.StarNow
	local max_wing_star_num = wing_template_ptr.WingAddStarMax
	local cur_wing_level    = wing_template_ptr.UpgradeNow
	local max_wing_level    = wing_template_ptr.WingUpgradeMax

    --TODO 测试 get_cur_level_wing这个接口能不能判断
	if wing_template_ptr.Type == hld.e_item_wing_type_normal and wing_ptr ~= item_set_ref:get_cur_level_wing() then
		return ConstDefine.e_item_string_full_level
    end

	if wing_template_ptr.Type == hld.e_item_wing_type_illusion or wing_template_ptr.Type == hld.e_item_wing_type_special_santo then
		return ConstDefine.e_item_string_unkown
    end

	if cur_wing_star_num >= max_wing_star_num and cur_wing_level >= max_wing_level then
		return ConstDefine.e_item_string_full_level
    end

    if cur_wing_star_num == max_wing_star_num then
        
        if wing_template_ptr.Type == hld.e_item_wing_type_normal then         
			if player_ref:get_unit_info(hld.e_role_info_exp_level) < wing_template_ptr.WingOpenNeedRoleClass then
				return ConstDefine.e_item_string_level
            end
		else
			local cur_max_level_wing_ptr = item_set_ref:get_cur_level_wing()
			if cur_max_level_wing_ptr == nil then
				return ConstDefine.e_item_string_unkown
            end

			local cur_max_level_item_template_ptr = template_manager:get_line("ItemTemplate", cur_max_level_wing_ptr:get_item_template_id())
			if cur_max_level_item_template_ptr == nil then
				return ConstDefine.e_item_string_unkown
            end

            local cur_max_level_wing_template_ptr = template_manager:get_line("WingTemplate", cur_max_level_item_template_ptr.logic_id + cur_max_level_wing_ptr:get_data_info(hld.e_item_info_upgrade_count))
            if cur_max_level_wing_template_ptr == nil then
                return ConstDefine.e_item_string_unkown
            end

			if wing_template_ptr.UpgradeNow >= cur_max_level_wing_template_ptr.UpgradeNow and wing_template_ptr.UpgradeNow < 5 then
                return ConstDefine.e_item_string_level
            end
        end

		if item_set_ref:count_item_by_template_id(wing_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, -1) < wing_template_ptr.UpgradeMatirialNum then
			if use_up_item == 0 then
				return ConstDefine.e_item_string_matirial_not_enough
            else
				use_money_instead_matirial = true
			end
        end

		if use_money_instead_matirial then
			local GoodsTemplate = template_manager:get_line("GoodsTemplate", wing_template_ptr.UpgradeShopId)
			local new_price_array = GoodsTemplate.NewPrice
			if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
				local money_id = new_price_array[region_money_type_index + 1]
				local money_num = new_price_array[region_money_num_index + 1] * wing_template_ptr.UpgradeMatirialNum
				if not player_ref:can_cut_money(money_id, money_num) then
					return ConstDefine.e_item_string_auto_use_money_not_enough
                end
				player_ref:cut_money(money_id, money_num, hld.e_server_log_cut_money_wing_upgrade_auto_buy, wing_template_ptr.UpgradeShopId, 0)
			else
                return ConstDefine.e_item_string_no_item
            end
		else
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_upgrade, wing_ptr:get_data_info(hld.e_item_info_info_id) , wing_template_ptr.UpgradeMatirialId, wing_template_ptr.UpgradeMatirialNum, hld.e_item_locked)
        end

		if #(upgrade_money_array) >= hld.e_item_tuple_max and player_ref:can_cut_money(upgrade_money_array[hld.e_item_tuple_id + 1], upgrade_money_array[hld.e_item_tuple_num + 1]) == false then
			return ConstDefine.e_item_string_money
        end

		local upgrade_bless_now = wing_ptr:get_data_info(hld.e_item_info_wing_bless)
		if is_cost_money then
			if #(upgrade_money_array) >= hld.e_item_tuple_max then
				player_ref:cut_money(upgrade_money_array[hld.e_item_tuple_id + 1], upgrade_money_array[hld.e_item_tuple_num + 1], hld.e_server_log_cut_money_wing_upgrade, real_id, 0)
			else
				return ConstDefine.e_item_string_unkown
            end
        end

        wing_ptr:set_data_info(hld.e_item_info_wing_bless, upgrade_bless_now + wing_template_ptr.EachUpgradeBless)
        
        upgrade_bless_now = wing_ptr:get_data_info(hld.e_item_info_wing_bless)
        
		if wing_class:wing_upgrade_rate(upgrade_bless_now, wing_template_ptr) == true then
			item_set_ref:set_wing_att(false)

			wing_ptr:set_data_info(hld.e_item_info_wing_exp, 0)
			wing_ptr:set_data_info(hld.e_item_info_wing_bless, 0)

			if wing_template_ptr.Type ~= hld.e_item_wing_type_normal then
                wing_ptr:set_data_info(hld.e_item_info_upgrade_count, wing_ptr:get_data_info(hld.e_item_info_upgrade_count) + 1)
                item_set_ref:set_wing_att(true)
                
                local MsgData =
                {
                    show_type = hld.e_item_upgrade_effect_type_wing_upgrade,
                    show_value = cur_wing_level + 1
                }
                MessageMgr:send_message_to_self(player_ref, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)
    
                local temp_real_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
				local new_wing_template_ptr = template_manager:get_line("WingTemplate", temp_real_id);
				if new_wing_template_ptr ~= nil then
					if new_wing_template_ptr.StarBuffId > 0 then
						local PassiveSkillTemplate = template_manager:get_line("PassiveSkillTemplate", new_wing_template_ptr.StarBuffId)
						local open_item_array = PassiveSkillTemplate.OpenItem
						if open_item_array ~= nil and #(open_item_array) < 2 then
							item_set_ref:star_skill_unlock(wing_guid, wing_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
                        end
					end
                    if new_wing_template_ptr.NoticeId > 0 then
                        local temp_string = hld.get_str_by_string_template_id(new_wing_template_ptr.WingNameId, 1)
                        local notice_data = {};
                        notice_data.notice_id = new_wing_template_ptr.NoticeId
                        notice_data.item_name = {}
                        notice_data.num_type_data = {}
                        table.insert(notice_data.item_name, temp_string);
                        table.insert(notice_data.num_type_data, new_wing_template_ptr.UpgradeNow);
                        server_tool_mgr:send_notice(player_ref, notice_data);
                    end
				end
			else
				local next_level_wing = item_set_ref:get_item_by_slot(hld.e_bag_type_wing, wing_ptr:get_data_info(hld.e_item_info_slot) + 1)
				if next_level_wing == nil then
					return ConstDefine.e_item_string_unkown
                end

				local next_level_wing_item_template = template_manager:get_line("ItemTemplate", next_level_wing:get_item_template_id())
				if next_level_wing_item_template == nil then
					return ConstDefine.e_item_string_unkown
                end

				local next_level_wing_wing_template = template_manager:get_line("WingTemplate", next_level_wing_item_template.logic_id)
				if next_level_wing_wing_template == nil then
					return ConstDefine.e_item_string_unkown
                end

				if next_level_wing_wing_template.Type == hld.e_item_wing_type_normal then
					next_level_wing:set_data_info(hld.e_item_info_activate, 1)
					next_level_wing:set_data_info(hld.e_item_info_upgrade_count, 0)
					next_level_wing:set_data_info(hld.e_item_info_wing_exp, 0)
					next_level_wing:set_data_info(hld.e_item_info_wing_bless, 0)
					item_set_ref:send_item_one(next_level_wing, true, 0)
                end

				local cur_level_wing = item_set_ref:get_cur_level_wing()
				if cur_level_wing == nil then
					return ConstDefine.e_item_string_unkown
                end
                
                item_set_ref:set_wing_att(true)

                local MsgData =
                {
                    show_type = hld.e_item_upgrade_effect_type_wing_upgrade,
                    show_value = next_level_wing_wing_template.UpgradeNow
                }
                MessageMgr:send_message_to_self(player_ref, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)

				player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_wing_upgrade, 0, 0)
				player_ref:get_achievement_mgr():item_changed(wing_ptr, hld.e_item_operation_featherupgrade_grade)

                local temp_real_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
				local current_wing_template_ptr = template_manager:get_line("WingTemplate", temp_real_id);
                if current_wing_template_ptr ~= nil and current_wing_template_ptr.NoticeId > 0 then
                    player_ref:send_upgrade_wing_notify_with_item(current_wing_template_ptr.NoticeId, current_wing_template_ptr.WingNameId, current_wing_template_ptr.UpgradeNow, wing_ptr)
                    player_ref:refresh_service_goal(hld.e_service_goal_type_wing_level)
                end
			end
		end

		item_set_ref:send_item_one(wing_ptr, true, 0)
		local temp_cur_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_wing)
        player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_wing, temp_cur_value + 1)
        player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_wing_all_star, 0, 0)
        return hld.e_error_code_success
            
	else
		if item_set_ref:count_item_by_template_id(wing_template_ptr.AddStarMatirialId, hld.e_bag_type_bag, -1) < wing_template_ptr.AddStarMatirialNum then
			if use_up_item == 0 then
				return ConstDefine.e_item_string_matirial_not_enough
			else
				use_money_instead_matirial = true
            end
        end
		
		if use_money_instead_matirial then
            local GoodsTemplate = template_manager:get_line("GoodsTemplate", wing_template_ptr.StarShopId)
            local new_price_array = GoodsTemplate.NewPrice
            if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
                local money_id = new_price_array[region_money_type_index + 1]
                local money_num = new_price_array[region_money_num_index + 1] * wing_template_ptr.AddStarMatirialNum
				if not player_ref:can_cut_money(money_id, money_num) then
					return ConstDefine.e_item_string_auto_use_money_not_enough
                end
				player_ref:cut_money(money_id, money_num, hld.e_server_log_cut_money_wing_add_star_auto_buy, wing_template_ptr.StarShopId, 0)
			else
				return ConstDefine.e_item_string_no_item
            end
		else
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_upgrade, wing_ptr:get_data_info(hld.e_item_info_info_id) , wing_template_ptr.AddStarMatirialId, wing_template_ptr.AddStarMatirialNum, hld.e_item_locked)
        end

		local add_star_exp_now = wing_ptr:get_data_info(hld.e_item_info_wing_exp)
        local add_star_exp_max = wing_template_ptr.AddStarMaxEXP
        
        local crirical_flag = false
        local rate = hld.random_gen:get_random(1, 100)
		if rate < wing_template_ptr.AddStarCriticalRate then
			add_star_exp_now = add_star_exp_now + (wing_template_ptr.EachAddStarExp * 2)
			crirical_flag = true
		else
			add_star_exp_now = add_star_exp_now + wing_template_ptr.EachAddStarExp
			crirical_flag = false
        end

		local temp_star = 0
		while add_star_exp_now >= add_star_exp_max do
			add_star_exp_now = add_star_exp_now - add_star_exp_max
			temp_star = temp_star + 1
        end
        
        if temp_star > 0 then

            item_set_ref:set_wing_att(false)

            if cur_wing_star_num + temp_star > max_wing_star_num then
				temp_star = max_wing_star_num - cur_wing_star_num
            end
            
            wing_ptr:set_data_info(hld.e_item_info_upgrade_count, wing_ptr:get_data_info(hld.e_item_info_upgrade_count) + temp_star)
            
            local wing_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
			local wing_temp_ptr = template_manager:get_line("WingTemplate", wing_id)
            
            if wing_temp_ptr ~= nil then

				item_set_ref:set_wing_att(true)
				if wing_temp_ptr.StarBuffId > 0 then
                    local PassiveSkillTemplate = template_manager:get_line("PassiveSkillTemplate", new_wing_template_ptr.StarBuffId)
                    local open_item_array = PassiveSkillTemplate.OpenItem
                    if open_item_array ~= nil and #(open_item_array) < 2 then
                        item_set_ref:star_skill_unlock(wing_guid, wing_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
                    end

                    local MsgData =
                    {
                        show_type = hld.e_item_upgrade_effect_type_wing_upgrade,
                        show_value = next_level_wing_wing_template.UpgradeNow
                    }
                    MessageMgr:send_message_to_self(player_ref, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)
				end
            end
        end
        
        wing_ptr:set_data_info(hld.e_item_info_wing_exp, add_star_exp_now)
            
        item_set_ref:send_item_one(wing_ptr, true, 0)
            
        player_ref:refresh_service_goal(hld.e_service_goal_type_wing_level)
        player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_wing_all_star, 0, 0)
		if crirical_flag then
			return hld.e_error_code_item_add_star_critical
        end

        local temp_cur_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_wing)
        player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_wing, temp_cur_value + 1)
		return hld.e_error_code_success
    end
end

--[[
函数说明: 启用翅膀外观
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]

function wing_class:show_this_wing(unit_array_index, wing_guid, item_slot, is_mission_wing, is_need_send_notice)

	local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local wing_ptr = item_set_ref:get_item(tostring(wing_guid))
	if wing_ptr == nil or wing_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

    if wing_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
        return ConstDefine.e_item_string_chant_show_unactivate_wing
    end

    local item_template_ptr = template_manager:get_line("ItemTemplate", wing_ptr:get_item_template_id())
	if item_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local wing_template_ptr = template_manager:get_line("WingTemplate", item_template_ptr.logic_id)
    if wing_template_ptr == nil then
        return hld.e_error_code_no_template
    end

    local IIllusionList_array = wing_template_ptr.IIllusionList

    local wing_template_id = 0
    if item_slot < 0 then
        wing_template_id = wing_ptr:get_data_info(hld.e_item_info_info_id)
        wing_ptr:set_data_info(hld.e_item_info_showing_illusion_idex, item_slot)
    else
        if item_slot >= #(IIllusionList_array) then
            return hld.e_error_code_no_template
        end

        local wing_illusion_ptr = item_set_ref:get_item_by_template_id(IIllusionList_array[item_slot + 1])
        if wing_illusion_ptr == nil then
            return ConstDefine.e_item_string_no_item
        end

        if wing_illusion_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
            return ConstDefine.e_item_string_chant_show_unactivate_wing
        end

        wing_template_id = wing_illusion_ptr:get_data_info(hld.e_item_info_info_id)
        wing_ptr:set_data_info(hld.e_item_info_showing_illusion_idex, item_slot)
    end

    local cur_showing_wing = player_ref:get_unit_info(hld.e_role_info_wing_showd_template_id)
    if cur_showing_wing == wing_template_id then
        wing_template_id = 0
    end

    player_ref:set_unit_info(hld.e_role_info_wing_showd_template_id, wing_template_id)
    player_ref:send_info_one(hld.e_role_info_wing_showd_template_id)

    item_set_ref:set_showed_wing()
    if wing_template_id > 0 then
        if player_ref:get_game_att(hld.e_unit_game_att_movement) == AttDef.e_move_ment_walk then
            if true == is_mission_wing then
                player_ref:get_pawn_att():apply_att_change_effect(4, hld.e_unit_attack_att_move_speed, 0, wing_template_ptr.MaxWalkSpeed, AttDef.send_gs_to_client, true)
            end
            player_ref:get_pawn_att():set_game_att(hld.e_unit_game_att_movement, AttDef.e_move_ment_wing_walk, true)
        end
        item_set_ref:send_item_one(wing_ptr, true, 0)
    else
        if true == is_need_send_notice then
            local notice_str = hld.get_str_by_string_template_id(90096106)
            player_ref:send_notice(notice_str)
        end
        if player_ref:get_game_att(hld.e_unit_game_att_movement) == AttDef.e_move_ment_wing_walk then
            if true == is_mission_wing then
                player_ref:get_pawn_att():apply_att_change_effect(4, hld.e_unit_attack_att_move_speed, 0, wing_template_ptr.MaxWalkSpeed, AttDef.send_gs_to_client, false)
            end
            player_ref:get_pawn_att():set_game_att(hld.e_unit_game_att_movement, AttDef.e_move_ment_walk, true)
        elseif player_ref:get_game_att(hld.e_unit_game_att_movement) == AttDef.e_move_ment_wing_fly then
            player_ref:get_pawn_att():apply_att_change_effect(4, hld.e_unit_attack_att_move_speed, 0, wing_template_ptr.MaxFlySpeed, AttDef.send_gs_to_client, false)
            local mount_ptr = item_set_ref:get_showed_mount()
            if mount_ptr ~= nil then
                local mount_temp_ptr = template_manager:get_line("MountTemplate", mount_ptr:get_item_template_id() + mount_ptr:get_data_info(hld.e_item_info_upgrade_count))
                if mount_temp_ptr ~= nil then
                    player_ref:skill_cast_by_skill_id(13300600)
                end
            else
                player_ref:get_pawn_att():set_game_att(hld.e_unit_game_att_movement, hld.e_move_ment_walk, true)
            end
        end
    end

    local guid_a = player_ref:get_unit_guid_A()
    local guid_b = player_ref:get_unit_guid_B()
    local MsgData = 
    {
        role_guid = {guid_a, guid_b},
        wing_template_id = wing_template_id
    }
    MessageMgr:send_message_to_aoi_lua(player_ref, "hld.item_proto.show_this_wing", MsgData, 10710, true, false)

    return hld.e_error_code_success

end

--[[
函数说明: 启用翅膀外观
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]
function wing_class:wing_shape_unlock(unit_array_index, wing_guid, item_slot)

	local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

	local item_wing_ptr = item_set_ref:get_item(tostring(wing_guid))
	if item_wing_ptr == nil then
		return ConstDefine.e_item_string_no_item
    end

	local money_cost = false
	local matirial_unlock = false

    local item_template_ptr = template_manager:get_line("ItemTemplate", item_wing_ptr:get_item_template_id())
	if item_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end

    local real_id = item_template_ptr.logic_id + item_wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
    
	local wing_template_ptr = template_manager:get_line("WingTemplate", real_id)
	if wing_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end

    local unlock_need_money = wing_template_ptr.UnlockNeedMoney
    local unlock_need_matirial = wing_template_ptr.UnlockNeedMatirial
    if unlock_need_money == nil  or unlock_need_matirial == nil then
        return ConstDefine.e_item_string_unkown
    end
	if 0 == item_wing_ptr:get_data_info(hld.e_item_info_activate) then
		if #(unlock_need_money) >= hld.e_item_tuple_max and #(unlock_need_money) % hld.e_item_tuple_max == 0 then
			if not player_ref:can_cut_money(unlock_need_money[hld.e_item_tuple_id + 1], unlock_need_money[hld.e_item_tuple_num + 1]) then
                return ConstDefine.e_item_string_money
            end
            money_cost = true
        end
		if #(unlock_need_matirial) >= 2 and #(unlock_need_matirial) % 2 == 0 then
			if item_set_ref:count_item_by_template_id(unlock_need_matirial[1], hld.e_bag_type_bag, -1) < tonumber(unlock_need_matirial[2]) then
			    return ConstDefine.e_item_string_matirial_not_enough;
            end
			matirial_unlock = true
        end
    else
        return ConstDefine.e_item_string_cant_unlock_again
    end

	if money_cost then
		if #(unlock_need_money) >= hld.e_item_tuple_max then
			player_ref.cut_money(unlock_need_money[1], unlock_need_money[2], hld.e_server_log_cut_money_wing_upgrade, item_wing_ptr:get_data_info(hld.e_item_info_info_id), 0)
		else
			return ConstDefine.e_item_string_unkown
        end
    end

	if matirial_unlock then
		if #(unlock_need_matirial) >= hld.e_item_tuple_max then
			item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_unlock, 0, unlock_need_matirial[1], unlock_need_matirial[2],hld.e_item_locked)
		else
			return ConstDefine.e_item_string_unkown
        end
    end

    item_set_ref:set_wing_att(false)
    
    item_wing_ptr:set_data_info(hld.e_item_info_activate, 1)

    item_wing_ptr:set_data_info(hld.e_item_info_over_time, 0)

    item_set_ref:set_wing_att(true)
    
    if wing_template_ptr.StarBuffId > 0 then
		local PassiveSkillTemplate  = template_manager:get_line("PassiveSkillTemplate", wing_template_ptr.StarBuffId)
        local open_item_array = PassiveSkillTemplate.OpenItem
        if open_item_array ~= nil and #(open_item_array) < 2 then
             item_set_ref:star_skill_unlock(wing_guid, item_wing_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
        end
    end
    
    item_set_ref:send_item_one(item_wing_ptr, true, 0)
    
    local temp_guid_string = hld.guid_transfer_to_string(item_wing_ptr:get_item_guid_A(), item_wing_ptr:get_item_guid_B())
    wing_class:show_this_wing(player_ref:get_array_index(), temp_guid_string, item_slot, false, false)

	player_ref:get_phantom_mgr():on_item_oper(item_wing_ptr, item_wing_ptr:get_data_info(hld.e_item_info_info_id))

	return hld.e_error_code_success
end

--[[
函数说明: 升魂
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]

function wing_class:wing_starupgrade(unit_array_index, wing_guid, use_up_item)
	
	local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end
    
    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

    local wing_ptr = item_set_ref:get_item(tostring(wing_guid))
	if wing_ptr == nil or wing_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

    local item_template_ptr = template_manager:get_line("ItemTemplate", wing_ptr:get_item_template_id())
	if item_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end

	if wing_ptr:get_data_info(hld.e_item_info_activate) == 0 or wing_ptr:get_data_info(hld.e_item_info_over_time) ~= 0 then
		return ConstDefine.e_item_string_unkown
    end

	if item_template_ptr.item_type ~= hld.e_item_type_wing then
		return ConstDefine.e_item_string_unkown
    end

	local use_money_instead_matirial = false

	local real_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_stargrade_count) + 1000
	local wing_template_ptr = template_manager:get_line("WingTemplate", real_id)
	if wing_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_money_type_index = region_template_ptr.RegionCode * 2;
	local region_money_num_index = region_money_type_index + 1;

    local is_cost_money = false
    local upgrade_money_array = wing_template_ptr.UpgradeMoney
	if #(upgrade_money_array) > 1 then
		is_cost_money = true
    end
	if is_cost_money and #(upgrade_money_array) % 2 ~= 0 then
		return ConstDefine.e_item_string_unkown
    end

	local offset_upgrade_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local offset_template_ptr = template_manager:get_line("WingTemplate", offset_upgrade_id)
	if offset_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if wing_template_ptr.UpgradeLimit > offset_template_ptr.UpgradeNow then
		return ConstDefine.e_item_string_unkown
    end

	local cur_wing_star_num = wing_template_ptr.StarGradeNow
	local max_wing_star_num = wing_template_ptr.StarGradeMax

	if wing_template_ptr.Type == hld.e_item_wing_type_shape_illusion and cur_wing_star_num >= max_wing_star_num then
        return ConstDefine.e_item_string_full_level
    end

	if item_set_ref:count_item_by_template_id(wing_template_ptr.AddStarMatirialId, hld.e_bag_type_bag, -1) < wing_template_ptr.AddStarMatirialNum then
		if use_up_item == 0 then
			return ConstDefine.e_item_string_matirial_not_enough;
		else
			use_money_instead_matirial = true
        end
    end

    if use_money_instead_matirial then
        local new_price_array = template_manager:get_line("GoodsTemplate", wing_template_ptr.StarShopId, "NewPrice")
        if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
			local money_id  = new_price_array[region_money_type_index + 1];
			local money_num = new_price_array[region_money_num_index + 1] * wing_template_ptr.AddStarMatirialNum
			if not player_ref:can_cut_money(money_id, money_num) then
				return ConstDefine.e_item_string_auto_use_money_not_enough
            end
			player_ref:cut_money(money_id, money_num, faiht.e_server_log_cut_money_wing_add_star_auto_buy, wing_template_ptr.StarShopId, 0)
		else
			return ConstDefine.e_item_string_no_item
        end
	else
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_starupgrade, 0 ,wing_template_ptr.AddStarMatirialId, wing_template_ptr.AddStarMatirialNum, hld.e_item_locked)
    end
 
    pawn_att_mgr:apply_att_change_by_array(player_ref, wing_template_ptr.AttArray, false, 1)
	wing_ptr:set_data_info(hld.e_item_info_stargrade_count, wing_ptr:get_data_info(hld.e_item_info_stargrade_count) + 1)
	local wing_next_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_stargrade_count) + 1000
	local wing_next_temp_ptr = template_manager:get_line("WingTemplate", wing_next_id);
    if wing_next_temp_ptr ~= nil then
        
        pawn_att_mgr:apply_att_change_by_array(player_ref,wing_next_temp_ptr.AttArray, true, 1)

        local MsgData =
        {
            show_type = hld.e_item_upgrade_effect_type_wing_add_star,
            show_value = wing_next_temp_ptr.StarGradeNow
        }
        MessageMgr:send_message_to_self(player_ref, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)

        if wing_next_temp_ptr.StarBuffId > 0 then
			local PassiveSkillTemplate = template_manager:get_line("PassiveSkillTemplate", wing_next_temp_ptr.StarBuffId)
            local open_item_array = PassiveSkillTemplate.OpenItem
			
            if open_item_array ~= nil and #(open_item_array) < 2 then
                 item_set_ref:star_skill_unlock(wing_guid, wing_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
            end
        end
    end

	item_set_ref:send_item_one(wing_ptr, true, 0)

	local temp_cur_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_wing)
    player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_wing, temp_cur_value + 1)
    
	return hld.e_error_code_success
end

--[[
函数说明: 激活形态
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]
function wing_class:wing_shape_illusion_unlock(unit_array_index, wing_guid, illusion_slot)
	print("zym1")
    local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end
		print("zym1")
    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end
	print("zym1")
    local temp_wing_ptr = item_set_ref:get_item(tostring(wing_guid))
	if temp_wing_ptr == nil or temp_wing_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end
	print("zym1")
    local item_template_ptr = template_manager:get_line("ItemTemplate", temp_wing_ptr:get_item_template_id())
	if item_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end
	print("zym1")
	local current_wing_basic_level = temp_wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local current_wing_basic_actived = temp_wing_ptr:get_data_info(hld.e_item_info_activate)
	print("zym1")
	if current_wing_basic_actived == 0 then
		return ConstDefine.e_item_string_unkown
    end
	print("zym1")
    local real_id = item_template_ptr.logic_id + temp_wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local star_illusion_wing_template_ptr = template_manager:get_line("WingTemplate", real_id)
    local illusion_list_array = star_illusion_wing_template_ptr.IIllusionList
    if illusion_slot >= #(illusion_list_array) then
		return ConstDefine.e_item_string_have_no_illusion
    end
	print("zym1")
	local wing_illusion_item_template_id = illusion_list_array[illusion_slot + 1]
	print("zym1")
	if item_set_ref:get_item_by_template_id(wing_illusion_item_template_id) ~= nil then
		return ConstDefine.e_item_string_cant_unlock_again
    end

	print("zym1")
	local temp_wing_illusion_item_ptr = template_manager:get_line("ItemTemplate", wing_illusion_item_template_id)
	if temp_wing_illusion_item_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
	print("zym1")
	local illusion_wing_template = temp_wing_illusion_item_ptr.logic_id
	local temp_illusion_wing_template_ptr = template_manager:get_line("WingTemplate", illusion_wing_template)
	if temp_illusion_wing_template_ptr == nil then
		return ConstDefine.e_item_string_have_no_illusion
    end
	print("zym1")
	local real_wing_template_ptr = template_manager:get_line("WingTemplate", real_id)
	if real_wing_template_ptr == nil then
		return ConstDefine.e_item_string_have_no_illusion
    end
	print("zym1")
    local pre_wing_requirement_array = temp_illusion_wing_template_ptr.PreWingRequirement
    if pre_wing_requirement_array == nil then
		return ConstDefine.e_item_string_unkown
    end
		print("zym1")
	if #(pre_wing_requirement_array) >= 2 then
		if real_wing_template_ptr.UpgradeNow < pre_wing_requirement_array[2] then
			return ConstDefine.e_item_string_unkown
        end
    end
	print("zym1")
    local unlock_need_matirial_array = temp_illusion_wing_template_ptr.UnlockNeedMatirial
    if unlock_need_matirial_array == nil then
		return ConstDefine.e_item_string_unkown
    end
		print("zym1")
	if #(unlock_need_matirial_array) < hld.e_item_tuple_max or #(unlock_need_matirial_array) % hld.e_item_tuple_max ~= 0 then
		return ConstDefine.e_item_string_unkown
    end
	print("zym1")
    local is_cut_money = false
    local unlock_need_money_array = temp_illusion_wing_template_ptr.UnlockNeedMoney
    if unlock_need_money_array == nil then
		return ConstDefine.e_item_string_unkown
    end
		print("zym1")
	if #(unlock_need_money_array) >= hld.e_item_tuple_max then
		if not player_ref:can_cut_money(unlock_need_money_array[1], unlock_need_money_array[2]) then
			return ConstDefine.e_item_string_money
        end
		is_cut_money = true
    end
	print("zym1")
	local has_enough_item = item_set_ref:count_item_by_template_id(unlock_need_matirial_array[1], hld.e_bag_type_bag, -1) >= tonumber(unlock_need_matirial_array[2])
	if has_enough_item == false then
		return ConstDefine.e_item_string_matirial_not_enough
    end
	print("zym1")
	local illusion_item = item_set_ref:create_item_by_template(hld.e_server_log_add_item_wing_iilusion, 0, wing_illusion_item_template_id, 1, 0,0,0,0);
	if illusion_item == nil then
		print("zym2")
		return ConstDefine.e_item_string_unkown
    end
	print("zym1")
	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_iilusion, 0, unlock_need_matirial_array[1], unlock_need_matirial_array[2], hld.e_item_locked)
	if is_cut_money then
		player_ref:cut_money(unlock_need_money_array[1], unlock_need_money_array[2], hld.e_server_log_cut_money_get_wing_illusion, -1, 0)
    end
       	print("zym1") 
    item_set_ref:set_wing_att(false)

    illusion_item:set_data_info(hld.e_item_info_activate, 1)
    
    item_set_ref:set_wing_att(true)
    
    item_set_ref:send_item_one(illusion_item, true, 0);
    
    local temp_guid_string = hld.guid_transfer_to_string(temp_wing_ptr:get_item_guid_A(), temp_wing_ptr:get_item_guid_B())
    wing_class:show_this_wing(player_ref:get_array_index(), temp_guid_string, illusion_slot, false, false)

	return hld.e_error_code_success
end

--[[
函数说明: 形态升级
player_ref 角色引用
wing_guid   翅膀guid 传递的string
use_up_item 材料
--]]
function wing_class:wing_illusionupgrade(unit_array_index, wing_guid, use_up_item)
    
    local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
	end

    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return ConstDefine.e_item_string_unkown
    end

    local wing_ptr = item_set_ref:get_item(tostring(wing_guid))
	if wing_ptr == nil or wing_ptr:get_item_info_ptr() == nil then
		return ConstDefine.e_item_string_no_item
    end

    local item_template_ptr = template_manager:get_line("ItemTemplate", wing_ptr:get_item_template_id())
	if item_template_ptr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local use_money_instead_matirial = false
	local matirial_count = 0

	if wing_ptr:get_data_info(hld.e_item_info_activate) <= 0 then
		return ConstDefine.e_item_string_mount_unlock
    end

	local region_template_ptr = server_tool_mgr:get_region_template_ptr()
	if region_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local region_money_type_index = region_template_ptr.RegionCode * 2
	local region_money_num_index = region_money_type_index + 1

	local wing_real_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local real_wing_template_ptr = template_manager:get_line("WingTemplate", wing_real_id)
	if real_wing_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if real_wing_template_ptr.Type ~= hld.e_item_wing_type_illusion then
		return ConstDefine.e_item_string_auto_use_money_not_enough
    end

	if real_wing_template_ptr.UpgradeNow >= real_wing_template_ptr.WingUpgradeMax then
		return ConstDefine.e_item_string_full_level
    end

	local temp_cur_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount)
    player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1)

	matirial_count = item_set_ref:count_item_by_template_id(real_wing_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, -1)
    
    if matirial_count < real_wing_template_ptr.UpgradeMatirialNum then
		if use_up_item == 0 then
			return ConstDefine.e_item_string_matirial_not_enough
		else
			use_money_instead_matirial = true
        end
    end

	if use_money_instead_matirial then

        local new_price_array = template_manager:get_line("GoodsTemplate", real_wing_template_ptr.UpgradeShopId, "NewPrice")
        if new_price_array ~= nil and #(new_price_array) > region_money_num_index then
			local money_id  = new_price_array[1];
			local money_num = new_price_array[2] * real_wing_template_ptr.UpgradeMatirialNum
			if not player_ref:can_cut_money(money_id, money_num) then
                return ConstDefine.e_item_string_auto_use_money_not_enough
            end
			player_ref:cut_money(money_id, money_num, hld.e_server_log_cut_money_mount_upgrade_auto_buy, real_wing_template_ptr.UpgradeMatirialNum, 0)
		else
			return ConstDefine.e_item_string_no_item
        end
	else
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wing_illusionupgrade, 0, real_wing_template_ptr.UpgradeMatirialId, real_wing_template_ptr.UpgradeMatirialNum, hld.e_item_locked)
    end

    pawn_att_mgr:apply_att_change_by_array(player_ref,real_wing_template_ptr.AttArray, false, 1)

    local wing_upgrade_now = wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
    
    wing_ptr:set_data_info(hld.e_item_info_upgrade_count, wing_upgrade_now + 1)
    
    local new_wing_real_id = item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local new_real_wing_template_ptr = template_manager:get_line("WingTemplate", new_wing_real_id)
    if new_real_wing_template_ptr ~= nil then       
        pawn_att_mgr:apply_att_change_by_array(player_ref,new_real_wing_template_ptr.AttArray, true, 1)
		if new_real_wing_template_ptr.StarBuffId > 0 then
            item_set_ref:star_skill_unlock(wing_guid, wing_ptr:get_data_info(hld.e_item_info_upgrade_count), false, false)
        end
	else
        wing_ptr:set_data_info(hld.e_item_info_upgrade_count, wing_upgrade_now)
        local old_real_wing_template_ptr = template_manager:get_line("WingTemplate", item_template_ptr.logic_id + wing_upgrade_now)
        if old_real_wing_template_ptr ~= nil then
            pawn_att_mgr:apply_att_change_by_array(player_ref,old_real_wing_template_ptr.AttArray, true, 1)
        end
    end
    
    item_set_ref:send_item_one(wing_ptr, true, 0)

    local final_wing_template_ptr = template_manager:get_line("WingTemplate", item_template_ptr.logic_id + wing_ptr:get_data_info(hld.e_item_info_upgrade_count))
    if final_wing_template_ptr ~= nil then
        local MsgData =
        {
            show_type = hld.e_item_upgrade_effect_type_wing_upgrade,
            show_value = final_wing_template_ptr.UpgradeNow
        }
        MessageMgr:send_message_to_self(player_ref, "hld.item_proto.show_item_upgrade_effect", MsgData, 10723)
    end

	return hld.e_error_code_success;
end
----------------------------------------------------------------------------------------------------------------------------------------

wing = wing_class
return wing