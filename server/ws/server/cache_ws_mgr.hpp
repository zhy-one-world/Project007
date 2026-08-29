#pragma once
/********************************************************************
created: 2020年12月15日
file base: cache
author: zs
purpose: 缓存管理系统
*********************************************************************/
#ifndef _CACHE_WS_MGR_HPP_
#define _CACHE_WS_MGR_HPP_
#include "logic/type_def.hpp"
#include "Logic/legion_def.hpp"
#include "Logic/arena_def.hpp"
#include "legion/legion_ws_applicant.h"
#include "red_package/red_package_ws.h"

namespace faith
{
	class map_ws;
	class cache_ws_mgr
	{
	public:
		cache_ws_mgr();
		~cache_ws_mgr();	
	public:
		static cache_ws_mgr& get_instance()
		{
			static cache_ws_mgr s_cache_ws_mgr;
			return s_cache_ws_mgr;
		}
	public:
		//初始化
		void init();
		void clear_data();

		s_city_war_all_member_info* get_s_city_war_all_member_info_ptr();
		void back_s_city_war_all_member_info_ptr(s_city_war_all_member_info* map_ws_ptr);

		player_arena_rank* get_player_arena_rank_ptr();
		void back_player_arena_rank_ptr(player_arena_rank* player_arena_rank_ptr);

		legion_ws_applicant* get_legion_ws_applicant_ptr();
		void back_legion_ws_applicant_ptr(legion_ws_applicant* legion_ws_applicant_ptr);

		red_package_ws* get_red_package_ws_ptr();
		void back_red_package_ws_ptr(red_package_ws* red_package_ws_ptr);

	private:
		std::list<s_city_war_all_member_info*>		m_cache_s_city_war_all_member_info;
		std::list<player_arena_rank*>		m_cache_player_arena_rank;
		std::list<legion_ws_applicant*>		m_cache_legion_ws_applicant;
		std::list<red_package_ws*>			m_cache_red_package_ws;

	};
}
#endif