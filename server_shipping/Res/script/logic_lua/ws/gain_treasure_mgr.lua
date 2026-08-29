local gain_treasure_mgr = RequireSingleton("gain_treasure_mgr")


local  max_team_member_num = 3
local  gain_treasure_gain_item_max_num = 5;

local  gain_treasure_killed_record_one_boss_max_size = 5;
local  gain_treasure_boss_max_size = 150; --最大boss数量



local gain_treasure_gain_item_record_max_size = 30;


local  one_gain_item_record_list_get_num = 30;



local  gain_treasure_record_info_max_size = gain_treasure_boss_max_size * gain_treasure_killed_record_one_boss_max_size;

function gain_treasure_mgr:init()
	 if is_init() then
	 	return;
	 end
	 self.m_record_map = {}
	 self.m_gain_item_record_list = {}
	 self.m_boss_info_map = {}
end


function gain_treasure_mgr:Update(i_nTime)
	-- body
end

function gain_treasure_mgr:init_manager()
	 self:req_load_gain_treasure_record_info_from_db();
end

function gain_treasure_mgr:save_gain_treasure_record_info_into_db(boss_spawn_point_template_id,del_stamp)
	local  one_boss_record_all = self:get_gain_treasure_record_one_boss_info(boss_spawn_point_template_id);
	if  one_boss_record_all  == nil or  #one_boss_record_all.record_info_list <= 0 then
		return;
	end
    local ite = one_boss_record_all.record_info_list[1]
    if ite == nil then
    	return
    end

	local record_info = ite;

    local  t_dp_msg = {} 


	t_dp_msg.del_stamp = del_stamp;
	t_dp_msg.boss_spawn_point_template_id = record_info.boss_spawn_point_template_id;
	t_dp_msg.killed_stamp = record_info.killed_stamp;
	t_dp_msg.role_name = {}
	t_dp_msg.role_guid = {}
	t_dp_msg.gain_item_template_id = {}

	for i=1,max_team_member_num do
		local elem = record_info.player_info_arr[i]
		local single = {}
		if elem ~= nil then																																		
			single.role_name = elem.role_name
			single.role_guid = elem.role_guid
			local item_str = ""
			local t_item_id = {}
			for i=1,gain_treasure_gain_item_max_num do
				if elem.gain_item_template_id ~= nil and elem.gain_item_template_id[i] ~= nil and elem.gain_item_template_id[i] ~= 0 then
				   table.insert(t_item_id, elem.gain_item_template_id[i])
				end
			end
			item_str = table.concat(t_item_id, "&#")
			single.gain_item_template_id = item_str
		else
			single.role_name = ""
			single.role_guid = 0
			single.gain_item_template_id = ""
		end
		table.insert(t_dp_msg.role_name, single.role_name)
		table.insert(t_dp_msg.role_guid, single.role_guid)
		table.insert(t_dp_msg.gain_item_template_id, single.gain_item_template_id)  
	end
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_gain_treasure_record_info", t_dp_msg, 35104, 0)
end

function gain_treasure_mgr:req_load_gain_treasure_record_info_from_db()
	local vec_boss_spawn_point_template_id = self:get_npc_spawn_point_template_id_by_map_type(hld.e_map_type_boss_home);
	for i=1, #vec_boss_spawn_point_template_id do
		repeat
			local npc_spawn_point_template_ptr = template_manager:get_line("NpcSpawnPointTemplate", vec_boss_spawn_point_template_id[i]);
			if npc_spawn_point_template_ptr == nil then
				break;
			end
			local npc_template_ptr = template_manager:get_line("NpcTemplate", npc_spawn_point_template_ptr.TemplateId);
			if  npc_template_ptr == nil  or  npc_template_ptr.NpcType ~= hld.e_unit_type_monster  or npc_template_ptr.SubType ~= hld.e_monster_type_boss then
				break;
			end
			
			local t_dp_msg = {}
			t_dp_msg.boss_spawn_point_id = vec_boss_spawn_point_template_id[i]
			MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_all_gain_treasure_record_info", t_dp_msg, 35102, 0)
		until true
	end
	

	vec_boss_spawn_point_template_id = nil
	vec_boss_spawn_point_template_id = self:get_npc_spawn_point_template_id_by_map_type(hld.e_map_type_boss_vip_home);
	for i=1, #vec_boss_spawn_point_template_id do
		repeat
			local npc_spawn_point_template_ptr = template_manager:get_line("NpcSpawnPointTemplate", vec_boss_spawn_point_template_id[i]);
			if npc_spawn_point_template_ptr == nil then
				break;
			end
			local npc_template_ptr = template_manager:get_line("NpcTemplate", npc_spawn_point_template_ptr.TemplateId);
			if  npc_template_ptr == nil  or  npc_template_ptr.NpcType ~= hld.e_unit_type_monster  or npc_template_ptr.SubType ~= hld.e_monster_type_boss then
				break;
			end
			
			local t_dp_msg = {}
			t_dp_msg.boss_spawn_point_id = vec_boss_spawn_point_template_id[i]
			MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_all_gain_treasure_record_info", t_dp_msg, 35102, 0)
		until true
	end
end

function gain_treasure_mgr:load_gain_treasure_record_info_from_db_proc(record_infos, data_num)
	 for i=1,data_num do
	 	 if i > gain_treasure_record_info_max_size then
	 	 	return
	 	 end
	 	 self:load_gain_treasure_record_info(record_infos[i])
	 end
end

function gain_treasure_mgr:load_gain_treasure_record_info(record_info_db)
	--delog(record_info_db)
	local  record_info = {};
	record_info.boss_spawn_point_template_id = record_info_db.boss_spawn_point_template_id;
	record_info.killed_stamp = record_info_db.killed_stamp;
	record_info.player_info_arr = {}

	for i=1,max_team_member_num do

		record_info.player_info_arr[i] = {}

		record_info.player_info_arr[i].role_name = record_info_db.player_info_arr[i].role_name
		record_info.player_info_arr[i].role_guid = record_info_db.player_info_arr[i].role_guid
		record_info.player_info_arr[i].gain_item_template_id = {}


		local vec_gain_item_template_str = string.split(record_info_db.player_info_arr[i].gain_item_template_id, "&#")

		for j=1,#vec_gain_item_template_str do
			if j > gain_treasure_gain_item_max_num then
				break
			end
			record_info.player_info_arr[i].gain_item_template_id[j] = tonumber(vec_gain_item_template_str[j])
		end 
	end
	--delog(record_info)
	self:add_gain_treasure_record(record_info);
	self:add_gain_item_reocrd_by_one_boss(record_info);
end

function gain_treasure_mgr:get_gain_treasure_record_one_boss_info( boss_spawn_point_template_id)
	  return self.m_record_map[boss_spawn_point_template_id];
end

function gain_treasure_mgr:oper_all_obj_add_record(all_obj, record_info)
	
	 table.insert(all_obj.record_info_list, 1,  record_info)

	 local del_stamp = 0;
	 if #all_obj.record_info_list > gain_treasure_killed_record_one_boss_max_size then

	 	local key = #record_info_list
			
		del_stamp = record_info_list[key].killed_stamp;
		table.remove(all_obj.record_info_list, key)

	 end
	 return del_stamp;
end

function gain_treasure_mgr:add_gain_treasure_record(record_info)
	
	local all_obj = self.m_record_map[record_info.boss_spawn_point_template_id];
	local del_stamp = 0;
	if all_obj == nil then
		
		local all_obj = {}
		all_obj.record_info_list = {}

		self:oper_all_obj_add_record(all_obj, record_info);
		self.m_record_map[record_info.boss_spawn_point_template_id] =  all_obj;

	else
		del_stamp = self:oper_all_obj_add_record(all_obj, record_info);;
	end
	return del_stamp;
end

function gain_treasure_mgr:add_gain_treasure_record_and_save(record_info)
	local boss_spawn_point_template_ptr = template_manager:get_line("NpcSpawnPointTemplate", record_info.boss_spawn_point_template_id);
	if  boss_spawn_point_template_ptr == nil or record_info.killed_stamp <= 0 then
		return;
	end
	local  del_stamp = 0;
	del_stamp = self:add_gain_treasure_record(record_info);
	self:add_gain_item_reocrd_by_one_boss(record_info);
	self:save_gain_treasure_record_info_into_db(record_info.boss_spawn_point_template_id, del_stamp);
end

function gain_treasure_mgr:add_gain_item_record(record_info)

	local cur_record_num = #self.m_gain_item_record_list;
		
	local insert_pos = cur_record_num +1;

	for i=1,cur_record_num do
		local ite = self.m_gain_item_record_list[i]
		if record_info.killed_stamp >= ite.killed_stamp then
			insert_pos = i
			record_info.record_index = ite.record_index + 1
			if record_info.record_index >= gain_treasure_gain_item_record_max_size then
				record_info.record_index = 0;
			end
			break; 
		end
		ite.record_index = ite.record_index + 1
		if ite.record_index >= gain_treasure_gain_item_record_max_size then
		   ite.record_index = 0
		end
	end

	if insert_pos == cur_record_num +1 then
		if cur_record_num == 0 then
			record_info.record_index = 0
		else
			record_info.record_index = self.m_gain_item_record_list[cur_record_num].record_index - 1
		    if record_info.record_index < 0 then
		    	record_info.record_index = gain_treasure_gain_item_record_max_size - 1
		    end
		end
    end

    --delog("--------insert--pos--", insert_pos, record_info.record_index)

    table.insert(self.m_gain_item_record_list, insert_pos, record_info)

    --delog(self.m_gain_item_record_list)


    if cur_record_num > gain_treasure_gain_item_record_max_size then
        table.remove(self.m_gain_item_record_list)
    end
end

function gain_treasure_mgr:add_gain_item_reocrd_by_one_boss(record_info)
	
		--先放记录 再放置顶 可以让置顶的物品一直在记录上面
	self:add_gain_item_reocrd_by_dorp_record_model(record_info, hld.e_drop_record_model_type_record);
	self:add_gain_item_reocrd_by_dorp_record_model(record_info, hld.e_drop_record_model_type_set_top);
end


function gain_treasure_mgr:add_gain_item_reocrd_by_dorp_record_model(record_info, record_model)
	  for i=1,max_team_member_num do
	  	 repeat
			 if record_info.player_info_arr[i].role_name == "" then
			    break
			 end
		  	 for j = 1,gain_treasure_gain_item_max_num do
		  	 	repeat 
	                if record_info.player_info_arr[i].gain_item_template_id[j] == nil then
	                	break
	                end
			  	 	local item_template_ptr = template_manager:get_line("ItemTemplate", record_info.player_info_arr[i].gain_item_template_id[j]);
					if  item_template_ptr == nil  or  item_template_ptr.DropRecordModel ~= record_model then
					
						break
					end
					local gain_item_record_info = {}
					
					gain_item_record_info.boss_spawn_point_template_id = record_info.boss_spawn_point_template_id;
					gain_item_record_info.role_name = record_info.player_info_arr[i].role_name;
					gain_item_record_info.item_template_id = record_info.player_info_arr[i].gain_item_template_id[j];
					gain_item_record_info.killed_stamp = record_info.killed_stamp;
					gain_item_record_info.role_guid = record_info.player_info_arr[i].role_guid;
					gain_item_record_info.record_index = 0
					self:add_gain_item_record(gain_item_record_info);
			    until true
			 end	 	
		 until true
	  end
end


function gain_treasure_mgr:req_kill_record_end(session, boss_spawn_point_template_id)
	
	if  session == nil then
		return;
	end

	local  kill_record_end_msg = {};
	if self:fill_kill_record_end_msg(kill_record_end_msg, boss_spawn_point_template_id) <= 0 then
		return;
	end

	MessageMgr:send_to_client(session, "hld.game_proto.gain_treasure_kill_record_end",kill_record_end_msg, 10346)
end

function gain_treasure_mgr:fill_kill_record_end_msg(kill_record_end_msg,  boss_spawn_point_template_id)
	local boss_spawn_point_template_ptr = template_manager:get_line("NpcSpawnPointTemplate", boss_spawn_point_template_id);
	if  boss_spawn_point_template_ptr == nil then
		return 0;
	end

	local  one_boss_record_ptr = self:get_gain_treasure_record_one_boss_info(boss_spawn_point_template_id);
	if  one_boss_record_ptr == nil or #one_boss_record_ptr.record_info_list <= 0  then
		return 0;
    end
		
	kill_record_end_msg.boss_spawn_point_template_id = boss_spawn_point_template_id;


	kill_record_end_msg.record_arr = {}

	for i=1,#one_boss_record_ptr.record_info_list do
		local ite = one_boss_record_ptr.record_info_list[i]
		local one_record = {}
		one_record.killed_stamp = ite.killed_stamp
		one_record.role_name = {}
		for j=1,max_team_member_num do
			if ite.player_info_arr[i].role_name ~= "" then
				table.insert(one_record.role_name, ite.player_info_arr[i].role_name)
			end
		end
		table.insert(kill_record_end_msg.record_arr, one_record)
	end
	return 1;
end

function gain_treasure_mgr:req_gain_item_record(session,  record_index)
	
	if  session == nil then
		return;
	end

	local gain_item_record_end_msg = {}
	if self:fill_gain_item_record_end_msg(gain_item_record_end_msg, record_index) <= 0 then
		return;
	end
	delog("#####################################")
	delog(gain_item_record_end_msg)
	MessageMgr:send_to_client(session, "hld.game_proto.gain_treasure_gain_item_record_end",gain_item_record_end_msg, 10348)
end

function gain_treasure_mgr:fill_gain_item_record_end_msg( gain_item_record_end_msg,  record_index)

        delog(self.m_gain_item_record_list)
	
		if #self.m_gain_item_record_list <= 0 then
			return 0;
		end

		--先找到已经发送记录的位置
		local index = 1
		
		if record_index >= 0 then

			for i=1, #self.m_gain_item_record_list do
				local ite = self.m_gain_item_record_list[i]
				if ite.record_index == record_index then
					index = i + 1
					break
				end
			end
		end

		gain_item_record_end_msg.record_arr = {}
		for i=0, one_gain_item_record_list_get_num - 1 do
			if index > #self.m_gain_item_record_list then
				break
			end
			local  ite = self.m_gain_item_record_list[index]
	
			if ite.role_name ~= "" then
			    local  one_record = {}
			   	one_record.role_name = ite.role_name;
				one_record.boss_spawn_point_template_id = ite.boss_spawn_point_template_id;
				one_record.item_template_id = ite.item_template_id;
				one_record.killed_stamp = ite.killed_stamp;
				one_record.record_index = ite.record_index;
				one_record.role_guid = ite.role_guid;
				table.insert(gain_item_record_end_msg.record_arr, one_record)
			end
			index = index + 1
		end
		return 1;
end

function gain_treasure_mgr:get_npc_spawn_point_template_id_by_map_id(map_template_id)
	
		local vec_boss_spawn_point_template_id = {};


		local map_template_ptr = template_manager:get_line("MapTemplate", map_template_id);
		if  map_template_ptr == nil then
		
			return vec_boss_spawn_point_template_id;
		end
		local  npc_spawn_id = map_template_ptr.NpcPointId;
		local  next_spawn_id = 0;

		while true do
		
			local npc_spawn_point_template_ptr = template_manager:get_line("NpcSpawnPointTemplate", npc_spawn_id);
			if  npc_spawn_point_template_ptr == nil  or npc_spawn_point_template_ptr.MapId ~= map_template_id then
				break;
			end
			table.insert(vec_boss_spawn_point_template_id, npc_spawn_id)
			
			next_spawn_id = npc_spawn_point_template_ptr.NextSpawnNpcId;
			if next_spawn_id <= npc_spawn_id then
			
				break;
			end
			npc_spawn_id = next_spawn_id;
		end
		return vec_boss_spawn_point_template_id;
end

function gain_treasure_mgr:get_npc_spawn_point_template_id_by_map_type(map_type)
	local  vec_boss_spawn_point_template_id = {}

	if map_type >= hld.e_map_type_max then
		return vec_boss_spawn_point_template_id;
	end

	local  table_ptr = template_manager:get_template("MapTemplate");
	if table_ptr == nil then
		
		return vec_boss_spawn_point_template_id;
	end

	local temp_boss_spawn_point_template_id = nil 

	for  k,v in pairs(table_ptr) do
		repeat 
			temp_boss_spawn_point_template_id = nil 
			local map_template_ptr = v
			if  map_template_ptr == nil or map_template_ptr.Type ~= map_type then
				
			    break;
			end
			temp_boss_spawn_point_template_id = self:get_npc_spawn_point_template_id_by_map_id(k);

			for i=1,  #temp_boss_spawn_point_template_id do
				table.insert(vec_boss_spawn_point_template_id, temp_boss_spawn_point_template_id[i])
			end
			
		until true

	end
	return vec_boss_spawn_point_template_id;
end

function gain_treasure_mgr:get_gain_treasure_boss_info_by_id(boss_spawn_point_template_id)
     return self.m_boss_info_map[boss_spawn_point_template_id];
end


function gain_treasure_mgr:update_gain_treasure_boss_info( boss_info)
    local  ite = self.m_boss_info_map[boss_info.boss_spawn_point_template_id];
	if ite == nil then
		self.m_boss_info_map[boss_info.boss_spawn_point_template_id] =  boss_info
		self:send_gain_treasure_boss_to_all(boss_info.boss_spawn_point_template_id)
		
	else
		
		--boss刚复活或者刚死亡 消息发送给全服玩家
	    if (ite.boss_guid == 0 and boss_info.boss_guid ~= 0)
				or boss_info.boss_guid == 0 then
			
			self.m_boss_info_map[boss_info.boss_spawn_point_template_id] = boss_info
			self:send_gain_treasure_boss_to_all(boss_info.boss_spawn_point_template_id);
			
			--boss归属改变只发给对应地图的玩家
		else
			
			self.m_boss_info_map[boss_info.boss_spawn_point_template_id] = boss_info;
			self:send_gain_treasure_boss_to_map(boss_info.boss_spawn_point_template_id);
		end
	end
end

function gain_treasure_mgr:send_gain_treasure_boss_info_one(session, boss_spawn_point_template_id)
	
	local  boss_info_msg = {};
	if self:fill_boss_info_one_msg(boss_info_msg, boss_spawn_point_template_id) <= 0 then
	   return;
	end
	MessageMgr:send_to_client(session, "hld.game_proto.gain_treasure_boss_info_one", boss_info_msg, 10349)
end

function gain_treasure_mgr:send_gain_treasure_boss_info_all(session)
	
	local boss_info_all_msg = {};

    boss_info_all_msg.boss_info = {}

	local total_len = 10
	local single_len = 0

	for k,v in pairs(self.m_boss_info_map) do
		local boss_info = {}
		self:fill_boss_info_one_msg(boss_info, v.boss_spawn_point_template_id)

		local s_boss_info = encode("hld.game_proto.gain_treasure_boss_info_one", boss_info)

		single_len = string.len(s_boss_info) + 3

		if total_len + single_len > 11500 then
		   MessageMgr:send_to_client(session, "hld.game_proto.gain_treasure_boss_info_all", boss_info_all_msg, 10350)
		   total_len  = 10;
		   boss_info_all_msg.boss_info = {}
		end
		table.insert(boss_info_all_msg.boss_info, boss_info)
		total_len = total_len + single_len
	end

	MessageMgr:send_to_client(session, "hld.game_proto.gain_treasure_boss_info_all", boss_info_all_msg, 10350)
end

function gain_treasure_mgr:fill_boss_info_one_msg(boss_info_one_msg, boss_spawn_point_template_id)
     local  ite = self.m_boss_info_map[boss_spawn_point_template_id];
	 if ite == nil  then
		return 0;
	end
	boss_info_one_msg.boss_spawn_point_template_id = boss_spawn_point_template_id;
	boss_info_one_msg.boss_guid = ite.boss_guid;
	boss_info_one_msg.next_refresh_stamp = ite.next_refresh_stamp;
	boss_info_one_msg.first_player_name = ite.player_info[1].player_name;
	boss_info_one_msg.player_guid = {}

	for i=1,max_team_member_num do
		if ite.player_info[i] ~= nil and ite.player_info[i].player_guid ~= 0 then
           table.insert(boss_info_one_msg.player_guid, ite.player_info[i].player_guid)
		end
	end
	return 1;
end


function gain_treasure_mgr:send_gain_treasure_boss_to_all(boss_spawn_point_template_id)
	
	local boss_info_msg = {};
	if self:fill_boss_info_one_msg(boss_info_msg, boss_spawn_point_template_id) <= 0 then
		return;
	end

	MessageMgr:send_message_to_all_client("hld.game_proto.gain_treasure_boss_info_one", boss_info_msg, 10349)
end

function gain_treasure_mgr:send_gain_treasure_boss_to_map(boss_spawn_point_template_id)
	
	local boss_spawn_point_template_ptr = template_manager:get_line("NpcSpawnPointTemplate", boss_spawn_point_template_id);
	if  boss_spawn_point_template_ptr == nil then
		return;
	end
	local  boss_info_msg = {};
	if self:fill_boss_info_one_msg(boss_info_msg, boss_spawn_point_template_id) <= 0 then
		return;
	end

	local  ite = self.m_boss_info_map[boss_spawn_point_template_id];
	if ite == nil then
		return;
	end
	local  cur_num = 0;

	for i=1,max_team_member_num do
		if ite.player_info[i].player_guid ~= 0 then
			cur_num = cur_num + 1
		end
	end

	if cur_num == 0 then --如果为0则表示仇恨被清除，发给所有人
		MessageMgr:send_message_to_all_client("hld.game_proto.gain_treasure_boss_info_one", boss_info_msg, 10349)
		return;
	end
	MessageMgr:send_message_to_map(boss_spawn_point_template_ptr.MapId, "hld.game_proto.gain_treasure_boss_info_one", boss_info_msg, 10349)
end

function gain_treasure_mgr:change_player_name_func(role_guid, role_name, original_name)

	for k, v in pairs(self.m_boss_info_map) do
		for i=1,max_team_member_num do
			if v.player_info[i].player_guid == role_guid then
				v.player_info[i].role_name = role_name
			end
		end
	end
	for k,v  in pairs(self.m_gain_item_record_list) do
		if v.role_guid == role_guid then
		   v.role_name = role_name
		end
	end
end


function gain_treasure_mgr_req_load_gain_treasure_record_info_from_db( )
	 gain_treasure_mgr:req_load_gain_treasure_record_info_from_db()
end


function gain_treasure_mgr_load_gain_treasure_record_info_from_db_proc(message_data)
	local msg_data = decode("hld.dp2s_proto.load_all_gain_treasure_record_info", message_data)
	if msg_data == nil then
		return
	end
	gain_treasure_mgr:load_gain_treasure_record_info_from_db_proc(msg_data.record_infos, msg_data.data_num)
end

function gain_treasure_mgr_add_gain_treasure_record_and_save(message_data)
	local msg_data = decode("hld.st_proto.st_gain_treasure_record_info_one", message_data)
	if msg_data == nil  then
		return
	end
	gain_treasure_mgr:add_gain_treasure_record_and_save(msg_data)
end

function gain_treasure_mgr_req_kill_record_end(role_guid, boss_spawn_point_template_id)
    local guid_ptr = hld.get_guid_64(role_guid)
	local session = hld.get_client_session_by_guid(guid_ptr)
	if session == nil then
		return
	end
	gain_treasure_mgr:req_kill_record_end(session, boss_spawn_point_template_id)
end

function gain_treasure_mgr_req_gain_item_record(role_guid, record_index)
	local guid_ptr = hld.get_guid_64(role_guid)
	local session = hld.get_client_session_by_guid(guid_ptr)
	if session == nil then
		return
	end
	gain_treasure_mgr:req_gain_item_record(session, record_index)
end

function gain_treasure_mgr_update_gain_treasure_boss_info(message_data)
	local msg_data = decode("hld.st_proto.st_gain_treasure_boss_info", message_data)
	if msg_data == nil then
		return
	end
	gain_treasure_mgr:update_gain_treasure_boss_info(msg_data)
end

function gain_treasure_mgr_send_gain_treasure_boss_info_all(role_guid)
    local guid_ptr = hld.get_guid_64(role_guid)
	local session = hld.get_client_session_by_guid(guid_ptr)
	if session == nil then
		return
	end

	ProtectedCall(function() gain_treasure_mgr:send_gain_treasure_boss_info_all(session) end);

	--gain_treasure_mgr:send_gain_treasure_boss_info_all(session)
end


function gain_treasure_mgr_change_player_name_func(role_guid, role_name, original_name)
	gain_treasure_mgr:change_player_name_func(role_guid, role_name, original_name)
end



return gain_treasure_mgr
