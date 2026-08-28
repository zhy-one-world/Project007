/********************************************************************
	created:	2015/07/22
	created:	22:7:2015   20:48
	file base:	skill
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _FAITH_SKILL_HPP_
#define _FAITH_SKILL_HPP_

#include "logic/type_def.hpp"
#include "Logic/skill_def.hpp"
#include "base.hpp"

namespace hld
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_skill = e_msg_base_skill,
		e_msgindex_cs2dp_save_char_skill,
		e_msgindex_dp2cs_load_char_legion_skill,
		e_msgindex_cs2dp_save_char_legion_skill,
	};

	struct dp2cs_load_char_skill : packet_base
	{
		dp2cs_load_char_skill()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_skill;
		}

		guid_64						role_guid;
		int32							unit_array_index;
		int32							data_num;
		s_skill_info				data_list[MAX_SKILL_NUM];
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_skill_info));
		}
	};

	struct cs2dp_save_char_skill : packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		uint32							data_num;
		s_skill_info					data_list[e_skill_type_max];
		cs2dp_save_char_skill()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_skill;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_skill_info));
		}
	};

	struct dp2cs_load_char_legion_skill : packet_base
	{
		dp2cs_load_char_legion_skill()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_legion_skill;
		}

		guid_64						role_guid;
		int32						unit_array_index;
		int32						data_num;
		s_legion_skill_info		data_list[max_passive_skill_num];
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_legion_skill_info));
		}
	};

	struct cs2dp_save_char_legion_skill : packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		uint32							data_num;
		s_legion_skill_info			data_list[max_legion_skill_num];
		cs2dp_save_char_legion_skill()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_legion_skill;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_legion_skill_info));
		}
	};


	

#pragma pack(pop)
}

#endif
