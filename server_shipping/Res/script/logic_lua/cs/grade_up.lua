local grade_up_class = {}
function grade_up_class:init()
end




define_C[10441] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.character_grade_up", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	grade_up_class:go_grade_up(player_ref,msg_data.grade_type,msg_data.class_change,msg_data.skill_template_id)
end

function grade_up_class:go_grade_up(player_ref,grade_type,class_change,skill_template_id)
    local game_config_template_ptr =template_manager:get_line("GameConfigTemplate",ConstDefine.game_config_template_id)
		if game_config_template_ptr == nil then
			return
		end
		local cur_level = player_ref:get_unit_info(hld.e_role_info_exp_level);
		if cur_level > tonumber(hld.init_unit:get_max_player_level()) then
			return;
		end
		if player_ref:get_unit_info(hld.e_role_info_class_branch) ~= 0 and class_change ~= 0 then
			return;
		end
		if player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade) / ConstDefine.player_grade_up_stage_nums >= hld.init_unit:get_max_grade_level() then
			return;
		end
		-- 当前的转数的角色模板
		local cur_player_template_id = player_ref:get_unit_info(hld.e_role_info_template_id);
		local cur_player_template_ptr = template_manager:get_line("PlayerInitTemplate",cur_player_template_id)
		if cur_player_template_ptr == nil then
			return;
		end

		-- 检查是否已达到最大转生数
		local t_cur_big_grade = Common:StrToTable(cur_player_template_ptr.GradeBranch)
		local cur_big_grade_size = #t_cur_big_grade 
		if cur_big_grade_size+ 1 ~= ConstDefine.player_grade_up_stage_nums then
			return;
		end
		if grade_type == 0 then
			--神格突破
			if player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade) % (cur_big_grade_size + 1) == cur_big_grade_size then
				return;
			end
			--神格突破
			grade_up_class:grade_god_hood(player_ref,player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade) % (cur_big_grade_size + 1) + 1, skill_template_id);
			return;
		elseif grade_type == 1 then
			--天启突破
			if player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade) % (cur_big_grade_size + 1) ~= cur_big_grade_size then		
				return;
			end
		else
			return;
		end
		if (player_ref:get_unit_info(hld.e_role_info_class_branch) ~= 0 and player_ref:get_unit_info(hld.e_role_info_class_branch) ~= class_change and class_change ~= 0) then
			return;
		end

		local new_player_template_id = cur_player_template_id + 1;
		local new_player_template_ptr = template_manager:get_line("PlayerInitTemplate",new_player_template_id)
		if new_player_template_ptr == nil then
			return;
		end
		--检查角色的等级是否达到了转生要求
		if cur_level < cur_player_template_ptr.LevelMax then
			return;
		end

		--检查人物是否有足够转生的财物

		--检查转职消耗的物品
		local t_cur_big_grade_GradeUpCostItem = Common:StrToTable(cur_player_template_ptr.GradeUpCostItem)
		local item_need_arr_len = #t_cur_big_grade_GradeUpCostItem
		if (item_need_arr_len > 0 and item_need_arr_len % 2 == 0) then
			local item_template_id = 0;
			local item_num = 0;

			for i=1,item_need_arr_len,2 do
				item_template_id = t_cur_big_grade_GradeUpCostItem[i];
				item_num = t_cur_big_grade_GradeUpCostItem[i + 1];
			end

			--物品是否足够

			local item_ref = player_ref:get_item_set();
			local have_item_num = item_ref:count_item_by_template_id(item_template_id, hld.e_bag_type_bag,-1);
			if have_item_num < item_num then
				return;
			end
		end

		--转职需要完成的任务

		local cur_grade_num = player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade);
		local temp_num = math.floor(cur_grade_num / ConstDefine.player_grade_up_one_mission_num);
		if temp_num == 0 then
			local mission_flag = hld.init_unit:get_left_shift_operation_result(1,cur_grade_num + 1)
			if hld.init_unit:get_bitwise_and_operation_result(mission_flag,player_ref:get_logic_data(hld.e_role_logic_info_done_grade_mission)) == 0 and new_player_template_ptr.GradeUpMission ~= 0 then
				return;
			end
		else
		
			local mission_flag = hld.init_unit:get_left_shift_operation_result(1,(cur_grade_num + 1) % ConstDefine.player_grade_up_one_mission_num)
			if (hld.init_unit:get_bitwise_and_operation_result(mission_flag,player_ref:get_logic_data(hld.e_role_logic_info_done_grade_mission_1 + temp_num - 1)) == 0 and new_player_template_ptr.GradeUpMission ~= 0) then
				return;
			end
		end

		--检查转职消耗的物品
		if item_need_arr_len > 0 and item_need_arr_len % 2 == 0 then
			local item_template_id = 0;
			local item_num = 0;

			for i = 1, item_need_arr_len, 2 do
				item_template_id = t_cur_big_grade_GradeUpCostItem[i];
				item_num = t_cur_big_grade_GradeUpCostItem[i + 1];
			end

			local item_ref = player_ref:get_item_set();
			local have_item_num = item_ref:count_item_by_template_id(item_template_id, hld.e_bag_type_bag,-1);
			if have_item_num < item_num then
				return;
			end
			item_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_grade_up, 0, item_template_id, item_num, 1);
		end

		local old_equip_buff_id = player_ref:get_item_set():get_equipment_upgrade_buff_id(false);
		player_ref:get_item_set():equip_off_upgrade_show_buff();	--去除装备显示光效buff

		--增加属性

		pawn_att_mgr:apply_att_change_by_array(player_ref,new_player_template_ptr.GradeUpProperty,true,1.0)

		local t_cur_big_grade_GradeUpTalent = Common:StrToTable(new_player_template_ptr.GradeUpTalent)

		if #(t_cur_big_grade_GradeUpTalent) >= 2 then
			local money_type = t_cur_big_grade_GradeUpTalent[1];
			local money_num = t_cur_big_grade_GradeUpTalent[2];
			player_ref:add_money_or_exp(money_type, money_num,hld.e_server_log_add_money_exp_level_up,cur_player_template_id);
			player_ref:send_money_one(money_type);
		end

		local old_class = player_ref:get_unit_info(hld.e_role_info_class_branch)
		--设置新的玩家信息
		player_ref:set_unit_info(hld.e_role_info_template_id, new_player_template_id);
		if class_change ~= 0 then
			player_ref:set_unit_info(hld.e_role_info_class_branch, class_change)
		end
	
		player_ref:broadcast_info_one(hld.e_role_info_template_id);
		player_ref:broadcast_info_one(hld.e_role_info_class_branch);

		-- mission check
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_character_level,0,0);

		-- 通给消息给WS(类似军团成员信息同步这类需求会用到)
		player_ref:sync_data_to_ws_lua(hld.e_sync_cs2ws_data_player_template, new_player_template_id,-1);


		player_ref:get_achievement_mgr():player_changed(hld.eat_role_transmigration_level_count);
		player_ref:get_element_heart_mgr():auto_get_equip_slot();
		--m_spirit_mgr.auto_get_equip_slot();
		player_ref:get_patron_saint_mgr():auto_get_equip_slot();

		--old_equip_buff_id;
		local new_equip_buff_id = player_ref:get_item_set():get_equipment_upgrade_buff_id(false);

		player_ref:get_item_set():equip_on_upgrade_show_buff();	--添加装备光效buff

		player_ref:set_logic_data(hld.e_role_logic_info_god_hood_grade, player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade) + 1);

		player_ref:send_logic_one(hld.e_role_logic_info_god_hood_grade);

		player_ref:refresh_service_goal(hld.e_service_goal_type_god_hood_grade);

		player_ref:send_grade_up_response(1, old_equip_buff_id, new_equip_buff_id,0);
		--角色转生公告
		player_ref:send_grade_up_notice(new_player_template_id);
		if player_ref:get_write_log() then
			local cur_role_class = player_ref:get_unit_info(hld.e_role_info_class_branch)
			hld.server_log:grade_up_log(player_ref:get_unit_info_inst(), old_class, cur_role_class);
		end

		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_grade_levelup,0,0);

		--设置巅峰 更新排行
		local GradeNum = player_ref:get_grade_num();
		if GradeNum >= ConstDefine.player_grade_up_peakedness_num then
			local level = player_ref:get_unit_info(hld.e_role_info_exp_level);
			player_ref:get_ranking_mgr():set_and_sync_ranking_data_str(hld.e_RankingIndex_level, tostring(level), -1, 1);		--更新排行榜数据
		end
end

function grade_up_class:grade_god_hood(player_ref,hood_index, skill_template_id)
	-- 当前的转数的角色模板
		local cur_player_template_id = player_ref:get_unit_info(hld.e_role_info_template_id);
		local cur_player_template_ptr = template_manager:get_line("PlayerInitTemplate",cur_player_template_id)
		if cur_player_template_ptr == nil then
			return;
		end
		local t_cur_player_template_ptr = Common:StrToTable(cur_player_template_ptr.GradeBranch)
		if hood_index > #t_cur_player_template_ptr or hood_index <= 0 then
			return;
		end
		local m_grade_god_hood_id = t_cur_player_template_ptr[hood_index]
		local grade_god_hood_template_ptr = template_manager:get_line("GradeGodHoodTemplate",m_grade_god_hood_id);
		if grade_god_hood_template_ptr == nil then
			return;
		end
		--检查等级
		local cur_level = player_ref:get_unit_info(hld.e_role_info_exp_level);
		if cur_level < grade_god_hood_template_ptr.LevelMax then
			return;
		end
		--检查转职消耗的物品
		local t_grade_god_hood_GradeUpCostItem = Common:StrToTable(grade_god_hood_template_ptr.GradeUpCostItem)
		local item_need_arr_len = #t_grade_god_hood_GradeUpCostItem;
		if item_need_arr_len > 0 and item_need_arr_len % 2 == 0 then
			local item_template_id = 0;
			local item_num = 0;
			for i = 1,item_need_arr_len, 2 do
				item_template_id = t_grade_god_hood_GradeUpCostItem[i];
				item_num = t_grade_god_hood_GradeUpCostItem[i + 1];
			end
			local have_item_num = player_ref:get_item_set():count_item_by_template_id(item_template_id, hld.e_bag_type_bag,-1);
			if have_item_num < item_num then
				return;
			end

		end
		--转职需要完成的任务

		local cur_grade_num = player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade);
		local temp_num = math.floor(cur_grade_num / ConstDefine.player_grade_up_one_mission_num)
		if temp_num == 0 then
			local mission_flag = hld.init_unit:get_left_shift_operation_result(1,cur_grade_num + 1);
			if hld.init_unit:get_bitwise_and_operation_result(mission_flag,player_ref:get_logic_data(hld.e_role_logic_info_done_grade_mission)) == 0 and grade_god_hood_template_ptr.GradeUpMission ~= 0 then
				return;
			end
		else
			local mission_flag =hld.init_unit:get_left_shift_operation_result(1,((cur_grade_num + 1) % ConstDefine.player_grade_up_one_mission_num));
			if hld.init_unit:get_bitwise_and_operation_result(mission_flag,player_ref:get_logic_data(hld.e_role_logic_info_done_grade_mission_1 + temp_num - 1)) == 0 and grade_god_hood_template_ptr.GradeUpMission ~= 0 then
				return;
			end
		end
		--检查转职消耗的物品
		if item_need_arr_len > 0 and item_need_arr_len % 2 == 0 then
			local item_template_id = 0;
			local item_num = 0;

			for i=1,item_need_arr_len,2 do
				item_template_id = t_grade_god_hood_GradeUpCostItem[i];
				item_num = t_grade_god_hood_GradeUpCostItem[i + 1];
			end
			--物品是否足够
			local have_item_num = player_ref:get_item_set():count_item_by_template_id(item_template_id, hld.e_bag_type_bag,-1);
			if have_item_num < item_num then
				return;
			end
			player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_grade_up, 0, item_template_id, item_num, 1);
		end
		--激活技能
		--激活分支技能
		local t_grade_god_hood_GradeUpSkills = Common:StrToTable(grade_god_hood_template_ptr.GradeUpSkills)
		if skill_template_id ~= 0 then
			local temp_skill_set = hld.skill_manager:get_instance():get_skill_set(player_ref:get_array_index());
			for i = 1,#t_grade_god_hood_GradeUpSkills do
				if skill_template_id == t_grade_god_hood_GradeUpSkills[i] then
					delog(skill_template_id)	
					temp_skill_set:activate_skill(skill_template_id, true)
				end
			end
		end

		--增加属性

		pawn_att_mgr:apply_att_change_by_array(player_ref,grade_god_hood_template_ptr.GradeUpProperty,true,1.0)

		player_ref:set_logic_data(hld.e_role_logic_info_god_hood_grade, player_ref:get_logic_data(hld.e_role_logic_info_god_hood_grade) + 1);

		player_ref:send_logic_one(hld.e_role_logic_info_god_hood_grade);

		player_ref:refresh_service_goal(hld.e_service_goal_type_god_hood_grade);

		--添加属性天赋
		local t_grade_god_hood_GradeUpTalent = Common:StrToTable(grade_god_hood_template_ptr.GradeUpTalent)

		if #t_grade_god_hood_GradeUpTalent >= 2 then
			local money_type = t_grade_god_hood_GradeUpTalent[1];
			local money_num = t_grade_god_hood_GradeUpTalent[2];
			player_ref:add_money_or_exp(money_type, money_num,hld.e_server_log_add_money_exp_level_up,m_grade_god_hood_id);
			player_ref:send_money_one(money_type);
		end
		local old_equip_buff_id = player_ref:get_item_set():get_equipment_upgrade_buff_id(false);
		local new_equip_buff_id = player_ref:get_item_set():get_equipment_upgrade_buff_id(false);
		player_ref:send_grade_up_response(1, old_equip_buff_id, new_equip_buff_id, skill_template_id);
end

grade_up = grade_up_class

return grade_up