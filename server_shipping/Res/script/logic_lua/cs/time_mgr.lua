

local   time_mgr =  RequireSingleton("time_mgr")

function time_mgr:init()
    if is_init() then
     return
    end
     -----------------------------------------
      -------该部分数据是不可以热更
     ----------------------------------------
     self.m_last_time = 0
     self.m_last_collect_time = 0;
     self.m_delta_sec = 0;


     self.m_code_memory = { }
     self.m_code_start = {}

 

end

local is_code = false
function time_mgr:code_memory(code, is_start)
    if is_code == false then
       return
    end


    local cur_mem = collectgarbage("count")

    if self.m_code_memory[code] == nil then
         self.m_code_start[code] = 0

         self.m_code_memory[code] = { total = 0,count = 0}
    end
    if is_start then
        self.m_code_start[code] = cur_mem
        self.m_code_memory[code].count = self.m_code_memory[code].count + 1
    else
       local delta = cur_mem - self.m_code_start[code]
       self.m_code_memory[code].total = self.m_code_memory[code].total + delta
    end

end



---------------------------------------
        ---每帧执行
---------------------------------------

local show_inteval_time = 10000  --10sec 

local collect_time =  1800000   --30min



function time_mgr:is_sec_tick(i_nTime)
          
     self.m_delta_sec = self.m_delta_sec + i_nTime
     if self.m_delta_sec < 1000 then
        return false
     end
     self.m_delta_sec = 0
     return true
end


function time_mgr:Update(i_nTime)

     --print("--enter--update---")
     
     if self:is_sec_tick(i_nTime) then
        self:collect_memeory()
        self:show_info()
     end

end



function time_mgr:is_collect()
     local cur_msec = get_g_time_mesc()
     if self.m_last_collect_time > cur_msec then
        return false
     end
     self.m_last_collect_time = cur_msec + collect_time
     return true
end



function time_mgr:collect_memeory()
     if not self:is_collect() then
       return
     end 

     --collectgarbage("collect")

    --delog("-----collect--memory---", n)
end



function time_mgr:is_show()
     local cur_msec = get_g_time_mesc()
     if self.m_last_time > cur_msec then
        return false
     end
     self.m_last_time = cur_msec + show_inteval_time
     return true
end


function time_mgr:show_info()
    if not self:is_show() then
        return
    end 

    --collectgarbage("collect")


    local cur_mem = collectgarbage("count")

    print("---show_memory--", cur_mem)
    --[[if (self.m_code_memory[1639] ~= nil) then

    show_table(self.m_code_memory[1639])

    end]]

    --self:write_file(cur_mem)

end



function test1( ... )
  --collectgarbage("collect");--为了有干净的环境,先把可以收集的其他垃圾赶走先
  local  c1 = collectgarbage("count")
  print("start,Lua",c1)
  local colen = {}--在这里,colen是本地变量

  for i=1,500000 do
    table.insert(colen,{}) --不断的把新创建的数组,放到一个局部的变量中
  end
  local c2 = collectgarbage("count")
  print("end lua:",c2)
end



function time_mgr:write_file(cur_mem)
   local  file = io.open("memory.log", "a")
   local  ss  = "----memory----"..cur_mem.."\n"
   file:write(ss)
   file:close()
end



function time_mgr:get_cur_time()
	 return  get_g_time_mesc()
end










return time_mgr