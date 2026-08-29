local talent = {}

local e_talent_layer = 
{
    need_id = 1,  --需求的id<100层级>100talent_list_template_id
    need_num = 2, --需求的数量
    template_id_0 = 3, --这一层的天赋id
    template_id_1 = 4, --这一层的天赋id
    template_id_2 = 5, --这一层的天赋id
    template_id_3 = 6, --这一层的天赋id
    max = 6,
};


local  e_talent_info = 
{
    template_id = 0, --模板id
    cast_money = 1, --总共消耗多少点天赋在这个节点上
    max = 2,
};


local e_open_layer_limit = 
{
	ntype = 1,
	num = 2,
	max = 2
};

local TALENT_ID_LOW = 100;


function talent:init( )
	-- body
end


define_C[12553] = function(message_data, player_ref)
	local msg_data = decode("hld.talent_proto.level_up", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	talent:talent_level_up(player_ref, msg_data.talent_template_id,msg_data.talent_list_template_id, msg_data.layer_index)
end


define_C[12555] = function (message_data, player_ref)
	local msg_data = decode("hld.talent_proto.level_up", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	talent:talent_reset(player_ref)
end


function talent:talent_level_up(player_ref, n_talent_template_id, n_talent_list_template_id, n_layer_index)
	local talent_mgr = player_ref:get_talent_mgr()
	if talent_mgr == nil then
		return
	end
	local talent_template_ptr = template_manager:get_line("TalentTemplate", n_talent_template_id)
    if talent_template_ptr == nil then
        return 
    end
    if (talent_template_ptr.Type > 0) then
		if (player_ref:get_unit_info(hld.e_role_info_class_branch) ~= talent_template_ptr.Type) then
			return
	    end
	end
	local t_layer = self:get_data_by_layer_index(n_talent_list_template_id, n_layer_index)
	if (#t_layer < e_talent_layer.max) then
		return 
	end

	local in_data = false;


	for i = e_talent_layer.template_id_0,  e_talent_layer.template_id_3 do
		local temp_id = t_layer[i]
		local temp_talent_template_ptr = template_manager:get_line("TalentTemplate", temp_id)
		if temp_talent_template_ptr ~= nil  and talent_template_ptr.Series == temp_talent_template_ptr.Series then
			in_data = true;
			break;		
		end
	end
	if in_data == false then
		return;
	end
	if t_layer[e_talent_layer.need_id] > 0 then
		if (t_layer[e_talent_layer.need_id] < TALENT_ID_LOW) then
			local  t_need_layer = self:get_data_by_layer_index(talent_list_template_id, layer_data[e_talent_layer_need_id]);
			if (#t_layer < e_talent_layer.max) then
				self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
		        return 
	        end		
			if self:get_layer_talent_num(talent_mgr, t_need_layer) < t_layer[e_talent_layer.need_num] then
				self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
				return;
		    end
		else	
			if (self:get_talent_list_num(talent_mgr, t_layer[e_talent_layer.need_id]) < t_layer[e_talent_layer.need_num]) then
				self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
				return;	
			end
		end
	end
	local can_open_id = talent_template_ptr.OpenTalentId;
	if (can_open_id > 0) then
		local open_talent_template_ptr = template_manager:get_line("TalentTemplate", can_open_id);
		if (open_talent_template_ptr ~= nil) then
			local  talent_ptr = talent_mgr:get_talent_by_series(open_talent_template_ptr.Series);
			if (talent_ptr == nil) then
				self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
				return;
			end
			if (talent_ptr:get_talent_template_id() < can_open_id) then
				self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
				return;
			end		
		end
	end

	local OpenLayerLimitSize = #talent_template_ptr.OpenrLayerLimit;
	if  OpenLayerLimitSize == e_open_layer_limit.max then
		local OpenLayerLimitType = talent_template_ptr.OpenrLayerLimit[e_open_layer_limit.ntype];
		local OpenLayerLimitNum = talent_template_ptr.OpenrLayerLimit[e_open_layer_limit.num];
		if (talent_mgr:get_talent_cast(OpenLayerLimitType) < OpenLayerLimitNum) then
			self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
			return;
		end
	end

	local  talent_ptr = talent_mgr:get_talent_by_series(talent_template_ptr.Series);

	local old_data_ary = {0,0}
	local new_talent_info = {0,0}
	local cast_money = 0;
	local cast_money_type = -1;
	if (talent_ptr ~= nil) then
		if (talent_template_ptr.Level >= talent_template_ptr.LevelMax) then
			return;
		end
		if (talent_ptr:get_talent_template_id() > n_talent_template_id) then
			self:level_up_end_msg(player_ref, ConstDefine.e_item_string_can_not_use)
			return;
		end
		for i=0,e_talent_info.max -1 do
			old_data_ary[i + 1] = talent_ptr:get_data_info(i)
		end
		local next_talent_template_ptr = template_manager:get_line("TalentTemplate", n_talent_template_id + 1);
		if (next_talent_template_ptr == nil) then
			return;
		end
		if (#next_talent_template_ptr.OpenMoney >= 2) then
			
			cast_money_type = next_talent_template_ptr.OpenMoney[1];
			cast_money = next_talent_template_ptr.OpenMoney[2];
		end
		new_talent_info[e_talent_info.template_id + 1] = n_talent_template_id + 1
		new_talent_info[e_talent_info.cast_money + 1] = old_data_ary[e_talent_info.cast_money + 1] + cast_money;
	else
		if (#talent_template_ptr.OpenMoney >= 2) then
			cast_money_type = talent_template_ptr.OpenMoney[1];
			cast_money = talent_template_ptr.OpenMoney[2];
		end
		new_talent_info[e_talent_info.template_id + 1] = n_talent_template_id;
		new_talent_info[e_talent_info.cast_money + 1] = cast_money;
    end
	if (talent_template_ptr.Level > 1 and (new_talent_info[e_talent_info.template_id + 1] - old_data_ary[e_talent_info.template_id + 1]) > 1) then
		return
	end
	if player_ref:can_cut_money(cast_money_type, cast_money) == false then		
		return;
	end
	if (talent_ptr == nil) then
		talent_ptr = talent_mgr:get_talent_empty();
	end
	if (talent_ptr == nil) then
		return;
	end
	delog("----cast_money_type--cast_money----", cast_money_type, cast_money)

	player_ref:cut_money(cast_money_type, cast_money, hld.e_server_log_cut_money_talent_open, n_talent_template_id,0);

	local m_unit_array_index = player_ref:get_array_index()
	talent_ptr:set_talent_buff(m_unit_array_index, false);
	talent_ptr:clear_data();
		--talent_ptr:set_talent_data(new_talent_info);
	talent_ptr:lua_set_talent_data(new_talent_info[e_talent_info.template_id + 1], new_talent_info[e_talent_info.cast_money + 1])
	talent_ptr:set_valid_state(true);
	talent_ptr:set_talent_buff(m_unit_array_index, true);
	talent_ptr:set_talent_skill(m_unit_array_index, true);
	talent_mgr:send_talent_one(talent_ptr);
	player_ref:set_unit_gs_change(); --改变战力

	self:level_up_end_msg(player_ref, ConstDefine.e_item_string_succeed, old_data_ary[e_talent_info.template_id + 1], new_talent_info[e_talent_info.template_id + 1])
end


function talent:level_up_end_msg(player_ref, n_result,  n_old_talent_template_id, n_new_talent_template_id)
    local old = n_old_talent_template_id or 0
    local new = n_new_talent_template_id or 0
	msg_data = 
	{
		operate_result = n_result,
		old_talent_template_id = old,
		new_talent_template_id = new,
    }
    MessageMgr:send_message_to_self(player_ref, "hld.talent_proto.level_up_end",msg_data, 12554)
end


function talent:get_data_by_layer_index(n_talent_list_template_id, n_layer_index)
	delog("---get_data_by_layer_index----", n_talent_list_template_id, n_layer_index)
	local t_line = template_manager:get_line("TalentListTemplate", n_talent_list_template_id)
    if t_line == nil then
    	delog("---get_data_by_layer_index--sss--", n_talent_list_template_id, n_layer_index)
        return {}
    end
    if n_layer_index < 1 or n_layer_index > 10 then
       return {}
    end
    local key = "Layer"..n_layer_index
    return t_line[key]
end

function talent:get_layer_talent_num(talent_mgr, t_layer)
	local  talent_num = 0;
	if (#t_layer < e_talent_layer.max) then
		return talent_num
	end

	for i = e_talent_layer.template_id_0,  e_talent_layer.e_talent_layer.template_id_3 do
		local temp_id = t_layer[i]
		local talent_template_ptr = template_manager:get_line("TalentTemplate", temp_id)
		if  talent_template_ptr ~= nil then
			local talent_ptr = talent_mgr:get_talent_by_series(talent_template_ptr.Series)
			if talent_ptr ~= nil then
			   talent_num = talent_num + talent_ptr:get_data_info(e_talent_info.cast_money);
			end
		end
	end
	return talent_num;
end

function talent:get_talent_list_num( talent_mgr,  talent_list_template_id)
	local  talent_num = 0;
	local talent_list_template_ptr = template_manager:get_line("TalentListTemplate", talent_list_template_id);
	if (talent_list_template_ptr == nil) then	
		return talent_num;
	end
	for i=1,10 do
		local key = "Layer"..i;
		talent_num = talent_num + self:get_layer_talent_num(talent_mgr, talent_list_template_ptr[key])
	end
	return talent_num
end


function talent:talent_reset(player_ref)
	local talent_mgr = player_ref:get_talent_mgr()
	if talent_mgr == nil then
		return
	end
	talent_mgr:talent_reset()
end
talent_lua_mgr = talent

return talent