/********************************************************************
created: 2021年9月24日
file base: attack_city_def
file ext: hpp
author: zhaoyuming

purpose:
*********************************************************************/
#ifndef _ATTACK_CITY_DEF_HPP_
#define _ATTACK_CITY_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/legion_def.hpp"
#include "Logic/char_def.hpp"

namespace faith
{
#pragma pack(push,1)
	
	const int32	attack_city_group_max_num						= 4;	// 攻城战每组军团数
	const int32 attack_city_save_dp_max_num						= 30;	// 军团信息存储最大数量	
	const int32 attack_city_rank_save_dp_max_num				= 100;	// 排名信息存储最大数量	
	
	// 攻城战分级等级
	enum e_attack_city_group_level
	{
		e_attack_city_group_level_s,					// S级
		e_attack_city_group_level_a,					// A级
		e_attack_city_group_level_b,					// B级
		e_attack_city_group_level_c,					// C级
		e_attack_city_group_level_max,
	};

	// 攻城战复活点类型
	enum e_attack_city_born_pos_type
	{
		e_attack_city_born_pos_type_attack1,			// 进攻方复活点1
		e_attack_city_born_pos_type_attack2,			// 进攻方复活点2
		e_attack_city_born_pos_type_attack3,			// 进攻方复活点3
		e_attack_city_born_pos_type_attack4,			// 进攻方复活点4
		e_attack_city_born_pos_type_guard,				// 防守方复活点
		e_attack_city_born_pos_type_max,
	};

	// 攻城战配置参数
	enum e_attack_city_data
	{
		e_attack_city_data_map_id,						// 攻城战地图id
		// 时间部分配置
		e_attack_city_data_check_time,					// 军团检查时间(活动正式开启前多少秒)
		e_attack_city_data_group_time,					// 分组时间(活动正式开启前多少秒)
		e_attack_city_data_ready_time,					// 准备持续时间
		e_attack_city_data_ready_end_time,				// 准备阶段倒计时时间(最后几秒开始显示倒计时)
		e_attack_city_data_phase_swithch_time,			// 阶段切换时间
		e_attack_city_data_phase_swithch_transfer_time,	// 轮换阶段传送时间(最后几秒开始传送)
		e_attack_city_data_round_time,					// 每轮持续时间
		e_attack_city_data_repair_statue_need_time,		// 修复雕像所需时间
		e_attack_city_data_transfer_time,				// 传送时间
		// 积分获取配置
		e_attack_city_data_kill_player_source,			// 击杀玩家获得分数
		e_attack_city_data_kill_statue_source,			// 摧毁雕像获得分数
		e_attack_city_data_kill_gate_source,			// 摧毁门获得分数
		e_attack_city_data_kill_boss_source,			// 摧毁boss获得分数
		e_attack_city_data_help_kill_statue_source,		// 帮助摧毁雕像获得分数
		e_attack_city_data_help_kill_gate_source,		// 帮助摧毁门获得分数
		e_attack_city_data_help_kill_boss_source,		// 帮助摧毁boss获得分数
		e_attack_city_data_repair_statue_num,			// 修复雕像获得分数
		// buff相关配置
		e_attack_city_data_player_invincible_buff_id,	// 玩家传送无敌buffid
		e_attack_city_data_invincible_buff_id,			// NPC无敌BuffId
		e_attack_city_data_end_invincible_buff_id,		// 最后大门NPC无敌BuffId
		e_attack_city_data_gate_buff_id,				// 每个城门增加的减伤buffid
		e_attack_city_data_more_win_buff_id,			// 对战连胜军团时增加的buffid
		e_attack_city_data_ready_buff_id,				// 准备时间获得的buffId
		// 复活点配置
		e_attack_city_data_random_pos_x,				// 复活点x轴偏移系数
		e_attack_city_data_random_pos_y,				// 复活点y轴偏移系数
		e_attack_city_data_random_pos_z,				// 复活点z轴偏移系数
		e_attack_city_data_born_npc_id,					// 复活NPCId
		// NPC刷新配置
		e_attack_city_data_rank_wave_first_id,			// 排名阶段NPC刷新表首Id
		e_attack_city_data_attack_wave_first_id,		// 攻城阶段NPC刷新表首Id
		e_attack_city_data_more_win_num,				// 连胜开始计算次数
		e_attack_city_data_change_bush_id,				// 变身buffId
		e_attack_city_data_no_change_bush_id,			// 解除变身buffid
		e_attack_city_data_static_wave_id,				// 静态NPC刷新ID
		e_attack_city_data_max,
	};

	// 攻城战配置参数
	enum e_attack_city_map_event
	{
		e_attack_city_map_event_clear,					// 全部复原
		e_attack_city_map_event_clear_wall,				// 隐藏出生点空气墙
		e_attack_city_map_event_clear_guide_post,		// 清除路标
		e_attack_city_map_event_max,
	};


	// 攻城战配置参数
	enum e_attack_city_map_time
	{
		e_attack_city_map_begin,						// 开始阶段
		e_attack_city_map_ready_time,					// 准备阶段
		e_attack_city_map_rank_time,					// 排名阶段
		e_attack_city_map_phase_swithch_time,			// 切换轮次阶段
		e_attack_city_map_round_time,					// 轮次阶段
		e_attack_city_map_end_time,						// 结束阶段
	};

	// 排名参数枚举
	enum e_attack_city_rank_info
	{
		e_attack_city_rank_info_rank_index,				// 排名索引
		e_attack_city_rank_info_server_id,				// 服务器id
		e_attack_city_rank_info_class_type,				// 角色职业
		e_attack_city_rank_info_role_source,			// 个人分数
		e_attack_city_rank_info_kill_player_num,		// 击杀玩家数
		e_attack_city_rank_info_kill_statue_num,		// 摧毁雕像数
		e_attack_city_rank_info_kill_gate_num,			// 摧毁门数
		e_attack_city_rank_info_repair_statue_num,		// 修复雕像数
		e_attack_city_rank_info_kill_boss_num,			// 击杀boss数
		e_attack_city_rank_info_max,
	};

	// 地图Npc枚举
	enum e_attack_city_npc_type
	{
		e_attack_city_npc_type_left_outer_statue,		// 左外侧雕像
		e_attack_city_npc_type_right_outer_statue,		// 右外侧雕像
		e_attack_city_npc_type_left_within_statue,		// 左侧内测雕像
		e_attack_city_npc_type_right_within_statue,		// 右侧内测雕像
		e_attack_city_npc_type_centre_statue,			// 中间雕像
		e_attack_city_npc_type_left_outer_gate,			// 左外侧城门
		e_attack_city_npc_type_right_outer_gate,		// 右外侧城门
		e_attack_city_npc_type_left_within_gate,		// 左侧内测城门
		e_attack_city_npc_type_right_within_gate,		// 右侧内测城门
		e_attack_city_npc_type_centre_gate,				// 中间城门
		e_attack_city_npc_type_boss,					// 最终boss
		e_attack_city_npc_type_attack_npc,				// 攻击方变身NPC1
		e_attack_city_npc_type_attack_npc2,				// 攻击方变身NPC2
		e_attack_city_npc_type_guard_npc,				// 防守方变身NPC
		e_attack_city_npc_type_max,
	};

	// 玩家操作
	enum e_attack_city_operate
	{
		e_attack_city_operate_interaction,				// 发送交互
		e_attack_city_operate_begin_interaction,		// 开始交互
		e_attack_city_operate_result_interaction,		// 交互成功
		e_attack_city_operate_result_break,				// 中断交互
		e_attack_city_operate_result_morph,				// 变身
		e_attack_city_operate_transfer1 = 6,			// 左1
		e_attack_city_operate_transfer2,				// 左2
		e_attack_city_operate_transfer3,				// 左3
		e_attack_city_operate_transfer4,				// 右1	
		e_attack_city_operate_transfer5,				// 右2
		e_attack_city_operate_transfer6,				// 左3
		e_attack_city_operate_end_transfer				// 离开传送点

	};

	// 玩家操作
	enum e_attack_city_transfer_pos
	{
		e_attack_city_transfer_pos_left_begin1,
		e_attack_city_transfer_pos_left_begin2,
		e_attack_city_transfer_pos_left_begin3,
		e_attack_city_transfer_pos_right_begin1,
		e_attack_city_transfer_pos_right_begin2,
		e_attack_city_transfer_pos_right_begin3,
		e_attack_city_transfer_pos_left_end1,
		e_attack_city_transfer_pos_left_end2,
		e_attack_city_transfer_pos_left_end3,
		e_attack_city_transfer_pos_right_end1,
		e_attack_city_transfer_pos_right_end2,
		e_attack_city_transfer_pos_right_end3,
		e_attack_city_transfer_pos_max,
	};

	// 玩家操作
	enum e_attack_city_achievement_type
	{
		e_attack_city_achievement_type_kill_1,
		e_attack_city_achievement_type_kill_2,
		e_attack_city_achievement_type_kill_3,
		e_attack_city_achievement_type_kill_statue,
		e_attack_city_achievement_type_kill_gate,
		e_attack_city_achievement_type_repair_statue,
		e_attack_city_achievement_type_max,
	};

	// 玩家操作
	enum e_attack_transfer_info_type
	{
		e_attack_transfer_info_type_begin,
		e_attack_transfer_info_type_error,
		e_attack_transfer_info_type_succeed,
		e_attack_transfer_info_type_end,
	};

	// 玩家操作
	enum e_attack_check_type
	{
		e_attack_check_type_no_check,		// 未检查
		e_attack_check_type_check,			// 已检查
		e_attack_check_type_join_end,		// 已结束
	};

	// npc交互逻辑
	struct s_npc_interaction_time
	{
		guid_64	role_guid;
		guid_64	prop_npc_guid;
		float	left_time;

		s_npc_interaction_time()
		{
			role_guid.clear_data();
			prop_npc_guid.clear_data();
			left_time = 0;
		}
	};

	// 传送信息数组
	struct s_attack_transfer_info
	{
		guid_64		role_guid;		// 角色id
		s_map_pos	transfer_pos;	// 传送位置
		int64		transfer_time;	// 传送时间
		bool		is_transfer;	// 是否进行传送了
		s_attack_transfer_info()
		{
			memset(this, 0, sizeof(s_attack_transfer_info));
		}
	};

	// 传送信息数组
	struct s_attack_need_transfer_info
	{
		guid_64		role_guid;		// 角色id
		bool		is_transfer;	// 是否进行传送了
		s_attack_need_transfer_info()
		{
			memset(this, 0, sizeof(s_attack_need_transfer_info));
		}
	};

	// 攻城战NpcId
	struct s_attack_city_npc_info
	{
		int32		npc_type;							// 攻城战中的NPC类型
		guid_64		npc_guid;							// NPCGuid
		int32		template_id;						// 模板Id
		s_map_pos	born_pos;							// 复活坐标
		guid_64		born_npc_guid;						// 复活NPCId

		s_attack_city_npc_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_attack_city_npc_info));
		}

	};


	// 攻城战分组信息
	struct s_attack_city_legion_info
	{
		int32	group_level;							// 活动后等级		/100 等于等级 %100 等于分组索引
		guid_64 legion_guid;							// 军团Guid
		int32	server_id;								// 服务器id
		xchar	legion_name[max_name_size + 1];			// 军团名称
		int32	winning_streak_num;						// 连胜次数
		int32	is_check;								// 是否有效
		s_attack_city_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_attack_city_legion_info));
			group_level = -1;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		void set_legion_name(xstring _legion_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, _legion_name.c_str(), _legion_name.size() > sizeof(legion_name) ? sizeof(legion_name) : _legion_name.size());
		}

		bool operator < (const s_attack_city_legion_info& other_info) const
		{
			return group_level < other_info.group_level;
		}

		bool operator > (const s_attack_city_legion_info& other_info) const
		{
			return group_level > other_info.group_level;
		}
	};


	//攻城战军团信息
	struct s_attack_city_map_legion_info
	{
		int32	group_level;											// 分组等级
		int32	mode_index;												// 攻守模式
		int32	server_id;												// 服务器id
		guid_64 legion_guid;											// 军团Guid
		xchar	legion_name[max_name_size + 1];							// 军团名称
		guid_64 role_guid_list[max_legion_member_num];					// 军团参与玩家
		int64	legion_source;											// 攻防模式军团积分
		int32	is_win_num;												// >0是连胜奖励 -1是终结连胜
		int32	kill_1;													// 击杀数第一
		int32	kill_2;													// 击杀数第二
		int32	kill_3;													// 击杀数第三
		int32	kill_statue;											// 摧毁雕像
		int32	kill_gate;												// 摧毁城门
		int32	repair_statue;											// 复活雕像
		s_attack_city_map_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_attack_city_map_legion_info));
		}

		void clear_role_list()
		{
			memset(role_guid_list, 0, sizeof(role_guid_list));
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		void set_legion_name(xstring _legion_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, _legion_name.c_str(), _legion_name.size() > sizeof(legion_name) ? sizeof(legion_name) : _legion_name.size());
		}
	

		void add_role(guid_64 role_guid)
		{
			for (int32 i = 0; i < max_legion_member_num; ++i)
			{
				if (role_guid_list[i] == role_guid || false == role_guid_list[i].is_valid())
				{
					role_guid_list[i] = role_guid;
					break;
				}
			}
		}

		void add_legion_source(int64 add_num)
		{
			legion_source += add_num;
		}
	};

	struct s_attack_city_rank_info_db
	{


	};



	//攻城战个人排名信息
	struct s_attack_city_rank_info
	{
		int32	group_level;							// 分组等级
		int32	group_rank;								// 分组排名
		guid_64 legion_guid;							// 军团Guid
		guid_64 role_guid;								// 角色Guid
		xchar	role_name[max_name_size + 1];			// 玩家名称
		int32	role_info[e_attack_city_rank_info_max];	// 角色信息列表

		s_attack_city_rank_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_attack_city_rank_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}

		int32 get_role_info(int32 role_type)
		{
			if (role_type < 0 || role_type >= e_attack_city_rank_info_max)
			{
				return -1;
			}
			return role_info[role_type];
		}

		void set_role_info(int32 role_type, int32 role_value)
		{
			if (role_type < 0 || role_type >= e_attack_city_rank_info_max)
			{
				return;
			}
			role_info[role_type] = role_value;
		}

		void add_role_info(int32 role_type, int64 role_value)
		{
			if (role_type < 0 || role_type >= e_attack_city_rank_info_max)
			{
				return;
			}
			role_info[role_type] += role_value;
		}
	};

	//攻城战个人排名信息
	struct s_attack_city_slot_tem_info
	{
		guid_64 role_guid;							
		int32	role_source;
		s_attack_city_slot_tem_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_attack_city_slot_tem_info));
		}
	};
#pragma pack(pop)
}

#endif