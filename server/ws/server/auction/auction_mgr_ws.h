/********************************************************************
file name:	relation_mgr_ws.h
created:	2017/06/20 20:46
author:		zhy
purpose:
*********************************************************************/
#include "Logic/auction_def.h"
#include "Internal/auction_msg.hpp"

namespace faith
{
	class client_session;

	typedef std::unordered_map<ui64, bool> selling_goods_map;
	typedef std::vector<s_auction_timer_info>  wait_show_vector;
	typedef std::vector<s_auction_timer_info>::iterator  wait_show_vector_it;

	typedef std::vector<s_auction_info>  time_tip_vector;
	typedef std::vector<s_auction_info>::iterator  time_tip_vector_it;


	typedef std::queue<s_auction_person_info>  person_info_queue;




	class auction_mgr_ws
	{
	public:
		auction_mgr_ws();
		~auction_mgr_ws();
		void clear_data();
	public:
		static auction_mgr_ws&	get_instance();
		void tick(const int64& new_time);
	public:
		void add_auction_info_to_db(const s_auction_info& info, const guid_64& old_item_guid = guid_64(), const s_unit_info& role_info = s_unit_info(), bool is_add_to_log = false, const login_fixed_data& third_info = login_fixed_data());
		void add_share_list_to_db(const s_auction_share_info* share_list, int32 list_num);
		void send_auction_puchase_success_info(const s_auction_info& info);
		void buy(const guid_64& role_guid, const guid_64& item_guid);
		void find_buy_target_end(const s_auction_info& info, guid_64 role_guid, guid_64 item_guid);
		void cancel_sell(const guid_64& role_guid, const guid_64& item_guid);
		void cancel_sell_end(const s_auction_info& info, guid_64 role_guid);
		void operate_selling_lock(guid_64 item_guid, bool is_add);
		bool is_item_locked(guid_64 item_guid);

		void sell_end(const s_auction_info& info, int32 sell_result, const s_unit_info& role_info, bool is_add_to_log, const login_fixed_data& third_info);
		void sell_rare_item_proc(const s_auction_info& info);

		void trade_bid(const guid_64& role_guid, const guid_64& trade_guid, const int32& price_type, const int32& price_value);
		void find_bid_end_proc(const guid_64& role_guid, const guid_64& item_guid, const s_auction_info& info, const int32& price_type, const int32& price_value);
		void bid_success_proc(const s_auction_info& auction_info, const guid_64& old_buyer);

		void clear_old_item();
		void get_old_item_end_proc(const s_auction_info* auction_info, const int32& data_num);

		void buy_set_bind(s_auction_info *auction_info_ptr);
		void send_reward_to_buyer(const s_auction_info& auction_info);
		void share_reward_to_seller(const s_auction_info& auction_info);
		void add_record_to_db(const s_auction_info& auction_info, guid_64 role_guid = 0, guid_64 record_guid = 0, bool is_sell = false);

		void del_bid_record(const guid_64& item_guid);
		void clear_old_bid_record();

		void req_share_list_end_proc(const int32& money_type, const int32& money_value, const int32& item_template_id,
			const s_auction_share_info* share_info, const int32& data_num, 
			const guid_64& buyer_guid);

		void force_clear_all_old_item();

		void check_world_auction_sell();
		void set_world_auction_sell_num(int32 sell_num);
		void set_need_world_acution_check(bool is_need);
		void dp_check_world_auction_sell_end(int32 sell_num);


		void set_is_show(s_auction_info &auction_info);
		

	private:
		static auction_mgr_ws s_auction_mgr_ws;
		selling_goods_map	m_selling_goods_map;
		selling_goods_map	m_direct_purchase_map;

		//优化
		//std::vector<std::string> notice_string_vector;

		int64				m_old_tick_time;
		int64				m_next_clear_old_legion_item_time;


		bool				m_is_clearing_old_item;
		bool				m_is_force_clear_old_item;
		
		bool				m_need_check_world_auction;
		int32				m_world_sell_num;
		int64				m_next_check_world_auction_time;
	public:
		void  add_wait_show(s_auction_timer_info &auctin_info);
		void  wait_tick();
		s_auction_timer_info& find_timer(guid_64 item_guid);
		void  del_timer(guid_64 item_guid);
		void  send_to_dp_show(s_auction_info auction_info);
		void  send_to_dp_show_end(int32 result, s_auction_info auction_info);
		void  sell_rare_item_proc_ex(const s_auction_timer_info& info);
		void  cancel_del_show(const s_auction_info& auction_info);
	private:
		wait_show_vector    m_wait_show_vec;
		s_auction_timer_info m_empty;
	public:
		void time_out_tick(const int64& new_time);
		void send_to_dp_time_out(int32 is_first);
		void send_to_dp_time_out_end(const s_auction_info *aution_info_array, int32 num);
		void send_to_owner(const s_auction_info& auction_info);
		//void give_item_to_buyer(const s_auction_info& auction_info);
		void give_money_to_seller(const s_auction_info& auction_info);
		void give_item_to_seller(const s_auction_info& auction_info);
		void sync_to_cs_del(const s_auction_info& auction_info);
	private:
		bool is_request_time_out = false;
		int64 m_time_out_last = 0;
		int32 m_is_first = 1;
	public:
		void time_tip_tick();
		void add_tip(const s_auction_info &aution_info);
		void add_wait_time(s_auction_info &info);
		bool is_time_tip(int64 sell_time, int32 cur_time);
		void send_up_notice(const s_auction_info &info);
		void send_prepare_notice(const s_auction_info& info);
	private:
		time_tip_vector   m_time_tip_vec;
	public:
		void update_auction_time(s_auction_info &auction_info);
	public:
		int32 get_config_param(e_auction_param param_type);
	public:
		void on_login(client_session* session); //外部调用
	public:
		void auction_bid_db_sucess(const guid_64& buyer_guid,  const guid_64& item_guid);

	//////////////////////////////个人拍卖///////////////////////////////////
	public:
		void add_person_info(const cs2ws_auction_request_refresh_person *p_data);

		void person_tick();
		void process_person_info(s_auction_person_info& person_info);
		void make_person_auction(s_auction_person_info& person_info, int32 item_id);

		const std::vector<int32> *get_item_config(int32 role_class_type);
		int32 get_area_index(int32 role_lv);
		int32 get_item_id(const std::vector<int32> *p_vec, int32 idx);


	private:
		person_info_queue  m_person_info_queue;
	public:
		bool is_use_lua();
		packet_s2s *get_auction_buffer(s_auction_info& auction_info);



	};

}
