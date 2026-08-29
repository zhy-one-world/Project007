local auction = {}


local e_auction_operate_type = 
{
    non = 0,
    sell = 1, --卖
    purchase = 2, --买
    cancel_sell = 3, --取消出售
    bid = 4,  --竞标
    read_record = 5, --读纪录
    max = 6,
};

local max_auction_sell_num = 16;



function auction:init(  )


	self.m_oper_type = {}

	self.m_oper_type[e_auction_operate_type.sell] = self.sell_item;

	self.m_oper_type[e_auction_operate_type.purchase] = self.buy_item;

	self.m_oper_type[e_auction_operate_type.cancel_sell] = self.cancel_sell_item;

	self.m_oper_type[e_auction_operate_type.bid] = self.bid_item

	self.m_oper_type[e_auction_operate_type.read_record] = self.read_record

end



define_C[13701] = function (message_data, player_ref)
	local msg_data = decode("hld.auction_proto.operate", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	if player_ref:is_self_server() == false then
		return;
	end
	---player_ref.get_auction_mgr().operate(request.operate_type(), request.item_guid(), request.item_num(), request.money_type(), request.money_num());

	auction:operate(player_ref, msg_data)
end

define_C[13706] = function(message_data, player_ref)
	local msg_data = decode("hld.auction_proto.req_auction_list", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	auction:req_goods_list(player_ref, msg_data)
end


define_C[13708] = function(message_data, player_ref)
	local msg_data = decode("hld.auction_proto.req_trade_record", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	auction:req_trade_record(player_ref, msg_data);
end



define_C[13710] = function(message_data, player_ref)
	local msg_data = decode("hld.auction_proto.req_another_sell_info", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	auction:req_another_sell_info(player_ref, msg_data)
end


define_C[13712] = function(message_data, player_ref)
	local msg_data = decode("hld.auction_proto.req_self_bid_list", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	if player_ref:is_self_server() == false then
		return;
	end

	auction:req_self_bid_info(player_ref, msg_data)
end



function auction:operate_cpp(player_index, n_operate_type, item_guid_a, item_guid_b, n_sell_num,  n_price_type, n_price_value)
	local n_item_guid = item_guid_b << 32 | item_guid_a;
	local t_msg = 
	  {
	   operate_type = n_operate_type,
	   item_guid = n_item_guid,
	   item_num = n_sell_num,
	   money_type = n_price_type,
	   money_num = n_price_value
	}

	local player_ref = hld.get_player_by_index(player_index)
	if nil == player_ref then
		return ConstDefine.e_item_string_unkown;
	end

	auction:operate(player_ref, t_msg)

	return 0;

end

function auction:operate(player_ref, msg_data)
	 local oper_type  =  msg_data.operate_type
	 local item_guid =  msg_data.item_guid
	 local sell_num = msg_data.item_num
	 local price_type = msg_data.money_type
	 local price_value = msg_data.money_num
	 --delog(msg_data)
     self.m_oper_type[oper_type](self, player_ref, msg_data)
     return 0;
end



function auction:sell_item(player_ref, msg_data)

	 --delog(msg_data);
	
	 local item_guid =  msg_data.item_guid

	 local sell_num = msg_data.item_num
	 local price_type = msg_data.money_type
	 local price_value = msg_data.money_num

	 ------------------------------------------------
	 local game_config_ptr = server_tool_mgr:get_game_config_template_ptr()
	 if game_config_ptr == nil then
	 	return 
	 end
	  delog("--------AB-", item_guid)

	 local A, B = Common:Int64to32(item_guid)



	local item_ptr = player_ref:get_item_set():get_item(A, B)
	if item_ptr == nil  or item_ptr:get_data_info(hld.e_item_info_locked) > 0 then

	 	return 
	 end
	 local t_line = server_tool_mgr:get_item_template(item_ptr)
	 if t_line == nil then
	 	return 
	 end
	 if  #t_line.TradingBaseMoneyArray <= 0 then
	 	return
	 end


	 local is_price_money_type_can_use = false;

	 for i=1, #t_line.TradingBaseMoneyArray, 2 do
	 	if t_line.TradingBaseMoneyArray[i] == price_type then
           is_price_money_type_can_use = true
	 	end
	 end

	 if not is_price_money_type_can_use then
	 	return 
	 end

	 if sell_num <= 0 then
	 	sell_num = item_ptr.get_data_info(hld.e_item_info_stack_count)
	 end

	local  tolal_sell_price_min = 0;
	if price_type < #t_line.AuctionMinMoneyArray then
		 tolal_sell_price_min = t_line.AuctionMinMoneyArray[price_type + 1] * sell_num;
     end
	if tolal_sell_price_min < game_config_ptr.AuctionSellTotalPriceMin  then

		tolal_sell_price_min = game_config_ptr.AuctionSellTotalPriceMin;
	end	

	if (price_value < tolal_sell_price_min) then
		return;
	end

	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end

    local  empty_selling_index = auction_mgr:get_empty_sell_slot();
	if empty_selling_index < 0 or empty_selling_index >= max_auction_sell_num then
		return;
	end


	local result = auction_mgr:lua_make_auction_to_ws(item_ptr, sell_num, price_type, price_value)

	if result == true then
		player_ref:get_item_set():del_item(hld.e_server_log_del_item_auction, 0, item_ptr, sell_num)
	end
end


function auction:buy_item(player_ref, msg_data)
	local item_guid =  msg_data.item_guid

	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end

    ---local A, B = Common:Int64to32(item_guid)

    --local guid_ptr =  hld.get_guid_64(A, B)

    local guid_ptr = hld.get_guid_64(item_guid)

    auction_mgr:buy_item(guid_ptr)
end

function auction:cancel_sell_item(player_ref, msg_data)
	local item_guid =  msg_data.item_guid
	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end

    --local A, B = Common:Int64to32(item_guid)

    --local guid_ptr =  hld.get_guid_64(A, B)

    local guid_ptr = hld.get_guid_64(item_guid)

    auction_mgr:cancel_sell_item(guid_ptr)

end

function auction:bid_item(player_ref, msg_data)
	 local item_guid =  msg_data.item_guid
	 local price_type = msg_data.money_type
	 local price_value = msg_data.money_num
	 ----------------------------------------------

	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end
    --local A, B = Common:Int64to32(item_guid)

    --local guid_ptr =  hld.get_guid_64(A, B)

    local guid_ptr = hld.get_guid_64(item_guid)


    auction_mgr:bid_item(guid_ptr, price_type, price_value)

end


function auction:read_record(player_ref, msg_data)
	local item_guid =  msg_data.item_guid
	local price_type = msg_data.money_type
	 --------------------------------------------
	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end
    --local A, B = Common:Int64to32(item_guid)


    --local guid_ptr =  hld.get_guid_64(A, B)


    local guid_ptr = hld.get_guid_64(item_guid)


    auction_mgr:read_record(guid_ptr, price_type)
end


function auction:req_goods_list(player_ref, request)
	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end

    auction_mgr:req_goods_list(request.main_type  , request.sub_type , 
			request.item_color, request.quality_level,
			request.single_money_sort_type, request.total_money_sort_type,
			request.is_show_gold, request.is_show_diamond, request.show_page, request.key_word
			,request.trade_type, request.career_type);
end

function auction:req_trade_record(player_ref, msg_data)
	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end

    auction_mgr:req_trade_record(msg_data.trade_type, msg_data.page_index)
end


function auction:req_another_sell_info(player_ref, msg_data)
	local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end
    auction_mgr:req_another_sell_info(msg_data.sell_money_type, msg_data.item_template_id)
end


function auction:req_self_bid_info(player_ref, msg_data)
     local auction_mgr = player_ref:get_auction_mgr();
    if auction_mgr == nil then
        return;
    end
	auction_mgr:req_self_bid_info(msg_data.trade_type, msg_data.page_index);
end

auction_lua_mgr = auction
return auction