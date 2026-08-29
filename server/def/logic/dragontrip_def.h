#ifndef DRAGONTRIP_DEF_H
#define DRAGONTRIP_DEF_H
#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace faith
{
#pragma pack(push, 1)

	const int32  dragontrip_money_max = 10;

	const int32  dragontrip_error_sucess = 0;
	const int32  dragontrip_error_times = -1;
	const int32  dragontrip_error_state = -2;
	const int32  dragontrip_error_not_money = -3;
	const int32  dragontrip_error_not_item = -4;
	const int32  dragontrip_error_reward_round = -5;
	const int32  dragontrip_error_have_get = -6;
	const int32  dragontrip_error_not_item2 = -7;
	const int32  dragontrip_five_num = 5;

	enum e_dragontrip_inst_data
	{
		e_dragontrip_free_time,
		e_dragontrip_round,
		e_dragontrip_pos,
		e_dragontrip_rewardflag,
		e_dragontrip_oper_time,
		e_dragontrip_active_state,
		e_dragontrip_day_times,
		e_dragontrip_max,
	};

	enum e_dragontrip_reward
	{
		e_dragontrip_reward_r1 = 1,
		e_dragontrip_reward_r2  =2,
		e_dragontrip_reward_r3 = 3,
		e_dragontrip_reward_r4 = 4,
		e_dragontrip_reward_r5 = 5,
		e_dragontrip_reward_rall  = 6,
	};

	enum e_dragontrip_oper
	{
		e_dragontrip_oper_money = 1,
		e_dragontrip_oper_item = 2,
		e_dragontrip_oper_roll = 3,
		e_dragontrip_oper_reward_round = 4,
		e_dragontrip_oper_reward_all = 5,
		e_dragontrip_oper_refresh_recycle = 6,
		e_dragontrip_oper_refresh_day_times = 7,
		e_dragontrip_oper_quick = 8,
	};

	enum e_dragontrip_type
	{
		e_dragontrip_type_point  = 1,
		e_dragontrip_type_reward_round = 2,
		e_dragontrip_type_reward_all = 3,
	};

	enum e_dragontrip_point
	{
		e_dragontrip_point_start = 0,

		e_dragontrip_point_transfer1 = 4,
		e_dragontrip_point_transfer2 = 17,

	};

	enum e_dragontrip_param
	{
		e_dragontrip_param_money_type,
		e_dragontrip_param_money_num,
		e_dragontrip_param_cd_second,
		e_dragontrip_param_day_limit,

		e_dragontrip_param_item_type,
		e_dragontrip_param_item_num,

		e_dragontrip_param_point_max,

		e_dragontrip_param_reward_max,

		e_dragontrip_param_max,
	};


	struct s_dragontrip_info
	{
		int32 data_ary[e_dragontrip_max];

		s_dragontrip_info()
		{
			reset();
		}
		void reset()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool operator==(const s_dragontrip_info &rhs)
		{
			for (int32 i = 0; i < e_dragontrip_max; i++)
			{
				if (rhs.data_ary[i] != data_ary[i])
				{
					return false;
				}
			}
			return true;
		}

	};

	struct s_dragontrip_info_db
	{
		guid_64 role_guid;
		s_dragontrip_info data_info;

	};









#pragma pack(pop)
}


#endif