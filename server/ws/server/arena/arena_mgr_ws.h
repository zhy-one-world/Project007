/********************************************************************
created: 2016年8月23日14:56:28
file base: arena_mgr_ws
file ext: hpp
author: zhangminghai

purpose: about arena system's arena_mgr
*********************************************************************/
#ifndef _ARENA_MGR_WS_H_
#define _ARENA_MGR_WS_H_

#include "server/client_session_mgr.hpp"
#include "Logic/arena_def.hpp"
#include "Logic/special_name_def.hpp"
#include "Internal/char_msg.hpp"

namespace faith
{
	const int32 k_rand_name_dic_chinese_male_first_begin = 90086001;
	const int32 k_rand_name_dic_chinese_male_first_end = 90086142;
	const int32 k_rand_name_dic_chinese_male_second_begin = 90085001;
	const int32 k_rand_name_dic_chinese_male_second_end = 90085084;

	const int32 k_rand_name_dic_chinese_female_first_begin = 90088001;
	const int32 k_rand_name_dic_chinese_female_first_end = 90088328;
	const int32 k_rand_name_dic_chinese_female_second_begin = 90087001;
	const int32 k_rand_name_dic_chinese_female_second_end = 90087090;


	const int32 k_rand_name_dic_no_chinese_male_first_begin = 90082001;
	const int32 k_rand_name_dic_no_chinese_male_first_end = 90082982;
	const int32 k_rand_name_dic_no_chinese_male_second_begin = 90083001;
	const int32 k_rand_name_dic_no_chinese_male_second_end = 90083105;

	const int32 k_rand_name_dic_no_chinese_female_first_begin = 90082001;
	const int32 k_rand_name_dic_no_chinese_female_first_end = 90082982;
	const int32 k_rand_name_dic_no_chinese_female_second_begin = 90084001;
	const int32 k_rand_name_dic_no_chinese_female_second_end = 90084312;

	class  arena_mgr_ws
	{
	public:
		arena_mgr_ws() {};
		~arena_mgr_ws() {};
	public:
		static arena_mgr_ws& get_instance()
		{
			static arena_mgr_ws s_arena_mgr;
			return s_arena_mgr;
		}
	public:

		void							init_manager();	//初始化管理
		void							clear_data();

		void							heart_tick(int32 elapse_time);

		void							req_load_ranks_from_db();

		void							create_robot();
		void							create_robot_one(ArenaRobotTemplate* robot_template_ptr);
		void							load_ranks_from_db(const s_arena_rank* arena_rank_info, int32 start_index, int32 data_num);
		void							fill_empty_pos();

		void							save_ranks_to_db();
		void							save_change_rank_result(const player_arena_rank& my_new_info, const player_arena_rank& target_new_info);

		void							req_challenge(guid_64 challenge_role_guid, guid_64 target_role_guid, int32 cost_money_type, int32 cost_money_num, int32 target_cur_pos, int32 self_old_pos);
		int32							get_req_result(player_arena_rank* target_ptr, player_arena_rank* challenge_ptr, guid_64 target_guid, guid_64 challenge_guid, int32 challenge_old_pos);

		void							add_challeng_log(const guid_64& challenge_role_guid, const guid_64& target_role_guid, bool initiative, bool result);

		void							get_choose_list(const guid_64& role_guid, const int32& role_pos);

		int32                           get_area_index(int32  start, int32 end, int64 cur_time, int32& is_cd);


		void							get_first_three(guid_64 role_guid);
		
		int32							change_rank(const player_arena_rank& challenge_info, const guid_64& target_guid, const int32 target_index);
		void							sync_gs_value(const guid_64& role_guid, const int32& temp_pos, const int64& new_gs);

		int32							get_player_current_index(const guid_64& role_guid);

		void							req_get_player_rank(const guid_64& role_guid);

		void							sync_player_gs(const guid_64& role_guid, int64 gs_value, int32 role_rank);
		void							sync_player_arena_military_rank(const guid_64& role_guid, int64 military_level, int32 role_rank);
		void							sync_first_rank_player_info(const player_arena_rank& old_first_player_info, const player_arena_rank& new_first_player_info);


		player_arena_rank*				get_player_arena(const guid_64& role_guid);

		player_arena_rank*				get_data_by_index(const int32& index);

		void							reset_challenge_state(guid_64 self_guid, guid_64 target_guid, int32 target_rank, int32 self_rank);

		//rename
		void							change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步

		bool                            is_use_lua();
	private:
		player_arena_rank				m_arena_rank[rank_max];

		int64							m_sec_tick = 0;
	};

}
#endif
