#ifndef _SOCIAL_DEF_HPP_
#define _SOCIAL_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace faith
{
#pragma pack(push,1)

	const int32 max_team_member_num = 3;	// 一个组队中成员的最大数量
	const int32 gain_treasure_gain_team_num = 5; //打宝boss队伍数据记录数量
	const int32 ready_transfer_sec = 3;		// 传送倒计时
	const int32 platform_get_team_num = 10;
	const int32 cap_offline_check_sec = 15;	// 离线监测
	const int32 wait_for_mem_ready_sec = 20;//准备等待时间

	const int32 first_team_aim_template_id = 122000001;
	
	const int32 match_hunting_level_low = 60;
	const int32 match_hunting_level_high = 500;
	const int32 match_hunting_lv_interval = 20;

	enum e_join_team_way
	{
		e_join_team_way_by_team_guid,        // 通过队伍的GUID来加入队伍
		e_join_team_way_by_captain_guid,     // 通过队长的GUID来加入队伍
		e_join_team_way_max
	};

	enum ETeamMemberInfo
	{
		ETeamMemberInfo_is_online,			// 成员是否在线
		ETeamMemberInfo_line_id,			// 成员所在线
		ETeamMemberInfo_player_template_id,	// 成员的头像ID
		ETeamMemberInfo_cur_hp,				// 成员的当前生命值
		ETeamMemberInfo_max_hp,				// 成员的最大生命值
		ETeamMemberInfo_level,				// 成员的等级
		ETeamMemberInfo_fighting_power,		// 成员的战斗力
		ETeamMemberInfo_cur_map,			// 成员当前所在的地图ID
		ETeamMemberInfo_damage,				// 造成的伤害
		ETeamMemberInfo_PosX,				// 当前坐标X
		ETeamMemberInfo_PosY,				// 当前坐标Y
		ETeamMemberInfo_PosZ,				// 当前坐标Z
		ETeamMemberInfo_ready_state,		// 准备状态
		ETeamMemberInfo_follow_state,		// 跟随状态
		ETeamMemberInfo_check_ret,			// checkret
		ETeamMemberInfo_one_stop_flags,		// 一条龙flags
		ETeamMemberInfo_agora_id,			// AgoraId
		ETeamMemberInfo_max
	};

	struct s_team_member_info
	{
		guid_64		role_guid;						// 成员的角色GUID
		int32		role_index;						// 成员的数组下标
		xchar		role_name[max_name_size + 1];	// 成员的名称
		int64		gs_value;						// 成员的战力值
		int64		data_ary[ETeamMemberInfo_max];

		bool is_valid() const
		{
			return role_guid.is_valid();
		}

		s_team_member_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_team_member_info));
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

	struct s_player_team_info
	{
		guid_64		team_guid;
		guid_64		captain_guid;
		int32		member_num;
		int32		team_map_id;
		e_team_type	team_type;
		int32		team_sub_type_id;
		int32		team_one_stop_flags;
		guid_64		team_mems_guid[max_team_member_num];

		bool is_valid()
		{
			return team_guid.is_valid();
		}

		void clear_mems_guid()
		{
			memset(team_mems_guid, 0, sizeof(team_mems_guid));
		}

		s_player_team_info()
		{
			clear_data();
		}

		void clear_data()
		{
			team_guid.clear_data();
			captain_guid.clear_data();
			member_num = 0;
			team_map_id = 0;
			team_type = e_team_type_none;
			team_sub_type_id = -1;
			team_one_stop_flags = -1;
			clear_mems_guid();
		}
		bool to_proto(faith::st_proto::st_player_team_info *st_team_ptr)
		{
			if (st_team_ptr == nullptr)
			{
				return false;
			}
			st_team_ptr->set_team_guid(team_guid.server_64);
			st_team_ptr->set_captain_guid(captain_guid.server_64);
			st_team_ptr->set_member_num(member_num);
			st_team_ptr->set_team_map_id(team_map_id);
			st_team_ptr->set_team_type(team_type);
			st_team_ptr->set_team_sub_type_id(team_sub_type_id);
			st_team_ptr->set_team_one_stop_flags(team_one_stop_flags);
			for (int32 i = 0; i <max_team_member_num; i++)
			{
				st_team_ptr->add_team_mems_guid(team_mems_guid[i]);
			}
			return true;
		}
		void from_proto(const faith::st_proto::st_player_team_info& st_team_ref)
		{
			team_guid.server_64 = st_team_ref.team_guid();
			captain_guid.server_64 = st_team_ref.captain_guid();
			member_num = st_team_ref.member_num();
			team_map_id = st_team_ref.team_map_id();
			team_type = (e_team_type)st_team_ref.team_type();
			team_sub_type_id = st_team_ref.team_sub_type_id();
			team_one_stop_flags = st_team_ref.team_one_stop_flags();
			for (int32 i = 0; i < st_team_ref.team_mems_guid_size(); i++)
			{
				team_mems_guid[i] = st_team_ref.team_mems_guid(i);
			}

		}
	};

	enum e_team_error
	{
		e_team_error_none,
		e_team_error_team_full,							// 队伍已满
		e_team_error_fighting_power_not_enough,			// 战斗力不足不能加入队伍
		e_team_error_level_not_enough,					// 等级不足不能加入队伍
		e_team_error_player_already_join_team,			// 玩家已经加了队伍
		e_team_error_player_already_join_other_team,	// 玩家已经加入了其他队伍
		e_team_error_player_already_offline,			// 玩家已经离线
		e_team_error_team_already_dissolve,				// 队伍已经解散
		e_team_error_target_player_not_in_team,			// 目标玩家未在任何队伍中
		e_team_error_target_player_is_not_captain,		// 目标玩家不是队长
		e_team_error_captain_change,					// 队长已变更
		e_team_error_create_raid_failed,				// 创建副本失败
		e_team_error_invalid_raid_instance,				// 无效的副本地图实例
		e_team_error_member_change_during_transfer,		// 传送期间队伍成员发生改变
		e_team_error_raid_condition_less,				// 不满足目标副本条件 如等级进入次数
		e_team_error_cur_map_cant_ready,				// 当前地图不可发起准备
		e_team_error_team_member_is_full,				// 队伍人数已满
		e_team_error_raidmulti_ingame,					// 队伍已经进入多人本
		e_team_error_max
	};

	enum e_player_match_team_ret
	{
		e_player_match_team_none,
		e_player_match_team_waiting,
		e_player_match_team_succeed
	};

	struct s_team_auto_match_condition
	{
		e_team_type	aim_type;
		int32		aim_sub_type;
		int32		os_flags;

		s_team_auto_match_condition()
		{
			clear_data();
		}
		
		s_team_auto_match_condition(e_team_type _aim_type, int32 _aim_sub_type)
			: aim_type(_aim_type), aim_sub_type(_aim_sub_type), os_flags(-1)
		{}

		void clear_data()
		{
			aim_type = e_team_type_none;
			aim_sub_type = -1;
			os_flags = -1;
		}
	};
	typedef std::map<guid_64, s_team_auto_match_condition> player_auto_match_map;

	enum e_team_common_check_invoker
	{
		e_team_common_check_invoker_auto_match_team,
		e_team_common_check_invoker_mem_ready,
		e_team_common_check_invoker_up_mem_check_ret
	};
	
	enum e_team_req_mate_type
	{
		e_team_req_mate_type_friend,
		e_team_req_mate_type_legion
	};

	enum e_team_mem_ready_state
	{
		e_team_mem_ready_state_none,
		e_team_mem_ready_state_ready,
		e_team_mem_ready_state_waiting_ready,
	};

	enum e_team_one_stop_aim_state
	{
		e_team_one_stop_aim_ok,
		e_team_one_stop_aim_lack_of_times,
		e_team_one_stop_aim_locked,
	};

#pragma pack(pop)
}

#endif