local arena_mgr_ws_class={}

function arena_mgr_ws_class:init()

end

function arena_mgr_ws_class:req_load_ranks_from_db_lua()
	local  t_ret_msg = {
          role_guid = 10,
	   }
	MessageMgr:ws2dp_send_message("hld.server2dp_proto.test_ws2dp_load_arena_rank",t_ret_msg, 20001, 0);
end

arena_mgr_ws = arena_mgr_ws_class
return arena_mgr_ws