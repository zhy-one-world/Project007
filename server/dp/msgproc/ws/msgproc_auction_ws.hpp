/********************************************************************
	created:	2017年10月17日13:04:37
	file base:	msgproc_auction_ws
	file ext:	hpp
	author:		luoxingyu
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_AUCTION_WS_HPP_
#define _DP_MSGPROC_AUCTION_WS_HPP_

#include <net/tcp_client.hpp>
#include "Logic/auction_def.h"
#include "internal/auction_msg.hpp"
#include "db_manager.hpp"

namespace faith
{
	
	void auction_add_item_into_db_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_add_item_into_db_end_proc(db_result_type result, uint32 connindex, s_auction_info auction_info, guid_64 old_item_guid, s_unit_info role_info, bool is_add_to_log, login_fixed_data third_info);

	void auction_find_buy_target_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_find_buy_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, guid_64 item_guid);
	void auction_del_buy_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, guid_64 item_guid, s_auction_info item_info);

	void auction_find_cancel_target_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_find_cancel_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, guid_64 item_guid);
	void auction_del_cancel_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, s_auction_info auction_info);

	void auction_find_bid_target_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_find_bid_target_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid, int32 price_money_type, int32 price_money_value, guid_64 item_guid);

	void auction_bid_success_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_bid_success_add_replace_info(const ws2dp_auction_bid_success &msg);
	void auction_bid_success_add_notice_info(const ws2dp_auction_bid_success &info);
	void auction_bid_success_end_proc(db_result_type result, uint32 connindex,  s_auction_info auction_item_info);

	void auction_get_and_clear_old_item_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_del_no_buyer_old_item_proc(std::string param_string, uint32 connindex);
	void auction_get_old_item_proc(const std::string& param_string, uint32 connindex);
	void auction_get_old_item_end_proc(db_result_type result, uint32 connindex);
	void auction_send_old_item_proc(db_result_type result, const dp2ws_auction_clear_old_item_end& msg, uint32 connindex);

	void auction_add_share_list_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_add_notice_is_sell(const ws2dp_auction_add_purchase_record &info);
	void auction_add_trade_record_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	void auction_req_share_list_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_req_share_list_end_proc(db_result_type result, uint32 connindex, int32 money_type, int32 money_value, guid_64 buyer_guid, guid_64 item_guid, int32 item_template_id);
	

	void auction_check_legion_selling_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_check_legion_selling_end_proc(db_result_type result, uint32 connindex, guid_64 legion_guid);

	void auction_del_bid_record_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	void auction_clear_old_bid_record_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	void ws2dp_req_check_world_auction_sell(uint32 connindex, const void *data_ptr, size_t data_len);
	void check_world_auction_sell_end(db_result_type result, uint32 connindex);

	void auction_update_sell_show_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_update_sell_show_proc_end(db_result_type result, uint32 connindex, s_auction_info auction_info);

	void auction_time_out_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void auction_time_out_proc_end(db_result_type result, uint32 connindex);
	void auction_send_time_out_proc(db_result_type result, const dp2ws_aution_time_out_end& msg, uint32 connindex);

	void auction_add_item_into_db_proc_lua(uint32 connindex, const char *data_ptr, size_t data_len);
	void auction_find_buy_target_proc_lua(uint32 connindex, int64 role_guid, int64 item_guid);
	void auction_find_cancel_target_proc_lua(uint32 connindex, int64 role_guid, int64 item_guid);
	void auction_find_bid_target_proc_lua(uint32 connindex, int64 role_guid, int64 item_guid, int32 money_type, int32 money_value);
	void auction_bid_success_proc_lua(uint32 connindex, int64 new_record_id, int64 old_buyer_guid, const char *data_ptr, size_t data_len);
	void auction_get_and_clear_old_item_proc_lua(uint32 connindex, int32 trade_type, int64 cur_time);
	void auction_add_share_list_proc_lua(uint32 connindex, int64 item_guid[auction_share_list_max_num], int64 role_guid[auction_share_list_max_num], float share_rate[auction_share_list_max_num], xstring share_mail_title[auction_share_list_max_num], xstring share_mail_content[auction_share_list_max_num]);
	void auction_add_trade_record_proc_lua(uint32 connindex, int64 record_guid, int64 role_guid, bool is_sell, const char *data_ptr, size_t data_len);
	void auction_req_share_list_proc_lua(uint32 connindex, int64 item_guid, int32 item_template_id, int64 buyer_guid, int32 money_type, int32 money_value);
	void auction_check_legion_selling_proc_lua(uint32 connindex, int64 legion_guid, int64 cur_time);
	void auction_del_bid_record_proc_lua(uint32 connindex, int64 item_guid);
	void auction_clear_old_bid_record_proc_lua(uint32 connindex);
	void auction_update_sell_show_proc_lua(uint32 connindex, int32 end_time, const char *data_ptr, size_t data_len);
	void auction_time_out_proc_lua(uint32 connindex, int64 cur_mil_sec, int64 sell_time, int64 deal_time, int32 is_first);
}

#endif