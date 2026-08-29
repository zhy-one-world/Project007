local msgproc_ranking_ws={}

function msgproc_ranking_ws:init()
	--dump("----0------------")
end
--功能消息入口
define_C[20001] = function(message_data, conn_index)
	 print("-------test_ws2dp_load_arena_rank ----", message_data, conn_index)
	local msg_data = decode("hld.server2dp_proto.test_ws2dp_load_arena_rank", message_data)
	if msg_data == nil then
     	return
    end
	 print("-------test_ws2dp_load_arena_rank ----", msg_data.role_guid) 
	hld.do_load_arena_rank(msg_data.role_guid,msg_data.role_guid);
end


return msgproc_ranking_ws