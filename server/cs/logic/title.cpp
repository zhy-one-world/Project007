/********************************************************************
created: 2016年7月14日
file base: title
author: zhy
purpose: 单个称号
*********************************************************************/


#include "title.h"
#include "logic/unit_man.h"
#include "achievement_def.h"
#include "logic/player.hpp"

namespace hld
{
	ctitle::ctitle()
	{
		clear_data();
	}
	ctitle::~ctitle()
	{
	}
	void ctitle::tick(float elapse_time)
	{
	}
	void ctitle::clear_data()
	{
		m_title_info.clear_data();
		m_title_template_ptr = nullptr;
	}
	void ctitle::init_title_by_template(int32 title_id)
	{
		m_title_info.data_ary[e_title_info_template_id] = title_id;
		m_title_template_ptr = GET_TEMPLATE(TitleTemplate, title_id);
	}
	void ctitle::init_title_by_info(const s_title_info& title_info)
	{
		m_title_info = title_info;
		m_title_template_ptr = GET_TEMPLATE(TitleTemplate, title_info.data_ary[eaid_id]);
	}

	int32 ctitle::get_inst_data(e_title_info index) const
	{
		if (index < e_title_info_max)
		{
			return m_title_info.data_ary[index];
		}
		else return -1;
	}
	void ctitle::set_inst_data(e_title_info index, int32 num)
	{
		if (index < e_title_info_max)
		{
			m_title_info.data_ary[index] = num;
		}
	}
	void ctitle::set_title_static_att(int32 unit_index, bool is_add)
	{
		if (m_title_template_ptr == nullptr)
		{
			return;
		}
		player& player_ref = unit_man::get_player(unit_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (m_title_info.data_ary[e_title_info_if_geted])
		{
			player_ref.get_pawn_att().apply_att_change_by_array(m_title_template_ptr->StaticAttArray, is_add);
		}
	}
	void ctitle::set_title_equiping_att(int32 unit_index, bool is_add)
	{
		if (m_title_template_ptr == nullptr)
		{
			return;
		}
		player& player_ref = unit_man::get_player(unit_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (m_title_template_ptr->attribute_id == player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_equip_title_id))
		{
			player_ref.get_pawn_att().apply_att_change_by_array(m_title_template_ptr->AdditionArray, is_add);
		}
	}
}

