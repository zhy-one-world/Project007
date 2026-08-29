#pragma once
/********************************************************************
created:	2016Äê10ÔÂ24
file base:	msgproc_title
file ext:	hpp
author:		wucun

purpose:
*********************************************************************/
#ifndef _DP_MSGPROC_TITLE_HPP_
#define _DP_MSGPROC_TITLE_HPP_

#include "db_manager.hpp"

namespace faith
{
	//½ÇÉ«³ÆºÅ
	void cs2dp_req_load_char_title(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_title_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_title(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_title_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
