// Created:	2015/4/1
// Author: wenhm 


#include "logic/skill_manager.h"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "logic/skill_set.h"
#include "logic/unit.hpp"
#include "internet/skill.pb.h"
#include "utility/parse_msg.h"
#include <rlog.hpp>

using namespace faith;
const int32 k_max_event_num = 2000;
skill_set m_empty_skill_set;
skill_set* m_skill_player_ary[init_player_max];
skill_set* m_skill_npc_ary[init_npc_max];

void  skill_manager::init_manager()
{
	memset(m_skill_player_ary, 0, sizeof(m_skill_player_ary));
	memset(m_skill_npc_ary, 0, sizeof(m_skill_npc_ary));
}	
skill_set& skill_manager::get_skill_set(const int32 unit_array_index)
{
	if (unit_array_index < init_player_max)
	{
		if (unit_array_index > 0)
		{
			if (m_skill_player_ary[unit_array_index] == nullptr)
			{
				m_skill_player_ary[unit_array_index] = new skill_set();
				m_skill_player_ary[unit_array_index]->clear_data();
				m_skill_player_ary[unit_array_index]->set_owner_index(unit_array_index);
			}
			return *m_skill_player_ary[unit_array_index];
		}
	}
	else
	{
		int32 npc_array_index = unit_array_index - npc_arrary_index_begin;
		if (npc_array_index > 0 && npc_array_index < init_npc_max)
		{
			if (m_skill_npc_ary[npc_array_index] == nullptr)
			{
				m_skill_npc_ary[npc_array_index] = new skill_set();
				m_skill_npc_ary[npc_array_index]->clear_data();
				m_skill_npc_ary[npc_array_index]->set_owner_index(unit_array_index);
			}
			return *m_skill_npc_ary[npc_array_index];
		}
	}
	return m_empty_skill_set;
}
void skill_manager::remove_unit_skill(const int32& unit_array_index)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.clear_data();
}
void skill_manager::heart_tick(const int32 unit_array_index, const int64& new_time, const int32& tick_time)
{
	if (unit_array_index > 0)
	{
		skill_set& skill_set_ref = get_skill_set(unit_array_index);
		skill_set_ref.heart_tick(new_time, tick_time);
	}
}
void skill_manager::skill_change_msg(int32 unint_index, int32 skill_template_id, int32 pos_index)
{
	skill_set& skill_set_ref = get_skill_set(unint_index);
	skill_set_ref.skill_change_msg(skill_template_id, pos_index);
}
void skill_manager::skill_spell(use_skill_param& param)
{
	skill_set& skill_set_ref = get_skill_set(param.sender_unit_idf.runtime_id);
	skill_set_ref.resp_spell_cast(param);
}
void skill_manager::skill_cast(use_skill_param& param)
{
	skill_set& skill_set_ref = get_skill_set(param.sender_unit_idf.runtime_id);
	skill_set_ref.resp_spell_cast(param);
}
void skill_manager::skill_hurt(use_skill_param& param)
{
	skill_set& skill_set_ref = get_skill_set(param.sender_unit_idf.runtime_id);
	skill_set_ref.skill_hurt(param);
}
void skill_manager::distribute_msg(s_unit_identifier const& unit_idf, skill_proto_skill_operation& request, int32 damage_num)
{
	int32 skill_template_id = request.skill_template_id();
	guid_64 target_guid(request.target_id());
	s_unit_identifier target_unit_idf(target_guid, request.target_runtime_id());		
	guid_64 item_guid;

	skill_set& skill_set_ref = get_skill_set(unit_idf.runtime_id);
		
	unit& unit_ref = unit_man::get_unit(unit_idf);
	fvector skill_location = unit_ref.get_new_map_pos().unit_location;
	int32 skill_yaw = request.yaw();
	int32 effect_index = request.effect_index();
	int32 skill_order = request.skill_order();
	int32 skill_index = request.skill_index();
	bool be_hit = request.be_hit();
	bool be_critical = request.be_critical();
	int32 hit_random = request.hit_random();
	switch (request.operation_type())
	{
	case e_skill_operation_spell:
	case e_skill_operation_spellcast:
	{
		use_skill_param param;
		param.target_unit_idf = target_unit_idf;
		param.skill_template_id = skill_template_id;
		param.pos = skill_location;
		param.yaw = skill_yaw;
		param.skill_order = skill_order;
		param.damage_num = damage_num;
		skill_set_ref.resp_spell_cast(param);
		break;
	}
	case e_skill_operation_hurt:
	{
		if (be_hit == false)
		{
			_RLOG_(MINFO, "distribute_msg unit no hit");
			return;
		}
		use_skill_param param;
		param.sender_unit_idf = unit_idf;
		param.target_unit_idf = target_unit_idf;
		param.skill_template_id = skill_template_id;
		param.effect_index = effect_index;
		param.skill_order = skill_order;
		param.be_critical = be_critical;
		param.hit_random = hit_random;
		skill_set_ref.skill_hurt(param);
		break;
	}
	case e_skill_operation_cancel:
	{
		skill_set_ref.resp_cancel_skill(request);
		break;
	}
	case e_skill_operation_learn:
	{
		skill_set_ref.resp_learn_skill(request);
		break;
	}
	case e_skill_operation_up_level:
	{
		skill_set_ref.resp_upgrade_skill(request);
		break;
	}
	default:
		break;
	}
}
void skill_manager::send_skill_all(const int32& unit_array_index)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.send_skill_all();
}
void skill_manager::skill_set_owner(const int32& unit_array_index)
{
	unit& temp_unit = unit_man::get_unit(unit_array_index);
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.set_owner_guid(temp_unit.get_unit_guid());
}
void skill_manager::load_skill(const int32& unit_array_index, const skill_proto_skill_save_load& load_msg)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);

	skill_set_ref.load_skill_by_db(load_msg);
}

bool skill_manager::load_skill_by_db_lua(int32 unit_array_index, const char *data_ptr, int32 data_len)
{
	skill_proto_skill_save_load load_proto;

	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&load_proto, data_ptr, data_len, false);

	if (!is_sucess)
	{
		return false;
	}

	load_skill(unit_array_index,load_proto);
	return true;
}

void skill_manager::save_skill(const int32& unit_array_index, int32 save_type_ex)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.save_skill_to_db(save_type_ex);
}
void skill_manager::resp_master_skill_by_id(const int32& unit_array_index, int32 skill_id)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.add_skill_inst_by_id(skill_id, true);
}
void skill_manager::remove_unit_skill_by_id(const int32& unit_array_index, int32 skill_id)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.remove_skill_inst_by_id(skill_id);
}
void skill_manager::exp_level_up(const int32& unit_array_index, int32 cur_level)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.exp_level_up(cur_level);
}
void skill_manager::replace_skill_id(const int32& unit_array_index, int32 skill_series, int32 skill_template_id, bool apply, bool use_level)
{
	skill_set& skill_set_ref = get_skill_set(unit_array_index);
	skill_set_ref.change_skill(skill_series, skill_template_id, apply);
}

