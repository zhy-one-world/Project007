local dragontrip = {}

----------------------------------------------------------
 --是否启用 萌龙奇遇 启用必须 添加 11666 5 ，1404 6，1405 5协议协议控制表中

--热更此处不能修改
----------------------------------------------------------
function dragontrip:is_use_lua()  
	return false
end

------------------------------------------------------------

local e_dragontrip_oper =
{

	money = 1,
	item = 2,
	roll = 3,
	reward_round = 4,
	reward_all = 5,
	refresh_recycle = 6,
	refresh_day_times = 7,
};

local e_dragontrip_param = 
{
	money_type = 1,
	money_num = 2,
    cd_second = 3,
	day_limit = 4,

	item_type = 5,
	item_num = 6,

	point_max = 7,
	reward_max = 8,
	param_max = 8,
};

local e_dragontrip_inst_data =
{
	free_time = 0,
	round = 1,
	pos = 2,
	rewardflag = 3,
	oper_time = 4,
    active_state = 5,
	day_times = 6,
	max = 7,
};


local e_dragontrip_error = {
	sucess = 0,
	times = -1,
	state = -2,
	not_money = -3,
	not_item = -4,
	reward_round = -5,
	have_get = -6,
}

local e_dragontrip_point = 
{
	start = 0,
	transfer1 = 4,
	transfer2 = 17,

}

local e_dragontrip_type = 
{
	point  = 1,
	reward_round = 2,
	reward_all = 3,
};

--------------------------------------------------------
local player_mgr =  RequireSingleton("player_mgr")

local dragontrip_lua = RequireClass("dragontrip")  --数据类


local time_mgr = RequireSingleton("time_mgr")


function dragontrip_lua:_constructor(i_oPlayer)

	self.m_oPlayer = i_oPlayer
	self.m_tData = {}
	self.m_last_time = time_mgr:get_cur_time()

	self:init_func()
end


function dragontrip_lua:init_func()
   self.m_type_oper = {}
   self.m_type_oper[e_dragontrip_oper.money] = self.on_money;
   self.m_type_oper[e_dragontrip_oper.item]  = self.on_item;
   self.m_type_oper[e_dragontrip_oper.roll]   = self.on_roll;
   self.m_type_oper[e_dragontrip_oper.reward_round] = self.on_reward_round;
   self.m_type_oper[e_dragontrip_oper.reward_all] = self.on_reward_all;

end


function dragontrip_lua:Update()
	 local cur_time = time_mgr:get_cur_time()
	 local delta_time = cur_time - self.m_last_time
	 if delta_time < 5000 then
	 	return
	 end
	 self.m_last_time = cur_time
	 --delog("----cur_time---", cur_time)

	 --delog(self)
	 --delog(self.m_tData)
end


function dragontrip_lua:get_player()
	return self.m_oPlayer
end



function dragontrip_lua:load_db_data(s_buffer)
	local msg_data = decode("hld.cs2dp_proto.role_dragontrip_db", s_buffer)
	delog("-------load--dragontrip_lua-----")
	--delog(msg_data)
	if #msg_data.data_ary ~= e_dragontrip_inst_data.max then
		return false
	end

	for i=1,#msg_data.data_ary do
		self.m_tData[i] = msg_data.data_ary[i]
	end

	self:send_to_client()

	return true
end


function dragontrip_lua:save_to_db(i_type)
	 delog("------save--dragontrip_lua-----")
	 if not dragontrip:is_use_lua()  then
	 	return
	 end

     local player_ref = self:get_player():get_cpp_obj();

	 local t_db_msg =
	 {
	   role_guid = self.m_oPlayer.m_role_id,
	   unit_array_index = self.m_oPlayer.m_unit_index,
	   save_type_ex = i_type,
	   db_data = { data_ary = {} }

	 }

	 for i=1, #self.m_tData do
	 	table.insert(t_db_msg.db_data.data_ary, self.m_tData[i])
	 end

     MessageMgr:send_message_to_dp(player_ref, "hld.cs2dp_proto.save_role_dragontrip",t_db_msg, 1404)

  
end


function dragontrip_lua:send_to_client()
    
    local player_ref = self:get_player():get_cpp_obj();
     
	 local t_client_msg = 
	 {
	     data_ary = {},

	     reward_ary = {},

	     params = {}

	 }

	 for i=1,#self.m_tData do
	 	 table.insert(t_client_msg.data_ary, self.m_tData[i])
	 end



	 for i=1,6 do
	 	 local res = self:get_reward_flag(i)
	 	 table.insert(t_client_msg.reward_ary,  res)
	 end

	 table.insert(t_client_msg.params, os.time())

     --delog(t_client_msg)

	 MessageMgr:send_message_to_self(player_ref, "hld.dragontrip_proto.dragontrip_all",t_client_msg, 11665)
end

function dragontrip_lua:day_refresh()

	   self:set_data(e_dragontrip_inst_data.day_times, 0)
	   self:send_to_client()
	   self:oper_msg(e_dragontrip_oper.refresh_day_times, 0)

end


function dragontrip_lua:refresh_recycle()
	  local value = self:get_data(e_dragontrip_inst_data.active_state)

      for i=1, #self.m_tData do
      	   self.m_tData[i] = 0
      end

      self:set_data(e_dragontrip_inst_data.active_state, value)
      self:send_to_client()

      self:oper_msg(e_dragontrip_oper.refresh_recycle, 0)

end


function dragontrip_lua:oper_msg(in_oper_type,  oper_error, roll_point, in_item_id, in_item_num)
	local player_ref = self:get_player():get_cpp_obj();
	local roll =  roll_point or 0
	local id = in_item_id or 0
	local num = in_item_num or 0
	local msg_data = {
	    oper_type = in_oper_type,
	    result = oper_error,
	    roll_num = roll,
	    item_id = id,
	    item_num = num,
	}

	MessageMgr:send_message_to_self(player_ref, "hld.dragontrip_proto.dragontrip_operate_end",msg_data, 11667)
end


function dragontrip_lua:on_operate(oper_type, oper_param)
	 local player_ref = self:get_player():get_cpp_obj();
	 local is_open = player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_dragontrip);
	 if is_open == false then
	 	return 
	 end

	 delog("----oper_type---", oper_type)

	 --delog(self.m_type_oper)

	 self.m_type_oper[oper_type](self, oper_type, oper_param)
end



function dragontrip_lua:get_data(i_idx)
	 local idx = i_idx + 1
	 return self.m_tData[idx]
end

function dragontrip_lua:set_data(i_idx, value)
	 local idx = i_idx + 1
	 self.m_tData[idx] = value
end



function dragontrip_lua:is_free()
	local free_time = self:get_data(e_dragontrip_inst_data.free_time);
	local cur_time = os.time();
	if (cur_time - free_time > self:get_config_param(e_dragontrip_param.cd_second)) then
		return true;
	end
	return false;
end


function dragontrip_lua:get_config_param(param_type)
	--local t_default =  { 3, 100, 86400, 10,  31140405,1, 28, 5}
	local t_line  = server_tool_mgr:get_game_config_template_ptr()
	if t_line == nil then
		return 999
	end
	return t_line.DragontripArray[param_type]
end

function dragontrip_lua:get_item_by_type_pos(type, pos)
	local t_tab = template_manager:get_template("DragonTripTemplate");
	if t_tab  == nil then
		return 0, nil 
	end
	for k,v in pairs(t_tab) do
		if v.Type == type and v.Pos == pos then
			return k, v
		end
	end
	return 0, nil

end



function dragontrip_lua:on_money(oper_type, oper_param)
	delog("-------enter--dragontrip_lua:on_money-")
    local player_ref = self:get_player():get_cpp_obj();
 	local cur_times = self:get_data(e_dragontrip_inst_data.day_times);
 	local day_limit = self:get_config_param(e_dragontrip_param.day_limit)
 	if cur_times >= day_limit then
 		self:oper_msg(oper_type, e_dragontrip_error.times)
 		return
 	end
 	if  self:get_data(e_dragontrip_inst_data.active_state) ~= 0 then
 		self:oper_msg(oper_type, e_dragontrip_error.state)
 		return 
 	end
 	if  self:is_free()  then
 		self:set_data(e_dragontrip_inst_data.free_time, os.time())
 	else
 		local money_id =  self:get_config_param(e_dragontrip_param.money_type)
 		local money_num = self:get_config_param(e_dragontrip_param.money_num)

 		if (not player_ref:can_cut_money(money_id,  money_num)) then	
		     self:oper_msg(oper_type, e_dragontrip_error.not_money);
			 return;
		end

		player_ref:cut_money(money_id, money_num, hld.e_server_log_cut_money_dragontrip, 0,0);
 	end
 	self:set_data(e_dragontrip_inst_data.active_state, 1)

 	self:set_data(e_dragontrip_inst_data.day_times, cur_times + 1)

 	self:send_to_client()

 	self:oper_msg(oper_type, e_dragontrip_error.sucess);

end


function dragontrip_lua:on_item(oper_type, oper_param)
	local player_ref = self:get_player():get_cpp_obj();

	if self:get_data(e_dragontrip_inst_data.active_state) ~= 0 then
		self:oper_msg(oper_type, e_dragontrip_error.state);
		return;
    end

    local item_id = self:get_config_param(e_dragontrip_param.item_type)
    local item_num  = self:get_config_param(e_dragontrip_param.item_num)

    delog("------item--id----num-", item_id, item_num)

	if (player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag, -1) < item_num) then		
		self:oper_msg(oper_type, e_dragontrip_error.not_item);
		return;
	end

	player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_dragontrip, 0, item_id, item_num, hld.e_item_locked);

	self:set_data(e_dragontrip_inst_data.active_state, 2);


	self:send_to_client();


	self:oper_msg(oper_type, e_dragontrip_error.sucess);
end




function dragontrip_lua:on_roll(oper_type, oper_param)
    delog("---------dragontrip-----roll-----enter---")


	if self:get_data(e_dragontrip_inst_data.active_state) == 0 then
	    delog("---------dragontrip-----roll-----enter-=-1111---")
		return;
	end
	local  max_point = self:get_config_param(e_dragontrip_param.point_max);

	local  step = hld.random_gen:get_random(1, 6);

	local  cur_pos = self:get_data(e_dragontrip_inst_data.pos);

	local  end_pos = (cur_pos + step) % max_point;

	local  is_case = 0;

	local   item_id = 0;
	local   item_num = 0;

	if end_pos == e_dragontrip_point.transfer1  then
		
		end_pos = e_dragontrip_point.transfer2;

		is_case = 1;

		
	elseif  end_pos == e_dragontrip_point.transfer2 then
		end_pos = e_dragontrip_point.transfer1;
		
	else
		if end_pos ~= e_dragontrip_point.start then
			local id, config_ptr = self:get_item_by_type_pos(e_dragontrip_type.point, end_pos);
			if config_ptr == nil then
				 delog("---------dragontrip-----roll-----enter-=-333333---")
				return;
			end
			local ItemTemplatePtr = template_manager:get_line("ItemTemplate", config_ptr.ItemId);
			if ItemTemplatePtr == nil then
				 delog("---------dragontrip-----roll-----enter-=-444444444---")
				return;
			end

			local player_ref = self:get_player():get_cpp_obj()

			local dragontrip_mgr = player_ref:get_dragontrip_mgr()


			local  level = ItemTemplatePtr.item_color;


			item_id, item_num = dragontrip_mgr:get_activity_item(level, item_id, item_num);

			if item_id == 0 then
				return;
			end
			local  rwdItemTemplatePtr = template_manager:get_line("ItemTemplate", item_id);
			if  rwdItemTemplatePtr == nil then
				return;
			end

			item_num = self:get_data(e_dragontrip_inst_data.active_state) * item_num;



			delog("------item---",hld.e_server_log_add_item_dragontrip)
			local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_dragontrip, 0, item_id, item_num, 1,0,0,0);
			if temp_item ~= nil then
				 player_ref:get_item_set():put_item_into_bag(temp_item);
			end
		  end
		  is_case = 3
	end
	if ((is_case == 3 or is_case == 1) and (end_pos < cur_pos)) then
		
		local round = self:get_data(e_dragontrip_inst_data.round);
		self:set_data(e_dragontrip_inst_data.round, round + 1);

    end
	self:set_data(e_dragontrip_inst_data.active_state, 0);
	delog("-------------end-------pos--------", end_pos)
	self:set_data(e_dragontrip_inst_data.pos, end_pos);


	self:send_to_client();
	self:oper_msg(oper_type, e_dragontrip_error.sucess, step, item_id, item_num);
	delog("---------dragontrip----------roll---sucess---")
end


function dragontrip_lua:get_reward_flag(idx)
	  if idx < 1 or idx > 31 then
	  	 return 1
	  end
	  local reward_flag = self:get_data(e_dragontrip_inst_data.rewardflag)

	  local flag =  reward_flag & 1 << idx 

	  local result = (flag ~= 0 and 1) or 0

	  return result
end


function dragontrip_lua:set_reward_flag(idx)
	 if idx < 1 or idx > 31 then
	  	 return 
	  end
	  local flag = self:get_data(e_dragontrip_inst_data.rewardflag)

	  flag = flag | 1 << idx

	  self:set_data(e_dragontrip_inst_data.rewardflag, flag)

end


function dragontrip_lua:on_reward_round(oper_type, req_round)


	if req_round < 1 or req_round > 5 then
		return;
	end
	local round_num = self:get_data(e_dragontrip_inst_data.round);
	if round_num < req_round then
		self:oper_msg(oper_type, e_dragontrip_error.reward_round);
		return;
	end
	if self:get_reward_flag(req_round) == 1 then
	
		self:oper_msg(oper_type, e_dragontrip_error.have_get);
		return;
	end

	self:get_reward(e_dragontrip_type.reward_round, req_round);

	self:set_reward_flag(req_round);
	self:send_to_client();

	self:oper_msg(oper_type, e_dragontrip_error.sucess);
end

function dragontrip_lua:on_reward_all(oper_type, oper_param)
 

	local round_num = self:get_data(e_dragontrip_inst_data.round);
	if (round_num < self:get_config_param(e_dragontrip_param.reward_max)) then
	
		self:oper_msg(oper_type, e_dragontrip_error.reward_round);
		return;
	end
	if  self:get_reward_flag(6) == 1 then
	
		self:oper_msg( oper_type, e_dragontrip_error.have_get);
		return;
	end

	self:get_reward(e_dragontrip_type.reward_all, 0);

	self:set_reward_flag(6);


	self:sync_all_message_to_client();
	self:oper_msg(oper_type, e_dragontrip_error.sucess);

end


function dragontrip_lua:get_reward( type,  pos)
    local player_ref = self:get_player():get_cpp_obj()
	local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 	   return
 	end



	local item_id = 0;
	local item_num = 0;
	item_id, item_num = dragontrip_mgr:get_round_reward(pos, item_id, item_num);
	local  ItemTemplatePtr = template_manager:get_line("ItemTemplate", item_id);
	if ItemTemplatePtr == nil then
		return;
	end
	local t_promp_item_data= {}

	local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_dragontrip, 0, item_id, item_num, 1,0,0,0);
	if (temp_item ~= nil) then
		player_ref:get_item_set():put_item_into_bag(temp_item);


		table.insert(t_promp_item_data, item_id)
 		table.insert(t_promp_item_data, item_num)
 		table.insert(t_promp_item_data, temp_item:get_data_info(hld.e_item_info_locked))

 		welfare:get_item_send_promp_msg_to_client(player_ref, t_promp_item_data,false)
	end
end












function dragontrip:day_refresh(unit_index)
	local oPlayer = player_mgr:find_player(unit_index)
    oPlayer:get_system("dragontrip"):day_refresh()
end



function dragontrip:refresh_recycle(unit_index)
	local oPlayer = player_mgr:find_player(unit_index)
    oPlayer:get_system("dragontrip"):refresh_recycle()
end










---------------------------------------------------------



function dragontrip:init()

   self.m_type_oper = {}
   self.m_type_oper[e_dragontrip_oper.money] = self.on_money;
   self.m_type_oper[e_dragontrip_oper.item]  = self.on_item;
   self.m_type_oper[e_dragontrip_oper.roll]   = self.on_roll;
   self.m_type_oper[e_dragontrip_oper.reward_round] = self.on_reward_round;
   self.m_type_oper[e_dragontrip_oper.reward_all] = self.on_reward_all;
end

define_C[11666] = function(message_data, player_ref)
    local msg_data = decode("hld.dragontrip_proto.dragontrip_operate", message_data)
    if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

    if (not dragontrip:is_use_lua()) then
      dragontrip:on_operate(player_ref, msg_data.oper_type, msg_data.oper_param)
      return
    end
    ----------------------------------------------
      -----使用LUA 数据

    ----------------------------------------------
     delog("--use--lua---data")

    local oPlayer = player_mgr:find_player(player_ref:get_array_index())

    oPlayer:get_system("dragontrip"):on_operate(msg_data.oper_type, msg_data.oper_param)

end


function dragontrip:oper_msg(player_ref, in_oper_type,  oper_error, roll_point, in_item_id, in_item_num)
	local roll =  roll_point or 0
	local id = in_item_id or 0
	local num = in_item_num or 0
	local msg_data = {
	    oper_type = in_oper_type,
	    result = oper_error,
	    roll_num = roll,
	    item_id = id,
	    item_num = num,
	}
	MessageMgr:send_message_to_self(player_ref, "hld.dragontrip_proto.dragontrip_operate_end",msg_data, 11667)
end


function dragontrip:on_operate(player_ref, oper_type, oper_param)
	 local is_open = player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_dragontrip);
	 if is_open == false then
	 	return 
	 end

	 self.m_type_oper[oper_type](self, player_ref, oper_type, oper_param)
end




function dragontrip:on_money(player_ref, oper_type, oper_param)
    local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 	   return
 	end
 	local cur_times = dragontrip_mgr:get_data(e_dragontrip_inst_data.day_times);
 	local day_limit = self:get_config_param(e_dragontrip_param.day_limit)
 	if cur_times >= day_limit then
 		self:oper_msg(player_ref, oper_type, e_dragontrip_error.times)
 		return
 	end
 	if dragontrip_mgr:get_data(e_dragontrip_inst_data.active_state) ~= 0 then
 		self:oper_msg(player_ref, oper_type, e_dragontrip_error.state)
 		return 
 	end
 	if  self:is_free(dragontrip_mgr)  then
 		dragontrip_mgr:set_data(e_dragontrip_inst_data.free_time, os.time())
 	else
 		local money_id = self:get_config_param(e_dragontrip_param.money_type)
 		local money_num = self:get_config_param(e_dragontrip_param.money_num)

 		if (not player_ref:can_cut_money(money_id,  money_num)) then	
		     self:oper_msg(player_ref, oper_type, e_dragontrip_error.not_money);
			 return;
		end

		player_ref:cut_money(money_id, money_num, hld.e_server_log_cut_money_dragontrip, 0,0);
 	end
 	dragontrip_mgr:set_data(e_dragontrip_inst_data.active_state, 1)

 	dragontrip_mgr:set_data(e_dragontrip_inst_data.day_times, cur_times + 1)

 	dragontrip_mgr:sync_all_message_to_client()

 	self:oper_msg(player_ref, oper_type, e_dragontrip_error.sucess);

end

function dragontrip:on_item(player_ref, oper_type, oper_param)

	local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 	   return
 	end
	
	if dragontrip_mgr:get_data(e_dragontrip_inst_data.active_state) ~= 0 then
		self:oper_msg(player_ref, oper_type, e_dragontrip_error.state);
		return;
    end

    local item_id = self:get_config_param(e_dragontrip_param.item_type)
    local item_num  = self:get_config_param(e_dragontrip_param.item_num)

    delog("------item--id----num-", item_id, item_num)

	if (player_ref:get_item_set():count_item_by_template_id(item_id, hld.e_bag_type_bag, -1) < item_num) then		
		self:oper_msg(player_ref,oper_type, e_dragontrip_error.not_item);
		return;
	end

	player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_dragontrip, 0, item_id, item_num, hld.e_item_locked);

	dragontrip_mgr:set_data(e_dragontrip_inst_data.active_state, 2);


	dragontrip_mgr:sync_all_message_to_client();


	self:oper_msg(player_ref, oper_type, e_dragontrip_error.sucess);
end

function dragontrip:on_roll(player_ref, oper_type, oper_param)
    delog("---------dragontrip-----roll-----enter---")
	local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 		delog("---------dragontrip-----roll-----enter-12321--")
 	   return
 	end

	if dragontrip_mgr:get_data(e_dragontrip_inst_data.active_state) == 0 then
	    delog("---------dragontrip-----roll-----enter-=-1111---")
		return;
	end
	local  max_point = self:get_config_param(e_dragontrip_param.point_max);

	local  step = hld.random_gen:get_random(1, 6);

	local  cur_pos = dragontrip_mgr:get_data(e_dragontrip_inst_data.pos);

	local  end_pos = (cur_pos + step) % max_point;

	local  is_case = 0;

	local   item_id = 0;
	local   item_num = 0;

	if end_pos == e_dragontrip_point.transfer1  then
		
		end_pos = e_dragontrip_point.transfer2;

		is_case = 1;

		
	elseif  end_pos == e_dragontrip_point.transfer2 then
		end_pos = e_dragontrip_point.transfer1;
		
	else
		if end_pos ~= e_dragontrip_point.start then
			local id, config_ptr = self:get_item_by_type_pos(e_dragontrip_type.point, end_pos);
			if config_ptr == nil then
				 delog("---------dragontrip-----roll-----enter-=-333333---")
				return;
			end
			local ItemTemplatePtr = template_manager:get_line("ItemTemplate", config_ptr.ItemId);
			if ItemTemplatePtr == nil then
				 delog("---------dragontrip-----roll-----enter-=-444444444---")
				return;
			end


			local  level = ItemTemplatePtr.item_color;


			item_id, item_num = dragontrip_mgr:get_activity_item(level, item_id, item_num);

			if item_id == 0 then
				return;
			end
			local  rwdItemTemplatePtr = template_manager:get_line("ItemTemplate", item_id);
			if  rwdItemTemplatePtr == nil then
				return;
			end

			item_num = dragontrip_mgr:get_data(e_dragontrip_inst_data.active_state) * item_num;

			--local item_num = dragontrip_mgr:get_data(e_dragontrip_inst_data.active_state);

			delog("------item---",hld.e_server_log_add_item_dragontrip)
			local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_dragontrip, 0, item_id, item_num, 1,0,0,0);
			if temp_item ~= nil then
				 player_ref:get_item_set():put_item_into_bag(temp_item);
			end
		  end
		  is_case = 3
	end
	if ((is_case == 3 or is_case == 1) and (end_pos < cur_pos)) then
		
		local round = dragontrip_mgr:get_data(e_dragontrip_inst_data.round);
		dragontrip_mgr:set_data(e_dragontrip_inst_data.round, round + 1);

    end
	dragontrip_mgr:set_data(e_dragontrip_inst_data.active_state, 0);
	delog("-------------end-------pos--------", end_pos)
	dragontrip_mgr:set_data(e_dragontrip_inst_data.pos, end_pos);


	dragontrip_mgr:sync_all_message_to_client();
	self:oper_msg(player_ref, oper_type, e_dragontrip_error.sucess, step, item_id, item_num);
	delog("---------dragontrip----------roll---sucess---")
end

function dragontrip:on_reward_round(player_ref, oper_type, req_round)
    local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 	   return
 	end
	if req_round < 1 or req_round > 5 then
		return;
	end
	local round_num = dragontrip_mgr:get_data(e_dragontrip_inst_data.round);
	if round_num < req_round then
		self:oper_msg(player_ref,oper_type, e_dragontrip_error.reward_round);
		return;
	end
	if dragontrip_mgr:get_reward_flag(req_round) == 1 then
	
		self:oper_msg(player_ref,oper_type, e_dragontrip_error.have_get);
		return;
	end

	self:get_reward(player_ref,e_dragontrip_type.reward_round, req_round);

	dragontrip_mgr:set_reward_flag(req_round);
	dragontrip_mgr:sync_all_message_to_client();

	self:oper_msg(player_ref,oper_type, e_dragontrip_error.sucess);
end

function dragontrip:on_reward_all(player_ref, oper_type, oper_param)
    local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 	   return
 	end

	local round_num = dragontrip_mgr:get_data(e_dragontrip_inst_data.round);
	if (round_num < self:get_config_param(e_dragontrip_param.reward_max)) then
	
		self:oper_msg(player_ref, oper_type, e_dragontrip_error.reward_round);
		return;
	end
	if dragontrip_mgr:get_reward_flag(6) == 1 then
	
		self:oper_msg(player_ref, oper_type, e_dragontrip_error.have_get);
		return;
	end

	self:get_reward(player_ref, e_dragontrip_type.reward_all, 0);

	dragontrip_mgr:set_reward_flag(6);


	dragontrip_mgr:sync_all_message_to_client();
	self:oper_msg(player_ref, oper_type, e_dragontrip_error.sucess);

end


function dragontrip:get_reward(player_ref,  type,  pos)

	local dragontrip_mgr = player_ref:get_dragontrip_mgr()
 	if dragontrip_mgr == nil then
 	   return
 	end

	--[[local id, config_ptr = self:get_item_by_type_pos(type, pos);
	if  config_ptr == nil  then
		return;
	end]]

	local item_id = 0;
	local item_num = 0;
	item_id, item_num = dragontrip_mgr:get_round_reward(pos, item_id, item_num);
	local  ItemTemplatePtr = template_manager:get_line("ItemTemplate", item_id);
	if ItemTemplatePtr == nil then
		return;
	end
	local t_promp_item_data= {}

	local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_dragontrip, 0, item_id, item_num, 1,0,0,0);
	if (temp_item ~= nil) then
		player_ref:get_item_set():put_item_into_bag(temp_item);


		table.insert(t_promp_item_data, item_id)
 		table.insert(t_promp_item_data, item_num)
 		table.insert(t_promp_item_data, temp_item:get_data_info(hld.e_item_info_locked))

 		welfare:get_item_send_promp_msg_to_client(player_ref, t_promp_item_data,false)
	end
end

function dragontrip:is_free(dragontrip_mgr)
	local free_time = dragontrip_mgr:get_data(e_dragontrip_inst_data.free_time);
	local cur_time = os.time();
	if (cur_time - free_time > self:get_config_param(e_dragontrip_param.cd_second)) then
		return true;
	end
	return false;
end


function dragontrip:get_config_param(param_type)
	--local t_default =  { 3, 100, 86400, 10,  31140405,1, 28, 5}
	local t_line  = server_tool_mgr:get_game_config_template_ptr()
	if t_line == nil then
		return 999
	end
	return t_line.DragontripArray[param_type]
end

function dragontrip:get_item_by_type_pos(type, pos)
	local t_tab = template_manager:get_template("DragonTripTemplate");
	if t_tab  == nil then
		return 0, nil 
	end
	for k,v in pairs(t_tab) do
		if v.Type == type and v.Pos == pos then
			return k, v
		end
	end
	return 0, nil

end


g_dragontrip = dragontrip
return dragontrip