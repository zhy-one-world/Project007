/********************************************************************
	file name:	auction_cs.h
	created:	2017/06/11 0:14
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _AUCTION_CS_H_
#define _AUCTION_CS_H_

#include "Logic/auction_def.h"
#include "Internal/auction_msg.hpp"
#include "Internet/auction.pb.h"
#include "utility/cs_date.hpp"

namespace faith
{
	class player;
	class citem;
	typedef std::vector<s_auction_bid_replace_info> bid_replace_vector;
    typedef std::vector<s_auction_bid_replace_info>::iterator bid_replace_vector_it;

	struct auction_search_condition
	{
		bool		is_waiting_find;
		int64		next_can_find_time;

		int32		main_type;
		int32		sub_type;
		int32		item_color;
		int32		quality_level;
		int32		single_money_sort_mode;
		int32		total_money_sort_mode;
		bool		is_show_gold;
		bool		is_show_diamond;
		int32		show_page;
		xchar		key_word[auction_find_key_word_max_lenth + 1];
		int32		trade_type;
		int32       career_type;
		guid_64		seller_guid;
		guid_64		self_guid;

		auction_search_condition()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(auction_search_condition));
		}
	};

	struct auction_req_record_info
	{
		bool		is_waiting_find;
		int64		next_can_find_time;

		int32		trade_type;
		int32		page_index;

		auction_req_record_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(auction_req_record_info));
		}
	};


	class cauction_mgr
	{
	public:
		cauction_mgr();
		~cauction_mgr();
		void clear_data();
		void heart_tick(int64 new_time);
		void set_array_index(int32 array_index) { m_unit_array_index = array_index; };
	public:
		int32 get_empty_sell_slot();
		void operate(int32 operate_type, guid_64 item_guid, int32 sell_num, int32 price_type, int32 price_value);
		void sell_item(guid_64 item_guid, int32 sell_num, int32 price_type, int32 price_value);
		void sell_item_end(s_auction_info sell_info, int32 sell_result);
		void cancel_sell_item(guid_64 item_guid);
		void cancel_sell_item_end(s_auction_info sell_info);
		void sell_item_success(s_auction_info sell_info);
		void buy_item(guid_64 item_guid);
		void buy_set_bind(s_auction_info &auction_info);
		void buy_item_end(s_auction_info item_info);
		void bid_item(guid_64 item_guid, int32 price_type, int32 price_value);
		void send_selling_list_one(int32 list_index);
		void send_selling_list_one(guid_64 item_guid);
		void make_package_info(auction_proto_auction_item_info* target_info, s_auction_info source_info);
		void send_selling_list_all();

		void load_self_selling_from_db(const s_auction_info* auction_info, int32 data_num);

		bool load_self_selling_by_db_lua(const char *data_ptr, int32 data_len);

		void req_goods_list_end(const s_auction_info* auction_info, int32 data_num, int32 cur_page, int32 total_num);
		
		void remove_from_selling_list(guid_64 item_guid);
		
		void req_goods_list(int32 main_type, int32 sub_type, 
			int32 item_color, int32 quality_level, 
			int32 single_money_sort_mode, int32 total_money_sort_mode,
			bool is_show_gold, bool is_show_diamond, int32 show_page, std::string key_word,
			int32 trade_type, int32 career_type);

		void start_find_list();
		void start_req_record_list();

		void req_trade_record(int32 trade_type, int32 page_index);
		void req_trade_record_end(const s_auction_record_info* auction_info, int32 data_num, int32 cur_page, int32 total_record_num);

		void req_another_sell_info(int32 sell_money_type, int32 item_template_id);
		void req_another_sell_info_end(const s_auction_info* auction_info, int32 data_num, int32 money_type, int32 item_template_id);

		void req_self_bid_info(int32 trade_type, int32 page_index);

		void put_auction_item_bag(const s_auction_info& auction_info);

		void send_reward_to_buyer(const s_auction_info& auction_info);
		void send_reward_to_seller(const s_auction_info& auction_info, const int32& after_tax_reward);
		void add_new_record_to_db(const s_auction_info& auction_info, const int32& after_tax_reward);

		void find_bid_target_end(s_auction_info auction_info);
		void return_money_to_old_buyer(const guid_64& old_buyer_guid, const int32& item_id, const int32& money_type, const int32& money_value);

		void set_is_show(s_auction_info & auction_info);
		//void req_bid_record_failure(int32 in_oper_type);
		//void req_bid_record_failure_end(const s_auction_bid_replace_info *info_array, int32 num);
		void read_record(guid_64 record_guid,int32 type);
		//void sync_bid_record_failure(const s_auction_bid_replace_info &new_info);
		//void sync_client_bid_record_failure_data();
		int32 get_config_param(e_auction_param param_type);
		void time_out_del(const s_auction_info & auction_info);

		void req_bid_notice(int32 in_oper_type);
		void req_bid_notice_end(const s_auction_bid_notice_info * data_array, int32 num);
		void req_bid_notice_red_end(int32 result, int32 is_red);

	private:
		s_auction_info					m_selling_array[max_auction_sell_num];
		auction_search_condition		m_find_info;
		auction_req_record_info			m_req_record_info;
		int32							m_unit_array_index;
		int32                           is_load;
		int32                           m_next_interval;
	/*	std::vector<s_auction_bid_replace_info> m_bid_record_vec;*/
	public:
		void person_tick(int64 new_time);  //在线模式
		void calc_refresh_time(int64 new_time);
		void person_sec_tick(); //离线模式
		void refresh_yesterday();
		void refresh_today();
		bool request_refresh_to_ws(int64 item_time);
		bool is_same_day(int64 dbtime);
	private:
		int64                           m_refresh_time;
		time_info                       m_refresh_info;
	public:
		bool lua_make_auction_to_ws(citem *temp_item, int32 sell_num, int32 price_type, int32 price_value);
		//void lua_buy_item(int32 item_guid_a, int32 item_guid_b);
		//void lua_cancel_sell_item(int32 item_guid_a, int32 item_guid_b);
		//void lua_bid_item(int32 item_guid_a, int32 item_guid_b, int32 price_type, int32 price_value);
		//void lua_read_record(int32 item_guid_a, int32 item_guid_b, int32 type);
	};

}

#endif // !_AUCTION_CS_H_
