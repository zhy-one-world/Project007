local pawn_att={}
function pawn_att:init()
end
function pawn_att:apply_att_change_by_array(player_ref, att_array, is_add, add_count)
	local att_num = #att_array;
	if (att_num == 0 or math.fmod(att_num, hld.e_att_one_max) ~= 0) then
		return;
	end
	for i = 1, att_num, hld.e_att_one_max do
		local att_level = math.floor(tonumber(att_array[i + hld.e_att_one_level]) + 0.5);
		local att_id = math.floor(tonumber(att_array[i + hld.e_att_one_att_id]) + 0.5);
		local att_value = tonumber(att_array[i + hld.e_att_one_value]) * add_count;
		local att_percent = tonumber(att_array[i + hld.e_att_one_percent]) * add_count;
		local is_show = math.floor(tonumber(att_array[i + hld.e_att_one_show]) + 0.5);
		player_ref:get_pawn_att():apply_att_change_effect(att_level, att_id, att_value, att_percent, is_show, is_add);
	end
end
function pawn_att:set_excellent_att_array(player_ref, base_att_array, item_ptr, is_add, add_count)
	if nil == player_ref or nil == base_att_array or nil == item_ptr then
		return;
	end
	
	local att_num = #base_att_array;
	if (att_num == 0 or math.fmod(att_num, hld.e_att_one_max) ~= 0) then
		return;
	end

	local random_had_flag = item_ptr:get_data_info(hld.e_item_info_random_had_flag)
	local value_info_index = hld.e_item_info_random_property1
	for i = 1, att_num, hld.e_att_one_max do
		if value_info_index > hld.e_item_info_random_property6 then
			return;
		end
		local index = (i - 1) // hld.e_att_one_max
		if (random_had_flag & 1 << index) > 0 then
			local att_level = math.floor(tonumber(base_att_array[i + hld.e_att_one_level]) + 0.5);
			local att_id = math.floor(tonumber(base_att_array[i + hld.e_att_one_att_id]) + 0.5);
			local att_value = tonumber(base_att_array[i + hld.e_att_one_value]) * add_count * item_ptr:get_data_info(value_info_index);
			local att_percent = tonumber(base_att_array[i + hld.e_att_one_percent]) * add_count * item_ptr:get_data_info(value_info_index);
			local is_show = math.floor(tonumber(base_att_array[i + hld.e_att_one_show]) + 0.5);
			player_ref:get_pawn_att():apply_att_change_effect(att_level, att_id, att_value, att_percent, is_show, is_add);
			value_info_index = value_info_index + 1
		end
	end
end
pawn_att_mgr = pawn_att
return pawn_att_mgr






