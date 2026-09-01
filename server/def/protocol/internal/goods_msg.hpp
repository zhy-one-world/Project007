/********************************************************************
created: 2016/07/5
created: 5:7:2016 21:41
file base: store_goods_msg
file ext: hpp
author: zhangminghai

purpose:
*********************************************************************/
#ifndef _FAITH_GOODS_HPP_
#define _FAITH_GOODS_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/goods_def.hpp"
namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_goods = faith::e_msg_base_goods,
		e_msgindex_cs2dp_save_char_goods,
	};
	// DP2CS 分批发送玩家的所有商品信息
	struct dp2cs_load_char_goods : public faith::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		bool								is_over;		// 标识玩家商品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32									data_num;
		s_goods_info						data_list[MAX_GOODSINST_SIZE_PROP];	//商品购买的记录
		dp2cs_load_char_goods()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_goods;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_goods_info));
		}
	};
	struct cs2dp_save_char_goods : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_goods_info						data_list[MAX_GOODSINST_SIZE_PROP];;	//商品购买的记录

		cs2dp_save_char_goods()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_goods;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_goods_info));
		}
	};
#pragma pack(pop)
}

#endif