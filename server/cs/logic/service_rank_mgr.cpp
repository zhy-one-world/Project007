/********************************************************************
created: 2018年4月12日11:42
file base: service_rank_mgr
file ext: cpp
author: wangdi

purpose: about service rank
*********************************************************************/


#include "connection_mgr.hpp"
#include "internal/core.hpp"
#include "internal/service_rank_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "internet/ranking.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "service_rank_mgr.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"

namespace faith
{
	service_rank_mgr::service_rank_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	service_rank_mgr::~service_rank_mgr()
	{

	}

	void service_rank_mgr::clear_data()
	{
		for (int32 i = 0; i < e_service_rank_type_max; i++)
		{
			m_recevice_rewards_state_arr[i] = 0;
		}
	}

	void service_rank_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}

	void service_rank_mgr::load_recevice_rewards_state_from_db(const s_service_rank_reward_state* data_list, int32 data_num)
	{
		for (int32 i = 0; i < data_num; i++)
		{
			if (data_list[i].rank_type >= e_service_rank_type_max)
			{
				continue;
			}
			m_recevice_rewards_state_arr[data_list[i].rank_type] = data_list[i].reward_state;
		}
		send_service_rank_reward_state_all();
	}

	bool service_rank_mgr::load_recevice_rewards_state_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		cs2dp_proto::role_service_rank_reward_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_service_rank_reward_state));
		if (p_data == nullptr)
		{
			return false;
		}
		s_service_rank_reward_state *p_row = (s_service_rank_reward_state *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			cs2dp_proto::role_service_rank_reward_row db_row = msg.row_data(i);
			p_row->rank_type = db_row.rank_type();
			p_row->reward_state = db_row.reward_state();
			p_row++;
		}
		load_recevice_rewards_state_from_db((const s_service_rank_reward_state *)p_data, msg.row_count());
		return true;

	}

	void service_rank_mgr::save_recevice_rewards_state_to_db(int32 save_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_service_rank_reward_state);
		if (is_use == false)
		{
			cs2dp_save_service_rank_reward_state req;
			req.save_type_ex = save_type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			int32 data_num = 0;
			for (int32 i = 0; i < e_service_rank_type_max; i++)
			{
				if (m_recevice_rewards_state_arr[i] > 0)
				{
					req.data_list[data_num].rank_type = i;
					req.data_list[data_num].reward_state = m_recevice_rewards_state_arr[i];
					data_num++;
				}
			}
			req.data_num = data_num;
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			cs2dp_proto::save_role_service_rank_reward msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(save_type);

			cs2dp_proto::role_service_rank_reward_db * db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			int32 data_num = 0;
			for (int32 i = 0; i < e_service_rank_type_max; i++)
			{
				if (m_recevice_rewards_state_arr[i] > 0)
				{
					cs2dp_proto::role_service_rank_reward_row *db_row = db_data->add_row_data();
					if (db_row == nullptr)
					{
						return;
					}
					db_row->set_rank_type(i);
					db_row->set_reward_state(m_recevice_rewards_state_arr[i]);
					data_num++;
				}
			}
			db_data->set_row_count(data_num);
			if (data_num == 0)
			{
				db_data->add_row_data();
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_service_rank_reward_state);
		}
	}

	void service_rank_mgr::send_service_rank_reward_state_all()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		ranking_proto_service_rank_recevice_reward_state_all msg;
		for (int32 i = 0; i < e_service_rank_type_max; i++)
		{
			ranking_proto_service_rank_recevice_reward_state_one* info_ptr = msg.add_service_rank_type();
			if (nullptr == info_ptr)
			{
				continue;
			}
			info_ptr->set_service_rank_type(i);
			info_ptr->set_recevice_reward_state(m_recevice_rewards_state_arr[i]);
		}

		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_service_rank_reward_state_all);
	}

	void service_rank_mgr::send_service_rank_reward_state_one(int32 info_type)
	{
		if (info_type < e_service_rank_type_level || info_type >= e_service_rank_type_max || nullptr == m_player_ptr)
		{
			return;
		}
	
		ranking_proto_service_rank_recevice_reward_state_one msg;
		msg.set_service_rank_type(info_type);
		msg.set_recevice_reward_state(m_recevice_rewards_state_arr[info_type]);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_service_rank_reward_state_one);
	}

	void service_rank_mgr::receive_reward_by_type(int32 rank_type)
	{
		if (rank_type < e_service_rank_type_level || rank_type >= e_service_rank_type_max || nullptr == m_player_ptr)
		{
			return;
		}

		if (m_recevice_rewards_state_arr[rank_type] != e_service_rank_reward_state_type_have_qualification)
		{
			return;
		}

		cs2ws_req_service_rank_info req;
		req.role_guid = m_player_ptr->get_unit_guid();
		req.rank_type = rank_type;
		connection_mgr::getInstance().send_to_ws( &req, sizeof(req));
	}

	void service_rank_mgr::receive_reward_by_type_end(int32 rank_type, int32 role_num)
	{
		if (rank_type < e_service_rank_type_level || rank_type >= e_service_rank_type_max || m_recevice_rewards_state_arr[rank_type] != e_service_rank_reward_state_type_have_qualification || nullptr == m_player_ptr)
		{
			return;
		}

		ServiceRankTemplate* service_rank_template_ptr = GET_TEMPLATE(ServiceRankTemplate, service_rank_first_template_id + rank_type);
		if (nullptr == service_rank_template_ptr || service_rank_template_ptr->RankNum.size() < 2 || service_rank_template_ptr->EndTime > globle_data::get_instance().get_server_on_days())
		{
			return;
		}

		int32 reward_index = -1;
		if (role_num < 0)
		{
			return;
		}
		else if (0 == role_num)
		{
			//为0的是未进入排行 单独处理
			reward_index = service_rank_template_ptr->RankNum.size();
		}
		else
		{
			for (int32 i = 0; i < service_rank_template_ptr->RankNum.size(); i++)
			{
				if (role_num <= service_rank_template_ptr->RankNum[i])
				{
					reward_index = i;
					break;
				}
			}
		}
		std::vector<int32> item_vec;

		switch (reward_index)
		{
		case e_service_rank_reward_type_first:
			item_vec = service_rank_template_ptr->ItemRewardOne;
			break;

		case e_service_rank_reward_type_second:
			item_vec = service_rank_template_ptr->ItemRewardTwo;
			break;

		case e_service_rank_reward_type_third:
			item_vec = service_rank_template_ptr->ItemRewardThree;
			break;

		case e_service_rank_reward_type_fourth:
			item_vec = service_rank_template_ptr->ItemRewardFour;
			break;

		case e_service_rank_reward_type_fifth:
			item_vec = service_rank_template_ptr->ItemRewardFive;
			break;

		default:
			break;
		}

		if (item_vec.size() <= 0 || item_vec.size() % 2 != 0)
		{
			return;
		}

		if (!m_player_ptr->get_item_set().is_empty_slot_enough(e_bag_type_bag, item_vec.size() / 2))
		{
			std::string show_string = template_manager::get_instance().get_str_id_by_notice_id(bag_is_full_notice_id);
			m_player_ptr->get_chat_mgr().send_notice(bag_is_full_notice_id, show_string);
			return;
		}

		std::vector<s_item_template_info> promp_item_data;
		//超时时发邮件
		std::vector<s_item_template_info> mail_item_list;
		for (int32 i = 0; i < item_vec.size() / 2; i++)
		{
			int32 item_template_id = item_vec[i * 2];
			int32 item_num = item_vec[i * 2 + 1];

			ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_template_id);
			if (nullptr == ItemTemplatePtr)
			{
				continue;
			}

			//跳过不属于自己的物品
			if (ItemTemplatePtr->class_type.size() > 0)
			{
				bool need_pass = true;

				for (int32 j = 0; j < ItemTemplatePtr->class_type.size(); j++)
				{
					if (ItemTemplatePtr->class_type[j] == 0 || ItemTemplatePtr->class_type[j] == m_player_ptr->get_unit_info(e_role_info_class_type))
					{
						need_pass = false;
					}
				}

				if (need_pass)
				{
					continue;
				}
			}

			if (globle_data::get_instance().get_server_on_days() >= service_rank_template_ptr->GetRewardEndTime)
			{
				s_item_template_info mail_item;
				mail_item = m_player_ptr->get_item_set().get_const_att_item_by_id(item_template_id, item_num, 0, 1, m_player_ptr->get_unit_info(e_role_info_class_type), m_player_ptr->get_unit_info(e_role_info_exp_level));
				mail_item_list.push_back(mail_item);
			}
			else
			{
				citem* temp_item = m_player_ptr->get_item_set().create_item_by_template(e_server_log_add_item_service_rank,0,item_template_id, item_num, 1);
				if (temp_item)
				{
					int32 is_item_locked = temp_item->get_data_info(e_item_info_locked);
					m_player_ptr->get_item_set().put_in_bag(temp_item);
					promp_item_data.push_back({ item_template_id, item_num, is_item_locked });
				}
			}
		}

		if (promp_item_data.size() > 0)
		{
			m_player_ptr->get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
		}

		if (mail_item_list.size() > 0)
		{
			std::vector<std::string> content_params_title;
			content_params_title.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_service_rank_end_time_reward_title));
			std::string title = init_unit::implode(content_params_title);

			std::vector<std::string> content_params_contenttext;
			content_params_contenttext.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_service_rank_end_time_reward_context));
			std::string contenttext = init_unit::implode(content_params_contenttext);
			globle_data::get_instance().send_mail_with_item(m_player_ptr->get_unit_guid(),
				m_player_ptr->get_unit_info(e_role_info_server_id),
				mail_item_list, title, contenttext);
		}

		set_receive_reward_state(rank_type, e_service_rank_reward_state_type_already_receive);
	}

	void service_rank_mgr::set_receive_reward_state(int32 rank_type, int32 reward_state)
	{
		if (rank_type < e_service_rank_type_level || rank_type >= e_service_rank_type_max 
			|| reward_state < e_service_rank_reward_state_type_no_qualification || reward_state >= e_service_rank_reward_state_type_max)
		{
			return;
		}

		m_recevice_rewards_state_arr[rank_type] = reward_state;
		send_service_rank_reward_state_one(rank_type);
	}

	bool service_rank_mgr::is_can_set_reward_stete_have(int32 rank_type, const int64& rank_value)
	{
		if (rank_type < e_service_rank_type_level || rank_type >= e_service_rank_type_max || m_recevice_rewards_state_arr[rank_type] > e_service_rank_reward_state_type_no_qualification
			|| nullptr == m_player_ptr || !m_player_ptr->is_self_server())
		{
			return false;
		}
		ServiceRankTemplate* service_rank_template_ptr = GET_TEMPLATE(ServiceRankTemplate, service_rank_first_template_id + rank_type);
		if (nullptr == service_rank_template_ptr || (int64)service_rank_template_ptr->MinValue > rank_value || service_rank_template_ptr->EndTime <= globle_data::get_instance().get_server_on_days())
		{
			return false;
		}
		return true;
	}

	void service_rank_mgr::check_receive_reward_end_time()
	{
		for (int32 i = 0; i < e_service_rank_type_max; i++)
		{
			ServiceRankTemplate* service_rank_template_ptr = GET_TEMPLATE(ServiceRankTemplate, service_rank_first_template_id + i);
			if (nullptr == service_rank_template_ptr)
			{
				continue;
			}
			if (m_recevice_rewards_state_arr[i] == e_service_rank_reward_state_type_have_qualification && service_rank_template_ptr->EndTime < service_rank_template_ptr->GetRewardEndTime
				&& globle_data::get_instance().get_server_on_days() >= service_rank_template_ptr->GetRewardEndTime)
			{
				receive_reward_by_type(i);
			}
		}
	}

	void service_rank_mgr::check_receive_reward_state(e_RankingIndex ranking_index, const int64& rank_value)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		int32 service_rank_type = get_service_rank_type_by_ranking_type(ranking_index);
		if (service_rank_type < 0)
		{
			return;
		}
		if (m_player_ptr->get_service_rank_mgr().is_can_set_reward_stete_have(service_rank_type, rank_value))
		{
			m_player_ptr->get_service_rank_mgr().set_receive_reward_state(service_rank_type, e_service_rank_reward_state_type_have_qualification);
		}
	}

	int32 service_rank_mgr::get_service_rank_type_by_ranking_type(e_RankingIndex ranking_index)
	{
		for (int32 i = 0; i < e_service_rank_type_max; i++)
		{
			ServiceRankTemplate* service_rank_template_ptr = GET_TEMPLATE(ServiceRankTemplate, service_rank_first_template_id + i);
			if (nullptr != service_rank_template_ptr && service_rank_template_ptr->RankingType == ranking_index)
			{
				return i;
			}
		}
		return -1;
	}

}