/********************************************************************
created: 2019年03月17日
file base: time_limit_activity_object
file ext: cpp
author: wangdi

purpose: time limit object
*********************************************************************/

#include "time_limit_activity_object.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "game_enum_def_s.h"
#include "cell_server.hpp"
#include "logic/item_set.h"
#include "server_log.hpp"
#include "utility/random.h"
#include "system/item/item_system.h"
#include "time_limit_activity_temp_mgr.hpp"
#include "utility/init_unit.h"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "internet/time_limit_activity.pb.h"
#include <rlog.hpp>

namespace faith
{
	time_limit_activity_object::time_limit_activity_object()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	time_limit_activity_object::~time_limit_activity_object()
	{
	}

	void time_limit_activity_object::heart_tick(const int64& new_time)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (m_need_sync_to_ranking == true && new_time >= m_interval_time)
		{
			int32 activity_type = get_act_type();
			int32 highest_record_type = 0;
			if (activity_type >= e_time_limit_activity_type_cross_gs_up && activity_type <= e_time_limit_activity_type_cross_spirit_up)
			{
				int32 last_time = get_data_info(faith::e_time_limit_activity_info_end_time) - time_helper::get_cur_time_new().second;
				if (last_time > second_tick_time * 60 * 15)
				{
					m_interval_time = new_time + second_tick_time * 60 * 5;		//5分钟
				}
				else
				{
					m_interval_time = new_time + second_tick_time * 1;		//1秒钟
				}
				highest_record_type = activity_type - e_time_limit_activity_type_cross_gs_up + e_role_history_highest_record_gs_value;
			}
			else
			{
				m_interval_time = new_time + second_tick_time;					//1秒钟
				highest_record_type = activity_type - e_time_limit_activity_type_gs_up + e_role_history_highest_record_gs_value;
			}

			//m_interval_time = new_time + second_tick_time * 1;					//1秒钟

			int32 ranking_index = activity_type - faith::e_time_limit_activity_type_gs_up + faith::e_RankingIndex_time_limit_activity_gs_up;
			int64 ranking_value = m_player_ptr->get_highest_record(highest_record_type);
			int64 save_value = get_activity_schedule() + get_activity_schedule(5);
			int32 peak_value = m_player_ptr->get_highest_record(highest_record_type) - save_value;
			m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data((e_RankingIndex)ranking_index, ranking_value, 0, peak_value);
			m_need_sync_to_ranking = false;
		}
	}

	void time_limit_activity_object::clear_data()
	{
		m_activity_info.clear_data();
		m_cycle_num = 0;
		m_cycle_index = 0;
		m_interval_time = 0;
		m_is_old_act = false;
		m_time_limit_activity_branch_template_id = 0;
	}

	void time_limit_activity_object::clear_schedule_and_reward_flag()
	{
		for (int32 i = 0; i < time_limit_activity_schedule_num; i++)
		{
			m_activity_info.activity_schedule[i] = 0;
		}
		set_gs_up_init_value();
		m_activity_info.reward_flag = 0;
	}

	void time_limit_activity_object::set_gs_up_init_value()
	{
		//提升榜初始值设置
		if (m_player_ptr == nullptr)
		{
			return;
		}
		int32 activity_type = get_act_type();
		int32 highest_record_type = -1;
		if (activity_type >= faith::e_time_limit_activity_type_gs_up && activity_type <= faith::e_time_limit_activity_type_spirit_up)
		{
			highest_record_type = activity_type - e_time_limit_activity_type_gs_up + e_role_history_highest_record_gs_value;
		}
		if (activity_type >= faith::e_time_limit_activity_type_cross_gs_up && activity_type <= faith::e_time_limit_activity_type_cross_spirit_up)
		{
			highest_record_type = activity_type - e_time_limit_activity_type_cross_gs_up + e_role_history_highest_record_gs_value;
		}
		if (highest_record_type >= 0)
		{
			//当天刷新过战力提升榜初始值 则使用零点的记录数值
			if (m_player_ptr->is_need_refresh_gs_up_start_value_time() == false)
			{
				_RLOG_(MINFO, ::faith::log_detail::format_message("time_limit_activity_object::set_gs_up_init_value  highest_record_type:{} value:{}",  highest_record_type, 
					m_player_ptr->get_unit_i64_info_data((e_role_i64_info)((e_role_history_highest_record)highest_record_type - e_role_history_highest_record_gs_value + e_role_i64_info_history_best_gs_value))));


				int64 gs_value = m_player_ptr->get_unit_i64_info_data((e_role_i64_info)((e_role_history_highest_record)highest_record_type - e_role_history_highest_record_gs_value + e_role_i64_info_history_best_gs_value));
				int32 set_num = gs_value > int32_MAX_NUM ? int32_MAX_NUM : gs_value;
				set_activity_schedule(set_num);
				if (set_num != gs_value)
				{
					set_activity_schedule((gs_value - set_num), 5);
				}
			}
			else
			{
				_RLOG_(MINFO, ::faith::log_detail::format_message("time_limit_activity_object::set_gs_up_init_value  highest_record_type:{} value:{}",  highest_record_type,  m_player_ptr->get_highest_record(highest_record_type)));

				int64 gs_value = m_player_ptr->get_highest_record(highest_record_type);
				int32 set_num = gs_value > int32_MAX_NUM ? int32_MAX_NUM : gs_value;
				set_activity_schedule(set_num);
				if (set_num != gs_value)
				{
					set_activity_schedule((gs_value - set_num), 5);
				}
			}
			//schedule中第二位表示第一位进度值是否有效 用于区分角色上线第一次开启这个活动时重置战力初始值 战力初始值本身就是0 这两种情况
			set_activity_schedule(1, 1);
		}
	}

	void time_limit_activity_object::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}
	void time_limit_activity_object::init_end()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (get_act_type() == e_time_limit_activity_type_dragontrip)
		{		
			m_player_ptr->get_dragontrip_mgr().refresh_recycle();
		}
		if (get_act_type() == e_time_limit_activity_type_sky_treasure)
		{
			m_player_ptr->get_skytreasure_mgr().refresh_recycle();
		}
		if (get_act_type() == e_time_limit_activity_type_starark)
		{
			m_player_ptr->get_starark_mgr().refresh_recycle();
		}
		if (get_act_type() == e_time_limit_activity_type_lucky_card)
		{
			m_player_ptr->get_lucky_card_mgr().refresh_recycle();
		}
		if (get_act_type() == e_time_limit_activity_type_star_trip)
		{
			m_player_ptr->get_star_trip_mgr().init_activiy_info();
		}

	}

	void time_limit_activity_object::refresh_by_zero(const int64& time_now)
	{
		bool is_refresh = false;
		switch (get_act_type())
		{
		//周期刷新
		case e_time_limit_activity_type_total_charge:
		case e_time_limit_activity_type_total_cost:
		case e_time_limit_activity_type_treasure:
		case e_time_limit_activity_type_treasure_royal1:
		case e_time_limit_activity_type_treasure_royal2:
		case e_time_limit_activity_type_treasure_royal3:
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_treasure_sprite:
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_landing_reward:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_national_treasure_4:
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
		case e_time_limit_activity_type_treasure_element:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_dragontrip:
		case e_time_limit_activity_type_special_exchange:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare_reward:
		case e_time_limit_activity_type_sky_treasure:
		case e_time_limit_activity_type_starark:
		case e_time_limit_activity_type_weekend_top_up:
		case e_time_limit_activity_type_support_for_promotion:
		case e_time_limit_activity_type_lucky_card:
		case e_time_limit_activity_type_cross_total_charge:
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3:
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_new_server_mail:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		case e_time_limit_activity_type_limit_time_treasure2:
		case e_time_limit_activity_type_limit_time_notice:
		case e_time_limit_activity_type_diamond_shop:
		{
			if (time_now / second_tick_time >= get_data_info(e_time_limit_activity_info_end_time))
			{
				is_refresh = true;
			}
		}
			break;
		//每日刷新
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_cloud_shop:
		case e_time_limit_activity_type_cross_cloud_shop:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_new_behavior_welfare:
		case e_time_limit_activity_type_daily_free_reward:
		{
			is_refresh = true;
		}
			break;
		default:
			break;
		}

		//如果刷新 就清理进度和领奖标记 如果不刷新 每天要计算一次时间和id 方便改表后以表格时间为准
		if (is_refresh)
		{
			clear_schedule_and_reward_flag();
			init_end();
		}

		//限时商店和折扣商店 刷新每日限购类型的进度
		if (e_time_limit_activity_type_shop == get_act_type() 
			|| e_time_limit_activity_type_discount_shop == get_act_type()
			|| e_time_limit_activity_type_discount_shop1 == get_act_type()
			|| e_time_limit_activity_type_discount_shop2 == get_act_type()
			|| e_time_limit_activity_type_discount_shop3 == get_act_type()
			|| e_time_limit_activity_type_item_shop == get_act_type()
			|| e_time_limit_activity_type_item_shop1 == get_act_type()
			|| e_time_limit_activity_type_item_shop2 == get_act_type()
			|| e_time_limit_activity_type_item_shop3 == get_act_type()
			|| e_time_limit_activity_type_weekend_top_up == get_act_type()
			|| e_time_limit_activity_type_support_for_promotion == get_act_type())
		{
			for (int32 i = 0; i < time_limit_activity_schedule_num; i++)
			{
				int32 branch_template_id = m_time_limit_activity_branch_template_id + i;
				if (!is_same_cycle(branch_template_id))
				{
					break;
				}
				const s_time_limit_activity_branch_temp& act_branch_temp = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id);
				if (act_branch_temp.is_valid() && act_branch_temp.ParamArr1.size() > 0 && act_branch_temp.ParamArr1[0] == e_time_limit_shop_purchase_limit_type_day)
				{
					set_activity_schedule(0, i);
				}
			}
		

		}

		if (e_time_limit_activity_type_diamond_shop == get_act_type())
		{
			set_activity_schedule(0, 1);
		}

		const s_time_limit_activity_temp& act_temp = get_time_limit_activity_temp();
		if (!act_temp.is_valid())
		{
			return;
		}
		
		if (m_is_old_act)
		{
			m_time_limit_activity_branch_template_id = init_unit::get_cur_time_limit_branch_id_with_first_id(act_temp, time_limit_activity_temp_mgr::get_instance().get_branch_temp_map(), get_data_info(e_time_limit_activity_info_start_time), get_data_info(e_time_limit_activity_info_end_time), get_data_info(e_time_limit_activity_info_first_branch_guid));
		}
		else
		{
			int32 base_time;
			m_time_limit_activity_branch_template_id = init_unit::get_cur_time_limit_activity_id_by_temp(act_temp, time_limit_activity_temp_mgr::get_instance().get_branch_temp_map()
				, m_activity_info.data_ary[e_time_limit_activity_info_start_time]
				, m_activity_info.data_ary[e_time_limit_activity_info_end_time]
				, m_cycle_num, m_cycle_index, base_time, m_activity_info.data_ary[e_time_limit_activity_info_first_branch_guid]
				, cell_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time)
				, cell_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time)
				, cell_server::getInstance().is_cross_cs());
		}

		if (is_open())
		{
			send_info_to_self();
			if (get_act_type() == e_time_limit_activity_type_new_server_mail)
			{
				check_send_new_server_mail();
			}
		}

		//init_end();
		
	}

	//这个函数用于读取库中的数据之后初始化 不对读取出来的数据进行赋值 (开始时间 结束时间)
	void time_limit_activity_object::init_template_id() //question_1 init activity temp id
	{		
		const s_time_limit_activity_temp& act_temp = get_time_limit_activity_temp();
		//从库中读取出的数据如果是0 需要刷新数据 避免上一天的数据残留
		if (!act_temp.is_valid())
		{
			return;
		}

		if (m_is_old_act)
		{
			m_time_limit_activity_branch_template_id = init_unit::get_cur_time_limit_branch_id_with_first_id(act_temp, time_limit_activity_temp_mgr::get_instance().get_branch_temp_map(), get_data_info(e_time_limit_activity_info_start_time), get_data_info(e_time_limit_activity_info_end_time), get_data_info(e_time_limit_activity_info_first_branch_guid));
		}
		else
		{
			int32 base_time;
			m_time_limit_activity_branch_template_id = init_unit::get_cur_time_limit_activity_id_by_temp(act_temp, time_limit_activity_temp_mgr::get_instance().get_branch_temp_map()
				, m_activity_info.data_ary[e_time_limit_activity_info_start_time]
				, m_activity_info.data_ary[e_time_limit_activity_info_end_time]
				, m_cycle_num, m_cycle_index, base_time, m_activity_info.data_ary[e_time_limit_activity_info_first_branch_guid]
				, cell_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_cross_server_time)
				, cell_server::getInstance().get_server_info_arr(e_server_info_type_begin_first_merge_server_time)
				, cell_server::getInstance().is_cross_cs());
		}

		//刷新排行进度初始值
		if (get_act_type() >= e_time_limit_activity_type_gs_up && get_act_type() <= e_time_limit_activity_type_cross_spirit_up && get_activity_schedule(0) <= 0 && get_activity_schedule(1) <= 0)
		{
			set_gs_up_init_value();
		}
		if (is_open())
		{
			send_info_to_self();
		}
	}
	void time_limit_activity_object::init_template_id_no_cross(int32 act_guid)
	{
		if (nullptr == m_player_ptr || !m_player_ptr->is_self_server())
		{
			return;
		}
		if (act_guid > 0 && get_act_guid() <= 0)
		{
			set_data_info(e_time_limit_activity_info_guid, act_guid);
		}
		init_template_id();
	}
	void time_limit_activity_object::init_template_id_cross(int32 act_guid)
	{
		if (nullptr == m_player_ptr || m_player_ptr->is_self_server())
		{
			return;
		}
		if (act_guid > 0 && get_act_guid() <= 0)
		{
			set_data_info(e_time_limit_activity_info_guid, act_guid);
		}
		init_template_id();
	}
	bool time_limit_activity_object::is_can_get_reward(int32 reward_template_id, int32 param_int_1)
	{
		int32 reward_index = reward_template_id - m_time_limit_activity_branch_template_id;

		//启示之路单独处理
		if (get_act_type() == e_time_limit_activity_type_behavior_welfare
			|| get_act_type() == e_time_limit_activity_type_behavior_welfare1
			|| get_act_type() == e_time_limit_activity_type_behavior_welfare2
			|| get_act_type() == e_time_limit_activity_type_behavior_welfare3
			|| get_act_type() == e_time_limit_activity_type_new_behavior_welfare)
		{
			reward_index = param_int_1;
		}

		if (!is_open() || !is_same_cycle(reward_template_id) || is_reward_geted(reward_index))
		{
			return false;
		}

		const s_time_limit_activity_branch_temp& time_limit_activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(reward_template_id);
		if (time_limit_activity_branch_template_ptr.ActGuid <= 0)
		{
			return false;
		}

		switch (get_act_type())
		{
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare:
		case e_time_limit_activity_type_new_behavior_welfare_reward:	
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3:
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		case e_time_limit_activity_type_limit_time_treasure2:
		{
			return  get_cur_value(reward_index) >= get_target_value(reward_template_id, reward_index);
		}
		break;
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_weekend_top_up:
		case e_time_limit_activity_type_support_for_promotion:
		{
			int32 temp_target_value = get_target_value(reward_template_id);
			if (temp_target_value < 0)
			{
				return true;
			}
			return  get_cur_value(reward_index) + param_int_1 <= get_target_value(reward_template_id);
		}
			break;
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_special_exchange:
		{
			int32 temp_target_value = get_target_value(reward_template_id);
			if (temp_target_value < 0)
			{
				return true;
			}
			return  get_cur_value(reward_index) < get_target_value(reward_template_id);
		}
			break;
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_landing_reward:
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() <= 1)
			{
				break;
			}
			int32 cur_second = get_cur_value();
			return cur_second >= time_limit_activity_branch_template_ptr.Condition[0] && cur_second < time_limit_activity_branch_template_ptr.Condition[1];
		}
			break;
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_national_treasure_4:
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() <= 1)
			{
				break;
			}
			//单独处理
			if (get_cur_value() >= time_limit_activity_branch_template_ptr.Condition[0])
			{
				m_player_ptr->get_time_limit_activity_mgr().check_time_limit_activity_common_data_to_ws(reward_template_id, get_act_type(), time_limit_activity_branch_template_ptr.Condition[1]);
			}
		}
		break;
		case e_time_limit_activity_type_new_server_mail:
		{
			if (nullptr == m_player_ptr)
			{
				return false;
			}
			if (m_player_ptr->get_logic_data(e_role_logic_info_is_have_new_server_mail) > 0)
			{
				return false;
			}
			return  get_cur_value(reward_index) >= get_target_value(reward_template_id, reward_index);
		}
		break;
		// 每日免费礼包不需要领取条件 只需要判断是否领取过就行
		case e_time_limit_activity_type_daily_free_reward:
		{
			return true;
		}
		break;
		default:
			break;
		}
		return false;
	}

	int32 time_limit_activity_object::get_cur_value(int32 index)
	{
		int32 return_value = 0;
		switch (get_act_type())
		{
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_national_treasure_4:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare_reward:
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3:
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_daily_free_reward:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		case e_time_limit_activity_type_limit_time_treasure2:
		{
			return_value = get_activity_schedule();
		}
		break;
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		{
			return_value = time_helper::get_cur_time_new().second_in_day;
		}
		break;
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_special_exchange:
		case e_time_limit_activity_type_new_behavior_welfare:
		case e_time_limit_activity_type_support_for_promotion:
		case e_time_limit_activity_type_weekend_top_up:
		{
			return_value = get_activity_schedule(index);
		}
		break;
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_landing_reward:
		{
			//计算距离活动开始时间的秒数
			return_value = time_helper::get_cur_time_new().second - get_data_info(e_time_limit_activity_info_start_time);
		}
		break;
		case e_time_limit_activity_type_new_server_mail:
		{
			if (nullptr == m_player_ptr)
			{
				return 0;
			}
			return_value = m_player_ptr->get_unit_info(e_role_info_exp_level);
		}
		break;
		default:
			break;
		}
		return return_value;
	}
	int32 time_limit_activity_object::get_target_value(int32 reward_template_id, int32 index)
	{
		int32 return_value = 0;
		const s_time_limit_activity_branch_temp& time_limit_activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(reward_template_id);
		if (time_limit_activity_branch_template_ptr.ActGuid <= 0)
		{
			return return_value;
		}

		switch (get_act_type())
		{
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare_reward:
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3:
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_new_server_mail:
		case e_time_limit_activity_type_limit_time_treasure2:
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() <= 0)
			{
				break;
			}
			return_value = time_limit_activity_branch_template_ptr.Condition[0];
		}
		break;
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_landing_reward:
		case e_time_limit_activity_type_daily_free_reward:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() <= 1)
			{
				break;
			}
			return_value = time_limit_activity_branch_template_ptr.Condition[1];
		}
		break;
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_new_behavior_welfare:
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() <= index * 2 + 1)
			{
				break;
			}
			return_value = time_limit_activity_branch_template_ptr.Condition[index * 2 + 1];
		}
		break;
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_weekend_top_up:
		case e_time_limit_activity_type_support_for_promotion:
		{
			if (time_limit_activity_branch_template_ptr.ParamArr1.size() <= 0)
			{
				break;
			}
			if (e_time_limit_shop_purchase_limit_type_none == time_limit_activity_branch_template_ptr.ParamArr1[0])
			{
				return_value = -1;
			}
			else if (e_time_limit_shop_purchase_limit_type_day == time_limit_activity_branch_template_ptr.ParamArr1[0]
				|| e_time_limit_shop_purchase_limit_type_cycle == time_limit_activity_branch_template_ptr.ParamArr1[0])
			{
				if (time_limit_activity_branch_template_ptr.ParamArr1.size() <= 1)
				{
					break;
				}
				return_value = time_limit_activity_branch_template_ptr.ParamArr1[1];
			}
		}
		break;
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_special_exchange:
		{
			if (time_limit_activity_branch_template_ptr.ParamArr1.size() <= 0)
			{
				break;
			}
			return_value = time_limit_activity_branch_template_ptr.ParamArr1[0];
		}
		break;
		case e_time_limit_activity_type_national_treasure_4:
		case e_time_limit_activity_type_national_treasure_2:
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() <= 1)
			{
				break;
			}
			if (index < 0 || index >= 2)
			{
				break;
			}
			return_value = time_limit_activity_branch_template_ptr.Condition[index];
		}
		break;
		default:
			break;
		}
		return return_value;
	}

	void time_limit_activity_object::get_reward(int32 reward_template_id)
	{
		if (!is_can_get_reward(reward_template_id))
		{
			return;
		}
		send_reward(reward_template_id);
	}

	void time_limit_activity_object::buy_goods(int32 reward_template_id, int32 goods_num)
	{
		if (nullptr == m_player_ptr || goods_num <= 0 || !is_can_get_reward(reward_template_id, goods_num))
		{
			return;
		}
		const s_time_limit_activity_branch_temp& time_limit_activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(reward_template_id);
		if (time_limit_activity_branch_template_ptr.ActGuid <= 0 || time_limit_activity_branch_template_ptr.Reward.size() <= 0 || time_limit_activity_branch_template_ptr.Reward.size() % 3 != 0
			|| time_limit_activity_branch_template_ptr.Condition.size() < 4 || time_limit_activity_branch_template_ptr.ParamArr2.size() < 1)
		{
			return;
		}

		if (time_limit_activity_branch_template_ptr.ParamArr2[0] != 0 && time_limit_activity_branch_template_ptr.ParamArr2[0] != m_player_ptr->get_unit_info(e_role_info_class_type))
		{
			return;
		}

		if (!m_player_ptr->can_cut_money((e_money_type)time_limit_activity_branch_template_ptr.Condition[2], time_limit_activity_branch_template_ptr.Condition[3] * goods_num))
		{
			return;
		}
		e_server_log_cut_money log_type = e_server_log_cut_money_time_limit_shop;
		switch (get_act_type())
		{
		case e_time_limit_activity_type_shop:
		{
			log_type = e_server_log_cut_money_activity_shop;
		}
		break;
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		{
			log_type = e_server_log_cut_money_discount_shop;
		}
		break;
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		{
			log_type = e_server_log_cut_money_item_shop;
		}
		break;
		case e_time_limit_activity_type_weekend_top_up:
		{
			log_type = e_server_log_cut_money_weekend_top_up;
		}
		break;
		case e_time_limit_activity_type_support_for_promotion:
		{
			log_type = e_server_log_cut_money_support_for_promotion;
		}
		break;
		default:
			break;
			log_type = e_server_log_cut_money_time_limit_shop;
		}
		m_player_ptr->cut_money((e_money_type)time_limit_activity_branch_template_ptr.Condition[2], time_limit_activity_branch_template_ptr.Condition[3] * goods_num, log_type);

		std::vector<citem*> item_array;
		vector<s_item_template_info> promp_item_data;
		for (int32 i = 0; i * 3 + 2 < time_limit_activity_branch_template_ptr.Reward.size(); i++)
		{
			int32 item_template_id = time_limit_activity_branch_template_ptr.Reward[i * 3];
			int32 item_num = time_limit_activity_branch_template_ptr.Reward[i * 3 + 1] * goods_num;
			int32 is_lock = time_limit_activity_branch_template_ptr.Reward[i * 3 + 2];


			promp_item_data.push_back({ item_template_id,item_num,is_lock });
			server_log::time_limit_activity_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), reward_template_id, get_act_type(), get_cur_value(reward_template_id - m_time_limit_activity_branch_template_id)
				, get_target_value(reward_template_id, reward_template_id - m_time_limit_activity_branch_template_id), item_template_id, item_num, m_player_ptr->get_login_type(), time_limit_activity_branch_template_ptr.Condition[2], time_limit_activity_branch_template_ptr.Condition[3] * goods_num, time_limit_activity_branch_template_ptr.AttributeId);
		}

		m_player_ptr->get_item_set().put_in_bag(e_server_log_add_item_time_limit, reward_template_id, promp_item_data);
		m_player_ptr->send_promp_msg_to_client(promp_item_data);

		add_activity_schedule(goods_num, reward_template_id - m_time_limit_activity_branch_template_id);

		server_log::time_limit_activity_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), reward_template_id, get_act_type(), get_cur_value(reward_template_id - m_time_limit_activity_branch_template_id)
			, get_target_value(reward_template_id, reward_template_id - m_time_limit_activity_branch_template_id), 0, 0, m_player_ptr->get_login_type(), time_limit_activity_branch_template_ptr.Condition[2], time_limit_activity_branch_template_ptr.Condition[3] * goods_num, time_limit_activity_branch_template_ptr.AttributeId);
		send_info_to_self();
	}

	void time_limit_activity_object::send_reward(int32 reward_template_id)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		const s_time_limit_activity_branch_temp& time_limit_activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(reward_template_id);
		if (time_limit_activity_branch_template_ptr.ActGuid <= 0 || time_limit_activity_branch_template_ptr.Reward.size() <= 0 || time_limit_activity_branch_template_ptr.Reward.size() % 2 != 0)
		{
			return;
		}
		int32 cost_money_type = 0;
		int32 cost_money_num = 0;
		//兑换活动可多次兑换 次数记录在进度中 不需要记录是否领取过
		if (get_act_type() == e_time_limit_activity_type_item_exchange || get_act_type() == e_time_limit_activity_type_special_exchange)
		{
			//判断物品够不够
			if (time_limit_activity_branch_template_ptr.Condition.size() <= 0 || time_limit_activity_branch_template_ptr.Condition.size() % 2 != 0)
			{
				return;
			}
			for (int32 i = 0; i * 2 + 1 < time_limit_activity_branch_template_ptr.Condition.size(); i++)
			{
				if (item_system::can_cost_item(m_player_ptr, e_bag_type_bag, time_limit_activity_branch_template_ptr.Condition[i * 2], time_limit_activity_branch_template_ptr.Condition[i * 2 + 1]) == false)
				{
					return;
				}
			}
			for (int32 i = 0; i * 2 + 1 < time_limit_activity_branch_template_ptr.Condition.size(); i++)
			{
				item_system::cost_item_from_bag(m_player_ptr, e_bag_type_bag, time_limit_activity_branch_template_ptr.Condition[i * 2], time_limit_activity_branch_template_ptr.Condition[i * 2 + 1]);
			}

			add_activity_schedule(1, reward_template_id - m_time_limit_activity_branch_template_id);
		}
		//小额充值 需要判断玩家身上的钱是否足够 并扣除这部分钻石
		else if (get_act_type() == e_time_limit_activity_type_small_charge 
			|| get_act_type() == e_time_limit_activity_type_daily_gift_charge )
		{
			if (time_limit_activity_branch_template_ptr.Condition.size() < 1)
			{
				return;
			}

			RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, time_limit_activity_branch_template_ptr.Condition[0]);
			if (nullptr == recharge_template_ptr || recharge_template_ptr->MoneyGetArray.size() < 2)
			{
				return;
			}

			if (!m_player_ptr->can_cut_money(e_money_type(recharge_template_ptr->MoneyGetArray[0]), recharge_template_ptr->MoneyGetArray[1]))
			{
				return;
			}
			m_player_ptr->cut_money(e_money_type(recharge_template_ptr->MoneyGetArray[0]), recharge_template_ptr->MoneyGetArray[1], e_server_log_cut_money_time_limit_small_charge);

			add_activity_schedule(1, reward_template_id - m_time_limit_activity_branch_template_id);
			cost_money_type = recharge_template_ptr->MoneyGetArray[0];
			cost_money_num = recharge_template_ptr->MoneyGetArray[1];
		}
		else
		{
			set_reward_get_flag_with_id(reward_template_id);
		}

		std::vector<s_item_template_info> promp_item_data;
		for (int32 i = 0; i * 2 + 1 < time_limit_activity_branch_template_ptr.Reward.size(); i++)
		{
			int32 item_template_id = time_limit_activity_branch_template_ptr.Reward[i * 2];
			int32 item_num = time_limit_activity_branch_template_ptr.Reward[i * 2 + 1];

			ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_template_id);
			if (nullptr == ItemTemplatePtr)
			{
				continue;
			}

			//跳过不属于自己的物品
			if (ItemTemplatePtr->class_type.size() > 0)
			{
				bool need_pass = true;

				for (int32 j = 0; j < ItemTemplatePtr->class_type.size(); j++)
				{
					if (ItemTemplatePtr->class_type[j] == 0 || ItemTemplatePtr->class_type[j] == m_player_ptr->get_unit_info(e_role_info_class_type))
					{
						need_pass = false;
					}
				}

				if (need_pass)
				{
					continue;
				}
			}

			promp_item_data.push_back({ item_template_id, item_num, 1 });
			server_log::time_limit_activity_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), reward_template_id, get_act_type(), get_cur_value(reward_template_id - m_time_limit_activity_branch_template_id)
				, get_target_value(reward_template_id, reward_template_id - m_time_limit_activity_branch_template_id), item_template_id, item_num, m_player_ptr->get_login_type(), cost_money_type, cost_money_num, time_limit_activity_branch_template_ptr.AttributeId);
		}
		m_player_ptr->get_item_set().put_in_bag(e_server_log_add_item_time_limit, reward_template_id, promp_item_data);
	
		m_player_ptr->send_promp_msg_to_client(promp_item_data);

		send_info_to_self();

		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		int32 region_code = region_template_ptr->RegionCode;
		if (region_code != e_version_region_type_jp)
		{
			return;
		}
		if (get_act_type() == e_time_limit_activity_type_new_server_treasure && time_limit_activity_branch_template_ptr.Condition.size() > 0)
		{
			if (time_limit_activity_branch_template_ptr.Condition[0] >= 200)
			{
				game_proto_send_new_server_treasure_finish send_msg;
				m_player_ptr->send_message_to_self(&send_msg, e_msgindex_s2c_send_new_server_treasure_finish);
			}
		}
	}

	bool time_limit_activity_object::is_same_cycle(int32 reward_template_id)
	{
		if (reward_template_id < m_time_limit_activity_branch_template_id)
		{
			return false;
		}

		const s_time_limit_activity_temp& activity_temp = get_time_limit_activity_temp();
		if (activity_temp.ActGuid <= 0)
		{
			return false;
		}

		for (int32 i = 0; i + 1 < activity_temp.CycleFirstId.size(); i++)
		{
			//这个id在两个周期之间
			if (m_time_limit_activity_branch_template_id == activity_temp.CycleFirstId[i] && reward_template_id >= activity_temp.CycleFirstId[i + 1])
			{
				return false;
			}
		}
		const s_time_limit_activity_branch_temp& act_branch_temp = get_time_limit_activity_branch_temp();
		if (act_branch_temp.ActGuid <= 0)
		{
			return false;
		}
		const s_time_limit_activity_branch_temp& reward_act_branch_temp = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(reward_template_id);
		if (reward_act_branch_temp.ActGuid <= 0 || reward_act_branch_temp.ActivityType != act_branch_temp.ActivityType
			|| reward_act_branch_temp.CycleIndex != act_branch_temp.CycleIndex)
		{
			return false;
		}
		return true;
	}
	void time_limit_activity_object::get_reward_by_check_common_data_end(int32 reward_template_id, bool is_achieve)
	{
		if (!is_achieve)
		{
			return;
		}

		int32 reward_index = reward_template_id - m_time_limit_activity_branch_template_id;

		if (!is_open() || !is_same_cycle(reward_template_id) || is_reward_geted(reward_index))
		{
			return;
		}

		send_reward(reward_template_id);
	}

	bool time_limit_activity_object::is_valid()
	{
		return m_activity_info.data_ary[e_time_limit_activity_info_guid] > 0;
	}

	bool time_limit_activity_object::is_open()
	{
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		if (get_act_type() != e_time_limit_activity_type_national_treasure_3 || get_act_type() != e_time_limit_activity_type_national_treasure_4)
		{
			if (!init_unit::is_cross_time_limit_cross_activity(get_act_type()) && !m_player_ptr->is_self_server())
			{
				return false;
			}
		}
		int32 cur_time_stamp = time_helper::get_cur_time_new().second;
		return cur_time_stamp >= m_activity_info.data_ary[e_time_limit_activity_info_start_time] && cur_time_stamp < m_activity_info.data_ary[e_time_limit_activity_info_end_time] && m_time_limit_activity_branch_template_id > 0;
	}
	void time_limit_activity_object::send_info_to_self()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		int32 act_guid = get_data_info(e_time_limit_activity_info_guid);
		if (act_guid <= 0)
		{
			return;
		}
		switch (get_act_type())
		{
		case e_time_limit_activity_type_daily_charge:
		case e_time_limit_activity_type_treasure:
		case e_time_limit_activity_type_treasure_royal1:
		case e_time_limit_activity_type_treasure_royal2:
		case e_time_limit_activity_type_treasure_royal3:
		case e_time_limit_activity_type_time_welfare:
		case e_time_limit_activity_type_time_welfare2:
		case e_time_limit_activity_type_time_welfare3:
		case e_time_limit_activity_type_time_welfare4:
		case e_time_limit_activity_type_treasure_sprite:
		case e_time_limit_activity_type_time_welfare5:
		case e_time_limit_activity_type_daily_charge2:
		case e_time_limit_activity_type_daily_charge3:
		case e_time_limit_activity_type_daily_charge4:
		case e_time_limit_activity_type_daily_charge5:
		case e_time_limit_activity_type_daily_charge6:
		case e_time_limit_activity_type_total_charge2:
		case e_time_limit_activity_type_behavior_welfare_reward:
		case e_time_limit_activity_type_behavior_welfare_reward1:
		case e_time_limit_activity_type_behavior_welfare_reward2:
		case e_time_limit_activity_type_behavior_welfare_reward3:
		case e_time_limit_activity_type_total_landing:
		case e_time_limit_activity_type_total_cost_jewel:
		case e_time_limit_activity_type_landing_reward:
		case e_time_limit_activity_type_national_treasure_1:
		case e_time_limit_activity_type_national_treasure_2:
		case e_time_limit_activity_type_national_treasure_3:
		case e_time_limit_activity_type_national_treasure_4:
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
		case e_time_limit_activity_type_treasure_element:
		case e_time_limit_activity_type_new_total_landing:
		case e_time_limit_activity_type_new_behavior_welfare_reward:
		case e_time_limit_activity_type_sky_treasure:
		case e_time_limit_activity_type_new_total_charge_1:
		case e_time_limit_activity_type_new_total_charge_2:
		case e_time_limit_activity_type_new_total_charge_3: 
		case e_time_limit_activity_type_new_total_cost_jewel_1:
		case e_time_limit_activity_type_new_total_cost_jewel_2:
		case e_time_limit_activity_type_new_total_cost_jewel_3:
		case e_time_limit_activity_type_new_server_treasure:
		case e_time_limit_activity_type_new_server_mail:
		case e_time_limit_activity_type_daily_free_reward:
		case e_time_limit_activity_type_time_feed_back1:
		case e_time_limit_activity_type_time_feed_back2:
		case e_time_limit_activity_type_time_feed_back3:
		case e_time_limit_activity_type_limit_time_treasure2:
		case e_time_limit_activity_type_limit_time_notice:
		{
			time_limit_activity_proto_norank_type_activity_message msg;
			msg.set_activity_type(act_guid);
			msg.set_activity_value(get_activity_schedule());
			//最多循环50次
			for (int32 i = 0; i < 50; i++)
			{
				int32 branch_template_id = m_time_limit_activity_branch_template_id + i;
				if (!is_same_cycle(branch_template_id))
				{
					break;
				}
				time_limit_activity_proto_activity_tempid_and_flag* reward_flag = msg.add_act_msg();
				reward_flag->set_activity_temp_id(branch_template_id);
				if (is_reward_geted(i))
				{
					reward_flag->set_activity_reward_flag(1);
				}
				else
				{
					reward_flag->set_activity_reward_flag(0);
				}
			}
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_update_sub_no_rank);
		}
			break;
		case e_time_limit_activity_type_behavior_welfare:
		case e_time_limit_activity_type_behavior_welfare1:
		case e_time_limit_activity_type_behavior_welfare2:
		case e_time_limit_activity_type_behavior_welfare3:
		case e_time_limit_activity_type_new_behavior_welfare:
		{
			time_limit_activity_proto_all_behavior_type_activity_message msg;
			msg.set_activity_type(act_guid);
			msg.set_activity_sub_id(m_time_limit_activity_branch_template_id);
			const s_time_limit_activity_branch_temp& branch_template_ptr = get_time_limit_activity_branch_temp();
			if (branch_template_ptr.ActGuid <= 0)
			{
				break;
			}
			for (int32 i = 0; i < time_limit_activity_schedule_num; i++)
			{
				if (i >= branch_template_ptr.Condition.size() / 2)
				{
					break;
				}
				time_limit_activity_proto_one_behavior_message* one_msg = msg.add_all_msg();
				one_msg->set_behavior_idex(i);
				one_msg->set_behavior_value(get_activity_schedule(i));
			}
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_update_all_behavior);
		}
			break;
		case e_time_limit_activity_type_shop:
		case e_time_limit_activity_type_discount_shop:
		case e_time_limit_activity_type_discount_shop1:
		case e_time_limit_activity_type_discount_shop2:
		case e_time_limit_activity_type_discount_shop3:
		case e_time_limit_activity_type_item_exchange:
		case e_time_limit_activity_type_small_charge:
		case e_time_limit_activity_type_daily_gift_charge:
		case e_time_limit_activity_type_item_shop:
		case e_time_limit_activity_type_item_shop1:
		case e_time_limit_activity_type_item_shop2:
		case e_time_limit_activity_type_item_shop3:
		case e_time_limit_activity_type_special_exchange:
		case e_time_limit_activity_type_support_for_promotion:
		case e_time_limit_activity_type_weekend_top_up:
		{
			time_limit_activity_proto_norank_type_activity_message msg;
			msg.set_activity_type(act_guid);
			msg.set_activity_value(0);
			for (int32 i = 0; i < time_limit_activity_schedule_num; i++)
			{
				int32 branch_template_id = m_time_limit_activity_branch_template_id + i;
				if (!is_same_cycle(branch_template_id))
				{
					break;
				}
				time_limit_activity_proto_activity_tempid_and_flag* reward_flag = msg.add_act_msg();
				reward_flag->set_activity_temp_id(branch_template_id);
				reward_flag->set_activity_reward_flag(get_activity_schedule(i));
			}
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_update_sub_no_rank);
		}
		break;
		case e_time_limit_activity_type_diamond_shop:
		{
			if (get_activity_schedule(0) <= 0)
			{
				refresh_diamond_shop(1);
			}
			time_limit_activity_proto_diamond_shop_info msg;
			msg.set_is_free(get_activity_schedule(1));
			for (int32 i = 0; i < 9; i++)
			{
				time_limit_activity_proto_diamond_shop_sub_info* sub_info = msg.add_goods_info();
				if (nullptr == sub_info)
				{
					continue;
				}
				sub_info->set_goods_index(get_activity_schedule(2 + (i * 2)));
				sub_info->set_goods_state(get_activity_schedule(2 + (i * 2 + 1)));
			}
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_send_diamond_shop_info);
		}
		break;
		case e_time_limit_activity_type_star_trip:
		{
			if (false == m_player_ptr->get_star_trip_mgr().get_is_init())
			{
				m_player_ptr->get_star_trip_mgr().init_activiy_info();
			}
			
		}
		break;
		default:
		{
			time_limit_activity_proto_norank_type_activity_message msg;
			msg.set_activity_type(act_guid);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_update_sub_no_rank);
		}
			break;
		}
	}

	int32 time_limit_activity_object::get_data_info(e_time_limit_activity_info info_type)
	{
		if (info_type >= e_time_limit_activity_info_max)
		{
			return 0;
		}
		return m_activity_info.data_ary[info_type];
	}

	void time_limit_activity_object::set_data_info(e_time_limit_activity_info info_type, int32 info_value)
	{
		if (info_type >= e_time_limit_activity_info_max)
		{
			return;
		}
		m_activity_info.data_ary[info_type] = info_value;
	}

	int32 time_limit_activity_object::get_activity_schedule(int32 schedule_index /*= 0*/)
	{
		if (schedule_index >= time_limit_activity_schedule_num)
		{
			return 0;
		}
		return m_activity_info.activity_schedule[schedule_index];
	}

	void time_limit_activity_object::set_activity_schedule(int32 schedule_value, int32 schedule_index /*= 0*/)
	{
		if (schedule_index < 0 || schedule_index >= time_limit_activity_schedule_num)
		{
			return;
		}
		m_activity_info.activity_schedule[schedule_index] = schedule_value;
	}

	void time_limit_activity_object::add_activity_schedule(int32 add_value, int32 schedule_index /*= 0*/)
	{
		if (schedule_index < 0 || schedule_index >= time_limit_activity_schedule_num)
		{
			return;
		}
		m_activity_info.activity_schedule[schedule_index] += add_value;
	}

	void time_limit_activity_object::set_activity_schedule_str(xstring schedule_str)
	{
		m_activity_info.set_schedule_str(schedule_str);
	}

	xstring time_limit_activity_object::get_activity_schedule_str()
	{
		return m_activity_info.schedule_str;
	}

	bool time_limit_activity_object::is_reward_geted(int64 reward_index /*= 0*/)
	{
		if (reward_index < 0 || reward_index >= time_limit_activity_get_reward_flag_index_max)
		{
			return true;
		}
		int64 temp_flag = 1;
		temp_flag = temp_flag << reward_index;
		return (temp_flag & m_activity_info.reward_flag) > 0;
	}

	void time_limit_activity_object::set_reward_get_flag(int64 reward_index /*= 0*/)
	{
		if (reward_index < 0 || reward_index >= time_limit_activity_get_reward_flag_index_max)
		{
			return;
		}
		int64 temp_flag = 1;
		temp_flag = temp_flag << reward_index;
		m_activity_info.reward_flag |= temp_flag;
	}

	void time_limit_activity_object::set_reward_get_flag_with_id(int32 reward_template_id)
	{
		if (!is_same_cycle(reward_template_id))
		{
			return;
		}
		set_reward_get_flag(reward_template_id - m_time_limit_activity_branch_template_id);
	}

	const s_time_limit_activity_temp& time_limit_activity_object::get_time_limit_activity_temp()
	{
		return time_limit_activity_temp_mgr::get_instance().get_time_limit_act_with_id(get_data_info(e_time_limit_activity_info_guid));
	}

	const s_time_limit_activity_branch_temp& time_limit_activity_object::get_time_limit_activity_branch_temp()
	{
		return time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(m_time_limit_activity_branch_template_id);
	}

	void time_limit_activity_object::set_need_sync_to_ranking()
	{
		m_need_sync_to_ranking = true;
	}

	bool time_limit_activity_object::is_affected_by_pause_flag()
	{
		const s_time_limit_activity_temp& act_temp_info = get_time_limit_activity_temp();
		if (act_temp_info.ActGuid > 0 && act_temp_info.ParamArr3.size() > 0)
		{
			return act_temp_info.ParamArr3[0];
		}
		return false;
	}

	int32 time_limit_activity_object::get_act_type()
	{
		const s_time_limit_activity_temp& act_temp_info = get_time_limit_activity_temp();
		if (!act_temp_info.is_valid())
		{
			return -1;
		}
		else
		{
			return act_temp_info.ActivityType;
		}
	}

	void time_limit_activity_object::load_lucky_card_info_end(bool is_init)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		m_player_ptr->get_lucky_card_mgr().load_lucky_card_end(m_activity_info, is_init);
	}

	void time_limit_activity_object::check_send_new_server_mail()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		if (!is_can_get_reward(m_time_limit_activity_branch_template_id))
		{
			return;
		}
		
		m_player_ptr->set_logic_data(e_role_logic_info_is_have_new_server_mail, 1);

		item_set& temp_item_set = m_player_ptr->get_item_set();

		int32 first_template_id = get_data_info(e_time_limit_activity_info_first_branch_guid);
		//找id最多循环100次 防止无限循环 
		for (int32 i = 0; i < 10; ++i)
		{
			const s_time_limit_activity_branch_temp& time_limit_activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(first_template_id + i);

			if (time_limit_activity_branch_template_ptr.ActGuid <= 0 || time_limit_activity_branch_template_ptr.ActivityType != get_act_type() || time_limit_activity_branch_template_ptr.ParamArr1.size() != 2)
			{
				break;
			}
			if (time_limit_activity_branch_template_ptr.ParamArr3.size() > 0)
			{
				if (globle_data::get_instance().get_server_on_days() > time_limit_activity_branch_template_ptr.ParamArr3[0])
				{
					break;
				}
			}
			std::vector<citem*> item_list;
			std::vector<int32> item_list_arr = time_limit_activity_branch_template_ptr.Reward;
			if (item_list_arr.size() <= 0 || item_list_arr.size() % 3 != 0)
			{
				break;
			}
			for (int32 i = 0; i < item_list_arr.size() / 3; i++)
			{
				int32 award_template_id = item_list_arr[i * 3];
				int32 award_num = item_list_arr[i * 3 + 1];
				int32 is_locked = item_list_arr[i * 3 + 2];
				citem* award_item = temp_item_set.create_item_by_template(e_server_log_add_item_mail, 0, award_template_id, award_num, is_locked);
				if (nullptr == award_item)
				{
					break;
				}
				item_list.push_back(award_item);
			}

			int32 money_list[4] = { 0,0,0,0 };
			for (int32 i = 0; i < time_limit_activity_branch_template_ptr.ParamArr2.size() / 2; ++i)
			{
				if (i >= 2)
				{
					break;
				}
				money_list[i * 2] = time_limit_activity_branch_template_ptr.ParamArr2[i * 2];
				money_list[i * 2 + 1] = time_limit_activity_branch_template_ptr.ParamArr2[i * 2 + 1];
			}

			std::string title_id = init_unit::change_i32_to_string(time_limit_activity_branch_template_ptr.ParamArr1[0]);
			std::string context_id = init_unit::change_i32_to_string(time_limit_activity_branch_template_ptr.ParamArr1[1]);
			m_player_ptr->get_mail_mgr().send_mail_to_another_player_by_system(m_player_ptr->get_unit_guid(), m_player_ptr->get_unit_info(e_role_info_server_id), title_id, context_id, money_list[0], money_list[1], money_list[2], money_list[3], item_list);
		}
	}


	void time_limit_activity_object::refresh_diamond_shop(int32 refresh_type)
	{
		// refresh_type为1是初始化刷新 refresh_type为2是玩家主动点击刷新
		if (nullptr == m_player_ptr)
		{
			return;
		}

		int32 first_template_id = get_data_info(e_time_limit_activity_info_first_branch_guid);
		const s_time_limit_activity_branch_temp& act_branch_temp = get_time_limit_activity_branch_temp();
		if (act_branch_temp.is_valid() == false || act_branch_temp.ParamArr1.size() < 4)
		{
			return;
		}

		//是否可以刷新 
		if (refresh_type == 1 && get_activity_schedule(0) <= 0)	// 检查初始化次数
		{
			set_activity_schedule(1,0);
		}
		else if (refresh_type == 2 && get_activity_schedule(1) <= 0) // 检查每日免费次数
		{
			set_activity_schedule(1, 1);
		}
		else if (refresh_type == 2 && act_branch_temp.Condition.size() >= 2)
		{
			int32 money_type = act_branch_temp.Condition[0];
			int32 money_num = act_branch_temp.Condition[1];

			if (!m_player_ptr->can_cut_money((e_money_type)money_type, money_num))
			{
				time_limit_activity_proto_buy_diamond_shop_end msg;
				msg.set_result(7);
				m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
				return;
			}

			// 消耗货币
			m_player_ptr->cut_money((e_money_type)money_type, money_num, e_server_log_cut_money_time_refresh_diamond_shop, first_template_id);
		}
		else
		{
			return;
		}

		std::vector<s_diamond_shop_info> goods_list;

		for (int32 i = 0; i < act_branch_temp.Reward.size() / 10; ++i)
		{
			s_diamond_shop_info tem_info;
			tem_info.goods_index = act_branch_temp.Reward[i * 10];
			tem_info.shop_level = act_branch_temp.Reward[i * 10 + 1];
			tem_info.total_weight = act_branch_temp.Reward[i * 10 + 2];
			tem_info.is_set = false;
			goods_list.push_back(tem_info);
		}

		int32 all_goods_num = act_branch_temp.ParamArr1[0];

		for (int32 i = 0; i < all_goods_num; ++i)
		{
			int32 cur_level = 0;
			if (i < act_branch_temp.ParamArr1[1])
			{
				cur_level = 1;
			}
			else if (i < (act_branch_temp.ParamArr1[1] + act_branch_temp.ParamArr1[2]))
			{
				cur_level = 2;
			}
			else if (i < (act_branch_temp.ParamArr1[1] + act_branch_temp.ParamArr1[2] + act_branch_temp.ParamArr1[3]))
			{
				cur_level = 3;
			}

		

			int64 total_weight = 0;
			for (int32 j = 0; j <goods_list.size(); ++j)
			{
				if (goods_list[j].shop_level == cur_level && goods_list[j].is_set == false)
				{
					total_weight += goods_list[j].total_weight;
				}
			}
			
			int32 random_value = random_gen::get_random(0, total_weight);

			int64 cur_weight = 0;
			int32 goods_index = -1;
			for (int32 j = 0; j < goods_list.size(); ++j)
			{
				if (goods_list[j].shop_level == cur_level && goods_list[j].is_set == false)
				{
					cur_weight += goods_list[j].total_weight;
					if (cur_weight >= random_value)
					{
						goods_index = goods_list[j].goods_index;
						goods_list[j].is_set = true;
						break;
					}
				}
			}

			if (goods_index == -1)
			{
				continue;
			}

			set_activity_schedule(goods_index, (2 + i * 2));
			set_activity_schedule(0, (2 + i * 2 + 1));
		}
		send_info_to_self();
		time_limit_activity_proto_buy_diamond_shop_end msg;
		msg.set_result(8);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
		
	}

	void time_limit_activity_object::buy_diamond_shop_by_index(int32 buy_index)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		time_limit_activity_proto_buy_diamond_shop_end msg;
		
		int32 first_template_id = get_data_info(e_time_limit_activity_info_first_branch_guid);
		const s_time_limit_activity_branch_temp& act_branch_temp = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(first_template_id);
		if (act_branch_temp.is_valid() == false || act_branch_temp.ParamArr1.size() < 4)
		{
			msg.set_result(1);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
			return;
		}

	
		if (buy_index < 0 || buy_index >= act_branch_temp.ParamArr1[0])
		{
			msg.set_result(2);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
			return;
		}

		if (get_activity_schedule(2 + (buy_index * 2) + 1) > 0)
		{
			msg.set_result(6);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
			return;
		}

		int32 goods_index = get_activity_schedule(2 + (buy_index * 2));

		if (goods_index < 0)
		{
			msg.set_result(3);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
			return;
		}

		int32 need_money_type = 0;
		int32 need_money_num = 0;
		int32 reward_id = 0;
		int32 reward_num = 0;
		int32 is_lock = 0;

		for (int32 i = 0; i < act_branch_temp.Reward.size() / 10; ++i)
		{
			if (act_branch_temp.Reward[i * 10] == goods_index)
			{
				reward_id = act_branch_temp.Reward[i * 10 + 4];
				reward_num = act_branch_temp.Reward[i * 10 + 5];
				is_lock = act_branch_temp.Reward[i * 10 + 6];
				need_money_type = act_branch_temp.Reward[i * 10 + 7];
				need_money_num = act_branch_temp.Reward[i * 10 + 8];
				break;
			}
		}

		if (reward_id <= 0)
		{
			msg.set_result(3);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
			return;
		}

		if (!m_player_ptr->can_cut_money((e_money_type)need_money_type, need_money_num))
		{
			msg.set_result(4);
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
			return;
		}

		std::vector<s_item_template_info> promp_item_data;
		promp_item_data.push_back({ reward_id,reward_num,is_lock });
		m_player_ptr->get_item_set().put_in_bag(e_server_log_add_buy_diamond_shop, goods_index, promp_item_data);
		m_player_ptr->cut_money((e_money_type)need_money_type, need_money_num, e_server_log_cut_money_time_buy_diamond_shop);
		set_activity_schedule(1, 2 + (buy_index * 2) + 1);

		send_info_to_self();

		msg.set_result(0);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_buy_diamond_shop_end);
	}
}
