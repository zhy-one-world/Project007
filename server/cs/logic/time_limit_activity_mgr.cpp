/********************************************************************
created: 2019年03月20日
file base: time_limit_activity_mgr
file ext: cpp
author: wangdi

purpose: time limit activity
*********************************************************************/

#include "connection_mgr.hpp"
#include "time_limit_activity_mgr.h"
#include "logic/player.hpp"
#include "logic/item_set.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"
#include "internet/time_limit_activity.pb.h"
#include "internet/net.pb.h"
#include "time_limit_activity_temp_mgr.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/game.pb.h"
#include "internal/time_limit_activity_msg.hpp"
#include "internal/core.hpp"

namespace faith
{
	time_limit_activity_mgr::time_limit_activity_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	time_limit_activity_mgr::~time_limit_activity_mgr()
	{
	}

	void time_limit_activity_mgr::clear_data()
	{
		m_activity_obj.clear();
		m_empty_act_obj.clear_data();
		m_act_obj_vec.clear();
	}

	void time_limit_activity_mgr::heart_tick(const int64& new_time)
	{
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end();)
		{
			auto temp_it = ite++;
			temp_it->second.heart_tick(new_time);
		}
	}

	void time_limit_activity_mgr::init_all_act()
	{
		add_new_act_with_new_temp();
		delete_old_act_with_new_temp();
		init_template_id_all();
		send_open_activity_all();
		check_refresh_charge_value();
		send_limit_time_notice_info();
	}

	void time_limit_activity_mgr::change_added_drop_item()
	{
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(e_time_limit_activity_type_added_drop);
		if (!activity_obj_ptr.is_valid())
		{
			return;
		}

		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = activity_obj_ptr.get_time_limit_activity_branch_temp();
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr1.size() != time_limit_act_branch_temp.ParamArr2.size()
			|| time_limit_act_branch_temp.ParamArr1.size() != time_limit_act_branch_temp.ParamArr3.size())
		{
			return;
		}

		DropTemplate* drop_template_ptr = GET_TEMPLATE(DropTemplate, time_limit_activity_added_drop_drop_template_id);
		if (nullptr == drop_template_ptr || drop_template_ptr->IDs.size() != drop_template_ptr->Weights.size() || drop_template_ptr->IDs.size() != drop_template_ptr->Bounds.size())
		{
			return;
		}
		for (int32 i = 0; i <= drop_template_ptr->IDs.size(); i++)
		{
			if (i < time_limit_act_branch_temp.ParamArr1.size())
			{
				drop_template_ptr->IDs[i] = time_limit_act_branch_temp.ParamArr1[i];
				drop_template_ptr->Weights[i] = time_limit_act_branch_temp.ParamArr2[i];
				drop_template_ptr->Bounds[i] = time_limit_act_branch_temp.ParamArr3[i];
			}
			else
			{
				drop_template_ptr->IDs[i] = 0;
				drop_template_ptr->Weights[i] = 0;
				drop_template_ptr->Bounds[i] = 0;
			}
		}
	}

	void time_limit_activity_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			ite->second.set_player_ptr(player_ptr);
		}
	}

	void time_limit_activity_mgr::refresh_by_zero(const int64& time_now)
	{
		add_new_act_with_new_temp();
		delete_old_act_with_new_temp();
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			ite->second.refresh_by_zero(time_now);
			reset_ranking_value((e_time_limit_activity_type)ite->second.get_act_type());//只有排行榜才会刷
		}
		send_open_activity_all();
		add_activity_schedule(e_time_limit_activity_type_total_landing, 1);
		add_activity_schedule(e_time_limit_activity_type_new_total_landing, 1);
	}

	void time_limit_activity_mgr::send_open_activity_all()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		time_limit_activity_proto_all_open_activity_message msg;
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			const s_time_limit_activity_temp& activity_template_ptr = ite->second.get_time_limit_activity_temp();
			if (ite->second.is_open() && activity_template_ptr.ActGuid > 0 && activity_template_ptr.ParamArr1.size() > 0)
			{
				time_limit_activity_proto_one_open_activity_message* one_activity_msg = msg.add_activity_message();
				one_activity_msg->set_activity_temp_id(activity_template_ptr.ActGuid);
				one_activity_msg->set_activity_type(ite->second.get_act_type());
				one_activity_msg->set_activity_start_time(ite->second.get_data_info(e_time_limit_activity_info_start_time));
				one_activity_msg->set_activity_end_time(ite->second.get_data_info(e_time_limit_activity_info_end_time));
				one_activity_msg->set_activity_order(activity_template_ptr.ParamArr1[0]);
				one_activity_msg->set_activity_start_sub_temp_id(ite->second.get_branch_template_id());
			}
		}
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_update_all_open_act);
	}

	void time_limit_activity_mgr::send_open_activity_branch_info_all()
	{
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			if (ite->second.is_open())
			{
				ite->second.send_info_to_self();
			}
		}
	}

	void time_limit_activity_mgr::add_new_act_with_new_temp()
	{
		if (nullptr == m_player_ptr || !m_player_ptr->is_self_server())
		{
			return;
		}
		 
		for (int32 i = e_time_limit_activity_type_total_charge; i < e_time_limit_activity_type_max; i++)
		{
			int32 act_guid = time_limit_activity_temp_mgr::get_instance().get_new_limit_act_with_act_type(i);
			if (act_guid <= 0)
			{
				continue;
			}
			auto ite = m_activity_obj.find(act_guid);
			if (ite == m_activity_obj.end())
			{
				//给老活动一个标记位置
				for (auto temp_ite = m_activity_obj.begin(); temp_ite != m_activity_obj.end();)
				{
					auto other_ite = temp_ite++;
					if (other_ite->second.get_act_type() == i)
					{
						other_ite->second.set_is_old_act(true);
					}
				}
				m_activity_obj[act_guid].clear_data();
				m_activity_obj[act_guid].set_player_ptr(m_player_ptr);
				m_activity_obj[act_guid].set_data_info(e_time_limit_activity_info_type, i);
				m_activity_obj[act_guid].set_data_info(e_time_limit_activity_info_guid, act_guid);
				m_activity_obj[act_guid].init_end();
				if (i == e_time_limit_activity_type_lucky_card)
				{
					m_activity_obj[act_guid].load_lucky_card_info_end(true);
				}
			}
			else
			{
				for (auto temp_ite = m_activity_obj.begin(); temp_ite != m_activity_obj.end();)
				{
					auto other_ite = temp_ite++;
					if (other_ite->second.get_act_type() == i && other_ite->first != act_guid)
					{
						other_ite->second.set_is_old_act(true);
					}
				}
			}
		}
	}

	void time_limit_activity_mgr::delete_old_act_with_new_temp()
	{
		if (nullptr == m_player_ptr || !m_player_ptr->is_self_server())
		{
			return;
		}
		std::vector<int32> delete_activity_id_arr;
		for (auto ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			const s_time_limit_activity_temp& act_temp = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_with_id(ite->first);
			if (!act_temp.is_valid())
			{
				delete_activity_id_arr.push_back(ite->first);
			}
		}
		for (int32 i = 0; i < delete_activity_id_arr.size(); i++)
		{
			m_activity_obj.erase(delete_activity_id_arr[i]);
		}
	}

	void time_limit_activity_mgr::init_template_id_all()
	{
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			ite->second.init_template_id_no_cross();
		}
	}
	void time_limit_activity_mgr::init_template_id_cross()
	{
		time_limit_act_map::iterator ite;
		for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			if (init_unit::is_cross_time_limit_cross_activity(ite->second.get_act_type()))
			{
				ite->second.init_template_id_cross();
			}
		}
	}
	void time_limit_activity_mgr::load_data_from_db(const s_time_limit_activity_info_db *activity_data, int32 data_num)
	{
		for (int32 i = 0; i < data_num; i++)
		{
			int32 act_guid = activity_data[i].data_info.data_ary[e_time_limit_activity_info_guid];
			if (act_guid <= 0)
			{
				continue;
			}
			m_activity_obj[act_guid].clear_data();
			m_activity_obj[act_guid].set_activity_info(activity_data[i].data_info);
			m_activity_obj[act_guid].set_player_ptr(m_player_ptr);
			if (e_time_limit_activity_type_lucky_card == activity_data[i].data_info.data_ary[e_time_limit_activity_info_type])
			{
				m_activity_obj[act_guid].load_lucky_card_info_end();
			}
		}
	}
	bool time_limit_activity_mgr::load_data_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_time_limit_activity_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_time_limit_activity_info_db));
		if (p_data == nullptr)
		{
			return false;
		}
		s_time_limit_activity_info_db *p_row = (s_time_limit_activity_info_db *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_time_limit_activity_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_info.data_ary[j] = db_row.data_ary(j);
			}
			for (int32 j = 0; j < db_row.activity_schedule_size(); j++)
			{
				p_row->data_info.activity_schedule[j] = db_row.activity_schedule(j);
			}
			p_row->data_info.set_schedule_str(db_row.schedule_str());
			p_row->data_info.reward_flag = db_row.reward_flag();
			p_row++;
		}
		load_data_from_db((const s_time_limit_activity_info_db *)p_data, msg.row_count());
		return true;

	}

	time_limit_activity_object& time_limit_activity_mgr::get_act_obj_by_act_guid(int32 act_guid)
	{
		if (act_guid <= 0)
		{
			return m_empty_act_obj;
		}
		auto ite = m_activity_obj.find(act_guid);
		if (ite == m_activity_obj.end())
		{
			return m_empty_act_obj;
		}
		else
		{
			return m_activity_obj[act_guid];
		}
	}

	time_limit_activity_object& time_limit_activity_mgr::get_new_act_obj_by_act_type(e_time_limit_activity_type activity_type)
	{
		std::vector<int32> act_id_arr = get_activity_obj(activity_type);
		if (act_id_arr.size() <= 0)
		{
			return m_empty_act_obj;
		}
		return get_act_obj_by_act_guid(act_id_arr[0]);
	}

	std::vector<int32>& time_limit_activity_mgr::get_activity_obj(e_time_limit_activity_type activity_type)
	{	
		m_act_obj_vec.clear();
		if (activity_type >= e_time_limit_activity_type_max)
		{
			return m_act_obj_vec;
		}
		for (auto ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
		{
			if (ite->second.get_act_type() == activity_type)
			{
				m_act_obj_vec.push_back(ite->first);
			}
		}
		const std::map<int32, s_time_limit_activity_temp>& time_limit_activity_temp_map = time_limit_activity_temp_mgr::get_instance().get_temp_map();
		std::sort(m_act_obj_vec.begin(), m_act_obj_vec.end(), [&](const int32& a, const int32& b)
		{
			if (time_limit_activity_temp_map.find(a) == time_limit_activity_temp_map.end() || time_limit_activity_temp_map.find(b) == time_limit_activity_temp_map.end())
			{
				return true;
			}
			return time_limit_activity_temp_map.at(a).CreateTime > time_limit_activity_temp_map.at(b).CreateTime;
		});

		return m_act_obj_vec;
	}

	void time_limit_activity_mgr::reset_ranking_value(e_time_limit_activity_type activity_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		if (activity_type == e_time_limit_activity_type_total_charge && is_activity_open(e_time_limit_activity_type_total_charge))
		{
			m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_time_limit_activity_total_recharge, get_activity_schedule(e_time_limit_activity_type_total_charge));
		}

		if (activity_type == e_time_limit_activity_type_cross_total_charge && is_activity_open(e_time_limit_activity_type_cross_total_charge))
		{
			m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_cross_time_limit_activity_total_recharge, get_activity_schedule(e_time_limit_activity_type_cross_total_charge));
		}

		if (activity_type == e_time_limit_activity_type_total_cost && is_activity_open(e_time_limit_activity_type_total_cost))
		{
			m_player_ptr->get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_time_limit_activity_total_cost, get_activity_schedule(e_time_limit_activity_type_total_cost));
		}

		if (activity_type >= e_time_limit_activity_type_gs_up && activity_type <= e_time_limit_activity_type_spirit_up)
		{
			set_up_rank_activity_schedule(activity_type);
		}
	}

	void time_limit_activity_mgr::apply_small_charge_award(int32 recharge_template_id)
	{
		std::vector<int32> act_id_arr = get_activity_obj(e_time_limit_activity_type_small_charge);
		for (int32 i = 0; i < act_id_arr.size(); i++)
		{
			if (!m_activity_obj[act_id_arr[i]].is_valid() || false == m_activity_obj[act_id_arr[i]].is_open())
			{
				continue;
			}
			int32 branch_template_id = m_activity_obj[act_id_arr[i]].get_branch_template_id();

			//最多循环50次 找到同一天内 充值id于当前充值id相同的
			for (int32 j = 0; j < 50; j++)
			{
				if (!m_activity_obj[act_id_arr[i]].is_same_cycle(branch_template_id + j))
				{
					break;
				}
				const s_time_limit_activity_branch_temp& activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id + j);
				if (activity_branch_template_ptr.Condition.size() > 0 && activity_branch_template_ptr.Condition[0] == recharge_template_id)
				{
					apply_award(branch_template_id + j, act_id_arr[i]);
					break;
				}
			}
		}
	}
	void time_limit_activity_mgr::apply_daily_gift_charge_award(int32 recharge_template_id)
	{
		std::vector<int32> act_id_arr = get_activity_obj(e_time_limit_activity_type_daily_gift_charge);
		for (int32 i = 0; i < act_id_arr.size(); i++)
		{
			if (!m_activity_obj[act_id_arr[i]].is_valid() || false == m_activity_obj[act_id_arr[i]].is_open())
			{
				continue;
			}
			int32 branch_template_id = m_activity_obj[act_id_arr[i]].get_branch_template_id();

			//最多循环50次 找到同一天内 充值id于当前充值id相同的
			for (int32 j = 0; j < 50; j++)
			{
				if (!m_activity_obj[act_id_arr[i]].is_same_cycle(branch_template_id + j))
				{
					break;
				}
				const s_time_limit_activity_branch_temp& activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id + j);
				if (activity_branch_template_ptr.Condition.size() > 0 && activity_branch_template_ptr.Condition[0] == recharge_template_id)
				{
					apply_award(branch_template_id + j, act_id_arr[i]);
					if (m_player_ptr != nullptr)
					{
						m_player_ptr->get_star_trip_mgr().target_mission(e_star_trip_target_type_buy_today_gift);
					}
					break;
				}
			}
		}
	}

	void time_limit_activity_mgr::check_send_new_server_mail()
	{
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(e_time_limit_activity_type_new_server_mail);
		if (!activity_obj_ptr.is_valid())
		{
			return;
		}
		if (activity_obj_ptr.is_open())
		{
			
			activity_obj_ptr.check_send_new_server_mail();
		}	
	}

	void time_limit_activity_mgr::buy_time_limit_item(int32 activity_type, int32 buy_num)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}

		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type((e_time_limit_activity_type)activity_type);
		if (!activity_obj_ptr.is_valid())
		{
			send_buy_time_limit_item_end(activity_type, (int32)e_buy_time_limit_item_end_type_template_error);
			return;
		}
		int32 money_type = -1;
		int32 money_num = -1;
		int32 item_id = -1;
		game_proto_limit_time_notice_info_list msg;
		int32 branch_template_id = activity_obj_ptr.get_branch_template_id();
		const s_time_limit_activity_branch_temp& sub_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id);
		if (false == sub_template_ptr.is_valid())
		{
			send_buy_time_limit_item_end(activity_type, (int32)e_buy_time_limit_item_end_type_template_error);
			return;
		}
		switch (activity_type)
		{
			
		case e_time_limit_activity_type_lucky_card:
		{
			if (sub_template_ptr.Condition.size() >= 4)
			{
				money_type = sub_template_ptr.Condition[2];
				money_num = sub_template_ptr.Condition[3];
				item_id = sub_template_ptr.Condition[0];
			}
		}
		break;
		case e_time_limit_activity_type_dragontrip:
		{
			if (sub_template_ptr.Condition.size() >= 3)
			{
				money_type = sub_template_ptr.Condition[1];
				money_num = sub_template_ptr.Condition[2];
				item_id = sub_template_ptr.Condition[0];
			}
		}
		break;
		default:
			break;
		}
		if (money_type == -1 || money_num == -1 || item_id == -1)
		{
			
			send_buy_time_limit_item_end(activity_type, (int32)e_buy_time_limit_item_end_type_template_error);
			return;
		}

		if (!m_player_ptr->can_cut_money((e_money_type)money_type, money_num * buy_num))
		{
			send_buy_time_limit_item_end(activity_type, (int32)e_buy_time_limit_item_end_type_money_error);
			return;
		}
		std::vector<s_item_template_info> item_list;
		item_list.push_back({ item_id, buy_num, 1 });
		m_player_ptr->get_item_set().put_in_bag(e_server_log_add_item_time_limit_buy, activity_type, item_list);
		m_player_ptr->cut_money((e_money_type)money_type, money_num * buy_num, e_server_log_cut_money_time_limit_buy_item);
		send_buy_time_limit_item_end(activity_type, (int32)e_buy_time_limit_item_end_type_buy_succeed);
	}

	void time_limit_activity_mgr::send_buy_time_limit_item_end(int32 activity_type, int32 result_type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		time_limit_activity_proto_buy_time_limit_item_end msg;
		msg.set_activity_type(activity_type);
		msg.set_result(result_type);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_buy_time_limit_item_end);
	}

	void time_limit_activity_mgr::send_limit_time_notice_info()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(e_time_limit_activity_type_limit_time_notice);
		if (!activity_obj_ptr.is_valid())
		{
			return;
		}
		game_proto_limit_time_notice_info_list msg;
		int32 branch_template_id = activity_obj_ptr.get_branch_template_id();
		const s_time_limit_activity_branch_temp& sub_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id);
		if (false == sub_template_ptr.is_valid())
		{
			return;
		}
		if (sub_template_ptr.ParamArr1.size() > 0)
		{
			for (int32 i = 0; i < sub_template_ptr.ParamArr1.size(); i++)
			{
				msg.add_black_show_notice(sub_template_ptr.ParamArr1[i]);
			}
		}
		
		if (sub_template_ptr.Reward.size() / 2 > 0 && sub_template_ptr.Reward.size() % 2 == 0)
		{
			for (int32 i = 0; i < sub_template_ptr.Reward.size() / 2; i++)
			{
				game_proto_limit_time_notice_info * tem_info = msg.add_info_list();
				if (nullptr == tem_info)
				{
					break;
				}
				tem_info->set_template_id(sub_template_ptr.Reward[i * 2]);
				tem_info->set_weight_num(sub_template_ptr.Reward[i * 2 + 1]);
			}
		}
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_limit_time_notice_info_list);
	}

	void time_limit_activity_mgr::set_up_rank_activity_schedule(e_time_limit_activity_type activity_type)
	{
		if (activity_type < faith::e_time_limit_activity_type_gs_up || activity_type > e_time_limit_activity_type_cross_spirit_up)
		{
			return;
		}
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
		if (!activity_obj_ptr.is_valid())
		{
			return;
		}
		if (activity_obj_ptr.is_open())
		{
			activity_obj_ptr.set_need_sync_to_ranking();
		}
	}

	int32 time_limit_activity_mgr::get_treasure_lua_award_index(e_time_limit_activity_type activity_type)
	{
		if (activity_type != e_time_limit_activity_type_treasure
			&& activity_type != e_time_limit_activity_type_treasure_royal1
			&& activity_type != e_time_limit_activity_type_treasure_royal2
			&& activity_type != e_time_limit_activity_type_treasure_royal3
			&& activity_type != e_time_limit_activity_type_treasure_sprite
			&& activity_type != e_time_limit_activity_type_treasure_element
			&& activity_type != e_time_limit_activity_type_new_server_treasure
			&& activity_type != e_time_limit_activity_type_limit_time_treasure2)
		{
			return 0;
		}
		int32 index = 0;
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
		if (!activity_obj_ptr.is_valid() || !activity_obj_ptr.is_open())
		{
			return 0;
		}
		const s_time_limit_activity_branch_temp& branch_template_ptr = activity_obj_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid <= 0 || branch_template_ptr.ParamArr1.size() < 1)
		{
			return 0;
		}
		index = branch_template_ptr.ParamArr1[0];

		
		return index;
	}

	int32 time_limit_activity_mgr::get_activity_circle_num(e_time_limit_activity_type activity_type)
	{
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
		if (!activity_obj_ptr.is_valid())
		{
			return 0;
		}
		return activity_obj_ptr.get_cycle_num();
	}

	int32 time_limit_activity_mgr::get_activity_circle_index(e_time_limit_activity_type activity_type)
	{
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
		if (!activity_obj_ptr.is_valid())
		{
			return 0;
		}
		return activity_obj_ptr.get_cycle_index();
	}

	int32 time_limit_activity_mgr::get_activity_branch_template_id(e_time_limit_activity_type activity_type)
	{
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
		if (!activity_obj_ptr.is_valid())
		{
			return 0;
		}
		return activity_obj_ptr.get_branch_template_id();
	}

	int32 time_limit_activity_mgr::get_activity_template_id(e_time_limit_activity_type activity_type)
	{
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
		if (!activity_obj_ptr.is_valid())
		{
			return 0;
		}
		return activity_obj_ptr.get_act_guid();
	}

	void time_limit_activity_mgr::save_data_to_db(const int32& unit_array_index, int32 save_type_ex)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_time_limit_activity_info);
		if (is_use == false)
		{
			int32 data_num = 0;
			cs2dp_save_time_limit_activity_info req;
			time_limit_act_map::iterator ite;
			for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
			{
				req.activity_data_array[data_num].role_guid = m_player_ptr->get_unit_guid();
				req.activity_data_array[data_num].data_info = ite->second.get_activity_info();
				data_num++;
			}
			req.data_num = data_num;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = unit_array_index;
			req.save_type_ex = save_type_ex;
			req.is_self_server = m_player_ptr->is_self_server();
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_time_limit_activity msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(unit_array_index);
			msg.set_save_type_ex(save_type_ex);
			msg.set_is_self_server(m_player_ptr->is_self_server());

			faith::cs2dp_proto::role_time_limit_activity_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			int32 row_count = 0;
			time_limit_act_map::iterator ite;
			for (ite = m_activity_obj.begin(); ite != m_activity_obj.end(); ite++)
			{
				faith::cs2dp_proto::role_time_limit_activity_row *db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				s_time_limit_activity_info& tmp = ite->second.get_activity_info();
				for (int32 i = 0; i < e_time_limit_activity_info_max; i++)
				{
					db_row->add_data_ary(tmp.data_ary[i]);
				}
				for (int32 i = 0; i < time_limit_activity_schedule_num; i++)
				{
					db_row->add_activity_schedule(tmp.activity_schedule[i]);
				}
				db_row->set_schedule_str(tmp.schedule_str);
				db_row->set_reward_flag(tmp.reward_flag);
				row_count++;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_time_limit_activity_info);

		}
	}

	bool time_limit_activity_mgr::is_activity_open(e_time_limit_activity_type activity_type)
	{
		bool is_have_one_open = false;
		std::vector<int32> act_id_arr = get_activity_obj(activity_type);
		for (int32 i = 0; i < act_id_arr.size(); i++)
		{
			if (m_activity_obj[act_id_arr[i]].is_valid() && m_activity_obj[act_id_arr[i]].is_open())
			{
				is_have_one_open = true;
				break;
			}
		}
		return is_have_one_open;
	}

	void time_limit_activity_mgr::apply_award(int32 reward_template_id, int32 act_guid, int32 param_int_1)
	{
		const s_time_limit_activity_branch_temp& reward_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(reward_template_id);
		if (reward_template_ptr.ActGuid <= 0 || act_guid <= 0)
		{
			return;
		}
		auto ite = m_activity_obj.find(act_guid);
		if (ite == m_activity_obj.end())
		{
			return;
		}
		time_limit_activity_object& activity_obj_ptr = m_activity_obj[act_guid];
		if (false == activity_obj_ptr.is_valid())
		{
			return;
		}
		if (e_time_limit_activity_type_shop == activity_obj_ptr.get_act_type() 
			|| e_time_limit_activity_type_discount_shop == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_discount_shop1 == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_discount_shop2 == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_discount_shop3 == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_item_shop == activity_obj_ptr.get_act_type() 
			|| e_time_limit_activity_type_item_shop1 == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_item_shop2 == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_item_shop3 == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_weekend_top_up == activity_obj_ptr.get_act_type()
			|| e_time_limit_activity_type_support_for_promotion == activity_obj_ptr.get_act_type())
		{
			activity_obj_ptr.buy_goods(reward_template_id, param_int_1);
		}
		else
		{
			activity_obj_ptr.get_reward(reward_template_id);
		}
	}

	void time_limit_activity_mgr::activity_behavior_done(e_time_limit_behavior_type behavior_type, int32 add_value)
	{
		if (behavior_type <= e_time_limit_behavior_type_non || behavior_type >= e_time_limit_behavior_type_max || nullptr == m_player_ptr)
		{
			return;
		}
		for (int32 activity_type = 0; activity_type < e_time_limit_activity_type_max; activity_type++)
		{
			if (!init_unit::is_time_limit_behavior_welfare(activity_type))
			{
				continue;
			}
			e_time_limit_activity_type reward_type = (e_time_limit_activity_type)init_unit::get_behavior_welfare_reward_type(activity_type);
			if (reward_type < 0)
			{
				continue;
			}
			std::vector<int32> act_id_arr = get_activity_obj((e_time_limit_activity_type)activity_type);
			std::vector<int32> act_reward_id_arr = get_activity_obj(reward_type);
			if (act_id_arr.size() != act_reward_id_arr.size())
			{
				continue;;
			}
			for (int32 i = 0; i < act_id_arr.size(); i++)
			{
				time_limit_activity_object& activity_obj_ptr = m_activity_obj[act_id_arr[i]];
				if (!activity_obj_ptr.is_valid() || !activity_obj_ptr.is_open())
				{
					continue;
				}

				const s_time_limit_activity_branch_temp& branch_template_ptr = activity_obj_ptr.get_time_limit_activity_branch_temp();
				if (branch_template_ptr.ActGuid <= 0 || branch_template_ptr.Condition.size() <= 0 || branch_template_ptr.Condition.size() % 2 != 0)
				{
					continue;
				}

				int32 done_index = -1;
				for (int32 i = 0; i * 2 < branch_template_ptr.Condition.size(); i++)
				{
					if (branch_template_ptr.Condition[i * 2] == behavior_type)
					{
						done_index = i;
						break;
					}
				}

				if (done_index < 0 || activity_obj_ptr.is_reward_geted(done_index))
				{
					continue;
				}

				activity_obj_ptr.add_activity_schedule(add_value, done_index);
				server_log::time_limit_activity_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), activity_obj_ptr.get_branch_template_id(), (e_time_limit_activity_type)activity_type
					, activity_obj_ptr.get_cur_value(done_index), activity_obj_ptr.get_target_value(activity_obj_ptr.get_branch_template_id(), done_index), 0, 0, m_player_ptr->get_login_type(),0,0, branch_template_ptr.AttributeId);

				time_limit_activity_object& behavior_welfare_reward_obj_ptr = m_activity_obj[act_reward_id_arr[i]];
				if (activity_obj_ptr.is_can_get_reward(activity_obj_ptr.get_branch_template_id(), done_index) && behavior_welfare_reward_obj_ptr.is_valid()
					&& branch_template_ptr.Reward.size() > done_index)
				{
					behavior_welfare_reward_obj_ptr.add_activity_schedule(branch_template_ptr.Reward[done_index]);
					behavior_welfare_reward_obj_ptr.send_info_to_self();
					activity_obj_ptr.set_reward_get_flag(done_index);
					server_log::time_limit_activity_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), behavior_welfare_reward_obj_ptr.get_branch_template_id(), reward_type
						, behavior_welfare_reward_obj_ptr.get_cur_value(), behavior_welfare_reward_obj_ptr.get_target_value(behavior_welfare_reward_obj_ptr.get_branch_template_id()), 0, 0, m_player_ptr->get_login_type(),0,0, branch_template_ptr.AttributeId);
				}

				time_limit_activity_proto_one_behavior_type_activity_message msg;
				msg.set_act_guid(activity_obj_ptr.get_act_guid());
				msg.set_activity_type(activity_obj_ptr.get_act_guid());
				msg.set_activity_sub_id(activity_obj_ptr.get_branch_template_id());
				msg.set_behavior_idex(done_index);
				msg.set_behavior_value(activity_obj_ptr.get_activity_schedule(done_index));
				m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_time_limit_update_one_behavior);
			}
		}
	}

	int32 time_limit_activity_mgr::get_activity_schedule(e_time_limit_activity_type activity_type, int32 schedule_index /*= 0*/, int32 act_guid)
	{
		if (init_unit::is_no_superposition_time_limit_activity(activity_type) || act_guid <= 0)
		{
			time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
			if (!activity_obj_ptr.is_valid())
			{
				return 0;
			}
			return activity_obj_ptr.get_activity_schedule(schedule_index);
		}
		else
		{
			auto ite = m_activity_obj.find(act_guid);
			if (ite == m_activity_obj.end())
			{
				return 0;
			}
			time_limit_activity_object& activity_obj_ptr = m_activity_obj[act_guid];
			if (!activity_obj_ptr.is_valid())
			{
				return 0;
			}
			return activity_obj_ptr.get_activity_schedule(schedule_index);
		}
		return 0;
	}

	void time_limit_activity_mgr::set_activity_schedule(e_time_limit_activity_type activity_type, int32 schedule_value, int32 schedule_index /*= 0*/, int32 act_guid)
	{
		if (act_guid <= 0)
		{
			time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
			if (!activity_obj_ptr.is_valid())
			{
				return;
			}
			activity_obj_ptr.set_activity_schedule(schedule_value, schedule_index);
			activity_obj_ptr.send_info_to_self();
		}
		else
		{
			auto ite = m_activity_obj.find(act_guid);
			if (ite == m_activity_obj.end())
			{
				return;
			}
			time_limit_activity_object& activity_obj_ptr = m_activity_obj[act_guid];
			if (!activity_obj_ptr.is_valid())
			{
				return;
			}
			activity_obj_ptr.set_activity_schedule(schedule_value, schedule_index);
			activity_obj_ptr.send_info_to_self();
		}
	}

	void time_limit_activity_mgr::add_activity_schedule(e_time_limit_activity_type activity_type, int32 add_value, int32 schedule_index /*= 0*/, int32 act_guid)
	{
		std::vector<int32> act_id_arr = get_activity_obj(activity_type);
		if (act_id_arr.size() <= 0)
		{
			return;
		}
		for (int32 i = 0; i < act_id_arr.size(); i++)
		{
			time_limit_activity_object& activity_obj_ptr = m_activity_obj[act_id_arr[i]];
			if (false == activity_obj_ptr.is_valid())
			{
				continue;
			}
			activity_obj_ptr.add_activity_schedule(add_value, schedule_index);
			activity_obj_ptr.send_info_to_self();
		}
		
		sync_common_data_after_add_schedule(activity_type, add_value);
	}
	void time_limit_activity_mgr::sync_common_data_after_add_schedule(e_time_limit_activity_type activity_type, int32 add_value)
	{
		switch (activity_type)
		{
			case faith::e_time_limit_activity_type_national_treasure_4:
			case faith::e_time_limit_activity_type_national_treasure_2:
			{
				std::vector<int32> condition_list;
				//向ws同步最新充值数据

				time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type(activity_type);
				if (!activity_obj_ptr.is_valid() || !activity_obj_ptr.is_open())
				{
					break;
				}
				int32 branch_template_id = activity_obj_ptr.get_branch_template_id();
				if (branch_template_id == 0)
				{
					break;
				}
				for (int32 i = 0; i < time_limit_activity_schedule_num; i++)
				{
					if (!activity_obj_ptr.is_same_cycle(branch_template_id + i))
					{
						break;
					}
					const s_time_limit_activity_branch_temp& activity_branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id + i);
					if (activity_branch_template_ptr.Condition.size() > 1)
					{
						condition_list.push_back(activity_branch_template_ptr.Condition[1]);
					}
				}
				sync_time_limit_activity_data_to_ws(activity_type, add_value, condition_list);
			}
			break;
		default:
			break;
		}
	}

	void time_limit_activity_mgr::sync_time_limit_activity_data_to_ws(int32 activity_type, int32 data_num, std::vector<int32> condition_list)
	{
		cs2ws_time_limit_activity_common_data_sync msg;
		msg.data_num = data_num;
		msg.activity_type = activity_type;
		vector<int32>::iterator it;
		int32 condition_index = 0;
		for (it = condition_list.begin();it!=condition_list.end();it++)
		{
			if (condition_index >= time_limit_activity_schedule_num)
			{
				break;
			}
			msg.condition_list[condition_index] = *it;
			condition_index++;
		}
		msg.condition_num = condition_index;
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_time_limit_activity_common_data_sync));
	}

	void time_limit_activity_mgr::check_time_limit_activity_common_data_to_ws(int32 activity_id, int32 activity_type, int32 check_num)
	{
		cs2ws_check_time_limit_activity_common_data msg;
		msg.user_guid = m_player_ptr->get_unit_guid();
		msg.activity_type = activity_type;
		msg.check_data = check_num;
		msg.activity_id = activity_id;
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_check_time_limit_activity_common_data));
	}

	void time_limit_activity_mgr::check_time_limit_activity_common_data_to_ws_resp(int32 activity_id,bool is_achieve)
	{
		const s_time_limit_activity_branch_temp& reward_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(activity_id);
		if (reward_template_ptr.ActGuid <= 0)
		{
			return;
		}
		time_limit_activity_object& activity_obj_ptr = get_new_act_obj_by_act_type((e_time_limit_activity_type)reward_template_ptr.ActivityType);
		if (false == activity_obj_ptr.is_valid())
		{
			return;
		}
		activity_obj_ptr.get_reward_by_check_common_data_end(activity_id, is_achieve);
	}

	void time_limit_activity_mgr::check_refresh_charge_value()
	{
		if (nullptr == m_player_ptr)
			return;
		//使用每日充值同步显示活动充值相关 这里依赖的是 正常活动开启时充值数值一定是大于等于每日充值数值的
		int32 cur_daily_recharge_num = m_player_ptr->get_unit_info(e_role_info_daily_recharge_num);
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_total_charge))
		{
			set_activity_schedule(e_time_limit_activity_type_total_charge, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_cross_total_charge))
		{	
			set_activity_schedule(e_time_limit_activity_type_cross_total_charge, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_total_charge2))
		{
			set_activity_schedule(e_time_limit_activity_type_total_charge2, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_new_total_charge_1))
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_charge_1, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_new_total_charge_2))
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_charge_2, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_new_total_charge_3))
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_charge_3, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_daily_charge))
		{
			set_activity_schedule(e_time_limit_activity_type_daily_charge, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_daily_charge2))
		{
			set_activity_schedule(e_time_limit_activity_type_daily_charge2, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_daily_charge3))
		{
			set_activity_schedule(e_time_limit_activity_type_daily_charge3, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_daily_charge4))
		{
			set_activity_schedule(e_time_limit_activity_type_daily_charge4, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_daily_charge5))
		{
			set_activity_schedule(e_time_limit_activity_type_daily_charge5, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_daily_charge6))
		{
			set_activity_schedule(e_time_limit_activity_type_daily_charge6, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_national_treasure_1))
		{
			set_activity_schedule(e_time_limit_activity_type_national_treasure_1, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_national_treasure_2))
		{
			set_activity_schedule(e_time_limit_activity_type_national_treasure_2, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_national_treasure_3))
		{
			set_activity_schedule(e_time_limit_activity_type_national_treasure_3, cur_daily_recharge_num);
		}
		if (cur_daily_recharge_num > get_activity_schedule(e_time_limit_activity_type_national_treasure_4))
		{
			set_activity_schedule(e_time_limit_activity_type_national_treasure_4, cur_daily_recharge_num);
		}
		int32 cur_daily_cost_num = m_player_ptr->get_unit_info(e_role_info_daily_cost);
		if (cur_daily_cost_num > get_activity_schedule(e_time_limit_activity_type_total_cost))
		{
			set_activity_schedule(e_time_limit_activity_type_total_cost, cur_daily_cost_num);
		}
		if (cur_daily_cost_num > get_activity_schedule(e_time_limit_activity_type_total_cost_jewel))
		{
			set_activity_schedule(e_time_limit_activity_type_total_cost_jewel, cur_daily_cost_num);
		}
		if (cur_daily_cost_num > get_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_1))
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_1, cur_daily_cost_num);
		}
		if (cur_daily_cost_num > get_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_2))
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_2, cur_daily_cost_num);
		}
		if (cur_daily_cost_num > get_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_3))
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_3, cur_daily_cost_num);
		}
		if (get_activity_schedule(e_time_limit_activity_type_total_landing) <= 0)
		{
			set_activity_schedule(e_time_limit_activity_type_total_landing, 1);
		}
		if (get_activity_schedule(e_time_limit_activity_type_new_total_landing) <= 0)
		{
			set_activity_schedule(e_time_limit_activity_type_new_total_landing, 1);
		}
	}
}
