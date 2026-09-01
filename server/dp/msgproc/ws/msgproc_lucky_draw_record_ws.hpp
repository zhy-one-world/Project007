/********************************************************************
	created:	2016年12月12日13:04:37
	file base:	msgproc_big_player_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_LUCKY_DRAW_RECORD_WS_H_
#define _MSGPROC_LUCKY_DRAW_RECORD_WS_H_

#include "db_manager.hpp"
#include "utility/parse_msg.h"

namespace faith
{
	void del_draw_info_from_db(uint32 connindex, const void *data_ptr, size_t data_len);
	void add_draw_info_to_db(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_draw_info_by_db(uint32 connindex, const void *data_ptr, size_t data_len);

	void load_draw_rocord_info_end(db_result_type result, uint32 connindex);

	void del_draw_info_from_db_lua(uint32 connindex, int64 info_guid);
	void add_draw_info_to_db_lua(uint32 connindex, int64 info_guid, int64 role_guid, xstring role_name, int32 item_template_id, int32 get_type, int32 record_type, int64 record_time);
	void load_draw_info_by_db_lua(uint32 connindex);
}

#endif