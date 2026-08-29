local awaken_lua_mgr = {}
function awaken_lua_mgr:init()
end

local  awaken_error_success = 0;
local  awaken_error_need_guid = -1;
local  awaken_error_main_guid = -2;
local  awaken_error_item_color = -3;
local  awaken_error_item_info = -4;
local  awaken_error_item_type = -5;
local  awaken_error_awaken_config = -6;
local  awaken_error_max_level = -7;
local  awaken_error_tran_level = -8;
local  awaken_error_need_power = -9;
local  awaken_error_need_consume = -10;


function awaken_mgr_is_use_lua()
     return true
end

function awaken_mgr_is_type(item_ptr)
    local template_id = item_ptr:get_data_info(hld.e_item_info_info_id)
    local level = item_ptr:get_data_info(hld.e_item_info_wing_exp)

    local tLine = template_manager:get_line("ItemTemplate", template_id)
    if next(tLine) == nil then
        return false
    end
    if  tLine.AwakenID == 0 then
        return false
    end
    return true
end

function awaken_mgr_is_eat(item_obj, my_class)
    local ItemTemplateID = item_obj:get_data_info(0)
    local tLine = template_manager:get_line("ItemTemplate", ItemTemplateID);
    if next(tLine) == nil then
        return 0
    end
    if  tLine.AwakenPower == 0 then
        return 0
    end
    if  tLine.class_type ~= '' and tLine.class_type ~= tostring(my_class) then
        return 0
    end

    local upgrade = item_obj:get_data_info(6)
    local addon = item_obj:get_data_info(9)
    local fumo = item_obj:get_data_info(28)
    local awakenexp = item_obj:get_data_info(11)
    if upgrade ~= 0 or addon ~=0  or fumo ~= 0 or awakenexp ~= 0 then
        return 0
    end
    for i=0,3 do
        local value  = item_obj:get_data_info(20 + i)
        if value ~= 0 then
           return 0 
        end
    end
    return tLine.AwakenPower
end

function awaken_mgr_is_need(awaken_mgr, need_guid_array_str, my_class)
    local  t_guid_str = string.split(need_guid_array_str,'|')
    for k, v  in pairs(t_guid_str) do
         local item_ptr =  awaken_mgr:get_item_id_str(v)
         if item_ptr == nil then
            return false
         end
         if awaken_mgr_is_eat(item_ptr, my_class) == 0 then
            return false
         end
    end
    return true
    
end

function awaken_mgr_get_all_power(awaken_mgr,need_guid_array_str)
    local  t_guid_str = string.split(need_guid_array_str,'|')
    local total_power = 0

    for k, v  in pairs(t_guid_str) do
        local item_ptr = awaken_mgr:get_item_id_str(v) 

        local template_id = item_ptr:get_data_info(hld.e_item_info_info_id)

        local tLine = template_manager:get_line("ItemTemplate", template_id)

        total_power = tLine.AwakenPower + total_power
    end
    return total_power
end

function awaken_mgr_get_con_param(TemplateID, level)
 
    local tLine = template_manager:get_line("ItemTemplate", TemplateID);
    if next(tLine) == nil then
        return nil
    end

    local awaken_id = tLine.AwakenID + level

    local tawkLine = template_manager:get_line("AwakenTemplate", awaken_id);

    if next(tawkLine) == nil then
        return nil
    end
    return tawkLine

end

function awaken_mgr_get_player_class(player_ref)
    if player_ref == nil then
        return -1
    end
    local my_class = player_ref:get_unit_info(hld.e_role_info_class_type)
    --print("-----my_class------", my_class)
    return my_class
end

function awaken_mgr_is_eat(item_obj, my_class)
    local ItemTemplateID = item_obj:get_data_info(0)
    local tLine = template_manager:get_line("ItemTemplate", ItemTemplateID);
    if next(tLine) == nil then
        return 0
    end
    if  tLine.AwakenPower == 0 then
        return 0
    end
    if  tLine.class_type ~= '' and tLine.class_type ~= tostring(my_class) then
        return 0
    end

    local upgrade = item_obj:get_data_info(6)
    local addon = item_obj:get_data_info(9)
    local fumo = item_obj:get_data_info(28)
    local awakenexp = item_obj:get_data_info(11)
    if upgrade ~= 0 or addon ~=0  or fumo ~= 0 or awakenexp ~= 0 then
        return 0
    end
    for i=0,3 do
        local value  = item_obj:get_data_info(20 + i)
        if value ~= 0 then
           return 0 
        end
    end
    return tLine.AwakenPower
end

function awaken_mgr_item_operate(player_index,main_guid_str, need_guid_array_str)
    --print("---awaken_mgr_item_operate--", player_index, main_guid_str, need_guid_array_str)
    local player_ref = hld.get_player_by_index(tonumber(player_index));
    local awaken_mgr = player_ref:get_awaken_mgr();

    local my_class = awaken_mgr_get_player_class(player_ref)

    local item_ptr = awaken_mgr:get_item_id_str(main_guid_str)
    if item_ptr == nil then
       awaken_mgr:operate_msg_str(main_guid_str, awaken_error_main_guid)
       return
    end

    if not awaken_mgr_is_type(item_ptr) then
       awaken_mgr:operate_msg_str(main_guid_str, awaken_error_item_type)
       return
    end
    if not awaken_mgr_is_need(awaken_mgr, need_guid_array_str, my_class) then
 
       awaken_mgr:operate_msg_str(main_guid_str, awaken_error_need_guid)
       return
    end


    local template_id = item_ptr:get_data_info(hld.e_item_info_info_id)
    local level = item_ptr:get_data_info(hld.e_item_info_wing_exp)

    local tconfig = awaken_mgr_get_con_param(template_id, level)

    if tconfig == nil then

         awaken_mgr:operate_msg_str(main_guid_str, awaken_error_awaken_config)
         return 
    end
  
    if level >= tconfig.LevelMax then
         awaken_mgr:operate_msg_str(main_guid_str, awaken_error_max_level)
         return 
    end
 
    if tconfig.GradeLimit > awaken_mgr:get_grade_num() then
        awaken_mgr:operate_msg_str(main_guid_str, awaken_error_tran_level)
        return 
    end

    local owner_power  = awaken_mgr_get_all_power(awaken_mgr,need_guid_array_str)

    if owner_power  < tconfig.NeedPower then
        awaken_mgr:operate_msg_str(main_guid_str, awaken_error_need_power)
        return 
    end

    if not awaken_mgr:is_material(tconfig.MatirialID, tconfig.MatirialNum) then
       awaken_mgr:operate_msg_str(main_guid_str, awaken_error_need_consume)
       return 
    end

    awaken_mgr:set_awaken_attr(item_ptr, false)
    player_ref:get_item_set():equip_off_awaken_show_buff()
    item_ptr:set_data_info(hld.e_item_info_wing_exp, level + 1)
    awaken_mgr:set_awaken_attr(item_ptr, true)
    player_ref:get_item_set():equip_on_awaken_show_buff()
    awaken_mgr:syc_client_update_item(item_ptr)
    awaken_mgr:del_material(tconfig.MatirialID, tconfig.MatirialNum)

    awaken_mgr:item_str_to_item_guid_array_del(need_guid_array_str)

    awaken_mgr:operate_msg_str(main_guid_str, awaken_error_success);

	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_max_awaken_level, 0, 0)

    awaken_mgr:sync_fight_power(item_ptr)

end


--------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------

local  awaken = {}

local  enum_awaken_error = 
{
    success = 0,
	eat_success = 1,
    need_guid = -1,
    main_guid = -2,
    item_color = -3,
    item_info = -4,
    item_type = -5,
    awaken_config = -6,
    max_level = -7,
    tran_level = -8,
    need_power = -9,
    need_consume = -10,
}


define_C[10753] = function(message_data, player_ref)
    local msg_data = decode("hld.item_proto.item_awaken", message_data)
    if msg_data == nil then
        return
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
        return;
    end
	awaken:item_awaken_operate(player_ref, msg_data.main_item_guid, msg_data.need_item_guid);
end


function awaken:init()
end


function awaken:item_awaken_operate(player_ref, t_main_item_guid, t_need_item_guid)
    local awaken_mgr = player_ref:get_awaken_mgr();
    if  awaken_mgr == nil then
        return;
    end

    local item_ptr = player_ref:get_item_set():get_item(t_main_item_guid[1], t_main_item_guid[2])
    if item_ptr == nil then
       self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.main_guid)
       return
    end

    if not self:is_type(item_ptr) then
       self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.item_type)
       return
    end
 
    if not self:is_need(player_ref, t_need_item_guid) then
       self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.need_guid)
       return
    end

    local template_id = item_ptr:get_data_info(hld.e_item_info_info_id)
    local level = item_ptr:get_data_info(hld.e_item_info_wing_exp)
    
    local tconfig = self:get_con_param(template_id, level)
    if tconfig == nil then
         self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.awaken_config)
         return 
    end

    if level >= tconfig.LevelMax then
         self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.max_level)
         return 
    end
	
	local cur_grade_num = player_ref:get_grade_num()
    if tconfig.GradeLimit > cur_grade_num then
        self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.tran_level)
        return 
    end

	local have_power = item_ptr:get_data_info(hld.e_item_info_wing_bless)
    local owner_power = self:get_all_power(player_ref,t_need_item_guid) + have_power

	local s_item_guid = self:table_to_guid_str(t_need_item_guid)
	
	-- 删除道具
	if #s_item_guid > 0 then
		awaken_mgr:item_str_to_item_guid_array_del(s_item_guid)
	end
	-- 减少属性
	awaken_mgr:set_awaken_attr(item_ptr, false)
	
    if  owner_power < tconfig.NeedPower then
		item_ptr:set_data_info(hld.e_item_info_wing_bless, owner_power)
		awaken_mgr:set_awaken_attr(item_ptr, true)
		awaken_mgr:syc_client_update_item(item_ptr)
		self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.eat_success);
		return 
    end
	
 
	item_ptr:set_data_info(hld.e_item_info_wing_bless, owner_power)
	
	local add_level = 0;
	for i=0,40 do
        local tem_config = self:get_con_param(template_id, level + i)
		if tem_config == nil then
			-- 如果为没有数据 默认为
			item_ptr:set_data_info(hld.e_item_info_wing_bless, 0)
			break
		end
		
		--检查转职
		if tem_config.GradeLimit > cur_grade_num then
			-- 满阶设置为0
			item_ptr:set_data_info(hld.e_item_info_wing_bless, 0)
			break
		end
		
		--检查上限
		if tem_config.LevelMax <= level + i then
			-- 满阶设置为0
			item_ptr:set_data_info(hld.e_item_info_wing_bless, 0)
			break
		end
		
		if owner_power < tem_config.NeedPower then
			-- 如果不够下一级设置成百分比
			item_ptr:set_data_info(hld.e_item_info_wing_bless, owner_power)
			break
		end
		
		owner_power = owner_power - tem_config.NeedPower
		
		-- 检查觉醒道具
		local owner_matirial_num = player_ref:get_item_set():count_item_by_template_id(tem_config.MatirialID, hld.e_bag_type_bag, -1);
		if  owner_matirial_num < tem_config.MatirialNum then
			break
		end
		-- 扣除觉醒道具
		player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_awaken, 0,tem_config.MatirialID, tem_config.MatirialNum,hld.e_item_locked);

		add_level = add_level + 1
		
    end
	
	-- 设置觉醒等级
	item_ptr:set_data_info(hld.e_item_info_wing_exp, level + add_level)
	
	-- 刷新觉醒Buff
    player_ref:get_item_set():equip_on_awaken_show_buff()
    
	-- 增加属性
	awaken_mgr:set_awaken_attr(item_ptr, true)
	
    awaken_mgr:syc_client_update_item(item_ptr)

    self:operate_msg(player_ref, t_main_item_guid, enum_awaken_error.success);

    awaken_mgr:sync_fight_power(item_ptr)
	
	player_ref:refresh_service_goal(hld.e_service_goal_type_awaken_level);
    player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_awaken_level_num,0,0);

end

function awaken:operate_msg(player_ref, t_main_item_guid, i_result)
     delog("-------i_result---", i_result)
     local msg_data = 
     {
        item_guid = { t_main_item_guid[1], t_main_item_guid[2]},
        result = i_result
     }
     MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_awaken_end",msg_data, 10754)
end

function awaken:get_player_class(player_ref)
    if player_ref == nil then
        return -1
    end
    local my_class = player_ref:get_unit_info(hld.e_role_info_class_type)
    return my_class
end

function awaken:is_type(item_ptr)
    local template_id = item_ptr:get_data_info(hld.e_item_info_info_id)
    local level = item_ptr:get_data_info(hld.e_item_info_wing_exp)

    local t_line = template_manager:get_line("ItemTemplate", template_id)
    if t_line == nil then
        return false
    end
    if  t_line.AwakenID == 0 then
        return false
    end
    return true
end


function awaken:is_need(player_ref, t_need_item_guid)
    local num = #t_need_item_guid
    if num == 0 or  num % 2 ~= 0 then
        return true
    end
    local my_class = self:get_player_class(player_ref)
    for i=1, num, 2 do
        local item_ptr = player_ref:get_item_set():get_item(t_need_item_guid[i], t_need_item_guid[i + 1])
        if item_ptr == nil then
            return false
        end
        if self:is_eat(item_ptr, my_class) == 0 then
            return false
        end
    end
    return true
end

function awaken:is_same_class_type(s_class_type, my_class)
     local t_class_type = Common:StrToTable(s_class_type)
     if #t_class_type == 0 then
        return true
     end
     for i=1,#t_class_type do
        if my_class == t_class_type[i] then
            return true
        end
     end
     return false
end

function  awaken:is_eat(item_ptr, my_class)
    local item_template_id = item_ptr:get_data_info(0)
    local t_line = template_manager:get_line("ItemTemplate", item_template_id);
    if  t_line == nil then
        return 0
    end
    if  t_line.AwakenPower == 0 then
        return 0
    end

    if not self:is_same_class_type(t_line.class_type, my_class) then
        return 0
    end

	if t_line.item_type == 4 and t_line.sub_type == 54 then
		return t_line.AwakenPower
    end
    --[[if  t_line.class_type ~= '' and t_line.class_type ~= tostring(my_class) then
        return 0
    end]]
    local upgrade = item_ptr:get_data_info(6)
    local addon = item_ptr:get_data_info(9)
    local fumo = item_ptr:get_data_info(28)
    local awakenexp = item_ptr:get_data_info(11)
    if upgrade ~= 0 or addon ~=0  or fumo ~= 0 or awakenexp ~= 0 then
        return 0
    end
    for i=0,1 do
        local value = item_ptr:get_data_info(46 + i)
        if value ~= 0 then
           return 0 
        end
    end
    for i=0,3 do
        local value  = item_ptr:get_data_info(20 + i)
        if value ~= 0 then
           return 0 
        end
    end
    return t_line.AwakenPower
end

function awaken:get_con_param(template_id, level)
 
    local t_line = template_manager:get_line("ItemTemplate", template_id);
    if  t_line == nil then
        return nil
    end

    local awaken_id = t_line.AwakenID + level

    local t_awk_line = template_manager:get_line("AwakenTemplate", awaken_id);

    if  t_awk_line == nil then
        return nil
    end
    return t_awk_line
end

function awaken:get_all_power(player_ref,t_need_item_guid)
   
    local total_power = 0

    local num = #t_need_item_guid
	if num == 0 then
		return 0
	end
	
    for i = 1, num, 2 do
        local item_ptr = player_ref:get_item_set():get_item(t_need_item_guid[i], t_need_item_guid[i + 1])

        local template_id = item_ptr:get_data_info(hld.e_item_info_info_id)

        local t_line = template_manager:get_line("ItemTemplate", template_id)

        total_power = total_power + t_line.AwakenPower
  
    end
    return total_power
end

function awaken:table_to_guid_str(t_need_item_guid)
     local num = #t_need_item_guid
	 if num == 0 then
		return "";
	 end
     local t_guid_str = {}
     for i = 1, num, 2 do
         local s_guid_str = hld.guid_transfer_to_string(t_need_item_guid[i], t_need_item_guid[i + 1])
         table.insert(t_guid_str, s_guid_str)
     end
     local s_ret =  table.concat( t_guid_str, "|");

     return s_ret
end








-----------------------------------------------------------------------------------------

return awaken_lua_mgr






