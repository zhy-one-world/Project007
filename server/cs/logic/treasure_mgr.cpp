/********************************************************************
created: 2017-5-26 16:50:30
file base: treasure_mgr
file ext: cpp
author: zhy

purpose:
*********************************************************************/
#include "logic/unit_man.h"
#include "treasure_mgr.h"
#include "logic/item_set.h"
#include "server_log.hpp"
#include "time.hpp"
#include "time_def.hpp"
#include "logic/player.hpp"
#include "system/item/item_system.h"
#include "internal/treasure_msg.hpp"
#include "internet/character.pb.h"
#include "internet/net.pb.h"
#include "utility/init_unit.h"
namespace hld
{

	ctreasure_mgr::ctreasure_mgr()
	{
		m_unit_array_index = 0;
		clear_data();
	}

	ctreasure_mgr::~ctreasure_mgr(void)
	{
		m_unit_array_index = 0;
		clear_data();
	}

	void ctreasure_mgr::heart_tick(const int64& new_time)
	{

	}
	void ctreasure_mgr::set_player_ptr(int32 unit_array_index)
	{
		m_unit_array_index = unit_array_index;
	}
	void ctreasure_mgr::clear_data()
	{
		m_prize_record_list.clear();
	}

	void ctreasure_mgr::save_treasure_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);

		cs2dp_save_char_treasure req;
		req.save_type_ex = save_type;
		req.role_guid = player_ref.get_unit_guid();
		req.unit_array_index = m_unit_array_index;
		int32 data_count = 0;

		for (std::list<s_treasure_record>::iterator it = m_prize_record_list.begin(); it != m_prize_record_list.end(); ++it)
		{
			req.data_list[data_count++] = *it;
		}
		req.data_num = data_count;
		player_ref.send_message_to_dp( &req, req.get_pak_length());
	}
	void ctreasure_mgr::load_treasure_by_db(const s_treasure_record* treasure_data, int32 data_num)
	{
		m_prize_record_list.clear();
		for (int32 i = 0; i < data_num; i++)
		{
			m_prize_record_list.push_back(treasure_data[i]);
		}
		send_treasure_all();
	}
	void ctreasure_mgr::send_treasure_all()
	{
		character_proto_treasure_prize_record record_msg;

		for (std::list<s_treasure_record>::iterator it = m_prize_record_list.begin(); it != m_prize_record_list.end(); ++it)
		{
			for (int32 i = 0; i < e_treasure_record_max; ++i)
			{
				record_msg.add_record_list(it->data_ary[i]);
			}
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		player_ref.send_message_to_self(&record_msg, e_msgindex_s2c_treasure_record_list);
	}

	void ctreasure_mgr::treasure_prize(int32 treasure_type, int32 prize_type, int32 cost_money_type, int32 cost_money_value, int32 prize_score, int32 prize_luck, int32 item_id, int32 item_num, int32 prize_list[treasure_prize_num])
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 is_free = 0;
		int32 cost_lock = 0;
		int32 old_lucky_value = 0;
		int32 new_lucky_value = 0;
		int32 is_first_time = 0;
		std::vector<int32> log_item_array;
		log_item_array.reserve(treasure_prize_num + 2);

		if (prize_type == e_treasure_prize_type_one)
		{
			switch (treasure_type)
			{
			case e_treasure_type_element:
			{
				is_free = player_ref.get_logic_data(e_role_logic_info_element_free);
				if (is_free > 0)
				{
					player_ref.set_time_data(e_time_type_treasure_element_free, utility::get_tick_count() + day_tick_time);
					player_ref.send_time_one(e_time_type_treasure_element_free);
					player_ref.set_logic_data(e_role_logic_info_element_free, is_free - 1);
					player_ref.send_logic_one(e_role_logic_info_element_free);

					old_lucky_value = player_ref.get_logic_data(e_role_logic_info_element_luck);
					new_lucky_value = player_ref.get_logic_data(e_role_logic_info_element_luck);
				}
			}
			break;
			case e_treasure_type_item:
			{
				is_free = player_ref.get_logic_data(e_role_logic_info_jewel_free);
				if (is_free > 0)
				{
					player_ref.set_time_data(e_time_type_treasure_jewel_free, utility::get_tick_count() + day_tick_time);
					player_ref.send_time_one(e_time_type_treasure_jewel_free);
					player_ref.set_logic_data(e_role_logic_info_jewel_free, is_free - 1);
					player_ref.send_logic_one(e_role_logic_info_jewel_free);

					old_lucky_value = player_ref.get_logic_data(e_role_logic_info_jewel_luck);
					new_lucky_value = player_ref.get_logic_data(e_role_logic_info_jewel_luck);
				}
			}
			break;
			case e_treasure_type_spirit:
			{
				is_free = player_ref.get_logic_data(e_role_logic_info_spirit_free);
				if (is_free > 0)
				{
					player_ref.set_time_data(e_time_type_treasure_spirit_free, utility::get_tick_count() + day_tick_time);
					player_ref.send_time_one(e_time_type_treasure_spirit_free);
					player_ref.set_logic_data(e_role_logic_info_spirit_free, is_free - 1);
					player_ref.send_logic_one(e_role_logic_info_spirit_free);

					old_lucky_value = player_ref.get_logic_data(e_role_logic_info_spirit_luck);
					new_lucky_value = player_ref.get_logic_data(e_role_logic_info_spirit_luck);
				}
			}
			break;
			default:
				is_free = 0;
				break;
			}
		}

		if (is_free == 0)
		{
			// 钻石抽奖处理
			if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_ten)
			{
				if (player_ref.can_cut_money((e_money_type)cost_money_type, cost_money_value) == false)
				{
					return;
				}
				player_ref.cut_money((e_money_type)cost_money_type, cost_money_value, e_server_log_cut_money_treasure_prize);
				if (treasure_type != e_treasure_type_time_limit)
				{
					player_ref.add_money(e_money_type_treasure_score, prize_score, e_server_log_add_money_treasure_prize, -1, false);
					player_ref.send_money_one(e_money_type_treasure_score);
				}
				std::vector<int32> must_item;
				must_item.push_back(item_id);
				must_item.push_back(item_num);
				log_item_array.push_back(item_id);
				log_item_array.push_back(item_num);

				player_ref.get_item_set().put_in_bag(e_server_log_add_item_treasure, treasure_type, must_item);
			}
			else  // 道具抽奖处理
			{
				if (item_system::can_cost_item(&player_ref, e_bag_type_bag, cost_money_type, cost_money_value) == false)
				{
					return; 
				}
				if (item_system::get_item_count(&player_ref, e_bag_type_bag, cost_money_type, e_item_lock_lock) > 0)
				{
					cost_lock = 1;
				}
				item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, cost_money_type, cost_money_value);
			}
		}
		if (prize_luck == -1)
		{
			if (treasure_type == e_treasure_type_item)
			{
				old_lucky_value = player_ref.get_logic_data(e_role_logic_info_jewel_luck);
				player_ref.set_logic_data(e_role_logic_info_jewel_luck, 0);
				player_ref.send_logic_one(e_role_logic_info_jewel_luck);
				new_lucky_value = player_ref.get_logic_data(e_role_logic_info_jewel_luck);
				if (prize_type == e_treasure_prize_type_one_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_jewel_first_one) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_jewel_first_one, 1);
				}
				else if (prize_type == e_treasure_prize_type_ten_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_jewel_first_ten) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_jewel_first_ten, 1);
				}
			}
			else if (treasure_type == e_treasure_type_element)
			{
				old_lucky_value = player_ref.get_logic_data(e_role_logic_info_element_luck);
				player_ref.set_logic_data(e_role_logic_info_element_luck, 0);
				player_ref.send_logic_one(e_role_logic_info_element_luck);
				new_lucky_value = player_ref.get_logic_data(e_role_logic_info_element_luck);
				if (prize_type == e_treasure_prize_type_one_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_element_first_one) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_element_first_one, 1);
				}
				else if (prize_type == e_treasure_prize_type_ten_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_element_first_ten) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_element_first_ten, 1);
				}
			}
			else if (treasure_type == e_treasure_type_spirit)
			{
				old_lucky_value = player_ref.get_logic_data(e_role_logic_info_spirit_luck);
				player_ref.set_logic_data(e_role_logic_info_spirit_luck, 0);
				player_ref.send_logic_one(e_role_logic_info_spirit_luck);
				new_lucky_value = player_ref.get_logic_data(e_role_logic_info_spirit_luck);

				if (prize_type == e_treasure_prize_type_one_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_spirit_first_one) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_spirit_first_one, 1);
				}
				else if (prize_type == e_treasure_prize_type_ten_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_spirit_first_ten) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_spirit_first_ten, 1);
				}
			}
		}
		else
		{
			if (treasure_type == e_treasure_type_item)
			{
				old_lucky_value = player_ref.get_logic_data(e_role_logic_info_jewel_luck);
				new_lucky_value = old_lucky_value;
				if (false == is_free)
				{
					player_ref.set_logic_data(e_role_logic_info_jewel_luck, old_lucky_value + prize_luck);
					player_ref.send_logic_one(e_role_logic_info_jewel_luck);
					new_lucky_value = player_ref.get_logic_data(e_role_logic_info_jewel_luck);
				}
				if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_one_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_jewel_first_one) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_jewel_first_one, 1);
				}
				else if (prize_type == e_treasure_prize_type_ten || prize_type == e_treasure_prize_type_ten_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_jewel_first_ten) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_jewel_first_ten, 1);
				}
			}
			else if (treasure_type == e_treasure_type_element)
			{
				old_lucky_value = player_ref.get_logic_data(e_role_logic_info_element_luck);
				new_lucky_value = old_lucky_value;
				if (false == is_free)
				{
					player_ref.set_logic_data(e_role_logic_info_element_luck, old_lucky_value + prize_luck);
					player_ref.send_logic_one(e_role_logic_info_element_luck);
					new_lucky_value = player_ref.get_logic_data(e_role_logic_info_element_luck);
				}
				if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_one_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_element_first_one) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_element_first_one, 1);
				}
				else if (prize_type == e_treasure_prize_type_ten || prize_type == e_treasure_prize_type_ten_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_element_first_ten) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_element_first_ten, 1);
				}
			}
			else if (treasure_type == e_treasure_type_spirit)
			{
				old_lucky_value = player_ref.get_logic_data(e_role_logic_info_spirit_luck);
				new_lucky_value = old_lucky_value;
				if (false == is_free)
				{
					player_ref.set_logic_data(e_role_logic_info_spirit_luck, old_lucky_value + prize_luck);
					player_ref.send_logic_one(e_role_logic_info_spirit_luck);
					new_lucky_value = player_ref.get_logic_data(e_role_logic_info_spirit_luck);
				}
				if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_one_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_spirit_first_one) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_spirit_first_one, 1);
				}
				else if (prize_type == e_treasure_prize_type_ten || prize_type == e_treasure_prize_type_ten_item)
				{
					if (player_ref.get_logic_data(e_role_logic_info_spirit_first_ten) <= 0)
					{
						is_first_time = 1;
					}
					player_ref.set_logic_data(e_role_logic_info_spirit_first_ten, 1);
				}
			}
			else if (treasure_type == e_treasure_type_new_server)
			{
				old_lucky_value = 0;
				new_lucky_value = 0;
				is_first_time = 0;
			}
			else if (treasure_type == e_treasure_type_time_limit)
			{
				// 增加活动抽奖次数
				player_ref.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_limit_time_treasure2, prize_score, 0);
				// 增加活动抽奖幸运值
				player_ref.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_limit_time_treasure2, prize_luck, 1);
			}
			
		}
		//限时行为皇家宝藏抽奖计数
		if (treasure_type == e_treasure_type_item )//|| treasure_type == e_treasure_type_new_server)
		{
			if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_one_item)
			{
				player_ref.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_treasure_lottery, 1);  //单抽，一次
				player_ref.get_star_trip_mgr().target_mission(e_star_trip_target_type_treasure, 1);
			}
			else if (prize_type == e_treasure_prize_type_ten || prize_type == e_treasure_prize_type_ten_item)
			{
				player_ref.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_treasure_lottery, 10);  //十抽，十次
				player_ref.get_star_trip_mgr().target_mission(e_star_trip_target_type_treasure, 10);
			}
		}
		static character_proto_treasure_get_prize_end msg;
		msg.set_operate_type(treasure_type);
		msg.set_prize_type(prize_type);
		msg.clear_prize_list();
		static int32 record_time = time_helper::get_time();
		int32 now_time = time_helper::get_time();
		if (now_time  <= record_time)
		{
			now_time = record_time + 1;
		}
		record_time = now_time;
		s_treasure_record treasure_record;
		treasure_record.data_ary[e_treasure_record_time] = now_time;
		treasure_record.data_ary[e_treasure_record_treasure_type] = treasure_type;
		treasure_record.data_ary[e_treasure_record_prize_type] = prize_type;
		int32 record_index = e_treasure_record_prize_0;
		//把符文礼包丢到背包里
		std::vector<int32> item_inst_array;
		item_inst_array.clear();

		bool is_bag_full = false;
		//e_bag_type bag_type = e_bag_type_treasure;
		e_bag_type bag_type = e_bag_type_bag;
		if (treasure_type == e_treasure_type_element)
		{
			bag_type = e_bag_type_elemenet;
		}
		std::vector<s_item_template_info> item_array;
		for (int32 i = 0; i < treasure_prize_num - 2; i = i + 3)
		{
			if (prize_list[i] >= 0)
			{
				msg.add_prize_list(prize_list[i]);
				treasure_record.data_ary[record_index++] = prize_list[i + 1];
				int32 temp_item_id = prize_list[i + 1];
				int32 temp_item_num = prize_list[i + 2];
				log_item_array.push_back(temp_item_id);
				log_item_array.push_back(temp_item_num);

				//符文宝藏条件下 物品不是元素符文 就是符文礼包 直接丢到背包里
				if (treasure_type == e_treasure_type_element)
				{
					ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, temp_item_id);
					if (item_template_ptr != nullptr && item_template_ptr->item_type != e_item_type_element_heart)
					{
						item_inst_array.push_back(temp_item_id);
						item_inst_array.push_back(temp_item_num);
						continue;
					}
				}
				int32 is_lock = 0;
				if (prize_type == e_treasure_prize_type_one_item || prize_type == e_treasure_prize_type_ten_item)
				{
					is_lock = 1;
				}
				if (cost_lock > 0)
				{
					is_lock = 1;
				}
				item_array.push_back({ temp_item_id , temp_item_num, is_lock });
			}
		}

		player_ref.get_item_set().put_in_bag(e_server_log_add_item_treasure_prize, player_ref.get_unit_info(e_role_info_move_map_id), item_array, e_bag_type_none, e_item_mail_type_blessing);

		if (item_inst_array.size() > 0)
		{
			player_ref.get_item_set().put_in_bag(e_server_log_add_item_treasure, treasure_type, item_inst_array);
		}

		player_ref.send_message_to_self(&msg, e_msgindex_s2c_treasure_get_przie);
		m_prize_record_list.push_front(treasure_record);
		if (m_prize_record_list.size() >= treasure_prize_record_num)
		{
			m_prize_record_list.pop_back();
		}

		if (treasure_type == e_treasure_type_item)
		{
			e_time_limit_activity_type open_activity_type = e_time_limit_activity_type_max;
			if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure))
			{
				open_activity_type = e_time_limit_activity_type_treasure;
			}
			else if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_royal1))
			{
				open_activity_type = e_time_limit_activity_type_treasure_royal1;
			}
			else if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_royal2))
			{
				open_activity_type = e_time_limit_activity_type_treasure_royal2;
			}
			else if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_royal3))
			{
				open_activity_type = e_time_limit_activity_type_treasure_royal3;
			}
			if (open_activity_type != e_time_limit_activity_type_max)
			{
				if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_one_item)
				{
					player_ref.get_time_limit_activity_mgr().add_activity_schedule(open_activity_type, 1);
				}
				else
				{
					player_ref.get_time_limit_activity_mgr().add_activity_schedule(open_activity_type, 10);
				}
				player_ref.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_time_limit_activity_treasure, player_ref.get_time_limit_activity_mgr().get_activity_schedule(open_activity_type));
			}
		}
		else if (treasure_type == e_treasure_type_new_server)
		{
			if (prize_type == e_treasure_prize_type_one || prize_type == e_treasure_prize_type_one_item)
			{
				player_ref.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_server_treasure, 1);
			}
			else if (prize_type == e_treasure_prize_type_ten || prize_type == e_treasure_prize_type_ten_item)
			{
				player_ref.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_server_treasure, 10);
			}
		}

		if (player_ref.get_write_log())
		{
			time_limit_activity_mgr& activity_mgr = player_ref.get_time_limit_activity_mgr();
			if (treasure_type == e_treasure_type_item)
			{
				e_time_limit_activity_type open_activity_type = e_time_limit_activity_type_max;
				if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure))
				{
					open_activity_type = e_time_limit_activity_type_treasure;
				}
				else if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_royal1))
				{
					open_activity_type = e_time_limit_activity_type_treasure_royal1;
				}
				else if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_royal2))
				{
					open_activity_type = e_time_limit_activity_type_treasure_royal2;
				}
				else if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_royal3))
				{
					open_activity_type = e_time_limit_activity_type_treasure_royal3;
				}
				if (open_activity_type != e_time_limit_activity_type_max)
				{
					std::vector<int32> item_array;
					for (int32 i = 0; i < treasure_prize_num - 2; i = i + 3)
					{
						if (prize_list[i] >= 0)
						{
							item_array.push_back(prize_list[i + 1]);
							item_array.push_back(prize_list[i + 2]);
						}
					}
					server_log::time_limit_activity_log_zw(player_ref.get_third_info(), player_ref.get_unit_info_inst(), activity_mgr.get_activity_template_id(open_activity_type)
						, activity_mgr.get_activity_branch_template_id(open_activity_type), activity_mgr.get_activity_circle_num(open_activity_type)
						, activity_mgr.get_activity_circle_index(open_activity_type), activity_mgr.get_treasure_lua_award_index(open_activity_type)
						, cost_money_type, cost_money_value, item_array);

					time_limit_activity_object&	log_activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(open_activity_type);
					if (log_activity_ptr.is_valid())
					{
						for (int32 i = 0; i < item_array.size(); i += 2)
						{
							server_log::time_limit_activity_log(player_ref.get_unit_info_inst(), player_ref.get_third_info(),
								log_activity_ptr.get_branch_template_id(), open_activity_type, 0, 0, item_array[i], item_array[i + 1], player_ref.get_login_type(), cost_money_type, cost_money_value, log_activity_ptr.get_time_limit_activity_branch_temp().AttributeId);
						}
					}
				}
			}
			else if (treasure_type == e_treasure_type_element)
			{
				if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_element))
				{
					std::vector<int32> item_array;
					for (int32 i = 0; i < treasure_prize_num - 2; i = i + 3)
					{
						if (prize_list[i] >= 0)
						{
							item_array.push_back(prize_list[i + 1]);
							item_array.push_back(prize_list[i + 2]);
						}
					}
					server_log::time_limit_activity_log_zw(player_ref.get_third_info(), player_ref.get_unit_info_inst(), activity_mgr.get_activity_template_id(e_time_limit_activity_type_treasure_element)
						, activity_mgr.get_activity_branch_template_id(e_time_limit_activity_type_treasure_element), activity_mgr.get_activity_circle_num(e_time_limit_activity_type_treasure_element)
						, activity_mgr.get_activity_circle_index(e_time_limit_activity_type_treasure_element), activity_mgr.get_treasure_lua_award_index(e_time_limit_activity_type_treasure_element)
						, cost_money_type, cost_money_value, item_array);

					time_limit_activity_object&	log_activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_treasure_element);
					if (log_activity_ptr.is_valid())
					{
						for (int32 i = 0; i < item_array.size(); i += 2)
						{
							server_log::time_limit_activity_log(player_ref.get_unit_info_inst(), player_ref.get_third_info(),
								log_activity_ptr.get_branch_template_id(), e_time_limit_activity_type_treasure_element, 0, 0, item_array[i], item_array[i + 1], player_ref.get_login_type(), cost_money_type, cost_money_value, log_activity_ptr.get_time_limit_activity_branch_temp().AttributeId);
						}
					}
				}
			}
			else if (treasure_type == e_treasure_type_spirit)
			{
				if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_treasure_sprite))
				{
					std::vector<int32> item_array;
					for (int32 i = 0; i < treasure_prize_num - 2; i = i + 3)
					{
						if (prize_list[i] >= 0)
						{
								item_array.push_back(prize_list[i + 1]);
								item_array.push_back(prize_list[i + 2]);
						}
					}
					server_log::time_limit_activity_log_zw(player_ref.get_third_info(), player_ref.get_unit_info_inst(), activity_mgr.get_activity_template_id(e_time_limit_activity_type_treasure_sprite)
						, activity_mgr.get_activity_branch_template_id(e_time_limit_activity_type_treasure_sprite), activity_mgr.get_activity_circle_num(e_time_limit_activity_type_treasure_sprite)
						, activity_mgr.get_activity_circle_index(e_time_limit_activity_type_treasure_sprite), activity_mgr.get_treasure_lua_award_index(e_time_limit_activity_type_treasure_sprite)
						, cost_money_type, cost_money_value, item_array);

					time_limit_activity_object&	log_activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_treasure_sprite);
					if (log_activity_ptr.is_valid())
					{
						for (int32 i = 0; i < item_array.size(); i += 2)
						{
							server_log::time_limit_activity_log(player_ref.get_unit_info_inst(), player_ref.get_third_info(),
								log_activity_ptr.get_branch_template_id(), e_time_limit_activity_type_treasure_sprite, 0, 0, item_array[i], item_array[i + 1], player_ref.get_login_type(), cost_money_type, cost_money_value, log_activity_ptr.get_time_limit_activity_branch_temp().AttributeId);
						}
					}
				}
			}
			else if (treasure_type == e_treasure_type_new_server)
			{
				if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_new_server_treasure))
				{
					std::vector<int32> item_array;
					for (int32 i = 0; i < treasure_prize_num - 2; i = i + 3)
					{
						if (prize_list[i] >= 0)
						{
							item_array.push_back(prize_list[i + 1]);
							item_array.push_back(prize_list[i + 2]);
						}
					}
					server_log::time_limit_activity_log_zw(player_ref.get_third_info(), player_ref.get_unit_info_inst(), activity_mgr.get_activity_template_id(e_time_limit_activity_type_new_server_treasure)
						, activity_mgr.get_activity_branch_template_id(e_time_limit_activity_type_new_server_treasure), activity_mgr.get_activity_circle_num(e_time_limit_activity_type_new_server_treasure)
						, activity_mgr.get_activity_circle_index(e_time_limit_activity_type_new_server_treasure), activity_mgr.get_treasure_lua_award_index(e_time_limit_activity_type_new_server_treasure)
						, cost_money_type, cost_money_value, item_array);

					time_limit_activity_object&	log_activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_new_server_treasure);
					if (log_activity_ptr.is_valid())
					{
						for (int32 i = 0; i < item_array.size(); i += 2)
						{
							server_log::time_limit_activity_log(player_ref.get_unit_info_inst(), player_ref.get_third_info(),
								log_activity_ptr.get_branch_template_id(), e_time_limit_activity_type_new_server_treasure, 0, 0, item_array[i], item_array[i + 1], player_ref.get_login_type(), cost_money_type, cost_money_value, log_activity_ptr.get_time_limit_activity_branch_temp().AttributeId);
						}
					}
				}
			}
			else if (treasure_type == e_treasure_type_time_limit)
			{
				if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_limit_time_treasure2))
				{
					std::vector<int32> item_array;
					for (int32 i = 0; i < treasure_prize_num - 2; i = i + 3)
					{
						if (prize_list[i] >= 0)
						{
							item_array.push_back(prize_list[i + 1]);
							item_array.push_back(prize_list[i + 2]);
						}
					}
					server_log::time_limit_activity_log_zw(player_ref.get_third_info(), player_ref.get_unit_info_inst(), activity_mgr.get_activity_template_id(e_time_limit_activity_type_limit_time_treasure2)
						, activity_mgr.get_activity_branch_template_id(e_time_limit_activity_type_limit_time_treasure2), activity_mgr.get_activity_circle_num(e_time_limit_activity_type_limit_time_treasure2)
						, activity_mgr.get_activity_circle_index(e_time_limit_activity_type_limit_time_treasure2), activity_mgr.get_treasure_lua_award_index(e_time_limit_activity_type_limit_time_treasure2)
						, cost_money_type, cost_money_value, item_array);

					time_limit_activity_object&	log_activity_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_limit_time_treasure2);
					if (log_activity_ptr.is_valid())
					{
						for (int32 i = 0; i < item_array.size(); i += 2)
						{
							server_log::time_limit_activity_log(player_ref.get_unit_info_inst(), player_ref.get_third_info(),
								log_activity_ptr.get_branch_template_id(), e_time_limit_activity_type_limit_time_treasure2, 0, 0, item_array[i], item_array[i + 1], player_ref.get_login_type(), cost_money_type, cost_money_value, log_activity_ptr.get_time_limit_activity_branch_temp().AttributeId);
						}
					}
				}
			}

			server_log::lucky_draw_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), 
				log_item_array, treasure_type, prize_type, is_first_time, old_lucky_value, new_lucky_value, is_free, player_ref.get_login_type());

			set_log_var(log_head);
			
			player_ref.get_log_common_head_info(log_head);

			server_log::serverLuckyDraw(log_head, item_id, item_num, treasure_type, prize_type, is_first_time, old_lucky_value, new_lucky_value, is_free);
		}

		send_treasure_all();
	}

	void ctreasure_mgr::send_treasure_notice(int32 item_id, int32 item_quality, int32 operate_type)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 notice_id = 0;
		if (1 == operate_type)
		{
			if (1 == item_quality)
			{
				notice_id = treasure_royal_better_notice_id;
			}
			else if (2 == item_quality)
			{
				notice_id = treasure_royal_best_notice_id;
			}

		}
		else if (2 == operate_type)
		{
			if (1 == item_quality)
			{
				notice_id = treasure_element_better_notice_id;
			}
			else if (2 == item_quality)
			{
				notice_id = treasure_element_best_notice_id;
			}
		}
		else if (3 == operate_type)
		{
			if (1 == item_quality)
			{
				notice_id = spirit_element_better_notice_id;
			}
			else if (2 == item_quality)
			{
				notice_id = spirit_element_best_notice_id;
			}
		}
		else if (4 == operate_type)
		{
			if (1 == item_quality)
			{
				notice_id = time_limit_best_notice_id;
			}
			else if (2 == item_quality)
			{
				notice_id = stime_limit_better_notice_id;
			}
		}
		if (notice_id <= 0)
		{
			return;
		}
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (nullptr == item_template_ptr)
		{
			return;
		}
		s_item_info item_data;
		if (item_template_ptr->item_type == e_item_type_spirit)
		{
			item_data.data_ary[e_item_info_info_id] = item_id;
			item_data.data_ary[e_item_info_stack_count] = 1;
		}
		else
		{
			init_unit::init_item_data(item_data, item_id, 0, 1);
		}

		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		notice_str_params_vec.push_back(player_ref.get_name());
		notice_str_params_vec.push_back("%d");
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		player_ref.get_chat_mgr().send_notice(notice_id, notice_str, &item_data, 0);
	}
}

