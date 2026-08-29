
/********************************************************************
created: 2016/07/20
created: 20:7:2016 11:11
file base: pokedex
file ext: hpp
author: XingLong

purpose:
*********************************************************************/

#ifndef _FAITH_POKEDEX_HPP_
#define _FAITH_POKEDEX_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/pokedex_def.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_pokedexs = faith::e_msg_base_pokedex,
		e_msgindex_cs2dp_save_char_pokedexs,
		e_msgindex_cs2fep_send_pokedexs_all_info,
	};

	enum
	{
		e_msgindex_dp2cs_load_char_tinder = faith::e_msg_base_tinder,
		e_msgindex_cs2dp_save_char_tinder,
	};

	// DP2CS 分批发送玩家的所有信仰信息
	struct dp2cs_load_char_pokedex : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								data_num;
		s_pokedex_info						data_list[POKEDEX_NUM_MAX];
		dp2cs_load_char_pokedex()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_pokedexs;
		}
	};

	struct cs2dp_save_char_pokedex : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_pokedex_info						data_list[POKEDEX_NUM_MAX];	//图鉴实例数据
		cs2dp_save_char_pokedex()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_pokedexs;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_pokedex_info));
		}
	};
	struct char_pokedex_db_info
	{
		guid_64								role_guid;
		int32								pokedex_data[e_pokedex_Info_max];
	};

	struct dp2cs_load_char_tinder : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		bool								is_over;		
		s_tinder_info						tinder_data;
		dp2cs_load_char_tinder()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_tinder;
		}
	};

	struct cs2dp_save_char_tinder : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_tinder_info						tinder_data;

		cs2dp_save_char_tinder()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_tinder;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&tinder_data - (ULONG_PTR)&wheader;
			return (basic_len + sizeof(s_tinder_info));
		}
	};
#pragma pack(pop)
}
#endif