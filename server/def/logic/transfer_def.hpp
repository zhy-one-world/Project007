/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 15:52
  file base: scene_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _TRANSFER_DEF_HPP_
#define _TRANSFER_DEF_HPP_

#include "logic/type_def.hpp"
#include "logic/math.hpp"
#include <string>
#include "world_def.hpp"

namespace faith 
{
#pragma pack(push,1)

	static uint32	INVALID_KEY_VALUE = 0;

	enum ETransferStatus
	{
		TR_STATUS_CREATED,
		TR_STATUS_RUNNING,
		TR_STATUS_FINISHED,
		TR_STATUS_FAILED,
	};

	enum EFieldTypes
	{
		TR_FIELD_TYPE_UNDEFINED,
		TR_FIELD_TYPE_INT,
		TR_FIELD_TYPE_SHORT,
		TR_FIELD_TYPE_TINY,
		TR_FILED_TYPE_BIG,
		TR_FIELD_TYPE_FLOAT,
		TR_FIELD_TYPE_STRING,
		TR_FIELD_TYPE_TIME,
	};

	const int32 kmax_transfer_dynamic_int_num = 5;

	struct s_transfer_info
	{		
		s_map_pos		map_pos;
		e_scene_type	scene_type;
		uint32			map_id;
		int32			line_id;
		guid_64			map_guid;
		int32			war_index;
		int32			dynamic_ints[kmax_transfer_dynamic_int_num];
		s_transfer_info()
		{
			clear_data();
		}
		void clear_data()
		{
			map_pos.clear_data();
			scene_type = e_scene_type_low_world;
			map_id = 0;
			line_id = 0;
			map_guid.clear_data();
			war_index = 0;
			memset(dynamic_ints, 0, sizeof(dynamic_ints));
		}
	};
#pragma pack(pop)
}

#endif
