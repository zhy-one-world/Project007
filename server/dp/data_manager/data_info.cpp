/********************************************************************
	created:	2018Äê1ÔÂ4ÈÕ20:52:57
	file base:	data_info
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "data_info.hpp"

namespace faith
{
	data_info::data_info()
	{
		m_data_info = nullptr;
		clear_data();
	}

	data_info::~data_info()
	{
		clear_data();
	}
	void data_info::clear_data()
	{
		m_data_size = 0;
		m_data_num = 0;
		if (m_data_info)
		{
			delete[] m_data_info;
		}
	}
	void data_info::add_data_info(const void* data_ptr, int32 data_size, int32 data_num)
	{
		if (m_data_size < data_size)
		{
			if (m_data_info)
			{
				delete[] m_data_info;
			}
			m_data_info = new i8[data_size];
		}

		memcpy(m_data_info, data_ptr, data_size);
		m_data_size = data_size;
		m_data_num = data_num;
	}
	bool data_info::get_data_info(void* data_ptr, int32& data_num)
	{
		if (nullptr == data_ptr)
		{
			return false;
		}
		if (m_data_size > 0)
		{
			memcpy(data_ptr, m_data_info, m_data_size);
		}
		data_num = m_data_num;
		return true;
	}
	bool data_info::get_proto_info(void* data_ptr, int32& data_size)
	{
		if (nullptr == data_ptr)
		{
			return false;
		}
		if (m_data_size > 0)
		{
			memcpy(data_ptr, m_data_info, m_data_size);
		}
		data_size = m_data_size;
		return true;
	}
}