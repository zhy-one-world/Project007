/********************************************************************
created: 2017-5-26 16:54:23
file base: talent
file ext: hpp

purpose:
*********************************************************************/
#ifndef _FAITH_TALENT_HPP_
#define _FAITH_TALENT_HPP_

#include "base.hpp"
#include "Logic/talent_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_talent = hld::e_msg_base_talent,
		e_msgindex_cs2dp_save_char_talent,
	};

	struct dp2cs_load_char_talent : public hld::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		int32								data_num;
		s_talent_info						data_list[TALENT_NUM_MAX];

		dp2cs_load_char_talent()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_talent;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_talent_info));
		}
	};
	struct cs2dp_save_char_talent : public hld::packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		int32							data_num;
		s_talent_info					data_list[TALENT_NUM_MAX];	//ÊµÀýÊý¾Ý
		cs2dp_save_char_talent()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_talent;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_talent_info));
		}
	};

#pragma pack(pop)
}

#endif
