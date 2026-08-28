#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace hld
{
	struct NpcSpawnPointTemplate;
	struct target_time_spawn_info;
	struct target_time_monster_component;

	class permanent_monster_system
	{
	public:
		static void start_up(Entity* map_ent, int32 spawn_point_id);
		static void shut_down(Entity* map_ent);
		static void heart_tick(const int64& new_time);
	public:
		static void on_npc_delete(Entity* map_ent, const int32& unit_index, bool is_dead);
	private:
		static void spawn_by_target_time_sec(int64 new_time, Entity* map_ent, target_time_monster_component& ttm_cp);
		static void add_target_time_monster(Entity* map_ent, const target_time_spawn_info& spawn_info);
		static bool is_in_refresh_time(NpcSpawnPointTemplate* spawn_template_ptr);
		static int64 get_next_spawn_time(NpcSpawnPointTemplate* spawn_template_ptr, bool is_be_kill);
		static int32 get_next_target_time_spawn_index(NpcSpawnPointTemplate* spawn_template_ptr);
	};
}
