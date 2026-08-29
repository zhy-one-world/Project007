local dp_msgproc = {}

local define_DP = define_C


function dp_msgproc:init(  )
	delog("----dp_msgproc--init----")
end



define_DP[1409] = function(message_data, conn_index)
    --delog("-------load_role_starark------", string.len(message_data))
    local msg_data = decode("hld.dp2cs_proto.load_role_starark", message_data)
    if msg_data == nil then
     	 return
    end

    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local db_data = msg_data.db_data

	--delog(db_data)

    local s_buffer = encode("hld.db_proto.role_starark_db", db_data)

    --delog("----msg_data--------", string.len(s_buffer))

	local is_sucess = player_ref:get_starark_mgr():load_starark_by_proto_db(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----starark---error-----")
		return
	end
	delog("-----------load----starark---sucess-----")
	player_ref:set_loading_flag(e_data_flag_starark);
end

define_DP[1407] = function (message_data, conn_index )
    delog("-------load_role_skytreasure------", string.len(message_data))
	local msg_data = decode("hld.dp2cs_proto.load_role_skytreasure", message_data)
    if msg_data == nil then
     	 return
    end

    delog("--type-------", type(msg_data.db_data))

    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end
	local is_sucess = player_ref:get_skytreasure_mgr():load_skytreasure_by_db_lua(message_data, string.len(message_data));

	if (is_sucess ~= true) then
		print("-----------load----skytreasure---error-----")
		return
	end
	delog("-----------load----skytreasure---sucess-----")
	player_ref:set_loading_flag(e_data_flag_skytreasure);
end


define_DP[36600] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_recycle", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_recycle_mgr():load_recycle_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----recycle---error-----")
		return
	end
	delog("-----------load----recycle---sucess-----")
	player_ref:set_loading_flag(e_data_flag_recycle);
end

define_DP[36602] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_recycle_task", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer =  msg_data.db_data
	local is_sucess = player_ref:get_recycle_mgr():load_recycle_task_by_db_lua(s_buffer, string.len(s_buffer));
     
	if (is_sucess ~= true) then
		print("-----------load----recycle-task--error-----")
		return
	end
	delog("-----------load----recycle--task-sucess-----")
	player_ref:set_loading_flag(e_data_flag_recycle_task);
end

local player_mgr = RequireSingleton("player_mgr")

define_DP[1405] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

    local is_sucess = false
    local s_buffer = msg_data.db_data
	if not  g_dragontrip:is_use_lua() then


       is_sucess = player_ref:get_dragontrip_mgr():load_dragontrip_by_db_lua(s_buffer, string.len(s_buffer));

   else 

       	local oPlayer =  player_mgr:find_player(msg_data.unit_array_index)

        is_sucess = oPlayer:get_system("dragontrip"):load_db_data(s_buffer)

   end

	if (is_sucess ~= true) then
		print("-----------load----dragontrip---error-----")
		return
	end
	delog("-----------load----dragontrip---sucess-----")
	player_ref:set_loading_flag(e_data_flag_dragontrip);
end


define_DP[36400] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_phantom_mgr():load_phantom_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----phantom---error-----")
		return
	end
	delog("-----------load----phantom---sucess-----")
	player_ref:set_loading_flag(e_data_flag_phantom);
end

define_DP[33315] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_chat_mgr():load_chat_record_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----chat_record---error-----")
		return
	end
	delog("-----------load----chat_record--sucess-----")
	player_ref:set_loading_flag(e_data_flag_chat_record);
end

--------------------------------------------------------------------------------------------------------------------------------------------------



define_DP[1904] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_legion_skill():load_legion_skill_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----legion_skill---error-----")
		return
	end
	delog("-----------load--legion_skill--sucess-----")
	player_ref:set_loading_flag(e_data_flag_legion_skill);
end




define_DP[36604] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_recycle_mgr():load_recycle_invited_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----recycle_invited---error-----")
		return
	end
	delog("-----------load---recycle_invited--sucess-----")
	player_ref:set_loading_flag(e_data_flag_recycle_invited);
end

define_DP[1401] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_competition_mgr():load_role_competition_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess == 0) then
		print("-----------load----competition---error-----")
		return
	end
	
	if (is_sucess == 1) then
	 delog("-----------load--role_competition--sucess-----")
	 player_ref:set_loading_flag(e_data_flag_player_competition);
	end
	
	if (is_sucess == 2) then
	 delog("-----------load--element_competition--sucess-----")
	 player_ref:set_loading_flag(e_data_flag_element_competition);
	end
end


define_DP[1394] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_person_infor_mgr():load_role_person_infomation_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----infomation---error-----")
		return
	end
	delog("-----------load--infomation--sucess-----")
	player_ref:set_loading_flag(e_data_flag_player_information);
end


define_DP[36500] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_oracle_trial_mgr():load_oracle_trial_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----oracle_trial---error-----")
		return
	end
	delog("-----------load--oracle_trial--sucess-----")
	player_ref:set_loading_flag(e_data_flag_oracle_trial);
end

------------------------------------------------------------------------------------------------------------------------------------

define_DP[36007] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_server_harry_mgr():load_role_harry_record_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----role_harry_record---error-----")
		return
	end
	delog("-----------load--role_harry_record--sucess-----")
	player_ref:set_loading_flag(e_data_flag_harry_record);
end

define_DP[35900] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_seven_day_goal_mgr():load_seven_day_goal_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----seven_day_goal---error-----")
		return
	end
	delog("-----------load--seven_day_goal--sucess-----")
	player_ref:set_loading_flag(e_data_flag_seven_day_goal);
end

define_DP[35600] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_boss_island_mgr():load_boss_island_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----boss_island---error-----")
		return
	end
	delog("-----------load--boss_island-sucess-----")
	player_ref:set_loading_flag(e_data_flag_boss_island);
end

------------------------------------------------------------------------------------------------------------------------
define_DP[35401] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_time_limit_activity_mgr():load_data_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----time_limit_activity---error-----")
		return
	end
	delog("-----------load--time_limit_activity-sucess-----")
	player_ref:set_loading_flag(e_data_flag_time_limit_activity);
end

define_DP[35100] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_gain_treasure_mgr():load_role_gain_treasure_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----gain_treasure---error-----")
		return
	end
	delog("-----------load--gain_treasure-sucess-----")
	player_ref:set_loading_flag(e_data_flag_gain_treasure);
end

define_DP[1347] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:load_role_logic_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----logic---error-----")
		return
	end
	delog("-----------load--logic-sucess-----")
	player_ref:set_loading_flag(e_data_flag_logic_info);
end


define_DP[35000] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_service_rank_mgr():load_recevice_rewards_state_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----logic---error-----")
		return
	end
	delog("-----------load--logic-sucess-----")
	player_ref:set_loading_flag(e_data_flag_service_rank);
end


define_DP[33001] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_mail_mgr():load_had_globel_mail_guid_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----mail_had--error-----")
		return
	end
	delog("-----------load--mail_had-----")
	player_ref:set_loading_flag(e_data_flag_globel_mail_had);
end


define_DP[34708] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_auction_mgr():load_self_selling_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----self_selling--error-----")
		return
	end
	delog("-----------load--self_selling------")
	player_ref:set_loading_flag(e_data_flag_auction_self_selling);
end

---------------------------------------------------------------------------------------------------------------------------


define_DP[1382] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:load_history_highest_record_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----highest_record--error-----")
		return
	end
	delog("-----------load--highest_record------")
	player_ref:set_loading_flag(e_data_flag_highest_record);
end


define_DP[1383] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:load_daily_must_do_count_by_db_lua(msg_data.role_guid,s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----must_do_count--error-----")
		return
	end
	delog("-----------load--must_do_count------")
	player_ref:set_loading_flag(e_data_flag_daily_must_do_count);
end


define_DP[34400] = function (message_data, conn_index)

	local msg_data, ss = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
    	 print("---------fsdfsdfsd", ss)
     	 return
    end
    delog(msg_data)
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_talent_mgr():load_talent_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----talent--error-----")
		return
	end
	delog("-----------load--talent------")
	player_ref:set_loading_flag(e_data_flag_talent);
end

define_DP[34301] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_first_time_do_mgr():load_frist_time_do_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----frist_time_do--error-----")
		return
	end
	delog("-----------load--frist_time_do------")
	player_ref:set_loading_flag(e_data_flag_fist_time_do);
end


define_DP[34201] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_guide_mgr():load_guide_record_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----guide_record--error-----")
		return
	end
	delog("-----------load--guide_record_do------")
	player_ref:set_loading_flag(e_data_flag_guide);
end


define_DP[34100] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_title_mgr():load_title_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load----title--error-----")
		return
	end
	delog("-----------load-title_do------")
	player_ref:set_loading_flag(e_data_flag_title);
end


------------------------------------------------------------------------------------------------------------------------------------

define_DP[33602] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_welfare_mgr():load_active_degree_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---active_degree--error-----")
		return
	end
	delog("-----------load-active_degree------")
	player_ref:set_loading_flag(e_data_flag_active_degree);
end

define_DP[33600] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_welfare_mgr():load_welfare_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---welfare--error-----")
		return
	end
	delog("-----------load-welfare------")
	player_ref:set_loading_flag(e_data_flag_welfare);
end


define_DP[33400] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_convert_mgr():load_convert_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---convert--error-----")
		return
	end
	delog("-----------load-convert_------")
	player_ref:set_loading_flag(e_data_flag_convert);
end

define_DP[1357] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:load_yesterday_must_do_remain_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---yesterday_must_do_remain--error-----")
		return
	end
	delog("-----------load-yesterday_must_do_remain------")
	player_ref:set_loading_flag(e_data_flag_yesterday_must_do_remain);
end

define_DP[33106] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_worship_target():load_worship_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---worship--error-----")
		return
	end
	delog("-----------load-worship------")
	player_ref:set_loading_flag(e_data_flag_worship);
end

define_DP[33200] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_patron_saint_mgr():load_tinder_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---tinder-error-----")
		return
	end
	delog("-----------load-tinder------")
	player_ref:set_loading_flag(e_data_flag_tinder);
end
--------------------------------------------------------------------------------------------------------------------------



define_DP[32700] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_pokedex_mgr():load_pokedex_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---pokedex--error-----")
		return
	end
	delog("-----------load---pokedex------")
	player_ref:set_loading_flag(e_data_flag_pokedex);
end


define_DP[32600] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_achievement_mgr():load_achievement_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---achievement--error-----")
		return
	end
	delog("-----------load---achievement------")
	player_ref:set_loading_flag(e_data_flag_achievement);
end


define_DP[32500] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_special_name_mgr():load_data_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---special_name--error-----")
		return
	end
	delog("-----------load---special_name------")
	player_ref:set_loading_flag(e_data_flag_special_name);
end


define_DP[34600] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_service_goal_mgr():load_service_goal_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---service_goal--error-----")
		return
	end
	delog("-----------load---service_goal-----")
	player_ref:set_loading_flag(e_data_flag_service_goal);
end


define_DP[2400] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local player_unit_index = player_ref:get_array_index()

	local s_buffer = msg_data.db_data
	local is_sucess = hld.map_record_mgr:get_instance():load_map_record_by_db_lua(player_unit_index, s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---map-record--error-----")
		return
	end
	delog("-----------load---map-record-----")
	player_ref:set_loading_flag(e_data_flag_map);
end


define_DP[32300] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_goods_helper_mgr():load_purchase_record_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---goods_helper--error-----")
		return
	end
	delog("-----------load---goods_helper-----")
	player_ref:set_loading_flag(e_data_flag_goods);
end

----------------------------------------------------------------------------------------------------------------------

define_DP[2000] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_pawn_att():load_base_att_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---att--error-----")
		return
	end
	delog("-----------load---att_----")
	player_ref:set_loading_flag(e_data_flag_att);
end



define_DP[32200] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_mission_mgr():load_mission_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---att--error-----")
		return
	end
	delog("-----------load---att_----")
	player_ref:get_mission_mgr():refresh_missions();
	player_ref:set_loading_flag(e_data_flag_mission);
end

define_DP[32100] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_belief_mgr():load_belief_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---belief--error-----")
		return
	end
	delog("-----------load---belief----")
	player_ref:set_loading_flag(e_data_flag_belief);
end

define_DP[1343] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:load_money_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---money--error-----")
		return
	end
	delog("-----------load---money----")
	player_ref:set_loading_flag(e_data_flag_money);
end


define_DP[1345] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:load_role_time_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---time--error-----")
		return
	end
	delog("-----------load---time----")
	player_ref:set_loading_flag(e_data_flag_time);
end




define_DP[1338] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end

	local s_buffer = msg_data.db_data
	local is_sucess = hld.dp2cs_load_role_info_lua(msg_data.unit_array_index,s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---role info--error-----")
		return
	end
	delog("-----------load---role-info---")
	player_ref:set_loading_flag(e_data_flag_info);
end


define_DP[32800] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_mail_mgr():load_mail_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---time--error-----")
		return
	end
	delog("------####-----load---mail sucess----")
	---player_ref:set_loading_flag(e_data_flag_time);
end


define_DP[32801] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_mail_mgr():load_mail_item_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---mial_item --error-----")
		return
	end
	delog("-----####-----load---mail--item--- sucess----")
	---player_ref:set_loading_flag(e_data_flag_time);
end


define_DP[1800] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = player_ref:get_item_set():load_item_by_db_lua(s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---item-- --error-----")
		return
	end
	delog("-----####-----load---role ---item--- sucess----")
	---player_ref:set_loading_flag(e_data_flag_time);
end

define_DP[1900] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = hld.skill_manager:get_instance():load_skill_by_db_lua(player_ref:get_array_index(), s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---role--skill-----error-----")
		return
	end
	delog("-----####-----load---role--skill--- sucess----")
	player_ref:set_loading_flag(e_data_flag_skill);
end

define_DP[2100] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2cs_proto.load_role_db_data", message_data)
    if msg_data == nil then
     	 return
    end
    local player_ref = hld.get_player_by_index(msg_data.unit_array_index)
	if nil == player_ref then
		return;
	end

	if player_ref:is_valid() == false or player_ref:get_unit_guid():get_AB() ~= msg_data.role_guid then
		return;
	end

	local s_buffer = msg_data.db_data
	local is_sucess = hld.buff_man:get_instance():load_buff_by_db_lua(player_ref:get_array_index(), s_buffer, string.len(s_buffer));

	if (is_sucess ~= true) then
		print("-----------load---role--buff-----error-----")
		return
	end
	delog("-----####-----load---role--buff--- sucess----")
	player_ref:set_loading_flag(e_data_flag_buff);
end



 
































g_dp_msgproc = dp_msgproc


return dp_msgproc