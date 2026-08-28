#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace hld
{
	class npc;
	struct MapTemplate;
	struct base_map_component;
	struct wave_monster_component;

	class wave_monster_system
	{
	public:
		static void start_up(Entity* map_ent, MapTemplate* map_template);
		static void shut_down(Entity* map_ent);
		static void heart_tick(const int64& new_time);
	public:
		static void init_brush(Entity* map_ent);
		static void on_npc_delete(Entity* map_ent, npc* npc_ptr);
		static bool monster_kill_end(Entity* map_ent);
	private:
		static void refresh_brush(Entity* map_ent);
		static void send_wave_info(Entity* map_ent, const int32& wave_index);
	};
}
