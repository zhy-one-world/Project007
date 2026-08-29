#include "components/play/finger_guess_component.h"
#include "finger_guess_system.h"
#include "internet/finger_guess.pb.h"
#include "logic/player.hpp"
#include "lua/script_mgr.h"
#include "internet/net.pb.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/random.h"

using namespace faith;

bool finger_guess_system::start_up(player* player_ptr)
{
	player_ptr->m_finger_guess_component = std::make_shared<finger_guess_component>();
	auto& finger_guess_rate = GAMECONFIG->FingerPlayRate;
	auto rate = random_gen::get_random(1, 1000000);
	int32 win_count = 2;
	for (auto& it : finger_guess_rate)
	{
		if (rate <= it)
		{
			break;
		}
		win_count--;
	}
	player_ptr->m_finger_guess_component->end_win = win_count;
	finger_guess_s2c_finger_guess_info msg;
	msg.set_win_end(win_count);
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_finger_guess_info);
	return true;
}
void finger_guess_system::shut_down(player* player_ptr)
{
	player_ptr->m_finger_guess_component = nullptr;
}
void finger_guess_system::update()
{

}
void finger_guess_system::player_off_line(player* player_ptr)
{
	auto& finger_guess_component = player_ptr->m_finger_guess_component;
	if (finger_guess_component == nullptr)
	{
		return;
	}
	player_ptr->cut_money(e_money_type_silver_bind, 100, e_server_log_cut_money_finger_guess_offline);
	shut_down(player_ptr);
}
void finger_guess_system::guess_end(player* player_ptr)
{
	auto& finger_guess_component = player_ptr->m_finger_guess_component;
	if (finger_guess_component == nullptr)
	{
		CONSOLE_ERROR("finger_guess_component is nullptr");
		return;
	}
	int32 ret = 0;
	int32 money_type = 0;
	int32 money_num = 0;
	int32 win = 0;
	if (finger_guess_component->end_win > 0)
	{
		win = 1;
		script_mgr::get_instance().call_func("formula_calculation_mgr", "finger_guess_award", 3, false, "%d>%d%d%d", finger_guess_component->end_win, &ret, &money_type, &money_num);
		player_ptr->add_money(e_money_type(money_type), money_num, e_server_log_add_money_finger_guess, finger_guess_component->end_win);
		std::vector<s_item_template_info> item_list;
		std::vector<s_item_template_info> money_list;
		money_list.push_back({ money_type, money_num });
	}
	else
	{
		script_mgr::get_instance().call_func("formula_calculation_mgr", "finger_guess_punishment", 3, false, "%d>%d%d%d", player_ptr->get_unit_info(e_role_info_exp_level), &ret, &money_type, &money_num);
		player_ptr->cut_money((e_money_type)money_type, money_num, e_server_log_cut_money_finger_guess);
		money_num = -money_num;
	}
	shut_down(player_ptr);
	finger_guess_s2c_finger_guess_end msg;
	msg.set_win_end(win);
	msg.set_money_id(money_type);
	msg.set_money_count(money_num);
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_finger_guess_end);
}