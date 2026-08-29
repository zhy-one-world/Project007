local belief_rune={}
function belief_rune:init()
end
define_C[14400] = function(message_data, player_ref)
	local msg_data = decode("hld.belief_rune_proto.belief_rune_equip_on", message_data)
	local error_id = 0;
	local decompose_rune_piece = 0;
	error_id = belief_rune:equip_on(player_ref, msg_data.item_guid, msg_data.item_slot);
	belief_rune:response_operate_result(player_ref, hld.e_belief_rune_operate_type_equip_on, error_id, decompose_rune_piece)
end
define_C[14401] = function(message_data, player_ref)
	local msg_data = decode("hld.belief_rune_proto.belief_rune_level_up", message_data)
	local error_id = 0;
	local decompose_rune_piece = 0;
	error_id = belief_rune:level_up(player_ref, msg_data.item_guid);
	belief_rune:response_operate_result(player_ref, hld.e_belief_rune_operate_type_level_up, error_id, decompose_rune_piece)
end
define_C[14402] = function(message_data, player_ref)
	local msg_data = decode("hld.belief_rune_proto.belief_rune_equip_decompose", message_data)
	local error_id = 0;
	local decompose_rune_piece = 0;
	error_id, decompose_rune_piece = belief_rune:decompose(player_ref, msg_data.item_guid_array, msg_data.decompose_type);
	belief_rune:response_operate_result(player_ref, hld.e_belief_rune_operate_type_decompose, error_id, decompose_rune_piece)
end
function belief_rune:equip_on(player_ref, item_guid, item_slot)
-- 检查玩家是否存在
	if (player_ref == nil or player_ref:is_valid() == false)then
		return hld.e_belief_rune_error_type_player_error;
	end

	item_slot = item_slot - 1;

	--检查要放入的孔的位置是否合法
	if (item_slot < 0 or item_slot >= ConstDefine.belief_rune_equip_bag_num)then
		return hld.e_belief_rune_error_type_param_slot_error;
	end

	local belief_type = math.floor(item_slot / ConstDefine.belief_rune_equip_slot_each_type_num);

	-- 检查改槽孔是否开启
	local player_level = belief_mgr:get_belief_level(player_ref, belief_type);
	local open_level = self:get_unlock_level(item_slot);

	if (player_level < open_level) then
		return hld.e_belief_rune_error_type_param_slot_is_unlock;
	end
	--检查道具是否存在
	local item_ptr = player_ref:get_item_set():get_item(item_guid[1], item_guid[2]);
	if (nil == item_ptr)then
		return hld.e_belief_rune_error_type_item_not_exist;
	end
	-- 检查道具类型是否为信仰符文
	local item_template_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id());
	if (item_template_ptr == nil or item_template_ptr.item_type ~= hld.e_item_type_belief_rune)then
		return hld.e_belief_rune_error_type_item_is_not_belief_rune;
	end
	-- 检查道具的子类型是否合法
	if (item_template_ptr.sub_type ~= belief_type)then
		return hld.e_belief_rune_error_type_item_is_not_sub_belief_rune;
	end

	-- 检查模板数据表里面是否存在该数据
	local old_belief_rune_go_to_slot = item_ptr:get_data_info(hld.e_item_info_slot);
	local BeliefRunesTemplatePtr = self:get_belief_rune_template(item_ptr);

	if (BeliefRunesTemplatePtr == nil)then
		return hld.e_belief_rune_error_type_template_not_found;
	end
	-- 检查该道具的容器类型是否合法
	if (item_ptr:get_data_info(hld.e_item_info_container_type) ~= hld.e_bag_type_belief_rune)then
		return hld.e_belief_rune_error_type_item_container_error;
	end

	local equip_off_item_array = {};
	local equip_on_item_array = {};

	-- 检查该槽孔是否已经装备了符文道具
	local old_item = player_ref:get_item_set():get_item_by_slot(hld.e_bag_type_belief_rune_equip, item_slot);

	if (old_item ~= nil)then
		-- 同属性只能放一个
		local att_type = BeliefRunesTemplatePtr.AttType;
		local old_item_solt = old_item:get_data_info(hld.e_item_info_slot);
		local BeliefRunesTemplatePtr_old = self:get_belief_rune_template(old_item);
		local old_att_type = BeliefRunesTemplatePtr_old.AttType;
		if(att_type ~= old_att_type) then
			if (player_ref:get_belief_rune_mgr():is_type_equiped(belief_type, att_type) == true)then
				return hld.e_belief_rune_error_type_equip_on_one_attr;
			end
		end
		--替換操作
		local bag_slot = item_ptr:get_data_info(hld.e_item_info_slot);
		old_item:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_belief_rune);
		old_item:set_data_info(hld.e_item_info_slot, bag_slot);
		player_ref:get_item_set():set_target_slot_guid(hld.e_bag_type_belief_rune, bag_slot, old_item:get_item_guid());
		player_ref:get_item_set():send_item_one(old_item, true, 0);--发送消息到前端更新
		player_ref:get_belief_rune_mgr():set_type_equiped(belief_type, old_att_type, false);

		item_ptr:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_belief_rune_equip);
		item_ptr:set_data_info(hld.e_item_info_slot, item_slot);
		player_ref:get_item_set():set_target_slot_guid(hld.e_bag_type_belief_rune_equip, item_slot, item_ptr:get_item_guid());
		player_ref:get_item_set():send_item_one(item_ptr, true, 0);--发送消息到前端更新
		player_ref:get_belief_rune_mgr():set_type_equiped(belief_type, att_type, true);

		table.insert(equip_on_item_array, item_ptr);
		table.insert(equip_off_item_array, old_item);
	else
		-- 同属性的只能装备一个
		local att_type = BeliefRunesTemplatePtr.AttType;
		if (player_ref:get_belief_rune_mgr():is_type_equiped(belief_type, att_type) == true)then
			return hld.e_belief_rune_error_type_equip_on_one_attr;
		end

		-- 装备操作
		item_ptr:set_data_info(hld.e_item_info_container_type, hld.e_bag_type_belief_rune_equip);--修改所在的背包类型(改為信仰符文已裝備的背包裡面)
		player_ref:get_item_set():clear_target_slot(hld.e_bag_type_belief_rune, item_ptr:get_data_info(hld.e_item_info_slot));--从符文背包里面清空
		item_ptr:set_data_info(hld.e_item_info_slot, item_slot);--物品的槽孔位置
		player_ref:get_item_set():set_target_slot_guid(hld.e_bag_type_belief_rune_equip, item_slot, item_ptr:get_item_guid());--放入已经装备的背包里面
		player_ref:get_item_set():send_item_one(item_ptr, true, 0);--发送消息到前端更新

		table.insert(equip_on_item_array, item_ptr);
		player_ref:get_belief_rune_mgr():set_type_equiped(belief_type, att_type, true);
	end
	self:refresh_att(player_ref, equip_off_item_array, equip_on_item_array);
	player_ref:set_unit_gs_change();
	player_ref:get_belief_mgr():sync_ranking_belief_data();

	return hld.e_belief_rune_error_type_none;
end
function belief_rune:level_up(player_ref, item_guid)
	-- 检查玩家是否存在
	if (player_ref == nil)then
		return hld.e_belief_rune_error_type_player_error;
	end
	--检查道具是否存在
	local item_ptr = player_ref:get_item_set():get_item(item_guid[1],item_guid[2]);
	if (nil == item_ptr)then
		return hld.e_belief_rune_error_type_item_not_exist;
	end
	-- 检查道具类型是否为信仰符文
	local item_template_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id());
	if (item_template_ptr == nil or item_template_ptr.item_type ~= hld.e_item_type_belief_rune)then
		return hld.e_belief_rune_error_type_item_is_not_belief_rune;
	end
	-- 检查模板数据表里面是否存在该数据
	local BeliefRunesTemplatePtr = self:get_belief_rune_template(item_ptr);
	if (BeliefRunesTemplatePtr == nil)then
		return hld.e_belief_rune_error_type_template_not_found;
	end
	-- 是否已经满级了
	local cur_upgrade_num = item_ptr:get_data_info(hld.e_item_info_upgrade_count);
	if (cur_upgrade_num >= BeliefRunesTemplatePtr.MaxUpgradeLevel - 1)then
		return hld.e_belief_rune_error_type_is_max_level;
	end
	--检查升级模板数据格式是否正确
	local BeliefRunesUpgradeTemplatePtr = template_manager:get_line("BeliefRunesUpgradeTemplate", BeliefRunesTemplatePtr.UpgradeId + cur_upgrade_num);

	if (BeliefRunesUpgradeTemplatePtr == nil)then
		return hld.e_belief_rune_error_type_template_data_error;
	end
	local updrade_cost = BeliefRunesUpgradeTemplatePtr.UpgradeCost
	local upgrade_cost_num = #(updrade_cost);
	if math.fmod(upgrade_cost_num, 2) ~= 0 then
		return hld.e_belief_rune_error_type_template_data_error;
	end
	--检查消耗的资源是否足够
	for i = 1, upgrade_cost_num, 2 do
		local money_id = tonumber(updrade_cost[i]);
		local money_value = tonumber(updrade_cost[i + 1]);
		if (false == player_ref:can_cut_money(money_id, money_value))then
			return hld.e_belief_rune_error_type_levelup_money_not_enough;
		end
	end

	for i = 1, upgrade_cost_num, 2 do
		local money_id = tonumber(updrade_cost[i]);
		local money_value = tonumber(updrade_cost[i + 1]);
		player_ref:cut_money(money_id, money_value, hld.e_server_log_cut_money_belief_rune_levelup, item_ptr:get_item_template_id(), cur_upgrade_num);
	end

	local is_equiping = item_ptr:get_data_info(hld.e_item_info_container_type) == hld.e_bag_type_belief_rune_equip;
	if (is_equiping)then
		self:set_belief_rune_one_att(player_ref, item_ptr, false);
	end

	item_ptr:set_data_info(hld.e_item_info_upgrade_count, cur_upgrade_num + 1);
	player_ref:get_item_set():send_item_one(item_ptr, true, 0);

	if (is_equiping)then
		self:set_belief_rune_one_att(player_ref, item_ptr, true);
		player_ref:set_unit_gs_change();
		player_ref:get_belief_mgr():sync_ranking_belief_data();
	end

	--发送公告
	if (BeliefRunesUpgradeTemplatePtr.Notice_id > 0)then
		local notice_data = {};
		notice_data.notice_id = BeliefRunesUpgradeTemplatePtr.Notice_id
		notice_data.item_data = {}
		notice_data.num_type_data = {}
		table.insert(notice_data.item_data, item_ptr:get_item_inst());
		table.insert(notice_data.num_type_data,item_ptr:get_data_info(hld.e_item_info_upgrade_count) + 1);
		server_tool_mgr:send_notice(player_ref, notice_data);
	end

	return hld.e_belief_rune_error_type_none;
end
function belief_rune:decompose(player_ref, item_guid_array, decompose_type)
	-- 检查玩家是否存在
	if (player_ref == nil)then
		return hld.e_belief_rune_error_type_player_error;
	end
	local guid_num = #item_guid_array;
	if (guid_num < 2)then
		return hld.e_belief_rune_error_type_param_empty;
	end

	local recover_get_array = {};
	local recover_need_array = {};
	local del_array = {};

	for i = 1, guid_num, 2 do
		--检查道具是否存在
		local item_ptr = player_ref:get_item_set():get_item(item_guid_array[i], item_guid_array[i+1]);
		if (nil == item_ptr) then
			return hld.e_belief_rune_error_type_param_empty;
		end
		--装备在身上的不允许进行分解
		local is_equiping = item_ptr:get_data_info(hld.e_item_info_container_type) == hld.e_bag_type_belief_rune_equip;
		if (is_equiping == true) then
			return hld.e_belief_rune_error_type_param_empty;
		end
		-- 检查道具类型是否为信仰符文
		local item_template_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id());
		if (item_template_ptr == nil or item_template_ptr.item_type ~= hld.e_item_type_belief_rune) then
			return hld.e_belief_rune_error_type_param_empty;
		end
		-- 检查模板数据表里面是否存在该数据
		local BeliefRunesTemplatePtr = self:get_belief_rune_template(item_ptr);
		if (BeliefRunesTemplatePtr == nil) then
			return hld.e_belief_rune_error_type_param_empty;
		end

		local cur_upgrade_num = item_ptr:get_data_info(hld.e_item_info_upgrade_count);
		local BeliefRunesUpgradeTemplatePtr = template_manager:get_line("BeliefRunesUpgradeTemplate", BeliefRunesTemplatePtr.UpgradeId + cur_upgrade_num);
		if (BeliefRunesUpgradeTemplatePtr == nil) then
			return hld.e_belief_rune_error_type_param_empty;
		end
		--dump(decompose_type)
		if (decompose_type == 1) then
			local diamond_recovery_cost = BeliefRunesUpgradeTemplatePtr.DiamondRecoveryCost
			local diamond_recovery_cost_num = #(diamond_recovery_cost);
			if math.fmod(diamond_recovery_cost_num, 2) ~= 0 then
				return hld.e_belief_rune_error_type_param_empty;
			end
			local diamond_recovery_money = BeliefRunesUpgradeTemplatePtr.DiamondRecoveryMoney
			local diamond_recovery_money_num = #(diamond_recovery_money);
			if math.fmod(diamond_recovery_money_num, 2) ~= 0 then
				return hld.e_belief_rune_error_type_param_empty;
			end

			for i = 1, diamond_recovery_cost_num, 2 do
				local money_id = tonumber(diamond_recovery_cost[i]);
				local money_value = tonumber(diamond_recovery_cost[i + 1]);
				if (recover_need_array[money_id]== nil) then
					recover_need_array[money_id] = money_value;
				else
					recover_need_array[money_id] = recover_need_array[money_id] + money_value;
				end
			end

			for i = 1, diamond_recovery_money_num, 2 do
				local money_id = tonumber(diamond_recovery_money[i]);
				local money_value = tonumber(diamond_recovery_money[i + 1]);
				if (recover_get_array[money_id]== nil) then
					recover_get_array[money_id] = money_value;
				else
					recover_get_array[money_id] = recover_get_array[money_id] + money_value;
				end
			end
		else
			local recovery_reward = BeliefRunesUpgradeTemplatePtr.RecoveryReward
			local recovery_reward_num = #(recovery_reward);
			if math.fmod(recovery_reward_num, 2) ~= 0 then
				return hld.e_belief_rune_error_type_param_empty;
			end

			for i = 1, recovery_reward_num, 2 do
				local money_id = tonumber(recovery_reward[i]);
				local money_value = tonumber(recovery_reward[i + 1]);
				if (recover_get_array[money_id]== nil) then
					recover_get_array[money_id] = money_value;
				else
					recover_get_array[money_id] = recover_get_array[money_id] + money_value;
				end
			end
		end
		table.insert(del_array, item_ptr);
	end

	for k,v in pairs(recover_need_array) do
		if (false == player_ref:can_cut_money(k, v)) then
			return hld.e_belief_rune_error_type_decompose_money_not_enough;
		end
	end
	for k,v in pairs(recover_need_array) do
		player_ref:cut_money(k, v, hld.e_server_log_cut_money_belief_rune_diamond_recover, 0, 0);
	end
	for k,v in pairs(del_array) do
		player_ref:get_item_set():del_item(hld.e_server_log_del_item_item_sell, 0, v, v:get_data_info(hld.e_item_info_stack_count));
	end

	local decompose_rune_piece = 0;
	for k,v in pairs(recover_get_array) do
		player_ref:add_money_or_exp(k, v, hld.e_server_log_add_money_belief_rune_piece, 0);
		--计算获得的符文碎片
		if (k == hld.e_money_type_belief_rune_piece) then
			decompose_rune_piece = decompose_rune_piece + v;
		end
	end

	return hld.e_belief_rune_error_type_none, decompose_rune_piece;
end
function belief_rune:get_unlock_level(target_slot)
	local game_config_template_ptr = server_tool_mgr:get_game_config_template_ptr();
	if (game_config_template_ptr == nil) then
		return 0;
	end
	local belief_series_array = {}
	belief_series_array[1] = game_config_template_ptr.FireUnlockRequiresFireBeliefLevel
	belief_series_array[2] = game_config_template_ptr.IceUnlockRequiresIceBeliefLevel
	belief_series_array[3] = game_config_template_ptr.LightUnlockRequiresLightBeliefLevel
	belief_series_array[4] = game_config_template_ptr.DarkUnlockRequiresDarkBeliefLevel
	local page_type = math.floor(target_slot / ConstDefine.belief_rune_equip_slot_each_type_num) + 1;
	local page_solt = math.fmod(target_slot, ConstDefine.belief_rune_equip_slot_each_type_num) + 1;
	local level = tonumber(belief_series_array[page_type][page_solt]);
	return level;
end
function belief_rune:get_belief_rune_template(item_ptr)
	local item_template_ptr = template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id());
	if (nil == item_template_ptr)then
		return nil;
	end
	return template_manager:get_line("BeliefRunesTemplate", item_template_ptr.logic_id)
end

function belief_rune:refresh_att(player_ref, item_off, item_on)
	if (player_ref == nil) then
		return;
	end
	for k,v in pairs(item_off) do
		self:set_belief_rune_one_att(player_ref, v, false);
	end

	for k,v in pairs(item_on) do
		self:set_belief_rune_one_att(player_ref, v, true);
	end
end
function belief_rune:set_belief_rune_one_att(player_ref, item_ptr, is_add)
	local BeliefRunesTemplatePtr = self:get_belief_rune_template(item_ptr);
	if (nil == BeliefRunesTemplatePtr) then
		return false;
	end
	pawn_att_mgr:apply_att_change_by_array(player_ref, BeliefRunesTemplatePtr.BasicAtt1, is_add, 1);
	pawn_att_mgr:apply_att_change_by_array(player_ref, BeliefRunesTemplatePtr.BasicAtt2, is_add, 1);
	pawn_att_mgr:apply_att_change_by_array(player_ref, BeliefRunesTemplatePtr.BasicAtt3, is_add, 1);
	pawn_att_mgr:apply_att_change_by_array(player_ref, BeliefRunesTemplatePtr.UpgradeAttEach1, is_add, item_ptr:get_data_info(hld.e_item_info_upgrade_count));
	pawn_att_mgr:apply_att_change_by_array(player_ref, BeliefRunesTemplatePtr.UpgradeAttEach2, is_add, item_ptr:get_data_info(hld.e_item_info_upgrade_count));
	pawn_att_mgr:apply_att_change_by_array(player_ref, BeliefRunesTemplatePtr.UpgradeAttEach3, is_add, item_ptr:get_data_info(hld.e_item_info_upgrade_count));
	return true;
end
function belief_rune:response_operate_result(player_ref, operate_type, error_id, decompose_rune_piece)
	local resp = {};
	resp.operate_type = operate_type;
	resp.error_id = error_id;
	if (operate_type == hld.e_belief_rune_operate_type_decompose and decompose_rune_piece ~= nil and decompose_rune_piece > 0)then
		resp.rune_piece = decompose_rune_piece;
	end
	MessageMgr:send_message_to_self(player_ref, "hld.belief_rune_proto.belief_rune_operate_result", resp, 14403)
end
belief_rune_mgr = belief_rune
return belief_rune_mgr
