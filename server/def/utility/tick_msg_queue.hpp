/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:24
	file base:	tick_msg_queue
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef TICK_MSG_QUEUE_HPP
#define TICK_MSG_QUEUE_HPP

#include <queue>
#include "logic/type_def.hpp"

namespace hld
{
	const int32 MAX_MSG_SIZE = 1024 * 8;

	class tick_msg_queue
	{
		typedef boost::function<int32(void*, size_t)> sender_handler_type;
		struct msg_data_type
		{
			void*	msg_ptr;
			size_t	size;
		};

	public:
		tick_msg_queue();
		~tick_msg_queue();

	public:
		void set_sender(sender_handler_type sender);	
		void update(uint32 elapse_time);
		bool push(void* msg_ptr, size_t size);

	private:
		static boost::pool<>		m_msg_pool;			// 设为static主要是每个player都共用一个pool
		std::queue<msg_data_type>	m_tick_msg_queue;
		sender_handler_type			m_sender;
	};
}


#endif // TICK_MSG_QUEUE_HPP
