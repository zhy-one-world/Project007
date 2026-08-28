
/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 19:40
  file base: item
  file ext: cpp
  author: zhy
  
  purpose: 
*********************************************************************/

#include "item.h"
#include "template/template_manager.h"
#include "logic/skill_manager.h"
#include "logic/buff_man.h"
#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "internet/item.pb.h"
#include "utility/init_unit.h"

namespace hld
{
	citem::citem(void)
	{
		clear_data();
	}

	citem::~citem(void)
	{
	}

	void citem::heart_tick(const int64& new_time)
	{
	}

	void citem::clear_data()
	{
		m_item_data.clear_data();
		m_item_info_ptr = nullptr;
		memset(m_item_temp_succinct_block, 0, sizeof(m_item_temp_succinct_block));

		m_item_temp_forge_flag = false;
		m_is_add_att = true;
	}

	const int32 citem::get_item_template_id() const
	{
		if (nullptr == m_item_info_ptr)
		{
			return 0;
		}
		return m_item_info_ptr->attribute_id;
	}

	MountTemplate* citem::get_mount_template_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(MountTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}
	FasionTemplate* citem::get_fashion_template_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(FasionTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}

	BeastSpiritTemplate* citem::get_beast_spirit_template_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}

		return GET_TEMPLATE(BeastSpiritTemplate, m_item_info_ptr->logic_id);

	}

	EquipTemplate* citem::get_equip_info_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(EquipTemplate, m_item_info_ptr->logic_id);
	}
	ItemUpgradeTemplate* citem::get_upgrade_info_ptr()
	{
		EquipTemplate*  equip_template_ptr = get_equip_info_ptr();
		if (nullptr == equip_template_ptr)
		{
			return nullptr;
		}
		//return GET_TEMPLATE(ItemUpgradeTemplate, equip_template_ptr->UpgradeTemplateId + m_item_data.data_ary[e_item_info_upgrade_count]);
		return nullptr;
	}
	ItemAddonTemplate* citem::get_add_on_info_ptr()
	{
		EquipTemplate*  equip_template_ptr = get_equip_info_ptr();
		if (nullptr == equip_template_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(ItemAddonTemplate, equip_template_ptr->AddOnTemplateId + m_item_data.data_ary[e_item_info_add_on]);
	}
	ItemSuccinctTemplate* citem::get_succinc_info_ptr()
	{
		EquipTemplate*  equip_template_ptr = get_equip_info_ptr();
		if (nullptr == equip_template_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(ItemSuccinctTemplate, equip_template_ptr->SuccinctTemplateID);
	}
	WingTemplate* citem::get_wing_template_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(WingTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}
	ProtectSpiritTemplate* citem::get_protect_spirit_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(ProtectSpiritTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}
	SpiritTemplate* citem::get_sprite_info_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(SpiritTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}
	DeityTemplate * citem::get_deity_info_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(DeityTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}
	WeddingRingTemplate* citem::get_wedding_ring_info_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(WeddingRingTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_upgrade_count]);
	}
	WeddingRingTemplate* citem::get_wedding_ring_info_ptr_by_mate_ring_level()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(WeddingRingTemplate, m_item_info_ptr->logic_id + m_item_data.data_ary[e_item_info_random_property1]);
	}
	AdditionBuffTemplate* citem::get_heart_value_template_ptr()
	{
		int32 heart_value = get_data_info(e_item_info_random_property2);
		int32 template_id = heart_value_template_begin_id;
		while (true)
		{
			AdditionBuffTemplate* cur_addition_tmpl_ptr = GET_TEMPLATE(AdditionBuffTemplate, template_id);
			if (cur_addition_tmpl_ptr && cur_addition_tmpl_ptr->AdditionBuffType == e_addition_buff_couple_heart_value + 1)
			{
				if (cur_addition_tmpl_ptr->EquipNum <= heart_value && heart_value <= cur_addition_tmpl_ptr->UpgradeNum)
					return cur_addition_tmpl_ptr;
				else
					template_id += 1;
			}
			else
				break;
		}
		return nullptr;
	}
	MountEquipTemplate *citem::get_mount_equip_template_ptr()
	{
		if (nullptr == m_item_info_ptr)
		{
			return nullptr;
		}
		return GET_TEMPLATE(MountEquipTemplate, m_item_info_ptr->logic_id);

	}

	int32 citem::have_not_save_forge_att_num()
	{
		int32 not_save_num = 0;
		for (int32 i = e_item_info_forge_property1; i <= e_item_info_forge_property6; i++)
		{
			if (get_data_info((e_item_info)i) > 0)
			{
				not_save_num++;
			}
		}
		return not_save_num;
	}

	int32 citem::have_excellent_att_num()
	{
		int32 have_att_num = 0;
		ItemTemplate* item_temp_ptr = get_item_info_ptr();
		if (nullptr == item_temp_ptr)
		{
			return have_att_num;
		}
		int32 total_att_num = 0;
		if (item_temp_ptr->item_type == e_item_type_spirit)
		{
			SpiritTemplate* spirit_template_ptr = get_sprite_info_ptr();
			if (NULL == spirit_template_ptr)
			{
				return have_att_num;
			}
			total_att_num = spirit_template_ptr->BaseExcellentAtt.size() / hld::e_att_one_max;
			if (total_att_num != spirit_template_ptr->GreenExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->BlueExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->PurpleExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->OrangeExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->RedExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->PinkExcellentRatio.size() / 2
				|| total_att_num <= 0)
			{
				return have_att_num;
			}
		}
		else
		{
			//EquipTemplate* equip_template_ptr = get_equip_info_ptr();
			//if (NULL == equip_template_ptr)
			//{
			//	return have_att_num;
			//}
			//total_att_num = equip_template_ptr->BaseExcellentAtt.size() / hld::e_att_one_max;
			//if (total_att_num != equip_template_ptr->GreenExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->BlueExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->PurpleExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->OrangeExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->RedExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->PinkExcellentRatio.size() / 2
			//	|| total_att_num <= 0)
			//{
			//	return have_att_num;
			//}
		}
		
		int32 temp_forge_att_flag = get_data_info(e_item_info_random_had_flag);
		for (int32 i = 0; i < total_att_num; ++i)
		{
			if ((temp_forge_att_flag >> i) & 1)
				have_att_num++;
		}
		return have_att_num;
	}


	void citem::fill_item_info_msg(item_proto_item_info& item_info_msg)
	{
		item_info_msg.set_item_guid(get_item_guid().server_64);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			item_info_msg.add_item_data(get_data_info((e_item_info)i));
		}
	}

	void citem::set_item_data(const s_item_info& item_data)
	{
		memcpy(&m_item_data, &item_data, sizeof(item_data));
		m_item_info_ptr = GET_TEMPLATE(ItemTemplate, item_data.data_ary[e_item_info_info_id]);
	}
	int32 citem::get_item_logic_id()
	{
		if (nullptr == m_item_info_ptr)
		{
			return 0;
		}
		return m_item_info_ptr->logic_id;
	}

	bool citem::is_item_belong_to_class(const e_class_type class_type)
	{
		ItemTemplate* item_template_ptr = get_item_info_ptr();
		if (nullptr == item_template_ptr)
		{
			return false;
		}
		const int32 class_type_array_size = item_template_ptr->class_type.size();
		if (0 == class_type_array_size)
		{
			return true;
		}
		for (int32 class_type_array_index = 0; class_type_array_index < class_type_array_size; class_type_array_index++)
		{
			if ((e_class_type)item_template_ptr->class_type[class_type_array_index] == class_type)
			{
				return true;
			}
		}
		return false;
	}

	void citem::set_unit_buff(const int32& unit_array_index, bool is_add)
	{
		if (NULL == m_item_info_ptr)
		{
			return;
		}
		if (is_add)
		{
			for (int32 i = 0; i < m_item_info_ptr->buff_trigger_id.size(); ++i)
			{
				if (m_item_info_ptr->buff_trigger_id[i] > 0)
				{
					buff_man::add_buff_inst(unit_array_index, unit_array_index, m_item_info_ptr->buff_trigger_id[i]);
				}
			}
		}
		else
		{
			for (int32 i = 0; i < m_item_info_ptr->buff_trigger_id.size(); ++i)
			{
				if (m_item_info_ptr->buff_trigger_id[i] > 0)
				{
					buff_man::del_buff_by_template_id(unit_array_index, unit_array_index, m_item_info_ptr->buff_trigger_id[i]);
				}
			}
		}
	}
	void citem::set_unit_skill(const int32& unit_array_index, bool is_add)
	{
		if (NULL == m_item_info_ptr)
		{
			return;
		}
		if (is_add)
		{
			for (int32 i = 0; i < m_item_info_ptr->skill_array.size(); ++i)
			{
				skill_manager::resp_master_skill_by_id(unit_array_index, m_item_info_ptr->skill_array[i]);
			}
		}
		else
		{
			for (int32 i = 0; i < m_item_info_ptr->skill_array.size(); ++i)
			{
				skill_manager::remove_unit_skill_by_id(unit_array_index, m_item_info_ptr->skill_array[i]);
			}
		}
	}
	void citem::set_unit_att(const int32& unit_array_index, bool is_add)
	{
		player& player_ref = unit_man::get_player(unit_array_index);

		for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property3; ++i)
		{
			if (m_item_data.data_ary[i] > e_unit_attack_att_none && m_item_data.data_ary[i] < e_unit_attack_att_max)
			{
				player_ref.get_pawn_att().apply_att_change_effect(4, m_item_data.data_ary[i], m_item_data.data_ary[i + 3], 0, true, is_add);
			}
		}
	}

	void citem::set_unit_equip_passive_skill(const int32 & unit_array_index, bool is_add)
	{
		player& player_ref = unit_man::get_player(unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		if (NULL == m_item_info_ptr)
		{
			return;
		}
		if (nullptr == get_equip_info_ptr())
		{
			return;
		}
		std::vector<int32> star_buff_list = get_equip_info_ptr()->StarBuffId;
		for (int32 i = 0; i < star_buff_list.size(); ++i)
		{
			player_ref.get_passive_skill().passive_skill_special(star_buff_list[i], is_add);
		}
	}

	bool citem::is_equip_type()
	{
		if (nullptr == m_item_info_ptr)
		{
			return false;
		}
		return init_unit::is_equip_type(m_item_info_ptr->item_type);
	}

	// 如果不是水果就返回e_fruit_type_none,否则返回水果的类型
	e_fruit_type citem::is_fruit()
	{
		if (nullptr == m_item_info_ptr)
		{
			return e_fruit_type_none;
		}
		if (e_item_type_expendable == m_item_info_ptr->item_type && e_prop_sub_type_fruit == m_item_info_ptr->sub_type)
		{
			PropBasicAttributeTemplate* prop_template_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, m_item_info_ptr->logic_id);
			if (nullptr == prop_template_ptr)
			{
				return e_fruit_type_none;
			}
			if (prop_template_ptr->RoleAttributeChange.size() < e_att_one_max)
			{
				return e_fruit_type_none;
			}
			switch ((int32)prop_template_ptr->RoleAttributeChange[e_att_one_att_id])
			{
				case 1: return e_fruit_type_strength;
				case 2: return e_fruit_type_agility;
				case 3: return e_fruit_type_intellect;
				case 4: return e_fruit_type_stamina;
			}
		}

		return e_fruit_type_none;
	}
	bool citem::can_merge()
	{
		if (nullptr == m_item_info_ptr)
		{
			return false;
		}
		//限时物品不可以堆叠
		if (get_data_info(e_item_info_over_time) > 0)
		{
			return false;
		}
		// 堆叠数目已满的道具不需要合并
		if (get_data_info(e_item_info_stack_count) >= m_item_info_ptr->max_pile_num)
		{
			return false;
		}
		return true;
	}
	// 获取装备的卓越属性的个数,返回0-6
	int32 citem::get_excellent_att_num()
	{
		int32 excellent_att_num = 0;
		for (int32 i = e_item_info_random_property1; i <= e_item_info_random_property6; ++i)
		{
			if (get_data_info((e_item_info)i) > 0)
			{
				++excellent_att_num;
			}
		}

		return excellent_att_num;
	}

	void citem::set_temp_succinct_att(item_temp_succinct_property index, int32 value)
	{
		if (index<item_temp_succinct_property_max)
		{
			m_item_temp_succinct_block[index] = value;
		}
	}

	int32 citem::get_temp_succinct_att(item_temp_succinct_property index)
	{
		if (index<item_temp_succinct_property_max)
		{
			return m_item_temp_succinct_block[index];
		}
		return -999;
	}


	void citem::set_forge_property(int32 idex)
	{
		if (idex < 0 || idex >= item_temp_forge_property_max)
		{
			return;
		}
		int32	item_temp_forge_lock_block[item_temp_forge_property_max];
		int32	item_temp_forge_lock_name_block[item_temp_forge_property_max];
		int32	item_temp_forge_lock_name_flag_block[item_temp_forge_property_max];

		memset(item_temp_forge_lock_block, 0, sizeof(item_temp_forge_lock_block));
		memset(item_temp_forge_lock_name_block, 0, sizeof(item_temp_forge_lock_name_block));
		memset(item_temp_forge_lock_name_flag_block, 0, sizeof(item_temp_forge_lock_name_flag_block));
		int32 total_att_num = 0;
		std::vector<int32> att_index_array;

		ItemTemplate* item_temp_ptr = get_item_info_ptr();
		if (nullptr == item_temp_ptr)
		{
			return;
		}
		if (item_temp_ptr->item_type == e_item_type_spirit)
		{
			SpiritTemplate* spirit_template_ptr = get_sprite_info_ptr();
			if (nullptr == spirit_template_ptr
				|| spirit_template_ptr->BaseExcellentAtt.size() % hld::e_att_one_max != 0
				|| spirit_template_ptr->GreenExcellentRatio.size() % 2 != 0
				|| spirit_template_ptr->BlueExcellentRatio.size() % 2 != 0
				|| spirit_template_ptr->PurpleExcellentRatio.size() % 2 != 0
				|| spirit_template_ptr->OrangeExcellentRatio.size() % 2 != 0
				|| spirit_template_ptr->RedExcellentRatio.size() % 2 != 0
				|| spirit_template_ptr->PinkExcellentRatio.size() % 2 != 0)
			{
				return;
			}
			total_att_num = spirit_template_ptr->BaseExcellentAtt.size() / hld::e_att_one_max;
			if (total_att_num != spirit_template_ptr->GreenExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->BlueExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->PurpleExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->OrangeExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->RedExcellentRatio.size() / 2
				|| total_att_num != spirit_template_ptr->PinkExcellentRatio.size() / 2
				|| total_att_num <= 0)
			{
				return;
			}
			att_index_array.reserve(total_att_num);
			for (int32 i = 0; i < total_att_num; ++i)
			{
				att_index_array.push_back(spirit_template_ptr->BaseExcellentAtt[i * 5 + 1]);
			}
		}
		else
		{
			//EquipTemplate* equip_template_ptr = get_equip_info_ptr();
			//if (nullptr == equip_template_ptr
			//	|| equip_template_ptr->BaseExcellentAtt.size() % hld::e_att_one_max != 0
			//	|| equip_template_ptr->GreenExcellentRatio.size() % 2 != 0
			//	|| equip_template_ptr->BlueExcellentRatio.size() % 2 != 0
			//	|| equip_template_ptr->PurpleExcellentRatio.size() % 2 != 0
			//	|| equip_template_ptr->OrangeExcellentRatio.size() % 2 != 0
			//	|| equip_template_ptr->RedExcellentRatio.size() % 2 != 0
			//	|| equip_template_ptr->PinkExcellentRatio.size() % 2 != 0)
			//{
			//	return;
			//}
			//total_att_num = equip_template_ptr->BaseExcellentAtt.size() / hld::e_att_one_max;
			//if (total_att_num != equip_template_ptr->GreenExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->BlueExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->PurpleExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->OrangeExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->RedExcellentRatio.size() / 2
			//	|| total_att_num != equip_template_ptr->PinkExcellentRatio.size() / 2
			//	|| total_att_num <= 0)
			//{
			//	return;
			//}
			//att_index_array.reserve(total_att_num);
			//for (int32 i = 0; i < total_att_num; ++i)
			//{
			//	att_index_array.push_back(equip_template_ptr->BaseExcellentAtt[i * 5 + 1]);
			//}
		}
		int32 final_att_num = have_excellent_att_num();
		int32 temp_forge_att_flag = get_data_info(e_item_info_random_had_flag);
		for (int32 i = 0, j = 0; i < total_att_num && j < final_att_num; ++i)
		{
			int32 temp_flag = (temp_forge_att_flag >> i) & 1;
			if (temp_flag == 1)
			{
				item_temp_forge_lock_block[j] = get_data_info((e_item_info)(e_item_info_random_property1 + j));
				item_temp_forge_lock_name_block[j] = att_index_array[i];
				item_temp_forge_lock_name_flag_block[j] = i;
				j++;
			}
		}

		int32 forge_property = 0;
		forge_property |= item_temp_forge_lock_block[idex];
		forge_property |= (item_temp_forge_lock_name_block[idex] << 16);
		forge_property |= (item_temp_forge_lock_name_flag_block[idex] << 24);
		set_data_info(e_item_info(idex + e_item_info_forge_property1), forge_property);
	}
	void citem::get_forge_property(int32 i, int32 *forge_att, int32 *forge_name_att, int32 *forge_name_flag_att)
	{
		*forge_name_flag_att = get_forge_name_flag_att(i);
		*forge_name_att = get_forge_name_att(i);
		*forge_att = get_forge_att(i);
	}

	int32 citem::get_forge_att(int32 item_data_index)
	{
		if (item_data_index < e_item_info_forge_property1 || item_data_index > e_item_info_forge_property6)
		{
			return 0;
		}
		int32 forge_property = get_data_info(e_item_info(item_data_index));
		int32 forge_att = forge_property & 0XFFFF;
		return forge_att;
	}
	int32 citem::get_forge_name_att(int32 item_data_index)
	{
		if (item_data_index < e_item_info_forge_property1 || item_data_index > e_item_info_forge_property6)
		{
			return 0;
		}
		int32 forge_property = get_data_info(e_item_info(item_data_index));
		forge_property &= 0XFF0000;
		int32 forge_name_att = (forge_property >> 16);
		return forge_name_att;
	}
	int32 citem::get_forge_name_flag_att(int32 item_data_index)
	{
		if (item_data_index < e_item_info_forge_property1 || item_data_index > e_item_info_forge_property6)
		{
			return 0;
		}
		int32 forge_property = get_data_info(e_item_info(item_data_index));
		int32 forge_name_flag_att = forge_property >> 24;
		return forge_name_flag_att;
	}
	int32 citem::get_forge_change_gs(int32 player_index)
	{
		player& player_ref = unit_man::get_player(player_index);
		if (false == player_ref.is_valid())
		{
			return 0;
		}

		std::vector<float> add_att;
		std::vector<float> del_att;

		SpiritTemplate* spirit_template_ptr = get_sprite_info_ptr();
		if (nullptr == spirit_template_ptr)
		{
			return 0;
		}

		std::vector<float> base_att_array = spirit_template_ptr->BaseExcellentAtt;
		int32 data_num = base_att_array.size();

		for (int32 i = e_item_info_forge_property1; i <= e_item_info_forge_property6; i++)
		{
			int32 forge_property = get_data_info(e_item_info(i));
			int32 forge_att = forge_property & 0XFFFF;		//属性倍率
			forge_property &= 0XFF0000;	
			int32 forge_name_att = (forge_property >> 16);	//属性索引
		
			if (forge_name_att <= 0)
			{
				continue;
			}

			for (int32 j = 0; j < (data_num / e_att_one_max); j++)
			{
				//判断是否存在id
				if (forge_name_att == base_att_array[j * e_att_one_max + e_att_one_att_id])
				{
					add_att.push_back(base_att_array[j * e_att_one_max + e_att_one_level]);
					add_att.push_back(base_att_array[j * e_att_one_max + e_att_one_att_id]);
					add_att.push_back(base_att_array[j * e_att_one_max + e_att_one_value] * forge_att);
					add_att.push_back(base_att_array[j * e_att_one_max + e_att_one_percent] * forge_att);
					add_att.push_back(base_att_array[j * e_att_one_max + e_att_one_show]);
					break;
				}
			}
		}

		//没有增加属性说明不是重铸
		if (add_att.size() <= 0)
		{
			return 0;
		}

		int32 random_had_flag = get_data_info(e_item_info_random_had_flag);
		int32 value_info_index = e_item_info_random_property1;
		
		for (int32 i = 0; i < (data_num / e_att_one_max); i++)
		{
			if (value_info_index > e_item_info_random_property6)
			{
				break;
			}
			if ((random_had_flag & 1 << i) > 0)
			{
				int32 forge_property = get_data_info(e_item_info(value_info_index));
				if (forge_property <= 0)
				{
					continue;
				}
				del_att.push_back(base_att_array[i * e_att_one_max + e_att_one_level]);
				del_att.push_back(base_att_array[i * e_att_one_max + e_att_one_att_id]);
				del_att.push_back(base_att_array[i * e_att_one_max + e_att_one_value] * forge_property);
				del_att.push_back(base_att_array[i * e_att_one_max + e_att_one_percent] * forge_property);
				del_att.push_back(base_att_array[i * e_att_one_max + e_att_one_show]);
				value_info_index++;
			}
		}

		return player_ref.get_pawn_att().get_change_gs_value(add_att, del_att);
	}
	bool citem::can_used_by_class_type(e_class_type class_type)
	{
		if (nullptr == m_item_info_ptr)
		{
			return false;
		}

		int32 temp_len = m_item_info_ptr->class_type.size();
		if (temp_len <= 0)
		{
			return true;
		}

		for (int32 i = 0; i < temp_len; ++i)
		{
			if (m_item_info_ptr->class_type[i] == class_type || m_item_info_ptr->class_type[i] == 0)
			{
				return true;
			}
		}

		return false;
	}
	bool citem::is_succinct_full()
	{
		std::vector<int32> vec_property_max_limit;
		get_property_max_limit(vec_property_max_limit);

		int32 item_info_check_start = e_item_info_succinct_property1;
		for (int32 i = 0; i < vec_property_max_limit.size(); ++i)
		{
			int32 real_data_index = item_info_check_start + i;
			if (real_data_index > e_item_info_succinct_property6)
			{
				break;
			}
			if (get_data_info((e_item_info)real_data_index) < vec_property_max_limit[i])
			{
				return false;
			}
		}
		return true;
	}

	int32 citem::get_item_succinct_level()
	{
		std::vector<int32> vec_property_limit;
		std::vector<int32> vec_property_max_limit;
		get_property_max_limit(vec_property_max_limit);
		
		ItemSuccinctTemplate* temp_succinct_template = get_succinc_info_ptr();
		if (nullptr == temp_succinct_template || temp_succinct_template->PropertyMaxLimit.size() <= 0 || temp_succinct_template->PropertyID.size() <= 0 || temp_succinct_template->PropertyMaxLimit.size() % temp_succinct_template->PropertyID.size() != 0)
		{
			return 0;
		}
		
		vec_property_limit = temp_succinct_template->PropertyMaxLimit;
		int32 property_size = temp_succinct_template->PropertyMaxLimit.size() / temp_succinct_template->PropertyID.size();
		int32 item_info_check_start = e_item_info_succinct_property1;
		int32 cur_min_level = property_size;
		int32 cur_max_level = property_size;//满级
		for (int32 i = 0; i < temp_succinct_template->PropertyID.size(); i++)//根据洗练属性获取所属级别
		{
			
			int32 real_data_index = item_info_check_start + i;
			if (real_data_index > e_item_info_succinct_property6)
			{
				break;
			}

			int32 item_succinct_property = get_data_info((e_item_info)real_data_index);
			if (item_succinct_property >= vec_property_max_limit[i])
			{
				if (vec_property_max_limit[i] <= 0)
				{
					cur_max_level = 0;
				}
				continue;
			}

			for (int32 j = 0; j < property_size; j++)
			{
				if (item_succinct_property < vec_property_limit[j * temp_succinct_template->PropertyID.size() + i])
				{
					if (j < cur_min_level)
					{
						cur_min_level = j;
					}
					break;
				}
			}
		}

		if (cur_max_level != 0)//表里的属性没问题
		{
			return cur_min_level;
		}
		return 0;
	}

	int32 citem::get_item_grade()
	{
		int32 return_value = 0;
		if (m_item_info_ptr == nullptr)
		{
			return return_value;
		}

		if (m_item_info_ptr->level_limit.size() < 2)
		{
			return 0;
		}
		return_value = init_unit::get_player_grade_num(m_item_info_ptr->level_limit[0]) + 1;

		return return_value;
	}
	
	int32 citem::get_item_color()
	{
		ItemTemplate* item_template_ptr = get_item_info_ptr();
		if (nullptr == item_template_ptr)
		{
			return 0;
		}
		return item_template_ptr->item_color;
	}
	void citem::get_property_max_limit(std::vector<int32>& vec_property_max_limit)
	{
		ItemSuccinctTemplate* temp_succinct_template = get_succinc_info_ptr();
		if (nullptr == temp_succinct_template || temp_succinct_template->PropertyMaxLimit.size() <= 0 || temp_succinct_template->PropertyID.size() <= 0 || temp_succinct_template->PropertyMaxLimit.size() % temp_succinct_template->PropertyID.size() != 0)
		{
			return;
		}
		int32 max_succinct_buff_level = temp_succinct_template->PropertyMaxLimit.size() / temp_succinct_template->PropertyID.size();
		int32 max_limit_begin_index = temp_succinct_template->PropertyID.size() * (max_succinct_buff_level - 1);
		for (int32 i = 0; i < temp_succinct_template->PropertyID.size(); i++)
		{
			vec_property_max_limit.push_back(temp_succinct_template->PropertyMaxLimit[max_limit_begin_index + i]);
		}
	}

	e_item_succinct_buff_level citem::get_item_succinct_current_buff_level()
	{
		ItemSuccinctTemplate* temp_succinct_template = get_succinc_info_ptr();
		if (nullptr == temp_succinct_template || temp_succinct_template->PropertyMaxLimit.size() <= 0 || temp_succinct_template->PropertyID.size() <= 0
			|| temp_succinct_template->PropertyMaxLimit.size() % temp_succinct_template->PropertyID.size() != 0)
		{
			return e_item_succinct_buff_level_none;
		}
		int32 max_succinct_buff_level = temp_succinct_template->PropertyMaxLimit.size() / temp_succinct_template->PropertyID.size();
		int32 item_info_check_start = e_item_info_succinct_property1;
		for (int32 i = 0; i < max_succinct_buff_level; i++)
		{
			for (int32 j = 0; j < temp_succinct_template->PropertyID.size(); j++)
			{
				int32 real_data_index = item_info_check_start + j;
				if (temp_succinct_template->PropertyMaxLimit[i * temp_succinct_template->PropertyID.size() + j] <= 0 || real_data_index > e_item_info_succinct_property6)
				{
					return e_item_succinct_buff_level_none;
				}
				if (get_data_info((e_item_info)real_data_index) < temp_succinct_template->PropertyMaxLimit[i * temp_succinct_template->PropertyID.size() + j])
				{
					return (e_item_succinct_buff_level)i;
				}
			}
		}
		return (e_item_succinct_buff_level)max_succinct_buff_level;
	}

	void citem::get_item_msg(item_proto_update_character_item& item_msg, bool to_self)
	{
		item_proto_item_info* item_info_ptr = item_msg.add_item_list();
		item_info_ptr->set_item_guid(get_item_guid().server_64);
		if (to_self)
		{
			for (int32 i = 0; i < e_item_info_max; ++i)
			{
				item_info_ptr->add_item_data(get_data_info((e_item_info)i));
			}
		}
		else
		{
			item_info_ptr->add_item_data(get_data_info(e_item_info_info_id));
			item_info_ptr->add_item_data(get_data_info(e_item_info_slot));
			item_info_ptr->add_item_data(get_data_info(e_item_info_container_type));
			item_info_ptr->add_item_data(get_data_info(e_item_info_stack_count));
			item_info_ptr->add_item_data(get_data_info(e_item_info_activate));//
			item_info_ptr->add_item_data(get_data_info(e_item_info_is_first));
			item_info_ptr->add_item_data(get_data_info(e_item_info_upgrade_count));
		}
	}

	int32 citem::get_real_id()
	{
		if (nullptr != m_item_info_ptr)
		{
			return m_item_info_ptr->logic_id + get_data_info(e_item_info_upgrade_count);
		}
		return 0;
	}
	int64 citem::calcu_single_item_fighting_power(int32 array_index)
	{
		int64 fighting_power = 0;
		player& self_ref = unit_man::get_player(array_index);
		if (false == self_ref.is_valid())
		{
			return 0;
		}

		if (nullptr == get_item_info_ptr())
		{
			return 0;
		}

		const double* att_array = self_ref.m_pawn_att.get_attack_att_all();

		switch (get_item_info_ptr()->item_type)
		{
		case e_item_type_weapon:
		case e_item_type_armor:
		{
			fighting_power = calcu_single_equip_fighting_power(att_array);
			fighting_power += self_ref.get_jewel_carve_mgr().get_jewel_fight(this, att_array);
		}
		break;
		case e_item_type_mount:
		{

			int32 wing_template_id = get_item_logic_id() + get_data_info(e_item_info_upgrade_count);
			MountTemplate* temp_ptr = GET_TEMPLATE(MountTemplate, wing_template_id);
			if (nullptr == temp_ptr)
			{
				return 0;
			}
			fighting_power = init_unit::calcu_fighting_power_by_att_array(temp_ptr->AttArray, att_array);
			if (temp_ptr->Type == mount_type_normal || temp_ptr->Type == mount_type_special)
			{
				int32 mount_upgrade_star_id = get_item_logic_id() + get_data_info(e_item_info_stargrade_count) + difference_from_upgrade_star_id_and_item_logic_id;
				MountTemplate* mount_upgrade_star_ptr = GET_TEMPLATE(MountTemplate, mount_upgrade_star_id);
				if (nullptr != mount_upgrade_star_ptr)
				{
					fighting_power += init_unit::calcu_fighting_power_by_att_array(mount_upgrade_star_ptr->AttArray, att_array);
				}
			}
			fighting_power += self_ref.get_mount_power_mgr().calc_mount_power_fight(this);
		}
		break;

		case e_item_type_wing:
		{
			int32 wing_template_id = get_item_logic_id() + get_data_info(e_item_info_upgrade_count);
			WingTemplate* temp_ptr = GET_TEMPLATE(WingTemplate, wing_template_id);
			if (nullptr == temp_ptr)
			{
				return 0;
			}
			fighting_power = init_unit::calcu_fighting_power_by_att_array(temp_ptr->AttArray, att_array);
			if (temp_ptr->Type == e_item_wing_type_shape_illusion)
			{
				int32 wing_upgrade_star_id = get_item_logic_id() + get_data_info(e_item_info_stargrade_count) + difference_from_upgrade_star_id_and_item_logic_id;
				int32 wing_psychic_id = get_item_logic_id() + get_data_info(e_item_info_forge_property1) + difference_from_wing_psychic_id_and_item_logic_id;
				WingTemplate* wing_upgrade_star_ptr = GET_TEMPLATE(WingTemplate, wing_upgrade_star_id);
				WingTemplate* wing_psychic_ptr = GET_TEMPLATE(WingTemplate, wing_psychic_id);
				if (nullptr != wing_upgrade_star_ptr)
				{
					fighting_power += init_unit::calcu_fighting_power_by_att_array(wing_upgrade_star_ptr->AttArray, att_array);
				}
				if (nullptr != wing_psychic_ptr)
				{
					fighting_power += init_unit::calcu_fighting_power_by_att_array(wing_psychic_ptr->AttArray, att_array);
					if (get_data_info(e_item_info_forge_property1) >= 101)
					{
						fighting_power += init_unit::calcu_fighting_power_by_att_array(wing_psychic_ptr->ExcellentAttArray, att_array);
					}
				}
			}
		}
		break;
		case e_item_type_spirit:
		{

		}
		break;
		default:
			break;
		}

		return fighting_power;

	}
	int64 citem::calcu_single_equip_fighting_power(const double* att_array)
	{
		if (nullptr == att_array)
		{
			return 0;
		}
		ItemTemplate* item_tmpl_ptr = get_item_info_ptr();
		if (nullptr == item_tmpl_ptr)
		{
			return 0;
		}
		if (init_unit::get_slot_by_item_type(item_tmpl_ptr->item_type, item_tmpl_ptr->sub_type) == e_role_equip_slot_max)
		{
			return 0;
		}
		EquipTemplate* equip_tmpl_ptr = get_equip_info_ptr();
		if (nullptr == equip_tmpl_ptr)
		{
			return 0;
		}
		if (nullptr == m_item_info_ptr)
		{
			return 0;
		}

		int64 fighting_power = 0;
		int32 upgrade_num = get_data_info(e_item_info_upgrade_count);

		fighting_power += init_unit::calcu_fighting_power_by_att_array(equip_tmpl_ptr->AttArray, att_array);

		//std::vector<float> temp_excellent_att_array = init_unit::get_excellent_att_array(equip_tmpl_ptr->BaseExcellentAtt, get_item_inst());

		//fighting_power += init_unit::calcu_fighting_power_by_att_array(temp_excellent_att_array, att_array);

		fighting_power += init_unit::calcu_fighting_power_by_att_array(init_unit::get_equip_upgrade_att_array(m_item_info_ptr->logic_id, upgrade_num), att_array);
		
		int32 addon_num = get_data_info(e_item_info_add_on);
		int32 len = equip_tmpl_ptr->EquipAddonAtt.size();
		if (len > 0 && len % e_att_one_max == 0)
		{
			fighting_power += init_unit::calcu_fighting_power_by_att_array(equip_tmpl_ptr->EquipAddonAtt, att_array, addon_num);
		}

		ItemSuccinctTemplate* succinc_template_ptr = get_succinc_info_ptr();
		if (nullptr != succinc_template_ptr)
		{
			std::vector<int32> succinct_att_array;
			for (int32 index = e_item_info_succinct_property1; index <= e_item_info_succinct_property6; index++)
			{
				succinct_att_array.push_back(get_data_info((e_item_info)index));
			}
			int32 real_succinct_size = succinct_att_array.size();
			int32 size_num = succinc_template_ptr->PropertyID.size();
			for (int32 index = 0; index < size_num; index++)
			{
				if (index < real_succinct_size)
				{
					std::vector<float> succinct_att;
					succinct_att.push_back(4);
					succinct_att.push_back(succinc_template_ptr->PropertyID[index]);
					succinct_att.push_back(succinct_att_array[index]);
					succinct_att.push_back(0);
					succinct_att.push_back(1);
					fighting_power += init_unit::calcu_fighting_power_by_att_array(succinct_att, att_array);
				}
			}
		}

		//附魔
		int32 enchant_num = get_data_info(e_item_info_illusion_had_byte);
		int32 enchant_level = enchant_num / hld::max_enchant_type_num;
		int32 enchant_type = enchant_num % hld::max_enchant_type_num;
		int32 enchant_slot = get_data_info(e_item_info_slot) - 1;
		EquipEnchantTemplate* enchant_template_ptr = template_manager::get_instance().get_equip_enchant_template_ptr(enchant_level, enchant_type, enchant_slot);
		if (enchant_template_ptr != nullptr)
		{
			if (enchant_template_ptr->ItemGet.size() % 2 == 0)
			{
				fighting_power += init_unit::calcu_fighting_power_by_att_array(enchant_template_ptr->AttArray, att_array);
			}
		}

		//宝石
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
		{
			int32 jewel_id = get_data_info((e_item_info)i);
			if (jewel_id <= 0)
			{
				continue;
			}
			ItemTemplate* jewel_ptr = GET_TEMPLATE(ItemTemplate, jewel_id);
			if (jewel_ptr == nullptr)
			{
				continue;
			}

			JewelTemplate* jewel_template_ptr = GET_TEMPLATE(JewelTemplate, jewel_ptr->logic_id);
			if (jewel_template_ptr == nullptr)
			{
				continue;
			}
			fighting_power += init_unit::calcu_fighting_power_by_att_array(jewel_template_ptr->JewelAtt, att_array);
		}

		int32 awaken_level = get_data_info(e_item_info_wing_exp);
		if (item_tmpl_ptr->AwakenID != 0 && awaken_level != 0)
		{
			AwakenTemplate * awaken_template = GET_TEMPLATE(AwakenTemplate, item_tmpl_ptr->AwakenID + awaken_level);
			if (awaken_template != nullptr)
			{
				fighting_power += init_unit::calcu_fighting_power_by_att_array(awaken_template->AttArray, att_array);
			}

		}
		return fighting_power;
	}
}