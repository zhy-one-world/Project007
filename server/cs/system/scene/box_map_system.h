#pragma once

#include "base/ecs_world.h"

namespace hld
{
	const int32 g_box_born_group_len = 4;//宝箱出生点组长度
	class map_s2s_box_map_info;
	class player;
	class npc;
	struct MapTemplate;
	struct box_map_component;

	class box_map_system
	{
	public:
		static void start_up(Entity* map_ent, MapTemplate* map_template);
		static void shut_down(Entity* map_ent);
		static void heart_tick(const int64& new_time);
	public:

		static void on_player_save(Entity* map_ent, map_s2s_box_map_info* msg);
		static void on_player_load(Entity* map_ent, map_s2s_box_map_info* msg);
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
		static void begin_kill_boss(Entity* map_ent, player* player_ptr);
		static void get_npc_award(Entity* map_ent, player* player_ptr, e_award_mark_type mark_type, int32 spawn_id);
		static npc* get_npc_by_level(Entity* map_ent, e_award_mark_level award_level);
		static void kill_award_npc(Entity* map_ent, player* player_ptr, npc* npc_ptr);
		static void clear_multiple_treasure(Entity* map_ent, player* player_ptr);
		static int64 open_treasure_pic(Entity* map_ent, player* player_ptr);
		static void open_new_map(Entity* map_ent, player* player_ptr);
		static void refresh_award(Entity* map_ent, player* player_ptr);
		static int64 get_next_award_npc(Entity* map_ent, player* player_ptr);
	private:
		static void refresh_monster(Entity* map_ent, player* player_ptr);
		static void refresh_nomal_award(Entity* map_ent, box_map_component& box_map_cp, player* player_ptr);
		static npc* refresh_big_award(Entity* map_ent, player* player_ptr, int32 spawn_id, int32 npc_id);
		static int32 get_born_list(Entity* map_ent);
		static void map_settlement(Entity* map_ent);
	};
}
