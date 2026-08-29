local red_package_mgr = RequireSingleton("red_package_mgr")

local red_package_total_num = 50;
local red_package_max_receiver_num = 50;

local packege_length_max = 11500;	

---------------------------------------------
--开启 数据库协议 33021, 2 33022, 2 加载
--开启 WS  33011, 2  33013, 2
--开启 CS  33012, 5, 33014, 5
--[[
33021	2
33022	2
33011	2
33013	2
33012	5
33014	5
]]




-----------------------------------------------



function red_package_mgr:init()
	 if is_init() then
	 	return
	 end

	 -----------------------------------------
	 self.m_red_package_arr = {}
	 self.m_red_count = 0



     self.m_tick_count = 0
     self.m_auto_save_count = 0;

	 -------------------------------------
end

local const_sec_interval = 1000
local const_save_interval = 600000


function red_package_mgr:Update(i_nTime)
      if not is_red_use_lua()  then
      	 return
      end

	  self.m_tick_count  = self.m_tick_count + i_nTime
	  self.m_auto_save_count = self.m_auto_save_count + i_nTime
	  if self.m_tick_count < const_sec_interval then
	  	 return
	  end
      self.m_tick_count = 0
	  self:sec_tick()
end

function red_package_mgr:sec_tick()
	
	local game_config_ptr = template_manager:get_line("GameConfigTemplate", 99000001);
	if game_config_ptr == nil then
		return;
	end

	local cur_time = os.time()
	local del_list = {}
	for k, v in pairs(self.m_red_package_arr) do
		local red_obj = v
		if red_obj:get_remain_times() <= 0 or (cur_time - red_obj:get_create_time() > game_config_ptr.RedPackageAutoDelTime) then
			self:del_red_msg(red_obj)
			table.insert(del_list, red_obj:get_guid())
		end
	end
	for i=1, #del_list do
		self:del_red(del_list[i])
	end

	if self.m_auto_save_count < const_save_interval then
		return
	end
	self.m_auto_save_count = 0

	self:save_tick()
end

function red_package_mgr:save_tick()
	  self:save_info_to_db()
end


function red_package_mgr:save_info_to_db()
	 local t_save = {}
	 t_save.data_num = 0;
	 t_save.data_ary = {}
	 for k, v in pairs(self.m_red_package_arr) do
	 	 local t_single  = v:get_db_proto_data()
	 	 table.insert(t_save.data_ary, t_single)
	 	 t_save.data_num = t_save.data_num + 1
	 end


	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_save_red_package", t_save, 33015,0)
end



function red_package_mgr:find_package_index(red_package_guid)
	  return self.m_red_package_arr[red_package_guid]
end

function red_package_mgr:find_empty_slot( )
	  if self.m_red_count < red_package_total_num then

	  	  return true
	  end
	  return false
end


function red_package_mgr:add_red(red_obj)
	 local key = red_obj:get_guid()
	 if key == 0 then
	 	return
	 end
	 if self:find_empty_slot() == false then
	 	return
	 end
	 self.m_red_package_arr[key] = red_obj

	 self.m_red_count = self.m_red_count + 1

end

function red_package_mgr:del_red(key)
	 if self.m_red_package_arr[key] == nil then
	 	return
	 end
	 self.m_red_package_arr[key] = nil 
	 self.m_red_count = self.m_red_count - 1
end



function red_package_mgr:load_receiver_by_db(red_package_guid)
	local t_dp_data = 
	{
      red_package_guid = red_package_guid
	}
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_load_red_package_receiver", t_dp_data,33017, 0)
end



function red_package_mgr:load_red_package_end(msg_data)
     if not is_red_use_lua()  then
      	 return
     end
	 for i=1, #msg_data.red_package_list do
	 	repeat
	 		 local red_package_guid = msg_data.red_package_list[i].red_package_guid
	 		 local target_red_obj = self:find_package_index(red_package_guid)
	 		 if target_red_obj ~= nil then
	 		 	 break;
	 		 end
	 		 if not self:find_empty_slot() then
	 		 	break
	 		 end

		 	 local red_obj = NewClass("red_package")
		 	 red_obj:set_base_info(msg_data.red_package_list[i])

		 	 self:add_red(red_obj)

		 	 if red_obj:get_remain_times() > 0 then
		 	 	self:load_receiver_by_db(red_obj:get_guid())
		 	 end
	 	until true
	 end
end

function red_package_mgr:load_red_package_receiver_end(msg_data)
     if not is_red_use_lua()  then
      	 return
     end
	 if msg_data.data_num <= 0 or #msg_data.receiver_list >= red_package_max_receiver_num then
	 	 return
	 end

	 local  red_package_guid = msg_data.receiver_list[1].red_package_guid;
	 local  red_obj = self:find_package_index(red_package_guid)
	 if red_obj == nil then
	 	return 
	 end

	 for i=1, msg_data.data_num do
	 	 red_obj:add_receiver(msg_data.receiver_list[i])
	 end
end


function red_package_mgr:get_oldest_red_package_slot()
	delog("---enter--get_oldest_red_package_slot---")
	local oldest_create_time = 0;
	local res = nil;

	for k, v in pairs(self.m_red_package_arr) do
		if v:get_template_id() ~= 0 then
	       if oldest_create_time == 0 then
	       	  oldest_create_time = v:get_create_time() 
	       	  res = v
	       elseif v:get_create_time() < oldest_create_time then
	       	  oldest_create_time = v:get_create_time()
	       	  res = v
	       	end
	    end
	end
	return res;
end


function red_package_mgr:del_red_msg(red_obj)
	 
   local guid_ptr = hld.get_guid_64(red_obj:get_guid())

   local t_client = 
   {
     red_package_guid = { guid_ptr:get_A(), guid_ptr:get_B()}

   }
   MessageMgr:send_message_to_all_client("hld.character_proto.del_red_package", t_client, 13408)


   local t_dp_data = 
   {
      red_package_guid = red_obj:get_guid()
   }

   delog(t_dp_data)

   MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_del_red_package_by_sql", t_dp_data, 33020, 0)
end




function red_package_mgr:get_suit_slot()
	if not self:find_empty_slot() then
       local old_red_obj = self:get_oldest_red_package_slot()
       if old_red_obj == nil then
          return false
       end
       -----------------------------------------------

       self:del_red_msg(old_red_obj)
       self:del_red(old_red_obj:get_guid())
       return self:find_empty_slot()
	end 
	return true
end


function red_package_mgr:add_red_package(red_package_template_id, sender_guid, sender_name, text)
 
	local red_pack_template_ptr = template_manager:get_line("RedPackageTemplate", red_package_template_id);
	if red_pack_template_ptr == nil then
		return ConstDefine.e_item_string_unkown;
    end

    local guid_ptr = hld.get_guid_64(sender_guid)

	local session = hld.get_client_session_by_guid(guid_ptr);
	if session == nil then
		
		return ConstDefine.e_item_string_unkown;
	end

	local  is_empty_slot = self:get_suit_slot();
	if not is_empty_slot then
		return e_red_bag_no_more_space;
	end

	local new_red_obj = NewClass("red_package")


	local vip_level = session:get_role_info_data(hld.e_role_info_vip_level)
	
	
	local add_result = new_red_obj:init_by_info(red_package_template_id, sender_guid, vip_level,sender_name, text);
    
    self:add_red(new_red_obj)

	if add_result == ConstDefine.e_red_bag_send_success or
	   add_result == ConstDefine.e_red_bag_get_globel_success or
	   add_result == ConstDefine.e_red_bag_get_legion_success or
	   add_result == ConstDefine.e_red_bag_get_vip_success then

	    local t_dp_msg = new_red_obj:get_db_base_proto_data()

	    MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_add_red_package_to_sql", t_dp_msg, 33018, 0)
	
		self:send_info_to_all(new_red_obj);

		local notice_id = red_pack_template_ptr.NoTiceId;

		local t_notice_id = template_manager:get_str_id_by_notice_id(notice_id);

        local vec_notice_str = { tostring(t_notice_id[1]), new_red_obj:get_sender_name(), new_red_obj:get_send_text()}

 

        local notice_str = table.concat( vec_notice_str, "&#")


		local create_time = os.time();

		local send_guid_ptr = hld.get_guid_64(new_red_obj:get_sender_guid())
		hld.event_ws_mgr:get_instance():send_notice_to_all(notice_id, create_time, send_guid_ptr , notice_str, 0);
	end

	return add_result;
end


function red_package_mgr:send_info_to_all(red_obj)
    delog("------red_package_mgr:send_info_to_all-----")
	if red_obj == nil then
	  	 return
	end

	local temp_template_id = red_obj:get_template_id();
	if temp_template_id <= 0 then
	  	return
	end
	local red_pack_template_ptr = template_manager:get_line("RedPackageTemplate", temp_template_id);
	if red_pack_template_ptr ==  nil then
		return
	end


	if red_pack_template_ptr.Type == hld.e_red_package_type_globel or
	   red_pack_template_ptr.Type == hld.e_red_package_type_vip then
	   local t_client = { info_list = {} }

	   self:add_info_to_msg(t_client, red_obj)

	   MessageMgr:send_message_to_all_client("hld.character_proto.update_red_pack_info", t_client, 13407)

	elseif red_pack_template_ptr.Type == hld.e_red_package_type_legion then

        local t_client = { info_list = {} }

	    self:add_info_to_msg(t_client, red_obj)

	    local guid_ptr = hld.get_guid_64(red_obj:get_sender_guid())

	    local temp_legion = hld.legion_ws_mgr:get_instance():get_unit_legion(guid_ptr);

	    if temp_legion ~= nil then

	       MessageMgr:send_message_to_all_legion_member(temp_legion, "hld.character_proto.update_red_pack_info", t_client, 13407)       
        end
    end
end

function red_package_mgr:get_red_package(red_package_guid,  role_guid, role_name)
   local  guid_ptr = hld.get_guid_64(role_guid)
   local  session = hld.get_client_session_by_guid(guid_ptr);
   if session == nil then
		return;
   end

   local target_red_obj = self:find_package_index(red_package_guid);
   if target_red_obj == nil then

   	   local t_cs_data = 
   	   {
   	       receiver_guid = role_guid,
   	       get_result = ConstDefine.e_red_bag_alread_done
   	   }

   	   MessageMgr:send_to_cs(session, "hld.ws2cs_proto.get_red_package_end", t_cs_data, 33014)
   	   return
	end
	if target_red_obj:get_remain_times() <= 0 then
	   --正常情况下，找不到这个红包应该是红包已经领完被删了，不用考虑传了个假GUID的情况
	   local t_cs_data = 
   	   {
   	       receiver_guid = role_guid,
   	       get_result = ConstDefine.e_red_bag_alread_done
   	   }

   	   MessageMgr:send_to_cs(session, "hld.ws2cs_proto.get_red_package_end", t_cs_data, 33014)
	
		return;
	end

	if target_red_obj:is_in_receiver_list(role_guid) then

	   local t_cs_data = 
   	   {
   	       receiver_guid = role_guid,
   	       red_package_guid = target_red_obj:get_guid();
   	       get_result = ConstDefine.e_red_bag_cant_draw_again
   	   }

   	    MessageMgr:send_to_cs(session, "hld.ws2cs_proto.get_red_package_end", t_cs_data, 33014)
		return;
	end

	local draw_num = target_red_obj:draw_red_package(role_guid, role_name);
	if draw_num > 0 then

		--send_info_to_all(target_index);
		--不再全发了，全发的话，会发生当已抽取人较多的时候，包量较大的问题
	   self:send_new_draw_info(target_red_obj, role_guid, role_name, draw_num);

	   local t_cs_data = 
   	   {
   	       receiver_guid = role_guid,
   	       red_package_guid = target_red_obj:get_guid(),
   	       get_result = ConstDefine.e_red_bag_get_success,
   	       money_type = target_red_obj:get_money_type(),
   	       money_num = draw_num
   	   }

   	   MessageMgr:send_to_cs(session, "hld.ws2cs_proto.get_red_package_end", t_cs_data, 33014)	
	end
end


function red_package_mgr:send_new_draw_info(red_obj, role_guid, role_name, money_num)
	  if red_obj == nil then
	  	 return
	  end

	  local t_client_data = {}
	  local r_a, r_b = red_obj:get_guid_A_B()
	  t_client_data.red_package_guid = { r_a, r_b}
	  t_client_data.remain_money_num = red_obj:get_remain_num()
	  t_client_data.remain_get_times = red_obj:get_remain_times()

	  local guid_ptr = hld.get_guid_64(role_guid)
	  t_client_data.receiver_guid = { guid_ptr:get_A(), guid_ptr:get_B()}
	  t_client_data.receiver_name = role_name
	  t_client_data.draw_num = money_num



	  MessageMgr:send_message_to_all_client("hld.character_proto.update_new_draw_info", t_client_data, 13409)
end


function red_package_mgr:change_player_name_func(role_guid, role_name)
    for k, v in pairs(self.m_red_package_arr) do
    	v:change_player_name_func(role_guid, role_name)
    end
end




function red_package_mgr:add_info_to_msg(t_all, red_obj)
	 local t_single = red_obj:get_client_msg_data()
	 table.insert(t_all.info_list, t_single)
end

function red_package_mgr:get_single_data_and_len(red_obj)
	 local t_single = red_obj:get_client_msg_data()

	 local s_msg = encode("hld.character_proto.red_pack_info", t_single)

	 delog("-string.len(s_msg)-----",string.len(s_msg))

	 local  len = string.len(s_msg) + 3   --- 填充数据是会多几个字节

	 return t_single, len
end



function red_package_mgr:send_info_to_one(target_guid)
    delog("-----send--client --red_package_mgr:send_info_to_one--", target_guid)
    local  guid_ptr = hld.get_guid_64(target_guid)
	local  target_session = hld.get_client_session_by_guid(guid_ptr);
	if target_session == nil then
		return;
	end

	local t_client_data = { info_list = {} }

	local t_single = nil
	local single_len = 0
	local total_len = 0

	for k, red_obj  in pairs(self.m_red_package_arr) do
		repeat

		local is_add = false
		local red_pack_template_ptr = red_obj:get_template_ptr()
		if red_pack_template_ptr == nil then

		    break
		end
		delog("---red_pack_template_ptr---", red_pack_template_ptr.Type)
		if red_pack_template_ptr.Type == hld.e_red_package_type_globel or
		   red_pack_template_ptr.Type == hld.e_red_package_type_vip  then
		   t_single, single_len = self:get_single_data_and_len(red_obj)


		   is_add = true
		elseif red_pack_template_ptr.Type == hld.e_red_package_type_legion then

			local sender_guid = red_obj:get_sender_guid();
			local sender_guid_ptr = hld.get_guid_64(sender_guid)
			local temp_legion = hld.legion_ws_mgr:get_instance():get_unit_legion(sender_guid_ptr);
			if (temp_legion ~=  nil and temp_legion:is_in_this_legion(guid_ptr) == true) then
				
                t_single, single_len = self:get_single_data_and_len(red_obj)
                is_add = true
			end 
        end
        if is_add == false then
        	break
        end
        if (total_len + single_len) > packege_length_max then
   
           MessageMgr:send_to_client(target_session, "hld.character_proto.update_red_pack_info", t_client_data, 13407)
           t_client_data.info_list = {} 
           total_len = 0
        end
        table.insert(t_client_data.info_list, t_single)
        total_len = total_len + single_len
		until true
	end
	if #t_client_data.info_list > 0 then
		self:test_send_length(t_client_data)
        MessageMgr:send_to_client(target_session, "hld.character_proto.update_red_pack_info", t_client_data, 13407)
	end
end

function red_package_mgr:test_send_length(t_client_data)
	local ss = encode("hld.character_proto.update_red_pack_info", t_client_data)

	delog("------client-data-length--", string.len(ss))
end


function red_package_send_info_to_one(target_guid)
	 red_package_mgr:send_info_to_one(target_guid)
end


function red_package_change_player_name(role_guid, role_name)
	 red_package_mgr:change_player_name_func(role_guid, role_name)
end





return red_package_mgr