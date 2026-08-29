/********************************************************************
created: 2022年2月23日
file base: time_limit_gift_mgr
file ext: cpp
author: zhaoyuming
*********************************************************************/

#include "internet/game.pb.h"
#include "internal/char_msg.hpp"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "time_def.hpp"
#include "time_limit_gift_mgr.h"
#include "internet/net.pb.h"
#include "utility/init_unit.h"


namespace faith
{
	time_limit_gift_mgr::time_limit_gift_mgr()
	{

	}

	time_limit_gift_mgr::~time_limit_gift_mgr()
	{

	}

	void time_limit_gift_mgr::clear_data()
	{
		m_info_list.clear();
	}

	void time_limit_gift_mgr::init_data()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
	}

	void time_limit_gift_mgr::heart_tick(const int64 & new_time)
	{
		if (m_info_list.size() > 0)
		{
			bool is_change = false;
			int32 now_time = new_time / second_tick_time;
			for (int32 i = 0; i < m_info_list.size(); ++i)
			{
				if (now_time > m_info_list[i].end_time)
				{
					m_info_list[i].state_info = e_time_limit_gift_state_end;
					is_change = true;
				}
			}
			if (is_change)
			{
				update_all_info();
			}
		}
	}

	void time_limit_gift_mgr::load_info_end(const s_time_limit_gift_db_info * dp_info, int32 data_num)
	{
		if (nullptr == dp_info)
		{
			return;
		}
		// 缓存军团信息
		m_info_list.clear();
		for (int32 i = 0; i < data_num; ++i)
		{
			m_info_list.push_back(dp_info[i]._info);
		}
		update_all_info();
	}

	void time_limit_gift_mgr::save_info(e_save_role_data_type eType)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		guid_64 role_guid = player_ref.get_unit_guid();
		cs2dp_save_time_limit_gift_to_db msg;
		msg.role_guid = role_guid;
		msg.save_type_ex = eType;
		msg.unit_array_index = m_array_index;
		msg.date_num = 0;
		int32 data_num = 0;
		for (int32 i = 0; i < m_info_list.size() && i < time_limit_gift_db_num; ++i)
		{
			msg.info_list[i].role_guid = role_guid;
			msg.info_list[i]._info = m_info_list[i];
			++data_num;
		}
		msg.date_num = data_num;
		player_ref.send_message_to_dp(&msg, e_msgindex_cs2dp_save_time_limit_gift_to_db);
	}

	void time_limit_gift_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	int32 time_limit_gift_mgr::get_receive_award_time(int32 day_num)
	{
		// 当天0点时间戳 + (天数 * 每日的秒数)
		return time_helper::get_stamp_by_hour_min(0, 0) + (day_num * day_time_second);
	}
	void time_limit_gift_mgr::update_all_info()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		game_proto_update_time_limit_gift_info info_ptr;
		for (int32 i = 0; i < m_info_list.size(); ++i)
		{
			s_time_limit_gift_info role_info = m_info_list[i];
			game_proto_time_limit_gift_info * tem_info = info_ptr.add_info_list();
			if (nullptr == tem_info)
			{
				continue;
			}
			tem_info->set_template_id(role_info.template_id);
			tem_info->set_state_info(role_info.state_info);
			tem_info->set_begin_time(role_info.begin_time);
			tem_info->set_end_time(role_info.end_time);
			tem_info->set_trigger_num(role_info.trigger_num);
			tem_info->set_buy_num(role_info.buy_num);

		}
		player_ref.send_message_to_self(&info_ptr, e_msgindex_s2c_update_time_limit_gift);

	}

	void time_limit_gift_mgr::operation_begin(int32 operation_type, int32 template_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		TimeLimitGiftTemplate* tem_ptr = GET_TEMPLATE(TimeLimitGiftTemplate, template_id);
		if (tem_ptr == nullptr || tem_ptr == nullptr)
		{
			send_operate_end(e_time_limit_gift_operation_end_type_template_error);
			return;
		}

		switch (operation_type)
		{
		case e_time_limit_gift_operation_type_activate:
		{
			if (tem_ptr->MaxTriggerNum <= 0)
			{
				send_operate_end(e_time_limit_gift_operation_end_type_template_error);
				return;
			}
			int64 cur_time = utility::get_tick_count() / 1000;
			s_time_limit_gift_info& role_info = get_info(template_id);
			if (role_info.is_valid())
			{
				// 没有触发间隔的就是无法多次触发的
				if (role_info.trigger_num < tem_ptr->MaxTriggerNum &&				// 判断触发次数
					role_info.buy_num < tem_ptr->MaxBuyTime &&					// 判断购买次数
					cur_time >(role_info.end_time + tem_ptr->TriggerTime))
				{
					role_info.trigger_num++;
					role_info.state_info = e_time_limit_gift_state_begin;
					role_info.begin_time = cur_time;
					role_info.end_time = cur_time + tem_ptr->LifeTime;
					update_all_info();
					send_operate_end(e_time_limit_gift_operation_end_type_activate_succeed, template_id);
					return;
				}
			}
			else
			{
				s_time_limit_gift_info tem_info;
				tem_info.template_id = template_id;
				tem_info.state_info = e_time_limit_gift_state_begin;
				tem_info.begin_time = cur_time;
				tem_info.trigger_num = 1;
				role_info.buy_num = 0;
				tem_info.end_time = cur_time + tem_ptr->LifeTime;
				m_info_list.push_back(tem_info);
				update_all_info();
				send_operate_end(e_time_limit_gift_operation_end_type_activate_succeed, template_id);
				return;
			}
		}
		break;
		case e_time_limit_gift_operation_type_buy:
		{
			s_time_limit_gift_info& role_info = get_info(template_id);
			if (false == role_info.is_valid())
			{
				send_operate_end(e_time_limit_gift_operation_end_typed_error_01);
				return;
			}
			if (role_info.state_info != e_time_limit_gift_state_begin)
			{
				return;
			}
			// 判断是否过了购买时间
			int64 cur_time = utility::get_tick_count() / 1000;
			if (cur_time > role_info.end_time)
			{
				send_operate_end(e_time_limit_gift_operation_end_type_time_error);
				return;
			}
			if (tem_ptr->MoneyList.size() != 2)
			{
				send_operate_end(e_time_limit_gift_operation_end_type_template_error);
				return;
			}

			if (!player_ref.can_cut_money((e_money_type)tem_ptr->MoneyList[0], tem_ptr->MoneyList[1]))
			{
				send_operate_end(e_time_limit_gift_operation_end_type_money_error);
				return;
			}

			// 消耗货币
			player_ref.cut_money((e_money_type)tem_ptr->MoneyList[0], tem_ptr->MoneyList[1], e_server_log_cut_money_time_limit_gift, template_id);

			if (tem_ptr->Reward.size() > 0)
			{
				// 发放奖励
				player_ref.get_item_set().put_in_bag(e_server_log_add_item_time_limit_gift, template_id, init_unit::reward_regional_diff(tem_ptr->Reward[0]), e_bag_type_none);

			}
			
			// 设置商品状态
			role_info.state_info = e_time_limit_gift_state_end;

			// 增加购买次数
			role_info.buy_num++;

			// 同步购买信息
			update_all_info();
			
			// 发送操作成功
			send_operate_end(e_time_limit_gift_operation_end_type_buy_succeed);
		}
		break;
		default:
			break;
		}
	}

	void time_limit_gift_mgr::send_operate_end(e_time_limit_gift_operation_end_type end_type, int32 template_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		game_proto_time_limit_gift_operation_end msg;
		msg.set_result((int32)end_type);
		msg.set_template_id(template_id);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_time_limit_gift_operation_end);
	}

	s_time_limit_gift_info& time_limit_gift_mgr::get_info(int32 template_id)
	{
		for (int32 i = 0; i < m_info_list.size(); ++i)
		{
			if (m_info_list[i].template_id == template_id)
			{
				return m_info_list[i];
			}
		}
		return m_empty_info;
	}

	void time_limit_gift_mgr::apply_recharge(int32 goods_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		for (int32 i = 0; i < m_info_list.size(); ++i)
		{
			TimeLimitGiftTemplate* tem_ptr = GET_TEMPLATE(TimeLimitGiftTemplate, m_info_list[i].template_id);
			if (tem_ptr == nullptr)
			{
				continue;
			}
			if (tem_ptr->ConsumeType == 2 && tem_ptr->MoneyList.size() > 0)
			{
				std::vector<int32> reward_list = init_unit::reward_regional_diff(tem_ptr->MoneyList[0]);
				if (reward_list.size() <= 0 || reward_list[0] != goods_id)
				{
					continue;
				}
				RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, goods_id);
				if (nullptr == recharge_template_ptr || recharge_template_ptr->MoneyGetArray.size() < 2)
				{
					continue;
				}

				s_time_limit_gift_info& role_info = m_info_list[i];
				if (role_info.state_info != e_time_limit_gift_state_begin)
				{
					continue;
				}
				// 判断是否过了购买时间
				int64 cur_time = utility::get_tick_count() / 1000;
				if (cur_time > role_info.end_time)
				{
					send_operate_end(e_time_limit_gift_operation_end_type_time_error);
					return;
				}

				if (!player_ref.can_cut_money(e_money_type(recharge_template_ptr->MoneyGetArray[0]), recharge_template_ptr->MoneyGetArray[1]))
				{
					continue;
				}


				player_ref.cut_money(e_money_type(recharge_template_ptr->MoneyGetArray[0]), recharge_template_ptr->MoneyGetArray[1], e_server_log_cut_money_time_limit_gift);

				// 发放奖励
				if (tem_ptr->Reward.size() > 0)
				{
					player_ref.get_item_set().put_in_bag(e_server_log_add_item_time_limit_gift, goods_id, init_unit::reward_regional_diff(tem_ptr->Reward[0]));
				}

				// 增加购买次数
				role_info.buy_num++;

				// 设置商品状态
				role_info.state_info = e_time_limit_gift_state_end;

				// 同步购买信息
				update_all_info();

				// 发送操作成功
				send_operate_end(e_time_limit_gift_operation_end_type_buy_succeed);

				return;
			}
		}
	}

	void time_limit_gift_mgr::check_target_map(int32 map_template_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		faith::template_manager::template_type* template_list = template_manager::get_instance().get_templates(e_TimeLimitGiftTemplate);
		if (nullptr == template_list)
		{
			return;
		}
		template_manager::template_type::iterator it;
		for (it = template_list->begin(); it != template_list->end(); ++it)//遍历表中每一行
		{
			TimeLimitGiftTemplate* template_ptr = (TimeLimitGiftTemplate*)it->second;
			if (template_ptr == nullptr)
			{
				continue;
			}
			if (template_ptr->TriggerType != 1)
			{
				continue;
			}
			if (template_ptr->MapWinList.size() <= 0)
			{
				continue;
			}
			for (int32 i = 0; i < template_ptr->MapWinList.size(); ++i)
			{
				if (map_template_id == template_ptr->MapWinList[i])
				{
					// 检查任务
					if (template_ptr->NeedMission.size() >= 2)
					{
						cmission* mission_ptr = player_ref.get_mission_mgr().find_mission_by_index(e_mission_slot_main);
						if (nullptr == mission_ptr)
						{
							continue;
						}
						MissionTemplate* mission_template_ptr = mission_ptr->get_mission_template_ptr();
						if (mission_template_ptr == nullptr)
						{
							continue;
						}
						int32 cur_mission_id = mission_template_ptr->attribute_id;
						if (cur_mission_id < template_ptr->NeedMission[0] || cur_mission_id > template_ptr->NeedMission[1])
						{
							continue;
						}
					}

					// 检查等级
					if (template_ptr->NeedLevel.size() >= 2)
					{
						int32 player_level = player_ref.get_unit_info(e_role_info_exp_level);
						if (player_level < template_ptr->NeedLevel[0] || player_level > template_ptr->NeedLevel[1])
						{
							continue;
						}
					}
					game_proto_initiative_time_limit_gift msg;
					msg.set_template_id(template_ptr->attribute_id);
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_initiative_time_limit_gift);
					return;
				}
			}
		}
	}

	
}