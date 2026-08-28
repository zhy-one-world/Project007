/********************************************************************
	created:	2015/03/21
	created:	21:3:2015   21:36
	file base:	map_cs
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "internet/belief_rune.pb.h"
#include "cell_server.hpp"
#include "cell_server.hpp"
#include "connection_mgr.hpp"
#include "internal/cross_server_harry_msg.hpp"
#include "internal/world_boss_msg.hpp"
#include "Logic/activity_def.hpp"
#include "Logic/char_def.hpp"
#include "Logic/npc_def.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "map_cs.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "internet/net.pb.h"
#include "../npc.hpp"
#include "template/ActivityCommonConfigTemplate_S.h"
#include "belief_rune_def.hpp"

namespace hld
{
	map_cs::map_cs() : map_object()
	{
		clear_data();
	}

	map_cs::~map_cs()
	{
	}

	void map_cs::clear_data()
	{
		map_object::clear_data();
		m_map_state = e_map_state_in_game;

		m_target_time_spawn_vec.clear();
		m_is_win = false;

		m_stamp_last_sec_tick = 0;
		m_stamp_last_2sec_tick = 0;
		m_stamp_last_3sec_tick = 0;
		m_stamp_last_10sec_tick = 0;
		m_transfer_all_player_delay_seconds = 0;
	}

	bool map_cs::create_by_template_data(const guid_64& map_guid, int32 template_id, int32 line_id, int32 map_all_time)
	{
		bool init_res = init_map_data(map_guid, template_id, line_id, map_all_time);
		if (false == init_res)
		{
			return false;
		}
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return false;
		}
		int32 npc_spawn_id = map_template_ptr->NpcPointId;
		int32 next_spawn_id = 0;
		if (map_template_ptr->NpcPointId > 0)
		{
			while (true)
			{
				NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_id);
				if (npc_spawn_point_template_ptr == nullptr)
				{
					break;
				}
				next_spawn_id = npc_spawn_point_template_ptr->NextSpawnNpcId;
				if (npc_spawn_point_template_ptr->MapId == template_id)
				{
					if (npc_spawn_point_template_ptr->SpawnRate >= 0)
					{
						NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
						if (npc_template_ptr == nullptr)
						{
							if (next_spawn_id <= npc_spawn_id)
							{
								break;
							}
							npc_spawn_id = next_spawn_id;
							continue;
						}
						if (npc_template_ptr->NpcType == e_unit_type_monster
							&& init_unit::is_world_boss(npc_template_ptr->SubType))
						{
							if (next_spawn_id <= npc_spawn_id)
							{
								break;
							}
							npc_spawn_id = next_spawn_id;
							//世界Boss啥的归WS上的某个管理器处理
							continue;
						}
						npc* new_monster_ptr = nullptr;
						if (npc_spawn_point_template_ptr->IsCreateByMapCreate > 0
							&& is_in_refresh_time(npc_spawn_point_template_ptr))
						{
							new_monster_ptr = spawn_npc(npc_spawn_id);
						}

						if (nullptr == new_monster_ptr)
						{
							int64 new_spawn_stamp = get_next_spawn_time(npc_spawn_point_template_ptr);
							if (new_spawn_stamp >= 0)
							{
								target_time_spawn_info temp_info;
								temp_info.spawn_template_id = npc_spawn_id;
								temp_info.spawn_template_ptr = npc_spawn_point_template_ptr;
								temp_info.target_spawn_time = new_spawn_stamp;
								m_target_time_spawn_vec.push_back(temp_info);
							}
						}
					}
				}
				if (next_spawn_id <= npc_spawn_id)
				{
					break;
				}
				npc_spawn_id = next_spawn_id;
			}
		}
		if (map_template_ptr->Type == e_map_type_cross_server_harry || map_template_ptr->Type == e_map_type_big_map)
		{
			cs2ws_send_create_harry_player msg;
			msg.need_create = true;
			msg.map_type = map_template_ptr->Type;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		}
		return true;
	}

	float map_cs::get_time_left(int32 player_idx)
	{
		return map_object::get_activity_map_sec_left(get_map_type(), player_idx);
	}

	//每秒钟触发一次的慢tick,用于比较慢不需要以帧为单位判断的逻辑(比如看一下怪是否都死亡了重新刷一次),
	//少判断,可减小服务器压力.因为采用时间累加,此tick会有少许误差,特别要求时间精度的逻辑不要使用 --Steven.Han
	void map_cs::tick_sec(const int64& new_time)
	{
		spawn_by_target_time_sec(new_time);

		check_invalid_map_operate();

		//update_belief_cloister_player_info(false);
		if (m_transfer_all_player_delay_seconds != 0 && new_time >= m_transfer_all_player_delay_seconds)
		{
			m_transfer_all_player_delay_seconds = 0;
			transfer_all_player();
		}
	}
	
	void map_cs::tick_2sec(const int64& new_time)
	{
		//update_belief_cloister_player_info(true);
	}

	void map_cs::tick_3sec(const int64& new_time)
	{
		if (get_map_type() == e_map_type_cross_server_harry)
		{
			if (cell_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_harry, e_activity_time_get_all) <= 0)
			{
				transfer_all_player();
			}
		}
	}

	//void map_cs::update_belief_cloister_player_info(bool is_update_point)
	//{
	//	if (get_map_type() != e_map_type_belief_cloister)
	//		return;

	//	ActivityCommonConfigTemplate *ActivityCommonConfigTemplatePtr = GET_TEMPLATE(ActivityCommonConfigTemplate,
	//		belief_cloister_attributeid_in_activity_tbl);
	//	if (ActivityCommonConfigTemplatePtr == nullptr)
	//		return;

	//	for (unit_index_map_it it = m_player_index_list.begin(); it != m_player_index_list.end();)
	//	{
	//		unit_index_map_it& temp_it = it++;
	//		player& temp_player = unit_man::get_player(temp_it->first);
	//		if (temp_player.is_valid())
	//		{
	//			int32 leave_left_second = temp_player.get_belief_rune_mgr().get_leave_stage_left_time();
	//			if (is_update_point == true)
	//			{
	//				if (leave_left_second >= 0)
	//				{
	//					if (leave_left_second <= 0)
	//					{
	//						temp_player.transfer_by_template(big_world_map_id, 0, temp_player.get_unit_info(e_role_info_server_id), guid_64(), 0);
	//					}
	//					continue;
	//				}

	//				temp_player.get_belief_rune_mgr().add_stage_point(ActivityCommonConfigTemplatePtr->ParamInt3);
	//				if (temp_player.get_belief_rune_mgr().get_stage_point() >= ActivityCommonConfigTemplatePtr->ParamInt1)
	//				{
	//					temp_player.get_belief_rune_mgr().set_stage_point(ActivityCommonConfigTemplatePtr->ParamInt1);
	//					temp_player.get_belief_rune_mgr().set_leave_stage_left_time(belief_cloister_leave_map_wait_seconds);
	//				}

	//				sync_stage_point_to_player(temp_it->first, temp_player.get_belief_rune_mgr().get_stage_point());
	//			}
	//			else
	//			{
	//				if (leave_left_second <= 0)
	//					continue;

	//				leave_left_second -= 1;
	//				leave_left_second = (leave_left_second >= 0) ? leave_left_second : 0;
	//				temp_player.get_belief_rune_mgr().set_leave_stage_left_time(leave_left_second);
	//			}
	//		}
	//	}
	//}

	void map_cs::check_invalid_map_operate()
	{
		int32 map_left_sec = get_time_left();
		if (map_left_sec >= 0)
		{
			return;
		}
		if (get_player_num() <= 0)
		{
			return;
		}
		transfer_all_player();
	}

	void map_cs::heart_tick(const int64& new_time)
	{
		if (e_map_state_return_data == m_map_state)
		{
			return;
		}

		float deltaseconds = (new_time - m_begin_map_time) / 1000.f;
		m_begin_map_time = new_time;

		if (new_time > m_stamp_last_sec_tick)//跨度超过1秒了,触发慢tick
		{
			m_stamp_last_sec_tick = new_time + second_tick_time;
			tick_sec(new_time);
		}
		if (new_time > m_stamp_last_2sec_tick)
		{
			m_stamp_last_2sec_tick = new_time + second_tick_time * 2;
			tick_2sec(new_time);
		}
		if (new_time > m_stamp_last_3sec_tick)
		{
			m_stamp_last_3sec_tick = new_time + second_tick_time * 3;
			tick_3sec(new_time);
		}
		if (new_time > m_stamp_last_10sec_tick)
		{
			m_stamp_last_10sec_tick = new_time + second_tick_time * 10;
			send_gain_treasure_npc_to_ws();
		}
	}

	void map_cs::on_player_enter_scene(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (get_map_type() == e_map_type_belief_cloister)
		{
			player_ref.get_belief_rune_mgr().set_stage_point(0);
			player_ref.get_belief_rune_mgr().set_leave_stage_left_time(-1);
		}

		if (get_map_type() == e_map_type_cross_server_harry)
		{
			if (!player_ref.is_self_server())
			{
				player_ref.get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_plunder);
			}
			else
			{
				player_ref.get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_cross_sever);
			}
			if (cell_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_harry, e_activity_time_get_all) > 0)
			{
				if (!player_ref.is_self_server())
				{
					cs2ws_send_harry_notice_to_all_player msg;
					msg.string_id = 90203530;
					connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
				}
			}
		}
		player_ref.get_assist_fight_mgr().sync_assist_fight_state();
	}

	void map_cs::on_player_leave_scene(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		// 如果是从大世界传送到其他地图要先保存一下角色在大世界时候的PK模式
		if (get_map_type() == e_map_type_big_map)
		{
			int32 cur_pk_mode = player_ref.get_pk_community_mgr().get_entire_pk_mode();
			if (cur_pk_mode == e_pk_mode_plunder)
			{
				cur_pk_mode = e_pk_mode_cross_sever;
			}
			player_ref.set_logic_data(e_role_logic_info_main_pk_mode, cur_pk_mode);
			
		}

		// 如果是打宝地图或者水晶幻境 离开时还原大世界的pk模式 
		if (get_map_type() == e_map_type_boss_home || get_map_type() == e_map_type_boss_vip_home || get_map_type() == e_map_type_crystak_dreamland || get_map_type() == e_map_type_cross_server_harry)
		{
			int32 main_pk_mode = player_ref.get_logic_data(e_role_logic_info_main_pk_mode);
			player_ref.get_pk_community_mgr().set_entire_pk_mode(main_pk_mode);
		}
	}

	void map_cs::on_npc_delete(const int32& unit_index, bool is_dead)
	{
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return;
		}

		npc& temp_npc = unit_man::get_npc(unit_index);

		NpcTemplate* npc_template_ptr = temp_npc.get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			return;
		}

		int32 spawn_point_id = temp_npc.get_spawn_point_id();
		NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_point_id);
		if (npc_spawn_point_template_ptr)
		{
			int64 new_spawn_stamp = get_next_spawn_time(npc_spawn_point_template_ptr, is_dead);
			if (new_spawn_stamp >= 0)
			{
				target_time_spawn_info temp_info;
				temp_info.spawn_template_id = spawn_point_id;
				temp_info.spawn_template_ptr = npc_spawn_point_template_ptr;
				temp_info.target_spawn_time = new_spawn_stamp;
				m_target_time_spawn_vec.push_back(temp_info);
			}
		}

		if (npc_template_ptr->NpcType == e_unit_type_monster
			&& init_unit::is_world_boss(npc_template_ptr->SubType))
		{
			player& player_ref = unit_man::get_player(temp_npc.get_killer_array_index());

			cs2ws_world_boss_dead world_boss_dead_msg;
			world_boss_dead_msg.world_boss_spawn_template_id = temp_npc.get_spawn_point_id();

			if (player_ref.is_valid())
			{
				memcpy(world_boss_dead_msg.killer_name, player_ref.get_name(), max_name_size);

				if (player_ref.get_write_log())
				{
					int32 must_do_type = -1;
					if (npc_template_ptr->SubType == e_monster_type_world_boss)
					{
						must_do_type = e_daily_must_do_typ_world_boss;
					}
					else if (npc_template_ptr->SubType == e_monster_type_gold_army)
					{
						must_do_type = e_daily_must_do_typ_gold_army;
					}
					server_log::activity_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), must_do_type, 0);
					
				}
			}
			cell_server::getInstance().set_world_boss_guid(guid_64());
			connection_mgr::getInstance().send_to_ws( &world_boss_dead_msg, sizeof(cs2ws_world_boss_dead));
		}

		if (npc_template_ptr->NpcType == e_unit_type_monster)
			player_kill_monster(temp_npc.get_killer_array_index(), init_unit::is_boss(npc_template_ptr->SubType));

	}

	void map_cs::on_player_dead(const int32& unit_index)
	{
		player& temp_player = unit_man::get_player(unit_index);
		if (temp_player.is_valid() == false)
			return;

		if (get_map_type() == e_map_type_belief_cloister)
		{
			ActivityCommonConfigTemplate *ActivityCommonConfigTemplatePtr = GET_TEMPLATE(ActivityCommonConfigTemplate, belief_cloister_attributeid_in_activity_tbl);
			if (ActivityCommonConfigTemplatePtr == nullptr)
				return;

			if (temp_player.get_belief_rune_mgr().is_in_dead() == true)
				return;
			temp_player.get_belief_rune_mgr().set_is_in_dead(true);

			temp_player.get_belief_rune_mgr().add_stage_point(ActivityCommonConfigTemplatePtr->ParamInt6);
			if (temp_player.get_belief_rune_mgr().get_stage_point() > ActivityCommonConfigTemplatePtr->ParamInt1)
				temp_player.get_belief_rune_mgr().set_stage_point(ActivityCommonConfigTemplatePtr->ParamInt1);

			sync_stage_point_to_player(unit_index, temp_player.get_belief_rune_mgr().get_stage_point());
		}
		
	}

	npc* map_cs::spawn_npc(int32 spawn_point_id, int32 dynamic_lv)
	{
		NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_point_id);
		if (nullptr == npc_spawn_point_template_ptr)
		{
			return nullptr;
		}

		int32 life_time_sec = get_npc_left_life_time(spawn_point_id);
		if (0 == life_time_sec)
		{
			// 没有必要创建
			return nullptr;
		}
		
		if (npc_spawn_point_template_ptr->SpawnRate > 0 && npc_spawn_point_template_ptr->SpawnRate < 100)
		{
			int32 gen_rand_num = ((rand() % 100) + 1);
			if (gen_rand_num > npc_spawn_point_template_ptr->SpawnRate)
			{
				return nullptr;
			}
		}

		npc* npc_ptr = map_object::spawn_npc(spawn_point_id, dynamic_lv);
		if (nullptr == npc_ptr)
		{
			return nullptr;
		}
		npc& npc_ref = *npc_ptr;
		
		if (life_time_sec > 0)
		{
			npc_ref.set_live_time(init_unit::get_end_time(life_time_sec));
		}
		send_gain_treasure_record_to_ws_when_npc_spawn(npc_ref);
		return &npc_ref;
	}

	int32 map_cs::get_npc_left_life_time(int32 spawn_template_id)
	{
		int32 begin_time_sec, end_time_sec;
		get_valid_period_spawn_time(spawn_template_id, begin_time_sec, end_time_sec);
		if (-1 == begin_time_sec || -1 == end_time_sec)
		{
			// 这个怪物不需要定时消失的逻辑
			return -1;
		}
		
		int32 cur_time_sec = time_helper::get_today_time_in_sec();
		if (cur_time_sec < begin_time_sec || cur_time_sec > end_time_sec)
		{
			return 0;
		}
		return end_time_sec - cur_time_sec;
	}

	void map_cs::get_valid_period_spawn_time(int32 spawn_template_id, int32& begin_time_sec, int32& end_time_sec)
	{
		begin_time_sec = -1;
		end_time_sec = -1;
		NpcSpawnPointTemplate* spawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_template_id);
		if (nullptr == spawn_template_ptr)
		{
			return;
		}
		if (spawn_template_ptr->RespawnPeriod.size() <= 0)
		{
			return;
		}
		const std::vector<int32>& respawn_period_arr = spawn_template_ptr->RespawnPeriod;
		int32 len = respawn_period_arr.size();
		if (len <= 0 || len % e_respawn_period_tuple_max != 0)
		{
			return;
		}

		int32 cur_time_sec = time_helper::get_today_time_in_sec();
		for (int32 i = 0; i < len; i += e_respawn_period_tuple_max)
		{
			int32 begin_t = respawn_period_arr[i + e_respawn_period_tuple_begin_time];
			int32 end_t = respawn_period_arr[i + e_respawn_period_tuple_end_time];
			if (begin_t < 0 || end_t <= begin_t)
			{
				//配表错误
				return;
			}
			if (cur_time_sec < end_t)
			{
				begin_time_sec = begin_t;
				end_time_sec = end_t;
				break;
			}
		}
		if (-1 == begin_time_sec)
		{
			// 这种情况下就要第二天种怪了
			begin_time_sec = respawn_period_arr[0] + day_time_second;
			end_time_sec = respawn_period_arr[1] + day_time_second;
		}
	}
	
	int32 map_cs::get_next_target_time_spawn_index(NpcSpawnPointTemplate* spawn_template_ptr)
	{
		int32 return_value = -1;
		if (spawn_template_ptr == nullptr)
		{
			return return_value;
		}
		int32 cur_time_sec = time_helper::get_today_time_in_sec();

		switch (spawn_template_ptr->RespawnType)
		{
		case e_npc_respawn_type_duration_time:
		{
			return_value = -1;
		}
		break;
		case e_npc_respawn_type_in_certain_time:
		{
			if (spawn_template_ptr->RespawnPeriod.size() % e_respawn_period_tuple_max != 0)
			{
				return_value = -1;
			}
			else
			{
				int32 refresh_num = spawn_template_ptr->RespawnPeriod.size() / e_respawn_period_tuple_max;
				for (int32 i = 0; i < refresh_num; ++i)
				{
					if (cur_time_sec + spawn_template_ptr->RespawnTime < spawn_template_ptr->RespawnPeriod[i * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time])
					{
						return_value = i;
						break;
					}
				}
				if (return_value == -1)
				{
					return_value = 0;
				}
			}
		}
		break;
		default:
			break;
		}
		return return_value;
	}

	int64 map_cs::get_next_spawn_time(NpcSpawnPointTemplate* spawn_template_ptr, bool is_be_kill)
	{
		int64 return_value = -1;

		if (spawn_template_ptr == nullptr)
		{
			return return_value;
		}

		int32 cur_time_sec = time_helper::get_today_time_in_sec();

		switch (spawn_template_ptr->RespawnType)
		{
		case e_npc_respawn_type_duration_time:
		{
			if (false == is_be_kill)
			{
				break;
			}
			return_value = init_unit::get_end_time(spawn_template_ptr->RespawnTime);
		}
		break;
		case e_npc_respawn_type_in_certain_time:
		{
			int32 next_time_index = get_next_target_time_spawn_index(spawn_template_ptr);
			if (next_time_index >= 0
				&& next_time_index * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time < spawn_template_ptr->RespawnPeriod.size())
			{
				int32 begin_time = spawn_template_ptr->RespawnPeriod[next_time_index * e_respawn_period_tuple_max + e_respawn_period_tuple_begin_time];
				int32 end_time = spawn_template_ptr->RespawnPeriod[next_time_index * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time];

				if (cur_time_sec > begin_time
					&& cur_time_sec < end_time)
				{
					return_value = init_unit::get_end_time(spawn_template_ptr->RespawnTime);
				}
				else
				{
					if (cur_time_sec < begin_time)
					{
						return_value = init_unit::get_end_time(begin_time - cur_time_sec);
					}
					else
					{
						return_value = init_unit::get_end_time(begin_time - cur_time_sec + day_time_second);
					}
				}
			}
		}
		break;
		default:
		break;
		}
		return return_value;
	}

	bool map_cs::is_in_refresh_time(NpcSpawnPointTemplate* spawn_template_ptr)
	{
		if (spawn_template_ptr == nullptr)
		{
			return false;
		}
		int32 cur_time_sec = time_helper::get_today_time_in_sec();

		switch (spawn_template_ptr->RespawnType)
		{
		case e_npc_respawn_type_duration_time:
		case e_npc_respawn_type_target_time:
			return true;
		break;
		case e_npc_respawn_type_in_certain_time:
		{
			if (spawn_template_ptr->RespawnPeriod.size() % e_respawn_period_tuple_max != 0)
			{
				return false;
			}
			else
			{
				int32 refresh_num = spawn_template_ptr->RespawnPeriod.size() / e_respawn_period_tuple_max;
				for (int32 i = 0; i < refresh_num; ++i)
				{
					int32 begin_time = spawn_template_ptr->RespawnPeriod[i * e_respawn_period_tuple_max + e_respawn_period_tuple_begin_time];
					int32 end_time = spawn_template_ptr->RespawnPeriod[i * e_respawn_period_tuple_max + e_respawn_period_tuple_end_time];
					if (cur_time_sec >= begin_time
						&& cur_time_sec < end_time)
					{
						return true;
					}
				}
			}
		}
		break;
		default:
			break;
		}
		return false;
	}

	void map_cs::spawn_by_target_time_sec(int64 new_time)
	{
		for (target_time_spawn_vec::iterator iter = m_target_time_spawn_vec.begin(); iter != m_target_time_spawn_vec.end();)
		{
			if (iter->spawn_template_ptr == nullptr
				|| iter->target_spawn_time < 0)
			{
				iter = m_target_time_spawn_vec.erase(iter);
				continue;
			}
			if (new_time < iter->target_spawn_time)
			{
				++iter;
				continue;
			}

			if (spawn_npc(iter->spawn_template_id))
			{
				iter = m_target_time_spawn_vec.erase(iter);
				continue;
			}
			else
			{
				int64 next_spawn_stamp = get_next_spawn_time(iter->spawn_template_ptr, true);
				iter->target_spawn_time = next_spawn_stamp;
			}
			++iter;
		}
	}
	void map_cs::send_gain_treasure_npc_to_ws()
	{
		if (nullptr == get_map_template_ptr())
		{
			return;
		}
		if (false == init_unit::is_map_gain_treasure(get_map_type()) && false == init_unit::is_map_boss_island(get_map_type()))
		{
			return;
		}
		if (get_npc_num()<=0)
		{
			return;
		}
		for (unit_index_map_it it = m_npc_index_list.begin(); it != m_npc_index_list.end(); )
		{
			auto temp_it = it++;
			npc& temp_npc = unit_man::get_npc(temp_it->first);
			if (false == temp_npc.is_valid())
			{
				continue;
			}
			temp_npc.send_gain_treasure_boss_owner_to_ws();
		}
	}

	void map_cs::add_kill_count_with_boss_island(npc& temp_npc, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr, std::set<int32> player_index_arr)
	{
		return;
		if (get_map_type() != e_map_type_boss_island)
		{
			return;
		}
		if (temp_npc.get_unit_type() != e_unit_type_monster || init_unit::is_boss(temp_npc.get_unit_sub_type()) != true)
		{
			return;
		}
		std::set<int32>::iterator it;
		for (it = player_index_arr.begin(); it != player_index_arr.end(); it++)
		{
			player& player_ref = unit_man::get_player(*it);
			if (false == player_ref.is_valid())
			{
				continue;
			}
			auto cur_player_map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
			if (nullptr == cur_player_map_object_ptr)
			{
				continue;
			}
			if (false == init_unit::is_map_boss_island(cur_player_map_object_ptr->get_map_type()))
			{
				continue;
			}
			int32 max_count = GAMECONFIG->BossIslandMaxKillNum;
			int32 cur_kill_num = player_ref.get_boss_island_mgr().get_boss_island_info(e_island_record_killed_island_boss);
			cur_kill_num++;
			if (cur_kill_num > max_count)
			{
				cur_kill_num = max_count;
			}
			player_ref.get_boss_island_mgr().set_boss_island_info(e_island_record_killed_island_boss, cur_kill_num);		
			player_ref.get_boss_island_mgr().send_player_boss_island_info_one(e_island_record_killed_island_boss);
			player_ref.set_must_do_count(e_daily_must_do_typ_boss_island, player_ref.get_must_do_count(e_daily_must_do_typ_boss_island) + 1);
			player_ref.send_daily_must_do_count_info_one(e_daily_must_do_typ_boss_island);
			player_ref.set_daily_active_degree_info(e_daily_must_do_typ_boss_island);
		}
	}

	void map_cs::send_gain_treasure_record_to_ws_when_npc_dead(npc& temp_npc, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr,std::set<int32> player_index_arr)
	{
		if (false == init_unit::is_map_gain_treasure(get_map_type()))
		{
			return;
		}
		if (temp_npc.get_unit_type() != e_unit_type_monster || init_unit::is_boss(temp_npc.get_unit_sub_type()) != true)
		{
			return;
		}
		temp_npc.send_gain_treasure_record_to_ws(time_helper::get_cur_time_new().second, player_info_arr);
	}

	void map_cs::send_gain_treasure_record_to_ws_when_npc_spawn(npc& temp_npc)
	{
		if (false == init_unit::is_map_gain_treasure(get_map_type()) && false == init_unit::is_map_boss_island(get_map_type()))
		{
			return;
		}
		if ((temp_npc.get_unit_type() != e_unit_type_monster || temp_npc.get_unit_sub_type() != e_monster_type_boss)
			&&(temp_npc.get_unit_type() != e_unit_type_npc || (temp_npc.get_unit_sub_type() != e_npc_type_island_small_chests && temp_npc.get_unit_sub_type() != e_npc_type_island_big_chests)))
		{
			return;
		}
		temp_npc.send_gain_treasure_boss_info_when_spawn();
	}

	static bool exists_team_guid(const std::vector<guid_64>& ret_team_guid_arr, guid_64 checking_team_guid)
	{
		for (const guid_64& team_guid_in_ret_arr : ret_team_guid_arr)
		{
			if (checking_team_guid == team_guid_in_ret_arr)
			{
				return true;
			}
		}

		return false;
	}

	void map_cs::player_kill_monster(const int32& unit_index, bool isBoss)
	{
		player& temp_player = unit_man::get_player(unit_index);
		if (temp_player.is_valid() == false)
			return;

		if (get_map_type() == e_map_type_belief_cloister)
		{
			ActivityCommonConfigTemplate *ActivityCommonConfigTemplatePtr = GET_TEMPLATE(ActivityCommonConfigTemplate, belief_cloister_attributeid_in_activity_tbl);
			if (ActivityCommonConfigTemplatePtr == nullptr)
				return;

			if (isBoss == true)
				temp_player.get_belief_rune_mgr().add_stage_point(ActivityCommonConfigTemplatePtr->ParamInt5);
			else
				temp_player.get_belief_rune_mgr().add_stage_point(ActivityCommonConfigTemplatePtr->ParamInt4);

			if (temp_player.get_belief_rune_mgr().get_stage_point() > ActivityCommonConfigTemplatePtr->ParamInt1)
				temp_player.get_belief_rune_mgr().set_stage_point(ActivityCommonConfigTemplatePtr->ParamInt1);

			sync_stage_point_to_player(unit_index, temp_player.get_belief_rune_mgr().get_stage_point());
		}
		else if (get_map_type() == e_map_type_boss_home)
		{
			if (isBoss == true)
			{
				if (temp_player.is_player_in_team() == true)
				{
					for (int32 i = 0; i < max_team_member_num; i++)
					{
						player& team_player = unit_man::get_player(temp_player.get_team_cs_mgr().get_unit_team_info().team_mems_guid[i]);
						if (temp_player.is_valid() == false)
							continue;
					}
				}
			}
		}
		
		if (temp_player.is_player_in_team() == true)
		{
			for (int32 i = 0; i < max_team_member_num; i++)
			{
				player& team_player = unit_man::get_player(temp_player.get_team_cs_mgr().get_unit_team_info().team_mems_guid[i]);
				if (temp_player.is_valid() == false)
					continue;
				team_player.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_kill_monster);
			}
		}
		else
		{
			temp_player.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_kill_monster);
		}

	}

	void map_cs::sync_stage_point_to_player(const int32& unit_index, const int32& stage_point_value)
	{
		if (get_map_type() != e_map_type_belief_cloister)
			return;

		player& temp_player = unit_man::get_player(unit_index);
		if (temp_player.is_valid() == false)
			return;

		hld::belief_rune_proto_belief_cloister_point_update msg;
		msg.set_point_value(stage_point_value);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_belief_cloister_point_update);
	}

	void map_cs::create_harry_map_normal_npc(int32 num)
	{
		MapTemplate* map_template_ptr = get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return;
		}
		int32 template_id = map_template_ptr->attribute_id;
		for (int32 i = 0; i < 10; i++)
		{
			int32 id = harry_map_first_spawn_id + i + (3 - num);
			NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, id);
			if (npc_spawn_point_template_ptr == nullptr)
			{
				break;
			}
			if (npc_spawn_point_template_ptr->MapId != template_id)
			{
				break;
			}
			if (npc_spawn_point_template_ptr->SpawnRate < 0)
			{
				continue;
			}
			NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
			if (npc_template_ptr == nullptr)
			{
				continue;
			}
			if (npc_template_ptr->NpcType == e_unit_type_monster
				&& init_unit::is_world_boss(npc_template_ptr->SubType))
			{
				//世界Boss啥的归WS上的某个管理器处理
				continue;
			}

			npc* new_monster_ptr = nullptr;
			if (npc_spawn_point_template_ptr->IsCreateByMapCreate > 0
				&& is_in_refresh_time(npc_spawn_point_template_ptr))
			{
				new_monster_ptr = spawn_npc(id);
			}

			if (nullptr == new_monster_ptr)
			{
				int64 new_spawn_stamp = get_next_spawn_time(npc_spawn_point_template_ptr);
				if (new_spawn_stamp >= 0)
				{
					target_time_spawn_info temp_info;
					temp_info.spawn_template_id = id;
					temp_info.spawn_template_ptr = npc_spawn_point_template_ptr;
					temp_info.target_spawn_time = new_spawn_stamp;
					m_target_time_spawn_vec.push_back(temp_info);
				}
			}
		}
	}

	void map_cs::set_transfer_all_player_delay_seconds(int64 delay_time)
	{
		m_transfer_all_player_delay_seconds = time_helper::get_cur_time_new().millisecond + second_tick_time * delay_time;
	}
	void map_cs::record_server_crystal_log(int32 unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		guid_64 play_guid = player_ref.get_unit_guid();
		for (int32 i = 0; i < m_enter_map_array.size(); ++i)
		{
			//如果存在不记录
			if (m_enter_map_array[i] == play_guid)
			{
				return;
			}
		}
		int32 is_self_server = 0;
		if (false == player_ref.is_self_server())
		{
			is_self_server = 1;
		}
		set_log_var(log_head)
			player_ref.get_log_common_head_info(log_head);
		server_log::serverCrystalLog(log_head, is_self_server);
		m_enter_map_array.push_back(play_guid);
	}
}
