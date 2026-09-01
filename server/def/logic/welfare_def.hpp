/********************************************************************
created: 2016/10/8
file base: welfare_def
file ext: hpp
author: lxy
*********************************************************************/
#ifndef _WELFARE_DEF_HPP_
#define _WELFARE_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)

	const int32							daily_ttendance_begin_template_id = 107012001;										//每日签到奖励模板初始id
	const int32							cumulative_sign_in_reward_num		= 4;												//累计签到奖励数量		


	struct s_welfare_info
	{
		int32						data_ary[e_welfare_type_max];
		s_welfare_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_welfare_info_db
	{
		guid_64 role_guid;
		s_welfare_info data_info;
	};

	struct s_welfare_random_get_info
	{
		int32						data_ary[e_welfare_random_item_max];
		s_welfare_random_get_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_welfare_random_get_info_db
	{
		guid_64 role_guid;
		s_welfare_random_get_info data_info;
	};

	struct s_active_degree_info
	{
		int32						data_ary[e_daily_active_degree_type_max];
		s_active_degree_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_active_degree_info_db
	{
		guid_64 role_guid;
		s_active_degree_info data_info;
	};

#pragma pack(pop)
}

#endif
