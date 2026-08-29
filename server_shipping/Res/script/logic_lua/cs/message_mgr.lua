local message_mgr={}

local  time_mgr =  RequireSingleton("time_mgr")

function message_mgr:init()
   
end


function message_mgr:send_message_to_self(player_ref, message_type, message_table, message_header)
	if nil == player_ref then
		return
	end
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
    player_ref:send_message_to_self_lua(code, code_length, message_header);
end


function message_mgr:send_message_to_dp(player_ref, message_type, message_table, message_header)
  if nil == player_ref then
    return
  end
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
    player_ref:send_message_to_dp_lua(code, code_length, message_header);
end


function message_mgr:send_message_to_aoi_lua(player_ref, message_type, message_table, message_header, to_self, use_length)
	if nil == player_ref then
		return
	end
    local code=encode(message_type, message_table);
    local code_length=string.len(code);
    player_ref:send_message_to_aoi_lua(code, code_length, message_header, to_self, use_length);
end
function on_data_received_cs(message_data, message_header, player_index)
   local func =  define_C[message_header];
   if func == nil then
       print("-------not find ----", message_header)
       return;
   end
   	local player_ref = hld.get_player_by_index(player_index)
	if nil == player_ref then
		return;
	end
  delog("----cs---", message_header)
  time_mgr:code_memory(message_header, true)
  func(message_data, player_ref)
  time_mgr:code_memory(message_header, false)
end


function on_data_received_ss(message_data, message_header,conn_index)
  local func =  define_C[message_header];
  if func == nil then
       print("-------not find ----", message_header)
       return;
  end
  delog("----ss---", message_header)

  time_mgr:code_memory(message_header, true)

  func(message_data, conn_index)

  time_mgr:code_memory(message_header, false)
end

MessageMgr=message_mgr;

return message_mgr;