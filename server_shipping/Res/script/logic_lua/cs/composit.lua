local composit_class={}

function composit_class:init()
	--dump("----0------------")
end
--功能消息入口
define_C[10708] = function(message_data, player_ref)   --10707
	local msg_data = decode("hld.item_proto.item_composit", message_data)
	composit_class:operate_composit(player_ref, msg_data.composit_template_id , msg_data.first_use_locked_material , msg_data.composit_num)
end

function composit_class:operate_composit( player_ref ,composit_template_id,first_use_locked_material ,composit_num)
	if player_ref == nil then
		return
	end
	
	local composit_template_id = composit_template_id
	local first_use_locked_material = first_use_locked_material
	local temp_composit_num = composit_num
	local result_code = {}
	local item_data_array = {}
	local error_num = 0	--用于存储合成失败的数量
	for i=1,temp_composit_num do
		local num = composit_class:item_composit(player_ref,composit_template_id, first_use_locked_material, item_data_array)
		table.insert( result_code, num)
		if num == hld.e_error_code_item_composit then --判断本次合成是否失败
			error_num = error_num + 1
		end
	end
	
	welfare:get_item_send_promp_msg_to_client(player_ref,item_data_array , false)
	composit_class:send_item_composit_response(player_ref,composit_template_id, result_code)
	
	--检查合成任务  tonumber(composit_template_id), tonumber(composit_num - error_num)
	player_ref:get_mission_mgr():target_check(hld.e_mission_end_type_compound_item,tonumber(composit_template_id), tonumber(composit_num - error_num));
end

function composit_class:item_composit(player_ref, composit_template_id, first_use_locked_material, item_data_array)
	print("composit_class:item_composit composit_template_id:"..composit_template_id)
	local CompositTemplate = template_manager:get_line("CompositTemplate",composit_template_id)
	if nil == CompositTemplate then
		print("composit_class:item_composit CompositTemplate is null composit_template_id:"..composit_template_id)
		return hld.e_error_code_no_template
	end
	
	local MaterialList = CompositTemplate.MaterialList
	local CostMoney = CompositTemplate.CostMoney
	local Rate = CompositTemplate.Rate
	local CompositItemTemplateID = CompositTemplate.CompositItemTemplateID
	local noticeID = CompositTemplate.noticeID
	if nil == next(MaterialList)  then
		print("composit_class:item_composit MaterialList is null composit_template_id:"..composit_template_id)
		return hld.e_error_code_template_param
	end
	if nil == next(CostMoney)  then
		print("composit_class:item_composit CostMoney is null composit_template_id:"..composit_template_id)
		return hld.e_error_code_template_param
	end
	if "" == Rate then
		print("composit_class:item_composit Rate is null composit_template_id:"..composit_template_id)
		return hld.e_error_code_template_param
	end
	if "" == CompositItemTemplateID then
		print("composit_class:item_composit CompositItemTemplateID is null composit_template_id:"..composit_template_id)
		return hld.e_error_code_template_param
	end
	if "" == noticeID then
		print("composit_class:item_composit noticeID is null composit_template_id:"..composit_template_id)
		return hld.e_error_code_template_param
	end

	local material_list_len = #MaterialList
	local CostMoney_num = #CostMoney
	
	local empty_slot_after_composit = 0
	if material_list_len > 0 then
		if material_list_len %2 ~= 0 then
			print("composit_class:item_composit material_list_len is error composit_template_id:"..composit_template_id.." material_list_len:"..material_list_len)			
			return hld.e_error_code_template_param
		end
		local have = 0
		local need = 0
		for i=1,material_list_len , 2 do
			have = player_ref:get_item_set():count_item_by_template_id(tonumber(MaterialList[i]) , hld.e_bag_type_bag , -1)
			need = MaterialList[i + 1]
			if tonumber(have)  < tonumber(need)  then
				print("composit_class:item_composit item not enough composit_template_id:"..composit_template_id.." item_id:"..tonumber(MaterialList[i]).." need_num:"..need.." have_num:"..have)
				return hld.e_error_code_item_matirial_not_enough
			elseif have == tonumber(need)  then
				empty_slot_after_composit = empty_slot_after_composit + 1
			end
		end
	end
	
	--检查钱物是否足够
	if 2 ~= tonumber(CostMoney_num)  then
		print("composit_class:item_composit CostMoney_num is error composit_template_id:"..composit_template_id)
		return hld.e_error_code_template_param
	end
	local need_money_id = CostMoney[1]
	local need_money_num = CostMoney[2]
	if player_ref:can_cut_money(tonumber(need_money_id) , need_money_num) == false then
		print("composit_class:item_composit money is low composit_template_id:"..composit_template_id)
		return hld.e_error_code_no_money
	end
	--检查背包有没有空闲空间
	local empty_slot_count = player_ref:get_item_set():get_empty_slot_num(hld.e_bag_type_bag) + empty_slot_after_composit
	if empty_slot_count <= 0 then
		print("composit_class:item_composit bag is full composit_template_id:"..composit_template_id)
		return hld.e_error_code_item_bag_full;
	end
	
	--扣除所有需要消耗的物品
	local material_id = 0
	local material_num = 0
	local use_locked_material = 0
	local material_num_unlocked = 0
	local material_num_locked = 0
	for i=1,material_list_len , 2 do
		material_id = MaterialList[i]
		material_num = MaterialList[i + 1]
		material_num_unlocked = player_ref:get_item_set():count_item_by_template_id(material_id, hld.e_bag_type_bag, 0)
		material_num_locked = player_ref:get_item_set():count_item_by_template_id(material_id, hld.e_bag_type_bag, 1)
		if first_use_locked_material == true then
			if material_num_locked > 0 then
				use_locked_material = 1
			end
		else
			if tonumber(material_num_unlocked)  < tonumber(material_num)  then
				use_locked_material = 1;
			end
		end
		player_ref:get_item_set():cost_item_by_id_with_lock_states(hld.e_server_log_del_item_composit , 0,tonumber( material_id), tonumber(material_num),  use_locked_material)
	end
	--player_ref:cut_money(cost_money_id, cost_money_sum, hld.e_server_log_cut_money_convert_main_ui , 0 , 0)
	player_ref:cut_money(tonumber(need_money_id) ,need_money_num, hld.e_server_log_cut_money_composit, composit_template_id , 0)
	player_ref:send_money_one(tonumber(need_money_id))
	--根据概率计算物品是否能成功合成
	local first_time_do_mgr_ref = player_ref:get_first_time_do_mgr()
	if first_time_do_mgr_ref:is_first_time_do(hld.e_first_time_do_type_composit) == false then
		local success_field = Rate * 100;
		local rand_num = math.random(0, 99) 
		if rand_num > success_field then
			return hld.e_error_code_item_composit;
		end
	else
		first_time_do_mgr_ref:add_do_record(hld.e_first_time_do_type_composit)
	end
	
	--创建合成的新物品
	local new_item = {}
	local new_item_template_id = 0
	--create_item_by_template(hld.e_server_log_add_item_get_welfare, welfare_template_id,item_template_id, item_num, 1, 0, 0, 0)
	new_item =  player_ref:get_item_set():create_item_by_template(hld.e_server_log_add_item_item_composit,0,CompositItemTemplateID, 1,use_locked_material, composit_template_id , 0 ,0 )
	new_item_template_id = CompositItemTemplateID
	if new_item == nil then
		print("composit_class:item_composit create item fail composit_template_id:"..composit_template_id)
		return hld.e_error_code_item_create_item
	end
	local temp_item = new_item
	local tem_item_info = temp_item:get_item_inst_tem();
	player_ref:get_item_set():put_item_into_bag(temp_item)
	player_ref:get_achievement_mgr():item_changed(temp_item,hld.e_item_operation_synthesis)
	if next(item_data_array) == nil then
		table.insert( item_data_array, CompositItemTemplateID )
		table.insert( item_data_array, 1 )
		table.insert( item_data_array, use_locked_material )
	else
		local is_find = false
		local n = 1
		local item_array_size = #item_data_array
		for i=1, tonumber(item_array_size) , 3  do
			if item_data_array[i] == CompositItemTemplateID  then
				item_data_array[i + 1] = 1 + item_data_array[i + 1]
				is_find = true
				n = i
			end
		end
		if is_find == false then
			table.insert( item_data_array, CompositItemTemplateID )
			table.insert( item_data_array, 1 )
			table.insert( item_data_array, use_locked_material )
			
		end
	end
	--合成成功公告
	local notice_id = tonumber(noticeID)
	if notice_id > 0 then	
		local notice_info = {}
		notice_info.notice_id = notice_id
		notice_info.item_data = {}
		table.insert(notice_info.item_data, tem_item_info);
		server_tool_mgr:send_notice(player_ref, notice_info)
	end
	
	return hld.e_error_code_success
end


function composit_class:send_item_composit_response( player_ref ,item_template_id ,result_code)
	local item_composit_response = {role_guid = {} , result_code = {} }
	table.insert( item_composit_response.role_guid, player_ref:get_unit_guid_A())
	table.insert( item_composit_response.role_guid, player_ref:get_unit_guid_B())
	item_composit_response.composit_item_template_id = item_template_id
	for i=1,#result_code do
		table.insert( item_composit_response.result_code, result_code[i])
	end
	MessageMgr:send_message_to_self(player_ref, "hld.item_proto.item_composit_end",item_composit_response, 10707)
end
		
composit = composit_class
return composit