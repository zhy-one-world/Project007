/********************************************************************
  created: 2017年8月10日11:27:00
  file base: service_goal_def
  file ext: hpp
  author: 韩泽成
  purpose: 七日目标定义文件
*********************************************************************/
#ifndef _SERVICE_GOAL_DEF_HPP_
#define _SERVICE_GOAL_DEF_HPP_
#include "logic/type_def.hpp"
#include "game_enum_def_s.h"
namespace faith
{
#pragma pack(push,1)

	const int32 MAX_SERVICE_GOAL_NUM = 100;

	const int32 MAX_SCHEDULE_VALUE = 100;

	const int32 MAX_REWARD_DAYS = 7;

	struct s_service_goal_info
	{
		int32 data_ary[e_service_goal_inst_data_max];
		s_service_goal_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	
	struct s_service_goal_info_db
	{
		guid_64				role_guid;
		s_service_goal_info data_info;
	};
#pragma pack(pop)
}

#endif
