/********************************************************************
  created: 2015年8月5日11:13:58
  file base: ai_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _AI_DEF_HPP_
#define _AI_DEF_HPP_
#include "logic/type_def.hpp"
#include "unit_def.hpp"
#include "char_def.hpp"
namespace hld
{
	const int32 m_SkillMaxDistance = 200;
	const int32 npc_use_skill_len = 4;
	const int64	m_patrol_reach_judge_distance = 1000;		//npc巡逻到达目标点范围距离(平方值)
	const int32 move_yaw_value = 5;//npc移动时每次移动的角度
	enum e_ai_state
	{	
		e_ai_state_stand,	//原地不动
		e_ai_state_move,	//随机移动
		e_ai_state_chase,	//追击
		e_ai_state_move_to_born,//跑回出生点
		e_ai_state_skill,	//释放技能
		e_ai_state_born,	//出生
		e_ai_state_dead,	//死亡
		e_ai_state_dont_move,	//定身
		e_ai_state_follow,	//跟随
	};

	enum e_ai_patrol_type		//巡逻类型
	{
		e_ai_patrol_type_none,		//不巡逻
		e_ai_patrol_type_random,	//随机巡逻
		e_ai_patrol_type_line,		//定线巡逻
		e_ai_patrol_type_max,		
	};

	enum e_ai_target_check_ret
	{
		e_ai_target_check_ok,
		e_ai_target_check_invalid,
		e_ai_target_check_del,
		e_ai_target_check_dead
	};

	struct input_data
	{
		float				m_time_step;//时间差
		float				m_move_time;//移动的时间
		int32					m_monster_state;//npc所在的状态
		guid_64			m_target_guid;//目标guid
		fvector				m_target_position;//目标位置

		int32			m_skill_id;//技能id
		int32			m_skill_state;//技能目前处于的状态
		void ClearData()
		{
			m_monster_state = e_ai_state_stand;
			m_target_position.clear_data();
			m_target_guid.clear_data();
			m_skill_id = 0;
			m_skill_state = 0;
		}
	};

	struct out_data
	{
		int32					m_monster_state;//npc所在的状态
		hld::guid_64		m_TargetGuid;//目标guid
		fvector				m_TargetPosition;//目标位置

		int32			m_skill_id;//技能id
		int32			m_NextSkillState;//技能下一个的状态
		void ClearData()
		{
			m_monster_state = e_ai_state_stand;
			m_TargetGuid.clear_data();
			m_skill_id = 0;
			m_NextSkillState = 0;
		}
	};
	struct s_hate_info
	{
		s_unit_identifier unit_identifier;
		int64 gs_value;
		int64 hate_value;
		s_hate_info()
		{
			clear_data();
		}
		void clear_data()
		{
			unit_identifier.clear_data();
			hate_value = 0;
			gs_value = 0;
		}
	};
	typedef std::vector<s_hate_info> hate_info_vector;

	struct s_damage_info
	{
		s_unit_identifier unit_identifier;
		guid_64 unit_legion_guid;
		int64 damage_value;
		s_damage_info()
		{
			clear_data();
		}
		void clear_data()
		{
			unit_identifier.clear_data();
			unit_legion_guid.clear_data();
			damage_value = 0;
		}
		bool operator > (const s_damage_info& info) const
		{
			return damage_value > info.damage_value;
		}
	};
	struct s_damage_info_to_client
	{
		s_damage_info damage_info;
		xchar	player_name[max_name_size + 1];
		s_damage_info_to_client()
		{
			clear_data();
		}
		void clear_data()
		{
			damage_info.clear_data();
			memset(player_name, 0, sizeof(player_name));
		}
	};
	typedef std::vector<s_damage_info> damage_info_vector;

	struct s_boss_damage_list_person_item
	{
		guid_64 role_guid;
		xchar	role_name[max_name_size + 1];
		int64	damage_value;
		guid_64 master_guid;
		bool	team_check;
		guid_64 helper_guid[5];
		s_boss_damage_list_person_item()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid.clear_data();
			memset(role_name, 0, sizeof(role_name));
			damage_value = 0;
			master_guid.clear_data();
			team_check = false;
			for (auto i = 0; i < 5; ++i)
				helper_guid[i].clear_data();
		}
	};

	struct s_boss_damage_list_total_item
	{
		guid_64 role_guid;
		xchar	role_name[max_name_size + 1];
		int64	damage_value;
		s_boss_damage_list_total_item()
		{
			clear_data();
		}
		void clear_data()
		{
			role_guid.clear_data();
			damage_value = 0;
			memset(role_name, 0, sizeof(role_name));
		}
	};
}

#endif
