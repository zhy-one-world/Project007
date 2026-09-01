#ifndef _CHEST_ARRIVAL_MGR_H_
#define _CHEST_ARRIVAL_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/activity_def.hpp"
#include "server/client_session_mgr.hpp"
#include "ws_client.hpp"

namespace faith
{
	// 宝箱降临活动的管理器
	class chest_arrival_mgr
	{
	public:
		const float check_spawn_time_tick_duration = 30.0f; // 单位为秒
		const int32 spawn_chests_announcement_template_id = 93000024; // 刷宝箱时的公告内容模板

		static chest_arrival_mgr& get_instance()
		{
			static chest_arrival_mgr instance;
			return instance;
		}

		explicit chest_arrival_mgr();

		void	tick(const float elapse);
		void	check_should_spawn_tick();
		void	set_chest_wave_flag(int32 wave_index);
		bool	get_chest_wave_flag(int32 wave_index);
		void	spawn_chests(const int32 chest_disappear_time);
		void	send_activity_announcement();
		void	send_message_to_all_cs(const void* data_ptr, size_t data_len);

	private:
		float	m_check_spawn_time_tick;
		bool	m_chest_wave_flag[chest_arrival_activity::spawn_chest_time_arr_size];
	};
}

#endif
