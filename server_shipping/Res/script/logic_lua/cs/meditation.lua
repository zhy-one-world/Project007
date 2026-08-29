local meditation_class={}

function meditation_class:init()
	--dump("----0------------")
end
--功能消息入口
define_C[10458] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.get_meditation_reward", message_data)
	meditation_class:operate_meditation(player_ref, msg_data.get_type)
end

function meditation_class:operate_meditation( player_ref, get_type )
	if get_type < Meditationdef.e_meditation_reward_usual or get_type > Meditationdef.e_meditation_reward_four then
		return 
	end
	if player_ref == nil then
		return
	end
	local msg = {get_type = tonumber(get_type)}
	local meditation_time = player_ref:get_time_data(hld.e_time_type_have_meditation_time) / ConstDefine.minute_tick_time;
	
	if math.floor(meditation_time)  <= 0 then
		msg.endType = Meditationdef.e_meditation_reward_less_one_munite
		MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
		return
	end
	local cur_vip_level = player_ref:get_vip_level()

	local vip_template_ptr = template_manager:get_template_by_vip_level(cur_vip_level);
	if vip_template_ptr == nil then
		msg.endType = Meditationdef.e_meditation_reward_unknown
		MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
		return
	end
	
	local player_upgrade_template_ptr = template_manager:get_line("PlayerUpgradeTemplate",player_ref:get_unit_info(hld.e_role_info_upgrade_id))
	if player_upgrade_template_ptr == nil then
		msg.endType = Meditationdef.e_meditation_reward_unknown
		MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
		return
	end
	local game_config_ptr = template_manager:get_line("GameConfigTemplate", 99000001)
	if game_config_ptr == nil then
		msg.endType = Meditationdef.e_meditation_reward_unknown
		MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
		return
	end
	
	local exp_num= 0 
	local cost_money_type = -1
	local add_exp_string = ""
	local cost_money_value = 0
	local is_add_exp_string = 0
	
	if get_type == Meditationdef.e_meditation_reward_usual then
		exp_num = tonumber(player_upgrade_template_ptr.BaseExp * game_config_ptr.MeditationExpRewardRadio)
		exp_num = math.floor( exp_num )
		local meditation_standard_gs =player_upgrade_template_ptr.MeditationStandardGs
		if meditation_standard_gs > 0 then
			local gs_value = player_ref:get_unit_gs_value() - meditation_standard_gs
			if gs_value >= 0 then
				if gs_value > meditation_standard_gs *3 then
					gs_value = meditation_standard_gs * 3
				end
				is_add_exp_string = 1
				add_exp_string = player_ref:get_meditation_mgr():lua_meditation_add_exp(get_type,gs_value,tonumber(player_upgrade_template_ptr.BaseExp),tonumber(player_upgrade_template_ptr.MeditationExpAddition),tonumber(game_config_ptr.MeditationExpRewardRadio ), meditation_standard_gs , 100 , tonumber(meditation_time))
			end
			
		end
		exp_num = math.floor(meditation_time) * exp_num 
		exp_num = math.floor( exp_num )
		player_ref:set_time_data(hld.e_time_type_meditation_full_send_mail, 0) --成功领取奖励后将标记置位 0 ，表示当下次冥想满时可再次发送邮件
	end
	
	if get_type == Meditationdef.e_meditation_reward_four then
		
		if vip_template_ptr.IsFourfoldMeditationBonuseGet <= 0 then
			msg.endType = Meditationdef.e_meditation_reward_vip
			MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
			return
		end
		exp_num = player_upgrade_template_ptr.BaseExp * game_config_ptr.MeditationExpRewardRadio           --基础经验
		exp_num = math.floor( exp_num )
		local meditation_standard_gs = player_upgrade_template_ptr.MeditationStandardGs
		if meditation_standard_gs > 0 then
			local gs_value = player_ref:get_unit_gs_value() - meditation_standard_gs
			if gs_value >= 0 then
				if gs_value > meditation_standard_gs * 3 then
					gs_value = meditation_standard_gs * 3
				end
				is_add_exp_string = 1
				add_exp_string = player_ref:get_meditation_mgr():lua_meditation_add_exp(get_type,gs_value,tonumber(player_upgrade_template_ptr.BaseExp),tonumber(player_upgrade_template_ptr.MeditationExpAddition),tonumber(game_config_ptr.MeditationExpRewardRadio) , meditation_standard_gs , 100 , tonumber(meditation_time))
			end
		end
		
		exp_num = exp_num * 1.5
		exp_num = math.floor( exp_num )
		exp_num = math.floor(meditation_time) * exp_num
		exp_num = math.floor( exp_num )
		cost_money_type = hld.e_money_type_jewel_bind
		cost_money_value = meditation_time * game_config_ptr.MeditationDiamondCostRadio
		if math.floor(cost_money_value) <= 0 then
			cost_money_value = 1 
		end
		player_ref:set_time_data(hld.e_time_type_meditation_full_send_mail , 0)
	end
	if cost_money_type >= 0 then
		if false == player_ref:can_cut_money(tonumber(cost_money_type) , math.floor(cost_money_value)) then
			msg.endType = Meditationdef.e_meditation_reward_money_lack
			MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
			return
		end
		player_ref:cut_money(tonumber(cost_money_type) , math.floor(cost_money_value), hld.e_server_log_cut_money_meditation ,0 , 0);
	end
	local showed_exp = 0

	if is_add_exp_string == 0 then
		showed_exp = player_ref:lua_add_exp(exp_num)
	end
	if is_add_exp_string == 1 then
		showed_exp = player_ref:lua_add_exp(add_exp_string)
	end
	player_ref:set_time_data(hld.e_time_type_have_meditation_time , 0)
	msg.endType = Meditationdef.e_meditation_get_reward_success
	MessageMgr:send_message_to_self(player_ref, "hld.character_proto.get_meditation_reward_end",msg, 10459)
	local client_pak = {item_data = {}}
	table.insert( client_pak.item_data,tonumber(showed_exp))
	MessageMgr:send_message_to_self(player_ref, "hld.item_proto.get_meditation_item_msg",client_pak, 10747)
	hld.server_log:getmeditation_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), get_type, meditation_time)	
end


meditation = meditation_class
return meditation