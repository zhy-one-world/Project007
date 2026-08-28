/********************************************************************
created: 2017年7月7日
file base: team_cs_mgr
file ext: h
author: Kero
purpose: team mgr in cs
*********************************************************************/

#include "aoi/aoi_system.h"
#include "cell_server.hpp"
#include "components/scene/base_map_component.h"
#include "connection_mgr.hpp"
#include "internal/core.hpp"
#include "internet/error.pb.h"
#include "internet/net.pb.h"
#include "internet/team.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "npc.hpp"
#include "server_log.hpp"
#include "system/scene/base_map_system.h"
#include "team_cs_mgr.h"
#include <Utility/parse_msg.h>


namespace hld
{
	team_cs_mgr::team_cs_mgr()
	{
		m_unit_array_index = -1;
		clear_data();
	}
	void team_cs_mgr::clear_data()
	{
		m_unit_team_info.clear_data();
		m_self_one_stop_flags = -1;
		m_is_not_drop = false;
	}

	void team_cs_mgr::set_unit_array_index(int32 unit_array_index)
	{
		clear_data();
		m_unit_array_index = unit_array_index;
	}

	void team_cs_mgr::set_player_team_info(const s_player_team_info& player_team_info, bool is_player_init)
	{
		if (false == player_team_info.team_guid.is_valid())
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}

		s_player_team_info old_team_info = m_unit_team_info;
		m_unit_team_info = player_team_info;

		team_proto_set_player_team_aoi_info set_team_aoi_info_msg;
		fill_aoi_info_msg(set_team_aoi_info_msg);
		player_ref.send_message_to_aoi(&set_team_aoi_info_msg, e_msgindex_s2c_set_player_team_aoi_info);

		if (is_player_init)
		{
			// 玩家初始化到这里为止 后面都是正常队伍信息更新的逻辑
			return;
		}

		check_update_team_mission();

		if (old_team_info.is_valid() && m_unit_team_info.is_valid()
			&& old_team_info.captain_guid != m_unit_team_info.captain_guid)
		{
			on_team_captain_changed(old_team_info);
		}

		if (old_team_info.is_valid() && m_unit_team_info.is_valid()
			&& old_team_info.team_type != m_unit_team_info.team_type)
		{
			on_team_type_changed(old_team_info);
		}
		if (old_team_info.is_valid() == false && m_unit_team_info.is_valid())
		{
			on_enter_team(old_team_info);
		}
		//if (old_team_info.is_valid() && m_unit_team_info.is_valid() == false)
		//{
		//	on_leave_team(old_team_info);
		//}

		player_ref.get_achievement_mgr().create_team();
	}

	void team_cs_mgr::clear_player_team_info()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}

		s_player_team_info old_team_info = m_unit_team_info;
		if (old_team_info.is_valid())
		{
			on_leave_team(old_team_info);
		}
		m_unit_team_info.clear_data();

		const guid_64 player_guid = player_ref.get_unit_guid();
		hld::team_proto_clear_player_team_aoi_info clear_player_team_aoi_info_msg;
		clear_player_team_aoi_info_msg.set_role_guid(player_guid.server_64);
		player_ref.send_message_to_aoi(&clear_player_team_aoi_info_msg, e_msgindex_s2c_clear_player_team_aoi_info);

		if (player_ref.get_assist_fight_mgr().is_multi_map_assist_fight())
		{
			player_ref.get_assist_fight_mgr().cancel_assist_fight();
		}
	}

	void team_cs_mgr::fill_aoi_info_msg(team_proto_set_player_team_aoi_info& msg_data)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		const guid_64 player_guid = player_ref.get_unit_guid();
		const s_player_team_info& player_team_info = m_unit_team_info;
		msg_data.set_role_guid(player_guid.server_64);
		msg_data.set_team_guid(player_team_info.team_guid.server_64);
		msg_data.set_captain_guid(player_team_info.captain_guid.server_64);
		msg_data.set_member_num(player_team_info.member_num);
		msg_data.set_team_type(player_team_info.team_type);
		msg_data.set_team_sub_type_id(player_team_info.team_sub_type_id);
		msg_data.set_one_stop_flags(player_team_info.team_one_stop_flags);
	}

	void team_cs_mgr::gen_player_team_member_info(s_team_member_info& team_mem_info, e_team_type team_type, int32 team_sub_type_id)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}

		team_mem_info.role_guid = player_ref.get_unit_guid();
		team_mem_info.role_index = m_unit_array_index;
		team_mem_info.set_role_name(player_ref.get_name());
		team_mem_info.gs_value = player_ref.get_unit_gs_value();
		team_mem_info.data_ary[ETeamMemberInfo_is_online] = 1;
		team_mem_info.data_ary[ETeamMemberInfo_line_id] = player_ref.get_unit_info(e_role_info_move_line_id);
		team_mem_info.data_ary[ETeamMemberInfo_player_template_id] = player_ref.get_unit_info(e_role_info_template_id);
		team_mem_info.data_ary[ETeamMemberInfo_cur_hp] = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
		team_mem_info.data_ary[ETeamMemberInfo_max_hp] = player_ref.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
		team_mem_info.data_ary[ETeamMemberInfo_level] = player_ref.get_unit_info(e_role_info_exp_level);
		team_mem_info.data_ary[ETeamMemberInfo_cur_map] = player_ref.get_unit_info(e_role_info_move_map_id);
		team_mem_info.data_ary[ETeamMemberInfo_damage] = 0;
		team_mem_info.data_ary[ETeamMemberInfo_one_stop_flags] = get_self_one_stop_flags(team_sub_type_id);

		e_error_code check_ret = e_error_code_success;
		if (is_team_type_raid(team_type))
		{
			//check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(player_ref.get_array_index(), team_sub_type_id);
		}
		team_mem_info.data_ary[ETeamMemberInfo_check_ret] = check_ret;
	}

	void team_cs_mgr::send_leave_team_to_ws()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		const guid_64& team_guid = m_unit_team_info.team_guid;
		if (team_guid.is_valid() == false)
		{
			return;
		}

		cs2ws_del_team_member del_team_member_msg;
		del_team_member_msg.team_guid = team_guid;
		del_team_member_msg.member_guid = player_ref.get_unit_guid();
		connection_mgr::getInstance().send_to_ws(&del_team_member_msg, sizeof(del_team_member_msg));
	}

	void team_cs_mgr::on_player_reconnect()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}

		if (m_unit_team_info.is_valid())
		{
			team_proto_set_player_team_aoi_info set_team_aoi_info_msg;
			fill_aoi_info_msg(set_team_aoi_info_msg);
			player_ref.send_message_to_self(&set_team_aoi_info_msg, e_msgindex_s2c_set_player_team_aoi_info);
		}
		else
		{
			const guid_64 player_guid = player_ref.get_unit_guid();
			hld::team_proto_clear_player_team_aoi_info clear_player_team_aoi_info_msg;
			clear_player_team_aoi_info_msg.set_role_guid(player_guid.server_64);
			player_ref.send_message_to_self(&clear_player_team_aoi_info_msg, e_msgindex_s2c_clear_player_team_aoi_info);
		}
	}

	void team_cs_mgr::on_team_captain_changed(const s_player_team_info& old_team_info)
	{
		//if (old_team_info.team_one_stop_flags >= 0 && m_unit_team_info.team_one_stop_flags >= 0)
		//{
		//	//一条龙队伍换队长了
		//	check_one_stop_finish_flags();
		//	check_new_aim_for_one_stop();
		//}

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
			return;

		if (player_ref.get_assist_fight_mgr().get_assist_fight_boss_id().is_valid() &&
			player_ref.get_unit_guid() == old_team_info.captain_guid || player_ref.get_unit_guid() == m_unit_team_info.captain_guid)
		{
			npc& boss_ref = unit_man::get_npc(player_ref.get_assist_fight_mgr().get_assist_fight_boss_id());
			if (boss_ref.is_valid())
			{
				boss_ref.change_team_captain(old_team_info.captain_guid, m_unit_team_info.captain_guid);
			}
		}
	}
	void team_cs_mgr::on_enter_team(const s_player_team_info& old_team_info)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
			return;

		if (player_ref.get_assist_fight_mgr().get_assist_fight_boss_id().is_valid())
		{
			npc& boss_ref = unit_man::get_npc(player_ref.get_assist_fight_mgr().get_assist_fight_boss_id());
			if (boss_ref.is_valid())
			{
				boss_ref.role_add_team(player_ref.get_unit_guid(), m_unit_team_info.captain_guid);
			}
		}
	}
	void team_cs_mgr::on_leave_team(const s_player_team_info& old_team_info)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
			return;

		if (player_ref.get_assist_fight_mgr().get_assist_fight_boss_id().is_valid())
		{
			npc& boss_ref = unit_man::get_npc(player_ref.get_assist_fight_mgr().get_assist_fight_boss_id());
			if (boss_ref.is_valid())
			{
				boss_ref.role_leave_team(player_ref.get_unit_guid(), old_team_info.captain_guid);
			}
		}
	}

	void team_cs_mgr::on_team_type_changed(const s_player_team_info& old_team_info)
	{
		// 逻辑都放到ws了
		//if (old_team_info.team_one_stop_flags < 0 && m_unit_team_info.team_one_stop_flags < 0 
		//	&& e_team_type_one_dragon == m_unit_team_info.team_type)
		//{
		//	// 从一个其他目标切换到一条龙
		//	check_one_stop_finish_flags();
		//	check_new_aim_for_one_stop();
		//}
	}

	void team_cs_mgr::on_player_transfer_finished()
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}

		if (e_map_type_big_map == base_map_system::get_map_type(my_player_ref.get_map_ent()))
		{
			if (false == m_unit_team_info.is_valid())
			{
				refresh_mission_id_when_cant_finish();
			}
		}

		check_one_stop_finish_flags(true);
		set_is_not_drop(false);	//初始化是否是助战玩家
	}

	bool team_cs_mgr::is_in_same_legion(guid_64& my_guid, guid_64& other_mem_guid)
	{
		player& my_player_ref = unit_man::get_player(my_guid);
		if (!my_player_ref.is_valid())
		{
			return false;
		}
		player& other_mem_player = unit_man::get_player(other_mem_guid);
		if (!other_mem_player.is_valid())
		{
			return false;
		}

		return my_player_ref.is_in_the_same_legion(other_mem_player);
	}

	void team_cs_mgr::share_team_mission_npc_kill(int32 npc_id, hate_info_vector const& hate_list)
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return;
		}
		if (npc_id <= 0)
		{
			return;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(my_player_ref.get_map_ent(), my_player_ref.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return;
		}
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			player& aoi_player_ref = unit_man::get_player(unit_index);
			if (aoi_player_ref.is_valid() == false)
			{
				continue;
			}
			if (aoi_player_ref.get_unit_guid() == my_player_ref.get_unit_guid())
			{
				continue; // 当前玩家不用再计算一次任务杀怪数
			}

			if (my_player_ref.is_in_same_team(aoi_player_ref) == true)
			{
				bool in_hate_list = false;
				for (int32 hate_index = 0; hate_index < hate_list.size(); ++hate_index)
				{
					if (hate_list[hate_index].unit_identifier.runtime_id == unit_index)
					{
						in_hate_list = true;
						break;
					}
				}
				if (in_hate_list == false)
				{
					//aoi_player_ref.get_mission_mgr().npc_killed(npc_id);
					aoi_player_ref.get_mission_mgr().target_check(e_mission_end_type_npc_kill, npc_id);
				}
			}
		}
	}

	void team_cs_mgr::share_team_exp(void* npc_ptr)
	{
		npc* dead_npc_ptr = static_cast<npc*>(npc_ptr);
		if (nullptr == dead_npc_ptr)
		{
			return;
		}
		npc& dead_npc_ref = *dead_npc_ptr;
		NpcTemplate* dead_npc_template = dead_npc_ref.get_npc_template();
		if (nullptr == dead_npc_template)
		{
			return;
		}

		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid() || !dead_npc_ref.is_valid())
		{
			return;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(dead_npc_ref.get_map_ent(), dead_npc_ref.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return;
		}
		auto npc_map_ptr = world_cs::get_map_by_guid<map_object>(dead_npc_ref.get_map_guid());
		if (npc_map_ptr == nullptr)
		{
			return;
		}
		int32 npc_map_template_id = npc_map_ptr->get_map_template_id();

		int32 team_same_legion_mem_num = 0;
		int32 team_member_baseexp_all = 0;
		std::vector<player*> team_mem_in_aoi;
		team_mem_in_aoi.clear();
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			player& aoi_player_ref = unit_man::get_player(unit_index);
			if (aoi_player_ref.is_valid() == false)
			{
				continue;
			}

			if (my_player_ref.is_in_same_team(aoi_player_ref) == false)
			{
				continue;
			}

			PlayerUpgradeTemplate* exp_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, aoi_player_ref.get_unit_info(e_role_info_upgrade_id));
			if (nullptr == exp_template_ptr)
			{
				continue;
			}
			team_member_baseexp_all += exp_template_ptr->BaseExp;
			team_mem_in_aoi.push_back(&aoi_player_ref);

			if (my_player_ref.is_in_the_same_legion(aoi_player_ref))
			{
				team_same_legion_mem_num++;
			}
		}

		int32 team_mem_num = team_mem_in_aoi.size();
		if (team_mem_num <= 0)
		{
			return;
		}
		float exp_extra_fix_team = 0.f;
		float exp_extra_fix_legion = 0.f;
		float raid_fix_team = 0.f;
		int32 extra_fix_team_num = team_mem_num;
		auto my_player_temp_ptr = world_cs::get_map_by_guid<map_object>(my_player_ref.get_map_guid());
		if (nullptr != my_player_temp_ptr)
		{
			if (my_player_temp_ptr->get_map_type() == e_map_type_big_map)
			{
				extra_fix_team_num = calc_team_extra_fix_num(npc_map_template_id);
			}
		}
		init_unit::get_team_share_exp_ratio(extra_fix_team_num, team_same_legion_mem_num, exp_extra_fix_team, exp_extra_fix_legion, raid_fix_team);

		int32 base_exp_value = my_player_ref.get_base_exp_of_dead_npc(dead_npc_ref);
		int32 mem_base_exp = base_exp_value / team_mem_num;

		bool is_cur_player_have_couple_in_team = false;//该局部变量用于队伍中存在自己的伴侣且怪物是自己打死的，需要给自己经验加成
		for (int32 i = 0; i < m_unit_team_info.member_num; i++)
		{
			if (my_player_ref.is_couple(m_unit_team_info.team_mems_guid[i]))
			{
				is_cur_player_have_couple_in_team = true;
			}

		}
		/*for (int32 i = 0; i < team_mem_num; i++)
		{
			player*  team_mem_ptr = team_mem_in_aoi[i];
			if (nullptr == team_mem_ptr)
			{
				continue;
			}
			if (my_player_ref.is_couple(team_mem_ptr->get_unit_guid()))
			{
				is_cur_player_have_couple_in_team = true;
			}
		}*/

		for (int32 i = 0; i < team_mem_num; i++)
		{
			player* team_mem_ptr = team_mem_in_aoi[i];
			if (nullptr == team_mem_ptr)
			{
				continue;
			}

			PlayerUpgradeTemplate* exp_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, team_mem_ptr->get_unit_info(e_role_info_upgrade_id));
			if (nullptr == exp_template_ptr)
			{
				continue;
			}

			float exp_extra_fix = exp_extra_fix_team;
			auto cur_player_map_object_ptr = world_cs::get_map_by_guid<map_object>(team_mem_ptr->get_map_guid());
			if (nullptr == cur_player_map_object_ptr)
			{
				continue;
			}
			if (cur_player_map_object_ptr->get_map_type() == e_map_type_field)
			{
				exp_extra_fix += (3 * exp_extra_fix_team);//上古遗迹额外给三倍的组队经验
			}
			if (my_player_ref.is_in_the_same_legion(*team_mem_ptr))
			{
				exp_extra_fix += exp_extra_fix_legion;
			}

			if (my_player_ref.is_couple(team_mem_ptr->get_unit_guid()))
			{
				exp_extra_fix += GAMECONFIG->CoupleAddExpRate;
			}

			if (true == is_cur_player_have_couple_in_team && my_player_ref.get_unit_guid() == team_mem_ptr->get_unit_guid())
			{
				exp_extra_fix += GAMECONFIG->CoupleAddExpRate;
			}

			if (dead_npc_template->DeadExpMode == e_kill_npc_exp_mode_player_lv)
			{
				mem_base_exp = team_mem_ptr->get_base_exp_of_dead_npc(dead_npc_ref);;
			}
			else
			{
				//mem_base_exp = base_exp_value * (exp_template_ptr->BaseExp / (float)team_member_baseexp_all);
			}

			team_mem_ptr->add_exp_with_fix(mem_base_exp, exp_extra_fix);
		}
	}

	uint32 team_cs_mgr::calc_team_extra_fix_num(int32 map_template_id)
	{
		int32 num = 0;
		for (int32 i = 0; i < m_unit_team_info.member_num; i++)
		{
			player& play_ref = unit_man::get_player(m_unit_team_info.team_mems_guid[i]);
			if (play_ref.is_valid() == false)
			{
				continue;
			}
			int32 temp_map_template_id = play_ref.get_unit_info(e_role_info_move_map_id);
			MapTemplate* temp_map_template_ptr = GET_TEMPLATE(MapTemplate, temp_map_template_id);
			if (temp_map_template_ptr == nullptr)
			{
				continue;
			}
			if (temp_map_template_ptr->Type == hld::e_map_type_big_map && temp_map_template_id == map_template_id)
			{
				num++;
			}
		}

		return num;
	}

	//void team_cs_mgr::share_team_drop_item(const guid_64& map_guid, int32 drop_template_id, s_map_pos& init_pos, s_map_pos& init_target_pos)
	//{
	//	player& my_player_ref = unit_man::get_player(m_unit_array_index);
	//	if (!my_player_ref.is_valid())
	//	{
	//		return;
	//	}
	//	if (drop_template_id <= 0)
	//	{
	//		return;
	//	}
	//	const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(my_player_ref.get_map_guid(), my_player_ref.get_old_map_pos().unit_location);
	//	if (aoi_tower_watch.empty())
	//	{
	//		return;
	//	}
	//	for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
	//	{
	//		const int32& unit_index = it->first;
	//		player& aoi_player_ref = unit_man::get_player(unit_index);
	//		if (aoi_player_ref.is_valid() == false)
	//		{
	//			continue;
	//		}
	//		if (aoi_player_ref.get_unit_guid() == my_player_ref.get_unit_guid())
	//		{
	//			continue;
	//		}

	//		if (my_player_ref.is_in_same_team(aoi_player_ref) == true)
	//		{
	//			std::vector<int32> empty_array;	//空数组
	//			world_cs::create_and_add_drop_box(map_guid, drop_template_id, init_pos, init_target_pos, aoi_player_ref.get_identifier(), empty_array);
	//		}
	//	}
	//}

	player* team_cs_mgr::get_captain_from_aoi()
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return nullptr;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(my_player_ref.get_map_ent(), my_player_ref.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return nullptr;
		}
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			player& aoi_player_ref = unit_man::get_player(unit_index);
			if (aoi_player_ref.is_valid() == false)
			{
				continue;
			}
			if (aoi_player_ref.get_unit_guid() == my_player_ref.get_unit_guid())
			{
				continue;
			}

			if (my_player_ref.is_in_same_team(aoi_player_ref) == true)
			{
				if (true == aoi_player_ref.is_player_team_captain())
				{
					return &aoi_player_ref;
				}
			}
		}
		return nullptr;
	}

	hld::e_team_type team_cs_mgr::get_team_type()
	{
		s_player_team_info& player_team_info = get_unit_team_info();
		return player_team_info.team_type;
	}

	bool team_cs_mgr::is_assist_in_raid()
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return false;
		}

		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(my_player_ref.get_map_guid());
		if (nullptr == map_object_ptr)
		{
			return false;
		}

		MapTemplate* map_cfg = map_object_ptr->get_map_template_ptr();
		if (nullptr == map_cfg)
		{
			return false;
		}

		//if (!get_is_not_drop())
		//{
		//	return false;
		//}

		if (false == is_map_type_can_assist(map_object_ptr->get_map_type(), map_cfg->Order))
		{
			return false;
		}

		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(m_unit_array_index);
		//if (false == map_record_set_ref.is_valid())
		//{
		//	return false;
		//}
		//int32 group_map_id = map_object_ptr->get_map_template_id();
		//if (map_record_set_ref.get_enter_count(group_map_id) <= (map_cfg->entercount + map_record_set_ref.get_buy_count(group_map_id) + map_record_set_ref.get_use_item_add_count(group_map_id)))
		//{
		//	return false;
		//}
		//if (map_record_set_ref.get_enter_count(map_object_ptr->get_map_template_id()) > map_cfg->EnterCount)
		//{
		//	int32 can_extra_enter_count = my_player_ref.get_vip_extra_raid_enter(map_object_ptr->get_map_template_id());
		//	if (can_extra_enter_count >= 0)
		//	{
		//		return false;
		//	}
		//}
		return true;
	}

	e_mission_slot team_cs_mgr::get_misssion_index_by_team_type(int32 team_type)
	{
		e_mission_slot mission_index = e_mission_slot_max;

		if (team_type < 0)
		{
			team_type = get_team_type();
		}

		switch (team_type)
		{
		case e_team_type_none:
			break;
		case e_team_type_mission_daily:
			mission_index = e_mission_slot_daily;
			break;
		case e_team_type_mission_against:
			mission_index = e_mission_slot_against;
			break;
		default:
			break;
		}
		return mission_index;
	}

	int32 team_cs_mgr::get_team_sub_type_id()
	{
		s_player_team_info& player_team_info = get_unit_team_info();
		return player_team_info.team_sub_type_id;
	}

	void team_cs_mgr::set_team_aim_info(e_team_type team_type, int32 sub_id)
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}

		cs2ws_team_change_aim_info change_aim_msg;
		change_aim_msg.role_guid = my_player_ref.get_unit_guid();
		change_aim_msg.team_type_id = team_type;
		change_aim_msg.team_sub_type_id = sub_id;
		connection_mgr::getInstance().send_to_ws(&change_aim_msg, sizeof(change_aim_msg));
	}

	void team_cs_mgr::sync_mission_id()
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return;
		}
		const e_mission_slot mission_index = get_misssion_index_by_team_type();
		const int32	new_mission_id = get_team_sub_type_id();
		if (new_mission_id <= 0)
		{
			return;
		}
		if (true == my_player_ref.is_player_team_captain())
		{
			return;
		}
		my_player_ref.change_mission(mission_index, new_mission_id);
	}

	void team_cs_mgr::refresh_mission_id()
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return;
		}
		for (int32 i = e_mission_slot_daily; i <= e_mission_slot_against; i++)
		{
			e_mission_slot mission_s_type = (e_mission_slot)i;
			if (false == my_player_ref.is_special_mission_unlocked(mission_s_type))
			{
				continue;
			}
			if (true == my_player_ref.is_cur_mission_accord_level(mission_s_type))
			{
				continue;
			}
			const int32	new_mission_id = my_player_ref.get_new_mission_id(mission_s_type);
			my_player_ref.change_mission(mission_s_type, new_mission_id);
		}
	}

	void team_cs_mgr::refresh_mission_id_when_cant_finish()
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (!my_player_ref.is_valid())
		{
			return;
		}
		for (int32 i = e_mission_slot_daily; i <= e_mission_slot_against; i++)
		{
			e_mission_slot mission_s_type = (e_mission_slot)i;
			if (false == my_player_ref.is_special_mission_unlocked(mission_s_type))
			{
				continue;
			}
			if (false == my_player_ref.is_cur_mission_high_level(mission_s_type))
			{
				continue;
			}
			const int32	new_mission_id = my_player_ref.get_new_mission_id(mission_s_type);
			my_player_ref.change_mission(mission_s_type, new_mission_id);
		}
	}

	void team_cs_mgr::check_update_team_mission()
	{
		//理论上新增mem也会走到这里 到时候看看有么有bug
		if (false == m_unit_team_info.is_valid())
		{
			return;
		}
		if (m_unit_team_info.team_type != e_team_type_mission_daily && m_unit_team_info.team_type != e_team_type_mission_against)
		{
			return;
		}
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}

		e_mission_slot mission_type = get_misssion_index_by_team_type();
		if (false == my_player_ref.is_special_mission_unlocked(mission_type))
		{
			return;
		}

		if (my_player_ref.is_special_mission_finished(mission_type))
		{
			return;
		}

		int32 cur_mission_id = my_player_ref.get_mission_id(mission_type);
		if (m_unit_team_info.team_sub_type_id == cur_mission_id)
		{
			return;
		}

		if (m_unit_team_info.captain_guid == my_player_ref.get_unit_guid())
		{
			set_team_aim_info(m_unit_team_info.team_type, cur_mission_id);
		}
		else
		{
			sync_mission_id();
		}
	}

	void  team_cs_mgr::give_captain_reward()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}

		if (m_unit_team_info.captain_guid != player_ref.get_unit_guid() || m_unit_team_info.member_num < 2)
		{
			return;
		}
		//如果为助战则不发放队长奖励
		if (is_assist_in_raid() == false)
		{
			return;
		}
		if (get_is_not_drop() == true)
		{
			return;
		}

		bool is_same_legion = false;
		for (int32 i = 1; i <= m_unit_team_info.member_num; i++)
		{
			player& player_mem = unit_man::get_player(m_unit_team_info.team_mems_guid[i]);
			if (m_unit_team_info.captain_guid != m_unit_team_info.team_mems_guid[i] && player_ref.is_in_the_same_legion(player_mem))
			{
				is_same_legion = true;
			}
		}
		if (!is_same_legion)
		{
			return;
		}

		if (player_ref.check_can_get_be_helped_reward())
		{
			auto map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
			if (nullptr == map_object_ptr)
			{
				return;
			}
			MapTemplate* map_cfg = map_object_ptr->get_map_template_ptr();
			if (nullptr == map_cfg)
			{
				return;
			}
			std::vector<int32> rwd_list = map_cfg->CaptainRewardItems;
			int32 len = rwd_list.size();
			if (len > 0 && len % 2 == 0)
			{
				int32 money_type = e_money_type_assist_fighting;
				int32 money_num = player_ref.get_help_value(rwd_list[1]);

				player_ref.add_money_or_exp((e_money_type)money_type, money_num, e_server_log_add_money_captain_reward);
				player_ref.delete_get_be_helped_reward_times(money_num);

				vector<s_item_template_info> promp_item_data;
				promp_item_data.push_back({ rwd_list[0], money_num, 1 });
				player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
			}
		}
	}

	void team_cs_mgr::check_raid_team_finish_state() //副本扣次数时可能改变了flags 走这里必然是有队伍的
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}

		if (false == init_unit::is_team_type_raid_for_team(m_unit_team_info.team_type))
		{
			return;
		}

		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(my_player_ref.get_map_guid());
		if (nullptr == map_object_ptr)
		{
			return;
		}

		int32 map_temp_id = map_object_ptr->get_map_template_id();
		if (m_unit_team_info.team_sub_type_id != map_temp_id)
		{
			return;
		}

		check_one_stop_finish_flags();
	}

	void team_cs_mgr::check_one_stop_finish_flags(bool is_player_init) //任务完成时可能改变了flags 主线任务会解锁 日常讨伐会完成
	{
		ZoneScoped;
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}

		auto map_entity = my_player_ref.get_map_ent();
		if (nullptr == map_entity)
		{
			CONSOLE_ERROR("team_cs_mgr::check_one_stop_finish_flags map_entity is null");
			return;
		}
		auto base_map_cp = map_entity->get_component<base_map_component>();
		if (false == base_map_cp.isValid())
		{
			CONSOLE_ERROR("team_cs_mgr::check_one_stop_finish_flags base_map_cp is null");
			return;
		}

		int32 new_one_stop = get_self_one_stop_flags();
		if (new_one_stop == m_self_one_stop_flags)
		{
			// 没变
			return;
		}

		if (m_unit_team_info.is_valid() && false == is_player_init)
		{
			// 改变team_ws里的成员数据 及队伍数据
			cs2ws_after_team_aim_finish_op aim_finish_op_msg;

			int32 map_temp_id = base_map_cp->m_map_id;
			if (m_unit_team_info.team_sub_type_id == map_temp_id)
			{
				//aim_finish_op_msg.map_check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(m_unit_array_index, map_temp_id);
			}

			aim_finish_op_msg.role_guid = my_player_ref.get_unit_guid();
			aim_finish_op_msg.role_one_stop_flags = new_one_stop;
			connection_mgr::getInstance().send_to_ws(&aim_finish_op_msg, sizeof(aim_finish_op_msg));
		}

		// 改变客户端flags缓存
		team_proto_self_os_flags_update update_os_msg;
		update_os_msg.set_own_os_flags(new_one_stop);
		my_player_ref.send_message_to_self(&update_os_msg, e_msgindex_s2c_self_os_flags_update);

		m_self_one_stop_flags = new_one_stop;
	}

	void team_cs_mgr::check_new_aim_for_one_stop(bool from_raid_map)
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}
		if (false == m_unit_team_info.is_valid() || my_player_ref.get_unit_guid() != m_unit_team_info.captain_guid)
		{
			return;
		}

		if (false == init_unit::is_team_type_one_stop(m_unit_team_info.team_type))
		{
			return;
		}

		if (m_unit_team_info.team_one_stop_flags < 0)
		{
			return; //不是一条龙属性的队伍
		}

		if (from_raid_map)
		{
			auto map_object_ptr = world_cs::get_map_by_guid<map_object>(my_player_ref.get_map_guid());
			if (nullptr == map_object_ptr)
			{
				return;
			}
			int32 map_temp_id = map_object_ptr->get_map_template_id();
			if (m_unit_team_info.team_sub_type_id != map_temp_id)
			{
				return;
			}
		}

		e_team_type matched_team_type = m_unit_team_info.team_type;
		int32 matched_team_sub_id = m_unit_team_info.team_sub_type_id;
		get_self_adapt_team_aim(matched_team_type, matched_team_sub_id);
		if (matched_team_type == m_unit_team_info.team_type && matched_team_sub_id == m_unit_team_info.team_sub_type_id)
		{
			return;
		}

		set_team_aim_info(matched_team_type, matched_team_sub_id);
	}

	int32 team_cs_mgr::get_self_one_stop_flags(int32 tempid)
	{
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return 0;
		}
		int32 player_level = my_player_ref.get_unit_info(e_role_info_exp_level);

		TeamAimTypeTemplate* one_stop_cfg = GET_TEMPLATE(TeamAimTypeTemplate, first_team_aim_template_id + e_team_type_one_dragon);
		if (nullptr == one_stop_cfg)
		{
			return 0;
		}
		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(m_unit_array_index);
		//if (false == map_record_set_ref.is_valid())
		//{
		//	return 0;
		//}

		int32 ret_flags = 0;
		int32 sub_id_size = one_stop_cfg->SubTypeId.size();
		for (int32 idx = 0; idx < sub_id_size; idx++)
		{
			TeamAimTypeTemplate* _aim_cfg = GET_TEMPLATE(TeamAimTypeTemplate, one_stop_cfg->SubTypeId[idx]);
			if (nullptr == _aim_cfg)
			{
				continue;
			}

			if (_aim_cfg->SubTypeId.size() > 0)
			{
				int32 map_temp_id = _aim_cfg->SubTypeId[0];
				if (true == m_unit_team_info.is_valid())
				{
					if (tempid <= 0)
					{
						tempid = m_unit_team_info.team_sub_type_id;
					}
					MapTemplate* map_cfg = GET_TEMPLATE(MapTemplate, tempid);
					if (nullptr != map_cfg)
					{
						int32 group_temp_id = map_cfg->GroupType;
						if (group_temp_id == map_temp_id)
						{
							map_temp_id = tempid;
						}
					}
				}
				MapTemplate* raid_map_cfg = GET_TEMPLATE(MapTemplate, map_temp_id);
				if (nullptr == raid_map_cfg)
				{
					continue;
				}
				if (player_level < raid_map_cfg->EnterLevel)
				{
					init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_locked);
					continue;
				}
				if (false == my_player_ref.get_func_unlock_mgr().is_func_unlock(raid_map_cfg->UnlockCondition))
				{
					init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_locked);
					continue;
				}
				//e_error_code check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(my_player_ref.get_array_index(), map_temp_id);
				//if (e_error_code_success != check_ret && e_error_code_map_lack_of_times != check_ret)
				//{
				//	init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_locked);
				//	continue;
				//}

				//int32 enter_count = map_record_set_ref.get_enter_count(map_temp_id);
				//int32 buy_count = map_record_set_ref.get_buy_count(map_temp_id);
				//if (enter_count >= (raid_map_cfg->EnterCount + buy_count))
				//{
				//	init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_lack_of_times);
				//	continue;
				//}
				init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_ok);
			}
			else
			{
				e_mission_slot mission_index = get_misssion_index_by_team_type(_aim_cfg->AimType);
				if (mission_index != e_mission_slot_daily && mission_index != e_mission_slot_against)
				{
					continue;
				}

				if (false == my_player_ref.is_special_mission_unlocked(mission_index))
				{
					init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_locked);
					continue;
				}
				if (my_player_ref.is_special_mission_finished(mission_index))
				{
					init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_lack_of_times);
					continue;
				}
				init_unit::set_flag_data_2bit(ret_flags, idx + 1, e_team_one_stop_aim_ok);
			}
		}

		return ret_flags;
	}

	void team_cs_mgr::get_self_adapt_team_aim(e_team_type& adapt_type, int32& adapt_sub_id)
	{
		adapt_type = e_team_type_one_dragon;
		adapt_sub_id = -1;
		player& my_player_ref = unit_man::get_player(m_unit_array_index);
		if (false == my_player_ref.is_valid())
		{
			return;
		}
		int32 player_level = my_player_ref.get_unit_info(e_role_info_exp_level);

		TeamAimTypeTemplate* one_stop_cfg = GET_TEMPLATE(TeamAimTypeTemplate, first_team_aim_template_id + e_team_type_one_dragon);
		if (nullptr == one_stop_cfg)
		{
			return;
		}
		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(m_unit_array_index);
		//if (false == map_record_set_ref.is_valid())
		//{
		//	return;
		//}

		int32 sub_id_size = one_stop_cfg->SubTypeId.size();
		for (int32 idx = 0; idx < sub_id_size; idx++)
		{
			TeamAimTypeTemplate* _aim_cfg = GET_TEMPLATE(TeamAimTypeTemplate, one_stop_cfg->SubTypeId[idx]);
			if (nullptr == _aim_cfg)
			{
				continue;
			}

			if (_aim_cfg->SubTypeId.size() > 0)
			{
				int32 final_map_temp_id = _aim_cfg->SubTypeId[0];
				MapTemplate* raid_map_cfg = GET_TEMPLATE(MapTemplate, final_map_temp_id);
				if (nullptr == raid_map_cfg)
				{
					continue;
				}
				if (player_level < raid_map_cfg->EnterLevel)
				{
					continue;
				}
				if (false == my_player_ref.get_func_unlock_mgr().is_func_unlock(raid_map_cfg->UnlockCondition))
				{
					continue;
				}
				//int32 enter_count = map_record_set_ref.get_enter_count(final_map_temp_id);
				//int32 buy_count = map_record_set_ref.get_buy_count(final_map_temp_id);
				//if (enter_count >= (raid_map_cfg->EnterCount + buy_count))
				//{
				//	continue;
				//}
				//for (int32 map_idx = _aim_cfg->SubTypeId.size() - 1; map_idx > 0; map_idx--)
				//{
				//	int32 tmp_map_id = _aim_cfg->SubTypeId[map_idx];
				//	map_record& tmp_map_record = map_record_set_ref.get_map_record(tmp_map_id);
				//	if (tmp_map_record.is_map_clearance())
				//	{
				//		final_map_temp_id = tmp_map_id;
				//		break;
				//	}
				//}
				adapt_type = (e_team_type)_aim_cfg->AimType;
				adapt_sub_id = final_map_temp_id;
				break;
			}
			else
			{
				e_mission_slot mission_index = get_misssion_index_by_team_type(_aim_cfg->AimType);
				if (mission_index != e_mission_slot_daily && mission_index != e_mission_slot_against)
				{
					continue;
				}

				if (false == my_player_ref.is_special_mission_unlocked(mission_index))
				{
					continue;
				}
				if (my_player_ref.is_special_mission_finished(mission_index))
				{
					continue;
				}
				adapt_type = (e_team_type)_aim_cfg->AimType;
				break;
			}
		}
	}

	bool team_cs_mgr::is_team_type_raid(e_team_type team_type)
	{
		return init_unit::is_team_type_raid_map(team_type);
	}

	bool team_cs_mgr::is_map_type_can_assist(e_map_type map_type, int32 map_order)
	{
		if (map_order == e_map_order_type_multiplayer_raid)
		{
			return true;
		}
		switch (map_type)
		{
			//case e_map_type_princess_guard_war:
			//case e_map_type_lava_hellfire:
			//case e_map_type_sea_of_hate:
		case e_map_type_single_assist_boss:
			//case e_map_type_broken_sky:
			return true;
		}
		return false;
	}

	void team_cs_mgr::c2cs_create_team(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		team_proto_create_team req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		c2cs_create_team_logic(player_ref, req);
		//if (player_ref.get_assist_fight_mgr().is_assist_helper())
		//{
		//	player_ref.send_notice("90204214");
		//	return;
		//}
		//e_team_type team_type = (e_team_type)(req.team_type());
		//int32 team_sub_type_id = req.team_sub_type_id();

		//if (cell_server::getInstance().get_is_self_gate_server() == false)
		//{
		//	e_error_code check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(unit_index, team_sub_type_id);
		//	if (is_team_type_raid(team_type) && e_error_code_success != check_ret && e_error_code_map_lack_of_times != check_ret)
		//	{
		//		team_proto_team_error team_error_msg;
		//		team_error_msg.set_error_type(e_team_error_raid_condition_less);
		//		player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
		//		return;
		//	}
		//}

		////跨服状态下不允许创建副本队伍
		//if (is_team_type_raid(team_type) && false == player_ref.is_self_server())
		//{
		//	const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
		//	player_ref.send_notice(notice_str);
		//	return;
		//}

		//// 已经在队伍中就不能创建队伍
		//if (player_ref.is_player_in_team())
		//{
		//	team_proto_team_error team_error_msg;
		//	team_error_msg.set_error_type(e_team_error_player_already_join_team);
		//	player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
		//	return;
		//}

		//int32 required_fighting_power = req.required_fighting_power();
		//int32 required_level = req.required_level();
		//bool auto_accept_new_member = req.auto_accept_new_member();
		//
		//cs2ws_create_team create_team_msg;
		//if (e_team_type_one_dragon == team_type)
		//{
		//	// 如果是一条龙
		//	create_team_msg.team_one_stop_flags = player_ref.get_team_cs_mgr().get_self_one_stop_flags();
		//	player_ref.get_team_cs_mgr().get_self_adapt_team_aim(team_type, team_sub_type_id);
		//}

		//create_team_msg.team_type = team_type;
		//create_team_msg.team_sub_type_id = team_sub_type_id;
		//create_team_msg.required_fighting_power = required_fighting_power;
		//create_team_msg.required_level = required_level;
		//create_team_msg.auto_accept_new_member = auto_accept_new_member;
		//player_ref.get_team_cs_mgr().gen_player_team_member_info(create_team_msg.captain_info, team_type, team_sub_type_id);

		//connection_mgr::getInstance().send_to_ws( &create_team_msg, sizeof(create_team_msg));
	}

	void team_cs_mgr::c2cs_create_team_logic(player& player_ref, team_proto_create_team& req)
	{
		if (player_ref.get_assist_fight_mgr().is_assist_helper())
		{
			player_ref.send_notice("90204214");
			return;
		}
		int32 unit_index = player_ref.get_array_index();
		e_team_type team_type = (e_team_type)(req.team_type());
		int32 team_sub_type_id = req.team_sub_type_id();

		if (cell_server::getInstance().get_is_self_gate_server() == false)
		{
			//e_error_code check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(unit_index, team_sub_type_id);
			//if (is_team_type_raid(team_type) && e_error_code_success != check_ret && e_error_code_map_lack_of_times != check_ret)
			//{
			//	team_proto_team_error team_error_msg;
			//	team_error_msg.set_error_type(e_team_error_raid_condition_less);
			//	player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
			//	return;
			//}
		}

		//跨服状态下不允许创建副本队伍
		if (is_team_type_raid(team_type) && false == player_ref.is_self_server())
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
			player_ref.send_notice(notice_str);
			return;
		}

		// 已经在队伍中就不能创建队伍
		if (player_ref.is_player_in_team())
		{
			team_proto_team_error team_error_msg;
			team_error_msg.set_error_type(e_team_error_player_already_join_team);
			player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
			return;
		}

		int32 required_fighting_power = req.required_fighting_power();
		int32 required_level = req.required_level();
		bool auto_accept_new_member = req.auto_accept_new_member();

		cs2ws_create_team create_team_msg;
		if (e_team_type_one_dragon == team_type)
		{
			// 如果是一条龙
			create_team_msg.team_one_stop_flags = player_ref.get_team_cs_mgr().get_self_one_stop_flags();
			player_ref.get_team_cs_mgr().get_self_adapt_team_aim(team_type, team_sub_type_id);
		}

		create_team_msg.team_type = team_type;
		create_team_msg.team_sub_type_id = team_sub_type_id;
		create_team_msg.required_fighting_power = required_fighting_power;
		create_team_msg.required_level = required_level;
		create_team_msg.auto_accept_new_member = auto_accept_new_member;
		player_ref.get_team_cs_mgr().gen_player_team_member_info(create_team_msg.captain_info, team_type, team_sub_type_id);

		connection_mgr::getInstance().send_to_ws(&create_team_msg, sizeof(create_team_msg));
	}

	void team_cs_mgr::c2cs_create_team_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(team_proto_create_team, c2cs_create_team);
	}

	void team_cs_mgr::c2cs_change_team_type(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		team_proto_update_team_attribute req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		c2cs_change_team_type_logic(player_ref, req);
		//int32 limit_fight_power = -1;
		//int32 limit_level = -1;

		//e_team_type team_type_id = (e_team_type)req.team_type_id();
		////跨服状态下不允许创建副本队伍
		//if (is_team_type_raid(team_type_id) && false == player_ref.is_self_server())
		//{
		//	const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
		//	player_ref.send_notice(notice_str);
		//	return;
		//}
		//int32 team_sub_type_id = -1;
		//bool is_auto_match = req.is_auto_match();
		//if (req.has_limit_fight_power())
		//{
		//	limit_fight_power = req.limit_fight_power();
		//}
		//if (req.has_limit_level())
		//{
		//	limit_level = req.limit_level();
		//}
		//if (req.has_team_sub_type_id())
		//{
		//	team_sub_type_id = req.team_sub_type_id();
		//}

		//int32 one_stop_ = player_ref.get_team_cs_mgr().get_self_one_stop_flags(req.team_sub_type_id());
		//cs2ws_change_team_type change_msg;
		//change_msg.one_stop_ = one_stop_;
		//change_msg.sub_temp_id = req.team_sub_type_id();
		//change_msg.player_guid = player_ref.get_unit_guid();
		//change_msg.team_guid = player_ref.get_team_guid();
		//change_msg.is_auto_match = is_auto_match;
		//change_msg.limit_fight_power = limit_fight_power;
		//change_msg.limit_level = limit_level;
		//change_msg.has_auto_match = req.has_is_auto_match();
		//change_msg.has_team_type = req.has_team_type_id();
		//change_msg.has_team_sub_type_id = req.has_team_sub_type_id();
		//change_msg.team_type = team_type_id;
		//s_player_team_info player_team_info = player_ref.get_team_cs_mgr().get_unit_team_info();
		//for (int32 i = 0; i < max_team_member_num; i++)
		//{
		//	player& team_player_ref = unit_man::get_player(player_team_info.team_mems_guid[i]);
		//	if (team_player_ref.is_valid() == false)
		//	{
		//		continue;
		//	}
		//	team_player_ref.get_team_cs_mgr().gen_player_team_member_info(change_msg.team_member_info[i], player_ref.get_team_cs_mgr().get_unit_team_info().team_type, team_sub_type_id);
		//}
		//connection_mgr::getInstance().send_to_ws( &change_msg, sizeof(change_msg));

	}

	void team_cs_mgr::c2cs_change_team_type_logic(player& player_ref, team_proto_update_team_attribute& req)
	{
		int32 limit_fight_power = -1;
		int32 limit_level = -1;

		e_team_type team_type_id = (e_team_type)req.team_type_id();
		//跨服状态下不允许创建副本队伍
		if (is_team_type_raid(team_type_id) && false == player_ref.is_self_server())
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
			player_ref.send_notice(notice_str);
			return;
		}
		int32 team_sub_type_id = -1;
		bool is_auto_match = req.is_auto_match();
		if (req.has_limit_fight_power())
		{
			limit_fight_power = req.limit_fight_power();
		}
		if (req.has_limit_level())
		{
			limit_level = req.limit_level();
		}
		if (req.has_team_sub_type_id())
		{
			team_sub_type_id = req.team_sub_type_id();
		}

		int32 one_stop_ = player_ref.get_team_cs_mgr().get_self_one_stop_flags(req.team_sub_type_id());
		cs2ws_change_team_type change_msg;
		change_msg.one_stop_ = one_stop_;
		change_msg.sub_temp_id = req.team_sub_type_id();
		change_msg.player_guid = player_ref.get_unit_guid();
		change_msg.team_guid = player_ref.get_team_guid();
		change_msg.is_auto_match = is_auto_match;
		change_msg.limit_fight_power = limit_fight_power;
		change_msg.limit_level = limit_level;
		change_msg.has_auto_match = req.has_is_auto_match();
		change_msg.has_team_type = req.has_team_type_id();
		change_msg.has_team_sub_type_id = req.has_team_sub_type_id();
		change_msg.team_type = team_type_id;
		s_player_team_info player_team_info = player_ref.get_team_cs_mgr().get_unit_team_info();
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			player& team_player_ref = unit_man::get_player(player_team_info.team_mems_guid[i]);
			if (team_player_ref.is_valid() == false)
			{
				continue;
			}
			team_player_ref.get_team_cs_mgr().gen_player_team_member_info(change_msg.team_member_info[i], player_ref.get_team_cs_mgr().get_unit_team_info().team_type, team_sub_type_id);
		}
		connection_mgr::getInstance().send_to_ws(&change_msg, sizeof(change_msg));
	}

	void team_cs_mgr::c2cs_change_team_type_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(team_proto_update_team_attribute, c2cs_change_team_type)
	}

	void team_cs_mgr::c2cs_join_team(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		team_proto_join_team req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		c2cs_join_team_logic(player_ref, req);
		//if (player_ref.get_assist_fight_mgr().check_cant_join_team())
		//{
		//	player_ref.send_notice("90204214");
		//	return;
		//}
		//guid_64 tar_player_guid = req.tar_player_guid();
		//player& tar_player_ref = unit_man::get_player(tar_player_guid);

		//if (false == tar_player_ref.is_valid())
		//{
		//	team_proto_team_error team_error_msg;
		//	team_error_msg.set_error_type(e_team_error_player_already_offline);		
		//	player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
		//	return;
		//}
		//if (false == player_ref.is_self_server(tar_player_ref.get_array_index()))
		//{
		//	const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
		//	player_ref.send_notice(notice_str);
		//	return;
		//}
		//guid_64 join_team_guid(req.team_guid());
		//if (join_team_guid.is_valid() == false)
		//{
		//	team_proto_team_error team_error_msg;
		//	team_error_msg.set_error_type(e_team_error_team_already_dissolve);
		//	player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
		//	return;
		//}

		//cs2ws_join_team join_team_msg;
		//join_team_msg.team_guid = join_team_guid;
		//tar_player_ref.get_team_cs_mgr().gen_player_team_member_info(join_team_msg.member_info);
		//connection_mgr::getInstance().send_to_ws( &join_team_msg, sizeof(join_team_msg));
	}

	void team_cs_mgr::c2cs_join_team_logic(player& player_ref, team_proto_join_team& req)
	{
		if (player_ref.get_assist_fight_mgr().check_cant_join_team())
		{
			player_ref.send_notice("90204214");
			return;
		}
		guid_64 tar_player_guid = req.tar_player_guid();
		player& tar_player_ref = unit_man::get_player(tar_player_guid);

		if (false == tar_player_ref.is_valid())
		{
			team_proto_team_error team_error_msg;
			team_error_msg.set_error_type(e_team_error_player_already_offline);
			player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
			return;
		}
		if (false == player_ref.is_self_server(tar_player_ref.get_array_index()))
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
			player_ref.send_notice(notice_str);
			return;
		}
		guid_64 join_team_guid(req.team_guid());
		if (join_team_guid.is_valid() == false)
		{
			team_proto_team_error team_error_msg;
			team_error_msg.set_error_type(e_team_error_team_already_dissolve);
			player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
			return;
		}

		cs2ws_join_team join_team_msg;
		join_team_msg.team_guid = join_team_guid;
		tar_player_ref.get_team_cs_mgr().gen_player_team_member_info(join_team_msg.member_info);
		connection_mgr::getInstance().send_to_ws(&join_team_msg, sizeof(join_team_msg));
	}

	void team_cs_mgr::c2cs_join_team_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(team_proto_join_team, c2cs_join_team);
	}

	void team_cs_mgr::ws2cs_apply_to_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_apply_to_join_team* packet = static_cast<const ws2cs_apply_to_join_team*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_apply_to_join_team))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;

		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (is_team_type_raid(packet->team_type_id))
		{
			int32 team_map_id = packet->team_sub_type_id;
			if (team_map_id <= 0)
			{
				return;
			}
			//e_error_code check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(player_ref.get_array_index(), team_map_id);
			//if (e_error_code_success != check_ret && e_error_code_map_lack_of_times != check_ret)
			//{
			//	team_proto_team_error team_error_msg;
			//	team_error_msg.set_error_type(e_team_error_raid_condition_less);
			//	player_ref.send_message_to_self(&team_error_msg, e_msgindex_s2c_team_error);
			//	return;
			//}
		}

		cs2ws_apply_to_join_team join_team_apply_msg;
		join_team_apply_msg.team_guid = packet->team_guid;
		player_ref.get_team_cs_mgr().gen_player_team_member_info(join_team_apply_msg.applicant_info, packet->team_type_id, packet->team_sub_type_id);
		connection_mgr::getInstance().send_to_ws(&join_team_apply_msg, sizeof(join_team_apply_msg));
	}

	void team_cs_mgr::ws2cs_set_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_set_player_team_info* packet = static_cast<const ws2cs_set_player_team_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_set_player_team_info))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;

		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.get_team_cs_mgr().set_player_team_info(packet->player_team_info);
	}

	void team_cs_mgr::ws2cs_clear_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_clear_player_team_info* packet = static_cast<const ws2cs_clear_player_team_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_clear_player_team_info))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;

		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		const s_player_team_info& team_info_cur = player_ref.get_team_cs_mgr().get_unit_team_info();

		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
		if (nullptr == map_object_ptr)
		{
			return;
		}

		e_map_type cur_map_type = map_object_ptr->get_map_type();
		// 在组队副本中 需要强制传出
		int32 cur_map_template_id = player_ref.get_unit_info(e_role_info_move_map_id);
		if (is_team_type_raid(team_info_cur.team_type) && team_info_cur.team_sub_type_id == cur_map_template_id && cur_map_type != e_map_type_field)
		{
			player_ref.transfer_by_template(player_ref.get_unit_info(hld::e_role_info_main_map_id), player_ref.get_main_line_id(), 0, guid_64(), 0);
		}

		player_ref.get_team_cs_mgr().clear_player_team_info();
		player_ref.get_team_cs_mgr().refresh_mission_id();
		//player_ref.get_team_cs_mgr().refresh_mission_id_when_cant_finish();
	}

	// 	void team_cs_mgr::ws2cs_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	// 	{
	// 		const ws2cs_join_team* packet = static_cast<const ws2cs_join_team*>(data_ptr);
	// 		if (nullptr == packet)
	// 		{
	// 			return;
	// 		}
	// 		if (data_len != sizeof(ws2cs_join_team))
	// 		{
	// 			return;
	// 		}
	// 
	// 		guid_64 role_guid = packet->role_guid;
	// 		guid_64 team_guid = packet->team_guid;
	// 
	// 		player& player_ref = unit_man::get_player(role_guid);
	// 		if (false == player_ref.is_valid())
	// 		{
	// 			return;
	// 		}
	// 
	// 		cs2ws_join_team join_team_msg;
	// 		join_team_msg.team_guid = team_guid;
	// 		player_ref.get_team_cs_mgr().gen_player_team_member_info(join_team_msg.member_info);
	// 		connection_mgr::getInstance().send_to_all_ws( &join_team_msg, sizeof(join_team_msg));
	// 	}

	void team_cs_mgr::ws2cs_transfer_team_member_to_scene_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_transfer_team_member_to_scene* packet = static_cast<const ws2cs_transfer_team_member_to_scene*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_transfer_team_member_to_scene))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (!player_ref.is_valid())
		{
			return;
		}

		//e_error_code check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(player_ref.get_array_index(), packet->map_template_id);
		//if (e_error_code_success != check_ret && e_error_code_map_lack_of_times != check_ret)
		//{
		//	return;
		//}

		int32 map_template_id = packet->map_template_id;
		guid_64 map_guid = packet->map_guid;
		player_ref.transfer_by_template(map_template_id, 0, 0, map_guid, 0);
	}

	void team_cs_mgr::ws2cs_team_common_check_in_cs_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_team_common_check_in_cs* packet = static_cast<const ws2cs_team_common_check_in_cs*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_team_common_check_in_cs))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (!player_ref.is_valid())
		{
			return;
		}

		e_team_type final_team_type = packet->team_type_id;
		int32 final_sub_type_id = packet->team_sub_type_id;
		int32 final_own_flags = -1;
		int32 final_is_match_one_stop = false;
		e_error_code final_check_ret = e_error_code_success;

		if (is_team_type_raid(final_team_type))
		{
			//final_check_ret = map_record_mgr::get_instance().check_can_transfer_to_scene(player_ref.get_array_index(), final_sub_type_id);
		}

		if (e_team_common_check_invoker_auto_match_team == packet->check_invoker)
		{
			if (e_team_type_one_dragon == final_team_type)
			{
				// 如果是一条龙
				player_ref.get_team_cs_mgr().get_self_adapt_team_aim(final_team_type, final_sub_type_id);

				if (e_team_type_one_dragon == final_team_type)
				{
					// 说明所有一条龙目标没次数了
					final_check_ret = e_error_code_map_lack_of_times;
				}
				else
				{
					final_is_match_one_stop = true;
				}
			}

			final_own_flags = player_ref.get_team_cs_mgr().get_self_one_stop_flags();
		}

		cs2ws_team_common_check_in_cs team_common_check_in_cs_msg;
		team_common_check_in_cs_msg.role_guid = packet->role_guid;
		team_common_check_in_cs_msg.team_guid = packet->team_guid;
		team_common_check_in_cs_msg.team_type_id = final_team_type;
		team_common_check_in_cs_msg.team_sub_type_id = final_sub_type_id;
		team_common_check_in_cs_msg.check_invoker = packet->check_invoker;
		team_common_check_in_cs_msg.map_check_ret = final_check_ret;
		team_common_check_in_cs_msg.own_one_stop_flags = final_own_flags;
		team_common_check_in_cs_msg.is_match_one_stop = final_is_match_one_stop;

		connection_mgr::getInstance().send_to_ws(&team_common_check_in_cs_msg, sizeof(team_common_check_in_cs_msg));
	}

	void team_cs_mgr::ws2cs_sync_team_start_aim_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_sync_team_start_aim* packet = static_cast<const ws2cs_sync_team_start_aim*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_sync_team_start_aim))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (!player_ref.is_valid())
		{
			return;
		}

		//暂时木有逻辑
	}

	void team_cs_mgr::lua_ws2cs_apply_to_join_team_func(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::apply_to_join_team msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_apply_to_join_team packet;

		packet.from_proto(msg);

		ws2cs_apply_to_join_team_func(conn_index, &packet, sizeof(ws2cs_apply_to_join_team));
	}

	void team_cs_mgr::lua_ws2cs_set_player_team_info_func(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::set_player_team_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_set_player_team_info packet;

		packet.from_proto(msg);

		ws2cs_set_player_team_info_func(conn_index, &packet, sizeof(ws2cs_set_player_team_info));
	}

	void team_cs_mgr::lua_ws2cs_clear_player_team_info_func(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::clear_player_team_info msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_clear_player_team_info packet;

		packet.from_proto(msg);
		ws2cs_clear_player_team_info_func(conn_index, &packet, sizeof(ws2cs_clear_player_team_info));

	}

	void team_cs_mgr::lua_ws2cs_transfer_team_member_to_scene_func(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::transfer_team_member_to_scene msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_transfer_team_member_to_scene packet;
		packet.from_proto(msg);

		ws2cs_transfer_team_member_to_scene_func(conn_index, &packet, sizeof(ws2cs_transfer_team_member_to_scene));
	}

	void team_cs_mgr::lua_ws2cs_team_common_check_in_cs_func(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::team_common_check_in_cs msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_team_common_check_in_cs packet;
		packet.from_proto(msg);

		ws2cs_team_common_check_in_cs_func(conn_index, &packet, sizeof(ws2cs_team_common_check_in_cs));
	}

	void team_cs_mgr::lua_ws2cs_sync_team_start_aim_func(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		hld::ws2cs_proto::sync_team_start_aim msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_sync_team_start_aim packet;
		packet.from_proto(msg);

		ws2cs_team_common_check_in_cs_func(conn_index, &packet, sizeof(ws2cs_sync_team_start_aim));

	}

	team_set::team_set()
	{
		clear_data();
	}

	team_set::~team_set()
	{

	}

	void team_set::clear_data()
	{
		m_team_members.clear();
		m_team_guid.clear_data();
		team_member_num = 0;
	}

	void team_set::init(guid_64 team_guid)
	{
		clear_data();
		m_team_guid = team_guid;
	}

	hld::guid_64 team_set::get_team_guid()
	{
		return m_team_guid;
	}

	player* team_set::get_player_ptr(const int32 index)
	{
		if (0 > index || index >= get_team_member_num())
		{
			return nullptr;
		}
		return m_team_members[index];
	}

	int32 team_set::get_team_member_num()
	{
		return team_member_num;
	}

	bool team_set::join_team(player* player_ptr)
	{
		if (nullptr == player_ptr)
		{
			return false;
		}
		const guid_64 team_guid = player_ptr->get_team_guid();
		if (team_guid != get_team_guid())
		{
			return false;
		}
		if (true == has_in_team(player_ptr->get_unit_guid()))
		{
			return false;
		}
		add_to_team(player_ptr);
		team_member_num++;
		return true;
	}

	bool team_set::has_in_team(guid_64 player_guid)
	{
		const int32 vec_size = get_team_member_num();
		for (int32 i = 0; i < vec_size; i++)
		{
			player* player_ptr = m_team_members[i];
			if (nullptr == player_ptr)
			{
				continue;
			}
			if (player_ptr->get_unit_guid() == player_guid)
			{
				return true;
			}
		}
		return false;
	}

	void team_set::add_to_team(player* player_ptr)
	{
		m_team_members.push_back(player_ptr);
	}
}
