/********************************************************************
created:	2016年9月22日14:18:14
file base:	msgproc_special_name
file ext:	hpp
author:		zhangmnighai

purpose:
*********************************************************************/
#ifndef _DP_MSGPROC_CONVERT_HPP_
#define _DP_MSGPROC_CONVERT_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace faith
{
	//角色称号
	void cs2dp_req_load_char_convert(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_convert_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_convert(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_convert_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif