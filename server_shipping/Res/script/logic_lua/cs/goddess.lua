
local goddess = {}

local enum_goddess_operate_type =
{
     none = 0,  --空
     unlock = 1,  --女神解锁
     equip = 2,  --装备女神
     unequip = 3,  --卸下女神
     fight = 4,  --女神出战
     equip_upgrade = 5,  --女神装备升级
     goddess_slot_unlock = 6, --女神槽位解锁
     goddess_star_upgrade  = 7, -- 女神升星
     max  = 7,
};

local max_goddess_equip_num = 28

function goddess:init()
	self.m_function = {}
	self.m_function[enum_goddess_operate_type.unlock] = self.goddess_unlock;
	self.m_function[enum_goddess_operate_type.fight] =  self.goddess_fight;
	self.m_function[enum_goddess_operate_type.equip_upgrade] = self.goddess_equip_upgrade;
	self.m_function[enum_goddess_operate_type.goddess_slot_unlock] = self.cooperate_slot_unlock;
	self.m_function[enum_goddess_operate_type.goddess_star_upgrade] = self.goddess_star_upgrade;
end


define_C[10751] = function(message_data, player_ref)
    local msg_data = decode("hld.item_proto.item_operation", message_data)
    if msg_data == nil then
     	return
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	goddess:goddess_operate(player_ref,math.floor(msg_data.operation_type), msg_data.item_guid, msg_data.item_slot, msg_data.item_num);
end


function goddess:goddess_operate(player_ref,  operate_type, t_item_guid, slot, n_item_num)
	delog("---------goddess_operate------" , operate_type, slot, n_item_num)
	local result  = -1

	local fun = self.m_function[operate_type]
	if fun == nil then
		return
    end
    result = fun(self, player_ref, t_item_guid, slot, n_item_num)

    local msg_data = 
    {
        operation_state = 0,
        operation_type = 0,
        --role_guid  = {0,0},
        --item_guid = {0,0},
        --item_slot = 0,
        --item_num = 0,
    }
    msg_data.operation_state = result
    msg_data.operation_type = operate_type

    delog(msg_data)

    MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_operation_end",msg_data, 10752)
end

function goddess:goddess_unlock(player_ref, t_item_guid, slot, num)

	  delog("----goddess_unlock------", t_item_guid, slot)
	 local item_ptr = player_ref:get_item_set():get_item(t_item_guid[1], t_item_guid[2])
	 if item_ptr == nil then
	 	return ConstDefine.e_item_string_unkown
	 end
	 local t_item_template = self:get_item_template(item_ptr);
	 if t_item_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	 end
	 local t_deity_template = self:get_deity_template(item_ptr);
	 if t_deity_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	 end
	 if item_ptr:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_goddess_equip then
	 	return ConstDefine.e_item_string_unkown;
	 end
	 if t_item_template.item_type ~= hld.e_item_type_goddess_equip or t_item_template.sub_type ~= 0 then
	 	return ConstDefine.e_item_string_unkown;
	 end

	 if item_ptr:get_data_info(hld.e_item_info_is_first) == 1 then
	 	return ConstDefine.e_item_string_unkown;
	 end

	 pawn_att_mgr:apply_att_change_by_array(player_ref, t_deity_template.AttArray, true, 1)

	 item_ptr:set_data_info(hld.e_item_info_is_first, 1);
	 player_ref:get_goddess_mgr():set_star_is_open(item_ptr, true);
	 player_ref:get_item_set():send_item_one(item_ptr, true, 0);
	 player_ref:get_item_set():calcu_addition(hld.e_addition_buff_goddess, true);
	 delog("----goddess_unlock-----sucess-")
	 return 0
end

function goddess:get_item_template(item_ptr)
	if item_ptr == nil then
		return nil;
    end
    return template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id())
end

function goddess:get_deity_template(item_ptr)
	local t_item_template = self:get_item_template(item_ptr)
	if t_item_template == nil then
		return nil,0
	end
	local  deity_id = t_item_template.logic_id  + item_ptr:get_data_info(hld.e_item_info_upgrade_count)
	return template_manager:get_line("DeityTemplate", deity_id) , deity_id;
end


function goddess:goddess_fight_cpp(player_index, item_guid_a, item_guid_b)
	local player_ref = hld.get_player_by_index(player_index)
	if nil == player_ref then
		return ConstDefine.e_item_string_unkown;

	end
	return goddess:goddess_fight(player_ref, { item_guid_a, item_guid_b}, 0)
end


function goddess:goddess_fight(player_ref, t_item_guid, slot)
	delog("----goddess_fight------", t_item_guid, slot)
	local goddess_mgr =  player_ref:get_goddess_mgr()
	if goddess_mgr == nil then
	 	return ConstDefine.e_item_string_unkown
	end
	local item_ptr = player_ref:get_item_set():get_item(t_item_guid[1], t_item_guid[2])
	if item_ptr == nil then
	 	return ConstDefine.e_item_string_unkown
	end

	if item_ptr:get_data_info(hld.e_item_info_is_first) <= 0 then
	 	return ConstDefine.e_item_string_unkown;
	end

	local t_deity_template = self:get_deity_template(item_ptr);
	if t_deity_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	end
	local old_item_ptr = self:get_fighting_goddess(player_ref)
	if old_item_ptr ~= nil then
	 	local t_old_item_guid = { old_item_ptr:get_item_guid_A(), old_item_ptr:get_item_guid_B() };
	 	if t_old_item_guid[1] == t_item_guid[1] and t_old_item_guid[2] == t_item_guid[2] then

	 	 	item_ptr:set_data_info(hld.e_item_info_starskill_flag, 0)
	 	 	item_ptr:set_data_info(hld.e_item_info_activate, 0)
	 	 	player_ref:kill_pet_by_type(hld.e_summon_pet_type_goddess);
	 	 	player_ref:get_item_set():send_item_one(item_ptr,true, 0)
	 	 	goddess_mgr:calcu_cooperate_buff(item_ptr, false);
	 	 	return 0
	 	end

 	    old_item_ptr:set_data_info(hld.e_item_info_starskill_flag, 0);
		old_item_ptr:set_data_info(hld.e_item_info_activate, 0);
		player_ref:get_item_set():send_item_one(old_item_ptr,true, 0);
		goddess_mgr:calcu_cooperate_buff(old_item_ptr, false);
	end

    item_ptr:set_data_info(hld.e_item_info_starskill_flag, 1);
	item_ptr:set_data_info(hld.e_item_info_activate, 1);
	item_ptr:set_data_info(hld.e_item_info_illusion_had_byte, 0);
	player_ref:get_item_set():send_item_one(item_ptr,true, 0);

	player_ref:summon_pet(t_deity_template.NpcId);
	goddess_mgr:calcu_cooperate_buff(item_ptr, true);
	goddess_mgr:calcu_goddess_buff(item_ptr, true);
	delog("----goddess_fight--sucess-----", t_item_guid, slot)
	return 0;
end


function goddess:get_fighting_goddess(player_ref)
	for i = 0, max_goddess_equip_num - 1 do
		repeat 
	        local item_ptr = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_goddess_equip, i);
	        if (item_ptr == nil) then
	           break;
	        end
	        local t_item_template = self:get_item_template(item_ptr)
	        if (t_item_template == nil) then
	           break;
	        end
	        if (item_ptr:get_data_info(hld.e_item_info_starskill_flag) == 1 and 
	           t_item_template.item_type == hld.e_item_type_goddess_equip and 
	           t_item_template.sub_type == 0) then
	           return item_ptr;
	        end
        until true
	end	
	return nil;
end

function goddess:goddess_equip_upgrade(player_ref, t_item_guid, slot,item_N)
	delog("-----------goddess_equip_upgrade----------")
	local goddess_mgr =  player_ref:get_goddess_mgr()
	if goddess_mgr == nil then
	 	return ConstDefine.e_item_string_unkown
	end
	local item_ptr = player_ref:get_item_set():get_item(t_item_guid[1], t_item_guid[2])
	if item_ptr == nil then
	 	return ConstDefine.e_item_string_unkown
	end
	if item_ptr:get_data_info(hld.e_item_info_activate) ~= 1 then
		return ConstDefine.e_item_string_unkown;
    end
    local t_item_template = self:get_item_template(item_ptr);
	if t_item_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	end



	local t_deity_template, deity_id = self:get_deity_template(item_ptr);
	if t_deity_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	end
	if t_item_template.sub_type <= 0 then
		return ConstDefine.e_item_string_unkown;
	end


	local cur_level = t_deity_template.CurLevel

	if t_deity_template.CurLevel >= t_deity_template.MaxLevel then
		return ConstDefine.e_item_string_unkown;
	end

	local t_next_deity_template =  template_manager:get_line("DeityTemplate", deity_id + 1)
	if t_next_deity_template == nil then
		return ConstDefine.e_item_string_unkown
	end

	local is_upgrade = false

	local t_AddExpCost = Common:StrToTable(t_deity_template.AddExpCost)
	local t_UpgradeCost = Common:StrToTable(t_deity_template.UpgradeCost)
	if #t_AddExpCost > 0 and #t_UpgradeCost > 0 and
	   #t_AddExpCost % 2 == 0 and #t_UpgradeCost % 2 == 0 then
	   local cur_exp  = item_ptr:get_data_info(hld.e_item_info_wing_exp) 

	   local need_material = 0;
	   local need_material_num = 0;
	   if cur_exp < t_deity_template.UpgradeNeedNum then
	   	    need_material = t_AddExpCost[1]
	   	    need_material_num = t_AddExpCost[2]

	   	    ------------------------------------------------------------------------------------
	   	    if (item_N > 1  and item_N <= 100) then

	            local cur_own_num = player_ref:get_item_set():count_item_by_template_id(need_material, hld.e_bag_type_bag, -1)
		   	    if cur_own_num  < need_material_num then
		   	       return ConstDefine.e_item_string_matirial_not_enough
		   	    end

	   	    	local mul_need_num = need_material_num * item_N
	   	    	local consume_num = mul_need_num;
	   	    	if (cur_exp + mul_need_num) > t_deity_template.UpgradeNeedNum then
	                consume_num = t_deity_template.UpgradeNeedNum - cur_exp
	            end
	            if (consume_num < need_material_num) then
                    return ConstDefine.e_item_string_unkown
	            end

		   	    local add_point = consume_num

		   	    if (cur_own_num < consume_num) then
		   	    	add_point = cur_own_num
		   	    end
		   	    item_ptr:set_data_info(hld.e_item_info_wing_exp, cur_exp + add_point)

		   	    need_material_num = add_point

	   	    else
		   	    if cur_exp + need_material_num >  t_deity_template.UpgradeNeedNum then
		   	       return ConstDefine.e_item_string_unkown
		   	    end
		   	    -------------------------------------------------------------------------------------

		   	    local cur_own_num = player_ref:get_item_set():count_item_by_template_id(need_material, hld.e_bag_type_bag, -1)
		   	    if cur_own_num  < need_material_num then
		   	       return ConstDefine.e_item_string_matirial_not_enough
		   	    end
		   	    item_ptr:set_data_info(hld.e_item_info_wing_exp, cur_exp + need_material_num)
	   	    end
	   	else

	   		is_upgrade = true
	   		need_material = t_UpgradeCost[1]
	   		need_material_num = t_UpgradeCost[2]
	   		local cur_own_num  = player_ref:get_item_set():count_item_by_template_id(need_material, hld.e_bag_type_bag, -1)
	   	    if cur_own_num  < need_material_num then
	   	       return ConstDefine.e_item_string_matirial_not_enough
	   	    end

	   	    item_ptr:set_data_info(hld.e_item_info_wing_exp, 0);
	   	end

	   	player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_upgrade_goddess_equip, 0, need_material, need_material_num, hld.e_item_locked);
	end
	if is_upgrade == true then
		player_ref:get_item_set():set_goddess_equip_att(item_ptr, false)
		item_ptr:set_data_info(hld.e_item_info_upgrade_count, cur_level + 1)
		player_ref:get_item_set():set_goddess_equip_att(item_ptr, true)
		player_ref:get_item_set():calcu_addition(hld.e_addition_buff_goddess, true)
    end

    player_ref:get_item_set():send_item_one(item_ptr,true, 0)

    delog("-----------goddess_equip_upgrade-------sucess---")

    return 0;
end

function goddess:cooperate_slot_unlock(player_ref, t_item_guid, slot)
	 delog("----cooperate_slot_unlock------", t_item_guid, slot)
	 local  unlock_flag = player_ref:get_unit_info(hld.e_role_info_goddess_slot_flag);
     local  item_ptr =  self:get_select_type_goddess(player_ref, 1);
     if item_ptr == nil then
     	return ConstDefine.e_item_string_unkown
     end
     local t_deity_template = self:get_deity_template(item_ptr)
     if t_deity_template == nil then
     	return ConstDefine.e_item_string_unkown
     end
     local t_CooperateUnlockNeed = Common:StrToTable(t_deity_template.CooperateUnlockNeed)
     if #t_CooperateUnlockNeed < 6 then
     	return ConstDefine.e_item_string_unkown
     end

     if slot <= 0 then
     	return ConstDefine.e_item_string_unkown
     end
     local index = slot - 1;
     local need_material  =  t_CooperateUnlockNeed[index * 2 + 1]
     local need_material_num = t_CooperateUnlockNeed[index * 2 + 2]
     local cur_own_num =  player_ref:get_item_set():count_item_by_template_id(need_material, hld.e_bag_type_bag, -1);
     if cur_own_num < need_material_num then
     	return ConstDefine.e_item_string_matirial_not_enough;
     end

     player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_goddess_slot_unlock, 0, need_material, need_material_num, hld.e_item_locked)

     local offset_bit = hld.init_unit:get_left_shift_operation_result(1, index)
     local or_result = hld.init_unit:get_bitwise_or_operation_result(unlock_flag, offset_bit)
     player_ref:set_unit_info(hld.e_role_info_goddess_slot_flag, or_result);
     player_ref:send_info_one(hld.e_role_info_goddess_slot_flag, false);
     return 0
end

function goddess:get_star_deity_template(item_ptr)
	local t_item_template = self:get_item_template(item_ptr)
	if t_item_template == nil then
		return nil,0
	end
	local  deity_id = t_item_template.logic_id  + item_ptr:get_data_info(hld.e_item_info_upgrade_count)
	local t_deity_template =  template_manager:get_line("DeityTemplate", deity_id)
	if t_deity_template == nil then
		return nil,0
	end
	local star_id =  t_deity_template.StarID + item_ptr:get_data_info(hld.e_item_info_stargrade_count);

	local t_deity_star_template = template_manager:get_line("DeityTemplate", star_id)

	return t_deity_star_template, star_id
end

function goddess:get_level_num( player_ref, sub_type, level)
	local  num = 0
	for i = 0, max_goddess_equip_num - 1 do
		repeat 
	        local item_ptr = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_goddess_equip, i);
	        if (item_ptr == nil) then
	           break;
	        end
	        local t_item_template = self:get_item_template(item_ptr)
	        if (t_item_template == nil) then
	           break;
	        end
	        local t_deity_template = self:get_deity_template(item_ptr);
	        if t_deity_template == nil then
	 	       break;
	        end
	        if (t_item_template.item_type == hld.e_item_type_goddess_equip and 
	            t_item_template.sub_type ~= 0  and
	            t_deity_template.Type  == sub_type) then

	           local cur_level = item_ptr:get_data_info(hld.e_item_info_upgrade_count) 
	           if cur_level >= level then   	
	              num = num + 1
	           end
	        end
        until true
	end	
	if num == 6 then
	   return true
	end
	return false
end

function goddess:goddess_star_upgrade(player_ref, t_item_guid, slot)

	 delog("----goddess_star_upgrade------", t_item_guid, slot)
	local goddess_mgr =  player_ref:get_goddess_mgr()
	if goddess_mgr == nil then
	 	return ConstDefine.e_item_string_unkown
	end
	local item_ptr = player_ref:get_item_set():get_item(t_item_guid[1], t_item_guid[2])
	if item_ptr == nil then
	 	return ConstDefine.e_item_string_unkown
	 end
	 local t_item_template = self:get_item_template(item_ptr);
	 if t_item_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	 end

	local t_deity_template = self:get_deity_template(item_ptr);
    if t_deity_template == nil then
	    return ConstDefine.e_item_string_unkown;
    end

	 local t_star_deity_template = self:get_star_deity_template(item_ptr);
	 if t_star_deity_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	 end
	 if item_ptr:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_goddess_equip then
	 	return ConstDefine.e_item_string_unkown;
	 end
	 if t_item_template.item_type ~= hld.e_item_type_goddess_equip or t_item_template.sub_type ~= 0 then
	 	return ConstDefine.e_item_string_unkown;
	 end

	 if item_ptr:get_data_info(hld.e_item_info_is_first) ~= 1  then
	 	return ConstDefine.e_item_string_unkown;
	 end

	 if item_ptr:get_data_info(hld.e_item_info_succinct_level)  ~= 1 then
	 	return ConstDefine.e_item_string_unkown;
	 end


	 local t_star_deity_template, star_id = self:get_star_deity_template(item_ptr);
	 if t_star_deity_template == nil then
	 	return ConstDefine.e_item_string_unkown;
	 end

	 local t_next_star_deity_template = template_manager:get_line("DeityTemplate", star_id + 1)

	 if t_next_star_deity_template  == nil then
	 	return ConstDefine.e_item_string_unkown;
	 end

	 local t_UpgradeCost = t_star_deity_template.UpgradeCost;

	 if #t_UpgradeCost == 0 or #t_UpgradeCost % 2 ~= 0 then
	    return  ConstDefine.e_item_string_unkown;
	 end

	local cur_own_num  = player_ref:get_item_set():count_item_by_template_id(t_UpgradeCost[1], hld.e_bag_type_bag, -1)
	if cur_own_num < t_UpgradeCost[2] then
	 	return ConstDefine.e_item_string_unkown;
	end

	
	local cur_level = item_ptr:get_data_info(hld.e_item_info_stargrade_count)

	if not self:get_level_num(player_ref, t_deity_template.Type,t_star_deity_template.EquNeedLv) then
	 	return ConstDefine.e_item_string_unkown
    end

    player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_star_upgrade_goddess, 0, t_UpgradeCost[1], t_UpgradeCost[2], hld.e_item_locked);

	player_ref:get_item_set():set_goddess_equip_att(item_ptr, false)


	item_ptr:set_data_info(hld.e_item_info_stargrade_count, cur_level + 1)

	player_ref:get_item_set():set_goddess_equip_att(item_ptr, true)



	player_ref:get_item_set():send_item_one(item_ptr, true, 0);

	hld.server_log:serverGodnessStar(player_ref:get_share_log_head(),  cur_level + 1);
	delog("----goddess_unlock-----sucess-")
	-- body
end



function goddess:get_select_type_goddess(player_ref, i_type)
	for i = 0, max_goddess_equip_num - 1 do
		repeat 
	        local item_ptr = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_goddess_equip, i);
	        if (item_ptr == nil) then
	           break;
	        end
	        local t_item_template = self:get_item_template(item_ptr)
	        if (t_item_template == nil) then
	           break;
	        end
	        local t_deity_template = self:get_deity_template(item_ptr);
	        if t_deity_template == nil then
	 	       break;
	        end
	        if (t_item_template.item_type == hld.e_item_type_goddess_equip and 
	            t_item_template.sub_type == 0  and
	            t_deity_template.Type  == i_type) then
	           return item_ptr;
	        end
        until true
	end	
	return nil
end

goddess_mgr = goddess

return goddess