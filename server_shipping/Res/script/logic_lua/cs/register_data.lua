local resigter_data  = {}


function resigter_data:init()
	-- body
end

RegistSingleton("time_mgr", true);

RegistSingleton("player_mgr", true);



RegistClass("unit")

InheritClass("player", "unit")

RegistClass("dragontrip")






return resigter_data