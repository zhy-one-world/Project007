/********************************************************************
created: 2016年7月18日21:24:26
file base: achievement
author: 张宇翔
purpose: 成就存档消息
*********************************************************************/
#ifndef _FAITH_ACHIEVEMENT_HPP_
#define _FAITH_ACHIEVEMENT_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/achievement_def.h"
namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_achievement = faith::e_msg_base_achievement,
		e_msgindex_cs2dp_save_char_achievement,
	};

	// DP2CS 分批发送玩家的所有物品信息
	struct dp2cs_load_char_achievement : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32									data_num;
		s_achievement_info					data_list[MAX_ACHIEVEMENT_NUM];
		dp2cs_load_char_achievement()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_achievement;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_achievement_info));
		}
	};
	struct cs2dp_save_char_achievement : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_achievement_info					data_list[MAX_ACHIEVEMENT_NUM];	//元素之心实例数据

		cs2dp_save_char_achievement()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_achievement;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_achievement_info));
		}

	};

#pragma pack(pop)
}

#endif
