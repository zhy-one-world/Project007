/********************************************************************
created: 2017年6月20日
file base: legion_cs_mgr
file ext: h
author: Kero
purpose: legion mgr in cs
*********************************************************************/

#include "cell_server.hpp"
#include "components/item/item_base_component.h"
#include "connection_mgr.hpp"
#include "internet/legion.pb.h"
#include "legion_cs_mgr.h"
#include "Logic/activity_def.hpp"
#include "logic/buff_man.h"
#include "Logic/cross_server_world_boss_def.hpp"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "internet/net.pb.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include <Utility/parse_msg.h>
#include "npc.hpp"
#include "internal/core.hpp"
#include "internal/cross_server_world_boss_msg.hpp"
#include "internet/item.pb.h"
#include <rlog.hpp>




namespace faith
{

//#define PROTO_FUNC_TO_PACKET_EX(FUNC, NAME)                \
//	faith::ws2cs_proto::##NAME msg;          \
//	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len); \
//	if (is_sucess == false)   \
//	{                        \
//		return;               \
//	}                          \
//	ws2cs_##NAME packet; \
//	packet.from_proto(msg);          \
//	FUNC(conn_index, &packet, sizeof(ws2cs_##NAME)); \

	legion_cs_mgr::legion_cs_mgr()
	{
		m_unit_array_index = -1;
		clear_data();
	}

	void legion_cs_mgr::clear_data()
	{
		m_unit_legion_info.clear_data();
		m_get_legion_boss_award_log.clear();
		m_last_convenient_join_legion_time = 0;
		m_is_city_master = false;
		m_is_win_city_war_legion_member = false;
		m_occupied_safe_ids.clear();
		m_last_terr_safe_id = -1;
	}

	void legion_cs_mgr::set_unit_array_index(int32 unit_array_index)
	{
		m_unit_array_index = unit_array_index;
	}

	void legion_cs_mgr::set_legion_info(const s_player_legion_info& legion_info)
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (false == m_unit_ref.is_valid())
		{
			return;
		}

		if (false == legion_info.legion_guid.is_valid())
		{
			return;
		}
		m_unit_legion_info = legion_info;
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid())
		{
			if (false == player_ref.is_self_server())
			{
				return;
			}
		}
		set_occupied_territorys();
		
		legion_proto_set_player_legion_aoi_info set_player_legion_aoi_info_msg;
		set_player_legion_aoi_info_msg.set_role_guid(m_unit_ref.get_unit_guid().server_64);
		set_player_legion_aoi_info_msg.set_legion_guid(legion_info.legion_guid.server_64);
		set_player_legion_aoi_info_msg.set_legion_name(legion_info.legion_name);
		set_player_legion_aoi_info_msg.set_job_title(legion_info.job_title);
		m_unit_ref.send_message_to_aoi(&set_player_legion_aoi_info_msg, e_msgindex_s2c_set_player_legion_aoi_info);

		del_territory_buff(m_unit_array_index);
		int32 cur_terr_safe_id = get_cur_legion_territory_safe_id();
		if (cur_terr_safe_id > 0)
		{
			m_last_terr_safe_id = cur_terr_safe_id;
			MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, cur_terr_safe_id);
			if (map_safe_template_ptr != nullptr)
			{
				add_territory_buff(m_unit_array_index, map_safe_template_ptr->DependentTerritory);
			}
		}
	}

	void legion_cs_mgr::change_legion_terr_buff()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (false == m_unit_ref.is_valid())
		{
			return;
		}

		if (false == m_unit_legion_info.legion_guid.is_valid())
		{
			return;
		}
		int32 cur_terr_safe_id = get_cur_legion_territory_safe_id();
		if (cur_terr_safe_id > 0)
		{
			if (cur_terr_safe_id == m_last_terr_safe_id)
			{
				return;
			}
			m_last_terr_safe_id = cur_terr_safe_id;
			del_territory_buff(m_unit_array_index);
			MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, cur_terr_safe_id);
			if (map_safe_template_ptr != nullptr)
			{
				add_territory_buff(m_unit_array_index, map_safe_template_ptr->DependentTerritory);
			}
		}
		else
		{
			if (cur_terr_safe_id == m_last_terr_safe_id)
			{
				return;
			}
			m_last_terr_safe_id = cur_terr_safe_id;
			del_territory_buff(m_unit_array_index);
		}
	}

	void legion_cs_mgr::add_cross_server_boss_kill_legion_log(int32 npc_arr_idx)
	{
		npc& npc_ref = unit_man::get_npc(npc_arr_idx);
		if (!npc_ref.is_valid())
		{
			return;
		}
		player& play_ptr = unit_man::get_player(npc_ref.get_killer_array_index());
		if (!play_ptr.is_valid())
		{
			return;
		}
		score_record score_top1;
		score_record score_top2;
		score_record score_top3;

		std::vector<score_record> legion_score_rank = npc_ref.get_legion_score_rank().get_score_record_list_all();
		int32 score_size = legion_score_rank.size();
		if (score_size >= 1)
		{
			score_top1 = legion_score_rank[0];
		}
		if (score_size >= 2)
		{
			score_top2 = legion_score_rank[1];
		}
		if (score_size >= 3)
		{
			score_top3 = legion_score_rank[2];
		}
		
		server_log::cross_server_boss_kill_log(npc_ref.get_npc_template_id(), play_ptr.get_legion_guid(), play_ptr.get_legion_info().legion_name, play_ptr.get_unit_info(e_role_info_server_id),score_top1.role_guid, score_top1.role_name, score_top1.server_id, score_top2.role_guid, score_top2.role_name, score_top2.server_id,score_top3.role_guid, score_top3.role_name, score_top3.server_id);

		set_log_var(log_head);
		play_ptr.get_log_common_head_info(log_head);

		server_log::serverCrossServerBossKill(log_head, npc_ref.get_npc_template_id(), play_ptr.get_legion_guid().server_64, play_ptr.get_legion_info().legion_name,
			score_top1.role_guid.server_64, score_top1.role_name, 
			score_top2.role_guid.server_64, score_top2.role_name,
			score_top3.role_guid.server_64, score_top3.role_name);



	}
 	void legion_cs_mgr::on_npc_dead(int32 npc_arr_idx)
 	{
		npc& npc_ref = unit_man::get_npc(npc_arr_idx);
		if (!npc_ref.is_valid())
		{
			return;
		}
		if (npc_ref.get_unit_type() != e_unit_type_monster)
		{
			return;
		}
		float gg_fix = npc_ref.get_npc_template()->DeadGloryGlow;
		player& killer_ref = unit_man::get_player(npc_ref.get_killer_array_index());

 		damage_info_vector damage_list = npc_ref.get_ai_unit().get_damage_list();
		std::sort(damage_list.begin(), damage_list.end(), std::greater<s_damage_info>());
		int32 list_len = damage_list.size();
		if (FLOAT_IS_ZERO(gg_fix) == false)
		{
			cs2ws_legion_add_glory_glow add_glory_glow_msg;

			for (int32 idx = 0; idx < list_len; idx++)
			{
				const s_damage_info& damage_info = damage_list[idx];
				int32 gg_val = damage_info.damage_value * gg_fix;
				if (gg_val <= 0)
				{
					continue;
				}
			
				add_glory_glow_msg.role_guid = damage_info.unit_identifier.unit_guid;
				add_glory_glow_msg.add_gg_value = gg_val;
				connection_mgr::getInstance().send_to_ws( &add_glory_glow_msg, sizeof(add_glory_glow_msg));
			}
		}
		
		auto map_type = base_map_system::get_map_type(npc_ref.get_map_ent());
		auto map_sub_type = base_map_system::get_map_sub_type(npc_ref.get_map_ent());
		if (npc_ref.get_unit_sub_type() != e_monster_type_nomal)
		{
			add_cross_server_boss_kill_legion_log(npc_arr_idx);
		}
		cs2ws_settle_legion_act_rank legion_act_rank_msg;
		switch (npc_ref.get_unit_sub_type())
		{
		case e_monster_type_boss:
		{
			if (map_type == e_map_type_legion_station)
			{
				//legion_station_map_cs* legion_boss_map_cs_ptr = dynamic_cast<legion_station_map_cs*>(map_object_ptr);
				//if (nullptr == legion_boss_map_cs_ptr)
				//{
				//	return;
				//}
				//cs2ws_legion_boss_damage_player damage_msg;
				//damage_msg.npc_id = npc_ref.get_npc_template_id();
				//damage_msg.legion_guid = legion_boss_map_cs_ptr->get_map_own_legion();

				//int32 member_num = 0;
				//score_indicator& play_rank_info = npc_ref.get_play_score_rank();
				//play_rank_info.sort();
				//std::vector<score_record> record_score = npc_ref.get_play_score_rank().get_score_record_list_all();
				//for (int32 idx = 0; idx < record_score.size(); idx++)
				//{
				//	const score_record& damage_info = record_score[idx];
				//	if (member_num >= max_legion_member_num)
				//	{
				//		break;
				//	}
				//	if (damage_info.role_guid.is_valid())
				//	{
				//		damage_msg.player_guid_arr[member_num] = damage_info.role_guid;
				//	}
				//	member_num++;
				//}
				//damage_msg.member_num = member_num;
				//connection_mgr::getInstance().send_to_ws(&damage_msg, sizeof(damage_msg));
			}
			if (map_type == e_map_type_cross_server_pk && map_sub_type == 4)
			{
				player& play_ptr = unit_man::get_player(npc_ref.get_killer_array_index());
				if (!play_ptr.is_valid())
				{
					return;
				}
				cs2ws_make_cross_server_world_legion_award msg;
				int32 _rank = 0;
				auto play_rank_info = npc_ref.get_play_score_rank();
				auto record_score = npc_ref.get_play_score_rank().get_score_record_list_all();
				if (record_score.size() <= 0)
				{
					return;
				}
				for (int32 idx = 0; idx < record_score.size(); idx++)
				{
					_rank++;
					const score_record& damage_info = record_score[idx];
					if (msg.member_num >= max_hurt_monster_player_num)
					{
						break;
					}
					if (damage_info.role_guid.is_valid())
					{
						msg.member_scores[msg.member_num].play_guid = damage_info.role_guid;
						msg.member_scores[msg.member_num].legion_guid = damage_info.legion_guid;
						msg.member_num++;
						_RLOG_(MINFO, ::faith::log_detail::format_message("roleguid:{} legionguid:{} bosstemplateid:{} damage:{}", damage_info.role_guid.server_64, damage_info.legion_guid.server_64,  npc_ref.get_npc_template_id(),  damage_info.score));
					}
				}
				msg.boss_id = npc_ref.get_npc_template_id();
				msg.kill_boss_play_guid = play_ptr.get_unit_guid();
				npc_ref.get_legion_score_rank().sort();
				std::vector<score_record> legion_score_rank = npc_ref.get_legion_score_rank().get_score_record_list_all();
				msg.score_num = act_num;
				if (legion_score_rank.size() < act_num)
				{
					msg.score_num = legion_score_rank.size();
				}
				for (int32 i = 0; i < msg.score_num; i++)
				{
					msg.legion_score_rank[i] = legion_score_rank[i];
				}
				connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
			}
		}
		break;
		case e_monster_type_legion_bonfire:
		{
			cs2ws_bonfire_dead bonfire_dead_msg;
			bonfire_dead_msg.bonfire_guid = npc_ref.get_unit_guid();
			bonfire_dead_msg.legion_guid = npc_ref.get_legion_guid();
			connection_mgr::getInstance().send_to_ws( &bonfire_dead_msg, sizeof(bonfire_dead_msg));
		}
		break;
		//case e_monster_type_world_boss:
		//	legion_act_rank_msg.activity_rank_type = e_legion_rank_type_world_elite;
		//	// 这里不用break 要走下面的boss的逻辑 也不要在这里插入代码
		//case e_monster_type_gold_army:
		//{
		//	if (legion_act_rank_msg.activity_rank_type < 0)
		//	{
		//		legion_act_rank_msg.activity_rank_type = e_legion_rank_type_world_boss;
		//	}
		//	legion_act_rank_msg.activity_sub_id = npc_ref.get_npc_template_id();
		//	legion_act_rank_msg.special_legion_guid = killer_ref.get_legion_guid();
		//	int32 _rank = 0;
		//	for (int32 idx = 0; idx < list_len; idx++)
		//	{
		//		_rank++;
		//		const s_damage_info& damage_info = damage_list[idx];
		//		if (legion_act_rank_msg.member_num >= max_hurt_monster_player_num)
		//		{
		//			break;
		//		}
		//		if (damage_info.unit_legion_guid.is_valid())
		//		{
		//			legion_act_rank_msg.member_scores[legion_act_rank_msg.member_num].mem_guid = damage_info.unit_identifier.unit_guid;
		//			legion_act_rank_msg.member_scores[legion_act_rank_msg.member_num].mem_legion_guid = damage_info.unit_legion_guid;
		//			legion_act_rank_msg.member_scores[legion_act_rank_msg.member_num].mem_score = damage_info.damage_value / 10000 > 0 ? damage_info.damage_value / 10000 : 1;
		//			legion_act_rank_msg.member_scores[legion_act_rank_msg.member_num].mem_rank_num = _rank;
		//			legion_act_rank_msg.member_num++;
		//		}
		//		else
		//		{
		//			ActivityCommonConfigTemplate* _act_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
		//			if (_act_config_ptr == nullptr)
		//			{
		//				return;
		//			}
		//			std::vector<int32>& rwd_data = _act_config_ptr->RankRewards;
		//			std::vector<s_item_template_info> item_list;
		//			globle_data::get_instance().get_item_list_by_rank_rwd(_rank, rwd_data, item_list, cell_server::getInstance().get_world_level_last());

		//				const std::string& npc_name = template_manager::get_instance().get_str_by_string_template_id(npc_ref.get_npc_template()->NpcName);
		//				std::string rank_num = init_unit::change_i64_to_string(_rank);
		//				std::vector<std::string> content_params;

		//				content_params.push_back("90303039");
		//				content_params.push_back(npc_name);
		//				content_params.push_back(rank_num);
		//				std::string contenttext = init_unit::implode(content_params);
		//				if (item_list.empty())
		//				{
		//					globle_data::get_instance().send_mail_to_another_player_by_system(
		//						damage_info.unit_identifier.unit_guid, 
		//						guid_64(),
		//						0, 
		//						"90303013", 
		//						"90303037", 
		//						contenttext,
		//						0, 0, 0, 0, 0, 0);
		//				}
		//				else
		//				{
		//					globle_data::get_instance().send_mail_with_item(damage_info.unit_identifier.unit_guid, 0, item_list, "90303037", contenttext);
		//				}
		//		}
		//	}
		//	if (legion_act_rank_msg.member_num > 0)
		//	{
		//		connection_mgr::getInstance().send_to_ws( &legion_act_rank_msg, legion_act_rank_msg.get_pak_length());
		//	} 
		//}
		//break;
		
		default: 
			break;
		}
 	}
	
	void legion_cs_mgr::set_last_convenient_join_legion_time()
	{
		m_last_convenient_join_legion_time = (int64)time(nullptr);
	}
	bool legion_cs_mgr::can_convenient_join_legion()
	{
		int64 cur_time = (int64)time(nullptr);
		if (cur_time - m_last_convenient_join_legion_time > convenient_join_legion_cd)
		{
			return true;
		}

		return false;
	}
	void legion_cs_mgr::c2cs_convenient_join_legion(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		faith::legion_proto_convenient_join_legion convenient_join_legion_msg;
		bool suc = parse_msg::getInstance().parse_message_new(&convenient_join_legion_msg, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);

		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		//跨服状态下不允许
		if (false == player_ref.is_self_server())
		{
			return;
		}
		// 还在CD中
		if (player_ref.get_legion_cs_mgr().can_convenient_join_legion() == false)
		{
			return;
		}
		player_ref.get_legion_cs_mgr().set_last_convenient_join_legion_time();
		

		cs2ws_convenient_join_legion msg;
		msg.role_guid = player_ref.get_unit_guid();
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void legion_cs_mgr::send_to_ws_add_legion_event(std::string show_string)
	{
		cs2ws_add_legion_event msg;
		msg.legion_guid = m_unit_legion_info.legion_guid;
		memcpy(msg.extra_data, show_string.c_str(), show_string.length() >= max_once_legion_event_size ? max_once_legion_event_size : show_string.length());

		connection_mgr::getInstance().send_to_ws( &msg, sizeof(msg));
	}

	void legion_cs_mgr::add_kill_boss_event(std::string member_name, int32 boss_id, const std::vector<int32>& item_id_array)
	{
		if (member_name == "" || boss_id <= 0)
		{
			return;
		}
		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, boss_id);
		if (nullptr == npc_template_ptr)
		{
			return;
		}

		if (npc_template_ptr->NpcType == e_unit_type_summoned)
		{
			return;
		}

		std::string item_str = "";

		for (int32 temp_item_id : item_id_array)
		{
			ItemTemplate* line_ptr = GET_TEMPLATE(ItemTemplate, temp_item_id);
			if (nullptr == line_ptr)
			{
				continue;
			}
			if (4 == line_ptr->item_color || 5 == line_ptr->item_color || 6 == line_ptr->item_color)
			{
				if (1 == line_ptr->item_type || 2 == line_ptr->item_type)
				{
					std::string item_name = "";
					if (!template_manager::get_instance().get_str_by_attribute_id(item_name, line_ptr->ItemName))
					{
						continue;
					}
					item_str += " ";
					item_str += item_name;
				}
			}
		}

		std::vector<std::string> vec_show_str;
		vec_show_str.push_back(template_manager::get_instance().int_to_string(legion_event_kill_boss_str_model_id));
		vec_show_str.push_back(member_name);
		vec_show_str.push_back(template_manager::get_instance().int_to_string(npc_template_ptr->NpcName));
		vec_show_str.push_back(item_str);
		std::string data_str = init_unit::implode(vec_show_str);
		send_to_ws_add_legion_event(data_str);
	}
	bool legion_cs_mgr::is_legion_boss_map(guid_64 map_guid)
	{
		auto map_type = base_map_system::get_map_type(map_guid);
		if (map_type != e_map_type_legion_station) //e_map_type_legion_boss
		{
			return false;
		}

		return true;
	}

	void legion_cs_mgr::c2cs_draw_mem_daily_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		faith::legion_proto_draw_mem_daily_award draw_mem_daily_msg;
		bool suc = parse_msg::getInstance().parse_message_new(&draw_mem_daily_msg, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);

		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		//跨服状态下不允许
		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (false == player_ref.get_legion_guid().is_valid())
		{
			return;
		}

		int32 award_level = player_ref.get_legion_info().legion_construction_level[ELegionInfo_construction_level_3];
		int32 legion_template_id = first_legion_template_id + award_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}
		
		int64 legion_daily_rwd_flag_data = player_ref.get_time_data(e_time_type_legion_daily_award_flags);
		int32 is_got_rwd = init_unit::get_flag_data(legion_daily_rwd_flag_data, e_legion_daily_rwd_flag_daily_everyone);
		if (1 == is_got_rwd)
		{
			// 今天已经领取过就不能再领取了
			legion_proto_legion_error legion_err_msg;
			legion_err_msg.set_error_type(e_legion_error_already_get_occupation_daily_award);
			player_ref.send_message_to_self(&legion_err_msg, e_msgindex_s2c_legion_error);
			return;
		}
		init_unit::mark_flag_data(legion_daily_rwd_flag_data, e_legion_daily_rwd_flag_daily_everyone);
		player_ref.set_time_data(e_time_type_legion_daily_award_flags, legion_daily_rwd_flag_data);
		player_ref.send_time_one(e_time_type_legion_daily_award_flags);
		
		std::vector<int32> rwd_list = legion_template_ptr->MemDailyAwards;
		int32 len = rwd_list.size();
		if (len > 0 && len % 2 == 0)
		{
			item_set& item_set_ref = player_ref.get_item_set();
			item_set_ref.put_in_bag(e_server_log_add_item_legion_draw_mem_daily, 0, rwd_list);
		}
	}
	
	void legion_cs_mgr::c2cs_legion_territory_buff_operate(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		faith::legion_proto_legion_territory_buff_operate territory_buff_operate_msg;
		bool suc = parse_msg::getInstance().parse_message_new(&territory_buff_operate_msg, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);

		player& m_unit_ref = unit_man::get_player(unit_index);
		if (!m_unit_ref.is_valid())
		{
			return;
		}

		//跨服状态下不允许
		if (false == m_unit_ref.is_self_server())
		{
			return;
		}
		if (!m_unit_ref.get_legion_guid().is_valid())
		{
			return;
		}
		legion_cs_mgr& legion_cs_ref = m_unit_ref.get_legion_cs_mgr();

		int32 operate_type = territory_buff_operate_msg.operate_type();
		if (operate_type == 0)
		{
			legion_cs_ref.del_territory_buff(unit_index);
		}
		if (operate_type == 1)
		{
			legion_cs_ref.add_territory_buff(unit_index, territory_buff_operate_msg.territory_id());
		}
		return;
	}

	void legion_cs_mgr::c2cs_goto_next_stage_of_bonfire_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		faith::legion_proto_goto_next_stage_of_bonfire_map goto_next_stage_of_bonfire_map_msg;
		bool suc = parse_msg::getInstance().parse_message_new(&goto_next_stage_of_bonfire_map_msg, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		//跨服状态下不允许
		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (!player_ref.get_legion_guid().is_valid())
		{
			return;
		}

		auto map_type = base_map_system::get_map_type(player_ref.get_map_ent());
		if (map_type != e_map_type_legion_bonfire)
		{
			return;
		}

		//map_object_ptr->goto_next_stage();
	}

	int32 legion_cs_mgr::c2cs_store_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		int32 error_id = e_legion_warehouse_error_none;
		legion_proto_operation_legion_warehouse_item req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		if (!suc)
		{
			return e_legion_unknown_error;
		}
		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (!player_ref.is_valid() || player_ref.get_session_state() != e_session_status_in_gaming)
		{
			return e_legion_unknown_error;
		}
		//跨服状态下不允许
		if (false == player_ref.is_self_server())
		{
			return e_legion_cross_server_error;
		}
		// 检查是否加入军团
		if (!player_ref.get_legion_guid().is_valid())
		{
			return e_legion_not_in_legion_error;
		}
		// 1.检查道具是否存在
		if (req.item_guid_size() != 2)
		{
			return e_legion_unknown_error;
		}
		guid_64 item_guid(req.item_guid(0), req.item_guid(1));
		if (item_guid.is_valid() == false)
		{
			return e_legion_unknown_error;
		}

		citem* item = player_ref.get_item_set().get_item(item_guid);
		if (!item)
		{
			return e_legion_not_has_item_error;
		}
		// 2.要存储的道具类型必须是 武器 首饰 防具 魔法书
		ItemTemplate* item_template_ptr = item->get_item_info_ptr();
		if (item_template_ptr == nullptr || !(
			item_template_ptr->item_type == e_item_type_weapon || 
			item_template_ptr->item_type == e_item_type_armor ))
		{
			return e_legion_warehouse_item_type_error;
		}
		// 3.如果是装备 只能捐献紫色以上品质的装备
		if (item_template_ptr->item_color < e_item_color_purple)
		{
			return e_legion_warehouse_item_equip_quality_error;
		}
		// 4.如果是装备 只能捐赠未绑定的装备
		if (item->get_data_info(e_item_info_container_type) == e_bag_type_equip)
		{
			return e_legion_warehouse_item_equip_equip_on_error;
		}

		// 发送消息给ws
		s_legion_warehouse_info temp_info;
		temp_info.item_info.item_guid = item_guid;
		temp_info.legion_guid = player_ref.get_legion_guid();
		temp_info.add_time = utility::get_tick_count();
		temp_info.item_info.clear_data();
		temp_info.item_info = item->get_item_inst();

		s_legion_warehouse_log log;
		log.role_guid = player_ref.get_unit_guid();
		log.item_guid = item_guid;
		log.legion_guid = player_ref.get_legion_guid();
		log.add_time = utility::get_tick_count();
		log.job = player_ref.get_player_legion_job_title();
		memcpy(log.role_name, player_ref.get_name(), max_name_size);
		log.operation_type = req.operation_type();
		log.item_template_id = item->get_data_info(e_item_info_info_id);

		cs2ws_legion_warehouse_operation msg;
		msg.role_guid = player_ref.get_unit_guid();
		msg.operation_type = req.operation_type();
		msg.legion_warehouse_info = temp_info;
		msg.legion_warehouse_log = log;

		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		return 0;
	}

	int32 legion_cs_mgr::c2cs_role_get_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		int32 error_id = e_legion_warehouse_error_none;
		legion_proto_operation_legion_warehouse_item req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return e_legion_unknown_error;
		}

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (!player_ref.is_valid() || player_ref.get_session_state() != e_session_status_in_gaming)
		{
			return e_legion_unknown_error;
		}
		//跨服状态下不允许
		if (false == player_ref.is_self_server())
		{
			return e_legion_cross_server_error;
		}
		// 检查是否加入军团
		if (!player_ref.get_legion_guid().is_valid())
		{
			return e_legion_not_in_legion_error;
		}
		// 1.检查自己的道具背包是否已满
		if (player_ref.get_item_set().get_empty_slot_num(e_bag_type_bag) <= 0)
		{
			return e_legion_warehouse_equip_bag_id_full;
		}
		// 2.检查自己的军团积分是否足够
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, req.item_id());
		if (item_template_ptr == nullptr)
		{
			return e_legion_warehouse_item_tempelete_none;
		}
		if (!player_ref.can_cut_money(e_money_type_legion_warehouse_score, item_template_ptr->LegionDonateReduceIntegral))
		{
			return e_legion_warehouse_score_not_enough;
		}

		if (req.item_guid_size() != 2)
		{
			return e_legion_unknown_error;
		}
		guid_64 item_guid(req.item_guid(0), req.item_guid(1));
		if (item_guid.is_valid() == false)
		{
			return e_legion_unknown_error;
		}


		// 发送消息给ws
		s_legion_warehouse_info temp_info;
		temp_info.legion_guid = player_ref.get_legion_guid();
		temp_info.add_time = utility::get_tick_count();
		temp_info.item_info.clear_data();
		temp_info.item_info.item_guid = item_guid;

		s_legion_warehouse_log log;
		log.role_guid = player_ref.get_unit_guid();
		log.item_guid = item_guid;
		log.legion_guid = player_ref.get_legion_guid();
		log.add_time = utility::get_tick_count();
		log.job = player_ref.get_player_legion_job_title();
		memcpy(log.role_name, player_ref.get_name(), max_name_size);
		log.operation_type = req.operation_type();
		log.item_template_id = req.item_id();

		cs2ws_legion_warehouse_operation msg;
		msg.role_guid = player_ref.get_unit_guid();
		msg.operation_type = req.operation_type();
		msg.legion_warehouse_info = temp_info;
		msg.legion_warehouse_log = log;

		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));

		return error_id;
	}

	int32 legion_cs_mgr::c2cs_destroy_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		int32 error_id = e_legion_warehouse_error_none;
		legion_proto_operation_legion_warehouse_item req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return e_legion_unknown_error;
		}

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);
		player& player_ref = unit_man::get_player(unit_index);
		if (!player_ref.is_valid() || player_ref.get_session_state() != e_session_status_in_gaming)
		{
			return e_legion_unknown_error;
		}
		//跨服状态下不允许
		if (false == player_ref.is_self_server())
		{
			return e_legion_cross_server_error;
		}
		// 检查是否加入军团
		if (!player_ref.get_legion_guid().is_valid())
		{
			return e_legion_not_in_legion_error;
		}

		// 1.检查自己是否有权限销毁道具
		if (!(player_ref.get_player_legion_job_title() == e_legion_job_title_assistant_chief || player_ref.get_player_legion_job_title() == e_legion_job_title_chief))
		{
			return e_legion_warehouse_lack_of_authority;
		}

		// 发送消息给ws
		s_legion_warehouse_info temp_info;
		temp_info.legion_guid = player_ref.get_legion_guid();
		temp_info.add_time = utility::get_tick_count();
		temp_info.item_info.clear_data();

		s_legion_warehouse_log log;
		log.role_guid = player_ref.get_unit_guid();
		log.legion_guid = player_ref.get_legion_guid();
		log.add_time = utility::get_tick_count();
		log.job = player_ref.get_player_legion_job_title();
		memcpy(log.role_name, player_ref.get_name(), max_name_size);
		log.operation_type = req.operation_type();
		log.item_template_id = 0;

		cs2ws_legion_warehouse_operation msg;
		msg.operation_type = req.operation_type();
		msg.role_guid = player_ref.get_unit_guid();
		msg.legion_warehouse_info = temp_info;
		msg.legion_warehouse_log = log;


		if (req.item_guid_size() % 2 != 0)
		{
			return e_legion_warehouse_none_item_by_destroy;
		}

		int32 del_index = 0;
		for (int32 i = 0; i < req.item_guid_size(); i = i + 2)
		{
			guid_64 item_guid;
			item_guid.A = req.item_guid(0 + i);
			item_guid.B = req.item_guid(1 + i);
			if (item_guid.is_valid() == false)
				continue;
			msg.del_array[del_index] = item_guid;
			del_index++;
		}

		msg.del_num = del_index;

		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));

		return error_id;
	}


	void legion_cs_mgr::ws2cs_set_legion_info(const void* data_ptr)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}
		const ws2cs_set_player_legion_info* packet = static_cast<const ws2cs_set_player_legion_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		
		const s_player_legion_info& player_legion_info = packet->player_legion_info;
		set_legion_info(player_legion_info);
		send_all_legion_skill_level_to_client();//发送自己在当前军团的技能等级

	}

	void legion_cs_mgr::ws2cs_clear_legion_info()
	{
		player& my_unit_ref = unit_man::get_player(m_unit_array_index);
		if (!my_unit_ref.is_valid())
		{
			return;
		}

		if (e_legion_job_title_chief == get_legion_info().job_title)
		{
			set_is_city_master(false);
			change_city_master_stuff();
		}
		set_is_win_city_war_member(false);
		change_city_war_winner_legion_title_and_mount();

		//delete_legion_skill();
		del_territory_buff(m_unit_array_index);
		clear_data();

		auto map_type = base_map_system::get_map_type(my_unit_ref.get_map_ent());

		switch (map_type)
		{
		case e_map_type_legion_boss:
		case e_map_type_city_war:
		case e_map_type_legion_bonfire:
		case e_map_type_legion_station:
			my_unit_ref.transfer_by_template(my_unit_ref.get_unit_info(faith::e_role_info_main_map_id), my_unit_ref.get_main_line_id(), 0, guid_64(), 0);
			break;
		default:
			break;
		}

		legion_proto_clear_player_legion_aoi_info clear_player_legion_aoi_info_msg;
		clear_player_legion_aoi_info_msg.set_role_guid(my_unit_ref.get_unit_guid().server_64);
		my_unit_ref.send_message_to_aoi(&clear_player_legion_aoi_info_msg, e_msgindex_s2c_clear_player_legion_aoi_info);

		if (my_unit_ref.get_assist_fight_mgr().is_in_assist_fight())
		{
			my_unit_ref.get_assist_fight_mgr().cancel_assist_fight();
		}
	}

 	void legion_cs_mgr::ws2cs_create_legion_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
 	{
 		const ws2cs_create_legion* packet = static_cast<const ws2cs_create_legion*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_create_legion))
		{
			return;
		}

		guid_64 legion_guid = packet->legion_guid;
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		s_legion_member_info chief_info = packet->chief_info;
		int32 legion_level = packet->legion_level;
		int32 legion_id = first_legion_template_id + legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_id);
		if (nullptr == legion_template_ptr)
		{
			send_create_legion_result_to_ws(chief_info,legion_guid, e_legion_error_template_error);
			return;
		}

		player& player_ref = unit_man::get_player(packet->chief_info.role_guid);
		if (player_ref.is_valid() == false)
		{
			send_create_legion_result_to_ws(chief_info,legion_guid, e_legion_error_create_legion_invalid_role_state);
			return;
		}

		// 检查角色等级
		if (player_ref.get_unit_info(e_role_info_exp_level) < legion_template_ptr->CreateNeedLevel)
		{
			send_create_legion_result_to_ws(chief_info,legion_guid, e_legion_error_create_legion_no_enough_level);
			return;
		}

		// 检查角色的钱是否足够
		int32 temp_len = legion_template_ptr->CreateNeedMoney.size();
		if (temp_len > 0 && temp_len % 2 == 0)
		{
			for (int32 i = 0; i < temp_len; i += 2)
			{
				int32 need_money_id  = legion_template_ptr->CreateNeedMoney[i];
				int32 need_money_num = legion_template_ptr->CreateNeedMoney[i + 1];
				if (player_ref.can_cut_money((e_money_type)need_money_id, need_money_num) == false)
				{
					send_create_legion_result_to_ws(chief_info,legion_guid, e_legion_error_create_legion_no_enough_money);
					return;
				}
			}

			for (int32 i = 0; i < temp_len; i += 2)
			{
				int32 need_money_id  = legion_template_ptr->CreateNeedMoney[i];
				int32 need_money_num = legion_template_ptr->CreateNeedMoney[i + 1];

				player_ref.cut_money((e_money_type)need_money_id, need_money_num, e_server_log_cut_money_create_legion);
			}
		}
		
		player_ref.get_achievement_mgr().create_legion();
		send_create_legion_result_to_ws(chief_info,legion_guid, e_legion_error_none);
 	}
	void legion_cs_mgr::send_create_legion_result_to_ws(s_legion_member_info& chief_info,guid_64 legion_guid, e_legion_error error_code)
	{
		cs2ws_create_legion_result create_legion_result_msg;
		create_legion_result_msg.legion_guid = legion_guid;
		create_legion_result_msg.error_code = error_code;
		create_legion_result_msg.chief_info = chief_info;
		connection_mgr::getInstance().send_to_ws( &create_legion_result_msg, sizeof(create_legion_result_msg));
	}

	void legion_cs_mgr::ws2cs_legion_donate_money_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_donate_legion_money* packet = static_cast<const ws2cs_donate_legion_money*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_donate_legion_money))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		int32 donate_times = packet->donate_times;
		if (donate_times < 0)
		{
			return;
		}

		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		int32 legion_level = player_ref.get_player_legion_level();
		if (legion_level <= 0)
		{
			return;
		}

		int32 legion_template_id = first_legion_template_id + legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}

		// 获取捐献需要花费的金钱
		if (legion_template_ptr->DonateMoneyCost.size() < 2)
		{
			return;
		}
		int32 donate_cost_money_id = legion_template_ptr->DonateMoneyCost[0];
		int64 donate_cost_money_num = legion_template_ptr->DonateMoneyCost[1] * donate_times;



		// 获取捐献后玩家将会获得的战功总量
		int64 battle_achievement_get = legion_template_ptr->BattleAchievementGet * donate_times;
		
		// 检查玩家是否有足够的钱来捐献
		if (!player_ref.can_cut_money((e_money_type)donate_cost_money_id, donate_cost_money_num))
		{
			return;
		}

		// 如果还能继续加战功就给玩家加上相应的战功值
		int32 donate_earn_by_money = player_ref.get_unit_info(e_role_info_legion_donate_earn_by_money);
		int32 player_vip_level = player_ref.get_vip_level();
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(player_vip_level);
		if (vip_template_ptr == nullptr || vip_template_ptr->LegionDonateLimiteRate.size() != 2)
		{
			return;
		}
		int32 DonateLimit = legion_template_ptr->BattleAchievementGetLimit*vip_template_ptr->LegionDonateLimiteRate[1];
		if (donate_earn_by_money>=DonateLimit)
		{
			battle_achievement_get = 0;
			//std::string notice_str = template_manager::get_instance().get_str_by_string_template_id(90091293);
			//player_ref.send_notice(notice_str);
			//return;//超出上限直接拦截
		}

		int32 valied_donate_times = donate_times;
		int32 battle_ach_get_once = legion_template_ptr->BattleAchievementGet;

		if (battle_achievement_get < 0 || battle_achievement_get + donate_earn_by_money < 0 || battle_achievement_get > DonateLimit)
		{
			return;//执行到这里说明客户端被破解，直接return
		}

		if (donate_earn_by_money + battle_achievement_get > DonateLimit)
		{
			battle_achievement_get = DonateLimit - donate_earn_by_money > 0 ? DonateLimit - donate_earn_by_money : 0;
			//if (battle_ach_get_once > 0)
			//{
			//	valied_donate_times = battle_achievement_get / battle_ach_get_once;
			//	if (valied_donate_times <= 0)
			//	{
			//		valied_donate_times = 1;
			//	}
			//}
		}

		// 获取捐献将会增加的军团资金(超过上限的部分直接扣除，并且不会算到军团资金内)
		int32 donate_get_money_num = legion_template_ptr->DonateMoneyGet * valied_donate_times;

		player_ref.cut_money((e_money_type)donate_cost_money_id, donate_cost_money_num, e_server_log_cut_money_legion_donate);

		if (battle_achievement_get > 0)
		{
			if (player_ref.add_money_or_exp(e_money_type_battle_achievement, battle_achievement_get, e_server_log_add_money_legion_donate_money) == false)
			{
				return;
			}
			player_ref.set_unit_info(e_role_info_legion_donate_earn_by_money, donate_earn_by_money + battle_achievement_get);
			player_ref.send_info_one(e_role_info_legion_donate_earn_by_money);

			if (player_ref.get_unit_info(e_role_info_legion_donate_earn_by_money) >= DonateLimit)
			{
				s_chat_notice_info notice_data;
				player_ref.get_chat_mgr().send_notice_new(legion_donate_notice_id, notice_data);
			}
		}

		int32 m_legion_donate_num =  player_ref.get_unit_info(e_role_info_legion_donate_num);
		player_ref.set_unit_info(e_role_info_legion_donate_num, m_legion_donate_num + donate_times);
		player_ref.refresh_service_goal(e_service_goal_type_legion_donate_num);

		cs2ws_donate_legion_money donate_legion_money_msg;
		donate_legion_money_msg.client_uid = player_ref.get_client_uid();
		donate_legion_money_msg.money_num = donate_get_money_num;
		donate_legion_money_msg.battle_achievement = battle_achievement_get;
		connection_mgr::getInstance().send_to_ws( &donate_legion_money_msg, sizeof(donate_legion_money_msg));

		player_ref.get_mission_mgr().target_check(e_mission_end_type_legion_donate, donate_times);
	}

	void legion_cs_mgr::ws2cs_legion_donate_item_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_donate_legion_item* packet = static_cast<const ws2cs_donate_legion_item*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_donate_legion_item))
		{
			return;
		}
		
		guid_64 role_guid = packet->role_guid;
		int32 donate_item_index = packet->donate_item_index;
		int32 donate_item_num = packet->donate_item_num;
		
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		if (donate_item_num < 0)
		{
			return;
		}

		int32 legion_level = player_ref.get_player_legion_level();
		int32 legion_template_id = first_legion_template_id + legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}

		// 检查表格数据是否正确
		int32 len = legion_template_ptr->DonateItemsID.size();
		if (len < 4 || legion_template_ptr->DonateItemGetMoney.size() != len || legion_template_ptr->DonateItemGetBattleAchievement.size() != len || donate_item_index >= len)
		{
			return;
		}

		// 获取捐献需要耗费的道具
		int32 donate_item_id = legion_template_ptr->DonateItemsID[donate_item_index];


		int32 DonateItemGetBattleOnce = legion_template_ptr->DonateItemGetBattleAchievement[donate_item_index];
		// 获取捐献后玩家将会获得的战功
		int64 battle_achievement_get = DonateItemGetBattleOnce * donate_item_num;
		
		// 检查玩家要捐献的物品是否足够
		item_set& item_set_ref = player_ref.get_item_set();
		if (item_set_ref.get_item_count(e_bag_type_bag, donate_item_id) < donate_item_num)
		{
			return;
		}
		
		// 如果还能继续加战功就给玩家加上相应的战功值
		int32 donate_earn_by_item = player_ref.get_unit_info(e_role_info_legion_donate_earn_by_item);
		int32 player_vip_level = player_ref.get_vip_level();
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(player_vip_level);
		if (vip_template_ptr == nullptr || vip_template_ptr->LegionDonateLimiteRate.size() != 2)
		{
			return;
		}
		int32 DonateLimite = legion_template_ptr->DonateItemGetBattleAchievementLimit*vip_template_ptr->LegionDonateLimiteRate[0];
		if (donate_earn_by_item >= DonateLimite)
		{
			battle_achievement_get = 0;
			//std::string notice_str = template_manager::get_instance().get_str_by_string_template_id(90091293);
			//player_ref.send_notice(notice_str);
			//return;//超出上限直接拦截
		}

		if (battle_achievement_get < 0 || donate_earn_by_item + battle_achievement_get < 0 || battle_achievement_get > DonateLimite)
		{
			return;//执行到这里说明客户端被破解，直接return
		}

		int32 vaild_item_num = donate_item_num;
		int32 DonateGetMoney = legion_template_ptr->DonateItemGetMoney[donate_item_index];
		if (donate_earn_by_item + battle_achievement_get > DonateLimite)
		{
			battle_achievement_get = DonateLimite - donate_earn_by_item > 0 ? DonateLimite - donate_earn_by_item : 0;
		//	if (DonateItemGetBattleOnce > 0)
		//	{
		//		vaild_item_num = battle_achievement_get / DonateItemGetBattleOnce;
		//		if (vaild_item_num == 0)
		//		{
		//			vaild_item_num = 1;
		//		}
		//	}
		}

		// 获取捐献后军团将会获得的军团资金(不能超过上限,并且扣除的就是实际捐赠数量)
		int32 legion_add_money = DonateGetMoney * vaild_item_num;

		item_set_ref.cost_item_by_id_with_lock_states(e_server_log_del_item_legion_donate, 0 , donate_item_id, donate_item_num, 0);
		if (battle_achievement_get > 0)
		{
			if (player_ref.add_money_or_exp(e_money_type_battle_achievement, battle_achievement_get, e_server_log_add_money_legion_donate_item) == false)
			{
				return;
			}
			player_ref.set_unit_info(e_role_info_legion_donate_earn_by_item, donate_earn_by_item + battle_achievement_get);
			player_ref.send_info_one(e_role_info_legion_donate_earn_by_item);

		}

		cs2ws_donate_legion_item donate_legion_item_msg;
		donate_legion_item_msg.client_uid = player_ref.get_client_uid();
		donate_legion_item_msg.item_index = donate_item_index;
		donate_legion_item_msg.item_num = vaild_item_num;
		donate_legion_item_msg.money_num = legion_add_money;
		donate_legion_item_msg.battle_achievement = battle_achievement_get;
		connection_mgr::getInstance().send_to_ws( &donate_legion_item_msg, sizeof(donate_legion_item_msg));

		int32 m_legion_donate_num = player_ref.get_unit_info(e_role_info_legion_donate_num);
		player_ref.set_unit_info(e_role_info_legion_donate_num, m_legion_donate_num + donate_item_num);
		player_ref.refresh_service_goal(e_service_goal_type_legion_donate_num);

		player_ref.get_mission_mgr().target_check(e_mission_end_type_legion_donate, donate_item_num);
	}

	void legion_cs_mgr::ws2cs_enter_legion_boss_map_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_enter_legion_boss_map* packet = static_cast<const ws2cs_enter_legion_boss_map*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_enter_legion_boss_map))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		player_ref.transfer_by_template(packet->boss_map_id, 0, 0, packet->boss_map_guid, 0);
	}

	void legion_cs_mgr::ws2cs_get_legion_boss_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_get_legion_boss_award* packet = static_cast<const ws2cs_get_legion_boss_award*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_get_legion_boss_award))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		int32 boss_map_id = packet->boss_map_id;

		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (player_ref.get_legion_cs_mgr().exist_get_legion_boss_award_log(boss_map_id))
		{
			return;
		}

		int32 legion_level = player_ref.get_player_legion_level();
		int32 legion_template_id = first_legion_template_id + legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}
		int64 award_money_num = 0;
		int32 temp_len = legion_template_ptr->LegionBossGetAwardList.size();
		for (int32 i = 0; i < temp_len; ++i)
		{
			if (legion_template_ptr->LegionBossGetAwardList[i] == boss_map_id && i + 1 < temp_len)
			{
				award_money_num = legion_template_ptr->LegionBossGetAwardList[i + 1];
				break;
			}
		}

		if (player_ref.add_money_or_exp(e_money_type_battle_achievement, award_money_num, e_server_log_add_money_legion_boss) == false)
		{
			return;
		}
		player_ref.get_legion_cs_mgr().add_get_legion_boss_award_log(boss_map_id);

		legion_proto_get_legion_boss_award_end get_legion_boss_award_end_msg;
		get_legion_boss_award_end_msg.set_boss_map_template_id(boss_map_id);
		get_legion_boss_award_end_msg.set_get_battle_achievement(award_money_num);
		player_ref.send_message_to_self(&get_legion_boss_award_end_msg, e_msgindex_s2c_get_legion_boss_award_end);
	}
	void legion_cs_mgr::add_get_legion_boss_award_log(int32 template_id)
	{
		m_get_legion_boss_award_log.push_back(template_id);
	}
	bool legion_cs_mgr::exist_get_legion_boss_award_log(int32 template_id)
	{
		int32 log_size = m_get_legion_boss_award_log.size();
		for (int32 i = 0; i < log_size; ++i)
		{
			if (m_get_legion_boss_award_log[i] == template_id)
			{
				return true;
			}
		}

		return false;
	}
	void legion_cs_mgr::ws2cs_set_get_legion_boss_award_log_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_set_get_legion_boss_award_log* packet = static_cast<const ws2cs_set_get_legion_boss_award_log*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_set_get_legion_boss_award_log))
		{
			return;
		}

		guid_64 role_guid = packet->legion_boss_award_get_log.role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		const s_legion_boss_award_get_log& get_log = packet->legion_boss_award_get_log;
		std::vector<int32>& get_legion_boss_award_log = player_ref.get_legion_cs_mgr().get_get_legion_boss_award_log();
		get_legion_boss_award_log.clear();
		for (int32 i = 0; i < get_log.get_logs_num; ++i)
		{
			get_legion_boss_award_log.push_back(get_log.get_logs[i]);
		}
	}

	void legion_cs_mgr::ws2cs_fire_bonfire_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const ws2cs_fire_bonfire* packet = static_cast<const ws2cs_fire_bonfire*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//if (data_len != sizeof(ws2cs_fire_bonfire))
		//{
		//	return;
		//}

		//guid_64 role_guid = packet->role_guid;
		//player& player_ref = unit_man::get_player(role_guid);
		//if (false == player_ref.is_valid())
		//{
		//	return;
		//}

		//ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_bonfire);
		//if (nullptr == activity_template_ptr || activity_template_ptr->ActivityOpenTime.size() < once_activity_open_time_array_size)
		//{
		//	return;
		//}
		//int32 legion_level = player_ref.get_player_legion_level();
		//int32 legion_template_id = first_legion_template_id + legion_level - 1;
		//LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		//if (nullptr == legion_template_ptr)
		//{
		//	return;
		//}

		//if (player_ref.get_pawn_att().get_game_att(e_unit_game_att_movement) >= e_move_ment_mount_fly)
		//{
		//	return;
		//}
		//s_map_pos temp_pos = player_ref.get_new_map_pos();
		//s_unit_identifier empty_owner;
		//npc* bonfire_npc = world_cs::spawn_npc(player_ref.get_map_guid(), temp_pos, legion_template_ptr->BonfireNpcId, empty_owner);
		//if (nullptr == bonfire_npc)
		//{
		//	return;
		//}
		//NpcTemplate* npc_cfg = bonfire_npc->get_npc_template();
		//if (nullptr == npc_cfg)
		//{
		//	return;
		//}
		//bonfire_npc->set_legion_guid(player_ref.get_legion_guid());
		//bonfire_npc->get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_family);

		//uint32 now_stamp = time_helper::get_cur_time_new().second;
		//uint32 end_stamp = time_helper::get_stamp_by_hour_min(activity_template_ptr->ActivityOpenTime[6], activity_template_ptr->ActivityOpenTime[7]);
		//uint32 bonfire_end_stamp = now_stamp + npc_cfg->LifeTime;
		//int32 life_time_sec = end_stamp - now_stamp;
		//if (life_time_sec < npc_cfg->LifeTime)
		//{
		//	bonfire_npc->set_live_time(init_unit::get_end_time(life_time_sec));
		//	bonfire_end_stamp = end_stamp;
		//}

		//cs2ws_fire_bonfire_ret fire_bonfire_ret_msg;
		//fire_bonfire_ret_msg.role_guid = role_guid;
		//fire_bonfire_ret_msg.bonfire_guid = bonfire_npc->get_unit_guid();
		//fire_bonfire_ret_msg.bonfire_map_guid = bonfire_npc->get_map_guid();
		//fire_bonfire_ret_msg.bonfire_pos = bonfire_npc->get_new_map_pos().unit_location;
		//fire_bonfire_ret_msg.bonfire_end_stamp = bonfire_end_stamp;
		//connection_mgr::getInstance().send_to_ws( &fire_bonfire_ret_msg, sizeof(fire_bonfire_ret_msg));
	}

	void legion_cs_mgr::ws2cs_add_fuel_to_bonfire_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_bonfire_add_fuel* packet = static_cast<const ws2cs_bonfire_add_fuel*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_bonfire_add_fuel))
		{
			return;
		}
		//utility::get_tick_count();
		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_bonfire);
		if (nullptr == activity_template_ptr)
		{
			return;
		}

		// 发到cs处理的时候记得判断pos
		if (player_ref.get_map_guid() != packet->bonfire_map_guid)
		{
			return;
		}
		
		int32 legion_level = player_ref.get_legion_info().legion_level;
		int32 legion_template_id = first_legion_template_id + legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}
		if (legion_template_ptr->BonfireNpcFireBuff.size() < 1)
		{
			return;
		}
		
		int32 cost_jewel = activity_template_ptr->ParamInt2;
		if (!player_ref.can_cut_money(e_money_type_jewel_bind, cost_jewel))
		{
			return;
		}

		int32 next_add_times = packet->add_fuel_times + 1;
		
		std::vector<int32> buff_times_list = activity_template_ptr->ParamIntArr1;
		std::vector<int32> color_buff_list = activity_template_ptr->ParamIntArr2;
		int32 buff_times_list_len = buff_times_list.size();
		int32 buff_list_len = color_buff_list.size();
		if (buff_times_list_len != buff_list_len
			|| buff_times_list_len <= 0
			|| next_add_times > buff_times_list[buff_times_list_len - 1])
		{
			player_ref.send_notice("90203033");//加火到达上限返回提示
			return;
		}
		int32 need_add_color_buff = color_buff_list[0];
		for (int32 idx = 0; idx < buff_times_list_len; idx++)
		{
			if (next_add_times <= buff_times_list[idx])
			{
				need_add_color_buff = color_buff_list[idx];
				break;
			}
		}

		//auto bonfire_map_ptr = world_cs::get_map_by_guid<legion_station_map_cs>(packet->bonfire_map_guid);
		//if (nullptr == bonfire_map_ptr)
		//{
		//	return;
		//}

		//npc& bonfire_npc = unit_man::get_npc(bonfire_map_ptr->get_bonfire_guid());

		//// 篝火火焰颜色
		//buff_man::add_buff_inst(player_ref.m_array_index, bonfire_npc.m_array_index, need_add_color_buff);
		//// 给经验数值的buff
		//int32 need_add_logic_buff = legion_template_ptr->BonfireNpcFireBuff[0];
		//buff_man::add_buff_inst(player_ref.m_array_index, bonfire_npc.m_array_index, need_add_logic_buff);
		//// 加火特效
		//buff_man::add_buff_inst(player_ref.m_array_index, bonfire_npc.m_array_index, activity_template_ptr->ParamInt4);

		//player_ref.cut_money(e_money_type_jewel_bind, cost_jewel, e_server_log_cut_money_legion_bonfire);
		//// 给战功
		//int64 battle_achievement_get = activity_template_ptr->ParamInt3;
		//player_ref.add_money_or_exp(e_money_type_battle_achievement, battle_achievement_get, e_server_log_add_money_legion_add_fuel_to_bonfire);

		//cs2ws_bonfire_add_fuel_ret add_fuel_ret_msg;
		//add_fuel_ret_msg.role_guid = role_guid;
		//connection_mgr::getInstance().send_to_ws( &add_fuel_ret_msg, sizeof(add_fuel_ret_msg));

		legion_proto_add_bonfire_end msg;
		player_ref.send_message_to_self(&msg, e_mgsindex_s2c_legion_add_bonfire_end);
	}
	
	void legion_cs_mgr::ws2cs_legion_rm_recruit_cd_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_legion_remove_recruit_cd* packet = static_cast<const ws2cs_legion_remove_recruit_cd*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_legion_remove_recruit_cd))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		int32 legion_level = player_ref.get_player_legion_level();
		int32 legion_template_id = first_legion_template_id + legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}

		int32 need_cost_jewel = 0;
		if (legion_template_ptr->RemoveRecruitCDCost.size() >= 2)
		{
			need_cost_jewel = legion_template_ptr->RemoveRecruitCDCost[1];
		}
		if (player_ref.can_cut_money(e_money_type_jewel_bind, need_cost_jewel) == false)
		{
			return;
		}
		player_ref.cut_money(e_money_type_jewel_bind, need_cost_jewel, e_server_log_cut_money_legion_rm_recruit_cd);
		
		cs2ws_legion_remove_recruit_cd_ret remove_recruit_cd_ret_msg;
		remove_recruit_cd_ret_msg.role_guid = role_guid;
		connection_mgr::getInstance().send_to_ws( &remove_recruit_cd_ret_msg, sizeof(remove_recruit_cd_ret_msg));
	}
	
	void legion_cs_mgr::ws2cs_draw_bonfire_daily_rwd_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_draw_bonfire_daily_rwd* packet = static_cast<const ws2cs_draw_bonfire_daily_rwd*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_draw_bonfire_daily_rwd))
		{
			return;
		}
		//utility::get_tick_count();
		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_bonfire);
		if (nullptr == activity_template_ptr)
		{
			return;
		}
		// 发到cs处理的时候记得判断pos
		if (player_ref.get_map_guid() != packet->bonfire_map_guid)
		{
			return;
		}

		int64 legion_daily_rwd_flag_data = player_ref.get_time_data(e_time_type_legion_daily_award_flags);
		int32 is_got_rwd = init_unit::get_flag_data(legion_daily_rwd_flag_data, e_legion_daily_rwd_flag_bonfire);
		if (1 == is_got_rwd)
		{
			// 今天已经领取过就不能再领取了
			legion_proto_legion_error legion_err_msg;
			legion_err_msg.set_error_type(e_legion_error_already_get_occupation_daily_award);
			player_ref.send_message_to_self(&legion_err_msg, e_msgindex_s2c_legion_error);
			return;
		}
		init_unit::mark_flag_data(legion_daily_rwd_flag_data, e_legion_daily_rwd_flag_bonfire);
		player_ref.set_time_data(e_time_type_legion_daily_award_flags, legion_daily_rwd_flag_data);
		player_ref.send_time_one(e_time_type_legion_daily_award_flags);
		
		std::vector<int32> rwd_list = activity_template_ptr->ParamIntArr3;
		int32 len = rwd_list.size();
		if (len > 0 && len % 2 == 0)
		{
			item_set& item_set_ref = player_ref.get_item_set();
			item_set_ref.put_in_bag(e_server_log_add_item_legion_draw_bonfire_daily, 0, rwd_list);
		}
	}

	void legion_cs_mgr::lua_ws2cs_create_legion_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		faith::ws2cs_proto::create_legion msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_create_legion packet;

		packet.from_proto(msg);

		ws2cs_create_legion_fun(conn_index, &packet, sizeof(ws2cs_create_legion)); 

	}
	void legion_cs_mgr::lua_ws2cs_legion_donate_money_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		faith::ws2cs_proto::donate_legion_money msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_donate_legion_money packet;

		packet.from_proto(msg);

		ws2cs_legion_donate_money_fun(conn_index, &packet, sizeof(ws2cs_donate_legion_money));
	}
	void legion_cs_mgr::lua_ws2cs_legion_donate_item_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		faith::ws2cs_proto::donate_legion_item msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_donate_legion_item packet;

		packet.from_proto(msg);

		ws2cs_legion_donate_item_fun(conn_index, &packet, sizeof(ws2cs_donate_legion_item));
	}


	void legion_cs_mgr::lua_ws2cs_enter_legion_boss_map_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		faith::ws2cs_proto::donate_legion_item msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_donate_legion_item packet;

		packet.from_proto(msg);

		ws2cs_legion_donate_item_fun(conn_index, &packet, sizeof(ws2cs_donate_legion_item));
	}



	void legion_cs_mgr::lua_ws2cs_get_legion_boss_award_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_get_legion_boss_award_fun, get_legion_boss_award)
	}
	void legion_cs_mgr::lua_ws2cs_set_get_legion_boss_award_log_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_set_get_legion_boss_award_log_fun, set_get_legion_boss_award_log)
	}
	void legion_cs_mgr::lua_ws2cs_fire_bonfire_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		//PROTO_FUNC_TO_PACKET(ws2cs_fire_bonfire_fun, set_get_legion_boss_award_log)
	}
	void legion_cs_mgr::lua_ws2cs_add_fuel_to_bonfire_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_add_fuel_to_bonfire_fun, bonfire_add_fuel)
	}
	void legion_cs_mgr::lua_ws2cs_legion_rm_recruit_cd_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_legion_rm_recruit_cd_fun, legion_remove_recruit_cd)
	}
	void legion_cs_mgr::lua_ws2cs_draw_bonfire_daily_rwd_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_draw_bonfire_daily_rwd_fun, draw_bonfire_daily_rwd)
	}
	void legion_cs_mgr::ws2cs_level_up_legion_skill_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_level_up_legion_skill* packet = static_cast<const ws2cs_level_up_legion_skill*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_level_up_legion_skill))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (!player_ref.get_legion_guid().is_valid())
		{
			return;
		}
		int32 cur_player_level = player_ref.get_unit_info(e_role_info_exp_level);
		legion_cs_mgr& legion_cs_ref = player_ref.get_legion_cs_mgr();

		LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, packet->skill_template_id);
		if (nullptr == legion_skill_ptr)
		{
			return;
		}
		BuffDataTemplate* buff_data_ptr = GET_TEMPLATE(BuffDataTemplate, legion_skill_ptr->Buff);
		if (buff_data_ptr == nullptr)
		{
			return;
		}

		int32 cur_skill_level = buff_man::get_buff_pile_by_template_id(legion_cs_ref.m_unit_array_index, legion_cs_ref.m_unit_array_index, legion_skill_ptr->Buff);
		if (cur_skill_level >= buff_data_ptr->MaxPileDepth)
		{
			return;
		}

		//技能等级上限改为玩家等级
		if (cur_skill_level >= cur_player_level)
		{
			return;
		}

		int32 unlock_contribution_size = legion_skill_ptr->UnlockContribution.size();
		for (int32 i = 0; i < unlock_contribution_size - 1; i += 2)
		{
			if (legion_skill_ptr->UnlockContribution[i] > cur_skill_level)
			{
				break;
			}
			if (cur_skill_level == legion_skill_ptr->UnlockContribution[i])
			{
				if (packet->donate_contribution < legion_skill_ptr->UnlockContribution[i + 1])
				{
					return;
				}
				break;
			}
		}

		//legion_skill_ptr->UpLevelCost1.size() < 3 || 取消战功
		if (legion_skill_ptr->UpLevelCost2.size() < 3)
		{
			return;
		}
		//int32 level_up_need_money1 = legion_skill_ptr->UpLevelCost1[1] + legion_skill_ptr->UpLevelCost1[2] * cur_skill_level;
		int32 level_up_need_money2 = legion_skill_ptr->UpLevelCost2[1] + legion_skill_ptr->UpLevelCost2[2] * cur_skill_level;
		//!player_ref.can_cut_money((e_money_type)legion_skill_ptr->UpLevelCost1[0], level_up_need_money1) || 取消战功消耗
		if (!player_ref.can_cut_money((e_money_type)legion_skill_ptr->UpLevelCost2[0], level_up_need_money2))
		{
			return;
		}
		//player_ref.cut_money((e_money_type)legion_skill_ptr->UpLevelCost1[0], level_up_need_money1, e_server_log_cut_money_legion_skill);
		player_ref.cut_money((e_money_type)legion_skill_ptr->UpLevelCost2[0], level_up_need_money2, e_server_log_cut_money_legion_skill);

		buff_man::add_buff_inst(legion_cs_ref.m_unit_array_index, legion_cs_ref.m_unit_array_index, legion_skill_ptr->Buff);

		legion_cs_ref.send_all_legion_skill_level_to_client();

		legion_proto_level_up_legion_skill_end level_up_legion_skill_end_msg;
		level_up_legion_skill_end_msg.set_skill_id(packet->skill_template_id);
		player_ref.send_message_to_self(&level_up_legion_skill_end_msg, e_msgindex_s2c_level_up_legion_skill_end);
	}

	void legion_cs_mgr::ws2cs_begin_city_war_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_begin_city_war* packet = static_cast<const ws2cs_begin_city_war*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_begin_city_war))
		{
			return;
		}

		//guid_64 city_war_map_guid = packet->city_war_map_guid;
		//auto city_war_map_ptr = world_cs::get_map_by_guid<city_war_map_cs>(city_war_map_guid);
		//if (nullptr == city_war_map_ptr)
		//{
		//	return;
		//}

		//city_war_map_ptr->set_join_war_legions(packet->join_war_legions);
		//city_war_map_ptr->set_cur_territory(packet->cur_terr_id);
		//city_war_map_ptr->set_cross_city_idex(packet->cross_city_idex);
	}

	void legion_cs_mgr::ws2cs_enter_city_war_map_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_enter_city_war_map* packet = static_cast<const ws2cs_enter_city_war_map*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_enter_city_war_map))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		player_ref.transfer_by_template(city_war_map_id, 0, 0, packet->map_guid, packet->war_index);
	}

	void legion_cs_mgr::ws2cs_city_war_operate_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_city_war_operate* packet = static_cast<const ws2cs_city_war_operate*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_city_war_operate))
		{
			return;
		}

		//guid_64 city_war_map_guid = packet->city_war_map_guid;
		//auto city_war_map_ptr = world_cs::get_map_by_guid<city_war_map_cs>(city_war_map_guid);
		//if (nullptr == city_war_map_ptr)
		//{
		//	return;
		//}
		//guid_64 role_guid = packet->role_guid;
		//int32 operate_type = packet->operate_type;
		//guid_64 prop_npc_guid = packet->prop_npc_guid;
		//city_war_map_ptr->city_war_operate(role_guid, operate_type, prop_npc_guid);
	}

	void legion_cs_mgr::ws2cs_get_construction_buff_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const ws2cs_get_construction_buff* packet = static_cast<const ws2cs_get_construction_buff*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//if (data_len != sizeof(ws2cs_get_construction_buff))
		//{
		//	return;
		//}

		//guid_64 role_guid = packet->role_guid;
		//int32 construction_index = packet->construction_index;
		//int32 construction_level = packet->construction_level;
		//int32 legion_template_id = first_legion_template_id + construction_level - 1;
		//LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		//if (nullptr == legion_template_ptr)
		//{
		//	return;
		//}

		//// 获取领取BUFF需要花费的战功
		//int32 get_buff_cost = 0;
		//int32 buff_id = 0;
		//switch ((ELegionInfo)construction_index)
		//{
		//case ELegionInfo_construction_level_main:
		//	if (legion_template_ptr->MainCostructionBuffGetCost.size() >= 2)
		//	{
		//		get_buff_cost = legion_template_ptr->MainCostructionBuffGetCost[1];
		//		buff_id = legion_template_ptr->MainConstructionBuffID;
		//	}
		//	break;
		//case ELegionInfo_construction_level_1:
		//	if (legion_template_ptr->Construction_1_BuffGetCost.size() >= 2)
		//	{
		//		get_buff_cost = legion_template_ptr->Construction_1_BuffGetCost[1];
		//		buff_id = legion_template_ptr->Construction_1_BuffID;
		//	}
		//	break;
		//case ELegionInfo_construction_level_2:
		//	if (legion_template_ptr->Construction_2_BuffGetCost.size() >= 2)
		//	{
		//		get_buff_cost = legion_template_ptr->Construction_2_BuffGetCost[1];
		//		buff_id = legion_template_ptr->Construction_2_BuffID;
		//	}
		//	break;
		//case ELegionInfo_construction_level_3:
		//	if (legion_template_ptr->Construction_3_BuffGetCost.size() >= 2)
		//	{
		//		get_buff_cost = legion_template_ptr->Construction_3_BuffGetCost[1];
		//		buff_id = legion_template_ptr->Construction_3_BuffID;
		//	}
		//	break;
		//}

		//player& player_ref = unit_man::get_player(role_guid);
		//if (false == player_ref.is_valid() || player_ref.is_valid() == false)
		//{
		//	return;
		//}

		//// 看玩家的战功是否足够
		//if (player_ref.get_money_data(e_money_type_battle_achievement) < get_buff_cost)
		//{
		//	return;
		//}
		//std::string cause_id;
		//std::stringstream cause_id_stream;
		//cause_id_stream << int32(e_server_log_cut_money_legion_buff);
		//cause_id_stream >> cause_id;
		//player_ref.cut_money(e_money_type_battle_achievement, get_buff_cost, cause_id);

		//// 给玩家附上军团BUFF
		//int32 player_unit_index = player_ref.get_array_index();
		//buff_man::add_buff_inst(player_unit_index, player_unit_index, buff_id);

		//legion_proto_get_construction_buff_end get_buff_end_msg;
		//get_buff_end_msg.set_construction_index(construction_index);
		//player_ref.send_message_to_self(&get_buff_end_msg, e_msgindex_s2c_legion_get_construction_buff_end);
	}

	void legion_cs_mgr::ws2cs_get_bonus_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_get_bonus_award* packet = static_cast<const ws2cs_get_bonus_award*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_get_bonus_award))
		{
			return;
		}
		guid_64 role_guid = packet->role_guid;
		if (role_guid.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		if (packet->get_all == false)//不是获取全部走的逻辑
		{
			int32 bonus_enum = packet->mission_enum;
			int32 sub_type = packet->sub_type_id;
			LegionSalaryTemplate*salary_temp_ptr = GET_TEMPLATE(LegionSalaryTemplate, first_legion_bonus_template_id + bonus_enum);
			if (salary_temp_ptr == nullptr)
			{
				return;
			}
			std::vector<int32> award_list = salary_temp_ptr->SubAimReward;
			if (award_list.size() != 2 * salary_temp_ptr->SubTypeId.size() || award_list.size() < 2 * sub_type + 1)
			{
				return;
			}
			if (salary_temp_ptr->JobRate.size() < 4)
			{
				return;
			}
			float award_rate = salary_temp_ptr->JobRate[packet->legion_job];
			int32 gold_num = get_bonus_gold_num(role_guid, bonus_enum, sub_type, award_rate);	
			if (gold_num <= 0)
			{
				return;
			}
			std::vector<int32> cur_award_list = { award_list[sub_type * 2], gold_num };
			int32 len = cur_award_list.size();
			if (len > 0 && len % 2 == 0)
			{
				player_ref.add_money_by_template_tuple(cur_award_list);
			}
			legion_proto_get_legion_bonus_award_end	result_msg;
			player_ref.send_message_to_self(&result_msg, e_msgindex_s2c_get_bonus_rewards_end);

			cs2ws_legion_get_bonus_award_end award_msg;
			award_msg.role_guid = role_guid;
			award_msg.glod_num = gold_num;
			connection_mgr::getInstance().send_to_ws( &award_msg, sizeof(award_msg));

			player_ref.get_recycle_mgr().on_event(e_recycle_task_type_legin_reward_10);
			return;
		}
		if (true == packet->get_all)//获取全部走的逻辑
		{
			int32 gold_sum = 0;
			int32 success_sum = 0;
			for (int32 i = 0; i < e_legion_bonus_type_max; i++)
			{
				LegionSalaryTemplate*salary_temp_ptr = GET_TEMPLATE(LegionSalaryTemplate, first_legion_bonus_template_id + i);
				if (salary_temp_ptr == nullptr)
				{
					continue;
				}
				std::vector<int32> need_count_arr = salary_temp_ptr->SubTypeId;
				float award_rate = salary_temp_ptr->JobRate[packet->legion_job];
				for (int32 j = 0; j < salary_temp_ptr->SubTypeId.size(); j++)
				{
					if (need_count_arr[j] <= packet->finish_count[i])
					{
						int32 cur_value = get_bonus_gold_num(role_guid, i, j, award_rate);
						if (cur_value > 0)
						{
							gold_sum += cur_value;
							success_sum++;
						}
						//gold_sum += get_bonus_gold_num(role_guid, i, j, award_rate);
					}
					if (need_count_arr[j] > packet->finish_count[i])
					{
						break;
					}
				}
			}
			if (gold_sum > 0)
			{
				LegionSalaryTemplate*salary_temp_ptr = GET_TEMPLATE(LegionSalaryTemplate, first_legion_bonus_template_id);
				if (salary_temp_ptr == nullptr)
				{
					return;
				}
				std::vector<int32> cur_award_list = { salary_temp_ptr->SubAimReward[0], gold_sum };
				int32 len = cur_award_list.size();
				if (len > 0 && len % 2 == 0)
				{
					player_ref.add_money_by_template_tuple(cur_award_list);
				}
				legion_proto_get_legion_bonus_award_end	result_msg;
				player_ref.send_message_to_self(&result_msg, e_msgindex_s2c_get_bonus_rewards_end);


				cs2ws_legion_get_bonus_award_end award_msg;
				award_msg.role_guid = role_guid;
				award_msg.glod_num = gold_sum;
				connection_mgr::getInstance().send_to_ws( &award_msg, sizeof(award_msg));

				player_ref.get_recycle_mgr().on_event(e_recycle_task_type_legin_reward_10, success_sum);
			}
		}
	}

	void legion_cs_mgr::ws2cs_answer_question_right_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_legion_answer_question_right* packet = static_cast<const ws2cs_legion_answer_question_right*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		PlayerUpgradeTemplate* temp_player_upgrade_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, player_ref.get_unit_info(e_role_info_upgrade_id));
		if (temp_player_upgrade_ptr == nullptr)
		{
			return;
		}

		QuestionBankTemplate* question_bank_template = GET_TEMPLATE(QuestionBankTemplate, packet->question_template_id);
		if (nullptr == question_bank_template)
		{
			return;
		}

		float final_exp = float(temp_player_upgrade_ptr->BaseExp * question_bank_template->ItemRewardArray);
		player_ref.add_exp(final_exp, true);

		player_ref.set_must_do_count(e_daily_must_do_typ_legion_answer, player_ref.get_must_do_count(e_daily_must_do_typ_legion_answer) + 1);
		player_ref.send_daily_must_do_count_info_one(e_daily_must_do_typ_legion_answer);
		player_ref.set_daily_active_degree_info(e_daily_must_do_typ_legion_answer);

		int32 len = question_bank_template->BattleRewardArray.size();
		if (len > 0 && len % 2 == 0)
		{
			player_ref.add_money_by_template_tuple(question_bank_template->BattleRewardArray);
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_answer);
		if (nullptr != config_ptr)
		{
			if (packet->is_answer)
			{
				std::vector<int32> item_arr;

				item_arr.push_back(config_ptr->ParamInt1);
				item_arr.push_back(config_ptr->ParamInt3);
				
				player_ref.get_item_set().put_in_bag(e_server_log_add_item_legion_answer_question_right, 0, item_arr);
			}
			for (int32 i = 0; i < packet->data_num; ++i)
			{
				if (i < max_legion_member_num)
				{
					player& other_player_ref = unit_man::get_player(packet->legion_play_guid[i]);
					if (false == other_player_ref.is_valid())
					{
						continue;
					}
					std::vector<int32> item_arr;

					item_arr.push_back(config_ptr->ParamInt2);
					item_arr.push_back(1);
					other_player_ref.get_item_set().put_in_bag(e_server_log_add_item_legion_answer_question_right, 0, item_arr);
				}
			}
		}
	}

	void legion_cs_mgr::lua_ws2cs_begin_city_war_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_begin_city_war_fun, begin_city_war)
	}

	void legion_cs_mgr::lua_ws2cs_enter_city_war_map_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_enter_city_war_map_fun, enter_city_war_map)
	}

	void legion_cs_mgr::lua_ws2cs_city_war_operate_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_city_war_operate_fun, city_war_operate)
	}

	void legion_cs_mgr::lua_ws2cs_get_occupation_daily_award_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_get_occupation_daily_award_fun, get_occupation_daily_award)
	}

	void legion_cs_mgr::lua_ws2cs_get_city_war_master_stuff_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_get_city_war_master_stuff_fun, get_city_master_stuff)
	}

	void legion_cs_mgr::lua_ws2cs_del_city_war_master_stuff_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_del_city_war_master_stuff_fun, del_city_master_stuff)
	}

	void legion_cs_mgr::lua_ws2cs_add_city_war_winner_stuff_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_add_city_war_winner_stuff_fun, add_city_war_winner_stuff)
	}

	void legion_cs_mgr::lua_ws2cs_del_city_war_winner_stuff_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_del_city_war_winner_stuff_fun, del_city_war_winner_stuff)
	}

	void legion_cs_mgr::lua_ws2cs_get_bonus_award_fun(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_get_bonus_award_fun, get_bonus_award)
	}

	void legion_cs_mgr::lua_ws2cs_answer_question_right_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_answer_question_right_func, legion_answer_question_right)
	}

	void legion_cs_mgr::ws2cs_operation_legion_warehouse_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_legion_warehouse_operation* packet = static_cast<const ws2cs_legion_warehouse_operation*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (packet->error_id > 0) {
			// nothing to do here!!
		}
		else {
			if (packet->operation_type == e_legion_warehouse_operation_store) {
				auto item_ent = get_entity(packet->legion_warehouse_info.item_info.item_guid);
				if (nullptr == item_ent)
				{
					CONSOLE_ERROR("item_ent is nullptr item_guid:{}", packet->legion_warehouse_info.item_info.item_guid.server_64);
					return;
				}
				auto item_base_cp = item_ent->get_component<item_base_component>();
				auto item_template_ptr = item_base_cp->m_item_template;
				int64 LegionDonateAdd = item_template_ptr->LegionDonateAddIntegral;
				// 扣去物品
				item_system::cost_item_by_ent(item_ent);
				// 获得仓库积分
				player_ref.add_money_or_exp(e_money_type_legion_warehouse_score, LegionDonateAdd, e_server_log_add_legion_warehouse_score);
			}
			else if (packet->operation_type == e_legion_warehouse_operation_out) {
				// 添加物品到自己的背包
				s_item_info item_info = packet->legion_warehouse_info.item_info;
				int32 item_template_id = item_info.data_ary[e_item_info_info_id];
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
				if (nullptr == item_template_ptr)
				{
					return;
				}
				if (!player_ref.can_cut_money(e_money_type_legion_warehouse_score, item_template_ptr->LegionDonateReduceIntegral))
				{
					return;
				}
				item_info.data_ary[e_item_info_container_type] = 0;
				item_info.data_ary[e_item_info_slot] = -1;
				item_info.data_ary[e_item_info_locked] = 1;
				citem* item = player_ref.get_item_set().create_item_by_info(item_info, e_server_log_add_item_legion_warehouse, 0, true);
				player_ref.get_item_set().put_in_bag(item);
				// 扣去仓库积分
				player_ref.cut_money(e_money_type_legion_warehouse_score, item_template_ptr->LegionDonateReduceIntegral, e_server_log_cut_money_legion_warehouse_score);
			}
			else if (packet->operation_type == e_legion_warehouse_operation_destroy) {
				// nothing to do here!!
			}
		}
		// 回复消息到前端
		legion_proto_operation_legion_warehouse_item_end resp;
		resp.set_error_code(packet->error_id);
		player_ref.send_message_to_self(&resp, e_msgindex_s2c_operation_legion_warehouse_item_end);
	}

	int32 legion_cs_mgr::get_bonus_gold_num(guid_64 role_guid, int32 bonus_enum, int32 sub_type, float award_rate)
	{
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return 0;
		}
		LegionSalaryTemplate*salary_temp_ptr = GET_TEMPLATE(LegionSalaryTemplate, first_legion_bonus_template_id + bonus_enum);
		if (salary_temp_ptr == nullptr)
		{
			return 0;
		}
		std::vector<int32> award_list = salary_temp_ptr->SubAimReward;
		if (award_list.size() != 2 * salary_temp_ptr->SubTypeId.size() || award_list.size() < 2 * sub_type + 1)
		{
			return 0;
		}
		int64 legion_legion_bonus_award_flag_data = player_ref.get_time_data(e_time_type_legion_bonus_award_flags);
		int32 rwd_flag_idx = (salary_temp_ptr->SubTypeId.size())*bonus_enum + sub_type + 1;
		int32 is_got_rwd = init_unit::get_flag_data(legion_legion_bonus_award_flag_data, rwd_flag_idx);
		if (1 == is_got_rwd)
		{
			return 0;
		}
		init_unit::mark_flag_data(legion_legion_bonus_award_flag_data, rwd_flag_idx);
		player_ref.set_time_data(e_time_type_legion_bonus_award_flags, legion_legion_bonus_award_flag_data);
		player_ref.send_time_one(e_time_type_legion_bonus_award_flags);
		int32 gold_num = (int32)(award_list[sub_type * 2 + 1] * award_rate);
		return gold_num;
	}

	void legion_cs_mgr::ws2cs_get_occupation_daily_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_get_occupation_daily_award* packet = static_cast<const ws2cs_get_occupation_daily_award*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_get_occupation_daily_award))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		if (role_guid.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		int32 territory_id = packet->territory_id;
		CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_cfg)
		{
			return;
		}

		//int64 legion_daily_rwd_flag_data = player_ref.get_time_data(e_time_type_legion_daily_award_flags);
		//int32 rwd_flag_idx = territory_id - overlord_war_territory_flag + 1;

		//int32 is_got_rwd = init_unit::get_flag_data(legion_daily_rwd_flag_data, rwd_flag_idx);
		//if (1 == is_got_rwd)
		//{
		//	// 今天已经领取过就不能再领取了
		//	legion_proto_legion_error legion_err_msg;
		//	legion_err_msg.set_error_type(e_legion_error_already_get_occupation_daily_award);
		//	player_ref.send_message_to_self(&legion_err_msg, e_msgindex_s2c_legion_error);
		//	return;
		//}
		//init_unit::mark_flag_data(legion_daily_rwd_flag_data, rwd_flag_idx);
		//player_ref.set_time_data(e_time_type_legion_daily_award_flags, legion_daily_rwd_flag_data);
		//player_ref.send_time_one(e_time_type_legion_daily_award_flags);

// 		int32 legion_level = player_ref.get_player_legion_level();
// 		int32 legion_template_id = first_legion_template_id + legion_level - 1;
// 		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
// 		if (nullptr == legion_template_ptr)
// 		{
// 			return;
// 		}
		
		std::vector<int32> rwd_list = territory_cfg->DailyRewards;
		int32 len = rwd_list.size();
		if (len > 0 && len % 2 == 0)
		{
			item_set& item_set_ref = player_ref.get_item_set();
			item_set_ref.put_in_bag(e_server_log_add_item_legion_occupation_daily, territory_id, rwd_list);
		}

		legion_proto_get_occupation_daily_award_end result_msg;
		result_msg.set_terr_id(territory_id);
		player_ref.send_message_to_self(&result_msg, e_msgindex_s2c_get_occupation_daily_award_end);
	}
	
	void legion_cs_mgr::ws2cs_get_city_war_master_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(ws2cs_get_city_master_stuff))
		{
			return;
		}
		const ws2cs_get_city_master_stuff* packet = static_cast<const ws2cs_get_city_master_stuff*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		if (role_guid.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (e_legion_job_title_chief == player_ref.m_legion_cs_mgr.get_legion_info().job_title)
		{
			player_ref.get_legion_cs_mgr().set_is_city_master(true);
			player_ref.get_legion_cs_mgr().change_city_master_stuff();
		}
	}
	
	void legion_cs_mgr::ws2cs_del_city_war_master_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(ws2cs_del_city_master_stuff))
		{
			return;
		}
		const ws2cs_del_city_master_stuff* packet = static_cast<const ws2cs_del_city_master_stuff*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		if (role_guid.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.get_legion_cs_mgr().set_is_city_master(false);
		player_ref.get_legion_cs_mgr().change_city_master_stuff();
	}

	void legion_cs_mgr::ws2cs_add_city_war_winner_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(ws2cs_add_city_war_winner_stuff))
		{
			return;
		}
		const ws2cs_add_city_war_winner_stuff* packet = static_cast<const ws2cs_add_city_war_winner_stuff*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		guid_64 role_guid = packet->role_guid;
		if (role_guid.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.get_legion_cs_mgr().set_is_win_city_war_member(true);
		player_ref.get_legion_cs_mgr().change_city_war_winner_legion_title_and_mount();;
	}

	void legion_cs_mgr::ws2cs_del_city_war_winner_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(ws2cs_del_city_war_winner_stuff))
		{
			return;
		}
		const ws2cs_del_city_war_winner_stuff* packet = static_cast<const ws2cs_del_city_war_winner_stuff*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		guid_64 role_guid = packet->role_guid;
		if (role_guid.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.get_legion_cs_mgr().set_is_win_city_war_member(false);
		player_ref.get_legion_cs_mgr().change_city_war_winner_legion_title_and_mount();;
	}

	std::vector<int32> legion_cs_mgr::get_all_legion_skill_level()
	{
		std::vector<int32> skill_level;
		skill_level.clear();
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return skill_level;
		}
		if (!self_ref.get_legion_guid().is_valid())
		{
			return skill_level;
		}
		for (int32 temp_id = first_legion_skill_id; temp_id <= finally_legion_skill_id; temp_id++)
		{
			LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, temp_id);
			if (nullptr == legion_skill_ptr)
			{
				return skill_level;
			}
			BuffDataTemplate* buff_data_ptr = GET_TEMPLATE(BuffDataTemplate, legion_skill_ptr->Buff);
			if (buff_data_ptr == nullptr)
			{
				return skill_level;
			}
			int32 cur_skill_level = buff_man::get_buff_pile_by_template_id(m_unit_array_index, m_unit_array_index, legion_skill_ptr->Buff);
			skill_level.push_back(cur_skill_level);
		}
		return skill_level;
	}

	void legion_cs_mgr::send_all_legion_skill_level_to_client()
	{
		std::vector<int32> skill_level_arr = get_all_legion_skill_level();
		if (skill_level_arr.size() != (finally_legion_skill_id - first_legion_skill_id + 1))
		{
			return;
		}
		legion_proto_get_all_legion_skill_level_end level_msg;
		for (int32 i = 0; i < skill_level_arr.size(); i++)
		{
			level_msg.add_legion_skill_level(skill_level_arr[i]);
		}
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return;
		}
		self_ref.send_message_to_self(&level_msg, e_msgindex_s2c_get_legion_skill_level_end);
	}

	void legion_cs_mgr::delete_legion_skill()
	{
		int32 start_legion_skill_template_id = first_legion_skill_id;
		for (int32 i = 0; i < 20; i++)
		{
			LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, start_legion_skill_template_id + i);
			if (nullptr == legion_skill_ptr)
			{
				break;
			}
			buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, legion_skill_ptr->Buff);
		}
	}


	bool legion_cs_mgr::add_mount_legion_reward(const int32& mount_item_template_id, bool is_force_equip)
	{
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return false;
		}
		item_set& item_set_ref = self_ref.get_item_set();
		citem* reward_mount_ptr = item_set_ref.get_item_by_template_id(mount_item_template_id);
		if (nullptr == reward_mount_ptr)
		{
			return false;
		}
		int32 is_activate = reward_mount_ptr->get_data_info(e_item_info_activate);
		if (is_activate <= 0)
		{
			item_set_ref.set_mount_att(false);
			reward_mount_ptr->set_data_info(e_item_info_activate, 1);
			item_set_ref.set_mount_att(true);
			if (is_force_equip)
			{
				item_set_ref.show_this_mount(reward_mount_ptr, -1);
			}
			self_ref.refresh_service_goal(e_service_goal_type_mount_num);
		}
		return true;
	}
		 
	bool legion_cs_mgr::del_mount_legion_reward(const int32& mount_item_template_id)
	{
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return false;
		}
		item_set& item_set_ref = self_ref.get_item_set();
		citem* mount_ptr = item_set_ref.get_item_by_template_id(mount_item_template_id);
		if (mount_ptr == nullptr)
		{
			return false;
		}
		item_set_ref.set_mount_att(false);
		mount_ptr->set_data_info(e_item_info_activate, 0);
		item_set_ref.set_mount_att(true);
		item_set_ref.clear_quick_mount_call_element(mount_ptr->get_item_guid());			//取消掉快捷召唤
		int32 now_show_mount_id = self_ref.get_unit_info(e_role_info_mount_showd_template_id);//取消显示mount
		if (mount_ptr->get_item_template_id() == now_show_mount_id)
		{
			//切换坐骑
			now_show_mount_id = item_set_ref.get_highest_level_activate_mount_template_id();
			item_set_ref.show_this_mount(item_set_ref.get_item_by_template_id(now_show_mount_id), -1);
		}
		item_set_ref.send_item_one(mount_ptr);
		self_ref.get_passive_skill().passive_skill_special(mount_ptr->get_data_info(e_item_info_succinct_property2), false);
		return true;
	}

	bool legion_cs_mgr::add_wing_legion_reward(const int32& wing_item_template_id, bool is_force_equip)
	{
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return false;
		}

		item_set& item_set_ref = self_ref.get_item_set();
		citem* reward_wing_ptr = item_set_ref.get_item_by_template_id(wing_item_template_id);
		if (nullptr == reward_wing_ptr )
		{
			return false;
		}
		int32 is_activate = reward_wing_ptr->get_data_info(e_item_info_activate);
		if (is_activate <= 0)
		{
			//先全都删掉属性
			item_set_ref.set_wing_att(false);
			//激活	
			reward_wing_ptr->set_data_info(e_item_info_activate, 1);
			//设置属性
			item_set_ref.set_wing_att(true);
			if (is_force_equip)
			{
				self_ref.set_unit_info(e_role_info_wing_showd_template_id, wing_item_template_id);
				self_ref.get_item_set().set_showed_wing();
				item_proto_show_this_wing msg;
				guid_64 role_guid = self_ref.get_unit_guid();
				msg.add_role_guid(role_guid.A);
				msg.add_role_guid(role_guid.B);
				msg.set_wing_template_id(wing_item_template_id);
				self_ref.send_message_to_aoi(&msg, e_msgindex_s2c_show_this_whig);			
			}
		}
		item_set_ref.send_item_one(reward_wing_ptr);
		return true;
	}

	bool legion_cs_mgr::del_wing_legion_reward(const int32& wing_item_template_id)
	{
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return false;
		}
		item_set& item_set_ref = self_ref.get_item_set();
		citem* temp_wing_ptr = item_set_ref.get_item_by_template_id(wing_item_template_id);
		if (nullptr == temp_wing_ptr)
		{
			return false;
		}
		//先全都删掉属性
		item_set_ref.set_wing_att(false);
		//激活	
		temp_wing_ptr->set_data_info(e_item_info_activate, 0);
		//设置属性
		item_set_ref.set_wing_att(true);
		//取消显示wing
		int32 now_show_wing_id = self_ref.get_unit_info(e_role_info_wing_showd_template_id);
		if (city_master_wing_id == now_show_wing_id)
		{
			now_show_wing_id = item_set_ref.get_highest_level_activate_wing_template_id();
			if (now_show_wing_id == 0)
			{
				self_ref.set_unit_info(e_role_info_wing_showd_template_id, 0);
				self_ref.get_item_set().set_showed_wing();
				item_proto_show_this_wing msg;
				guid_64 role_guid = self_ref.get_unit_guid();
				msg.add_role_guid(role_guid.A);
				msg.add_role_guid(role_guid.B);
				msg.set_wing_template_id(0);
				self_ref.send_message_to_aoi(&msg, e_msgindex_s2c_show_this_whig);
			}
			else
			{
				item_set_ref.show_this_wing(item_set_ref.get_item_by_template_id(now_show_wing_id), -1);
			}
		}
		item_set_ref.send_item_one(temp_wing_ptr);
		self_ref.get_passive_skill().passive_skill_special(temp_wing_ptr->get_data_info(e_item_info_succinct_property2), false);
		return true;
	}

	void legion_cs_mgr::add_city_war_winner_buff()
	{
		ActivityCommonConfigTemplate* act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (nullptr == act_com_ptr)
		{
			return;
		}
		int32 buff_id = act_com_ptr->ParamInt2;
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return;
		}
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, buff_id);
	}

	void legion_cs_mgr::del_city_war_winner_buff()
	{
		ActivityCommonConfigTemplate* act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (nullptr == act_com_ptr)
		{
			return;
		}
		int32 buff_id = act_com_ptr->ParamInt2;
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return;
		}
		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, buff_id);
	}

	bool legion_cs_mgr::add_title_legion_reward(const int32& title_template_id, bool is_force_equip)
	{
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return false;
		}
		bool is_have = self_ref.get_title_mgr().is_have_title(title_template_id);
		if (is_have == false)
		{
			self_ref.get_title_mgr().add_title_by_template_id(title_template_id);
		}
		else
		{
			return false;
		}
		if (is_force_equip)
		{
			self_ref.get_title_mgr().equip_on_title(title_template_id);
		}
		return true;
	}

	bool legion_cs_mgr::del_title_legion_reward(const int32& title_template_id)
	{
		player& self_ref = unit_man::get_player(m_unit_array_index);
		if (self_ref.is_valid() == false)
		{
			return false;
		}
		int32 show_title = self_ref.get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
		self_ref.get_title_mgr().remove_title_by_template_id(title_template_id);
		if (show_title == title_template_id)
		{
			show_title = self_ref.get_title_mgr().get_first_active_title_template_id();
			self_ref.get_title_mgr().equip_on_title(show_title);
		}
		return true;
	}

	void legion_cs_mgr::add_city_master_wing()
	{
		add_wing_legion_reward(city_master_wing_id, m_is_city_master);
	}

	void legion_cs_mgr::del_city_master_wing()
	{
		del_wing_legion_reward(city_master_wing_id);
	}
	void legion_cs_mgr::add_city_master_buff()
	{
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, city_master_buff_id);
	}

	void legion_cs_mgr::del_city_master_buff()
	{
		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, city_master_buff_id);
	}

	void legion_cs_mgr::add_city_master_title()
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			add_title_legion_reward(city_master_title_id, m_is_city_master);
			del_title_legion_reward(city_war_cross_master_title_id);
		}
		else
		{
			add_title_legion_reward(city_war_cross_master_title_id, m_is_city_master);
			del_title_legion_reward(city_master_title_id);
		}
	}

	void legion_cs_mgr::del_city_master_title()
	{
		del_title_legion_reward(city_master_title_id);
		del_title_legion_reward(city_war_cross_master_title_id);
	}

	void legion_cs_mgr::add_city_master_mount()
	{
		add_mount_legion_reward(city_master_mount_id, m_is_city_master);
	}

	void legion_cs_mgr::del_city_master_mount()
	{
		del_mount_legion_reward(city_master_mount_id);
	}

	void legion_cs_mgr::change_city_master_stuff()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid())
		{
			if (false == player_ref.is_self_server())
			{
				return;
			}
		}
		if (m_is_city_master)
		{
			add_city_master_wing();
			//add_city_master_mount();
			add_city_master_title();
			add_city_master_buff();
		}
		else
		{
			del_city_master_wing();
			//del_city_master_mount();
			del_city_master_title();
			del_city_master_buff();
		}
	}

	void legion_cs_mgr::add_city_war_winner_title()
	{
		if (!cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			add_title_legion_reward(city_war_winner_title_id, !m_is_city_master);
			del_title_legion_reward(city_war_cross_winner_title_id);
		}
		else
		{
			add_title_legion_reward(city_war_cross_winner_title_id, !m_is_city_master);
			del_title_legion_reward(city_war_winner_title_id);
		}
	}

	void legion_cs_mgr::del_city_war_winner_title()
	{
		del_title_legion_reward(city_war_winner_title_id);
		del_title_legion_reward(city_war_cross_winner_title_id);
	}

	void legion_cs_mgr::add_city_war_winner_mount()
	{
		add_mount_legion_reward(lord_legion_mem_mount_id, !m_is_city_master);
	}

	void legion_cs_mgr::del_city_war_winner_mount()
	{
		del_mount_legion_reward(lord_legion_mem_mount_id);
	}

	void legion_cs_mgr::change_city_war_winner_legion_title_and_mount()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid())
		{
			if (false == player_ref.is_self_server())
			{
				return;
			}
		}
		if (m_is_win_city_war_legion_member)
		{
			add_city_war_winner_title();
			add_city_war_winner_buff();
			//add_city_war_winner_mount();
		}
		else
		{
			del_city_war_winner_title();
			del_city_war_winner_buff();
			//del_city_war_winner_mount();
		}
	}

	void legion_cs_mgr::set_occupied_territorys()
	{
		m_occupied_safe_ids.clear();
		for (int32 terr_idx = 0; terr_idx < max_territory_num; terr_idx++)
		{
			int32 temp_terr_id = m_unit_legion_info.occupied_territorys[terr_idx];
			if (temp_terr_id <= 0)
			{
				break;
			}
			CityWarTerritoryTemplate* city_war_territory_template_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, temp_terr_id);
			if (city_war_territory_template_ptr == nullptr)
			{
				continue;
			}
			int32 contain_zone_id_size = city_war_territory_template_ptr->ContainZoneId.size();
			for (int32 contain_zone_idx = 0; contain_zone_idx < contain_zone_id_size; contain_zone_idx++)
			{
				m_occupied_safe_ids.push_back(city_war_territory_template_ptr->ContainZoneId[contain_zone_idx]);
			}
		}
	}

	int32 legion_cs_mgr::get_cur_legion_territory_safe_id()
	{
		unit& m_unit_ref = unit_man::get_unit(m_unit_array_index);
		if (!m_unit_ref.is_valid())
		{
			return -1;
		}

		int32 safe_size = m_occupied_safe_ids.size();
		if (safe_size <= 0)
		{
			return -1;
		}

		const s_map_block* cur_safe_region = m_unit_ref.get_in_map_safe();
		if (nullptr == cur_safe_region)
		{
			return -1;
		}
		
		for (int32 safe_map_idx = 0; safe_map_idx < safe_size; safe_map_idx++)
		{
			int32 occ_id = m_occupied_safe_ids[safe_map_idx];
			if (occ_id == cur_safe_region->region_id)
			{
				return occ_id;
			}
		}
		return -1;

		//s_map_pos cur_player_pos = m_unit_ref.get_new_map_pos();
		//for (int32 safe_map_idx = 0; safe_map_idx < safe_size; safe_map_idx++)
		//{
		//	MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, m_occupied_safe_ids[safe_map_idx]);
		//	if (map_safe_template_ptr == nullptr)
		//	{
		//		continue;
		//	}
		//	s_map_block tmp_block;
		//	tmp_block.priority = map_safe_template_ptr->Priority;
		//	int32 point_arr_size = map_safe_template_ptr->PointAry.size();
		//	for (int32 j = 0; j + 1 < point_arr_size; j += 2)
		//	{
		//		if (map_safe_template_ptr->PointAry[j] > 0)
		//		{
		//			tmp_block.points.push_back(fvector(map_safe_template_ptr->PointAry[j], map_safe_template_ptr->PointAry[j + 1]));
		//		}
		//	}
		//	if (init_unit::is_in_safe(tmp_block.points, cur_player_pos.unit_location))
		//	{
		//		return m_occupied_safe_ids[safe_map_idx];
		//	}
		//}
		//return -1;
	}

	float legion_cs_mgr::get_cur_territory_exp_fix()
	{
		int32 cur_terr_safe_id = get_cur_legion_territory_safe_id();
		if (cur_terr_safe_id <= 0)
		{
			return 0.0f;
		}

		MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, cur_terr_safe_id);
		if (map_safe_template_ptr == nullptr)
		{
			return 0.0f;
		}

		CityWarTerritoryTemplate* city_war_territory_template_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, map_safe_template_ptr->DependentTerritory);
		if (city_war_territory_template_ptr == nullptr)
		{
			return 0.0f;
		}

		return city_war_territory_template_ptr->TerritoryAddition;
	}

	void	legion_cs_mgr::del_territory_buff(int32 unit_index)
	{
		for (int32 terr_idx = 0; terr_idx < max_territory_num; terr_idx++)
		{
			int32 temp_terr_id = m_unit_legion_info.occupied_territorys[terr_idx];
			if (temp_terr_id <= 0)
			{
				break;
			}
			CityWarTerritoryTemplate* city_war_territory_template_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, temp_terr_id);
			if (city_war_territory_template_ptr == nullptr)
			{
				continue;
			}
			buff_man::del_buff_by_template_id(unit_index, unit_index, city_war_territory_template_ptr->TerritoryBuff);
		}
	}

	void	legion_cs_mgr::add_territory_buff(int32 unit_index, int32 territory_id)
	{
		for (int32 terr_idx = 0; terr_idx < max_territory_num; terr_idx++)
		{
			int32 temp_terr_id = m_unit_legion_info.occupied_territorys[terr_idx];
			if (temp_terr_id <= 0)
			{
				break;
			}
			if (temp_terr_id == territory_id)
			{
				CityWarTerritoryTemplate* city_war_territory_template_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, temp_terr_id);
				if (city_war_territory_template_ptr == nullptr)
				{
					break;
				}
				int32 territory_buff_id = city_war_territory_template_ptr->TerritoryBuff;
				buff_man::add_buff_inst(unit_index, unit_index, territory_buff_id);
			}
		}
	}

	 void legion_cs_mgr::set_legion_skill_data(int64 role_guid, int32 legion_skill_id, int32 curr_level)
	{
		s_legion_skill_info_db legion_skill_info_db ;
		legion_skill_info_db.role_guid = role_guid;
		legion_skill_info_db.legion_skill_info.data_ary[e_legion_skill_info_legion_skill_id] = legion_skill_id;
		legion_skill_info_db.legion_skill_info.data_ary[e_legion_skill_info_curr_level] = curr_level;	
	}

	void	legion_cs_mgr::ws2cs_confirm_change_legion_name_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_confirm_change_legion_name* packet = static_cast<const ws2cs_confirm_change_legion_name*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		if (false == player_ref.get_legion_info().legion_guid.is_valid())
		{
			return;
		}
		//以下写更改军团名要处理了关联信息
		bool is_need_change = packet->is_need_change;
		memset(player_ref.get_legion_info().legion_name, 0, sizeof(player_ref.get_legion_info().legion_name));
		player_ref.get_legion_info().set_legion_name(packet->legion_name);

		memset(player_ref.get_legion_cs_mgr().get_legion_info().legion_name, 0, sizeof(player_ref.get_legion_cs_mgr().get_legion_info().legion_name));
		player_ref.get_legion_cs_mgr().get_legion_info().set_legion_name(packet->legion_name);

		legion_proto_set_player_legion_aoi_info set_player_legion_aoi_info_msg;
		set_player_legion_aoi_info_msg.set_role_guid(player_ref.get_unit_guid().server_64);
		set_player_legion_aoi_info_msg.set_legion_guid(player_ref.get_legion_info().legion_guid.server_64);
		set_player_legion_aoi_info_msg.set_legion_name(player_ref.get_legion_info().legion_name);
		set_player_legion_aoi_info_msg.set_job_title(player_ref.get_legion_info().job_title);
		player_ref.send_message_to_aoi(&set_player_legion_aoi_info_msg, e_msgindex_s2c_set_player_legion_aoi_info);
		//player_ref.send_info_all();

		if (!is_need_change)//只需要执行一次，所以如果guid不等于军团长guid 就不再执行了
		{
			return;
		}
		guid_64 world_boss_guid = cell_server::getInstance().get_world_boss_guid();
		npc& npc_ref = unit_man::get_npc(world_boss_guid);
		if (npc_ref.is_valid())
		{
			npc_ref.npc_change_legion_name_func(player_ref.get_legion_info().legion_guid, packet->legion_name);
		}

		//if (cell_server::getInstance().get_activity_sec_left(e_activity_type_city_war, e_activity_time_get_all) > 0)
		//{
		//	std::vector<city_war_map_cs*> all_city_map;
		//	world_cs::get_all_map_by_template_id<city_war_map_cs>(all_city_map, legion_city_war_map_temp_id);
		//	for (int32 i = 0; i < all_city_map.size(); i++)
		//	{
		//		all_city_map[i]->change_legion_name_func(packet->role_guid, packet->legion_name);
		//	}
		//}
		//if (cell_server::getInstance().get_activity_sec_left(e_activity_type_overlord_city_war, e_activity_time_get_all, false, cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war)) > 0)
		//{
		//	auto city_overlord_war_map_ptr = world_cs::get_map_by_template_id<city_war_map_cs>(legion_city_war_map_temp_id);
		//	if (nullptr != city_overlord_war_map_ptr)
		//	{
		//		city_overlord_war_map_ptr->change_legion_name_func(packet->role_guid, packet->legion_name);
		//	}
		//}
	}

	void	legion_cs_mgr::ws2cs_rem_item_change_legion_name_care_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_rem_item_change_legion_name_care* packet = static_cast<const ws2cs_rem_item_change_legion_name_care*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		uint32 change_legion_name_care = 31140139;
		player_ref.sub_re_legion_name(change_legion_name_care);
	}
	void legion_cs_mgr::ws2cs_change_gate_legion_name_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const ws2cs_change_gate_legion_name* packet = static_cast<const ws2cs_change_gate_legion_name*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (packet->legion_guid.is_valid() ==  false)
		{
			return;
		}
		//if ((cell_server::getInstance().get_activity_sec_left(e_activity_type_city_war_cross_server, e_activity_time_get_all) > 0 && cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) ||
		//	(cell_server::getInstance().get_activity_sec_left(e_activity_type_overlord_city_war, e_activity_time_get_all, false, cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war)) > 0 && cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war)))
		//{
		//	std::vector<city_war_map_cs*> all_city_map;
		//	world_cs::get_all_map_by_template_id<city_war_map_cs>(all_city_map, legion_city_war_map_temp_id);
		//	for (int32 i = 0; i < all_city_map.size(); i++)
		//	{
		//		all_city_map[i]->change_legion_name_func(packet->legion_guid, packet->legion_name);
		//	}
		//}
		//if (cell_server::getInstance().get_activity_sec_left(e_activity_type_cross_server_world_boss, e_activity_time_get_all) > 0 && cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
		//{
		//	ActivityCommonConfigTemplate* cross_server_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		//	if (cross_server_boss_config_ptr != nullptr && cross_server_boss_config_ptr->ParamIntArr4.size() % 4 == 0 && cross_server_boss_config_ptr->ParamIntArr4.size() / 4 == map_max)
		//	{
		//		std::vector<cross_server_world_boss_map_cs*> all_cross_boss_map;
		//		all_cross_boss_map.clear();
		//		for (int32 i = 0; i < map_max; ++i)
		//		{
		//			int32 map_id = cross_server_boss_config_ptr->ParamIntArr4[i * 4];
		//			all_cross_boss_map.push_back(world_cs::get_map_by_template_id<cross_server_world_boss_map_cs>(map_id));
		//		}
		//		for (int32 i = 0; i < all_cross_boss_map.size(); i++)
		//		{
		//			all_cross_boss_map[i]->change_gate_legion_name(packet->legion_guid, packet->legion_name);
		//		}
		//	}
		//}
	}
	void legion_cs_mgr::lua_ws2cs_confirm_change_legion_name_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_confirm_change_legion_name_func, confirm_change_legion_name)
	}
	void legion_cs_mgr::lua_ws2cs_rem_item_change_legion_name_care_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_rem_item_change_legion_name_care_func, rem_item_change_legion_name_care)
	}
	void legion_cs_mgr::lua_ws2cs_change_gate_legion_name_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_change_gate_legion_name_func, change_gate_legion_name)
	}
}
