local type	= type;
local print	= print;
local pairs	= pairs;
local ipairs= ipairs;
local setmetatable		= setmetatable;
local table_insert		= table.insert;

local class_faith = {}
function class_faith:init()
end

local fEmpty = function() end;
local tClassS = {};

local registclass = function(i_sClassName)
	if type(i_sClassName) == "string" then
		if tClassS[i_sClassName] then
			print("ERROR!!! class already exist!!!", i_sClassName);
			print(debug.traceback());
			return;
		end;
		local objClass = {};
		objClass._constructor	= fEmpty;
		objClass.__index		= objClass;
		tClassS[i_sClassName] = objClass;
		return objClass;
	else
		print("ERROR!!! regist class name type ERROR!!!", i_sClassName);
		print(debug.traceback());
	end;
end;

local requireclass = function(i_sClassName)
	local oClass = tClassS[i_sClassName];
	if not oClass then
		print("ERROR!!! require class not exist!!!", i_sClassName);
		print(debug.traceback());
	end;
	return oClass;
end;

local newclass = function(i_sClassName, ...)
	local oClass = requireclass(i_sClassName);
	if oClass then
		local object = {};
		setmetatable(object, oClass);
		object:_constructor(...);
		return object;
	end;
end;

local inheritclass = function(i_sClassNameDerive, i_sClassNameBase)
	local object = registclass(i_sClassNameDerive);
	if object then
		local oClass = requireclass(i_sClassNameBase);
		if oClass then
			object._super = oClass;
			setmetatable(object, oClass);
		end;
		return object;
	end;
end;

local findkey = function(object, key)
	local superlist = object._superlist;
	return superlist[1][key] or superlist[2][key] or superlist[3][key] or superlist[4][key];
 	-- for _, super in ipairs(object._superlist) do
 	-- 	if super[key] then
 	-- 		return super[key];
 	-- 	end;
 	-- end;
end;
local t = {__index = findkey};
local inheritclasses = function(i_sClassNameDerive, i_tClassNameBase)
	if #i_tClassNameBase == 0 then
		print("ERROR!!! inheritclasses bases table is empty!!!");
		print(debug.traceback());
		return;
	end;
	local object = registclass(i_sClassNameDerive);
	if object then
		local superlist = {};
		for _, sClassNameBase in ipairs(i_tClassNameBase) do
			local oClass = requireclass(sClassNameBase);
			if oClass then
				table_insert(superlist, oClass);
			end;
		end;
		object._superlist = superlist;
		setmetatable(object, t);
		return object;
	end;
end;

RegistClass		= registclass;
RequireClass	= requireclass;
NewClass		= newclass;
InheritClass	= inheritclass;
InheritClasses	= inheritclasses;


---------------------------------------------------------------------

local tName2Singleton	= {};
local tSingletonSet		= {};
local tUpdateSet        = {};



-------------------------------------------------------

RegistSingleton = function(i_sSingletonName, i_bUpdate)
	if type(i_sSingletonName) == "string" then
		if tName2Singleton[i_sSingletonName] then
			print("ERROR!!! regist singleton repeat.", i_sSingletonName);
		else
			local singleton = {_name = i_sSingletonName};
			tName2Singleton[i_sSingletonName] = singleton;
			table_insert(tSingletonSet, singleton);
			if i_bUpdate then
				table_insert(tUpdateSet, singleton);
			end
			return singleton;
		end		
	else
		print("ERROR!!! regist singleton type ERROR!!!", i_sSingletonName);
		print(debug.traceback());
	end
end


RequireSingleton = function(i_sSingletonName, i_bIgnore)
    --delog("---------RequireSingleton-------", i_sSingletonName)
	local oSingleton = tName2Singleton[i_sSingletonName];
	if not oSingleton and not i_bIgnore then
		print("ERROR!!! require singleton ERROR!!!", i_sSingletonName);
		print(debug.traceback());
	end
	return oSingleton;
end


local g_time_msec = 0

local last_time = 0;

UpdateSingleton = function(i_nTime)
    g_time_msec = i_nTime
    --delog("----UpdateSingleton---", i_nTime)
	local curtime = g_time_msec
	if  last_time == 0 then
		last_time = g_time_msec
	end
	local delta = curtime - last_time;
   	last_time = curtime;
	if delta <= 0 then
	      return
	end
	for _, v in ipairs(tUpdateSet) do
	      ProtectedCall(function() v:Update(delta) end);
	end

end


function get_g_time_mesc()
	return g_time_msec
end

function get_end_time(add_sec)
	 local now_msec = g_time_msec
	 local end_time = now_msec + add_sec * 1000
	 return end_time
end

return class_faith