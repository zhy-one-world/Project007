/********************************************************************
created: 2019年4月4日
file base: cross_server_harry_msg
file ext: hpp
author: zhangshuo

purpose: 跨服PK
*********************************************************************/

#ifndef _FAITH_CROSS_SERVER_HARRY_MSG_HPP_
#define _FAITH_CROSS_SERVER_HARRY_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/cross_server_harry_def.hpp"
#include "Logic/fake_player_def.hpp"
#include "internet/ws2cs.pb.h"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_ws2dp_save_cross_server_harry_info = faith::e_msg_base_cross_server_harry,
		e_msgindex_ws2dp_load_cross_server_harry_info,
		e_msgindex_dp2ws_load_cross_server_harry_info,
		e_msgindex_ws2ws_send_cross_server_harry_info,
		e_msgindex_ws2ws_send_cross_server_harry_one,
		e_msgindex_ws2ws_send_cross_server_harry_player_info,
		e_msgindex_cs2dp_save_role_harry_info,
		e_msgindex_dp2cs_load_role_harry_info,
		e_msgindex_ws2cs_send_cur_server_harry_info,
		e_msgindex_cs2ws_send_change_server_money,
		e_msgindex_ws2cs_give_player_defend_rank_award,
		e_msgindex_ws2ws_send_server_harry_rank,
		e_msgindex_ws2dp_load_harry_player_detail,
		e_msgindex_dp2ws_load_harry_player_base_info_end,
		e_msgindex_dp2ws_load_harry_player_att_info_end,
		e_msgindex_dp2ws_load_harry_player_fight_att_info_end,
		e_msgindex_dp2ws_load_harry_player_skill_info_end,
		e_msgindex_dp2ws_load_harry_player_item_info_end,
		e_msgindex_dp2ws_load_harry_player_buff_end,
		e_msgindex_dp2ws_load_harry_player_belief_end,
		e_msgindex_cs2ws_send_create_harry_player,
		e_msgindex_ws2cs_send_create_normal_player,
		e_msgindex_ws2cs_send_create_first_harry_player,
		e_msgindex_ws2cs_send_clear_harry_player,
		e_msgindex_cs2ws_send_harry_notice_to_all_player,
		e_msgindex_ws2dp_send_del_one_server_msg,
	};

	struct ws2dp_save_cross_server_harry_msg_all : public faith::packet_base
	{
		s_server_harry_msg	server_harry_msg;
		ws2dp_save_cross_server_harry_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_cross_server_harry_info;
		}
	};

	struct ws2dp_load_cross_server_harry_msg_all : public faith::packet_base
	{
		ws2dp_load_cross_server_harry_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_cross_server_harry_info;
		}
	};

	struct ws2dp_del_one_server_msg : public faith::packet_base
	{
		int32 server_id;
		ws2dp_del_one_server_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_send_del_one_server_msg;
		}
	};

	struct dp2ws_load_cross_server_harry_msg_all : public faith::packet_base
	{
		bool is_empty_msg;
		int32 data_num;
		s_server_harry_msg	harry_msg[dp_save_max_server_num];
		dp2ws_load_cross_server_harry_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_cross_server_harry_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&harry_msg - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_server_harry_msg));
		}
	};

	struct ws2ws_send_cross_server_harry_msg_all : public faith::packet_base
	{
		s_server_harry_msg	harry_msg[max_server_num];
		int32 data_num;
		bool is_need_set_name;
		ws2ws_send_cross_server_harry_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_cross_server_harry_info;
		}
	};

	struct ws2ws_send_cross_server_harry_msg_one : public faith::packet_base
	{
		s_harry_money_change	harry_msg[max_server_num];
		int32					data_num;
		ws2ws_send_cross_server_harry_msg_one()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_cross_server_harry_one;
		}
	};

	struct ws2ws_send_server_harry_player_msg : public faith::packet_base
	{
		int32			server_id;
		int64			player_guid_arr[3];
		xchar			player_name[max_name_size + 1];
		ws2ws_send_server_harry_player_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_cross_server_harry_player_info;
		}
	};

	struct ws2ws_send_server_harry_award : public faith::packet_base
	{
		int32			server_rank;
		ws2ws_send_server_harry_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_server_harry_rank;
		}
	};

	struct cs2dp_save_role_harry_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_role_harry_info					info_data;
		cs2dp_save_role_harry_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_role_harry_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&info_data - (ULONG_PTR)&wheader;
			return (basic_len + sizeof(s_role_harry_info));
		}
	};

	struct dp2cs_load_role_harry_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		s_db_role_harry_info				info_data;
		dp2cs_load_role_harry_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_role_harry_info;
		}
	};

	struct ws2cs_send_cur_server_harry_msg : public packet_base
	{
		s_server_harry_msg			server_harry_msg[max_server_num];
		ws2cs_send_cur_server_harry_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_cur_server_harry_info;
		}
		bool to_proto(faith::ws2cs_proto::send_cur_server_harry_msg& msg)
		{
			for (int32 i = 0; i < max_server_num; i++)
			{
				faith::st_proto::st_server_harry_msg* st_harry_ptr = msg.add_server_harry_msg();
				if (st_harry_ptr == nullptr)
				{
					return false;
				}
				server_harry_msg[i].to_proto(st_harry_ptr);
			}
			return true;
		}
		void from_proto(const faith::ws2cs_proto::send_cur_server_harry_msg& msg)
		{
			for (int32 i = 0; i < max_server_num && i < msg.server_harry_msg_size(); i++)
			{
				server_harry_msg[i].from_proto(msg.server_harry_msg(i));
			}
		}
	};

	struct cs2ws_send_change_server_money : public packet_base
	{
		bool			is_special_harry;
		int32			change_value;
		int32			server_id;
		cs2ws_send_change_server_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_change_server_money;
		}
	};

	struct cs2ws_send_harry_notice_to_all_player : public packet_base
	{
		int32			string_id;
		cs2ws_send_harry_notice_to_all_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_harry_notice_to_all_player;
		}
	};

	struct ws2dp_load_harry_player_detail : public packet_base
	{
		int32			player_idex;
		guid_64			role_guid;
		int32			server_id;
		ws2dp_load_harry_player_detail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_harry_player_detail;
		}
	};

	struct dp2ws_load_harry_player_base_info_end : public packet_base
	{
		guid_64 role_guid;
		s_unit_info role_info;
		int32 big_type;

		dp2ws_load_harry_player_base_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_base_info_end;
		}
	};

	struct dp2ws_load_harry_player_att_info_end : public packet_base
	{
		guid_64 role_guid;
		s_base_att_info att_info;
		int32 big_type;

		dp2ws_load_harry_player_att_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_att_info_end;
		}
	};

	struct dp2ws_load_harry_player_fight_att_info_end : public packet_base
	{
		guid_64 role_guid;
		s_arena_char_fight_att att_info;
		int32 big_type;

		dp2ws_load_harry_player_fight_att_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_fight_att_info_end;
		}
	};

	struct dp2ws_load_harry_player_skill_info_end : public packet_base
	{
		guid_64 role_guid;
		int32 big_type;
		s_skill_info skill_data[MAX_SKILL_NUM];
		dp2ws_load_harry_player_skill_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_skill_info_end;
		}
	};

	struct dp2ws_load_harry_player_item_info_end : public packet_base
	{
		guid_64 role_guid;
		int32 big_type;
		s_item_info item_data[role_show_item_num];
		dp2ws_load_harry_player_item_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_item_info_end;
		}
	};

	struct dp2ws_load_harry_player_buff_end : public packet_base
	{
		guid_64			role_guid;
		int32			big_type;
		uint32			data_num;
		s_buff_info		buff_data[MAN_UNIT_BUFF_INST];
		dp2ws_load_harry_player_buff_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_buff_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&buff_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_buff_info));
		}
	};

	struct dp2ws_load_harry_player_belief_end : public packet_base
	{
		guid_64			role_guid;
		int32			big_type;
		int32			data_num;
		s_belief_info	data_list[e_belief_series_type_max];
		dp2ws_load_harry_player_belief_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_harry_player_belief_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_belief_info));
		}
	};

	struct cs2ws_send_create_harry_player : public packet_base
	{
		bool			need_create;
		int32			map_type;
		cs2ws_send_create_harry_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_create_harry_player;
		}
	};

	struct ws2cs_send_create_normal_player : public packet_base
	{
		int32			crate_num;
		ws2cs_send_create_normal_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_create_normal_player;
		}
		void to_proto(faith::ws2cs_proto::send_create_normal_player& msg)
		{
			msg.set_crate_num(crate_num);
		}
		void from_proto(const faith::ws2cs_proto::send_create_normal_player& msg)
		{
			crate_num = msg.crate_num();
		}
	};

	struct ws2cs_create_harry_player_statue : public packet_base
	{
		s_fake_player_info				big_player_info;
		int32							big_type;
		guid_64							map_guid;
		ws2cs_create_harry_player_statue()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_create_first_harry_player;
		}
		bool to_proto(faith::ws2cs_proto::create_harry_player_statue& msg)
		{
			faith::st_proto::st_fake_player_info *st_fake_ptr = msg.mutable_big_player_info();
			if (st_fake_ptr == nullptr)
			{
				return false;
			}
			big_player_info.to_proto(st_fake_ptr);
			msg.set_big_type(big_type);
			msg.set_map_guid(map_guid.server_64);
			return true;
		}
		void from_proto(const faith::ws2cs_proto::create_harry_player_statue& msg)
		{
			big_player_info.from_proto(msg.big_player_info());
			big_type = msg.big_type();
			map_guid.server_64 = msg.map_guid();
		}
	};

	struct ws2cs_claer_all_harry_player : public packet_base
	{
		bool							need_clear;
		ws2cs_claer_all_harry_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_clear_harry_player;
		}
		void to_proto(faith::ws2cs_proto::claer_all_harry_player& msg)
		{
			msg.set_need_clear(need_clear);

		}
		void from_proto(const faith::ws2cs_proto::claer_all_harry_player& msg)
		{
			need_clear = msg.need_clear();
		}
	};


#pragma pack(pop)
}

#endif
