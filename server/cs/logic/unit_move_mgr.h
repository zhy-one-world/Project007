
#ifndef __UNIT_MOVE_MGR_H__
#define __UNIT_MOVE_MGR_H__

#include "logic/type_def.hpp"
namespace faith
{
	class unit_move_mgr
	{
	public:
		unit_move_mgr();
		void clear_data();
		void heart_tick(const int64& new_time);
		void set_unit_ptr(int32 unit_array_index) { m_unit_array_index = unit_array_index; }

		void start(fvector const& end_pos, float move_time, bool can_break = true, bool send_client = true);
		void start(int32 move_length, float move_time, float move_yaw, bool be_back = false, bool is_not_sync_rotation = false, bool is_client_driver = false, bool can_break = true, bool send_client = true);

		void stop(bool can_break = false);

		void mid_move(const int64& new_time);
	public:
		void send_unit_move(const fvector& dest_pos, float move_length, float move_time, float move_yaw, bool be_back = false, bool is_not_sync_rotation = false, bool is_client_driver = false);
		void send_unit_end_location(const fvector end_location);
		void send_unit_yaw();
		void tick_sync_pos(const int64& new_time);
	public:
		bool& is_runing() { return m_is_runing; }
	private:
		int32 m_unit_array_index;
		bool m_is_runing;
		int64 m_tick_time;
		float m_move_time;
		float m_move_speed;
		float m_move_length;
		float m_move_yaw;
		float m_move_length_sum;
		bool  m_be_back;
		bool m_can_break;
		int64 m_unit_pos_tick;
	};
}

#endif //__unit_move_mgrMEENT_H__
