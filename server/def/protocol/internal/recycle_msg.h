#ifndef RECYCLE_MSG_H
#define RECYCLE_MSG_H
#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/recycle_def.h"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_recycle = faith::e_msg_base_recycle,
		e_msgindex_cs2dp_save_char_recycle,
		e_msgindex_dp2cs_load_char_recycle_task,
		e_msgindex_cs2dp_save_char_recycle_task,

		e_msgindex_dp2cs_load_char_recycle_invited,

		e_msgindex_cs2dp_recycle_get_inviter_info,
		e_msgindex_dp2cs_recycle_get_inviter_info_end,
		e_msgindex_cs2dp_recycle_inviter_add,
	};
	struct dp2cs_load_char_recycle : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		s_recycle_info					    data_info;
		dp2cs_load_char_recycle()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_recycle;
		}
	};
	struct cs2dp_save_char_recycle : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_recycle_info					    data_info;

		cs2dp_save_char_recycle()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_recycle;
		}
	};
	//////////////////////////////////////////////////////
	struct dp2cs_load_char_recycle_task : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		//bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_recycle_task_info					data_list[MAX_RECYCLE_TASK_NUM];
		dp2cs_load_char_recycle_task()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_recycle_task;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_recycle_task_info));
		}
	};
	/////////////////////////////////////////////////////
	struct cs2dp_save_char_recycle_task : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_recycle_task_info					data_list[MAX_RECYCLE_TASK_NUM];

		cs2dp_save_char_recycle_task()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_recycle_task;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_recycle_task_info));
		}

	};

	struct dp2cs_load_char_recycle_invited : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		//bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_recycle_invited_info			    data_list[MAX_RECYCLE_INVITED_NUM];
		dp2cs_load_char_recycle_invited()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_recycle_invited;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_recycle_invited_info));
		}
	};

	struct cs2dp_recycle_get_inviter_info : public packet_base
	{
		guid_64				role_guid;
		guid_64				target_guid;
		cs2dp_recycle_get_inviter_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_recycle_get_inviter_info;
		}
	};
	struct dp2cs_recycle_get_inviter_info_end : public packet_base
	{
		guid_64					        role_guid;
		guid_64                         target_guid;
		int32                           error;
		s_recycle_inviter_base_info_db	data_info;
		dp2cs_recycle_get_inviter_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_recycle_get_inviter_info_end;
		}
	};

	struct cs2dp_save_char_recycle_invited : public faith::packet_base
	{
		guid_64				role_guid;
		guid_64				target_guid;
		cs2dp_save_char_recycle_invited()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_recycle_inviter_add;
		}
	};











#pragma pack(pop)
}










#endif