/********************************************************************
	created:	2016Äê12ÔÂ12ÈÕ12:56:43
	file base:	msgproc_legion_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_LEGION_WS_H_
#define _MSGPROC_LEGION_WS_H_

#include "legion_def.hpp"


namespace hld
{
	struct db_result_type;

	// legion
	void save_legion_info_lua(uint32 connindex, int64 legion_guid, xstring legion_name, xstring announcement, int64 gs_value, int64 data_ary[ELegionInfo_max]);
	void save_legion_member_info_lua(uint32 connindex, int64 legion_guid, int64 role_guid, xstring role_name, int64 gs_value, int64 data_ary[e_legion_member_info_max]);
	void save_legion_city_war_member_info_lua(uint32 connindex, int64 legion_guid, int64 role_guid, int64 use_type, xstring role_name, int64 player_temp_id, int64 week_contribution, int64 job_title);
	void delete_legion_info_lua(uint32 connindex, int64 legion_guid);
	void delete_legion_member_info_lua(uint32 connindex, int64 role_guid);
	void delete_legion_city_war_member_info_lua(uint32 connindex, int32 del_type);
	void delete_legion_applicant_lua(uint32 connindex, int64 role_guid);
	void save_legion_applicant_lua(uint32 connindex, int64 legion_guid, int64 role_guid, xstring role_name, float exist_time, int64 gs_value, int32 data_ary[ELegionApplicantInfo_max]);
	void save_legion_boss_award_get_log_lua(uint32 connindex, int64 role_guid, int32 boss_map_id);
	void load_all_legion_boss_award_get_log_lua(uint32 connindex, int64 role_guid);
	void save_city_info_lua(uint32 connindex, int64 occupation_legion_guid, xstring city_master_name);
	void save_war_bid_info_lua(uint32 connindex, int32 territory_id, int32 index_in_terr, int64 legion_guid, int32 cur_bid_price, int32 is_prior_maintain,
		int32 occupy_time_stamp, int32 server_id, xstring legion_name);
	void clear_war_bid_info_lua(uint32 connindex, int32 clear_type);
	void clear_city_war_reward_stuff_lua(uint32 connindex, int32 del_stuff_type, int64 target_guid);
	void save_legion_event_lua(uint32 connindex, int64 legion_guid, int32 event_idx, xstring extra_data, int32 happen_time);
	void clear_legion_event_lua(uint32 connindex, int64 legion_guid);
	void save_legion_bonus_info_lua(uint32 connindex, int64 legion_guid, int32 mission_id, int32 finish_count);
	void clear_legion_bonus_info_lua(uint32 connindex, int64 legion_guid);

	void save_legion_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_info_end(db_result_type result, uint32 connindex);
	void load_all_legion_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_legion_info_end(db_result_type result, uint32 connindex, int32 server_id);
	void delete_legion_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void delete_legion_info_end(db_result_type result, uint32 connindex);
	void delete_legion_member_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void delete_legion_member_info_end(db_result_type result, uint32 connindex);
	void delete_legion_city_war_member_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void delete_legion_city_war_member_info_end(db_result_type result, uint32 connindex);
	void save_legion_member_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_member_info_end(db_result_type result, uint32 connindex);
	void save_legion_city_war_member_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_city_war_member_info_end(db_result_type result, uint32 connindex);
	void load_all_legion_member_info(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_member_info_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_city_war_member_info(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_city_war_member_info_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void delete_legion_applicant(uint32 connindex, const void *data_ptr, size_t data_len);
	void delete_legion_applicant_end(db_result_type result, uint32 connindex);
	void save_legion_applicant(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_applicant_end(db_result_type result, uint32 connindex);
	void load_all_legion_applicant(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_applicant_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void save_legion_boss_award_get_log(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_boss_award_get_log_end(db_result_type result, uint32 connindex);
	void load_all_legion_boss_award_get_log(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_all_legion_boss_award_get_log_end(db_result_type result, uint32 connindex);
	void save_city_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_city_info_end(db_result_type result, uint32 connindex);
	void load_city_info(uint32 connindex);
	void load_city_info_end(db_result_type result, uint32 connindex);
	void load_war_bid_info(uint32 connindex);
	void load_war_bid_info_end(db_result_type result, uint32 connindex);
	void save_war_bid_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_war_bid_info_end(db_result_type result, uint32 connindex);
	void clear_war_bid_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_war_bid_info_end(db_result_type result, uint32 connindex);
	void load_all_legion_event(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_event_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void save_legion_event(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_event_end(db_result_type result, uint32 connindex);
	void del_legion_event(uint32 connindex, const void *data_ptr, size_t data_len);
	void del_legion_event_end(db_result_type result, uint32 connindex);
	void clear_legion_event(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_legion_event_end(db_result_type result, uint32 connindex);
	void clear_overlord_reward_stuff_func(uint32 connindex, const int32& mount_template_id, const int32& wing_template_id, const int32& title_id, const int32& buff_if);
	void clear_city_war_reward_stuff(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_city_war_reward_stuff_end(db_result_type result, uint32 connindex);
	void load_legion_bonus_info(uint32 connindex);
	void load_legion_bonus_info_end(db_result_type result, uint32 connindex, int32 server_id);
	void save_legion_bonus_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_legion_bonus_info_end(db_result_type result, uint32 connindex);
	void clear_legion_bonus_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_legion_bonus_info_end(db_result_type result, uint32 connindex);
	void load_all_legion_warehouse_info(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_warehouse_info_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void store_legion_warehouse(uint32 connindex, const void *data_ptr, size_t data_len);
	void store_legion_warehouse_end(db_result_type result, uint32 connindex);
	void store_legion_warehouse_log(uint32 connindex, const void *data_ptr, size_t data_len);
	void store_legion_warehouse_log_end(db_result_type result, uint32 connindex);
	void role_get_legion_warehouse(uint32 connindex, const void *data_ptr, size_t data_len);
	void role_get_legion_warehouse_end(db_result_type result, uint32 connindex);
	void destroy_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len);
	void destroy_legion_warehouse_item_end(db_result_type result, uint32 connindex);
	void clear_legion_warehouse(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_legion_warehouse_end(db_result_type result, uint32 connindex);
	void load_all_legion_warehouse_log(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);
	void load_all_legion_warehouse_log_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index);

	void load_cross_city_info_lua(uint32 connindex);
	void load_cross_city_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_cross_city_info_end(db_result_type result, uint32 connindex);

	void save_cross_city_info_lua(uint32 connindex, uint32 rank_idex, int64 legion_guid, uint32 server_id, int64 chief_guid, xstring legion_name, xstring chief_name);
	void save_cross_city_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_cross_city_info_end(db_result_type result, uint32 connindex);

	void clear_cross_city_info_lua(uint32 connindex);
	void clear_cross_city_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_cross_city_info_end(db_result_type result, uint32 connindex);

	void clear_only_one_member_title(int32 title_temp_id, guid_64 role_guid, int32 connindex);

	static s_legion_info legion_info_arr_dp[max_legion_count];
}

#endif