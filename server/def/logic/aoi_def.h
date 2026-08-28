#pragma once
/********************************************************************
created: 2016Äê8ÔÂ11ÈÕ13:20:37
file base: aoi_def
file ext: hpp
author: zhy

purpose:
*********************************************************************/
#ifndef _AOI_DEF_HPP_
#define _AOI_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/char_def.hpp"

namespace hld
{
#pragma pack(push,1)
	const int32 AOI_SEE_TOWER_NUM = 10;
	const int32 AOI_MAIN_TOWER_NUM = 1000;
	struct s_tower_pos
	{
		int32 x;
		int32 y;
		s_tower_pos()
		{
			x = 0;
			y = 0;
		}
		void clear_data()
		{
			x = 0;
			y = 0;
		}
	};
	struct s_tower_info
	{
		int32			unit_index;
		e_unit_type unit_type;
		s_tower_info()
		{
			unit_index = 0;
			unit_type = e_unit_type_null;
		}
	};
	enum e_aoi_move_type
	{
		e_aoi_move_type_add,
		e_aoi_move_type_remove,
		e_aoi_move_type_update
	};
#pragma pack(pop)
}
#endif
