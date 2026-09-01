/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-17 11:14
	
	File Name :	role_pk_manager.h
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

#ifndef _ROLE_PK_MANAGER_H_
#define _ROLE_PK_MANAGER_H_

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include <unordered_map>
#include <common/singleton.hpp>
#include "role_pk_info.h"
#include "Logic/gate_def.hpp"

//////////////////////////////////////////////////////////////////////////
//	Class Declare
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	typedef std::unordered_map<ui64, c_role_pk_info> all_role_pk_map;
	typedef all_role_pk_map::iterator all_role_pk_map_it;

	typedef std::list<c_role_pk_info*> sign_up_list;
	typedef std::unordered_map<int32, sign_up_list> all_sign_up_map;
	typedef all_sign_up_map::iterator all_sign_up_map_it;

	typedef std::unordered_map<ui64, s_role_pk_team_info> all_team_map;
	typedef all_team_map::iterator all_team_map_it;


	class role_pk_manager : public faith::singleton<role_pk_manager>
	{
		friend class faith::singleton<role_pk_manager>;
	public:
		role_pk_manager();
		~role_pk_manager();
	public:
		void tick(float tick_time);
		void db_role_pk_top();
		void group_pk_join();
	public:
		void remove_role_pk_info(const guid_64& role_guid);
		c_role_pk_info* get_role_pk_info(const guid_64& role_guid);
		void add_role_pk_info(const guid_64& role_guid, xchar* role_name, int32 template_id, int32 role_gs);
		void dp_role_pk_info(s_role_pk_info& pk_info, int32 top_num = -1);
		void send_role_pk_info(const c_role_pk_info& pk_info);
		void send_group_success(const guid_64& role_guid, int32 server_id, const guid_64& another_role_guid, const guid_64& map_guid, int32 war_index);
		guid_64 get_player_role_pk_map(const guid_64& role_guid);
		void clear_player_role_pk_map(const guid_64& role_guid);

	public:
		e_cross_sign_up_result sign_up(const guid_64& role_guid, guid_64& team_guid);
		e_cross_sign_up_result sign_up_leave(const guid_64& role_guid, guid_64& team_guid);
		void role_pk_result(const guid_64& finish_map_guid, e_team_num win_team);
		const s_role_pk_team_info& get_war_map_team_map(guid_64 map_guid) { return m_all_war_map_team_map[map_guid.server_64]; }

		//reanme
		void change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
	private:
		//void 
	private:
		all_sign_up_map m_all_sign_up_map;
		all_role_pk_map m_all_role_pk_map;
		all_team_map	m_all_war_map_team_map;
		c_role_pk_info	m_ranking_top[PK_RANKING_TOP_NUM];
		i64				m_pk_ranking_top_refresh;
	};
}


#endif