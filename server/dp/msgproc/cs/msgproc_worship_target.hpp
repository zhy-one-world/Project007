/********************************************************************
created:	2016/09/19
created:	19:9:2016   20:02
file base:	msgproc_worship_target
file ext:	cpp
author:		zhangminghai

purpose:
*********************************************************************/
#ifndef _DP_MSGPROC_WORSHIP_TARGET_HPP_
#define _DP_MSGPROC_WORSHIP_TARGET_HPP_

#include "db_manager.hpp"

namespace hld
{
	//½ÇÉ«³ç°Ý¼ÇÂ¼
	void cs2dp_req_load_char_worship_target(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_worship_target_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_worship_target(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_worship_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	//void cs2dp_req_save_role_ranking(uint32 connindex, const void* data_ptr, size_t data_len);
	//void cs2dp_req_save_role_ranking_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
}

#endif

