/********************************************************************
created: 2016/07/155
created: 5:7:2016 14:41
file base: special_name_msg
file ext: hpp
author: zhangminghai

purpose:
*********************************************************************/
#ifndef _FAITH_SPECIAL_NAME_HPP_
#define _FAITH_SPECIAL_NAME_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/special_name_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_special_name = faith::e_msg_base_special_name,
		e_msgindex_cs2dp_save_char_special_name,
	};
	// DP2CS 分批发送玩家的所有称号信息
	struct dp2cs_load_char_special_name : public faith::packet_base
	{
		guid_64							role_guid;
		int32								unit_array_index;
		bool								is_over;		// 标识玩家称号是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		s_special_name_info					special_name_data;	//玩家的所有称号

		dp2cs_load_char_special_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_special_name;
		}
	};
	
	struct cs2dp_save_char_special_name : public faith::packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		s_special_name_info				name_data;	//玩家的所有称号

		cs2dp_save_char_special_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_special_name;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&name_data - (ULONG_PTR)&wheader;
			return (basic_len + sizeof(s_special_name_info));
		}
	};
	
#pragma pack(pop)
}

#endif