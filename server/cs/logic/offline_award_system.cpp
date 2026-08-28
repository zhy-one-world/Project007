#include "logic/drop.h"
#include "offline_award_system.h"
#include "utility/random.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "internet/map_award.pb.h"
#include "internet/net.pb.h"

using namespace hld;

bool  offline_award_system::start_up(player* player_ptr)
{
	return true;
}
void offline_award_system::shut_down(player* player_ptr)
{

}
void offline_award_system::player_online(player* player_ptr)
{
	ZoneScoped;
	auto logout_time = player_ptr->get_time_data(e_time_type_logout_time);
	auto login_time = player_ptr->get_time_data(e_time_type_login_time);
	auto offline_time = player_ptr->get_time_data(e_time_type_offline_award_time) == 0 ? logout_time : player_ptr->get_time_data(e_time_type_offline_award_time);
	if (offline_time >= logout_time)
	{
		player_ptr->set_time_data(e_time_type_offline_award_time, logout_time);
		return;
	}
	player_ptr->set_time_data(e_time_type_offline_award_time, logout_time);

	auto offline_tick = login_time - logout_time;
	auto sum_offline_time = player_ptr->get_logic_data(e_role_logic_info_offline_award_time);

	if (offline_tick > 0)
	{
		sum_offline_time += offline_tick;
		if (sum_offline_time >= GAMECONFIG->OfflineTime[2])
		{
			sum_offline_time = GAMECONFIG->OfflineTime[2];
		}
	}
	player_ptr->set_logic_data(e_role_logic_info_offline_award_time, sum_offline_time);
	send_drop_item(player_ptr);
}
s_offline_res offline_award_system::get_drop_item(player* player_ptr)
{
	ZoneScoped;
	s_offline_res res;
	auto sum_offline_time = player_ptr->get_logic_data(e_role_logic_info_offline_award_time);
	auto offline_time_random = player_ptr->get_logic_data(e_role_logic_info_offline_award_random);
	auto now_tick = utility::get_tick_count();

	if (sum_offline_time >= GAMECONFIG->OfflineTime[1])
	{
		if (offline_time_random > 0)
		{
			random_gen::set_seed(offline_time_random);
		}
		else
		{
			offline_time_random = now_tick / 10000;
			player_ptr->set_logic_data(e_role_logic_info_offline_award_random, offline_time_random);
			random_gen::set_seed(offline_time_random);
		}
	}
	else
	{
		return res;
	}
	int32 money_offline_count = sum_offline_time / GAMECONFIG->OfflineTime[0];
	int32 item_offline_count = sum_offline_time / GAMECONFIG->OfflineTime[1];
	auto map_template_id = player_ptr->get_unit_info(hld::e_role_info_main_map_id);
	auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
	if (map_template == nullptr)
	{
		CONSOLE_ERROR("map_template is nullptr map_template_id:{}", map_template_id);
		return res;
	}
	auto award_template_id = map_template->OfflineAward;
	auto class_type = player_ptr->get_unit_info(e_role_info_class_type);
	auto exp_level = player_ptr->get_unit_info(e_role_info_exp_level);
	auto award_template = GET_TEMPLATE(MapAwardTemplate, award_template_id);
	if (award_template == nullptr)
	{
		CONSOLE_ERROR("award_template is nullptr award_template_id:{}", award_template_id);
		return res;
	}
	for (int32 i = 0; i < item_offline_count; i++)
	{
		auto curstep = 1;
		cdrop::gen_drop_list_by_drop_id(award_template->DropId, res.item_list, 0, exp_level);
	}

	res.money_list.push_back({ award_template->MoneyList[0],award_template->MoneyList[1] * money_offline_count });

	random_gen::set_seed(now_tick);
	return res;
}
void offline_award_system::gain_drop_item(player* player_ptr)
{
	auto drop_res = get_drop_item(player_ptr);

	item_system::put_item_into_bag(player_ptr, drop_res.item_list);

	for (auto& money : drop_res.money_list)
	{
		player_ptr->add_money((e_money_type)money.m_item_id, money.m_item_num, e_server_log_add_money_offline_award, -1, true);
	}
	player_ptr->send_promp_msg_to_client(drop_res.item_list, drop_res.money_list);
	player_ptr->set_logic_data(e_role_logic_info_offline_award_time, 0);
	player_ptr->set_logic_data(e_role_logic_info_offline_award_random, 0);
	return;
}

void offline_award_system::send_drop_item(player* player_ptr)
{
	auto drop_res = get_drop_item(player_ptr);

	offline_award_proto_award_list offline_award_msg;
	for (auto& item : drop_res.item_list)
	{
		auto item_msg = offline_award_msg.add_item_list();
		item_msg->set_item_template_id(item.m_item_id);
		item_msg->set_item_num(item.m_item_num);
		item_msg->set_item_lock(item.m_lock);
	}
	for (auto& money : drop_res.money_list)
	{
		auto money_msg = offline_award_msg.add_money_list();
		money_msg->set_money_type(money.m_item_id);
		money_msg->set_money_num(money.m_item_num);
	}
	offline_award_msg.set_offline_time(player_ptr->get_logic_data(e_role_logic_info_offline_award_time));
	player_ptr->send_message_to_self(&offline_award_msg, e_msgindex_s2c_send_offline_award);
}