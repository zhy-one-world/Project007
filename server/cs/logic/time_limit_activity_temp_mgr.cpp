/********************************************************************
  created: 2019-7-22
  file base: time_limit_activity_temp_mgr
  file ext: CPP
  author: zhangshuo
  
  purpose: 
*********************************************************************/

/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include "cell_server.hpp"
#include "utility/init_unit.h"
#include "Logic/time_limit_activity_temp_mgr.hpp"
#include "logic/unit_man.h"
#include "template/template_manager.h"
#include "time_def.hpp"

namespace hld
{
	time_limit_activity_temp_mgr::time_limit_activity_temp_mgr()
	{
		clear_data();
	}

	void time_limit_activity_temp_mgr::clear_data()
	{
		m_time_limit_activity_temp_map.clear();
		m_temporary_time_limit_activity_temp_map.clear();
		m_time_limit_activity_branch_temp_map.clear();
		m_temporary_time_limit_activity_branch_temp_map.clear();
		m_empty_limit_act_temp.clear_data();
		m_empty_limit_act_branch_temp.clear_data();
	}

	void time_limit_activity_temp_mgr::init_manager()
	{

	}

	void time_limit_activity_temp_mgr::heart_tick(const int64& time_new)
	{
		ZoneScoped;
	
	}

	void time_limit_activity_temp_mgr::end_act_with_act_type(int32 act_type, bool is_need_send)
	{
		if (act_type < e_time_limit_activity_type_total_charge || act_type >= e_time_limit_activity_type_max)
		{
			return;
		}
		for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end(); ite++)
		{
			if (ite->second.ActivityType == act_type)
			{
				ite->second.IsEndAct = 1;
			}
		}
		if (is_need_send)
		{
			unit_man::refresh_all_player_time_limit();
		}
	}

	void time_limit_activity_temp_mgr::recv_cs2ws_limit_act_temp(const s_time_limit_activity_temp_db * temp_db_info, int32 data_num, bool is_begin)
	{
		if (data_num > max_send_cs2ws_once_num)
		{
			return;
		}
		if (is_begin)
		{
			m_temporary_time_limit_activity_temp_map.clear();
			m_temporary_time_limit_activity_branch_temp_map.clear();
		}
		for (int32 i = 0; i < data_num; i++)
		{
			int32 act_guid = temp_db_info[i].ActGuid;
			if (act_guid > 0)
			{
				s_time_limit_activity_temp temp_info;
				parse_char_temp(temp_info, temp_db_info[i]);
				m_temporary_time_limit_activity_temp_map[act_guid] = temp_info;
			}
		}
	}

	void time_limit_activity_temp_mgr::recv_cs2ws_limit_act_branch_temp(const s_time_limit_activity_branch_temp_db * temp_db_info, int32 data_num, bool is_end)
	{
		if (data_num > max_send_cs2ws_once_num)
		{
			return;
		}
		for (int32 i = 0; i < data_num; i++)
		{
			int32 act_guid = temp_db_info[i].ActGuid;
			if (act_guid > 0)
			{
				s_time_limit_activity_branch_temp temp_info;
				parse_char_branch_temp(temp_info, temp_db_info[i]);
				m_temporary_time_limit_activity_branch_temp_map[act_guid] = temp_info;
			}
		}
		if (is_end)
		{
			m_time_limit_activity_temp_map.clear();
			m_time_limit_activity_branch_temp_map.clear();
			m_time_limit_activity_temp_map = m_temporary_time_limit_activity_temp_map;
			m_time_limit_activity_branch_temp_map = m_temporary_time_limit_activity_branch_temp_map;
			unit_man::refresh_all_player_time_limit();
		}
	}


	int32 time_limit_activity_temp_mgr::get_new_limit_act_with_act_type(int32 activity_type)
	{
		int32 return_id = -1;
		int32 cur_max_create_time = 0;
		auto ite = m_time_limit_activity_temp_map.begin();
		for (; ite != m_time_limit_activity_temp_map.end(); ++ite)
		{
			if (ite->second.ActivityType == activity_type)
			{
				int32 cur_create_time = ite->second.CreateTime;
				if (cur_create_time > cur_max_create_time)
				{
					cur_max_create_time = cur_create_time;
					return_id = ite->second.ActGuid;
				}
			}
		}
		return return_id;
	}

	const s_time_limit_activity_temp & time_limit_activity_temp_mgr::get_time_limit_act_with_id(int32 temp_id)
	{
		auto ite = m_time_limit_activity_temp_map.find(temp_id);
		if (ite == m_time_limit_activity_temp_map.end())
		{
			return m_empty_limit_act_temp;
		}
		else
		{
			return m_time_limit_activity_temp_map[temp_id];
		}
	}

	const s_time_limit_activity_branch_temp & time_limit_activity_temp_mgr::get_time_limit_act_branch_with_id(int32 temp_id)
	{
		auto ite = m_time_limit_activity_branch_temp_map.find(temp_id);
		if (ite == m_time_limit_activity_branch_temp_map.end())
		{
			return m_empty_limit_act_branch_temp;
		}
		else
		{
			return m_time_limit_activity_branch_temp_map[temp_id];
		}
	}

	void time_limit_activity_temp_mgr::parse_char_temp(s_time_limit_activity_temp& temp_info, const s_time_limit_activity_temp_db& temp_dp_info)
	{
		temp_info.ActGuid = temp_dp_info.ActGuid;
		temp_info.AttributeId = temp_dp_info.AttributeId;
		temp_info.ActivityType = temp_dp_info.ActivityType;
		temp_info.CycleParamType = temp_dp_info.CycleParamType;
		temp_info.OpenType = temp_dp_info.OpenType;
		temp_info.OpenServerDate = temp_dp_info.OpenServerDate;
		temp_info.FirstOpenDayOfWeek = temp_dp_info.FirstOpenDayOfWeek;
		temp_info.DurationDays = temp_dp_info.DurationDays;
		temp_info.CycleDays = temp_dp_info.CycleDays;
		temp_info.IsEndAct = temp_dp_info.IsEndAct;
		temp_info.CreateTime = temp_dp_info.CreateTime;
		temp_info.EndTime = temp_dp_info.EndTime;
		temp_info.TemplateFromType = temp_dp_info.TemplateFromType;
		temp_info.RefreshTime = temp_dp_info.RefreshTime;
		temp_info.ShowType = temp_dp_info.ShowType;
		init_unit::parse_char_to_vector(temp_info.CycleParamNum, temp_dp_info.CycleParamNum);
		init_unit::parse_char_to_vector(temp_info.CycleFirstId, temp_dp_info.CycleFirstId);
		init_unit::parse_char_to_vector(temp_info.OpenSpencificDate, temp_dp_info.OpenSpencificDate);
		init_unit::parse_char_to_vector(temp_info.TextImage, temp_dp_info.TextImage);
		init_unit::parse_char_to_vector(temp_info.ParamArr1, temp_dp_info.ParamArr1);
		init_unit::parse_char_to_vector(temp_info.ParamArr2, temp_dp_info.ParamArr2);
		init_unit::parse_char_to_vector(temp_info.ParamArr3, temp_dp_info.ParamArr3);
		init_unit::parse_char_to_vector(temp_info.ParamArr4, temp_dp_info.ParamArr4);
		init_unit::parse_char_to_vector(temp_info.ParamArr5, temp_dp_info.ParamArr5);
		init_unit::parse_char_to_vector(temp_info.OriginalCycleFirstId, temp_dp_info.OriginalCycleFirstId);
		init_unit::parse_char_to_vector(temp_info.OldCycleFirstId, temp_dp_info.OldCycleFirstId);
	}

	void time_limit_activity_temp_mgr::parse_char_branch_temp(s_time_limit_activity_branch_temp& temp_info, const s_time_limit_activity_branch_temp_db& temp_dp_info)
	{
		temp_info.ActGuid = temp_dp_info.ActGuid;
		temp_info.AttributeId = temp_dp_info.AttributeId;;
		temp_info.ActivityType = temp_dp_info.ActivityType;
		temp_info.CycleIndex = temp_dp_info.CycleIndex;
		init_unit::parse_char_to_vector(temp_info.Condition, temp_dp_info.Condition);
		init_unit::parse_char_to_vector(temp_info.Reward, temp_dp_info.Reward);
		init_unit::parse_char_to_vector(temp_info.TextImage, temp_dp_info.TextImage);
		init_unit::parse_char_to_vector(temp_info.ParamArr1, temp_dp_info.ParamArr1);
		init_unit::parse_char_to_vector(temp_info.ParamArr2, temp_dp_info.ParamArr2);
		init_unit::parse_char_to_vector(temp_info.ParamArr3, temp_dp_info.ParamArr3);
		init_unit::parse_char_to_vector(temp_info.ParamArr4, temp_dp_info.ParamArr4);
		init_unit::parse_char_to_vector(temp_info.ParamArr5, temp_dp_info.ParamArr5);
	}
}
