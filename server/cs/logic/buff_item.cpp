/********************************************************************
created:	2010/03/03
created:	3:3:2010   14:46
filename: 	d:\worksite\skill\lowserver\octopus\app\cs\low\new_buff\buff_item.cpp
file path:	d:\worksite\skill\lowserver\octopus\app\cs\low\new_buff
file base:	buff_item
file ext:	cpp
author:		zhangjianyu

purpose:
*********************************************************************/

#include "time.hpp"
#include "buff_item.h"
#include "logic/unit.hpp"
#include "buff_event.h"
#include "template/template_manager.h"
#include "logic/world_cs.h"
#include "logic/unit_man.h"
#include "utility/guid_gen.h"
#include "internet/skill.pb.h"
#include "internet/net.pb.h"

namespace hld
{
	buff_item::buff_item()
	{
		clear_data();
	}

	buff_item::~buff_item()
	{
		clear_data();
	}

	void buff_item::clear_data()
	{
		m_is_end = false;
		m_data_use = false;
		m_sender_index = 0;					//发送者
		m_owner_index = 0;					//拥有者
		m_buff_guid.clear_data();					//buff id (同一player上标识不同buff)
		m_life_time = 0;
		m_buff_data.Clear();					//buff数据
		for (int32 i = 0; i < EBuffInfo_max; ++i)
		{
			m_buff_data.add_data_ary(0);
		}
		m_buff_template_ptr = nullptr;			//模板指针
		m_begin_action.clear();					//开始的action
		m_begin_action_num = 0;
		m_cyc_effect.clear();					//循环的action
		m_cyc_action_num = 0;
		m_end_action.clear();					//结束action
		m_end_action_num = 0;
		m_effect_action.clear();				//持续action
		m_effect_action_num = 0;
		m_event_action.clear();					//事件action
		m_event_action_num = 0;
		m_last_action_tick = 0;
		m_jump_tick = 0;
		m_env.clear_data();
		m_trigger_event = false;
		m_is_ignore_att = false;
		m_is_send_client = true;
		m_is_send_one = false;
	}
	bool buff_item::on_event(s_trigger_param& trigger_param)
	{
		if (nullptr == m_buff_template_ptr)
		{
			return false;
		}
		if (m_buff_template_ptr->EventCondition.size() < 10 || m_buff_template_ptr->EventCondition[0] <= 0)
		{
			return false;
		}

		if (m_buff_template_ptr->EventCondition[0] == trigger_param.m_type)
		{
			switch (trigger_param.m_type)
			{
			case e_game_event_type_unit_dispel_self:
			case e_game_event_type_unit_dispel_target:
			{
				m_life_time = 0;
				send_buff_by_del();
				return true;
			}
			break;
			default:
				return game_event_handler::on_event(m_owner_index, trigger_param, m_buff_template_ptr->EventCondition);
			}
		}
		return false;
	}
	void buff_item::buff_event_param(bool apply)
	{
		if (nullptr == m_buff_template_ptr)
		{
			return;
		}
		change_event_param(m_owner_index, m_env.exenv.skill_template_id, m_env.exenv.skill_cd, apply, m_buff_template_ptr->EventCondition);
	}

	void buff_item::trigger_event(int32 target_index, bool is_add)
	{
		if (is_add)
		{//触发做个标记
			m_trigger_event = true;
		}
		if (m_trigger_event == false || is_add == false)
		{//如果没有触发过，就不做删除操作
			return;
		}
		m_env.pbuff = this;
		m_env.apply = is_add;
		m_env.nevent = 1;
		m_env.neventrecever = target_index;
		m_env.neventsender = m_owner_index;
		for (int32 nloopcount = 0; nloopcount < m_event_action_num; nloopcount++)
		{
			effect& effect_ref = m_event_action[nloopcount];
			if (effect_ref.action.is_condition_right(m_env))
			{
				effect_ref.action(m_env);
			}
		}
	}

	bool buff_item::is_zlzz()
	{
		return m_env.exenv.skill_template_id == passive_skill_zlzz_id;
	}
	void buff_item::send_show()
	{
		player& player_ref = unit_man::get_player(m_sender_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		skill_proto_trigger_passive_skill item_msg;
		item_msg.set_skill_template_id(m_env.exenv.skill_template_id);
		player_ref.send_message_to_self(&item_msg, e_msgindex_s2c_trigger_passive_skill);
	}
	void buff_item::set_buff_info(EBuffInfo e_buff_info, int32 buff_value)
	{
		if (e_buff_info < EBuffInfo_template_id || e_buff_info >= EBuffInfo_max)
		{
			return;
		}
		m_buff_data.set_data_ary(e_buff_info, buff_value);
	}
	int32 buff_item::get_buff_info(EBuffInfo e_buff_info)
	{
		if (e_buff_info < EBuffInfo_template_id || e_buff_info >= EBuffInfo_max)
		{
			return 0;
		}
		return m_buff_data.data_ary(e_buff_info);
	}
	void buff_item::init_data(const int32& owner_index, const int32& sender_index, const guid_64& buff_guid, const int32& buff_template_id, const int32& skill_template_id, const bool& is_ignore_att, const int32& effect_index, int32 buff_level)
	{
		m_env.apply = true;
		m_is_ignore_att = is_ignore_att;
		load_from_template(buff_template_id, skill_template_id, effect_index);
		set_sender_index(sender_index);
		set_owner_index(owner_index);
		set_buff_guid(buff_guid);
		set_buff_info(EBuffInfo_pile_count, buff_level);
		unit& unit_ref = unit_man::get_unit(m_sender_index);
		m_buff_data.set_buff_guid(buff_guid.server_64);
		m_buff_data.set_sender_guid(unit_ref.get_unit_guid().server_64);
	}

	bool buff_item::load_from_template(const int32& buff_template_id, const int32& skill_template_id, int32 effect_index)
	{
		m_buff_template_ptr = GET_TEMPLATE(BuffDataTemplate, buff_template_id);

		if (nullptr == m_buff_template_ptr)
		{
			return false;
		}
		set_buff_info(EBuffInfo_template_id, buff_template_id);
		set_buff_info(EBuffInfo_load_type, m_buff_template_ptr->SaveType);
		set_buff_info(EBuffInfo_time, m_buff_template_ptr->DuringTime * 1000);
		set_buff_info(EBuffInfo_series, m_buff_template_ptr->Series);
		set_buff_info(EBuffInfo_skill_template_id, skill_template_id);
		set_buff_info(EBuffInfo_effect_index, effect_index);
		set_buff_time(init_unit::get_end_time(m_buff_template_ptr->DuringTime));

		uint32 idx = 0;
		for (idx = 0; idx < m_buff_template_ptr->BeginActions.size(); ++idx)
		{
			if (m_buff_template_ptr->BeginActions[idx] <= 0)
			{
				continue;
			}
			BuffActionTemplate* buff_action_ptr = GET_TEMPLATE(BuffActionTemplate, m_buff_template_ptr->BeginActions[idx]);
			if (buff_action_ptr)
			{
				if (m_is_ignore_att
					&& buff_action_ptr->Action == "modatt")
				{
					//这个Buff在创建时设置了“忽略属性”并且此Action就是要更改属性时，忽略此操作
				}
				else
				{
					effect	eff;
					eff.action.set_template(buff_action_ptr);
					m_begin_action.push_back(eff);
					++m_begin_action_num;
				}
			}
		}

		for (idx = 0; idx < m_buff_template_ptr->CycActions.size(); ++idx)
		{
			if (m_buff_template_ptr->CycActions[idx] <= 0)
			{
				continue;
			}
			BuffActionTemplate* buff_action_ptr = GET_TEMPLATE(BuffActionTemplate, m_buff_template_ptr->CycActions[idx]);
			if (buff_action_ptr)
			{
				if (m_is_ignore_att
					&& buff_action_ptr->Action == "modatt")
				{
					//这个Buff在创建时设置了“忽略属性”并且此Action就是要更改属性时，忽略此操作
				}
				else
				{
					effect_cyc	eff_cyc;
					if (get_buff_time() != 0)
					{
						if (m_buff_template_ptr->JumpTime > 0)
						{
							eff_cyc.m_lanuch_type = ecalt_lanuch_interval;
							eff_cyc.m_interval = m_buff_template_ptr->JumpTime;
						}
						else
						{
							eff_cyc.m_lanuch_type = ecalt_lanuch_timepoint;
							eff_cyc.m_interval = m_buff_template_ptr->JumpTime;
							eff_cyc.m_interval = -eff_cyc.m_interval;
						}
						eff_cyc.action.set_template(buff_action_ptr);
						m_cyc_effect.push_back(eff_cyc);
						++m_cyc_action_num;
					}
				}
			}
		}

		for (idx = 0; idx < m_buff_template_ptr->EffectActions.size(); ++idx)
		{
			if (m_buff_template_ptr->EffectActions[idx] <= 0)
			{
				continue;
			}
			BuffActionTemplate* buff_action_ptr = GET_TEMPLATE(BuffActionTemplate, m_buff_template_ptr->EffectActions[idx]);
			if (buff_action_ptr)
			{
				if (m_is_ignore_att
					&& buff_action_ptr->Action == "modatt")
				{
					//这个Buff在创建时设置了“忽略属性”并且此Action就是要更改属性时，忽略此操作
				}
				else
				{
					effect	eff;
					eff.action.set_template(buff_action_ptr);
					m_effect_action.push_back(eff);
					++m_effect_action_num;
				}
			}
		}

		for (idx = 0; idx < m_buff_template_ptr->EndActions.size(); ++idx)
		{
			if (m_buff_template_ptr->EndActions[idx] <= 0)
			{
				continue;
			}
			BuffActionTemplate* buff_action_ptr = GET_TEMPLATE(BuffActionTemplate, m_buff_template_ptr->EndActions[idx]);
			if (buff_action_ptr)
			{
				if (m_is_ignore_att
					&& buff_action_ptr->Action == "modatt")
				{
					//这个Buff在创建时设置了“忽略属性”并且此Action就是要更改属性时，忽略此操作
				}
				else
				{
					effect	eff;
					eff.action.set_template(buff_action_ptr);
					m_end_action.push_back(eff);
					++m_end_action_num;
				}
			}
		}
		for (idx = 0; idx < m_buff_template_ptr->EventActions.size(); ++idx)
		{
			if (m_buff_template_ptr->EventActions[idx] <= 0)
			{
				continue;
			}
			BuffActionTemplate* buff_action_ptr = GET_TEMPLATE(BuffActionTemplate, m_buff_template_ptr->EventActions[idx]);
			if (buff_action_ptr)
			{
				if (m_is_ignore_att
					&& buff_action_ptr->Action == "modatt")
				{
					//这个Buff在创建时设置了“忽略属性”并且此Action就是要更改属性时，忽略此操作
				}
				else
				{
					effect	eff;
					eff.action.set_template(buff_action_ptr);
					m_event_action.push_back(eff);
					++m_event_action_num;
				}
			}
		}
		return true;
	}

	int32 buff_item::dec_pile()
	{
		if (get_cur_pile_count() > 1)
		{
			change_cur_pile_count(-1);
			send_buff_by_inf();
			return buff_ret_true;
		}
		end_buff(NULL);
		return buff_ret_kill;
	}

	void buff_item::dec_all_pile(bool time_over)
	{
		end_buff(NULL, time_over);
	}

	void buff_item::pile_one(buff_ex_env_param* penv, bool change_level)
	{
		if (nullptr == m_buff_template_ptr)
		{
			return;
		}
		set_buff_time(init_unit::get_end_time(m_buff_template_ptr->DuringTime));
		if (m_buff_template_ptr && m_buff_template_ptr->JumpTime > 0)
		{
			m_jump_tick = m_buff_template_ptr->JumpTime * 1000;
			m_last_action_tick = utility::get_tick_count() + m_jump_tick;
		}
		bool be_end = true;
		if (change_level)
		{
			if (get_cur_pile_count() < get_max_pile_count())
			{
				end_buff(nullptr);
				m_is_end = false;
				change_cur_pile_count(1);
			}
			else
			{
				be_end = false;
			}
		}
		if (be_end)
		{
			if (penv)
			{
				m_env.exenv = *penv;
				if (m_buff_template_ptr->JumpTime > 0)
				{
					m_env.exenv.by_client = false;
					penv->by_client = false;
				}
			}
			begin_buff(penv);
			if (get_cur_pile_count() == 1)
			{
				send_buff_by_add(penv);
			}
			else
			{
				send_buff_by_inf();
			}
		}
	}
	float buff_item::get_buff_percent()
	{
		float percent_value = get_buff_info(EBuffInfo_percent) / 100.f;
		return percent_value;
	}
	s_buff_info buff_item::get_fake_buff_info_inst()
	{
		s_buff_info buff_info;
		buff_info.buff_guid = get_buff_guid();
		for (int32 i = 0; i < EBuffInfo_max; ++i)
		{
			if (i < m_buff_data.data_ary_size())
			{
				buff_info.data_ary[i] = m_buff_data.data_ary(i);
			}
		}
		return buff_info;
	}
	void buff_item::check_need_update()
	{
		if (is_empty())
		{
			return;
		}
		if (m_buff_template_ptr->NeedRefresh == 0)
		{
			return;
		}
		send_buff_by_inf();
	}
	void buff_item::change_buff_percent(const float& percent_value)
	{
		int32 add_percent = percent_value * 100;
		int32 old_percent = get_buff_info(EBuffInfo_percent);
		set_buff_info(EBuffInfo_percent, old_percent + add_percent);
	}
	void buff_item::change_life_time(const float& during_time_fix)
	{
		m_life_time = init_unit::get_end_time(during_time_fix);
		send_buff_by_inf();
	}

	void buff_item::add_life_time(const float& during_time_fix)
	{
		int32 mill_sec = during_time_fix * 1000;
		m_life_time += mill_sec;
		send_buff_by_inf();
	}

	int32 buff_item::heart_tick(const int64& new_time)
	{
		if (nullptr == m_buff_template_ptr)
		{
			return buff_ret_kill;
		}

		cyc_buff(new_time);

		if (m_buff_template_ptr->DuringTime < -0.5)
		{
			return buff_ret_keep;
		}
		if (new_time >= get_buff_time())
		{
			return buff_ret_kill;
		}
		return buff_ret_keep;
	}
	bool buff_item::begin_effect(buff_ex_env_param* penv)
	{
		if (false == can_use())
		{
			return false;
		}
		if (penv)
		{
			m_env.exenv = *penv;
		}
		m_env.pbuff = this;
		m_env.apply = true;
		for (int32 nloopcount = 0; nloopcount < m_effect_action_num; nloopcount++)
		{
			effect& effect_ref = m_effect_action[nloopcount];
			if (effect_ref.action.is_condition_right(m_env))
			{
				if (effect_ref.action(m_env) == 0)
				{
					effect_ref.effected = false;
					m_is_send_client = false;
				}
				else
				{
					effect_ref.effected = true;
				}
			}
			else
			{
				effect_ref.effected = false;
				m_is_send_client = false;
			}
		}

		return true;
	}

	bool buff_item::end_effect(buff_ex_env_param* penv)
	{
		if (false == can_use())
		{
			return false;
		}
		m_env.apply = false;
		if (penv)
		{
			m_env.exenv = *penv;
		}
		m_env.pbuff = this;

		for (int32 nloopcount = 0; nloopcount < m_effect_action_num; nloopcount++)
		{
			effect& effect_ref = m_effect_action[nloopcount];
			if (effect_ref.action.is_auto() && effect_ref.effected)
			{
				effect_ref.action(m_env);
			}
		}
		return true;
	}

	bool buff_item::begin_buff(buff_ex_env_param* penv)
	{
		if (m_buff_template_ptr == nullptr)
		{
			return false;
		}

		begin_effect(penv);
		if (penv && penv->reload)
		{
			return true;
		}
		if (penv)
		{
			m_env.exenv = *penv;
		}
		if (can_use())
		{
			buff_event_param(true);
			m_env.pbuff = this;
			m_env.apply = true;
			for (int32 nloopcount = 0; nloopcount < m_begin_action_num; nloopcount++)
			{
				effect& effect_ref = m_begin_action[nloopcount];
				if (effect_ref.action.is_condition_right(m_env))
				{
					effect_ref.action(m_env);
				}

			}
		}
		return true;
	}

	bool buff_item::cyc_buff(const int64& new_time)
	{
		if (m_jump_tick <= 0 || new_time < m_last_action_tick)
		{
			return false;
		}
		if (can_use() == false)
		{
			return false;
		}
		m_last_action_tick = new_time + m_jump_tick;
		m_env.pbuff = this;

		for (int32 nloopcount = 0; nloopcount < m_cyc_action_num; nloopcount++)
		{
			effect_cyc& effect_cyc_ref = m_cyc_effect[nloopcount];
			if (effect_cyc_ref.action.is_condition_right(m_env))
			{
				effect_cyc_ref.action(m_env);
			}
		}
		cyc_buff_show();
		return true;
	}
	void buff_item::cyc_buff_show()
	{
		if (m_buff_template_ptr->DuringTime >= -0.5)
		{
			return;
		}
		if (m_buff_template_ptr->EventCondition[0] != 0)
		{
			return;
		}
		if (m_env.exenv.skill_template_id < passive_skill_start || m_env.exenv.skill_template_id > passive_skill_end)
		{
			return;
		}
		if (m_is_send_one)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_sender_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		skill_proto_trigger_passive_skill item_msg;

		item_msg.set_skill_template_id(m_env.exenv.skill_template_id);
		player_ref.send_message_to_self(&item_msg, e_msgindex_s2c_trigger_passive_skill);
		if (m_jump_tick < jump_time_limit)
		{
			m_is_send_one = true;
		}
	}
	bool buff_item::end_buff(buff_ex_env_param* penv, bool time_over)
	{
		if (m_is_end)
		{
			return true;
		}
		m_is_end = true;
		end_effect(penv);
		if (penv)
		{
			m_env.exenv = *penv;
		}
		if (false == time_over)
		{
			send_buff_by_del();
		}
		if (can_use())
		{
			m_env.pbuff = this;
			m_env.apply = true;
			for (int32 nloopcount = 0; nloopcount < m_end_action_num; nloopcount++)
			{
				effect& effect_ref = m_end_action[nloopcount];
				if (effect_ref.action.is_condition_right(m_env))
					effect_ref.action(m_env);
			}
			buff_event_param(false);
		}
		return true;
	}

	void buff_item::send_buff_by_add(buff_ex_env_param* penv)
	{
		if (!is_sync() || m_is_send_client == false)
			return;

		unit& recver_unit = unit_man::get_unit(get_owner_index());
		buff_proto_buff_operation_end badd;
		badd.set_operation_type(e_buff_operation_type_add);
		if (penv && penv->last_chain_unit.is_valid())
		{
			badd.set_sender_guid(penv->last_chain_unit.server_64);
		}
		else
		{
			unit& sender_unit = unit_man::get_unit(get_sender_index());
			badd.set_sender_guid(sender_unit.get_unit_guid().server_64);
		}
		badd.set_target_guid(recver_unit.get_unit_guid().server_64);
		badd.set_buff_guid(get_buff_guid().server_64);
		badd.set_template_id(get_buff_template_id());
		badd.set_effect_index(get_buff_info(EBuffInfo_effect_index));
		if (m_buff_template_ptr->DuringTime != -1)
		{
			int32 last_time = get_buff_time() - utility::get_tick_count();
			badd.set_time(last_time > 0 ? last_time : 0);
		}
		else
		{
			badd.set_time(-1000);
		}
		if (get_cur_pile_count() > 1)
		{
			badd.set_pile_count(get_cur_pile_count());
		}
		if (get_skill_template_id() > 0)
		{
			badd.set_skill_template_id(get_skill_template_id());
		}
		if (recver_unit.is_valid())
		{
			if (m_buff_template_ptr->OtherPlayerCanSee > 0)
			{
				if (m_buff_template_ptr->Series == 90000
					|| m_buff_template_ptr->Series == 90001
					|| m_buff_template_ptr->Series == 90002
					|| m_buff_template_ptr->Series == 90003
					|| recver_unit.get_cur_map_type() == e_map_type_attack_city)
				{//坐骑和翅膀相关的buff还是要全部广播
					recver_unit.send_message_to_aoi(&badd, e_msgindex_s2c_buff_operation, true, false);
				}
				else
				{
					recver_unit.send_message_to_aoi(&badd, e_msgindex_s2c_buff_operation, true, true);
				}
			}
			else
			{
				recver_unit.send_message_to_self(&badd, e_msgindex_s2c_buff_operation);
			}
		}
	}

	void buff_item::send_buff_by_del()
	{
		if (!is_sync() || m_is_send_client == false)
			return;

		unit& unit_ptr = unit_man::get_unit(get_owner_index());

		buff_proto_buff_operation_end bdel;
		bdel.set_operation_type(e_buff_operation_type_del);
		bdel.set_target_guid(unit_ptr.get_unit_guid().server_64);
		bdel.set_buff_guid(get_buff_guid().server_64);
		bdel.set_template_id(get_buff_template_id());
		if (unit_ptr.is_valid())
		{
			if (m_buff_template_ptr->OtherPlayerCanSee > 0)
			{
				unit_ptr.send_message_to_aoi(&bdel, e_msgindex_s2c_buff_operation);
			}
			else
			{
				unit_ptr.send_message_to_self(&bdel, e_msgindex_s2c_buff_operation);
			}
		}

	}

	void buff_item::send_buff_by_inf(const int32& recv_id)
	{
		if (!is_sync())
			return;

		unit& recver_unit = unit_man::get_unit(get_owner_index());

		buff_proto_buff_operation_end bain;
		bain.set_operation_type(e_buff_operation_type_info);
		if (m_env.exenv.last_chain_unit.is_valid())
		{
			bain.set_sender_guid(m_env.exenv.last_chain_unit.server_64);
		}
		else
		{
			unit& sender_unit = unit_man::get_unit(get_sender_index());
			bain.set_sender_guid(sender_unit.get_unit_guid().server_64);
		}

		if (bain.sender_guid() != recver_unit.get_unit_guid().server_64)
		{
			bain.set_target_guid(recver_unit.get_unit_guid().server_64);
		}
		bain.set_buff_guid(get_buff_guid().server_64);
		bain.set_template_id(get_buff_template_id());
		bain.set_effect_index(get_buff_info(EBuffInfo_effect_index));
		if (m_buff_template_ptr->DuringTime != -1)
		{
			int32 last_time = get_buff_time() - utility::get_tick_count();
			bain.set_time(last_time > 0 ? last_time : 0);
		}
		else
		{
			bain.set_time(-1000);
		}
		if (get_cur_pile_count() > 1)
		{
			bain.set_pile_count(get_cur_pile_count());
		}
		if (recver_unit.is_valid())
		{
			if (recv_id == 0)
			{

				if (m_buff_template_ptr->OtherPlayerCanSee > 0)
				{
					recver_unit.send_message_to_aoi(&bain, e_msgindex_s2c_buff_operation);
				}
				else
				{
					recver_unit.send_message_to_self(&bain, e_msgindex_s2c_buff_operation);
				}
			}
			else
			{
				unit& recv_unit = unit_man::get_unit(recv_id);
				if (recv_unit.is_valid())
				{
					recv_unit.send_message_to_self(&bain, e_msgindex_s2c_buff_operation);
				}
			}

		}
	}
	void buff_item::get_buff_info_msg(buff_proto_buff_info_all& buff_info_all_msg)
	{
		buff_proto_buff_info* buff_info_msg = buff_info_all_msg.add_buff_info();
		*buff_info_msg = m_buff_data;
		if (m_buff_template_ptr->DuringTime != -1)
		{
			int32 last_time = get_buff_time() - utility::get_tick_count();
			buff_info_msg->set_data_ary(EBuffInfo_time, last_time > 0 ? last_time : 0);
		}
		else
		{
			buff_info_msg->set_data_ary(EBuffInfo_time, -1000);
		}
	}

	void buff_item::save(buff_proto_buff_save_load& request_save)
	{
		set_buff_info(EBuffInfo_time, m_life_time - utility::get_tick_count());
		buff_proto_buff_info* buff_info = request_save.add_data_ary();
		*buff_info = m_buff_data;
	}

	bool buff_item::load(const buff_proto_buff_info& buff_data)
	{
		m_buff_data = buff_data;
		m_life_time = get_buff_info(EBuffInfo_time) + utility::get_tick_count();
		return true;
	}
	bool buff_item::can_use()
	{
		player& owner_player = unit_man::get_player(m_owner_index);
		if (owner_player.is_valid()
			&& owner_player.m_cur_map_template_ptr != nullptr)
		{
			if (m_buff_template_ptr->InvalidMapTypeArray.empty() == false)
			{
				for (int32 i = 0; i < m_buff_template_ptr->InvalidMapTypeArray.size(); ++i)
				{
					if (m_buff_template_ptr->InvalidMapTypeArray[i] == owner_player.m_cur_map_template_ptr->Type)
					{
						return false;
						break;
					}
				}
			}
			if (m_buff_template_ptr->ValidMapTypeArray.empty() == false)
			{
				for (int32 i = 0; i < m_buff_template_ptr->ValidMapTypeArray.size(); ++i)
				{
					if (m_buff_template_ptr->ValidMapTypeArray[i] == owner_player.m_cur_map_template_ptr->Type)
					{
						return true;
						break;
					}
				}
				return false;
			}
		}
		return true;
	}
	bool buff_item::can_cannel()
	{
		if (is_empty())
		{
			return false;
		}

		return m_buff_template_ptr->CanCannel != 0;
	}
	int32 buff_item::get_buff_template_id()
	{
		return get_buff_info(EBuffInfo_template_id);
	}
	int32  buff_item::get_skill_template_id()
	{
		return get_buff_info(EBuffInfo_skill_template_id);
	}
	int64& buff_item::get_buff_time()
	{
		return m_life_time;
	}
	void buff_item::set_buff_time(int64 buff_time)
	{
		m_life_time = buff_time;
	}
	void buff_item::add_buff_time(int64 buff_time)
	{
		m_life_time += buff_time;
	}
	int32 buff_item::get_type()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->Type;
	}
	int32 buff_item::get_level()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->Level;
	}
	int32
		buff_item::get_race()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->Race;
	}
	int32
		buff_item::get_max_pile_count()
	{
		if (is_empty())
		{
			return 1;
		}
		return m_buff_template_ptr->MaxPileDepth > 0 ? m_buff_template_ptr->MaxPileDepth : 1;
	}

	int32 buff_item::get_cur_pile_count()
	{
		return get_buff_info(EBuffInfo_pile_count);
	}

	void buff_item::change_cur_pile_count(int32 pile_count)
	{
		int32 old_count = get_buff_info(EBuffInfo_pile_count);
		set_buff_info(EBuffInfo_pile_count, old_count + pile_count);
	}

	int32 buff_item::get_save_type()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->SaveType;
	}

	int32 buff_item::is_pile_spec()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->MaxPileDepth <= 1;
	}

	int32 buff_item::is_sync()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->Sync;
	}

	int32 buff_item::is_dead_del()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->ExistWhenDeath;
	}

	int32 buff_item::is_offline()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->SaveType == e_buff_svt_save_load;
	}
	int32 buff_item::is_map_end()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->MapEndClear;
	}
	bool buff_item::is_keep()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->TimeEndKeep;
	}
	bool buff_item::is_empty()
	{
		return (m_buff_template_ptr == NULL);
	}
	uint32 buff_item::get_series()
	{
		if (is_empty())
		{
			return 0;
		}
		return m_buff_template_ptr->Series;
	}
	int32 buff_item::is_same_level_cover()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->SameLevelCover;
	}

	int32 buff_item::is_diff_level_cover()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->DiffLevelCover;
	}

	int32 buff_item::is_same_level_same_sender_wrap()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->SameLevelSameSenderWrap;
	}

	int32 buff_item::is_same_level_diff_sender_wrap()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->SameLevelDifSenderWrap;
	}

	int32 buff_item::is_same_level_time_cumulative()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->SameLevelTimeCumulative;
	}
	int32 buff_item::is_diff_level_time_cumulative()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->DiffevelTimeCumulative;
	}
	int32 buff_item::is_del_when_reset_skill()
	{
		if (is_empty())
		{
			return false;
		}
		return m_buff_template_ptr->DelByResetSkill;
	}
	e_buff_add_judge buff_item::add_buff_type(const int32& sender_index, const int32& buff_template_id)
	{
		BuffDataTemplate* buff_template_ptr = GET_TEMPLATE(BuffDataTemplate, buff_template_id);
		if (nullptr == buff_template_ptr || nullptr == m_buff_template_ptr)
		{
			return e_buff_add_judge_non;
		}
		if (buff_template_ptr->Level == m_buff_template_ptr->Level)
		{
			if (buff_template_ptr->SameLevelCover)
			{
				return e_buff_add_judge_can_cover;
			}
			else
			{
				if (false == FLOAT_IS_ZERO(buff_template_ptr->BuffPercent))
				{
					return e_buff_add_judge_can_percent;
				}
				if (sender_index == get_sender_index())
				{
					if (buff_template_ptr->SameLevelSameSenderWrap)
					{
						return e_buff_add_judge_can_wrap;
					}
					else if (buff_template_ptr->SameLevelTimeCumulative)
					{
						return e_buff_add_judge_can_cumulative;
					}
				}
				else
				{
					if (buff_template_ptr->SameLevelDifSenderWrap)
					{
						return e_buff_add_judge_can_wrap;
					}
					else if (buff_template_ptr->DiffevelTimeCumulative)
					{
						return e_buff_add_judge_can_cumulative;
					}
				}
			}
			//又不能叠又不能续的
			return e_buff_add_judge_has_it;
		}
		else
		{
			if (buff_template_ptr->DiffLevelCover)
			{
				if (m_buff_template_ptr->Level >= buff_template_ptr->Level)
				{//当前的比新的小，还不让加，就说 已有了
					return e_buff_add_judge_has_it;
				}
				else
				{//当前的比新的大，就说 现在的更屌
					return e_buff_add_judge_can_cover;
				}
			}
			else
			{
				if (buff_template_ptr->DiffevelTimeCumulative)
				{
					return e_buff_add_judge_can_cumulative;
				}
				else
				{
					return e_buff_add_judge_non;
				}
			}
		}
	}
}
