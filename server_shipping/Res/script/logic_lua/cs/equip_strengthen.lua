local equip_strengthen={}
local player_ref
local player_arrar_index
function equip_strengthen:init()
end


--强化入口
function item_leve_up(m_unit_array_index, item_guid_data, bSuccess, use_up_item, first_use_lock)
    print("enter",item_guid_data)
    player_arrar_index = m_unit_array_index
    player_ref = hld.get_player_by_index(player_arrar_index)
    if player_ref == nil or player_arrar_index == nil then
        return
    end
    if item_guid_data ~= nil then
        bSuccess=item_upgrade(item_guid_data, use_up_item, first_use_lock)
    end
end

function item_upgrade(item_guid_data, protect_item_index, first_use_lock)
    if player_ref:is_valid()==false then
        return hld.e_error_code_item_upgrade
    end
    print("pass",player_ref:get_item_set())
    local item_ptr = player_ref:get_item_set():get_item(item_guid_data)
    print("test1",item_ptr)
    if item_ptr == nil then
        return hld.e_error_code_item_upgrade --没有物品
    end

    if item_ptr:get_item_info_ptr() == nil then
        return hld.e_error_code_item_upgrade
    end
    local equip_template_ptr = get_equip_info(item_ptr)
    print("test2",equip_template_ptr)
    if equip_template_ptr == nil then
        return hld.e_error_code_item_upgrade
    end
    local item_upgrade_ptr ,upgrade_template_id= get_upgrade_info(item_ptr)
    print("test3",item_upgrade_ptr)
    if item_upgrade_ptr == nil then
        return hld.e_error_code_item_upgrade
    end
    local Upgrade_Template_Rate = item_upgrade_ptr.Rate
    local Upgrade_Template_ProtectItemInfo = item_upgrade_ptr.ProtectItemInfo
    local Upgrade_Template_MoneyCostList = item_upgrade_ptr.MoneyCostList
    local Upgrade_Template_ItemIdList = item_upgrade_ptr.ItemIdList
    local Upgrade_Template_ItemNumList = item_upgrade_ptr.ItemNumList
    if #Upgrade_Template_Rate <= 0 or #Upgrade_Template_ProtectItemInfo % 2 ~= 0 then
        return hld.e_error_code_item_upgrade
    end
    local game_config_template_ptr =template_manager:get_line("GameConfigTemplate",ConstDefine.game_config_template_id)
    if game_config_template_ptr == nil then
        return hld.e_error_code_item_upgrade
    end
    local temp_protect_data_num = #Upgrade_Template_ProtectItemInfo / 2
    local upgrade_num_now = item_ptr:get_data_info(hld.e_item_info_upgrade_count)

    --在这做个判断  判断强化的限时活动是否开启   开启了就设置开启的值
    local upgrade_max_num
    if player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_add_upgrade_level) then
        upgrade_max_num = equip_template_ptr.limit_activity_equip_upgrade_num
    else
        upgrade_max_num = equip_template_ptr.equip_upgrade_num
    end

    if upgrade_num_now >= upgrade_max_num then  --满级
        return hld.e_error_code_item_full_level
    end
    if #Upgrade_Template_MoneyCostList % 2 ~= 0 then
        return hld.e_error_code_item_upgrade
    end
    for i=1,#Upgrade_Template_MoneyCostList,2 do    --金钱判断
        if not player_ref:can_cut_money(Upgrade_Template_MoneyCostList[i], Upgrade_Template_MoneyCostList[i+1]) then
            return hld.e_error_code_item_money
        end
    end
    for i=1,#Upgrade_Template_ItemIdList do   --材料判断
        if Upgrade_Template_ItemIdList[i] == 0 then
            break
        end
        if player_ref:get_item_set():count_item_by_template_id(Upgrade_Template_ItemIdList[i],hld.e_bag_type_bag) < Upgrade_Template_ItemIdList[i] then
            return hld.e_error_code_item_matirial_not_enough
        end
    end

    --强化道具判断
    --记录使用的道具
    local protect_material_id = 0;
    local protect_material_num = 0;
    local max_down_num = hld.e_upgrade_result_max;
    local success_rate_add_value = 0;
    if protect_item_index >= 0 then
        local protect_use_state = upgrade_protect_item_can_use(Upgrade_Template_ProtectItemInfo, upgrade_num_now, protect_item_index)
        if protect_use_state ~= e_error_type_succeed then
            return protect_use_state
        end
        protect_material_id = Upgrade_Template_ProtectItemInfo[protect_item_index * 2 + 1]
        protect_material_num = Upgrade_Template_ProtectItemInfo[protect_item_index * 2 + 2]

        local protect_item_template_ptr = template_manager:get_line("ItemTemplate", protect_material_id)
        if protect_item_template_ptr == nil then
            return hld.e_error_code_item_upgrade
        end
        local protect_props_template_UpgradeProtectInfoArray = template_manager:get_field("PropBasicAttributeTemplate", protect_item_template_ptr.logic_id,"UpgradeProtectInfoArray")
        if protect_props_template_UpgradeProtectInfoArray == nil then
            return hld.e_error_code_item_upgrade
        end

        if #protect_props_template_UpgradeProtectInfoArray >= 2 then
            if protect_props_template_UpgradeProtectInfoArray[1] == hld.e_item_upgrade_protect_type_low_level_undown or protect_props_template_UpgradeProtectInfoArray[1] == hld.e_item_upgrade_protect_type_high_level_undown then
                max_down_num = protect_props_template_UpgradeProtectInfoArray[2]
            elseif protect_props_template_UpgradeProtectInfoArray[1] == hld.e_item_upgrade_protect_type_low_level_undown then
                success_rate_add_value = protect_props_template_UpgradeProtectInfoArray[2]
            else

            end
        end
    end
    --记录货币id和数量
	local money_type = 0
    local money_num = 0
    if #Upgrade_Template_MoneyCostList == 2 then
        money_type = Upgrade_Template_MoneyCostList[1]
		money_num = Upgrade_Template_MoneyCostList[2];
    end
    --可以升级
    for i=1,#Upgrade_Template_MoneyCostList,2 do    --花钱
        player_ref:cut_money(Upgrade_Template_MoneyCostList[i], Upgrade_Template_MoneyCostList[i+1], hld.e_server_log_cut_money_item_upgrade, item_ptr:get_data_info(hld.e_item_info_info_id))
    end
    for i=1,#Upgrade_Template_ItemIdList do   --所有材料
        if Upgrade_Template_ItemIdList[i] == 0 then
            break
        end
        repeat
            if Upgrade_Template_ItemIdList[i] == ConstDefine.gods_reel or Upgrade_Template_ItemIdList[i] == ConstDefine.must_success_props_item or Upgrade_Template_ItemIdList[i] == ConstDefine.not_downgrade_props_item then
                break
            end
            if first_use_lock == 1 and player_ref:get_item_set():count_item_by_template_id(Upgrade_Template_ItemIdList[i], hld.e_bag_type_bag, 1) > 0 then
                item_ptr:set_data_info(hld.e_item_info_locked, 1)
            elseif player_ref:get_item_set():count_item_by_template_id(Upgrade_Template_ItemIdList[i], hld.e_bag_type_bag, 0) < Upgrade_Template_ItemNumList[i] then
                item_ptr:set_data_info(hld.e_item_info_locked, 1)   --使用到了绑定素材
            end
            player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_upgrade, item_ptr:get_data_info(hld.e_item_info_info_id) , Upgrade_Template_ItemIdList[i], Upgrade_Template_ItemNumList[i], first_use_lock)
        until true
    end
    --记录消耗的材料
    local material_one_id = 0;
    local material_two_id = 0;
    local material_one_num = 0;
    local material_two_num = 0;
    if #Upgrade_Template_ItemNumList >= 1 then
        material_one_id = Upgrade_Template_ItemList[1]
		material_one_num = Upgrade_Template_ItemNumList[1]
    end
    if #Upgrade_Template_ItemNumList >= 2 then
        material_two_id = Upgrade_Template_ItemList[2];
		material_two_num = Upgrade_Template_ItemNumList[2];
    end
    --if
end

function get_equip_info(item_ptr)
    if item_ptr== nil then
        return
    end
    return template_manager:get_line("EquipTemplate",item_ptr:get_item_logic_id())
end

function get_upgrade_info(equip_template_ptr, item_ptr)
    if item_ptr== nil or equip_template_ptr == nil then
        return
    end
    local upgrade_template_id = equip_template_ptr.UpgradeTemplateId+item_ptr:get_item_inst().data_ary[hld.e_item_info_upgrade_count]
    return template_manager:get_line("ItemUpgradeTemplate",upgrade_template_id), upgrade_template_id
end

function upgrade_protect_item_can_use(Upgrade_Template_ProtectItemInfo, cur_upgrade_num, protect_item_index)
    if protect_item_index < 0 then
        return hld.e_error_code_item_upgrade
    end
    if #Upgrade_Template_ProtectItemInfo % 2 ~= 0 then
        return hld.e_error_code_item_upgrade
    end
    local data_num = #Upgrade_Template_ProtectItemInfo / 2
    if protect_item_index >= data_num then
        return hld.e_error_code_item_upgrade
    end
    local item_id = Upgrade_Template_ProtectItemInfo[protect_item_index * 2 + 1]
    local item_num = Upgrade_Template_ProtectItemInfo[protect_item_index * 2 + 2]
    if item_id <= 0 or item_num <= 0 then
        return hld.e_error_code_item_upgrade
    end

    if player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag) < item_num then
        return hld.e_error_code_item_matirial_not_enough
    end
    local item_template_ptr = template_manager:get_line("ItemTemplate", item_id)
    if item_template_ptr == nil then
        return hld.e_error_code_item_upgrade
    end
    local props_template_FuncLevelLimit = template_manager:get_field("PropBasicAttributeTemplate", item_template_ptr.logic_id,"FuncLevelLimit")
    if props_template_FuncLevelLimit == nil then
        return hld.e_error_code_item_upgrade
    end
    --如果使用小于作用下限或者大于作用上限，则不能使用
    if #props_template_FuncLevelLimit >= 2 then
        if cur_upgrade_num < props_template_FuncLevelLimit[1] or cur_upgrade_num > props_template_FuncLevelLimit[2] then
            return hld.e_error_code_item_upgrade
        end
    end
    return hld.e_error_code_success
end


return equip_strengthen
