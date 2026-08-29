#include "time.hpp"
#include "unit_move_mgr.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "internet/aoi.pb.h"
#include "internet/net.pb.h"

namespace faith
{
	unit_move_mgr::unit_move_mgr()
	{
		m_unit_array_index = 0;
		clear_data();
	};

	void unit_move_mgr::clear_data()
	{
		m_is_runing = false;
		m_tick_time = 0;
		m_move_speed = 0;
		m_move_length = 0;
		m_move_yaw = 0;
		m_move_length_sum = 0;
		m_move_time = 0;
		m_be_back = false;
		m_can_break = true;
		m_unit_pos_tick = 0;
	}
	void unit_move_mgr::heart_tick(const int64& new_time)
	{
		if (m_is_runing)
		{
			mid_move(new_time);
		}
		tick_sync_pos(new_time);
	}

	void unit_move_mgr::start(fvector const& end_pos, float move_time, bool can_break, bool send_client)
	{
		clear_data();
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		const s_map_pos& old_map_pos = unit_ref.get_new_map_pos();
		fvector sub_pos = end_pos - old_map_pos.unit_location;
		float move_yaw = sub_pos.rotator().Yaw;
		s_map_pos new_map_pos;
		new_map_pos.unit_location = unit_ref.get_new_map_pos().unit_location;
		new_map_pos.set_rotation(old_map_pos.unit_rotation);
		new_map_pos.unit_rotation.yaw = move_yaw;
		m_can_break = can_break;
		if (move_time <= 0.1f || sub_pos.length_2d() < 1.0f)
		{
			new_map_pos.set_location(end_pos);
			unit_ref.set_new_map_pos(new_map_pos);
			if (send_client)
			{
				send_unit_move(end_pos, 0, move_time, move_yaw);
			}
		}
		else
		{
			float move_length = sub_pos.length_2d();
			unit_ref.set_new_map_pos(new_map_pos);
			if (send_client)
			{
				send_unit_move(end_pos, 0, move_time, move_yaw);
			}
			start(move_length, move_time, move_yaw, false, false, false, can_break, false);
		}
	}
	void unit_move_mgr::start(int32 move_length, float move_time, float move_yaw, bool be_back, bool is_not_sync_rotation, bool is_client_driver, bool can_break, bool send_client)
	{
		if (move_length == 0)
		{
			return;
		}
		clear_data();
		if (move_length < 0)
		{
			m_be_back = true;
			move_length = abs(move_length);
		}
		else
		{
			m_be_back = false;
		}
		if (send_client)
		{
			send_unit_move(fvector(), move_length, move_time, move_yaw, m_be_back, is_not_sync_rotation, is_client_driver);
		}
		if (move_time < 0.1)
		{
			m_tick_time = utility::get_tick_count();
			m_move_speed = move_length;
			m_move_length = move_length;
			m_move_yaw = move_yaw;
			mid_move(0);
		}
		else
		{
			m_is_runing = true;
			m_tick_time = utility::get_tick_count();
			m_move_time = move_time * 1000;
			m_move_speed = move_length / move_time;
			m_move_length = move_length;
			m_move_yaw = move_yaw;
		}
	}

	void unit_move_mgr::stop(bool can_break)
	{
		if (can_break)
		{
			clear_data();
		}
		else
		{
			if (m_can_break == false)
			{
				clear_data();
			}
		}
	}

	void unit_move_mgr::mid_move(const int64& new_time)
	{
		float move_length = m_move_length;
		if (new_time > 0)
		{
			int64 sub_time = new_time - m_tick_time;
			m_tick_time = new_time;
			m_move_time -= sub_time;
			move_length = m_move_speed * sub_time / 1000;
		}
		if (m_move_length_sum + move_length > m_move_length)
		{
			move_length = m_move_length - m_move_length_sum;
			if (move_length <= 0)
			{
				stop(true);
				return;
			}
		}
		m_move_length_sum += move_length;
		if (m_move_length_sum <= 0 && m_move_time <= 0)
		{
			stop(true);
			return;
		}
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		s_map_pos unit_map_pos = unit_ref.get_new_map_pos();
		if (m_be_back)
		{
			unit_map_pos.unit_location.x -= move_length * cos(m_move_yaw * faith::RADIAN);
			unit_map_pos.unit_location.y -= move_length * sin(m_move_yaw * faith::RADIAN);
		}
		else
		{
			unit_map_pos.unit_location.x += move_length * cos(m_move_yaw * faith::RADIAN);
			unit_map_pos.unit_location.y += move_length * sin(m_move_yaw * faith::RADIAN);
		}

		unit_ref.set_new_map_pos(unit_map_pos);
	}
	void unit_move_mgr::send_unit_move(const fvector& dest_pos, float move_length, float move_time, float move_yaw, bool be_back, bool is_not_sync_rotation, bool is_client_driver)
	{
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		const s_map_pos& unit_map_pos = unit_ref.get_new_map_pos();
		aoi_proto_aoi_move msg;
		msg.set_unit_guid(unit_ref.get_unit_guid().server_64);
		msg.add_cur_pos(unit_map_pos.unit_location.x);
		msg.add_cur_pos(unit_map_pos.unit_location.y);
		msg.add_cur_pos(unit_map_pos.unit_location.z);
		msg.add_dest_pos(dest_pos.x);
		msg.add_dest_pos(dest_pos.y);
		msg.add_dest_pos(dest_pos.z);
		msg.set_move_time(move_time);
		msg.set_move_length(move_length);
		msg.set_move_yaw(move_yaw);
		msg.set_be_back(be_back);
		msg.set_is_not_sync_rotation(is_not_sync_rotation);
		msg.set_is_client_driver(is_client_driver);
		unit_ref.send_message_to_aoi(&msg, e_msgindex_s2c_move, true);
	}
	void unit_move_mgr::send_unit_end_location(const fvector end_location)
	{
		m_unit_pos_tick = utility::get_tick_count() + sync_pos_fly_tick_time;
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		aoi_proto_aoi_pos_update m_aoi_update_msg;
		m_aoi_update_msg.set_unit_guid(unit_ref.get_unit_guid().server_64);
		m_aoi_update_msg.add_move_vector(end_location.x);
		m_aoi_update_msg.add_move_vector(end_location.y);
		m_aoi_update_msg.add_move_vector(end_location.z);
		unit_ref.send_message_to_aoi(&m_aoi_update_msg, e_msgindex_s2c_aoi_update, true, false);
		//aoi_system::send_message_aoi_all(
		//	unit_ref.get_map_guid(),
		//	&m_aoi_update_msg,
		//	e_msgindex_s2c_aoi_update,
		//	unit_ref.get_old_map_pos().unit_location,
		//	m_unit_array_index);
	}
	void unit_move_mgr::tick_sync_pos(const int64& new_time)
	{
		if (new_time < m_unit_pos_tick)
		{
			return;
		}
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		if (unit_ref.is_valid() == false)
		{
			return;
		}
		send_unit_yaw();
		if (unit_ref.get_old_map_pos().unit_location != unit_ref.get_new_map_pos().unit_location)
		{
			if (unit_ref.get_pawn_att().is_can_move())
			{
				world_cs::update_aoi_pos(unit_ref.get_map_ent(), unit_ref.get_array_index(), unit_ref.get_old_map_pos().unit_location, unit_ref.get_new_map_pos().unit_location);
				unit_ref.set_old_map_pos(unit_ref.get_new_map_pos());
			}
		}
		m_unit_pos_tick = new_time + sync_pos_walk_tick_time;
	}
	void unit_move_mgr::send_unit_yaw()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		if (player_ref.get_old_map_pos().unit_rotation.yaw != player_ref.get_new_map_pos().unit_rotation.yaw)
		{
			aoi_proto_unit_rotation_end msg;
			msg.set_unit_guid(player_ref.get_unit_guid().server_64);
			msg.set_yaw(player_ref.get_new_map_pos().unit_rotation.yaw);
			player_ref.send_message_to_aoi(&msg, e_msgindex_s2c_aoi_rotation, false);
			player_ref.set_old_map_yaw(player_ref.get_new_map_pos().unit_rotation.yaw);
		}
	}
}