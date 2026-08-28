/********************************************************************
created: 2017年6月8日15:55
file base: patron_saint
file ext: cpp
author: Lxy

purpose: about role's pokedex_mgr
*********************************************************************/

#include "internet/item.pb.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "lua/script_mgr.h"
#include "internet/net.pb.h"
#include "patron_saint_mgr.h"
#include "system/item/item_system.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "internal/pokedex_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "utility/parse_msg.h"
#include "internet/pokedex.pb.h"
namespace hld
{
	cpatron_saint_mgr::cpatron_saint_mgr()
	{
		clear_data();

	}

	cpatron_saint_mgr::~cpatron_saint_mgr(void)
	{
		clear_data();
	}

	void cpatron_saint_mgr::set_player_array_index(int32 array_index)
	{
		m_unit_array_index = array_index;
	}

	void cpatron_saint_mgr::clear_data()
	{

	}

	void cpatron_saint_mgr::heart_tick()
	{

	}

	int32 cpatron_saint_mgr::purchase_prop_slot()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return e_item_string_unkown;
		}
		TinderTemplate* tinder_template_ptr_unlock = GET_TEMPLATE(TinderTemplate, get_tinder_info(e_tinder_info_current_unlock_template_id));
		if (nullptr == tinder_template_ptr_unlock)
		{
			return e_item_string_unkown;
		}
		/////////////////////////
		int32 cur_unlock_order = tinder_template_ptr_unlock->CurrentOrder;
		if (cur_unlock_order > hld::tinder_old_num_max)
		{
			cur_unlock_order += 2; // 后面只有4个
		}
		int32 cur_equip_slot_can_open_num = cur_unlock_order / 6;
		/////////////////////////
		//int32 cur_equip_slot_can_open_num = tinder_template_ptr_unlock->CurrentOrder / 6;

		int32 cur_slot_open_num = temp_player.get_unit_info(e_role_info_tinder_prop_slot_open_num);
		if (cur_slot_open_num >= cur_equip_slot_can_open_num)
		{
			return e_item_string_unkown;
		}

		temp_player.set_unit_info(e_role_info_tinder_prop_slot_open_num, cur_slot_open_num + 1);
		temp_player.send_info_one(e_role_info_tinder_prop_slot_open_num);

		return e_item_string_succeed;
	}

	int32 cpatron_saint_mgr::prop_equip(guid_64 mark_guid, int32 target_slot)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return e_item_string_unkown;
		}
		//citem* new_slot_mark = temp_player.get_item_set().get_item(mark_guid);
		//if (new_slot_mark == nullptr)
		//{
		//	return e_item_string_unkown;
		//}
		//if (new_slot_mark->get_data_info(e_item_info_container_type) != e_bag_type_bag )
		//{
		//	return e_item_string_unkown;
		//}		
		//TinderTemplate* tinder_template_ptr_unlock = GET_TEMPLATE(TinderTemplate, get_tinder_info(e_tinder_info_current_unlock_template_id));
		//if (nullptr == tinder_template_ptr_unlock)
		//{
		//	return e_item_string_unkown;
		//}
		///////////////////////////////////////////////////////
		//int32 cur_unlock_order = tinder_template_ptr_unlock->CurrentOrder;
		//if (cur_unlock_order > hld::tinder_old_num_max)
		//{
		//	cur_unlock_order += 2; // 后面只有4个
		//}
		////int32 cur_equip_slot_open_num = tinder_template_ptr_unlock->CurrentOrder / 6;
		//int32 cur_equip_slot_open_num = cur_unlock_order / 6;
		/////////////////////////////////////////////////
		//if (target_slot >= cur_equip_slot_open_num)
		//{
		//	return e_item_string_unkown;
		//}
		//if (target_slot < 0)
		//{
		//	return e_item_tinder_skill_slot_not_enough;
		//}
		//ItemTemplate* mark_item_template_ptr = new_slot_mark->get_item_info_ptr();
		//if (mark_item_template_ptr == nullptr || mark_item_template_ptr->item_type != e_item_type_stone || mark_item_template_ptr->sub_type != e_stone_sub_type_tinder_prop)
		//{
		//	return e_item_string_unkown;
		//}		
		//citem* old_slot_mark = temp_player.get_item_set().get_item_by_slot(e_bag_type_equip_tinder_prop, target_slot);
		//if (nullptr != old_slot_mark)
		//{
		//	ItemTemplate* old_item_template_ptr = old_slot_mark->get_item_info_ptr();
		//	if (old_item_template_ptr == nullptr)
		//	{
		//		return e_item_string_unkown;
		//	}
		//	int32 already_used_tinder_star = get_tinder_star_already_used();
		//	if (already_used_tinder_star > m_tinder_info.data_ary[e_tinder_info_total_star_level]+ m_tinder_info.data_ary[e_tinder_info_add_star_level] - old_item_template_ptr->FashionId + mark_item_template_ptr->FashionId)
		//	{
		//		return e_item_string_tinder_point_not_enough;
		//	}
		//	int32 old_mark_go_to_slot = new_slot_mark->get_data_info(e_item_info_slot);
		//	old_slot_mark->set_data_info(e_item_info_container_type, e_bag_type_bag);
		//	old_slot_mark->set_data_info(e_item_info_slot, old_mark_go_to_slot);
		//	temp_player.get_item_set().set_target_slot_guid(e_bag_type_bag, old_mark_go_to_slot, old_slot_mark->get_item_guid());
		//	temp_player.get_item_set().send_item_one(old_slot_mark);
		//	m_tinder_info.data_ary[e_tinder_info_add_star_level] -= old_item_template_ptr->FashionId;
		//}
		//else
		//{
		//	temp_player.get_item_set().clear_target_slot(e_bag_type_bag, new_slot_mark->get_data_info(e_item_info_slot));
		//}
		//m_tinder_info.data_ary[e_tinder_info_add_star_level] += mark_item_template_ptr->FashionId;
		//new_slot_mark->set_data_info(e_item_info_container_type, e_bag_type_equip_tinder_prop);
		//new_slot_mark->set_data_info(e_item_info_slot, target_slot);
		//temp_player.get_item_set().set_target_slot_guid(e_bag_type_equip_tinder_prop, target_slot, new_slot_mark->get_item_guid());
		//temp_player.get_item_set().send_item_one(new_slot_mark);
		send_tinder_info_one(e_tinder_info_add_star_level);
		return e_item_string_succeed;
	}

	int32 cpatron_saint_mgr::prop_unequip(guid_64 mark_guid)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return e_item_string_unkown;
		}
		//citem* target_mark = temp_player.get_item_set().get_item(mark_guid);
		//if (target_mark == nullptr)
		//{
		//	return e_item_string_unkown;
		//}
		//if (target_mark->get_data_info(e_item_info_container_type) != e_bag_type_equip_tinder_prop)
		//{
		//	return e_item_string_unkown;
		//}
		//ItemTemplate* mark_item_template_ptr = target_mark->get_item_info_ptr();
		//if (mark_item_template_ptr == nullptr || mark_item_template_ptr->item_type != e_item_type_stone || mark_item_template_ptr->sub_type != e_stone_sub_type_tinder_prop)
		//{
		//	return e_item_string_unkown;
		//}
		//int32 target_operate_slot = temp_player.get_item_set().get_empty_slot(e_bag_type_bag);
		//if (target_operate_slot < 0 || target_operate_slot >= MAX_ITEM_BAG_NUM)
		//{
		//	return e_item_string_unkown;
		//}
		//int32 already_used_tinder_star = get_tinder_star_already_used();
		//if (already_used_tinder_star > m_tinder_info.data_ary[e_tinder_info_total_star_level] + m_tinder_info.data_ary[e_tinder_info_add_star_level] - mark_item_template_ptr->FashionId)
		//{
		//	return e_item_string_tinder_point_not_enough;
		//}
		//m_tinder_info.data_ary[e_tinder_info_add_star_level] -= mark_item_template_ptr->FashionId;
		//temp_player.get_item_set().clear_target_slot(e_bag_type_equip_tinder_prop, target_mark->get_data_info(e_item_info_slot));
		//target_mark->set_data_info(e_item_info_container_type, e_bag_type_bag);
		//target_mark->set_data_info(e_item_info_slot, target_operate_slot);
		//temp_player.get_item_set().set_target_slot_guid(e_bag_type_bag, target_operate_slot, target_mark->get_item_guid());
		//
		//temp_player.get_item_set().send_item_one(target_mark);
		send_tinder_info_one(e_tinder_info_add_star_level);
		return e_item_string_succeed;
	}
	void cpatron_saint_mgr::tinder_operate(guid_64 mark_guid, int32 target_slot, int32 operate_type)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		int32 operate_sult = e_item_string_unkown;
		switch (operate_type)
		{
		case hld::e_patron_saint_mark_operate_type_equip_prop:
			operate_sult = prop_equip(mark_guid, target_slot);
			break;
		case hld::e_patron_saint_mark_operate_type_unequip_prop:
			operate_sult = prop_unequip(mark_guid);
			break;
		case hld::e_patron_saint_mark_operate_type_purchase_prop_new_slot:
			operate_sult = purchase_prop_slot();
			break;
		default:
			break;
		}
		item_proto_tinder_prop_operate_end msg;
		msg.set_mark_guid_a(mark_guid.A);
		msg.set_mark_guid_b(mark_guid.B);
		msg.set_target_slot(target_slot);
		msg.set_operate_type(operate_type);
		msg.set_operate_result(operate_sult);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_tinder_prop_operate_end);
	}

	int32 cpatron_saint_mgr::get_tinder_star_already_used()
	{
		int32 already_used_tinder_star = 0;
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return already_used_tinder_star;
		}

		citem* temp_mark_ptr = nullptr;
		for (int32 i = 0; i < max_proctect_mark_equip_slot_num; i++)
		{
			//temp_mark_ptr = temp_player.get_item_set().get_item_by_slot(e_bag_type_equip_protect, i);
			//if (nullptr == temp_mark_ptr)
			//{
			//	continue;
			//}

			//ProtectSpiritTemplate* temp_mark_template_ptr = temp_mark_ptr->get_protect_spirit_ptr();
			//if (nullptr == temp_mark_template_ptr)
			//{
			//	continue;
			//}
			//already_used_tinder_star += temp_mark_template_ptr->NeedStarLevel;
		}
		return already_used_tinder_star;
	}

	int32 cpatron_saint_mgr::get_mark_level_sum()
	{
		return get_tinder_info(e_tinder_info_total_star_level);
	}

	void cpatron_saint_mgr::sync_tinder_data()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		const double* att_array = temp_player.m_pawn_att.get_attack_att_all();
		int64 level_ranking_value = 0;
		int64 unlock_ranking_value = 0;
		
		TinderTemplate* tinder_unlock_template_ptr = GET_TEMPLATE(TinderTemplate, m_tinder_info.data_ary[e_tinder_info_current_unlock_template_id]);
		if (nullptr != tinder_unlock_template_ptr)
		{
			unlock_ranking_value = init_unit::calcu_fighting_power_by_att_array(tinder_unlock_template_ptr->AddAtt, att_array);
		}

		TinderTemplate* tinder_level_template_ptr = GET_TEMPLATE(TinderTemplate, m_tinder_info.data_ary[e_tinder_info_current_level_template_id]);
		if (nullptr != tinder_level_template_ptr)
		{
			level_ranking_value = init_unit::calcu_fighting_power_by_att_array(tinder_level_template_ptr->AddAtt, att_array);
		}
		
		int32 ranking_value = unlock_ranking_value + level_ranking_value;
		temp_player.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_inspiration, ranking_value);
	}

	int32 cpatron_saint_mgr::get_mark_activated_num()
	{
		return 0;
	}

	void cpatron_saint_mgr::auto_get_equip_slot()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		int32 cur_grade = init_unit::get_player_grade_num(temp_player.get_unit_info(e_role_info_exp_level));

		int32 max_slot_open_req_num = GAMECONFIG->ProtectMarkSlotOpenGradeReqArray.size();
		int32 new_open_num = 0;
		if (cur_grade < 0 || cur_grade >= max_slot_open_req_num)
		{
			new_open_num = max_slot_open_req_num;
		}
		else
		{
			for (int32 i = 0; i < max_slot_open_req_num; ++i)
			{
				if (cur_grade >= GAMECONFIG->ProtectMarkSlotOpenGradeReqArray[i])
				{
					new_open_num++;
				}
			}
		}
		if (new_open_num > temp_player.get_unit_info(e_role_info_protect_mark_slot_open_num))
		{
			temp_player.set_unit_info(e_role_info_protect_mark_slot_open_num, new_open_num);
			temp_player.send_info_one(e_role_info_protect_mark_slot_open_num);
		}
	}

	void cpatron_saint_mgr::save_tinder_info_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_tinder);
		if (is_use == false)
		{
			cs2dp_save_char_tinder req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_unit_array_index;
			req.tinder_data = m_tinder_info;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			hld::cs2dp_proto::save_role_tinder msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_unit_array_index);
			msg.set_save_type_ex(save_type);

			hld::cs2dp_proto::role_tinder_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0 ; i < e_tinder_info_max; i++)
			{
				db_data->add_data_ary(m_tinder_info.data_ary[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_tinder);
		}
	}

	void cpatron_saint_mgr::load_tinder_from_db(const s_tinder_info& tinder_data)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		m_tinder_info = tinder_data;
		send_tinder_info_all();
		set_unit_skill();
	}
	bool cpatron_saint_mgr::load_tinder_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_tinder_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_tinder_info db_data;

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}

		load_tinder_from_db(db_data);
		return true;
	}

	void cpatron_saint_mgr::send_tinder_info_all()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		pokedex_proto_update_tinder_info_all tinder_msg;
		for (int32 i = 0; i < e_tinder_info_max; ++i)
		{
			tinder_msg.add_tinder_info(m_tinder_info.data_ary[i]);
		}
		temp_player.send_message_to_self(&tinder_msg, e_msgindex_s2c_tinder_info_all);
	}

	void cpatron_saint_mgr::send_tinder_info_one(int32 info_index)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (info_index < 0 || info_index >= e_tinder_info_max)
		{
			return;
		}

		pokedex_proto_update_tinder_info_one tinder_msg;
		tinder_msg.set_info_index(info_index);
		tinder_msg.set_info_value(m_tinder_info.data_ary[info_index]);
		temp_player.send_message_to_self(&tinder_msg, e_msgindex_s2c_tinder_info_one);
	}
	void cpatron_saint_mgr::set_tinder_att_all(bool is_add)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		//TinderTemplate* tinder_template_ptr = GET_TEMPLATE(TinderTemplate, m_tinder_info.data_ary[e_tinder_info_current_level_template_id]);
		//if (nullptr != tinder_template_ptr)
		//{
		//	temp_player.get_item_set().item_change_att(tinder_template_ptr->AddAtt, 1, is_add);
		//}

		//TinderTemplate* tinder_template_ptr_unlock = GET_TEMPLATE(TinderTemplate, m_tinder_info.data_ary[e_tinder_info_current_unlock_template_id]);
		//if (nullptr != tinder_template_ptr_unlock)
		//{
		//	temp_player.get_item_set().item_change_att(tinder_template_ptr_unlock->AddAtt, 1, is_add);
		//}

		//for (int32 i = 0; i < max_proctect_mark_equip_slot_num; ++i)
		//{
		//	citem* cur_slot_mark = temp_player.get_item_set().get_item_by_slot(e_bag_type_equip_protect, i);
		//	if (nullptr == cur_slot_mark)
		//	{
		//		continue;
		//	}
		//	ProtectSpiritTemplate* mark_template_ptr = cur_slot_mark->get_protect_spirit_ptr();
		//	if (nullptr == mark_template_ptr)
		//	{
		//		continue;
		//	}
		//	temp_player.get_passive_skill().passive_skill_special(mark_template_ptr->SkillId, is_add);
		//}
	}

	int32 cpatron_saint_mgr::set_unit_skill()
	{
		int32 result = 0;
		script_mgr::get_instance().call_func("patron_saint", "set_unit_skill_cpp", 1, false, "%d>%d", m_unit_array_index, &result);
		return result;
		/*
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (GAMECONFIG->PatronSaintSkillStartId.size() <= player_ref.get_unit_info(e_role_info_class_type))
		{
			return 0;
		}
		skill_set& skill_set_ptr = skill_manager::get_skill_set(m_unit_array_index);
		for (int32 i = 1; i < GAMECONFIG->PatronSaintSkillStartId.size(); i++)
		{
			skill* skill_ptr = skill_set_ptr.get_skill_inst_by_id(GAMECONFIG->PatronSaintSkillStartId[i]);
			if (nullptr == skill_ptr)
			{
				return 0;
			}
			TinderTemplate* tinder_template_ptr_unlock = GET_TEMPLATE(TinderTemplate, m_tinder_info.data_ary[e_tinder_info_current_unlock_template_id]);
			if (nullptr == tinder_template_ptr_unlock)
			{
				return 0;
			}
			if (skill_ptr->get_data_info(e_skill_info_activated) <= 0 && 
				(tinder_template_ptr_unlock->CurrentOrder >= (hld::tinder_order_num_one_canto * i) ||
				 tinder_template_ptr_unlock->CurrentOrder == hld::tinder_new_order_max))
			{
				skill_set_ptr.activate_skill(GAMECONFIG->PatronSaintSkillStartId[i]);
				return 1;
			}
		}
		return 0;
		*/
	}
	bool cpatron_saint_mgr::is_cur_unlock(int32 new_template_id)
	{
		int32 unlock_template_id = m_tinder_info.data_ary[e_tinder_info_current_unlock_template_id];

		TinderTemplate* unlock_tinder_template_ptr = GET_TEMPLATE(TinderTemplate, unlock_template_id);
		if (unlock_tinder_template_ptr == nullptr)
		{
			return false;
		}
		TinderTemplate * upgrade_tinder_template_ptr = GET_TEMPLATE(TinderTemplate, new_template_id);
		if (upgrade_tinder_template_ptr == nullptr)
		{
			return false;
		}
		if (unlock_tinder_template_ptr->CurrentOrder < upgrade_tinder_template_ptr->CurrentOrder)
		{
			return false;
		}
		return true;
	}
	int32 cpatron_saint_mgr::get_tinder_total_upgrade_num()
	{
		if (m_tinder_info.data_ary[e_tinder_info_current_level_template_id] <= 0)
		{
			return 0;
		}
		return m_tinder_info.data_ary[e_tinder_info_current_level_template_id] - tinder_level_start_id + 1;
	}
	bool cpatron_saint_mgr::is_matiral(TinderTemplate *new_tinder_template_ptr)
	{
		if (new_tinder_template_ptr == nullptr)
		{
			return false;
		}
		if (new_tinder_template_ptr->attribute_id <= hld::tinder_old_end_id)
		{
			return true;
		}

		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}

		int32 need_matiral = new_tinder_template_ptr->MoneyConsume.size();
		if (need_matiral == 0 || need_matiral % 2 != 0)
		{
			return false;
		}
		for (int32 i = 0; i < need_matiral; i += 2)
		{
			int32  matiral_id = new_tinder_template_ptr->MoneyConsume[i];
			int32  matiral_num = new_tinder_template_ptr->MoneyConsume[i + 1];
			if (item_system::can_cost_item(&temp_player, e_bag_type_bag, matiral_id, matiral_num) == false)
			{
				return false;
			}
		}
		return true;
	}
	bool cpatron_saint_mgr::cut_matiral(TinderTemplate *new_tinder_template_ptr)
	{
		if (new_tinder_template_ptr == nullptr)
		{
			return false;
		}
		if (new_tinder_template_ptr->attribute_id <= hld::tinder_old_end_id)
		{
			return true;
		}
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}

		int32 need_matiral = new_tinder_template_ptr->MoneyConsume.size();
		if (need_matiral == 0 || need_matiral % 2 != 0)
		{
			return false;
		}
		for (int32 i = 0; i < need_matiral; i += 2)
		{
			int32  matiral_id = new_tinder_template_ptr->MoneyConsume[i];
			int32  matiral_num = new_tinder_template_ptr->MoneyConsume[i + 1];
			item_system::cost_item_from_bag(&temp_player, e_bag_type_bag, matiral_id, matiral_num);

		}
		return true;
	}

	int32 cpatron_saint_mgr::protect_skill_upgrade()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return 0;
		}

		set_unit_skill();

		sync_tinder_data();
		
		return 1;
	}

	int32 cpatron_saint_mgr::get_tinder_info(int32 info_index)
	{
		if (info_index < 0 || info_index >= e_tinder_info_max)
		{
			return 0;
		}
		return m_tinder_info.data_ary[info_index];
	}

	void cpatron_saint_mgr::set_tinder_info(int32 info_index, int32 info_value)
	{
		if (info_index < 0 || info_index >= e_tinder_info_max)
		{
			return;
		}
		m_tinder_info.data_ary[info_index] = info_value;
	}

}
