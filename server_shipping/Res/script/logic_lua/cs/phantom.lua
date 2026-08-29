local phantom = {}


local phantom_error_sucess = 0;
local phantom_error_id_not_exist = -1;
local phantom_error_not_finish = -2;
local phantom_error_is_active = -3;

local e_phantom_inst_data = 
{
	config_id = 0,
	state = 1,
	item_1 = 2,
	item_2 = 3,
	item_3 = 4,
	item_4 = 5,
	item_5 = 6,
	item_6 = 7,
	item_7 = 8,
	item_8 = 9,
	item_9 = 10,
	item_10 = 11,
	max = 12
};


function phantom:init()
	
end


define_C[11632] = function(message_data, player_ref)
    local msg_data = decode("hld.phantom_proto.phantom_operate", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	phantom:oper_active(player_ref, msg_data.phantom_id)
end


function phantom:oper_active(player_ref, phantom_id)
	local phantom_mgr = player_ref:get_phantom_mgr()
	if phantom_mgr == nil then
		return 
	end
	local phantom_obj = phantom_mgr:lua_find_object(phantom_id)
	if not phantom_obj:is_valid() then
		self:oper_msg(player_ref, phantom_id, phantom_error_id_not_exist)
		return
	end

	local class_type = player_ref:get_unit_info(hld.e_role_info_class_type)

	if not phantom_obj:is_finish(class_type) then
		self:oper_msg(player_ref, phantom_id, phantom_error_not_finish)
		return
	end
	if phantom_obj:get_inst_data(e_phantom_inst_data.state) == 1 then
	   self:oper_msg(player_ref, phantom_id, phantom_error_is_active)
	   return
	end

	phantom_obj:set_inst_data(e_phantom_inst_data.state, 1);


	local skill_id = phantom_obj:get_skill_id();

	phantom_mgr:active_skill(skill_id)

	self:sync_one_message_to_client(player_ref, phantom_obj)

	self:oper_msg(player_ref, phantom_id, phantom_error_sucess)
end


function phantom:oper_msg(player_ref, n_phantom_id, n_result)
	local msg_data = {
	    phantom_id = n_phantom_id,
	    oper_type = 0,
	    result = n_result
    }
    MessageMgr:send_message_to_self(player_ref, "hld.phantom_proto.phantom_operate_end",msg_data, 11634)

end

function phantom:sync_one_message_to_client(player_ref,  phantom_obj)
    local t_data = {}
	for i = e_phantom_inst_data.config_id ,e_phantom_inst_data.item_10 do
		local value = phantom_obj:get_inst_data(i)
		table.insert(t_data, value)
	end
	local msg_data = {
	     phantom_info = t_data
    }
    MessageMgr:send_message_to_self(player_ref, "hld.phantom_proto.phantom_one",msg_data, 11630)
end

phantom_lua_mgr = phantom

return phantom

