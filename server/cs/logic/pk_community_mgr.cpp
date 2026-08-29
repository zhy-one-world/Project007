/********************************************************************
created: 2016年8月9日13:56:28
file base: pk_mgr
file ext: cpp
author: zhangminghai

purpose: about pk and community system's store_mgr
*********************************************************************/

#include "cell_server.hpp"
#include "internet/character.pb.h"
#include "logic/buff_man.h"
#include "logic/npc.hpp"
#include "logic/player.hpp"
#include "Logic/time_def.hpp"
#include "logic/unit.hpp"
#include "logic/unit_man.h"
#include "internet/net.pb.h"
#include "pk_community_def.hpp"
#include "pk_community_mgr.h"
#include "server_log.hpp"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "time.hpp"
#include "utility/init_unit.h"
#include "utility/globle_data.h"


namespace faith
{
	void pk_community_mgr::clear_data()
	{
		m_map_have_not_safe_region = 0;
		m_pre_safe_map_template_id = 0;
		m_max_safe_map_priority = 0;
		m_pk_value_time = 0;
		m_is_pk_value_time = false;
		m_cut_interval_get_region = 0;	//区域类型tick
		m_region_type = e_region_type_zhongli;
		m_pre_location.clear_data();
		m_fight_back_list.clear();
		m_tick_interval_1sec = 0;
	}

	void pk_community_mgr::heart_tick(const int64& new_time)
	{
		if (m_is_pk_value_time)
		{
			if (new_time >= m_pk_value_time)
			{
				player& player_ref = unit_man::get_player(m_unit_array_index);
				if (player_ref.is_valid())
				{
					cut_pk_value(PER_MIN_CLEAR_PK_VALUE);
				}
				else
				{
					m_is_pk_value_time = false;
				}
				m_pk_value_time = new_time + pk_value_cut_time;
			}
		}

		if (new_time > m_cut_interval_get_region)
		{
			player& player_ref = unit_man::get_player(m_unit_array_index);
			if (player_ref.is_valid())
			{
				set_big_map_region_type();
			}
			m_cut_interval_get_region = new_time + second_tick_time * 2;
		}
		
		if (new_time > m_tick_interval_1sec)
		{
			m_tick_interval_1sec = new_time + second_tick_time;
			heart_tick_1sec(new_time);
		}
	}

	void pk_community_mgr::heart_tick_1sec(const int64& new_time)
	{
		check_fight_back();
	}

	void pk_community_mgr::set_unit_array_index(int32 unit_array_index)
	{
		m_unit_array_index = unit_array_index;
	}

	bool pk_community_mgr::is_map_can_change_pk_mode(int32 map_template_id)
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return false;
		}

		return init_unit::is_map_can_change_pk_mode(map_template_ptr->Type);
	}

	void pk_community_mgr::change_pk_info(int32 pk_type, int32 pk_value, bool need_check_map)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}

		if (need_check_map && false == init_unit::is_can_change_pk_mode(base_map_system::get_map_type(m_unit_ref.get_map_ent())))
		{
			return;
		}

		bool if_change = false;
		switch (pk_type)
		{
		case faith::e_pk_info_pk_mode:
		{
			int32 pk_mode_val = get_pk_mode(pk_value);
			if (e_pk_mode_peace <= pk_mode_val && pk_mode_val < e_pk_mode_max)
			{
				m_unit_ref.get_pawn_att().set_unit_base_att(e_base_att_info_pk_mode, pk_value);
				if_change = true;
			}
		}
		break;
		case faith::e_pk_info_community_type:
		{
			if (0 <= pk_value && pk_value < e_community_type_max)
			{
				m_unit_ref.get_pawn_att().set_unit_base_att(e_base_att_info_community_type, pk_value);
				if_change = true;
			}
		}
		break;
		}
		if(if_change)
		{
			sync_pk_info_aoi((e_pk_info)pk_type);
			
			player& player_ref = unit_man::get_player(m_unit_array_index);
			if (player_ref.is_valid() && player_ref.get_is_robot() == false)
			{
				int32 pk_value = player_ref.get_pk_community_mgr().get_pk_value();
				int32 pk_mode = player_ref.get_pk_community_mgr().get_pk_mode();
				server_log::player_pk_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), pk_mode, pk_value);
				
			}
		}
	}

	void pk_community_mgr::sync_pk_info_aoi(e_pk_info pk_type)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		
		int32 pk_value = -1;
		switch (pk_type)
		{
		case faith::e_pk_info_pk_mode:
		{
			pk_value = m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_pk_mode);
		}
		break;
		case faith::e_pk_info_community_type:
		{
			pk_value = m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
		}
		break;
		}
		character_proto_pk_mode_change_end end_msg;
		end_msg.add_role_guid(m_unit_ref.get_unit_guid().A);
		end_msg.add_role_guid(m_unit_ref.get_unit_guid().B);
		end_msg.set_pk_info_type(pk_type);
		end_msg.set_pk_info_value(pk_value);
		m_unit_ref.send_message_to_aoi(&end_msg, e_msgindex_s2c_change_pk_mode_end);
	}

	void pk_community_mgr::on_player_transfer_init()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return ;
		}
		int32 cur_pk_value = m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_pk_value);
		if (cur_pk_value > 0)
		{
			m_pk_value_time = utility::get_tick_count() + pk_value_cut_time;
			m_is_pk_value_time = true;
		}
		m_map_have_not_safe_region = 0;
		m_pre_safe_map_template_id = 0;
		m_max_safe_map_priority = -1;
		
		int32 map_template_id = m_unit_ref.get_unit_info(faith::e_role_info_move_map_id);
		MapTemplate* map_tempalte_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (map_tempalte_ptr)
		{
			int32 len = map_tempalte_ptr->SafePoint.size();
			for (int32 i = 0;i < len;i++)
			{
				MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, map_tempalte_ptr->SafePoint[i]);
				if (map_safe_template_ptr != nullptr && map_safe_template_ptr->Priority > m_max_safe_map_priority)
				{
					m_max_safe_map_priority = map_safe_template_ptr->Priority;
				}
			}
		}
	}

	void pk_community_mgr::add_pk_value(int32 pk_value)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		e_pk_mode_type map_pk_type = get_cur_map_pk_type();
		if (map_pk_type == e_pk_mode_type_no_kill_point)
		{
			return;
		}
		if (m_region_type == e_region_type_danger_zhongli)	//危险中立区不增加pk值
		{
			return;
		}

		if (m_unit_ref.get_legion_guid().is_valid())
		{
			legion_cs_mgr& legion_cs_ref = m_unit_ref.get_legion_cs_mgr();

			if (legion_cs_ref.get_legion_job_title() == e_legion_job_title_chief) //领主在领地内不加PK值
			{
				if (legion_cs_ref.is_city_master())	//霸主不增加PK值
				{
					return;
				}
				if (legion_cs_ref.get_cur_legion_territory_safe_id() > 0)
				{
					return;
				}
			}
		}

		int32 temp_pk_value = m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_pk_value);
		if (temp_pk_value <= 0 && (temp_pk_value + pk_value) > 0)
		{//退出好人阵营
			//quit_community(e_community_type_kind_man);
		}
		
		pk_value_skip_level_add_buff(temp_pk_value, temp_pk_value + pk_value);
		temp_pk_value += pk_value;
		m_unit_ref.get_pawn_att().set_unit_base_att(e_base_att_info_pk_value, temp_pk_value);
		m_unit_ref.get_pawn_att().send_base_att_one(e_base_att_info_pk_value);
		m_pk_value_time = utility::get_tick_count() + pk_value_cut_time;				//重计时间
		m_is_pk_value_time = true;

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() && player_ref.get_write_log())
		{
			int32 pk_value = player_ref.get_pk_community_mgr().get_pk_value();
			int32 pk_mode = player_ref.get_pk_community_mgr().get_pk_mode();
			server_log::player_pk_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), pk_mode, pk_value);
			
		}
	}


	int32 pk_community_mgr::cut_pk_value(int32 pk_value)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return 0;
		}
		int32 temp_pk_value = m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_pk_value);
		if (temp_pk_value > 0)
		{
			int32 left_value = temp_pk_value - pk_value;
			int32 old_pk_value = temp_pk_value;
			pk_value_skip_level_add_buff(temp_pk_value, temp_pk_value - pk_value);
			temp_pk_value -= pk_value;
			temp_pk_value = temp_pk_value > 0 ? temp_pk_value : 0;
			m_unit_ref.get_pawn_att().set_unit_base_att(e_base_att_info_pk_value, temp_pk_value);
			m_unit_ref.get_pawn_att().send_base_att_one(e_base_att_info_pk_value);
			if (temp_pk_value <= 0)
			{
				m_is_pk_value_time = false;
			}
			if (left_value >= 0)
			{
				return pk_value;
			}
			else
			{
				return old_pk_value;
			}
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() && player_ref.get_write_log())
		{
			int32 pk_value = player_ref.get_pk_community_mgr().get_pk_value();
			int32 pk_mode = player_ref.get_pk_community_mgr().get_pk_mode();
			server_log::player_pk_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), pk_mode, pk_value);
			
		}
		return 0;
	}

	int32 pk_community_mgr::get_pk_value() 
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return -1;
		}
		return m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_pk_value);
	}

	bool pk_community_mgr::check_if_can_attack_target_by_region(unit* target_unit)
	{
		return true;

	}

	bool pk_community_mgr::is_skill_target(int32 target_unit_index, int32 skill_target_type)
	{
		if (skill_target_type < 0 || skill_target_type >= eskilltarget_max)
		{
			return false;
		}
		s_is_skill_target_param param;
		unit& temp_my_unit_ref = unit_man::get_unit(m_unit_array_index);
		unit& temp_target_unit_ref = unit_man::get_unit(target_unit_index);
		if (!temp_my_unit_ref.is_valid() || !temp_target_unit_ref.is_valid())
		{
			return false;
		}
		if (temp_target_unit_ref.get_unit_type() == faith::e_unit_type_summoned)
		{
			npc& npc_ref = unit_man::get_npc(target_unit_index);
			if (npc_ref.is_valid() && npc_ref.is_summon_pet())
				return false;
		}
		s_unit_identifier my_unit_identifier;
		my_unit_identifier.clear_data();
		if (temp_my_unit_ref.get_unit_type() == faith::e_unit_type_summoned || temp_my_unit_ref.get_unit_type() == e_unit_type_hide)
		{
			npc* npc_ptr = npc::cast(&temp_my_unit_ref);
			if (npc_ptr == nullptr)
			{
				return false;
			}
			my_unit_identifier = npc_ptr->get_owner();
			unit& unit_ref = unit_man::get_unit(my_unit_identifier);
			if (unit_ref.is_valid() == false)
			{
				return false;
			}
			if (unit_ref.get_map_guid() != npc_ptr->get_map_guid())
			{
				return false;
			}
		}
		if (my_unit_identifier.is_valid() == false)
		{
			my_unit_identifier = temp_my_unit_ref.get_identifier();
		}

		s_unit_identifier target_unit_identifier;
		target_unit_identifier.clear_data();
		if (temp_target_unit_ref.get_unit_type() == faith::e_unit_type_summoned)
		{
			npc* npc_ptr = npc::cast(&temp_target_unit_ref);
			if (npc_ptr == nullptr)
			{
				return false;
			}
			target_unit_identifier = npc_ptr->get_owner();
			unit& unit_ref = unit_man::get_unit(target_unit_identifier);
			if (unit_ref.is_valid() == false)
			{
				return false;
			}
			if (unit_ref.get_map_guid() != npc_ptr->get_map_guid())
			{
				return false;
			}
		}
		if (target_unit_identifier.is_valid() == false)
		{
			target_unit_identifier = temp_target_unit_ref.get_identifier();
		}

		unit& my_unit_ref = unit_man::get_unit(my_unit_identifier);
		unit& target_unit_ref = unit_man::get_unit(target_unit_identifier);
		if (my_unit_ref.is_valid() == false || target_unit_ref.is_valid() == false)
		{
			return false;
		}

		if (target_unit_ref.is_dead())
		{
			return false;
		}
		param.skill_target_type = skill_target_type;
		int32	sender_region_type = my_unit_ref.get_pk_community_mgr().get_region_type();
		int32	target_region_type = target_unit_ref.get_pk_community_mgr().get_region_type();
		if (sender_region_type > target_region_type)
		{
			param.region_type = sender_region_type;
		}
		else
		{
			param.region_type = target_region_type;
		}
		//param.map_pk_type = get_cur_map_pk_type(); //统一用region_type判断了
		param.sender_unit_type = my_unit_ref.get_unit_type();
		param.target_unit_type = target_unit_ref.get_unit_type();
		param.sender_line_id = my_unit_ref.get_line_id();
		param.target_line_id = target_unit_ref.get_line_id();
		param.sender_community_type = my_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
		param.target_community_type = target_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
		param.sender_pk_mode = my_unit_ref.get_pk_community_mgr().get_pk_mode();
		param.target_pk_mode = target_unit_ref.get_pk_community_mgr().get_pk_mode();
		param.sender_move_state = my_unit_ref.get_pawn_att().get_game_att(e_unit_game_att_movement);
		param.target_move_state = target_unit_ref.get_pawn_att().get_game_att(e_unit_game_att_movement);
		param.target_pk_value = target_unit_ref.get_pk_community_mgr().get_pk_value();
		param.is_in_same_team = (my_unit_ref.get_team_guid().is_valid() && my_unit_ref.get_team_guid() == target_unit_ref.get_team_guid());
		param.is_in_same_legion = (my_unit_ref.get_legion_guid().is_valid() && my_unit_ref.get_legion_guid() == target_unit_ref.get_legion_guid());
		param.is_self = (my_unit_ref.get_unit_guid() == target_unit_ref.get_unit_guid());
		param.sender_sever_id = my_unit_ref.get_unit_info(e_role_info_server_id);
		param.target_sever_id = target_unit_ref.get_unit_info(e_role_info_server_id);
		param.cur_server_id = cell_server::getInstance().get_server_id();

		if (param.target_community_type == e_community_type_npc_pet)
		{
			npc* npc_ptr = npc::cast(&target_unit_ref);
			if (npc_ptr)
			{
				if (npc_ptr->get_owner().unit_guid == my_unit_ref.get_unit_guid())
				{
					param.is_self_pet = true;
				}
				else
				{
					unit& unit_ref = unit_man::get_unit(npc_ptr->get_owner());
					if (unit_ref.is_valid())
					{
						param.target_community_type = unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
						param.target_pk_mode = unit_ref.get_pk_community_mgr().get_pk_mode();
					}
				}
			}
		}
		if (param.sender_community_type == e_community_type_npc_pet)
		{
			npc* npc_ptr = npc::cast(&my_unit_ref);
			if (npc_ptr)
			{
				if (npc_ptr->get_owner().unit_guid == target_unit_ref.get_unit_guid())
				{
					param.is_self_owner = true;
				}
				else
				{
					unit& unit_ref = unit_man::get_unit(npc_ptr->get_owner());
					if (unit_ref.is_valid())
					{
						param.sender_community_type = unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
						param.sender_pk_mode = unit_ref.get_pk_community_mgr().get_pk_mode();
					}
				}
			}
		}
		
		if (param.target_unit_type == e_unit_type_monster && target_unit_ref.get_unit_sub_type() == e_monster_type_boss)
		{
			param.is_boss = true;
		}
		if (param.target_unit_type == e_unit_type_player && param.sender_unit_type == e_unit_type_player)
		{
			c_fight_back_map_ite _ite = m_fight_back_list.find(target_unit_ref.get_unit_guid().server_64);
			param.is_fight_back_state = (_ite != m_fight_back_list.end());
			param.is_team_protect = my_unit_ref.get_pk_community_mgr().get_team_protect();
			param.is_legion_protect = my_unit_ref.get_pk_community_mgr().get_legion_protect();
		}

		bool is_skill_target = init_unit::is_skill_target_with_param(param);
		return is_skill_target;
	}

	int32 pk_community_mgr::get_entire_pk_mode()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return -1;
		}
		return m_unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_pk_mode);
	}

	void pk_community_mgr::set_entire_pk_mode(int32 pk_cfg_entire, bool sync_aoi)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		if (m_unit_ref.is_self_server() && pk_cfg_entire == e_pk_mode_plunder)
		{
			pk_cfg_entire = e_pk_mode_cross_sever;
		}
		m_unit_ref.get_pawn_att().set_unit_base_att(e_base_att_info_pk_mode, pk_cfg_entire);

		if (sync_aoi)
		{
			sync_pk_info_aoi(e_pk_info_pk_mode);
		}
	}

	int32 pk_community_mgr::get_pk_mode(int32 pk_cfg_entire)
	{
		if (pk_cfg_entire < 0)
		{
			pk_cfg_entire = get_entire_pk_mode();
		}
		return pk_cfg_entire & 0xF;
	}

	void pk_community_mgr::set_pk_mode(int32 cfg_value)
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		pk_cfg_entire >>= e_pk_cfg_type_pk_mode_end;
		pk_cfg_entire <<= e_pk_cfg_type_pk_mode_end;
		cfg_value &= 0xF; //保证cfg_value<16
		pk_cfg_entire |= cfg_value;

		set_entire_pk_mode(pk_cfg_entire);
	}

	bool pk_community_mgr::get_is_fight_back()
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		return init_unit::get_flag_data(pk_cfg_entire, e_pk_cfg_type_is_fight_back) == 1;
	}

	void pk_community_mgr::set_is_fight_back(bool cfg_value)
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		if (cfg_value)
		{
			init_unit::mark_flag_data(pk_cfg_entire, e_pk_cfg_type_is_fight_back);
		}
		else
		{
			init_unit::erase_flag_data(pk_cfg_entire, e_pk_cfg_type_is_fight_back);
		}
		set_entire_pk_mode(pk_cfg_entire);
	}

	bool pk_community_mgr::get_team_protect()
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		return init_unit::get_flag_data(pk_cfg_entire, e_pk_cfg_type_team_protect) == 1;
	}

	void pk_community_mgr::set_team_protect(bool cfg_value)
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		if (cfg_value)
		{
			init_unit::mark_flag_data(pk_cfg_entire, e_pk_cfg_type_team_protect);
		}
		else
		{
			init_unit::erase_flag_data(pk_cfg_entire, e_pk_cfg_type_team_protect);
		}
		set_entire_pk_mode(pk_cfg_entire);
	}

	bool pk_community_mgr::get_legion_protect()
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		return init_unit::get_flag_data(pk_cfg_entire, e_pk_cfg_type_legion_protect) == 1;
	}

	void pk_community_mgr::set_legion_protect(bool cfg_value)
	{
		int32 pk_cfg_entire = get_entire_pk_mode();
		if (cfg_value)
		{
			init_unit::mark_flag_data(pk_cfg_entire, e_pk_cfg_type_legion_protect);
		}
		else
		{
			init_unit::erase_flag_data(pk_cfg_entire, e_pk_cfg_type_legion_protect);
		}
		set_entire_pk_mode(pk_cfg_entire);
	}

	bool pk_community_mgr::is_in_peace_pk_mode()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return false;
		}
		if (get_pk_mode() == e_pk_mode_peace)
		{
			return true;
		}
		return false;
	}

	bool pk_community_mgr::is_in_legal_pk_area()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return false;
		}
		int32 cur_map_template_id = m_unit_ref.get_unit_info(e_role_info_move_map_id);
		s_map_pos cur_player_pos = m_unit_ref.get_new_map_pos();
		MapTemplate* cur_map_template_ptr = GET_TEMPLATE(MapTemplate, cur_map_template_id);
		if (cur_map_template_ptr == nullptr)
		{
			return false;
		}
		if (cur_map_template_ptr->IsAllowPK == e_pk_mode_type_no_kill_point)
		{
			return true;
		}
		for (int32 i = 0; i < cur_map_template_ptr->SafePoint.size(); ++i)
		{
			int32 safe_template_id = cur_map_template_ptr->SafePoint[i];
			MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, safe_template_id);
			if (map_safe_template_ptr != nullptr
				&& map_safe_template_ptr->IsAllowPK == e_pk_mode_type_no_kill_point)
			{
				s_map_block tmp_block;
				tmp_block.priority = map_safe_template_ptr->Priority;
				for (int32 j = 0; j + 1 < map_safe_template_ptr->PointAry.size(); j += 2)
				{
					//if (map_safe_template_ptr->PointAry[j] > 0)
					//{
						tmp_block.points.push_back(fvector(map_safe_template_ptr->PointAry[j], map_safe_template_ptr->PointAry[j + 1]));
					//}
				}
				if (init_unit::is_in_safe(tmp_block.points, cur_player_pos.unit_location))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool pk_community_mgr::is_in_not_peace_and_kindevil()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return false;
		}
		if (get_pk_mode() != e_pk_mode_peace
			&& get_pk_mode() != e_pk_mode_good_and_evil)
		{
			return true;
		}
		return false;
	}

	void pk_community_mgr::pk_value_skip_level_add_buff(int32 old_value, int32 new_value)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		int32 level_values[4];
		int32 level_buff_ids[4];
		level_values[0] = GAMECONFIG->PKValueLevelA;
		level_values[1] = GAMECONFIG->PKValueLevelB;
		level_values[2] = GAMECONFIG->PKValueLevelC;
		level_values[3] = GAMECONFIG->PKValueLevelD;

		level_buff_ids[0] = GAMECONFIG->PkALevelBuffId;
		level_buff_ids[1] = GAMECONFIG->PkBLevelBuffId;
		level_buff_ids[2] = GAMECONFIG->PkCLevelBuffId;
		level_buff_ids[3] = GAMECONFIG->PkDLevelBuffId;

		if (old_value < new_value)
		{

			for (int32 i = 0; i < 4;++i)
			{
				if (old_value < level_values[i] && level_values[i] <= new_value)
				{
					if (i != 0)
					{
						buff_man::del_buff_by_template_id(m_unit_ref.get_array_index(), m_unit_ref.get_array_index(), level_buff_ids[i - 1]);
					}
					buff_man::add_buff_inst(m_unit_ref.get_array_index(), m_unit_ref.get_array_index(), level_buff_ids[i]);

					//pkbuff值重置
					mod_buff_dur_time(new_value);
					return;
				}
			}

			//pkbuff值重置
			mod_buff_dur_time(new_value);
		}
		else if (old_value > new_value)	
		{
			for (int32 i = 0; i < 4; ++i)
			{
				if (old_value >= level_values[i] && level_values[i] > new_value)
				{
					buff_man::del_buff_by_template_id_one(m_unit_ref.get_array_index(), m_unit_ref.get_array_index(), level_buff_ids[i]);
					if (i != 0)
					{
						buff_man::add_buff_inst(m_unit_ref.get_array_index(), m_unit_ref.get_array_index(), level_buff_ids[i - 1]);
						mod_buff_dur_time(new_value);
					}
					//buff_man::del_buff_by_template_id_one(m_unit_ptr->get_array_index(), m_unit_ptr->get_array_index(), level_buff_ids[i]);
					return;
				}
			}
		}
	}

	void	pk_community_mgr::pk_value_add_buff(int32 pk_value)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		int32 level_values[4];
		int32 level_buff_ids[4];
		level_values[0] = GAMECONFIG->PKValueLevelA;
		level_values[1] = GAMECONFIG->PKValueLevelB;
		level_values[2] = GAMECONFIG->PKValueLevelC;
		level_values[3] = GAMECONFIG->PKValueLevelD;

		level_buff_ids[0] = GAMECONFIG->PkALevelBuffId;
		level_buff_ids[1] = GAMECONFIG->PkBLevelBuffId;
		level_buff_ids[2] = GAMECONFIG->PkCLevelBuffId;
		level_buff_ids[3] = GAMECONFIG->PkDLevelBuffId;

		for (int32 i = 0; i < 3; ++i)
		{
			if (level_values[i] <= pk_value && pk_value < level_values[i + 1]  )
			{
				buff_man::add_buff_inst(m_unit_ref.get_array_index(), m_unit_ref.get_array_index(), level_buff_ids[i]);
				//pkbuff值重置
				mod_buff_dur_time(pk_value);
				return;
			}
			if (pk_value >= level_values[3])
			{
				buff_man::add_buff_inst(m_unit_ref.get_array_index(), m_unit_ref.get_array_index(), level_buff_ids[3]);
				//pkbuff值重置
				mod_buff_dur_time(pk_value);
			}
		}
	}

	void	pk_community_mgr::mod_buff_dur_time(int32 pk_value)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		int32 level_values[4];
		int32 level_buff_ids[4];
		level_values[0] = GAMECONFIG->PKValueLevelA;
		level_values[1] = GAMECONFIG->PKValueLevelB;
		level_values[2] = GAMECONFIG->PKValueLevelC;
		level_values[3] = GAMECONFIG->PKValueLevelD;

		level_buff_ids[0] = GAMECONFIG->PkALevelBuffId;
		level_buff_ids[1] = GAMECONFIG->PkBLevelBuffId;
		level_buff_ids[2] = GAMECONFIG->PkCLevelBuffId;
		level_buff_ids[3] = GAMECONFIG->PkDLevelBuffId;

		int32 index = -1;
		for (int32 i = 3; i >= 0; i--)
		{
			if (pk_value >= level_values[i])
			{
				index = i;
				break;
			}
		}
		if (index < 0)
			return;

		BuffDataTemplate* m_buff_template_ptr = GET_TEMPLATE(BuffDataTemplate, level_buff_ids[index]);
		if (nullptr == m_buff_template_ptr)
		{
			return;
		}

		int32 dur_time = (pk_value - level_values[index] + PER_MIN_CLEAR_PK_VALUE) * (pk_value_cut_time / second_tick_time) / PER_MIN_CLEAR_PK_VALUE;	//服务器的刷新时间
		buff_man::mod_buff_during_time_by_series_id(m_unit_ref.get_array_index(), m_buff_template_ptr->Series, dur_time);
		return;
	}

	void	pk_community_mgr::check_first_hurt_add_pk_vaule()
	{
		//非善恶模式第一次攻击玩家增加pk值
		int32 my_pk_mode = get_pk_mode();
		if (my_pk_mode != e_pk_mode_good_and_evil
			&& my_pk_mode != e_pk_mode_peace
			&& !is_in_legal_pk_area()
			&& get_pk_value() == 0)
		{
			add_pk_value(FIRST_HURT_ADDED_PK_VALUE);
		}
	}
	bool pk_community_mgr::check_if_move()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return false;
		}
		int32 unit_template_id = m_unit_ref.get_unit_info(faith::e_role_info_template_id);
		if ((m_unit_ref.get_unit_info(faith::e_role_info_move_pos_x) == m_pre_location.x) && (m_unit_ref.get_unit_info(faith::e_role_info_move_pos_y) == m_pre_location.y) && (m_unit_ref.get_unit_info(faith::e_role_info_move_pos_z) == m_pre_location.z))
		{
			return false;
		}
		m_pre_location = fvector(m_unit_ref.get_unit_info(faith::e_role_info_move_pos_x), m_unit_ref.get_unit_info(faith::e_role_info_move_pos_y), m_unit_ref.get_unit_info(faith::e_role_info_move_pos_z));
		return true;
	}

	void pk_community_mgr::set_big_map_region_type()
	{
		if (m_map_have_not_safe_region > 0)
		{
			return;
		}

		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}

		if (false == check_if_move())
		{
			return;
		}

		MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, m_pre_safe_map_template_id);
		if (map_safe_template_ptr != nullptr && map_safe_template_ptr->Priority >= m_max_safe_map_priority)
		{
			map_point_vec map_pos_ary;
			for (int32 i = 0;i + 1 < map_safe_template_ptr->PointAry.size();i += 2)
			{
				//if (map_safe_template_ptr->PointAry[i] > 0)
				//{
					map_pos_ary.push_back(fvector(map_safe_template_ptr->PointAry[i], map_safe_template_ptr->PointAry[i + 1]));
				//}
			}
			if (init_unit::is_in_safe(map_pos_ary, m_unit_ref.get_new_map_pos().unit_location))
			{
				return;
			}
		}

		//int32 cur_map_template_id = m_unit_ref.get_unit_info(faith::e_role_info_move_map_id);
		// if (cur_map_template_id == faith::big_world_map_id) //依赖于副本中没配安全区就好 不用特例
		const s_map_block* cur_safe_region = m_unit_ref.get_in_map_safe();
		if (nullptr == cur_safe_region)
		{
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, m_unit_ref.get_unit_info(e_role_info_move_map_id));
			if (nullptr == map_template_ptr)
			{
				return;
			}
			// 大世界安全区会包裹不完全导致安全区失效
			if (map_template_ptr->Type == e_map_type_big_map)
			{
				m_map_have_not_safe_region = 0;
				return; //没有配安全区的地图 比如副本
			}
			m_map_have_not_safe_region = 1;
			return; //没有配安全区的地图 比如副本
		}

		m_pre_safe_map_template_id = cur_safe_region->region_id;
		m_region_type = cur_safe_region->region_type;
	}


	void pk_community_mgr::init_map_pk_type(e_pk_mode_type mode_type)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, m_unit_ref.get_unit_info(e_role_info_move_map_id));
		if (nullptr == map_template_ptr)
		{
			return;
		}
		e_pk_mode_type map_pk_type = get_cur_map_pk_type();
		if (mode_type != e_pk_mode_type_max)
		{
			map_pk_type = mode_type;
		}

		switch (map_pk_type)
		{
		case e_pk_mode_type_cant:
		{
			m_region_type = e_region_type_safe;
		}
		break;
		case e_pk_mode_type_allow:
		{
			m_region_type = e_region_type_zhongli;
		}
		break;
		case e_pk_mode_type_no_kill_point:
		{
			m_region_type = e_region_type_danger_zhongli;
		}
		break;
		}
	}

	e_pk_mode_type pk_community_mgr::get_cur_map_pk_type()
	{
		unit& my_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!my_unit_ref.is_valid())
		{
			return e_pk_mode_type_cant;
		}
		//return base_map_system::get_map_pk_mode(my_unit_ref.get_map_guid());
		return e_pk_mode_type_allow;
	}

	void pk_community_mgr::on_player_be_attacked(int32 unit_array_index)
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}
		unit& unit_ref = unit_man::get_unit(unit_array_index);
		if (false == unit_ref.is_valid() || e_unit_type_player != unit_ref.get_unit_type())
		{
			return;
		}

		//攻击友方阵营计入pk值
		int32 att_community_type = unit_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
		int32 self_community_type = my_player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
		if (globle_data::get_instance().get_community_group_relation_by_type(att_community_type, self_community_type) > 0)
		{
			if (unit_ref.is_self_server() && my_player_ref.is_self_server())//跨服不加PK值
			{
				unit_ref.get_pk_community_mgr().check_first_hurt_add_pk_vaule();
			}
		}

		add_fight_back(unit_ref.get_unit_guid());
		my_player_ref.send_attacker_info(unit_ref);
	}
	
	void pk_community_mgr::on_player_be_attacked_by_robot_player(int32 unit_array_index)
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return;
		}
		npc& npc_ref = unit_man::get_npc(unit_array_index);
		if (!npc_ref.is_valid())
		{
			return;
		}

		if (base_map_system::get_map_type(my_player_ref.get_map_ent()) != e_map_type_single_land_boss)
		{
			return;
		}
		if (npc_ref.get_unit_type() == e_unit_type_monster && npc_ref.get_unit_sub_type() == e_monster_type_robot_player)
		{
			my_player_ref.send_attacker_info_by_robot_player(npc_ref);
		}

	}

	void pk_community_mgr::add_fight_back(const guid_64& attacker_guid)
	{
		if (false == get_is_fight_back())
		{
			return;
		}
		int32 cur_stamp = time_helper::get_cur_time_new().second;
		m_fight_back_list[attacker_guid.server_64] = cur_stamp + auto_fight_back_sec;
	}
	
	void pk_community_mgr::check_fight_back()
	{
		int32 cur_stamp = time_helper::get_cur_time_new().second;
		for (c_fight_back_map_ite ite = m_fight_back_list.begin(); ite != m_fight_back_list.end(); )
		{
			if (cur_stamp > ite->second)
			{
				ite = m_fight_back_list.erase(ite);
			}
			else
			{
				++ite;
			}
		}
	}

	faith::int32 pk_community_mgr::get_region_type()
	{
		return m_region_type;
	}
}