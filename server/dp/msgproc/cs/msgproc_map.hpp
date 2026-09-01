/********************************************************************
created:	2016年7月20日
file base:	msgproc_map_record
file ext:	hpp
author:		JiKai
*********************************************************************/

#ifndef _DP_MSGPROC_MAP_RECORD_HPP_
#define _DP_MSGPROC_MAP_RECORD_HPP_

#include "db_manager.hpp"

namespace faith
{
	void cs2dp_req_load_char_map_record(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_map_record_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_map_record(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_map_record_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif