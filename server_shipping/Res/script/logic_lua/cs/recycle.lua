local recycle = {}

local recycle_type_enum =
{
	recuit = 1,
	login  = 2,
	task   = 3,
	buy    = 4,
	reward = 5,
};

local enum_recycle_error =
{  
    sucess = 0,
	days = -1,
	con = -2,
	have_get = -3,
	consume = -4,
	task_id = -5,
	task_state = -6,
	invaild_key = -7,
	level = 8
}

local enum_recycle_task_data = 
{
	config_id  = 0,
	finish_num = 1,
	state      = 2,
	max        = 3,
};


function recycle:init()
   self.m_type_oper = {}
   self.m_type_oper[recycle_type_enum.login] = self.process_login
   self.m_type_oper[recycle_type_enum.task]  = self.process_task;
   self.m_type_oper[recycle_type_enum.buy]   = self.process_buy;
   self.m_type_oper[recycle_type_enum.recuit] = self.process_invited;
end



define_C[11652] = function (message_data, player_ref)
	local msg_data = decode("hld.recycle_proto.recycle_operate", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	recycle:oper(player_ref, msg_data.recycle_id, msg_data.oper_type)
end


function recycle:get_recycle_template_by_id(recycle_id)
	delog("----get_recycle_template_by_id------", recycle_id)
    local  t_line = template_manager:get_line("RecycleTemplate",recycle_id)
	return t_line
end


function recycle:oper(player_ref,i_recycle_id, i_oper_type)
	delog("-----oper------", i_recycle_id, i_oper_type)
 	local t_line = self:get_recycle_template_by_id(i_recycle_id)
 	if t_line == nil then
 		return
 	end
 	self.m_type_oper[t_line.RecycleType](self,player_ref,i_recycle_id)
end 

function recycle:oper_end_msg(player_ref,i_recycle_id, i_type, i_result)
    delog("------oper_end_msg----", i_recycle_id, i_type, i_result)
	local msg_data = {
	    recycle_id = i_recycle_id,
	    oper_type = i_type,
	    result = i_result
	}
	MessageMgr:send_message_to_self(player_ref, "hld.recycle_proto.recycle_operate_end",msg_data, 11653)
end


function recycle:process_login(player_ref, i_recycle_id)
	delog("-----process_login---", i_recycle_id)
	local t_line = self:get_recycle_template_by_id(i_recycle_id)
 	if t_line == nil then
 		return
 	end 
 	local recycle_mgr = player_ref:get_recycle_mgr()
 	if recycle_mgr == nil then
 	   return
 	end
 	local recycle_obj = recycle_mgr:get_recycle_obj()
 	if recycle_obj == nil then
 	  return
 	end
 	local i_delta = recycle_mgr:get_delta_time();
 	if i_delta < 0 then
 	   self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.days);
 	   return;
 	end
 	local t_con_value = Common:StrToTable(t_line.ConditionValue)
 	if #t_con_value ~= 1 or t_con_value[1] > i_delta then
 	    self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.con);
 		return;
 	end
 	if recycle_obj:get_login_flag_bit(t_line.Pos) then
 	    self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.have_get);
 	    return;
 	end
 	recycle_obj:set_login_flag_bit(t_line.Pos);
 	self:get_reward(player_ref, t_line)

 	local flag = recycle_obj:get_login_flag_bit(t_line.Pos);
 	local state = 0
 	if flag then
 	   state  = 1
 	end
 	delog("------state----", state)

 	self:sync_one_message_to_client(player_ref, i_recycle_id, state, 11651)

 	self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.sucess)
end

function recycle:sync_one_message_to_client(player_ref,i_recycle_id, i_state, i_head)
	 local  msg_data = {
	      recycle_id = i_recycle_id,
	      state = i_state
	 }
	 MessageMgr:send_message_to_self(player_ref, "hld.recycle_proto.recycle_reward_one",msg_data, i_head)

end


function recycle:get_reward(player_ref, t_line)
     local t_reward_item_array = Common:StrToTable(t_line.RewardItemArray)
	 local cur_level = player_ref:get_unit_info(hld.e_role_info_exp_level);
     if #t_reward_item_array == 0 or #t_reward_item_array % 3 ~= 0 then
     	return;
     end
	 local reward_id = 0;
     for i=1, #t_reward_item_array, 3 do
		if cur_level > t_reward_item_array[i] and cur_level <= t_reward_item_array[i + 1] then
			reward_id = t_reward_item_array[i + 2];
			break;
		end
     end
	 local reward_array = self:reward_regional_diff(reward_id);
	 if #reward_array == 0 or #reward_array % 2 ~= 0 then
     	return;
     end
	 local t_promp_item_data= {}
	 for j=1, #reward_array, 2 do
		local item_id = reward_array[j]
		local item_num = reward_array[j + 1]
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr ~= nil then
			local item_ptr = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_recycle, 0, item_id, item_num, 1,0,0,0);
			if item_ptr ~= nil then
				player_ref:get_item_set():put_item_into_bag(item_ptr)
				table.insert(t_promp_item_data, item_id)
				table.insert(t_promp_item_data, item_num)
				table.insert(t_promp_item_data, item_ptr:get_data_info(hld.e_item_info_locked))
			end
		end
	 end
     welfare:get_item_send_promp_msg_to_client(player_ref, t_promp_item_data,false)
end

function recycle:reward_regional_diff(reward_id)
	local reward_array = {}
	local region_code = template_manager:get_field("RegionTemplate", 150000000, "RegionCode");
	local RewardTemplate = template_manager:get_line("RewardByRegionTemplate", reward_id);
	if region_code == hld.e_version_region_type_china_main then
		return RewardTemplate["ChinaMainRegionReward"]
	end
	if region_code == hld.e_version_region_type_kr then
		return RewardTemplate["KRRegionReward"]
	end
	if region_code == hld.e_version_region_type_china_tw then
		return RewardTemplate["ChinaTWRegionReward"]
	end
	if region_code == hld.e_version_region_type_en then
		return RewardTemplate["ENRegionReward"]
	end
	if region_code == hld.e_version_region_type_tha then
		return RewardTemplate["THARegionReward"]
	end
	if region_code == hld.e_version_region_type_vn then
		return RewardTemplate["VNRegionReward"]
	end
	if region_code == hld.e_version_region_type_jp then
		return RewardTemplate["JPRegionReward"]
	end
	if region_code == hld.e_version_region_type_jp then
		return RewardTemplate["RUSRegionReward"]
	end
	if region_code == hld.e_version_region_type_jp then
		return RewardTemplate["SGRegionReward"]
	end
	return reward_array
end

function recycle:process_task(player_ref, i_recycle_id)
	local t_line = self:get_recycle_template_by_id(i_recycle_id)
 	if t_line == nil then
 		return
 	end 
 	local recycle_mgr = player_ref:get_recycle_mgr()
 	if recycle_mgr == nil then
 	   return
 	end
 	local i_delta = recycle_mgr:get_delta_time();
 	if i_delta < 0 then
 	   self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.days);
 	   return;
 	end
 	local recycle_task = recycle_mgr:get_recycle_task_by_id(i_recycle_id)
 	if recycle_task == nil then
 	    self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.task_id);
 		return;
 	end
 	if recycle_task:get_inst_data(enum_recycle_task_data.config_id) ~= i_recycle_id then
 		self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.task_id);
 		return;
    end
    if recycle_task:get_inst_data(enum_recycle_task_data.state) ~= 1 then
       self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.task_state);
       return;
    end

    recycle_task:set_inst_data(enum_recycle_task_data.state, 2);
    self:get_reward(player_ref, t_line)
    self:sync_one_task_message_to_client(player_ref, recycle_task)


    self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.sucess)
end

function recycle:sync_one_task_message_to_client(player_ref, recycle_task)
	local msg_data = {
	     recycle_id = recycle_task:get_inst_data(enum_recycle_task_data.config_id),
	     finish_num = recycle_task:get_inst_data(enum_recycle_task_data.finish_num),
	     state =  recycle_task:get_inst_data(enum_recycle_task_data.state)
    }
    MessageMgr:send_message_to_self(player_ref, "hld.recycle_proto.recycle_item_one",msg_data, 11654)
end


function recycle:process_buy(player_ref, i_recycle_id)
	local t_line = self:get_recycle_template_by_id(i_recycle_id)
 	if t_line == nil then
 		return
 	end 
 	local recycle_mgr = player_ref:get_recycle_mgr()
 	if recycle_mgr == nil then
 	   return
 	end
 	local recycle_obj = recycle_mgr:get_recycle_obj()
 	if recycle_obj == nil then
 	  return
 	end
 	local i_delta = recycle_mgr:get_delta_time();
 	if i_delta < 0 then
 	   self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.days);
 	   return;
 	end
 	if recycle_obj:get_prop_flag_bit(t_line.Pos) then
 	   self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.have_get);
 	   return
 	end
 	if not self:is_buy(player_ref, t_line) then
 	   self:oper_end_msg(player_ref,i_recycle_id, 0, enum_recycle_error.consume);
 	   return;
 	end
 	self:buy_consume(player_ref, t_line)
 	recycle_obj:set_prop_flag_bit(t_line.Pos);
 	self:get_reward(player_ref, t_line)

 	local flag = recycle_obj:get_prop_flag_bit(t_line.Pos);
 	local state = 0
 	if flag then
 	   state  = 1
 	end
 	delog("------state----", state)

 	self:sync_one_message_to_client(player_ref, i_recycle_id, state, 11655)

 	self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.sucess)



end


function recycle:is_buy(player_ref, t_line)
    local t_con_value = Common:StrToTable(t_line.ConditionValue)
    if #t_con_value == 0 or #t_con_value % 2 ~= 0 then
       return false
    end
    for i=1, #t_con_value, 2 do
    	local monye_id = t_con_value[i];
    	local money_num = t_con_value[i + 1];
    	if not player_ref:can_cut_money(monye_id, money_num) then
    	   return false
    	end
    end
    return true
end

function recycle:buy_consume(player_ref, t_line)
	local t_con_value = Common:StrToTable(t_line.ConditionValue)
    if #t_con_value == 0 or #t_con_value % 2 ~= 0 then
       return
    end
    for i=1, #t_con_value, 2 do
    	local monye_id = t_con_value[i];
    	local money_num = t_con_value[i + 1];
    	player_ref:cut_money(monye_id, money_num, hld.e_sereve_log_cut_money_recycle_buy, 0,0)
    end
end


function recycle:process_invited(player_ref, i_recycle_id)
	delog("-------process_invited---", i_recycle_id)
	local  my_level = player_ref:get_unit_info(hld.e_role_info_exp_level);
	local t_line = self:get_recycle_template_by_id(i_recycle_id)
 	if t_line == nil then
 		return
 	end 

 	local recycle_mgr = player_ref:get_recycle_mgr()
 	if recycle_mgr == nil then
 	   return
 	end
 	local recycle_obj = recycle_mgr:get_recycle_obj()
 	if recycle_obj == nil then
 	  return
 	end

 	local  recycle_invited_obj = recycle_mgr:get_recycle_invited_obj()
 	if recycle_invited_obj == nil then
 		return
 	end

 	local t_con_value = Common:StrToTable(t_line.ConditionValue)
 	local t_master_level = Common:StrToTable(t_line.MasterLevel)
 	if #t_con_value ~= 2 or #t_master_level ~= 2 then
 		return;
 	end
 	if my_level < t_master_level[2] then
 		self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.level);
 		return;
 	end

 	if not recycle_invited_obj:is_get_reward(t_con_value[2], t_con_value[1]) then
 		self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.con);
 	   return;
 	end

 	if recycle_obj:get_invited_level_flag_bit(t_line.Pos) then
 	   self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.have_get);
 	   return;
 	end

 	recycle_obj:set_invited_level_flag_bit(t_line.Pos);

 	self:get_reward(player_ref, t_line)

 	local flag = recycle_obj:get_invited_level_flag_bit(t_line.Pos);
 	local state = 0
 	if flag then
 	   state  = 1
 	end
 	delog("------state----", state)

 	self:sync_one_message_to_client(player_ref, i_recycle_id, state, 11649)

 	self:oper_end_msg(player_ref, i_recycle_id, 0, enum_recycle_error.sucess)
end


return recycle







