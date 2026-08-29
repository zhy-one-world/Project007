local d2w_msgproc_lua_class={}


local red_package_mgr = RequireSingleton("red_package_mgr")
local arena_mgr = RequireSingleton("arena_mgr")

function d2w_msgproc_lua_class:init()
	--dump("----0------------")
end
--功能消息入口
define_C[1311] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_get_game_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_get_game_info----")
	hld.dp2ws_rep_get_game_info_ws_lua(conn_index,msg_data.server_id,msg_data.begin_time,msg_data.server_info_arr,msg_data.gm_commond_arr);
end

define_C[1337] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_load_character", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_load_character----")
	local unit_info = msg_data.role_info
	
	hld.dp2ws_rep_get_role_info_ws_lua(conn_index, unit_info.role_guid, unit_info.account, unit_info.role_name, unit_info.role_appearance, 
	unit_info.role_first_kill_npc_id_arr, unit_info.map_guid, unit_info.data_i64_ary, unit_info.data_ary, 
	msg_data.ws_data_ary, msg_data.is_login, msg_data.is_send_chat, msg_data.worship_value, msg_data.eresult)
end

define_C[32901] = function(message_data, conn_index)
    --print("--------message_data----", string.len(message_data));
    
	local msg_data = decode("hld.dp2s_proto.dp2ws_load_arena_rank", message_data)
	if msg_data == nil then
     	return
    end

	print("-------d2w_msgproc_lua_class dp2ws_load_arena_rank----")

	if not is_arena_use_lua() then
	
		local t_load_data_all_msg = msg_data.load_data_all;
		local s_buffer = encode("hld.dp2s_proto.s_arena_rank_all", t_load_data_all_msg)
		
		hld.dp2ws_rep_load_arena_rank_lua(conn_index,s_buffer, string.len(s_buffer), msg_data.start_index)

		return
    end

   arena_mgr:load_ranks_from_db(msg_data.load_data_all.load_data, msg_data.start_index, msg_data.load_data_all.row_count)

end

define_C[32919] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_create_robot", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_create_robot----")
    if not is_arena_use_lua() then
		hld.arena_mgr_ws:get_instance():create_robot();
    end
    arena_mgr:create_robot()
end

define_C[32920] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_arena_load_finish", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_arena_load_finish----")
	if not is_arena_use_lua() then
		hld.arena_mgr_ws:get_instance():fill_empty_pos();
	end
	arena_mgr:fill_empty_pos()
end

define_C[33101] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_load_all_ranking_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_load_all_ranking_info----")
	
	local t_load_data_all_msg = msg_data.data_info;
	local s_buffer = encode("hld.dp2s_proto.s_ranking_top_info", t_load_data_all_msg)
	
	hld.dp2ws_rep_load_all_ranking_info_lua(conn_index,s_buffer, string.len(s_buffer), msg_data.ranking_type)
end

define_C[33107] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_load_one_ranking_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_load_one_ranking_info----")
	
	local t_load_data_all_msg = msg_data.data_info;
	local s_buffer = encode("hld.dp2s_proto.s_ranking_player_info", t_load_data_all_msg)
	
	hld.dp2ws_rep_load_one_ranking_info_lua(conn_index,s_buffer, string.len(s_buffer), msg_data.sender_guid, msg_data.ranking_type, msg_data.worship_add_value, msg_data.worship_type, msg_data.end_result)
end

define_C[33111] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_get_worship_value_end", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_get_worship_value_end----")
	
	local t_load_data_all_msg = msg_data.target;
	local s_buffer = encode("hld.dp2s_proto.s_ranking_player_info", t_load_data_all_msg)
	
	hld.dp2ws_rep_load_one_worship_info_proc_lua(conn_index,s_buffer, string.len(s_buffer), msg_data.sender, msg_data.is_load)
end

define_C[33116] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_load_all_service_rank_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_load_all_service_rank_info----")
	
	local t_load_data_all_msg = msg_data.all_data;
	local s_buffer = encode("hld.dp2s_proto.s_service_rank_info", t_load_data_all_msg)
	
	hld.dp2ws_rep_load_all_service_rank_info_lua(conn_index,s_buffer, string.len(s_buffer))
end

define_C[1335] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_load_last_server_list", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_load_last_server_list----")
	hld.dp2ws_rep_load_last_server_list_lua(conn_index,msg_data.server_id,msg_data.last_server_list);
end

define_C[1332] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_gm_set_result", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_gm_set_result----")
	hld.dp2ws_gm_set_result_proc_lua(conn_index,msg_data.listen_port,msg_data.handle_index,msg_data.result_code,msg_data.result_msg);
end

define_C[1319] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_save_recharge", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_save_recharge----")
	hld.dp2ws_save_recharge_proc_lua(conn_index,msg_data.order_id,msg_data.role_guid,msg_data.game_goods_id,msg_data.pay_price
	,msg_data.success,msg_data.order_num,msg_data.payment_type,msg_data.direct_diamond,msg_data.client_uid,msg_data.order_token);
end

define_C[1321] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_send_recharge_end_result", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_send_recharge_end_result----")
	hld.dp2ws_send_recharge_end_result_proc_lua(conn_index,msg_data.result_type,msg_data.order_num,msg_data.payment_type,msg_data.goods_id
	,msg_data.direct_diamond,msg_data.role_guid,msg_data.order_id);
end

define_C[3003] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_query_server_info_gm", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_query_server_info_gm----")
	hld.dp2ws_gm_get_server_info_proc_lua(conn_index,msg_data.acc_num,msg_data.role_num);
end

define_C[1327] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_gm_change_account", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_gm_change_account----")
	hld.dp2ws_gm_change_account_proc_lua(conn_index,msg_data.change_res,msg_data.listen_port,msg_data.handle_index);
end

define_C[1314] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_gm_get_role_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_gm_get_role_info----")
	
	local t_load_data_all_msg = msg_data.role_info_arr;
	local s_buffer = encode("hld.dp2s_proto.dp2ws_gm_get_role_info_arr", t_load_data_all_msg)
	
	hld.dp2ws_gm_get_role_info_proc_lua(conn_index,s_buffer, string.len(s_buffer), msg_data.listen_port, msg_data.handle_index, msg_data.check_type);
end

define_C[1317] = function(message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.dp2ws_gm_modify_role_info", message_data)
	if msg_data == nil then
     	return
    end
	print("-------d2w_msgproc_lua_class dp2ws_gm_modify_role_info----")
	hld.dp2ws_gm_modify_role_info_proc_lua(conn_index,msg_data.role_guid,msg_data.listen_port,msg_data.handle_index);
end



define_C[33021] = function (message_data, conn_index)
	local msg_data = decode("hld.dp2s_proto.load_red_package_end", message_data)
	if msg_data == nil then
		return
	end

	delog("----load--red_package------")


	if not is_red_use_lua() then
	    hld.lua_dp2ws_load_red_package_end_proc(conn_index, message_data, string.len(message_data))
	    return
	end 

	red_package_mgr:load_red_package_end(msg_data)

end


define_C[33022] = function ( message_data, conn_index )
	local msg_data = decode("hld.dp2s_proto.load_red_package_receiver_end", message_data)
	if msg_data == nil then
		return
	end

	if not is_red_use_lua() then
	    hld.lua_dp2ws_load_red_package_receiver_end_proc(conn_index, message_data, string.len(message_data))
	    return
	end 

	delog("----load--red_package---receiver---")

	red_package_mgr:load_red_package_receiver_end(msg_data)

end

d2w_msgproc_lua = d2w_msgproc_lua_class
return d2w_msgproc_lua
















