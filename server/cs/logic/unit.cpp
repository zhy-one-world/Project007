/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 20:18
  file base: unit
  file ext: cpp
  author: zhy
  
  purpose: 
*********************************************************************/

/************************************************************************/
/*                             File Include                             */
/************************************************************************/

#include "cell_server.hpp"
#include "connection_mgr.hpp"
#include "utility/init_unit.h"
#include "internet/character.pb.h"
#include "internet/net.pb.h"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/skill_manager.h"
#include "logic/unit.hpp"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "npc.hpp"
#include "utility/random.h"
#include "internet/talent.pb.h"

namespace faith
{
	/************************************************************************/
	/*                           Class Implement                            */
	/************************************************************************/
	unit::unit()
	{
		m_array_index = 0;
	}

	unit::~unit()
	{
	}
	void unit::clear_data()
	{
		buff_man::del_all_buff(get_array_index(), get_array_index());
		skill_manager::remove_unit_skill(get_array_index());
		m_is_data_use = false;
		m_array_index = -1;
		m_unit_type = e_unit_type_null;
		m_unit_sub_type = 0;
		m_line_id = 0;
		m_unit_capsule = 0;
		m_kill_array_index = 0;
		m_be_critical = false;
		m_unit_identifier.clear_data();
		memset(&m_role_info, 0, sizeof(m_role_info));
		m_unit_half_height = 0;
		memset(m_role_guid_str, 0, sizeof(m_role_guid_str));
		get_item_set().clear_data();
		m_pk_community_mgr.clear_data();
		m_legion_cs_mgr.clear_data();
		m_team_cs_mgr.clear_data();
		m_unit_move_mgr.clear_data();
		m_new_map_pos.clear_data();
		m_old_map_pos.clear_data();
		m_time_stamp = 0;
		m_map_ent = nullptr;
		m_role_info.clear_data();
		m_mount_ammo_flag = false;
		m_safe_point_vec_len = 0;
		m_safe_point_vec = nullptr;
		memset(m_addition_buff, 0, sizeof(m_addition_buff));
		m_pawn_att.clear_data();
		m_cur_map_template_ptr = nullptr;
		event_processer* event_processer_ptr = event_manager::get_instance().get_event_processer(m_array_index);
		if (event_processer_ptr)
		{
			event_processer_ptr->clear_data();
		}
		m_move_map_guid.clear_data();
		m_last_attack_idex = -1;
		item_system::clear_data(this);
	}
	void unit::set_array_index(int32 array_index)
	{
		m_array_index = array_index;
		m_pawn_att.set_unit_ptr(array_index);
		m_pk_community_mgr.set_unit_array_index(array_index);
		m_legion_cs_mgr.set_unit_array_index(array_index);
		m_team_cs_mgr.set_unit_array_index(array_index);
		m_war_state_mgr.set_unit_ptr(array_index);
		m_unit_move_mgr.set_unit_ptr(m_array_index);
	}
	void unit::heart_tick(const int64& new_time, const int32& tick_time)
	{
		if (m_kill_array_index > 0)
		{
			be_dead();
			m_kill_array_index = 0;
			return;
		}
		event_processer* event_processer_ptr = event_manager::get_instance().get_event_processer(m_array_index);
		if (event_processer_ptr)
		{
			event_processer_ptr->heart_tick(new_time);
		}
		m_war_state_mgr.heart_tick(new_time);
		skill_manager::heart_tick(get_array_index(), new_time,tick_time);
		buff_man::heart_tick(get_array_index(), new_time);
		m_pawn_att.heart_tick(new_time);
		m_unit_move_mgr.heart_tick(new_time);
		m_pk_community_mgr.heart_tick(new_time);
	}

	std::vector<int32> unit::get_addition_buff_id_arr(e_addition_buff addition_buff_type)
	{
		std::vector<int32> buff_id_arr;
		if (addition_buff_type >= e_addition_buff_max)
		{
			return buff_id_arr;
		}
		for (int32 i = 0;i < max_each_type_addition_buff_num;i++)
		{
			if (m_addition_buff[addition_buff_type][i] != 0)
			{
				buff_id_arr.push_back(m_addition_buff[addition_buff_type][i]);
			}
		}
		return buff_id_arr;
	}

	void unit::add_addition_buff_id_arr(e_addition_buff addition_buff_type, int32 buff_id)
	{
		if (addition_buff_type >= e_addition_buff_max)
		{
			return;
		}
		for (int32 i = 0;i < faith::max_each_type_addition_buff_num;i++)
		{
			if (m_addition_buff[addition_buff_type][i] == 0)
			{
				m_addition_buff[addition_buff_type][i] = buff_id;
				break;
			}
		}
	}

	void unit::reset_addition_buff_id_by_type(e_addition_buff addition_buff_type)
	{
		for (int32 i = 0;i < faith::max_each_type_addition_buff_num;i++)
		{
			m_addition_buff[addition_buff_type][i] = 0;
		}
	}

	void unit::send_all_addition_buff_info(unit& temp_player)
	{
		character_proto_addition_buff_all addition_buff_info_res;

		addition_buff_info_res.add_role_guid(temp_player.get_unit_guid().A);
		addition_buff_info_res.add_role_guid(temp_player.get_unit_guid().B);
		for (int32 i = 0; i < e_addition_buff_max; ++i)
		{
			std::vector<int32> addtion_buff_id_arr = temp_player.get_addition_buff_id_arr((e_addition_buff)i);
			for (int32 j = 0;j < addtion_buff_id_arr.size();j++)
			{
				addition_buff_info_res.add_buff_id(addtion_buff_id_arr[j]);
			}
		}
		temp_player.send_message_to_self(&addition_buff_info_res, e_msgindex_s2c_addition_buff_all);
	}
	void unit::send_addition_buff_info(guid_64 guid, e_addition_buff addition_buff_type, int32 addition_buff_id)
	{
		character_proto_addition_buff addition_buff_info_res;

		addition_buff_info_res.add_role_guid(guid.A);
		addition_buff_info_res.add_role_guid(guid.B);
		addition_buff_info_res.set_addition_buff_type((int32)addition_buff_type);
		addition_buff_info_res.set_addition_buff_id(addition_buff_id);

		send_message_to_self(&addition_buff_info_res, e_msgindex_s2c_addition_buff);
	}

	void unit::send_addition_buff_info_arr(guid_64 guid, e_addition_buff addition_buff_type, std::vector<int32> addition_buff_id_arr)
	{
		character_proto_addition_buff_arr addition_buff_arr;
		addition_buff_arr.add_role_guid(guid.A);
		addition_buff_arr.add_role_guid(guid.B);
		addition_buff_arr.set_addition_buff_type((int32)addition_buff_type);
		for (int32 i = 0;i < addition_buff_id_arr.size();i++)
		{
			addition_buff_arr.add_addition_buff_id_arr(addition_buff_id_arr[i]);
		}

		send_message_to_self(&addition_buff_arr, e_msgindex_s2c_addition_buff_arr);
	}

	/************************************************************************/
	/*                          基础属性相关                                */
	/************************************************************************/
	void unit::set_unit_info_all(const s_unit_info& tempInfo)
	{
		memcpy(&m_role_info, &tempInfo, sizeof(s_unit_info));
		m_new_map_pos.unit_location.x = m_role_info.data_ary[e_role_info_move_pos_x];
		m_new_map_pos.unit_location.y = m_role_info.data_ary[e_role_info_move_pos_y];
		m_new_map_pos.unit_location.z = m_role_info.data_ary[e_role_info_move_pos_z];
		m_new_map_pos.unit_rotation.yaw = m_role_info.data_ary[e_role_info_move_pos_yaw];
		m_old_map_pos = m_new_map_pos;
		m_unit_identifier.runtime_id = get_array_index();
		m_unit_identifier.unit_guid = m_role_info.role_guid;
		m_safe_point_vec = globle_data::get_instance().get_map_save_point_ary(m_role_info.data_ary[e_role_info_move_map_id]);
		if (m_safe_point_vec)
		{
			m_safe_point_vec_len = (*m_safe_point_vec).size();
		}

		m_cur_map_template_ptr = GET_TEMPLATE(MapTemplate, m_role_info.data_ary[e_role_info_move_map_id]);
		xstring guid_str = m_role_info.role_guid.to_string();
		memcpy(m_role_guid_str, guid_str.c_str(), guid_str.size() > max_unit_guid_str_max ? max_unit_guid_str_max : guid_str.size());
	}
	int64 unit::get_unit_i64_info_data(e_role_i64_info eType)
	{
		if (eType < 0 || eType >= e_role_i64_info_max)
		{
			return -1;
		}
		return  m_role_info.data_i64_ary[eType];
	}
	void unit::set_unit_i64_info_data(int32 idex, int64 gs_value)
	{
		if (idex < 0 || idex >= e_role_i64_info_max)
		{
			return;
		}
		//if (idex == e_role_i64_info_gs_value)
		//{
		//	m_role_info.data_ary[e_role_info_GS_value] = gs_value;
		//}
		m_role_info.data_i64_ary[idex] = gs_value;
	}
	void unit::set_unit_info(e_role_info eType, int32 iValue)
	{
		if (eType >= e_role_info_max)
		{
			return;
		}
		m_role_info.data_ary[eType] = iValue;
	}
	void unit::set_new_map_pos(const s_map_pos& map_pos)
	{
		//aoi_system::update_object(get_map_guid(), get_old_map_pos().unit_location, map_pos.unit_location, m_array_index);
		m_new_map_pos = map_pos;
		set_move_pos(map_pos);
		if (m_cur_map_template_ptr == nullptr)
		{
			return;
		}
		if (m_cur_map_template_ptr->Type == faith::e_map_type_big_map && m_status == e_session_status_in_gaming)
		{
			set_main_pos(map_pos);
		}
	}
	void unit::set_new_map_angle(const int32& map_angel)
	{
		m_new_map_pos.unit_rotation.yaw = map_angel;
		m_role_info.data_ary[e_role_info_move_pos_yaw] = map_angel;
		MapTemplate* temp_map_template_ptr = GET_TEMPLATE(MapTemplate, m_role_info.data_ary[e_role_info_move_map_id]);
		if (temp_map_template_ptr == nullptr)
		{
			return;
		}
		if (temp_map_template_ptr->Type == faith::e_map_type_big_map)
		{
			m_role_info.data_ary[e_role_info_main_pos_yaw] = map_angel;
		}
	}
	void unit::set_move_pos(const s_map_pos& map_pos)
	{
		m_role_info.data_ary[e_role_info_move_pos_x] = map_pos.unit_location.x;
		m_role_info.data_ary[e_role_info_move_pos_y] = map_pos.unit_location.y;
		m_role_info.data_ary[e_role_info_move_pos_z] = map_pos.unit_location.z;
		m_role_info.data_ary[e_role_info_move_pos_yaw] = map_pos.unit_rotation.yaw;
	}
	void unit::set_main_pos(const s_map_pos& map_pos)
	{
		m_role_info.data_ary[e_role_info_main_pos_x] = map_pos.unit_location.x;
		m_role_info.data_ary[e_role_info_main_pos_y] = map_pos.unit_location.y;
		m_role_info.data_ary[e_role_info_main_pos_z] = map_pos.unit_location.z;
		m_role_info.data_ary[e_role_info_main_pos_yaw] = map_pos.unit_rotation.yaw;
	}
	s_map_pos unit::get_main_pos()
	{
		float pos_x = m_role_info.data_ary[e_role_info_main_pos_x];
		float pos_y = m_role_info.data_ary[e_role_info_main_pos_y];
		float pos_z = m_role_info.data_ary[e_role_info_main_pos_z];
		float yaw   = m_role_info.data_ary[e_role_info_main_pos_yaw];
		return s_map_pos(pos_x, pos_y, pos_z, 0, 0, yaw);
	}
	void unit::set_line_id(int32 line_id)
	{
		m_line_id = line_id;
		//m_role_info.data_ary[e_role_info_main_line_id] = line_id;
		m_role_info.data_ary[e_role_info_move_line_id] = line_id;
	}
	void unit::set_main_line_id(int32 line_id)
	{
		m_role_info.data_ary[e_role_info_main_line_id] = line_id;
	}
	int32 unit::get_main_line_id()
	{
		return m_role_info.data_ary[e_role_info_main_line_id];
	}
	void unit::set_war_index(int32 war_index)
	{
		m_war_index = war_index;
	}

	void unit::set_must_do_count(e_daily_must_do_typ eType, int32 iValue)
	{
		if (eType >= e_daily_must_do_typ_max)
		{
			return;
		}
		m_daily_must_do_count.data_ary[eType] = iValue;
	}


	bool unit::is_self_server(int32 unit_index)
	{
		if (0 == unit_index)
		{
			return cell_server::getInstance().get_is_self_server(get_unit_info(e_role_info_server_id));
		}
		else
		{
			unit& unit_ref = unit_man::get_unit(unit_index);
			if (false == unit_ref.is_valid())
			{
				return false;
			}
			else
			{
				return unit_ref.get_unit_info(e_role_info_server_id) == get_unit_info(e_role_info_server_id);
			}

		}
	}
	xstring unit::get_class_name()
	{
		return globle_data::get_instance().get_class_name_by_type(m_role_info.data_ary[e_role_info_class_type]);
	}
	/************************************************************************/
	/*                          战斗属性相关                                */
	/************************************************************************/
	void unit::send_warning(e_warning_type warn_type, int32 warn_value, bool warn_show)
	{
		faith::character_proto_character_warning warn_msg;
		warn_msg.set_unit_guid(get_unit_guid().server_64);
		warn_msg.set_warn_type(warn_type);
		warn_msg.set_warn_value(warn_value);
		warn_msg.set_warn_show(warn_show);
		send_message_to_aoi(&warn_msg, faith::e_msgindex_s2c_character_warning);
	}
	bool	unit::is_dead()
	{
		return m_pawn_att.is_dead();
	}

	const s_map_block* unit::get_in_map_safe()
	{
		if (nullptr == m_safe_point_vec)
		{
			return nullptr;
		}
		const s_map_block* cur_map_block_ptr = nullptr;
		int32 priority = -1;
		for (int32 i = 0; i < m_safe_point_vec_len; ++i)
		{
			if (init_unit::is_in_safe((*m_safe_point_vec)[i].points, m_new_map_pos.unit_location) && ((*m_safe_point_vec)[i].priority > priority))
			{
				cur_map_block_ptr = &((*m_safe_point_vec)[i]);
				priority = (*m_safe_point_vec)[i].priority;
				break; //现在m_safe_point_vec是按照priority降序排的
			}
		}
		return cur_map_block_ptr;
	}

	bool unit::change_hp(double hp_value, int32 array_index, bool be_critical, int32 skill_template_id)
	{
		bool can_change_hp = true;
		if (m_pawn_att.get_be_hurt_type() == e_be_hurt_type_count)
		{
			if (hp_value > 0)
			{
				hp_value = 1;
			}
			else
			{
				hp_value = 0;
			}
		}
		else if (m_pawn_att.get_be_hurt_type() == e_be_hurt_type_self && array_index != m_array_index)//如果受伤害类型为自己且造成伤害的不为自己，则不执行后续掉血逻辑,且记录一下造成伤害的idex并加一下伤害buff
		{
			can_change_hp = false;
		}
		if (m_unit_type == e_unit_type_npc && m_unit_sub_type == e_npc_type_server_harry_chests)
		{
			return false;//雕像不应该会被扣血
		}
		if (hp_value > 0)
		{
			get_pk_community_mgr().on_player_be_attacked(array_index);
			get_pk_community_mgr().on_player_be_attacked_by_robot_player(array_index);
			if (array_index != m_array_index)
			{
				if (array_index >= npc_arrary_index_begin)
				{
					npc& npc_ref = unit_man::get_npc(array_index);
					if (npc_ref.is_valid())
					{
						player& player_ref = unit_man::get_player(npc_ref.get_owner());
						if (player_ref.is_valid())
						{
							m_last_attack_idex = player_ref.get_array_index();
						}
					}
				}
				else
				{
					m_last_attack_idex = array_index;
				}
			}
		}
		else
		{
			float no_back = 1 - get_pawn_att().get_game_att(e_unit_game_att_no_back_hp_percent);
			no_back = no_back < 0 ? 0 : no_back;
			hp_value *= no_back;
		}
		if (array_index >= npc_arrary_index_begin)
		{
			npc& npc_ref = unit_man::get_npc(array_index);
			if (npc_ref.is_valid())
			{
				player& player_ref = unit_man::get_player(npc_ref.get_owner());
				if (player_ref.is_valid() && npc_ref.get_map_guid() == player_ref.get_map_guid() && player_ref.check_can_receive_summon_damage() && npc_ref.get_assist_fight_guid().is_valid() == false && npc_ref.get_is_be_assist_fight_npc() == false)
				{
					array_index = player_ref.get_array_index();
				}
			}
		}
		double old_hp = m_pawn_att.get_unit_base_att(e_base_att_info_hp_cur);
		double all_hp = m_pawn_att.get_attack_att_value(e_unit_attack_att_hp_max);
		double new_hp = old_hp - hp_value;
		bool is_dead = false;

		if (new_hp <= 0 && can_change_hp)
		{
			if (m_pawn_att.get_state_att(e_unit_attack_state_will_dead))
			{
				//m_pawn_att.set_state_att(e_unit_attack_state_will_dead, false);
				m_pawn_att.set_unit_base_att(e_base_att_info_hp_cur, 1);
				trigger_unit_event(e_game_event_type_unit_will_dead);
				return false;
			}
			if (m_pawn_att.get_state_att(e_unit_attack_state_will_dead_percent))
			{
				if (random_gen::get_random(0, 100) <= m_pawn_att.get_state_percent(e_unit_attack_state_will_dead_percent))
				{
					m_pawn_att.set_unit_base_att(e_base_att_info_hp_cur, 1);
					trigger_unit_event(e_game_event_type_unit_will_dead);
					return false;
				}
			}
			new_hp = 0;

			if (get_pawn_att().get_state_att(e_unit_attack_state_no_dead) == false)
			{
				is_dead = true;
				if (array_index >= npc_arrary_index_begin)
				{
					npc& npc_ref = unit_man::get_npc(array_index);
					if (npc_ref.is_valid())
					{
						player& player_ref = unit_man::get_player(npc_ref.get_owner());
						if (player_ref.is_valid())
						{
							m_kill_array_index = player_ref.get_array_index();
						}
						else
						{
							m_kill_array_index = array_index;
						}
					}
				}
				else
				{
					m_kill_array_index = array_index;
				}
				if (m_pawn_att.get_be_hurt_type() == e_be_hurt_type_self)
				{
					m_kill_array_index = m_last_attack_idex;
				}
				m_kill_skill_template_id = skill_template_id;
				m_kill_dead_reason = e_be_dead_other;
				m_be_critical = be_critical;
				get_pawn_att().set_state_att(e_unit_attack_state_dead, true);
			}
			else
			{
				new_hp = 1;//不可死亡状态将血量强制设定为1，因为为0有可能导致无法对目标再造成伤害
			}
		}
		else if (new_hp > all_hp)
		{
			new_hp = all_hp;
		}
		if (can_change_hp)
		{
			hp_value = old_hp - new_hp;
			float old_hp_precent = old_hp / get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
			float new_hp_precent = new_hp / get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
			if (hp_value >= 0)
			{
				trigger_unit_event(e_game_event_type_unit_hp_down, new_hp_precent * 100, old_hp_precent * 100);
			}
			if (hp_value < 0)
			{
				trigger_unit_event(e_game_event_type_unit_hp_up, new_hp_precent * 100, old_hp_precent * 100);
			}
			m_pawn_att.set_unit_base_att(e_base_att_info_hp_cur, new_hp);
		}
		if (hp_value > 0)
		{
			be_attacked(array_index, hp_value);
		}
		if(is_dead)
		{
			trigger_unit_event(e_game_event_type_unit_dead, m_array_index);
		}

		if (get_unit_type() == faith::e_unit_type_monster)
		{
			player& attacker_ref = unit_man::get_player(array_index);
			if (attacker_ref.is_valid() == true)
			{
				int32 npc_id = get_unit_info(faith::e_role_info_template_id);
				attacker_ref.get_mission_mgr().target_check(e_mission_end_type_npc_attacked_num, npc_id);
			}
		}

		npc& cur_npc_ref = unit_man::get_npc(m_array_index);//判断是否需要给当前npc加自动掉血buff
		if (cur_npc_ref.is_valid() && array_index != m_array_index)
		{
			NpcTemplate* npc_temp_ptr = cur_npc_ref.get_npc_template();
			if (npc_temp_ptr != nullptr)
			{
				std::vector<int32> gs_add_buff_id = npc_temp_ptr->GsAddBuffId;
				if (gs_add_buff_id.size() > 0 && gs_add_buff_id.size() % 2 == 0)
				{
					int64 all_hate_player_gs_value = cur_npc_ref.get_ai_unit().get_all_hate_list_player_gs();
					int64 cur_npc_gs = npc_temp_ptr->NormNpcGs;
					float reduce_gs = all_hate_player_gs_value - cur_npc_gs;
					int32 buff_id = 0;
					for (int32 i = 0; i < gs_add_buff_id.size() / 2; i++)
					{
						float min_rate = gs_add_buff_id[(i * 2)] / 100;
						float min_temp_gs = cur_npc_gs * min_rate;
						if (reduce_gs >= min_temp_gs)
						{
							buff_id = gs_add_buff_id[(i * 2) + 1];
						}
					}
					if (buff_id > 0)
					{
						buff_man::add_buff_inst(m_array_index, m_array_index, buff_id);
					}
				}
			}
		}
		return is_dead;
	}
	void  unit::full_hp()
	{
		double hp = get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
		change_hp(-hp, get_array_index(), false, 0);
	}
	void unit::send_suck_blood_value(int32 suck_value)
	{
		if (0 >= suck_value)
		{
			return;
		}
		if (e_unit_type_player != get_unit_type())
		{
			return;
		}
		
		faith::talent_proto_suck_blood_value suck_pak;
		suck_pak.set_sender_guid(m_role_info.role_guid.server_64);
		suck_pak.set_suck_blood_value(suck_value);
		send_message_to_self(&suck_pak, e_msgindex_s2c_suck_blood_value);
	}

	float unit::get_left_hp_per()
	{
		double cur_hp = m_pawn_att.get_unit_base_att(e_base_att_info_hp_cur);
		double max_hp = m_pawn_att.get_attack_att_value(e_unit_attack_att_hp_max);
		return cur_hp / max_hp;
	}

	void unit::send_role_i64_value(int32 idex, bool is_to_aoi)
	{
		if (idex < 0 || idex >= e_role_i64_info_max)
		{
			return;
		}
		faith::character_proto_role_i64_info_one role_info_one_msg;
		role_info_one_msg.set_unit_guid(get_unit_guid().server_64);
		role_info_one_msg.set_info_type(idex);
		role_info_one_msg.set_info_value(m_role_info.data_i64_ary[idex]);
		if (is_to_aoi)
		{
			send_message_to_aoi(&role_info_one_msg, faith::e_msgindex_s2c_character_gs_value, true);
		}
		else
		{
			send_message_to_self(&role_info_one_msg, faith::e_msgindex_s2c_character_gs_value);
		}
	}

	void unit::send_info_one(e_role_info info_index, bool is_to_aoi)
	{
		if (info_index >= e_role_info_max)
		{
			return;
		}
		faith::character_proto_role_info_one role_info_one_msg;
		role_info_one_msg.set_unit_guid(get_unit_guid().server_64);
		role_info_one_msg.set_info_type(info_index);
		role_info_one_msg.set_info_value(m_role_info.data_ary[info_index]);
		if (is_to_aoi)
		{
			send_message_to_aoi(&role_info_one_msg, faith::e_msgindex_s2c_character_info_one, true);
		}
		else
		{
			send_message_to_self(&role_info_one_msg, faith::e_msgindex_s2c_character_info_one);
		}
	}

	void unit::send_info_all(bool is_refresh_by_zero, bool is_refresh_by_special_time)
	{
		faith::character_proto_unit_info_msg unit_info_msg;
		unit_info_msg.set_role_guid(get_unit_guid().server_64);
		unit_info_msg.set_role_name(m_role_info.role_name);
		unit_info_msg.set_role_appearance(m_role_info.role_appearance);
		unit_info_msg.set_is_refresh_by_zero(is_refresh_by_zero);
		unit_info_msg.set_is_refresh_by_special_time(is_refresh_by_special_time);
		for (int i = 0; i < e_role_info_max; ++i)
		{
			unit_info_msg.add_data_ary(m_role_info.data_ary[i]);
		}
		for (int32 i = 0; i < e_role_i64_info_max; i++)
		{
			unit_info_msg.add_data_str_ary(to_string(m_role_info.data_i64_ary[i]));
		}
		send_message_to_self(&unit_info_msg, faith::e_msgindex_s2c_character_info_all);
	}

	void unit::broadcast_info_one(e_role_info info_index)
	{
		if (info_index >= e_role_info_max)
		{
			return;
		}
		faith::character_proto_role_info_one role_info_one_msg;
		role_info_one_msg.set_unit_guid(get_unit_guid().server_64);
		role_info_one_msg.set_info_type(info_index);
		role_info_one_msg.set_info_value(m_role_info.data_ary[info_index]);
		send_message_to_aoi(&role_info_one_msg, faith::e_msgindex_s2c_broadcast_character_info_one);
	}

	void unit::send_unit_begin_info()
	{
		faith::character_proto_load_role_info_end load_role_info_end_msg;
		load_role_info_end_msg.set_runtime_id(get_array_index());
		character_proto_unit_info_msg* role_info_msg = load_role_info_end_msg.mutable_role_info();
		role_info_msg->set_role_guid(get_unit_guid().server_64);
		role_info_msg->set_role_name(m_role_info.role_name);
		role_info_msg->set_role_appearance(m_role_info.role_appearance);
		for (int32 i = 0; i < e_role_i64_info_max; i++)
		{
			role_info_msg->add_data_str_ary(to_string(m_role_info.data_i64_ary[i]));
		}		
		for (int i = 0; i < e_role_info_max; ++i)
		{
			role_info_msg->add_data_ary(m_role_info.data_ary[i]);
		}
		send_message_to_self(&load_role_info_end_msg, faith::e_msgindex_s2c_load_role_info);
	}
	
	guid_64& unit::get_legion_guid()
	{
		return get_legion_info().legion_guid;
	}

	bool unit::is_in_the_same_legion(unit& another_unit)
	{
		const guid_64& my_legion_guid = get_legion_guid();
		const guid_64& another_legion_guid = another_unit.get_legion_guid();
		if (!my_legion_guid.is_valid() || !another_legion_guid.is_valid())
		{
			return false;
		}
		return my_legion_guid == another_legion_guid;
	}

	guid_64& unit::get_team_guid()
	{
		return get_team_info().team_guid;
	}

	bool unit::is_in_same_team(unit& another_unit)
	{
		const guid_64& my_team_guid = get_team_guid();
		const guid_64& another_team_guid = another_unit.get_team_guid();
		if (!my_team_guid.is_valid() || !another_team_guid.is_valid())
		{
			return false;
		}

		return my_team_guid == another_team_guid;
	}

	void unit::send_daily_must_do_count_info_one(e_daily_must_do_typ info_index)
	{
		if (info_index >= e_daily_must_do_typ_max)
		{
			return;
		}

		faith::character_proto_daily_must_do_count daily_must_do_count;	
		daily_must_do_count.add_role_guid(get_unit_guid().A);
		daily_must_do_count.add_role_guid(get_unit_guid().B);
		daily_must_do_count.set_must_do_type(info_index);
		daily_must_do_count.set_must_do_value(m_daily_must_do_count.data_ary[info_index]);
		send_message_to_self(&daily_must_do_count, faith::e_msgindex_s2c_update_daily_must_do_count_one);
	}

	void unit::send_daily_must_do_count_info_all()
	{
		faith::character_proto_daily_must_do_count_all daily_must_do_count;
		daily_must_do_count.add_role_guid(get_unit_guid().A);
		daily_must_do_count.add_role_guid(get_unit_guid().B);

		for (int i = 0; i < e_daily_must_do_typ_cross_pk; i++)//发这个是因为客户端枚举不对应
		{
			daily_must_do_count.add_data_array(m_daily_must_do_count.data_ary[i]);
		}
		send_message_to_self(&daily_must_do_count, faith::e_msgindex_s2c_update_daily_must_do_count_all);


		daily_must_do_count.clear_data_array();
		for (int i = 0; i < e_daily_must_do_typ_max; i++)
		{
			daily_must_do_count.add_data_array(m_daily_must_do_count.data_ary[i]);
		}
		send_message_to_self(&daily_must_do_count, faith::e_msgindex_s2c_update_daily_must_do_count_all);
	}
	void unit::trigger_unit_event(int32 event_type, int64 param1, int64 param2, int64 param3)
	{
		event_processer* event_processer_ptr = event_manager::get_instance().get_event_processer(m_array_index);
		if (event_processer_ptr)
		{
			static s_trigger_param param;
			param.m_type = event_type;
			param.m_owner_index = get_array_index();
			param.m_event_param_1 = param1;
			param.m_event_param_2 = param2;
			param.m_event_param_3 = param3;
			event_processer_ptr->add_trigger_param(param);
		}
	}

	float unit::get_game_att(int32 att_index)
	{
		return get_pawn_att().get_game_att(att_index);
	}

	void unit::set_name(xstring role_name)
	{
		memset(m_role_info.role_name, 0, sizeof(m_role_info.role_name));
		memcpy(m_role_info.role_name, role_name.c_str(), role_name.size() > sizeof(m_role_info.role_name) ? sizeof(m_role_info.role_name) : role_name.size());
	}

	void unit::set_appearance(xstring role_apearance)
	{
		memset(m_role_info.role_appearance, 0, sizeof(m_role_info.role_appearance));
		memcpy(m_role_info.role_appearance, role_apearance.c_str(), role_apearance.size() > sizeof(m_role_info.role_appearance) ? sizeof(m_role_info.role_appearance) : role_apearance.size());
	}

}
