/********************************************************************
	created:	2016Äê12ÔÂ12ÈÕ12:59:26
	file base:	msgproc_best_record_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_BEST_RECORD_WS_H_
#define _MSGPROC_BEST_RECORD_WS_H_

#include "db_manager.hpp"
#include <Logic/best_record_def.hpp>

namespace hld
{
	void save_best_record(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_best_record_end(db_result_type result);
	void load_all_best_record(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_best_record_end(db_result_type result, uint32 connindex);

	void save_best_record_lua(uint32 connindex, uint32 map_template_id, int64 role_guid[hld::single_map_record_num], xstring role_name[hld::single_map_record_num], int32 role_value[hld::single_map_record_num]);
	void load_all_best_record_lua(uint32 connindex);
}

#endif