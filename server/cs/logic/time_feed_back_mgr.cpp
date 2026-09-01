/********************************************************************
created: 2022年2月23日
file base: time_feed_back_mgr
file ext: cpp
author: zhaoyuming
*********************************************************************/

#include "time_feed_back_mgr.h"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "server_log.hpp"
#include "time_def.hpp"
#include "logic/player.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "time_limit_activity_temp_mgr.hpp"
#include "internal/char_msg.hpp"


namespace faith
{
	time_feed_back_mgr::time_feed_back_mgr()
	{

	}

	time_feed_back_mgr::~time_feed_back_mgr()
	{

	}

	void time_feed_back_mgr::clear_data()
	{
		m_info_list.clear();
	}

	void time_feed_back_mgr::init_data()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
	}

	void time_feed_back_mgr::heart_tick(const int64 & new_time)
	{
		if (m_info_list.size() > 0)
		{
			std::vector<s_time_feed_back_info> tem_list = m_info_list;
			m_info_list.clear();
			int32 now_time = new_time / second_tick_time;
			for (int32 i = 0; i < tem_list.size(); ++i)
			{
				s_time_feed_back_info tem_info = tem_list[i];
				if (now_time > tem_info.end_time)
				{
					player& player_ref = unit_man::get_player(m_array_index);
					if (player_ref.is_valid() && tem_info.info_state == e_time_feed_back_info_state_buy)
					{
						std::vector<citem*> item_list;
						std::string title_id = "90306377";
						std::string context_id = "90306378";
						player_ref.get_mail_mgr().send_mail_to_another_player_by_system(player_ref.get_unit_guid(), player_ref.get_unit_info(e_role_info_server_id), title_id, context_id, tem_info.money_type, tem_info.money_num, 0, 0, item_list);
					}
					tem_info.info_state = e_time_feed_back_info_state_end;
				}
				if (tem_info.info_state >= e_time_feed_back_info_state_end)
				{
					continue;
				}
				m_info_list.push_back(tem_info);
			}
		}
	}

	void time_feed_back_mgr::load_info_end(const s_time_feed_back_db_info * dp_info, int32 data_num)
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
	}

	void time_feed_back_mgr::save_info(e_save_role_data_type eType)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		guid_64 role_guid = player_ref.get_unit_guid();
		cs2dp_save_time_feed_back_to_db msg;
		msg.role_guid = role_guid;
		msg.save_type_ex = eType;
		msg.unit_array_index = m_array_index;
		msg.date_num = 0;
		int32 data_num = 0;
		for (int32 i = 0; i < m_info_list.size() && i < time_feed_back_to_db_num; ++i)
		{
			msg.info_list[i].role_guid = role_guid;
			msg.info_list[i]._info = m_info_list[i];
			++data_num;
		}
		msg.date_num = data_num;
		player_ref.send_message_to_dp(&msg, e_msgindex_cs2dp_save_time_feed_back_to_db);
	}

	void time_feed_back_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	int32 time_feed_back_mgr::get_receive_award_time(int32 day_num)
	{
		// 当天0点时间戳 + (天数 * 每日的秒数)
		return time_helper::get_stamp_by_hour_min(0, 0) + (day_num * day_time_second);
	}
	void time_feed_back_mgr::send_time_feed_back_info()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		game_proto_send_time_feed_back_info_list info_ptr;
		for (int32 i = 0; i < m_info_list.size(); ++i)
		{
			s_time_feed_back_info back_info = m_info_list[i];
			game_proto_send_time_feed_back_info* tem_info = info_ptr.add_info_list();
			if (nullptr == tem_info)
			{
				continue;
			}
			tem_info->set_template_id(back_info.template_id);
			tem_info->set_sub_template_id(back_info.sub_template_id);
			tem_info->set_info_state(back_info.info_state);
			tem_info->set_get_time(back_info.get_time);
			tem_info->set_end_time(back_info.end_time);
			tem_info->set_money_type(back_info.money_type);
			tem_info->set_money_num(back_info.money_num);
		}
		player_ref.send_message_to_self(&info_ptr, e_msgindex_s2c_send_time_feed_back_info);

	}

	void time_feed_back_mgr::time_feed_back_operation(int32 operation_type, int32 mian_id, int32 sub_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& activity_object = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(mian_id);
		if (activity_object.is_valid() == false)
		{
			send_operate_end(e_time_feed_back_operation_end_template_error);
			return;
		}
		const s_time_limit_activity_branch_temp& sub_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(sub_id);
		if (sub_template_ptr.ActGuid <= 0)
		{
			send_operate_end(e_time_feed_back_operation_end_template_error);
			return;
		}
		
		switch (operation_type)
		{
		case e_time_feed_back_operation_type_buy:
		{
			if (sub_template_ptr.Reward.size() != 2 || sub_template_ptr.ParamArr1.size() != 3 || sub_template_ptr.ParamArr2.size() != 2 || sub_template_ptr.Condition.size() != 1 || sub_template_ptr.ParamArr1[0] != 1)
			{
				send_operate_end(e_time_feed_back_operation_end_template_error);
				return;
			}

			int32 cur_money_type = sub_template_ptr.ParamArr1[1];
			int32 cur_money_num = sub_template_ptr.ParamArr1[2];

			// 检查货币是否足够
			if (!player_ref.can_cut_money(e_money_type(cur_money_type), cur_money_num))
			{
				send_operate_end(e_time_feed_back_operation_end_money_error);
				return;
			}

			// 消耗货币
			player_ref.cut_money((e_money_type)cur_money_type, cur_money_num, e_server_log_cut_money_time_feed_back, mian_id, sub_id);
			
			player_ref.get_item_set().put_in_bag(e_server_log_add_item_time_feed_back, sub_id, sub_template_ptr.Reward);
			// 记录购买信息
			s_time_feed_back_info add_info;
			add_info.template_id = mian_id;
			add_info.sub_template_id = sub_id;
			add_info.info_state = e_time_feed_back_info_state_buy;
			add_info.get_time = get_receive_award_time(sub_template_ptr.Condition[0]);
			add_info.end_time = activity_object.get_data_info(e_time_limit_activity_info_end_time);
			add_info.money_type = sub_template_ptr.ParamArr2[0];
			add_info.money_num = sub_template_ptr.ParamArr2[1];
			m_info_list.push_back(add_info);

			// 同步购买信息
			send_time_feed_back_info();
			
			// 发送操作结果
			send_operate_end(e_time_feed_back_operation_end_buy_succeed);
		}
		break;
		case e_time_feed_back_operation_type_receive_award:
		{
			s_time_feed_back_info& back_info = get_info_by_id(mian_id, sub_id);
			if (false == back_info.is_valid())
			{
				send_operate_end(e_time_feed_back_operation_end_error_01);
				return;
			}

			// 判断是否到领取时间
			int64 cur_time = utility::get_tick_count() / 1000;
			if (cur_time < back_info.get_time)
			{
				send_operate_end(e_time_feed_back_operation_end_time_error);
				return;
			}

			// 增加货币
			player_ref.add_money_or_exp((e_money_type)back_info.money_type, back_info.money_num, e_server_log_add_money_time_feed_back, sub_id);
			
			// 设置商品状态
			back_info.info_state = e_time_feed_back_info_state_receive_award;

			// 同步购买信息
			send_time_feed_back_info();
			
			// 发送操作成功
			send_operate_end(e_time_feed_back_operation_end_get_succeed);
		}
		break;
		default:
			break;
		}
	}

	void time_feed_back_mgr::send_operate_end(e_time_feed_back_operation_end_type end_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		game_proto_time_feed_back_operation_end msg;
		msg.set_result((int32)end_type);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_time_feed_back_operation_end);
	}

	void time_feed_back_mgr::apply_recharge(int32 goods_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		for (int32 i = e_time_limit_activity_type_time_feed_back1; i <= e_time_limit_activity_type_time_feed_back3; ++i)
		{
			int32 act_guid = time_limit_activity_temp_mgr::get_instance().get_new_limit_act_with_act_type(i);
			if (act_guid <= 0)
			{
				continue;
			}

			time_limit_activity_object& activity_object = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(act_guid);
			if (activity_object.is_valid() == false)
			{
				continue;
			}
			int32 branch_template_id = activity_object.get_branch_template_id();
			//最多循环50次
			for (int32 j = 0; j < 20; j++)
			{
				int32 sub_template_id = branch_template_id + j;
				if (!activity_object.is_same_cycle(sub_template_id))
				{
					break;
				}
				// 有数据说明已经买过了不需要再购买了
				if (get_info_by_id(act_guid, sub_template_id).is_valid())
				{
					continue;
				}
				const s_time_limit_activity_branch_temp& sub_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(sub_template_id);
				if (sub_template_ptr.ParamArr1.size() >= 3 && sub_template_ptr.ParamArr1[0] == 2 && sub_template_ptr.ParamArr1[1] == goods_id)
				{
					RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, goods_id);
					if (nullptr == recharge_template_ptr || recharge_template_ptr->MoneyGetArray.size() < 2)
					{
						continue;
					}

					if (!player_ref.can_cut_money(e_money_type(recharge_template_ptr->MoneyGetArray[0]), recharge_template_ptr->MoneyGetArray[1]))
					{
						continue;
					}
					player_ref.cut_money(e_money_type(recharge_template_ptr->MoneyGetArray[0]), recharge_template_ptr->MoneyGetArray[1], e_server_log_cut_money_time_feed_back);
					player_ref.get_item_set().put_in_bag(e_server_log_add_item_time_feed_back, 0, sub_template_ptr.Reward, e_bag_type_none);

					// 记录购买信息
					s_time_feed_back_info add_info;
					add_info.template_id = act_guid;
					add_info.sub_template_id = sub_template_id;
					add_info.info_state = e_time_feed_back_info_state_buy;
					add_info.get_time = get_receive_award_time(sub_template_ptr.Condition[0]);
					add_info.end_time = activity_object.get_data_info(e_time_limit_activity_info_end_time);
					add_info.money_type = sub_template_ptr.ParamArr2[0];
					add_info.money_num = sub_template_ptr.ParamArr2[1];
					m_info_list.push_back(add_info);

					// 同步购买信息
					send_time_feed_back_info();

					// 发送操作结果
					send_operate_end(e_time_feed_back_operation_end_buy_succeed);
					return;
				}
			}

		}
		
	
		
	}

	s_time_feed_back_info& time_feed_back_mgr::get_info_by_id(int32 main_id, int32 sub_id)
	{
		for (int32 i = 0; i < m_info_list.size(); ++i)
		{
			if (m_info_list[i].template_id == main_id && m_info_list[i].sub_template_id == sub_id)
			{
				return m_info_list[i];
			}
		}
		return m_empty_info;
	}

	
}