local convert_class={}

function convert_class:init()
end
--功能消息入口
define_C[12403] = function(message_data, player_ref)
	local msg_data = decode("hld.convert_proto.convert_operate", message_data)
	convert_class:operate_convert(player_ref, msg_data.convert_type , msg_data.convert_num)
end

--转换入口逻辑
function convert_class:operate_convert(player_ref , convert_type, convert_num)
	--dump(convert_num)
	--dump(unit_array_index)
	--dump(convert_type)
	
	local msg = {convert_type = tonumber(convert_type),convert_num=tonumber(convert_num)}
	--dump(msg)
	if player_ref == nil then
		return
	end
	if convert_type < 0 or convert_type >= hld.e_convert_type_max then
		--dump("----2")
		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	if convert_num <= 0 then
		convert_num = 1
	end
	--dump('======22222=====')
	if 1 == convert_num and true == convert_class:is_free_convert(convert_type , player_ref) then
		convert_class:free_convert(convert_type, convert_num, msg , player_ref)
		return
	end
	--dump("----4" ,convert_class:is_free_convert(convert_type , player_ref))
	if convert_num > player_ref:get_convert_mgr():get_m_all_convert_time(convert_type) then
		msg.opereate_result = tonumber(1)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	local upgrade_template = template_manager:get_line("PlayerUpgradeTemplate", player_ref:get_unit_info(hld.e_role_info_upgrade_id))
	--dump("----3")
	--dump(upgrade_template)
	if upgrade_template == nil  then
		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	local ConvertTemplate = GetConverTemplateByType(convert_type)
	if nil == ConvertTemplate then
		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	local convert_template_CostMoneyArray =ConvertTemplate.CostMoneyArray
	local convert_template_StaticRewardMoneyArray =ConvertTemplate.StaticRewardMoneyArray
	local convert_template_DyamicRewardMoneyArray =ConvertTemplate.DyamicRewardMoneyArray
	local convert_template_AdditionalMoneyArray =ConvertTemplate.AdditionalMoneyArray
	local convert_template_RedDiamondFreeConvert = ConvertTemplate.RedDiamondFreeConvert
	--dump(convert_template_id)
	if #(convert_template_CostMoneyArray) < 2 or
		#(convert_template_StaticRewardMoneyArray) % 2 ~= 0 or
		#(convert_template_DyamicRewardMoneyArray) % 2 ~= 0 or
		#(convert_template_AdditionalMoneyArray) % 2 ~= 0 then

		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
--	dump(convert_template_StaticRewardMoneyArray)
	local static_reward_money_num = #convert_template_StaticRewardMoneyArray / 2;
	local dynamic_reward_money_num = #convert_template_DyamicRewardMoneyArray / 2;
	local cost_array_size = #convert_template_CostMoneyArray
	local cur_convert_serial_count = 0;
	msg.convert_reward = {}
	--dump("----4") 
	--dump(convert_type) 
	--dump(cost_array_size) 
	cur_convert_serial_count = player_ref:get_convert_mgr():get_m_vip_convert_time(convert_type) - player_ref:get_convert_mgr():get_m_all_convert_time(convert_type)
	--dump("----5") 
	--dump(player_ref:get_convert_mgr():get_m_vip_convert_time(convert_type))
	--dump(player_ref:get_convert_mgr():get_m_all_convert_time(convert_type))
	if cur_convert_serial_count + 1 > cost_array_size then
		--dump(cur_convert_serial_count) 
		--dump(cost_array_size) 
		msg.opereate_result = tonumber(2)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	--print("zym1")
	if #convert_template_RedDiamondFreeConvert > 0 and self:GetConverFreeNumByType(player_ref, convert_type) then
		--print("zym2")
		local get_money_type = convert_template_RedDiamondFreeConvert[1]
		local get_money_number = convert_template_RedDiamondFreeConvert[2]
		player_ref:lua_add_money_or_exp(tonumber(get_money_type), get_money_number, hld.e_server_log_add_money_convert_main_ui,0)  --int64接口使用不了添加string调用接口
		--dump("----7.111111")
		table.insert(msg.convert_reward , get_money_type)
		table.insert(msg.convert_reward , get_money_number)
		self:add_money_tip(player_ref, get_money_type, get_money_number)
		--print("zym3")
		--先设置
		local new_convert_num = self:SetConverFreeNumByType(player_ref, convert_type)
		player_ref:get_convert_mgr():set_m_all_convert_time(4 , new_convert_num)		
		local other_msg = {convert_type = 4, convert_times = tonumber(new_convert_num)}
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_info_one",other_msg, 12401)
		
		msg.convert_type = convert_type
		msg.convert_num = player_ref:get_convert_mgr():get_m_all_convert_time(convert_type)
		msg.opereate_result = tonumber(0)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_transform , convert_type , 0)
		return		
	end
	--dump("----6")
	local cost_money_id = convert_template_CostMoneyArray[1]
	local cost_money_sum = convert_template_CostMoneyArray[2 + cur_convert_serial_count]
	--dump(cur_convert_serial_count)
	--dump(cur_convert_serial_count)
	if false == player_ref:can_cut_money(tonumber(cost_money_id) , tonumber(cost_money_sum) ) then
		msg.opereate_result = tonumber(2)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	--dump("----7.0000000")
	--dump(static_reward_money_num)
	--dump(cost_money_sum)
	player_ref:cut_money(cost_money_id, cost_money_sum, hld.e_server_log_cut_money_convert_main_ui , 0 , 0)
	if convert_type == 0  or convert_type == 2 or convert_type == 3 then   --lua导表有问题，添加新判断到底是那种转换方式
		--dump(convert_type)
		for i=1,#convert_template_StaticRewardMoneyArray , 2 do
			local add_money_id = convert_template_StaticRewardMoneyArray[i]
			local add_money_value = convert_template_StaticRewardMoneyArray[i+ 1]
			player_ref:lua_add_money_or_exp(tonumber(add_money_id), add_money_value, hld.e_server_log_add_money_convert_main_ui,0)  --int64接口使用不了添加string调用接口
			--dump("----7.111111")
			table.insert(msg.convert_reward , add_money_id)
			table.insert(msg.convert_reward , add_money_value)

			self:add_money_tip(player_ref, add_money_id, add_money_value)
			--dump(add_money_id)
			--dump(add_money_value)
		end
	elseif convert_type == 1 then   --lua导表有问题，添加新判断到底是那种转换方式
		for i=1,#convert_template_DyamicRewardMoneyArray , 2 do
			local add_money_id = convert_template_DyamicRewardMoneyArray[i]
			--dump(add_money_id)
			local add_money_value = convert_template_DyamicRewardMoneyArray[i + 1]
			--dump(add_money_id)
			--dump(hld.e_money_type_exp)
			if tonumber(add_money_id)  == hld.e_money_type_exp then
				--dump("----8.1111111")
				--dump(upgrade_template_ptr)
				delog("------BaseExp------add_money_value--",upgrade_template.BaseExp,  add_money_value)
				add_money_value = upgrade_template.BaseExp * add_money_value
				--self:add_money_tip(player_ref, add_money_id, add_money_value)
				add_money_value = player_ref:lua_add_exp(add_money_value)
				table.insert(msg.convert_reward , add_money_id)
				table.insert(msg.convert_reward , add_money_value)

				msg.is_tip = 1
				msg.str_exp = add_money_value

				--self:add_money_tip(player_ref, add_money_id, add_money_value)
				--dump(add_money_id)
				--dump(add_money_value)
			end
			if tonumber(add_money_id) == hld.e_money_type_silver_bind then
			--	dump("----8.2222222")
				add_money_value = upgrade_template.BaseCoin * add_money_value;
				add_money_value = cur_convert_serial_count * add_money_value;
				player_ref:lua_add_money_or_exp(tonumber(add_money_id), add_money_value, hld.e_server_log_add_money_convert_main_ui,0)
				table.insert(msg.convert_reward , add_money_id)
				table.insert(msg.convert_reward , add_money_value)

				self:add_money_tip(player_ref, add_money_id, add_money_value)
				--dump(add_money_id)
				--dump(add_money_value)
			end
		end
	end
	--dump("----9")
	local new_convert_num = player_ref:get_convert_mgr():get_m_all_convert_time(convert_type) - convert_num
	--dump(new_convert_num)
	player_ref:get_convert_mgr():set_m_all_convert_time(convert_type , new_convert_num)
	msg.convert_type = convert_type
	msg.convert_num = player_ref:get_convert_mgr():get_m_all_convert_time(convert_type)
	msg.opereate_result = tonumber(0)
	MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_transform , convert_type , 0)
	--dump("----10")
	--dump(add_money_id)
	--dump(new_convert_num)
	--dump(msg)
end


function convert_class:add_money_tip(player_ref, in_money_type, in_money_num)
     local msg_data = {
         money_id =  in_money_type,
         money_num = in_money_num
     }
     MessageMgr:send_message_to_self(player_ref, "hld.item_proto.add_money" , msg_data, 10724)
end




function convert_class:is_free_convert(convert_type , player_ref)
	local is_free = false;
		if hld.e_convert_type_bind_siliver == convert_type then
			local time = 0
			local return_value = player_ref:get_time_data(hld.e_time_type_next_free_change_gold_time)
			if player_ref:get_time_data(hld.e_time_type_next_free_change_gold_time)  <= os.time()  then
				is_free = true;
			end
		end
		if hld.e_convert_type_bind_diamond == convert_type then
			local time = 0
			local return_value = player_ref:get_time_data(hld.e_time_type_red_convert_last_time)
			if player_ref:get_time_data(hld.e_time_type_red_convert_last_time)  <= os.time()  then
				is_free = true;
			end
		end
	return is_free;
end

function convert_class:free_convert(convert_type, convert_num, msg , player_ref)
	if player_ref == nil then
		return
	end
	local upgrade_template = template_manager:get_line("PlayerUpgradeTemplate", player_ref:get_unit_info(hld.e_role_info_upgrade_id))
	if upgrade_template == nil  then
		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	local ConvertTemplate = GetConverTemplateByType(convert_type)
	if nil  == ConvertTemplate then
		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	local convert_template_CostMoneyArray =ConvertTemplate.CostMoneyArray
	local convert_template_StaticRewardMoneyArray =ConvertTemplate.StaticRewardMoneyArray
	local convert_template_DyamicRewardMoneyArray =ConvertTemplate.DyamicRewardMoneyArray
	local convert_template_AdditionalMoneyArray =ConvertTemplate.AdditionalMoneyArray

	if #convert_template_CostMoneyArray < 2 or
		#(convert_template_StaticRewardMoneyArray) % 2 ~= 0 or
		#(convert_template_DyamicRewardMoneyArray) % 2 ~= 0 or
		#(convert_template_AdditionalMoneyArray) % 2 ~= 0 	then

		msg.opereate_result = tonumber(3)
		MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
		return
	end
	
	local static_reward_money_num = #(convert_template_StaticRewardMoneyArray) / 2;
	local dynamic_reward_money_num = #(convert_template_DyamicRewardMoneyArray) / 2;
	local cur_convert_serial_count = 0;
	cur_convert_serial_count = player_ref:get_convert_mgr():get_m_vip_convert_time(convert_type) - player_ref:get_convert_mgr():get_m_all_convert_time(convert_type) 
	msg.convert_reward ={}
	for i=1,#convert_template_StaticRewardMoneyArray,2 do
		local add_money_id = convert_template_StaticRewardMoneyArray[i]
		local add_money_value = convert_template_StaticRewardMoneyArray[i + 1]
		player_ref:lua_add_money_or_exp(tonumber(add_money_id), add_money_value, hld.e_server_log_add_money_convert_main_ui,0)
		table.insert(msg.convert_reward , tonumber(add_money_id))
		table.insert(msg.convert_reward , tonumber(add_money_value))
		self:add_money_tip(player_ref, add_money_id, add_money_value)
	end
	for i=1,#convert_template_DyamicRewardMoneyArray , 2 do
		local add_money_id = convert_template_StaticRewardMoneyArray[i]
		local add_money_value = convert_template_StaticRewardMoneyArray[i + 1]
		if add_money_id == hld.e_money_type_exp then
				add_money_value = upgrade_template.BaseExp * add_money_value
				add_money_value = cur_convert_serial_count * add_money_value
				add_money_value = player_ref:lua_add_exp(add_money_value, cur_convert_serial_count)
				table.insert(msg.get_convert_reward , add_money_id)
				table.insert(msg.get_convert_reward , add_money_value)

				self:add_money_tip(player_ref, add_money_id, add_money_value)
		end
		if add_money_id == hld.e_money_type_silver_bind then
				add_money_value = upgrade_template.BaseCoin * add_money_value;
				add_money_value = cur_convert_serial_count * add_money_value;
				player_ref:lua_add_money_or_exp(tonumber(add_money_id), add_money_value, hld.e_server_log_add_money_convert_main_ui,0)
				table.insert(msg.get_convert_reward , add_money_id)
				table.insert(msg.get_convert_reward , add_money_value)

				self:add_money_tip(player_ref, add_money_id, add_money_value)
		end
	end
	local distance_time = ConvertTemplate.FreeChangeGoldDistanceSecond
	-- local distance_time_arr = ConvertTemplate.FreeChangeGoldDistanceSecond
	-- local region_code = template_manager:get_field("RegionTemplate", 150000000, "RegionCode");
	-- if distance_time_arr ~= nil and region_code ~= nil then
		-- if #distance_time_arr / 2 > 0 and #distance_time_arr % 2 == 0 then
			-- for i = 1, #distance_time_arr, 2 do
				-- if region_code == distance_time_arr[i] then
					-- distance_time = distance_time_arr[i + 1]
					-- break;
				-- end
			-- end
		-- end
	-- end	
	local next_time = os.time() + distance_time


	if hld.e_convert_type_bind_siliver == convert_type then	
		player_ref:set_time_data(hld.e_time_type_next_free_change_gold_time, next_time)
		player_ref:send_time_one(hld.e_time_type_next_free_change_gold_time)
	end
	if hld.e_convert_type_bind_diamond == convert_type then	
		player_ref:set_time_data(hld.e_time_type_red_convert_last_time, next_time)
		player_ref:send_time_one(hld.e_time_type_red_convert_last_time)
	end

	msg.convert_type = convert_type
	msg.convert_num = player_ref:get_convert_mgr():get_m_all_convert_time(convert_type)
	msg.opereate_result = tonumber(0)

	MessageMgr:send_message_to_self(player_ref, "hld.convert_proto.convert_operate_end",msg, 12404)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_transform , convert_type , 0)
end

function GetConverTemplateByType(Type)
	local convert_template = template_manager:get_template("ConvertTemplate");
	local region_code = template_manager:get_field("RegionTemplate", 150000000, "RegionCode");
	local ConvertTemplate = nil
	for k,v in pairs(convert_template) do
		if v.ConvertType == Type and next(v.RegionArr) ~= nil then
			local RegionArrty = v.RegionArr
			for i = 1, #(RegionArrty) do
				if RegionArrty[i] == region_code then
					return v
				end
			end
		end
	end
end


function  convert_class:GetConverFreeNumByType(player_ref, Type)
	local num = player_ref:get_convert_mgr():get_m_all_convert_time(4)	
	if num < (10^Type) then
		return true
	end	
	local new_num = (num / 10^Type) % 10
	new_num = new_num - (new_num % 1)
	if new_num == 0 then
		return true
	else
		return false
	end
end

function  convert_class:SetConverFreeNumByType(player_ref, Type)
	local num = player_ref:get_convert_mgr():get_m_all_convert_time(4)
	local new_num = num + (10 ^ Type)
	return new_num
end


convert = convert_class
return convert