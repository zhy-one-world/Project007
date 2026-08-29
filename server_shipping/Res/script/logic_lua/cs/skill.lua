local skill = {}


function skill:init()
	-- body
end

define_C[10609] = function (message_data, player_ref)
	local msg_data = decode("hld.skill_proto.change_skill_index", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	skill:skill_use_message(player_ref, msg_data)
end

define_C[10611] = function (message_data, player_ref)
	local msg_data = decode("hld.skill_proto.skill_spell", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	skill:skill_spell_message(player_ref, msg_data)
end

define_C[10613] = function (message_data, player_ref)
	local msg_data = decode("hld.skill_proto.skill_cast", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	skill:skill_cast_message(player_ref, msg_data)

end

define_C[10616] = function (message_data, player_ref)
	local msg_data = decode("hld.skill_proto.skill_hurt", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	skill:skill_hurt_message(player_ref, msg_data)
end



function skill:skill_use_message(player_ref, msg_data)
	delog("---skilll")
	hld.skill_manager:get_instance():skill_change_msg(player_ref:get_array_index(), msg_data.skill_template_id, msg_data.pos_index);
end


function skill:skill_hurt_message(player_ref, msg_data)
	delog("-------skill_hurt----------------")
    if #msg_data.postion < 3 then
		return;
	end
	local  target_guid_ptr = hld.get_guid_64(msg_data.target_id)
	
	local  target_unit_idf_ptr = hld.s_unit_identifier:new_local(target_guid_ptr, msg_data.target_runtime_id);

	local  param_ptr = hld.use_skill_param:new_local();

	param_ptr.sender_unit_idf = player_ref:get_identifier();
	param_ptr.target_unit_idf = target_unit_idf_ptr;
	param_ptr.skill_template_id = msg_data.skill_template_id;
	param_ptr.effect_index = msg_data.effect_index;
	param_ptr.skill_order = msg_data.skill_order;
	param_ptr.be_hit = msg_data.be_hit;
	param_ptr.be_critical = msg_data.be_critical;
	param_ptr.hit_random =  msg_data.hit_random;
	param_ptr.pos.x = msg_data.postion[1];
	param_ptr.pos.y = msg_data.postion[2];
	param_ptr.pos.z = msg_data.postion[3];

	hld.skill_manager:get_instance():skill_hurt(param_ptr);
end

function skill:skill_cast_message(player_ref, msg_data)

	delog("-------------------------cast-------------------")

    local target_guid_ptr = hld.get_guid_64(msg_data.target_id)
    local target_unit_idf_ptr = hld.s_unit_identifier:new_local(target_guid_ptr, msg_data.target_runtime_id);
	local  param_ptr = hld.use_skill_param:new_local();
	param_ptr.sender_unit_idf = player_ref:get_identifier();
	param_ptr.target_unit_idf = target_unit_idf_ptr;
	param_ptr.skill_template_id = msg_data.skill_template_id;
	param_ptr.pos = player_ref:get_new_map_pos().unit_location;
	param_ptr.yaw = msg_data.yaw;
	param_ptr.skill_order = msg_data.skill_order;
	hld.skill_manager:get_instance():skill_cast(param_ptr);

end

function skill:skill_spell_message(player_ref, msg_data)
    --delog(msg_data)
    --local guid_ptr =        hld.get_guid_64(item_guid)

    delog("--------------target_id---", type(msg_data.target_id))

	local target_guid_ptr = hld.get_guid_64(msg_data.target_id)

	--delog("--------------target_id111---", msg_data.target_id)
    local target_unit_idf_ptr = hld.s_unit_identifier:new_local(target_guid_ptr, msg_data.target_runtime_id);
	local  param_ptr = hld.use_skill_param:new_local();

	param_ptr.sender_unit_idf = player_ref:get_identifier();
	param_ptr.target_unit_idf = target_unit_idf_ptr;
	param_ptr.skill_template_id = msg_data.skill_template_id;
	param_ptr.pos = player_ref:get_new_map_pos().unit_location;
	param_ptr.yaw = msg_data.yaw;
	param_ptr.skill_order = msg_data.skill_order;
	hld.skill_manager:get_instance():skill_spell(param_ptr);
end

g_skill = skill


return skill