
#include "ai_unit.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"

namespace faith
{
	ai_unit::ai_unit()
	{
		clear_data();
	}
	void ai_unit::clear_data()
	{
		m_ai_level_nomal.clear_data();
	}
	//npc初始化
	void ai_unit::init_ai(npc* unit_ptr, const s_map_pos& born_pos, const s_map_pos& patrol_pos, NpcTemplate* npc_template_ptr, Entity* map_ent)
	{
		if (NULL == unit_ptr || NULL == npc_template_ptr)
		{
			return;
		}
		m_ai_level_nomal.create_ai(unit_ptr, born_pos, patrol_pos, npc_template_ptr, map_ent);
	}

	void ai_unit::init_fake_player_info(const s_fake_player_info& fake_player_ptr)
	{
		m_ai_level_nomal.init_fake_player_info(fake_player_ptr);
	}

	//npc时间tick
	void ai_unit::ai_tick(const int64& new_time)
	{
		m_ai_level_nomal.ai_tick(new_time);
	}
	void ai_unit::relieve_stop_time(const int32& skill_template_id)
	{
		m_ai_level_nomal.relieve_stop_time(skill_template_id);
	}
	bool ai_unit::check_play_in_damage_list(guid_64 player_guid)
	{
		damage_info_vector damage_list = get_damage_list();
		int32 list_len = damage_list.size();
		for (int32 idx = 0; idx < list_len; idx++)
		{
			const s_damage_info& damage_info = damage_list[idx];
			if (damage_info.unit_identifier.unit_guid == player_guid)
			{
				return true;
			}
		}
		return false;
	}

	void ai_unit::clear_bron_data()
	{
		m_ai_level_nomal.npc_bron_clear();
	}

	void ai_unit::attack_value(const int32& attack_index, const int64 hp)
	{
		m_ai_level_nomal.attack_value(attack_index, hp);
	}
	void ai_unit::add_damage_value_with_summon(const int32& attack_index, const int64 hp)
	{
		m_ai_level_nomal.add_damage_value_with_summon(attack_index, hp);
	}
	void ai_unit::add_level_nomal_next_time(int32 add_time)
	{
		int64 time_now = utility::get_tick_count();
		m_ai_level_nomal.set_next_time(time_now, add_time);
	}
	void ai_unit::set_level_nomal_born_pos(s_map_pos born_pos)
	{
		m_ai_level_nomal.set_born_pos(born_pos);
	}
	void ai_unit::clear_hate_value()
	{
		m_ai_level_nomal.clear_hate_value();
	}
	guid_64 ai_unit::get_follow_guid()
	{
		return m_ai_level_nomal.get_follow_guid();
	}
	void ai_unit::set_follow_guid(guid_64 role_guid)
	{
		m_ai_level_nomal.set_follow_guid(role_guid);
	}
	const ai_level_nomal & ai_unit::get_ai_level()
	{
		return m_ai_level_nomal;
	}
	fvector ai_unit::get_bron_location()
	{
		return m_ai_level_nomal.get_bron_location();
	}
}