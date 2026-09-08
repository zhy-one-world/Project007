#pragma once
#include "logic/type_def.hpp"
#include <Logic/char_def.hpp>
#include <Logic/legion_def.hpp>
#include <Logic/transfer_def.hpp>
#include <Logic/ranking_def.hpp>
#include <singleton.hpp>
#include <google/protobuf/message.h>
#include <json/json.h>

namespace faith
{
	class client_session;
	struct packet_s2s;

	struct team_ws
	{
		guid_64 get_team_guid() { return guid_64(); }
		guid_64 get_team_map_guid() { return guid_64(); }
		void on_member_enter_scene(client_session*) {}
		void sync_team_info_to_cs(client_session*) {}
		template<typename... Args> void update_member_map(Args&&...) {}
		template<typename... Args> void gen_player_team_info(Args&&...) {}
	};

	struct legion_ws
	{
		guid_64 get_legion_guid() { return guid_64(); }
		void on_legion_member_login(client_session*) {}
		void on_legion_member_logout(client_session*) {}
		void send_message_to_all_member(google::protobuf::Message*, uint32) {}
		void send_message_to_all_member_data(const void*, size_t, uint32) {}
		void sync_legion_info_to_cs(client_session*) {}
		template<typename... Args> void get_player_legion_info(Args&&...) {}
	};

	struct legion_ws_city_war
	{
		void check_and_send_city_master_online_acconcement(const guid_64&) {}
		void send_city_war_info_to_other_ws() {}
		void clear_data() {}
		template<typename... Args> void set_cross_server_territory_war_state(Args&&...) {}
		guid_64 get_city_master_guid() { return guid_64(); }
		template<typename... Args> void set_city_master_name(Args&&...) {}
		template<typename... Args> void save_city_info_into_db(Args&&...) {}
		guid_64 get_overlord_legion() { return guid_64(); }
	};

	struct marry_couple_data {
		guid_64 couple_guid;
		guid_64 role_guid;
		xchar role_name[64]{};
	};

	template<typename T>
	struct stub_ret { static T get() { return T{}; } };
	template<>
	struct stub_ret<void> { static void get() {} };
	template<typename T>
	struct stub_ret<T*> { static T* get() { return nullptr; } };


	class team_ws_mgr
	{
	public:
		static team_ws_mgr& get_instance() { static team_ws_mgr s; return s; }
		template<typename... Args> void tick(Args&&...) {}
		template<typename... Args> void on_player_login(Args&&...) {}
		template<typename... Args> void on_player_logout(Args&&...) {}
		template<typename... Args> void on_player_offline(Args&&...) {}
		template<typename... Args> void on_player_online(Args&&...) {}
		template<typename... Args> void on_unit_leave_map(Args&&...) {}
		template<typename... Args> void on_team_map_recycle(Args&&...) {}
		template<typename... Args> int32 send_recruit_proc(Args&&...) { return 0; }
		team_ws* get_unit_team(const guid_64&) { return nullptr; }
	};

	class legion_ws_mgr
	{
	public:
		static legion_ws_mgr& get_instance() { static legion_ws_mgr s; return s; }
		template<typename... Args> void tick(Args&&...) {}
		template<typename... Args> void refresh_day_for_zero(Args&&...) {}
		template<typename... Args> void refresh_day_for_five(Args&&...) {}
		template<typename... Args> void refresh_week_for_twenty_three(Args&&...) {}
		template<typename... Args> void clear_all_legion_city_war_member(Args&&...) {}
		template<typename... Args> void create_all_legion_station_map(Args&&...) {}
		template<typename... Args> void calcu_one_legion_ranks(Args&&...) {}
		template<typename... Args> void get_boss_award_get_log(Args&&...) {}
		template<typename... Args> void on_player_reconnect(Args&&...) {}
		legion_ws* get_legion(const guid_64&) { return nullptr; }
		legion_ws* get_unit_legion(const guid_64&) { return nullptr; }
		legion_ws_city_war& get_legion_city_war() { static legion_ws_city_war s; return s; }
	};

	class role_pk_manager : public singleton<role_pk_manager>
	{
		friend class singleton<role_pk_manager>;
		role_pk_manager() = default;
	public:
		template<typename... Args> void tick(Args&&...) {}
		guid_64 get_player_role_pk_map(const guid_64&) { return guid_64(); }
		template<typename... Args> void clear_player_role_pk_map(Args&&...) {}
	};

	class marry_mgr_ws
	{
	public:
		static marry_mgr_ws& get_instance() { static marry_mgr_ws s; return s; }
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void session_online_marry_handle(Args&&...) {}
		template<typename... Args> void session_offline_marry_handle(Args&&...) {}
		template<typename... Args> void sync_marry_mission_info(Args&&...) {}
		template<typename... Args> void sync_wedding_ring_info(Args&&...) {}
		template<typename... Args> void dp2ws_ret_last_login_time_end(Args&&...) {}
		template<typename... Args> void load_data_end(Args&&...) {}
		marry_couple_data get_couple_data(const guid_64&) { return marry_couple_data(); }
	};

	class ranking_mgr_ws
	{
	public:
		static void init_manager() {}
		template<typename... Args> static void heart_tick(Args&&...) {}
		static void delete_ranking_with_server_list_refresh_end() {}
		static ranking_list* get_ranking_list_by_type(int32) { return nullptr; }
		template<typename... Args> static void clear_data_by_ranking_type(Args&&...) {}
		static int64 get_srv_avg_level() { return 0; }
		static void sync_calcu_world_level_tick_day() {}
		static void send_world_srv_lv_info_all() {}
		template<typename... Args> static void send_avg_level_to_client(Args&&...) {}
		template<typename... Args> static void send_service_rank_info_all_to_client(Args&&...) {}
		template<typename... Args> static void get_player_worship_value_end(Args&&...) {}
		template<typename... Args> static void load_all_server_rank_from_db(Args&&...) {}
		template<typename... Args> static void load_one_worship_ranking_from_db(Args&&...) {}
		template<typename... Args> static void load_ranking_from_db(Args&&...) {}
		template<typename... Args> static void load_world_level_from_db(Args&&...) {}
		template<typename... Args> static void set_service_rank_check_index(Args&&...) {}
		template<typename... Args> static void sync_oracle_trial_play_info(Args&&...) {}
	};

	class web_client
	{
	public:
		static web_client& get_instance() { static web_client s; return s; }
		template<typename... Args> void tick(Args&&...) {}
		static void handler_http_onrecv(const void*) {}
		template<typename... Args> void gm_order_end(Args&&...) {}
		template<typename... Args> void do_gm_order(Args&&...) {}
		template<typename... Args> void db_role_info_modified(Args&&...) {}
		template<typename... Args> void send_change_account_end(Args&&...) {}
		template<typename... Args> void set_role_info(Args&&...) {}
		template<typename... Args> void set_role_items_info(Args&&...) {}
		template<typename... Args> void set_role_lock(Args&&...) {}
		template<typename... Args> void set_role_money(Args&&...) {}
		template<typename... Args> void set_role_time(Args&&...) {}
	};

	class cache_ws_mgr
	{
	public:
		static cache_ws_mgr& get_instance() { static cache_ws_mgr s; return s; }
		void init() {}
	};

	class recharge_service
	{
	public:
		static recharge_service& getInstance() { static recharge_service s; return s; }
		static recharge_service& get_instance() { return getInstance(); }
		template<typename... Args> void req_recharge(Args&&...) {}
	};

	class script_mgr
	{
	public:
		static script_mgr& get_instance() { static script_mgr s; return s; }
		template<typename... Args> void call_func(Args&&...) {}
	};

	class assist_fight_mgr
	{
	public:
		static assist_fight_mgr& get_instance() { static assist_fight_mgr s; return s; }
		template<typename... Args> void check_assist_fight_legion_team(Args&&...) {}
		template<typename... Args> void on_player_logout(Args&&...) {}
		template<typename... Args> void sync_assist_fight_info(Args&&...) {}
		template<typename... Args> void sync_assist_fight_info_to_cs_login(Args&&...) {}
	};

	class appearance_info_mgr
	{
	public:
		static appearance_info_mgr& get_instance() { static appearance_info_mgr s; return s; }
		template<typename... Args> void req_load_appearance_info_by_id_end(Args&&...) {}
		template<typename... Args> void req_save_appearance_info_to_id_end(Args&&...) {}
	};

	class arena_mgr_ws
	{
	public:
		static arena_mgr_ws& get_instance() { static arena_mgr_ws s; return s; }
		template<typename... Args> void create_robot(Args&&...) {}
		template<typename... Args> void fill_empty_pos(Args&&...) {}
		template<typename... Args> void heart_tick(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void load_ranks_from_db(Args&&...) {}
	};

	class attack_city_ws_mgr
	{
	public:
		static attack_city_ws_mgr& get_instance() { static attack_city_ws_mgr s; return s; }
		template<typename... Args> void add_rank_data(Args&&...) {}
		template<typename... Args> void clear_legion_list(Args&&...) {}
		template<typename... Args> void clear_rank_data(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void load_group_data_end(Args&&...) {}
		template<typename... Args> void refresh_legion_list(Args&&...) {}
		template<typename... Args> void send_info_all_to_other_ws(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class auction_mgr_ws
	{
	public:
		static auction_mgr_ws& get_instance() { static auction_mgr_ws s; return s; }
		template<typename... Args> void auction_bid_db_sucess(Args&&...) {}
		template<typename... Args> void cancel_sell_end(Args&&...) {}
		template<typename... Args> void dp_check_world_auction_sell_end(Args&&...) {}
		template<typename... Args> void find_bid_end_proc(Args&&...) {}
		template<typename... Args> void find_buy_target_end(Args&&...) {}
		template<typename... Args> packet_s2s* get_auction_buffer(Args&&...) { return nullptr; }
		template<typename... Args> void get_old_item_end_proc(Args&&...) {}
		template<typename... Args> bool is_use_lua(Args&&...) { return false; }
		template<typename... Args> void on_login(Args&&...) {}
		template<typename... Args> void operate_selling_lock(Args&&...) {}
		template<typename... Args> void req_share_list_end_proc(Args&&...) {}
		template<typename... Args> void sell_end(Args&&...) {}
		template<typename... Args> void send_to_dp_show_end(Args&&...) {}
		template<typename... Args> void send_to_dp_time_out_end(Args&&...) {}
		template<typename... Args> void set_need_world_acution_check(Args&&...) {}
		template<typename... Args> void set_world_auction_sell_num(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class best_record_mgr
	{
	public:
		static best_record_mgr& get_instance() { static best_record_mgr s; return s; }
		template<typename... Args> bool is_use_lua(Args&&...) { return false; }
	};

	class big_player_ws_mgr
	{
	public:
		static big_player_ws_mgr& get_instance() { static big_player_ws_mgr s; return s; }
		template<typename... Args> guid_64 get_big_player_guid(Args&&...) { return guid_64(); }
		template<typename... Args> void handle_att_info(Args&&...) {}
		template<typename... Args> void handle_base_detail_info(Args&&...) {}
		template<typename... Args> void handle_buff_info(Args&&...) {}
		template<typename... Args> void handle_item_info(Args&&...) {}
		template<typename... Args> void handle_skill_info(Args&&...) {}
		template<typename... Args> bool is_use_lua(Args&&...) { return false; }
		template<typename... Args> void load_data_by_db_end(Args&&...) {}
		template<typename... Args> void load_statue_by_ws_proc(Args&&...) {}
		template<typename... Args> void set_title_session_login(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class boss_island_ws_mgr
	{
	public:
		static boss_island_ws_mgr& get_instance() { static boss_island_ws_mgr s; return s; }
		template<typename... Args> void send_boss_island_info_all(Args&&...) {}
	};

	class broken_sky_mgr
	{
	public:
		static broken_sky_mgr& get_instance() { static broken_sky_mgr s; return s; }
		template<typename... Args> void set_boss_level(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class chest_arrival_mgr
	{
	public:
		static chest_arrival_mgr& get_instance() { static chest_arrival_mgr s; return s; }
		template<typename... Args> void tick(Args&&...) {}
	};

	class city_war_territory_mgr
	{
	public:
		static city_war_territory_mgr& get_instance() { static city_war_territory_mgr s; return s; }
		template<typename... Args> void clear_all_bid_info(Args&&...) {}
		template<typename... Args> void clear_bid_info_for_time(Args&&...) {}
		template<typename... Args> void pre_load_bid_info(Args&&...) {}
		template<typename... Args> void refresh_bid_info_when_server_list_end(Args&&...) {}
		template<typename... Args> void remove_bid_info_when_server_remove(Args&&...) {}
		template<typename... Args> void send_city_info_all_to_other_ws(Args&&...) {}
	};

	class cloud_shop_mgr
	{
	public:
		static cloud_shop_mgr& get_instance() { static cloud_shop_mgr s; return s; }
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void load_role_record_data(Args&&...) {}
		template<typename... Args> void send_open_msg_to_all_client(Args&&...) {}
		template<typename... Args> void send_open_msg_to_client(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class cross_ladder_ws_mgr_new
	{
	public:
		static cross_ladder_ws_mgr_new& get_instance() { static cross_ladder_ws_mgr_new s; return s; }
		template<typename... Args> void add_cross_ladder_info(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void refresh_daily_ticket_info(Args&&...) {}
		template<typename... Args> void send_upgrade_notify(Args&&...) {}
		template<typename... Args> void session_online(Args&&...) {}
		template<typename... Args> void set_is_load_all(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class cross_server_city_war_ws_mgr
	{
	public:
		static cross_server_city_war_ws_mgr& get_instance() { static cross_server_city_war_ws_mgr s; return s; }
		template<typename... Args> void clear_city_master(Args&&...) {}
		template<typename... Args> void func_clear_all_city_war_msg(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void refresh_rank_with_server_list(Args&&...) {}
		template<typename... Args> void req_load_db_msg(Args&&...) {}
		template<typename... Args> void send_city_info_all_to_client(Args&&...) {}
		template<typename... Args> void send_city_info_all_to_other_ws(Args&&...) {}
		template<typename... Args> void set_refresh_cross_city_war(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class cross_server_harry_ws_mgr
	{
	public:
		static cross_server_harry_ws_mgr& get_instance() { static cross_server_harry_ws_mgr s; return s; }
		template<typename... Args> void func_when_add_other_server(Args&&...) {}
		template<typename... Args> void func_when_remove_other_server(Args&&...) {}
		template<typename... Args> void handle_att_info(Args&&...) {}
		template<typename... Args> void handle_base_detail_info(Args&&...) {}
		template<typename... Args> void handle_belief_info(Args&&...) {}
		template<typename... Args> void handle_buff_info(Args&&...) {}
		template<typename... Args> void handle_fight_att_info(Args&&...) {}
		template<typename... Args> void handle_item_info(Args&&...) {}
		template<typename... Args> void handle_skill_info(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void init_server_money_with_refresh(Args&&...) {}
		template<typename... Args> void recv_load_all_server_harry_msg(Args&&...) {}
		template<typename... Args> void refresh_server_harry_with_server_list(Args&&...) {}
		template<typename... Args> void send_all_harry_msg_to_other_server(Args&&...) {}
		template<typename... Args> void send_award_with_player_defend_value(Args&&...) {}
		template<typename... Args> void send_award_with_server_money_rank(Args&&...) {}
		template<typename... Args> void send_harry_msg_to_one_client(Args&&...) {}
		template<typename... Args> void send_server_harry_best_player_msg_to_gate(Args&&...) {}
		template<typename... Args> void send_server_harry_msg_to_cs(Args&&...) {}
		template<typename... Args> void set_is_need_create_harry_player(Args&&...) {}
		template<typename... Args> void set_server_harry_best_player(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class cross_server_pk_ws_mgr
	{
	public:
		static cross_server_pk_ws_mgr& get_instance() { static cross_server_pk_ws_mgr s; return s; }
		template<typename... Args> void clear_all_island_msg(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void recv_dp_msg_to_ws(Args&&...) {}
		template<typename... Args> void send_island_msg_all_to_client(Args&&...) {}
		template<typename... Args> void send_winner_fake_player_to_other_server(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class cross_server_world_boss_ws_mgr
	{
	public:
		static cross_server_world_boss_ws_mgr& get_instance() { static cross_server_world_boss_ws_mgr s; return s; }
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void locd_boss_show_info(Args&&...) {}
		template<typename... Args> void send_cross_boss_info_all_to_other_ws(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class crystal_fairyland_mgr
	{
	public:
		static crystal_fairyland_mgr& get_instance() { static crystal_fairyland_mgr s; return s; }
		template<typename... Args> void tick(Args&&...) {}
	};

	class element_war_ws_mgr
	{
	public:
		static element_war_ws_mgr& get_instance() { static element_war_ws_mgr s; return s; }
		template<typename... Args> void add_element_war_info(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void refresh_daily_ticket_info(Args&&...) {}
		template<typename... Args> void send_upgrade_notify(Args&&...) {}
		template<typename... Args> void session_online(Args&&...) {}
		template<typename... Args> void set_is_load_all(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class event_ws_mgr
	{
	public:
		static event_ws_mgr& get_instance() { static event_ws_mgr s; return s; }
		template<typename... Args> void load_mail_event_end(Args&&...) {}
		template<typename... Args> void load_mail_event_item_end(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class gain_treasure_ws_mgr
	{
	public:
		static gain_treasure_ws_mgr& get_instance() { static gain_treasure_ws_mgr s; return s; }
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> bool is_use_lua(Args&&...) { return false; }
		template<typename... Args> void load_gain_treasure_record_info_from_db_proc(Args&&...) {}
		template<typename... Args> void send_gain_treasure_boss_info_all(Args&&...) {}
	};

	class legion_ws_answer
	{
	public:
		static legion_ws_answer& get_instance() { static legion_ws_answer s; return s; }
		template<typename... Args> void tick(Args&&...) {}
	};

	class lucky_draw_record_ws_mgr
	{
	public:
		static lucky_draw_record_ws_mgr& get_instance() { static lucky_draw_record_ws_mgr s; return s; }
		template<typename... Args> void load_data_by_db_end(Args&&...) {}
		template<typename... Args> void send_record_all_to_req_player(Args&&...) {}
	};

	class mail_ws_mgr
	{
	public:
		static mail_ws_mgr& get_instance() { static mail_ws_mgr s; return s; }
		template<typename... Args> void check_id_mail_end(Args&&...) {}
		template<typename... Args> void load_other_server_mail_end(Args&&...) {}
		template<typename... Args> void load_other_server_mail_record_end(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class pk_king_mgr
	{
	public:
		static pk_king_mgr& get_instance() { static pk_king_mgr s; return s; }
		template<typename... Args> bool player_is_pk_king(Args&&...) { return false; }
		template<typename... Args> void tick(Args&&...) {}
	};

	class recharge_mgr_ws
	{
	public:
		static recharge_mgr_ws& get_instance() { static recharge_mgr_ws s; return s; }
		template<typename... Args> void db_save_recharge_end(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void send_recharge_end_result(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class red_package_ws_mgr
	{
	public:
		static red_package_ws_mgr& get_instance() { static red_package_ws_mgr s; return s; }
		template<typename... Args> void load_red_package_end(Args&&...) {}
		template<typename... Args> void load_red_package_receiver_end(Args&&...) {}
		template<typename... Args> void send_info_to_one(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class rename_mgr
	{
	public:
		static rename_mgr& get_instance() { static rename_mgr s; return s; }
		template<typename... Args> void change_player_name_end(Args&&...) {}
		template<typename... Args> void check_role_name_end(Args&&...) {}
	};

	class time_limit_activity_temp_ws_mgr
	{
	public:
		static time_limit_activity_temp_ws_mgr& get_instance() { static time_limit_activity_temp_ws_mgr s; return s; }
		template<typename... Args> void heart_tick(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void load_branch_temp_end(Args&&...) {}
		template<typename... Args> void load_new_temp(Args&&...) {}
		template<typename... Args> void load_new_temp_on_server_open(Args&&...) {}
		template<typename... Args> void load_temp_end(Args&&...) {}
		template<typename... Args> void send_to_cs_template(Args&&...) {}
		template<typename... Args> void send_to_gateway_template(Args&&...) {}
	};

	class time_limit_activity_ws_mgr
	{
	public:
		static time_limit_activity_ws_mgr& get_instance() { static time_limit_activity_ws_mgr s; return s; }
		template<typename... Args> void get_activity_common_data_to_gate(Args&&...) {}
		template<typename... Args> void init_all_time_limit_ws(Args&&...) {}
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void load_end_time_limit_act(Args&&...) {}
		template<typename... Args> void refresh_day_for_zero(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};

	class world_boss_ws_mgr
	{
	public:
		static world_boss_ws_mgr& get_instance() { static world_boss_ws_mgr s; return s; }
		template<typename... Args> void init_manager(Args&&...) {}
		template<typename... Args> void send_all_boss_hp_per(Args&&...) {}
		template<typename... Args> void send_world_boss_all(Args&&...) {}
		template<typename... Args> void tick(Args&&...) {}
	};


	namespace cross_transfer_logic
	{
		template<typename... Args> inline void on_player_logout(Args&&...) {}
		inline void tick() {}
	}
}
