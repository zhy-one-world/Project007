/********************************************************************
	file name:	msgproc_auction.hpp
	created:	2017/06/11 0:45
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_AUCTION_HPP_
#define _DP_MSGPROC_AUCTION_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	//auction
	void cs2dp_auction_load_self_selling(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_auction_load_self_selling_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);

	void cs2dp_auction_req_auction_list_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_auction_req_auction_data_num_end(db_result_type result, uint32 connindex, std::string param, guid_64 role_guid, int32 cur_page_index);
	void cs2dp_auction_req_auction_list_end_proc(db_result_type result, uint32 connindex, int32 all_data_num, guid_64 role_guid, int32 cur_page_index);

	void cs2dp_auction_req_trade_record_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_auction_req_trade_record_num_end(db_result_type result, uint32 connindex, std::string param, guid_64 role_guid, int32 cur_page_index);
	void cs2dp_auction_req_trade_record_proc_end(db_result_type result, uint32 connindex, int32 all_record_num, guid_64 role_guid, int32 cur_page_index);

	void cs2dp_auction_req_another_sell_info_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_auction_req_another_sell_info_end_proc(db_result_type result, uint32 connindex, int32 check_money_type, guid_64 role_guid, int32 item_template_id);

	void cs2dp_auction_req_self_bid_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_auction_req_self_bid_num_end(db_result_type result, uint32 connindex, std::string param, guid_64 role_guid, int32 cur_page_index);
	void cs2dp_auction_req_self_bid_end_proc(db_result_type result, uint32 connindex, int32 all_data_num, guid_64 role_guid, int32 cur_page_index);

	void cs2dp_auction_req_bid_record_failure_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_auction_req_bid_record_failure_end_proc(db_result_type result, uint32 connindex,guid_64 role_guid);

	void cs2dp_auction_req_bid_notice_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_auction_req_bid_notice_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid);
	void cs2dp_auction_req_bid_notice_red_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid);

}

#endif
