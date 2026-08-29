#pragma once

#include "base/ecs_world.h"

namespace faith
{
	class npc;
	struct npc_refrush_component;

	class npc_refrush_system
	{
	public:
		static void start_up(Entity* map_ent);
		static void shut_down(Entity* map_ent);
		static void heart_tick(const int64& new_time);
	public:
		static void on_npc_dead(Entity* map_ent, npc* npc_ptr);
		static void on_map_settlement(Entity* map_ent);
	private:
		static void refresh_npc(int64 new_time, Entity* map_ent, npc_refrush_component& nr_cp);
	};
}
