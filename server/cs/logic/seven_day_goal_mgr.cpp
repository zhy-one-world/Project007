/********************************************************************
created: 2019年2月13日
file base: seven_day_goal_mgr
file ext: cpp
author: wangdi
purpose: 七日目标实现文件
*********************************************************************/

#include "arena_def.hpp"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "internet/net.pb.h"
#include "server_log.hpp"
#include "seven_day_goal.h"
#include "seven_day_goal_mgr.h"
#include "template/StringConst_S.h"
#include "time_def.hpp"
#include "internet/service_goal.pb.h"
#include "internet/cs2dp.pb.h"
#include "internal/seven_day_goal_msg.hpp"
#include "utility/parse_msg.h"
#include "utility/init_unit.h"

namespace hld
{
	
	cseven_day_goal_mgr::cseven_day_goal_mgr()
	{
		m_player_index = 0;
		clear_data();
	}

	cseven_day_goal_mgr::~cseven_day_goal_mgr()
	{

	}

	void cseven_day_goal_mgr::clear_data()
	{
		for (int32 seven_day_goal_index = 0; seven_day_goal_index < max_seven_day_goal_num; seven_day_goal_index++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(seven_day_goal_index);
			temp_seven_day_goal_obj.clear_data();
		}
		memset(m_seven_day_goal_reward_days, 0, sizeof(m_seven_day_goal_reward_days));
		m_seven_day_goal_obj_num = 0;
		m_empty_goal_obj.clear_data();
	}

	void cseven_day_goal_mgr::set_player_index(int32 player_index)
	{
		m_player_index = player_index;
	}

	void cseven_day_goal_mgr::send_all_seven_day_goal()
	{
		player& temp_player = get_player();
		if (!temp_player.is_valid())
		{
			return;
		}
		service_goal_proto_seven_day_goal_all seven_day_goal_msg_all;
		for (int32 seven_day_goal_index = 0; seven_day_goal_index < m_seven_day_goal_obj_num; seven_day_goal_index++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(seven_day_goal_index);
			service_goal_proto_seven_day_goal_one* seven_day_goal_one_ptr = seven_day_goal_msg_all.add_seven_day_goal_array();
			make_seven_day_goal_one_proto(seven_day_goal_one_ptr, temp_seven_day_goal_obj);

			if (seven_day_goal_msg_all.ByteSize() > packege_length_max)
			{
				send_message_to_self(&seven_day_goal_msg_all, e_msgindex_s2c_seven_day_goal_all);
				seven_day_goal_msg_all.clear_seven_day_goal_array();
			}
		}
		if (seven_day_goal_msg_all.seven_day_goal_array_size() > 0)
		{
			send_message_to_self(&seven_day_goal_msg_all, e_msgindex_s2c_seven_day_goal_all);
		}
	}

	void cseven_day_goal_mgr::received_seven_day_goal(int32 seven_day_goal_id)
	{
		cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_id(seven_day_goal_id);
		if (!temp_seven_day_goal_obj.is_valid())
		{
			return;
		}

		int32 operate_result = temp_seven_day_goal_obj.can_received_seven_day_goal();
		if (e_item_string_succeed == operate_result)
		{
			if (put_reward_into_bag(temp_seven_day_goal_obj.get_data_info(e_seven_day_goal_info_id)))
			{
				temp_seven_day_goal_obj.set_data_info(e_seven_day_goal_info_state, e_service_goal_state_has_been_received);
				player& temp_player = get_player();
				if (temp_player.is_valid())
				{
					temp_player.get_mission_mgr().target_check(e_mission_end_type_service_goal);
				}			
			}
			else
			{
				operate_result = 0;
			}
		}
		send_seven_day_goal_operate_result_to_self(e_service_goal_operate_type_receive, operate_result, temp_seven_day_goal_obj);
	}

	bool cseven_day_goal_mgr::put_reward_into_bag(int32 seven_day_goal_info_id)
	{
		ServiceGoalTemplate* temp_template_ptr = GET_TEMPLATE(ServiceGoalTemplate, seven_day_goal_info_id);
		std::vector<int32> RewardItem = init_unit::reward_regional_diff(temp_template_ptr->RewardItemArray);
		if (nullptr == temp_template_ptr || 0 != RewardItem.size() % 2)
		{
			return false;
		}
		player& temp_player = get_player();
		if (false == temp_player.is_valid())
		{
			return false;
		}
		const e_class_type class_type = (e_class_type)temp_player.get_unit_info(e_role_info_class_type);
		std::vector<int32> item_reward_array = globle_data::get_instance().get_item_by_class_type(class_type, RewardItem, 1);

		return	temp_player.get_item_set().put_in_bag(e_server_log_add_item_seven_day, seven_day_goal_info_id, item_reward_array, e_bag_type_none, e_item_transform_type_three, e_item_mail_type_nomal, temp_template_ptr->NoticeId);
	}

	void cseven_day_goal_mgr::refresh_seven_day_goal_received_reward_days()
	{
		clear_seven_day_goal_reward_days();
		for (int32 array_index = 0; array_index < m_seven_day_goal_obj_num; array_index++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(array_index);
			if (e_service_goal_state_has_been_received == temp_seven_day_goal_obj.get_data_info(e_seven_day_goal_info_state))
			{
				add_seven_day_goal_reward_days(temp_seven_day_goal_obj.get_seven_day_goal_active_data(), temp_seven_day_goal_obj.get_seven_day_goal_active_type());
			}
		}
	}

	void cseven_day_goal_mgr::add_seven_day_goal_reward_days(const int32 reward_day, const e_service_goal_active_type active_type)
	{
   		if (0 >= reward_day 
			|| max_reward_days < reward_day
			|| e_service_goal_active_type_max <= active_type)
		{
			return;
		}
		m_seven_day_goal_reward_days[reward_day][active_type]++;
	}

	void cseven_day_goal_mgr::clear_seven_day_goal_reward_days()
	{
		memset(m_seven_day_goal_reward_days, 0, sizeof(m_seven_day_goal_reward_days));
	}

	void cseven_day_goal_mgr::load_seven_day_goal_by_db(const s_seven_day_goal_info* seven_day_goal_data, const int32 seven_day_goal_num)
	{
		if (nullptr == seven_day_goal_data)
		{
			return;
		}
		template_manager::template_type* ladder_tmpl_table = template_manager::get_instance().get_templates(e_ServiceGoalTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return;
		}

		template_manager::template_type::iterator ite;
		time_info cur_time_info = time_helper::get_cur_time_new();
		//今天零点时间戳
		int32 today_zero_oclock_time_stamp = cur_time_info.second - cur_time_info.second_in_day;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			ServiceGoalTemplate* temp_seven_day_goal_template_ptr = (ServiceGoalTemplate*)(ite->second);
			if (nullptr == temp_seven_day_goal_template_ptr
				|| temp_seven_day_goal_template_ptr->ActiveType == e_service_goal_active_type_basis
				|| temp_seven_day_goal_template_ptr->ActiveType == e_service_goal_active_type_advance
				|| temp_seven_day_goal_template_ptr->ConditionType == e_service_goal_type_final)
			{
				continue;
			}

			s_seven_day_goal_info new_seven_day_goal_info;
			for (int32 i = 0; i < seven_day_goal_num; i++)
			{
				if (seven_day_goal_data[i].data_ary[e_seven_day_goal_info_id] == ite->first)
				{
					new_seven_day_goal_info = seven_day_goal_data[i];
					break;
				}
			}
			//数据库没有就初始化数据
			if (new_seven_day_goal_info.data_ary[e_seven_day_goal_info_id] <= 0)
			{
				new_seven_day_goal_info.data_ary[e_seven_day_goal_info_id] = ite->first;
				int32 open_date = temp_seven_day_goal_template_ptr->TheData;
				if (temp_seven_day_goal_template_ptr->ActiveType == e_service_goal_active_type_final)
				{
					open_date = max_reward_days;
				}
				new_seven_day_goal_info.data_ary[e_seven_day_goal_info_open_time] = today_zero_oclock_time_stamp + day_time_second * (open_date - 1);
			}
			create_seven_day_goal_obj(new_seven_day_goal_info);
		}
		// 接下来发送给客户端
		send_all_seven_day_goal();
	}

	bool cseven_day_goal_mgr::load_seven_day_goal_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_seven_day_goal_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_seven_day_goal_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_seven_day_goal_info *p_row = (s_seven_day_goal_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::cs2dp_proto::role_seven_day_goal_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_seven_day_goal_by_db((const s_seven_day_goal_info *)p_data, msg.row_count());
		return true;


	}

	void cseven_day_goal_mgr::save_seven_day_goal_to_db(const int32 save_type)
	{
		player& player_ref = get_player();
		if (!player_ref.is_valid())
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_seven_day_goal);
		if (is_use == false)
		{
			cs2dp_save_char_seven_day_goal req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_player_index;
			int32 data_num = 0;
			for (int32 i = 0; i < m_seven_day_goal_obj_num; i++)
			{
				cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(i);
				req.data_list[data_num] = temp_seven_day_goal_obj.get_seven_day_goal_info();
				++data_num;
			}
			req.data_num = data_num;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			hld::cs2dp_proto::save_role_seven_day_goal msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_index);
			msg.set_save_type_ex(save_type);

			hld::cs2dp_proto::role_seven_day_goal_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			int32 row_num = 0;
			for (int32 i = 0; i < m_seven_day_goal_obj_num; i++)
			{
				cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(i);
				const s_seven_day_goal_info& tmp = temp_seven_day_goal_obj.get_seven_day_goal_info();
				hld::cs2dp_proto::role_seven_day_goal_row *db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				for (int32 i = 0; i < e_seven_day_goal_info_max; i++)
				{
					db_row->add_data_ary(tmp.data_ary[i]);
				}
				++row_num;
			}
			db_data->set_row_count(row_num);
			if (row_num == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_seven_day_goal);
		}
	}
	void cseven_day_goal_mgr::refresh_seven_day_goal(const e_service_goal_type seven_day_goal_type)
	{
		for (int32 array_index = 0; array_index < m_seven_day_goal_obj_num; array_index++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(array_index);
			if (seven_day_goal_type == temp_seven_day_goal_obj.get_seven_day_goal_condition_type())
			{
				refresh_seven_day_goal(&temp_seven_day_goal_obj, true);
			}
		}
	}

	void cseven_day_goal_mgr::refresh_seven_day_goal_all()
	{
		if (m_seven_day_goal_obj_num < 0
			|| m_seven_day_goal_obj_num >= max_seven_day_goal_num)
		{
			return;
		}
		for (int32 array_index = 0; array_index < m_seven_day_goal_obj_num; array_index++)
		{
			refresh_seven_day_goal(&m_seven_day_goal_obj_array[array_index]);
		}
	}

	int32 cseven_day_goal_mgr::get_seven_day_goal_reward_days(int32 reward_data, e_service_goal_active_type active_type)
	{
		if (0 >= reward_data
			|| max_reward_days < reward_data
			|| e_service_goal_active_type_max <= active_type)
		{
			return 0;
		}
		return 	m_seven_day_goal_reward_days[reward_data][active_type];
	}

	int32 cseven_day_goal_mgr::get_specify_reward_num()
	{
		int32 specify_reward_num = 0;
		for (int32 i = 0; i < m_seven_day_goal_obj_num; i++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(i);
			if (e_service_goal_type_big_reward == temp_seven_day_goal_obj.get_seven_day_goal_condition_type()
				&& e_service_goal_state_has_been_received == temp_seven_day_goal_obj.get_data_info(e_seven_day_goal_info_state))
			{
				specify_reward_num++;
			}
		}
		return specify_reward_num;
	}

	player& cseven_day_goal_mgr::get_player()  const
	{
		return unit_man::get_player(m_player_index);
	}

	int32 cseven_day_goal_mgr::get_has_been_received()
	{
		int32  has_num = 0;
		for (int32 array_index = 0; array_index < m_seven_day_goal_obj_num; array_index++)
		{
			cseven_day_goal& temp_service_goal_obj = get_seven_day_goal_obj_by_index(array_index);
			if (temp_service_goal_obj.get_data_info(e_seven_day_goal_info_state) == e_service_goal_state_has_been_received && temp_service_goal_obj.get_seven_day_goal_condition_type() != e_service_goal_type_big_reward)
			{
				has_num++;
			}
		}
		return has_num;
	}

	cseven_day_goal& cseven_day_goal_mgr::get_seven_day_goal_obj_by_index(int32 obj_index)
	{
		if (0 > obj_index || max_seven_day_goal_num <= obj_index)
		{
			return m_empty_goal_obj;
		}
		return m_seven_day_goal_obj_array[obj_index];
	}
	cseven_day_goal* cseven_day_goal_mgr::get_seven_day_goal_obj_by_index_lua(int32 obj_index)
	{
		if (0 > obj_index || max_seven_day_goal_num <= obj_index)
		{
			return &m_empty_goal_obj;
		}
		return &m_seven_day_goal_obj_array[obj_index];
	}

	cseven_day_goal& cseven_day_goal_mgr::get_seven_day_goal_obj_by_id(int32 obj_id)
	{
		for (int32 i = 0; i < m_seven_day_goal_obj_num; i++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(i);
			if (obj_id == temp_seven_day_goal_obj.get_data_info(e_seven_day_goal_info_id))
			{
				return temp_seven_day_goal_obj;
			}
		}
		return m_empty_goal_obj;
	}

	std::vector<cseven_day_goal*> cseven_day_goal_mgr::get_seven_day_goal_obj_by_type(const e_service_goal_type seven_day_goal_type)
	{
		std::vector<cseven_day_goal*> temp_vector;
		for (int32 array_index = 0; array_index < m_seven_day_goal_obj_num; array_index++)
		{
			cseven_day_goal& temp_seven_day_goal_obj = get_seven_day_goal_obj_by_index(array_index);
			if (seven_day_goal_type == temp_seven_day_goal_obj.get_seven_day_goal_condition_type())
			{
				temp_vector.push_back(&temp_seven_day_goal_obj);
			}
		}
		return temp_vector;
	}

	void cseven_day_goal_mgr::create_seven_day_goal_obj(s_seven_day_goal_info seven_day_goal_info)
	{
		cseven_day_goal& new_seven_day_goal_obj = get_seven_day_goal_obj_by_index(m_seven_day_goal_obj_num);
		new_seven_day_goal_obj.init_seven_day_goal(seven_day_goal_info);
		m_seven_day_goal_obj_num++;
	}

	void cseven_day_goal_mgr::send_message_to_self(google::protobuf::Message* net_pro, uint32 header)
	{
		player& temp_player = get_player();
		if (temp_player.is_valid())
		{
			temp_player.send_message_to_self(net_pro, header);
		}
	}
	
	//第二个参数控制是否可以自增 主要针对需要靠自身进度更新刷新的类型 比如加好友个数 防止刷新全部的时候错误自增
	void cseven_day_goal_mgr::refresh_seven_day_goal(cseven_day_goal* seven_day_goal_obj_ptr, bool is_self_add)
	{
		player& temp_player = get_player();
		if (!temp_player.is_valid() || nullptr == seven_day_goal_obj_ptr || !seven_day_goal_obj_ptr->is_valid())
		{
			return;
		}
		if (e_service_goal_state_unfinished != seven_day_goal_obj_ptr->get_data_info(e_seven_day_goal_info_state))
		{
			return;
		}
		ServiceGoalTemplate* temp_template_ptr = seven_day_goal_obj_ptr->get_service_goal_template_ptr();
		if (3 != temp_template_ptr->ConditionValue.size())
		{
			return;
		}
		int32 value1 = temp_template_ptr->ConditionValue[0];
		int32 value2 = temp_template_ptr->ConditionValue[1];
		int32 value3 = temp_template_ptr->ConditionValue[2];
		e_service_goal_type seven_day_goal_type = seven_day_goal_obj_ptr->get_seven_day_goal_condition_type();

		int32 target_value = value1;
		int32 current_value = 0;
		switch (seven_day_goal_type)
		{
		case e_service_goal_type_role_level:
		{
			current_value = temp_player.get_unit_info(e_role_info_exp_level);
			break;
		}

		case e_service_goal_type_role_gs:
		{
			current_value = temp_player.get_role_gs();
			break;
		}

		case e_service_goal_type_big_reward:
		{
			const int32 active_data = value2;
			const int32 active_type = value3;
			current_value = get_seven_day_goal_reward_days(active_data, (e_service_goal_active_type)active_type);
			break;
		}

		case e_service_goal_type_strength_any:
		{
			const int32 strength_item_level = value2;
			current_value = temp_player.get_item_set().get_equip_num_by_strength_limit(strength_item_level);
			break;
		}

		case e_service_goal_type_addition_the_body:
		{
			const int32 addition_item_leve = value2;
			current_value = temp_player.get_item_set().get_equip_num_by_addition_limie(addition_item_leve);
			break;
		}

		case e_service_goal_type_mount_level:
		{
			const int32 order_num = value2;
			int32 current_mount_grade = 0;
			int32 current_mount_level = 0;
			temp_player.get_item_set().get_mount_grade_and_level_by_order_num(order_num, current_mount_grade, current_mount_level);
			current_value = current_mount_grade * 11 + current_mount_level;
			break;
		}

		case e_service_goal_type_mount_num:
		{
			target_value = value1;
			current_value = temp_player.get_item_set().get_total_mount_activate_num();
			break;
		}

		case e_service_goal_type_friend_num:
		{
			target_value = value1;
			if (is_self_add)
			{
				current_value = seven_day_goal_obj_ptr->get_data_info(e_seven_day_goal_info_schedule) + 1;
			}
			break;
		}

		case e_service_goal_type_friendliness:
		{
			target_value = value1;
			current_value = temp_player.get_highest_record(e_role_history_highest_record_higest_friendliness_value);
			break;
		}

		case e_service_goal_type_clearance_fuben:
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

		case e_service_goal_type_body_has_color_equip:
		{
			const int32 target_item_grade = value2;
			const int32 target_item_color = value3;
			current_value = temp_player.get_item_set().get_equip_num_by_limit(target_item_grade, target_item_color);
			break;
		}

		case e_service_goal_type_legion_donate_num:
		{
			current_value = temp_player.get_highest_record(e_role_history_highest_record_battle_achievement);
			break;
		}

		case e_service_goal_type_soldier_rank_level:
		{
			int32 current_level = temp_player.get_special_name_mgr().get_solider_rank_level();
			int32 current_star = temp_player.get_special_name_mgr().get_solider_rank_star();
			current_value = (current_level - 1) * 5 + current_star;
			break;
		}

		case e_service_goal_type_arena_rank_level:
		{
			const int32 target_rank_level = value1;
			const int32 current_rank_level = temp_player.get_arena_mgr().current_rank();
			target_value = 1;
			if (current_rank_level < 0
				|| current_rank_level >= rank_max)
			{
				current_value = 0;
			}
			else if (current_rank_level <= target_rank_level)
			{
				current_value = 1;
			}
			break;
		}

		case e_service_goal_type_actived_illustration_num:
		{
			target_value = value1;
			current_value = temp_player.get_pokedex_mgr().get_active_pokedex_num();
			break;
		}

		case e_service_goal_type_actived_any_chapter_illustration:
		{
			current_value = temp_player.get_pokedex_mgr().get_all_active_pokedex_num();
			break;
		}

		case e_service_goal_type_demons_tower_layer_num:
		{
			target_value = value1;
			current_value = temp_player.get_highest_record(e_role_history_highest_record_demon_tower_highest_level);
			break;
		}

		case e_service_goal_type_achievement_title:
		{
			target_value = value1;
			current_value = temp_player.get_special_name_mgr().get_achievement_title_level();
			break;
		}

		case e_service_goal_type_wing_level:
		{
			int32 current_wing_grade = 0;
			int32 current_wing_level = 0;
			temp_player.get_item_set().get_cur_wing_grade_and_level(current_wing_grade, current_wing_level);
			current_value = (current_wing_grade - 1) * 11 + current_wing_level;

			break;
		}
		case e_service_goal_type_element_heart_total_level:
		{
			target_value = value1;
			current_value = temp_player.get_item_set().get_total_element_heart_level();
			break;
		}

		case e_service_goal_type_equiped_element_heart_num:
		{
			target_value = value1;
			const int32 element_heart_quality = value2;
			//current_value = temp_player.get_element_heart_mgr().get_current_equipped_heart_num(element_heart_quality);
			break;
		}

		case e_service_goal_type_equiped_spirit_total_level:
		{
			target_value = value1;
			//current_value = temp_player.get_spirit_mgr().get_spirit_total_level();
			break;
		}

		case e_service_goal_type_equiped_spirit_num:
		{
			target_value = value1;
			const int32 m_condition_color = value2;
			//current_value = temp_player.get_spirit_mgr().num_of_spirit_equiped(m_condition_color);
			break;
		}

		case e_service_goal_type_belief_rank:
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

		case e_service_goal_type_god_hood_grade:
		{
			current_value = temp_player.get_logic_data(e_role_logic_info_god_hood_grade);
			break;
		}
		
		case e_service_goal_type_seven_day_final:
		{
			current_value = get_specify_reward_num();
			break;
		}
		case e_service_goal_type_awaken_level:
		{
			const int32 strength_item_level = value2;
			current_value = temp_player.get_item_set().get_equip_num_by_awaken_limit(strength_item_level);
			break;
		}

		case e_service_goal_type_max:
			break;
		default:
			break;
		}
		if (seven_day_goal_obj_ptr->cal_seven_day_goal_state(target_value, current_value))
		{
			send_seven_day_goal_one_to_self(*seven_day_goal_obj_ptr);
		}
	}

	void cseven_day_goal_mgr::make_seven_day_goal_one_proto(service_goal_proto_seven_day_goal_one* msg_one, cseven_day_goal& seven_day_goal_obj_ref)
	{
		if (nullptr == msg_one || !seven_day_goal_obj_ref.is_valid())
		{
			return;
		}
		msg_one->clear_seven_day_goal_info();
		for (int32 i = 0; i < e_seven_day_goal_info_max; i++)
		{
			msg_one->add_seven_day_goal_info(seven_day_goal_obj_ref.get_data_info((e_seven_day_goal_info)i));
		}
	}

	void cseven_day_goal_mgr::send_seven_day_goal_one_to_self(cseven_day_goal& seven_day_goal_obj_ref)
	{
		service_goal_proto_seven_day_goal_one temp_proto;
		make_seven_day_goal_one_proto(&temp_proto, seven_day_goal_obj_ref);
		send_message_to_self(&temp_proto, e_msgindex_s2c_seven_day_goal_one);
	}

	void cseven_day_goal_mgr::send_seven_day_goal_operate_result_to_self(int32 operate_type, int32 operate_end_state, cseven_day_goal& seven_day_goal_obj_ref)
	{
		service_goal_proto_seven_day_goal_operate_end msg;
		service_goal_proto_seven_day_goal_one* temp_proto = msg.mutable_seven_day_goal_data();
		make_seven_day_goal_one_proto(temp_proto, seven_day_goal_obj_ref);

		msg.set_operation_type(operate_type);
		msg.set_operation_end_state(operate_end_state);
		send_message_to_self(&msg, e_msgindex_s2c_seven_day_goal_operate_end);
	}

}