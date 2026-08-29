#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace faith
{
	struct ItemTemplate;
	struct s_item_template_info;
	class item_s2s_item_data;
	class item_s2s_sl_item_info;
	class item_s2c_item_data;
	class unit;

	class item_base_system
	{
	public:
		static void start_up(unit* unit_ptr, Entity* item_ent, ItemTemplate* item_template, const item_s2s_item_data& msg);
		static void shut_down(Entity* item_ent);
		static void heart_tick(const int64& new_time);
		static void save_item_to_db(Entity* item_ent, item_s2s_item_data* msg);
	public:
		static void generate_base_msg(item_s2s_item_data& msg, const s_item_template_info& item_info);
		static void generate_client_msg(Entity* item_ent, item_s2c_item_data* msg);
	};
}
