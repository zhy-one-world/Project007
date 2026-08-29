#include "base_map_system.h"
#include "components/scene/base_map_component.h"
#include "components/scene/element_map_component.h"
#include "element_map_system.h"
#include "internet/character.pb.h"
#include "internet/map.pb.h"
#include "internet/net.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "wave_monster_system.h"

using namespace faith;

void element_map_system::start_up(Entity* map_ent, MapTemplate* map_template)
{
	ZoneScoped;
	if (map_template->Type != e_map_type_demon_tower_fuben)
	{
		return;
	}	

	auto element_map_cp = map_ent->add_component<element_map_component>();
	element_map_cp->m_state = e_element_map_state_none;
	wave_monster_system::start_up(map_ent, map_template);
}
void  element_map_system::shut_down(Entity* map_ent)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	wave_monster_system::shut_down(map_ent);
	element_map_cp->m_state = e_element_map_state_close;
	map_ent->remove_component<element_map_component>();
}

void element_map_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	//g_ecs->each<element_map_refrush_component>([&](Entity* ent, ComponentHandle<element_map_refrush_component> amb_cp) -> bool {
	//	if (amb_cp->m_be_call == false)
	//	{
	//		//refresh_boss(ent);
	//		return true;
	//	}
	//	return true;
	//});
}
void element_map_system::on_player_save(Entity* map_ent, map_s2s_element_map_info* msg)
{
	ZoneScoped;

	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
}

void element_map_system::on_player_add(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	element_map_cp->m_state = e_element_map_state_player_join;
	return;
}

void element_map_system::on_player_delete(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
}

void element_map_system::on_player_load_data_over(Entity* map_ent, player* player_ptr)
{
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	element_map_cp->m_state = e_element_map_state_runtime;
}

void element_map_system::on_player_enter_scene(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	wave_monster_system::init_brush(map_ent);
}

void element_map_system::on_player_leave_scene(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
}
void element_map_system::on_player_dead(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	player_ptr->get_transfer_control().teleport_by_template(player_ptr->get_unit_info(e_role_info_main_map_id), 0, 0, empty_guid_64, 0);
}

void element_map_system::on_player_reconnect(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
}

void element_map_system::on_npc_add(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	if (element_map_cp->m_state != e_element_map_state_runtime)
	{
		CONSOLE_ERROR("element_map_cp->m_state:{}", int32(element_map_cp->m_state));
		return;
	}
}

void element_map_system::on_npc_dead(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
}
void element_map_system::on_npc_delete(Entity* map_ent, npc* npc_ptr, bool is_dead)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	if (wave_monster_system::monster_kill_end(map_ent))
	{
		element_map_cp->m_state = e_element_map_state_end_map;
		auto base_map_cp = map_ent->get_component<base_map_component>();
		auto map_template = base_map_cp->m_map_template;
		for (auto& it : base_map_cp->m_player_list)
		{
			auto& player_ref = unit_man::get_player(it);
			if (map_template->FirstWinAwardItems.size() == map_template->FirstWinAwardItemsNum.size())
			{
				std::vector<s_item_template_info> item_list;
				for (size_t i = 0; i < map_template->FirstWinAwardItems.size(); i++)
				{
					item_list.push_back(s_item_template_info(map_template->FirstWinAwardItems[i], map_template->FirstWinAwardItemsNum[i], 1));
				}
				item_system::put_item_into_bag(&player_ref, item_list);
			}
			else
			{
				CONSOLE_ERROR("FirstWinAwardItems size error item_size:{} item_num:{}", map_template->FirstWinAwardItems.size(), map_template->FirstWinAwardItemsNum.size());
			}
			player_ref.get_map_record_mgr()->upgrade_map(base_map_cp->m_map_id);
			player_ref.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_demonstower, base_map_cp->m_map_template->MapLevel);
			//player_ref.get_element_heart_mgr().element_heart_unlock_equip_slot(base_map_cp->m_map_id);
		}

		faith::character_proto_map_fuben_over msg;
		msg.set_fuben_template_id(base_map_cp->m_map_id);
		base_map_system::send_map_message(map_ent, &msg, e_msgindex_s2c_map_fuben_over);
	}
}
void element_map_system::on_unit_be_damage(Entity* map_ent, const int32& attack_index, const int32& be_attack_index, double damage_value)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
}

void element_map_system::open_new_map(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		return;
	}
	if (element_map_cp->m_state != e_element_map_state_end_map)
	{
		CONSOLE_ERROR("element_map_cp->m_state:{}", int32(element_map_cp->m_state));
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();

	auto next_map_template = GET_TEMPLATE(MapTemplate, base_map_cp->m_map_template->NextMapId);
	if (nullptr == next_map_template)
	{
		CONSOLE_ERROR("next_map_template is null id:{}", base_map_cp->m_map_template->NextMapId);
		return;
	}
	player_ptr->full_hp();
	if (next_map_template->WorldID != base_map_cp->m_map_template->WorldID || next_map_template->IslandId != base_map_cp->m_map_template->IslandId)
	{
		player_ptr->get_transfer_control().teleport_by_template(base_map_cp->m_map_template->NextMapId, 0, 0, empty_guid_64, 0);
	}
	else
	{
		shut_down(map_ent);
		wave_monster_system::shut_down(map_ent);
		base_map_system::map_settlement(map_ent, next_map_template);

		start_up(map_ent, next_map_template);
		auto new_element_map_cp = map_ent->get_component<element_map_component>();
		new_element_map_cp->m_state = e_element_map_state_runtime;
		wave_monster_system::start_up(map_ent, next_map_template);
		wave_monster_system::init_brush(map_ent);
		send_map_info(map_ent, player_ptr);

		map_s2c_map_anim_end res;
		res.set_next_map_id(next_map_template->attribute_id);
		player_ptr->send_message_to_self(&res, e_msgindex_s2c_map_anim_end);
	}
}
void element_map_system::send_map_info(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto element_map_cp = map_ent->get_component<element_map_component>();
	if (false == element_map_cp.isValid())
	{
		CONSOLE_INFO("element_map_cp is null");
		return;
	}
	//map_s2c_box_map_info msg;
	//for (int32 i = 0; i < e_role_box_map_max; ++i)
	//{
	//	msg.add_data_array(element_map_cp->m_data_array[i]);
	//}
	//if (unit_index > 0)
	//{
	//	auto& player = unit_man::get_player(unit_index);
	//	player.send_message_to_self(&msg, e_msgindex_s2c_box_map_info);
	//}
	//else
	//{
	//	base_map_system::send_map_message(map_ent, &msg, e_msgindex_s2c_box_map_info);
	//}
}