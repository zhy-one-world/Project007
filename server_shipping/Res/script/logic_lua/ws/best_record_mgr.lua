local best_record_mgr = RequireSingleton("best_record_mgr")

local single_map_record_num = 5;					--每个地图的记录数量


local 	e_best_record_order_none = 0		-- 无
local	e_best_record_order_by_bigger = 1	-- 值更大的记录是更好的记录
local	e_best_record_order_by_smaller = 2	 -- 值更小的记录是更好的记录
local	e_best_record_order_max = 3



function best_record_mgr:init()
	 if is_init() then
	 	return 
	 end
	 self.m_activity_record_list = {}
     self.m_save_interval = 0.0;
	 self.m_sec_tick = 0.0;
	 self.m_is_saving = false;
	 self.m_cur_saving_index = -1;
	 self.m_max_saving_count = -1;
end


function best_record_mgr:save_best_record_to_db(best_record)
	if best_record.map_template_id <= 0 then
		return;
	end
	local t_dp_msg = {};

	t_dp_msg.record_info = {}

	t_dp_msg.record_info.map_template_id = best_record.map_template_id
	t_dp_msg.record_info.role_guid = {}
	t_dp_msg.record_info.role_name = {}
	t_dp_msg.record_info.role_value = {} 

	for i=1,single_map_record_num do
		table.insert(t_dp_msg.record_info.role_guid, best_record.single_map_best_record[i].role_guid)
		table.insert(t_dp_msg.record_info.role_name, best_record.single_map_best_record[i].role_name)
		table.insert(t_dp_msg.record_info.role_value, best_record.single_map_best_record[i].value)
	end

	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_best_record", t_dp_msg , 33902,0)
end

function best_record_mgr:load_all_best_record_from_db()
	 self.m_activity_record_list = {}
	 local t_dp_msg = {}
	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_best_record", t_dp_msg , 33900,0)	
end


function best_record_mgr:get_record_by_template_id(map_template_id)
	
	local  best_record_index = -1;

	for i=1,#self.m_activity_record_list do
		if self.m_activity_record_list[i].map_template_id == map_template_id then
			best_record_index = i
			break;
		end
	end
	if best_record_index < 0 then
       return self:create_record_by_template_id(map_template_id)
	end
	
	return self.m_activity_record_list[best_record_index];
end

function best_record_mgr:get_new_best_record()
	 local  new_best_record = {}
	 new_best_record.map_template_id = 0
	 new_best_record.single_map_best_record = {}

	 for i=1,single_map_record_num do
	 	 new_best_record.single_map_best_record[i] = {}
	 	 new_best_record.single_map_best_record[i].role_guid = 0
	 	 new_best_record.single_map_best_record[i].role_name = ""
	 	 new_best_record.single_map_best_record[i].value = 0
	 end
	 return new_best_record
end


function best_record_mgr:create_record_by_template_id(map_template_id)
	local  map_template_ptr = template_manager:get_line("MapTemplate", map_template_id);
	if  map_template_ptr == nil then
		return nil;
	end
	local order_type = map_template_ptr.BestRecordOrderType;
	if order_type <= 0 then
		return nil;
	end
	local new_best_record = self:get_new_best_record()
	new_best_record.map_template_id = map_template_id;

	table.insert(self.m_activity_record_list, new_best_record)
	local size_activity_record_list = #self.m_activity_record_list;
	return self.m_activity_record_list[size_activity_record_list];
end

function best_record_mgr:get_order_type_by_template_id(map_template_id)
	
	local map_template_ptr = template_manager:get_line("MapTemplate", map_template_id);
	if  map_template_ptr == nil then
		return -1;
	end
	return map_template_ptr.BestRecordOrderType;
end

function best_record_mgr:get_convert_map_template_id(map_template_id)	--活动副本中最好记录改为世界统一,统一设置map_template_id
	
	local map_template_ptr = template_manager:get_line("MapTemplate", map_template_id);
	if  map_template_ptr == nil then
		return -1;
	end
	if map_template_ptr.Type == hld.e_map_type_broken_sky or map_template_ptr.Type == hld.e_map_type_king_of_pk or map_template_ptr.Type == e_map_type_temple or map_template_ptr.Type == e_map_type_empire_treasure then
		
		return map_template_id // 100 * 100 + 1;
	end
	return map_template_id;
end


function best_record_mgr:get_insert_index(best_record,  new_record, order_type)
    --delog("---best_record-----")
	--delog(best_record)
	--delog("---new_record------")
	--delog(new_record)
	
	local insert_index = 1;
	if best_record == nil then
		return -1;
	end
	if order_type <= 0 or order_type >= e_best_record_order_max then
		return -1;
	end

	for i=single_map_record_num,1, -1 do
		    
		if best_record.single_map_best_record[i].role_guid ~= 0 then
				
		
			if order_type == e_best_record_order_by_bigger then
				if best_record.single_map_best_record[i].value >= new_record.value then
				   insert_index = i + 1;
				   break
				end
			else
				if best_record.single_map_best_record[i].value <= new_record.value then
					insert_index = i + 1
					delog("-------------insert_index---pos--", insert_index)
					break
				end
			end
	    end
	end
	return insert_index;
end

function best_record_mgr:judget_if_can_add( best_record, new_record)
	
	if best_record == nil then
		return false;
	end
	if new_record.role_guid == 0 then
		return false;
	end
    local  order_type = self:get_order_type_by_template_id(best_record.map_template_id);
	if order_type <= 0 or order_type >= e_best_record_order_max then
		return false;
	end
	local insert_index = self:get_insert_index(best_record, new_record, order_type);
	if insert_index < 0 or insert_index > single_map_record_num then
		return false;
	end

	for i=1,single_map_record_num do
		if best_record.single_map_best_record[i].role_guid == new_record.role_guid then
			 if order_type == e_best_record_order_by_bigger then
				if new_record.value <= best_record.single_map_best_record[i].value then
					return false;
				else
					return true;
				end
				
			elseif order_type == e_best_record_order_by_smaller then
				if new_record.value >= best_record.single_map_best_record[i].value then			
					return false;
				else
					return true;
				end
			end
		end
	end
	return true
end

function best_record_mgr:insert_record(best_record, new_record, is_from_db)
	   if best_record == nil then
		   return;
	   end
	   if new_record.role_guid == false then
		   return;
	   end
	   local order_type = self:get_order_type_by_template_id(best_record.map_template_id);
	   --delog("-------xxxxxxxxx---order_type------", order_type)
	   local insert_index = self:get_insert_index(best_record, new_record, order_type);
	   if insert_index < 0 or insert_index > single_map_record_num then
			return;
	   end

	   for i= single_map_record_num, insert_index, -1 do
	   	  best_record.single_map_best_record[i] = best_record.single_map_best_record[i - 1]
	   end
	   best_record.single_map_best_record[insert_index] = new_record;
	   if not is_from_db then
			self:save_best_record_to_db(best_record);
	   end
end


function best_record_mgr:del_record(best_record,  new_record)
	
	local del_index = -1;
	if best_record == nil then
		return;
	end
	if new_record.role_guid == 0 then
		return;
	end

	for i=1,single_map_record_num do
		if best_record.single_map_best_record[i].role_guid == new_record.role_guid then
			del_index = i;
			break;
		end
	end
	if del_index < 0 then
		return
	end
	if del_index < single_map_record_num then
		for i= del_index,single_map_record_num -1 do
			 best_record.single_map_best_record[i] = best_record.single_map_best_record[i + 1]
		end
	end
	best_record.single_map_best_record[single_map_record_num] = { role_guid  = 0, role_name = "", value = 0}
end

function best_record_mgr:set_best_record(map_template_id, new_record, is_from_db)
	
	self:team_best_record_role_name_replace(map_template_id, new_record);
	local best_record = self:get_record_by_template_id(map_template_id);
	if not self:judget_if_can_add(best_record, new_record) then
		return;
	end
	if not is_from_db then
		
		self:del_record(best_record, new_record);
	end
	self:insert_record(best_record, new_record, is_from_db);

	if not is_from_db then
		--更新副本最快纪录的公告
	   self:notice_best_record(map_template_id, new_record);
	end
end




function best_record_mgr:notice_best_record(map_template_id, best_record)
	
	local  best_record_str = self:get_record_by_template_id(map_template_id);
	if best_record_str == nil then
		return;
	end
	if best_record_str.single_map_best_record[1].role_guid ~= best_record.role_guid then --不是第一名不发公告
		return;
	end
	local map_template_ptr = template_manager:get_map_template_map_id(map_template_id);
	if map_template_ptr == nil then
		return;
	end
	local map_name = "";
	map_name =  hld.get_str_by_string_template_id(map_template_ptr.MapName, 0)

	local notice_id = map_template_ptr.Noticeld;		--93000034
	local best_value = best_record.value;
	if map_template_ptr.Type == hld.e_map_type_exp_fuben and best_value >= 10000 then
		
		notice_id = exp_best_record_hundred_million_notice_id;
		--保留两位小数
		local temp_best_value = best_value // 100;
		local best_value = temp_best_value / 100;
	end
	if notice_id <= 0 then
		return;
	end
		
	local player_name_str = "";

	local guid_ptr = hld.get_guid_64(best_record.role_guid)
	local team = hld.team_ws_mgr:get_instance():get_team(guid_ptr);
	if team ~= nil  and  self:is_multiple_raid(map_template_ptr.Order) then
		
        player_name_str = team:get_member_list_name()
	
	else
		player_name_str = best_record.role_name;
	end
	local vec_str_time = {};
	if map_template_ptr.Order == hld.e_map_order_type_story_raid and map_template_ptr.Order == hld.e_map_order_type_multiplayer_raid  then
		vec_str_time[1] = self:get_map_difficulty_notice_id(map_template_id);
	else
		local t_notice_id = template_manager:get_str_id_by_notice_id(notice_id);
		vec_str_time[1] = tostring(t_notice_id[1])
	end
	vec_str_time[2] = player_name_str;
	vec_str_time[3] = map_name;
	if map_template_ptr.BestRecordOrderType == 2 then
		
		vec_str_time[4] = tostring(best_record.value // 60);
		vec_str_time[5] = tostring(best_record.value % 60);
	
	elseif map_template_ptr.BestRecordOrderType == 1 then
		
		vec_str_time[4] = tostring(best_value);
	end
	local notice_str = table.concat(vec_str_time, "&#");
	local create_time = os.time();
	hld.event_ws_mgr:get_instance():send_notice_to_all(notice_id, create_time, guid_ptr, notice_str, 0);

end

function best_record_mgr:is_multiple_raid(map_type)
    if map_type == hld.e_map_order_type_multiplayer_raid then
		return true;
	end
	return false
end

function best_record_mgr:team_best_record_role_name_replace(map_template_id, best_record)
	
	local map_template_ptr = template_manager:get_map_template_map_id(map_template_id);
	if map_template_ptr == nil then
		return;
	end
	local guid_ptr = hld.get_guid_64(best_record.role_guid)
	local team = hld.team_ws_mgr:get_instance():get_unit_team(guid_ptr);
	if  team == nil and  self:is_multiple_raid(map_template_ptr.Order) then
		
		local  team_mem_str = team:get_member_list_name();
		best_record.role_guid = team:get_team_guid():get_AB();

		local  fomart_name = string.gsub(team_mem_str, " ", "&#")
		best_record.role_name = fomart_name;
	end
end

local mul_notice_id = 
{
	[0] = "90300101",                            -- //简单
	[1] = "90300102",                            --普通
	[2] = "90300103",                            --困难
	[3] = "90300104",                            --英雄
	[4] = "90300105"                             --地狱
}

function best_record_mgr:get_map_difficulty_notice_id(map_template_id)
	
	local map_template_ptr = template_manager:get_map_template_map_id(map_template_id);
	if  map_template_ptr == nil then
		
		return "";
	end
	if map_template_ptr.Order == hld.e_map_order_type_story_raid then
		 if map_template_ptr.Difficulty == hld.e_map_difficulty_easy then
		 	return tostring(90300098)   --普通
		 elseif map_template_ptr.Difficulty == hld.e_map_difficulty_normal then
		 	return tostring(90300099)   --精英
		 elseif map_template_ptr.Difficulty == hld.e_map_difficulty_heard then
		 	return tostring(90300100)   --炼狱
		 else
		 	return ""
		 end
	elseif map_template_ptr.Order == hld.e_map_order_type_multiplayer_raid then
        if mul_notice_id[map_template_ptr.Difficulty] ~= nil then
        	return mul_notice_id[map_template_ptr.Difficulty]
        else 
        	return ""
        end
    else
        return "" 
    end
end

function best_record_mgr:change_player_name_func(role_guid,  role_name)
	for i=1,#self.m_activity_record_list do
		local cur_elem = self.m_activity_record_list[i]
		for j = 1, single_map_record_num do
			if cur_elem.single_map_best_record[j].role_guid == role_guid then
			   cur_elem.single_map_best_record[j].role_name = role_name
			end
		end
	end
	
end


function best_record_mgr_get_record_by_template_id(role_guid, map_template_id)
    local guid_ptr = hld.get_guid_64(role_guid)
    local session =  hld.get_client_session_by_guid(guid_ptr);

	local get_best_record_end_msg = {};
	
	local  record = best_record_mgr:get_record_by_template_id(map_template_id);
	if  record == nil then
		get_best_record_end_msg.map_template_id = 0;
		
	else
		get_best_record_end_msg.map_template_id = record.map_template_id;

		get_best_record_end_msg.best_record_role_name = {}
		get_best_record_end_msg.best_record_value = {} 

		for i=1,single_map_record_num do
			repeat
			if record.single_map_best_record[i].role_guid == 0  then
				break;
			end
			table.insert(get_best_record_end_msg.best_record_role_name, record.single_map_best_record[i].role_name)
			table.insert(get_best_record_end_msg.best_record_value, record.single_map_best_record[i].value)
			until true
		end	
	end

	MessageMgr:send_to_client(session,"hld.game_proto.get_best_record_end", get_best_record_end_msg, 10325)
end


function best_record_mgr_set_best_record(message_data)
	local msg_data = decode("hld.cs2ws_proto.set_best_record", message_data)
	if msg_data == nil then
		return
	end
	best_record_mgr:set_best_record(msg_data.map_template_id, msg_data.record, false)
end


function best_record_mgr_load_all_best_record(message_data)
	 local msg_data = decode("hld.dp2s_proto.load_all_best_record", message_data)
	 if msg_data == nil then
	 	return
	 end
	 for i=1,msg_data.db_record_num do
	 	 local new_record = {}
	 	 new_record.role_guid = msg_data.db_record_list[i].role_guid
	 	 new_record.role_name = msg_data.db_record_list[i].role_name
	 	 new_record.value = msg_data.db_record_list[i].value

	 	 best_record_mgr:set_best_record(msg_data.db_record_list[i].map_template_id, new_record, true)
	
	 end
end

function best_record_mgr_change_player_name_func(role_guid,  role_name)
	best_record_mgr:change_player_name_func(role_guid, role_name)
end


return best_record_mgr