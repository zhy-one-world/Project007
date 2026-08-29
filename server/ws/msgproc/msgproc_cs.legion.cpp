/********************************************************************
	created:	2017/06/07
	file base:	msgproc_cs.legion
	file ext:	cpp
	author:		Kero
	
	purpose:	cs2ws msg proc for legion
*********************************************************************/
#include "msgproc_cs.hpp"
#include "server/client_session_mgr.hpp"
#include "server/legion/legion_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "template/template_manager.h"
#include "server/activity/cross_server_city_war_ws_mgr.h"
#include "server/ranking/ranking_mgr_ws.h"
#include "server/mail/mail_event_ws.h"
#include "globle_data.h"
#include "net.pb.h"

namespace faith
{
	void cs2ws_req_create_legion_result(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_create_legion_result* packet = static_cast<const cs2ws_create_legion_result*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_create_legion_result))
		{
			return;
		}

		guid_64 legion_guid = packet->legion_guid;
		e_legion_error error_code = packet->error_code;
		s_legion_member_info chief_info = packet->chief_info;
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_mgr_ref.create_legion_end(chief_info,legion_guid, error_code);
	}

	void cs2ws_req_convenient_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_convenient_join_legion* packet = static_cast<const cs2ws_convenient_join_legion*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_convenient_join_legion))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();

		legion_ws_mgr_ref.convenient_join_legion(session);
	}

	void cs2ws_req_donate_legion_money(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_donate_legion_money* packet = static_cast<const cs2ws_donate_legion_money*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_donate_legion_money))
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packet->client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		int32 add_money_num = packet->money_num;					// 增加的军团资金数目
		int32 battle_achievement_get = packet->battle_achievement;	// 玩家因捐献而获得的战功值
		if (add_money_num <= 0)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		
		// 给军团增加相应数额的军团资金
		legion_ws_ptr->add_asset_money(add_money_num);

		// 反馈给客户端获得的战功信息
		//legion_proto_donate_end donate_end_msg;
		//donate_end_msg.set_add_legion_money(add_money_num);
		//donate_end_msg.set_add_battle_achievement(battle_achievement_get);
		//session->send_to_client(&donate_end_msg, e_msgindex_s2c_legion_donate_end);

		// 向军团事件记录器中记录捐献事件
		int32 cur_legion_level = legion_ws_ptr->get_legion_info(ELegionInfo_construction_level_main);
		int32 legion_template_id = first_legion_template_id + cur_legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}
		if (legion_template_ptr->DonateMoneyCost.size() >= 2 && legion_template_ptr->DonateMoneyGet > 0)
		{
			legion_ws_event_logger& event_logger_ref = legion_ws_ptr->get_event_logger();
			int32 donate_money_scale = legion_template_ptr->DonateMoneyCost[1] / legion_template_ptr->DonateMoneyGet;
			event_logger_ref.add_donate_money_event(session->m_role_info.role_name, add_money_num, battle_achievement_get, add_money_num * donate_money_scale);
		}

		//增加捐赠贡献度
		if (legion_template_ptr->GetContributionScale.size() > e_legion_get_contribution_type_donate_money && add_money_num / legion_template_ptr->DonateMoneyGet > 0 && battle_achievement_get > 0)
		{
			int32 contribution_get = legion_template_ptr->GetContributionScale[e_legion_get_contribution_type_donate_money] * add_money_num / legion_template_ptr->DonateMoneyGet;
			legion_ws_ptr->add_member_contribution(session->get_role_guid(), contribution_get);
		}
		// 反馈给客户端获得的战功信息,移到WS上发，避免消息包先后顺序导致的某些bug
		legion_proto_donate_end donate_end_msg;
		donate_end_msg.set_add_legion_money(add_money_num);
		donate_end_msg.set_add_battle_achievement(battle_achievement_get);
		session->send_to_client(&donate_end_msg, e_msgindex_s2c_legion_donate_end);
	}

	void cs2ws_req_donate_legion_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_donate_legion_item* packet = static_cast<const cs2ws_donate_legion_item*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_donate_legion_item))
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packet->client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		int32 add_item_index = packet->item_index;
		int32 add_item_num = packet->item_num;
		int32 add_money_num = packet->money_num;
		int32 battle_achievement_get = packet->battle_achievement;
		if (add_item_num <= 0)
		{
			return;
		}

		int32 real_item_index = ELegionInfo_asset_item_1 + add_item_index;
		if (real_item_index < ELegionInfo_asset_item_1 || real_item_index > ELegionInfo_asset_item_4)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		int32 new_legion_item_num = legion_ws_ptr->get_legion_info((ELegionInfo)real_item_index) + add_item_num;
		legion_ws_ptr->set_legion_info((ELegionInfo)real_item_index, new_legion_item_num);
		legion_ws_ptr->send_legion_attr_one((ELegionInfo)real_item_index);

		legion_ws_ptr->add_asset_money(add_money_num);

		// 向军团事件记录器中记录捐献事件
		int32 cur_legion_level = legion_ws_ptr->get_legion_info(ELegionInfo_construction_level_main);
		int32 legion_template_id = first_legion_template_id + cur_legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr || legion_template_ptr->DonateItemsID.size() <= add_item_index || legion_template_ptr->GetContributionScale.size() <= e_legion_get_contribution_type_donate_item)
		{
			return;
		}
		int32 donate_item_id = legion_template_ptr->DonateItemsID[add_item_index];
		//int32 donate_item_get_money = legion_template_ptr->DonateItemGetMoney[add_item_index];

		legion_ws_event_logger& event_logger_ref = legion_ws_ptr->get_event_logger();
		event_logger_ref.add_donate_item_event(session->m_role_info.role_name, donate_item_id, add_item_num, battle_achievement_get, add_money_num);

		//增加捐赠贡献度
		if (battle_achievement_get > 0)
		{
			int32 contribution_get = legion_template_ptr->GetContributionScale[e_legion_get_contribution_type_donate_item] * add_item_num;
			legion_ws_ptr->add_member_contribution(session->get_role_guid(), contribution_get);
		}

		//增加分红
		s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(e_legion_bonus_type_donate_count);
		bonus_info.finish_count += add_item_num;
		legion_ws_ptr->get_bonus_info_ins().add_legion_bonus_info_map(bonus_info);
			
		// 反馈给客户端获得的战功信息
		legion_proto_donate_end donate_end_msg;
		donate_end_msg.set_add_legion_money(add_money_num);
		donate_end_msg.set_add_battle_achievement(battle_achievement_get);
		session->send_to_client(&donate_end_msg, e_msgindex_s2c_legion_donate_end);
	}

	void cs2ws_req_kill_legion_boss(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_kill_legion_boss* packet = static_cast<const cs2ws_kill_legion_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_kill_legion_boss))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		int32 boss_map_tmpl_id = packet->map_template_id;

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->set_kill_legion_boss_name(packet->killer_name);
		//legion_ws_boss_record_set& legion_boss_record = legion_ws_ptr->get_boss_record_set();
		//legion_boss_record.set_boss_killed(boss_map_tmpl_id, packet->killer_name);
		
		// 向所有的客户端同步击杀军团BOSS的信息
		legion_proto_kill_legion_boss kill_legion_boss_to_client_msg;
		kill_legion_boss_to_client_msg.set_boss_map_template_id(boss_map_tmpl_id);
		kill_legion_boss_to_client_msg.set_killer_name(packet->killer_name);
		legion_ws_ptr->send_message_to_all_member(&kill_legion_boss_to_client_msg, e_msgindex_s2c_kill_legion_boss);
	}

	void cs2ws_req_set_city_occupation(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_set_city_occupation* packet = static_cast<const cs2ws_set_city_occupation*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_set_city_occupation))
		{
			return;
		}
		int32 cross_city_idex = packet->cross_city_idex;
		guid_64 map_guid = packet->territory_map_guid;
		guid_64 legion_guid = packet->occupation_legion_guid;
		guid_64 second_guid = packet->second_legion_guid;
		guid_64 third_guid = packet->third_legion_guid;
		if (cross_city_idex <= 0)
		{
			legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
			city_war_mgr.end_war(map_guid, legion_guid);
		}
		else
		{
			cross_city_idex--;
			cross_server_city_war_ws_mgr::get_instance().end_cross_city_war(map_guid, cross_city_idex, legion_guid, second_guid, third_guid, packet->first_legion_member_all, packet->second_legion_member_all, packet->third_legion_member_all, packet->first_legion_member_num, packet->second_legion_member_num, packet->third_legion_member_num);
		}

	}

	void cs2ws_req_update_legion_member_info_one(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const cs2ws_update_legion_member_info_one* packet = static_cast<const cs2ws_update_legion_member_info_one*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//if (data_len != sizeof(cs2ws_update_legion_member_info_one))
		//{
		//	return;
		//}
	
		//guid_64 role_guid = packet->role_guid;
		//e_legion_member_info info_index = (e_legion_member_info)packet->info_index;
		//int32 info_value = packet->info_value;

		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}
		//s_legion_member_info* member_info = legion_ws_ptr->get_member(role_guid);
		//if (nullptr == member_info)
		//{
		//	return;
		//}
		//legion_ws_ptr->update_member_info_one(role_guid, info_index, info_value);
	}

	void cs2ws_req_fire_bonfire_ret(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const cs2ws_fire_bonfire_ret* packet = static_cast<const cs2ws_fire_bonfire_ret*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//if (data_len != sizeof(cs2ws_fire_bonfire_ret))
		//{
		//	return;
		//}
		//
		//guid_64 role_guid = packet->role_guid;
		//guid_64 bonfire_guid = packet->bonfire_guid;
		//guid_64 bonfire_map_guid = packet->bonfire_map_guid;
		//fvector bonfire_pos = packet->bonfire_pos;
		//uint32 bonfire_end_stamp = packet->bonfire_end_stamp;

		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//legion_ws_ptr->on_create_bonfire_npc(bonfire_guid, bonfire_map_guid, bonfire_pos, bonfire_end_stamp);
	}
	
	void cs2ws_req_add_fuel_to_bonfire_ret(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_bonfire_add_fuel_ret* packet = static_cast<const cs2ws_bonfire_add_fuel_ret*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_bonfire_add_fuel_ret))
		{
			return;
		}
		
		guid_64 role_guid = packet->role_guid;

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		legion_ws_ptr->add_fuel_to_bonfire_succeed(role_guid);
	}
	
	void cs2ws_req_add_legion_glory_glow(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_legion_add_glory_glow* packet = static_cast<const cs2ws_legion_add_glory_glow*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_legion_add_glory_glow))
		{
			return;
		}
		
		guid_64 role_guid = packet->role_guid;
		
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		
		legion_ws_ptr->add_glory_glow(packet->add_gg_value);
		
		int32 cur_legion_level = legion_ws_ptr->get_legion_info(ELegionInfo_construction_level_main);
		int32 legion_template_id = first_legion_template_id + cur_legion_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr || legion_template_ptr->GetContributionScale.size() <= e_legion_get_contribution_type_add_glory_glow)
		{
			return;
		}
		legion_ws_ptr->add_member_contribution(role_guid, packet->add_gg_value);
	}
	
	void cs2ws_req_legion_rm_recruit_cd_ret(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_legion_remove_recruit_cd_ret* packet = static_cast<const cs2ws_legion_remove_recruit_cd_ret*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_legion_remove_recruit_cd_ret))
		{
			return;
		}
		
		guid_64 role_guid = packet->role_guid;
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		
		legion_ws_ptr->set_last_recruit_in_world_stamp(0);
		legion_proto_recruit_in_world_end recruit_in_world_end_msg;
		recruit_in_world_end_msg.set_next_recruit_stamp(0);
		session->send_to_client(&recruit_in_world_end_msg, e_msgindex_s2c_legion_recruit_in_world_end);
	}

	void cs2ws_req_legion_bonfire_dead(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const cs2ws_bonfire_dead* packet = static_cast<const cs2ws_bonfire_dead*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//if (data_len != sizeof(cs2ws_bonfire_dead))
		//{
		//	return;
		//}

		//guid_64 legion_guid = packet->legion_guid;
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		//if (nullptr == legion_ws_ptr)
		//{
		//	return;
		//}

		//legion_ws_ptr->set_bonfire_end_stamp(time_helper::get_cur_time_new().second);

		//legion_proto_bonfire_dead bonfire_dead_msg;
		//bonfire_dead_msg.set_legion_guid(legion_guid.server_64);
		//legion_ws_ptr->send_message_to_all_member(&bonfire_dead_msg, e_msgindex_s2c_bonfire_dead);
	}

	void cs2ws_req_add_legion_event(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_add_legion_event* packet = static_cast<const cs2ws_add_legion_event*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_add_legion_event))
		{
			return;
		}

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(packet->legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		s_legion_event new_event;
		new_event.legion_guid = packet->legion_guid;
		new_event.happen_time = time_helper::get_cur_time_new().second;
		memcpy(new_event.extra_data, packet->extra_data, max_once_legion_event_size);
		legion_ws_ptr->get_event_logger().add_event(new_event);
	}

	void cs2ws_req_get_legion_contribution(uint32 conn_index, const void* data_ptr, size_t data_len) 
	{
		const cs2ws_get_legion_contribution* packet = static_cast<const cs2ws_get_legion_contribution*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		
		legion_ws* legion_ws_ptr = nullptr;
		for (int32 i = 0; i < packet->member_num; i++)
		{
			if (nullptr == legion_ws_ptr)
			{
				legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(packet->member_contribution[i].role_guid);
			}
			if (nullptr == legion_ws_ptr)
			{
				break;
			}
			legion_ws_ptr->add_member_contribution(packet->member_contribution[i].role_guid, packet->member_contribution[i].contribution_get);
		}
	}

	void cs2ws_req_add_legion_act_score(uint32 conn_index, const void* data_ptr, size_t data_len) 
	{
		//屏蔽军团boss和篝火的拍卖行
		if (true)
		{
			return;
		}
		const cs2ws_add_legion_act_score* packet = static_cast<const cs2ws_add_legion_act_score*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(packet->legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->add_act_attend_mems_info(packet->activity_type, packet->member_scores, packet->member_num);
		legion_ws_ptr->set_act_got_score(packet->activity_type, packet->legion_score);
	}

	void cs2ws_req_get_bonus_award_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_legion_get_bonus_award_end* packet = static_cast<const cs2ws_legion_get_bonus_award_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(packet->role_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		if (packet->glod_num <= 0)
		{
			return;
		}
		s_legion_member_info* member_info = legion_ws_ptr->get_member(packet->role_guid);
		if (nullptr == member_info)
		{
			return;
		}
		xstring send_name = member_info->role_name;
		legion_ws_ptr->notice_get_bonus_reward(packet->role_guid, send_name, packet->glod_num);
	}

	void cs2ws_req_add_bonus_count(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_add_bonus_count* packet = static_cast<const cs2ws_add_bonus_count*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(packet->legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(packet->mission_enum);
		bonus_info.finish_count += packet->finish_count;
		legion_ws_ptr->get_bonus_info_ins().add_legion_bonus_info_map(bonus_info);
	}

	void cs2ws_req_settle_legion_act_rank(uint32 conn_index, const void* data_ptr, size_t data_len) 
	{
		const cs2ws_settle_legion_act_rank* packet = static_cast<const cs2ws_settle_legion_act_rank*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		
		e_legion_rank_type rank_type = (e_legion_rank_type)packet->activity_rank_type;
		legion_rank_list* rank_list_ptr = legion_ws_mgr::get_instance().get_legion_rank(rank_type, packet->activity_sub_id);
		if (nullptr == rank_list_ptr)
		{
			return;
		}
		legion_rank_list& rank_list_ref = *rank_list_ptr;

		unit_guid_map _legion_rank_score_all;
		_legion_rank_score_all.clear();
		std::unordered_map<ui64, legion_act_attend_mems_map> _legion_rank_mems_all;
		_legion_rank_mems_all.clear();


		for (int32 idx = 0; idx < packet->member_num; idx++)
		{
			const s_legion_member_attend_activity& mem_score_info = packet->member_scores[idx];
			const guid_64& legion_guid = mem_score_info.mem_legion_guid;
			if (false == legion_guid.is_valid())
			{
				continue;
			}
			legion_act_attend_mems_map& _legion_rank_mems = _legion_rank_mems_all[legion_guid.server_64];
			_legion_rank_mems[mem_score_info.mem_guid] = mem_score_info;

			_legion_rank_score_all[legion_guid.server_64] += mem_score_info.mem_score;

			if (rank_type == e_legion_rank_type_world_boss)
			{
				legion_ws*legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
				if (legion_ws_ptr == nullptr)
				{
					continue;
				}
				s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(e_legion_bonus_world_boss);
				bonus_info.finish_count += 1;
				legion_ws_ptr->get_bonus_info_ins().add_legion_bonus_info_map(bonus_info);
			}
			//if (rank_type == e_legion_rank_type_cross_world_boss)
			//{

			//	legion_ws*legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
			//	if (legion_ws_ptr == nullptr)
			//	{
			//		continue;
			//	}
			//	s_legion_bonus_info& bonus_info = legion_ws_ptr->get_bonus_info_ins().get_bonus_one(e_legion_bonus_cross_world_boss);
			//	bonus_info.finish_count += 1;
			//	legion_ws_ptr->get_bonus_info_ins().add_legion_bonus_info_map(bonus_info);
			//}
		}
		rank_list_ref.clear();
		unit_guid_map_it legion_score_ite = _legion_rank_score_all.begin();
		for (; legion_score_ite != _legion_rank_score_all.end(); ++legion_score_ite)
		{
			legion_ws* score_legion_ptr = legion_ws_mgr::get_instance().get_legion(legion_score_ite->first);
			if (nullptr == score_legion_ptr)
			{
				continue;
			}
			legion_ws_mgr::get_instance().insert_legion_rank_one(rank_list_ref, score_legion_ptr, legion_score_ite->second, rank_type, packet->activity_sub_id);
		

			if (rank_type == e_legion_rank_type_world_elite)
			{
				s_legion_bonus_info& bonus_info = score_legion_ptr->get_bonus_info_ins().get_bonus_one(e_legion_bonus_world_elite);
				bonus_info.finish_count += 1;
				score_legion_ptr->get_bonus_info_ins().add_legion_bonus_info_map(bonus_info);
			}
		}
		legion_ws_mgr::get_instance().send_act_rank_reward(rank_type, packet->activity_sub_id, rank_list_ref, _legion_rank_mems_all);
		legion_ws_mgr::get_instance().send_act_boss_rank_notice(rank_list_ref, packet->activity_sub_id);

		guid_64 special_legion = packet->special_legion_guid;
		legion_ws* special_legion_ptr = legion_ws_mgr::get_instance().get_legion(special_legion);
		if (special_legion.is_valid() && special_legion_ptr)
		{
			legion_ws_mgr::get_instance().send_act_special_reward(rank_type, special_legion_ptr, _legion_rank_mems_all[special_legion.server_64]);
		}
		 
		NpcTemplate* _npc_temp_ptr = GET_TEMPLATE(NpcTemplate, packet->activity_sub_id);
		if (_npc_temp_ptr == nullptr)
		{
			return;
		}
		ActivityCommonConfigTemplate* _act_config_ptr = legion_ws_mgr::get_instance().get_activity_cfg_ptr((e_legion_rank_type)packet->activity_rank_type);
		if (_act_config_ptr == nullptr)
		{
			return;
		}
		int32 _rank = 0;
		legion_rank_list& _rank_list = rank_list_ref;
		for (legion_rank_list::const_iterator ite = _rank_list.begin(); ite != _rank_list.end(); ++ite)
		{
			_rank++;
			legion_act_attend_mems_map _legion_rank_mems = _legion_rank_mems_all[ite->role_guid.server_64];
			legion_act_attend_mems_map::iterator legion_rank_ite = _legion_rank_mems.begin();
			for (; legion_rank_ite != _legion_rank_mems.end(); ++legion_rank_ite)
			{
				std::vector<s_item_template_info> item_list;
				std::vector<int32>& rwd_data = _act_config_ptr->RankRewards;
				const std::string& npc_name = template_manager::get_instance().get_str_by_string_template_id(_npc_temp_ptr->NpcName);
				std::string rank_num = init_unit::change_i64_to_string(legion_rank_ite->second.mem_rank_num);
				std::string legion_rank_num = init_unit::change_i64_to_string(_rank);
				std::vector<std::string> content_params;
				content_params.push_back("90303038");
				content_params.push_back(npc_name);
				content_params.push_back(rank_num);
				content_params.push_back(legion_rank_num);
				std::string contenttext = init_unit::implode(content_params);
				globle_data::get_instance().get_item_list_by_rank_rwd(legion_rank_ite->second.mem_rank_num, rwd_data, item_list, ranking_mgr_ws::get_world_level_last());
				if (!item_list.empty())
				{
					mail_ws_mgr::get_instance().send_mail_system(legion_rank_ite->second.mem_guid, 0, item_list, "90303037", contenttext);
				}
				else
				{
					std::vector<std::string> content_params;
					content_params.push_back("90095253");
					content_params.push_back(npc_name);
					content_params.push_back(rank_num);
					content_params.push_back(legion_rank_num);
					std::string contenttext = init_unit::implode(content_params);
					mail_ws_mgr::get_instance().send_mail_system(legion_rank_ite->second.mem_guid, 0, item_list, "90303037", contenttext);
				}
			}
		}
	}

	void cs2ws_req_apply_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_apply_to_join_legion* packet = static_cast<const cs2ws_apply_to_join_legion*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_apply_to_join_legion))
		{
			return;
		}
		s_client_uid client_uid = packet->client_uid;
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_mgr_ref.join_to_legion(session, packet->legion_guid, true);
	}

	void cs2ws_req_invite_player_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_invite_to_join_legion* packet = static_cast<const cs2ws_invite_to_join_legion*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_invite_to_join_legion))
		{
			return;
		}
		s_client_uid client_uid = packet->client_uid;
		s_client_uid invite_client_uid = packet->invite_client_uid;
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		client_session* new_member_session = nullptr;

		if (true == invite_client_uid.is_valid())
		{
			new_member_session = client_session_mgr::getInstance().get_session(invite_client_uid);
		}
		if (nullptr == new_member_session)
		{
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_player_added_not_online);
			return;
		}
		guid_64 invite_guid = new_member_session->get_role_guid();
		// 如果要添加的玩家已经加入军团就不能再添加他
		if (legion_ws_mgr_ref.get_unit_legion(invite_guid) != nullptr)
		{
			legion_ws_mgr_ref.send_legion_error_to_session(session, e_legion_error_player_added_is_already_join_other_legion);
			return;
		}

		legion_proto_invite_player_end invite_player_to_join_legion_end_msg;
		legion_proto_legion_brief_info* legion_brief_info = invite_player_to_join_legion_end_msg.mutable_invite_legion_info();;
		if (nullptr == legion_brief_info)
		{
			return;
		}
		invite_player_to_join_legion_end_msg.set_invite_role_guid(session->get_role_guid());
		invite_player_to_join_legion_end_msg.set_invite_role_name(session->m_role_info.role_name);
		invite_player_to_join_legion_end_msg.set_invite_role_level(session->m_role_info.data_ary[e_role_info_exp_level]);
		legion_ws_ptr->fill_legion_brief_info_msg(*legion_brief_info);
		new_member_session->send_to_client(&invite_player_to_join_legion_end_msg, e_msgindex_s2c_invite_player_to_join_legion_end);

		legion_proto_invite_player_succeed invite_player_to_join_legion_succeed_msg;
		session->send_to_client(&invite_player_to_join_legion_succeed_msg, e_msgindex_s2c_invite_player_succeed);
	}

	void cs2ws_req_operation_legion_warehouse_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_legion_warehouse_operation* packet = static_cast<const cs2ws_legion_warehouse_operation*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_legion_warehouse_operation))
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(packet->legion_warehouse_info.legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		if(packet->operation_type == e_legion_warehouse_operation_store)
		{
			legion_ws_ptr->get_warehouse().operation_store_legion_warehouse(*packet);
		}
		else if(packet->operation_type == e_legion_warehouse_operation_out)
		{
			legion_ws_ptr->get_warehouse().operation_out_legion_warehouse(*packet);
		}
		else if (packet->operation_type == e_legion_warehouse_operation_destroy)
		{
			legion_ws_ptr->get_warehouse().destroy_item(*packet);
		}
		
	}

	void cs2ws_req_confirm_change_legion_name(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_confirm_change_legion_name* packet = static_cast<const cs2ws_confirm_change_legion_name*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
	
		guid_64	role_guid = packet->role_guid;
		int32 m_legion_error_type = packet->m_legion_error_type;
		if (m_legion_error_type != e_legion_name_error_change_name_succeed)
		{
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(session->get_role_guid());
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->ref_all_about_legion_name(role_guid);

		legion_proto_req_change_legion_name_end msg;
		msg.set_ret(e_legion_name_error_change_name_succeed);
		session->send_to_client(&msg, e_msgindex_s2c_change_legion_name_end);
	}
}