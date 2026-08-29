#include "skytreasure_mgr.h"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "utility/random.h"
#include "logic/player.hpp"
#include "template/template_manager.h"
#include "time_limit_activity_object.h"
#include "system/item/item_system.h"
#include "internet/cs2dp.pb.h"
#include "internal/char_msg.hpp"
#include "internet/dp2cs.pb.h"
#include "utility/parse_msg.h"
#include "internet/net.pb.h"
#include "internet/skytreasure.pb.h"
#include "time_limit_activity_temp_mgr.hpp"
#include "utility/init_unit.h"

namespace faith
{
	skytreasure_mgr::skytreasure_mgr()
	{
		m_array_index = 0;
		clear_data();
	}
	skytreasure_mgr::~skytreasure_mgr()
	{

	}

	void skytreasure_mgr::clear_data()
	{
		for (int32 i = 0; i < max_skytreasure_num; i++)
		{
			m_skytreasure_info_list[i].clear_data();
		}
		m_skytreasure_num = 0;
	}

	void skytreasure_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}
	void skytreasure_mgr::save_skytreasure_to_db(s_skytreasure_info skytreasure_info)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_role_skytreasure_to_db);
		if (is_use == false)
		{
			cs2dp_save_role_skytreasure_to_db req;
			//req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;
			req.skytreasure_info = skytreasure_info;

			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_role_skytreasure_to_db));
		}
		else
		{
			cs2dp_proto::save_role_skytreasure msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			cs2dp_proto::role_skytreasure_db * db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			db_data->set_skytreasure_layer_num(skytreasure_info.skytreasure_layer_num);
			db_data->set_skytreasure_pos(skytreasure_info.skytreasure_pos);
			db_data->set_skytreasure_pos_in_array(skytreasure_info.skytreasure_pos_in_array);

			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_role_skytreasure_to_db);
		}

	}

	void skytreasure_mgr::load_skytreasure_by_db(const int32 skytreasure_num, const s_skytreasure_info skytreasure_info_list[max_skytreasure_num])
	{
		if (skytreasure_num >= max_skytreasure_num)
		{
			return;
		}
		for (int32 i = 0; i < skytreasure_num; i++)
		{
			m_skytreasure_info_list[m_skytreasure_num] = skytreasure_info_list[i];
			m_skytreasure_num++;
		}
		sync_all_message_to_client();
	}

	bool skytreasure_mgr::load_skytreasure_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		dp2cs_proto_load_role_skytreasure msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}
		dp2cs_get_role_skytreasure* p_data = (dp2cs_get_role_skytreasure *)msg.db_data().c_str();
		load_skytreasure_by_db(msg.row_count(), p_data->skytreasure_data_list);
		return true;
	}

	void skytreasure_mgr::sync_all_message_to_client()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		skytreasure_proto::skytreasure_all msg_all;
		for (int32 i = 0; i < m_skytreasure_num && i < max_skytreasure_num; i++)
		{
			skytreasure_proto_skytreasure_info_one* one_data_info = msg_all.add_skytreasure_list();
			if (one_data_info == nullptr)
			{
				return;
			}
			one_data_info->set_skytreasure_layer_num(m_skytreasure_info_list[i].skytreasure_layer_num);
			one_data_info->set_skytreasure_pos(m_skytreasure_info_list[i].skytreasure_pos);
			one_data_info->set_skytreasure_pos_in_array(m_skytreasure_info_list[i].skytreasure_pos_in_array);
		}

		temp_player.send_message_to_self(&msg_all, e_msgindex_s2c_skytreasure_all);

	}

	void skytreasure_mgr::refresh_recycle()
	{
		for (int32 i = 0; i < m_skytreasure_num; i++)
		{
			if (m_skytreasure_info_list[i].skytreasure_layer_num >= 0 && m_skytreasure_info_list[i].skytreasure_pos >=0)
			{
				m_skytreasure_info_list[i].skytreasure_pos_in_array = -1;

				//周期刷新改变数据库
				s_skytreasure_info skytreasure_info;
				skytreasure_info.skytreasure_layer_num = m_skytreasure_info_list[i].skytreasure_layer_num;
				skytreasure_info.skytreasure_pos = m_skytreasure_info_list[i].skytreasure_pos;
				skytreasure_info.skytreasure_pos_in_array = m_skytreasure_info_list[i].skytreasure_pos_in_array;
				save_skytreasure_to_db(skytreasure_info);
			}
		}
		sync_all_message_to_client();
		operate_end(e_skytreasure_refresh_recycle, skytreasure_operate_sucess);
	}

	void skytreasure_mgr::on_operate(int32 operate_type, int32 param1, int32 param2)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (!temp_player.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_sky_treasure))
		{
			return;
		}
		switch (operate_type)
		{
		case e_skytreasure_get_reward:
			get_reward(operate_type, param1, param2);
			break;
		case e_skytreasure_buy_key:
			buy_key(operate_type, param1, param2);
			break;
		default:
			break;
		}
	}

	void skytreasure_mgr::get_reward(int32 operate_type, int32 layer_num, int32 slot_pos)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (layer_num < 0 || slot_pos < 0)
		{
			return;
		}
		//判断这个格子是不是已经翻过
		for (int32 i = 0; i < m_skytreasure_num; i++)
		{
			if (m_skytreasure_info_list[i].skytreasure_layer_num == layer_num && m_skytreasure_info_list[i].skytreasure_pos == slot_pos && m_skytreasure_info_list[i].skytreasure_pos_in_array >= 0)
			{
				return;
			}
		}

		time_limit_activity_object& skytreasure_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_sky_treasure);
		if (skytreasure_act.is_valid() == false)
		{
			return;
		}
		int32 branch_template_id = skytreasure_act.get_branch_template_id() + layer_num;

		const s_time_limit_activity_branch_temp& branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id);
		if (branch_template_ptr.ActGuid < 0)
		{
			return;
		}

		int32 num = branch_template_ptr.Reward.size();
		if (num % 2 != 0 || num == 0)
		{
			return;
		}
		if (branch_template_ptr.ParamArr1.size() < 3)
		{
			return;
		}
		if (branch_template_ptr.ParamArr2.size() < 3)
		{
			return;
		}

		//找到当前层翻开奖励的数量 并将已有奖励的位置放到数组里
		int32 get_reward_num = 0;        //当前层翻牌次数
		int32 reward_arr[max_skytreasure_reward_num];
		memset(reward_arr, -1, sizeof(reward_arr));
		for (int32 j = 0; j < max_skytreasure_num; j++)
		{
			if (m_skytreasure_info_list[j].skytreasure_layer_num == layer_num && m_skytreasure_info_list[j].skytreasure_pos_in_array >= 0)
			{
				if (m_skytreasure_info_list[j].skytreasure_pos_in_array < max_skytreasure_reward_num)
				{
					reward_arr[m_skytreasure_info_list[j].skytreasure_pos_in_array] = m_skytreasure_info_list[j].skytreasure_pos_in_array;
				}
				get_reward_num++;
			}
		}

		if (reward_arr[0] >= 0)
		{
			get_reward_num--; //除去大奖
		}

		//获得随机奖励 不能随到之前有的
		int32 reward_pos = random_gen::get_random(0, num / 2 - get_reward_num - 2);
		int32 temp_num = 0;
		int32 real_reward_pos_in_arr = -1;

		//翻牌小于等于10次随机一个不是大奖的  (大奖是表中第一个)
		if (get_reward_num < branch_template_ptr.ParamArr2[0])
		{
			for (int32 j = 0; j < max_skytreasure_reward_num; j++)
			{
				if (reward_arr[j] == -1 && j > 0)
				{
					if (temp_num == reward_pos)
					{
						real_reward_pos_in_arr = j;
						break;
					}
					temp_num++;
				}
			}
		}
		else //翻牌大于10次有 (次数*0.2+10)% 几率获得大奖 
		{
			//拿到过大奖了
			if (reward_arr[0] >= 0)
			{
				for (int32 j = 0; j < max_skytreasure_reward_num; j++)
				{
					if (reward_arr[j] == -1 && j > 0)
					{
						if (temp_num == reward_pos)
						{
							real_reward_pos_in_arr = j;
							break;
						}
						temp_num++;
					}
				}
			}
			else
			{
				//如果只剩大奖了 给他大奖
				if (num / 2 - get_reward_num == 1)
				{
					real_reward_pos_in_arr = 0;
				}
				else
				{
					//没有拿到过大奖 先抽大奖
					int32 big_reward_random_num = random_gen::get_random(0, 1000);
					if (big_reward_random_num < get_reward_num * (branch_template_ptr.ParamArr2[1] * 0.1f) + (branch_template_ptr.ParamArr2[2] * 0.1f))
					{
						real_reward_pos_in_arr = 0;
					}
					else
					{
						//随机一个不是大奖的
						for (int32 j = 0; j < max_skytreasure_reward_num; j++)
						{
							if (reward_arr[j] == -1 && j > 0)
							{
								if (temp_num == reward_pos)
								{
									real_reward_pos_in_arr = j;
									break;
								}
								temp_num++;
							}
						}
					}
				}
			}
		}
		
		int32 item_id = 0;
		int32 item_num = 0;
		if (real_reward_pos_in_arr * 2 + 1 >= branch_template_ptr.Reward.size())
		{
			return;
		}
		item_id = branch_template_ptr.Reward[real_reward_pos_in_arr * 2];
		item_num = branch_template_ptr.Reward[real_reward_pos_in_arr * 2 + 1];
	
		if (item_id == 0 || item_num == 0)
		{
			return;
		}

		//扣除钥匙
		if (item_system::can_cost_item(&player_ref, e_bag_type_bag, branch_template_ptr.ParamArr1[0], 1) == false)
		{
			operate_end(operate_type, skytreasure_operate_error_not_item);
			return;
		}
		item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, branch_template_ptr.ParamArr1[0], 1);

		//获得奖励
		//vector<int32> promp_item_data;

		citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_skytreasure, 0, item_id, item_num, 1);
		if (temp_item != nullptr)
		{
			player_ref.get_item_set().put_in_bag(temp_item);

			/*promp_item_data.push_back(item_id);
			promp_item_data.push_back(item_num);
			promp_item_data.push_back(temp_item->get_data_info(e_item_info_locked));

			player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);*/
		}

		//如果是大奖则发送公告
		if (item_id == branch_template_ptr.Condition[0])
		{
			send_skytreasure_notice(item_id);
		}

		//存库 每翻一次牌 存一次库
		s_skytreasure_info skytreasure_info;
		skytreasure_info.skytreasure_layer_num = layer_num;
		skytreasure_info.skytreasure_pos = slot_pos;
		skytreasure_info.skytreasure_pos_in_array = real_reward_pos_in_arr;
		save_skytreasure_to_db(skytreasure_info);

		m_skytreasure_info_list[m_skytreasure_num] = skytreasure_info;
		m_skytreasure_num++;

		sync_all_message_to_client();

		operate_end(operate_type, skytreasure_operate_sucess);
	}

	void skytreasure_mgr::buy_key(int32 operate_type, int32 branch_template_id, int32 item_num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(branch_template_id);
		if (branch_template_ptr.ActGuid < 0)
		{
			return;
		}

		int32 num = branch_template_ptr.ParamArr1.size();
		if (num < 3)
		{
			return;
		}

		//扣除钥匙花费
		if (!player_ref.can_cut_money((e_money_type)branch_template_ptr.ParamArr1[1],branch_template_ptr.ParamArr1[2] * item_num))
		{
			operate_end(operate_type, skytreasure_operate_error_not_money);
			return;
		}
		player_ref.cut_money((e_money_type)branch_template_ptr.ParamArr1[1], branch_template_ptr.ParamArr1[2] * item_num, e_server_log_cut_money_skytreasure_buy_key);

		//获得钥匙
		std::vector<s_item_template_info> promp_item_data;

		citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_skytreasure, 0, branch_template_ptr.ParamArr1[0], item_num, 1);
		if (temp_item != nullptr)
		{
			player_ref.get_item_set().put_in_bag(temp_item);
			promp_item_data.push_back({ branch_template_ptr.ParamArr1[0], item_num,  temp_item->get_data_info(e_item_info_locked) });
			player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
		}

		operate_end(operate_type, skytreasure_operate_sucess);
	}

	void skytreasure_mgr::send_skytreasure_notice(int32 item_id)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (nullptr == item_template_ptr)
		{
			return;
		}

		int32 notice_id = skytreasure_notice_id;		
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(player_ref.get_name());
		vec_notice_str.push_back(template_manager::get_instance().int_to_string(item_template_ptr->ItemName));
		xstring notice_str = init_unit::implode(vec_notice_str);
		player_ref.get_chat_mgr().send_notice(notice_id, notice_str);
		
	}

	void skytreasure_mgr::operate_end(int32 operate_type, int32 result)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		skytreasure_proto_skytreasure_operate_end end_msg;

		end_msg.set_operation_type(operate_type);
		end_msg.set_operation_result(result);

		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_skytreasure_operate_end);
	}
}