#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"
#include "google/protobuf/message.h"

namespace hld
{
	class player;
	class npc;
	enum e_map_change_type;
	enum e_pk_mode_type;
	struct MapTemplate;
	struct base_map_component;

	class base_map_system
	{
	public:

		static void start_up(Entity* map_ent, int32 line_id, MapTemplate* map_template);
		static void shut_down(Entity* entity);
	public:
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
		static void on_unit_be_damage(Entity* entity, const int32& attack_index, const int32& be_attack_index, double damage_value);
	public:
		static int32 get_map_template_id(Entity* entity);
		static int32 get_map_template_id(const guid_64& map_guid);
		static int32 get_map_type(Entity* entity);
		static int32 get_map_type(const guid_64& map_guid);
		static int32 get_map_sub_type(Entity* entity);
		static int32 get_map_sub_type(const guid_64& map_guid);
		static MapTemplate* get_map_template(Entity* entity);
		static MapTemplate* get_map_template(const guid_64& map_guid);
	public:
		static void map_settlement(Entity* entity, MapTemplate* map_template);
		static void transfer_all_player(Entity* entity, int32 map_template_id = 0);
		static void remove_all_monster(Entity* entity);
		static void kill_all_monster(Entity* entity, int32 player_index);
		static void add_map_buff(base_map_component& base_map_cp, int32 unit_index);
		static void add_vip_map_buff(base_map_component& base_map_cp, int32 unit_index);
		static void set_default_pk_mode(base_map_component& base_map_cp, int32 unit_index);
	public:
		static void send_map_message(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header);
	public:
		static void map_change_log(base_map_component& base_map_cp, int32 unit_index, e_map_change_type chang_type);
	public:
		static void gm_show_all_npc(Entity* map_ent);

	};
}
