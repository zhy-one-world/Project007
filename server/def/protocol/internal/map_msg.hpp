/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 21:41
  file base: item
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_MAP_HPP_
#define _FAITH_MAP_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/map_def.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_map = faith::e_msg_base_map,
		e_msgindex_cs2dp_save_char_map,

		e_msgindex_dp2cs_load_char_demons_tower,
		e_msgindex_cs2dp_save_char_demons_tower,
	};

	struct dp2cs_load_char_map : public packet_base
	{
		bool                 is_over;
		guid_64              role_guid;
		int32							unit_array_index;
		int32                data_num;
		s_map_record_info    data_list[max_map_record_num];

		dp2cs_load_char_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_map;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_map_record_info));
		}
	};

	struct cs2dp_save_char_map : public packet_base
	{
		guid_64					role_guid;
		int32					unit_array_index;
		int32					save_type_ex;
		int32					data_num;
		s_map_record_info		data_list[max_map_record_num];

		cs2dp_save_char_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_map;
		}

		uint32 get_pak_length()
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_map_record_info));
		}
	};

	struct dp2cs_load_char_demons_tower : public packet_base
	{
		bool                 is_over;
		guid_64	             role_guid;
		int32				 unit_array_index;
		s_demons_tower_info  demons_tower_info;

		dp2cs_load_char_demons_tower()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_demons_tower;
		}
	};

	struct cs2dp_save_char_demons_tower : public packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		s_demons_tower_info				demon_tower_info;

		cs2dp_save_char_demons_tower()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_demons_tower;
		}
	};
#pragma pack(pop)
}

#endif
