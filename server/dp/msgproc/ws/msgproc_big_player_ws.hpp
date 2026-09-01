/********************************************************************
	created:	2016年12月12日13:04:37
	file base:	msgproc_big_player_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_BIG_PLAYER_WS_H_
#define _MSGPROC_BIG_PLAYER_WS_H_

#include "db_manager.hpp"


namespace faith
{
	void load_big_player(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_big_player_end(db_result_type result, uint32 connindex);
	void save_big_player_to_sql(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_big_player_detail(uint32 connindex, const void *data_ptr, size_t data_len);

	void save_big_player_to_sql_lua(uint32 connindex, int32 player_type[e_big_player_type_max], int64 role_guid[e_big_player_type_max]);
	void load_big_player_lua(uint32 connindex);
	void load_big_player_detail_lua(uint32 connindex, int32 player_type, int64 role_guid);
	
}

#endif