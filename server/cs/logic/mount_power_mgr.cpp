#include "mount_power_mgr.h"
#include "mount_power_def.h"
#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "system/item/item_system.h"
#include "internet/mountpower.pb.h"
#include "internet/net.pb.h"
#include "utility/init_unit.h"

namespace faith
{
	mount_power_mgr::mount_power_mgr()
	{
		m_array_index = 0;
		clear_data();

	}

	mount_power_mgr::~mount_power_mgr()
	{

	}

	void mount_power_mgr::clear_data()
	{

	}

	void mount_power_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;

	}

	void mount_power_mgr::on_oper(std::vector<guid_64>& s_item_guid,int32 oper_type, int32 slot, guid_64 d_item_guid)
	{
		if (s_item_guid.size() < 1)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		int32 result = 0;
		switch (oper_type)
		{
		case e_mount_power_oper_equip:
			result = on_equip(s_item_guid[0], slot, d_item_guid);
			break;
		case e_mount_power_oper_star:
			result =  on_star(s_item_guid[0], slot);
			break;
		default:
			break;
		}

		mount_power_proto_mount_power_operate_end msg;
		msg.add_item_guid(s_item_guid[0].A);
		msg.add_item_guid(s_item_guid[0].B);
		msg.set_oper_type(oper_type);
		msg.set_slot(slot);
		msg.set_result(result);

		temp_player.send_message_to_self(&msg, e_msgindex_s2c_mountpower_operate_end);
	}

	int32 mount_power_mgr::on_equip(guid_64 s_item_guid, int32 slot, guid_64 d_item_guid)
	{
		if (slot < e_mount_power_0 || slot > e_mount_power_2)
		{
			return  mount_power_error_unknow;
		}

		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return  mount_power_error_unknow;
		}
		//citem *s_item_ptr = player_ref.get_item_set().get_item(s_item_guid);
		//citem *d_item_ptr = player_ref.get_item_set().get_item(d_item_guid);

		//if (s_item_ptr == nullptr || d_item_ptr == nullptr)
		//{
		//	return mount_power_error_unknow;
		//}

		//MountTemplate *mount_config_ptr = s_item_ptr->get_mount_template_ptr();
		//if (mount_config_ptr == nullptr)
		//{
		//	return  mount_power_error_unknow;
		//}
		//MountEquipTemplate *mount_equ_config_ptr = d_item_ptr->get_mount_equip_template_ptr();
		//if (mount_equ_config_ptr == nullptr)
		//{
		//	return mount_power_error_unknow;
		//}
		//if (s_item_ptr->get_data_info(e_item_info_activate) <= 0)
		//{
		//	return mount_power_error_unknow;
		//}

		//if (mount_equ_config_ptr->Mount != 0 && mount_equ_config_ptr->Mount != s_item_ptr->get_data_info(e_item_info_info_id))
		//{
		//	return mount_power_error_slot_id;
		//}
		//if (mount_equ_config_ptr->EquipSlot != slot)
		//{
		//	return mount_power_error_slot;
		//}
		//int32 old_template_id = s_item_ptr->get_data_info((e_item_info)(mount_power_slot_start + slot * 2));
		//int32 level = s_item_ptr->get_data_info((e_item_info)(mount_power_slot_start + slot * 2 + 1));

		////set_mount_power_attr(old_template_id, level, false);

		//if (old_template_id != 0)
		//{

		//	if (!put_mount_equ_in_bag(old_template_id, 1))
		//	{
		//		return mount_power_error_unknow;
		//	}

		//}
		//set_mount_power_attr(old_template_id, level, false);

		//s_item_ptr->set_data_info((e_item_info)(mount_power_slot_start + slot * 2), d_item_ptr->get_data_info(e_item_info_info_id));


		//set_mount_power_attr(d_item_ptr->get_data_info(e_item_info_info_id), level, true);


		////item_system::cost_item_from_bag(&player_ref.get_item_set().del_item(e_server_log_del_item_mount_equip_off, 0, d_item_ptr, 1);

		//
		//set_core_is_active(s_item_ptr);

		//player_ref.get_item_set().send_item_one(s_item_ptr, true);

		//player_ref.get_item_set().sync_ranking_mount();

		return mount_power_error_sucess;

	}

	MountEquipTemplate * mount_power_mgr::get_mount_equ_template_by_template_id(int32 template_id)
	{
		ItemTemplate *item_ptr = GET_TEMPLATE(ItemTemplate, template_id);
		if (!item_ptr)
			return nullptr;
		return GET_TEMPLATE(MountEquipTemplate, item_ptr->logic_id);
	}

	MountEquipTemplate * mount_power_mgr::get_mount_equ_template_by_mount_id(int32 mount_id)
	{

	   return GET_TEMPLATE(MountEquipTemplate, mount_id);
	}

	void mount_power_mgr::set_mount_power_attr(int32 template_id, int32 level, bool is_add)
	{
		MountEquipTemplate * mount_equ_config_ptr = get_mount_equ_template_by_template_id(template_id);
		if (mount_equ_config_ptr == nullptr)
		{
			return;
		}
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
	
		player_ref.get_pawn_att().apply_att_change_by_array(mount_equ_config_ptr->BaseAtt, is_add);
		player_ref.get_pawn_att().apply_att_change_by_array(mount_equ_config_ptr->UpgradeAttEach, is_add, level);

	}

	bool mount_power_mgr::put_mount_equ_in_bag(int32 template_id, int32 level)
	{
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return false; 
		}


		//citem* p_new_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_add_item_to_bag, 0, template_id);
		//if (p_new_item == nullptr)
		//	return false;
		//p_new_item->set_data_info(e_item_info_upgrade_count, level);

		//return player_ref.get_item_set().put_in_bag(p_new_item);
		return false;
	}

	void mount_power_mgr::set_core_is_active(citem *item_ptr)
	{
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (item_ptr == nullptr)
		{
			return;
		}
		if (item_ptr->get_data_info(e_item_info_activate) <= 0)
		{
			return;
		}
		MountTemplate *mount_config_ptr = item_ptr->get_mount_template_ptr();
		if (mount_config_ptr == nullptr)
		{
			return;
		}

		MountEquipTemplate *mount_equ_main_ptr = get_mount_equ_template_by_mount_id(mount_config_ptr->PowerID);
		if (mount_equ_main_ptr == nullptr || mount_equ_main_ptr->SkillType == 0)   //无核心技能
		{
			return;
		}

		bool is_full_equ = true;
		MountEquipTemplate *mount_equ_config_ptr = nullptr;

		for (int32 i = e_mount_power_0; i <= e_mount_power_2; i++)
		{
			int32 template_id = item_ptr->get_data_info((e_item_info)(mount_power_slot_start + i * 2));
			MountEquipTemplate *mount_equ_config_ptr = get_mount_equ_template_by_template_id(template_id);
			if (mount_equ_config_ptr == nullptr)
			{
				is_full_equ = false;
				break;
			}
			if (mount_equ_config_ptr->IsOnly == 0)
			{
				is_full_equ = false;
				break;
			}
		}
		if (mount_equ_main_ptr->SkillMountLevel.size() != 2)
		{
			return;
		}
		int32 NeedLevel = mount_equ_main_ptr->SkillMountLevel[0] * 11 + mount_equ_main_ptr->SkillMountLevel[1];

		int32 CurLevel =  mount_config_ptr->ClassNow * 11 + mount_config_ptr->GradeNow;

		if (CurLevel < NeedLevel)
		{
			return;
		}
		if (is_full_equ == true)
		{
			int32 is_active = item_ptr->get_data_info(e_item_info(e_item_info_succinct_level));
			if (is_active == 1)
			{
				return;
			}
			item_ptr->set_data_info(e_item_info(e_item_info_succinct_level), 1); // 给客户端

			player_ref.get_passive_skill().passive_skill_special(mount_equ_main_ptr->SkillId, true);

			return;

		}
		else
		{
			int32 is_active = item_ptr->get_data_info(e_item_info(e_item_info_succinct_level));
			if (is_active == 0)
			{
				return;
			}
			item_ptr->set_data_info(e_item_info(e_item_info_succinct_level), 0);

			player_ref.get_passive_skill().passive_skill_special(mount_equ_main_ptr->SkillId, false);
		}
	}


	int32 mount_power_mgr::on_star(guid_64 s_item_guid, int32 slot)
	{

		if (slot < e_mount_power_0 || slot > e_mount_power_2)
		{
			return mount_power_error_unknow;
		}
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return mount_power_error_unknow;
		}
		//citem *s_item_ptr = player_ref.get_item_set().get_item(s_item_guid);
		//if (s_item_ptr == nullptr)
		//{
		//	return mount_power_error_unknow;
		//}
		//if (s_item_ptr->get_data_info(e_item_info_activate) <= 0)
		//{
		//	return mount_power_error_unknow;
		//}

		//MountTemplate *mount_config_ptr = s_item_ptr->get_mount_template_ptr();
		//if (mount_config_ptr == nullptr)
		//{
		//	return mount_power_error_unknow;
		//}
		//MountEquipTemplate *mount_main_ptr = get_mount_equ_template_by_mount_id(mount_config_ptr->PowerID);
		//if (mount_main_ptr == nullptr)
		//{
		//	return mount_power_error_unknow;
		//}
		//int32 template_id = s_item_ptr->get_data_info((e_item_info)(mount_power_slot_start + slot * 2));
		//int32 level = s_item_ptr->get_data_info((e_item_info)(mount_power_slot_start + slot * 2 + 1));

		//if (mount_main_ptr->StarMax <= (level + 1))
		//{
		//	return mount_power_error_max_upgrade;
		//}
		//if (template_id <= 0)
		//{
		//	return mount_power_error_unknow;
		//}
		//MountEquipTemplate *star_up_ptr = get_mount_equ_template_by_item(s_item_ptr, slot, level);
		//if (star_up_ptr == nullptr)
		//{
		//	return mount_power_error_unknow;
		//}
		//if (star_up_ptr->StarMountLevel.size() == 2)
		//{
		//	int32  need_level = star_up_ptr->StarMountLevel[0] * 11 + star_up_ptr->StarMountLevel[1];
		//	int32  cur_level = mount_config_ptr->ClassNow * 11 + mount_config_ptr->GradeNow;
		//	if (cur_level < need_level)
		//	{
		//		return mount_power_error_need_mount_level;
		//	}
		//}

		//if (star_up_ptr->StarCost.size() != 2)
		//{
		//	return mount_power_error_unknow;
		//}

		//if (item_system::can_cost_item(&player_ref, e_bag_type_bag, star_up_ptr->StarCost[0], star_up_ptr->StarCost[1]) == false)
		//{
		//	return mount_power_error_cost_item;
		//}
		//item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, star_up_ptr->StarCost[0], star_up_ptr->StarCost[1]);

		//set_mount_power_attr(template_id, level, false);

		//s_item_ptr->set_data_info((e_item_info)(mount_power_slot_start + slot * 2 + 1), level + 1);

		//set_mount_power_attr(template_id, level + 1, true);

		//player_ref.get_item_set().send_item_one(s_item_ptr, true);

		//player_ref.get_item_set().sync_ranking_mount();

		return mount_power_error_sucess;

		

	}

	MountEquipTemplate *mount_power_mgr::get_mount_equ_template_by_item(citem *item,  int32 slot ,int32 level)
	{
		if (item == nullptr)
		{
			return nullptr;
		}
		MountTemplate *mount_config_ptr = item->get_mount_template_ptr();
		if (mount_config_ptr == nullptr)
		{
			return nullptr;
		}
		MountEquipTemplate *mount_main_ptr = get_mount_equ_template_by_mount_id(mount_config_ptr->PowerID);
		if (mount_main_ptr == nullptr)
		{
			return nullptr;
		}
		if (mount_main_ptr->SlotID.size() != 3)
		{
			return nullptr;
		}
		if (slot < 0 || slot >= mount_main_ptr->SlotID.size())
		{
			return nullptr;
		}
		return  get_mount_equ_template_by_mount_id(mount_main_ptr->SlotID[slot] + level);
	}

	int64 mount_power_mgr::calc_mount_power_fight(citem *item)
	{
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return 0;
		}
		const double* att_array = player_ref.m_pawn_att.get_attack_att_all();
		int64 rank_value = 0;
		for (int32 i = e_mount_power_0; i <= e_mount_power_2; i++)
		{
			int32 template_id = item->get_data_info((e_item_info)(mount_power_slot_start + i * 2));
			int32 level = item->get_data_info((e_item_info)(mount_power_slot_start + i * 2 + 1));

			if (template_id == 0)
			{
				continue;
			}

			MountEquipTemplate * equ_ptr = get_mount_equ_template_by_template_id(template_id);
			if (equ_ptr == nullptr)
			{
				continue;
			}

			rank_value += init_unit::calcu_fighting_power_by_att_array(equ_ptr->BaseAtt, att_array);
			rank_value += init_unit::calcu_fighting_power_by_att_array(equ_ptr->UpgradeAttEach, att_array, level);			

		}
		 
		return rank_value;
	}

	void mount_power_mgr::set_item_attr_all(citem *item, bool is_add)
	{
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return ;
		}
		if (item->get_data_info(e_item_info_activate) <= 0)
		{
			return;
		}

		for (int32 i = e_mount_power_0; i <= e_mount_power_2; i++)
		{
			int32 template_id = item->get_data_info((e_item_info)(mount_power_slot_start + i * 2));
			int32 level = item->get_data_info((e_item_info)(mount_power_slot_start + i * 2 + 1));

			if (template_id == 0)
			{
				continue;
			}

			MountEquipTemplate * equ_ptr = get_mount_equ_template_by_template_id(template_id);
			if (equ_ptr == nullptr)
			{
				continue;
			}
			player_ref.get_pawn_att().apply_att_change_by_array(equ_ptr->BaseAtt, is_add);
			player_ref.get_pawn_att().apply_att_change_by_array(equ_ptr->UpgradeAttEach, is_add, level);
		}
	}

	void mount_power_mgr::set_core_skill(citem *item)
	{
		player & player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (item->get_data_info(e_item_info_activate) <= 0)
		{
			return;
		}

		if (item->get_data_info(e_item_info_succinct_level) == 0)
		{
			return;
		}

		MountTemplate *mount_config_ptr = item->get_mount_template_ptr();
		if (mount_config_ptr == nullptr)
		{
			return;
		}

		MountEquipTemplate *mount_equ_main_ptr = get_mount_equ_template_by_mount_id(mount_config_ptr->PowerID);
		if (mount_equ_main_ptr == nullptr)
		{
			return;
		}
		player_ref.get_passive_skill().passive_skill_special(mount_equ_main_ptr->SkillId, true);
	}


}