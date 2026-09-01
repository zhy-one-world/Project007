/********************************************************************
created: 2016/7/18
created: 18:7:2016 16:06
file base: pokedex
file ext: cpp
author: XingLong

purpose:
*********************************************************************/

#include "pokedex_mgr.h"
#include "pokedex_def.hpp"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "template/StringConst_S.h"
#include "utility/parse_msg.h"
#include "internet/pokedex.pb.h"
#include "internet/net.pb.h"
#include "internal/pokedex_msg.hpp"
#include "internet/cs2dp.pb.h"
namespace faith
{
	cpokedex_mgr::cpokedex_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();

	}

	cpokedex_mgr::~cpokedex_mgr(void)
	{
		clear_data();
	}

	void cpokedex_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}
	void cpokedex_mgr::send_pokedex_all()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		pokedex_proto_update_character_pokedex_all pokedex_msg;
		pokedex_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		pokedex_msg.add_role_guid(m_player_ptr->get_unit_guid().B);

		for (int32 pokedex_index = 0; pokedex_index <POKEDEX_NUM_MAX; ++pokedex_index)
		{
			cpokedex& temp_pokedex = m_pokedex_ary[pokedex_index];
			int32 temp_size = pokedex_msg.ByteSize();
			if (temp_size > packege_length_max)
			{
				m_player_ptr->send_message_to_self(&pokedex_msg, e_msgindex_s2c_pokedex_update_character_all);
				pokedex_msg.clear_pokedex_list();
			}
			if (nullptr != temp_pokedex.get_pokedex_info_ptr())
			{
				pokedex_proto_pokedex_info* pokedex_info_ptr = pokedex_msg.add_pokedex_list();
				if (pokedex_info_ptr == nullptr)
				{
					continue;
				}
				for (int32 i = 0; i < e_pokedex_Info_max; ++i)
				{
					pokedex_info_ptr->add_pokedex_data(temp_pokedex.get_data_info((e_pokedex_Info)i));
				}
				for (int32 i = 0; i < PER_POKEDEX_PSYCHE_NUM; ++i)
				{
					pokedex_info_ptr->add_collect_num(temp_pokedex.get_psyche_num(i));
				}
			}
		}
		if (pokedex_msg.pokedex_list_size() > 0)
		{
			m_player_ptr->send_message_to_self(&pokedex_msg, e_msgindex_s2c_pokedex_update_character_all);
		}
	}

	void cpokedex_mgr::send_pokedex_one(int32 pokedex_template_id)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		cpokedex* temp_ptr = get_pokedex_by_templateid(pokedex_template_id);
		if (temp_ptr == nullptr)
		{
			return;
		}
		pokedex_proto_update_character_pokedex_all pokedex_msg;

		pokedex_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		pokedex_msg.add_role_guid(m_player_ptr->get_unit_guid().B);
		if (nullptr != temp_ptr->get_pokedex_info_ptr())
		{
			pokedex_proto_pokedex_info* pokedex_info_ptr = pokedex_msg.add_pokedex_list();
			if (pokedex_info_ptr == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_pokedex_Info_max; ++i)
			{
				pokedex_info_ptr->add_pokedex_data(temp_ptr->get_data_info((e_pokedex_Info)i));
			}
			for (int32 i = 0; i < PER_POKEDEX_PSYCHE_NUM; ++i)
			{
				pokedex_info_ptr->add_collect_num(temp_ptr->get_psyche_num(i));
			}
		}
		m_player_ptr->send_message_to_self(&pokedex_msg, e_msgindex_s2c_pokedex_update_character_all);
	}

	void cpokedex_mgr::clear_data()
	{	
		for (int32 i = 0; i < POKEDEX_NUM_MAX; ++i)
		{
			m_pokedex_ary[i].clear_data();
		}		
	}

	void cpokedex_mgr::submit_psyche(int32 pokedex_id)
	{	
		pokedex_proto_pokedex_operate_end end_msg;
		end_msg.set_pokedex_template_id(pokedex_id);
		if (m_player_ptr == nullptr)
		{
			return;
		}

		for (int32 i = 0; i < POKEDEX_NUM_MAX; ++i)
		{
			int32 pokedex_template_id = m_pokedex_ary[i].get_template_id();
			if (pokedex_template_id == 0)
			{
				continue;
			}
			cpokedex& target_pokedex_ptr = m_pokedex_ary[i];
			if (target_pokedex_ptr.is_all_active())
			{
				continue;
			}
			PokedexTemplate* target_pokedex_template_ptr = target_pokedex_ptr.get_pokedex_info_ptr();
			if (target_pokedex_template_ptr == nullptr
				|| target_pokedex_template_ptr->OpenLevel > m_player_ptr->get_unit_info(e_role_info_exp_level))
			{
				continue;
			}

			item_set& temp_item_set = m_player_ptr->get_item_set();
			for (int32 j = 0; j < target_pokedex_template_ptr->PsycheIdArray.size(); ++j)
			{
				if (j >= PER_POKEDEX_PSYCHE_NUM)
				{
					break;
				}
				ItemTemplate* psyche_item_template_ptr = GET_TEMPLATE(ItemTemplate, target_pokedex_template_ptr->PsycheIdArray[j]);
				if (psyche_item_template_ptr == nullptr)
				{
					continue;
				}
				PsycheTemplate* psyche_template_ptr = GET_TEMPLATE(PsycheTemplate, psyche_item_template_ptr->logic_id);
				if (psyche_template_ptr == nullptr)
				{
					continue;
				}
				int32 submited_num = target_pokedex_ptr.get_psyche_num(j);
				if (submited_num >= psyche_template_ptr->PsycheNum)
				{
					continue;
				}
				int32 cur_has_num = temp_item_set.get_item_count(e_bag_type_bag, target_pokedex_template_ptr->PsycheIdArray[j]);
				if (cur_has_num <= 0)
				{
					continue;
				}
				int32 can_submit_num = cur_has_num;
				bool is_activate = false;
				if (submited_num + can_submit_num >= psyche_template_ptr->PsycheNum)
				{
					can_submit_num = psyche_template_ptr->PsycheNum - submited_num;
					is_activate = true;
				}
				if (can_submit_num <= 0)
				{
					continue;
				}
				end_msg.add_submit_item_array(target_pokedex_template_ptr->PsycheIdArray[j]);
				end_msg.add_submit_item_array(can_submit_num);

				target_pokedex_ptr.add_psyche_num(j, can_submit_num);
				temp_item_set.cost_item_by_id_with_lock_states(e_server_log_del_item_submit_psyche, pokedex_id, target_pokedex_template_ptr->PsycheIdArray[j], can_submit_num, 0);

				// 活跃度
				int32 temp_cur_value = m_player_ptr->get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_submit_one_illustration);
				m_player_ptr->get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_submit_one_illustration, temp_cur_value + 1);

				if (is_activate)
				{
					temp_item_set.item_change_att(psyche_template_ptr->AttArray, 1, true);
				}
			}
			if (target_pokedex_ptr.is_all_active())
			{
				temp_item_set.item_change_att(target_pokedex_template_ptr->AttArray, 1, true);
			}
			send_pokedex_one(pokedex_template_id);
			m_player_ptr->get_achievement_mgr().player_changed(eat_illustration_finish_num_count);
		}
		end_msg.set_end_result(e_item_string_succeed);
		m_player_ptr->send_message_to_self(&end_msg, e_msgindex_s2c_pokedex_operate_end);
		m_player_ptr->refresh_service_goal(e_service_goal_type_actived_illustration_num);
		m_player_ptr->refresh_service_goal(e_service_goal_type_actived_any_chapter_illustration);
	}

	cpokedex* cpokedex_mgr::get_pokedex_by_templateid(int32 templayeid)
	{
		for (int32 i = 0; i < POKEDEX_NUM_MAX;++i)
		{
			if (templayeid == m_pokedex_ary[i].get_template_id())
			{
				return &m_pokedex_ary[i];
			}
		}
		return nullptr;
	}

	cpokedex*	cpokedex_mgr::get_pokedex_by_index(int32 index)
	{
		if (index < 0 || index >= POKEDEX_NUM_MAX)
			return nullptr;
		return &m_pokedex_ary[index];
	}

	cpokedex * cpokedex_mgr::get_palace_pokedex()
	{
		for (int32 i = 0; i < POKEDEX_NUM_MAX; ++i)
		{
			PokedexTemplate* temp_pokedex_template = m_pokedex_ary[i].get_pokedex_info_ptr();
			if (temp_pokedex_template && temp_pokedex_template->Type == 0)
			{
				return &m_pokedex_ary[i];
			}
		}
		return nullptr;
	}

	void cpokedex_mgr::check_activate_fetter(int32 pokedex_template_id)
	{
		if (m_player_ptr->is_valid() == false)
		{
			return;
		}
		float add_percent = get_add_percent();
		for (int32 i = 0; i < POKEDEX_NUM_MAX; ++i)
		{
			if (m_pokedex_ary[i].get_data_info(e_pokedex_Info_state) == 0)
			{
				continue;
			}
			PokedexTemplate* pokedex_template_ptr = m_pokedex_ary[i].get_pokedex_info_ptr();
			if (nullptr == pokedex_template_ptr)
			{
				continue;
			}
			if (pokedex_template_ptr->Type == 1)
			{
				if (pokedex_template_ptr->FetterArr.size() > 0)
				{
					for (int32 j = 0; j < pokedex_template_ptr->FetterArr.size();++j)
					{
						if (m_pokedex_ary[i].get_data_info(e_pokedex_Info_fetter_state_1 + j) > 0)
						{
							continue;
						}
						PsycheTemplate* psyche_template_ptr = GET_TEMPLATE(PsycheTemplate, pokedex_template_ptr->FetterArr[j]);
						if (nullptr == psyche_template_ptr)
						{
							continue;
						}
						if (psyche_template_ptr->FetterActivateNeedArray.size() > 0)
						{
							int32 is_contain = 0;
							for (int32 len = 0; len < psyche_template_ptr->FetterActivateNeedArray.size(); ++len)
							{
								if (pokedex_template_id == psyche_template_ptr->FetterActivateNeedArray[len])
								{
									is_contain = 1;
									break;
								}
							}
							int32 total_num = 0;
							if (is_contain > 0)
							{
								for (int32 len = 0; len < psyche_template_ptr->FetterActivateNeedArray.size(); ++len)
								{
									cpokedex* pokedex_ptr = get_pokedex_by_templateid(psyche_template_ptr->FetterActivateNeedArray[len]);
									if (pokedex_ptr && pokedex_ptr->get_data_info(e_pokedex_Info_state) > 0)
									{
										total_num++;
									}
								}
							}
							else
							{
								continue;
							}
							if (total_num == psyche_template_ptr->FetterActivateNeedArray.size())
							{
								m_pokedex_ary[i].set_data_info(e_pokedex_Info(e_pokedex_Info_fetter_state_1 + j), 1);
								m_player_ptr->get_item_set().item_change_att(psyche_template_ptr->FetterAttArray, add_percent, true);
							}
						}
					}
				}
			}
		}
		send_pokedex_all();
	}

	float cpokedex_mgr::get_add_percent()
	{
		float add_percent = 1;
		cpokedex* main_pokedex_ptr = get_palace_pokedex();
		if (nullptr == main_pokedex_ptr)
		{
			return add_percent;
		}
		PokedexTemplate* pokedex_template_ptr = main_pokedex_ptr->get_pokedex_info_ptr();
		if (nullptr == pokedex_template_ptr)
		{
			return add_percent;
		}
		if (pokedex_template_ptr->AttArray.size() > 0)
		{
			add_percent = pokedex_template_ptr->AttArray[0] + 1;
		}
		if (add_percent < 1)
		{
			return 1;
		}
		return add_percent;
	}


	void cpokedex_mgr::save_pokedex_to_db(int32 save_type)
	{
 		if (nullptr == m_player_ptr)
		{
			return;
		}

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_pokedexs);
		if (is_use == false)
		{

			cs2dp_save_char_pokedex req;
			req.save_type_ex = save_type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			uint32 pokedex_count = 0;

			for (uint32 i = 0; i < POKEDEX_NUM_MAX; ++i)
			{
				if (m_pokedex_ary[i].get_data_info(e_pokedex_Info_template_id) > 0)
				{
					req.data_list[i] = m_pokedex_ary[i].get_data_info();
					pokedex_count++;
				}

			}
			req.data_num = pokedex_count;

			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			cs2dp_proto::save_role_pokedex msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(save_type);
			
			cs2dp_proto::role_pokedex_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}

			uint32 row_count = 0;

			for (uint32 i = 0; i < POKEDEX_NUM_MAX; ++i)
			{
				if (m_pokedex_ary[i].get_data_info(e_pokedex_Info_template_id) > 0)
				{
					s_pokedex_info&tmp = m_pokedex_ary[i].get_data_info();
					cs2dp_proto::role_pokedex_row *db_row = db_data->add_row_data();
					if (db_row == nullptr)
					{
						return;
					}
					for (int32 j = 0; j < e_pokedex_Info_max; j++)
					{
						db_row->add_data_ary(tmp.data_ary[j]);
					}
					for (int32 k = 0; k < PER_POKEDEX_PSYCHE_NUM; k++)
					{
						db_row->add_collect_num(tmp.collect_num[k]);
					}
					row_count++;
				}
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_pokedexs);

		}
	}

	int32 cpokedex_mgr::get_all_active_pokedex_num()
	{
		int32 pokedex_num = 0;
		for (int32 i = 0; i < POKEDEX_NUM_MAX;++i)
		{
			if (m_pokedex_ary[i].is_active())
			{
				pokedex_num++;
			}
		}
		return pokedex_num;
	}

	int32 cpokedex_mgr::get_active_pokedex_num()
	{
		int32 total_active_num = 0;
		for (int32 i = 0; i < POKEDEX_NUM_MAX; ++i)
		{
			total_active_num += m_pokedex_ary[i].get_active_num();
		}
		return total_active_num;
	}

	void cpokedex_mgr::init_manager(const s_pokedex_info* pokedex_data, int32 pokedex_num)
	{
		PlayerInitTemplate* player_init_template_ptr = nullptr;
		if (m_player_ptr->is_valid())
		{
			player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, m_player_ptr->get_unit_info(e_role_info_template_id));
			if (player_init_template_ptr)
			{
				for (int32 i = 0; i < POKEDEX_NUM_MAX; ++i)
				{
					if (i < player_init_template_ptr->InitPokedex.size())
					{
						int32 is_contain = 0;
						for (int32 j = 0; j < pokedex_num; ++j)
						{
							if (player_init_template_ptr->InitPokedex[i] == pokedex_data[j].data_ary[e_pokedex_Info_template_id])
							{
								m_pokedex_ary[i].set_pokedex_info(pokedex_data[j]);
								m_pokedex_ary[i].set_pokedex_template_id(player_init_template_ptr->InitPokedex[i]);
								is_contain = 1;
							}

						}
						if (is_contain == 0)
						{
							m_pokedex_ary[i].set_pokedex_template_id(player_init_template_ptr->InitPokedex[i]);
						}
					}
				}
			}
		}
	}

	void cpokedex_mgr::load_pokedex_from_db(const s_pokedex_info* pokedex_data, int32 pokedex_num)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		init_manager(pokedex_data, pokedex_num);
		float add_percent = get_add_percent();
		for (int32 i = 0; i < pokedex_num; i++)
		{
			PokedexTemplate* temp_pokedex_template = m_pokedex_ary[i].get_pokedex_info_ptr();
			if (m_pokedex_ary[i].get_data_info(e_pokedex_Info_state)
				&& temp_pokedex_template != nullptr)
			{
				m_player_ptr->get_item_set().item_change_att(temp_pokedex_template->AttArray, add_percent, true);
				int32 fetter_num = temp_pokedex_template->FetterArr.size();
				if (temp_pokedex_template->Type == 1 && fetter_num > 0)
				{
					for (int32 j = 0; j < fetter_num; ++j)
					{
						if (m_pokedex_ary[i].get_data_info(e_pokedex_Info_fetter_state_1 + j) <= 0)
						{
							continue;
						}
						PsycheTemplate* psyche_template_ptr = GET_TEMPLATE(PsycheTemplate, temp_pokedex_template->FetterArr[j]);
						if (nullptr == psyche_template_ptr)
						{
							continue;
						}
						m_player_ptr->get_item_set().item_change_att(psyche_template_ptr->FetterAttArray, add_percent, true);
					}
				}

			}
		}
		send_pokedex_all();	
	}
	bool cpokedex_mgr::load_pokedex_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		cs2dp_proto::role_pokedex_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_pokedex_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_pokedex_info *p_row = (s_pokedex_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			cs2dp_proto::role_pokedex_row db_row = msg.row_data(i);

			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}

			p_row++;
		}
		load_pokedex_from_db((const s_pokedex_info *)p_data, msg.row_count());
		return true;

	}
}
