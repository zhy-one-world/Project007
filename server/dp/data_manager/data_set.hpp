/********************************************************************
	created:	2018年1月4日20:46:28
	file base:	data_set
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_DATA_SET_HPP_
#define _DP_DATA_SET_HPP_

#include "data_info.hpp"

namespace faith
{
	class data_set
	{
	public:
		data_set();
		~data_set();
	public:
		void clear_data();
	public:
		void add_data_info(int64 data_type, const void* data_ptr, int32 data_size, int32 data_num = 1);
		bool get_data_info(int64 data_type, void* data_ptr, int32& data_num);
		bool get_proto_info(int64 data_type, void* data_ptr, int32& data_size);
		const int64& get_save_time_last() { return m_save_time_last; }
	private:
		std::unordered_map<int64, data_info> m_data_info_map;
		int64 m_save_time_last;
	};
}
#endif
