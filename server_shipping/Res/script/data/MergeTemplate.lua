
local map_data = {
}local data = { 
    [10200] = {},
    [10203] = {["DbIp"] = "192.168.0.203",["DbPort"] = 3006,},

 }
local _default_values = {
    ["MergeServerId"] = 10200,
    ["DbIp"] = "\"192.168.0.200\"",
    ["DbPort"] = 3306,
    ["UserName"] = "\"root\"",
    ["UserPassword"] = "\"root123\"",
    ["SchemaName"] = "\" low_game_schema\"",
}
do
    local base = { __index = _default_values, __newindex = function() error( "Attempt to modify read-only table" ) end } 
    for k,v in pairs(data) do
        setmetatable( v, base )
    end
end
return data