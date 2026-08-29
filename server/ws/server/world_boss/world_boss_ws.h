#ifndef _WS_WORLD_BOSS_WS_H_
#define _WS_WORLD_BOSS_WS_H_

#include "logic/type_def.hpp"
#include "Logic/world_boss_def.hpp"

namespace faith
{
	struct NpcSpawnPointTemplate;
	struct NpcTemplate;

	class world_boss_ws
	{
	public:
		world_boss_ws();
		~world_boss_ws();
		void clear_data();

		void				tick(int32 dt);
		int32				get_spawn_id() { return m_world_boss_data.npc_respawn_point_template_id; };
		void				set_spawn_id(int32 npc_respawn_point_template_id) { m_world_boss_data.npc_respawn_point_template_id = npc_respawn_point_template_id; };

		int32				get_npc_array_index() { return m_world_boss_data.world_boss_npc_array_index; };
		void				set_npc_array_index(int32 npc_array_index) { m_world_boss_data.world_boss_npc_array_index = npc_array_index; };
		bool				is_alive();
		int32				boss_state() { return  m_world_boss_data.boss_daily_refresh; }; //is_alive只设置boss可以打-活的并设置为1，不可以打-打死了，没有打两种状态，此处将不可以打的两种状态区分，设置为0是没有打状态，反之
		void				set_boss_state(int32 refresh_boss_state) { m_world_boss_data.boss_daily_refresh = refresh_boss_state; };
		int32				get_line_id() { return m_world_boss_data.line_id; };
		void				set_line_id(int32 line_id) { m_world_boss_data.line_id = line_id; };

		int32				get_last_respawn_time() { return m_world_boss_data.last_respawn_time; };
		void				set_last_respawn_time(int32 last_respawn_time) { m_world_boss_data.last_respawn_time = last_respawn_time; };

		int32				get_respawn_phase() { return m_world_boss_data.respawn_phase; };
		void				increase_respawn_phase();
		void				reset_respawn_phase();
		bool				is_can_respawn();
		bool				is_need_pre_create() { return m_is_need_pre_create; };
		void				set_pre_create(bool is_pre_create) { m_is_need_pre_create = is_pre_create; };
		void				set_world_boss_dead(const xchar* killer_name);
		s_world_boss_info	get_data_copy() { return m_world_boss_data; };
		void				set_alive(int32 alive_state) { m_world_boss_data.is_alive = alive_state; };

		xchar*				get_killer_name() { return m_world_boss_data.killer_name; };
		void				set_killer_name(const xchar* killer_name) { memcpy(m_world_boss_data.killer_name, killer_name, max_name_size); };
		xchar*				get_first_killer_name() { return m_world_boss_data.first_killer_name; };
		void				set_first_killer_name(const xchar* first_killer_name) { memcpy(m_world_boss_data.first_killer_name, first_killer_name, max_name_size); };
		int32				get_first_killed_time() { return m_world_boss_data.first_killed_time; };
		int32				get_is_get_prize() { return m_world_boss_data.is_get_prize; };
		int32				set_is_get_prize(e_first_kill_prize_type is_get) { return m_world_boss_data.is_get_prize = is_get; };

		void				init_by_info(s_world_boss_info temp_info);
		void				init_by_spawn_id(int32 spawn_template_id);
		void				set_creating(bool is_creating) { m_is_creating = is_creating; };
		bool				get_creating() {return m_is_creating; };
		NpcSpawnPointTemplate*	get_respawn_template_ptr() { return m_spawn_template_ptr; };
		NpcTemplate*		get_npc_template_ptr() { return m_npc_template_ptr; };
		int32				get_hp_per() { return m_hp_per; }
		void				set_hp_per(int32 hp_per) { m_hp_per = hp_per; }

		guid_64				get_boss_guid() { return m_cur_guid; }
		void				set_boss_guid(guid_64 cur_boss_guid) { m_cur_guid = cur_boss_guid; }

		int32				get_boss_random_pos_id() { return m_boss_random_pos_id; }
		void				set_boss_random_pos_id(int32 boss_random_pos_id) { m_boss_random_pos_id = boss_random_pos_id; }

		bool					m_is_recv_get_prize_msg;	//是否接收到了客户端发送的 请求领奖的消息
	private:
		s_world_boss_info		m_world_boss_data;
		NpcSpawnPointTemplate*	m_spawn_template_ptr;
		NpcTemplate*			m_npc_template_ptr;
		bool					m_is_creating;
		bool					m_is_need_pre_create;	//是否需要在服务器启动时即刻创建，不管刷新时间
		int32					m_hp_per;
		guid_64					m_cur_guid;
		int32                   m_boss_random_pos_id;	//世界boss随机位置id
	};
}



#endif
