local init_unit = {}
function init_unit:init()
end

function get_map_need_add_hate(map_type)
	return not ((map_type == hld.e_map_type_king_of_pk) or
		        (map_type == hld.e_map_type_cross_server_pk))
	-- body
end

function is_levelup_add_hp(map_type)
	return not (map_type == hld.e_map_type_cross_server_pk)
end


function is_send_clear_title_with_clear_big_player( big_player)
	return  ((big_player == hld.e_big_player_type_lord_or_war) or
		     (big_player == hld.e_big_player_type_best_barserker) or
		     (big_player == hld.e_big_player_type_best_wizard) or 
		     (big_player == hld.e_big_player_type_best_guardian) or 
		     (big_player == hld.e_big_player_type_best_assassinator))
end

function is_cross_server_title(title_type, is_begin_cross_activity)
	 if is_begin_cross_activity == true and title_type == hld.e_title_type_kingofpk  then
	 	return true
	 end 
	 if ((title_type == hld.e_title_type_best_barserker) or
	     (title_type == hld.e_title_type_best_wizard) or 
	     (title_type == hld.e_title_type_best_guardian) or
	     (title_type == hld.e_title_type_best_assassinator)) then
	    return true
	end
	return false
end

function is_cross_server_big_player(player_type, is_begin_cross_activity)
	if is_begin_cross_activity == true and player_type == hld.e_big_player_type_lord_or_war then
		return true
	end
	if ((player_type == hld.e_big_player_type_lord_or_war) or
		(player_type == hld.e_big_player_type_best_wizard) or
		(player_type == hld.e_big_player_type_best_guardian) or 
		(player_type == hld.e_big_player_type_best_assassinator)) then
	   return true
	end 
	return false
end

function get_map_public(map_type)
	return ((map_type == hld.e_map_type_big_map) or 
		    (map_type == hld.e_map_type_new) or
		    (map_type == hld.e_map_type_main) or 
		    (map_type == hld.e_map_type_field) or
		    (map_type == hld.e_map_type_crystak_dreamland) or 
		    (map_type == hld.e_map_type_boss_home) or 
		    (map_type == hld.e_map_type_boss_vip_home) or 
		    (map_type == hld.e_map_type_boss_island) or 
		    (map_type == hld.e_map_type_belief_cloister) or 
		    (map_type == hld.e_map_type_cross_server_pk) or 
		    (map_type == hld.e_map_type_cross_server_harry))
end

function  is_cross_server_npc(npc_type)
	 return ((npc_type == hld.e_npc_type_island_big_chests) or 
	 	     (npc_type == hld.e_npc_type_island_small_chests) or 
	 	     (npc_type == hld.e_npc_type_cross_pk_chests) or 
	 	     (npc_type == hld.e_npc_type_cross_pk_chests))
end

function get_map_level_suppress(map_type)
	return ((map_type == hld.e_map_type_big_map) or 
		    (map_type == hld.e_map_type_new) or 
		    (map_type == hld.e_map_type_main) or 
		    (map_type == hld.e_map_type_field) or
		    (map_type == hld.e_map_type_crystak_dreamland) or 
		    (map_type == hld.e_map_type_boss_home) or
		    (map_type == hld.e_map_type_boss_vip_home) or 
		    (map_type == hld.e_map_type_boss_island))
end


function is_team_exp_map(map_type)
     return  ((map_type == hld.e_map_type_field) or
	         (map_type == hld.e_map_type_big_map) or
	         (map_type == hld.e_map_type_main) or
	         (map_type == hld.e_map_type_new) or
	         (map_type == hld.e_map_type_legion_boss) or
	         (map_type == hld.e_map_type_broken_sky) or
	         (map_type == hld.e_map_type_belief_cloister) or
	         (map_type == hld.e_map_type_princess_guard_war) or
	         (map_type == hld.e_map_type_lava_hellfire) or
	         (map_type == hld.e_map_type_sea_of_hate) or 
	         (map_type == hld.e_map_type_temple)) 
end

function get_map_gain_treasure_multiplayer(map_type)
	 return ((map_type == hld.e_map_type_boss_home) or
	 	     (map_type == hld.e_map_type_boss_vip_home) or 
	 	     (map_type == hld.e_map_type_boss_island))

end

function  get_map_open_new(map_type)
	 return ((map_type == hld.e_map_type_big_map) or
	 	     (map_type == hld.e_map_type_new) or 
	 	     (map_type == hld.e_map_type_main) or 
	 	     (map_type == hld.e_map_type_field) or 
	 	     (map_type == hld.e_map_type_broken_sky) or
	 	     (map_type == hld.e_map_type_temple) or 
	 	     (map_type == hld.e_map_type_empire_treasure) or 
	 	     (map_type == hld.e_map_type_king_of_pk) or 
	 	     (map_type == hld.e_map_type_crystak_dreamland) or 
	 	     (map_type == hld.e_map_type_boss_vip_home) or
	 	     (map_type == hld.e_map_type_boss_home) or 
	 	     (map_type == hld.e_map_type_boss_island) or 
	 	     (map_type == hld.e_map_type_belief_cloister))
end

function  get_map_create_full(map_type)
	return ((map_type == hld.e_map_type_temple) or
		    (map_type == hld.e_map_type_empire_treasure) or
		    (map_type == hld.e_map_type_broken_sky) or
		    (map_type == hld.e_map_type_belief_cloister))
end

function  get_map_hold(map_type)
	return ((map_type == hld.e_map_type_big_map) or
		    (map_type == hld.e_map_type_new) or 
		    (map_type == hld.e_map_type_main) or 
		    (map_type == hld.e_map_type_field) or 
		    (map_type == hld.e_map_type_crystak_dreamland) or
		    (map_type == hld.e_map_type_legion_boss) or 
		    (map_type == hld.e_map_type_city_war) or 
		    (map_type == hld.e_map_type_king_of_pk) or 
		    (map_type == hld.e_map_type_legion_bonfire) or 
		    (map_type == hld.e_map_type_boss_island) or 
		    (map_type == hld.e_map_type_belief_cloister) or 
		    (map_type == hld.e_map_type_cross_server_pk) or 
		    (map_type == hld.e_map_type_broken_sky) or 
		    (map_type == hld.e_map_type_cross_server_harry))
	-- body
end

function  is_map_gain_treasure(map_type)
	return ((map_type == hld.e_map_type_boss_home) or
		    (map_type == hld.e_map_type_boss_vip_home))
end

function is_map_boss_island(map_type)
	return (map_type == hld.e_map_type_boss_island)
end


function is_map_belief_cloister(map_type)
	return (map_type == hld.e_map_type_belief_cloister)
end

function is_boss(boss_type)
	return ((boss_type == hld.e_monster_type_boss) or
		    (boss_type == hld.e_monster_type_world_boss) or
		    (boss_type == hld.e_monster_type_gold_army))
end

function is_world_boss(boss_type)
	return ((boss_type == hld.e_monster_type_world_boss) or 
		    (boss_type == hld.e_monster_type_gold_army))
end

function is_goto_same_map(map_id_to, line_id_to,  map_id_cur, line_id_cur)
	if map_id_cur ~= map_id_to then
		return false
	end
	if line_id_to ~= 0 and line_id_cur ~= line_id_to then
		return false
	end
	return true
end

function is_fake_player(unit_type, sub_type)
	if ((unit_type == hld.e_unit_type_npc) and 
		(sub_type == hld.e_npc_type_lord_city or 
		 sub_type == hld.e_npc_type_lord_pk or 
		 sub_type == hld.e_npc_type_lord_sanctuary or
		 sub_type == hld.e_npc_type_lord_god)) then
	     return true
	end
	if ((unit_type == hld.e_unit_type_monster) and (sub_type == hld.e_monster_type_player_data)) then
		return true
	end
	return false
end


function is_equip_type(item_type)
	return ((item_type == hld.e_item_type_weapon) or
		    (item_type == hld.e_item_type_armor) or
		    (item_type == hld.e_item_type_fashion))
end

function is_spirit_type(item_type)
	return item_type == hld.e_item_type_spirit
end


function is_can_use_this_jewel_slot_on_this_equip_level(equip_level,slot_index)
		local array_size = 0;
		if slot_index < hld.e_item_info_jewel_slot_0
			or slot_index >= hld.e_item_info_jewel_vip_slot_0 then
		
			return false;
		end
		local temp_index = slot_index - hld.e_item_info_jewel_slot_0;
		local game_config_ptr = template_manager:get_template("GameConfigTemplate")
		if game_config_ptr == nil then
			return false;
		end
		array_size = #game_config_ptr.JewelSlotReqEquipLevel
		if temp_index >= array_size then
			return false;
		end
		return equip_level >= game_config_ptr.JewelSlotReqEquipLevel[temp_index];
end

function is_can_use_this_jewel_slot_on_this_vip_level(role_vip_level, slot_index)
	if slot_index < hld.e_item_info_jewel_vip_slot_0 or slot_index > hld.e_item_info_jewel_vip_slot_1 then
		return false
	end
	local temp_index = slot_index - hld.e_item_info_jewel_vip_slot_0;
	local vip_template_ptr = template_manager:get_template_by_vip_level(role_vip_level);
	if vip_template_ptr == nil then
		return false
	end
	local vip_template = vip_template_ptr;
	return vip_template_ptr.ExtraJewelSlotNum > temp_index;	
end


function is_team_type_raid_map(team_type)

	return	((team_type == hld.e_team_type_raid_daily_exp) or
		     (team_type == hld.e_team_type_raid_daily_gold) or
		     (team_type == hld.e_team_type_raid_multi_0) or
		     (team_type == hld.e_team_type_raid_multi_1) or
		     (team_type == hld.e_team_type_raid_multi_2) or
		     (team_type == hld.e_team_type_raid_sky) or
		     (team_type == hld.e_team_type_raid_temple) or
		     (team_type == hld.e_team_type_raid_treasure) or
		     (team_type == hld.e_team_type_ancient_hang_up)) 
end


return init_unit;