--[[
		Lua Script File
 --]]

-- ************************************************ lua文件引用路径设置 ************************************
local path = package.path
local start_idx = string.find(path,":")
local end_idx = string.find(path,"\\Rel")
local abs_path = string.sub(path,start_idx - 1,end_idx - 1)
local all_path_1 = abs_path.."\\res\\script\\logic_lua\\?.lua"
local all_path_2 = abs_path.."\\res\\script\\logic_lua\\"
local all_path_3 = abs_path.."\\res\\script\\data\\?.lua"
local all_path_4 = abs_path.."\\res\\script\\data\\"
local all_path_5 = abs_path.."\\res\\script\\logic_lua\\cs\\?.lua"
local all_path_6 = abs_path.."\\res\\script\\logic_lua\\dp\\?.lua"
local all_path_7 = abs_path.."\\res\\script\\logic_lua\\ws\\?.lua"
package.path = package.path ..";"..all_path_1
package.path = package.path ..";"..all_path_2
package.path = package.path ..";"..all_path_3
package.path = package.path ..";"..all_path_4


package.path = package.path .. ";"..abs_path.."\\res\\script\\protobuf\\?.lua"

pb_path = abs_path.."\\res\\script\\protobuf\\pb\\"

--print(package.path)

-- ************************************************ lua文件引用 ************************************

g_is_first = true

g_server_type = ""

print("faith end")
function cs_main()
	if  g_is_first == true then
		package.path = package.path..";"..all_path_5
		g_server_type = "cs"
	end

	package.loaded["cfg_data"] = nil
	require("cfg_data")
	print("cs_lua_main begin")
	package.loaded["init"] = nil
	require("init")
	main_init();
	print("cs_lua_main end")

	--local n = collectgarbage("step")

    --delog("-----collect--memory---", n)

	g_is_first = false
end

function dp_main()
	if  g_is_first == true then
		package.path = package.path..";"..all_path_6
		g_server_type = "dp"

		-------------------------------------------------


		-------------------------------------------------
	end


	print("dp_lua_main begin")
    package.loaded["cfg_data"] = nil
	require("cfg_data")
	package.loaded["init"] = nil
	require("init")
	main_init();
	print("dp_lua_main end")

	g_is_first = false
end

function ws_main()
	if  g_is_first == true then
		package.path = package.path..";"..all_path_7
		g_server_type = "ws"
	end

	print("ws_lua_main begin")
	package.loaded["cfg_data"] = nil
	require("cfg_data")
	package.loaded["init"] = nil
	require("init")
	main_init();
	print("ws_lua_main end")

	g_is_first = false
end

print("faith end")

function is_init()
	return g_is_first == false
end

function get_sever_type()
	return g_server_type
end


--[[function hotupdate() --调试更新 单个文件
	package.loaded["red_package_mgr"] = nil 
	local temp = require("red_package_mgr")
	temp:init();
	package.loaded["red_package"] = nil 
	local temp = require("red_package")
	temp:init();
end]]








