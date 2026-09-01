#ifndef PHANTOM_MSG_HPP
#define PHANTOM_MSG_HPP
#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/phantom_def.h"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_phantom = faith::e_msg_base_phantom,
		e_msgindex_cs2dp_save_char_phantom,
	};
	struct dp2cs_load_char_phantom : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		//bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_phantom_info					    data_list[MAX_PHANTOM_NUM];
		dp2cs_load_char_phantom()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_phantom;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_phantom_info));
		}
	};

	struct cs2dp_save_char_phantom : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_phantom_info					    data_list[MAX_PHANTOM_NUM];	

		cs2dp_save_char_phantom()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_phantom;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_phantom_info));
		}

	};






#pragma pack(pop)
}


#endif
