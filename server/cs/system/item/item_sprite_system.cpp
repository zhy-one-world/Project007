#include "components/item/item_base_component.h"
#include "components/item/item_manager_component.h"
#include "components/item/item_sprite_component.h"
#include "internet/character.pb.h"
#include "internet/error.pb.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "item_sprite_system.h"
#include "item_system.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"

using namespace hld;
void item_sprite_system::start_up(Entity* item_ent, ItemTemplate* item_template, SpiritTemplate* sprite_template, const item_s2s_item_data& msg)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->add_component<item_sprite_component>();
	memset(item_sprite_cp->m_data_array, 0, sizeof(item_sprite_cp->m_data_array));
	if (msg.has_sprite_data())
	{
		auto& sprite_data = msg.sprite_data();
		for (int32 i = 0; i < e_item_sprite_max && i < sprite_data.data_array_size(); ++i)
		{
			item_sprite_cp->m_data_array[i] = sprite_data.data_array(i);
		}
		item_sprite_cp->m_sprite_template = sprite_template;
	}
}
void item_sprite_system::shut_down(Entity* item_ent)
{
	ZoneScoped;
	item_ent->remove_component<item_sprite_component>();
}
void item_sprite_system::heart_tick(const int64& new_time)
{
	ZoneScoped;

}
void item_sprite_system::save_item_to_db(Entity* item_ent, item_s2s_item_data* msg)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	if (false == item_sprite_cp.isValid())
	{
		return;
	}
	auto sprite_data = msg->mutable_sprite_data();
	for (int32 i = 0; i < e_item_sprite_max; ++i)
	{
		sprite_data->add_data_array(item_sprite_cp->m_data_array[i]);
	}
}
void item_sprite_system::dynamic_sprite(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	static_sprite(item_data, item_info, item_template);
}
void item_sprite_system::static_sprite(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	auto sprite_data = item_data.mutable_sprite_data();
	for (int32 i = 0; i < e_item_sprite_max; ++i)
	{
		sprite_data->add_data_array(0);
	}
}
void item_sprite_system::generate_client_msg(Entity* item_ent, item_s2c_item_data* msg)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	if (false == item_sprite_cp.isValid())
	{
		return;
	}
	auto sprite_data = msg->mutable_sprite_data();
	for (int32 i = 0; i < e_item_sprite_max; ++i)
	{
		sprite_data->add_data_array(item_sprite_cp->m_data_array[i]);
	}
}
Entity* item_sprite_system::get_sprite_by_slot(player* player_ptr, int32 equip_slot)
{
	if (equip_slot <= 0)
	{
		CONSOLE_ERROR("equip_slot:{} <= 0", equip_slot);
		return nullptr;
	}

	auto& item_manager_cp = player_ptr->m_item_manager_component;
	auto& item_map = item_manager_cp->m_bag_map[e_bag_type_equip_spirit];
	for (auto& it : item_map)
	{
		auto item_sprite_cp = it->get_component<item_sprite_component>();
		if (item_sprite_cp->m_data_array[e_item_sprite_equip_slot] == equip_slot)
		{
			return it;
		}
	}
	return nullptr;
}
void item_sprite_system::change_sprite_att(unit* unit_ptr, Entity* item_ent, bool is_add)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	unit_ptr->get_pawn_att().apply_att_change_by_array(item_sprite_cp->m_sprite_template->BasicProperty, is_add);
}
void item_sprite_system::activate_sprite(player* player_ptr, int32 item_id)
{
	ZoneScoped;
	auto item_template = GET_TEMPLATE(ItemTemplate, item_id);
	if (nullptr == item_template)
	{
		CONSOLE_ERROR("item_template == nullptr item_id:{}", item_id);
		return;
	}

	SpiritTemplate* sprite_template_ptr = GET_TEMPLATE(SpiritTemplate, item_template->logic_id);
	if (sprite_template_ptr == nullptr)
	{
		CONSOLE_ERROR("sprite_template_ptr == nullptr item_id:{} sprite_id:{}", item_id, item_template->logic_id);
		return;
	}
	auto item_ptr = item_system::get_item_by_id(player_ptr, e_bag_type_equip_spirit, item_id);
	if (item_ptr)
	{
		CONSOLE_ERROR("activate_sprite item_ptr is have item_id:{}", item_id);
		return;
	}
	int32 cost_item_id = sprite_template_ptr->UpLevelCost[0];
	int32 cost_item_num = sprite_template_ptr->UpLevelCost[1];
	if (item_system::can_cost_item(player_ptr, e_bag_type_bag, cost_item_id, cost_item_num) == false)
	{
		CONSOLE_ERROR("cost_item_id:{} UpLevelCost:{}", cost_item_id, cost_item_num);
		return;
	}
	item_system::cost_item_from_bag(player_ptr, e_bag_type_bag, cost_item_id, cost_item_num);

	s_item_template_info item_list(item_id, 1, 1);
	auto item_ent = item_system::create_item(player_ptr, item_list);
	if (nullptr == item_ent)
	{
		CONSOLE_ERROR("item_ent == nullptr item_id:{}", item_id);
		return;
	}
	item_system::put_item_into_bag(player_ptr, item_ent, e_bag_type_equip_spirit);
	change_sprite_att(player_ptr, item_ent, true);
	item_system::send_item_one(player_ptr, { item_ent });

	guid_64 sprite_guid = item_ent->getEntityId();
	character_proto_spirit_operation_end sprite_msg;
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_A());
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_B());
	sprite_msg.add_spirit_guid(sprite_guid.A);
	sprite_msg.add_spirit_guid(sprite_guid.B);
	sprite_msg.set_operation_type(e_sprite_operate_equip);
	sprite_msg.set_target_operate_slot(0);
	sprite_msg.set_operation_result(e_error_code_success);
	player_ptr->send_message_to_aoi(&sprite_msg, e_msgindex_s2c_spirit_operation);
}
void item_sprite_system::sprite_level_up(player* player_ptr, Entity* item_ent)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	if (false == item_sprite_cp.isValid())
	{
		CONSOLE_ERROR("item_sprite_cp == nullptr");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();	
	auto sprite_level = item_sprite_cp->m_data_array[e_item_sprite_level];
	auto sprite_template = item_sprite_cp->m_sprite_template;

	auto next_sprite_id = item_base_cp->m_item_template->logic_id + sprite_level + 1;
	auto next_sprite_template = GET_TEMPLATE(SpiritTemplate, next_sprite_id);
	if (nullptr == next_sprite_template)
	{
		CONSOLE_ERROR("next_sprite_template == nullptr item_id:{} next_sprite_id:{}", item_base_cp->m_item_id, next_sprite_id);
		return;
	}

	if (sprite_template->UpLevelCost.size() < 2)
	{
		CONSOLE_ERROR("UpLevelCost size error sprite_id:{} size :{}", sprite_template->attribute_id, sprite_template->UpLevelCost.size());
		return;
	}
	int32 cost_item_id = sprite_template->UpLevelCost[0];
	int32 cost_item_num = sprite_template->UpLevelCost[1];

	if (item_system::can_cost_item(player_ptr, e_bag_type_bag, cost_item_id, cost_item_num) == false)
	{
		CONSOLE_ERROR("sprite_id:{} cost_item_id:{} cost_item_num:{}", sprite_template->attribute_id, cost_item_id, cost_item_num);
		return;
	}
	item_system::cost_item_from_bag(player_ptr, e_bag_type_bag, cost_item_id, cost_item_num);
	change_sprite_att(player_ptr, item_ent, false);
	item_sprite_cp->m_data_array[e_item_sprite_level] += 1;
	item_sprite_cp->m_sprite_template = next_sprite_template;
	change_sprite_att(player_ptr, item_ent, true);
	item_system::send_item_one(player_ptr, { item_ent });

	guid_64 sprite_guid = item_ent->getEntityId();
	character_proto_spirit_operation_end sprite_msg;
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_A());
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_B());
	sprite_msg.add_spirit_guid(sprite_guid.A);
	sprite_msg.add_spirit_guid(sprite_guid.B);
	sprite_msg.set_operation_type(e_sprite_operate_upgrade);
	sprite_msg.set_target_operate_slot(item_sprite_cp->m_data_array[e_item_sprite_equip_slot]);
	sprite_msg.set_operation_result(e_error_code_success);
	player_ptr->send_message_to_aoi(&sprite_msg, e_msgindex_s2c_spirit_operation);
}
void item_sprite_system::equip_on_sprite(player* player_ptr, Entity* item_ent, int32& target_slot)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	if (false == item_sprite_cp.isValid())
	{
		return;
	}
	if (target_slot <= 0)
	{
		CONSOLE_ERROR("target_slot:{} <= 0", target_slot);
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();
	if (item_base_cp->m_data_array[e_item_base_bag_type] != e_bag_type_equip_spirit)
	{
		CONSOLE_ERROR("bag_type:{}", item_base_cp->m_data_array[e_item_base_bag_type]);
		return;
	}
	int32 old_slot = item_sprite_cp->m_data_array[e_item_sprite_equip_slot];
	if (target_slot == old_slot)
	{
		CONSOLE_ERROR("target_slot == old_slot target_slot:{}", target_slot);
		return;
	}
	if (old_slot > 0)
	{
		equip_off_sprite(player_ptr, item_ent, false);
	}
	auto player_upgrade_template = GET_TEMPLATE(PlayerUpgradeTemplate, player_ptr->get_unit_info(e_role_info_upgrade_id));
	if (nullptr == player_upgrade_template)
	{
		CONSOLE_ERROR("player_upgrade_template == nullptr upgrade_id:{}", player_ptr->get_unit_info(e_role_info_upgrade_id));
		return;
	}
	if (target_slot > player_upgrade_template->SpriteCount)
	{
		CONSOLE_ERROR("target_slot:{} player_upgrade_template->SpriteCount:{}", target_slot, player_upgrade_template->SpriteCount);
		return;
	}
	auto old_sprite = get_sprite_by_slot(player_ptr, target_slot);
	if (old_sprite != nullptr)
	{
		equip_off_sprite(player_ptr, old_sprite, true);
	}

	auto sprite_template_ptr = item_sprite_cp->m_sprite_template;

	//被动技能
	for (auto& it : sprite_template_ptr->StarBuffId)
	{
		if (it > 0)
		{
			player_ptr->get_passive_skill().passive_skill_special(it, true);
		}
	}
	item_sprite_cp->m_data_array[e_item_sprite_equip_slot] = target_slot;

	item_system::send_item_one(player_ptr, { item_ent });

	player_ptr->check_and_set_highest_record(e_role_history_highest_record_total_sprite_equip_level);

	guid_64 sprite_guid = item_ent->getEntityId();
	character_proto_spirit_operation_end sprite_msg;
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_A());
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_B());
	sprite_msg.add_spirit_guid(sprite_guid.A);
	sprite_msg.add_spirit_guid(sprite_guid.B);
	sprite_msg.set_operation_type(e_sprite_operate_fight);
	sprite_msg.set_target_operate_slot(target_slot);
	sprite_msg.set_operation_result(e_error_code_success);
	player_ptr->send_message_to_aoi(&sprite_msg, e_msgindex_s2c_spirit_operation);
}
void item_sprite_system::equip_off_sprite(player* player_ptr, Entity* item_ent, bool send_client)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	if (false == item_sprite_cp.isValid())
	{
		CONSOLE_ERROR("item_sprite_cp == nullptr");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();

	if (item_base_cp->m_data_array[e_item_base_bag_type] != e_bag_type_equip_spirit)
	{
		CONSOLE_ERROR("bag_type:{}", item_base_cp->m_data_array[e_item_base_bag_type]);
		return;
	}

	int32 old_slot = item_sprite_cp->m_data_array[e_item_sprite_equip_slot];
	if (old_slot <= 0)
	{
		CONSOLE_ERROR("old_slot:{} <= 0", old_slot);
		return;
	}

	auto sprite_template_ptr = item_sprite_cp->m_sprite_template;

	//被动技能
	for (auto& it : sprite_template_ptr->StarBuffId)
	{
		if (it > 0)
		{
			player_ptr->get_passive_skill().passive_skill_special(it, false);
		}
	}
	item_sprite_cp->m_data_array[e_item_sprite_equip_slot] = 0;
	item_system::send_item_one(player_ptr, { item_ent });

	if (send_client)
	{
		guid_64 sprite_guid = item_ent->getEntityId();
		character_proto_spirit_operation_end sprite_msg;
		sprite_msg.add_role_guid(player_ptr->get_unit_guid_A());
		sprite_msg.add_role_guid(player_ptr->get_unit_guid_B());
		sprite_msg.add_spirit_guid(sprite_guid.A);
		sprite_msg.add_spirit_guid(sprite_guid.B);
		sprite_msg.set_operation_type(e_sprite_operate_unequip);
		sprite_msg.set_target_operate_slot(old_slot);
		sprite_msg.set_operation_result(e_error_code_success);
		player_ptr->send_message_to_aoi(&sprite_msg, e_msgindex_s2c_spirit_operation);
	}
}
void item_sprite_system::set_scene_show(player* player_ptr, Entity* item_ent, int32 target_slot)
{
	ZoneScoped;
	auto item_sprite_cp = item_ent->get_component<item_sprite_component>();
	if (false == item_sprite_cp.isValid())
	{
		CONSOLE_ERROR("item_sprite_cp == nullptr");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();

	if (item_base_cp->m_data_array[e_item_base_bag_type] != e_bag_type_equip_spirit)
	{
		CONSOLE_ERROR("bag_type:{}", item_base_cp->m_data_array[e_item_base_bag_type]);
		return;
	}
	int32 self_slot = item_sprite_cp->m_data_array[e_item_sprite_equip_slot];
	if (self_slot <= 0)
	{
		CONSOLE_ERROR("self_slot:{} <= 0", self_slot);
		return;
	}
	if (self_slot == target_slot)
	{
		CONSOLE_ERROR("self_slot == target_slot old_slot:{}", self_slot, target_slot);
		return;
	}
	auto target_sprite = get_sprite_by_slot(player_ptr, target_slot);
	if (target_sprite != nullptr)
	{
		auto target_sprite_cp = target_sprite->get_component<item_sprite_component>();
		target_sprite_cp->m_data_array[e_item_sprite_equip_slot] = self_slot;
		item_system::send_item_one(player_ptr, { target_sprite });
	}
	item_sprite_cp->m_data_array[e_item_sprite_equip_slot] = target_slot;
	item_system::send_item_one(player_ptr, { item_ent });

	guid_64 sprite_guid = item_ent->getEntityId();
	character_proto_spirit_operation_end sprite_msg;
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_A());
	sprite_msg.add_role_guid(player_ptr->get_unit_guid_B());
	sprite_msg.add_spirit_guid(sprite_guid.A);
	sprite_msg.add_spirit_guid(sprite_guid.B);
	sprite_msg.set_operation_type(e_sprite_operate_equip);
	sprite_msg.set_target_operate_slot(target_slot);
	sprite_msg.set_operation_result(e_error_code_success);
	player_ptr->send_message_to_aoi(&sprite_msg, e_msgindex_s2c_spirit_operation);
}