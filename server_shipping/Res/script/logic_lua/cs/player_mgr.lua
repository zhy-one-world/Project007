
local unit = RequireClass("unit")


function unit:_constructor(i_unit_index, i_role_id)
	self.m_unit_index = i_unit_index
	self.m_role_id = i_role_id
end


function unit:get_unit_index()
	return self.m_unit_index
end



function unit:Update()
   --delog("---player-unit-", self.m_unix_index, self.m_role_id)
end




local cplayer = RequireClass("player")


function cplayer:_constructor(i_unit_index, i_role_id)
	self._super._constructor(self, i_unit_index, i_role_id)
	
	self.m_tsystem = {}
	self.m_tsystem["dragontrip"] = NewClass("dragontrip", self)

	-----------------------------------------------
	local player_ref = hld.get_player_by_index(self.m_unit_index)
	if player_ref == nil then
	 	print("--------------create---player--error")
	 	return
	end
	
	

	self.m_cpp_obj = player_ref

end

function cplayer:get_system(i_name)
	local t_sys =  self.m_tsystem[i_name]

	--delog(t_sys, "---cplayer:get_system-")

	return t_sys
end


function cplayer:get_cpp_obj( )
     return self.m_cpp_obj
end



local t_update_list =
{
    "dragontrip"
}

function cplayer:Update()
	 self._super.Update(self)

	 --delog("---player--", self.m_unix_index, self.m_role_id)
	 for i = 1, #t_update_list do
	     local name = t_update_list[i]
	     self:get_system(name):Update()
	 end
end



function cplayer:save_to_db(save_type)
	 
	 for k, v in pairs(self.m_tsystem) do
	 	  v:save_to_db(save_type)
	 end
end





local player_mgr =  RequireSingleton("player_mgr")



function player_mgr:init()
	if is_init() then
	 	return
	end

    delog("-----player_mgr--init-----")
    self.m_tIDplayer = {}
    self.m_tGuidPlayer = {}
end



function player_mgr:create_player(unit_index, role_id)
	 local oPlayer = NewClass("player", unit_index, role_id)
	 self.m_tIDplayer[unit_index] = oPlayer
	 self.m_tGuidPlayer[role_id] = oPlayer
end

function player_mgr:del_player(unit_index)
	local oPlayer = self.m_tIDplayer[unit_index]
	if oPlayer == nil then
		return
	end

	local role_id = oPlayer.m_role_id

	self.m_tIDplayer[unit_index] = nil


	local g_oPlayer =  self.m_tGuidPlayer[role_id]
	if g_oPlayer:get_unit_index() ~= unit_index then
        return 
    end
    self.m_tGuidPlayer[role_id] = nil
end


function player_mgr:save_player(unit_index, save_type)
	local oPlayer = self.m_tIDplayer[unit_index]
	oPlayer:save_to_db(save_type)
end


function player_mgr:find_player(unit_index)
	return self.m_tIDplayer[unit_index]
end



function player_mgr:Update( )
	for k, v in pairs(self.m_tIDplayer) do
		v:Update()
	end
end


function create_player(unit_index, user_id)
	 delog("-----create_player---")
	 player_mgr:create_player(unit_index, user_id)
end


function del_player(unit_index)
	delog("----del_player--", unit_index)
	player_mgr:del_player(unit_index)
end


function save_player(unit_index, save_type)
	delog("----save_player--", unit_index)
    player_mgr:save_player(unit_index, save_type)
end


return player_mgr
