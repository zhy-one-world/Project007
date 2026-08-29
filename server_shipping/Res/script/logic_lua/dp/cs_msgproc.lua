local cs_msgproc = {}

local define_CS = define_C


local e_save_role_data_type	=						 --角色数据保存类型
{
	exit_game = 0,						--退出游戏
	leave_scene = 1,					--传送时离开场景相关数据保存
	update = 2,						    --定时存盘
	cross = 3,							--传送到其他服务器
    role_logout_max = 4,
};

--[[
local	e_data_flag_none =							0x0000000000000000;
local	e_data_flag_info =							0x0000000000000001;		--角色基本信息
local	e_data_flag_buff =							0x0000000000000002;		--角色Buff
local	e_data_flag_skill =							0x0000000000000004;		--角色技能
local	e_data_flag_time =							0x0000000000000008;		-- 玩家时间
local	e_data_flag_money =							0x0000000000000010;		-- 角色货币
local	e_data_flag_belief =						0x0000000000000020;		--角色信仰
local	e_data_flag_spirit =						0x0000000000000040;		--角色精灵
local	e_data_flag_mission =						0x0000000000000080;		--角色任务
local	e_data_flag_att =							0x0000000000000100;		--角色属性
local	e_data_flag_goods =							0x0000000000000200;		--角色购买记录
local	e_data_flag_map =							0x0000000000000400;		--地图记录
local	e_data_flag_service_goal =					0x0000000000000800;		--七日目标记录
local	e_data_flag_special_name =					0x0000000000001000;		--角色称号记录
local	e_data_flag_achievement =					0x0000000000002000;		--角色成就记录
local	e_data_flag_pokedex =						0x0000000000004000;		--角色图鉴记录
	--const int64	e_data_flag_demons_tower =					0x0000000000008000;		// 角色万魔塔记录
local	e_data_flag_mail =							0x0000000000010000;		--角色邮件记录
local	e_data_flag_tinder =						0x0000000000020000;		--角色火种记录
local	e_data_flag_worship =						0x0000000000040000;		--玩家崇拜记录
local   e_data_flag_yesterday_must_do_remain =		0x0000000000080000;		-- 昨天没做的必做任务
local	e_data_flag_convert =						0x0000000000100000;		-- 玩家转换记录
local	e_data_flag_item =							0x0000000000200000 ;	--角色物品
local	e_data_flag_welfare =						0x0000000000400000;		--玩家福利
local	e_data_flag_active_degree =					0x0000000000800000;		--玩家活跃度
local	e_data_flag_title =							0x0000000001000000;		--玩家称号
local	e_data_flag_guide =							0x0000000002000000;		--新手引导
local	e_data_flag_fist_time_do =					0x0000000004000000;		--首次操作
	--const int64	e_data_flag_skill_use =						0x0000000008000000;		// 技能使用位置
local	e_data_flag_talent =						0x0000000010000000;		--角色天赋
local   e_data_flag_daily_must_do_count =			0x0000000020000000;		--每日必做次数记录
local	e_data_flag_highest_record =				0x0000000040000000;		--角色历史最高纪录
local	e_data_flag_auction_self_selling =			0x0000000080000000;		--交易行自己的出售物
local	e_data_flag_globel_mail_had =				0x0000000100000000;		--已拥有的全局邮件
local   e_data_flag_service_rank	=				0x0000000200000000;		--全民冲榜信息
local   e_data_flag_logic_info =					0x0000000400000000;		--玩家逻辑数据
local   e_data_flag_gain_treasure =					0x0000000800000000;		--打宝数据
local   e_data_flag_time_limit_activity =			0x0000001000000000;		--限时活动数据	
local   e_data_flag_relation =						0x0000002000000000;		--角色关系
local   e_data_flag_boss_island =					0x0000004000000000;		-- 永恒岛数据
local   e_data_flag_chat =							0x0000008000000000;		-- 聊天数据
local   e_data_flag_passive_skill =					0x0000010000000000;		-- 被动技能
local   e_data_flag_seven_day_goal =				0x0000020000000000;		--七日目标前七天记录
local   e_data_flag_harry_record =					0x0000040000000000;		-- 跨服掠夺玩家记录
local   e_data_flag_phantom =                       0x0000080000000000;		--角色幻灵
local   e_data_flag_oracle_trial=					0x0000100000000000;		--天启试炼
local   e_data_flag_chat_record =                   0x0000200000000000;		--
local   e_data_flag_player_information =			0x0000400000000000;		-- 个人信息
local   e_data_flag_recycle =                       0x0000800000000000;     --回归信息
local   e_data_flag_recycle_task =                  0x0001000000000000;     -- 回归任务
local   e_data_flag_player_competition =			0x0002000000000000;		--竞赛信息
local   e_data_flag_recycle_invited =               0x0004000000000000;		--回归招募
local   e_data_flag_dragontrip      =               0x0008000000000000;		--萌龙奇遇
local   e_data_flag_legion_skill	=			    0x0010000000000000;		-- 军团技能
local   e_data_flag_skytreasure =                   0x0020000000000000;		-- 天空宝库
local   e_data_flag_starark         =               0x0040000000000000;		-- 星海方舟
]]

 



function cs_msgproc:init()
    --delog("----cs_msgproc---init---")
end


define_CS[1408] = function(message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_starark", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_info = msg_data.db_data

    delog("--d#####################--save--role_data-----" )

    delog(t_db_info)

    local s_buffer = encode("hld.db_proto.role_starark_db", t_db_info)

    local is_save = hld.cs2dp_save_role_starark_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_data-----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_starark, msg_data.save_type_ex);
    end


    local sql = "";

    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_starark_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_info.data_ary do
    	sql = sql..t_db_info.data_ary[i]..","
    end

    sql = sql.."\\'"
    sql = sql..t_db_info.body..""
    sql = sql.."\\'"
    sql = sql.. ")');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_starark, sql, msg_data.save_type_ex)
end


define_CS[36601] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_recycle", message_data)
	if msg_data == nil then
     	return
    end 
    --delog("------xxxxxx----enter-----save---role_recycle------",e_data_flag_recycle)
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_recycle_db", t_db_row)

    local is_save = hld.cs2dp_save_role_recycle_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_recycle----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_recycle, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_recycle_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    sql = sql .. t_db_row.e_start_time_1 .. ","
    sql = sql .. t_db_row.e_login_state_2 .. ","
    sql = sql .. t_db_row.e_prop_state_3 .. ","
    sql = sql .. t_db_row.inviter_guid .. ","
    sql = sql .. t_db_row.invited_state
    sql = sql .. ")');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_recycle, sql, msg_data.save_type_ex)

end

define_CS[36603] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_recycle_task", message_data)
	if msg_data == nil then
     	return
    end 
    delog(msg_data)
    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_recycle_task_db", t_n_db_row)

    delog("-------t_n_db_row -------",string.len(s_buffer))
    delog(t_n_db_row)



    local is_save = hld.cs2dp_save_role_recycle_task_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_recycle_task----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_recycle_task, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_recycle_task_save('"..msg_data.role_guid.."','";
    


	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	--delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_recycle_task, sql, msg_data.save_type_ex)

end

define_CS[1404] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_dragontrip", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_dragontrip_db", t_db_row)

    local is_save = hld.cs2dp_save_role_dragontrip_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_recycle_task----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_dragontrip, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_dragontrip_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    --delog("----sql---", sql)

    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_dragontrip, sql, msg_data.save_type_ex)
end

define_CS[36401] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_phantom", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_phantom_db", t_n_db_row)


    local is_save = hld.cs2dp_save_role_phantom_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_recycle_task----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_phantom, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_phantom_save('"..msg_data.role_guid.."','";
    


	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	--delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_phantom, sql, msg_data.save_type_ex)

end

--------------------------------------------------------------------------------------------------------------------

define_CS[36502] = function (message_data, conn_index)
    delog("---enter---save_role_oracle_trial---")
	local msg_data = decode("hld.cs2dp_proto.save_role_oracle_trial", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_oracle_trial_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_oracle_trial_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_recycle_task----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_oracle_trial, msg_data.save_type_ex);
    end
    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_oracle_trial_cache_lua---")
    end 

    local sql_head = "call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid)..".sp_oracle_trial_player_info_save("..msg_data.role_guid..",";

    local sql = ""


    for i=1,t_n_db_row.row_count do
		local t_row = t_n_db_row.row_data[i]
        sql = sql_head;
		sql = sql.. t_row.tier_num .. ","

		sql = sql .. "'(";
		sql = sql .. t_row.role_guid .. ",";
		sql = sql .. t_row.tier_num .. ",";
		sql = sql .. t_row.customs_state ..",";
		sql = sql .. t_row.reward_stat .. ")'";
		sql = sql .. ");";
		delog("---sql--oracle_trial-", sql)
		hld.cs2dp_save_push_sql_no_callback_lua(sql,e_data_flag_none, msg_data.role_guid)
	end
end


define_CS[33314] = function(message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_chat_record", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data


    local s_buffer = encode("hld.cs2dp_proto.role_chat_record_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_chat_record_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--chat_record----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_chat_record, msg_data.save_type_ex);
    end
    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_chat_record_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_chat_record_save('"..msg_data.role_guid.."','";

    for i=1,t_n_db_row.row_count do

    	local t_row = t_n_db_row.row_data[i]
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		sql = sql .. t_row.pos .. ","
		sql = sql .. "\\'"
		sql = sql .. t_row.record .. ""
		sql = sql .. "\\'"

		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	delog("--sql---chat_record--", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_chat_record, sql, msg_data.save_type_ex)



end


define_CS[1392] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_person_information", message_data)
	if msg_data == nil then
     	return
    end 
    --delog("------xxxxxx----enter-----save---role_recycle------",e_data_flag_recycle)
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_person_information_db", t_db_row)

    local is_save = hld.cs2dp_save_role_person_information_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--e_data_flag_player_information----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_player_information, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_person_information_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid)..".sp_save_person_information(";

	sql = sql .. msg_data.role_guid .. ",";
	sql = sql .. "'(";
	sql = sql .. msg_data.role_guid .. ",";
	sql = sql .. t_db_row.infor_state .. ",";
	sql = sql .. t_db_row.reward_state .. ",";
	sql = sql .. "\\\'" .. t_db_row.gender .. "\\\'" .. ",";
	sql = sql .. "\\\'" .. t_db_row.personalized_signature .. "\\\'" ..",";
	sql = sql .. t_db_row.birthday_year .. ",";
	sql = sql .. t_db_row.birthday_mouth .. ",";
	sql = sql .. t_db_row.birthday_day .. ",";
	sql = sql .. "\\\'" .. t_db_row.city .. "\\\'" .. ",";
	sql = sql .. "\\\'" .. t_db_row.hometown .. "\\\'" .. ",";
	sql = sql .. "\\\'" .. t_db_row.online_state .. "\\\'" .. ",";

	sql = sql .. t_db_row.tags[1] .. ",";
	sql = sql .. t_db_row.tags[2] .. ",";
	sql = sql .. t_db_row.tags[3] .. ",";
	sql = sql .. t_db_row.tags[4] .. ",";
    sql = sql .. t_db_row.tags[5] .. ",";
    sql = sql .. t_db_row.tags[6] .. ",";
	sql = sql .. t_db_row.tags[7] .. ",";
	sql = sql .. t_db_row.tags[8] .. ")'";
	sql = sql .. ");";

   
    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_player_information, sql, msg_data.save_type_ex)
end

define_CS[1400] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_competition", message_data)
	if msg_data == nil then
     	return
    end 
    --delog("------xxxxxx----enter-----save---role_recycle------",e_data_flag_recycle)
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_competition_db", t_db_row)

    local is_save = hld.cs2dp_save_role_competition_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--e_data_flag_player_competition----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_player_competition, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_competition_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid)..".sp_save_role_competition(";

	sql = sql .. msg_data.role_guid .. ",";
	sql = sql .. "'(";
	sql = sql .. msg_data.role_guid .. ",";

	sql = sql .. t_db_row.is_buy .. ",";
	sql = sql .. t_db_row.buy_manual_type .. ",";
	sql = sql .. t_db_row.point_add_percent .. ",";
	sql = sql .. t_db_row.season .. ",";
	sql = sql .. t_db_row.competition_begin_time .. ",";
	sql = sql .. t_db_row.competition_end_time .. ",";
	sql = sql .. t_db_row.level .. ",";
	sql = sql .. t_db_row.cur_exp .. ",";
	sql = sql .. t_db_row.need_reset .. ",";
	sql = sql .. t_db_row.assist_fight_guid .. ",";

	sql = sql .. t_db_row.reward_state[1] .. ",";
	sql = sql .. t_db_row.reward_state[2] .. ",";
	sql = sql .. t_db_row.reward_state[3] .. ",";
	sql = sql .. t_db_row.reward_state[4] .. ",";
    sql = sql .. t_db_row.reward_state[5] .. ",";
    sql = sql .. t_db_row.reward_state[6] .. ",";
	sql = sql .. t_db_row.reward_state[7] .. ")'";
	sql = sql .. ");";

   
    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_player_competition, sql, msg_data.save_type_ex)
end

define_CS[1905] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_legion_skill", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data

    delog(t_n_db_row[1])
    delog(type(t_n_db_row[2]))

    delog(t_n_db_row)

    local s_buffer = encode("hld.cs2dp_proto.role_legion_skill_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_legion_skill_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--e_data_flag_legion_skill----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_legion_skill, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_legion_skill_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_legion_skill_save('"..msg_data.role_guid.."','";
    


	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_legion_skill, sql, msg_data.save_type_ex)
end


define_CS[1406] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_skytreasure", message_data)
	if msg_data == nil then
     	return
    end 
    --delog("------xxxxxx----enter-----save---role_recycle------",e_data_flag_recycle)
    local t_db_row = msg_data.db_data
    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid)..".sp_role_skytreasure_save(";
	sql = sql..msg_data.role_guid..","
	sql = sql..t_db_row.skytreasure_layer_num..","
	sql = sql..t_db_row.skytreasure_pos.. ",'"
	sql = sql.."(".. msg_data.role_guid .. ","..t_db_row.skytreasure_layer_num .. "," .. t_db_row.skytreasure_pos .. "," .. t_db_row.skytreasure_pos_in_array.. ")";
	sql = sql.. "');"; 

	delog("---sql--role_skytreasure-", sql)
	hld.cs2dp_save_push_sql_no_callback_lua(sql, e_data_flag_skytreasure, msg_data.role_guid)
end


------------------------------------------------------------------------------------------------------------------------------------------------

define_CS[36006] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_harry_record", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_harry_record_db", t_db_row)

    local is_save = hld.cs2dp_save_role_harry_record_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--harry_record----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_harry_record, msg_data.save_type_ex);
    end



    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_harry_info_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)

    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_harry_record, sql, msg_data.save_type_ex)
end

define_CS[35901] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_seven_day_goal", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_seven_day_goal_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_seven_day_goal_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--_seven_day_goal----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_seven_day_goal, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_seven_day_goal_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_seven_day_goal_save('"..msg_data.role_guid.."','";
    


	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_seven_day_goal, sql, msg_data.save_type_ex)
end


define_CS[1903] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_passive_skill", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_passive_skill_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_passive_skill_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--_passive_skill----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_passive_skill, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_passive_skill_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_passive_skill_save('"..msg_data.role_guid.."','";
    


	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_passive_skill, sql, msg_data.save_type_ex)
end

define_CS[35601] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_boss_island", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_boss_island_db", t_db_row)

    local is_save = hld.cs2dp_save_role_boss_island_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--harry_record----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_boss_island, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_boss_island_info_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)

    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_boss_island, sql, msg_data.save_type_ex)
end


----------------------------------------------------------------------------------------------------------------------------------------------

define_CS[35402] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_time_limit_activity", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data
    local s_buffer = encode("hld.cs2dp_proto.role_time_limit_activity_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_time_limit_activity_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--_passive_skill----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_time_limit_activity, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_role_time_limit_activity_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_time_limit_activity_save('"..msg_data.role_guid.."','";
   
    local num = 0;
	for i=1,t_n_db_row.row_count do
		local t_row = t_n_db_row.row_data[i]
		if t_row.data_ary[4] > 0 then
			if num ~= 0 then
				sql = sql ..","
			end
			num = num + 1
			sql = sql .. "(" .. msg_data.role_guid.. ",";
			for j=1, #t_row.data_ary do
				sql = sql ..t_row.data_ary[j]..","
			end
			for j=1, #t_row.activity_schedule do
				sql = sql ..t_row.activity_schedule[j]..","
			end
			sql = sql ..t_row.reward_flag..")" 
		end
	end
	sql = sql .. "');"

	delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_time_limit_activity, sql, msg_data.save_type_ex)
end


define_CS[35101] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_gain_treasure", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_gain_treasure_db", t_db_row)

    local is_save = hld.cs2dp_save_role_gain_treasure_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--e_data_flag_gain_treasure----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_gain_treasure, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_gain_treasure_info_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)

    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_gain_treasure, sql, msg_data.save_type_ex)
end

define_CS[1348] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_logic", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_logic_db", t_db_row)

    local is_save = hld.cs2dp_save_role_logic_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--e_data_flag_logic_info----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_logic_info, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_logic_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_logic_info, sql, msg_data.save_type_ex)
end


define_CS[35001] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_service_rank_reward", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_service_rank_reward_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_service_rank_reward_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--e_data_flag_service_rank---" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_service_rank, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_save_data_flag_service_rank_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_service_rank_reward_state_save('"..msg_data.role_guid.."','";
    


	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]

		sql = sql .. t_row.rank_type .. ",";
	    sql = sql .. t_row.reward_state;

		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_service_rank, sql, msg_data.save_type_ex)
end


--------------------------------------------------------------------------------------------------------------------------------

define_CS[1381] = function (message_data, conn_index)
    delog("-----1381---len---", string.len(message_data))
	local msg_data = decode("hld.cs2dp_proto.save_role_highest_record", message_data)
	if msg_data == nil then
     	return
    end

    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_highest_record_db", t_db_row)

    local is_save = hld.cs2dp_save_role_highest_record_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_highest_record---" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_highest_record, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_highest_record_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_highest_record, sql, msg_data.save_type_ex)
end


define_CS[1384] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_daily_must_do_count", message_data)
	if msg_data == nil then
     	return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_daily_must_do_count_db", t_db_row)

    local is_save = hld.cs2dp_save_role_daily_must_do_count_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--daily_must_do_count---" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_daily_must_do_count, msg_data.save_type_ex);
    end

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_daily_must_do_count_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_row.data_ary do
    	sql = sql .. t_db_row.data_ary[i]
    	if i ~= #t_db_row.data_ary then
    	   sql = sql .. ","
    	end
    end
    sql = sql .. ")');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_daily_must_do_count, sql, msg_data.save_type_ex)
end


define_CS[34401] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_talent", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_talent_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_talent_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_talent----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_talent, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_role_talent_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_talent_save('"..msg_data.role_guid.."','";
    

	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	--delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_talent, sql, msg_data.save_type_ex)
end

define_CS[34101] = function (message_data, conn_index)
	local msg_data = decode("hld.cs2dp_proto.save_role_title", message_data)
	if msg_data == nil then
     	return
    end 

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_title_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_title_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_title----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_title, msg_data.save_type_ex);
    end

    if (is_save == false) then
    	print("---ERROR-#######--cs2dp_role_title_cache_lua---")
    end 

    local sql = ""
	sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_title_save('"..msg_data.role_guid.."','";
    

	for i=1,t_n_db_row.row_count do
		sql = sql .. "(" .. msg_data.role_guid.. ",";
		local t_row = t_n_db_row.row_data[i]
		for j=1, #t_row.data_ary do
			sql = sql ..t_row.data_ary[j]
			if (j ~= #t_row.data_ary) then
				sql = sql..",";
			end
		end
		sql = sql ..")"
		if i ~= t_n_db_row.row_count then
			sql = sql ..","
		end
	end
	sql = sql .. "');"

	--delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_title, sql, msg_data.save_type_ex)
end


---------------------------------------------------------------------------------------------------------------------------------
define_CS[33603] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_active_degree", message_data)
    if msg_data == nil then
        return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_active_degree_db", t_db_row)

    local is_save = hld.cs2dp_save_role_active_degree_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_active_degree---" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_active_degree, msg_data.save_type_ex);
    end

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_active_degree_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_row.data_ary do
        sql = sql .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_active_degree, sql, msg_data.save_type_ex)
end

define_CS[33601] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_welfare", message_data)
    if msg_data == nil then
        return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_welfare_db", t_db_row)

    local is_save = hld.cs2dp_save_role_welfare_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_welfare---" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_welfare, msg_data.save_type_ex);
    end

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_welfare_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_row.data_ary do
        sql = sql .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_welfare, sql, msg_data.save_type_ex)
end

define_CS[33401] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_convert", message_data)
    if msg_data == nil then
        return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_convert_db", t_db_row)

    local is_save = hld.cs2dp_save_role_convert_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_convert---" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_convert, msg_data.save_type_ex);
    end

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_convert_save('"..msg_data.role_guid.."','";



    for i=1, #t_db_row.data_ary do

        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local i_value = t_db_row.data_ary[i]
        local i_idx = i - 1

        sql = sql .. i_idx ..","
        sql = sql .. i_value 

        sql = sql ..")"
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. "');"


    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_convert, sql, msg_data.save_type_ex)
end


define_CS[1358] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_yesterday_must_do_remain", message_data)
    if msg_data == nil then
        return
    end 

    --delog(msg_data)

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_yesterday_must_do_remain_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_yesterday_must_do_remain_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_yesterday_must_do_remain----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_yesterday_must_do_remain, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_e_data_flag_yesterday_must_do_remain---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_yesterday_must_do_remain_save('"..msg_data.role_guid.."','";

    local count = 0;
    for i=1,t_n_db_row.row_count do
       local t_row = t_n_db_row.row_data[i]
       if t_row.role_guid ~= 0 then
          count = count + 1
       end
    end
    

    for i=1,t_n_db_row.row_count do
        local t_row = t_n_db_row.row_data[i]
        if t_row.role_guid ~= 0 then
            sql = sql .. "(" .. t_row.role_guid.. ",";
            sql = sql .. t_row.role_exp_level_yesterday .. ","
            sql = sql .. t_row.save_date .. ","
            sql = sql .. t_row.role_vip_level_yesterday .. ","
            sql = sql .. t_row.distance_today_day_count .. ","
            sql = sql .. t_row.role_arena_rank .. ",";
            for j=1, #t_row.data_ary do
                sql = sql ..t_row.data_ary[j]
                if (j ~= #t_row.data_ary) then
                    sql = sql..",";
                end
            end
            sql = sql ..")"

            count = count - 1
            if count ~= 0 then
               sql = sql ..","
            end
        end
    end
    sql = sql .. "');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_yesterday_must_do_remain, sql, msg_data.save_type_ex)
end


define_CS[33105] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_worship", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_worship_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_worship_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_title----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_worship, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--e_data_flag_worship---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_worship_target_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        sql = sql .. t_row.ranking_index .. ","
        sql = sql .. t_row.target_guid
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_worship, sql, msg_data.save_type_ex)
end



define_CS[33201] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_tinder", message_data)
    if msg_data == nil then
        return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_tinder_db", t_db_row)

    local is_save = hld.cs2dp_save_role_tinder_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_tinder----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_tinder, msg_data.save_type_ex);
    end



    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_tinder_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","


    for i=1, #t_db_row.data_ary do
        sql = sql .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. ")');"

    delog("----sql---", sql)

    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_tinder, sql, msg_data.save_type_ex)
end



-----------------------------------------------------------------------------------------------------------------------------------------

define_CS[32701] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_pokedex", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data



    local s_buffer = encode("hld.cs2dp_proto.role_pokedex_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_pokedex_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_pokedex----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_pokedex, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_title_cache_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_pokedex_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_pokedex, sql, msg_data.save_type_ex)
end


define_CS[32601] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_achievement", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_achievement_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_achievement_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_achievement----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_achievement, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_achievement_cache_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_achievement_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_achievement, sql, msg_data.save_type_ex)
end



define_CS[32501] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_special_name", message_data)
    if msg_data == nil then
        return
    end 
    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_special_name_db", t_db_row)

    local is_save = hld.cs2dp_save_role_special_name_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_special_name----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_special_name, msg_data.save_type_ex);
    end

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_special_name_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_row.data_ary do
        sql = sql .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. ")');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_special_name, sql, msg_data.save_type_ex)
end


define_CS[34601] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_service_goal", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_service_goal_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_service_goal_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_service_goal----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_service_goal, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_service_goal_cache_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_service_goal_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_service_goal, sql, msg_data.save_type_ex)
end

define_CS[2401] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_map_record", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_map_record_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_map_record_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_map_record----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_map, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_map_record_cache_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_map_record_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_map, sql, msg_data.save_type_ex)
end

define_CS[32301] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_goods", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_goods_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_goods_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_goods----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_goods, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_goods_cache_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_goods_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_goods, sql, msg_data.save_type_ex)
end


------------------------------------------------------------------------------------------------------------------------------
define_CS[2001] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_att", message_data)
    if msg_data == nil then
        return
    end 

    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_att_db", t_db_row)

    local is_save = hld.cs2dp_save_role_att_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_att----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_att, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_mission_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_att_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_row.data_ary do
        sql = sql .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. ")');"


    delog("----sql----", sql)

    hld.cs2dp_push_role_att_sql_add_callback(sql, msg_data.role_guid, t_db_row.attack_att, conn_index, msg_data.unit_array_index, msg_data.save_type_ex);

end


define_CS[32201] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_mission", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_mission_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_mission_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_mission----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_mission, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_mission_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_mission_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_mission, sql, msg_data.save_type_ex)
end


define_CS[32101] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_belief", message_data)
    if msg_data == nil then
        return
    end 

    local t_n_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_belief_db", t_n_db_row)

    local is_save = hld.cs2dp_save_role_belief_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_belief----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_belief, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_belief_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_belief_save('"..msg_data.role_guid.."','";
    

    for i=1,t_n_db_row.row_count do
        sql = sql .. "(" .. msg_data.role_guid.. ",";
        local t_row = t_n_db_row.row_data[i]
        for j=1, #t_row.data_ary do
            sql = sql ..t_row.data_ary[j]
            if (j ~= #t_row.data_ary) then
                sql = sql..",";
            end
        end
        sql = sql ..")"
        if i ~= t_n_db_row.row_count then
            sql = sql ..","
        end
    end
    sql = sql .. "');"

    delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_belief, sql, msg_data.save_type_ex)
end

local e_money_array_type = 
{
    cur = 0, --当前钱数
    total_get = 1, --总获得数
    total_cost = 2, --总消费数
    max = 3
};


define_CS[1344] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_money", message_data)
    if msg_data == nil then
        return
    end 

    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_money_db", t_db_row)

    local is_save = hld.cs2dp_save_role_money_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_money----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_money, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_money_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_money_save('"..msg_data.role_guid.."','";

    sql = sql .. "(" .. msg_data.role_guid .. "," .. e_money_array_type.cur + 1 .. ",";

    for i=1, #t_db_row.cur_data_ary do
        sql = sql .. t_db_row.cur_data_ary[i]
        if i ~= #t_db_row.cur_data_ary then
           sql = sql .. ","
        end
    end

    sql = sql .. "),"

    sql = sql .. "(" .. msg_data.role_guid .. "," .. e_money_array_type.total_get + 1 .. ",";

    for i=1, #t_db_row.all_data_ary do
        sql = sql.. t_db_row.all_data_ary[i]
        if i ~= #t_db_row.all_data_ary then
           sql = sql .. ","
        end
    end

    sql = sql .. "),"

    sql = sql .. "(" .. msg_data.role_guid .. "," .. e_money_array_type.total_cost + 1 .. ",";

    for i=1, #t_db_row.cost_data_ary do
        sql = sql .. t_db_row.cost_data_ary[i]
        if i ~= #t_db_row.cost_data_ary then
           sql = sql .. ","
        end
    end

    sql =  sql .. ")');";

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_money, sql, msg_data.save_type_ex)
end

define_CS[1346] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_time", message_data)
    if msg_data == nil then
        return
    end 

    local t_db_row = msg_data.db_data

    local s_buffer = encode("hld.cs2dp_proto.role_time_db", t_db_row)

    local is_save = hld.cs2dp_save_role_time_cache_lua(msg_data.role_guid, s_buffer, string.len(s_buffer))
    if (is_save and  msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then  
       delog("----save--role_time----" )
       hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_time, msg_data.save_type_ex);
    end

    if (is_save == false) then
        print("---ERROR-#######--cs2dp_role_time_lua---")
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_time_save('"..msg_data.role_guid.."','";

    sql = sql.. "("..msg_data.role_guid..","

    for i=1, #t_db_row.data_ary do
        sql = sql .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. ")');"
    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_time, sql, msg_data.save_type_ex)
end

---------------------------------------------------------------------------------------------------------------------

local e_role_right_vip_point = 3;

define_CS[1340] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_info", message_data)
    if msg_data == nil then
        return
    end 

    local t_db_row = msg_data.db_data

    if t_db_row.data_ary[hld.e_role_info_recharge_num + 1] > 0 then
        
        local gm_sql = ""
           
        gm_sql =  gm_sql .. "call sp_gm_set_role_right('"
        gm_sql =  gm_sql .. t_db_row.account .. "',"
        gm_sql =  gm_sql .. e_role_right_vip_point .. ","
        gm_sql =  gm_sql .. t_db_row.data_ary[hld.e_role_info_recharge_num + 1] .. ")";

        hld.cs2dp_save_push_sql_no_callback_lua(gm_sql,e_data_flag_none, msg_data.role_guid)
    end


    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_info_save("

    sql = sql .. "'UPDATE role_info SET "
    sql = sql .. "role_appearance = " .. "\\\'" .. t_db_row.role_appearance .. "\\\',";
    sql = sql .. "map_guid = " .. t_db_row.map_guid .. ",";
    sql = sql .. "role_first_kill_npc_id_arr = " .. "\\\'" .. t_db_row.role_first_kill_ary .. "\\\',";
    
    for i=1,#t_db_row.data_i64_ary do
        sql = sql .. e_role_i64_info_str[i] .. " = " .. t_db_row.data_i64_ary[i] .. ",";   
    end

    for i=1, #t_db_row.data_ary do
        sql = sql .. e_role_info_str[i] .. " = " .. t_db_row.data_ary[i]
        if i ~= #t_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. " WHERE role_guid =" .. msg_data.role_guid .. ";');";

    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_no_cache_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_info, sql, msg_data.save_type_ex)
end


define_CS[36607] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_info", message_data)
    if msg_data == nil then
        return
    end 

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.target_guid).. ".sp_role_recycle_add_invited(".. msg_data.target_guid.server_64 ..",";
    sql = sql.. msg_data.role_guid..",";
    sql = sql.. "'(" .. msg_data.target_guid .. ","..msg_data.role_guid .. "," ..0 .. ")'";
    sql = sql.. ");";

    hld.cs2dp_save_push_sql_no_callback_lua(sql, e_data_flag_none, msg_data.role_guid);

end

define_CS[34200] = function(message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_guide", message_data)
    if msg_data == nil then
        return
    end 
    local t_db_row = msg_data.db_data
    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_guide_record_save(" .. msg_data.role_guid .. ",";
    sql = sql.. t_db_row.guide_trigger_id .. ",'";
    sql = sql.. "(" .. msg_data.role_guid .. "," .. t_db_row.guide_trigger_id .."," .. t_db_row.state .. ")";
    sql = sql.. "');";
    hld.cs2dp_save_push_sql_no_callback_lua(sql, e_data_flag_guide, msg_data.role_guid);
end



define_CS[2101] = function (message_data, conn_index)
    local msg_data = decode("hld.cs2dp_proto.save_role_db_data", message_data)
    if msg_data == nil then
        return
    end 

    local s_buffer = msg_data.db_data

    local guid_ptr = hld.get_guid_64(msg_data.role_guid)

    local  data_set_ptr = hld.data_manager:get_instance():get_data_set(guid_ptr);

    if  data_set_ptr ~= nil then
        data_set_ptr:add_data_info(e_data_flag_buff, msg_data.db_data, string.len(msg_data.db_data), 1);
        if(msg_data.save_type_ex ~= e_save_role_data_type.exit_game) then
            hld.cs2dp_req_send_info_end(true, conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_buff, msg_data.save_type_ex);
        end
         delog("----save--role---buff----" )
    end

    delog("----decode----buff----start")

    local t_n_db_row = decode("hld.buff_proto.buff_save_load", msg_data.db_data)

    if t_n_db_row == nil then
       print("----decode----role_buff--error-----")
       return
    end

    --delog(t_n_db_row)

    local sql = ""
    sql = sql.."call "..hld.data_manager:get_instance():get_db_name(msg_data.role_guid).. ".sp_role_buff_save('"..msg_data.role_guid.."','";


    for i=1, #t_n_db_row.data_ary do
        local t_db_row = t_n_db_row.data_ary[i]

        sql = sql .. "("
        sql = sql .. msg_data.role_guid .. ","
        sql = sql .. t_db_row.buff_guid .. ","
        sql = sql .. t_db_row.sender_guid .. ","

        for j=1, #t_db_row.data_ary do
            sql = sql .. t_db_row.data_ary[j]
            if j ~= #t_db_row.data_ary then
               sql = sql .. ","
            end
        end
        sql = sql .. ")"
        if i ~= #t_n_db_row.data_ary then
           sql = sql .. ","
        end
    end
    sql = sql .. "');";


    --delog("----sql---", sql)
    hld.cs2dp_save_push_sql_lua(conn_index, msg_data.role_guid, msg_data.unit_array_index, e_data_flag_buff, sql, msg_data.save_type_ex)
end










g_cs_msgproc = cs_msgproc

return cs_msgproc