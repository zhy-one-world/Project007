/********************************************************************
	created:	2016年10月09
	file base:	msgproc_welfare
	file ext:	hpp
	author:		lxy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_WELFARE_HPP_
#define _DP_MSGPROC_WELFARE_HPP_

#include "db_manager.hpp"

namespace hld
{
	//角色福利
	void cs2dp_req_load_char_welfare(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_welfare_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_welfare(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_welfare_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);

	void cs2dp_req_load_char_active_degree(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_active_degree_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_active_degree(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_active_degree_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);

	void cs2dp_req_load_char_welfare_random_get(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_welfare_random_get_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_welfare_random_get(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
