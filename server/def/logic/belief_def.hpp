/********************************************************************
created: 2016/04/21
created: 21:4:2016 15:40
file base: belief_def
file ext: hpp
author: W.H.B.A.(王黄博昂)

purpose:define the type of belief, define the type and the length of belief inst data
*********************************************************************/
#ifndef _BELIEF_DEF_HPP_
#define _BELIEF_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "game_enum_def_s.h"
#include "template/template_base.h"

namespace hld
{
#pragma pack(push,1)

	const	float	belief_buff_change_time_diff = 3.0;	//信仰Buff切换时间

	//define the type of belief
	//enum EBeliefType
	//{
	//	e_belief_type_spring,							// 火
	//	e_belief_type_summer,							// 冰
	//	e_belief_type_autumn,							// 光
	//	e_belief_type_winter,							// 暗
	//	e_belief_type_max
	//};

	//enum EBeliefBuffIndex
	//{
	//	e_belief_buff_index_0,							// 一阶段
	//	e_belief_buff_index_1,							// 二阶段
	//	e_belief_buff_index_2,							// 三阶段
	//	e_belief_buff_index_max
	//};

	const	int32	max_belief_buff_num = e_belief_series_type_max * e_belief_buff_level_max;		//信仰buff库，每个信仰三个buff

	const	int32	belief_buff_req_array[e_belief_buff_level_max] = {1, 4, 7};

	//define the inst data struct
	struct s_belief_info
	{
		int32						data_ary[e_belief_info_max];				// 女神实例数据
		s_belief_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool to_proto(hld::st_proto::st_belief_info* st_belief_ptr)
		{
			if (st_belief_ptr == nullptr)
			{
				return false;
			}
			for (int32 i = 0; i < e_belief_info_max; i++)
			{
				st_belief_ptr->add_data_ary(data_ary[i]);
			}
			return true;
		}
		void from_proto(const hld::st_proto::st_belief_info& st_belief_ref)
		{
			for (int32 i = 0; i < e_belief_info_max && i < st_belief_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_belief_ref.data_ary(i);
			}
		}
	};
	struct s_belief_info_db
	{
		guid_64 role_guid;
		s_belief_info data_info;
		s_belief_info_db() {};
		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};
#pragma pack(pop)
}

#endif
