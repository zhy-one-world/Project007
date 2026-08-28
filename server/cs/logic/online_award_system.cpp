#include "online_award_system.h"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "logic/drop.h"
#include "system/item/item_system.h"
#include "components/item/item_base_component.h"

using namespace hld;

bool  online_award_system::start_up(player* player_ptr)
{
	return true;
}
void online_award_system::shut_down(player* player_ptr)
{

}
void online_award_system::heart_tick(player* player_ptr, const int64& new_time, const int32& tick_time)
{
	ZoneScoped;
	auto map_template_id = player_ptr->get_unit_info(hld::e_role_info_main_map_id);
	auto class_type = player_ptr->get_unit_info(e_role_info_class_type);
	auto exp_level = player_ptr->get_unit_info(e_role_info_exp_level);

	auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
	if (map_template == nullptr)
	{
		CONSOLE_ERROR("map_template is nullptr map_template_id:{}", map_template_id);
		return;
	}	
	auto award_template = GET_TEMPLATE(MapAwardTemplate, map_template->OnlineAward);
	if (award_template == nullptr)
	{
		CONSOLE_ERROR("award_template is nullptr award_template_id:{}", map_template->OnlineAward);
		return;
	}
	auto online_time = player_ptr->get_logic_data(e_role_logic_info_online_award_time);
	auto money_count = player_ptr->get_logic_data(e_role_logic_info_online_award_money_count);
	auto item_count = player_ptr->get_logic_data(e_role_logic_info_online_award_item_count);
	online_time += tick_time;
	if (online_time >= GAMECONFIG->OnlineTime[2] * 1000)
	{
		online_time = GAMECONFIG->OnlineTime[2] * 1000;
	}
	player_ptr->set_logic_data(e_role_logic_info_online_award_time, online_time);
	auto online_money_count = online_time / (GAMECONFIG->OnlineTime[0] * 1000);

	if (online_money_count > money_count)
	{
		auto loop_count = online_money_count - money_count;
		auto money_type = award_template->MoneyList[0];
		auto money_num = award_template->MoneyList[1] * loop_count;
		auto old_money_num = player_ptr->get_logic_data(e_role_logic_info_online_award_money_num);

		player_ptr->set_logic_data(e_role_logic_info_online_award_money_type, money_type);
		player_ptr->set_logic_data(e_role_logic_info_online_award_money_num, old_money_num + money_num);
		player_ptr->set_logic_data(e_role_logic_info_online_award_money_count, online_money_count);

		player_ptr->send_logic_one(e_role_logic_info_online_award_money_type);
		player_ptr->send_logic_one(e_role_logic_info_online_award_money_num);
		player_ptr->send_logic_one(e_role_logic_info_online_award_time);
	}
	auto online_item_count = online_time / (GAMECONFIG->OnlineTime[1] * 1000);

	if (online_item_count > item_count)
	{
		auto loop_count = online_item_count - item_count;
		std::vector<s_item_template_info> item_list;
		for (int32 i = 0; i < loop_count; i++)
		{
			cdrop::gen_drop_list_by_drop_id(award_template->DropId, item_list, 0, exp_level);
		}
		if (item_list.empty() == false)
		{
			for (auto& it : item_list)
			{
				it.m_bag_type = e_bag_type_online_award;
			}
			item_system::put_item_into_bag(player_ptr, item_list);
		}
		player_ptr->set_logic_data(e_role_logic_info_online_award_item_count, online_item_count);
	}
}
void online_award_system::get_online_award(player* player_ptr)
{
	ZoneScoped;
	std::vector<s_item_template_info> item_list;
	std::vector<s_item_template_info> money_list;
	auto money_type = player_ptr->get_logic_data(e_role_logic_info_online_award_money_type);
	auto money_num = player_ptr->get_logic_data(e_role_logic_info_online_award_money_num);
	if (money_num > 0)
	{
		money_list.push_back({ money_type, money_num });
		player_ptr->add_money((e_money_type)money_type, money_num, e_server_log_add_money_online_award, -1, true);
	}
	auto bag_item_list = item_system::take_out_item(player_ptr, e_bag_type_online_award);
	for (auto& it : bag_item_list)
	{
		auto item_base_cp = it->get_component<item_base_component>();
		item_list.push_back({ item_base_cp->m_item_id,item_base_cp->m_data_array[e_item_base_count],item_base_cp->m_data_array[e_item_base_locked] });
	}
	item_system::put_item_into_bag(player_ptr, bag_item_list, e_bag_type_bag);
	item_system::send_promp_msg_to_client(player_ptr, item_list, money_list);
	if (item_list.empty() == false || money_list.empty() == false)
	{
		player_ptr->set_logic_data(e_role_logic_info_online_award_money_type, 0);
		player_ptr->set_logic_data(e_role_logic_info_online_award_money_num, 0);
		player_ptr->set_logic_data(e_role_logic_info_online_award_time, 0);
		player_ptr->set_logic_data(e_role_logic_info_online_award_money_count, 0);
		player_ptr->set_logic_data(e_role_logic_info_online_award_item_count, 0);

		player_ptr->send_logic_one(e_role_logic_info_online_award_money_type);
		player_ptr->send_logic_one(e_role_logic_info_online_award_money_num);
		player_ptr->send_logic_one(e_role_logic_info_online_award_time);
	}
}