/********************************************************************
created: 2016年5月14日
file base: achievement
author: 张宇翔
purpose: 单个任务
*********************************************************************/
#ifndef _ACHIEVEMENT_HPP_
#define _ACHIEVEMENT_HPP_

#include "Logic/achievement_def.h"

namespace hld
{
	class player;
	class unit;
	struct AchievementTemplate;
	struct s_item_template_info;

	class cachievement
	{
	public:
		cachievement();
		~cachievement();
	public:
		void tick(float elapse_time);
		void init_achievement_mgr(player* player_ptr);
		void clear_data();

		bool init_achievement_by_template(int32 achievement_id, player* player_ptr);
		bool init_achievement_by_info(s_achievement_info achievement_info, player* player_ptr);
		int32 finish_achievement(std::vector<s_item_template_info>& get_money_tuple_array);//完成发钱


		AchievementTemplate* get_achievement_template_ptr() { return m_achievement_ptr; }
		int32 get_achievement_type();

		int32 get_achievement_state() { return m_achievement_info.data_ary[eaid_current_state]; }
		bool set_achievement_state(e_achievement_state achievement_state);

		bool inc_count();
		bool replace_count(int32 new_value);

		s_achievement_info& get_achievement_info_all() { return m_achievement_info; }

		void set_info(s_achievement_info new_info) { m_achievement_info = new_info; }
		int32 get_inst_data(int32 index) const;
		bool set_inst_data(int32 index, int32 num);
		bool check_achievement_state();// 判断目标达成可以完成

	private:
		AchievementTemplate* m_achievement_ptr;
		player* m_player_ptr;
		s_achievement_info    m_achievement_info;

	};
}

#endif