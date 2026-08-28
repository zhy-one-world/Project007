/********************************************************************
  created: 2016年6月2日14:00:55
  file base: att_msg
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_ATT_MSG_HPP_
#define _FAITH_ATT_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/att_def.hpp"

namespace hld
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_att					= hld::e_msg_base_att,
		e_msgindex_cs2dp_save_char_att,
	};

	// DP2CS 分批发送玩家的所有物品信息
	struct dp2cs_load_char_att : public hld::packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		s_base_att_info					data_info;
		dp2cs_load_char_att()
		{
			memset(this, 0, sizeof(dp2cs_load_char_att));
			wheader	= e_msgindex_dp2cs_load_char_att;
		}
	};
	struct cs2dp_save_char_att : public hld::packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		s_base_att_info					data_info;
		f32								attack_att[e_unit_attack_att_max];
		int32							save_type_ex;

		cs2dp_save_char_att()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_att;
		}
	};

#pragma pack(pop)
}

#endif
