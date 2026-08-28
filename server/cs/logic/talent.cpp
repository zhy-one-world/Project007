/********************************************************************
created: 2017-5-26 16:36:18
file base: talent
file ext: cpp
author: zhy

purpose: 
*********************************************************************/

#include "talent.h"
#include "template/template_manager.h"
#include "logic/buff_man.h"
#include "logic/skill_manager.h"
#include "buff_def.hpp"
namespace hld
{
	ctalent::ctalent(void)
	{
		clear_data();
	}
	ctalent::~ctalent(void)
	{
	}

	void ctalent::heart_tick(const int64& new_time)
	{
		if (is_valid() == false)
		{
			return;
		}

	}
	void ctalent::clear_data()
	{
		m_is_data_use = false;
		m_talent_data.clear_data();
		m_talent_template_ptr = nullptr;
	}

	bool& ctalent::is_valid()
	{
		return m_is_data_use;
	}
	void ctalent::set_talent_data(const s_talent_info& talent_data)
	{
		memcpy(&m_talent_data, &talent_data, sizeof(m_talent_data));
		m_talent_template_ptr = GET_TEMPLATE(TalentTemplate, m_talent_data.data_ary[e_talent_info_template_id]);
	}
	int32 ctalent::get_talent_series()
	{
		if (m_talent_template_ptr)
		{
			return m_talent_template_ptr->Series;
		}
		return 0;
	}
	void ctalent::set_talent_buff(const int32& unit_array_index, bool is_add)
	{
		if (NULL == m_talent_template_ptr)
		{
			return;
		}
		if (is_add)
		{
			buff_ex_env_param penv;
			if (e_talent_buff_add_type_accumulate == m_talent_template_ptr->BuffAddType)
			{
				penv.buff_level = m_talent_template_ptr->Level;
			}
			else
			{
				penv.buff_level = 1;
			}
			
			for (int32 i = 0; i < m_talent_template_ptr->BuffArray.size(); ++i)
			{
				buff_man::add_buff_inst(unit_array_index, unit_array_index, m_talent_template_ptr->BuffArray[i], &penv);
			}
			
		}
		else
		{
			for (int32 i = 0; i < m_talent_template_ptr->BuffArray.size(); ++i)
			{
				buff_man::del_buff_by_template_id(unit_array_index, unit_array_index, m_talent_template_ptr->BuffArray[i]);
			}
		}
	}

	void ctalent::set_talent_skill(const int32& unit_array_index, bool is_add)
	{
		if (NULL == m_talent_template_ptr)
		{
			return;
		}
		if (m_talent_template_ptr->ReplaceSkill.size() == 0 || m_talent_template_ptr->ReplaceSkill.size() % 2 != 0)
		{
			return;
		}
		for (int32 i = 0; i < m_talent_template_ptr->ReplaceSkill.size(); i += 2)
		{
			if (m_talent_template_ptr->ReplaceSkill[i] > 0 && m_talent_template_ptr->ReplaceSkill[i + 1] > 0)
			{
				skill_manager::replace_skill_id(unit_array_index, m_talent_template_ptr->ReplaceSkill[i], m_talent_template_ptr->ReplaceSkill[i + 1], is_add);
			}
		}
	}
	int32 ctalent::get_talent_gs_value()
	{
		if (0 >= get_data_info(e_talent_info_cast_money))
		{
			return 0;
		}

		TalentTemplate* temp_talent_template_ptr = get_talent_template_ptr();
		if (nullptr == temp_talent_template_ptr)
		{
			return 0;
		}
		return temp_talent_template_ptr->GSAddValue * m_talent_template_ptr->Level;
	}

	void ctalent::lua_set_talent_data(int32 template_id, int32 cast_money)
	{
		s_talent_info tmp;
		tmp.data_ary[e_talent_info_template_id] = template_id;
		tmp.data_ary[e_talent_info_cast_money] = cast_money;
		set_talent_data(tmp);
	}

}
