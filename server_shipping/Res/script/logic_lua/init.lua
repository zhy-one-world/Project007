

function need_load(file_name)
	if  g_one_files[file_name] == nil then  --- 非热更文件
		return true
	end
	delog("--------need_load----", file_name)
	if  not is_init() then    
		return true
	end
	return false
end

function need_init(file_name)
    if g_one_init[file_name] == nil then
       return true
    end
    if not is_init() then
       return true
    end
    return false
end

function main_init()
	local random_seed = os.time();
	math.randomseed(random_seed);
	for k,v in pairs(g_t_files) do
		print("reload lua :", v, "begin")
		if need_load(v) then ------加载一次文件
			package.loaded[v] = nil
			local temp = require(v);
			if need_init(v) then ----多次加载一次初始化
			   temp:init();
			   print("reload lua :", v, "end")
			end
		end
	end
	--local cur_mem_before = collectgarbage("count")
	--delog("-------cur_mem-front----", cur_mem_before)
	--local n = collectgarbage("collect")

    --local cur_mem = collectgarbage("count")
	--delog("-------cur_mem-back----", cur_mem)
end


function hotupdate() --调试更新 单个文件
	--delog("-----start---")
	for k, v in pairs(g_debug_files) do
	   if need_load(v) then
	   	  package.loaded[v] = nil
	   	  print("-----update--", v, "end")
		  local temp = require(v);
		  if need_init(v) then ----多次加载一次初始化
			   temp:init();
		  end
		  --print("-----update--", v, "end")
		end
	end
end