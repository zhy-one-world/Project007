/********************************************************************
  created: 2019年4月4日
  file base: cross_server_harry_ws_mgr
  file ext: cpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#include "cross_server_harry_ws_mgr.h"
#include "cross_server_pk_ws_mgr.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "cross_server_harry_msg.hpp"
#include "big_player_msg.hpp"
#include "ws_client.hpp"
#include "Logic/time_def.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "world_server.hpp"
#include "../mail/mail_event_ws.h"
#include "../ranking/ranking_mgr_ws.h"
#include "../mail/event_ws_mgr.h"
#include "utility/guid_gen.h"
#include "cross_server_pk_msg.hpp"
#include "server_log.hpp"
#include "globle_data.h"
#include "system/scene/cs_map_mgr_system.h"
#include "base/ecs_world.h"
#include "game.pb.h"
#include "net.pb.h"

namespace faith
{
	cross_server_harry_ws_mgr::cross_server_harry_ws_mgr()
	{
		clear_data();
	}

	void cross_server_harry_ws_mgr::clear_data()
	{
		for (int32 i = 0; i < max_server_num; i++)
		{
			m_server_harry_msg_arr[i].clear_data();
		}
		m_timer = 0;
		m_last_10sec_tick_time = 0;
		m_empty_server_harry_msg.clear_data();
		m_is_set_special_server_money = false;
		m_need_set_next_special_money_time = 0;
		m_need_set_special_server_idex_arr.clear();
		m_is_have_change = false;
		for (int32 i = 0; i < 3; i++)
		{
			m_player_info_array[i].clear();
		}
		m_is_need_init_server_money = false;
		m_is_load_all_harry_msg = false;
		m_can_load_fake_player = false;
		m_is_recv_gate_server_msg = false;
		m_is_load_harry_player = false;
		m_server_change_special_money_num = 0;
		m_last_tick_time = 0;
		m_last_sec_tick_time = 0;
	}

	void cross_server_harry_ws_mgr::init_manager()
	{
		send_load_db_server_harry_all_msg();
		m_can_load_fake_player = true;
	}

	void cross_server_harry_ws_mgr::tick(int64 time_new)
	{
		m_timer += time_new;
		if (true == world_server::getInstance().is_sky_island_server() && false == m_is_load_all_harry_msg)
		{
			return;
		}
		if (m_timer - m_last_10sec_tick_time > second_tick_time * 10)
		{
			m_last_10sec_tick_time = m_timer;
			tick_10sec(time_new);
		}
		if (m_timer - m_last_sec_tick_time > second_tick_time)
		{
			m_last_sec_tick_time = m_timer;
			tick_sec(time_new);
		}
	}

	void cross_server_harry_ws_mgr::init_harry_map()
	{
		auto map_ent = cs_map_mgr_system::init_map_by_battle(harry_map_id);
		if (map_ent)
		{
			m_harry_map_guid = map_ent->getEntityId();
		}
	}

	void cross_server_harry_ws_mgr::get_valid_harry_map(guid_64& map_guid, int32& war_idex)
	{
		map_guid = guid_64();
		auto map_ent = get_entity(m_harry_map_guid);
		if (map_ent)
		{
			map_guid = map_ent->getEntityId();
			war_idex = 1;
		}
	}

	void cross_server_harry_ws_mgr::recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id)
	{
		e_error_code ret = e_error_code_success;
		int32 error_ret = ret;
		int32 map_temp_id = harry_map_id;
		guid_64 map_guid = guid_64();
		int32 war_idx = 0;
		get_valid_harry_map(map_guid, war_idx);
		if (false == map_guid.is_valid())
		{
			init_harry_map();
			get_valid_harry_map(map_guid, war_idx);
		}
		if (false == map_guid.is_valid())
		{
			error_ret = e_error_code_map_init_map_err;
		}
		ws2ws_transfer_cross_pk_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idx;
		msg.activity_type = (int32)e_activity_type_cross_server_harry;
		msg.server_id = world_server::getInstance().get_server_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_map_island_state_msg, &msg, sizeof(msg));
	}


	void cross_server_harry_ws_mgr::tick_10sec(int64 time_new)
	{
		if (m_is_have_change && world_server::getInstance().is_sky_island_server())//同步信息
		{
			send_all_harry_msg_to_other_server();
			send_harry_msg_to_all_client();
			m_is_have_change = false;
			save_to_db_server_harry_all_msg();
		}
		if (!world_server::getInstance().is_sky_island_server())
		{
			send_cur_server_change_harry_msg_to_gate_server();
		}
	}

	void cross_server_harry_ws_mgr::tick_sec(int64 time_new)
	{
		if (m_is_need_init_server_money == true && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry))
		{
			init_server_money();
			m_is_need_init_server_money = false;
			send_all_harry_msg_to_other_server();
		}

		if (is_in_harry_special_game_time() && !m_is_set_special_server_money)
		{
			m_is_set_special_server_money = true;
			make_special_money_with_special_time();
			send_all_harry_msg_to_other_server();
		}
		else if (m_need_set_next_special_money_time <= time_helper::get_cur_time_new().second
			&& m_need_set_next_special_money_time > 0
			&& is_in_harry_special_game_time())
		{
			clear_all_server_special_money();
			make_special_money_for_other_server();
			send_all_harry_msg_to_other_server();
		}
		else if (!is_in_harry_special_game_time() && m_is_set_special_server_money)
		{
			m_is_set_special_server_money = false;
			clear_all_server_special_money();
			send_all_harry_msg_to_other_server();
		}
		if (m_need_create_player)
		{
			create_harry_player(e_activity_type_cross_server_harry);
		}
		if (m_is_recv_gate_server_msg && m_can_load_fake_player && !m_is_load_harry_player)
		{
			load_fake_data(e_activity_type_cross_server_harry);
		}
	}

	void cross_server_harry_ws_mgr::save_to_db_server_harry_all_msg()
	{
		if (false == world_server::getInstance().is_sky_island_server())
		{
			return;
		}
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == m_server_harry_msg_arr[i].is_valid())
			{
				continue;
			}
			server2dp_proto_ws2dp_save_cross_server_harry_msg_all harry_msg;
			harry_msg.set_server_id(m_server_harry_msg_arr[i].server_id);
			harry_msg.set_server_money(m_server_harry_msg_arr[i].server_money);
			harry_msg.set_special_money(m_server_harry_msg_arr[i].special_money);
			harry_msg.set_first_player_guid(m_server_harry_msg_arr[i].first_player_guid.server_64);
			harry_msg.set_second_player_guid(m_server_harry_msg_arr[i].second_player_guid.server_64);
			harry_msg.set_third_player_guid(m_server_harry_msg_arr[i].third_player_guid.server_64);
			harry_msg.set_first_player_name(m_server_harry_msg_arr[i].first_player_name);
			ws_client::getInstance().send_to_dp_new(&harry_msg, e_msg_ws2dp_save_cross_server_harry_info);
			//ws2dp_save_cross_server_harry_msg_all harry_msg;
			//harry_msg.server_harry_msg = m_server_harry_msg_arr[i];
			//ws_client::getInstance().send_to_dp(&harry_msg, sizeof(harry_msg));
		}
	}

	void cross_server_harry_ws_mgr::send_load_db_server_harry_all_msg()
	{
		server2dp_proto_ws2dp_load_cross_server_harry_msg_all harry_msg;
		ws_client::getInstance().send_to_dp_new(&harry_msg, e_msg_ws2dp_save_load_cross_server_harry_info);

		//ws2dp_load_cross_server_harry_msg_all msg;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void cross_server_harry_ws_mgr::send_del_one_server_msg_to_dp(int32 server_id)
	{
		if (false == world_server::getInstance().is_sky_island_server())
		{
			return;
		}
		server2dp_proto_ws2dp_del_one_server_msg harry_msg;
		harry_msg.set_server_id(server_id);
		ws_client::getInstance().send_to_dp_new(&harry_msg, e_msg_ws2dp_save_send_del_one_server_msg);

		//ws2dp_del_one_server_msg msg;
		//msg.server_id = server_id;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	bool cross_server_harry_ws_mgr::is_have_this_server_id(int32 server_id)
	{
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].server_id == server_id)
			{
				return true;
			}
		}
		return false;
	}

	void cross_server_harry_ws_mgr::refresh_server_harry_with_server_list()
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry) || false == m_is_load_all_harry_msg || false == world_server::getInstance().is_sky_island_server())
		{
			return;
		}
		game_info_map cur_server_list = world_server::getInstance().get_server_list();
		for (game_info_map_it it = cur_server_list.begin(); it != cur_server_list.end(); ++it)
		{
			if (false == is_have_this_server_id(it->second.server_id))
			{
				func_when_add_other_server(it->second.server_id);
			}		
		}

		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == world_server::getInstance().is_have_this_server(m_server_harry_msg_arr[i].server_id))
			{
				func_when_remove_other_server(m_server_harry_msg_arr[i].server_id);
			}
		}

	}

	void cross_server_harry_ws_mgr::func_when_add_other_server(int32 server_id)
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry) || false == m_is_load_all_harry_msg  || server_id == world_server::getInstance().get_cross_id())
		{
			return;
		}
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].server_id == server_id)
			{
				return;
			}
		}
		int32 empty_idex = -1;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (!m_server_harry_msg_arr[i].is_valid())
			{
				empty_idex = i;
				break;
			}
		}
		if (empty_idex < 0)
		{
			return;
		}
		bool is_have_db_msg = false;
		for (int32 i = 0; i < dp_save_max_server_num; i++)
		{
			if (m_dp_harry_msg_arr[i].server_id == server_id)
			{
				m_server_harry_msg_arr[empty_idex] = m_dp_harry_msg_arr[i];
				is_have_db_msg = true;
				break;
			}
		}
		if (!is_have_db_msg)
		{
			init_one_msg_by_server_id(server_id);
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::func_when_remove_other_server(int32 server_id)
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry) || false == m_is_load_all_harry_msg)
		{
			return;
		}
		//send_del_one_server_msg_to_dp(server_id);
		s_server_harry_msg cur_server_msg;
		cur_server_msg.clear_data();
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].server_id == server_id)
			{
				cur_server_msg = m_server_harry_msg_arr[i];
				break;
			}
		}
		if (!cur_server_msg.is_valid())
		{
			return;
		}
		bool is_in_dp_msg = false;
		for (int32 i = 0; i < dp_save_max_server_num; i++)
		{
			if (m_dp_harry_msg_arr[i].server_id == server_id)
			{
				m_dp_harry_msg_arr[i] = cur_server_msg;
				is_in_dp_msg = true;
				break;
			}
		}
		if (!is_in_dp_msg)
		{
			for (int32 i = 0; i < dp_save_max_server_num; i++)
			{
				if (!m_dp_harry_msg_arr[i].is_valid())
				{
					m_dp_harry_msg_arr[i] = cur_server_msg;
					break;
				}
			}
		}
		server2dp_proto_ws2dp_save_cross_server_harry_msg_all harry_msg;
		harry_msg.set_server_id(cur_server_msg.server_id);
		harry_msg.set_server_money(cur_server_msg.server_money);
		harry_msg.set_special_money(cur_server_msg.special_money);
		harry_msg.set_first_player_guid(cur_server_msg.first_player_guid.server_64);
		harry_msg.set_second_player_guid(cur_server_msg.second_player_guid.server_64);
		harry_msg.set_third_player_guid(cur_server_msg.third_player_guid.server_64);
		harry_msg.set_first_player_name(cur_server_msg.first_player_name);
		ws_client::getInstance().send_to_dp_new(&harry_msg, e_msg_ws2dp_save_cross_server_harry_info);

		//ws2dp_save_cross_server_harry_msg_all harry_msg;
		//harry_msg.server_harry_msg = cur_server_msg;
		//ws_client::getInstance().send_to_dp(&harry_msg, sizeof(harry_msg));

		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].server_id == server_id)
			{
				m_server_harry_msg_arr[i].clear_data();
				break;
			}
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::init_one_msg_by_server_id(int32 server_id)
	{
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return;
		}
		std::vector<int32> init_money_arr = act_cfg_ptr->ParamIntArr1;
		if (init_money_arr.size() < e_server_money_max)
		{
			return;
		}
		int32 init_money = init_money_arr[e_server_money_init];
		int32 empty_idex = -1;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (!m_server_harry_msg_arr[i].is_valid())
			{
				empty_idex = i;
				break;
			}
		}
		if (empty_idex < 0)
		{
			return;
		}
		m_server_harry_msg_arr[empty_idex].server_id = server_id;
		m_server_harry_msg_arr[empty_idex].server_money = init_money;
		m_server_harry_msg_arr[empty_idex].special_money = 0;
	}

	void cross_server_harry_ws_mgr::recv_load_all_server_harry_msg(bool is_empty, int32 data_num, const s_server_harry_msg* server_harry_msg_arr)
	{
		m_is_load_all_harry_msg = true;
		if (is_empty)
		{
			m_is_need_init_server_money = true;
			return;
		}
		else
		{
			std::set<int32> server_id_arr;
			world_server::getInstance().get_server_id_arr(server_id_arr);
			if (data_num > dp_save_max_server_num || nullptr == server_harry_msg_arr)
			{
				return;
			}

			int32 empty_idex = 0;
			for (int32 i = 0; i < data_num; ++i)//先查表里的数据是否为cfg中存在的
			{
				if (server_harry_msg_arr[i].server_id <= 0 || server_harry_msg_arr[i].server_id == world_server::getInstance().get_cross_id())
				{
					continue;
				}
				bool is_have_id = false;
				for (std::set<int32>::iterator it = server_id_arr.begin(); it != server_id_arr.end(); ++it)
				{
					if (*it == server_harry_msg_arr[i].server_id)
					{
						is_have_id = true;
						break;
					}
				}
				if (empty_idex >= max_server_num)
				{
					break;
				}
				if (!is_have_id)
				{
					continue;
				}
				m_server_harry_msg_arr[empty_idex] = server_harry_msg_arr[i];
				empty_idex++;
			}
			for (std::set<int32>::iterator it = server_id_arr.begin(); it != server_id_arr.end(); ++it)//再补没有的
			{
				bool is_have_server = false;
				for (int32 j = 0; j < max_server_num; j++)
				{
					if (*it == m_server_harry_msg_arr[j].server_id)
					{
						is_have_server = true;
						break;
					}
				}
				if (is_have_server)
				{
					continue;
				}
				if (*it == world_server::getInstance().get_cross_id())
				{
					continue;
				}
				init_one_msg_by_server_id(*it);
			}
			for (int32 i = 0; i < data_num; i++)
			{
				m_dp_harry_msg_arr[i] = server_harry_msg_arr[i];
			}
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::send_all_harry_msg_to_other_server(int32 server_id, bool is_need_set_name)
	{
		if (false == world_server::getInstance().is_sky_island_server() || false == gate_proxy::getInstance().is_gate_run() || false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry))
		{
			return;
		}
		int32 data_num = 0;
		ws2ws_send_cross_server_harry_msg_all all_harry_msg;
		all_harry_msg.is_need_set_name = is_need_set_name;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == m_server_harry_msg_arr[i].is_valid())
			{
				continue;
			}
			all_harry_msg.harry_msg[i] = m_server_harry_msg_arr[i];
			data_num++;
		}
		all_harry_msg.data_num = data_num;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_send_cross_server_harry_info, &all_harry_msg, sizeof(all_harry_msg));
	}

	void cross_server_harry_ws_mgr::recv_gate_harry_msg(const s_server_harry_msg* server_harry_msg_arr, int32 data_num, bool is_need_set_name)
	{
		if (nullptr == server_harry_msg_arr || data_num <= 0 || data_num > max_server_num)
		{
			return;
		}
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (!is_need_set_name && server_harry_msg_arr[i].server_id == world_server::getInstance().get_server_id())
			{
				m_server_harry_msg_arr[i].server_money = server_harry_msg_arr[i].server_money;
				m_server_harry_msg_arr[i].server_id = server_harry_msg_arr[i].server_id;
				m_server_harry_msg_arr[i].special_money = server_harry_msg_arr[i].special_money;
			}
			else
			{
				m_server_harry_msg_arr[i] = server_harry_msg_arr[i];
			}
		}
		send_harry_msg_to_all_client();
		m_is_recv_gate_server_msg = true;
		send_server_harry_msg_to_cs(0);
	}

	void cross_server_harry_ws_mgr::clear_change_value()
	{
		m_server_change_special_money_num = 0;
		m_server_change_harry_msg.clear();
	}

	void cross_server_harry_ws_mgr::send_cur_server_change_harry_msg_to_gate_server()
	{
		if (m_server_change_harry_msg.size() <= 0 && m_server_change_special_money_num == 0)
		{
			return;//没有改变，则不发送
		}
		int32 data_num = 0;
		bool have_cur_server_special_money_num = false;
		ws2ws_send_cross_server_harry_msg_one one_msg;
		std::map<int32, int32>::iterator ite;
		for (ite = m_server_change_harry_msg.begin(); ite != m_server_change_harry_msg.end(); )
		{
			std::map<int32, int32>::iterator temp_ite = ite++;
			int32 server_id = temp_ite->first;
			int32 value = temp_ite->second;
			if (server_id <= 0 || value == 0)
			{
				continue;
			}
			if (data_num >= max_server_num)
			{
				break;
			}
			one_msg.harry_msg[data_num].server_id = server_id;
			one_msg.harry_msg[data_num].change_money = value;
			if (server_id == world_server::getInstance().get_server_id())
			{
				one_msg.harry_msg[data_num].change_special_money = m_server_change_special_money_num;
				have_cur_server_special_money_num = true;
			}
			data_num++;
		}
		if (m_server_change_special_money_num != 0 && !have_cur_server_special_money_num && data_num < max_server_num)
		{
			one_msg.harry_msg[data_num].server_id = world_server::getInstance().get_server_id();
			one_msg.harry_msg[data_num].change_special_money = m_server_change_special_money_num;
			data_num++;
		}
		one_msg.data_num = data_num;
		int32 gate_server_id = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), gate_server_id, e_msgindex_ws2ws_send_cross_server_harry_one, &one_msg, sizeof(one_msg));
		clear_change_value();
	}

	void cross_server_harry_ws_mgr::recv_server_harry_msg(const s_harry_money_change* server_harry_msg, int32 data_num)
	{
		if (nullptr == server_harry_msg)
		{
			return;
		}
		for (int32 i = 0; i <= data_num; i++)
		{
			if (i >= max_server_num)
			{
				break;
			}
			int32 server_id = server_harry_msg[i].server_id;
			int32 change_normal_value = server_harry_msg[i].change_money;
			int32 change_special_value = server_harry_msg[i].change_special_money;
			s_server_harry_msg& server_harry_msg = get_server_money_by_server_id(server_id);
			if (false == server_harry_msg.is_valid())
			{
				continue;
			}
			server_harry_msg.server_money += change_normal_value;
			server_harry_msg.special_money += change_special_value;
			if (server_harry_msg.special_money < 0)//当gate同步信息时，有几率导致高倍采集正好清零，此时发来的同步数据会导致特殊采集次数小于0
			{
				server_harry_msg.special_money = 0;
			}
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::change_server_money(bool is_special, int32 change_num, int32 server_id)
	{
		s_server_harry_msg& cur_msg = get_cur_server_money();
		if (!cur_msg.is_valid())
		{
			return;
		}
		if (is_special && world_server::getInstance().get_server_id() == server_id)
		{
			m_server_change_special_money_num += change_num;
			cur_msg.special_money += change_num;
			m_server_change_harry_msg[server_id] += change_num;
			if (world_server::getInstance().get_server_id() == server_id)
			{
				cur_msg.server_money += change_num;
			}
		}
		if (!is_special)
		{
			m_server_change_harry_msg[server_id] += change_num;
			if (world_server::getInstance().get_server_id() == server_id)
			{
				cur_msg.server_money += change_num;
			}
		}
	}

	s_server_harry_msg& cross_server_harry_ws_mgr::get_server_money_by_server_id(int32 server_id)
	{
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == m_server_harry_msg_arr[i].is_valid())
			{
				continue;
			}
			if (m_server_harry_msg_arr[i].server_id == server_id)
			{
				return m_server_harry_msg_arr[i];
			}
		}
		return m_empty_server_harry_msg;
	}

	s_server_harry_msg& cross_server_harry_ws_mgr::get_cur_server_money()
	{
		int32 cur_server_id = world_server::getInstance().get_server_id();
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == m_server_harry_msg_arr[i].is_valid())
			{
				continue;
			}
			if (m_server_harry_msg_arr[i].server_id == cur_server_id)
			{
				return m_server_harry_msg_arr[i];
			}
		}
		return m_empty_server_harry_msg;
	}

	void cross_server_harry_ws_mgr::send_harry_msg_to_all_client()
	{
		if (false == gate_proxy::getInstance().is_gate_run() || false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross))
		{
			return;
		}
		game_proto_cross_server_harry_all_msg all_harry_msg;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == m_server_harry_msg_arr[i].is_valid())
			{
				continue;
			}
			game_proto_cross_server_harry_one_msg* one_harry_msg = all_harry_msg.add_all_harry_msg();
			fill_harry_one_msg(*one_harry_msg, m_server_harry_msg_arr[i]);
		}
		client_session_mgr::getInstance().send_message_to_all_client(&all_harry_msg, e_msgindex_s2c_cross_server_harry_msg_info_all);
	}

	void cross_server_harry_ws_mgr::send_harry_msg_to_one_client(client_session * session_ptr)
	{
		if (nullptr == session_ptr)
		{
			return;
		}
		if (false == gate_proxy::getInstance().is_gate_run() || false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross))
		{
			return;
		}
		game_proto_cross_server_harry_all_msg all_harry_msg;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (false == m_server_harry_msg_arr[i].is_valid())
			{
				continue;
			}
			game_proto_cross_server_harry_one_msg* one_harry_msg = all_harry_msg.add_all_harry_msg();
			fill_harry_one_msg(*one_harry_msg, m_server_harry_msg_arr[i]);
		}
		session_ptr->send_to_client(&all_harry_msg, e_msgindex_s2c_cross_server_harry_msg_info_all);
	}

	void cross_server_harry_ws_mgr::fill_harry_one_msg(game_proto_cross_server_harry_one_msg & one_msg, const s_server_harry_msg & harry_msg)
	{
		one_msg.set_server_id(harry_msg.server_id);
		one_msg.set_server_money(harry_msg.server_money);
		one_msg.set_special_money(harry_msg.special_money);
		one_msg.set_first_player_guida(harry_msg.first_player_guid.A);
		one_msg.set_first_player_guidb(harry_msg.first_player_guid.B);
		one_msg.set_second_player_guida(harry_msg.second_player_guid.A);
		one_msg.set_second_player_guidb(harry_msg.second_player_guid.B);
		one_msg.set_third_player_guida(harry_msg.third_player_guid.A);
		one_msg.set_third_player_guidb(harry_msg.third_player_guid.B);
		one_msg.set_first_player_name(harry_msg.first_player_name);
	}

	void cross_server_harry_ws_mgr::init_server_money_with_refresh()
	{
		if (false == world_server::getInstance().is_sky_island_server() || false == gate_proxy::getInstance().is_gate_run())
		{
			return;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return;
		}
		std::vector<int32> init_money_arr = act_cfg_ptr->ParamIntArr1;
		if (init_money_arr.size() < e_server_money_max)
		{
			return;
		}
		int32 init_money = init_money_arr[e_server_money_init];
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].is_valid())
			{
				m_server_harry_msg_arr[i].server_money = init_money;
				m_server_harry_msg_arr[i].special_money = 0;
			}
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::init_server_money()
	{
		if (false == world_server::getInstance().is_sky_island_server() || false == gate_proxy::getInstance().is_gate_run())
		{
			return;
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return;
		}
		std::vector<int32> init_money_arr = act_cfg_ptr->ParamIntArr1;
		if (init_money_arr.size() < e_server_money_max)
		{
			return;
		}
		int32 init_money = init_money_arr[e_server_money_init];

		std::set<int32> server_id_arr;
		world_server::getInstance().get_server_id_arr(server_id_arr);
		int32 i = 0;
		for (std::set<int32>::iterator it = server_id_arr.begin(); it != server_id_arr.end(); ++it)
		{
			if (*it == world_server::getInstance().get_cross_id())
			{
				continue;
			}
			m_server_harry_msg_arr[i].server_id = *it;
			m_server_harry_msg_arr[i].server_money = init_money;
			m_server_harry_msg_arr[i].special_money = 0;
			i++;
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::send_server_harry_msg_to_cs(int32 conn_index)
	{
		ws2cs_send_cur_server_harry_msg harry_msg;	
		memcpy(harry_msg.server_harry_msg, m_server_harry_msg_arr, sizeof(harry_msg.server_harry_msg));
		bool is_use = proto_by_lua(e_msgindex_ws2cs_send_cur_server_harry_info);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&harry_msg, sizeof(harry_msg), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::send_cur_server_harry_msg pro_msg;
			harry_msg.to_proto(pro_msg);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_send_cur_server_harry_info, e_server_type_cs);
		}
	}

	bool cross_server_harry_ws_mgr::is_in_harry_game_time()
	{
		return world_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_harry, e_activity_time_get_all) > 0;
	}

	bool cross_server_harry_ws_mgr::is_in_harry_special_game_time()
	{
		return world_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_harry, e_activity_time_get_all, true) > 0;
	}

	void cross_server_harry_ws_mgr::make_special_money_with_special_time()
	{
		if (false == world_server::getInstance().is_sky_island_server())
		{
			return;//只有gate服的ws处理这件事情
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return;
		}
		std::vector<int32> init_money_arr = act_cfg_ptr->ParamIntArr1;
		if (init_money_arr.size() < e_server_money_max)
		{
			return;
		}
		int32 special_init_count = init_money_arr[e_server_money_special];
		if (special_init_count <= 0)
		{
			return;
		}
		m_need_set_next_special_money_time = 0;
		m_need_set_special_server_idex_arr.clear();
		std::vector<int32> server_idex_arr;
		server_idex_arr.clear();
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].is_valid())
			{
				server_idex_arr.push_back(i);
			}
		}
		if (server_idex_arr.size() <= 0)
		{
			return;
		}
		random_array(server_idex_arr);
		int32 idex_len = server_idex_arr.size();
		int32 half_idex_len = idex_len / 2;
		for (int32 i = 0; i < half_idex_len; i++)
		{
			if (server_idex_arr[i] >= max_server_num)
			{
				continue;
			}
			if (m_server_harry_msg_arr[server_idex_arr[i]].is_valid())
			{
				m_server_harry_msg_arr[server_idex_arr[i]].special_money = special_init_count;
			}
		}
		for (int32 i = half_idex_len; i < idex_len; i++)
		{
			m_need_set_special_server_idex_arr.push_back(server_idex_arr[i]);
		}
		m_need_set_next_special_money_time = time_helper::get_cur_time_new().second + special_money_time;
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::clear_all_server_special_money()
	{
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].is_valid())
			{
				m_server_harry_msg_arr[i].special_money = 0;
			}
		}
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::make_special_money_for_other_server()
	{
		if (false == world_server::getInstance().is_sky_island_server())
		{
			return;//只有gate服的ws处理这件事情
		}
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return;
		}
		std::vector<int32> init_money_arr = act_cfg_ptr->ParamIntArr1;
		if (init_money_arr.size() < e_server_money_max)
		{
			return;
		}
		int32 special_init_count = init_money_arr[e_server_money_special];
		if (special_init_count <= 0)
		{
			return;
		}
		if (m_need_set_special_server_idex_arr.size() <= 0)
		{
			return;
		}
		for (int32 i = 0; i < m_need_set_special_server_idex_arr.size(); i++)
		{
			if (m_need_set_special_server_idex_arr[i] >= max_server_num)
			{
				continue;
			}
			if (m_server_harry_msg_arr[m_need_set_special_server_idex_arr[i]].is_valid())
			{
				m_server_harry_msg_arr[m_need_set_special_server_idex_arr[i]].special_money = special_init_count;
			}
		}
		m_need_set_next_special_money_time = 0;
		m_is_have_change = true;
	}

	void cross_server_harry_ws_mgr::set_server_harry_best_player(int32 server_id, int64 first_player_guid, int64 second_player_guid, int64 third_player_guid, const xchar * player_name)
	{
		s_server_harry_msg& server_harry_msg = get_server_money_by_server_id(server_id);
		if (nullptr == player_name || strlen(player_name) == 0)
		{
			server_harry_msg.clear_role_name();
		}
		else
		{
			server_harry_msg.set_role_name(player_name);
		}
		server_harry_msg.first_player_guid = first_player_guid;
		server_harry_msg.second_player_guid = second_player_guid;
		server_harry_msg.third_player_guid = third_player_guid;


		if (world_server::getInstance().is_sky_island_server())
		{
			m_is_have_change = true;
			return;//GATE服不需要继续执行
		}
		if (server_id == world_server::getInstance().get_server_id())
		{
			load_fake_data(e_activity_type_cross_server_harry);
			m_need_create_player = true;
		}
	}

	void cross_server_harry_ws_mgr::send_server_harry_best_player_msg_to_gate()
	{
		if (world_server::getInstance().is_sky_island_server())
		{
			return;
		}
		const s_server_harry_msg& server_harry_msg = get_cur_server_money();
		ws2ws_send_server_harry_player_msg msg;

		msg.player_guid_arr[0] = server_harry_msg.first_player_guid.server_64;
		msg.player_guid_arr[1] = server_harry_msg.second_player_guid.server_64;
		msg.player_guid_arr[2] = server_harry_msg.third_player_guid.server_64;
		
		memcpy(msg.player_name, server_harry_msg.first_player_name, max_name_size);

		msg.server_id = server_harry_msg.server_id;
		cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2ws_send_cross_server_harry_player_info, &msg, sizeof(msg));
	}

	bool cross_server_harry_ws_mgr::is_can_join_harry_map(client_session* session_ptr)
	{
		if (nullptr == session_ptr)
		{
			return false;
		}
		if (!session_ptr->is_in_game())
		{
			return false;
		}
		if (false == is_in_harry_game_time())
		{
			session_ptr->send_notice("90201806");
			return false;
		}
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, harry_map_id);
		if (nullptr == map_ptr)
		{
			session_ptr->send_notice("90090448");
			return false;
		}
		if (map_ptr->EnterLevel > session_ptr->get_role_info_data(e_role_info_exp_level))
		{
			session_ptr->send_notice("90090448");
			return false;
		}
		return true;
	}

	void cross_server_harry_ws_mgr::send_award_with_player_defend_value()
	{
		if (world_server::getInstance().is_sky_island_server())
		{
			return;
		}
		ranking_list* cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_cross_server_plinder_guard_value);
		if (nullptr == cur_ranking_list_ptr)
		{
			return;
		}
		int32 player_rank = 1;
		ranking_list_ite cur_list_ite = cur_ranking_list_ptr->begin();
		for (int32 i = 0; i < 50; i++)//暂时定只发前×个
		{
			if (cur_list_ite == cur_ranking_list_ptr->end())
			{
				break;
			}
			give_award_with_player_defend_value(cur_list_ite->role_guid, player_rank, cur_list_ite->role_name);
			cur_list_ite++;
			player_rank++;
		}
	}

	void cross_server_harry_ws_mgr::give_award_with_player_defend_value(guid_64 player_guid, int32 player_rank, const xchar* player_name)
	{
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		std::vector<int32>& rwd_data = act_com_cfg->ParamIntArr3;
		globle_data::get_instance().get_item_list_by_rank_rwd(player_rank, rwd_data, drop_item_list, ranking_mgr_ws::get_world_level_last());

		if (drop_item_list.size() <= 0)
		{
			return;
		}
		if (nullptr == player_name)
		{
			return;
		}
		std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_harry_player_award_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_harry_player_award_context));
		content_params.push_back(init_unit::change_i32_to_string(player_rank));
		std::string contenttext = init_unit::implode(content_params);

		mail_ws_mgr::get_instance().send_mail_system(player_guid, 0, drop_item_list, title, contenttext, 0, 0, 0, 0);
	}

	void cross_server_harry_ws_mgr::send_award_with_server_money_rank()
	{
		if (!world_server::getInstance().is_sky_island_server())
		{
			return;//只有gate需要运行该结算函数
		}
		std::vector<s_server_harry_msg> server_harry_arr;
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg_arr[i].is_valid())
			{
				server_harry_arr.push_back(m_server_harry_msg_arr[i]);
			}
		}
		sort(server_harry_arr.begin(), server_harry_arr.end());
		int32 rank_idex = 1;

		for (int32 i = 0; i < server_harry_arr.size(); i++)
		{
			if (!server_harry_arr[i].is_valid())
			{
				continue;
			}
			ws2ws_send_server_harry_award msg;
			msg.server_rank = rank_idex;
			cross::send_msg_to_ws(guid_64(), server_harry_arr[i].server_id, e_msgindex_ws2ws_send_server_harry_rank, &msg, sizeof(msg));
			rank_idex++;
		}
	}

	void cross_server_harry_ws_mgr::recv_gate_with_server_money_rank(int32 server_rank)
	{
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		std::vector<int32>& rwd_data = act_com_cfg->RankRewards;
		globle_data::get_instance().get_item_list_by_rank_rwd(server_rank, rwd_data, drop_item_list, ranking_mgr_ws::get_world_level_last());
		s_game_info* game_info_ptr = world_server::getInstance().get_server_config_by_server_id(world_server::getInstance().get_server_id());
		if (nullptr == game_info_ptr)
		{
			return;
		}
		if (drop_item_list.size() <= 0)
		{
			return;
		}

		int32 item_num = drop_item_list.size();

		std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_harry_server_award_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_harry_server_award_context));
		content_params.push_back(init_unit::change_i32_to_string(server_rank));
		std::string contenttext = init_unit::implode(content_params);

		s_item_info final_item_array[max_item_per_mail];
		s_mail_info final_mail_info;
		get_mail_data(final_mail_info, final_item_array, item_num, drop_item_list, title, contenttext);
		final_mail_info.data_ary[EMailInfo_IsNeedDelete] = time_helper::get_cur_time_new().second + day_time_second * 15;//15天删除
		event_ws_mgr::get_instance().add_globel_mail(final_mail_info, final_item_array, item_num);

	}
	void cross_server_harry_ws_mgr::get_mail_data(s_mail_info & mail_info, s_item_info * item_array, int32 & item_num, const std::vector<s_item_template_info>& item_list, const xstring & title, const xstring & content)
	{
		item_num = item_num > max_item_per_mail ? max_item_per_mail : item_num;
		mail_info.mail_guid = guid_gen::make_guid();
		int32 title_size = title.size();
		int32 content_size = content.size();
		if (title_size > max_mail_title_length)
		{
			title_size = max_mail_title_length;
		}
		if (content_size > max_text_contents_length)
		{
			content_size = max_text_contents_length;
		}
		memcpy(mail_info.mail_title, title.c_str(), title_size);
		memcpy(mail_info.text_contents, content.c_str(), content_size);
		mail_info.data_ary[EMailInfo_SendTime] = time_helper::get_cur_time_new().second;

		for (uint32 i = 0; i < item_num; ++i)
		{
			if (init_unit::init_item_data(item_array[i], item_list[i].m_item_id, 0, item_list[i].m_item_num, e_bag_type_mail, 1, item_list[i].m_lock))
			{
				mail_info.data_ary[EMailInfo_ItemGuid11 + i * 2] = item_array[i].item_guid.A;
				mail_info.data_ary[EMailInfo_ItemGuid11 + i * 2 + 1] = item_array[i].item_guid.B;
			}
		}
	}

	void cross_server_harry_ws_mgr::load_fake_data_with_fake_type(int32 fake_type)
	{
		if (fake_type<0 || fake_type>e_fake_type_max)
		{
			return;
		}
		const s_pk_player_msg& player_msg = cross_server_pk_ws_mgr::get_instance().get_pk_player_msg(fake_type - e_fake_type_best_barserker);
		guid_64 player_guid;
		int32 server_id;
		player_guid = player_msg.role_guid;
		server_id = player_msg.server_id;

		server2dp_proto_ws2dp_load_harry_player_detail msg;
		msg.set_server_id(server_id);
		msg.set_role_guid(player_guid.server_64);
		msg.set_player_idex(fake_type);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_load_load_harry_player_detail, server_id);
		//ws2dp_load_harry_player_detail msg;
		//msg.role_guid = player_guid;
		//msg.player_idex = fake_type;
		//msg.server_id = server_id;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), server_id);

	}

	void cross_server_harry_ws_mgr::load_fake_data(int32 active_type)
	{
		if (active_type == e_activity_type_cross_server_harry)
		{
			for (int32 i = e_fake_type_first_harry_player; i <= e_fake_type_third_harry_player; i++)
			{
				m_player_info_array[i].clear();
			}
			m_is_load_harry_player = true;
			const s_server_harry_msg& cur_harry_msg = get_cur_server_money();
			if (!cur_harry_msg.first_player_guid.is_valid())
			{
				m_is_load_end = true;
				return;
			}
			for (int32 i = 0; i <= e_fake_type_third_harry_player; i++)
			{
				guid_64 player_guid;
				switch (i)
				{
				case e_fake_type_first_harry_player:
					player_guid = cur_harry_msg.first_player_guid;
					break;
				case e_fake_type_second_harry_player:
					player_guid = cur_harry_msg.second_player_guid;
					break;
				case e_fake_type_third_harry_player:
					player_guid = cur_harry_msg.third_player_guid;
					break;
				default:
					break;
				}
				if (!player_guid.is_valid())
				{
					return;
				}
				m_player_info_array[i].guid = guid_gen::make_guid();
				m_player_info_array[i].mask_data_block(s_fake_player_info::efpi_spirit);
				//m_player_info_array[i].mask_data_block(s_fake_player_info::efpi_fight_att);
				m_player_info_array[i].mask_data_block(s_fake_player_info::efpi_skill);

				server2dp_proto_ws2dp_load_harry_player_detail msg;
				msg.set_role_guid(player_guid.server_64);
				msg.set_player_idex(i);
				ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_load_load_harry_player_detail);

				//ws2dp_load_harry_player_detail msg;
				//msg.role_guid = player_guid;
				//msg.player_idex = i;
				//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
			}
		}
		else if (active_type == e_activity_type_cross_server_pk)
		{
			for (int32 i = e_fake_type_best_barserker; i <= e_fake_type_best_assassinator; i++)
			{
				m_player_info_array[i].clear();
				cs_map_mgr_system::send_harry_player(big_world_map_id, m_player_info_array[i], i);
			}
			for (int32 i = e_fake_type_best_barserker; i <= e_fake_type_best_assassinator; i++)
			{
				const s_pk_player_msg& player_msg = cross_server_pk_ws_mgr::get_instance().get_pk_player_msg(i - e_fake_type_best_barserker);
				guid_64 player_guid;
				int32 server_id;
				player_guid = player_msg.role_guid;
				server_id = player_msg.server_id;
				if (!player_guid.is_valid())
				{
					cross_server_pk_ws_mgr::get_instance().set_load_all_fake_player(i - e_fake_type_best_barserker);
					continue;
				}
				m_player_info_array[i].guid = guid_gen::make_guid();
				m_player_info_array[i].mask_data_block(s_fake_player_info::efpi_spirit);
				m_player_info_array[i].mask_data_block(s_fake_player_info::efpi_fight_att);
				m_player_info_array[i].mask_data_block(s_fake_player_info::efpi_skill);

				server2dp_proto_ws2dp_load_harry_player_detail msg;
				msg.set_role_guid(player_guid.server_64);
				msg.set_player_idex(i);
				msg.set_server_id(server_id);
				ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_load_load_harry_player_detail, server_id);

				//ws2dp_load_harry_player_detail msg;
				//msg.role_guid = player_guid;
				//msg.player_idex = i;
				//msg.server_id = server_id;
				//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), server_id);
			}
		}
	}

	void cross_server_harry_ws_mgr::handle_base_detail_info(int32 big_type, const s_unit_info role_info)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		m_player_info_array[big_type].role_info = role_info;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_info);
		load_end_fake_player(big_type);
	}
	void cross_server_harry_ws_mgr::handle_item_info(int32 big_type, const s_item_info* item_info)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		int32 item_count = 0;
		for (int32 i = 0; i < role_show_item_num; ++i)
		{
			if (!item_info[i].item_guid.is_valid())
			{
				continue;
			}
			m_player_info_array[big_type].item_data[i] = item_info[i];
			item_count++;
		}
		m_player_info_array[big_type].item_num = item_count;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_item);
		load_end_fake_player(big_type);
	}
	void cross_server_harry_ws_mgr::handle_att_info(int32 big_type, const s_base_att_info& att_info)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		m_player_info_array[big_type].att_info = att_info;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_att);
		load_end_fake_player(big_type);
	}
	void cross_server_harry_ws_mgr::handle_fight_att_info(int32 big_type, const s_arena_char_fight_att& att_info)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		m_player_info_array[big_type].m_fight_att = att_info;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_fight_att);
		load_end_fake_player(big_type);
	}
	void cross_server_harry_ws_mgr::handle_skill_info(int32 big_type, const s_skill_info* skill_info)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		int32 skill_num = 0;
		for (int32 i = 0; i < MAX_SKILL_NUM; ++i)
		{
			m_player_info_array[big_type].skill_data[i] = skill_info[i];
			skill_num++;
		}
		m_player_info_array[big_type].skill_num = skill_num;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_skill);
		load_end_fake_player(big_type);
	}

	void cross_server_harry_ws_mgr::handle_buff_info(int32 big_type, const s_buff_info* buff_info)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		int32 buff_num = 0;
		for (int32 i = 0; i < MAN_UNIT_BUFF_INST; ++i)
		{
			m_player_info_array[big_type].buff_data[i] = buff_info[i];
			buff_num++;
		}
		m_player_info_array[big_type].buff_num = buff_num;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_buff);
		load_end_fake_player(big_type);
	}

	void cross_server_harry_ws_mgr::handle_belief_info(int32 big_type, const s_belief_info* belief_data)
	{
		if (big_type < 0 || big_type >= e_fake_type_max)
		{
			return;
		}
		int32 belief_num = 0;
		for (int32 i = 0; i < e_belief_series_type_max; ++i)
		{
			m_player_info_array[big_type].belief_data[i] = belief_data[i];
			belief_num++;
		}
		m_player_info_array[big_type].belief_num = belief_num;
		m_player_info_array[big_type].mask_data_block(s_fake_player_info::efpi_belief);
		load_end_fake_player(big_type);
	}

	void cross_server_harry_ws_mgr::load_end_fake_player(int32 idex)
	{
		if (idex >= e_fake_type_first_harry_player && idex <= e_fake_type_third_harry_player)
		{
			bool is_load_harry_end = true;
			for (int32 i = e_fake_type_first_harry_player; i <= e_fake_type_third_harry_player; i++)
			{
				if (!m_player_info_array[i].all_data_block_masked() && m_player_info_array[i].guid.is_valid())
				{
					is_load_harry_end = false;
				}
			}
			m_is_load_end = is_load_harry_end;
		}
		if (idex >= e_fake_type_best_barserker && idex <= e_fake_type_best_assassinator)
		{
			if (m_player_info_array[idex].all_data_block_masked() && m_player_info_array[idex].guid.is_valid())
			{
				cs_map_mgr_system::send_harry_player(big_world_map_id, m_player_info_array[idex], idex);
				cross_server_pk_ws_mgr::get_instance().set_load_all_fake_player(idex - e_fake_type_best_barserker);
			}		
		}
	}

	void cross_server_harry_ws_mgr::set_is_need_create_harry_player(bool need_create)
	{
		if (world_server::getInstance().is_sky_island_server() || 0 == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry))
		{
			return;
		}
		m_need_create_player = need_create;
	}

	void cross_server_harry_ws_mgr::create_harry_player(int32 active_type)
	{
		if (active_type == e_activity_type_cross_server_harry)
		{
			if (m_is_load_end || m_player_info_array[e_fake_type_first_harry_player].all_data_block_masked())
			{
				ws2cs_claer_all_harry_player clear_msg;
				clear_msg.need_clear = true;
				world_server::getInstance().broadcast(&clear_msg, sizeof(clear_msg), e_server_type_cs);

				m_need_create_player = false;
				int32 create_num = 0;
				for (int32 i = e_fake_type_first_harry_player; i <= e_fake_type_third_harry_player; i++)
				{
					if (!m_player_info_array[i].guid.is_valid())
					{
						create_num++;
					}
				}
				if (create_num > 0)
				{
					ws2cs_send_create_normal_player msg;
					msg.crate_num = create_num;
					bool is_use = proto_by_lua(e_msgindex_ws2cs_send_create_normal_player);
					if (is_use == false)
					{
						world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
					}
					else
					{
						faith::ws2cs_proto::send_create_normal_player pro_msg;
						msg.to_proto(pro_msg);
						world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_send_create_normal_player, e_server_type_cs);

					}
				}
				for (int32 i = e_fake_type_first_harry_player; i <= e_fake_type_third_harry_player; i++)
				{
					if (false == m_player_info_array[i].guid.is_valid() || false == m_player_info_array[i].all_data_block_masked())
					{
						break;
					}
					cs_map_mgr_system::send_harry_player(harry_map_id, m_player_info_array[i], i);
					CONSOLE_INFO("create_harry_player guid:{} rank:{} ", m_player_info_array[i].guid.server_64, i);
				}
			}
		}
	}
}
