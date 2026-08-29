/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:18
	file base:	recvbuffer
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "db_config.hpp"
#include "recvbuffer.hpp"
#include <string>
#include <assert.h>
#include <exception>

namespace faith
{
	int32 recvbuffer::get_used_size()
	{
		//m_wpos == m_rpos，即buffer为空
		return (m_wpos - m_rpos + m_buff_size) % m_buff_size;
	}

	int32 recvbuffer::get_idle_size()
	{
		//留一个byte作为buffer已满的标志
		return m_buff_size - get_used_size() - 1;
	}

	bool recvbuffer::writeable(const int32 len, int32& real_wpos)
	{
		int32 real_idle_size = get_idle_size();
		real_wpos = m_wpos;

		//如果buff未端不够写，则从头开始写。
		int32 count = m_buff_size - m_wpos;
		if (m_wpos >= m_rpos && count < len)
		{
			real_idle_size -= count;
			real_wpos = 0;
		}
		return real_idle_size >= len;
	}

	bool recvbuffer::readable(const int32 len, int32& real_rpos)
	{
		int32 real_used_size = get_used_size();
		real_rpos = m_rpos;

		//如果buff末端不够读，则从头开始读
		int32 count = m_buff_size - m_rpos;
		if(m_rpos >= m_wpos && count < len)
		{
			real_used_size -= count;
			real_rpos = 0;
		}
		return real_used_size >= len;
	}

	recvbuffer::recvbuffer()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
		m_buff_size = INIT_BUFF_SIZE;
		m_rpos = m_wpos = m_old_wpos = 0;
		m_reserve_size = 0;
	}

	recvbuffer::~recvbuffer()
	{
		/*if(m_buffer)
		{
			delete [] m_buffer;
		}*/
	}

	bool recvbuffer::write(const void* data,const int32 len)
	{
		assert(m_buffer);

		boost::recursive_mutex::scoped_lock lock(m_mutex);

		if(data != NULL && len > 0)
		{
			if(m_reserve_size == 0)
			{
				bool is_ok = reserve(len);
				if(!is_ok)
				{
					return false;
				}
			}

			assert(len == m_reserve_size);

			//将数据写入缓存区
			memcpy(m_buffer + m_wpos, data, len);
			commit();
		}

		return true;
	}

	void recvbuffer::read(void* data, const int32 len)
	{
		assert(m_buffer);

		boost::recursive_mutex::scoped_lock lock(m_mutex);

		if(data != NULL && len > 0)
		{
			int32 real_rpos;
			if(readable(len,real_rpos))
			{
				memcpy(data, m_buffer + real_rpos, len);
				return;
			}

			throw std::exception();
		}
		return;
	}

	xchar* recvbuffer::rptr(const int32 len)
	{
		assert(m_buffer);

		if(len > 0)
		{
			int32 real_rpos;
			if(readable(len,real_rpos))
			{
				return m_buffer + real_rpos;
			}

			throw std::exception();
		}
		return m_buffer;
	}

	void recvbuffer::rnext(const int32 len)
	{
		assert(m_buffer);

		boost::recursive_mutex::scoped_lock lock(m_mutex);

		if(len > 0)
		{
			int32 real_rpos;
			if(readable(len,real_rpos))
			{
				m_rpos = (real_rpos + len) % m_buff_size;
				return;
			}

			throw std::exception();
		}
		return;
	}

	bool recvbuffer::reserve(const int32 len)
	{
		assert(m_buffer);

		boost::recursive_mutex::scoped_lock lock(m_mutex);

		if(len > 0)
		{
			//不够写，则扩大缓存区
			int32 real_wpos;
			if(!writeable(len,real_wpos))
			{
				std::cout<<_XTEXT("recvbuffer::reserve  wirteable is full len")<< len<<std::endl;
			}
			else
			{
				m_wpos = real_wpos;
			}
			m_reserve_size = len;
		}
		return true;
	}

	void recvbuffer::commit()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		if(m_reserve_size > 0)
		{
			m_wpos = (m_wpos + m_reserve_size) % m_buff_size;
			m_old_wpos = m_wpos;
			m_reserve_size = 0;
		}
	}

	void recvbuffer::rollback()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		if(m_reserve_size > 0)
		{
			m_wpos = m_old_wpos;
			m_reserve_size = 0;
		}
	}

	xchar* recvbuffer::wptr()
	{
		assert(m_buffer);
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return m_buffer + m_wpos;
	}
}

