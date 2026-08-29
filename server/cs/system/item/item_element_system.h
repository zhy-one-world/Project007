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
	class element_heart_proto_element_heart_operate;
	class player;

	class item_element_system
	{
	public:
		static void start_up(Entity* item_ent, ItemTemplate* item_template, ElementHeartTemplate* sprite_template, const item_s2s_item_data& msg);
		static void shut_down(Entity* item_ent);
		static void heart_tick(const int64& new_time);
		static void save_item_to_db(Entity* item_ent, item_s2s_item_data* msg);
	public:
		static void dynamic_element(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void static_element(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void generate_client_msg(Entity* item_ent, item_s2c_item_data* msg);
		static void change_element_att(unit* unit_ptr, Entity* item_ent, bool is_add);
	private:
		static Entity* get_element_by_slot(player* player_ptr, int32 equip_slot);
	public:
		static void element_operate_message(player* player_ptr, const element_heart_proto_element_heart_operate& msg);
		static void element_upgrade(player* player_ptr, Entity* item_ent);
		static void equip_on_element(player* player_ptr, Entity* item_ent);
		static void equip_off_element(player* player_ptr, Entity* item_ent, bool send_client = true);
		static void element_recovery(player* player_ptr, const std::vector<Entity*>& item_ent_array);
		static void element_merge(player* player_ptr, Entity* first_item_ent, Entity* second_item_ent);
	};
}
