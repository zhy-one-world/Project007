#ifndef _WS_BOSS_ISLAND_WS_MGR_H_
#define _WS_BOSS_ISLAND_WS_MGR_H_

#include "Logic/boss_island_def.hpp"
#include "Logic/gain_treasure_def.hpp"
#include "../client_session_mgr.hpp"
namespace faith
{
	class game_proto_gain_treasure_boss_info_one;
	class game_proto_gain_treasure_boss_info_all;

	class boss_island_ws_mgr
	{
	public:
		explicit boss_island_ws_mgr();
		static boss_island_ws_mgr& get_instance()
		{
			static boss_island_ws_mgr instance;
			return instance;
		}
		void								init_manager();
		void								update_boss_island_info(const s_gain_treasure_boss_info& boss_info);
		void								send_boss_island_info_to_map(int32 npc_spawn_id);
		void								send_boss_island_info_to_all_player(int32 npc_spawn_id);
		bool								fill_boss_info_one_msg(game_proto_gain_treasure_boss_info_one& boss_info_msg,int32 npc_spawn_id);
		void								send_boss_island_info_all(client_session* session);
		void								fill_boss_info_all_msg(game_proto_gain_treasure_boss_info_all& msg);
		void								send_all_msg_to_client(const void* data_ptr, size_t data_len, client_session* session);
		void								send_boss_island_info_to_all_player_ws(const void* data_ptr, size_t data_len);
		void								send_ws2ws_all_boss_island_msg(int32 server_id, guid_64 role_guid);
	private:
		std::map<int32, s_gain_treasure_boss_info>	m_boss_info_map;
	};
}

#endif