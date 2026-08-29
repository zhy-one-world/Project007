/********************************************************************
	file name:	msgproc_relation.hpp
	created:	2018/09/15 23:32
	author:		dengronggen
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_CHAT_WS_HPP_
#define _DP_MSGPROC_CHAT_WS_HPP_

#include "db_manager.hpp"

namespace faith
{
	void save_chat_with_offline(uint32 connindex, const void* data_ptr, size_t data_len);
	void load_chat_with_offline(uint32 connindex, const void* data_ptr, size_t data_len);
	void load_chat_with_offline_end(db_result_type result,uint32 connindex, const guid_64& role_guid);
	void eliminate_chat_with_offline(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
