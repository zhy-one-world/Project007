/********************************************************************
created: 2016��8��23��13:56:28
file base: arena_mgr_ws_ws
file ext: cpp
author: zhangminghai

purpose: about arena system's arena_mgr_ws
*********************************************************************/

#include <time.hpp>
#include "ws_client.hpp"
#include "arena_mgr_ws.h"
#include "../ranking/ranking_mgr_ws.h"
#include <arena_msg.hpp>
#include "core.hpp"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "utility/random.h"
#include "template/template_manager.h"
#include "template/StringConst_S.h"
#include "../legion/legion_ws_mgr.h"
#include "../big_player_ws_mgr.h"
#include "server_log.hpp"
#include "utility/guid_gen.h"

#include "globle_data.h"
#include "arena.pb.h"
#include "time_def.hpp"
#include "net.pb.h"
#include <rlog.hpp>


namespace faith
{
	void arena_mgr_ws::init_manager()
	{

	}

	void arena_mgr_ws::clear_data()
	{
		m_sec_tick = 0;
		for (int32 i = 0; i < rank_max; ++i)
		{
			m_arena_rank[i].clear_data();
		}
	}

	void arena_mgr_ws::req_load_ranks_from_db()
	{
		//ws2dp_load_arena_rank qwe;
		//ws_client::getInstance().send_to_dp(&request_db, sizeof(request_db));
		//serverws2dp_proto_test_ws2dp_load_arena_rank request_db;
		//request_db.set_role_guid(10);
		//ws_client::getInstance().send_to_dp_new(&request_db, e_mgsindex_ws2dp_msg_test);
		//script_mgr::get_instance().call_func("arena_mgr_ws", "req_load_ranks_from_db_lua", 0, false, "");

		server2dp_proto_ws2dp_add_relation req;
		req.set_role_guid(123);
		req.set_relation_guid(321);
		req.set_relation_name("abc");
		req.set_relation_gs_value(1234);
		for (int32 i = e_relation_data_type; i < e_relation_data_max; i++)
		{
			req.add_data_array(i);
		}
		ws_client::getInstance().send_to_dp_new(&req, e_msg_ws2dp_add_relation, 0);
	}

	void arena_mgr_ws::create_robot()
	{
		if (is_use_lua())
		{
			return;
		}
		template_manager::template_type* robot_name_tmpl_table = template_manager::get_instance().get_templates(e_ArenaRobotTemplate);
		if (nullptr == robot_name_tmpl_table)
		{
			return;
		}
		template_manager::template_type::iterator ite;
		for (ite = robot_name_tmpl_table->begin(); ite != robot_name_tmpl_table->end(); ++ite)
		{
			ArenaRobotTemplate* robot_template_ptr = (ArenaRobotTemplate*)(ite->second);
			if (robot_template_ptr == nullptr)
			{
				continue;
			}
			create_robot_one(robot_template_ptr);
		}
		save_ranks_to_db();
	}

	void arena_mgr_ws::create_robot_one(ArenaRobotTemplate* robot_template_ptr)
	{
		if (robot_template_ptr == nullptr)
		{
			return;
		}

		int32 data_index = robot_template_ptr->Rank;
		if (data_index < 0
			|| data_index >= rank_max)
		{
			return;
		}

		const xstring& robot_name = template_manager::get_instance().get_str_by_string_template_id(robot_template_ptr->RobotNameId);

		m_arena_rank[data_index].role_guid = guid_gen::make_guid();

		m_arena_rank[data_index].set_role_name(robot_name);
		m_arena_rank[data_index].rank_pos = robot_template_ptr->Rank;
		m_arena_rank[data_index].gs_value = robot_template_ptr->GsValue;
		m_arena_rank[data_index].robot_template_id = robot_template_ptr->attribute_id;
		m_arena_rank[data_index].military_level = robot_template_ptr->MilitaryLevel;
	}

	void arena_mgr_ws::load_ranks_from_db(const s_arena_rank* arena_rank_info, int32 start_index, int32 data_num)
	{
		if (arena_rank_info == nullptr
			|| start_index < 0
			|| start_index + data_num > rank_max)
		{
			return;
		}

		if (start_index == 0)
		{
			clear_data();
			for (int32 i = 0; i < rank_max; ++i)
			{
				m_arena_rank[i].rank_pos = i;
			}
		}

		for (int32 i = 0; i < data_num; ++i)
		{
			int32 temp_index = arena_rank_info[i].rank_pos;
			if (temp_index < 0
				|| temp_index >= rank_max)
			{
				continue;
			}

			m_arena_rank[temp_index].rank_pos = arena_rank_info[i].rank_pos;
			m_arena_rank[temp_index].role_guid = arena_rank_info[i].role_guid;
			m_arena_rank[temp_index].set_role_name(arena_rank_info[i].role_name);
			m_arena_rank[temp_index].gs_value = arena_rank_info[i].gs_value;
			m_arena_rank[temp_index].role_template_id = arena_rank_info[i].role_template_id;
			m_arena_rank[temp_index].robot_template_id = arena_rank_info[i].robot_template_id;
			m_arena_rank[temp_index].military_level = arena_rank_info[i].military_rank_level;
			m_arena_rank[temp_index].vip_level = arena_rank_info[i].vip_level;
			m_arena_rank[temp_index].set_game_channel(arena_rank_info[i].game_channel);

			if (arena_rank_info[i].role_template_id == 0)
			{
				ArenaRobotTemplate* robot_template_ptr = GET_TEMPLATE(ArenaRobotTemplate, arena_rank_info[i].robot_template_id);
				if (robot_template_ptr != nullptr)
				{
					m_arena_rank[temp_index].gs_value = robot_template_ptr->GsValue;
					m_arena_rank[temp_index].military_level = robot_template_ptr->MilitaryLevel;
					const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(robot_template_ptr->RobotNameId);
					m_arena_rank[temp_index].set_role_name(temp_string);

				}
			}
		}

		//if (start_index + data_num == rank_max)
		//{
		//	fill_empty_pos();
		//}
	}

	void arena_mgr_ws::fill_empty_pos()
	{
		if (is_use_lua())
		{
			return;
		}
		bool is_has_empty_pos = false;

		std::string fill_num_string = "arena fill empty pos start \n";

		for (int32 i = 0; i < rank_max; ++i)
		{
			if (m_arena_rank[i].role_guid.is_valid())
			{
				continue;
			}

			ArenaRobotTemplate* robot_template_ptr = GET_TEMPLATE(ArenaRobotTemplate, arena_robot_template_start_id + i);
			if (robot_template_ptr == nullptr
				|| robot_template_ptr->Rank != m_arena_rank[i].rank_pos)
			{
				continue;
			}
			create_robot_one(robot_template_ptr);
			is_has_empty_pos = true;	

			std::string temp_string;
			std::stringstream temp_stream;
			temp_stream << i << "---";
			temp_stream >> temp_string;

			fill_num_string += temp_string;
		}

		if (is_has_empty_pos)
		{
			save_ranks_to_db();
			
			fill_num_string += "end \n";
			_RLOG_(MINFO, ::faith::log_detail::format_message("{}",  fill_num_string.c_str()));
		}
	}

	void arena_mgr_ws::save_ranks_to_db()
	{
		if (is_use_lua())
		{
			return;
		}
		ws2dp_save_arena_rank request_db;
		faith::server2dp_proto::ws2dp_save_arena_rank proto_msg;
		request_db.is_first = true;
		int32 temp_count = 0;
		for (int32 i = 0; i < rank_max; ++i)
		{
			if (!m_arena_rank[i].role_guid.is_valid())
			{
				continue;
			}

			s_arena_rank temp_rank_info;
			temp_rank_info.rank_pos = m_arena_rank[i].rank_pos;
			temp_rank_info.role_guid = m_arena_rank[i].role_guid;
			memcpy(temp_rank_info.role_name, m_arena_rank[i].role_name, max_name_size);
			temp_rank_info.gs_value = m_arena_rank[i].gs_value;
			temp_rank_info.role_template_id = m_arena_rank[i].role_template_id;
			temp_rank_info.robot_template_id = m_arena_rank[i].robot_template_id;
			temp_rank_info.military_rank_level = m_arena_rank[i].military_level;
			temp_rank_info.vip_level = m_arena_rank[i].vip_level;
			temp_rank_info.set_game_channel(m_arena_rank[i].game_channel);

			request_db.data_list[temp_count] = temp_rank_info;
			temp_count++;

			if (temp_count >= rank_save_per_time_max)
			{
				proto_msg.Clear();

				request_db.to_proto(proto_msg);
				ws_client::getInstance().send_to_dp_new(&proto_msg, e_msgindex_ws2dp_save_arena_rank);
			
				
				//ws_client::getInstance().send_to_dp(&request_db, sizeof(ws2dp_save_arena_rank));


				request_db.is_first = false;
				request_db.clear_data_array();
				temp_count = 0;
			}
		}

		if (temp_count > 0)
		{
			proto_msg.Clear();

			request_db.to_proto(proto_msg);
			ws_client::getInstance().send_to_dp_new(&proto_msg, e_msgindex_ws2dp_save_arena_rank);

			//ws_client::getInstance().send_to_dp(&request_db, sizeof(ws2dp_save_arena_rank));
		}
	}

	void arena_mgr_ws::save_change_rank_result(const player_arena_rank& my_new_info, const player_arena_rank& target_new_info)
	{
		server2dp_proto_ws2dp_save_arena_rank_change req;
		server2dp_proto_s_arena_rank* my_new_rank_info = req.mutable_my_new_data();
		if (nullptr == my_new_rank_info)
		{
			return;
		}
		my_new_rank_info->set_rank_pos(my_new_info.rank_pos);
		my_new_rank_info->set_role_guid(my_new_info.role_guid);
		my_new_rank_info->set_role_name(my_new_info.role_name);
		my_new_rank_info->set_gs_value(my_new_info.gs_value);
		my_new_rank_info->set_role_template_id(my_new_info.role_template_id);
		my_new_rank_info->set_robot_template_id(my_new_info.robot_template_id);
		my_new_rank_info->set_military_rank_level(my_new_info.military_level);
		my_new_rank_info->set_vip_level(my_new_info.vip_level);
		my_new_rank_info->set_game_channel(my_new_info.game_channel);

		server2dp_proto_s_arena_rank* target_new_rank_info = req.mutable_target_new_data();
		if (nullptr == my_new_rank_info)
		{
			return;
		}
		target_new_rank_info->set_rank_pos(target_new_info.rank_pos);
		target_new_rank_info->set_role_guid(target_new_info.role_guid);
		target_new_rank_info->set_role_name(target_new_info.role_name);
		target_new_rank_info->set_gs_value(target_new_info.gs_value);
		target_new_rank_info->set_role_template_id(target_new_info.role_template_id);
		target_new_rank_info->set_robot_template_id(target_new_info.robot_template_id);
		target_new_rank_info->set_military_rank_level(target_new_info.military_level);
		target_new_rank_info->set_vip_level(target_new_info.vip_level);
		target_new_rank_info->set_game_channel(target_new_info.game_channel);

		ws_client::getInstance().send_to_dp_new(&req, e_msg_ws2dp_save_arena_rank_change);

		//ws2dp_save_arena_rank_change request_db;
		//request_db.my_new_data.rank_pos = my_new_info.rank_pos;
		//request_db.my_new_data.role_guid = my_new_info.role_guid;
		//memcpy(request_db.my_new_data.role_name, my_new_info.role_name, max_name_size);
		//request_db.my_new_data.gs_value = my_new_info.gs_value;
		//request_db.my_new_data.role_template_id = my_new_info.role_template_id;
		//request_db.my_new_data.robot_template_id = my_new_info.robot_template_id;
		//request_db.my_new_data.military_rank_level = my_new_info.military_level;
		//request_db.my_new_data.vip_level = my_new_info.vip_level;
		//request_db.my_new_data.set_game_channel(my_new_info.game_channel);

		//request_db.target_new_data.rank_pos = target_new_info.rank_pos;
		//request_db.target_new_data.role_guid = target_new_info.role_guid;
		//memcpy(request_db.target_new_data.role_name, target_new_info.role_name, max_name_size);
		//request_db.target_new_data.gs_value = target_new_info.gs_value;
		//request_db.target_new_data.role_template_id = target_new_info.role_template_id;
		//request_db.target_new_data.robot_template_id = target_new_info.robot_template_id;
		//request_db.target_new_data.military_rank_level = target_new_info.military_level;
		//request_db.target_new_data.vip_level = target_new_info.vip_level;
		//request_db.target_new_data.set_game_channel(target_new_info.game_channel);
		//ws_client::getInstance().send_to_dp(&request_db, sizeof(ws2dp_save_arena_rank_change));
	}

	void arena_mgr_ws::heart_tick(int32 elapse_time)
	{
		if (is_use_lua())
		{
			return;
		}
		m_sec_tick += elapse_time;
		if (m_sec_tick > 10 * minute_tick_time)
		{
			save_ranks_to_db();
			m_sec_tick = 0;
		}
		if (false)
		{
			req_load_ranks_from_db();
		}
	}

	void arena_mgr_ws::sync_player_gs(const guid_64& role_guid, int64 gs_value, int32 role_rank)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "arena_mgr_sync_player_gs", 0, false, "%l%l%d", role_guid.server_64, gs_value, role_rank);
			return;
		}
		if (!role_guid.is_valid()
			|| role_rank < 0
			|| role_rank >= rank_max)
		{
			return;
		}
		player_arena_rank* temp_info = get_data_by_index(role_rank);
		if (temp_info == nullptr
			|| temp_info->role_guid != role_guid)
		{
			return;
		}
		temp_info->gs_value = gs_value;
	}

	void arena_mgr_ws::sync_player_arena_military_rank(const guid_64& role_guid, int64 military_level, int32 role_rank)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "arena_mgr_sync_arena_military_rank", 0, false, "%l%l%d", role_guid.server_64, military_level, role_rank);
			return;
		}
		if (!role_guid.is_valid()
			|| role_rank < 0
			|| role_rank >= rank_max)
		{
			return;
		}
		player_arena_rank* temp_info = get_data_by_index(role_rank);
		if (temp_info == nullptr
			|| temp_info->role_guid != role_guid)
		{
			return;
		}
		temp_info->military_level = military_level;
	}
	void arena_mgr_ws::sync_first_rank_player_info(const player_arena_rank& old_first_player_info, const player_arena_rank& new_first_player_info)
	{
		int32 big_player_type = big_player_ws_mgr::get_instance().get_big_player_type_with_ranking_type(e_RankingIndex_arena);
		if (big_player_type > 0 && globle_data::get_instance().get_server_on_days() > 1)
		{
			big_player_ws_mgr::get_instance().set_big_player_guid((e_big_player_type)big_player_type, new_first_player_info.role_guid);
		}


		if (old_first_player_info.role_guid == new_first_player_info.role_guid)
		{
			return;
		}

		ws2cs_sync_first_rank_end resp;

		client_session*	old_client_session_ptr = client_session_mgr::getInstance().get_session(old_first_player_info.role_guid);
		if (nullptr != old_client_session_ptr)
		{
			resp.role_guid = old_first_player_info.role_guid;
			resp.is_first = false;
			resp.ranking_type = e_RankingIndex_arena;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_first_rank_end);
			if (is_use == false)
			{
				old_client_session_ptr->send_to_cs(&resp, sizeof(ws2cs_sync_first_rank_end));
			}
			else
			{
				faith::ws2cs_proto::sync_first_rank_end pro_msg;
				resp.to_proto(pro_msg);
				old_client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sync_first_rank_end);
			}
		}

		client_session*	new_client_session_ptr = client_session_mgr::getInstance().get_session(new_first_player_info.role_guid);
		if (nullptr != new_client_session_ptr)
		{
			resp.role_guid = new_first_player_info.role_guid;
			resp.is_first = true;
			resp.ranking_type = e_RankingIndex_arena;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_sync_first_rank_end);
			if (is_use == false)
			{
				new_client_session_ptr->send_to_cs(&resp, sizeof(ws2cs_sync_first_rank_end));
			}
			else
			{
				faith::ws2cs_proto::sync_first_rank_end pro_msg;
				resp.to_proto(pro_msg);
				new_client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sync_first_rank_end);
			}
		}
	}


	void arena_mgr_ws::reset_challenge_state(guid_64 self_guid, guid_64 target_guid, int32 target_rank, int32 self_rank)
	{
		if (self_rank >= 0
			&& self_rank < rank_max
			&& m_arena_rank[self_rank].role_guid == self_guid)
		{
			m_arena_rank[self_rank].challenge_end_time = 0;
		}

		if (target_rank >= 0
			&& target_rank < rank_max
			&& m_arena_rank[target_rank].role_guid == target_guid)
		{
			m_arena_rank[target_rank].challenge_end_time = 0;
		}
	}

	void arena_mgr_ws::req_challenge(guid_64 challenge_role_guid, guid_64 target_role_guid, int32 cost_money_type, int32 cost_money_num, int32 target_cur_pos, int32 self_old_pos)
	{
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(challenge_role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}

		int32 self_cur_pos = self_old_pos;
		player_arena_rank*  temp_target_player = get_data_by_index(target_cur_pos);
		player_arena_rank*  temp_challenge_player = get_data_by_index(self_cur_pos);
		int32 req_result = get_req_result(temp_target_player, temp_challenge_player, target_role_guid, challenge_role_guid, self_old_pos);
		if (req_result == e_arena_self_pos_is_changed)
		{
			self_cur_pos = get_player_current_index(challenge_role_guid);
		}

		ws2cs_req_challenge_msg response;
		response.role_guid = challenge_role_guid;
		response.target_guid = target_role_guid;
		response.req_result = req_result;
		if (temp_target_player != nullptr)
		{
			response.robot_id = temp_target_player->robot_template_id;
		}
		response.cost_money_type = cost_money_type;
		response.cost_money_value = cost_money_num;
		response.target_cur_pos = target_cur_pos;
		response.self_cur_pos = self_cur_pos;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_req_challenge);
		if (is_use == false)
		{
			client_session_ptr->send_to_cs(&response, sizeof(ws2cs_req_challenge_msg));
		}
		else
		{
			faith::ws2cs_proto::req_challenge_msg pro_msg;
			response.to_proto(pro_msg);
			client_session_ptr->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_req_challenge);
		}
		if (req_result == e_arena_match_success)
		{
			if (temp_target_player != nullptr)
			{
				temp_target_player->challenge_end_time = init_unit::get_end_time(300 + 30);
			}

			if (temp_challenge_player != nullptr)
			{
				temp_challenge_player->challenge_end_time = init_unit::get_end_time(300 + 30);
			}
		}
	}

	int32 arena_mgr_ws::get_req_result(player_arena_rank* target_ptr, player_arena_rank* challenge_ptr, guid_64 target_guid, guid_64 challenge_guid, int32 challenge_old_pos)
	{
		if (target_ptr == nullptr)
		{
			return e_arena_target_pos_is_changed;
		}
		else if (target_ptr->role_guid != target_guid)
		{
			return e_arena_target_pos_is_changed;
		}

		if (challenge_ptr == nullptr)
		{
			if (challenge_old_pos != arena_not_in_list_rank)
			{
				return e_arena_self_pos_is_changed;
			}
		}
		else
		{
			if (challenge_ptr->role_guid != challenge_guid)
			{
				return e_arena_self_pos_is_changed;
			}
		}

		if (target_ptr->challenge_end_time > utility::get_tick_count())
		{
			return e_arena_target_is_challenging;
		}
		if (challenge_ptr != nullptr
			&& challenge_ptr->challenge_end_time > utility::get_tick_count())
		{
			return e_arena_self_is_challenging;
		}

		return e_arena_match_success;
	}


	void arena_mgr_ws::get_choose_list(const guid_64& role_guid, const int32& role_pos)
	{
		int32 index = role_pos;
		{
			player_arena_rank* tamp_player_info = get_data_by_index(role_pos);
			if (tamp_player_info == nullptr)
			{
				if (role_pos != arena_not_in_list_rank)
				{
					index = arena_not_in_list_rank;
				}
			}
			else
			{
				if (tamp_player_info->role_guid != role_guid)
				{
					index = get_player_current_index(role_guid);
				}
			}
		}

		int64 cur_time = utility::get_tick_count();


		int32 match_index1, match_index2, match_index3;
		if (index < 0
			|| index == rank_max - 1) //4999--->
		{
			int32 random_range = arena_choose_list_base_range / 3;
			/*	match_index1 = random_gen::get_random((rank_max - random_range), rank_max - 1);
				match_index2 = random_gen::get_random((rank_max - random_range * 2), rank_max - 1 - random_range);
				match_index3 = random_gen::get_random((rank_max - random_range * 3), rank_max - 1 - random_range * 2);*/
			int32 is_cd1 = 0,is_cd2 = 0,is_cd3 = 0;
			match_index1 = get_area_index((rank_max - random_range), rank_max - 1, cur_time, is_cd1);
			match_index2 = get_area_index((rank_max - random_range * 2), rank_max - 1 - random_range, cur_time, is_cd2);
			match_index3 = get_area_index((rank_max - random_range * 3), rank_max - 1 - random_range * 2, cur_time, is_cd3);

			if (is_cd1 || is_cd2 || is_cd3)
			{
				 random_range = areaa_cloose_second_list_base_range / 3;
				 match_index1 = get_area_index((rank_max - random_range), rank_max - 1, cur_time, is_cd1);
				 match_index2 = get_area_index((rank_max - random_range * 2), rank_max - 1 - random_range, cur_time, is_cd2);
				 match_index3 = get_area_index((rank_max - random_range * 3), rank_max - 1 - random_range * 2, cur_time, is_cd3);

			}
		}
		else if (0 == index)
		{
			match_index1 = 2;
			match_index2 = 1;
			match_index3 = 0;
		}
		else if (index < 10)
		{
			match_index1 = index+1;
			match_index2 = index;
			match_index3 = index-1;
		}
		else
		{
			int32 random_range = 0;
			if (index > arena_choose_list_base_range - 1)  // 300 ------5000
			{
				random_range = arena_choose_list_base_range / 3;     //step  =100
			}
			else                                    // 10-299
			{
				random_range = (index / 3) - 1;
			}
			int32 behind_max_index = index + random_range;   
			if (behind_max_index >= rank_max)
			{
				behind_max_index = rank_max - 1;
			}

			/*match_index1 = random_gen::get_random(index + 1, behind_max_index);
			match_index2 = random_gen::get_random((index - random_range), index - 1);
			match_index3 = random_gen::get_random((index - random_range * 2), index - 1 - random_range);*/
			int32 is_cd = 0;
			match_index1 = get_area_index(index + 1, behind_max_index,cur_time, is_cd);
			match_index2 = get_area_index((index - random_range), index - 1, cur_time, is_cd);
			match_index3 = get_area_index((index - random_range * 2), index - 1 - random_range, cur_time, is_cd);

		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}

		ws2cs_req_choose_list response;
		response.role_guid = role_guid;

		if (match_index1 >= 0
			|| match_index1 < rank_max)
		{
			response.target_player[0] = m_arena_rank[match_index1];
		}
		else if (index >= 0
			|| index < rank_max)
		{
			response.target_player[0] = m_arena_rank[index];
		}
		else
		{
			response.target_player[0] = m_arena_rank[rank_max - 1];
		}

		if (match_index2 >= 0
			|| match_index2 < rank_max)
		{
			response.target_player[1] = m_arena_rank[match_index2];
		}
		else if (index >= 0
			|| index < rank_max)
		{
			response.target_player[1] = m_arena_rank[index];
		}
		else
		{
			response.target_player[1] = m_arena_rank[rank_max - 2];
		}

		if (match_index3 >= 0
			|| match_index3 < rank_max)
		{
			response.target_player[2] = m_arena_rank[match_index3];
		}
		else if (index >= 0
			|| index < rank_max)
		{
			response.target_player[2] = m_arena_rank[index];
		}
		else
		{
			response.target_player[2] = m_arena_rank[rank_max - 3];
		}
		bool is_use = proto_by_lua(e_msg_index_ws2cs_req_challenge_list);
		if (is_use == false)
		{
			client_session_ptr->send_to_cs(&response, sizeof(ws2cs_req_choose_list));
		}
		else
		{
			faith::ws2cs_proto::req_choose_list pro_msg;
			response.to_proto(pro_msg);
			client_session_ptr->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_req_challenge_list);
		}
	}
	int32 arena_mgr_ws::get_area_index(int32 start, int32 end, int64 cur_time, int32& is_cd)
	{
		is_cd = 0;
		if (start < 0 || start >= rank_max || end < 0 || end >= rank_max)
		{
			return  rank_max - 1;
		}
		int32 index = random_gen::get_random(start, end);

		if (m_arena_rank[index].challenge_end_time < cur_time)
		{
			return index;
		}
		for (int32 i = index - 1; i >= start; i--)
		{
			if (m_arena_rank[i].challenge_end_time < cur_time)
			{
				return i;
			}
		}
		for (int32 i = index + 1; i <= end; i++)
		{
			if (m_arena_rank[i].challenge_end_time < cur_time)
			{
				return i;
			}
		}
		is_cd = 1;
		return index;
	}
	void arena_mgr_ws::get_first_three(guid_64 role_guid)
	{
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		arena_proto_get_top_three_player_end msg;
		guid_64 first_guid = ranking_mgr_ws::get_ranking_player_guid(e_RankingIndex_arena, 0);
		guid_64 second_guid = ranking_mgr_ws::get_ranking_player_guid(e_RankingIndex_arena, 1);
		guid_64 third_guid = ranking_mgr_ws::get_ranking_player_guid(e_RankingIndex_arena, 2);

		msg.add_role_guid(first_guid.A);
		msg.add_role_guid(first_guid.B);

		msg.add_role_guid(second_guid.A);
		msg.add_role_guid(second_guid.B);

		msg.add_role_guid(third_guid.A);
		msg.add_role_guid(third_guid.B);

		client_session_ptr->send_to_client(&msg, e_msgindex_s2c_get_top_three_player_end);
	}
	

	int32 arena_mgr_ws::change_rank(const player_arena_rank& challenge_info, const guid_64& target_guid, const int32 target_index)
	{
		player_arena_rank* target_info = get_data_by_index(target_index);
		if (target_info == nullptr
			|| target_info->role_guid != target_guid)
		{
			return -1;
		}

		if (challenge_info.rank_pos >= 0
			&& challenge_info.rank_pos < rank_max)
		{
			player_arena_rank* temp_challenge_info = get_data_by_index(challenge_info.rank_pos);
			if (temp_challenge_info == nullptr
				|| temp_challenge_info->role_guid != challenge_info.role_guid)
			{
				return -1;
			}
		}

		reset_challenge_state(challenge_info.role_guid, target_guid, target_index, challenge_info.rank_pos);
		int32 my_index = challenge_info.rank_pos;
		const int32 first_index = 0;

		if (my_index < target_index
			&& my_index > 0)
		{
			return my_index;
		}

		player_arena_rank old_first_player_info = m_arena_rank[first_index];
		int32 cur_time = time_helper::get_cur_time_new().second;

		s_ranking_player_info my_palyer_info;
		my_palyer_info.set_role_name(challenge_info.role_name);
		my_palyer_info.role_guid = challenge_info.role_guid;
		my_palyer_info.ranking_value = target_index;
		my_palyer_info.ranking_type = e_RankingIndex_arena;
		my_palyer_info.save_time = cur_time;
		my_palyer_info.supporting_guid.A = challenge_info.gs_value;
		my_palyer_info.vip_level = challenge_info.vip_level;
		my_palyer_info.set_game_channel(challenge_info.game_channel);
		my_palyer_info.standby_parameter = challenge_info.gs_value;
		legion_ws* my_legion_ptr = legion_ws_mgr::get_instance().get_unit_legion(challenge_info.role_guid);
		if (nullptr != my_legion_ptr)
		{
			my_palyer_info.set_supporting_name(my_legion_ptr->get_legion_name());
		}
		if (my_index == 0)
		{
			my_palyer_info.ranking_value = my_index;
			ranking_mgr_ws::sync_after_merge_first_player_deal(my_palyer_info);
			return my_index;
		}
		player_arena_rank target_player = m_arena_rank[target_index];
		m_arena_rank[target_index].role_guid = challenge_info.role_guid;
		memcpy(m_arena_rank[target_index].role_name, challenge_info.role_name, max_name_size);
		m_arena_rank[target_index].gs_value = challenge_info.gs_value;
		m_arena_rank[target_index].role_template_id = challenge_info.role_template_id;
		m_arena_rank[target_index].robot_template_id = 0;
		m_arena_rank[target_index].vip_level = challenge_info.vip_level;
		m_arena_rank[target_index].military_level = challenge_info.military_level;
		m_arena_rank[target_index].set_game_channel(challenge_info.game_channel);
		if (my_index >= 0
			&& my_index < rank_max)
		{
			m_arena_rank[my_index].role_guid = target_player.role_guid;
			memcpy(m_arena_rank[my_index].role_name, target_player.role_name, max_name_size);
			m_arena_rank[my_index].gs_value = target_player.gs_value;
			m_arena_rank[my_index].role_template_id = target_player.role_template_id;
			m_arena_rank[my_index].robot_template_id = target_player.robot_template_id;
			m_arena_rank[my_index].vip_level = target_player.vip_level;
			m_arena_rank[my_index].military_level = target_player.military_level;
			m_arena_rank[my_index].set_game_channel(target_player.game_channel);
		}

		bool is_robot = true;
		s_ranking_player_info target_palyer_info;
		if (target_player.robot_template_id <= 0)
		{
			is_robot = false;
			target_palyer_info.set_role_name(target_player.role_name);
			target_palyer_info.role_guid = target_player.role_guid;
			target_palyer_info.ranking_value = my_index;
			target_palyer_info.ranking_type = e_RankingIndex_arena;
			target_palyer_info.save_time = cur_time;
			target_palyer_info.supporting_guid.A = target_player.gs_value;
			target_palyer_info.vip_level = target_player.vip_level;
			target_palyer_info.set_game_channel(target_player.game_channel);
			target_palyer_info.standby_parameter = target_player.gs_value;
			legion_ws* target_legion_ptr = legion_ws_mgr::get_instance().get_unit_legion(target_player.role_guid);
			if (nullptr != target_legion_ptr)
			{
				target_palyer_info.set_supporting_name(target_legion_ptr->get_legion_name());
			}
		}
		ranking_mgr_ws::sync_player_arena(my_palyer_info, target_palyer_info, is_robot);
		//----------------------------------------------------

		if (my_index >= 0
			&& my_index < rank_max)
		{
			save_change_rank_result(m_arena_rank[target_index], m_arena_rank[my_index]);
		}
		else
		{
			player_arena_rank temp_target_new_info;
			temp_target_new_info.rank_pos = arena_not_in_list_rank;
			temp_target_new_info.role_guid = target_guid;
			save_change_rank_result(m_arena_rank[target_index], temp_target_new_info);
		}

		//save_ranks_one_to_db(target_index);
		//if (my_index < 0)
		//{
		//	del_arena_rank_by_guid(target_role_guid);
		//}
		//else
		//{
		//	save_ranks_one_to_db(my_index);
		//}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(target_guid);
		if (client_session_ptr != nullptr)
		{
			ws2cs_rep_get_arena_rank  resp;
			resp.role_guid = target_guid;
			resp.arena_rank = my_index;

			bool is_use = proto_by_lua(e_msg_index_ws2cs_rep_get_arena_rank);
			if (is_use == false)
			{
				client_session_ptr->send_to_cs(&resp, sizeof(resp));
			}
			else
			{
				faith::ws2cs_proto::rep_get_arena_rank pro_msg;
				resp.to_proto(pro_msg);
				client_session_ptr->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_rep_get_arena_rank);
			}

			get_choose_list(target_guid, my_index);
		}

		my_index = target_index;

		player_arena_rank new_first_player_info = m_arena_rank[first_index];
		sync_first_rank_player_info(old_first_player_info, new_first_player_info);
		return my_index;
	}

	void arena_mgr_ws::sync_gs_value(const guid_64& role_guid, const int32& temp_pos, const int64& new_gs)
	{
		player_arena_rank* temp_data = get_data_by_index(temp_pos);
		if (temp_data == nullptr
			|| temp_data->role_guid != role_guid)
		{
			return;
		}
		temp_data->gs_value = new_gs;
	}

	int32 arena_mgr_ws::get_player_current_index(const guid_64& role_guid)
	{
		int32 index = -1;
		for (int32 i = 0; i < rank_max;++i)
		{
			if (role_guid == m_arena_rank[i].role_guid)
			{
				index = i;
				break;
			}
		}
		return index;
	}

	
	void arena_mgr_ws::req_get_player_rank(const guid_64& role_guid)
	{
		ws2cs_rep_get_arena_rank  resp;
		resp.role_guid = role_guid;
		resp.is_request = 1;
		player_arena_rank* temp_rank = get_player_arena(role_guid);
		if (temp_rank != nullptr)
		{
			resp.arena_rank = temp_rank->rank_pos;
		}
		else
		{
			resp.arena_rank = arena_not_in_list_rank;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		bool is_use = proto_by_lua(e_msg_index_ws2cs_rep_get_arena_rank);
		if (is_use == false)
		{
			client_session_ptr->send_to_cs(&resp, sizeof(resp));
		}
		else
		{
			faith::ws2cs_proto::rep_get_arena_rank pro_msg;
			resp.to_proto(pro_msg);
			client_session_ptr->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_rep_get_arena_rank);
		}
	}


	player_arena_rank* arena_mgr_ws::get_player_arena(const guid_64& role_guid)
	{
		for (int32 i = 0; i < rank_max; ++i)
		{
			if (m_arena_rank[i].role_guid == role_guid)
			{
				return &m_arena_rank[i];
			}
		}
		return nullptr;
	}

	player_arena_rank* arena_mgr_ws::get_data_by_index(const int32& index)
	{
		if (index < 0 || index >= rank_max)
		{
			return nullptr;
		}
		return &m_arena_rank[index];
	}

	void arena_mgr_ws::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "arena_mgr_change_player_name", 0, false, "%l%s", role_guid.server_64, role_name.c_str());
			return;
		}
		for ( int32 i = 0; i < rank_max; ++i)
		{
			if (m_arena_rank[i].role_guid == role_guid)
			{
				m_arena_rank[i].set_role_name(role_name);
				break;
			}
			
		}
	}
	bool arena_mgr_ws::is_use_lua()
	{
		static int is_get = 0;
		static bool getRet = false;
		if (is_get == 0)
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "is_arena_use_lua", 1, false, ">%b", &bRet);
			getRet = bRet;
			is_get = 1;
			return bRet;
		}
		return getRet;
	}
}