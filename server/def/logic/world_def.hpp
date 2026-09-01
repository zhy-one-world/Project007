/********************************************************************
	created:	2015/03/23
	created:	23:3:2015   16:24
	file base:	world_def
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WORLD_DEF_HPP_
#define _WORLD_DEF_HPP_

#include "logic/type_def.hpp"
#include "logic/math.hpp"
#include <string>

namespace faith 
{
#pragma pack(push,1)

	const int32 color_arr_size = 4; // RGBA
	const int32 cross_state_mark = 500000;
	const int32 cross_state = 10000;
	const int32 gm_set_result_len = 30;

	const int64 e_ws_flag_none =											0x0000000000000000;
	const int64 e_ws_flag_big_player =										0x0000000000000001;
	const int64 e_ws_flag_cs_connect =										0x0000000000000002;
	const int64 e_ws_flag_gm_common =										0x0000000000000004;
	const int64 e_ws_flag_harry =											0x0000000000000008;
	const int64 e_ws_flag_cross_pk =										0x0000000000000010;
	const int64 e_ws_flag_overload_war =									0x0000000000000020;
	const int64 e_ws_flag_city_war =										0x0000000000000040;
	const int64 e_ws_flag_cross_boss =										0x0000000000000080;
	const int64 e_ws_flag_server_cross_time =								0x0000000000000100;
	const int64 e_ws_flag_server_refresh =									0x0000000000000200;
	const int64 e_ws_flag_time_limit_info =									0x0000000000000400;
	const int64 e_ws_flag_time_limit_template =								0x0000000000000800;
	const int64	e_ws_flag_ranking_info =									0x0000000000001000;
	const int64	e_ws_flag_load_legion_info =								0x0000000000002000;
	const int64	e_ws_flag_cross_server_state =								0x0000000000004000;
	const int64 e_ws_flag_init_time_limit_template =						0x0000000000008000;
	const int64 e_ws_flag_load_cloud_shop_info =							0x0000000000010000;
	const int64 e_ws_flag_fep_connect =										0x0000000000020000;
	const int64 e_ws_flag_attack_city =										0x0000000000040000;
///////////////////////////////////////////////////////////////////////////////////////

	enum
	{
		e_zone_uid_invalid				= 0,
		e_invalid_instance_obj_id		= 0,
		e_max_limited_broadcast_count	= 0xFFFFFFFF,
	};

	enum e_scene_type : uint32
	{
		e_scene_type_low_world,
		e_scene_type_low_seam_world,
		e_scene_type_low_instance,
		e_scene_type_low_new_instance,
		e_scene_type_low_instance_object,
		e_scene_type_low_map,
		e_scene_type_low_load_area,
		e_scene_type_low_nouse0,
		e_scene_type_low_guild,
		e_scene_type_low_battle,
		e_scene_type_low_nouse1,
		e_scene_type_low_exit,
		e_scene_type_low_local = 0xffffffff,
	};

	enum e_need_server_cross
	{
		e_need_server_cross_none,
		e_need_server_cross_begin_cross,
		e_need_server_cross_begin_cross_pk,
		e_need_server_cross_begin_cross_activity,
		e_need_server_cross_begin_cross_harry,
		e_need_server_cross_begin_cross_city_war,
		e_need_server_cross_begin_cross_legion_territory_war,
		e_need_server_cross_begin_cross_boss,
		e_need_server_cross_begin_cross_ladder,
		e_need_server_cross_begin_cross_cloud,
		e_need_server_element_war,
		e_need_server_attack_city,
		e_need_server_cross_max
	};

	enum e_server_info_type
	{
		e_server_info_type_begin_cross_server_time,
		e_server_info_type_server_state,
		e_server_info_type_server_cross_state,
		e_server_info_type_begin_first_cross_server_time,
		e_server_info_type_begin_first_merge_server_time,
		e_server_info_type_time_limit_ranking_level,
		e_server_info_type_close_exchange,
		e_server_info_type_only_create_role,
		e_server_info_type_max
	};

	enum e_server_state
	{
		e_server_state_none,
		e_server_state_new,
		e_server_state_cross,
		e_server_state_merge,
		e_server_state_max
	};

	enum e_server_cross_state
	{
		e_server_cross_state_none,
		e_server_cross_state_new,
		e_server_cross_state_mid,
		e_server_cross_state_old,
		e_server_cross_state_max
	};

	enum e_http_error_code
	{
		e_http_error_code_no_error = 0,
		e_http_error_code_wrong_server_id,
		e_http_error_code_operate_failed,
		e_http_error_code_json_parse_failed,
		e_http_error_code_server_is_running,
		e_http_error_code_wrong_server_type,
		e_http_error_code_proto_parse_failed,
		e_http_error_code_db_error,
		e_http_error_code_role_online,
		e_http_error_code_time_out,
	};

	struct world_location
	{
		world_location()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		uint32			transfer_type_id;
		uint32			instance_type_id;
		s_map_pos		map_pos;
	};

#pragma pack(pop)
}

#endif
