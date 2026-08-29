/********************************************************************
created: 2016/08/24
file base: arena
file ext: hpp
author: zhangminghai

purpose:
*********************************************************************/
#ifndef _FAITH_ARENA_HPP_
#define _FAITH_ARENA_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/arena_def.hpp"
#include "Logic/buff_def.hpp"
#include "Logic/spirit_def.hpp"
#include "Logic/belief_def.hpp"
#include "att_msg.hpp"
#include "skill_msg.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_load_arena_rank = faith::e_msg_base_arena,
		e_msgindex_dp2ws_load_arena_rank,

		e_msgindex_ws2dp_save_arena_rank,
		e_msgindex_dp2ws_save_arena_rank,

		e_msgindex_ws2dp_save_arena_rank_change,

		e_msgindex_cs2dp_add_arena_log,

		e_msgindex_cs2dp_load_arena_log,
		e_msgindex_dp2cs_resp_load_arena_log,

		e_msgindex_dp2cs_load_arena_data_error,

		e_msgindex_ws2dp_fresh_all_rank,
	
		e_msgindex_dp2cs_arena_load_char_error,
		e_msgindex_dp2cs_arena_load_char_info,
		e_msgindex_dp2cs_arena_load_char_att,
		e_msgindex_dp2cs_arena_load_char_skill,
		e_msgindex_dp2cs_arena_load_char_item,
		e_msgindex_dp2cs_arena_load_char_spirit,
		e_msgindex_dp2cs_arena_load_char_fight_att,
		e_msgindex_dp2cs_arena_load_char_buff,
		e_msgindex_dp2cs_arena_load_char_belief,

		e_msgindex_dp2ws_create_robot,
		e_msgindex_dp2ws_load_finish

	};

	// 请求读取
	struct ws2dp_load_arena_rank : public faith::packet_base
	{
		ws2dp_load_arena_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_arena_rank;
		}
	};


	//DP2ws 读取所有竞技场排名
	struct dp2ws_load_arena_rank : public faith::packet_base
	{
		int32							start_index;
		int32							data_num;
		s_arena_rank					load_data[rank_save_per_time_max];
		dp2ws_load_arena_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_arena_rank;
		}
		uint32 get_pak_length() const
		{
			int32 basic_len = (ULONG_PTR)&load_data - (ULONG_PTR)&wheader;
			return basic_len + data_num * sizeof(s_arena_rank);
		}
	};

	struct dp2ws_create_robot : public faith::packet_base
	{
		dp2ws_create_robot()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_create_robot;
		}
	};

	struct dp2ws_arena_load_finish : public faith::packet_base
	{
		dp2ws_arena_load_finish()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_finish;
		}
	};


	//ws2dp	请求存储排名信息
	struct ws2dp_save_arena_rank : public faith::packet_base
	{
		bool								is_first;
		s_arena_rank						data_list[rank_save_per_time_max];
		ws2dp_save_arena_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_arena_rank;
		}
		void clear_data_array()
		{
			memset(data_list, 0, sizeof(s_arena_rank) * rank_save_per_time_max);
		}
		bool to_proto(faith::server2dp_proto::ws2dp_save_arena_rank& msg)
		{
			msg.set_is_first(is_first);
			for (int32  i = 0; i < rank_save_per_time_max; i++)
			{
				faith::server2dp_proto::s_arena_rank* arena_rank_ptr = msg.add_data_list();
				if (arena_rank_ptr == nullptr)
				{
					return false;
				}
				data_list[i].to_proto(arena_rank_ptr);
			}
			return true;
		}
		void from_proto(const faith::server2dp_proto::ws2dp_save_arena_rank& msg)
		{
			is_first = msg.is_first();
			for (int32 i = 0; i < rank_save_per_time_max && i < msg.data_list_size(); i++)
			{
				data_list[i].from_proto(msg.data_list(i));
			}
		}
	};

	struct ws2dp_save_arena_rank_change : public faith::packet_base
	{
		s_arena_rank						my_new_data;
		s_arena_rank						target_new_data;
		ws2dp_save_arena_rank_change()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_arena_rank_change;
		}
	};



	//cs2dp 请求添加挑战记录
	struct cs2dp_add_arena_log : public faith::packet_base
	{
		s_chanllenge_log_info   log_data;
		int32					data_num;
		cs2dp_add_arena_log() 
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_add_arena_log;
		}
		uint32 get_pak_length() const
		{
			int32 basic_len = wheader;
			return (basic_len + data_num * sizeof(s_chanllenge_log_info));
		}
	};

	//cs2dp 请求读取挑战记录
	struct cs2dp_load_arena_log : public faith::packet_base
	{
		guid_64		role_guid;
		int32		array_index;
		cs2dp_load_arena_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_load_arena_log;
		}
	};
	//dp2cs响应读取记录
	struct dp2cs_resp_load_arena_log : public faith::packet_base
	{
		guid_64		role_guid;
		int32		array_index;
		int32		data_num;
		s_chanllenge_log_info role_challenge_log[challenge_log_max];
		dp2cs_resp_load_arena_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_resp_load_arena_log;
		}
		void clear_data()
		{
			role_guid.clear_data();
			array_index = 0;
			data_num = 0;
			g_clear_data(role_challenge_log, challenge_log_max);
		}
	};


	struct dp2cs_load_arena_data_error : public faith::packet_base
	{
		guid_64		role_guid;
		int32		error_code;
		int32		error_result;
		dp2cs_load_arena_data_error()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_arena_data_error;
		}
	};

	//ws2dp	请求更新所有排名信息
	struct ws2dp_fresh_all_rank : public faith::packet_base
	{
		ws2dp_fresh_all_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_fresh_all_rank;
		}
	};
	
	struct dp2cs_load_arena_char_error : public faith::packet_base
	{
		guid_64		role_guid;
		guid_64		map_guid;
		int64		error_code;
		int32		error_result;
		dp2cs_load_arena_char_error()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_error;
		}
	};

	struct dp2cs_arena_load_char_info : public packet_base
	{
		guid_64 map_guid;
		s_unit_info role_info;

		dp2cs_arena_load_char_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_info;
		}
	};

	struct dp2cs_arena_load_char_att : public dp2cs_load_char_att
	{
		guid_64 map_guid;
		dp2cs_arena_load_char_att()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_att;
		}
	};

	struct dp2cs_arena_load_char_skill : public packet_base
	{
		guid_64 map_guid;
		guid_64	role_guid;
		int32 data_num;
		s_skill_info skill_data[MAX_SKILL_NUM];

		dp2cs_arena_load_char_skill()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_skill;
		}
		void clear_data()
		{
			map_guid.clear_data();
			role_guid.clear_data();
			data_num = 0;
			g_clear_data(skill_data, MAX_SKILL_NUM);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&skill_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_skill_info));
		}
	};

	struct dp2cs_arena_load_char_item : public faith::packet_base
	{
		guid_64 map_guid;
		guid_64 role_guid;
		int32 data_num;
		s_item_info item_data[fake_item_num];

		dp2cs_arena_load_char_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_item;
		}
		void clear_data()
		{
			map_guid.clear_data();
			role_guid.clear_data();
			data_num = 0;
			g_clear_data(item_data, fake_item_num);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&item_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_item_info));
		}
	};

	struct dp2cs_arena_load_char_spirit : public faith::packet_base
	{
		guid_64			map_guid;
		guid_64			role_guid;
		s_spirit_info	spirit_data;
		bool			is_has_fighting_spirit;
		dp2cs_arena_load_char_spirit()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_spirit;
		}
	};

	struct dp2cs_arena_load_char_fight_att : public faith::packet_base
	{
		guid_64					map_guid;
		s_arena_char_fight_att	m_fight_att;

		dp2cs_arena_load_char_fight_att()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_fight_att;
		}
	};

	struct dp2cs_arena_load_char_buff : public faith::packet_base
	{
		guid_64			map_guid;
		uint32			data_num;
		s_buff_info		buff_data[MAN_UNIT_BUFF_INST];

		dp2cs_arena_load_char_buff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_buff;
		}
		void clear_data()
		{
			map_guid.clear_data();
			data_num = 0;
			g_clear_data(buff_data, MAN_UNIT_BUFF_INST);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&buff_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_buff_info));
		}
	};
	struct dp2cs_arena_load_char_belief : public faith::packet_base
	{
		guid_64 map_guid;
		guid_64 role_guid;
		int32 data_num;
		s_belief_info belief_data[max_item_num];

		dp2cs_arena_load_char_belief()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_arena_load_char_belief;
		}
		void clear_data()
		{
			map_guid.clear_data();
			role_guid.clear_data();
			data_num = 0;
			g_clear_data(belief_data, max_item_num);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&belief_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_belief_info));
		}
	};

#pragma pack(pop)
}

#endif