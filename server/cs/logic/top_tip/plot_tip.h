#pragma once

#include "Logic/top_tip_def.hpp"

namespace hld
{
	class player;

	class plot_tip
	{
	public:
		explicit plot_tip();
		~plot_tip();

		void				clear_data();

	public:
		bool				init(const std::vector<int32>& plot_tips_arr, guid_64 map_obj_guid, float end_time = 0.0f);
		void				tick(int32 new_time);

		void				pop_destroy_plot_tip();

		static void			send_pop_plot_tip(player& player_ref, int32 str_id);

	private:
		void				on_pop_plot_tip(int32 str_id);

	private:
		int64				m_end_time;
		guid_64				m_map_obj_guid;
		s_plot_tip_entry	m_plot_tips[max_plot_tip_entry_num];
	};
}
