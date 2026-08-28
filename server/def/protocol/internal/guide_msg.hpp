#ifndef _FAITH_GUIDE_MSG_HPP_
#define _FAITH_GUIDE_MSG_HPP_

#include "logic/type_def.hpp"
#include "Logic/guide_def.hpp"
#include "base.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_cs2dp_save_guide_record = hld::e_msg_base_guide,
		e_msgindex_dp2cs_load_guide_record
	};

	struct cs2dp_save_guide_record : public packet_base
	{
		guid_64	role_guid;
		int32	unit_array_index;
		int32	save_type_ex;
		s_record_info	record_info;

		cs2dp_save_guide_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_guide_record;
		}
	};

	struct dp2cs_load_guide_record : public packet_base
	{
		guid_64	role_guid;
		int32							unit_array_index;
		int32	data_num;
		s_record_info	data_list[hld::max_guide_record_num];

		dp2cs_load_guide_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_guide_record;
		}
	};

#pragma pack(pop)
}

#endif
