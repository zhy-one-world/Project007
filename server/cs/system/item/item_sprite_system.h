#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace hld
{
	struct ItemTemplate;
	class item_s2s_item_data;
	class item_s2s_sl_item_info;
	class item_s2c_item_data;
	class player;

	class item_sprite_system
	{
	public:
		static void start_up(Entity* item_ent, ItemTemplate* item_template, SpiritTemplate* sprite_template, const item_s2s_item_data& msg);
		static void shut_down(Entity* item_ent);
		static void heart_tick(const int64& new_time);
		static void save_item_to_db(Entity* item_ent, item_s2s_item_data* msg);
	public:
		static void dynamic_sprite(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void static_sprite(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void generate_client_msg(Entity* item_ent, item_s2c_item_data* msg);
		static void change_sprite_att(unit* unit_ptr, Entity* item_ent, bool is_add);
	private:
		static Entity* get_sprite_by_slot(player* player_ptr, int32 equip_slot);
	public:
		static void activate_sprite(player* player_ptr, int32 item_id);
		static void sprite_level_up(player* player_ptr, Entity* item_ent);
		static void equip_on_sprite(player* player_ptr, Entity* item_ent, int32& target_slot);
		static void equip_off_sprite(player* player_ptr, Entity* item_ent, bool send_client = true);
		static void set_scene_show(player* player_ptr, Entity* item_ent, int32 target_slot);
	};
}
