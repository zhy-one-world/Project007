local belief={}
local function_array ={}
local belief_record_type={}
function belief:init()
	function_array[0]= self.belief_upgrade
	function_array[1]= self.belief_upgrade
	belief_record_type[0] = hld.e_role_history_highest_record_belief_spring
	belief_record_type[1] = hld.e_role_history_highest_record_belief_summer
	belief_record_type[2] = hld.e_role_history_highest_record_belief_autumn
	belief_record_type[3] = hld.e_role_history_highest_record_belief_winter
end
define_C[10901] = function(message_data, player_ref)
	local msg_data = decode("hld.belief_proto.belief_operation", message_data)
	belief:belief_operate(player_ref, msg_data.operation_type, msg_data.belief_type);
end
function belief:belief_operate(player_ref, operation_type, belief_type)
	if nil == player_ref then
		return;
	end
	if nil == function_array[operation_type] then
		return;
	end
	local end_state = function_array[operation_type](self, player_ref, belief_type);
	if end_state == ConstDefine.e_belief_string_ji_huo_succeed then
		player_ref:get_belief_mgr():sync_ranking_belief_data();
	end
	local temp_cur_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_upgrade_one_belief); --活跃度
	player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_upgrade_one_belief, temp_cur_value + 1);
	local belief_msg = {};
	belief_msg.role_guid = {};
	table.insert(belief_msg.role_guid, player_ref:get_unit_guid_A());
	table.insert(belief_msg.role_guid, player_ref:get_unit_guid_B());
	belief_msg.belief_type = belief_type
	belief_msg.operation_type = operation_type
	belief_msg.operation_state = end_state
	MessageMgr:send_message_to_self(player_ref, "hld.belief_proto.belief_operation_end",belief_msg, 10902)
	player_ref:refresh_service_goal(hld.e_service_goal_type_belief_rank);
	if end_state > 0 then
		--self:send_belief_notice(belief_type);
	end
end
function belief:belief_upgrade(player_ref, belief_type)
	local end_state = ConstDefine.e_item_string_unkown;
	if (player_ref == nil)then
		return end_state;
	end
	if (belief_type < hld.e_belief_series_type_spring or belief_type >= hld.e_belief_series_type_max)then
		return end_state;
	end
	local belief_ptr = player_ref:get_belief_mgr():getbelief(belief_type);
	if nil == belief_ptr then
		return end_state;
	end
	local belief_template_ptr = template_manager:get_line("BeliefTemplate", belief_ptr:get_data_info(hld.e_belief_info_template_id) + belief_ptr:get_data_info(hld.e_belief_info_upgrade_count));
	if (belief_template_ptr == nil)then
		return end_state;
	end
	local player_level = player_ref:get_unit_info(hld.e_role_info_exp_level);
	local level_limit = belief_template_ptr.ActivateLevelRequire;
	if (level_limit > player_level)then
		return ConstDefine.e_item_string_level;
	end

	if (belief_template_ptr.OperateType == hld.e_belief_upgrade_type_none)then
		return end_state;
	end
	local money_cost_list = belief_template_ptr.MoneyCostList;
	local money_cost_list_num = #(money_cost_list);
	if math.fmod(money_cost_list_num, 2) ~= 0 then
		return end_state;
	end

	for i = 1, money_cost_list_num, 2 do
		local money_id = tonumber(money_cost_list[i]);
		local money_value = tonumber(money_cost_list[i + 1]);
		if (false == player_ref:can_cut_money(money_id, money_value)) then
			return ConstDefine.e_item_string_money;
		end
	end

	for i = 1, money_cost_list_num, 2 do
		local money_id = tonumber(money_cost_list[i]);
		local money_value = tonumber(money_cost_list[i + 1]);
		player_ref:cut_money(money_id, money_value, hld.e_server_log_cut_money_belief_upgrade_up, belief_ptr:get_data_info(hld.e_belief_info_template_id), belief_type);
	end

	local activate_rate = hld.random_gen:get_random(0, 1000);
	local target_rate = belief_template_ptr.SuccessRatio * 1000;
	local try_times = belief_ptr:get_data_info(hld.e_belief_info_try_upgrade_times);

	local is_success = false;
	if (try_times >= belief_template_ptr.MaxActivateTimeLimit)then
		is_success = true;
	elseif(try_times < belief_template_ptr.MinActivateTimeNeed)then
		is_success = false;
	else
		is_success = activate_rate <= target_rate;
	end

	if (is_success)then
		pawn_att_mgr:apply_att_change_by_array(player_ref, belief_template_ptr.AttArray, false, 1)
		belief_ptr:set_data_info(hld.e_belief_info_try_upgrade_times, 0);
		local cur_upgrade_num = belief_ptr:get_data_info(hld.e_belief_info_upgrade_count);
		belief_ptr:set_data_info(hld.e_belief_info_upgrade_count, cur_upgrade_num + 1);
		belief_ptr:refresh_template();
		belief_template_ptr = template_manager:get_line("BeliefTemplate", belief_ptr:get_data_info(hld.e_belief_info_template_id) + belief_ptr:get_data_info(hld.e_belief_info_upgrade_count));
		if (belief_template_ptr) then
			pawn_att_mgr:apply_att_change_by_array(player_ref, belief_template_ptr.AttArray, true, 1)
			local record_type = belief_record_type[belief_type];
			if (record_type >= 0 and record_type < hld.e_role_history_highest_record_max) then
				player_ref:check_and_set_highest_record(record_type, belief_template_ptr.CurRankNumber, 0);
			end
		end
		player_ref:get_achievement_mgr():player_changed(hld.eat_belief_grade_count);
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_belief_all_num,0,0);
		end_state = ConstDefine.e_belief_string_ji_huo_succeed;
	else
		belief_ptr:set_data_info(hld.e_belief_info_try_upgrade_times, try_times + 1);
		end_state = ConstDefine.e_belief_string_ji_huo_fail;
	end

	self:send_belief_one(player_ref, belief_ptr, belief_type);
	return end_state;
end
function belief:send_belief_one(player_ref, belief_ptr, belief_type)
	local  belief_msg = {};
	belief_msg.role_guid = {};
	table.insert(belief_msg.role_guid, player_ref:get_unit_guid_A());
	table.insert(belief_msg.role_guid, player_ref:get_unit_guid_B());
	belief_msg.belief_type = belief_type;
	belief_msg.belief_list = {};
	local belief_array = {}
	belief_array.belief_data = {}
	for i = 0, hld.e_belief_info_max -1 do
		table.insert(belief_array.belief_data, belief_ptr:get_data_info(i));
	end
	table.insert(belief_msg.belief_list, belief_array);
	MessageMgr:send_message_to_aoi_lua(player_ref, "hld.belief_proto.update_character_belief_one",belief_msg, 10904, true, false)
end
function belief:get_belief_level(player_ref, belief_type)
	if (belief_type < hld.e_belief_series_type_spring or belief_type >= hld.e_belief_series_type_max) then
		return 0;
	end
	if (nil == player_ref) then
		return 0;
	end
	local belief_ptr = player_ref:get_belief_mgr():getbelief(belief_type);
	if nil == belief_ptr then
		return 0;
	end
	local belief_template_ptr = template_manager:get_line("BeliefTemplate", belief_ptr:get_data_info(hld.e_belief_info_template_id) + belief_ptr:get_data_info(hld.e_belief_info_upgrade_count));
	if nil == belief_template_ptr then
		return 0;
	end
	return belief_template_ptr.CurRankNumber;
end
belief_mgr = belief
return belief_mgr
