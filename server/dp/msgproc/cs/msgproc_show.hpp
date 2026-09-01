/********************************************************************
	created:	2016年9月8日11:18:14
	file base:	msgproc_show
	file ext:	hpp
	author:		lxy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_SHOW_HPP_
#define _DP_MSGPROC_SHOW_HPP_

#include "db_manager.hpp"

namespace faith
{
	//角色显示（坐骑翅膀快捷召唤啥的）
	void cs2dp_req_load_char_show(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_show_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_show(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
