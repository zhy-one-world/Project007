#include "world_boss_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "utility/globle_data.h"
#include "utility/init_unit.h"
#include "Logic/activity_def.hpp"
#include "Logic/npc_def.hpp"
#include "../mail/event_ws_mgr.h"
#include "../ranking/ranking_mgr_ws.h"
#include "world_server.hpp"
#include "utility/random.h"
#include "template/template_manager.h"
#include "system/scene/cs_map_system.h"
#include "system/scene/cs_map_mgr_system.h"
#include "server/legion/legion_ws_mgr.h"
#include "world_boss_msg.hpp"
#include "character.pb.h"
#include "../mail/mail_event_ws.h"
#include "game.pb.h"
#include "net.pb.h"

namespace hld
{
	world_boss_ws_mgr::world_boss_ws_mgr()
	{
		clear_data();
	}

	world_boss_ws_mgr::~world_boss_ws_mgr()
	{

	}

	void world_boss_ws_mgr::clear_data()
	{
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			m_world_boss_data[i].clear_data();
			m_world_boss_respawn_ptr_list[i] = nullptr;
		}
		m_last_save_time = 0;
		m_timer = 0;
		last_tick_time = 0;
		one_sec_time = 0;
		m_is_all_boss_dead = true;
		m_cur_score_npc_id = 0;
		m_cur_boss_type = 0;
		m_create_boss_tamp = 0;
		m_ready_boss_tamp = 0;
		m_is_create_boss_ready_npc = 0;
		m_3sec_tick = 0;
		m_remove_boss_tamp = 0;
		m_is_send_legion_welfare = false;
		m_activity_rank_indicator.clear_data();
		m_is_need_sort = false;
		memset(lower_boss_arr, 0, sizeof(lower_boss_arr));
		memset(intermediate_boss_arr, 0, sizeof(intermediate_boss_arr));
		memset(senior_boss_arr, 0, sizeof(senior_boss_arr));
		is_in_active_time = false;
		is_send_first_stage_notice = false;
		is_send_second_stage_notice = false;
		is_send_third_stage_notice = false;
		for (int32 i = 0; i < max_legion_member_num; i++)
		{
			legion_member_arr[i].clear_data();
		}
	}

	void world_boss_ws_mgr::clear_world_boss_activity_info()
	{
		m_personal_world_boss_score_info.clear();		//清空世界boss个人伤害列表
		m_legion_world_boss_score_info.clear();			//清空世界boss军团伤害列表
		m_all_join_world_boss_member.clear();			//清空所有参加世界boss成员
		m_boss_create_pos.clear();						//清空世界boss随机位置
		m_personal_total_rank_indicator.clear_data();	//清空个人总伤害
		m_activity_rank_indicator.clear_data();			//清空军团总伤害
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			m_world_boss_data[i].set_boss_state(0);
		}
	}

	void world_boss_ws_mgr::tick(int32 dt)
	{
		//此处Tick的间隔值是毫秒单位
		//1秒钟（1000毫秒）tick一次
		m_3sec_tick += dt;
		if (m_3sec_tick > 3000)
		{
			m_3sec_tick = 0;
			tick_3sec();
		}
		have_world_boss_time();
		m_timer += dt;
		if (m_timer - last_tick_time > 1000)
		{
			time_info cur_time_info = time_helper::get_cur_time_new();
			last_tick_time = m_timer;

			if (cur_time_info.second >= m_create_boss_tamp && cur_time_info.second <= m_remove_boss_tamp)
			{
				//在活动时间内
				if (!is_in_active_time)
				{
					is_in_active_time = true;
					is_send_first_stage_notice = true;
					is_send_second_stage_notice = true;
					is_send_third_stage_notice = true;

					clear_world_boss_activity_info();

					find_need_create_world_boss();					//设置需要创建的低中高世界boss数组
					assign_postion_to_world_boss();					//设置世界boss位置
					assign_world_boss_to_legion();					//分配世界boss给每个军团
					legion_ws_mgr::get_instance().send_all_legion_world_boss_info();   //发送所有攻略boss给对应军团
				}
				send_world_boss_notice();		
			}
			else
			{
				//不在活动时间内(活动结束)
				if (is_in_active_time)
				{
					is_in_active_time = false;
					is_send_first_stage_notice = false;
					is_send_second_stage_notice = false;
					is_send_third_stage_notice = false;
					set_is_all_boss_dead(true);
				}
			}

			if (world_server::getInstance().get_activity_sec_left(e_activity_type_world_boss, e_activity_time_get_gaming) >= 0 && m_is_send_legion_welfare == false)
			{
				m_is_send_legion_welfare = true;
			}
			if (world_server::getInstance().get_activity_sec_left(e_activity_type_world_boss, e_activity_time_get_all) < 0 && m_is_send_legion_welfare == true)
			{
				//活动结束后发放军团福利奖励 不需要等到boss死完
				send_strategy_reward();
				send_leigon_welfare_activity_end(e_activity_type_world_boss);
				send_strategy_result();

				m_is_send_legion_welfare = false;

				clear_world_boss_activity_info();
			}

			//循环世界boss容器，看活着的Boss是否能刷新，能刷新就remove掉，并加入可刷新列表
			for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
			{
				if (m_world_boss_data[i].get_spawn_id() <= 0)
				{
					continue;
				}

				if (cur_time_info.second >= m_create_boss_tamp && cur_time_info.second <= m_remove_boss_tamp && !m_world_boss_data[i].get_creating() && can_create_world_boss(m_world_boss_data[i].get_spawn_id()))
				{
					//创建成功再设置为true
					if (create_world_boss(m_world_boss_data[i].get_spawn_id()))
					{
						set_is_all_boss_dead(false);
						m_world_boss_data[i].set_creating(true);
						legion_ws_mgr::get_instance().clear_boss_rank_list();//清理BOSS排行榜
					}
				}

				NpcSpawnPointTemplate* temp_respawn_template_ptr = m_world_boss_data[i].get_respawn_template_ptr();
				if (temp_respawn_template_ptr == nullptr)
				{
					continue;
				}

				if (can_remove_world_boss(m_world_boss_data[i].get_spawn_id()))
				{
					m_world_boss_data[i].set_creating(false);
					if (0 == m_world_boss_data[i].is_alive())
					{
						continue;
					}
					int32 old_line_info = m_world_boss_data[i].get_line_id();
					auto map_ent = cs_map_mgr_system::get_map_ws_by_line(temp_respawn_template_ptr->MapId, old_line_info);
					if (map_ent != nullptr)
					{
						ws2cs_remove_world_boss remove_msg;
						remove_msg.world_boss_npc_array_index = m_world_boss_data[i].get_npc_array_index();
						remove_msg.world_boss_guid = m_world_boss_data[i].get_boss_guid();
						bool is_use = proto_by_lua(e_msgindex_ws2cs_remove_world_boss);
						if (is_use == false)
						{
							cs_map_system::send_message_to_cs(map_ent, &remove_msg, sizeof(ws2cs_remove_world_boss));
						}
						else
						{
							hld::ws2cs_proto::remove_world_boss pro_msg;
							remove_msg.to_proto(pro_msg);
							cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msgindex_ws2cs_remove_world_boss);
						}
						m_world_boss_data[i].set_alive(0);
					}
				}
			}
		}
	}

	void world_boss_ws_mgr::tick_3sec()
	{
		add_boss_already_dead_score();
	}

	int32 world_boss_ws_mgr::get_legion_rank_with_legion_boss(guid_64 legion_guid)
	{
		if (m_cur_boss_type != e_boss_type_legion)
		{
			return -1;
		}
		m_legion_rank_indicator.sort();
		int32 rank_idex = m_legion_rank_indicator.get_score_record_idx(legion_guid);
		return rank_idex;
	}

	score_indicator* world_boss_ws_mgr::get_legion_score_all()
	{
		if (m_cur_boss_type != e_boss_type_legion)
		{
			return nullptr;
		}
		m_legion_rank_indicator.sort();
		return &m_legion_rank_indicator;
	}

	int64 world_boss_ws_mgr::get_legion_damage_with_legion_boss(guid_64 legion_guid)
	{
		if (m_cur_boss_type != e_boss_type_legion)
		{
			return -1;
		}
		m_legion_rank_indicator.sort();
		score_record* cur_score = m_legion_rank_indicator.get_score_record(legion_guid);
		if (nullptr == cur_score)
		{
			return -1;
		}
		int64 damage_value = cur_score->score;
		return damage_value;
	}

	void world_boss_ws_mgr::send_damage_list(client_session* session, int32 boss_id)
	{
		ZoneScoped;
		if (session == nullptr)
		{
			CONSOLE_ERROR("session is null");
			return;
		}
		if (m_cur_boss_type == e_boss_type_world)
		{
			if (m_is_all_boss_dead)
			{
				CONSOLE_ERROR("m_is_all_boss_dead:{} session is null", m_is_all_boss_dead);
				return;
			}
			int64 cur_time = time_helper::get_cur_time_new().second;
			int64 last_time = session->m_last_send_world_boss_damage_list_sec;
			if (cur_time < last_time + 4)
			{
				CONSOLE_ERROR("send_damage_list time is not enough cur_time:{} last_time:{}", cur_time, last_time + 4);
				return;
			}
			MapTemplate* map_template_ptr = cs_map_system::get_map_template(session->get_map_guid());
			if (map_template_ptr == nullptr)
			{
				CONSOLE_ERROR("map_template_ptr is null map_guid:{}", session->get_map_guid().server_64);
				return;
			}
			if (map_template_ptr->Type != hld::e_map_type_big_map)
			{
				CONSOLE_ERROR("map_template_ptr->Type is not e_map_type_big_map Type:{}", map_template_ptr->Type);
				return;
			}

			if (boss_id == 0)
			{
				CONSOLE_ERROR("boss_id is 0");
				return;
			}

			session->m_last_send_world_boss_damage_list_sec = cur_time;

			if (m_is_need_sort)
			{
				m_activity_rank_indicator.sort();
				m_personal_total_rank_indicator.sort();
				m_is_need_sort = false;
			}
			game_proto_score_record_list score_record_list;
			game_proto_score_record_list legion_score_record_list;
			int32 to_client_top_num = 3;
			score_record_list.set_npc_id(boss_id);
			m_personal_total_rank_indicator.fill_score_record_list_msg(score_record_list, to_client_top_num);
			score_record_list.clear_extra_score();
			score_record_list.clear_extra_rank();
			if (nullptr == m_personal_total_rank_indicator.get_score_record(session->get_role_guid()))
			{
				session->send_to_client(&score_record_list, e_msgindex_s2c_score_record_list);
			}
			else
			{
				m_personal_total_rank_indicator.fill_extra_score_record_msg(score_record_list, session->get_role_guid());
				session->send_to_client(&score_record_list, e_msgindex_s2c_score_record_list);
			}
			if (!session->get_legion_guid().is_valid())
			{
				return;
			}
			guid_64 legion_guid = session->get_legion_guid();
			legion_score_record_list.set_npc_id(boss_id);
			m_activity_rank_indicator.fill_score_record_list_msg(legion_score_record_list, to_client_top_num);
			legion_score_record_list.set_is_legion_score(true);
			legion_score_record_list.clear_extra_score();
			legion_score_record_list.clear_extra_rank();
			if (nullptr == m_activity_rank_indicator.get_score_record(legion_guid))
			{
				session->send_to_client(&legion_score_record_list, e_msgindex_s2c_score_record_list);
			}
			else
			{
				m_activity_rank_indicator.fill_extra_score_record_msg(legion_score_record_list, legion_guid);
				session->send_to_client(&legion_score_record_list, e_msgindex_s2c_score_record_list);
			}
		}
		else if (m_cur_boss_type == e_boss_type_legion)
		{
			int64 cur_time = time_helper::get_cur_time_new().second;
			int64 last_time = session->m_last_send_world_boss_damage_list_sec;
			if (cur_time < last_time + 4)
			{
				CONSOLE_ERROR("send_damage_list time is not enough cur_time:{} last_time:{}", cur_time, last_time + 4);
				return;
			}
			auto map_template_ptr = cs_map_system::get_map_template(session->get_map_guid());
			if (map_template_ptr == nullptr)
			{
				CONSOLE_ERROR("map_template_ptr is null map_guid:{}", session->get_map_guid().server_64);
				return;
			}
			if (map_template_ptr->Type != hld::e_map_type_legion_station)
			{
				CONSOLE_ERROR("map_template_ptr->Type is not e_map_type_legion_station Type:{}", map_template_ptr->Type);
				return;
			}
			session->m_last_send_world_boss_damage_list_sec = cur_time;
			int32 to_client_top_num = 3;
			if (m_is_need_sort)
			{
				m_legion_rank_indicator.sort();
				m_is_need_sort = false;
			}
			game_proto_score_record_list legion_score_record_list;
			if (false == session->get_legion_guid().is_valid())
			{
				return;
			}
			guid_64 legion_guid = session->get_legion_guid();
			legion_score_record_list.set_npc_id(m_cur_score_npc_id);
			m_legion_rank_indicator.fill_score_record_list_msg(legion_score_record_list, to_client_top_num);
			legion_score_record_list.set_is_legion_score(true);
			legion_score_record_list.clear_extra_score();
			legion_score_record_list.clear_extra_rank();
			if (nullptr == m_legion_rank_indicator.get_score_record(legion_guid))
			{
				session->send_to_client(&legion_score_record_list, e_msgindex_s2c_score_record_list);
			}
			else
			{
				m_legion_rank_indicator.fill_extra_score_record_msg(legion_score_record_list, legion_guid);
				session->send_to_client(&legion_score_record_list, e_msgindex_s2c_score_record_list);
			}
		}
	}

	void world_boss_ws_mgr::create_world_boss_success(guid_64 cur_boss_guid, int32 world_boss_array_index, int32 npc_spawn_template_id, int32 npc_spawn_line_id)
	{
		NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_template_id);
		if (nullptr == spawn_point_template_ptr)
		{
			return;
		}
		//if (npc_spawn_template_id <= end_boss_ready_npc_id&&npc_spawn_template_id >= first_boss_ready_npc_id)
		//{
		//	s_world_boss_ready_info world_boss_ready_npc;//这里将BOSS准备的裂隙存起来
		//	world_boss_ready_npc.line_id = npc_spawn_line_id;
		//	world_boss_ready_npc.npc_guid = cur_boss_guid;
		//	world_boss_ready_npc.npc_respawn_point_template_id = npc_spawn_template_id;
		//	world_boss_ready_npc.arr_index = world_boss_array_index;
		//	m_create_ready_boss_npc_arr.insert(m_create_ready_boss_npc_arr.end(), world_boss_ready_npc);
		//	return;
		//}

		world_boss_ws* temp_info = get_world_boss_info(npc_spawn_template_id);
		if (temp_info == nullptr)
		{
			return;
		}
		temp_info->set_alive(1);
		temp_info->set_npc_array_index(world_boss_array_index);
		temp_info->set_line_id(npc_spawn_line_id);
		temp_info->set_creating(false);
		temp_info->set_boss_guid(cur_boss_guid);
		if (m_boss_create_pos.count(npc_spawn_template_id) == 1)
		{
			temp_info->set_boss_random_pos_id(m_boss_create_pos[npc_spawn_template_id]);
		}
		if (can_create_world_boss(npc_spawn_template_id))
		{
			temp_info->set_creating(true);
		}

		send_world_boss_one(*temp_info, true);
		return;
	}

	void world_boss_ws_mgr::operate_respawn_list(int32 npc_spawn_template_id, bool is_add)
	{
		if (is_add)
		{
			world_boss_ws* temp_info = get_world_boss_info(npc_spawn_template_id);
			if (temp_info == nullptr)
			{
				return;
			}

			for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
			{
				if (m_world_boss_respawn_ptr_list[i] == nullptr)
				{
					continue;
				}
				if (m_world_boss_respawn_ptr_list[i]->get_spawn_id() == npc_spawn_template_id)
				{
					//已经在刷新列表中的，不能反复添加
					return;
				}
			}

			int32 empty_index = -1;
			for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
			{
				if (m_world_boss_respawn_ptr_list[i] == nullptr)
				{
					empty_index = i;
					break;
				}
			}
			if (empty_index < 0)
			{
				return;
			}
			m_world_boss_respawn_ptr_list[empty_index] = temp_info;
		}
		else
		{
			for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
			{
				if (m_world_boss_respawn_ptr_list[i]->get_spawn_id() == npc_spawn_template_id)
				{
					m_world_boss_respawn_ptr_list[i] = nullptr;
					break;
				}
			}
		}
	}


	void world_boss_ws_mgr::req_frist_kill_world_boss_prize(const guid_64& req_guid, int32 spawn_npc_template_id)
	{
		if (req_guid.is_valid() == false)
		{
			return;
		}
		world_boss_ws* world_boss_info = world_boss_ws_mgr::get_instance().get_world_boss_info(spawn_npc_template_id);
		if (nullptr == world_boss_info)
		{
			return;
		}

		if (world_boss_info->m_is_recv_get_prize_msg)
		{
			return;
		}
		world_boss_info->m_is_recv_get_prize_msg = true;
		int32 first_kill_welfare_type = 14;
		WelfareTemplate* first_kill_welfare_template_ptr = template_manager::get_instance().get_welfare_template_ptr_by_type_and_target(first_kill_welfare_type, spawn_npc_template_id);
		if (nullptr == first_kill_welfare_template_ptr)
		{
			world_boss_info->m_is_recv_get_prize_msg = false;
			return;
		}
		//已经领取过了
		if (e_first_kill_prize_type_was_get == world_boss_info->get_is_get_prize())
		{
			world_boss_info->m_is_recv_get_prize_msg = false;
			return;
		}
		client_session* frist_killer = client_session_mgr::getInstance().get_session_by_role_name(world_boss_info->get_first_killer_name());
		if (nullptr == frist_killer)
		{
			world_boss_info->m_is_recv_get_prize_msg = false;
			return;
		}
		//判断 是否首杀者
		if (req_guid == frist_killer->get_role_guid())
		{
			ws2cs_create_first_kill_prize pak;
			pak.first_killer_guid = req_guid;
			pak.first_kill_welfare_template_id = first_kill_welfare_template_ptr->attribute_id;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_create_first_kill_prize);
			if (is_use == false)
			{
				frist_killer->send_to_cs(&pak, sizeof(ws2cs_create_first_kill_prize));
			}
			else
			{
				hld::ws2cs_proto::create_first_kill_prize pro_msg;
				pak.to_proto(pro_msg);
				frist_killer->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_create_first_kill_prize);
			}
		}
		world_boss_info->m_is_recv_get_prize_msg = false;
	}


	void world_boss_ws_mgr::first_killer_get_prize_func(int32 welfare_template_id, int32 is_get_prize, const s_item_info(&reward_array)[max_world_boss_reward_num])
	{
		if (is_get_prize >= e_first_kill_prize_type_max || is_get_prize < e_first_kill_prize_type_cant_get)
		{
			return;
		}
		if (welfare_template_id <= 0)
		{
			return;
		}
		WelfareTemplate* welfare_template_ptr = GET_TEMPLATE(WelfareTemplate, welfare_template_id);
		if (nullptr == welfare_template_ptr)
		{
			return;
		}

		NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, welfare_template_ptr->TargetValue);
		if (nullptr == spawn_point_template_ptr)
		{
			return;
		}
		world_boss_ws* world_boss_ptr = get_world_boss_info(spawn_point_template_ptr->attribute_id);
		if (nullptr == world_boss_ptr)
		{
			return;
		}
		if (true == world_boss_ptr->m_is_recv_get_prize_msg)
		{
			return;
		}
		world_boss_ptr->m_is_recv_get_prize_msg = true;

		world_boss_ptr->set_is_get_prize((e_first_kill_prize_type)is_get_prize);
		world_boss_ws_mgr::get_instance().send_world_boss_one(*world_boss_ptr, true, true);
		//world_boss_ws_mgr::get_instance().save_world_boss_to_sql();

		if (e_first_kill_prize_type_was_get == is_get_prize)
		{
			std::string first_killer_name = world_boss_ptr->get_first_killer_name();
			client_session* first_killer_session = client_session_mgr::getInstance().get_session_by_role_name(first_killer_name);
			if (nullptr == first_killer_session)
			{
				return;
			}
			std::string reward_name = "";
			s_item_info item_data[chat_max_item];
			int32 item_num = 0;
			for (int32 index = 0; index < max_world_boss_reward_num; index++)
			{
				ItemTemplate* item_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(reward_array[index].data_ary[e_item_info_info_id]);
				if (nullptr == item_ptr)
				{
					continue;
				}
				//std::string temp_item_name = template_manager::get_instance().get_str_by_string_template_id(item_ptr->item_name);
				reward_name.append("%d");

				if (item_num >= 0 && item_num < chat_max_item)
				{
					item_data[item_num] = reward_array[index];
					++item_num;
				}
			}
			const std::string& npc_name = template_manager::get_instance().get_str_by_string_template_id(world_boss_ptr->get_npc_template_ptr()->NpcName);
			int32 notice_id = 93000076;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(first_killer_name);
			vec_notice_str.push_back(npc_name);
			vec_notice_str.push_back(reward_name);
			std::string notice_str = init_unit::implode(vec_notice_str);
			int32 create_time = time_helper::get_cur_time_new().second;

			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, first_killer_session->get_role_guid(), notice_str, item_data, item_num);
		}
		world_boss_ptr->m_is_recv_get_prize_msg = false;
	}

	//void world_boss_ws_mgr::remove_world_boss_ready_npc(int32 ready_npc_id)
	//{
	//	if (ready_npc_id >= m_create_ready_boss_npc_arr.size())
	//	{
	//		return;
	//	}
	//	int32 world_boss_ready_spawn_template_id = first_boss_ready_npc_id + ready_npc_id;
	//	NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, world_boss_ready_spawn_template_id);
	//	if (nullptr == spawn_point_template_ptr)
	//	{
	//		return;
	//	}
	//	int32 old_line_info = m_create_ready_boss_npc_arr[ready_npc_id].line_id;
	//	auto old_map_ws = cs_map_mgr_system::get_map_ws_by_line(spawn_point_template_ptr->MapId, old_line_info);
	//	if (old_map_ws != nullptr)
	//	{
	//		int32 old_cs_uid = old_map_ws->get_cs_uid();
	//		ws2cs_remove_world_boss remove_msg;
	//		remove_msg.world_boss_npc_array_index = m_create_ready_boss_npc_arr[ready_npc_id].arr_index;
	//		remove_msg.world_boss_guid = m_create_ready_boss_npc_arr[ready_npc_id].npc_guid;
	//		world_server::getInstance().send_by_uid(old_cs_uid, &remove_msg, sizeof(ws2cs_remove_world_boss));
	//	}
	//}


//	void world_boss_ws_mgr::create_world_boss_ready_npc()
//	{
//		if (true == m_is_create_boss_ready_npc)
//		{
//			return;
//		}
//		time_info cur_time_info = time_helper::get_cur_time_new();
//
//		for (int32 i = first_boss_ready_npc_id; i < end_boss_ready_npc_id + 1; i++)
//		{
//			ws2cs_create_world_boss create_msg;
//			create_msg.world_boss_spawn_template_id = i;
//			NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, i);
//			if (spawn_point_template_ptr == nullptr)
//			{
//				return;
//			}
//			auto temp_map_ws = cs_map_mgr_system::get_map_ws_by_line(spawn_point_template_ptr->MapId, 1);
//			if (temp_map_ws == nullptr)
//			{
//				return;
//			}
//			int32 cs_uid = temp_map_ws->get_cs_uid();
//			if (cur_time_info.second >= m_ready_boss_tamp && cur_time_info.second <= m_create_boss_tamp)
//			{
//				world_server::getInstance().send_by_uid(cs_uid, &create_msg, sizeof(ws2cs_create_world_boss));
//			}
//		}
//		m_is_create_boss_ready_npc = true;
//	}
//
	bool world_boss_ws_mgr::create_world_boss(int32 SpawnId)
	{
		ZoneScoped;

		time_info cur_time_info = time_helper::get_cur_time_new();

		ws2cs_create_world_boss create_msg;
		create_msg.world_boss_spawn_template_id = SpawnId;
		if (m_boss_create_pos.count(SpawnId) == 1)
		{
			create_msg.world_boss_random_pos_arr_id = m_boss_create_pos[SpawnId];
		}
		else
		{
			CONSOLE_ERROR("m_boss_create_pos.count(SpawnId) != 1 SpawnId:{}", SpawnId);
			return false;
		}
		NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, SpawnId);
		if (spawn_point_template_ptr == nullptr)
		{
			CONSOLE_ERROR("spawn_point_template_ptr is nullptr spawn_id:{}", SpawnId);
			return false;
		}
		auto map_ent = cs_map_mgr_system::get_map_ws_by_line(spawn_point_template_ptr->MapId, 1);
		if (map_ent == nullptr)
		{
			CONSOLE_ERROR("map_ent is nullptr map_id:{}", spawn_point_template_ptr->MapId);
			return false;
		}
		//if (spawn_point_template_ptr->NpcType == 10)
		//{
		//	remove_world_boss_ready_npc(SpawnId - first_boss_spawn_id);
		//}
		if (cur_time_info.second >= m_create_boss_tamp && cur_time_info.second <= m_remove_boss_tamp)
		{
			bool is_use = proto_by_lua(e_msgindex_ws2cs_create_world_boss);
			if (is_use == false)
			{
				cs_map_system::send_message_to_cs(map_ent, &create_msg, sizeof(ws2cs_create_world_boss));
			}
			else
			{
				hld::ws2cs_proto::create_world_boss pro_msg;
				create_msg.to_proto(pro_msg);
				cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msgindex_ws2cs_create_world_boss);
			}
		}
		return true;
	}

	void world_boss_ws_mgr::have_world_boss_time()
	{
		ActivityCommonConfigTemplate*act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return;
		}
		bool is_in_day = false;
		time_info cur_time_info = time_helper::get_cur_time_new();
		std::vector<int32> open_data = world_server::getInstance().get_activity_open_data(e_activity_type_world_boss);
		for (int32 i = 0; i < open_data.size(); ++i)
		{
			if (cur_time_info.day_in_week == open_data[i])
			{
				is_in_day = true;
			}
		}

		if (!is_in_day)
		{
			return;
		}

		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(e_activity_type_world_boss);
		if (open_time.size() % once_activity_open_time_array_size != 0)
		{
			return;
		}
		const std::vector<int32>& act_time_one = open_time;
		for (int32 i = 0; i < act_time_one.size() / once_activity_open_time_array_size; i++)
		{
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss) && i >= 1)
			{
				break;
			}

			m_create_boss_tamp = time_helper::get_stamp_by_hour_min(act_time_one[i * once_activity_open_time_array_size + 2], act_time_one[i * once_activity_open_time_array_size + 3]);
			m_remove_boss_tamp = time_helper::get_stamp_by_hour_min(act_time_one[i * once_activity_open_time_array_size + 6], act_time_one[i * once_activity_open_time_array_size + 7]);

			if (time_helper::get_cur_time_new().second >= time_helper::get_stamp_by_hour_min(act_time_one[i * once_activity_open_time_array_size + 6], act_time_one[i * once_activity_open_time_array_size + 7]))
			{
				continue;
			}
			else
			{
				return;
			}
		}
	}

	void world_boss_ws_mgr::init_world_boss_data(int32 respawn_point_template_id)
	{
		world_boss_ws* temp_info = get_empty_boss_info();
		if (temp_info == nullptr)
		{
			return;
		}
		temp_info->init_by_spawn_id(respawn_point_template_id);
	}

	void world_boss_ws_mgr::init_manager()
	{
		for (int32 i = 0; i < GAMECONFIG->WorldBossSpawnTemplateIdArray.size(); ++i)
		{
			if (i >= max_world_boss_and_gold_army_num)
			{
				break;
			}
			int32 spawn_point_template_id = GAMECONFIG->WorldBossSpawnTemplateIdArray[i];
			init_world_boss_data(spawn_point_template_id);
		}
	}

	world_boss_ws* world_boss_ws_mgr::get_world_boss_info(int32 respawn_point_template_id)
	{
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			if (m_world_boss_data[i].get_spawn_id() == respawn_point_template_id)
			{
				return &m_world_boss_data[i];
			}
		}
		return nullptr;
	}

	world_boss_ws* world_boss_ws_mgr::add_world_boss_info(const s_world_boss_info world_boss_info)
	{
		world_boss_ws* temp_info_ptr = get_world_boss_info(world_boss_info.npc_respawn_point_template_id);
		if (temp_info_ptr == nullptr)
		{
			//此处为空，说明当前没有这个Boss数据
			//因为在load前，是先要用gameconfig表格中的数据初始化存储器的，所以一般来说，是一定能找得到的
			//找不到，就说明gameconfig中就没有，也就说明这个boss被删了，也就不用存档了
			return nullptr;
		}
		else
		{
			temp_info_ptr->init_by_info(world_boss_info);
		}
		return temp_info_ptr;
	}

	world_boss_ws* world_boss_ws_mgr::get_empty_boss_info()
	{
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			if (m_world_boss_data[i].get_spawn_id() == 0)
			{
				return &m_world_boss_data[i];
			}
		}
		return nullptr;
	}

	void world_boss_ws_mgr::load_world_boss_event_end(const s_world_boss_info* world_boss_info_ptr, int32 info_num)
	{
		//这个代码只在 服务器启动时 调用的，从数据库读服务器存档的结果后调用
		//所以在这里，要重新生成上次服务器下线时，没死的世界boss
		for (int32 i = 0; i < info_num; ++i)
		{
			s_world_boss_info temp_info = world_boss_info_ptr[i];
			world_boss_ws* temp_world = add_world_boss_info(temp_info);
			if (temp_world != nullptr
				&& temp_info.is_alive > 0)
			{
				temp_world->set_pre_create(true);
				operate_respawn_list(temp_world->get_spawn_id(), true);
			}
		}
	}
	void world_boss_ws_mgr::send_world_boss_list()
	{
		int32 record_size = m_damage_rank_indicator.get_score_record_list_all().size();
		for (int32 i = 0; i < record_size; ++i)
		{
			client_session *session_info = client_session_mgr::getInstance().get_session(m_damage_rank_indicator.get_score_record_list_all()[i].role_guid);
			if (session_info)
			{
				send_damage_list(session_info);
			}
		}
	}
	void world_boss_ws_mgr::set_world_boss_dead(int32 npc_spawn_template_id, const xchar* killer_name)
	{
		world_boss_ws* temp_info = get_world_boss_info(npc_spawn_template_id);
		if (temp_info == nullptr)
		{
			return;
		}

		temp_info->set_world_boss_dead(killer_name);
		calculate_other_score(npc_spawn_template_id);
		send_world_boss_one(*temp_info, true);
		send_world_boss_list();
		operate_respawn_list(npc_spawn_template_id, true);

	}

	void world_boss_ws_mgr::send_world_boss_all(client_session* session, bool is_need_show)
	{
		world_boss_ws* total_boss_info = world_boss_ws_mgr::get_instance().get_total_world_boss_info();
		if (total_boss_info == nullptr)
		{
			return;
		}
		character_proto_update_world_boss msg;
		msg.set_is_need_show(is_need_show);
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			if (total_boss_info[i].get_spawn_id() > 0)
			{
				character_proto_world_boss_data* pak = msg.add_world_boss_array();
				if (nullptr == pak)
				{
					return;
				}
				pak->set_killer_name(total_boss_info[i].get_killer_name());
				pak->set_first_killer_name(total_boss_info[i].get_first_killer_name());
				pak->set_first_killed_time(total_boss_info[i].get_first_killed_time());
				pak->set_npc_respawn_point_template_id(total_boss_info[i].get_spawn_id());
				pak->set_is_alive(total_boss_info[i].is_alive());
				pak->set_line_id(total_boss_info[i].get_line_id());
				pak->set_is_get_prize(total_boss_info[i].get_is_get_prize());
				pak->set_random_pos_id(total_boss_info[i].get_boss_random_pos_id());
				pak->set_boss_daily_refresh(total_boss_info[i].boss_state());
			}
		}

		if (session != nullptr)
		{
			session->send_to_client(&msg, e_msgindex_s2c_update_world_boss);
		}
		else
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_update_world_boss);
		}
	}
	void world_boss_ws_mgr::send_world_boss_one(world_boss_ws send_info, bool is_need_show, bool is_only_to_first_killer)
	{
		character_proto_update_world_boss msg;
		msg.set_is_need_show(is_need_show);
		character_proto_world_boss_data* pak = msg.add_world_boss_array();
		if (nullptr == pak)
		{
			return;
		}
		pak->set_killer_name(send_info.get_killer_name());
		pak->set_first_killer_name(send_info.get_first_killer_name());
		pak->set_first_killed_time(send_info.get_first_killed_time());
		pak->set_npc_respawn_point_template_id(send_info.get_spawn_id());
		pak->set_is_alive(send_info.is_alive());
		pak->set_line_id(send_info.get_line_id());
		pak->set_is_get_prize(send_info.get_is_get_prize());
		pak->set_random_pos_id(send_info.get_boss_random_pos_id());
		pak->set_boss_daily_refresh(send_info.boss_state());
		if (false == is_only_to_first_killer)
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_update_world_boss);
		}
		else
		{
			client_session* first_killer = client_session_mgr::getInstance().get_session_by_role_name(send_info.get_first_killer_name());
			if (nullptr == first_killer)
			{
				return;
			}
			first_killer->send_to_client(&msg, e_msgindex_s2c_update_world_boss);
		}
	}

	void world_boss_ws_mgr::send_all_boss_hp_per(client_session* session)
	{
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			if (m_world_boss_data[i].is_alive() && m_world_boss_data[i].get_hp_per() > 0)
			{
				character_proto_npc_left_hp_per msg;
				msg.set_npc_spawn_point_template_id(m_world_boss_data[i].get_spawn_id());
				msg.set_left_hp_per(m_world_boss_data[i].get_hp_per());
				session->send_to_client(&msg, e_msgindex_s2c_npc_left_hp_per);
			}
		}
	}

	void world_boss_ws_mgr::set_damage_list(const score_record* top_record_arr, int32 record_num, int32 boss_type, bool is_legion_score, int32 cur_npc_id)
	{
		if (!is_in_active_time && boss_type == e_boss_type_world)
		{
			return;
		}
		if (m_cur_score_npc_id != cur_npc_id)
		{
			m_damage_rank_indicator.clear_data();
			m_legion_rank_indicator.clear_data();
		}
		m_is_need_sort = true;
		m_cur_score_npc_id = cur_npc_id;
		m_cur_boss_type = boss_type;
		if (!is_legion_score)
		{
			for (int32 i = 0; i < record_num; ++i)
			{
				if (!top_record_arr[i].role_guid.is_valid())
				{
					break;
				}

				//世界boss个人得分信息
				if (boss_type == e_boss_type_world)
				{
					int64 last_score = 0;
					if (m_personal_world_boss_score_info.find(cur_npc_id) != m_personal_world_boss_score_info.end())
					{
						last_score = m_personal_world_boss_score_info[cur_npc_id].set_score_record_by_list(top_record_arr[i]);
					}
					else
					{
						score_indicator new_score_indicator;
						m_personal_world_boss_score_info.insert({ cur_npc_id, new_score_indicator });
						last_score = m_personal_world_boss_score_info[cur_npc_id].set_score_record_by_list(top_record_arr[i]);
					}

					m_personal_total_rank_indicator.add_score_by_info(top_record_arr[i].role_guid, top_record_arr[i].role_name, top_record_arr[i].role_level, top_record_arr[i].score - last_score, top_record_arr[i].server_id, top_record_arr[i].legion_guid);

					if (m_all_join_world_boss_member.find(top_record_arr[i].role_guid) == m_all_join_world_boss_member.end() && top_record_arr[i].legion_guid.is_valid())
					{
						m_all_join_world_boss_member.insert({ top_record_arr[i].role_guid, top_record_arr[i].legion_guid });
					}
				}
				else
				{
					m_damage_rank_indicator.set_score_record_by_list(top_record_arr[i]);
				}
			}
		}
		else
		{
			for (int32 i = 0; i < record_num; ++i)
			{
				if (!top_record_arr[i].role_guid.is_valid())
				{
					break;
				}

				//世界boss军团得分信息
				if (boss_type == e_boss_type_world)
				{
					int64 last_score = 0;
					if (m_legion_world_boss_score_info.find(cur_npc_id) != m_legion_world_boss_score_info.end())
					{
						last_score = m_legion_world_boss_score_info[cur_npc_id].set_score_record_by_list(top_record_arr[i]);
					}
					else
					{
						score_indicator new_score_indicator;
						m_legion_world_boss_score_info.insert({ cur_npc_id, new_score_indicator });
						last_score = m_legion_world_boss_score_info[cur_npc_id].set_score_record_by_list(top_record_arr[i]);
					}
					m_activity_rank_indicator.add_score_by_info(top_record_arr[i].role_guid, top_record_arr[i].role_name, top_record_arr[i].role_level, top_record_arr[i].score - last_score);
				}
				else
				{
					m_legion_rank_indicator.set_score_record_by_list(top_record_arr[i]);
				}
			}
		}
	}

	void world_boss_ws_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name, const xstring& original_name)
	{
		xchar char_role_name[max_name_size + 1] = { 0 };
		memcpy(char_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());

		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			if (m_world_boss_data[i].get_killer_name() == original_name)
			{
				m_world_boss_data[i].set_killer_name(char_role_name);
			}

			if (m_world_boss_data[i].get_first_killer_name() == original_name)
			{
				m_world_boss_data[i].set_first_killer_name(char_role_name);
			}

		}
	}

	int32 world_boss_ws_mgr::get_cur_first_boss_spawn_id()
	{
		int32 cur_world_level = ranking_mgr_ws::get_world_level_last();
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return first_boss_spawn_id;
		}
		if (act_temp_ptr->ParamInt1 <= 0 || act_temp_ptr->ParamInt2 <= 0 || act_temp_ptr->ParamInt3 <= 0)
		{
			return first_boss_spawn_id;
		}
		if (cur_world_level >= act_temp_ptr->ParamInt3)
		{
			return act_temp_ptr->ParamInt2;
		}
		return act_temp_ptr->ParamInt1;
	}

	void world_boss_ws_mgr::world_boss_change_legion_name_func(guid_64 legion_guid, const xchar* legion_name)
	{
		m_legion_rank_indicator.change_legion_name_func(legion_guid, legion_name);
		m_activity_rank_indicator.change_legion_name_func(legion_guid, legion_name);
	}

	void world_boss_ws_mgr::send_leigon_welfare_activity_end(int32 activity_type)
	{
		ActivityCommonConfigTemplate* _act_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (_act_config_ptr == nullptr)
		{
			return;
		}

		if (activity_type == e_activity_type_world_boss && m_cur_boss_type == e_boss_type_world && m_activity_rank_indicator.get_score_recore_num() > 0)
		{
			m_activity_rank_indicator.sort();
			std::vector<score_record> record_score = m_activity_rank_indicator.get_score_record_list_all();
			int32 rank_index = 1;
			for (int32 idx = 0; idx < record_score.size(); idx++)
			{
				//设置军团福利
				legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(record_score[idx].role_guid);
				if (nullptr == legion_ptr)
				{
					continue;
				}
				legion_ptr->send_legion_welfare_by_activity_and_rank(e_activity_type_world_boss, rank_index);

				//世界boss军团上架
				int32 player_num = 0;
				guid_64* mem_list = find_legion_jion_member(legion_ptr->get_legion_guid(), player_num);
				if (mem_list == nullptr)
				{
					continue;
				}
				std::vector<s_item_template_info> _item_list;
				std::vector<int32>& rwd_data = _act_config_ptr->RankRewards;
				globle_data::get_instance().get_item_list_by_rank_rwd(rank_index, rwd_data, _item_list, ranking_mgr_ws::get_world_level_last());

				for (auto& item_ite : _item_list)
				{
					legion_ptr->give_activity_reward(_act_config_ptr, mem_list, player_num, item_ite.m_item_id, item_ite.m_item_num);
				}


				++rank_index;
			}
			//m_activity_rank_indicator.clear_data();
		}
	}
	void world_boss_ws_mgr::sync_single_boss_rank_result(client_session* session)
	{
		ZoneScoped;
		if (m_cur_boss_type != e_boss_type_world)
		{
			CONSOLE_ERROR("m_cur_boss_type:{} ", m_cur_boss_type);
			return;
		}

		if (session == nullptr)
		{
			CONSOLE_ERROR("session is nullptr");
			return;
		}

		auto map_template_ptr = cs_map_system::get_map_template(session->get_map_guid());
		if (map_template_ptr == nullptr)
		{
			CONSOLE_ERROR("map_template_ptr is nullptr map_guid:{}", session->get_map_guid().server_64);
			return;
		}
		if (map_template_ptr->Type != hld::e_map_type_big_map)
		{
			CONSOLE_ERROR("map_template_ptr->Type:{} ", map_template_ptr->Type);
			return;
		}
		game_proto_boss_result_rank_list_end result_msg;

		int32 top_num = 0;

		for (int32 i = 0; i < 3; i++)
		{
			score_record * tmp = m_damage_rank_indicator.get_score_record_at(i);
			if (tmp == nullptr)
			{
				break;
			}
			game_proto_boss_rank_item *item = result_msg.add_top_list();
			if (item == nullptr)
			{
				break;
			}
			item->set_role_name(tmp->role_name);
			item->set_role_guid_a(tmp->role_guid.A);
			item->set_role_guid_b(tmp->role_guid.B);
			item->set_damage_value(init_unit::change_i64_to_string(tmp->score));
			item->set_rank(i + 1);
		}

		result_msg.set_top_num(top_num);

		game_proto_boss_rank_item *my_item = result_msg.mutable_my_rank();
		if (my_item != nullptr)
		{
			my_item->set_role_name("");
			my_item->set_role_guid_a(0);
			my_item->set_role_guid_b(0);
			my_item->set_damage_value("");
			my_item->set_rank(0);
		}
		int32  my_rank_index = 0;
		score_record * tmp = m_damage_rank_indicator.get_score_record_and_idx(session->get_role_guid(), my_rank_index);
		if (tmp != nullptr && my_item != nullptr)
		{
			my_item->set_role_name(tmp->role_name);
			my_item->set_role_guid_a(tmp->role_guid.A);
			my_item->set_role_guid_b(tmp->role_guid.B);
			my_item->set_damage_value(init_unit::change_i64_to_string(tmp->score));
			my_item->set_rank(my_rank_index);
		}
		result_msg.set_boss_template_id(m_cur_score_npc_id);
		session->send_to_client(&result_msg, e_msgindex_s2c_boss_result_rank_end);


	}
	void world_boss_ws_mgr::sync_boss_rank_result()
	{
		if (m_cur_boss_type != e_boss_type_world)
		{
			return;
		}
		m_damage_rank_indicator.sort();

		int32 record_size = m_damage_rank_indicator.get_score_record_list_all().size();
		for (int32 i = 0; i < record_size; ++i)
		{
			client_session *session_info = client_session_mgr::getInstance().get_session(m_damage_rank_indicator.get_score_record_list_all()[i].role_guid);
			if (session_info)
			{
				sync_single_boss_rank_result(session_info);
			}
		}
	}

	void world_boss_ws_mgr::find_need_create_world_boss()
	{
		if (GAMECONFIG->WorldBossSpawnTemplateIdArray.size() < 12 || GAMECONFIG->WorldBossSpawnTemplateIdArray.size() > max_world_boss_and_gold_army_num)
		{
			return;
		}
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return;
		}
		if (act_temp_ptr->ParamIntArr2.size() < 21)
		{
			return;
		}
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 active_legion_num = legion_ws_mgr::get_instance().get_active_legion_num();
		int32 boss_num = 0;           //需要生成的boss数量

		memset(lower_boss_arr, 0, sizeof(lower_boss_arr));
		memset(intermediate_boss_arr, 0, sizeof(intermediate_boss_arr));
		memset(senior_boss_arr, 0, sizeof(senior_boss_arr));

		if (cur_time_info.second >= m_create_boss_tamp && cur_time_info.second <= m_remove_boss_tamp)
		{
			//低级世界boss
			if (active_legion_num <= act_temp_ptr->ParamIntArr2[0])
			{
				boss_num = act_temp_ptr->ParamIntArr2[3];
			}
			else if (active_legion_num > act_temp_ptr->ParamIntArr2[0] && active_legion_num <= act_temp_ptr->ParamIntArr2[1])
			{
				boss_num = act_temp_ptr->ParamIntArr2[4];
			}
			else if (active_legion_num > act_temp_ptr->ParamIntArr2[1] && active_legion_num <= act_temp_ptr->ParamIntArr2[2])
			{
				boss_num = act_temp_ptr->ParamIntArr2[5];
			}
			else
			{
				boss_num = act_temp_ptr->ParamIntArr2[6];
			}

			if (boss_num > max_world_boss_arr_num)
			{
				return;
			}

			int32 random_arr1[max_world_boss_arr_num] = { 0,1,2,3 };
			for (int32 i = max_world_boss_arr_num - 1; i >= 0; i--)
			{
				int32 temp_rand_num = random_gen::get_random(0, max_world_boss_arr_num - 1);
				int32 temp = random_arr1[i];
				random_arr1[i] = random_arr1[temp_rand_num];
				random_arr1[temp_rand_num] = temp;
			}

			for (int32 j = 0; j < boss_num; j++)
			{
				if (random_arr1[j] >= GAMECONFIG->WorldBossSpawnTemplateIdArray.size())
				{
					break;
				}
				lower_boss_arr[j] = GAMECONFIG->WorldBossSpawnTemplateIdArray[random_arr1[j]];
			}


			//中级世界boss
			boss_num = 0;
			if (active_legion_num <= act_temp_ptr->ParamIntArr2[7])
			{
				boss_num = act_temp_ptr->ParamIntArr2[10];
			}
			else if (active_legion_num > act_temp_ptr->ParamIntArr2[7] && active_legion_num <= act_temp_ptr->ParamIntArr2[8])
			{
				boss_num = act_temp_ptr->ParamIntArr2[11];
			}
			else if (active_legion_num > act_temp_ptr->ParamIntArr2[8] && active_legion_num <= act_temp_ptr->ParamIntArr2[9])
			{
				boss_num = act_temp_ptr->ParamIntArr2[12];
			}
			else
			{
				boss_num = act_temp_ptr->ParamIntArr2[13];
			}

			if (boss_num > max_world_boss_arr_num)
			{
				return;
			}

			int32 random_arr2[max_world_boss_arr_num] = { 0,1,2,3 };
			for (int32 i = max_world_boss_arr_num - 1; i >= 0; i--)
			{
				int32 temp_rand_num = random_gen::get_random(0, max_world_boss_arr_num - 1);
				int32 temp = random_arr2[i];
				random_arr2[i] = random_arr2[temp_rand_num];
				random_arr2[temp_rand_num] = temp;
			}

			for (int32 j = 0; j < boss_num; j++)
			{
				if (random_arr2[j] + 4 >= GAMECONFIG->WorldBossSpawnTemplateIdArray.size())
				{
					break;
				}
				intermediate_boss_arr[j] = GAMECONFIG->WorldBossSpawnTemplateIdArray[random_arr2[j] + 4];
			}


			//高级世界boss
			boss_num = 0;
			if (active_legion_num <= act_temp_ptr->ParamIntArr2[14])
			{
				boss_num = act_temp_ptr->ParamIntArr2[17];
			}
			else if (active_legion_num > act_temp_ptr->ParamIntArr2[14] && active_legion_num <= act_temp_ptr->ParamIntArr2[15])
			{
				boss_num = act_temp_ptr->ParamIntArr2[18];
			}
			else if (active_legion_num > act_temp_ptr->ParamIntArr2[15] && active_legion_num <= act_temp_ptr->ParamIntArr2[16])
			{
				boss_num = act_temp_ptr->ParamIntArr2[19];
			}
			else
			{
				boss_num = act_temp_ptr->ParamIntArr2[20];
			}

			if (boss_num > max_world_boss_arr_num)
			{
				return;
			}

			int32 random_arr3[max_world_boss_arr_num] = { 0,1,2,3 };
			for (int32 i = max_world_boss_arr_num - 1; i >= 0; i--)
			{
				int32 temp_rand_num = random_gen::get_random(0, max_world_boss_arr_num - 1);
				int32 temp = random_arr3[i];
				random_arr3[i] = random_arr3[temp_rand_num];
				random_arr3[temp_rand_num] = temp;
			}

			for (int32 j = 0; j < boss_num; j++)
			{
				if (random_arr2[j] + 8 >= GAMECONFIG->WorldBossSpawnTemplateIdArray.size())
				{
					break;
				}
				senior_boss_arr[j] = GAMECONFIG->WorldBossSpawnTemplateIdArray[random_arr3[j] + 8];
			}
		}
	}

	bool world_boss_ws_mgr::can_create_world_boss(int32 boss_spawn_id)
	{
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return false;
		}

		time_info cur_time_info = time_helper::get_cur_time_new();
		if (cur_time_info.second >= m_create_boss_tamp && cur_time_info.second <= m_remove_boss_tamp)
		{
			if (cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt1)
			{
				for (int32 i = 0; i < max_world_boss_arr_num; i++)
				{
					if (lower_boss_arr[i] == boss_spawn_id)
					{
						return true;
					}
				}
			}
			else if (cur_time_info.second >= m_create_boss_tamp + act_temp_ptr->ParamInt1 && cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt2)
			{
				for (int32 i = 0; i < max_world_boss_arr_num; i++)
				{
					if (intermediate_boss_arr[i] == boss_spawn_id)
					{
						return true;
					}
				}
			}
			else
			{
				for (int32 i = 0; i < max_world_boss_arr_num; i++)
				{
					if (senior_boss_arr[i] == boss_spawn_id)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool world_boss_ws_mgr::can_remove_world_boss(int32 boss_spawn_id)
	{
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return false;
		}

		time_info cur_time_info = time_helper::get_cur_time_new();
		if (cur_time_info.second < m_create_boss_tamp || cur_time_info.second > m_create_boss_tamp + act_temp_ptr->ParamInt1)
		{
			for (int32 i = 0; i < max_world_boss_arr_num; i++)
			{
				if (lower_boss_arr[i] == boss_spawn_id)
				{
					return true;
				}
			}
		}
		
		if (cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt1 || cur_time_info.second > m_create_boss_tamp + act_temp_ptr->ParamInt2)
		{
			for (int32 i = 0; i < max_world_boss_arr_num; i++)
			{
				if (intermediate_boss_arr[i] == boss_spawn_id)
				{
					return true;
				}
			}
		}

		if (cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt2 || cur_time_info.second > m_remove_boss_tamp)
		{
			for (int32 i = 0; i < max_world_boss_arr_num; i++)
			{
				if (senior_boss_arr[i] == boss_spawn_id)
				{
					return true;
				}
			}
		}

		return false;
	}

	void world_boss_ws_mgr::assign_world_boss_to_legion()
	{
		if (lower_boss_arr[0] == 0 || intermediate_boss_arr[0] == 0 || senior_boss_arr[0] == 0)
		{
			return;
		}

		int32 active_low_index = 0;
		int32 active_intermediate_index = 0;
		int32 active_senior_index = 0;
		int32 low_index = 0;
		int32 intermediate_index = 0;
		int32 senior_index = 0;

		for (auto ite = legion_ws_mgr::get_instance().get_legion_map().begin(); ite != legion_ws_mgr::get_instance().get_legion_map().end(); )
		{
			// tick里可能删掉legion 要用这种方式防止迭代器失效
			auto temp_it = ite++;
			//统计活跃军团数量
			if (temp_it->second.week_is_have_player_active())
			{
				if (active_low_index >= max_world_boss_arr_num || lower_boss_arr[active_low_index] == 0)
				{
					active_low_index = 0;
				}
				temp_it->second.set_strategy_world_boss_arr(0, lower_boss_arr[active_low_index]);
				active_low_index++;

				if (active_intermediate_index >= max_world_boss_arr_num || intermediate_boss_arr[active_intermediate_index] == 0)
				{
					active_intermediate_index = 0;
				}
				temp_it->second.set_strategy_world_boss_arr(1, intermediate_boss_arr[active_intermediate_index]);
				active_intermediate_index++;

				if (active_senior_index >= max_world_boss_arr_num || senior_boss_arr[active_senior_index] == 0)
				{
					active_senior_index = 0;
				}
				temp_it->second.set_strategy_world_boss_arr(2, senior_boss_arr[active_senior_index]);
				active_senior_index++;
			}
			else
			{
				if (low_index >= max_world_boss_arr_num || lower_boss_arr[low_index] == 0)
				{
					low_index = 0;
				}
				temp_it->second.set_strategy_world_boss_arr(0, lower_boss_arr[low_index]);
				low_index++;

				if (intermediate_index >= max_world_boss_arr_num || intermediate_boss_arr[intermediate_index] == 0)
				{
					intermediate_index = 0;
				}
				temp_it->second.set_strategy_world_boss_arr(1, intermediate_boss_arr[intermediate_index]);
				intermediate_index++;

				if (senior_index >= max_world_boss_arr_num || senior_boss_arr[senior_index] == 0)
				{
					senior_index = 0;
				}
				temp_it->second.set_strategy_world_boss_arr(2, senior_boss_arr[senior_index]);
				senior_index++;
			}
		}
	}

	void world_boss_ws_mgr::assign_world_boss_to_one_legion(const guid_64& legion_guid)
	{
		if (lower_boss_arr[0] == 0 || intermediate_boss_arr[0] == 0 || senior_boss_arr[0] == 0)
		{
			return;
		}
		legion_ws_map::iterator ite = legion_ws_mgr::get_instance().get_legion_map().find(legion_guid);
		if (ite != legion_ws_mgr::get_instance().get_legion_map().end())
		{
			ite->second.set_strategy_world_boss_arr(0, lower_boss_arr[0]);
			ite->second.set_strategy_world_boss_arr(1, intermediate_boss_arr[0]);
			ite->second.set_strategy_world_boss_arr(2, senior_boss_arr[0]);
		}
	}

	void world_boss_ws_mgr::send_strategy_world_boss_info(const guid_64& legion_guid, const guid_64& role_guid)
	{
		legion_ws_map::iterator ite;
		ite = legion_ws_mgr::get_instance().get_legion_map().find(legion_guid);
		if (ite != legion_ws_mgr::get_instance().get_legion_map().end())
		{
			ite->second.send_one_legion_strategy_world_boss(role_guid);
		}
	}

	guid_64* world_boss_ws_mgr::find_legion_jion_member(const guid_64& legion_guid, int32& player_num)
	{
		player_num = 0;
		for (int32 i = 0; i < max_legion_member_num; i++)
		{
			legion_member_arr[i].clear_data();
		}

		std::map<guid_64, guid_64>::iterator ite;
		for (ite = m_all_join_world_boss_member.begin(); ite != m_all_join_world_boss_member.end(); ite++)
		{
			if (ite->second == legion_guid)
			{
				legion_member_arr[player_num] = ite->first;
				player_num++;
			}
		}
		return legion_member_arr;
	}

	void world_boss_ws_mgr::send_strategy_reward()
	{
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return;
		}
		if (act_temp_ptr->ParamIntArr4.size() < 15)
		{
			return;
		}

		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_world_boss_strategy_reward_title);
		const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_world_boss_strategy_reward_context);
		if (nullptr == title || nullptr == content)
		{
			return;
		}

		for (auto ite = legion_ws_mgr::get_instance().get_legion_map().begin(); ite != legion_ws_mgr::get_instance().get_legion_map().end(); )
		{
			int32 reward_level = 0;
			// tick里可能删掉legion 要用这种方式防止迭代器失效
			auto temp_it = ite++;

			NpcSpawnPointTemplate* low_npc_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->second.get_strategy_world_boss_arr(0));
			if (low_npc_temp_ptr == nullptr)
			{
				continue;
			}
			int32 low_boss_id = low_npc_temp_ptr->TemplateId;
			if (m_legion_world_boss_score_info.count(low_boss_id) == 1)
			{
				score_record* legion_score = m_legion_world_boss_score_info[low_boss_id].get_score_record(temp_it->first);
				world_boss_ws* world_info = get_world_boss_info(temp_it->second.get_strategy_world_boss_arr(0));
				if (legion_score != nullptr && world_info != nullptr && !world_info->is_alive())
				{
					reward_level++;
				}
			}

			NpcSpawnPointTemplate* intermediate_npc_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->second.get_strategy_world_boss_arr(1));
			if (intermediate_npc_temp_ptr == nullptr)
			{
				continue;
			}
			int32 intermediate_boss_id = intermediate_npc_temp_ptr->TemplateId;
			if (m_legion_world_boss_score_info.count(intermediate_boss_id) == 1)
			{
				score_record* legion_score = m_legion_world_boss_score_info[intermediate_boss_id].get_score_record(temp_it->first);
				world_boss_ws* world_info = get_world_boss_info(temp_it->second.get_strategy_world_boss_arr(1));
				if (legion_score != nullptr && world_info != nullptr && !world_info->is_alive())
				{
					reward_level++;
				}
			}

			NpcSpawnPointTemplate* senior_npc_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->second.get_strategy_world_boss_arr(2));
			if (senior_npc_temp_ptr == nullptr)
			{
				continue;
			}
			int32 senior_boss_id = senior_npc_temp_ptr->TemplateId;
			if (m_legion_world_boss_score_info.count(senior_boss_id) == 1)
			{
				score_record* legion_score = m_legion_world_boss_score_info[senior_boss_id].get_score_record(temp_it->first);
				world_boss_ws* world_info = get_world_boss_info(temp_it->second.get_strategy_world_boss_arr(2));
				if (legion_score != nullptr && world_info != nullptr && !world_info->is_alive())
				{
					reward_level++;
				}
			}

			int32 player_num = 0;
			guid_64* mem_list = find_legion_jion_member(temp_it->first, player_num);
			if (mem_list == nullptr || reward_level == 0)
			{
				continue;
			}

			//奖励物品
			std::vector<s_item_template_info> drop_item_list;

			for (int32 i = 0; i < player_num; i++)
			{
				for (int32 j = 0; j < act_temp_ptr->ParamIntArr4.size() - 4; j++)
				{
					if (act_temp_ptr->ParamIntArr4[j] == reward_level)
					{
						drop_item_list.clear();
						if (act_temp_ptr->ParamIntArr4[j + 1] != 0 && act_temp_ptr->ParamIntArr4[j + 2] != 0)
						{
							drop_item_list.push_back({ act_temp_ptr->ParamIntArr4[j + 1],act_temp_ptr->ParamIntArr4[j + 2] });
						}
						if (act_temp_ptr->ParamIntArr4[j + 3] != 0 && act_temp_ptr->ParamIntArr4[j + 4] != 0)
						{
							drop_item_list.push_back({ act_temp_ptr->ParamIntArr4[j + 3] ,act_temp_ptr->ParamIntArr4[j + 4] });
						}
						if (mem_list != nullptr && mem_list[i].is_valid())
						{
							mail_ws_mgr::get_instance().send_mail_system(mem_list[i], 0, drop_item_list, title, content);
							break;
						}
					}
				}
			}
		}
	}

	void world_boss_ws_mgr::send_world_boss_info(const guid_64& legion_guid, const guid_64& role_guid, int32 cur_boss_level)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		time_info cur_time_info = time_helper::get_cur_time_new();

		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr != nullptr)
		{
			int32 boss_level = 0;
			int32 temp_arr[max_world_boss_arr_num];
			memset(temp_arr, 0, sizeof(temp_arr));

			if (cur_boss_level == 0)
			{
				if (cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt1)
				{
					boss_level = 0;
					int32 j = 0;
					for (int32 i = 0; i < max_world_boss_arr_num; i++)
					{
						world_boss_ws* world_info = get_world_boss_info(lower_boss_arr[i]);
						if (world_info != nullptr)
						{
							if (world_info->is_alive())
							{
								temp_arr[j] = lower_boss_arr[i];
								j++;
							}
						}
					}
				}
				else if (cur_time_info.second >= m_create_boss_tamp + act_temp_ptr->ParamInt1 && cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt2)
				{
					boss_level = 1;
					int32 j = 0;
					for (int32 i = 0; i < max_world_boss_arr_num; i++)
					{
						world_boss_ws* world_info = get_world_boss_info(intermediate_boss_arr[i]);
						if (world_info != nullptr)
						{
							if (world_info->is_alive())
							{
								temp_arr[j] = intermediate_boss_arr[i];
								j++;
							}
						}
					}
				}
				else
				{
					boss_level = 2;
					int32 j = 0;
					for (int32 i = 0; i < max_world_boss_arr_num; i++)
					{
						world_boss_ws* world_info = get_world_boss_info(senior_boss_arr[i]);
						if (world_info != nullptr)
						{
							if (world_info->is_alive())
							{
								temp_arr[j] = senior_boss_arr[i];
								j++;
							}
						}
					}
				}
			}
			else if (cur_boss_level == 1)
			{
				boss_level = 0;
				int32 j = 0;
				for (int32 i = 0; i < max_world_boss_arr_num; i++)
				{
					world_boss_ws* world_info = get_world_boss_info(lower_boss_arr[i]);
					if (world_info != nullptr)
					{
						if (world_info->is_alive())
						{
							temp_arr[j] = lower_boss_arr[i];
							j++;
						}
					}
				}
			}
			else if (cur_boss_level == 2)
			{
				boss_level = 1;
				int32 j = 0;
				for (int32 i = 0; i < max_world_boss_arr_num; i++)
				{
					world_boss_ws* world_info = get_world_boss_info(intermediate_boss_arr[i]);
					if (world_info != nullptr)
					{
						if (world_info->is_alive())
						{
							temp_arr[j] = intermediate_boss_arr[i];
							j++;
						}
					}
				}
			}
			else if (cur_boss_level == 3)
			{
				boss_level = 2;
				int32 j = 0;
				for (int32 i = 0; i < max_world_boss_arr_num; i++)
				{
					world_boss_ws* world_info = get_world_boss_info(senior_boss_arr[i]);
					if (world_info != nullptr)
					{
						if (world_info->is_alive())
						{
							temp_arr[j] = senior_boss_arr[i];
							j++;
						}
					}
				}
			}
	
			legion_proto_all_world_boss_info msg_all;

			for (int32 i = 0; i < max_world_boss_arr_num; i++)
			{
				int32 legion_player_num = 0;
				if (temp_arr[i] != 0)
				{
					NpcSpawnPointTemplate* npc_spawn_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_arr[i]);
					if (npc_spawn_temp_ptr == nullptr)
					{
						continue;
					}
					if (m_personal_world_boss_score_info.count(npc_spawn_temp_ptr->TemplateId) == 1)
					{
						std::vector<score_record>::iterator iter = m_personal_world_boss_score_info[npc_spawn_temp_ptr->TemplateId].get_score_record_list_all().begin();
						for (; iter != m_personal_world_boss_score_info[npc_spawn_temp_ptr->TemplateId].get_score_record_list_all().end(); iter++)
						{
							if (legion_guid == iter->legion_guid)
							{
								legion_player_num++;
							}
						}
					}
					legion_proto_one_world_boss_info* one_data_info = msg_all.add_world_boss_info_arr();
					if (one_data_info == nullptr)
					{
						return;
					}
					one_data_info->set_boss_level(boss_level);
					one_data_info->set_boss_id(temp_arr[i]);
					one_data_info->set_legion_player_num(legion_player_num);
				}
			}

			for (int32 i = 0; i < max_world_boss_arr_num; ++i)
			{
				if (lower_boss_arr[i] > 0)
				{
					legion_proto_one_world_boss_info* one_data_info = msg_all.add_all_info();
					if (one_data_info == nullptr)
					{
						continue;
					}
					one_data_info->set_boss_level(0);
					one_data_info->set_boss_id(lower_boss_arr[i]);
				}
			}
			for (int32 i = 0; i < max_world_boss_arr_num; ++i)
			{
				if (intermediate_boss_arr[i] > 0)
				{
					legion_proto_one_world_boss_info* one_data_info = msg_all.add_all_info();
					if (one_data_info == nullptr)
					{
						continue;
					}
					one_data_info->set_boss_level(1);
					one_data_info->set_boss_id(intermediate_boss_arr[i]);
				}
			}
			for (int32 i = 0; i < max_world_boss_arr_num; ++i)
			{
				if (senior_boss_arr[i] > 0)
				{
					legion_proto_one_world_boss_info* one_data_info = msg_all.add_all_info();
					if (one_data_info == nullptr)
					{
						continue;
					}
					one_data_info->set_boss_level(2);
					one_data_info->set_boss_id(senior_boss_arr[i]);
				}
			}
			session->send_to_client(&msg_all, e_msgindex_s2c_all_world_boss_info);
		}
	}

	void world_boss_ws_mgr::send_strategy_result()
	{
		m_activity_rank_indicator.sort();
		m_personal_total_rank_indicator.sort();

		int32 rank_num = 3;	//排行榜人数
		legion_proto_world_boss_strategy_result all_rank_msg;
		for (int32 i = 0; i < rank_num; i++)
		{
			if (m_activity_rank_indicator.get_score_record_at(i) == nullptr)
			{
				continue;
			}
			legion_proto_world_boss_score_info *one_data_info = all_rank_msg.add_legion_score_info();
			if (one_data_info == nullptr)
			{
				return;
			}
			one_data_info->set_role_guid(m_activity_rank_indicator.get_score_record_at(i)->role_guid);
			one_data_info->set_role_name(m_activity_rank_indicator.get_score_record_at(i)->role_name);
			one_data_info->set_role_score(m_activity_rank_indicator.get_score_record_at(i)->score);
		}

		for (int32 i = 0; i < rank_num; i++)
		{
			if (m_personal_total_rank_indicator.get_score_record_at(i) == nullptr)
			{
				continue;
			}
			legion_proto_world_boss_score_info *one_data_info = all_rank_msg.add_personal_score_info();
			if (one_data_info == nullptr)
			{
				return;
			}
			one_data_info->set_role_guid(m_personal_total_rank_indicator.get_score_record_at(i)->role_guid);
			one_data_info->set_role_name(m_personal_total_rank_indicator.get_score_record_at(i)->role_name);
			one_data_info->set_role_score(m_personal_total_rank_indicator.get_score_record_at(i)->score);
		}

		legion_ws_map::iterator ite;
		for (ite = legion_ws_mgr::get_instance().get_legion_map().begin(); ite != legion_ws_mgr::get_instance().get_legion_map().end(); )
		{
			int32 reward_level = 0;
			// tick里可能删掉legion 要用这种方式防止迭代器失效
			auto temp_it = ite++;

			NpcSpawnPointTemplate* low_npc_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->second.get_strategy_world_boss_arr(0));
			if (low_npc_temp_ptr == nullptr)
			{
				continue;
			}
			int32 low_boss_id = low_npc_temp_ptr->TemplateId;
			if (m_legion_world_boss_score_info.count(low_boss_id) == 1)
			{
				score_record* legion_score = m_legion_world_boss_score_info[low_boss_id].get_score_record(temp_it->first);
				world_boss_ws* world_info = get_world_boss_info(temp_it->second.get_strategy_world_boss_arr(0));
				if (legion_score != nullptr && world_info != nullptr && !world_info->is_alive())
				{
					reward_level++;
				}
			}

			NpcSpawnPointTemplate* intermediate_npc_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->second.get_strategy_world_boss_arr(1));
			if (intermediate_npc_temp_ptr == nullptr)
			{
				continue;
			}
			int32 intermediate_boss_id = intermediate_npc_temp_ptr->TemplateId;
			if (m_legion_world_boss_score_info.count(intermediate_boss_id) == 1)
			{
				score_record* legion_score = m_legion_world_boss_score_info[intermediate_boss_id].get_score_record(temp_it->first);
				world_boss_ws* world_info = get_world_boss_info(temp_it->second.get_strategy_world_boss_arr(1));
				if (legion_score != nullptr && world_info != nullptr && !world_info->is_alive())
				{
					reward_level++;
				}
			}

			NpcSpawnPointTemplate* senior_npc_temp_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, temp_it->second.get_strategy_world_boss_arr(2));
			if (senior_npc_temp_ptr == nullptr)
			{
				continue;
			}
			int32 senior_boss_id = senior_npc_temp_ptr->TemplateId;
			if (m_legion_world_boss_score_info.count(senior_boss_id) == 1)
			{
				score_record* legion_score = m_legion_world_boss_score_info[senior_boss_id].get_score_record(temp_it->first);
				world_boss_ws* world_info = get_world_boss_info(temp_it->second.get_strategy_world_boss_arr(2));
				if (legion_score != nullptr && world_info != nullptr && !world_info->is_alive())
				{
					reward_level++;
				}
			}

			int32 player_num = 0;
			guid_64* mem_list = find_legion_jion_member(temp_it->first, player_num);
			if (mem_list == nullptr)
			{
				continue;
			}

			for (int32 i = 0; i < player_num; i++)
			{
				client_session* session = client_session_mgr::getInstance().get_session(mem_list[i]);
				if (nullptr == session)
				{
					continue;
				}
				legion_proto_world_boss_score_info *extra_legion_info = all_rank_msg.mutable_legion_extra_score();
				legion_proto_world_boss_score_info *extra_personal_info = all_rank_msg.mutable_personal_extra_score();
				if (extra_legion_info == nullptr || extra_personal_info == nullptr)
				{
					continue;
				}

				if (m_activity_rank_indicator.get_score_record(temp_it->first) != nullptr)
				{
					extra_legion_info->set_role_guid(temp_it->first);
					extra_legion_info->set_role_name(m_activity_rank_indicator.get_score_record(temp_it->first)->role_name);
					extra_legion_info->set_role_score(m_activity_rank_indicator.get_score_record(temp_it->first)->score);
				}
				if (m_personal_total_rank_indicator.get_score_record(mem_list[i]) != nullptr)
				{
					extra_personal_info->set_role_guid(mem_list[i]);
					extra_personal_info->set_role_name(m_personal_total_rank_indicator.get_score_record(mem_list[i])->role_name);
					extra_personal_info->set_role_score(m_personal_total_rank_indicator.get_score_record(mem_list[i])->score);
				}

				all_rank_msg.set_reward_level(reward_level);
				session->send_to_client(&all_rank_msg, e_msgindex_s2c_world_boss_strategy_result);
			}
		}
	}

	void world_boss_ws_mgr::assign_postion_to_world_boss()
	{
		int32 random_arr1[max_world_boss_arr_num] = { 0,1,2,3 };
		for (int32 i = max_world_boss_arr_num - 1; i >= 0; i--)
		{
			int32 temp_rand_num = random_gen::get_random(0, max_world_boss_arr_num - 1);
			int32 temp = random_arr1[i];
			random_arr1[i] = random_arr1[temp_rand_num];
			random_arr1[temp_rand_num] = temp;
		}
		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (lower_boss_arr[i] != 0)
			{
				m_boss_create_pos[lower_boss_arr[i]] = random_arr1[i];
			}
		}

		int32 random_arr2[3] = { 4,5,6 };
		for (int32 i = 2; i >= 0; i--)
		{
			int32 temp_rand_num = random_gen::get_random(0, 2);
			int32 temp = random_arr2[i];
			random_arr2[i] = random_arr2[temp_rand_num];
			random_arr2[temp_rand_num] = temp;
		}
		for (int32 i = 0; i < 3; i++)
		{
			if (intermediate_boss_arr[i] != 0)
			{
				m_boss_create_pos[intermediate_boss_arr[i]] = random_arr2[i];
			}
		}

		int32 random_arr3[2] = { 7,8 };
		for (int32 i = 1; i >= 0; i--)
		{
			int32 temp_rand_num = random_gen::get_random(0, 1);
			int32 temp = random_arr3[i];
			random_arr3[i] = random_arr3[temp_rand_num];
			random_arr3[temp_rand_num] = temp;
		}
		for (int32 i = 0; i < 2; i++)
		{
			if (senior_boss_arr[i] != 0)
			{
				m_boss_create_pos[senior_boss_arr[i]] = random_arr3[i];
			}
		}
	}

	void world_boss_ws_mgr::send_world_boss_notice()
	{
		time_info cur_time_info = time_helper::get_cur_time_new();
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return;
		}
		if (cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt1 && is_send_first_stage_notice)
		{
			int32 notice_id = world_boss_first_stage_notice_id;
			std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
			int32 create_time = time_helper::get_cur_time_new().second;

			legion_ws_map::iterator ite;
			for (ite = legion_ws_mgr::get_instance().get_legion_map().begin(); ite != legion_ws_mgr::get_instance().get_legion_map().end(); )
			{
				// tick里可能删掉legion 要用这种方式防止迭代器失效
				auto temp_it = ite++;
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, temp_it->first, notice_str);
			}
			is_send_first_stage_notice = false;
		}
		else if (cur_time_info.second >= m_create_boss_tamp + act_temp_ptr->ParamInt1 && cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt2 && is_send_second_stage_notice)
		{
			int32 notice_id = world_boss_second_stage_notice_id;
			std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
			int32 create_time = time_helper::get_cur_time_new().second;

			legion_ws_map::iterator ite;
			for (ite = legion_ws_mgr::get_instance().get_legion_map().begin(); ite != legion_ws_mgr::get_instance().get_legion_map().end(); )
			{
				// tick里可能删掉legion 要用这种方式防止迭代器失效
				auto temp_it = ite++;
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, temp_it->first, notice_str);
			}
			is_send_second_stage_notice = false;
		}
		else if (cur_time_info.second >= m_create_boss_tamp + act_temp_ptr->ParamInt2 && cur_time_info.second < m_remove_boss_tamp && is_send_third_stage_notice)
		{
			int32 notice_id = world_boss_third_stage_notice_id;
			std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
			int32 create_time = time_helper::get_cur_time_new().second;

			legion_ws_map::iterator ite;
			for (ite = legion_ws_mgr::get_instance().get_legion_map().begin(); ite != legion_ws_mgr::get_instance().get_legion_map().end(); )
			{
				// tick里可能删掉legion 要用这种方式防止迭代器失效
				auto temp_it = ite++;
				event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, temp_it->first, notice_str);
			}
			is_send_third_stage_notice = false;
		}
	}

	void world_boss_ws_mgr::add_boss_already_dead_score()
	{
		if (!is_in_active_time)
		{
			return;
		}

		ActivityCommonConfigTemplate*act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return;
		}

		time_info cur_time_info = time_helper::get_cur_time_new();

		int world_boss_level = -1;
		if (cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt1)
		{
			world_boss_level = e_world_boss_level_1;
		}
		else if (cur_time_info.second >= m_create_boss_tamp + act_temp_ptr->ParamInt1 && cur_time_info.second < m_create_boss_tamp + act_temp_ptr->ParamInt2)
		{
			world_boss_level = e_world_boss_level_2;
		}
		else if (cur_time_info.second >= m_create_boss_tamp + act_temp_ptr->ParamInt2 && cur_time_info.second < m_remove_boss_tamp)
		{
			world_boss_level = e_world_boss_level_3;
		}

		if (world_boss_level < 0)
		{
			return;
		}

		for (int32 world_boss_arr_num = 0; world_boss_arr_num < max_world_boss_arr_num; world_boss_arr_num++)
		{
			NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, first_boss_spawn_id + max_world_boss_arr_num * world_boss_level + world_boss_arr_num);
			if (spawn_point_template_ptr == nullptr)
			{
				continue;
			}

			world_boss_ws* world_boss_ws_ptr = get_world_boss_info(spawn_point_template_ptr->attribute_id);
			if (world_boss_ws_ptr == nullptr || world_boss_ws_ptr->is_alive())
			{
				continue;
			}

			if (m_personal_world_boss_score_info.find(spawn_point_template_ptr->TemplateId) != m_personal_world_boss_score_info.end())
			{
				std::vector<score_record>& score_record_list_all = m_personal_world_boss_score_info[spawn_point_template_ptr->TemplateId].get_score_record_list_all();
				for (const auto& score_record_one : score_record_list_all)
				{
					if (score_record_one.other_score <= 0)
					{
						continue;
					}
					m_is_need_sort = true;
					m_personal_total_rank_indicator.add_score_by_info(score_record_one.role_guid, score_record_one.role_name, score_record_one.role_level, score_record_one.other_score, score_record_one.server_id, score_record_one.legion_guid);
				}
			}

			if (m_legion_world_boss_score_info.find(spawn_point_template_ptr->TemplateId) != m_legion_world_boss_score_info.end())
			{
				std::vector<score_record>& score_record_list_all = m_legion_world_boss_score_info[spawn_point_template_ptr->TemplateId].get_score_record_list_all();
				for (const auto& score_record_one : score_record_list_all)
				{
					if (score_record_one.other_score <= 0)
					{
						continue;
					}
					m_is_need_sort = true;
					m_activity_rank_indicator.add_score_by_info(score_record_one.role_guid, score_record_one.role_name, score_record_one.role_level, score_record_one.other_score);
				}
			}
		}
	}

	void world_boss_ws_mgr::calculate_other_score(int32 boss_spawn_id)
	{
		//如果能删除表示boss是被系统杀掉的不增加奖励
		if (can_remove_world_boss(boss_spawn_id))
		{
			return;
		}

		int32 boss_create_time = get_world_boss_create_time(boss_spawn_id);
		int32 kill_boss_sum_time = time_helper::get_cur_time_new().second - boss_create_time;

		if (kill_boss_sum_time <= 0)
		{
			return;
		}

		NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, boss_spawn_id);
		if (spawn_point_template_ptr == nullptr)
		{
			return;
		}

		if (m_legion_world_boss_score_info.find(spawn_point_template_ptr->TemplateId) != m_legion_world_boss_score_info.end())
		{
			legion_ws_map& legion_ws_map_ref = legion_ws_mgr::get_instance().get_legion_map();
			for (auto ite = legion_ws_map_ref.begin(); ite != legion_ws_map_ref.end(); ite++)
			{
				if (ite->second.get_strategy_world_boss_arr(get_world_boss_level(boss_spawn_id)) != boss_spawn_id)
				{
					continue;
				}
				
				m_legion_world_boss_score_info[spawn_point_template_ptr->TemplateId].calculate_other_score(ite->first, kill_boss_sum_time);

				if (m_personal_world_boss_score_info.find(spawn_point_template_ptr->TemplateId) != m_personal_world_boss_score_info.end())
				{
					for (auto mem_ite = m_all_join_world_boss_member.begin(); mem_ite != m_all_join_world_boss_member.end(); mem_ite++)
					{
						if ((guid_64)ite->first == mem_ite->second)
						{
							m_personal_world_boss_score_info[spawn_point_template_ptr->TemplateId].calculate_other_score(mem_ite->first, kill_boss_sum_time);
						}
					}
				}
			}
		}
	}

	int32 world_boss_ws_mgr::get_world_boss_create_time(int32 boss_spawn_id)
	{
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return 0;
		}

		time_info cur_time_info = time_helper::get_cur_time_new();
		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (lower_boss_arr[i] == boss_spawn_id)
			{
				return m_create_boss_tamp;
			}
		}

		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (intermediate_boss_arr[i] == boss_spawn_id)
			{
				return m_create_boss_tamp + act_temp_ptr->ParamInt1;
			}
		}

		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (senior_boss_arr[i] == boss_spawn_id)
			{
				return m_create_boss_tamp + act_temp_ptr->ParamInt2;
			}
		}
		return 0;
	}

	int32 world_boss_ws_mgr::get_world_boss_level(int32 boss_spawn_id)
	{
		ActivityCommonConfigTemplate* act_temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		if (act_temp_ptr == nullptr)
		{
			return e_world_boss_level_max;
		}

		time_info cur_time_info = time_helper::get_cur_time_new();
		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (lower_boss_arr[i] == boss_spawn_id)
			{
				return e_world_boss_level_1;
			}
		}

		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (intermediate_boss_arr[i] == boss_spawn_id)
			{
				return e_world_boss_level_2;
			}
		}

		for (int32 i = 0; i < max_world_boss_arr_num; i++)
		{
			if (senior_boss_arr[i] == boss_spawn_id)
			{
				return e_world_boss_level_3;
			}
		}
		return e_world_boss_level_max;
	}

}
