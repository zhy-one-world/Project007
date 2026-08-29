#pragma once
/********************************************************************
created: 2018年09月13日
author: 刘阳
purpose: 信仰符文管理系统
*********************************************************************/
#ifndef _BELIEF_RUNE_MGR_H_
#define _BELIEF_RUNE_MGR_H_

#include"item.h"

namespace faith
{
	typedef std::vector<guid_64> ITEM_ARRAY;

	typedef std::map<int32, int32> belief_rune_page_value;
	typedef std::map<int32, belief_rune_page_value> belief_rune_page_map;

	class player;
	struct BeliefRunesTemplate;
	enum e_belief_rune_operate_type;

	class belief_rune_mgr
	{
	public:
		belief_rune_mgr();
		~belief_rune_mgr();
	public:
		void clear_data();
		void init_data();
		void set_player_ptr(player* player_ptr);
		void heart_tick(const int64& new_time);
		void reset_belief_equip_state_cache();
		bool is_type_equiped(e_belief_series_type page_type, int32 belief_rune_type);
		void set_type_equiped(e_belief_series_type page_type, int32 belief_rune_type, bool is_equiped);

		BeliefRunesTemplate* get_belief_rune_template(const citem& item);

		bool set_belief_rune_one_att(citem& item, bool is_add);
		bool set_belief_rune_all_att(bool is_add);
		
		void response_operate_result(e_belief_rune_operate_type operate_type, int32 error_id);

		//信仰回廊相关
		int32 get_stage_point() { return m_stage_point; };
		void add_stage_point(int32 num) { m_stage_point += num; };
		void set_stage_point(int32 num) { m_stage_point = num; };
		int32 get_leave_stage_left_time() { return m_leave_stage_left_time; };
		void set_leave_stage_left_time(int32 seconds) { m_leave_stage_left_time = seconds; };
		bool is_in_belief_cloister_battle() { return m_is_in_belief_cloister_battle; };
		void set_is_in_belief_cloister_battle(bool ret) { m_is_in_belief_cloister_battle = ret; };
		bool is_in_dead() { return m_is_dead_flag; };
		void set_is_in_dead(bool ret) { m_is_dead_flag = ret; };

		int64 calcu_total_fighting_power();

	private:
		player* m_player_ptr;
		int32 m_attr_type_equiped_cache[e_belief_series_type_max];
		int32 m_decompose_rune_piece; //分解获得的符文碎片 用于前端显示

		//信仰回廊相关
		int32 m_stage_point;
		int32 m_leave_stage_left_time;
		bool m_is_in_belief_cloister_battle;
		bool m_is_dead_flag;
	};
}

#endif