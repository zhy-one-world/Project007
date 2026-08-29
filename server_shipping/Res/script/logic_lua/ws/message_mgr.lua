local message_mgr={}

function message_mgr:init()
  

end

function message_mgr:ws2dp_send_message(message_type, message_table, message_header,server_id)
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
	--print("-------ws2dp_send_message ----", message_header)
    hld.ws_client:getInstance():send_to_dp_lua(code, code_length, message_header, server_id);
end



function message_mgr:send_to_cs(session, message_type, message_table, message_header)
    if nil == session then
        return
    end
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
    session:send_to_cs_lua(code, code_length, message_header);
end

local is_hot = false
local function hook(message_header)
    if  is_hot then 
      hotupdate()
    end
end


function on_data_received_ws(message_data, message_header, conn_index)
	 delog("-------on_data_received_ws ----", message_header)
   local func =  define_C[message_header];
   if func == nil then
       print("-------not find ----", message_header)
       return;
   end
   hook(message_header)
   func(message_data, conn_index)
end



-----------------------------------------------------------------------------------------------


function message_mgr:send_to_client(session, message_type, message_table, message_header)
    if nil == session then
        return
    end
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
    session:send_to_client(code, code_length, message_header);
end


function message_mgr:send_message_to_all_client(message_type, message_table, message_header)
    local code=encode(message_type, message_table);
    local code_length=string.len(code);

    hld.get_client_session_mgr():send_message_to_all_client_data_lua(code, code_length, message_header)
end
function message_mgr:send_message_to_map(map_template_id,message_type, message_table, message_header)
    local code=encode(message_type, message_table);
    local code_length=string.len(code);

    hld.get_client_session_mgr():send_message_to_map_lua(map_template_id, code, code_length, message_header)
end


function message_mgr:send_message_to_all_legion_member(legion, message_type, message_table, message_header)
    if legion == nil then
      return
    end
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
    legion:send_message_to_all_member_data_lua(code, code_length, message_header)
end


function on_data_received_cs(message_data, message_header, session_index)
  local func =  define_C[message_header];
  if func == nil then
       print("-------not find ----", message_header)
       return;
  end
  local session = hld.get_client_session_by_index(session_index)
  if nil == session then
     print("----------not session-", session_index)
    return;
  end
  delog("---------message--head---", message_header)
  hook(message_header)
  func(message_data, session)
end

----------------------------------------------------------------------------

MessageMgr=message_mgr;

return message_mgr;