local rename_mgr = RequireSingleton("rename_mgr")





local	e_change_failed_common_error = 0
local	e_change_role_success = 1
local 	e_change_failed_role_name_invalid = 2
local	e_change_failed_role_name_duplicate = 3
local	e_change_failed_role_name_size_too_short = 4
local	e_change_failed_role_name_size_too_long = 5
local	e_change_failed_role_item_not_enough = 6
local	e_change_role_name_available = 7



local  max_name_size = 100;								-- 角色名最大字符数
local  min_name_size = 2;								-- 角色名最小字符数
local  CHANGE_NAME_CARD_ID = 31000159


--[[

e_msgindex_ws2dp_check_player_name,
e_msgindex_dp2ws_check_player_name,
e_msgindex_ws2dp_change_player_name,
e_msgindex_dp2ws_change_player_name_end,
e_msgindex_ws2cs_change_player_name,
e_msgindex_ws2cs_sub_player_rename_item,
e_msgindex_cs2ws_sub_player_rename_item_end,

1385 5
1386 2
1387 5
1388 2
1389 
1390
1391




]]





function rename_mgr:init()
	if is_init() then
	 	return
	end
	self.m_want_use_names = {}
end

function rename_mgr:Updata(i_nTime)
	
end

function rename_mgr:clear_data()

end


function rename_mgr:add_want_use_name( name, role_guid)
	  self.m_want_use_names[name] = role_guid
end


function rename_mgr:del_want_use_name(name)
	 if self.m_want_use_names[name] == nil then
	 	return
	 end
	 self.m_want_use_names[name] = nil
end

function rename_mgr:is_in_want_use_name(name)
	 if self.m_want_use_names[name] ~= nil then
	 	return true
	 end
	 return false
end


function rename_mgr:get_name_in_want_use_name(role_guid)
	  for k,v in pairs(self.m_want_use_names) do
	  	  if role_guid == v then
	  	  	return k
	  	  end
	  end
	  return ""
end


function rename_mgr:check_name_is_avaliable(role_name, role_guid )
	local check_ret = e_change_role_success
	 if  hld.invalid_ansi_word:is_valid_ansi_str(role_name) == false then
		
		check_ret = e_change_failed_role_name_invalid
	 end

	 if hld.invalid_ansi_word:include_invalid_ansi_str(role_name)  then
		
		check_ret = e_change_failed_role_name_invalid;
	 end

	--判断角色名长度是否合法
	 if string.len(role_name) < min_name_size then
		
		check_ret = e_change_failed_role_name_size_too_short;
	 end
	 if string.len(role_name) > max_name_size then
		
		check_ret = e_change_failed_role_name_size_too_long;
	end

	if self:is_in_want_use_name(role_name) then
		delog("--ddd-role---guid---", role_guid)
		check_ret = e_change_failed_role_name_duplicate;
	end
	local want_name = self:get_name_in_want_use_name(role_guid)
	if string.len(want_name) > 0 then
		delog("--sss-role---guid---", role_guid)
		check_ret = e_change_failed_common_error;
	end

	return check_ret 
end

function rename_mgr:change_player_name(name, role_guid)
	delog("---role---guid---", role_guid)
	local guid_ptr = hld.get_guid_64(role_guid)

	local  player_session = hld.get_client_session_by_guid(guid_ptr);
	if  player_session == nil then
		return false;
	end


	local check_ret = self:check_name_is_avaliable(name, role_guid);

	if check_ret ~= e_change_role_success then
		local t_client_msg = {}
		t_client_msg.ret = check_ret 

		MessageMgr:send_to_client(player_session, "hld.character_proto.change_player_name_end", t_client_msg, 10511)

		return false;
	end


	local t_dp_msg = {}

	t_dp_msg.role_guid = role_guid
	t_dp_msg.role_name = name

	local server_id = player_session:get_role_info_data(hld.e_role_info_server_id)

	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_check_player_name", t_dp_msg, 1385, server_id)

	return true;

end


function rename_mgr:sub_rename_item_end(ret, role_guid)
     local want_name = ""
     local original_name = ""
	 local guid_ptr = hld.get_guid_64(role_guid)
	 local  player_session = hld.get_client_session_by_guid(guid_ptr);
	 if  player_session == nil then
		self:give_back_rename_card(role_guid);
		return want_name, original_name;
	 end
	 --self:give_back_rename_card(role_guid);
	 want_name = self:get_name_in_want_use_name(role_guid);
	
	 if string.len(want_name) == 0 then

		return  want_name, original_name;
	 end

	 if ret ~= e_change_role_success then
		
		self:del_want_use_name(want_name)

		local t_client_msg = 
		{
		   ret = ret
	    }

	    MessageMgr:send_to_client(player_session, "hld.character_proto.change_player_name_end", t_client_msg, 10511)
		return want_name, original_name;
	end

	local t_dp_msg = 
	{
	    role_guid = role_guid,
	    role_name = want_name
	}

	local server_id = player_session:get_role_info_data(hld.e_role_info_server_id)
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_change_player_name", t_dp_msg, 1387, server_id)


	original_name = player_session:get_role_name();
    local  char_role_name = string.sub(want_name, 1, max_name_size) 
	player_session:set_role_name(char_role_name);

	local t_cs_msg = 
	{
	    role_guid = role_guid,
	    role_name = want_name
	}
	MessageMgr:send_to_cs(player_session, "hld.ws2cs_proto.change_player_name", t_cs_msg, 1389)
	return want_name, original_name;
end


function rename_mgr:check_role_name_end(role_guid, role_name, result)
	local  check_ret = e_change_role_success;

	if result ~= e_change_role_success then
		self:del_want_use_name(role_name);
		check_ret = e_change_failed_role_name_duplicate;
	else
		check_ret = e_change_role_name_available;
	end

    local want_name = self:get_name_in_want_use_name(role_guid)
    if string.len(want_name) == 0 or check_ret ~= e_change_role_name_available then
   	    local t_client_msg = 
   	    {
   	       ret = check_ret

   	    }
		local guid_ptr = hld.get_guid_64(role_guid)
		local player_session = hld.get_client_session_by_guid(guid_ptr);
		if player_session ~= nil then
			MessageMgr:send_to_client(player_session, "hld.character_proto.change_player_name_end", t_client_msg, 10511)
		end
	else
        local t_cs_msg = 
        {
           role_guid = role_guid,
           item_templete_id = CHANGE_NAME_CARD_ID
        }
        local guid_ptr = hld.get_guid_64(role_guid)
		local player_session = hld.get_client_session_by_guid(guid_ptr);
		if  player_session == nil then	
			return;
		end
		MessageMgr:send_to_cs(player_session, "hld.ws2cs_proto.sub_player_rename_item", t_cs_msg, 1390)	
	end	
end


function rename_mgr:change_player_name_end(role_guid, role_name)
	self:del_want_use_name(role_name);
	local check_ret = e_change_role_success;
	local t_client_msg = 
	{
	   ret = check_ret
    }
	local guid_ptr = hld.get_guid_64(role_guid)
	local player_session = hld.get_client_session_by_guid(guid_ptr);
	if player_session ~= nil then
		MessageMgr:send_to_client(player_session, "hld.character_proto.change_player_name_end", t_client_msg, 10511)
	end
end

function rename_mgr:confirm_change_name(confirm_type, role_guid, role_name )
	local guid_ptr = hld.get_guid_64(role_guid)
	local player_session = hld.get_client_session_by_guid(guid_ptr);
	if player_session == nil then
		return
	end

	if not confirm_type then
		return
	end

	local check_ret = self:check_name_is_avaliable(role_name, role_guid)
	if check_ret ~= e_change_role_success then
		local t_client_msg =
		{
			ret = check_ret
		}
		MessageMgr:send_to_client(player_session, "hld.character_proto.change_player_name_end", t_client_msg, 10511)
		return
	end

	self:add_want_use_name(role_name, role_guid)

	local t_dp_msg = 
	{
	    role_guid = role_guid,
	    role_name = role_name
	}
	local server_id = player_session:get_role_info_data(hld.e_role_info_server_id)
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_check_player_name", t_dp_msg, 1385, server_id)
end


function rename_mgr:give_back_rename_card(role_guid)

 
	local	mail_title_char = "90303013";
	local	mail_content_char = "90303013";

	local  item_list_ptr = hld.mail_ws_mgr:get_instance():get_free_item_list()
	hld.mail_ws_mgr:get_instance():add_free_item(CHANGE_NAME_CARD_ID, 1)

	local guid_ptr = hld.get_guid_64(role_guid)
	
	hld.mail_ws_mgr:get_instance():send_mail_system(guid_ptr, 0, item_list_ptr, mail_title_char, mail_content_char, 0, 0,0,0);
end



function rename_change_player_name(name, role_guid)
	 return rename_mgr:change_player_name(name, role_guid)
end


function rename_sub_rename_item_end( ret,  role_guid)
	 return rename_mgr:sub_rename_item_end(ret, role_guid)
end


function rename_check_role_name_end( role_guid, role_name, result)
	  rename_mgr:check_role_name_end(role_guid, role_name, result)
end


function rename_change_player_name_end(role_guid, role_name)
	  rename_mgr:change_player_name_end(role_guid, role_name)
end

function rename_confirm_change_name( confirm_type, role_guid, role_name)
	  rename_mgr:confirm_change_name(confirm_type, role_guid, role_name)
end


return rename_mgr














