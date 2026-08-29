/********************************************************************
created: 2016/4/21
created: 14:7:2016 16:12:22
file base: pokedex
file ext: h
author: XingLong

purpose:
*********************************************************************/

#include "pokedex.h"
#include "att_def.hpp"
#include "utility/guid_gen.h"
#include "template/template_manager.h"

namespace faith
{
	cpokedex::cpokedex()
	{
		clear_data();
	
	}

	cpokedex::~cpokedex()
	{
		clear_data();
	}

	void cpokedex::clear_data()
	{
		m_pokedex_data.clear_data();
		m_pokedex_info_ptr = nullptr;
	}

	void cpokedex::set_pokedex_info(const s_pokedex_info& pokedex_data)
	{
		m_pokedex_data = pokedex_data;
		m_pokedex_info_ptr = GET_TEMPLATE(PokedexTemplate, m_pokedex_data.data_ary[e_pokedex_Info_template_id]);
	}

	void cpokedex::set_pokedex_template_id(const int32&  pokedex_template_id)
	{
		m_pokedex_data.data_ary[e_pokedex_Info_template_id] = pokedex_template_id;
		m_pokedex_info_ptr = GET_TEMPLATE(PokedexTemplate, pokedex_template_id + m_pokedex_data.data_ary[e_pokedex_Info_upgrade_num]);
	}


	int32 cpokedex::get_data_info(int32 eIndex)
	{
		if (eIndex < e_pokedex_Info_template_id || eIndex >= e_pokedex_Info_max)
		{
			return 0;
		}
		 return m_pokedex_data.data_ary[eIndex]; 
	}

	void cpokedex::set_data_info(e_pokedex_Info eIndex, int32 iValue)
	{
		if (eIndex < e_pokedex_Info_max)
		{
			m_pokedex_data.data_ary[eIndex] = iValue;
		}
	}

	void cpokedex::add_psyche_num(int32 eIndex, int32 iValue)
	{
		if (eIndex < 0 || eIndex >= PER_POKEDEX_PSYCHE_NUM)
		{
			return;
		}
		 m_pokedex_data.collect_num[eIndex] += iValue; 
	}

	PokedexTemplate * cpokedex::get_pokedex_info_ptr()
	{
		return m_pokedex_info_ptr = GET_TEMPLATE(PokedexTemplate, m_pokedex_data.data_ary[e_pokedex_Info_template_id] + m_pokedex_data.data_ary[e_pokedex_Info_upgrade_num]); 
	}

	bool cpokedex::is_single_target_active(int32 index) 
	{
		if (m_pokedex_info_ptr == nullptr)
		{
			return false;
		}
		PokedexTemplate* temp_pokedex_ptr = GET_TEMPLATE(PokedexTemplate, get_template_id());
		if (nullptr == temp_pokedex_ptr
			|| index >= temp_pokedex_ptr->PsycheIdArray.size()
			|| index < 0
			|| index >= PER_POKEDEX_PSYCHE_NUM)
		{
			return false;
		}

		ItemTemplate* item_psyche_ptr = GET_TEMPLATE(ItemTemplate, temp_pokedex_ptr->PsycheIdArray[index]);
		if (nullptr == item_psyche_ptr)
		{
			return false;
		}
		PsycheTemplate* temp_psyche_ptr = GET_TEMPLATE(PsycheTemplate, item_psyche_ptr->logic_id);
		if (nullptr == temp_psyche_ptr)
		{
			return false;
		}

		return m_pokedex_data.collect_num[index] >= temp_psyche_ptr->PsycheNum;
	}

	PsycheTemplate* cpokedex::get_single_psyche_ptr(int32 index)
	{
		if (m_pokedex_info_ptr == nullptr)
		{
			return nullptr;
		}
		if (index < 0
			|| index >= m_pokedex_info_ptr->PsycheIdArray.size())
		{
			return nullptr;
		}

		ItemTemplate* temp_item_template_ptr = GET_TEMPLATE(ItemTemplate, m_pokedex_info_ptr->PsycheIdArray[index]);
		if (temp_item_template_ptr == nullptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(PsycheTemplate, temp_item_template_ptr->logic_id);
	}

	bool cpokedex::is_all_active()
	{
		PokedexTemplate* temp_pokedex_ptr = GET_TEMPLATE(PokedexTemplate, get_template_id());
		if (nullptr == temp_pokedex_ptr)
		{
			return false;
		}

		bool all_active = true;
		for (uint32 i = 0; i < temp_pokedex_ptr->PsycheIdArray.size(); ++i)
		{
			ItemTemplate* psyche_item_ptr = GET_TEMPLATE(ItemTemplate, temp_pokedex_ptr->PsycheIdArray[i]);
			if (psyche_item_ptr == nullptr)
			{
				return false;
			}
			PsycheTemplate* temp_psyche_ptr = GET_TEMPLATE(PsycheTemplate, psyche_item_ptr->logic_id);
			if (nullptr == temp_psyche_ptr)
			{
				return false;
			}

			if (m_pokedex_data.collect_num[i] < temp_psyche_ptr->PsycheNum)
			{
				all_active = false;
				break;
			}
		}
		return all_active;
	}
	bool cpokedex::is_active()
	{
		return m_pokedex_data.data_ary[e_pokedex_Info_state] == 1;
	}

	int32 cpokedex::get_psyche_num(int32 eIndex)
	{ 
		if (eIndex < 0 || eIndex >= PER_POKEDEX_PSYCHE_NUM)
		{
			return 0;
		}
		return m_pokedex_data.collect_num[eIndex]; 
	}

	bool cpokedex::is_psyche_submit_finish(int32 psyche_item_template_id)
	{
		//if (m_pokedex_info_ptr == nullptr)
		//{
		//	return true;
		//}
		//ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, psyche_item_template_id);
		//if (item_template_ptr == nullptr)
		//{
		//	return true;
		//}

		//PsycheTemplate* psyche_template_ptr = GET_TEMPLATE(PsycheTemplate, item_template_ptr->logic_id);
		//if (psyche_template_ptr == nullptr)
		//{
		//	return true;
		//}

		//for (int32 i = 0; i < m_pokedex_info_ptr->PsycheIdArray.size(); ++i)
		//{
		//	if (psyche_item_template_id == m_pokedex_info_ptr->PsycheIdArray[i]
		//		&& i < PER_POKEDEX_PSYCHE_NUM)
		//	{
		//		return get_psyche_num(i) >= psyche_template_ptr->PsycheNum;
		//	}
		//}
		return true;
	}

	int32 cpokedex::get_active_num()
	{
		PokedexTemplate* temp_pokedex_ptr = GET_TEMPLATE(PokedexTemplate, get_template_id());
		if (nullptr == temp_pokedex_ptr)
		{
			return false;
		}

		int32 active_num = 0;
		for (uint32 i = 0; i < temp_pokedex_ptr->PsycheIdArray.size(); ++i)
		{
			ItemTemplate* psyche_item_ptr = GET_TEMPLATE(ItemTemplate, temp_pokedex_ptr->PsycheIdArray[i]);
			if (psyche_item_ptr == nullptr)
			{
				return false;
			}
			PsycheTemplate* temp_psyche_ptr = GET_TEMPLATE(PsycheTemplate, psyche_item_ptr->logic_id);
			if (nullptr == temp_psyche_ptr)
			{
				return false;
			}

			if (m_pokedex_data.collect_num[i] == temp_psyche_ptr->PsycheNum)
			{
				active_num++;
			}
		}
		return active_num;
	}

}