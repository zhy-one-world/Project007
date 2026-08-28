
#include "belief_rune_mgr.h"
#include "template/template_manager.h"
#include "logic/player.hpp" 
#include "internet/belief_rune.pb.h"
#include "belief_rune_def.hpp"
#include "internet/net.pb.h"
#include "utility/init_unit.h"

namespace hld
{
	belief_rune_mgr::belief_rune_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}
	belief_rune_mgr::~belief_rune_mgr()
	{

	}
	void belief_rune_mgr::clear_data()
	{
		reset_belief_equip_state_cache();

		m_stage_point = 0;
		m_leave_stage_left_time = -1;
		m_is_in_belief_cloister_battle = false;
		m_is_dead_flag = false;
		m_decompose_rune_piece = 0;
	}

	void belief_rune_mgr::init_data()
	{
		reset_belief_equip_state_cache();
		for (int32 i = 0; i < belief_rune_equip_bag_num; i++)
		{
			e_belief_series_type page_type = (e_belief_series_type)(i / belief_rune_equip_slot_each_type_num);

			//citem* item = m_player_ptr->get_item_set().get_item_by_slot(e_bag_type_belief_rune_equip, i);
			//if (item == nullptr)
			//	continue;

			//// 检查道具类型是否为信仰符文
			//ItemTemplate* item_template_ptr = item->get_item_info_ptr();
			//if (item_template_ptr == nullptr || item_template_ptr->item_type != e_item_type_belief_rune)
			//	continue;
			//
			//// 检查模板数据表里面是否存在该数据
			//BeliefRunesTemplate* BeliefRunesTemplatePtr = get_belief_rune_template(*item);
			//if (BeliefRunesTemplatePtr == nullptr)
			//	continue;

			//set_type_equiped(page_type, BeliefRunesTemplatePtr->AttType, true);
		}

	}

	void belief_rune_mgr::reset_belief_equip_state_cache()
	{
		for (int32 i = 0; i < e_belief_series_type_max; i++)
		{
			m_attr_type_equiped_cache[i] = 0;
		}
	}

	bool belief_rune_mgr::is_type_equiped(e_belief_series_type page_type, int32 belief_rune_type)
	{
		if (belief_rune_type < 0 || belief_rune_type > belief_rune_equip_bag_num)
			return true;

		return (m_attr_type_equiped_cache[page_type] & (1 << (belief_rune_type - 1))) != 0;
	}

	void belief_rune_mgr::set_type_equiped(e_belief_series_type page_type, int32 belief_rune_type, bool is_equiped)
	{
		if (belief_rune_type < 0 || belief_rune_type > belief_rune_equip_bag_num)
			return;

		if (is_equiped == true)
			m_attr_type_equiped_cache[page_type] = m_attr_type_equiped_cache[page_type] | (1 << (belief_rune_type - 1));
		else
			m_attr_type_equiped_cache[page_type] = m_attr_type_equiped_cache[page_type] & (~(1 << (belief_rune_type - 1)));
	}

	void belief_rune_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}
	void belief_rune_mgr::heart_tick(const int64& new_time)
	{

	}

	//模板
	BeliefRunesTemplate* belief_rune_mgr::get_belief_rune_template(const citem& item)
	{
		if (!item.get_item_info_ptr())
		{
			return nullptr;
		}
		return GET_TEMPLATE(BeliefRunesTemplate, item.get_item_info_ptr()->logic_id);
	}
	void belief_rune_mgr::response_operate_result(e_belief_rune_operate_type operate_type, int32 error_id)
	{
		hld::belief_rune_proto_belief_rune_operate_result resp;
		resp.set_operate_type(operate_type);
		resp.set_error_id(error_id);
		if (operate_type == e_belief_rune_operate_type_decompose && m_decompose_rune_piece > 0) {
			resp.set_rune_piece(m_decompose_rune_piece);
			m_decompose_rune_piece = 0;
		}
		m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_belief_rune_operate_result);
	}

	bool belief_rune_mgr::set_belief_rune_one_att(citem& item, bool is_add)
	{
		if (!item.get_item_info_ptr())
		{
			return false;
		}
		BeliefRunesTemplate *BeliefRunesTemplatePtr = get_belief_rune_template(item);
		if (!BeliefRunesTemplatePtr)
		{
			return false;
		}
		m_player_ptr->get_pawn_att().apply_att_change_by_array(BeliefRunesTemplatePtr->BasicAtt1, is_add);
		m_player_ptr->get_pawn_att().apply_att_change_by_array(BeliefRunesTemplatePtr->BasicAtt2, is_add);
		m_player_ptr->get_pawn_att().apply_att_change_by_array(BeliefRunesTemplatePtr->BasicAtt3, is_add);
		m_player_ptr->get_pawn_att().apply_att_change_by_array(BeliefRunesTemplatePtr->UpgradeAttEach1, is_add, item.get_data_info(e_item_info_upgrade_count));
		m_player_ptr->get_pawn_att().apply_att_change_by_array(BeliefRunesTemplatePtr->UpgradeAttEach2, is_add, item.get_data_info(e_item_info_upgrade_count));
		m_player_ptr->get_pawn_att().apply_att_change_by_array(BeliefRunesTemplatePtr->UpgradeAttEach3, is_add, item.get_data_info(e_item_info_upgrade_count));
		return true;
	}

	bool belief_rune_mgr::set_belief_rune_all_att(bool is_add)
	{
		if (m_player_ptr == nullptr)
		{
			return false;
		}
		bool end_state = true;
		//for (int32 equip_slot = 0; equip_slot < belief_rune_equip_bag_num; equip_slot++)
		//{
		//	citem* item = m_player_ptr->get_item_set().get_item_by_slot(e_bag_type_belief_rune_equip, equip_slot);
		//	if (item != nullptr)
		//	{
		//		end_state = end_state && set_belief_rune_one_att(*item, is_add);
		//	}
		//}
		return end_state;
	}

	int64 belief_rune_mgr::calcu_total_fighting_power()
	{
		if (m_player_ptr == nullptr)
		{
			return 0;
		}
		const double* att_array = m_player_ptr->m_pawn_att.get_attack_att_all();
		int64 ranking_value = 0;
		for (int32 equip_slot = 0; equip_slot < belief_rune_equip_bag_num; equip_slot++)
		{
			//citem* item = m_player_ptr->get_item_set().get_item_by_slot(e_bag_type_belief_rune_equip, equip_slot);
			//if (nullptr != item && nullptr != item->get_item_info_ptr())
			//{
			//	BeliefRunesTemplate *BeliefRunesTemplatePtr = get_belief_rune_template(*item);
			//	if (nullptr != BeliefRunesTemplatePtr)
			//	{
			//		ranking_value += init_unit::calcu_fighting_power_by_att_array(BeliefRunesTemplatePtr->BasicAtt1, att_array);
			//		ranking_value += init_unit::calcu_fighting_power_by_att_array(BeliefRunesTemplatePtr->BasicAtt2, att_array);
			//		ranking_value += init_unit::calcu_fighting_power_by_att_array(BeliefRunesTemplatePtr->BasicAtt3, att_array);
			//		ranking_value += init_unit::calcu_fighting_power_by_att_array(BeliefRunesTemplatePtr->UpgradeAttEach1, att_array, item->get_data_info(e_item_info_upgrade_count));
			//		ranking_value += init_unit::calcu_fighting_power_by_att_array(BeliefRunesTemplatePtr->UpgradeAttEach2, att_array, item->get_data_info(e_item_info_upgrade_count));
			//		ranking_value += init_unit::calcu_fighting_power_by_att_array(BeliefRunesTemplatePtr->UpgradeAttEach3, att_array, item->get_data_info(e_item_info_upgrade_count));
			//	}
			//}
		}
		return ranking_value;
	}
}