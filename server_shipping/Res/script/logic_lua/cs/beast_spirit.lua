local beast_spirit_class = {}

function beast_spirit_class:init()
end

----------------------------------------------------------------------------------------------------------------------------------------
function beast_spirit_class:use_beast_spirit(unit_array_index, item_guid, item_num)
	
    local player_ref = hld.get_player_by_index(tonumber(unit_array_index))
	if player_ref == nil then
		return 0
	end

    local item_set_ref = player_ref:get_item_set()
	if item_set_ref == nil then
        return 0
    end

	local item_ptr = item_set_ref:get_item(tostring(item_guid))
	if item_ptr == nil or item_ptr:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_bag then
		return 0
	end
	
	local item_template_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id())
	if item_template_ptr == nil then
		return 0
    end

	if item_ptr:get_data_info(hld.e_item_info_stack_count) < item_num or item_num <= 0 then
        return 0
    end	

    local beast_spirit_template_ptr = template_manager:get_line("BeastSpiritTemplate", item_template_ptr.logic_id)
	if beast_spirit_template_ptr == nil then
		return 0
    end

    local is_upgrade = false
    local unlock_condition_array = beast_spirit_template_ptr.UnlockConditionArray
	for i = 1, #(unlock_condition_array), 2 do
		local item_mount_or_wing_ptr = item_set_ref:get_item_by_template_id(unlock_condition_array[i])
		if item_mount_or_wing_ptr ~= nil then
			local logic_id = template_manager:get_field("ItemTemplate", item_mount_or_wing_ptr:get_item_template_id(), "logic_id")
            local upgrade_count = item_mount_or_wing_ptr:get_data_info(hld.e_item_info_upgrade_count)
		    if item_set_ref:is_class_enable(item_mount_or_wing_ptr) == true then
				if beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_mount then
					local mount_template_ptr = template_manager:get_line("MountTemplate", logic_id + upgrade_count)
					if mount_template_ptr ~= nil then
					    if tonumber(mount_template_ptr.ClassNow) >= tonumber(unlock_condition_array[i + 1]) then
						    is_upgrade = true
                            break
                        end
					end
				elseif beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_wing then
					local wing_template_ptr = template_manager:get_line("WingTemplate", logic_id + upgrade_count)
					if wing_template_ptr ~= nil then
					    if tonumber(wing_template_ptr.UpgradeNow) >= tonumber(unlock_condition_array[i + 1]) then
						    is_upgrade = true
						    break
                        end
                    end
				end
			end
        end
    end

	if beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_spirit then
		is_upgrade = true
    end

	if is_upgrade == false then
		return 0
    end

    item_set_ref:change_beast_spirit_att(beast_spirit_template_ptr.BeastSpiritType, false)
    
    if item_set_ref:set_mount_or_wing_beast_spirit_bag_data(item_ptr:get_data_info(hld.e_item_info_info_id), beast_spirit_template_ptr.BeastSpiritType, item_num) then
        
        item_set_ref:change_beast_spirit_att(beast_spirit_template_ptr.BeastSpiritType, true)

		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_use_beast, 0, item_ptr:get_data_info(hld.e_item_info_info_id), item_num, 1);

		if beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_mount then
			item_set_ref:sync_ranking_mount();
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_mount_beast_spirit, 0);
		elseif beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_wing then
			item_set_ref:sync_ranking_wing()
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_wing_beast_spirit, 0);
        elseif beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_spirit then
            item_set_ref:sync_ranking_spirit();
		end
		return 1
	end

    if beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_mount then
        item_set_ref:sync_ranking_mount();
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_mount_beast_spirit, 0);
    elseif beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_wing then
        item_set_ref:sync_ranking_wing()
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_wing_beast_spirit, 0);
    elseif beast_spirit_template_ptr.BeastSpiritType == hld.e_beast_spirit_type_spirit then
        item_set_ref:sync_ranking_spirit();
    end

    item_set_ref:change_beast_spirit_att(beast_spirit_template_ptr.BeastSpiritType, true)
	return 0
end
----------------------------------------------------------------------------------------------------------------------------------------
beast_spirit = beast_spirit_class
return beast_spirit