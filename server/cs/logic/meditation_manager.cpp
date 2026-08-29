/********************************************************************
created: 2016年9月20日17:46:11
file base: meditation_manager
file ext: h
author: zhangminghai

purpose: about meditation
*********************************************************************/


#include "func_unlock_mgr.h"
#include "internet/character.pb.h"
#include "internet/net.pb.h"
#include "logic/buff_man.h"
#include "logic/player.hpp"
#include "Logic/time_def.hpp"
#include "meditation_manager.h"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "time.hpp"
#include "utility/globle_data.h"
#include "utility/init_unit.h"



namespace faith
{
	meditation_manager::meditation_manager()
	{
		m_player_ptr = nullptr;
		clear_data();
	}
	meditation_manager::~meditation_manager()
	{

	}

	void meditation_manager::heart_tick(const int64& new_time)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (m_old_tick_time == 0)
		{
			m_old_tick_time = new_time;
			return;
		}
		int64 gap_time = new_time - m_old_tick_time;
		if (!m_player_ptr->get_func_unlock_mgr().is_func_unlock("Meditation"))
		{
			return;
		}
		if (m_player_ptr->is_self_server() == false)
		{
			return;
		}
		if (m_player_ptr->is_dead())
		{
			return;
		}
		int32 meditation_time_max = GAMECONFIG->MeditationTimeMax * second_tick_time;
		if (!m_is_in_meditation)
		{
			m_no_operate_time += gap_time;
			if (m_no_operate_time >= meditation_begin_time)
			{
				start_mediation(false);
			}

			/*取消冥想药水 以及剩余时间计算
			if (m_no_operate_time >= meditation_begin_time && m_player_ptr->get_time_data(e_time_type_meditation_remaining_time) > 0)
			{
				start_mediation(false);
			}
			*/

		}

		/*取消冥想药水 以及剩余时间计算
		else if(m_player_ptr->get_time_data(e_time_type_have_meditation_time) + gap_time < meditation_time_max && m_player_ptr->get_time_data(e_time_type_meditation_remaining_time) > gap_time)
		{
			int64 cur_meditation_time = m_player_ptr->get_time_data(e_time_type_have_meditation_time);
			int64 meditation_remaining_time = m_player_ptr->get_time_data(e_time_type_meditation_remaining_time);
			m_player_ptr->set_time_data(e_time_type_have_meditation_time, cur_meditation_time + gap_time);
			m_player_ptr->set_time_data(e_time_type_meditation_remaining_time, meditation_remaining_time - gap_time);
		}
		else
		{
			if (m_player_ptr->get_time_data(e_time_type_have_meditation_time) + gap_time >= meditation_time_max)
			{
				int64 meditation_time = meditation_time_max - m_player_ptr->get_time_data(e_time_type_have_meditation_time);
				int64 meditation_remaining_time = m_player_ptr->get_time_data(e_time_type_meditation_remaining_time);
				m_player_ptr->set_time_data(e_time_type_have_meditation_time, meditation_time_max);
				m_player_ptr->set_time_data(e_time_type_meditation_remaining_time, meditation_remaining_time - meditation_time);

			}else if (m_player_ptr->get_time_data(e_time_type_meditation_remaining_time) <= gap_time)
			{
				int64 cur_meditation_time = m_player_ptr->get_time_data(e_time_type_have_meditation_time);
				int64 meditation_remaining_time = m_player_ptr->get_time_data(e_time_type_meditation_remaining_time);
				m_player_ptr->set_time_data(e_time_type_have_meditation_time, cur_meditation_time + meditation_remaining_time);
				m_player_ptr->set_time_data(e_time_type_meditation_remaining_time, 0);
			}
			break_meditation();
		}
		*/

		else if (m_player_ptr->get_time_data(e_time_type_have_meditation_time) < meditation_time_max)
		{
			int64 cur_meditation_time = m_player_ptr->get_time_data(e_time_type_have_meditation_time);
			m_player_ptr->set_time_data(e_time_type_have_meditation_time, cur_meditation_time + gap_time);
		}

 		if (m_player_ptr->get_time_data(e_time_type_meditation_full_send_mail) == 0)
 		{
 			if (m_player_ptr->get_time_data(e_time_type_have_meditation_time) >= meditation_time_max)
 			{
 				if (m_player_ptr->get_func_unlock_mgr().is_func_unlock("Meditation"))
				{
 					globle_data::get_instance().send_mail_to_another_player_by_system(
						m_player_ptr->get_unit_guid(), 
						m_player_ptr->get_unit_guid(), 
						m_player_ptr->get_unit_info(e_role_info_server_id),
						"90303013", "90303027", "90303028",
						0, 0, 0, 0, 0, 0);
 					m_player_ptr->set_time_data(e_time_type_meditation_full_send_mail, 1); //成功发送后将标记置位 1 ，表示无需再发送邮件
 				}
 			}
 		}
		
		m_old_tick_time = new_time;
	}

	void meditation_manager::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}

	void meditation_manager::offline_meditation_settlement()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}

		if (m_player_ptr->get_func_unlock_mgr().is_func_unlock("Meditation") == false)
		{
			return;
		}
		
		i64 m_temp_time = 0;
		const int64 now_time = utility::get_tick_count();
		const int64 last_logout_time = m_player_ptr->get_time_data(e_time_type_last_save_data_time);
		const int64 have_meditation_time = m_player_ptr->get_time_data(e_time_type_have_meditation_time);
		int32 meditation_time_max = GAMECONFIG->MeditationTimeMax * second_tick_time;

		//取消冥想药水 以及剩余时间计算
		/*int64 meditation_remaining_time = m_player_ptr->get_time_data(e_time_type_meditation_remaining_time);
		if (last_logout_time > 0 && meditation_remaining_time > 0)
		{
			if (meditation_remaining_time >= (now_time - last_logout_time))
			{
				m_temp_time = now_time - last_logout_time + have_meditation_time;
				m_temp_time = m_temp_time > meditation_time_max ? meditation_time_max : m_temp_time;
				m_player_ptr->set_time_data(e_time_type_meditation_remaining_time, meditation_remaining_time - (now_time - last_logout_time));
			}
			else
			{
				m_temp_time = meditation_remaining_time + have_meditation_time;
				m_temp_time = m_temp_time > meditation_time_max ? meditation_time_max : m_temp_time;
				m_player_ptr->set_time_data(e_time_type_meditation_remaining_time, 0);
			}
		}
		m_player_ptr->send_time_one(e_time_type_meditation_remaining_time);*/

		if (last_logout_time > 0)
		{
			m_temp_time = now_time - last_logout_time + have_meditation_time;
			m_temp_time = m_temp_time > meditation_time_max ? meditation_time_max : m_temp_time;
		}

		m_player_ptr->set_time_data(e_time_type_have_meditation_time, m_temp_time);
		m_player_ptr->send_time_one(e_time_type_have_meditation_time);
	}

	void meditation_manager::start_mediation(bool is_player_active)
	{ 
		if (m_player_ptr == nullptr)
		{
			return;
		}
		//if (!m_player_ptr->get_func_unlock_mgr().is_func_unlock("Meditation"))
		//{
		//	//防止到时间后无限调用这个函数
		//	m_no_operate_time = 0;
		//	return;
		//}

		m_is_in_meditation = true;
		m_no_operate_time = 0;

		character_proto_start_meditation_end resp;
		resp.add_role_guid(m_player_ptr->get_unit_guid().A);
		resp.add_role_guid(m_player_ptr->get_unit_guid().B);
		resp.set_is_player_active(is_player_active);
		m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_start_meditation_end);

	}

	void meditation_manager::stop_mediation()
	{
		m_is_in_meditation = false;
		m_no_operate_time = 0;
		if (nullptr != m_player_ptr)
		{
			m_player_ptr->send_time_one(e_time_type_have_meditation_time);
			//m_player_ptr->send_time_one(e_time_type_meditation_remaining_time);			//取消冥想药水 以及剩余时间计算
			character_proto_stop_meditation resp;
			resp.add_role_guid(m_player_ptr->get_unit_guid().A);
			resp.add_role_guid(m_player_ptr->get_unit_guid().B);
			m_player_ptr->send_message_to_self(&resp, e_msgindex_s2c_stop_mediation); 
		}
	}


	void meditation_manager::clear_data()
	{
		m_no_operate_time = 0;
		m_old_tick_time = 0;
		m_is_in_meditation = false;
	}


	void meditation_manager::break_meditation()
	{
		m_no_operate_time = 0;
		if (m_is_in_meditation)
		{
			stop_mediation();
		}
	}
	xstring meditation_manager::lua_meditation_add_exp(int32 get_type , int32 gs_value , int32 BaseExp, float MeditationExpAddition , float MeditationExpRewardRadio , int32 meditation_standard_gs , int32 coefficient , int32 meditation_time)
	{
		if (m_player_ptr == nullptr)
		{
			return "";
		}
		int64 exp_num = BaseExp * MeditationExpRewardRadio;
		switch (get_type)
		{
		case e_meditation_reward_usual:
		{
			exp_num = exp_num + gs_value * (int64)(BaseExp) * (int64)(MeditationExpAddition * 100) * (int64)(MeditationExpRewardRadio) / meditation_standard_gs / (int64)(coefficient);
			exp_num *= meditation_time;
		}
		break;
		case e_meditation_reward_double:
		{

		}
		break;
		case e_meditation_reward_four:	//钻石奖励
		{
			exp_num = exp_num + gs_value * (int64)(BaseExp) * (int64)(MeditationExpAddition * 100) * (int64)(MeditationExpRewardRadio) / meditation_standard_gs / (int64)(coefficient);
			exp_num *= 1.5;
			exp_num *= meditation_time;
		}
		break;
		};
		return init_unit::change_i64_to_string(exp_num);
	}
	void meditation_manager::add_buff(const bool is_add, const int32 buff_id)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		const int32 unit_array_index = m_player_ptr->get_array_index();
		if (true == is_add)
		{
			buff_man::add_buff_inst(unit_array_index, unit_array_index, buff_id);
		}
		else
		{
			buff_man::del_buff_by_template_id(unit_array_index, unit_array_index, buff_id);
		}
	}

	void meditation_manager::set_meditation_reward_time()
	{
		if (m_player_ptr->get_time_data(e_time_type_have_meditation_time) == 0)
		{
			m_player_ptr->set_time_data(e_time_type_have_meditation_time, GAMECONFIG->MeditationFirstRewardTime * second_tick_time);
			m_player_ptr->send_time_one(e_time_type_have_meditation_time);
		}
	}
}
