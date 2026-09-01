/********************************************************************
  created: 2019年2月13日
  file base: seven_day_goal_def
  file ext: hpp
  author: wangdi
  purpose: 七日目标定义文件
*********************************************************************/
#ifndef _SEVEN_DAY_GOAL_DEF_HPP_
#define _SEVEN_DAY_GOAL_DEF_HPP_
#include "logic/type_def.hpp"
#include "game_enum_def_s.h"
namespace faith
{
#pragma pack(push,1)

	const int32 max_seven_day_goal_num = 100;

	const int32 max_schedule_value = 100;

	const int32 max_reward_days = 7;

	struct s_seven_day_goal_info
	{
		int32 data_ary[e_seven_day_goal_info_max];
		s_seven_day_goal_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	
	struct s_seven_day_goal_info_db
	{
		guid_64					role_guid;
		s_seven_day_goal_info	data_info;
	};
#pragma pack(pop)
}

#endif
