/********************************************************************
created: 2016/04/21
created: 21:4:2016 21:23:32
file base: belief
file ext: cpp
author: W.H.B.A.

purpose:
*********************************************************************/

#include "belief_def.hpp"
#include "belief_mgr.h"
#include "internet/belief.pb.h"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"

namespace faith
{

	cbelief_mgr::cbelief_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	cbelief_mgr::~cbelief_mgr(void)
	{
		clear_data();
	}

	void cbelief_mgr::heart_tick(const int64& new_time)
	{/*
		if (m_player_ptr == nullptr)
		{
			return;
		}

		if (new_time >= m_buff_change_time)
		{
			m_buff_change_time = init_unit::get_end_time(GAMECONFIG->BeliefBuffChangeDiffTime);

			for (uint32 i = 0; i < e_belief_type_max; ++i)
			{
				m_belief_ary[i].heart_tick(new_time);
			}

			for (int32 i = 0; i <max_belief_buff_num; ++i)
			{
				buff_man::del_buff_by_template_id(m_player_ptr->get_array_index(), m_player_ptr->get_array_index(), belief_buff_array[i]);
			}

			m_cur_buff_array = get_belief_buff_id_array();
			if (m_cur_buff_array.empty())
			{
				m_buff_index = 0;
			}
			else
			{
				if (m_buff_index >= m_cur_buff_array.size())
				{
					m_buff_index = 0;
				}
				buff_man::add_buff_inst(m_player_ptr->get_array_index(), m_player_ptr->get_array_index(), m_cur_buff_array[m_buff_index]);
				m_buff_index++;
			}
		}*/
	}

	void cbelief_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}


	void cbelief_mgr::clear_data()
	{
		for (int32 i = 0; i < e_belief_series_type_max; ++i)
		{
			m_belief_ary[i].clear_data();
		}
		m_buff_index = 0;
		m_cur_buff_array.clear();
		m_buff_change_time = 0;
	}

	BeliefTemplate* cbelief_mgr::get_cur_max_rank()
	{
		BeliefTemplate* BeliefTemplatePtr = nullptr;
		for (uint32 i = 0; i < faith::e_belief_series_type_max; i++)
		{
			if (m_belief_ary[i].get_belief_info_ptr() == nullptr)
			{
				continue;
			}

			//空值直接赋值
			if (nullptr == BeliefTemplatePtr)
			{
				BeliefTemplatePtr = m_belief_ary[i].get_belief_info_ptr();
			}
			else
			{
				//先判断阶数 阶数相同判断星数
				if (BeliefTemplatePtr->CurRankNumber < m_belief_ary[i].get_belief_info_ptr()->CurRankNumber)
				{
					BeliefTemplatePtr = m_belief_ary[i].get_belief_info_ptr();
				}
				else if(BeliefTemplatePtr->CurRankNumber == m_belief_ary[i].get_belief_info_ptr()->CurRankNumber && BeliefTemplatePtr->CurStarNumber < m_belief_ary[i].get_belief_info_ptr()->CurStarNumber)
				{
					BeliefTemplatePtr = m_belief_ary[i].get_belief_info_ptr();
				}
			}
		}
		return BeliefTemplatePtr;
	}
	int32 cbelief_mgr::get_cur_max_rank_value(int32 target_star)
	{
		BeliefTemplate* BeliefTemplatePtr = get_cur_max_rank();
		if (BeliefTemplatePtr == nullptr)
		{
			return 0;
		}
		else
		{
			int32 current_value = 0;
			//小于目标星级则不算达到此阶
			if (BeliefTemplatePtr->CurStarNumber < target_star)
			{
				current_value = BeliefTemplatePtr->CurRankNumber - 1;
			}
			else
			{
				current_value = BeliefTemplatePtr->CurRankNumber;
			}
			return current_value;
		}
		return 0;
	}

	cbelief* cbelief_mgr::getbelief(int32 belief_type)
	{
		if (belief_type < e_belief_series_type_spring || belief_type >= e_belief_series_type_max)
		{
			return nullptr;
		}
		if (nullptr == m_player_ptr)
		{
			return nullptr;
		}
		return &m_belief_ary[belief_type];
	}

	void cbelief_mgr::send_belief_notice(int32& belief_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		cbelief* temp_belief_ptr = getbelief(belief_type);
		if (nullptr == temp_belief_ptr)
		{
			return;
		}
		BeliefTemplate* template_ptr = temp_belief_ptr->get_belief_info_ptr();
		if (nullptr == template_ptr)
		{
			return;
		}
		xstring belief_name = get_belief_name((e_belief_series_type)belief_type);
		if (template_ptr->Notice_id > 0)
		{
			s_chat_notice_info notice_data;
			notice_data.num_type_data.push_back(template_ptr->CurRankNumber);
			notice_data.describe_str.push_back(belief_name);
			if (93000179 == template_ptr->Notice_id
				|| 93000178 == template_ptr->Notice_id)
			{
				notice_data.describe_str.push_back(get_sub_belief_buff_name(*temp_belief_ptr));
			}
			m_player_ptr->get_chat_mgr().send_notice_new(template_ptr->Notice_id, notice_data);
		}

	}


	void cbelief_mgr::sync_ranking_belief_data()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		const double* att_array = m_player_ptr->m_pawn_att.get_attack_att_all();
		int64 ranking_value = 0;
		for (int32 belief_type = e_belief_series_type_spring; belief_type < e_belief_series_type_max; belief_type++)
		{
			BeliefTemplate* belief_ptr = m_belief_ary[belief_type].get_belief_info_ptr();
			if (nullptr != belief_ptr)
			{
				ranking_value += init_unit::calcu_fighting_power_by_att_array(belief_ptr->AttArray, att_array);
			}
		}
		//增加信仰符文战力
		ranking_value += m_player_ptr->get_belief_rune_mgr().calcu_total_fighting_power();
		m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_belief, ranking_value);
	}

	int32 cbelief_mgr::get_belief_record_type(int32 belief_type)
	{
		int32 record_type = -1;
		switch (belief_type)
		{
		case e_belief_series_type_spring:
			record_type = e_role_history_highest_record_belief_spring;
			break;
		case e_belief_series_type_summer:
			record_type = e_role_history_highest_record_belief_summer;
			break;
		case e_belief_series_type_autumn:
			record_type = e_role_history_highest_record_belief_autumn;
			break;
		case e_belief_series_type_winter:
			record_type = e_role_history_highest_record_belief_winter;
			break;
		default:
			break;
		}
		return record_type;
	}

	xstring cbelief_mgr::get_belief_name(e_belief_series_type belief_type)
	{
		int32 name_id = 0;
		xstring return_empty_string = "";
		switch (belief_type)
		{
		case faith::e_belief_series_type_spring:
			name_id = 90095162;
			break;
		case faith::e_belief_series_type_summer:
			name_id = 90095163;
			break;
		case faith::e_belief_series_type_autumn:
			name_id = 90095164;
			break;
		case faith::e_belief_series_type_winter:
			name_id = 90095165;
			break;
		case faith::e_belief_series_type_max:
			break;
		default:
			break;
		}

		if (name_id != 0)
		{
			const xstring& name_str = template_manager::get_instance().get_str_by_string_template_id(name_id);
			return name_str;
		}
		return return_empty_string;
	}

	xstring cbelief_mgr::get_sub_belief_buff_name(const cbelief& belief_ref)
	{
		int32 name_index = -1;
		xstring name_str = "";
		BeliefTemplate*	 belief_ptr = belief_ref.get_belief_info_ptr();
		if (nullptr == belief_ptr)
		{
			return name_str;
		}
		int32 rank_num = belief_ptr->CurRankNumber;
		switch (rank_num)
		{
		case 1:	//1阶
			name_index = 0;
			break;
		case 4:	//4阶
			name_index = 1;
			break;
		case 7:	//7阶
			name_index = 2;
			break;
		default:
			break;
		}
		if (name_index < 0)
		{
			return name_str;
		}
		name_index = name_index + (belief_ptr->BeliefType * faith::e_belief_buff_level_max);
	    const std::vector<std::string>& temp_array_list = template_manager::get_instance().get_string_list(90090833);
		if (temp_array_list.empty())
		{
			return name_str;
		}
		if ((e_belief_buff_level_max *  e_belief_series_type_max )!= temp_array_list.size())
		{
			return name_str;
		}
		name_str = temp_array_list[name_index];

		return name_str;

	}
	void cbelief_mgr::belief_change_att(std::vector<f32> const & att_array, int32 add_count, bool is_add)
	{
		int32 arr_len = att_array.size();
		m_player_ptr->get_pawn_att().apply_att_change_by_array(att_array, is_add, add_count);
	}

	int32 cbelief_mgr::get_equip_num_by_belief_level(int32 rank_num, int32 star_num)
	{
		int32 equip_num = 0;
		for (int32 i = e_belief_series_type_spring; i < e_belief_series_type_max; ++i)
		{
			cbelief& temp_belief = m_belief_ary[i];
			BeliefTemplate const* belief_template_ptr = temp_belief.get_belief_info_ptr();
			if (belief_template_ptr == NULL)
			{
				continue;;
			}
			if ((belief_template_ptr->CurStarNumber >= star_num && belief_template_ptr->CurRankNumber >= rank_num) || belief_template_ptr->CurRankNumber > rank_num)
			{
				equip_num++;
			}
		}
		
		return equip_num;
	}

	void cbelief_mgr::set_belief_by_info(const s_belief_info& belief_data, int32 belief_number)//???
	{
		if (belief_number >= e_belief_series_type_max || belief_number < e_belief_series_type_spring)
		{
			return;
		}
		cbelief& temp_belief = m_belief_ary[belief_number];
		int32 belief_template_id = belief_data.data_ary[e_belief_info_template_id];
		temp_belief.set_belief_data(belief_data);
		BeliefTemplate const* belief_template_ptr = temp_belief.get_belief_info_ptr();
		if (belief_template_ptr == NULL)
		{
			return;
		}
		if (belief_data.data_ary[e_belief_info_upgrade_count] > 0)
		{
			belief_change_att(belief_template_ptr->AttArray, 1, true);
		}
	}



	int32 cbelief_mgr::get_total_rank()
	{
		int total_rank = 0;
		for (uint32 i = 0; i < faith::e_belief_series_type_max; i++)
		{
			if (m_belief_ary[i].get_belief_info_ptr() == nullptr
				|| m_belief_ary[i].get_data_info(e_belief_info_upgrade_count) == 0)
			{
				continue;
			}
			total_rank += m_belief_ary[i].get_belief_info_ptr()->CurRankNumber;
		}
		return total_rank;
	}


	void cbelief_mgr::save_belief_to_db(int32 save_type)
	{
		if (NULL == m_player_ptr)
		{
			return;
		}

		//bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_belief);
		//if (is_use == false)
		//{

		//	cs2dp_save_char_belief req;
		//	req.save_type_ex = save_type;
		//	req.role_guid = m_player_ptr->get_unit_guid();
		//	req.unit_array_index = m_player_ptr->get_array_index();
		//	uint32 belief_count = 0;

		//	for (uint32 i = 0; i < e_belief_series_type_max; ++i)
		//	{
		//		req.data_list[i] = m_belief_ary[i].get_belief_inst();
		//		++belief_count;
		//	}

		//	req.data_num = belief_count;
		//	m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		//}
		//else
		//{
		//	faith::cs2dp_proto::cs2dp_proto::save_role_belief msg;
		//	msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
		//	msg.set_unit_array_index(m_player_ptr->get_array_index());
		//	msg.set_save_type_ex(save_type);
		//	faith::cs2dp_proto::cs2dp_proto::role_belief_db *db_data = msg.mutable_db_data();
		//	if (db_data == nullptr)
		//	{
		//		return;
		//	}
		//	uint32 row_count = 0;

		//	for (uint32 i = 0; i < e_belief_series_type_max; ++i)
		//	{
		//		const s_belief_info& tmp = m_belief_ary[i].get_belief_inst();
		//		faith::cs2dp_proto::role_belief_row *db_row = db_data->add_row_data();
		//		if (db_row == nullptr)
		//		{
		//			return;
		//		}
		//		for (int32 j = 0; j < e_belief_info_max; j++)
		//		{
		//			db_row->add_data_ary(tmp.data_ary[j]);
		//		}
		//		++row_count;
		//	}
		//	db_data->set_row_count(row_count);
		//	if (row_count == 0)
		//	{
		//		db_data->add_row_data();
		//	}
		//	m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_belief);
		//}
	}

	void cbelief_mgr::load_belief_by_db(const s_belief_info* belief_data, int32 belief_num)
	{
		if (NULL == m_player_ptr)
		{
			return;
		}
		if (belief_num == 0)
		{
			for (int32 i = 0; i < e_belief_series_type_max; ++i)
			{
				s_belief_info new_belief;
				new_belief.data_ary[e_belief_info_template_id] = 57010001 + i * 10000;
				new_belief.data_ary[e_belief_info_upgrade_count] = 0;
				new_belief.data_ary[e_belief_info_try_upgrade_times] = 0;
				set_belief_by_info(new_belief, i);
			}
		}
		else
		{
			for (int32 i = 0; i < belief_num; i++)
			{
				set_belief_by_info(belief_data[i], i);
			}
		}

		send_belief_all();
	}

	bool cbelief_mgr::load_belief_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_belief_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_belief_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_belief_info *p_row = (s_belief_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_belief_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_belief_by_db((const s_belief_info *)p_data, msg.row_count());
		return true;
	}


	void cbelief_mgr::send_belief_all()
	{
		belief_proto_update_character_belief_all belief_msg;
		belief_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		belief_msg.add_role_guid(m_player_ptr->get_unit_guid().B);

		for (int32 belief_type = e_belief_series_type_spring; belief_type < e_belief_series_type_max; ++belief_type)
		{
			cbelief& temp_belief = m_belief_ary[belief_type];
			belief_proto_belief_info* belief_info_ptr = belief_msg.add_belief_list();
			for (int32 i = 0; i < e_belief_info_max; ++i)
			{
				belief_info_ptr->add_belief_data(temp_belief.get_data_info((e_belief_info)i));
			}
		}
		m_player_ptr->send_message_to_self(&belief_msg, e_msgindex_s2c_belief_update_character_all);
	}

	void cbelief_mgr::send_belief_one(int32 belief_type)
	{
		if (belief_type < e_belief_series_type_spring || belief_type >= e_belief_series_type_max)
		{
			return;
		}

		belief_proto_update_character_belief_one belief_msg;
		belief_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		belief_msg.add_role_guid(m_player_ptr->get_unit_guid().B);
		belief_msg.set_belief_type(belief_type);					

		belief_proto_belief_info* belief_info_ptr = belief_msg.add_belief_list();

		cbelief& temp_belief = m_belief_ary[belief_type];

		for (int32 i = 0; i < e_belief_info_max; ++i)
		{
			belief_info_ptr->add_belief_data(temp_belief.get_data_info((e_belief_info)i));
		}
		m_player_ptr->send_message_to_aoi(&belief_msg, e_msgindex_s2c_belief_update_character_one);
	}

	std::vector<int32> cbelief_mgr::get_belief_buff_id_array()
	{
		std::vector<int32> return_array;
		for (int32 i = 0; i < e_belief_series_type_max; ++i)
		{
			int32 buff_id = m_belief_ary[i].get_buff_id();
			if (buff_id > 0)
			{
				return_array.push_back(buff_id);
			}
		}
		return return_array;
	}

	int32 cbelief_mgr::get_belief_level(int32 belief_type)
	{
		if (belief_type < e_belief_series_type_spring || belief_type >= e_belief_series_type_max)
		{
			return 0;
		}
		if (nullptr == m_player_ptr
			|| m_belief_ary[belief_type].get_belief_info_ptr() == nullptr)
		{
			return 0;
		}
		return m_belief_ary[belief_type].get_belief_info_ptr()->CurRankNumber;
	}




}

