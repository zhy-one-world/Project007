
#include "cross_arena_island.h"
#include "map_object.h"
#include "connection_mgr.hpp"
#include "Logic/cross_server_pk_def.hpp"
#include "server_log.hpp"
#include "cell_server.hpp"
#include "../unit_man.h"
#include "../player.hpp"
#include "utility/init_unit.h"
#include "utility/globle_data.h"
#include "internal/cross_server_pk_msg.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "internal/core.hpp"

namespace faith
{
	bool cross_arena_island::is_vaild()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (true == m_island_player_msg[i].player_guid.is_valid())
			{
				return true;
			}
		}
		return false;
	}

	void cross_arena_island::clear_data()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			m_island_player_msg[i].clear_data();
		}
		m_is_in_the_war = false;
		m_pk_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_pk);
		m_need_winer_num = 2;
		if (nullptr != m_pk_config_ptr)
		{
			m_small_free_time = m_pk_config_ptr->ParamInt2;
			m_war_time = m_pk_config_ptr->ParamInt4;
		}
		m_next_begin_war_time = 0;
		m_1v1_pk_state = e_1v1_pk_state_ready;
		m_next_ready_war_time = 0;
		m_winner_player_guid = guid_64();
		m_island_idex = -1;
		m_cur_island_class_type = -1;
		m_is_pk_count = 0;
		m_end_war_transfer_time = 0;
	}

	void cross_arena_island::set_island_msg(score_record& cur_score, s_map_pos born_pos, int32 score)
	{
		if (true == m_island_player_msg[0].player_guid.is_valid())
		{
			m_island_player_msg[1].player_born_pos = born_pos;
			m_island_player_msg[1].player_guid = cur_score.role_guid;
			m_island_player_msg[1].set_role_name(cur_score.role_name);
			m_island_player_msg[1].win_num = 0;
			m_island_player_msg[1].score = score;
			m_island_player_msg[1].server_id = cur_score.server_id;
		}
		else
		{
			m_island_player_msg[0].player_born_pos = born_pos;
			m_island_player_msg[0].player_guid = cur_score.role_guid;
			m_island_player_msg[0].set_role_name(cur_score.role_name);
			m_island_player_msg[0].win_num = 0;
			m_island_player_msg[0].score = score;
			m_island_player_msg[0].server_id = cur_score.server_id;
		}
	}

	void cross_arena_island::tick(const int64& new_time)
	{
		if (false == m_is_in_the_war)
		{
			if (m_1v1_pk_state == e_1v1_pk_state_max && m_end_war_transfer_time > 0)
			{
				if (new_time > m_end_war_transfer_time)
				{
					m_end_war_transfer_time = 0;
					if (!not_have_other_one())
					{
						for (int32 i = 0; i < max_island_player_num; i++)
						{
							player& temp_player = unit_man::get_player(m_island_player_msg[i].player_guid);
							if (temp_player.is_valid() && false == temp_player.is_dead())
							{
								temp_player.transfer_by_template_pos(temp_player.get_unit_info(e_role_info_move_map_id), 0, m_cur_map_ptr->get_born_pos(temp_player.get_array_index()), temp_player.get_map_guid());
							}
							if (temp_player.is_valid() && true == temp_player.is_dead())
							{
								temp_player.back_to_life(e_revive_type_born_pos);
							}
						}
					}
				}
			}
			return;
		}
		if (m_next_begin_war_time > 0)
		{
			if (new_time > m_next_begin_war_time && m_1v1_pk_state == e_1v1_pk_state_ready)
			{
				start_pk();
			}
			if (new_time > m_next_begin_war_time + m_war_time * second_tick_time && m_1v1_pk_state == e_1v1_pk_state_begin)
			{
				reset_war(true);
			}
		}
		else if (m_next_ready_war_time > 0)
		{
			if (new_time > m_next_ready_war_time && m_1v1_pk_state == e_1v1_pk_state_end)
			{
				end_one_war();
			}
		}
	}

	s_pk_island_player_msg cross_arena_island::get_player_msg_for_guid(guid_64 role_guid)
	{
		s_pk_island_player_msg player_msg;
		player_msg.clear_data();
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (m_island_player_msg[i].player_guid == role_guid)
			{
				player_msg = m_island_player_msg[i];
				break;
			}
		}
		return player_msg;
	}

	int32 cross_arena_island::get_cur_island_war_idex()
	{
		int32 war_idex = m_island_idex;
		int32 class_type = m_cur_island_class_type;
		if (war_idex < 0 || class_type < 0 || war_idex >= max_cross_server_all_class_war_count || class_type >= max_branch_type)
		{
			return -1;
		}
		war_idex = war_idex + class_type * max_cross_server_war_count;
		switch (m_pk_state)
		{
		case faith::e_cross_pk_state_16:
			break;
		case faith::e_cross_pk_state_8:
			war_idex += 8;
			break;
		case faith::e_cross_pk_state_4:
			war_idex += 12;
			break;
		case faith::e_cross_pk_state_2:
			war_idex += 14;
			break;
		default:
			war_idex = -1;
			break;
		}
		return war_idex;
	}

	void cross_arena_island::fill_simple_msg(s_pk_island_simple_msg& simple_msg)
	{
		simple_msg.pk_state = m_pk_state;
		guid_64 one_player_guid = m_island_player_msg[0].player_guid;
		//player& player_one_ref = unit_man::get_player(one_player_guid);
		//if (player_one_ref.is_valid() == false)
		//{
		//	return;
		//}
		int32 war_idex = get_cur_island_war_idex();
		simple_msg.war_idex = war_idex;

		s_pk_island_player_msg player_msg = get_player_msg_for_guid(one_player_guid);
		simple_msg.island_player_msg[0].player_guid = one_player_guid;
		simple_msg.island_player_msg[0].set_role_name(m_island_player_msg[0].role_name);
		simple_msg.island_player_msg[0].server_id = m_island_player_msg[0].server_id;
		simple_msg.island_player_msg[0].win_num = player_msg.win_num;
		simple_msg.island_player_msg[0].is_winner = (one_player_guid == m_winner_player_guid ? 1 : 0);

		guid_64 other_player_guid = m_island_player_msg[1].player_guid;
		if (other_player_guid.is_valid())
		{
			//player& player_two_ref = unit_man::get_player(other_player_guid);
			//if (player_two_ref.is_valid() == true)
			//{
				s_pk_island_player_msg player_lose_msg = get_player_msg_for_guid(other_player_guid);
				simple_msg.island_player_msg[1].player_guid = other_player_guid;
				simple_msg.island_player_msg[1].set_role_name(m_island_player_msg[1].role_name);
				simple_msg.island_player_msg[1].server_id = m_island_player_msg[1].server_id;
				simple_msg.island_player_msg[1].win_num = player_lose_msg.win_num;
				simple_msg.island_player_msg[1].is_winner = (other_player_guid == m_winner_player_guid ? 1 : 0);
			//}
		}	
	}

	void cross_arena_island::end_one_war()
	{
		check_is_need_end_war();
		m_1v1_pk_state = e_1v1_pk_state_ready;
		transfer_player_to_born_pos();
		m_next_begin_war_time = time_helper::get_cur_time_new().millisecond + m_small_free_time * second_tick_time;
		m_next_ready_war_time = 0;
		send_client_map_state();
	}

	int32 cross_arena_island::reset_war(bool is_time_out)
	{
		m_1v1_pk_state = e_1v1_pk_state_end;
		set_pk_type(e_pk_mode_type_cant, e_pk_mode_peace);
		m_next_begin_war_time = 0;
		m_next_ready_war_time = time_helper::get_cur_time_new().millisecond + m_small_free_time * second_tick_time;
		send_client_map_state();
		m_is_pk_count++;
		if (m_is_pk_count >= m_need_winer_num * 2 - 1)
		{
			end_war();
			return 1;
		}
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (m_island_player_msg[i].win_num >= m_need_winer_num)
			{
				end_war();
				return 1;
			}
		}

		if (true == is_time_out && true == m_is_in_the_war)
		{
			send_ws_msg_time_out();
		}
		return 0;
	}

	void cross_arena_island::start_pk()
	{
		reset_player_state();
		m_1v1_pk_state = e_1v1_pk_state_begin;
		set_pk_type(e_pk_mode_type_no_kill_point, e_pk_mode_slaughter);	
		send_client_map_state();
	}

	void cross_arena_island::begin_war()
	{
		m_is_in_the_war = true;
		bool need_end_war = false;
		need_end_war = check_is_need_end_war();
		if (true == need_end_war)
		{
			return;
		}
		transfer_player_to_born_pos();
		m_next_begin_war_time = time_helper::get_cur_time_new().millisecond + m_small_free_time * second_tick_time;
		m_next_ready_war_time = 0;
		send_client_map_state();
	}

	void cross_arena_island::set_pk_type(e_pk_mode_type mode_type, e_pk_mode pk_mode)
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			player& temp_player = unit_man::get_player(m_island_player_msg[i].player_guid);
			temp_player.get_pk_community_mgr().init_map_pk_type(mode_type);
			temp_player.get_pk_community_mgr().set_entire_pk_mode(pk_mode);//这样设置后 反击等选项为0
		}
	}

	void cross_arena_island::transfer_player_to_born_pos()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			player& temp_player = unit_man::get_player(m_island_player_msg[i].player_guid);
			if (temp_player.is_valid() && false == temp_player.is_dead())
			{
				temp_player.transfer_by_template_pos(temp_player.get_unit_info(e_role_info_move_map_id), 0, m_island_player_msg[i].player_born_pos, temp_player.get_map_guid());
			}
			if (temp_player.is_valid() && true == temp_player.is_dead())
			{
				temp_player.back_to_life(e_revive_type_born_pos);
			}
		}
	}

	void cross_arena_island::transfer_player_to_main_island()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			player& temp_player = unit_man::get_player(m_island_player_msg[i].player_guid);
			if (temp_player.is_valid())
			{
				temp_player.transfer_by_template_pos(temp_player.get_unit_info(e_role_info_move_map_id), 0, m_cur_map_ptr->get_born_pos(temp_player.get_array_index()), temp_player.get_map_guid());
			}
		}
	}

	void cross_arena_island::end_war()
	{
		m_1v1_pk_state = e_1v1_pk_state_max;
		set_winner_player_guid();
		send_award_to_winner_player();
		m_is_in_the_war = false;
		send_client_map_state();
		set_pk_type(e_pk_mode_type_cant, e_pk_mode_peace);
		send_ws_msg_time_out();
		send_winner_notice();
		if (nullptr == m_cur_map_ptr)
		{
			return;
		}
		m_end_war_transfer_time = m_small_free_time * second_tick_time + time_helper::get_cur_time_new().millisecond;
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			player& temp_player = unit_man::get_player(m_island_player_msg[i].player_guid);
			if (temp_player.is_valid() && true == temp_player.is_dead())
			{
				temp_player.back_to_life(e_revive_type_born_pos);
			}
		}
	}

	int32 cross_arena_island::set_win_player(guid_64 player_guid)
	{
		int32 idex = -1;
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (player_guid == m_island_player_msg[i].player_guid)
			{
				idex = i;
				m_island_player_msg[i].win_num += 1;
			}
		}
		return reset_war();
	}

	s_map_pos cross_arena_island::get_player_born_pos(guid_64 player_guid)
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (player_guid == m_island_player_msg[i].player_guid)
			{
				return m_island_player_msg[i].player_born_pos;
			}
		}
		return s_map_pos();
	}

	void cross_arena_island::set_winner_player_guid()
	{
		if (m_winner_player_guid.is_valid())
		{
			return;
		}
		if (false == m_island_player_msg[1].player_guid.is_valid())
		{
			m_winner_player_guid = m_island_player_msg[0].player_guid;
			return;
		}
		player& player_one_ref = unit_man::get_player(m_island_player_msg[1].player_guid);
		player& player_two_ref = unit_man::get_player(m_island_player_msg[0].player_guid);
		if (player_one_ref.is_valid() == false || false == player_one_ref.is_in_cross_pk_map())
		{
			m_winner_player_guid = m_island_player_msg[0].player_guid;
			return;
		}
		if (player_two_ref.is_valid() == false || false == player_two_ref.is_in_cross_pk_map())
		{
			m_winner_player_guid = m_island_player_msg[1].player_guid;
			return;
		}
		if (m_island_player_msg[0].win_num != m_island_player_msg[1].win_num)
		{
			if (m_island_player_msg[0].win_num > m_island_player_msg[1].win_num)
			{
				m_winner_player_guid = m_island_player_msg[0].player_guid;
				return;
			}
			else
			{
				m_winner_player_guid = m_island_player_msg[1].player_guid;
				return;
			}
		}
		else if (m_island_player_msg[0].win_num == m_island_player_msg[1].win_num)
		{
			if (m_island_player_msg[0].score != m_island_player_msg[1].score)
			{
				m_winner_player_guid = m_island_player_msg[0].score > m_island_player_msg[1].score ? m_island_player_msg[0].player_guid : m_island_player_msg[1].player_guid;
				return;
			}
			m_winner_player_guid = player_one_ref.get_unit_gs_value() >= player_two_ref.get_unit_gs_value() ? m_island_player_msg[0].player_guid : m_island_player_msg[1].player_guid;
		}
	}

	guid_64 cross_arena_island::get_winner_player_guid()
	{
		return m_winner_player_guid;
	}

	xchar* cross_arena_island::get_winner_player_name()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (m_island_player_msg[i].player_guid == m_winner_player_guid)
			{
				return m_island_player_msg[i].role_name;
			}
		}
		return empty_char_array;
	}

	guid_64 cross_arena_island::get_lose_player_guid()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (m_winner_player_guid != m_island_player_msg[i].player_guid)
			{
				return m_island_player_msg[i].player_guid;
			}
		}
		return guid_64();
	}

	xchar* cross_arena_island::get_lose_player_name()
	{
		for (int32 i = 0; i < max_island_player_num; i++)
		{
			if (m_winner_player_guid != m_island_player_msg[i].player_guid)
			{
				return m_island_player_msg[i].role_name;
			}
		}
		return empty_char_array;
	}

	void cross_arena_island::set_pk_state(e_cross_pk_state pk_state)
	{
		m_pk_state = pk_state;
		if (nullptr == m_pk_config_ptr)
		{
			return;
		}
		int32 idex = (int32)pk_state;
		std::vector<int32> win_num_arr = m_pk_config_ptr->ParamIntArr3;
		if (win_num_arr.size() <= idex)
		{
			return;
		}
		m_need_winer_num = win_num_arr[idex];
	}

	bool cross_arena_island::not_have_other_one()
	{
		bool not_have = false;
		if ((m_island_player_msg[0].player_guid.is_valid()) != (m_island_player_msg[1].player_guid.is_valid()))
		{
			not_have = true;
		}
		return not_have;
	}

	void cross_arena_island::send_winner_notice()
	{
		bool is_only_one_player = false;
		if (m_cur_map_ptr == nullptr)
		{
			return;
		}
		if (false == m_winner_player_guid.is_valid())
		{
			return;
		}
		int32 notice_id = 0;
		int32 class_type_str_id = 0;
		int32 class_type = m_cur_island_class_type + 1;
		int32 pk_state_id = 0;
		switch (class_type)
		{
		case e_class_type_barserker:
			class_type_str_id = 90069724;
			break;
		case e_class_type_wizard:
			class_type_str_id = 90069729;
			break;
		case e_class_type_guardian:
			class_type_str_id = 90069734;
			break;
		case e_class_type_assassinator:
			class_type_str_id = 90069739;
			break;
		default:
			break;
		}
		switch (m_pk_state)
		{
		case faith::e_cross_pk_state_16:
			pk_state_id = 90305267;
			notice_id = 93000269;
			break;
		case faith::e_cross_pk_state_8:
			pk_state_id = 90305268;
			notice_id = 93000269;
			break;
		case faith::e_cross_pk_state_4:
			pk_state_id = 90305269;
			notice_id = 93000269;
			break;
		case faith::e_cross_pk_state_2:
			notice_id = 93000272;
			break;
		default:
			break;
		}
		if (false == get_lose_player_guid().is_valid())
		{
			is_only_one_player = true;
			notice_id += 1;
		}
		const std::string& notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		const std::string& class_type_str = template_manager::get_instance().get_str_by_string_template_id(class_type_str_id);
		const std::string& pk_state_str =  template_manager::get_instance().get_str_by_string_template_id(pk_state_id);

		if (m_pk_state != e_cross_pk_state_2)
		{
			std::vector<std::string> notice_str_params_vec;
			notice_str_params_vec.push_back(notice_str_id);
			notice_str_params_vec.push_back(get_winner_player_name());
			if (!is_only_one_player)
			{
				notice_str_params_vec.push_back(get_lose_player_name());
			}
			notice_str_params_vec.push_back(pk_state_str);
			std::string notice_str = init_unit::implode(notice_str_params_vec);
			m_cur_map_ptr->send_notice_to_all_player(notice_id, notice_str);
			return;
		}
		else
		{
			player& player_one_ref = unit_man::get_player(m_winner_player_guid);
			if (false == player_one_ref.is_valid())
			{
				return;
			}
			std::vector<std::string> notice_str_params_vec;
			notice_str_params_vec.push_back(notice_str_id);
			notice_str_params_vec.push_back(get_winner_player_name());
			if (!is_only_one_player)
			{
				notice_str_params_vec.push_back(get_lose_player_name());
			}
			notice_str_params_vec.push_back(class_type_str);
			std::string notice_str = init_unit::implode(notice_str_params_vec);
			m_cur_map_ptr->send_notice_to_all_player(notice_id, notice_str);

			int32 notice_id_end = 93000274;
			std::string notice_str_id_end = template_manager::get_instance().get_str_id_by_notice_id(notice_id_end);
			std::vector<std::string> notice_str_params_vec_end;
			notice_str_params_vec_end.push_back(notice_str_id_end);
			notice_str_params_vec_end.push_back(class_type_str);
			std::string notice_str_end = init_unit::implode(notice_str_params_vec_end);
			m_cur_map_ptr->send_notice_to_all_player(notice_id_end, notice_str_end);
			return;
		}

	}

	void cross_arena_island::send_award_to_winner_player()
	{
		if (false == m_winner_player_guid.is_valid())
		{
			return;
		}
		auto player_msg = get_player_msg_for_guid(m_winner_player_guid);

		int32 award_id = first_award_att_id + 1;
		if (m_pk_state <= e_cross_pk_state_none || m_pk_state >= e_cross_pk_state_max)
		{
			return;
		}
		award_id += (int32)m_pk_state;
		RewardSettlementTemplate* temp_ptr = GET_TEMPLATE(RewardSettlementTemplate, award_id);
		if (nullptr == temp_ptr)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();

		std::vector<int32> item_list = temp_ptr->ItemRewards;
		if (item_list.size() <= 0 || item_list.size() % 2 != 0)
		{
			return;
		}
		for (int32 i = 0; i < item_list.size() - 1; i += 2)
		{
			drop_item_list.push_back({ item_list[i] , item_list[i + 1] });
		}
		if (drop_item_list.size() > 0)
		{
			std::string title = "";
			std::vector<std::string> content_params;
			title = globle_data::get_instance().get_mail_common_text_id((e_mail_common_text)((int32)e_mail_common_text_cross_pk_out_16_award_title + 2 * ((int32)m_pk_state + 1)));
			
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id((e_mail_common_text)((int32)e_mail_common_text_cross_pk_out_16_award_context + 2 * ((int32)m_pk_state + 1))));
			std::string contenttext = init_unit::implode(content_params);

			globle_data::get_instance().send_mail_with_item(m_winner_player_guid, player_msg.server_id, drop_item_list, title, contenttext, 0, 0, 0, 0);

			{
				player& player_ref = unit_man::get_player(m_winner_player_guid);
				if (player_ref.is_valid())
				{
					server_log::occupation_pk_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_login_type(), m_cur_island_class_type + 1, m_pk_state + 1, get_lose_player_guid(), 1, drop_item_list);
					set_log_var(log_head);
					player_ref.get_log_common_head_info(log_head);
					server_log::serverOccupationPk(log_head, m_cur_island_class_type + 1, m_pk_state + 1, get_lose_player_guid(), 1, drop_item_list);
				}
				else if (m_winner_player_guid.is_valid())
				{
					set_log_var(log_head);
					set_log_common_head_part3(log_head, "null", cell_server::getInstance().get_server_id(), utility::get_tick_count());
					server_log::serverOccupationPk(log_head, m_cur_island_class_type + 1, m_pk_state + 1, get_lose_player_guid().server_64, 1, drop_item_list);
					server_log::occupation_pk_log(m_winner_player_guid, m_cur_island_class_type + 1, m_pk_state + 1, get_lose_player_guid().server_64, 1, drop_item_list);
				}
			}
			{
				drop_item_list.clear();
				player& player_ref = unit_man::get_player(get_lose_player_guid());
				if (player_ref.is_valid())
				{
					server_log::occupation_pk_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_login_type(), m_cur_island_class_type + 1, m_pk_state + 1, m_winner_player_guid, 0, drop_item_list);
					set_log_var(log_head);
					player_ref.get_log_common_head_info(log_head);
					server_log::serverOccupationPk(log_head, m_cur_island_class_type + 1, m_pk_state + 1, m_winner_player_guid.server_64, 0, drop_item_list);
				}
				else if (get_lose_player_guid().is_valid())
				{
					server_log::occupation_pk_log(get_lose_player_guid(), m_cur_island_class_type + 1, m_pk_state + 1, m_winner_player_guid.server_64, 0, drop_item_list);

					set_log_var(log_head);
					set_log_common_head_part3(log_head, "null", cell_server::getInstance().get_server_id(), utility::get_tick_count());
					server_log::serverOccupationPk(log_head, m_cur_island_class_type + 1, m_pk_state + 1, m_winner_player_guid.server_64, 0, drop_item_list);
				}
			}
		}
		if (m_pk_state == e_cross_pk_state_2)
		{
			cs2ws_set_big_player pk_king_winner_msg;
			pk_king_winner_msg.winner_guid = m_winner_player_guid;
			pk_king_winner_msg.big_player_type = e_big_player_type_best_barserker + m_cur_island_class_type;
			connection_mgr::getInstance().send_to_ws(&pk_king_winner_msg, sizeof(pk_king_winner_msg));
			player& winner_player_ref = unit_man::get_player(m_winner_player_guid);
			if (winner_player_ref.is_valid() == false)
			{
				return;
			}
			winner_player_ref.get_title_mgr().add_title_by_template_id(title_best_barserker_temp_id + m_cur_island_class_type);
			winner_player_ref.get_title_mgr().equip_on_title(title_best_barserker_temp_id + m_cur_island_class_type);
		}
	}

	void cross_arena_island::send_ws_msg_time_out()
	{
		if (is_vaild() == false)
		{
			return;
		}
		s_pk_island_simple_msg simple_island_msg;
		s_pk_island_all_msg all_msg;
		all_msg.clear_data();
		simple_island_msg.clear_data();
		int32 war_count = 1;
		fill_simple_msg(simple_island_msg);
		all_msg.island_msg[0] = simple_island_msg;
		cs2ws_update_one_island_msg island_msg;
		island_msg.island_msg = all_msg;
		island_msg.war_num = war_count;
		island_msg.is_need_send_to_client = true;
		island_msg.pk_count = m_is_pk_count;
		if (false == m_is_in_the_war)
		{
			island_msg.is_end_war = 1;
			island_msg.winner_guid = m_winner_player_guid;
		}
		connection_mgr::getInstance().send_to_ws(&island_msg, sizeof(island_msg));
	}

	void cross_arena_island::reset_player_state()
	{
		player& player_ref = unit_man::get_player(m_island_player_msg[0].player_guid);
		if (player_ref.is_valid() && player_ref.is_in_cross_pk_map())
		{
			player_ref.change_hp_and_del_fuhuo_buff();
		}
		player& other_ref = unit_man::get_player(m_island_player_msg[1].player_guid);
		if (other_ref.is_valid() && other_ref.is_in_cross_pk_map())
		{
			other_ref.change_hp_and_del_fuhuo_buff();
		}
	}

	void cross_arena_island::send_client_map_state()
	{
		game_proto_cross_server_pk_map_island_msg msg;
		msg.set_island_idex(m_island_idex);
		msg.set_map_state(m_1v1_pk_state);
		msg.set_war_idex(get_cur_island_war_idex());
		int32 left_time = -1;
		switch (m_1v1_pk_state)
		{
		case faith::e_1v1_pk_state_ready:
			left_time = (m_next_begin_war_time - time_helper::get_cur_time_new().millisecond) / second_tick_time;
			msg.set_left_time(left_time);
			break;
		case faith::e_1v1_pk_state_begin:
			left_time = (m_next_begin_war_time + m_war_time * second_tick_time - time_helper::get_cur_time_new().millisecond) / second_tick_time;
			msg.set_left_time(left_time);
			break;
		case faith::e_1v1_pk_state_end:
			left_time = (m_next_ready_war_time - time_helper::get_cur_time_new().millisecond) / second_tick_time;
			msg.set_left_time(left_time);
			break;
		case faith::e_1v1_pk_state_max:
			msg.set_left_time(0);
			break;
		default:
			return;
		}
		send_msg_to_island_player(&msg, e_msgindex_s2c_pk_map_island_state_msg);
		//player& player_ref = unit_man::get_player(m_island_player_msg[0].player_guid);
		//if (player_ref.is_valid())
		//{
		//	msg.set_cur_win_num(m_island_player_msg[0].win_num);
		//	msg.set_other_win_num(m_island_player_msg[1].win_num);
		//	if (idex == 0)
		//	{
		//		msg.set_win_state(e_pk_island_win_state_win);
		//	}
		//	else if (idex == -1)
		//	{
		//		msg.set_win_state(e_pk_island_win_state_draw);
		//	}
		//	else if (idex == 1)
		//	{
		//		msg.set_win_state(e_pk_island_win_state_lose);
		//	}
		//	else
		//	{
		//		msg.set_win_state(e_pk_island_win_state_max);
		//	}
		//	player_ref.send_message_to_self(&msg, e_msgindex_s2c_pk_map_island_state_msg);
		//}


		//player& other_ref = unit_man::get_player(m_island_player_msg[1].player_guid);
		//if (other_ref.is_valid())
		//{
		//	msg.set_cur_win_num(m_island_player_msg[1].win_num);
		//	msg.set_other_win_num(m_island_player_msg[0].win_num);
		//	if (idex == 0)
		//	{
		//		msg.set_win_state(e_pk_island_win_state_lose);
		//	}
		//	else if (idex == -1)
		//	{
		//		msg.set_win_state(e_pk_island_win_state_draw);
		//	}
		//	else if (idex == 1)
		//	{
		//		msg.set_win_state(e_pk_island_win_state_win);
		//	}
		//	else
		//	{
		//		msg.set_win_state(e_pk_island_win_state_max);
		//	}
		//	other_ref.send_message_to_self(&msg, e_msgindex_s2c_pk_map_island_state_msg);
		//}
	}

	void cross_arena_island::send_msg_to_island_player(google::protobuf::Message* net_pro, uint32 header)
	{
		if (net_pro == nullptr)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_island_player_msg[0].player_guid);
		if (player_ref.is_valid())
		{
			player_ref.send_message_to_self(net_pro, header);
		}
		player& other_ref = unit_man::get_player(m_island_player_msg[1].player_guid);
		if (other_ref.is_valid())
		{
			other_ref.send_message_to_self(net_pro, header);
		}
	}

	bool cross_arena_island::check_is_need_end_war()
	{
		bool need_end_war = false;
		player& player_ref = unit_man::get_player(m_island_player_msg[1].player_guid);
		if (!player_ref.is_valid() || !m_island_player_msg[1].player_guid.is_valid() || !player_ref.is_in_cross_pk_map())
		{
			need_end_war = true;
		}
		if (false == need_end_war)
		{
			player& other_ref = unit_man::get_player(m_island_player_msg[0].player_guid);
			if (!other_ref.is_valid() || !m_island_player_msg[0].player_guid.is_valid() || !other_ref.is_in_cross_pk_map())
			{
				need_end_war = true;
			}
		}
		if (need_end_war)
		{
			end_war();
		}
		return need_end_war;
	}
}