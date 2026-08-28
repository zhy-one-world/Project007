/********************************************************************
created: 2016/07/15
created: 21:4:2016 15:40
file base: special_name_def
file ext: hpp
author: zhangminghai

purpose:define the type of sepcial_name, define the type and the length of sepcial_name inst data
*********************************************************************/
#ifndef _SPECIAL_NAME_DEF_HPP_
#define _SPECIAL_NAME_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace hld
{
#pragma pack(push,1)
	enum e_special_name_type
	{
		e_special_name_type_acheivement = 0,	//	成就称号
		e_special_name_type_acheivement_star,
		e_special_name_type_military_rank,		//	军衔称号
		e_special_name_type_military_rank_star,
		e_special_name_type_max,				//称号的最大值				
	};

	enum e_special_name_operate_type
	{
		e_special_name_operate_upgrade,		//	升级
		e_special_name_operate_promote,		//	升星 
	};
	enum e_special_name_upgrade_end_state
	{
		e_special_name_upgrade_succeed,		//成功
		e_special_name_upgrade_fail,		//失败
	};
	struct s_special_name_info
	{
		int32						data_ary[e_special_name_type_max];				// 称号实例数据
		s_special_name_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_special_name_info_db
	{
		guid_64 role_guid;
		s_special_name_info data_info;
		s_special_name_info_db()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};

	const int32 max_num_of_star = 5;
	const int32 military_rank_start_id = 59010001;
	const int32 acheivement_start_id = 59000001;

#pragma pack(pop)
}
#endif