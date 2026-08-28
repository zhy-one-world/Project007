/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-17 13:19
	
	File Name :	role_pk_manager.cpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "role_pk_manager.h"
#include <role_pk_msg.hpp>
#include "ws_client.hpp"
#include "../client_session_mgr.hpp"
#include "utility/guid_gen.h"
#include "template/template_manager.h"
#include "world_server.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "cross.pb.h"
#include "net.pb.h"
//////////////////////////////////////////////////////////////////////////
//
//	Class Implement
//
//////////////////////////////////////////////////////////////////////////
namespace hld
{

	role_pk_manager::role_pk_manager()
	{
		m_pk_ranking_top_refresh = 0;
		m_all_sign_up_map.clear();
		m_all_role_pk_map.clear();
		m_all_war_map_team_map.clear();
	}

	role_pk_manager::~role_pk_manager()
	{
	}

	void role_pk_manager::tick(float tick_time)
	{
		time_info cur_time = time_helper::get_cur_time_new();
		for (all_role_pk_map_it it = m_all_role_pk_map.begin(); it != m_all_role_pk_map.end(); ++it)
		{
			it->second.heart_tick(tick_time);
		}
		if (cur_time.second - m_pk_ranking_top_refresh >= ROLE_PK_RANKING_TOP_TIME)
		{
			db_role_pk_top();
			m_pk_ranking_top_refresh = cur_time.second;
		}
		group_pk_join();
	}

	void role_pk_manager::group_pk_join()
	{
		for (all_sign_up_map_it sign_it = m_all_sign_up_map.begin(); sign_it != m_all_sign_up_map.end(); ++sign_it)
		{
			sign_up_list& temp_sign_list = sign_it->second;
			if (temp_sign_list.size() >= PK_RANKING_WAR_NUM)
			{
				auto map_ent = cs_map_mgr_system::get_map_ws_by_min(PK_RANKING_MAP_1v1);
				if (nullptr == map_ent)
				{
					CONSOLE_ERROR("role_pk_manager::group_pk_join map_ent is nullptr");
					return;
				}

				//现在的匹配只支持1v1
				s_role_pk_team_info team_info;
				sign_up_list::iterator first_it = temp_sign_list.begin();
				team_info.team_1[0] = (*first_it)->get_role_guid();
				sign_up_list::iterator second_it = first_it++;
				team_info.team_2[0] = (*second_it)->get_role_guid();

				send_group_success((*first_it)->get_role_guid(), (*first_it)->get_role_info(e_role_pk_info_server_id), (*second_it)->get_role_guid(), map_ent->getEntityId(), e_team_num_1);
				(*first_it)->set_pk_state(e_pk_state_join_war);
				(*first_it)->set_cur_war_map(map_ent->getEntityId());

				send_group_success((*second_it)->get_role_guid(), (*second_it)->get_role_info(e_role_pk_info_server_id), (*first_it)->get_role_guid(), map_ent->getEntityId(), e_team_num_2);
				(*second_it)->set_pk_state(e_pk_state_join_war);
				(*second_it)->set_cur_war_map(map_ent->getEntityId());
				temp_sign_list.erase(first_it);
				temp_sign_list.erase(second_it);

				// map_ptr->get_map_guid()这里需要个地图id_成员id的map 结算时通过地图id找到玩家
				m_all_war_map_team_map[map_ent->getEntityId()] = team_info;
			}
		}
	}

	void role_pk_manager::db_role_pk_top()
	{
		server2dp_proto_ws2dp_load_role_pk_top msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_role_pk_top);


		//ws2dp_load_role_pk_top msg;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void role_pk_manager::remove_role_pk_info(const guid_64& role_guid)
	{
		all_role_pk_map_it role_it = m_all_role_pk_map.find(role_guid.server_64);
		if (role_it != m_all_role_pk_map.end())
		{
			c_role_pk_info& role_pk_info = role_it->second;
			LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(role_pk_info.get_role_pk_info(e_role_pk_info_ladder_score));
			if (ladder_template_ptr)
			{
				all_sign_up_map_it score_it = m_all_sign_up_map.find(ladder_template_ptr->attribute_id);
				if (score_it != m_all_sign_up_map.end())
				{
					sign_up_list& temp_sign_list = score_it->second;
					for (sign_up_list::iterator sign_it = temp_sign_list.begin(); sign_it != temp_sign_list.end(); ++sign_it)
					{
						if ((*sign_it)->get_role_guid() == role_guid)
						{
							temp_sign_list.erase(sign_it);
							break;
						}
					}
				}
			}
			role_pk_info.set_pk_state(e_pk_state_no_join);
			role_pk_info.set_game_state(false);
			role_pk_info.clear_cur_war_map();
		}
	}

	void role_pk_manager::add_role_pk_info(const guid_64& role_guid, xchar* role_name, int32 template_id, int32 role_gs)
	{
		all_role_pk_map_it it = m_all_role_pk_map.find(role_guid.server_64);
		if (it != m_all_role_pk_map.end())
		{
			it->second.set_game_state(true);
			send_role_pk_info(it->second);
		}
		else
		{
			c_role_pk_info new_pk_info;
			new_pk_info.set_game_state(true);
			m_all_role_pk_map[role_guid.server_64] = new_pk_info;

			server2dp_proto_ws2dp_load_role_pk_info msg;
			msg.set_role_guid(role_guid.server_64);
			msg.set_role_name(role_name);
			msg.set_template_id(template_id);
			msg.set_role_gs(role_gs);
			ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_role_pk_info);

			//ws2dp_load_role_pk_info msg;
			//memcpy(msg.role_name, role_name, max_name_size);
			//msg.role_guid = role_guid;
			//msg.template_id = template_id;
			//msg.role_gs = role_gs;
			//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
		}
	}

	c_role_pk_info* role_pk_manager::get_role_pk_info(const guid_64& role_guid)
	{
		all_role_pk_map_it it = m_all_role_pk_map.find(role_guid.server_64);
		if (it != m_all_role_pk_map.end())
		{
			return &(it->second);
		}
		return nullptr;
	}

	void role_pk_manager::dp_role_pk_info(s_role_pk_info& pk_info, int32 top_num)
	{
		if (top_num >= 0 && top_num < PK_RANKING_TOP_NUM)
		{
			m_ranking_top[top_num].set_role_pk_info(pk_info);
		}
		else
		{
			m_all_role_pk_map[pk_info.role_guid.server_64].set_role_pk_info(pk_info);
			send_role_pk_info(m_all_role_pk_map[pk_info.role_guid.server_64]);
		}
	}

	void role_pk_manager::send_role_pk_info(const c_role_pk_info& role_pk_info)
	{
		cross_proto_cross_info_end msg;
		msg.set_cross_result(e_cross_sign_up_success);
		cross_proto_cross_role_info* self_role_info = msg.add_role_info_ary();
		self_role_info->set_role_guid(role_pk_info.get_role_guid().server_64);
		self_role_info->set_role_name(role_pk_info.get_role_name());
		for (int32 j = 0; j < e_role_pk_info_max; ++j)
		{
			self_role_info->add_data_array(role_pk_info.get_role_pk_info((e_role_pk_info)j));
		}
		for (int32 i = 0; i < PK_RANKING_TOP_NUM; ++i)
		{
			c_role_pk_info& top_info = m_ranking_top[i];
			cross_proto_cross_role_info* ranking_role_info = msg.add_role_info_ary();
			ranking_role_info->set_role_guid(top_info.get_role_guid().server_64);
			ranking_role_info->set_role_name(top_info.get_role_name());
			for (int32 j = 0; j < e_role_pk_info_max; ++j)
			{
				ranking_role_info->add_data_array(top_info.get_role_pk_info((e_role_pk_info)j));
			}
		}		
		if (role_pk_info.get_role_info(e_role_pk_info_server_id) != world_server::getInstance().get_server_id())
		{
			cross::send_msg_to_ws(role_pk_info.get_role_guid(), role_pk_info.get_role_info(e_role_pk_info_server_id), e_msgindex_s2c_cross_cross_info, &msg);
		}
		else
		{
			client_session* session_ptr = client_session_mgr::getInstance().get_session(role_pk_info.get_role_guid());
			if (nullptr == session_ptr)
			{
				return;
			}
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_cross_info);
		}
	}

	void role_pk_manager::send_group_success(const guid_64& role_guid, int32 server_id, const guid_64& another_role_guid, const guid_64& map_guid, int32 war_index)
	{
		cross_proto_begin_war msg;
		msg.set_map_guid(map_guid.server_64);
		msg.set_war_index(war_index);

		c_role_pk_info* role_pk_info_ptr = get_role_pk_info(another_role_guid);
		if (role_pk_info_ptr == nullptr)
		{
			return;
		}
		msg.set_template_id(role_pk_info_ptr->get_role_info(e_role_pk_info_template_id));

		if (server_id != world_server::getInstance().get_server_id())
		{
			cross::send_msg_to_ws(role_guid, server_id, e_msgindex_s2c_cross_begin_war, &msg);
		}
		else
		{
			client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr == session_ptr)
			{
				return;
			}
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_begin_war);
		}
	}

	guid_64 role_pk_manager::get_player_role_pk_map(const guid_64& role_guid)
	{
		c_role_pk_info* role_info_ptr = get_role_pk_info(role_guid);
		if (nullptr == role_info_ptr)
		{
			return 0;
		}

		return role_info_ptr->get_cur_war_map();
	}

	void role_pk_manager::clear_player_role_pk_map(const guid_64& role_guid)
	{
		c_role_pk_info* role_info_ptr = get_role_pk_info(role_guid);
		if (nullptr == role_info_ptr)
		{
			return;
		}

		role_info_ptr->clear_cur_war_map();
	}

	e_cross_sign_up_result role_pk_manager::sign_up(const guid_64& role_guid, guid_64& team_guid)
	{
		all_role_pk_map_it role_it = m_all_role_pk_map.find(role_guid.server_64);
		if (role_it == m_all_role_pk_map.end())
		{
			return e_cross_sign_up_no_role;
		}
		c_role_pk_info& role_pk_info = role_it->second;
		if (role_pk_info.get_pk_state() != e_pk_state_no_join)
		{
			return e_cross_sign_up_already;
		}
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(role_pk_info.get_role_pk_info(e_role_pk_info_ladder_score));
		if (nullptr == ladder_template_ptr)
		{
			return e_cross_sign_up_template;
		}
		role_pk_info.set_pk_state(e_pk_state_success);
		all_sign_up_map_it score_it = m_all_sign_up_map.find(ladder_template_ptr->attribute_id);
		if (score_it == m_all_sign_up_map.end())
		{
			sign_up_list new_sign_lsit;
			new_sign_lsit.push_back(&role_pk_info);
			m_all_sign_up_map[ladder_template_ptr->attribute_id] = new_sign_lsit;
		}
		else
		{
			sign_up_list& temp_sign_list = score_it->second;
			temp_sign_list.push_back(&role_pk_info);
		}
		return e_cross_sign_up_success;
	}

	e_cross_sign_up_result role_pk_manager::sign_up_leave(const guid_64& role_guid, guid_64& team_guid)
	{
		all_role_pk_map_it role_it = m_all_role_pk_map.find(role_guid.server_64);
		if (role_it == m_all_role_pk_map.end())
		{
			return e_cross_sign_up_no_role;
		}
		c_role_pk_info& role_pk_info = role_it->second;
		if (role_pk_info.get_pk_state() != e_pk_state_success)
		{
			return e_cross_sign_up_state_error;
		}
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(role_pk_info.get_role_pk_info(e_role_pk_info_ladder_score));
		if (nullptr == ladder_template_ptr)
		{
			return e_cross_sign_up_template;
		}
		role_pk_info.set_pk_state(e_pk_state_no_join);
		all_sign_up_map_it score_it = m_all_sign_up_map.find(ladder_template_ptr->attribute_id);
		if (score_it != m_all_sign_up_map.end())
		{
			sign_up_list& temp_sign_list = score_it->second;
			for (sign_up_list::iterator it = temp_sign_list.begin(); it != temp_sign_list.end(); ++it)
			{
				if ((*it)->get_role_guid() == role_guid)
				{
					temp_sign_list.erase(it);
					break;
				}
			}
		}
		return e_cross_sign_up_leave_success;
	}

	void role_pk_manager::role_pk_result(const guid_64& finish_map_guid, e_team_num win_team)
	{
		for (int32 i = 0; i < PK_RANKING_TEAM_NUM; ++i)
		{
			const s_role_pk_team_info& _team_info = get_war_map_team_map(finish_map_guid);

			if (_team_info.team_1[i].is_valid())
			{
				c_role_pk_info* role_pk_info = get_role_pk_info(_team_info.team_1[i]);
				if (nullptr != role_pk_info)
				{
					switch (win_team)
					{
					case e_team_num_1:
						role_pk_info->game_win(_team_info.team_2);
						break;
					case e_team_num_2:
						role_pk_info->game_lost(_team_info.team_2);
						break;
					default:
						role_pk_info->game_peace(_team_info.team_2);
						break;
					}
					role_pk_info->set_pk_state(e_pk_state_no_join);					
				}
			}
			if (_team_info.team_2[i].is_valid())
			{
				c_role_pk_info* role_pk_info = get_role_pk_info(_team_info.team_2[i]);
				if (nullptr != role_pk_info)
				{
					switch (win_team)
					{
					case e_team_num_1:
						role_pk_info->game_lost(_team_info.team_1);
						break;
					case e_team_num_2:
						role_pk_info->game_win(_team_info.team_1);
						break;
					default:
						role_pk_info->game_peace(_team_info.team_1);
						break;
					}
					role_pk_info->set_pk_state(e_pk_state_no_join);
				}
			}
		}

		m_all_war_map_team_map.erase(finish_map_guid.server_64);
	}

	void role_pk_manager::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		//pk_rank
		for (int32 i = 0; i < PK_RANKING_TOP_NUM; ++i)
		{
			c_role_pk_info& role_pk_info = m_ranking_top[i];
			if ( role_pk_info.get_role_guid() == role_guid)
			{
				role_pk_info.set_role_name(role_name);
			}
		}

		auto iter_all_role_pk = m_all_role_pk_map.find(role_guid.server_64);
		if (iter_all_role_pk != m_all_role_pk_map.end())
		{
			iter_all_role_pk->second.set_role_name(role_name);
		}

		auto iter_all_sign_up = m_all_sign_up_map.begin();
		for (; iter_all_sign_up != m_all_sign_up_map.end(); ++iter_all_sign_up)
		{
			sign_up_list& sign_list = iter_all_sign_up->second;
			auto iter_sign_list = sign_list.begin();
			for ( ; iter_sign_list != sign_list.end(); ++iter_sign_list)
			{
				if ( nullptr == (*iter_sign_list))
				{
					continue;
				}

				if ((*iter_sign_list)->get_role_guid() == role_guid)
				{
					(*iter_sign_list)->set_role_name(role_name);
				}
			}
		}
	}
}