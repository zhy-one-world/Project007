#pragma once

#include "Logic/ranking_def.hpp"
#include "title_def.hpp"

namespace faith
{
	class ranking_proto_ranking_player_info_lua;
	class ranking_proto_ranking_player_info;

	class  ranking_mgr_ws
	{
	public:
		static void init_manager();	//初始化管理
		static void heart_tick(int64& time_now);
		static void clear_data();
		static void clear_data_by_ranking_type(e_RankingIndex rank_type);//限时活动排行榜数据清理调用
	public:
		static void tick_save_ranking_list();
		static void save_ranking_info_to_db(const s_ranking_player_info& ranking_info);
		static void set_ranking_save_flag(int32 ranking_type, bool is_save);
		static void req_load_all_ranking_from_db();					//现在ranking的load 是在军团管理器的dbload完成后进行的
		static bool fixed_ranking_supporting_data(s_ranking_player_info* load_info, const guid_64& legion_guid, const xstring& legion_name);
		static bool fixed_ranking_legion_name_data(s_ranking_player_info* load_info, const guid_64& legion_guid, const xstring& legion_name);
		static void change_ranking_player_legion_info(const guid_64& ranking_guid, const guid_64& legion_guid, const xstring& legion_name);
		static void load_ranking_from_db(const s_ranking_player_info* load_info, int32 data_num, int32 ranking_type);
		static void c2ws_get_ranking_list_func(e_RankingIndex ranking_index, const guid_64& role_guid);
		static void ws2gws_get_ranking_list_func(const void* data_ptr, size_t data_len);
		static void c2ws_get_my_rank_func(e_RankingIndex ranking_type, const guid_64& sender_guid, const guid_64& legion_guid);
		static void ws2gate_get_my_rank_func(const guid_64& player_guid, int32 server_id, const guid_64& role_guid, int32 ranking_type);
		static bool make_ranking_data(ranking_proto_ranking_player_info_lua* ranking_player_data, const s_ranking_player_info& player_data);
		static bool make_ranking_data(ranking_proto_ranking_player_info* ranking_player_data, const s_ranking_player_info& player_data);
		static s_ranking_player_info* get_ranking_player_info_by_type(e_RankingIndex ranking_index, const guid_64& target_guid);
		static void set_gate_ranking_legion_name(const guid_64& legion_guid, const xstring& legion_name);
		static void set_ranking_leigon_chief_guid(int32 rank_type, const guid_64& legion_guid, const guid_64& role_guid, const xstring& legion_chief_name, const int32& legion_chief_vip_level);//更改军团长的Guid
		static void delete_ranking_by_role_guid(e_RankingIndex ranking_index, const guid_64& target_guid);
		static void open_server_first_day_proc();
		static void send_gs_ranking(e_RankingIndex get_ranking_type, e_RankingIndex cross_ranking_type);
		static void delete_ranking_with_server_list_refresh_end();
		static void delete_ranking_with_server_lits_refresh_by_type(e_RankingIndex ranking_type);
		static void get_gs_ranking(const void* data_ptr, size_t data_len);
		static ranking_list delete_ranking_by_server_id(ranking_list ranking, int32 server_id);
		static ranking_list merge_ranking(ranking_list ranking1, ranking_list ranking2);
		static void c2ws_get_player_ranking_info_func(e_RankingIndex ranking_type, const guid_64& sender_guid, const guid_64& target_guid);
		static void send_operate_result_to_client(const guid_64& sender_guid, const int32& result_id);
		static void get_player_worship_value(const guid_64& sender_guid, const guid_64& target_guid);
		static void get_player_worship_value_end(bool is_load, const guid_64& sender_guid, const s_ranking_player_info& ranking_info);
		static ranking_list* get_ranking_list_by_type(e_RankingIndex ranking_index);
		static s_ranking_player_info* get_ranking_info_by_type(e_RankingIndex ranking_index, int32 begin_index, int32 end_index);
		static bool is_legion_ranking(e_RankingIndex ranking_index);
		static void cs2ws_sync_player_vip_level_func(const guid_64& role_guid,const int32& vip_level);
		static void change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
		static void change_ranking_legion_name_info(const guid_64& ranking_guid, const guid_64& legion_guid, const xstring& legion_name);
	public:		    				    
		static void sync_player_info(const s_ranking_player_info& player_info);
		static bool sync_player_info_insert(const s_ranking_player_info& player_info, ranking_list* ranking_ptr);
		static void sync_player_arena(s_ranking_player_info& my_player_info, s_ranking_player_info& target_player_info, bool is_robot);
		static void sync_after_merge_first_player_deal(s_ranking_player_info& my_player_info);
		static void sync_player_arena_lua(const char* my_str, int32 my_len, const char*dst_str, int32 dst_len, bool is_robot);
		static int32 insert_player_arena(s_ranking_player_info& player_info);
		static void	sync_player_arena_gs(const guid_64& role_guid, int64 gs_value);

		static void	save_worship_to_db(const guid_64& sender_guid,const s_ranking_player_info* target, int32& add_value, int32& worship_type);
		static void	create_robot_data();
	public:
		static void	cs2ws_req_add_worship_func(e_RankingIndex ranking_index, int32 worship_type, const guid_64& sender_guid, const guid_64& target_guid);
		static void	load_one_worship_ranking_from_db(const guid_64& sender, const int32& ranking_type, const int32& worship_add_value, const s_ranking_player_info& one_load_info, const int32& worship_type, const int32& worship_result);
		static void get_player_rank_level(int32 ranking_index, const guid_64& player_guid, int32& rank_level, int64& rank_value);
		static guid_64 get_ranking_player_guid(e_RankingIndex ranking_index, int32 ranking_pos);
		static int32 get_ranking_title_type(int32  ranking_type);
		static void first_ranking_player_online_func(const guid_64& role_guid, bool(&loading_title_flag)[faith::e_title_type_max]);

	public:
		static void sync_first_rank_player_info(const s_ranking_player_info& old_first_player_info, const s_ranking_player_info& new_first_player_info, bool is_notice = true);

		static void save_service_rank_list_to_db_by_type(e_service_rank_type service_rank_type);
		static void set_service_rank_map_by_type(e_service_rank_type service_rank_type);
		static void req_load_all_server_rank_from_db();
		static void load_all_server_rank_from_db(const s_service_rank_player_info* load_info, int32 data_num);
		static void set_service_rank_check_index(int32 service_rank_check_index);
		static int32 get_service_rank_check_index();

		static void send_service_rank_info_all_to_client(client_session* session);
		static void send_service_rank_info_one_to_client(client_session* session, int32 rank_num, int32 send_type);
		static void send_service_rank_first_player_to_client_by_type(client_session* session, int32 send_type);
		static int32 get_player_service_rank_index(const guid_64& role_guid, const int32& rank_type);
		static void send_avg_level_to_client(client_session* session);

		static void sync_oracle_trial_play_info(guid_64 play_guid, int32 rank_value);
		static void get_oracle_trial_rank_info(guid_64 play_guid);
		static void send_oracle_trial_rank_info(guid_64 play_guid, int32 server_id);
		static void send_get_oracle_trial_info();
		static void can_send_oracle_trial_notice(const guid_64& old_guid, const guid_64& play_guid);
		static void send_oracle_trial_notice(const xstring player_name, int32 server_id);
		static void send_oracle_trial_notice(const xstring player_name, const xstring legion_name, int32 server_id);
				    
		static void replace_cross_ladder_ranking();
				    
		static void replace_element_war_ranking();
		/************************************************************************/
		/*						根据排行榜计算平均等级				       	    */
		/************************************************************************/
	public:
		static int32 get_srv_avg_level();
		static int32 get_world_level_last();
		static int32 get_world_level_cur();
		static int32 calcu_srv_avg_level(int32 rank_min_index, int32 rank_max_index);
		static int32 calcu_all_server_avg_level();//跨服平均等级
		static void	sync_srv_avg_level_to_cs();
		static int32 get_rank_level_with_num(int32 num);	//根据传入num 获得等级排行榜前num的平均等级
		static void	send_world_srv_lv_info_all(int32 conn_index = -1);
		static void get_world_level_param(int32 &start, int32& end);
		static int32 get_world_level_for_now();
		static int32 get_world_level_for_old();
		static void	load_world_level_from_db(int32 lv_cur, int32 lv_last);
		static void	sync_calcu_world_level_tick_day();
		static int32 get_exp_server_begin_level_length_by_region();
		static int32 get_get_exp_server_begin_level(int32 offset);
	};
	
}
