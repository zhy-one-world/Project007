#include "components/item/item_manager_component.h"
#include "components/item/item_upgrade_component.h"
#include "internal/char_msg.hpp"
#include "internet/error.pb.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "item_equip_system.h"
#include "item_upgrade_system.h"
#include "logic/player.hpp"

#include "server_log.hpp"

using namespace faith;


void item_upgrade_system::start_up(player* player_ptr)
{
	ZoneScoped;
	return;
}
void item_upgrade_system::shut_down(player* player_ptr)
{
	ZoneScoped;
	player_ptr->m_item_upgrade_component = nullptr;
}
void item_upgrade_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	//g_ecs->each<wave_monster_tick_component>([&](Entity* map_ent, ComponentHandle<wave_monster_tick_component> wmt_cp) -> bool {
	//	refresh_brush(map_ent);
	//	return true;
	//});
}

void item_upgrade_system::load_data_from_db(player* player_ptr, const item_s2s_sl_item_upgrade& msg)
{
	ZoneScoped;
	player_ptr->m_item_upgrade_component = std::make_shared<item_upgrade_component>();
	memset(player_ptr->m_item_upgrade_component->m_upgrade_data, 0, sizeof(player_ptr->m_item_upgrade_component->m_upgrade_data));
	for (int32 i = 0; i < e_role_equip_slot_max && i < msg.data_array_size(); ++i)
	{
		player_ptr->m_item_upgrade_component->m_upgrade_data[i] = msg.data_array(i);
	}
	send_item_upgrade_num(player_ptr, e_role_equip_slot_weapon_1, e_error_code_success);
}
void item_upgrade_system::save_data_to_db(player* player_ptr, int32 save_type_ex)
{
	ZoneScoped;
	auto& item_upgrade_cp = player_ptr->m_item_upgrade_component;
	if (nullptr == item_upgrade_cp)
	{
		CONSOLE_ERROR("item_upgrade_cp is nullptr player_guid:{}", player_ptr->get_unit_guid().server_64);
		return;
	}
	item_s2s_sl_item_upgrade msg;
	for (int32 i = 0; i < e_role_equip_slot_max; ++i)
	{
		msg.add_data_array(item_upgrade_cp->m_upgrade_data[i]);
	}
	player_ptr->send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_item_upgrade, save_type_ex);

}
int32 item_upgrade_system::get_item_upgrade_num(player* player_ptr, e_role_equip_slot equip_slot)
{
	ZoneScoped;
	if (equip_slot < e_role_equip_slot_weapon_1 || equip_slot >= e_role_equip_slot_max)
	{
		CONSOLE_ERROR("equip_slot is invalid equip_slot:{}", (int32)equip_slot);
		return 0;
	}
	auto item_upgrade_cp = player_ptr->m_item_upgrade_component;
	if (nullptr == item_upgrade_cp)
	{
		CONSOLE_ERROR("item_upgrade_cp is nullptr player_guid:{}", player_ptr->get_unit_guid().server_64);
		return 0;
	}
	return item_upgrade_cp->m_upgrade_data[equip_slot];
}
e_error_code item_upgrade_system::item_upgrade(player* player_ptr)
{
	ZoneScoped;
	auto item_upgrade_cp = player_ptr->m_item_upgrade_component;
	if (nullptr == item_upgrade_cp)
	{
		CONSOLE_ERROR("item_upgrade_cp is nullptr player_guid:{}", player_ptr->get_unit_guid().server_64);
		return e_error_code_no_component;
	}
	e_role_equip_slot min_slot = e_role_equip_slot_weapon_1;
	int32 min_upgrade_count = int32_MAX_NUM;
	for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_amulet; i++)
	{
		if (item_upgrade_cp->m_upgrade_data[i] < min_upgrade_count)
		{
			min_upgrade_count = item_upgrade_cp->m_upgrade_data[i];
			min_slot = e_role_equip_slot(i);
		}
	}
	int32 ret = e_error_code_no_param;
	int32 money_type = 0;
	int32 money_num = 0;
	script_mgr::get_instance().call_func("formula_calculation_mgr", "get_upgrade_money_cost", 2, false, "%d%d>%d%d", min_slot, item_upgrade_cp->m_upgrade_data[min_slot], & money_type, & money_num);

	if (player_ptr->can_cut_money((e_money_type)money_type, money_num) == false)
	{ 
		CONSOLE_ERROR("money not enough failed ret:{} min_slot:{} money_type:{} money_num:{}", ret, int32(min_slot), money_type, money_num);
		return e_error_code_no_money;
	}
	player_ptr->cut_money((e_money_type)money_type, money_num, e_server_log_cut_money_item_upgrade, min_slot);

	auto item_ent = item_equip_system::get_equip_item(player_ptr, min_slot);
	if (nullptr == item_ent)
	{
		item_upgrade_cp->m_upgrade_data[min_slot] += 1;
		send_item_upgrade_num(player_ptr, min_slot, e_error_code_success);
		return e_error_code_success;
	}

	change_upgrade_att(player_ptr, min_slot, false);
	item_upgrade_cp->m_upgrade_data[min_slot] += 1;
	change_upgrade_att(player_ptr, min_slot, true);

	player_ptr->get_mission_mgr().target_check(e_mission_end_type_upgrade_total_level);
	send_item_upgrade_num(player_ptr, min_slot, e_error_code_success);
	return e_error_code_success;
}
void item_upgrade_system::send_item_upgrade_num(player* player_ptr, e_role_equip_slot equip_slot, e_error_code res)
{
	ZoneScoped;
	auto item_upgrade_cp = player_ptr->m_item_upgrade_component;
	item_s2c_item_upgrade msg;
	msg.set_res(res);
	msg.set_unit_guid(player_ptr->get_unit_guid().server_64);
	for (int32 i = 0; i < e_role_equip_slot_max; ++i)
	{
		msg.add_data_array(item_upgrade_cp->m_upgrade_data[i]);
	}
	player_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_item_upgrade_data);
}

int32 item_upgrade_system::get_upgrade_all_count(player* player_ptr)
{
	ZoneScoped;
	auto item_upgrade_cp = player_ptr->m_item_upgrade_component;
	if (nullptr == item_upgrade_cp)
	{
		CONSOLE_ERROR("item_upgrade_cp is nullptr player_guid:{}", player_ptr->get_unit_guid().server_64);
		return 0;
	}
	int32 ret = 0;
	for (int32 i = e_role_equip_slot_weapon_1; i < e_role_equip_slot_max; i++)
	{
		ret += item_upgrade_cp->m_upgrade_data[i];
	}
	return ret;
}
void item_upgrade_system::change_upgrade_att(player* player_ptr, e_role_equip_slot equip_slot, bool is_add)
{
	ZoneScoped;
	auto item_upgrade_cp = player_ptr->m_item_upgrade_component;
	int32 attack_value = 0;
	int32 defense_value = 0;
	int32 hp_value = 0;
	script_mgr::get_instance().call_func("formula_calculation_mgr", "get_upgrade_att_array", 3, false, "%d%d>%d%d%d", equip_slot, item_upgrade_cp->m_upgrade_data[equip_slot], &attack_value, &defense_value, &hp_value);
	std::vector<float> att_array;
	att_array.push_back(4);
	att_array.push_back(e_unit_attack_att_attack_max);
	att_array.push_back(attack_value);
	att_array.push_back(0);
	att_array.push_back(1);

	att_array.push_back(4);
	att_array.push_back(e_unit_attack_att_armor);
	att_array.push_back(defense_value);
	att_array.push_back(0);
	att_array.push_back(1);

	att_array.push_back(4);
	att_array.push_back(e_unit_attack_att_hp_max);
	att_array.push_back(hp_value);
	att_array.push_back(0);
	att_array.push_back(1);
	player_ptr->get_pawn_att().apply_att_change_by_array(att_array, is_add, 1);
}