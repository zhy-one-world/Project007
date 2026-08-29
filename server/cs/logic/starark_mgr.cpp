#include "internal/char_msg.hpp"
#include "internet/cs2dp.pb.h"
#include "internet/net.pb.h"
#include "internet/starark.pb.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "starark_mgr.h"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"
#include "utility/random.h"



namespace faith
{
	/////////////////////////////////////////////////////////////
	my_iterator::my_iterator(int32 step, s_starark_memory_info *p_array)
	{
		get_area(step, m_start, m_end);
		m_array = p_array;
	}

	void my_iterator::get_area(int32 step, int32& start, int32& end)
	{
		static int32 distribute[starark_max_step_num] = { 4,4,3,2,2,1 };
		start = 0;
		end = 0;
		if (step > starark_max_step_num)
		{
			return;
		}
		for (int32 i = 0; i < step; i++)
		{
			if (i - 1 >= 0)
			{
				start += distribute[i - 1];
			}
			end += distribute[i];
		}
	}
	/////////////////////////////////////////////////////////////


	starark_mgr::starark_mgr()
	{

	}
	starark_mgr::~starark_mgr()
	{

	}

	void starark_mgr::clear_data()
	{
		m_data.reset();
		memset((void *)m_round_ary, 0, sizeof(s_starark_memory_info) * starark_round_max);
	}

	void starark_mgr::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	void starark_mgr::save_starark_to_db(int32 save_type)
	{

		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		info_to_string();

		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_role_starark_to_db);
		if (is_use == false)
		{
			cs2dp_save_role_starark_to_db req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_array_index;

			//info_to_string();
			req.starark_info = m_data;
			player_ref.send_message_to_dp(&req, sizeof(cs2dp_save_role_starark_to_db));

		}
		else
		{

			cs2dp_proto::save_role_starark msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type);
			db_proto::role_starark_db* db_info = msg.mutable_db_data();

			for (int32 i = 0; i < e_starark_max; i++)
			{
				db_info->add_data_ary(m_data.data_ary[i]);
			}
			db_info->set_body(m_data.body);

			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_role_starark_to_db);
		}
	}
	void starark_mgr::info_to_string()
	{
		std::stringstream buff;
		xstring ret = "";

		m_data.data_ary[e_starark_head] = 0;
		for (int32 i = 0; i < starark_round_max; i++)
		{
			if (m_round_ary[i].item_id == 0)
			{
				continue;
			}
			buff << m_round_ary[i].item_id << ',';
			buff << m_round_ary[i].item_num << ',';
			buff << m_round_ary[i].state << ',';
			m_data.data_ary[e_starark_head] |= (1 << i);
		}
		buff >> ret;
		int32  copy_len = ret.length() > starark_max_db_len ? starark_max_db_len : ret.length();
		memset(m_data.body, 0, starark_max_db_len);
		memcpy(m_data.body, ret.c_str(), copy_len);
	}
	void starark_mgr::string_to_info()
	{
		std::vector<int32> tmp_vec;
		xchar *p_start = m_data.body;
		xchar *p_end = p_start;
		while (*p_end != '\0')
		{
			if (*p_end == ',')
			{
				xstring temp(p_start, p_end - p_start);
				int32 res = init_unit::change_string_to_i32(temp);
				tmp_vec.push_back(res);
				p_start = p_end + 1;
			}
			p_end++;
		}
		int32 pos = 0;
		for (int32 i = 0; i < starark_round_max; i++)
		{
			if ((m_data.data_ary[e_starark_head] & (1 << i)) != 0)
			{
				if (tmp_vec.size() > (pos + 2))
				{
					m_round_ary[i].item_id = tmp_vec[pos++];
					m_round_ary[i].item_num = tmp_vec[pos++];
					m_round_ary[i].state = tmp_vec[pos++];
				}
			}
		}
	}


	void starark_mgr::load_starark_by_db(const s_starark_info & data_info)
	{
		m_data = data_info;
		string_to_info();

		sync_all_message_to_client();
	}


	bool starark_mgr::load_starark_by_proto_db(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}

		s_starark_info starark_info;

		db_proto_role_starark_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			starark_info.data_ary[i] = msg.data_ary(i);
		}
		int32 len = msg.body().size() > starark_max_db_len ? starark_max_db_len : msg.body().size();
		memcpy(starark_info.body, msg.body().c_str(), len);

		load_starark_by_db(starark_info);

		return true;
	}
	void starark_mgr::sync_all_message_to_client()
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		starark_proto::starark_all msg_all;

		for (int32 i = e_starark_step; i < e_starark_max; i++)
		{
			msg_all.add_data_ary(get_data(i));
		}

		for (int32 i = 0; i < starark_round_max; i++)
		{
			s_starark_memory_info *tmp = m_round_ary + i;
			msg_all.add_round_ary(tmp->item_id);
			msg_all.add_round_ary(tmp->item_num);
		    msg_all.add_round_ary(tmp->state);
		}
		temp_player.send_message_to_self(&msg_all, e_msgindex_s2c_starark_all);
	}

	int32 starark_mgr::get_data(int32 idx) const
	{
		if (idx < e_starark_step || idx > e_starark_max)
		{
			return -1;
		}
		return m_data.data_ary[idx];
	}

	void starark_mgr::set_data(int32 idx, int32 value)
	{
		if (idx < e_starark_step || idx > e_starark_max)
		{
			return;
		}
		m_data.data_ary[idx] = value;
	}
	void starark_mgr::set_round_data(int32 idx, bool is_add, int32 item_id , int32 item_num)
	{
		if (idx < 0 || idx >= starark_round_max)
		{
			return;
		}
		if (is_add)
		{
			m_round_ary[idx].item_id = item_id;
			m_round_ary[idx].item_num = item_num;
			m_round_ary[idx].state = 0;

			int32 flag = get_data(e_starark_head);

			flag |= (1 << idx);

			set_data(e_starark_head, flag);
			
		}
		else
		{
			m_round_ary[idx].reset();

			int32 flag = get_data(e_starark_head);

			flag &= ~(1 << idx);

			set_data(e_starark_head, flag);

		}

	}

	void starark_mgr::on_operate(int32 oper_type, int32 param1, int32 param2, int32 param3)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (temp_player.is_valid() == false)
		{
			return;
		}
		if (!temp_player.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_starark))
		{
			return;
		}
		switch (oper_type)
		{
		case e_starark_oper_load:
			 on_load(oper_type, param1, param2,param3);
			 break;
		case e_starark_oper_unload:
			 on_unload(oper_type, param1, param2, param3);
			 break;
		case e_starark_oper_change:
			 on_change(oper_type);
			 break;
		case e_starark_oper_buy:
			 on_buy(oper_type, param1);
			 break;
		case e_starark_oper_round:
			 on_round(oper_type);
			break;
		case e_starark_oper_five_round:
			on_five_round(oper_type);
			break;
		default:
			break;
		}
	}

	void starark_mgr::oper_msg(int32 oper_type, int32 result, int32 pos /* = 0 */, int32 num /* = 0 */)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		starark_proto_starark_operate_end end_msg;

		end_msg.set_oper_type(oper_type);
		end_msg.set_result(result);
		end_msg.set_pos(pos);
		end_msg.set_num(num);
		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_starark_operate_end);
	}

	void starark_mgr::on_load(int32 oper_type, int32 step, int32 item_id, int32 step_pos)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (step < 1 || step > 6 || step_pos < 0)
		{
			return; 
		}
		ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_id);
		if (nullptr == ItemTemplatePtr)
		{
			return;
		}
		int32 item_num = 0;

		get_act_item_by_step(step, item_id, item_num);

		if (item_num == 0)
		{
			return;
		}
		my_iterator my_iter(step, get_round());

		///////////////////////////////////////////

		bool is_exist = false;

		for (my_iter.begin(); my_iter.end(); my_iter++)
		{
			s_starark_memory_info *tmp = my_iter.get();
			if (tmp->item_id == item_id)
			{
				is_exist = true;
				break;
			}
		}
		if (is_exist == true)
		{
			oper_msg(oper_type, starark_error_is_exist);
			return;
		}

		int32 offset = my_iter.m_start + step_pos;
		if (offset >= my_iter.m_end)
		{
			return;
		}

		if (m_round_ary[offset].item_id != 0)
		{
			return;
		}

		set_round_data(offset, true, item_id, item_num);

		sync_all_message_to_client();

		oper_msg(oper_type, starark_error_sucess);
	}

	void starark_mgr::on_unload(int32 oper_type, int32 step, int32 item_id, int32 step_pos)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (step < 1 || step > 6 || step_pos < 0)
		{
			return;
		}
		my_iterator my_iter(step, get_round());

		int32 offset = my_iter.m_start + step_pos;
		if (offset >= my_iter.m_end)
		{
			return;
		}

		if (m_round_ary[offset].item_id != item_id)
		{
			return;
		}

		set_round_data(offset, false);

		sync_all_message_to_client();

		oper_msg(oper_type, starark_error_sucess);

	}
	void starark_mgr::on_change(int32 oper_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 state = get_data(e_starark_step);
		int32 flag = get_data(e_starark_head);

		if (state == 0)
		{
			if (flag != 0xffff)
			{
				oper_msg(oper_type, starark_error_not_full);
				return;
			}

			set_data(e_starark_step, 1);
			sync_all_message_to_client();


			oper_msg(oper_type, starark_error_sucess);

			return;
		}

		restart();

		oper_msg(oper_type, starark_error_sucess);




	}

	void starark_mgr::on_buy(int32 oper_type, int32 num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (num <= 0)
		{
			return;
		}

		const std::vector<int32>* p_conifg = get_act_cost_array();
		if (p_conifg == nullptr)
		{
			return;
		}
		int32 dst_item_id = (*p_conifg)[0];
		int32 money_type = (*p_conifg)[1];
		int32 money_num = (*p_conifg)[2];

		int32 total_num = money_num * num;

		if (!player_ref.can_cut_money((e_money_type)money_type, total_num))
		{
			oper_msg(oper_type, starark_error_not_money);
			return;
		}

		int32 need_slot_num = 0;
		int32 total_item_num = num;
		need_slot_num = total_item_num;
		ItemTemplate* temp_item_ptr = GET_TEMPLATE(ItemTemplate, dst_item_id);
		if (temp_item_ptr == nullptr)
		{
			oper_msg(oper_type, starark_error_unknown);
			return;
		}
		if (temp_item_ptr->max_pile_num != 0)
		{
			need_slot_num = total_item_num / (temp_item_ptr->max_pile_num);
			if (total_item_num % temp_item_ptr->max_pile_num != 0)
			{
				need_slot_num += 1;
			}
		}
		e_bag_type need_bag = init_unit::get_item_bag_type(dst_item_id);
		if (player_ref.get_item_set().is_empty_slot_enough(need_bag, need_slot_num) == false)
		{
			oper_msg(oper_type, starark_error_bag_full);
			return;
		}

		player_ref.cut_money((e_money_type)money_type, total_num, e_server_log_cut_money_starark_buy_item);

		vector<s_item_template_info> promp_item_data;

		citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_starark, 0, dst_item_id, num, 1);
		if (temp_item != nullptr)
		{
			player_ref.get_item_set().put_in_bag(temp_item);

			promp_item_data.push_back({ dst_item_id,num,temp_item->get_data_info(e_item_info_locked) });
			player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
		}
		oper_msg(oper_type, starark_error_sucess);

	}

	void starark_mgr::on_round(int32 oper_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (get_data(e_starark_step) == 0)
		{
			return;
		}
		const std::vector<int32> *p_config = get_act_all_weight();
		if (p_config == nullptr)
		{
			return;
		}

		int32 item_id = 0, item_num = 0;
		int32 cur_times = get_data(e_starark_times);

		if (!get_act_need_item_num(cur_times, item_id, item_num))
		{
			oper_msg(oper_type, starark_error_times_max);
			return;
		}

		if (item_system::can_cost_item(&player_ref, e_bag_type_bag, item_id, item_num) == false)
		{
			oper_msg(oper_type, starark_error_not_item);
			return;

		}

		int32 cur_num_array[starark_max_step_num] = { 0 };
		get_per_area_num(cur_num_array);

		if (cur_times <= 7)
		{
			cur_num_array[starark_max_step_num - 1] = 0;
		}



		int32 step = get_step(p_config, cur_num_array);
		if (step == -1)
		{
			return;
		}
		int32 pos =  get_step_offset(step, cur_num_array[step]);
		if (pos == -1)
		{
			return;
		}
		int32 times = get_step_times(p_config, step);
		if (times == -1)
		{
			return;
		}

		item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, item_id, item_num);


		m_round_ary[pos].state = times;

		cur_times++;
		set_data(e_starark_last_id, pos);
		set_data(e_starark_times, cur_times);


		vector<int32> promp_item_data;

		int32 result_id = m_round_ary[pos].item_id;
		int32 result_num = m_round_ary[pos].item_num * times;

		//std::cout << "-----item_id----" << m_round_ary[pos].item_id << "--item_num--" << m_round_ary[pos].item_num << "----times----" << times << std::endl;
		citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_starark, 0, result_id, result_num, 1);
		if (temp_item != nullptr)
		{
			player_ref.get_item_set().put_in_bag(temp_item);

			promp_item_data.push_back(result_id);
			promp_item_data.push_back(result_num);
			promp_item_data.push_back(temp_item->get_data_info(e_item_info_locked));
			//player_ref.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
		}

	
		sync_all_message_to_client();
		oper_msg(oper_type, starark_error_sucess, pos, times);

	}

	void starark_mgr::on_five_round(int32 oper_type)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (get_data(e_starark_step) == 0)
		{
			return;
		}
		const std::vector<int32> *p_config = get_act_all_weight();
		if (p_config == nullptr)
		{
			return;
		}

		int32 item_id = 0, item_num = 0;
		int32 cur_times = get_data(e_starark_times);

		if (!get_act_need_five_item_num(cur_times, item_id, item_num))
		{
			oper_msg(oper_type, starark_error_times_max);
			return;
		}

		if (item_system::can_cost_item(&player_ref, e_bag_type_bag, item_id, item_num) == false)
		{
			oper_msg(oper_type, starark_error_not_item);
			return;
		}

		item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, item_id, item_num);

	
		starark_proto::starark_five_round_rewad_list reward_msg;

		int32 pos = 0;
		int32 times = 0;
		for (int32 i = 0; i < 5; ++i)
		{
			if (cur_times < starark_round_max)
			{
				int32 cur_num_array[starark_max_step_num] = { 0 };
				get_per_area_num(cur_num_array);

				if (cur_times <= 7)
				{
					cur_num_array[starark_max_step_num - 1] = 0;
				}

				int32 step = get_step(p_config, cur_num_array);
				if (step == -1)
				{
					continue;
				}

				pos = get_step_offset(step, cur_num_array[step]);
				if (pos == -1)
				{
					continue;
				}

				times = get_step_times(p_config, step);
				if (times == -1)
				{
					continue;
				}

				m_round_ary[pos].state = times;

				cur_times++;
				set_data(e_starark_last_id, pos);
				set_data(e_starark_times, cur_times);

				int32 result_id = m_round_ary[pos].item_id;
				int32 result_num = m_round_ary[pos].item_num * times;

				citem* temp_item = player_ref.get_item_set().create_item_by_template(e_server_log_add_item_starark, 0, result_id, result_num, 1);
				if (temp_item != nullptr)
				{
					player_ref.get_item_set().put_in_bag(temp_item);
					starark_proto::starark_rewad_item* item_info = reward_msg.add_item_list();
					if (nullptr == item_info)
					{
						continue;
					}
				
					item_info->set_item_id(result_id);
					item_info->set_item_num(result_num);
				}
			}
		}

		player_ref.send_message_to_self(&reward_msg, e_msgindex_s2c_starark_five_round_rewad_list);

		sync_all_message_to_client();
		oper_msg(oper_type, starark_error_sucess, pos, times);
	}


	void starark_mgr::get_act_item_by_step(int32 step, int32 item_id, int32 &item_num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		time_limit_activity_object& starark_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_starark);
		if (starark_act.is_valid() == false)
		{
			return;
		}

		const s_time_limit_activity_branch_temp& branch_template_ptr = starark_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return;
		}
		const std::vector<int32> *p_vec = nullptr;

		switch (step)
		{
		case e_starark_step_1:
		case e_starark_step_2:
			p_vec = &branch_template_ptr.ParamArr1;
			break;
		case e_starark_step_3:
		case e_starark_step_4:
			p_vec = &branch_template_ptr.ParamArr2;
			break;
		case e_starark_step_5:
		case e_starark_step_6:
			p_vec = &branch_template_ptr.ParamArr3;
			break;
		default:
			break;
		}
		if (p_vec == nullptr)
		{
			return;
		}
		if (p_vec->size() % e_starark_elem_max != 0)
		{
			return;
		}

		for (int32 i =  0 ; i <  p_vec->size() ; i += e_starark_elem_max)
		{
			if ((*p_vec)[i + e_starark_elem_item_id] == item_id && (*p_vec)[i + e_starark_elem_step_id] == step)
			{
				item_num = (*p_vec)[i + e_starark_elem_item_num];
			}
		}
	}


	const std::vector<int32> * starark_mgr::get_act_all_weight()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		time_limit_activity_object& starark_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_starark);
		if (starark_act.is_valid() == false)
		{
			return nullptr;
		}
		const s_time_limit_activity_branch_temp& branch_template_ptr = starark_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return nullptr;
		}
		const std::vector<int32> *p_vec = nullptr;

		p_vec = &branch_template_ptr.ParamArr5;

		if (p_vec->size() != starark_max_step_num * e_starark_weight_max)
		{
			return nullptr;
		}
		return p_vec;
	}
	const std::vector<int32> * starark_mgr::get_act_cost_array()
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		time_limit_activity_object& starark_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_starark);
		if (starark_act.is_valid() == false)
		{
			return nullptr;
		}
		const s_time_limit_activity_branch_temp& branch_template_ptr = starark_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return nullptr;
		}
		const std::vector<int32> *p_vec = nullptr;

		p_vec = &branch_template_ptr.Condition;

		if (p_vec->size() != 3)
		{
			return nullptr;
		}
		return p_vec;
	}
	bool starark_mgr::get_act_need_item_num(int32 times, int32& need_item_id, int32& need_item_num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		time_limit_activity_object& starark_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_starark);
		if (starark_act.is_valid() == false)
		{
			return false;
		}
		const s_time_limit_activity_branch_temp& branch_template_ptr = starark_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return false;
		}
		if (times < 0 || times >= starark_round_max)
		{
			return false;
		}
		if (branch_template_ptr.Condition.size() != 3 || branch_template_ptr.Reward.size() != starark_round_max)
		{
			return false; 
		}

		need_item_id = branch_template_ptr.Condition[0];

		need_item_num = branch_template_ptr.Reward[times];

		return true;
	}

	bool starark_mgr::get_act_need_five_item_num(int32 times, int32 & need_item_id, int32 & need_item_num)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		time_limit_activity_object& starark_act = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_starark);
		if (starark_act.is_valid() == false)
		{
			return false;
		}
		const s_time_limit_activity_branch_temp& branch_template_ptr = starark_act.get_time_limit_activity_branch_temp();
		if (branch_template_ptr.ActGuid < 0)
		{
			return false;
		}

		if (times < 0 || times >= starark_round_max)
		{
			return false;
		}

		if (branch_template_ptr.Condition.size() != 3 || branch_template_ptr.Reward.size() != starark_round_max)
		{
			return false;
		}

		need_item_id = branch_template_ptr.Condition[0];

		for (int32 i = 0; i < 5; ++i)
		{
			if (times + i < starark_round_max)
			{
				need_item_num += branch_template_ptr.Reward[times + i];
			}		
		}

		return true;
	}

	void starark_mgr::get_area(int32 step, int32& start, int32& end)
	{

		static int32 distribute[starark_max_step_num] = { 4,4,3,2,2,1};
		start = 0;
		end = 0;
		if (step > starark_max_step_num)
		{
			return;
		}
		for (int32 i = 0; i < step; i++)
		{
			if (i - 1 >= 0)
			{
				start += distribute[i - 1];
			}
			end += distribute[i];
		}
	}
	void starark_mgr::get_per_area_num(int32 cur_num_array[starark_max_step_num])
	{
		for (int32 i = 0; i < starark_max_step_num; i++)
		{
			my_iterator my_iter(i + 1, get_round());

			for (my_iter.begin(); my_iter.end(); my_iter++)
			{
				s_starark_memory_info *temp = my_iter.get();
				if (temp->state != 0)
				{
					continue;
				}
				cur_num_array[i]++;
			}
		}
	}

	int32 starark_mgr::get_step(const std::vector<int32> *p_config, int32 cur_num_array[starark_max_step_num])
	{
		if (p_config == nullptr)
		{
			return 0;
		}
		int32 total_weight = 0;
		for (int32 i = 0; i < starark_max_step_num; i++)
		{
			if (cur_num_array[i] != 0)
			{
				total_weight += (*p_config)[i * e_starark_weight_max + e_starark_weight_base];
			}
		}
		int32  random_value = random_gen::get_random(0, total_weight);

		int32 cur_weight = 0;
		for (int32 i = 0; i < starark_max_step_num; i++)
		{
			if (cur_num_array[i] == 0)
			{
				continue;
			}
			cur_weight += (*p_config)[i * e_starark_weight_max + e_starark_weight_base];
			if (cur_weight >= random_value)
			{
				return i;
			}
		}
		return -1;
	}
	int32 starark_mgr::get_step_offset(int32 step, int32 num)
	{
		int32 pos = random_gen::get_random(1, num);
		int32 count = 0;
		my_iterator my_iter(step + 1, get_round());
		for (my_iter.begin(); my_iter.end(); ++my_iter)
		{
			s_starark_memory_info *temp = my_iter.get();
			if (temp->state != 0)
			{
				continue;
			}
			count++;
			if (count == pos)
			{
				return my_iter.get_idx();
			}
		}
		return -1;
	}
	int32 starark_mgr::get_step_times(const std::vector<int32> *p_config,int32 step)
	{
		if (p_config == nullptr)
		{
			return 0;
		}
		int32 total_weight = 0;
		for (int32 i = e_starark_weight_1; i < e_starark_weight_max; i++)
		{
			total_weight += (*p_config)[step * e_starark_weight_max + i];
		}
		int32  random_value = random_gen::get_random(0, total_weight);

		int32 cur_weight = 0;

		for (int32 i = e_starark_weight_1; i < e_starark_weight_max; i++)
		{
			cur_weight += (*p_config)[step * e_starark_weight_max + i];
			if (cur_weight >= random_value)
			{
				return i - 1;
			}
		}
		return -1;	
	}

	void starark_mgr::restart()
	{

		m_data.reset();

		m_data.data_ary[e_starark_head] = 0xFFFF;

		for (int32 i = 0; i < starark_round_max; i++)
		{
			m_round_ary[i].state = 0;
		}

		sync_all_message_to_client();

	}

	void starark_mgr::refresh_recycle()
	{
		
		clear_data();

		sync_all_message_to_client();

		oper_msg(e_starark_oper_refresh, starark_error_sucess);
	}
}