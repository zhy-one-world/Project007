/*
	author:: Allen_wl
*/

#ifndef _CRYSTAL_FAIRYLAND_MGR_H_
#define _CRYSTAL_FAIRYLAND_MGR_H_

#include "logic/type_def.hpp"

namespace faith
{
	enum e_error_code;
	struct ActivityCommonConfigTemplate;

	class crystal_fairyland_mgr
	{
		crystal_fairyland_mgr();
		~crystal_fairyland_mgr();
	public:
		static crystal_fairyland_mgr& get_instance()
		{
			static crystal_fairyland_mgr instance;
			return instance;
		}

	public:
		void	tick(int64& new_time);
		void	clear_data();
		void	init_data();
		void	double_time_send_notice_to_all();
		bool	is_during_double_time() { return m_during_double_time; };
		void	set_open_double_time(bool is_open) { m_during_double_time = is_open;};
		void	check_open_double_time();
		void	set_notice_flag(bool notice_flag) { m_had_notice = notice_flag; };
		bool	is_activity_open();
		e_error_code	is_can_join();
		guid_64	get_valid_shui_jing_map(int32& map_war_idx);
		void	recv_other_ws_transfer_gate_map(guid_64 role_guid, int32 server_id);

	public:
		bool	m_had_notice;
		int64	m_next_tick;
		bool	m_during_double_time;
		ActivityCommonConfigTemplate* m_act_template_ptr;
		guid_64	m_crystal_fairyland_map_guid;
		
	};


}

#endif
