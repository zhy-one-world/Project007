/********************************************************************
	created:	2019Äê1ÔÂ17ÈÕ12:59:26
	file base:	msgproc_cross_server_pk_ws
	file ext:	hpp
	author:		zhangshuo
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_CROSS_SERVER_PK_WS_H_
#define _MSGPROC_CROSS_SERVER_PK_WS_H_

#include "db_manager.hpp"
#include <Logic/cross_server_pk_def.hpp>
namespace faith
{
	void save_cross_server_pk_ws_info_lua(uint32 connindex, int32 pk_state, int32 war_index, int64 player_guid[max_island_player_num],
		xstring role_name[max_island_player_num], int32 is_winner[max_island_player_num], int32 server_id[max_island_player_num], int32 win_num[max_island_player_num]);
	void save_cross_server_pk_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_cross_server_pk_ws_info_end(db_result_type result, uint32 connindex);

	void clear_cross_server_pk_ws_info_lua(uint32 connindex);
	void clear_cross_server_pk_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_cross_server_pk_ws_info_end(db_result_type result, uint32 connindex);

	void load_cross_server_pk_ws_info_lua(uint32 connindex);
	void load_cross_server_pk_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_cross_server_pk_ws_info_end(db_result_type result, uint32 connindex);
}

#endif