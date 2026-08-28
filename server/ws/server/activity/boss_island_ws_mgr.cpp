#include "boss_island_ws_mgr.h"
#include "template/template_manager.h"
#include <boss_island_msg.hpp>
#include "ws_client.hpp"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "world_server.hpp"
#include "game.pb.h"
#include "net.pb.h"

namespace hld
{
	boss_island_ws_mgr::boss_island_ws_mgr()
	{

	}

	void boss_island_ws_mgr::init_manager()
	{

	}

	void boss_island_ws_mgr::update_boss_island_info(const s_gain_treasure_boss_info& boss_info)
	{
		auto ite = m_boss_info_map.find(boss_info.boss_spawn_point_template_id);
		if (ite == m_boss_info_map.end())
		{
			m_boss_info_map.insert({ boss_info.boss_spawn_point_template_id, boss_info });
			send_boss_island_info_to_all_player(boss_info.boss_spawn_point_template_id);
		}
		else
		{
			//boss刚复活或者刚死亡 消息发送给全服玩家
			if ((!ite->second.boss_guid.is_valid() && boss_info.boss_guid.is_valid())
				|| !boss_info.boss_guid.is_valid())
			{
				ite->second = boss_info;
				send_boss_island_info_to_all_player(boss_info.boss_spawn_point_template_id);
			}
			//boss归属改变只发给对应地图的玩家
			else
			{
				ite->second = boss_info;
				send_boss_island_info_to_map(boss_info.boss_spawn_point_template_id);
			}
		}
	}

	void boss_island_ws_mgr::send_boss_island_info_to_map(int32 npc_spawn_id)
	{
		NpcSpawnPointTemplate* boss_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_id);
		if (nullptr == boss_spawn_point_template_ptr)
		{
			return;
		}
		game_proto_gain_treasure_boss_info_one boss_info_msg;
		if (fill_boss_info_one_msg(boss_info_msg, npc_spawn_id)  == false)
		{
			return;
		}
		client_session_mgr::getInstance().send_message_to_map(boss_spawn_point_template_ptr->MapId, &boss_info_msg, e_msgindex_s2c_gain_treasure_boss_info_one);
	}
	void boss_island_ws_mgr::send_boss_island_info_to_all_player(int32 npc_spawn_id)
	{
		if (false == gate_proxy::getInstance().is_gate_run()
			|| (true == gate_proxy::getInstance().is_gate_run() && world_server::getInstance().is_sky_island_server()))
		{
			game_proto_gain_treasure_boss_info_one boss_info_msg;
			if (fill_boss_info_one_msg(boss_info_msg, npc_spawn_id) == false)
			{
				return;
			}
			client_session_mgr::getInstance().send_message_to_all_client(&boss_info_msg, e_msgindex_s2c_gain_treasure_boss_info_one);
		}
		if (true == gate_proxy::getInstance().is_gate_run()   && world_server::getInstance().is_sky_island_server())
		{
			game_proto_gain_treasure_boss_info_one boss_info_msg;
			if (fill_boss_info_one_msg(boss_info_msg, npc_spawn_id) == false)
			{
				return;
			}
			cross::send_msg_to_ws(guid_64(), 0, e_msgindex_s2c_boss_island_info_one, &boss_info_msg, sizeof(boss_info_msg));
		}
	}
	bool boss_island_ws_mgr::fill_boss_info_one_msg(game_proto_gain_treasure_boss_info_one & boss_info_msg, int32 npc_spawn_id)
	{
		auto ite = m_boss_info_map.find(npc_spawn_id);
		if (ite == m_boss_info_map.end())
		{
			return false;
		}
		boss_info_msg.set_boss_spawn_point_template_id(npc_spawn_id);
		boss_info_msg.set_boss_guid(ite->second.boss_guid.server_64);
		boss_info_msg.set_next_refresh_stamp(ite->second.next_refresh_stamp);
		boss_info_msg.set_first_player_name(ite->second.player_info[0].player_name);
		boss_info_msg.set_map_type(e_map_type_boss_island);
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			if (!ite->second.player_info[i].player_guid.is_valid())
			{
				continue;
			}
			boss_info_msg.add_player_guid(ite->second.player_info[i].player_guid.server_64);
		}
		return 1;
	}
	void boss_island_ws_mgr::send_boss_island_info_all(client_session * session)
	{
		if (nullptr == session)
		{
			return;
		}
		if (false == gate_proxy::getInstance().is_gate_run()
			|| (true == gate_proxy::getInstance().is_gate_run() && world_server::getInstance().is_sky_island_server()))
		{
			game_proto_gain_treasure_boss_info_all boss_info_all_msg;
			for (auto ite = m_boss_info_map.begin(); ite != m_boss_info_map.end(); ite++)
			{
				game_proto_gain_treasure_boss_info_one* boss_info = boss_info_all_msg.add_boss_info();
				if (nullptr == boss_info)
				{
					continue;
				}
				fill_boss_info_one_msg(*boss_info, ite->second.boss_spawn_point_template_id);
				if (boss_info_all_msg.ByteSize() > packege_length_max)
				{
					session->send_to_client(&boss_info_all_msg, e_msgindex_s2c_gain_treasure_boss_info_all);
					boss_info_all_msg.clear_boss_info();
				}
			}
			session->send_to_client(&boss_info_all_msg, e_msgindex_s2c_gain_treasure_boss_info_all);
		}
		else if (true == gate_proxy::getInstance().is_gate_run() )
		{
			gate2ws_server_id msg;
			msg.server_id = session->get_role_info_data(e_role_info_server_id);
			cross::send_msg_to_ws(session->get_role_guid(), world_server::getInstance().get_cross_id(), e_msgindex_s2c_gain_treasure_boss_info_all, &msg, sizeof(msg));
		}
	}

	void boss_island_ws_mgr::send_ws2ws_all_boss_island_msg(int32 server_id, guid_64 role_guid)
	{
		game_proto_gain_treasure_boss_info_all boss_info_all_msg;
		for (auto ite = m_boss_info_map.begin(); ite != m_boss_info_map.end(); ite++)
		{
			game_proto_gain_treasure_boss_info_one* boss_info = boss_info_all_msg.add_boss_info();
			if (nullptr == boss_info)
			{
				continue;
			}
			fill_boss_info_one_msg(*boss_info, ite->second.boss_spawn_point_template_id);
			if (boss_info_all_msg.ByteSize() > packege_length_max)
			{
				cross::send_msg_to_ws(role_guid, server_id, e_msgindex_s2c_gain_treasure_boss_info_one, &boss_info_all_msg);
				boss_info_all_msg.clear_boss_info();
			}
		}
		cross::send_msg_to_ws(role_guid, server_id, e_msgindex_s2c_gain_treasure_boss_info_one, &boss_info_all_msg);
	}

	void boss_island_ws_mgr::fill_boss_info_all_msg(game_proto_gain_treasure_boss_info_all& msg)
	{
		for (auto ite = m_boss_info_map.begin(); ite != m_boss_info_map.end(); ite++)
		{
			game_proto_gain_treasure_boss_info_one* boss_info = msg.add_boss_info();
			if (nullptr == boss_info)
			{
				continue;
			}
			fill_boss_info_one_msg(*boss_info, ite->second.boss_spawn_point_template_id);
		}
	}
	void boss_island_ws_mgr::send_all_msg_to_client(const void* data_ptr, size_t data_len, client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		session->send_to_client(data_ptr, data_len, e_msgindex_s2c_gain_treasure_boss_info_all);
	}
	void boss_island_ws_mgr::send_boss_island_info_to_all_player_ws(const void* data_ptr, size_t data_len)
	{
		client_session_mgr::getInstance().send_message_to_all_client_data(data_ptr, data_len, e_msgindex_s2c_gain_treasure_boss_info_one);
	}
}
