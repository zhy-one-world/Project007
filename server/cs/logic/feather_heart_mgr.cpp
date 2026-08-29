#include "feather_heart_mgr.h"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "utility/globle_data.h"
#include "utility/init_unit.h"

namespace faith
{
	feather_heart_mgr::feather_heart_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	feather_heart_mgr::~feather_heart_mgr()
	{
	}

	void feather_heart_mgr::clear_data()
	{
		for (int32 i =0 ; i < e_feather_heart_max; ++i)
		{
			m_feather_heart[i].clear_data();
		}
	}

	void feather_heart_mgr::set_player_ptr(player *in_player)
	{
		m_player_ptr = in_player;
	}

	void feather_heart_mgr::feather_event_param(int32 skill_template_id, int64 skill_cd, bool apply, int32 template_id)
	{

		feather_heart * feather_heart_ptr = get_slot_by_template_id(template_id);
		if (feather_heart_ptr == nullptr)
		{
			return;
		}
		feather_heart_ptr->feather_event_param(skill_template_id, skill_cd, apply);

	}
	GodHeartTemplate* feather_heart_mgr::get_feather_heart_template(citem& feather_heart)
	{
		if (!feather_heart.get_item_info_ptr())
		{
			return nullptr;
		}
		return GET_TEMPLATE(GodHeartTemplate, feather_heart.get_item_info_ptr()->logic_id);	 
	}
	GodHeartTemplate * feather_heart_mgr::get_feather_heart_template_by_template_id(int32 template_id)
	{
	   ItemTemplate *item_ptr = GET_TEMPLATE(ItemTemplate, template_id);
	   if (!item_ptr)
		   return nullptr;
	   return GET_TEMPLATE(GodHeartTemplate, item_ptr->logic_id);
	}
	void feather_heart_mgr::set_feather_heart_attr(int32 template_id, int32 level, bool is_add)
	{
		GodHeartTemplate *god_template = get_feather_heart_template_by_template_id(template_id);
		if (god_template == nullptr)
			return;

		if (m_player_ptr == nullptr)
			return;

		m_player_ptr->get_pawn_att().apply_att_change_by_array(god_template->BasicAtt, is_add);
		m_player_ptr->get_pawn_att().apply_att_change_by_array(god_template->UpgradeAttEach, is_add, level);
	}
	void feather_heart_mgr::set_feather_heart_attr_all(bool is_add)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		std::vector<citem*> temp_feather_array = m_player_ptr->get_item_set().get_wing_feather();

		for (int32 i = 0 ; i < temp_feather_array.size(); i++)
		{
			citem* temp_wing_feather = temp_feather_array[i];
			if (temp_wing_feather == nullptr)
			{
				continue;
			}
			set_feather_heart_attr_all(*temp_wing_feather, is_add);
		}
	}

	void feather_heart_mgr::set_feather_heart_attr_all(citem &item, bool is_add)
	{

		if (item.get_data_info(e_item_info_activate) <= 0)
		{
			return;
		}
		int32 template_core_add_level = get_slot_add_level_by_item(&item);
		for (int32 i = e_feather_heart_0; i <= e_feather_heart_3; i++)
		{
			int32 template_id = item.get_data_info((e_item_info)(feather_slot_start + i * 2));
			int32 template_level = item.get_data_info((e_item_info)(feather_slot_start + i * 2 + 1));
			int32 add_level = 0;
			if (i != e_feather_heart_3)
			{
				add_level = template_core_add_level;
			}
			if (template_id == 0)
			{
				continue;
			}
			set_feather_heart_attr(template_id, template_level + add_level, is_add);
			if (is_add)
			{
				add_buff_att(template_id, template_level, add_level);
			}
			else
			{
				dec_buff_att(template_id, template_level, add_level);
			}
		}
	}

	void feather_heart_mgr::put_feather_heart_in_bag(int32 template_id, int32 level)
	{
		if (m_player_ptr == nullptr)
			return;

	   citem* p_new_item =	m_player_ptr->get_item_set().create_item_by_template(e_server_log_add_item_add_item_to_bag, 0,template_id);
	   if (p_new_item == nullptr)
		   return;
	   p_new_item->set_data_info(e_item_info_upgrade_count, level);

	   m_player_ptr->get_item_set().put_in_bag(p_new_item, e_bag_type_none, e_item_mail_type_feather_heart_put_bag);

	}

	feather_heart * feather_heart_mgr::get_empty_slot()
	{

		for (int32 i = 0; i < e_feather_heart_max; i++)
		{
			if (!m_feather_heart[i].is_use())
			{
				return m_feather_heart + i;
			}	
		}
		return nullptr;
	}
	feather_heart * feather_heart_mgr::get_slot_by_template_id(int32 template_id)
	{
		 for (int32 i = 0; i < e_feather_heart_max ; i++)
		 {
			 if (m_feather_heart[i].get_template_id() == template_id)
			 {
				 return  m_feather_heart + i;
			 }
		 }
		 return nullptr;

	}

	void feather_heart_mgr::init_set_feather_skill_info()
	{
		if (m_player_ptr == nullptr)
			return;

		std::vector<citem*> temp_feather_array =  m_player_ptr->get_item_set().get_wing_feather();

		for (int32 i = 0; i < temp_feather_array.size(); ++i)
		{
			citem* temp_wing_feather = temp_feather_array[i];
			ItemTemplate *item_template_ptr = temp_wing_feather->get_item_info_ptr();
			if (item_template_ptr == nullptr)
			{
				continue;
			}
			GodHeartTemplate *god_templater_ptr = get_feather_heart_template_by_template_id(item_template_ptr->FashionId);
			if (god_templater_ptr == nullptr)
			{
				continue;
			}
			feather_heart * feather_heart_ptr = get_empty_slot();
			if (feather_heart_ptr == nullptr)
			{  // error
				return;
			}
			feather_heart_ptr->set_base_info(this, m_player_ptr->get_array_index(), temp_wing_feather->get_data_info(e_item_info_info_id), temp_wing_feather->get_item_guid());
		}
	}
	int32 feather_heart_mgr::get_slot_add_level_by_item(citem *item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return 0;
		}
		int32 core_template_id = item_ptr->get_data_info((e_item_info)(feather_slot_start + e_feather_heart_3 * 2));
		int32 core_template_level = item_ptr->get_data_info((e_item_info)(feather_slot_start + e_feather_heart_3 * 2 + 1));

		return get_slot_add_level_by_template_id(core_template_id, core_template_level + 1);



	}

	int32 feather_heart_mgr::get_slot_add_level_by_template_id(int32 template_id, int32 level)
	{
		GodHeartTemplate *god_ptr = get_feather_heart_template_by_template_id(template_id);
		if (god_ptr == nullptr)
		{
			return 0;
		}

		return get_slot_add_level_by_core(god_ptr->ChangeLevel, level);	
	}

	int32 feather_heart_mgr::get_slot_add_level_by_core(const std::vector<int32>& change_level_vec, int32 level)
	{
		int32 add_level = 0;
		int32 num = change_level_vec.size();

		if (num == 0 || num % 3 != 0)
		{
			return add_level;
		}

		for (int32 i = 0; i < num; i++)
		{
			int32 start_level = change_level_vec[i * 3 + 0];
			int32 end_level = change_level_vec[i * 3 + 1];
			if (start_level <= level && level <= end_level)
			{
				add_level = change_level_vec[i * 3 + 2];
				break;
			}
		}
		
		return add_level;	
	}

	void feather_heart_mgr::sync_ranking_feather_heart_data()
	{
		if (nullptr == m_player_ptr)
			return;

		const double* att_array = m_player_ptr->m_pawn_att.get_attack_att_all();
		int64 ranking_value = 0;
		std::vector<citem*> temp_feather_array = m_player_ptr->get_item_set().get_wing_feather();

		for (int32 index = 0; index < temp_feather_array.size(); index++)
		{
			citem* temp_wing_feather = temp_feather_array[index];
			if (temp_wing_feather == nullptr)
				continue;
			if (temp_wing_feather->get_data_info(e_item_info_activate) <= 0)
				continue;

			//计算本身加成
			{
				auto feather_template_ptr = temp_wing_feather->get_item_info_ptr();
				if (nullptr == feather_template_ptr)
					continue;
				int32 real_id = feather_template_ptr->logic_id + temp_wing_feather->get_data_info(e_item_info_upgrade_count);
				FeatherTemplate* feather_att_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
				if (nullptr == feather_att_ptr)
					continue;
				ranking_value += init_unit::calcu_fighting_power_by_att_array(feather_att_ptr->AttArray, att_array, 1.0, false);
			}
			//计算强化功能加成
			{
				int32 template_core_add_level = get_slot_add_level_by_item(temp_wing_feather);
				for (int32 i = e_feather_heart_0; i <= e_feather_heart_3; i++)
				{
					int32 template_id = temp_wing_feather->get_data_info((e_item_info)(feather_slot_start + i * 2));
					int32 template_level = temp_wing_feather->get_data_info((e_item_info)(feather_slot_start + i * 2 + 1));
					int32 add_level = 0;

					if (i != e_feather_heart_3)
						add_level = template_core_add_level;
					if (template_id == 0)
						continue;
					GodHeartTemplate *god_template = get_feather_heart_template_by_template_id(template_id);
					if (god_template == nullptr)
						continue;

					ranking_value += init_unit::calcu_fighting_power_by_att_array(god_template->BasicAtt, att_array);
					ranking_value += init_unit::calcu_fighting_power_by_att_array(god_template->UpgradeAttEach, att_array, template_level + add_level);
				}
			}
		}

		m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_feather_heart, ranking_value);
	}

	int32 feather_heart_mgr::get_buff_id(int32 template_id, int32 level, int32 add_level)
	{
		GodHeartTemplate *god_ptr = get_feather_heart_template_by_template_id(template_id);
		if (god_ptr == nullptr || god_ptr->Way == 0)
		{
			return 0;
		}
		if (god_ptr->StarBuffId.size() == 0 || god_ptr->StarBuffId.size() % 3 != 0)
		{
			return 0;
		}
		int32 total_level = level + add_level + 1;
		for (int32 i = 0; i < god_ptr->StarBuffId.size() / 3; i++)
		{
			int32 start_level = god_ptr->StarBuffId[i * 3 + 0];
			int32 end_level = god_ptr->StarBuffId[i * 3 + 1];
			if (start_level <= total_level  && total_level <= end_level)
			{
				return god_ptr->StarBuffId[i * 3 + 2];
			}
		}
		return 0;

	}

	void feather_heart_mgr::add_buff_att(int32 template_id, int32 level, int32 add_level)
	{
		if (template_id <= 0)
		{
			return;
		}
		int32 buff_id = get_buff_id(template_id, level, add_level);
		if (buff_id <= 0)
		{
			return;
		}
		buff_man::add_buff_inst(m_player_ptr->get_array_index(), m_player_ptr->get_array_index(), buff_id);

	}

	void feather_heart_mgr::dec_buff_att(int32 template_id, int32 level,int32 add_level)
	{
		if (template_id <= 0)
		{
			return;
		}
		int32 buff_id = get_buff_id(template_id, level, add_level);
		if (buff_id <= 0)
		{
			return;
		}
		buff_man::del_buff_by_template_id(m_player_ptr->get_array_index(), m_player_ptr->get_array_index(), buff_id);

	}
}