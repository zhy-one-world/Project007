/********************************************************************
	created:	2016Äê12ÔÂ12ÈÕ12:46:40
	file base:	msgproc_role_pk_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_ROLE_PK_WS_H_
#define _MSGPROC_ROLE_PK_WS_H_

#include "db_manager.hpp"
#include "utility/parse_msg.h"

namespace hld
{
	void load_role_pk_top(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_role_pk_top_end(db_result_type result, uint32 connindex);

	void load_role_pk_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_role_pk_info_end(db_result_type result, uint32 connindex);

	void save_role_pk_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_role_pk_info_end(db_result_type result, uint32 connindex);

	void save_role_pk_honor_refresh(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_role_pk_honor_refresh_end(db_result_type result, uint32 connindex);

	void load_role_pk_log(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_role_pk_log_end(db_result_type result, uint32 connindex, guid_64 role_guid, int32 server_id);

	void load_role_pk_top_lua(uint32 connindex);
	void load_role_pk_info_lua(uint32 connindex, int64 role_guid, int32 tempalte_id, int32 role_gs, xstring role_name);
	void save_role_pk_info_lua(uint32 connindex, int64 role_guid, xstring target_name1, xstring target_name2, xstring target_name3, int32 laddet_score, int32 win_result, int32 cross_honor);
	void save_role_pk_honor_refresh_lua(uint32 connindex, int64 role_guid, int32 refresh_time);
	void load_role_pk_log_lua(uint32 connindex, int64 role_guid, int32 server_id);
}

#endif