local auction_mgr = RequireSingleton("auction_mgr")


local  auction_check_time = 60000 --1min拍卖行检测时间
local  auction_legion_sell_check_interval = 5;			--5sec军团拍卖红点检查间隔

local  auction_share_list_max_num = 130;					--分红列表上限

local   e_auction_operate_type_non = 0 
local   e_auction_operate_type_sell = 1 --卖
local   e_auction_operate_type_purchase = 2 --买
local   e_auction_operate_type_cancel_sell = 3 --取消出售
local   e_auction_operate_type_bid = 4 --竞标
local   e_auction_operate_type_read_record = 5 --读纪录
local   e_auction_operate_type_max = 6

local   second_tick_time = 1000

local	e_auction_param_private_del = 1 --个人拍卖删除时间
local	e_auction_param_private_delay_start = 2 -- 个人随机上架起始时间
local	e_auction_param_private_delay_end = 3 --个人随机上架结束时间
local	e_auction_param_wait_bid = 4 -- 世界，军团拍卖等待时间
local	e_auction_param_wait_notice = 5 -- 品质道具通告时间
local	e_auction_param_add_time = 6    --加时时间
local   e_auction_param_add_n = 7      --加时次数
local   e_auction_param_share_money_limit = 8 --共享拍卖分红上限
local   e_auction_param_person_refresh_time = 9  -- 个人刷新时间
local   e_auction_param_person_refresh_item_num = 10 -- 个人刷新物品数量
local	e_auction_param_max = 10


local   auction_clear_item_num_per_time = 100;				-- 每次清理拍卖行逾期物品个数

--[[
34712 5
34707 5
34716 5
34703 5
34727 5
34752 5

]]




function auction_mgr:init()
	 if is_init() then
	 	return
	 end
	 ---------------------------------
	 self.m_selling_goods_map = {}
	 self.m_direct_purchase_map = {}

	 self.m_old_tick_time = 0;
	 self.m_next_clear_old_legion_item_time = 0;


	 self.m_is_clearing_old_item = false;
	 self.m_is_force_clear_old_item = false;
		
	 self.m_need_check_world_auction = false;
	 self.m_world_sell_num = 0
	 self.m_next_check_world_auction_time = 0;

	 self.m_wait_show_vec = {}
	 --self.m_empty 

	 self.is_request_time_out = false;
	 self.m_time_out_last = 0;
	 self.m_is_first = 1;

	 self.m_time_tip_vec = {}


	 self.m_person_info_queue = {}


end

function auction_mgr:Update(i_nTime)
	--[[if false then
		return
	end]]
	if not is_auciton_use_lua() then
		return
	end

	local new_time = get_g_time_mesc()
	if self.m_is_force_clear_old_item  then
		
		if not self.m_is_clearing_old_item then
			self:clear_old_item();
		end
	else	
		if new_time - self.m_old_tick_time > auction_check_time then
				
			self.m_old_tick_time = new_time;
			self:clear_old_item();		
		end
    end

	if new_time > self.m_next_check_world_auction_time then
		if self.m_need_check_world_auction then
			
			self:check_world_auction_sell();
		end
		self.m_next_check_world_auction_time = get_end_time(auction_legion_sell_check_interval);
	end
	self:wait_tick();
	self:time_out_tick();
	self:time_tip_tick();
	self:person_tick();
	--self:show_info()
end

local show_inteval_time = 10000
function auction_mgr:is_show()
     local cur_msec = get_g_time_mesc()
     if self.m_last_time == nil then
     	self.m_last_time = 0
     end
     if self.m_last_time > cur_msec then
        return false
     end
     self.m_last_time = cur_msec + show_inteval_time
     return true
end


function auction_mgr:show_info()
    if not self:is_show() then
        return
    end 


    delog(self.m_wait_show_vec)
    delog("----self.m_time_tip_vec---", #self.m_time_tip_vec)

    delog("---cur---time--", os.time())

    --show_table(self.m_code_memory[1639])


end



function auction_mgr:get_unit_info()
	 local t_unit = {
	  role_guid = 0,
	  account = "",
	  role_name = "",
	  role_appearance = "",
	  first_kill_npc_arr = "",
	  map_guid = 0,
	  data_i64_ary = {},
	  data_ary = {},
	 }
	 for i=1, hld.e_role_i64_info_max do
	 	t_unit.data_i64_ary[i] = 0
	 end
	 for i=1, hld.e_role_info_max do
	 	t_unit.data_ary[i] = 0
	 end

	 return t_unit
end

function auction_mgr:get_third_info( )
	 local t_third = {}
     for i=1,15 do
     	local key = "param"..i
     	t_third[key] = ""
     end
     return t_third
end


function auction_mgr:add_auction_info_to_db( info,  in_old_item_guid, in_role_info, in_is_add_to_log,  in_third_info)
	  local old_item_guid  = in_old_item_guid or 0
	  local role_info = in_role_info or  self:get_unit_info()
	  local is_add_to_log = in_is_add_to_log or false
	  local third_info = in_third_info or self:get_third_info()

	  local t_dp_msg = 
	  {
	     is_add_to_log = is_add_to_log,
         old_item_guid = old_item_guid,
         info = info,
         role_info = role_info,
         third_info = third_info,
	  }

	  --delog(t_dp_msg)

	  MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_add_item_into_db", t_dp_msg, 34701,0)
end

function auction_mgr:get_share_list( )
	 local t_share = {
	    item_guid = 0,
	    role_guid = 0,
	    share_rate = 0,
	    share_mail_title = "",
	    share_mail_content = "",
	 }
	 return t_share
end


function auction_mgr:add_share_list_to_db(share_list, list_num)
	  if share_list == nil	or  list_num == 0 then
			return;
	  end


	  local t_dp_msg = {}
	  t_dp_msg.item_guid = {}
	  t_dp_msg.role_guid = {}
	  t_dp_msg.share_rate = {}
	  t_dp_msg.share_mail_title = {}
	  t_dp_msg.share_mail_content = {}


	  for i=1,auction_share_list_max_num do
	  	if share_list[i] ~= nil then
	  		table.insert(t_dp_msg.item_guid, share_list[i].item_guid)
	  		table.insert(t_dp_msg.role_guid, share_list[i].role_guid)
	  		table.insert(t_dp_msg.share_rate, share_list[i].share_rate)
	  		table.insert(t_dp_msg.share_mail_title, share_list[i].share_mail_title)
	  		table.insert(t_dp_msg.share_mail_content, share_list[i].share_mail_content)
   	  	else
   	  		table.insert(t_dp_msg.item_guid, 0)
	  		table.insert(t_dp_msg.role_guid, 0)
	  		table.insert(t_dp_msg.share_rate, 0)
	  		table.insert(t_dp_msg.share_mail_title, "")
	  		table.insert(t_dp_msg.share_mail_content, "")
	  	end
	  end

	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_add_share_list", t_dp_msg, 34734,0)
end


function auction_mgr:send_auction_puchase_success_info(info)
    if  info.item_info.item_guid == 0 then
	  return;
    end
    local guid_ptr = hld.get_guid_64(info.seller_guid)
	local  target_session = hld.get_client_session_by_guid(guid_ptr);
	if target_session == nil or
		target_session:get_cs_conn_index() < 0 then
	
		return;
	end
	local t_cs_msg = {}
	t_cs_msg.auction_info = deep_copy(info)


	MessageMgr:send_to_cs(target_session, "hld.ws2cs_proto.auction_send_sell_success_info",t_cs_msg, 34712)
end


function auction_mgr:buy(role_guid, item_guid)
	 if self:is_item_locked(item_guid) then

	 	local t_client_msg = {}
	 	t_client_msg.operate_type = e_auction_operate_type_purchase
	 	t_client_msg.operate_result = ConstDefine.e_auction_purchase_already_purchased
	 	t_client_msg.item_guid = item_guid
	 	t_client_msg.set_item_num = 0
	 	t_client_msg.set_money_type = 0
	 	t_client_msg.set_money_num = 0

	 	local guid_ptr = hld.get_guid_64(role_guid)
		local  target_session = hld.get_client_session_by_guid(guid_ptr);

		if target_session ~= nil then
			
			MessageMgr:send_to_client(target_session, "hld.auction_proto.operate_end", t_client_msg, 13702)
		end
		return;
	 end
	 self:operate_selling_lock(item_guid, true);

	 local t_dp_msg = {}
	 t_dp_msg.role_guid = role_guid
	 t_dp_msg.item_guid = item_guid

	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_find_buy_target", t_dp_msg, 34705,0)
end


function auction_mgr:find_buy_target_end(info, role_guid,item_guid)
    local guid_ptr = hld.get_guid_64(role_guid)
	local session_player = hld.get_client_session_by_guid(guid_ptr);
   
	if info.item_info.item_guid == 0 then
	
		local t_client_msg = {}
 		t_client_msg.operate_type = e_auction_operate_type_purchase
 		t_client_msg.operate_result = ConstDefine.e_auction_purchase_already_purchased
 		t_client_msg.item_guid = 0
 		t_client_msg.set_item_num = 0
 		t_client_msg.set_money_type = 0
 		t_client_msg.set_money_num = 0

		if  session_player ~= nil and session_player:get_cs_conn_index() >= 0 then
			MessageMgr:send_to_client(session_player, "hld.auction_proto.operate_end", t_client_msg, 13702);
		end
	else
	
		if  session_player == nil or
			session_player:get_cs_conn_index() < 0 then
		
			--对方必须在线 否则不知道扣多少钱
			self:add_auction_info_to_db(info);
		else
			delog("--auction_purchase_success----")
			local t_cs_msg = {}
			t_cs_msg.auction_info = deep_copy(info)
			delog("--auction_purchase_success--1111--")
			t_cs_msg.role_guid = role_guid
			MessageMgr:send_to_cs(session_player, "hld.ws2cs_proto.auction_purchase_success", t_cs_msg,34707)
			delog("--auction_purchase_success--222--")
		end
	end
	self:operate_selling_lock(item_guid, false);
end

function auction_mgr:cancel_sell(role_guid, item_guid)
	 if self:is_item_locked(item_guid) then

	 	    local t_client_msg = {}

			t_client_msg.item_guid = item_guid;
			t_client_msg.item_num = 0;
			t_client_msg.operate_type = e_auction_operate_type_cancel_sell;
			t_client_msg.money_type = 0;
			t_client_msg.money_num = 0;
			t_client_msg.operate_result = ConstDefine.e_auction_cancel_sell_already_sell;

			local guid_ptr = hld.get_guid_64(role_guid)
	        local target_session = hld.get_client_session_by_guid(guid_ptr);

			if target_session ~= nil then
			
				MessageMgr:send_to_client(target_session, "hld.auction_proto.operate_end", t_client_msg, 13702);
			end
			return;
	  end

	  self:operate_selling_lock(item_guid, true);

	  local t_dp_msg = {}

	  t_dp_msg.role_guid = role_guid
	  t_dp_msg.item_guid = item_guid

	  MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_cancel_sell", t_dp_msg, 34714,0)
end

function auction_mgr:cancel_sell_end( info, role_guid)
	    --guid一定有，用ID判定是否找到
	if info.item_info.data_ary[hld.e_item_info_info_id + 1] == 0 then
	
		local t_client_msg = {}
		t_client_msg.operate_type = e_auction_operate_type_cancel_sell;
		t_client_msg.operate_result = ConstDefine.e_auction_cancel_sell_already_sell;

		--下面这几个都没什么用
		t_client_msg.item_guid = 0;
		t_client_msg.item_num = 0;
	    t_client_msg.money_type = 0;
		t_client_msg.money_num = 0;

		local guid_ptr = hld.get_guid_64(role_guid)
        local session_player = hld.get_client_session_by_guid(guid_ptr);


		if session_player ~= nil and  session_player:get_cs_conn_index() >= 0 then
		
			MessageMgr:send_to_client(session_player, "hld.auction_proto.operate_end", t_client_msg, 13702);
		end
	
	else
	    local guid_ptr = hld.get_guid_64(role_guid)
		local session_player = hld.get_client_session_by_guid(guid_ptr);
		if session_player == nil or  session_player:get_cs_conn_index() < 0 then
		
			local sender_name = hld.globle_data:get_instance():get_mail_common_text(hld.e_mail_common_text_sender_system);
			local mail_title = hld.globle_data:get_instance():get_mail_common_text(hld.e_mail_common_text_auction_cancel_sell_title);
			local mail_content = hld.globle_data:get_instance():get_mail_common_text(hld.e_mail_common_text_auction_cancel_sell_content);

			local temp_item_info_array = hld.event_ws_mgr:get_instance().get_free_list()
			local s_item_buffer = encode("hld.st_proto.st_item_info", info.item_info)
			local item_info_ptr = hld.get_item_info_by_str(s_item_buffer, string.len(s_item_buffer))
			temp_item_info_array.add_free_item(item_info_ptr);
			hld.event_ws_mgr:get_instance():send_mail_to_player_by_system(guid_ptr, 0, sender_name, mail_title, mail_content,
				0,0,0,0, temp_item_info_array);
		else
		
			self:cancel_del_show(info);

			local t_cs_msg = {}

			t_cs_msg.auction_info = deep_copy(info);
			t_cs_msg.role_guid = role_guid;
			MessageMgr:send_to_cs(session_player, "hld.ws2cs_proto.auction_cancel_sell", t_cs_msg,34716, 0)		
		end
	end
	self:operate_selling_lock(info.item_info.item_guid, false);
end

function auction_mgr:operate_selling_lock(item_guid, is_add)
	if item_guid == 0 then
		return;
	end	
	if is_add == true then
		
		if  not self:is_item_locked(item_guid) then		
			self.m_selling_goods_map[item_guid] = true
		end
	
	else
		if self:is_item_locked(item_guid) then	
			self.m_selling_goods_map[item_guid] = nil
		end
	end
end

function auction_mgr:is_item_locked(item_guid)
	if self.m_selling_goods_map[item_guid] == nil then
		return false
	end
	return true
end

function auction_mgr:sell_end(info, sell_result, role_info, is_add_to_log,  third_info)
	local temp_login_type = hld.e_login_type_new_account_and_password;
	if info.trade_type == hld.e_trading_type_normal_sell or info.trade_type == hld.e_trading_type_world then
		local seller_guid_ptr = hld.get_guid_64(info.seller_guid)
		local session_player = hld.get_client_session_by_guid(seller_guid_ptr);
		if session_player ~= nil and session_player:get_cs_conn_index() >= 0 then
			
			local t_cs_msg = {}
			t_cs_msg.info = deep_copy(info)
			t_cs_msg.operate_result = sell_result


			MessageMgr:send_to_cs(session_player, "hld.ws2cs_proto.auction_sell_end", t_cs_msg, 34703)
	
			temp_login_type = session_player:get_login_type()
		end
		if sell_result == ConstDefine.e_auction_sell_success and info.show_guid == 0 then
			
				local auction_time_info = {}
				auction_time_info.show_time = 0
				auction_time_info.is_update = 0
				auction_time_info.template_id = 0
				auction_time_info.auction_info = deep_copy(info)
				if session_player ~=  nil then
				
					auction_time_info.template_id = session_player:get_role_info_data(hld.e_role_info_template_id);
				end
				delog("----add_wait_show---")
				--self:add_wait_show(auction_time_info);	
		end
	elseif info.trade_type == hld.e_trading_type_legion then
		delog("--111-sell--end-----")

	    local seller_guid_ptr = hld.get_guid_64(info.seller_guid)
		local  temp_legion = hld.legion_ws_mgr:get_instance():get_legion(seller_guid_ptr);
		if sell_result == ConstDefine.e_auction_sell_success and temp_legion ~= nil then
		
			temp_legion:set_auction_selling_check_start(true);
	    end
	end
    delog("--111-sell--end-----")
	if is_add_to_log == true then
		
		local item_num =  info.item_info.data_ary[hld.e_item_info_stack_count + 1];
		local money_type = info.sell_money_type;
		local money_count = info.sell_total_money;
		local  item_id = info.item_info.data_ary[hld.e_item_info_info_id + 1];
		hld.server_log:auction_sell_log(item_id, item_num, money_type, money_count);
		local s_info_buffer = encode("hld.st_proto.st_auction_info", info)
		local info_ptr = hld.get_auction_info_by_str(s_info_buffer, string.len(s_info_buffer))
		local s_role_info_buffer = encode("hld.st_proto.st_unit_info", role_info)
		local role_info_ptr = hld.get_unit_info_by_str(s_role_info_buffer, string.len(s_role_info_buffer))
		local s_third_buffer = encode("hld.st_proto.st_login_fixed_data",  third_info)
		local third_info_ptr = hld.get_login_fixed_data_by_str(s_third_buffer, string.len(s_third_buffer))
		hld.server_log:auction_db_log(info_ptr, hld.e_auction_state_start_sell, role_info_ptr, third_info_ptr, temp_login_type);

		local log_head = hld.get_log_common_head()
		if info.trade_type ~= hld.e_trading_type_normal_sell or info.show_guid ~= 0 then
			
			hld.lua_set_log_common_head_part3(log_head, "null", hld.get_world_server():get_server_id(), get_g_time_mesc())
			hld.server_log:serverAuction(log_head, "null", "null", "null", 0,
			tostring(info.item_info.item_guid), info.item_info.data_ary[hld.e_item_info_info_id + 1], info.item_info.data_ary[hld.e_item_info_stack_count + 1],
			info.trade_type, "null", "null", "null", 0, info.sell_money_type, hld.globle_data:get_instance():get_money_str_by_type(info.sell_money_type), info.sell_total_money, hld.e_auction_state_start_sell);
	
		else
			
			hld.lua_set_log_common_head_part1(log_head, hld.get_app_key(), third_info.param7, third_info.param4 , third_info.param5);
			hld.lua_set_log_common_head_part2(log_head, third_info.param2, tonumber(third_info.param15) or 0);
			hld.lua_set_log_common_head_part3(log_head, role_info.account, role_info.data_ary[hld.e_role_info_server_id + 1], get_g_time_mesc())
			hld.lua_set_log_common_head_part4(log_head, role_info.role_guid, role_info.role_name, role_info.data_ary[hld.e_role_info_exp_level+1], role_info.data_ary[hld.e_role_info_gender+1])
			hld.lua_set_log_common_head_part5(log_head, role_info.data_i64_ary[hld.e_role_i64_info_gs_value +1],role_info.data_ary[hld.e_role_info_class_type + 1], role_info.data_ary[hld.e_role_info_vip_level+1])

			hld.server_log:serverAuction(log_head, role_info.account, tostring(role_info.role_guid), role_info.role_name, 0,
			tostring(info.item_info.item_guid), info.item_info.data_ary[hld.e_item_info_info_id+1], info.item_info.data_ary[hld.e_item_info_stack_count + 1],
			info.trade_type, "null", "null", "null", 0, info.sell_money_type, hld.globle_data:get_instance():get_money_str_by_type(info.sell_money_type), info.sell_total_money, hld.e_auction_state_start_sell);
		end
	end
end

function auction_mgr:sell_rare_item_proc(info)
	local seller_guid_ptr = hld.get_guid_64(info.seller_guid)
	local session_player = hld.get_client_session_by_guid(seller_guid_ptr);
	if session_player == nil or session_player:get_cs_conn_index() < 0 then
		return;
	end
	local item_template_ptr = template_manager:get_line("ItemTemplate", info.item_info.data_ary[hld.e_item_info_info_id + 1]);
	if item_template_ptr == nil then
		
		return;
	end

	local  notice_param_array = hld.event_ws_mgr:get_instance():get_free_param();
		
	for i=1,hld.e_item_info_max do
		hld.event_ws_mgr:get_instance():add_free_param(info.item_info.data_ary[i])
	end

	--跑马灯的
	local vec_notice = {};
	local t_notice_id = template_manager:get_str_id_by_notice_id(93000232)
	vec_notice[1] = tostring(t_notice_id[1])
	
	vec_notice[2] = info.seller_name;

	--结构体中的name没有颜色代码
	local temp_string = hld.get_str_by_string_template_id(item_template_ptr.item_name, 0);
	vec_notice[3] = temp_string

	local  notice_str = table.concat( vec_notice, "&#")
	hld.event_ws_mgr:get_instance():send_notice_to_all(93000232, os.time(), seller_guid_ptr, notice_str, session_player:get_role_info_data(hld.e_role_info_template_id));

	--聊天频道的
	hld.event_ws_mgr:get_instance():send_notice_with_param(
			seller_guid_ptr, info.seller_name, 
			"90089566", 
			notice_param_array, hld.e_notice_param_item_info, 
			hld.e_chat_type_world, session_player:get_role_info_data(hld.e_role_info_template_id), 93000233);
end


function auction_mgr:trade_bid(role_guid,  item_guid, price_type,  price_value)
	if self:is_item_locked(item_guid) then
		
		local t_client_msg = {}
	    t_client_msg.operate_type = e_auction_operate_type_bid
		t_client_msg.operate_result = ConstDefine.e_auction_bid_money_changed;

			--下面这几个都没什么用
		t_client_msg.item_guid = item_guid;
		t_client_msg.item_num = 0;
		t_client_msg.money_type = 0;
		t_client_msg.money_num = 0;

		local guid_ptr  = hld.get_guid_64(role_guid)

		local target_session = hld.get_client_session_by_guid(guid_ptr);
		if target_session ~= nil then
			MessageMgr:send_to_client(target_session, "hld.auction_proto.operate_end", t_client_msg, 13702)
		end
		return;
	end
	self:operate_selling_lock(item_guid, true);

	local t_dp_msg = {}

	t_dp_msg.role_guid  = role_guid;
	t_dp_msg.item_guid = item_guid
	t_dp_msg.price_money_type = price_type;
	t_dp_msg.price_money_value = price_value;
	delog(t_dp_msg)
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_find_bid_target", t_dp_msg, 34725,0)
end


function auction_mgr:find_bid_end_proc(role_guid, item_guid, info, price_type, price_value)
    delog("-----auction_mgr:find_bid_end_proc----")
    local guid_ptr = hld.get_guid_64(role_guid)
	local session_player = hld.get_client_session_by_guid(guid_ptr);

	if info.item_info.item_guid == 0 then

		local t_client_msg = {}
		t_client_msg.operate_type = e_auction_operate_type_bid;
		t_client_msg.operate_result = ConstDefine.e_auction_purchase_already_purchased;

		--下面这几个都没什么用
		t_client_msg.item_guid = 0;
		t_client_msg.item_num = 0;
		t_client_msg.money_type = 0;
		t_client_msg.money_num = 0;

		if session_player ~= nil and session_player:get_cs_conn_index() >= 0 then
			MessageMgr:send_to_client(session_player, "hld.auction_proto.operate_end", t_client_msg, 13702)
		end
		self:operate_selling_lock(item_guid, false);
	
	else
		delog("----price_type--price--value", price_type, price_value)
		delog(info)
		if info.sell_money_type ~= price_type or info.cur_bid_money ~= price_value then
			
			local t_client_msg = {}
			t_client_msg.operate_type = e_auction_operate_type_bid;
			t_client_msg.operate_result = ConstDefine.e_auction_bid_money_changed;

			--下面这几个都没什么用
			t_client_msg.item_guid = 0;
			t_client_msg.item_num = 0;
			t_client_msg.money_type = 0;
			t_client_msg.money_num = 0;

			if session_player ~= nil and session_player:get_cs_conn_index() >= 0 then
				MessageMgr:send_to_client(session_player, "hld.auction_proto.operate_end", t_client_msg, 13702)
			end
			self:operate_selling_lock(item_guid, false);
		end
		if  session_player == nil and session_player:get_cs_conn_index() < 0 then
		
			self:operate_selling_lock(item_guid, false);
		
		else
			--对方必须在线 否则不知道扣多少钱
			local t_cs_msg = {}
			t_cs_msg.auction_info = deep_copy(info)
			t_cs_msg.role_guid = role_guid;

			delog("----css----------------", price_type, price_value)

			MessageMgr:send_to_cs(session_player, "hld.ws2cs_proto.auction_find_bid_target_end", t_cs_msg, 34727)

		end
	end
end

function auction_mgr:bid_success_proc(auction_info, old_buyer)

	local t_dp_msg = {}
	t_dp_msg.new_record_id = hld.make_guid_lua(0)
	t_dp_msg.old_buyer_guid = old_buyer;
	local  new_auction_info = deep_copy(auction_info);
	self:update_auction_time(new_auction_info);
	t_dp_msg.auction_info = new_auction_info

	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_bid_success", t_dp_msg, 34730, 0)

	if old_buyer ~= 0  and  old_buyer ~= auction_info.buyer_guid then
		local old_buyer_ptr = hld.get_guid_64(old_buyer)
		local session_player = hld.get_client_session_by_guid(old_buyer_ptr);
		if session_player ~= nil and session_player:get_cs_conn_index() >= 0 then
			local t_client_msg = {}
			t_client_msg.item_id = auction_info.item_info.data_ary[hld.e_item_info_info_id + 1]
			t_client_msg.item_num = auction_info.item_info.data_ary[hld.e_item_info_stack_count + 1]
			MessageMgr:send_to_client(session_player, "hld.auction_proto.bid_are_overtaken", t_client_msg, 13713)
		end
	end
end

function auction_mgr:clear_old_item()
	local t_dp_msg = {}
	t_dp_msg.cur_time = get_g_time_mesc()
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_clear_old_item", t_dp_msg, 34732, 0)

	self.m_is_clearing_old_item = true;
end


function auction_mgr:get_old_item_end_proc(auction_info, data_num)
	local  game_config_ptr = template_manager:get_line("GameConfigTemplate", 99000001);
	if game_config_ptr == nil then
		return;
	end
	for i=1,data_num do

		self:buy_set_bind(auction_info[i]);
		self:send_reward_to_buyer(auction_info[i]);
		self:share_reward_to_seller(auction_info[i]);

		if auction_info[i].trade_type == hld.e_trading_type_normal_sell or auction_info[i].trade_type == hld.e_trading_type_world then
			self:add_record_to_db(auction_info[i], auction_info[i].buyer_guid, 0, false);
			self:add_record_to_db(auction_info[i], auction_info[i].seller_guid, 0, true);
	    elseif auction_info[i].trade_type == hld.e_trading_type_legion then
			
				--军团拍卖 记录属于军团 且 出售人就是军团guid
			self:add_record_to_db(auction_info[i], auction_info[i].seller_guid);
			if auction_info[i].buyer_guid == 0 and not self.m_is_force_clear_old_item then
				
				if auction_info[i].is_sky_order ~= 1 then
					
					local temp_info = auction_info[i];
					temp_info.trade_type = hld.e_trading_type_world;
					temp_info.seller_guid = 0;
					temp_info.sell_time = temp_info.deal_time;
					temp_info.deal_time = get_end_time(game_config_ptr.WorldAuctionItemLiveTime) + self:get_config_param(e_auction_param_wait_bid) * second_tick_time;

					self:add_auction_info_to_db(temp_info, nil, nil, true);

					self:send_up_notice(temp_info);

					self:set_need_world_acution_check(true);
				end
			end
		else
		end
	end

	self.m_is_clearing_old_item = false;

	if self.m_is_force_clear_old_item and data_num < auction_clear_item_num_per_time then
		
		self.m_is_force_clear_old_item = false;
			--FAITH_LOG_CONSOLE(server_log::get_game_log(), __XFUNCTION__ << "\nServer Merge Auction Clear Success, Please Wait 2 Minutes And Do Next Step\n");
	end
	
end


function auction_mgr:buy_set_bind(auction_info_ptr)
	if auction_info_ptr == nil then
		
		return;
	end
	auction_info_ptr.item_info.data_ary[hld.e_item_info_locked + 1] = 1;
end

local  max_item_per_mail = 5;	
function auction_mgr:init_mail_data_array(msg_data,  t_item_guid,  money_typ1,  money_num1,  money_typ2,  money_num2, sender_guid,  cur_time_sec)
	   msg_data.mail_guid = hld.make_guid_lua(0)
	   msg_data.data_ary = {}
	   for i=1, hld.EMailInfo_max do
	   	   table.insert(msg_data.data_ary, 0)
	   end
	   msg_data.text_contents = ""
	   msg_data.sender_name = ""
	   msg_data.mail_title = ""

	   for i=1,#t_item_guid do
	   	 if i > max_item_per_mail then
	   	 	break
	   	 end
	   	 local guid_ptr = hld.get_guid_64(t_item_guid[i])
	   	 local idx = i - 1

	   	 msg_data.data_ary[idx * 2 + hld.EMailInfo_ItemGuid11 + 1] = guid_ptr:get_A()
	   	 msg_data.data_ary[idx * 2 + hld.EMailInfo_ItemGuid12 + 1] = guid_ptr:get_B()
	   end


		msg_data.data_ary[hld.EMailInfo_MoneyTyp1 + 1] = money_typ1;
		msg_data.data_ary[hld.EMailInfo_MoneyNum1 + 1] = money_num1;
		msg_data.data_ary[hld.EMailInfo_MoneyTyp2 + 1] = money_typ2;
		msg_data.data_ary[hld.EMailInfo_MoneyNum2 + 1] = money_num2;

		msg_data.data_ary[hld.EMailInfo_ReadState + 1] = 0;

		local sender_guid_ptr = hld.get_guid_64(sender_guid)

		msg_data.data_ary[hld.EMailInfo_SenderGuid1 + 1] = sender_guid_ptr:get_A();
		msg_data.data_ary[hld.EMailInfo_SenderGuid2 + 1] = sender_guid_ptr:get_B();
		msg_data.data_ary[hld.EMailInfo_SendTime + 1] = cur_time_sec;
end


function auction_mgr:send_reward_to_buyer(auction_info)
    delog("---1111111111111111111111111auction_mgr:send_reward_to_buyer----")
	if auction_info.trade_type == hld.e_trading_type_normal_sell then

			--普通没竞拍 东西也不在这给
	
	elseif auction_info.trade_type == hld.e_trading_type_world or auction_info.trade_type == hld.e_trading_type_legion then
		    delog("-------------auction_mgr:send_reward_to_buyer---start----")

			if auction_info.buyer_guid == 0 then
				return;
			end
			local  sender_name = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_sender_system);
			local  mail_title =  hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_auction_purchase_title);
			local  mail_content = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_auction_purchase_content);

			if sender_name == "" or  mail_title == "" or mail_content == "" then
				return;
			end
			local  temp_mail_info = {}


			local   temp_item_guid_array = {};

			temp_item_guid_array[1] = auction_info.item_info.item_guid;

			self:init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0, os.time());

			temp_mail_info.sender_name = sender_name;
			temp_mail_info.mail_title = mail_title;
			temp_mail_info.text_contents = mail_content;

			local buyer_guid_ptr = hld.get_guid_64(auction_info.buyer_guid)

			local s_mail_buffer = encode("hld.db_proto.role_mail_row", temp_mail_info)

			local mail_info_ptr = hld.get_mail_info_by_str(s_mail_buffer, string.len(s_mail_buffer));

			delog("----1111111111")

			local s_item_buffer = encode("hld.st_proto.st_item_info", auction_info.item_info)

			delog("----2222222222")

			local item_info_ptr = hld.get_item_info_by_str(s_item_buffer, string.len(s_item_buffer))


			delog("----33333333333333")

			hld.mail_ws_mgr:get_instance():send_mail(buyer_guid_ptr, 0, mail_info_ptr, item_info_ptr, 1, false);

			local s_info_buffer = encode("hld.st_proto.st_auction_info", auction_info)
		    local info_ptr = hld.get_auction_info_by_str(s_info_buffer, string.len(s_info_buffer))

		    delog("----4444444444")

		    local s_default_buffer = ""

		    local default_role_ptr = hld.get_unit_info_by_str(s_default_buffer, string.len(s_default_buffer))
		    delog("----555555")
		    local default_login_ptr = hld.get_login_fixed_data_by_str(s_default_buffer, string.len(s_default_buffer))

		    delog("----66666666666")


			hld.server_log:auction_db_log(info_ptr, hld.e_auction_state_deal, default_role_ptr, default_login_ptr, hld.e_login_type_new_account_and_password);

			 delog("-------------auction_mgr:send_reward_to_buyer---end----")

			local log_head = hld.get_log_common_head()

			hld.lua_set_log_common_head_part3(log_head, "null", hld.get_world_server():get_server_id(), get_g_time_mesc())

			hld.server_log:serverAuction(log_head, "null",tostring(auction_info.seller_guid), auction_info.seller_name,0,
				tostring(auction_info.item_info.item_guid), auction_info.item_info.data_ary[hld.e_item_info_info_id + 1], auction_info.item_info.data_ary[hld.e_item_info_stack_count +1],
				auction_info.trade_type, "null", tostring(auction_info.buyer_guid), auction_info.buyer_name, 0, auction_info.sell_money_type, hld.globle_data:get_instance():get_money_str_by_type(auction_info.sell_money_type), auction_info.sell_total_money, hld.e_auction_state_deal);
	else
	end


	self:del_bid_record(auction_info.item_info.item_guid);
end


function auction_mgr:share_reward_to_seller(auction_info)

		if auction_info.trade_type == hld.e_trading_type_normal_sell or auction_info.trade_type == hld.e_trading_type_world then
		elseif auction_info.trade_type == hld.e_trading_type_legion then

			local t_dp_msg = {}
			t_dp_msg.item_guid = auction_info.item_info.item_guid
			t_dp_msg.item_template_id = auction_info.item_info.data_ary[hld.e_item_info_info_id + 1]
			t_dp_msg.buyer_guid = auction_info.buyer_guid


			if auction_info.buyer_guid == 0  and auction_info.sell_money_type == hld.e_money_type_jewel then
			
				t_dp_msg.money_type =  tonumber(hld.e_money_type_jewel_bind);
			else
				t_dp_msg.money_type = auction_info.sell_money_type;
			end
			t_dp_msg.money_value = auction_info.cur_bid_money;
			if auction_info.buyer_guid == 0 then
			
				local item_id = auction_info.item_info.data_ary[hld.e_item_info_info_id +1];
				local  item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
				if item_template_ptr ~= nil and #item_template_ptr.AuctionAutoRecoverMoneyArray > t_dp_msg.money_type then
				
					t_dp_msg.money_value = item_template_ptr.AuctionAutoRecoverMoneyArray[t_dp_msg.money_type + 1];
				end
			end

			MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_req_share_list", t_dp_msg, 34735, 0)
	  else
	  end
			
end


function auction_mgr:add_record_to_db(auction_info, in_role_guid, in_record_guid, in_is_sell)
	    local role_guid = in_role_guid or 0
	    local record_guid = in_record_guid or 0 
	    local is_sell = in_is_sell or false
	    if auction_info.trade_type == hld.e_trading_type_world and auction_info.buyer_guid == 0 then
		
			return;
		end

		local t_dp_msg = {};
		if record_guid ~= 0 then
		
			t_dp_msg.record_guid = record_guid;
		
		else
		
			t_dp_msg.record_guid = hld.make_guid_lua(role_guid);
		end
		t_dp_msg.role_guid = role_guid;
		t_dp_msg.is_sell = is_sell;
		t_dp_msg.auction_info = deep_copy(auction_info)

		MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_add_purchase_record", t_dp_msg, 34710, 0)

end

function auction_mgr:del_bid_record( item_guid)
	    if item_guid == 0 then
			return;
		end

		local t_dp_msg = {}
		t_dp_msg.item_guid = item_guid
        MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_del_bid_record", t_dp_msg, 34741, 0)
end

function auction_mgr:clear_old_bid_record()
	 local t_dp_msg = {}
	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_clear_old_bid_record", t_dp_msg, 34742, 0)
end

local  max_mail_title_length = 64;								-- 最大标题长度
local  max_text_contents_length = 400                      

function auction_mgr:req_share_list_end_proc(money_type,  money_value, item_template_id, share_info,  data_num,  buyer_guid)
	if share_info == nil  or data_num == 0 or money_value == 0 then
		return;
	end

	local game_config_ptr = template_manager:get_line("GameConfigTemplate", 99000001);
	if game_config_ptr == nil then
		
		return;
	end

	local region_code = hld.globle_data:get_instance():get_region_code();
	if region_code == -1 then
		return
	end

	local  temp_mail_info = {}
	local  temp_item_guid_array = {};

	local  sender_name = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_sender_system);
	if sender_name == "" then
		return;
	end

	self:init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0, os.time())
	temp_mail_info.sender_name =  sender_name

	local  share_money_value = money_value;
	if  share_money_value > self:get_config_param(e_auction_param_share_money_limit) then
		share_money_value = self:get_config_param(e_auction_param_share_money_limit)
	end
	local deduct_num = 0;
	if #game_config_ptr.AuctionShareRate >= 2 and #game_config_ptr.AuctionShareRate % 2 == 0 then
		
			local  share_rate = game_config_ptr.AuctionShareRate;
			local  arr_len = #game_config_ptr.AuctionShareRate;
			for i=1, arr_len, 2 do
				if share_rate[i] == region_code then
					deduct_num = share_money_value * share_rate[ i + 1] / 100 - 0.5
				end
			end
	end

	share_money_value  = share_money_value - math.floor(deduct_num)

	temp_mail_info.data_ary[hld.EMailInfo_MoneyTyp1 + 1] = money_type;

	for i=1, data_num do
			temp_mail_info.mail_guid = hld.make_guid_lua(share_info[i].role_guid)
			temp_mail_info.mail_title = string.sub(share_info[i].share_mail_title, 1, max_mail_title_length)

			temp_mail_info.text_contents = string.sub( share_info[i].share_mail_content, 1,  max_text_contents_length);
			local temp_money_value = share_money_value * share_info[i].share_rate + 0.5;
			--+0.5 四舍五入
 
			local share_money_num = math.floor(temp_money_value);
			if share_money_num <= 0 then
				share_money_num = 1;
			end
			temp_mail_info.data_ary[hld.EMailInfo_MoneyNum1 + 1] = share_money_num;
			local guid_ptr = hld.get_guid_64(share_info[i].role_guid)
			local s_mail_buffer = encode("hld.db_proto.role_mail_row", temp_mail_info)
			local mail_ptr = hld.get_mail_info_by_str(s_mail_buffer, string.len(s_mail_buffer))
			hld.mail_ws_mgr:get_instance():send_mail(guid_ptr, 0, mail_ptr, nil, 0, false);
	end

	if buyer_guid  ~= 0 then
		local buyer_guid_ptr = hld.get_guid_64(buyer_guid)
		local  session_player = hld.get_client_session_by_guid(buyer_guid_ptr);
		if session_player == nil then
			return;
		end
		hld.server_log:auctionbid_log(session_player:get_third_info(), session_player:get_unit_info(), money_type, money_value, item_template_id, data_num, buyer_guid_ptr);
	end
end

function auction_mgr:force_clear_all_old_item()
	  self.m_is_clearing_old_item = false;
	  self.m_is_force_clear_old_item = true;
end

function auction_mgr:check_world_auction_sell()
	 local  t_dp_msg = {}
	 MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_check_world_sell", t_dp_msg, 34743, 0)
end


function auction_mgr:set_world_auction_sell_num( sell_num)
	   self.m_world_sell_num = sell_num;
end

function auction_mgr:set_need_world_acution_check(is_need)
	   self.m_need_check_world_auction = is_need;
end

function auction_mgr:dp_check_world_auction_sell_end(sell_num)
	   self:set_world_auction_sell_num(sell_num);

	   local is_world_sell = (sell_num > 0 and true) or false;
	   self:set_need_world_acution_check(is_world_sell)

		local t_client_msg = {}
		t_client_msg.sell_flag = is_world_sell

		MessageMgr:send_message_to_all_client("hld.auction_proto.auction_sell_flag", t_client_msg, 13714)

end

function auction_mgr:set_is_show(auction_info)
	  auction_info.is_show = 1;

end


function auction_mgr:add_wait_show(auctin_timer_info)
	local start = self:get_config_param(e_auction_param_private_delay_start);
    local i_end = self:get_config_param(e_auction_param_private_delay_end);
	auctin_timer_info.show_time = os.time() + hld.random_gen:get_random(start, i_end);

	--auctin_timer_info.show_time = os.time() + 20

	delog("-------add---wait---show--")
	table.insert(self.m_wait_show_vec, auctin_timer_info)
end


function auction_mgr:wait_tick()
	  local cur_time = os.time()
	  for k,v in pairs(self.m_wait_show_vec) do
	  	 if v.show_time < cur_time and v.is_update == 0 then
	  	 	delog("-------$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
	  	 	self:send_to_dp_show(v.auction_info)
	  	 	v.is_update = 1
	  	 end
	  end
end



function auction_mgr:find_timer(item_guid)
      for k, v in pairs(self.m_wait_show_vec) do
      	 if v.auction_info.item_info.item_guid == item_guid then
      	 	return v;
      	 end
      end
      return nil;
end

function auction_mgr:del_timer(item_guid)
	local key  = nil
	for k, v in pairs(self.m_wait_show_vec) do
	 	 if v.auction_info.item_info.item_guid == item_guid then
      	 	key = k
      	 end 
	end
	if key ~= nil then
	 	table.remove(self.m_wait_show_vec, key)
	end
end

function auction_mgr:send_to_dp_show(auction_info)
	local t_dp_msg = {}
	t_dp_msg.data_info = deep_copy(auction_info)
	t_dp_msg.end_time = self:get_config_param(e_auction_param_private_del)
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_show_sell", t_dp_msg, 34745, 0)
end

local e_item_color_orange = 5		--橙色
function auction_mgr:send_to_dp_show_end(result,  auction_info)

	local timer_info = self:find_timer(auction_info.item_info.item_guid);
	if timer_info == nil then
		return;
	end
	if result ~= 1 then
		
		timer_info.is_update = 0;
	end
	--self:sell_rare_item_proc_ex(timer_info);
	if timer_info.auction_info.item_color >= e_item_color_orange then
		
		self:sell_rare_item_proc_ex(timer_info);
	end

	self:del_timer(auction_info.item_info.item_guid);
end

function auction_mgr:sell_rare_item_proc_ex(info)
	local  item_template_ptr = template_manager:get_line("ItemTemplate", info.auction_info.item_info.data_ary[hld.e_item_info_info_id + 1])
	if item_template_ptr == nil then
		return;
	end

	local  notice_param_array = hld.event_ws_mgr:get_instance():get_free_param();
		
	for i=1,hld.e_item_info_max do
		hld.event_ws_mgr:get_instance():add_free_param(info.auction_info.item_info.data_ary[i])
	end

	--跑马灯的
	local vec_notice = {};
	local t_notice_id = template_manager:get_str_id_by_notice_id(93000232)
	vec_notice[1] = tostring(t_notice_id[1])
	
	vec_notice[2] = info.auction_info.seller_name;

	--结构体中的name没有颜色代码
	local temp_string = hld.get_str_by_string_template_id(item_template_ptr.item_name, 0);
	vec_notice[3] = temp_string

	local  notice_str = table.concat( vec_notice, "&#")
	local  seller_guid_ptr = hld.get_guid_64(info.auction_info.seller_guid)
	hld.event_ws_mgr:get_instance():send_notice_to_all(93000232, os.time(), seller_guid_ptr, notice_str, info.template_id);

	--聊天频道的
	hld.event_ws_mgr:get_instance():send_notice_with_param(
			seller_guid_ptr, info.auction_info.seller_name, 
			"90089566", 
			notice_param_array, hld.e_notice_param_item_info, 
			hld.e_chat_type_world, info.template_id, 93000233);
end


function auction_mgr:cancel_del_show(auction_info)
	if auction_info.is_show == 1 then
		return;
	end
	self:del_timer(auction_info.item_info.item_guid);
end

local auction_time_out_interval = 60000
function auction_mgr:time_out_tick()
	local new_time = get_g_time_mesc()
	if self.m_time_out_last == 0 then
		self.m_time_out_last = new_time + auction_time_out_interval;
	end
	if self.is_request_time_out == true then
		return;
	end
	if new_time > self.m_time_out_last then
		delog("#####################################################")
		self.m_time_out_last = new_time + auction_time_out_interval;
		self:send_to_dp_time_out(self.m_is_first);
		self.m_is_first = 0;
		self.is_request_time_out = true;
	end
end

function auction_mgr:send_to_dp_time_out(is_first)
	if is_first == 1 then
		self.m_wait_show_vec = {}
	end

	local cur_time = os.time();

	local t_dp_msg = {}

	t_dp_msg.cur_mil_sec = cur_time - self:get_config_param(e_auction_param_private_del)
	t_dp_msg.cur_mil_sec = t_dp_msg.cur_mil_sec * 1000

	t_dp_msg.sell_time =  cur_time * 1000

	t_dp_msg.deal_time =  cur_time + self:get_config_param(e_auction_param_private_del)
	t_dp_msg.deal_time =  t_dp_msg.deal_time * 1000
	t_dp_msg.is_first = is_first


	MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_auction_time_out", t_dp_msg, 34747, 0)
end


function auction_mgr:send_to_dp_time_out_end(aution_info_array, num)
	
	self.is_request_time_out = false;
	if aution_info_array == nil or  num == 0 then
		return;
	end
	for i=1, num do
		self:send_to_owner(aution_info_array[i]);
		delog("---add_record_to_db---")
		self:add_record_to_db(aution_info_array[i], aution_info_array[i].seller_guid, 0, true);
		delog("---add_record_to_db---")
		self:sync_to_cs_del(aution_info_array[i]);
		delog("---add_record_to_db---")

	end
end


function auction_mgr:send_to_owner(auction_info);
	 self:give_item_to_seller(auction_info)
end


function auction_mgr:give_item_to_seller(auction_info)
	
	if auction_info.seller_guid == 0 then
		return;
	end
	local sender_name = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_sender_system);
	local mail_title = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_auction_time_out_title);
	local mail_content = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_auction_time_out_content);

	if sender_name == "" or  mail_title == "" or mail_content == "" then
		return;
	end
	local  temp_mail_info = {}

	local temp_item_guid_array = {};

	temp_item_guid_array[1] = auction_info.item_info.item_guid

	self:init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0, os.time());

	temp_mail_info.sender_name = sender_name;
	temp_mail_info.mail_title = mail_title;
	temp_mail_info.text_contents = mail_content;

	local seller_guid_ptr = hld.get_guid_64(auction_info.seller_guid)
	delog("---start--mail-----row---------")
	local s_mail_buffer = encode("hld.db_proto.role_mail_row", temp_mail_info)
	delog("---start--mail-----row---------")
	local mail_ptr = hld.get_mail_info_by_str(s_mail_buffer, string.len(s_mail_buffer))
	delog("---st_item_info--------") 
	local s_item_buffer = encode("hld.st_proto.st_item_info", auction_info.item_info)
	delog("---st_item_info--------") 

	local item_ptr = hld.get_item_info_by_str(s_item_buffer, string.len(s_item_buffer))
	delog("---st_item_info--------") 

	hld.mail_ws_mgr:get_instance():send_mail(seller_guid_ptr, 0, mail_ptr,item_ptr , 1 ,false);

end

function auction_mgr:sync_to_cs_del(auction_info)
	local seller_guid_ptr = hld.get_guid_64(auction_info.seller_guid)
	local session_player = hld.get_client_session_by_guid(seller_guid_ptr);
	if session_player ~= nil and session_player:get_cs_conn_index() >= 0 then

		local t_cs_msg = {}
		t_cs_msg.role_guid = auction_info.seller_guid
		t_cs_msg.auction_info = deep_copy(auction_info)

		MessageMgr:send_to_cs(session_player,"hld.ws2cs_proto.auction_time_out_del", t_cs_msg, 34752)

	end
end

function auction_mgr:time_tip_tick()
	 if #self.m_time_tip_vec == 0 then
	    return
	 end
	 local cur_sec = os.time()

	  for i = #self.m_time_tip_vec, 1, -1 do
	  	  local it = self.m_time_tip_vec[i]
	  	  if self:is_time_tip(it.sell_time, cur_sec) then
	  	  	 self:send_prepare_notice(it)
	  	  	 table.remove(self.m_time_tip_vec, i)
	  	  end
	  end 
end

local e_item_color_red = 6
function auction_mgr:add_tip(aution_info)
	if aution_info.item_color >= e_item_color_red then
	   table.insert(self.m_time_tip_vec, 1, aution_info)
	end
end

function auction_mgr:is_time_tip(sell_time, cur_time)
    local  delta_time = cur_time - sell_time // second_tick_time;
	if delta_time < self:get_config_param(e_auction_param_wait_notice) then
		return false;
	end
	return true;	
end


function auction_mgr:add_wait_time(info)
	local  item_template_ptr = template_manager:get_line("ItemTemplate", info.item_info.data_ary[hld.e_item_info_info_id +1])
	if item_template_ptr ~= nil then
		temp_info.deal_time = temp_info.deal_time + item_template_ptr.TradingAwaitTime * 1000;
		return
	end	
end


function auction_mgr:send_up_notice(info)
	 local  item_template_ptr = template_manager:get_line("ItemTemplate", info.item_info.data_ary[hld.e_item_info_info_id +1])
	 if item_template_ptr == nil then
		return;
	 end
	 if info.item_color < e_item_color_red then
		return;
	 end

	local  notice_param_array = hld.event_ws_mgr:get_instance():get_free_param();
		
	for i=1,hld.e_item_info_max do
		hld.event_ws_mgr:get_instance():add_free_param(info.item_info.data_ary[i])
	end

	--跑马灯的
	local vec_notice = {};
	local t_notice_id = template_manager:get_str_id_by_notice_id(93000416)
	vec_notice[1] = tostring(t_notice_id[1])
	
	vec_notice[2] = info.seller_name;

	--结构体中的name没有颜色代码
	local temp_string = hld.get_str_by_string_template_id(item_template_ptr.item_name, 0);
	vec_notice[3] = temp_string

	local  notice_str = table.concat( vec_notice, "&#")
	local default_guid_ptr = hld.get_guid_64(0)
	hld.event_ws_mgr:get_instance():send_notice_to_all(93000416, os.time(), default_guid_ptr, notice_str, 0);

	--聊天频道的
	hld.event_ws_mgr:get_instance():send_notice_with_param(
			default_guid_ptr, "", 
			"90089577",
			notice_param_array, hld.e_notice_param_item_info, 
			hld.e_chat_type_system, 0, 93000417);


	self:add_tip(info)
end

function auction_mgr:send_prepare_notice(info)
	
    local  item_template_ptr = template_manager:get_line("ItemTemplate", info.item_info.data_ary[hld.e_item_info_info_id +1])
	if item_template_ptr == nil then
		return;
	 end
	 if info.item_color < e_item_color_red then
		return;
	 end

	local  notice_param_array = hld.event_ws_mgr:get_instance():get_free_param();
		
	for i=1,hld.e_item_info_max do
		hld.event_ws_mgr:get_instance():add_free_param(info.item_info.data_ary[i])
	end

	--跑马灯的
	local vec_notice = {};
	local t_notice_id = template_manager:get_str_id_by_notice_id(93000418)
	vec_notice[1] = tostring(t_notice_id[1])
	
	vec_notice[2] = info.seller_name;

	--结构体中的name没有颜色代码
	local temp_string = hld.get_str_by_string_template_id(item_template_ptr.item_name, 0);
	vec_notice[3] = temp_string

	local  notice_str = table.concat( vec_notice, "&#")
	local default_guid_ptr = hld.get_guid_64(0)
	hld.event_ws_mgr:get_instance():send_notice_to_all(93000418, os.time(), default_guid_ptr, notice_str, 0);

	--聊天频道的
	hld.event_ws_mgr:get_instance():send_notice_with_param(
			default_guid_ptr, "", 
			"90089578",
			notice_param_array, hld.e_notice_param_item_info, 
			hld.e_chat_type_system, 0, 93000419);

end

function auction_mgr:update_auction_time(auction_info)

	    local  end_time = auction_info.deal_time // 1000;
		local  cur_time = os.time();
		delog("-----xxxx--", end_time, cur_time)
		local  delta_time = end_time - cur_time;
		if delta_time < 0 then
			return;
		end
		delog("-----delta--", delta_time, self:get_config_param(e_auction_param_add_time))
		if delta_time > self:get_config_param(e_auction_param_add_time) then
		
			auction_info.add_time = 0;
			return;
		end
		if auction_info.add_time >= self:get_config_param(e_auction_param_add_n) then
			return;
		end
		local interval_time = 30;
		if  self:get_config_param(e_auction_param_add_n) > 0 then
		
			interval_time = self:get_config_param(e_auction_param_add_time) // self:get_config_param(e_auction_param_add_n);
		end
		auction_info.add_time =  auction_info.add_time + 1;
		auction_info.deal_time =   auction_info.deal_time + interval_time * 1000;
		delta_time = delta_time + interval_time;
		if delta_time > self:get_config_param(e_auction_param_add_time) then
			auction_info.add_time = 0;
		end
end

function auction_mgr:get_config_param(param_type)
	
	local  config_init = {86400, 60, 600, 600, 540, 180, 6,100,39600,4};
	local  game_config_template_ptr = template_manager:get_line("GameConfigTemplate", 99000001);
	if game_config_template_ptr == nil then
		return config_init[param_type];
	end
	if #game_config_template_ptr.AuctionArray < e_auction_param_max then
		
		return config_init[param_type];
	end
	return game_config_template_ptr.AuctionArray[param_type];
end


function auction_mgr:on_login(session)
    if  session == nil then
		return;
	end

	if not session:is_self_server() then
		return;
	end
	local is_world_sell = (self.m_world_sell_num > 0 and true)  or false

	local t_client_msg = {}
	t_client_msg.sell_flag = is_world_sell

	MessageMgr:send_to_client(session, "hld.auction_proto.auction_sell_flag", t_client_msg, 13714)
end


function auction_mgr:auction_bid_db_sucess(buyer_guid, item_guid)
	local t_client_msg = {}
	t_client_msg.item_guid = item_guid
	t_client_msg.item_num = 0
	t_client_msg.operate_type = e_auction_operate_type_bid
	t_client_msg.money_type = 0
	t_client_msg.money_num = 0
	t_client_msg.operate_result = ConstDefine.e_auction_bid_success 

	local buyer_guid_ptr = hld.get_guid_64(buyer_guid)

	local new_session_player = hld.get_client_session_by_guid(buyer_guid_ptr);
	if new_session_player ~= nil and  new_session_player:get_cs_conn_index() >= 0 then
		MessageMgr:send_to_client(new_session_player,"hld.auction_proto.operate_end", t_client_msg, 13702)
	end
end


function auction_mgr:add_person_info(persion_info)
      table.insert(self.m_person_info_queue, 1, persion_info)
end

local auction_one_frame_person_num = 50

function auction_mgr:person_tick()
	 if #self.m_person_info_queue == 0 then
	 	return
	 end
	 local enter_count = 0;
	 for i = #self.m_person_info_queue, 1, -1  do
	 	 local elem = self.m_person_info_queue[i]
	 	 self:process_person_info(elem)
	 	 enter_count = enter_count + 1
	 	 table.remove(self.m_person_info_queue, i)
	 	 if enter_count == auction_one_frame_person_num then
             break
         end
	 end
end


function auction_mgr:process_person_info(person_info)

	 local refresh_item_num = self:get_config_param(e_auction_param_person_refresh_item_num);

	 local idx = self:get_area_index(person_info.role_lv);
	 if idx < 0 or idx >= 6 then
		 return;
	 end
	 local p_vec = self:get_item_config(person_info.role_class_type);
	 if p_vec == nil then
		 return;
	 end

	 for i=1,refresh_item_num do
	 	local item_id = self:get_item_id(p_vec, idx);
		if item_id == 0 then 
			break;
		end
	    self:make_person_auction(person_info, item_id);
	 end
end


function auction_mgr:make_person_auction(person_info, item_id)
	    local  item_template_ptr = template_manager:get_line("ItemTemplate", item_id);
		if item_template_ptr == nil then
			return;
		end

		local item_info_ptr = hld.get_item_info_by_str("", 0)

        local result = hld.init_unit:init_item_data(item_info_ptr, item_id, -1, 1, hld.e_bag_type_bag, 0, 1,0,0);

        local auction_info_ptr = hld.get_auction_info_by_str("", 0)


		if hld.globle_data:get_instance():make_auction_info(auction_info_ptr, item_info_ptr, 1, hld.e_trading_type_world) == false then
			return;
		end


		local hex_auction_info_str = hld.get_auction_buffer(auction_info_ptr)

		delog("--hex_auction_info_str-", hex_auction_info_str)

		local bin_auction_info_str = hex2bin(hex_auction_info_str)

		delog("--bin_auction_info_str-", bin_auction_info_str)

		local new_auction_info = decode("hld.st_proto.st_auction_info", bin_auction_info_str)

		delog(new_auction_info)

		--/////////////////////////////////////////////
		new_auction_info.is_show = 1;
		new_auction_info.show_guid = person_info.role_guid;
		--//////////////////////////////////////////////
		local RegionCode = hld.globle_data:get_instance():get_region_code();
		if RegionCode == -1 or #item_template_ptr.AuctionCanSellMoneyArray <= RegionCode then
			return;
		end
		local money_type = item_template_ptr.AuctionCanSellMoneyArray[RegionCode + 1];
		if #item_template_ptr.AuctionRecommendMoneyArray < money_type then
			return;
		end
		
		local money_price = item_template_ptr.AuctionRecommendMoneyArray[money_type + 1];

		new_auction_info.sell_money_type = money_type;
		new_auction_info.sell_total_money = money_price;
		new_auction_info.sell_single_money = money_price;

		--///////////////////////////////////////
		if person_info.item_time == 0 then
		
			new_auction_info.sell_time = get_g_time_mesc();
			new_auction_info.deal_time = get_end_time(self:get_config_param(e_auction_param_private_del));
		
		else
		
			new_auction_info.sell_time = person_info.item_time;
			new_auction_info.deal_time = person_info.item_time + self:get_config_param(e_auction_param_private_del) * second_tick_time;

		end
		--//////////////////////////////////////

		self:add_auction_info_to_db(new_auction_info, nil, nil, true);
end


function auction_mgr:get_item_config(role_class_type)
	local  game_config_template_ptr = template_manager:get_line("GameConfigTemplate", 99000001);
	if  game_config_template_ptr == nil then
		return nil;
	end

	local p_vec = nil;
   
	if role_class_type == hld.e_class_type_barserker then
		p_vec = game_config_template_ptr.BarserkerAuctionEquRandomSet;
	elseif role_class_type == hld.e_class_type_wizard then
		p_vec = game_config_template_ptr.WizardAuctionEquRandomSet;
	elseif role_class_type == hld.e_class_type_guardian then
		p_vec = game_config_template_ptr.GuardianAuctionEquRandomSet;
	elseif role_class_type == hld.e_class_type_assassinator then
		p_vec = game_config_template_ptr.AssassinatorAuctionEquRandomSet;
	else 
	end
	return p_vec;
end

function auction_mgr:get_area_index(role_lv)
	 --delog("--111-person_info.role_lv",  person_info.role_lv)
	 local  range = { 0, 199, 200, 299, 300, 399, 400, 499, 500, 599, 600,9999};

	 for i=0, 5 do
	 	if range[i * 2 + 1] <= role_lv and  role_lv <= range[i * 2 + 2] then
	 		return i
	 	end
	 end
	 return -1;
end

function auction_mgr:get_item_id(p_vec, idx)
	
	if p_vec == nil or #p_vec ~= 54 then
	
		return 0;
	end
	if idx < 0 or  idx >= 6 then
		return 0;
	end
	local  random_pos = hld.random_gen:get_random(0, 8);
	local item_id = p_vec[idx * 9 + random_pos + 1];
	return item_id;
end


local max_legion_member_num = 250

function auction_mgr:give_activity_reward(act_config_id, mem_list,  play_num,  item_id, item_num, legion_guid)
	 delog("--auction_mgr:give_activity_reward---")
	 local act_config_ptr = template_manager:get_line("ActivityCommonConfigTemplate", act_config_id)
	 if act_config_ptr == nil then
	 	return
	 end
	 delog("-1111-auction_mgr:give_activity_reward---")
	 local  attend_num = tonumber(act_config_ptr.ParamString1);
	if  attend_num < play_num then
		attend_num = play_num
	end
	if attend_num <= 0 then
		return;
	end
	local _share_rate = 1 / attend_num;

	local sell_item_mail_t = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_legion_rank_rwd_title);
	local sell_item_mail_c = hld.globle_data:get_instance():get_mail_common_text_id(hld.e_mail_common_text_legion_rank_rwd_content);
	local  content_params = {}
	content_params[1] = sell_item_mail_c;
	content_params[2] = tostring(act_config_ptr.ActivityName);
	sell_item_mail_c = table.concat(content_params, "&#")

	 delog("-222-auction_mgr:give_activity_reward---")

	local rwd_relate_mem_num = 1;
	local rwd_relate_mems = {};

	for i=1,play_num do
		rwd_relate_mems[rwd_relate_mem_num] = {}
		rwd_relate_mems[rwd_relate_mem_num].role_guid = mem_list[i];
		rwd_relate_mems[rwd_relate_mem_num].share_rate = (_share_rate > 0.2 and 0.2) or _share_rate
		rwd_relate_mems[rwd_relate_mem_num].share_mail_title = sell_item_mail_t;
		rwd_relate_mems[rwd_relate_mem_num].share_mail_content = sell_item_mail_c 
		rwd_relate_mem_num = rwd_relate_mem_num + 1;
		if rwd_relate_mem_num >= max_legion_member_num then
			break;
		end
		--FAITH_LOG_SERVER(server_log::get_game_log(), "roleguid:" << mem_list[i] << "   sharerate:" << _share_rate << "   itemid:" << item_id << "   itemnum:" << item_num << "   activittype:" << act_config_ptr->ActivityType);
	end

	local  rwd_item_info = hld.get_item_info_by_str("", 0);
	hld.init_unit:init_item_data(rwd_item_info, item_id, -1, item_num, hld.e_bag_type_bag, 0, 1,0,0);

	local legion_guid_ptr = hld.get_guid_64(legion_guid)

	for i=1,item_num  do
		local auction_info_ptr = hld.get_auction_info_by_str("", 0)

		hld.globle_data:get_instance():make_auction_info(auction_info_ptr, rwd_item_info, 1, hld.e_trading_type_legion,legion_guid_ptr, "", 0, 0, 0) 

		local hex_auction_info_str = hld.get_auction_buffer(auction_info_ptr)

		local bin_auction_info_str = hex2bin(hex_auction_info_str)

		local rwd_auction_info = decode("hld.st_proto.st_auction_info", bin_auction_info_str)
		self:add_wait_time(rwd_auction_info)
		self:add_auction_info_to_db(rwd_auction_info, nil, nil , true)
		self:send_up_notice(rwd_auction_info)

		for j=1, rwd_relate_mem_num - 1 do
			rwd_relate_mems[j].item_guid = rwd_auction_info.item_info.item_guid
		end

		self:add_share_list_to_db(rwd_relate_mems, rwd_relate_mem_num)
	end

	local t_client_msg = {}

	local legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_legion(legion_guid_ptr)

    MessageMgr:send_message_to_all_legion_member(legion_ws_ptr, "hld.legion_proto.notice_new_item_in_legion_auction", t_client_msg, 12193)

	--send_message_to_all_member(&msg, e_msgindex_s2c_notice_new_item_in_legion_auction);
end


function auction_mgr_give_activity_reward(act_config_id, mem_list_str, item_id, item_num, legion_guid)
	 local t_mem_list = Common:StrToTable(mem_list_str)
	 delog(t_mem_list)
	 local play_num = #t_mem_list
	 auction_mgr:give_activity_reward(act_config_id, t_mem_list,  play_num,  item_id, item_num, legion_guid)
end


function auction_mgr_test_legion()
	 delog("-------test--auction_mgr_test_legion-")
	 local act_config_id = 120000011
	 local mem_list_str = "6862246746932651835,6862985687465994043,6863364172869019451,6863623077792589627,6865859866695575355,6937914205148292923,"
	 local legion_guid = 6936428206593419067

	 local t_item_id = {31020375}

	 for i=1,#t_item_id do
	 	auction_mgr_give_activity_reward(act_config_id, mem_list_str, t_item_id[i], 1, legion_guid)
	 end
end

----///////////////////////////////CS//////////////////////////////////////////////////////////

function auction_mgr_add_auction_info_to_db(message_data)
	  delog("--111-auction_mgr_add_auction_info_to_db-----")
	 local msg_data = decode("hld.cs2ws_proto.auction_add_item_into_db", message_data)
	 if msg_data == nil then
	 	return
	 end
	 delog("-222--auction_mgr_add_auction_info_to_db-----")
	 local role_info = msg_data.role_info 
	 if role_info.role_first_kill_ary ~= nil then
	 	role_info.first_kill_npc_arr = role_info.role_first_kill_ary
	 	role_info.role_first_kill_ary = nil 
	 end
	 auction_mgr:add_auction_info_to_db(msg_data.auction_info, msg_data.old_item_guid, role_info, true, msg_data.third_info);
end

function auction_mgr_buy(role_guid, item_guid)
	delog("---buy--", role_guid, item_guid)
	auction_mgr:buy(role_guid, item_guid)
end

function auction_mgr_send_auction_puchase_success_info(message_data)
     delog("---send_auction_puchase_success_info-----")
	 local auction_info = decode("hld.st_proto.st_auction_info", message_data)
	 if auction_info == nil then
	 	return
	 end
	 auction_mgr:send_auction_puchase_success_info(auction_info)
end


function auction_mgr_cancel_sell(role_guid, item_guid)
	auction_mgr:cancel_sell(role_guid, item_guid)
end

function auction_mgr_trade_bid(role_guid,item_guid,price_money_type,price_money_value)
	delog("---auction_mgr_trade_bid-----")
	auction_mgr:trade_bid(role_guid,item_guid,price_money_type,price_money_value)
end

function auction_mgr_bid_success_proc(message_data, old_buyer_guid)
	delog("---auction_mgr_bid_success_proc-----")
	 local auction_info = decode("hld.st_proto.st_auction_info", message_data)
	 if auction_info == nil then
	 	return
	 end
	 auction_mgr:bid_success_proc(auction_info, old_buyer_guid)
end

function auction_mgr_share_reward_to_seller(message_data)
	 delog("-----auction_mgr_share_reward_to_seller----")
	 local auction_info = decode("hld.st_proto.st_auction_info", message_data)
	 if auction_info == nil then
	 	return
	 end
	 auction_mgr:share_reward_to_seller(auction_info)
end

function auction_mgr_add_record_to_db( message_data,  role_guid, record_guid, is_sell)
	 delog("-----auction_mgr_add_record_to_db----")
	 local auction_info = decode("hld.st_proto.st_auction_info", message_data)
	 if auction_info == nil then
	 	return
	 end

	 auction_mgr:add_record_to_db(auction_info, role_guid, record_guid, is_sell)
end


function auction_mgr_del_bid_record(item_guid)
	 delog("-------auction_mgr_del_bid_record----")
	 auction_mgr:del_bid_record(item_guid)
end


function auction_mgr_add_person_info(role_guid, role_class_type, role_lv, item_time)
	   local person_info = 
	   {
	     role_guid = role_guid,
	     role_class_type = role_class_type,
	     role_lv = role_lv,
	     item_time = item_time,
	   }
	  delog(person_info)
	  auction_mgr:add_person_info(person_info)
end



function auction_mgr_on_login(role_guid)
       local guid_ptr = hld.get_guid_64(role_guid)

       local session = hld.get_client_session_by_guid(guid_ptr)

       auction_mgr:on_login(session)
end


------------/////////////DP--WS///////////////////////////////////

function auction_mgr_auction_bid_db_sucess(buyer_guid, item_guid)
	  delog("---dp--auction_bid_db_sucess")
 	  auction_mgr:auction_bid_db_sucess(buyer_guid, item_guid)
end 


function auction_mgr_send_to_dp_time_out_end(message_data)
       delog("---dp--send_to_dp_time_out_end")
	   local msg_data = decode("hld.dp2s_proto.aution_time_out_end", message_data)
	   if msg_data == nil then
	   	  return
	   end
	   auction_mgr:send_to_dp_time_out_end(msg_data.data_info, msg_data.data_num)
end


function auction_mgr_send_to_dp_show_end(result, message_data)
      delog("---dp--send_to_dp_show_end")
	  local msg_data = decode("hld.st_proto.st_auction_info", message_data)
	  if msg_data == nil then
	  	return
	  end
	  auction_mgr:send_to_dp_show_end(result, msg_data)
end

function auciton_mgr_dp_check_world_auction_sell(sell_num)
     delog("---dp--check_world_auction_sell")
	 auction_mgr:dp_check_world_auction_sell_end(sell_num)
end

function auction_mgr_req_share_list_end_proc(message_data)
     delog("---dp--req_share_list_end_proc")
	 local msg_data = decode("hld.dp2s_proto.auction_req_share_list_end", message_data)
	 if msg_data == nil then
	 	return
	 end
	 auction_mgr:req_share_list_end_proc(msg_data.money_type, msg_data.money_value, msg_data.item_template_id, msg_data.data_info, msg_data.data_num, msg_data.buyer_guid)
end

function auction_mgr_get_old_item_end_proc(message_data)
     delog("---dp--get_old_item_end_proc")
	 local msg_data = decode("hld.dp2s_proto.auction_clear_old_item_end", message_data)
	 if msg_data == nil then
	 	return
	 end
	 auction_mgr:get_old_item_end_proc(msg_data.data_info, msg_data.data_num);
end

function auction_mgr_operate_selling_lock(item_guid, is_add)
	  delog("---dp--operate_selling_lock")
	  auction_mgr:operate_selling_lock(item_guid, is_add)
end

function auction_mgr_find_bid_end_proc(message_data)
      delog("---dp--find_bid_end_proc")
	 local msg_data = decode("hld.dp2s_proto.auction_find_bid_target_end", message_data)
	 if msg_data == nil then
	 	return
	 end

	 auction_mgr:find_bid_end_proc(msg_data.role_guid, msg_data.item_guid, msg_data.auction_info, msg_data.price_money_type, msg_data.price_money_value)
end

function auction_mgr_sell_end(message_data)
     delog("---dp--sell_end")
	 local msg_data = decode("hld.dp2s_proto.auction_sell_end", message_data)
	 if msg_data == nil then
	 	return
	 end
	 auction_mgr:sell_end(msg_data.info, msg_data.operate_result, msg_data.role_info, msg_data.is_add_to_log, msg_data.third_info);
end

function auction_mgr_cancel_sell_end(message_data)
      delog("---dp--cancel_sell_end")
	 local msg_data = decode("hld.dp2s_proto.auction_cancel_sell", message_data)
	 if msg_data == nil then
	 	return
	 end
	 delog("enter--cancel_sell_end")
	 auction_mgr:cancel_sell_end(msg_data.auction_info, msg_data.role_guid)
end

function auction_mgr_find_buy_target_end(message_data)
	 local msg_data = decode("hld.dp2s_proto.auction_find_buy_target_end", message_data)
	 if msg_data == nil then
	 	return
	 end

	 delog("---find--buy--", msg_data)

	 --delog(msg_data)

	 auction_mgr:find_buy_target_end(msg_data.auction_info, msg_data.role_guid, msg_data.item_guid);
end

--------------///////////////////////////////



return auction_mgr









