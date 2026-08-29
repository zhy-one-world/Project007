/********************************************************************
created: 2016/10/09
file base: title
file ext: hpp
author: wucun

purpose:
*********************************************************************/
#ifndef _FAITH_TITLE_HPP_
#define _FAITH_TITLE_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/title_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_title = faith::e_msg_base_title,
		e_msgindex_cs2dp_save_char_title,
	};

	struct dp2cs_load_char_title : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								data_num;
		s_title_info						data_list[faith::MAX_TITLE_NUM];
		dp2cs_load_char_title()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_title;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_skill_info));
		}
	};

	struct cs2dp_save_char_title : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_title_info						data_list[faith::MAX_TITLE_NUM];

		cs2dp_save_char_title()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_title;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_title_info));
		}
	};
#pragma pack(pop)
}

#endif