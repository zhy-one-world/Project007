/********************************************************************
  created: 2019年1月15日
  file base: cross_server_pk_ws_mgr
  file ext: hpp
  author: zhangshuo
  purpose: 
*********************************************************************/
#ifndef _CROSS_SERVER_PK_WS_MGR_WS_H_
#define _CROSS_SERVER_PK_WS_MGR_WS_H_

#include "server/client_session_mgr.hpp"
#include "Logic/cross_server_pk_def.hpp"
#include "cross_server_pk_msg.hpp"

namespace hld
{
	class game_proto_cross_server_one_island_msg;
	class game_proto_pk_one_player_msg;

	class cross_server_pk_ws_mgr
	{
	public:
		static cross_server_pk_ws_mgr& get_instance()
		{
			static cross_server_pk_ws_mgr instance;
			return instance;
		}
		void						init_manager();
		void						tick(int64 time_new);
		void						clear_data();
		void						tick_3sec(int64	time_new);

		e_error_code				is_can_join(guid_64 role_guid);
		e_error_code				is_can_join_with_cross_gate(int32 class_type);
		e_error_code				is_can_join_with_cross(guid_64 role_guid);

		void						send_to_gate_transfer_map(int32 class_type, guid_64 role_guid, int32 activity_type, int32 server_id = 0, guid_64 legion_guid = guid_64());
		void						recv_other_ws_transger_gate_map(int32 class_type, guid_64 role_guid, int32 server_id);
		void						transfer_to_map_with_gate_msg(guid_64 role_guid, int32 error_ret, int32 map_temp_id, guid_64 map_guid, int32 war_idex, int32 server_id);

		bool						is_in_game_time();
		
		void						get_pk_king_map_guid(int32 class_type, guid_64& map_guid, int32& war_idex);
		void						init_cross_server_pk_map();
		
		bool						is_have_pk_map();

		void						save_cs2ws_island_msg(const s_pk_island_all_msg& island_all_msg, int32 war_count, guid_64 winner_guid, int32 is_end_war, bool is_need_send_to_client, int32 pk_count);
		void						save_dp2ws_island_msg(const s_pk_island_simple_msg& island_simple_msg);

		void						ws2dp_save_island_msg();
		void						ws2dp_save_simple_island_msg(const s_pk_island_simple_msg& island_msg);

		void						req_load_island_msg();

		void						recv_dp_msg_to_ws(s_dp2ws_island_simple_msg& simple_msg);

		void						clear_all_island_msg();
		void						send_clear_all_island_msg_to_client();
		void						clear_all_big_player_and_title();

		void						send_island_msg_one_to_client(const s_pk_island_simple_msg& island_simple_msg, guid_64 winner_guid, int32 is_end_war, int32 pk_count);
		void						send_island_msg_all_to_client(client_session* session_ptr = nullptr);
		void						send_ws2ws_all_pk_msg(int32 server_id, guid_64 role_guid);
		void						send_pk_to_all_player_ws(const void* data_ptr, size_t data_len);

		void						fill_one_island_msg(game_proto_cross_server_one_island_msg& island_msg, const s_pk_island_simple_msg& island_simple_msg);
		void						fill_pk_one_player_msg(game_proto_pk_one_player_msg& player_msg, const s_pk_island_simple_player_msg& sim_player_msg);

		int32						get_map_temp_id_with_class_type(int32 class_type);

		void						set_cross_server_pk_map_end(int32 class_type);

		void						send_all_msg_to_client(const void* data_ptr, size_t data_len, client_session* session);

		void						send_winner_fake_player_to_other_server(int32 server_id = 0);
		void						set_winner_fake_player_with_gate_msg(int32 data_num, const s_pk_player_msg* pk_player_msg);

		void						set_is_create_fake_player(bool need_create) { m_is_create_fake_player = need_create; };
		bool						get_is_create_fake_player() { return m_is_create_fake_player; };


		s_pk_player_msg&			get_pk_player_msg(int32 idex);

		void						set_winner_fake_player_gate_msg();

		void						set_load_all_fake_player(int32 idex);
		bool						get_is_load_all_fake_player();
		void						load_fake_player_with_last_time();//加载剩余没load的

		void						set_need_create_fake_player(bool need_create) { m_need_create_fake_player = need_create; };
	private:
		explicit cross_server_pk_ws_mgr();

		guid_64						m_cross_pk_map_guid[max_branch_type];
		int32						m_cross_pk_map_id_arr[max_branch_type];
		uint32						m_last_pk_king_stamp;

		int64						m_last_tick_time;
		int64						m_timer;
		int64						m_last_3sec_tick_time;

		s_pk_island_all_msg			m_island_all_msg;

		bool						m_is_clear_db;
		bool						m_is_cur_map_end[max_branch_type];

		s_pk_player_msg				m_pk_player_msg[max_branch_type];//四个第一名
		s_pk_player_msg				m_empty_player_msg;

		bool						m_is_create_fake_player = false;
		bool						m_can_craete_fake_player = false;//已连接上DP 可以开始创建
		
		bool						m_recv_gate_msg = false;	//	GATE是否发来数据

		int64						m_last_load_fake_player_time;//下次load时间，防止因对方服务器未启动，导致读取失败

		bool						m_is_load_all_fake_player[max_branch_type];

		bool						m_need_create_fake_player = false;// CS发来数据 开始创建
	};
}

#endif
