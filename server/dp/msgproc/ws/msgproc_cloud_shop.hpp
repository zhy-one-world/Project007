/********************************************************************
	file name:	msgproc_cloud_shop.hpp
	created:	2018/07/18 18:02
	author:		shangxiaoliang
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_CLOUD_SHOP_HPP_
#define _DP_MSGPROC_CLOUD_SHOP_HPP_

#include "db_manager.hpp"


namespace faith
{
	//云购
	void ws2dp_req_load_cloud_role_record(uint32 connindex, const void* data_ptr, size_t data_len);
	void req_load_cloud_shop_role_record(uint32 connindex, int32 start_pos);
	void ws2dp_req_load_cloud_role_record_end(db_result_type result, uint32 connindex, int32 start_pos);

	void ws2dp_req_save_cloud_role_record(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_save_cloud_role_record_end(db_result_type result, uint32 connindex);

	void ws2dp_req_clear_cloud_shop_player_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_clear_cloud_shop_player_info_end(db_result_type result, uint32 connindex);
}

#endif
