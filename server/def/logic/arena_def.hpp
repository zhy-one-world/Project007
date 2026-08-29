/********************************************************************
created: 2016/08/23
created: 4:7:2016 15:07
file base: arena_def
file ext: hpp
author: zhangminghai

purpose:
*********************************************************************/
#ifndef _ARENA_DEF_HPP_
#define _ARENA_DEF_HPP_
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "login_def.hpp"
#include "utility/cs_date.hpp"
//#include "template/template_manager.h"


namespace faith
{
#pragma pack(push,1)

	const int32 rank_max = 5000;
	const int32 rank_save_per_time_max = 500;
	const int32	challenge_log_max = 100;//最大保存100条记录
	const int32	arena_choose_list_base_range = 300;
	const int32	arena_map_template_id = 71100004;
	const int32	arena_reward_template_start_id = 109000001;
	const int32	arena_not_in_list_rank = -1;
	const int32	arena_robot_template_start_id = 110000001;
	const int32 areaa_cloose_second_list_base_range = 600;

	enum e_req_challenge_end
	{
		e_req_challenge_end_success = 0,			//发送请求成功
		e_req_challenge_end_play_not_exist,			//玩家不存在
		e_req_challenge_end_times_lack,				//次数不足
		e_req_challenge_end_colling_time,			//挑战冷却中
		e_req_challenge_end_target_is_in_challenge,	//玩家正在被其他玩家挑战
		e_req_challenge_end_max,
	};


	struct arena_player_info
	{

		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int64		gs_value;
		int32		role_template_id;

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > sizeof(role_name) ? sizeof(role_name) : p_role_name.size());
		}

		arena_player_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct  player_arena_rank
	{
		int32		rank_pos;
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int64		gs_value;
		int32		role_template_id;
		int64		challenge_end_time;
		int32		robot_template_id;
		int32		military_level;
		int32		vip_level;
		xchar		game_channel[e_login_fixed_data_lenght + 1];
		player_arena_rank()
		{
			clear_data();
		};	
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
		void set_game_channel(xstring p_game_channel)
		{
			memset(game_channel, 0, sizeof(game_channel));
			memcpy(game_channel, p_game_channel.c_str(), p_game_channel.size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : p_game_channel.size());
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		bool to_proto(faith::st_proto::st_player_arena_rank * st_arena_ptr)
		{
			if (st_arena_ptr == nullptr)
			{
				return false;
			}
			st_arena_ptr->set_rank_pos(rank_pos);
			st_arena_ptr->set_role_guid(role_guid.server_64);
			st_arena_ptr->set_role_name(role_name);
			st_arena_ptr->set_gs_value(gs_value);
			st_arena_ptr->set_role_template_id(role_template_id);
			st_arena_ptr->set_challenge_end_time(challenge_end_time);
			st_arena_ptr->set_robot_template_id(robot_template_id);
			st_arena_ptr->set_military_level(military_level);
			st_arena_ptr->set_vip_level(vip_level);
			st_arena_ptr->set_game_channel(game_channel);
			return true;
		}
		void from_proto(const faith::st_proto::st_player_arena_rank& st_arena_ref)
		{
			rank_pos = st_arena_ref.rank_pos();
			role_guid.server_64 = st_arena_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, st_arena_ref.role_name());
			gs_value = st_arena_ref.gs_value();
			role_template_id = st_arena_ref.role_template_id();
			challenge_end_time = st_arena_ref.challenge_end_time();
			robot_template_id = st_arena_ref.robot_template_id();
			military_level = st_arena_ref.military_level();
			vip_level = st_arena_ref.vip_level();
			my_memcopy_string(game_channel, e_login_fixed_data_lenght, st_arena_ref.game_channel());
		}
	};


	struct s_arena_rank
	{
		int32		rank_pos;
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int64		gs_value;
		int32		role_template_id;
		int32		robot_template_id;
		int32		military_rank_level;
		int32		vip_level;
		xchar		game_channel[e_login_fixed_data_lenght + 1];

		s_arena_rank()
		{
			clear_data();
		};
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > sizeof(role_name) ? sizeof(role_name) : p_role_name.size());
		}
		void set_game_channel(xstring p_game_channel)
		{
			memset(game_channel, 0, sizeof(game_channel));
			memcpy(game_channel, p_game_channel.c_str(), p_game_channel.size() > e_login_fixed_data_lenght ? e_login_fixed_data_lenght : p_game_channel.size());
		}
		void to_proto(faith::server2dp_proto::s_arena_rank* arena_rank_ptr)
		{
			arena_rank_ptr->set_rank_pos(rank_pos);
			arena_rank_ptr->set_role_guid(role_guid.server_64);
			arena_rank_ptr->set_role_name(role_name);
			arena_rank_ptr->set_gs_value(gs_value);
			arena_rank_ptr->set_role_template_id(role_template_id);
			arena_rank_ptr->set_robot_template_id(robot_template_id);
			arena_rank_ptr->set_military_rank_level(military_rank_level);
			arena_rank_ptr->set_vip_level(vip_level);
			arena_rank_ptr->set_game_channel(game_channel);
		}

		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::server2dp_proto::s_arena_rank& arena_rank_ref)
		{
			rank_pos = arena_rank_ref.rank_pos();
			role_guid.server_64 = arena_rank_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, arena_rank_ref.role_name());
			gs_value = arena_rank_ref.gs_value();
			role_template_id = arena_rank_ref.role_template_id();
			robot_template_id = arena_rank_ref.robot_template_id();
			military_rank_level = arena_rank_ref.military_rank_level();
			vip_level = arena_rank_ref.vip_level();
			my_memcopy_string(game_channel, e_login_fixed_data_lenght, arena_rank_ref.game_channel());
		}
	};




	struct s_chanllenge_log_info
	{
		int32   log_id;
		guid_64 role_guid;
		xchar	role_name[max_name_size + 1];
		int32   role_rank;
		guid_64 target_guid;
		xchar	target_name[max_name_size + 1];
		int32	target_rank;
		int32	initiative;
		int32	result;
		int32	date;

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > sizeof(role_name) ? sizeof(role_name) : p_role_name.size());
		}

		void set_target_name(xstring p_target_name)
		{
			memset(target_name, 0, sizeof(target_name));
			memcpy(target_name, p_target_name.c_str(), p_target_name.size() > sizeof(role_name) ? sizeof(role_name) : p_target_name.size());
		}

		s_chanllenge_log_info()
		{
			memset(this, 0, sizeof(*this));
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};


	struct s_chanllenge_log_info_db
	{
		guid_64		role_guid;
		s_chanllenge_log_info data_info;
		s_chanllenge_log_info_db() {};
	};

	struct s_arena_char_fight_att
	{
		guid_64		role_guid;
		double		att_value[e_unit_attack_att_max];

		s_arena_char_fight_att()
		{
			memset(this, 0, sizeof(*this));
		}
		bool to_proto(faith::st_proto::st_arena_char_fight_att* st_arean_ptr)
		{
			if (st_arean_ptr == nullptr)
			{
				return false;
			}
			st_arean_ptr->set_role_guid(role_guid.server_64);
			for (int32 i = 0; i < e_unit_attack_att_max; i++)
			{
				st_arean_ptr->add_att_value(att_value[i]);
			}
			return true;
		}
		void from_proto(const faith::st_proto::st_arena_char_fight_att& st_arean_ref)
		{
			role_guid.server_64 = st_arean_ref.role_guid();
			for (int32 i = 0; i < e_unit_attack_att_max && i < st_arean_ref.att_value_size(); i++)
			{
				att_value[i] = st_arean_ref.att_value(i);
			}
		}
	};

#pragma pack(pop)
}
#endif