/********************************************************************
created:	2019年5月28日15:39:03
file base:	msgproc_cross_server_world_boss_ws
file ext:	hpp
author:		zhaoyuming

purpose:
*********************************************************************/

#ifndef _MSGPROC_CROSS_SERVER_WORLD_BOSS_WS_H_
#define _MSGPROC_CROSS_SERVER_WORLD_BOSS_WS_H_

#include "db_manager.hpp"

namespace faith
{
	void save_cross_server_world_boss_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_cross_server_world_boss_ws_info_end(db_result_type result, uint32 connindex);

	void load_cross_server_world_boss_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_cross_server_world_boss_ws_info_end(db_result_type result, uint32 connindex);














}
#endif //_MSGPROC_CROSS_SERVER_WORLD_BOSS_WS_H_