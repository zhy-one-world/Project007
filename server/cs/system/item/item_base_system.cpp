#include "components/item/item_base_component.h"
#include "internet/item.pb.h"
#include "item_base_system.h"
#include "item_def.hpp"
#include "logic/unit.hpp"
#include "server_log.hpp"

using namespace hld;

void item_base_system::start_up(unit* unit_ptr, Entity* item_ent, ItemTemplate* item_template, const item_s2s_item_data& msg)
{
	ZoneScoped;
	auto item_base_cp = item_ent->add_component<item_base_component>();
	item_base_cp->m_item_template = item_template;
	item_base_cp->m_item_id = item_template->attribute_id;
	item_base_cp->m_owner_ptr = unit_ptr;
	memset(item_base_cp->m_data_array, 0, sizeof(item_base_cp->m_data_array));
	if (msg.has_base_data())
	{
		auto& base_data = msg.base_data();
		for (int32 i = 0; i < e_item_base_max && i < base_data.data_array_size(); ++i)
		{
			item_base_cp->m_data_array[i] = base_data.data_array(i);
		}
	}
}
void item_base_system::shut_down(Entity* item_ent)
{
	ZoneScoped;
	item_ent->remove_component<item_base_component>();
}
void item_base_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
}
void item_base_system::save_item_to_db(Entity* item_ent, item_s2s_item_data* msg)
{
	ZoneScoped;
	auto item_base_cp = item_ent->get_component<item_base_component>();
	msg->set_item_guid(item_ent->getEntityId());
	msg->set_item_id(item_base_cp->m_item_id);
	auto base_data = msg->mutable_base_data();
	for (int32 i = 0; i < e_item_base_max; ++i)
	{
		base_data->add_data_array(item_base_cp->m_data_array[i]);
	}
}
void item_base_system::generate_base_msg(item_s2s_item_data& msg, const s_item_template_info& item_info)
{
	ZoneScoped;
	auto base_data = msg.mutable_base_data();
	for (int32 i = 0; i < e_item_base_max; ++i)
	{
		base_data->add_data_array(0);
	}	
	auto item_guid = guid_gen::make_guid();
	msg.set_item_guid(item_guid.server_64);
	msg.set_item_id(item_info.m_item_id);
	base_data->set_data_array(e_item_base_bag_type, item_info.m_bag_type);
	base_data->set_data_array(e_item_base_count, item_info.m_item_num);
	base_data->set_data_array(e_item_base_locked, item_info.m_lock);
}
void item_base_system::generate_client_msg(Entity* item_ent, item_s2c_item_data* msg)
{
	auto item_base_cp = item_ent->get_component<item_base_component>();
	if (false == item_base_cp.isValid())
	{
		CONSOLE_ERROR("item_base_cp is nullptr");
		return;
	}
	msg->set_item_guid(item_ent->getEntityId());
	msg->set_item_id(item_base_cp->m_item_id);
	auto base_data = msg->mutable_base_data();
	for (int32 i = 0; i < e_item_base_max; ++i)
	{
		base_data->add_data_array(item_base_cp->m_data_array[i]);
	}
}