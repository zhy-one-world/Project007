#ifndef _FAITH_APPEARANCE_INFO_MSG_HPP_
#define _FAITH_APPEARANCE_INFO_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "core.hpp"
#include "utility/score_indicator.h"
#include "Logic/chat_def.hpp"
namespace hld
{
#pragma pack(push,1)
	const int MAX_APPEARANCE_INFO_LENGTH = 1000;
	enum
	{
		e_msgindex_ws2dp_save_appearance_info_to_id = hld::e_msg_appearance_info,
		e_msgindex_dp2ws_save_appearance_info_to_id_end,
		e_msgindex_ws2dp_load_appearance_info_by_id,
		e_msgindex_dp2ws_load_appearance_info_by_id_end,
	};

	struct ws2dp_save_appearance_info_to_id : public hld::packet_base
	{
		s_client_uid client_uid;
		xchar appearance_info[MAX_APPEARANCE_INFO_LENGTH+1];
		ws2dp_save_appearance_info_to_id()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_appearance_info_to_id;
		}
	};

	struct dp2ws_save_appearance_info_to_id_end : public hld::packet_base
	{
		s_client_uid client_uid;
		xchar appearance_info[MAX_APPEARANCE_INFO_LENGTH+1];
		int32 appearance_id;
		dp2ws_save_appearance_info_to_id_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_save_appearance_info_to_id_end;
		}
	};

	struct ws2dp_load_appearance_info_by_id : public hld::packet_base
	{
		s_client_uid client_uid;
		int32 appearance_id;
		ws2dp_load_appearance_info_by_id()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_appearance_info_by_id;
		}
	};

	struct dp2ws_load_appearance_info_by_id_end : public hld::packet_base
	{
		s_client_uid client_uid;
		xchar appearance_info[MAX_APPEARANCE_INFO_LENGTH+1];
		int32 appearance_id;
		dp2ws_load_appearance_info_by_id_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_appearance_info_by_id_end;
		}
	};

#pragma pack(pop)
}
#endif