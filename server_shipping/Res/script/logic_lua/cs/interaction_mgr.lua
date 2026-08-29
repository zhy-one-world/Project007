local interaction_mgr={}
function interaction_mgr:init()
end

define_C[14001] = function (message_data, player_ref)
	local msg_data = decode("hld.interaction_proto.c_s_interaction_invite", message_data)
	if message_data == nil then
		return 
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	interaction_mgr:c2s_interaction_invite(player_ref, msg_data)

end

define_C[14003] = function (message_data, player_ref)
	local msg_data = decode("hld.interaction_proto.c_s_interaction_invite_reply", message_data)
	if msg_data == nil then
		return 
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	interaction_mgr:c2s_interaction_invite_reply(player_ref, msg_data)
end

define_C[14005] = function (message_data, player_ref)
	local msg_data = decode("hld.interaction_proto.c_s_real_start_interaction", message_data)
	if message_data == nil then
		return 
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	interaction_mgr:c_s_real_start_interaction(player_ref, msg_data)
end

define_C[14007] = function(message_data, player_ref)
	local msg_data = decode("hld.interaction_proto.c_s_stop_interaction", message_data);
	if msg_data == nil then
		return
	end
	interaction_mgr:c2s_stop_interaction(player_ref);
end

function interaction_mgr:c2s_stop_interaction(player_ref)
	if nil == player_ref then
		return;
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	player_ref:get_interaction_mgr():stop_interaction();
end

function interaction_mgr:c2s_interaction_invite(player_ref ,msg_data)
	local be_guid_ptr = hld.get_guid_64(msg_data.be_invited_guid)
	delog("-----c2s_interaction_invite---11111-") 

	local be_invite_player_ref = hld.unit_man():get_player(be_guid_ptr);
	if  be_invite_player_ref:is_valid() == false  or be_invite_player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	delog("-----c2s_interaction_invite---2222-") 

	be_invite_player_ref:get_interaction_mgr():recv_interaction_invite(player_ref:get_unit_guid(), msg_data.interaction_type);
end


function interaction_mgr:c2s_interaction_invite_reply(player_ref, msg_data)
	 local  interaction_mgr_cpp =  player_ref:get_interaction_mgr()
	 if interaction_mgr_cpp == nil then
	 	return;
	 end
	 delog("------c2s_interaction_invite_reply--------")
	 delog(msg_data)

	 local invite_guid_ptr = hld.get_guid_64(msg_data.invite_guid)
	 interaction_mgr_cpp:recv_interaction_answer(invite_guid_ptr, msg_data.interaction_type, msg_data.reply_result);
end


function interaction_mgr:c_s_real_start_interaction(player_ref, msg_data)
	 local n64 = player_ref:get_unit_guid():get_AB();
	 if n64 ~= msg_data.invite_guid and n64 ~= msg_data.be_invited_guid then
	     return
	 end 
	 local invite_guid_ptr = hld.get_guid_64(msg_data.invite_guid)
	 local invite_ref = hld.unit_man():get_player(invite_guid_ptr);
	 if false == invite_ref:is_valid() or invite_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		 return;
	 end
	 local be_invited_guid_ptr = hld.get_guid_64(msg_data.be_invited_guid)
	 invite_ref:get_interaction_mgr():recv_real_start_interaction(be_invited_guid_ptr, msg_data.interaction_type);
end



return interaction_mgr



