#pragma once
#include "logic/type_def.hpp"
#include "item_def.hpp"

namespace faith
{
	class player;

	struct s_offline_res
	{
		std::vector<s_item_template_info> item_list;
		std::vector<s_item_template_info> money_list;
	};

	class offline_award_system
	{
	public:
		static bool start_up(player* player_ptr);
		static void shut_down(player* player_ptr);
	public:
		static void player_online(player* player_ptr);
		static s_offline_res get_drop_item(player* player_ptr);
		static void gain_drop_item(player* player_ptr);
		static void send_drop_item(player* player_ptr);
	private:

	};
}