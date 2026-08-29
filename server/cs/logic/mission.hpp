/********************************************************************
created: 2016年5月5日11:14:26
file base: mission
author: 张宇翔
purpose: 单个任务
*********************************************************************/
#ifndef _MISSION_HPP_
#define _MISSION_HPP_

#include "Logic/mission_def.hpp"

namespace faith
{
	class player;
	struct s_item_template_info;
	struct MissionTemplate;

	class cmission
	{
	public:
		explicit cmission(void);
		~cmission(void);

	public:
		void				heart_tick(const int64& new_time);
		void				clear_data();
		void				init_mission_by_template(int32 mission_id, int32 mission_slot, player* player_ptr);
		void				init_mission_by_info(s_mission_info mission_info, player* player_ptr);
		bool				accept_mission();//接任务
		int32				finish_mission(int32 npc_id,e_mission_finish_type finish_type = e_mission_finish_type_normal);//完成发钱
		int32				finish_pre_proc(int32 finish_type);
		int32				fill_mission_reward_and_cost_array(std::vector<s_item_template_info>& item_reward_array,
															std::vector<s_item_template_info>& money_reward_array,
															std::vector<s_item_template_info>& money_cost_array,
															int32 finish_type);
		bool				is_last_daily_or_against();
		int32				can_finish_judge(const std::vector<s_item_template_info>& item_reward_array,
											const std::vector<s_item_template_info>& money_reward_array,
											const std::vector<s_item_template_info>& money_cost_array, int32 npc_id);

		int32				get_reward_and_cost(const std::vector<s_item_template_info>& item_reward_array,
											const std::vector<s_item_template_info>& money_reward_array,
											const std::vector<s_item_template_info>& money_cost_array);
		int32				finish_end_proc(int32 finish_type, const std::vector<s_item_template_info>& item_reward_array);

		MissionTemplate*	get_mission_template_ptr() { return m_mission_ptr; }
		int32				get_mission_end_type();

		int32				get_mission_state() { return m_mission_info.data_ary[e_mission_inst_data_mission_state]; }
		bool				set_mission_state(e_mission_state mission_state);

		s_mission_info&		get_ins_data_array() { return m_mission_info; }

		int32				get_inst_data(int32 index);
		bool				set_inst_data(int32 index, int32 num);
	public:
		void				create_npc();
		int32				get_mission_id();
		bool				can_accept_mission();//判断任务是否满足接取条件
		bool				check_mission_state();// 判断目标达成可以完成
		
		const bool			is_mission_target(const int32 npc_template_id);
		const int32			get_mission_goal_npc_kill_id(const int32 goal_index = 0);
		void				change_mission(const int32 mission_template_id);

		void				check_mission_state_with_aoi_npc();

		//任务计数相关
		bool				is_can_check(e_mission_end_type target_type, int32 target_info_0, int32 target_info_1, bool need_send_is_self = true);
		bool				target_check(e_mission_end_type mission_end_type, int32 target_info_0 = 0, int32 target_info_1 = 0, bool need_send_is_self = true);
		bool				start_dialog(int32 npc_id);//对话
		bool				is_valid(bool is_check_state = true);
		bool				marry_mission_can_check();

		//任务自己可见怪
		void				check_self_see_npc();
		bool				is_can_see_npc_guid(guid_64 npc_guid);
	private:
		// 任务目标相关的功能
		bool				is_valid_goal_index(const int32 goal_index);
		const int32			get_goal_index(const int32 killed_npc_template_id); // 检查杀掉的NPC是否是符合完成任务目标的NPC
		void				set_cur_mission_goal_num(const int32 goal_index, const int32 value);
		int32				get_cur_mission_goal_num(const int32 goal_index);

		int32				get_cur_reached_num(int32 target_info_0, int32 target_info_1);
		int32				get_target_num();
	private:
		bool				is_mission_goal_finish(const std::vector<int32>& template_goal_arr); // 检查是否任务目标是否完成

		// 任务动态刷NPC相关
		bool				exist_mission_create_npc_by_unit_index(const int32 npc_unit_array_index);
		bool				exist_mission_create_npc_by_id(const int32 npc_template_id);
		bool				exist_mission_create_npc_by_goal_index(const int32 goal_index);
		bool				add_mission_create_npc(const int32 npc_unit_array_index);
		bool				is_finish_mission_when_have_mission_create_npc();
		void				clear_mission_create_npcs();
		void				send_mission_create_npcs_info();
		void				operate_npc_show_by_state();

		void				active_temporary_wing(bool is_add);

	private:
		MissionTemplate*	m_mission_ptr;

		player*				m_player_ptr;
		s_mission_info		m_mission_info;

		int32				m_mission_create_npc[MAX_MISSION_LINE_NUM];

		std::vector<guid_64> m_npc_guid_arr;
	};
}

#endif