#include "../mail/event_ws_mgr.h"
#include "../mail/mail_event_ws.h"
#include "../ranking/ranking_mgr_ws.h"
#include "broken_sky_mgr.h"
#include "game.pb.h"
#include "globle_data.h"
#include "Logic/activity_def.hpp"
#include "Logic/time_def.hpp"
#include "net.pb.h"
#include "utility/init_unit.h"
#include "world_server.hpp"

namespace faith
{
	broken_sky_mgr::broken_sky_mgr()
	{
		m_game_state = e_broken_sky_not_start;
		m_boss_level = -1;
		clear_data();
		if (nullptr == m_brokensky_config_ptr)
		{
			m_brokensky_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_broken_sky);
		}
	}

	void broken_sky_mgr::tick(const int64& new_time)
	{
		static int64 last_tick_time = new_time;
		float elapse_sec = (float)(new_time - last_tick_time) / second_tick_time;
		last_tick_time = new_time;

		switch (m_game_state)
		{
		case e_broken_sky_not_start:
			if (is_in_join_time() || is_in_game_time())
			{
				m_game_over_flag = false;
				m_game_state = e_broken_sky_in_join;
			}
			break;
		case e_broken_sky_in_join:
			if (is_in_game_time())
			{
				m_game_state = e_broken_sky_in_game;
			}
			break;
		case e_broken_sky_in_game:
			if (false == is_in_game_time() || m_is_boss_dead == true)
			{
				m_game_state = e_broken_sky_game_end;
				send_settlement_to_all_player();
				if (m_is_boss_dead)
				{
					m_boss_level = -1;
				}
				world_server::getInstance().save_server_attr_val_db(e_srv_attr_val_broken_sky_boss_level_last, m_boss_level);			
				clear_data();
			}
			break;
		case e_broken_sky_game_end:
			if (false == is_in_game_time())
			{
				m_game_state = e_broken_sky_not_start;
			}
			break;
		default:
			break;
		}

		m_tick_3sec += elapse_sec;
		if (m_tick_3sec >= 3.f)
		{
			m_tick_3sec = 0.f;
			tick_3sec();
		}
	}

	void broken_sky_mgr::tick_3sec()
	{
		send_top_records();
	}

	void broken_sky_mgr::send_top_records()
	{
		if (m_game_state == e_broken_sky_not_start)
		{
			return;
		}
		int32 damage_list_length = m_damage_rank_indicator.get_score_recore_num();
		if (damage_list_length <= 0)
		{
			return;
		}

		std::vector<client_session*> fep_session_data;
		client_session* all_session_map = client_session_mgr::getInstance().get_all_session();
		for (int32 i = 0; i <= client_session_mgr::getInstance().get_session_array_used(); ++i)
 		{
 			client_session& client_session_ref = all_session_map[i];
 			if (client_session_ref.is_vaild() == false)
 			{
				continue;
 			}
			// 判断是否是在破碎虚空的地图
			int32 role_in_map_id = client_session_ref.m_role_info.data_ary[e_role_info_move_map_id];
			if (is_broken_sky_map_id(role_in_map_id) == false)
			{
				continue;
			}
			fep_session_data.push_back(&client_session_ref);
		}

		int32 session_num = fep_session_data.size();
		if (session_num <= 0)
		{
			return;
		}

		m_damage_rank_indicator.sort();
		game_proto_score_record_list score_record_list;
		int32 to_client_top_num = 5;
		m_damage_rank_indicator.fill_score_record_list_msg(score_record_list, to_client_top_num);

		for (int32 j = 0; j < session_num; ++j)
		{
			client_session* client_session_ptr = fep_session_data[j];
			if (nullptr == client_session_ptr)
			{
				continue;
			}
			score_record_list.clear_extra_score();
			score_record_list.clear_extra_rank();
			if (nullptr == m_damage_rank_indicator.get_score_record(client_session_ptr->get_role_guid()))
			{
				client_session_ptr->send_to_client(&score_record_list, e_msgindex_s2c_score_record_list);
			}
			else
			{
				m_damage_rank_indicator.fill_extra_score_record_msg(score_record_list, client_session_ptr->get_role_guid());
				client_session_ptr->send_to_client(&score_record_list, e_msgindex_s2c_score_record_list);
			}
		}
	}

	void broken_sky_mgr::clear_data()
	{
		m_tick_3sec = 0.0f;
		m_damage_rank_indicator.clear_data();
		m_all_damage = 0;
		m_is_boss_dead = false;
		m_game_over_flag = true;
	}

	void broken_sky_mgr::compare_and_update_top_records(int32 map_id, const score_record* top_record_arr, int32 record_num)
	{
		for (int32 i = 0; i < record_num; ++i)
		{
			if (!top_record_arr[i].role_guid.is_valid())
			{
				break;
			}
			score_record* player_record = m_damage_rank_indicator.get_score_record(top_record_arr[i].role_guid);
			if (nullptr == player_record)
			{
				m_damage_rank_indicator.add_score_record(top_record_arr[i].role_guid, top_record_arr[i].role_name, top_record_arr[i].role_level, top_record_arr[i].score);
			}
			else
			{
				player_record->score = top_record_arr[i].score;
			}
		}
	}
	bool broken_sky_mgr::get_game_over_flag()
	{
		return m_game_over_flag;
	}

	e_error_code broken_sky_mgr::is_can_join()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_broken_sky, e_activity_time_get_all);
		if (act_left < 0)
		{
			return e_error_code_map_broken_sky_not_begin;
		}
		
		if (get_game_over_flag() == true)
		{
			return e_error_code_map_broken_sky_game_over;
		}
		
		return e_error_code_success;
	}

	void broken_sky_mgr::sync_game_info_to_all_map_inst(int32 map_id, guid_64 from_map_inst_guid, int64 damage, const score_record* top_record_arr, int32 record_num)
	{
		if (get_game_over_flag() == true)
		{
			return;
		}

		// 更新对应的分数榜
		compare_and_update_top_records(map_id, top_record_arr, record_num);
		m_all_damage += damage;

		ws2cs_broken_sky_info_sync broken_sky_info_sync_msg;
		broken_sky_info_sync_msg.map_template_id = map_id;
		broken_sky_info_sync_msg.broken_sky_inst_guid = from_map_inst_guid;
		broken_sky_info_sync_msg.damage = m_all_damage;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_broken_sky_info_sync);
		if (is_use == false)
		{
			send_message_to_all_cs(&broken_sky_info_sync_msg, sizeof(broken_sky_info_sync_msg));
		}
		else
		{
			faith::ws2cs_proto::broken_sky_info_sync pro_msg;
			pro_msg.set_map_template_id(map_id);
			pro_msg.set_broken_sky_inst_guid(from_map_inst_guid.server_64);
			pro_msg.set_damage(m_all_damage);

			world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_broken_sky_info_sync, e_server_type_cs);

		}
	}

	void broken_sky_mgr::sync_kill_boss_to_all_map_inst(int32 map_id, int32 killer_unit_index, guid_64 killer_guid)
	{
		if (m_is_boss_dead || m_game_over_flag)
		{
			return;
		}
		send_top_records();
		m_is_boss_dead = true;

		ws2cs_broken_sky_kill_boss broken_sky_kill_boss_msg;
		broken_sky_kill_boss_msg.map_template_id = map_id;
		broken_sky_kill_boss_msg.killer_unit_index = killer_unit_index;
		broken_sky_kill_boss_msg.killer_guid = killer_guid;

		bool is_use = proto_by_lua(e_msg_index_ws2cs_broken_sky_kill_boss);
		if (is_use == false)
		{
			send_message_to_all_cs(&broken_sky_kill_boss_msg, sizeof(broken_sky_kill_boss_msg));
		}
		else
		{
			faith::ws2cs_proto::broken_sky_kill_boss pro_msg;
			pro_msg.set_map_template_id(map_id);
			pro_msg.set_killer_unit_index(killer_unit_index);
			pro_msg.set_killer_guid(killer_guid.server_64);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_broken_sky_kill_boss, e_server_type_cs);

		}
	}

	bool broken_sky_mgr::is_broken_sky_map_id(int32 map_id)
	{
		return broken_sky_map_template_id == map_id;
	}

	void broken_sky_mgr::send_message_to_all_cs(const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr || data_len == 0)
		{
			return;
		}
		world_server::getInstance().broadcast(data_ptr, data_len, e_server_type_cs);
	}

	bool broken_sky_mgr::is_in_join_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_broken_sky, e_activity_time_get_ready);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	bool broken_sky_mgr::is_in_game_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_broken_sky, e_activity_time_get_gaming);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	void broken_sky_mgr::send_settlement_to_all_player()
	{
		bool is_set = false;
		guid_64 target_guid;
		xstring target_name = "";
		m_damage_rank_indicator.sort();
		int32 damage_list_length = m_damage_rank_indicator.get_score_recore_num();
		for (int32 player_rank_idx = 0; player_rank_idx < damage_list_length; player_rank_idx++)
		{
			score_record* player_record_ptr = m_damage_rank_indicator.get_score_record_at(player_rank_idx);
			if (nullptr == player_record_ptr)
			{
				break;
			}
			give_rank_rewards(player_record_ptr->role_guid, player_rank_idx + 1);
			if (!is_set)
			{
				is_set = true;
				target_name = player_record_ptr->role_name;
				target_guid = player_record_ptr->role_guid;
			}
		}

		if (is_set)
		{
			send_top_player_notice(target_guid, target_name);
		}
	}
	void broken_sky_mgr::send_top_player_notice(const guid_64& target_guid, const xstring& target_name)
	{
		int32 notice_id = 93000145;
		int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(target_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, target_guid, notice_str);
	}

	void broken_sky_mgr::give_rank_rewards(guid_64 player_guid, int32 player_rank)
	{
		if (nullptr == m_brokensky_config_ptr)
		{
			return;
		}
		
		std::vector<s_item_template_info> drop_item_list;
		std::vector<int32>& rwd_data = m_brokensky_config_ptr->ParamIntArr3;
		globle_data::get_instance().get_item_list_by_rank_rwd(player_rank, rwd_data, drop_item_list, ranking_mgr_ws::get_world_level_last());

		if (drop_item_list.size() <= 0)
		{
			return;
		}
		score_record* cur_player_record = m_damage_rank_indicator.get_score_record(player_guid);
		if (nullptr == cur_player_record)
		{
			return;
		}

		score_record* num_1_player = m_damage_rank_indicator.get_score_record_at(0);
		if (nullptr == num_1_player)
		{
			return;
		}

		std::string role_name1 = num_1_player->role_name;
		std::string role_name2 = "none";
		std::string role_name3 = "none";
		std::string cur_player_name = cur_player_record->role_name;
		std::string role_score1 = init_unit::change_i64_to_string(num_1_player->score);
		std::string role_score2 = "0";
		std::string role_score3 = "0";
		std::string cur_player_score = init_unit::change_i64_to_string(cur_player_record->score);
		std::string cur_player_rank = init_unit::change_i64_to_string(player_rank);

		score_record* num_2_player = m_damage_rank_indicator.get_score_record_at(1);
		if (nullptr != num_2_player)
		{
			role_name2 = num_2_player->role_name;
			role_score2 = init_unit::change_i64_to_string(num_2_player->score);
		}
		score_record* num_3_player = m_damage_rank_indicator.get_score_record_at(2);
		if (nullptr != num_3_player)
		{
			role_name3 = num_3_player->role_name;
			role_score3 = init_unit::change_i64_to_string(num_3_player->score);
		}

		std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_broken_sky_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_broken_sky_context));
		content_params.push_back(role_name1);
		content_params.push_back(role_score1);
		content_params.push_back(role_name2);
		content_params.push_back(role_score2);
		content_params.push_back(role_name3);
		content_params.push_back(role_score3);
		content_params.push_back(cur_player_name);
		content_params.push_back(cur_player_rank);
		content_params.push_back(cur_player_score);
		std::string contenttext = init_unit::implode(content_params);

		int32 upgrade_id = player_upgrade_cfg_first_id + cur_player_record->role_level - 1;
		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_id);
		if (nullptr == upgrade_template_ptr)
		{
			return;
		}

		RewardSettlementTemplate* reward_template_ptr = template_manager::get_instance().get_template_by_activity_score(e_activity_type_broken_sky, cur_player_record->score);
		if (nullptr == reward_template_ptr)
		{
			return;
		}

		int32 score_award_money = upgrade_template_ptr->BaseCoin * reward_template_ptr->Coinrewards;
		int32 score_award_exp = upgrade_template_ptr->BaseExp  * reward_template_ptr->Experiencerewardfactor;

		mail_ws_mgr::get_instance().send_mail_system(player_guid, 0, drop_item_list, title, contenttext, e_money_type_exp, score_award_exp, e_money_type_silver_bind, score_award_money);
	}

	void broken_sky_mgr::set_boss_level(int32 boss_level)
	{
		if (boss_level <= 0)
		{
			return;
		}
		m_boss_level = boss_level;
	}

	int32 broken_sky_mgr::get_boss_level()
	{
		if (m_boss_level <= 0)
		{
			calcu_boss_level();
		}

		return m_boss_level;
	}

	void broken_sky_mgr::calcu_boss_level()
	{
		if (nullptr == m_brokensky_config_ptr || m_brokensky_config_ptr->ParamIntArr1.size() < 2)
		{
			return;
		}

		if (m_boss_level > 0)
		{
			return;
		}

		m_boss_level = ranking_mgr_ws::get_srv_avg_level();
	}

	void broken_sky_mgr::sync_creat_boss_to_cs(guid_64 from_map_guid)
	{
		ws2cs_broken_sky_creat_boss broken_sky_creat_boss_msg;
		broken_sky_creat_boss_msg.boss_level = get_boss_level();
		broken_sky_creat_boss_msg.all_damage = m_all_damage;
		broken_sky_creat_boss_msg.from_map_guid = from_map_guid;
		
		if (m_is_boss_dead || m_game_over_flag)
		{
			broken_sky_creat_boss_msg.boss_level = -1; //表明创建副本时活动还在 副本内要创建boss时活动结束
		}
		bool is_use = proto_by_lua(e_msg_index_ws2cs_broken_sky_creat_boss);
		if (is_use == false)
		{
			send_message_to_all_cs(&broken_sky_creat_boss_msg, sizeof(broken_sky_creat_boss_msg));
		}
		else
		{
			faith::ws2cs_proto::broken_sky_creat_boss pro_msg;
			pro_msg.set_boss_level(get_boss_level());
			pro_msg.set_all_damage(m_all_damage);
			pro_msg.set_from_map_guid(from_map_guid.server_64);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_broken_sky_creat_boss, e_server_type_cs);
		}
	}

	void broken_sky_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		std::vector<score_record>& socre_record = m_damage_rank_indicator.get_score_record_list_all();
		auto iter_record = socre_record.begin();
		for (; iter_record != socre_record.end(); ++iter_record)
		{
			if (iter_record->role_guid == role_guid)
			{
				xchar char_role_name[max_name_size + 1] = { 0 };
				memcpy(char_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
				iter_record->set_role_name(char_role_name);
				break;
			}
		}
	}
}