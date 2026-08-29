/********************************************************************
created: 2017年8月10日11:27:00
file base: service_goal
file ext: cpp
author: 韩泽成
purpose: 七日目标头文件
*********************************************************************/

#include "arena_def.hpp"
#include "internet/net.pb.h"
#include "internet/service_goal.pb.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "service_goal.hpp"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "time_def.hpp"
#include "utility/globle_data.h"
#include "utility/init_unit.h"

namespace faith
{
	
	cservice_goal::cservice_goal()
	{
		m_player_index = 0;
		clear_data();
	}

	cservice_goal::~cservice_goal()
	{

	}

	void cservice_goal::set_player_index(int32 player_index)
	{
		m_player_index = player_index;
	}

	void cservice_goal::clear_data()
	{
		m_template_ptr = nullptr;
		m_service_goal_info.clear_data();
	}

	void cservice_goal::init_service_goal(ServiceGoalTemplate* template_ptr, const s_service_goal_info service_goal_info)
	{
		m_template_ptr = template_ptr;
		m_service_goal_info = service_goal_info;
	}

	const bool cservice_goal::is_unfinished(void)
	{
		if (e_service_goal_state_unfinished == get_service_goal_state())
		{
			return true;
		}
		return false;
	}

	const bool cservice_goal::is_finished(void)
	{
		if (e_service_goal_state_finished == get_service_goal_state())
		{
			return true;
		}
		return false;
	}

	const bool cservice_goal::is_received(void)
	{
		if (e_service_goal_state_has_been_received == get_service_goal_state())
		{
			return true;
		}
		return false;
	}

	const bool cservice_goal::is_empty()
	{
		if (true == is_unfinished() && 0 == get_service_goal_schedule())
		{
			return true;
		}
		return false;
	}

	const bool cservice_goal::is_non_nullptr()
	{
		player& temp_player = get_player();
		if (nullptr == m_template_ptr || false == temp_player.is_valid())
		{
			return false;
		}
		return true;
	}

	void cservice_goal::received_service_goal(void)
	{
		if (false == is_non_nullptr())
		{
			return;
		}
		int32   operate_result = can_received_service_goal();
		if (e_item_string_succeed == operate_result)
		{
			bool result = put_reward_into_bag();
			if (true == result)
			{
				set_service_goal_state(e_service_goal_state_has_been_received);	
			}
			else
			{
				operate_result = 0;
			}
		}
		send_service_goal_operate_result_to_self((int32)e_service_goal_operate_type_receive, operate_result);
	}

	void cservice_goal::refresh_service_goal(bool is_self_add)
	{
		if (false == is_non_nullptr())
		{
			return;
		}

		e_service_goal_type service_goal_type = get_service_goal_condition_type();
		if (false == is_unfinished())
		{
			return;
		}
		player& temp_player = get_player();
		if (false == temp_player.is_valid())
		{
			return;
		}
		if (3 != m_template_ptr->ConditionValue.size())
		{
			return;
		}
		const int32 value1 = m_template_ptr->ConditionValue[0];
		const int32 value2 = m_template_ptr->ConditionValue[1];
		const int32 value3 = m_template_ptr->ConditionValue[2];

		int32 target_value = value1;
		int32 current_value = 0;
		switch (service_goal_type)
		{
		case faith::e_service_goal_type_role_level:
		{
			current_value = temp_player.get_unit_info(e_role_info_exp_level);
			break;
		}

		case faith::e_service_goal_type_role_gs:
		{
			current_value = temp_player.get_role_gs();
			break;
		}

		case faith::e_service_goal_type_big_reward:
		{
			const int32 active_data = value2;
			const int32 active_type = value3;
			current_value = temp_player.get_service_goal_mgr().get_service_goal_reward_days(active_data, (e_service_goal_active_type)active_type);
			break;
		}

		case faith::e_service_goal_type_strength_any:
		{
			const int32 strength_item_level = value2;
			//current_value = temp_player.get_item_set().get_equip_num_by_strength_limit(strength_item_level);
			break;
		}

		case faith::e_service_goal_type_addition_the_body:
		{
			const int32 addition_item_leve = value2;
			//current_value = temp_player.get_item_set().get_equip_num_by_addition_limie(addition_item_leve);
			break;
		}
			
		case faith::e_service_goal_type_mount_level:
		{
			const int32 order_num = value2;
			int32 current_mount_grade = 0;
			int32 current_mount_level = 0;
			//temp_player.get_item_set().get_mount_grade_and_level_by_order_num(order_num, current_mount_grade, current_mount_level);
			current_value = current_mount_grade * 11 + current_mount_level;
			break;
		}

		case faith::e_service_goal_type_mount_num:
		{
			target_value = value1;
			//current_value = temp_player.get_item_set().get_total_mount_activate_num();
			break;
		}

		case faith::e_service_goal_type_friend_num:
		{
			target_value = value1;
			if (is_self_add)
			{
				current_value = get_data_info(e_service_goal_inst_data_schedule) + 1;
			}
			break;
		}

		case faith::e_service_goal_type_friendliness:
		{
			target_value = value1;
			current_value = temp_player.get_highest_record(e_role_history_highest_record_higest_friendliness_value);
			break;
		}

		case faith::e_service_goal_type_clearance_fuben:
		{
			const int32 fuben_id = value2;
			if (true == temp_player.is_fuben_clearance(fuben_id))
			{
				current_value = 1;
			}
			else
			{
				current_value = 0;
			}
			break;
		}

		case faith::e_service_goal_type_body_has_color_equip:
		{
			const int32 target_item_grade = value2;
			const int32 target_item_color = value3;
			//current_value = temp_player.get_item_set().get_equip_num_by_limit(target_item_grade, target_item_color);
			break;
		}

		case faith::e_service_goal_type_legion_donate_num:
		{
			current_value = temp_player.get_unit_info(e_role_info_legion_donate_num);
			break;
		}

		case faith::e_service_goal_type_soldier_rank_level:
		{
			int32 current_level = temp_player.get_special_name_mgr().get_solider_rank_level();
			int32 current_star = temp_player.get_special_name_mgr().get_solider_rank_star();
			current_value = (current_level - 1) * 5 + current_star;
			break;
		}

		case faith::e_service_goal_type_arena_rank_level:
		{
			const int32 target_rank_level = value1;
			const int32 current_rank_level = temp_player.get_arena_mgr().current_rank();
			target_value = 1;
			if (current_rank_level < 0
				|| current_rank_level >= rank_max)
			{
				current_value = 0;
			}
			else if(current_rank_level <= target_rank_level)
			{
				current_value = 1;
			}
			break;
		}

		case faith::e_service_goal_type_actived_illustration_num:
		{
			target_value = value1;
			current_value = temp_player.get_pokedex_mgr().get_all_active_pokedex_num();
			break;
		}

		case faith::e_service_goal_type_actived_any_chapter_illustration:
		{
			current_value = temp_player.get_pokedex_mgr().get_all_active_pokedex_num();
			break;
		}

		case faith::e_service_goal_type_demons_tower_layer_num:
		{
			target_value = value1;
			current_value = temp_player.get_highest_record(e_role_history_highest_record_demon_tower_highest_level);
			break;
		}

		case faith::e_service_goal_type_achievement_title:
		{
			target_value = value1;
			current_value = temp_player.get_special_name_mgr().get_achievement_title_level();
			break;
		}

		case faith::e_service_goal_type_wing_level:
		{
			int32 current_wing_grade = 0;
			int32 current_wing_level = 0;
			//temp_player.get_item_set().get_cur_wing_grade_and_level(current_wing_grade, current_wing_level);
			current_value = (current_wing_grade - 1) * 11 + current_wing_level;
			
			break;
		}
		case faith::e_service_goal_type_element_heart_total_level:
		{
			target_value = value1;
			//current_value = temp_player.get_item_set().get_total_element_heart_level();
			break;
		}

		case faith::e_service_goal_type_equiped_element_heart_num:
		{
			target_value = value1;
			const int32 element_heart_quality = value2;
			//current_value = temp_player.get_element_heart_mgr().get_current_equipped_heart_num(element_heart_quality);
			break;
		}

		case faith::e_service_goal_type_equiped_spirit_total_level:
		{
			target_value = value1;
			//current_value = temp_player.get_spirit_mgr().get_spirit_total_level();
			break;
		}

		case faith::e_service_goal_type_equiped_spirit_num:
		{
			target_value = value1;
			const int32 m_condition_color = value2;
			//current_value = temp_player.get_spirit_mgr().num_of_spirit_equiped(m_condition_color);
			break;
		}

		case faith::e_service_goal_type_belief_rank:
		{
			target_value = value1;
			const int32 target_star = value2;
			BeliefTemplate* BeliefTemplatePtr = temp_player.get_belief_mgr().get_cur_max_rank();
			if (nullptr == BeliefTemplatePtr)
			{
				current_value = 0;
			}
			else
			{
				//小于目标星级则不算达到此阶
				if (BeliefTemplatePtr->CurStarNumber < target_star)
				{
					current_value = BeliefTemplatePtr->CurRankNumber - 1;
				}
				else
				{
					current_value = BeliefTemplatePtr->CurRankNumber;
				}
			}
			break;
		}

		case faith::e_service_goal_type_final:
		{
			current_value = temp_player.get_service_goal_mgr().get_specify_reward_num();
			break;
		}

		case e_service_goal_type_god_hood_grade:
		{
			current_value = temp_player.get_logic_data(e_role_logic_info_god_hood_grade);
			break;
		}
		case e_service_goal_type_no_limit_time_final:
		{
			current_value = temp_player.get_service_goal_mgr().get_specify_reward_num();
			break;
		}
		case e_service_goal_type_awaken_level:
		{
			const int32 strength_item_level = value2;
			//current_value = temp_player.get_item_set().get_equip_num_by_awaken_limit(strength_item_level);
			break;
		}
		case faith::e_service_goal_type_max:
			break;
		default:
			break;
		}
		if (cal_service_goal_state(target_value, current_value))
		{
			send_service_goal_one_to_self();
		}
	}

	void cservice_goal::make_service_goal_one_proto(faith::service_goal_proto_service_goal_one* proto_service_goal_one)
	{
		if (nullptr == proto_service_goal_one)
		{
			return;
		}
		proto_service_goal_one->clear_service_goal_info();
		for (int32 data_index = 0; data_index < e_service_goal_inst_data_max; data_index++)
		{
			proto_service_goal_one->add_service_goal_info(get_data_info((e_service_goal_inst_data)data_index));
		}
	}

	const int32 cservice_goal::get_service_goal_template_id(void)
	{
		const s_service_goal_info& temp_service_goal_info = get_service_goal_info();
		return get_data_info(e_service_goal_inst_data_id);
	}

	const e_service_goal_type cservice_goal::get_service_goal_condition_type()
	{
		if (nullptr == m_template_ptr)
		{
			return e_service_goal_type_max;
		}
		return (e_service_goal_type)m_template_ptr->ConditionType;
	}

	const int32 cservice_goal::get_service_goal_active_data()
	{
		if (nullptr == m_template_ptr)
		{
			return 0;
		}
		return m_template_ptr->TheData;
	}

	const e_service_goal_active_type cservice_goal::get_service_goal_active_type()
	{
		if (nullptr == m_template_ptr)
		{
			return e_service_goal_active_type_max;
		}
		return (e_service_goal_active_type)m_template_ptr->ActiveType;
	}

	const int32 cservice_goal::get_notice_id()
	{
		if (nullptr == m_template_ptr)
		{
			return 0;
		}
		return m_template_ptr->NoticeId;
	}

	const int32 cservice_goal::get_service_goal_target_value()
	{
		if (m_template_ptr->ConditionValue.size() > 0)
		{
			return m_template_ptr->ConditionValue[0];
		}
		return 0;
	}

	int32 cservice_goal::get_service_goal_schedule()
	{
		const s_service_goal_info& temp_service_goal_info = get_service_goal_info();
		return get_data_info(e_service_goal_inst_data_schedule);
	}

	void cservice_goal::set_service_goal_schedule(int32 service_goal_schedule)
	{
		set_data_info(e_service_goal_inst_data_schedule, service_goal_schedule);
	}

	bool cservice_goal::put_reward_into_bag()
	{
		const int32 item_array_size = init_unit::reward_regional_diff(m_template_ptr->RewardItemArray).size();
		if (0 != item_array_size % 2)
		{
			return false;
		}
		player& temp_player = get_player();
		if (false == temp_player.is_valid())
		{
			return false;
		}
		const e_class_type class_type = (e_class_type)temp_player.get_unit_info(e_role_info_class_type);
		std::vector<int32> item_reward_array = globle_data::get_instance().get_item_by_class_type(class_type, init_unit::reward_regional_diff(m_template_ptr->RewardItemArray), 1);

		//return	temp_player.get_item_set().put_in_bag(e_server_log_add_item_service_goal, 0, item_reward_array, e_bag_type_none, e_item_transform_type_three, e_item_mail_type_nomal, get_notice_id());
		return true;
	}

	void cservice_goal::send_notice_service_goal(const std::vector<int32>& item_array,const int32 item_locaked)
	{
		player& temp_player = get_player();
		if (nullptr == m_template_ptr || false == temp_player.is_valid())
		{
			return;
		}
		s_item_info item_data[chat_max_item];
		int32 item_num = 0;

		std::string item_names_str = "";
		int32 item_array_size = item_array.size();
		for (int32 array_index = 0; array_index < item_array_size; array_index += 2)
		{
			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_array[array_index]);
			if (nullptr == item_template_ptr || item_template_ptr->item_color < faith::e_item_color_orange)
			{
				continue;
			}


			//citem* item_ptr = temp_player.get_item_set().create_item_by_template(e_server_log_add_item_notice_info,0,item_array[array_index], item_array[array_index + 1], item_locaked);
			//if (item_ptr == nullptr)
			//{
			//	continue;
			//}

			//item_names_str += " ";
			//item_names_str += "%d";
			//if (item_num < chat_max_item)
			//{
			//	item_data[item_num] = item_ptr->get_item_inst();
			//	++item_num;
			//}

		}

		int32 notice_id = 93000074;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(get_player().get_name());
		vec_notice_str.push_back(item_names_str);
		std::string notice_str = init_unit::implode(vec_notice_str);

		get_player().get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_num);
	}

	const e_service_goal_state cservice_goal::get_service_goal_state(void)
	{
		return (e_service_goal_state)get_data_info(e_service_goal_inst_data_state);
	}

	void cservice_goal::set_service_goal_state(e_service_goal_state service_goal_state)
	{
		set_data_info(e_service_goal_inst_data_state, service_goal_state);
	}

	void cservice_goal::send_service_goal_one_to_self()
	{
		faith::service_goal_proto_service_goal_one temp_proto;
		make_service_goal_one_proto(&temp_proto);
		send_message_to_self(&temp_proto, e_msgindex_s2c_service_goal_one);
	}

	void cservice_goal::send_service_goal_operate_result_to_self(const int32 operate_type, const int32 operate_end_state)
	{
		faith::service_goal_proto_service_goal_operate_end msg;
		faith::service_goal_proto_service_goal_one* temp_proto = msg.mutable_service_goal_one();
		make_service_goal_one_proto(temp_proto);

		//msg.set_allocated_service_goal_one(*temp_proto);
		msg.set_operation_type(operate_type);
		msg.set_operation_end_state(operate_end_state);
		send_message_to_self(&msg, e_msgindex_s2c_service_goal_operate_end);
	}

	player& cservice_goal::get_player() const
	{
		return unit_man::get_player(m_player_index);
	}

	int32 cservice_goal::cal_target_value(int32 grade_num, int32 level_num)
	{
		return ((grade_num - 1) * 11 + level_num);
	}

	int32 cservice_goal::get_data_info(e_service_goal_inst_data data_index)
	{
		if (data_index >= e_service_goal_inst_data_max)
		{
			return 0;
		}
		return m_service_goal_info.data_ary[data_index];
	}

	void cservice_goal::set_data_info(e_service_goal_inst_data data_index, int32 data_value)
	{
		if (data_index >= e_service_goal_inst_data_max)
		{
			return;
		}
		m_service_goal_info.data_ary[data_index] = data_value;
	}

	void cservice_goal::send_message_to_self(google::protobuf::Message* net_pro, uint32 header)
	{
		player& temp_player = get_player();
		if (temp_player.is_valid())
		{
			temp_player.send_message_to_self(net_pro, header);
		}
	}
	bool cservice_goal::cal_service_goal_state(const int32 target_value, const int32 current_value)
	{
		if (current_value >= target_value)
		{
			set_service_goal_state(e_service_goal_state_finished);
			return true;
		}
		const int32 current_schedule = get_service_goal_schedule();
		if (current_value > current_schedule)
		{
			set_service_goal_schedule(current_value);
			return true;
		}
		return false;
	}

	const int32 cservice_goal::can_received_service_goal()
	{
		player& ref_player = get_player();

		//if (false == has_service_goal_activated())
		//{
		//	return e_active_has_not_start;
		//}
		if (!ref_player.is_valid() || (!ref_player.get_func_unlock_mgr().is_func_unlock("TargetHebdo") && 0 == ref_player.get_time_data(e_time_type_service_goal_close_time)))
		{
			return e_item_string_unkown;
		}
		//只有限时大奖需要判断时间
		//超时或者未到24小时内均不可领取
		if (m_template_ptr != nullptr && m_template_ptr->ConditionType == e_service_goal_type_final)
		{
			int32 time_now = time_helper::get_cur_time_new().second;
			if (time_now > get_data_info(e_service_goal_inst_data_over_time) || get_data_info(e_service_goal_inst_data_over_time) - time_now > day_time_second)
			{
				return e_item_string_unkown;
			}
			if (!ref_player.get_service_goal_mgr().check_in_time())
			{
				return e_item_string_unkown;
			}
		}
		if (e_service_goal_state_finished == get_service_goal_state())
		{
			return e_item_string_succeed;
		}
		return e_item_string_unkown;
	}

	const s_service_goal_info& cservice_goal::get_service_goal_info()
	{
		return m_service_goal_info;
	}

	void cservice_goal::refresh_over_time(const int32& time_now)
	{
		int32 active_data = 0;
		if(get_service_goal_active_type() == e_service_goal_active_type_final) 
		{
			active_data = MAX_REWARD_DAYS;
		}
		else
		{
			active_data = get_service_goal_active_data();
		}
		set_data_info(e_service_goal_inst_data_over_time, active_data * day_time_second + time_now);
	}
}

