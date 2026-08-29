#include "components/item/item_base_component.h"
#include "components/item/item_element_component.h"
#include "components/item/item_manager_component.h"
#include "internet/elementheart.pb.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "item_element_system.h"
#include "item_system.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"

using namespace faith;
void item_element_system::start_up(Entity* item_ent, ItemTemplate* item_template, ElementHeartTemplate* element_template, const item_s2s_item_data& msg)
{
	ZoneScoped;
	auto item_element_cp = item_ent->add_component<item_element_component>();
	memset(item_element_cp->m_data_array, 0, sizeof(item_element_cp->m_data_array));
	if (msg.has_element_data())
	{
		auto& element_data = msg.element_data();
		for (int32 i = 0; i < e_item_element_max && i < element_data.data_array_size(); ++i)
		{
			item_element_cp->m_data_array[i] = element_data.data_array(i);
		}
		item_element_cp->m_element_template = element_template;
		item_element_cp->m_element_upgrade_template = GET_TEMPLATE(ElementHeartUpgradeTemplate, element_template->UpgradeId + item_element_cp->m_data_array[e_item_element_level]);
	}
}
void item_element_system::shut_down(Entity* item_ent)
{
	ZoneScoped;
	item_ent->remove_component<item_element_component>();
}
void item_element_system::heart_tick(const int64& new_time)
{
	ZoneScoped;

}
void item_element_system::save_item_to_db(Entity* item_ent, item_s2s_item_data* msg)
{
	ZoneScoped;
	auto item_element_cp = item_ent->get_component<item_element_component>();
	if (false == item_element_cp.isValid())
	{
		return;
	}
	auto element_data = msg->mutable_element_data();
	for (int32 i = 0; i < e_item_element_max; ++i)
	{
		element_data->add_data_array(item_element_cp->m_data_array[i]);
	}
}
void item_element_system::dynamic_element(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	static_element(item_data, item_info, item_template);
}
void item_element_system::static_element(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	auto element_data = item_data.mutable_element_data();
	for (int32 i = 0; i < e_item_element_max; ++i)
	{
		element_data->add_data_array(0);
	}
}
void item_element_system::generate_client_msg(Entity* item_ent, item_s2c_item_data* msg)
{
	ZoneScoped;
	auto item_element_cp = item_ent->get_component<item_element_component>();
	if (false == item_element_cp.isValid())
	{
		return;
	}
	auto element_data = msg->mutable_element_data();
	for (int32 i = 0; i < e_item_element_max; ++i)
	{
		element_data->add_data_array(item_element_cp->m_data_array[i]);
	}
}
void  item_element_system::change_element_att(unit* unit_ptr, Entity* item_ent, bool is_add)
{
	ZoneScoped;
	auto item_element_cp = item_ent->get_component<item_element_component>();
	unit_ptr->get_pawn_att().apply_att_change_by_array(item_element_cp->m_element_template->BasicAtt, is_add);
	if (item_element_cp->m_data_array[e_item_element_level] > 0)
	{
		unit_ptr->get_pawn_att().apply_att_change_by_array(item_element_cp->m_element_template->UpgradeAttEach, is_add, item_element_cp->m_data_array[e_item_element_level]);
	}
}
Entity* item_element_system::get_element_by_slot(player* player_ptr, int32 equip_slot)
{
	ZoneScoped;
	if (equip_slot <= 0)
	{
		CONSOLE_ERROR("equip_slot:{} <= 0", equip_slot);
		return nullptr;
	}

	auto& item_manager_cp = player_ptr->m_item_manager_component;
	auto& item_map = item_manager_cp->m_bag_map[e_bag_type_equip_elemenet];
	for (auto& it : item_map)
	{
		auto item_element_cp = it->get_component<item_element_component>();
		if (item_element_cp->m_data_array[e_item_element_equip_slot] == equip_slot)
		{
			return it;
		}
	}
	return nullptr;
}
void item_element_system::element_operate_message(player* player_ptr, const element_heart_proto_element_heart_operate& msg)
{
	ZoneScoped;
	if (msg.element_heart_guid_size() <= 0 || msg.element_heart_guid_size() % 2 != 0)
	{
		return;
	}
	std::vector<guid_64> item_guid_array;
	for (int32 i = 0; i < msg.element_heart_guid_size(); i += 2)
	{
		guid_64 item_guid;
		item_guid.A = msg.element_heart_guid(0 + i);
		item_guid.B = msg.element_heart_guid(1 + i);
		item_guid_array.push_back(item_guid);
	}
	switch (msg.operation_type())
	{
	case e_element_heart_operate_upgrade:
	{
		auto item_ent = get_entity(item_guid_array[0]);
		if (nullptr == item_ent)
		{
			CONSOLE_ERROR("item_ent == nullptr");
			return;
		}
		element_upgrade(player_ptr, item_ent);
	}
	break;
	case e_element_heart_operate_equip_on:
	{
		auto item_ent = get_entity(item_guid_array[0]);
		if (nullptr == item_ent)
		{
			CONSOLE_ERROR("item_ent == nullptr");
			return;
		}
		equip_on_element(player_ptr, item_ent);
	}
	break;
	case e_element_heart_operate_equip_off:
	{
		auto item_ent = get_entity(item_guid_array[0]);
		if (nullptr == item_ent)
		{
			CONSOLE_ERROR("item_ent == nullptr");
			return;
		}
		equip_off_element(player_ptr, item_ent);
	}
	break;
	case e_element_heart_operate_recovery:
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
		element_recovery(player_ptr, item_ent_array);
	}
	break;
	case e_element_heart_operate_merge:
	{
		if (item_guid_array.size() < 2)
		{
			CONSOLE_ERROR("item_guid_array.size() < 2");
			return;
		}
		auto first_item_ent = get_entity(item_guid_array[0]);
		auto second_item_ent = get_entity(item_guid_array[1]);
		if (nullptr == first_item_ent || nullptr == second_item_ent)
		{
			CONSOLE_ERROR("first_item_ent == nullptr || second_item_ent == nullptr");
			return;
		}
		element_merge(player_ptr, first_item_ent, second_item_ent);
	}
	break;
	case e_element_heart_operate_summon:
	{

	}
	break;
	default:
		CONSOLE_ERROR("operation_type:{}", msg.operation_type());
		break;
	}
	element_heart_proto_element_heart_operate_end end_msg;
	player_ptr->send_message_to_self(&end_msg, e_msgindex_s2c_element_heart_operate);
}
void item_element_system::element_upgrade(player* player_ptr, Entity* item_ent)
{
	ZoneScoped;
	auto item_element_cp = item_ent->get_component<item_element_component>();
	if (false == item_element_cp.isValid())
	{
		CONSOLE_ERROR("item_element_cp == nullptr");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();
	
	auto element_template = item_element_cp->m_element_template;
	auto element_upgrade_template = item_element_cp->m_element_upgrade_template;

	auto next_element_upgrade_template = GET_TEMPLATE(ElementHeartUpgradeTemplate, element_template->UpgradeId + item_element_cp->m_data_array[e_item_element_level] + 1);
	if (next_element_upgrade_template == nullptr)
	{
		CONSOLE_ERROR("next_element_upgrade_template == nullptr upgrade_id:{} level:{}", element_template->UpgradeId, item_element_cp->m_data_array[e_item_element_level] + 1);
		return;
	}
	for (int i = 0; i < element_upgrade_template->UpgradeCost.size(); i += 2)
	{
		auto money_id = element_upgrade_template->UpgradeCost[i];
		auto money_value = element_upgrade_template->UpgradeCost[i + 1];
		if (false == player_ptr->can_cut_money(e_money_type(money_id), money_value))
		{
			CONSOLE_ERROR("money_id:{} money_value:{}", money_id, money_value);
			return;
		}
	}
	for (int i = 0; i < element_upgrade_template->UpgradeCost.size(); i += 2)
	{
		auto money_id = element_upgrade_template->UpgradeCost[i];
		auto money_value = element_upgrade_template->UpgradeCost[i + 1];
		player_ptr->cut_money(e_money_type(money_id), money_value, e_server_log_cut_money_element_heart_upgrade, element_template->attribute_id, element_upgrade_template->attribute_id);
	}
	if (item_base_cp->m_data_array[e_item_base_bag_type] == e_bag_type_equip_elemenet)
	{
		change_element_att(player_ptr, item_ent, false);
	}
	item_element_cp->m_data_array[e_item_element_level] += 1;
	item_element_cp->m_element_upgrade_template = next_element_upgrade_template;
	if (item_base_cp->m_data_array[e_item_base_bag_type] == e_bag_type_equip_elemenet)
	{
		change_element_att(player_ptr, item_ent, true);
	}
	item_system::send_item_one(player_ptr, { item_ent });
}
void item_element_system::equip_on_element(player* player_ptr, Entity* item_ent)
{
	ZoneScoped;
	auto item_element_cp = item_ent->get_component<item_element_component>();
	if (false == item_element_cp.isValid())
	{
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();
	if (item_base_cp->m_data_array[e_item_base_bag_type] != e_bag_type_elemenet)
	{
		CONSOLE_ERROR("bag_type:{}", item_base_cp->m_data_array[e_item_base_bag_type]);
		return;
	}

	auto element_template = item_element_cp->m_element_template;
	auto target_slot = element_template->AttType;
	auto open_count = player_ptr->get_unit_info(e_role_info_element_heart_slot_open_num);
	if (target_slot > open_count)
	{
		CONSOLE_ERROR("target_slot:{} open_count:{}", target_slot, open_count);
		return;
	}
	auto old_element = get_element_by_slot(player_ptr, target_slot);
	if (old_element != nullptr)
	{
		equip_off_element(player_ptr, old_element, true);
	}
	item_system::remove_item_from_bag(player_ptr, item_ent);
	item_system::put_item_into_bag(player_ptr, item_ent, e_bag_type_equip_elemenet);
	item_element_cp->m_data_array[e_item_element_equip_slot] = target_slot;

	change_element_att(player_ptr, item_ent, true);

	item_system::send_item_one(player_ptr, { item_ent });
}
void item_element_system::equip_off_element(player* player_ptr, Entity* item_ent, bool send_client)
{
	ZoneScoped;
	auto item_element_cp = item_ent->get_component<item_element_component>();
	if (false == item_element_cp.isValid())
	{
		CONSOLE_ERROR("item_element_cp == nullptr");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();

	if (item_base_cp->m_data_array[e_item_base_bag_type] != e_bag_type_equip_elemenet)
	{
		CONSOLE_ERROR("bag_type:{}", item_base_cp->m_data_array[e_item_base_bag_type]);
		return;
	}
	change_element_att(player_ptr, item_ent, false);
	item_element_cp->m_data_array[e_item_element_equip_slot] = 0;
	item_system::remove_item_from_bag(player_ptr, item_ent);
	item_system::put_item_into_bag(player_ptr, item_ent, e_bag_type_elemenet);
	if (send_client)
	{
		item_system::send_item_one(player_ptr, { item_ent });
	}
}
void item_element_system::element_recovery(player* player_ptr, const std::vector<Entity*>& item_ent_array)
{
	ZoneScoped;
	std::map<int32, int32> recovery_map;
	std::vector<guid_64> del_guid_array;
	for (auto& it : item_ent_array)
	{
		auto item_element_cp = it->get_component<item_element_component>();
		if (false == item_element_cp.isValid())
		{
			CONSOLE_ERROR("item_element_cp is invalid!");
			continue;
		}
		auto element_upgrade_template = item_element_cp->m_element_upgrade_template;
		for (int32 i = 0; i < element_upgrade_template->RecoveryReward.size(); i += 2)
		{
			recovery_map[element_upgrade_template->RecoveryReward[i]] += element_upgrade_template->RecoveryReward[i + 1];
		}
		del_guid_array.push_back(it->getEntityId());
		auto item_del_cp = it->add_component<item_del_component>();
		item_del_cp->del_reason = e_item_del_reason_recovery;
		item_system::del_item(it);
	}
	item_system::send_item_del(player_ptr, del_guid_array);
	std::vector<s_item_template_info> res_list;
	for (auto& it : recovery_map)
	{
		player_ptr->add_money_or_exp(e_money_type(it.first), it.second, e_server_log_add_money_element_heart_recovery);
		res_list.push_back({ it.first, it.second });
	}
	item_system::send_promp_msg_to_client(player_ptr, {}, res_list);
}
void item_element_system::element_merge(player* player_ptr, Entity* first_item_ent, Entity* second_item_ent)
{
	ZoneScoped;
	auto first_item_element_cp = first_item_ent->get_component<item_element_component>();
	auto second_item_element_cp = second_item_ent->get_component<item_element_component>();
	if (false == first_item_element_cp.isValid() || false == second_item_element_cp.isValid())
	{
		CONSOLE_ERROR("first_item_element_cp == nullptr || second_item_element_cp == nullptr");
		return;
	}
	auto first_element_template = first_item_element_cp->m_element_template;
	auto merge_element_template = GET_TEMPLATE(ElementHeartTemplate, first_element_template->MergeId);
	if (merge_element_template == nullptr)
	{
		CONSOLE_ERROR("merge_element_template == nullptr MergeId:{}", first_element_template->MergeId);
		return;
	}
	auto create_item_id = merge_element_template->ItemTemplateId;
	auto create_item_template = GET_TEMPLATE(ItemTemplate, create_item_id);
	if (create_item_template == nullptr)
	{
		CONSOLE_ERROR("create_item_template == nullptr create_item_id:{}", create_item_id);
		return;
	}
	for (int32 i = 0; i < merge_element_template->MergeCost.size(); i += 2)
	{
		auto money_id = merge_element_template->MergeCost[i];
		auto money_value = merge_element_template->MergeCost[i + 1];
		if (false == player_ptr->can_cut_money(e_money_type(money_id), money_value))
		{
			CONSOLE_ERROR("money_id:{} money_value:{}", money_id, money_value);
			return;
		}
	}

	auto first_item_del_cp = first_item_ent->add_component<item_del_component>();
	first_item_del_cp->del_reason = e_item_del_reason_cost;
	item_system::del_item(first_item_ent);
	auto second_item_del_cp = second_item_ent->add_component<item_del_component>();
	second_item_del_cp->del_reason = e_item_del_reason_cost;
	item_system::del_item(second_item_ent);

	for (int32 i = 0; i < merge_element_template->MergeCost.size(); i += 2)
	{
		auto money_id = merge_element_template->MergeCost[i];
		auto money_value = merge_element_template->MergeCost[i + 1];
		player_ptr->cut_money(e_money_type(money_id), money_value, e_server_log_cut_money_composit, first_element_template->attribute_id, second_item_element_cp->m_element_template->attribute_id);
	}
	auto create_item_ent = item_system::create_item(player_ptr, create_item_id);
	item_system::put_item_into_bag(player_ptr, create_item_ent, e_bag_type_elemenet);
	item_system::send_item_one(player_ptr, { create_item_ent });
}