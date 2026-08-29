local dp2ws_send_class={}

function dp2ws_send_class:init()

end

function dp2ws_send_class:send_test_msg(connindex,s_role_guid)
	print("send_test_msg",s_role_guid)
	local  t_ret_msg = {
          role_guid = s_role_guid,
	   }
	MessageMgr:dp2s_send_message("hld.dp2s_proto.test_dp2s_test_msg",t_ret_msg, 20001, connindex);
end


dp2ws_send = dp2ws_send_class
return dp2ws_send





















