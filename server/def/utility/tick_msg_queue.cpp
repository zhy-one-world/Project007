/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:25
	file base:	tick_msg_queue
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "tick_msg_queue.hpp"

namespace faith
{

	boost::pool<> tick_msg_queue::m_msg_pool(MAX_MSG_SIZE);

	tick_msg_queue::tick_msg_queue()
	{
	}

	tick_msg_queue::~tick_msg_queue()
	{
		while (!m_tick_msg_queue.empty())
		{
			msg_data_type msg_data;
			msg_data = m_tick_msg_queue.front();
			m_tick_msg_queue.pop();
			m_msg_pool.free(msg_data.msg_ptr);
		}
	}

	void tick_msg_queue::set_sender(sender_handler_type sender)
	{
		m_sender = sender;
	}

	void tick_msg_queue::update(uint32 elapse_time)
	{
		if (!m_tick_msg_queue.empty())
		{
			msg_data_type msg_data;
			msg_data = m_tick_msg_queue.front();

			if (0 != m_sender(msg_data.msg_ptr, msg_data.size))
			{
				m_tick_msg_queue.pop();
				m_msg_pool.free(msg_data.msg_ptr);
			} // else ÖØÐÂ·¢ËÍ
		}
	}

	bool tick_msg_queue::push(void* msg_ptr, size_t size)
	{
		if (size > MAX_MSG_SIZE)
		{
			return false;
		}

		void* new_msg_ptr = m_msg_pool.malloc();

		if (NULL == new_msg_ptr)
		{
			return false;
		}

		memcpy(new_msg_ptr, msg_ptr, size);
		msg_data_type msg_data;
		msg_data.msg_ptr	= new_msg_ptr;
		msg_data.size		= size;
		m_tick_msg_queue.push(msg_data);
		return true;
	}

}
