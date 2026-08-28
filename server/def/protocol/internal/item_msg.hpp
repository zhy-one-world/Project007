/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 21:41
  file base: item
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_ITEM_HPP_
#define _FAITH_ITEM_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/item_def.hpp"
#include "net/http_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_item					= hld::e_msg_base_item,
		e_msgindex_cs2dp_save_char_item,
	};

	// DP2CS 分批发送玩家的所有物品信息
	struct dp2cs_load_char_item : public hld::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		s_daemon_callback_info				callback_info;
		int32								bag_type;		// 背包类型
		int32								data_num;
		s_item_info							data_list[MAX_ITEM_BAG_NUM];

		dp2cs_load_char_item()
		{
			memset(this, 0, sizeof(dp2cs_load_char_item));
			callback_info.clear_data();
			wheader	= e_msgindex_dp2cs_load_char_item;
		}
		void clear_data()
		{
			role_guid.clear_data();
			unit_array_index = 0;
			callback_info.clear_data();
			bag_type = 0;
			data_num = 0;
			memset(data_list, 0, sizeof(data_list));
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_item_info));
		}
	};



	struct cs2dp_save_char_item : public hld::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								bag_type;		// 背包类型
		int32								data_num;
		s_item_info					        data_list[MAX_ITEM_BAG_NUM];	//物品实例数据

		cs2dp_save_char_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_item;
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
