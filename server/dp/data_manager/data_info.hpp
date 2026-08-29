/********************************************************************
	created:	2018Äê1ÔÂ4ÈÕ20:52:57
	file base:	data_info
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_DATA_INFO_HPP_
#define _DP_DATA_INFO_HPP_

#include "logic/type_def.hpp"

namespace faith
{
	const int64 save_data_time = 300000;
	class data_info
	{
	public:
		data_info();
		~data_info();
	public:
		void clear_data();
	public:
		void add_data_info(const void* data_ptr, int32 data_size, int32 data_num);
		bool get_data_info(void* data_ptr, int32& data_num);
		bool get_proto_info(void* data_ptr, int32& data_size);
	private:
		i8* m_data_info;
		int32 m_data_size;
		int32 m_data_num;
	};
}

#endif
