#include "components/npc/treasure_pic_component.h"
#include "components/scene/box_map_component.h"
#include "logic/drop.h"
#include "logic/npc.hpp"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "utility/random.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/play/wheel_plate_system.h"
#include "system/scene/box_map_system.h"
#include "treasure_pic_system.h"
#include "internet/map.pb.h"
#include "internet/net.pb.h"

using namespace faith;

void treasure_pic_system::start_up(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto npc_template = npc_ptr->get_npc_template();
	if (nullptr == player_ptr)
	{
		CONSOLE_ERROR("nullptr == player_ptr npc_id:{}", npc_template->attribute_id);
		return;
	}
	if (npc_template->NpcType == e_unit_type_award_mark && npc_template->SubType == e_award_mark_type_treasure_pic)
	{
		npc_ptr->m_treasure_pic_component = std::make_shared<treasure_pic_component>();
		auto player_upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, player_ptr->get_unit_info(e_role_info_upgrade_id));
		std::vector<int32> treasure_array = player_upgrade_template_ptr->RandomTreasure;
		while (treasure_array.size() > 3)
		{
			auto random_index = random_gen::get_random(0, treasure_array.size() - 1);
			treasure_array.erase(treasure_array.begin() + random_index);
		}
		for (auto& it : treasure_array)
		{
			npc_ptr->m_treasure_pic_component->m_treasure_pic_id.push_back(it);
		}
	}

	return;
}
void treasure_pic_system::shut_down(npc* npc_ptr)
{
	ZoneScoped;
	npc_ptr->m_treasure_pic_component = nullptr;
}
void treasure_pic_system::get_npc_award(player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto treasure_pic_cp = npc_ptr->m_treasure_pic_component;
	if (treasure_pic_cp == nullptr)
	{
		return;
	}

	wheel_plate_system::wheel_plate_add_score(player_ptr, npc_ptr);

	map_s2c_get_npc_award_end msg;
	msg.set_award_type(e_award_mark_type_treasure_pic);
	msg.set_npc_guid(npc_ptr->get_unit_guid().server_64);
	for (auto& it : treasure_pic_cp->m_treasure_pic_id)
	{
		msg.add_treasure_select_list(it);
	}
	player_ptr->send_message_to_self(&msg, e_msgindex_c2s_get_npc_award_end);
}

void treasure_pic_system::get_treasure_award(player* player_ptr, npc* npc_ptr, int32 treasure_index)
{
	ZoneScoped;
	auto treasure_pic_cp = npc_ptr->m_treasure_pic_component;
	if (treasure_pic_cp == nullptr)
	{
		CONSOLE_ERROR("award_cp == nullptr");
		return;
	}
	if (treasure_index >= treasure_pic_cp->m_treasure_pic_id.size())
	{
		CONSOLE_ERROR("award_cp->treasure_pic_id.size():{} treasure_index:{}", treasure_pic_cp->m_treasure_pic_id.size(), treasure_index);
		return;
	}
	auto treasure_id = treasure_pic_cp->m_treasure_pic_id[treasure_index];
	auto treasure_pic_template = GET_TEMPLATE(TreasurePicTemplate, treasure_id);
	if (treasure_pic_template == nullptr)
	{
		CONSOLE_ERROR("treasure_pic_template == nullptr treasure_id:{}", treasure_id);
		return;
	}
	auto search_level = player_ptr->get_logic_data(e_role_logic_info_search_level);
	if (search_level >= treasure_pic_template->DropIdList.size())
	{
		CONSOLE_ERROR("search_level < treasure_pic_template->SearchLevel search_level:{} treasure_pic_template->DropIdList.size():{}", search_level, treasure_pic_template->DropIdList.size());
		return;
	}
	auto drop_id = treasure_pic_template->DropIdList[search_level];
	std::vector<s_item_template_info> item_list;
	cdrop::gen_drop_list_by_drop_id(drop_id, item_list, 0, 0);

	item_system::put_item_into_bag(player_ptr, item_list);
	player_ptr->send_promp_msg_to_client(item_list);
	box_map_system::clear_multiple_treasure(npc_ptr->get_map_ent(), player_ptr);
	shut_down(npc_ptr);
	unit_man::remove_npc(npc_ptr->get_array_index(), false);
}