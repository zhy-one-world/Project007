/********************************************************************
created: 2016年10月18日
file base: title_mgr
file ext: cpp
author: wucun

purpose: about role's title_mgr
*********************************************************************/

#include "cell_server.hpp"
#include "title_mgr.h"
#include "logic/unit_man.h"
#include "lua/script_mgr.h"
#include "logic/player.hpp"
#include "time_def.hpp"
#include "internet/title.pb.h"
#include "internet/net.pb.h"
#include "internet/cs2dp.pb.h"
#include "internal/title_msg.hpp"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"
#include "utility/globle_data.h"

namespace hld
{
	ctitle_mgr::ctitle_mgr()
	{
		m_unit_array_index = 0;
		clear_data();
	}
	ctitle_mgr::~ctitle_mgr(void)
	{
	}
	void ctitle_mgr::clear_data()
	{
		m_title_info.clear();
		memset(m_loading_finish_need_add_title_type_arr, 0, sizeof(m_loading_finish_need_add_title_type_arr));
	}
	void ctitle_mgr::heart_tick(const int64& new_time)
	{	
		int32 current_time = time_helper::get_cur_time_new().second;
		for (title_map_it it = m_title_info.begin(); it != m_title_info.end(); ++it)
		{
			ctitle& title_ref = it->second;
			if (title_ref.get_inst_data(e_title_info_if_geted) > 0)
			{
				TitleTemplate* title_template_ptr = title_ref.get_title_template_ptr();
				if (title_template_ptr == nullptr)
				{
					return;
				}
				if (title_template_ptr->EndTimeType == hld::e_title_end_type_time || title_template_ptr->EndTimeType == hld::e_title_end_type_condition_1)
				{
					int32 remain_time = title_ref.get_inst_data(e_title_info_end_time) - current_time;
					if (remain_time <= 0)
					{
						remove_title_by_template_id(title_ref.get_inst_data(e_title_info_template_id));
					}
				}
			}
		}
	}


	void ctitle_mgr::push_msg_own_title_all()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		title_proto_own_title_msg_all resp;
		for (title_map_it it = m_title_info.begin(); it != m_title_info.end(); ++it)
		{
			if (it->second.get_inst_data(hld::e_title_info_if_geted) > 0)
			{
				title_proto_title_info_one* title_info_one_resp = resp.add_title_info_arr();
				for (int32 i = 0; i < e_title_info_max; ++i)
				{
					title_info_one_resp->add_title_data(it->second.get_inst_data((e_title_info)i));
				}
			}
		}

		player_ref.send_message_to_self(&resp, e_msgindex_s2c_title_own_title_msg_all);
	}

	void ctitle_mgr::push_msg_one_title(int32 title_template_id)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		title_map_it it = m_title_info.find(title_template_id);
		if (it == m_title_info.end())
		{
			return;
		}
		title_proto_title_info_one resp;
		for (int32 i = 0; i < e_title_info_max; ++i)
		{
			resp.add_title_data(it->second.get_inst_data((e_title_info)i));
		}
		player_ref.send_message_to_self(&resp, e_msgindex_s2c_title_title_msg_one);
	}

	void ctitle_mgr::add_title_msg_send(int32 title_template_id, int32 result)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		title_proto_title_add_title_end msg;
		msg.add_unit_guid(player_ref.get_unit_guid().A);
		msg.add_unit_guid(player_ref.get_unit_guid().B);
		msg.set_template_id(title_template_id);
		msg.set_result(result);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_title_add_title_end);
	}

	void ctitle_mgr::open_title_system_send()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		title_proto_title_open_title_system_end msg;
		msg.add_unit_guid(player_ref.get_unit_guid().A);
		msg.add_unit_guid(player_ref.get_unit_guid().B);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_title_open_title_system_end);
	}

	void ctitle_mgr::save_title_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_title);
		if (is_use == false)
		{
			cs2dp_save_char_title req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = player_ref.get_array_index();
			int32 title_count = 0;
			for (title_map_it it = m_title_info.begin(); it != m_title_info.end(); ++it)
			{
				if (title_count >= MAX_TITLE_NUM)
				{
					break;
				}
				req.data_list[title_count] = it->second.get_title_info_all();
				title_count++;
			}
			req.data_num = title_count;
			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_char_title));
		}
		else
		{
			hld::cs2dp_proto::save_role_title msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(player_ref.get_array_index());
			msg.set_save_type_ex(save_type);

			hld::cs2dp_proto::role_title_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			int32 row_count = 0;
			for (title_map_it it = m_title_info.begin(); it != m_title_info.end(); ++it)
			{
				if (row_count >= MAX_TITLE_NUM)
				{
					break;
				}
				hld::cs2dp_proto::role_title_row *db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				s_title_info& tmp = it->second.get_title_info_all();
				for (int32 i =0 ; i< e_title_info_max; i++)
				{
					db_row->add_data_ary(tmp.data_ary[i]);

				}
				row_count++;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_title);
		}

	}
	void ctitle_mgr::load_title_by_db(const s_title_info* title_data, int32 data_num)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		for (int32 i = 0; i < data_num; i++)
		{
			add_title_by_info(title_data[i]);
		}
		hld::template_manager::template_type* title_table = template_manager::get_instance().get_templates(e_TitleTemplate);
		if (nullptr == title_table)
		{
			return;
		}
		hld::template_manager::template_type::iterator ite;
		int32 title_count = 0;
		for (ite = title_table->begin(); ite != title_table->end(); ++ite)
		{
			TitleTemplate* title_template_ptr = (TitleTemplate*)(ite->second);
			if (title_template_ptr == nullptr)
			{
				continue;
			}

			title_map_it it = m_title_info.find(title_template_ptr->attribute_id);
			if (it != m_title_info.end())
			{
				continue;
			}

			bool is_channel_can_add = false;
			if (title_template_ptr->ChannelLimit.size() == 0)
			{
				is_channel_can_add = true;
			}
			if (!is_channel_can_add && title_template_ptr->ChannelLimit.size() % 2 != 0)
			{
				continue;
			}
			for (int32 i = 0; i < title_template_ptr->ChannelLimit.size(); i += 2)
			{
				const std::string& target_login_type = title_template_ptr->ChannelLimit[i];
				const std::string& target_recharge_id = title_template_ptr->ChannelLimit[i + 1];
				if (player_ref.get_login_type() == init_unit::change_string_to_i32(target_login_type) 
					&& (target_recharge_id == "" || player_ref.get_third_info().param14 == target_recharge_id))
				{
					is_channel_can_add = true;
					break;
				}
			}

			if (cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				if (title_template_ptr->attribute_id == city_master_title_id)
				{
					is_channel_can_add = false;
				}
			}
			else
			{
				if (title_template_ptr->attribute_id == city_war_cross_master_title_id)
				{
					is_channel_can_add = false;
				}
			}

			if (cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				if (title_template_ptr->attribute_id == city_war_winner_title_id)
				{
					is_channel_can_add = false;
				}
			}
			else
			{
				if (title_template_ptr->attribute_id == city_war_cross_winner_title_id)
				{
					is_channel_can_add = false;
				}
			}
			
			if (!is_channel_can_add)
			{
				continue;
			}

			ctitle new_title;
			new_title.init_title_by_template(title_template_ptr->attribute_id);
			m_title_info[title_template_ptr->attribute_id] = new_title;
		}

		push_msg_own_title_all();

		// 处理离线时的title情况
		// 要最后执行这个，否则会重复加
		for (int32 i = 0; i < MAX_TITLE_NUM; i++)
		{
			e_title_special_handle handle_type = m_loading_finish_need_add_title_type_arr[i];
			switch (handle_type)
			{
			case hld::e_title_special_handle_none:
			{
				break;
			}
			case hld::e_title_special_handle_add:
			{
				const int32 title_template_id = get_template_id_by_type_and_value(e_title_type(i), 0, 0);
				if (is_have_title(title_template_id) == false)
				{
					add_title_by_template_id(title_template_id);
				}
				break;
			}
			case hld::e_title_special_handle_remove:
			{
				const int32 title_template_id = get_title_template_id_by_type(e_title_type(i));
				remove_title_by_template_id(title_template_id);
				break;
			}
			default:
				break;
			}
		}
	}

	bool ctitle_mgr::load_title_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_title_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_title_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_title_info *p_row = (s_title_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::cs2dp_proto::role_title_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_title_by_db((const s_title_info *)p_data, msg.row_count());
		return true;
	}
	//基本操作
	bool ctitle_mgr::is_have_title(int32 title_template_id)
	{
		title_map_it it = m_title_info.find(title_template_id);
		if (it != m_title_info.end() && it->second.get_inst_data(e_title_info_if_geted) == 1)
		{
			return true;
		}
		return false;
	}
	bool ctitle_mgr::is_equip_title(int32 title_template_id)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		if (player_ref.get_pawn_att().get_unit_base_att(hld::e_base_att_info_equip_title_id) == title_template_id)
		{
			return true;
		}
		return false;
	}

	int32 ctitle_mgr::get_title_template_id_by_type(e_title_type title_type)
	{
		hld::template_manager::template_type* title_table = template_manager::get_instance().get_templates(e_TitleTemplate);
		if (nullptr == title_table)
		{
			return -1;
		}
		hld::template_manager::template_type::iterator ite;
		int32 title_count = 0;
		for (ite = title_table->begin(); ite != title_table->end(); ++ite)
		{
			TitleTemplate *title_template_ptr = (TitleTemplate*)(ite->second);
			if (title_template_ptr->HighTitleType == title_type)
			{
				return title_template_ptr->attribute_id;
			}
		}
		return -1;
	}
	int32 ctitle_mgr::get_template_id_by_type_and_value(e_title_type title_type, int32 value1, int32  value2)
	{
		hld::template_manager::template_type* title_table = template_manager::get_instance().get_templates(e_TitleTemplate);
		if (nullptr == title_table)
		{
			return -1;
		}
		int32 title_template_id = -1;
		hld::template_manager::template_type::iterator ite;
		for (ite = title_table->begin(); ite != title_table->end(); ++ite)	// 特殊处理
		{
			TitleTemplate *title_template_ptr = title_template_ptr = (TitleTemplate*)(ite->second);
			if (nullptr == title_template_ptr)
			{
				continue;
			}
			if (title_template_ptr->HighTitleType == title_type)
			{
				if ((title_template_ptr->NeedValue1 == 0 || title_template_ptr->NeedValue1 == value1)
					&& (title_template_ptr->NeedValue2 == 0 || title_template_ptr->NeedValue2 == value1))
				{
					return title_template_ptr->attribute_id;
				}
				return -1;
			}
		}
		return -1;
	}
	void ctitle_mgr::add_loading_finish_title_type(e_title_type title_type, e_title_special_handle handle_type)
	{
		if (title_type >= hld::e_title_type_max)
		{
			return;
		}
		m_loading_finish_need_add_title_type_arr[title_type] = handle_type;
	}

	void ctitle_mgr::add_loading_ranking_finish_title_type( const bool(&loading_title_flag)[hld::e_title_type_max])
	{ 
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		for (int32 i = 0; i < e_title_type_max; i++)
		{
			if (false == player_ref.is_self_server() && false == init_unit::is_cross_server_title(i, cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity)))
			{
				continue;
			}
			if (loading_title_flag[i])
			{
				add_loading_finish_title_type(e_title_type(i), e_title_special_handle_add); 
			}
			else
			{
				int32 title_id = get_title_template_id_by_type(e_title_type(i));
				TitleTemplate* title_ptr =  GET_TEMPLATE(TitleTemplate, title_id);
				if (nullptr!= title_ptr)
				{
					if (title_ptr->EndTimeType > e_title_end_time_type_forever)
					{
						add_loading_finish_title_type(e_title_type(i), e_title_special_handle_remove);
					}
					else
					{
						add_loading_finish_title_type(e_title_type(i), e_title_special_handle_none);
					}
				} 
			} 
		}
		 
	}

	int32 ctitle_mgr::get_first_active_title_template_id()
	{
		for (title_map_it it = m_title_info.begin(); it != m_title_info.end(); ++it)
		{
			if (it->second.get_inst_data(e_title_info_if_geted) > 0)
			{
				return it->second.get_inst_data(e_title_info_template_id);
			}
		}
		return 0;
	}

	int32 ctitle_mgr::is_add_title_by_template_id(int32 title_template_id)
	{
		if (title_template_id <= 0)
		{
			return 0;
		}

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return -1;
		}
		e_title_add_result_type result = hld::e_title_add_result_type_none;
		TitleTemplate* title_template_ptr = GET_TEMPLATE(TitleTemplate, title_template_id);
		if (title_template_ptr == nullptr)
		{
			return -1;
		}
		title_map_it it = m_title_info.find(title_template_id);
		if (it == m_title_info.end())
		{
			return 0;
		}
		int32 is_exist = it->second.get_inst_data(e_title_info_if_geted);
		if (is_exist == 0)
		{
			return 0;
		}
		if (title_template_ptr->EndTimeType == hld::e_title_end_type_forever ||
			title_template_ptr->EndTimeType == hld::e_title_end_type_condition_2)
		{
			return -1;
		}
		return 0;	
	}

	//添加与删除
	void ctitle_mgr::add_title_by_template_id(int32 title_template_id)	//1--添加称号,2--增加时长,3--不可再次使用
	{
		if (title_template_id <= 0)
		{
			return;
		}

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		e_title_add_result_type result = hld::e_title_add_result_type_none;
		TitleTemplate* title_template_ptr = GET_TEMPLATE(TitleTemplate, title_template_id);
		if (title_template_ptr == nullptr)
		{
			return;
		}
		title_map_it it = m_title_info.find(title_template_id);

		s_title_info title_info;
		title_info.data_ary[e_title_info_template_id] = title_template_id;
		title_info.data_ary[e_title_info_if_geted] = 1;
		title_info.data_ary[e_title_info_if_need_see] = 1;
		title_info.data_ary[e_title_info_if_need_equip] = 1;

		switch (title_template_ptr->EndTimeType)
		{
		case hld::e_title_end_type_forever:
		case hld::e_title_end_type_condition_2:
		{
			if (it != m_title_info.end())
			{
				if (it->second.get_inst_data(e_title_info_if_geted))
				{
					result = hld::e_title_add_result_type_cannot_use_again;
				}
				else
				{
					it->second.set_inst_data(e_title_info_if_geted, 1);
					it->second.set_title_static_att(m_unit_array_index, true);
				}
			}
			else
			{
				result = hld::e_title_add_result_type_normal;
			}
			break;
		}
		case hld::e_title_end_type_time:
		{
			if (1 == title_template_ptr->DeadActiveLine)
			{
				if (it != m_title_info.end() && it->second.get_inst_data(e_title_info_if_geted))
				{
					result = hld::e_title_add_result_type_none;
				}
				else
				{
					int32 add_time = 0;
					switch (title_template_ptr->HighTitleType)
					{
					case 1:
					{
						ActivityCommonConfigTemplate* activity_common_config_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, 120000002);
						std::vector<int32> open_time = cell_server::getInstance().get_activity_open_time(e_activity_type_pk_king);
						if (nullptr != activity_common_config_template_ptr && once_activity_open_time_array_size < open_time.size())
						{
							int32 cur_stamp = time_helper::get_cur_time_new().second;
							int32 begin_stamp = time_helper::get_stamp_by_hour_min(open_time[2], open_time[3]) + hld::day_time_second;
							add_time = begin_stamp - cur_stamp;
						}
					}
					default:
						break;
					}
					//添加该称号
					title_info.data_ary[e_title_info_start_time] = time_helper::get_cur_time_new().second;
					title_info.data_ary[e_title_info_end_time] = time_helper::get_cur_time_new().second + add_time;
					result = hld::e_title_add_result_type_normal;
				}
			}
			else if (0 < title_template_ptr->LastTime)
			{
				if (it != m_title_info.end())
				{
					//叠加时间(start_time后推持续时间)
					int32 end_time = it->second.get_inst_data(e_title_info_end_time) + title_template_ptr->LastTime * 60;
					it->second.set_inst_data(e_title_info_end_time, end_time);
					result = hld::e_title_add_result_type_add_time;
				}
				else
				{
					//添加该称号
					title_info.data_ary[e_title_info_start_time] = time_helper::get_cur_time_new().second;
					title_info.data_ary[e_title_info_end_time] = time_helper::get_cur_time_new().second + title_template_ptr->LastTime * 60;
					result = hld::e_title_add_result_type_normal;
				}
			}
			break;
		}
		case hld::e_title_end_type_condition_1:
		{
			title_info.data_ary[e_title_info_start_time] = time_helper::get_cur_time_new().second;
			title_info.data_ary[e_title_info_end_time] = time_helper::get_cur_time_new().second;
			result = hld::e_title_add_result_type_normal;
		}
		break;
		}
		if (result == hld::e_title_add_result_type_normal)
		{
			add_title_by_info(title_info);
		}
		push_msg_one_title(title_template_id);
		add_title_msg_send(title_template_id, result);
		if (player_ref.get_pawn_att().get_unit_base_att(hld::e_base_att_info_equip_title_id) == 0)
		{
			player_ref.get_pawn_att().set_unit_base_att(hld::e_base_att_info_equip_title_id, -1);
			player_ref.get_pawn_att().send_base_att_one(hld::e_base_att_info_equip_title_id);
			open_title_system_send();
		}

		if (hld::e_title_add_result_type_none == result)
		{
			return;
		}

		if (hld::e_title_add_result_type_cannot_use_again != result)
		{
			// 解锁称号公告
			int32 notice_id = title_template_ptr->Noticeld;	//93000035
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(std::string(player_ref.get_name()));
			vec_notice_str.push_back(template_manager::get_instance().int_to_string(title_template_ptr->HighTitleName));
			xstring notice_str = init_unit::implode(vec_notice_str);
			player_ref.get_chat_mgr().send_notice(notice_id, notice_str);
		}
	}
	void ctitle_mgr::add_title_by_info(const s_title_info& title_info)
	{
		ctitle new_title;
		new_title.init_title_by_info(title_info);
		new_title.set_title_static_att(m_unit_array_index, true);
		new_title.set_title_equiping_att(m_unit_array_index, true);
		m_title_info[new_title.get_inst_data(e_title_info_template_id)] = new_title;
	}
	void ctitle_mgr::remove_title_by_template_id(int32 title_template_id)
	{
		if (title_template_id <= 0)
		{
			return;
		}
		title_map_it it = m_title_info.find(title_template_id);
		if (it == m_title_info.end())
		{
			return;
		}
		ctitle& title_ref = it->second;
		title_ref.set_title_static_att(m_unit_array_index, false);
		if (is_equip_title(title_template_id))
		{
			equip_off_title();
		}
		for (int32 i = e_title_info_start_time; i < e_title_info_max; ++i)
		{
			title_ref.set_inst_data(e_title_info(i), 0);
		}
		push_msg_one_title(title_template_id);
	}

	//装备与卸下
	void ctitle_mgr::equip_on_title(int32 title_template_id)
	{
		script_mgr::get_instance().call_func("role_summary", "equip_on_title_lua", 0, false, "%d%d", m_unit_array_index, title_template_id);
		/*if (title_template_id <= 0)
		{
			return;
		}
		title_map_it it = m_title_info.find(title_template_id);
		if (it == m_title_info.end())
		{
			return;
		}*/
		/*player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		equip_off_title();
		player_ref.get_pawn_att().set_unit_base_att(hld::e_base_att_info_equip_title_id, title_template_id);
		player_ref.get_pawn_att().send_base_att_one(hld::e_base_att_info_equip_title_id);
		it->second.set_title_equiping_att(m_unit_array_index, true);*/
	}
	void ctitle_mgr::equip_off_title()
	{
		script_mgr::get_instance().call_func("role_summary", "equip_off_title_lua", 0, false, "%d", m_unit_array_index);

		/*player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 title_template_id = player_ref.get_pawn_att().get_unit_base_att(hld::e_base_att_info_equip_title_id);
		if (title_template_id <= 0)
		{
			return;
		}*/
		/*title_map_it it = m_title_info.find(title_template_id);
		if (it == m_title_info.end())
		{
			return;
		}
		it->second.set_title_equiping_att(m_unit_array_index, false);
		player_ref.get_pawn_att().set_unit_base_att(hld::e_base_att_info_equip_title_id, -1);
		player_ref.get_pawn_att().send_base_att_one(hld::e_base_att_info_equip_title_id);*/
	}

	//添加称号各个情况
	void ctitle_mgr::add_title_by_type_and_value(e_title_type title_type, int32 value1, int32  value2)
	{
		int32 title_template_id = get_template_id_by_type_and_value(title_type, value1, value2);
		add_title_by_template_id(title_template_id);
	}

	void ctitle_mgr::ranking_first_changed(int32 ranking_type, bool is_first, bool is_notice)	//排行升到第一名或者从第一名掉下来
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		e_title_type title_type = e_title_type_max;
		switch (ranking_type)
		{ 
		case e_RankingIndex_arena:
			title_type = hld::e_title_type_arena_rank_first;
			break;
		case e_RankingIndex_worship:
			title_type = hld::e_title_type_worship_rank_first;
			break;
		case e_RankingIndex_gs:
			title_type = hld::e_title_type_gs_rank_first;
			break;
		case e_RankingIndex_box_map_level:
			title_type = hld::e_title_type_money_rank_first;
			break;
		case e_RankingIndex_gs_barserker:
			title_type = hld::e_title_type_gs_ranking_barserker;
			break;
		case e_RankingIndex_gs_wizard:
			title_type = hld::e_title_type_gs_ranking_wizard;
			break;
		case e_RankingIndex_gs_guardian:
			title_type = hld::e_title_type_gs_ranking_guardian;
			break;
		case e_RankingIndex_gs_assassinator:
			title_type = hld::e_title_type_gs_ranking_assassinator;
			break;  
		default:
			break;
		} 

		if (title_type >= e_title_type_max)
		{
			return;
		}
		int32 title_template_id = get_title_template_id_by_type(title_type);
		if (title_template_id<=0)
		{
			return;
		}
		if (is_first)
		{
			//如果小于一天，就不会加称号
			int32 open_days = globle_data::get_instance().get_server_on_days();
			if (open_days > 1)
			{
				add_title_by_template_id(title_template_id);
				if (is_notice)
				{
					player_ref.get_chat_mgr().send_change_first_ranking_notice(ranking_type, is_first);
				}
				if (player_ref.get_pawn_att().get_unit_base_att(hld::e_base_att_info_equip_title_id) <= 0)
				{
					player_ref.get_pawn_att().set_unit_base_att(hld::e_base_att_info_equip_title_id, title_template_id);
					player_ref.get_pawn_att().send_base_att_one(hld::e_base_att_info_equip_title_id);
					open_title_system_send();
				}
			}
		}
		else
		{
			remove_title_by_template_id(title_template_id);
		}
		 
	}

	void ctitle_mgr::set_title_no_need_see()
	{
		for (title_map_it it = m_title_info.begin(); it != m_title_info.end(); ++it)
		{
			it->second.set_inst_data(e_title_info_if_need_see, 0);
		}
	}
	ctitle& ctitle_mgr::get_title_by_template_id(int32 template_id)
	{
		title_map_it it = m_title_info.find(template_id);
		if (it == m_title_info.end())
		{
			return m_ctitle_default;
		}
		return it->second;
	}
}
