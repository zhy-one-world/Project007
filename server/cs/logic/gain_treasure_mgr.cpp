/********************************************************************
created: 2018年5月5日
file base: gain_treasure_mgr
file ext: cpp
author: wangdi

purpose: 打宝相关
*********************************************************************/


#include "connection_mgr.hpp"
#include "internet/cs2dp.pb.h"
#include "gain_treasure_mgr.h"
#include "internal/gain_treasure_msg.hpp"
#include "Logic/activity_def.hpp"
#include "player.hpp"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "internet/net.pb.h"
#include "internet/game.pb.h"
#include "utility/parse_msg.h"
#include "utility/globle_data.h"

namespace faith
{
	gain_treasure_mgr::gain_treasure_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	gain_treasure_mgr::~gain_treasure_mgr()
	{

	}

	void gain_treasure_mgr::clear_data()
	{
		for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
		{
			m_gain_treasure_info[i] = 0;
		}
	}

	void gain_treasure_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}

	bool gain_treasure_mgr::can_have_item(int32 npc_level)
	{
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		auto map_type = base_map_system::get_map_type(m_player_ptr->get_map_ent());

		if (false == init_unit::is_map_gain_treasure(map_type))
		{
			return true;
		}

		if (true == is_finish_today_field_boss() && e_map_type_boss_home == map_type)
		{
			return false;
		}

		ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
		if (nullptr == act_common_config_ptr)
		{
			return false;
		}
		int32 max_level_gap = act_common_config_ptr->ParamInt3;
		int32 cur_player_level = m_player_ptr->get_owner_unit_level();
		int32 cur_level_gap = cur_player_level - npc_level;
		
		if (cur_level_gap > max_level_gap)
		{
			return false;
		}
		return true;
	}

	bool gain_treasure_mgr::is_finish_today_field_boss()
	{
		ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
		if (nullptr == act_common_config_ptr)
		{
			return false;
		}

		int32 max_count = act_common_config_ptr->ParamInt2;
		int32 cur_count = m_gain_treasure_info[e_gain_treasure_info_type_killed_field_boss];
		if (cur_count < max_count)
		{
			return false;
		}

		return true;
	}

	bool gain_treasure_mgr::can_entry_to_boss_single_map()
	{
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_single);
		if (nullptr == act_common_config_ptr)
		{
			return false;
		}
		//int32 vip_level = m_player_ptr->get_vip_level();
		//if (vip_level < act_common_config_ptr->ParamInt1)
		//{
		//	return false;
		//}
		int32 today_count = get_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss);
		if (today_count >= act_common_config_ptr->ParamInt3 + get_gain_treasure_info(e_gain_treasure_info_type_buy_simple_boss)|| today_count < 0)
		{
			return false;
		}
		return true;
		//int32 item_id, item_num, money_id, money_num;
		//if (act_common_config_ptr->ParamIntArr1.size() < 2 || act_common_config_ptr->ParamIntArr2.size() < 2)
		//{
		//	return false;
		//}
		//item_id = act_common_config_ptr->ParamIntArr1[0];
		//item_num = act_common_config_ptr->ParamIntArr1[1];
		//money_id = act_common_config_ptr->ParamIntArr2[0];
		//money_num = act_common_config_ptr->ParamIntArr2[1];
		//item_set& item_ref = m_player_ptr->get_item_set();
		//int32 boss_scroll_num = item_ref.get_item_count(item_id);
		//if (boss_scroll_num < item_num && !m_player_ptr->can_cut_money((e_money_type)money_id, money_num))
		//{
		//	return false;
		//}
		//return true;
	}

	void gain_treasure_mgr::load_gain_treasure_info_from_db(const s_gain_treasure_info& info_data)
	{
		for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
		{
			m_gain_treasure_info[i] = info_data.data_ary[i];
		}
		send_gain_treasure_info_all();
	}
	bool gain_treasure_mgr::load_role_gain_treasure_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_gain_treasure_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_gain_treasure_info db_data;

		for (int32 i = 0 ; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}
		

		load_gain_treasure_info_from_db(db_data);

		return true;
	}

	void gain_treasure_mgr::save_gain_treasure_info_to_db(int32 save_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_gain_treasure_info);
		if (is_use == false)
		{
			cs2dp_save_gain_treasure_info req;
			req.save_type_ex = save_type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
			{
				req.info_data.data_ary[i] = m_gain_treasure_info[i];
			}
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_gain_treasure msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(save_type);

			faith::cs2dp_proto::role_gain_treasure_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
			{
				db_data->add_data_ary(m_gain_treasure_info[i]);
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_gain_treasure_info);
		}
	}

	void gain_treasure_mgr::send_gain_treasure_info_all()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		
		game_proto_gain_treasure_info_all msg;
		for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
		{
			game_proto_gain_treasure_info_one* info_ptr = msg.add_info_arr();
			if (nullptr == info_ptr)
			{
				continue;
			}
			info_ptr->set_info_type(i);
			info_ptr->set_info_value(m_gain_treasure_info[i]);
		}
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_gain_treasure_info_update_all);
	}

	void gain_treasure_mgr::send_gain_treasure_info_one(int32 info_type)
	{
		if (info_type < e_gain_treasure_info_type_killed_field_boss || info_type >= e_gain_treasure_info_type_max || nullptr == m_player_ptr)
		{
			return;
		}

		game_proto_gain_treasure_info_one msg;
		msg.set_info_type(info_type);
		msg.set_info_value(m_gain_treasure_info[info_type]);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_gain_treasure_info_update_one);
	}

	void gain_treasure_mgr::set_gain_treasure_info(int32 info_type, int32 info_value)
	{
		if (info_type < e_gain_treasure_info_type_killed_field_boss || info_type >= e_gain_treasure_info_type_max)
		{
			return;
		}
		m_gain_treasure_info[info_type] = info_value;
	}

	int32 gain_treasure_mgr::get_gain_treasure_info(int32 info_type)
	{
		if (info_type < e_gain_treasure_info_type_killed_field_boss || info_type >= e_gain_treasure_info_type_max)
		{
			return -1;
		}
		return m_gain_treasure_info[info_type];
	}

	void gain_treasure_mgr::send_gain_treasure_record_to_ws(int32 boss_spawn_point_template_id, int32 killed_stamp, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr)
	{
		cs2ws_add_gain_treasure_record_info record_info_msg;
		//record_info_msg.record_info.boss_spawn_point_template_id = boss_spawn_point_template_id;
		//record_info_msg.record_info.killed_stamp = killed_stamp;
	
		//std::vector<string> role_name_arr;
		//for (int32 i = 0; i < player_info_arr.size();i++)
		//{
		//	std::string player_name = player_info_arr[i].role_name;
		//	if (role_name_arr.size()==0)
		//	{
		//		role_name_arr.push_back(player_name);
		//		continue;
		//	}
		//	bool is_have_name = false;
		//	for (int32 j = 0; j < role_name_arr.size();j++)
		//	{
		//		if (role_name_arr[j] == player_name)
		//		{
		//			is_have_name = true;
		//		}
		//	}
		//	if (false == is_have_name)
		//	{
		//		role_name_arr.push_back(player_name);
		//		continue;
		//	}
		//}
		//int32 player_num = role_name_arr.size();
		//if (0 == player_num)
		//{
		//	return;
		//}

		//std::vector<s_gain_treasure_record_player_info> player_info_arr_new;
		//s_gain_treasure_record_player_info null_player_info;
		//null_player_info.clear_data();

		//int32 list_num = player_info_arr.size();
		//for (int32 i = 0; i < player_num;i++)
		//{
		//	player_info_arr_new.push_back(null_player_info);
		//	memcpy(player_info_arr_new[i].role_name, role_name_arr[i].c_str(), role_name_arr[i].size() >= max_name_size ? max_name_size : role_name_arr[i].size());
		//}
		//for (int32 i = 0; i <list_num; i++)
		//{
		//	int32 item_idx = i / player_num;
		//	if (item_idx >= gain_treasure_gain_item_max_num)
		//	{
		//		continue;
		//	}
		//	player_info_arr_new[i % player_num].gain_item_template_id[item_idx] = player_info_arr[i].gain_item_template_id[0];
		//}
		//for (int32 i = 0; i < player_info_arr_new.size(); i++)
		//{
		//	if (i >= max_team_member_num)
		//	{
		//		break;
		//	}
		//	record_info_msg.record_info.player_info_arr[i] = player_info_arr_new[i];
		//}	
		int32 data_num = 0;
		record_info_msg.boss_spawn_point_temp_id = boss_spawn_point_template_id;
		record_info_msg.killed_stamp = killed_stamp;
		for (int32 i = 0; i < player_info_arr.size(); i++)
		{
			if (i >= gain_treasure_gain_item_max_num)
			{
				break;
			}
			record_info_msg.player_info[i] = player_info_arr[i];
			data_num++;
		}
		record_info_msg.data_num = data_num;
		connection_mgr::getInstance().send_to_ws( &record_info_msg, record_info_msg.get_pak_length());
	}

	void gain_treasure_mgr::send_gain_treasure_boss_info_to_ws(int32 boss_spawn_point_template_id, guid_64 boss_guid, int32 next_refresh_stamp, const std::vector<s_gain_treasure_player_info>& player_info_arr)
	{
		cs2ws_update_gain_treasure_boss_info boss_info_msg;
		boss_info_msg.boss_info.boss_spawn_point_template_id = boss_spawn_point_template_id;
		boss_info_msg.boss_info.boss_guid = boss_guid;
		boss_info_msg.boss_info.next_refresh_stamp = next_refresh_stamp;
		for (int32 i = 0; i < player_info_arr.size(); i++)
		{
			if (i >= max_team_member_num)
			{
				break;
			}
			boss_info_msg.boss_info.player_info[i] = player_info_arr[i];
		}
		connection_mgr::getInstance().send_to_ws( &boss_info_msg, sizeof(boss_info_msg));
	}

	void gain_treasure_mgr::buy_single_boss_num()
	{
		game_proto_buy_simple_boss_num_end msg;
		VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(m_player_ptr->get_vip_level(false));
		RegionTemplate* region_ptr = globle_data::get_instance().get_region_template_ptr();
		if (nullptr == vip_temp_ptr || nullptr == region_ptr || vip_temp_ptr->VIPGainTreasureCost.size() < 0 || vip_temp_ptr->VIPGainTreasureCost.size() % 5 != 0)
		{
			msg.set_error_index(e_buy_simple_boss_num_error_unkonwn);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_send_buy_simage_boss_num_end);
			return;
		}
		bool is_cur_region = false;
		int32 cost_num = 0;
		int32 money_type = 0;
		int32 money_num = 0;
		int32 money_coef = 0;
		for (int32 i = 0; i < vip_temp_ptr->VIPGainTreasureCost.size() / 5; ++i)
		{
			if (vip_temp_ptr->VIPGainTreasureCost[i * 5] == region_ptr->RegionCode)
			{
				is_cur_region = true;
				cost_num = vip_temp_ptr->VIPGainTreasureCost[i * 5 + 1];
				money_type = vip_temp_ptr->VIPGainTreasureCost[i * 5 + 2];
				money_num = vip_temp_ptr->VIPGainTreasureCost[i * 5 + 3];
				money_coef = vip_temp_ptr->VIPGainTreasureCost[i * 5 + 4];
				break;
			}
		}
		if (false == is_cur_region)
		{
			msg.set_error_index(e_buy_simple_boss_num_error_unkonwn);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_send_buy_simage_boss_num_end);
			return;
		}
		if (get_gain_treasure_info(e_gain_treasure_info_type_buy_simple_boss) >= cost_num)
		{
			msg.set_error_index(e_buy_simple_boss_num_error_lack_num);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_send_buy_simage_boss_num_end);
			return;
		}
		money_num += (money_coef * get_gain_treasure_info(e_gain_treasure_info_type_buy_simple_boss));
		if (!m_player_ptr->can_cut_money(e_money_type(money_type), money_num))
		{
			msg.set_error_index(e_buy_simple_boss_num_error_lack_money);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_send_buy_simage_boss_num_end);
			return;
		}
		m_player_ptr->cut_money(e_money_type(money_type), money_num, e_server_log_cut_money_buy_single_boss);
		set_gain_treasure_info(e_gain_treasure_info_type_buy_simple_boss, get_gain_treasure_info(e_gain_treasure_info_type_buy_simple_boss) + 1);
		send_gain_treasure_info_one(e_gain_treasure_info_type_buy_simple_boss);
		m_player_ptr->get_star_trip_mgr().target_mission(e_star_trip_target_type_buy_single_boss);
		msg.set_error_index(e_buy_simple_boss_num_error_succeed);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_send_buy_simage_boss_num_end);
	}

	int32 gain_treasure_mgr::get_map_rank()
	{
		return m_map_rank;
	}

	void gain_treasure_mgr::set_map_rank(int32 _rank)
	{
		m_map_rank = _rank;
	}

	void gain_treasure_mgr::refresh_gain_treasure_info()
	{
		for (int32 i = 0; i < e_gain_treasure_info_type_max; i++)
		{
			set_gain_treasure_info(i, 0);
		}
		send_gain_treasure_info_all();
	}

}
