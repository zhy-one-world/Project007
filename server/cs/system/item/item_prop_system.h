#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace hld
{
	struct ItemTemplate;
	struct PropBasicAttributeTemplate;
	struct s_item_template_info;
	class item_s2s_item_data;
	class item_s2s_sl_item_info;
	class item_s2c_item_data;
	class player;

	class item_prop_system
	{
	public:
		static void start_up(Entity* item_ent, ItemTemplate* item_template, PropBasicAttributeTemplate* prop_template, const item_s2s_item_data& msg);
		static void shut_down(Entity* item_ent);
		static void heart_tick(const int64& new_time);
		static void save_item_to_db(Entity* item_ent, item_s2s_item_data* msg);
	public:
		static void dynamic_prop(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void static_prop(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void generate_client_msg(Entity* item_ent, item_s2c_item_data* msg);
	public:
		static void item_use(player* player_ptr, Entity* item_ent, int32 use_num = 0);
	};
}
