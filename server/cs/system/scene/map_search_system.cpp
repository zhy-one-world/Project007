#include "map_search_system.h"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "logic/player.hpp"

using namespace faith;

bool  map_search_system::start_up(int32 player_index)
{
	return true;
}
void map_search_system::shut_down(int32 player_index)
{
}
void map_search_system::heart_tick(int32 player_index, const int64& new_time, const int32& tick_time)
{
	ZoneScoped;	
	auto& player = unit_man::get_player(player_index);
	if (player.is_valid() == false)
	{
		CONSOLE_ERROR("player is invalid player_index:{}", player_index);
		return;
	}
	auto energy_time = player.get_time_data(e_time_type_energy_back);
	auto diff_time = new_time - energy_time;
	auto player_upgrade_template = template_manager::get_instance().get_player_upgrade_by_level(player.get_unit_info(e_role_info_class_type), player.get_unit_info(e_role_info_exp_level));
	if (player_upgrade_template == nullptr)
	{
		CONSOLE_ERROR("player_upgrade_template is nullptr class_type:{} exp_level:{}", player.get_unit_info(e_role_info_class_type), player.get_unit_info(e_role_info_exp_level));
		return;
	}
	if (diff_time >= player_upgrade_template->EnergyBackTime * 1000)
	{
		auto cur_energy = player.get_money_data(e_money_type_energy);
		auto add_energy  = player_upgrade_template->EnergyBackValue * diff_time / (player_upgrade_template->EnergyBackTime * 1000);
		if (cur_energy + add_energy > player_upgrade_template->EnergyMax)
		{
			add_energy = player_upgrade_template->EnergyMax - cur_energy;
		}
		if (add_energy > 0)
		{
			player.add_money(e_money_type_energy, add_energy, e_server_log_add_money_energy_tick, -1, true);
		}

		player.set_time_data(e_time_type_energy_back, new_time);
		player.send_time_one(e_time_type_energy_back);
	}
}
int32 map_search_system::upgrade_search_level(int32 player_index)
{
	ZoneScoped;
	auto& player = unit_man::get_player(player_index);
	if (player.is_valid() == false)
	{
		CONSOLE_ERROR("player is invalid player_index:{}", player_index);
		return 1;
	}
	auto search_level = player.get_logic_data(e_role_logic_info_search_level);
	auto search_level_template = GET_TEMPLATE(SearchLevelTemplate, search_level + search_level_template_id_start);
	if (nullptr == search_level_template || search_level_template->UpgradeMoney.size() < 2)
	{
		CONSOLE_ERROR("search_level_template is null search_level:{} player_index:{}", search_level, player_index);
		return 2;
	}
	e_money_type money_type = e_money_type(search_level_template->UpgradeMoney[0]);
	int32 money_num = search_level_template->UpgradeMoney[1];
	search_level++;	
	if (player.can_cut_money(money_type, money_num) == false)
	{
		CONSOLE_ERROR("jewel is not enough");
		return 3;
	}
	player.cut_money(money_type, money_num, e_server_log_cut_money_upgrade_search_level, search_level);
	player.set_logic_data(e_role_logic_info_search_level, search_level);
	player.send_logic_one(e_role_logic_info_search_level);
	return 0;
}