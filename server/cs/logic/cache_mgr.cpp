/********************************************************************
created: 2016年7月15日
file base: cache
author: zhy
purpose: 缓存管理系统
*********************************************************************/
#include "cache_mgr.hpp"
#include "logic/player.hpp"
#include"item.h"
#include"buff_item.h"
#include "map_record_set.h"
#include "npc.hpp"
#include "skill.h"
#include "skill_effect.h"

namespace faith
{
	cache_mgr::cache_mgr()
	{
		clear_data();
	}
	cache_mgr::~cache_mgr()
	{
	}	
	void cache_mgr::init()
	{
		/*int32 player_num = SERVERCONFIG->init_player_num;
		int32 npc_num = init_npc_max;
		int32 item_num = player_num * 500;
		int32 skill_num = player_num * 100;
		int32 skill_effect_num = player_num * 10 + npc_num;
		int32 buff_num = player_num * 200 + npc_num;
		int32 talent_num = player_num * 20 + 500;
		for (int32 i = 0; i < player_num; ++i)
		{
			player* player_ptr = new player();
			back_player_ptr(player_ptr);
		}
		for (int32 i = 0; i < npc_num; ++i)
		{
			npc* npc_ptr = new npc();
			back_npc_ptr(npc_ptr);
		}
		for (int32 i = 0; i < item_num; ++i)
		{
			citem* item_ptr = new citem();
			back_item_ptr(item_ptr);
		}
		for (int32 i = 0; i < skill_num; ++i)
		{
			skill* skill_ptr = new skill();
			back_skill_ptr(skill_ptr);
		}
		for (int32 i = 0; i < skill_effect_num; ++i)
		{
			skill_effect* skill_effect_ptr = new skill_effect();
			back_skill_effect_ptr(skill_effect_ptr);
		}
		for (int32 i = 0; i < buff_num; ++i)
		{
			buff_item* buff_item_ptr = new buff_item();
			back_buff_item_ptr(buff_item_ptr);
		}
		for (int32 i = 0; i < player_num; ++i)
		{
			map_record_set* map_record_set_ptr = new map_record_set();
			back_map_record_set_ptr(map_record_set_ptr);
		}
		for (int32 i = 0; i < player_num; ++i)
		{
			legion_member_score* legion_member_score_ptr = new legion_member_score();
			back_legion_member_score_ptr(legion_member_score_ptr);
		}
		for (int32 i = 0; i < talent_num; ++i)
		{
			ctalent* ctalent_ptr = new ctalent();
			back_ctalent_ptr(ctalent_ptr);
		}*/
	}
	void cache_mgr::clear_data()
	{

	}
	player* cache_mgr::get_player_ptr()
	{
		ZoneScoped;
		player* player_ptr = nullptr;
		if (m_cache_player.empty())
		{
			player_ptr = new player();
			player_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_player.begin();
			player_ptr = *it;
			m_cache_player.pop_front();
		}
		return player_ptr;
	}
	void cache_mgr::back_player_ptr(player* player_ptr)
	{
		ZoneScoped;
		if (nullptr == player_ptr)
		{
			return;
		}
		player_ptr->del_obj_lua();
		player_ptr->clear_data();
		m_cache_player.push_back(player_ptr);
	}
	npc* cache_mgr::get_npc_ptr()
	{
		ZoneScoped;
		npc* npc_ptr = nullptr;
		if (m_cache_npc.empty())
		{
			npc_ptr = new npc();
			npc_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_npc.begin();
			npc_ptr = *it;
			m_cache_npc.pop_front();
		}
		return npc_ptr;
	}
	void cache_mgr::back_npc_ptr(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr)
		{
			return;
		}
		npc_ptr->clear_data();
		m_cache_npc.push_back(npc_ptr);

	}
	citem* cache_mgr::get_item_ptr()
	{
		ZoneScoped;
		citem* item_ptr = nullptr;
		if (m_cache_item.empty())
		{
			item_ptr = new citem();
			item_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_item.begin();
			item_ptr = *it;
			m_cache_item.pop_front();
		}
		return item_ptr;
	}
	void cache_mgr::back_item_ptr(citem* item_ptr)
	{
		ZoneScoped;
		if (nullptr == item_ptr)
		{
			return;
		}
		item_ptr->clear_data();
		m_cache_item.push_back(item_ptr);

	}
	skill* cache_mgr::get_skill_ptr()
	{
		ZoneScoped;
		skill* skill_ptr = nullptr;
		if (m_cache_skill.empty())
		{
			skill_ptr = new skill();
			skill_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_skill.begin();
			skill_ptr = *it;
			m_cache_skill.pop_front();
		}
		return skill_ptr;
	}
	void cache_mgr::back_skill_ptr(skill* skill_ptr)
	{
		ZoneScoped;
		if (nullptr == skill_ptr)
		{
			return;
		}
		skill_ptr->clear_data();
		m_cache_skill.push_back(skill_ptr);

	}
	skill_effect* cache_mgr::get_skill_effect_ptr()
	{
		ZoneScoped;
		skill_effect* skill_effect_ptr = nullptr;
		if (m_cache_skill_effect.empty())
		{
			skill_effect_ptr = new skill_effect();
			skill_effect_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_skill_effect.begin();
			skill_effect_ptr = *it;
			m_cache_skill_effect.pop_front();
		}
		return skill_effect_ptr;
	}
	void cache_mgr::back_skill_effect_ptr(skill_effect* skill_effect_ptr)
	{
		ZoneScoped;
		if (nullptr == skill_effect_ptr)
		{
			return;
		}
		skill_effect_ptr->clear_data();
		m_cache_skill_effect.push_back(skill_effect_ptr);

	}
	buff_item* cache_mgr::get_buff_item_ptr()
	{
		ZoneScoped;
		buff_item* buff_item_ptr = nullptr;
		if (m_cache_buff_item.empty())
		{
			buff_item_ptr = new buff_item();
			buff_item_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_buff_item.begin();
			buff_item_ptr = *it;
			m_cache_buff_item.pop_front();
		}
		return buff_item_ptr;
	}
	void cache_mgr::back_buff_item_ptr(buff_item* buff_item_ptr)
	{
		ZoneScoped;
		if (nullptr == buff_item_ptr)
		{
			return;
		}
		buff_item_ptr->clear_data();
		m_cache_buff_item.push_back(buff_item_ptr);
	}

	map_record_set* cache_mgr::get_map_record_set_ptr()
	{
		ZoneScoped;
		map_record_set* map_record_set_ptr = nullptr;
		if (m_cache_map_record_set.empty())
		{
			map_record_set_ptr = new map_record_set();
			map_record_set_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_map_record_set.begin();
			map_record_set_ptr = *it;
			m_cache_map_record_set.pop_front();
		}
		return map_record_set_ptr;
	}
	void cache_mgr::back_map_record_set_ptr(map_record_set* map_record_set_ptr)
	{
		ZoneScoped;
		if (nullptr == map_record_set_ptr)
		{
			return;
		}
		map_record_set_ptr->clear_data();
		m_cache_map_record_set.push_back(map_record_set_ptr);
	}

	legion_member_score* cache_mgr::get_legion_member_score_ptr()
	{
		ZoneScoped;
		legion_member_score* legion_member_score_ptr = nullptr;
		if (m_cache_legion_member_score.empty())
		{
			legion_member_score_ptr = new legion_member_score();
			legion_member_score_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_legion_member_score.begin();
			legion_member_score_ptr = *it;
			m_cache_legion_member_score.pop_front();
		}
		return legion_member_score_ptr;
	}
	void cache_mgr::back_legion_member_score_ptr(legion_member_score* legion_member_score_ptr)
	{
		ZoneScoped;
		if (nullptr == legion_member_score_ptr)
		{
			return;
		}
		legion_member_score_ptr->clear_data();
		m_cache_legion_member_score.push_back(legion_member_score_ptr);
	}

	ctalent* cache_mgr::get_ctalent_ptr()
	{
		ZoneScoped;
		ctalent* ctalent_ptr = nullptr;
		if (m_cache_ctalent.empty())
		{
			ctalent_ptr = new ctalent();
			ctalent_ptr->clear_data();
		}
		else
		{
			auto it = m_cache_ctalent.begin();
			ctalent_ptr = *it;
			m_cache_ctalent.pop_front();
		}
		return ctalent_ptr;
	}
	void cache_mgr::back_ctalent_ptr(ctalent* ctalent_ptr)
	{
		ZoneScoped;
		if (nullptr == ctalent_ptr)
		{
			return;
		}
		ctalent_ptr->clear_data();
		m_cache_ctalent.push_back(ctalent_ptr);
	}
}