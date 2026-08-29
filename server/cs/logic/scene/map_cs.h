/********************************************************************
	created:	2015/03/21
	created:	21:3:2015   21:36
	file base:	map_cs
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

//#include <queue>
#include "npc_spawner.h"
#include "map_object.h"

namespace faith
{
	struct target_time_spawn_info
	{
		int32 spawn_template_id;
		int64 target_spawn_time;
		NpcSpawnPointTemplate* spawn_template_ptr;

		target_time_spawn_info ()
		{
			spawn_template_id = 0;
			target_spawn_time = 0;
			spawn_template_ptr = nullptr;
		}
	};

	typedef std::vector<target_time_spawn_info>	target_time_spawn_vec;

	class map_cs : public map_object
	{
	public:
		map_cs();
		~map_cs();
	public:
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		virtual bool			create_by_template_data(const guid_64& map_guid, int32 template_id, int32 line_id, int32 map_all_time);
		virtual void			heart_tick(const int64& new_time);
		virtual void			clear_data();

		virtual float			get_time_left(int32 player_idx = -1);

	private:
		void					tick_sec(const int64& new_time);
		void					tick_2sec(const int64& new_time);
		void					tick_3sec(const int64& new_time);
		void					check_invalid_map_operate();

	public:

		/************************************************************************/
		/*					unit数据                                            */
		/************************************************************************/
		virtual void			on_player_enter_scene(const int32& unit_index);
		virtual	void			on_player_leave_scene(const int32& unit_index);
		virtual void			on_npc_delete(const int32& unit_index, bool is_dead);
		virtual void			on_player_dead(const int32& unit_index);

		virtual npc*			spawn_npc(int32 spawn_point_id, int32 dynamic_lv = -1);
		int32					get_npc_left_life_time(int32 spawn_template_id);
		void					get_valid_period_spawn_time(int32 spawn_template_id, int32& begin_time_sec, int32& end_time_sec);
		int32					get_next_target_time_spawn_index(NpcSpawnPointTemplate* spawn_template_ptr);
		int64					get_next_spawn_time(NpcSpawnPointTemplate* spawn_template_ptr, bool is_be_kill = false);
		bool					is_in_refresh_time(NpcSpawnPointTemplate* spawn_template_ptr);
		void					spawn_by_target_time_sec(int64 new_time); //target time spawn
		void					send_gain_treasure_npc_to_ws();
		void					send_gain_treasure_record_to_ws_when_npc_dead(npc& temp_npc, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr,std::set<int32> player_index_arr);
		void					send_gain_treasure_record_to_ws_when_npc_spawn(npc& temp_npc);

		void					add_kill_count_with_boss_island(npc& temp_npc, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr,std::set<int32> player_index_arr);
		void					player_kill_monster(const int32& unit_index, bool isBoss = false);
		void					sync_stage_point_to_player(const int32& unit_index, const int32& stage_point_value);
		//void					update_belief_cloister_player_info(bool is_update_point);
		void					create_harry_map_normal_npc(int32 num);

		void					set_transfer_all_player_delay_second(int64 delay_time);
		void					set_transfer_all_player_delay_seconds(int64 delay_time);

		void					record_server_crystal_log(int32 unit_index);
	private:
		xstring					m_map_lua_fun;
		target_time_spawn_vec	m_target_time_spawn_vec;	// 指定时间刷新列表

	private:
		int64					m_stamp_last_sec_tick;
		int64					m_stamp_last_3sec_tick;
		int64					m_stamp_last_2sec_tick;
		int64					m_stamp_last_10sec_tick;

		int64					m_transfer_all_player_delay_seconds;
	};
}
