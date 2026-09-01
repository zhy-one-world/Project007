/********************************************************************
created: 2017年8月10日11:27:00
file base: service_goal_def
file ext: cpp
author: 韩泽成
purpose: 七日目标实现文件
*********************************************************************/

#include "connection_mgr.hpp"
#include "internet/cs2dp.pb.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "service_goal.hpp"
#include "service_goal_mgr.hpp"
#include "internal/service_goal_msg.hpp"
#include "template/template_manager.h"
#include "internet/service_goal.pb.h"
#include "internet/net.pb.h"
#include "utility/parse_msg.h"

namespace faith
{
	
	cservice_goal_mgr::cservice_goal_mgr()
	{
		m_player_index = 0;
		clear_data();
	}

	cservice_goal_mgr::~cservice_goal_mgr()
	{

	}

	void cservice_goal_mgr::clear_data()
	{
		for (int32 service_goal_index = 0; service_goal_index < MAX_SERVICE_GOAL_NUM; service_goal_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(service_goal_index);
			temp_service_goal_obj.clear_data();
		}
		memset(m_service_goal_reward_days, 0, sizeof(m_service_goal_reward_days));
		m_service_goal_obj_num = 0;
		m_empty_goal_obj.clear_data();
		m_service_goal_max_over_time = 0;
	}

	void cservice_goal_mgr::set_player_index(int32 player_index)
	{
		m_player_index = player_index;
		for (int32 service_goal_index = 0; service_goal_index < MAX_SERVICE_GOAL_NUM; service_goal_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(service_goal_index);
			temp_service_goal_obj.set_player_index(player_index);
		}
	}

	void cservice_goal_mgr::send_all_service_goal()
	{
		player& temp_player = get_player();
		if (!temp_player.is_valid())
		{
			return;
		}
		service_goal_proto_service_goal_all service_goal_msg_all;
		service_goal_msg_all.add_role_guid(temp_player.get_unit_guid().A);
		service_goal_msg_all.add_role_guid(temp_player.get_unit_guid().B);
		for (int32 service_goal_index = 0; service_goal_index < m_service_goal_obj_num; service_goal_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(service_goal_index);
			/*if (false == temp_service_goal_obj.has_service_goal_activated())
			{
				continue;
			}*/
			service_goal_proto_service_goal_one* service_goal_one_ptr = service_goal_msg_all.add_service_goal_array();
			temp_service_goal_obj.make_service_goal_one_proto(service_goal_one_ptr);

			if (service_goal_msg_all.ByteSize() > packege_length_max)
			{
				send_message_to_self(&service_goal_msg_all, e_msgindex_s2c_service_goal_all);
				service_goal_msg_all.clear_service_goal_array();
			}
		}
		if (service_goal_msg_all.service_goal_array_size() > 0)
		{
			send_message_to_self(&service_goal_msg_all, faith::e_msgindex_s2c_service_goal_all);
		}
	}

	void cservice_goal_mgr::receive_service_goal(int32 service_goal_id)
	{
		//超过了最长时间就不能领取了
		//int32 time_now = time_helper::get_cur_time_new().second;
		//if (time_now > m_service_goal_max_over_time)
		//{
		//	return;
		//}
		cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_id(service_goal_id);
		temp_service_goal_obj.received_service_goal();
		refresh_service_goal_received_reward_days();
		refresh_service_goal(e_service_goal_type_big_reward);
		refresh_service_goal(e_service_goal_type_get_all_reward);
		refresh_service_goal(e_service_goal_type_final);
		refresh_service_goal(e_service_goal_type_no_limit_time_final);
		check_over();
	}

	void cservice_goal_mgr::refresh_service_goal_received_reward_days()
	{
		clear_service_goal_reward_days();
		for (int32 array_index = 0; array_index < m_service_goal_obj_num; array_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(array_index);
			if (true == temp_service_goal_obj.is_received())
			{
				add_service_goal_reward_days(temp_service_goal_obj.get_service_goal_active_data(), temp_service_goal_obj.get_service_goal_active_type());
			}
		}
	}

	void cservice_goal_mgr::add_service_goal_reward_days(const int32 reward_day, const e_service_goal_active_type active_type)
	{
   		if (0 >= reward_day 
			|| MAX_REWARD_DAYS < reward_day
			|| e_service_goal_active_type_max <= active_type)
		{
			return;
		}
		m_service_goal_reward_days[reward_day][active_type]++;
	}

	void cservice_goal_mgr::clear_service_goal_reward_days()
	{
		memset(m_service_goal_reward_days, 0, sizeof(m_service_goal_reward_days));
	}

	void cservice_goal_mgr::load_service_goal_by_db(const s_service_goal_info* service_goal_data, const int32 service_goal_num)
	{
		if (nullptr == service_goal_data)
		{
			return;
		}
		template_manager::template_type* ladder_tmpl_table = template_manager::get_instance().get_templates(e_ServiceGoalTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return;
		}

		template_manager::template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			ServiceGoalTemplate* temp_service_goal_template_ptr = (ServiceGoalTemplate*)(ite->second);
			if (nullptr == temp_service_goal_template_ptr || temp_service_goal_template_ptr->ActiveType == e_service_goal_active_type_seven_day
				|| temp_service_goal_template_ptr->ConditionType == e_service_goal_type_seven_day_final)
			{
				continue;
			}

			s_service_goal_info new_service_goal_info;
			new_service_goal_info.data_ary[e_service_goal_inst_data_id] = (temp_service_goal_template_ptr->attribute_id);
			for (int32 i = 0; i < service_goal_num; i++)
			{
				if (service_goal_data[i].data_ary[e_service_goal_inst_data_id] == temp_service_goal_template_ptr->attribute_id)
				{
					new_service_goal_info = service_goal_data[i];
					break;
				}
			}
			create_service_goal_obj(temp_service_goal_template_ptr, new_service_goal_info);
		}
		// 接下来发送给客户端
		send_all_service_goal();
	}
	bool cservice_goal_mgr::load_service_goal_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_service_goal_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_service_goal_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_service_goal_info *p_row = (s_service_goal_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_service_goal_row db_row = msg.row_data(i);
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_service_goal_by_db((const s_service_goal_info *)p_data, msg.row_count());
		return true;

	}

	void cservice_goal_mgr::save_service_goal_to_db(const int32 save_type)
	{
		player& player_ref = get_player();
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_service_goal);
		if (is_use == false)
		{
			cs2dp_save_char_service_goal req;
			req.save_type_ex = save_type;
			req.role_guid = player_ref.get_unit_guid();
			req.unit_array_index = m_player_index;
			int32 data_num = 0;
			for (int32 service_goal_index = 0; service_goal_index < m_service_goal_obj_num; service_goal_index++)
			{
				cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(service_goal_index);
				req.data_list[data_num] = temp_service_goal_obj.get_service_goal_info();
				++data_num;
			}
			req.data_num = data_num;
			player_ref.send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			faith::cs2dp_proto::save_role_service_goal msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_index);
			msg.set_save_type_ex(save_type);
			faith::cs2dp_proto::role_service_goal_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			int32 row_count = 0;
			for (int32 service_goal_index = 0; service_goal_index < m_service_goal_obj_num; service_goal_index++)
			{
				cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(service_goal_index);
				const s_service_goal_info& tmp = temp_service_goal_obj.get_service_goal_info();
				
				faith::cs2dp_proto::role_service_goal_row *row_data = db_data->add_row_data();
				if (row_data == nullptr)
				{
					return;
				}
				for (int32 i = 0; i < e_service_goal_inst_data_max; i++)
				{
					row_data->add_data_ary(tmp.data_ary[i]);
				}
				++row_count;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_service_goal);
		}
	}

	void cservice_goal_mgr::service_goal_operate(const int32 operate_type, const int32 service_goal_id)
	{
		e_service_goal_operate_type temp_operate = (e_service_goal_operate_type)operate_type;
		switch (temp_operate)
		{
		case faith::e_service_goal_operate_type_none:
			break;
		case faith::e_service_goal_operate_type_receive:
			receive_service_goal(service_goal_id);
			break;
		default:
			break;
		}
	}

	void cservice_goal_mgr::refresh_service_goal(const e_service_goal_type service_goal_type)
	{
		std::vector<cservice_goal*> vector_cservice_goal = get_service_goal_obj_by_type(service_goal_type);
		int32 vector_cservice_goal_size = vector_cservice_goal.size();
		for (int32 i = 0; i < vector_cservice_goal_size; i++)
		{
			cservice_goal* service_goal_obj = vector_cservice_goal[i];
			if (nullptr == service_goal_obj)
			{
				continue;
			}
			service_goal_obj->refresh_service_goal(true);
		}
	}

	void cservice_goal_mgr::refresh_service_goal_all()
	{
		if (m_service_goal_obj_num < 0
			|| m_service_goal_obj_num >= MAX_SERVICE_GOAL_NUM)
		{
			return;
		}
		for (int32 array_index = 0; array_index < m_service_goal_obj_num; array_index++)
		{
			m_service_goal_obj_array[array_index].refresh_service_goal();
		}
	}

	void cservice_goal_mgr::refresh_over_time_all()
	{
		player& temp_player = get_player();
		if (!temp_player.is_valid())
		{
			return;
		}
		if (m_service_goal_obj_num < 0
			|| m_service_goal_obj_num >= MAX_SERVICE_GOAL_NUM)
		{
			return;
		}
		int32 time_now = time_helper::get_cur_time_new().second;
		bool is_send = false;
		if (temp_player.get_func_unlock_mgr().is_func_unlock("TargetHebdo") || temp_player.get_time_data(e_time_type_service_goal_close_time) > 0)
		{
			for (int32 i = 0; i < m_service_goal_obj_num; i++)
			{
				//大于0说明已经设置过了 不需要重复设置时间
				if (m_service_goal_obj_array[i].get_data_info(e_service_goal_inst_data_over_time) > 0)
				{
					continue;
				}
				m_service_goal_obj_array[i].refresh_over_time(time_now);
				is_send = true;
			}
			if (m_service_goal_max_over_time <= 0)
			{
				for (int32 i = 0; i < m_service_goal_obj_num; i++)
				{
					if (m_service_goal_obj_array[i].get_data_info(e_service_goal_inst_data_over_time) > m_service_goal_max_over_time)
					{
						m_service_goal_max_over_time = m_service_goal_obj_array[i].get_data_info(e_service_goal_inst_data_over_time);
					}
				}
			}
		} 
		if (is_send)
		{
			send_all_service_goal();
		}
	}

	int32 cservice_goal_mgr::get_service_goal_reward_days(const int32 reward_data, const e_service_goal_active_type active_type)
	{
		if (0 >= reward_data
			|| MAX_REWARD_DAYS < reward_data
			|| e_service_goal_active_type_max <= active_type)
		{
			return 0;
		}
		return 	m_service_goal_reward_days[reward_data][active_type];
	}

	int32 cservice_goal_mgr::get_specify_reward_num()
	{
		int32 specify_reward_num = 0;
		for (int32 array_index = 0; array_index < m_service_goal_obj_num; array_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(array_index);
			if (e_service_goal_type_big_reward == temp_service_goal_obj.get_service_goal_condition_type()
				&&(temp_service_goal_obj.is_received()))
			{
				specify_reward_num++;
			}
		}
		return specify_reward_num;
	}

	player& cservice_goal_mgr::get_player()  const
	{
		return unit_man::get_player(m_player_index);
	}

	cservice_goal& cservice_goal_mgr::get_service_goal_obj_by_index(const int32 obj_index)
	{
		if (0 > obj_index || MAX_SERVICE_GOAL_NUM <= obj_index)
		{
			return m_empty_goal_obj;
		}
		return m_service_goal_obj_array[obj_index];
	}

	cservice_goal& cservice_goal_mgr::get_service_goal_obj_by_id(const int32 obj_id)
	{
		for (int32 array_index = 0; array_index < m_service_goal_obj_num; array_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(array_index);
			if (obj_id == temp_service_goal_obj.get_service_goal_template_id())
			{
				return temp_service_goal_obj;
			}
		}
		return m_empty_goal_obj;
	}

	std::vector<cservice_goal*> cservice_goal_mgr::get_service_goal_obj_by_type(const e_service_goal_type service_goal_type)
	{
		std::vector<cservice_goal*> temp_vector;
		for (int32 array_index = 0; array_index < m_service_goal_obj_num; array_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(array_index);
			if (service_goal_type == temp_service_goal_obj.get_service_goal_condition_type())
			{
				temp_vector.push_back(&temp_service_goal_obj);
			}
		}
		return temp_vector;
	}

	void cservice_goal_mgr::create_service_goal_obj(ServiceGoalTemplate* template_ptr, const s_service_goal_info service_goal_info)
	{
		cservice_goal& new_service_goal_obj = get_service_goal_obj_by_index(m_service_goal_obj_num);
		new_service_goal_obj.init_service_goal(template_ptr, service_goal_info);
		m_service_goal_obj_num++;
	}

	void cservice_goal_mgr::send_message_to_self(google::protobuf::Message* net_pro, uint32 header)
	{
		player& temp_player = get_player();
		if (temp_player.is_valid())
		{
			temp_player.send_message_to_self(net_pro, header);
		}
	}
	bool cservice_goal_mgr::check_in_time()
	{
		int32 time_now = time_helper::get_cur_time_new().second;
		return time_now < m_service_goal_max_over_time;
	}
	bool cservice_goal_mgr::check_over()
	{
		player& temp_player = get_player();
		if (!temp_player.is_valid())
		{
			return false;
		}
		if (check_in_time())
		{
			return false;
		}
		for (int32 array_index = 0; array_index < m_service_goal_obj_num; array_index++)
		{
			cservice_goal& temp_service_goal_obj = get_service_goal_obj_by_index(array_index);
			if (temp_service_goal_obj.get_service_goal_active_type() == e_service_goal_active_type_advance || 
				(temp_service_goal_obj.get_service_goal_active_type() == e_service_goal_active_type_final && temp_service_goal_obj.get_service_goal_condition_type() == e_service_goal_type_no_limit_time_final) )
			{
				if (!temp_service_goal_obj.is_received())
				{
					return false;
				}
			}
		}
		//设置玩家  
		int32 cur_time = time_helper::get_today_zero_time_info().second;
		temp_player.set_time_data(e_time_type_service_goal_close_time, cur_time + day_time_second);
		temp_player.send_time_one(e_time_type_service_goal_close_time);
		return true;
	}
}