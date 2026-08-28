#include "legion_ws_event_logger.h"
#include "utility/guid_gen.h"
#include "ws_client.hpp"
// #include "time.hpp"
#include "utility/cs_date.hpp"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "legion_ws.h"
#include "legion_ws_mgr.h"
#include <legion_msg.hpp>
#include "net.pb.h"

namespace hld
{
	legion_ws_event_logger::legion_ws_event_logger()
	{
		clear_data();
	}
	legion_ws_event_logger::~legion_ws_event_logger()
	{
		clear_data();
	}

	void legion_ws_event_logger::clear_data()
	{
		m_legion_guid.clear_data();
		m_event_list.clear();
	}

	void legion_ws_event_logger::init(guid_64 cur_legion)
	{
		m_legion_guid = cur_legion;
	}

	void legion_ws_event_logger::add_member_join_event(std::string member_name)
	{
		if (member_name == "")
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_member_join_str_model_id));
		vec_show_str.push_back(member_name);
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_member_leave_event(std::string member_name)
	{
		if (member_name == "")
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_member_leave_str_model_id));
		vec_show_str.push_back(member_name);
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_member_post_change_event(std::string member_name, std::string old_job, std::string new_job)
	{
		if (member_name == "" || old_job == "" || new_job == "")
		{
			return;
		}
		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_member_post_change_str_model_id));
		vec_show_str.push_back(member_name);
		vec_show_str.push_back(old_job);
		vec_show_str.push_back(new_job);
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_donate_money_event(std::string member_name, int32 legion_up_money_num, int32 battle_achievement_num, int32 donate_money_num)
	{
		if (member_name == "" || legion_up_money_num < 0 || battle_achievement_num < 0 || donate_money_num < 0)
		{
			return;
		}
		
		std::vector<std::string> vec_show_str;
		if (battle_achievement_num == 0) 
		{
			vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_donate_money_and_item_no_battle_achievement_str_model_id));
		}
		else
		{
			vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_donate_money_and_item_str_model_id));
		}
		vec_show_str.push_back(member_name);
		vec_show_str.push_back(template_manager::get_instance().int_to_string(donate_money_num));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(90029602));//"金币"字符串id
		if (battle_achievement_num != 0)
		{
			vec_show_str.push_back(template_manager::get_instance().int_to_string(battle_achievement_num));
		}
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_up_money_num));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_donate_item_event(std::string member_name, int32 item_id, int32 item_num, int32 battle_achievement_num, int32 donate_item_get_money)
	{
		if (member_name == "" || item_id <= 0 || item_num < 0 || battle_achievement_num < 0 || donate_item_get_money < 0)
		{
			return;
		}
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (nullptr == item_template_ptr)
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		if (battle_achievement_num == 0)
		{
			vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_donate_money_and_item_no_battle_achievement_str_model_id));
		}
		else
		{
			vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_donate_money_and_item_str_model_id));
		}
		vec_show_str.push_back(member_name);
		vec_show_str.push_back(template_manager::get_instance().int_to_string(item_num));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(item_template_ptr->ItemName));
		if (battle_achievement_num != 0)
		{
			vec_show_str.push_back(template_manager::get_instance().int_to_string(battle_achievement_num));
		}
		vec_show_str.push_back(template_manager::get_instance().int_to_string(donate_item_get_money));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_legion_construction_level_up_event(ELegionInfo construction_idex, int32 level)
	{
		if (construction_idex >= ELegionInfo_max || level <= 0)
		{
			return;
		}
		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_legion_construction_level_up_str_model_id));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_main_construction_name_id + construction_idex));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(level));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_on_create_bonfire_event()
	{
		auto int_s = template_manager::get_instance().int_to_string(legion_event_on_create_bonfire_str_model_id);
		add_event(int_s);
	}

	void legion_ws_event_logger::add_occupy_city_event(std::string chief_name, int32 territory_id)
	{
		if (chief_name == "" || territory_id <= 0)
		{
			return;
		}

		CityWarTerritoryTemplate* territory_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_ptr)
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_occupy_city_str_model_id));
		vec_show_str.push_back(chief_name);
		vec_show_str.push_back(template_manager::get_instance().int_to_string(territory_ptr->TerrName));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_win_overlord_city_war_event(std::string chief_name)
	{
		if (chief_name == "")
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_win_overlord_city_war_str_model_id));
		vec_show_str.push_back(chief_name);
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_lose_city_by_war_event(int32 territory_id)
	{
		if (territory_id <= 0)
		{
			return;
		}
		CityWarTerritoryTemplate* territory_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_ptr)
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_lose_city_by_war_str_model_id));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(territory_ptr->TerrName));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_lose_city_by_maintain_money_not_enough_event(int32 territory_id)
	{
		if (territory_id <= 0)
		{
			return;
		}
		CityWarTerritoryTemplate* territory_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_ptr)
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_lose_city_by_maintain_money_not_enough_str_model_id));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(territory_ptr->TerrName));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_bid_win_event(int32 territory_id)
	{
		if (territory_id <= 0)
		{
			return;
		}
		CityWarTerritoryTemplate* territory_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_ptr)
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_bid_win_str_model_id));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(territory_ptr->TerrName));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_bid_fail_event(int32 territory_id)
	{
		if (territory_id <= 0)
		{
			return;
		}
		CityWarTerritoryTemplate* territory_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_ptr)
		{
			return;
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_bid_fail_str_model_id));
		vec_show_str.push_back(template_manager::get_instance().int_to_string(territory_ptr->TerrName));
		std::string data_str = init_unit::implode(vec_show_str);

		add_event(data_str);
	}

	void legion_ws_event_logger::add_member_change_name_event(const xstring& original_name, const xstring& current_name)
	{
		std::vector<std::string> vec_str_up_item;
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(legion_member_change_name_event_id);
		vec_str_up_item.push_back(temp_string);
		vec_str_up_item.push_back(original_name);
		vec_str_up_item.push_back(current_name);
		std::string data_str = init_unit::implode(vec_str_up_item);
	
		add_event(data_str);
	}

	void legion_ws_event_logger::clear_event()
	{
		m_event_list.clear();
	}

	void legion_ws_event_logger::add_event(std::string& data_str)
	{
		s_legion_event new_event;
		new_event.legion_guid = m_legion_guid;
		new_event.happen_time = time_helper::get_cur_time_new().second;
		memcpy(new_event.extra_data, data_str.c_str(), data_str.length() >= max_once_legion_event_size ? max_once_legion_event_size : data_str.length());
		add_event(new_event);
	}

	void legion_ws_event_logger::add_event(s_legion_event& new_event)
	{

		int32 cur_event_num = m_event_list.size();
		if (0 == cur_event_num)
		{
			new_event.event_idx = 0;
		}
		else
		{
			new_event.event_idx = m_event_list.begin()->event_idx + 1;
			if (new_event.event_idx >= max_legion_event_num)
			{
				new_event.event_idx = 0;
			}
		}
		if (cur_event_num >= max_legion_event_num)
		{
			m_event_list.pop_back();
		}

		m_event_list.push_front(new_event);
		save_legion_event_into_db(new_event);
	}

	static void fill_legion_event_msg(legion_proto_legion_event& legion_event_msg, s_legion_event& legion_event_info)
	{
		legion_event_msg.set_event_idx(legion_event_info.event_idx);
		legion_event_msg.set_extra_data(legion_event_info.extra_data);
		legion_event_msg.set_happen_time(legion_event_info.happen_time);
	}

	// 返回值说明:
	//  1 打包正常
	// -1 事件列表是空的
	// -2 申请legion_proto_legion_event数据包失败
	int32 legion_ws_event_logger::fill_get_event_list_msg(legion_proto_get_legion_event_list_end& get_event_list_end_msg, uint32 happen_time)
	{
		get_event_list_end_msg.set_event_list_len(0);
		if (m_event_list.size() <= 0)
		{
			return -1;
		}

		std::list<s_legion_event>::iterator ite;
		if (0 == happen_time)
		{
			ite = m_event_list.begin();
		}
		else
		{
			for (ite = m_event_list.begin(); ite != m_event_list.end(); ++ite)
			{
				if (ite->happen_time <= happen_time)
				{
					++ite;
					break;
				}
			}
		}

		int32 event_list_len = 0;
		for (int32 i = 0; i < once_legion_event_list_get_num; ++i)
		{
			if (ite == m_event_list.end())
			{
				break;
			}
			legion_proto_legion_event* legion_event_msg = get_event_list_end_msg.add_event_list();
			if (nullptr == legion_event_msg)
			{
				return -2;
			}
			fill_legion_event_msg(*legion_event_msg, *ite);

			++event_list_len;
			++ite;
		}
		if (event_list_len <= 0)
		{
			return -1;
		}

		get_event_list_end_msg.set_event_list_len(event_list_len);
		return 1;
	}

	void legion_ws_event_logger::recv_load_all_legion_event_from_db(const s_legion_event* event_data_arr, int32 arr_len)
	{
		for (int32 i = 0; i < arr_len; i++)
		{
			const s_legion_event* event_data = event_data_arr + i;
			if (nullptr == event_data)
			{
				continue;
			}
			load_legion_event(*event_data);
		}

		//sql中排序
		//m_event_list.sort([](const s_legion_event& a, const s_legion_event& b) { return a.happen_time > b.happen_time; });
	}

	void legion_ws_event_logger::load_legion_event(const s_legion_event& event_data)
	{
		m_event_list.push_front(event_data);
	}
	
	void legion_ws_event_logger::save_legion_event_into_db(const s_legion_event& event_data)
	{
		//ws2dp_save_legion_event save_event_data_msg;
		//save_event_data_msg.event_data = event_data;
		//ws_client::getInstance().send_to_dp(&save_event_data_msg, sizeof(save_event_data_msg));

		server2dp_proto_ws2dp_save_legion_event msg;
		server2dp_proto_s_legion_event* event_info = msg.mutable_event_data();
		if (nullptr == event_info)
		{
			return;
		}
		event_info->set_legion_guid(event_data.legion_guid.server_64);
		event_info->set_event_idx(event_data.event_idx);
		event_info->set_extra_data(event_data.extra_data);
		event_info->set_happen_time(event_data.happen_time);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_legion_event);
	}
	
	void legion_ws_event_logger::del_legion_event_from_db(int32 event_idx)
	{
		ws2dp_del_legion_event del_event_data_msg;
		del_event_data_msg.legion_guid = m_legion_guid;
		del_event_data_msg.event_idx = event_idx;
		ws_client::getInstance().send_to_dp(&del_event_data_msg, sizeof(del_event_data_msg));
	}

	void legion_ws_event_logger::clear_legion_event_in_db()
	{
		//ws2dp_clear_legion_event clear_all_event_data_msg;
		//clear_all_event_data_msg.legion_guid = m_legion_guid;
		//ws_client::getInstance().send_to_dp(&clear_all_event_data_msg, sizeof(clear_all_event_data_msg));

		server2dp_proto_ws2dp_clear_legion_event msg;
		msg.set_legion_guid(m_legion_guid.server_64);
		ws_client::getInstance().send_to_dp_new(&msg, e_msgindex_ws2dp_clear_legion_event);
	}
}