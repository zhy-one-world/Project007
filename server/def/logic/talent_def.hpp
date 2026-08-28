/********************************************************************
created: 2017-5-26 16:31:46
file base: talent_def
file ext: hpp
author: zhy
*********************************************************************/
#ifndef _TALENT_DEF_HPP_
#define _TALENT_DEF_HPP_

#include "logic/type_def.hpp"

namespace hld
{
#pragma pack(push,1)
	const int32 TALENT_NUM_MAX = 200;//总共多少天赋
	const int32 TALENT_ID_LOW = 100;//需求的id < 100 层级 >100 talent_list_template_id
	const int32 talent_kinds_num = 5;
	const int32 talent_layers_num = 10;
	const int32 role_talent_list_base_id = 25000001;
	const int32 role_talent_list_add_num = 10000;
	enum e_talent_info_global
	{
		e_talent_info_global_prekind,
		e_talent_info_global_prelayer,
		e_talent_info_global_need_money,
		e_talent_info_global_cur_money,
		e_talent_info_global_activate,
		e_talent_info_global_max
	};

	enum e_talent_buff_add_type
	{
		e_talent_buff_add_type_none,
		e_talent_buff_add_type_accumulate
	};

	enum e_open_layer_limit
	{
		e_open_layer_limit_type,
		e_open_layer_limit_num,
		e_open_layer_limit_max
	};

	struct s_talent_info_global
	{
		int32					data_ary[e_talent_info_global_max];
		s_talent_info_global()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};

	//define the inst data struct
	struct s_talent_info
	{
		int32						data_ary[e_talent_info_max];				//天赋实例数据
		s_talent_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_talent_info_db
	{
		guid_64 role_guid;
		s_talent_info data_info;
	};
#pragma pack(pop)
}

#endif
