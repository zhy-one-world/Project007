/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:14
	file base:	recvbuffer
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _RECV_BUFFER_H_
#define _RECV_BUFFER_H_

#include "logic/type_def.hpp"

#define	INIT_BUFF_SIZE 4194304
#define	INC_BUFF_SIZE  13072

namespace faith
{
	//自增长的循环存取连续块缓冲区
	class recvbuffer
	{
	public:
		recvbuffer(	);
		~recvbuffer(	);
	public:
		bool					write( const void * data, const int32 len );
		void					read( void * data, const int32 len );
		xchar*					rptr( const int32 len );
		void					rnext( const int32 len );
		bool					reserve( const int32 len );				//保留len长度的buff用于写.
		void					commit(	);								//提交保留的buff
		void					rollback(	);
		xchar*					wptr(	);								//获取写指针,必须和reserve(), commit(),一起使用
		boost::recursive_mutex& get_mutex(	)	{ return m_mutex; };	//对于调用rptr(), 返回的读指针，可能会在自增长过程中改变，导致rptr()返回值指向错误的地址，调rptr()前要进行锁操作

	private:
		int32						get_used_size(	);
		int32						get_idle_size(	);
		bool					writeable( const int32 len, int32& write_pos );
		bool					readable( const int32 len, int32& read_pos );
	private:
		xchar						m_buffer[INIT_BUFF_SIZE];
		int32						m_buff_size;
		int32						m_reserve_size;
		int32						m_rpos;
		int32						m_wpos;
		int32						m_old_wpos;
		boost::recursive_mutex	m_mutex;
	};
}

#endif
