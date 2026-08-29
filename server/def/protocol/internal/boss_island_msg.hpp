#ifndef _FAITH_BOSS_ISLAND_HPP_
#define _FAITH_BOSS_ISLAND_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/boss_island_def.hpp"
#include "Logic/gain_treasure_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum 
	{
		e_msgindex_dp2cs_load_boss_island_info = faith::e_msg_base_boss_island,
		e_msgindex_cs2dp_save_boss_island_info,
		e_msgindex_cs2ws_update_boss_island_info,

	};
	struct dp2cs_load_boss_island_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		s_boss_island_info					info_data;
		dp2cs_load_boss_island_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_boss_island_info;
		}
	};

	struct cs2dp_save_boss_island_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_boss_island_info				info_data;
		cs2dp_save_boss_island_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_boss_island_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&info_data - (ULONG_PTR)&wheader;
			return (basic_len + sizeof(s_boss_island_info));
		}
	};

	struct cs2ws_update_boss_island_info : public faith::packet_base
	{
		s_gain_treasure_boss_info			boss_info;

		cs2ws_update_boss_island_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_update_boss_island_info;
		}
	};
#pragma pack(pop)
}

#endif
