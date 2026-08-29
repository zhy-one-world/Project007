#include "goddess_mgr.h"
#include "logic/unit_man.h"
#include "logic/buff_man.h"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "template/StringConst_S.h"
#include "lua/script_mgr.h"
#include "npc.hpp"

namespace faith
{
	goddess_mgr::goddess_mgr()
	{
		m_array_index = 0;
	}
	goddess_mgr::~goddess_mgr()
	{
	}

	bool goddess_mgr::goddess_gods_on(citem * item, int32 slot)
	{
		if (nullptr == item)
		{
			return false;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		DeityTemplate* deity_template_ptr = item->get_deity_info_ptr();
		if (nullptr == deity_template_ptr)
		{
			return false;
		}
		bool is_exchange = false;
		int32 exchange_slot = item->get_data_info(e_item_info_illusion_had_byte);
		if (item->get_data_info(e_item_info_activate) > 0)
		{
			is_exchange = true;
			goddess_gods_off(item, slot, true);
		}
		//if (item->get_data_info(e_item_info_activate) == 0)
		{
			citem* old_equip_goddess = get_cur_slot_goddess(slot);
			if (old_equip_goddess)
			{
				if (item->get_item_guid().is_valid() && old_equip_goddess->get_item_guid().is_valid() &&
					old_equip_goddess->get_item_guid() != item->get_item_guid())
				{
					if (!goddess_gods_off(old_equip_goddess, slot, true))
					{
						return false;
					}
					if (is_exchange)
					{
						goddess_gods_on(old_equip_goddess, exchange_slot);
					}
				}
			}
			item->set_data_info(e_item_info_activate, 1);
			item->set_data_info(e_item_info_illusion_had_byte, slot);//记录上阵位置
			if (slot > 0)
			{
				calcu_cooperate_buff(item, true);//设置完出战位置之后再计算
			}
			if (slot == 0 && item->get_data_info(e_item_info_starskill_flag) == 0)
			{
				goddess_fight(item->get_item_guid());//主出战位操作
			}
			player_ref.get_item_set().send_item_one(item);
			return true;
		}
		return false;
	}

	bool goddess_mgr::goddess_gods_off(citem * item, int32 slot, bool is_auto_off)
	{
		if (nullptr == item)
		{
			return false;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		DeityTemplate* deity_template_ptr = item->get_deity_info_ptr();
		if (nullptr == deity_template_ptr)
		{
			return false;
		}
		if (item->get_data_info(e_item_info_activate) > 0)
		{
			int32 real_slot = item->get_data_info(e_item_info_illusion_had_byte);
			if (real_slot > 0)
			{
				calcu_cooperate_buff(item, false);
			}
			item->set_data_info(e_item_info_activate, 0);
			
			if (real_slot == 0)
			{
				goddess_fight(item->get_item_guid());//主出战位操作
			}
			player_ref.get_item_set().send_item_one(item);
			return true;
		}
		return false;
	}

	int32 goddess_mgr::goddess_fight(const guid_64& item_guid)
	{
		int32 i_ret = e_item_string_unkown;
		script_mgr::get_instance().call_func("goddess_mgr", "goddess_fight_cpp", 1, false, "%d%d%d>%d", m_array_index, item_guid.A, item_guid.B, &i_ret);
		return i_ret;
		/*player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return e_item_string_unkown;
		}
		citem* goddess_item = player_ref.get_item_set().get_item(item_guid);
		if (nullptr == goddess_item)
		{
			return e_item_string_unkown;
		}
		if (goddess_item->get_data_info(e_item_info_is_first) <= 0)
		{
			return e_item_string_unkown;
		}
		DeityTemplate* deity_template_ptr = goddess_item->get_deity_info_ptr();
		if (nullptr == deity_template_ptr)
		{
			return e_item_string_unkown;
		}
		citem* old_fight_goddess = get_fighting_goddess();
		if (nullptr != old_fight_goddess)
		{
			if (old_fight_goddess->get_item_guid() == item_guid)
			{
				goddess_item->set_data_info(e_item_info_starskill_flag, 0);
				goddess_item->set_data_info(e_item_info_activate, 0);
				player_ref.kill_pet_by_type(e_summon_pet_type_goddess);
				player_ref.get_item_set().send_item_one(goddess_item);
				calcu_cooperate_buff(goddess_item, false);
				return 0;
			}
			old_fight_goddess->set_data_info(e_item_info_starskill_flag, 0);
			old_fight_goddess->set_data_info(e_item_info_activate, 0);
			player_ref.get_item_set().send_item_one(old_fight_goddess);
			calcu_cooperate_buff(old_fight_goddess, false);
		}
		goddess_item->set_data_info(e_item_info_starskill_flag, 1);
		goddess_item->set_data_info(e_item_info_activate, 1);
		goddess_item->set_data_info(e_item_info_illusion_had_byte, 0);
		player_ref.get_item_set().send_item_one(goddess_item);
		player_ref.summon_pet(deity_template_ptr->NpcId);
		calcu_cooperate_buff(goddess_item, true);
		calcu_goddess_buff(goddess_item, true);
		return 0;*/
	}

	citem * goddess_mgr::get_fighting_goddess()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		for (int32 i = 0; i < max_goddess_equip_num; i++)
		{
			citem* temp_item = player_ref.get_item_set().get_item_by_slot(e_bag_type_goddess_equip, i);
			if (temp_item == nullptr)
			{
				continue;
			}
			ItemTemplate* temp_item_template = temp_item->get_item_info_ptr();
			if (nullptr == temp_item_template)
			{
				continue;
			}
			if (temp_item->get_data_info(e_item_info_starskill_flag) == 1 && temp_item_template->item_type == e_item_type_goddess_equip
				&& temp_item_template->sub_type == 0)
			{
				return temp_item;
			}
		}
		return nullptr;
	}

	citem * goddess_mgr::get_cur_slot_goddess(int32 slot)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		for (int32 i = 0; i < max_goddess_equip_num; i++)
		{
			citem* temp_item = player_ref.get_item_set().get_item_by_slot(e_bag_type_goddess_equip, i);
			if (temp_item == nullptr)
			{
				continue;
			}
			ItemTemplate* temp_item_template = temp_item->get_item_info_ptr();
			if (nullptr == temp_item_template)
			{
				continue;
			}
			if (temp_item->get_data_info(e_item_info_illusion_had_byte) == slot && temp_item_template->item_type == e_item_type_goddess_equip
				&& temp_item_template->sub_type == 0 && temp_item->get_data_info(e_item_info_activate) > 0)
			{
				return temp_item;
			}
		}
		return nullptr;
	}

	citem * goddess_mgr::get_cur_slot_equip(int32 goddess_type,int32 slot)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		for (int32 i = 0; i < max_goddess_equip_num; i++)
		{
			citem* temp_item = player_ref.get_item_set().get_item_by_slot(e_bag_type_goddess_equip, i);
			if (temp_item == nullptr)
			{
				continue;
			}
			ItemTemplate* temp_item_template = temp_item->get_item_info_ptr();
			if (nullptr == temp_item_template)
			{
				continue;
			}
			DeityTemplate* deity_template_ptr = temp_item->get_deity_info_ptr();
			if (nullptr == deity_template_ptr)
			{
				continue;
			}
			if (temp_item->get_data_info(e_item_info_slot) == slot && temp_item_template->item_type == e_item_type_goddess_equip
				&& temp_item_template->sub_type > 0 && deity_template_ptr->Type == goddess_type)
			{
				return temp_item;
			}
		}
		return nullptr;
	}

	citem * goddess_mgr::get_select_type_goddess(int32 type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		for (int32 i = 0; i < max_goddess_equip_num; i++)
		{
			citem* temp_item = player_ref.get_item_set().get_item_by_slot(e_bag_type_goddess_equip, i);
			if (temp_item == nullptr)
			{
				continue;
			}
			ItemTemplate* temp_item_template = temp_item->get_item_info_ptr();
			if (nullptr == temp_item_template)
			{
				continue;
			}
			DeityTemplate* deity_template_ptr = temp_item->get_deity_info_ptr();
			if (nullptr == deity_template_ptr)
			{
				continue;
			}
			if (temp_item_template->item_type == e_item_type_goddess_equip&& temp_item_template->sub_type == 0 && deity_template_ptr->Type == type)
			{
				return temp_item;
			}
		}
		return nullptr;
	}

	void goddess_mgr::calcu_cooperate_buff(citem * item_ptr, bool is_add)
	{
		if (nullptr == item_ptr)
		{
			return;
		}
		ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
		if (nullptr == item_template_ptr)
		{
			return;
		}
		DeityTemplate* deity_template_ptr = item_ptr->get_deity_info_ptr();
		if (nullptr == deity_template_ptr)
		{
			return;
		}
		if (item_template_ptr->sub_type != 0)
		{
			return;
		}
		int32 effect_buff_type = item_ptr->get_data_info(e_item_info_illusion_had_byte);
		if (effect_buff_type < 0 || effect_buff_type >= e_branch_type_dark)
		{
			return;
		}
		if (deity_template_ptr->CooperateBuff.size() < e_branch_type_dark)
		{
			return;
		}
		if (is_add)
		{
			buff_man::add_buff_inst(m_array_index, m_array_index, deity_template_ptr->CooperateBuff[effect_buff_type]);
		}
		else
		{
			buff_man::del_buff_by_template_id(m_array_index, m_array_index, deity_template_ptr->CooperateBuff[effect_buff_type]);
		}

	}

	void goddess_mgr::calcu_goddess_buff(citem* item_ptr, bool is_add)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}
		if (nullptr == item_ptr)
		{
			return;
		}
		if (item_ptr->get_data_info(e_item_info_is_first) == 0)
		{
			return;
		}
		DeityTemplate* template_ptr = item_ptr->get_deity_info_ptr();
		if (nullptr == template_ptr)
		{
			return;
		}
		npc* goddess_npc = player_ref.get_pet_ptr_by_type(e_summon_pet_type_goddess);
		if (nullptr == goddess_npc)
		{
			return;
		}
		if (template_ptr->NpcId != goddess_npc->get_npc_template_id())
		{
			return;
		}
		int32 array_index = goddess_npc->get_array_index();
		for (int32 i = 0;i < max_goddess_equip_num;i++)
		{
			citem* temp_item = player_ref.get_item_set().get_item_by_slot(e_bag_type_goddess_equip, i);
			if (temp_item == nullptr)
			{
				continue;
			}
			ItemTemplate* temp_item_template = temp_item->get_item_info_ptr();
			if (nullptr == temp_item_template)
			{
				continue;
			}
			DeityTemplate* deity_template_ptr = temp_item->get_deity_info_ptr();
			if (nullptr == deity_template_ptr)
			{
				continue;
			}
			if (temp_item_template->sub_type > 0 && deity_template_ptr->Type == template_ptr->Type)
			{
				if (template_ptr->AdditionBuff.size() >= per_goddess_type_equip_num)
				{
					if (is_add)//装备所携带的光效buff
					{
						buff_man::add_buff_inst(array_index, array_index, template_ptr->AdditionBuff[temp_item_template->sub_type - 1]);
					}
					else
					{
						buff_man::del_buff_by_template_id(array_index, array_index, template_ptr->AdditionBuff[temp_item_template->sub_type - 1]);
					}
				}
				if (temp_item_template->sub_type == 1 || temp_item_template->sub_type == 2 || temp_item_template->sub_type == 6)
				{
					for (int32 j = 0; j < deity_template_ptr->AdditionBuff.size(); j++)
					{
						if (is_add)//装备所携带的技能属性buff
						{
							buff_man::add_buff_inst(array_index, array_index, deity_template_ptr->AdditionBuff[j]);
						}
						else
						{
							buff_man::del_buff_by_template_id(array_index, array_index, deity_template_ptr->AdditionBuff[j]);
						}
					}
				}
			}
		}
		{
			if (item_ptr->get_data_info(e_item_info_succinct_level) == 0)
			{
				return;
			}
			DeityTemplate *star_deity_template_ptr = GET_TEMPLATE(DeityTemplate, template_ptr->StarID + item_ptr->get_data_info(e_item_info_stargrade_count));
			if (star_deity_template_ptr == nullptr)
			{
				return;
			}
			if (is_add)
			{
				buff_man::add_buff_inst(array_index, array_index, star_deity_template_ptr->ShowBuff);

			}
			else
			{
				buff_man::del_buff_by_template_id(array_index, array_index, star_deity_template_ptr->ShowBuff);
			}
		}
	}

	void goddess_mgr::enter_reload_goddess()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		citem* goddess_item = get_fighting_goddess();
		if (nullptr == goddess_item)
		{
			return;
		}
		DeityTemplate* deity_template_ptr = goddess_item->get_deity_info_ptr();
		if (nullptr == deity_template_ptr)
		{
			return;
		}
		player_ref.summon_pet(deity_template_ptr->NpcId);
		calcu_goddess_buff(goddess_item,true);
	}

	void goddess_mgr::set_star_is_open(citem *item_ptr, bool is_send)
	{
		if (nullptr == item_ptr)
		{
			return;
		}
		ItemTemplate * item_info_ptr = item_ptr->get_item_info_ptr();
		if (item_ptr == nullptr)
		{
			return;
		}
		DeityTemplate* template_ptr = item_ptr->get_deity_info_ptr();
		if (nullptr == template_ptr)
		{
			return;
		}
		if (item_ptr->get_data_info(e_item_info_succinct_level) == 1 || item_ptr->get_data_info(e_item_info_is_first) <= 0)
		{
			return;
		}

		int32 start_index = e_branch_type_dark + (template_ptr->Type - 1) * per_goddess_type_equip_num;
		int32 end_index = start_index + per_goddess_type_equip_num - 1;

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		bool is_open = true;
		for (int32 i = start_index; i <= end_index; i++)
		{
			citem* temp_item = player_ref.get_item_set().get_item_by_slot(e_bag_type_goddess_equip, i);
			if (temp_item == nullptr)
			{
				is_open = false;
				break;
			}
		}
		if (!is_open)
		{
			return;
		}
		
		player_ref.get_item_set().set_goddess_equip_att(item_ptr, false);
		item_ptr->set_data_info(e_item_info_succinct_level, 1);
		player_ref.get_item_set().set_goddess_equip_att(item_ptr, true);

		if (is_send)
		{
			player_ref.get_item_set().send_item_one(item_ptr);
		}


		
	}



}