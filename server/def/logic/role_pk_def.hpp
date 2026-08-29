/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-27 13:40
	
	File Name :	role_pk_define.h
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/
#ifndef _ROLE_PK_DEFINE_H_
#define _ROLE_PK_DEFINE_H_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include <Logic/char_def.hpp>
#include "Logic/map_def.hpp"
//////////////////////////////////////////////////////////////////////////
//
//	Macro And Struct Define
//
//////////////////////////////////////////////////////////////////////////
namespace faith
{
#pragma pack(push,1)
	const int32 ROLE_PK_RANKING_TOP_TIME = 600000;
	const int32 ROLE_PK_WAR_TIME = 600000;
	const int32 PK_RANKING_WAR_NUM = 2;
	const int32 PK_RANKING_MAP_1v1 = 71160003;
	const uint32 PK_RANKING_TOP_NUM = 10;
	const uint32 PK_RANKING_TEAM_NUM = 3;
	const uint32 CROSS_WAR_INFO_NUM = 50;
	enum e_team_num
	{
		e_team_num_null,
		e_team_num_1,
		e_team_num_2,
		e_team_num_3
	};
	enum e_pk_state
	{
		e_pk_state_no_join,//未报名
		e_pk_state_success,//报名成功
		e_pk_state_join_war,//匹配成功
		e_pk_state_begin_loading,//可以加载
	};
	enum e_role_pk_info
	{
		e_role_pk_info_template_id,
		e_role_pk_info_server_id,
		e_role_pk_info_gs,
		e_role_pk_info_war_num,
		e_role_pk_info_win_num,
		e_role_pk_info_last_win,
		e_role_pk_info_join_num,
		e_role_pk_info_ladder_score,
		e_role_pk_info_ranking_num,
		e_role_pk_info_refresh_time,
		e_role_pk_info_max
	};

	enum e_server_info_type_string
	{
		e_server_info_type_string_ip,
		e_server_info_type_string_port,
		e_server_info_type_string_max
	};
	struct s_role_pk_info
	{
		guid_64						role_guid;
		xchar						role_name[max_name_size + 1];
		int32						data_array[e_role_pk_info_max];
		s_role_pk_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_role_pk_result
	{
		xchar				role_name[max_name_size + 1];
		guid_64				role_guid;
		int32				war_result;
		s_role_pk_result()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_cross_war_db
	{
		xchar				target_name_1[max_name_size + 1];
		xchar				target_name_2[max_name_size + 1];
		xchar				target_name_3[max_name_size + 1];
		int32				ladder_score;
		int32				win_result;
		int32				cross_honor;
		s_cross_war_db()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_role_pk_team_info
	{
		guid_64				team_1[PK_RANKING_TEAM_NUM];
		guid_64				team_2[PK_RANKING_TEAM_NUM];
		s_role_pk_team_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};
#pragma pack(pop)
}
#endif