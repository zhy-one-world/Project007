local marry_mgr = {}
function marry_mgr:init()
end
local game_config_id = 99000001;

define_C[13650] = function(message_data, player_ref)
	local msg_data = decode("hld.relation_proto.c2s_operate_marry_examination", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	marry_mgr:confirm_marry_operate(player_ref,msg_data.operate_type,msg_data.ext_param_1,msg_data.ext_param_2)
end

define_C[13647] = function(message_data, player_ref)
	local msg_data = decode("hld.relation_proto.c2s_get_marry_examination", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	marry_mgr:get_marry_examination(player_ref,msg_data.is_near)
end

define_C[13649] = function(message_data, player_ref)
	local msg_data = decode("hld.relation_proto.c2s_confirm_marry_examination", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	marry_mgr:onfirm_marry_examination(player_ref,msg_data.is_receive)
end

define_C[13639] = function(message_data, player_ref)
	local msg_data = decode("hld.relation_proto.c2s_req_upgrade_wedding_ring", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	marry_mgr:upgrade_wedding_ring(player_ref,msg_data.use_item_id)
end

function marry_mgr:onfirm_marry_examination(player_ref, is_receive)
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	if	is_receive == nil then
		return;
	end
	player_ref:get_marry_mgr():confirm_marry_dati(is_receive);
end

function marry_mgr:confirm_marry_operate(player_ref, operate_type, ext_param_1, ext_param_2)
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	if	operate_type == nil or ext_param_1 == nil or ext_param_2 == nil then
		return;
	end
	player_ref:get_marry_mgr():confirm_marry_operate(operate_type, ext_param_1, ext_param_2);
end

function marry_mgr:get_marry_examination(player_ref, is_near)
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	if	is_near == nil then
		return;
	end
	if is_near == 1 then
		player_ref:get_marry_mgr():begin_marry_dati(true);
		return;
	end
	player_ref:get_marry_mgr():begin_marry_dati(false);
end

function marry_mgr:upgrade_wedding_ring(player_ref, use_item_id)
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	if	use_item_id == nil then
		return;
	end
	player_ref:get_marry_mgr():wedding_ring_upgrade(use_item_id);
end

--根据结婚道具id获得初始等级
function get_init_level_by_marry_item_id(real_id, marry_item_id)
	local init_level = 0
	local WeddingRingTemplate = template_manager:get_line("WeddingRingTemplate", real_id)
	if nil == WeddingRingTemplate then
		return init_level;
	end
	if marry_item_id == 0 then
		init_level = WeddingRingTemplate.InitLevel2
	else
		local init_level_vec = {}
		init_level_vec = WeddingRingTemplate.InitLevel
		for k,v in pairs(init_level_vec) do
			if marry_item_id == tonumber(v) then
				init_level = tonumber(init_level_vec[k+1])
			end
		end
	end
	return init_level
end

--激活戒指
function activate_wedding_ring(player_index, marry_item_id)
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();

	if equip_item:get_data_info(hld.e_item_info_activate) == 0 then
		equip_item:set_data_info(hld.e_item_info_activate, 1);
		local real_id = equip_item:get_real_id();
		local init_level = get_init_level_by_marry_item_id(real_id, marry_item_id);
		if init_level > equip_item:get_data_info(hld.e_item_info_upgrade_count) then
			equip_item:set_data_info(hld.e_item_info_upgrade_count, init_level);
			equip_item:set_data_info(hld.e_item_info_wing_exp, 0);
		end
		item_set:set_wedding_equip_att(equip_item, true);
		item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
		player_ref:sync_wedding_ring_level_to_ws();
		item_set:set_wedding_init_fashion_state(true); --激活赠送的时装
		player_ref:player_changed(hld.eat_role_marry);
		local new_value = player_ref:get_logic_data(hld.e_role_logic_info_marry_times) + 1;
		player_ref:set_logic_data(hld.e_role_logic_info_marry_times, new_value);
		player_ref:add_title_by_type_and_value(hld.e_title_type_marry_times, new_value, 0);
	end
	--print "activate_wedding_ring"
end

--关闭戒指
function close_wedding_ring(player_index)
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();

	if equip_item:get_data_info(hld.e_item_info_activate) == 1 then
		equip_item:set_data_info(hld.e_item_info_activate, 0);
		item_set:set_wedding_equip_att(equip_item, false);
		equip_item:set_data_info(hld.e_item_info_random_property2, 0); -- 清空灵犀值
		item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
		item_set:set_wedding_init_fashion_state(false); --取消赠送的时装
		local new_value = player_ref:get_logic_data(hld.e_role_logic_info_marry_divorce_times) + 1;
		player_ref:set_logic_data(hld.e_role_logic_info_marry_divorce_times, new_value);
		player_ref:add_title_by_type_and_value(hld.e_title_type_marry_divorce_times, new_value, 0);
	end
	--print "close_wedding_ring"
end

--培养戒指
function wedding_ring_upgrade(player_index, use_item_id)
	--print "wedding_ring_upgrade"
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();
	
	if equip_item:get_data_info(hld.e_item_info_activate) == 0 then
		return 90029001;
	end

	local real_id = equip_item:get_real_id();
	local template = template_manager:get_line("WeddingRingTemplate", real_id);
	if template == nil  then
		return 90029001;
	end

	local cur_ring_star_num = template["StarNow"];
	local max_ring_star_num = template["RingAddStarMax"];
	local cur_ring_level = template["UpgradeNow"];
	local max_ring_level = template["RingUpgradeMax"];
	
	if cur_ring_star_num >= max_ring_star_num and cur_ring_level >= max_ring_level then
		return 90029054;
	end
	
	if cur_ring_star_num == max_ring_star_num then
		--升阶
		--判断是不是所需材料
		local has_num = item_set:count_item_by_template_id(use_item_id, hld.e_bag_type_bag, -1);
		if use_item_id ~= template["UpgradeMatirialId"] or has_num < template["UpgradeMatirialNum"] then
			return 90029055;
		end
		--扣除材料
		item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wedding_ring_upgrade, equip_item:get_data_info(hld.e_item_info_info_id), template["UpgradeMatirialId"], template["UpgradeMatirialNum"], hld.e_item_locked);
		--升阶
		item_set:set_wedding_equip_att(equip_item, false);
		equip_item:set_data_info(hld.e_item_info_wing_exp, 0);
		equip_item:set_data_info(hld.e_item_info_upgrade_count, equip_item:get_data_info(hld.e_item_info_upgrade_count) + 1);
		item_set:set_wedding_equip_att(equip_item, true);
		item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
		player_ref:sync_wedding_ring_level_to_ws();
		return 0;
	else
		--升星
		--判断材料
		local add_exp = 0;
		local material = template.AddStarMatirialId
		for k,v in pairs(material) do
			if use_item_id == tonumber(v) then
				add_exp = tonumber(material[k+1])
			end
		end

		local has_num1 = item_set:count_item_by_template_id(use_item_id, hld.e_bag_type_bag, -1)
		if add_exp == 0 or item_set:count_item_by_template_id(use_item_id, hld.e_bag_type_bag, -1) < 1 then
			return 90029055;
		end
		--扣除材料
		item_set:cost_item_by_id_with_lock_states(hld.e_server_log_del_item_wedding_ring_upgrade, equip_item:get_data_info(hld.e_item_info_info_id), use_item_id, 1, hld.e_item_locked);
		--升星

		local add_star_exp_now = equip_item:get_data_info(hld.e_item_info_wing_exp);
		local add_star_exp_max = template["AddStarMaxEXP"];
		add_star_exp_now = add_star_exp_now + add_exp;

		if add_star_exp_now >= add_star_exp_max then
			local upgrade_value = 0
			while add_star_exp_now >= add_star_exp_max and add_star_exp_max ~= 0 do
				add_star_exp_now = add_star_exp_now - add_star_exp_max;
				upgrade_value = upgrade_value + 1
				add_star_exp_max = template_manager:get_field("WeddingRingTemplate", real_id + upgrade_value, "AddStarMaxEXP");
			end
			if cur_ring_star_num + upgrade_value >= max_ring_star_num then
				upgrade_value = max_ring_star_num - cur_ring_star_num
			end
			item_set:set_wedding_equip_att(equip_item, false);
			equip_item:set_data_info(hld.e_item_info_upgrade_count, equip_item:get_data_info(hld.e_item_info_upgrade_count) + upgrade_value);
			--equip_item:set_data_info(hld.e_item_info_upgrade_count,   1);
			item_set:set_wedding_equip_att(equip_item, true);
			player_ref:sync_wedding_ring_level_to_ws();
			if cur_ring_star_num + upgrade_value == max_ring_star_num then
				add_star_exp_now = 0
			end
		end
		equip_item:set_data_info(hld.e_item_info_wing_exp, add_star_exp_now);
		item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
		return 0;
	end
	return 90029001;
end


--获取戒指等级
function get_ring_level(player_index)
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();
	
	if equip_item:get_data_info(hld.e_item_info_activate) == 0 then
		return 0;
	else
		return equip_item:get_data_info(hld.e_item_info_upgrade_count);
	end
end

--上线刷新信息
function refresh_wedding_ring_info(player_index, is_marry, marry_item_id, mate_ring_level, heart_value)
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();
	
	if equip_item:get_data_info(hld.e_item_info_activate) == 0 then
		if is_marry == 1 then
			--先激活
			equip_item:set_data_info(hld.e_item_info_activate, 1);
			--设置等级
			local init_level = get_init_level_by_marry_item_id(equip_item:get_real_id(), marry_item_id);
			if init_level > equip_item:get_data_info(hld.e_item_info_upgrade_count) then
				equip_item:set_data_info(hld.e_item_info_upgrade_count, init_level);
			end
			--设置配偶强化等级
			equip_item:set_data_info(hld.e_item_info_random_property1, mate_ring_level)
			--设置灵犀值
			equip_item:set_data_info(hld.e_item_info_random_property2, heart_value)
			item_set:set_wedding_equip_att(equip_item, true);
			item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
			player_ref:sync_wedding_ring_level_to_ws();
			item_set:set_wedding_init_fashion_state(true); --激活赠送的时装
			player_ref:player_changed(hld.eat_role_marry);
			local new_value = player_ref:get_logic_data(hld.e_role_logic_info_marry_times) + 1;
			player_ref:set_logic_data(hld.e_role_logic_info_marry_times, new_value);
			player_ref:add_title_by_type_and_value(hld.e_title_type_marry_times, new_value, 0);
		end
	else
		--激活了 发现没结婚 关闭
		if is_marry == 0  then
			equip_item:set_data_info(hld.e_item_info_activate, 0);
			item_set:set_wedding_equip_att(equip_item, false);
			equip_item:set_data_info(hld.e_item_info_random_property2, 0); -- 清空灵犀值
			item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
			item_set:set_wedding_init_fashion_state(false); --取消赠送的时装
			local new_value = player_ref:get_logic_data(hld.e_role_logic_info_marry_divorce_times) + 1;
			player_ref:set_logic_data(hld.e_role_logic_info_marry_divorce_times, new_value);
			player_ref:add_title_by_type_and_value(hld.e_title_type_marry_divorce_times, new_value, 0);
		else
			--激活了 也结婚了  就按需刷新
			if mate_ring_level ~= equip_item:get_data_info(hld.e_item_info_random_property1) or heart_value ~= equip_item:get_data_info(hld.e_item_info_random_property2) then
				item_set:set_wedding_equip_att(equip_item, false);
				equip_item:set_data_info(hld.e_item_info_random_property1, mate_ring_level);
				equip_item:set_data_info(hld.e_item_info_random_property2, heart_value);
				item_set:set_wedding_equip_att(equip_item, true);
				item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
			end
		end
	end
	--print "refresh_wedding_ring_info"
end

--刷新配偶的婚戒培养等级e_item_info_random_property1
function refresh_mate_wedding_ring_level(player_index, ring_level)
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();
	if ring_level ~= equip_item:get_data_info(hld.e_item_info_random_property1) then
		item_set:set_wedding_equip_att(equip_item, false);
		equip_item:set_data_info(hld.e_item_info_random_property1, ring_level);
		item_set:set_wedding_equip_att(equip_item, true);
		item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
	end
	--print "refresh_mate_wedding_ring_level"
end

--刷新灵犀值e_item_info_random_property2
function refresh_heart_value(player_index, heart_value)
	local player_ref 	= hld.get_player_by_index(player_index);
	local item_set 		= player_ref:get_item_set();
	local equip_item 	= item_set:get_wedding_equip_item();
	if heart_value ~= equip_item:get_data_info(hld.e_item_info_random_property2) then
		item_set:set_wedding_equip_att(equip_item, false);
		equip_item:set_data_info(hld.e_item_info_random_property2, heart_value);
		item_set:set_wedding_equip_att(equip_item, true);
		item_set:send_item_one(equip_item, true, hld.e_update_item_info_mode_default);
	end
	--print "refresh_heart_value"
end

function random_dati()
	local timu = 0
	local question_template = template_manager:get_template("QuestionMarryTemplate")
	if question_template == nil then
		return
	end
	local id_list = {}
	for k,v in pairs(question_template) do
		table.insert(id_list,k)
	end
	local question_template_size =#id_list
	--随机
	math.randomseed(tostring(os.time()):reverse():sub(1, 7))
	local m_random = math.random(question_template_size)

	print("random_dati".."-"..question_template_size.."-"..m_random)

	timu = id_list[m_random]
	return timu
end

function set_new_timu(player_index,timu_id)
	local player_ref = hld.get_player_by_index(player_index)
	player_ref:set_unit_info(hld.e_role_info_marry_dati_id,timu_id)
end
function add_dati_right_num(player_index)
	local player_ref = hld.get_player_by_index(player_index)
	local old_num = player_ref.get_unit_info(hld.e_role_info_marry_dati_right_num,timu_id)
	player_ref:set_unit_info(hld.e_role_info_marry_dati_right_num,old_num + 1)
end
function add_dati_num(player_index)
	local player_ref = hld.get_player_by_index(player_index)
	local old_num = player_ref.get_unit_info(hld.e_role_info_marry_dati_current_answer,timu_id)
	player_ref:set_unit_info(hld.e_role_info_marry_dati_current_answer,old_num + 1)
end
function set_answer(player_index,answer)
	local player_ref = hld.get_player_by_index(player_index)
	player_ref:set_unit_info(hld.e_role_info_marry_dati_current_answer,answer)
end

function begin_marry_dati(player_index,couple_player_index)
	local player_ref = hld.get_player_by_index(player_index)
	local couple_ref = hld.get_player_by_index(couple_player_index)
	if  couple_ref:is_self_server() == false then
		return 90096777
	end
	if  couple_ref:is_in_copy_map() == true then
		return 90096777
	end
	if couple_ref:is_player_in_the_same_team(player_ref)  == false then
		return 90096775
	end
	local end_num = player_ref:get_unit_info(hld.e_role_info_marry_dati_num)
	if end_num >= 5 then
		return 90096778
	end
	result = 0
	return result
end

function confirm_dati(player_index,couple_index,chose)
	print("confirm_dati")
	local player_ref = hld.get_player_by_index(player_index)
	local couple_ref = hld.get_player_by_index(couple_index)

	--设置当前答案
	local is_right = -1
	player_ref:set_unit_info(hld.e_role_info_marry_dati_current_answer,chose)
	local couple_answer = couple_ref:get_unit_info(hld.e_role_info_marry_dati_current_answer)
	local timu_id = player_ref:get_unit_info(hld.e_role_info_marry_dati_id)
	if couple_answer == 0 then
		return is_right, 0-- 等待对方选择
	end
	local is_true = false 
	if chose == -1 or couple_answer == -1 or chose ~= couple_answer then
		--有人弃权
		is_true = false
		is_right = 1
	elseif chose == couple_answer  then
		is_true = true
		is_right = 0
	end
	--发奖	
	get_dati_reward(player_index,timu_id,is_true)
	get_dati_reward(couple_index,timu_id,is_true)

	--增加答对数量
	local old_right_num = player_ref:get_unit_info(hld.e_role_info_marry_dati_right_num)
	if is_right == 0 then
		player_ref:set_unit_info(hld.e_role_info_marry_dati_right_num,old_right_num + 1)
		couple_ref:set_unit_info(hld.e_role_info_marry_dati_right_num,old_right_num + 1)
			--增加灵犀值
		local add_num =  template_manager:get_field("GameConfigTemplate", game_config_id, "DaTiAddValue" )
		player_ref:add_heart_value(1,add_num)
		couple_ref:add_heart_value(1,add_num)	
	end

	--增加答题
	local old_num = player_ref:get_unit_info(hld.e_role_info_marry_dati_num)
	player_ref:set_unit_info(hld.e_role_info_marry_dati_num,old_num + 1)
	couple_ref:set_unit_info(hld.e_role_info_marry_dati_num,old_num + 1)

	if (old_num + 1 == 5)  and (old_right_num + 1 == 5) then
		player_ref:get_marry_dati_last_reward()
		couple_ref:get_marry_dati_last_reward()
	end
	--重置
	player_ref:set_unit_info(hld.e_role_info_marry_dati_current_answer,0)
	couple_ref:set_unit_info(hld.e_role_info_marry_dati_current_answer,0)
	if old_num + 1 < 5 then
		local new_timu = random_dati()
		--更新问题
		player_ref:set_unit_info(hld.e_role_info_marry_dati_id,new_timu)
		couple_ref:set_unit_info(hld.e_role_info_marry_dati_id,new_timu)
	end
	return is_right,couple_answer
end

function get_dati_reward(player_index,timu_id,is_true)
	local player_ref = hld.get_player_by_index(player_index)
	player_ref:get_marry_dati_reward(timu_id,is_true)
end
return marry_mgr