#pragma once

#include "logic/type_def.hpp"
#include "Logic/map_def.hpp"
#include "Logic/cross_server_pk_def.hpp"
#include "map_object.h"

namespace faith
{

	class cross_arena_island
	{
	public:
		void				clear_data();
		void				set_island_msg(score_record& cur_score, s_map_pos born_pos, int32 score);
		void				tick(const int64& new_time);

		bool				get_is_in_the_war() { return m_is_in_the_war; };
		void				set_is_in_the_war(bool is_in_war) { m_is_in_the_war = is_in_war; };

		void				begin_war();//开始擂台赛

		int32				set_win_player(guid_64 player_guid);

		s_map_pos			get_player_born_pos(guid_64 player_guid);

		guid_64				get_winner_player_guid();
		xchar*				get_winner_player_name();
		void				set_winner_player_guid();
		guid_64				get_lose_player_guid();
		xchar*				get_lose_player_name();

		bool				is_vaild();

		void				set_pk_state(e_cross_pk_state pk_state);

		void				end_war();

		void				transfer_player_to_born_pos();//传送玩家到战斗区域

		void				set_map_ptr(map_object* map_ptr) { m_cur_map_ptr = map_ptr; };

		void				set_pk_type(e_pk_mode_type mode_type, e_pk_mode pk_mode);

		int32				reset_war(bool is_time_out = false);//重置某一场

		void				start_pk();//开始擂台赛的某一场

		void				end_one_war();//结束一小场

		void				fill_simple_msg(s_pk_island_simple_msg& simple_msg);

		s_pk_island_player_msg				get_player_msg_for_guid(guid_64 role_guid);

		void				send_client_map_state();

		void				set_island_idex(int32 island_idex) { m_island_idex = island_idex; };

		void				send_msg_to_island_player(google::protobuf::Message* net_pro, uint32 header);

		void				set_cur_island_class_type(int32 class_type) { m_cur_island_class_type = class_type; };

		int32				get_cur_island_war_idex();

		bool				check_is_need_end_war();

		void				reset_player_state();

		void				send_ws_msg_time_out();//时间到 平局 也发一次信息同步到WS

		void				send_award_to_winner_player();

		void				transfer_player_to_main_island();

		void				send_winner_notice();

		int32				get_is_pk_count() { return m_is_pk_count; };

		bool				not_have_other_one();
	private:
		s_pk_island_player_msg m_island_player_msg[max_island_player_num];

		bool			m_is_in_the_war;
		ActivityCommonConfigTemplate*	m_pk_config_ptr;

		e_cross_pk_state		m_pk_state;

		int32			m_need_winer_num;//需要多少场才算获胜

		int32			m_is_pk_count;//已经进行了多少场PK

		int32			m_small_free_time;//休息小间隔时长
		int32			m_war_time;//战斗时长

		int64			m_next_begin_war_time;//下次开始战斗的时间
		int64			m_next_ready_war_time;//下次准备战斗的时间
		int64			m_end_war_transfer_time;//结束传送时间

		map_object*		m_cur_map_ptr;

		e_1v1_pk_state	m_1v1_pk_state;

		guid_64			m_winner_player_guid;

		int32			m_island_idex;//几号岛屿，方便操控客户端对应空气墙

		int32			m_cur_island_class_type;
	};
}
