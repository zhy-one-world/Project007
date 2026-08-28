/********************************************************************
created: 2016/04/28
file base: spirit
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_SPIRIT_HPP_
#define _FAITH_SPIRIT_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/spirit_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_spirit = hld::e_msg_base_spirit,
		e_msgindex_cs2dp_save_char_spirit,
	};

	// DP2CS 分批发送玩家的所有物品信息
	struct dp2cs_load_char_spirit : public hld::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32									data_num;
		s_spirit_info						data_list[sprite_bag_num];
		dp2cs_load_char_spirit()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_spirit;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_spirit_info));
		}

	};

	struct cs2dp_save_char_spirit : public hld::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_spirit_info						data_list[sprite_bag_num];
		cs2dp_save_char_spirit()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_spirit;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_spirit_info));
		}

	};






#pragma pack(pop)
}

#endif