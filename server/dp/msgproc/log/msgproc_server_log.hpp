/*
author: allen wl
time: 2017 11 30
*/

#ifndef _MSGPROC_SERVER_LOG_H_
#define _MSGPROC_SERVER_LOG_H_

#include "db_manager.hpp"
#include <server_log_msg.hpp>

namespace hld
{
	void dp_save_log_info(xstring& log_string, xstring& table_name);
	void dp_save_log_info_end(db_result_type result);
	void server2dp_req_save_log_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void server2dp_req_save_log_info_end(db_result_type result, uint32 connindex);

	void server2dp_req_save_chat_log_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void server2dp_req_save_chat_log_info_end(db_result_type result, uint32 connindex);

	void server2dp_req_save_legion_log(uint32 connindex, const void* data_ptr, size_t data_len);
	void server2dp_req_save_legion_log_end(db_result_type result, uint32 connindex);
}

#endif
