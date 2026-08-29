#include "components/scene/npc_refrush_component.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "logic/npc.hpp"
#include "npc_refrush_system.h"
#include "server_log.hpp"
#include "template/template_manager.h"

using namespace faith;

void npc_refrush_system::start_up(Entity* map_ent)
{
	ZoneScoped;
	map_ent->add_component<npc_refrush_component>();
}
void  npc_refrush_system::shut_down(Entity* map_ent)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto box_map_cp = map_ent->get_component<npc_refrush_component>();
	if (false == box_map_cp.isValid())
	{
		return;
	}
	map_ent->remove_component<npc_refrush_component>();
}

void npc_refrush_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	g_ecs->each<npc_refrush_component>([&](Entity* map_ent, ComponentHandle<npc_refrush_component> nr_cp) -> bool {
		refresh_npc(new_time, map_ent, nr_cp.get());
		return true;
	});
}
void npc_refrush_system::on_npc_dead(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;

	auto nr_cp = map_ent->get_component<npc_refrush_component>();
	if (false == nr_cp.isValid())
	{
		return;
	}
	auto npc_map_point = npc_ptr->get_spawn_point_id();
	auto npc_spawn_template = GET_TEMPLATE(NpcSpawnPointTemplate, npc_map_point);
	if (nullptr == npc_spawn_template)
	{
		CONSOLE_ERROR("npc_spawn_template is null npc_map_point:{}", npc_map_point);
		return;
	}
	if (npc_spawn_template->RespawnType == e_npc_respawn_type_non)
	{
		return;
	}
	auto now_time = utility::get_tick_count();
	now_time += npc_spawn_template->RespawnTime * 1000;
	if (nr_cp->m_refrush_list.empty())
	{
		nr_cp->m_refrush_list.push_back({ npc_map_point, now_time });
	}
	else
	{
		bool is_insert = false;
		for (auto it = nr_cp->m_refrush_list.begin(); it != nr_cp->m_refrush_list.end(); ++it)
		{
			if (now_time < it->m_next_refrush_time)
			{
				nr_cp->m_refrush_list.insert(it, { npc_map_point, now_time });
				is_insert = true;
				break;
			}
		}
		if (false == is_insert)
		{
			nr_cp->m_refrush_list.push_back({ npc_map_point, now_time });
		}
	}
}

void npc_refrush_system::on_map_settlement(Entity* map_ent)
{
	ZoneScoped;
	auto nr_cp = map_ent->get_component<npc_refrush_component>();
	if (false == nr_cp.isValid())
	{
		return;
	}
	nr_cp->m_refrush_list.clear();

}
void npc_refrush_system::refresh_npc(int64 new_time, Entity* map_ent, npc_refrush_component& nr_cp)
{
	ZoneScoped;
	if (nr_cp.m_refrush_list.empty())
	{
		return;
	}
	for (auto it = nr_cp.m_refrush_list.begin(); it != nr_cp.m_refrush_list.end();)
	{
		auto temp_it = it++;
		if (temp_it->m_next_refrush_time <= new_time)
		{
			auto npc_point_template = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->m_npc_point_id);
			if (nullptr == npc_point_template)
			{
				break;
			}

			s_map_pos born_pos;
			born_pos.unit_location.x = npc_point_template->PosX;
			born_pos.unit_location.y = npc_point_template->PosY;
			born_pos.unit_location.z = npc_point_template->PosZ;
			born_pos.unit_rotation.pitch = npc_point_template->Pitch;
			born_pos.unit_rotation.roll = npc_point_template->Roll;
			born_pos.unit_rotation.yaw = npc_point_template->Yaw;
			world_cs::spawn_npc(npc_point_template->TemplateId, born_pos, map_ent, 0, temp_it->m_npc_point_id);
			nr_cp.m_refrush_list.erase(temp_it);
		}
		else
		{
			break;
		}
	}
}
