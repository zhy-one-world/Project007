local seven_day_goal_class = {}
function seven_day_goal_class:init()
end

define_C[13807] = function(message_data, player_ref)
	local msg_data = decode("hld.service_goal_proto.seven_day_goal_operate", message_data)
	if msg_data == nil then
		return
    end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	seven_day_goal_class:seven_day_goal_operate(player_ref,msg_data.operate_type,msg_data.seven_day_goal_id)
end

define_C[13803] = function(message_data, player_ref)
	local msg_data = decode("hld.service_goal_proto.service_goal_operate", message_data)
	if msg_data == nil then
		return
    end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	seven_day_goal_class:service_goal_operate(player_ref,msg_data.operate_type,msg_data.service_goal_id)
end

function seven_day_goal_class:service_goal_operate(player_ref, operate_type,service_goal_id)
	if player_ref == nil or operate_type == nil or service_goal_id == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	player_ref:get_service_goal_mgr():service_goal_operate(operate_type, service_goal_id);

end
function seven_day_goal_class:seven_day_goal_operate(player_ref,operate_type, seven_day_goal_id)
    if operate_type == hld.e_service_goal_operate_type_receive then
        seven_day_goal_class:received_seven_day_goal(player_ref,seven_day_goal_id);

        player_ref:get_seven_day_goal_mgr():refresh_seven_day_goal_received_reward_days();
 
        seven_day_goal_class:refresh_seven_day_goal(player_ref,hld.e_service_goal_type_big_reward);
        seven_day_goal_class:refresh_seven_day_goal(player_ref,hld.e_service_goal_type_get_all_reward);
        seven_day_goal_class:refresh_seven_day_goal(player_ref,hld.e_service_goal_type_seven_day_final);
    end
end

function seven_day_goal_class:received_seven_day_goal(player_ref,seven_day_goal_id)

    local temp_seven_day_goal_obj = player_ref:get_seven_day_goal_mgr():get_seven_day_goal_obj_by_id(seven_day_goal_id);
    if temp_seven_day_goal_obj == nil then
        return;
    end
    local operate_result = temp_seven_day_goal_obj:can_received_seven_day_goal();
    if (ConstDefine.e_item_string_succeed == operate_result) then
	    if (player_ref:get_seven_day_goal_mgr():put_reward_into_bag(temp_seven_day_goal_obj:get_data_info(hld.e_seven_day_goal_info_id))) then
            temp_seven_day_goal_obj:set_data_info(hld.e_seven_day_goal_info_state, hld.e_service_goal_state_has_been_received);
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_service_goal, 0, 0)
		else
            operate_result = 0;
        end
    end
    player_ref:get_seven_day_goal_mgr():send_seven_day_goal_operate_result_to_self(hld.e_service_goal_operate_type_receive, operate_result, temp_seven_day_goal_obj);
end

function seven_day_goal_class:refresh_seven_day_goal(player_ref,seven_day_goal_type)

	local m_num = player_ref:get_seven_day_goal_mgr():get_seven_day_goal_num()
    for i=0,m_num-1 do
        local temp_seven_day_goal_obj = player_ref:get_seven_day_goal_mgr():get_seven_day_goal_obj_by_index_lua(i);
        if (seven_day_goal_type == temp_seven_day_goal_obj:get_seven_day_goal_condition_type()) then
			seven_day_goal_class:refresh_seven_day_goal_one(player_ref,temp_seven_day_goal_obj, true);
        end
    end
end

function seven_day_goal_class:refresh_seven_day_goal_one(temp_player,seven_day_goal_obj_ptr,is_self_add)

		if (hld.e_service_goal_state_unfinished ~= seven_day_goal_obj_ptr:get_data_info(hld.e_seven_day_goal_info_state)) then
			return;
        end
        local temp_template_ptr = template_manager:get_line("ServiceGoalTemplate", seven_day_goal_obj_ptr:get_data_info(hld.e_seven_day_goal_info_id));
        if temp_template_ptr == nil then
			return
        end
        local temp_ConditionValue = Common:StrToTable(temp_template_ptr.ConditionValue)
		if (3 ~= #temp_ConditionValue) then
			return;
        end
		local value1 = temp_ConditionValue[1];
		local value2 = temp_ConditionValue[2];
		local value3 = temp_ConditionValue[3];
		local seven_day_goal_type = seven_day_goal_obj_ptr:get_seven_day_goal_condition_type();
		local target_value = value1;
		local current_value = 0;    
        if seven_day_goal_type == hld.e_service_goal_type_role_level then
			current_value = temp_player:get_unit_info(e_role_info_exp_level);
		elseif seven_day_goal_type == hld.e_service_goal_type_role_gs then
			current_value = temp_player:get_role_gs();
		elseif seven_day_goal_type == hld.e_service_goal_type_big_reward then
			local active_data = value2;
			local active_type = value3;
			current_value = temp_player:get_seven_day_goal_mgr():get_seven_day_goal_reward_days(active_data, active_type);
		elseif seven_day_goal_type == hld.e_service_goal_type_strength_any then
			local strength_item_level = value2;
			current_value = temp_player:get_item_set():get_equip_num_by_strength_limit(strength_item_level);
		elseif seven_day_goal_type == hld.e_service_goal_type_addition_the_body then
			local addition_item_leve = value2;
			current_value = temp_player:get_item_set():get_equip_num_by_addition_limie(addition_item_leve);
		elseif seven_day_goal_type == hld.e_service_goal_type_mount_level then
			local order_num = value2;
			local current_mount_grade = 0;
			local current_mount_level = 0;
			current_mount_grade,current_mount_level = temp_player:get_item_set():get_mount_grade_and_level_by_order_num(order_num, current_mount_grade, current_mount_level);
			current_value = current_mount_grade * 11 + current_mount_level;
		elseif seven_day_goal_type == hld.e_service_goal_type_mount_num then
			target_value = value1;
			current_value = temp_player:get_item_set():get_total_mount_activate_num();
		elseif seven_day_goal_type == hld.e_service_goal_type_friend_num then
			target_value = value1;
			if (is_self_add) then
				current_value = seven_day_goal_obj_ptr:get_data_info(hld.e_seven_day_goal_info_schedule) + 1;
            end
        elseif seven_day_goal_type == hld.e_service_goal_type_friendliness then
			target_value = value1;
			current_value = temp_player:get_highest_record(hld.e_role_history_highest_record_higest_friendliness_value);
		elseif seven_day_goal_type == hld.e_service_goal_type_clearance_fuben then
			local fuben_id = value2;
			if (true == temp_player:is_fuben_clearance(fuben_id)) then
				current_value = 1
			else
				current_value = 0;
            end
        elseif seven_day_goal_type == hld.e_service_goal_type_body_has_color_equip then
			local target_item_grade = value2;
			local target_item_color = value3;
			current_value = temp_player:get_item_set():get_equip_num_by_limit(target_item_grade, target_item_color);
		elseif seven_day_goal_type == hld.e_service_goal_type_legion_donate_num then
			current_value = temp_player:get_highest_record(hld.e_role_history_highest_record_battle_achievement);
		elseif seven_day_goal_type == hld.e_service_goal_type_soldier_rank_level then
			local current_level = temp_player:get_special_name_mgr():get_solider_rank_level();
			local current_star = temp_player:get_special_name_mgr():get_solider_rank_star();
			current_value = (current_level - 1) * 5 + current_star;
		elseif seven_day_goal_type == hld.e_service_goal_type_arena_rank_level then
			local target_rank_level = value1;
			local current_rank_level = temp_player:get_arena_mgr():current_rank();
			target_value = 1;
			if (current_rank_level < 0 or current_rank_level >= rank_max) then
				current_value = 0;
			elseif (current_rank_level <= target_rank_level) then
				current_value = 1;
            end
		elseif seven_day_goal_type == hld.e_service_goal_type_actived_illustration_num then
			target_value = value1;
			current_value = temp_player:get_pokedex_mgr():get_active_pokedex_num();
        elseif seven_day_goal_type == hld.e_service_goal_type_actived_any_chapter_illustration then
			current_value = temp_player:get_pokedex_mgr():get_all_active_pokedex_num();
        elseif seven_day_goal_type == hld.e_service_goal_type_demons_tower_layer_num then
			target_value = value1;
			current_value = temp_player:get_highest_record(hld.e_role_history_highest_record_demon_tower_highest_level);
        elseif seven_day_goal_type == hld.e_service_goal_type_achievement_title then
			target_value = value1;
			current_value = temp_player:get_special_name_mgr():get_achievement_title_level();
        elseif seven_day_goal_type == hld.e_service_goal_type_wing_level then
			local current_wing_grade = 0;
			local current_wing_level = 0;
			current_wing_grade,current_wing_level = temp_player:get_item_set():get_cur_wing_grade_and_level(current_wing_grade, current_wing_level);
			current_value = (current_wing_grade - 1) * 11 + current_wing_level;
		elseif seven_day_goal_type == hld.e_service_goal_type_element_heart_total_level then
			target_value = value1;
			current_value = temp_player:get_item_set():get_total_element_heart_level();
        elseif seven_day_goal_type == hld.e_service_goal_type_equiped_element_heart_num then
			target_value = value1;
			local element_heart_quality = value2;
			current_value = temp_player:get_element_heart_mgr():get_current_equipped_heart_num(element_heart_quality);
		elseif seven_day_goal_type == hld.e_service_goal_type_equiped_spirit_total_level then
			target_value = value1;
			current_value = temp_player:get_spirit_mgr():get_spirit_total_level();
        elseif seven_day_goal_type == hld.e_service_goal_type_equiped_spirit_num then
			target_value = value1;
			local m_condition_color = value2;
			current_value = temp_player:get_spirit_mgr():num_of_spirit_equiped(m_condition_color);
        elseif seven_day_goal_type == hld.e_service_goal_type_belief_rank then
			target_value = value1;
			local target_star = value2;
			current_value = temp_player:get_belief_mgr():get_cur_max_rank_value(target_star);
        elseif seven_day_goal_type == hld.e_service_goal_type_god_hood_grade then
			current_value = temp_player:get_logic_data(hld.e_role_logic_info_god_hood_grade);
        elseif seven_day_goal_type == hld.e_service_goal_type_seven_day_final then
			current_value = temp_player:get_seven_day_goal_mgr():get_specify_reward_num();
        end
		if (seven_day_goal_obj_ptr:cal_seven_day_goal_state(target_value, current_value)) then
			temp_player:get_seven_day_goal_mgr():send_seven_day_goal_one_to_self(seven_day_goal_obj_ptr);
        end
  
end

seven_day_goal = seven_day_goal_class

return seven_day_goal
