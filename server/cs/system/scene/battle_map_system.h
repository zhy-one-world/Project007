#pragma once

#include "base/ecs_world.h"

namespace hld
{
	class player;
	class npc;
	class map_s2s_battle_map_info;
	struct MapTemplate;

	class battle_map_system
	{
	public:
		static void start_up(Entity* map_ent, MapTemplate* map_template);
		static void shut_down(Entity* map_ent);
		static void heart_tick(const int64& new_time);
	public:
		static void on_player_save(Entity* map_ent, map_s2s_battle_map_info* msg);
		static void on_player_add(Entity* entity, player* player_ptr);
		static void on_player_delete(Entity* entity, player* player_ptr);
		static void on_player_load_data_over(Entity* map_ent, player* player_ptr);
		static void on_player_enter_scene(Entity* entity, player* player_ptr);
		static void on_player_leave_scene(Entity* entity, player* player_ptr);
		static void on_player_dead(Entity* entity, player* player_ptr);
		static void on_player_reconnect(Entity* entity, player* player_ptr);
		static void on_npc_add(Entity* entity, npc* npc_ptr);
		static void on_npc_dead(Entity* entity, npc* npc_ptr);
		static void on_npc_delete(Entity* entity, npc* npc_ptr, bool is_dead);
		static void on_unit_be_damage(Entity* map_ent, const int32& attack_index, const int32& be_attack_index, double damage_value);
	public:
		static void mop_up_map(player* player_ptr, int32 map_id);
	};
}
