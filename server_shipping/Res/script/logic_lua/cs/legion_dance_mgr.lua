--------------------数据---------------------
m_game_config_id = 99000001;
--------------------逻辑----------------------
local legion_dance = {}
function legion_dance:init()
end
define_C[15502] = function(message_data, player_ref)
	local msg_data = decode("hld.legion_proto.invite_legion_dance", message_data)
	legion_dance:invite_legion_dance(player_ref, msg_data.play_guid_a, msg_data.play_guid_b);
end
define_C[15505] = function(message_data, player_ref)
	local msg_data = decode("hld.legion_proto.be_invite_legion_dance_end", message_data)
	if msg_data.is_reception and msg_data.play_guid_a > 0 then
		legion_dance:accept_inviter_legion_dance(player_ref, msg_data.play_guid_a, msg_data.play_guid_b);
	else
		local target_ref = hld.get_player_by_guid(msg_data.play_guid_a, msg_data.play_guid_b);
		if nil ~= target_ref then
			target_ref:get_legion_dance_mgr():send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_target_refuse,tostring(player_ref:get_name()));
		else
			player_ref:get_legion_dance_mgr():send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_player_info_error, tostring(0));
		end
	end
end
define_C[15508] = function(message_data, player_ref)
	--local msg_data = decode("hld.legion_proto.be_invite_legion_dance_end", message_data)
	legion_dance:legion_dance_end(player_ref);
end
--[[
函数说明: 	传功结束给予经验
参数说明:	player_index		传功结束的玩家
返回值说明:	
--]]
function legion_dance:legion_dance_end_award(player_ref, target_ref)
	if nil == target_ref then
		player_ref:get_legion_dance_mgr():send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_player_info_error, tostring(0));
		return ;
	end
	local exp_num = legion_dance:get_legion_dance_exp(player_ref:get_unit_info(hld.e_role_info_exp_level), target_ref:get_unit_info(hld.e_role_info_exp_level), player_ref:get_legion_dance_mgr():get_world_server_last_level());
	if tonumber(exp_num) <= 0 then
		return
	end
	player_ref:lua_add_exp(tostring(exp_num), true);
	local add_exp_num = player_ref:get_unit_info(hld.e_role_info_legion_dance_exp_num) + 1;
	player_ref:set_unit_info(hld.e_role_info_legion_dance_exp_num, add_exp_num);
	player_ref:send_info_one(hld.e_role_info_legion_dance_exp_num);
	player_ref:add_legion_dance_daily_must();
end

--[[
函数说明: 	根据等级和等级差获取玩家要获得的经验
参数说明:	play_level			玩家等级 
			level_gap			玩家等级差
返回值说明:	返回要给予的经验
--]]
function legion_dance:get_legion_dance_exp(play_level, target_level, server_level)
	local _exp = 0;
	local level_exp = 0;
	if tonumber(server_level) <= 0 then
		local config_level = template_manager:get_field("GameConfigTemplate", m_game_config_id, "FirstExpLevel");
		if tonumber(config_level) ~= '' then
		   server_level = tonumber(config_level)
		end
	end
	local exp_choice = template_manager:get_field("ActivityCommonConfigTemplate", 120000024, "ParamIntArr1");
		if	nil == next(exp_choice) then
		 return _exp;
	 end
	local rule_level = template_manager:get_field("GameConfigTemplate", m_game_config_id, "ExpServerEndLevel");
	if rule_level <= play_level then	--判断基础等级是自己等级还是服务器等级
		level_exp =  template_manager:get_field("PlayerUpgradeTemplate", (21000000 + tonumber(server_level)), "BaseExp");		--获取等级对应的经验系数
	else 
		level_exp =  template_manager:get_field("PlayerUpgradeTemplate", (21000000 + tonumber(play_level)), "BaseExp");
	end
	local foundation = tonumber(exp_choice[2]);																				--获得基础经验系数
	local level_max_difference = template_manager:get_field("ActivityCommonConfigTemplate", 120000024, "ParamInt1");	--活动最大等级差
	local level_difference = 0;
	if play_level > target_level then																				--计算当前等级差
		level_difference = play_level - target_level;
	else
		level_difference = target_level - play_level;
	end
	
	if level_max_difference < level_difference then
		level_difference = level_max_difference;
	end
	_exp = self:LegionDanceExpCount(tonumber(level_exp), foundation, level_difference)
	return tonumber(_exp)
end

--[[
函数说明: 根据基础系数和等级差计算结果
参数说明: level_exp			等级对应的经验系数
		  foundation 		基础系数
		  level_difference	等级差
返回值:	  返回计算结果
--]]
function legion_dance:LegionDanceExpCount(level_exp, foundation, level_difference)
	local _exp = foundation * level_exp;	--计算基础经验
	_exp = _exp * (0.5 + (level_difference / 100))
	return _exp
end

--[[
函数说明: 	共舞结束处理
参数说明:	player_index			玩家索引
返回值说明:	
--]]
function legion_dance:legion_dance_end(player_ref)
	local target_guid = player_ref:get_interaction_mgr():get_interaction_guid();
	local target_ref= hld.get_player_by_guid(target_guid);
	if nil == target_ref then	
		return
	end
	player_ref:get_interaction_mgr():stop_interaction();	--结束互动
	--判断奖励获取次数是否足够
	local exp_num = player_ref:get_unit_info(hld.e_role_info_legion_dance_exp_num);
	local target_exp_num = target_ref:get_unit_info(hld.e_role_info_legion_dance_exp_num);
	local exp_max_num = template_manager:get_field("ActivityCommonConfigTemplate", 120000024, "ParamInt2")
	if exp_num < exp_max_num then
		self:legion_dance_end_award(player_ref, target_ref)
	end
	if target_exp_num < exp_max_num then
		self:legion_dance_end_award(target_ref, player_ref)
	end
	
end

--[[
函数说明:	邀请玩家进行共舞
参数说明:	player_index 自己的索引
			target_guid  目标的guid
返回值
--]]
function legion_dance:invite_legion_dance(player_ref, target_guid_a, target_guid_b)
	local player_legion_dance = player_ref:get_legion_dance_mgr();
	local target_ref = hld.get_player_by_guid(target_guid_a, target_guid_b);
	if false == self:Is_Can_Invite_Legion_Dance(player_ref, target_ref) or nil == target_ref then
		return;
	end
	player_legion_dance:send_legion_dance_invite(target_ref:get_array_index());
end	

--[[
函数说明:	判断玩家是否可以邀请共舞
参数说明:	player_index 自己的索引
			target_guid  目标的guid
返回值:		状态正常返回true, 不可以返回false
--]]	
function legion_dance:Is_Can_Invite_Legion_Dance(player_ref, target_ref)
	local player_legion_dance = player_ref:get_legion_dance_mgr();

	if nil == target_ref then																					--判断玩家信息是否有效
		player_legion_dance:send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_player_info_error, tostring(0));
		return false;
	end

	local map_id = target_ref:get_unit_info(hld.e_role_info_move_map_id);
	local map_type = template_manager:get_field("MapTemplate", map_id , "Type")
	if  map_type ~= hld.e_map_type_legion_station then														--判断玩家是否在军团驻地中
		player_legion_dance:send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_not_in_map, tostring(target_ref:get_name()));	
		return false;
	end

	if target_ref:get_session_state() ~= hld.e_session_status_in_gaming then									--判断玩家是否在线
		player_legion_dance:send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_not_in_game,tostring(target_ref:get_name()));
		return false;
	end

	if target_ref:get_game_att(hld.e_unit_game_att_war_state) == hld.e_war_state_attack_in then			--判断玩家是否在战斗状态
		player_legion_dance:send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_in_war, tostring(target_ref:get_name()));
		return false;
	end

	if player_ref:get_game_att(hld.e_unit_game_att_war_state) == hld.e_war_state_attack_in then				--判断自己是否在战斗状态
		player_legion_dance:send_legion_dance_invite_error_tip(hld.e_legion_dance_invite_error_my_in_war, tostring(target_ref:get_name()));
		return false;
	end

	return true
end

--[[
函数说明:	判断玩家是否可以接受共舞
参数说明:	player_index 自己的索引
			target_guid  目标的guid
返回值:		状态正常返回true, 不可以返回false
--]]	
function legion_dance:Is_Can_Accept_Legion_Dance(player_ref, target_ref)
	local player_legion_dance = player_ref:get_legion_dance_mgr();

	if nil == target_ref then																					--判断玩家信息是否有效
		player_legion_dance:send_legion_dance_accept_error_tip(hld.e_legion_dance_accept_error_player_info_error, tostring(0));
		return false;
	end

	if player_ref:get_interaction_mgr():get_interaction_state() > hld.e_interaction_state_wait_reply then		--判断玩家是否可以进行共舞
		player_legion_dance:send_legion_dance_accept_error_tip(hld.e_legion_dance_accept_error_in_interactive, tostring(target_ref:get_name()));
		return false;
	end

	if target_ref:get_session_state() ~= hld.e_session_status_in_gaming then									--判断玩家是否在线
		player_legion_dance:send_legion_dance_accept_error_tip(hld.e_legion_dance_accept_error_not_in_game, tostring(target_ref:get_name()));
		return false;
	end

	local map_id = target_ref:get_unit_info(hld.e_role_info_move_map_id);
	local map_type = template_manager:get_field("MapTemplate", map_id , "Type")
	if  map_type ~= hld.e_map_type_legion_station then														--判断玩家是否在军团驻地中
		player_legion_dance:send_legion_dance_accept_error_tip(hld.e_legion_dance_accept_error_not_in_map, tostring(target_ref:get_name()));	
		return false;
	end
	if target_ref:get_game_att(hld.e_unit_game_att_war_state) == hld.e_war_state_attack_in then				--判断玩家是否在战斗状态
		player_legion_dance:send_legion_dance_accept_error_tip(hld.e_legion_dance_accept_error_in_war, tostring(target_ref:get_name()));
		return false;
	end
	if player_ref:get_game_att(hld.e_unit_game_att_war_state) == hld.e_war_state_attack_in then				--判断自己是否在战斗状态
		player_legion_dance:send_legion_dance_accept_error_tip(hld.e_legion_dance_accept_error_my_in_war,tostring(target_ref:get_name()));
		return false;
	end
	
	return true
end

	
	
--[[
函数说明:	接受玩家进行共舞的处理
参数说明:	player_index 自己的索引
			target_guid  目标的guid
返回值
--]]
function legion_dance:accept_inviter_legion_dance(player_ref, target_guid_a, target_guid_b)	
	--当前是被邀请方 target_guid是邀请方的guid
	local player_legion_dance = player_ref:get_legion_dance_mgr();
	local target_ref = hld.get_player_by_guid(target_guid_a, target_guid_b);
	if false == self:Is_Can_Accept_Legion_Dance(player_ref, target_ref) or nil == target_ref then
		return;
	end
	--以下部分是开始进行交互
	player_ref:get_interaction_mgr():set_is_inviter(false);
	target_ref:get_interaction_mgr():set_is_inviter(true);
	player_ref:get_interaction_mgr():set_interaction_info(target_ref:get_unit_guid(), hld.e_interaction_type_yao_legion_dance);
	target_ref:get_interaction_mgr():set_interaction_info(player_ref:get_unit_guid(), hld.e_interaction_type_yao_legion_dance);
	player_ref:get_interaction_mgr():send_interaction_answer_to_two_side(target_ref:get_unit_guid(), hld.e_interaction_type_yao_legion_dance, hld.e_interaction_reply_agree);
	player_ref:get_legion_dance_mgr():legion_dance_official_start(target_ref:get_unit_guid());
end

return legion_dance;