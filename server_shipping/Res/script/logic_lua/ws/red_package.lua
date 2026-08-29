local red_package = RequireClass("red_package") 

local max_name_size = 100

local red_package_max_receiver_num = 50;

local red_package_max_text_length =	60;


function red_package:_constructor()
	 self:clear()
end

function red_package:init( )
	delog("---red_package---metatable--init")
end


function red_package:clear( )
	self.m_base_info = {}
	self.m_receiver_info_list = {}
	self.m_receiver_count = 0
	self.m_money_type = -1
end


function red_package:set_base_info(t_base)
	 self.m_base_info = t_base
end


function red_package:get_guid()
	return self.m_base_info.red_package_guid
end

function red_package:get_sender_guid()
    return self.m_base_info.sender_guid
end

function red_package:get_template_id()
	return self.m_base_info.red_package_template_id
end

function red_package:get_sender_name( )
	return self.m_base_info.sender_name
end

function red_package:get_send_text()
	return self.m_base_info.text	-- body
end	

function red_package:get_create_time()
	return self.m_base_info.create_time
end	

function red_package:get_remain_times()
	return self.m_base_info.remain_times
end

function red_package:get_remain_num()
	return self.m_base_info.remain_money_num
end

function red_package:get_template_ptr( )
	return template_manager:get_line("RedPackageTemplate", self:get_template_id());
end


function red_package:get_money_type()
	if self.m_money_type < 0 or self.m_money_type >= hld.e_money_type_max then
		local t_line = self:get_template_ptr() 
        if t_line ~= nil and #t_line.CanGetMoneyArray >= 2 then			
			self.m_money_type = t_line.CanGetMoneyArray[1];
		end
	end
	return self.m_money_type; 
end

function red_package:get_receiver_list()
	return self.m_receiver_info_list
end

function red_package:get_complete_data()
	return { self.m_base_info, self.m_receiver_info_list}
end


function red_package:init_by_info(red_package_template_id, sender_guid, sender_vip_level, sender_name, text)
    local red_pack_template_ptr = template_manager:get_line("RedPackageTemplate", red_package_template_id);
	if red_pack_template_ptr == nil or #red_pack_template_ptr.CanGetMoneyArray < 2 then
	
		return ConstDefine.e_item_string_unkown;
	end
	if string.len(sender_name) > max_name_size then
	
		return ConstDefine.e_item_string_unkown;
	end
	if string.len(text) > red_package_max_text_length then
	
		return ConstDefine.e_item_string_unkown;
	end



	self.m_base_info.red_package_guid = hld.make_guid_lua(sender_guid)
	self.m_base_info.red_package_template_id = red_package_template_id;
	self.m_base_info.sender_guid = sender_guid;
	self.m_base_info.create_time = os.time()
	self.m_base_info.remain_times = red_pack_template_ptr.CanGetTimes;
	self.m_base_info.sender_vip_level = sender_vip_level;

	self.m_base_info.remain_money_num = red_pack_template_ptr.CanGetMoneyArray[2] - red_pack_template_ptr.CanGetTimes;
	--因为在实际 领取 红包时，会强行每人至少1块钱，所以在存储余额时，提前减掉 总人数 X 1块钱
	self.m_base_info.sender_name = sender_name

	self.m_base_info.text = text



	--不可单独设置，会在 get_XXX 函数中自动设置
	--m_red_package_template_ptr = red_pack_template_ptr;
	--m_money_type = red_pack_template_ptr->CanGetMoneyArray[0];
	local  res = ConstDefine.e_red_bag_send_success
	if red_pack_template_ptr.Type == hld.e_red_package_type_globel then
		 res = ConstDefine.e_red_bag_get_globel_success
	elseif red_pack_template_ptr.Type == hld.e_red_package_type_legion then
		 res = ConstDefine.e_red_bag_get_legion_success
    elseif red_pack_template_ptr.Type == hld.e_red_package_type_vip then
    	 res = ConstDefine.e_red_bag_get_vip_success
    end

    return res;

end

function red_package:find_empty_receiver_list_slot()
	if self.m_receiver_count >= red_package_max_receiver_num then
		return false
	end
	return true
end	


function red_package:add_receiver(receiver_info)
	 if receiver_info.receiver_guid == 0  then
		return;
	 end

	 if not self:find_empty_receiver_list_slot() then
	 	 return
	 end

	 local key = receiver_info.receiver_guid

	 self.m_receiver_info_list[key] = receiver_info

	 self.m_receiver_count = self.m_receiver_count + 1
		
end


function red_package:is_in_receiver_list(receiver_guid)
	 local  receiver_obj = self.m_receiver_info_list[receiver_guid]
	 local  res =  (receiver_obj ~= nil and true) or false
	 return res 
end


function red_package:get_random_money_num()
	if self.m_base_info.remain_times <= 0 then
	      --不能拿剩余钱数做判断，剩余钱数是可能等于0的，因为默认的1块钱并不算入 剩余钱数 中
		return -1;
    end
	--虽然默认给分了一块钱，但不能在这随机时候处理，因为 新剩余钱数 = 原剩余钱 - 随机数（此函数返回值）
	--如果此时就 +1 ，那之后的逻辑就等于用 原完全随机的数值 减掉了 完全随机出出的值+不随机的基础值（最低的那1块钱）
	---这样会造成在最后一个人时候，永远会多给出1块钱
	if self.m_base_info.remain_money_num <= 0 then
		return 0;

	elseif self.m_base_info.remain_times == 1 then
		
		return self.m_base_info.remain_money_num;
	
	else
		
		local max_num = self.m_base_info.remain_money_num // self.m_base_info.remain_times * 2;
		--最大值是2倍平均值

		local  random_value = hld.random_gen:get_random(0, max_num)
		return random_value;
	end
	return -1;
end



 	-- body
 function red_package:draw_red_package(receiver_guid, receiver_name)

	if self:is_in_receiver_list(receiver_guid) then
	
		return -1;
	end

	delog("----receiver_guid-- type---", receiver_guid)


	local guid_ptr = hld.get_guid_64(receiver_guid)

    local session = hld.get_client_session_by_guid(guid_ptr)
	if session == nil then
		return -1;
	end

	local temp_num = self:get_random_money_num();
	if temp_num < 0 then
		return -1;
	end
	delog("---------temp_num---", temp_num)
	local final_num = temp_num + 1;  --至少给1块钱，这个随机出来的数不含1块钱，直接加就行，总钱数在创建红包时就已经做过处理了
	self.m_base_info.remain_times = self.m_base_info.remain_times -1;
	self.m_base_info.remain_money_num = self.m_base_info.remain_money_num - temp_num

	if self:find_empty_receiver_list_slot() then
	
		  local name_len = string.len(receiver_name);
		  if name_len > max_name_size then
			 name_len = max_name_size;
		  end

		  local res_name = string.sub(receiver_name, 1, name_len)
		
          local t_new_info = 
          {
            red_package_guid = self:get_guid(),
            receiver_guid  = receiver_guid,
            role_name = res_name,
            money_num = final_num
          }

          self:add_receiver(t_new_info)

          self:save_receiver(receiver_guid)
    end
	return final_num;
end


function red_package:get_receiver(receiver_guid)
	 return self.m_receiver_info_list[receiver_guid]
end


function red_package:save_receiver(receiver_guid)
     local receiver_obj = self:get_receiver(receiver_guid)
     if receiver_obj == nil then
     	return;
     end
     local t_dp_data = {  }
     t_dp_data.red_info = deep_copy(receiver_obj)

     MessageMgr:ws2dp_send_message("hld.server2dp_proto.ws2dp_add_red_package_receiver_to_sql", t_dp_data,33019, 0)
end


function red_package:change_player_name_func(role_guid, role_name)
	if self.m_base_info.sender_guid == role_guid then

		
		self.m_base_info.sender_name = role_name
	end

	local recv_obj = self.m_receiver_info_list[role_guid]
	if recv_obj == nil then
        return
    end

    recv_obj.role_name = role_name
end

function red_package:get_db_proto_data()
	 local t_single = { }
	 t_single.base_info = deep_copy(self.m_base_info)
	 t_single.info_list = {}

	 for k, v in pairs(self.m_receiver_info_list) do
	 	 local tmp = deep_copy(v)
	 	 table.insert(t_single.info_list, tmp)
	 end
	 return t_single
end

function red_package:get_db_base_proto_data( )
	  local t_base = {}
      t_base.red_info = deep_copy(self.m_base_info)

      return t_base
end


function red_package:get_guid_A_B()
	local guid_ptr = hld.get_guid_64(self.m_base_info.red_package_guid)

	return guid_ptr:get_A(), guid_ptr:get_B()
end

function red_package:get_send_guid_A_B()
	local guid_ptr = hld.get_guid_64(self.m_base_info.sender_guid)

	return guid_ptr:get_A(), guid_ptr:get_B()
end


function red_package:get_client_msg_data()


	 t_client = {}
	 local r_a, r_b = self:get_guid_A_B();
	 t_client.red_pack_guid = { r_a, r_b}
	 t_client.template_id = self:get_template_id()
     local s_a, s_b = self:get_send_guid_A_B()
	 t_client.sender_guid = {s_a, s_b}
	 t_client.create_time = self:get_create_time()
	 t_client.sender_name = self:get_sender_name()
	 t_client.send_text = self:get_send_text()
	 t_client.remain_times = self:get_remain_times()
	 t_client.remain_money_num = self:get_remain_num()

	 t_client.receiver_list = {}



	 for k, v in pairs(self.m_receiver_info_list) do
	 	local t_recv = { }
	 	local guid_ptr = hld.get_guid_64(v.receiver_guid)
	 	t_recv.receiver_guid = { guid_ptr:get_A(), guid_ptr:get_B()}
	 	t_recv.receiver_name = v.role_name
	 	t_recv.money_num = v.money_num

	 	table.insert(t_client.receiver_list, t_recv)
	 end
	 return t_client
end

















return red_package



