/********************************************************************
created: 2020年12月15日
file base: cache
author: zs
purpose: 缓存管理系统
*********************************************************************/
#include "cache_ws_mgr.hpp"
#include "game_cfg/servers_config.h"



namespace faith
{
	cache_ws_mgr::cache_ws_mgr()
	{
		clear_data();
	}
	cache_ws_mgr::~cache_ws_mgr()
	{
	}	
	void cache_ws_mgr::init()
	{
		int32 player_num = SERVERCONFIG->init_player_num;
		int32 red_num = red_package_total_num * (float)((float)player_num / (float)init_normal_max);

		for (int32 i = 0; i < player_num; ++i)
		{
			legion_ws_applicant* legion_ws_applicant_ptr = new legion_ws_applicant();
			back_legion_ws_applicant_ptr(legion_ws_applicant_ptr);
		}

		for (int32 i = 0; i < red_num; ++i)
		{
			red_package_ws* red_package_ws_ptr = new red_package_ws();
			back_red_package_ws_ptr(red_package_ws_ptr);
		}
	}
	void cache_ws_mgr::clear_data()
	{

	}

	s_city_war_all_member_info* cache_ws_mgr::get_s_city_war_all_member_info_ptr()
	{
		s_city_war_all_member_info* s_city_war_all_member_info_ptr = nullptr;
		if (m_cache_s_city_war_all_member_info.empty())
		{
			s_city_war_all_member_info_ptr = new s_city_war_all_member_info();
			s_city_war_all_member_info_ptr->clear_data();
		}
		else
		{
			std::list<s_city_war_all_member_info*>::iterator it = m_cache_s_city_war_all_member_info.begin();
			s_city_war_all_member_info_ptr = *it;
			m_cache_s_city_war_all_member_info.pop_front();
		}
		return s_city_war_all_member_info_ptr;
	}
	void cache_ws_mgr::back_s_city_war_all_member_info_ptr(s_city_war_all_member_info* s_city_war_all_member_info_ptr)
	{
		if (nullptr == s_city_war_all_member_info_ptr)
		{
			return;
		}
		s_city_war_all_member_info_ptr->clear_data();
		m_cache_s_city_war_all_member_info.push_back(s_city_war_all_member_info_ptr);
	}

	player_arena_rank* cache_ws_mgr::get_player_arena_rank_ptr()
	{
		player_arena_rank* player_arena_rank_ptr = nullptr;
		if (m_cache_player_arena_rank.empty())
		{
			player_arena_rank_ptr = new player_arena_rank();
			player_arena_rank_ptr->clear_data();
		}
		else
		{
			std::list<player_arena_rank*>::iterator it = m_cache_player_arena_rank.begin();
			player_arena_rank_ptr = *it;
			m_cache_player_arena_rank.pop_front();
		}
		return player_arena_rank_ptr;
	}
	void cache_ws_mgr::back_player_arena_rank_ptr(player_arena_rank* player_arena_rank_ptr)
	{
		if (nullptr == player_arena_rank_ptr)
		{
			return;
		}
		player_arena_rank_ptr->clear_data();
		m_cache_player_arena_rank.push_back(player_arena_rank_ptr);
	}

	legion_ws_applicant * cache_ws_mgr::get_legion_ws_applicant_ptr()
	{
		legion_ws_applicant* legion_ws_applicant_ptr = nullptr;
		if (m_cache_legion_ws_applicant.empty())
		{
			legion_ws_applicant_ptr = new legion_ws_applicant();
			legion_ws_applicant_ptr->clear_data();
		}
		else
		{
			std::list<legion_ws_applicant*>::iterator it = m_cache_legion_ws_applicant.begin();
			legion_ws_applicant_ptr = *it;
			m_cache_legion_ws_applicant.pop_front();
		}
		return legion_ws_applicant_ptr;
	}

	void cache_ws_mgr::back_legion_ws_applicant_ptr(legion_ws_applicant* legion_ws_applicant_ptr)
	{
		if (nullptr == legion_ws_applicant_ptr)
		{
			return;
		}
		legion_ws_applicant_ptr->clear_data();
		m_cache_legion_ws_applicant.push_back(legion_ws_applicant_ptr);
	}

	red_package_ws * cache_ws_mgr::get_red_package_ws_ptr()
	{
		red_package_ws* red_package_ws_ptr = nullptr;
		if (m_cache_red_package_ws.empty())
		{
			red_package_ws_ptr = new red_package_ws();
			red_package_ws_ptr->clear_data();
		}
		else
		{
			std::list<red_package_ws*>::iterator it = m_cache_red_package_ws.begin();
			red_package_ws_ptr = *it;
			m_cache_red_package_ws.pop_front();
		}
		return red_package_ws_ptr;
	}

	void cache_ws_mgr::back_red_package_ws_ptr(red_package_ws* red_package_ws_ptr)
	{
		if (nullptr == red_package_ws_ptr)
		{
			return;
		}
		red_package_ws_ptr->clear_data();
		m_cache_red_package_ws.push_back(red_package_ws_ptr);
	}
}