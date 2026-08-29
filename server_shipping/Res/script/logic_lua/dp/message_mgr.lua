local message_mgr={}

function message_mgr:init()
  
end

function on_data_received_dp(message_data, message_header, conn_index)
	 delog("-------on_data_received_dp ----", message_header)
   local func =  define_C[message_header];
   if func == nil then
       print("-------not find ----", message_header)
       return;
   end
   func(message_data, conn_index)
end

function message_mgr:dp2s_send_message(message_type, message_table, message_header, conn_index)
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
	  delog("-------dp2s_send_message ----", message_header)
    hld.dbproxy_service:getInstance():send_message_lua(conn_index, code, code_length, message_header);
end

MessageMgr=message_mgr;

return message_mgr;