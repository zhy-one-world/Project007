/********************************************************************
created: 2014/07/30
created: 30:7:2014 22:11
file base: instance_def
file ext: hpp
author: Locke

purpose:
*********************************************************************/
#ifndef _INSTANCE_DEF_HPP_
#define _INSTANCE_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/att_def.hpp"

namespace hld
{
	enum instance_type
	{
		nouse_world,//0,无缝世界(目前无效)
		nouse_seamworld,//1,有缝世界(没用)
		loading_area,//2,载入区域
		multi_play,//3,普通副本
		team_play,//4,普通组队副本
		battle,//5,战场
		guild_instance,//6,家族
		internet_bar,//7,网吧
		same_guild_enter_same_instance,//8,同一个家族进入同一个副本
		corps_instance,	// 9,同一个军团进一个副本
	};
	struct tagForce
	{
		int32			ForceID;			// 所需要的声望ID
		int32			ForceValue;			// 所需要的声望值

		tagForce()
		{
			ForceID = 0;
			ForceValue = 0;
		}
	};
	// 副本模板
	struct tagInstanceInfo
	{
		int32						ResID;					// 副本场景资源id
		std::string				ResModName;				// 副本npc资源修改id
		instance_type			i_type;					// 副本类型
		uint32					exit_pos;				// 出口位置
		uint32					born_pos;				// 入口位置
		uint32					relive_pos;				// 复活点
		uint32					re_online_pos;			// 重新登陆点
		int32						dl_limit_player_count;	// 该副本所容纳的最大人数
		int32						aoi;					// aoi范围
		int32						limit_count;			// 最多存在个数
		int32						limit_duration_time;	// 总时间限制
		int32						delay_delete;			// 延迟删除
		bool					can_login;				// 下线后是否可以直接登录到副本
		int32						can_pk;					// 是否可以pk 0,根据场景,1,不可,2完全可以
		bool					camp_split;				// 是否根据阵营分割筛选
		int32						force_team_countdown;	// 离队是否倒计时(时间)
		int32						limit_level_low;		// 进入该副本的最低经验等级
		int32						limit_level_high;		// 进入该副本的最高经验等级
		std::vector<tagForce>	limit_forces;			// 所需要的声望值
		std::map<uint32, uint32>		limit_item;			// 所需要的物品ID (玩家拥有这个物品即可，不分装备上还是背包里有)
		std::vector<uint32>		limit_buff;				// 所需要的buff ID (buff消失怎么办)
		std::vector<uint32>		limit_rewarded_quest;	// 需要完成了的任务
		std::vector<uint32>		limit_have_quest;		// 需要完成中的任务
		std::set<uint32>			limit_job;				// 职业限制
		int32						limit_sex;				// 性别限制
		std::set<uint32>			limit_ip_group;			// ip限制
		f32						reset_time;				// 副本重置时间
		int32						enter_times;			// 副本重置前，玩家最多进入的次数
		f32						finish_time;			// 副本完成周期
		int32						finish_times;			// 副本完成周期，玩家最多完成的次数
		f32						reset_time_player;		// 副本重置时间
		int32						enter_times_player;		// 副本重置前，玩家最多进入的次数
		f32						finish_time_player;		// 副本完成周期
		int32						finish_times_player;	// 副本完成周期，玩家最多完成的次数
		std::vector<uint32>		buff_info;				// 进入副本后添加的buff
		std::map<uint32, uint32>		tird_info;			// 疲劳信息
		std::map<uint32, uint32>		del_item_info;		// 进入副本后扣除的道具列表
		std::string				script_name;			// 扩展脚本
		uint32					war_map_id;
		uint32					jzbase_map_id;
		i8						limit_mt_low;			// 进入该副本的最低转生次数
		i8						limit_mt_high;			// 进入该副本的最高转生次数
		int32						one_day_finish_times;	// 每天最多完成次数
		bool					check_high;
		bool					disable_hidden_line;
		bool					disable_player_limit;
		bool					bactionmap;

		// .. 其它数据

		tagInstanceInfo() :
			ResID(0),
			i_type(nouse_world),
			exit_pos(0),
			born_pos(0),
			relive_pos(0),
			re_online_pos(0),
			dl_limit_player_count(0),
			aoi(5600),
			limit_count(0),
			limit_duration_time(0),
			delay_delete(0),
			can_login(0),
			can_pk(0),
			camp_split(0),
			force_team_countdown(0),
			limit_level_low(0),
			limit_level_high(0),
			limit_sex(0),
			reset_time(0),
			enter_times(0),
			finish_time(0),
			finish_times(0),
			reset_time_player(0),
			enter_times_player(0),
			finish_time_player(0),
			finish_times_player(0),
			war_map_id(0),
			jzbase_map_id(0),
			limit_mt_low(0),
			limit_mt_high(0),
			one_day_finish_times(0),
			check_high(true),
			disable_hidden_line(false),
			disable_player_limit(true),
			bactionmap(false)
		{
		}
	};
}
#endif