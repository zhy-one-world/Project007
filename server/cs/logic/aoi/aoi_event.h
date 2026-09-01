/********************************************************************
created:	2016年1月4日17:26:07
file base:	aoi_event_h
file ext:	h
author:		zhy

purpose:
*********************************************************************/
#ifndef _AOI_EVENT_H_
#define _AOI_EVENT_H_
#include "logic/type_def.hpp"

namespace faith
{
	enum e_aoi_move_type;

	class aoi_event
	{
	public:
		static void add_object(const int32& unit_index);
		static void add_object(const int32& unit_index, e_aoi_move_type move_type);
		static void remove_object(const int32& unit_index);
		static void remove_object(const int32& unit_index, e_aoi_move_type move_type);
		static void update_object(const int32& unit_index);
		static void update_object(const int32& unit_index, e_aoi_move_type move_type);
	};
}
#endif