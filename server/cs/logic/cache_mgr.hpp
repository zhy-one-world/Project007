#pragma once
/********************************************************************
created: 2016年7月15日
file base: cache
author: zhy
purpose: 缓存管理系统
*********************************************************************/
#ifndef _CACHE_MGR_HPP_
#define _CACHE_MGR_HPP_
#include "logic/type_def.hpp"
#include "Logic/legion_def.hpp"

namespace faith
{
	class player;
	class npc;
	class citem;
	class skill;
	class skill_effect;
	class buff_item;
	class map_record_set;
	class ctalent;
	class ctitle;
	class cache_mgr
	{
	public:
		cache_mgr();
		~cache_mgr();	
	public:
		static cache_mgr& get_instance()
		{
			static cache_mgr s_cache_mgr;
			return s_cache_mgr;
		}
	public:
		//初始化
		void init();
		void clear_data();
		player* get_player_ptr();
		void back_player_ptr(player* player_ptr);
		npc* get_npc_ptr();
		void back_npc_ptr(npc* npc_ptr);
		citem* get_item_ptr();
		void back_item_ptr(citem* item_ptr);
		skill* get_skill_ptr();
		void back_skill_ptr(skill* skill_ptr);
		skill_effect* get_skill_effect_ptr();
		void back_skill_effect_ptr(skill_effect* skill_effect_ptr);
		buff_item* get_buff_item_ptr();
		void back_buff_item_ptr(buff_item* buff_item_ptr);
		map_record_set* get_map_record_set_ptr();
		void back_map_record_set_ptr(map_record_set* map_record_set_ptr);
		legion_member_score* get_legion_member_score_ptr();
		void back_legion_member_score_ptr(legion_member_score* legion_member_score_ptr);
		ctalent* get_ctalent_ptr();
		void back_ctalent_ptr(ctalent* ctalent_ptr);
	private:
		std::list<player*>		m_cache_player;
		std::list<npc*>			m_cache_npc;
		std::list<citem*>		m_cache_item;
		std::list<skill*>		m_cache_skill;
		std::list<skill_effect*>m_cache_skill_effect;
		std::list<buff_item*>	m_cache_buff_item;
		std::list<map_record_set*>	m_cache_map_record_set;
		std::list<legion_member_score*>	m_cache_legion_member_score;
		std::list<ctalent*>		m_cache_ctalent;
	};
}
#endif