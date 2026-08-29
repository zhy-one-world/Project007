/***********************************************
* created	: 2020年8月18日
* file base : element_war_ws_mgr
* file ext  : h
* author    : 赵玉明
* purpose   : 元素争霸
************************************************/

#ifndef _ELEMENT_WAR_WS_MGR_H_
#define _ELEMENT_WAR_WS_MGR_H_

#include "Logic/map_def.hpp"
#include "Logic/gate_def.hpp"
#include "Logic/element_war_def.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "element_war_msg.hpp"
#include "server_log.hpp"

namespace faith
{
	class c_element_war_role_info
	{
	public:
		c_element_war_role_info();
		~c_element_war_role_info();

		bool	is_valid()
		{
			return m_role_info.role_guid.is_valid();
		}

		void	set_role_info(const s_element_war_role_info& info);
		void	set_role_guid(const guid_64& temp_guid);
		void	set_role_name(const xchar* name);
		void	set_role_name(const xstring& name);
		void	set_team_guid(const guid_64& temp_guid);
		void	clear_team_guid();
		void	set_server_name(const xchar* name);
		void	set_data_array_info(e_element_war_role_info info_type, int32 info_value);
		void	set_gs_value(int64 gs_value);
		void	reduce_ticket();
		void	add_ticket_num();
		void	set_segment_value(int32 segment_value);

		s_element_war_role_info& get_role_info() { return m_role_info; }
		const xchar*	get_role_name() const { return m_role_info.role_name; }
		const guid_64&	get_role_guid() const { return m_role_info.role_guid; }
		const guid_64&	get_team_guid() const { return m_team_guid; }		
		const xchar*	get_server_name() const { return m_role_info.server_name; }
		int32	get_data_array_info(e_element_war_role_info info_type) const;
		int64	get_gs_value();
		int32	get_segment_value() const { return m_cur_segment_value; }

		e_element_war_role_state get_pk_state() { return m_role_state; }
		void	set_pk_state(e_element_war_role_state state) { m_role_state = state; }
		void	check_reset_pk_state();

		void	set_sign_up_time(int64 time) { m_sign_up_time = time; }
		int64	get_sign_up_time() const { return m_sign_up_time; }

		void	set_previous_opponent(guid_64 role_id) { m_previous_opponent = role_id; }
		guid_64 get_previous_oppenent() { return m_previous_opponent; }

		void	set_server_info(const xstring& out_ip, int32 out_port);
		const xstring&	get_server_out_ip() { return m_server_out_ip; }
		int32	get_server_out_port() { return m_server_out_port; }

		void	update_info_to_dp();
		void	refresh_season_info();

		std::vector<int32>	get_robot_array();	//获取机器人模板数组

		void	save_mission_info();
	
		void	buy_ticket();

		void	check_player_mission(e_element_war_mission_type mission_type, int32 add_num);

		void	game_end(bool is_win);
		void	send_last_season_score(int32 server_id = 0);

		void	set_is_send_tip(bool is_send) { m_is_send_tip = is_send; }
		bool	get_is_send_tip() { return m_is_send_tip; }
		void	send_math_tip();
	private:
		s_element_war_role_info		m_role_info;	//基本信息
		e_element_war_role_state	m_role_state;	//状态

		guid_64						m_team_guid;

		int64						m_sign_up_time;
		guid_64						m_previous_opponent;
		xstring						m_server_out_ip;
		int32						m_server_out_port;
		int32						m_cur_segment_value;
		bool						m_is_send_tip;
	};


	using element_war_sign_up_list = std::list<c_element_war_role_info*>;						//段位匹配列表
	using element_war_all_sign_up_map = std::unordered_map<int32, element_war_sign_up_list>;	//全部匹配列表

	class element_war_ws_mgr
	{
	public:
		static element_war_ws_mgr& get_instance()
		{
			static element_war_ws_mgr instance;
			return instance;
		}
		element_war_ws_mgr();
		~element_war_ws_mgr();

		void	clear_data();
		void	init_manager(bool need_load_dp = true);
		void	tick(int64 time_new);

		bool	is_in_game_time();
		bool	is_in_activity_time();

		bool	is_match_server();
		int32	get_match_server_id();

		void	init_activity_season_list(std::vector<std::string> time_list);
		int64	transfer_date_to_stamp(int64 date);
	public:
		/*****************************************************************/
		/*							通用功能						 */
		/*****************************************************************/
		//根据信息获取段位
		int32	get_segment_value(const c_element_war_role_info& info);
		//获取可购买次数
		int32	get_can_buy_num(int32 vip_level);
		//开启赛季
		void	begin_one_season(int32 cur_season_num);
		//结束赛季
		void	end_one_season();
		//刷新每日信息
		void	refresh_daily_ticket_info();
		//发送活动开启公告
		void	send_activity_begin_notice();
		//获取原区传送信息
		bool	get_transfer_game_info(const guid_64& role_guid, const int32& server_id, s_game_info& game_info);
		//Gm增加次数本服
		void	gm_send_add_ticket(guid_64 role_guid);
		//Gm增加次数Gate服
		void	gm_add_ticket(guid_64 role_guid);
	private:
		int32												m_config_match_server_id;				//匹配服务器Id
		int32												m_cur_season;							//当前赛季
		int32												m_send_notify_flag;						//活动开启公告
		int64												m_activity_timer;						//上次刷新时间
		int64												m_sec_timer;							//传送时间戳
		bool												m_refresh_flag;							//每日刷新
		std::vector<s_element_war_activity_time>			m_activity_time_vec;					//赛季时间列表
		std::unordered_map<int64, s_game_info>				m_transfer_info_map;					//传送列表
		std::unordered_map<int64, s_game_info>				m_role_server_info_map;					//角色原服务器传送信息

		/*****************************************************************/
		/*							匹配服ws功能						 */
		/*****************************************************************/
	public:
		//匹配函数
		void	match(int64 time_new);			
		//匹配成功
		void	match_success(std::vector<guid_64> first_team_list, std::vector<guid_64> secord_team_list);
		//匹配机器人
		bool	match_with_robot(c_element_war_role_info* play_role_info, std::vector<guid_64> first_team_list, std::vector<guid_64> secord_team_list);
		//计算匹配区间
		void	get_expend_range(const c_element_war_role_info& info, int64 time_new, int32 expend_time, int32& upper_limit, int32& lower_limit);
		//进行单人匹配
		e_element_war_sign_up_ret	single_sign_up(guid_64 role_guid);
		//进行多人人匹配
		e_element_war_sign_up_ret	team_sign_up(std::vector<guid_64> role_guid_list, guid_64 team_guid, guid_64& error_guid);
		//取消匹配
		void	sign_up_leave(guid_64 role_guid);
		//发送匹配失败信息
		void	send_sign_up_result(const guid_64& role_guid, e_element_war_sign_up_ret result_type, int32 server_id);
		//获取可匹配排名数组
		void	get_valid_role_list(const c_element_war_role_info& info, int64 time_new, std::vector<guid_64>& first_role_guid_array, std::vector<guid_64>& second_role_guid_array);
		//从匹配列表中清除玩家
		void	erase_match_by_list(std::vector<guid_64>& role_guid_array);
		//从数据库中读取玩家
		void	add_element_war_info(const s_element_war_role_info& info);
		//登陆同步玩家数据
		void	req_element_war_info(const guid_64& role_guid, const ws2ws_element_war_req_role_info* msg);
		//从数据库拉取玩家信息
		void	load_info_from_db();
		//设置是否加载完成
		void	set_is_load_all(bool is_load);
		//设置服务器信息
		void	set_role_server_info(const guid_64& role_guid, const xstring& server_ip, int32 server_port);
		//重置角色状态
		void	reset_role_state(const guid_64& role_guid);
		//发送角色信息到客户端
		void	send_element_war_info(c_element_war_role_info& info, bool is_online = false);
		//发送赛季奖励
		void	send_season_reward_to_player(const guid_64& role_guid);
		//发送段位提升奖励(新赛季开始时)
		void    send_season_upreward_to_player(const guid_64& role_guid);
		//获取每日任务
		std::vector<int32>	get_mission_list();
		//游戏结束结算
		void	map_game_end(const cs2ws_element_war_map_game_end_role_info* end_info);
		//检查是否可以领取任务奖励 
		void	check_can_get_mission_reward(const guid_64& role_guid, int32 mission_index);
		//发送排名奖励
		void	send_season_ranking_reward_to_player(const guid_64& role_guid, int32 server_id);
		//购买次数成功增加次数
		void	buy_ticket_num(guid_64 role_guid);
		//获取赛季信息
		void	req_element_ladder_last_score(const guid_64& role_guid, int32 server_id);
	private:
		std::unordered_map<ui64, c_element_war_role_info>		m_all_player_info;				//所有玩家列表
		element_war_all_sign_up_map								m_all_sign_up_map;				//匹配列表

		int32													m_config_match_time_limit;		//匹配时间限制
		int32													m_config_transfer_time;			//传送时间延迟
		bool													m_is_load_all;					//是否加载完角色数据

		int32													m_config_daily_ticket_num;		//每日次数
		int32													m_get_reward_need_num;			//领取奖励需要次数
		bool													m_is_in_activity_time;			//是否在活动时间
		/*****************************************************************/
		/*							比赛区ws功能						 */
		/*****************************************************************/
	public:
		//创建战斗地图
		void	deal_with_create_map_msg(s_element_war_match_info& match_info);
		//发送创建地图成功
		void	send_match_success_msg(guid_64 map_id, int32 war_index, const s_element_war_match_info& opponent_info);
		//创建机器人
		void	create_robot_init_map(guid_64 map_guid, s_element_war_match_info & match_info);

	private:
		std::unordered_map<int64, s_element_war_match_info>		m_war_map_list;					//战斗地图列表

		/*****************************************************************/
		/*							角色原区ws功能						 */
		/*****************************************************************/
	public:
		//玩家登陆处理
		void	session_online(client_session* session);
		//发送获取玩家信息
		bool	send_req_element_war_role_info(client_session* session, bool is_online = false);
		//发送赛季提示
		void	send_upgrade_notify(client_session* session);
		//检查是否可以进行匹配
		e_element_war_sign_up_ret	check_can_sign_up(client_session* session);
		//发送多人匹配准备
		void	send_team_ready_end(guid_64 team_guid);
		//多人匹配逻辑
		void	send_team_sign_up(client_session* session);
		//单人匹配逻辑
		void	send_single_sign_up(client_session* session);
		//取消匹配逻辑
		void	send_close_sign_up(client_session* session);
		//传送玩家
		void	transfer_player(int64 time_new);
		//战斗地图创建成功处理
		void	deal_with_match_success_msg(const guid_64& role_guid, const ws2ws_element_war_send_match_success* msg);
		//断线重连
		void	add_break_line_player(s_break_line_login_info break_info);
		//检查离线玩家
		void	check_break_line_player();
		//删除玩家从离线列表
		void	del_break_line_player(guid_64 role_guid);
		//发送断线重连消息			
		void	send_break_line_msg(guid_64 role_guid, int32 map_template_id);
		//进行重连
		void	break_line_login_msg(guid_64 role_guid, bool is_login);
		//获取重连信息
		s_break_line_login_info		get_player_break_login_info(guid_64 role_guid);
		//比赛结果的本服处理
		void	deal_with_pk_result_msg(const guid_64& role_guid, const ws2ws_element_war_sync_game_result* msg);
		//获取任务奖励
		void	get_mission_reward(guid_64 role_guid, int32 mission_idex);
	private:
		std::list<s_element_war_transfer_info>					m_transfer_list;				//延时传送列表
		std::unordered_map<int64, int32>						m_notify_map;					//段位提升提示
		int32													m_config_level_limit;			//活动限制等级
		std::vector<s_break_line_login_info>					m_break_line_login_list;		//断线重连列表

					
	};
}
#endif