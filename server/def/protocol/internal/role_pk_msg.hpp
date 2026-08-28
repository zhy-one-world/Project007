/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 21:41
  file base: role_pk
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_ROLE_PK_HPP_
#define _FAITH_ROLE_PK_HPP_

#include "base.hpp"
#include "Logic/role_pk_def.hpp"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_load_role_pk_top = hld::e_msg_base_role_pk,
		e_msgindex_dp2ws_load_role_pk_top,
		e_msgindex_ws2dp_load_role_pk_info,
		e_msgindex_dp2ws_load_role_pk_info,
		e_msgindex_cs2ws_role_pk_result,
		e_msgindex_ws2dp_save_role_pk_info,
		e_msgindex_ws2dp_save_role_pk_honor_refresh,
		e_msgindex_ws2dp_load_role_pk_log,
		e_msgindex_dp2ws_load_role_pk_log,
	};
	struct ws2dp_load_role_pk_top : public hld::packet_base
	{
		ws2dp_load_role_pk_top()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_role_pk_top;
		}
	}; 
	struct dp2ws_load_role_pk_top : public hld::packet_base
	{
		s_role_pk_info						pk_top_array[PK_RANKING_TOP_NUM];

		dp2ws_load_role_pk_top()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_role_pk_top;
		}
	};


	struct ws2dp_load_role_pk_info : public hld::packet_base
	{
		guid_64								role_guid;
		xchar								role_name[max_name_size + 1];
		int32								template_id;
		int32								role_gs;

		ws2dp_load_role_pk_info()
		{
			memset(this, 0, sizeof(*this));
			wheader	= e_msgindex_ws2dp_load_role_pk_info;
		}
	};
	struct dp2ws_load_role_pk_info : public hld::packet_base
	{
		s_role_pk_info						role_pk_info;

		dp2ws_load_role_pk_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_role_pk_info;
		}
	};

	struct cs2ws_role_pk_result : public packet_base
	{
		guid_64				finish_war_map;
		e_team_num			win_team_num;
		cs2ws_role_pk_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_role_pk_result;
		}
	};
	struct ws2dp_save_role_pk_info : public hld::packet_base
	{
		guid_64			role_guid;
		s_cross_war_db	cross_war_db;

		ws2dp_save_role_pk_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_role_pk_info;
		}
	};
	struct ws2dp_save_role_pk_honor_refresh : public hld::packet_base
	{
		guid_64			role_guid;
		int32			refresh_time;

		ws2dp_save_role_pk_honor_refresh()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_role_pk_honor_refresh;
		}
	};
	struct ws2dp_load_role_pk_log : public hld::packet_base
	{
		guid_64			role_guid;
		int32			server_id;
		ws2dp_load_role_pk_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_role_pk_log;
		}
	};
	struct dp2ws_load_role_pk_log : public hld::packet_base
	{
		guid_64			role_guid;
		int32			server_id;
		int32			data_len;
		s_cross_war_db	data_array[CROSS_WAR_INFO_NUM];
		dp2ws_load_role_pk_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_role_pk_log;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_array - (ULONG_PTR)&wheader;
			return (basic_len + data_len * sizeof(s_cross_war_db));
		}
	};


	

#pragma pack(pop)
}

#endif
