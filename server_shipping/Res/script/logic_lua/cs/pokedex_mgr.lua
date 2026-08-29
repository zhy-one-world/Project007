local pokedex_mgr_class = {}

--[[
函数说明: 	初始化
--]]
function pokedex_mgr_class:init()
end

function pokedex_mgr_class:get_pokedex_info_ptr(pokedex_item)
	if nil == pokedex_item then
		return nil
    end
    local template_id = pokedex_item:get_data_info(hld.e_pokedex_Info_template_id) + pokedex_item:get_data_info(hld.e_pokedex_Info_upgrade_num)
    return template_manager:get_line("PokedexTemplate", template_id)
end


function pokedex_mgr_class:get_add_percent(temp_pokedex_mgr)
    local add_percent = 1.0
    local main_pokedex_ptr = temp_pokedex_mgr:get_palace_pokedex()
    if main_pokedex_ptr == nil then
        return add_percent
    end
    local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(main_pokedex_ptr)
    if pokedex_template_ptr == nil then
        return add_percent
    end
    local att_array = pokedex_template_ptr.AttArray
    if #(att_array) > 0 then
        add_percent = att_array[1] + 1
    end
    if add_percent < 1 then
        return 1.0
    end
    return add_percent
end
---------------------------------------------------------------------------------------------------------------------------------------
define_C[11501] = function(message_data, player_ref)

	local msg_data = decode("hld.pokedex_proto.pokedex_operate", message_data)
    pokedex_mgr_class:pokedex_operate(player_ref, msg_data["pokedex_template_id"], msg_data["operation_type"], msg_data["operate_index"])

end

--[[
函数说明: 处理消息接口
--]]
function pokedex_mgr_class:pokedex_operate(player_ref, pokedex_template_id, operation_type, operate_index)

	if player_ref == nil then
		return
	end

	local operate_result = 0

    if operation_type == hld.e_pokedex_operation_type_submit_psyche then
        operate_result = pokedex_mgr_class:pokedex_submit_psyche(player_ref, pokedex_template_id, operate_index)

    elseif operation_type == hld.e_pokedex_operation_type_change then
        operate_result = pokedex_mgr_class:submit_psyche_all(player_ref, pokedex_template_id)

    elseif operation_type == hld.e_pokedex_operation_type_main_palace_upgrade then
        operate_result = pokedex_mgr_class:main_palace_upgrade(player_ref, pokedex_template_id)

    elseif operation_type == hld.e_pokedex_operation_type_oracle_activate then
        operate_result = pokedex_mgr_class:oracle_activate(player_ref, pokedex_template_id)

    elseif operation_type == hld.e_pokedex_operation_type_oracle_upgrade or operation_type == hld.e_pokedex_operation_type_mainland_upgrade then
        operate_result = pokedex_mgr_class:pokedex_upgrade(player_ref, pokedex_template_id)
    
    elseif operation_type == hld.e_pokedex_operation_type_mainland_activate then
        operate_result = pokedex_mgr_class:mainland_activate(player_ref, pokedex_template_id)

    end

	local MsgData = 
	{
        operate_type = operation_type,
        operate_result = operate_result,
	}
	MessageMgr:send_message_to_self(player_ref, "hld.pokedex_proto.pokedex_operate_result_end", MsgData, 11502)
end
----------------------------------------------------------------------------------------------------------------------------------------
--[[
函数说明: 单个提交
--]]
function pokedex_mgr_class:pokedex_submit_psyche(player_ref, pokedex_template_id, operate_index)
    
    if player_ref == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local temp_item_set = player_ref:get_item_set()
    if temp_item_set == nil then
        return ConstDefine.e_item_string_unkown
    end

    local temp_pokedex_mgr = player_ref:get_pokedex_mgr()
    if temp_pokedex_mgr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_templateid(pokedex_template_id)
	if pokedex_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
	if pokedex_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
		
    local cur_choosed_psyche_num = pokedex_ptr:get_data_info(hld.e_pokedex_Info_psyche_num_1 + operate_index)
    local psyche_id_array = pokedex_template_ptr.PsycheIdArray
    if psyche_id_array == nil then
        return ConstDefine.e_item_string_unkown
    end
	local max_submit_psyche_num  = psyche_id_array[operate_index * 2 + 2]
	local total_psyche_num       = #(psyche_id_array) / 2
	if operate_index < total_psyche_num and total_psyche_num <= PokedexDef.POKEDEX_PSYCHE_MAX then
        
        if cur_choosed_psyche_num >= max_submit_psyche_num then
			return ConstDefine.e_item_string_can_not_use
        end
        
		local cur_has_num = temp_item_set:count_item_by_template_id(psyche_id_array[operate_index * 2 + 1], hld.e_bag_type_bag, -1)
		if cur_has_num <= 0 then
		    return ConstDefine.e_item_string_matirial_not_enough
        end
        
        if cur_has_num <= max_submit_psyche_num - cur_choosed_psyche_num then
			temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_submit_psyche, 0, psyche_id_array[operate_index * 2 + 1], cur_has_num, hld.e_item_locked)
			pokedex_ptr:set_data_info(hld.e_pokedex_Info_psyche_num_1 + operate_index, cur_choosed_psyche_num + cur_has_num)
		else
			temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_submit_psyche, 0, psyche_id_array[operate_index * 2 + 1], max_submit_psyche_num - cur_choosed_psyche_num, hld.e_item_locked)
			pokedex_ptr:set_data_info(hld.e_pokedex_Info_psyche_num_1 + operate_index, max_submit_psyche_num)
        end
		temp_pokedex_mgr:send_pokedex_one(pokedex_template_id);
	else
		return ConstDefine.e_item_string_unkown
    end
    return 0
end

--[[
函数说明: 一键提交
--]]
function pokedex_mgr_class:submit_psyche_all(player_ref, pokedex_template_id)
    
    if player_ref == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local temp_item_set = player_ref:get_item_set()
    if temp_item_set == nil then
        return ConstDefine.e_item_string_unkown
    end

    local temp_pokedex_mgr = player_ref:get_pokedex_mgr()
    if temp_pokedex_mgr == nil then
        return ConstDefine.e_item_string_unkown
    end

    local MsgData = {}
    MsgData.submit_item_array = {}

	local can_submit = 0
    for i = 0, PokedexDef.POKEDEX_NUM_MAX - 1, 1 do
        local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_index(i)
        if pokedex_ptr ~= nil then
			if pokedex_ptr:get_data_info(hld.e_pokedex_Info_template_id) > 0 then
				if pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) == 0 then
				    local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
                    if pokedex_template_ptr ~= nil and pokedex_template_ptr.Type >= 2 then
                        local psyche_id_array = pokedex_template_ptr.PsycheIdArray
				        if #(psyche_id_array) >= 2 and #(psyche_id_array) / 2 <= PokedexDef.POKEDEX_PSYCHE_MAX then
					        for j = 0, #(psyche_id_array) / 2 - 1, 1 do
						        local template_id    = psyche_id_array[j * 2 + 1]
						        local max_submit_num = psyche_id_array[j * 2 + 2]
						        local cur_submit_num = pokedex_ptr:get_data_info(hld.e_pokedex_Info_psyche_num_1 + j)
						        local cur_has_num    = temp_item_set:count_item_by_template_id(template_id, hld.e_bag_type_bag, -1)
						        if cur_submit_num >= tonumber(max_submit_num) then
							        pokedex_ptr:set_data_info(hld.e_pokedex_Info_psyche_num_1 + j, max_submit_num)
                                elseif cur_has_num > 0 then
						            if cur_has_num <= max_submit_num - cur_submit_num then
							            temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_submit_psyche, 0, template_id, cur_has_num, hld.e_item_locked)
							            pokedex_ptr:set_data_info(hld.e_pokedex_Info_psyche_num_1 + j, cur_submit_num + cur_has_num)
							            can_submit = 1
                                    
                                        table.insert(MsgData.submit_item_array, pokedex_ptr:get_data_info(hld.e_pokedex_Info_template_id))
							            table.insert(MsgData.submit_item_array, template_id)
							            table.insert(MsgData.submit_item_array, cur_has_num)
							            temp_pokedex_mgr:send_pokedex_one(pokedex_ptr:get_data_info(hld.e_pokedex_Info_template_id))
						            else
							            temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_submit_psyche, 0, template_id, max_submit_num - cur_submit_num, hld.e_item_locked)
							            pokedex_ptr:set_data_info(hld.e_pokedex_Info_psyche_num_1 + j, max_submit_num)
							            can_submit = 1

                                        table.insert(MsgData.submit_item_array, pokedex_ptr:get_data_info(hld.e_pokedex_Info_template_id))
							            table.insert(MsgData.submit_item_array, template_id)
							            table.insert(MsgData.submit_item_array, max_submit_num - cur_submit_num);
							            temp_pokedex_mgr:send_pokedex_one(pokedex_ptr:get_data_info(hld.e_pokedex_Info_template_id))
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    end

    MessageMgr:send_message_to_self(player_ref, "hld.pokedex_proto.pokedex_operate_end", MsgData, 11504)
    return 0
end

--[[
函数说明: 共鸣升级
--]]
function pokedex_mgr_class:main_palace_upgrade(player_ref, pokedex_template_id)
    
    if player_ref == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local temp_item_set = player_ref:get_item_set()
    if temp_item_set == nil then
        return ConstDefine.e_item_string_unkown
    end

    local temp_pokedex_mgr = player_ref:get_pokedex_mgr()
    if temp_pokedex_mgr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_templateid(pokedex_template_id)
	if pokedex_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
	if pokedex_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	local upgrade_level = pokedex_ptr:get_data_info(hld.e_pokedex_Info_upgrade_num)
	if upgrade_level >= pokedex_template_ptr.MaxLevel then
        return ConstDefine.e_item_string_full_level
    end
	
	local next_pokedex_template_ptr = template_manager:get_line("PokedexTemplate", pokedex_ptr:get_data_info(hld.e_pokedex_Info_template_id) + upgrade_level + 1)
	if next_pokedex_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local upgrade_cost = pokedex_template_ptr.UpgradeCost
    if #(upgrade_cost) >= 2 then
		local cur_has_num = temp_item_set:count_item_by_template_id(upgrade_cost[1], hld.e_bag_type_bag, -1)
		if cur_has_num < tonumber(upgrade_cost[2]) then
			return ConstDefine.e_item_string_matirial_not_enough
        end
		temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_mainpalace_upgrade, 0, upgrade_cost[1], upgrade_cost[2], hld.e_item_locked)
		pokedex_ptr:set_data_info(hld.e_pokedex_Info_upgrade_num, upgrade_level + 1)
    end
    
    temp_pokedex_mgr:send_pokedex_one(pokedex_template_id)
    
    local add_percent = 0.0

    local cur_att_array = pokedex_template_ptr.AttArray
    local next_att_array = next_pokedex_template_ptr.AttArray
    if #(cur_att_array) > 0 and #(next_att_array) > 0 then
		add_percent = next_att_array[1] - cur_att_array[1]
    end
    
    for i = 0, PokedexDef.POKEDEX_NUM_MAX - 1, 1 do
        local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_index(i)
        if pokedex_ptr ~= nil then
		    local temp_pokedex_template = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
            if temp_pokedex_template ~= nil and pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) > 0 then

                pawn_att_mgr:apply_att_change_by_array(player_ref, temp_pokedex_template.AttArray, true, add_percent)

                local fetter_arr = temp_pokedex_template.FetterArr
                local fetter_num = #(fetter_arr)
		        if temp_pokedex_template.Type == 1 and fetter_num > 0 then
                    for j = 0, fetter_num - 1, 1 do
                        if pokedex_ptr:get_data_info(hld.e_pokedex_Info_fetter_state_1 + j) > 0 then
				            local psyche_template_ptr = template_manager:get_line("PsycheTemplate", fetter_arr[j + 1])
                            if psyche_template_ptr ~= nil then
                                pawn_att_mgr:apply_att_change_by_array(player_ref, psyche_template_ptr.FetterAttArray, true, add_percent)
                            end
                        end
                    end
                end
            end
		end
	end
	return 0
end

--[[
函数说明: 人物激活
--]]
function pokedex_mgr_class:oracle_activate(player_ref, pokedex_template_id)

	if player_ref == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local temp_item_set = player_ref:get_item_set()
    if temp_item_set == nil then
        return ConstDefine.e_item_string_unkown
    end

    local temp_pokedex_mgr = player_ref:get_pokedex_mgr()
    if temp_pokedex_mgr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_templateid(pokedex_template_id)
	if pokedex_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) > 0 then
		return ConstDefine.e_item_string_unkown
    end

	local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
	if pokedex_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local unlock_cost = pokedex_template_ptr.UnlockCost
	if #(unlock_cost) >= 2 then
		local cur_has_num = temp_item_set:count_item_by_template_id(unlock_cost[1], hld.e_bag_type_bag, -1)
		if cur_has_num < tonumber(unlock_cost[2]) then
			return ConstDefine.e_item_string_matirial_not_enough
        end
		temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_oracle_activate, 0, unlock_cost[1], unlock_cost[2], hld.e_item_locked)
		pokedex_ptr:set_data_info(hld.e_pokedex_Info_state, 1)
		temp_pokedex_mgr:send_pokedex_one(pokedex_template_id)

        local add_percent = pokedex_mgr_class:get_add_percent(temp_pokedex_mgr)
        pawn_att_mgr:apply_att_change_by_array(player_ref, pokedex_template_ptr.AttArray, true, add_percent)

		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_pokedex_activation, 0, 0)
		player_ref:refresh_service_goal(hld.e_service_goal_type_actived_illustration_num)
		player_ref:refresh_service_goal(hld.e_service_goal_type_actived_any_chapter_illustration)
    end
    
    pokedex_mgr_class:check_activate_fetter(player_ref, temp_pokedex_mgr, pokedex_template_id);
	return 0
end

function pokedex_mgr_class:check_activate_fetter(player_ref, temp_pokedex_mgr, pokedex_template_id)

	local add_percent = pokedex_mgr_class:get_add_percent(temp_pokedex_mgr) 
    for i = 0, PokedexDef.POKEDEX_NUM_MAX - 1, 1 do
        local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_index(i)
        if pokedex_ptr ~= nil and pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) > 0 then
			local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
            if pokedex_template_ptr ~= nil then               
                if pokedex_template_ptr.Type == 1 then
                    local fetter_arr = pokedex_template_ptr.FetterArr
                    if #(fetter_arr) > 0 then                   
					    for j = 0, #(fetter_arr) - 1, 1 do
                            if pokedex_ptr:get_data_info(hld.e_pokedex_Info_fetter_state_1 + j) == 0 then
						        local psyche_template_ptr = template_manager:get_line("PsycheTemplate", fetter_arr[j+1])
                                if psyche_template_ptr ~= nil then
                                    local fetter_activate_need_array = psyche_template_ptr.FetterActivateNeedArray
                                    if #(fetter_activate_need_array) > 0 then
                                        local is_contain = 0
							            for len = 0, #(fetter_activate_need_array) - 1, 1  do
								            if tonumber(pokedex_template_id) == tonumber(fetter_activate_need_array[len + 1]) then
									            is_contain = 1
                                                break
                                            end
                                        end
							            local total_num = 0;
                                        if is_contain > 0 then
								            for len = 0, #(fetter_activate_need_array) - 1, 1 do
									            local temp_pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_templateid(fetter_activate_need_array[len+1])
									            if temp_pokedex_ptr ~= nil and temp_pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) > 0 then
										            total_num = total_num + 1
                                                end
								            end
    
                                            if total_num == #(fetter_activate_need_array) then
                                                pokedex_ptr:set_data_info(hld.e_pokedex_Info_fetter_state_1 + j, 1)
                                                pawn_att_mgr:apply_att_change_by_array(player_ref, psyche_template_ptr.FetterAttArray, true, add_percent)
                                            end
                                        end
                                    end
                                end
                            end
						end
					end
                end
            end
        end
    end
    temp_pokedex_mgr:send_pokedex_all()
end

--[[
函数说明: 人物升级
--]]
function pokedex_mgr_class:pokedex_upgrade(player_ref, pokedex_template_id)

    if player_ref == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local temp_item_set = player_ref:get_item_set()
    if temp_item_set == nil then
        return ConstDefine.e_item_string_unkown
    end

    local temp_pokedex_mgr = player_ref:get_pokedex_mgr()
    if temp_pokedex_mgr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_templateid(pokedex_template_id)
	if pokedex_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

	if pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) <= 0 then
		return ConstDefine.e_item_string_unkown
    end
    
    local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
	if pokedex_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    if pokedex_ptr:get_data_info(hld.e_pokedex_Info_upgrade_num) >= pokedex_template_ptr.MaxLevel then
		return ConstDefine.e_item_string_full_level
    end
    
    local upgrade_cost = pokedex_template_ptr.UpgradeCost
	if #(upgrade_cost) < 2 then
        return ConstDefine.e_item_string_unkown
    end
	if pokedex_template_ptr.Type == 2 or pokedex_template_ptr.Type == 3 then
		if player_ref:can_cut_money(upgrade_cost[1], upgrade_cost[2]) == false then
			return hld.e_fund_money_not_enough
        end
		player_ref:cut_money(upgrade_cost[1], upgrade_cost[2], hld.e_server_log_cut_money_pokedex_upgrade, -1, 0)
	end
	if pokedex_template_ptr.Type == 1 then
		local cur_has_num = temp_item_set:count_item_by_template_id(upgrade_cost[1], hld.e_bag_type_bag, -1)
		if cur_has_num < tonumber(upgrade_cost[2]) then
			return ConstDefine.e_item_string_matirial_not_enough
        end
		temp_item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_pokedex_upgrade, 0, upgrade_cost[1], upgrade_cost[2], hld.e_item_locked)
	end
	pokedex_ptr:set_data_info(hld.e_pokedex_Info_upgrade_num, pokedex_ptr:get_data_info(hld.e_pokedex_Info_upgrade_num) + 1)
	temp_pokedex_mgr:send_pokedex_one(pokedex_template_id)

	local next_pokedex_template_ptr = template_manager:get_line("PokedexTemplate", pokedex_template_id + pokedex_ptr:get_data_info(hld.e_pokedex_Info_upgrade_num))
	if next_pokedex_template_ptr ~= nil then
        local add_percent = pokedex_mgr_class:get_add_percent(temp_pokedex_mgr)
        pawn_att_mgr:apply_att_change_by_array(player_ref, pokedex_template_ptr.AttArray, false, add_percent)
        pawn_att_mgr:apply_att_change_by_array(player_ref, next_pokedex_template_ptr.AttArray, true, add_percent)
    end
	return 0
end

--[[
函数说明: 图鉴激活
--]]
function pokedex_mgr_class:mainland_activate(player_ref, pokedex_template_id)
    
    if player_ref == nil then
		return ConstDefine.e_item_string_unkown
    end
    
    local temp_item_set = player_ref:get_item_set()
    if temp_item_set == nil then
        return ConstDefine.e_item_string_unkown
    end

    local temp_pokedex_mgr = player_ref:get_pokedex_mgr()
    if temp_pokedex_mgr == nil then
        return ConstDefine.e_item_string_unkown
    end

	local pokedex_ptr = temp_pokedex_mgr:get_pokedex_by_templateid(pokedex_template_id)
	if pokedex_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    if pokedex_ptr:get_data_info(hld.e_pokedex_Info_state) > 0 then
		return ConstDefine.e_item_string_unkown
    end
    
    local pokedex_template_ptr = pokedex_mgr_class:get_pokedex_info_ptr(pokedex_ptr)
	if pokedex_template_ptr == nil then
		return ConstDefine.e_item_string_unkown
    end

    local psyche_id_array = pokedex_template_ptr.PsycheIdArray
	if #(psyche_id_array )/ 2 <= 0 then
		return ConstDefine.e_item_string_unkown
    end
        
    local total_collect_num = 0
	for i = 0, #(psyche_id_array)/2 - 1, 1 do
		if pokedex_ptr:get_data_info(hld.e_pokedex_Info_psyche_num_1 + i) < tonumber(psyche_id_array[i * 2 + 2]) then
			break
        end
        total_collect_num = total_collect_num + 1
    end
        
	if total_collect_num < #(psyche_id_array) / 2 then
		return ConstDefine.e_item_string_unkown
    end

    pokedex_ptr:set_data_info(hld.e_pokedex_Info_state, 1)
    
    temp_pokedex_mgr:send_pokedex_one(pokedex_template_id)
    
    local add_percent = pokedex_mgr_class:get_add_percent(temp_pokedex_mgr)
    
    pawn_att_mgr:apply_att_change_by_array(player_ref, pokedex_template_ptr.AttArray, true, add_percent)

	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_pokedex_activation, 0, 0)

	player_ref:refresh_service_goal(hld.e_service_goal_type_actived_illustration_num)
    player_ref:refresh_service_goal(hld.e_service_goal_type_actived_any_chapter_illustration)
    
	return 0
end
------------------------------------------------------------------------------------------------------------------------------------------
pokedex_mgr = pokedex_mgr_class
return pokedex_mgr