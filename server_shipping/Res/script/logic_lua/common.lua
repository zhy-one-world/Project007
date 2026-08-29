local common = {}
function common:init()
end
local tRecursion = {}
local function print_table(tab, depth, f)
    local t = {}
    for i = 1, depth do
        table.insert(t, "  ")
    end
    local str = table.concat(t)
	for k, v in pairs(tab) do
		if type(v) == "table" then
        	f(str .. k .. " = " .. tostring(v))
        else
        	f(str .."{ ".. k .. " = " .. tostring(v).." }")
        end
        if type(v) == "table" and not tRecursion[v] then
            tRecursion[v] = true
            print_table(v, depth + 1, f)
        end
    end
end

common.PrintTable = function(i_tTable, i_bStdOut)
   if type(i_tTable) ~= "table" then
        print("WARNING!!! PrintTable param not a table.")
        return
    end
    local f =  print 
    f("----- table print begin -----")
    print_table(i_tTable, 0, f)
    f("----- table print end ------")
    tRecursion = {}
end

function common:StrToTable(str)
    if  type(str) == "table" then
        return str
    end
    if str == nil or type(str) ~= "string" or str == "" then
        return {}
    end
    str = "{"..str.."}"
    return load("return " .. str)()
end


local bdelog = false
-- 打log
function delog( sLog, ... )
    if bdelog then 
        if not sLog then
            print( "=====error====",nil )
            return
        end
        if type(sLog) == "table" then
            print("************ table print begin **********")
            print_table(sLog, 3, print)
            print("************ table print end ***********")
            tRecursion = {}
        else
            print( sLog, ... )
        end
    end 
end

function show_table( sLog, ... )
    
    if not sLog then
        print( "=====error====",nil )
        return
    end
    if type(sLog) == "table" then
        print("************ table print begin **********")
        print_table(sLog, 3, print)
        print("************ table print end ***********")
        tRecursion = {}
    else
        print( sLog, ... )
    end
     
end

function common:Int64to32(value)
    local A = (value << 32) >> 32
    local B =  value >> 32
    if hld.init_unit:is_big_endian() then
        return B, A 
    end
    return A, B
end




local print = print;
local xpcall= xpcall;
local debug_traceback = debug.traceback;

local xpcall_err = function(err)
    print("ERROR!!!", err);
    print(debug.traceback());
end

ProtectedCall = function(f)
    return xpcall(f, xpcall_err);
end




function deep_copy(orig)
  local copy
  if type(orig) == "table" then
    copy = {}
    for orig_key, orig_value in next, orig, nil do
      copy[deep_copy(orig_key)] = deep_copy(orig_value)
    end
    setmetatable(copy, deep_copy(getmetatable(orig)))
  else
    copy = orig
  end
  return copy
end


function bin2hex(s)
    s=string.gsub(s,"(.)",function (x) return string.format("%02X",string.byte(x)) end)
    return s
end
 
local h2b = {
    ["0"] = 0,
    ["1"] = 1,
    ["2"] = 2,
    ["3"] = 3,
    ["4"] = 4,
    ["5"] = 5,
    ["6"] = 6,
    ["7"] = 7,
    ["8"] = 8,
    ["9"] = 9,
    ["A"] = 10,
    ["B"] = 11,
    ["C"] = 12,
    ["D"] = 13,
    ["E"] = 14,
    ["F"] = 15
}
 
function hex2bin( hexstr )
    local s = string.gsub(hexstr, "(.)(.)", function ( h, l )
         return string.char(h2b[h]*16+h2b[l])
    end)
    return s
end

--[[function  hex2bin(hexstr)
    local new_str = "";
    local char  
    for i = 1, string.len(hexstr) - 1, 2 do
        local doublebytestr = string.sub(hexstr, i, i+1);
        local n = tonumber(doublebytestr, 16);
        if 0 == n then
             char = '\00'
             --bytesfile:write('\00');
        else
             char = string.format("%c", n);
        end
        new_str = new_str .. char
    end
    return new_str
end]]


Common = common
return common






