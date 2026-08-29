local achievement = {}




local  e_achievement_inst_data = 
{
	id = 0, --模板id
	current_num = 1, --当前进度
	current_state = 2, --当前状态
	max = 3,
};


local  e_achievement_state = 
{
	accept = 0, --未完成
	can_finish = 1, --已完成
	done = 2, --已领取
};




function achievement:init()


end



define_C[11605] = function(message_data, player_ref)
    local msg_data = decode("hld.achievement_proto.achievement_finish", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("----enter----achievement---one------")

	achievement:set_achievement_success(player_ref, msg_data.achievement_id, msg_data.achievement_state)
end


define_C[11604] = function(message_data, player_ref)
	local msg_data = decode("hld.achievement_proto.achievement_all_finish", message_data)
	if msg_data == nil then
		return;
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end

	delog("----enter----achievement---all------")

	achievement:set_can_receive_achievement_success(player_ref)
end


function achievement:set_achievement_success(player_ref, achievement_id, achievement_state)

	    local achievement_mgr = player_ref:get_achievement_mgr()
	    if achievement_mgr == nil then
	     	return
	    end

	    delog("-----achievement_id---",achievement_id)
		local achievement_obj = achievement_mgr:find_achievement(achievement_id);
		if achievement_obj == nil then
			return;
		end

		local template_id = achievement_obj:get_inst_data(e_achievement_inst_data.id)


		local t_line = template_manager:get_line("AchievementTemplate", template_id)

		if t_line == nil then
			return;

		end
		if (achievement_state ~= e_achievement_state.can_finish) then
			return;
		end

		if achievement_obj:get_inst_data(e_achievement_inst_data.current_state) ~= e_achievement_state.can_finish then
			return;
		end

	    local money_array_size = #t_line.FinishMoney;
		if (money_array_size == 0 or money_array_size % 2 ~= 0) then		
			return ;
		end

		local t_result = {}

		for i=1, money_array_size, 2 do
			local  money_id = t_line.FinishMoney[i];
			local  money_num = t_line.FinishMoney[i + 1];
			player_ref:add_money_or_exp(money_id, money_num, hld.e_server_log_add_money_achievement, template_id);

			self:merge_item_or_money_to_three_tuples(t_result, money_id, money_num, 0)	
		end

		achievement_obj:set_inst_data(e_achievement_inst_data.current_state, e_achievement_state.done)


		welfare:get_item_send_promp_msg_to_client(player_ref, t_result,false)


		achievement_mgr:send_one_achievement(achievement_obj);

		achievement_mgr:save_achievement_log(template_id);
end


function achievement:merge_item_or_money_to_three_tuples(t_result, money_id, money_num, lock)
	  local len = #t_result
	  if len == 0 then
	  	 table.insert(t_result, money_id);
	  	 table.insert(t_result, money_num);
	  	 table.insert(t_result, lock);
	  	 return;
	  end

	  local item_template_ptr = template_manager:get_line("ItemTemplate", money_id);

	  for i=1,len, 3 do
	  	  local source_id = t_result[i + 0];
		  local source_num = t_result[i + 1];
		  local source_lock_stated = t_result[i + 2];

		  if source_id == money_id and source_lock_stated == lock then
		  	  if (item_template_ptr == nil) then
				    --找不到物品id说明是钱，直接加就行
					t_result[i + 1] = source_num + money_num;
					return;
			  else
					local max_pile_num = item_template_ptr.max_pile_num;
					if max_pile_num <= 0 then
						max_pile_num = 1;
					end
					if source_num + money_num <= max_pile_num then
						t_result[i + 1] = source_num + money_num;
						return;
					else
						local can_pile_num = max_pile_num - t_result[i + 1];
						t_result[i + 1] = max_pile_num;
						money_num = money_num - can_pile_num;
					end
				end
		    end	
	   end
	   if (money_num <= 0) then	
			return;
	   end
	   table.insert(t_result, money_id);
	   table.insert(t_result, money_num);
	   table.insert(t_result, lock);
end


function achievement:set_can_receive_achievement_success(player_ref)
	   local achievement_mgr = player_ref:get_achievement_mgr()
	   if achievement_mgr == nil then
	     	return
	   end
	   achievement_mgr:set_can_receive_achievement_success()
end


achievement_lua_mgr = achievement



return achievement



