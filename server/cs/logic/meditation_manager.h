/********************************************************************
created: 2016Äê9ÔÂ20ÈÕ17:46:11
file base: meditation_manager
file ext: h
author: zhangminghai

purpose: about meditation
*********************************************************************/
#ifndef _MEDITATION_MANAGER_H_
#define _MEDITATION_MANAGER_H_

#include "Logic/char_def.hpp"
namespace faith
{
	class player;
	class meditation_manager
	{

	public:
		meditation_manager();
		~meditation_manager(void);


	public:
		void	heart_tick(const int64& new_time);
		void	clear_data();
		void	set_player_ptr(player* player_ptr);
		void	offline_meditation_settlement();

		void	start_mediation(bool is_player_active);
		void	stop_mediation();

		void	break_meditation();


		void    add_buff(const bool is_add, const int32 buff_id);
		xstring  lua_meditation_add_exp(int32 get_type, int32 gs_value, int32 BaseExp, float MeditationExpAddition, float MeditationExpRewardRadio, int32 meditation_standard_gs, int32 coefficient, int32 meditation_time);

		void    set_meditation_reward_time();

	private:
		int64           m_no_operate_time;
		player*			m_player_ptr;
		bool			m_is_in_meditation;
		int64			m_old_tick_time;
	};
}
#endif