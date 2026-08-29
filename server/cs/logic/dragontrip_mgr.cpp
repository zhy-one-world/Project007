#include "internet/dragontrip.pb.h"
#include "dragontrip_mgr.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"

#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "utility/random.h"
#include "internet/net.pb.h"
#include "utility/parse_msg.h"
#include "internet/cs2dp.pb.h"
#include "internal/char_msg.hpp"

namespace faith
{
	dragontrip_mgr::dragontrip_mgr()
	{
		m_array_index = 0;
		clear_data();
	}
	dragontrip_mgr::~dragontrip_mgr()
	{

	}

	void dragontrip_mgr::clear_data()
	{
		
	}
	bool dragontrip_mgr::is_use_lua()
	{
		//script_mgr::get_instance().call_func("reload", 0);
		bool bRet = false;
		return bRet;
	}
	void dragontrip_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}
	void dragontrip_mgr::save_dragontrip_to_db(int32 save_type)
	{
		if (is_use_lua())
		{
			return;
		}

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
	
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_role_dragontrip_to_db);
		if (is_use == false)
		{

			cs2dp_save_role_dragontrip_to_db req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;
			req.dragontrip_info = m_data;

			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_role_dragontrip_to_db));

		}
		else
		{

			faith::cs2dp_proto::save_role_dragontrip msg;

			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type);

			faith::cs2dp_proto::role_dragontrip_db* db_row = msg.mutable_db_data();
			for (int32 i = 0; i < e_dragontrip_max; i++)
			{
				db_row->add_data_ary(m_data.data_ary[i]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_role_dragontrip_to_db);
		}
	}

	void dragontrip_mgr::load_dragontrip_by_db(const s_dragontrip_info &data_info)
	{
		if (is_use_lua())
		{
			return;
		}
		m_data = data_info;
		sync_all_message_to_client();	 
	}

	bool dragontrip_mgr::load_dragontrip_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (is_use_lua())
		{
			return false;
		}
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}

		s_dragontrip_info row_info;

		faith::cs2dp_proto::role_dragontrip_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			row_info.data_ary[i] = msg.data_ary(i);
		}

		load_dragontrip_by_db(row_info);

		return true;

	}

	void dragontrip_mgr::sync_all_message_to_client()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		faith::dragontrip_proto::dragontrip_all msg_all;

		for (int32 i = e_dragontrip_free_time; i < e_dragontrip_max; i++)
		{
			msg_all.add_data_ary(get_data(i));
		}

		for (int32 i = e_dragontrip_reward_r1;i <= e_dragontrip_reward_rall; i++)
		{
			msg_all.add_reward_ary(get_reward_flag(i));
		}

		msg_all.add_params(time_helper::get_time());

		temp_player.send_message_to_self(&msg_all, e_mgsindex_s2c_dragontrip_all);

	}

	void dragontrip_mgr::on_operate(int32 oper_type, int32 param)
	{
		if (is_use_lua())
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (!temp_player.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_dragontrip))
		{
			return;
		}
		switch (oper_type)
		{
		case e_dragontrip_oper_money:
			on_money(oper_type, param);
		    break;
		case e_dragontrip_oper_item:
			on_item(oper_type, param);
			break;
		case e_dragontrip_oper_roll:
			if (get_data(e_dragontrip_active_state) <= 2)
			{
				on_roll(oper_type, param);
			}
			else
			{
				on_five_roll(oper_type, param);
			}
			break;
		case e_dragontrip_oper_reward_round:
			on_reward_round(oper_type, param);
			break;
		case e_dragontrip_oper_reward_all:
			on_reward_all(oper_type, param);
			break;
		case e_dragontrip_oper_quick:
			on_quick(oper_type, param);
			break;
		default:
			break;
		}	
	}

	void dragontrip_mgr::on_money(int32 oper_type, int32 param)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 cur_times = get_data(e_dragontrip_day_times);
		if (!player_ref.get_is_robot())
		{
			if (cur_times >= get_config_param(e_dragontrip_param_day_limit))
			{
				oper_msg(oper_type, dragontrip_error_times);
				return;

			}
		}
		if (get_data(e_dragontrip_active_state) != 0)
		{
			oper_msg(oper_type, dragontrip_error_state);
			return;
		}
		if (is_free())
		{
			set_data(e_dragontrip_free_time, time_helper::get_time());
		}
		else
		{
			if (!player_ref.can_cut_money((e_money_type)get_config_param(e_dragontrip_param_money_type), get_config_param(e_dragontrip_param_money_num)))
			{
				oper_msg(oper_type, dragontrip_error_not_money);
				return;
			}

			player_ref.cut_money((e_money_type)get_config_param(e_dragontrip_param_money_type), get_config_param(e_dragontrip_param_money_num), e_server_log_cut_money_dragontrip);
		}
		set_data(e_dragontrip_active_state, 1);
		int32 day_times = get_data(e_dragontrip_day_times);
		set_data(e_dragontrip_day_times, cur_times + 1);

		sync_all_message_to_client();


		oper_msg(oper_type, dragontrip_error_sucess);

	}
	void dragontrip_mgr::on_item(int32 oper_type, int32 param)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (get_data(e_dragontrip_active_state) != 0)
		{
			oper_msg(oper_type, dragontrip_error_state);
			return;
		}

		if (param == 0)
		{
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num)) == false)
			{
				oper_msg(oper_type, dragontrip_error_not_item);
				return;

			}
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num));
			set_data(e_dragontrip_active_state, 2);
		}
		else
		{
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num) * dragontrip_five_num) == false)
			{
				oper_msg(oper_type, dragontrip_error_not_item2);
				return;

			}
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num) * dragontrip_five_num);
			set_data(e_dragontrip_active_state, 3);
		}
		
		sync_all_message_to_client();
		oper_msg(oper_type, dragontrip_error_sucess);

	}

	void dragontrip_mgr::on_roll(int32 oper_type, int32 param)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (get_data(e_dragontrip_active_state) == 0)
		{
			return;
		}
		int32 max_point = get_config_param(e_dragontrip_param_point_max);

		int32 step = random_gen::get_random(1, 6);

		int32 cur_pos = get_data(e_dragontrip_pos);

		int32 end_pos = (cur_pos + step) % max_point;

		int32 is_case = 0;

		int32  item_id = 0;
		int32  item_num = 0;

		if (end_pos == e_dragontrip_point_transfer1)
		{
			end_pos = e_dragontrip_point_transfer2;

			is_case = 1;

		}
		else if(end_pos == e_dragontrip_point_transfer2)
		{
			end_pos = e_dragontrip_point_transfer1;
		}
		else
		{

			if (end_pos != e_dragontrip_point_start)
			{
				DragonTripTemplate *config_ptr = get_item_by_type_pos(e_dragontrip_type_point, end_pos);
				if (config_ptr == nullptr)
				{
					return;
				}
				ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, config_ptr->ItemId);
				if (nullptr == ItemTemplatePtr)
				{
					return;
				}
				int32  level = ItemTemplatePtr->item_color;


				get_activity_item(level, item_id, item_num);

				if (item_id == 0)
				{
					return;
				}
				ItemTemplate* rwdItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_id);
				if (nullptr == rwdItemTemplatePtr)
				{
					return;
				}

				if (get_data(e_dragontrip_active_state) <= 2)
				{
					item_num *= get_data(e_dragontrip_active_state);
				}
				else
				{
					item_num *= 2;
				}
				item_system::put_item_into_bag(&temp_player, { {item_id, item_num, 1} });
			}
			is_case = 3;
		}
		if ((is_case == 3 || is_case == 1) && (end_pos < cur_pos))
		{
			int32 round = get_data(e_dragontrip_round);
			set_data(e_dragontrip_round, round + 1);

		}

		set_data(e_dragontrip_active_state, 0);
		set_data(e_dragontrip_pos, end_pos);


		sync_all_message_to_client();
		oper_msg(oper_type, dragontrip_error_sucess, step, item_id, item_num);

	}
	void dragontrip_mgr::on_five_roll(int32 oper_type, int32 param)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (get_data(e_dragontrip_active_state) <= 2)
		{
			return;
		}
		std::vector<int32> item_list;
		int32 end_pos = 0;
		int32 all_step = 0;
		for (int32 i = 0; i < dragontrip_five_num; ++i)
		{
			int32 max_point = get_config_param(e_dragontrip_param_point_max);

			int32 step = random_gen::get_random(1, 6);

			int32 cur_pos = get_data(e_dragontrip_pos);

			end_pos = (cur_pos + step) % max_point;

			int32 is_case = 0;

			int32  item_id = 0;
			int32  item_num = 0;

			if (end_pos == e_dragontrip_point_transfer1)
			{
				end_pos = e_dragontrip_point_transfer2;

				is_case = 1;

			}
			else if (end_pos == e_dragontrip_point_transfer2)
			{
				end_pos = e_dragontrip_point_transfer1;
			}
			else
			{

				if (end_pos != e_dragontrip_point_start)
				{
					DragonTripTemplate *config_ptr = get_item_by_type_pos(e_dragontrip_type_point, end_pos);
					if (config_ptr == nullptr)
					{
						return;
					}
					ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, config_ptr->ItemId);
					if (nullptr == ItemTemplatePtr)
					{
						return;
					}
					int32  level = ItemTemplatePtr->item_color;


					get_activity_item(level, item_id, item_num);

					if (item_id == 0)
					{
						return;
					}

					ItemTemplate* rwdItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_id);
					if (nullptr == rwdItemTemplatePtr)
					{
						return;
					}

					if (get_data(e_dragontrip_active_state) <= 2)
					{
						item_num *= get_data(e_dragontrip_active_state);
					}
					else
					{
						item_num *= 2;
					}
					item_system::put_item_into_bag(&temp_player, { {item_id, item_num, 1} });
					
					item_list.push_back(item_id);
					item_list.push_back(item_num);
					item_list.push_back(end_pos);
				}
				is_case = 3;
			}

			if ((is_case == 3 || is_case == 1) && (end_pos < cur_pos))
			{
				int32 round = get_data(e_dragontrip_round);
				set_data(e_dragontrip_round, round + 1);

			}
			all_step += step;
			set_data(e_dragontrip_pos, end_pos);
		}

		set_data(e_dragontrip_active_state, 0);
		set_data(e_dragontrip_pos, end_pos);

		sync_all_message_to_client();
		oper_msg_item_list(oper_type, dragontrip_error_sucess, all_step, item_list);
	}
	void dragontrip_mgr::on_reward_round(int32 oper_type, int32 req_round)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (req_round < e_dragontrip_reward_r1 || req_round > e_dragontrip_reward_r5)
		{
			return;
		}
		int32 round_num = get_data(e_dragontrip_round);
		if (round_num < req_round)
		{
			oper_msg(oper_type, dragontrip_error_reward_round);
			return;
		}
		if (get_reward_flag(req_round) == 1)
		{
			oper_msg(oper_type, dragontrip_error_have_get);
			return;
		}

		get_reward(e_dragontrip_type_reward_round, req_round);

		set_reward_flag(req_round);


		sync_all_message_to_client();
		oper_msg(oper_type, dragontrip_error_sucess);


	}

	void dragontrip_mgr::on_reward_all(int32 oper_type, int32 param)
	{

		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		int32 round_num = get_data(e_dragontrip_round);
		if (round_num < get_config_param(e_dragontrip_param_reward_max))
		{
			oper_msg(oper_type, dragontrip_error_reward_round);
			return;
		}
		if (get_reward_flag(e_dragontrip_reward_rall) == 1)
		{
			oper_msg(oper_type, dragontrip_error_have_get);
			return;
		}

		get_reward(e_dragontrip_type_reward_all, 0);

		set_reward_flag(e_dragontrip_reward_rall);


		sync_all_message_to_client();
		oper_msg(oper_type, dragontrip_error_sucess);


	}

	void dragontrip_mgr::on_quick(int32 oper_type, int32 param)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (get_data(e_dragontrip_active_state) != 0)
		{
			oper_msg(oper_type, dragontrip_error_state);
			return;
		}

		if (param == 0)
		{
			int32 cur_times = get_data(e_dragontrip_day_times);
			if (!player_ref.get_is_robot() && cur_times >= get_config_param(e_dragontrip_param_day_limit))
			{
				oper_msg(oper_type, dragontrip_error_times);
				return;
			}

			if (get_data(e_dragontrip_active_state) != 0)
			{
				oper_msg(oper_type, dragontrip_error_state);
				return;
			}

			if (is_free())
			{
				set_data(e_dragontrip_free_time, time_helper::get_time());
			}
			else
			{
				if (!player_ref.can_cut_money((e_money_type)get_config_param(e_dragontrip_param_money_type), get_config_param(e_dragontrip_param_money_num)))
				{
					oper_msg(oper_type, dragontrip_error_not_money);
					return;
				}

				player_ref.cut_money((e_money_type)get_config_param(e_dragontrip_param_money_type), get_config_param(e_dragontrip_param_money_num), e_server_log_cut_money_dragontrip);
			}
			set_data(e_dragontrip_active_state, 1);
			int32 day_times = get_data(e_dragontrip_day_times);
			set_data(e_dragontrip_day_times, cur_times + 1);

		}
		else if (param == 1)
		{
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num)) == false)
			{
				oper_msg(oper_type, dragontrip_error_not_item);
				return;

			}
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num));
			set_data(e_dragontrip_active_state, 2);
		}
		else
		{
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num) * dragontrip_five_num) == false)
			{
				oper_msg(oper_type, dragontrip_error_not_item2);
				return;

			}
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, get_config_param(e_dragontrip_param_item_type), get_config_param(e_dragontrip_param_item_num) * dragontrip_five_num);
			set_data(e_dragontrip_active_state, 3);
		}

		if (get_data(e_dragontrip_active_state) <= 2)
		{
			on_roll(oper_type, param);
		}
		else
		{
			on_five_roll(oper_type, param);
		}

		//sync_all_message_to_client();
		//oper_msg(oper_type, dragontrip_error_sucess);
	}

	void dragontrip_mgr::get_reward(int32 type, int32 pos)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		//DragonTripTemplate *config_ptr = get_item_by_type_pos(type, pos);
		//if (config_ptr == nullptr)
		//{
		//	return;
		//}
		int32 item_id = 0;
		int32 item_num = 0;
		get_round_reward(pos, item_id, item_num);
		if (item_id == 0)
		{
			return;
		}
		ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_id);
		if (nullptr == ItemTemplatePtr)
		{
			return;
		}


		std::vector<s_item_template_info> promp_item_data;
		promp_item_data.push_back({ item_id ,item_num , 1 });
		item_system::put_item_into_bag(&temp_player, promp_item_data);
		item_system::send_promp_msg_to_client(&temp_player, promp_item_data);
	}
	int32 dragontrip_mgr::get_config_param(e_dragontrip_param type)
	{
		int32 config_init[e_dragontrip_param_max] = { 3, 100, 86400, 10,  31140405,1, 28, 5};

		if (GAMECONFIG->DragontripArray.size() < e_dragontrip_param_max)
		{
			return config_init[type];
		}
		return GAMECONFIG->DragontripArray[type];
	}

	DragonTripTemplate *dragontrip_mgr::get_item_by_type_pos(int32 type, int32 pos)
	{
		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_DragonTripTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return nullptr;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 id = it->first;
			DragonTripTemplate* config_ptr = (DragonTripTemplate*)it->second;
			if (config_ptr == nullptr)
			{
				continue;
			}
			if (config_ptr->Type != type)
			{
				continue;
			}
			if (config_ptr->Pos != pos)
			{
				continue;
			}
			return config_ptr;
		}
		return nullptr;
	}

	void dragontrip_mgr::oper_msg(int32 oper_type, int32 result,int32 roll_num, int32 item_id, int32 item_num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		dragontrip_proto_dragontrip_operate_end end_msg;

		end_msg.set_oper_type(oper_type);
		end_msg.set_result(result);
		end_msg.set_roll_num(roll_num);
		end_msg.set_item_id(item_id);
		end_msg.set_item_num(item_num);

		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_dragontrip_operate_end);
	}

	void dragontrip_mgr::oper_msg_item_list(int32 oper_type, int32 result, int32 roll_num, std::vector<int32> item_list)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		dragontrip_proto_dragontrip_operate_end end_msg;

		end_msg.set_oper_type(oper_type);
		end_msg.set_result(result);
		end_msg.set_roll_num(roll_num);

		for (int32 i = 0; i < item_list.size() / 3; ++i)
		{
			dragontrip_proto_dragontrip_item_info * item_info = end_msg.add_item_list();
			if (nullptr == item_info)
			{
				break;
			}
			item_info->set_item_id(item_list[i * 3]);
			item_info->set_item_num(item_list[i * 3 + 1]);
			item_info->set_end_pos(item_list[i * 3 + 2]);
		}
	
		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_dragontrip_operate_end);
	}

	void dragontrip_mgr::day_refresh()
	{
		set_data(e_dragontrip_day_times, 0);
		sync_all_message_to_client();

		oper_msg(e_dragontrip_oper_refresh_day_times, 0);

	}

	bool dragontrip_mgr::is_free()
	{
		int32 free_time = get_data(e_dragontrip_free_time);
		int32 cur_time = time_helper::get_time();
		if (cur_time - free_time > get_config_param(e_dragontrip_param_cd_second))
		{
			return true;
		}
		return false;

	}

	void dragontrip_mgr::set_free_time()
	{
		set_data(e_dragontrip_free_time, time_helper::get_time());
		sync_all_message_to_client();
	}

	

	void dragontrip_mgr::set_data(int32 idx, int32 value)
	{
		if (idx < e_dragontrip_free_time || idx > e_dragontrip_max)
		{
			return;
		}
		m_data.data_ary[idx] = value;
	}
	int32 dragontrip_mgr::get_data(int32 idx) const
	{
		if (idx < e_dragontrip_free_time || idx > e_dragontrip_max)
		{
			return -1;
		}
		return m_data.data_ary[idx];
	}

	int32 dragontrip_mgr::get_reward_flag(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return false;
		}
		return (m_data.data_ary[e_dragontrip_rewardflag] & 1 << idx) != 0 ? 1 : 0;
	}

	void dragontrip_mgr::set_reward_flag(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return;
		}
		m_data.data_ary[e_dragontrip_rewardflag] |= 1 << idx;
	}

	void dragontrip_mgr::refresh_recycle()
	{
		if (is_use_lua())
		{
			return;
		}
		int32 state = get_data(e_dragontrip_active_state);
		m_data.reset();
		set_data(e_dragontrip_active_state, state);
		sync_all_message_to_client();

		oper_msg(e_dragontrip_oper_refresh_recycle, 0);
	}

	void dragontrip_mgr::get_activity_item(int32 level, int32& out_item_id, int32& out_item_num)
	{
		out_item_id = 0;
		out_item_num = 0;

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		time_limit_activity_object& dragontrip_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_dragontrip);
		if (dragontrip_act.is_valid() == false)
		{
			return;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = dragontrip_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return;
		}

	    const std::vector<int32> *p_vec = nullptr;

		switch (level)
		{
		case e_item_color_red:
			p_vec = &branch_template_ptr.ParamArr3;
			break;
		case e_item_color_orange:
			p_vec = &branch_template_ptr.ParamArr2;
			break;
		case e_item_color_purple:
			p_vec = &branch_template_ptr.ParamArr1;
			break;
		default:
			break;
		}
		if (p_vec == nullptr)
		{
			return;
		}
		int32 num = p_vec->size();

		if (num % 3 != 0 || num == 0)
		{
			return;
		}

		int32 total_weight = 0;

		for (int i = 0; i < num / 3;  i++)
		{
			total_weight += (*p_vec)[i * 3 + 2];
		}

		int32 random_value = random_gen::get_random(0, total_weight);
		int32 cur_weight = 0;
		for (int32 i = 0; i < num / 3; i++)
		{
			cur_weight += (*p_vec)[i * 3 + 2];
			if (cur_weight >= random_value)
			{
				out_item_id = (*p_vec)[i * 3 + 0];
				out_item_num = (*p_vec)[i * 3 + 1];
				break;
			}
		}

	}

	void dragontrip_mgr::get_round_reward(int32 pos, int32& out_item_id, int32& out_item_num)
	{
		out_item_id = 0;
		out_item_num = 0;

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		time_limit_activity_object& dragontrip_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_dragontrip);
		if (dragontrip_act.is_valid() == false)
		{
			return;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = dragontrip_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return;
		}

		int32 num = branch_template_ptr.Reward.size();

		if (num % 3 != 0 || num == 0)
		{
			return;
		}

		for (int32 i= 0; i < num / 3; i++)
		{
			if (branch_template_ptr.Reward[i * 3 + 0] == pos)
			{
				out_item_id = branch_template_ptr.Reward[i * 3 + 1];
				out_item_num = branch_template_ptr.Reward[i * 3 + 2];
				break;
			}
		}
	}

}