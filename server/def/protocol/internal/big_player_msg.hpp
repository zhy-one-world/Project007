/********************************************************************
created: 2016/10/17
file base: world boss
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_BIG_PLAYER_HPP_
#define _FAITH_BIG_PLAYER_HPP_

#include "logic/type_def.hpp"
#include "Logic/fake_player_def.hpp"
#include "base.hpp"
#include "internet/ws2ws.pb.h"
#include "internet/dp2s.pb.h"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_save_big_player = faith::e_msg_big_player,
		e_msgindex_ws2dp_load_big_player,
		e_msgindex_ws2dp_load_big_player_detail,
		e_msgindex_dp2ws_load_big_player_end,
		e_msgindex_dp2ws_load_big_player_base_info_end,
		e_msgindex_dp2ws_load_big_player_att_info_end,
		e_msgindex_dp2ws_load_big_player_item_info_end,
		e_msgindex_dp2ws_load_big_player_skill_info_end,
		e_msgindex_dp2ws_load_big_player_buff_end,
		e_msgindex_ws2cs_create_big_player_statue,
		e_msgindex_cs2ws_create_big_player_success,
		e_msgindex_ws2ws_big_player_info,
	};

	struct ws2dp_save_big_player_to_db : public faith::packet_base
	{
		s_big_player_db						data_info[e_big_player_type_max];
		ws2dp_save_big_player_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_big_player;
		}
	};

	struct ws2dp_load_big_player_by_db : public faith::packet_base
	{
		ws2dp_load_big_player_by_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_big_player;
		}
	};

	//struct ws2dp_load_big_player_by_db_one : public faith::packet_base
	//{
	//	guid_64						player_guid;
	//	ws2dp_load_big_player_by_db_one()
	//	{
	//		memset(this, 0, sizeof(*this));
	//		wheader = e_msgindex_ws2dp_load_big_player_one;
	//	}
	//};
	struct dp2ws_load_big_player_end: public faith::packet_base
	{
		s_big_player_db						data_info[e_big_player_type_max];
		dp2ws_load_big_player_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_big_player_end;
		}
		bool to_proto(faith::dp2s_proto::load_big_player_end& msg)
		{
			for (int32 i = 0; i < e_big_player_type_max; i++)
			{
				faith::st_proto::st_big_player_db* st_big_ptr = msg.add_data_info();
				if (st_big_ptr == nullptr)
				{
					return false;
				}
				data_info[i].to_proto(st_big_ptr);
			}
			return true;
		}
		void from_proto(const faith::dp2s_proto::load_big_player_end& msg)
		{
			for (int32 i = 0; i < e_big_player_type_max && i < msg.data_info_size(); i++)
			{
				data_info[i].from_proto(msg.data_info(i));
			}
		}
	};

	struct ws2ws_send_big_player_msg : public faith::packet_base
	{
		s_big_player_db						data_info[e_big_player_type_max];
		bool								is_load_flag;
		ws2ws_send_big_player_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_big_player_info;
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));

		}
		bool to_proto(faith::ws2ws_proto::send_big_player_msg& msg)
		{
			for (int32 i = 0; i < e_big_player_type_max; i++)
			{
				faith::st_proto::st_big_player_db* st_big_ptr = msg.add_data_info();
				if (st_big_ptr == nullptr)
				{
					return false;
				}
			    data_info[i].to_proto(st_big_ptr);
			}
			msg.set_is_load_flag(is_load_flag);
		}
		void from_proto(const faith::ws2ws_proto::send_big_player_msg& msg)
		{
			for (int32 i = 0; i < e_big_player_type_max && i < msg.data_info_size(); i++)
			{
				data_info[i].from_proto(msg.data_info(i));
			}
			is_load_flag = msg.is_load_flag();
		}
	};

	struct ws2dp_load_big_player_detail : public packet_base
	{
		int32			big_type;
		guid_64			role_guid;
		ws2dp_load_big_player_detail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_big_player_detail;
		}
	};

	struct dp2ws_load_big_player_base_info_end : public packet_base
	{
		guid_64 role_guid;
		s_unit_info role_info;
		int32 big_type;

		dp2ws_load_big_player_base_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_big_player_base_info_end;
		}
	};

	struct dp2ws_load_big_player_att_info_end : public packet_base
	{
		guid_64 role_guid;
		s_base_att_info att_info;
		int32 big_type;

		dp2ws_load_big_player_att_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_big_player_att_info_end;
		}
	};

	struct dp2ws_load_big_player_skill_info_end : public packet_base
	{
		guid_64 role_guid;
		int32 big_type;
		s_skill_info skill_data[MAX_SKILL_NUM];
		dp2ws_load_big_player_skill_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_big_player_skill_info_end;
		}
	};

	struct dp2ws_load_big_player_item_info_end : public packet_base
	{
		guid_64 role_guid;
		int32 big_type;
		s_item_info item_data[role_show_item_num];
		dp2ws_load_big_player_item_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_big_player_item_info_end;
		}
	};

	struct dp2ws_load_big_player_buff_end : public packet_base
	{
		guid_64			role_guid;
		int32			big_type;
		uint32			data_num;
		s_buff_info		buff_data[MAN_UNIT_BUFF_INST];
		dp2ws_load_big_player_buff_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_big_player_buff_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&buff_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_buff_info));
		}
	};

	struct ws2cs_create_big_player_statue : public packet_base
	{
		s_fake_player_info				big_player_info;
		int32							big_type;
		guid_64							map_guid;
		ws2cs_create_big_player_statue()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_create_big_player_statue;
		}
	};

	struct cs2ws_create_big_player_success : public packet_base
	{
		guid_64							big_player_guid;
		int32							big_type;
		cs2ws_create_big_player_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_create_big_player_success;
		}
	};

#pragma pack(pop)
}
#endif