/********************************************************************
  created: 2019年1月15日
  file base: cross_server_pk_def
  file ext: hpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#ifndef _CROSS_SERVER_PK_DEF_HPP_
#define _CROSS_SERVER_PK_DEF_HPP_

#include "char_def.hpp"
#include "game_enum_def_s.h"

namespace faith 
{
#pragma pack(push,1)
	const int32 max_island_player_num = 2;//一个小岛最大人数
	const int32 max_island_team_num = 8;//最大1v1对数
	const int32 max_1v1_player_num = 16;//16强

	const int32 max_branch_type = 4;
	const int32 max_cross_server_war_count = 15;//最大比赛记录数量（8+4+2+1）
	const int32 max_cross_server_all_class_war_count = 60;

	const int32 first_award_att_id = 113018001;//参与奖的ID（第一个奖励）

	const int32 title_best_barserker_temp_id = 39000026;//最强战士称号ID

	const int32 load_fake_player_time = 60;//load 雕像 间隔

	//enum e_cross_pk_state
	//{
	//	e_cross_pk_state_none = 0,
	//	e_cross_pk_state_16,
	//	e_cross_pk_state_8,
	//	e_cross_pk_state_4,
	//	e_cross_pk_state_2,
	//	e_cross_pk_state_max,
	//};

	//enum e_1v1_pk_state
	//{
	//	e_1v1_pk_state_ready = 0,
	//	e_1v1_pk_state_begin,
	//	e_1v1_pk_state_end,
	//	e_1v1_pk_state_max,
	//};

	enum e_pk_island_win_state
	{
		e_pk_island_win_state_win,//胜利
		e_pk_island_win_state_draw,//平手
		e_pk_island_win_state_lose,//失败
		e_pk_island_win_state_max
	};

	enum e_pk_map_id
	{
		e_pk_map_id_barserker = 71211103,//狂战
		e_pk_map_id_wizard,//法师
		e_pk_map_id_guardian,//骑士
		e_pk_map_id_assassinator,//刺客
	};

	struct s_pk_island_player_msg
	{
		guid_64		player_guid;
		s_map_pos	player_born_pos;
		int32		win_num;
		xchar		role_name[max_name_size + 1];
		int32		score;
		int32		server_id;
		s_pk_island_player_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_island_player_msg));
		}
		void set_role_name(xstring _name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _name.c_str(), _name.size() > sizeof(role_name) ? sizeof(role_name) : _name.size());
		}
		void set_map_pos(const s_map_pos map_pos)
		{
			player_born_pos = map_pos;
		}
	};

	struct s_pk_island_simple_player_msg
	{
		guid_64		player_guid;
		xchar		role_name[max_name_size + 1];
		int32		win_num;
		int32		server_id;
		int32		is_winner;
		s_pk_island_simple_player_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_island_simple_player_msg));
		}
		void set_role_name(xstring _name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _name.c_str(), _name.size() > sizeof(role_name) ? sizeof(role_name) : _name.size());
		}
	};

	struct s_pk_island_simple_msg
	{
		s_pk_island_simple_player_msg	island_player_msg[max_island_player_num];
		int32							pk_state;
		int32							war_idex;
		s_pk_island_simple_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_island_simple_msg));
		}
		bool is_vaild()
		{
			return island_player_msg[0].player_guid.is_valid();
		}
	};

	struct s_pk_island_all_msg
	{
		s_pk_island_simple_msg	island_msg[max_cross_server_all_class_war_count];
		s_pk_island_all_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_island_all_msg));
		}
		bool is_vaild()
		{
			return island_msg[0].island_player_msg[0].player_guid.is_valid();
		}
	};

	struct s_dp2ws_island_simple_msg
	{
		int32	war_idex;
		guid_64	winner_player_guid;
		int32	island_pk_state;
		xchar	winner_player_name[max_name_size + 1];
		int32	winner_player_win_num;
		int32	winner_player_server_id;
		int32	one_player_is_winner;
		guid_64	lose_player_guid;
		xchar	lose_player_name[max_name_size + 1];
		int32	lose_player_win_num;
		int32	lose_player_server_id;
		int32	other_player_is_winner;
		s_dp2ws_island_simple_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_dp2ws_island_simple_msg));
		}
		bool is_vaild()
		{
			return winner_player_guid.is_valid();
		}
	};

	struct s_pk_player_msg
	{
		int32		server_id;
		guid_64		role_guid;
		s_pk_player_msg()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_player_msg));
		}
	};
#pragma pack(pop)
}
#endif
