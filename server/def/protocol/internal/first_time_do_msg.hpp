#ifndef _FAITH_FIRST_TIME_DO_MSG_HPP_
#define _FAITH_FIRST_TIME_DO_MSG_HPP_

#include "logic/type_def.hpp"
#include "Logic/first_time_do_def.hpp"
#include "base.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_cs2dp_save_first_time_do_record = faith::e_msg_base_first_time_do,
		e_msgindex_dp2cs_load_first_time_do_record
	};

	struct cs2dp_save_first_time_do_record : public packet_base
	{
		guid_64	role_guid;
		int32	do_type;

		cs2dp_save_first_time_do_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_first_time_do_record;
		}
	};

	struct dp2cs_load_first_time_do_record : public packet_base
	{
		guid_64					role_guid;
		int32							unit_array_index;
		int32					data_num;
		s_first_time_do_record	data_list[e_first_time_do_type_max];

		dp2cs_load_first_time_do_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_first_time_do_record;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_first_time_do_record));
		}
	};

#pragma pack(pop)
}

#endif
