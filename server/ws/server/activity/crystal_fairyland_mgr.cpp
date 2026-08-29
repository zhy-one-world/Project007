#include "crystal_fairyland_mgr.h"
#include "utility/init_unit.h"
#include "utility/cs_date.hpp"
#include "../mail/event_ws_mgr.h"
#include "template/template_manager.h"
#include "Logic/time_def.hpp"
#include "cross_server_pk_msg.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "world_server.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "base/ecs_world.h"
#include "system/scene/cs_map_system.h"
#include "net.pb.h"

namespace faith
{ 

	crystal_fairyland_mgr::crystal_fairyland_mgr()
	{
		clear_data();
		init_data();
	}

	crystal_fairyland_mgr::~crystal_fairyland_mgr()
	{

	}


	void	crystal_fairyland_mgr::tick(int64& new_time)
	{
		if (new_time > m_next_tick)
		{
			m_next_tick = new_time + minute_tick_time;

			check_open_double_time();
			if (is_during_double_time())
			{
				double_time_send_notice_to_all();
			}
			else
			{
				set_notice_flag(false);
			}
		}
	}

	void	crystal_fairyland_mgr::clear_data()
	{
		m_had_notice = false;
		m_next_tick = 0;
		m_during_double_time = false;
		m_act_template_ptr = nullptr;
	}

	void	crystal_fairyland_mgr::init_data()
	{
		int32 ActTemplateID = faith::first_activity_common_template_id + e_activity_type_crystal_fairyland;
		m_act_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, ActTemplateID);
	}

	void crystal_fairyland_mgr::recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id)
	{
		auto ret = is_can_join();
		int32 error_ret = ret;
		int32 war_idex = 0;
		int32 map_temp_id = shui_jing_map_tempplate_id;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity) > 0)
		{
			map_temp_id = shui_jing_map_cross_temp_id;
		}
		guid_64 map_guid = guid_64();
		if (ret == e_error_code_success)
		{
			int32 war_idx = 0;
			map_guid = get_valid_shui_jing_map(war_idx);
		}
		if (false == map_guid.is_valid())
		{
			ret = e_error_code_map_init_map_err;
		}
		ws2ws_transfer_cross_pk_map_result msg;
		msg.role_guid = role_guid;
		msg.result = error_ret;
		msg.map_guid = map_guid;
		msg.map_temp_id = map_temp_id;
		msg.war_idex = war_idex;
		msg.activity_type = (int32)e_activity_type_crystal_fairyland;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_s2c_pk_map_island_state_msg, &msg, sizeof(msg));
	}

	guid_64 crystal_fairyland_mgr::get_valid_shui_jing_map(int32& map_war_idx)
	{
		int32 map_temp_id = shui_jing_map_tempplate_id;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity) > 0)
		{
			map_temp_id = shui_jing_map_cross_temp_id;
		}
		if (!is_activity_open())
		{
			m_crystal_fairyland_map_guid.clear_data();
			return guid_64();
		}

		Entity* map_ent = nullptr;
		if (m_crystal_fairyland_map_guid.is_valid())
		{
			map_ent = get_entity(m_crystal_fairyland_map_guid);
			if (nullptr == map_ent)
			{
				m_crystal_fairyland_map_guid.clear_data();
			}
		}

		if (false == m_crystal_fairyland_map_guid.is_valid())
		{
			auto new_map_ent = cs_map_mgr_system::init_map_by_battle(map_temp_id);
			if (new_map_ent)
			{
				m_crystal_fairyland_map_guid = new_map_ent->getEntityId();
				map_ent = new_map_ent;
			}
		}

		if (nullptr != map_ent)
		{
			map_war_idx = cs_map_system::get_unit_count(map_ent) % 4 + 1;
		}
		return m_crystal_fairyland_map_guid;
	}

	e_error_code crystal_fairyland_mgr::is_can_join()
	{
		if (false == is_activity_open())
		{
			return e_error_code_map_map_not_found;
		}
		if (cs_map_system::is_full(m_crystal_fairyland_map_guid))
		{
			return e_error_code_map_max_players;
		}
		return e_error_code_success;
	}

	bool crystal_fairyland_mgr::is_activity_open()
	{
		return world_server::getInstance().get_activity_sec_left(e_activity_type_crystal_fairyland, e_activity_time_get_all) >= 0;
	}

	void	crystal_fairyland_mgr::check_open_double_time()
	{
		if (nullptr == m_act_template_ptr)
		{
			return;
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(e_activity_type_crystal_fairyland);
		if (open_time.size() != once_activity_open_time_array_size || m_act_template_ptr->ActivityOpenTime1.size() != once_activity_open_time_array_size)
		{
			return;
		}
		int32 BeginStamp1 = faith::time_helper::get_stamp_by_hour_min(open_time[4], open_time[5]);
		int32 EndStamp1 = faith::time_helper::get_stamp_by_hour_min(open_time[6], open_time[7]);
		int32 BeginStamp2 = faith::time_helper::get_stamp_by_hour_min(m_act_template_ptr->ActivityOpenTime1[4], m_act_template_ptr->ActivityOpenTime1[5]);
		int32 EndStamp2 = faith::time_helper::get_stamp_by_hour_min(m_act_template_ptr->ActivityOpenTime1[6], m_act_template_ptr->ActivityOpenTime1[7]);
		int64 cur_time = time_helper::get_cur_time_new().second;

		if (BeginStamp1 < cur_time && cur_time <= EndStamp1)
		{
			set_open_double_time(true);
			return;
		}
		else if (BeginStamp2 < cur_time && cur_time <= EndStamp2)
		{
			set_open_double_time(true);
			return;
		}
		set_open_double_time(false);
	}

	void	crystal_fairyland_mgr::double_time_send_notice_to_all()
	{
		if (m_had_notice)
		{
			return;
		}
		m_had_notice = true;
		int32 notice_id = 93000146;
		int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		std::string notice_str = init_unit::implode(vec_notice_str);
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}
}
