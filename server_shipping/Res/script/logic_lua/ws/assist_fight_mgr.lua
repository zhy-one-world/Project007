local assist_fight_mgr = RequireSingleton("assist_fight_mgr")


local	e_assist_fight_type_legion = 0
local	e_assist_fight_type_server = 1
local	e_assist_fight_type_robot = 2


local   assist_fight_max_num = 5;					--助战最大人数
local   packege_length_max = 11500

local    max_team_member_num = 3;	--一个组队中成员的最大数量
local    e_team_error_team_member_is_full = 16

--[[
33816 5

]]



function assist_fight_mgr:init()
	 if is_init() then
	 	return
	 end
	 ----------------------------

	 self.m_assist_fight_list = {}

end


function assist_fight_mgr:Update( )
	
end

function assist_fight_mgr:set_info(my_info, m_in_info)
    my_info.asssit_fight_guid = m_in_info.asssit_fight_guid;
	my_info.role_name = m_in_info.role_name
	my_info.role_lv = m_in_info.role_lv;
	my_info.role_professional = m_in_info.role_professional;
	my_info.role_head_icon = m_in_info.role_head_icon;
	my_info.assist_fight_type = m_in_info.assist_fight_type;
	my_info.boss_id = m_in_info.boss_id;
	my_info.boss_guid = m_in_info.boss_guid;
	my_info.map_id = m_in_info.map_id;
	my_info.map_guid = m_in_info.map_guid;
	my_info.unit_location = deep_copy(m_in_info.unit_location)
	my_info.unit_rotation = deep_copy(m_in_info.unit_rotation)
	my_info.assist_fight_tag = m_in_info.assist_fight_tag;
	my_info.legion_guid = m_in_info.legion_guid;
end

function assist_fight_mgr:remove_assist_list(my_info, role_guid)
	local assist_guid_list = my_info.assist_guid_list
	for i=1,assist_fight_max_num do
		if assist_guid_list[i] == role_guid then
			assist_guid_list[i] = 0
		end
	end
end

function assist_fight_mgr:insert_assist_list(my_info,role_guid)
	local assist_guid_list = my_info.assist_guid_list
	for i=1,assist_fight_max_num do
		if  assist_guid_list[i] ~= 0 and assist_guid_list[i] == role_guid then
			return true
		end
	end
	for i=1,assist_fight_max_num do
		if  assist_guid_list[i] == 0 then
			assist_guid_list[i] = role_guid
			return true
		end
	end
	return false
end


function assist_fight_mgr:start_assist_fight(m_in_info)
	local asssit_fight_guid = m_in_info.asssit_fight_guid

	if self.m_assist_fight_list[asssit_fight_guid] ~= nil then
		 local  m_old_info = self.m_assist_fight_list[asssit_fight_guid];
		 self:set_info(m_old_info, m_in_info) 
		 m_old_info.start_assist_time = os.time();

	else
	
		local  m_info = deep_copy(m_in_info);
		m_info.start_assist_time = os.time();
		self.m_assist_fight_list[asssit_fight_guid] = m_info
	
		--第一次发送提示公告到军团频道
		if m_in_info.assist_fight_type == e_assist_fight_type_legion then
			
			self:send_create_assist_fight_notic(m_in_info);
		end	
	end
	if m_in_info.assist_fight_type == e_assist_fight_type_legion then
		
		self:broadcast_to_legion(m_in_info.legion_guid, m_in_info.asssit_fight_guid);
	
	elseif m_in_info.assist_fight_type == e_assist_fight_type_server then
		
	    self:broadcast_to_server();
	end			
end


function assist_fight_mgr:cancel_assist_fight(cancel_type,assist_fight_guid, role_guid)
	local  normal_fight_guid = 0
	if cancel_type == 0 then --被助战者自己取消
		
		if self.m_assist_fight_list[role_guid] == nil then
			return;
		end
	
		self.m_assist_fight_list[assist_fight_guid] = nil 	
	elseif cancel_type == 1 then --助战者自己取消

		local guid_ptr = hld.get_guid_64(role_guid)
		
		local role_session = hld.get_client_session_by_guid(guid_ptr);
		if  role_session ~= nil then
			
				role_session:set_m_is_in_assist(false);
				role_session:set_assit_fight_guid(0);
		end

		if self.m_assist_fight_list[assist_fight_guid]== nil then
			return;
		end
		local m_info = m_assist_fight_list[assist_fight_guid];
		self:remove_assist_list(m_info, role_guid);
	end
	self:sync_assist_fight_info(role_guid);
end

function assist_fight_mgr:sync_assist_fight_info(role_guid, in_is_login, in_create_guid)
	local guid_ptr = hld.get_guid_64(role_guid)
		
	local role_session = hld.get_client_session_by_guid(guid_ptr);

	if role_session == nil then
		return;
	end

	local t_client_msg = {}

	t_client_msg.is_finish = false
	t_client_msg.assist_fight_list = {}

	local total_len = 10
	local single_len = 0


	for k,v in pairs(self.m_assist_fight_list) do

		repeat
		local  m_info_data = v;
		if m_info_data.assist_fight_type == e_assist_fight_type_legion then
			if role_session:get_legion_guid():get_AB() ~= m_info_data.legion_guid then
				break;
			end
		end
		local m_info_msg = {}

		local asssit_fight_guid_ptr = hld.get_guid_64(m_info_data.asssit_fight_guid)

		m_info_msg.assist_fight_guid_a = asssit_fight_guid_ptr:get_A()
		m_info_msg.assist_fight_guid_b = asssit_fight_guid_ptr:get_B()
		m_info_msg.assist_fight_type = m_info_data.assist_fight_type
		m_info_msg.role_name = m_info_data.role_name
		m_info_msg.role_lv = m_info_data.role_lv
		m_info_msg.role_professional = m_info_data.role_professional
		m_info_msg.head_icon = m_info_data.role_head_icon
		m_info_msg.boss_id = m_info_data.boss_id
		m_info_msg.map_id = m_info_data.map_id
		m_info_msg.start_assist_time = m_info_data.start_assist_time

		local s_single_buffer = encode("hld.assist_fight_proto.assist_fight_info", m_info_msg)
		single_len = string.len(s_single_buffer) + 3

		if (total_len + single_len) > packege_length_max then
  
           MessageMgr:send_to_client(role_session, "hld.assist_fight_proto.sycn_assist_fight_info", t_client_msg, 15603)
           t_client_msg.assist_fight_list = {} 
           total_len = 10
        end
        table.insert(t_client_msg.assist_fight_list, m_info_msg)
        total_len = total_len + single_len

		until true
	end
	local create_guid =  in_create_guid or 0
	local is_login = in_is_login or false
	if  create_guid ~= 0  and  create_guid == role_guid then	
		t_client_msg.is_sync = false
	else
		t_client_msg.is_sync = true
	end
	t_client_msg.is_finish  = true
	t_client_msg.is_login = is_login
	MessageMgr:send_to_client(role_session, "hld.assist_fight_proto.sycn_assist_fight_info", t_client_msg, 15603)
end


function assist_fight_mgr:broadcast_to_server( )
	  local t_client_msg = {}
	  t_client_msg.assist_fight_list = {}

	  for k,v in pairs(self.m_assist_fight_list) do

	    local m_info_data = v
	
		local m_info_msg = {}

		local asssit_fight_guid_ptr = hld.get_guid_64(m_info_data.asssit_fight_guid)

		m_info_msg.assist_fight_guid_a = asssit_fight_guid_ptr:get_A()
		m_info_msg.assist_fight_guid_b = asssit_fight_guid_ptr:get_B()
		m_info_msg.assist_fight_type = m_info_data.assist_fight_type
		m_info_msg.role_name = m_info_data.role_name
		m_info_msg.role_lv = m_info_data.role_lv
		m_info_msg.role_professional = m_info_data.role_professional
		m_info_msg.boss_id = m_info_data.boss_id
		m_info_msg.map_id = m_info_data.map_id
		table.insert(t_client_msg.assist_fight_list, m_info_msg)
	end

	MessageMgr:send_message_to_all_client("hld.assist_fight_proto.sycn_assist_fight_info", t_client_msg, 15603)
end

function assist_fight_mgr:broadcast_to_legion(legion_guid, in_create_guid)
	 local create_guid = in_create_guid or 0
	 local legion_guid_ptr = hld.get_guid_64(legion_guid)

	 local m_legion = hld.legion_ws_mgr:get_instance():get_legion(legion_guid_ptr)
	 if m_legion == nil then
		return;
	 end

	 local s_guid_array = m_legion:get_all_member_guid()
	 local t_guid_array  = Common:StrToTable(s_guid_array)

	 for i=1, #t_guid_array do
	 	 if t_guid_array[i] ~= 0 then
            self:sync_assist_fight_info(t_guid_array[i], false, create_guid);
	 	 end
	 end
end

function assist_fight_mgr:sync_assist_fight_info_to_cs(role_guid, assist_fight_guid)
	local  guid_ptr = hld.get_guid_64(role_guid)
	local  helper_session = hld.get_client_session_by_guid(guid_ptr);
	if helper_session ~=  nil then
		if self.m_assist_fight_list[assist_fight_guid] == nil then
			return;
		end
		local team_ws_ptr = hld.team_ws_mgr:get_instance():get_team(helper_session:get_team_guid());
		if team_ws_ptr ~= nil then
			team_ws_ptr:del_member(guid_ptr);
		end

		local t_cs_msg = {}
		t_cs_msg.role_guid = role_guid
		t_cs_msg.m_assist_fight_info = deep_copy(self.m_assist_fight_list[assist_fight_guid])

		MessageMgr:send_to_cs(helper_session, "hld.ws2cs_proto.sync_assist_fight_info", t_cs_msg, 33816)
	end
end

function assist_fight_mgr:req_assist_fight(role_guid, assist_fight_guid)
	local  guid_ptr = hld.get_guid_64(role_guid)
	local role_session = hld.get_client_session_by_guid(guid_ptr);

	if role_session  == nil then
		return;
	end
	if role_guid == assist_fight_guid then
		
		role_session:send_notice("90204183");
		return;
	end
	if self.m_assist_fight_list[role_guid] ~= nil then
		
		role_session:send_notice("90204328");
		return;
	end
	if self.m_assist_fight_list[assist_fight_guid] == nil then
		return;
	end

	local m_info = self.m_assist_fight_list[assist_fight_guid];
	if m_info.asssit_fight_guid == 0 then
		return;
	end
	if role_session:get_m_is_in_assist() then 	
		return;
	end
	--检查玩家地图类型
	local map_template_ptr = template_manager:get_line("MapTemplate", hld.cs_map_system:get_map_template_id(role_session:get_map_guid()))
	if  map_template_ptr == nil then
		return;
	end
	if (map_template_ptr.Type ~= hld.e_map_type_boss_home and	--地宫boss
		map_template_ptr.Type ~= hld.e_map_type_boss_island and		--天空岛
		map_template_ptr.Type ~= hld.e_map_type_boss_vip_home and	--boss之家
		map_template_ptr.Type ~= hld.e_map_type_belief_cloister and	--信仰回廊
		map_template_ptr.Type ~= hld.e_map_type_field and	--上古遗迹
		map_template_ptr.Type ~= hld.e_map_type_big_map) then	 --大世界
		
		role_session:send_notice("90204165");
		return;
	end
	if m_info.assist_fight_tag == 1 then
		local asssit_fight_guid_ptr = hld.get_guid_64(asssit_fight_guid)
		local main_session = hld.get_client_session_by_guid(asssit_fight_guid_ptr);
		if  main_session == nil then
			
			return;
		end
		if not main_session:get_team_guid():is_valid() then
			return;
		end
		local  m_team = hld.team_ws_mgr:get_instance():get_team(main_session:get_team_guid());
		if m_team == nil then
			return;
		end
		if m_team:get_member_num() >= max_team_member_num then

			local t_client_msg  = 
			{
			   error_type = e_team_error_team_member_is_full

			}
			MessageMgr:send_to_client(role_session, "hld.team_proto.team_error", t_client_msg, 11731)
			return;
		end
	end

	local  m_check_result =  self:check_can_assist_fight(role_guid,m_info.assist_fight_tag, m_info.map_id, m_info.map_guid);
	if  not m_check_result then
		return;
	end
	if m_info.assist_fight_tag == 0 then
		
		if not self:insert_assist_list(m_info,role_guid) then
		
			role_session:send_notice("90204164");
			return;
		end
	end
	--向cs的mgr同步自己的助战信息
	self:sync_assist_fight_info_to_cs(role_guid, assist_fight_guid);

	if m_info.assist_fight_tag == 0 then
		role_session:set_m_is_in_assist(true);
		role_session:set_assit_fight_guid(assist_fight_guid);
	end
end

function assist_fight_mgr:check_can_assist_fight(role_guid, assist_fight_tag, map_template_id, map_guid)
	local guid_ptr = hld.get_guid_64(role_guid)
	local role_session = hld.get_client_session_by_guid(guid_ptr);

	if role_session == nil then
		return false; 
	end

	if assist_fight_tag == 0 then
		
		local map_template_ptr = template_manager:get_line("MapTemplate", map_template_id);
		if map_template_ptr == nil then
			return false;
		end
		if role_session:get_role_info_data(hld.e_role_info_exp_level) < map_template_ptr.EnterLevel then
			return false;
		end
		local game_config_template_ptr = template_manager:get_line("GameConfigTemplate", 99000001);
		if  game_config_template_ptr == nil  then
			return false;
		end

		local  player_class = role_session:get_role_info_data(hld.e_role_info_class_type);
		if player_class < 1 or player_class > #game_config_template_ptr.InitRoleAry then
			return false;
		end

		local  player_init_template_id = game_config_template_ptr.InitRoleAry[player_class];
		local  player_cur_template_id = role_session:get_role_info_data(hld.e_role_info_template_id);
		if (player_cur_template_id - player_init_template_id < map_template_ptr.EnterGradeLevel) then
			return false;
		end

		if (map_template_ptr.Type == hld.e_map_type_boss_home or --地宫boss
			map_template_ptr.Type == hld.e_map_type_boss_island or --天空岛
			map_template_ptr.Type == hld.e_map_type_boss_vip_home or --boss之家
			map_template_ptr.Type == hld.e_map_type_belief_cloister or --信仰回廊
			map_template_ptr.Type == hld.e_map_type_field or  --上古遗迹   
			map_template_ptr.Type == hld.e_map_type_big_map) then
			
			return true;
			
		else
			role_session:send_notice("90204165");
			return false;
		end
	end
	return true;
end


function assist_fight_mgr:sync_assist_fight_info_to_cs_login(role_guid)
	local guid_ptr = hld.get_guid_64(role_guid)
	local role_session = hld.get_client_session_by_guid(guid_ptr)

	if role_session == nil then
		return;
	end
	if self.m_assist_fight_list[role_guid] ~= nil  then
		return;
	end
	if role_session:get_m_is_in_assist() then
		self:sync_assist_fight_info_to_cs(role_guid, role_session:get_assit_fight_guid())
	end
end

function assist_fight_mgr:check_assist_fight_legion_team(role_guid)
	local guid_ptr = hld.get_guid_64(role_guid)
	local role_session = hld.get_client_session_by_guid(guid_ptr)
	if role_session == nil then
		return;
	end
	if self.m_assist_fight_list[role_guid] == nil then
		return;
	end

	local map_temp_ptr = template_manager:get_line("MapTemplate", hld.cs_map_system:get_map_template_id(role_session:get_map_guid()))

	if map_temp_ptr ~= nil then	
		if  map_temp_ptr.Order == hld.e_map_order_type_multiplayer_raid then
			self:cancel_assist_fight(0, role_guid, role_guid);
		end
    end
end

function assist_fight_mgr:on_player_logout(role_guid)
	local guid_ptr = hld.get_guid_64(role_guid)
	local role_session = hld.get_client_session_by_guid(guid_ptr)

	if role_session == nil then
		return;
	end
	if self.m_assist_fight_list[role_guid] ~= nil then
		
		local m_assist_info = self.m_assist_fight_list[role_guid];
		if m_assist_info.assist_fight_tag == 1 then	
			self:cancel_assist_fight(0, role_guid, role_guid);
			self:broadcast_to_legion(role_session:get_legion_guid())
		end
	end
end

function assist_fight_mgr:send_create_assist_fight_notic(m_in_info)
    local asssit_fight_guid_ptr = hld.get_guid_64(m_in_info.asssit_fight_guid)
	local session = hld.get_client_session_by_guid(asssit_fight_guid_ptr)
	if session == nil then
		return;
	end
	--跨服状态下不允许
	if false == session:is_self_server() then
		return;
	end
	local legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid())
	if  legion_ws_ptr == nil then
		return;
	end
	local _npc_template_ptr = template_manager:get_line("NpcTemplate", m_in_info.boss_id)
	if  _npc_template_ptr == nil then
		return;
	end
	local notice_id = 93000428;
	local t_notice_id = template_manager:get_str_id_by_notice_id(notice_id);
	local content_text_arr = { tostring(t_notice_id[1]),  session:get_role_name(),  tostring(_npc_template_ptr.NpcName) }
	local param_arr_ptr = hld.event_ws_mgr:get_instance():get_free_param()

	delog(content_text_arr)

    local content_text = table.concat( content_text_arr, "&#")

	hld.event_ws_mgr:get_instance():send_notice_with_param(asssit_fight_guid_ptr, "", content_text, param_arr_ptr, hld.e_notice_param_open_widget, hld.e_chat_type_legion,0,0);
end













function assist_fight_mgr_start_assist_fight(s_buffer)
	local msg_data = decode("hld.st_proto.st_assist_fight_info", s_buffer)
	if msg_data == nil then
		return
	end
	assist_fight_mgr:start_assist_fight(msg_data)
end

function assist_fight_mgr_cancel_assist_fight(cancel_type,assist_fight_guid,role_guid)
     assist_fight_mgr:cancel_assist_fight(cancel_type, assist_fight_guid, role_guid)
end

function assist_fight_mgr_sync_assist_fight_info(role_guid, is_login, create_guid)
	 assist_fight_mgr:sync_assist_fight_info(role_guid, is_login, create_guid)
end

function assist_fight_mgr_req_assist_fight(role_guid, assist_fight_guid)
	 assist_fight_mgr:req_assist_fight(role_guid, assist_fight_guid)
end

function assist_fight_mgr_sync_assist_fight_info_to_cs_login(role_guid)
	 assist_fight_mgr:sync_assist_fight_info_to_cs_login(role_guid)
end

function assist_fight_mgr_check_assist_fight_legion_team(role_guid)
	 assist_fight_mgr:check_assist_fight_legion_team(role_guid)
end

function assist_fight_mgr_on_player_logout(role_guid)
	assist_fight_mgr:on_player_logout(role_guid)
end

return assist_fight_mgr