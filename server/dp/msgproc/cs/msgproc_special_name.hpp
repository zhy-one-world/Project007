/********************************************************************
	created:	2015年7月24日14:18:14
	file base:	msgproc_special_name
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_SPECIAL_NAME_HPP_
#define _DP_MSGPROC_SPECIAL_NAME_HPP_

#include "db_manager.hpp"

namespace faith
{
	//角色称号
	void cs2dp_req_load_char_special_name(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_special_name_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_special_name(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_special_name_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
