local resigter_data  = {}


function resigter_data:init()
	-- body
end

--RegistSingleton("time_mgr", true);

RegistSingleton("red_package_mgr", true);



RegistClass("red_package")



RegistSingleton("arena_mgr", true)


RegistSingleton("rename_mgr", false)


RegistSingleton("assist_fight_mgr", false)


RegistSingleton("auction_mgr", true)


RegistSingleton("gain_treasure_mgr", false)


RegistSingleton("best_record_mgr", false)


RegistSingleton("big_player_mgr", true)


RegistClass("score_indicator")




function is_red_use_lua()
 	return false
end 

function is_arena_use_lua()
	return false
end

function is_rename_use_lua()
	return false
end


function is_assist_use_lua()
	return false
end

function is_auciton_use_lua()
	return false
end

function is_treasure_use_lua()
	return false
end


function is_best_record_use_lua()
	return false
end

function is_big_player_use_lua()
	return false
end



return resigter_data