local c2ws_msgproc = {}

function c2ws_msgproc:init()
	delog("----c2ws_msgproc--init---")
end



define_C[12104] = function(message_data, session)
     local msg_data = decode("hld.legion_proto.create_legion", message_data)
     if msg_data == nil then
     	return
     end

     if session:is_self_server() == false then
		return;
     end

	 local  auto_accept_new_member = (msg_data.auto_accept_new_member > 0 and true )  or false ;
	 local  legion_name = msg_data.legion_name;
	 local  new_legion_level = msg_data.legion_level;
	 local name_len = string.len(legion_name);
	 if name_len >= 100 then
		return;
	 end

	 delog(msg_data)
     local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();
	 legion_ws_mgr_ref:create_legion_begin(session:get_role_guid(), legion_name, name_len, new_legion_level, auto_accept_new_member);
end


define_C[12112] = function(message_data, session)
     local msg_data = decode("hld.legion_proto.leave_legion", message_data)
     if msg_data == nil then
     	return
     end

     if session:is_self_server() == false then
		return;
     end

     local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();
	 local  legion_ws_ptr = legion_ws_mgr_ref:get_unit_legion(session:get_role_guid());
	 if legion_ws_ptr == nil then
		return;
	 end

	 if legion_ws_mgr_ref:check_can_leave_legion(legion_ws_ptr, session:get_role_guid() ,nil) == false then
		return;
	 end

	 local t_client_msg  = {}

	 MessageMgr:send_to_client(session, "hld.legion_proto.leave_legion_end", t_client_msg, 12113)

	 


	 local  member_info_ptr = legion_ws_ptr:get_member(session:get_role_guid());
	 if (member_info_ptr ~= nil  and member_info_ptr.data_ary[hld.e_legion_member_info_agora_id] ~= 0) then
		legion_ws_ptr:leave_voie_channel(session:get_role_guid());
	  end

	 delog("----msg_data--goto--------")

	 legion_ws_ptr:del_member(session:get_role_guid(), session:get_third_info(), session:get_login_type());
end


define_C[12110] = function ( message_data,  session)
     local msg_data = decode("hld.legion_proto.dissolve_legion", message_data)
     if msg_data == nil then
     	return
     end

     if session:is_self_server() == false then
		return;
     end

     local legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	 if legion_ws_ptr == nil then
		
			return;
	 end
		
		-- 如果角色不是团长就不能解散军团
	 if legion_ws_ptr:is_chief(session:get_role_guid()) == false then
			return;
	 end
		
	-- 如果是城战的参战军团且城战已经开始,那么城战期间不能解散军团
	 local  city_war_mgr = hld.legion_ws_mgr:get_instance():get_legion_city_war();
	 if city_war_mgr:is_during_city_war(legion_ws_ptr:get_legion_guid()) == true then	
		  hld.legion_ws_mgr:get_instance():send_legion_error_to_session(session, hld.e_legion_error_can_not_dissolve_during_city_war);
		  return;
	 end
	 if hld.cross_server_city_war_ws_mgr:get_instance():is_in_game_time() 
	    and hld.cross_server_city_war_ws_mgr:get_instance():get_cur_legion_in_rank(legion_ws_ptr:get_legion_guid()) >= 0 then
		
		  hld.legion_ws_mgr:get_instance():send_legion_error_to_session(session, hld.e_legion_error_can_not_dissolve_during_city_war);
			return;
	 end
		
		-- 如果是战争之王且正在举办王城舞会就不能解散军团
		-- PS: 和城战进行时不同的是,舞会期间王城占领军团的非城主成员是可以退出军团的
	 if (legion_ws_ptr:get_chief_guid() == city_war_mgr:get_city_master_guid()
			and city_war_mgr:is_banquet_holding() == true) then
	
			hld.legion_ws_mgr:get_instance():send_legion_error_to_session(session, hld.e_legion_error_can_not_dissolve_during_banquet);
			return;
	  end

	  local legion_guid = legion_ws_ptr:get_legion_guid();

	  hld.legion_ws_mgr:get_instance():del_legion(legion_guid, true, session:get_role_guid(), session:get_third_info(), session:get_login_type());
end

define_C[12114] = function (message_data ,  session)
	  local msg_data = decode("hld.legion_proto.get_legion_list", message_data)
	  if msg_data == nil then
	  	return 
	  end
	  if session:is_self_server() == false then
			return;
	  end
	  local  start_index = msg_data.start_index;
	  local  base_fighting_power = msg_data.base_fighting_power;
	  local  only_get_auto_accpet_new_member_legion = msg_data.only_get_auto_accept_new_member;

	  delog("---c2ws_req_get_legion_list--")
	  delog(msg_data)

	  hld.c2ws_req_get_legion_list_logic(session, start_index, base_fighting_power, only_get_auto_accpet_new_member_legion);
end


define_C[12119] = function (message_data, session)
	  local msg_data = decode("hld.legion_proto.get_applicant_info_list", message_data)
	  if msg_data == nil then
	  	return
	  end
	  if session:is_self_server() == false then
		  return;
	  end
	  local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	  if  legion_ws_ptr == nil then
		  return;
	  end

	  delog("---c2ws_req_get_legion_applicant_list--")
	  delog(msg_data)

	  legion_ws_ptr:send_applicant_info_list(session);
end


define_C[12121] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.process_apply", message_data)
	if msg_data == nil then
	 	return
	end


	 delog("---c2ws_req_process_legion_apply--")
	 delog(msg_data)

	if session:is_self_server() == false then
		  return;
	end


	  	-- 如果要处理入团申请的角色不在军团中就不能进行这个操作
	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	  if (legion_ws_ptr == nil) then
		
			return;
	  end

   	local  op_member_job_title = legion_ws_ptr:get_job_title(session:get_role_guid());
	-- 没有任何职位的
	if (op_member_job_title <= hld.e_legion_job_title_none) then
	
		return;
	end

	local applicant_guid_ptr = hld.get_guid_64(msg_data.applicant_guid)

	 -- 如果申请者的申请信息不在申请列表中,说明这个申请者的申请已经被处理过了
	local  applicant_list = legion_ws_ptr:get_applicant_list();
	local applicant_info = applicant_list:get_applicant(applicant_guid_ptr);
	if (nil  == applicant_info) then
	
		return;
	end

	
	local applicant_session = hld.get_client_session_by_guid(applicant_guid_ptr);

	-- 根据接受或拒绝入团申请来进行不同的操作
	local  is_accept = (msg_data.is_accept > 0 and true) or false;

	if (is_accept and hld.legion_ws_mgr:get_instance():check_can_join_legion(legion_ws_ptr,applicant_guid_ptr, session)) then
	
		-- 创建军团成员信息
		local new_member_info = hld.s_legion_member_info:new_local()
	
		hld.legion_ws_mgr:get_instance():generate_player_legion_info(applicant_session, new_member_info, applicant_info);

		if (false == legion_ws_ptr:add_member(new_member_info)) then
			return;
		end
		local applicant_list_ref = legion_ws_ptr:get_applicant_list();
		applicant_list_ref:del_applicant_with_join_legion(applicant_guid_ptr);
	end

	applicant_list:del_applicant(applicant_guid_ptr); --删除放这里 上面会用到
	
	if (applicant_session ~= nil ) then

		local i_res = (is_accept and  hld.e_legion_apply_stage_is_accepted) or hld.e_legion_apply_stage_is_refused

		local t_app_msg = 
		{
		   apply_stage = i_res

	    }

	     MessageMgr:send_to_client(applicant_session, "hld.legion_proto.apply_to_join_legion_end", t_app_msg, 12117)
	end
	local i_value = (is_accept and 1) or 0
	local t_pro_msg = 
	{
	   applicant_guid = msg_data.applicant_guid,
	   is_accept = i_value

    }
	MessageMgr:send_to_client(session, "hld.legion_proto.process_apply_end", t_pro_msg, 12122)
end


define_C[12129] = function (message_data , session)
	 local msg_data = decode("hld.legion_proto.construction_level_up", message_data)
	 if msg_data == nil then
	 	return
	 end

	 delog("-----c2ws_req_legion_construction_level_up---------")


	 if session:is_self_server() == false then
		  return;
	 end
	 local  construction_index = msg_data.construction_index;
	 local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();

		-- 不在任何军团中的角色不能进行军团建筑升级操作
	 local  legion_ws_ptr = legion_ws_mgr_ref:get_unit_legion(session:get_role_guid());
	 if (nil  == legion_ws_ptr) then
 		
			return;
	 end

	 -- 只有职位至少是副军团长级别的成员才能够升级军团建筑
	  if (legion_ws_ptr:get_job_title(session:get_role_guid()) < hld.e_legion_job_title_assistant_chief) then
		
			return;
	  end

	 legion_ws_ptr:construction_level_up(construction_index);
end

define_C[12133] = function (message_data , session)
	local msg_data = decode("hld.legion_proto.donate_money", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		  return;
	end

	delog("---c2ws_req_legion_donate_money-----")
	hld.c2ws_req_legion_donate_money_logic(session, msg_data.donate_times)
end


define_C[12134] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.donate_item", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		  return;
	end

	delog("---------c2ws_req_legion_donate_item---")

	hld.c2ws_req_legion_donate_item_logic(session, msg_data.donate_item_index, msg_data.donate_times)
end


local max_legion_announcement_size = 400

define_C[12136] = function ( message_data, session)
	local msg_data = decode("hld.legion_proto.update_announcement", message_data);
	if msg_data == nil then
	 	return
	end
	if session:is_self_server() == false then
		  return;
	end

	delog("--------hld.legion_proto.update_announcement---")
	delog(msg_data)

	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil  == legion_ws_ptr) then
		return;
	end

		-- 如果角色在军团没有任何职位就不能修改军团公告
	if (legion_ws_ptr:get_job_title(session:get_role_guid()) <= hld.e_legion_job_title_none) then
		return;
	end

		-- 新公告内容的长度不能超过指定长度
	local  announcement_content = msg_data.content;
	if string.len(announcement_content) > max_legion_announcement_size then
		return;
	end
		-- 检查是否可用
	if (hld.invalid_ansi_word:is_valid_ansi_str(announcement_content) == false) then
		
		hld.legion_ws_mgr:get_instance():send_legion_error_to_session(session, hld.e_legion_error_error_legion_name_illegal);
		return;
	end
	if hld.invalid_ansi_word:include_invalid_ansi_str(announcement_content) then
		hld.legion_ws_mgr:get_instance():send_legion_error_to_session(session, faithe_legion_error_error_legion_name_illegal);
		return;
	end

	delog("--write--sucess")
	legion_ws_ptr:set_announcement(announcement_content, string.len(announcement_content), true, true, session:get_role_guid(), session:get_third_info(), session:get_login_type());
	legion_ws_ptr:send_legion_announcement();

end

define_C[12138] = function (message_data, session)
	 local msg_data = decode("hld.legion_proto.get_legion_event_list", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if session:is_self_server() == false then
		  return;
	 end

	 delog("--hld.legion_proto.get_legion_event_list--")

	 hld.c2ws_req_get_event_list_logic(session, msg_data.from_happen_time)

end


define_C[12140] = function (message_data, session)
     local msg_data = decode("hld.legion_proto.kickout_legion_member", message_data)
     if msg_data == nil then
     	return
     end


     delog("--hld.legion_proto.kickout_legion_member--")

     if session:is_self_server() == false then
		  return;
	 end
     local  leaver_guid_ptr = hld.get_guid_64(msg_data.leaver_guid);

	 local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	 if (nil == legion_ws_ptr) then
		
			return;
	 end

	 if (false == hld.legion_ws_mgr:get_instance():check_can_leave_legion(legion_ws_ptr, leaver_guid_ptr, session)) then
		return;
	 end

	--如果被踢出的成员当前在线就通他已经被T了
	 local  leaver_session = hld.get_client_session_by_guid(leaver_guid_ptr); 
	 if  leaver_session ~= nil then

	 	local  t_leaver_msg = 
	 	{

	 	    leaver_guid = msg_data.leaver_guid
	    }

	    MessageMgr:send_to_client(leaver_session, "hld.legion_proto.kickout_legion_member_end", t_leaver_msg, 12141)


	    local t_test 


	    delog(t_leaver_msg)

	
	 end

	 legion_ws_ptr:del_member(leaver_guid_ptr, session:get_third_info(), session:get_login_type());

end


define_C[12142] = function (message_data, session)
	 local msg_data = decode("hld.legion_proto.appoint_job_title", message_data)
	 if msg_data == nil then
	 	return
	 end

	 delog("---hld.legion_proto.appoint_job_title--")

	 if session:is_self_server() == false then
		  return;
	 end

	--如果发起任命请求的角色没有在任何军团中就不能进行任命操作
	 local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();

	 local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	 if (nil == legion_ws_ptr) then
		return;
	 end

	  --只有军团长才能进行任命操作
	  if (legion_ws_ptr:get_job_title(session:get_role_guid()) ~= hld.e_legion_job_title_chief) then
		return;
	  end


	  local  member_guid_ptr = hld.get_guid_64(msg_data.member_guid)


		--guid_64 member_guid(req.member_guid());
	  if (member_guid_ptr:is_valid() == false or msg_data.member_guid == legion_ws_ptr:get_chief_guid():get_AB()) then
		
			return;
	  end
	  local  job_title = msg_data.job_title;
		
	 local ret = legion_ws_ptr:appoint(member_guid_ptr, job_title, session:get_third_info(), session:get_login_type());
	 if ret == 1 then

			--给军团长发送一条委任结果反馈
			local t_client_msg = 
			{
			   result = ret,
			   job_title = job_title,
			   is_to_chief = true,
		    }

		    MessageMgr:send_to_client(session, "hld.legion_proto.appoint_job_title_end", t_client_msg, 12143)


			-- 给被委任的成员发送一条被指派的结果反馈
			local  appoint_session = hld.get_client_session_by_guid(member_guid_ptr);
			if (appoint_session ~= nil) then
			
				t_client_msg.is_to_chief = false
				MessageMgr:send_to_client(appoint_session, "hld.legion_proto.appoint_job_title_end", t_client_msg, 12143)
			end
	  elseif ret == -1 then  --要委任的职位不存在
	  elseif ret == -2 then  --要委任的角色不在军团中
	  elseif ret == -3 then   --要委托的职位已达到上限
             legion_ws_mgr_ref:send_legion_error_to_session(session, hld.e_legion_error_job_title_num_max);
      else                --其他错误   
      end
end


define_C[12148] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.get_legion_boss_award", message_data)
	if msg_data == nil then
		return
	end

	 if session:is_self_server() == false then
		  return;
	 end

	 delog("--hld.legion_proto.get_legion_boss_award-")

	 hld.c2ws_req_get_legion_boss_award_logic(session)
end


define_C[12150] = function (message_data, session)
    local msg_data = decode("hld.legion_proto_get_banquet_info", message_data)
    if msg_data == nil then
    	return
    end

	 if session:is_self_server() == false then
		  return;
	 end

	 hld.c2ws_req_get_banquet_info_logic(session)
end


define_C[12152] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.request_hold_banquet", message_data)
    if msg_data == nil then
    	return
    end

	if session:is_self_server() == false then
		  return;
	end
	hld.c2ws_req_request_hold_banquet_logic(session, msg_data.banquet_level)
end

define_C[12154] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.take_part_in_banquet", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		  return;
	end

	local  legion_city_war = hld.legion_ws_mgr:get_instance():get_legion_city_war();
	if (legion_city_war:is_banquet_holding() == false) then
		
			return;
	end

	legion_city_war:take_part_in_banquet(session);
end


define_C[12156] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.get_city_war_info", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		  return;
	end

	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil  == legion_ws_ptr) then
		return;
	end
	if hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_legion_territory_war) == 0 then		
		hld.city_war_territory_mgr:get_instance():req_cross_server_legion_city_war_info(session:get_role_guid());
		return;
	end

	local city_war_mgr = hld.legion_ws_mgr:get_instance():get_legion_city_war();

	--delog(msg_data)
	city_war_mgr:send_city_war_info_all_msg(session);

end


define_C[12158] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.apply_city_war_bid", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		  return;
	end


	local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();

		-- 如果申请参加城战的角色没有加入任何军团就不能申请
	local  legion_ws_ptr = legion_ws_mgr_ref:get_unit_legion(session:get_role_guid());
	if (nil  == legion_ws_ptr) then	
		return;
	end
	local  legion_guid = legion_ws_ptr:get_legion_guid();

		--如果申请参加城战的角色不是军团长或者副军团长就不能申请
	if legion_ws_ptr:is_chief(session:get_role_guid()) == false and false == legion_ws_ptr:is_assistant_chief(session:get_role_guid()) then
		return;
	end
	local  terr_id = msg_data.terr_id;
    local  in_terr_idx = msg_data.in_terr_idx;
		
	if not legion_ws_mgr_ref:get_legion_city_war():is_during_bid_time(terr_id) then
		return;
	end

	if (false == legion_ws_ptr:is_can_bid()) then
		return;
	end

	legion_ws_ptr:set_next_bid_time();
		--如果不是gate服且开启了跨服城战
	if hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_legion_territory_war) == 0 then 	
			--像gate请求验
		hld.city_war_territory_mgr:get_instance():req_cross_server_apply_city_war_bid(legion_guid,terr_id, in_terr_idx, session:get_role_guid());
		return;
	end

	local  ret = hld.city_war_territory_mgr:get_instance():bid_one_territory(legion_guid, terr_id, in_terr_idx, session:get_role_guid());

	local t_client_msg = 
	{
	   result = ret,

    }
    MessageMgr:send_to_client(session, "hld.legion_proto.apply_city_war_bid_end", t_client_msg, 12159)
end


define_C[12160] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.entry_city_war_map", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		  return;
	end

	hld.c2ws_req_enter_city_war_map_logic(session, msg_data.terr_id)
end


define_C[12161] = function (message_data, session)
     local msg_data = decode("hld.legion_proto.city_war_operate", message_data)
     if msg_data == nil then
     	return
     end

     if session:is_self_server() == false then
		  return;
	 end

	 hld.c2ws_req_city_war_operate_logic(session, msg_data.operate_type, msg_data.prop_npc_guid, msg_data.terr_id);

end


define_C[12166] = function (message_data, session)
    local msg_data = decode("hld.legion_proto.set_auto_accept_new_member_setting", message_data)
    if msg_data == nil then
    	return
    end
    if session:is_self_server() == false then
		  return;
	end

	local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();
	local  legion_ws_ptr = legion_ws_mgr_ref:get_unit_legion(session:get_role_guid());
	if (nil == legion_ws_ptr) then
		return;
	end
		
		-- 只有军团长才可以设置自动收人的配置
	if (legion_ws_ptr:get_job_title(session:get_role_guid()) ~= hld.e_legion_job_title_chief) then
		return;
	end

	local  is_auto_accept = msg_data.is_auto_accept;
	if  is_auto_accept == true then
		
		legion_ws_ptr:set_legion_info(hld.ELegionInfo_auto_accept_new_member, 1);
		legion_ws_mgr_ref:add_into_auto_accept_new_member_ordered_legion_list(legion_ws_ptr:get_legion_gs_value(), legion_ws_ptr:get_legion_guid());
		
	else
		
		legion_ws_ptr:set_legion_info(hld.ELegionInfo_auto_accept_new_member, 0);
		legion_ws_mgr_ref:del_from_auto_accept_new_member_ordered_legion_list(legion_ws_ptr:get_legion_guid());
	end

	legion_ws_ptr:send_legion_attr_one(hld.ELegionInfo_auto_accept_new_member);

end


define_C[12170] = function (message_data, session)
    local msg_data = decode("hld.legion_proto.get_city_master_info", message_data)
    if msg_data == nil then
    	return
    end

    if session:is_self_server() == false then
		  return;
	end

	local city_war_mgr = hld.legion_ws_mgr:get_instance():get_legion_city_war();
	local city_master_guid = city_war_mgr:get_city_master_guid():get_AB();
	local occupy_legion_guid = city_war_mgr:get_overlord_legion():get_AB();

	local t_client_msg = 
	{
	   master_guid = city_master_guid,
	   master_name = city_war_mgr:get_city_master_name(),
	   legion_guid = occupy_legion_guid,
	   legion_name = city_war_mgr:get_overlord_legion_name(),

    }


    delog(t_client_msg)


	MessageMgr:send_to_client(session, "hld.legion_proto.get_city_master_info_end", t_client_msg, 12171)
	
end

define_C[12174] = function (message_data, session)
    local msg_data = decode("hld.legion_proto.get_legion_rank", message_data)
    if msg_data == nil then
    	return 
    end
    if session:is_self_server() == false then
		 return;
	end

	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil  == legion_ws_ptr) then
		
			return;
	end



	local  legion_rank = legion_ws_ptr:get_legion_rank();

	local t_client_msg = 
	{
	    rank = legion_rank

    }
    delog(t_client_msg)
    MessageMgr:send_to_client(session, "hld.legion_proto.get_legion_rank_end", t_client_msg, 12175)
end


define_C[12179] = function (message_data, session)
	 local msg_data = decode("hld.legion_proto.add_fuel_to_fire", message_data)
	 if msg_data == nil then
	 	return 
	 end

	 if session:is_self_server() == false then
		 return;
	 end

	 hld.c2ws_req_add_fuel_to_fire_logic(session);

end


define_C[12181] = function ( message_data, session)
	 local msg_data = decode("hld.legion_proto.recruit_in_world", message_data)
	 if msg_data == nil then
	 	return 
	 end

	 if session:is_self_server() == false then
		 return;
	 end

	 hld.c2ws_req_send_legion_recruit_in_world_logic(session, msg_data.is_rm_cd);

end


define_C[12151] = function (message_data, session)
     local msg_data = decode("hld.legion_proto.req_territory_info", message_data)
     if msg_data == nil then
     	return
     end
     if session:is_self_server() == false then
		 return;
	 end

	 local  legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();

		--如果申请参加城战的角色没有加入任何军团就不能申请
	 local legion_ws_ptr = legion_ws_mgr_ref:get_unit_legion(session:get_role_guid());
	 if (nil  == legion_ws_ptr) then
		
			return;
	end
    local  legion_guid = legion_ws_ptr:get_legion_guid();

	local  terr_id = msg_data.terr_id;
		
	legion_ws_mgr_ref:get_legion_city_war():send_territory_info_one(session, terr_id);

end


define_C[12053] = function (message_data, session) -- c2ws_req_get_city_war_legion_rank_info
	 local msg_data = decode("hld.legion_proto.req_territory_info", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if session:is_self_server() == false then
		 return;
	 end


	 hld.c2ws_req_get_city_war_legion_rank_info_logic(session)

end

local cross_server_territory_city_max = 7

define_C[12055] = function (message_data, session)
	 local msg_data = decode("hld.legion_proto.set_territory_prior_maintain", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if session:is_self_server() == false then
		 return;
	 end

	 local legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();

	 local  legion_ws_ptr = hld.legion_ws_mgr_ref:get_unit_legion(session:get_role_guid());
	 if (nil == legion_ws_ptr) then
		
			return;
	 end

	 if (legion_ws_ptr:is_chief(session:get_role_guid()) == false) then
		
			return;
	 end
	 if hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_legion_territory_war) == 0 then
		
		local  array_length = #msg_data.terr_ids;
		if  array_length > cross_server_territory_city_max then
			array_length = cross_server_territory_city_max;
		end
		local  territoryIds = {};

		for i=1,array_length do
			table.insert(territoryIds, msg_data.terr_ids[i])
		end

			--像gate请求验证
		hld.city_war_territory_mgr:get_instance():req_cross_server_territory_prior_maintain(legion_ws_ptr:get_legion_guid(), territoryIds, session:get_role_guid(), array_length);
		return;
	end
		
	local t_client_msg =
	{
	   terr_ids = {},
	   is_priors = {},

    }

	local  legion_guid = legion_ws_ptr:get_legion_guid();

	for i=1,#msg_data.terr_ids do
		local terr_id = msg_data.terr_ids[i]
		local prior_ret = hld.city_war_territory_mgr:get_instance():set_is_prior_maintain(legion_guid, terr_id);
		table.insert(t_client_msg.terr_ids, terr_id)
		table.insert(t_client_msg.is_priors, prior_ret)
	end

	MessageMgr:send_to_client(session, "hld.legion_proto.set_territory_prior_maintain_end", t_client_msg, 12056)

end


define_C[12060] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.draw_bonfire_daily_award", message_data)
	if msg_data == nil then
		return
	end

	if session:is_self_server() == false then
		 return;
	end

	hld.c2ws_req_draw_bonfire_daily_reward_logic(session);

end


define_C[12183] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.legion_call_operate", message_data)
	if msg_data == nil then
		return 
	end
	if session:is_self_server() == false then
		 return;
	end

	hld.c2ws_req_legion_call_operate_lua(session, message_data, string.len(message_data));
end

define_C[12061] = function ( message_data, session)
	local msg_data = decode("hld.legion_proto.level_up_legion_skill", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		 return;
	end
	local guid_ptr = hld.get_guid_64(msg_data.role_guid)
	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(guid_ptr);
	if (nil == legion_ws_ptr) then	
		return;
	end
	legion_ws_ptr:level_up_legion_skill(guid_ptr, msg_data.skill_id);

end


define_C[12191] = function (message_data, session)
    local msg_data = decode("hld.game_proto.transfer_to_pk_king_map", session)
    if msg_data == nil then
    	return
    end
    if session:is_self_server() == false then
		 return;
	end

	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil == legion_ws_ptr) then
		return;
	end
	legion_ws_ptr:join_legion_station_map(session:get_role_guid());
end

define_C[12194] = function (message_data, session)
    local msg_data = decode("hld.legion_proto.get_legion_bonus_info", message_data)
    if msg_data == nil then
    	return
    end
    if session:is_self_server() == false then
		 return;
	end

	hld.c2ws_req_get_bonus_rewards_lua(session, message_data, string.len(message_data));

end


define_C[12199] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.send_stimulate_bonus", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false then
		 return;
	end

	hld.c2ws_req_stimulate_bonus_lua(session, message_data, string.len(message_data));

end


define_C[12066] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.get_legion_answer_question_info", message_data)
	if msg_data == nil then
		return
	end

	hld.legion_ws_answer:get_instance():get_legion_question_info(session:get_role_guid());
end



define_C[12069] = function (message_data, session)
	local msg_data = decode("hld.legion_proto_req_legion_warehouse_item_info", message_data)
	if msg_data == nil then
		return
	end

	if session:is_self_server() == false then
		 return;
	end
	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil == legion_ws_ptr) then
		return;
	end

	legion_ws_ptr:get_warehouse():send_item_list_to_client(session:get_role_guid());

end


define_C[12073] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.req_legion_warehouse_log", message_data)
	if msg_data == nil then
		return
	end

	if session:is_self_server() == false then
		 return;
	end
	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil == legion_ws_ptr) then
		return;
	end

	legion_ws_ptr:get_warehouse():send_log_list_to_client(session:get_role_guid());

end


define_C[12075] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.req_change_legion_name", message_data)
	if msg_data == nil then
		return
	end

    local t_client_msg = 
    {
       ret = 0,
    } 
	if session:is_self_server() == false then
		 t_client_msg.ret = hld.e_legion_name_error_cross_server
		 MessageMgr:send_to_client(session, "hld.legion_proto.req_change_legion_name_end" ,t_client_msg, 12076)
		 return;
	end
	local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	if (nil == legion_ws_ptr) then
		
		 t_client_msg.ret = hld.e_legion_name_error_player_not_legion
		 MessageMgr:send_to_client(session, "hld.legion_proto.req_change_legion_name_end" ,t_client_msg, 12076)
		return;
	end

	local  legion_name = msg_data.legion_name;
	legion_ws_ptr:change_legion_name(session:get_role_guid(), legion_name,  string.len(legion_name));
end


define_C[12077] = function (message_data, session)
	 local msg_data = decode("hld.legion_proto.req_confirm_change_legion_name", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if session:is_self_server() == false then
		 return;
	 end
	 local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(session:get_role_guid());
	 if (nil == legion_ws_ptr) then
		return;
	 end
	 if msg_data.confirm_type then
		 local  legion_name = msg_data.legion_name;
		 legion_ws_ptr:confirm_change_legion_name(session:get_role_guid(), legion_name, string.len(legion_name));
	 end
end


define_C[12079] = function (message_data, session)
     local msg_data = decode("hld.legion_proto.join_cross_city_war", message_data)
     if msg_data == nil then
     	return
     end

     hld.c2ws_req_join_cross_city_war_lua(session, message_data, string.len(message_data))

end


define_C[12081] = function (message_data, session)
    local msg_data = decode("hld.legion_proto.get_legion_answer_rank", message_data)
    if msg_data == nil then
    	return
    end

    hld.legion_ws_answer:get_instance():send_legion_answer_rank_list(session:get_role_guid());
	-- body
end

------------------------------------------------------------------------------------------------
                                  -- store---
-------------------------------------------------------------------------------------------------

define_C[11204] = function (message_data, session)
     local msg_data = decode("hld.goods_proto.goods_operate", message_data)
     if msg_data == nil then
     	return
     end

     hld.c2ws_req_goods_operate_lua(session, message_data, string.len(message_data))

end

------------------------------------------------------------------------------------------------------
                                -- team --
---------------------------------------------------------------------------------------------------------

define_C[11709] = function (message_data, session)   --c2ws_req_let_role_join_team
	
end





define_C[11712] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.leave_team", message_data)
	 if msg_data == nil then
	 	return
	 end

	 local  team_guid_ptr =  hld.get_guid_64(msg_data.team_guid);
	 local  team_ws_ptr = hld.team_ws_mgr:get_instance():get_team(team_guid_ptr);
	 if (nil  == team_ws_ptr) then
		return;
	 end

	 local  member_info_ptr = team_ws_ptr:get_member(session:get_role_guid());
	 if member_info_ptr ~= nil and member_info_ptr.data_ary[hld.ETeamMemberInfo_agora_id] ~= 0 then
		 team_ws_ptr:leave_voie_channel(session:get_role_guid());
	 end
	 team_ws_ptr:del_member(session:get_role_guid());

end

define_C[11716] = function (message_data, session)
     local msg_data = decode("hld.team_proto.kickout_team_member", message_data)
     if msg_data == nil then
     	return
     end

     local  team_guid_ptr = hld.get_guid_64(msg_data.team_guid);
	 if (team_guid_ptr:is_valid() == false) then
		 return;
	 end
	 local  team_ws_ptr = hld.team_ws_mgr:get_instance():get_team(team_guid_ptr);
	 if (nil == team_ws_ptr) then
		return;
	 end

		--如果当前要发起T人请求的角色不是队长就不能T人
	 if team_ws_ptr:get_captain_guid():get_AB() ~= session:get_role_guid():get_AB() then
			return;
	 end

	 local  leaver_guid_ptr = hld.get_guid_64(msg_data.leaver_guid)
	 if leaver_guid_ptr:is_valid() == false then
		return;
	 end
		--如果要T的人处于多人副本中，则不允许踢出副本
	 local  leave_player_session = hld.get_client_session_by_guid(leaver_guid_ptr);
	 if  leave_player_session ~= nil then
		
			local map_guid_ptr = leave_player_session:get_map_guid();

			local map_template_id = hld.cs_map_system:get_map_template_id(map_guid_ptr);

			local t_line = template_manager:get_line("MapTemplate", map_template_id)

			if (t_line ~= nil) then
				if (t_line.Order == hld.e_map_order_type_multiplayer_raid) then	
					session:send_notice("90096105");
					return;
				end
			end
	 end
	 if team_ws_ptr:del_member(leaver_guid_ptr) == true then

			local  leaver_session = hld.get_client_session_by_guid(leaver_guid_ptr);
			if (leaver_session ~= nil ) then

				local t_client_msg = {}

				MessageMgr:send_to_client(leaver_session, "hld.team_proto.kickout_team_member_end", t_client_msg, 11717)
			end
	 end
end


define_C[11726] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.set_captain", message_data)
	 if msg_data == nil then
	 	return
	 end
	 local  team_guid_ptr = hld.get_guid_64(msg_data.team_guid);
	 local team_ws_ptr = hld.team_ws_mgr:get_instance():get_team(team_guid_ptr);
	 if  team_ws_ptr == nil then
		
			return;
	 end

		
	 if team_ws_ptr:get_captain_guid():get_AB() ~= session:get_role_guid():get_AB() then
		return;
	 end

	 local  new_captain_guid_ptr = hld.get_guid_64(msg_data.new_captain_guid);
	 team_ws_ptr:set_captain(new_captain_guid_ptr);
end


define_C[11723] = function (message_data, session)
	local msg_data = decode("hld.team_proto.invite_player", message_data)
	if msg_data == nil then
		return
	end

	hld.c2ws_req_invite_player_join_team_lua(session, message_data, string.len(message_data));

	--[[local  team_guid_ptr = hld.get_guid_64(msg_data.team_guid);
    local  team_ws_mgr_ref = hld.team_ws_mgr:get_instance();
	local  team_ws_ptr = team_ws_mgr_ref:get_team(team_guid_ptr);
	if  team_ws_ptr == nil then
		
			return;
	end


	local  invitee_guid_ptr = faith:get_guid_64(msg_data.invitee_guid);
	if (invitee_guid_ptr:is_valid() == false) then
		return;
	end

	local  invitee_session = hld.get_client_session_by_guid(invitee_guid_ptr);
	if  invitee_session == nil then	
		team_ws_mgr_ref:send_team_error_to_session(session, e_team_error_player_already_offline);
		return;
	end


		-- 被邀请的玩家已经在邀请者的队伍中
	if (team_ws_ptr:get_member(invitee_guid) ~= nullptr)
	{
		team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_player_already_join_team);
		return;
	}

	--被邀请的玩家已经在其他队伍中
	if team_ws_mgr_ref:get_unit_team(invitee_guid) ~= nullptr then
	
		team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_player_already_join_other_team);
		return;
	end

	--如果是队长邀请就加入邀请列表中
	if (team_ws_ptr:get_captain_guid():get_AB() == session:get_role_guid():get_AB() then
	
		team_ws_ptr:add_invite_guid(invitee_guid_ptr);
	end

	team_proto_invite_player_end invite_player_end_msg;
	team_proto_team_brief_info* team_brief_info = invite_player_end_msg.add_inviter_team_info();
	invite_player_end_msg.set_sender_guid(session->get_role_guid().server_64);
	team_ws_ptr->get_team_brief_info_pkg(team_brief_info);
	invitee_session->send_to_client(&invite_player_end_msg, e_msgindex_s2c_invite_player_end);

	team_proto_invite_player_success invite_player_success_msg;
	invite_player_success_msg.set_invitee_guid(invitee_guid.server_64);
	session->send_to_client(&invite_player_success_msg, e_msgindex_s2c_invite_player_success);]]

end


define_C[11722] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.transfer_team_to_scene", message_data)
	 if msg_data == nil then
	 	return
	 end
	 local  team_ws_mgr_ref = hld.team_ws_mgr:get_instance();

	 local  team_guid_ptr = hld.get_guid_64(msg_data.team_guid);
	 local  team_ws_ptr = team_ws_mgr_ref:get_team(team_guid_ptr);
	 if  team_ws_ptr == nil then
		return;
	 end

		-- 只有队长才能进行传送
	 if session:get_role_guid():get_AB() ~= team_ws_ptr:get_captain_guid():get_AB() then
		
			return;
	 end

	 local  ret = team_ws_ptr:transfer_all_member_to_scene(msg_data.map_template_id);
	 if ret  ~= hld.e_team_error_none then
		 team_ws_mgr_ref:send_team_error_to_session(session, ret);
	 end

end


define_C[11714] = function (message_data, session)
    local msg_data = decode("hld.team_proto.ready", message_data)
    if msg_data == nil then
    	return
    end

    hld. c2ws_req_ready_lua(session, message_data, string.len(message_data))

	-- body
end


define_C[11718] = function (message_data, session)
	local msg_data = decode("hld.team_proto.search_team_by_map_id", message_data)
	if msg_data == nil then
		return
	end

	hld.c2ws_req_search_team_list_lua(session, message_data, string.len(message_data))

end



define_C[11734] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.get_no_team_mate_list", message_data)
	 if msg_data == nil then
	 	return
	 end

	 hld.team_ws_mgr:get_instance():req_no_team_mate_list(session, msg_data.mate_type);

end

define_C[11706] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.join_team_apply", message_data)
	 if msg_data == nil then
	 	return
	 end

	 if session:get_m_is_in_assist() then
		session:send_notice("90204219");
		return;
	 end
	 delog(msg_data)
     local  join_team_way = msg_data.join_team_way;
	 local  given_guid_ptr = hld.get_guid_64(msg_data.given_guid);
	 local  captain_guid = nil;
	 local  team_ws_mgr_ref = hld.team_ws_mgr:get_instance();
	 local  team_ws_ptr = nil;
    if join_team_way == hld.e_join_team_way_by_team_guid then

			team_ws_ptr = team_ws_mgr_ref:get_team(given_guid_ptr);
			if  team_ws_ptr == nil then
				team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_team_already_dissolve);
				return;
			end
			captain_guid = team_ws_ptr:get_captain_guid();


			--通过队长的ID来获取队伍
	elseif join_team_way == hld.e_join_team_way_by_captain_guid then
			team_ws_ptr = team_ws_mgr_ref:get_unit_team(given_guid_ptr);
			if  team_ws_ptr == nil then	
				team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_target_player_not_in_team);
				return;
			end
			if team_ws_ptr:get_captain_guid():get_AB() ~= given_guid_ptr:get_AB() then
				team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_target_player_is_not_captain);
				return;
			end
			captain_guid = given_guid_ptr;
	else
		  return;
	end

		-- 申请者已经在某个队伍中
	if team_ws_mgr_ref:get_unit_team(session:get_role_guid()) ~= nil then
	
			team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_player_already_join_other_team);
			return;
    end

    if team_ws_ptr:get_team_state() == hld.e_team_state_game and is_team_type_raid_map(team_ws_ptr:get_team_type()) then
	
			team_ws_mgr_ref:send_team_error_to_session(session, hld.e_team_error_raidmulti_ingame);
			return;
	end


	team_ws_ptr:req_add_member_to_cs(session);

end


define_C[11736] = function (message_data, session)
	local msg_data = decode("hld.team_proto.get_team_list_by_aim", message_data)
	if msg_data == nil then
		return
	end

	hld.c2ws_req_get_team_list_by_aim_lua(session, message_data, string.len(message_data))
end


define_C[11738] = function (message_data, session)
	local msg_data = decode("hld.team_proto.auto_match_team", message_data)
	if msg_data == nil then
		return
	end

	local  team_type_id = msg_data.team_type_id;
	local  sub_type_id =  msg_data.team_sub_type_id;
	local  is_begin_match = msg_data.match_state;

	if team_type_id <= hld.e_team_type_none or team_type_id >= hld.e_team_type_max then
		return;
	end
	local  team_ws_mgr_ref = hld.team_ws_mgr:get_instance();
	if  team_ws_mgr_ref:is_team_type_valid(team_type_id, sub_type_id) == false then
		return;
	end

	local default_guid_ptr = hld.get_guid_64()

	local  i_match_ret = hld.e_player_match_team_none;
	if is_begin_match then

		team_ws_mgr_ref:req_condition_check_to_cs(session, hld.e_team_common_check_invoker_auto_match_team, team_type_id, sub_type_id, default_guid_ptr);
	else
		
		team_ws_mgr_ref:player_stop_auto_match_team(session:get_role_guid());
		i_match_ret = hld.e_player_match_team_none;
	end

	local t_client_msg =
	{
	   match_ret = i_match_ret

	}

	MessageMgr:send_to_client(session, "hld.team_proto.auto_match_team_end", t_client_msg, 11739)
end


define_C[11742] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.team_mem_operate", message_data)
	 if msg_data == nil then
	 	return
	 end

	 hld.c2ws_req_team_mem_operate_lua(session, message_data, string.len(message_data))

end


define_C[11746] = function (message_data, session)
	 local msg_data = decode("hld.team_proto.refused_to_join_team", message_data)
	 if msg_data == nil then
	 	return
	 end
	 if msg_data.tar_player_guid == nil or msg_data.tar_player_guid == 0 then
	 	return
	 end
	 local  tar_guid_ptr  =  hld.get_guid_64(msg_data.tar_player_guid);
	 local  tar_session = hld.get_client_session_by_guid(tar_guid_ptr);
	 if  tar_session == nil then
		return;
	 end
	 local my_64 = session:get_role_guid():get_AB()

	 local t_client_msg = 
	 {
	    tar_player_guid = my_64
	 }

	 MessageMgr:send_to_client(tar_session, "hld.team_proto.refused_to_join_team_end", t_client_msg, 11747)


end



-----------------------------------------------------------------------------------------------------------------

                                           --activity----

--------------------------------------------------------------------------------------------------------------------


define_C[10324] = function (message_data, session)
	 local msg_data = decode("hld.game_proto.get_best_record", message_data)
	 if msg_data == nil then
	 	return
	 end

	 hld.c2ws_req_get_best_record_lua(session, message_data, string.len(message_data))
end

local  pk_king_map_template_id = 71140001
define_C[10329] = function (message_data, session)
	local msg_data = decode("hld.game_proto.transfer_to_pk_king_map", message_data)
	if msg_data == nil then
	 	return
	end

	if  session:is_self_server() == false and 
	    hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity) == 0 then
		return;
	end
	local  map_temp_ptr = template_manager:get_line("MapTemplate", pk_king_map_template_id);
	if map_temp_ptr == nil then
		return;
	end
	if session:get_role_info_data(hld.e_role_info_exp_level) < map_temp_ptr.EnterLevel then
		
		session:send_notice("90090228");
		return;
    end
	
	local  pk_king_mgr_ref = hld.pk_king_mgr:get_instance();
	if  hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity) == 0  then --本服PK之王逻辑

		local  ret = pk_king_mgr_ref:is_can_join(session:get_role_guid());
		if  ret ~= hld.e_map_error_none then
		
			hld.cs_map_mgr_system:send_map_error_to_client(session, ret);
			if  ret == hld.e_map_error_pk_king_ready_not_join_time then
				session:send_notice("90305000");
			end
			return;
		end

		local  war_idx = 0;
		local  pk_map_guid_ptr = hld.get_guid_64();
		war_idx = pk_king_mgr_ref:get_valid_pkking_map(war_idx, pk_map_guid_ptr);
		if pk_map_guid.is_valid() == false then
		
			hld.cs_map_mgr_system:send_map_error_to_client(session, hld.e_map_error_init_map_err);
			return;
		end
		hld.cs_map_mgr_system:transfer_to_map(session, pk_king_map_template_id, pk_map_guid_ptr, war_idx, 0);
	
	elseif hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity) ~= 0  then--跨服PK之王逻辑

		if pk_king_mgr_ref:is_in_notice_time() then
		
			session:send_notice("90305000");
			return;
		end
		
		if pk_king_mgr_ref:is_in_ready_time() == false then
		
			hld.cs_map_mgr_system:send_map_error_to_client(session, hld.e_map_error_pk_king_ready_end);
			return;
		end
		local default_guid_ptr = hld.get_guid_64()
		hld.cross_server_pk_ws_mgr:get_instance():send_to_gate_transfer_map(0, session:get_role_guid(), hld.e_activity_type_pk_king, 0,  default_guid_ptr);
	end

end

local shui_jing_map_tempplate_id = 71160001;

define_C[10360] = function (message_data, session)
	 local msg_data = decode("hld.game_proto.transfer_to_pk_king_map", message_data)
	 if msg_data == nil then
	 	return
	 end

	 if  session:is_self_server() == false  and  hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity) == 0 then
		 return;
	 end

	local  crystal_mgr_ref = hld.crystal_fairyland_mgr:get_instance();
	if  hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity) == 0 then --本服水晶幻境
		
		local  ret = crystal_mgr_ref:is_can_join();
		if  ret ~= hld.e_map_error_none then
		
			hld.cs_map_mgr_system:send_map_error_to_client(session, ret);
			return;
		end

		local  war_idx = 0 ;
		local shui_jing_map_guid = nil;
		shui_jing_map_guid, war_idx = crystal_mgr_ref:get_valid_shui_jing_map(war_idx);
		if  shui_jing_map_guid.is_valid() == false then
		
			hld.cs_map_mgr_system:send_map_error_to_client(session, hld.e_map_error_init_map_err);
			return;
		end
		hld.cs_map_mgr_system:transfer_to_map(session, shui_jing_map_tempplate_id, shui_jing_map_guid, war_idx, 0);
	
	elseif hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_cross_begin_cross_activity) ~= 0 then --跨服水晶幻境逻辑

		if crystal_mgr_ref:is_activity_open() == false then
		
			session:send_notice("90090360");
			return;
		end
		local default_guid_ptr = hld.get_guid_64()
		hld.cross_server_pk_ws_mgr:get_instance():send_to_gate_transfer_map(0, session:get_role_guid(), hld.e_activity_type_crystal_fairyland, 0, default_guid_ptr);
	end
end


define_C[10331] = function (message_data, session)
	local msg_data = decode("hld.game_proto.transfer_to_broken_sky_map", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end

	local  map_template_id = msg_data.map_id;
	local  broken_sky_mgr_ref = hld.broken_sky_mgr:get_instance();
	local  ret = broken_sky_mgr_ref:is_can_join();
	if  ret == hld.e_map_error_none then
	    local default_guid_ptr = hld.get_guid_64()
		hld.cs_map_mgr_system:transfer_to_map(session, map_template_id, default_guid_ptr, 0, 0);
	else
		hld.cs_map_mgr_system:send_map_error_to_client(session, ret);
	end
end


define_C[10337] = function ( message_data, session)
	local msg_data = decode("hld.game_proto.get_one_map_info", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_vaild() == false then
		return;
	end

	local role_guid = session:get_role_guid();

	local  map_template_ptr = template_manager:get_line("MapTemplate", msg_data.map_template_id);
	if map_template_ptr == nil then
		return;
	end
	local  operate_map_type = map_template_ptr.Type;
    if  operate_type == hld.e_map_type_legion_boss or hld.e_map_type_legion_station then

		local legion_ws_mgr_ref = hld.legion_ws_mgr:get_instance();
		local legion_ws_ptr = legion_ws_mgr_ref:get_unit_legion(role_guid);
		if  legion_ws_ptr == nil then
			return;
		end
		legion_ws_ptr:send_legion_boss_map_info(role_guid);

	elseif operate_type == hld.e_map_type_king_of_pk then

		local pk_king_mgr_ref = hld.pk_king_mgr:get_instance();
		pk_king_mgr_ref:send_player_num_in_map_to_client(role_guid);
	end
end

define_C[14600] = function (message_data, session)
	local msg_data = decode("hld.game_proto.cross_server_pk_map_transfer", message_data)
	if msg_data == nil then
		return
	end
	local  class_type = session:get_role_info_data(hld.e_role_info_class_type);
	local  pk_mgr_ref = hld.cross_server_pk_ws_mgr:get_instance();
	if  hld.get_gate_proxy():is_gate_run() == false or
		(hld.get_gate_proxy():is_gate_run() and hld.get_world_server():is_sky_island_server()) then

		local war_idex = 0;
		local ret = pk_mgr_ref:is_can_join(session:get_role_guid());
		local map_temp_id = pk_mgr_ref:get_map_temp_id_with_class_type(class_type);
		if  ret == hld.e_map_error_none then
			
			local map_guid_ptr = hld.get_guid_64();
			war_idex = pk_mgr_ref:get_pk_king_map_guid(class_type, map_guid_ptr, war_idex);
			hld.cs_map_mgr_system:transfer_to_map(session, map_temp_id, map_guid_ptr, war_idex, 0);
		end
			
	end
	if  hld.get_gate_proxy():is_gate_run() == true then
	
		local ret = pk_mgr_ref:is_can_join_with_cross(session:get_role_guid());
		if  ret == hld.e_map_error_none then


			local default_guid_ptr = hld.get_guid_64()
		
			pk_mgr_ref:send_to_gate_transfer_map(class_type, session:get_role_guid(), hld.e_activity_type_cross_server_pk, 0, default_guid_ptr);
		end
	end

end


define_C[12902] = function (message_data, session)
	local msg_data = decode("hld.game_proto.get_world_boss_damage", message_data)
	if msg_data == nil then
		return
	end

	hld.world_boss_ws_mgr:get_instance():send_damage_list(session,msg_data.boss_id);

end

local const_harry_map_id = 71211107;	
define_C[14805] = function (message_data, session)
    local msg_data = decode("hld.game_proto.transfer_to_harry_map", message_data)
    if msg_data == nil then
    	return
    end

    if hld.get_world_server():get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry) == 0 then

		session:send_notice("90090360");
		return;
	end
	local  server_id = msg_data.server_id;
	if (hld.get_gate_proxy():is_gate_run() == true and 
		hld.get_world_server():get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry) ~= 0 and
		server_id == hld.get_world_server():get_server_id()) then
	
		local can_join = hld.cross_server_harry_ws_mgr:get_instance():is_can_join_harry_map(session);
		if not can_join then
			return;
		end
		local map_guid_ptr = hld.get_guid_64();
		local war_idex = 0;
		hld.cross_server_harry_ws_mgr:get_instance():get_valid_harry_map(map_guid_ptr, war_idex);
		if  not map_guid_ptr.is_valid() then
		
			hld.cross_server_harry_ws_mgr:get_instance():init_harry_map();
			hld.cross_server_harry_ws_mgr:get_instance():get_valid_harry_map(map_guid, war_idex);
		end
		if not map_guid_ptr.is_valid() then
		
			session:send_notice("90090579");
			return;
		end
		hld.cs_map_mgr_system:transfer_to_map(session, const_harry_map_id, map_guid_ptr, war_idex, server_id);
	
	elseif hld.get_gate_proxy():is_gate_run() == true and
	       hld.get_world_server():get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry) ~= 0 and
		   server_id ~= hld.get_world_server():get_server_id() and
		   server_id ~= hld.get_world_server():get_cross_id()  then
	
		local can_join = hld.cross_server_harry_ws_mgr:get_instance():is_can_join_harry_map(session);
		if not can_join then
		
			return;
		end
		local default_guid_ptr = hld.get_guid_64()
		hld.cross_server_pk_ws_mgr:get_instance():send_to_gate_transfer_map(0, session:get_role_guid(), hld.e_activity_type_cross_server_harry, server_id, default_guid_ptr);
	end

end


define_C[12606] = function (message_data, session)
	  local msg_data = decode("hld.welfare_proto.get_reward_activatecode", message_data)
	  if msg_data == nil then
	  	return
	  end
	  local now_time_sec = os.time();
	  local now_time_msec = now_time_sec * 1000
	  if now_time_msec <= session:get_m_activate_code_time() then
	  	  session:send_notice("90095223");
	  	  return
	  end
	  local next_time_msec =  now_time_msec + 1000;
	  session:set_m_activate_code_time(next_time_msec)
	  hld.web_client:get_instance():send_activate_code(session, msg_data.activatecode, msg_data.login_type);
end



define_C[13603] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_addfriend", message_data)
	if msg_data == nil then
		return
	end

	if  session:is_self_server() == false then
		return;
	end
	if session:is_vaild() == false then
		return;
	end
	if msg_data.guid == session:get_role_guid():get_AB() then
		return;
	end
	local guid_ptr = hld.get_guid_64(msg_data.guid)
	session:get_relation_list_mgr():add_relation(msg_data.relation_type, guid_ptr);
end


define_C[13606] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_delfriend", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end
	if  session:is_vaild() == false then
		return;
	end
	local relation_type = msg_data.friendtype;
	local guid_ptr = hld.get_guid_64(msg_data.guid)
	session:get_relation_list_mgr():del_relation(guid_ptr, relation_type);

		

	local target_session_ptr = hld.get_client_session_by_guid(guid_ptr);
	if target_session_ptr ~= nil then
		
		target_session_ptr:get_relation_list_mgr():del_relation(session:get_role_guid(), relation_type);	
	else
		session:get_relation_list_mgr():del_relation_to_dp(guid_ptr, session:get_role_guid(), relation_type);
	end

end

define_C[13601] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_friend_userinfo", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end
	if  session:is_vaild() == false then
		return;
	end
	if msg_data.sub_type ~= 0 then
		
		session:get_relation_list_mgr():find_player_to_add_friend_by_pattern(msg_data.name);

	else
		session:get_relation_list_mgr():find_player_to_add_friend();
	end
end


define_C[13628] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_syc_nearly_state", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end

	hld.c2ws_req_syc_nearly_state_lua(session, message_data, string.len(message_data))
end


define_C[13631] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_operate_other_add_req", message_data)
	if msg_data == nil then
		return
	end

	if  session:is_self_server() == false then
		return;
	end


    hld.c2ws_req_operate_other_req_lua(session, message_data, string.len(message_data))
end



define_C[13615] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_marrage", message_data)
	if msg_data == nil then
		return
	end

	if  session:is_self_server() == false then
		return;
	end

	local 	captin_guid_ptr = hld.get_guid_64(msg_data.main_guid);
	local 	sub_guid_ptr = hld.get_guid_64(msg_data.sub_guid);
	hld.marry_mgr_ws:get_instance():req_check_marry_with_sb(captin_guid_ptr, sub_guid_ptr, msg_data.marry_item);
end



define_C[13633] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_reply_choose", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end


    local role_guid_ptr  = hld.get_guid_64(msg_data.role_guid);
	if not msg_data.is_divorce then
		
		hld.marry_mgr_ws:get_instance():c2ws_reply_marry_choice(role_guid_ptr, msg_data.is_agree);
	else
		hld.marry_mgr_ws:get_instance():c2ws_reply_divorce_choice(role_guid_ptr, msg_data.is_agree, msg_data.is_force);
	end
end


define_C[13636] = function ( message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_divorce", message_data)
	if msg_data == nil then
		return
	end

	if  session:is_self_server() == false then
		return;
	end

	local	role_guid_ptr = hld.get_guid_64(msg_data.main_guid);
	local 	sub_guid_ptr =  hld.get_guid_64(msg_data.sub_guid);
	hld.marry_mgr_ws:get_instance():c2ws_req_check_divorce(role_guid_ptr, sub_guid_ptr, msg_data.is_force);
end


define_C[13637] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_marry_get_reward", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end

	local role_guid_ptr = hld.get_guid_64(msg_data.role_guid)

	hld.marry_mgr_ws:get_instance():c2ws_get_married_reward(role_guid_ptr);

end


define_C[13642] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_get_marry_mission", message_data)
	if msg_data == nil then
		return
	end

	local role_guid = session:get_role_guid();
	hld.marry_mgr_ws:get_instance():c2ws_get_marry_mission(role_guid, msg_data.is_near);
end


define_C[13644] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_confirm_receive_marry_mission", message_data)
	if msg_data == nil then
		return
	end

	hld.c2s_confirm_receive_marry_task_lua(session, message_data, string.len(message_data))
end


define_C[13645] = function (message_data, session)
	local msg_data = decode("hld.relation_proto.c2s_req_bless_one_couple", message_data)
	if msg_data == nil then
		return
	end

	if session:is_self_server() == false then
		return;
	end
	local guid_ptr = hld.get_guid_64(msg_data.target_guid_a, msg_data.target_guid_b)
	hld.marry_mgr_ws:get_instance():req_bless_one_couple(session,  guid_ptr);
end


define_C[12001] = function (message_data, session)
	local msg_data = decode("hld.ranking_proto.get_top_list", message_data)
	if msg_data == nil then
		return
	end



	local 	req_guid_ptr =  hld.get_guid_64(msg_data.role_guid); 
	hld.ranking_mgr_ws:get_instance():c2ws_get_ranking_list_func(msg_data.index, req_guid_ptr);
end



define_C[12019] = function (message_data, session)
	local msg_data = decode("hld.ranking_proto.get_my_rank_lua", message_data)
	if msg_data == nil then
		return
	end

	local  sender_guid_ptr = hld.get_guid_64(msg_data.sender_guid_a, msg_data.sender_guid_b) ;

	local A = msg_data.legion_guid_a or 0 
	local B = msg_data.legion_guid_b or 0

	local  legion_guid_ptr = hld.get_guid_64(A, B);

	hld.ranking_mgr_ws:get_instance():c2ws_get_my_rank_func(msg_data.rank_type, sender_guid_ptr, legion_guid_ptr);
	-- body
end


define_C[12010] = function (message_data, session)
	local msg_data = decode("hld.ranking_proto.get_player_ranking_value", message_data)
	if msg_data == nil then
		return
	end

	local sender_guid_ptr = hld.get_guid_64(msg_data.sender_guid)
	local target_guid_ptr = hld.get_guid_64(msg_data.target_guid)

	hld.ranking_mgr_ws:get_instance():c2ws_get_player_ranking_info_func(msg_data.ranking_type, sender_guid_ptr, target_guid_ptr);

end


define_C[12017] = function (message_data, session)
	local msg_data = decode("hld.ranking_proto.service_rank_get_first_player", message_data)
	if msg_data == nil then
		return
	end


	hld.ranking_mgr_ws:get_instance():send_service_rank_first_player_to_client_by_type(session, msg_data.service_rank_type);

end




define_C[12610] = function (message_data, session)
	local msg_data = decode("hld.welfare_proto.req_frist_kill_world_boss_prize", message_data)
	if msg_data == nil then
		return
	end


	local 	self_guid_ptr =  hld.get_guid_64(msg_data.first_killer_guid);
	hld.world_boss_ws_mgr:get_instance():req_frist_kill_world_boss_prize(self_guid_ptr, msg_data.npc_respawn_point_template_id);
end


define_C[10361] = function (message_data, session)
    local msg_data = decode("hld.game_proto.send_world_level", message_data)
    if msg_data == nil then
    	return
    end

    local t_client_msg = 
    {
      server_level = 0

    };

	t_client_msg.server_level = hld.ranking_mgr_ws:get_instance():get_srv_avg_level();

	MessageMgr:send_to_client(session, "hld.game_proto.send_world_level", t_client_msg, 10362)
end



define_C[16207] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.legion_strategy_info", message_data)
	if msg_data == nil then
		return
	end

	
	local legion_guid_ptr = hld.get_guid_64(msg_data.legion_guid);
	local self_guid_ptr = hld.get_guid_64(msg_data.role_guid);
	hld.world_boss_ws_mgr:get_instance():send_strategy_world_boss_info(legion_guid_ptr, self_guid_ptr);

end


define_C[16211] = function (message_data, session)
	local msg_data = decode("hld.legion_proto.legion_strategy_info", message_data)
	if msg_data == nil then
		return
	end


	hld.legion_guid_ptr = hld.get_guid_64(msg_data.legion_guid);
	hld.self_guid_ptr = hld.get_guid_64(msg_data.role_guid);
	hld.world_boss_ws_mgr:get_instance():send_world_boss_info(legion_guid_ptr, self_guid_ptr, msg_data.boss_level);
end



define_C[10345] = function (message_data, session)
	local msg_data = decode("hld.game_proto.gain_treasure_req_kill_record", message_data)
	if msg_data == nil then
		return
	end

	hld.gain_treasure_ws_mgr:get_instance():req_kill_record_end(session, msg_data.boss_spawn_point_template_id);

end


define_C[10347] = function (message_data, session)
	local msg_data = decode("hld.game_proto.gain_treasure_req_gain_item_record", message_data)
	if msg_data == nil  then
		return
	end

	hld.gain_treasure_ws_mgr:get_instance():req_gain_item_record(session, msg_data.record_index);

end


define_C[12226] = function (message_data, session)
	local msg_data = decode("hld.chat_proto.join_voice_channel", message_data)
	if msg_data == nil then
		return
	end


	local  role_guid = session:get_role_guid();
	local  channel_type = msg_data.channel_type;


	local t_end_msg = 
	{

	   channel_type = channel_type
	}

	MessageMgr:send_to_client(session, "hld.chat_proto.join_voice_channel_end", t_end_msg, 12227)



	if channel_type == hld.e_chat_type_legion then
		
		local legion_guid = session:get_legion_guid();
		local  legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_legion(legion_guid);
		if (legion_ws_ptr == nil ) then
			return;
		end
		legion_ws_ptr:join_voice_channel(role_guid);
	
	elseif channel_type == hld.e_chat_type_team then
		
		local team_guid = session:get_team_guid();
		local team_ws_ptr = hld.team_ws_mgr:get_instance():get_team(team_guid);
		if (team_ws_ptr == nil) then
			return;
		end
		team_ws_ptr:join_voice_channel(role_guid);
		
	elseif channel_type == hld.e_chat_type_none then
		
		local legion_ws_ptr = hld.legion_ws_mgr:get_instance():get_unit_legion(role_guid);
		if legion_ws_ptr ~= nil then
		
			legion_ws_ptr:leave_voie_channel(role_guid);
		end
		local  team_ws_ptr = hld.team_ws_mgr:get_instance():get_unit_team(role_guid);
		if team_ws_ptr ~= nil then
		
			team_ws_ptr:leave_voie_channel(role_guid);
		end
	end
end


define_C[14301] = function (message_data, session)
	local msg_data = decode("hld.cloud_shop_proto_cloud_shop_req", message_data)
	if msg_data == nil then
		return
	end

	hld.cloud_shop_mgr:get_instance():handler_client_request(session, msg_data.req_type);

end


define_C[14706] = function (message_data, session)
	local msg_data = decode("hld.time_limit_activity_proto.get_rank_type_activity_message", message_data)
	if msg_data == nil then
		return
	end

	hld.time_limit_activity_ws_mgr:get_instance():get_rank_info(msg_data.activity_type, session:get_role_guid());
end


define_C[14708] = function (message_data, session)
	local msg_data = decode("hld.time_limit_activity_proto.get_all_common_data_message", message_data)
	if msg_data == nil then
		return
	end

	hld.time_limit_activity_ws_mgr:get_instance():get_activity_common_data_to_gate(session);

	-- body
end

define_C[10510] = function (message_data, session)
	local msg_data = decode("hld.character_proto.change_player_name", message_data)
	if msg_data == nil then
		return
	end

	if session:is_self_server() == false then
		return;
	end

	hld.rename_mgr:get_instance():change_player_name(msg_data.role_name, session:get_role_guid());

end

define_C[10512] = function (message_data, session)
	local msg_data = decode("hld.character_proto.confirm_change_name", message_data)
	if msg_data == nil then
		return
	end

	hld.rename_mgr:get_instance():confirm_change_name(msg_data.confirm_type, session:get_role_guid(), msg_data.role_name);
end


define_C[15101] = function (message_data, session)
	local msg_data = decode("hld.game_proto.cross_server_world_boss_map_transfer", message_data)
	if msg_data == nil then
		return
	end

	local  map_type = msg_data.boss_map_type;
	local  world_boss_mgr_ref = hld.cross_server_world_boss_ws_mgr:get_instance();
	local  default_guid_ptr = hld.get_guid_64()
	if session:is_self_server() == true then
		
		if  hld.get_gate_proxy():is_gate_run() == true then
			local ret = world_boss_mgr_ref:is_can_join_with_cross(session:get_role_guid(), map_type);
			if  ret == hld.e_map_error_none then
				hld.cross_server_pk_ws_mgr:get_instance():send_to_gate_transfer_map(map_type, session:get_role_guid(), hld.e_activity_type_cross_server_world_boss, hld.get_world_server():get_cross_id(),default_guid_ptr);
			end
		end
		
	else
		if hld.get_world_server():is_sky_island_server() and world_boss_mgr_ref:is_in_cross_boss_map(session:get_map_guid()) then
		
			local war_idex = 0;
			local map_temp_id = world_boss_mgr_ref:get_cross_server_world_boss_map_temp_id(map_type);
			if map_temp_id == -1 then
				return;
			end
			local  map_guid_ptr = hld.get_guid_64();
			war_idex =  world_boss_mgr_ref:get_world_boss_map_guid(map_guid, war_idex, map_type);
			if not map_guid_ptr:is_valid() then
				return;
			end
			hld.cs_map_mgr_system:transfer_to_map(session, map_temp_id, map_guid_ptr, -1, hld.get_world_server():get_cross_id());
		else
			session:send_notice("90305139");
		end
	end

end


define_C[15102] = function (message_data, session)
	local msg_data = decode("hld.game_proto.get_cross_boss_damage_list", message_data)
	if msg_data == nil then
		return
	end

	local  world_boss_mgr_ref = hld.cross_server_world_boss_ws_mgr:get_instance();
	world_boss_mgr_ref:send_kill_boss_show(session);

end


define_C[15305] = function (message_data, session)
	local msg_data = decode("hld.game_proto.get_cross_boss_damage_list", message_data)
	if msg_data == nil then
		return
	end

	hld.ranking_mgr_ws:get_instance():get_oracle_trial_rank_info(session:get_role_guid());

end


define_C[15400] = function ( message_data, session)
	local msg_data = decode("hld.legion_proto.go_to_legion_station_map", message_data)
	if msg_data == nil then
		return
	end
	if  session:is_self_server() == false then
		return;
	end
	local  legion_ptr = hld.legion_ws_mgr:get_instance():get_legion(session:get_legion_guid());
	if legion_ptr == nil then
		return;
	end
	legion_ptr:join_legion_station_map(session:get_role_guid());
end


define_C[15602] = function (message_data, session)
	local msg_data = decode("hld.assist_fight_proto.get_assist_fight_info", message_data)
	if msg_data == nil then
		return
	end

	hld.assist_fight_mgr:get_instance():sync_assist_fight_info(session:get_role_guid(), true);

end

define_C[15604] = function (message_data, session)
	local msg_data = decode("hld.assist_fight_proto.req_assist_fight", message_data)
	if msg_data == nil then
		return
	end

	local assist_guid_ptr = hld.get_guid_64(msg_data.assist_fight_guid_a, msg_data.assist_fight_guid_b)


	hld.assist_fight_mgr:get_instance():req_assist_fight(session:get_role_guid(), assist_guid_ptr);
end



define_C[10533] = function(message_data, session)
	local msg_data = decode("hld.character_proto.save_appearance_info_to_id", message_data)
	if msg_data == nil then
		return
	end


	hld.appearance_info_mgr:get_instance():req_save_appearance_info_to_id(session:get_client_uid(), msg_data.appearance_info_string);

end



define_C[10535] = function (message_data, session)
	local msg_data = decode("hld.character_proto.load_appearance_info_by_id", message_data)
	if msg_data == nil then
		return
	end


	hld.appearance_info_mgr:get_instance():req_load_appearance_info_by_id(session:get_client_uid(), msg_data.appearance_info_id);

end


define_C[15905] = function (message_data, session)
   local msg_data = decode("hld.game_proto.send_need_break_line_login_end", message_data)
   if msg_data == nil then
   	 return
   end

   	local  is_login = msg_data.is_login;
	if msg_data.active_type == hld.e_activity_type_pk_king then

		hld.pk_king_mgr:get_instance():break_line_login_msg(session:get_role_guid(), is_login);
	end
	if msg_data.active_type == hld.e_activity_type_element_war then

		hld.element_war_ws_mgr:get_instance():break_line_login_msg(session:get_role_guid(), is_login);
	end

end


define_C[10559] = function (message_data, session)
	local msg_data = decode("hld.game_proto.send_to_server_special_msg", message_data)
	if msg_data == nil then
		return
	end

	local special_msg = msg_data.special_msg;

	if special_msg == "XGaJj[Vrr4-Qz6zij4tO!-mPZtEVv?NzU<+I5JfCI5i!nuTex1" then
		
		hld.get_world_server():set_server_act_hidden(true, false);
	elseif special_msg == "LKkGgMjrg-Uvksv6AYTSW?ONAl18MAx]hV>NKPHgS4JST]eDTI" then
		hld.get_world_server():set_server_act_hidden(false, false);
	end

end


define_C[16101] = function (message_data, session)
	local msg_data = decode("hld.game_proto.element_war_send_sign_up", message_data)
	if msg_data == nil then
		return
	end

	if session:is_self_server() == false or hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_element_war) == 0  then
		return;
	end
	local  team_ref = hld.team_ws_mgr:get_instance():get_team(session:get_team_guid());
	if team_ref ~= nil and team_ref:get_member_num() > 1 then
		
		hld.element_war_ws_mgr:get_instance():send_team_sign_up(session);
	
	else
		
		hld.element_war_ws_mgr:get_instance():send_single_sign_up(session);
	end

end


define_C[16105] = function (message_data, session)
	local msg_data = decode("hld.game_proto.element_war_send_close_sign_up", message_data)
	if msg_data == nil then
		return
	end
	if session:is_self_server() == false or hld.get_world_server():get_need_begin_cross_gm_common(hld.e_need_server_element_war) == 0  then
		return;
	end


	hld.element_war_ws_mgr:get_instance():send_close_sign_up(session);
end


define_C[16113] = function (message_data, session)
	local msg_data = decode("hld.game_proto.element_war_get_mission_reward", message_data)
	if msg_data == nil then
		return
	end

	if hld.world_server:getInstance():get_need_begin_cross_gm_common(hld.e_need_server_element_war) == 0 then
		return;
	end
	hld.element_war_ws_mgr:get_instance():get_mission_reward(session:get_role_guid(), msg_data.mission_index);
end


define_C[16115] = function (message_data, session)
	local msg_data = decode("hld.game_proto.element_war_request_role_info", message_data)
	if msg_data == nil then
		return
	end


	hld.element_war_ws_mgr:get_instance():send_req_element_war_role_info(session, false);

	-- body
end



























g_c2ws_msgproc = c2ws_msgproc


return c2ws_msgproc