/********************************************************************
created: 2016/08/25
file base: event
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_LUCKY_DRAW_RECORD_HPP_
#define _FAITH_LUCKY_DRAW_RECORD_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/item_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_add_draw_info_to_db = hld::e_msg_base_lucky_draw_record,
		e_msgindex_ws2dp_del_draw_info_to_db,
		e_msgindex_ws2dp_load_info_from_db,
		e_msgindex_dp2ws_load_info_from_db_end,
		e_msgindex_cs2ws_add_new_draw_record,
	};

	struct cs2ws_add_new_draw_record : public packet_base
	{
		s_hope_hunt_record_info		record_info;
		cs2ws_add_new_draw_record()
		{
			wheader = e_msgindex_cs2ws_add_new_draw_record;
		}
	};

	struct ws2dp_add_new_draw_record : public packet_base
	{
		s_hope_hunt_record_info		record_info;
		ws2dp_add_new_draw_record()
		{
			wheader = e_msgindex_ws2dp_add_draw_info_to_db;
		}
	};

	struct ws2dp_del_draw_record : public packet_base
	{
		guid_64						info_guid;
		ws2dp_del_draw_record()
		{
			wheader = e_msgindex_ws2dp_del_draw_info_to_db;
		}
	};

	struct ws2dp_load_draw_record_by_db : public packet_base
	{
		ws2dp_load_draw_record_by_db()
		{
			wheader = e_msgindex_ws2dp_load_info_from_db;
		}
	};

	struct dp2ws_load_draw_record_by_db_end : public packet_base
	{
		int32					data_num;
		s_hope_hunt_record_info data_info[draw_record_total_num];
		dp2ws_load_draw_record_by_db_end()
		{
			data_num = 0;
			wheader = e_msgindex_dp2ws_load_info_from_db_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_hope_hunt_record_info));
		}
	};

#pragma pack(pop)
}
#endif