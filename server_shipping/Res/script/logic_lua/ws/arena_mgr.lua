local arena_mgr = RequireSingleton("arena_mgr")


-----------------------------------------------------------
--[[开启LUA 功能
消息DP-WS 
32901	2
32919	2
32920	2
CS-WS
1634	2
1636	2
1638	2
1640	2
1642	2
1644	2
WS-CS
1637	5
1635	5
1647	5
1639	5








]]



local k_rand_name_dic_chinese_male_first_begin = 90086001;
local k_rand_name_dic_chinese_male_first_end = 90086142;
local k_rand_name_dic_chinese_male_second_begin = 90085001;
local k_rand_name_dic_chinese_male_second_end = 90085084;

local k_rand_name_dic_chinese_female_first_begin = 90088001;
local k_rand_name_dic_chinese_female_first_end = 90088328;
local k_rand_name_dic_chinese_female_second_begin = 90087001;
local k_rand_name_dic_chinese_female_second_end = 90087090;


local k_rand_name_dic_no_chinese_male_first_begin = 90082001;
local k_rand_name_dic_no_chinese_male_first_end = 90082982;
local k_rand_name_dic_no_chinese_male_second_begin = 90083001;
local k_rand_name_dic_no_chinese_male_second_end = 90083105;

local k_rand_name_dic_no_chinese_female_first_begin = 90082001;
local k_rand_name_dic_no_chinese_female_first_end = 90082982;
local k_rand_name_dic_no_chinese_female_second_begin = 90084001;
local k_rand_name_dic_no_chinese_female_second_end = 90084312;


-----------------------------------------------------------









local rank_max = 5000;
local rank_save_per_time_max = 500;
local challenge_log_max = 100; --最大保存100条记录
local arena_choose_list_base_range = 300;
local arena_map_template_id = 71100004;
local arena_reward_template_start_id = 109000001;
local arena_not_in_list_rank = -1;
local arena_robot_template_start_id = 110000001;
local areaa_cloose_second_list_base_range = 600;



----------------------------------------------------------
local max_name_size = 100
----------------------------------------------------------




function arena_mgr:init()
	 if is_init() then
	 	return
	 end
	 -------------------------------------------------
	 self.m_arena_rank = {}

	 self.m_sec_tick = 0;


	 -----------show-------------------------------

     self.m_count_time = 0 
	 self.m_t_count = {}
	 self.m_t_count.list = 0
	 self.m_t_count.result = 0


	 -------------------------------------------------
end

function arena_mgr:clear_data()
     self.m_sec_tick = 0
     self.m_arena_rank = {}
end


local const_save_interval = 600000  --10min
local show_inteval_time = 10000  --10se

function arena_mgr:Update(i_nTime)
	if not is_arena_use_lua() then
		return
	end 
	---------------------------------
	 self:show_info(i_nTime)

	 -------------------------
	 self.m_sec_tick  = self.m_sec_tick + i_nTime
	 if self.m_sec_tick < const_save_interval then
	 	return
	 end

	 self:save_ranks_to_db()

	 self.m_sec_tick = 0


end

function arena_mgr:show_info(i_nTime)
      self.m_count_time = self.m_count_time + i_nTime
      if self.m_count_time < show_inteval_time then
      	return
      end

      self.m_count_time = 0

      --show_table(self.m_t_count)
 

end



function arena_mgr:create_robot()
	local  robot_name_tmpl_table = template_manager:get_template("ArenaRobotTemplate");
	if robot_name_tmpl_table == nil then
		return;
	end

	for k, v in pairs(robot_name_tmpl_table) do
		if v ~= nil then
		  self:create_robot_one(k, v)
		end
	end

	self:save_ranks_to_db();
end


function arena_mgr:create_robot_one(k, robot_template_ptr)
	if  robot_template_ptr  == nil or next(robot_template_ptr) == nil then
		return;
	end

	local data_index = robot_template_ptr.Rank;
	if data_index < 0  or  data_index >= rank_max then
		return;
	end

	local robot_name = hld.get_str_by_string_template_id(robot_template_ptr.RobotNameId,0);


	if self.m_arena_rank[data_index] == nil then
       self.m_arena_rank[data_index] = {}
	end

	self.m_arena_rank[data_index].role_guid = hld.make_guid_lua(0);

	self.m_arena_rank[data_index].role_name = robot_name
	self.m_arena_rank[data_index].rank_pos = robot_template_ptr.Rank;
	self.m_arena_rank[data_index].gs_value = robot_template_ptr.GsValue;
	self.m_arena_rank[data_index].robot_template_id = k;
	self.m_arena_rank[data_index].military_level = robot_template_ptr.MilitaryLevel;

	self.m_arena_rank[data_index].role_template_id = 0
	self.m_arena_rank[data_index].vip_level = 0
	self.m_arena_rank[data_index].game_channel = ""

	self.m_arena_rank[data_index].challenge_end_time = 0;


end


function arena_mgr:load_ranks_from_db(arena_rank_info, start_index,  data_num)
	--delog("----start_index---data_num", start_index, data_num)
 	if arena_rank_info  == nil or start_index < 0 or  (start_index + data_num) > rank_max then
		return;
	end


	if start_index == 0 then
		
		self:clear_data();
		for i = 0, rank_max - 1 do
			self.m_arena_rank[i] = {}
			self.m_arena_rank[i].rank_pos = i
		end
	end

	for i=1, #arena_rank_info do
		repeat
			local temp_index = arena_rank_info[i].rank_pos;
			if temp_index < 0 or temp_index >= rank_max then
				break;
			end
			--print("--arena_rank_info[i].role_guid----", arena_rank_info[i].role_guid)
		    self.m_arena_rank[temp_index].rank_pos = arena_rank_info[i].rank_pos;
			self.m_arena_rank[temp_index].role_guid = arena_rank_info[i].role_guid;
			self.m_arena_rank[temp_index].role_name = arena_rank_info[i].role_name;
			self.m_arena_rank[temp_index].gs_value = arena_rank_info[i].gs_value;
			self.m_arena_rank[temp_index].role_template_id = arena_rank_info[i].role_template_id;
			self.m_arena_rank[temp_index].robot_template_id = arena_rank_info[i].robot_template_id;
			self.m_arena_rank[temp_index].military_level = arena_rank_info[i].military_rank_level;
			self.m_arena_rank[temp_index].vip_level = arena_rank_info[i].vip_level;
			self.m_arena_rank[temp_index].game_channel = arena_rank_info[i].game_channel;

			self.m_arena_rank[temp_index].challenge_end_time = 0;

			if arena_rank_info[i].role_template_id == 0 then
				local robot_template_ptr = template_manager:get_line("ArenaRobotTemplate", arena_rank_info[i].robot_template_id);
				if robot_template_ptr ~= nil then
				
					self.m_arena_rank[temp_index].gs_value = robot_template_ptr.GsValue;
					self.m_arena_rank[temp_index].military_level = robot_template_ptr.MilitaryLevel;
					local temp_string = hld.get_str_by_string_template_id(robot_template_ptr.RobotNameId,0);
					self.m_arena_rank[temp_index].role_name = temp_string;
				end
			end
		until true
	end
end 


function arena_mgr:fill_empty_pos( )
	-- body

    local is_has_empty_pos = false;

	local fill_num_string = "arena fill empty pos start \n";


	for i=0, rank_max - 1 do
		repeat 
			if self.m_arena_rank[i] == nil then 
			   self.m_arena_rank[i] = {}
			end
			--print("---sdfsdfds---",self.m_arena_rank[i].role_guid)
			if self.m_arena_rank[i].role_guid ~= nil and self.m_arena_rank[i].role_guid ~= 0 then
				break
			end 
			local k = arena_robot_template_start_id + i
			local robot_template_ptr = template_manager:get_line("ArenaRobotTemplate", arena_robot_template_start_id + i);
			if robot_template_ptr == nil or robot_template_ptr.Rank ~= self.m_arena_rank[i].rank_pos then
				 break
			end


			print("------empty------i------", i)
			
			self:create_robot_one(k, robot_template_ptr);
			is_has_empty_pos = true;	


			fill_num_string = fill_num_string .. i .. "---";
		until true
	end
	if  is_has_empty_pos then
		
		self:save_ranks_to_db();


			
		fill_num_string = fill_num_string .. "end \n";
	    delog("--------test000000000000000")
		print(fill_num_string)
    end
end

function arena_mgr:save_ranks_to_db()

	if not is_arena_use_lua() then
		return
	end 

	delog("----start--save---")

    local t_dp_msg = {}
    t_dp_msg.is_first = true
    t_dp_msg.data_list = {}
	local temp_count = 0;

	for i= 0, rank_max - 1 do
		repeat 
			if self.m_arena_rank[i] == nil or self.m_arena_rank[i].role_guid == 0 then
				break
			end

			local  temp_rank_info = self:copy_save_data(self.m_arena_rank[i])
			table.insert(t_dp_msg.data_list, temp_rank_info)

			temp_count = temp_count + 1;

			if temp_count >= rank_save_per_time_max then

				MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_arena_rank", t_dp_msg, 32902, 0)
				t_dp_msg.is_first = false;
				t_dp_msg.data_list = {}
				temp_count = 0;
			end
		until  true
	end
	if temp_count > 0 then
	   MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_arena_rank", t_dp_msg, 32902, 0)
	end
end

function arena_mgr:copy_save_data(rank_info)
	 local t_db_data = {
		rank_pos = rank_info.rank_pos ,
		role_guid = rank_info.role_guid,
		role_name = rank_info.role_name or "",
		gs_value = rank_info.gs_value or 0,
		role_template_id = rank_info.role_template_id or 0,
		robot_template_id = rank_info.robot_template_id or 0,
		military_rank_level = rank_info.military_level or 0,
		vip_level = rank_info.vip_level or 0,
		game_channel = rank_info.game_channel or "",
	} 
	 return t_db_data
end


function arena_mgr:save_change_rank_result(my_new_info, target_new_info)
    local t_dp_msg = {}

    t_dp_msg.my_new_data = self:copy_save_data(my_new_info)


    t_dp_msg.target_new_data = self:copy_save_data(target_new_info)
    --delog(t_dp_msg.target_new_data)

    MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_arena_rank_change", t_dp_msg, 32904, 0)
end





function arena_mgr:req_challenge(challenge_role_guid, target_role_guid, cost_money_type, cost_money_num, target_cur_pos, self_old_pos)
	local challenge_guid_ptr = hld.get_guid_64(challenge_role_guid)
	local  client_session_ptr = hld.get_client_session_by_guid(challenge_guid_ptr);
	if client_session_ptr == nil then
		return;
	end
	local self_cur_pos = self_old_pos;
	local temp_target_player = self:get_data_by_index(target_cur_pos);
	local temp_challenge_player = self:get_data_by_index(self_cur_pos);
	local req_result = self:get_req_result(temp_target_player, temp_challenge_player, target_role_guid, challenge_role_guid, self_old_pos)
	if  req_result == ConstDefine.e_arena_self_pos_is_changed then
		self_cur_pos = self:get_player_current_index(challenge_role_guid);
	end


	local t_cs_msg = {}
	t_cs_msg.role_guid = challenge_role_guid;
    t_cs_msg.target_guid = target_role_guid;
    t_cs_msg.req_result = req_result;

	if temp_target_player ~= nil then
		t_cs_msg.robot_id = temp_target_player.robot_template_id;
	end
	t_cs_msg.cost_money_type = cost_money_type;
	t_cs_msg.cost_money_value = cost_money_num;
	t_cs_msg.target_cur_pos = target_cur_pos;
	t_cs_msg.self_cur_pos = self_cur_pos;

	MessageMgr:send_to_cs(client_session_ptr, "hld.ws2cs_proto.req_challenge_msg", t_cs_msg, 1637)
	
	if req_result == e_arena_match_success then
		
		if temp_target_player ~= nullptr then
			
			temp_target_player.challenge_end_time = self:get_end_time(300 + 30); --就是个保险，防止被挑战者锁死，300地图时间加个30的保险
		end
		if temp_challenge_player ~= nullptr then
		
			temp_challenge_player.challenge_end_time = self:get_end_time(300 + 30);
		end
	end
end

function arena_mgr:get_end_time(add_sec)
	 local now_msec = get_g_time_mesc()
	 local end_time = now_msec + add_sec * 1000
	 return end_time
end


function arena_mgr:get_req_result(target_ptr,  challenge_ptr,  target_guid,  challenge_guid, challenge_old_pos)
	if target_ptr ==  nil then
		return ConstDefine.e_arena_target_pos_is_changed;

	elseif target_ptr.role_guid ~= target_guid then
	
		return ConstDefine.e_arena_target_pos_is_changed;
	end

	if challenge_ptr == nil then
		if challenge_old_pos ~= arena_not_in_list_rank then
			return ConstDefine.e_arena_self_pos_is_changed;
		end
	else
		if challenge_ptr.role_guid ~= challenge_guid then
			return ConstDefine.e_arena_self_pos_is_changed;
		end
	end

	if target_ptr.challenge_end_time  > get_g_time_mesc() then
		return ConstDefine.e_arena_target_is_challenging;
	end
	if challenge_ptr ~= nil and challenge_ptr.challenge_end_time > get_g_time_mesc() then
		return ConstDefine.e_arena_self_is_challenging;
	end

	return ConstDefine.e_arena_match_success;
end


function arena_mgr:get_choose_list(role_guid,  role_pos)
	local index = role_pos;
	
	local  tamp_player_info = self:get_data_by_index(role_pos);
	if tamp_player_info == nil then		
		if role_pos ~= arena_not_in_list_rank then
			 index = arena_not_in_list_rank;
		end	
	else
	   if tamp_player_info.role_guid ~= role_guid then
			index = self:get_player_current_index(role_guid);
	   end
	end

	local cur_time = get_g_time_mesc();


	local  match_index1 = 0
	local  match_index2 = 0
	local  match_index3 = 0
	if index < 0 or  index == rank_max - 1 then  --4999--->
		
		local random_range = arena_choose_list_base_range // 3;
		local is_cd1 = 0
		local is_cd2 = 0
		local is_cd3 = 0
			
		match_index1, is_cd1 = self:get_area_index((rank_max - random_range), rank_max - 1, cur_time);
		match_index2, is_cd2 = self:get_area_index((rank_max - random_range * 2), rank_max - 1 - random_range, cur_time);
		match_index3, is_cd3 = self:get_area_index((rank_max - random_range * 3), rank_max - 1 - random_range * 2, cur_time);

		if is_cd1 == 1 or is_cd2 == 1 or is_cd3 == 1 then  -- 扩大范围
			
			random_range = areaa_cloose_second_list_base_range // 3;
			match_index1 = self:get_area_index((rank_max - random_range), rank_max - 1, cur_time);
			match_index2 = self:get_area_index((rank_max - random_range * 2), rank_max - 1 - random_range, cur_time);
			match_index3 = self:get_area_index((rank_max - random_range * 3), rank_max - 1 - random_range * 2, cur_time);
	    end
		
	elseif  index == 0 then   -- 第一名
		
		match_index1 = 2;
		match_index2 = 1;
		match_index3 = 0;
	elseif index < 10 then -- 2-9 前10名
		match_index1 = index+1;
		match_index2 = index;
		match_index3 = index-1;
	else
		
		local random_range = 0;
		if index > arena_choose_list_base_range - 1 then  -- 300 ------5000
			
			random_range = arena_choose_list_base_range // 3;     --step  =100
		else                                    -- 10-299
			random_range = (index // 3) - 1;       --        
		end
		local behind_max_index = index + random_range;   
		if behind_max_index >= rank_max then
			behind_max_index = rank_max - 1;
		end

	
		match_index1 = self:get_area_index(index + 1, behind_max_index,cur_time);
		match_index2 = self:get_area_index((index - random_range), index - 1, cur_time);
		match_index3 = self:get_area_index((index - random_range * 2), index - 1 - random_range, cur_time);
	end

		
	local guid_ptr = hld.get_guid_64(role_guid)

	local  client_session_ptr = hld.get_client_session_by_guid(guid_ptr);
	if  client_session_ptr == nil then
		return;
	end

	local t_cs_msg = {}


	t_cs_msg.role_guid = role_guid;

    t_cs_msg.target_player = {}

	if match_index1 >= 0 or match_index1 < rank_max then
		
		t_cs_msg.target_player[1] = deep_copy(self.m_arena_rank[match_index1])
		
	elseif index >= 0 or index < rank_max then
		t_cs_msg.target_player[1] = deep_copy(self.m_arena_rank[index]);
	else
		
		t_cs_msg.target_player[1] = deep_copy(self.m_arena_rank[rank_max - 1])
	end

	if match_index2 >= 0 or match_index2 < rank_max then
		
		t_cs_msg.target_player[2] = deep_copy(self.m_arena_rank[match_index2])

	elseif index >= 0 or index < rank_max then
		t_cs_msg.target_player[2] = deep_copy(self.m_arena_rank[index]);
	else
		
		t_cs_msg.target_player[2] = deep_copy(self.m_arena_rank[rank_max - 2])
	end

	if match_index3 >= 0 or match_index3 < rank_max then
		
		t_cs_msg.target_player[3] = deep_copy(self.m_arena_rank[match_index3])

	elseif index >= 0 or index < rank_max then

		t_cs_msg.target_player[3] = deep_copy(self.m_arena_rank[index]);
	else

		t_cs_msg.target_player[3] = deep_copy(self.m_arena_rank[rank_max - 3])
	end
	MessageMgr:send_to_cs(client_session_ptr, "hld.ws2cs_proto.req_choose_list", t_cs_msg, 1639)

	self.m_t_count.list = self.m_t_count.list + 1
end

function arena_mgr:get_area_index( start,  in_end,  cur_time)
	local is_cd = 0;

	if start < 0 or start >= rank_max or in_end < 0 or in_end >= rank_max then
		return  rank_max - 1 , is_cd
	end
	local index = hld.random_gen:get_random(start, in_end);

	if self.m_arena_rank[index].challenge_end_time < cur_time then
		return index , is_cd
	end

	for i = index - 1, start, -1  do
		if self.m_arena_rank[i].challenge_end_time < cur_time then
		  return i, is_cd
		end
	end

	for i = index + 1, in_end do
	    if self.m_arena_rank[i].challenge_end_time < cur_time then
	      return i, is_cd
	    end
	end
	is_cd = 1
	return index, is_cd
end


function arena_mgr:get_first_three(role_guid)
	local guid_ptr = hld.get_guid_64(role_guid)
    local client_session_ptr = hld.get_client_session_by_guid(guid_ptr);
	if  client_session_ptr == nil then
		return;
	end


	local t_client_msg = {};

	local  first_guid = hld.ranking_mgr_ws:get_instance():get_ranking_player_guid(hld.e_RankingIndex_arena, 0);
	local  second_guid = hld.ranking_mgr_ws:get_instance():get_ranking_player_guid(hld.e_RankingIndex_arena, 1);
	local  third_guid = hld.ranking_mgr_ws:get_instance():get_ranking_player_guid(hld.e_RankingIndex_arena, 2);

	t_client_msg.role_guid = {}

	table.insert(t_client_msg.role_guid, first_guid:get_A())
	table.insert(t_client_msg.role_guid, first_guid:get_B())

	table.insert(t_client_msg.role_guid, second_guid:get_A())
	table.insert(t_client_msg.role_guid, second_guid:get_B())


    table.insert(t_client_msg.role_guid, third_guid:get_A())
	table.insert(t_client_msg.role_guid, third_guid:get_B())


	MessageMgr:send_to_client(client_session_ptr, "hld.arena_proto.get_top_three_player_end", t_client_msg, 11906)
end


function arena_mgr:change_rank(challenge_info,  target_guid, target_index)
	delog("------chanage_info----xxxxxxxxxxxxxxxxxxx-------")
	--delog(challenge_info)

    local  target_info = self:get_data_by_index(target_index);
	if target_info == nil or target_info.role_guid ~= target_guid then
		return -1;
	end

	if challenge_info.rank_pos >= 0 and challenge_info.rank_pos < rank_max then
		local temp_challenge_info = self:get_data_by_index(challenge_info.rank_pos);
		if temp_challenge_info == nil or temp_challenge_info.role_guid ~= challenge_info.role_guid then
			return -1;
		end
	end

	self:reset_challenge_state(challenge_info.role_guid, target_guid, target_index, challenge_info.rank_pos)
    local my_index = challenge_info.rank_pos;
	local first_index = 0;

	if my_index < target_index and my_index >= 0 then
		return my_index;
	end

	local  old_first_player_info = deep_copy(self.m_arena_rank[first_index]);
	local cur_time = get_g_time_mesc()

		--交换 challenge 和target
	local  target_player = deep_copy(self.m_arena_rank[target_index]);
	self.m_arena_rank[target_index].role_guid = challenge_info.role_guid;
	self.m_arena_rank[target_index].role_name = string.sub(challenge_info.role_name, 1, max_name_size)
	self.m_arena_rank[target_index].gs_value = challenge_info.gs_value;
	self.m_arena_rank[target_index].role_template_id = challenge_info.role_template_id;
	self.m_arena_rank[target_index].robot_template_id = 0;
	self.m_arena_rank[target_index].vip_level = challenge_info.vip_level;
	self.m_arena_rank[target_index].military_level = challenge_info.military_level;
	self.m_arena_rank[target_index].game_channel = challenge_info.game_channel;
	if my_index >= 0
	   and my_index < rank_max then
		
		self.m_arena_rank[my_index].role_guid = target_player.role_guid;
		self.m_arena_rank[my_index].role_name = string.sub(target_player.role_name, 1, max_name_size)
		self.m_arena_rank[my_index].gs_value = target_player.gs_value;
		self.m_arena_rank[my_index].role_template_id = target_player.role_template_id;
		self.m_arena_rank[my_index].robot_template_id = target_player.robot_template_id;
		self.m_arena_rank[my_index].vip_level = target_player.vip_level;
		self.m_arena_rank[my_index].military_level = target_player.military_level;
		self.m_arena_rank[my_index].game_channel = target_player.game_channel;
	end
		-------------- 交换竞技场排行榜 ----------------------------------------
	local  my_palyer_info = {}
	my_palyer_info.role_name = challenge_info.role_name;
	my_palyer_info.role_guid = challenge_info.role_guid;
	my_palyer_info.ranking_value = target_index;
	my_palyer_info.ranking_type = hld.e_RankingIndex_arena;
	my_palyer_info.save_time = cur_time;
	my_palyer_info.supporting_guid = challenge_info.gs_value;
	my_palyer_info.vip_level = challenge_info.vip_level;
	my_palyer_info.game_channel = challenge_info.game_channel;
	my_palyer_info.standby_parameter = challenge_info.gs_value;
	my_palyer_info.Peak = 0
	my_palyer_info.server_id = 0
	my_palyer_info.supporting_name = ""

	local challenge_guid_ptr = hld.get_guid_64(challenge_info.role_guid)
	local  my_legion_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(challenge_guid_ptr);
	if  my_legion_ptr ~= nil then
		my_palyer_info.supporting_name = my_legion_ptr:get_legion_name();
	end

	local is_robot = true;
	local target_palyer_info = {} 
	target_palyer_info.role_guid = 0;

	if  target_player.robot_template_id <= 0 then

		is_robot = false;
		target_palyer_info.role_name = target_player.role_name;
		target_palyer_info.role_guid = target_player.role_guid;
		target_palyer_info.ranking_value = my_index;
		target_palyer_info.ranking_type = hld.e_RankingIndex_arena;
		target_palyer_info.save_time = cur_time;
		target_palyer_info.supporting_guid = target_player.gs_value;
		target_palyer_info.vip_level = target_player.vip_level;
		target_palyer_info.game_channel = target_player.game_channel;
		target_palyer_info.standby_parameter = target_player.gs_value;
		local target_guid_ptr = hld.get_guid_64(target_player.role_guid)
		local target_legion_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(target_guid_ptr);
		if target_legion_ptr ~= nil then
			target_palyer_info.supporting_name = target_legion_ptr:get_legion_name();	
		end
	end
	local ss_my_info = encode("hld.st_proto.st_ranking_player_info", my_palyer_info)
	local ss_dst_info = encode("hld.st_proto.st_ranking_player_info",target_palyer_info)
	hld.ranking_mgr_ws:get_instance():sync_player_arena_lua(ss_my_info, string.len(ss_my_info),  ss_dst_info, string.len(ss_dst_info), is_robot);
	-----------------------------------------------------

	if my_index >= 0 and my_index < rank_max then

		delog("--------exist-----")
		
		self:save_change_rank_result(self.m_arena_rank[target_index], self.m_arena_rank[my_index]);
	
	else
		delog("-----not--exist---------")
		
		local temp_target_new_info = {};
		temp_target_new_info.rank_pos = arena_not_in_list_rank;
		temp_target_new_info.role_guid = target_guid;

		self:save_change_rank_result(self.m_arena_rank[target_index], temp_target_new_info);
	end


	--向被挑战人同步数据，将 我现在的 排名同步给他
	local target_guid_ptr = hld.get_guid_64(target_guid)
	local client_session_ptr = hld.get_client_session_by_guid(target_guid_ptr);
	if client_session_ptr ~= nil then
		
		local t_cs_msg = {}

		t_cs_msg.role_guid = target_guid;
			--↓就是“my_index”，没错
		t_cs_msg.arena_rank = my_index;

		t_cs_msg.is_request = 0;

		MessageMgr:send_to_cs(client_session_ptr, "hld.ws2cs_proto.rep_get_arena_rank", t_cs_msg, 1635)

		--被挑战人的排名已经变化，所以此时他可以挑战的名次也要改变

		self:get_choose_list(target_guid, my_index);
	end

	my_index = target_index;

	local new_first_player_info = deep_copy(self.m_arena_rank[first_index]);

	self:sync_first_rank_player_info(old_first_player_info, new_first_player_info)
	return my_index;
end


function arena_mgr:sync_gs_value(role_guid,  temp_pos,  new_gs)
    local  temp_data = self:get_data_by_index(temp_pos);
	if temp_data == nil or temp_data.role_guid ~= role_guid then
		return;
	end
	temp_data.gs_value = new_gs;
end


function arena_mgr:get_player_current_index(role_guid)
	local index = -1
	for i = 0, rank_max - 1 do
		if  role_guid == self.m_arena_rank[i].role_guid then
			index = i;
			break
		end
	end
	return index
end

function arena_mgr:req_get_player_rank(role_guid)
	local t_cs_msg = {}
	t_cs_msg.role_guid = role_guid
	t_cs_msg.is_request = 1;
	local temp_rank = self:get_player_arena(role_guid);
	if temp_rank ~= nil then	
		t_cs_msg.arena_rank = temp_rank.rank_pos;
	else
		t_cs_msg.arena_rank = arena_not_in_list_rank;
	end

	local guid_ptr = hld.get_guid_64(role_guid)

	local client_session_ptr = hld.get_client_session_by_guid(guid_ptr);
	if  client_session_ptr == nil then
		return;
	end
	MessageMgr:send_to_cs(client_session_ptr, "hld.ws2cs_proto.rep_get_arena_rank", t_cs_msg, 1635)
end



function arena_mgr:sync_player_gs(role_guid, gs_value, role_rank)
	if role_guid == 0 or role_rank < 0 or role_rank >= rank_max then
		return;
	end
	local temp_info = self:get_data_by_index(role_rank);
	if temp_info == nil or temp_info.role_guid ~= role_guid then
		return;
	end
	temp_info.gs_value = gs_value;
end

function arena_mgr:sync_player_arena_military_rank(role_guid,  military_level, role_rank)
	if role_guid == 0 or role_rank < 0 or role_rank >= rank_max then
		return;
	end
	local temp_info = self:get_data_by_index(role_rank);
	if temp_info == nil or temp_info.role_guid ~= role_guid then
		return;
	end
	temp_info.military_level = military_level;
end


function arena_mgr:sync_first_rank_player_info(old_first_player_info,  new_first_player_info)
	local  big_player_type = hld.big_player_ws_mgr:get_instance():get_big_player_type_with_ranking_type(hld.e_RankingIndex_arena);
	if big_player_type > 0 and hld.globle_data:get_instance():get_server_on_days(-1) > 1 then

		local new_guid_ptr = hld.get_guid_64(new_first_player_info.role_guid)
		
		hld.big_player_ws_mgr:get_instance():set_big_player_guid(big_player_type, new_guid_ptr);
	end
	if old_first_player_info.role_guid == new_first_player_info.role_guid then
		return;
	end

	local t_cs_msg = {}


    local old_guid_ptr = hld.get_guid_64(old_first_player_info.role_guid)


	local 	old_client_session_ptr = hld.get_client_session_by_guid(old_guid_ptr);
	if old_client_session_ptr ~= nil then
	
		t_cs_msg.role_guid = old_first_player_info.role_guid;
		t_cs_msg.is_first = false;
		t_cs_msg.ranking_type = hld.e_RankingIndex_arena;

		MessageMgr:send_to_cs(old_client_session_ptr, "hld.ws2cs_proto.sync_first_rank_end", t_cs_msg, 1647)
		
	end

    local new_guid_ptr = hld.get_guid_64(old_first_player_info.role_guid)

	local new_client_session_ptr = hld.get_client_session_by_guid(new_guid_ptr);
	if new_client_session_ptr ~= nil then
		
		t_cs_msg.role_guid = new_first_player_info.role_guid;
		t_cs_msg.is_first = true;
		t_cs_msg.ranking_type = hld.e_RankingIndex_arena;

		MessageMgr:send_to_cs(new_client_session_ptr, "hld.ws2cs_proto.sync_first_rank_end", t_cs_msg, 1647)
	
	end
end




function arena_mgr:get_player_arena(role_guid)
	for i = 0, rank_max - 1 do
		if self.m_arena_rank[i].role_guid == role_guid then
			return self.m_arena_rank[i]
		end
	end
	return nil;
end

function arena_mgr:get_data_by_index(index)
	if index < 0 or index >= rank_max then
		return nil 
	end
	return self.m_arena_rank[index]
end

function arena_mgr:reset_challenge_state(self_guid, target_guid, target_rank, self_rank)
	if self_rank >= 0 and 
	   self_rank < rank_max and 
	   self.m_arena_rank[self_rank].role_guid == self_guid then
		
	   self.m_arena_rank[self_rank].challenge_end_time = 0;
	end

	if target_rank >= 0 and
	   target_rank < rank_max and
	   self.m_arena_rank[target_rank].role_guid == target_guid then

		self.m_arena_rank[target_rank].challenge_end_time = 0;
	end
	self.m_t_count.result = self.m_t_count.result + 1
end


function arena_mgr:change_player_name_func(role_guid, role_name)
	for i = 0, rank_max - 1 do
		if self.m_arena_rank[i].role_guid == role_guid then
		   delog("----------change--name----",role_guid, role_name)
           self.m_arena_rank[i].role_name = string.sub(role_name, 1, max_name_size)
		end
	end
end




function arena_mgr_change_player_name(role_guid, role_name)
	  arena_mgr:change_player_name_func(role_guid, role_name)
end

function arena_mgr_sync_arena_military_rank(role_guid,  military_level, role_rank)
	  delog("-----aaaaaa--sdfsdfs---military_level---", military_level)
	  arena_mgr:sync_player_arena_military_rank(role_guid,  military_level, role_rank)
end

function arena_mgr_sync_player_gs(role_guid, gs_value, role_rank)
	  delog("----arena_mgr_sync_player_gs---", gs_value)
	  arena_mgr:sync_player_gs(role_guid, gs_value, role_rank)
end


return arena_mgr












