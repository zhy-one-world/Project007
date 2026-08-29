#include "box_award_system.h"
#include "components/npc/box_award_component.h"
#include "internet/map.pb.h"
#include "logic/drop.h"
#include "logic/item_set.h"
#include "logic/npc.hpp"
#include "internet/net.pb.h"
#include "server_log.hpp"
#include "server_log_def.hpp"
#include "system/item/item_system.h"
#include "system/play/finger_guess_system.h"
#include "system/play/smashing_bottle_system.h"
#include "system/play/wheel_plate_system.h"
#include "system/scene/box_map_system.h"
#include "system/scene/flip_box_system.h"
#include "internet/aoi.pb.h"


using namespace faith;

void box_award_system::start_up(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto npc_template = npc_ptr->get_npc_template();
	if (npc_template->NpcType != e_unit_type_award_mark)
	{
		return;
	}
	if (nullptr == player_ptr)
	{
		CONSOLE_ERROR("nullptr == player_ptr npc_id:{}", npc_template->attribute_id);
		return;
	}


	if (npc_template->SubType != e_award_mark_type_treasure_pic)
	{
		npc_ptr->m_box_award_component = std::make_shared<box_award_component>();
		npc_ptr->m_box_award_component->m_can_get_award = false;
		play_collection(player_ptr, npc_ptr);
	}
	return;
}
void box_award_system::shut_down(npc* npc_ptr)
{
	ZoneScoped;
	npc_ptr->m_box_award_component = nullptr;
}
void box_award_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
}
void box_award_system::aoi_msg(npc* npc_ptr, aoi_proto_unit_aoi_all& msg)
{
	ZoneScoped;
	auto award_cp = npc_ptr->m_box_award_component;
	if (award_cp)
	{
		for (auto& it : award_cp->m_drop_item)
		{
			msg.add_self_award_list(it.m_item_id);
			msg.add_self_award_list(it.m_item_num);
			msg.add_self_award_list(it.m_lock);
		}
	}
}
void box_award_system::get_npc_award(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto award_cp = npc_ptr->m_box_award_component;
	if (award_cp == nullptr)
	{
		return;
	}
	if (award_cp->m_can_get_award == false)
	{
		CONSOLE_ERROR("award_cp->m_can_get_award == false unit_index:{}", npc_ptr->get_array_index());
		return;
	}
	award_cp->m_can_get_award = false;

	auto npc_template = npc_ptr->get_npc_template();
	if (npc_template->NpcType != e_unit_type_award_mark)
	{
		CONSOLE_ERROR("npc_template->NpcType :{}", npc_template->NpcType);
		return;
	}

	wheel_plate_system::wheel_plate_add_score(player_ptr, npc_ptr);

	auto spawn_id = npc_ptr->get_spawn_point_id();
	auto multiple_drop_id = npc_template->MultipleDropId;
	auto award_mark_type = (e_award_mark_type)npc_ptr->get_unit_sub_type();

	map_s2c_get_npc_award_end msg;
	msg.set_award_type(award_mark_type);
	msg.set_npc_guid(npc_ptr->get_unit_guid().server_64);

	switch (award_mark_type)
	{
	case e_award_mark_type_nomal:
	{
		item_system::put_item_into_bag(player_ptr, award_cp->m_drop_item);
	}
	break;
	case e_award_mark_type_special:
	{
		box_map_system::get_npc_award(npc_ptr->get_map_ent(), player_ptr, award_mark_type, spawn_id);
	}
	break;
	case e_award_mark_type_bank_robbery:
	{
		flip_box_system::start_up(npc_ptr->get_map_ent(), npc_template->FlipBoxId, player_ptr);
	}
	break;
	case e_award_mark_type_refrush_monster:
	{
		npc_ptr->get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_common_monster);
		npc_ptr->get_pawn_att().send_base_att_one(e_base_att_info_community_type);
	}
	break;
	case e_award_mark_type_finger_guess:
	{
		finger_guess_system::start_up(player_ptr);
	}
	break;
	case e_award_mark_type_smashing_bottle:
	{
		smashing_bottle_system::start_up(player_ptr, npc_template->FlipBoxId);
	}
	break;
	case e_award_mark_type_transfer:
	{
		auto big_npc = box_map_system::get_npc_by_level(npc_ptr->get_map_ent(), e_award_mark_level_big);
		if (big_npc)
		{
			big_npc->m_box_award_component->m_can_get_award = true;
			msg.set_ex_npc_guid(big_npc->get_unit_guid().server_64);
			auto map_pos = npc_ptr->get_new_map_pos();
			player_ptr->transfer_local_pos(map_pos, false);
		}
	}
	break;
	default:
		CONSOLE_ERROR("award_mark_type :{}", (int32)award_mark_type);
		break;
	}
	player_ptr->send_message_to_self(&msg, e_msgindex_c2s_get_npc_award_end);
}

void box_award_system::play_collection(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto npc_template = npc_ptr->get_npc_template();
	switch (npc_template->AwardType)
	{
	case e_box_award_type_player_search_level:
	{
		drop_item_by_player_search_level(player_ptr, npc_ptr);
	}break;
	case e_box_award_type_self_drop:
	{
		drop_item_by_self_drop(player_ptr, npc_ptr);
	}break;
	default:
		CONSOLE_ERROR("AwardType:{}", npc_template->AwardType);
		break;
	}
}
void box_award_system::drop_item_by_player_search_level(player* player_ptr, npc* npc_ptr)
{
	auto search_level = player_ptr->get_logic_data(e_role_logic_info_search_level);
	auto player_upgrade_template = template_manager::get_instance().get_player_upgrade_by_level(player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));
	if (player_upgrade_template == nullptr)
	{
		CONSOLE_ERROR("player_upgrade_template is nullptr class_type:{} exp_level:{}", player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));
		return;
	}
	if (search_level >= player_upgrade_template->RandomItem.size())
	{
		CONSOLE_ERROR("search_level is out of range search_level:{} RandomItem_size:{}", search_level, player_upgrade_template->RandomItem.size());
		return;
	}
	auto search_level_template = GET_TEMPLATE(SearchLevelTemplate, search_level + search_level_template_id_start);
	if (nullptr == search_level_template)
	{
		CONSOLE_ERROR("search_level_template is null search_level:{} player_index:{}", search_level, player_ptr->get_array_index());
		return;
	}
	auto random_item = player_upgrade_template->RandomItem[search_level];
	auto box_award_cp = npc_ptr->m_box_award_component;

	cdrop::gen_drop_list_by_drop_id(random_item, box_award_cp->m_drop_item, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));
}
void box_award_system::drop_item_by_self_drop(player* player_ptr, npc* npc_ptr)
{
	auto box_award_cp = npc_ptr->m_box_award_component;
	for (auto& it : npc_ptr->get_npc_template()->DropID)
	{
		cdrop::gen_drop_list_by_drop_id(it, box_award_cp->m_drop_item, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));
	}
}