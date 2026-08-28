#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace hld
{
	struct ItemTemplate;
	struct EquipTemplate;
	struct s_item_template_info;
	struct item_equip_component;
	class item_s2s_item_data;
	class item_s2c_item_data;
	class player;
	class unit;

	class item_equip_system
	{
	public:
		static void start_up(Entity* item_ent, ItemTemplate* item_template, EquipTemplate* equip_template, const item_s2s_item_data& msg);
		static void shut_down(Entity* item_ent);
		static void heart_tick(const int64& new_time);
		static void save_item_to_db(Entity* item_ent, item_s2s_item_data* msg);
	public:
		static void dynamic_equip(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void static_equip(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void generate_client_msg(Entity* item_ent, item_s2c_item_data* msg);
		static void change_equip_att(unit* unit_ptr, Entity* item_ent, bool is_add);
		static Entity* get_equip_item(unit* unit_ptr, e_role_equip_slot equip_slot);
	public:
		static void equip_on_equip(unit* unit_ptr, Entity* item_ent, int32& target_slot);
		static void equip_off_equip(unit* unit_ptr, Entity* item_ent, bool send_info = true);
		static void equip_recovery(player* player_ptr, const std::vector<Entity*>& item_ent);
	};
}
