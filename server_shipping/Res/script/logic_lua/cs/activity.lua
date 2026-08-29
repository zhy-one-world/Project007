local activity = {}


function activity:init( )
	-- body
end



define_C[14705] = function (message_data, player_ref)
    local msg_data = decode("hld.time_limit_activity_proto.time_limit_activity_send_get_award", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	activity:c2cs_time_limit_get_act_reward(player_ref, msg_data)
end


define_C[14709] = function (message_data, player_ref)
	local msg_data = decode("hld.time_limit_activity_proto.add_activity_schedule", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	activity:add_activity_schedule(player_ref, msg_data)
end


function activity:c2cs_time_limit_get_act_reward(player_ref, msg_data)
	 delog(msg_data)

	 local reward_template_ptr = hld.time_limit_activity_temp_mgr:get_instance():get_time_limit_act_branch_with_id(msg_data.activity_id);
	 if reward_template_ptr.ActGuid <= 0 then	
		 return;
	 end
	 if msg_data.act_guid <= 0 then
		
			return;
	 end

	 delog("----------", reward_template_ptr.ActivityType)
		--小额充值和启示之路在服务器直接获得奖励 不需要从客户端请求奖励
	if (hld.e_time_limit_activity_type_small_charge == reward_template_ptr.ActivityType 
			or hld.e_time_limit_activity_type_behavior_welfare == reward_template_ptr.ActivityType
			or hld.e_time_limit_activity_type_behavior_welfare1 == reward_template_ptr.ActivityType
			or hld.e_time_limit_activity_type_behavior_welfare2 == reward_template_ptr.ActivityType
			or hld.e_time_limit_activity_type_behavior_welfare3 == reward_template_ptr.ActivityType
			or hld.e_time_limit_activity_type_daily_gift_charge == reward_template_ptr.ActivityType
			or hld.e_time_limit_activity_type_new_behavior_welfare == reward_template_ptr.ActivityType) then
		    delog("---type-----error----",reward_template_ptr.ActivityType)
			return;
	end

	player_ref:get_time_limit_activity_mgr():apply_award(msg_data.activity_id, msg_data.act_guid, msg_data.param_int_1);
end

function activity:add_activity_schedule(player_ref, msg_data)

    delog("---add_activity_schedule---")
    delog(msg_data)

	--这个功能暂时只用于记录玩家点击转盘
	if (hld.e_time_limit_activity_type_treasure == msg_data.add_type
		or hld.e_time_limit_activity_type_treasure_royal1 == msg_data.add_type
		or hld.e_time_limit_activity_type_treasure_royal2 == msg_data.add_type
	    or hld.e_time_limit_activity_type_treasure_royal3 == msg_data.add_type
		or hld.e_time_limit_activity_type_treasure_sprite == msg_data.add_type
		or hld.e_time_limit_activity_type_treasure_element == msg_data.add_type) then

		player_ref:get_time_limit_activity_mgr():add_activity_schedule(msg_data.add_type, msg_data.add_value, msg_data.schedule_index, 0);
	end
end

g_activity = activity

return activity

