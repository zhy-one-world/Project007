/********************************************************************
created: 2016/7/14
created: 14:7:2016 15:55:12
file base: pokedex
file ext: h
author: XingLong

purpose: 
*********************************************************************/
#ifndef _POKEDEX_H_
#define _POKEDEX_H_

#include <Logic/pokedex_def.hpp>
#include "template/ServerTemplateHead.h"

namespace faith
{
	class cpokedex
	{
	public:
		cpokedex();
		~cpokedex();
	public: 
		void									clear_data();																		 //清空图鉴信息
		
	public:
		int32									get_template_id() { return m_pokedex_data.data_ary[EPokedexInfo_template_id]; }
		void									set_pokedex_info(const s_pokedex_info&  pokedex_data);								//设置图鉴的全部实例信息
		void									set_pokedex_template_id(const int32&  pokedex_template_id);
		s_pokedex_info&							get_data_info() { return m_pokedex_data; }
		int32									get_data_info(int32 eIndex);														//得到某种个精魄的数量	
		void									set_data_info(e_pokedex_Info eIndex, int32 iValue);
		int32									get_psyche_num(int32 eIndex);
		void                                    add_psyche_num(int32 eIndex, int32 iValue);											//添加精魄数量
		PokedexTemplate*						get_pokedex_info_ptr();

		bool									is_single_target_active(int32 index);
		PsycheTemplate*							get_single_psyche_ptr(int32 index);
		bool									is_all_active();
		bool									is_psyche_submit_finish(int32 psyche_item_template_id);

		int32									get_active_num();//得到激活数量

		bool									is_active();

	private:
		s_pokedex_info							m_pokedex_data;																					//图鉴实例信息
		PokedexTemplate*						m_pokedex_info_ptr;																				//图鉴模板指针
	};
}
#endif