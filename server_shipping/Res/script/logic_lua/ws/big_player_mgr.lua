local big_player_mgr = RequireSingleton("big_player_mgr")

--[[
开启
]]


function big_player_mgr:init()
	if is_init() then
	    return
	end
	self:clear_data()
end

function big_player_mgr:clear_data()
	 self.player_guid_array = {}
	 self.player_info_array = {}
	 self.statue_guid_array = {}
	 self.create_list = {}
	 self.m_last_save_time = 0
	 self.m_timer = 0
	 self.m_last_tick_time = 0

end


function big_player_mgr:Update(i_nTime)
	if not is_big_player_use_lua() then
	   return
	end 
	self.m_timer = self.m_timer + i_nTime;
	if self.m_timer - self.m_last_tick_time > 1000 then
		
		self.m_last_tick_time = self.m_timer;
		if self.m_timer - self.m_last_save_time > 180000 then
			self.m_last_save_time = self.m_timer;
				--三分钟一存档
			self:save_data_to_db();
		end
	end

	--self:show_info()
end


local show_inteval_time = 10000
function big_player_mgr:is_show()
     local cur_msec = get_g_time_mesc()
     if self.m_last_time == nil then
     	self.m_last_time = 0
     end
     if self.m_last_time > cur_msec then
        return false
     end
     self.m_last_time = cur_msec + show_inteval_time
     return true
end


function big_player_mgr:show_info()
    if not self:is_show() then
        return
    end 


end


function big_player_mgr:set_big_player_guid(big_type, player_guid, from)
	delog("----big_type--player_guid-", big_type, player_guid,from)
	if big_type >= hld.e_big_player_type_max then
		return;
	end
	local old_player_guid = player_guid;
	self.player_guid_array[big_type] = player_guid;
end

function big_player_mgr:get_big_player_guid(big_type)

	delog("------big_type---", big_type)
	
	if big_type >= hld.e_big_player_type_max then
		
		return 0
	end
	if self.player_guid_array[big_type] == nil then
		return 0
	end
	return self.player_guid_array[big_type]
end

function big_player_mgr:save_data_to_db()
	local t_dp_msg = {}
	t_dp_msg.role_guid = {}
	t_dp_msg.player_type = {}


	for i= 0, hld.e_big_player_type_max - 1 do
		table.insert(t_dp_msg.role_guid, self:get_big_player_guid(i))
		table.insert(t_dp_msg.player_type, i)
	end
	delog(t_dp_msg)

	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_big_player_to_db", t_dp_msg, 34000, 0)
end

function big_player_mgr:load_data_by_db()
	 local t_dp_msg = {}
	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_big_player_by_db", t_dp_msg, 34001, 0)
end

------------------------------------------------------------------------------------------------
                       --s_fake_player_info--
-----------------------------------------------------------------------------------------------
        
local efpi_none = 0
local efpi_info = 1
local efpi_att = 2
local efpi_skill = 3
local efpi_item = 4
local efpi_buff = 5
local efpi_spirit = 6
local efpi_fight_att = 7
local efpi_belief = 8
local efpi_max = 9

function big_player_mgr:get_s_unit_info( )
	 local t_unit = {
	  role_guid = 0,
	  account = "",
	  role_name = "",
	  role_appearance = "",
	  role_first_kill_ary = "",
	  map_guid = 0,
	  data_i64_ary = {},
	  data_ary = {},
	 }
	 for i=1, hld.e_role_i64_info_max do
	 	t_unit.data_i64_ary[i] = 0
	 end
	 for i=1, hld.e_role_info_max do
	 	t_unit.data_ary[i] = 0
	 end
	 return t_unit
end
local e_base_att_info_max = 18

function big_player_mgr:get_s_base_att_info( )
	local t_base = 
	{
	   data_ary = {} 
    }
    for i=1,e_base_att_info_max do
    	t_base.data_ary[i] = 0
    end
    return t_base
end
function big_player_mgr:get_s_skill_info( num )
	local t_n_skill = {}


	for i=1,num do
		local t_single_skill = 
		{
	     data_ary = {}
	    }
	    for i=1,hld.e_skill_info_max do
	    	t_single_skill.data_ary[i] = 0
	    end
	    t_n_skill[i] = t_single_skill
	end

    return t_n_skill
end

function big_player_mgr:get_s_item_info( num )
	local t_n_item_info = {}

	for i=1,num  do
		local t_single_item_info = {
		    item_guid = 0,
		    data_ary = {},
		    param_str = "",
	    }
	    for i=1, hld.e_item_info_max do
	    	t_single_item_info.data_ary[i] = 0
	    end
	    t_n_item_info[i] = t_single_item_info
	end
	return t_n_item_info
end

function big_player_mgr:get_s_arena_char_fight_att()
	 local t_area = 
	 {
	  role_guid = 0,
	  att_value = {}
	 }
	 for i=1,hld.e_unit_attack_att_max do
	 	t_area.att_value[i] = 0
	 end
	 return t_area
end

local ESpiritInfo_max = 21

function big_player_mgr:get_s_spirit_info()
	 local t_spirit = 
	 {
	   spirit_guid = 0,
	   data_ary = {}
	 }
	 for i=1, ESpiritInfo_max do
	 	t_spirit.data_ary[i] = 0
	 end
	 return t_spirit
	
end

function big_player_mgr:get_new_s_fake_player_info( )
	 local new_info =  {}
	 new_info.data_block_mask = 0
	 new_info.guid = 0
	 new_info.role_info = self:get_s_unit_info()
	    ------------------------------
	 new_info.att_info = self:get_s_base_att_info()

	 new_info.skill_num = 1
	 new_info.skill_data = self:get_s_skill_info(1)
	 new_info.item_num = 1
	 new_info.item_data = self:get_s_item_info(1)
	 new_info.buff_num = 0
	 new_info.buff_data = {}
	 new_info.m_fight_att = self:get_s_arena_char_fight_att()
	 new_info.spirit_data = self:get_s_spirit_info()
	 new_info.belief_num = 0
	 new_info.belief_data = {}
	 return new_info
end

function big_player_mgr:mask_data_block(player_info, id)
	 if id >= efpi_info and id < efpi_max then
	 	player_info.data_block_mask = player_info.data_block_mask | 1 << (id - 1)
	 end
end

function big_player_mgr:all_data_block_masked(player_info)
	  local flag = player_info.data_block_mask
	  local all_masked = true;
	  for i= efpi_info,efpi_max - 1 do
	  	  local res = (flag >> (i - 1)) & 1
	  	  if res == 0 then
	  	  	all_masked = false
	  	  	break;
	  	  end
	  end
	  return all_masked
end

function big_player_mgr:reset_data_block_mask(player_info)
	   player_info.data_block_mask = 0
end



------------------------------------------------------------------
-------------------------------------------------------------------

function big_player_mgr:load_detail_data(index)

	if index < 0 or index >= hld.e_big_player_type_max then
		return;
	end
	local TempGuid= self:get_big_player_guid(index);
	if TempGuid == 0 then 
		return;
	end

	local  t_dp_msg = { };
	t_dp_msg.role_guid = TempGuid;
	t_dp_msg.big_type = index;
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_big_player_detail", t_dp_msg, 34002, 0)


	if self.player_info_array[index] == nil then
	   self.player_info_array[index] = self:get_new_s_fake_player_info()
	end

	--这俩不load
	self:mask_data_block(self.player_info_array[index], efpi_spirit)
	self:mask_data_block(self.player_info_array[index], efpi_fight_att)
end


function big_player_mgr:load_data_by_db_end(data_info, data_num)


	if data_num > hld.e_big_player_type_max then
		return;
	end
	for i=1,data_num do
	    repeat 
			local player_type = data_info[i].player_type

			if player_type >= hld.e_big_player_type_max then
				break
			end
			local cross_activity = hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity)
			cross_activity = (cross_activity ~= 0 and true) or false
			local cross_city_war = hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_city_war)
			cross_city_war = (cross_city_war ~= 0 and true) or false

			if  hld.get_world_server():is_sky_island_server() == false and hld.init_unit:is_cross_server_big_player(player_type, cross_activity,cross_city_war) == true then
				break
			end
	
		    self.player_guid_array[player_type] = data_info[i].role_guid
		    if self.player_info_array[player_type] == nil then
		       self.player_info_array[player_type] = self:get_new_s_fake_player_info()
		    end
		    self.player_info_array[player_type].guid = hld.make_guid_lua(0)
		    self:load_detail_data(player_type)
	    until true
	end

	--delog("----result------")


	--for i= 0,16 do
	--	print("-----------k-----v----", i, self.player_guid_array[i])
	--end



	if hld.get_world_server():get_cross_id() <= 0 then
		hld.get_world_server():set_ws_loading_flag(e_ws_flag_big_player)
	else
		if hld.get_world_server():is_sky_island_server() then
			hld.get_world_server():set_ws_loading_flag(e_ws_flag_big_player)
			self:send_big_player_msg_to_other_server(true, 0)
		else
			hld.get_world_server():send_to_gate_get_msg(e_ws_flag_big_player)
		end
	end
end

function big_player_mgr:handle_base_detail_info(big_type,role_info)
	
	if  big_type < 0 or big_type >= hld.e_big_player_type_max then
		return;
	end
	player_info_array[big_type].role_info = role_info;
	self:mask_data_block(player_info_array[big_type], efpi_info)

	self:load_end(big_type);
end



function big_player_mgr:handle_item_info(big_type, item_info)

	local role_show_item_num = hld.e_role_equip_slot_max + hld.e_fashion_equip_slot_max + 4
	
	if big_type < 0 or big_type >= hld.e_big_player_type_max then
		return;
	end
	local item_count = 0;
	local idx = 1
	for i=1, role_show_item_num do
		if item_info[i] ~= nil and item_info[i].item_guid ~= 0 then
           self.player_info_array[big_type].item_data[idx] = deep_copy(item_info[i])
           idx = idx +1
           item_count = item_count + 1
		end
	end
	self.player_info_array[big_type].item_num = item_count
	self:mask_data_block(self.player_info_array[big_type], efpi_item)

	self:load_end(big_type)
end

function big_player_mgr:handle_att_info(big_type, att_info)
	
	if big_type < 0 or big_type >= hld.e_big_player_type_max then
		return;
	end
	self.player_info_array[big_type].att_info = att_info;
	self:mask_data_block(self.player_info_array[big_type], efpi_att)
	
	self:load_end(big_type);
end

local MAX_SKILL_NUM = 300
function big_player_mgr:handle_skill_info(big_type, skill_info)
	
	if big_type < 0 or big_type >= hld.e_big_player_type_max then
		return;
	end
	local skill_num = 0
	local idx = 1
	for i=1,MAX_SKILL_NUM do
		if skill_info[i] ~= nil then
			self.player_info_array[big_type].skill_data[idx]  = skill_info[i]
			idx = idx + 1
		 	skill_num = skill_num + 1
	    end	
	end
	self.player_info_array[big_type].skill_num = skill_num

	self:mask_data_block(self.player_info_array[big_type], efpi_skill)
	self:load_end(big_type);
end

local MAN_UNIT_BUFF_INST = 500
function big_player_mgr:handle_buff_info(big_type,  buff_info)	
	if big_type < 0 or big_type >= hld.e_big_player_type_max then
		return;
	end

	local buff_num = 0
	local idx = 1

	for i=1,MAN_UNIT_BUFF_INST do
		if buff_info[i] ~= nil then
			self.player_info_array[big_type].buff_data[idx] = buff_info[i]
			idx = idx +1
			buff_num = buff_num +1
		end
	end
	self.player_info_array[big_type].buff_num = buff_num
	self:mask_data_block(self.player_info_array[big_type], efpi_buff)

	self:load_end(big_type);
end

function big_player_mgr:load_end(big_type)
	if self.player_info_array[big_type].all_data_block_masked() then
		for i= 0, hld.e_big_player_type_max - 1 do
			if self.create_list[i] == nil then
			   self.create_list[i] = big_type
			end	
		end
	end 
end

local  big_world_map_id = 71000001;				--世界地图id
function big_player_mgr:crate_big_player_statue_by_big_type(big_type)
	
	--if not self.player_info_array[big_type].all_data_block_masked() then
		--return false;
	--end
	--这里不能用 globle_data 读表，会读出nullptr
	local game_config_ptr = template_manager:get_line("GameConfigTemplate", 99000001)
	if game_config_ptr == nil then
		return false;
	end
	delog("---sdfsdfsdf-----")

	local player_info = self.player_info_array[big_type]
	delog(player_info)
	local s_player_info = encode("hld.st_proto.st_fake_player_info", player_info)

	delog("------decode---sucess-----")

	local player_info_ptr = hld.get_fake_player_info_by_str(s_player_info, string.len(s_player_info))

	hld.cs_map_mgr_system:send_big_player(big_world_map_id, player_info_ptr, big_type);
	self:reset_data_block_mask(self.player_info_array[big_type])
	return true;
end

function big_player_mgr:crate_big_player_statue_success(big_type, statue_guid)
	  if big_type < 0 or big_type >= hld.e_big_player_type_max then
		 return;
	  end
	  self.statue_guid_array[big_type] = statue_guid;
	  self:send_big_player_statue_info();
end

function big_player_mgr:send_big_player_statue_info()
    
    local t_client_msg = {}
    t_client_msg.statue_guid = {}

    for i= 0,hld.e_big_player_type_max do
    	if self.statue_guid_array[i] == nil then
           table.insert(t_client_msg.statue_guid, 0)
           table.insert(t_client_msg.statue_guid, 0)
    	else
    	   local guid_ptr = hld.get_guid_64(self.statue_guid_array[i])
    	   table.insert(t_client_msg.statue_guid, guid_ptr:get_A())
    	   table.insert(t_client_msg.statue_guid, guid_ptr:get_B())
    	end
    end
    --delog(t_client_msg)
    MessageMgr:send_message_to_all_client("hld.character_proto.update_big_player_statue_guid", t_client_msg, 13301)		
end

function big_player_mgr:load_statue_by_ws_proc(request_player_guid)
    local t_client_msg = {}
    t_client_msg.statue_guid = {}

    for i= 0,hld.e_big_player_type_max do
    	if self.statue_guid_array[i] == nil then
           table.insert(t_client_msg.statue_guid, 0)
           table.insert(t_client_msg.statue_guid, 0)
    	else
    	   local guid_ptr = hld.get_guid_64(self.statue_guid_array[i])
    	   table.insert(t_client_msg.statue_guid, guid_ptr:get_A())
    	   table.insert(t_client_msg.statue_guid, guid_ptr:get_B())
    	end
    end
    delog("-------load_statue_by_ws_proc--")
    delog(t_client_msg)
    local guid_ptr = hld.get_guid_64(request_player_guid)
    
	local temp_session = hld.get_client_session_by_guid(guid_ptr)
		-- nullptr说明不在线/不存在，cs_uid = 0 说明不在CS上（比如传送中）
	if temp_session ~= nil and temp_session:get_cs_conn_index() >= 0 then
	    MessageMgr:send_to_client(temp_session, "hld.character_proto.update_big_player_statue_guid", t_client_msg, 13301)
	end
end

function big_player_mgr:send_big_player_msg_to_other_server(is_load_flag, server_id)
	if hld.get_world_server():is_sky_island_server() == false or  hld.get_gate_proxy():is_gate_run() == false then
	    return;
	end
	local t_ws_msg = {}

	t_ws_msg.data_info = {}

	for i= 0,hld.e_big_player_type_max - 1 do
        local tmp = {}
        tmp.player_type = i
        tmp.role_guid = self.player_guid_array[i] or 0

        table.insert(t_ws_msg.data_info, tmp)
	end

	t_ws_msg.is_load_flag = is_load_flag

	delog(t_ws_msg)


	local guid_ptr = hld.get_guid_64(0)

	local s_data_buffer = encode("hld.ws2ws_proto.send_big_player_msg", t_ws_msg)
   
    local out_len = 0
	local msg_ptr, len = hld.get_send_big_player_msg(s_data_buffer, string.len(s_data_buffer), out_len)
	if msg_ptr == nil then
		return
	end
	hld.cross.send_msg_to_ws(guid_ptr, server_id, 34011, msg_ptr, len);
end

function big_player_mgr:is_set_title_session_login(i, role_guid)
	
	local cur_guid = self:get_big_player_guid(i)
	if cur_guid == role_guid then
	   	 return true
	end
	return false
end




function big_player_mgr:clear_title_with_big_player_clear(big_player_type)
	local title_type = self:get_big_player_title_type(big_player_type);
	if title_type < 0 then
		return;
	end

	delog("--clear_title_with_big_player_clear")

	local client_session_mgr_ptr =   hld.get_client_session_mgr()
	if  client_session_mgr_ptr == nil then
		return
	end
	
	local session_used_num = client_session_mgr_ptr:get_session_array_used();

	for i = 0 , session_used_num do
		local client_session_ptr = client_session_mgr_ptr:get_session_by_use_index(i)
		if client_session_ptr:is_vaild() then
			local t_cs_msg = {
			  role_guid = client_session_ptr:get_role_guid():get_AB(),
			  title_type = title_type 
		    }
		    --delog(t_cs_msg)
		    MessageMgr:send_to_cs(client_session_ptr, "hld.ws2cs_proto.clear_big_player_title", t_cs_msg, 1556)
		end
	end
end


function big_player_mgr:get_big_player_title_type(player_type)

	if player_type == hld.e_big_player_type_lord_of_city then
		return hld.e_title_type_legion_city_win_chief
	elseif player_type == hld.e_big_player_type_lord_or_war then
		return hld.e_title_type_kingofpk;
	elseif player_type == hld.e_big_player_type_best_barserker then
		return hld.e_title_type_best_barserker
	elseif player_type == hld.e_big_player_type_best_wizard then
		return hld.e_title_type_best_wizard
	elseif player_type == hld.e_big_player_type_best_guardian then
		return hld.e_title_type_best_guardian 
	elseif player_type == hld.e_big_player_type_best_assassinator then
		return hld.e_title_type_best_assassinator;
	elseif player_type ==  hld.e_big_player_type_gs_first_barserker then
		return hld.e_title_type_gs_ranking_barserker
	elseif player_type == hld.e_big_player_type_gs_first_wizard then
		return hld.e_title_type_gs_ranking_wizard
	elseif player_type == hld.e_big_player_type_gs_first_guardian then
		return hld.e_title_type_gs_ranking_guardian
	elseif player_type == hld.e_big_player_type_gs_first_assassinator then
		return hld.e_title_type_gs_ranking_assassinator
	elseif player_type == hld.e_big_player_type_gs_first_player then
		return hld.e_title_type_gs_rank_first
	elseif player_type == hld.e_big_player_type_money_first_player then
		return hld.e_title_type_money_rank_first;
	elseif player_type == hld.e_big_player_type_worship_first_player then
		return hld.e_title_type_worship_rank_first;
	elseif player_type == hld.e_big_player_type_arena_first_player then
		return hld.e_title_type_arena_rank_first
	else
		return -1;
	end
	return -1;
end

function big_player_mgr:get_big_player_type_with_ranking_type(ranking_type)
    if ranking_type == hld.e_RankingIndex_gs_barserker then
		return hld.e_big_player_type_gs_first_barserker
	elseif ranking_type ==  hld.e_RankingIndex_gs_wizard then
		return hld.e_big_player_type_gs_first_wizard
	elseif ranking_type == hld.e_RankingIndex_gs_guardian then
		return hld.e_big_player_type_gs_first_guardian
	elseif ranking_type == hld.e_RankingIndex_gs_assassinator then
		return hld.e_big_player_type_gs_first_assassinator
	elseif ranking_type ==  hld.e_RankingIndex_gs then
		return hld.e_big_player_type_gs_first_player
	elseif ranking_type ==  hld.e_RankingIndex_money then
		return hld.e_big_player_type_money_first_player
	elseif ranking_type == hld.e_RankingIndex_worship then
		return hld.e_big_player_type_worship_first_player
	elseif ranking_type ==  hld.e_RankingIndex_arena then
		return hld.e_big_player_type_arena_first_player;
	else
		return -1;
	end	
	return -1;
	-- body
end

function big_player_mgr_set_big_player_guid(big_type, player_guid, from)
	 big_player_mgr:set_big_player_guid(big_type, player_guid, from)
end

function big_player_mgr_get_big_player_guid(big_type)
	  local ret_guid = big_player_mgr:get_big_player_guid(big_type)
	  return tostring(ret_guid)
end

function big_player_mgr_load_data_by_db_end(message_data)
	 local msg_data = decode("hld.dp2s_proto.load_big_player_end", message_data)
	 if msg_data == nil then
	 	return
	 end
	 big_player_mgr:load_data_by_db_end(msg_data.data_info,hld.e_big_player_type_max)
end

function big_player_mgr_crate_big_player_statue_success(big_type, big_player_guid)
	 big_player_mgr:crate_big_player_statue_success(big_type, big_player_guid)
end

function big_player_mgr_load_statue_by_ws_proc(request_player_guid)
	 big_player_mgr:load_statue_by_ws_proc(request_player_guid)
end

function big_player_mgr_send_big_player_msg_to_other_server(is_load_flag, server_id)
	 big_player_mgr:send_big_player_msg_to_other_server(is_load_flag, server_id)
end

function big_player_mgr_is_set_title_session_login(i, role_guid)
	 big_player_mgr:is_set_title_session_login(i, role_guid)
end


function big_player_mgr_test_func()
	delog("sssssssss")
	ProtectedCall ( function()  big_player_mgr:crate_big_player_statue_by_big_type(15) end )
	--ProtectedCall ( function()  big_player_mgr:send_big_player_msg_to_other_server(true, 0) end ) 
	--for i=0,16 do
		--print("-----player_type--", i, big_player_mgr:get_big_player_title_type(i))
	--end

	--ProtectedCall(function( )
	--	big_player_mgr:crate_big_player_statue_success(15,6862246746932651835)
	--end)


end







return big_player_mgr
