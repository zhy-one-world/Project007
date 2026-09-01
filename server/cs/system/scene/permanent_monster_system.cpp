#include "permanent_monster_system.h"
#include "components/scene/permanent_monster_component.h"
#include "server_log.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "logic/scene/npc_spawner.h"
#include "logic/scene/map_cs.h"
#include "logic/npc.hpp"

using namespace faith;

void permanent_monster_system::start_up(Entity* map_ent, int32 spawn_point_id)
{
	ZoneScoped;
	auto npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_point_id);
	if (nullptr == npc_spawn_point_template_ptr)
	{
		CONSOLE_ERROR("npc_spawn_point_template_ptr is nullptr spawn_point_id:{}", spawn_point_id);
		return;
	}
	auto pm_cp = map_ent->add_component<permanent_monster_component>();
	auto npc_spawn_id = spawn_point_id;
	int32 next_spawn_id = 0;
	int32 map_template_id = npc_spawn_point_template_ptr->MapId;
	while (true)
	{
		npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_id);
		if (npc_spawn_point_template_ptr == nullptr)
		{
			CONSOLE_ERROR("npc_spawn_point_template_ptr is nullptr spawn_point_id:{}", npc_spawn_id);
			break;
		}
		next_spawn_id = npc_spawn_point_template_ptr->NextSpawnNpcId;
		if (next_spawn_id <= npc_spawn_id)
		{
			CONSOLE_INFO("spawn npc end npc_spawn_id:{} next_spawn_id:{}", npc_spawn_id, next_spawn_id);
			break;
		}
		if (npc_spawn_point_template_ptr->MapId != map_template_id)
		{
			CONSOLE_ERROR("spawn npc map error! npc_spawn_id:{} spawn_point_id:{} map_template_id:{} cur_map_id:{}", npc_spawn_id, spawn_point_id, map_template_id, npc_spawn_point_template_ptr->MapId);
			break;
		}
		
		if (npc_spawn_point_template_ptr->SpawnRate < 0 || npc_spawn_point_template_ptr->SpawnRate > 100)
		{
			CONSOLE_ERROR("spawn npc rate error! npc_spawn_id:{} map_id:{} SpawnRate:{}", npc_spawn_id, npc_spawn_point_template_ptr->MapId, npc_spawn_point_template_ptr->SpawnRate);
			break;
		}
		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
		if (npc_template_ptr == nullptr)
		{
			CONSOLE_ERROR("spawn npc is null! npc_spawn_id:{} npc_id:{}", npc_spawn_id, npc_spawn_point_template_ptr->TemplateId);
			break;
		}

		npc_spawn_id = next_spawn_id;

		if (npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(npc_template_ptr->SubType))
		{//世界Boss啥的归WS上的某个管理器处理		
			continue;
		}
		if (npc_spawn_point_template_ptr->IsCreateByMapCreate > 0 && is_in_refresh_time(npc_spawn_point_template_ptr))
		{
			int32 gen_rand_num = ((rand() % 100) + 1);
			if (gen_rand_num > npc_spawn_point_template_ptr->SpawnRate)
			{
				continue;
			}
			world_cs::spawn_npc(npc_spawn_id, map_ent);
		}
		else
		{
			int64 new_spawn_stamp = get_next_spawn_time(npc_spawn_point_template_ptr, false);
			if (new_spawn_stamp >= 0)
			{
				target_time_spawn_info temp_info;
				temp_info.spawn_template_id = npc_spawn_id;
				temp_info.spawn_template_ptr = npc_spawn_point_template_ptr;
				temp_info.target_spawn_time = new_spawn_stamp;
				add_target_time_monster(map_ent, temp_info);
			}
		}
	}
	return;
}
void permanent_monster_system::shut_down(Entity* entity)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto pm_cp = entity->get_component<permanent_monster_component>();
	if (false == pm_cp.isValid())
	{
		return;
	}
}
void permanent_monster_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	g_ecs->each<target_time_monster_component>([&](Entity* ent, ComponentHandle<target_time_monster_component> ttm_cp) -> bool {
		spawn_by_target_time_sec(new_time, ent, ttm_cp.get());
		return true;
	});
}
void permanent_monster_system::spawn_by_target_time_sec(int64 new_time, Entity* map_ent, target_time_monster_component& ttm_cp)
{
	ZoneScoped;
	for (auto it = ttm_cp.m_target_time_spawn_vec.begin(); it != ttm_cp.m_target_time_spawn_vec.end();)
	{
		auto temp_it = it++;
		if (new_time < (*temp_it).target_spawn_time)
		{
			break;
		}
		world_cs::spawn_npc((*temp_it).spawn_template_id, map_ent);
		ttm_cp.m_target_time_spawn_vec.erase(temp_it);
	}
	if (ttm_cp.m_target_time_spawn_vec.empty())
	{
		map_ent->remove_component<target_time_monster_component>();
	}
}
void permanent_monster_system::on_npc_delete(Entity* map_ent, const int32& unit_index, bool is_dead)
{
	ZoneScoped;
	auto& npc_ref = unit_man::get_npc(unit_index);

	int32 spawn_point_id = npc_ref.get_spawn_point_id();
	auto npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_point_id);
	if (npc_spawn_point_template_ptr)
	{
		int64 new_spawn_stamp = get_next_spawn_time(npc_spawn_point_template_ptr, is_dead);
		if (new_spawn_stamp >= 0)
		{
			target_time_spawn_info temp_info;
			temp_info.spawn_template_id = spawn_point_id;
			temp_info.spawn_template_ptr = npc_spawn_point_template_ptr;
			temp_info.target_spawn_time = new_spawn_stamp;
			add_target_time_monster(map_ent, temp_info);
		}
	}
}
void permanent_monster_system::add_target_time_monster(Entity* map_ent, const target_time_spawn_info& spawn_info)
{
	ZoneScoped;
	auto ttm_cp = map_ent->add_component<target_time_monster_component>();
	bool is_insert = false;
	for (int32 i = ttm_cp->m_target_time_spawn_vec.size() - 1; i >= 0 ; --i)
	{
		if (spawn_info.target_spawn_time > ttm_cp->m_target_time_spawn_vec[i].target_spawn_time)
		{
			ttm_cp->m_target_time_spawn_vec.insert(ttm_cp->m_target_time_spawn_vec.begin(), i, spawn_info);
			is_insert = true;
			break;
		}
	}
	if (false == is_insert)
	{
		ttm_cp->m_target_time_spawn_vec.insert(ttm_cp->m_target_time_spawn_vec.begin(), 0, spawn_info);
	}
}
bool permanent_monster_system::is_in_refresh_time(NpcSpawnPointTemplate* spawn_template_ptr)
{
	ZoneScoped;
	if (spawn_template_ptr == nullptr)
	{
		return false;
	}
	int32 cur_time_sec = time_helper::get_today_time_in_sec();

	switch (spawn_template_ptr->RespawnType)
	{
	case e_npc_respawn_type_duration_time:
	case e_npc_respawn_type_target_time:
		return true;
		break;
	case e_npc_respawn_type_in_certain_time:
	{
		if (spawn_template_ptr->RespawnPeriod.size() % e_respawn_period_tuple_max != 0)
		{
			return false;
		}
		else
		{
			int32 refresh_num = spawn_template_ptr->RespawnPeriod.size() / e_respawn_period_tuple_max;
			for (int32 i = 0; i < refresh_num; ++i)
			{
				int32 begin_time = spawn_template_ptr->RespawnPeriod[i * e_respawn_period_tuple_max + e_respawn_period_tuple_begin_time];
				int32 end_time = spawn_template_ptr->RespawnPeriod[i * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time];
				if (cur_time_sec >= begin_time
					&& cur_time_sec < end_time)
				{
					return true;
				}
			}
		}
	}
	break;
	default:
		break;
	}
	return false;
}

int64 permanent_monster_system::get_next_spawn_time(NpcSpawnPointTemplate* spawn_template_ptr, bool is_be_kill)
{
	ZoneScoped;
	int64 return_value = -1;

	if (spawn_template_ptr == nullptr)
	{
		return return_value;
	}

	int32 cur_time_sec = time_helper::get_today_time_in_sec();

	switch (spawn_template_ptr->RespawnType)
	{
	case e_npc_respawn_type_duration_time:
	{
		if (false == is_be_kill)
		{
			break;
		}
		return_value = init_unit::get_end_time(spawn_template_ptr->RespawnTime);
	}
	break;
	case e_npc_respawn_type_in_certain_time:
	{
		int32 next_time_index = get_next_target_time_spawn_index(spawn_template_ptr);
		if (next_time_index >= 0
			&& next_time_index * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time < spawn_template_ptr->RespawnPeriod.size())
		{
			int32 begin_time = spawn_template_ptr->RespawnPeriod[next_time_index * e_respawn_period_tuple_max + e_respawn_period_tuple_begin_time];
			int32 end_time = spawn_template_ptr->RespawnPeriod[next_time_index * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time];

			if (cur_time_sec > begin_time
				&& cur_time_sec < end_time)
			{
				return_value = init_unit::get_end_time(spawn_template_ptr->RespawnTime);
			}
			else
			{
				if (cur_time_sec < begin_time)
				{
					return_value = init_unit::get_end_time(begin_time - cur_time_sec);
				}
				else
				{
					return_value = init_unit::get_end_time(begin_time - cur_time_sec + day_time_second);
				}
			}
		}
	}
	break;
	default:
		break;
	}
	return return_value;
}

int32 permanent_monster_system::get_next_target_time_spawn_index(NpcSpawnPointTemplate* spawn_template_ptr)
{
	ZoneScoped;
	int32 return_value = -1;
	if (spawn_template_ptr == nullptr)
	{
		return return_value;
	}
	int32 cur_time_sec = time_helper::get_today_time_in_sec();

	switch (spawn_template_ptr->RespawnType)
	{
	case e_npc_respawn_type_duration_time:
	{
		return_value = -1;
	}
	break;
	case e_npc_respawn_type_in_certain_time:
	{
		if (spawn_template_ptr->RespawnPeriod.size() % e_respawn_period_tuple_max != 0)
		{
			return_value = -1;
		}
		else
		{
			int32 refresh_num = spawn_template_ptr->RespawnPeriod.size() / e_respawn_period_tuple_max;
			for (int32 i = 0; i < refresh_num; ++i)
			{
				if (cur_time_sec + spawn_template_ptr->RespawnTime < spawn_template_ptr->RespawnPeriod[i * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time])
				{
					return_value = i;
					break;
				}
			}
			if (return_value == -1)
			{
				return_value = 0;
			}
		}
	}
	break;
	default:
		break;
	}
	return return_value;
}