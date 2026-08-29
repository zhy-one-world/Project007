#include "base_map_system.h"
#include "box_map_system.h"
#include "components/npc/box_award_component.h"
#include "components/scene/base_map_component.h"
#include "components/scene/box_map_component.h"
#include "internet/map.pb.h"
#include "internet/net.pb.h"
#include "logic/map_record.h"
#include "logic/npc.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "npc_refrush_system.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/npc/treasure_pic_system.h"
#include "template/template_manager.h"
#include "utility/random.h"

using namespace faith;

void box_map_system::start_up(Entity* map_ent, MapTemplate* map_template)
{
	ZoneScoped;
	if (map_template->Type != e_map_type_big_map)
	{
		return;
	}

	auto box_map_cp = map_ent->add_component<box_map_component>();
	box_map_cp->m_state = e_box_map_state_none;
	box_map_cp->m_box_map_template = GET_TEMPLATE(BoxMapTemplate, map_template->LogicId);
	box_map_cp->m_award_rate = 1000000 / box_map_cp->m_box_map_template->AwardMaxCount;
	npc_refrush_system::start_up(map_ent);
}
void  box_map_system::shut_down(Entity* map_ent)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	box_map_cp->m_state = e_box_map_state_close;
	map_ent->remove_component<box_map_component>();
}

void box_map_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	//g_ecs->each<box_map_refrush_component>([&](Entity* ent, ComponentHandle<box_map_refrush_component> amb_cp) -> bool {
	//	if (amb_cp->m_be_call == false)
	//	{
	//		//refresh_boss(ent);
	//		return true;
	//	}
	//	return true;
	//});
}
void box_map_system::on_player_save(Entity* map_ent, map_s2s_box_map_info* msg)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}

	auto base_map_cp = map_ent->get_component<base_map_component>();
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc = unit_man::get_npc(it);
		if (npc.get_unit_type() == e_unit_type_award_mark && npc.get_unit_sub_type() != e_award_mark_type_treasure_pic)
		{
			auto point_msg = msg->add_npc_point_array();
			point_msg->set_npc_id(npc.get_npc_template_id());
			point_msg->set_spawn_point_id(npc.get_spawn_point_id());
			if (npc.m_box_award_component->m_can_get_award)
			{
				msg->set_data_array(e_role_box_map_next_award, npc.get_spawn_point_id());
			}
		}
	}
}
void box_map_system::on_player_load(Entity* map_ent, map_s2s_box_map_info* msg)
{

}

void box_map_system::on_player_add(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	box_map_cp->m_state = e_box_map_state_player_join;
	return;
}

void box_map_system::on_player_delete(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
}

void box_map_system::on_player_load_data_over(Entity* map_ent, player* player_ptr)
{
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
}

void box_map_system::on_player_enter_scene(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	player_ptr->get_map_record_mgr()->create_map_record(base_map_cp->m_map_id);

	box_map_cp->m_state = e_box_map_state_select_npc;
	refresh_monster(map_ent, player_ptr);
	refresh_award(map_ent, player_ptr);
	map_s2c_kill_award_npc_end msg;
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc = unit_man::get_npc(it);
		if (npc.is_valid() && npc.m_box_award_component)
		{
			msg.add_npc_guid(npc.get_unit_guid().server_64);
		}
	}
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_kill_award_npc);
}

void box_map_system::on_player_leave_scene(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	if (box_map_cp->m_boss_list.empty() == false)
	{
		auto boss_list = box_map_cp->m_boss_list;

		for (auto& it : boss_list)
		{
			unit_man::remove_npc(it, false);
		}
		box_map_cp->m_boss_list.clear();
		player_ptr->set_player_relive();
		player_ptr->transfer_born();
	}
}
void box_map_system::on_player_dead(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto boss_list = box_map_cp->m_boss_list;

	for (auto& it : boss_list)
	{
		unit_man::remove_npc(it, false);
	}
	box_map_cp->m_boss_list.clear();
	map_s2c_map_anim_end res;
	res.set_next_map_id(base_map_cp->m_map_id);
	player_ptr->send_message_to_self(&res, e_msgindex_s2c_map_anim_end);
}

void box_map_system::on_player_reconnect(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
}

void box_map_system::on_npc_add(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
}

void box_map_system::on_npc_dead(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
}
void box_map_system::on_npc_delete(Entity* map_ent, npc* npc_ptr, bool is_dead)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	if (npc_ptr->get_unit_type() == e_unit_type_monster && npc_ptr->get_unit_sub_type() == e_monster_type_award_boss)
	{
		box_map_cp->m_boss_list.erase(npc_ptr->get_array_index());
		if (box_map_cp->m_boss_list.empty() && is_dead)
		{
			map_settlement(map_ent);
			return;
		}
	}
	if (npc_ptr->get_unit_type() == e_unit_type_award_mark)
	{
		bool have_award = false;
		for (auto npc_it : base_map_cp->m_npc_list)
		{
			auto& npc = unit_man::get_npc(npc_it);
			if (npc.get_unit_type() == e_unit_type_award_mark && npc.m_box_award_component)
			{
				have_award = true;
				break;
			}
		}
		if (have_award == false)
		{
			auto it = base_map_cp->m_player_list.begin();
			auto& player_ref = unit_man::get_player(*it);
			refresh_award(map_ent, &player_ref);
			map_s2c_get_npc_award_end msg;
			msg.set_award_type(e_award_mark_type_special);
			player_ref.send_message_to_self(&msg, e_msgindex_c2s_get_npc_award_end);
		}
	}
}
void box_map_system::on_unit_be_damage(Entity* map_ent, const int32& attack_index, const int32& be_attack_index, double damage_value)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
}

void box_map_system::begin_kill_boss(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_ERROR("box_map_cp is null");
		return;
	}
	if (box_map_cp->m_state != e_box_map_state_select_npc)
	{
		CONSOLE_ERROR("box_map_cp->m_state:{}", int32(box_map_cp->m_state));
		return;
	}

	if (player_ptr->can_cut_money(e_money_type_boss_progress, box_map_cp->m_box_map_template->SearchRefrushBossValue) == false)
	{
		CONSOLE_ERROR("have_progress:{} need_count:{}", player_ptr->get_money_data(e_money_type_boss_progress), box_map_cp->m_box_map_template->SearchRefrushBossValue);
		return;
	}
	if (box_map_cp->m_box_map_template->BossFightId.size() < 2)
	{
		CONSOLE_ERROR("BossFightId size is less than 2");
		return;
	}
	auto npc_id = box_map_cp->m_box_map_template->BossFightId[0];
	auto player_pos_id = box_map_cp->m_box_map_template->BossFightId[1];
	auto npc = world_cs::spawn_npc(npc_id, map_ent, player_ptr);
	if (npc)
	{
		box_map_cp->m_boss_list.insert(npc->get_array_index());
		npc->get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_common_monster);
		npc->get_pawn_att().send_base_att_one(e_base_att_info_community_type);

		auto spawn_template = GET_TEMPLATE(NpcSpawnPointTemplate, player_pos_id);
		if (spawn_template)
		{
			s_map_pos pos;
			pos.unit_location.x = spawn_template->PosX;
			pos.unit_location.y = spawn_template->PosY;
			pos.unit_location.z = spawn_template->PosZ;
			pos.unit_rotation.yaw = spawn_template->Yaw;
			pos.unit_rotation.pitch = spawn_template->Pitch;
			pos.unit_rotation.roll = spawn_template->Roll;
			player_ptr->transfer_local_pos(pos);
		}
	}
}
void box_map_system::refresh_monster(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_ERROR("box_map_cp is null");
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	for (auto& it : box_map_cp->m_box_map_template->MonsterBrushId)
	{
		world_cs::spawn_npc(it, map_ent);
	}	
	for (auto& it : box_map_cp->m_box_map_template->BossBrushId)
	{
		auto npc = world_cs::spawn_npc(it, map_ent, player_ptr);
		if (npc)
		{
			npc->get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_common_friend);
			npc->get_pawn_att().send_base_att_one(e_base_att_info_community_type);
		}
	}
}

void box_map_system::refresh_award(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_ERROR("box_map_cp is null");
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto box_map_info = map_record_ptr->get_map_record_data<s_box_map_info>();
	if (nullptr == box_map_info)
	{
		CONSOLE_ERROR("box_map_info is null");
		return;
	}
	auto box_map_template = box_map_cp->m_box_map_template;

	if (box_map_info->m_npc_point.empty() == false)
	{
		for (auto& it : box_map_info->m_npc_point)
		{
			auto npc_ptr = refresh_big_award(map_ent, player_ptr, it.m_spawn_point_id, it.m_npc_id);
			if (npc_ptr)
			{
				if (it.m_spawn_point_id == map_record_ptr->get_data_info(e_role_box_map_next_award))
				{
					npc_ptr->m_box_award_component->m_can_get_award = true;
				}
				if (it.m_npc_id == box_map_template->SpecialId)
				{
					npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_special;
				}
				else if (std::find(box_map_template->BigAward.begin(), box_map_template->BigAward.end(), it.m_npc_id) != box_map_template->BigAward.end())
				{
					npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_big;
				}
				else if (std::find(box_map_template->MiddleAward.begin(), box_map_template->MiddleAward.end(), it.m_npc_id) != box_map_template->MiddleAward.end())
				{
					npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_middle;
				}
				else
				{
					npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_small;
				}
			}
		}
		box_map_info->m_npc_point.clear();
	}
	else
	{
		int32 refrush_count = box_map_template->AwardMaxCount;
		//Ë¢´ó½±
		if (box_map_template->BigAward.empty() == false)
		{
			auto spawn_id = get_born_list(map_ent);
			auto random_index = random_gen::get_random(0, box_map_template->BigAward.size() - 1);
			auto npc_ptr = refresh_big_award(map_ent, player_ptr, spawn_id, box_map_template->BigAward[random_index]);
			if (npc_ptr)
			{
				npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_big;
				refrush_count--;
			}
		}
		//Ë¢ÖÐ½±
		if (box_map_template->MiddleAward.empty() == false)
		{
			for (int32 i = 0; i < box_map_template->MiddleAwardCount; ++i)
			{
				auto spawn_id = get_born_list(map_ent);
				auto random_index = random_gen::get_random(0, box_map_template->MiddleAward.size() - 1);
				auto npc_ptr = refresh_big_award(map_ent, player_ptr, spawn_id, box_map_template->MiddleAward[random_index]);
				if (npc_ptr)
				{
					npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_middle;
					refrush_count--;
				}
			}
		}
		//Ë¢Ð¡½±
		for (int32 i = 0; i < refrush_count; ++i)
		{
			auto spawn_id = get_born_list(map_ent);
			auto npc_ptr = world_cs::spawn_npc(spawn_id, map_ent, player_ptr);
			if (npc_ptr)
			{
				npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_small;
			}
		}
	}
}
int64 box_map_system::get_next_award_npc(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_INFO("box_map_cp is null");
		return 0;
	}

	if (box_map_cp->m_state != e_box_map_state_select_npc)
	{
		CONSOLE_ERROR("box_map_cp->m_state:{}", int32(box_map_cp->m_state));
		return 0;
	}

	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto now_tick = utility::get_tick_count() / 1000 - g_role_box_search_time_cut;
	if (now_tick < map_record_ptr->get_data_info(e_role_box_map_search_time))
	{
		CONSOLE_ERROR("search time is not enough");
		return 0;
	}
	map_record_ptr->set_data_info(e_role_box_map_search_time, now_tick + g_role_box_search_time_interval);

	if (player_ptr->can_cut_money(e_money_type_energy, 1) == false)
	{
		CONSOLE_ERROR("player_ptr->can_cut_money(e_money_type_energy, 1) == false");
		return 0;
	}
	box_map_cp->m_state = e_box_map_state_wait_select_end;
	player_ptr->cut_money(e_money_type_energy, 1, e_server_log_cut_money_get_award, 0);
	player_ptr->set_logic_data(e_role_logic_info_search_value, player_ptr->get_logic_data(e_role_logic_info_search_value) + 1);
	player_ptr->get_mission_mgr().target_check(e_mission_end_type_search_count);

	auto search_random = random_gen::get_random(1, 1000000);
	int32 random_sum = 0;
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc_ref = unit_man::get_npc(it);
		if (npc_ref.m_box_award_component && npc_ref.m_box_award_component->m_can_get_award)
		{
			CONSOLE_INFO("npc_guid:{} npc_index:{} spawn_id:{}", npc_ref.get_unit_guid().server_64, npc_ref.get_array_index(), npc_ref.get_spawn_point_id());
			return npc_ref.get_unit_guid().server_64;
		}
	}

	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc_ref = unit_man::get_npc(it);
		if (npc_ref.m_box_award_component && npc_ref.m_box_award_component->m_award_level == e_award_mark_level_special)
		{
			random_sum += map_record_ptr->get_data_info(e_role_box_map_special_open_compensate) + box_map_cp->m_award_rate;
			if (search_random <= random_sum)
			{
				map_record_ptr->set_data_info(e_role_box_map_middle_open_compensate, 0);
				map_record_ptr->set_data_info(e_role_box_map_big_open_compensate, 0);
				map_record_ptr->set_data_info(e_role_box_map_special_refrush_compensate, 0);
				map_record_ptr->set_data_info(e_role_box_map_special_open_compensate, 0);
				npc_ref.m_box_award_component->m_can_get_award = true;
				CONSOLE_INFO("npc_guid:{} npc_index:{} spawn_id:{}", npc_ref.get_unit_guid().server_64, npc_ref.get_array_index(), npc_ref.get_spawn_point_id());
				return npc_ref.get_unit_guid().server_64;
			}
		}
		
	}
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc_ref = unit_man::get_npc(it);
		if (npc_ref.m_box_award_component && npc_ref.m_box_award_component->m_award_level == e_award_mark_level_big)
		{
			random_sum += map_record_ptr->get_data_info(e_role_box_map_big_open_compensate) + box_map_cp->m_award_rate;
			if (search_random <= random_sum)
			{
				map_record_ptr->set_data_info(e_role_box_map_big_open_compensate, 0);
				npc_ref.m_box_award_component->m_can_get_award = true;
				CONSOLE_INFO("npc_guid:{} npc_index:{} spawn_id:{}", npc_ref.get_unit_guid().server_64, npc_ref.get_array_index(), npc_ref.get_spawn_point_id());
				return npc_ref.get_unit_guid().server_64;
			}
		}
	}
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc_ref = unit_man::get_npc(it);
		if (npc_ref.m_box_award_component && npc_ref.m_box_award_component->m_award_level == e_award_mark_level_middle)
		{
			random_sum += map_record_ptr->get_data_info(e_role_box_map_middle_open_compensate) + box_map_cp->m_award_rate;
			if (search_random <= random_sum)
			{
				map_record_ptr->set_data_info(e_role_box_map_middle_open_compensate, 0);
				map_record_ptr->add_data_info(e_role_box_map_special_open_compensate, box_map_cp->m_box_map_template->SpecialBoxOpen);
				npc_ref.m_box_award_component->m_can_get_award = true;
				CONSOLE_INFO("npc_guid:{} npc_index:{} spawn_id:{}", npc_ref.get_unit_guid().server_64, npc_ref.get_array_index(), npc_ref.get_spawn_point_id());
				return npc_ref.get_unit_guid().server_64;
			}
		}
	}	
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc_ref = unit_man::get_npc(it);
		if (npc_ref.m_box_award_component && npc_ref.m_box_award_component->m_award_level == e_award_mark_level_small)
		{
			random_sum += box_map_cp->m_award_rate;
			if (search_random <= random_sum)
			{
				map_record_ptr->add_data_info(e_role_box_map_middle_open_compensate, box_map_cp->m_box_map_template->MiddleBoxOpen);
				npc_ref.m_box_award_component->m_can_get_award = true;
				CONSOLE_INFO("npc_guid:{} npc_index:{} spawn_id:{}", npc_ref.get_unit_guid().server_64, npc_ref.get_array_index(), npc_ref.get_spawn_point_id());
				return npc_ref.get_unit_guid().server_64;
			}
		}
	}
	CONSOLE_ERROR("random_sum:{} search_random:{}", random_sum, search_random);
	return 0;
}
void box_map_system::refresh_nomal_award(Entity* map_ent, box_map_component& box_map_cp, player* player_ptr)
{
	auto spawn_id = get_born_list(map_ent);
	if (spawn_id <= 0)
	{
		CONSOLE_ERROR("spawn_id is zero");
		return;
	}	
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	auto change_random = random_gen::get_random(1, 1000000);
	//int32 middler_refrush_random = 0;
	//if (map_record_ptr->get_data_info(e_role_box_map_middle_refrush_compensate) > box_map_cp.m_box_map_template->MiddleBoxRefrush[0])
	//{
	//	middler_refrush_random = box_map_cp.m_box_map_template->MiddleBoxRefrush[1] + box_map_cp.m_box_map_template->MiddleBoxRefrush[2] * (map_record_ptr->get_data_info(e_role_box_map_middle_refrush_compensate) - box_map_cp.m_box_map_template->MiddleBoxRefrush[0]);
	//}
	//if (change_random < middler_refrush_random)
	//{
	//	map_record_ptr->set_data_info(e_role_box_map_middle_refrush_compensate, 0);
	//	auto change_random = random_gen::get_random(1, 1000000);
	//	auto sum_random = 0;
	//	for (size_t i = 0; i < box_map_cp.m_box_map_template->SpecialAward.size(); i = i + 2)
	//	{
	//		sum_random += box_map_cp.m_box_map_template->SpecialAward[i + 1];
	//		if (change_random <= sum_random)
	//		{
	//			refresh_big_award(map_ent, player_ptr, spawn_id, box_map_cp.m_box_map_template->SpecialAward[i]);
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	world_cs::spawn_npc(spawn_id, map_ent, player_ptr);
	//	map_record_ptr->add_data_info(e_role_box_map_middle_refrush_compensate, 1);
	//}
}
npc* box_map_system::refresh_big_award(Entity* map_ent, player* player_ptr, int32 spawn_id, int32 npc_id)
{
	ZoneScoped;
	auto npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_id);
	if (npc_spawn_point_template_ptr == nullptr)
	{
		CONSOLE_ERROR("npc_spawn_point_template_ptr is nullptr npc_spawn_template_id = {}", spawn_id);
		return nullptr;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	fvector location(npc_spawn_point_template_ptr->PosX, npc_spawn_point_template_ptr->PosY, npc_spawn_point_template_ptr->PosZ);
	frotator rotation(npc_spawn_point_template_ptr->Roll, npc_spawn_point_template_ptr->Pitch, npc_spawn_point_template_ptr->Yaw);
	s_map_pos bornPos(location, rotation);
	auto npc_prt = world_cs::spawn_npc(npc_id, bornPos, map_ent, player_ptr, spawn_id);
	return npc_prt;
}
int32 box_map_system::get_born_list(Entity* map_ent)
{
	ZoneScoped;
	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto box_map_cp = map_ent->get_component<box_map_component>();

	std::vector<int32> born_point_array;
	std::set<int32> old_born_point;

	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc_ref = unit_man::get_npc(it);
		old_born_point.insert(npc_ref.get_spawn_point_id());
	}
	for (auto& it : box_map_cp->m_box_map_template->AwardBrushId)
	{
		auto old_it = old_born_point.find(it);
		if (old_it != old_born_point.end())
		{
			continue;
		}
		born_point_array.push_back(it);
	}
	if (born_point_array.empty())
	{
		CONSOLE_ERROR("born_point_array is empty");
		return  box_map_cp->m_box_map_template->AwardBrushId[0];
	}
	auto max_index = random_gen::get_random(0, born_point_array.size() - 1);
	return born_point_array[max_index];
}
void box_map_system::get_npc_award(Entity* map_ent, player* player_ptr, e_award_mark_type mark_type, int32 spawn_id)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_INFO("box_map_cp is null");
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();

	for (auto npc_it : base_map_cp->m_npc_list)
	{
		auto& npc = unit_man::get_npc(npc_it);
		if (npc.get_unit_type() == e_unit_type_award_mark && npc.m_box_award_component)
		{
			item_system::put_item_into_bag(player_ptr, npc.m_box_award_component->m_drop_item);
		}
		else
		{
			CONSOLE_ERROR("box_award_cp is null npc_index:{} npc_id:{}", npc_it, npc.get_npc_template_id());
		}
	}
}
npc* box_map_system::get_npc_by_level(Entity* map_ent, e_award_mark_level award_level)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_INFO("box_map_cp is null");
		return nullptr;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();

	for (auto npc_it : base_map_cp->m_npc_list)
	{
		auto& npc = unit_man::get_npc(npc_it);
		if (npc.get_unit_type() == e_unit_type_award_mark && npc.m_box_award_component && npc.m_box_award_component->m_award_level == award_level)
		{
			return &npc;
		}
	}
	return nullptr;
}
void box_map_system::kill_award_npc(Entity* map_ent, player* player_ptr, npc* npc_ptr)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_ERROR("box_map_cp is null");
		return;
	}
	if (box_map_cp->m_state != e_box_map_state_wait_select_end)
	{
		CONSOLE_ERROR("box_map_cp->m_state:{}", int32(box_map_cp->m_state));
		return;
	}
	auto box_award_cp = npc_ptr->m_box_award_component;
	if (nullptr == box_award_cp)
	{
		CONSOLE_ERROR("box_award_cp is null");
		return;
	}
	if (npc_ptr->get_unit_sub_type() != e_award_mark_type_transfer)
	{
		box_map_cp->m_state = e_box_map_state_select_npc;
	}

	auto npc_index = npc_ptr->get_array_index();
	auto award_level = box_award_cp->m_award_level;

	unit_man::remove_npc(npc_index, false);

	map_s2c_kill_award_npc_end msg;
	switch (award_level)
	{
	case faith::e_award_mark_level_small:
	{
		auto base_map_cp = map_ent->get_component<base_map_component>();
		int32 middle_num = 0;
		for (auto& it : base_map_cp->m_npc_list)
		{
			auto& npc_ref = unit_man::get_npc(it);
			if (npc_ref.is_valid() && npc_ref.m_box_award_component && npc_ref.m_box_award_component->m_award_level == e_award_mark_level_middle)
			{
				middle_num++;
			}
		}
		auto middle_random = box_map_cp->m_box_map_template->MiddleAwardRefrush[middle_num];
		auto random_num = random_gen::get_random(1, 1000000);
		if (random_num > middle_random)
		{
			auto born_id = get_born_list(map_ent);
			auto new_npc_ptr = world_cs::spawn_npc(born_id, map_ent, player_ptr);
			new_npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_small;
			msg.add_npc_guid(new_npc_ptr->get_unit_guid().server_64);
		}
		else
		{
			auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
			auto special_random_num = random_gen::get_random(1, 1000000);
			auto special_open_count = map_record_ptr->get_data_info(e_role_box_map_special_refrush_compensate);
			auto special_random = box_map_cp->m_box_map_template->SpecialBoxRefrush[1] + map_record_ptr->get_data_info(e_role_box_map_special_open_compensate);
			if (special_open_count >= box_map_cp->m_box_map_template->SpecialBoxRefrush[0] && special_random <= special_random_num)
			{
				auto born_id = get_born_list(map_ent);
				auto new_npc_ptr = refresh_big_award(map_ent, player_ptr, born_id, box_map_cp->m_box_map_template->SpecialId);
				new_npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_special;
				msg.add_npc_guid(new_npc_ptr->get_unit_guid().server_64);
			}
			else
			{
				auto born_id = get_born_list(map_ent);
				auto npc_id = box_map_cp->m_box_map_template->MiddleAward[random_gen::get_random(0, box_map_cp->m_box_map_template->MiddleAward.size() - 1)];
				auto new_npc_ptr = refresh_big_award(map_ent, player_ptr, born_id, npc_id);
				new_npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_middle;
				msg.add_npc_guid(new_npc_ptr->get_unit_guid().server_64);
				map_record_ptr->add_data_info(e_role_box_map_special_refrush_compensate, 1);
				if (map_record_ptr->get_data_info(e_role_box_map_special_refrush_compensate) > box_map_cp->m_box_map_template->SpecialBoxRefrush[0])
				{
					map_record_ptr->add_data_info(e_role_box_map_special_open_compensate, box_map_cp->m_box_map_template->SpecialBoxRefrush[2]);
				}
			}
		}
	}
		break;
	case faith::e_award_mark_level_middle:
	{
		auto born_id = get_born_list(map_ent);
		auto new_npc_ptr = world_cs::spawn_npc(born_id, map_ent, player_ptr);
		new_npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_small;
		msg.add_npc_guid(new_npc_ptr->get_unit_guid().server_64);
	}
		break;
	case faith::e_award_mark_level_big:
	{
		auto born_id = get_born_list(map_ent);
		auto npc_id = box_map_cp->m_box_map_template->BigAward[random_gen::get_random(0, box_map_cp->m_box_map_template->BigAward.size() - 1)];
		auto new_npc_ptr = refresh_big_award(map_ent, player_ptr, born_id, npc_id);
		new_npc_ptr->m_box_award_component->m_award_level = e_award_mark_level_big;
		msg.add_npc_guid(new_npc_ptr->get_unit_guid().server_64);
	}
		break;
	case faith::e_award_mark_level_special:
	{
		auto base_map_cp = map_ent->get_component<base_map_component>();
		auto npc_list = base_map_cp->m_npc_list;
		for (auto& it : npc_list)
		{
			auto& npc_ref = unit_man::get_npc(it);
			if (npc_ref.is_valid() && npc_ref.m_box_award_component)
			{
				unit_man::remove_npc(it, false);
			}
		}
		refresh_award(map_ent, player_ptr);
		for (auto& it : base_map_cp->m_npc_list)
		{
			auto& npc_ref = unit_man::get_npc(it);
			if (npc_ref.is_valid() && npc_ref.m_box_award_component)
			{
				msg.add_npc_guid(npc_ref.get_unit_guid().server_64);
			}
		}
	}
		break;
	default:
	{
		CONSOLE_ERROR("award_level:{}", int32(award_level));
	}
		break;
	}
	player_ptr->send_message_to_self(&msg, e_msgindex_s2c_kill_award_npc);
}

void box_map_system::clear_multiple_treasure(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_INFO("box_map_cp is null");
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	box_map_cp->m_state = e_box_map_state_select_npc;
	player_ptr->cut_money(e_money_type_treasure_pic, box_map_cp->m_box_map_template->TreasureMaxCount, e_server_log_cut_money_treasure_pic, base_map_cp->m_map_id);
}
int64 box_map_system::open_treasure_pic(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_INFO("box_map_cp is null");
		return 0;
	}
	if (box_map_cp->m_state != e_box_map_state_select_npc)
	{
		CONSOLE_ERROR("box_map_cp->m_state:{}", int32(box_map_cp->m_state));
		return 0;
	}

	if (player_ptr->can_cut_money(e_money_type_treasure_pic, box_map_cp->m_box_map_template->TreasureMaxCount) == false)
	{
		CONSOLE_ERROR("money not enough need_num:{} have_num:{}", box_map_cp->m_box_map_template->TreasureMaxCount, player_ptr->get_money_data(e_money_type_treasure_pic));
		return 0;
	}
	auto spawn_id = get_born_list(map_ent);
	if (spawn_id <= 0)
	{
		CONSOLE_ERROR("spawn_id is zero");
		return 0;
	}
	box_map_cp->m_state = e_box_map_state_wait_select_end;
	auto npc_ptr = refresh_big_award(map_ent, player_ptr, spawn_id, box_map_cp->m_box_map_template->TreasureNpcId);
	if (npc_ptr)
	{
		treasure_pic_system::start_up(player_ptr, npc_ptr);
		return npc_ptr->get_unit_guid().server_64;
	}
	else
	{
		CONSOLE_ERROR("create npc is null npc_id:{} spawn_id:{}", box_map_cp->m_box_map_template->TreasureNpcId, spawn_id);
		return 0;
	}
}
void box_map_system::open_new_map(Entity* map_ent, player* player_ptr)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	if (box_map_cp->m_state != e_box_map_state_end_map)
	{
		CONSOLE_ERROR("box_map_cp->m_state:{}", int32(box_map_cp->m_state));
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();

	auto next_map_template = GET_TEMPLATE(MapTemplate, base_map_cp->m_map_template->NextMapId);
	if (nullptr == next_map_template)
	{
		CONSOLE_ERROR("next_map_template is null id:{}", base_map_cp->m_map_template->NextMapId);
		return;
	}
	auto map_record_ptr = player_ptr->get_map_record_mgr()->get_map_record(base_map_cp->m_map_id);
	map_record_ptr->clear_data_info();
	player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_box_map_level, base_map_cp->m_map_template->MapLevel);

	player_ptr->set_player_relive();
	player_ptr->get_mission_mgr().target_check(e_mission_end_type_map, base_map_cp->m_map_id);
	player_ptr->set_money_data(e_money_type_boss_progress, 0);
	player_ptr->send_money_one(e_money_type_boss_progress);
	shut_down(map_ent);
	npc_refrush_system::shut_down(map_ent);

	if (next_map_template->WorldID != base_map_cp->m_map_template->WorldID || next_map_template->IslandId != base_map_cp->m_map_template->IslandId)
	{
		player_ptr->get_transfer_control().teleport_by_template(base_map_cp->m_map_template->NextMapId, 0, 0, empty_guid_64, 0);
	}
	else
	{
		base_map_system::map_settlement(map_ent, next_map_template);
		start_up(map_ent, next_map_template);
		auto new_box_map_cp = map_ent->get_component<box_map_component>();
		new_box_map_cp->m_state = e_box_map_state_select_npc;

		player_ptr->set_unit_info(e_role_info_main_map_id, base_map_cp->m_map_template->NextMapId);
		player_ptr->set_unit_info(e_role_info_move_map_id, base_map_cp->m_map_template->NextMapId);
		player_ptr->transfer_born();

		refresh_monster(map_ent, player_ptr);
		refresh_award(map_ent, player_ptr);
		map_s2c_map_anim_end res;
		res.set_next_map_id(next_map_template->attribute_id);
		player_ptr->send_message_to_self(&res, e_msgindex_s2c_map_anim_end);
		map_s2c_get_npc_award_end msg;
		player_ptr->send_message_to_self(&msg, e_msgindex_c2s_get_npc_award_end);
	}
}
void box_map_system::map_settlement(Entity* map_ent)
{
	ZoneScoped;
	auto box_map_cp = map_ent->get_component<box_map_component>();
	if (false == box_map_cp.isValid())
	{
		CONSOLE_INFO("box_map_cp is null");
		return;
	}
	box_map_cp->m_state = e_box_map_state_end_map;

	auto base_map_cp = map_ent->get_component<base_map_component>();
	auto next_map_id = base_map_cp->m_map_template->NextMapId;

	auto next_map_template = GET_TEMPLATE(MapTemplate, next_map_id);
	if (nullptr == next_map_template)
	{
		CONSOLE_ERROR("next_map_template is null id:{}", next_map_id);
		return;
	}

	map_s2c_map_settlement msg;
	msg.set_next_map_id(next_map_id);
	base_map_system::send_map_message(map_ent, &msg, e_msgindex_s2c_map_settlement);
}
