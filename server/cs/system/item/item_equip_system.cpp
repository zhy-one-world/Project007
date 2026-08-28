#include "components/item/item_base_component.h"
#include "components/item/item_equip_component.h"
#include "components/item/item_manager_component.h"
#include "item_base_system.h"
#include "item_def.hpp"
#include "item_equip_system.h"
#include "item_system.h"
#include "item_upgrade_system.h"
#include "lua/script_mgr.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "logic/unit.hpp"
#include "utility/random.h"
#include "internet/item.pb.h"
#include "utility/init_unit.h"

using namespace hld;
void item_equip_system::start_up(Entity* item_ent, ItemTemplate* item_template, EquipTemplate* equip_template, const item_s2s_item_data& msg)
{
	ZoneScoped;
	auto item_equip_cp = item_ent->add_component<item_equip_component>();
	memset(item_equip_cp->m_data_array, 0, sizeof(item_equip_cp->m_data_array));
	if (msg.has_equip_data())
	{
		auto& equip_data = msg.equip_data();
		for (int32 i = 0; i < e_item_equip_max && i < equip_data.data_array_size(); ++i)
		{
			item_equip_cp->m_data_array[i] = equip_data.data_array(i);
		}
		item_equip_cp->m_equip_template = equip_template;
	}
}
void item_equip_system::shut_down(Entity* item_ent)
{
	ZoneScoped;
	item_ent->remove_component<item_equip_component>();

}
void item_equip_system::heart_tick(const int64& new_time)
{
	ZoneScoped;

}
void item_equip_system::save_item_to_db(Entity* item_ent, item_s2s_item_data* msg)
{
	ZoneScoped;
	auto item_equip_cp = item_ent->get_component<item_equip_component>();
	if (false == item_equip_cp.isValid())
	{
		return;
	}
	auto equip_data = msg->mutable_equip_data();
	for (int32 i = 0; i < e_item_equip_max; ++i)
	{
		equip_data->add_data_array(item_equip_cp->m_data_array[i]);
	}
}
void item_equip_system::dynamic_equip(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	auto player_level = player_ptr->get_unit_info(e_role_info_exp_level);
	auto search_level = player_ptr->get_logic_data(e_role_logic_info_search_level);
	auto random_level = random_gen::get_random(player_level - 3, player_level + 3);
	random_level = random_level < 1 ? 1 : random_level;
	auto random_color_level = 0;
	auto search_level_template = GET_TEMPLATE(SearchLevelTemplate, search_level + search_level_template_id_start);
	if (search_level_template)
	{
		auto random_color = random_gen::get_random(1, 1000000);
		auto sum_random_color = 0;
		for (int32 i = 0; i < search_level_template->ColorPercent.size(); ++i)
		{
			sum_random_color += search_level_template->ColorPercent[i];
			if (random_color <= sum_random_color)
			{
				random_color_level = i;
				break;
			}
		}
	}
	auto att_attack_end = 0;
	auto att_defense_end = 0;
	auto att_hp_end = 0;
	script_mgr::get_instance().call_func("formula_calculation_mgr", "get_item_base_att_array", 3, false, "%d%d>%d%d%d", random_level, random_color_level, &att_attack_end, &att_defense_end, &att_hp_end);
	auto equip_data = item_data.mutable_equip_data();
	for (int32 i = 0; i <e_item_equip_max; ++i)
	{
		equip_data->add_data_array(0);
	}
	equip_data->set_data_array(e_item_equip_level, random_level);
	equip_data->set_data_array(e_item_equip_color, random_color_level);
	equip_data->set_data_array(e_item_equip_attack, att_attack_end);
	equip_data->set_data_array(e_item_equip_defense, att_defense_end);
	equip_data->set_data_array(e_item_equip_hp, att_hp_end);
}
void item_equip_system::static_equip(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	auto equip_data = item_data.mutable_equip_data();
	for (int32 i = 0; i < e_item_equip_max; ++i)
	{
		equip_data->add_data_array(0);
	}
}
void item_equip_system::generate_client_msg(Entity* item_ent, item_s2c_item_data* msg)
{
	auto item_equip_cp = item_ent->get_component<item_equip_component>();
	if (false == item_equip_cp.isValid())
	{
		return;
	}
	auto equip_data = msg->mutable_equip_data();
	for (int32 i = 0; i < e_item_equip_max; ++i)
	{
		equip_data->add_data_array(item_equip_cp->m_data_array[i]);
	}
}
Entity* item_equip_system::get_equip_item(unit* unit_ptr, e_role_equip_slot equip_slot)
{
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	auto& item_map = item_manager_cp->m_bag_map[e_bag_type_equip];
	for (auto& it : item_map)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		auto item_slot = init_unit::init_unit::get_slot_by_item_type(item_base_cp->m_item_template->item_type, item_base_cp->m_item_template->sub_type);
		if (item_slot == equip_slot)
		{
			return it;
		}
	}
	return nullptr;
}
void item_equip_system::change_equip_att(unit* unit_ptr, Entity* item_ent, bool is_add)
{
	ZoneScoped;
	auto item_equip_cp = item_ent->get_component<item_equip_component>();
	std::vector<float> att_array;
	att_array.push_back(4);
	att_array.push_back(e_unit_attack_att_attack_max);
	att_array.push_back(item_equip_cp->m_data_array[e_item_equip_attack]);
	att_array.push_back(0);
	att_array.push_back(1);

	att_array.push_back(4);
	att_array.push_back(e_unit_attack_att_armor);
	att_array.push_back(item_equip_cp->m_data_array[e_item_equip_defense]);
	att_array.push_back(0);
	att_array.push_back(1);

	att_array.push_back(4);
	att_array.push_back(e_unit_attack_att_hp_max);
	att_array.push_back(item_equip_cp->m_data_array[e_item_equip_hp]);
	att_array.push_back(0);
	att_array.push_back(1);
	unit_ptr->get_pawn_att().apply_att_change_by_array(att_array, is_add, 1);

	auto player_ptr = player::cast(unit_ptr);
	if (player_ptr)
	{
		auto item_base_cp = item_ent->get_component<item_base_component>();
		e_role_equip_slot equip_slot = init_unit::get_slot_by_item_type(item_base_cp->m_item_template->item_type, item_base_cp->m_item_template->sub_type);
		item_upgrade_system::change_upgrade_att(player_ptr, equip_slot, is_add);
	}
}

void item_equip_system::equip_on_equip(unit* unit_ptr, Entity* item_ent, int32& target_slot)
{
	ZoneScoped;
	auto item_equip_cp = item_ent->get_component<item_equip_component>();
	if (false == item_equip_cp.isValid())
	{
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();

	auto item_bag_type = e_bag_type(item_base_cp->m_data_array[e_item_base_bag_type]);
	auto item_template = item_base_cp->m_item_template;

	if (item_bag_type == e_bag_type_equip)
	{
		CONSOLE_ERROR("equip_on_equip item_bag_type == e_bag_type_equip");
		return;
	}

	// equip_slot表示物品被装备后应该在哪个装备窗口中
	e_role_equip_slot equip_slot = init_unit::get_slot_by_item_type(item_template->item_type, item_template->sub_type);
	if (equip_slot >= e_role_equip_slot_max)
	{
		CONSOLE_ERROR("equip_slot:{}", (int32)equip_slot);
		return;
	}

	// 如果相应的装备窗口已经有了装备就脱掉它
	auto old_item = get_equip_item(unit_ptr, equip_slot);
	if (old_item)
	{
		equip_off_equip(unit_ptr, old_item, true);
	}

	item_system::remove_item_from_bag(unit_ptr, item_ent);

	// 向相应的装备窗口中添加要装备的物品
	item_base_cp->m_data_array[e_item_base_locked] = 1;
	item_system::put_item_into_bag(unit_ptr, item_ent, e_bag_type_equip);

	change_equip_att(unit_ptr, item_ent, true);

	item_system::send_item_one(unit_ptr, { item_ent });

	//如果是玩家，更新一下最高纪录
	auto* player_ptr = player::cast(unit_ptr);
	if (player_ptr)
	{
		player_ptr->get_mission_mgr().target_check(e_mission_end_type_equip_target_color);
		player_ptr->get_mission_mgr().target_check(e_mission_end_type_install_equip);
	}
}
void item_equip_system::equip_off_equip(unit* unit_ptr, Entity* item_ent, bool send_info)
{
	ZoneScoped;
	auto item_equip_cp = item_ent->get_component<item_equip_component>();
	if (false == item_equip_cp.isValid())
	{
		CONSOLE_ERROR("item_equip_cp is nullptr");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();

	auto item_bag_type = e_bag_type(item_base_cp->m_data_array[e_item_base_bag_type]);
	auto item_template = item_base_cp->m_item_template;

	if (item_bag_type != e_bag_type_equip)
	{
		CONSOLE_ERROR("item_bag_type:{}", (int32)item_bag_type);
		return;
	}
	change_equip_att(unit_ptr, item_ent, false);

	item_system::remove_item_from_bag(unit_ptr, item_ent);
	item_system::put_item_into_bag(unit_ptr, item_ent);

	item_system::send_item_one(unit_ptr, { item_ent });
	auto player_ptr = player::cast(unit_ptr);
	if (player_ptr)
	{
		player_ptr->get_mission_mgr().target_check(e_mission_end_type_inset_genstone);
	}
}
void item_equip_system::equip_recovery(player* player_ptr, const std::vector<Entity*>& item_ent)
{
	std::map<int32, int32> money_get_map;
	std::vector<guid_64> del_guid_array;
	for (auto& it : item_ent)
	{
		auto item_equip_cp = it->get_component<item_equip_component>();
		if (false == item_equip_cp.isValid())
		{
			CONSOLE_ERROR("item_equip_cp is nullptr");
			continue;
		}
		auto item_level = item_equip_cp->m_data_array[e_item_equip_level];
		auto item_color = item_equip_cp->m_data_array[e_item_equip_color];

		int32 money_type = 0;
		int32 money_num = 0;
		script_mgr::get_instance().call_func("formula_calculation_mgr", "get_item_recovery_end", 2, false, "%d%d>%d%d", item_level, item_color, &money_type, &money_num);
		money_get_map[money_type] += money_num;
		del_guid_array.push_back(guid_64(it->getEntityId()));
	}
	for (auto& it : del_guid_array)
	{
		auto item_ent = get_entity(it);
		auto item_del_cp = item_ent->add_component<item_del_component>();
		item_del_cp->del_reason = e_item_del_reason_recovery;
		item_system::del_item(item_ent);
	}
	item_system::send_item_del(player_ptr, del_guid_array);

	std::vector<s_item_template_info> money_array;
	for (auto& it : money_get_map)
	{
		player_ptr->add_money_or_exp(e_money_type(it.first), it.second, e_server_log_add_money_item_recovery);
		money_array.push_back({ it.first, it.second, 0 });
	}
	item_system::send_promp_msg_to_client(player_ptr, {}, money_array);
}