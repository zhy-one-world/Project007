/********************************************************************
created: 2017-5-26 16:54:23
file base: treasure_msg
file ext: hpp

purpose:
*********************************************************************/
#ifndef _FAITH_TREASURE_HPP_
#define _FAITH_TREASURE_HPP_

#include "base.hpp"
#include "Logic/treasure_def.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_treasure = faith::e_msg_base_treasure,
		e_msgindex_cs2dp_save_char_treasure,
	};

	struct dp2cs_load_char_treasure : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								data_num;
		s_treasure_record					data_list[treasure_prize_record_num];

		dp2cs_load_char_treasure()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_treasure;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_treasure_record));
		}
	};
	struct cs2dp_save_char_treasure : public faith::packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		int32							data_num;
		s_treasure_record				data_list[treasure_prize_record_num];
		cs2dp_save_char_treasure()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_treasure;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_treasure_record));
		}
	};

#pragma pack(pop)
}

#endif
