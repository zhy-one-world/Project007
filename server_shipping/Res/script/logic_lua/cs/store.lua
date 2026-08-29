local store = {}


local enum_request_update_type = 
{
	normal = 0, --正常刷新
	get_server_open_time = 1,--刷新限购类
}

local goods_max_num = 40

local enum_goods_info = 
{
	id = 0, --模板id
	owner_store = 1,--隶属商店id
	purchased_num = 2,---已购买的数量
	last_update_time = 3,--最近一次更新时间
	max = 4,
};

function store:init( )
	
end

define_C[11206] = function ( message_data, player_ref)
	local msg_data = decode("hld.goods_proto.request_update_store", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	store:handle_request_update(player_ref, msg_data.request_type, msg_data.update_time)
end


function store:handle_request_update(player_ref, req_type, time)
	 delog("------store:handle_request_update---", req_type)
	 if req_type == enum_request_update_type.normal then
	 	self:send_store_goods_record_all(player_ref)
	 elseif req_type == enum_request_update_type.get_server_open_time then
        self:send_server_open_time(player_ref)
	 end
end

function store:send_store_goods_record_all_cpp(player_index)
	local player_ref = hld.get_player_by_index(player_index)
	if nil == player_ref then
		return;
	end
	store:send_store_goods_record_all(player_ref)
end

function store:send_store_goods_record_all(player_ref)
    local  player_store_helper = player_ref:get_goods_helper_mgr()
    if player_store_helper == nil then
       return;
    end
    local msg_data = {
         store_list = {}
    }

    for i=0, hld.e_store_type_max - 1 do
    	local store_goods_ref = player_store_helper:get_s_store_goods_by_index(i)
    	if store_goods_ref ~= nil and store_goods_ref:is_valid() then
          local t_store =  self:get_goods_msg(store_goods_ref)
          table.insert(msg_data.store_list, t_store)
    	end
    end

	MessageMgr:send_message_to_self(player_ref, "hld.goods_proto.update_store_goods",msg_data, 11205)
    
end

function store:get_goods_msg(store_goods_ref)
	 local t_store_list = {}
	 t_store_list.store_id = store_goods_ref:get_store_id()
	 t_store_list.goods_list = {}
	 for i = 0,  goods_max_num -1 do
	 	local goods_ref = store_goods_ref:get_goods_by_idex(i)
	 	local t_single = { goods_info = {}}
	 	if goods_ref ~= nil and goods_ref:is_data_use() then
           local t_single = { goods_info = {}}  
           local id = math.floor(goods_ref:get_goods_data_by_index(enum_goods_info.id))
           local store_id =  math.floor(store_goods_ref:get_store_id())
           local purchased_num= math.floor(goods_ref:get_goods_data_by_index(enum_goods_info.purchased_num))
           table.insert(t_single.goods_info, id)


           table.insert(t_single.goods_info, store_id)
           table.insert(t_single.goods_info, purchased_num)

           --table.insert(t_single.goods_info, goods_ref:get_goods_data_by_index(enum_goods_info.id))
           --table.insert(t_single.goods_info, store_goods_ref:get_store_id())
           --table.insert(t_single.goods_info, goods_ref:get_goods_data_by_index(enum_goods_info.purchased_num))
           table.insert(t_single.goods_info, 0)
           table.insert(t_store_list.goods_list, t_single)
	 	end	
	 end
	 return t_store_list
end

function store:send_server_open_time_cpp(player_index)
	local player_ref = hld.get_player_by_index(player_index)
	if nil == player_ref then
		return;
	end
	store:send_server_open_time(player_ref)
end


function store:send_server_open_time(player_ref)
    local open_time = hld.store_mgr:get_instance():get_server_open_time()
	local msg_data = {
        server_open_time = open_time
	}
	MessageMgr:send_message_to_self(player_ref, "hld.goods_proto.goods_get_server_opent_time",msg_data, 11207)
end



function store:is_use_lua()
	return true
end

function store:buy_goods(unit_array_index, goods_id, goods_num, store_id,is_auto_buy)
	 return store:buy_goods_oper(unit_array_index, goods_id, goods_num, store_id,is_auto_buy)
end

function store:buy_goods_oper(unit_array_index, goods_id, goods_num, store_id,is_auto_buy)
	delog("----buy_goods--------" ,unit_array_index,goods_id, goods_num, store_id)
    local player_ref = hld.get_player_by_index(unit_array_index)
	if player_ref == nil then
		return ConstDefine.e_item_string_unkown;
    end
    local player_store_helper = player_ref:get_goods_helper_mgr()
    if player_store_helper == nil then
       return ConstDefine.e_item_string_unkown;
    end
    if goods_num <= 0 then
        return ConstDefine.e_buy_goods_end_goods_num_is_zero
    end

    local t_temp_goods_ptr = template_manager:get_line("GoodsTemplate", goods_id);
    if t_temp_goods_ptr == nil then
       return ConstDefine.e_item_string_unkown;
    end

    local  t_temp_item_ptrs = template_manager:get_line("ItemTemplate", t_temp_goods_ptr.ItemId);
	if t_temp_item_ptrs == nil then
	   return ConstDefine.e_item_string_unkown;
	end

	local  t_temp_store_ptr,i_store_attr_id = self:get_store_config_by_id(store_id);
	if t_temp_store_ptr == nil  then
	   return ConstDefine.e_item_string_unkown;
    end

    if not self:is_in_store(t_temp_store_ptr, goods_id) then
    	return ConstDefine.e_item_string_unkown
    end

    local b_is_full_exc_att = self:is_full_exc_att(t_temp_store_ptr)

    if not player_store_helper:check_is_goods_open_time(goods_id) then
    	return  ConstDefine.e_buy_goods_end_in_time_limit
    end

    local player_level = player_ref:get_unit_info(hld.e_role_info_exp_level);

    local i_ret_level = self:is_level(t_temp_goods_ptr, player_level)
    if i_ret_level ~= ConstDefine.e_item_string_succeed then
    	return i_ret_level
    end

    if player_ref:get_vip_level() < t_temp_goods_ptr.NeedVIPLevel then
       return ConstDefine.e_item_string_vip_level;
    end

    if player_store_helper:can_buy(goods_id, store_id) == false then
    	return ConstDefine.e_item_string_unkown
    end

    local i_can_buy_num =  player_store_helper:can_buy_num(goods_id, store_id);
	if (i_can_buy_num == 0 or (i_can_buy_num ~= -1 and (i_can_buy_num - goods_num) < 0)) then
		return ConstDefine.e_buy_goods_end_state_num_limit;
	end

    local  need_slot_num = self:get_need_slot_num(t_temp_item_ptrs, goods_num, t_temp_goods_ptr)


    local  need_bag = hld.init_unit:get_item_bag_type(t_temp_goods_ptr.ItemId);

	if (player_ref:get_item_set():is_empty_slot_enough(need_bag, need_slot_num) == false) then		
		return ConstDefine.e_buy_goods_end_state_bag;
	end

	local t_NeedItemId = Common:StrToTable(t_temp_goods_ptr.NeedItemId)

	if #t_NeedItemId >= 2 then
		if (player_ref:get_item_set():count_item_by_template_id(t_NeedItemId[1], hld.e_bag_type_bag) < (t_NeedItemId[2] * goods_num)) then		
			return ConstDefine.e_buy_goods_end_need_item_not_enough;
		end
	end

	if (i_can_buy_num == -1 or (i_can_buy_num - goods_num) >= 0) then
        local total_price = self:get_cost_of_goods(goods_id, goods_num)
        if #total_price ~= 2 then
         	return ConstDefine.e_item_string_unkown
        end

        if player_ref:can_cut_money(total_price[1], total_price[2]) == false then
         	return ConstDefine.e_buy_goods_end_state_money
        end

        local temp_item = player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_buy_goods, goods_id, t_temp_goods_ptr.ItemId, goods_num * t_temp_goods_ptr.GoodsNum, t_temp_goods_ptr.IsLock,  -t_temp_goods_ptr[" EffectiveTime"],0,0);
		if (temp_item == nil) then
			return ConstDefine.e_item_string_unkown;
		end


		if (t_temp_item_ptrs.item_type == hld.e_item_type_spirit) then
			 temp_item:set_data_info(hld.e_item_info_upgrade_count, 1)
		end

		if (#t_NeedItemId >= 2) then
				--消耗物品
			player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_shop_buy ,0, t_NeedItemId[1], t_NeedItemId[2] * goods_num, 1);
		end

		player_ref:get_item_set():put_item_into_bag(temp_item);


		player_ref:cut_money(total_price[1], total_price[2], hld.e_server_log_cut_money_buy_goods, goods_id, goods_num)

		if (total_price[1] == hld.e_money_type_jewel) then		
			player_ref:get_welfare_mgr():set_active_degree_info(hld.e_daily_active_degree_type_cost_one_diamond, 1);
		end

		player_store_helper:add_buy_info(goods_id, goods_num, store_id);

		self:send_store_goods_record_all(player_ref);

		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_purchase_goods, goods_id, goods_num);
		if t_temp_item_ptrs.item_type == hld.e_item_type_armor and t_temp_item_ptrs.sub_type == hld.e_armor_sub_type_amulet then
			player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_intensify_add_gs, hld.e_power_up_prompt_exchange_amulet, 0);
		end


		if (t_temp_store_ptr.StoreType == hld.e_store_type_recommend or 
			t_temp_store_ptr.StoreType == hld.e_store_type_daily or 
			t_temp_store_ptr.StoreType == hld.e_store_type_expendables) then
				--记录钻石商店购买事件
			player_ref:get_time_limit_activity_mgr():activity_behavior_done(hld.e_time_limit_behavior_type_buy_in_diamond_store,1);
		end

		if (player_ref:get_write_log()) then
			hld.server_log:buy_goods_role_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), i_store_attr_id, goods_id, goods_num, player_ref:get_login_type(),player_ref:get_time_data(hld.e_time_type_login_time));
			hld.server_log:serverBuyGoods(player_ref:get_share_log_head(), i_store_attr_id, self:get_store_str_by_store(i_store_attr_id), goods_num, goods_id, self:get_item_str_by_item(goods_id), 1, total_price[1], self:get_money_str_by_Money(total_price[1]), total_price[2]);
			hld.server_log:role_mall_log(player_ref:get_third_info(), player_ref:get_unit_info_inst(), total_price[1], total_price[2], i_store_attr_id, goods_num, goods_id, player_ref:get_login_type());
		end
		return 0
	end
	return ConstDefine.e_item_string_unkown
end

function store:get_store_config_by_id(store_id)
	local t_store_table = template_manager:get_template("StoreTemplate")
	if t_store_table == nil then
	   return nil, 0
	end
	for k, v  in pairs(t_store_table) do
		if store_id == v.StoreType then
		   return v, k
		end
	end
	return nil , 0
end

function store:is_in_store(t_temp_store_ptr, goods_id)
	 local t_StoreGoods = Common:StrToTable(t_temp_store_ptr.StoreGoods)
	 for i=1, #t_StoreGoods do
	 	 if t_StoreGoods[i] == goods_id then
	 	 	return true
	 	 end
	 end
	 delog(t_StoreGoods)
	 --delog("----not--find----", goods_id)
	 return false
end

local t_store_type_map = 
{
	[hld.e_store_type_hope_point_first] = 1,
	[hld.e_store_type_hope_point_second] = 1,
	[hld.e_store_type_hope_point_third] = 1,
	[hld.e_store_type_magic_crystal_third] = 1,

}

function store:is_full_exc_att(t_temp_store_ptr)
	 local store_type  = t_temp_store_ptr.StoreType
	 if t_store_type_map[store_type] == nil then
	 	return false
	 end
	 return true
  
end


function store:is_level(t_temp_goods_ptr, player_level, t_temp_item_ptrs)
	  local t_Levellimit = Common:StrToTable(t_temp_goods_ptr.Levellimit)
	  if #t_Levellimit < 2 then
	  	 return ConstDefine.e_item_string_level
	  end
	  if player_level < t_Levellimit[1] or player_level > t_Levellimit[2] then
	  	 if self:is_special_level(t_temp_item_ptrs) then
	  	 	return  ConstDefine.e_item_string_not_belong_level
	  	 end
	  	 return ConstDefine.e_item_string_level
	  end

	  return ConstDefine.e_item_string_succeed
end

local t_item_sub_map  = 
{
	[hld.e_prop_sub_type_hp_min] = 1,
	[hld.e_prop_sub_type_hp_mld] = 1,
	[hld.e_prop_sub_type_hp_max] = 1,
	[hld.e_prop_sub_type_hp_shop] = 1,
	[hld.e_prop_sub_type_hp_essence] = 1,

}

function store:is_special_level(t_temp_item_ptrs)
     if t_temp_item_ptrs.item_type ~= hld.e_item_type_expendable then
        return false
     end
     local sub_type = t_temp_item_ptrs.sub_type;
     if t_item_sub_map[sub_type] == nil then
     	return false
     end
     return true
end

function store:get_need_slot_num(t_temp_item_ptrs, goods_num, t_temp_goods_ptr)
    local  need_slot_num = 0;
	local  total_item_num = goods_num * t_temp_goods_ptr.GoodsNum;
	need_slot_num = total_item_num;
	if t_temp_item_ptrs.max_pile_num ~= 0 then
	   need_slot_num  = total_item_num // t_temp_item_ptrs.max_pile_num 
	   if total_item_num % t_temp_item_ptrs.max_pile_num ~= 0 then
	   	  need_slot_num = need_slot_num + 1
	   end
	end
	return need_slot_num
end

function store:get_cost_of_goods(goods_templateid, goods_num)
    local region_template_ptr=server_tool_mgr:get_region_template_ptr()

	if (region_template_ptr == nil) then
		return {};
	end
	local money_type_index = region_template_ptr.RegionCode * 2;
	local money_num_index = money_type_index + 1;
	
	local goods_ptr = template_manager:get_line("GoodsTemplate", goods_templateid);
	if (goods_ptr == nil ) then
		return {}
	end

	local t_NewPrice = Common:StrToTable(goods_ptr.NewPrice)
    if #t_NewPrice <= money_num_index then
       return {}
    end

    local money_id =  t_NewPrice[money_type_index + 1]
    local money_num = t_NewPrice[money_num_index + 1]
    
    return {money_id, money_num * goods_num}
end

function store:get_store_str_by_store(store_id)
	local store_name = ""
	local store_ptr = template_manager:get_line("StoreTemplate", tonumber(store_id))
	local store_name_array = template_manager:get_field("GameConfigTemplate", m_game_config_id, "StoreNameIdArray");
	if next(store_ptr) ~= nil and next(store_name_array) ~= nil then
		local store_index = store_ptr["StoreType"] + 1;
		if store_name_array[store_index] ~= nil then
			store_name = hld.get_str_by_string_template_id(tonumber(store_name_array[store_index]), 0)
		end
	end
	return tostring(store_name)
end

function store:get_item_str_by_item(item_id)
	local item_name = ""
	local Goods_ptr = template_manager:get_line("GoodsTemplate", item_id);
	if next(Goods_ptr) ~= nil then
		local item_ptr = template_manager:get_line("ItemTemplate", Goods_ptr["ItemId"]);
		if next(item_ptr) ~= nil then
			if item_ptr["item_name"] ~= nil then
				item_name = hld.get_str_by_string_template_id(tonumber(item_ptr["item_name"]), 0)
				item_name = string.gsub(item_name,"<.->", "")
			end
		end
	end
	return item_name
end

function store:get_money_str_by_Money(money_type)
	local money_name = ""
	local money_ptr = template_manager:get_template("MoneyTemplate")
	if money_ptr == nil then
		return money_name
	end
	for k, v in pairs(money_ptr) do
		if v["MoneyType"] ~= nil then
			if v["MoneyType"] == money_type then
				money_name = hld.get_str_by_string_template_id(tonumber(v["NameId"]), 0)
				break;
			end
		end
	end
	return tostring(money_name)
end

store_mgr = store


return store