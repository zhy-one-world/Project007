#include "feather_heart.h"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "logic/unit_man.h"
#include "logic/buff_man.h"
namespace hld
{
	void feather_heart::clear_data()
	{
		m_trigger_event = false;
		m_template_id = 0;
		m_owner_index = -1;
		m_item_guid = 0;
		m_feather_mgr = nullptr;

		for (int32 i = 0; i < e_feather_have_skill_max; i++)
		{
			m_have_skill[i] = 0;
		}

	}
	bool feather_heart::on_event(s_trigger_param& trigger_param)
	{
		//return false;
		FeatherTemplate *feather_template_ptr = get_feather_template();
		if (feather_template_ptr == nullptr)
		{
			return false;

		}
		return game_event_handler::on_event(m_owner_index, trigger_param, feather_template_ptr->EventCondition);
	}
	void feather_heart::feather_event_param(int32 skill_template_id, int64 skill_cd, bool apply)
	{
		FeatherTemplate *feather_template_ptr = get_feather_template();
		if (feather_template_ptr == nullptr)
		{
			return ;

		}
		change_event_param(m_owner_index, skill_template_id, skill_cd, apply, feather_template_ptr->EventCondition);
	}
	void feather_heart::trigger_event(int32 target_index, bool is_add)
	{
		if (is_add)
		{//触发做个标记
			m_trigger_event = true;
		}
		if (m_trigger_event == false && is_add == false)
		{//如果没有触发过，就不做删除操作
			return;
		}
		FeatherTemplate *feather_template_ptr = get_feather_template();
		if (feather_template_ptr == nullptr)
		{
			return;
		}
		if (is_add)
		{
			for (int32 i = 0; i < feather_template_ptr->BuffArray.size(); ++i)
			{
			     buff_man::add_buff_inst(m_owner_index, target_index, feather_template_ptr->BuffArray[i]);
			}
			clear_skill();
			for (int32 i = 0; i < e_feather_have_skill_max; ++i)
			{
				int32 buffid = get_feather_slot_startbuff(i);
				if (buffid != 0)
				{
					buff_man::add_buff_inst(m_owner_index, target_index, buffid);
					set_skill(i , buffid);
				}
			}
		}
		else
		{
			for (int32 i = 0; i < feather_template_ptr->BuffArray.size(); ++i)
			{
				buff_man::del_buff_by_template_id(m_owner_index, target_index, feather_template_ptr->BuffArray[i]);
			}
			for (int32 i = 0; i < e_feather_have_skill_max ; ++i)
			{
				if (m_have_skill[i] != 0)
				{
					buff_man::del_buff_by_template_id(m_owner_index, target_index, m_have_skill[i]);
				}	
			}
		}
	}
	void feather_heart::set_base_info(feather_heart_mgr *in_feather_mgr_ptr, int32 in_owner_index, int32 in_template_id, guid_64 in_item_guid)
	{
		m_feather_mgr = in_feather_mgr_ptr;
		m_owner_index = in_owner_index;
		m_template_id = in_template_id;
		m_item_guid   = in_item_guid;
		
	}
	bool feather_heart::is_use()
	{
		return m_template_id != 0;
	}

	int32 feather_heart::get_template_id()
	{
		return m_template_id;
	}


	void feather_heart::set_skill(int32 slot, int32 buffid)
	{
		if (slot < 0 || slot >= e_feather_have_skill_max)
			return;

		m_have_skill[slot] = buffid;
	}
	void feather_heart::clear_skill()
	{
		for (int32 i = 0; i < e_feather_have_skill_max; i++)
		{
			m_have_skill[i] = 0;
		}
	}

	FeatherTemplate *feather_heart::get_feather_template()
	{
		player& temp_player = unit_man::get_player(m_owner_index);
		if (temp_player.is_valid() == false)
		{
			return nullptr;
		}
	    citem * item_ptr = temp_player.get_item_set().get_item(m_item_guid);
		if (item_ptr == nullptr)
		{
			return nullptr;
		}
		int32 now_feather_count = item_ptr->get_data_info(e_item_info_upgrade_count);
		int32 real_id = item_ptr->get_item_info_ptr()->logic_id + now_feather_count;
		FeatherTemplate* feather_upgrade_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
		return feather_upgrade_ptr;
	}

	int32 feather_heart::get_feather_slot_startbuff(int32 slot)
	{
		if (m_feather_mgr == nullptr)
		{
			return 0;

		}
		player& temp_player = unit_man::get_player(m_owner_index);
		if (temp_player.is_valid() == false)
		{
			return 0;
		}
		citem * item_ptr = temp_player.get_item_set().get_item(m_item_guid);
		if (item_ptr == nullptr)
		{
			return 0;
		}
		int32 template_id =  item_ptr->get_data_info(e_item_info(feather_slot_start + slot * 2));
		int32 template_level = item_ptr->get_data_info(e_item_info(feather_slot_start + slot * 2 + 1));
		int32 template_add_level = m_feather_mgr->get_slot_add_level_by_item(item_ptr);
			//item_ptr->get_data_info(e_item_info(e_feather_core_add_level_start + slot));
	
		if (template_id == 0)
		{
			return 0;
		}
		GodHeartTemplate *god_ptr = m_feather_mgr->get_feather_heart_template_by_template_id(template_id);
		if (god_ptr == nullptr || god_ptr->Way == 1)
		{
			return 0;
		}
		if (god_ptr->StarBuffId.size() == 0 || god_ptr->StarBuffId.size() % 3 != 0)
		{
			return 0;
		}
		int32 total_level = template_level + template_add_level + 1;
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

	
}