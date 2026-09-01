/********************************************************************
created: 2019年5月24日
file base: cross_ladder_mgr
file ext: h
author: pengbiao
purpose: 跨服天梯
*********************************************************************/

#ifndef _CROSS_LADDER_WS_MGR_H_
#define _CROSS_LADDER_WS_MGR_H_

#include "Logic/map_def.hpp"
#include "Logic/gate_def.hpp"
#include "Logic/cross_ladder_def.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_ladder_msg.hpp"

namespace faith
{
	class c_cross_ladder_role_info
	{
	public:
		c_cross_ladder_role_info();
		~c_cross_ladder_role_info();

		bool	is_valid()
		{
			return m_role_info.role_guid.is_valid();
		}

		void	set_role_info(const s_cross_ladder_role_info& info);
		void	set_role_guid(const guid_64& tmp_guid);
		void	set_role_name(const xchar* name);
		void	set_role_name(const xstring& name);
		void	set_server_name(const xchar* name);
		void	set_data_array_info(e_cross_ladder_role_info info_type, int32 info_value);
		void	set_gs_value(int64 gs_value);

		void	reduce_ticket();
		void	add_ticket_num();

		s_cross_ladder_role_info& get_role_info() { return m_role_info; }
		const guid_64&	get_role_guid() const { return m_role_info.role_guid; }
		const xchar*	get_role_name() const { return m_role_info.role_name; }
		const xchar*	get_server_name() const { return m_role_info.server_name; }
		int32	get_data_array_info(e_cross_ladder_role_info info_type) const;
		int64	get_gs_value();

		e_cross_ladder_role_state get_pk_state() { return m_role_state; }
		void	set_pk_state(e_cross_ladder_role_state state) { m_role_state = state; }
		void	check_reset_pk_state();

		void	set_sign_up_time(int64 time) { m_sign_up_time = time; }
		int64	get_sign_up_time() const { return m_sign_up_time; }

		void	set_previous_opponent(guid_64 role_id) { m_previous_opponent = role_id; }
		guid_64 get_previous_oppenent() { return m_previous_opponent; }

		void	set_server_info(const xstring& out_ip, int32 out_port);
		const xstring&	get_server_out_ip() { return m_server_out_ip; }
		int32	get_server_out_port() { return m_server_out_port; }

		void	game_win();
		void	game_lose();
		void	game_peace();
		void	game_settle_common(e_map_war_result result, const std::vector<int32> & reward_vec, bool is_upgrade);

		void	update_info_to_dp();
		void	refresh_season_info();
		void	send_last_season_score(int32 server_id = 0);
	private:
		bool	check_is_upgrade(int32 old_score, int32 new_score);
	private:
		s_cross_ladder_role_info	m_role_info;	//基本信息
		e_cross_ladder_role_state	m_role_state;	//状态

		int64						m_sign_up_time;
		guid_64						m_previous_opponent;
		xstring						m_server_out_ip;
		int32						m_server_out_port;
	};


	using cross_ladder_activity_time_vec	= std::vector<s_cross_ladder_activity_time>;
	using cross_ladder_all_role_info_map	= std::unordered_map<ui64, c_cross_ladder_role_info>;
	using cross_ladder_sign_up_list			= std::list<c_cross_ladder_role_info*>;
	using cross_ladder_all_sign_up_map		= std::unordered_map<int32, cross_ladder_sign_up_list>;
	using server_online_num_map				= std::unordered_map<int32, int32>;
	using cross_ladder_transfer_list		= std::list<s_cross_ladder_transfer_info>;
	using cross_ladder_map_info				= std::unordered_map<ui64, s_cross_ladder_match_info>;
	using cross_ladder_season_reward_info	= std::list<s_cross_ladder_season_reward_info>;
	using cross_ladder_upgrade_notify_map   = std::unordered_map<ui64, int32>;
	using cross_ladder_transfer_info_map    = std::unordered_map<ui64, s_game_info>;

	class cross_ladder_ws_mgr_new
	{
	public:
		static cross_ladder_ws_mgr_new& get_instance()
		{
			static cross_ladder_ws_mgr_new instance;
			return instance;
		}
		cross_ladder_ws_mgr_new();
		~cross_ladder_ws_mgr_new();

		void	clear_data();
		void	init_manager(bool need_load_dp = true);
		void	tick(int64 time_new);

		bool	is_in_game_time();
		bool	is_in_activity_time();

		bool	is_match_server();
		int32	get_match_server_id();

		int64	transfer_date_to_stamp(int64 date);
	public:
		/*****************************************************************/
		/*							角色原区ws功能						 */
		/*****************************************************************/
		bool	send_req_cross_ladder_role_info(client_session* session, bool is_online = false);
		e_cross_ladder_sign_up_ret	check_can_sign_up(client_session* session);

		void	deal_with_match_success_msg(const guid_64& role_guid, const ws2ws_cross_ladder_send_match_success* msg);
		void	transfer_player(int64 time_new);
		void	deal_with_pk_result_msg(const guid_64& role_guid, const ws2ws_cross_ladder_sync_game_result* msg);
		void	send_upgrade_notify(client_session* session);
		void	session_online(client_session* session);
		bool	get_transfer_game_info(const guid_64& role_guid, const int32& server_id, s_game_info& game_info);
		void	send_activity_begin_notice();

	public:
		/*****************************************************************/
		/*							比赛区ws功能						 */
		/*****************************************************************/

		void	deal_with_create_map_msg(s_cross_ladder_match_info& match_info);
		void	deal_with_create_robot_map(s_cross_ladder_match_info& match_info);
		void	send_match_success_msg(const guid_64& role_guid, int32 server_id, guid_64 map_id, int32 war_index, const s_cross_ladder_match_role_info& opponent_info);
		void	deal_with_pk_result_from_cs(const guid_64& map_id, const guid_64& winner_guid);

	public:
		/*****************************************************************/
		/*							匹配服ws功能						 */
		/*****************************************************************/
		void	load_info_from_db();
		void	add_cross_ladder_info(const s_cross_ladder_role_info& info);
		void	req_cross_ladder_info(const guid_64& role_guid,const ws2ws_cross_ladder_req_role_info* msg);
		void	send_cross_ladder_info(const c_cross_ladder_role_info& info, bool is_online = false);
		void	set_is_load_all(bool is_load_all) { m_is_load_all = is_load_all; }

		void	refresh_daily_ticket_info();
		void	req_buy_ticket(const guid_64& role_guid);
		void	refound_ticket_cost(const guid_64& role_guid, int32 server_id);
		void	req_cross_ladder_last_score(const guid_64& role_guid, int32 server_id);

		e_cross_ladder_sign_up_ret	sign_up(const guid_64& role_guid);
		e_cross_ladder_sign_up_ret	sign_up_leave(const guid_64& role_guid);
		void	send_sign_up_result(const guid_64& role_guid, e_cross_ladder_sign_up_ret result_type, int32 server_id);
		void	update_server_online_num(int32 server_id, int32 online_num);
		void	set_role_server_info(const guid_64& role_guid, const xstring& server_ip,int32 server_port);

		void	begin_one_season(int32 cur_season_num);
		void	end_one_season();
		void	send_season_reward_to_player(const guid_64& role_guid);
		void	send_season_ranking_reward_to_player(const guid_64& role_guid, int32 server_id);
		void    send_season_upreward_to_player(const guid_64& role_guid);

		void	match(int64 time_new);
		void	match_success(c_cross_ladder_role_info* first_role, c_cross_ladder_role_info* second_role);
		bool	match_with_robot(c_cross_ladder_role_info* role_info, const std::vector<int32> & robot_template_id);
		int32	get_ladder_segment_value(const c_cross_ladder_role_info& info);
		void	get_expend_range(const c_cross_ladder_role_info& info, int64 time_new, int32 expend_time, int32& upper_limit, int32& lower_limit);
		c_cross_ladder_role_info* get_first_valid_role(int32 segment_key, int64 time_new, c_cross_ladder_role_info* cur_role_ptr);

		void	reset_role_state(const guid_64& role_guid);
		void	deal_with_pk_result_from_ws(bool draw, const guid_64& winner_guid, const guid_64& loser_guid);

		int32	get_last_win_limit(){ return m_config_last_win_limit; }
		int32	get_last_win_rate() { return m_config_last_win_rate;  }
		int32	get_rank_limit_socre() { return m_config_rank_limit_score; }
		int32	get_cur_season()	{ return m_cur_season; }

	private://通用
		int32								m_send_notify_flag;
		int64								m_sec_timer;
		int64								m_activity_timer;
		bool								m_is_in_activity_time;
		int32								m_cur_season;
		cross_ladder_activity_time_vec		m_activity_time_vec;

	private://原区ws
		cross_ladder_transfer_list			m_transfer_list;						//延迟传送列表
		cross_ladder_upgrade_notify_map		m_notify_map;							//段位提升通知列表
		cross_ladder_transfer_info_map		m_transfer_info_map;					//比赛服务器信息存储

	private://比赛区ws
		cross_ladder_map_info				m_war_map_team_map;						//本服务器创建的比赛地图
		cross_ladder_transfer_info_map		m_role_server_info_map;					//用于玩家比赛结束返回原区使用

	private://匹配服

		bool								m_refresh_flag;
		bool								m_is_load_all;
		cross_ladder_all_role_info_map 		m_all_ladder_info_map;					//所有玩家信息
		cross_ladder_all_sign_up_map 		m_all_sign_up_map;						//所有报名信息
		server_online_num_map 				m_server_online_num_map;				//服务器在线人数map（不是准确数值）

		bool								m_send_reward_flag;						//奖励分批发送
		cross_ladder_season_reward_info		m_season_reward_list;

	private:
		int32								m_config_match_server_id;
		int32								m_config_level_limit;
		int32								m_config_transfer_time;
		int32								m_config_daily_ticket_num;
		int32								m_config_daily_can_buy_num;
		int32								m_config_daily_ticket_cost;
		int32								m_config_daily_ticket_cost_type;
		int64								m_config_match_time_limit;				//匹配时间限制
		int32								m_config_last_win_limit;				//连胜加成次数上限
		int32								m_config_last_win_rate;					//连胜加成系数
		int32								m_config_rank_limit_score;				//全服排行榜积分限制
	};
}
#endif
