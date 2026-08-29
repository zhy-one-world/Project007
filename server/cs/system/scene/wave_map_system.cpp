#include "server_log.hpp"
#include "template/template_manager.h"
#include "wave_map_system.h"
#include "components/scene/award_map_component.h"
#include "components/scene/base_map_component.h"
#include "logic/world_cs.h"
#include "map_search_system.h"
#include "logic/unit_man.h"
#include "logic/buff_man.h"
#include "logic/npc.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "gm_order_def.hpp"

using namespace faith;

void wave_map_system::start_up(Entity* map_ent, int32 brush_id)
{
	ZoneScoped;
	auto brush_strange_template = GET_TEMPLATE(BrushStrangeTemplate, brush_id);
	if (nullptr == brush_strange_template)
	{
		CONSOLE_ERROR("brush_strange_template is null id:{}", brush_id);
		return;
	}
	if (brush_strange_template->WaveId.size() < 2)
	{
		CONSOLE_ERROR("WaveId size short BrushId:{}, size:{}", brush_id, brush_strange_template->WaveId.size());
		return;
	}
	auto award_map_cp = map_ent->add_component<award_map_component>();
	award_map_cp->m_npc_wave_map.clear();
	award_map_cp->m_npc_damage_list.clear();
	award_map_cp->m_wave_index = 0;
	award_map_cp->m_player_index = -1;
	award_map_cp->m_brush_strange_template = brush_strange_template;

}
void  wave_map_system::shut_down(Entity* entity)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	entity->remove_component<award_map_component>();
}

void wave_map_system::heart_tick(const int64& new_time)
{
	ZoneScoped;
	g_ecs->each<award_map_boss_component>([&](Entity* ent, ComponentHandle<award_map_boss_component> amb_cp) -> bool {
		if (amb_cp->m_be_call == false)
		{
			refresh_boss(ent);
			return true;
		}
		return true;
	});
}

void wave_map_system::on_player_add(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	award_map_cp->m_player_index = unit_index;
	refresh_brush(entity);
	return;
}

void wave_map_system::on_player_delete(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	if (award_map_cp->m_player_index != unit_index)
	{
		CONSOLE_ERROR("unit_index is not player index:{} unit_index:{}", award_map_cp->m_player_index, unit_index);
		return;
	}
	auto& player_ref = unit_man::get_player(unit_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player is null index:{}", unit_index);
		return;
	}
	award_map_cp->m_player_index = -1;
}

void wave_map_system::on_player_load_data_over(Entity* map_ent, const int32& unit_index)
{
}
void wave_map_system::on_player_enter_scene(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto& player_ref = unit_man::get_player(unit_index);
	if (!player_ref.is_valid())
	{
		CONSOLE_ERROR("player is null index:{}", unit_index);
		return;
	}
}

void wave_map_system::on_player_leave_scene(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto& player_ref = unit_man::get_player(unit_index);
	if (false == player_ref.is_valid())
	{
		return;
	}
}
void wave_map_system::on_player_dead(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	if (award_map_cp->m_player_index != unit_index)
	{
		CONSOLE_ERROR("unit_index is not player index:{} unit_index:{}", award_map_cp->m_player_index, unit_index);
		return;
	}

	send_refresh_boss(entity, e_map_search_boss_type_be_killed);

	entity->remove_component<award_map_boss_component>();

	auto wave_index = award_map_cp->m_wave_index;
	auto wave_map = award_map_cp->m_npc_wave_map[wave_index];
	for (auto it : wave_map)
	{
		unit_man::remove_npc(it, false);
	}
	refresh_wave(entity, award_map_cp->m_brush_strange_template->WaveId[wave_index], wave_index);
}

void wave_map_system::on_player_reconnect(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto& player_ref = unit_man::get_player(unit_index);
	if (!player_ref.is_valid())
	{
		CONSOLE_ERROR("player is null index:{}", unit_index);
		return;
	}
}

void wave_map_system::on_npc_add(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto& npc_ptr = unit_man::get_npc(unit_index);
	if (npc_ptr.is_valid() == false)
	{
		CONSOLE_ERROR("npc_ptr is null index:{}", unit_index);
		return;
	}
	auto spawn_id = npc_ptr.get_spawn_point_id();
	if (npc_ptr.get_unit_type() == e_unit_type_monster)
	{
		award_map_cp->m_npc_wave_map[spawn_id].push_back(unit_index);
	}
}

void wave_map_system::on_npc_dead(Entity* entity, const int32& unit_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	auto& player_ref = unit_man::get_player(award_map_cp->m_player_index);
	if (!player_ref.is_valid())
	{
		CONSOLE_ERROR("player is null index:{}", award_map_cp->m_player_index);
		return;
	}
	auto& npc_ptr = unit_man::get_npc(unit_index);
	if (npc_ptr.is_valid() == false)
	{
		CONSOLE_ERROR("npc_ptr is null index:{}", unit_index);
		return;
	}
	auto spawn_id = npc_ptr.get_spawn_point_id();
	award_map_cp->m_npc_wave_map[spawn_id].remove(unit_index);
	award_map_cp->m_npc_damage_list.remove(unit_index);

	if (npc_ptr.is_boss())
	{
		entity->remove_component<award_map_boss_component>();
		send_refresh_boss(entity, e_map_search_boss_type_boss_killed);
		send_refresh_boss(entity, e_map_search_boss_type_boss_no_kill);
	}
	if (award_map_cp->m_npc_wave_map[spawn_id].empty())
	{
		auto wave_index = award_map_cp->m_wave_index;
		wave_index += 1;
		if (wave_index >= award_map_cp->m_brush_strange_template->WaveId.size())
		{
			wave_index = 0;
		}
		auto next_wave = wave_index + 1;
		if (next_wave >= award_map_cp->m_brush_strange_template->WaveId.size())
		{
			next_wave = 0;
		}
		award_map_cp->m_wave_index = wave_index;
		refresh_wave(entity, award_map_cp->m_brush_strange_template->WaveId[next_wave], next_wave);

	}
}
void wave_map_system::on_npc_delete(Entity* entity, const int32& unit_index, bool is_dead)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto& npc_ptr = unit_man::get_npc(unit_index);
	if (npc_ptr.is_valid() == false)
	{
		CONSOLE_ERROR("npc_ptr is null index:{}", unit_index);
		return;
	}
	auto spawn_id = npc_ptr.get_spawn_point_id();
	award_map_cp->m_npc_wave_map[spawn_id].remove(unit_index);
	award_map_cp->m_npc_damage_list.remove(unit_index);
}
void wave_map_system::on_unit_be_damage(Entity* entity, const int32& attack_index, const int32& be_attack_index, double damage_value)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	if (award_map_cp->m_player_index != attack_index)
	{
		CONSOLE_ERROR("attack_index is not player index:{} attack_index:{}", award_map_cp->m_player_index, attack_index);
		return;
	}
	award_map_cp->m_npc_damage_list.push_back(be_attack_index);
}

void wave_map_system::refresh_brush(Entity* entity)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}	
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		CONSOLE_ERROR("award_map_cp is null");
		return;
	}

	auto wave_index = award_map_cp->m_wave_index;
	refresh_wave(entity, award_map_cp->m_brush_strange_template->WaveId[wave_index], wave_index);
	int32 next_wave = wave_index + 1;
	if (next_wave >= award_map_cp->m_brush_strange_template->WaveId.size())
	{
		next_wave = 0;
	}
	refresh_wave(entity, award_map_cp->m_brush_strange_template->WaveId[next_wave], next_wave);

}
void wave_map_system::refresh_wave(Entity* entity, int32 wave_id, int32 wave_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto wave_template = GET_TEMPLATE(WaveTemplate, wave_id);
	if (nullptr == wave_template)
	{
		CONSOLE_ERROR("wave_template is null wave_id:{}", wave_id);
		return;
	}
	for (int32 i = 0; i < wave_template->NpcId.size(); ++i)
	{
		if (wave_template->NpcId[i] <=0)
		{
			break;
		}
		s_map_pos born_pos;
		born_pos.unit_location.x = wave_template->BornX[i];
		born_pos.unit_location.y = wave_template->BornY[i];
		born_pos.unit_location.z = wave_template->BornZ[i];
		born_pos.unit_rotation.pitch = wave_template->BornPitch[i];
		born_pos.unit_rotation.roll = wave_template->BornRoll[i];
		born_pos.unit_rotation.yaw = wave_template->BornYaw[i];
		world_cs::spawn_npc(wave_template->NpcId[i], born_pos, entity, 0, wave_index);
	}
}
void wave_map_system::refresh_boss(Entity* entity)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	entity->with<base_map_component, award_map_component>([entity](ComponentHandle<base_map_component> base_map_cp, ComponentHandle<award_map_component> award_map_cp) {

		if (award_map_cp.isValid() == false)
		{
			CONSOLE_ERROR("award_map_cp is null");
			return;
		}
		if (award_map_cp->m_npc_damage_list.empty() == false)
		{
			return;
		}
		auto amb_cp = entity->get_component<award_map_boss_component>();
		amb_cp->m_be_call = true;
		auto wave_index = award_map_cp->m_wave_index;
		auto wave_map = award_map_cp->m_npc_wave_map[wave_index];
		for (auto it : wave_map)
		{
			unit_man::remove_npc(it, false);
		}
		auto wave_template = GET_TEMPLATE(WaveTemplate, award_map_cp->m_brush_strange_template->WaveId[wave_index]);

		if (nullptr == wave_template)
		{
			CONSOLE_ERROR("wave_template is null m_wave_index:{} wave_id:{}", wave_index, award_map_cp->m_brush_strange_template->WaveId[wave_index]);
			return;
		}

		//s_map_pos born_pos;
		//born_pos.unit_location.x = wave_template->BossBornPos[0];
		//born_pos.unit_location.y = wave_template->BossBornPos[1];
		//born_pos.unit_location.z = wave_template->BossBornPos[2];
		//born_pos.unit_rotation.pitch = wave_template->BossBornRot[0];
		//born_pos.unit_rotation.roll = wave_template->BossBornRot[1];
		//born_pos.unit_rotation.yaw = wave_template->BossBornRot[2];
		//world_cs::spawn_npc(brush_strange_template->BossId, born_pos, entity, 0, wave_index);
		send_refresh_boss(entity, e_map_search_boss_type_boss_refresh);

	});

}
void wave_map_system::send_refresh_boss(Entity* entity, int32 res)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		return;
	}
	auto& player = unit_man::get_player(award_map_cp->m_player_index);
	if (!player.is_valid())
	{
		CONSOLE_ERROR("player is null index:{}", award_map_cp->m_player_index);
		return;
	}
	game_proto_kill_boss_end msg;
	msg.set_result(res);
	player.send_message_to_self(&msg, e_msgindex_s2c_kill_boss);
}
int32 wave_map_system::kill_boss(Entity* entity)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return 0;
	}
	auto old_amb_cp = entity->get_component<award_map_boss_component>();
	if (old_amb_cp)
	{
		return 1;
	}
	auto amb_cp = entity->add_component<award_map_boss_component>();
	amb_cp->m_be_call = false;
	send_refresh_boss(entity, e_map_search_boss_type_boss_wait);
	return 0;
}
void wave_map_system::kill_cur_wave(Entity* entity)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is null");
		return;
	}
	auto award_map_cp = entity->get_component<award_map_component>();
	if (false == award_map_cp.isValid())
	{
		CONSOLE_ERROR("award_map_cp is null");
		return;
	}
	auto wave_map = award_map_cp->m_npc_wave_map[award_map_cp->m_wave_index];
	buff_ex_env_param penv;
	penv.damage_num = int32_MAX_NUM;
	penv.buff_level = 1;
	for (auto it : wave_map)
	{
		buff_man::add_buff_inst(award_map_cp->m_player_index, it, GM_DAMAGE_BUFF_ID, &penv);
	}
}
