/********************************************************************
created: 2018年5月5日
file base: msgproc_gain_treasure
file ext: hpp
author: wangdi

purpose: 打宝相关
*********************************************************************/

#ifndef _DP_MSGPROC_MSGPROC_GAIN_HPP_
#define _DP_MSGPROC_MSGPROC_GAIN_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace faith
{
	void cs2dp_req_load_gain_treasure_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_gain_treasure_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_gain_treasure_info(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_gain_treasure_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
