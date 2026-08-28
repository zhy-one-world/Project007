/********************************************************************
created: 2019年03月20日
file base: time_limit_activity_ws_mgr
file ext: cpp
author: wangdi

purpose: time limit activity
*********************************************************************/

#include "time_limit_activity_ws_mgr.h"
#include "Logic/time_def.hpp"
#include "../ranking/ranking_mgr_ws.h"
#include <time_limit_activity_msg.hpp> 
#include "../mail/event_ws_mgr.h"
#include "server/mail/mail_event_ws.h"
#include "utility/init_unit.h"
#include "server_log.hpp"
#include "ws/world_server.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "time_limit_activity_temp_ws_mgr.h"
#include "cloud_shop_mgr.h"
#include "globle_data.h"
#include "time_limit_activity.pb.h"
#include "net.pb.h"

namespace hld
{
	time_limit_activity_ws_mgr::time_limit_activity_ws_mgr()
	{
		clear_data();
	}


	void time_limit_activity_ws_mgr::clear_data()
	{
		m_empty_info.clear_data();
		m_activity_info.clear();
		for (int32 i = 0; i < e_time_limit_activity_type_max; i++)
		{
			s_time_limit_activity_ws_info temp_info;
			m_activity_info[i] = temp_info;
			m_activity_info[i].clear_data();
			m_activity_info[i].set_activity_info(e_time_limit_ws_act_info_type, i);
			switch (i)
			{
			case e_time_limit_activity_type_total_charge:
			{
				m_activity_info[i].ranking_type = e_RankingIndex_time_limit_activity_total_recharge;
			}
			break;
			case e_time_limit_activity_type_cross_total_charge:
			{
				m_activity_info[i].ranking_type = e_RankingIndex_cross_time_limit_activity_total_recharge;
			}
			break;
			case e_time_limit_activity_type_total_cost:
			{
				m_activity_info[i].ranking_type = e_RankingIndex_time_limit_activity_total_cost;
			}
			break;
			case e_time_limit_activity_type_gs_up:
			case e_time_limit_activity_type_mount_up:
			case e_time_limit_activity_type_wing_up:
			case e_time_limit_activity_type_equip_up:
			case e_time_limit_activity_type_spirit_up:
			case e_time_limit_activity_type_cross_gs_up:
			case e_time_limit_activity_type_cross_mount_up:
			case e_time_limit_activity_type_cross_wing_up:
			case e_time_limit_activity_type_cross_equip_up:
			case e_time_limit_activity_type_cross_spirit_up:
			{
				m_activity_info[i].ranking_type = i - e_time_limit_activity_type_gs_up + e_RankingIndex_time_limit_activity_gs_up;
			}
				break;
			default:
				break;
			}
		}
	}

	void time_limit_activity_ws_mgr::init_manager()
	{
		send_load_time_limit_act_to_db();
	}

	void time_limit_activity_ws_mgr::refresh_data(e_time_limit_activity_type activity_type)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}
		int32 base_time = 0;
		int32 first_branch_time_id = 0;
		int32 old_act_guid = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_act_guid);
		int32 new_act_guid = time_limit_activity_temp_ws_mgr::get_instance().get_new_limit_act_temp_with_type(activity_type);
		if (old_act_guid != new_act_guid && old_act_guid > 0)
		{
			clear_activity_by_type((e_time_limit_activity_type)activity_type);
		}
		const s_time_limit_activity_temp& temp_info = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_with_id(new_act_guid);
		int32 new_refresh_time = 0;
		activity_info_ite->second.set_activity_info(e_time_limit_ws_act_info_act_guid, new_act_guid);
		activity_info_ite->second.set_activity_info(e_time_limit_ws_act_info_branch_temp_id, init_unit::get_cur_time_limit_activity_id_by_temp(temp_info, time_limit_activity_temp_ws_mgr::get_instance().get_branch_temp_map(), activity_info_ite->second.open_time
			, new_refresh_time, activity_info_ite->second.cycle_num, activity_info_ite->second.cycle_index, base_time, first_branch_time_id, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time)
			, world_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time), world_server::getInstance().is_sky_island_server()));
		activity_info_ite->second.set_activity_info(e_time_limit_ws_act_info_refresh_time, new_refresh_time);
	}

	int32 time_limit_activity_ws_mgr::get_time_limit_activity_rank_min_config_value(e_time_limit_activity_type activity_type)
	{
		
		s_time_limit_activity_ws_info& cur_total_consume_charge_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_total_consume_charge_activity.is_valid() || !cur_total_consume_charge_activity.is_open())
		{
			return 0;
		}
		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_total_consume_charge_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr1.size() < 1) //huitouzaikankan 
		{
			switch (activity_type)
			{
			case hld::e_time_limit_activity_type_total_charge:
			case hld::e_time_limit_activity_type_cross_total_charge:
				return GAMECONFIG->MinimumRecharge;
				break;
			case hld::e_time_limit_activity_type_total_cost:
				return GAMECONFIG->MinimumConsumption;
				break;
			default:
				break;
			}
			return 0;
		}
		return time_limit_act_branch_temp.ParamArr1[0];
	}

	void time_limit_activity_ws_mgr::tick(const int64& now_time)
	{
		time_info now_tm = time_helper::get_time_by_stamp_new(now_time, true);
		static int32 tick_day_last = 0;
		int32 tick_day_cur = now_tm.year * 1000 + now_tm.day_in_year;
		if (tick_day_cur > tick_day_last)
		{
			tick_day_last = tick_day_cur;
			tick_1_day(now_time);
		}
	}

	void time_limit_activity_ws_mgr::tick_1_day(const int64& now_time)
	{
	}

	void time_limit_activity_ws_mgr::refresh_day_for_zero(const uint64& tick_time)
	{
		int32 cur_time = tick_time / second_tick_time;
		for (auto ite = m_activity_info.begin(); ite != m_activity_info.end(); ++ite)
		{
			if (ite->first == e_time_limit_activity_type_cloud_shop || ite->first == e_time_limit_activity_type_cross_cloud_shop)
			{
				if (get_can_begin_cloud_shop_type_in_this_server() != ite->first)
				{
					continue;
				}
				if (ite->second.get_activity_info(e_time_limit_ws_act_info_flag) <= 0 && ite->second.get_activity_info(e_time_limit_ws_act_info_branch_temp_id) > 0)
				{
					cloud_shop_mgr::get_instance().send_rewards_and_clear_info((e_time_limit_activity_type)ite->first);
				}
				//交换大奖得主名称
				ite->second.set_activity_string_param(ite->second.get_activity_string_param(e_time_limit_activity_string_param_index_1), e_time_limit_activity_string_param_index_2);
				ite->second.clear_activity_string_param(e_time_limit_activity_string_param_index_1);
				ite->second.set_activity_info(e_time_limit_ws_act_info_schedule, 0);
				ite->second.set_activity_info(e_time_limit_ws_act_info_flag, 0);
			}

			if (cur_time >= ite->second.get_activity_info(e_time_limit_ws_act_info_refresh_time) && ite->second.get_activity_info(e_time_limit_ws_act_info_refresh_time) > 0)
			{
				end_act_with_time_limit_activity_type((e_time_limit_activity_type)ite->first);
			}
			else if (ite->second.get_activity_info(e_time_limit_ws_act_info_refresh_time) <= 0)
			{
				clear_activity_by_type((e_time_limit_activity_type)ite->first);
			}

			refresh_data((e_time_limit_activity_type)ite->first);
		}
		save_ws_time_limit_act_to_db();
	}

	void time_limit_activity_ws_mgr::init_all_time_limit_ws()
	{
		//每日刷新有同样的效果 如果每日刷新走过了再走这个
		if (world_server::getInstance().is_need_refresh_ws_daily_info_at_zero_hour())
		{
			return;
		}
		//只在表格改变时刷新数据 重新计算开始和结束时间 只用于启服后和reloadcsv调用一次
		for (int32 i = 0; i < e_time_limit_activity_type_max; i++)
		{
			refresh_data((e_time_limit_activity_type)i);
		}
		save_ws_time_limit_act_to_db();
	}

	void time_limit_activity_ws_mgr::get_rank_info(e_time_limit_activity_type activity_type, guid_64 role_uid, int32 source_server_id)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}
		s_ranking_player_info self_info;
		e_RankingIndex ranking_type = e_RankingIndex_max;

		ranking_type = (e_RankingIndex)activity_info_ite->second.ranking_type;

		if (source_server_id == 0 && init_unit::is_rank_type_cross_server(ranking_type) && world_server::getInstance().get_server_id() != world_server::getInstance().get_cross_id())
		{
			int32 cross_group_ip = world_server::getInstance().get_server_id();
			int32 server_id = world_server::getInstance().get_cross_id();
			ws2gate_time_limit_get_rank_info msg;
			msg.activity_type = activity_type;
			msg.server_id = cross_group_ip;
			msg.role_guid = role_uid;
			cross::send_msg_to_ws(role_uid, server_id, e_msgindex_ws2ws_req_time_limit_rank_info, &msg, sizeof(msg));
			return;
		}

		int32 ranking_level = -1;
		int64 ranking_value = 0;
		s_ranking_player_info* cur_ranking_player_info_ptr = ranking_mgr_ws::get_ranking_player_info_by_type(ranking_type, role_uid);
		if (nullptr != cur_ranking_player_info_ptr)
		{
			if (cur_ranking_player_info_ptr->ranking_value >= get_time_limit_activity_rank_min_config_value(activity_type))
			{
				ranking_mgr_ws::get_player_rank_level(ranking_type, role_uid, ranking_level, ranking_value);
			}
			self_info = *cur_ranking_player_info_ptr;
		}

		std::vector<s_ranking_player_info> top_info_arr;
		ranking_list* ranking_ptr = ranking_mgr_ws::get_ranking_list_by_type(ranking_type);
		if (ranking_ptr != nullptr)
		{
			int32 Index = 0;
			ranking_list_ite ite = ranking_ptr->begin();
			for (; ite != ranking_ptr->end() && Index < 50; ++ite, ++Index)
			{
				top_info_arr.push_back(*ite);
			}
		}

		time_limit_activity_proto_rank_type_activity_message msg;
		msg.set_activity_type(activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_act_guid));
		msg.set_rank_value(ranking_level);
		for (int32 i = 0;i < top_info_arr.size();i++)
		{
			ranking_proto_ranking_player_info* temp_proto_info = msg.add_top_info_arr();
			ranking_mgr_ws::make_ranking_data(temp_proto_info, top_info_arr[i]);
		}
		ranking_proto_ranking_player_info* self_proto_info = msg.add_self_info_arr();
		ranking_mgr_ws::make_ranking_data(self_proto_info, self_info);
		
		if (source_server_id == 0)
		{
			client_session* session = client_session_mgr::getInstance().get_session(role_uid);
			if (nullptr == session)
			{
				return;
			}
			session->send_to_client(&msg, e_msgindex_s2c_time_limit_update_sub_rank);
		}
		else
		{
			cross::send_msg_to_ws(role_uid, source_server_id, e_msgindex_ws2ws_res_time_limit_rank_info, &msg);
		}
	}

	void time_limit_activity_ws_mgr::send_activity_rank_rewards(e_time_limit_activity_type activity_type)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}

		std::string mail_title = "";
		std::string mail_context = "";
		switch (activity_type)
		{
		case hld::e_time_limit_activity_type_total_charge:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_total_recharge_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_total_recharge_context);
		}
		break;
		case hld::e_time_limit_activity_type_cross_total_charge:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_total_recharge_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_total_recharge_context);
		}
		break;
		case hld::e_time_limit_activity_type_total_cost:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_total_cost_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_total_cost_context);
		}
		break;
		case hld::e_time_limit_activity_type_gs_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_gs_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_gs_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_mount_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_mount_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_mount_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_wing_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_wing_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_wing_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_equip_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_equip_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_equip_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_spirit_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_spirit_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_spirit_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_cross_gs_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_cross_mount_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_cross_wing_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_cross_equip_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_context);
		}
		break;
		case hld::e_time_limit_activity_type_cross_spirit_up:
		{
			mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_title);
			mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_context);
		}
		break;

		default:
			break;
		}

		ranking_list* cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type((e_RankingIndex)activity_info_ite->second.ranking_type);
		if (nullptr == cur_ranking_list_ptr)
		{
			return;
		}

		for (auto iter = cur_ranking_list_ptr->begin(); iter != cur_ranking_list_ptr->end(); ++iter)
		{
			CONSOLE_INFO("send_activity_rank_rewards  rank_type:{} role_guid:{} rank_value:{} peak:{}", (int32)activity_info_ite->second.ranking_type, iter->role_guid.server_64, iter->ranking_value, iter->Peak);
		}

		int32 min_value = get_time_limit_activity_rank_min_config_value(activity_type);

		const s_time_limit_activity_temp& activity_template_ptr = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_with_id(activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_act_guid));
		if (!activity_template_ptr.is_valid())
		{
			return;
		}

		//计算出下个周期id 保证在两个周期之间 如果当前周期为最后一个周期 则为0
		int32 next_cycle_first_template_id = 0;
		for (int32 i = 0; i + 1 < activity_template_ptr.CycleFirstId.size(); i++)
		{
			if (activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_branch_temp_id) == activity_template_ptr.CycleFirstId[i])
			{
				next_cycle_first_template_id = activity_template_ptr.CycleFirstId[i + 1];
			}
		}

		//最多50个循环
		ranking_list_ite cur_list_ite = cur_ranking_list_ptr->begin();
		for (int32 i = 0; i < 50; i++)
		{
			int32 temp_activity_branch_template_id = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_branch_temp_id) + i;
			if (next_cycle_first_template_id > 0 && temp_activity_branch_template_id >= next_cycle_first_template_id)
			{
				break;
			}
			const s_time_limit_activity_branch_temp&  temp_activity_branch_template_ptr = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(temp_activity_branch_template_id);
			if (temp_activity_branch_template_ptr.ActGuid <= 0 || temp_activity_branch_template_ptr.ActivityType != activity_template_ptr.ActivityType
				|| temp_activity_branch_template_ptr.Condition.size() < 2 || temp_activity_branch_template_ptr.Condition[0] > temp_activity_branch_template_ptr.Condition[1]
				|| temp_activity_branch_template_ptr.Reward.size() <= 0 || temp_activity_branch_template_ptr.Reward.size() % 2 != 0)
			{
				break;
			}

			bool is_end = false;
			for (int32 j = temp_activity_branch_template_ptr.Condition[0]; j <= temp_activity_branch_template_ptr.Condition[1]; j++)
			{
				if (cur_list_ite == cur_ranking_list_ptr->end() || cur_list_ite->ranking_value < min_value)
				{
					is_end = true;
					break;
				}
				std::vector<s_item_template_info> reward_item_list;
				for (int32 k = 0; k * 2 + 1 < temp_activity_branch_template_ptr.Reward.size(); k++)
				{
					reward_item_list.push_back(s_item_template_info(temp_activity_branch_template_ptr.Reward[k * 2], temp_activity_branch_template_ptr.Reward[k * 2 + 1], 1));
				}
				if (reward_item_list.size() > 0)
				{
					std::vector<string> content_params;
					if (activity_type >= e_time_limit_activity_type_gs_up && activity_type <= e_time_limit_activity_type_cross_spirit_up)
					{
						content_params.push_back(mail_context);
						int32 Index = 0;
						ranking_list_ite ite = cur_ranking_list_ptr->begin();
						for (; ite != cur_ranking_list_ptr->end() && Index < 3; ++ite, ++Index)
						{
							content_params.push_back(ite->role_name);
							content_params.push_back(init_unit::change_i32_to_string(ite->Peak));
						}
						for (;Index < 3;Index++)
						{
							const std::string& unknown_name_str = template_manager::get_instance().get_str_by_string_template_id(90200229);
							content_params.push_back(unknown_name_str);
							content_params.push_back(init_unit::change_i32_to_string(0));
						}
						content_params.push_back(init_unit::change_i32_to_string(j));
						content_params.push_back(init_unit::change_i32_to_string(cur_list_ite->ranking_value - cur_list_ite->Peak));
						content_params.push_back(init_unit::change_i32_to_string(cur_list_ite->Peak));
					}
					else
					{
						content_params.push_back(mail_context);
						content_params.push_back(init_unit::change_i32_to_string(j));
					}
					std::string temp_mail_context = init_unit::implode(content_params);
					if ((activity_type >= e_time_limit_activity_type_gs_up && activity_type <= e_time_limit_activity_type_cross_spirit_up) || activity_type == e_time_limit_activity_type_cross_total_charge)
					{
						mail_ws_mgr::get_instance().send_mail_system(cur_list_ite->role_guid, cur_list_ite->server_id, reward_item_list, mail_title, temp_mail_context);
					}
					else
					{
						mail_ws_mgr::get_instance().send_mail_system(cur_list_ite->role_guid, 0, reward_item_list, mail_title, temp_mail_context);
					}

					for (int32 k = 0 ; k < reward_item_list.size(); ++k)
					{
						s_item_template_info log_item_info = reward_item_list[k];
						s_unit_info log_role_info;
						log_role_info.role_guid = cur_list_ite->role_guid;
						server_log::time_limit_activity_log(log_role_info, login_fixed_data(), temp_activity_branch_template_id, activity_type, cur_list_ite->ranking_value, cur_list_ite->Peak, log_item_info.m_item_id, log_item_info.m_item_num, 0,0,0, temp_activity_branch_template_ptr.AttributeId);
					}
				}
				cur_list_ite++;
			}

			if (is_end)
			{
				break;
			}
		}
	}

	void time_limit_activity_ws_mgr::update_activity_common_data(int32 activity_type, int32 data_num, std::vector<int32> condition_list)
	{
		ws2ws_time_limit_activity_common_data_sync msg;
		msg.activity_type = activity_type;
		msg.data_num = data_num;
		msg.condition_num = 0;
		for (int32 i = 0 ; i < condition_list.size(); i++)
		{
			if (i >= time_limit_activity_schedule_num)
			{
				break;
			}
			msg.condition_list[i] = condition_list[i];
			msg.condition_num++;
		}

		int32 server_id_ = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), server_id_, e_msg_index_ws2ws_time_limit_activity_common_data_sync, &msg, sizeof(msg));
	}

	void time_limit_activity_ws_mgr::update_activity_common_data_on_gate(int32 activity_type, int32 data_num, std::vector<int32> condition_list)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}
		int32 cur_schedule = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule);
		activity_info_ite->second.set_activity_info(e_time_limit_ws_act_info_schedule, cur_schedule + data_num);
		save_ws_time_limit_act_to_db(activity_type);

		for (int32 i = 0; i < condition_list.size(); i++)
		{
			if (cur_schedule < condition_list[i] && activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule) >= condition_list[i])
			{
				send_activity_schedule_to_other_ws((e_time_limit_activity_type)activity_type);
				break;
			}
		}
	}
	void time_limit_activity_ws_mgr::update_activity_common_data_on_local(int32 activity_type, int32 data_num, std::vector<int32> condition_list)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}
		int32 cur_schedule = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule);
		activity_info_ite->second.set_activity_info(e_time_limit_ws_act_info_schedule, cur_schedule + data_num);
		save_ws_time_limit_act_to_db(activity_type);
		for (int32 i = 0; i < condition_list.size(); i++)
		{
			vector<int32> data_type_list;
			vector<int32> data_num_list;
			data_type_list.push_back(activity_type);
			data_num_list.push_back(activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule));
			time_limit_activity_ws_mgr::get_instance().send_activity_common_data(data_type_list, data_num_list);
			break;
		}
	}

	void time_limit_activity_ws_mgr::send_activity_common_data(std::vector<int32> data_type_list, std::vector<int32> data_num_list)
	{
		hld::time_limit_activity_proto_all_common_data_message msg;
		int32 list_size = data_type_list.size();
		for (int32 i = 0; i < list_size; i++)
		{
			if (i >= data_num_list.size())
			{
				break;
			}
			hld::time_limit_activity_proto_common_data_message *common_data = msg.add_data_list();
			common_data->set_data_id(data_type_list[i]);
			common_data->set_data_num(data_num_list[i]);
		}
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_time_limit_update_common_data);
	}

	void time_limit_activity_ws_mgr::check_common_data(int32 activity_id, guid_64 user_guid, int32 activity_type, int32 check_num)
	{
		if (activity_type == e_time_limit_activity_type_national_treasure_4)
		{
			auto activity_info_ite = m_activity_info.find(activity_type);
			if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
			{
				return;
			}
			bool is_achieve = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule) >= check_num;
			time_limit_activity_ws_mgr::get_instance().check_common_data_end(activity_id,user_guid, is_achieve);
		}
		else
		{
			ws2ws_check_time_limit_activity_common_data msg;
			msg.server_id = world_server::getInstance().get_server_id();
			msg.user_guid = user_guid;
			msg.activity_type = activity_type;
			msg.check_data = check_num;
			msg.activity_id = activity_id;
			int32 server_id = world_server::getInstance().get_cross_id();
			cross::send_msg_to_ws(guid_64(), server_id, e_msg_index_ws2ws_check_time_limit_activity_common_data, &msg, sizeof(msg));
		}
	}

	void time_limit_activity_ws_mgr::check_common_data_on_gate(int32 server_id, int32 activity_id, guid_64 user_guid, int32 activity_type, int32 check_num)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}
		ws2ws_check_time_limit_activity_common_data_end resp;
		bool is_achieve = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule) >= check_num;
		resp.user_guid = user_guid;
		resp.is_achieve = is_achieve;
		resp.activity_id = activity_id;
		cross::send_msg_to_ws(guid_64(), server_id, e_msg_index_ws2ws_check_time_limit_activity_common_data_end, &resp, sizeof(resp));
	}
	void time_limit_activity_ws_mgr::check_common_data_end(int32 activity_id, guid_64 user_guid,bool is_achieve)
	{
		client_session* cur_session = client_session_mgr::getInstance().get_session(user_guid);
		if (nullptr != cur_session)
		{
			ws2cs_check_time_limit_activity_common_data resp;
			resp.user_guid = user_guid;
			resp.is_achieve = is_achieve;
			resp.activity_id = activity_id;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_check_time_limit_activity_common_data);
			if (is_use == false)
			{
				cur_session->send_to_cs(&resp, sizeof(ws2cs_check_time_limit_activity_common_data));
			}
			else
			{
				hld::ws2cs_proto::check_time_limit_activity_common_data pro_msg;
				resp.to_proto(pro_msg);
				cur_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_check_time_limit_activity_common_data);
			}
		}
	}

	void time_limit_activity_ws_mgr::get_activity_common_data_to_gate(client_session* session_ptr)
	{
		if (nullptr == session_ptr)
		{
			return;
		}
		if (world_server::getInstance().get_cross_id() > 0)
		{
			ws2ws_get_time_limit_activity_common_data_to_gate msg;
			msg.role_guid = session_ptr->get_role_guid();
			msg.server_id = world_server::getInstance().get_server_id();
			int32 server_id = world_server::getInstance().get_cross_id();
			cross::send_msg_to_ws(guid_64(), server_id, e_msg_index_ws2ws_get_time_limit_activity_common_data_to_gate, &msg, sizeof(msg));
		}
		auto activity_info_ite = m_activity_info.find(e_time_limit_activity_type_national_treasure_4);
		if (activity_info_ite == m_activity_info.end())
		{
			return;
		}
		vector<int32> data_type_list;
		vector<int32> data_num_list;
		data_type_list.push_back(e_time_limit_activity_type_national_treasure_4);
		data_num_list.push_back(activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule));
		time_limit_activity_ws_mgr::get_instance().send_activity_common_data_one(session_ptr->get_role_guid(), data_type_list, data_num_list);
	}

	void time_limit_activity_ws_mgr::get_activity_common_data_on_gate(int32 server_id, guid_64 role_guid)
	{
		auto activity_info_ite = m_activity_info.find(e_time_limit_activity_type_national_treasure_2);
		if (activity_info_ite == m_activity_info.end())
		{
			return;
		}
		ws2ws_get_time_limit_activity_common_data_to_ws msg;
		msg.activity_type = e_time_limit_activity_type_national_treasure_2;
		msg.activity_schedule = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule);
		msg.role_guid = role_guid;
		cross::send_msg_to_ws(guid_64(), server_id, e_msg_index_ws2ws_get_time_limit_activity_common_data_to_ws, &msg, sizeof(msg));
	}

	void time_limit_activity_ws_mgr::send_activity_common_data_one(guid_64 role_guid, std::vector<int32> data_type_list, std::vector<int32> data_num_list)
	{
		client_session *session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		hld::time_limit_activity_proto_all_common_data_message msg;

		for (int32 idx = 0 ; idx < data_type_list.size();idx++)
		{
			if (idx >= data_num_list.size())
			{
				break;
			}
			hld::time_limit_activity_proto_common_data_message *common_data = msg.add_data_list();
			common_data->set_data_id(data_type_list[idx]);
			common_data->set_data_num(data_num_list[idx]);
		}
		session_ptr->send_to_client(&msg, e_msgindex_s2c_time_limit_update_common_data);
	}

	void time_limit_activity_ws_mgr::end_act_with_gm(int32 activity_type)
	{
		if (activity_type >= e_time_limit_activity_type_max)
		{
			return;
		}
		if (activity_type == -1)//全清
		{
			for (auto ite = m_activity_info.begin(); ite != m_activity_info.end(); ++ite)
			{
				if (ite->second.get_activity_info(e_time_limit_ws_act_info_refresh_time) > 0)
				{
					end_act_with_time_limit_activity_type((e_time_limit_activity_type)ite->first);
				}
				if (ite->first == e_time_limit_activity_type_max - 1)
				{
					time_limit_activity_temp_ws_mgr::get_instance().end_act_with_act_type(ite->first, true);
				}
				else
				{
					time_limit_activity_temp_ws_mgr::get_instance().end_act_with_act_type(ite->first, false);
				}
			}
		}
		else
		{
			auto ite = m_activity_info.find(activity_type);
			if (ite != m_activity_info.end())
			{
				if (ite->second.get_activity_info(e_time_limit_ws_act_info_refresh_time) > 0)
				{
					end_act_with_time_limit_activity_type((e_time_limit_activity_type)activity_type);
				}
				time_limit_activity_temp_ws_mgr::get_instance().end_act_with_act_type(activity_type, true);
			}
		}
	}

	void time_limit_activity_ws_mgr::end_act_with_time_limit_activity_type(e_time_limit_activity_type activity_type)
	{
		if (activity_type >= e_time_limit_activity_type_max)
		{
			return;
		}
		if (init_unit::is_rank_time_limit_cross_activity(activity_type))
		{
			send_activity_rank_rewards((e_time_limit_activity_type)activity_type);
		}
		clear_activity_by_type((e_time_limit_activity_type)activity_type);
	}

	void time_limit_activity_ws_mgr::send_load_time_limit_act_to_db()
	{
		ws2dp_load_time_limit_ws_info msg;
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void time_limit_activity_ws_mgr::save_ws_time_limit_act_to_db(int32 activity_type /*= -1*/)
	{
		if (activity_type >= e_time_limit_activity_type_max)
		{
			return;
		}
		ws2dp_save_time_limit_ws_info msg;
		int32 data_num = 0;
		if (activity_type < 0)
		{
			int32 index = 0;
			for (auto ite = m_activity_info.begin(); ite != m_activity_info.end(); ++ite)
			{
				msg.act_info[index] = ite->second.get_ws_db_info();
				data_num++;
				index++;
			}
		}
		else
		{
			auto ite = m_activity_info.find(activity_type);
			if (ite != m_activity_info.end())
			{
				msg.act_info[0] = ite->second.get_ws_db_info();
				data_num++;
			}
		}

		if (data_num > 0)
		{
			msg.data_num = data_num;
			ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
		}
	}

	void time_limit_activity_ws_mgr::load_end_time_limit_act(const s_time_limit_ws_info* act_info, int32 data_num, int32 is_end)
	{
		if (nullptr == act_info || data_num > e_time_limit_activity_type_max)
		{
			return;
		}

		for (int32 i = 0; i < data_num; i++)
		{
			int32 dp_act_type = act_info[i].act_info[e_time_limit_ws_act_info_type];
			auto activity_info_ite = m_activity_info.find(dp_act_type);
			if (activity_info_ite != m_activity_info.end())
			{
				activity_info_ite->second.set_ws_db_info(act_info[i]);
			}
		}
		if (1 == is_end)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_time_limit_info);
		}
		
	}

	bool time_limit_activity_ws_mgr::is_activity_open(e_time_limit_activity_type activity_type)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return false;
		}
		return activity_info_ite->second.is_open();
	}

	void time_limit_activity_ws_mgr::send_activity_schedule_to_other_ws(e_time_limit_activity_type activity_type, int32 server_id /*= 0*/)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}
		ws2ws_time_limit_activity_common_data_sync_end msg;
		msg.activity_type = activity_type;
		msg.activity_schedule = activity_info_ite->second.get_activity_info(e_time_limit_ws_act_info_schedule);
		cross::send_msg_to_ws(guid_64(), server_id, e_msg_index_ws2ws_time_limit_activity_common_data_sync_end, &msg, sizeof(msg));
	}

	void time_limit_activity_ws_mgr::clear_activity_by_type(e_time_limit_activity_type activity_type, bool is_need_save /*= false*/)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return;
		}

		if (init_unit::is_rank_time_limit_cross_activity(activity_type))
		{
			ranking_mgr_ws::clear_data_by_ranking_type((e_RankingIndex)activity_info_ite->second.ranking_type);
		}

		if (activity_type == e_time_limit_activity_type_cloud_shop || activity_type == e_time_limit_activity_type_cross_cloud_shop)
		{
			//cloud_shop_mgr::get_instance().clear_cloud_shop_player_info();
		}
		
		activity_info_ite->second.clear_all_activity_info();
		if (is_need_save)
		{
			save_ws_time_limit_act_to_db(activity_type);
		}
	}

	//跨服只返回跨服云购 
	s_time_limit_activity_ws_info& time_limit_activity_ws_mgr::get_cur_cloud_shop_activity()
	{
		if (world_server::getInstance().get_cross_id() > 0 && world_server::getInstance().is_sky_island_server())
		{
			return m_activity_info[e_time_limit_activity_type_cross_cloud_shop];
		}
		else if (world_server::getInstance().get_cross_id() == 0 && !world_server::getInstance().is_sky_island_server())
		{
			return m_activity_info[e_time_limit_activity_type_cloud_shop];
		}
		else
		{
			return m_empty_info;
		}
	}

	s_time_limit_activity_ws_info& time_limit_activity_ws_mgr::get_activity_info_ref(e_time_limit_activity_type activity_type)
	{
		auto activity_info_ite = m_activity_info.find(activity_type);
		if (activity_type >= e_time_limit_activity_type_max || activity_info_ite == m_activity_info.end())
		{
			return m_empty_info;
		}
		else
		{
			return activity_info_ite->second;
		}
	}

	e_time_limit_activity_type time_limit_activity_ws_mgr::get_can_begin_cloud_shop_type_in_this_server()
	{
		if (world_server::getInstance().get_cross_id() > 0 && world_server::getInstance().is_sky_island_server())
		{
			return e_time_limit_activity_type_cross_cloud_shop;
		}

		if (world_server::getInstance().get_cross_id() <= 0 && !world_server::getInstance().is_sky_island_server())
		{
			return e_time_limit_activity_type_cloud_shop;
		}

		return e_time_limit_activity_type_max;//不开返回无效值
	}

}