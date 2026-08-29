/********************************************************************
	created:	2016年12月12日12:50:18
	file base:	msgproc_ranking_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_RANKING_WS_H_
#define _MSGPROC_RANKING_WS_H_

#include "db_manager.hpp"
#include "Logic/arena_def.hpp"

namespace faith
{
	// 需要越过cs直接读/写数据库的内容
	//排行榜
	void save_arena_rank(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_arena_rank_(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_arena_rank_end(db_result_type result);

	void save_arena_rank_first(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_arena_rank_second(db_result_type result, uint32 connindex, const s_arena_rank target_new_info);

	void save_arena_rank_first_lua(uint32 connindex, uint32 t_rank_pos, int64 t_role_guid, xstring t_role_name, int64 t_gs_value, uint32 t_role_temp_id, uint32 t_robot_temp_id, uint32 t_military_rank_level
		, uint32 t_vip_level, xstring t_game_channel, uint32 n_rank_pos, int64 n_role_guid, xstring n_role_name, int64 n_gs_value, uint32 n_role_temp_id, uint32 n_robot_temp_id, uint32 n_military_rank_level
		, uint32 n_vip_level, xstring n_game_channel);
	void load_all_ranking_info_lua(uint32 connindex, uint32 ranking_type);
	void save_ranking_info_one_lua(uint32 connindex, int64 role_guid,xstring role_name,int64 ranking_type,int64 ranking_value,
		int64 save_time,int64 supporting_guid,xstring supporting_name,int32 vip_level,int32 peak,int32 server_id,xstring game_channel,int64 standby_parameter);
	void load_all_service_rank_info_lua(uint32 connindex, uint32 rank_type);
	void save_ranking_worship_lua(uint32 connindex, int64 sender_guid, int64 role_guid, xstring role_name, int64 ranking_type, int64 ranking_value,
		int64 save_time, int64 supporting_guid, xstring supporting_name, int32 vip_level, int32 peak, int32 server_id, xstring game_channel, int64 standby_parameter,
		int64 worship_add_value, int32 worship_type);
	void load_player_worship_value_lua(uint32 connindex, int64 sender, int64 target);

	void load_arena_rank(uint32 connindex, const void *data_ptr, size_t data_len);
	void do_load_arena_rank(uint32 connindex, int32 start_index);
	void load_arena_rank_end(db_result_type result,uint32 connindex, int32 old_start_from);

	//void load_all_ranking_info_proc();
	void load_all_ranking_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_ranking_type(uint32 connindex, int32 ranking_type);
	void load_all_ranking_info_end(db_result_type result, uint32 connindex, int32 ranking_type);

	void save_ranking_info_one(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_ranking_info_one_end(db_result_type result, uint32 connindex, const guid_64& target_guid);

	void save_one_ranking_list(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_one_ranking_list_end(db_result_type result, uint32 connindex );

	void save_ranking_worship(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_ranking_worship_end(db_result_type result, uint32 connindex, const guid_64& sender_guid, int32 from_ranking_type, int32 worship_add_value, int32 worship_type);

	void load_player_worship_value(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_player_worship_value_end(db_result_type result, uint32 connindex, const guid_64& sender_guid);

	void add_robot_worship(uint32 connindex, const void *data_ptr, size_t data_len);
	void add_robot_worship_end(db_result_type result, uint32 connindex);

	void save_one_service_rank_list(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_one_service_rank_list_end(db_result_type result, uint32 connindex);
	void load_all_service_rank_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_service_rank_type(uint32 connindex, int32 rank_type);
	void load_all_service_rank_info_end(db_result_type result, uint32 connindex);
}

#endif