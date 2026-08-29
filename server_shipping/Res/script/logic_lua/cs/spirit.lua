local spirit={}
local function_array ={}
function spirit:init()
	function_array[0]= self.fight_spirit
	function_array[2]= self.send_to_itembag
	function_array[3]= self.upgrade_spirit
	function_array[5]= self.send_to_itembag
	function_array[8]= self.purchase_equip_slot
	function_array[11]= self.star_upgrade
end
define_C[11101] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.spirit_operation", message_data)
	spirit:spirit_operate(player_ref, msg_data.operation_type, msg_data.spirit_guid, msg_data.target_slot);
end
define_C[10742] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_forge", message_data)
	if msg_data.is_save_operate == true then
		spirit:spirit_save_forge(player_ref, msg_data.item_guid);
	else
		local use_locked_item = 0
		if msg_data.first_use_locked_item == true then
			use_locked_item = 1
		end
		spirit:spirit_forge(player_ref, msg_data.item_guid, msg_data.forge_lock_flag, use_locked_item);
	end
end
define_C[10744] = function(message_data, player_ref)
	local msg_data = decode("hld.item_proto.item_forge", message_data)
	spirit:spirit_forge_begin(player_ref, msg_data.item_guid);
end
function spirit:spirit_operate(player_ref, operation_type, spirit_guid, target_slot)
	if nil == player_ref then
		return;
	end
	if nil == function_array[operation_type] then
		return;
	end
	local operate_end_msg = {};
	operate_end_msg.role_guid ={}
	table.insert(operate_end_msg.role_guid, player_ref:get_unit_guid_A())
	table.insert(operate_end_msg.role_guid, player_ref:get_unit_guid_B())
	operate_end_msg.spirit_guid = spirit_guid
	operate_end_msg.operation_type = operation_type
	local end_state = function_array[operation_type](self, player_ref, spirit_guid, target_slot);
	if end_state == 0 then
		player_ref:get_item_set():sync_ranking_spirit();
	end
	operate_end_msg.operation_result = end_state;
	operate_end_msg.target_operate_slot = target_slot;
	MessageMgr:send_message_to_self(player_ref, "hld.character_proto.spirit_operation_end",operate_end_msg, 11104)
end
function spirit:fight_spirit(player_ref, spirit_guid)

	if (player_ref == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local target_sprite_item = player_ref:get_item_set():get_item(spirit_guid[1], spirit_guid[2]);
	if (nil == target_sprite_item)then
		return ConstDefine.e_item_string_unkown;
	end
	if (target_sprite_item:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_equip_spirit)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(target_sprite_item);
	if (nil == item_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end
	local spirit_template_id = item_template_ptr.logic_id + target_sprite_item:get_data_info(hld.e_item_info_upgrade_count) - 1;
	local spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id);
	if (spirit_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end

	local fight_add_bonus = spirit_template_ptr.FightAddBonus;
	local old_fight_sprite_item = self:get_fighting_spirit(player_ref)

	local old_item_template_ptr = server_tool_mgr:get_item_template(old_fight_sprite_item);
	if (old_fight_sprite_item ~= nil and old_item_template_ptr ~= nil)then
		old_fight_sprite_item:set_data_info(hld.e_item_info_activate, 0);
		local spirit_template_id = old_item_template_ptr.logic_id + old_fight_sprite_item:get_data_info(hld.e_item_info_upgrade_count) - 1;
		local old_spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id);
		if (old_spirit_template_ptr == nil)then
			return ConstDefine.e_item_string_unkown;
		end
		local star_upgrade_template_id = old_spirit_template_ptr.StarGradeTemplateID + old_fight_sprite_item:get_data_info(hld.e_item_info_stargrade_count);
		local star_upgrade_template_ptr = template_manager:get_line("SpiritTemplate", star_upgrade_template_id);
		if (star_upgrade_template_ptr == nil)then
			return ConstDefine.e_item_string_unkown;
		end
		local add_percent = tonumber(fight_add_bonus[1]) - tonumber(fight_add_bonus[2]);
		player_ref:get_spirit_mgr():operate_basic_att(old_fight_sprite_item, false, add_percent);
		pawn_att_mgr:set_excellent_att_array(player_ref, old_spirit_template_ptr.BaseExcellentAtt, old_fight_sprite_item, false, add_percent);
		pawn_att_mgr:apply_att_change_by_array(player_ref, star_upgrade_template_ptr.BasicProperty, false, add_percent);
		--buff_man::get_instance():del_buff_by_template_id(player_ref:get_array_index(), player_ref:get_array_index(), old_fight_sprite.get_spirit_info_ptr().SpiritDisplayBuffID);
		player_ref:get_item_set():send_item_one(old_fight_sprite_item, true, 0);
		if spirit_guid[1] == old_fight_sprite_item:get_item_guid_A() and spirit_guid[2] == old_fight_sprite_item:get_item_guid_B() then
			return 0;
		end
	end
	target_sprite_item:set_data_info(hld.e_item_info_activate, 1);
	local star_upgrade_template_id = spirit_template_ptr.StarGradeTemplateID + target_sprite_item:get_data_info(hld.e_item_info_stargrade_count);
	local star_upgrade_template_ptr = template_manager:get_line("SpiritTemplate", star_upgrade_template_id);
	if (star_upgrade_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local add_percent = tonumber(fight_add_bonus[1]) - tonumber(fight_add_bonus[2]);
	player_ref:get_spirit_mgr():operate_basic_att(target_sprite_item, true, add_percent);
	pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, target_sprite_item, true, add_percent);
	pawn_att_mgr:apply_att_change_by_array(player_ref, star_upgrade_template_ptr.BasicProperty, true, add_percent);
	player_ref:get_item_set():send_item_one(target_sprite_item, true, 0);

	return 0;
end
function spirit:upgrade_spirit(player_ref, spirit_guid)
	if (nil == player_ref)then
		return ConstDefine.e_item_string_unkown;
	end
	local temp_spirit_item = player_ref:get_item_set():get_item(spirit_guid[1], spirit_guid[2]);
	if (nil == temp_spirit_item)then
		return ConstDefine.e_item_string_unkown;
	end
	if (temp_spirit_item:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_equip_spirit)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(temp_spirit_item);
	if (nil == item_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end
	local spirit_template_id = item_template_ptr.logic_id + temp_spirit_item:get_data_info(hld.e_item_info_upgrade_count) - 1;
	local spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id);
	if (nil == spirit_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end
	if (false == player_ref:can_cut_money(hld.e_money_type_spirit_crystal, spirit_template_ptr.ExpPerUpgrade))then
		return ConstDefine.e_buy_goods_end_state_money;
	end
	local temp_cur_value = player_ref:get_welfare_mgr():get_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_spirit); -- 活跃度
	player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_strengthing_one_spirit, temp_cur_value + 1);

	local next_sprite_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id + 1);

	if (next_sprite_template_ptr == nil or spirit_template_ptr.Level + 1 ~= next_sprite_template_ptr.Level or spirit_template_ptr.Type ~= next_sprite_template_ptr.Type)then
		return ConstDefine.e_item_string_full_level;
	end

	player_ref:cut_money(hld.e_money_type_spirit_crystal, spirit_template_ptr.ExpPerUpgrade, hld.e_server_log_cut_money_sprite_upgrade, temp_spirit_item:get_item_template_id(), 0);
	local cur_exp = temp_spirit_item:get_data_info(hld.e_item_info_wing_exp);
	if (cur_exp + spirit_template_ptr.ExpPerUpgrade < spirit_template_ptr.UpgradeCost)then
		temp_spirit_item:set_data_info(hld.e_item_info_wing_exp, cur_exp + spirit_template_ptr.ExpPerUpgrade);
		player_ref:get_item_set():send_item_one(temp_spirit_item, true, 0);
		return 0;
	else
		local temp_exp = cur_exp + spirit_template_ptr.ExpPerUpgrade - spirit_template_ptr.UpgradeCost;
		temp_spirit_item:set_data_info(hld.e_item_info_wing_exp, temp_exp);
	end

	local fight_add_bonus = spirit_template_ptr.FightAddBonus;
	if (temp_spirit_item:get_data_info(hld.e_item_info_activate) > 0)then
		player_ref:get_spirit_mgr():operate_basic_att(temp_spirit_item, false, tonumber(fight_add_bonus[1]));
	else
		player_ref:get_spirit_mgr():operate_basic_att(temp_spirit_item, false, tonumber(fight_add_bonus[2]));
	end

	temp_spirit_item:set_data_info(hld.e_item_info_upgrade_count, temp_spirit_item:get_data_info(hld.e_item_info_upgrade_count) + 1);

	local next_fight_add_bonus = next_sprite_template_ptr.FightAddBonus;
	if (temp_spirit_item:get_data_info(hld.e_item_info_activate) > 0)then
		player_ref:get_spirit_mgr():operate_basic_att(temp_spirit_item, true, tonumber(next_fight_add_bonus[1]));
	else
		player_ref:get_spirit_mgr():operate_basic_att(temp_spirit_item, true, tonumber(next_fight_add_bonus[2]));
	end


	player_ref:get_item_set():send_item_one(temp_spirit_item, true, 0);
	player_ref:check_and_set_highest_record(hld.e_role_history_highest_record_total_sprite_equip_level, 0, 0);
	player_ref:get_achievement_mgr():player_changed(hld.eat_spirit_level_count); --成就

	player_ref:get_spirit_mgr():set_qiyuan_att_all();
	player_ref:get_spirit_mgr():set_jiban_att_all();
	
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_spirit_upgrade_num, 0, 0)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_raise_sprite, 0);
	--杀怪数三个都存吧……
	-- 精灵升到满级公告
	if (next_sprite_template_ptr.Noticeld > 0)then
		local notice_data = {};
		notice_data.notice_id = next_sprite_template_ptr.Notice_id--93000032
		notice_data.item_data = {}
		notice_data.num_type_data = {}
		table.insert(notice_data.item_data, temp_spirit_item:get_item_inst());
		table.insert(notice_data.num_type_data, next_sprite_template_ptr.Level);
		server_tool_mgr:send_notice(player_ref, notice_data);
	end
	player_ref:refresh_service_goal(hld.e_service_goal_type_equiped_spirit_total_level);
	return 0;
end
function spirit:purchase_equip_slot(player_ref, spirit_guid, slot_index)
	if (nil == player_ref)then
		return ConstDefine.e_item_string_unkown;
	end
	if (slot_index >= 200 or slot_index < 0)then
		return ConstDefine.e_item_string_unkown;
	end
	--配置检测
	local game_config_template_ptr = server_tool_mgr:get_game_config_template_ptr();
	if (game_config_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local sprite_equip_slot_purchase_array = game_config_template_ptr.SpriteEquipSlotPurchaseArray;
	local sprite_equip_slot_purchase_array_num = #(sprite_equip_slot_purchase_array);
	if (math.fmod(sprite_equip_slot_purchase_array_num, 2) ~= 0 or slot_index >= math.floor(sprite_equip_slot_purchase_array_num / 2))then
		return ConstDefine.e_item_string_unkown;
	end
	local sprite_equip_slot_open_grade_req_array = game_config_template_ptr.SpriteEquipSlotOpenGradeReqArray;
	local sprite_equip_slot_open_grade_req_array_num = #(sprite_equip_slot_open_grade_req_array);
	if (slot_index >= sprite_equip_slot_open_grade_req_array_num)then
		return ConstDefine.e_item_string_unkown;
	end
	if (player_ref:get_spirit_mgr():check_equip_slot_is_unlock(slot_index) == true)then
		return ConstDefine.e_item_string_unkown;
	end
	if (player_ref:get_grade_num() < tonumber(sprite_equip_slot_open_grade_req_array[slot_index + 1]))then
		return ConstDefine.e_item_string_level;
	end
	local need_material_id = tonumber(sprite_equip_slot_purchase_array[slot_index*2 + 1]);
	local need_material_num = tonumber(sprite_equip_slot_purchase_array[slot_index*2 + 2]);
	if (player_ref:get_item_set():count_item_by_template_id(need_material_id, hld.e_bag_type_bag, -1) < need_material_num)then
		return ConstDefine.e_item_string_matirial_not_enough;
	end
	player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_unlock_spirit_slot, 0, need_material_id, need_material_num, 1);

	player_ref:get_spirit_mgr():unlock_equip_slot(slot_index);

	player_ref:set_unit_info(hld.e_role_info_sprite_equip_slot_open_num, player_ref:get_unit_info(hld.e_role_info_sprite_equip_slot_open_num) + 1);
	player_ref:send_info_one(hld.e_role_info_sprite_equip_slot_open_num);

	return 0;
end
function spirit:star_upgrade(player_ref, spirit_guid, use_lock_first)
	if (nil == player_ref)then
		return ConstDefine.e_item_string_unkown;
	end
	local temp_spirit_item = player_ref:get_item_set():get_item(spirit_guid[1], spirit_guid[2]);
	local matirial_count = 0;
	if (nil == temp_spirit_item)then
		return ConstDefine.e_item_string_unkown;
	end
	if (temp_spirit_item:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_equip_spirit)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(temp_spirit_item);
	local spirit_template_id = item_template_ptr.logic_id + temp_spirit_item:get_data_info(hld.e_item_info_upgrade_count) - 1;
	local spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id);
	if (spirit_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local spirit_star_grade = temp_spirit_item:get_data_info(hld.e_item_info_stargrade_count);
	local spirit_starupgrade_template_id = spirit_template_ptr.StarGradeTemplateID + spirit_star_grade;
	local spirit_starupgrade_template_ptr = template_manager:get_line("SpiritTemplate", spirit_starupgrade_template_id);
	if (spirit_starupgrade_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end

	if ((spirit_starupgrade_template_ptr.StarGradeNow >= spirit_starupgrade_template_ptr.StarGradeMax))then
		return ConstDefine.e_item_string_full_level;
	end
	if (spirit_starupgrade_template_ptr.GradeLimit > player_ref:get_grade_num())then
		return ConstDefine.e_item_string_unkown;
	end
	matirial_count = player_ref:get_item_set():count_item_by_template_id(spirit_starupgrade_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, -1);
	if (matirial_count < spirit_starupgrade_template_ptr.UpgradeMatirialNum)then--素材不足
		return ConstDefine.e_item_string_matirial_not_enough;
	end
	if (temp_spirit_item:get_data_info(hld.e_item_info_locked) <= 0)then
		local lock_item_num = player_ref:get_item_set():count_item_by_template_id(spirit_starupgrade_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, hld.e_item_locked);
		local unlock_item_num = player_ref:get_item_set():count_item_by_template_id(spirit_starupgrade_template_ptr.UpgradeMatirialId, hld.e_bag_type_bag, hld.e_item_unlocked);
		if (use_lock_first > 0 and lock_item_num > 0)then
			temp_spirit_item:set_data_info(hld.e_item_info_locked, hld.e_item_locked);
		end
		if (use_lock_first <= 0 and unlock_item_num < spirit_starupgrade_template_ptr.UpgradeMatirialNum)then
			return ConstDefine.e_item_string_matirial_not_enough;
		end
	end
	player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_put_in_bag_not_create, 0, spirit_starupgrade_template_ptr.UpgradeMatirialId, spirit_starupgrade_template_ptr.UpgradeMatirialNum, use_lock_first);

	--升星
	temp_spirit_item:set_data_info(hld.e_item_info_stargrade_count, spirit_star_grade + 1);
	local fight_add_bonus = spirit_starupgrade_template_ptr.FightAddBonus;
	if (temp_spirit_item:get_data_info(hld.e_item_info_activate) > 0)then
		pawn_att_mgr:apply_att_change_by_array(player_ref, spirit_starupgrade_template_ptr.BasicProperty, false, tonumber(fight_add_bonus[1]));
	else
		pawn_att_mgr:apply_att_change_by_array(player_ref, spirit_starupgrade_template_ptr.BasicProperty, false, tonumber(fight_add_bonus[2]));
	end
	local next_starupgrade_template_id = spirit_starupgrade_template_id + 1;
	local next_starupgrade_template_ptr = template_manager:get_line("SpiritTemplate", next_starupgrade_template_id);
	if (next_starupgrade_template_ptr)then
		if (temp_spirit_item:get_data_info(hld.e_item_info_activate) > 0)then
			pawn_att_mgr:apply_att_change_by_array(player_ref, next_starupgrade_template_ptr.BasicProperty, true, tonumber(fight_add_bonus[1]));
		else
			pawn_att_mgr:apply_att_change_by_array(player_ref, next_starupgrade_template_ptr.BasicProperty, true, tonumber(fight_add_bonus[2]));
		end

		--被动技能
		local passive_skill_template_id_array = next_starupgrade_template_ptr.StarBuffId;
		for i=1,#passive_skill_template_id_array,1 do
			local passive_skill_template_ptr = template_manager:get_line("PassiveSkillTemplate",  passive_skill_template_id_array[i]);
			if (passive_skill_template_ptr and passive_skill_template_ptr.OpenItem.size() < 2) then
				player_ref:get_item_set():star_skill_unlock(spirit_guid, next_starupgrade_template_ptr.StarGradeNow, false);
			end
		end
	else
		temp_spirit_item:set_data_info(hld.e_item_info_stargrade_count, spirit_star_grade);

		if (temp_spirit_item:get_data_info(hld.e_item_info_activate) > 0)then
			player_ref:get_item_set():item_change_att(spirit_starupgrade_template_ptr.BasicProperty, tonumber(fight_add_bonus[1]), true);
		else
			player_ref:get_item_set():item_change_att(spirit_starupgrade_template_ptr.BasicProperty, tonumber(fight_add_bonus[2]), true);
		end
	end
	player_ref:get_item_set():send_item_one(temp_spirit_item, true, 0);
	--记录精灵觉醒事件
	player_ref:get_time_limit_activity_mgr():activity_behavior_done(hld.e_time_limit_behavior_type_telves_awaken, 1);
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_raise_sprite, 0);
	return 0;
end

function spirit:send_to_itembag(player_ref, spirit_guid)
	if (player_ref == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_ptr = player_ref:get_item_set():get_item(spirit_guid[1], spirit_guid[2]);
	if (item_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	if (item_ptr:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_equip_spirit)then
		return ConstDefine.e_item_string_unkown;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr);
	if (nil == item_template_ptr)then
		return ConstDefine.e_item_string_unkown;
	end
	--除了删除精灵包里的，还要在物品里加个东西
	local spirit_template_id = item_template_ptr.logic_id + item_ptr:get_data_info(hld.e_item_info_upgrade_count) - 1;
	local spirit_template_ptr = template_manager:get_line("SpiritTemplate", spirit_template_id);
	if (spirit_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end
	local star_upgrade_template_id = spirit_template_ptr.StarGradeTemplateID + item_ptr:get_data_info(hld.e_item_info_stargrade_count);
	local star_upgrade_template_ptr = template_manager:get_line("SpiritTemplate", star_upgrade_template_id);
	if (star_upgrade_template_ptr == nil)then
		return ConstDefine.e_item_string_unkown;
	end

	if (player_ref:get_item_set():get_empty_slot_num(hld.e_bag_type_bag) < 1)then
		return ConstDefine.e_item_string_bag_full;
	end
	local empty_slot = player_ref:get_item_set():get_empty_slot(hld.e_bag_type_bag);
	local temp_level = spirit_template_ptr.Level;

	local fight_add_bonus = spirit_template_ptr.FightAddBonus;
	if (item_ptr:get_data_info(hld.e_item_info_activate) > 0)then
		player_ref:get_spirit_mgr():operate_basic_att(item_ptr, false, tonumber(fight_add_bonus[1]));
		pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, item_ptr, false, tonumber(fight_add_bonus[1]));
		pawn_att_mgr:apply_att_change_by_array(player_ref, star_upgrade_template_ptr.BasicProperty, false, tonumber(fight_add_bonus[1]));
	else
		player_ref:get_spirit_mgr():operate_basic_att(item_ptr, false, tonumber(fight_add_bonus[2]));
		pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, item_ptr, false, tonumber(fight_add_bonus[2]));
		pawn_att_mgr:apply_att_change_by_array(player_ref, star_upgrade_template_ptr.BasicProperty, false, tonumber(fight_add_bonus[2]));
	end

	local same_spirit_num = 0;

	for spirit_slot = 0, 200 do
		local equip_spirit_item = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_equip_spirit, spirit_slot);
		if (equip_spirit_item ~= nil)then
			if (equip_spirit_item:get_data_info(hld.e_item_info_info_id) == item_ptr:get_data_info(hld.e_item_info_info_id))then
				same_spirit_num = same_spirit_num + 1;
			end
		end
	end
	if (same_spirit_num == 1)then
		player_ref:get_passive_skill():passive_skill_special(item_ptr:get_data_info(hld.e_item_info_succinct_property2), false);
		player_ref:get_passive_skill():passive_skill_special(item_ptr:get_data_info(hld.e_item_info_succinct_property3), false);
	end

	item_ptr:set_data_info(hld.e_item_info_activate, 0);
	item_ptr:set_data_info(hld.e_item_info_prototype_id, item_template_ptr.logic_id);
	player_ref:get_item_set():send_spirit_msg(item_ptr,true);

	player_ref:get_item_set():put_item_into_bag(item_ptr);
	player_ref:get_item_set():sync_ranking_spirit();
	player_ref:get_spirit_mgr():set_qiyuan_att_all();
	player_ref:get_spirit_mgr():set_jiban_att_all();
	return 0;
end
function spirit:get_fighting_spirit(player_ref)
	if (player_ref == nil) then
		return nil;
	end
	for spirit_slot = 0, 200 do
		local item_ptr = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_equip_spirit, spirit_slot);
		if (item_ptr ~= nil and item_ptr:get_data_info(hld.e_item_info_activate) > 0) then
			return item_ptr;
		end
	end
	return nil;
end
function spirit:get_spirit_template_ptr(item_ptr)
	if nil == item_ptr then
		return nil;
	end
	local item_template_ptr = server_tool_mgr:get_item_template(item_ptr);
	if (nil == item_template_ptr)then
		return nil;
	end
	--除了删除精灵包里的，还要在物品里加个东西
	local spirit_template_id = item_template_ptr.logic_id + item_ptr:get_data_info(hld.e_item_info_upgrade_count) - 1;
	return template_manager:get_line("SpiritTemplate", spirit_template_id);
end
function spirit:send_forge_message(player_ref, item_ptr, is_save_operate, in_is_enabled)
	--发送消息
	local msg = {};
	msg.property_name = {}
	msg.property_value = {}
	msg.property_index = {}
	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property5 do
		local forge_property = item_ptr:get_data_info(i);
		if forge_property > 0 then
			local forge_name_flag_att = item_ptr:get_forge_name_flag_att(i);
			local forge_name_att = item_ptr:get_forge_name_att(i);
			local forge_att = item_ptr:get_forge_att(i);
			table.insert(msg.property_name, forge_name_att);
			table.insert(msg.property_value, forge_att);
			table.insert(msg.property_index, forge_name_flag_att);
			delog("---i----pos---type---value---", i, forge_name_flag_att, forge_name_att ,forge_att)
		end
	end
	local change_gs =  tonumber(item_ptr:get_forge_change_gs(player_ref:get_array_index()));
	msg.change_gs_value = change_gs;
	msg.is_save_operate = is_save_operate;
	msg.in_is_enabled = in_is_enabled;
	MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_forge_end",msg, 10743)
end

function spirit:spirit_forge_begin(temp_player, item_guid)

	if temp_player == nil then
		return hld.e_error_code_no_template
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end
	
	local temp_item = item_set_ref:get_item(item_guid[1], item_guid[2])
	if temp_item == nil then
		return hld.e_error_code_no_template
	end

	local total_att_num = 0
	local spirit_template_ptr = self:get_spirit_template_ptr(temp_item);

	local BaseExcellentAtt = spirit_template_ptr.BaseExcellentAtt;
	local BaseExcellentAtt_num = #(BaseExcellentAtt);
	local GreenExcellentRatio = spirit_template_ptr.GreenExcellentRatio;
	local GreenExcellentRatio_num = #(GreenExcellentRatio);
	local BlueExcellentRatio = spirit_template_ptr.BlueExcellentRatio;
	local BlueExcellentRatio_num = #(BlueExcellentRatio);
	local PurpleExcellentRatio = spirit_template_ptr.PurpleExcellentRatio;
	local PurpleExcellentRatio_num = #(PurpleExcellentRatio);
	local OrangeExcellentRatio = spirit_template_ptr.OrangeExcellentRatio;
	local OrangeExcellentRatio_num = #(OrangeExcellentRatio);
	local RedExcellentRatio = spirit_template_ptr.RedExcellentRatio;
	local RedExcellentRatio_num = #(RedExcellentRatio);
	local total_att_num = BaseExcellentAtt_num // hld.e_att_one_max;
	if (total_att_num ~= GreenExcellentRatio_num // 2
		or total_att_num ~= BlueExcellentRatio_num // 2
		or total_att_num ~= PurpleExcellentRatio_num // 2
		or total_att_num ~= OrangeExcellentRatio_num // 2
		or total_att_num ~= RedExcellentRatio_num // 2
		or total_att_num <= 0)then
		return hld.e_error_code_no_template;
	end

	local in_is_enabled = (temp_item:have_not_save_forge_att_num() > 0);
	self:send_forge_message(temp_player, temp_item, false, in_is_enabled)
	return hld.e_error_code_success;
end

function spirit:spirit_forge(temp_player, item_guid, forge_lock_flag, use_lock_first)
	
	if temp_player == nil then
		return hld.e_error_code_no_template
	end

    local item_set_ref = temp_player:get_item_set()
	if item_set_ref == nil then
        return hld.e_error_code_no_template
	end
	
	local temp_item = item_set_ref:get_item(item_guid[1], item_guid[2])
	if temp_item == nil then
		return hld.e_error_code_no_template
	end

	local forge = {}
	local lock_forge_mum = 0
	for i = 0, hld.item_temp_forge_property_max -1 do
		forge[i] = (forge_lock_flag >> i) & 1
		if forge[i] > 0 then
			lock_forge_mum = lock_forge_mum + 1
		end
	end
    delog("---------forge-----------")
	delog(forge)

	local total_att_num = 0
	local att_index_array = {}
	local final_excellent_color = 0
	local att_num_rand_value = hld.random_gen:get_random(1, 100)
	local att_num_rand_target_value = 0
	local rand_final_att_num = 0 --需要几条卓越属性		
	local spirit_template_ptr = self:get_spirit_template_ptr(temp_item)

	local BaseExcellentAtt = spirit_template_ptr.BaseExcellentAtt
	local BaseExcellentAtt_num = #(BaseExcellentAtt)
	local GreenExcellentRatio = spirit_template_ptr.GreenExcellentRatio
	local GreenExcellentRatio_num = #(GreenExcellentRatio)
	local BlueExcellentRatio = spirit_template_ptr.BlueExcellentRatio
	local BlueExcellentRatio_num = #(BlueExcellentRatio)
	local PurpleExcellentRatio = spirit_template_ptr.PurpleExcellentRatio
	local PurpleExcellentRatio_num = #(PurpleExcellentRatio)
	local OrangeExcellentRatio = spirit_template_ptr.OrangeExcellentRatio
	local OrangeExcellentRatio_num = #(OrangeExcellentRatio)
	local RedExcellentRatio = spirit_template_ptr.RedExcellentRatio
	local RedExcellentRatio_num = #(RedExcellentRatio)

	total_att_num = BaseExcellentAtt_num // hld.e_att_one_max
	if total_att_num ~= GreenExcellentRatio_num // 2
		or total_att_num ~= BlueExcellentRatio_num // 2
		or total_att_num ~= PurpleExcellentRatio_num // 2
		or total_att_num ~= OrangeExcellentRatio_num // 2
		or total_att_num ~= RedExcellentRatio_num // 2
		or total_att_num <= 0 then
		return hld.e_error_code_no_template
	end

	local lock_item_num = item_set_ref:count_item_by_template_id(spirit_template_ptr.MaterialID, hld.e_bag_type_bag, hld.e_item_locked)
	local unlock_item_num = item_set_ref:count_item_by_template_id(spirit_template_ptr.MaterialID, hld.e_bag_type_bag, hld.e_item_unlocked)
	if use_lock_first == 1 then
		if lock_item_num + unlock_item_num < spirit_template_ptr.MaterialCost then
			return ConstDefine.e_item_string_matirial_not_enough
		end
	else
		if unlock_item_num < spirit_template_ptr.MaterialCost then
			return ConstDefine.e_item_string_matirial_not_enough
		end
	end

	local LockMaterialID = spirit_template_ptr.LockMaterialID
	local LockMaterialCost = spirit_template_ptr.LockMaterialCost
	local LockMaterialID_num = #(LockMaterialID)
	if lock_forge_mum > 0 and item_set_ref:count_item_by_template_id(tonumber(LockMaterialID[lock_forge_mum]), hld.e_bag_type_bag, -1) < tonumber(LockMaterialCost[lock_forge_mum]) then
		--没材料
		return hld.e_error_code_item_matirial_not_enough
	end
	--扣除物品
	item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_forge, 0, spirit_template_ptr.MaterialID, spirit_template_ptr.MaterialCost, use_lock_first)
	if lock_forge_mum > 0 then
		item_set_ref:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_forge, 0, tonumber(LockMaterialID[lock_forge_mum]),tonumber(LockMaterialCost[lock_forge_mum]), use_lock_first)
	end
	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property6 do
		temp_item:set_data_info(i, 0)
	end

	local is_first = 0
	if temp_player:get_first_time_do_mgr():is_first_time_do(hld.e_first_time_do_type_spirit_forge) then 
		temp_player:get_first_time_do_mgr():add_do_record(hld.e_first_time_do_type_spirit_forge)
		is_first = 1
	end
	
	local first_better_att_num = 0
	local first_better_forge = {}
	for i = 0, hld.item_temp_forge_property_max - 1 do
		first_better_forge[i] = 0
		if is_first == 1 then	
			local item_temp_forge_block = temp_item:get_data_info(i + hld.e_item_info_random_property1)
			if (item_temp_forge_block > 0) and (forge[i] == 0) then
				first_better_forge[i] = 1
				first_better_att_num = first_better_att_num + 1
				temp_item:set_forge_property(i)
			end
		end
	end
	

	for i = 0, hld.item_temp_forge_property_max - 1 do
		if forge[i] > 0 then
			temp_item:set_forge_property(i)
		end
	end

	for i = 1, BaseExcellentAtt_num, hld.e_att_one_max do
		table.insert(att_index_array, tonumber(BaseExcellentAtt[i + 1]))
	end


	local ExcellentAttNumGenRate = spirit_template_ptr.ExcellentAttNumGenRate;
	local ExcellentAttNumGenRate_num = #(ExcellentAttNumGenRate);
	for i = 0, ExcellentAttNumGenRate_num - 1 do
		att_num_rand_target_value = att_num_rand_target_value + ExcellentAttNumGenRate[i + 1]
		if att_num_rand_value <= att_num_rand_target_value then
			rand_final_att_num = i
			break;
		end
	end

	delog("---------rand_final_att_num----", rand_final_att_num)

	if is_first == 1 then
		lock_forge_mum = lock_forge_mum +first_better_att_num
	end

	local max_att_num = ExcellentAttNumGenRate_num - 1;
	local final_att_num = 0
	if rand_final_att_num < lock_forge_mum then
		if lock_forge_mum + rand_final_att_num > ExcellentAttNumGenRate_num then
			final_att_num = ExcellentAttNumGenRate_num
		else
			final_att_num = lock_forge_mum + rand_final_att_num
		end
	else
		final_att_num = rand_final_att_num
	end

	local get_att_index_array = {}
	local temp_att_index_lib_array = {}

	local in_temp_att_index_lib_array = {}
	for  i = 0, total_att_num - 1 do
		table.insert(in_temp_att_index_lib_array, false)
	end

	for i = 0, hld.item_temp_forge_property_max - 1 do
		if forge[i] > 0 then
			local forge_property = temp_item:get_data_info(i + hld.e_item_info_forge_property1)
			local forge_name_flag_att = forge_property >> 24
			in_temp_att_index_lib_array[forge_name_flag_att] = true
		end
	end
	
	if is_first == 1 then
		for i = 0, hld.item_temp_forge_property_max - 1 do
			if first_better_forge[i] > 0 then
				forge_property = temp_item:get_data_info(i + hld.e_item_info_forge_property1)
				forge_name_flag_att = forge_property >> 24
				in_temp_att_index_lib_array[forge_name_flag_att] = true
			end
		end
	end

	for i = 0, total_att_num - 1 do
		if in_temp_att_index_lib_array[i] ~= true then
			table.insert(temp_att_index_lib_array, i)
		end
	end

	for i = 0, final_att_num - lock_forge_mum - 1 do
		if #temp_att_index_lib_array > 0 then
			local size_temp_att_index_lib = #temp_att_index_lib_array
			local temp_index = hld.random_gen:get_random(1, size_temp_att_index_lib)
			table.insert(get_att_index_array, temp_att_index_lib_array[temp_index])
			table.remove(temp_att_index_lib_array, temp_index)
		end
	end

	local temp_color_rate_array_old = {}
	local temp_color_rate_array = {}
	for i = 0, hld.item_temp_forge_property_max - 1 do
		if (first_better_forge[i] > 0) and (is_first == 1) then
			local forge_property_old = temp_item:get_data_info(i + hld.e_item_info_forge_property1)
			local att_index_old = forge_property_old >> 24
			local random_limit_old = (temp_item:get_data_info(i + hld.e_item_info_forge_property1)) & 65535
			
			for j = 2, hld.e_item_color_pink do
				temp_color_rate_array_old = template_manager:get_spirit_excellent_target_color_array(spirit_template_ptr, j)
				local min_limit_old = temp_color_rate_array_old[att_index_old * 2 + 1]
				local max_limit_old = temp_color_rate_array_old[att_index_old * 2 + 2]

				if (max_limit_old > random_limit_old) and (random_limit_old > min_limit_old) then	
					local ForgeExcellentAttQualityWeight = spirit_template_ptr.ForgeExcellentAttQualityWeight
					local limit_color_value = 0
					for n = 1, j + 1 do
						limit_color_value = limit_color_value + tonumber(ForgeExcellentAttQualityWeight[n])
					end
					local final_excellent_color = spirit:rand_spirit_excellent_att_color(spirit_template_ptr, limit_color_value)

					if final_excellent_color > j then
						temp_color_rate_array = template_manager:get_spirit_excellent_target_color_array(spirit_template_ptr, final_excellent_color)
						local min_limit = temp_color_rate_array[att_index_old * 2 + 1]
						local max_limit = temp_color_rate_array[att_index_old * 2 + 2]
						local random_limit = hld.random_gen:get_random(min_limit, max_limit)
						local forge_property = 0
						forge_property = forge_property | random_limit
						forge_property = forge_property | (att_index_array[att_index_old + 1] << 16)
						forge_property = forge_property | (att_index_old << 24)
						temp_item:set_data_info(i + hld.e_item_info_forge_property1, forge_property)										
					end
				
					
					if	final_excellent_color == j then
						local random_limit_new = hld.random_gen:get_random(random_limit_old , max_limit_old)
						local forge_property = 0
						forge_property = forge_property | random_limit_new
						forge_property = forge_property | (att_index_array[att_index_old + 1] << 16)
						forge_property = forge_property | (att_index_old << 24)
						temp_item:set_data_info(i + hld.e_item_info_forge_property1, forge_property)
					end
				end				
			end
		end
	end

	local j = 0
	for i = 1, #get_att_index_array do
		if get_att_index_array[i] >= 0 and get_att_index_array[i] < 32 then
			while forge[j] > 0 do
				j = j + 1
			end
			if j > final_att_num then
				break
			end

			local att_index = get_att_index_array[i];
			local final_excellent_color = spirit:rand_spirit_excellent_att_color(spirit_template_ptr,1)
			temp_color_rate_array = template_manager:get_spirit_excellent_target_color_array(spirit_template_ptr, final_excellent_color)
			local min_limit = temp_color_rate_array[att_index * 2 + 1]
			local max_limit = temp_color_rate_array[att_index * 2 + 2]
			local random_limit = hld.random_gen:get_random(min_limit, max_limit)
			local forge_property = 0
			forge_property = forge_property | random_limit
			forge_property = forge_property | (att_index_array[att_index + 1] << 16)
			forge_property = forge_property | (att_index << 24)
			temp_item:set_data_info(j + hld.e_item_info_forge_property1, forge_property)
			j = j + 1;
		end
	end

	local have_att_idex = {};
	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property5 do
		if (temp_item:get_data_info(i) ~= 0) then
			table.insert(have_att_idex,i)
		end
	end

	for i = 1, #have_att_idex do
		temp_item:set_data_info(hld.e_item_info_forge_property1 + i -1, temp_item:get_data_info(have_att_idex[i]));
	end
	for i = #have_att_idex + 1, hld.item_temp_forge_property_6 do
		temp_item:set_data_info(hld.e_item_info_forge_property1 + i - 1, 0)
	end
	temp_player:get_mission_mgr():target_check(hld.e_mission_end_type_spirit_baptism,0,0);
	--发送消息
	self:send_forge_message(temp_player, temp_item, false, true)
	return hld.e_error_code_success
end

function spirit:spirit_save_forge(player_ref, item_guid)

	if player_ref == nil then
		return hld.e_error_code_no_template
	end

	local item_ptr = player_ref:get_item_set():get_item(item_guid[1], item_guid[2])
	if item_ptr == nil then
		return hld.e_error_code_no_template
	end

	local spirit_template_ptr = self:get_spirit_template_ptr(item_ptr)
	if spirit_template_ptr == nil then
		return hld.e_error_code_no_template
	end

	local BaseExcellentAtt = spirit_template_ptr.BaseExcellentAtt
	local BaseExcellentAtt_num = #(BaseExcellentAtt)

	local total_att_num = BaseExcellentAtt_num // hld.e_att_one_max

	local fight_add_bonus = spirit_template_ptr.FightAddBonus;
	if item_ptr:get_data_info(hld.e_item_info_activate) > 0 then
		pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, item_ptr, false, fight_add_bonus[1])
	else
		pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, item_ptr, false, fight_add_bonus[2])
	end

	local temp_forge_att_flag = 0;
	local final_att_num = item_ptr:have_not_save_forge_att_num();
	local item_temp_forge_block = {};
	local item_temp_forge_sort_block = {};
	local item_temp_forge_name_flag_block = {};

	for i = 0, final_att_num - 1 do
		local forge_name_flag_att = item_ptr:get_forge_name_flag_att(i + hld.e_item_info_forge_property1);
		local forge_name_att = item_ptr:get_forge_name_att(i + hld.e_item_info_forge_property1);
		local forge_att = item_ptr:get_forge_att(i + hld.e_item_info_forge_property1);
		item_temp_forge_block[i] = forge_att;
		item_temp_forge_name_flag_block[i] = forge_name_flag_att;
		temp_forge_att_flag = temp_forge_att_flag | 1 << item_temp_forge_name_flag_block[i]
	end

	item_ptr:set_data_info(hld.e_item_info_random_had_flag, temp_forge_att_flag)
	for i = 0, final_att_num - 1 do
		local min = i;
		for j = i + 1, final_att_num - 1 do
			if tonumber(item_temp_forge_name_flag_block[j]) < tonumber(item_temp_forge_name_flag_block[min]) then
				min = j;
			end
		end
		item_temp_forge_sort_block[i] = item_temp_forge_block[min];
		if min ~= i then
			item_temp_forge_name_flag_block[min] = item_temp_forge_name_flag_block[i];
			item_temp_forge_block[min] = item_temp_forge_block[i];
		end
	end
	item_temp_forge_sort_block[final_att_num - 1] = item_temp_forge_block[final_att_num - 1];

	for i = 0, hld.item_temp_forge_property_max - 1 do
		if item_temp_forge_sort_block[i] ~= nil then
			item_ptr:set_data_info(hld.e_item_info_random_property1 + i, item_temp_forge_sort_block[i])
		else
			item_ptr:set_data_info(hld.e_item_info_random_property1 + i, 0)
		end

	end
	player_ref:get_item_set():send_item_one(item_ptr, true, 0);
	--发送消息
	self:send_forge_message(player_ref, item_ptr, true, false)

	for i = hld.e_item_info_forge_property1, hld.e_item_info_forge_property6 do
		item_ptr:set_data_info(i, 0);
	end

	if item_ptr:get_data_info(hld.e_item_info_activate) > 0 then
		pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, item_ptr, true, fight_add_bonus[1]);
	else
		pawn_att_mgr:set_excellent_att_array(player_ref, spirit_template_ptr.BaseExcellentAtt, item_ptr, true, fight_add_bonus[2]);
	end
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_spirit_baptism_color,0,0);
	return hld.e_error_code_success;
end

function spirit:rand_spirit_excellent_att_color(spirit_template_ptr,limit_color_value)
	if spirit_template_ptr == nil then
		return hld.e_item_color_max
	end

	local color_rand_value = hld.random_gen:get_random(limit_color_value, 100)
	local color_rand_target_value = 0;
	local final_excellent_color = 0;
	local ForgeExcellentAttQualityWeight = spirit_template_ptr.ForgeExcellentAttQualityWeight

	for i = 1, #ForgeExcellentAttQualityWeight do
		color_rand_target_value = color_rand_target_value + tonumber(ForgeExcellentAttQualityWeight[i])
		if color_rand_value <= color_rand_target_value then
			return i - 1
		end
	end
	return hld.e_item_color_max
end

spirit_mgr = spirit
return spirit_mgr






