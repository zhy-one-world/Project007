#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "recycle.h"
#include "internet/recycle.pb.h"
#include "recycle_mgr.h"
#include "template/template_manager.h"
#include "internet/net.pb.h"

namespace hld
{

	recycle::recycle()
	{
	}

	recycle::~recycle()
	{
	}
	void recycle::clear_data()
	{
		m_data_info.reset();

	}

	void recycle::cycle_reset()
	{
		m_data_info.cycle_reset();
	}

	void recycle::set_db_data(const s_recycle_info& in_data_info)
	{
		m_data_info = in_data_info;

	}

	void recycle::set_start_time(int32 in_start_time)
	{
		m_data_info.e_start_time_1 = in_start_time;
	}
	int32 recycle::get_start_time()
	{
		return m_data_info.e_start_time_1;
	}



	bool recycle::get_login_flag_bit(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return false;
		}

		return (m_data_info.e_login_state_2 & 1 << idx) != 0 ;

	}
	void recycle::set_login_flag_bit(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return;
		}

		m_data_info.e_login_state_2 = m_data_info.e_login_state_2 | 1 << idx;
	}

	bool recycle::get_prop_flag_bit(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return false;
		}

		return (m_data_info.e_prop_state_3 & 1 << idx) != 0;

	}
	void recycle::set_prop_flag_bit(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return;
		}

		m_data_info.e_prop_state_3 = m_data_info.e_prop_state_3 | 1 << idx;
	}


	bool recycle::is_key_reward()
	{
		return  m_data_info.inviter_guid.is_valid();
	}

	void recycle::set_key_reward(guid_64 in_inviter_guid)
	{
		m_data_info.inviter_guid = in_inviter_guid;
	}

	bool recycle::get_invited_level_flag_bit(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return false;
		}

		return (m_data_info.invited_state & 1 << idx) != 0;

	}
	void recycle::set_invited_level_flag_bit(int32 idx)
	{
		if (idx < 1 || idx > 31)
		{
			return;
		}

		m_data_info.invited_state = m_data_info.invited_state | 1 << idx;
	}




	/////////////////////////////////////  recycle  task////////////////////////////////////////////////////////

	recycle_task::recycle_task()
	{
		m_config_ptr = nullptr;
	}

	recycle_task::~recycle_task()
	{
	}
	void recycle_task::clear_data()
	{
		m_config_ptr = nullptr;
		m_recycle_task_info.reset();

	}
	bool recycle_task::set_inst_data(int32 idx, int32 value)
	{
		if (idx >= e_recycle_tk_config_id && idx < e_recycle_tk_max)
		{
			m_recycle_task_info.data_ary[idx] = value;
			return true;
		}
		return false;
	}

	int32 recycle_task::get_inst_data(int32 idx) const
	{
		if (idx >= e_recycle_tk_config_id &&  idx < e_recycle_tk_max)
		{
			return m_recycle_task_info.data_ary[idx];
		}
		else return -1;
	}

	bool recycle_task::init_recycle_task_by_template(int32 recycle_task_id)
	{
		RecycleTemplate *config_ptr = GET_TEMPLATE(RecycleTemplate, recycle_task_id);
		if (config_ptr == nullptr)
		{
			return false;
		}
		m_config_ptr = config_ptr;
		m_recycle_task_info.reset();
		set_inst_data(e_recycle_tk_config_id, recycle_task_id);
		return true;

	}
	bool recycle_task::init_recycle_task_by_info(const s_recycle_task_info &task_info)
	{
		for (int32 idx = e_recycle_tk_config_id; idx < e_recycle_tk_max; idx++)
		{
			if (set_inst_data(idx, task_info.data_ary[idx]) == false)
			{
				return false;
			}
		}
		int32 task_id = get_inst_data(e_recycle_tk_config_id);
		RecycleTemplate *config_ptr = GET_TEMPLATE(RecycleTemplate, task_id);
		if (config_ptr == nullptr)
		{
			return false;
		}
		m_config_ptr = config_ptr;
		return true;
	}

	bool recycle_task::is_finish_num()
	{
		if (m_config_ptr == nullptr || m_config_ptr->ConditionValue.size() < 1)
		{
			return false;
		}
		int32 num = get_inst_data(e_recycle_tk_finish_num);
		if (num < m_config_ptr->ConditionValue[0])
		{
			return false;
		}
		return true;
	}
	bool recycle_task::add_finish_num(int32 add_num)
	{
		if (m_config_ptr == nullptr || m_config_ptr->ConditionValue.size() < 1)
		{
			return false;
		}
		int32 num = get_inst_data(e_recycle_tk_finish_num);

		int32 total = num + add_num;

		if (num + add_num > m_config_ptr->ConditionValue[0])
		{ 
			total = m_config_ptr->ConditionValue[0];
		}
		set_inst_data(e_recycle_tk_finish_num, total);

		if (total == m_config_ptr->ConditionValue[0])
		{
			set_inst_data(e_recycle_tk_state, 1);
		}
		return true;
	}


	/////////////////////////////////////////////////

	recycle_invited::recycle_invited()
	{ 
		m_array_index = 0;
		clear_data();
	}

	recycle_invited::~recycle_invited()
	{
	}
	void recycle_invited::clear_data()
	{
		m_data.clear();
	}

	void recycle_invited::set_player_ptr(const int32 array_index)
	{
		m_array_index = array_index;
	}

	void recycle_invited::add(const s_recycle_invited_info & s_invited_info, bool is_send )
	{
		recycle_invited_map_it it = m_data.find(s_invited_info.role_guid);
		if (it != m_data.end())
		{
			return;
		}
		if (m_data.size() > MAX_RECYCLE_INVITED_NUM)
		{
			return;
		}

		m_data[s_invited_info.role_guid] = s_invited_info; 

		if (is_send)
		{
			sync_one_invited_one(s_invited_info);
		}

	}

	void recycle_invited::sync_all_invited_list()
	{

		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		recycle_proto_recycle_invited_list all_msg;

		for (recycle_invited_map_it it = m_data.begin() ; it != m_data.end(); it++)
		{
			s_recycle_invited_info& temp_info = it->second;
			recycle_proto_recycle_invited_one* one_msg = all_msg.add_invited_array();
			one_msg->add_guid(temp_info.role_guid.A);
			one_msg->add_guid(temp_info.role_guid.B);
			one_msg->set_level(temp_info.level);
			one_msg->set_head_id(temp_info.head_id);
			int32 grade_level = get_grade_level(temp_info.head_id, temp_info.class_type);
			one_msg->set_grade_level(grade_level);
			one_msg->set_invited_name(temp_info.invited_name);
		}

		temp_player.send_message_to_self(&all_msg, e_msgindex_s2c_recycle_invited_all);
	}

	void recycle_invited::sync_one_invited_one( const s_recycle_invited_info & temp_info)
	{
		player& temp_player = unit_man::get_player(m_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		recycle_proto_recycle_invited_one one_msg;
		one_msg.add_guid(temp_info.role_guid.A);
		one_msg.add_guid(temp_info.role_guid.B);
		one_msg.set_level(temp_info.level);
		one_msg.set_head_id(temp_info.head_id);
		int32 grade_level = get_grade_level(temp_info.head_id, temp_info.class_type);
		one_msg.set_grade_level(grade_level);
		one_msg.set_invited_name(temp_info.invited_name);

		temp_player.send_message_to_self(&one_msg, e_msgindex_s2c_recycle_invited_one);

	}
	int32 recycle_invited::get_grade_level(int32 head, int32 class_type)
	{
		return unit_man::get_grade_num(head, class_type);
	}

	bool recycle_invited::is_get_reward(int32 level, int32 num)
	{
		int32 count = 0;
		for (recycle_invited_map_it it = m_data.begin(); it != m_data.end(); it++)
		{
			s_recycle_invited_info& temp_info = it->second;
			int32 grade_level = get_grade_level(temp_info.head_id, temp_info.class_type);
			if (grade_level < level)
			{
				continue;
			}
			count++;
		}
		if (count < num)
		{
			return false;
		}
		return true;
	}

	void recycle_invited::tick()
	{
		 for (recycle_invited_map_it it = m_data.begin(); it != m_data.end(); it++)
		 {
			 s_recycle_update_info&  update_info = recycle_man::get_instance().get_level_info(it->first);
			 if (update_info.m_level == 0)
			 {
				 return;
			 }
			 s_recycle_invited_info& temp_info = it->second;
			 if (temp_info.level == update_info.m_level &&
				 temp_info.head_id == update_info.m_head &&
				 temp_info.class_type == update_info.m_class_type)
			 {
				 continue;
			 }
			 temp_info.level = update_info.m_level;
			 temp_info.head_id = update_info.m_head;
			 temp_info.class_type = update_info.m_class_type;
			 sync_one_invited_one(temp_info);	   
		 }
	}


	


}