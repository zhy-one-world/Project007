#pragma once
/********************************************************************
created: 2016年5月10日11:14:26
file base: mission
author: 张宇翔
purpose: 任务管理系统
*********************************************************************/
#ifndef _MISSION_MGR_HPP_
#define _MISSION_MGR_HPP_
#include "mission.hpp"
#include"Logic/mission_def.hpp"

namespace faith
{
	class cmission_mgr
	{
	public:
		cmission_mgr();
		~cmission_mgr();
	public:
		//初始化
		void clear_data();
		void heart_tick(const int64& new_time);
		void set_player_ptr(const int32 array_index) { m_array_index = array_index; }
		void init_daily_mission();
		void init_against_mission();
		void init_marry_mission();
		void check_and_send_all_mission();
		//任务操作
		cmission*	find_mission(int32 mission_id);
		cmission*	find_mission_by_index(int32 mission_index);
		cmission*	find_mission_by_type(int32 mission_type);

		void gm_mission(int32 mission_id);
		bool random_daily_mission(int32 force_mission_id = 0);
		bool random_against_mission(int32 force_mission_id = 0);
		bool random_marry_mission(bool is_operate, int32 force_mission_id = 0);
		void clear_marry_mission();
		void reset_mission_chapter(bool is_need_send = true);
		void operate_npc_show(MissionTemplate* OldTemplatePtr, MissionTemplate* NewTemplatePtr);
		void set_marry_mission(int32 new_mission_id);
		//消息
		void send_one_mission(cmission& my_mission);
		void send_all_mission();

		//存档
		void save_mission_to_db(int32 save_type);
		void load_mission_by_db(const s_mission_info* mission_data, int32 mission_num);

		bool load_mission_by_db_lua(const char *data_ptr, int32 data_len);

		void check_refresh_time();//计算是否要刷新日常
		void change_mission(const e_mission_slot mission_index, const int32 mission_template_id);


		int32 get_mission_id(const e_mission_slot mission_index);

		int32 get_new_mission_id(const e_mission_slot mission_index);
		bool  is_cur_mission_accord_level(const e_mission_slot mission_index);
		bool  is_cur_mission_high_level(const e_mission_slot mission_index);

		void	refresh_missions();
		void	refresh_main_mission();
		void	refresh_side_mission(int32 trigger_id);
		void	refresh_grade_mission();
		bool	is_has_same_group_misison(int32 check_group_id);
		bool	can_set_in_this_slot(int32 target_slot, MissionTemplate* mission_template_ptr);
		int32	get_can_set_mission_slot(MissionTemplate* mission_template_ptr);
		int32	get_empty_side_mission_slot(int32 trigger_mission_id);

		//状态修改触发
		void	target_check(e_mission_end_type mission_end_type, int32 target_info_0 = 0, int32 target_info_1 = 0);
		bool start_dialog(int32 mission_id, int32 npc_id);

		void  gm_set_grade_mission_id(int32 mission_id);

		int32 m_cur_mission_chapter_id;
	private:
		cmission m_got_mission_array[e_mission_slot_max];
		int32 m_array_index;

	public: 
		int32 get_cur_mission_chapter_id() { return m_cur_mission_chapter_id; }
		void  operate_npc_show_by_lua(int32 old_mission_id, int32 new_mission_id);
		
		bool m_power_up_array[e_power_up_prompt_max];
		void set_power_up_be_use(int32 power_type);
		bool get_power_up_is_can_check(int32 power_type);
	};
}
#endif