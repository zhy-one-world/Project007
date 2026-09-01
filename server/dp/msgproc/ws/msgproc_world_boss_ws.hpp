/********************************************************************
	created:	2016年12月12日13:08:03
	file base:	msgproc_world_boss_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_WORLD_BOSS_WS_H_
#define _MSGPROC_WORLD_BOSS_WS_H_

#include "db_manager.hpp"
#include <Logic/team_def.hpp>
#include <Logic/world_boss_def.hpp>

namespace faith
{
	void load_world_boss(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_world_boss_end(db_result_type result, uint32 connindex);
	void save_world_boss_to_sql(uint32 connindex, const void *data_ptr, size_t data_len);

	void load_all_gain_treasure_record_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_gain_treasure_record_info_end(db_result_type result, uint32 connindex);
	void save_gain_treasure_record_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_gain_treasure_record_info_lua(uint32 connindex, uint32 boss_spawn_point_id);
	void save_gain_treasure_record_info_lua(uint32 connindex, uint32 del_stamp, uint32 boss_spawn_point_template_id, uint32 killed_stamp,
	xstring role_name[max_team_member_num], xstring gain_item_template_id[max_team_member_num], int64 role_guid[max_team_member_num]);

	void save_world_boss_to_sql_lua(uint32 connindex, int32 npc_index[max_world_boss_and_gold_army_num], int32 template_id[max_world_boss_and_gold_army_num], int32 is_alive[max_world_boss_and_gold_army_num],
		int32 line_id[max_world_boss_and_gold_army_num], int64 last_respawn_time[max_world_boss_and_gold_army_num], int32 respawn_phase[max_world_boss_and_gold_army_num], xstring killer_name[max_world_boss_and_gold_army_num],
		xstring first_killer_name[max_world_boss_and_gold_army_num], int32 first_killed_time[max_world_boss_and_gold_army_num], int32 is_get_prize[max_world_boss_and_gold_army_num]);
	void load_world_boss_lua(uint32 connindex);
}

#endif