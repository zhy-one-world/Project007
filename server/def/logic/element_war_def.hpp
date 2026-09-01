/**********************************************
* created	: 2020年8月18日
* file base : element_war_def
* file ext  : hpp
* author    : 赵玉明
* purpose   : 元素争霸
*********************************************/

#ifndef _ELEMENT_WAR_DEF_HPP_
#define _ELEMENT_WAR_DEF_HPP_

#include "char_def.hpp"
#include "pk_king_def.hpp"
namespace faith
{
#pragma pack(push,1)
	const int32 ELEMENT_WAR_MAP_ID					= 71220601;	//比赛地图id
	const int32 ELEMENT_WAR_PLAY_NUM				= 3;		//队伍最大玩家数量
	const int32 ELEMENT_WAR_MATCH_NUM				= 6;		//匹配组最大玩家数量
	const int32 ELEMENT_WAR_MAX_LOAD_DB_NUM			= 500;		//单次数据库最大加载数量
	const int32	ELEMENT_WAR_BUFF_POS_NUM			= 3;		//团队buff刷新点		
	const int32	ELEMENT_WAR_MAP_TEAM_NUM			= 2;		//队伍数量	
	const int32 ELEMENT_WAR_MAP_BEGIN_TIME			= 20;		//准备时间
	const int32	ELEMENT_WAR_MAP_SEND_TIME			= 3;		//发送倒计时时间
	const int32 ElEMENT_WAR_BOSS_DEAD_TIME			= 5;		//boss死亡延时 
	const int32 ElEMENT_WAR_MATH_TIME				= 5;		//匹配机器人时间
	const int32 ElEMENT_WAR_SHOW_TIP_TIME			= 10;		//弹提示时间


	enum e_element_war_role_info
	{
		e_element_war_role_info_template_id,				//职业
		e_element_war_role_info_vip_level,					//vip等级
		e_element_war_role_info_role_level,					//角色等级
		e_element_war_role_info_server_id,					//服务器id
		e_element_war_role_info_score_num,					//积分
		e_element_war_role_info_cur_season,					//当前赛季
		e_element_war_role_info_last_season,				//上次登陆赛季
		e_element_war_role_info_total_join_num,				//总次数
		e_element_war_role_info_total_win_num,				//胜利次数
		e_element_war_role_info_last_win_num,				//连胜次数
		e_element_war_role_info_ticket_num,					//剩余次数
		e_element_war_role_info_can_buy_num,				//可购买次数
		e_element_war_role_info_have_buy_num,				//已购买次数
		e_element_war_role_info_history_score,				//历史最高积分
		e_element_war_role_info_cur_season_join_num,		//本赛季参与次数
		e_element_war_role_info_last_score,					//上赛季积分
		e_element_war_role_info_season_reward,				//上赛季奖励标识
		e_element_war_role_info_last_season_join_num,		//上赛季参与次数
		e_element_war_role_info_mission1,					//每日任务1id
		e_element_war_role_info_mission2,					//每日任务2id
		e_element_war_role_info_mission3,					//每日任务3id
		e_element_war_role_info_mission4,					//每日任务4id
		e_element_war_role_info_mission1_have_num,			//每日任务1完成次数
		e_element_war_role_info_mission2_have_num,			//每日任务2完成次数
		e_element_war_role_info_mission3_have_num,			//每日任务3完成次数
		e_element_war_role_info_mission4_have_num,			//每日任务4完成次数
		e_element_war_role_info_max,
	};

	enum e_element_war_role_state
	{
		e_element_war_role_no_sign_up,					//未报名
		e_element_war_role_sign_up,						//报名成功
		e_element_war_role_join_war,					//比赛
	};

	enum e_element_war_sign_up_ret
	{
		e_element_war_sign_up_success,					//报名成功
		e_element_war_sign_up_leave_success,			//取消报名成功
		e_element_war_sign_up_failed,					//报名失败
		e_element_war_sign_up_no_ticket,				//次数不足
		e_element_war_sign_up_level_limit,				//等级不足
		e_element_war_sign_up_not_open,					//活动未开放
		e_element_war_sign_up_not_in_time,				//不在比赛时间内
		e_element_war_sign_up_cross_server_fail,		//跨区服务器连接失败
		e_element_war_sign_up_already,					//已经报名
		e_element_war_sign_up_time_out,					//匹配超时
		e_element_war_sign_up_map_limit,				//地图上限
		e_element_war_sign_up_team_max_num,				//组队人数过多
		e_element_war_sign_up_is_not_captation,			//不是队长不能发起匹配
		e_element_war_sign_up_is_not_line,				//队员处于离线状态
	};

	enum e_element_war_config_type
	{
		e_element_war_config_match_time_limit,			//匹配时间上限(s)
		e_element_war_config_transfer_time,				//原区传送延迟(s)
		e_element_war_config_max,
	};


	enum e_element_war_buy_limit_type
	{
		e_element_war_buy_limit_type_region,			//地区	
		e_element_war_buy_limit_type_num,				//可购买次数
		e_element_war_buy_limit_type_money_type,		//购买货币类型
		e_element_war_buy_limit_type_money_num,			//基础价格
		e_element_war_buy_limit_type_add_money,			//价格提示系数
		e_element_war_buy_limit_type_max,
	};

	enum e_element_war_map_state
	{
		e_element_war_map_state_begin,		//准备阶段
		e_element_war_map_state_first,		//第一阶段
		e_element_war_map_state_secord,		//第二阶段
		e_element_war_map_state_end,		//最终阶段
		e_element_war_map_state_max,
	};

	enum e_element_war_map_role_info
	{
		e_element_war_map_role_info_server_id ,			//服务器id	
		e_element_war_map_role_info_element_score,		//参赛前天梯积分
		e_element_war_map_role_info_score_num,			//活动势力值
		e_element_war_map_role_info_kill_player_num,	//击杀玩家数量
		e_element_war_map_role_info_kill_monster_num,	//击杀普通怪物数量
		e_element_war_map_role_info_kill_elite_num,		//击杀精英怪数量
		e_element_war_map_role_info_kill_boss_num,		//击杀boss数量
		e_element_war_map_role_info_team_index,			//队伍索引
		e_element_war_map_role_info_end_add_score,		//实际增加积分数
		e_element_war_map_role_info_max,
	};

	enum e_element_war_mission_type
	{
		e_element_war_mission_type_join_map_num,			//参与次数
		e_element_war_mission_type_win_num,					//获胜次数
		e_element_war_mission_type_kill_monster_num,		//击杀小怪次数
		e_element_war_mission_type_kill_player_num,			//击杀玩家次数
		e_element_war_mission_type_kill_elite_num,			//击杀精英次数
		e_element_war_mission_type_kill_boss_num,			//击杀boss次数
		e_element_war_mission_type_kill_first_score_num,	//获取积分第一次数
	};

	enum e_element_war_add_score
	{
		e_element_war_add_score_monster,					//击杀怪物积分
		e_element_war_add_score_elite,						//击杀精英怪积分
		e_element_war_add_score_boss,						//击杀boss积分
		e_element_war_add_score_player,						//击杀玩家积分
		e_element_war_add_score_buff,						//拥有buff积分
		e_element_war_add_score_max,
	};

	enum e_element_war_map_add_score
	{
		e_element_war_map_add_score1,						//排名1积分
		e_element_war_map_add_score2,						//排名2积分
		e_element_war_map_add_score3,						//排名3积分
		e_element_war_map_add_score4,						//排名4积分
		e_element_war_map_add_score5,						//排名5积分
		e_element_war_map_add_score6,						//排名6积分
		e_element_war_map_add_score_win,					//获胜积分
		e_element_war_map_add_score_los,					//失败积分
		e_element_war_map_add_score_kill_boss,				//击杀boss积分
		e_element_war_map_add_score_is_escape,				//逃跑扣除分数
		e_element_war_map_add_score_max,
	};

	enum e_element_war_buff_drop_pos
	{
		e_element_war_buff_drop_pos_id,						//安全区id
		e_element_war_buff_drop_pos_min_x,					//最小坐标x
		e_element_war_buff_drop_pos_min_y,					//最小坐标y
		e_element_war_buff_drop_pos_min_z,					//最小坐标z
		e_element_war_buff_drop_pos_max_x,					//最大坐标x
		e_element_war_buff_drop_pos_max_y,					//最大坐标y
		e_element_war_buff_drop_pos_max_z,					//最大坐标z
		e_element_war_buff_drop_pos_max,
	};

	enum e_element_war_get_mission_reward
	{
		e_element_war_get_mission_reward_succeed,			//领取成功
		e_element_war_get_mission_reward_not_info,			//信息错误
		e_element_war_get_mission_reward_not_num,			//未完成任务
		e_element_war_get_mission_reward_have_reward,		//已领取过奖励
	};

	enum e_element_war_buy_ticket_ret
	{
		e_element_war_buy_ticket_success,				//购买成功
		e_element_war_buy_ticket_failed,				//购买失败
		e_element_war_buy_ticket_jewl,					//钻石不足
		e_element_war_buy_ticket_limit,					//购买次数限制
		e_element_war_buy_ticket_not_open,				//活动未开放
	};

	struct s_element_war_buff_drop_pos_info
	{
		int32		info[e_element_war_map_add_score_max];
		s_element_war_buff_drop_pos_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_element_war_role_info
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int64		gs_value;
		int32		data_array[e_element_war_role_info_max];
		s_element_war_role_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_element_war_activity_time
	{
		int32 season;
		int64 begin_time;
		int64 end_time;
		s_element_war_activity_time()
		{
			memset(this, 0, sizeof(s_element_war_activity_time));
		}
	};

	struct s_element_war_match_role_info
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int32		server_id;
		int32		template_id;
		int32		role_level;
		int32		score_num;
		int32		total_join_num;
		int32		total_win_num;
		int32		last_win_num;
		int32		vip_level;
		xchar		role_server_out_ip[max_ip_address_length + 1];
		int32		role_server_out_port;
		bool		is_robor;
		int32		robor_template_id;
		s_element_war_match_role_info()
		{
			memset(this, 0, sizeof(s_element_war_match_role_info));
		}
		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
		void set_server_name(xstring _role_name)
		{
			memset(server_name, 0, sizeof(server_name));
			memcpy(server_name, _role_name.c_str(), _role_name.size() > sizeof(server_name) ? sizeof(server_name) : _role_name.size());
		}
		bool to_proto(faith::st_proto::st_element_war_match_role_info* st_role_ptr)
		{
			if (st_role_ptr == nullptr)
			{
				return false;
			}
			st_role_ptr->set_role_guid(role_guid.server_64);
			st_role_ptr->set_role_name(role_name);
			st_role_ptr->set_server_name(server_name);
			st_role_ptr->set_server_id(server_id);
			st_role_ptr->set_template_id(template_id);
			st_role_ptr->set_role_level(role_level);
			st_role_ptr->set_score_num(score_num);
			st_role_ptr->set_total_join_num(total_join_num);
			st_role_ptr->set_total_win_num(total_win_num);
			st_role_ptr->set_last_win_num(last_win_num);
			st_role_ptr->set_vip_level(vip_level);
			st_role_ptr->set_role_server_out_ip(role_server_out_ip);
			st_role_ptr->set_role_server_out_port(role_server_out_port);
			st_role_ptr->set_is_robor(is_robor);
			st_role_ptr->set_robor_template_id(robor_template_id);
			return true;
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::st_proto::st_element_war_match_role_info& st_role_ref)
		{

			role_guid.server_64 = st_role_ref.role_guid();
			my_memcopy_string(role_name, max_name_size,st_role_ref.role_name());
			my_memcopy_string(server_name,max_name_size,st_role_ref.server_name());
			server_id = st_role_ref.server_id();
			template_id = st_role_ref.template_id();
			role_level = st_role_ref.role_level();
			score_num = st_role_ref.score_num();
			total_join_num = st_role_ref.total_join_num();
			total_win_num = st_role_ref.total_win_num();
			last_win_num = st_role_ref.last_win_num();
			vip_level = st_role_ref.vip_level();
			my_memcopy_string(role_server_out_ip , max_ip_address_length,  st_role_ref.role_server_out_ip());
			role_server_out_port = st_role_ref.role_server_out_port();
			is_robor = st_role_ref.is_robor();
			robor_template_id = st_role_ref.robor_template_id();
		}

	};

	struct s_element_war_match_info
	{
		s_element_war_match_role_info first_role_info[ELEMENT_WAR_PLAY_NUM];
		s_element_war_match_role_info second_role_info[ELEMENT_WAR_PLAY_NUM];
		int32	map_level;
		s_element_war_match_info()
		{
			memset(this, 0, sizeof(s_element_war_match_info));
		}
		bool to_proto(faith::st_proto::st_element_war_match_info* st_match_ptr)
		{
			if (st_match_ptr == nullptr)
			{
				return false;
			}
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; i++)
			{
				faith::st_proto::st_element_war_match_role_info* st_role_ptr = st_match_ptr->add_first_role_info();
				if (st_role_ptr == nullptr)
				{
					return false;
				}
				first_role_info[i].to_proto(st_role_ptr);
			}
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; i++)
			{
				faith::st_proto::st_element_war_match_role_info* st_role_ptr = st_match_ptr->add_first_role_info();
				if (st_role_ptr == nullptr)
				{
					return false;
				}
				second_role_info[i].to_proto(st_role_ptr);
			}
			st_match_ptr->set_map_level(map_level);
			return true;
		}
	    void from_proto(const faith::st_proto::st_element_war_match_info& st_match_ref)
		{
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM && i < st_match_ref.first_role_info_size() ; i++)
			{
				first_role_info[i].from_proto(st_match_ref.first_role_info(i));
			}
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM && i < st_match_ref.second_role_info_size(); i++)
			{
				second_role_info[i].from_proto(st_match_ref.second_role_info(i));
			}
			map_level = st_match_ref.map_level();
		}
	};

	struct s_element_war_match_success_role_info
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		int32		server_id;
		int32		template_id;
		int32		score_num;
		int32		total_join_num;
		int32		total_win_num;
		int32		last_win_num;
		bool		is_robor;
		int32		robor_template_id;
		int32		vip_level;
		s_game_info	target_game_info;
		s_element_war_match_success_role_info()
		{
			memset(this, 0, sizeof(s_element_war_match_success_role_info));
		}
	};


	struct s_element_war_match_success_info
	{
		s_element_war_match_success_role_info first_role_info[ELEMENT_WAR_PLAY_NUM];
		s_element_war_match_success_role_info second_role_info[ELEMENT_WAR_PLAY_NUM];
		s_element_war_match_success_info()
		{
			memset(this, 0, sizeof(s_element_war_match_success_info));
		}
	};

	struct s_element_war_transfer_info
	{
		guid_64		m_role_guid;
		int32		m_server_id;
		guid_64		m_map_guid;
		int32		m_war_index;
		int64		m_transfer_time;
		s_element_war_transfer_info()
		{
			memset(this, 0, sizeof(s_element_war_transfer_info));
		}
		s_element_war_transfer_info(guid_64 role_guid, int32 server_id, guid_64 map_guid, int32 war_index, int64 transfer_time)
		{
			m_role_guid = role_guid;
			m_server_id = server_id;
			m_map_guid = map_guid;
			m_war_index = war_index;
			m_transfer_time = transfer_time;
		}
	};

	struct s_element_war_map_team_info
	{
		guid_64		role_guid_list[ELEMENT_WAR_PLAY_NUM];			//角色列表
		guid_64		boss_guid;										//bossGuid
		int64		last_send_boss_be_attack;						//上次发送boss被攻击时间
		int32		score_num;										//总分数
		int32		kill_unit_num;									//击杀怪物数量
		bool		is_create_buff;									//是否创建过buff
		s_map_pos	bron_pos;										//复活点
		s_element_war_map_team_info()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_in_team(guid_64 role_guid)
		{
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; i++)
			{
				if (role_guid == role_guid_list[i])
				{
					return true;
				}
			}
			return false;
		}
		bool is_team_boss(guid_64 role_guid)
		{
			return (role_guid == boss_guid);
		}
		void add_score(int32 num)
		{
			score_num += num;
		}
		bool check_reach_score_num(int32 num)
		{
			return score_num >= num;
		}
	};

	struct s_element_war_map_role_info
	{		
		guid_64		role_guid;										//角色guid
		xchar		role_name[max_name_size + 1];
		xchar		server_name[max_name_size + 1];
		bool		is_robot;
		bool		is_win;
		bool		is_escape;
		bool		is_in_safe_map;
		bool		is_kill_boss;
		int64		send_escape_time;
		int32		role_info[e_element_war_map_role_info_max];
		bool		is_first_enter;
		s_element_war_map_role_info()
		{
			memset(this, 0, sizeof(*this));
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_valid()
		{
			return role_guid.is_valid();
		}
		void set_role_name(const xstring play_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, play_name.c_str(), play_name.size() > sizeof(role_name) ? sizeof(role_name) : play_name.size());
		}
		void set_server_name(const xstring _server_name)
		{
			memset(server_name, 0, sizeof(server_name));
			memcpy(server_name, _server_name.c_str(), _server_name.size() > sizeof(server_name) ? sizeof(server_name) : _server_name.size());
		}
		int32 get_team_index()
		{
			return role_info[e_element_war_map_role_info_team_index];
		}
		void add_score(int32 num)
		{
			role_info[e_element_war_map_role_info_score_num] += num;
		}
		bool get_is_robot()
		{
			return is_robot;
		}
		bool operator < (const s_element_war_map_role_info& other_info) const
		{
			return role_info[e_element_war_map_role_info_score_num] < other_info.role_info[e_element_war_map_role_info_score_num];
		}

		bool operator > (const s_element_war_map_role_info& other_info) const
		{
			return role_info[e_element_war_map_role_info_score_num] > other_info.role_info[e_element_war_map_role_info_score_num];
		}
	};

	struct s_element_war_map_create_buff
	{
		int32	buff_id;
		int32	npc_id;
		int32	add_score_num;
		guid_64	role_guid;
		xchar	role_name[max_name_size + 1];
		s_element_war_map_create_buff()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_role_name(const xstring play_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, play_name.c_str(), play_name.size() > sizeof(role_name) ? sizeof(role_name) : play_name.size());
		}
		void clear_name()
		{
			memset(role_name, 0, sizeof(role_name));
		}
	};

#pragma pack(pop)
}

#endif
