/********************************************************************
	created:	2016/04/01
	filename: 	fuben_data_set.h
	file base:	fuben_data_set
	file ext:	    h
	author:		Steven.Han
	purpose:	
*********************************************************************/
#ifndef _FUBEN_DATA_SET_H_
#define _FUBEN_DATA_SET_H_

#include "logic/type_def.hpp"

namespace faith
{
	typedef std::unordered_map<int32, int32> fuben_boss_info_map;
	struct s_fuben_info_to_client
	{
		int32 wave_id;
		int32 cur_wave;
		int32 total_wave;
		int32 cur_npc;
		int32 total_npc;
		e_map_state cur_map_state;
		bool  is_over;
		int32 cur_player_num;
		int32 kill_monster_num;
		fuben_boss_info_map create_bosses;

		s_fuben_info_to_client()
		{
			is_over = false;
			wave_id = 0;
			cur_wave = 0;
			total_wave = 0;
			cur_npc = 0;
			total_npc = 0;
			cur_player_num = 0;
			cur_map_state = e_map_state_join;
			create_bosses.clear();
			kill_monster_num = 0;
		}
	};

	struct s_fuben_settlement //副本结算
	{
		s_fuben_settlement()
		{
			pass_time = 0;
			dead_count = 0;
			max_hit = 0;
			fuben_level = 0;
		}

		int32 pass_time;    //通关时间
		int32 dead_count;   //死亡次数
		int32 max_hit;      //最大连击数
		int32 fuben_level;  //副本评级
	};

	struct tag_fuben_data
	{
		int32 enter_count;       //副本完成次数
		int32 pass_time;         //本次副本完成时间
		int32 pass_time_mebest;  //自己完成这个副本的最短时间记录
	};

	class fuben_data_set
	{
	public:
		typedef std::unordered_map<int32, tag_fuben_data> fuben_data_map;

	public:
		tag_fuben_data* get_fuben_data( int32 fuben_id );
		void add_fuben_data(int32 fuben_id, tag_fuben_data& fuben_data);
		void clear_enter_count(void);
		void clear(void);

	public:


	private:
		fuben_data_map m_data_map;
	};

}

#endif // _buff_set_h_