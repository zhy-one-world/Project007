#include "components/item/item_base_component.h"
#include "components/item/item_manager_component.h"
#include "internal/char_msg.hpp"
#include "internet/item.pb.h"
#include "item_base_system.h"
#include "item_element_system.h"
#include "item_equip_system.h"
#include "item_prop_system.h"
#include "item_sprite_system.h"
#include "item_system.h"
#include "logic/player.hpp"
#include "logic/unit.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"

using namespace faith;

void item_system::load_data_over(unit* unit_ptr)
{
	auto& item_manager_cp = unit_ptr->m_item_manager_component;
	auto& equip_item_map = item_manager_cp->m_bag_map[e_bag_type_equip];
	for (auto& it : equip_item_map)
	{
		item_equip_system::change_equip_att(unit_ptr, it, true);
	}
	auto& sprite_item_map = item_manager_cp->m_bag_map[e_bag_type_equip_spirit];
	for (auto& it : sprite_item_map)
	{
		item_sprite_system::change_sprite_att(unit_ptr, it, true);
	}
	auto& element_item_map = item_manager_cp->m_bag_map[e_bag_type_equip_elemenet];
	for (auto& it : element_item_map)
	{
		item_element_system::change_element_att(unit_ptr, it, true);
	}
}
void item_system::save_data_to_db(player* player_ptr, int32 save_type_ex)
{
	ZoneScoped;
	auto& unit_item_cp = player_ptr->m_item_manager_component;
	if (nullptr == unit_item_cp)
	{
		CONSOLE_ERROR("unit_item_cp is nullptr player_guid:{}", player_ptr->get_unit_guid().server_64);
		return;
	}
	for (int32 i = e_bag_type_none + 1; i < e_bag_type_max; ++i)
	{
		auto& item_map = unit_item_cp->m_bag_map[e_bag_type(i)];
		item_s2s_sl_item_info msg;
		for (auto& item_it : item_map)
		{
			auto item_msg = msg.add_item_list();
			auto item_del_cp = item_it->get_component<item_del_component>();
			if (item_del_cp.isValid() == false)
			{
				item_base_system::save_item_to_db(item_it, item_msg);
				item_equip_system::save_item_to_db(item_it, item_msg);
				item_sprite_system::save_item_to_db(item_it, item_msg);
				item_prop_system::save_item_to_db(item_it, item_msg);
				item_element_system::save_item_to_db(item_it, item_msg);
			}
		}
		player_ptr->send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_item + i, save_type_ex);
	}
}
void item_system::del_item_entity(Entity* item_ent)
{
	ZoneScoped;
	item_equip_system::shut_down(item_ent);
	item_sprite_system::shut_down(item_ent);
	item_prop_system::shut_down(item_ent);
	item_element_system::shut_down(item_ent);
	item_base_system::shut_down(item_ent);
	destroy_entity(item_ent);
}
void item_system::generate_client_msg(Entity* item_ent, item_s2c_item_data* msg)
{
	ZoneScoped;
	item_base_system::generate_client_msg(item_ent, msg);
	item_equip_system::generate_client_msg(item_ent, msg);
	item_sprite_system::generate_client_msg(item_ent, msg);
	item_prop_system::generate_client_msg(item_ent, msg);
	item_element_system::generate_client_msg(item_ent, msg);
}
void item_system::create_item_msg(item_s2s_item_data& msg, unit* unit_ptr, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	item_base_system::generate_base_msg(msg, item_info);
	switch (item_template->item_type)
	{
	case e_item_type_armor:
	case e_item_type_weapon:
	{
		auto player_ptr = dynamic_cast<player*>(unit_ptr);
		if (player_ptr)
		{
			item_equip_system::dynamic_equip(msg, player_ptr, item_info, item_template);
		}
		else
		{
			item_equip_system::static_equip(msg, item_info, item_template);
		}
	}
	break;
	case e_item_type_spirit:
	{
		auto player_ptr = dynamic_cast<player*>(unit_ptr);
		if (player_ptr)
		{
			item_sprite_system::dynamic_sprite(msg, player_ptr, item_info, item_template);
		}
		else
		{
			item_sprite_system::static_sprite(msg, item_info, item_template);
		}
	}
	break;
	case e_item_type_expendable:
	{
		auto player_ptr = dynamic_cast<player*>(unit_ptr);
		if (player_ptr)
		{
			item_prop_system::dynamic_prop(msg, player_ptr, item_info, item_template);
		}
		else
		{
			item_prop_system::static_prop(msg, item_info, item_template);
		}
	}
	break;
	case e_item_type_element_heart:
	{
		auto player_ptr = dynamic_cast<player*>(unit_ptr);
		if (player_ptr)
		{
			item_element_system::dynamic_element(msg, player_ptr, item_info, item_template);
		}
		else
		{
			item_element_system::static_element(msg, item_info, item_template);
		}
	}
	default:
		CONSOLE_INFO("item_type is invalid item_id:{} item_type:{}", item_info.m_item_id, item_template->item_type);
		break;
	}
}
Entity* item_system::create_item(unit* unit_ptr, const item_s2s_item_data& msg)
{
	ZoneScoped;
	auto item_template = GET_TEMPLATE(ItemTemplate, msg.item_id());
	if (nullptr == item_template)
	{
		CONSOLE_ERROR("item_template is nullptr, item_id = %d", msg.item_id());
		return nullptr;
	}
	auto item_ent = create_entity();
	switch (item_template->item_type)
	{
	case e_item_type_armor:
	case e_item_type_weapon:
	{
		auto equip_template = GET_TEMPLATE(EquipTemplate, item_template->logic_id);
		if (nullptr == equip_template)
		{
			CONSOLE_ERROR("equip_template is nullptr, item_id:{} equip_id:{}", msg.item_id(), item_template->logic_id);
			destroy_entity(item_ent);
			return nullptr;
		}
		item_base_system::start_up(unit_ptr, item_ent, item_template, msg);
		item_equip_system::start_up(item_ent, item_template, equip_template, msg);
	}
	break;
	case e_item_type_spirit:
	{
		auto sprite_template = GET_TEMPLATE(SpiritTemplate, item_template->logic_id + msg.sprite_data().data_array(e_item_sprite_level));
		if (nullptr == sprite_template)
		{
			CONSOLE_ERROR("sprite_template is nullptr, item_id:{} sprite_id:{}", msg.item_id(), item_template->logic_id);
			destroy_entity(item_ent);
			return nullptr;
		}
		item_base_system::start_up(unit_ptr, item_ent, item_template, msg);
		item_sprite_system::start_up(item_ent, item_template, sprite_template, msg);
	}
	break;
	case e_item_type_expendable:
	{
		auto prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, item_template->logic_id);
		if (nullptr == prop_template)
		{
			CONSOLE_ERROR("prop_template is nullptr, item_id:{}, prop_id:{}", msg.item_id(), item_template->logic_id);
			destroy_entity(item_ent);
			return nullptr;
		}
		item_base_system::start_up(unit_ptr, item_ent, item_template, msg);
		item_prop_system::start_up(item_ent, item_template, prop_template, msg);
	}
	break;
	case e_item_type_element_heart:
	{
		auto element_template = GET_TEMPLATE(ElementHeartTemplate, item_template->logic_id);
		if (nullptr == element_template)
		{
			CONSOLE_ERROR("element_template is nullptr, item_id:{} element_id:{}", msg.item_id(), item_template->logic_id);
			destroy_entity(item_ent);
			return nullptr;
		}
		item_base_system::start_up(unit_ptr, item_ent, item_template, msg);
		item_element_system::start_up(item_ent, item_template, element_template, msg);
	}
	break;
	default:
		CONSOLE_ERROR("item_type is invalid item_id:{} item_type:{}", msg.item_id(), item_template->item_type);
		destroy_entity(item_ent);
		return nullptr;
	}
	return item_ent;
}