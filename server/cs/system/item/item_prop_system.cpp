#include "components/item/item_base_component.h"
#include "components/item/item_prop_component.h"
#include "utility/globle_data.h"
#include "internet/item.pb.h"
#include "item_prop_system.h"
#include "item_system.h"
#include "logic/drop.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"

using namespace hld;
void item_prop_system::start_up(Entity* item_ent, ItemTemplate* item_template, PropBasicAttributeTemplate* prop_template, const item_s2s_item_data& msg)
{
	ZoneScoped;
	auto item_prop_cp = item_ent->add_component<item_prop_component>();
	memset(item_prop_cp->m_data_array, 0, sizeof(item_prop_cp->m_data_array));
	if (msg.has_prop_data())
	{
		auto& prop_data = msg.prop_data();
		for (int32 i = 0; i < e_item_prop_max && i < prop_data.data_array_size(); ++i)
		{
			item_prop_cp->m_data_array[i] = prop_data.data_array(i);
		}
		item_prop_cp->m_prop_template = prop_template;
	}
}
void item_prop_system::shut_down(Entity* item_ent)
{
	ZoneScoped;
	item_ent->remove_component<item_prop_component>();

}
void item_prop_system::heart_tick(const int64& new_time)
{
	ZoneScoped;

}
void item_prop_system::save_item_to_db(Entity* item_ent, item_s2s_item_data* msg)
{
	ZoneScoped;
	auto item_prop_cp = item_ent->get_component<item_prop_component>();
	if (false == item_prop_cp.isValid())
	{
		return;
	}
	auto prop_data = msg->mutable_prop_data();
	for (int32 i = 0; i < e_item_prop_max; ++i)
	{
		prop_data->add_data_array(item_prop_cp->m_data_array[i]);
	}
}
void item_prop_system::dynamic_prop(item_s2s_item_data& item_data, player* player_ptr, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	static_prop(item_data, item_info, item_template);
}
void item_prop_system::static_prop(item_s2s_item_data& item_data, const s_item_template_info& item_info, ItemTemplate* item_template)
{
	ZoneScoped;
	auto prop_data = item_data.mutable_prop_data();
	for (int32 i = 0; i < e_item_prop_max; ++i)
	{
		prop_data->add_data_array(0);
	}
}
void item_prop_system::generate_client_msg(Entity* item_ent, item_s2c_item_data* msg)
{
	auto item_prop_cp = item_ent->get_component<item_prop_component>();
	if (false == item_prop_cp.isValid())
	{
		return;
	}
	auto prop_data = msg->mutable_prop_data();
	for (int32 i = 0; i < e_item_prop_max; ++i)
	{
		prop_data->add_data_array(item_prop_cp->m_data_array[i]);
	}
}

void item_prop_system::item_use(player* player_ptr, Entity* item_ent, int32 use_num)
{
	auto item_prop_cp = item_ent->get_component<item_prop_component>();
	if (false == item_prop_cp.isValid())
	{
		CONSOLE_ERROR("item_prop_cp is null");
		return;
	}
	auto item_base_cp = item_ent->get_component<item_base_component>();
	if (use_num <= 0 || use_num > item_base_cp->m_data_array[e_item_base_count])
	{
		use_num = item_base_cp->m_data_array[e_item_base_count];
	}
	auto prop_template = item_prop_cp->m_prop_template;
	if (prop_template->UseType == e_item_use_type_can_not_use || prop_template->UseType == e_item_use_type_only_jump)
	{
		CONSOLE_ERROR("prop_id:{} UseType:{}", prop_template->attribute_id, prop_template->UseType);
		return;
	}

	// 使钱财类的改变生效
	for (int32 i = 0; i < prop_template->MoneyChange.size(); i += 2)
	{
		int32 money_type = prop_template->MoneyChange[i];
		int64 money_num = prop_template->MoneyChange[i + 1] * use_num;
		player_ptr->add_money_or_exp((e_money_type)money_type, money_num, e_server_log_add_money_money_card);
	}
	switch (item_base_cp->m_item_template->sub_type)
	{
	case e_prop_sub_type_gife:
	{
		auto region_template = globle_data::get_instance().get_region_template_ptr();

		std::vector<s_item_info> filter_items_array;
		s_item_info item_data[chat_max_item];
		std::string item_names_str;
		item_names_str.reserve(100);
		int32 package_index = 0;
		int32 item_data_num = 0;
		for (int32 package_index = 0; package_index < use_num; package_index++)
		{
			std::vector<s_item_template_info> drop_list;
			cdrop::gen_drop_list_by_drop_id(prop_template->ItemGet, drop_list, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));
			item_system::put_item_into_bag(player_ptr, drop_list);
		}
	}
	break;
	default:
		break;
	}
	item_system::cost_item_by_ent(item_ent);
}
