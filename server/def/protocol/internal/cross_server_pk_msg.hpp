/********************************************************************
created: 2019年1月17日
file base: cross_server_pk_msg
file ext: hpp
author: zhangshuo

purpose: 跨服PK
*********************************************************************/

#ifndef _FAITH_CROSS_SERVER_PK_MSG_HPP_
#define _FAITH_CROSS_SERVER_PK_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/cross_server_pk_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum 
	{
		e_msgindex_ws2dp_save_cross_server_pk_info = faith::e_msg_base_cross_server_pk,
		e_msgindex_ws2dp_load_cross_server_pk_info,
		e_msgindex_dp2ws_load_cross_server_pk_info,
		e_msgindex_ws2dp_clear_cross_server_pk_info,

		e_msgindex_cs2ws_update_cross_server_pk_info,
		e_msgindex_cs2ws_cross_pk_map_is_end,
		e_msgindex_ws2ws_cross_pk_map_end,
		e_msgindex_ws2ws_cross_pk_map_result,
		e_msgindex_ws2ws_clear_all_pk_msg,
		e_msgindex_ws2ws_pk_fake_player_msg
	};

	struct ws2dp_save_cross_server_pk_msg_all : public faith::packet_base
	{
		s_pk_island_simple_msg	island_simple_msg;
		ws2dp_save_cross_server_pk_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_cross_server_pk_info;
		}
	};

	struct ws2dp_clear_cross_server_pk_msg_all : public faith::packet_base
	{
		ws2dp_clear_cross_server_pk_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_cross_server_pk_info;
		}
	};

	struct ws2dp_load_cross_server_pk_msg_all : public faith::packet_base
	{
		ws2dp_load_cross_server_pk_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_cross_server_pk_info;
		}
	};

	struct s_cross_server_pk_info_db
	{
		s_dp2ws_island_simple_msg				data_info;
		s_cross_server_pk_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct dp2ws_load_cross_server_pk_msg_all : public faith::packet_base
	{
		int32 data_num;
		s_dp2ws_island_simple_msg	simple_msg[max_cross_server_all_class_war_count];
		dp2ws_load_cross_server_pk_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_cross_server_pk_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&simple_msg - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_dp2ws_island_simple_msg));
		}
	};

	struct ws2ws_transfer_cross_pk_map : public faith::packet_base
	{
		int32						class_type;
		guid_64						role_guid;
		int32						server_id;
		int32						activity_type;
		guid_64						legion_guid;
		int32						terr_id;
		int32						map_type;
		ws2ws_transfer_cross_pk_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_pk_map_end;
		}
	};

	struct ws2ws_transfer_cross_pk_map_result : public faith::packet_base
	{
		int32						result;
		guid_64						role_guid;
		int32						map_temp_id;
		guid_64						map_guid;
		int32						war_idex;
		int32						activity_type;
		int32						server_id;
		ws2ws_transfer_cross_pk_map_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_pk_map_result;
		}
	};
	struct cs2ws_update_one_island_msg : public faith::packet_base
	{
		s_pk_island_all_msg		island_msg;
		int32					war_num;
		guid_64					winner_guid;
		int32					is_end_war;
		bool					is_need_send_to_client;
		int32					pk_count;
		cs2ws_update_one_island_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_update_cross_server_pk_info;
		}
	};

	struct cs2ws_cross_server_pk_map_msg : public packet_base
	{
		bool	is_end;
		int32	class_type;
		cs2ws_cross_server_pk_map_msg()
		{
			wheader = e_msgindex_cs2ws_cross_pk_map_is_end;
			is_end = false;
			class_type = 0;
		}
	};

	struct ws2ws_pk_harry_player : public faith::packet_base
	{
		s_pk_player_msg			pk_player_msg[max_branch_type];
		int32					data_num;
		ws2ws_pk_harry_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_pk_fake_player_msg;
		}
	};

#pragma pack(pop)
}

#endif
