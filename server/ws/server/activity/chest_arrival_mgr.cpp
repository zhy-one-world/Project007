#include "chest_arrival_mgr.h"
#include "utility/init_unit.h"
#include "utility/cs_date.hpp"
#include "../mail/event_ws_mgr.h"
#include "template/template_manager.h"
#include "world_server.hpp"

namespace hld
{
	chest_arrival_mgr::chest_arrival_mgr()
	{
		m_check_spawn_time_tick = 0;
		for (int32 i = 0; i < chest_arrival_activity::spawn_chest_time_arr_size; ++i)
		{
			m_chest_wave_flag[i] = false;
		}
	}

	void chest_arrival_mgr::tick(const float elapse)
	{
		//m_check_spawn_time_tick += elapse;
		//if (m_check_spawn_time_tick >= check_spawn_time_tick_duration)
		//{
		//	m_check_spawn_time_tick -= check_spawn_time_tick_duration;
		//	check_should_spawn_tick();
		//}
	}

	void chest_arrival_mgr::check_should_spawn_tick()
	{
		int32 cur_time = time_helper::get_today_time_in_sec();

		int32 spawn_time = 0;
		int32 disappear_time = 0;
		for (int32 i = 0; i < chest_arrival_activity::spawn_chest_time_arr_size; ++i)
		{
			spawn_time = chest_arrival_activity::spawn_chest_time[i];
			disappear_time = spawn_time + chest_arrival_activity::chest_live_time;

			// 如果是在活动时间内就刷出宝箱(即使是活动时间内关闭重启服务器活动也会继续正常进行
			// 而且新刷宝箱也会延续之前的剩余时间)
			if (spawn_time <= cur_time && cur_time < disappear_time)
			{
				if (get_chest_wave_flag(i) == true)
				{
					return;
				}
				set_chest_wave_flag(i);
				spawn_chests(disappear_time);
				send_activity_announcement();
			}
		}
	}

	void chest_arrival_mgr::set_chest_wave_flag(int32 wave_index)
	{
		for (int32 i = 0; i < chest_arrival_activity::spawn_chest_time_arr_size; ++i)
		{
			m_chest_wave_flag[i] = (i == wave_index ? true : false);
		}
	}

	bool chest_arrival_mgr::get_chest_wave_flag(int32 wave_index)
	{
		if (wave_index < 0 || chest_arrival_activity::spawn_chest_time_arr_size <= wave_index)
		{
			return false;
		}
		return m_chest_wave_flag[wave_index];
	}

	void chest_arrival_mgr::spawn_chests(const int32 chest_disappear_time)
	{
		if (chest_disappear_time <= 0)
		{
			return;
		}

		ws2cs_chest_arrivai_spawn_chest_wave spawn_chest_wave_msg;
		spawn_chest_wave_msg.chest_disappear_time = chest_disappear_time;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_chest_arrivai_spawn_chest_wave);
		if (is_use == false)
		{
			send_message_to_all_cs(&spawn_chest_wave_msg, sizeof(spawn_chest_wave_msg));
		}
		else
		{
			hld::ws2cs_proto::chest_arrivai_spawn_chest_wave pro_msg;
			pro_msg.set_chest_disappear_time(chest_disappear_time);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_chest_arrivai_spawn_chest_wave, e_server_type_cs);
		}
	}

	void chest_arrival_mgr::send_activity_announcement()
	{
		int32 create_time = time_helper::get_cur_time_new().second;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(spawn_chests_announcement_template_id);
		event_ws_mgr::get_instance().send_notice_to_all(spawn_chests_announcement_template_id, create_time, guid_64(), notice_str);
	}

	void chest_arrival_mgr::send_message_to_all_cs(const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr || data_len == 0)
		{
			return;
		}
		world_server::getInstance().broadcast(data_ptr, data_len, e_server_type_cs);
	}
}
