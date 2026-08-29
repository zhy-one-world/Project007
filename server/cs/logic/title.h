#pragma once
/********************************************************************
created: 2016年5月14日
file base: title
author: zhy
purpose: 单个称号
*********************************************************************/

#ifndef _TITLE_HPP_
#define _TITLE_HPP_


#include "Logic/title_def.hpp"
#include "template/template_manager.h"

namespace faith
{
	class ctitle
	{
	public:
		ctitle();
		~ctitle();
	public:
		void tick(float elapse_time);
		void clear_data();

		void init_title_by_template(int32 title_id);
		void init_title_by_info(const s_title_info& title_info);

		TitleTemplate* get_title_template_ptr() { return m_title_template_ptr; }
		s_title_info& get_title_info_all() { return m_title_info; }
		int32 get_inst_data(e_title_info index) const;
		void set_inst_data(e_title_info index, int32 num);
		void set_title_static_att(int32 unit_index, bool is_add);
		void set_title_equiping_att(int32 unit_index, bool is_add);
	private:
		TitleTemplate* m_title_template_ptr;
		s_title_info   m_title_info;
	};
}

#endif