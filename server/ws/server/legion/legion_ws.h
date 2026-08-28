#ifndef _WS_LEGION_WS_H_
#define _WS_LEGION_WS_H_


#include "legion_ws_applicant_list.h"
#include "legion_ws_event_logger.h"
#include "legion_ws_boss_record_set.h"
#include "legion_ws_bonus_info.h"
#include "legion_ws_warehouse.h"
#include "../client_session_mgr.hpp"
#include "base/ecs_world.h"

namespace hld
{
	class game_proto_legion_aunction_item_one;

	class legion_ws
	{
	public:
		explicit legion_ws(guid_64 legion_guid);
		explicit legion_ws();
		explicit legion_ws(const s_legion_info& legion_info);

		void								init();
		void								tick(const uint64& tick_time);
		void								tick_1_min(const uint64& tick_time);
		void								tick_5_min(const uint64& tick_time);
		void								tick_1_day(const uint64& tick_time);
		void								refresh_day_for_zero();
		void								refresh_day_for_five();
		void								refresh_week_for_zero();
		void								refresh_week_for_twenty_three();
		void								refresh_weekly();
		void								save_legion_info_to_db();

		void								save_legion_member_info_to_db(guid_64 member_guid);
		void								save_legion_member_info_to_db(const s_legion_member_info& member_info);
		void								del_legion_member_info_to_db(guid_64 member_guid);

		void								load_all_legion_member_info_from_db();
		void								load_all_applicant_from_db();
		void								load_all_legion_boss_record_from_db();
		void                                load_all_legion_warehouse_info_from_db();
		void                                load_all_legion_warehouse_log_from_db();
		void								load_all_legion_city_war_member_info_from_db();


		void								set_legion_info(ELegionInfo info_index, int64 info_data, bool sv_to_db = false);
		int64								get_legion_info(ELegionInfo info_index) const { return m_legion_info.data_ary[info_index]; }
		void								set_legion_gs_value(int64 gs_value);
		int64								get_legion_gs_value()const { return m_legion_info.gs_value; }


		s_legion_member_info*				get_member(guid_64 player_guid);
		const s_legion_member_info*			get_member(guid_64 player_guid) const;
		void								send_city_war_reward_with_terr_id(int32 terr_id);
		void								send_city_war_member_info_msg();
		void								add_city_war_member(guid_64 role_guid, int32 use_type);
		void								add_city_war_member_from_db(s_legion_city_war_member_info& c_member_info);
		void								save_city_war_member_to_db(s_legion_city_war_member_info& c_member_info);
		void								clear_all_city_war_member();
		void								clear_all_city_war_member_by_type(int32 use_type);
		//s_legion_member_info*				get_member(int32 legion_member_index);
		bool								add_member(s_legion_member_info& member_info, bool is_from_db = false, bool is_need_save_db = true, const login_fixed_data& third_info = login_fixed_data(), const int32& login_type = e_login_type_new_account_and_password);
		void								on_member_add(const s_legion_member_info& member_info, bool is_need_save_db = true);
		void								del_member(guid_64 player_guid, const login_fixed_data& third_info = login_fixed_data(), const int32& login_type = e_login_type_new_account_and_password);
		void								on_member_delete(const s_legion_member_info& member_info);
		void								clear_member();

		void								send_mail_to_all_member_with_master_city_war(bool is_winner = false);//霸主战所有人都发邮件奖励
		void								send_mail_to_officer_with_city_bid(int32 ter_id);//被超标后发邮件给军团长和副军团长

		const LegionConstructionsTemplate*	get_legion_cfg(hld::ELegionInfo construction_index = ELegionInfo_construction_level_main) const;
		void								recalcu_legion_fighting_power();
		bool								cost_maintain_money();
		void								add_asset_money(int32 add_val);
		void								add_glory_glow(int32 add_val);

		int32								appoint(guid_64 player_guid, e_legion_job_title job_title, const login_fixed_data& third_info = login_fixed_data(), const int32& login_type = e_login_type_new_account_and_password);
		e_legion_job_title					get_job_title(guid_64 player_guid) const;
		FString								get_job_title_string(guid_64 player_guid);
		bool								is_chief(guid_64 player_guid);
		bool								is_assistant_chief(guid_64 player_guid);
		void								set_member_online(client_session* legion_member_session, bool online);
		void								on_legion_member_reconnect(client_session* mem_session); //军团断线不处理 只管重连 要不然依赖e_legion_member_info_is_online的逻辑有问题
		void								on_legion_member_login(client_session* mem_session);
		void								on_legion_member_logout(client_session* mem_session);
		void								add_member_contribution(guid_64 player_guid, int32 add_contri_val);

		void								set_legion_rank(int32 rank) { m_legion_rank = rank; }
		int32								get_legion_rank();

		void								update_member_info_one(guid_64 player_guid, int32 info_index, int64 info_value, bool need_sync2c = true);
		void								update_member_info_login(client_session* mem_session);
		void								update_member_gs_value(guid_64 player_guid, int64 gs_value);
		void								update_member_contribution(guid_64 player_guid, int64 contribution_value);
		const guid_64&						get_legion_guid() const { return m_legion_info.legion_guid; }
		void								set_legion_name(const xchar* legion_name, int32 legion_name_len);
		xchar*								get_legion_name();
		int32								get_legion_level() { return get_legion_info(ELegionInfo_construction_level_main); }
		guid_64								get_chief_guid() { return m_chief_guid; }
		xchar*								get_chief_name();
		void								set_announcement(const xchar* content, int32 content_len, bool need_send_mail = true, bool need_save_db = true, guid_64 do_change_role_guid = guid_64(), const login_fixed_data& third_info = login_fixed_data(), const int32& login_type = e_login_type_new_account_and_password);
		xchar*								get_announcement() { return m_legion_info.announcement; }
		int32								get_member_count() const { return m_member_list.size(); }
		int32								get_max_member_count() const;
		bool								is_legion_full() const;

		bool								is_auto_accept_new_member();

		std::list<s_legion_member_info>&	get_member_list() { return m_member_list; }

		int32								construction_level_up(int32 construction_index);

		bool								fill_legion_info_msg(legion_proto_legion_info& legion_info_msg, client_session* client_session_ptr = nullptr);
		void								fill_legion_brief_info_msg(legion_proto_legion_brief_info& legion_brief_info_msg);
		void								fill_member_info_msg(legion_proto_member_info* member_info_msg, const s_legion_member_info& member_info_ref);
		void								fill_city_war_member_info_msg(legion_proto_city_war_member_info* member_info_msg, const s_legion_city_war_member_info& member_info_ref);
		void								fill_legion_welfare_member_info_msg(legion_proto_legion_welfare_member_info* member_info_msg, const s_legion_city_war_member_info& member_info_ref);

		bool								is_in_this_legion(guid_64 target_guid);
		
		void								send_cs_msg_to_all_member(const void* data_ptr, size_t data_len);
		void								send_message_to_all_member(google::protobuf::Message* net_pro, uint32 header);
		void								send_message_to_all_member_with_map_type(google::protobuf::Message* net_pro, uint32 header, int32 map_type);
		void								send_message_to_all_member_except(google::protobuf::Message* net_pro, uint32 header, guid_64 except_member);
		void								send_message_to_all_member_data(const void* data_ptr, int32 data_len,uint32 header);
		void                                send_message_to_all_member_data_lua(const char* data_ptr, int32 data_len, uint32 header);
		//void								send_message_to_all_cs(const void* data_ptr, size_t data_len);
		void								send_legion_all_info(client_session* session);
		//void								send_legion_all_info_to_all_member();
		void								send_legion_attr_all();
		void								send_legion_attr_one(ELegionInfo legion_info_index);
		void								send_legion_gs_value();
		void								send_legion_member_info_all(guid_64 member_guid);
		void								send_legion_member_info_one(guid_64 member_guid, e_legion_member_info info_index);
		void								send_legion_member_gs_value(guid_64 member_guid);
		void								send_legion_member_contribution_value(guid_64 member_guid);
		void								send_legion_announcement();
		void								send_add_member_message(const s_legion_member_info& new_member_info);
		void								send_del_member_message(const s_legion_member_info& del_member_info);
		void								send_construction_level_up_message(int32 construction_index);
		void								send_boss_record_list_to_all_member();
		void								send_legion_boss_award_get_log_to_all_member();
		void								send_legion_city_war_info(client_session* session);

		void								send_aoi_legion_info(client_session* mem_session, bool is_new_join = false);
		void								send_aoi_legion_info_to_all_member();
		void								get_player_legion_info(s_player_legion_info& legion_info, guid_64& role_guid);
		void								send_legion_boss_map_info(guid_64 role_guid);
		void								send_legion_average_lv_to_map_cs(Entity* map_end);

		void                                send_legion_strategy_world_boss();
		void                                send_one_legion_strategy_world_boss(guid_64 role_guid);

		void								save_legion_applicant_to_db();

		legion_ws_applicant_list&			get_need_save_db_applicant_list() { return m_need_save_db_applicant_list; }
		legion_ws_applicant_list&			get_applicant_list() { return m_applicant_list; }
		legion_ws_event_logger&				get_event_logger() { return m_event_logger; }
		legion_ws_boss_record_set&			get_boss_record_set() { return m_boss_record_set; }
		legion_ws_bonus_info&				get_bonus_info_ins() { return m_bonus_info; }
		legion_ws_warehouse&                get_warehouse() { return m_warehouse; }

		//自动检查军团长传位
		void								auto_appoint_chief();
		bool								is_member_offline_too_long_to_Dismiss(s_legion_member_info& member_info);
		guid_64								get_next_chief_guid();
		void								check_chief_valid_and_change();

		//GM指令将军团长踢出军团，并且更换一个新的军团长
		void								gm_kick_out_chief();

		//篝火
		int32								get_bonfire_add_fuel_times() { return m_bonfire_add_fuel_times; }
		void								send_player_bonfire_info(client_session* p_session);
		void								on_create_bonfire_npc();
		void								add_fuel_to_bonfire_succeed(guid_64 mem_guid);
		void								req_bonfire_rank_reward(guid_64 mem_guid);
		guid_64								get_valid_legion_station_map();	//活动军团驻地的地图guid

		uint64								get_last_recruit_in_world_stamp() { return m_last_recruit_in_world_stamp; }
		void								set_last_recruit_in_world_stamp(uint64 stamp) { m_last_recruit_in_world_stamp = stamp; }
		void								req_send_legion_recruit_in_world(guid_64 player_guid);
		bool								can_world_recruit_now();
		void								send_legion_recruit(guid_64 sender_guid, std::string sender_name = "", bool is_send_when_create = false);

		const std::set<int32>&				get_city_war_territories() { return m_city_war_territories; }
		void								add_city_war_territory(int32 terr_id);
		void								del_city_war_territory(int32 terr_id);
		void								send_one_territory_to_all_mem(int32 terr_id);
		void								clear_legion_city_war();
		void								maintain_territories();
		void								level_up_legion_skill(guid_64 player_guid, int32 skill_template_id);
		std::vector<int32>					get_legion_occupied_territorys();
		void								maintain_territories_cross_city();
		std::vector<int32>					get_legion_occupied_territorys_cross_city();

		void								del_legion_station_map();
		void								create_legion_station_map();
		void								join_legion_station_map(guid_64 role_guid);
		void								legion_brofire_end();
		void								set_legion_boss_create_time(int64 creat_time);
		/************************************************************************/
		/*                           公告                                       */
		/************************************************************************/
		void								notice_add_member(FString member_name);	//, int32 template_id = 0
		void								notice_del_member(FString member_name);	//, int32 template_id = 0
		void								notice_appoint(FString member_name, FString old_job_title, FString new_job_title);
		void							    notice_on_create_bonfire();
		void								notice_occupied_treeitory(int32 territory_id);
		void								notice_on_legion_boss();
		void								notice_world_mistress();
		void								notice_add_bonfire_successfull(std::string add_bonfire_player_name, int32 get_contribution);
		void								notice_get_bonus_reward(guid_64 send_guid, xstring member_name, int32 gold_num);
		void								notice_member_change_name(const xstring& original_name, const xstring& current_name);
		void								notice_member_change_legion_name(const xstring& original_name);
		void								notice_on_begin_overload_city_war(int32 rank_idex);
		void								notice_on_end_overload_city_war(int32 rank_idex);
		void								mail_on_begin_overload_city_war(int32 rank_idex);
		void								mail_on_end_overload_city_war(int32 rank_idex);
		void								notice_on_join_overload_city_war();
		void								mail_on_attack_city_group_up(int32 rank_level);
		void								mail_on_attack_city_single_reward(int32 rank_level, const guid_64 * guid_list, int32 data_num);
		void								set_attack_city_single_reward_by_rank(std::vector<s_item_template_info> & drop_list, int32 rank, int32 legion_level);
				
		void								check_legion_selling_end(int32 selling_num);
		void								set_auction_selling_check_start(bool is_start);

		void								send_legion_attack_city_call(guid_64 role_guid);
		/************************************************************************/
		/*                           军团活动                                   */
		/************************************************************************/
		void								check_reset_act_info(e_legion_act_type act_type);
		void								set_act_got_score(e_legion_act_type act_type, int64 score_val);
		void								add_act_attend_mems_info(e_legion_act_type act_type, const s_legion_member_attend_activity* attend_mem_arr, int32 attend_mem_num);
		void								add_act_attend_mem_info(e_legion_act_type act_type, const s_legion_member_attend_activity& attend_mem);
		void								give_activity_reward(ActivityCommonConfigTemplate* act_config_ptr, const legion_act_attend_mems_map& mem_list, int32 item_id, int32 item_num);//军团拍卖
		void								give_activity_reward(const ActivityCommonConfigTemplate* act_config_ptr, const guid_64 *mem_list, int32 play_num, int32 item_id, int32 item_num);
		int32								get_ranking_list_type(e_legion_rank_type legion_type, int32 sub_id = -1);
		void								gen_info_in_rank_list(s_ranking_player_info& new_rank_info, int64 rank_score, e_legion_rank_type rank_type, int32 sub_id = -1);
		legion_act_attend_mems_map&			get_attend_act_mems_map(e_legion_act_type act_type) { return m_attend_act_mems_map[act_type]; }
		std::string							get_kill_legion_boss_name();
		void								set_kill_legion_boss_name(const xchar* killer_name);
		void								refresh_city_war_can_award();
		void								refresh_master_city_war_can_award();

		int32								get_job_title_num(int32 job_title);

		int64								get_last_stimulated_stamp_by_guid(guid_64 player_guid);
		void								insert_last_stimulated_stamp(guid_64 player_guid, int64 stmp);

		void								join_voice_channel(guid_64 role_guid);
		void								leave_voie_channel(guid_64 role_guid);

		void								send_applicant_info_list(client_session* session);//发送申请军团人员列表到client
		void								send_applicant_info_list_to_have_job_member();//发送申请人员列表到所有拥有职位的人

		void								change_player_name_func(const guid_64& role_guid, const xstring& role_name);
		void								send_legion_info_to_all_member();	//给全部成员刷新军团信息
		void								send_change_name_mail_to_all_member(const xstring& original_name, const xstring& current_name);		//给军团内玩家发送改名邮件
		void								send_change_legion_name_mail_to_all_member(const xstring& original_name);		//给军团内玩家发军团改名邮件
		void								change_legion_name(const guid_64& role_guid, const xchar* legion_name, int32 legion_name_len); //判断是否可以更改军团名
		void								change_legion_name_end(const guid_64& role_guid, int32	num);
		void								confirm_change_legion_name(const guid_64& role_guid, const xchar* legion_name, int32 legion_name_len);		//确认更改军团名
		void								ref_all_about_legion_name(const guid_64& role_guid); //刷新关于军团名的其他信息
		void								set_m_new_legion_name(const	xchar* new_legion_name);

		void								send_mail_to_all_member_with_city_rank(int32 rank_idex, guid_64* play_guid_list, int32 play_num);//跨服城战排名邮件奖励

		void								add_cross_city_territories_prior(int32 terr_id);
		void								add_cross_city_territories_common(int32 terr_id);
		void								del_city_war_territory_prior(int32 terr_id);
		void								del_city_war_territory_common(int32 terr_id);
		void								send_to_gate_clear_bid_info(int terr_id);

		void								set_legion_answer_info(s_legion_answer_info answer_info) { m_legion_answer_info = answer_info; }
		s_legion_answer_info*				get_legion_answer_info() { return &m_legion_answer_info; }

		bool								is_all_member_logout_long_time();//是否所有成员均长时间未登陆
		void								set_legion_boss_damage_player(const guid_64* player_guid_arr, int32 player_num);
		void								send_legion_reward_with_legion_boss_rank(int32 rank);
		void								send_legion_boss_notice();
		void								send_all_legion_boss_notice();
		void								send_legion_boss_auction_item(std::vector<s_item_template_info>& _item_list, int32 act_type);
		void								fill_one_auction_item_msg(game_proto_legion_aunction_item_one & one_msg, int32 item_temp_id, int32 item_num, int32 item_lock);

		void								set_is_load_legion_member_end(bool is_end) { m_is_load_legion_member_end = is_end; };

		void								send_legion_welfare_by_activity_and_rank(int32 activity_type, int32 rank_index);
		void								send_legion_welfare_by_npc(int32 npc_temid, int32 rank_index);
		void								legion_welfare_account();
		bool								week_is_have_player_active();
		void								send_legion_welfare_member_info_msg();
		void								set_next_bid_time();
		bool								is_can_bid();

		void                                set_strategy_world_boss_arr(int32 index, int32 value);
		int32								get_strategy_world_boss_arr(int32 index);

		xstring                             get_all_member_guid();
		/************************************************************************/
		/*                           军团仓库                                   */
		/************************************************************************/

	private:
		s_legion_info						m_legion_info;
		std::list<s_legion_member_info>		m_member_list;
		std::list<s_legion_city_war_member_info>		m_city_war_member_list;
		guid_64								m_chief_guid;
		player_session_map					m_member_sessions; //维护这个可能会有bug 要谨慎

		legion_ws_applicant_list			m_need_save_db_applicant_list;	// 入队申请管理列表
		legion_ws_applicant_list			m_applicant_list;	// 入队申请管理列表
		legion_ws_event_logger				m_event_logger;		// 军团事件记录器
		legion_ws_boss_record_set			m_boss_record_set;	// 军团BOSS记录集合
		legion_ws_bonus_info				m_bonus_info;		// 军团分红记录器
		legion_ws_warehouse                 m_warehouse;        // 军团仓库

		int32								m_legion_rank;
		uint64								m_last_ordered_rank_stamp;
		uint64								m_last_recruit_in_world_stamp;
		uint64								m_recalcu_fighting_power_stamp;
		uint64								m_auto_appoint_chief_stamp;
		uint32								m_creat_legion_boss_stamp;
		std::string                         m_kill_legion_boss_name;
		uint64								m_auto_save_bound_to_db_stamp;
		uint64								m_tick_one_sec_stamp;
		guid_64								m_legion_boss_damage_player_arr[max_legion_member_num];
		int32								m_legion_boss_damage_player_num;

		//篝火相关
		int32								m_bonfire_add_fuel_times;
		unit_guid_map						m_member_add_fuel_times; //成员们拱火次数
		std::set<guid_64>					m_got_bonfire_reward_members;
		
		std::set<int32>						m_city_war_territories; //set默认是升序排序的 如果扣维护的钱可以利用

		std::set<int32>						m_cross_server_city_war_territories; //set默认是升序排序的 如果扣维护的钱可以利用
		
		guid_64								m_legion_station_map_guid;			//记录军团驻地的guid

		bool								m_is_auction_selling;
		uint64								m_next_auction_check_time;
		
		uint32								m_last_attend_act_stamp[e_legion_act_type_max];
		legion_act_attend_mems_map			m_attend_act_mems_map[e_legion_act_type_max]; //各个活动参加成员列表

		int32								m_selling_num;

		unit_guid_map						m_last_stimulate_bonus_stamp;

		std::string							m_new_legion_name;

		bool								m_is_legion_have_change;

		std::set<int32>						m_cross_city_territories_prior; //跨服城战的优先维护城池
		std::set<int32>						m_cross_city_territories_common; //跨服城战的普通维护城池

		int64								m_last_caclu_rank_gs_value;//上次同步信息时GS数值

		s_legion_answer_info				m_legion_answer_info;
		bool								m_is_load_legion_member_end;//是否加载完军团成员
		
		uint64								m_next_bid_time;

		int32                               m_strategy_world_boss[legion_strategy_world_boss_num];//军团攻略世界boss数组
	};
}

#endif