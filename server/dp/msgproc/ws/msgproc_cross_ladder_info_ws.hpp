/********************************************************************
created: 2019年5月31日
file base: msgproc_cross_ladder_info_ws
file ext: h
author: pengbiao

purpose: 跨服天梯
*********************************************************************/

#ifndef _DP_MSGPROC_CROSS_LADDER_INFO_WS_HPP_
#define _DP_MSGPROC_CROSS_LADDER_INFO_WS_HPP_

#include "db_manager.hpp"

namespace faith
{
	void ws2dp_cross_ladder_load_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_load_info_proc_end(db_result_type result, uint32 connindex, int32 cur_num);

	void ws2dp_cross_ladder_add_role_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_update_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_update_ticket_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_update_role_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_update_refresh_ticket_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_update_refresh_season_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2dp_cross_ladder_common_end(db_result_type result, uint32 connindex);
}

#endif