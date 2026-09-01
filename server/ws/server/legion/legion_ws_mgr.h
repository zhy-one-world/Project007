#ifndef _WS_LEGION_WS_MGR_H_
#define _WS_LEGION_WS_MGR_H_

#include "legion_ws.h"
#include "legion_ws_city_war.h"


namespace faith
{
	struct fighting_power_legion_pair
	{
		int64	fighting_power;
		guid_64 legion_guid;
		
		bool operator<(fighting_power_legion_pair& rhs)
		{
			return this->fighting_power < rhs.fighting_power;
		}
	};
	typedef std::map<ui64, legion_ws>	legion_ws_map;			// map的键值对为[legion的GUID/legion实例]
	typedef legion_ws_map::iterator		legion_ws_map_it;
	typedef std::map<ui64, guid_64>		role_legion_pair_map;	// map的键值对为[role的GUID/legion的GUID]

	class legion_ws_mgr
	{

	public:
		static legion_ws_mgr& get_instance()
		{
			static legion_ws_mgr instance;
			return instance;
		}

		void					tick(const uint64& tick_time);
		void					tick_1_min(const uint64& tick_time);
		void					tick_1_day(const uint64& tick_time);
		void					tick_5_min(const uint64& tick_time);
		void					refresh_day_for_zero();
		void					refresh_day_for_five();
		void					refresh_week_for_zero();
		void					refresh_week_for_twenty_three();
		void					delete_legion_from_db(guid_64 legion_guid);
		void					load_all_legion_bonus_info_from_db();
		void					clear_all_legion_city_war_member();
		
		int32					add_legion(legion_ws& new_legion);
		void					del_legion(guid_64 legion_guid, bool need_del_db = true, guid_64 del_role_guid = guid_64(), const login_fixed_data& third_info = login_fixed_data(), const int32& login_type = e_login_type_new_account_and_password);
		legion_ws*				get_legion(guid_64 legion_guid);
		legion_ws_map&			get_legion_map() { return m_legion_map; }
		int32					get_legion_num() { return m_legion_map.size(); }

		void					create_legion_begin(guid_64 player_guid, const xchar* l_name, int32 l_name_len, int32 l_level, bool auto_accept);
		void					create_legion_end(s_legion_member_info&	chief_info,guid_64 legion_guid, e_legion_error error_code);
		void					convenient_join_legion(client_session* session);
		void					refresh_all_legion_power();
		int32					is_can_create_legion();
		bool					check_can_join_legion(const legion_ws& legion_ref, guid_64 player_guid, client_session* op_member_session = nullptr);
		bool					check_can_leave_legion(const legion_ws& legion_ref, guid_64 player_guid, client_session* op_member_session = nullptr);
		void					on_player_join(guid_64 legion_guid, guid_64 player_guid, const xstring& legion_name);
		void					on_player_leave(guid_64 legion_guid, guid_64 player_guid, const xstring& legion_name);

		void					generate_player_legion_info(client_session* session, s_legion_member_info& member_info, legion_ws_applicant* applicant_info = nullptr);
		void					send_legion_error_to_session(client_session* session, e_legion_error err_type);
		bool					is_two_player_in_the_same_legion(guid_64 player1_guid, guid_64 player2_guid);
		void					on_player_reconnect(client_session* player_session);
		void					send_message_to_all_legion(google::protobuf::Message* net_pro, uint32 header);

		void					del_rank_with_del_legion(guid_64 legion_guid);

		//加入军团统一接口，true和false控制是否向客服端发送加入失败的消息，目前申请全部逻辑是不需要向客户端发送消息
		bool					join_to_legion(client_session* player_session, guid_64 legion_guid, bool need_send_message_to_client = true);

		// ***********************************
		// *        军团实例查询相关         *
		// ***********************************
		void					set_unit_legion(const guid_64& unit_guid, const guid_64& legion_guid);
		legion_ws*				get_unit_legion(const guid_64& unit_guid);
		void					clear_unit_legion(const guid_64& unit_guid);

		// ***********************************
		// *        军团名称查询相关         *
		// ***********************************
		void					add_name_in_legion_name_searcher(std::string legion_name);
		bool					exist_name_in_legion_name_searcher(std::string legion_name);
		void					clear_name_in_legion_name_searcher(std::string legion_name);

		// ***********************************
		// *        军团列表查询相关         *
		// ***********************************
		void					update_ordered_legion_list();
		void                    add_into_ordered_legion_list(std::vector<fighting_power_legion_pair>& legion_list, int64 legion_fighting_power, guid_64 legion_guid);
		void                    del_from_ordered_legion_list(std::vector<fighting_power_legion_pair>& legion_list, guid_64 legion_guid);
		bool                    get_legion_list(int32 start_index, int64 base_fighting_power, legion_proto_get_legion_list_end& get_legion_list_msg, bool only_accept_new_member_legion);

		void					add_into_auto_accept_new_member_ordered_legion_list(int64 legion_fighting_power, guid_64 legion_guid);
		void					del_from_auto_accept_new_member_ordered_legion_list(guid_64 legion_guid);
		uint64					get_order_legion_list_stamp() { return m_reorder_legion_list_time; }
		int32					get_legion_rank_in_list(guid_64 legion_guid, std::vector<fighting_power_legion_pair>* legion_list_ptr = nullptr);

		// ***********************************
		// *          军团活动相关           *
		// ***********************************
		legion_ws_city_war&		get_legion_city_war() { return m_legion_city_war; }
		bool					check_reset_act_rank(e_legion_act_type act_type);
		void					remove_legion_rank_one(legion_rank_list& rank_list, legion_ws* score_legion_ptr);
		void					insert_legion_rank_one(legion_rank_list& rank_list, legion_ws* score_legion_ptr, int64 legion_score, e_legion_rank_type rank_type, int32 rank_sub_id = -1);
		void					update_legion_rank_one(legion_rank_list& rank_list, legion_ws* score_legion_ptr, int64 legion_score, e_legion_rank_type rank_type, int32 rank_sub_id = -1);
		void					update_act_rank_all(e_legion_act_type act_type);
		void					send_act_rank_reward(e_legion_act_type act_type);
		void					send_act_rank_reward(e_legion_rank_type rank_type, int32 npc_template_id, const legion_rank_list& rank_list, const std::unordered_map<ui64, legion_act_attend_mems_map>& attend_mems_map_list);
		void					send_act_special_reward(e_legion_rank_type rank_type, legion_ws* rwd_legion_ptr, legion_act_attend_mems_map& attend_mems_map);
		void					settle_today_act_rank(e_legion_act_type act_type);
		void					tick_1_min_legion_activity(const uint64& tick_time);
		ActivityCommonConfigTemplate* get_activity_cfg_ptr(e_legion_rank_type rank_type);
		ActivityCommonConfigTemplate* get_activity_cfg_ptr(e_legion_act_type act_type);
		
		legion_rank_list*		get_legion_rank(e_legion_rank_type rank_type, int32 rank_sub_id = -1);
		ranking_list*			get_legion_ranking_by_ranking_type(e_RankingIndex ranking_type);
		void					calcu_all_legion_ranks();
		void					calcu_one_legion_ranks(legion_ws& rank_legion);
		void					send_act_boss_rank_notice(const legion_rank_list& rank_list, const int32& kill_npc_name);
		void					send_act_raid_rank_notice(e_legion_act_type& rank_type);

		void					clear_boss_rank_list();//每次BOSS开始时将排行榜清空
		// ***********************************
		// *          军团BOSS相关           *
		// ***********************************
		legion_ws_boss_award_get_log& get_boss_award_get_log() { return m_boss_award_get_log; }
		bool					is_in_legion_boss_game_time();

		//reanme
		void					change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
		void					change_player_name_in_list(legion_rank_list&  rank_list, const guid_64& role_guid, const xstring& role_name);

		bool					is_need_refresh_overload_city_bid();//判断是否需要更新霸主参赛军团

		legion_ws*				find_legion_ws_with_chief_guid(guid_64 chief_guid);
		legion_ws*				find_legion_ws_with_chief_name(xstring p_role_name);

		void					kick_out_chief_with_chief_account(xstring p_role_name);
		void					kick_out_chief_with_legion_guid(guid_64 legion_guid);
		void					deduct_maintain_territories();
		void					set_is_load_over(bool is_over) { m_is_load_over = is_over; };
		bool					get_is_load_over() { return m_is_load_over; };
		void					set_is_need_load(bool is_need) { m_is_need_load = is_need; };
		bool					get_is_need_load() { return m_is_need_load; };
		void					change_gate_legion_name(guid_64 legion_guid, xstring legion_name);

		void					create_all_legion_station_map();

		// ***********************************
		// *          世界BOSS相关           *
		// ***********************************
		int32                   get_active_legion_num();       //获得活跃军团数目
		void					send_all_legion_world_boss_info();

	private:
		explicit legion_ws_mgr();
		int32									m_min_interval;

		legion_ws_map							m_legion_map;
		role_legion_pair_map					m_role_legion_pair_map;

		std::string								m_legion_name_searcher[max_legion_count];

		// ***********************************
		// *        军团列表查询相关         *
		// ***********************************
		uint64									m_reorder_legion_list_time;
		std::vector<fighting_power_legion_pair>	m_ordered_legion_list;
		std::vector<fighting_power_legion_pair> m_ordered_auto_accept_new_member_legion_list;

		// ***********************************
		// *          军团活动相关           *
		// ***********************************
		legion_ws_city_war						m_legion_city_war;	// 军团城战活动
		legion_rank_list						m_legion_fight_power_rank;
		legion_rank_list						m_legion_city_war_rank;
		legion_rank_list						m_legion_act_ranks[e_legion_act_type_max]; //各个活动排行
		uint32									m_last_update_rank_stamp[e_legion_act_type_max];
		std::unordered_map<int32, legion_rank_list>		m_legion_monster_rank_map;
		// ***********************************
		// *          军团BOSS相关           *
		// ***********************************
		legion_ws_boss_award_get_log			m_boss_award_get_log;	// 所有玩家的军团BOSS奖励领取记录

		bool									m_is_need_refresh_overload_bid_city;//是否需要刷新霸主参战军团

		int64									m_legion_mgr_tick_five_min_last;//5min

		bool									m_is_load_over;
		bool									m_is_need_load;
	};
}

#endif