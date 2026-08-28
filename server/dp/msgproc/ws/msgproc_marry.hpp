/********************************************************************
	file name:	msgproc_marry.hpp
	created:	2017/12/07 23:32
	author:		Allen_Lei
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_MARRY_HPP_
#define _DP_MSGPROC_MARRY_HPP_

#include "db_manager.hpp"


namespace hld
{
	//½ÇÉ«¹ØÏµ
	void req_load_marry_info(uint32 connindex, int32 start_pos);
	void ws2dp_req_load_marry_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_load_marry_info_end(db_result_type result, uint32 connindex, int32 start_pos);
	void ws2dp_req_save_marry_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_save_marry_info_end(db_result_type result, uint32 connindex);   
	void ws2dp_req_load_couple_login_time(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_load_couple_login_time_end(db_result_type result, uint32 connindex, const guid_64& sender_guid  );
	void ws2dp_load_marry_info_cross_server_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_load_marry_info_cross_server_proc_end(db_result_type result, uint32 connindex, const guid_64& sender_guid);
}

#endif
