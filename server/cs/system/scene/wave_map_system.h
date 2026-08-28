#pragma once

#include "base/ecs_world.h"

namespace hld
{
	struct MapTemplate;

	class wave_map_system
	{
	public:
		static void start_up(Entity* map_ent, int32 brush_id);
		static void shut_down(Entity* entity);
		static void heart_tick(const int64& new_time);
	public:
		static void on_player_add(Entity* entity, const int32& unit_index);
		static void on_player_delete(Entity* entity, const int32& unit_index);
		static void on_player_load_data_over(Entity* map_ent, const int32& unit_index);
		static void on_player_enter_scene(Entity* entity, const int32& unit_index);
		static void on_player_leave_scene(Entity* entity, const int32& unit_index);
		static void on_player_dead(Entity* entity, const int32& unit_index);
		static void on_player_reconnect(Entity* entity, const int32& unit_index);
		static void on_npc_add(Entity* entity, const int32& unit_index);
		static void on_npc_dead(Entity* entity, const int32& unit_index);
		static void on_npc_delete(Entity* entity, const int32& unit_index, bool is_dead);
		static void on_unit_be_damage(Entity* entity, const int32& attack_index, const int32& be_attack_index, double damage_value);
	public:
		static int32 kill_boss(Entity* entity);
		static void kill_cur_wave(Entity* entity);
	private:
		static void refresh_brush(Entity* entity);
		static void refresh_wave(Entity* entity, int32 wave_id, int32 wave_index);
		static void refresh_boss(Entity* entity);
		static void send_refresh_boss(Entity* entity, int32 res);
	};
}
