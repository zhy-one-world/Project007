/********************************************************************
created:	2016年1月4日17:26:07
file base:	aoi_tower_h
file ext:	h
author:		zhy

purpose:
*********************************************************************/
#ifndef _AOI_TOWER_H_
#define _AOI_TOWER_H_
#include "logic/type_def.hpp"

namespace faith
{
	class aoi_tower
	{
	public:
		aoi_tower();
		~aoi_tower();
	public:
		void clear_data();
	public:
		void add_unit(const int32& unit_index);
		void remove_unit(const int32& unit_index);
		void get_unit_all(int32* unit_ary, int32& watch_array_num);
		bool get_unit_index(const int32& unit_index);
	public:
		void add_watch(const int32& unit_index);
		void remove_watch(const int32& unit_index);
		const unit_index_map& get_watch_all()const;
	private:
		unit_index_map				m_unit_index_list;
		unit_index_map				m_watch_index_list;
	};
}
#endif

