/********************************************************************
created: 2017-5-26 16:50:30
file base: talent
file ext: cpp
author: zhy

purpose:
*********************************************************************/

#include "cache_mgr.hpp"
#include "internal/talent_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "internet/talent.pb.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "server_log_def.hpp"
#include "talent_mgr.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "utility/parse_msg.h"

namespace faith
{

	ctalent_mgr::ctalent_mgr()
	{
		m_unit_array_index = 0;
		m_talent_vector.clear();
		m_empty_layer.clear();
	}

	ctalent_mgr::~ctalent_mgr(void)
	{
		clear_data();
	}

	void ctalent_mgr::heart_tick(const int64& new_time)
	{
		for (int32 i = 0; i < m_talent_vector.size(); i++)
		{
			if (nullptr != m_talent_vector[i] && m_talent_vector[i]->is_valid())
			{
				m_talent_vector[i]->heart_tick(new_time);
			}
		}
	}

	void ctalent_mgr::set_player_ptr(int32 unit_array_index)
	{
		m_unit_array_index = unit_array_index;
	}


	void ctalent_mgr::clear_data()
	{
		m_empty_layer.clear();
		for (int32 i = 0; i < m_talent_vector.size(); i++)
		{
			if (nullptr == m_talent_vector[i])
			{
				continue;
			}
			cache_mgr::get_instance().back_ctalent_ptr(m_talent_vector[i]);
			m_talent_vector[i] = nullptr;
		}
		m_talent_vector.clear();
	}

	void ctalent_mgr::set_talent_by_info(const s_talent_info& talent_data, int32 talent_index)
	{
		ctalent* talent_ptr = cache_mgr::get_instance().get_ctalent_ptr();
		if (nullptr == talent_ptr)
		{
			return;
		}
		talent_ptr->set_talent_data(talent_data);
		talent_ptr->set_valid_state(true);
		m_talent_vector.push_back(talent_ptr);
	}
	void ctalent_mgr::set_talent_all_buff(bool is_add)
	{
		for (int32 i = 0; i < m_talent_vector.size(); i++)
		{
			if (nullptr != m_talent_vector[i] && m_talent_vector[i]->is_valid())
			{
				m_talent_vector[i]->set_talent_buff(m_unit_array_index, is_add);
			}
		}
	}
	void ctalent_mgr::set_talent_all_skill(bool is_add)
	{
		for (int32 i = 0; i < m_talent_vector.size(); i++)
		{
			if (nullptr != m_talent_vector[i] && m_talent_vector[i]->is_valid())
			{
				m_talent_vector[i]->set_talent_skill(m_unit_array_index, is_add);
			}
		}
	}
	ctalent* ctalent_mgr::get_talent_by_template_id(int32 talent_template_id)
	{
		int32 len = m_talent_vector.size();
		for (int32 i = 0; i < len; ++i)
		{
			ctalent* talent_ref = m_talent_vector[i];
			if (nullptr != talent_ref && talent_ref->is_valid() && talent_ref->get_talent_template_id() == talent_template_id)
			{
				return talent_ref;
			}
		}
		return nullptr;
	}

	ctalent* ctalent_mgr::get_talent_empty()
	{
		ctalent* talent_ptr = cache_mgr::get_instance().get_ctalent_ptr();
		if (nullptr == talent_ptr)
		{
			return nullptr;
		}
		m_talent_vector.push_back(talent_ptr);
		int32 len = m_talent_vector.size();
		return m_talent_vector[len - 1];
	}

	ctalent* ctalent_mgr::get_talent_by_series(int32 series_id)
	{
		int32 len = m_talent_vector.size();
		for (int32 i = 0; i < len; ++i)
		{
			ctalent* talent_ref = m_talent_vector[i];
			if (nullptr != talent_ref && talent_ref->is_valid() && talent_ref->get_talent_series() == series_id)
			{
				return talent_ref;
			}
		}
		return nullptr;
	}

	int32 ctalent_mgr::get_layer_talent_num(std::vector<int32>& layer_data)
	{
		int32 talent_num = 0;
		if (layer_data.size() < e_talent_layer_max)
		{
			return talent_num;
		}
		for (int32 i = e_talent_layer_template_id_0; i < e_talent_layer_max; ++i)
		{
			TalentTemplate* talent_template_ptr = GET_TEMPLATE(TalentTemplate, layer_data[i]);
			if (talent_template_ptr)
			{
				ctalent* talent_ptr = get_talent_by_series(talent_template_ptr->Series);
				if (talent_ptr)
				{
					talent_num += talent_ptr->get_data_info(e_talent_info_cast_money);
				}
			}
		}
		return talent_num;
	}
	int32 ctalent_mgr::get_talent_list_num(int32 talent_list_template_id)
	{
		int32 talent_num = 0;
		TalentListTemplate* talent_list_template_ptr = GET_TEMPLATE(TalentListTemplate, talent_list_template_id);
		if (nullptr == talent_list_template_ptr)
		{
			return talent_num;
		}
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer1);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer2);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer3);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer4);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer5);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer6);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer7);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer8);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer9);
		talent_num += get_layer_talent_num(talent_list_template_ptr->Layer10);
		return talent_num;
	}
	std::vector<int32>& ctalent_mgr::get_data_by_layer_index(int32 talent_list_template_id, int32 layer_index)
	{
		TalentListTemplate* talent_list_template_ptr = GET_TEMPLATE(TalentListTemplate, talent_list_template_id);
		if (nullptr == talent_list_template_ptr)
		{
			return m_empty_layer;
		}
		switch (layer_index)
		{
		case 1:
			return talent_list_template_ptr->Layer1;
			break;
		case 2:
			return talent_list_template_ptr->Layer2;
			break;
		case 3:
			return talent_list_template_ptr->Layer3;
			break;
		case 4:
			return talent_list_template_ptr->Layer4;
			break;
		case 5:
			return talent_list_template_ptr->Layer5;
			break;
		case 6:
			return talent_list_template_ptr->Layer6;
			break;
		case 7:
			return talent_list_template_ptr->Layer7;
			break;
		case 8:
			return talent_list_template_ptr->Layer8;
			break;
		case 9:
			return talent_list_template_ptr->Layer9;
			break;
		case 10:
			return talent_list_template_ptr->Layer10;
			break;
		default:
			return m_empty_layer;
			break;
		}
	}

	int32 ctalent_mgr::get_total_gs_value()
	{
		int32 total_gs_value = 0;
		int32 len = m_talent_vector.size();
		for (int32 i = 0; i < len; ++i)
		{
			ctalent* talent_ref = m_talent_vector[i];
			if (nullptr != talent_ref && talent_ref->is_valid())
			{
				total_gs_value += talent_ref->get_talent_gs_value();
			}
		}
		return total_gs_value;
	}


	faith::int32 ctalent_mgr::get_talent_cast(const int32 talent_type)
	{
		int32 cast_num = 0;
		int32 len = m_talent_vector.size();
		for (int32 i = 0; i < len; i++)
		{
			cast_num += m_talent_vector[i]->get_data_info(e_talent_info_cast_money);
		}
		return cast_num;
	}

	void ctalent_mgr::save_talent_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);


		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_talent);
		if (is_use == false)
		{
			cs2dp_save_char_talent req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_unit_array_index;
			uint32 talent_count = 0;

			int32 len = m_talent_vector.size();
			for (int32 i = 0; i < len; i++)
			{
				if (i >= TALENT_NUM_MAX)
				{
					break;
				}
				if (nullptr != m_talent_vector[i] && m_talent_vector[i]->is_valid())
				{
					req.data_list[i] = m_talent_vector[i]->get_talent_inst();
					++talent_count;
				}
			}

			req.data_num = talent_count;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_talent msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_unit_array_index);
			msg.set_save_type_ex(save_type);

			faith::cs2dp_proto::role_talent_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}

			uint32 row_count = 0;

			int32 len = m_talent_vector.size();
			for (int32 i = 0; i < len; i++)
			{
				if (i >= TALENT_NUM_MAX)
				{
					break;
				}
				if (nullptr != m_talent_vector[i] && m_talent_vector[i]->is_valid())
				{
					faith::cs2dp_proto::role_talent_row *db_row = db_data->add_row_data();
					if (db_row == nullptr)
					{
						return;
					}
					const s_talent_info& tmp =  m_talent_vector[i]->get_talent_inst();
					for (int32 j = 0; j < e_talent_info_max; j++)
					{
						db_row->add_data_ary(tmp.data_ary[j]);
					}			
					++row_count;
				}
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_talent);
		}
	}

	void ctalent_mgr::load_talent_by_db(const s_talent_info* talent_data, int32 talent_num)
	{
		for (int32 i = 0; i < talent_num; i++)
		{
			set_talent_by_info(talent_data[i], i);
		}
		send_talent_all();
	}
	bool ctalent_mgr::load_talent_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_talent_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_talent_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_talent_info *p_row = (s_talent_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_talent_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_talent_by_db((const s_talent_info *)p_data, msg.row_count());

		return true;

	}

	void ctalent_mgr::send_talent_all()
	{
		talent_proto_talent_info_all talent_msg;

		int32 len = m_talent_vector.size();
		for (int32 i = 0; i < len; i++)
		{
			ctalent* talent_ref = m_talent_vector[i];
			if (nullptr != talent_ref && talent_ref->is_valid())
			{
				talent_proto_talent_info_one* talent_info_ptr = talent_msg.add_talent_array();
				for (int32 i = 0; i < e_talent_info_max; ++i)
				{
					talent_info_ptr->add_data_ary(talent_ref->get_data_info(i));
				}
			}
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		player_ref.send_message_to_self(&talent_msg, e_msgindex_s2c_talent_info_all);
	}

	void ctalent_mgr::send_talent_one(ctalent* talent_ptr)
	{
		if (nullptr == talent_ptr)
		{
			return;
		}

		talent_proto_talent_info_one talent_msg;

		for (int32 i = 0; i < e_talent_info_max; ++i)
		{
			talent_msg.add_data_ary(talent_ptr->get_data_info(i));
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		player_ref.send_message_to_self(&talent_msg, e_msgindex_s2c_talent_info_one);
	}
	void ctalent_mgr::talent_level_up(int32 talent_template_id, int32 talent_list_template_id, int32 layer_index)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		faith::talent_proto_level_up_end msg;
		TalentTemplate* talent_template_ptr = GET_TEMPLATE(TalentTemplate, talent_template_id);
		if (nullptr == talent_template_ptr)
		{
			return;
		}
		//只能开启通用和自己转职系的天赋
		if (talent_template_ptr->Type > 0)
		{
			if (player_ref.get_unit_info(e_role_info_class_branch) != talent_template_ptr->Type)
			{
				return;
			}
		}
		//获取所在列的数据
		std::vector<int32>& layer_data = get_data_by_layer_index(talent_list_template_id, layer_index);
		if (layer_data.size() < e_talent_layer_max)
		{
			return;
		}
		//判定是否在这列里
		bool in_data = false;
		for (int32 i = e_talent_layer_template_id_0; i < e_talent_layer_max; ++i)
		{
			TalentTemplate* temp_talent_template_ptr = GET_TEMPLATE(TalentTemplate, layer_data[i]);

			if (temp_talent_template_ptr && talent_template_ptr->Series == temp_talent_template_ptr->Series)
			{
				in_data = true;
				break;
			}
		}
		if (in_data == false)
		{
			return;
		}
		//判定是否满足开启列的需求
		if (layer_data[e_talent_layer_need_id] > 0)
		{
			if (layer_data[e_talent_layer_need_id] < TALENT_ID_LOW)
			{
				std::vector<int32>& need_layer_data = get_data_by_layer_index(talent_list_template_id, layer_data[e_talent_layer_need_id]);
				if (need_layer_data.size() < e_talent_layer_max)
				{
					msg.set_operate_result(faith::e_item_string_can_not_use);
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
					return;
				}
				if (get_layer_talent_num(need_layer_data) < layer_data[e_talent_layer_need_num])
				{
					msg.set_operate_result(faith::e_item_string_can_not_use);
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
					return;
				}
			}
			else
			{
				if (get_talent_list_num(layer_data[e_talent_layer_need_id]) < layer_data[e_talent_layer_need_num])
				{
					msg.set_operate_result(faith::e_item_string_can_not_use);
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
					return;
				}
			}
		}
		int32 can_open_id = talent_template_ptr->OpenTalentId;
		if (can_open_id > 0)
		{
			TalentTemplate* open_talent_template_ptr = GET_TEMPLATE(TalentTemplate, can_open_id);
			if (open_talent_template_ptr)
			{
				ctalent* talent_ptr = get_talent_by_series(open_talent_template_ptr->Series);
				if (nullptr == talent_ptr)
				{
					msg.set_operate_result(faith::e_item_string_can_not_use);
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
					return;
				}
				if (talent_ptr->get_talent_template_id() < can_open_id)
				{
					msg.set_operate_result(faith::e_item_string_can_not_use);
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
					return;
				}
			}
		}

		// 开启的天赋页限制
		const int32 OpenLayerLimitSize = talent_template_ptr->OpenrLayerLimit.size();
		if (OpenLayerLimitSize == faith::e_open_layer_limit_max)
		{
			const int32 OpenLayerLimitType = talent_template_ptr->OpenrLayerLimit[faith::e_open_layer_limit_type];
			const int32 OpenLayerLimitNum = talent_template_ptr->OpenrLayerLimit[faith::e_open_layer_limit_num];
			if (get_talent_cast(OpenLayerLimitType) < OpenLayerLimitNum)
			{
				msg.set_operate_result(faith::e_item_string_can_not_use);
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
				return;
			}
		}


		ctalent* talent_ptr = get_talent_by_series(talent_template_ptr->Series);
		int32 old_data_ary[e_talent_info_max];
		s_talent_info new_talent_info;
		int32 cast_money = 0;
		int32 cast_money_type = -1;
		if (nullptr != talent_ptr)
		{
			if (talent_template_ptr->Level >= talent_template_ptr->LevelMax)
			{
				return;
			}
			if (talent_ptr->get_talent_template_id() > talent_template_id)
			{
				msg.set_operate_result(faith::e_item_string_can_not_use);
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
				return;
			}
			for (int32 i = 0; i < e_talent_info_max; ++i)
			{
				old_data_ary[i] = talent_ptr->get_data_info(i);
			}
			TalentTemplate* next_talent_template_ptr = GET_TEMPLATE(TalentTemplate, talent_template_id + 1);
			if (nullptr == next_talent_template_ptr)
			{
				return;
			}
			if (next_talent_template_ptr->OpenMoney.size() >= 2)
			{
				cast_money_type = next_talent_template_ptr->OpenMoney[0];
				cast_money = next_talent_template_ptr->OpenMoney[1];
			}
			new_talent_info.data_ary[e_talent_info_template_id] = talent_template_id + 1;
			new_talent_info.data_ary[e_talent_info_cast_money] = old_data_ary[e_talent_info_cast_money] + cast_money;
		}
		else
		{
			memset(old_data_ary, 0, sizeof(old_data_ary));
			if (talent_template_ptr->OpenMoney.size() >= 2)
			{
				cast_money_type = talent_template_ptr->OpenMoney[0];
				cast_money = talent_template_ptr->OpenMoney[1];
			}
			new_talent_info.data_ary[e_talent_info_template_id] = talent_template_id;
			new_talent_info.data_ary[e_talent_info_cast_money] = cast_money;

		}
		if (talent_template_ptr->Level > 1 && new_talent_info.data_ary[e_talent_info_template_id] - old_data_ary[e_talent_info_template_id] > 1)
		{
			return;
		}
		if (player_ref.can_cut_money((e_money_type)cast_money_type, cast_money) == false)
		{
			return;
		}
		if (nullptr == talent_ptr)
		{
			talent_ptr = cache_mgr::get_instance().get_ctalent_ptr();
			if (nullptr == talent_ptr)
			{
				return;
			}
			m_talent_vector.push_back(talent_ptr);
		}
	
		player_ref.cut_money((e_money_type)cast_money_type, cast_money, e_server_log_cut_money_talent_open, talent_template_id);
		talent_ptr->set_talent_buff(m_unit_array_index, false);
		talent_ptr->clear_data();
		talent_ptr->set_talent_data(new_talent_info);
		talent_ptr->set_valid_state(true);
		talent_ptr->set_talent_buff(m_unit_array_index, true);
		talent_ptr->set_talent_skill(m_unit_array_index, true);
		send_talent_one(talent_ptr);

		player_ref.set_unit_gs_change();//改变战力

		msg.set_operate_result(faith::e_item_string_succeed);
		msg.set_old_talent_template_id(old_data_ary[e_talent_info_template_id]);
		msg.set_new_talent_template_id(new_talent_info.data_ary[e_talent_info_template_id]);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_talent_level_up_end);
	}
	void ctalent_mgr::talent_reset()
	{
		/*
		item_set& item_set_ref = item_manager::get_item_set(m_unit_array_index);
		guid_64 item_guid = item_set_ref.get_item_by_template_id(GAMECONFIG->TalentOpenItemId);
		if (item_guid.is_valid() == false)
		{
			return;
		}
		std::string del_item_cause_id;
		std::stringstream del_item_cause_id_stream;
		del_item_cause_id_stream << int32(e_server_log_del_item_clearn_talent);
		del_item_cause_id_stream >> del_item_cause_id;

		std::string del_item_log_location;
		std::stringstream del_item_location_stream;
		del_item_location_stream << int32(GAMECONFIG->TalentOpenItemId);
		del_item_location_stream >> del_item_log_location;
		item_set_ref.del_item(item_guid, 1, del_item_cause_id, del_item_log_location);*/

		set_talent_all_buff(false);
		set_talent_all_skill(false);
		clear_data();
		//int32 len = m_talent_vector.size();
		//for (int32 i = 0; i < len; ++i)
		//{
		//	ctalent* talent_ref = m_talent_vector[i];
		//	if (nullptr != talent_ref && talent_ref->is_valid())
		//	{	
		//		int32 talent_template_id = talent_ref->get_data_info(e_talent_info_template_id);
		//		TalentTemplate* next_talent_template_ptr = GET_TEMPLATE(TalentTemplate, talent_template_id);
		//		if (nullptr != next_talent_template_ptr)
		//		{
		//			if (next_talent_template_ptr->OpenMoney.size() >= 2)
		//			{
		//				if (next_talent_template_ptr->OpenMoney[0] == (int32)e_money_type_talent)
		//				{
		//					talent_num += talent_ref->get_data_info(e_talent_info_cast_money);
		//				}
		//				if (next_talent_template_ptr->OpenMoney[0] == (int32)e_money_type_attribute_talent)
		//				{
		//					attribute_talent += talent_ref->get_data_info(e_talent_info_cast_money);
		//				}
		//			}
		//		}
		//	}
		//}
		player& player_ref = unit_man::get_player(m_unit_array_index);

		int32 all_talent_num = player_ref.get_cur_level_max_talent_num();
		int64 cur_talent_num = player_ref.get_money_data(e_money_type_talent);
		int64 add_talent_num = all_talent_num - cur_talent_num;
		add_talent_num = add_talent_num > 0 ? add_talent_num : 0;

		int64 all_attribute_talent = player_ref.get_cur_level_max_attribute_talent_num();
		int64 cur_attribute_talent_num = player_ref.get_money_data(e_money_type_attribute_talent);
		int64 add_attribute_talent_num = all_attribute_talent - cur_attribute_talent_num;
		add_attribute_talent_num = add_attribute_talent_num > 0 ? add_attribute_talent_num : 0;

		player_ref.add_money(e_money_type_talent, add_talent_num, e_server_log_add_money_talent_recovery, 0, true);
		player_ref.add_money(e_money_type_attribute_talent, add_attribute_talent_num, e_server_log_add_money_talent_recovery, 0, true);

	}
}

