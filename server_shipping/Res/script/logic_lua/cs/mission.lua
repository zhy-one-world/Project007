local mission = {}


local  enum_mission_slot = 
{
     main  = 0,  --主线
     daily = 1, --日常
     against = 2, --讨伐
     grade_up = 3,  --转职
     side_0  = 4,  --支线_0
     side_1  = 5,  --支线_1
     side_2  = 6,  --支线_2
     side_3  = 7,  --支线_3
     side_4  = 8,  --支线_4
     side_5  = 9,  --支线_5
     side_6  = 10,  --支线_6
     extra_side_0 = 11,  --额外支线_0（结婚任务）
     extra_side_1 = 12,  --额外支线_1
     extra_side_2 = 13,  --额外支线_2
     extra_side_3 = 14,  --额外支线_3
     extra_side_4 = 15,  --额外支线_4
     extra_side_5 = 16,  --额外支线_5
     extra_side_6 = 17,   --额外支线_6
     extra_side_7 = 18,  --额外支线_7
     extra_side_8 = 19,  --额外支线_8
     extra_side_9 = 20,  --额外支线_9
     extra_side_10 = 21, --额外支线_10
     max = 22,
};

local enum_mission_type = 
{
	   main = 0, --主线
	   daily = 1, --日常
	   against = 2, --讨伐
	   side = 3, --支线
	   grade_up = 4, --转职任务
	   marry = 5, --夫妻任务
	   max = 6
};

local enum_mission_finish_type =
{
	normal = 0, --正常完成
	double = 1, --双倍奖励
	endall = 2, --一键完成
};

local enum_mission_operate_end_state = 
{
	system_wrong = 0, --失败
    succeed = 1, --成功
	bag_full = 2, --包满
	too_far  = 3, --距离过远
	not_enough_money = 4, --钱不够
	too_fast = 5, --操作太频繁
};


local enum_mission_inst_data =
{
	id =  0, --任务模板id
	i_type = 1, --任务类型
	slot = 2, --任务槽位
    goal1_num = 3, --任务目标1的当前数量
	goal2_num = 4, --任务目标2的当前数量
	goal3_num = 5, --任务目标3的当前数量
	goal4_num = 6, --任务目标4的当前数量
	star_level = 7, --日常任务的星级
	mission_state = 8, --任务的状态
	finish_item_id1 = 9, --奖励物品8
	finish_item_num1 = 10, --数量1 9
	finish_item_id2 = 11,  --10
	finish_item_num2 = 12,
	finish_item_id3 = 13,
	finish_item_num3 = 14,
	finish_item_id4 = 15,
	finish_item_num4 = 16,
	get_time = 17, --接受任务时间
	max      = 18,
};

function mission:init()
end



define_C[11004] = function(message_data, player_ref)
    local msg_data = decode("hld.mission_proto.mission_operate", message_data)
    if msg_data == nil then
     	return
    end

    if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
	 	return;
	end
	mission:mission_operate(player_ref,msg_data.mission_id, msg_data.mission_slot, msg_data.operation_type, msg_data.operate_npc_id);
end


function mission:mission_operate(player_ref, mission_id,mission_slot, mission_operate_type, npc_id)
	delog("------mission_operate------", mission_id, mission_slot)
	if mission_id <= 0 or mission_slot < hld.e_mission_slot_main or  mission_slot >= hld.e_mission_slot_max then
		return;
	end
	local end_state = 0;

	local mission_mgr = player_ref:get_mission_mgr()
	if mission_mgr == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end

	if mission_operate_type == hld.e_mission_operate_type_finish then

	   end_state = self:finish_mission(player_ref, mission_id, mission_slot, npc_id, enum_mission_finish_type.normal)

	elseif mission_operate_type == hld.e_mission_operate_type_refresh_star then

	   end_state = self:refresh_star(player_ref, mission_id)

	elseif mission_operate_type == hld.e_mission_operate_type_finish_all_daily then

	   end_state = self:finish_mission(player_ref, mission_id, mission_slot, npc_id, enum_mission_finish_type.endall)

	elseif mission_operate_type == hld.e_mission_operate_type_special_finish then

	   end_state = self:finish_mission(player_ref, mission_id, mission_slot, npc_id, enum_mission_finish_type.double)

	elseif mission_operate_type == hld.e_mission_operate_type_create_npc then

	   end_state = self:mission_create_npc(player_ref,mission_id)
	end

	local msg_data = 
	{
	    mission_id =  mission_id,
	    operation_type = mission_operate_type,
	    operation_end_state = end_state,
	    mission_slot = mission_slot,
	    mission_array = {},
	    daily_mission_done_num = 0,
	    against_mission_done_num = 0,
    }

    if end_state == enum_mission_operate_end_state.system_wrong or end_state == enum_mission_operate_end_state.too_fast then
        msg_data.daily_mission_done_num = player_ref:get_unit_info(hld.e_role_info_daily_done_num)
        msg_data.against_mission_done_num = player_ref:get_unit_info(hld.e_role_info_against_done_num)
        local i_start = hld.e_mission_slot_main
        local i_end =  hld.e_mission_slot_max - 1
        for i = i_start, i_end do
        	local t_single = { mission_info = {} }
        	local mission_ptr = mission_mgr:find_mission_by_index(mission_slot)
        	for j = enum_mission_inst_data.id , enum_mission_inst_data.max - 1 do
        		 table.insert(t_single.mission_info, mission_ptr:get_inst_data(j))
        	end
        	table.insert(msg_data.mission_array, t_single)
        end
     end

     MessageMgr:send_message_to_self(player_ref, "hld.mission_proto.mission_operate_end",msg_data, 11003)

end


function mission:get_template_by_mission(mission_ptr)
	 if mission_ptr == nil then
		print("mission:get_template_by_mission mission_ptr is nullptr")
	 	return nil
	 end
	 local template_id = mission_ptr:get_inst_data(enum_mission_inst_data.id)
	 local mission_template = template_manager:get_line("MissionTemplate", template_id)
	 if mission_template == nil then
		print("mission:get_template_by_mission mission_template is nullptr template_id:", template_id)		
	 end
	 return mission_template
end


function mission:finish_mission(player_ref,  mission_id, mission_slot, npc_id, mission_finish_type)
	if mission_id <= 0 or mission_slot < hld.e_mission_slot_main or  mission_slot >= hld.e_mission_slot_max then
		print("mission:finish_mission param error mission_id:"..mission_id.." mission_slot:"..mission_slot)
		return enum_mission_operate_end_state.system_wrong;
	end
	local g_config = server_tool_mgr:get_game_config_template_ptr()
	if g_config == nil then
		print("mission:finish_mission g_config is null")
		return enum_mission_operate_end_state.system_wrong;
	end
	local mission_mgr = player_ref:get_mission_mgr()
	if mission_mgr == nil then
		print("mission:finish_mission mission_mgr is null")
		return enum_mission_operate_end_state.system_wrong;
	end
	local mission_ptr = mission_mgr:find_mission_by_index(mission_slot)
	if mission_ptr == nil then
		print("mission:finish_mission mission_ptr is null")
		return enum_mission_operate_end_state.system_wrong;
	end

	if mission_ptr:get_inst_data(enum_mission_inst_data.id) ~= mission_id then
		print("mission:finish_mission id mission_id:"..mission_id.." inst_data_id:"..mission_ptr:get_inst_data(enum_mission_inst_data.id))
		return enum_mission_operate_end_state.too_fast;
	end

    local t_line  = self:get_template_by_mission(mission_ptr);
    if t_line == nil then
		print("mission:finish_mission t_line is null")
    	return enum_mission_operate_end_state.system_wrong;
    end

    local old_mission_template_ptr = mission_ptr:get_mission_template_ptr();
    if  old_mission_template_ptr == nil then
		print("mission:finish_mission old_mission_template_ptr is null")
    	return enum_mission_operate_end_state.system_wrong;
    end

    local old_mission_id  = mission_id


    local mission_end_state =  mission_ptr:finish_mission(npc_id, mission_finish_type)

    if mission_end_state ~= enum_mission_operate_end_state.succeed then
		print("mission:finish_mission mission_end_state is error mission_end_state:"..mission_end_state)
        return mission_end_state
    end


    local show_type  =  t_line.MissionShowType

    if show_type == hld.e_mission_type_main then

        mission_ptr:init_mission_by_template(t_line.NextMissionId,hld.e_mission_type_main,  player_ref)

        if mission_id == g_config.DailyMissionNeedId then
           mission_mgr:init_daily_mission();
        elseif mission_id == g_config.AgainstMissionNeedId then
           mission_mgr:init_against_mission();
        end

        mission_mgr:refresh_side_mission(mission_id)

        local cur_mission_chapter_id = mission_mgr:get_cur_mission_chapter_id()
        local chapter_template = template_manager:get_line("MissionChapterTemplate", cur_mission_chapter_id);

        if chapter_template ~= nil then
        	if mission_id >= chapter_template.LastMissionId then
        	   mission_mgr:reset_mission_chapter(true)
        	end
        end

        local new_mission_template_ptr = mission_ptr:get_mission_template_ptr()
        mission_mgr:operate_npc_show(old_mission_template_ptr, new_mission_template_ptr)
        player_ref:get_service_goal_mgr():refresh_over_time_all()

    elseif show_type == hld.e_mission_type_daily then
    	mission_mgr:random_daily_mission(0)
    	mission_mgr:target_check(hld.e_mission_end_type_clear_daily_mission, 0, 0);
    elseif show_type == hld.e_mission_type_against then
    	mission_mgr:random_against_mission(0) 
    	mission_mgr:target_check(hld.e_mission_end_type_clear_against_mission,0, 0);
    elseif show_type == hld.e_mission_type_side then
    	mission_mgr:refresh_side_mission(mission_id)
    elseif show_type == hld.e_mission_type_grade_up then
    	mission_mgr:refresh_grade_mission() 
    elseif show_type == hld.e_mission_type_marry then
    	mission_mgr:random_marry_mission(false,0)
    end

    self:send_one_mission(player_ref, mission_ptr)

    return enum_mission_operate_end_state.succeed


end


function mission:send_one_mission(player_ref, mission_ptr)
	  if mission_ptr:get_mission_template_ptr() == nil then
	  	return 
	  end
	  msg_data = {
	     mission_info = {}
	  }
	  for j = enum_mission_inst_data.id , enum_mission_inst_data.max - 1 do
        table.insert(msg_data.mission_info, mission_ptr:get_inst_data(j))
      end
      MessageMgr:send_message_to_self(player_ref, "hld.mission_proto.mission_info",msg_data, 11001)

end

function mission:refresh_star(player_ref, mission_id)
	local g_config = server_tool_mgr:get_game_config_template_ptr()
	if g_config == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end

	local mission_mgr = player_ref:get_mission_mgr()
	if mission_mgr == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end
	local mission_ptr = mission_mgr:find_mission_by_index(hld.e_mission_slot_daily)
	if mission_ptr == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end
	if mission_ptr:get_inst_data(enum_mission_inst_data.id) ~= mission_id then
	   return enum_mission_operate_end_state.system_wrong;
	end

	local g_config = server_tool_mgr:get_game_config_template_ptr()
	if g_config == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end
	local t_DailyMissionRefreshCost = Common:StrToTable(g_config.DailyMissionRefreshCost)
	if #t_DailyMissionRefreshCost < 2 then
	  return enum_mission_operate_end_state.system_wrong
	end

	if mission_ptr:get_inst_data(enum_mission_inst_data.star_level) == g_config.DailyMissionMaxStarLevel then
	   return enum_mission_operate_end_state.system_wrong;
	end
	if (not player_ref:can_cut_money(t_DailyMissionRefreshCost[1], t_DailyMissionRefreshCost[2])) then
	   return enum_mission_operate_end_state.system_wrong;
	end
	mission_ptr:set_inst_data(enum_mission_inst_data.star_level, g_config.DailyMissionMaxStarLevel);

	self:send_one_mission(player_ref, mission_ptr)
end



function mission:mission_create_npc(player_ref, mission_id)
	local t_line = template_manager:get_line("MissionTemplate", mission_id);
	if t_line == nil then
	   return enum_mission_operate_end_state.system_wrong
	end
	local mission_type = t_line.MissionShowType;
	if (mission_type < hld.e_mission_slot_main or hld.e_mission_slot_max <= mission_type) then	
	  return e_mission_operate_end_state_system_wrong;
    end

    local mission_mgr = player_ref:get_mission_mgr()
	if mission_mgr == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end
	local mission_ptr = mission_mgr:find_mission_by_index(mission_type)
	if mission_ptr == nil then
	   return enum_mission_operate_end_state.system_wrong;
	end
	if mission_ptr:get_mission_id() ~= mission_id then
	   return enum_mission_operate_end_state.system_wrong;
	end

	mission_ptr:create_npc();

	return  enum_mission_operate_end_state.succeed;
end



return mission