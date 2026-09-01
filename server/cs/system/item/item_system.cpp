#include "components/item/item_base_component.h"
#include "components/item/item_manager_component.h"
#include "utility/init_unit.h"
#include "internet/error.pb.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "item_base_system.h"
#include "item_element_system.h"
#include "item_equip_system.h"
#include "item_prop_system.h"
#include "item_sprite_system.h"
#include "item_system.h"
#include "logic/drop.h"
#include "logic/player.hpp"
#include "logic/unit.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"

using namespace faith;

void item_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	g_ecs->each<item_del_component>([&](Entity* ent, ComponentHandle<item_del_component> item_del_cp) -> bool {
		del_item(ent);
		return true;
	});
}
void item_system::load_data_from_db(unit* unit_ptr, const item_s2s_sl_item_info& msg, e_bag_type bag_type)
{
	ZoneScoped;
	if (nullptr == unit_ptr->m_item_manager_component)
	{
		unit_ptr->m_item_manager_component = std::make_shared<item_manager_component>();
	}
	std::vector<Entity*> item_list;

	for (auto& it : msg.item_list())
	{
		auto item_template = GET_TEMPLATE(ItemTemplate, it.item_id());
		if (nullptr == item_template)
		{
			CONSOLE_ERROR("item_template is nullptr, item_id:{}", it.item_id());
			continue;
		}
		auto item_ent = create_item(unit_ptr, it);
		if (item_ent)
		{
			put_item_into_bag(unit_ptr, item_ent, bag_type);
			item_list.push_back(item_ent);
		}
	}
	if (item_list.empty() == false)
	{
		send_item_one(unit_ptr, item_list);
	}
}
void item_system::clear_data(unit* unit_ptr)
{
	ZoneScoped;
	if (unit_ptr->m_item_manager_component == nullptr)
	{
		return;
	}
	for (auto& bag_it : unit_ptr->m_item_manager_component->m_bag_map)
	{
		for (auto& item_ent : bag_it.second)
		{
			del_item_entity(item_ent);
		}
	}
	unit_ptr->m_item_manager_component = nullptr;
}
Entity* item_system::create_item(unit* unit_ptr, const s_item_template_info& item_info)
{
	ZoneScoped;
	auto item_template = GET_TEMPLATE(ItemTemplate, item_info.m_item_id);
	if (nullptr == item_template)
	{
		CONSOLE_ERROR("item_template is nullptr, item_id = %d", item_info.m_item_id);
		return nullptr;
	}
	item_s2s_item_data msg;
	create_item_msg(msg, unit_ptr, item_info, item_template);
	return create_item(unit_ptr, msg);
}
void item_system::del_item(Entity* item_ent)
{
	ZoneScoped;
	auto item_del_cp = item_ent->get_component<item_del_component>();
	auto item_base_cp = item_ent->get_component<item_base_component>();
	auto del_reason = item_del_cp->del_reason;
	auto unit_ptr = item_base_cp->m_owner_ptr;
	auto item_guid = guid_64(item_ent->getEntityId());
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	if (item_manager_cp)
	{
		auto bag_type = e_bag_type(item_base_cp->m_data_array[e_item_base_bag_type]);
		auto& item_map = item_manager_cp->m_bag_map[bag_type];
		item_map.erase(item_ent);
	}
	del_item_entity(item_ent);
	if (del_reason == e_item_del_reason_cost)
	{
		send_item_del(unit_ptr, { item_guid });
	}
}
std::vector<Entity*> item_system::take_out_item(unit* unit_ptr, e_bag_type bag_type)
{
	ZoneScoped;
	auto& unit_item_cp = unit_ptr->m_item_manager_component;
	auto& item_map = unit_item_cp->m_bag_map[bag_type];
	std::vector<Entity*> item_array;
	for (auto& it : item_map)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		item_base_cp->m_data_array[e_item_base_bag_type] = e_bag_type_none;
		item_array.push_back(it);
	}
	item_map.clear();
	return item_array;
}
void item_system::put_item_into_bag(unit* unit_ptr, Entity* item_ent, e_bag_type bag_type)
{
	ZoneScoped;
	auto& unit_item_cp = unit_ptr->m_item_manager_component;
	auto item_base_cp = item_ent->get_component<item_base_component>();
	auto item_id = item_base_cp->m_item_id;
	auto in_bag_type = e_bag_type(bag_type > e_bag_type_none ? bag_type : item_base_cp->m_data_array[e_item_base_bag_type]);
	auto item_lock = item_base_cp->m_data_array[e_item_base_locked];
	auto item_count = item_base_cp->m_data_array[e_item_base_count];
	if (in_bag_type == e_bag_type_none)
	{
		in_bag_type = init_unit::get_item_bag_type(item_id);
	}
	if (in_bag_type == e_bag_type_none)
	{
		CONSOLE_ERROR("in_bag_type is invalid item_id:{}", item_id);
		in_bag_type = e_bag_type_bag;
	}
	auto& item_map = unit_item_cp->m_bag_map[in_bag_type];
	for (auto& it : item_map)
	{
		auto bag_item_base_cp = it->get_component<item_base_component>();
		if (bag_item_base_cp->m_item_id != item_id || bag_item_base_cp->m_data_array[e_item_base_locked] != item_lock)
		{
			continue;
		}
		auto sum_count = bag_item_base_cp->m_data_array[e_item_base_count] + item_count;
		if (item_base_cp->m_item_template->max_pile_num == 0)
		{
			bag_item_base_cp->m_data_array[e_item_base_count] = sum_count;
			item_count = 0;
			send_item_one(unit_ptr, { it });
		}
		if (sum_count > item_base_cp->m_item_template->max_pile_num)
		{
			bag_item_base_cp->m_data_array[e_item_base_count] = item_base_cp->m_item_template->max_pile_num;
			item_count = sum_count - item_base_cp->m_item_template->max_pile_num;
			send_item_one(unit_ptr, { it });
		}
		else
		{
			bag_item_base_cp->m_data_array[e_item_base_count] = sum_count;
			item_count = 0;
			send_item_one(unit_ptr, { it });
		}
		if (item_count <= 0)
		{
			break;
		}
	}
	item_base_cp->m_data_array[e_item_base_count] = item_count;
	if (item_count > 0)
	{
		item_base_cp->m_data_array[e_item_base_bag_type] = in_bag_type;
		item_map.insert(item_ent);
	}
	else
	{
		auto item_del_cp = item_ent->add_component<item_del_component>();
		item_del_cp->del_reason = e_item_del_reason_into_bag;
	}
}
void item_system::put_item_into_bag(unit* unit_ptr, const std::vector<s_item_template_info>& item_array)
{
	ZoneScoped;
	auto res_item_list = std::move(spread_item_by_max_num(unit_ptr, item_array));
	std::vector<item_s2s_item_data> item_msg_array;
	for (auto& it : res_item_list)
	{
		if (it.m_item_id < e_money_type_max && it.m_bag_type != e_bag_type_online_award)
		{
			auto player_ptr = dynamic_cast<player*>(unit_ptr);
			if (player_ptr)
			{
				player_ptr->add_money_or_exp((e_money_type)it.m_item_id, it.m_item_num, e_server_log_add_money_put_into_bag, -1);
			}
		}
		else
		{
			auto item_template = GET_TEMPLATE(ItemTemplate, it.m_item_id);
			item_s2s_item_data msg;
			create_item_msg(msg, unit_ptr, it, item_template);
			item_msg_array.push_back(msg);
		}
	}
	put_item_into_bag(unit_ptr, item_msg_array);
}
void item_system::put_item_into_bag(unit* unit_ptr, const std::vector<Entity*>& item_array, e_bag_type bag_type)
{
	ZoneScoped;
	std::vector<Entity*> ent_array;
	for (auto& it : item_array)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		item_base_cp->m_data_array[e_item_base_bag_type] = bag_type;
		put_item_into_bag(unit_ptr, it);
		if (it->get_component<item_del_component>().isValid() == false)
		{
			ent_array.push_back(it);
		}
		else
		{
			guid_64 item_guid(it->getEntityId());
			send_item_del(unit_ptr, { item_guid });
			del_item_entity(it);
		}
	}
	send_item_one(unit_ptr, ent_array);
}
void item_system::put_item_into_bag(unit* unit_ptr, const std::vector<item_s2s_item_data>& item_array)
{
	ZoneScoped;
	auto& unit_item_cp = unit_ptr->m_item_manager_component;
	std::vector<Entity*> ent_array;
	for (auto& it : item_array)
	{
		auto item_ent = create_item(unit_ptr, it);
		if (nullptr == item_ent)
		{
			CONSOLE_ERROR("item_ent is nullptr");
			continue;
		}
		put_item_into_bag(unit_ptr, item_ent);
		if (item_ent->get_component<item_del_component>().isValid() == false)
		{
			ent_array.push_back(item_ent);
		}
		else
		{
			del_item_entity(item_ent);
		}
	}
	send_item_one(unit_ptr, ent_array);
}
void item_system::remove_item_from_bag(unit* unit_ptr, Entity* item_ent)
{
	ZoneScoped;
	auto item_base_cp = item_ent->get_component<item_base_component>();
	auto item_bag_type = e_bag_type(item_base_cp->m_data_array[e_item_base_bag_type]);
	auto& old_item_map = unit_ptr->m_item_manager_component->m_bag_map[item_bag_type];
	old_item_map.erase(item_ent);
	item_base_cp->m_data_array[e_item_base_bag_type] = e_bag_type_none;
}
bool item_system::can_cost_item(unit* unit_ptr, e_bag_type bag_type, int32 item_id, int32 item_num, e_item_lock lock_state)
{
	auto item_count = get_item_count(unit_ptr, bag_type, item_id, lock_state);
	return item_count >= item_num;
}
void item_system::cost_item_from_bag(unit* unit_ptr, e_bag_type bag_type, int32 item_id, int32 item_count, e_item_lock first_use)
{
	ZoneScoped;
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	auto item_map = item_manager_cp->m_bag_map[bag_type];
	for (auto& it : item_map)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		if (item_base_cp->m_item_id == item_id)
		{
			if (item_base_cp->m_data_array[e_item_base_count] > item_count)
			{
				item_base_cp->m_data_array[e_item_base_count] -= item_count;
				send_item_one(unit_ptr, { it });
				item_count = 0;
				break;
			}
			else
			{
				item_count -= item_base_cp->m_data_array[e_item_base_count];
				item_base_cp->m_data_array[e_item_base_count] = 0;
				auto item_del_cp = it->add_component<item_del_component>();
				item_del_cp->del_reason = e_item_del_reason_cost;
				del_item(it);
			}
		}
	}
}
void item_system::cost_item_by_ent(Entity* item_ent, int32 cost_num)
{
	ZoneScoped;
	auto item_base_cp = item_ent->get_component<item_base_component>();
	if (cost_num == 0)
	{
		cost_num = item_base_cp->m_data_array[e_item_base_count];
	}
	item_base_cp->m_data_array[e_item_base_count] -= cost_num;
	if (item_base_cp->m_data_array[e_item_base_count] <= 0)
	{
		auto item_del_cp = item_ent->add_component<item_del_component>();
		item_del_cp->del_reason = e_item_del_reason_cost;
		del_item(item_ent);
	}
	else
	{
		send_item_one(item_base_cp->m_owner_ptr, { item_ent });
	}
}

std::vector<s_item_template_info> item_system::spread_item_by_max_num(unit* unit_ptr, const std::vector<s_item_template_info>& item_info_array)
{
	ZoneScoped;
	std::vector<s_item_template_info> open_item_list;
	for (auto& it : item_info_array)
	{
		auto item_template = GET_TEMPLATE(ItemTemplate, it.m_item_id);
		if (nullptr == item_template)
		{
			CONSOLE_ERROR("item_template is nullptr, item_id:{}", it.m_item_id);
			continue;
		}
		open_auto_use_item(open_item_list, unit_ptr, it, item_template);
	}
	std::vector<s_item_template_info> res_item_list;
	for (auto& it : open_item_list)
	{
		if (it.m_item_id <= e_money_type_max)
		{
			res_item_list.push_back(it);
			continue;
		}
		auto item_template = GET_TEMPLATE(ItemTemplate, it.m_item_id);

		if (item_template->max_pile_num <= 0)
		{
			res_item_list.push_back(it);
			continue;
		}
		int32 item_count = it.m_item_num;
		while (item_count > 0)
		{
			int32 item_num = item_count > item_template->max_pile_num ? item_template->max_pile_num : item_count;
			res_item_list.push_back({ it.m_item_id, item_num, it.m_lock, it.m_bag_type });
			item_count -= item_num;
		}

	}
	return res_item_list;
}
void item_system::open_auto_use_item(std::vector<s_item_template_info>& res_item_list, unit* unit_ptr, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	if (item_template->item_type != e_item_type_expendable)
	{
		res_item_list.push_back(item_info);
		return;
	}

	auto prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, item_template->logic_id);
	if (nullptr == prop_template)
	{
		CONSOLE_ERROR("prop_template is nullptr, item_id:{} prop_id:{}", item_template->attribute_id, item_template->logic_id);
		return;
	}
	if (prop_template->UseType == e_item_use_type_can_not_use || prop_template->UseType == e_item_use_type_only_jump)
	{
		res_item_list.push_back(item_info);
		return;
	}

	// 使钱财类的改变生效
	for (int32 i = 0; i < prop_template->MoneyChange.size(); i += 2)
	{
		int32 money_type = prop_template->MoneyChange[i];
		int64 money_num = prop_template->MoneyChange[i + 1] * item_info.m_item_num;
		res_item_list.push_back({ money_type, money_num, item_info.m_lock, item_info.m_bag_type });
	}
	switch (item_template->sub_type)
	{
	case e_prop_sub_type_gife:
	{
		std::vector<s_item_template_info> drop_list;
		for (int32 i = 0; i < item_info.m_item_num; i++)
		{
			cdrop::gen_drop_list_by_drop_id(prop_template->ItemGet, drop_list, unit_ptr->get_unit_info(e_role_info_class_type), unit_ptr->get_unit_info(e_role_info_exp_level));
		}
		for (auto&it : drop_list)
		{
			it.m_bag_type = item_info.m_bag_type;
		}
		res_item_list.insert(res_item_list.end(), drop_list.begin(), drop_list.end());
	}
	break;
	case e_prop_sub_type_money:
	{
		for (int32 i = 0; i < prop_template->MoneyChange.size(); i += 2)
		{
			const int32 money_type = prop_template->MoneyChange[i];
			int64 money_num = prop_template->MoneyChange[i + 1] * item_info.m_item_num;
			res_item_list.push_back({ money_type, money_num, item_info.m_lock, item_info.m_bag_type });
		}
	}
	break;
	case e_prop_sub_type_add_exp_with_num:
	{
		float exp_fix_num = prop_template->ExpNum;
		if (exp_fix_num >= .0f)
		{
			int32 upgrade_temp_id = unit_ptr->get_unit_info(e_role_info_upgrade_id);
			PlayerUpgradeTemplate* player_upgrade_template = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_temp_id);
			if (player_upgrade_template)
			{
				int64 exp_num = int64(player_upgrade_template->BaseExp) * exp_fix_num * int64(item_info.m_item_num);
				res_item_list.push_back({ e_money_type_exp, exp_num, item_info.m_lock, item_info.m_bag_type });
			}
		}
		else
		{
			CONSOLE_ERROR("exp_fix_num is invalid item_id:{} exp_fix_num:{}", item_template->attribute_id, exp_fix_num);
		}
	}
	break;
	default:
		CONSOLE_ERROR("item_template->sub_type is invalid item_id:{} sub_type:{}", item_template->attribute_id, item_template->sub_type);
		break;
	}
}
Entity* item_system::get_item_by_id(unit* unit_ptr, e_bag_type bag_type, int32 item_id)
{
	ZoneScoped;
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	auto& item_map = item_manager_cp->m_bag_map[bag_type];
	for (auto& it : item_map)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		if (item_base_cp->m_item_id == item_id)
		{
			return it;
		}
	}
	return nullptr;
}
int32 item_system::get_item_count(unit* unit_ptr, e_bag_type bag_type, int32 item_id, e_item_lock lock_state)
{
	ZoneScoped;
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	auto& item_map = item_manager_cp->m_bag_map[bag_type];
	int32 item_count = 0;
	for (auto& it : item_map)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		if (item_base_cp->m_item_id == item_id)
		{
			switch (lock_state)
			{	
			case faith::e_item_lock_all:
				item_count += item_base_cp->m_data_array[e_item_base_count];
				break;
			case faith::e_item_lock_lock:
				if (item_base_cp->m_data_array[e_item_base_locked] > 0)
				{
					item_count += item_base_cp->m_data_array[e_item_base_count];
				}
				break;
			case faith::e_item_lock_un_lock:
				if (item_base_cp->m_data_array[e_item_base_locked] == 0)
				{
					item_count += item_base_cp->m_data_array[e_item_base_count];
				}
				break;
			default:
				CONSOLE_ERROR("lock_state is invalid lock_state:{}", int32(lock_state));
				break;
			}
		}
	}
	return item_count;
}

void item_system::send_promp_msg_to_client(player* player_ptr, const std::vector<s_item_template_info>& items_array, const std::vector<s_item_template_info>& money_array)
{
	ZoneScoped;
	if (items_array.empty() && money_array.empty())
	{
		CONSOLE_ERROR("items_array and money_array is empty");
		return;
	}
	std::vector<s_item_template_info> merge_array;

	for (auto& it : items_array)
	{
		const int32 item_id = it.m_item_id;
		const int32 item_num = it.m_item_num;
		const int32 item_is_lock = it.m_lock;
		bool is_merge = false;
		for (auto& merge_it : merge_array)
		{
			const int32 temp_item_id = merge_it.m_item_id;
			const int32 temp_item_is_lock = merge_it.m_lock;
			if (temp_item_id == item_id && temp_item_is_lock == item_is_lock)
			{
				merge_it.m_item_num += item_num;
				is_merge = true;
				break;
			}
		}
		if (is_merge == false)
		{
			merge_array.push_back({item_id, item_num, item_is_lock});
		}
	}
	for (auto& it : money_array)
	{
		const int32 item_id = it.m_item_id;
		const int32 item_num = it.m_item_num;
		bool is_merge = false;
		for (auto& merge_it : merge_array)
		{
			const int32 temp_item_id = merge_it.m_item_id;
			if (temp_item_id == item_id)
			{
				merge_it.m_item_num += item_num;
				is_merge = true;
				break;
			}
		}
		if (is_merge == false)
		{
			merge_array.push_back({ item_id, item_num });
		}
	}
	faith::item_proto_item_get_item_msg client_pak;
	client_pak.add_role_guid(player_ptr->get_unit_guid().A);
	client_pak.add_role_guid(player_ptr->get_unit_guid().B);

	for (auto& it : merge_array)
	{
		client_pak.add_item_data(it.m_item_id);
		client_pak.add_item_data(it.m_item_num);
		client_pak.add_item_data(it.m_lock);
	}

	player_ptr->send_message_to_self(&client_pak, e_msgindex_s2c_get_item);
}		
void item_system::send_item_one(unit* unit_ptr, const std::vector<Entity*>& item_ent_array, bool to_self)
{
	item_s2c_item_list msg;
	msg.set_unit_guid(unit_ptr->get_unit_guid().server_64);
	for (auto& it : item_ent_array)
	{
		auto item_msg = msg.add_item_list();
		generate_client_msg(it, item_msg);
	}
	if (to_self)
	{
		unit_ptr->send_message_to_self(&msg, e_msgindex_s2c_item_update_character);
	}
	else
	{
		unit_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_item_update_character);
	}

}
void item_system::send_item_all(unit* unit_ptr)
{
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	item_s2c_item_list msg;
	msg.set_unit_guid(unit_ptr->get_unit_guid().server_64);

	for (auto& bag_it : item_manager_cp->m_bag_map)
	{
		for (auto& item_it : bag_it.second)
		{
			auto item_msg = msg.add_item_list();
			generate_client_msg(item_it, item_msg);
		}

		if (msg.item_list_size() > send_item_max_num)
		{
			unit_ptr->send_message_to_self(&msg, e_msgindex_s2c_item_update_character);
			msg.clear_item_list();
		}
	}
	if (msg.item_list_size() > 0)
	{
		unit_ptr->send_message_to_self(&msg, e_msgindex_s2c_item_update_character);
	}
}
void item_system::send_item_del(unit* unit_ptr, const std::vector<guid_64>& item_guid_array)
{
	if (item_guid_array.empty())
	{
		return;
	}
	item_proto_item_delete item_delete_msg;
	item_delete_msg.add_role_guid(unit_ptr->get_unit_guid().A);
	item_delete_msg.add_role_guid(unit_ptr->get_unit_guid().B);
	item_delete_msg.set_del_item_arr_len(item_guid_array.size());
	for (auto& item_guid : item_guid_array)
	{
		item_delete_msg.add_del_item_guid_arr(item_guid.A);
		item_delete_msg.add_del_item_guid_arr(item_guid.B);
	}

	unit_ptr->send_message_to_self(&item_delete_msg, e_msgindex_s2c_item_delete);
}
void item_system::item_operate_message(player* player_ptr, const item_proto_item_operation& msg)
{
	if (msg.item_guid_size() <= 0 || msg.item_guid_size() % 2 != 0)
	{
		CONSOLE_ERROR("item_guid_size is invalid item_guid_size:{}", msg.item_guid_size());
		return;
	}
	std::vector<guid_64> item_guid_array;
	for (int32 i = 0; i < msg.item_guid_size(); i += 2)
	{
		guid_64 item_guid;
		item_guid.A = msg.item_guid(0 + i);
		item_guid.B = msg.item_guid(1 + i);
		item_guid_array.push_back(item_guid);
	}
	switch (msg.operation_type())
	{
	case e_item_operation_equipon:
	{
		if (item_guid_array.empty())
		{
			CONSOLE_ERROR("item_guid_array is empty");
			return;
		}
		int32 equip_slot = msg.item_slot();
		auto item_ent = get_entity(item_guid_array[0]);
		if (item_ent == nullptr)
		{
			CONSOLE_ERROR("item_ent is nullptr");
			return;
		}
		item_equip_system::equip_on_equip(player_ptr, item_ent, equip_slot);
		item_sprite_system::equip_on_sprite(player_ptr, item_ent, equip_slot);
	}
	break;
	case e_item_operation_equipoff:
	{
		if (item_guid_array.empty())
		{
			CONSOLE_ERROR("item_guid_array is empty");
			return;
		}
		auto item_ent = get_entity(item_guid_array[0]);
		if (item_ent == nullptr)
		{
			CONSOLE_ERROR("item_ent is nullptr");
			return;
		}
		item_equip_system::equip_off_equip(player_ptr, item_ent);
		item_sprite_system::equip_off_sprite(player_ptr, item_ent);
	}
	break;
	case e_item_operation_recovery:
	{
		std::vector<Entity*> item_ent_array;
		for (auto& it : item_guid_array)
		{
			auto item_ent = get_entity(it);
			if (item_ent)
			{
				item_ent_array.push_back(item_ent);
			}
		}
		if (item_ent_array.empty())
		{
			CONSOLE_ERROR("item_ent_array is empty");
			return;
		}
		item_equip_system::equip_recovery(player_ptr, item_ent_array);
	}
	break;
	case e_item_operation_spirit_send_out:
	{
		if (item_guid_array.empty())
		{
			CONSOLE_ERROR("item_guid_array is empty");
			return;
		}
		item_sprite_system::activate_sprite(player_ptr, item_guid_array[0].A);
	}
	break;
	case e_item_operation_spirit_upgrade:
	{
		if (item_guid_array.empty())
		{
			CONSOLE_ERROR("item_guid_array is empty");
			return;
		}
		for (auto& it : item_guid_array)
		{
			auto item_ent = get_entity(it);
			if (item_ent)
			{
				item_sprite_system::sprite_level_up(player_ptr, item_ent);
			}
		}
	}
	break;
	case e_item_operation_spirit_had:
	{
		if (item_guid_array.empty())
		{
			CONSOLE_ERROR("item_guid_array is empty");
			return;
		}
		auto item_ent = get_entity(item_guid_array[0]);
		if (item_ent == nullptr)
		{
			CONSOLE_ERROR("item_ent is nullptr");
			return;
		}
		int32 equip_slot = msg.item_slot();
		item_sprite_system::set_scene_show(player_ptr, item_ent, equip_slot);
	}
	break;
	default:
		CONSOLE_ERROR("operation_type is invalid operation_type:{}", msg.operation_type());
		break;
	}
	item_proto_item_operation_end item_msg;

	item_msg.set_operation_state(e_error_code_success);
	item_msg.set_operation_type(msg.operation_type());
	item_msg.set_item_slot(msg.item_slot());

	item_msg.add_role_guid(player_ptr->get_unit_guid().A);
	item_msg.add_role_guid(player_ptr->get_unit_guid().B);
	for (auto& it : item_guid_array)
	{
		item_msg.add_item_guid(it.A);
		item_msg.add_item_guid(it.B);
	}

	player_ptr->send_message_to_self(&item_msg, e_msgindex_s2c_item_operation);
}
void item_system::clear_bag(unit* unit_ptr, e_bag_type bag_type)
{
	ZoneScoped;
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	auto& item_map = item_manager_cp->m_bag_map[bag_type];
	std::vector<guid_64> del_guid_array;
	for (auto& it : item_map)
	{
		del_guid_array.push_back(it->getEntityId());
		auto item_del_cp = it->add_component<item_del_component>();
		item_del_cp->del_reason = e_item_del_reason_clear_bag;
		del_item(it);
	}
	item_map.clear();
	send_item_del(unit_ptr, del_guid_array);
}