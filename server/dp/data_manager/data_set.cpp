/********************************************************************
	created:	2018年1月4日20:46:55
	file base:	data_set
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "data_set.hpp"
#include "time.hpp"
#include "char_def.hpp"

namespace faith
{
	data_set::data_set()
	{
		clear_data();
	}

	data_set::~data_set()
	{
	}
	void data_set::clear_data()
	{
		m_data_info_map.clear();
		m_save_time_last = 0;
	}
	void data_set::add_data_info(int64 data_type, const void* data_ptr, int32 data_size, int32 data_num)
	{
		m_data_info_map[data_type].add_data_info(data_ptr, data_size, data_num);
		m_save_time_last = utility::get_tick_count();
	}
	bool data_set::get_data_info(int64 data_type, void* data_ptr, int32& data_num)
	{
		auto it = m_data_info_map.find(data_type);
		if (it != m_data_info_map.end())
		{
			it->second.get_data_info(data_ptr, data_num);
			//m_data_info_map.erase(it);
			return true;
		}
		return false;
	}
	bool data_set::get_proto_info(int64 data_type, void* data_ptr, int32& data_size)
	{
		auto it = m_data_info_map.find(data_type);
		if (it != m_data_info_map.end())
		{
			return it->second.get_proto_info(data_ptr, data_size);
		}
		return false;
	}
}