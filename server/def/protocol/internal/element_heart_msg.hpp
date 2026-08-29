/********************************************************************
created: 2016年7月8日21:24:26
file base: mission
author: 张宇翔
purpose: 任元素之心存档消息
*********************************************************************/
#ifndef _FAITH_ELEMENT_HEART_HPP_
#define _FAITH_ELEMENT_HEART_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/element_heart_def.hpp"
#include "Logic/item_def.hpp"
namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_element_heart = faith::e_msg_base_element_heart,
		e_msgindex_cs2dp_save_char_element_heart,
	};

	// DP2CS 分批发送玩家的所有物品信息
	struct dp2cs_load_char_element_heart : public faith::packet_base
	{
		guid_64								role_guid;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32									data_num;
		s_item_info							element_heart_data[element_heart_bag_num + element_heart_equip_num];
		dp2cs_load_char_element_heart()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_element_heart;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&element_heart_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_item_info));
		}
	};
	struct cs2dp_save_char_element_heart : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_item_info					        data_list[element_heart_bag_num + element_heart_equip_num];	//元素之心实例数据

		cs2dp_save_char_element_heart()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_element_heart;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_item_info));
		}

	};

#pragma pack(pop)
}

#endif
