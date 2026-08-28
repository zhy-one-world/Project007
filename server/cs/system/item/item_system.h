#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace hld
{
	class unit;
	class player;
	class item_s2s_sl_item_info;
	class item_s2c_item_data;
	class item_proto_item_operation;
	class item_s2s_item_data;
	struct s_item_template_info;
	struct ItemTemplate;

	class item_system
	{
	public:
		static void heart_tick(const int64& new_time);
	public:
		static void load_data_from_db(unit* unit_ptr, const item_s2s_sl_item_info& msg, e_bag_type bag_type);
		static void load_data_over(unit* unit_ptr);
		static void save_data_to_db(player* player_ptr, int32 save_type_ex);
		static void clear_data(unit* unit_ptr);
	public:
		static void send_promp_msg_to_client(player* player_ptr, const std::vector<s_item_template_info>& items_array, const std::vector<s_item_template_info>& money_array = {});		//发送获得的物品弹框
		static void send_item_one(unit* unit_ptr, const std::vector<Entity*>& item_ent_array, bool to_self = true);
		static void send_item_all(unit* unit_ptr);
		static void send_item_del(unit* unit_ptr, const std::vector<guid_64>& item_guid_array);
	public:
		static Entity* create_item(unit* unit_ptr, const s_item_template_info& item_info);
		static void del_item(Entity* item_ent);
		static std::vector<Entity*> take_out_item(unit* unit_ptr, e_bag_type bag_type);
		static void put_item_into_bag(unit* unit_ptr, Entity* item_ent, e_bag_type bag_type = e_bag_type_none);
		static void put_item_into_bag(unit* unit_ptr, const std::vector<s_item_template_info>& item_array);
		static void put_item_into_bag(unit* unit_ptr, const std::vector<Entity*>& item_array, e_bag_type bag_type = e_bag_type_none);
		static void remove_item_from_bag(unit* unit_ptr, Entity* item_ent);
		static bool can_cost_item(unit* unit_ptr, e_bag_type bag_type, int32 item_id, int32 item_num, e_item_lock lock_state = e_item_lock_all);
		static void cost_item_from_bag(unit* unit_ptr, e_bag_type bag_type, int32 item_id, int32 item_count, e_item_lock first_use = e_item_lock_lock);
		static void cost_item_by_ent(Entity* item_ent, int32 cost_num = 0);
	public:
		static Entity* get_item_by_id(unit* unit_ptr, e_bag_type bag_type, int32 item_id);
		static int32 get_item_count(unit* unit_ptr, e_bag_type bag_type, int32 item_id, e_item_lock lock_state = e_item_lock_all);
	private:
		static Entity* create_item(unit* unit_ptr, const item_s2s_item_data& msg);
		static void del_item_entity(Entity* item_ent);
		static void create_item_msg(item_s2s_item_data& msg, unit* unit_ptr, const s_item_template_info& item_info, ItemTemplate* item_template);
		static void put_item_into_bag(unit* unit_ptr, const std::vector<item_s2s_item_data>& item_array);
		static void generate_client_msg(Entity* item_ent, item_s2c_item_data* msg);
		static std::vector<s_item_template_info> spread_item_by_max_num(unit* unit_ptr, const std::vector<s_item_template_info>& item_info_array);
		static void open_auto_use_item(std::vector<s_item_template_info>& res_item_list, unit* unit_ptr, const s_item_template_info& item_info, ItemTemplate* item_template);
	public:
		static void item_operate_message(player* player_ptr, const item_proto_item_operation& msg);
		static void clear_bag(unit* unit_ptr, e_bag_type bag_type);
	};
}
