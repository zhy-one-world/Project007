/********************************************************************
created: 2022年8月22日
file base: star_trip_mgr
file ext: cpp
author: zhaoyuming
*********************************************************************/

#include "internet/net.pb.h"
#include "internet/time_limit_activity.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "star_trip_mgr.h"
#include "time_limit_activity_object.h"
#include "utility/init_unit.h"



namespace hld
{
	star_trip_mgr::star_trip_mgr()
	{

	}

	star_trip_mgr::~star_trip_mgr()
	{

	}

	void star_trip_mgr::clear_data()
	{
		m_target_mission_time = 0;
		m_init = false;
		m_need_tip = 0;
		m_goods_list.clear();
		m_mission_list.clear();
	}

	void star_trip_mgr::init_data()
	{

	}

	void star_trip_mgr::heart_tick(const int64 & new_time)
	{
	}

	void star_trip_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	void star_trip_mgr::refresh_today_mission()
	{
		for (int32 i = 0; i < m_mission_list.size(); ++i)
		{
			if (m_mission_list[i].mission_type == 1)
			{
				m_mission_list[i].target_num = 0;
				m_mission_list[i].is_get = 0;
			}
		}
		send_all_activity_info_to_client();
	}

	void star_trip_mgr::set_limit_activiy_info()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid())
		{
			return;
		}

		for (int32 i = 0; i < m_goods_list.size(); ++i)
		{
			if (i >= time_limit_activity_schedule_num - 4)
			{
				break;
			}
			activity_ptr.set_activity_schedule(m_goods_list[i].buy_num, i + 4);
		}
		
		std::vector<int32> mission_state_list;
		mission_state_list.clear();
		for (int32 i = 0; i < m_mission_list.size(); ++i)
		{
			mission_state_list.push_back(m_mission_list[i].target_num);
			mission_state_list.push_back(m_mission_list[i].is_get);
		}

		xstring temp_str;
		init_unit::parse_vector_to_char(temp_str, mission_state_list);
		activity_ptr.set_activity_schedule_str(temp_str);
	}

	void star_trip_mgr::init_activiy_info()
	{
		
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid())
		{
			return;
		}

		const s_time_limit_activity_branch_temp& act_branch_temp = activity_ptr.get_time_limit_activity_branch_temp();
		if (act_branch_temp.is_valid() == false)
		{
			return;
		}
		m_init = true;
		m_goods_list.clear();
		for (int32 i = 0; i < act_branch_temp.Reward.size() / 5; ++i)
		{
			s_star_trip_goods_info tem_info;
			tem_info.item_id = act_branch_temp.Reward[i * 5];
			tem_info.item_num = act_branch_temp.Reward[i * 5 + 1];
			tem_info.is_lock = act_branch_temp.Reward[i * 5 + 2];
			tem_info.need_money = act_branch_temp.Reward[i * 5 + 3];
			tem_info.max_num = act_branch_temp.Reward[i * 5 + 4];
			tem_info.buy_num = activity_ptr.get_activity_schedule(i + 4);
			m_goods_list.push_back(tem_info);
		}

		int32 today_mission_num = act_branch_temp.ParamArr1.size() / 4;
		int32 activity_mission_num = act_branch_temp.ParamArr2.size() / 4;

		std::vector<int32> mission_state_list;
		mission_state_list.clear();
		init_unit::parse_char_to_vector(mission_state_list, activity_ptr.get_activity_schedule_str());

		for (int32 i = 0; i < today_mission_num; ++i)
		{
			s_star_trip_mission_info tem_info;
			tem_info.mission_type = 1;
			tem_info.target_type = act_branch_temp.ParamArr1[i * 4];
			tem_info.target_param = act_branch_temp.ParamArr1[i * 4 + 1];
			tem_info.finish_num = act_branch_temp.ParamArr1[i * 4 + 2];
			tem_info.award_num = act_branch_temp.ParamArr1[i * 4 + 3];

			if (mission_state_list.size() / 2 > i)
			{
				tem_info.target_num = mission_state_list[i * 2];
				tem_info.is_get = mission_state_list[i * 2 + 1];
			}
			else
			{
				tem_info.target_num = 0;
				tem_info.is_get = 0;
			}
			m_mission_list.push_back(tem_info);
		}

		for (int32 i = 0; i < activity_mission_num; ++i)
		{
			s_star_trip_mission_info tem_info;
			tem_info.mission_type = 2;
			tem_info.target_type = act_branch_temp.ParamArr2[i * 4];
			tem_info.target_param = act_branch_temp.ParamArr2[i * 4 + 1];
			tem_info.finish_num = act_branch_temp.ParamArr2[i * 4 + 2];
			tem_info.award_num = act_branch_temp.ParamArr2[i * 4 + 3];

			if (mission_state_list.size() / 2 > i + today_mission_num)
			{
				tem_info.target_num = mission_state_list[(i + today_mission_num) * 2];
				tem_info.is_get = mission_state_list[(i + today_mission_num) * 2 + 1];
			}
			else
			{
				tem_info.target_num = 0;
				tem_info.is_get = 0;
			}
			m_mission_list.push_back(tem_info);
		}
		if (act_branch_temp.Condition.size() > 0)
		{
			m_target_mission_time = act_branch_temp.Condition[0] + activity_ptr.get_data_info(e_time_limit_activity_info_start_time);
		}

		m_need_tip = activity_ptr.get_activity_schedule(2);

		send_all_activity_info_to_client();
	}

	void star_trip_mgr::send_all_activity_info_to_client()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid())
		{
			return;
		}

		time_limit_activity_proto_send_star_trip_info_all msg;
		msg.set_cur_source(activity_ptr.get_activity_schedule(1));
		msg.set_need_tip(m_need_tip);
		msg.set_target_day(m_target_mission_time);

		for (int32 i = 0; i < m_goods_list.size(); ++i)
		{
			time_limit_activity_proto_star_trip_goods_info * goods_info = msg.add_goods_list();
			if (nullptr != goods_info)
			{
				goods_info->set_show_index(i);
				goods_info->set_item_id(m_goods_list[i].item_id);
				goods_info->set_item_num(m_goods_list[i].item_num);
				goods_info->set_is_lock(m_goods_list[i].is_lock);
				goods_info->set_max_num(m_goods_list[i].max_num);
				goods_info->set_buy_num(m_goods_list[i].buy_num);
				goods_info->set_need_money(m_goods_list[i].need_money);
			}
		}

		for (int32 i = 0; i < m_mission_list.size(); ++i)
		{
			time_limit_activity_proto_star_trip_mission_info * tem_mission_info = msg.add_mission_list();
			if (nullptr != tem_mission_info)
			{
				tem_mission_info->set_show_index(i);
				tem_mission_info->set_mission_type(m_mission_list[i].mission_type);
				tem_mission_info->set_target_type(m_mission_list[i].target_type);
				tem_mission_info->set_target_param(m_mission_list[i].target_param);
				tem_mission_info->set_target_num(m_mission_list[i].target_num);
				tem_mission_info->set_finish_num(m_mission_list[i].finish_num);
				tem_mission_info->set_award_num(m_mission_list[i].award_num);
				tem_mission_info->set_is_get(m_mission_list[i].is_get);
			}
		}

		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_star_trip_info_all);
	}

	void star_trip_mgr::activity_operate(int32 operate_type, int32 parame1, int32 parame2)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		//e_star_trip_operate_end_no_target

		time_limit_activity_proto_star_trip_operate_end msg;
		msg.set_operate_type(operate_type);
		msg.set_parame1(parame1);
		msg.set_parame2(parame2);

		int32 operate_end = 0;
		switch (operate_type)
		{
		case s_star_trip_operate_type_buy:
		{
			operate_end = buy_item(parame1);
		}
		break;
		case s_star_trip_operate_type_mission:
		{
			operate_end = finish_mission(parame1);
		}
		break;
		case s_star_trip_operate_type_set_tip:
		{
			m_need_tip = parame1;
			operate_end = e_star_trip_operate_end_set_tip_finish;
		}
		break;
		default:
			break;
		}

		msg.set_result(operate_end);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_star_trip_operate_end);
	}

	void star_trip_mgr::target_mission(int32 mission_type, int32 parame1)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid() || activity_ptr.is_open() == false)
		{
			return;
		}

		int64 time_now = utility::get_tick_count();
		if (time_now / 1000 > m_target_mission_time)
		{
			return;
		}

		for (int32 i = 0; i < m_mission_list.size(); ++i)
		{
			if (m_mission_list[i].target_type != mission_type)
			{
				continue;
			}
			bool is_update = false;
			switch (mission_type)
			{
			case e_star_trip_target_type_buy_goods:
			case e_star_trip_target_type_convert:
			case e_star_trip_target_type_join_activity:
			case e_star_trip_target_type_map_finish:
			{
				if (parame1 == m_mission_list[i].target_param)
				{
					if (m_mission_list[i].target_num >= m_mission_list[i].finish_num)
					{
						is_update = false;
					}
					else
					{
						is_update = true;
					}
					m_mission_list[i].add_target_num();
				}
			}
			break;
			case e_star_trip_target_type_buy_single_boss:
			case e_star_trip_target_type_legion_donate:
			case e_star_trip_target_type_buy_today_gift:
			case e_star_trip_target_type_kill_single_boss:
			case e_star_trip_target_type_kill_home_boss:
			case e_star_trip_target_type_recharge:
			case e_star_trip_target_type_bid:
			{
				if (m_mission_list[i].target_num >= m_mission_list[i].finish_num)
				{
					is_update = false;
				}
				else
				{
					is_update = true;
				}
				m_mission_list[i].add_target_num();
			}
			break;
			case e_star_trip_target_type_treasure:
			{
				if (m_mission_list[i].target_num >= m_mission_list[i].finish_num)
				{
					is_update = false;
				}
				else
				{
					is_update = true;
				}
				m_mission_list[i].target_num += parame1;
			}
			break;
			default:
				break;
			}

			if (is_update)
			{
				time_limit_activity_proto_send_star_trip_mission_info_one msg;
				time_limit_activity_proto_star_trip_mission_info * tem_mission_info = msg.mutable_mission_info();
				if (nullptr != tem_mission_info)
				{
					tem_mission_info->set_show_index(i);
					tem_mission_info->set_mission_type(m_mission_list[i].mission_type);
					tem_mission_info->set_target_type(m_mission_list[i].target_type);
					tem_mission_info->set_target_param(m_mission_list[i].target_param);
					tem_mission_info->set_target_num(m_mission_list[i].target_num);
					tem_mission_info->set_finish_num(m_mission_list[i].finish_num);
					tem_mission_info->set_award_num(m_mission_list[i].award_num);
					tem_mission_info->set_is_get(m_mission_list[i].is_get);
				}
				msg.set_cur_source(activity_ptr.get_activity_schedule(1));
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_star_trip_mission_info_one);
			}
		}	
	}

	void star_trip_mgr::target_mission_by_mission_type(e_mission_end_type mission_type, int32 parame1, int32 parame2)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid() || activity_ptr.is_open() == false)
		{
			return;
		}

		int64 time_now = utility::get_tick_count();
		if (time_now / 1000 > m_target_mission_time)
		{
			return;
		}

		switch (mission_type)
		{
		case e_mission_end_type_purchase_goods:
		{
			target_mission(e_star_trip_target_type_buy_goods, parame1);
		}
		break;
		case e_mission_end_type_map:
		{
			target_mission(e_star_trip_target_type_map_finish, parame1);
		}
		break;
		case e_mission_end_type_kill_any_home_boss:
		{
			target_mission(e_star_trip_target_type_kill_home_boss);
		}
		break;
		case e_mission_end_type_kill_any_single_boss:
		{
			target_mission(e_star_trip_target_type_kill_single_boss);
		}
		break;
		case e_mission_end_type_legion_donate:
		{
			target_mission(e_star_trip_target_type_legion_donate);
		}
		break;
		case e_mission_end_type_transform:
		{
			target_mission(e_star_trip_target_type_convert, parame1);
		}
		break;
		default:
			break;
		}
	}

	int32 star_trip_mgr::buy_item(int32 item_index)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return e_star_trip_operate_end_error1;
		}

		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid())
		{
			return e_star_trip_operate_end_template_error;
		}

		if (item_index < 0 || item_index > m_goods_list.size())
		{
			return e_star_trip_operate_end_template_error;
		}
		
		// 获取商品信息
		s_star_trip_goods_info &buy_goods = m_goods_list[item_index];
		if (buy_goods.item_id <= 0)
		{
			return e_star_trip_operate_end_template_error;
		}

		// 判断购买次数
		if (!buy_goods.can_buy())
		{
			return e_star_trip_operate_end_no_buy_num;
		}

		// 判断积分
		int32 cur_source = activity_ptr.get_activity_schedule(1);
		int32 end_source = cur_source - buy_goods.need_money;
		if (end_source < 0)
		{
			return e_star_trip_operate_end_no_money;
		}

		// 创建物品
		citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_buy_star_trip, buy_goods.need_money, buy_goods.item_id, buy_goods.item_num, buy_goods.is_lock);
		if (temp_item == nullptr)
		{
			return e_star_trip_operate_end_no_item;
		}

		// 增加购买次数
		buy_goods.add_buy();
		// 将物品放入背包
		player_ref.get_item_set().put_in_bag(temp_item);
		// 减少积分
		activity_ptr.set_activity_schedule(end_source, 1);

		time_limit_activity_proto_send_star_trip_goods_info_one msg;
		time_limit_activity_proto_star_trip_goods_info * goods_info = msg.mutable_goods_info();
		if (nullptr != goods_info)
		{
			goods_info->set_show_index(item_index);
			goods_info->set_item_id(buy_goods.item_id);
			goods_info->set_item_num(buy_goods.item_num);
			goods_info->set_is_lock(buy_goods.is_lock);
			goods_info->set_max_num(buy_goods.max_num);
			goods_info->set_buy_num(buy_goods.buy_num);
			goods_info->set_need_money(buy_goods.need_money);
		}
		msg.set_cur_source(end_source);

		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_star_trip_goods_info_one);
		return e_star_trip_operate_end_buy_finish;
	}

	int32 star_trip_mgr::finish_mission(int32 missione_index)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return e_star_trip_operate_end_error1;
		}

		time_limit_activity_object& activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_star_trip);
		if (false == activity_ptr.is_valid())
		{
			return e_star_trip_operate_end_template_error;
		}

		if (missione_index < 0 || missione_index > m_mission_list.size())
		{
			return e_star_trip_operate_end_template_error;
		}

		int64 time_now = utility::get_tick_count();
		if (time_now / 1000 > m_target_mission_time)
		{
			return e_star_trip_operate_end_no_time;
		}

		// 获取商任务信息
		s_star_trip_mission_info &mission_info = m_mission_list[missione_index];
		if (mission_info.award_num <= 0)
		{
			return e_star_trip_operate_end_template_error;
		}

		if (mission_info.is_get > 0)
		{
			return e_star_trip_operate_end_is_get;
		}

		// 判断购买次数
		if (false == mission_info.is_finish())
		{
			return e_star_trip_operate_end_no_target;
		}

		mission_info.is_get = 1;
		activity_ptr.add_activity_schedule(mission_info.award_num, 1);
		
		time_limit_activity_proto_send_star_trip_mission_info_one msg;
		time_limit_activity_proto_star_trip_mission_info * tem_mission_info = msg.mutable_mission_info();
		if (nullptr != tem_mission_info)
		{
			tem_mission_info->set_show_index(missione_index);
			tem_mission_info->set_mission_type(mission_info.mission_type);
			tem_mission_info->set_target_type(mission_info.target_type);
			tem_mission_info->set_target_param(mission_info.target_param);
			tem_mission_info->set_target_num(mission_info.target_num);
			tem_mission_info->set_finish_num(mission_info.finish_num);
			tem_mission_info->set_award_num(mission_info.award_num);
			tem_mission_info->set_is_get(mission_info.is_get);
		}
		msg.set_cur_source(activity_ptr.get_activity_schedule(1));
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_star_trip_mission_info_one);
		return e_star_trip_operate_end_target_mission_finish;
	}

	bool star_trip_mgr::get_is_init()
	{
		return m_init;
	}

}