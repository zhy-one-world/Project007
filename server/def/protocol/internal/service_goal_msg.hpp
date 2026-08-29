/********************************************************************
created: 2017年8月10日11:15:00
file base: service_goal
author: 韩泽成
purpose: 七日目标存档消息
*********************************************************************/
#ifndef _FAITH_SERVICE_GOAL_HPP_
#define _FAITH_SERVICE_GOAL_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/service_goal_def.hpp"
namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_service_goal = faith::e_msg_base_service_goal,
		e_msgindex_cs2dp_save_char_service_goal,
	};

	struct dp2cs_load_char_service_goal : public faith::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		bool								is_over;		// 标识是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_service_goal_info					data_list[MAX_SERVICE_GOAL_NUM];
		dp2cs_load_char_service_goal()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_service_goal;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_service_goal_info));
		}
	};
	struct cs2dp_save_char_service_goal : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_service_goal_info					data_list[MAX_SERVICE_GOAL_NUM];

		cs2dp_save_char_service_goal()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_service_goal;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_service_goal_info));
		}

	};

#pragma pack(pop)
}

#endif
