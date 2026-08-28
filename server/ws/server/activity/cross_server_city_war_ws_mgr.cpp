/********************************************************************
  created: 2019年6月11日
  file base: cross_server_city_war_ws_mgr
  file ext: cpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#include "cross_server_city_war_ws_mgr.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "legion_msg.hpp"
#include "ws_client.hpp"
#include "Logic/time_def.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "world_server.hpp"
#include "../big_player_ws_mgr.h"
#include "../ranking/ranking_mgr_ws.h"
#include "server_log.hpp"
#include "../legion/legion_ws_mgr.h"
#include "../legion/legion_ws.h"
#include "../mail/event_ws_mgr.h"
#include "globle_data.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "net.pb.h"
namespace hld
{
	cross_server_city_war_ws_mgr::cross_server_city_war_ws_mgr()
	{
		clear_data();
	}

	void cross_server_city_war_ws_mgr::clear_data()
	{
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			m_city_war_all_legion_info[i].clear_data();
		}
		for (int32 i = 0; i < max_cross_server_city_group_num; i++)
		{
			m_city_war_map_guid_arr[i].clear_data();
			m_city_war_map_end[i] = false;
		}
		m_is_have_change = false;
		m_is_create_map = false;
		m_last_tick_time = 0;
		m_timer = 0;
		m_is_load_end = false;
		m_last_min_time = 0;
		m_is_refresh_cross_city_war = -1;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			m_all_legion_member_info[i].clear_data();
		}
	}

	void cross_server_city_war_ws_mgr::init_manager()
	{
		send_load_db_msg();
	}

	void cross_server_city_war_ws_mgr::send_load_db_msg()
	{
		//ws2dp_load_cross_city_info msg;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
		server2dp_proto_ws2dp_load_cross_city_info msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_cross_city_info);
	}

	void cross_server_city_war_ws_mgr::func_clear_all_city_war_msg()
	{
		for (int32 i = 0; i < max_cross_server_war_count; i++)
		{
			m_city_war_all_legion_info[i].clear_data();
		}
		//ws2dp_clear_cross_city_info msg;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));

		server2dp_proto_ws2dp_clear_cross_city_info msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_clear_cross_city_info);

		m_is_have_change = true;
	}

	void cross_server_city_war_ws_mgr::fil_cross_city_info(server2dp_proto_s_cross_city_info & city_info, s_cross_city_info & info)
	{
		city_info.set_legion_guid(info.legion_guid.server_64);
		city_info.set_server_id(info.server_id);
		city_info.set_chief_guid(info.chief_guid.server_64);
		city_info.set_legion_name(info.legion_name);
		city_info.set_chief_name(info.chief_name);
	}

	void cross_server_city_war_ws_mgr::req_load_db_msg(const s_cross_city_info* city_info, int32 data_num, bool is_empty_msg)
	{
		if (is_empty_msg)
		{
			m_is_load_end = true;
			return;
		}
		else
		{
			if (data_num > max_cross_server_all_city_num || city_info == nullptr || data_num <= 0)
			{
				m_is_load_end = true;
				return;
			}
			if (world_server::getInstance().is_sky_island_server())
			{
				bool is_change_server = false;
				for (int32 i = 0; i < data_num; i++)
				{
					m_city_war_all_legion_info[i] = city_info[i];
				}
			}
			else
			{
				for (int32 i = 0; i < data_num; i++)
				{
					m_city_war_all_legion_info[i] = city_info[i];
				}
			}
			m_is_load_end = true;
		}
	}

	void cross_server_city_war_ws_mgr::save_city_info_to_db()
	{
		//if (false == world_server::getInstance().is_gate_server())
		//{
		//	return;
		//}
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (true == m_city_war_all_legion_info[i].is_vaild())
			{
				//ws2dp_save_cross_city_info city_info_msg;
				//city_info_msg.city_info_msg = m_city_war_all_legion_info[i];
				//city_info_msg.rank_idex = i;
				//ws_client::getInstance().send_to_dp(&city_info_msg, sizeof(city_info_msg));
				server2dp_proto_ws2dp_save_cross_city_info msg;
				server2dp_proto_s_cross_city_info* city_info_msg = msg.mutable_city_info_msg();
				fil_cross_city_info(*city_info_msg, m_city_war_all_legion_info[i]);
				msg.set_rank_idex(i);
				ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_cross_city_info);
			}
		}
	}

	void cross_server_city_war_ws_mgr::tick(int64 time_new)
	{
		if (!is_can_open_overload_city_war_with_cur_server())
		{
			return;//本服霸主战也有该逻辑
		}
		m_timer += time_new;
		if (m_timer - m_last_tick_time > second_tick_time)
		{
			m_last_tick_time = m_timer;
			if (is_in_game_time() && !m_is_create_map)
			{
				for (int32 i = 0; i < max_cross_server_all_city_num; i++)
				{
					m_all_legion_member_info[i].clear_data();
				}
				clear_city_master();
				create_map_with_game_begin();
				send_legion_notice_with_begin(e_overload_city_war_type_start_city_war);
			}
			else if (!is_in_game_time() && m_is_create_map)
			{
				clear_msg_with_game_end();
			}
		}
		if (m_timer - m_last_min_time > second_tick_time * 10)
		{
			m_last_min_time = m_timer;
			tick_10_sec(time_new);
		}

	}

	void cross_server_city_war_ws_mgr::send_legion_notice_with_begin(e_overload_city_war_type cur_state)
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			int32 notice_id = 0;
			if (cur_state == e_overload_city_war_type_team_ready)
			{
				notice_id = 93000300;//世界霸主分组完毕公告
			}		
			if (cur_state == e_overload_city_war_type_start_city_war)
			{
				notice_id = 93000411;//世界霸主开始公告
			}
			std::vector<std::string> vec_str_up_item;
			vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			std::string notice_str = init_unit::implode(vec_str_up_item);
			int32 create_time = time_helper::get_cur_time_new().second;
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);

			for (int32 i = 0; i < max_cross_server_all_city_num; i++)
			{
				if (m_city_war_all_legion_info[i].is_vaild())
				{
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(m_city_war_all_legion_info[i].legion_guid);
					if (nullptr == legion_ws_ptr)
					{
						continue;
					}
					if (cur_state == e_overload_city_war_type_team_ready)
					{
						legion_ws_ptr->notice_on_begin_overload_city_war(i);
						legion_ws_ptr->mail_on_begin_overload_city_war(i);
					}
					if (cur_state == e_overload_city_war_type_start_city_war)
					{
						legion_ws_ptr->notice_on_join_overload_city_war();
					}
				}
			}
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				gws2ws_overload_city_begin_notice msg;
				if (cur_state == e_overload_city_war_type_team_ready)
				{
					msg.notice_type = e_overload_notice_type_world_begin;
				}
				if (cur_state == e_overload_city_war_type_start_city_war)
				{
					msg.notice_type = e_overload_notice_type_cross_city_war_begin_world;
				}
				cross::send_msg_to_ws(guid_64(), 0, e_msgindex_gws2ws_overload_begin_notice, &msg, sizeof(msg));

				for (int32 i = 0; i < max_cross_server_all_city_num; i++)
				{
					if (m_city_war_all_legion_info[i].is_vaild())
					{
						int32 server_id = m_city_war_all_legion_info[i].server_id;
						guid_64 legion_guid = m_city_war_all_legion_info[i].legion_guid;

						gws2ws_overload_city_begin_notice msg;
						msg.legion_guid = legion_guid;
						msg.group_num = i;
						if (cur_state == e_overload_city_war_type_team_ready)
						{
							msg.notice_type = e_overload_notice_type_legion_begin;
						}
						if (cur_state == e_overload_city_war_type_start_city_war)
						{
							msg.notice_type = e_overload_notice_type_cross_city_war_begin_legion;
						}
						cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_gws2ws_overload_begin_notice, &msg, sizeof(msg));
					}
				}
			}
			else
			{
				return;
			}
		}
	}

	void cross_server_city_war_ws_mgr::tick_10_sec(int64 time_new)
	{
		if (is_need_refresh_rank_city())
		{
			refresh_rank_city();           
			send_legion_notice_with_begin(e_overload_city_war_type_team_ready);
		}
		if (is_need_add_legion_with_rank_list() > 0)
		{
			add_legion_with_rank_list();
		}
		if (m_is_have_change)
		{
			m_is_have_change = false;
			send_city_info_all_to_other_ws();
			save_city_info_to_db();
			send_city_info_all_to_all_client();
		}
	}

	void cross_server_city_war_ws_mgr::refresh_rank_with_server_list()
	{
		if (!m_city_war_all_legion_info[0].is_vaild() || !world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			return;
		}
		ranking_list* list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);

		int32 cur_idex = 0;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			guid_64 legion_guid = m_city_war_all_legion_info[cur_idex].legion_guid;
			if (nullptr != list_ptr)
			{
				ranking_list_ite ite = list_ptr->begin();
				for (; ite != list_ptr->end(); ++ite)
				{
					if (ite->role_guid == legion_guid)
					{
						m_city_war_all_legion_info[cur_idex].server_id = ite->server_id;
						break;
					}
				}
			}
			if (!world_server::getInstance().is_have_this_server(m_city_war_all_legion_info[cur_idex].server_id))
			{
				CONSOLE_INFO("cross_server_city_war_ws_mgr_del_legion :{} legion_guid:{}", m_city_war_all_legion_info[cur_idex].server_id, m_city_war_all_legion_info[cur_idex].legion_guid.server_64);
				refresh_rank_with_del_legion(m_city_war_all_legion_info[cur_idex].legion_guid);
			}
			else
			{
				cur_idex++;
			}
		}
		m_is_have_change = true;
	}

	void cross_server_city_war_ws_mgr::refresh_rank_with_del_legion(guid_64 legion_guid)
	{
		int32 legion_rank = get_cur_legion_in_rank(legion_guid);
		if (legion_rank < 0)
		{
			return;
		}
		m_city_war_all_legion_info[legion_rank].clear_data();
		if (legion_rank >= max_cross_server_all_city_num - 1)
		{
			return;
		}
		for (int32 i = legion_rank; i < max_cross_server_all_city_num - 1; i++)
		{
			m_city_war_all_legion_info[i] = m_city_war_all_legion_info[i + 1];
			m_city_war_all_legion_info[i + 1].clear_data();
		}
		m_city_war_all_legion_info[max_cross_server_all_city_num - 1].clear_data();
		m_is_have_change = true;
	}

	void cross_server_city_war_ws_mgr::refresh_chief_with_legion(guid_64 legion_guid, guid_64 chief_guid, xstring chief_name)
	{
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (m_city_war_all_legion_info[i].legion_guid == legion_guid)
			{
				m_city_war_all_legion_info[i].chief_guid = chief_guid;
				m_city_war_all_legion_info[i].set_chief_name(chief_name);
				break;
			}
		}
		m_is_have_change = true;
	}

	void cross_server_city_war_ws_mgr::refresh_legion_name(guid_64 legion_guid, xstring legion_name)
	{
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (m_city_war_all_legion_info[i].legion_guid == legion_guid)
			{
				m_city_war_all_legion_info[i].set_legion_name(legion_name);
				break;
			}
		}
		m_is_have_change = true;
	}

	int32 cross_server_city_war_ws_mgr::is_need_add_legion_with_rank_list()
	{
		if (!m_is_load_end)
		{
			return 0;
		}
		if (m_is_create_map)
		{
			return 0;
		}
		if (!m_city_war_all_legion_info[0].legion_guid.is_valid())
		{
			return 0;
		}
		int32 empty_num = 0;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (!m_city_war_all_legion_info[i].is_vaild())
			{
				empty_num++;
			}
		}
		return empty_num;
	}

	void cross_server_city_war_ws_mgr::add_legion_with_rank_list()
	{
		int32 empty_num = is_need_add_legion_with_rank_list();
		if (empty_num <= 0)
		{
			return;
		}
		int32 first_empty_num = -1;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (!m_city_war_all_legion_info[i].is_vaild())
			{
				first_empty_num = i;
				break;
			}
		}
		if (first_empty_num < 0 || first_empty_num < (max_cross_server_all_city_num - empty_num))
		{
			CONSOLE_INFO("cross_server_city_war_ws_mgr first_empty_num:{}", first_empty_num);
			return;
		}
		ranking_list* cur_ranking_list_ptr = nullptr;
		if (world_server::getInstance().is_sky_island_server())
		{
			cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
		}
		else
		{
			cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_core);
		}		
		if (nullptr == cur_ranking_list_ptr)
		{
			return;
		}
		ranking_list_ite cur_list_ite = cur_ranking_list_ptr->begin();
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (cur_list_ite == cur_ranking_list_ptr->end())
			{
				continue;
			}
			if (first_empty_num >= max_cross_server_all_city_num)
			{
				return;
			}
			bool is_have = false;
			for (int32 j = 0; j < first_empty_num; j++)
			{
				if (m_city_war_all_legion_info[j].legion_guid == cur_list_ite->role_guid)
				{
					is_have = true;
					break;
				}
			}
			if (is_have)
			{
				cur_list_ite++;
				continue;
			}
			m_city_war_all_legion_info[first_empty_num].legion_guid = cur_list_ite->role_guid;
			m_city_war_all_legion_info[first_empty_num].chief_guid = cur_list_ite->supporting_guid;
			m_city_war_all_legion_info[first_empty_num].server_id = cur_list_ite->server_id;
			m_city_war_all_legion_info[first_empty_num].set_legion_name(cur_list_ite->role_name);
			m_city_war_all_legion_info[first_empty_num].set_chief_name(cur_list_ite->supporting_name);
			first_empty_num++;
			cur_list_ite++;
			m_is_have_change = true;
		}
	}

	void cross_server_city_war_ws_mgr::set_legion_all_member_list(guid_64 legion_guid, const guid_64* legion_all_member, int32 member_num)
	{
		if (!legion_guid.is_valid() || member_num > max_legion_member_num || nullptr == legion_all_member)
		{
			return;
		}
		int32 empty_idex = -1;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (!m_all_legion_member_info[i].legion_guid.is_valid())
			{
				empty_idex = i;
				break;
			}
		}
		if (empty_idex < 0)
		{
			return;
		}
		m_all_legion_member_info[empty_idex].clear_data();
		m_all_legion_member_info[empty_idex].legion_guid = legion_guid;
		for (int32 i = 0; i < member_num; i++)
		{
			m_all_legion_member_info[empty_idex].all_member_guid[i] = legion_all_member[i];
			CONSOLE_INFO("set_legion_all_member_list legion_guid:{} num:{} role_guid:{}", legion_guid.server_64, i, m_all_legion_member_info[empty_idex].all_member_guid[i].server_64);
		}

	}

	int32 cross_server_city_war_ws_mgr::get_legion_member_list_idex(guid_64 legion_guid)
	{
		int32 cur_idex = -1;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (m_all_legion_member_info[i].legion_guid == legion_guid)
			{
				cur_idex = i;
				break;
			}
		}
		return cur_idex;
	}

	void cross_server_city_war_ws_mgr::end_cross_city_war(guid_64 map_guid, int32 cross_city_idex, guid_64 first_legion_guid, guid_64 second_legion_guid, guid_64 third_legion_guid, const guid_64* first_member_guid, const guid_64* second_member_guid, const guid_64* third_member_guid, int32 first_member_num, int32 second_member_num, int32 third_member_num)
	{
		if (cross_city_idex >= max_cross_server_city_group_num || cross_city_idex < 0)
		{
			CONSOLE_INFO("cross_city_idex:{}", cross_city_idex);
			return;
		}
		m_city_war_map_end[cross_city_idex] = true;
		s_cross_city_info new_legion_info[max_cross_server_one_group_city_num];

		set_legion_all_member_list(first_legion_guid, first_member_guid, first_member_num);
		set_legion_all_member_list(second_legion_guid, second_member_guid, second_member_num);
		set_legion_all_member_list(third_legion_guid, third_member_guid, third_member_num);

		for (int32 i = 0; i < max_cross_server_one_group_city_num; i++)
		{
			if (m_city_war_all_legion_info[cross_city_idex * max_cross_server_one_group_city_num + i].legion_guid == first_legion_guid)
			{
				new_legion_info[0] = m_city_war_all_legion_info[cross_city_idex*max_cross_server_one_group_city_num + i];
				continue;
			}
			else if (m_city_war_all_legion_info[cross_city_idex * max_cross_server_one_group_city_num + i].legion_guid == second_legion_guid)
			{
				new_legion_info[1] = m_city_war_all_legion_info[cross_city_idex*max_cross_server_one_group_city_num + i];
				continue;
			}
			else if (m_city_war_all_legion_info[cross_city_idex * max_cross_server_one_group_city_num + i].legion_guid == third_legion_guid)
			{
				new_legion_info[2] = m_city_war_all_legion_info[cross_city_idex*max_cross_server_one_group_city_num + i];
				continue;
			}
		}
		for (int32 i = 0; i < max_cross_server_one_group_city_num; i++)
		{
			m_city_war_all_legion_info[cross_city_idex * max_cross_server_one_group_city_num + i] = new_legion_info[i];
		}
		if (is_all_city_war_end())
		{
			send_legion_reward_with_rank();
			set_city_war_master();
			m_is_have_change = true;
		}
	}

	void cross_server_city_war_ws_mgr::clear_city_master()
	{
		big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_lord_of_city, guid_64());
		big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_overload_legion, guid_64());
		big_player_ws_mgr::get_instance().clear_title_with_big_player_clear(e_big_player_type_lord_of_city);
		big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(false);
		if (!world_server::getInstance().is_sky_island_server())
		{
			legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
			city_war_mgr.del_overlord_legion();
		}
		m_is_have_change = true;
	}

	void cross_server_city_war_ws_mgr::set_city_war_master()
	{
		if (m_city_war_all_legion_info[0].is_vaild())
		{
			guid_64 new_city_master_guid = m_city_war_all_legion_info[0].chief_guid;
			big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_lord_of_city, new_city_master_guid);
			big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_overload_legion, m_city_war_all_legion_info[0].legion_guid);
			if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
				city_war_mgr.set_overlord_legion(m_city_war_all_legion_info[0].legion_guid, false);
				city_war_mgr.set_city_master_name(city_war_mgr.get_city_master_name_old(), true);
				city_war_mgr.send_city_war_end_notice(m_city_war_all_legion_info[0].legion_guid);
			}
			else
			{
				big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(false);
				client_session* new_city_master_session = client_session_mgr::getInstance().get_session(new_city_master_guid);
				if (new_city_master_session)
				{
					ws2cs_get_city_master_stuff get_city_master_stuff_msg;
					get_city_master_stuff_msg.role_guid = new_city_master_session->get_role_guid();
					bool is_use = proto_by_lua(e_msg_index_ws2cs_get_city_master_stuff);
					if (is_use == false)
					{
						new_city_master_session->send_to_cs(&get_city_master_stuff_msg, sizeof(get_city_master_stuff_msg));
					}
					else
					{
						hld::ws2cs_proto::get_city_master_stuff pro_msg;
						get_city_master_stuff_msg.to_proto(pro_msg);
						new_city_master_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_city_master_stuff);
					}
				}
			}
		}
	}

	void cross_server_city_war_ws_mgr::send_city_info_all_to_other_ws(int32 server_id)
	{
		if (false == world_server::getInstance().is_sky_island_server() || false == gate_proxy::getInstance().is_gate_run() || false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			return;
		}
		int32 data_num = 0;
		gws2ws_cross_city_info_all all_city_info_msg;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (false == m_city_war_all_legion_info[i].is_vaild())
			{
				continue;
			}
			all_city_info_msg.city_info_all[i] = m_city_war_all_legion_info[i];
			data_num++;
		}
		all_city_info_msg.data_num = data_num;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_gws2ws_cross_city_info_all, &all_city_info_msg, sizeof(all_city_info_msg));
	}

	void cross_server_city_war_ws_mgr::recv_gate_msg_city_info_all(s_cross_city_info* city_info, int32 data_num)
	{
		if (data_num > max_cross_server_all_city_num || city_info == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			m_city_war_all_legion_info[i].clear_data();
		}
		for (int32 i = 0; i < data_num; i++)
		{
			m_city_war_all_legion_info[i] = city_info[i];
		}
		send_city_info_all_to_all_client();
	}

	bool cross_server_city_war_ws_mgr::is_all_city_war_end()
	{
		bool is_end = true;
		for (int32 i = 0; i < max_cross_server_city_group_num; i++)
		{
			if (false == m_city_war_map_end[i])
			{
				is_end = false;
				break;
			}
		}
		return is_end;
	}

	void cross_server_city_war_ws_mgr::sort_legion_with_city_war_end()
	{
		s_cross_city_info mid_city_info;
		for (int32 i = 1; i < max_cross_server_city_group_num; i++)
		{
			mid_city_info.clear_data();
			if (m_city_war_all_legion_info[i * max_cross_server_one_group_city_num].is_vaild())
			{
				mid_city_info = m_city_war_all_legion_info[i * max_cross_server_one_group_city_num - 1];
				m_city_war_all_legion_info[i * max_cross_server_one_group_city_num - 1] = m_city_war_all_legion_info[i * max_cross_server_one_group_city_num];
				m_city_war_all_legion_info[i * max_cross_server_one_group_city_num] = mid_city_info;
			}
		}
	}

	void cross_server_city_war_ws_mgr::del_last_group_legion()
	{
		for (int32 i = 0; i < max_cross_server_one_group_city_num; i++)
		{
			m_city_war_all_legion_info[max_cross_server_all_city_num - 1 - i].clear_data();
		}
	}

	void cross_server_city_war_ws_mgr::send_legion_reward_with_rank()
	{
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				for (int32 i = 0; i < max_cross_server_all_city_num; i++)
				{
					if (!m_city_war_all_legion_info[i].is_vaild())
					{
						continue;
					}
					int32 server_id = m_city_war_all_legion_info[i].server_id;
					gws2ws_legion_rank_with_cross_city_war msg;
					msg.legion_guid = m_city_war_all_legion_info[i].legion_guid;
					msg.rank_idex = i;

					int32 cur_legion_idex = get_legion_member_list_idex(m_city_war_all_legion_info[i].legion_guid);
					if (cur_legion_idex < 0)
					{
						continue;
					}
					int32 empty_idex = 0;
					for (int32 i = 0; i < max_legion_member_num; i++)
					{
						CONSOLE_INFO("send_legion_reward_with_rank legion_guid:{} server_id:{} role_guid:{}", m_city_war_all_legion_info[i].legion_guid.server_64, m_city_war_all_legion_info[i].server_id, m_all_legion_member_info[cur_legion_idex].all_member_guid[i].server_64);
						if (!m_all_legion_member_info[cur_legion_idex].all_member_guid[i].is_valid() || empty_idex >= max_legion_member_num)
						{
							continue;
						}
						msg.member_guid_all[empty_idex] = m_all_legion_member_info[cur_legion_idex].all_member_guid[empty_idex];
						empty_idex++;
					}
					msg.member_num = empty_idex;
					cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_gws2ws_legion_rank_with_city_war, &msg, sizeof(msg));
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			for (int32 i = 0; i < max_cross_server_all_city_num; i++)
			{
				if (!m_city_war_all_legion_info[i].is_vaild())
				{
					continue;
				}
				guid_64 legion_guid = m_city_war_all_legion_info[i].legion_guid;

				int32 cur_legion_idex = get_legion_member_list_idex(legion_guid);
				if (cur_legion_idex < 0)
				{
					continue;
				}

				s_city_war_all_member_info mid_member_info;
				mid_member_info.clear_data();
				int32 empty_idex = 0;
				for (int32 i = 0; i < max_legion_member_num; i++)
				{
					if (!m_all_legion_member_info[cur_legion_idex].all_member_guid[i].is_valid() || empty_idex >= max_legion_member_num)
					{
						continue;
					}
					mid_member_info.all_member_guid[empty_idex] = m_all_legion_member_info[cur_legion_idex].all_member_guid[empty_idex];
					empty_idex++;
				}

				legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
				if (nullptr == legion_ws_ptr)
				{
					continue;
				}
				legion_ws_ptr->send_mail_to_all_member_with_city_rank(i, mid_member_info.all_member_guid, empty_idex);
			}
		}
	}

	void cross_server_city_war_ws_mgr::recv_gate_msg_send_legion_reward(guid_64 legion_guid, int32 rank_idex, guid_64* play_guid_list, int32 play_num)
	{
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr || nullptr == play_guid_list || play_num <= 0)
		{
			return;
		}
		legion_ws_ptr->send_mail_to_all_member_with_city_rank(rank_idex, play_guid_list, play_num);
	}

	void cross_server_city_war_ws_mgr::clear_msg_with_game_end()
	{
		m_is_create_map = false;
		for (int32 i = 0; i < max_cross_server_city_group_num; i++)
		{
			m_city_war_map_guid_arr[i].clear_data();
		}
	}

	void cross_server_city_war_ws_mgr::create_map_with_game_begin()
	{
		for (int32 i = 0; i < max_cross_server_city_group_num; i++)
		{
			m_city_war_map_end[i] = false;
		}
		m_is_create_map = true;
		for (int32 i = 0; i < max_cross_server_city_group_num; i++)
		{
			if (!m_city_war_all_legion_info[i * 3].is_vaild())
			{
				m_city_war_map_end[i] = true;
				continue;
			}
			auto map_ent = cs_map_mgr_system::init_map_by_battle(city_war_map_id);
			if (nullptr == map_ent)
			{
				CONSOLE_ERROR("map_ent is nullptr city_war_map_id:{}", city_war_map_id);
				continue;
			}
			m_city_war_map_end[i] = false;
			m_city_war_map_guid_arr[i] = map_ent->getEntityId();
			ws2cs_begin_city_war begin_war_msg;
			begin_war_msg.cur_terr_id = overlord_war_territory_flag;
			begin_war_msg.city_war_map_guid = m_city_war_map_guid_arr[i];
			begin_war_msg.cross_city_idex = i + 1;
			for (int32 j = 0; j < max_cross_server_one_group_city_num; j++)
			{
				s_cross_city_info& legion_info = m_city_war_all_legion_info[i * 3 + j];
				if (!legion_info.is_vaild())
				{
					continue;
				}
				begin_war_msg.join_war_legions[j].legion_guid = legion_info.legion_guid;
				memcpy(begin_war_msg.join_war_legions[j].legion_name, legion_info.legion_name, max_name_size);
				begin_war_msg.join_war_legions[j].bid_price = (max_cross_server_all_city_num - j);
			}

			bool is_use = proto_by_lua(e_msg_index_ws2cs_begin_city_war);
			if (is_use == false)
			{
				cs_map_system::send_message_to_cs(map_ent, &begin_war_msg, sizeof(begin_war_msg));
			}
			else
			{
				hld::ws2cs_proto::begin_city_war pro_msg;
				begin_war_msg.to_proto(pro_msg);
				cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msg_index_ws2cs_begin_city_war);
			}
		}
	}

	bool cross_server_city_war_ws_mgr::is_in_game_time()
	{
		return world_server::getInstance().get_activity_sec_left(e_activity_type_overlord_city_war, e_activity_time_get_all, false, world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war)) > 0;
	}

	bool cross_server_city_war_ws_mgr::is_can_join_with_cur_server(guid_64 role_guid, bool is_cross)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return false;
		}
		if (false == session_ptr->is_self_server())
		{
			return false;
		}
		guid_64 legion_guid = session_ptr->get_legion_guid();
		if (false == legion_guid.is_valid())
		{
			session_ptr->send_notice("90090597");
			return false;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(session_ptr->get_legion_guid());
		if (nullptr == legion_ws_ptr)
		{
			session_ptr->send_notice("90090597");
			return false;
		}
		if (!is_in_game_time())
		{
			session_ptr->send_notice("90201806");
			return false;
		}
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war) != is_cross)
		{
			session_ptr->send_notice("90305015");
			return false;
		}
		if (get_cur_legion_in_rank(legion_guid) < 0)
		{
			session_ptr->send_notice("90203559");
			return false;
		}
		return true;
	}

	e_error_code cross_server_city_war_ws_mgr::is_can_join(guid_64 legion_guid)
	{
		if (!is_in_game_time() || false == m_is_create_map)
		{
			return e_error_code_map_cross_city_war_not_begin;
		}
		guid_64 map_guid = get_map_guid_with_legion_guid(legion_guid);
		if (!map_guid.is_valid())
		{
			return e_error_code_map_cross_city_war_map_not_find;
		}
		if (true == get_map_is_end_with_legion_guid(legion_guid))
		{
			return e_error_code_map_cross_city_war_map_end;
		}
		if (cs_map_system::is_full(map_guid))
		{
			return e_error_code_map_cross_city_war_map_error;
		}
		return e_error_code_success;
	}

	guid_64 cross_server_city_war_ws_mgr::get_map_guid_with_legion_guid(guid_64 legion_guid)
	{
		guid_64 map_guid;
		map_guid.clear_data();
		int32 legion_rank = get_cur_legion_in_rank(legion_guid);
		if (legion_rank < 0)
		{
			return map_guid;
		}
		map_guid = m_city_war_map_guid_arr[legion_rank / max_cross_server_one_group_city_num];
		return map_guid;
	}

	bool cross_server_city_war_ws_mgr::get_map_is_end_with_legion_guid(guid_64 legion_guid)
	{
		int32 legion_rank = get_cur_legion_in_rank(legion_guid);
		if (legion_rank < 0)
		{
			return true;
		}
		return m_city_war_map_end[legion_rank / max_cross_server_one_group_city_num];
	}

	int32 cross_server_city_war_ws_mgr::get_cur_legion_in_rank(guid_64 legion_guid)
	{
		int32 legion_rank = -1;
		for (int32 i = 0; i < max_cross_server_all_city_num; i++)
		{
			if (m_city_war_all_legion_info[i].legion_guid == legion_guid)
			{
				legion_rank = i;
				break;
			}
		}
		return legion_rank;
	}

	bool cross_server_city_war_ws_mgr::is_can_open_overload_city_war_with_cur_server()
	{
		if ((world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war) && world_server::getInstance().is_sky_island_server())
			|| (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war) && !world_server::getInstance().is_sky_island_server()))
		{
			return true;
		}
		return false;
	}

	int64 cross_server_city_war_ws_mgr::get_cross_city_war_refresh_time()
	{
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return -1;
		}
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 now_stamp = cur_time_info.second;
		int32 day_in_week = cur_time_info.day_in_week;

		bool is_today_open = false;
		std::vector<int32> open_data = world_server::getInstance().get_activity_open_data(e_activity_type_overlord_city_war);
		if (world_server::getInstance().is_sky_island_server())//跨服霸主战判定
		{
			for (int32 idx = 0; idx < open_data.size(); idx++)
			{
				if (day_in_week == open_data[idx])
				{
					is_today_open = true;
					break;
				}
			}
		}
		else//本服霸主战判定
		{
			int32 server_open_days = globle_data::get_instance().get_server_days();
			int32 over_load_city_war_day = _activity_cfg_ptr->ParamInt1;
			if (server_open_days < over_load_city_war_day)
			{
				is_today_open = false;
			}
			//else if (server_open_days == over_load_city_war_day)
			//{
			//	is_today_open = true;
			//}
			else
			{
				for (int32 idx = 0; idx < open_data.size(); idx++)
				{
					if (day_in_week == open_data[idx])
					{
						is_today_open = true;
						break;
					}
				}
			}
		}

		if (false == is_today_open)
		{
			return -1;
		}

		int32 refresh_stamp = -1;
		//std::vector<int32> refresh_time_arr = _activity_cfg_ptr->ActivityOpenTime1;
		if (_activity_cfg_ptr->ActivityOpenTime1.size() < 2)
		{
			return -1;
		}
		refresh_stamp = time_helper::get_stamp_by_hour_min(_activity_cfg_ptr->ActivityOpenTime1[0], _activity_cfg_ptr->ActivityOpenTime1[1]);

		return refresh_stamp;	
	}

	bool cross_server_city_war_ws_mgr::is_need_refresh_rank_city()
	{
		if (!m_is_load_end)
		{
			return false;
		}
		if (m_is_refresh_cross_city_war != 0)
		{
			return false;
		}
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 now_stamp = cur_time_info.second;
		int64 refresh_stamp = get_cross_city_war_refresh_time();
		if (refresh_stamp < 0)
		{
			return false;
		}
		if (now_stamp >= refresh_stamp)
		{
			return true;
		}
		return false;
	}

	void cross_server_city_war_ws_mgr::refresh_rank_city()
	{
		if (!m_city_war_all_legion_info[0].is_vaild())//首次刷新逻辑
		{
			for (int32 i = 0; i < max_cross_server_all_city_num; i++)
			{
				m_city_war_all_legion_info[i].clear_data();
			}
			ranking_list* cur_ranking_list_ptr = nullptr;
			if (world_server::getInstance().is_sky_island_server())
			{
				cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_gs_all);
			}
			else
			{
				cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_legion_core);
			}
			if (nullptr == cur_ranking_list_ptr)
			{
				return;
			}
			ranking_list_ite cur_list_ite = cur_ranking_list_ptr->begin();
			for (int32 i = 0; i < max_cross_server_all_city_num; i++)
			{
				if (cur_list_ite == cur_ranking_list_ptr->end())
				{
					continue;
				}
				int32 cur_list_server_id = cur_list_ite->server_id;
				if (cur_list_server_id <= 0)
				{
					cur_list_server_id = world_server::getInstance().get_server_id();
				}
				m_city_war_all_legion_info[i].legion_guid = cur_list_ite->role_guid;
				m_city_war_all_legion_info[i].chief_guid = cur_list_ite->supporting_guid;
				m_city_war_all_legion_info[i].server_id = cur_list_server_id;
				m_city_war_all_legion_info[i].set_legion_name(cur_list_ite->role_name);
				m_city_war_all_legion_info[i].set_chief_name(cur_list_ite->supporting_name);
				cur_list_ite++;
			}
		}
		else
		{
			sort_legion_with_city_war_end();//升降级操作需要放到当日七点
			del_last_group_legion();
			add_legion_with_rank_list();
		}
		m_is_refresh_cross_city_war = 1;
		world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_is_refresh_cross_city_war, 1);
		m_is_have_change = true;
	}

	void cross_server_city_war_ws_mgr::send_city_info_all_to_client(client_session* session_ptr)
	{
		if (nullptr == session_ptr)
		{
			return;
		}
		int32 max_len = max_cross_server_all_city_num;
		//if (!is_need_send_last_group_msg())
		//{
		//	max_len -= max_cross_server_one_group_city_num;
		//}
		legion_proto_cross_city_info_all all_msg;
		for (int32 i = 0; i < max_len; i++)
		{
			if (false == m_city_war_all_legion_info[i].is_vaild())
			{
				continue;
			}
			legion_proto_cross_city_info_one* one_city_msg = all_msg.add_city_info_all();
			fill_one_city_msg(m_city_war_all_legion_info[i], *one_city_msg, i);
		}
		guid_64 overload_chief_guid = big_player_ws_mgr::get_instance().get_big_player_guid(e_big_player_type_lord_of_city);
		if (overload_chief_guid.is_valid())
		{
			all_msg.set_over_chief_guid_a(overload_chief_guid.A);
			all_msg.set_over_chief_guid_b(overload_chief_guid.B);
		}
		session_ptr->send_to_client(&all_msg, e_msgindex_s2c_cross_city_info_all);
	}

	bool cross_server_city_war_ws_mgr::is_need_send_last_group_msg()
	{
		int64 refrsh_stamp = get_cross_city_war_refresh_time();
		if (refrsh_stamp < 0)
		{
			return false;
		}
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return false;
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(e_activity_type_overlord_city_war);
		if (open_time.size() < 8)
		{
			return false;
		}
		int64 end_stamp = time_helper::get_stamp_by_hour_min(open_time[6], open_time[7]);
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 now_stamp = cur_time_info.second;
		if (now_stamp >= refrsh_stamp && now_stamp < end_stamp)
		{
			return true;
		}
		return false;
	}

	void cross_server_city_war_ws_mgr::send_city_info_all_to_all_client()
	{
		legion_proto_cross_city_info_all all_msg;
		int32 max_len = max_cross_server_all_city_num;
		//if (!is_need_send_last_group_msg())
		//{
		//	max_len -= max_cross_server_one_group_city_num;
		//}
		for (int32 i = 0; i < max_len; i++)
		{
			if (false == m_city_war_all_legion_info[i].is_vaild())
			{
				continue;
			}
			legion_proto_cross_city_info_one* one_city_msg = all_msg.add_city_info_all();
			fill_one_city_msg(m_city_war_all_legion_info[i], *one_city_msg, i);
		}
		guid_64 overload_chief_guid = big_player_ws_mgr::get_instance().get_big_player_guid(e_big_player_type_lord_of_city);
		if (overload_chief_guid.is_valid())
		{
			all_msg.set_over_chief_guid_a(overload_chief_guid.A);
			all_msg.set_over_chief_guid_b(overload_chief_guid.B);
		}
		client_session_mgr::getInstance().send_message_to_all_client(&all_msg, e_msgindex_s2c_cross_city_info_all);
	}

	void cross_server_city_war_ws_mgr::fill_one_city_msg(const s_cross_city_info& city_info, legion_proto_cross_city_info_one& one_msg, int32 legion_rank)
	{
		one_msg.set_legion_guid_a(city_info.legion_guid.A);
		one_msg.set_legion_guid_b(city_info.legion_guid.B);
		one_msg.set_server_id(city_info.server_id);
		one_msg.set_chief_guid_a(city_info.chief_guid.A);
		one_msg.set_chief_guid_b(city_info.chief_guid.B);
		one_msg.set_legion_rank(legion_rank);
		one_msg.set_legion_name(city_info.legion_name);
	}

	void cross_server_city_war_ws_mgr::send_to_gate_change_legion_msg(guid_64 legion_guid, e_change_cross_city_type chang_type, xstring role_name /* = "" */, guid_64 role_guid /* = guid_64() */)
	{
		if (get_cur_legion_in_rank(legion_guid) < 0)
		{
			return;
		}
		int32 server_id = world_server::getInstance().get_cross_id();
		ws2gws_change_cross_city_chief msg;
		msg.change_type = chang_type;
		msg.leigon_guid = legion_guid;
		msg.role_guid = role_guid;
		memset(msg.role_name, 0, sizeof(msg.role_name));
		memcpy(msg.role_name, role_name.c_str(), role_name.size() > sizeof(msg.role_name) ? sizeof(msg.role_name) : role_name.size());
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2gws_change_cross_city, &msg, sizeof(msg));
	}

	void cross_server_city_war_ws_mgr::recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id, guid_64 legion_guid)
	{
		auto ret = e_error_code_success;
		int32 map_temp_id = city_war_map_id;
		guid_64 map_guid = get_map_guid_with_legion_guid(legion_guid);
		int32 war_idx = get_cur_legion_in_rank(legion_guid) % 3;
		int32 error_ret = is_can_join(legion_guid);
		ws2ws_transfer_cross_pk_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idx;
		msg.activity_type = (int32)e_activity_type_overlord_city_war;
		msg.server_id = world_server::getInstance().get_server_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_map_island_state_msg, &msg, sizeof(msg));
	}
}
