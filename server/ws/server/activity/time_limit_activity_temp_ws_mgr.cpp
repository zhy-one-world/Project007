/********************************************************************
  created: 2019-7-22
  file base: time_limit_activity_temp_ws_mgr
  file ext: CPP
  author: zhangshuo
  
  purpose: 
*********************************************************************/

/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include "time_limit_activity_temp_ws_mgr.h"
#include "time_limit_activity_ws_mgr.h"
#include <time_limit_activity_msg.hpp> 
#include "utility/init_unit.h"
#include "Logic/time_def.hpp"
#include "ws/world_server.hpp"
#include "cross_group/gate_proxy.hpp"
#include "ws_client.hpp"
#include "cloud_shop_mgr.h"
#include "globle_data.h"

namespace faith
{
	time_limit_activity_temp_ws_mgr::time_limit_activity_temp_ws_mgr()
	{
		clear_data();
	}

	void time_limit_activity_temp_ws_mgr::clear_data()
	{
		m_time_limit_activity_temp_map.clear();
		m_time_limit_activity_branch_temp_map.clear();
		m_empty_limit_act_temp.clear_data();
		m_empty_limit_act_branch_temp.clear_data();
	}

	void time_limit_activity_temp_ws_mgr::init_manager()
	{
		send_to_dp_load_temp();
	}

	void time_limit_activity_temp_ws_mgr::heart_tick(const int64& time_new)
	{

	}

	void time_limit_activity_temp_ws_mgr::send_to_cs_template()
	{
		int32 data_num = 0;
		ws2cs_act_limit_temp msg;

		faith::ws2cs_proto::act_limit_temp pro_msg;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_time_limit_temp);

		bool is_begin = true;
		for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end();ite++)
		{
			const s_time_limit_activity_temp& temp_info = ite->second;
			package_char_temp(msg.temp_db_info[data_num], temp_info);
			data_num++;
			if (data_num >= max_send_cs2ws_once_num)
			{
				msg.data_num = data_num;
				msg.is_begin = is_begin;
				if (is_use == false)
				{
					world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
				}
				else
				{
					pro_msg.Clear();
					msg.to_proto(pro_msg);
					world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_time_limit_temp, e_server_type_cs);
				}
				msg.clear_data();
				data_num = 0;
				is_begin = false;
			}
		}
		msg.data_num = data_num;
		msg.is_begin = is_begin;

	
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
		}
		else
		{
			pro_msg.Clear();
			msg.to_proto(pro_msg);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_time_limit_temp, e_server_type_cs);
		}

		//发分支表
		int32 branch_data_num = 0;
		ws2cs_act_limit_branch_temp branch_msg;

		faith::ws2cs_proto::act_limit_branch_temp pro_branch_msg;
		is_use = proto_by_lua(e_msgindex_ws2cs_time_limit_branch_temp);

		for (auto ite = m_time_limit_activity_branch_temp_map.begin(); ite != m_time_limit_activity_branch_temp_map.end();ite++)
		{
			const s_time_limit_activity_branch_temp& temp_info = ite->second;
			branch_msg.is_end = false;
			package_char_branch_temp(branch_msg.temp_db_info[branch_data_num], temp_info);
			branch_data_num++;
			if (branch_data_num >= max_send_cs2ws_once_num)
			{
				branch_msg.data_num = branch_data_num;
				if (is_use == false)
				{
					world_server::getInstance().broadcast(&branch_msg, sizeof(branch_msg), e_server_type_cs);
				}
				else
				{
					pro_branch_msg.Clear();
					branch_msg.to_proto(pro_branch_msg);
					world_server::getInstance().broadcast_lua(&pro_branch_msg, e_msgindex_ws2cs_time_limit_branch_temp, e_server_type_cs);
				}
				branch_msg.clear_data();
				branch_data_num = 0;
			}
		}
		branch_msg.is_end = true;
		branch_msg.data_num = branch_data_num;

		if (is_use == false)
		{
			world_server::getInstance().broadcast(&branch_msg, sizeof(branch_msg), e_server_type_cs);
		}
		else
		{
			pro_branch_msg.Clear();
			branch_msg.to_proto(pro_branch_msg);
			world_server::getInstance().broadcast_lua(&pro_branch_msg, e_msgindex_ws2cs_time_limit_branch_temp, e_server_type_cs);
		}
	}

	void time_limit_activity_temp_ws_mgr::send_to_fep_template()
	{
		int32 data_num = 0;
		ws2fep_act_limit_temp msg;
		bool is_begin = true;
		for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end(); ite++)
		{
			const s_time_limit_activity_temp& temp_info = ite->second;
			package_char_temp(msg.temp_db_info[data_num], temp_info);
			data_num++;
			if (data_num >= max_send_cs2ws_once_num)
			{
				msg.data_num = data_num;
				msg.is_begin = is_begin;
				world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_fep);
				msg.clear_data();
				data_num = 0;
				is_begin = false;
			}
		}
		msg.data_num = data_num;
		msg.is_begin = is_begin;
		world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_fep);

		//发分支表
		int32 branch_data_num = 0;
		ws2fep_act_limit_branch_temp branch_msg;
		for (auto ite = m_time_limit_activity_branch_temp_map.begin(); ite != m_time_limit_activity_branch_temp_map.end(); ite++)
		{
			const s_time_limit_activity_branch_temp& temp_info = ite->second;
			branch_msg.is_end = false;
			package_char_branch_temp(branch_msg.temp_db_info[branch_data_num], temp_info);
			branch_data_num++;
			if (branch_data_num >= max_send_cs2ws_once_num)
			{
				branch_msg.data_num = branch_data_num;
				world_server::getInstance().broadcast(&branch_msg, sizeof(branch_msg), e_server_type_fep);
				branch_msg.clear_data();
				branch_data_num = 0;
			}
		}
		branch_msg.is_end = true;
		branch_msg.data_num = branch_data_num;
		world_server::getInstance().broadcast(&branch_msg, sizeof(branch_msg), e_server_type_fep);
	}

	void time_limit_activity_temp_ws_mgr::save_temp_to_db(const s_time_limit_activity_temp& temp_info)
	{
		ws2dp_save_time_limit_temp req;
		package_char_temp(req.temp_db_info, temp_info);
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}

	void time_limit_activity_temp_ws_mgr::delete_temp(int32 act_guid)
	{
		if (m_time_limit_activity_temp_map.find(act_guid) == m_time_limit_activity_temp_map.end())
		{
			return;
		}
		delete_cycle_first_branch_temp_arr(m_time_limit_activity_temp_map[act_guid].CycleFirstId, m_time_limit_activity_temp_map[act_guid].ActivityType);
		delete_cycle_first_branch_temp_arr(m_time_limit_activity_temp_map[act_guid].OldCycleFirstId, m_time_limit_activity_temp_map[act_guid].ActivityType);
		m_time_limit_activity_temp_map.erase(act_guid);
		ws2dp_delete_time_limit_temp msg;
		msg.act_guid = act_guid;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void time_limit_activity_temp_ws_mgr::delete_cycle_first_branch_temp_arr(std::vector<int32> cycle_first_branch_temp_arr, int32 activity_type)
	{
		for (int32 i = 0; i < cycle_first_branch_temp_arr.size(); i++)
		{
			std::vector<int32> cycle_first_branch_temp_id_arr;
			if (i + 1 < cycle_first_branch_temp_arr.size())
			{
				cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(cycle_first_branch_temp_arr[i], cycle_first_branch_temp_arr[i + 1], activity_type);

			}
			else
			{
				cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(cycle_first_branch_temp_arr[i], 0, activity_type);
			}

			for (int32 j = 0; j < cycle_first_branch_temp_id_arr.size(); j++)
			{
				delete_branch_temp(cycle_first_branch_temp_id_arr[j]);
			}
		}
	}

	void time_limit_activity_temp_ws_mgr::save_branch_temp_to_db(const s_time_limit_activity_branch_temp& temp_info)
	{
		ws2dp_save_time_limit_branch_temp req;
		package_char_branch_temp(req.temp_db_info, temp_info);
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}

	void time_limit_activity_temp_ws_mgr::delete_branch_temp(int32 act_guid)
	{
		if (m_time_limit_activity_branch_temp_map.find(act_guid) == m_time_limit_activity_branch_temp_map.end())
		{
			return;
		}
		m_time_limit_activity_branch_temp_map.erase(act_guid);
		ws2dp_delete_time_limit_branch_temp msg;
		msg.act_guid = act_guid;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void time_limit_activity_temp_ws_mgr::end_act_with_act_type(int32 act_type, int32 is_need_send_to_client)
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
				save_temp_to_db(ite->second);
			}
		}
		send_to_cs_end_act_type(act_type, is_need_send_to_client);
	}

	void time_limit_activity_temp_ws_mgr::send_to_cs_end_act_type(int32 act_type, int32 is_need_send_to_client)
	{
		ws2cs_end_act_type msg;
		msg.act_type = act_type;
		msg.is_need_send_to_client = is_need_send_to_client;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_end_act_type);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::end_act_type pro_msg;
			msg.to_proto(pro_msg);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_end_act_type, e_server_type_cs);
		}
	}

	const s_time_limit_activity_temp & time_limit_activity_temp_ws_mgr::get_time_limit_act_with_id(int32 temp_id)
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

	const s_time_limit_activity_temp& time_limit_activity_temp_ws_mgr::get_new_limit_act_with_type(int32 activity_type)
	{
		std::vector<int32> activity_id_arr = get_activity_id_arr_by_type(activity_type);
		if (activity_id_arr.size() <= 0)
		{
			return m_empty_limit_act_temp;
		}
		else
		{
			return get_time_limit_act_with_id(activity_id_arr[0]);
		}
	}

	const s_time_limit_activity_branch_temp & time_limit_activity_temp_ws_mgr::get_time_limit_act_branch_with_id(int32 temp_id)
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

	void time_limit_activity_temp_ws_mgr::send_to_dp_load_temp()
	{
		ws2dp_load_time_limit_temp req;
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}

	void time_limit_activity_temp_ws_mgr::parse_char_temp(s_time_limit_activity_temp& temp_info, const s_time_limit_activity_temp_db& temp_dp_info)
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

	void time_limit_activity_temp_ws_mgr::parse_char_branch_temp(s_time_limit_activity_branch_temp& temp_info, const s_time_limit_activity_branch_temp_db& temp_dp_info)
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

	void time_limit_activity_temp_ws_mgr::package_char_temp(s_time_limit_activity_temp_db & temp_dp_info, const s_time_limit_activity_temp & temp_info)
	{
		temp_dp_info.ActGuid = temp_info.ActGuid;
		temp_dp_info.AttributeId = temp_info.AttributeId;
		temp_dp_info.ActivityType = temp_info.ActivityType;
		temp_dp_info.CycleParamType = temp_info.CycleParamType;
		temp_dp_info.OpenType = temp_info.OpenType;
		temp_dp_info.OpenServerDate = temp_info.OpenServerDate;
		temp_dp_info.FirstOpenDayOfWeek = temp_info.FirstOpenDayOfWeek;
		temp_dp_info.DurationDays = temp_info.DurationDays;
		temp_dp_info.CycleDays = temp_info.CycleDays;
		temp_dp_info.IsEndAct = temp_info.IsEndAct;
		temp_dp_info.CreateTime = temp_info.CreateTime;
		temp_dp_info.EndTime = temp_info.EndTime;
		temp_dp_info.TemplateFromType = temp_info.TemplateFromType;
		temp_dp_info.RefreshTime = temp_info.RefreshTime;
		temp_dp_info.ShowType = temp_info.ShowType;
		xstring mid_str = "";
		init_unit::parse_vector_to_char(mid_str, temp_info.CycleParamNum);
		temp_dp_info.set_CycleParamNum(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.CycleFirstId);
		temp_dp_info.set_CycleFirstId(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.OpenSpencificDate);
		temp_dp_info.set_OpenSpencificDate(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.TextImage);
		temp_dp_info.set_TextImage(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr1);
		temp_dp_info.set_ParamArr1(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr2);
		temp_dp_info.set_ParamArr2(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr3);
		temp_dp_info.set_ParamArr3(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr4);
		temp_dp_info.set_ParamArr4(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr5);
		temp_dp_info.set_ParamArr5(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.OriginalCycleFirstId);
		temp_dp_info.set_OriginalCycleFirstId(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.OldCycleFirstId);
		temp_dp_info.set_OldCycleFirstId(mid_str);
	}

	void time_limit_activity_temp_ws_mgr::package_char_branch_temp(s_time_limit_activity_branch_temp_db & temp_dp_info, const s_time_limit_activity_branch_temp & temp_info)
	{
		temp_dp_info.ActGuid = temp_info.ActGuid;
		temp_dp_info.AttributeId = temp_info.AttributeId;
		temp_dp_info.ActivityType = temp_info.ActivityType;
		temp_dp_info.CycleIndex = temp_info.CycleIndex;
		xstring mid_str = "";
		init_unit::parse_vector_to_char(mid_str, temp_info.Condition);
		temp_dp_info.set_Condition(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.Reward);
		temp_dp_info.set_Reward(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.TextImage);
		temp_dp_info.set_TextImage(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr1);
		temp_dp_info.set_ParamArr1(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr2);
		temp_dp_info.set_ParamArr2(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr3);
		temp_dp_info.set_ParamArr3(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr4);
		temp_dp_info.set_ParamArr4(mid_str);
		init_unit::parse_vector_to_char(mid_str, temp_info.ParamArr5);
		temp_dp_info.set_ParamArr5(mid_str);
	}

	void time_limit_activity_temp_ws_mgr::load_temp_end(const s_time_limit_activity_temp_db* temp_dp_info, bool is_load_all, int32 data_num)
	{
		if (nullptr == temp_dp_info)
		{
			return;
		}
		if (data_num > max_load_time_limit_temp_num || data_num < 0)
		{
			return;
		}
		for (int32 i = 0; i < data_num; i++)
		{
			int32 act_guid = temp_dp_info[i].ActGuid;
			if (act_guid > 0)
			{
				s_time_limit_activity_temp temp_info;
				parse_char_temp(temp_info, temp_dp_info[i]);
				m_time_limit_activity_temp_map[act_guid] = temp_info;
			}
		}
	}

	void time_limit_activity_temp_ws_mgr::load_branch_temp_end(const s_time_limit_activity_branch_temp_db* temp_dp_info, bool is_load_all, int32 data_num)
	{
		if (nullptr == temp_dp_info)
		{
			return;
		}
		if (data_num > max_load_time_limit_temp_num || data_num < 0)
		{
			return;
		}
		for (int32 i = 0; i < data_num; i++)
		{
			int32 act_guid = temp_dp_info[i].ActGuid;
			if (act_guid > 0)
			{
				s_time_limit_activity_branch_temp temp_info;
				parse_char_branch_temp(temp_info, temp_dp_info[i]);
				m_time_limit_activity_branch_temp_map[act_guid] = temp_info;
			}
		}
		if (is_load_all)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_time_limit_template);
			if (0 == data_num)
			{
				set_is_load_empty_from_db(true);
			}
		}
	}
	void time_limit_activity_temp_ws_mgr::update_role_time_limit_act_guid(int32 act_type, int32 act_guid)
	{
		ws2dp_update_role_time_limit req;
		req.act_guid = act_guid;
		req.act_type = act_type;
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}

	void time_limit_activity_temp_ws_mgr::load_new_temp(bool is_empty, bool is_need_refresh_guid)
	{
		load_limit_act_temp(is_empty, is_need_refresh_guid);
		delete_temp_info_when_time_over();
		check_and_clear_branch_template_no_use();
		syn_template_info_to_cs_and_client_and_fep();
		time_limit_activity_ws_mgr::get_instance().init_all_time_limit_ws();
		if (!world_server::getInstance().is_loading_flag_finish(e_ws_flag_init_time_limit_template))
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_init_time_limit_template);
		}
	}

	int32 time_limit_activity_temp_ws_mgr::compare_limit_act_branch_temp(const TimeLimitActivityBranchTemplate* limit_act_temp, int32 act_temp_id)
	{
		if (limit_act_temp == nullptr || act_temp_id <= 0 || m_time_limit_activity_branch_temp_map.find(act_temp_id) == m_time_limit_activity_branch_temp_map.end())
		{
			return e_compare_temp_result_error;
		}
		const s_time_limit_activity_branch_temp& act_temp_info = m_time_limit_activity_branch_temp_map[act_temp_id];
		bool is_change = false;
		if (limit_act_temp->Condition != act_temp_info.Condition ||
			limit_act_temp->Reward != act_temp_info.Reward ||	
			limit_act_temp->TextImage != act_temp_info.TextImage ||
			limit_act_temp->ParamArr1 != act_temp_info.ParamArr1 ||
			limit_act_temp->ParamArr2 != act_temp_info.ParamArr2 ||
			limit_act_temp->ParamArr3 != act_temp_info.ParamArr3 ||
			limit_act_temp->ParamArr5 != act_temp_info.ParamArr5)
		{
			return e_compare_temp_result_change;
		}
		else
		{
			return e_compare_temp_result_none;
		}
	}

	std::vector<int32> time_limit_activity_temp_ws_mgr::get_first_cycle_branch_temp_arr_by_template(int32 first_branch_temp_id, int32 next_first_branch_temp_id, int32 activity_type)
	{
		std::vector<int32> cycle_first_branch_temp_id_arr;
		//最多循环查找100次 防止无限循环
		for (int32 i = 0; i < 100; i++)
		{
			int32 limit_act_branch_temp_id = first_branch_temp_id + i;
			TimeLimitActivityBranchTemplate* limit_act_branch_temp = template_manager::get_instance().get_time_limit_activity_branch_template_ptr(limit_act_branch_temp_id);
			if (limit_act_branch_temp == nullptr || limit_act_branch_temp->ActivityType != activity_type)
			{
				break;
			}
			if (next_first_branch_temp_id > 0 && limit_act_branch_temp_id >= next_first_branch_temp_id)
			{
				break;
			}
			cycle_first_branch_temp_id_arr.push_back(limit_act_branch_temp_id);
		}
		return cycle_first_branch_temp_id_arr;
	}

	std::vector<int32> time_limit_activity_temp_ws_mgr::get_first_cycle_branch_struct_arr_by_template(int32 first_branch_temp_id, int32 next_first_branch_temp_id, int32 activity_type)
	{
		std::vector<int32> cycle_first_branch_temp_id_arr;
		//最多循环查找100次 防止无限循环
		for (int32 i = 0; i < 100; i++)
		{
			int32 limit_act_branch_temp_id = first_branch_temp_id + i;
			if (m_time_limit_activity_branch_temp_map.find(limit_act_branch_temp_id) == m_time_limit_activity_branch_temp_map.end()
				|| m_time_limit_activity_branch_temp_map[limit_act_branch_temp_id].ActivityType != activity_type)
			{
				break;
			}
			if (next_first_branch_temp_id > 0 && limit_act_branch_temp_id >= next_first_branch_temp_id)
			{
				break;
			}
			cycle_first_branch_temp_id_arr.push_back(limit_act_branch_temp_id);
		}
		return cycle_first_branch_temp_id_arr;
	}

	int32 time_limit_activity_temp_ws_mgr::compare_limit_act_temp(const TimeLimitActivityTemplate * limit_act_temp, int32 act_temp_id, bool is_need_refresh_guid)
	{
		if (limit_act_temp == nullptr)
		{
			return e_compare_temp_result_error;
		}
		if (act_temp_id <= 0 || m_time_limit_activity_temp_map.find(act_temp_id) == m_time_limit_activity_temp_map.end())
		{
			return e_compare_temp_result_new;
		}
		int32 new_base_open_time = init_unit::get_cur_time_limit_activity_base_time_by_temp(exchange_to_struct_by_template_ptr(limit_act_temp), world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time)
			, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time));
		int32 new_duration_days = limit_act_temp->DurationDays;
		int32 new_cycle_days = limit_act_temp->CycleDays;

		const s_time_limit_activity_temp & act_temp_info = m_time_limit_activity_temp_map[act_temp_id];
		int32 old_base_open_time = init_unit::get_cur_time_limit_activity_base_time_by_temp(act_temp_info, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time)
			, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time));
		int32 old_duration_days = act_temp_info.DurationDays;
		int32 old_cycle_days = act_temp_info.CycleDays;

		int32 change_open_time = (new_base_open_time - old_base_open_time) > 0 ? (new_base_open_time - old_base_open_time) : (old_base_open_time - new_base_open_time);

		bool is_change = false;
		//paramArr4仅用于生成id 不用做对比
		if (limit_act_temp->CycleParamNum != act_temp_info.CycleParamNum ||
			limit_act_temp->CycleFirstId != act_temp_info.OriginalCycleFirstId ||	//firstid需要做特殊比对处理
			limit_act_temp->TextImage != act_temp_info.TextImage ||
			limit_act_temp->ShowType != act_temp_info.ShowType ||
			limit_act_temp->ParamArr1 != act_temp_info.ParamArr1 ||
			limit_act_temp->ParamArr2 != act_temp_info.ParamArr2 ||
			limit_act_temp->ParamArr3 != act_temp_info.ParamArr3 ||
			limit_act_temp->ParamArr4 != act_temp_info.ParamArr4 ||
			limit_act_temp->ParamArr5 != act_temp_info.ParamArr5)
		{
			is_change = true;
		}

		//持续时间 循环天数 开始时间错位
		if (new_duration_days != old_duration_days || new_cycle_days != old_cycle_days
			|| (change_open_time > 0 && change_open_time / day_time_second != new_cycle_days))
		{
			return e_compare_temp_result_new;
		}
		//起服时才会刷新guid 热更不刷新
		else if (is_need_refresh_guid && time_helper::get_cur_time_new().second > act_temp_info.RefreshTime && !init_unit::is_cross_time_limit_cross_activity(act_temp_info.ActivityType))
		{
			return e_compare_temp_result_refresh_guid;
		}
		else if (( change_open_time > 0 && change_open_time / day_time_second == new_cycle_days) || is_change)
		{
			return e_compare_temp_result_change;
		}
		else
		{
			return e_compare_temp_result_none;
		}
	}

	int32 time_limit_activity_temp_ws_mgr::compare_cycle_first_branch_temp_arr(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id)
	{
		if (nullptr == limit_act_temp || act_temp_id <= 0 || m_time_limit_activity_temp_map.find(act_temp_id) == m_time_limit_activity_temp_map.end()
			|| m_time_limit_activity_temp_map[act_temp_id].OriginalCycleFirstId.size() != m_time_limit_activity_temp_map[act_temp_id].CycleFirstId.size())
		{
			return e_compare_temp_result_error;
		}
		if (m_time_limit_activity_temp_map[act_temp_id].OriginalCycleFirstId != limit_act_temp->CycleFirstId)
		{
			return e_compare_temp_result_new;
		}
		for (int32 i = 0; i < limit_act_temp->CycleFirstId.size(); i++)
		{
			std::vector<int32> old_cycle_first_branch_temp_id_arr;
			std::vector<int32> new_cycle_first_branch_temp_id_arr;
			if (i + 1 < limit_act_temp->CycleFirstId.size())
			{
				old_cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(m_time_limit_activity_temp_map[act_temp_id].CycleFirstId[i], m_time_limit_activity_temp_map[act_temp_id].CycleFirstId[i + 1], limit_act_temp->ActivityType);
				new_cycle_first_branch_temp_id_arr = get_first_cycle_branch_temp_arr_by_template(limit_act_temp->CycleFirstId[i], limit_act_temp->CycleFirstId[i + 1], limit_act_temp->ActivityType);
			}
			else
			{
				old_cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(m_time_limit_activity_temp_map[act_temp_id].CycleFirstId[i], 0, limit_act_temp->ActivityType);
				new_cycle_first_branch_temp_id_arr = get_first_cycle_branch_temp_arr_by_template(limit_act_temp->CycleFirstId[i], 0, limit_act_temp->ActivityType);
			}
		
			if (old_cycle_first_branch_temp_id_arr.size() != new_cycle_first_branch_temp_id_arr.size())
			{
				return e_compare_temp_result_new;
			}


			for (int32 j = 0; j < old_cycle_first_branch_temp_id_arr.size(); j++)
			{
				TimeLimitActivityBranchTemplate* limit_act_branch_temp = template_manager::get_instance().get_time_limit_activity_branch_template_ptr(new_cycle_first_branch_temp_id_arr[j]);
				int32 compare_result = compare_limit_act_branch_temp(limit_act_branch_temp, old_cycle_first_branch_temp_id_arr[j]);
				if (compare_result == e_compare_temp_result_change)
				{
					return e_compare_temp_result_change;
				}
				else if (compare_result == e_compare_temp_result_error)
				{
					return e_compare_temp_result_error;
				}
			}
		}
		return e_compare_temp_result_none;
	}

	int32 time_limit_activity_temp_ws_mgr::make_new_limit_act_temp_with_temp(const TimeLimitActivityTemplate* limit_act_temp, int32 mark_time, bool is_need_update, int32 template_from_type)
	{
		if (nullptr == limit_act_temp)
		{
			return 0;
		}
		int32 temp_server_id = 0;
		if (!init_unit::is_cross_time_limit_cross_activity(limit_act_temp->ActivityType))
		{
			temp_server_id = world_server::getInstance().get_server_id() % 1000;
		}
		int32 act_guid = mark_time * time_limit_day_mark_num + temp_server_id * time_limit_server_mark_num + limit_act_temp->ActivityType;
		m_time_limit_activity_temp_map[act_guid].clear_data();
		m_time_limit_activity_temp_map[act_guid].ActGuid = act_guid;
		m_time_limit_activity_temp_map[act_guid].AttributeId = limit_act_temp->attribute_id;
		m_time_limit_activity_temp_map[act_guid].ActivityType = limit_act_temp->ActivityType;
		m_time_limit_activity_temp_map[act_guid].CycleParamType = limit_act_temp->CycleParamType;
		m_time_limit_activity_temp_map[act_guid].OpenType = limit_act_temp->OpenType;
		m_time_limit_activity_temp_map[act_guid].OpenServerDate = limit_act_temp->OpenServerDate;
		m_time_limit_activity_temp_map[act_guid].FirstOpenDayOfWeek = limit_act_temp->FirstOpenDayOfWeek;
		m_time_limit_activity_temp_map[act_guid].DurationDays = limit_act_temp->DurationDays;
		m_time_limit_activity_temp_map[act_guid].CycleDays = limit_act_temp->CycleDays;
		m_time_limit_activity_temp_map[act_guid].CycleParamNum = limit_act_temp->CycleParamNum;
		m_time_limit_activity_temp_map[act_guid].OpenSpencificDate = limit_act_temp->OpenSpencificDate;
		m_time_limit_activity_temp_map[act_guid].ShowType = limit_act_temp->ShowType;
		m_time_limit_activity_temp_map[act_guid].TextImage = limit_act_temp->TextImage;
		m_time_limit_activity_temp_map[act_guid].ParamArr1 = limit_act_temp->ParamArr1;
		m_time_limit_activity_temp_map[act_guid].ParamArr2 = limit_act_temp->ParamArr2;
		m_time_limit_activity_temp_map[act_guid].ParamArr3 = limit_act_temp->ParamArr3;
		m_time_limit_activity_temp_map[act_guid].ParamArr4 = limit_act_temp->ParamArr4;
		m_time_limit_activity_temp_map[act_guid].ParamArr5 = limit_act_temp->ParamArr5;
		m_time_limit_activity_temp_map[act_guid].CreateTime = time_helper::get_cur_time_new().second;
		m_time_limit_activity_temp_map[act_guid].EndTime = 0;
		m_time_limit_activity_temp_map[act_guid].TemplateFromType = template_from_type;
		m_time_limit_activity_temp_map[act_guid].RefreshTime = time_helper::get_cur_time_new().second + 30 * day_time_second;
		m_time_limit_activity_temp_map[act_guid].CycleFirstId = make_new_limit_act_branch_temp_with_temp_arr(limit_act_temp->CycleFirstId
			, m_time_limit_activity_temp_map[act_guid].OriginalCycleFirstId, mark_time, limit_act_temp->ActivityType);

		save_temp_to_db(m_time_limit_activity_temp_map[act_guid]);
		if (is_need_update)
		{
			update_role_time_limit_act_guid(limit_act_temp->ActivityType, act_guid);
		}
		return act_guid;
	}

	void time_limit_activity_temp_ws_mgr::change_limit_act_temp(const TimeLimitActivityTemplate * limit_act_temp, int32 act_temp_id, int32 mark_time, int32 template_from_type)
	{
		if (nullptr == limit_act_temp || act_temp_id <= 0 || m_time_limit_activity_temp_map.find(act_temp_id) == m_time_limit_activity_temp_map.end())
		{
			return;
		}
		m_time_limit_activity_temp_map[act_temp_id].CycleParamType = limit_act_temp->CycleParamType;
		m_time_limit_activity_temp_map[act_temp_id].OpenType = limit_act_temp->OpenType;
		m_time_limit_activity_temp_map[act_temp_id].OpenServerDate = limit_act_temp->OpenServerDate;
		m_time_limit_activity_temp_map[act_temp_id].FirstOpenDayOfWeek = limit_act_temp->FirstOpenDayOfWeek;
		m_time_limit_activity_temp_map[act_temp_id].DurationDays = limit_act_temp->DurationDays;
		m_time_limit_activity_temp_map[act_temp_id].CycleDays = limit_act_temp->CycleDays;
		m_time_limit_activity_temp_map[act_temp_id].CycleParamNum = limit_act_temp->CycleParamNum;
		m_time_limit_activity_temp_map[act_temp_id].OpenSpencificDate = limit_act_temp->OpenSpencificDate;
		m_time_limit_activity_temp_map[act_temp_id].ShowType = limit_act_temp->ShowType;
		m_time_limit_activity_temp_map[act_temp_id].TextImage = limit_act_temp->TextImage;
		m_time_limit_activity_temp_map[act_temp_id].ParamArr1 = limit_act_temp->ParamArr1;
		m_time_limit_activity_temp_map[act_temp_id].ParamArr2 = limit_act_temp->ParamArr2;
		m_time_limit_activity_temp_map[act_temp_id].ParamArr3 = limit_act_temp->ParamArr3;
		m_time_limit_activity_temp_map[act_temp_id].ParamArr4 = limit_act_temp->ParamArr4;
		m_time_limit_activity_temp_map[act_temp_id].ParamArr5 = limit_act_temp->ParamArr5;
		m_time_limit_activity_temp_map[act_temp_id].CreateTime = time_helper::get_cur_time_new().second;
		m_time_limit_activity_temp_map[act_temp_id].TemplateFromType = template_from_type;
		
		check_and_update_branch_temp_info(limit_act_temp, act_temp_id, mark_time, template_from_type);

		save_temp_to_db(m_time_limit_activity_temp_map[act_temp_id]);
	}

	std::vector<int32> time_limit_activity_temp_ws_mgr::make_new_limit_act_branch_temp_with_temp_arr(const std::vector<int32>& cycle_first_id_arr, std::vector<int32>& original_cycle_first_id_arr, int32 mark_time, int32 activity_type)
	{
		std::vector<int32> temp_cycle_first_id_arr;
		original_cycle_first_id_arr.clear();
		int32 temp_server_id = 0;
		if (!init_unit::is_cross_time_limit_cross_activity(activity_type))
		{
			temp_server_id = world_server::getInstance().get_server_id() % 1000;
		}
		for (int32 i = 0; i < cycle_first_id_arr.size(); i++)
		{
			TimeLimitActivityBranchTemplate* limit_act_first_branch_temp = template_manager::get_instance().get_time_limit_activity_branch_template_ptr(cycle_first_id_arr[i]);
			if (limit_act_first_branch_temp == nullptr || limit_act_first_branch_temp->ParamArr4.size() <= 0)
			{
				break;
			}
			//最多循环查找100次 防止无限循环
			for (int32 j = 0; j < 100; j++)
			{
				int32 limit_act_branch_temp_id = cycle_first_id_arr[i] + j;
				TimeLimitActivityBranchTemplate* limit_act_branch_temp = template_manager::get_instance().get_time_limit_activity_branch_template_ptr(limit_act_branch_temp_id);
				if (limit_act_branch_temp == nullptr || limit_act_branch_temp->ParamArr4.size() <= 0 || limit_act_branch_temp->ActivityType != activity_type)
				{
					break;
				}
				if (i + 1 < cycle_first_id_arr.size() && limit_act_branch_temp_id >= cycle_first_id_arr[i + 1])
				{
					break;
				}
				make_new_limit_act_branch_temp_with_temp(limit_act_branch_temp, mark_time);
			}
			temp_cycle_first_id_arr.push_back(mark_time * time_limit_day_mark_num + temp_server_id * time_limit_server_mark_num + limit_act_first_branch_temp->ParamArr4[0]);
			original_cycle_first_id_arr.push_back(cycle_first_id_arr[i]);
		}
		return temp_cycle_first_id_arr;
	}

	void time_limit_activity_temp_ws_mgr::change_limit_act_branch_temp(const TimeLimitActivityBranchTemplate* limit_act_temp, int32 act_temp_id)
	{
		if (nullptr == limit_act_temp || act_temp_id <= 0 || m_time_limit_activity_branch_temp_map.find(act_temp_id) == m_time_limit_activity_branch_temp_map.end())
		{
			return;
		}
		m_time_limit_activity_branch_temp_map[act_temp_id].CycleIndex = limit_act_temp->CycleIndex;
		m_time_limit_activity_branch_temp_map[act_temp_id].Condition = limit_act_temp->Condition;
		m_time_limit_activity_branch_temp_map[act_temp_id].Reward = limit_act_temp->Reward;
		m_time_limit_activity_branch_temp_map[act_temp_id].TextImage = limit_act_temp->TextImage;
		m_time_limit_activity_branch_temp_map[act_temp_id].ParamArr1 = limit_act_temp->ParamArr1;
		m_time_limit_activity_branch_temp_map[act_temp_id].ParamArr2 = limit_act_temp->ParamArr2;
		m_time_limit_activity_branch_temp_map[act_temp_id].ParamArr3 = limit_act_temp->ParamArr3;
		m_time_limit_activity_branch_temp_map[act_temp_id].ParamArr5 = limit_act_temp->ParamArr5;
		save_branch_temp_to_db(m_time_limit_activity_branch_temp_map[act_temp_id]);
	}

	void time_limit_activity_temp_ws_mgr::change_limit_act_branch_temp_all(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id)
	{
		if (nullptr == limit_act_temp || act_temp_id <= 0 || m_time_limit_activity_temp_map.find(act_temp_id) == m_time_limit_activity_temp_map.end()
			|| limit_act_temp->CycleFirstId.size() != m_time_limit_activity_temp_map[act_temp_id].CycleFirstId.size())
		{
			return;
		}
		for (int32 i = 0; i < limit_act_temp->CycleFirstId.size(); i++)
		{
			std::vector<int32> old_cycle_first_branch_temp_id_arr;
			std::vector<int32> new_cycle_first_branch_temp_id_arr;
			if (i + 1 < limit_act_temp->CycleFirstId.size())
			{
				old_cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(m_time_limit_activity_temp_map[act_temp_id].CycleFirstId[i], m_time_limit_activity_temp_map[act_temp_id].CycleFirstId[i + 1], limit_act_temp->ActivityType);
				new_cycle_first_branch_temp_id_arr = get_first_cycle_branch_temp_arr_by_template(limit_act_temp->CycleFirstId[i], limit_act_temp->CycleFirstId[i + 1], limit_act_temp->ActivityType);
			}
			else
			{
				old_cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(m_time_limit_activity_temp_map[act_temp_id].CycleFirstId[i], 0, limit_act_temp->ActivityType);
				new_cycle_first_branch_temp_id_arr = get_first_cycle_branch_temp_arr_by_template(limit_act_temp->CycleFirstId[i], 0, limit_act_temp->ActivityType);
			}

			if (old_cycle_first_branch_temp_id_arr.size() != new_cycle_first_branch_temp_id_arr.size())
			{
				continue;
			}

			for (int32 j = 0; j < old_cycle_first_branch_temp_id_arr.size(); j++)
			{
				TimeLimitActivityBranchTemplate* limit_act_branch_temp = template_manager::get_instance().get_time_limit_activity_branch_template_ptr(new_cycle_first_branch_temp_id_arr[j]);
				change_limit_act_branch_temp(limit_act_branch_temp, old_cycle_first_branch_temp_id_arr[j]);
			}
		}
	}

	void time_limit_activity_temp_ws_mgr::make_new_limit_act_branch_temp_with_temp(const TimeLimitActivityBranchTemplate * limit_act_temp, int32 mark_time)
	{
		if (nullptr == limit_act_temp || limit_act_temp->ParamArr4.size() <= 0)
		{
			return;
		}
		int32 temp_server_id = 0;
		if (!init_unit::is_cross_time_limit_cross_activity(limit_act_temp->ActivityType))
		{
			temp_server_id = world_server::getInstance().get_server_id() % 1000;
		}
		int32 act_guid = mark_time * time_limit_day_mark_num + temp_server_id * 10000 + limit_act_temp->ParamArr4[0];
		m_time_limit_activity_branch_temp_map[act_guid].clear_data();
		m_time_limit_activity_branch_temp_map[act_guid].ActGuid = act_guid;
		m_time_limit_activity_branch_temp_map[act_guid].AttributeId = limit_act_temp->attribute_id;
		m_time_limit_activity_branch_temp_map[act_guid].ActivityType = limit_act_temp->ActivityType;
		m_time_limit_activity_branch_temp_map[act_guid].CycleIndex = limit_act_temp->CycleIndex;
		m_time_limit_activity_branch_temp_map[act_guid].Condition = limit_act_temp->Condition;
		m_time_limit_activity_branch_temp_map[act_guid].Reward = limit_act_temp->Reward;
		m_time_limit_activity_branch_temp_map[act_guid].TextImage = limit_act_temp->TextImage;
		m_time_limit_activity_branch_temp_map[act_guid].ParamArr1 = limit_act_temp->ParamArr1;
		m_time_limit_activity_branch_temp_map[act_guid].ParamArr2 = limit_act_temp->ParamArr2;
		m_time_limit_activity_branch_temp_map[act_guid].ParamArr3 = limit_act_temp->ParamArr3;
		m_time_limit_activity_branch_temp_map[act_guid].ParamArr4 = limit_act_temp->ParamArr4;
		m_time_limit_activity_branch_temp_map[act_guid].ParamArr5 = limit_act_temp->ParamArr5;
		save_branch_temp_to_db(m_time_limit_activity_branch_temp_map[act_guid]);
	}
	bool time_limit_activity_temp_ws_mgr::is_self_region_act(const std::vector<int32>& param_int_arr)
	{
		if (param_int_arr.size() <= 0)
		{
			return true;
		}
		else
		{
			RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
			if (region_template_ptr != nullptr)
			{
				for (int32 i = 0; i < param_int_arr.size(); ++i)
				{
					if (region_template_ptr->RegionCode == param_int_arr[i])
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void time_limit_activity_temp_ws_mgr::check_and_clear_branch_template_no_use()
	{
		std::set<int32> all_id_set;
		all_id_set.clear();
		for (const auto& time_limit_activity_template_ref : m_time_limit_activity_temp_map)
		{
			const std::vector<int32>& cycle_first_branch_temp_arr = time_limit_activity_template_ref.second.CycleFirstId;
			for (int32 i = 0; i < cycle_first_branch_temp_arr.size(); i++)
			{
				std::vector<int32> cycle_first_branch_temp_id_arr;
				if (i + 1 < cycle_first_branch_temp_arr.size())
				{
					cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(cycle_first_branch_temp_arr[i], cycle_first_branch_temp_arr[i + 1], time_limit_activity_template_ref.second.ActivityType);

				}
				else
				{
					cycle_first_branch_temp_id_arr = get_first_cycle_branch_struct_arr_by_template(cycle_first_branch_temp_arr[i], 0, time_limit_activity_template_ref.second.ActivityType);
				}
				for (int32 j = 0; j < cycle_first_branch_temp_id_arr.size(); j++)
				{
					all_id_set.insert(cycle_first_branch_temp_id_arr[j]);
				}
			}
		}

		std::vector<int32> need_delete_template_id_arr;
		need_delete_template_id_arr.clear();
		for (const auto& time_limit_activity_branch_template_ref : m_time_limit_activity_branch_temp_map)
		{
			if (all_id_set.find(time_limit_activity_branch_template_ref.second.ActGuid) == all_id_set.end())
			{
				need_delete_template_id_arr.push_back(time_limit_activity_branch_template_ref.second.ActGuid);
			}
		}

		for (int32 i = 0; i < need_delete_template_id_arr.size(); i++)
		{
			delete_branch_temp(need_delete_template_id_arr[i]);
		}
	}

	bool time_limit_activity_temp_ws_mgr::is_self_server_temp_line(e_time_limit_activity_type activity_type, const std::vector<int32>& param_int_arr)
	{
		if (init_unit::is_cross_time_limit_cross_activity(activity_type))
		{
			if (world_server::getInstance().get_cross_id() <= 0)
			{
				return false;
			}
			int32 cur_time_limit_ranking_level = world_server::getInstance().get_server_info_arr(e_server_info_type_server_cross_state);
			if (param_int_arr.size() >= 1 && cur_time_limit_ranking_level == param_int_arr[0])
			{
				return true;
			}
		}
		else
		{
			int32 cur_server_state = e_server_state_new;
			if (world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time) > 0)
			{
				cur_server_state = e_server_state_cross;
			}
			if (world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time) > 0)
			{
				cur_server_state = e_server_state_merge;
			}
			for (int32 i = 0; i < param_int_arr.size(); i++)
			{
				if (param_int_arr[i] == cur_server_state || param_int_arr[i] == 0)
				{
					return true;
				}
			}
		}
		return false;
	}

	void time_limit_activity_temp_ws_mgr::check_and_update_branch_temp_info(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id, int32 mark_time, int32 template_from_type)
	{
		if (nullptr == limit_act_temp || act_temp_id <= 0 || m_time_limit_activity_temp_map.find(act_temp_id) == m_time_limit_activity_temp_map.end())
		{
			return;
		}
		int32 compare_result = compare_cycle_first_branch_temp_arr(limit_act_temp, act_temp_id);
		if (compare_result == e_compare_temp_result_new)
		{
			CONSOLE_INFO(" activity type:{} template from type:{}", limit_act_temp->ActivityType, template_from_type);
			for (int32 first_branch_template_id : m_time_limit_activity_temp_map[act_temp_id].CycleFirstId)
			{
				CONSOLE_INFO(" old id : {}", first_branch_template_id);
			}
			delete_cycle_first_branch_temp_arr(m_time_limit_activity_temp_map[act_temp_id].CycleFirstId, limit_act_temp->ActivityType);
			m_time_limit_activity_temp_map[act_temp_id].CycleFirstId = make_new_limit_act_branch_temp_with_temp_arr(limit_act_temp->CycleFirstId
				, m_time_limit_activity_temp_map[act_temp_id].OriginalCycleFirstId, mark_time, limit_act_temp->ActivityType);
			m_time_limit_activity_temp_map[act_temp_id].TemplateFromType = template_from_type;
			for (int32 first_branch_template_id : m_time_limit_activity_temp_map[act_temp_id].CycleFirstId)
			{
				CONSOLE_INFO(" new id : {}", first_branch_template_id);
			}
			save_temp_to_db(m_time_limit_activity_temp_map[act_temp_id]);
		}
		else if (compare_result == e_compare_temp_result_change)
		{
			m_time_limit_activity_temp_map[act_temp_id].TemplateFromType = template_from_type;
			save_temp_to_db(m_time_limit_activity_temp_map[act_temp_id]);
			change_limit_act_branch_temp_all(limit_act_temp, act_temp_id);
		}
	}

	void time_limit_activity_temp_ws_mgr::delete_temp_info_when_time_over()
	{
		int32 cur_time = time_helper::get_cur_time_new().second;
		for (int32 i = 0; i < e_time_limit_activity_type_max; i++)
		{
			vector<int32> activity_id_arr = get_activity_id_arr_by_type(i);
			if (activity_id_arr.size() <= 1)
			{
				continue;
			}
			if (init_unit::is_no_superposition_time_limit_activity(i))
			{
				for (int32 j = 1; j < activity_id_arr.size(); j++)
				{
					delete_temp(activity_id_arr[j]);
				}
			}
			else
			{
				for (int32 j = 1; j < activity_id_arr.size(); j++)
				{
					if (m_time_limit_activity_temp_map.find(activity_id_arr[j]) == m_time_limit_activity_temp_map.end())
					{
						continue;
					}
					if (m_time_limit_activity_temp_map[activity_id_arr[j]].EndTime <= 0)
					{
						int32 open_time = 0;
						int32 first_branch_temp_id = 0;
						int32 end_time = 0;
						int32 cycle_index = 0;
						int32 cycle_num = 0;
						int32 base_time = 0;
						int32 cur_branch_temp_id = init_unit::get_cur_time_limit_activity_id_by_temp(m_time_limit_activity_temp_map[activity_id_arr[j]], m_time_limit_activity_branch_temp_map
							, open_time, end_time, cycle_num, cycle_index, base_time, first_branch_temp_id, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time)
							, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time), world_server::getInstance().is_sky_island_server());
						if (cur_branch_temp_id <= 0)
						{
							delete_temp(activity_id_arr[j]);
						}
						else
						{
							m_time_limit_activity_temp_map[activity_id_arr[j]].EndTime = end_time + m_time_limit_activity_temp_map[activity_id_arr[j]].DurationDays * day_time_second;
							save_temp_to_db(m_time_limit_activity_temp_map[activity_id_arr[j]]);
						}
					}
					else if (cur_time > m_time_limit_activity_temp_map[activity_id_arr[j]].EndTime)
					{
						delete_temp(activity_id_arr[j]);
					}
				}
			}
		}
	}

	int32 time_limit_activity_temp_ws_mgr::get_cur_mark_time()
	{
		int32 cur_second = time_helper::get_local_cur_time_stamp() / second_tick_time;
		int32 cur_day = cur_second / day_time_second;
		return cur_day % 100;
	}

	std::vector<int32> time_limit_activity_temp_ws_mgr::get_activity_id_arr_by_type(int32 activity_type)
	{
		std::vector<int32> activity_id_arr;
		if (activity_type < 0 || activity_type >= e_time_limit_activity_type_max)
		{
			return activity_id_arr;
		}
		for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end(); ite++)
		{
			if (ite->second.ActivityType == activity_type)
			{
				activity_id_arr.push_back(ite->first);
			}
		}
		std::sort(activity_id_arr.begin(), activity_id_arr.end(), [&](const int32& a, const int32& b)
		{
			if (m_time_limit_activity_temp_map.find(a) == m_time_limit_activity_temp_map.end() || m_time_limit_activity_temp_map.find(b) == m_time_limit_activity_temp_map.end())
			{
				return true;
			}
			return m_time_limit_activity_temp_map[a].CreateTime > m_time_limit_activity_temp_map[b].CreateTime;
		});
		return activity_id_arr;
	}

	s_time_limit_activity_temp time_limit_activity_temp_ws_mgr::exchange_to_struct_by_template_ptr(const TimeLimitActivityTemplate* template_ptr)
	{
		s_time_limit_activity_temp activity_info;
		if (nullptr == template_ptr)
		{
			return activity_info;
		}
		activity_info.AttributeId = template_ptr->attribute_id;
		activity_info.ActivityType = template_ptr->ActivityType;
		activity_info.CycleParamType = template_ptr->CycleParamType;
		activity_info.CycleParamNum = template_ptr->CycleParamNum;
		activity_info.CycleFirstId = template_ptr->CycleFirstId;
		activity_info.OpenType = template_ptr->OpenType;
		activity_info.OpenServerDate = template_ptr->OpenServerDate;
		activity_info.OpenSpencificDate = template_ptr->OpenSpencificDate;
		activity_info.FirstOpenDayOfWeek = template_ptr->FirstOpenDayOfWeek;
		activity_info.DurationDays = template_ptr->DurationDays;
		activity_info.CycleDays = template_ptr->CycleDays;
		activity_info.ShowType = template_ptr->ShowType;
		activity_info.TextImage = template_ptr->TextImage;
		activity_info.ParamArr1 = template_ptr->ParamArr1;
		activity_info.ParamArr2 = template_ptr->ParamArr2;
		activity_info.ParamArr3 = template_ptr->ParamArr3;
		activity_info.ParamArr4 = template_ptr->ParamArr4;
		activity_info.ParamArr5 = template_ptr->ParamArr5;
		return activity_info;
	}

	void time_limit_activity_temp_ws_mgr::delete_time_limit_activity_by_type(int32 time_limit_activity_type)
	{
		vector<int32> activity_id_arr;
		if (time_limit_activity_type == -1 * e_time_limit_activity_delete_type_operator_use)
		{
			for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end(); ++ite)
			{
				if (ite->second.TemplateFromType == e_time_limit_template_from_type_operator_use)
				{
					activity_id_arr.push_back(ite->first);
				}
			}
		}
		else if (time_limit_activity_type == -1 * e_time_limit_activity_delete_type_time_limit_activity)
		{
			for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end(); ++ite)
			{
				if (ite->second.TemplateFromType == e_time_limit_template_from_type_time_limit_activity)
				{
					activity_id_arr.push_back(ite->first);
				}
			}
		}
		else if (time_limit_activity_type == -1 * e_time_limit_activity_delete_type_all)
		{
			for (auto ite = m_time_limit_activity_temp_map.begin(); ite != m_time_limit_activity_temp_map.end(); ++ite)
			{
				activity_id_arr.push_back(ite->first);
			}
		}
		else if (time_limit_activity_type >= e_time_limit_activity_delete_type_normal)
		{
			activity_id_arr = get_activity_id_arr_by_type(time_limit_activity_type);
		}
		for (int32 i = 0; i < activity_id_arr.size(); ++i)
		{
			delete_temp(activity_id_arr[i]);
		}
		time_limit_activity_ws_mgr::get_instance().init_all_time_limit_ws();
		cloud_shop_mgr::get_instance().send_open_msg_to_all_client();
		syn_template_info_to_cs_and_client_and_fep();
	}

	void time_limit_activity_temp_ws_mgr::syn_template_info_to_cs_and_client_and_fep()
	{
		send_to_cs_template();
		send_to_fep_template();
	}

	bool time_limit_activity_temp_ws_mgr::is_can_load_new_temp()
	{
		return world_server::getInstance().is_loading_flag_finish(e_ws_flag_cs_connect) && world_server::getInstance().is_loading_flag_finish(e_ws_flag_server_cross_time)
			&& world_server::getInstance().is_loading_flag_finish(e_ws_flag_time_limit_template) && world_server::getInstance().is_loading_flag_finish(e_ws_flag_cross_server_state)
			&& world_server::getInstance().is_loading_flag_finish(e_ws_flag_load_cloud_shop_info) && world_server::getInstance().is_loading_flag_finish(e_ws_flag_fep_connect);
	}

	void time_limit_activity_temp_ws_mgr::load_new_temp_on_server_open()
	{
		if (is_can_load_new_temp() && !world_server::getInstance().is_loading_flag_finish(e_ws_flag_init_time_limit_template))
		{
			load_new_temp(m_is_load_empty_from_db, true);
		}
	}

	void time_limit_activity_temp_ws_mgr::refresh_activity_guid(int32 old_act_guid, int32 new_act_guid)
	{
		ws2dp_refresh_role_time_limit_guid msg;
		msg.old_act_guid = old_act_guid;
		msg.new_act_guid = new_act_guid;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void time_limit_activity_temp_ws_mgr::load_limit_act_temp(bool is_need_update, bool is_need_refresh_guid)
	{
		std::set<int32> add_id_set;
		add_id_set.clear();
		int32 mark_time = get_cur_mark_time();
		load_limit_act_temp(mark_time, is_need_update, add_id_set, is_need_refresh_guid, e_OperatorUseActivityTemplate);
		load_limit_act_temp(mark_time, is_need_update, add_id_set, is_need_refresh_guid, e_TimeLimitActivityTemplate);
	}

	void time_limit_activity_temp_ws_mgr::load_limit_act_temp(int32 mark_time, bool is_need_update, std::set<int32>& add_id_set, bool is_need_refresh_guid, e_template template_name)
	{
		int32 template_from_type = 0;
		if (template_name == e_OperatorUseActivityTemplate)
		{
			template_from_type = e_time_limit_template_from_type_operator_use;
		}
		else if (template_name == e_TimeLimitActivityTemplate)
		{
			template_from_type = e_time_limit_template_from_type_time_limit_activity;
		}
		template_manager::template_type* local_table_ptr = template_manager::get_instance().get_templates(template_name);
		if (nullptr != local_table_ptr)
		{
			int32 local_act_temp_id;
			auto local_ite = local_table_ptr->begin();
			for (; local_ite != local_table_ptr->end(); ++local_ite)
			{
				TimeLimitActivityTemplate* time_limit_act_ptr = (TimeLimitActivityTemplate*)(local_ite->second);
				if (nullptr == time_limit_act_ptr)
				{
					continue;
				}
				if (time_limit_act_ptr->attribute_id <= 0 || time_limit_act_ptr->ActivityType < e_time_limit_activity_type_total_charge
					|| time_limit_act_ptr->ActivityType >= e_time_limit_activity_type_max)
				{
					continue;
				}
				if (!is_self_server_temp_line((e_time_limit_activity_type)time_limit_act_ptr->ActivityType, time_limit_act_ptr->ParamArr5))
				{
					continue;
				}
				if (!is_self_region_act(time_limit_act_ptr->ParamArr4))
				{
					continue;
				}
				auto id_ite = add_id_set.find(time_limit_act_ptr->ActivityType);
				if (id_ite != add_id_set.end())
				{
					continue;
				}
				add_id_set.insert(time_limit_act_ptr->ActivityType);
				local_act_temp_id = get_new_limit_act_temp_with_type(time_limit_act_ptr->ActivityType);
				int32 compare_state = compare_limit_act_temp(time_limit_act_ptr, local_act_temp_id, is_need_refresh_guid);
				if (compare_state == e_compare_temp_result_new)
				{
					make_new_limit_act_temp_with_temp(time_limit_act_ptr, mark_time, is_need_update, template_from_type);
				}
				else if (compare_state == e_compare_temp_result_change)
				{
					change_limit_act_temp(time_limit_act_ptr, local_act_temp_id, mark_time, template_from_type);
				}
				else if (compare_state == e_compare_temp_result_none)
				{
					check_and_update_branch_temp_info(time_limit_act_ptr, local_act_temp_id, mark_time, template_from_type);
				}
				else if (compare_state == e_compare_temp_result_refresh_guid)
				{
					delete_temp(local_act_temp_id);
					int32 new_act_guid = make_new_limit_act_temp_with_temp(time_limit_act_ptr, mark_time, is_need_update, template_from_type);
					refresh_activity_guid(local_act_temp_id, new_act_guid);
				}
			}
		}
	}

	int32 time_limit_activity_temp_ws_mgr::get_new_limit_act_temp_with_type(int32 activity_type)
	{
		int32 return_id = -1;
		int32 cur_max_create_time = 0;
		auto ite = m_time_limit_activity_temp_map.begin();
		for (;ite != m_time_limit_activity_temp_map.end(); ++ite)
		{
			if (ite->second.ActivityType == activity_type)
			{
				int32 cur_create_time = ite->second.CreateTime;
				if (cur_create_time > cur_max_create_time)
				{
					cur_max_create_time = cur_create_time;
					return_id =	ite->second.ActGuid;
				}
			}
		}
		return return_id;
	}

	void time_limit_activity_temp_ws_mgr::fill_time_limit_activity_template_one_msg(const s_time_limit_activity_temp * template_line, game_proto_template_table_data * activity_template_data)
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

	void time_limit_activity_temp_ws_mgr::fill_time_limit_activity_branch_template_one_msg(const s_time_limit_activity_branch_temp* template_line, game_proto_template_table_data * activity_template_data)
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
