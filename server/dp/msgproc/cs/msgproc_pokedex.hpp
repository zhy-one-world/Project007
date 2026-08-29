/********************************************************************
created:	2016年8月16日11:05:31
file base:	msgproc_pokedex
file ext:	hpp
author:		XingLong

purpose:
*********************************************************************/
#ifndef _DP_MSGPROC_POKEDEX_HPP_
#define _DP_MSGPROC_POKEDEX_HPP_

#include "db_manager.hpp"

namespace faith
{
	//角色技能
	void cs2dp_req_load_char_pokedex(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_pokedex_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_pokedex(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_pokedex_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);

	void cs2dp_req_load_char_tinder(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_tinder_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_tinder(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_tinder_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
