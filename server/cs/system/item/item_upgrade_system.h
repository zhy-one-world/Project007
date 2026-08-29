#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	enum e_error_code;
	enum e_role_equip_slot;
	class player;
	class item_s2s_sl_item_upgrade;

	class item_upgrade_system
	{
	public:
		static void start_up(player* player_ptr);
		static void shut_down(player* player_ptr);
		static void heart_tick(const int64& new_time);
		static void load_data_from_db(player* player_ptr, const item_s2s_sl_item_upgrade& msg);
		static void save_data_to_db(player* player_ptr, int32 save_type_ex);
	public:
		static int32 get_item_upgrade_num(player* player_ptr, e_role_equip_slot equip_slot);
		static e_error_code item_upgrade(player* player_ptr);
	public:
		static void send_item_upgrade_num(player* player_ptr, e_role_equip_slot equip_slot, e_error_code res);
	public:
		static int32 get_upgrade_all_count(player* player_ptr);
		static void change_upgrade_att(player* player_ptr, e_role_equip_slot equip_slot, bool is_add);
	};
}
