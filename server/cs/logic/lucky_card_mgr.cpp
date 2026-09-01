/********************************************************************
created: 2021年3月24日
file base: lucky_card_mgr
file ext: cpp
author: zhaoyuming
*********************************************************************/

#include "internet/game.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "lucky_card_mgr.h"
#include "internet/net.pb.h"
#include "system/item/item_system.h"
#include "time_def.hpp"
#include "time_limit_activity_object.h"
#include "utility/random.h"
#include "utility/init_unit.h"



namespace faith
{
	lucky_card_mgr::lucky_card_mgr()
	{

	}

	lucky_card_mgr::~lucky_card_mgr()
	{

	}

	void lucky_card_mgr::clear_data()
	{
		m_init_end = false;
	}

	void lucky_card_mgr::init_data()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		if (!player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_lucky_card))
		{
			return;
		}
		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			return;
		}
		const s_time_limit_activity_branch_temp& branch_template_ptr = lucky_card_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return;
		}

		//表示初始化结束
		m_init_end = true;
	}

	void lucky_card_mgr::heart_tick(const int64 & new_time)
	{
		if (m_lucky_card_info.state_info == e_lucak_card_game_state_type_end)
		{
			if (new_time >= m_clear_time)
			{
				m_lucky_card_info.end_clear();
				m_clear_time = 0;
				send_lucky_card_to_client();
			}
		}
	}

	void lucky_card_mgr::refresh_recycle()
	{
		clear_data();
	}

	void lucky_card_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	void lucky_card_mgr::load_lucky_card_end(s_time_limit_activity_info activity_info, bool is_init)
	{
		m_lucky_card_info.activity_guid = activity_info.data_ary[e_time_limit_activity_info_guid];
		m_lucky_card_info.state_info = activity_info.activity_schedule[e_lucak_card_set_activity_state_info];
		if (is_init)
		{
			m_lucky_card_info.is_have_free = 1;
		}
		else
		{
			m_lucky_card_info.is_have_free = activity_info.activity_schedule[e_lucak_card_set_activity_is_have_free];
		}
		m_lucky_card_info.end_info = activity_info.activity_schedule[e_lucak_card_set_activity_end_info];
		m_lucky_card_info.end_item_id = activity_info.activity_schedule[e_lucak_card_set_activity_end_item_id];
		m_lucky_card_info.no_get_num = activity_info.activity_schedule[e_lucak_card_set_activity_no_get_num];
		m_lucky_card_info.lucky_card_info = activity_info.activity_schedule[e_lucak_card_set_activity_lucky_card_info];
		m_lucky_card_info.set_item_array(activity_info.schedule_str);
		init_data();
		send_lucky_card_to_client();
	}

	void lucky_card_mgr::set_limit_activiy_info()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (false == lucky_card_ptr.is_valid())
		{
			return;
		}
		lucky_card_ptr.set_activity_schedule(m_lucky_card_info.state_info,		e_lucak_card_set_activity_state_info);
		lucky_card_ptr.set_activity_schedule(m_lucky_card_info.is_have_free,	e_lucak_card_set_activity_is_have_free);
		lucky_card_ptr.set_activity_schedule(m_lucky_card_info.end_info,		e_lucak_card_set_activity_end_info);
		lucky_card_ptr.set_activity_schedule(m_lucky_card_info.end_item_id,		e_lucak_card_set_activity_end_item_id);
		lucky_card_ptr.set_activity_schedule(m_lucky_card_info.no_get_num,		e_lucak_card_set_activity_no_get_num);
		lucky_card_ptr.set_activity_schedule(m_lucky_card_info.lucky_card_info, e_lucak_card_set_activity_lucky_card_info);
		lucky_card_ptr.set_activity_schedule_str(m_lucky_card_info.get_item_array);
	}

	void lucky_card_mgr::send_lucky_card_to_client()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		game_proto_refresh_lucky_card_info msg;
		msg.set_activity_guid(m_lucky_card_info.activity_guid);
		msg.set_state_info(m_lucky_card_info.state_info);
		msg.set_is_have_free(m_lucky_card_info.is_have_free);
		msg.set_lucky_card_info(m_lucky_card_info.lucky_card_info);
		if (m_lucky_card_info.state_info == e_lucak_card_game_state_type_end)
		{
			msg.set_end_item_id(m_lucky_card_info.end_item_id);
		}
		else
		{
			msg.set_end_item_id(0);
		}
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_refresh_lucky_card_info);
	}

	void lucky_card_mgr::send_operate_end(e_lucak_card_operate_end error_index)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		game_proto_lucky_card_operate_end msg;
		msg.set_error_index(error_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_lucky_card_operate_end);
	}

	void lucky_card_mgr::send_lucky_card_info(int32 card_index)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		game_proto_refresh_lucky_card msg;
		msg.set_card_index(card_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_refresh_lucky_card);
	}

	void lucky_card_mgr::next_state(int32 touch_index, int32 is_jump)
	{
		switch (m_lucky_card_info.state_info)
		{
		case e_lucak_card_game_state_type_begin:
		{
			//服务器做一下拦截防止客户端多次点击翻牌导致状态错乱
			if (touch_index != 0)
			{
				return;
			}
			if (is_jump)
			{
				jump_game();
			}
			else
			{
				game_begin();
			}
		}
		break;
		case e_lucak_card_game_state_type_first:
		{
			game_next(touch_index);
		}
		break;
		case e_lucak_card_game_state_type_second:
		{
			game_next(touch_index);
		}
		break;
		case e_lucak_card_game_state_type_three:
		{
			game_next(touch_index);
		}
		break;
		default:
			break;
		}

	}

	void lucky_card_mgr::game_begin()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			send_operate_end(e_lucak_card_operate_end_not_open);
			return;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = lucky_card_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0 || branch_template_ptr.Condition.size() < 2 || branch_template_ptr.ParamArr2.size() != 1)
		{
			return;
		}

		bool is_can = false;
		bool is_use_free = false;
		//检查是否是免费次数
		if (m_lucky_card_info.is_have_free > 0)
		{
			is_use_free = true;
			is_can = true;
		}
		else
		{
			// 检查消耗物品是否充足
			int32 item_id = branch_template_ptr.Condition[0];
			int32 item_num = branch_template_ptr.Condition[1];
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, item_id, item_num))
			{
				item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, item_id, item_num);
				is_use_free = false;
				is_can = true;
			}
			else
			{
				send_operate_end(e_lucak_card_operate_end_not_have_item);
				return;
			}
		}

		// 设置消费类型
		int32 use_type = e_lucak_card_use_type_normal;
		if (is_use_free)
		{
			use_type = e_lucak_card_use_type_free;
			m_lucky_card_info.is_have_free = 0;
		}
		else if (m_lucky_card_info.no_get_num >= branch_template_ptr.ParamArr2[0])
		{
			use_type = e_lucak_card_use_type_award;
			m_lucky_card_info.no_get_num = 0;
		}
		else
		{
			use_type = e_lucak_card_use_type_normal;
			m_lucky_card_info.no_get_num += 1;
		}

		// 设置奖励类型
		int32 award_type = get_award_type_by_use_type(use_type);
		m_lucky_card_info.end_info = award_type;
		if (award_type == e_lucak_card_award_type_special || award_type == e_lucak_card_award_type_first)
		{
			m_lucky_card_info.no_get_num = 0;
		}

		// 根据奖励类型获取奖励列表
		std::vector<s_lucky_card_award_item_info> item_list = get_item_array_by_award_type(award_type);
	
		// 根据奖励列表随机出最终奖品
		s_lucky_card_award_item_info item_info = get_award_by_item_list(item_list);
	
		// 记录最终奖励
		m_lucky_card_info.end_item_id = item_info.get_item_info(e_lucak_card_item_info_item_index);
	
		// 增加奖励领取次数
		add_award_get_num(item_info.get_item_info(e_lucak_card_item_info_item_index), 1);
		
		// 检查奖励领取次数
		check_award_get_num(award_type);
		
		// 设置进度状态
		m_lucky_card_info.state_info = e_lucak_card_game_state_type_first;

		// 设置翻牌数据
		m_lucky_card_info.lucky_card_info = 0;

		//同步数据到客户端
		send_lucky_card_to_client();
	}

	void lucky_card_mgr::jump_game()
	{

		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			send_operate_end(e_lucak_card_operate_end_not_open);
			return;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = lucky_card_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0 || branch_template_ptr.Condition.size() < 2 || branch_template_ptr.ParamArr2.size() != 1)
		{
			return;
		}

		bool is_can = false;
		bool is_use_free = false;
		//检查是否是免费次数
		if (m_lucky_card_info.is_have_free > 0)
		{
			is_use_free = true;
			is_can = true;
		}
		else
		{
			// 检查消耗物品是否充足
			int32 item_id = branch_template_ptr.Condition[0];
			int32 item_num = branch_template_ptr.Condition[1];
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, item_id, item_num))
			{
				item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, item_id, item_num);
				is_use_free = false;
				is_can = true;
			}
			else
			{
				send_operate_end(e_lucak_card_operate_end_not_have_item);
				return;
			}
		}

		// 设置消费类型
		int32 use_type = e_lucak_card_use_type_normal;
		if (is_use_free)
		{
			use_type = e_lucak_card_use_type_free;
			m_lucky_card_info.is_have_free = 0;
		}
		else if (m_lucky_card_info.no_get_num >= branch_template_ptr.ParamArr2[0])
		{
			use_type = e_lucak_card_use_type_award;
			m_lucky_card_info.no_get_num = 0;
		}
		else
		{
			use_type = e_lucak_card_use_type_normal;
			m_lucky_card_info.no_get_num += 1;
		}

		// 设置奖励类型
		int32 award_type = get_award_type_by_use_type(use_type);
		m_lucky_card_info.end_info = award_type;
		if (award_type == e_lucak_card_award_type_special || award_type == e_lucak_card_award_type_first)
		{
			m_lucky_card_info.no_get_num = 0;
		}

		// 根据奖励类型获取奖励列表
		std::vector<s_lucky_card_award_item_info> item_list = get_item_array_by_award_type(award_type);

		// 根据奖励列表随机出最终奖品
		s_lucky_card_award_item_info item_info = get_award_by_item_list(item_list);

		// 增加奖励领取次数
		add_award_get_num(item_info.get_item_info(e_lucak_card_item_info_item_index), 1);

		// 检查奖励领取次数
		check_award_get_num(award_type);

		// 设置进度状态
		m_lucky_card_info.state_info = e_lucak_card_game_state_type_end;

		// 记录最终奖励
		m_lucky_card_info.end_item_id = item_info.get_item_info(e_lucak_card_item_info_item_index);

		// 发放奖励
		s_lucky_card_award_item_info reward_info = get_award_by_item_index(m_lucky_card_info.end_item_id);
		std::vector<int32> item_arr;
		item_arr.push_back(reward_info.get_item_info(e_lucak_card_item_info_item_id));
		item_arr.push_back(reward_info.get_item_info(e_lucak_card_item_info_item_num));
		player_ref.get_item_set().put_in_bag(e_server_log_add_item_lucky_card, m_lucky_card_info.end_item_id, item_arr);

		if (m_lucky_card_info.end_info == e_lucak_card_award_type_special)
		{
			send_reward_notice(reward_info.get_item_info(e_lucak_card_item_info_item_id));
		}

		int64 time_now = utility::get_tick_count();
		m_clear_time = time_now + (1 * second_tick_time);

		//同步数据到客户端
		send_lucky_card_to_client();
	}

	void lucky_card_mgr::game_next(int32 touch_index)
	{
		if (touch_index < 1 || touch_index > 9)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			send_operate_end(e_lucak_card_operate_end_not_open);
			return;
		}

		if (m_lucky_card_info.get_lucky_card_info(touch_index) != 0)
		{
			send_operate_end(e_lucak_card_operate_end_have_touch);
			return;
		}

		int32 touch_result = get_next_touch_result();
		m_lucky_card_info.set_lucky_card_info(touch_index, touch_result);
		m_lucky_card_info.state_info += 1;
		if (m_lucky_card_info.state_info == e_lucak_card_game_state_type_end)
		{
			s_lucky_card_award_item_info reward_info = get_award_by_item_index(m_lucky_card_info.end_item_id);
			std::vector<int32> item_arr;
			item_arr.push_back(reward_info.get_item_info(e_lucak_card_item_info_item_id));
			item_arr.push_back(reward_info.get_item_info(e_lucak_card_item_info_item_num));
			player_ref.get_item_set().put_in_bag(e_server_log_add_item_lucky_card, m_lucky_card_info.end_item_id,item_arr);
			if (m_lucky_card_info.end_info == e_lucak_card_award_type_special) //|| m_lucky_card_info.end_info == e_lucak_card_award_type_first)
			{
				send_reward_notice(reward_info.get_item_info(e_lucak_card_item_info_item_id));
			}
			int64 time_now = utility::get_tick_count();
			m_clear_time = time_now + (2 * second_tick_time);
		}
		send_lucky_card_to_client();
	}

	void lucky_card_mgr::add_award_get_num(int32 item_index, int32 add_num)
	{
		std::vector<int32> arr_value;
		init_unit::parse_char_to_vector(arr_value, m_lucky_card_info.get_item_array);

		bool is_have = false;
		for (int32 i = 0; i < arr_value.size() / 2; i++)
		{
			if (arr_value[i * 2] == item_index)
			{
				is_have = true;
				arr_value[i * 2 + 1] += add_num;
				break;
			}
		}

		if (false == is_have)
		{
			arr_value.push_back(item_index);
			arr_value.push_back(add_num);
		}

		xstring temp_str;
		init_unit::parse_vector_to_char(temp_str, arr_value);
		m_lucky_card_info.set_item_array(temp_str);
	}

	void lucky_card_mgr::check_award_get_num(int32 award_type)
	{
		std::vector<s_lucky_card_award_item_info> item_array = get_item_array_by_award_type(award_type);
		std::vector<int32> arr_value;
		init_unit::parse_char_to_vector(arr_value, m_lucky_card_info.get_item_array);
		for (int32 i = 0; i < item_array.size(); ++i)
		{
			for (int32 j = 0; j < arr_value.size() / 2; ++j)
			{
				if (item_array[i].get_item_info(e_lucak_card_item_info_item_index) == arr_value[j * 2])
				{
					arr_value[j * 2 + 1] = item_array[i].get_num;
					break;
				}
			}
		}
		xstring temp_str;
		init_unit::parse_vector_to_char(temp_str, arr_value);
		m_lucky_card_info.set_item_array(temp_str);
	}

	int32 lucky_card_mgr::get_award_get_num(int32 item_index)
	{
		std::vector<int32> arr_value;
		init_unit::parse_char_to_vector(arr_value, m_lucky_card_info.get_item_array);
		int32 have_num = 0;

		for (int32 i = 0; i < arr_value.size() / 2; i++)
		{
			if (arr_value[i * 2] == item_index)
			{
				have_num = arr_value[i * 2 + 1];
				break;
			}
		}
		return have_num;
	}

	std::vector<s_lucky_card_award_item_info> lucky_card_mgr::get_item_array_by_award_type(int32 award_type)
	{
		std::vector<s_lucky_card_award_item_info> item_array;

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return item_array;
		}

		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			return item_array;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = lucky_card_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return item_array;
		}

		std::vector<int32> item_list = branch_template_ptr.Reward;
		if (item_list.size() <= 0 || (item_list.size() % e_lucak_card_item_info_max) != 0)
		{
			return item_array;
		}

		// 当前已获得奖励列表
		std::vector<int32> arr_value;
		init_unit::parse_char_to_vector(arr_value, m_lucky_card_info.get_item_array);

		int32 get_num = 0;

		for (int32 i = 0; i < item_list.size() / e_lucak_card_item_info_max; i++)
		{
			if (item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_award_type] == award_type)
			{
				s_lucky_card_award_item_info item_info;
				item_info.set_item_info(e_lucak_card_item_info_award_type, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_award_type]);
				item_info.set_item_info(e_lucak_card_item_info_item_index, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_index]);
				item_info.set_item_info(e_lucak_card_item_info_item_id, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_id]);
				item_info.set_item_info(e_lucak_card_item_info_item_num, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_num]);
				item_info.set_item_info(e_lucak_card_item_info_basice_weight, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_basice_weight]);
				item_info.set_item_info(e_lucak_card_item_info_change_weight, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_change_weight]);
				item_info.get_num = 0;
				for (int32 j = 0; j < arr_value.size() / 2; j++)
				{
					if (arr_value[j * 2] == item_info.get_item_info(e_lucak_card_item_info_item_index))
					{
						item_info.get_num = arr_value[j * 2 + 1];
						if (item_info.get_num < get_num)
						{
							get_num = item_info.get_num;
						}
						break;
					}
				}
				item_array.push_back(item_info);
			}
		}

		if (get_num > 0)
		{
			for (int32 i = 0; i < item_array.size(); i++)
			{
				item_array[i].get_num -= get_num;
			}
		}
		return item_array;
	}

	s_lucky_card_award_item_info lucky_card_mgr::get_award_by_item_list(std::vector<s_lucky_card_award_item_info>& item_list)
	{
		s_lucky_card_award_item_info award_item;
		if (item_list.size() <= 0)
		{
			return award_item;
		}

		int64 total_weight = 0;
		for (int32 i = 0; i < item_list.size(); ++i)
		{
			total_weight += item_list[i].math_weight();
		}

		int32 random_value = random_gen::get_random(0, total_weight);
		int32 cur_weight = 0;
		for (int32 i = 0; i < item_list.size(); i++)
		{
			cur_weight += item_list[i].math_weight();
			if (cur_weight >= random_value)
			{
				award_item = item_list[i];
				break;
			}
		}
		return award_item;
	}

	s_lucky_card_award_item_info lucky_card_mgr::get_award_by_item_index(int32 item_index)
	{
		s_lucky_card_award_item_info award_item;

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return award_item;
		}

		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			return award_item;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = lucky_card_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return award_item;
		}

		std::vector<int32> item_list = branch_template_ptr.Reward;
		if (item_list.size() <= 0 || (item_list.size() % e_lucak_card_item_info_max) != 0)
		{
			return award_item;
		}

		for (int32 i = 0; i < item_list.size() / e_lucak_card_item_info_max; i++)
		{
			if (item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_index] == item_index)
			{
				award_item.set_item_info(e_lucak_card_item_info_award_type, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_award_type]);
				award_item.set_item_info(e_lucak_card_item_info_item_index, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_index]);
				award_item.set_item_info(e_lucak_card_item_info_item_id, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_id]);
				award_item.set_item_info(e_lucak_card_item_info_item_num, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_item_num]);
				award_item.set_item_info(e_lucak_card_item_info_basice_weight, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_basice_weight]);
				award_item.set_item_info(e_lucak_card_item_info_change_weight, item_list[i * e_lucak_card_item_info_max + e_lucak_card_item_info_change_weight]);
				break;
			}
		}
		return award_item;
	}

	int32 lucky_card_mgr::get_award_type_by_use_type(int32 use_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return 0;
		}

		time_limit_activity_object& lucky_card_ptr = player_ref.get_time_limit_activity_mgr().get_act_obj_by_act_guid(m_lucky_card_info.activity_guid);
		if (lucky_card_ptr.is_valid() == false)
		{
			return 0;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = lucky_card_ptr.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return 0;
		}

		std::vector<int32> award_list;
		switch (use_type)
		{
		case e_lucak_card_use_type_free:
		{
			award_list = branch_template_ptr.ParamArr5;
		}
		break;
		case e_lucak_card_use_type_normal:
		{
			award_list = branch_template_ptr.ParamArr1;
		}
		break;
		case e_lucak_card_use_type_award:
		{
			award_list = branch_template_ptr.ParamArr3;
		}
		break;
		default:
			break;
		}

		if (award_list.size() / 2 <= 0 || award_list.size() % 2 != 0)
		{
			return 0;
		}

		int64 total_weight = 0;
		for (int32 i = 0; i < award_list.size() / 2; ++i)
		{
			total_weight += award_list[i * 2 + 1];
		}

		int32 random_value = random_gen::get_random(1, total_weight);
		int32 cur_weight = 0;
		for (int32 i = 0; i < award_list.size() / 2; i++)
		{
			cur_weight += award_list[i * 2 + 1];
			if (cur_weight >= random_value)
			{
				return award_list[i * 2];
			}
		}

		return 0;
	}

	void lucky_card_mgr::refresh_free_num()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		if (m_lucky_card_info.is_have_free == 1)
		{
			return;
		}
		m_lucky_card_info.is_have_free = 1;
		send_lucky_card_to_client();
	}

	int32 lucky_card_mgr::get_next_touch_result()
	{
		int32 touch_result = e_lucak_card_info_null;
		int32 reward_type = m_lucky_card_info.end_info;
		// 根据奖励类型计算每种颜色的数量	奖励类型1时 红色3个 黑色0个 类型每加一级 红色减一个 黑色加一个
		int32 red_num = 3 - (reward_type - 1);
		int32 black_num = reward_type - 1;

		red_num -= m_lucky_card_info.get_lucky_card_color_num(e_lucak_card_info_red);
		black_num -= m_lucky_card_info.get_lucky_card_color_num(e_lucak_card_info_black);
		
		if (0 >= red_num)
		{
			touch_result = e_lucak_card_info_black;
		}
		else if (0 >= black_num)
		{
			touch_result = e_lucak_card_info_red;
		}
		else
		{
			int32 all_num = red_num + black_num;
			int32 random_value = random_gen::get_random(1, all_num);
			if (random_value <= red_num)
			{
				touch_result = e_lucak_card_info_red;
			}
			else
			{
				touch_result = e_lucak_card_info_black;
			}
		}
		return touch_result;
	}

	void lucky_card_mgr::send_reward_notice(int32 item_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (temp_template_ptr == nullptr)
		{
			return;
		}
		std::vector<std::string> vec_notice_str;

		std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(lucak_card_notice_id);
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(player_ref.get_name());
		notice_str_params_vec.push_back(template_manager::get_instance().get_str_by_string_template_id(temp_template_ptr->ItemName));
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		player_ref.get_chat_mgr().send_notice(lucak_card_notice_id, notice_str);
	}


	




}