/********************************************************************
	created:	2016Äê3ÔÂ29ÈÕ16:02:29
	filename: 	buff.hpp
	file base:	buff
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _FAITH_BUFF_HPP_
#define _FAITH_BUFF_HPP_

#include "logic/type_def.hpp"
#include "Logic/buff_def.hpp"
#include "base.hpp"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_buff = e_msg_base_buff,
		e_msgindex_cs2dp_save_char_buff,
	};

	struct dp2cs_load_char_buff : packet_base
	{
		dp2cs_load_char_buff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_buff;
		}

		guid_64						role_guid;
		int32							unit_array_index;
		int32						data_num;
		s_buff_info					data_list[MAN_UNIT_BUFF_INST];
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_buff_info));
		}
	};

	struct cs2dp_save_char_buff : packet_base
	{
		cs2dp_save_char_buff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_buff;
		}
		guid_64					role_guid;
		int32					unit_array_index;
		int32					save_type_ex;
		int32					data_num;
		s_buff_info				data_list[MAN_UNIT_BUFF_INST];
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_buff_info));
		}
	};
#pragma pack(pop)
}

#endif
