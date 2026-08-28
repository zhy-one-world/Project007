/********************************************************************
created: 2016/09/22
created: 22:9:2016 14:41
file base: convert_msg
file ext: hpp
author: zhangminghai

purpose:
*********************************************************************/
#ifndef _FAITH_CONVERT_HPP_
#define _FAITH_CONVERT_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"

namespace hld
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_convert = hld::e_msg_base_convert,
		e_msgindex_cs2dp_save_char_convert,
	};
	// DP2CS 分批发送玩家的所有称号信息
	struct dp2cs_load_char_convert : public hld::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		bool								is_over;		
		int32								data_list[e_convert_type_max];

		dp2cs_load_char_convert()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_convert;
		}
	};

	struct cs2dp_save_char_convert : public hld::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_list[e_convert_type_max];

		cs2dp_save_char_convert()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_convert;
		}
	};

	struct s_convert_info_db
	{
		guid_64								role_guid;
		int32								currency_index;
		int32								times;
		s_convert_info_db()
		{
			role_guid = 0;
			currency_index = 0;
			times = 0;
		}
	};


#pragma pack(pop)
}

#endif