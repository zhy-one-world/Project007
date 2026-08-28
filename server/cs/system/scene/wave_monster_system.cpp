#include "wave_monster_system.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "components/scene/wave_monster_component.h"
#include "components/scene/base_map_component.h"
#include "logic/world_cs.h"
#include "base_map_system.h"
#include "internet/map.pb.h"
#include "logic/npc.hpp"
#include "internet/net.pb.h"

using namespace hld;

void wave_monster_system::start_up(Entity* map_ent, MapTemplate* map_template)
{
	ZoneScoped;
	map_ent->add_component<wave_monster_component>();
	return;
}
void wave_monster_system::shut_down(Entity* map_ent)
{
	map_ent->remove_component<wave_monster_component>();
}
void wave_monster_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	g_ecs->each<wave_monster_tick_component>([&](Entity* map_ent, ComponentHandle<wave_monster_tick_component> wmt_cp) -> bool {
		refresh_brush(map_ent);
		return true;
	});
}
void wave_monster_system::on_npc_delete(Entity* map_ent, npc* npc_ptr)
{
	ZoneScoped;
	auto wave_monster_cp = map_ent->get_component<wave_monster_component>();
	if (false == wave_monster_cp.isValid())
	{
		return;
	}
	for (auto it = wave_monster_cp->m_brush_list.begin(); it != wave_monster_cp->m_brush_list.end(); ++it)
	{
		auto& brush_info = it->second;
		auto find_it = brush_info.m_wave_list.find(npc_ptr->get_array_index());
		if (find_it != brush_info.m_wave_list.end())
		{
			brush_info.m_wave_list.erase(find_it);
			if (brush_info.m_wave_list.empty())
			{
				wave_monster_cp->m_brush_list.erase(it);
			}
			break;
		}
	}
}
bool wave_monster_system::monster_kill_end(Entity* map_ent)
{
	ZoneScoped;
	auto wave_monster_cp = map_ent->get_component<wave_monster_component>();
	if (false == wave_monster_cp.isValid())
	{
		return true;
	}
	return wave_monster_cp->m_brush_list.empty();
}
void wave_monster_system::init_brush(Entity* map_ent)
{
	ZoneScoped;
	auto wave_monster_cp = map_ent->get_component<wave_monster_component>();
	if (false == wave_monster_cp.isValid())
	{
		return;
	}
	map_ent->add_component<wave_monster_tick_component>();
	auto cur_time = utility::get_tick_count();

	auto base_map_cp = map_ent->get_component<base_map_component>();
	for (auto& it : base_map_cp->m_map_template->TriggerList)
	{
		if (it <= 0)
		{
			break;
		}
		auto brush_template = GET_TEMPLATE(BrushStrangeTemplate,it);
		if (nullptr == brush_template)
		{
			CONSOLE_ERROR("brush_template is nullptr id:{}", it);
			continue;
		}
		auto wave_count = brush_template->WaveId.size();
		if (wave_count != brush_template->NextTime.size() || wave_count != brush_template->LiveTime.size() || wave_count != brush_template->IsDead.size())
		{
			CONSOLE_ERROR("wave_count is not equal id:{} wave_count:{} NextTime:{} LiveTime:{} IsDead:{}", it, wave_count, brush_template->NextTime.size(), brush_template->LiveTime.size(), brush_template->IsDead.size());
			continue;
		}
		s_brush_info info;
		info.m_brush_id = it;
		info.m_cur_wave_index = 0;
		info.m_brush_template = brush_template;
		if (brush_template->IsDead[0] == 1)
		{
			auto add_time = (brush_template->LiveTime[0]) * 1000;
			int32 int_add_time = add_time;
			info.m_next_wave_time = cur_time + int_add_time;
		}
		else
		{
			auto add_time = (brush_template->LiveTime[0]) * 1000;
			int32 int_add_time = add_time;
			info.m_next_wave_time = cur_time + int_add_time;
		}
		wave_monster_cp->m_brush_list[it] = info;
	}
}
void wave_monster_system::refresh_brush(Entity* map_ent)
{
	ZoneScoped;
	auto wave_monster_cp = map_ent->get_component<wave_monster_component>();
	auto cur_time = utility::get_tick_count();
	auto base_map_cp = map_ent->get_component<base_map_component>();
	for (auto& it : wave_monster_cp->m_brush_list)
	{
		auto& brush_info = it.second;
		if (brush_info.m_next_wave_time > cur_time)
		{
			continue;
		}
		if (brush_info.m_brush_template->IsDead[brush_info.m_cur_wave_index] == 1)
		{
			if (brush_info.m_wave_list.empty() == false)
			{
				continue;
			}
		}
		auto wave_index = brush_info.m_cur_wave_index;
		auto wave_id = brush_info.m_brush_template->WaveId[wave_index];
		auto wave_template = GET_TEMPLATE(WaveTemplate, wave_id);
		if (nullptr == wave_template)
		{
			CONSOLE_ERROR("wave_template is nullptr id:{}", wave_id);
			continue;
		}
		auto npc_count = wave_template->NpcId.size();
		if (npc_count != wave_template->BornX.size() || npc_count != wave_template->BornY.size() || npc_count != wave_template->BornZ.size()|| npc_count != wave_template->BornYaw.size() || npc_count != wave_template->BornPitch.size() || npc_count != wave_template->BornRoll.size())
		{
			CONSOLE_ERROR("wave_count is not equal id:{} npc_count:{} BornX:{} BornY:{} BornZ:{} BornYaw:{} BornPitch:{} BornRoll:{}", wave_id, npc_count, wave_template->BornX.size(), wave_template->BornY.size(), wave_template->BornZ.size(), wave_template->BornYaw.size(), wave_template->BornPitch.size(), wave_template->BornRoll.size());
			continue;
		}

		for (auto i = 0; i < npc_count; ++i)
		{
			s_map_pos born_pos;
			born_pos.unit_location.x = wave_template->BornX[i];
			born_pos.unit_location.y = wave_template->BornY[i];
			born_pos.unit_location.z = wave_template->BornZ[i];
			born_pos.unit_rotation.yaw = wave_template->BornYaw[i];
			born_pos.unit_rotation.pitch = wave_template->BornPitch[i];
			born_pos.unit_rotation.roll = wave_template->BornRoll[i];

			auto npc_ptr = world_cs::spawn_npc(wave_template->NpcId[i], born_pos, map_ent, nullptr, brush_info.m_brush_id);
			if (npc_ptr)
			{
				brush_info.m_wave_list[npc_ptr->get_array_index()] = brush_info.m_cur_wave_index;
			}
		}
		brush_info.m_cur_wave_index++;
		if (brush_info.m_cur_wave_index < brush_info.m_brush_template->WaveId.size())
		{
			if (brush_info.m_brush_template->IsDead[brush_info.m_cur_wave_index] == 1)
			{
				brush_info.m_next_wave_time = cur_time + brush_info.m_brush_template->NextTime[brush_info.m_cur_wave_index] * 1000;
			}
			else
			{
				brush_info.m_next_wave_time = cur_time + brush_info.m_brush_template->LiveTime[brush_info.m_cur_wave_index] * 1000;
			}
		}
		else
		{
			map_ent->remove_component<wave_monster_tick_component>();
		}
	}
}
void wave_monster_system::send_wave_info(Entity* map_ent, const int32& wave_index)
{
	ZoneScoped;
	auto wave_monster_cp = map_ent->get_component<wave_monster_component>();
	if (false == wave_monster_cp.isValid())
	{
		return;
	}
	map_s2c_wave_info msg;
	msg.set_wave_index(wave_index);
	base_map_system::send_map_message(map_ent, &msg, e_msgindex_s2c_wave_trigger_message);
}