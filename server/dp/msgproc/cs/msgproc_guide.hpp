#ifndef _DP_MSGPROC_GUIDE_HPP_
#define _DP_MSGPROC_GUIDE_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	// 新手引导记录
	void cs2dp_req_load_char_guide_record(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_guide_record_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_guide_record(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
