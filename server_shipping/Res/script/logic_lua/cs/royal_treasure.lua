local royal_treasure = {}
local Treasure_Data = {}
local royal_treasure_data = {}
local royal_treasure_percent = {}
local element_treasure_data = {}
local element_treasure_percent = {}
local spirit_treasure_data = {}
local spirit_treasure_percent = {}
function royal_treasure:init()
	package.loaded["Treasure"] = nil
	Treasure_Data = require("Treasure")
	royal_treasure_data = Treasure_Data["royal_treasure_data"]
	royal_treasure_percent = Treasure_Data["royal_treasure_percent"]
	element_treasure_data = Treasure_Data["element_treasure_data"]
	element_treasure_percent = Treasure_Data["element_treasure_percent"]
	spirit_treasure_data = Treasure_Data["spirit_treasure_data"]
	spirit_treasure_percent = Treasure_Data["spirit_treasure_percent"]
	--新服转盘
	new_server_treasure_data = Treasure_Data["new_server_treasure_data"]
	new_server_treasure_percent = Treasure_Data["new_server_treasure_percent"]
	--限时活动转盘
	time_limit_activity_treasure_data2 = Treasure_Data["time_limit_activity_treasure_data2"]
	time_limit_activity_treasure_percent2 = Treasure_Data["time_limit_activity_treasure_percent2"]
end
define_C[10502] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.treasure_get_prize", message_data)
	royal_treasure:treasure_chou_jiang(player_ref, msg_data.operate_type, msg_data.prize_type);
end

define_C[10527] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.get_person_infor_reward", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	player_ref:get_person_infor_mgr():get_reward();
end

function royal_treasure:treasure_chou_jiang(player_ref, treasure_type, money_type)
	local target_treasure_data = {}
	local target_treasure_percent = {}
	local now_score = 0
	local is_first = 0
	local is_free = 0
	local is_time_limit_activity = 0
	local award_index = 0
	if treasure_type == hld.e_treasure_type_item then --皇家
		if(player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_treasure) == true) then
			is_time_limit_activity = 1
			award_index = player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_treasure)
		elseif (player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_treasure_royal1) == true) then
			is_time_limit_activity = 1
			award_index = player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_treasure_royal1)
		elseif (player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_treasure_royal2) == true) then
			is_time_limit_activity = 1
			award_index = player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_treasure_royal2)
		elseif (player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_treasure_royal3) == true) then
			is_time_limit_activity = 1
			award_index = player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_treasure_royal3)
		end
		now_score = player_ref:get_logic_data(hld.e_role_logic_info_jewel_luck);
		is_first = player_ref:get_logic_data(hld.e_role_logic_info_jewel_first_one);
		is_free = player_ref:get_logic_data(hld.e_role_logic_info_jewel_free);

		if money_type == hld.e_treasure_prize_type_ten or money_type == hld.e_treasure_prize_type_ten_item then
			is_first = player_ref:get_logic_data(hld.e_role_logic_info_jewel_first_ten);
		end

		target_treasure_data = royal_treasure_data
		target_treasure_percent = royal_treasure_percent
	elseif treasure_type == hld.e_treasure_type_element then--符文
		if(player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_treasure_element) == true) then
			is_time_limit_activity = 1
			award_index = player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_treasure_element)
		end
		now_score = player_ref:get_logic_data(hld.e_role_logic_info_element_luck);
		is_first = player_ref:get_logic_data(hld.e_role_logic_info_element_first_one);
		is_free = player_ref:get_logic_data(hld.e_role_logic_info_element_free);

		if money_type == hld.e_treasure_prize_type_ten or money_type == hld.e_treasure_prize_type_ten_item then
			is_first = player_ref:get_logic_data(hld.e_role_logic_info_element_first_ten);
		end

		target_treasure_data = element_treasure_data
		target_treasure_percent = element_treasure_percent
	elseif treasure_type == hld.e_treasure_type_spirit then --精灵
		if(player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_treasure_sprite) == true) then
			is_time_limit_activity = 1
			award_index = player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_treasure_sprite)
		end
		now_score = player_ref:get_logic_data(hld.e_role_logic_info_spirit_luck);
		is_first = player_ref:get_logic_data(hld.e_role_logic_info_spirit_first_one);
		is_free = player_ref:get_logic_data(hld.e_role_logic_info_spirit_free);

		if money_type == hld.e_treasure_prize_type_ten or money_type == hld.e_treasure_prize_type_ten_item then
			is_first = player_ref:get_logic_data(hld.e_role_logic_info_spirit_first_ten);
		end

		target_treasure_data = spirit_treasure_data
		target_treasure_percent = spirit_treasure_percent
	elseif treasure_type == hld.e_treasure_type_new_server then --新服
		is_time_limit_activity = 0
		now_score = 0
		is_first = 2
		is_free = 0
		target_treasure_data = new_server_treasure_data[1]
		target_treasure_percent = new_server_treasure_percent[1]
	elseif treasure_type == hld.e_treasure_type_time_limit then --限时转盘
		now_score = 0
		is_first = 2
		is_free = 0
		is_time_limit_activity = 0
		
		target_treasure_data = time_limit_activity_treasure_data2[1]
		target_treasure_percent = time_limit_activity_treasure_percent2[1]
		if(player_ref:get_time_limit_activity_mgr():is_activity_open(hld.e_time_limit_activity_type_limit_time_treasure2) == true) then
			now_score = player_ref:get_time_limit_activity_mgr():get_activity_schedule(hld.e_time_limit_activity_type_limit_time_treasure2, 1, 0)
			award_index	= player_ref:get_time_limit_activity_mgr():get_treasure_lua_award_index(hld.e_time_limit_activity_type_limit_time_treasure2)
			target_treasure_data = time_limit_activity_treasure_data2[award_index]
			target_treasure_percent = time_limit_activity_treasure_percent2[award_index]
		end
	end
	
	if is_time_limit_activity ~= nil and is_time_limit_activity == 1 and award_index ~= nil and award_index ~= 0 and treasure_type ~= hld.e_treasure_type_time_limit  then
		if #Treasure_Data["time_limit_activity_treasure_data"] >= award_index and #Treasure_Data["time_limit_activity_treasure_percent"] >= award_index then
			target_treasure_data = Treasure_Data["time_limit_activity_treasure_data"][award_index]
			target_treasure_percent = Treasure_Data["time_limit_activity_treasure_percent"][award_index]
		end
	end
	self:get_item_list(target_treasure_data, target_treasure_percent, now_score, is_first, player_ref, treasure_type, money_type, is_free);
end

function royal_treasure:get_item_list(treasure_data, treasure_percent, now_score, is_first, player_ref, treasure_type, money_type, is_free)
	local percent_index = 1;
	if money_type == hld.e_treasure_prize_type_ten or money_type == hld.e_treasure_prize_type_ten_item then
		percent_index = 10;
	end
	local prize_data = treasure_percent["prize_"..percent_index];
	local res_table = {cost_money_type = prize_data["cost_money"]["money_type"], cost_money_value=prize_data["cost_money"]["money_value"], prize_luck = 0, prize_index = {}};
	if money_type == hld.e_treasure_prize_type_one_item or money_type == hld.e_treasure_prize_type_ten_item then
		res_table.cost_money_type = prize_data["cost_item"]["item_id"];
		res_table.cost_money_value = prize_data["cost_item"]["item_num"];
	end
	local add_score = 0
	local add_item = {}
	if money_type == hld.e_treasure_prize_type_one or money_type == hld.e_treasure_prize_type_one_item then
		self:get_one_prize(treasure_percent["prize_1"], now_score, is_first, res_table, is_free,money_type)

		if money_type == hld.e_treasure_prize_type_one then
			add_score = treasure_percent["prize_1"]["add_score"]
			add_item = treasure_percent["prize_1"]["add_item"]
		else
			add_score = 0
			if treasure_type == hld.e_treasure_type_time_limit then
				add_score =  treasure_percent["prize_1"]["add_score"]
			end
			add_item["item_num"] = 0
			add_item["item_id"] = 0
			if res_table["prize_luck"] > 0 then
				res_table["prize_luck"] = 0
			end
		end
	else
		self:get_ten_prize(treasure_percent["prize_10"], now_score, is_first, res_table, 0,money_type)

		if money_type == hld.e_treasure_prize_type_ten then
			add_score = treasure_percent["prize_10"]["add_score"]
			add_item = treasure_percent["prize_10"]["add_item"]
		else
			add_score = 0
			if treasure_type == hld.e_treasure_type_time_limit then
				add_score =  treasure_percent["prize_10"]["add_score"]
			end
			add_item["item_num"] = 0
			add_item["item_id"] = 0
			if res_table["prize_luck"] > 0 then
				res_table["prize_luck"] = 0
			end
		end
	end
	local przie_list = {};
	for k,v in pairs(res_table["prize_index"]) do
		table.insert(przie_list, v - 1)
		table.insert(przie_list, treasure_data[v]["item_id"])
		table.insert(przie_list, treasure_data[v]["item_num"])
	end
	local table_num = #(przie_list);
	for i = table_num, 30 do
		table.insert(przie_list, -1)
	end
	player_ref:treasure_prize(treasure_type, money_type, res_table["cost_money_type"], res_table["cost_money_value"], add_score, res_table["prize_luck"], add_item["item_id"], add_item["item_num"], przie_list);
end
function royal_treasure:get_prize_by_list(prize_list)
	local random_num = math.random(1,10000);
	local res_index = prize_list[1]["prize_index"];
	local max_num = 0;
	for k,v in pairs(prize_list) do
		max_num = max_num + v["prize_percent"];
		if max_num >= random_num then
			return v["prize_index"];
		end
	end
	return res_index;
end
function royal_treasure:get_one_prize(treasure_percent, now_score, is_first, res_table, is_free,money_type)
	res_table["prize_luck"] = treasure_percent["add_luck"];
	if is_first == 0 then --第一次
		res_table["prize_index"][1] = self:get_prize_by_list(treasure_percent["first"]);
	elseif is_free == 1 then --免费
		res_table["prize_index"][1] = self:get_prize_by_list(treasure_percent["free"]);
	else
		local next_prize = treasure_percent["next"];
		local nomal_prize = next_prize["nomal_prize"];
		local best_prize = next_prize["best_prize"];
		local random_num = math.random(1,10000);
		local best_random = best_prize["basics"]
		if now_score > best_prize["min_score_add"] then
			 best_random = best_random + best_prize["score_add"] * (now_score - best_prize["min_score_add"]);
		end
		if now_score + 1 >= best_prize["max_score_add"] and money_type <= hld.e_treasure_prize_type_ten then
			best_random = 10000;
		end
		if best_random >= random_num then--是否能获得极品
			res_table["prize_index"][1] = self:get_prize_by_list(best_prize["percent"]);
			res_table["prize_luck"] = -1;
		else
			res_table["prize_index"][1] = self:get_prize_by_list(nomal_prize);
		end
	end
end

function royal_treasure:shuffle(t)
    if type(t)~="table" then
        return
    end
    local tab={}
    local index=1
	math.randomseed(os.time())
    while #t~=0 do
        local n=math.random(#t)
        if t[n]~=nil then
            tab[index]=t[n]
            table.remove(t,n)
            index=index+1
        end
    end
    return tab
end

function royal_treasure:get_ten_prize(treasure_percent, now_score, is_first, res_table, is_free,money_type)
	res_table["prize_luck"] = treasure_percent["add_luck"];
	local prize_list = {}
	if is_first == 0 then --第一次
		local first_prize = treasure_percent["first"];
		local better_num = 0;
		for i = 1, 10 do
			local random_num = math.random(1,10000);
			if random_num <= first_prize["better_percent"] and better_num < first_prize["better_max"] then
				prize_list[i] = self:get_prize_by_list(first_prize["better_prize"]);
				better_num = better_num + 1;
			elseif 10 - i + 1 + better_num <=  first_prize["better_min"] then
				prize_list[i] = self:get_prize_by_list(first_prize["better_prize"]);
				better_num = better_num + 1;
			else
				prize_list[i] = self:get_prize_by_list(first_prize["nomal_prize"]);
			end
		end
		res_table["prize_index"] = self:shuffle(prize_list)
	elseif is_free == 1 then --免费
		local free_prize = treasure_percent["free"];
		local better_num = 0;
		for i = 1, 10 do
			local random_num = math.random(1,10000);
			if random_num <= free_prize["better_percent"] and better_num < free_prize["better_max"] then
				prize_list[i] = self:get_prize_by_list(free_prize["better_prize"]);
				better_num = better_num + 1;
			elseif 10 - i + 1 + better_num <=  free_prize["better_min"] then
				prize_list[i] = self:get_prize_by_list(free_prize["better_prize"]);
				better_num = better_num + 1;
			else
				prize_list[i] = self:get_prize_by_list(free_prize["nomal_prize"]);
			end
		end
		res_table["prize_index"] = self:shuffle(prize_list)
	else
		local next_prize = treasure_percent["next"];
		local nomal_prize = next_prize["nomal_prize"];
		local better_prize = next_prize["better_prize"];
		local best_prize = next_prize["best_prize"];
		local random_num = math.random(1,10000);
		local best_random = best_prize["basics"];
		if now_score > best_prize["min_score_add"] then
			 best_random = best_random + best_prize["score_add"] * (now_score - best_prize["min_score_add"]);
		end
		if now_score + 10 >= best_prize["max_score_add"] and money_type <= hld.e_treasure_prize_type_ten then
			 best_random = 10000;
		end
		local better_num = 0;
		local best_num =0;

		for i = 1, 10 do
			local random_num = math.random(1,10000);

			if random_num <= best_random and best_num == 0 then--是否能获得极品
				prize_list[i] = self:get_prize_by_list(best_prize["percent"]);
				res_table["prize_luck"] = -1;
				best_num = best_num + 1;
				better_num = better_num + 1;
				best_random = 0;
			elseif random_num <= next_prize["better_percent"] and better_num < next_prize["better_max"]  then
				prize_list[i] = self:get_prize_by_list(better_prize);
				better_num = better_num + 1;
			elseif 10 - i + 1 + better_num <=  next_prize["better_min"] then
				prize_list[i] = self:get_prize_by_list(better_prize);
				better_num = better_num + 1;
			else
				prize_list[i] = self:get_prize_by_list(nomal_prize);
			end
		end
		res_table["prize_index"] = self:shuffle(prize_list)
	end
end
--get_item_list(royal_treasure_data, royal_treasure_percent, 0, 1, nil, 1, 10);
return royal_treasure
