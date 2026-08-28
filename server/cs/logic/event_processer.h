/********************************************************************
created:	2018/04/02 14:22:00
file base:  event_processer
file ext:	h
author:	    zhy

purpose:
*********************************************************************/
#pragma once

#include "logic/type_def.hpp"
#include "Logic/count_def.hpp"

namespace hld
{
	//游戏条件
	enum e_game_condition_type
	{
		e_game_condition_type_null,//不需要条件
		e_game_condition_type_movement,//运动状态
		e_game_condition_type_max
	};
	// 游戏事件枚举
	enum e_game_event_type
	{
		e_game_event_type_null,
		e_game_event_type_unit_move,				// 角色移动触发
		e_game_event_type_unit_skill,				// 角色使用技能触发
		e_game_event_type_unit_buff_del,			// buff删除触发
		e_game_event_type_unit_hp_down,				// 血量降低触发
		e_game_event_type_unit_hp_up,				// 血量升高触发
		e_game_event_type_unit_attack_num,			// 攻击次数触发
		e_game_event_type_unit_dispel_self,			// 驱散已方负面状态
		e_game_event_type_unit_bullet,				// 子弹释放对敌
		e_game_event_type_unit_dead,				// 死亡
		e_game_event_type_unit_critical,			// 暴击
		e_game_event_type_unit_critical_be,			// 被暴击
		e_game_event_type_unit_kill,				// 任意击杀
		e_game_event_type_unit_hit,					// 击中
		e_game_event_type_unit_will_dead,			// 将死
		e_game_event_type_unit_dispel_target,		// 驱散对方正面状态
		e_game_event_type_unit_kill_player,			// 击杀玩家
		e_game_event_type_unit_be_hit,				// 被击中
		e_game_event_type_unit_kill_npc,			// 击杀怪物
		e_game_event_type_unit_bullet_self,			// 子弹释放对自己
		e_game_event_type_unit_energy_low,			// 能量值过低
		e_game_event_type_max,
	};
	class game_event_handler;

	struct s_trigger_param
	{
		int32 m_type;			// 消息类型
		int32 m_owner_index;	// 谁触发的这个消息
		int64 m_event_param_1;	// 单个参数预留
		int64 m_event_param_2;	// 单个参数预留
		int64 m_event_param_3;	// 单个参数预留
		s_trigger_param()
		{
			m_type = 0;
			m_owner_index = 0;
			m_event_param_1 = 0;
			m_event_param_2 = 0;
			m_event_param_3 = 0;
		}
		bool operator == (const s_trigger_param& _left) const
		{
			bool b_type = (m_type == _left.m_type);
			bool b_owner_index = (_left.m_owner_index == 0 || m_owner_index == 0) ? true : _left.m_owner_index == m_owner_index;
			return b_type && b_owner_index;
		}
	};
	struct game_event
	{
		int32 m_type;          // 消息类型
		game_event_handler*		m_dist_ptr;		// 事件接受者
		int32 m_skill_id;		// 哪个技能产生的buff
		int32 m_sender_index;	// 发送者
		int32 m_tick_time;		// 执行一次的时间间隔
		int64 m_last_time;		// 最后一次的执行时间
		int32 m_random_num;		// 概率
		s_trigger_param m_event_param;	// 单个参数预留
		game_event()
		{
			m_type = 0;
			m_dist_ptr = nullptr;
			m_skill_id = 0;
			m_sender_index = 0;
			m_tick_time = 0;
			m_last_time = 0;
			m_random_num = 0;
		}
	};
	class game_event_handler
	{
	public:
		virtual bool on_event(s_trigger_param& trigger_param) = 0;
		virtual bool on_event(int32 owner_index, s_trigger_param& trigger_param, const std::vector<int32>& event_condition);
		virtual void trigger_event(int32 target_index, bool is_add) = 0;
		virtual void change_event_param(int32 owner_index, int32 skill_template_id, int64 skill_cd, bool apply, const std::vector<int32>& event_condition);
	};
	typedef std::list<game_event> game_event_list;
	typedef game_event_list::iterator game_event_list_it;
	typedef std::map<int32, game_event_list> game_event_map;
	typedef game_event_map::iterator game_event_map_it;

	class event_processer
	{
	public:
		//去掉了变量的const修饰 避免ios编译不过的问题
		event_processer();
		void clear_data();
		void add_event(const game_event& event);
		void del_event(int32 event_type, const game_event_handler* m_dist_ptr);
	public:
		void heart_tick(const int64& new_time);
		void add_trigger_param(s_trigger_param& param);
		void trigger_event(s_trigger_param& param);
		void kill_all_event();
		bool is_tip_skill(int32 skill_id);
	private:
		game_event_map m_event_map;
		std::list<s_trigger_param> m_param_list;
	};

	class event_manager
	{
	private:
		event_manager() {};
		~event_manager() {};
	public:
		static event_manager& get_instance()
		{
			static event_manager s_event;
			return s_event;
		}
	public:
		event_processer* get_event_processer(int32 unit_index);
	private:
		event_processer m_player_array[init_player_max];
		event_processer m_npc_array[init_npc_max];
	};
}
