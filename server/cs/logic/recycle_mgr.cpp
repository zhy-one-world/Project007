#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "utility/parse_msg.h"
#include "recycle_mgr.h"
#include "template/template_manager.h"
#include "time_def.hpp"
#include "internet/cs2dp.pb.h"
#include "internal/recycle_msg.h"
#include "internet/recycle.pb.h"
#include "internet/net.pb.h"
#include "utility/init_unit.h"
#include "connection_mgr.hpp"


namespace hld
{
	recycle_mgr::recycle_mgr()
	{
		m_array_index = 0;
		clear_data();
	}

	recycle_mgr::~recycle_mgr()
	{
	}

	void recycle_mgr::clear_data()
	{
		m_recycle_task_data.clear();
		m_recycle_obj.clear_data();
		m_load_num = 0;
		m_zone_sec = 0;

		m_is_request = false;

		m_recycle_invited_obj.clear_data();
	}
	void recycle_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;

		m_recycle_invited_obj.set_player_ptr(array_index);
	}

	void recycle_mgr::save_recycle_to_db(int32 save_type)
	{
	
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_recycle);
		if (is_use == false)
		{
			cs2dp_save_char_recycle req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;
			req.data_info = m_recycle_obj.get_db_data();

			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_char_recycle));
			return;
		}



		hld::cs2dp_proto::save_role_recycle msg;
		msg.set_role_guid(player_ref.get_unit_guid().server_64);
		msg.set_unit_array_index(m_array_index);
		msg.set_save_type_ex(save_type);

		//hld::cs2dp_proto::role_recycle_db test;

		hld::cs2dp_proto::role_recycle_db *db_info = msg.mutable_db_data();

		const s_recycle_info& tmp = m_recycle_obj.get_db_data();

		db_info->set_e_start_time_1(tmp.e_start_time_1);
		db_info->set_e_login_state_2(tmp.e_login_state_2);
		db_info->set_e_prop_state_3(tmp.e_prop_state_3);
		db_info->set_inviter_guid(tmp.inviter_guid.server_64);
		db_info->set_invited_state(tmp.invited_state);



		//test.set_e_start_time_1(tmp.e_start_time_1);
		//test.set_e_login_state_2(tmp.e_login_state_2);
		//test.set_e_prop_state_3(tmp.e_prop_state_3);
		//test.set_inviter_guid(tmp.inviter_guid.server_64);
		//test.set_invited_state(tmp.invited_state);


		//msg.set_allocated_db_data(&test);
		player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_recycle);
		//msg.release_db_data();

	}

	void recycle_mgr::load_recycle_by_db(const s_recycle_info & recycle_data)
	{
		m_recycle_obj.set_db_data(recycle_data);
		load_db_finish();
	}

	bool recycle_mgr::load_recycle_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_recycle_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_recycle_info db_data;
		db_data.e_start_time_1 = msg.e_start_time_1();
		db_data.e_login_state_2 = msg.e_login_state_2();
		db_data.e_prop_state_3 = msg.e_prop_state_3();
		db_data.inviter_guid.server_64 = msg.inviter_guid();
		db_data.invited_state = msg.invited_state();

		load_recycle_by_db(db_data);

		return true;
	}


	void recycle_mgr::save_recycle_task_to_db(int32 save_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_recycle_task);
		if (is_use == false)
		{
			cs2dp_save_char_recycle_task req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;

			int32 data_count = 0;
			for (recycle_task_map_it it = m_recycle_task_data.begin(); it != m_recycle_task_data.end(); it++)
			{
				if (data_count >= MAX_RECYCLE_TASK_NUM)
				{
					break;
				}
				recycle_task &temp_recycle_task = it->second;
				if (temp_recycle_task.get_inst_data(e_recycle_tk_config_id) <= 0)
				{
					continue;
				}
				req.data_list[data_count] = temp_recycle_task.get_recycle_task_info_all();
				data_count++;
			}
			req.data_num = data_count;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
			return;
		}


		hld::cs2dp_proto::save_role_recycle_task msg;

		msg.set_role_guid(player_ref.get_unit_guid().server_64);
		msg.set_unit_array_index(m_array_index);
		msg.set_save_type_ex(save_type);

		hld::cs2dp_proto::role_recycle_task_db *db_info = msg.mutable_db_data();
		if (db_info == nullptr)
		{
			return;
		}
		int32 data_count = 0;
		for (recycle_task_map_it it = m_recycle_task_data.begin(); it != m_recycle_task_data.end(); it++)
		{
			if (data_count >= MAX_RECYCLE_TASK_NUM)
			{
				break;
			}
			recycle_task &temp_recycle_task = it->second;
			if (temp_recycle_task.get_inst_data(e_recycle_tk_config_id) <= 0)
			{
				continue;
			}
			hld::cs2dp_proto::role_recycle_task_row *db_row = db_info->add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			for (int32 i = 0 ; i < e_recycle_tk_max; i ++)
			{
				db_row->add_data_ary(temp_recycle_task.get_inst_data(i));
			}
			data_count++;
		}
		db_info->set_row_count(data_count);
		if (data_count == 0)
		{
			db_info->add_row_data();
		}

		//hld::cs2dp_proto::role_recycle_task_db db_test;

		//db_test.set_row_count(-1);

		//packet_s2s *p_test = parse_msg::getInstance().serialze_buffer(&db_test);
		//std::cout << "----p_test---len----" << p_test->get_google_data_len() << std::endl;

		player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_recycle_task);
	}

	void recycle_mgr::load_recycle_task_by_db(const s_recycle_task_info *data_array, int32 data_num)
	{
		for (int32 i = 0; i < data_num; ++i)
		{
			const s_recycle_task_info& temp_info = data_array[i];
			if (temp_info.data_ary[e_recycle_tk_config_id] <= 0)
			{
				continue;
			}
			create_recycle_task_by_info(temp_info);
		}
		load_db_finish();
	}
	bool  recycle_mgr::load_recycle_task_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_recycle_task_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_recycle_task_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_recycle_task_info *p_row = (s_recycle_task_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::cs2dp_proto::role_recycle_task_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_recycle_task_by_db((const s_recycle_task_info *)p_data, msg.row_count());
		return true;
	}
	void recycle_mgr::create_recycle_task_by_info(const s_recycle_task_info & recycle_task_info)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (m_recycle_task_data.size() >= MAX_RECYCLE_TASK_NUM)
		{
			return;
		}

		recycle_task new_task;
		if (!new_task.init_recycle_task_by_info(recycle_task_info))
		{
			return;
		}

		int32 task_id = new_task.get_inst_data(e_recycle_tk_config_id);
		recycle_task_map_it it = m_recycle_task_data.find(task_id);
		if (it != m_recycle_task_data.end())
		{
			return;
		}
		m_recycle_task_data[task_id] = new_task;
	}

	void recycle_mgr::load_db_finish()
	{
		m_load_num++;
		if (m_load_num != 2)
		{
			return;
		}	
	}
	int32 recycle_mgr::get_config_param(e_recycle_param param_type)
	{
		int32 config_init[e_recycle_param_max] = { 13, 200, 7, 3, 13, 300};

		if (GAMECONFIG->RecycleArray.size() < e_recycle_param_max)
		{
			return config_init[param_type];
		}
		return GAMECONFIG->RecycleArray[param_type];
	}
	int32 recycle_mgr::get_delta_time()
	{
		int32 start_time =  m_recycle_obj.get_start_time();
		if (start_time == 0)
		{
			return -1;
		}
	
		int32 start_days = (start_time  + get_time_zone())/ hld::day_time_second;
		int32 now_days = (time_helper::get_time()+ get_time_zone()) / hld::day_time_second;
		int32 delta = now_days - start_days;
		if (delta < get_config_param(e_recycle_param_days))
		{
			return delta + 1;
		}
		return  -1;
	}

	void recycle_mgr::load_player_time_end_init() 
	{
		if (!is_refresh())
		{
			sync_all_message_to_client();
			return;
		}

		refresh_cycle();
		sync_all_message_to_client();
	}
	void recycle_mgr::gm(int32 timestamp)
	{
		if (timestamp == 0)
		{
			refresh_cycle();
			sync_all_message_to_client();
			return;
		}
		if (timestamp < 0)
		{
			int32 value  = time_helper::get_time() + timestamp * hld::day_time_second;
			m_recycle_obj.set_start_time(value);
			sync_all_message_to_client();
			return;
			
		}
	    m_recycle_obj.set_start_time(timestamp);
		sync_all_message_to_client();
	}
	int32 recycle_mgr::get_time_zone()
	{
		if (m_zone_sec != 0)
		{
			return m_zone_sec;
		}

		tm* now_time = time_helper::get_localtime();
		int64 local_sec = mktime(now_time);

		tm* gm_time = gmtime((const time_t *)&local_sec);
		int32 zero_sec = mktime(gm_time);

		m_zone_sec = local_sec - zero_sec;

		return m_zone_sec;

	}
	bool recycle_mgr::is_refresh()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}
		int32 get_offine_time = temp_player.get_time_data(e_time_type_logout_time);
		if (get_offine_time == 0)
		{
			return false;
		}
		int32 offline_days = (get_offine_time  + get_time_zone()) / hld::day_time_second;
		int32 now_days = (time_helper::get_time() + get_time_zone()) / hld::day_time_second;

		int32 delta = now_days - offline_days;
		if (delta < get_config_param(e_recycle_param_restart_days))
		{
			return false;
		}
		if (get_delta_time() > 0)
		{
			return false;
		}
		if (temp_player.get_unit_info(e_role_info_exp_level) < get_config_param(e_recycle_param_level))
		{
			return false;
		}
		if (globle_data::get_instance().get_server_on_days() < get_config_param(e_recycle_param_open_server))
		{
			return false;

		}
		return true;		 
	}

	void recycle_mgr::refresh_cycle()
	{
		m_recycle_obj.cycle_reset();
		m_recycle_obj.set_start_time(time_helper::get_time());
		m_recycle_task_data.clear();
	}

	void  recycle_mgr::sync_all_message_to_client()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		hld::recycle_proto_recycle_data_all msg_all;

		msg_all.set_start_time(m_recycle_obj.get_start_time());

		for (recycle_task_map_it it = m_recycle_task_data.begin(); it != m_recycle_task_data.end(); it++)
		{
			recycle_task& temp_task = it->second;
			if (!temp_task.get_config_ptr())
			{
				continue;
			}
			recycle_proto_recycle_item_one* one_msg = msg_all.add_task_array();
			one_msg->set_recycle_id(temp_task.get_inst_data(e_recycle_tk_config_id));
			one_msg->set_finish_num(temp_task.get_inst_data(e_recycle_tk_finish_num));
			one_msg->set_state(temp_task.get_inst_data(e_recycle_tk_state));
		}

		std::vector<s_recyle_id_pos> vec_info;
		get_recycle_id_pos_by_type(vec_info, e_recycle_type_login);

	
		for (int32 i = 0; i < vec_info.size(); i++)
		{
			int32 pos = vec_info[i].pos;
			bool flag = m_recycle_obj.get_login_flag_bit(pos);
			recycle_proto_recycle_reward_one * one_login_msg = msg_all.add_login_array();
			one_login_msg->set_recycle_id(vec_info[i].id);
			one_login_msg->set_state(flag ? 1 : 0);
		}

		get_recycle_id_pos_by_type(vec_info, e_recycle_type_buy);

		for (int32 i = 0; i < vec_info.size(); i++)
		{
			int32 pos = vec_info[i].pos;
			bool flag = m_recycle_obj.get_prop_flag_bit(pos);
			recycle_proto_recycle_reward_one * one_prop_msg = msg_all.add_buy_array();
			one_prop_msg->set_recycle_id(vec_info[i].id);
			one_prop_msg->set_state(flag ? 1 : 0);
		}

		get_recycle_id_pos_by_type(vec_info, e_recycle_type_recuit);
		for (int32 i = 0; i < vec_info.size(); i++)
		{
			int32 pos = vec_info[i].pos;
			bool flag = m_recycle_obj.get_invited_level_flag_bit(pos);
			recycle_proto_recycle_reward_one * one_invited_msg = msg_all.add_invited_level_array();
			one_invited_msg->set_recycle_id(vec_info[i].id);
			one_invited_msg->set_state(flag ? 1 : 0);
		}
		msg_all.add_params(time_helper::get_time());
		msg_all.add_params(get_time_zone());
		temp_player.send_message_to_self(&msg_all, e_msgindex_s2c_recycle_data_all);
	}
	void recycle_mgr::sync_one_login_message_to_client(int32 recycle_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		RecycleTemplate * recycle_template_ptr = get_recycle_template_by_id(recycle_id);
		if (recycle_template_ptr == nullptr)
		{
			return;
		}

		hld::recycle_proto_recycle_reward_one one_login_msg;
		one_login_msg.set_recycle_id(recycle_id);

		bool flag = m_recycle_obj.get_login_flag_bit(recycle_template_ptr->Pos);

		one_login_msg.set_state(flag ? 1 : 0);

		temp_player.send_message_to_self(&one_login_msg, e_msgindex_s2c_recycle_login_one);

	}

	void recycle_mgr::sync_one_task_message_to_client(recycle_task & task_info)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		hld::recycle_proto_recycle_item_one one_task_msg;
		one_task_msg.set_recycle_id(task_info.get_inst_data(e_recycle_tk_config_id));
		one_task_msg.set_finish_num(task_info.get_inst_data(e_recycle_tk_finish_num));
		one_task_msg.set_state(task_info.get_inst_data(e_recycle_tk_state));
		temp_player.send_message_to_self(&one_task_msg, e_msgindex_s2c_recycle_item_one);
	}

	void recycle_mgr::sync_one_buy_message_to_client(int32 recycle_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		RecycleTemplate * recycle_template_ptr = get_recycle_template_by_id(recycle_id);
		if (recycle_template_ptr == nullptr)
		{
			return;
		}

		hld::recycle_proto_recycle_reward_one one_buy_msg;
		one_buy_msg.set_recycle_id(recycle_id);

		bool flag = m_recycle_obj.get_prop_flag_bit(recycle_template_ptr->Pos);

		one_buy_msg.set_state(flag ? 1 : 0);

		temp_player.send_message_to_self(&one_buy_msg, e_msgindex_s2c_recycle_buy_one);

	}



	void recycle_mgr::get_recycle_id_pos_by_type(std::vector<s_recyle_id_pos>& out_vec_info, e_recycle_type type)
	{
		out_vec_info.clear();
		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_RecycleTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 id = it->first;
			RecycleTemplate* config_ptr = (RecycleTemplate*)it->second;
			if (config_ptr == nullptr)
			{
				continue;
			}
			if (config_ptr->RecycleType != type)
			{
				continue;
			}
			out_vec_info.push_back(s_recyle_id_pos(id, config_ptr->Pos));
		}
	}
	RecycleTemplate *recycle_mgr::get_recycle_template_by_id(int32 recycle_id)
	{
		return GET_TEMPLATE(RecycleTemplate, recycle_id);

	}
	RecycleTemplate *recycle_mgr::get_recycle_template_by_task_type(e_recycle_task_type task_type)
	{
		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_RecycleTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return nullptr;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 id = it->first;
			RecycleTemplate* config_ptr = (RecycleTemplate*)it->second;
			if (config_ptr == nullptr)
			{
				continue;
			}
			if (config_ptr->RecycleType != e_recycle_type_task)
			{
				continue;
			}
			if (config_ptr->ConditionType == task_type)
			{
				return config_ptr;
			}
		}
		return nullptr;
	}

	RecycleTemplate *recycle_mgr::get_recycle_template_by_map_id(int32 map_id)
	{
		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_RecycleTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return nullptr;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 id = it->first;
			RecycleTemplate* config_ptr = (RecycleTemplate*)it->second;
			if (config_ptr == nullptr)
			{
				continue;
			}
			if (config_ptr->RecycleType != e_recycle_type_task)
			{
				continue;
			}
			if (config_ptr->Pos == map_id)
			{
				return config_ptr;
			}
		}
		return nullptr;
	}
	void recycle_mgr::oper_end_msg(int32 recycle_id, int32 type, int32 result)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		hld::recycle_proto_recycle_operate_end msg;
		msg.set_recycle_id(recycle_id);
		msg.set_oper_type(type);
		msg.set_result(result);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_recycle_oper_end);
	}
	bool recycle_mgr::is_buy(RecycleTemplate *recycle_template_ptr)
	{ 
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return false;
		}
		if (recycle_template_ptr == nullptr)
		{
			return false;
		}
		int32 group_num = recycle_template_ptr->ConditionValue.size() / 2;
		if (group_num == 0 || recycle_template_ptr->ConditionValue.size() % 2 != 0)
		{
			return false;
		}
		for (int32  i = 0; i < group_num; i++)
		{
			int32 money_id = recycle_template_ptr->ConditionValue[i * 2];
			int32 money_num = recycle_template_ptr->ConditionValue[i * 2 + 1];

			if (!temp_player.can_cut_money(e_money_type(money_id), money_num))
			{
				return false;
			}
		}
		return true;
	}
	void recycle_mgr::buy_consume(RecycleTemplate *recycle_template_ptr)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return ;
		}
		if (recycle_template_ptr == nullptr)
		{
			return ;
		}
		int32 group_num = recycle_template_ptr->ConditionValue.size() / 2;
		if (group_num == 0 || recycle_template_ptr->ConditionValue.size() % 2 != 0)
		{
			return ;
		}
		for (int32 i = 0; i < group_num; i++)
		{
			int32 money_id = recycle_template_ptr->ConditionValue[i * 2];
			int32 money_num = recycle_template_ptr->ConditionValue[i * 2 + 1];
			temp_player.cut_money(e_money_type(money_id), money_num, e_sereve_log_cut_money_recycle_buy);
		}
	}
	void recycle_mgr::get_recycle_reward(RecycleTemplate *recycle_template_ptr)
	{
		if (recycle_template_ptr == nullptr)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		if (recycle_template_ptr->RewardItemArray.size() <= 0 || recycle_template_ptr->RewardItemArray.size() % 3 != 0)
		{
			return;
		}

		int32 cur_level = temp_player.get_unit_info(e_role_info_exp_level);
		int32 reward_id = 0;
		for (int32 i = 0; i < recycle_template_ptr->RewardItemArray.size() / 3; ++i)
		{
			if (cur_level >= recycle_template_ptr->RewardItemArray[i * 3] && cur_level <= recycle_template_ptr->RewardItemArray[i * 3 + 1])
			{
				reward_id = recycle_template_ptr->RewardItemArray[i * 3 + 2];
				break;
			}
		}

		if (reward_id <= 0)
		{
			return;
		}

		std::vector<int32> RewardItemArray = init_unit::reward_regional_diff(reward_id);
		std::vector<citem*> item_array;
		std::vector<s_item_template_info> promp_item_data;
		for (int32 i = 0; i < RewardItemArray.size() / 2; i++)
		{
			int32 item_id = RewardItemArray[i * 2];
			int32 item_num = RewardItemArray[i * 2 + 1];

			ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_id);
			if (nullptr == ItemTemplatePtr)
			{
				continue;
			}
			citem* temp_item = temp_player.get_item_set().create_item_by_template(e_server_log_add_item_recycle, 0, item_id, item_num, 1);
			if (temp_item != nullptr)
			{
				item_array.push_back(temp_item);
				promp_item_data.push_back({ item_id ,item_num ,temp_item->get_data_info(e_item_info_locked) });
			}
		}

		temp_player.get_item_set().put_in_bag(item_array);
		if (promp_item_data.size() > 0)
		{
			temp_player.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
		}
	}
	/////////////////////////////////////////////////////////////////////////////

	void recycle_mgr::on_event(e_recycle_task_type task_type, int32 add_num, int32 map_id)
	{
		if (get_delta_time() < 0)
		{
			return;
		}
		if (task_type == e_recycle_task_type_godness_5)
		{
			on_map_event(map_id, add_num);
			return;
		}

		RecycleTemplate *recycle_template_ptr = get_recycle_template_by_task_type(task_type);
		if (recycle_template_ptr == nullptr)
		{
			return;
		}
		event_process(recycle_template_ptr, add_num);
	}
	void recycle_mgr::on_map_event(int32 map_id, int32 add_num)
	{
		RecycleTemplate *recycle_template_ptr =  get_recycle_template_by_map_id(map_id);
		if (recycle_template_ptr == nullptr)
		{
			return;
		}
		event_process(recycle_template_ptr, add_num);
	}

	void recycle_mgr::event_process(RecycleTemplate *recycle_template_ptr, int32 add_num)
	{
		if (recycle_template_ptr == nullptr)
		{
			return;
		}
		recycle_task_map_it it = m_recycle_task_data.find(recycle_template_ptr->attribute_id);
		if (it == m_recycle_task_data.end())
		{
			if (m_recycle_task_data.size() >= MAX_RECYCLE_TASK_NUM)
			{
				return;
			}
			recycle_task new_task;
			if (!new_task.init_recycle_task_by_template(recycle_template_ptr->attribute_id))
			{
				return;
			}
			m_recycle_task_data[recycle_template_ptr->attribute_id] = new_task;
		}
		it = m_recycle_task_data.find(recycle_template_ptr->attribute_id);
		recycle_task& temp_task = it->second;
		if (temp_task.is_finish_num())
		{
			return;
		}
		temp_task.add_finish_num(add_num);
		sync_one_task_message_to_client(temp_task);
	}

	recycle_task&  recycle_mgr::get_recycle_task_by_id(int32 recycle_id)
	{
		recycle_task_map_it it = m_recycle_task_data.find(recycle_id);
		if (it == m_recycle_task_data.end())
		{
			return m_empty_task;
		}
		return  it->second;
	}
	//////////////////////////////////////////////////////////
	RecycleTemplate *recycle_mgr::get_recycle_template_by_level(int32 level)
	{

		template_manager::template_type* tempalte_table_ptr = template_manager::get_instance().get_templates(e_RecycleTemplate);
		if (tempalte_table_ptr == nullptr)
		{
			return nullptr;
		}
		template_manager::template_type::iterator it = tempalte_table_ptr->begin();
		for (; it != tempalte_table_ptr->end(); it++)
		{
			int32 id = it->first;
			RecycleTemplate* config_ptr = (RecycleTemplate*)it->second;
			if (config_ptr == nullptr)
			{
				continue;
			}
			if (config_ptr->RecycleType != e_recycle_type_reward)
			{
				continue;
			}
			if (config_ptr->ConditionValue.size() != 2)
			{
				continue;
			}
			if (config_ptr->ConditionValue[0] <= level && level <= config_ptr->ConditionValue[1])
			{
				return config_ptr;
			}
		}
		return nullptr;
	}

	int64 recycle_mgr::string_to_int64(xstring & s_digit)
	{
		std::stringstream ss;
		ss << s_digit;
		int64 value = 0;
		ss >> value;
		return value;
	}

	void recycle_mgr::on_process_invited_reward(xstring& s_guid)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false || !player_ref.is_self_server())
		{
			return;
		}
		guid_64 target_guid = string_to_int64(s_guid);

		if (m_recycle_obj.is_key_reward())
		{
			send_to_client_msg_by_invited_reward(e_recycle_error_have_get);
			return;
		}
		if (!target_guid.is_valid() || target_guid == player_ref.get_unit_guid())
		{
			send_to_client_msg_by_invited_reward(e_recycle_error_invaild_key);
			return;

		}
		if (m_is_request == true)
		{
			return;
		}

		player& inviter_player_ref = unit_man::get_player(target_guid);
		if (!inviter_player_ref.is_valid())
		{
			m_is_request = true;
			send_to_dp_find_player(target_guid, player_ref.get_unit_guid());
			return;
		}
		get_invited_reward(target_guid);

	}

	void recycle_mgr::send_to_client_msg_by_invited_reward(int32 result)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		hld::recycle_proto_recycle_invited_reward_end msg;
		msg.set_result(result);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_recycle_invited_reward_end);
	}

	///////////////////////////////////////////////////////////////
	void recycle_mgr::send_to_dp_find_player(guid_64 target_guid, guid_64 role_guid)
	{
	
		cs2dp_recycle_get_inviter_info msg;
		msg.target_guid = target_guid;
		msg.role_guid = role_guid;
		connection_mgr::getInstance().send_to_dp(&msg, sizeof(cs2dp_recycle_get_inviter_info), 0);

	}

	void recycle_mgr::send_to_dp_find_player_end(const s_recycle_inviter_base_info_db& data_info)
	{
		m_is_request = false;
		if (!data_info.inviter_guid.is_valid())
		{
			send_to_client_msg_by_invited_reward(e_recycle_error_invaild_key);
			return;
		}
		get_invited_reward(data_info.inviter_guid);
	}

	void recycle_mgr::get_invited_reward(guid_64 target_guid)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false || !player_ref.is_self_server())
		{
			return;
		}

		int32 my_level = player_ref.get_unit_info(e_role_info_exp_level);

		RecycleTemplate *recycle_template_ptr = get_recycle_template_by_level(my_level);
		if (recycle_template_ptr == nullptr)
		{
			return;
		}

		get_recycle_reward(recycle_template_ptr);

		m_recycle_obj.set_key_reward(target_guid);

		send_to_client_msg_by_invited_reward(e_recycle_error_sucess);

		save_to_dp_add_invited(target_guid, player_ref.get_unit_guid());

		player& inviter_player_ref = unit_man::get_player(target_guid);
		if (!inviter_player_ref.is_valid() ||  inviter_player_ref.get_session_state() != e_session_status_in_gaming)
		{
			return;
		}

		int32 class_type = player_ref.get_unit_info(e_role_info_class_type);

		s_recycle_invited_info new_invited(player_ref.get_unit_guid(), my_level, player_ref.get_unit_info(e_role_info_template_id), class_type, player_ref.get_name());

		inviter_player_ref.get_recycle_mgr().add_invited_info(new_invited);
	}

	void recycle_mgr::save_to_dp_add_invited(guid_64 target_guid, guid_64 invited_guid)
	{
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_recycle_inviter_add);
		if (is_use == false)
		{
			cs2dp_save_char_recycle_invited msg;
			msg.target_guid = target_guid;
			msg.role_guid = invited_guid;
			connection_mgr::getInstance().send_to_dp(&msg, sizeof(cs2dp_save_char_recycle_invited), 0);
		}
		else
		{
			hld::cs2dp_proto::save_recycle_invited msg;
			msg.set_role_guid(invited_guid.server_64);
			msg.set_target_guid(target_guid.server_64);
			connection_mgr::getInstance().send_to_dp_lua(&msg, e_msgindex_cs2dp_recycle_inviter_add, 0);
		}

	}

	//////////////////////////////////////////////////////////////////
	void recycle_mgr::sync_all_invited_list()
	{
		m_recycle_invited_obj.sync_all_invited_list();
	}

	void recycle_mgr::load_recycle_invited_by_db(const s_recycle_invited_info *data_array, int32 data_num)
	{
		for (int32 i = 0; i < data_num; ++i)
		{
			m_recycle_invited_obj.add(data_array[i]);
		}
		sync_all_invited_list();
	}
	bool recycle_mgr::load_recycle_invited_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::db_proto::role_proc_recycle_invited_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_recycle_invited_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_recycle_invited_info *p_row = (s_recycle_invited_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::db_proto::role_proc_recycle_invited_row db_row = msg.row_data(i);
			p_row->role_guid.server_64 = db_row.role_guid();
			p_row->level = db_row.level();
			p_row->head_id = db_row.head_id();
			p_row->class_type = db_row.class_type();
			parse_msg::getInstance().my_memcopy_string(p_row->invited_name, max_name_size, db_row.invited_name());
			p_row++;
		}
		load_recycle_invited_by_db((const s_recycle_invited_info *)p_data, msg.row_count());
		return true;

	}

	void recycle_mgr::add_invited_info(s_recycle_invited_info & new_invited_info)
	{
		m_recycle_invited_obj.add(new_invited_info, true);
	}
	void recycle_mgr::sync_one_invited_message_to_client(int32 recycle_id)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}

		RecycleTemplate * recycle_template_ptr = get_recycle_template_by_id(recycle_id);
		if (recycle_template_ptr == nullptr)
		{
			return;
		}

		hld::recycle_proto_recycle_reward_one one_invited_msg;
		one_invited_msg.set_recycle_id(recycle_id);

		bool flag = m_recycle_obj.get_invited_level_flag_bit(recycle_template_ptr->Pos);

		one_invited_msg.set_state(flag ? 1 : 0);

		temp_player.send_message_to_self(&one_invited_msg, e_msgindex_s2c_recycle_invited_level_one);

	}

	/////////////////////////////////////////////
 	void recycle_mgr::tick()
	{  
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		int32 level = temp_player.get_unit_info(e_role_info_exp_level);
		int32 head = temp_player.get_unit_info(e_role_info_template_id);
		int32 class_type = temp_player.get_unit_info(e_role_info_class_type);

		recycle_man::get_instance().update_level_info(temp_player.get_unit_guid(),level, head, class_type);

		m_recycle_invited_obj.tick();

	}



	//////////////////////////////////////////
	recycle_man::recycle_man()
	{
		m_level_update_data.clear();


	}
	recycle_man::~recycle_man()
	{
		m_level_update_data.clear();

	}
	recycle_man& recycle_man::get_instance()
	{
		static  recycle_man s_rm;

		return s_rm;
	}
	
	void recycle_man::update_level_info(guid_64 guid, int32 in_level, int32 in_head, int32 class_type)
	{	 
		level_update_map_it  it = m_level_update_data.find(guid);
		if (it != m_level_update_data.end())
		{
			s_recycle_update_info& level_info = it->second;
			level_info.m_level = in_level;
			level_info.m_head = in_head;
			level_info.m_class_type = class_type;
			return;
		}
		m_level_update_data[guid] = s_recycle_update_info(in_level, in_head, class_type);
	}

	s_recycle_update_info& recycle_man::get_level_info(guid_64 guid)
	{
		level_update_map_it it = m_level_update_data.find(guid);
		if (it != m_level_update_data.end())
		{
			return it->second;
		}
		return empty_data;
	}



}