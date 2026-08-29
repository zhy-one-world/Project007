/********************************************************************
created: 2016年9月13日17:46:11
file base: worship_target
file ext: h
author: zhangminghai

purpose: about rankings
*********************************************************************/

#include "connection_mgr.hpp"
#include "internal/core.hpp"
#include "internal/ranking_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "internet/ranking.pb.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "utility/globle_data.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"
#include "worship_target.h"

namespace faith
{
	cworship_target::cworship_target()
	{
		m_player_ptr = nullptr;
		clear_data();
	}


	cworship_target::~cworship_target(void)
	{
		
	}

	void cworship_target::fast_heart_tick(const int64& new_time)
	{
		if (m_is_worshiping
			&& new_time >= m_force_reset_worshiping_time)
		{
			set_is_worshiping(false);
		}
	}
	void cworship_target::clear_data()
	{
		for (int32 i = 0; i < DaliyWorshipTime; ++i)
		{
			m_my_record[i].ranking_index = 0;
			m_my_record[i].target_guid = 0;
		}
		set_is_worshiping(false);
	}

	void cworship_target::load_worship_target_from_db(const worship_record* data, int32 data_num)
	{
		for (int32 i = 0; i < data_num; ++i)
		{
			m_my_record[i].ranking_index = data[i].ranking_index;
			m_my_record[i].target_guid = data[i].target_guid;
		}
	}
	bool cworship_target::load_worship_by_db_lua(const char *data_ptr, int32 data_len)
	{

		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_worship_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(worship_record));
		if (p_data == nullptr)
		{
			return false;
		}
		worship_record *p_row = (worship_record *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_worship_row db_row = msg.row_data(i);
			p_row->ranking_index = db_row.ranking_index();
			p_row->target_guid.server_64 = db_row.target_guid();

			p_row++;
		}
		load_worship_target_from_db((const worship_record *)p_data, msg.row_count());
		return true;

	}


	void cworship_target::save_worship_target_to_db(int32 save_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		for (int32 i = 0; i < DaliyWorshipTime - 1; i++)
		{
			for (int32 j = i + 1; j < DaliyWorshipTime; j++)
			{
				if ((m_my_record[i].ranking_index == m_my_record[j].ranking_index) && (m_my_record[i].target_guid == m_my_record[j].target_guid))
				{
					for (int32 k = j; k < DaliyWorshipTime - 1; k++)
					{
						m_my_record[k] = m_my_record[k + 1];
					}
					m_my_record[DaliyWorshipTime - 1].ranking_index = 0;
					m_my_record[DaliyWorshipTime - 1].target_guid = 0;
					break;
				}
			}
		}


		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_worship_target);
		if (is_use == false)
		{

			cs2dp_save_char_worship_target req;
			req.save_type_ex = save_type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			int32  data_num = 0;

			for (int32 i = 0; i < DaliyWorshipTime; ++i)
			{
				req.data_list[i].ranking_index = m_my_record[i].ranking_index;
				req.data_list[i].target_guid = m_my_record[i].target_guid;
				if (m_my_record[i].target_guid.is_valid())
				{
					data_num++;
				}
			}
			req.data_num = data_num;
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_worship msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(save_type);

			faith::cs2dp_proto::role_worship_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}

			int32  row_count = 0;

			for (int32 i = 0; i < DaliyWorshipTime; ++i)
			{
				if (m_my_record[i].target_guid.is_valid() == false)
				{
					continue;
				}
				faith::cs2dp_proto::role_worship_row *db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				db_row->set_ranking_index(m_my_record[i].ranking_index);
				db_row->set_target_guid(m_my_record[i].target_guid.server_64);
				row_count++;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_worship_target);
		}
	}

	void cworship_target::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}


	void cworship_target::req_worship_player(e_RankingIndex ranking_index, int32 worship_type, const guid_64& target_player)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		if (m_is_worshiping)
		{
			return;
		}
		if (m_player_ptr->is_valid() == false)
		{
			return;
		}
		if (target_player.is_valid() == false)
		{
			return;
		}
		if (GAMECONFIG->MoneyWorshipCost.size() % 2 != 0
			|| worship_type < 0
			|| worship_type >= GAMECONFIG->WorshipAddValueArray.size())
		{
			return;
		}

		ranking_proto_worship_player_end resp;
		resp.set_target_guid(target_player.server_64);
		resp.set_ranking_index(ranking_index);
		resp.set_worship_value(0);
		resp.set_worship_type(worship_type);
		if (m_player_ptr->get_unit_info(e_role_info_worship_other_time) >= DaliyWorshipTime)
		{
			resp.set_worship_endtype(EWorshipEndType_time);
			m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_end);
			return;
		}

		bool is_target_worshiped = false;
		for (int32 i = 0; i < DaliyWorshipTime; ++i)
		{
			if (ranking_index == m_my_record[i].ranking_index
				&& target_player == m_my_record[i].target_guid)
			{
				is_target_worshiped = true;
				break;
			}
		}
		if (is_target_worshiped)
		{
			resp.set_worship_endtype(EWorshipEndType_same_person);
			m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_end);
			return;
		}

		if (worship_type == e_ranking_worship_type_money)
		{
			int32 money_request_num = GAMECONFIG->MoneyWorshipCost.size() / 2;
			for (int32 i = 0; i < money_request_num; ++i)
			{
				int32 money_id = GAMECONFIG->MoneyWorshipCost[i * 2];
				int32 money_value = GAMECONFIG->MoneyWorshipCost[i * 2 + 1];
				if (!m_player_ptr->can_cut_money((e_money_type)money_id, money_value))
				{
					resp.set_worship_endtype(EWorshipEndType_money_not_enough);
					m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_end);
					return;
				}
			}

			for (int32 i = 0; i < money_request_num; ++i)
			{
				int32 money_id = GAMECONFIG->MoneyWorshipCost[i * 2];
				int32 money_value = GAMECONFIG->MoneyWorshipCost[i * 2 + 1];
				m_player_ptr->cut_money((e_money_type)money_id, money_value, e_server_log_cut_money_worship_player);
			}
		}

		cs2ws_req_worship_player req;
		req.role_guid = m_player_ptr->get_unit_guid();
		req.ranking_index = ranking_index;
		req.target_guid = target_player;
		req.worship_type = worship_type;
		connection_mgr::getInstance().send_to_ws( &req, sizeof(cs2ws_req_worship_player));

		set_is_worshiping(true);
	}

	void cworship_target::req_worship_player_end(e_RankingIndex ranking_index, const int32& worship_type, const guid_64& target_guid, const int32& worship_value, const int32& worship_result)
	{
		set_is_worshiping(false);
		if ( nullptr == m_player_ptr)
		{
			return;
		}
		if (target_guid.is_valid() == false)
		{ 
			return;
		}
		 
		ranking_proto_worship_player_end resp;
		resp.set_ranking_index(ranking_index);
		resp.set_target_guid(target_guid.server_64);
		resp.set_worship_type(worship_type);
		resp.set_worship_value(worship_value); 
		resp.set_worship_endtype(worship_result);
		if (worship_result != EWorshipEndType_success)
		{
			m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_end);

			if (worship_type == e_ranking_worship_type_money
				&& GAMECONFIG->MoneyWorshipCost.size() >= 2)
			{
				const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
				const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_worship_failed_title);
				const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_worship_failed_context);

				if (sender_name == nullptr
					|| mail_title == nullptr
					|| mail_content == nullptr)
				{
					return;
				}
				globle_data::get_instance().send_mail_to_another_player_by_system(m_player_ptr->get_unit_guid(), m_player_ptr->get_unit_guid(), m_player_ptr->get_unit_info(e_role_info_server_id)
					, sender_name, mail_title, mail_content, GAMECONFIG->MoneyWorshipCost[0], GAMECONFIG->MoneyWorshipCost[1], 0, 0, nullptr, 0);
			}
			return;
		}

		PlayerUpgradeTemplate* temp_player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, m_player_ptr->get_unit_info(e_role_info_upgrade_id));
		if (temp_player_upgrade_ptr == nullptr)
		{
			resp.set_worship_endtype(EWorshipEndType_unknown);
			m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_end); 
			return;
		}

		float final_exp = float(temp_player_upgrade_ptr->BaseExp * GAMECONFIG->WorshipExpRewardRadio);

		m_player_ptr->set_unit_info(e_role_info_worship_other_time, m_player_ptr->get_unit_info(e_role_info_worship_other_time) + 1);
		m_player_ptr->send_info_one(e_role_info_worship_other_time);
		if (worship_type == e_ranking_worship_type_money)
		{
			m_player_ptr->add_exp(int32(GAMECONFIG->MoneyWorshipExpRate * final_exp));
		}
		else if (worship_type == e_ranking_worship_type_normal)
		{
			final_exp = m_player_ptr->add_exp(int32(final_exp));
		}

		m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_end);

		m_player_ptr->set_must_do_count(e_daily_must_do_typ_rank, m_player_ptr->get_must_do_count(e_daily_must_do_typ_rank) + 1);
		m_player_ptr->set_daily_active_degree_info(e_daily_must_do_typ_rank);
		m_player_ptr->send_daily_must_do_count_info_one(e_daily_must_do_typ_rank);
		m_player_ptr->get_mission_mgr().target_check(e_mission_end_type_cheer_worship);
		m_player_ptr->get_recycle_mgr().on_event(e_recycle_task_type_belief_11);
		m_player_ptr->get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_ranking_worship); //新限时行为排行榜崇拜计数
		send_worhip_record(); 
	}	

	void cworship_target::send_worhip_record()
	{
		ranking_proto_worship_record  resp;
		if (m_player_ptr == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < DaliyWorshipTime; i++)
		{
			ranking_proto_worship_record_one* record_one = resp.add_record_one();
			record_one->set_ranking_type(m_my_record[i].ranking_index);
			record_one->set_target_guid(m_my_record[i].target_guid.server_64);
		}
		m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_ranking_worship_record);
	}


	void cworship_target::add_new_record(const guid_64 target_guid, const int32 ranking_index)
	{
		for (int32 i = 0; i < DaliyWorshipTime; ++i)
		{
			if (!m_my_record[i].target_guid.is_valid())
			{
				m_my_record[i].target_guid = target_guid;
				m_my_record[i].ranking_index = ranking_index;
				break;
			}
		}
	}

	void cworship_target::fresh_data()
	{
		m_player_ptr->set_unit_info(e_role_info_worship_other_time, 0);
		m_player_ptr->send_info_one(e_role_info_worship_other_time);
		clear_data();
		send_worhip_record();
	}

	void cworship_target::set_is_worshiping(bool is_worshiping)
	{
		m_is_worshiping = is_worshiping;
		if (m_is_worshiping)
		{
			m_force_reset_worshiping_time = init_unit::get_end_time(3);
		}
		else
		{
			m_force_reset_worshiping_time = 0;
		}
	}
}