/********************************************************************
created: 2020-1-7
file base: time_limit_activity_temp_fep_mgr
file ext: cpp
author: wangdi

purpose:
*********************************************************************/

/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include "time_limit_activity_temp_fep_mgr.h"
#include "utility/init_unit.h"
#include "proxy_service_cli.hpp"
#include "csv_synchronization_fep_mgr.h"

namespace faith
{
	time_limit_activity_temp_fep_mgr::time_limit_activity_temp_fep_mgr()
	{
		clear_data();
	}

	void time_limit_activity_temp_fep_mgr::clear_data()
	{
		m_time_limit_activity_temp_map.clear();
		m_time_limit_activity_branch_temp_map.clear();
	}

	void time_limit_activity_temp_fep_mgr::init_manager()
	{

	}

	void time_limit_activity_temp_fep_mgr::heart_tick(const int64& time_new)
	{
	
	}

	void time_limit_activity_temp_fep_mgr::recv_ws2fep_limit_act_temp(const s_time_limit_activity_temp_db * temp_db_info, int32 data_num, bool is_begin)
	{
		if (data_num > max_send_cs2ws_once_num)
		{
			return;
		}
		if (is_begin)
		{
			m_time_limit_activity_temp_map.clear();
			m_time_limit_activity_branch_temp_map.clear();
		}
		for (int32 i = 0; i < data_num; i++)
		{
			int32 act_guid = temp_db_info[i].ActGuid;
			if (act_guid > 0)
			{
				s_time_limit_activity_temp temp_info;
				parse_char_temp(temp_info, temp_db_info[i]);
				m_time_limit_activity_temp_map[act_guid] = temp_info;
			}
		}
	}

	void time_limit_activity_temp_fep_mgr::recv_ws2fep_limit_act_branch_temp(const s_time_limit_activity_branch_temp_db * temp_db_info, int32 data_num, bool is_end)
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
				m_time_limit_activity_branch_temp_map[act_guid] = temp_info;
			}
		}

		if (is_end)
		{
			csv_synchronization_fep_mgr::get_instance().refresh_csv_template_msg_array();
			proxy_service_cli::getInstance().send_time_limit_activity_template_to_all();
		}
	}

	void time_limit_activity_temp_fep_mgr::parse_char_temp(s_time_limit_activity_temp& temp_info, const s_time_limit_activity_temp_db& temp_dp_info)
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

	void time_limit_activity_temp_fep_mgr::parse_char_branch_temp(s_time_limit_activity_branch_temp& temp_info, const s_time_limit_activity_branch_temp_db& temp_dp_info)
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

	void time_limit_activity_temp_fep_mgr::fill_time_limit_activity_template_one_msg(const s_time_limit_activity_temp * template_line, game_proto_template_table_data * activity_template_data)
	{
		if (template_line == nullptr || activity_template_data == nullptr)
		{
			return;
		}
		activity_template_data->set_attribute_id(template_line->ActGuid);
		game_proto_template_table_param_int*  activity_template_param_int = NULL;
		game_proto_template_table_param_string*  activity_template_param_string = NULL;
		int32	time_limit_activity_data_int;
		string	time_limit_activity_data_string;

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->ActGuid;
		activity_template_param_int->set_param_name("attribute_id");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->ActivityType;
		activity_template_param_int->set_param_name("ActivityType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->CycleParamType;
		activity_template_param_int->set_param_name("CycleParamType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->CycleParamNum.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->CycleParamNum[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->CycleParamNum[i]);
			}
		}
		activity_template_param_string->set_param_name("CycleParamNum");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->CycleFirstId.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->CycleFirstId[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->CycleFirstId[i]);
			}
		}
		activity_template_param_string->set_param_name("CycleFirstId");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->OpenType;
		activity_template_param_int->set_param_name("OpenType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->OpenServerDate;
		activity_template_param_int->set_param_name("OpenServerDate");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->OpenSpencificDate.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->OpenSpencificDate[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->OpenSpencificDate[i]);
			}
		}
		activity_template_param_string->set_param_name("OpenSpencificDate");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->FirstOpenDayOfWeek;
		activity_template_param_int->set_param_name("FirstOpenDayOfWeek");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->DurationDays;
		activity_template_param_int->set_param_name("DurationDays");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->CycleDays;
		activity_template_param_int->set_param_name("CycleDays");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->ShowType;
		activity_template_param_int->set_param_name("ShowType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->TextImage.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->TextImage[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->TextImage[i]);
			}
		}
		activity_template_param_string->set_param_name("TextImage");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr1.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr1[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr1[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr1");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr2.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr2[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr2[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr2");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr3.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr3[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr3[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr3");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr4.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr4[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr4[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr4");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr5.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr5[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr5[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr5");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);
	}

	void time_limit_activity_temp_fep_mgr::fill_time_limit_activity_branch_template_one_msg(const s_time_limit_activity_branch_temp* template_line, game_proto_template_table_data * activity_template_data)
	{
		if (nullptr == template_line || nullptr == activity_template_data)
		{
			return;
		}
		activity_template_data->set_attribute_id(template_line->ActGuid);
		game_proto_template_table_param_int*  activity_template_param_int = NULL;
		game_proto_template_table_param_string*  activity_template_param_string = NULL;
		int32	time_limit_activity_data_int;
		string	time_limit_activity_data_string;

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->ActGuid;
		activity_template_param_int->set_param_name("attribute_id");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->ActivityType;
		activity_template_param_int->set_param_name("ActivityType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->CycleIndex;
		activity_template_param_int->set_param_name("CycleIndex");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->Condition.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->Condition[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->Condition[i]);
			}
		}
		activity_template_param_string->set_param_name("Condition");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->Reward.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->Reward[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->Reward[i]);
			}
		}
		activity_template_param_string->set_param_name("Reward");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->TextImage.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->TextImage[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->TextImage[i]);
			}
		}
		activity_template_param_string->set_param_name("TextImage");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr1.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr1[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr1[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr1");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr2.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr2[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr2[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr2");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr3.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr3[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr3[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr3");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr4.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr4[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr4[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr4");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ParamArr5.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = to_string(template_line->ParamArr5[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += to_string(template_line->ParamArr5[i]);
			}
		}
		activity_template_param_string->set_param_name("ParamArr5");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);
	}

}
