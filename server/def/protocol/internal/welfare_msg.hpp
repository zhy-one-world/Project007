/********************************************************************
created: 2016/10/09
file base: welfare
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_WELFARE_HPP_
#define _FAITH_WELFARE_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/welfare_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_dp2cs_load_char_welfare = faith::e_msg_base_welfare,
		e_msgindex_cs2dp_save_char_welfare,
		e_msgindex_dp2cs_load_char_active_degree,
		e_msgindex_cs2dp_save_char_active_degree,
		e_msgindex_dp2cs_load_char_welfare_random_get,
		e_msgindex_cs2dp_save_char_welfare_random_get
	};

	struct dp2cs_load_char_welfare : public faith::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		s_welfare_info						welfare_data;
		dp2cs_load_char_welfare()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_welfare;
		}
	};

	struct cs2dp_save_char_welfare : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_welfare_info						welfare_data;
		cs2dp_save_char_welfare()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_welfare;
		}
	};

	struct dp2cs_load_char_active_degree : public faith::packet_base
	{
		guid_64								role_guid;
		int32							unit_array_index;
		s_active_degree_info				active_degree_data;
		dp2cs_load_char_active_degree()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_active_degree;
		}
	};

	struct cs2dp_save_char_active_degree : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_active_degree_info				active_degree_data;
		cs2dp_save_char_active_degree()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_active_degree;
		}
	};
	struct cs2dp_save_char_welfare_random_get : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_welfare_random_get_info			random_get_data;
		cs2dp_save_char_welfare_random_get()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_welfare_random_get;
		}
	};

	struct dp2cs_load_char_welfare_random_get : public faith::packet_base
	{
		guid_64								role_guid;
		s_welfare_random_get_info			random_get_data;
		int32								unit_array_index;
		dp2cs_load_char_welfare_random_get()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_welfare_random_get;
		}
	};
#pragma pack(pop)
}

#endif