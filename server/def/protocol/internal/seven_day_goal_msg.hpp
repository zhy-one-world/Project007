/********************************************************************
created: 2019年2月13日
file base: seven_day_goal_msg
file ext: hpp
author: wangdi
purpose: 七日目标存档消息
*********************************************************************/
#ifndef _FAITH_SEVEN_DAY_GOAL_HPP_
#define _FAITH_SEVEN_DAY_GOAL_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/seven_day_goal_def.hpp"
namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_seven_day_goal = hld::e_msg_base_seven_day_goal,
		e_msgindex_cs2dp_save_char_seven_day_goal,
	};

	struct dp2cs_load_char_seven_day_goal : public hld::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		bool								is_over;		// 标识是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_seven_day_goal_info				data_list[max_seven_day_goal_num];
		dp2cs_load_char_seven_day_goal()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_seven_day_goal;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_seven_day_goal_info));
		}
	};
	struct cs2dp_save_char_seven_day_goal : public hld::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_seven_day_goal_info				data_list[max_seven_day_goal_num];

		cs2dp_save_char_seven_day_goal()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_seven_day_goal;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_seven_day_goal_info));
		}

	};

#pragma pack(pop)
}

#endif
