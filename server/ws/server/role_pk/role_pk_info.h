/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-17 11:14
	
	File Name :	role_pk_info.h
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

#ifndef _ROLE_PK_INFO_H_
#define _ROLE_PK_INFO_H_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "Logic/role_pk_def.hpp"
#include "utility/cs_date.hpp"
//////////////////////////////////////////////////////////////////////////
//
//	Class Declare
//
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	class c_role_pk_info
	{
	public:
		c_role_pk_info();
		~c_role_pk_info();
		bool is_remove();
		void heart_tick(float deltaseconds);
		void refresh_honor();

	public:
		void set_role_pk_info(s_role_pk_info& pk_info);
		bool is_valid() { return m_role_pk_info.role_guid.is_valid(); }

		const xchar* get_role_name() const { return m_role_pk_info.role_name; }
		void set_role_name(xchar* role_name){	memcpy(m_role_pk_info.role_name, role_name, max_name_size);}
		void set_role_name(const xstring& role_name);
		const guid_64& get_role_guid()const { return m_role_pk_info.role_guid; }
		const int32& get_role_info(e_role_pk_info info_type) const { return m_role_pk_info.data_array[info_type]; }

		const int32 get_role_pk_info(e_role_pk_info info_index) const;
		void set_role_pk_info(e_role_pk_info info_index, int32 info_value);

		e_pk_state get_pk_state() { return m_pk_state; }
		void set_pk_state(e_pk_state pk_state) { m_pk_state = pk_state; }

		bool get_game_state() { return m_in_game; }
		void set_game_state(bool in_game) {
			m_in_game = in_game; 
			m_leave_game_time = time_helper::get_cur_time_new().second;
		}

		void game_win(const guid_64 tar_team[PK_RANKING_TEAM_NUM]);
		void game_lost(const guid_64 tar_team[PK_RANKING_TEAM_NUM]);
		void game_peace(const guid_64 tar_team[PK_RANKING_TEAM_NUM]);

		void game_settle_common(e_map_war_result fight_ret, const guid_64 tar_team[PK_RANKING_TEAM_NUM]);

		const guid_64& get_cur_war_map() { return m_cur_war_map; }
		void set_cur_war_map(const guid_64& map_guid) { m_cur_war_map = map_guid; }
		void clear_cur_war_map() { m_cur_war_map.clear_data(); }

	private:
		s_role_pk_info			m_role_pk_info;
		e_pk_state				m_pk_state;
		bool					m_in_game;
		i64						m_leave_game_time;
		guid_64					m_cur_war_map;
	};
}


#endif