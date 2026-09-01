/********************************************************************
created: 2019年5月24日
file base: oracle_trial_msg
file ext: hpp
author: zhaoyuming

purpose:
*********************************************************************/

#ifndef _ORACLE_TRIAL_MSG_HPP_
#define _ORACLE_TRIAL_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/oracle_trial_def.hpp"
#include "core.hpp"
#include "utility/score_indicator.h"
#include "Logic/chat_def.hpp"
#include <Logic/oracle_trial_def.hpp>
namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_oracle_trial_all_info = faith::e_msg_oracle_trial,
		e_msgindex_dp2cs_save_oracle_trial_info_nil,
		e_msgindex_cs2dp_save_oracle_trial_info
	};

	struct dp2cs_load_oracle_trial_all_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								data_num;
		s_oracle_trial_info_db				data_list[max_tier_num];
		dp2cs_load_oracle_trial_all_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_oracle_trial_all_info;
		}
		
	};

	struct dp2cs_save_oracle_trial_info_nil : public faith::packet_base
	{
		guid_64								role_guid;
		dp2cs_save_oracle_trial_info_nil()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_save_oracle_trial_info_nil;
		}

	};

	struct cs2dp_save_oracle_trial_info : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type;	
		int32								data_num;
		s_oracle_trial_info_db				info_data[max_tier_num];
		cs2dp_save_oracle_trial_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_oracle_trial_info;
		}

	};

#pragma pack(pop)
}
#endif