/********************************************************************
  created: 2019/06/17
  file base: feather_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FEATHER_DEF_HPP_
#define _FEATHER_DEF_HPP_

#include "logic/type_def.hpp"

namespace hld
{
#pragma pack(push,1)

	const int32	feather_heart_error_sucess = 0;
	const int32	feather_heart_error_unknow = -1;
	const int32 feather_heart_error_not_money = -2;
	const int32 feather_heart_error_max_upgrade = -3;
	const int32 feather_heart_error_equip_on_type = -4;
	const int32 feather_heart_error_equip_on_slot_type = -5;
	const int32 feather_slot_core =  e_item_info_succinct_level;
	const int32 feather_slot_start = e_item_info_jewel_slot_0;
	const int32 feather_slot_end = feather_slot_start + 5;
	const int32 feather_heart_slot_max = 4;

	enum e_feather_heart_oper
	{
		e_feather_heart_oper_grade,
		e_feather_heart_oper_equip_on,
		e_feather_heart_oper_piece,

	};
	enum e_feather_heart
	{
		e_feather_heart_0,
		e_feather_heart_1,
		e_feather_heart_2,
		e_feather_heart_3,
		e_feather_heart_max
	};
	enum e_feather_have_skill
	{
		e_feather_have_skill_0,
		e_feather_have_skill_1,
		e_feather_have_skill_2,
		e_feather_have_skill_max
	};
	enum e_feather_core_add_level
	{
		e_feather_core_add_level_start = e_item_info_forge_property1,
		e_feather_core_add_level_0 = e_feather_core_add_level_start,
		e_feather_core_add_level_1, 
		e_feather_core_add_level_2, 
	    e_feather_core_add_level_end,
	};
#pragma pack(pop)
}

#endif
