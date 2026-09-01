#ifndef _BROKEN_SKY_MGR_WS_H_
#define _BROKEN_SKY_MGR_WS_H_

#include "Logic/map_def.hpp"
#include "utility/score_indicator.h"

namespace faith
{
	enum e_error_code;
	struct ActivityCommonConfigTemplate;

	enum e_broken_sky_state
	{
		e_broken_sky_not_start, //游戏未开始
		e_broken_sky_in_join,	//准备时间
		e_broken_sky_in_game,	//游戏时间
		e_broken_sky_game_end,	//游戏结束
	};

	class broken_sky_mgr
	{
	public:
		static broken_sky_mgr& get_instance()
		{
			static broken_sky_mgr instance;
			return instance;
		}

		explicit broken_sky_mgr();

		void				tick(const int64& new_time);
		void				tick_3sec();
		void				clear_data();

		void				sync_game_info_to_all_map_inst(int32 map_id, guid_64 from_map_inst_guid, int64 damage, const score_record* top_record_arr, int32 record_num);
		void				sync_kill_boss_to_all_map_inst(int32 map_id, int32 killer_unit_index, guid_64 killer_guid);
		void				sync_creat_boss_to_cs(guid_64 from_map_guid);

		bool				is_broken_sky_map_id(int32 map_id);
		bool				get_game_over_flag();
		void				calcu_boss_level();
		int32				get_boss_level();
		void				set_boss_level(int32 boss_level);

		e_error_code		is_can_join();

		//rename
		void				change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
	private:
		void				send_message_to_all_cs(const void* data_ptr, size_t data_len);
		void				send_top_records();
		bool				is_in_join_time();
		bool				is_in_game_time();
		void				compare_and_update_top_records(int32 map_id, const score_record* top_record_arr, int32 record_num);
		void				send_settlement_to_all_player();
		void				send_top_player_notice(const guid_64& target_guid, const xstring& target_name);
		void				give_rank_rewards(guid_64 player_guid, int32 player_rank);
	private:
		bool							m_game_over_flag;
		bool							m_is_boss_dead;
		float							m_tick_3sec;
		e_broken_sky_state				m_game_state;

		ActivityCommonConfigTemplate*	m_brokensky_config_ptr;

		score_indicator					m_damage_rank_indicator;

		int32							m_boss_level;

		int64							m_all_damage;
	};
}

#endif
