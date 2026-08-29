
--------------------数据---------------------
local max_tier_num = 9;						--最大层数
local max_customs_num = 6;					--最大关卡数
local max_award_num = 6;					--每层最大奖励次数
local oracle_trial_type_id = 78000000;		--天启试炼在活动表的id
local stat_num_award = {3,6,9,12,15,18};	--星级奖励每级奖励所需要的星数
local game_config_id = 99000001;
--------------------逻辑----------------------
local oracle_trial = {}
function oracle_trial:init()
end

define_C[15303] = function(message_data, player_ref)
	local msg_data = decode("hld.game_proto.get_oracle_trial_customs_award", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return
    end
	oracle_trial:get_stat_num_award(player_ref, msg_data.tier_num, msg_data.award_num);
end

define_C[15301] = function(message_data, player_ref)
	local msg_data = decode("hld.game_proto.send_oracle_trial_commpingup", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return
    end
	oracle_trial:send_mopping_up_award(player_ref);
end

--[[
函数说明:   游戏地图结束后的玩家信息的处理 包括(玩家的试炼信息更新, 试炼排行榜的排名信息更新, 发放星级奖励)
参数说明: 	player_guid		玩家的Guid
			trial_num		要改变信息的层数
			customs_num		要改变信息的关卡数
			customs_state	玩家通关后的评级  (0 一星通关, 1 二星通关. 2 三星通关)
--]]
function oracle_trial:oracle_trial_map_game_over(player_index, map_template_id, customs_state)
	local tier_num = oracle_trial:get_tier_num_by_template_id(map_template_id)
	local customs_num = oracle_trial:get_customs_num_by_template_id(map_template_id)
	if tier_num <= 0 or tier_num <= 0 then
		return
	end
	local player_ref = hld.get_player_by_index(player_index);
	local player_oracle_trial_info = player_ref:get_oracle_trial_mgr()
	local tier_state = player_oracle_trial_info:get_tier_customs_state_info(tier_num)
	if oracle_trial:is_can_award_value(tier_state, customs_num, customs_state+1) then
		local new_tier_state = oracle_trial:set_pow_num(tier_state, customs_num, customs_state+1)
		player_oracle_trial_info:send_fuben_over_info(map_template_id, oracle_trial:get_pow_num(tier_state, customs_num),customs_state+1)
		player_oracle_trial_info:set_play_oracle_trial_info(tier_num, customs_num, new_tier_state)	
	end
	local old_customs_state = oracle_trial:get_pow_num(tier_state, customs_num)
	for i = old_customs_state+1, customs_state+1  do
		local drop_id = oracle_trial:get_drop_id_by_map_id_and_stat_num(map_template_id, i)
		player_ref:send_oracle_trial_fuben_over_award(map_template_id,drop_id,i)
	end	
end

--[[
函数说明:	根据地图id和评级获取掉落包id
参数说明:	map_id		地图模板id
			stat_num	地图结算评级
返回值说明: 返回地图评级对应的地图模板id
--]]
function oracle_trial:get_drop_id_by_map_id_and_stat_num(map_id, stat_num)
	local drop_id_arr = {}
	drop_id_arr = template_manager:get_field("MapTemplate", map_id , "SettleLevelDropIdArr" )
	if stat_num > 3 or stat_num < 1 then
		return 0
	end 
	if drop_id_arr ~= nil and drop_id_arr[stat_num] ~= nil and tonumber(drop_id_arr[stat_num]) > 0 then
		return drop_id_arr[stat_num]
	end
	return 0
end


--[[
函数说明：	根据地图模板id获取地图所在的层数
参数说明:	map_id	地图模板id
返回值说明: 返回地图所在的层数
-]]
function oracle_trial:get_tier_num_by_template_id(map_id)
	local map_id_arr = {}
	local config_ptr =  template_manager:get_field("GameConfigTemplate", game_config_id, "OracleTrialTierNum" )
	for i = 1, tonumber(config_ptr) do
		map_id_arr = template_manager:get_field("OracleTrialTemplate", oracle_trial_type_id + i, "MapId" )
		for k,v in pairs(map_id_arr) do
			if tonumber(map_id_arr[k]) == tonumber(map_id) then	
				return i
			end
		end
	end
	return -1
end

--[[
函数说明：	根据地图模板id获取地图所在层的关卡索引
参数说明:	map_id	地图模板id
返回值说明: 返回地图所在的层的关卡索引
]]--
function oracle_trial:get_customs_num_by_template_id(map_id)
	local config_ptr =  template_manager:get_field("GameConfigTemplate", game_config_id, "OracleTrialTierNum" )
	for i = 1, tonumber(config_ptr) do
		local map_id_arr = template_manager:get_field("OracleTrialTemplate", oracle_trial_type_id + i, "MapId" )
		for k,v in pairs(map_id_arr) do
			if tonumber(map_id_arr[k]) == tonumber(map_id) then	
				return k
			end
		end
	end
	return -1
end

--[[
函数说明:	判断是否可以修改关卡状态
参数说明:	tier_state 	当前层的状态
			customs_num 	要改变的关卡
			customs_state	要改变的状态
返回值:	 	所有判断都通过返回 true  否则返回 false
--]]
function oracle_trial:is_can_award_value(tier_state, customs_num, customs_state)
	if customs_num == 1 then
		local old_customs_state = oracle_trial:get_pow_num(tier_state, customs_num)
		if old_customs_state >= 0 and old_customs_state < customs_state then
			return true
		end
	end	
	local old_customs_state = oracle_trial:get_pow_num(tier_state, customs_num - 1)	
	if old_customs_state <= 0 and  customs_num > max_customs_num then	--判读前一关卡是否通过了
		return false
	end
	old_customs_state = oracle_trial:get_pow_num(tier_state, customs_num);
	if old_customs_state == -1 or old_customs_state >= customs_state then	--判读当前关卡是否可以修改
		return false
	end
	return true
end

--[[
函数说明: 	领取指定星级奖励并且设置奖励状态
参数说明:	player_guid		玩家的Guid
			trial_num		玩家要领取奖励的层数
			award_num		玩家要领取的那个星级奖励
返回值说明	如果领取成功返回1 领取失败返回错误提示	 2玩家已经领取过来 3玩家的星数不足	4奖励领取失败 未知错误
--]]
function oracle_trial:get_stat_num_award(player_ref, trial_num, award_num)
	local player_oracle_trial_info = player_ref:get_oracle_trial_mgr()
	local award_state = player_oracle_trial_info:get_tier_reward_state_info(trial_num)
	local customs_state = player_oracle_trial_info:get_tier_customs_state_info(trial_num)
	local get_award_state = oracle_trial:is_can_get_stat_num_award(customs_state, award_state, award_num)	
	local customs_error_index = 4	
	if get_award_state == 1 then
		local award_id = oracle_trial:get_stat_num_award_drop_id(trial_num, award_num)
		if award_id ~= 0 then
			player_ref:send_stat_num_award(award_id);		--发送玩家奖励
			award_state = award_state + (10^(award_num - 1))
			player_oracle_trial_info:set_play_oracle_trial_reward_info(trial_num, award_num, award_state);	--设置奖励领取状态
			customs_error_index = 1
		end
	elseif get_award_state == 2 then
		customs_error_index = 2
	elseif get_award_state == 3 then
		customs_error_index = 3
	end
	print("zym",123)
	local MsgData = 
	{
        error_index = customs_error_index,
	}
	MessageMgr:send_message_to_self(player_ref, "hld.game_proto.send_oracle_trial_customs_award_end",MsgData, 15304)

end

--[[
函数说明:	判断玩家是否可以领取星级奖励
参数说明:	customs_state 当前层数关卡状态
			award_state	  当前层数的奖励领取状态
			award_num	  要领取的是第几个奖励
返回值说明: 如果可以领取 返回1	不能领取返回错误提示 2玩家已经领取过来 3玩家的星数不足
--]]

function oracle_trial:is_can_get_stat_num_award(customs_state, award_state, award_num)
	local _tier_state = customs_state
	local stat_num 
	while(1)
	do
		if stat_num == nil then
			stat_num =  _tier_state % 10
		else
			stat_num = stat_num + _tier_state % 10
		end
		_tier_state = _tier_state / 10
		_tier_state = _tier_state - (_tier_state % 1)
		if _tier_state < 10 then
			stat_num = stat_num + _tier_state
			break
		end
	end
	if stat_num >= stat_num_award[award_num] then
		if award_state < (10^(award_num - 1)) then
			return 1
		end
		if tonumber(oracle_trial:get_pow_num(award_state, award_num)) == 0 then
			return 1
		else
			return 2
		end
	end
	return 3
end

--[[
函数说明:	 根据参数获取指定层中指定位置的奖励id(掉落包id)
参数说明:	 trial_num	指定的层数
			 award_num	指定的奖励
返回值说明	 获取奖励id成功返回奖励id	失败返回 0
--]]
function oracle_trial:get_stat_num_award_drop_id(trial_num, award_num)
	local award_drop_id_arr = template_manager:get_field("OracleTrialTemplate", oracle_trial_type_id + trial_num, "AwardId" )
	if award_drop_id_arr[award_num] ~= nil or tonumber(award_drop_id_arr[award_num]) > 0 then
		return tonumber(award_drop_id_arr[award_num])
	end
	return 0
end

--[[
函数说明: 根据层数和关卡数获得对应的地图id
参数说明:	 tier_num		指定的层数
			 customs_num	指定的关卡索引
返回值说明:	 返回指定的地图模板id
--]]
function oracle_trial:get_map_template_id_by_tier_and_customs(tier_num, customs_num)	
	local map_id_arr = template_manager:get_field("OracleTrialTemplate", oracle_trial_type_id + tier_num, "MapId" )
	if map_id_arr[customs_num] ~= nil or tonumber(map_id_arr[customs_num]) > 0 then
		return tonumber(map_id_arr[customs_num])
	end
	return 0
end

--[[
函数说明:	获取所有扫荡物品列表
参数说明:	role_guid 要获取扫荡物品列表的玩家guid
返回值说明	返回要发送的物品列表(类型是 drop_list_with_num)
--]]

function oracle_trial:oracle_get_comppingup_item_list(player_index)
	local player_ref = hld.get_player_by_index(player_index);
	local player_oracle_trial_info = player_ref:get_oracle_trial_mgr()
	local hight_rank = player_oracle_trial_info:get_hight_rank_num()
	if hight_rank < 10 then
		return 6
	end
	local drop_list = {
		item_id = {},
		item_num = {}
	}
	local config_ptr =  template_manager:get_field("GameConfigTemplate", game_config_id, "OracleTrialTierNum" )
	for i = 1, hight_rank/10 do
		if i > tonumber(config_ptr) then
			break
		end
		local tier_state = player_oracle_trial_info:get_tier_customs_state_info(i)
		for o = 1, max_customs_num do
			if i * 10 + o > hight_rank then
				break
			end
			local map_id = oracle_trial:get_map_template_id_by_tier_and_customs(i, o)
			local drop_id = template_manager:get_field("MapTemplate", map_id, "MoppingUpRandomAwardItems")
			local _drop_list = player_oracle_trial_info:get_drop_list_by_id(drop_id)
			if _drop_list ~= nil then
				for i = 0, _drop_list["num"] - 1 do
					if i <= 10 then
						table.insert(drop_list["item_id"],  _drop_list["item_id"][i])
						table.insert(drop_list["item_num"], _drop_list["item_num"][i])
					end
				end
			end

			if tonumber(tier_state) < 10 then
				break
			end
			tier_state = tonumber(tier_state)/10
			tier_state = tonumber(tier_state) - tonumber(tier_state)%1
		end
	end

	local new_drop_list = oracle_trial:tidy_drop_list(drop_list);
	local item_list = {};
	for k,v in pairs(new_drop_list["item_id"]) do
		table.insert(item_list, v)
	end
		local data_num = new_drop_list["data_num"]
	local item_num = {};
	for k,v in pairs(new_drop_list["item_num"]) do
		table.insert(item_num, v)
	end
	local table_num = #(item_list)
	for i = table_num, 10 do
		table.insert(item_list, -1)
	end
	table_num = #(item_num)
	for i = table_num, 10 do
		table.insert(item_num, -1)
	end
	player_oracle_trial_info:send_commingup_item_list(item_list,item_num, data_num)
	return 5
end

--[[
函数说明:	根据地图获取扫荡物品列表
参数说明:	map_id 地图id
返回值说明	返回扫荡物品列表
--]]
function oracle_trial:get_drop_list_by_map_id(map_id)
	local item_list = {
		item_id = {},
		item_num = {}
	}
	local drop_id = {}
	drop_id = template_manager:get_field("MapTemplate", map_id, "MoppingUpRandomAwardItems")
	local DropTemplate = template_manager:get_line("DropTemplate", drop_id)
	if nil == DropTemplate then
		return {}
	end
	local item_arr = {}
	item_arr = DropTemplate.IDs
	for k,v in pairs(item_arr) do
		table.insert(item_list["item_id"],v)
	end
	local item_num = {}
	item_num = DropTemplate.Counts
	for i = 1, #item_list["item_id"] do
		table.insert(item_list["item_num"],tonumber(item_num[i*2]))
	end
	return item_list
end

--[[
函数说明:	对扫荡物品列表进行整理
参数说明:	old_drop_list 所有扫荡物品可能会有重复
返回值说明	对扫荡物品列表进行整理返回没有重复的物品列表
]]--
function oracle_trial:tidy_drop_list(old_drop_list)
	local new_drop_list = {
		item_id = {},
		item_num = {},
		data_num = 0
	}	
	for k,v in pairs(old_drop_list["item_id"]) do
		local is_insert = true
			for i, o in pairs(new_drop_list["item_id"]) do
				if v == o then
					new_drop_list["item_num"][i] = new_drop_list["item_num"][i] + old_drop_list["item_num"][k]
					is_insert = false
				end
			end
		if is_insert then
			table.insert(new_drop_list["item_id"], old_drop_list["item_id"][k])
			table.insert(new_drop_list["item_num"],old_drop_list["item_num"][k])
			new_drop_list["data_num"] = new_drop_list["data_num"] + 1
		end
	end
	return new_drop_list
end

--[[
函数说明:	根据第二个参数获取第一个参数中的值	例如: get_pow_num(984, 2) 返回值8 get_pow_num(891234, 4) 返回值1
参数说明:	num 一个int32 数字
			square_num  要寻找数字的索引值
返回值说明:	返回参数1指定位置的值
--]]
function oracle_trial:get_pow_num(num, square_num)
	if square_num == 1 then
		return num%10
	elseif square_num > 1 then
		if num < (10^(square_num - 1)) then
			return 0;
		end
		num =  (num / (10^(square_num - 1))) % 10
		num = num - (num % 1)
		return num
	end
	return -1
end

--[[
函数说明:	将参数1指定位置的数字改成参数3  返回更改后的值
参数说明:	num要更改的值
			square_num 要更改的位置
			value要改成的值
返回值说明:	返回更改后的值
--]]
function oracle_trial:set_pow_num(num, square_num, value)
	if num < (10^(square_num - 1)) then
		return num + (10^(square_num - 1))*value;
	end
	return num + (value - oracle_trial:get_pow_num(num, square_num)) * (10^(square_num-1))
end

function oracle_trial:send_mopping_up_award(player_ref)
	if player_ref:is_valid() == false or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	player_ref:get_oracle_trial_mgr():send_mopping_up_award();
end
oracle_trial_mgr = oracle_trial
return oracle_trial_mgr;