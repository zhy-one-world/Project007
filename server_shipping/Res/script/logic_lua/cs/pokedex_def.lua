local pokedex_def = {};
function pokedex_def:init()
end

pokedex_def.POKEDEX_PSYCHE_MAX          = 20;
pokedex_def.POKEDEX_NUM_MAX             = 80;
pokedex_def.tinder_old_num_max          = 30;													-- 守护印记装备栏上限
pokedex_def.tinder_item_max             = 10;													-- 火种道具上限
pokedex_def.tinder_order_num_one_canto  = 6;
pokedex_def.tinder_new_order_max        = 34;
pokedex_def.tinder_canto_num_max        = 4;
pokedex_def.tinder_level_start_id       = 65010001;
pokedex_def.tinder_old_end_id           = 65000030;
pokedex_def.tinder_unlock_start_id      = 65000001;

PokedexDef = pokedex_def;
return PokedexDef;