/********************************************************************
created: 2019年5月24日
file base: cross_ladder_ws_mgr
file ext: cpp
author: pengbiao
purpose: 跨服天梯
*********************************************************************/

#include "cross_ladder_ws_mgr.h"
#include "Logic/char_def.hpp"
#include "template/template_manager.h"
#include "../mail/mail_event_ws.h"
#include "ws_client.hpp"
#include "world_server.hpp"
#include "../ranking/ranking_mgr_ws.h"
#include "../legion/legion_ws_mgr.h"
#include "../mail/event_ws_mgr.h"
#include "utility/guid_gen.h"
#include "game_cfg/servers_config.h"
#include "utility/random.h"
#include "utility/init_unit.h"
#include "globle_data.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "cross_ladder.pb.h"
#include "game.pb.h"
#include "net.pb.h"

namespace faith
{
	c_cross_ladder_role_info::c_cross_ladder_role_info()
	{
		m_role_state = e_cross_ladder_role_no_sign_up;
		m_sign_up_time = 0;
		m_previous_opponent = 0;
		m_server_out_port = 0;
	}

	c_cross_ladder_role_info::~c_cross_ladder_role_info()
	{

	}

	void c_cross_ladder_role_info::set_role_info(const s_cross_ladder_role_info& info)
	{
		memcpy(&m_role_info, &info, sizeof(m_role_info));
		if (m_role_info.gs_value <= 0)
		{
			m_role_info.gs_value = m_role_info.data_array[e_cross_ladder_role_info_role_gs];
		}
	}

	void c_cross_ladder_role_info::set_role_guid(const guid_64& temp_guid)
	{
		m_role_info.role_guid = temp_guid;
	}

	void c_cross_ladder_role_info::set_role_name(const xchar* name)
	{
		memset(m_role_info.role_name, 0, sizeof(m_role_info.role_name));
		int32 copy_size = strlen(name);
		copy_size = copy_size > max_name_size ? max_name_size : copy_size;
		memcpy(m_role_info.role_name, name, copy_size);
	}

	void c_cross_ladder_role_info::set_role_name(const xstring& name)
	{
		memset(m_role_info.role_name, 0, sizeof(m_role_info.role_name));
		int32 copy_size = name.size();
		copy_size = copy_size > max_name_size ? max_name_size : copy_size;
		memcpy(m_role_info.role_name, name.c_str(), copy_size);
	}

	void c_cross_ladder_role_info::set_server_name(const xchar* name)
	{
		memset(m_role_info.server_name, 0, max_name_size);
		memcpy(m_role_info.server_name, name, max_name_size);
	}

	void c_cross_ladder_role_info::set_data_array_info(e_cross_ladder_role_info info_type, int32 info_value)
	{
		if (info_type >= e_cross_ladder_role_info_max)
			return;
		m_role_info.data_array[info_type] = info_value;
	}

	void c_cross_ladder_role_info::set_gs_value(int64 gs_value)
	{
		m_role_info.gs_value = gs_value;
	}

	void c_cross_ladder_role_info::check_reset_pk_state()
	{
		if (m_role_state != e_cross_ladder_role_no_sign_up && m_sign_up_time + 600 * second_tick_time < utility::get_tick_count())
			m_role_state = e_cross_ladder_role_no_sign_up;
	}

	void c_cross_ladder_role_info::reduce_ticket()
	{
		if (m_role_info.data_array[e_cross_ladder_role_info_ticket_num])
			m_role_info.data_array[e_cross_ladder_role_info_ticket_num] -= 1;
	}

	void c_cross_ladder_role_info::add_ticket_num()
	{
		m_role_info.data_array[e_cross_ladder_role_info_ticket_num] += 1;
	}

	int32 c_cross_ladder_role_info::get_data_array_info(e_cross_ladder_role_info info_type) const
	{
		if (info_type >= e_cross_ladder_role_info_max)
			return 0;
		return m_role_info.data_array[info_type];
	}
	int64 c_cross_ladder_role_info::get_gs_value()
	{
		return m_role_info.gs_value;
	}
	void c_cross_ladder_role_info::set_server_info(const xstring& out_ip, int32 out_port)
	{
		m_server_out_ip = out_ip;
		m_server_out_port = out_port;
	}

	void c_cross_ladder_role_info::game_win()
	{
		int32 old_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
			return;

		m_role_info.data_array[e_cross_ladder_role_info_total_join_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_total_win_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_last_win_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_cur_season_join_num] += 1;

		int32 tmp_win_num = m_role_info.data_array[e_cross_ladder_role_info_last_win_num] - 1;
		if (tmp_win_num > cross_ladder_ws_mgr_new::get_instance().get_last_win_limit())
			tmp_win_num = cross_ladder_ws_mgr_new::get_instance().get_last_win_limit();

		int32 add_score = ladder_template_ptr->WinScore * (100 + cross_ladder_ws_mgr_new::get_instance().get_last_win_rate() * tmp_win_num) / 100.0;
		m_role_info.data_array[e_cross_ladder_role_info_ladder_score] += add_score;

		bool is_upgrade = false;
		int32 history_score = m_role_info.data_array[e_cross_ladder_role_info_history_score];
		if (m_role_info.data_array[e_cross_ladder_role_info_ladder_score] > history_score)
		{
			m_role_info.data_array[e_cross_ladder_role_info_history_score] = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			int32 new_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			if (check_is_upgrade(history_score, new_score))
				is_upgrade = true;
		}

		game_settle_common(e_map_war_result_win, ladder_template_ptr->WinReward, is_upgrade);
	}

	void c_cross_ladder_role_info::game_lose()
	{
		int32 old_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
			return;

		m_role_info.data_array[e_cross_ladder_role_info_ladder_score] += ladder_template_ptr->LoseScore;
		m_role_info.data_array[e_cross_ladder_role_info_total_join_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_cur_season_join_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_last_win_num] = 0;

		bool is_upgrade = false;
		int32 history_score = m_role_info.data_array[e_cross_ladder_role_info_history_score];
		if (m_role_info.data_array[e_cross_ladder_role_info_ladder_score] > history_score)
		{
			m_role_info.data_array[e_cross_ladder_role_info_history_score] = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			int32 new_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			if (check_is_upgrade(history_score, new_score))
				is_upgrade = true;
		}

		game_settle_common(e_map_war_result_lost, ladder_template_ptr->LoseReward, is_upgrade);
	}

	void c_cross_ladder_role_info::game_peace()
	{
		int32 old_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
			return;

		m_role_info.data_array[e_cross_ladder_role_info_ladder_score] += ladder_template_ptr->PeaceScore;
		m_role_info.data_array[e_cross_ladder_role_info_total_join_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_cur_season_join_num] += 1;
		m_role_info.data_array[e_cross_ladder_role_info_last_win_num] = 0;

		bool is_upgrade = false;
		int32 history_score = m_role_info.data_array[e_cross_ladder_role_info_history_score];
		if (m_role_info.data_array[e_cross_ladder_role_info_ladder_score] > history_score)
		{
			m_role_info.data_array[e_cross_ladder_role_info_history_score] = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			int32 new_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			if (check_is_upgrade(history_score, new_score))
				is_upgrade = true;
		}

		game_settle_common(e_map_war_result_peace, ladder_template_ptr->PeaceReward, is_upgrade);
	}

	bool c_cross_ladder_role_info::check_is_upgrade(int32 old_score, int32 new_score)
	{
		auto old_grade_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		auto new_grade_ptr = template_manager::get_instance().get_template_by_ladder_score(new_score);
		if (!old_grade_ptr || !new_grade_ptr)
			return false;
		if (old_grade_ptr->attribute_id != new_grade_ptr->attribute_id)
			return true;
		return false;
	}

	void c_cross_ladder_role_info::game_settle_common(e_map_war_result result, const std::vector<int32>& reward_vec, bool is_upgrade)
	{
		int32 cur_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(cur_score);
		if (nullptr == ladder_template_ptr)
		{
			return;
		}

		int32 vec_size = reward_vec.size();
		if (vec_size && vec_size % 3 == 0)
		{
			const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_reward_title);
			const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_reward_context);
			if (title != nullptr && content != nullptr)
			{
				int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.clear();

				for (int32 i = 0; i < vec_size / 3; ++i)
				{
					int32 tmp_index = i * 3;
					switch (reward_vec[tmp_index])
					{
					case e_cross_ladder_reward_first_money:
					{
						money_type_1 = reward_vec[tmp_index + 1];
						money_num_1 = reward_vec[tmp_index + 2];
					}
					break;
					case e_cross_ladder_reward_second_money:
					{
						money_type_2 = reward_vec[tmp_index + 1];
						money_num_2 = reward_vec[tmp_index + 2];
					}
					break;
					case e_cross_ladder_reward_object:
					{
						drop_item_list.push_back({ reward_vec[tmp_index + 1] ,reward_vec[tmp_index + 2] ,1 });
					}
					break;
					default:
						break;
					}
				}
				mail_ws_mgr::get_instance().send_mail_system(m_role_info.role_guid, m_role_info.data_array[e_cross_ladder_role_info_server_id], drop_item_list, title, content, money_type_1, money_num_1, money_type_2, money_num_2);
			}
		}

		if (cur_score >= cross_ladder_ws_mgr_new::get_instance().get_rank_limit_socre())
		{
			s_ranking_player_info rank_info;
			rank_info.role_guid = m_role_info.role_guid;;
			rank_info.set_role_name(m_role_info.role_name);
			rank_info.ranking_type = e_RankingIndex_cross_ladder_cur_season;
			rank_info.ranking_value = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
			rank_info.set_supporting_name(m_role_info.server_name);
			rank_info.supporting_guid.B = m_role_info.data_array[e_cross_ladder_role_info_template_id];
			rank_info.vip_level = m_role_info.data_array[e_cross_ladder_role_info_vip_level];
			rank_info.server_id = m_role_info.data_array[e_cross_ladder_role_info_server_id];
			rank_info.save_time = time_helper::get_cur_time_new().second;
			rank_info.standby_parameter = m_role_info.gs_value;
			ranking_mgr_ws::sync_player_info(rank_info);
		}

		update_info_to_dp();

		//更新玩家数据
		ws2ws_cross_ladder_sync_game_result msg;
		memcpy(msg.role_name, m_role_info.role_name, max_name_size);
		msg.ladder_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		msg.vip_leve = m_role_info.data_array[e_cross_ladder_role_info_vip_level];
		msg.ticket_num = m_role_info.data_array[e_cross_ladder_role_info_ticket_num];
		msg.is_upgrade = is_upgrade;
		cross::send_msg_to_ws(m_role_info.role_guid, m_role_info.data_array[e_cross_ladder_role_info_server_id], e_msgindex_ws2ws_cross_ladder_sync_game_result, &msg, sizeof(msg));
	}

	void c_cross_ladder_role_info::update_info_to_dp()
	{
		ws2dp_cross_ladder_update_info dp_msg;
		dp_msg.role_guid = m_role_info.role_guid;
		dp_msg.total_win_num = m_role_info.data_array[e_cross_ladder_role_info_total_win_num];
		dp_msg.last_win_num = m_role_info.data_array[e_cross_ladder_role_info_last_win_num];
		dp_msg.ladder_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		dp_msg.ticket_num = m_role_info.data_array[e_cross_ladder_role_info_ticket_num];
		dp_msg.history_score = m_role_info.data_array[e_cross_ladder_role_info_history_score];
		dp_msg.cur_season_join_num = m_role_info.data_array[e_cross_ladder_role_info_cur_season_join_num];
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void c_cross_ladder_role_info::refresh_season_info()
	{
		ws2dp_cross_ladder_refresh_season_info dp_msg;
		dp_msg.role_guid = m_role_info.role_guid;
		dp_msg.ladder_score = m_role_info.data_array[e_cross_ladder_role_info_ladder_score];
		dp_msg.history_score = m_role_info.data_array[e_cross_ladder_role_info_history_score];
		dp_msg.last_score = m_role_info.data_array[e_cross_ladder_role_info_last_score];
		dp_msg.season_reward = m_role_info.data_array[e_cross_ladder_role_info_season_reward];
		dp_msg.cur_season_join_num = m_role_info.data_array[e_cross_ladder_role_info_cur_season_join_num];
		dp_msg.last_season_join_num = m_role_info.data_array[e_cross_ladder_role_info_last_season_join_num];
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void c_cross_ladder_role_info::send_last_season_score(int32 server_id)
	{
		if (m_role_info.data_array[e_cross_ladder_role_info_last_score] == -1)
			return;
		ws2ws_cross_ladder_send_last_score msg;
		msg.role_guid = m_role_info.role_guid;
		msg.last_score = m_role_info.data_array[e_cross_ladder_role_info_last_score];
		if (server_id)
			cross::send_msg_to_ws(m_role_info.role_guid, server_id, e_msgindex_ws2ws_cross_ladder_send_last_score, &msg, sizeof(msg));
		else
			cross::send_msg_to_ws(m_role_info.role_guid, m_role_info.data_array[e_cross_ladder_role_info_server_id], e_msgindex_ws2ws_cross_ladder_send_last_score, &msg, sizeof(msg));
	}

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	cross_ladder_ws_mgr_new::cross_ladder_ws_mgr_new()
	{
		clear_data();
	}

	cross_ladder_ws_mgr_new::~cross_ladder_ws_mgr_new()
	{

	}

	void cross_ladder_ws_mgr_new::clear_data()
	{
		m_send_notify_flag = 0;
		m_sec_timer = 0;
		m_activity_timer = 0;
		m_is_in_activity_time = false;
		m_refresh_flag = false;
		m_cur_season = 0;
		m_is_load_all = false;
		m_activity_time_vec.clear();
		m_transfer_list.clear();
		m_transfer_info_map.clear();
		m_notify_map.clear();
		m_war_map_team_map.clear();
		m_role_server_info_map.clear();
		m_all_ladder_info_map.clear();
		m_all_sign_up_map.clear();
		m_server_online_num_map.clear();

		m_send_reward_flag = false;
		m_season_reward_list.clear();

		m_config_match_server_id = 0;
		m_config_level_limit = 0;
		m_config_transfer_time = 0;
		m_config_daily_ticket_num = 0;
		m_config_daily_can_buy_num = 0;
		m_config_daily_ticket_cost = 0;
		m_config_daily_ticket_cost_type = 0;
		m_config_match_time_limit = 0;
		m_config_last_win_limit = 0;
		m_config_last_win_rate = 0;
		m_config_rank_limit_score = 0;
	}

	void cross_ladder_ws_mgr_new::init_manager(bool need_load_dp)
	{
		m_activity_timer = utility::get_tick_count() / second_tick_time;

		auto config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr || e_cross_ladder_config_max != config_ptr->ParamIntArr2.size())
			return;

		if (config_ptr->ParamIntArr5.size() <= 0 || config_ptr->ParamIntArr5.size() % 3 != 0)
		{
			return;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		m_activity_time_vec.clear();
		for (auto iter = config_ptr->ParamStringArr2.begin(); iter != config_ptr->ParamStringArr2.end(); ++iter)
		{
			s_cross_ladder_activity_time item;
			int64 beginTime = 0;
			int64 endTime = 0;
			int32 region_type = -1;
			sscanf_s((*iter).c_str(), "%d-%d-%I64d-%I64d", &region_type, &item.season, &beginTime, &endTime);
			if (region_type != region_template_ptr->RegionCode)
			{
				continue;
			}
			item.begin_time = transfer_date_to_stamp(beginTime);
			item.end_time = transfer_date_to_stamp(endTime);
			m_activity_time_vec.push_back(item);
		}

		for (int32 i = 0; i < config_ptr->ParamIntArr5.size() / 3; i++)
		{
			if (config_ptr->ParamIntArr5[i * 3] == region_template_ptr->RegionCode)
			{
				m_config_daily_ticket_cost_type = config_ptr->ParamIntArr5[i * 3 + 1];
				m_config_daily_ticket_cost = config_ptr->ParamIntArr5[i * 3 + 2];
				break;
			}
		}

		m_config_level_limit = config_ptr->LimitLevel;
		m_config_daily_ticket_num = config_ptr->ParamInt1;
		m_config_daily_can_buy_num = config_ptr->ParamInt2;
		m_config_rank_limit_score = config_ptr->ParamInt4;
		m_config_match_server_id = GATECONFIG->gate_id;

		m_config_match_time_limit = config_ptr->ParamIntArr2[e_cross_ladder_config_match_time_limit] * second_tick_time;
		m_config_last_win_limit = config_ptr->ParamIntArr2[e_cross_ladder_config_last_win_limit];
		m_config_last_win_rate = config_ptr->ParamIntArr2[e_cross_ladder_config_last_win_rate];
		m_config_transfer_time = config_ptr->ParamIntArr2[e_cross_ladder_config_transfer_time] * second_tick_time;

		if (need_load_dp)
			load_info_from_db();
	}

	bool cross_ladder_ws_mgr_new::is_match_server()
	{
		return get_match_server_id() == world_server::getInstance().get_server_id();
	}

	int32 cross_ladder_ws_mgr_new::get_match_server_id()
	{
		return m_config_match_server_id;
	}

	void cross_ladder_ws_mgr_new::tick(int64 time_new)
	{
		if (!m_is_load_all && is_match_server())
			return;

		int64 time_new_sec = time_new / second_tick_time;
		for (auto iter = m_activity_time_vec.begin(); iter != m_activity_time_vec.end(); ++iter)
		{
			if (iter->begin_time <= time_new_sec && time_new_sec < iter->end_time)
			{
				m_is_in_activity_time = true;
				m_cur_season = iter->season;
			}
			if (m_activity_timer < iter->begin_time && iter->begin_time <= time_new_sec)
			{
				begin_one_season(iter->season);
			}
			if (m_activity_timer < iter->end_time && iter->end_time <= time_new_sec)
			{
				m_is_in_activity_time = false;
				end_one_season();
			}
		}
		m_activity_timer = time_new_sec;

		if (m_refresh_flag)
			refresh_daily_ticket_info();

		if (m_is_in_activity_time == false)
			return;
		if (is_match_server())
		{
			match(time_new);
		}
		else
		{
			if (time_new > m_sec_timer)
			{
				m_sec_timer = time_new + second_tick_time;
				transfer_player(time_new);
			}
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_ladder))
			{
				if (world_server::getInstance().get_activity_sec_left(e_activity_type_cross_ladder, e_activity_time_get_ready) > 0
					|| world_server::getInstance().get_activity_sec_left(e_activity_type_cross_ladder, e_activity_time_get_notice) > 0)
				{
					m_send_notify_flag = 1;
				}
				else if (world_server::getInstance().get_activity_sec_left(e_activity_type_cross_ladder, e_activity_time_get_gaming) > 0)
				{
					if (m_send_notify_flag == 1)
					{
						send_activity_begin_notice();
						m_send_notify_flag = 0;
					}
				}
			}
		}
	}

	bool cross_ladder_ws_mgr_new::is_in_activity_time()
	{
		return m_is_in_activity_time && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_ladder);
	}

	bool cross_ladder_ws_mgr_new::is_in_game_time()
	{
		return m_is_in_activity_time && world_server::getInstance().get_activity_sec_left(e_activity_type_cross_ladder, e_activity_time_get_gaming) > 0;
	}

	int64 cross_ladder_ws_mgr_new::transfer_date_to_stamp(int64 date)
	{
		tm base_time;
		base_time.tm_year = date / 10000000000 - 1900;
		base_time.tm_mon = date / 100000000 % 100 - 1;
		base_time.tm_mday = date / 1000000 % 100;
		base_time.tm_hour = date / 10000 % 100;
		base_time.tm_min = date / 100 % 100;
		base_time.tm_sec = date % 100;
		return mktime(&base_time);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------
	//	角色原区ws功能
	//-----------------------------------------------------------------------------------------------------------------------------------------

	bool cross_ladder_ws_mgr_new::send_req_cross_ladder_role_info(client_session* session, bool is_online)
	{
		if (nullptr == session || session->is_self_server() == false)
			return false;
		if (gate_proxy::getInstance().is_gate_run() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_ladder))
		{
			ws2ws_cross_ladder_req_role_info msg;
			memcpy(msg.role_name, session->m_role_info.role_name, max_name_size);
			memcpy(msg.server_name, world_server::getInstance().get_server_name().c_str(), max_name_size);
			msg.server_id = world_server::getInstance().get_server_id();
			msg.template_id = session->get_role_info_data(e_role_info_template_id);
			msg.role_gs = session->get_role_gs_value();
			msg.vip_level = session->get_role_info_data(e_role_info_vip_level);
			msg.is_online = is_online;
			cross::send_msg_to_ws(session->get_role_guid(), get_match_server_id(), e_msgindex_ws2ws_cross_ladder_req_role_info, &msg, sizeof(msg));
			return true;
		}
		return false;
	}
	e_cross_ladder_sign_up_ret	cross_ladder_ws_mgr_new::check_can_sign_up(client_session* session)
	{
		if (nullptr == session || false == session->is_vaild() || false == session->is_self_server())
		{
			return e_cross_ladder_sign_up_failed;
		}
		if (is_in_activity_time() == false)
		{
			return e_cross_ladder_sign_up_not_open;
		}
		if (world_server::getInstance().get_activity_sec_left(e_activity_type_cross_ladder, e_activity_time_get_gaming) <= 0)
		{
			return e_cross_ladder_sign_up_not_in_time;
		}
		if (false == gate_proxy::getInstance().is_gate_run())
		{
			return e_cross_ladder_sign_up_cross_server_fail;
		}
		if (session->get_role_info_data(e_role_info_exp_level) < m_config_level_limit)
		{
			return e_cross_ladder_sign_up_level_limit;
		}
		return e_cross_ladder_sign_up_success;
	}

	void cross_ladder_ws_mgr_new::deal_with_match_success_msg(const guid_64& role_guid, const ws2ws_cross_ladder_send_match_success* msg)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr != session_ptr)
		{
			m_transfer_list.push_back(s_cross_ladder_transfer_info(role_guid, msg->server_id, msg->map_id, msg->war_index, utility::get_tick_count()));

			/*if (msg->server_id != world_server::getInstance().get_server_id())
			{
				m_transfer_info_map[role_guid] = msg->target_game_info;
			}*/
			cross_ladder_send_match_success notify;
			notify.set_opponent_name(msg->opponent_name);
			notify.set_opponent_server_name(msg->opponent_server_name);
			notify.set_opponent_server_id(msg->opponent_server_id);
			notify.set_opponent_template_id(msg->opponent_template_id);
			notify.set_opponent_ladder_score(msg->opponent_ladder_score);
			notify.set_opponent_total_join_num(msg->opponent_total_join_num);
			notify.set_opponent_total_win_num(msg->opponent_total_win_num);
			notify.set_opponent_last_win_num(msg->opponent_last_win_num);
			session_ptr->send_to_client(&notify, e_msgindex_s2c_cross_ladder_send_match_success);
		}
	}

	void cross_ladder_ws_mgr_new::transfer_player(int64 time_new)
	{
		for (auto iter = m_transfer_list.begin(); iter != m_transfer_list.end();)
		{
			if (time_new >= iter->m_transfer_time + m_config_transfer_time)
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(iter->m_role_guid);
				if (nullptr != session_ptr)
				{
					if (iter->m_server_id == world_server::getInstance().get_server_id())
						cs_map_mgr_system::transfer_to_map(session_ptr, CROSS_LADDER_MAP_ID, iter->m_map_guid, iter->m_war_index);
					else
						cs_map_mgr_system::transfer_to_map(session_ptr, CROSS_LADDER_MAP_ID, iter->m_map_guid, iter->m_war_index, iter->m_server_id);
				}
				m_transfer_list.erase(iter++);
			}
			else
				break;
		}
	}

	void cross_ladder_ws_mgr_new::deal_with_pk_result_msg(const guid_64& role_guid, const ws2ws_cross_ladder_sync_game_result* msg)
	{
		int32 create_time = time_helper::get_cur_time_new().second;

		//1.同步排行榜
		s_ranking_player_info role_info;
		role_info.role_guid = role_guid;
		role_info.set_role_name(msg->role_name);
		role_info.ranking_type = e_RankingIndex_cross_ladder_score;
		role_info.ranking_value = msg->ladder_score;
		role_info.server_id = world_server::getInstance().get_server_id();
		role_info.vip_level = msg->vip_leve;
		role_info.save_time = create_time;
		ranking_mgr_ws::sync_player_info(role_info);

		if (msg->is_upgrade == false)
			return;
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(msg->ladder_score);
		if (nullptr == ladder_template_ptr)
			return;

		//2.发送系统公告
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr != legion_ptr)
		{
			int32 legion_notice_id = 93000296;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(legion_notice_id));
			vec_notice_str.push_back(msg->role_name);
			vec_notice_str.push_back(init_unit::change_i32_to_string(ladder_template_ptr->RankName));
			xstring notice_str = init_unit::implode(vec_notice_str);
			event_ws_mgr::get_instance().send_notice_to_all(legion_notice_id, create_time, role_guid, notice_str);

			vec_notice_str.clear();
			int32 world_notice_id = 93000297;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(world_notice_id));
			vec_notice_str.push_back(legion_ptr->get_legion_name());
			vec_notice_str.push_back(msg->role_name);
			vec_notice_str.push_back(init_unit::change_i32_to_string(ladder_template_ptr->RankName));
			xstring world_notice_str = init_unit::implode(vec_notice_str);
			event_ws_mgr::get_instance().send_notice_to_all(world_notice_id, create_time, guid_64(), world_notice_str);
		}
		else
		{
			int32 world_notice_id = 93000298;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(world_notice_id));
			vec_notice_str.push_back(msg->role_name);
			vec_notice_str.push_back(init_unit::change_i32_to_string(ladder_template_ptr->RankName));
			xstring notice_str = init_unit::implode(vec_notice_str);
			event_ws_mgr::get_instance().send_notice_to_all(world_notice_id, create_time, guid_64(), notice_str);
		}

		//3.弹窗提示 这里进行了修改 如果角色在原区比赛也同样延迟到玩家切换地图或上线发送
		//因为在比赛结束时候会弹出结算界面 导致2个界面冲突
		//client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		//if (nullptr != session_ptr && session_ptr->is_in_game())
		//{
		//	send_upgrade_notify(session_ptr, ladder_template_ptr->attribute_id);
		//}
		//else
		//{
		//	m_notify_map[role_guid] = ladder_template_ptr->attribute_id;
		//}
		m_notify_map[role_guid] = ladder_template_ptr->attribute_id;

		//4.段位提升奖励
		const auto& reward_vec = ladder_template_ptr->UpgradeReward;
		int32 vec_size = reward_vec.size();
		if (vec_size && vec_size % 3 == 0)
		{
			const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_upgrade_title);
			const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_upgrade_context);
			if (title != nullptr && content != nullptr)
			{
				std::vector<std::string> content_params;
				content_params.clear();
				content_params.push_back(std::string(content));
				content_params.push_back(init_unit::change_i32_to_string(ladder_template_ptr->RankName));
				std::string contenttext = init_unit::implode(content_params);

				int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.clear();

				for (int32 i = 0; i < vec_size / 3; ++i)
				{
					int32 tmp_index = i * 3;
					switch (reward_vec[tmp_index])
					{
					case e_cross_ladder_reward_first_money:
					{
						money_type_1 = reward_vec[tmp_index + 1];
						money_num_1 = reward_vec[tmp_index + 2];
					}
					break;
					case e_cross_ladder_reward_second_money:
					{
						money_type_2 = reward_vec[tmp_index + 1];
						money_num_2 = reward_vec[tmp_index + 2];
					}
					break;
					case e_cross_ladder_reward_object:
					{
						drop_item_list.push_back({ reward_vec[tmp_index + 1] ,reward_vec[tmp_index + 2] ,1 });
					}
					break;
					default:
						break;
					}
				}
				mail_ws_mgr::get_instance().send_mail_system(role_guid, 0, drop_item_list, title, contenttext, money_type_1, money_num_1, money_type_2, money_num_2);
			}
		}
	}

	void cross_ladder_ws_mgr_new::send_upgrade_notify(client_session* session)
	{
		if (nullptr == session)
			return;
		auto iter = m_notify_map.find(session->get_role_guid());
		if (iter != m_notify_map.end())
		{
			cross_ladder_send_upgrade_notify notify;
			notify.set_upgrade_id(iter->second);
			session->send_to_client(&notify, e_msgindex_s2c_cross_ladder_send_upgrade_notify);
			m_notify_map.erase(iter);
		}
	}

	void cross_ladder_ws_mgr_new::session_online(client_session* session)
	{
		if (nullptr == session)
			return;

		if (session->is_self_server())
		{
			send_req_cross_ladder_role_info(session, true);
			send_upgrade_notify(session);
		}

		game_proto_send_server_name msg;
		msg.set_is_self_server(session->is_self_server());
		msg.set_server_name(world_server::getInstance().get_server_name());
		session->send_to_client(&msg, e_msgindex_s2c_send_server_name);
	}

	bool cross_ladder_ws_mgr_new::get_transfer_game_info(const guid_64& role_guid, const int32& server_id, s_game_info& game_info)
	{
		//跨到比赛服
		{
			auto iter = m_transfer_info_map.find(role_guid);
			if (iter != m_transfer_info_map.end())
			{
				if (iter->second.server_id == server_id)
				{
					game_info = iter->second;
					m_transfer_info_map.erase(iter);
					return true;
				}
				return false;
			}
		}
		//回到原区
		{
			auto iter = m_role_server_info_map.find(role_guid);
			if (iter != m_role_server_info_map.end())
			{
				if (iter->second.server_id == server_id)
				{
					game_info = iter->second;
					m_role_server_info_map.erase(iter);
					return true;
				}
				return false;
			}
		}
		return false;
	}

	void cross_ladder_ws_mgr_new::send_activity_begin_notice()
	{
		int32 notice_id = 93000415;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------
	// 比赛服务器ws功能
	//-----------------------------------------------------------------------------------------------------------------------------------------

	void cross_ladder_ws_mgr_new::deal_with_create_map_msg(s_cross_ladder_match_info& match_info)
	{
		auto map_ent = cs_map_mgr_system::init_map_by_battle(CROSS_LADDER_MAP_ID);
		if (map_ent)
		{
			guid_64 map_id = map_ent->getEntityId();
			m_war_map_team_map[map_id] = match_info;
			send_match_success_msg(match_info.role_info[0].role_guid, match_info.role_info[0].server_id, map_id, 1, match_info.role_info[1]);
			send_match_success_msg(match_info.role_info[1].role_guid, match_info.role_info[1].server_id, map_id, 2, match_info.role_info[0]);
			/*if (match_info.role_info[0].server_id != world_server::getInstance().get_server_id())
			{
				s_game_info tmp_game_info;
				tmp_game_info.server_id = match_info.role_info[0].server_id;
				memcpy(tmp_game_info.out_ip, match_info.role_info[0].role_server_out_ip, max_ip_address_length);
				tmp_game_info.out_port = match_info.role_info[0].role_server_out_port;
				m_role_server_info_map[match_info.role_info[0].role_guid.server_64] = tmp_game_info;
				//m_role_server_info_map.insert(std::make_pair(match_info.role_info[0].role_guid, tmp_game_info));
			}
			if (match_info.role_info[1].server_id != world_server::getInstance().get_server_id())
			{
				s_game_info tmp_game_info;
				tmp_game_info.server_id = match_info.role_info[1].server_id;
				memcpy(tmp_game_info.out_ip, match_info.role_info[1].role_server_out_ip, max_ip_address_length);
				tmp_game_info.out_port = match_info.role_info[1].role_server_out_port;
				m_role_server_info_map[match_info.role_info[1].role_guid.server_64] = tmp_game_info;
			}*/
		}
		else
		{
			send_sign_up_result(match_info.role_info[0].role_guid, e_cross_ladder_sign_up_map_limit, match_info.role_info[0].server_id);
			send_sign_up_result(match_info.role_info[1].role_guid, e_cross_ladder_sign_up_map_limit, match_info.role_info[1].server_id);
			ws2ws_cross_ladder_create_map_failed msg;
			msg.first_role_guid = match_info.role_info[0].role_guid;
			msg.second_role_guid = match_info.role_info[1].role_guid;
			cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_cross_ladder_create_map_failed, &msg, sizeof(msg));
		}
	}

	void cross_ladder_ws_mgr_new::deal_with_create_robot_map(s_cross_ladder_match_info& match_info)
	{
		auto map_ent = cs_map_mgr_system::init_map_by_battle(CROSS_LADDER_MAP_ID);
		if (map_ent)
		{
			guid_64 map_id = map_ent->getEntityId();
			m_war_map_team_map[map_id] = match_info;
			send_match_success_msg(match_info.role_info[0].role_guid, match_info.role_info[0].server_id, map_id, 1, match_info.role_info[1]);

			ws2cs_map_dynamic_params map_dynamic_params_msg;
			map_dynamic_params_msg.dynamic_params_map_type = e_map_type_cross_ladder;
			map_dynamic_params_msg.map_guid = map_id;
			map_dynamic_params_msg.dynamic_params_int[0] = match_info.robot_template_id;
			map_dynamic_params_msg.dynamic_params_int[1] = match_info.role_info[1].template_id;
			memcpy(map_dynamic_params_msg.dynamic_params_name, match_info.role_info[1].role_name, max_name_size);

			cs_map_system::send_message_to_cs(map_ent, &map_dynamic_params_msg, sizeof(map_dynamic_params_msg));
		}
		else
		{
			send_sign_up_result(match_info.role_info[0].role_guid, e_cross_ladder_sign_up_map_limit, match_info.role_info[0].server_id);
			ws2ws_cross_ladder_create_map_failed msg;
			msg.first_role_guid = match_info.role_info[0].role_guid;
			msg.second_role_guid = 0;
			cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_cross_ladder_create_map_failed, &msg, sizeof(msg));
		}
	}

	void cross_ladder_ws_mgr_new::send_match_success_msg(const guid_64& role_guid, int32 server_id, guid_64 map_id, int32 war_index, const s_cross_ladder_match_role_info& opponent_info)
	{
		if (role_guid.is_valid())
		{
			ws2ws_cross_ladder_send_match_success msg;
			msg.server_id = world_server::getInstance().get_server_id();
			msg.map_id = map_id;
			msg.war_index = war_index;
			memcpy(msg.opponent_name, opponent_info.role_name, max_name_size);
			memcpy(msg.opponent_server_name, opponent_info.server_name, max_name_size);
			msg.opponent_server_id = opponent_info.server_id;
			msg.opponent_template_id = opponent_info.template_id;
			msg.opponent_ladder_score = opponent_info.ladder_score;
			msg.opponent_total_join_num = opponent_info.total_join_num;
			msg.opponent_total_win_num = opponent_info.total_win_num;
			msg.opponent_last_win_num = opponent_info.last_win_num;
			if (server_id == world_server::getInstance().get_server_id())
				deal_with_match_success_msg(role_guid, &msg);
			else
			{
				auto tmp_server_config = world_server::getInstance().get_server_config_by_server_id(world_server::getInstance().get_server_id());
				if (nullptr != tmp_server_config)
				{
					msg.target_game_info = *tmp_server_config;
					cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_cross_ladder_send_match_success, &msg, sizeof(msg));
				}
			}
		}
	}

	void cross_ladder_ws_mgr_new::deal_with_pk_result_from_cs(const guid_64& map_id, const guid_64& winner_guid)
	{
		auto iter = m_war_map_team_map.find(map_id);
		if (iter != m_war_map_team_map.end())
		{
			bool draw = false;
			if (winner_guid.is_valid() == false) draw = true;
			ws2ws_cross_ladder_send_game_result msg;
			msg.draw = draw;
			if (draw)
			{
				msg.winner_guid = iter->second.role_info[0].role_guid;
				msg.loser_guid = iter->second.role_info[1].role_guid;
			}
			else
			{
				msg.winner_guid = winner_guid;
				msg.loser_guid = iter->second.role_info[0].role_guid;
				if (winner_guid == iter->second.role_info[0].role_guid)
					msg.loser_guid = iter->second.role_info[1].role_guid;
			}
			cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_cross_ladder_send_game_result, &msg, sizeof(msg));
			m_war_map_team_map.erase(iter);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------
	//	匹配服ws功能
	//-----------------------------------------------------------------------------------------------------------------------------------------

	void cross_ladder_ws_mgr_new::load_info_from_db()
	{
		if (is_match_server() == false)
			return;
		ws2dp_cross_ladder_load_info dp_msg;
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void cross_ladder_ws_mgr_new::add_cross_ladder_info(const s_cross_ladder_role_info& info)
	{
		m_all_ladder_info_map[info.role_guid].set_role_info(info);
	}

	void cross_ladder_ws_mgr_new::req_cross_ladder_info(const guid_64& role_guid, const ws2ws_cross_ladder_req_role_info* msg)
	{
		if (!m_is_load_all || nullptr == msg)
			return;
		auto iter = m_all_ladder_info_map.find(role_guid);
		if (iter != m_all_ladder_info_map.end())
		{
			if (strcmp(iter->second.get_role_name(), msg->role_name) != 0
				|| strcmp(iter->second.get_server_name(), msg->server_name) != 0
				|| iter->second.get_data_array_info(e_cross_ladder_role_info_server_id) != msg->server_id
				|| iter->second.get_data_array_info(e_cross_ladder_role_info_template_id) != msg->template_id
				|| iter->second.get_data_array_info(e_cross_ladder_role_info_vip_level) != msg->vip_level
				|| iter->second.get_gs_value() != msg->role_gs)
			{
				iter->second.set_role_name(msg->role_name);
				iter->second.set_server_name(msg->server_name);
				iter->second.set_gs_value(msg->role_gs);
				iter->second.set_data_array_info(e_cross_ladder_role_info_server_id, msg->server_id);
				iter->second.set_data_array_info(e_cross_ladder_role_info_template_id, msg->template_id);
				iter->second.set_data_array_info(e_cross_ladder_role_info_vip_level, msg->vip_level);
				ws2dp_cross_ladder_update_role_info dp_msg;
				dp_msg.role_guid = role_guid;
				dp_msg.role_gs = msg->role_gs;
				dp_msg.vip_level = msg->vip_level;
				dp_msg.server_id = msg->server_id;
				dp_msg.template_id = msg->template_id;
				memcpy(dp_msg.role_name, msg->role_name, max_name_size);
				memcpy(dp_msg.server_name, msg->server_name, max_name_size);
				ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
			}
			send_cross_ladder_info(iter->second, msg->is_online);
			if (iter->second.get_data_array_info(e_cross_ladder_role_info_season_reward) > 0)
				send_season_reward_to_player(role_guid);

			send_season_upreward_to_player(role_guid);
		}
		else
		{
			c_cross_ladder_role_info new_role_info;
			new_role_info.set_role_guid(role_guid);
			new_role_info.set_role_name(msg->role_name);
			new_role_info.set_server_name(msg->server_name);
			new_role_info.set_gs_value(msg->role_gs);
			new_role_info.set_data_array_info(e_cross_ladder_role_info_template_id, msg->template_id);
			new_role_info.set_data_array_info(e_cross_ladder_role_info_vip_level, msg->vip_level);
			new_role_info.set_data_array_info(e_cross_ladder_role_info_server_id, msg->server_id);
			new_role_info.set_data_array_info(e_cross_ladder_role_info_ticket_num, m_config_daily_ticket_num);
			new_role_info.set_data_array_info(e_cross_ladder_role_info_can_buy_num, m_config_daily_can_buy_num);
			new_role_info.set_data_array_info(e_cross_ladder_role_info_last_score, -1);
			m_all_ladder_info_map[role_guid.server_64] = new_role_info;

			ws2dp_cross_ladder_add_role_info dp_msg;
			memcpy(&dp_msg.role_info, &new_role_info.get_role_info(), sizeof(s_cross_ladder_role_info));
			ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));

			send_cross_ladder_info(new_role_info, msg->is_online);
		}
	}

	void cross_ladder_ws_mgr_new::send_cross_ladder_info(const c_cross_ladder_role_info& info, bool is_online)
	{
		cross_ladder_send_role_info msg;
		msg.set_cross_result(1);
		msg.set_is_online(is_online);
		int32 ranking_level = -1;
		int64 ranking_value = 0;
		ranking_mgr_ws::get_player_rank_level(e_RankingIndex_cross_ladder_last_seaon, info.get_role_guid(), ranking_level, ranking_value);
		if (ranking_level < 0)
			ranking_level = 0;
		msg.set_last_season_ranking(ranking_level);

		for (int32 i = 0; i < e_cross_ladder_send_info_max; ++i)
		{
			msg.add_data_array(info.get_data_array_info(e_cross_ladder_role_info(i + CROSS_LADDER_SEND_INFO_OFFSET)));
		}

		cross::send_msg_to_ws(info.get_role_guid(), info.get_data_array_info(e_cross_ladder_role_info_server_id), e_msgindex_s2c_cross_ladder_send_role_info, &msg);
	}

	void cross_ladder_ws_mgr_new::refresh_daily_ticket_info()
	{
		if (is_match_server() == false)
			return;
		if (m_is_load_all == false)
		{
			m_refresh_flag = true;
			return;
		}
		m_refresh_flag = false;
		for (auto iter = m_all_ladder_info_map.begin(); iter != m_all_ladder_info_map.end(); ++iter)
		{
			auto& role_info = iter->second;
			role_info.set_data_array_info(e_cross_ladder_role_info_ticket_num, m_config_daily_ticket_num);
			role_info.set_data_array_info(e_cross_ladder_role_info_can_buy_num, m_config_daily_can_buy_num);
		}
		ws2dp_cross_ladder_refresh_ticket_info dp_msg;
		dp_msg.ticket_num = m_config_daily_ticket_num;
		dp_msg.can_buy_num = m_config_daily_can_buy_num;
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void cross_ladder_ws_mgr_new::req_buy_ticket(const guid_64& role_guid)
	{
		auto iter = m_all_ladder_info_map.find(role_guid);
		if (iter == m_all_ladder_info_map.end())
			return;
		auto& role_info = iter->second;
		int32 can_buy_num = role_info.get_data_array_info(e_cross_ladder_role_info_can_buy_num);
		int32 ticket_num = role_info.get_data_array_info(e_cross_ladder_role_info_ticket_num);
		if (can_buy_num > 0)
		{
			ticket_num += 1;
			can_buy_num -= 1;
			role_info.set_data_array_info(e_cross_ladder_role_info_can_buy_num, can_buy_num);
			role_info.set_data_array_info(e_cross_ladder_role_info_ticket_num, ticket_num);
			cross_ladder_respond_buy_join_ticket msg;
			msg.set_result(e_cross_ladder_buy_ticket_success);
			msg.set_ticket_num(ticket_num);
			msg.set_can_buy_num(can_buy_num);
			cross::send_msg_to_ws(role_info.get_role_guid(), role_info.get_data_array_info(e_cross_ladder_role_info_server_id), e_msgindex_s2c_cross_ladder_respond_buy_join_ticket, &msg);

			ws2dp_cross_ladder_update_ticket_info dp_msg;
			dp_msg.role_guid = role_info.get_role_guid();
			dp_msg.ticket_num = ticket_num;
			dp_msg.can_buy_num = can_buy_num;
			ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
		}
		else
		{
			refound_ticket_cost(role_info.get_role_guid(), role_info.get_data_array_info(e_cross_ladder_role_info_server_id));
			cross_ladder_respond_buy_join_ticket msg;
			msg.set_result(e_cross_ladder_buy_ticket_limit);
			msg.set_ticket_num(ticket_num);
			msg.set_can_buy_num(can_buy_num);
			cross::send_msg_to_ws(role_info.get_role_guid(), role_info.get_data_array_info(e_cross_ladder_role_info_server_id), e_msgindex_s2c_cross_ladder_respond_buy_join_ticket, &msg);
		}
	}

	void cross_ladder_ws_mgr_new::refound_ticket_cost(const guid_64& role_guid, int32 server_id)
	{
		const xchar* seller_mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_refund_title);
		const xchar* seller_mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_refund_context);
		if (seller_mail_title == nullptr || seller_mail_content == nullptr)
			return;
		std::vector<s_item_template_info> drop_item_list;
		mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, seller_mail_title, seller_mail_content, (e_money_type)m_config_daily_ticket_cost_type, m_config_daily_ticket_cost);
	}

	void cross_ladder_ws_mgr_new::req_cross_ladder_last_score(const guid_64& role_guid, int32 server_id)
	{
		auto iter = m_all_ladder_info_map.find(role_guid);
		if (iter != m_all_ladder_info_map.end())
		{
			iter->second.send_last_season_score(server_id);
		}
	}

	e_cross_ladder_sign_up_ret cross_ladder_ws_mgr_new::sign_up(const guid_64& role_guid)
	{
		auto role_it = m_all_ladder_info_map.find(role_guid.server_64);
		if (role_it == m_all_ladder_info_map.end())
			return e_cross_ladder_sign_up_failed;

		auto& role_info = role_it->second;
		role_info.check_reset_pk_state();
		if (role_info.get_pk_state() != e_cross_ladder_role_no_sign_up)
			return e_cross_ladder_sign_up_already;
		if (role_info.get_data_array_info(e_cross_ladder_role_info_ticket_num) <= 0)
			return e_cross_ladder_sign_up_no_ticket;

		int32 segment_value = get_ladder_segment_value(role_info);
		if (segment_value == 0)
			return e_cross_ladder_sign_up_failed;

		role_info.set_pk_state(e_cross_ladder_role_sign_up);
		role_info.set_sign_up_time(utility::get_tick_count());
		auto sign_list_it = m_all_sign_up_map.find(segment_value);
		if (sign_list_it == m_all_sign_up_map.end())
		{
			cross_ladder_sign_up_list new_sign_lsit;
			new_sign_lsit.push_back(&role_info);
			m_all_sign_up_map[segment_value] = new_sign_lsit;
		}
		else
		{
			cross_ladder_sign_up_list& temp_sign_list = sign_list_it->second;
			temp_sign_list.push_back(&role_info);
		}
		return e_cross_ladder_sign_up_success;
	}

	e_cross_ladder_sign_up_ret cross_ladder_ws_mgr_new::sign_up_leave(const guid_64& role_guid)
	{
		auto role_it = m_all_ladder_info_map.find(role_guid.server_64);
		if (role_it == m_all_ladder_info_map.end())
			return e_cross_ladder_sign_up_failed;

		auto& role_info = role_it->second;
		if (role_info.get_pk_state() != e_cross_ladder_role_sign_up)
			return e_cross_ladder_sign_up_failed;

		role_info.set_pk_state(e_cross_ladder_role_no_sign_up);
		int32 segment_value = get_ladder_segment_value(role_info);
		auto sign_list_it = m_all_sign_up_map.find(segment_value);
		if (sign_list_it != m_all_sign_up_map.end())
		{
			cross_ladder_sign_up_list& temp_sign_list = sign_list_it->second;
			for (auto it = temp_sign_list.begin(); it != temp_sign_list.end(); ++it)
			{
				if ((*it)->get_role_guid() == role_guid)
				{
					temp_sign_list.erase(it);
					break;
				}
			}
		}
		return e_cross_ladder_sign_up_leave_success;
	}

	void cross_ladder_ws_mgr_new::send_sign_up_result(const guid_64& role_guid, e_cross_ladder_sign_up_ret result_type, int32 server_id)
	{
		cross_ladder_respond_sign_up msg;
		msg.set_sign_up_result(result_type);
		if (server_id == world_server::getInstance().get_server_id())
		{
			client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr != session_ptr)
				session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_ladder_respond_sign_up);
		}
		else
			cross::send_msg_to_ws(role_guid, server_id, e_msgindex_s2c_cross_ladder_respond_sign_up, &msg);
	}

	void cross_ladder_ws_mgr_new::update_server_online_num(int32 server_id, int32 online_num)
	{
		m_server_online_num_map[server_id] = online_num;
	}

	void cross_ladder_ws_mgr_new::set_role_server_info(const guid_64& role_guid, const xstring& server_ip, int32 server_port)
	{
		auto role_it = m_all_ladder_info_map.find(role_guid.server_64);
		if (role_it != m_all_ladder_info_map.end())
		{
			role_it->second.set_server_info(server_ip, server_port);
		}
	}

	void cross_ladder_ws_mgr_new::begin_one_season(int32 cur_season_num)
	{
		m_transfer_list.clear();
		m_notify_map.clear();
		m_transfer_info_map.clear();
		m_role_server_info_map.clear();
		m_war_map_team_map.clear();
		m_all_sign_up_map.clear();
		m_server_online_num_map.clear();
		m_season_reward_list.clear();

		if (is_match_server() == false && world_server::getInstance().is_sky_island_server() == false && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_ladder))
		{
			std::vector<std::string> title_params;
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_cross_ladder_season_begin_title));
			title_params.push_back(init_unit::change_i32_to_string(cur_season_num));
			std::string titletext = init_unit::implode(title_params);
			std::vector<std::string> content_params;
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_cross_ladder_season_begin_context));
			content_params.push_back(init_unit::change_i32_to_string(cur_season_num));
			std::string contenttext = init_unit::implode(content_params);

			s_item_info final_item_array[max_item_per_mail];
			s_mail_info final_mail_info;
			final_mail_info.mail_guid = guid_gen::make_guid();
			final_mail_info.set_mail_title(titletext);
			final_mail_info.set_mail_content_text(contenttext);
			final_mail_info.data_ary[EMailInfo_IsNeedDelete] = time_helper::get_cur_time_new().second + day_time_second * 15;//15天删除
			final_mail_info.data_ary[EMailInfo_SendTime] = time_helper::get_cur_time_new().second;
			event_ws_mgr::get_instance().add_globel_mail(final_mail_info, final_item_array, 0);

			world_server::getInstance().set_ladder_world_level(0, true, true);
		}
	}

	void cross_ladder_ws_mgr_new::end_one_season()
	{
		if (is_match_server() == false)
		{
			ranking_mgr_ws::clear_data_by_ranking_type(e_RankingIndex_cross_ladder_score);
			return;
		}

		ranking_mgr_ws::replace_cross_ladder_ranking();

		for (auto iter = m_all_ladder_info_map.begin(); iter != m_all_ladder_info_map.end(); ++iter)
		{
			auto& role_info = iter->second;
			int32 old_score = role_info.get_data_array_info(e_cross_ladder_role_info_ladder_score);
			LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
			if (nullptr == ladder_template_ptr || old_score == 0)
			{
				role_info.set_data_array_info(e_cross_ladder_role_info_ladder_score, 0);
				role_info.set_data_array_info(e_cross_ladder_role_info_history_score, 0);
				role_info.set_data_array_info(e_cross_ladder_role_info_last_score, 0);
				role_info.set_data_array_info(e_cross_ladder_role_info_season_reward, 0);
				role_info.set_data_array_info(e_cross_ladder_role_info_last_season_join_num, role_info.get_data_array_info(e_cross_ladder_role_info_cur_season_join_num));
				role_info.set_data_array_info(e_cross_ladder_role_info_cur_season_join_num, 0);
				continue;
			}
			role_info.set_data_array_info(e_cross_ladder_role_info_ladder_score, ladder_template_ptr->ReservedScore);
			role_info.set_data_array_info(e_cross_ladder_role_info_history_score, 0);
			role_info.set_data_array_info(e_cross_ladder_role_info_last_score, old_score);
			role_info.set_data_array_info(e_cross_ladder_role_info_season_reward, m_cur_season);
			role_info.set_data_array_info(e_cross_ladder_role_info_last_season_join_num, role_info.get_data_array_info(e_cross_ladder_role_info_cur_season_join_num));
			role_info.set_data_array_info(e_cross_ladder_role_info_cur_season_join_num, 0);
			role_info.refresh_season_info();
			//role_info.send_last_season_score();
		}

		ws2ws_cross_ladder_notify_season_reward notify_msg;
		cross::send_msg_to_ws(0, -1, e_msgindex_ws2ws_cross_ladder_notify_get_season_reward, &notify_msg, sizeof(notify_msg));
	}

	void cross_ladder_ws_mgr_new::send_season_reward_to_player(const guid_64& role_guid)
	{
		auto role_iter = m_all_ladder_info_map.find(role_guid);
		if (role_iter == m_all_ladder_info_map.end())
			return;

		int32 old_score = role_iter->second.get_data_array_info(e_cross_ladder_role_info_last_score);
		int32 server_id = role_iter->second.get_data_array_info(e_cross_ladder_role_info_server_id);

		send_season_ranking_reward_to_player(role_guid, server_id);
		std::vector<std::string> content_params;
		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_season_reward_title);
		const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_season_reward_context);
		if (title == nullptr || content == nullptr)
		{
			return;
		}
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score, role_iter->second.get_data_array_info(e_cross_ladder_role_info_season_reward));
		if (ladder_template_ptr)
		{
			content_params.clear();
			content_params.push_back(std::string(content));
			content_params.push_back(init_unit::change_i32_to_string(ladder_template_ptr->RankName));
			std::string contenttext = init_unit::implode(content_params);

			std::vector<int32>& reward_vec = ladder_template_ptr->SeasonReward;
			int32 vec_size = reward_vec.size();
			if (vec_size && vec_size % 3 == 0)
			{
				int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.clear();

				for (int32 i = 0; i < vec_size / 3; ++i)
				{
					int32 tmp_index = i * 3;
					switch (reward_vec[tmp_index])
					{
					case e_cross_ladder_reward_first_money:
					{
						money_type_1 = reward_vec[tmp_index + 1];
						money_num_1 = reward_vec[tmp_index + 2];
					}
					break;
					case e_cross_ladder_reward_second_money:
					{
						money_type_2 = reward_vec[tmp_index + 1];
						money_num_2 = reward_vec[tmp_index + 2];
					}
					break;
					case e_cross_ladder_reward_object:
					{
						drop_item_list.push_back({ reward_vec[tmp_index + 1] ,reward_vec[tmp_index + 2] ,1 });
					}
					break;
					default:
						break;
					}
				}
				mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, title, contenttext, money_type_1, money_num_1, money_type_2, money_num_2);
				role_iter->second.set_data_array_info(e_cross_ladder_role_info_season_reward, 0);
				role_iter->second.refresh_season_info();
			}
		}
	}

	void cross_ladder_ws_mgr_new::send_season_ranking_reward_to_player(const guid_64& role_guid, int32 server_id)
	{
		int32 ranking_level = -1;
		int64 ranking_value = 0;
		ranking_mgr_ws::get_player_rank_level(e_RankingIndex_cross_ladder_last_seaon, role_guid, ranking_level, ranking_value);

		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		std::vector<int32>& rwd_data = act_com_cfg->ParamIntArr3;
		globle_data::get_instance().get_item_list_by_rank_rwd(ranking_level, rwd_data, drop_item_list);

		if (drop_item_list.size() <= 0)
		{
			return;
		}
		std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_cross_ladder_season_ranking_reward_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_cross_ladder_season_ranking_reward_context));
		content_params.push_back(init_unit::change_i32_to_string(ranking_level));
		std::string contenttext = init_unit::implode(content_params);

		mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, title, contenttext, 0, 0, 0, 0);
	}

	void cross_ladder_ws_mgr_new::send_season_upreward_to_player(const guid_64& role_guid)
	{
		auto role_iter = m_all_ladder_info_map.find(role_guid);
		if (role_iter == m_all_ladder_info_map.end())
			return;

		int32 reserved_score = role_iter->second.get_data_array_info(e_cross_ladder_role_info_ladder_score);
		int32 history_score = role_iter->second.get_data_array_info(e_cross_ladder_role_info_history_score);
		int32 server_id = role_iter->second.get_data_array_info(e_cross_ladder_role_info_server_id);

		if (history_score >= reserved_score)
		{
			return;
		}
		if (!m_is_in_activity_time)
		{
			return;
		}
		if (GATECONFIG->gate_id != world_server::getInstance().get_server_id())
		{
			return;
		}

		//4.当前所达到段位所有的奖励
		std::vector<LadderTemplate*> ladder_template_ptr_arr = template_manager::get_instance().get_template_array_by_ladder_score(reserved_score);
		for (int32 i = 0; i < ladder_template_ptr_arr.size(); i++)
		{
			if (ladder_template_ptr_arr[i] == nullptr)
			{
				return;
			}
			const auto& reward_vec = ladder_template_ptr_arr[i]->UpgradeReward;
			int32 vec_size = reward_vec.size();
			if (vec_size && vec_size % 3 == 0)
			{
				const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_upgrade_title);
				const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_ladder_upgrade_context);
				if (title != nullptr && content != nullptr)
				{
					std::vector<std::string> content_params;
					content_params.clear();
					content_params.push_back(std::string(content));
					content_params.push_back(init_unit::change_i32_to_string(ladder_template_ptr_arr[i]->RankName));
					std::string contenttext = init_unit::implode(content_params);

					int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
					std::vector<s_item_template_info> drop_item_list;
					drop_item_list.clear();

					for (int32 i = 0; i < vec_size / 3; ++i)
					{
						int32 tmp_index = i * 3;
						switch (reward_vec[tmp_index])
						{
						case e_cross_ladder_reward_first_money:
						{
							money_type_1 = reward_vec[tmp_index + 1];
							money_num_1 = reward_vec[tmp_index + 2];
						}
						break;
						case e_cross_ladder_reward_second_money:
						{
							money_type_2 = reward_vec[tmp_index + 1];
							money_num_2 = reward_vec[tmp_index + 2];
						}
						break;
						case e_cross_ladder_reward_object:
						{
							drop_item_list.push_back({ reward_vec[tmp_index + 1] ,reward_vec[tmp_index + 2] ,1});
						}
						break;
						default:
							break;
						}
					}
					mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, title, contenttext, money_type_1, money_num_1, money_type_2, money_num_2);
				}
			}
		}
		role_iter->second.set_data_array_info(e_cross_ladder_role_info_history_score, reserved_score);
		role_iter->second.refresh_season_info();
	}

	void cross_ladder_ws_mgr_new::match(int64 time_new)
	{
		for (auto iter = m_all_sign_up_map.begin(); iter != m_all_sign_up_map.end(); ++iter)
		{
			auto& cur_list = iter->second;
			for (auto role_iter = cur_list.begin(); role_iter != cur_list.end();)
			{
				if (*role_iter == nullptr)
				{
					cur_list.erase(role_iter++);
					continue;
				}
				if ((time_new - (*role_iter)->get_sign_up_time()) >= m_config_match_time_limit)
				{
					(*role_iter)->set_pk_state(e_cross_ladder_role_no_sign_up);
					send_sign_up_result((*role_iter)->get_role_guid(), e_cross_ladder_sign_up_time_out, (*role_iter)->get_data_array_info(e_cross_ladder_role_info_server_id));
					cur_list.erase(role_iter++);
				}
				else
					break;
			}
			if (cur_list.size() == 0)
				continue;
			if (cur_list.size() >= 2)
			{
				bool complete = false;
				auto first_role_ptr = cur_list.begin();
				auto second_role_ptr = cur_list.begin();
				while ((++second_role_ptr) != cur_list.end())
				{
					if ((*first_role_ptr)->get_previous_oppenent() != (*second_role_ptr)->get_role_guid() || (*first_role_ptr)->get_role_guid() != (*second_role_ptr)->get_previous_oppenent())
					{
						match_success(*first_role_ptr, *second_role_ptr);
						cur_list.erase(first_role_ptr);
						cur_list.erase(second_role_ptr);
						complete = true;
						break;
					}
				}
				if (complete)
					continue;
			}

			LadderTemplate* template_ptr = GET_TEMPLATE(LadderTemplate, iter->first);
			if (nullptr == template_ptr)
				continue;

			auto first_role_iter = cur_list.begin();
			c_cross_ladder_role_info* second_role_ptr = nullptr;
			int32 cur_segment = iter->first;
			int32 upper_limit = 0;
			int32 lower_limit = 0;
			get_expend_range(**first_role_iter, time_new, template_ptr->ExpandTime * second_tick_time, upper_limit, lower_limit);
			if (cur_segment == upper_limit && cur_segment == lower_limit)
				continue;

			for (auto min_key = cur_segment - 1, max_key = cur_segment + 1; min_key >= lower_limit || max_key <= upper_limit;)
			{
				if (min_key >= lower_limit)
				{
					second_role_ptr = get_first_valid_role(min_key, time_new, *first_role_iter);
					if (nullptr != second_role_ptr)
						break;
					--min_key;
				}
				if (max_key <= upper_limit)
				{
					second_role_ptr = get_first_valid_role(min_key, time_new, *first_role_iter);
					if (nullptr != second_role_ptr)
						break;
					++max_key;
				}
			}
			if (nullptr != second_role_ptr)
			{
				match_success(*first_role_iter, second_role_ptr);
				cur_list.erase(first_role_iter);
			}
			else
			{
				//尝试匹配机器人
				if (rand() % 100 < template_ptr->RobotRate)
				{
					if (match_with_robot(*first_role_iter, template_ptr->RobotTemplateID))
					{
						cur_list.erase(first_role_iter);
					}
				}
			}
		}
	}

	void cross_ladder_ws_mgr_new::match_success(c_cross_ladder_role_info* first_role, c_cross_ladder_role_info* second_role)
	{
		if (nullptr == first_role || nullptr == second_role)
			return;
		first_role->reduce_ticket();
		first_role->set_pk_state(e_cross_ladder_role_join_war);
		first_role->set_previous_opponent(second_role->get_role_guid());
		second_role->reduce_ticket();
		second_role->set_pk_state(e_cross_ladder_role_join_war);
		second_role->set_previous_opponent(first_role->get_role_guid());

		int32 first_server_id = first_role->get_data_array_info(e_cross_ladder_role_info_server_id);
		int32 second_server_id = second_role->get_data_array_info(e_cross_ladder_role_info_server_id);
		int32 target_server_id = first_server_id;
		if (m_server_online_num_map[first_server_id] > m_server_online_num_map[second_server_id])
			target_server_id = second_server_id;

		ws2ws_cross_ladder_create_map msg;
		memcpy(msg.match_info.role_info[0].role_name, first_role->get_role_name(), max_name_size);
		memcpy(msg.match_info.role_info[0].server_name, first_role->get_server_name(), max_name_size);
		msg.match_info.role_info[0].role_guid = first_role->get_role_guid();
		msg.match_info.role_info[0].server_id = first_role->get_data_array_info(e_cross_ladder_role_info_server_id);
		msg.match_info.role_info[0].template_id = first_role->get_data_array_info(e_cross_ladder_role_info_template_id);
		msg.match_info.role_info[0].ladder_score = first_role->get_data_array_info(e_cross_ladder_role_info_ladder_score);
		msg.match_info.role_info[0].total_join_num = first_role->get_data_array_info(e_cross_ladder_role_info_total_join_num);
		msg.match_info.role_info[0].total_win_num = first_role->get_data_array_info(e_cross_ladder_role_info_total_win_num);
		msg.match_info.role_info[0].last_win_num = first_role->get_data_array_info(e_cross_ladder_role_info_last_win_num);
		memcpy(msg.match_info.role_info[0].role_server_out_ip, first_role->get_server_out_ip().c_str(), max_ip_address_length);
		msg.match_info.role_info[0].role_server_out_port = first_role->get_server_out_port();

		memcpy(msg.match_info.role_info[1].role_name, second_role->get_role_name(), max_name_size);
		memcpy(msg.match_info.role_info[1].server_name, second_role->get_server_name(), max_name_size);
		msg.match_info.role_info[1].role_guid = second_role->get_role_guid();
		msg.match_info.role_info[1].server_id = second_role->get_data_array_info(e_cross_ladder_role_info_server_id);
		msg.match_info.role_info[1].template_id = second_role->get_data_array_info(e_cross_ladder_role_info_template_id);
		msg.match_info.role_info[1].ladder_score = second_role->get_data_array_info(e_cross_ladder_role_info_ladder_score);
		msg.match_info.role_info[1].total_join_num = second_role->get_data_array_info(e_cross_ladder_role_info_total_join_num);
		msg.match_info.role_info[1].total_win_num = second_role->get_data_array_info(e_cross_ladder_role_info_total_win_num);
		msg.match_info.role_info[1].last_win_num = second_role->get_data_array_info(e_cross_ladder_role_info_last_win_num);
		memcpy(msg.match_info.role_info[1].role_server_out_ip, second_role->get_server_out_ip().c_str(), max_ip_address_length);
		msg.match_info.role_info[1].role_server_out_port = second_role->get_server_out_port();

		cross::send_msg_to_ws(guid_64(), target_server_id, e_msgindex_ws2ws_cross_ladder_req_create_map, &msg, sizeof(msg));
	}

	bool cross_ladder_ws_mgr_new::match_with_robot(c_cross_ladder_role_info* role_info, const std::vector<int32>& robot_template_id)
	{
		if (nullptr == role_info || robot_template_id.empty())
			return false;

		int32 server_id = role_info->get_data_array_info(e_cross_ladder_role_info_server_id);
		int32 ladder_score = role_info->get_data_array_info(e_cross_ladder_role_info_ladder_score);

		bool is_en_code = false;
		RegionTemplate* region_template_ptr = GET_TEMPLATE(RegionTemplate, 150000000);
		if (region_template_ptr != nullptr)
		{
			is_en_code = region_template_ptr->RegionCode == e_version_region_type_en ? true : false;
		}
		int32 region_code = region_template_ptr->RegionCode;

		int32 min_reduce_score = 0;
		int32 max_reduce_score = GAMECONFIG->ScoreReduce;

		std::vector<ui64> enemy_guid_list;//从该组里随机抽一个打
		enemy_guid_list.clear();
		std::vector<ui64> server_guid_list;//相同服务器数组
		server_guid_list.clear();
		auto iter = m_all_ladder_info_map.begin();

		for (int32 count = 0; count < 10; count++)
		{
			for (; iter != m_all_ladder_info_map.end(); ++iter)
			{
				if (iter->second.get_role_guid() == role_info->get_role_guid())
					continue;
				if ((abs(ladder_score - iter->second.get_data_array_info(e_cross_ladder_role_info_ladder_score)) < max_reduce_score
					&& abs(ladder_score - iter->second.get_data_array_info(e_cross_ladder_role_info_ladder_score)) >= min_reduce_score)
					&& (role_info->get_previous_oppenent() != iter->second.get_role_guid() || is_en_code))
				{
					ui64 guid_64 = iter->first;
					if (iter->second.get_data_array_info(e_cross_ladder_role_info_server_id) != server_id || is_en_code)
					{
						enemy_guid_list.push_back(guid_64);
					}
					else if (count == 9) //第九次的时候记录所有可攻击角色数据
					{
						server_guid_list.push_back(guid_64);
					}

				}
				if (enemy_guid_list.size() >= 10)
				{
					break;
				}
			}
			if (enemy_guid_list.size() >= 10)
			{
				break;
			}
			min_reduce_score += GAMECONFIG->ScoreReduce;
			max_reduce_score += GAMECONFIG->ScoreReduce;
		}
		if (enemy_guid_list.size() <= 0)
		{
			enemy_guid_list = server_guid_list;
			if (enemy_guid_list.size() <= 0)
			{
				return false;
			}
		}
		int32 random_index = random_gen::get_random(0, enemy_guid_list.size() - 1);
		if (random_index >= enemy_guid_list.size() || random_index < 0)
		{
			random_index = 0;
		}
		ui64 enemy_guid64 = enemy_guid_list[random_index];
		iter = m_all_ladder_info_map.find(enemy_guid64);
		enemy_guid_list.clear();
		if (iter == m_all_ladder_info_map.end())
			return false;
		auto& robot_role_info = iter->second;
		auto player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, robot_role_info.get_data_array_info(e_cross_ladder_role_info_template_id));
		if (player_init_template_ptr == nullptr || player_init_template_ptr->Class > robot_template_id.size())
			return false;

		ws2ws_cross_ladder_create_map msg;
		msg.match_info.robot_template_id = robot_template_id[player_init_template_ptr->Class - 1];

		memcpy(msg.match_info.role_info[0].role_name, role_info->get_role_name(), max_name_size);
		msg.match_info.role_info[0].role_guid = role_info->get_role_guid();
		msg.match_info.role_info[0].server_id = role_info->get_data_array_info(e_cross_ladder_role_info_server_id);

		memcpy(msg.match_info.role_info[1].role_name, robot_role_info.get_role_name(), max_name_size);
		memcpy(msg.match_info.role_info[1].server_name, robot_role_info.get_server_name(), max_name_size);
		//msg.match_info.role_info[1].role_guid = robot_role_info.get_role_guid();
		msg.match_info.role_info[1].server_id = robot_role_info.get_data_array_info(e_cross_ladder_role_info_server_id);
		msg.match_info.role_info[1].template_id = robot_role_info.get_data_array_info(e_cross_ladder_role_info_template_id);
		msg.match_info.role_info[1].ladder_score = robot_role_info.get_data_array_info(e_cross_ladder_role_info_ladder_score);
		msg.match_info.role_info[1].total_join_num = robot_role_info.get_data_array_info(e_cross_ladder_role_info_total_join_num);
		msg.match_info.role_info[1].total_win_num = robot_role_info.get_data_array_info(e_cross_ladder_role_info_total_win_num);
		msg.match_info.role_info[1].last_win_num = robot_role_info.get_data_array_info(e_cross_ladder_role_info_last_win_num);
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_cross_ladder_req_create_map, &msg, sizeof(msg));

		role_info->reduce_ticket();
		role_info->set_pk_state(e_cross_ladder_role_join_war);
		role_info->set_previous_opponent(iter->second.get_role_guid());
		return true;
	}

	int32 cross_ladder_ws_mgr_new::get_ladder_segment_value(const c_cross_ladder_role_info& info)
	{
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(info.get_data_array_info(e_cross_ladder_role_info_ladder_score));
		if (nullptr != ladder_template_ptr)
		{
			return ladder_template_ptr->attribute_id;
		}
		return 0;
	}

	void cross_ladder_ws_mgr_new::get_expend_range(const c_cross_ladder_role_info& info, int64 time_new, int32 expend_time, int32& upper_limit, int32& lower_limit)
	{
		int32 cur_segment_value = get_ladder_segment_value(info);
		int32 expand_value = (time_new - info.get_sign_up_time()) / expend_time;
		int32 cur_last_win = info.get_data_array_info(e_cross_ladder_role_info_last_win_num);
		if (cur_last_win > 0)
		{
			upper_limit = cur_segment_value + (expand_value + 1) / 2;
			lower_limit = cur_segment_value - expand_value / 2;
		}
		else
		{
			upper_limit = cur_segment_value + expand_value / 2;
			lower_limit = cur_segment_value - (expand_value + 1) / 2;
		}
	}

	c_cross_ladder_role_info* cross_ladder_ws_mgr_new::get_first_valid_role(int32 segment_key, int64 time_new, c_cross_ladder_role_info* cur_role_ptr)
	{
		if (nullptr == cur_role_ptr)
			return nullptr;
		auto iter = m_all_sign_up_map.find(segment_key);
		if (iter != m_all_sign_up_map.end())
		{
			auto& cur_list = iter->second;
			for (auto role_iter = cur_list.begin(); role_iter != cur_list.end();)
			{
				if (*role_iter == nullptr)
					cur_list.erase(role_iter++);
				else if ((time_new - (*role_iter)->get_sign_up_time()) >= m_config_match_time_limit)
				{
					(*role_iter)->set_pk_state(e_cross_ladder_role_no_sign_up);
					send_sign_up_result((*role_iter)->get_role_guid(), e_cross_ladder_sign_up_time_out, (*role_iter)->get_data_array_info(e_cross_ladder_role_info_server_id));
					cur_list.erase(role_iter++);
				}
				else
				{
					if (cur_role_ptr->get_previous_oppenent() == (*role_iter)->get_role_guid() && cur_role_ptr->get_role_guid() == (*role_iter)->get_previous_oppenent())
						++role_iter;
					else
					{
						cur_list.erase(role_iter);
						return *role_iter;
					}
				}
			}
		}
		return nullptr;
	}

	void cross_ladder_ws_mgr_new::deal_with_pk_result_from_ws(bool draw, const guid_64& winner_guid, const guid_64& loser_guid)
	{
		auto winner_iter = m_all_ladder_info_map.find(winner_guid);
		auto loser_iter = m_all_ladder_info_map.find(loser_guid);
		if (draw)
		{
			if (winner_iter != m_all_ladder_info_map.end())
			{
				if (m_is_in_activity_time) winner_iter->second.game_peace();
				winner_iter->second.set_pk_state(e_cross_ladder_role_no_sign_up);
			}
			if (loser_iter != m_all_ladder_info_map.end())
			{
				if (m_is_in_activity_time) loser_iter->second.game_peace();
				loser_iter->second.set_pk_state(e_cross_ladder_role_no_sign_up);
			}
		}
		else
		{
			if (winner_iter != m_all_ladder_info_map.end())
			{
				if (m_is_in_activity_time) winner_iter->second.game_win();
				winner_iter->second.set_pk_state(e_cross_ladder_role_no_sign_up);
			}
			if (loser_iter != m_all_ladder_info_map.end())
			{
				if (m_is_in_activity_time) loser_iter->second.game_lose();
				loser_iter->second.set_pk_state(e_cross_ladder_role_no_sign_up);
			}
		}
	}

	void cross_ladder_ws_mgr_new::reset_role_state(const guid_64& role_guid)
	{
		auto iter = m_all_ladder_info_map.find(role_guid);
		if (iter != m_all_ladder_info_map.end())
		{
			iter->second.set_pk_state(e_cross_ladder_role_no_sign_up);
			iter->second.add_ticket_num();
		}
	}
}