/********************************************************************
created: 2016年10月8日
file base: welfare_mgr
file ext: cpp
author: luoxingyu

purpose: about role's welfare_mgr
*********************************************************************/

#include "welfare_mgr.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "template/StringConst_S.h"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "server_log.hpp"
#include "Logic/time_def.hpp"
#include "lua/script_mgr.h"
#include "internal/welfare_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/welfare.pb.h"
#include "internet/net.pb.h"
#include "utility/parse_msg.h"

namespace hld
{
	cwelfare_mgr::cwelfare_mgr()
	{
		m_unit_array_index = 0;
		clear_data();
	}
	cwelfare_mgr::~cwelfare_mgr(void)
	{

	}
	void cwelfare_mgr::clear_data()
	{
		m_online_time = 0;
		m_old_tick_time = 0;
		m_min_tick = 0;
		m_welfare_info.clear_data();
		m_active_degree_info.clear_data();
		m_random_get_info.clear_data();
	}

	void cwelfare_mgr::heart_tick(const int64& new_time)
	{
		if (m_online_time <= 0)
		{
			m_online_time = new_time;
		}
		else
		{
			int64 pass_mill_second = new_time - m_online_time;
			int64 pass_second = pass_mill_second / hld::second_tick_time;
			if (pass_second >= 1)
			{
				int32 old_time = get_active_degree_info(e_daily_active_degree_type_online_30_minutes);
				set_active_degree_info(e_daily_active_degree_type_online_30_minutes, old_time + pass_second);
				m_online_time = new_time - (pass_mill_second - pass_second * hld::second_tick_time);
			}
		}

		if (new_time > m_old_tick_time)
		{
			m_old_tick_time = new_time + welfare_send_time;
			//每10分钟主动同步一次
			send_active_degree();
		}
		//统一在player::refresh_daily_info里刷新
		/*if (new_time - m_min_tick >= minute_tick_time)
		{
			m_min_tick = new_time;
			player& player_ref = unit_man::get_player(m_unit_array_index);
			if (player_ref.is_valid() == false )
			{
				return;
			}
			bool is_refresh = false;
			if (is_need_refresh_welfare())
			{
				refresh_daily_info();
				is_refresh = true;
			}

			if (is_need_refresh_active_degree())
			{
				refresh_at_specify_hour();
				is_refresh = true;
			}
			if (is_refresh)
			{
				send_random_get_all();
				send_welfare_all();
				send_active_degree();
			}
		}*/

	}

	void cwelfare_mgr::load_welfare_by_db(const s_welfare_info* welfare_data)
	{
		if (welfare_data == nullptr)
		{
			return;
		}
		for (int32 i = 0; i < e_welfare_type_max; ++i)
		{
			m_welfare_info.data_ary[i] = welfare_data->data_ary[i];
		}
		
		send_welfare_all();
	}
	bool cwelfare_mgr::load_welfare_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_welfare_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}
		s_welfare_info db_data;
		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}
		load_welfare_by_db(&db_data);

		return true;

	}
	void cwelfare_mgr::save_welfare_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_welfare);
		if (is_use == false)
		{

			cs2dp_save_char_welfare req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = player_ref.get_array_index();
			req.welfare_data = m_welfare_info;

			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_char_welfare));
		}
		else
		{
			hld::cs2dp_proto::save_role_welfare msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(player_ref.get_array_index());
			msg.set_save_type_ex(save_type);

			hld::cs2dp_proto::role_welfare_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}

			for (int32 i = 0; i < e_welfare_type_max; i++)
			{
				db_data->add_data_ary(m_welfare_info.data_ary[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_welfare);
		}
	}

	void cwelfare_mgr::send_welfare_one(e_welfare_type info_index)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		welfare_proto_welfare_info_one msg;
		msg.set_info_type(info_index);
		msg.set_info_value(m_welfare_info.data_ary[info_index]);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_welfare_info_one);
	}
	void cwelfare_mgr::send_welfare_all()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		welfare_proto_welfare_info_all msg;
		for (int32 i = 0; i < e_welfare_type_max; ++i)
		{
			msg.add_info_value(m_welfare_info.data_ary[i]);
		}
		time_info cur_time_info = time_helper::get_cur_time_new();
		msg.set_days_num(time_helper::get_days_by_month(cur_time_info.year, cur_time_info.month_in_year));
		msg.set_today_data(cur_time_info.day_in_month + 1);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_welfare_info_all);
	}



	void cwelfare_mgr::set_welfare_info(e_welfare_type info_type, int32 info_value)
	{
		if (info_type >= e_welfare_type_max)
		{
			return;
		}
		m_welfare_info.data_ary[info_type] = info_value;
		send_welfare_one(info_type);
	}

	int32 cwelfare_mgr::get_welfare_info(e_welfare_type info_type)
	{
		if (info_type >= e_welfare_type_max)
		{
			return -1;
		}
		return m_welfare_info.data_ary[info_type];
	}

	void cwelfare_mgr::refresh_daily_info()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		if (player_ref.get_write_log())
		{
			set_log_var(log_head)
			player_ref.get_log_common_head_info(log_head);
			log_head.logTime -= second_tick_time * 100;  // 纪录上一天的在线
			int32 onlineTime = get_active_degree_info(e_daily_active_degree_type_online_30_minutes);
			server_log::serverEarlyWarning(log_head, onlineTime);

		}
	    //player_ref.set_time_data(e_time_type_welfare_last_refresh_time, time_helper::get_cur_time_new().second);
		set_welfare_info(e_welfare_type_online_time, 0);
		set_welfare_info(e_welfare_type_daily_recharge_reward, 0);
		set_welfare_info(e_welfare_type_is_today_check_in, 0);
		set_welfare_info(e_welfare_type_continue_login, 0);
		set_welfare_info(e_welfare_type_time_limit_reward, 0);//限时福利标记位
		set_active_degree_info(e_daily_active_degree_type_online_30_minutes, 0);
		//每日刷新，自动算登录次数

		//m_online_time = m_active_degree_info.data_ary[e_daily_active_degree_type_online_30_minutes];

		for (int32 i = e_welfare_random_item_continue_login_0; i < e_welfare_random_item_max; ++i)
		{
			m_random_get_info.data_ary[i] = 0;
		}

		for (int32 i = e_welfare_random_item_daily_online_0; i <= e_welfare_random_item_daily_online_7; ++i)
		{
			m_random_get_info.data_ary[i] = 0;
		}

	}

	void cwelfare_mgr::refresh_at_specify_hour()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		////存下一次刷新点的时间
		//int64 next_refresh = time_helper::get_next_refresh_time_stamp();
		//player_ref.set_time_data(e_time_type_active_degree_last_refresh_time, next_refresh);

		set_welfare_info(e_welfare_type_daily_activity_reward, 0);
		m_active_degree_info.clear_data();
		set_active_degree_info(e_daily_active_degree_type_login, 1);
	}

	int32 cwelfare_mgr::get_welfare_has_num(int32 welfare_typ)
	{
		if (welfare_typ < 0 || welfare_typ >= e_welfare_type_max)
		{
			return -1;
		}
		int32 has_num = 0;
		for (int32 i = 0; i < 30; ++i)
		{
			int32 temp_flag = 1 << i;
			if ((m_welfare_info.data_ary[welfare_typ] & temp_flag) > 0)
			{
				has_num++;
			}
		}
		return has_num;
	}

	void cwelfare_mgr::load_active_degree_by_db(const s_active_degree_info* active_degree_data)
	{
		if (active_degree_data == nullptr)
		{
			return;
		}

		for (int32 i = 0; i < e_daily_active_degree_type_max; ++i)
		{
			m_active_degree_info.data_ary[i] = active_degree_data->data_ary[i];
		}
		//m_online_time = m_active_degree_info.data_ary[e_daily_active_degree_type_online_30_minutes];
		send_active_degree();
	}
	bool cwelfare_mgr::load_active_degree_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_active_degree_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_active_degree_info db_data;

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}

		load_active_degree_by_db(&db_data);

		return true;

	}

	void cwelfare_mgr::save_active_degree_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_active_degree);
		if (is_use == false)
		{

			cs2dp_save_char_active_degree req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = player_ref.get_array_index();
			req.active_degree_data = m_active_degree_info;

			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_char_active_degree));
		}
		else
		{
			hld::cs2dp_proto::save_role_active_degree msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(player_ref.get_array_index());
			msg.set_save_type_ex(save_type);

			hld::cs2dp_proto::role_active_degree_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i <e_daily_active_degree_type_max; i++)
			{
				db_data->add_data_ary(m_active_degree_info.data_ary[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_active_degree);

		}

	}

	void cwelfare_mgr::send_active_degree()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		welfare_proto_active_degree_info msg;
		for (int32 i = 0; i < e_daily_active_degree_type_max; ++i)
		{
			msg.add_info_value(m_active_degree_info.data_ary[i]);
		}
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_active_degree_info);
	}

	void cwelfare_mgr::save_random_get_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);

		cs2dp_save_char_welfare_random_get req;
		req.save_type_ex = save_type;
		req.role_guid = player_ref.get_unit_guid();
		req.unit_array_index = m_unit_array_index;
		req.random_get_data = m_random_get_info;

		player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_char_welfare_random_get));
	}
	void cwelfare_mgr::load_random_get_by_db(const s_welfare_random_get_info* active_degree_data)
	{
		m_random_get_info = *active_degree_data;
		send_random_get_all();
	}
	void cwelfare_mgr::send_random_get_all()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		welfare_proto_random_get_info msg;
		for (int32 i = 0; i < e_welfare_random_item_max; ++i)
		{
			msg.add_info_value(m_random_get_info.data_ary[i]);
		}
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_random_get_info);
	}
	void cwelfare_mgr::send_random_get_one(e_welfare_random_item info_index)
	{
		if (info_index >= e_welfare_random_item_max)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		welfare_proto_random_get_info msg;
		msg.add_info_value(m_random_get_info.data_ary[info_index]);
		msg.add_data_index(info_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_random_get_info);
	}
	void cwelfare_mgr::set_random_get_info(e_welfare_random_item info_index, int32 info_value)
	{
		if (info_index >= e_welfare_random_item_max)
		{
			return;
		}
		m_random_get_info.data_ary[info_index] = info_value;
	}
	i32 cwelfare_mgr::get_random_get_info(e_welfare_random_item info_index)
	{
		if (info_index >= e_welfare_random_item_max)
		{
			return 0;
		}
		return m_random_get_info.data_ary[info_index];
	}

	void cwelfare_mgr::set_active_degree_info(e_daily_active_degree_type info_type, int32 info_value)
	{
		if (info_type >= e_daily_active_degree_type_max)
		{
			return;
		}
		int32 old_value = m_active_degree_info.data_ary[info_type];
		m_active_degree_info.data_ary[info_type] = info_value;
		if (info_type != e_daily_active_degree_type_online_30_minutes)
		{
			//避免每秒同步一次
			send_active_degree();
		}

		ActiveDegreeTemplate* template_ptr = template_manager::get_instance().get_template_by_active_degree_type(info_type);
		if (template_ptr == nullptr)
		{
			return;
		}
		int32 TempTargetValue = template_ptr->TargetValue;
		if (info_type == e_daily_active_degree_type_online_30_minutes)
		{
			//在线时间，填的目标值单位是分钟
			TempTargetValue *= 60;
		}
		if ((old_value < TempTargetValue) && (m_active_degree_info.data_ary[info_type] >= TempTargetValue))
		{
			player& player_ref = unit_man::get_player(m_unit_array_index);
			int32 cur_dative_degree = player_ref.get_unit_info(e_role_info_daily_activity_degree);
			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(player_ref.get_vip_level());
			if (vip_template_ptr == nullptr)
			{
				return;
			}
			int32 vip_active_degree_add = vip_template_ptr->LivenessBonuse;
			int32 base_active_degree_add = template_ptr->AddValue;
			/*player_ref.set_unit_info(e_role_info_daily_activity_degree, cur_dative_degree + vip_active_degree_add + base_active_degree_add);
			player_ref.send_info_one(e_role_info_daily_activity_degree);

			int32 activity_template_id = template_manager::get_instance().get_template_id_by_active_degree_type(info_type);
			if (activity_template_id > 0)
			{
				welfare_proto_activity_degree_finish finish_msg;
				finish_msg.set_activity_template_id(activity_template_id);
				player_ref.send_message_to_self(&finish_msg, e_msgindex_s2c_active_degree_finish);
			}*/
		}
	}


	void cwelfare_mgr::get_welfare_by_id(int32 welfare_template_id)
	{
		script_mgr::get_instance().call_func("welfare","operate_welfare", 0, false, "%d%d", m_unit_array_index, welfare_template_id);
	}

	void cwelfare_mgr::reset_first_recharge_welfare()
	{
		
		player& player_ref = unit_man::get_player(m_unit_array_index);
		//如果是老玩家，已充值，已领取奖励，则设置首充时间继续领取奖励
		if (is_welfare_geted(e_welfare_type_first_recharge_reward, 0) == 1)
		{
			if (player_ref.get_time_data(e_time_type_frist_recharge_time) == 0)
			{
				if (player_ref.get_unit_info(e_role_info_real_recharge_jewel_num) != 0)
				{
					int32 Zero_time = time_helper::get_today_zero_time_info().second;
					player_ref.set_time_data(e_time_type_frist_recharge_time, Zero_time);
				}
			}
		}
		//如果是老玩家，已充值，没有领取奖励，则设置首充时间继续领取奖励
		if (is_welfare_geted(e_welfare_type_first_recharge_reward, 0) == 0)
		{
			if (player_ref.get_time_data(e_time_type_frist_recharge_time) == 0)
			{
				if (player_ref.get_unit_info(e_role_info_real_recharge_jewel_num) != 0)
				{
					int32 Zero_time = time_helper::get_today_zero_time_info().second;
					player_ref.set_time_data(e_time_type_frist_recharge_time, Zero_time);
				}
			}
		}
	}

	void cwelfare_mgr::get_first_recharge_welfare_by_id()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		tm* now_time = time_helper::get_localtime();
		i64 cur_time = mktime(now_time);
		
		welfare_proto_get_first_recharge_time_end msg;

		WelfareTemplate* FirstTemplatePtr = GET_TEMPLATE(WelfareTemplate, 107000001);
		if (FirstTemplatePtr == nullptr)
		{
			msg.set_is_first_recharge_time(0);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_first_recharge_time_end);
			return;
		}
		WelfareTemplate* SecondTemplatePtr = GET_TEMPLATE(WelfareTemplate, 107000002);
		if (SecondTemplatePtr == nullptr)
		{
			msg.set_is_second_recharge_time(0);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_first_recharge_time_end);
			return;
		}
		WelfareTemplate* ThirdTemplatePtr = GET_TEMPLATE(WelfareTemplate, 107000003);
		if (ThirdTemplatePtr == nullptr)
		{
			msg.set_is_third_recharge_time(0);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_first_recharge_time_end);
			return;
		}
		int32 first_welfare_index = FirstTemplatePtr->Index;
		int32 second_welfare_index = SecondTemplatePtr->Index;
		int32 third_welfare_index = ThirdTemplatePtr->Index;

		i64 first_recharge_time = player_ref.get_time_data(e_time_type_frist_recharge_time);
		
		if (first_recharge_time == 0)
		{
			msg.set_is_first_recharge_time(1);
			msg.set_is_second_recharge_time(1);
			msg.set_is_third_recharge_time(1);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_first_recharge_time_end);
			return;
		}
		
		
		i64 first_target_recharge_time = first_recharge_time + 86400 * first_welfare_index;

		if (first_target_recharge_time - cur_time > 0)
		{
			msg.set_is_first_recharge_time(first_target_recharge_time - cur_time);
		}
		else
		{
			msg.set_is_first_recharge_time(0);
		}

		i64 Second_target_recharge_time = first_recharge_time + 86400 * second_welfare_index;

		if (Second_target_recharge_time - cur_time > 0)
		{
			msg.set_is_second_recharge_time(Second_target_recharge_time - cur_time);
		}
		else
		{
			msg.set_is_second_recharge_time(0);
		}
		i64 third_target_recharge_time = first_recharge_time + 86400 * third_welfare_index;

		if (third_target_recharge_time - cur_time > 0)
		{
			msg.set_is_third_recharge_time(third_target_recharge_time - cur_time);
		}
		else
		{
			msg.set_is_third_recharge_time(0);
		}
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_first_recharge_time_end);
	}

	void cwelfare_mgr::get_welfare_by_id_activatecode(int32 welfare_template_id)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		welfare_proto_get_reward_end msg;
		msg.set_template_id(welfare_template_id);

		if (welfare_template_id != 123)
		{
			msg.set_get_result(e_welfare_cant_get);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
			return;
		}

		//没有奖品表,手动设置奖品
		std::vector<s_item_template_info> item_list;
		item_list.push_back({ 31010024,3, 1 });
		item_list.push_back({ 31020104,2,1 });
		item_list.push_back({ 31020160, 1, 1 });

		int32 total_item_num = item_list.size();
		if (!player_ref.get_item_set().is_empty_slot_enough(e_bag_type_bag, total_item_num))
		{
			msg.set_get_result(e_welfare_no_empty_slot);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
			return;
		}
		msg.set_get_result(0);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
		player_ref.get_item_set().put_in_bag(e_server_log_add_item_activate_code, 0, item_list);
		player_ref.send_promp_msg_to_client(item_list);
	}

	bool cwelfare_mgr::get_welfare_by_first_recharge(int32 recharge_template_id)
	{
		const RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, recharge_template_id);
		if (recharge_template_ptr == nullptr)
		{
			return false;
		}
		int32 recharge_type = recharge_template_ptr->Type;
		if (recharge_type < 0
			|| recharge_type >= 32)
		{
			return false;
		}

		int32	first_recharge_type_geted_value = get_welfare_info(e_welfare_type_first_recharge_geted);
		int32 temp_flag = 1 << recharge_type;
		if (temp_flag & first_recharge_type_geted_value)
		{
			return false;
		}

		player& player_ref = unit_man::get_player(m_unit_array_index);
		int32	first_recharge_reward_money_num = recharge_template_ptr->FirstRechargeRewardMoneyArray.size() / 2;
		for (int32 i = 0; i < first_recharge_reward_money_num; i++)
		{
			int32 money_id = recharge_template_ptr->FirstRechargeRewardMoneyArray[i * 2];
			int64 money_num = recharge_template_ptr->FirstRechargeRewardMoneyArray[i * 2 + 1];
			if (money_id < 0 || money_id >= e_money_type_max)
			{
				continue;
			}
			player_ref.add_money_or_exp(e_money_type(money_id), money_num, e_server_log_add_money_welfare, recharge_template_ptr->attribute_id);
		}
		int32 new_first_recharge_type_geted_value = first_recharge_type_geted_value | temp_flag;
		set_welfare_info(e_welfare_type_first_recharge_geted, new_first_recharge_type_geted_value);
		return true;
	}


	void cwelfare_mgr::get_welfare(int32 welfare_template_id, std::vector<s_item_template_info>& item_list, std::vector<s_item_template_info>& money_list)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (!player_ref.is_valid())
		{
			return;
		}

		WelfareTemplate* TemplatePtr = GET_TEMPLATE(WelfareTemplate, welfare_template_id);
		if (TemplatePtr == nullptr)
		{
			return;
		}
		if (TemplatePtr->RewardItemArray.size() % 2 != 0
			|| TemplatePtr->RewardMoneyArray.size() % 2 != 0
			|| TemplatePtr->AdditionalItemRewardID.size() % 2 != 0
			|| TemplatePtr->AdditionalMoneyRewardID.size() % 2 != 0)
		{
			return;
		}

		int32 welfare_type = TemplatePtr->Type;

		int32 base_item_get_num = TemplatePtr->RewardItemArray.size() / 2;
		int32 base_money_get_num = TemplatePtr->RewardMoneyArray.size() / 2;
		int32 additional_item_get_num = TemplatePtr->AdditionalItemRewardID.size() / 2;
		int32 additional_money_get_num = TemplatePtr->AdditionalMoneyRewardID.size() / 2;

		for (int32 i = 0; i < base_item_get_num; ++i)
		{
			int32 item_id = TemplatePtr->RewardItemArray[i * 2];
			int32 item_num = TemplatePtr->RewardItemArray[i * 2 + 1];
			if (TemplatePtr->VipDoubleRank != 0 && player_ref.get_vip_level() >= TemplatePtr->VipDoubleRank)
			{
				item_num *= 2;
			}
			if (!is_correct_class_type(item_id))
			{
				continue;
			}
			add_welfare_to_vector(item_id, item_num, item_list);
		}
		for (int32 i = 0; i < base_money_get_num; ++i)
		{
			int32 money_id = TemplatePtr->RewardMoneyArray[i * 2];
			int32 money_num = TemplatePtr->RewardMoneyArray[i * 2 + 1];
			add_welfare_to_vector(money_id, money_num, money_list);
		}
		for (int32 i = 0; i < additional_item_get_num; ++i)
		{
			int32 item_id = TemplatePtr->AdditionalItemRewardID[i * 2];
			int32 item_num = TemplatePtr->AdditionalItemRewardID[i * 2 + 1];
			if (!is_correct_class_type(item_id))
			{
				continue;
			}
			add_welfare_to_vector(item_id, item_num, item_list);
		}
		for (int32 i = 0; i < additional_money_get_num; ++i)
		{
			int32 money_id = TemplatePtr->AdditionalMoneyRewardID[i * 2];
			int32 money_num = TemplatePtr->AdditionalMoneyRewardID[i * 2 + 1];
			add_welfare_to_vector(money_id, money_num, money_list);
		}

		if (welfare_type == e_welfare_type_online_time
			|| welfare_type == e_welfare_type_continue_login)
		{
			//这两种需要点击一次，将之前的所有的奖励都领取
			int32 pre_phase_id = welfare_template_id - 1;
			WelfareTemplate* PreTemplatePtr = GET_TEMPLATE(WelfareTemplate, pre_phase_id);
			if (PreTemplatePtr != nullptr && PreTemplatePtr->Type == welfare_type)
			{//前一行表格是同类的福利
				if (!is_welfare_geted(PreTemplatePtr->Type, PreTemplatePtr->Index))
				{//前一行表格没领取过

					//调用自己，把前一行奖励也加到获取列表里
					get_welfare(pre_phase_id, item_list, money_list);
				}
			}
		}
	}

	void cwelfare_mgr::add_welfare_to_vector(int32 id, int32 num, std::vector<s_item_template_info>& id_list)
	{
		//用于将同类的物品
		for (int32 i = 0; i < id_list.size(); ++i)
		{
			if (id_list[i].m_item_id == id)
			{
				ItemTemplate* temp_item_template_ptr = GET_TEMPLATE(ItemTemplate, id_list[i].m_item_id);
				if (temp_item_template_ptr != nullptr)
				{
					//物品还要考虑堆叠数
					int32 pile_num = temp_item_template_ptr->max_pile_num;
					if (id_list[i].m_item_num + num <= pile_num)
					{
						//能全部堆叠到一起
						id_list[i].m_item_num += num;
						return;
					}
					else
					{
						//堆叠不下了，补空，然后跳出，把多出的部分再push
						int32 can_merge_num = pile_num - id_list[i].m_item_num;
						id_list[i].m_item_num = pile_num;
						num -= can_merge_num;
						break;
					}
				}
				else
				{
					id_list[i].m_item_num += num;
					return;
				}
			}
		}
		id_list.push_back({ id, num });
	}


	int32 cwelfare_mgr::get_cur_value_by_welfare_typ(int32 welfare_typ)
	{
		int32 return_value = -1;
		if (welfare_typ < 0 || welfare_typ >= e_welfare_type_max)
		{
			return return_value;
		}

		player& player_ref = unit_man::get_player(m_unit_array_index);
		switch (welfare_typ)
		{
		case e_welfare_type_first_recharge_reward:
		{
			return_value = player_ref.get_unit_info(e_role_info_real_recharge_jewel_num);
		}
		break;
		case e_welfare_type_daily_recharge_reward:
		case e_welfare_type_weekend_recharge_reward:
		{
			return_value = player_ref.get_unit_info(e_role_info_daily_recharge_num);
		}
		break;
		case e_welfare_type_total_recharge_reward:
		{
			return_value = player_ref.get_unit_info(e_role_info_real_recharge_jewel_num);
		}
		break;
		case e_welfare_type_total_cost_reward:
		{
			return_value = player_ref.get_unit_info(e_role_info_total_diamond_cost_num);
		}
		break;
		case e_welfare_type_daily_activity_reward:
		{
			return_value = player_ref.get_unit_info(e_role_info_daily_activity_degree);
		}
		break;
		case e_welfare_type_exp_level_reward:
		{
			return_value = player_ref.get_unit_info(e_role_info_exp_level);
		}
		break;
		case e_welfare_type_gs_reward:
		{
			int64 gs_value = player_ref.get_unit_gs_value();
			return_value = gs_value > int32_MAX_NUM ? int32_MAX_NUM : gs_value;
		}
		break;
		case e_welfare_type_continue_login:
		{
			return_value = player_ref.get_unit_info(e_role_info_continue_login_time);
		}
		break;
		case e_welfare_type_total_login:
		{
			return_value = player_ref.get_unit_info(e_role_info_total_login_days);
		}
		break;
		case e_welfare_type_online_time:
		{
			return_value = get_active_degree_info(e_daily_active_degree_type_online_30_minutes);
		}
		break;
		case e_welfare_type_monthcard_reward:
		{
			return_value = player_ref.get_time_data(e_time_type_month_card_activity_time);
		}
		break;
		case e_welfare_type_time_limit_reward:
		{
			return_value = 0; //没有目标值,直接返回0
		}
		break;
		default:
			break;
		}
		return return_value;
	}

	int32 cwelfare_mgr::get_welfare_by_id_welfare_index(int32 welfare_template_id)
	{
		WelfareTemplate* WelfareTemplatePtr = GET_TEMPLATE(WelfareTemplate, welfare_template_id);
		if (WelfareTemplatePtr == nullptr || WelfareTemplatePtr->ReceiveTime.size() <= 0 || WelfareTemplatePtr->ReceiveTime.size() % 2 != 0)
		{
			return -1;
		}
		int32 index = WelfareTemplatePtr->Index;
		for (int32 i = 0; i < WelfareTemplatePtr->ReceiveTime.size(); i += 2)
		{
			time_info cur_time = time_helper::get_cur_time_new();
			if (cur_time.hour_in_day == WelfareTemplatePtr->ReceiveTime[i])
			{
				return index;
			}
			index++;
		}
		return -1;
	}

	bool cwelfare_mgr::is_welfare_geted(int32 welfare_typ, int32 welfare_index)
	{
		bool is_geted = true;
		if (welfare_typ < 0 || welfare_typ >= e_welfare_type_max)
		{
			return is_geted;
		}
		int32 temp_flag = 1 << welfare_index;
		int32 cur_geted_flag = m_welfare_info.data_ary[welfare_typ];
		switch (welfare_typ)
		{
		case e_welfare_type_continue_login:
		case e_welfare_type_online_time:
		{
			if (welfare_index >= cur_geted_flag)
			{
				is_geted = false;
			}
		}
		break;
		default:
		{
			if (temp_flag & cur_geted_flag)
			{
				is_geted = true;
			}
			else
			{
				is_geted = false;
			}
		}
			break;
		}


		return is_geted;
	}

	void cwelfare_mgr::set_welfare_get_flag(int32 welfare_typ, int32 welfare_index)
	{

		if (welfare_typ < 0 || welfare_typ >= e_welfare_type_max)
		{
			return;
		}
		int32 temp_flag = 1 << welfare_index;
		switch (welfare_typ)
		{
		case e_welfare_type_continue_login:
		case e_welfare_type_online_time:
		{
			m_welfare_info.data_ary[welfare_typ] = welfare_index + 1;
		}
		break;
		default:
		{
			m_welfare_info.data_ary[welfare_typ] |= temp_flag;
		}
			break;
		}
	}

	int32 cwelfare_mgr::get_active_degree_info(e_daily_active_degree_type info_type)
	{
		if ( info_type >= e_daily_active_degree_type_max)
		{
			return -1;
		}
		return m_active_degree_info.data_ary[info_type];
	}

	bool cwelfare_mgr::is_correct_class_type(int32 item_template_id)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		int32 cur_type = player_ref.get_unit_info(e_role_info_class_type);
		ItemTemplate* template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
		if (template_ptr == nullptr)
		{
			return false;
		}
		if (template_ptr->class_type.empty())
		{
			return true;
		}
		for (int32 i = 0; i < template_ptr->class_type.size(); ++i)
		{
			if (template_ptr->class_type[i] == 0
				|| template_ptr->class_type[i] == cur_type)
			{
				return true;
			}
		}
		return false;
	}

	hld::s_welfare_info& cwelfare_mgr::get_welfare_info_ref()
	{
		return m_welfare_info;
	}

	hld::s_active_degree_info& cwelfare_mgr::get_active_degree_info_ref()
	{
		return m_active_degree_info;
	}

	hld::s_welfare_random_get_info& cwelfare_mgr::get_random_get_info_ref()
	{
		return m_random_get_info;
	}

	bool cwelfare_mgr::is_recharge_time(int32 recharge_index)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		i64 first_recharge_time = player_ref.get_time_data(e_time_type_frist_recharge_time);
		tm* now_time = time_helper::get_localtime();
		if (now_time == nullptr)
		{
			return false;
		}
		i64 cur_time = mktime(now_time);
		if (first_recharge_time == 0)
		{
			return false;
		}
		if (cur_time >= first_recharge_time + 86400 * recharge_index)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void cwelfare_mgr::check_in(WelfareTemplate* welfare_template_ptr)
	{
		if (nullptr == welfare_template_ptr)
		{
			return;
		}

		if (is_welfare_geted(e_welfare_type_regular_check_in, welfare_template_ptr->Index))
		{
			return;
		}


		welfare_proto_get_reward_end msg;
		msg.set_template_id(welfare_template_ptr->attribute_id);

		player& player_ref = unit_man::get_player(m_unit_array_index);
		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 current_data = cur_time_info.day_in_month + 1;
		if (current_data < welfare_template_ptr->TargetValue)
		{
			msg.set_get_result(e_item_string_unkown);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
			return;
		}

		bool is_replacement = false;
		if (GAMECONFIG->MonthCardReplacementCost.size() < 3)
		{
			msg.set_get_result(e_item_string_unkown);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
			return;
		}
		int32 replace_money_cost_id = GAMECONFIG->MonthCardReplacementCost[0];
		int32 replace_money_cost_basic_num = GAMECONFIG->MonthCardReplacementCost[1];
		int32 replace_money_cost_coefficient_num = GAMECONFIG->MonthCardReplacementCost[2];
		int32 has_replace_times = get_has_replace_times();
		int32 replace_money_cost_num = replace_money_cost_basic_num + replace_money_cost_coefficient_num * (has_replace_times + 1);
		msg.set_get_result(e_item_string_unkown);
		// 判断是否是补签
		if (cur_time_info.day_in_month != welfare_template_ptr->Index)
		{
			// 补卡
			is_replacement = true;

			if (!player_ref.can_cut_money(e_money_type(replace_money_cost_id), replace_money_cost_num))
			{
				player_ref.get_chat_mgr().send_notice(welfare_template_ptr->NoticeId, template_manager::get_instance().int_to_string(90029046));
				msg.set_get_result(e_item_string_yuanbao_bind);
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
				return;
			}
		}

		std::vector<s_item_template_info> data_array;

		int32 target_value = welfare_template_ptr->TargetValue;
		// 常规签到
		if (false == get_welfare_reward(e_welfare_type_regular_check_in, target_value, msg, data_array))
		{
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
			return;
		}
		set_welfare_get_flag(e_welfare_type_regular_check_in, welfare_template_ptr->Index);
		//设置签到次数
		player_ref.set_logic_data(e_role_logic_info_check_in_days, player_ref.get_logic_data(e_role_logic_info_check_in_days) + 1);
		//更新客户端总签到次数
		player_ref.send_logic_one(e_role_logic_info_check_in_days);

		msg.set_get_result(e_item_string_succeed);
		send_notice(welfare_template_ptr->RewardItemArray, welfare_template_ptr->RewardMoneyArray, welfare_template_ptr->NoticeId);

		// vip签到
		/*if (player_ref.is_recharge_by_type(e_recharge_type_vip))
		{
			if (player_ref.get_vip_level() >= welfare_template_ptr->VipDoubleRank && 0 < welfare_template_ptr->VipDoubleRank)
			{
				target_value = welfare_template_ptr->TargetValue;
				if (false == is_welfare_geted(e_welfare_type_vip_double_check_in, target_value - 1))
				{
					if (false == get_welfare_reward(e_welfare_type_regular_check_in, target_value, msg, data_array))
					{
						player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
						return;
					}
					set_welfare_get_flag(e_welfare_type_vip_double_check_in, target_value - 1);
					msg.set_get_result(e_item_string_succeed);
					send_notice(welfare_template_ptr->RewardItemArray, welfare_template_ptr->RewardMoneyArray, welfare_template_ptr->NoticeId);
				}
			}
		}*/

		//// 月卡签到
		//if (player_ref.is_recharge_by_type(e_recharge_type_month_card) && false == is_replacement)
		//{
		//	target_value = player_ref.get_month_card_current_activity_time();
		//	if (false == is_welfare_geted(e_welfare_type_monthcard_reward, target_value - 1))
		//	{

		//		if (false == get_welfare_reward(e_welfare_type_monthcard_reward, target_value, msg, data_array))
		//		{
		//			player_ref.send_message_to_self(&msg, e_welfare_type_monthcard_reward);
		//			return;
		//		}
		//		set_welfare_get_flag(e_welfare_type_monthcard_reward, target_value - 1);
		//		WelfareTemplate* temp_welfare_template_ptr = template_manager::get_instance().get_welfare_template_ptr_by_type_and_target(e_welfare_type_monthcard_reward, target_value);
		//		if (nullptr != temp_welfare_template_ptr)
		//		{
		//			send_notice(temp_welfare_template_ptr->RewardItemArray, temp_welfare_template_ptr->RewardMoneyArray, temp_welfare_template_ptr->NoticeId);
		//		}

		//	}
		//}
		//// 尊享签到
		//if (player_ref.is_recharge_by_type(e_recharge_type_exclusive) && false == is_replacement)
		//{
		//	target_value = player_ref.get_exclusive_card_current_activity_time() % 31;
		//	if (false == is_welfare_geted(e_welfare_type_exclusive_check_in, target_value - 1))
		//	{
		//		if (false == get_welfare_reward(e_welfare_type_exclusive_check_in, target_value, msg, data_array))
		//		{
		//			player_ref.send_message_to_self(&msg, e_welfare_type_exclusive_check_in);
		//			return;
		//		}
		//		set_welfare_get_flag(e_welfare_type_exclusive_check_in, target_value - 1);
		//		WelfareTemplate* temp_welfare_template_ptr = template_manager::get_instance().get_welfare_template_ptr_by_type_and_target(e_welfare_type_exclusive_check_in, target_value);
		//		if (nullptr != temp_welfare_template_ptr)
		//		{
		//			send_notice(temp_welfare_template_ptr->RewardItemArray, temp_welfare_template_ptr->RewardMoneyArray, temp_welfare_template_ptr->NoticeId);
		//		}
		//	}
		//}
		//补领扣钱
		if (is_replacement)
		{
			player_ref.cut_money(e_money_type(replace_money_cost_id), replace_money_cost_num, e_server_log_cut_money_welfare_replacement);
			add_replace_times();
		}
		set_today_check_in();
		msg.set_is_replacement(is_replacement);

		player_ref.get_item_set().get_item_send_promp_msg_to_client(data_array);
		send_welfare_all();
		// 发送
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);

	}

	void cwelfare_mgr::month_card_check_in(WelfareTemplate* welfare_template_ptr)
	{
		if (nullptr == welfare_template_ptr)
		{
			return;
		}

		if (is_welfare_geted(e_welfare_type_monthcard_reward, welfare_template_ptr->Index))
		{
			return;
		}

		welfare_proto_get_reward_end msg;
		msg.set_template_id(welfare_template_ptr->attribute_id);
		player& player_ref = unit_man::get_player(m_unit_array_index);
		msg.set_get_result(e_item_string_unkown);
		int32 target_value = welfare_template_ptr->TargetValue;
		std::vector<s_item_template_info> data_array;
		// 月卡签到
		if (player_ref.is_recharge_by_type(e_recharge_type_month_card))
		{
			target_value = player_ref.get_month_card_current_activity_time();
			if (false == is_welfare_geted(e_welfare_type_monthcard_reward, target_value - 1))
			{
				if (false == get_welfare_reward(e_welfare_type_monthcard_reward, target_value, msg, data_array))
				{
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
					return;
				}
				msg.set_get_result(e_item_string_succeed);
				set_welfare_get_flag(e_welfare_type_monthcard_reward, target_value - 1);
				WelfareTemplate* temp_welfare_template_ptr = template_manager::get_instance().get_welfare_template_ptr_by_type_and_target(e_welfare_type_monthcard_reward, target_value);
				if (nullptr != temp_welfare_template_ptr)
				{
					send_notice(temp_welfare_template_ptr->RewardItemArray, temp_welfare_template_ptr->RewardMoneyArray, temp_welfare_template_ptr->NoticeId);
				}

			}
		}

		player_ref.get_item_set().get_item_send_promp_msg_to_client(data_array);
		send_welfare_all();
		// 发送
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
	}

	void cwelfare_mgr::exclusive_card_get_reward(WelfareTemplate* welfare_template_ptr)
	{
		if (nullptr == welfare_template_ptr)
		{
			return;
		}

		if (is_welfare_geted(e_welfare_type_exclusive_check_in, welfare_template_ptr->Index))
		{
			return;
		}
		welfare_proto_get_reward_end msg;
		msg.set_template_id(welfare_template_ptr->attribute_id);
		player& player_ref = unit_man::get_player(m_unit_array_index);
		msg.set_get_result(e_item_string_unkown);
		int32 target_value = welfare_template_ptr->TargetValue;
		std::vector<s_item_template_info> data_array;
		if (player_ref.is_recharge_by_type(e_recharge_type_exclusive))
		{
			target_value = player_ref.get_exclusive_card_current_activity_time();
			if (false == is_welfare_geted(e_welfare_type_exclusive_check_in, target_value - 1))
			{
				if (false == get_welfare_reward(e_welfare_type_exclusive_check_in, target_value, msg, data_array))
				{
					player_ref.send_message_to_self(&msg, e_welfare_type_exclusive_check_in);
					return;
				}
				msg.set_get_result(e_item_string_succeed);
				set_welfare_get_flag(e_welfare_type_exclusive_check_in, target_value - 1);
				WelfareTemplate* temp_welfare_template_ptr = template_manager::get_instance().get_welfare_template_ptr_by_type_and_target(e_welfare_type_exclusive_check_in, target_value);
				if (nullptr != temp_welfare_template_ptr)
				{
					send_notice(temp_welfare_template_ptr->RewardItemArray, temp_welfare_template_ptr->RewardMoneyArray, temp_welfare_template_ptr->NoticeId);
				}
			}
		}
		
		player_ref.get_item_set().get_item_send_promp_msg_to_client(data_array);
		send_welfare_all();
		// 发送
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_welfare_end);
	}

	void cwelfare_mgr::refresh_month_info()
	{
		m_welfare_info.data_ary[e_welfare_type_regular_check_in] = 0;
		m_welfare_info.data_ary[e_welfare_type_vip_double_check_in] = 0;
		m_welfare_info.data_ary[e_welfare_type_replace_times] = 0;
		send_welfare_all();
	}

	void cwelfare_mgr::refresh_grand_total_check_in_num()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		if (GAMECONFIG->GrandTotalCheckInNum.size() <= 0)
		{
			return;
		}

		int32 check_size = GAMECONFIG->GrandTotalCheckInNum.size();
		for (int32 i = 0; i < check_size; ++i)
		{
			//判断是否所有奖励都领取了	如果有未领取的就不刷新
			if (!player_ref.get_welfare_mgr().is_welfare_geted(e_welfare_type_cumulative_sign_in, i))
			{
				return;
			}
		}
		int32 max_check_num = GAMECONFIG->GrandTotalCheckInNum[check_size - 1];
		int32 check_in_days = player_ref.get_logic_data(e_role_logic_info_check_in_days);
		if (check_in_days >= max_check_num)
		{
			player_ref.set_logic_data(e_role_logic_info_check_in_days, check_in_days - max_check_num);
			player_ref.set_logic_data(e_role_logic_info_check_in_repetition_num, player_ref.get_logic_data(e_role_logic_info_check_in_repetition_num) + 1);
			m_welfare_info.data_ary[e_welfare_type_cumulative_sign_in] = 0;
		}
	}

	void cwelfare_mgr::retroactive_all_days()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		welfare_proto_retroactive_all_days_end msg;
		welfare_proto_get_reward_end end_msg;
		if (GAMECONFIG->MonthCardReplacementCost.size() < 3)
		{
			msg.set_get_result(e_item_string_unkown);	//表格错误
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_retroactive_all_days_end);
			return;
		}

		int32 replace_money_cost_id = GAMECONFIG->MonthCardReplacementCost[0];	//获取money类型
		int32 replace_money_num = get_retroactive_all_days_money_num();							//获取money数量
		msg.set_get_result(e_item_string_unkown);

		if (!player_ref.can_cut_money(e_money_type(replace_money_cost_id), replace_money_num))	//判读是否够全部补签的money
		{
			msg.set_get_result(e_item_string_yuanbao_bind);	//(绑钻)钻石不足
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_retroactive_all_days_end);
			return;
		}
		time_info now_day = time_helper::get_cur_time_new();						//获得当前是本月第几天
		time_info create_time = time_helper::get_time_by_stamp_new(player_ref.get_unit_i64_info_data(e_role_i64_info_create_time));
		std::vector<s_item_template_info> data_array;
		for (int32 i = 0; i < now_day.day_in_month; ++i)
		{
			//判断创建账号之前的日期不进行补签
			if (now_day.year == create_time.year && now_day.month_in_year == create_time.month_in_year && i < create_time.day_in_month)
			{
				continue;
			}
			if(is_welfare_geted(e_welfare_type_regular_check_in, i))
			{
				continue;
			}
			WelfareTemplate* TemplatePtr = GET_TEMPLATE(WelfareTemplate, daily_ttendance_begin_template_id + i);
			if (TemplatePtr == nullptr)
			{
				msg.set_get_result(e_item_string_unkown);	//表格错误直接结束 
				player_ref.send_message_to_self(&msg, e_mgsindex_s2c_retroactive_all_days_end);
				continue;									//防止前面奖励发送成功后面出现表格错误没有扣除钻石 防止刷物品
			}
			if (false == get_welfare_reward(e_welfare_type_regular_check_in, TemplatePtr->TargetValue, end_msg, data_array))
			{
				player_ref.send_message_to_self(&msg, e_mgsindex_s2c_retroactive_all_days_end);
				continue;
			}
			set_welfare_get_flag(e_welfare_type_regular_check_in, TemplatePtr->Index);
			//设置签到次数
			player_ref.set_logic_data(e_role_logic_info_check_in_days, player_ref.get_logic_data(e_role_logic_info_check_in_days) + 1);
			//设置补签次数
			add_replace_times();
		}
		//更新客户端总签到次数
		player_ref.send_logic_one(e_role_logic_info_check_in_days);
		msg.set_get_result(e_item_string_succeed);

		player_ref.cut_money(e_money_type(replace_money_cost_id), replace_money_num, e_server_log_cut_money_welfare_replacement);
		player_ref.get_item_set().get_item_send_promp_msg_to_client(data_array);
		//同步到客户端
		send_welfare_all();
		player_ref.send_message_to_self(&msg, e_mgsindex_s2c_retroactive_all_days_end);
	}

	int32  cwelfare_mgr::get_retroactive_all_days_money_num()
	{

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return -1;
		}
		if (GAMECONFIG->MonthCardReplacementCost.size() < 3)
		{
			return -1;
		}
		int32 replace_money_cost_id = GAMECONFIG->MonthCardReplacementCost[0];
		int32 replace_money_cost_basic_num = GAMECONFIG->MonthCardReplacementCost[1];
		int32 replace_money_cost_coefficient_num = GAMECONFIG->MonthCardReplacementCost[2];
		int32 has_replace_times = get_has_replace_times();
		int32 replace_money_num = 0;
		int32 retroactive_days_num = 0;

		time_info now_day = time_helper::get_cur_time_new();						//获得当前是本月第几天
		time_info create_time = time_helper::get_time_by_stamp_new(player_ref.get_unit_i64_info_data(e_role_i64_info_create_time));
		for (int32 i = 0; i < now_day.day_in_month; ++i)
		{
			//判断创建账号之前的日期不进行补签
			if (now_day.year == create_time.year && now_day.month_in_year == create_time.month_in_year && i < create_time.day_in_month)
			{
				continue;
			}
			if (!is_welfare_geted(e_welfare_type_regular_check_in, i))
			{
				retroactive_days_num++;
				replace_money_num += replace_money_cost_basic_num + replace_money_cost_coefficient_num * (has_replace_times + retroactive_days_num);
			}
		}
		return replace_money_num;
	}

	void cwelfare_mgr::get_cumulative_sign_in_reward(int32 index)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid() || index < 0)
		{
			return;
		}
		welfare_proto_get_cumulative_sign_in_reward_end msg;
		if (GAMECONFIG->GrandTotalCheckInNum.size() < index || GAMECONFIG->GrandTotalCheckInReward.size() % 8 != 0)
		{
			msg.set_get_result(e_item_string_unkown);			//表格错误 未知错误
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_get_cumulative_sign_in_reward_end);
			return;
		}
		
		int32 check_in_num = player_ref.get_logic_data(e_role_logic_info_check_in_days);
		int32 check_in_repetition_num = player_ref.get_logic_data(e_role_logic_info_check_in_repetition_num);
		if (check_in_num < GAMECONFIG->GrandTotalCheckInNum[index])
		{
			msg.set_get_result(e_welfare_cant_get);				//不够领取资格
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_get_cumulative_sign_in_reward_end);
			return;
		}
		if (is_welfare_geted(e_welfare_type_cumulative_sign_in, index))
		{
			msg.set_get_result(e_welfare_already_geted);		//已经领取过了
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_get_cumulative_sign_in_reward_end);
			return;
		}
		int32 begin_index = check_in_repetition_num < GAMECONFIG->GrandTotalCheckInReward.size() / 8 ? check_in_repetition_num * 8 : GAMECONFIG->GrandTotalCheckInReward.size() - 8;
		std::vector<s_item_template_info> item_list;
		if (GAMECONFIG->GrandTotalCheckInReward.size() < ((begin_index + (index * 2)) + 1))
		{
			msg.set_get_result(e_item_string_unkown);			//表格错误 未知错误
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_get_cumulative_sign_in_reward_end);
			return;
		}

		item_list.push_back({ GAMECONFIG->GrandTotalCheckInReward[(begin_index + (index * 2))],GAMECONFIG->GrandTotalCheckInReward[(begin_index + (index * 2)) + 1], 1 });		//物品id																		
		player_ref.get_item_set().put_in_bag(e_server_log_add_item_cumulative_sign_in, index, item_list);						//将物品放入背包

		set_welfare_get_flag(e_welfare_type_cumulative_sign_in, index);				//刷新标记位
		send_welfare_all();															//发送给客户端
		msg.set_get_result(e_item_string_succeed);									//领取成功
		player_ref.send_message_to_self(&msg, e_mgsindex_s2c_get_cumulative_sign_in_reward_end);
	}

	int32 cwelfare_mgr::data_to_target_value(const e_welfare_type welfare_type, const int32 target_value)
	{
		int32 result = -1;
		//const int32 today_data = time_helper::get_day();
		player& player_ref = unit_man::get_player(m_unit_array_index);
		switch (welfare_type)
		{
		case e_welfare_type_regular_check_in:
			result = target_value;
			break;
		case e_welfare_type_monthcard_reward:
		{
			result = player_ref.get_month_card_current_activity_time();
		}
		break;
		case e_welfare_type_exclusive_check_in:
			result = target_value;
			break;
		case e_welfare_type_vip_double_check_in:
			result = target_value;
			break;
		default:
			break;
		}
		return result;
	}

	bool cwelfare_mgr::is_today_check_in()
	{
		return (0 != m_welfare_info.data_ary[e_welfare_type_is_today_check_in]);
	}

	void cwelfare_mgr::set_today_check_in()
	{
		m_welfare_info.data_ary[e_welfare_type_is_today_check_in] = 1;
	}

	void cwelfare_mgr::send_notice(const std::vector<int32>& item_array, const std::vector<int32>& money_array, int32 notice_id)
	{
		//player& player_ref = unit_man::get_player(m_unit_array_index);
		//std::vector<int32> reward_item_array = item_array;
		//int32 temp_array_size = reward_item_array.size();
		//if (temp_array_size % 2 != 0)
		//{
		//	return;
		//}
		//std::string item_name = "";
		//for (int32 i = 0; i < temp_array_size; i = i + 2)
		//{
		//	ItemTemplate* item_template_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(reward_item_array[i]);
		//	if (nullptr == item_template_ptr)
		//	{
		//		continue;
		//	}
		//	std::string temp_name = "";
		//	if (template_manager::get_instance().get_str_by_attribute_id(temp_name, item_template_ptr->item_name))
		//	{
		//		item_name = item_name + template_manager::get_instance().int_to_string(reward_item_array[i + 1]) + temp_name + " ";
		//	}
		//}


		//const int32 string_length_def = 2;
		//std::vector<std::string> vec_str_up_item;
		//if (template_manager::get_instance().get_vec_str_by_notice_id(vec_str_up_item, notice_id)
		//	&& string_length_def == vec_str_up_item.size())
		//{
		//	std::string notice_str = vec_str_up_item[0]
		//		//	+ std::string(player_ref.get_name())
		//		+ item_name;
		//	player_ref.get_chat_mgr().send_notice(notice_id, notice_str);
		//}
	}

	int32 cwelfare_mgr::get_check_in_times()
	{
		int32 check_in_times = 0;
		for (int32 i = 0; i < 31; i++)
		{
			if (false == is_welfare_geted(hld::e_welfare_type_regular_check_in, i))
			{
				return check_in_times;
			}
			check_in_times++;
		}
		return check_in_times;
	}

	void cwelfare_mgr::add_replace_times()
	{
		m_welfare_info.data_ary[e_welfare_type_replace_times]++;
	}

	int32 cwelfare_mgr::get_has_replace_times()
	{
		return m_welfare_info.data_ary[e_welfare_type_replace_times];
	}

	bool cwelfare_mgr::get_welfare_reward(e_welfare_type welfare_type, const int32 welfare_target_value, welfare_proto_get_reward_end& msg, std::vector<s_item_template_info>& data_array)
	{
		WelfareTemplate* TemplatePtr = template_manager::get_instance().get_welfare_template_ptr_by_type_and_target(welfare_type, welfare_target_value);
		if (TemplatePtr == nullptr)
		{
			msg.set_get_result(e_item_string_unkown);
			return false;
		}

		if (TemplatePtr->RewardItemArray.size() % 2 != 0
			|| TemplatePtr->RewardMoneyArray.size() % 2 != 0
			|| TemplatePtr->AdditionalItemRewardID.size() % 2 != 0
			|| TemplatePtr->AdditionalMoneyRewardID.size() % 2 != 0)
		{
			return false;
		}

		std::vector<s_item_template_info> item_list;
		std::vector<s_item_template_info> money_list;
		int32 welfare_template_id = TemplatePtr->attribute_id;
		get_welfare(welfare_template_id, item_list, money_list);

		int32 total_item_num = item_list.size();
		player& player_ref = unit_man::get_player(m_unit_array_index);

		player_ref.get_item_set().put_in_bag(e_server_log_add_item_get_welfare, welfare_template_id, item_list);

		for (int32 i = 0; i < item_list.size(); ++i)
		{
			int32 item_template_id = item_list[i].m_item_id;
			int32 item_num = item_list[i].m_item_num;

			msg.add_item_get(item_template_id);
			msg.add_item_get(item_num);

			data_array.push_back({ item_template_id, item_num, 1});
		}

		for (int32 i = 0; i < money_list.size(); ++i)
		{
			int32 money_id = money_list[i].m_item_id;
			int64 money_num = money_list[i].m_item_num;
			player_ref.add_money_or_exp(e_money_type(money_id), money_num, e_server_log_add_money_welfare, welfare_template_id);
			msg.add_money_get(money_id);
			msg.add_money_get(money_num);
		}
		return true;
	}

}