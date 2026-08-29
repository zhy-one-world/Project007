#include "battle_map_system.h"
#include "base_map_system.h"
#include "wave_monster_system.h"
#include "server_log.hpp"
#include "logic/unit_man.h"
#include "internet/map.pb.h"
#include "components/scene/base_map_component.h"
#include "components/scene/battle_map_component.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "../item/item_system.h"
#include "internet/character.pb.h"
#include "internet/net.pb.h"

using namespace faith;

void battle_map_system::start_up(Entity* map_ent, MapTemplate* map_template)
{
	ZoneScoped;
	if (map_template->Type != e_map_type_battle)
	{
		return;
	}

	auto battle_map_cp = map_ent->add_component<battle_map_component>();
	battle_map_cp->m_map_state = e_battle_map_state_none;
	wave_monster_system::start_up(map_ent, map_template);
}
void  battle_map_system::shut_down(Entity* map_ent)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	wave_monster_system::shut_down(map_ent);
	battle_map_cp->m_map_state = e_battle_map_state_close;
	map_ent->remove_component<battle_map_component>();
}

void battle_map_system::heart_tick(const int64& new_time)
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
void battle_map_system::on_player_save(Entity* map_ent, map_s2s_battle_map_info* msg)
{
	ZoneScoped;

	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
}

void battle_map_system::on_player_add(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	battle_map_cp->m_map_state = e_battle_map_state_player_join;
	return;
}

void battle_map_system::on_player_delete(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
}

void battle_map_system::on_player_load_data_over(Entity* map_ent, player* player_ptr)
{
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	battle_map_cp->m_map_state = e_battle_map_state_runtime;
}

void battle_map_system::on_player_enter_scene(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	wave_monster_system::init_brush(map_ent);
}

void battle_map_system::on_player_leave_scene(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
}
void battle_map_system::on_player_dead(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	player_ptr->get_transfer_control().teleport_by_template(player_ptr->get_unit_info(e_role_info_main_map_id), 0, 0, empty_guid_64, 0);
}

void battle_map_system::on_player_reconnect(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
}

void battle_map_system::on_npc_add(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	if (battle_map_cp->m_map_state != e_battle_map_state_runtime)
	{
		CONSOLE_ERROR("battle_map_cp->m_map_state:{}", int32(battle_map_cp->m_map_state));
		return;
	}
}

void battle_map_system::on_npc_dead(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
}
void battle_map_system::on_npc_delete(Entity* map_ent, npc* npc_ptr, bool is_dead)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
	if (wave_monster_system::monster_kill_end(map_ent))
	{
		battle_map_cp->m_map_state = e_battle_map_state_end_map;
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
				player_ref.send_promp_msg_to_client(item_list);
			}
			else
			{
				CONSOLE_ERROR("FirstWinAwardItems size error item_size:{} item_num:{}", map_template->FirstWinAwardItems.size(), map_template->FirstWinAwardItemsNum.size());
			}
			//扣副本次数
			player_ref.cost_enter_scene_consume();
			player_ref.get_map_record_mgr()->upgrade_map(base_map_cp->m_map_id);
		}

		faith::character_proto_map_fuben_over msg;
		msg.set_fuben_template_id(base_map_cp->m_map_id);
		base_map_system::send_map_message(map_ent, &msg, e_msgindex_s2c_map_fuben_over);
	}
}
void battle_map_system::on_unit_be_damage(Entity* map_ent, const int32& attack_index, const int32& be_attack_index, double damage_value)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto battle_map_cp = map_ent->get_component<battle_map_component>();
	if (false == battle_map_cp.isValid())
	{
		return;
	}
}
void battle_map_system::mop_up_map(player* player_ptr, int32 map_id)
{
	ZoneScoped;
	auto map_template = GET_TEMPLATE(MapTemplate, map_id);
	if (nullptr == map_template)
	{
		CONSOLE_ERROR("map_template is null map_id:{}", map_id);
		return;
	}
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(map_id);
	if (nullptr == map_record_ptr)
	{
		CONSOLE_ERROR("map_record_ptr is null map_id:{}", map_id);
		return;
	}
	int32 player_vip_level = player_ptr->get_vip_level();
	if (map_template->TransportPrice.size() >= player_vip_level + 1)
	{
		e_money_type money_type = (e_money_type)map_template->TransportPrice[0];
		int32 money_num = map_template->TransportPrice[player_vip_level + 1];
		if (!player_ptr->can_cut_money(money_type, money_num))
		{
			CONSOLE_ERROR("money not enough money_type:{} money_num:{}", (int32)money_type, money_num);
			return;
		}
		player_ptr->cut_money(money_type, money_num, e_server_log_cut_money_map_mopping, map_id);
		if (map_template->FirstWinAwardItems.size() == map_template->FirstWinAwardItemsNum.size())
		{
			std::vector<s_item_template_info> item_list;
			for (size_t i = 0; i < map_template->FirstWinAwardItems.size(); i++)
			{
				item_list.push_back(s_item_template_info(map_template->FirstWinAwardItems[i], map_template->FirstWinAwardItemsNum[i], 1));
			}
			item_system::put_item_into_bag(player_ptr, item_list);
		}
		else
		{
			CONSOLE_ERROR("FirstWinAwardItems size error item_size:{} item_num:{}", map_template->FirstWinAwardItems.size(), map_template->FirstWinAwardItemsNum.size());
		}
	}
	else
	{
		CONSOLE_ERROR("TransportPrice size error size:{} vip_level:{}", map_template->TransportPrice.size(), player_vip_level);
		return;
	}
}
