/********************************************************************
file base:	passive_skill
file ext:	cpp
author:		zhy
purpose:
*********************************************************************/

#include "passive_skill.h"
#include "logic/unit_man.h"
#include "logic/buff_man.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "system/item/item_system.h"
#include "internet/net.pb.h"
#include "internet/skill.pb.h"
#include "buff_def.hpp"
#include "internal/char_msg.hpp"

using namespace faith;
passive_skill::passive_skill()
{
	ZoneScoped;
	m_array_index = 0;
	clear_data();
}

passive_skill::~passive_skill()
{
	ZoneScoped;

}
void passive_skill::heart_tick(const int64& new_time, const int32& tick_time)
{
	ZoneScoped;
}
void passive_skill::clear_data()
{
	ZoneScoped;
	m_passive_skill_map.clear();
}

void passive_skill::set_data_info(int32 passive_skill_id, e_passive_skill_info e_info, int32 data_value)
{
	ZoneScoped;
	if (e_info >= e_passive_skill_info_max)
	{
		CONSOLE_ERROR("set_data_info error e_info:{} passive_skill_id:{}", (int32)e_info, passive_skill_id);
		return;
	}
	auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("set_data_info error passive_skill_template_ptr is nullptr passive_skill_id:{}", passive_skill_id);
		return;
	}
	auto it = m_passive_skill_map.find(passive_skill_template_ptr->Series);
	if (it == m_passive_skill_map.end())
	{
		CONSOLE_ERROR("set_data_info error passive_skill_template_ptr->Series:{} passive_skill_id:{}", passive_skill_template_ptr->Series, passive_skill_id);
		return;
	}
	it->second.data_ary[e_info] = data_value;
}
int32 passive_skill::get_data_info(int32 passive_skill_id, e_passive_skill_info e_info)
{
	ZoneScoped;
	if (e_info >= e_passive_skill_info_max)
	{
		CONSOLE_ERROR("get_data_info error e_info:{} passive_skill_id:{}", (int32)e_info, passive_skill_id);
		return 0;
	}
	auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("get_data_info error passive_skill_template_ptr is nullptr passive_skill_id:{}", passive_skill_id);
		return 0;
	}
	auto it = m_passive_skill_map.find(passive_skill_template_ptr->Series);
	if (it == m_passive_skill_map.end())
	{
		CONSOLE_ERROR("get_data_info error passive_skill_template_ptr->Series:{} passive_skill_id:{}", passive_skill_template_ptr->Series, passive_skill_id);
		return 0;
	}
	return it->second.data_ary[e_info];
}
void passive_skill::set_passive_skill_att(int32 passive_skill_series, bool is_add)
{
	auto it = m_passive_skill_map.find(passive_skill_series);
	if (it == m_passive_skill_map.end())
	{
		CONSOLE_ERROR("passive_skill_series:{} is_add:{}", passive_skill_series, is_add);
		return;
	}
	auto passive_skill_id = it->second.data_ary[e_passive_skill_info_passive_skill_id];
	PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("passive_skill_template_ptr is nullptr passive_skill_series:{} passive_skill_id:{}", passive_skill_series, passive_skill_id);
		return;
	}
	player& player_ref = unit_man::get_player(m_array_index);
	player_ref.get_pawn_att().apply_att_change_by_array(passive_skill_template_ptr->AttArray, is_add);
}
void passive_skill::set_passive_skill_buff(int32 passive_skill_series, bool is_add)
{
	auto it = m_passive_skill_map.find(passive_skill_series);
	if (it == m_passive_skill_map.end())
	{
		CONSOLE_ERROR("passive_skill_series:{} is_add:{}", passive_skill_series, is_add);
		return;
	}
	auto passive_skill_id = it->second.data_ary[e_passive_skill_info_passive_skill_id];
	auto skill_cd = it->second.data_ary[e_passive_skill_info_cd];
	PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("passive_skill_template_ptr is nullptr passive_skill_series:{} passive_skill_id:{}", passive_skill_series, passive_skill_id);
		return;
	}
	for (int32 i = 0; i < passive_skill_template_ptr->BuffArray.size(); ++i)
	{
		if (is_add)
		{
			buff_ex_env_param penv;
			penv.buff_level = 1;
			penv.skill_template_id = passive_skill_id;
			penv.skill_cd = skill_cd * 1000;
			buff_man::add_buff_inst(m_array_index, m_array_index, passive_skill_template_ptr->BuffArray[i], &penv);
		}
		else
		{
			buff_man::del_buff_by_template_id(m_array_index, m_array_index, passive_skill_template_ptr->BuffArray[i]);
		}
	}
}
void passive_skill::save_passive_skill(int32 save_type_ex)
{
	ZoneScoped;
	player& player_ref = unit_man::get_player(m_array_index);
	skill_s2s_sl_passive_skill_info msg;
	for (auto& it : m_passive_skill_map)
	{
		auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, it.second.data_ary[e_passive_skill_info_passive_skill_id]);
		if (passive_skill_template_ptr->Type == e_passive_skill_info_type_direct_get)
		{
			auto skill_msg = msg.add_data_list();
			for (int32 i = 0; i < e_passive_skill_info_max; ++i)
			{
				skill_msg->add_data_ary(it.second.data_ary[i]);
			}
		}
	}
	player_ref.send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_passive_skill, save_type_ex);
}

void passive_skill::load_passive_skill(const skill_s2s_sl_passive_skill_info& load_data)
{
	ZoneScoped;
	for (auto& it : load_data.data_list())
	{
		s_passive_skill_info skill_info;
		for (int32 i = 0; i < e_passive_skill_info_max && i < it.data_ary_size(); ++i)
		{
			skill_info.data_ary[i] = it.data_ary(i);
		}
		auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, skill_info.data_ary[e_passive_skill_info_passive_skill_id]);
		if (passive_skill_template_ptr)
		{
			m_passive_skill_map.insert({ passive_skill_template_ptr->Series, skill_info });
			set_passive_skill_att(passive_skill_template_ptr->Series, true);
		}
		else
		{
			CONSOLE_ERROR("load_passive_skill error passive_skill_template_ptr is nullptr passive_skill_id:{}", skill_info.data_ary[e_passive_skill_info_passive_skill_id]);
		}
	}
	send_passive_info_all();
}

void passive_skill::send_passive_info_one(int32 skill_series)
{
	ZoneScoped;
	player& player_ref = unit_man::get_player(m_array_index);
	skill_proto_skill_info_all msg;
	msg.set_unit_guid(player_ref.get_unit_guid().server_64);

	auto it = m_passive_skill_map.find(skill_series);
	if (it == m_passive_skill_map.end())
	{
		msg.set_is_clear(skill_series);
	}
	else
	{
		msg.set_is_clear(0);
		auto temp_msg = msg.add_skill_list();
		for (int32 i = 0; i < e_passive_skill_info_max; ++i)
		{
			temp_msg->add_data_ary(it->second.data_ary[i]);
		}
	}

	player_ref.send_message_to_self(&msg, e_msgindex_s2c_update_passive_skill);
}

void passive_skill::send_passive_info_all()
{
	ZoneScoped;
	player& player_ref = unit_man::get_player(m_array_index);
	skill_proto_skill_info_all msg;
	msg.set_unit_guid(player_ref.get_unit_guid().server_64);
	msg.set_is_clear(true);
	if (m_passive_skill_map.empty() == false)
	{
		for (auto& it : m_passive_skill_map)
		{
			auto temp_msg = msg.add_skill_list();
			for (int32 i = 0; i < e_passive_skill_info_max; ++i)
			{
				temp_msg->add_data_ary(it.second.data_ary[i]);
			}
		}
	}
	player_ref.send_message_to_self(&msg, e_msgindex_s2c_update_passive_skill);
}

int32 passive_skill::get_passive_skill_id_by_buff_action_id(int32 buff_action_id)
{
	ZoneScoped;
	int32 fight_power = 0;
	for (auto it = m_passive_skill_map.begin(); it != m_passive_skill_map.end(); it++)
	{
		s_passive_skill_info& temp_info = it->second;
		if (temp_info.data_ary[e_passive_skill_info_equip_slot] > 0)
		{
			PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, temp_info.data_ary[e_passive_skill_info_passive_skill_id]);
			if (passive_skill_template_ptr && passive_skill_template_ptr->BuffArray.empty() == false)
			{
				BuffDataTemplate* buff_tem = GET_TEMPLATE(BuffDataTemplate, passive_skill_template_ptr->BuffArray[0]);
				if (buff_tem && buff_tem->CycActions.empty() == false)
				{
					if (buff_tem->CycActions[0] == buff_action_id)
					{
						return temp_info.data_ary[e_passive_skill_info_passive_skill_id];
					}
				}
			}
		}
	}
	return 0;
}

int64 passive_skill::get_passive_skill_gs()
{
	ZoneScoped;
	int64 fight_power = 0;
	for (auto it = m_passive_skill_map.begin(); it != m_passive_skill_map.end(); it++)
	{
		s_passive_skill_info& temp_info = it->second;
		if (temp_info.data_ary[e_passive_skill_info_equip_slot] > 0)
		{
			PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, temp_info.data_ary[e_passive_skill_info_passive_skill_id]);
			if (passive_skill_template_ptr)
			{
				fight_power += passive_skill_template_ptr->GSAddValue;
			}
		}
	}
	return fight_power;
}

s_passive_skill_info* passive_skill::get_passive_skill_by_series(int32 passive_skill_series)
{
	ZoneScoped;
	auto iter_skill = m_passive_skill_map.find(passive_skill_series);
	if (iter_skill != m_passive_skill_map.end())
	{
		return &(iter_skill->second);
	}

	return nullptr;
}
int32 passive_skill::get_passive_skill_equip_count()
{
	ZoneScoped;
	int32 res = 0;
	for (auto& it : m_passive_skill_map)
	{
		if (it.second.data_ary[e_passive_skill_info_equip_slot] > 0 && it.second.data_ary[e_passive_skill_info_equip_slot] < special_passive_skill_slot)
		{
			res++;
		}
	}
	return res;
}
void passive_skill::passive_skill_up_level(int32 passive_skill_id)
{
	ZoneScoped;
	auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("passive_skill_template_ptr is nullptr passive_skill_id:{}", passive_skill_id);
		return;
	}
	if (passive_skill_template_ptr->Type != e_passive_skill_info_type_direct_get)
	{
		CONSOLE_ERROR("passive_skill_template_ptr->Type:{} passive_skill_id:{}", passive_skill_template_ptr->Type, passive_skill_id);
		return;
	}
	auto next_passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id + 1);
	if (nullptr == next_passive_skill_template_ptr)
	{
		return;
	}
	if (passive_skill_template_ptr->OpenItem.size() < 2)
	{
		CONSOLE_ERROR("passive_skill_template_ptr->OpenItem.size() < 2 passive_skill_id:{}", passive_skill_id);
		return;
	}
	auto& player_ref = unit_man::get_player(m_array_index);
	if (item_system::can_cost_item(&player_ref, e_bag_type_bag, passive_skill_template_ptr->OpenItem[0], passive_skill_template_ptr->OpenItem[1]) == false)
	{
		CONSOLE_ERROR("item_id:{} need_count:{} passive_skill_id:{}", passive_skill_template_ptr->OpenItem[0], passive_skill_template_ptr->OpenItem[1], passive_skill_id);
		return;
	}
	item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, passive_skill_template_ptr->OpenItem[0], passive_skill_template_ptr->OpenItem[1]);

	auto it = m_passive_skill_map.find(passive_skill_template_ptr->Series);
	if (it != m_passive_skill_map.end())
	{
		set_passive_skill_att(passive_skill_template_ptr->Series, false);
		it->second.data_ary[e_passive_skill_info_passive_skill_id] = passive_skill_id + 1;
		set_passive_skill_att(passive_skill_template_ptr->Series, true);
	}
	else
	{
		s_passive_skill_info temp_info;
		temp_info.data_ary[e_passive_skill_info_passive_skill_id] = passive_skill_id;
		temp_info.data_ary[e_passive_skill_info_equip_slot] = 0;
		temp_info.data_ary[e_passive_skill_info_cd] = 0;
		m_passive_skill_map.insert({ passive_skill_template_ptr->Series, temp_info });
		set_passive_skill_att(passive_skill_template_ptr->Series, true);
	}
	send_passive_info_one(passive_skill_template_ptr->Series);
}
int32 passive_skill::passive_skill_equip(int32 passive_skill_id, int32 slot)
{
	ZoneScoped;
	if (slot <= 0)
	{
		CONSOLE_ERROR("slot:{} passive_skill_id:{}", slot, passive_skill_id);
		return 0;
	}
	auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("passive_skill_template_ptr is nullptr passive_skill_id:{}", passive_skill_id);
		return 0;
	}
	if (passive_skill_template_ptr->Type != e_passive_skill_info_type_direct_get)
	{
		CONSOLE_ERROR("passive_skill_template_ptr->Type:{} passive_skill_id:{}", passive_skill_template_ptr->Type, passive_skill_id);
		return passive_skill_id;
	}
	auto it = m_passive_skill_map.find(passive_skill_template_ptr->Series);
	if (it == m_passive_skill_map.end())
	{
		CONSOLE_ERROR("passive_skill_template_ptr->Series:{} passive_skill_id:{}", passive_skill_template_ptr->Series, passive_skill_id);
		return 0;
	}
	if (it->second.data_ary[e_passive_skill_info_equip_slot] == slot)
	{
		CONSOLE_ERROR("slot:{} passive_skill_id:{}", slot, passive_skill_id);
		return passive_skill_id;
	}
	int32 res_id = it->second.data_ary[e_passive_skill_info_passive_skill_id];
	for (auto& oldIt : m_passive_skill_map)
	{
		if (oldIt.second.data_ary[e_passive_skill_info_equip_slot] == slot)
		{
			set_passive_skill_buff(oldIt.first, false);
			oldIt.second.data_ary[e_passive_skill_info_equip_slot] = 0;
			res_id = oldIt.second.data_ary[e_passive_skill_info_passive_skill_id];
			send_passive_info_one(oldIt.first);
			break;
		}
	}
	if (it->second.data_ary[e_passive_skill_info_equip_slot] > 0)
	{
		it->second.data_ary[e_passive_skill_info_equip_slot] = slot;
	}
	else
	{
		it->second.data_ary[e_passive_skill_info_equip_slot] = slot;
		set_passive_skill_buff(passive_skill_template_ptr->Series, true);
	}
	send_passive_info_one(passive_skill_template_ptr->Series);
	auto& player_ref = unit_man::get_player(m_array_index);
	player_ref.get_mission_mgr().target_check(e_mission_end_type_passive_skill_count);
	return res_id;
}

void passive_skill::passive_skill_special(int32 passive_skill_id, bool is_add)
{
	ZoneScoped;
	auto passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_id);
	if (nullptr == passive_skill_template_ptr)
	{
		CONSOLE_ERROR("passive_skill_template_ptr is nullptr passive_skill_id:{}", passive_skill_id);
		return;
	}
	if (is_add)
	{
		auto it = m_passive_skill_map.find(passive_skill_template_ptr->Series);
		if (it != m_passive_skill_map.end())
		{
			CONSOLE_ERROR("passive_skill_template_ptr->Series:{} passive_skill_id:{}", passive_skill_template_ptr->Series, passive_skill_id);
			return;
		}
		s_passive_skill_info temp_info;
		temp_info.data_ary[e_passive_skill_info_passive_skill_id] = passive_skill_id;
		temp_info.data_ary[e_passive_skill_info_equip_slot] = special_passive_skill_slot;
		temp_info.data_ary[e_passive_skill_info_cd] = 0;
		m_passive_skill_map.insert({ passive_skill_template_ptr->Series, temp_info });
		set_passive_skill_att(passive_skill_template_ptr->Series, is_add);
		set_passive_skill_buff(passive_skill_template_ptr->Series, is_add);
	}
	else
	{
		auto it = m_passive_skill_map.find(passive_skill_template_ptr->Series);
		if (it == m_passive_skill_map.end())
		{
			CONSOLE_ERROR("passive_skill_template_ptr->Series:{} passive_skill_id:{}", passive_skill_template_ptr->Series, passive_skill_id);
			return;
		}
		set_passive_skill_att(passive_skill_template_ptr->Series, is_add);
		set_passive_skill_buff(passive_skill_template_ptr->Series, is_add);
		m_passive_skill_map.erase(it);
	}
}


