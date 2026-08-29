local welfare_class={}

function welfare_class:init()
end
define_C[12604] = function(message_data, player_ref)
	local msg_data = decode("hld.welfare_proto.get_reward", message_data)
	welfare_class:operate_welfare(player_ref, msg_data.template_id);
end

define_C[15800] = function(message_data, player_ref)
	local msg_data = decode("hld.welfare_proto.get_cumulative_sign_in_reward", message_data)
	if msg_data == nil then
     	return;
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	welfare_class:get_cumulative_sign_in_reward(player_ref, msg_data.reward_index);
end

define_C[15802] = function(message_data, player_ref)
	local msg_data = decode("hld.welfare_proto.retroactive_all_days", message_data)
	if msg_data == nil then
     	return;
    end
    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	welfare_class:retroactive_all_days(player_ref);
end

function welfare_class:operate_welfare(player_ref, welfare_template_id)
	local cur_class = player_ref:get_unit_info(hld.e_role_info_class_type)
	local msg = {template_id = tonumber(welfare_template_id),item_get={},money_get={}}
	local welfare_template = template_manager:get_line("WelfareTemplate", welfare_template_id)
	if welfare_template ==nil then
		msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
		return
	end
	local welfare_type = welfare_template.Type
	local welfare_value = welfare_template.TargetValue
	local welfare_index = 0
	hld.server_log:getwelfare_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), welfare_type, welfare_index)
	if welfare_type == hld.e_welfare_type_time_limit_reward then
		welfare_index = welfare_class:get_welfare_by_id_welfare_index(welfare_template)
		if welfare_index < 0 then
			msg.get_result=tonumber(ConstDefine.e_welfare_cant_get)
			MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
			return
		end
	else
		welfare_index = welfare_template.Index
	end
	if hld.e_welfare_type_regular_check_in == welfare_type then
		welfare_class:check_in(player_ref, welfare_template)
		return
	end
	if hld.e_welfare_type_monthcard_reward == welfare_type then
		welfare_class:month_card_check_in(player_ref, welfare_template)
		return
	end
	if hld.e_welfare_type_exclusive_check_in == welfare_type then
		welfare_class:exclusive_card_get_reward(player_ref, welfare_template)
		return
	end
	local cur_value = welfare_class:get_cur_value_by_welfare_typ(player_ref, welfare_type)
	if cur_value < welfare_value then
		msg.get_result=tonumber(ConstDefine.e_welfare_cant_get)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
		return
	end
	if welfare_class:is_welfare_geted(player_ref, welfare_type, welfare_index) then
		msg.get_result=tonumber(ConstDefine.e_welfare_already_geted)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
		return
	end
	local item_list={}
	local item_num_list={}
	local money_list={}
	local money_num_list={}
	local promp_item_data={}
	welfare_class:get_welfare(player_ref, welfare_template_id, item_list, item_num_list, money_list, money_num_list)
	
	--首充奖励特殊逻辑 第三天的时装奖励 改为随玩家选择时装任务变化(奖励为选择时装任务没有选择的一件时装) 第三天的其他奖励不变
	if welfare_type == hld.e_welfare_type_first_recharge_reward and welfare_index == 2 then
		welfare_class:get_first_recharge_fashion_reward(player_ref, cur_class, item_list, item_num_list)
	end
	
	--使用新的得到物品的提示
	--这个随机逻辑之所以这么复杂，就是因为点一次按钮，会将之前所有的全领掉
	--而且还要知道之前每一个福利给了什么
	if welfare_type == hld.e_welfare_type_online_time or welfare_type == hld.e_welfare_type_continue_login then
		--这两种需要点击一次，将之前的所有的奖励都领取
		for i=0,hld.e_welfare_random_item_max-1 do
		--这里其实只是给出一个较大值，防止死循环
			local pre_phase_id = welfare_template_id - i
			local pre_template = template_manager:get_line("WelfareTemplate", pre_phase_id)
			if pre_template ~= nil and pre_template.Type == welfare_type then
			--前一行表格是同类的福利
				if not welfare_class:is_welfare_geted(player_ref, pre_template.Type, pre_template.Index) then
				--前一行表格没领取过
				local item_drop_list_with_arr={item_id_arr={},item_const_att_arr={},item_lock_state_arr={}}
				repeat
						local RewardDropIDArray = pre_template.RewardDropID
						if #RewardDropIDArray <= cur_class then
							break
						end
						server_tool_mgr:gen_drop_id_list(RewardDropIDArray[cur_class + 1], item_drop_list_with_arr, ConstDefine.DROP_MAX_WEIGHT, 0, player_ref:get_unit_info(hld.e_role_info_class_type), player_ref:get_unit_info(hld.e_role_info_exp_level))
						local sub_temp_list = item_drop_list_with_arr.item_id_arr
						if #sub_temp_list > 0 then
							if welfare_type == hld.e_welfare_type_online_time then
								local real_random_index = hld.e_welfare_random_item_daily_online_0 + welfare_index - i

								welfare_class:set_random_get_info(player_ref, real_random_index, sub_temp_list[1])

								welfare_class:send_random_get_one(player_ref, real_random_index)
							elseif welfare_type == hld.e_welfare_type_continue_login then

								local real_random_index = hld.e_welfare_random_item_continue_login_0 + welfare_index - i
								welfare_class:set_random_get_info(player_ref, real_random_index, sub_temp_list[1])
								welfare_class:send_random_get_one(player_ref, real_random_index)
							end
							for sub_index=1,#sub_temp_list do

								welfare_class:add_welfare_to_vector(sub_temp_list[sub_index], 1, item_list, item_num_list)
							end
						end
					until true
				else
					break
				end
			end
		end
	end
	local total_item_num = #item_list
	if not player_ref:get_item_set():is_empty_slot_enough(hld.e_bag_type_bag, total_item_num) then
		msg.get_result=tonumber(ConstDefine.e_welfare_no_empty_slot)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
		return
	end
	if #money_list ~= #money_num_list or #item_list ~= #item_num_list then
		msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
		return
	end
	local notice_data={other_name={}, describe_str={}, item_name={}, num_type_data={},item_data={}}
	local item_array={}
	for i=1,#item_list do
		local item_template_id = item_list[i]
		local new_item_template_id = 0
		local item_num = item_num_list[i]
		local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_get_welfare, welfare_template_id,item_template_id, item_num, 1, 0, 0, 0)
		if temp_item ~= nil then
			local item_data =temp_item:get_item_inst()
			new_item_template_id = temp_item:get_item_template_id()
			local is_item_locked = temp_item:get_data_info(hld.e_item_info_locked)
			local item_tempid = template_manager:get_line("ItemTemplate", new_item_template_id)
			repeat
				if item_tempid == nil then
					break
				end
				if item_tempid.item_color >= hld.e_item_color_orange then
				table.insert(notice_data.item_data,item_data)
			end
				table.insert(item_array,temp_item)
				table.insert(promp_item_data,item_template_id)
				table.insert(promp_item_data,item_num)
				table.insert(promp_item_data,is_item_locked)

			until true
		end
		table.insert(msg.item_get, tonumber(new_item_template_id))
		table.insert(msg.item_get, tonumber(item_num))
	end
	welfare_class:put_item_into_bag(player_ref, item_array)
	--����
	if 0 ~= welfare_template.NoticeId and #notice_data.item_data > 0 then
		if welfare_type ~= hld.e_welfare_type_first_recharge_reward then
			table.insert(notice_data.num_type_data, welfare_value)
		end
		welfare_class:send_notice_new(player_ref, welfare_template.NoticeId,notice_data)
	end
	for i=1,#money_list do
		local money_id = money_list[i]
		local money_num = money_num_list[i]
		player_ref:add_money_or_exp(money_id, money_num, hld.e_server_log_add_money_welfare, welfare_template_id)
		table.insert(msg.money_get, tonumber(money_id))
		table.insert(msg.money_get, tonumber(money_num))
		table.insert(promp_item_data, money_id)
		table.insert(promp_item_data, money_num)
		table.insert(promp_item_data, 0)
	end
	welfare_class:get_item_send_promp_msg_to_client(player_ref, promp_item_data,false)
	msg.get_result = 0
	welfare_class:set_welfare_get_flag(player_ref, welfare_type, welfare_index)
	welfare_class:send_welfare_all(player_ref)
	MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end",msg, 12605)
	if hld.e_welfare_type_daily_activity_reward == welfare_type then
		player_ref:get_recycle_mgr():on_event(12)
	end
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_total_login, 0, 0)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_gs_welfare_num, 0, 0)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_level_welfare_num, 0, 0)
	if welfare_type == hld.e_welfare_type_exp_level_reward then
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_level_package, 0);
	end
	if welfare_type == hld.e_welfare_type_gs_reward then
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_gs_package, 0);
	end	
end

function welfare_class:get_welfare_by_id_welfare_index(welfare_template)
	if welfare_template ==nil then
		return -1
	end
	local ReceiveTime = welfare_template.ReceiveTime
	if #ReceiveTime <= 0 or #ReceiveTime % 2 ~= 0 then
		return -1
	end
	local index = welfare_template.Index
	for i=1, #ReceiveTime, 2 do
		local cur_time= os.date("*t")
		local cur_hour_in_day = cur_time.hour
		if cur_hour_in_day == ReceiveTime[i] then
			return index
		end
		index = index +1
	end
	return -1
end

function welfare_class:check_in(player_ref, welfare_template)
	if welfare_template == nil then
		return
	end
	if welfare_class:is_welfare_geted(player_ref, hld.e_welfare_type_regular_check_in, welfare_template.Index) then
		return
	end
	local temp, welfare_template_id = template_manager:GetLineByTypeAndTarget(welfare_template.Type,welfare_template.TargetValue)
	local msg = {template_id = tonumber(welfare_template_id),item_get={},money_get={}}
	local cur_time_info = os.date("*t")
	local current_data = cur_time_info.day
	if current_data < welfare_template.TargetValue then
		msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
		return
	end
	local is_replacement = false
	local game_config_template = template_manager:get_line("GameConfigTemplate", ConstDefine.game_config_template_id)
	local MonthCardReplacementCost = game_config_template.MonthCardReplacementCost
	if game_config_template == nil or #MonthCardReplacementCost < 3 then
		msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
		return
	end
	local replace_money_cost_id = MonthCardReplacementCost[1]
	local replace_money_cost_basic_num = MonthCardReplacementCost[2]
	local replace_money_cost_coefficient_num = MonthCardReplacementCost[3]
	local has_replace_times = welfare_class:get_has_replace_times(player_ref)
	local replace_money_cost_num = replace_money_cost_basic_num + replace_money_cost_coefficient_num * (has_replace_times + 1)
	msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
	if cur_time_info.day-1 ~=  welfare_template.Index then
		is_replacement = true
		if not player_ref:can_cut_money(replace_money_cost_id, replace_money_cost_num) then
			player_ref:get_chat_mgr():send_notice(welfare_template.NoticeId, tostring(90029046))
			msg.get_result=tonumber(ConstDefine.e_item_string_yuanbao_bind)
			MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
			return
		end
	end
	local data_array={}
	local target_value = welfare_template.TargetValue
	--常规签到
	if false == welfare_class:get_welfare_reward(player_ref, hld.e_welfare_type_regular_check_in, target_value, msg, data_array) then
		MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
		return
	end
	welfare_class:set_welfare_get_flag(player_ref, hld.e_welfare_type_regular_check_in, welfare_template.Index)
	--设置签到次数
	player_ref:set_logic_data(hld.e_role_logic_info_check_in_days,player_ref:get_logic_data(hld.e_role_logic_info_check_in_days) + 1)
	--更新客户端总签到次数
	player_ref:send_logic_one(hld.e_role_logic_info_check_in_days)
	local RewardItemArray = temp.RewardItemArray
	local RewardMoneyArray = temp.RewardMoneyArray
	msg.get_result=tonumber(ConstDefine.e_item_string_succeed)
	--welfare_class:send_notice(RewardItemArray, RewardMoneyArray, welfare_template.NoticeId)
	if is_replacement==true then
		player_ref:cut_money(replace_money_cost_id, replace_money_cost_num, hld.e_server_log_cut_money_welfare_replacement, welfare_template_id, 0)
		welfare_class:add_replace_times(player_ref)
	end
	welfare_class:set_today_check_in(player_ref)
	msg.is_replacement=is_replacement
	welfare_class:get_item_send_promp_msg_to_client(player_ref, data_array,false)
	welfare_class:send_welfare_all(player_ref)
	MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
end

function welfare_class:is_welfare_geted(player_ref, welfare_type, welfare_index)
	local is_geted = true
	if welfare_type < 0 or welfare_type >= hld.e_welfare_type_max then
		return is_geted
	end
	local temp_flag = 1 << welfare_index;
	local cur_geted_flag = player_ref:get_welfare_mgr():get_welfare_info(welfare_type)
	if welfare_type == hld.e_welfare_type_continue_login or welfare_type == hld.e_welfare_type_online_time  then
		if (welfare_index >= cur_geted_flag)then
			is_geted = false;
		end
	else
		if (temp_flag & cur_geted_flag) > 0 then
			is_geted = true;
		else
			is_geted = false;
		end
	end
	return is_geted
end

function welfare_class:get_has_replace_times(player_ref)
	return player_ref:get_welfare_mgr():get_welfare_info(hld.e_welfare_type_replace_times)
end

function welfare_class:get_welfare_reward(player_ref, welfare_type, welfare_target_value, msg, data_array)
	local template, template_id= template_manager:GetLineByTypeAndTarget(welfare_type, welfare_target_value)
	if template == nil then
		msg.get_result = tonumber(ConstDefine.e_item_string_unkown)
		return false
	end
	local RewardItemArray = template.RewardItemArray
	local RewardMoneyArray = template.RewardMoneyArray
	local AdditionalItemRewardID = template.AdditionalItemRewardID
	local AdditionalMoneyRewardID = template.AdditionalMoneyRewardID
	if math.fmod(#RewardItemArray, 2) ~= 0 or math.fmod(#RewardMoneyArray , 2) ~= 0 or math.fmod(#AdditionalItemRewardID , 2) ~=0 or math.fmod(#AdditionalMoneyRewardID , 2) ~= 0 then
		return false
	end
	local item_list={}
	local item_num_list={}
	local money_list={}
	local money_num_list={}
	local welfare_template_id = template_id
	welfare_class:get_welfare(player_ref, welfare_template_id, item_list, item_num_list, money_list, money_num_list)
	local total_item_num = #item_list
	if not player_ref:get_item_set():is_empty_slot_enough(hld.e_bag_type_bag, total_item_num) then
		msg.get_result=tonumber(ConstDefine.e_welfare_no_empty_slot)
		return false
	end
	if #money_list ~= #money_num_list or #item_list ~= #item_num_list then
		msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
		return false
	end
	local is_item_lock = 0
	for i=1,#item_list do
		local item_template_id = item_list[i]
		local item_num = item_num_list[i]
		local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_get_welfare, welfare_template_id,item_template_id, item_num, 1, 0, 0, 0)
		repeat
			if temp_item == nil then
				break
			end
			is_item_lock = temp_item:get_data_info(hld.e_item_info_locked)
			player_ref:get_item_set():put_item_into_bag(temp_item)
			table.insert(msg.item_get,tonumber(item_template_id))
			table.insert(msg.item_get,tonumber(item_num))
			table.insert(data_array,item_template_id)
			table.insert(data_array,item_num)
			table.insert(data_array,is_item_lock)

		until true
	end
	for i=1,#money_list do
		local money_id = money_list[i]
		local money_num = money_num_list[i]
		player_ref:add_money_or_exp(money_id, money_num, hld.e_server_log_add_money_welfare, welfare_template_id)
		table.insert(msg.money_get,tonumber(money_id))
		table.insert(msg.money_get,tonumber(money_num))
	end
	return true
end

function welfare_class:put_item_into_bag(player_ref, item_inst_array)
    for k,v in pairs(item_inst_array) do
		if v ~= nil then 
			player_ref:get_item_set():put_item_into_bag(v)
		end
    end
end

function welfare_class:send_item_by_mail(player_ref, item_array, mail_info)
    local item_info_array={};
    for i=1,#item_array do
        local item = item_array[i];
        repeat
            if item == nil then
                break;
            end
            table.insert(item_info_array,item:get_item_inst())

        until true
        player_ref:get_item_set():del_item(hld.e_server_log_del_item_send_mail, 0,item, item:get_data_info(hld.e_item_info_stack_count));
    end
    welfare_class:send_item_by_mail_overload(player_ref, item_info_array, mail_info);
end

function welfare_class:send_item_by_mail_overload(unit_array_index, item_array, mail_info)
    local temp_unit = hld.unit_man():get_unit(unit_array_index);
    if temp_unit:is_valid() == false then
        return;
    end
    local mail_item_info_array={}
    local item_in_mail_index = 1;
    local item_inst_array_size=#item_array;
    for i=1,item_inst_array_size do
        if item_in_mail_index <= ConstDefine.max_item_per_mail then
            mail_item_info_array[item_in_mail_index] = item_array[i];
            if item_in_mail_index == max_item_per_mail then
                hld.globle_data:get_instance():send_mail_to_another_player_by_system(
                    temp_unit:get_unit_guid(),
                    temp_unit:get_unit_guid(),
                    temp_unit:get_unit_info(hld.e_role_info_server_id),
                    "", mail_info.title,
                    mail_info.contentText,
                    mail_info.money_typ1,
                    mail_info.money_num1,
                    mail_info.money_typ2,
                    mail_info.money_num2,
                    mail_item_info_array,
                    max_item_per_mail);
                for i=1,#mail_item_info_array do
                    mail_item_info_array[i]=0;
                end
                item_in_mail_index = 1;
            else
                item_in_mail_index=item_in_mail_index+1;
            end
        end
    end
    if item_in_mail_index > 0 then
        hld.globle_data:get_instance():send_mail_to_another_player_by_system(
				temp_unit:get_unit_guid(),
				temp_unit:get_unit_guid(),
				temp_unit:get_unit_info(hld.e_role_info_server_id),
				"", mail_info.title,
				mail_info.contentText,
				mail_info.money_typ1,
				mail_info.money_num1,
				mail_info.money_typ2,
				mail_info.money_num2,
				mail_item_info_array,
				max_item_per_mail);
    end
end

function welfare_class:add_replace_times(player_ref)
	local temp=player_ref:get_welfare_mgr():get_welfare_info(hld.e_welfare_type_replace_times)
	player_ref:get_welfare_mgr():set_welfare_info(hld.e_welfare_type_replace_times, temp+1)
end

function welfare_class:set_today_check_in(player_ref)
	player_ref:get_welfare_mgr():set_welfare_info(hld.e_welfare_type_is_today_check_in, 1)
end

function welfare_class:get_item_send_promp_msg_to_client(player_ref, item_tuple_array, is_continue_get)
    --is_continue_get = is_continue_get~=false



    if #item_tuple_array % hld.e_item_get_info_max ~= 0 or #item_tuple_array <= 0 then

        return
    end

    local data_num = #item_tuple_array / hld.e_item_get_info_max


    local client_pak = {role_guid={tonumber(player_ref:get_unit_guid_A()),tonumber(player_ref:get_unit_guid_B())},is_continue_add=is_continue_get,item_data={}}

    for i=0,data_num-1 do
        local item_id = item_tuple_array[i * hld.e_item_get_info_max + hld.e_item_get_info_item_template_id+1]
        local item_num = item_tuple_array[i * hld.e_item_get_info_max + hld.e_item_get_info_item_num+1]
        local item_locked = item_tuple_array[i * hld.e_item_get_info_max + hld.e_item_get_info_locked_state+1]
        local item_template = template_manager:get_line("ItemTemplate",item_id)

        repeat
            if item_template and item_template.AutoUse ~= 0 then

                break
            end
            local cur_item_num = #client_pak.item_data / hld.e_item_get_info_max
            local is_merge = false

            for j=0,cur_item_num-1 do
                local this_id = client_pak.item_data[j * hld.e_item_get_info_max + hld.e_item_get_info_item_template_id+1]
				local this_num = client_pak.item_data[j * hld.e_item_get_info_max + hld.e_item_get_info_item_num+1]
                local this_locked = client_pak.item_data[j * hld.e_item_get_info_max + hld.e_item_get_info_locked_state+1]

                if this_id == item_id and this_locked == item_locked then

                    is_merge = true
                    client_pak.item_data[j * hld.e_item_get_info_max + hld.e_item_get_info_item_num] = this_num + item_num

					break
                end
            end
            if not is_merge then

                table.insert(client_pak.item_data,item_id)
                table.insert(client_pak.item_data,item_num)
                table.insert(client_pak.item_data,item_locked)

            end
        until true
    end

    for k,v in pairs(client_pak) do
		if type(v)=="table" then
			for key,val in pairs(v) do

			end
		else

		end
	end
    MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_get_item_msg", client_pak, 10711)
end

function welfare_class:set_welfare_get_flag(player_ref, welfare_type,welfare_index)
	if welfare_type < 0 or welfare_type >= hld.e_welfare_type_max then
		return
	end
	local end_value = 0;
	if welfare_type == hld.e_welfare_type_continue_login or welfare_type == hld.e_welfare_type_online_time  then
		end_value =  welfare_index + 1;
	else
		local temp_flag = 1 << welfare_index;
		end_value = temp_flag | player_ref:get_welfare_mgr():get_welfare_info(welfare_type)
	end
	player_ref:get_welfare_mgr():set_welfare_info(welfare_type, end_value)
end

function welfare_class:send_welfare_all(player_ref)
	local msg={info_value={}}
	for i=0,hld.e_welfare_type_max-1 do
		table.insert(msg.info_value, player_ref:get_welfare_mgr():get_welfare_info(i))
	end
	local cur_time_info = os.date("*t")
	msg.days_num = tonumber(welfare_class:get_days_by_month_year(cur_time_info.year,cur_time_info.month))
	msg.today_data = tonumber(cur_time_info.day)
	for k,v in pairs(msg) do
		if type(v)=="table" then
			for key,val in pairs(v) do

			end
		else

		end
	end
	MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.welfare_info_all", msg, 12602)
end

function welfare_class:get_days_by_month_year(year, month)
	local curYear = tonumber(year)
	local curMonth = tonumber(month)
	if not curYear or curYear <= 0 or not curMonth or curMonth <= 0 then
		return
	end
	local curDate = {}
	curDate.year = curYear
	curDate.month = curMonth + 1
	curDate.day = 0
	local maxDay = os.date("%d",os.time(curDate))
	return maxDay
end

function welfare_class:month_card_check_in(player_ref, welfare_template)
	if welfare_template == nil then
		return
	end
	if welfare_class:is_welfare_geted(player_ref, hld.e_welfare_type_monthcard_reward, welfare_template.Index) == true then
		return
	end
	local temp, welfare_template_id = template_manager:GetLineByTypeAndTarget(welfare_template.Type,welfare_template.TargetValue)
	local msg ={template_id=tonumber(welfare_template_id),item_get={},money_get={}}
	msg.get_result = tonumber(ConstDefine.e_item_string_unkown)
	local target_value = welfare_template.TargetValue
	local data_array = {}
	--月卡签到
	if player_ref:is_recharge_by_type(hld.e_recharge_type_month_card) then
		target_value = player_ref:get_month_card_current_activity_time()
		if not welfare_class:is_welfare_geted(player_ref, hld.e_welfare_type_monthcard_reward, target_value - 1) then
			if not welfare_class:get_welfare_reward(player_ref, hld.e_welfare_type_monthcard_reward, target_value, msg, data_array) then
				MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
				return
			end
			msg.get_result=tonumber(ConstDefine.e_item_string_succeed)
			welfare_class:set_welfare_get_flag(player_ref, hld.e_welfare_type_monthcard_reward, target_value - 1)
			local temp_welfare_template, template_id = template_manager:GetLineByTypeAndTarget(hld.e_welfare_type_monthcard_reward, target_value)
			if temp_welfare_template ~=nil then
				local RewardItemArray = temp_welfare_template.RewardItemArray
				local RewardMoneyArray = temp_welfare_template.RewardMoneyArray
				--welfare_class:send_notice(RewardItemArray, RewardMoneyArray, temp_welfare_template.NoticeId)

			end
		end
	end
	welfare_class:get_item_send_promp_msg_to_client(player_ref, data_array,false)
	welfare_class:send_welfare_all(player_ref)
	--����
	MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
end

function welfare_class:exclusive_card_get_reward(player_ref, welfare_template)
	if	welfare_template == nil then
		return
	end
	if welfare_class:is_welfare_geted(player_ref, hld.e_welfare_type_exclusive_check_in, welfare_template.Index) then
		return
	end
	local temp, welfare_template_id = template_manager:GetLineByTypeAndTarget(welfare_template.Type,welfare_template.TargetValue)
	local msg={template_id=tonumber(welfare_template_id),item_get={},money_get={}}
	temp=nil
	msg.get_result=tonumber(ConstDefine.e_item_string_unkown)
	local data_array={}
	if player_ref:is_recharge_by_type(hld.e_recharge_type_exclusive) then
		target_value = player_ref:get_exclusive_card_current_activity_time()
		if false == welfare_class:is_welfare_geted(player_ref, hld.e_welfare_type_exclusive_check_in, target_value - 1) then
			if false == welfare_class:get_welfare_reward(player_ref, hld.e_welfare_type_exclusive_check_in, target_value, msg, data_array) then
				MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
				return
			end
			msg.get_result=tonumber(ConstDefine.e_item_string_succeed)
			welfare_class:set_welfare_get_flag(player_ref, hld.e_welfare_type_exclusive_check_in, target_value - 1)
			local temp_welfare_template, template_id=template_manager:GetLineByTypeAndTarget(hld.e_welfare_type_exclusive_check_in, target_value)
			if temp_welfare_template ~= nil then
				--welfare_class:send_notice(temp_welfare_template.RewardItemArray, temp_welfare_template.RewardMoneyArray, temp_welfare_template.NoticeId)
			end
		end
	end
	welfare_class:get_item_send_promp_msg_to_client(player_ref, data_array,false)
	welfare_class:send_welfare_all(player_ref)
	--发送
	MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.get_reward_end", msg, 12605)
end

function welfare_class:get_welfare(player_ref, welfare_template_id, item_list, item_num_list, money_list, money_num_list)
	if not player_ref:is_valid() then
		return
	end
	local template = template_manager:get_line("WelfareTemplate", welfare_template_id)
	if template == nil then
		return
	end
	local RewardItemArray = template.RewardItemArray
	local RewardMoneyArray = template.RewardMoneyArray
	local AdditionalItemRewardID = template.AdditionalItemRewardID
	local AdditionalMoneyRewardID = template.AdditionalMoneyRewardID
	local welfare_type = template.Type
	if welfare_type == hld.e_welfare_type_daily_activity_reward then
		welfare_class:get_active_welfare(player_ref, welfare_template_id, item_list, item_num_list, money_list, money_num_list)
		return
	end
	if #RewardItemArray % 2 ~= 0 or #RewardMoneyArray % 2 ~= 0 or #AdditionalItemRewardID % 2 ~= 0 or #AdditionalMoneyRewardID % 2 ~= 0 then
		return
	end
	local base_item_get_num = #RewardItemArray / 2
	local base_money_get_num = #RewardMoneyArray / 2
	local additional_item_get_num = #AdditionalItemRewardID / 2
	local additional_money_get_num = #AdditionalMoneyRewardID / 2
	for i=0,base_item_get_num-1 do
		local item_id = RewardItemArray[i * 2 + 1]
		local item_num = RewardItemArray[i * 2 + 2]
		if template.VipDoubleRank ~= 0 and player_ref:get_vip_level() >= template.VipDoubleRank then
			item_num = item_num * 2
		end
		repeat
			if not welfare_class:is_correct_class_type(player_ref, item_id) then
				break
			end
			welfare_class:add_welfare_to_vector(item_id, item_num, item_list, item_num_list)
			break
		until true
	end
	for i=0,base_money_get_num-1 do
		local money_id = RewardMoneyArray[i * 2 + 1]
		local money_num = RewardMoneyArray[i * 2 + 2]
		welfare_class:add_welfare_to_vector(money_id, money_num, money_list, money_num_list)
	end
	for i=0,additional_item_get_num-1 do
		local item_id = AdditionalItemRewardID[i * 2 + 1]
		local item_num = AdditionalItemRewardID[i * 2 + 2]
		repeat
			if not welfare_class:is_correct_class_type(player_ref, item_id) then
				break
			end
			welfare_class:add_welfare_to_vector(item_id, item_num, item_list, item_num_list)
		until true
	end
	for i=0,additional_money_get_num-1 do
		local money_id = AdditionalMoneyRewardID[i * 2 + 1]
		local money_num = AdditionalMoneyRewardID[i * 2 + 2]
		welfare_class:add_welfare_to_vector(money_id, money_num, money_list, money_num_list)
	end
	if welfare_type == hld.e_welfare_type_online_time or welfare_type ==hld.e_welfare_type_continue_login then
		local pre_phase_id = welfare_template_id - 1
		local welfare_template = template_manager:get_line("WelfareTemplate", pre_phase_id)
		if welfare_template ~=nil and welfare_template.Type == welfare_type then
			if not welfare_class:is_welfare_geted(player_ref, welfare_template.Type, welfare_template.Index) then
				welfare_class:get_welfare(player_ref, pre_phase_id, item_list, item_num_list, money_list, money_num_list)
			end
		end
	end

end

function welfare_class:get_active_welfare(player_ref, welfare_template_id, item_list, item_num_list, money_list, money_num_list)	
	if not player_ref:is_valid() then
		return
	end
	local template = template_manager:get_line("WelfareTemplate", welfare_template_id)
	if template == nil then
		return
	end
	local region_code = template_manager:get_field("RegionTemplate", 150000000, "RegionCode");
	if region_code == nil then
		return
	end
	local RewardItemArray = template.RewardItemArray
	local RewardMoneyArray = template.RewardMoneyArray
	local AdditionalItemRewardID = template.AdditionalItemRewardID
	local AdditionalMoneyRewardID = template.AdditionalMoneyRewardID
	local welfare_type = template.Type
	if welfare_type ~= hld.e_welfare_type_daily_activity_reward then
		return
	end
	if #RewardItemArray % 3 ~= 0 or #RewardItemArray <= 0 then
		return
	end
	for i = 0, ((#RewardItemArray // 3) - 1) do
		if RewardItemArray[i * 3 + 1] == region_code then
			local item_id = RewardItemArray[i * 3 + 2]
			local item_num = RewardItemArray[i * 3 + 3]
			welfare_class:add_welfare_to_vector(item_id, item_num, item_list, item_num_list)
			break
		end
	end
end
function welfare_class:add_welfare_to_vector(id, num, id_list, num_list)
	if #id_list ~= #num_list or num==0 then
		return
	end
	for i=1,#id_list do
		if id_list[i] == id then
			local temp_item_template = template_manager:get_line("ItemTemplate", id_list[i])
			if temp_item_template ~=nil then
				--物品还要考虑堆叠数
				local pile_num = temp_item_template.max_pile_num
				if num_list[i] + num <=pile_num then
				--能全部堆叠到一起
					num_list[i] = num_list[i] + num
					return
				else
					local can_merge_num = pile_num - num_list[i]
					num_list[i] = pile_num
					num = num-can_merge_num
					break
				end
			else
				num_list[i] = num_list[i] + num
				return
			end
		end
	end
	table.insert(id_list,id)
	table.insert(num_list,num)
end

function welfare_class:is_correct_class_type(player_ref, item_template_id)
	local cur_type = player_ref:get_unit_info(hld.e_role_info_class_type)
	local template = template_manager:get_line("ItemTemplate", item_template_id)
	if template == nil then
		return false
	end
	local class_type = template.class_type
	if #class_type == 0 then
		return true
	end
	for i=1, #class_type do
		if tonumber(class_type[i]) == 0 or tonumber(class_type[i]) == cur_type then
			return true
		end
	end
	return false
end

function welfare_class:get_cur_value_by_welfare_typ(player_ref, welfare_type)
	local return_value = -1
	if welfare_type < 0 or welfare_type >= hld.e_welfare_type_max then
		return return_value
	end
	if welfare_type == hld.e_welfare_type_first_recharge_reward then
		return_value = player_ref:get_unit_info(hld.e_role_info_real_recharge_jewel_num)
	elseif welfare_type == hld.e_welfare_type_weekend_recharge_reward or welfare_type == hld.e_welfare_type_daily_recharge_reward then
		return_value = player_ref:get_unit_info(hld.e_role_info_daily_recharge_num)
	elseif welfare_type == hld.e_welfare_type_total_recharge_reward then
		return_value = player_ref:get_unit_info(hld.e_role_info_real_recharge_jewel_num)
	elseif welfare_type == hld.e_welfare_type_total_cost_reward then
		return_value = player_ref:get_unit_info(hld.e_role_info_total_diamond_cost_num)
	elseif welfare_type == hld.e_welfare_type_daily_activity_reward then
		return_value = player_ref:get_unit_info(hld.e_role_info_daily_activity_degree)
	elseif welfare_type == hld.e_welfare_type_exp_level_reward then
		return_value = player_ref:get_unit_info(hld.e_role_info_exp_level)
	elseif welfare_type == hld.e_welfare_type_gs_reward then
		return_value = player_ref:get_unit_gs_value()
	elseif welfare_type == hld.e_welfare_type_continue_login then
		return_value = player_ref:get_unit_info(hld.e_role_info_continue_login_time)
	elseif welfare_type == hld.e_welfare_type_total_login then
		return_value = player_ref:get_unit_info(hld.e_role_info_total_login_days)
	elseif welfare_type == hld.e_welfare_type_online_time then
		return_value = welfare_class:get_active_degree_info(player_ref, hld.e_daily_active_degree_type_online_30_minutes)
	elseif welfare_type == hld.e_welfare_type_monthcard_reward then
		return_value = player_ref:get_time_data(hld.e_time_type_month_card_activity_time)
	elseif welfare_type == hld.e_welfare_type_time_limit_reward then
		return_value = 0
	else
	end
	return return_value
end

function welfare_class:get_active_degree_info(player_ref, info_type)
	if info_type >= hld.e_daily_active_degree_type_max then
		return -1
	end
	return player_ref:get_welfare_mgr():get_active_degree_info(info_type)
end

function welfare_class:set_random_get_info(player_ref, info_index,info_value)
	if info_index >= hld.e_welfare_random_item_max then
		return
	end
	player_ref:get_welfare_mgr():set_random_get_info(info_index, tonumber(info_value))
end

function welfare_class:send_random_get_one(player_ref, info_index)
	if info_index >= hld.e_welfare_random_item_max then
		return
	end
	local msg={info_value={player_ref:get_welfare_mgr():get_random_get_info(info_index)},data_index={tonumber(info_index)}}
	MessageMgr:send_message_to_self(player_ref, "hld.welfare_proto.random_get_info",msg, 12607)
end

function welfare_class:send_notice(item_array,money_array,notice_id)

end

------------------------------待完善--------------------------------------
function welfare_class:send_notice_new(player_ref, notice_template_id, notice_data)

    if player_ref == nil then
        return
    end
    if notice_template_id <= 0 then
        return
    end
	local m_notice_data = {};
	m_notice_data.notice_id = notice_template_id
	m_notice_data.item_data = notice_data.item_data
	server_tool_mgr:send_notice(player_ref, m_notice_data);
end

function welfare_class:send_notice_new_overload(player_ref, notice_template_id, other_name, describe_str, item_name, num_type_data, item_data)

    if player_ref==nil then
        return
    end
    local notice_str = ""
    if welfare_class:parse_notice_by_symbol_type(notice_template_id, player_ref:get_name(), other_name, describe_str, item_name, num_type_data, notice_str) then
        welfare_class:make_and_send_notic_msg_data(player_ref, player_ref:get_unit_guid(),notice_template_id,notice_str,item_data)
    end
end

function welfare_class:parse_notice_by_symbol_type(notice_id, unit_name, other_name, describe_str, item_name, num_type_data, _out_notice_str)
    if notice_id <=0 then
        return false
    end
	local NoticeTemplate = template_manager:get_line("NoticeTemplate", notice_id)
    local notice_ContentText = NoticeTemplate.ContentText
    local notice_ParseTypeArrray = NoticeTemplate.ParseTypeArrray
    local param_type_size = #notice_ParseTypeArrray
    if #notice_ContentText <= 0 or param_type_size <= 0 then
        return false
    end
    local notice_str_id=tostring(notice_ContentText[1])
    local other_name_index = 0;
    local item_name_index = 0;
    local describe_str_index = 0;
    local num_type_data_index = 0;
    local item_data_array_index = 0;
    local item_data_index = 0;
    local is_had_item_link = false
    local notice_str_params_vec={}
    table.insert(notice_str_params_vec,notice_str_id)
    for param_type_index=1,param_type_size do
        if notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_unit_name then
            table.insert(notice_str_params_vec, unit_name)
        elseif notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_item_name then
            if item_name_index >= #item_name then
                return false
            end
            table.insert(notice_str_params_vec,item_name[item_name_index])
            item_name_index = item_name_index + 1
        elseif notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_num_data then
            if num_type_data_index >= #num_type_data then
                    return false;
            end
            table.insert(notice_str_params_vec,tostring(num_type_data[num_type_data_index]))
            num_type_data_index = num_type_data_index + 1
        elseif notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_item_data then
            if not is_had_item_link then
                is_had_item_link = true
                item_data_index = #notice_str_params_vec
                table.insert(notice_str_params_vec, " %d")
            else
                local temp_str = notice_str_params_vec[item_data_index]
                notice_str_params_vec[item_data_index] = temp_str.." %d"
            end
        elseif notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_describe_str then
            if describe_str_index >= #describe_str then
                return false
            end
            table.insert(notice_str_params_vec, describe_str[describe_str_index])
            describe_str_index = describe_str_index + 1
        elseif notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_other_name then
            if other_name_index >= string.len(unit_name) then
                return false
            end
            table.insert(notice_str_params_vec, other_name[other_name_index])
            other_name_index = other_name_index + 1
        elseif notice_ParseTypeArrray[param_type_index] == hld.e_notice_param_type_max then

        else

        end
    end
    _out_notice_str = init_unit.implode(notice_str_params_vec)
    return ture
end

function welfare_class:make_and_send_notic_msg_data(player_ref, unit_guid, notice_template_id, show_string, item_data)
    local notice_msg=hld.cs2ws_send_notice:new()
    notice_msg.role_guid = unit_guid
    notice_msg.notice_id = notice_template_id
    notice_msg.client_uid=player_ref:get_client_uid()
    local item_num = #item_data
    for index=1,item_num do
        if notice_msg.item_num >= ConstDefine.chat_max_item then
            local text_len = string.len(show_string)
            if text_len > ConstDefine.globel_message_max_size then
                return false
            end
            notice_msg.item_num =notice_msg.item_num - 1;			--消息里的定义不是item数量而是索引
            local show_tab=welfare_class:str2tab(show_string)
            for i=1,#show_tab do
                table.insert(notice_msg.notice_string,show_tab[i])
            end
            --TODO
        end
    end
end

function welfare_class:str2tab(str)
    if type(str)~="string" or str=="" then
        return
    end
    local tab={}
    for i=1,string.len(str) do
        table.insert(tab,string.sub(str,i,i))
    end
    return tab
end

function welfare_class:get_cumulative_sign_in_reward(player_ref, reward_index)
	if player_ref == nil or player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	if reward_index == nil then
		return;
	end
	player_ref:get_welfare_mgr():get_cumulative_sign_in_reward(reward_index)
end

function welfare_class:retroactive_all_days(player_ref)
	if player_ref == nil or player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	player_ref:get_welfare_mgr():retroactive_all_days()
end

function welfare_class:get_first_recharge_fashion_reward(player_ref, cur_class, item_list, item_num_list)
	if not player_ref:is_valid() then
		return
	end
	local game_config_template = template_manager:get_line("GameConfigTemplate", ConstDefine.game_config_template_id)
	if game_config_template == nil then
		return
	end
	local FashionArr = game_config_template.MissionChooseFashion
	if #FashionArr < 12 then
		return
	end
	local ChooseFashionId = player_ref:get_logic_data(hld.e_role_logic_info_choose_fashion_id)
	for i=1,#FashionArr-2 do
		if cur_class == FashionArr[i] then
			if ChooseFashionId == FashionArr[i+1] then
				welfare_class:add_welfare_to_vector(FashionArr[i+2], 1, item_list, item_num_list)
			else
				welfare_class:add_welfare_to_vector(FashionArr[i+1], 1, item_list, item_num_list)
			end
			break
		end
	end	
end

welfare = welfare_class
return welfare






