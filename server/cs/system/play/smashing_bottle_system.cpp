#include "components/play/smashing_bottle_component.h"
#include "internet/net.pb.h"
#include "internet/smashing_bottle.pb.h"
#include "logic/player.hpp"

#include "server_log.hpp"
#include "smashing_bottle_system.h"
#include "template/template_manager.h"
#include "utility/random.h"

using namespace faith;

const int32 smashing_bottle_play_count = 3;//�Ҿ�ƿ�غ���

bool smashing_bottle_system::start_up(player* player_ptr, int32 flip_box_id)
{
	player_ptr->m_smashing_bottle_component = std::make_shared<smashing_bottle_component>();
	auto& smashing_bottle_rate = GAMECONFIG->SmashingBottleRate;
	auto random_win = random_gen::get_random(1, 1000000);
	auto random_bottle = random_gen::get_random(1, 1000000);
	int32 random_sum = 0;
	int32 bottle_num = 0;
	int32 win_index = 0;
	for (int32 i = 0; i < smashing_bottle_play_count; ++i)
	{
		random_sum += smashing_bottle_rate[i];
		if (random_win <= random_sum)
		{
			win_index = i + 1;
			break;
		}
	}
	random_sum = 0;
	if (win_index > 0)
	{
		for (int32 i = smashing_bottle_play_count; i < smashing_bottle_play_count + smashing_bottle_play_count * 2; i += 2)
		{
			random_sum += smashing_bottle_rate[i + 1];
			if (random_bottle <= random_sum)
			{
				bottle_num = smashing_bottle_rate[i];
				break;
			}
		}
	}
	else
	{
		for (int32 i = smashing_bottle_play_count + smashing_bottle_play_count * 2; i < smashing_bottle_play_count + smashing_bottle_play_count * 4; i += 2)
		{
			random_sum += smashing_bottle_rate[i + 1];
			if (random_bottle <= random_sum)
			{
				bottle_num = smashing_bottle_rate[i];
				break;
			}
		}

	}

	player_ptr->m_smashing_bottle_component->bottle_num = bottle_num;
	player_ptr->m_smashing_bottle_component->end_win = win_index;
	smashing_bottle_s2c_smashing_bottle_info msg;
	msg.set_bottle_num(bottle_num);
	msg.set_win_end(win_index);
	msg.set_flip_box_id(flip_box_id);
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_smashing_bottle_info);
	return true;
}
void smashing_bottle_system::shut_down(player* player_ptr)
{
	player_ptr->m_smashing_bottle_component = nullptr;
}
void smashing_bottle_system::update()
{

}
void smashing_bottle_system::player_off_line(player* player_ptr)
{
	auto& smashing_bottle_cp = player_ptr->m_smashing_bottle_component;
	if (smashing_bottle_cp == nullptr)
	{
		return;
	}
	int32 ret = 0;
	int32 money_type = 0;
	int32 money_num = 0;
	script_mgr::get_instance().call_func("formula_calculation_mgr", "smashing_bottle_punishment", 3, false, "%d>%d%d%d", player_ptr->get_unit_info(e_role_info_exp_level), &ret, &money_type, &money_num);
	player_ptr->cut_money((e_money_type)money_type, money_num, e_server_log_cut_money_smashing_bottle_offline);
	shut_down(player_ptr);
}
void smashing_bottle_system::guess_end(player* player_ptr)
{
	auto& smashing_bottle_cp = player_ptr->m_smashing_bottle_component;
	if (smashing_bottle_cp == nullptr)
	{
		CONSOLE_ERROR("smashing_bottle_cp is nullptr");
		return;
	}
	int32 ret = 0;
	int32 money_type = 0;
	int32 money_num = 0;
	int32 win = 0;
	if (smashing_bottle_cp->end_win > 0)
	{
		win = 1;
		script_mgr::get_instance().call_func("formula_calculation_mgr", "smashing_bottle_award", 3, false, "%d>%d%d%d", smashing_bottle_play_count - smashing_bottle_cp->end_win + 1, &ret, &money_type, &money_num);
		player_ptr->add_money(e_money_type(money_type), money_num, e_server_log_add_money_finger_guess, smashing_bottle_cp->end_win);
		std::vector<s_item_template_info> item_list;
		std::vector<s_item_template_info> money_list;
		money_list.push_back({ money_type, money_num });
	}
	else
	{
		script_mgr::get_instance().call_func("formula_calculation_mgr", "smashing_bottle_punishment", 3, false, "%d>%d%d%d", player_ptr->get_unit_info(e_role_info_exp_level), &ret, &money_type, &money_num);
		player_ptr->cut_money((e_money_type)money_type, money_num, e_server_log_cut_money_smashing_bottle);
		money_num = -money_num;
	}
	shut_down(player_ptr);
	smashing_bottle_s2c_smashing_bottle_end msg;
	msg.set_win_end(win);
	msg.set_money_id(money_type);
	msg.set_money_count(money_num);
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_smashing_bottle_end);
}