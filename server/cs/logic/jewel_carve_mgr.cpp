#include "jewel_carve_mgr.h"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "system/item/item_system.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "utility/random.h"
#include "utility/init_unit.h"

namespace hld
{
	jewel_carve_mgr::jewel_carve_mgr()
	{
		clear_data();
	}
	jewel_carve_mgr::~jewel_carve_mgr()
	{

	}

	void jewel_carve_mgr::clear_data()
	{

	}
	void jewel_carve_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	void jewel_carve_mgr::on_oper(guid_64 item_guid, int32 type, int32 param)
	{

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (player_ref.get_func_unlock_mgr().is_func_unlock("UIJewelCarve") == false)
		{
			oper_result(player_ref, item_guid, type, jewel_carve_errror_open_level);
			return;
		}

		if (get_jewel_open_on_num(player_ref, item_guid) < get_config_param(e_jewel_carve_param_need_jewel_num))
		{
			oper_result(player_ref, item_guid, type, jewel_carve_error_need_jewel_num);
			return;
		}

		int32 result = 0;
		switch (type)
		{
		case e_jewel_carve_oper_random:
			result = on_random(player_ref,item_guid);
			break;
		case e_jewel_carve_oper_up:
			result = on_up(player_ref,item_guid);
			break;
		case e_jewel_carve_oper_cancel:
			result = on_cancel(player_ref, item_guid);
			break;
		case e_jewel_carve_oper_choose:
			result = on_choose(player_ref,item_guid, param);
			break;
		default:
			break;
		}

		oper_result(player_ref, item_guid, type, result);


	}

	void jewel_carve_mgr::oper_result(player &player_ref, guid_64 item_guid, int32 type, int32 result)
	{
		item_proto_item_jewel_carve_oper_end msg;
		msg.add_item_guid(item_guid.A);
		msg.add_item_guid(item_guid.B);
		msg.set_oper_type(type);
		msg.set_result(result);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_item_jewel_carve_oper_end);
	}

	int32 jewel_carve_mgr::on_random(player &player_ref, guid_64 item_guid)
	{
		citem *item_ptr = player_ref.get_item_set().get_item(item_guid);

		JewelCarveTemplate * jewel_carve_tmplate_ptr = get_jewel_carve_tmplate_by_guid_id(item_guid);
		if (item_ptr == nullptr || jewel_carve_tmplate_ptr == nullptr)
		{
			return jewel_carve_error_config;
		}
		if (jewel_carve_tmplate_ptr->Consume.size() != 2
			|| jewel_carve_tmplate_ptr->RandomSkillSet.size() == 0
			|| jewel_carve_tmplate_ptr->RandomSkillSet.size() % 2 != 0)
		{
			return jewel_carve_error_unknow;
		}
		if (item_ptr->get_data_info(e_item_info_jewel_carve_0) != 0)
		{
			return jewel_carve_error_unknow;
		}

		if (item_system::can_cost_item(&player_ref, e_bag_type_bag, jewel_carve_tmplate_ptr->Consume[0], jewel_carve_tmplate_ptr->Consume[1]) == false)
		{
			return jewel_carve_error_cost_item;
		}
		item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, jewel_carve_tmplate_ptr->Consume[0], jewel_carve_tmplate_ptr->Consume[1]);

		//////////////////////////////////////////////////////////////////////////////
		int32 num = jewel_carve_tmplate_ptr->RandomSkillSet.size();
		int32 total_weight = 0;
		for (int32 i = 0; i < num / 2; i++)
		{
			total_weight += jewel_carve_tmplate_ptr->RandomSkillSet[i * 2 + 1];
		}
		int32 pos = random_gen::get_random(0, total_weight);

		int32 cur_weight = 0;
		int32 skill_id = 0;
		for (int32 i = 0; i < num / 2; i++)
		{
			cur_weight += jewel_carve_tmplate_ptr->RandomSkillSet[i * 2 + 1];
			if (cur_weight >= pos)
			{
				skill_id = jewel_carve_tmplate_ptr->RandomSkillSet[i * 2];
				break;
			}
		}
		//////////////////////////////////////////////////////////////

		item_ptr->set_data_info(e_item_info_jewel_carve_0, skill_id);
		item_ptr->set_data_info(e_item_info_jewel_lucky_carve, 0);
		///////////////////////////////////////////////////////////

		player_ref.get_item_set().send_item_one(item_ptr, true);

		return jewel_carve_error_sucess;

	}

	int32 jewel_carve_mgr::on_up(player &player_ref,guid_64 item_guid)
	{
		citem *item_ptr = player_ref.get_item_set().get_item(item_guid);

		JewelCarveTemplate * jewel_carve_tmplate_ptr = get_jewel_carve_tmplate_by_guid_id(item_guid);
		if (item_ptr == nullptr || jewel_carve_tmplate_ptr == nullptr)
		{
			return jewel_carve_error_config;
		}
		int32 new_skill_id = item_ptr->get_data_info(e_item_info_jewel_carve_0);
		if ( new_skill_id == 0)
		{
			return jewel_carve_error_unknow;
		}
		if (is_add_lucky_same_skill(item_ptr))
		{
		    return on_cancel(player_ref, item_guid);

		}


		int32 idx = is_high_replace_low(item_ptr);
		if ( idx != -1)
		{
			return on_up_equ_skill_idx(player_ref, item_ptr, idx);
		}

		int32 skill_num = get_skill_num(item_ptr);
		if (skill_num == 0)
		{
			return on_up_equ_skill_idx(player_ref, item_ptr, e_item_info_jewel_carve_1);
		}
		
		if (skill_num == 1)
		{
			int32 pos = random_gen::get_random(1, 100);
			if (pos > get_config_param(e_jewel_carve_param_random_open_2) || item_ptr->get_data_info(e_item_info_jewel_lucky_carve) > 0)
			{
				return on_up_equ_skill_idx(player_ref, item_ptr, e_item_info_jewel_carve_2);
			}
			return on_up_equ_skill_idx(player_ref, item_ptr, e_item_info_jewel_carve_1);
		}

		if (skill_num == 2)
		{
			int32 pos = random_gen::get_random(1, 100);
			if (pos > get_config_param(e_jewel_carve_param_random_to_2))
			{
				return on_up_equ_skill_idx(player_ref, item_ptr, e_item_info_jewel_carve_2);
			}
			return on_up_equ_skill_idx(player_ref, item_ptr, e_item_info_jewel_carve_1);

		}

		return jewel_carve_error_unknow;

	}

	int32 jewel_carve_mgr::on_up_equ_skill_idx(player &player_ref, citem *item_ptr, int32 idx)
	{
		if (item_ptr == nullptr)
		{
			return jewel_carve_error_unknow;
		}

		if (idx != e_item_info_jewel_carve_1 && idx != e_item_info_jewel_carve_2)
		{
			return jewel_carve_error_unknow;
		}
		//int32 old_skill_id = item_ptr->get_data_info((e_item_info)idx);

		player_ref.get_item_set().set_equip_jewel_att(*item_ptr, false);

		int32 new_skill_id = item_ptr->get_data_info(e_item_info_jewel_carve_0);



		item_ptr->set_data_info((e_item_info)idx, new_skill_id);

		player_ref.get_item_set().set_equip_jewel_att(*item_ptr, true);



		item_ptr->set_data_info(e_item_info_jewel_carve_0, 0);
		item_ptr->set_data_info(e_item_info_jewel_lucky_carve, 0);


		player_ref.get_item_set().send_item_one(item_ptr, true);

		player_ref.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		player_ref.get_item_set().sync_whole_euip_fighting_power();

		return jewel_carve_error_sucess;
	}

	int32 jewel_carve_mgr::on_cancel(player &player_ref, guid_64 item_guid)
	{
		citem *item_ptr = player_ref.get_item_set().get_item(item_guid);

		JewelCarveTemplate * jewel_carve_tmplate_ptr = get_jewel_carve_tmplate_by_guid_id(item_guid);
		if (item_ptr == nullptr || jewel_carve_tmplate_ptr == nullptr)
		{
			return jewel_carve_error_config;
		}

		if (is_add_lucky_same_skill(item_ptr))
		{
			int32 value = player_ref.get_logic_data(e_role_logic_info_jewel_carve_lucky);
			if (value < get_config_param(e_jewel_carve_param_lucky_max))
			{
				player_ref.set_logic_data(e_role_logic_info_jewel_carve_lucky, value + 1);
				player_ref.send_logic_one(e_role_logic_info_jewel_carve_lucky);
			}
		}

		item_ptr->set_data_info(e_item_info_jewel_carve_0, 0);
		item_ptr->set_data_info(e_item_info_jewel_lucky_carve, 0);
		player_ref.get_item_set().send_item_one(item_ptr, true);


		return jewel_carve_error_sucess;
	}

	int32 jewel_carve_mgr::on_choose(player &player_ref,guid_64 item_guid, int32 param)
	{
		citem *item_ptr = player_ref.get_item_set().get_item(item_guid);

		JewelCarveTemplate * jewel_carve_tmplate_ptr = get_jewel_carve_tmplate_by_guid_id(item_guid);
		if (item_ptr == nullptr || jewel_carve_tmplate_ptr == nullptr)
		{
			return jewel_carve_error_config;
		}

		if (player_ref.get_logic_data(e_role_logic_info_jewel_carve_lucky) != get_config_param(e_jewel_carve_param_lucky_max))
		{
			return jewel_carve_error_not_enough_lucky;
		}
		if (!is_exist_skill(jewel_carve_tmplate_ptr->RandomSkillSet, param))
		{
			return jewel_carve_error_not_exist_skill_id;
		}

		item_ptr->set_data_info(e_item_info_jewel_carve_0, param);
		item_ptr->set_data_info(e_item_info_jewel_lucky_carve, 1);
		player_ref.set_logic_data(e_role_logic_info_jewel_carve_lucky, 0);
		player_ref.send_logic_one(e_role_logic_info_jewel_carve_lucky);


		player_ref.get_item_set().send_item_one(item_ptr, true);

		return jewel_carve_error_sucess;

	}

	int32 jewel_carve_mgr::get_config_param(e_jewel_carve_param e_type)
	{
		static int32 config_init[e_jewel_carve_param_max] = { 500, 10, 3 , 95, 50};

		if (GAMECONFIG->JewelCarveParam.size() < e_jewel_carve_param_max)
		{
			return config_init[e_type];
		}
		return GAMECONFIG->JewelCarveParam[e_type];
	}

	int32 jewel_carve_mgr::get_jewel_open_on_num(player &player_ref,guid_64 item_guid)
	{
		citem *item_ptr = player_ref.get_item_set().get_item(item_guid);
		if (item_ptr == nullptr)
		{
			return 0;
		}
		EquipTemplate *equip_info_ptr = item_ptr->get_equip_info_ptr();
		if (equip_info_ptr == nullptr)
		{
			return 0;
		}
		int32 count = 0;
		//for (int32 i= e_item_info_jewel_slot_0; i < e_item_info_jewel_vip_slot_0; i++)
		//{
		//	if (init_unit::is_can_use_this_jewel_slot_on_this_equip_level(equip_info_ptr->quality_level, i))
		//	{
		//		count++;
		//	}
		//}
		for (int32 j = e_item_info_jewel_vip_slot_0; j <= e_item_info_jewel_vip_slot_1; j++)
		{
			if (!init_unit::is_can_use_this_jewel_slot_on_this_vip_level(player_ref.get_vip_level(), j))
			{
				count++;
			}
		}
		return count;
	}

	JewelCarveTemplate* jewel_carve_mgr::get_jewel_carve_tmplate_by_id(int32 jewel_carve_id)
	{
		return GET_TEMPLATE(JewelCarveTemplate, jewel_carve_id);
	}

	JewelCarveTemplate* jewel_carve_mgr::get_jewel_carve_tmplate_by_guid_id(guid_64 item_guid)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		citem *item_ptr = player_ref.get_item_set().get_item(item_guid);
		if (item_ptr == nullptr)
		{
			return nullptr;
		}
		EquipTemplate *equip_info_ptr = item_ptr->get_equip_info_ptr();
		if (equip_info_ptr == nullptr)
		{
			return nullptr;
		}
		return get_jewel_carve_tmplate_by_id(equip_info_ptr->JewelCarveID);
	}

	bool jewel_carve_mgr::is_add_lucky_same_skill(citem *item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return false;
		}
		int32 skill_id = item_ptr->get_data_info(e_item_info_jewel_carve_0);

		JewelCarveTemplate *cur_tempalte_ptr = get_jewel_carve_tmplate_by_id(skill_id);
		if (cur_tempalte_ptr == nullptr)
		{
			return false;
		}

		for (int32 i = e_item_info_jewel_carve_1; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 pos_skill_id = item_ptr->get_data_info((e_item_info)i);
			JewelCarveTemplate *pos_tempalte_ptr = get_jewel_carve_tmplate_by_id(pos_skill_id);
			if (pos_tempalte_ptr == nullptr)
			{
				continue;
			}
			if (pos_tempalte_ptr->Type == cur_tempalte_ptr->Type && pos_tempalte_ptr->SubType >= cur_tempalte_ptr->SubType)
			{
				return true;
			}
		}
		return false;
	}
	int32 jewel_carve_mgr::is_high_replace_low(citem *item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return -1;
		}
		int32 skill_id = item_ptr->get_data_info(e_item_info_jewel_carve_0);

		JewelCarveTemplate *cur_tempalte_ptr = get_jewel_carve_tmplate_by_id(skill_id);
		if (cur_tempalte_ptr == nullptr)
		{
			return -1;
		}

		for (int32 i = e_item_info_jewel_carve_1; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 pos_skill_id = item_ptr->get_data_info((e_item_info)i);
			JewelCarveTemplate *pos_tempalte_ptr = get_jewel_carve_tmplate_by_id(pos_skill_id);
			if (pos_tempalte_ptr == nullptr)
			{
				continue;
			}
			if (pos_tempalte_ptr->Type == cur_tempalte_ptr->Type && pos_tempalte_ptr->SubType < cur_tempalte_ptr->SubType)
			{
				return i;
			}
		}
		return -1;

	}

	bool jewel_carve_mgr::is_exist_skill(const std::vector<int32>& random_skill_set, int32 skill_id)
	{
		 int32 num = random_skill_set.size();
		 if (num == 0 || num % 2 != 0)
		 {
			 return false;
		 }
		 for (int32 i = 0; i < num / 2; i++)
		 {
			 if (random_skill_set[i * 2] == skill_id)
			 {
				 return true;
			 }
		 }
		 return false;
	}



	

	int32 jewel_carve_mgr::get_skill_num(citem *item_ptr)
	{
		int32 count = 0;
		for (int32 i = e_item_info_jewel_carve_1; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 pos_skill_id = item_ptr->get_data_info((e_item_info)i);
			if (pos_skill_id == 0)
			{
				continue;
			}
			count++;
		}
		return count;
	}




	bool jewel_carve_mgr::is_can_inherit(citem* strip_item_ptr, citem* inherited_item_ptr)
	{
		if (strip_item_ptr == nullptr || inherited_item_ptr == nullptr)
		{
			return false;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		guid_64 item_guid = inherited_item_ptr->get_item_guid();
		if (get_jewel_open_on_num(player_ref, item_guid) < get_config_param(e_jewel_carve_param_need_jewel_num))
		{
			return false;
		}
		int32 strip_skill_num = inherit_skill_num(strip_item_ptr);
		int32 inherited_skill_num = inherit_skill_num(inherited_item_ptr);
		if (strip_skill_num > 0 && inherited_skill_num == 0)
		{
			return true;
		}
		return false;
	}

	int32 jewel_carve_mgr::inherit_skill_num(citem* item_ptr)
	{
		int32 skill_num = 0;
		for (int32 i = e_item_info_jewel_carve_0; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 pos_skill_id = item_ptr->get_data_info((e_item_info)i);
			if (pos_skill_id == 0)
			{
				continue;
			}
			skill_num++;
		}
		return skill_num;
	}

	void jewel_carve_mgr::set_inherit(citem* strip_item_ptr, citem* inherited_item_ptr)
	{
		if (!is_can_inherit(strip_item_ptr,inherited_item_ptr))
		{
			return;
		}
		for (int32 i = e_item_info_jewel_carve_0; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 strip_skill_id = strip_item_ptr->get_data_info((e_item_info)i);
			int32 inherited_skill_id = inherited_item_ptr->get_data_info((e_item_info)i);
			strip_item_ptr->set_data_info((e_item_info)i, inherited_skill_id);
			inherited_item_ptr->set_data_info((e_item_info)i, strip_skill_id);
		}
	}

	void jewel_carve_mgr::set_skill(citem *item_ptr, bool is_add)
	{
		if (item_ptr == nullptr)
		{
			return;
		}
		JewelCarveTemplate * jewel_carve_tmplate_ptr = get_jewel_carve_tmplate_by_guid_id(item_ptr->get_item_guid());
		if (jewel_carve_tmplate_ptr == nullptr)
		{
			return;
		}
		for (int32 i = e_item_info_jewel_carve_1; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 skill_id = item_ptr->get_data_info((e_item_info)i);
			if (skill_id == 0)
			{
				continue;
			}
			if (jewel_carve_tmplate_ptr->RealValue.size() > 0)
			{
				for (int32 j = 0; j < jewel_carve_tmplate_ptr->RealValue.size() / 2; ++j)
				{
					if (jewel_carve_tmplate_ptr->RealValue[j * 2] == skill_id)
					{
						skill_id = jewel_carve_tmplate_ptr->RealValue[j * 2 + 1];
						break;
					}
				}
			}
			JewelCarveTemplate *pos_tempalte_ptr = get_jewel_carve_tmplate_by_id(skill_id);
			if (pos_tempalte_ptr == nullptr)
			{
				continue;
			}
			switch (pos_tempalte_ptr->RealType)
			{
			case e_jewel_carve_real_type_add_attr:
				set_skill_attribute(item_ptr, pos_tempalte_ptr->RealValue, is_add);
				break;
			case e_jewel_carve_real_type_jewel_num:
				set_skill_jewel_num(item_ptr, pos_tempalte_ptr->RealValue, is_add);
				break;
			case e_jewel_carve_real_type_level_sum:
				set_skill_level_sum(item_ptr, pos_tempalte_ptr->RealValue, is_add);
				break;
			default:
				break;
			}
		}
	}

	int32 jewel_carve_mgr::get_jewel_num(citem * item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return 0;
		}
		int32 count = 0;
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; i++)
		{
			int32 jewel_id = item_ptr->get_data_info((e_item_info)i);
			if (jewel_id == 0)
			{
				continue;
			}
			count++;
		}
		return count;
	}

	int32 jewel_carve_mgr::get_jewel_level_sum(citem * item_ptr)
	{
		if (item_ptr == nullptr)
		{
			return 0;
		}
		int32 level_num = 0;
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; i++)
		{
			int32 jewel_id = item_ptr->get_data_info((e_item_info)i);
			if (jewel_id == 0)
			{
				continue;
			}
			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, jewel_id);
			if (item_template_ptr == nullptr)
			{
				continue;
			}
			JewelTemplate* jewel_template_ptr = GET_TEMPLATE(JewelTemplate, item_template_ptr->logic_id);
			if (jewel_template_ptr == nullptr)
			{
				continue;
			}
			level_num += jewel_template_ptr->JewelLevel;

		}
		return level_num;
	}

	void jewel_carve_mgr::set_skill_attribute(citem *citem_ptr, std::vector<int32>& RealValue, bool is_add)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}
		if (citem_ptr == nullptr || RealValue.size() != 2 )
		{
			return;
		}
		bool  is_self = RealValue[0] == 0 ? true : false;
		f32   add_percent = RealValue[1] / 100.0f;
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; i++)
		{
			int32 jewel_id = citem_ptr->get_data_info((e_item_info)i);
			if (jewel_id <= 0)
			{
				continue;
			}
			ItemTemplate* jewel_item_ptr = GET_TEMPLATE(ItemTemplate, jewel_id);
			if (jewel_item_ptr == nullptr)
			{
				continue;
			}
			JewelTemplate* jewel_ptr = GET_TEMPLATE(JewelTemplate, jewel_item_ptr->logic_id);
			if (jewel_ptr == nullptr)
			{
				continue;
			}
			if (is_self)
			{
				player_ref.get_item_set().item_change_att(jewel_ptr->JewelAtt, add_percent, is_add);

			}
			else 
			{
				int32 opposite_id = 0;
				if (jewel_ptr->JewelType == 1)
				{
					opposite_id = jewel_item_ptr->logic_id + 100;
				}
				else
				{
					opposite_id = jewel_item_ptr->logic_id - 100;
				}
				JewelTemplate* opposite_jewel_ptr = GET_TEMPLATE(JewelTemplate, opposite_id);
				if (opposite_jewel_ptr == nullptr)
				{
					continue;
				}
				player_ref.get_item_set().item_change_att(opposite_jewel_ptr->JewelAtt, add_percent, is_add);
			}
		}
	}
	void jewel_carve_mgr::set_skill_jewel_num(citem *citem_ptr, std::vector<int32>& RealValue, bool is_add)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (citem_ptr == nullptr || RealValue.size() == 0 || RealValue.size() % 2 != 0)
		{
			return;
		}
		int32 jewel_num = get_jewel_num(citem_ptr);
		int32 passive_skll_id = 0;
		for (int32 i = 0 ; i < RealValue.size() / 2; i++)
		{
			if (RealValue[2 * i] == jewel_num)
			{
				passive_skll_id = RealValue[2 * i + 1];
				break;
			}
		}
		player_ref.get_passive_skill().passive_skill_special(passive_skll_id, is_add);
	}

	void jewel_carve_mgr::set_skill_level_sum(citem *citem_ptr, std::vector<int32>& RealValue, bool is_add)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (citem_ptr == nullptr || RealValue.size() == 0 || RealValue.size() % 3 != 0)
		{
			return;
		}
		int32 level_sum = get_jewel_level_sum(citem_ptr);
		if (level_sum == 0)
		{
			return;
		}

		for (int32 i = 0; i < RealValue.size() / 3; i++)
		{

			f32   attr_id = RealValue[3 * i];
			int32 type = RealValue[3 * i + 2];
			f32 attr_value = 0.0f;
			f32 attr_percent = 0.0f;
			if (type == 0)
			{
				attr_value = RealValue[3 * i + 1] * level_sum;
			}
			else
			{
				attr_value = RealValue[3 * i + 1] * level_sum / 1000.0f;
			}
			std::vector<float> AttArray;
			AttArray.push_back(4);
			AttArray.push_back(attr_id);
			AttArray.push_back(attr_value);
			AttArray.push_back(attr_percent);
			AttArray.push_back(1);

			player_ref.get_item_set().item_change_att(AttArray, 1, is_add);
		}

	}
	int64 jewel_carve_mgr::get_jewel_fight(citem *item_ptr, const double* att_array)
	{
		if (item_ptr == nullptr || att_array == nullptr)
		{
			return 0;
		}
		int64 total_gs = 0;
		for (int32 i = e_item_info_jewel_carve_1; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 skill_id = item_ptr->get_data_info((e_item_info)i);
			if (skill_id == 0)
			{
				continue;
			}
			JewelCarveTemplate *pos_tempalte_ptr = get_jewel_carve_tmplate_by_id(skill_id);
			if (pos_tempalte_ptr == nullptr)
			{
				continue;
			}
			switch (pos_tempalte_ptr->RealType)
			{
			case e_jewel_carve_real_type_add_attr:
				total_gs += get_skill_attribute_fight(item_ptr, pos_tempalte_ptr->RealValue, att_array);
				break;
			case e_jewel_carve_real_type_jewel_num:
				total_gs += get_skill_jewel_num_fight(item_ptr, pos_tempalte_ptr->RealValue, att_array);
				break;
			case e_jewel_carve_real_type_level_sum:
				total_gs += get_skill_level_num_fight(item_ptr, pos_tempalte_ptr->RealValue, att_array);
				break;
			default:
				break;
			}
		}
		return total_gs;
	}

	int64 jewel_carve_mgr::get_skill_attribute_fight(citem *citem_ptr, std::vector<int32>& RealValue, const double* att_array)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return 0;
		}
		if (citem_ptr == nullptr || RealValue.size() != 2 || att_array == nullptr)
		{
			return 0;
		}
	    bool  is_self = RealValue[0] == 0 ? true : false;
		f32   add_percent = RealValue[1] / 100.0f;
		int64 total_gs = 0;
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; i++)
		{
			int32 jewel_id = citem_ptr->get_data_info((e_item_info)i);
			if (jewel_id <= 0)
			{
				continue;
			}
			ItemTemplate* jewel_item_ptr = GET_TEMPLATE(ItemTemplate, jewel_id);
			if (jewel_item_ptr == nullptr)
			{
				continue;
			}
			JewelTemplate* jewel_ptr = GET_TEMPLATE(JewelTemplate, jewel_item_ptr->logic_id);
			if (jewel_ptr == nullptr)
			{
				continue;
			}
			if (is_self)
			{
				
				total_gs += init_unit::calcu_fighting_power_by_att_array(jewel_ptr->JewelAtt, att_array, add_percent);

			}
			else
			{
				int32 opposite_id = 0;
				if (jewel_ptr->JewelType == 1)
				{
					opposite_id = jewel_item_ptr->logic_id + 100;
				}
				else
				{
					opposite_id = jewel_item_ptr->logic_id - 100;
				}
				JewelTemplate* opposite_jewel_ptr = GET_TEMPLATE(JewelTemplate, opposite_id);
				if (opposite_jewel_ptr == nullptr)
				{
					continue;
				}
				total_gs += init_unit::calcu_fighting_power_by_att_array(opposite_jewel_ptr->JewelAtt, att_array, add_percent);
			}
		}
		return total_gs;

	}

	int64 jewel_carve_mgr::get_skill_jewel_num_fight(citem *citem_ptr, std::vector<int32>& RealValue, const double* att_array)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return 0;
		}
		if (citem_ptr == nullptr || RealValue.size() == 0 || RealValue.size() % 2 != 0 || att_array == nullptr)
		{
			return 0;
		}
		int32 jewel_num = get_jewel_num(citem_ptr);
		int32 passive_skll_id = 0;
		for (int32 i = 0; i < RealValue.size() / 2; i++)
		{
			if (RealValue[2 * i] == jewel_num)
			{
				passive_skll_id = RealValue[2 * i + 1];
				break;
			}
		}
		int64 total_gs = 0;

		PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skll_id);
		if (passive_skill_template_ptr)
		{
			total_gs += passive_skill_template_ptr->GSAddValue;
		}
		return total_gs;

	}

	int64 jewel_carve_mgr::get_skill_level_num_fight(citem *citem_ptr, std::vector<int32>& RealValue, const double* att_array)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return 0;
		}
		if (citem_ptr == nullptr || RealValue.size() == 0 || RealValue.size() % 3 != 0 || att_array == nullptr)
		{
			return 0;
		}
		int32 level_sum = get_jewel_level_sum(citem_ptr);
		if (level_sum == 0)
		{
			return 0;
		}
		int64 total_gs = 0;
		for (int32 i = 0; i < RealValue.size() / 3; i++)
		{

			f32   attr_id = RealValue[3 * i];
			int32 type = RealValue[3 * i + 2];
			f32 attr_value = 0.0f;
			f32 attr_percent = 0.0f;
			if (type == 0)
			{
				attr_value = RealValue[3 * i + 1] * level_sum;
			}
			else
			{
				attr_value = RealValue[3 * i + 1] * level_sum / 1000.0f;
			}
			std::vector<float> AttArray;
			AttArray.push_back(4);
			AttArray.push_back(attr_id);
			AttArray.push_back(attr_value);
			AttArray.push_back(attr_percent);
			AttArray.push_back(1);

			total_gs += init_unit::calcu_fighting_power_by_att_array(AttArray, att_array);

		}
		return total_gs;

	}

	bool jewel_carve_mgr::my_greater(int32 skill_id_a, int32 skill_id_b)
	{
		JewelCarveTemplate * jewel_info_ptr_a = get_jewel_carve_tmplate_by_id(skill_id_a);
		if (jewel_info_ptr_a == nullptr)
		{
			return false;
		}
		JewelCarveTemplate * jewel_info_ptr_b = get_jewel_carve_tmplate_by_id(skill_id_b);
		if (jewel_info_ptr_b == nullptr)
		{
			return false;
		}
		return jewel_info_ptr_a->SubType > jewel_info_ptr_b->SubType; 
	}

	void jewel_carve_mgr::get_random_skill(std::vector<int32>& vec_skill_set_id, int32& skill_id1, int32& skill_id2, int32 open_num)
	{
		if (vec_skill_set_id.size() == 0)
		{
			return;
		}

		int32 num = vec_skill_set_id.size();

		for (int32 i = 0; i < num; i++)
		{
			int32 pos = random_gen::get_random(0, num - 1);
			int32 tmp = 0;
			if (pos != i)
			{
			   tmp	= vec_skill_set_id[i];
			   vec_skill_set_id[i] = vec_skill_set_id[pos];
			   vec_skill_set_id[pos] = tmp;
			}
		}

		std::sort(vec_skill_set_id.begin(), vec_skill_set_id.end(), jewel_carve_mgr::my_greater);

		std::vector<int32> result_id;

		for (int32 i = 0; i < vec_skill_set_id.size(); i++)
		{
			int32 is_find = false;
			for (int32 j = 0; j < result_id.size(); j++)
			{
				if (vec_skill_set_id[i] == result_id[j])
				{
					is_find = true;
					break;
				}
			}
			if (is_find == false)
			{
				result_id.push_back(vec_skill_set_id[i]);
			}
		}
		for (int32 i = 0; i < result_id.size() && i < open_num; i++)
		{
			if (i == 0)
			{
				skill_id1 = result_id[i];
			}
			else
			{
				skill_id2 = result_id[i];
			}
		}
	}
	
	

}