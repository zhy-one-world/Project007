/********************************************************************
created: 2016年5月10日21:24:26
file base: mission
author: 张宇翔
purpose: 任务存档消息
*********************************************************************/
#ifndef _FAITH_MISSION_HPP_
#define _FAITH_MISSION_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/mission_def.hpp"


namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_mission = faith::e_msg_base_mission,
		e_msgindex_cs2dp_save_char_mission,
	};

	// DP2CS 分批发送玩家的所有信仰信息
	struct dp2cs_load_char_mission : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_mission_info						data_list[e_mission_slot_max];
		dp2cs_load_char_mission()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_mission;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_mission_info));
		}
	};
	struct cs2dp_save_char_mission : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_mission_info						data_list[e_mission_slot_max];	//物品实例数据
		cs2dp_save_char_mission()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_mission;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_mission_info));
		}
	};

#pragma pack(pop)
}

#endif
