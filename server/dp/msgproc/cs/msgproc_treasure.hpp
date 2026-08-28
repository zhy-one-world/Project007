/********************************************************************
	created:	2017-5-26 20:35:41
	file base:	msgproc_talent
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_TREASURE_HPP_
#define _DP_MSGPROC_TREASURE_HPP_

#include "db_manager.hpp"

namespace hld
{
	//½ÇÉ«¼¼ÄÜ
	void cs2dp_req_load_char_treasure(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_treasure_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_treasure(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
