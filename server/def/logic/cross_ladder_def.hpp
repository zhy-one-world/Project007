/**********************************************
 * created	 : 2019年5月23日
 * file base : cross_ladder_define_
 * file ext  : hpp
 * author    : pengbiao
 * purpose   : 跨服天梯
 *********************************************/

#ifndef _CROSS_LADDER_DEFINE_HPP_
#define _CROSS_LADDER_DEFINE_HPP_

#include "char_def.hpp"

namespace faith
{
#pragma pack(push,1)

	const int32 CROSS_LADDER_MAP_ID					= 71211108;	//比赛地图id
	const int32 CROSS_LADDER_NEED_USER_NUMBER		= 2;		//单局比赛所需人数
	const int32 CROSS_LADDER_MAX_TURN_NUM			= 3;		//单局比赛最大轮数
	const int32	CROSS_LADDER_WIN_NEED_SCORE			= 2;		//单局胜利所需积分
	const int32 CROSS_LADDER_SEND_INFO_OFFSET		= 4;		//发送数据偏移量（wsRoleinfo无需全部发送）
	const int32 CROSS_LADDER_MAX_LOAD_DB_NUM		= 500;		//单次数据库最大加载数量

	enum e_cross_ladder_role_info
	{
		e_cross_ladder_role_info_template_id,			//职业
		e_cross_ladder_role_info_role_gs,				//战力
		e_cross_ladder_role_info_vip_level,				//vip等级
		e_cross_ladder_role_info_server_id,				//服务器id
		e_cross_ladder_role_info_ladder_score,			//天梯积分
		e_cross_ladder_role_info_total_join_num,		//总次数
		e_cross_ladder_role_info_total_win_num,			//胜利次数
		e_cross_ladder_role_info_last_win_num,			//连胜次数
		e_cross_ladder_role_info_ticket_num,			//剩余次数
		e_cross_ladder_role_info_can_buy_num,			//可购买次数
		e_cross_ladder_role_info_history_score,			//历史最高积分
		e_cross_ladder_role_info_last_score,			//上赛季积分
		e_cross_ladder_role_info_season_reward,			//上赛季奖励标识
		e_cross_ladder_role_info_cur_season_join_num,		//本赛季参与次数
		e_cross_ladder_role_info_last_season_join_num,		//上赛季参与次数
		e_cross_ladder_role_info_max,
	};

	enum e_cross_ladder_send_info
	{
		e_cross_ladder_send_role_info_ladder_score,			//天梯积分
		e_cross_ladder_send_role_info_total_join_num,		//总次数
		e_cross_ladder_send_role_info_total_win_num,		//胜利次数
		e_cross_ladder_send_role_info_last_win_num,			//连胜次数
		e_cross_ladder_send_role_info_ticket_num,			//剩余次数
		e_cross_ladder_send_role_info_can_buy_num,			//已购买参与次数
		e_cross_ladder_send_role_info_history_score,		//历史最高积分
		e_cross_ladder_send_role_info_last_score,			//上赛季积分
		e_cross_ladder_send_info_max,
	};

	enum e_cross_ladder_sign_up_ret
	{
		e_cross_ladder_sign_up_success,					//报名成功
		e_cross_ladder_sign_up_leave_success,			//取消报名成功
		e_cross_ladder_sign_up_failed,					//报名失败
		e_cross_ladder_sign_up_no_ticket,				//次数不足
		e_cross_ladder_sign_up_level_limit,				//等级不足
		e_cross_ladder_sign_up_not_open,				//活动未开放
		e_cross_ladder_sign_up_not_in_time,				//不在比赛时间内
		e_cross_ladder_sign_up_cross_server_fail,		//跨区服务器连接失败
		e_cross_ladder_sign_up_already,					//已经报名
		e_cross_ladder_sign_up_time_out,				//匹配超时
		e_cross_ladder_sign_up_map_limit,				//地图上限
	};

	enum e_cross_ladder_buy_ticket_ret
	{
		e_cross_ladder_buy_ticket_success,				//购买成功
		e_cross_ladder_buy_ticket_failed,				//购买失败
		e_cross_ladder_buy_ticket_jewl,					//钻石不足
		e_cross_ladder_buy_ticket_limit,				//购买次数限制
		e_cross_ladder_buy_ticket_not_open,				//活动未开放
	};

	enum e_cross_ladder_game_state
	{
		e_cross_ladder_game_state_wait,
		e_cross_ladder_game_state_prepare,
		e_cross_ladder_game_state_game,
		e_cross_ladder_game_state_one_turn_end,
		e_cross_ladder_game_state_end,
	};

	enum e_cross_ladder_role_state
	{
		e_cross_ladder_role_no_sign_up,					//未报名
		e_cross_ladder_role_sign_up,					//报名成功
		e_cross_ladder_role_join_war,					//比赛
	};

	enum e_cross_ladder_mission_type
	{
		e_cross_ladder_mission_join_num,				//每日参与要求
		e_cross_ladder_mission_win_num,					//每日胜场要求
	};

	enum e_cross_ladder_reward_type
	{
		e_cross_ladder_reward_no_use,					//未使用
		e_cross_ladder_reward_first_money,				//货币奖励
		e_cross_ladder_reward_second_money,				//货币奖励
		e_cross_ladder_reward_object					//道具奖励
	};

	enum e_cross_ladder_config_ParamIntArr2
	{
		e_cross_ladder_config_match_time_limit,			//匹配时间上限(s)
		e_cross_ladder_config_last_win_limit,			//连胜上限
		e_cross_ladder_config_last_win_rate,			//连胜增加积分比例 百分之
		e_cross_ladder_config_transfer_time,			//原区传送延迟(s)
		e_cross_ladder_config_max,						//匹配时间上限
	};

	enum e_cross_ladder_config_ParamIntArr1
	{
		e_cross_ladder_map_wait_last,					//地图配置—等待时间
		e_cross_ladder_map_prepare_last,				//地图配置—准备时间
		e_cross_ladder_map_game_last,					//地图配置—比赛时间
		e_cross_ladder_map_end_last,					//地图配置—小局结束时间
		e_cross_ladder_map_max,
	};

	struct s_cross_ladder_role_info
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int64		gs_value;
		int32		data_array[e_cross_ladder_role_info_max];
		s_cross_ladder_role_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_cross_ladder_map_role_info
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		s_map_pos	role_born_pos;
		int32		score;
		int32		template_id;
		s_cross_ladder_map_role_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_cross_ladder_map_role_info));
		}
	};

	struct s_cross_ladder_activity_time
	{
		int32 season;
		int64 begin_time;
		int64 end_time;
		s_cross_ladder_activity_time()
		{
			memset(this, 0, sizeof(s_cross_ladder_activity_time));
		}
	};

	struct s_cross_ladder_transfer_info
	{
		guid_64		m_role_guid;
		int32		m_server_id;
		guid_64		m_map_guid;
		int32		m_war_index;
		int64		m_transfer_time;
		s_cross_ladder_transfer_info()
		{
			memset(this, 0, sizeof(s_cross_ladder_transfer_info));
		}
		s_cross_ladder_transfer_info(guid_64 role_guid, int32 server_id, guid_64 map_guid, int32 war_index, int64 transfer_time)
		{
			m_role_guid = role_guid;
			m_server_id = server_id;
			m_map_guid = map_guid;
			m_war_index = war_index;
			m_transfer_time = transfer_time;
		}
	};

	struct s_cross_ladder_match_role_info
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int32		server_id;
		int32		template_id;
		int32		ladder_score;
		int32		total_join_num;
		int32		total_win_num;
		int32		last_win_num;
		xchar		role_server_out_ip[max_ip_address_length + 1];
		int32		role_server_out_port;
		s_cross_ladder_match_role_info()
		{
			memset(this, 0, sizeof(s_cross_ladder_match_role_info));
		}
	};

	struct s_cross_ladder_match_info
	{
		int32 robot_template_id;
		s_cross_ladder_match_role_info role_info[CROSS_LADDER_NEED_USER_NUMBER];
		s_cross_ladder_match_info()
		{
			memset(this, 0, sizeof(s_cross_ladder_match_info));
		}
	};

	struct s_cross_ladder_season_reward_info
	{
		guid_64		m_role_guid;
		int32		m_server_id;
		int32		m_ladder_score;
		s_cross_ladder_season_reward_info()
		{
			memset(this, 0, sizeof(s_cross_ladder_season_reward_info));
		}
		s_cross_ladder_season_reward_info(guid_64 role_guid, int32 server_id, int32 ladder_score)
		{
			m_role_guid = role_guid;
			m_server_id = server_id;
			m_ladder_score = ladder_score;
		}
	};

#pragma pack(pop)
}

#endif
