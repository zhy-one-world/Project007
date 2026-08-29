#include "cs_map_mgr_system.h"
#include "components/scene/cs_map_mgr_component.h"
#include "server_log.hpp"
#include "cs_map_system.h"
#include "template/template_manager.h"
#include "components/scene/cs_map_component.h"
#include "net/net_server_mgr.hpp"
#include "world_server.hpp"
#include "init_unit.h"
#include "game.pb.h"
#include "net.pb.h"

using namespace faith;
cs_map_mgr_component g_cs_mgr_cp;
bool cs_map_mgr_system::start_up()
{
	ZoneScoped;
	return true;
}
void cs_map_mgr_system::shut_down()
{
	ZoneScoped;
}
void cs_map_mgr_system::tick(float tick_time)
{
	ZoneScoped;
	cs_map_system::tick(tick_time);
}

Entity* cs_map_mgr_system::add_cs_map(int32 map_template_id, int32 line_id, int32 cs_uid)
{
	ZoneScoped;
	auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
	if (nullptr == map_template)
	{
		CONSOLE_ERROR("map_template is null, map_template_id:{}", map_template_id);
		return nullptr;
	}
	auto map_ent = create_entity();
	cs_map_system::start_up(map_ent, map_template, map_template_id, line_id, cs_uid);

	g_cs_mgr_cp.m_cs_map[cs_uid].insert(map_ent);

	return map_ent;
}
void cs_map_mgr_system::del_cs_map(Entity* map_ent)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto cs_map_cp = map_ent->get_component<cs_map_component>();
	auto cs_uid = cs_map_cp->m_cs_uid;
	cs_map_system::shut_down(map_ent);

	g_cs_mgr_cp.m_cs_map[cs_uid].erase(map_ent);
	destroy_entity(map_ent);
}
void cs_map_mgr_system::init_map_by_world(int32 conn_index)
{
	ZoneScoped;

	//if (GAMECONFIG->CsWorldMap.size() % 2 != 0)
	//{
	//	CONSOLE_ERROR("GAMECONFIG->CsWorldMap size is error size:{}", GAMECONFIG->CsWorldMap.size());
	//	return;
	//}
	//for (uint32 i = 0; i < GAMECONFIG->CsWorldMap.size();)
	//{
	//	int32 map_template_id = GAMECONFIG->CsWorldMap[i];

	//	int32 line_count = GAMECONFIG->CsWorldMap[i + 1];

	//	for (int32 line = 0; line < line_count; ++line)
	//	{
	//		init_map_by_main(map_template_id, conn_index);
	//	}
	//	i = i + 2;
	//}
}
Entity* cs_map_mgr_system::init_map_by_main(int32 map_template_id, int32 conn_index)
{
	ZoneScoped;
		int32 line_index = -1;
	auto line_info_it = g_cs_mgr_cp.m_line_info_map.find(map_template_id);
	if (line_info_it != g_cs_mgr_cp.m_line_info_map.end())
	{
		line_info_it->second += 1;
		line_index = line_info_it->second;
	}
	else
	{
		g_cs_mgr_cp.m_line_info_map.insert({ map_template_id, 1 });
		line_index = 1;
	}

	auto cs_map_ent = add_cs_map(map_template_id, line_index, conn_index);
	return cs_map_ent;
}
Entity* cs_map_mgr_system::init_map_by_battle(int32 map_template_id, int32 conn_index)
{
	ZoneScoped;
	if (conn_index < 0)
	{
		auto cs_ptr = net_server_mgr::getInstance().get_peer_by_type(e_server_type_cs);
		if (nullptr == cs_ptr)
		{
			CONSOLE_ERROR("cs_ptr is null");
			return nullptr;
		}
		conn_index = cs_ptr->get_conn_index();
	}
	auto cs_map_ent = add_cs_map(map_template_id, 0, conn_index);
	return cs_map_ent;
}

void cs_map_mgr_system::init_map_by_cross_world(int32 conn_index)
{
	ZoneScoped;
	//GATE上只需要创建跨服地图，所以单独处理，不创建其他地图
	std::vector<int32> map_id_arr = GAMECONFIG->GateServerMapIDArr;
	if (map_id_arr.size() <= 0)
	{
		return;
	}
	for (int32 i = 0; i < map_id_arr.size(); i++)
	{
		init_map_by_main(map_id_arr[i], conn_index);
	}

}

void cs_map_mgr_system::remove_map_by_uid(uint32 conn_index)
{
	ZoneScoped;

	auto it = g_cs_mgr_cp.m_cs_map.find(conn_index);
	if (it != g_cs_mgr_cp.m_cs_map.end())
	{
		auto cs_map_arr = it->second;
		for (auto map_ent : cs_map_arr)
		{
			del_cs_map(map_ent);
		}
		it->second.clear();
	}
	else
	{
		CONSOLE_ERROR("conn_index is not find conn_index:{}", conn_index);
	}
}

void cs_map_mgr_system::remove_map_by_guid(const guid_64& map_guid)
{
	ZoneScoped;
	auto cs_map_ent = get_entity(map_guid);
	if (nullptr == cs_map_ent)
	{
		CONSOLE_ERROR("cs_map_ent is null map_guid:{}", map_guid.server_64);
		return;
	}
	del_cs_map(cs_map_ent);
}
void cs_map_mgr_system::remove_map(Entity* map_ent)
{
	ZoneScoped;
	del_cs_map(map_ent);
}
int32 cs_map_mgr_system::remove_unit_from_map(const guid_64& map_guid, const guid_64& role_guid)
{
	auto cs_map_ent = get_entity(map_guid);
	if (nullptr == cs_map_ent)
	{
		CONSOLE_ERROR("cs_map_ent is null map_guid:{}", map_guid.server_64);
		return 0;
	}
	return cs_map_system::del_unit(cs_map_ent, role_guid);
}

void cs_map_mgr_system::on_unit_leave_map(int32 map_template_id, const guid_64& role_guid)
{
	MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
	if (map_template_ptr && map_template_ptr->ReenterCD > 0)
	{
		auto& rm_units_stamp = g_cs_mgr_cp.m_rm_units_stamp_data[map_template_id];
		rm_units_stamp[role_guid.server_64] = time_helper::get_cur_time_new().second;
	}
}

bool cs_map_mgr_system::check_unit_can_enter_map(int32 map_template_id, const guid_64& role_guid)
{
	ZoneScoped;
	//gate服上不允许传送到其他地图
	if (world_server::getInstance().get_cross_id() == world_server::getInstance().get_server_id())
	{
		if (false == init_unit::is_map_cross_server(map_template_id, world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity)) && false == init_unit::is_map_all_server(map_template_id))
		{
			CONSOLE_ERROR("gate server can not transfer to other map map_template_id:{}", map_template_id);
			return false;
		}
	}
	auto map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
	if (nullptr == map_template_ptr)
	{
		CONSOLE_ERROR("map_template_ptr is nullptr map_template_id:{}", map_template_id);
		return false;
	}
	if (g_cs_mgr_cp.m_rm_units_stamp_data.size() <= 0)
	{
		CONSOLE_INFO("g_cs_mgr_cp.m_rm_units_stamp_data.size() <= 0");
		return true;
	}

	auto it_map_rm_data = g_cs_mgr_cp.m_rm_units_stamp_data.find(map_template_id);
	if (it_map_rm_data == g_cs_mgr_cp.m_rm_units_stamp_data.end())
	{
		CONSOLE_INFO("it_map_rm_data == m_rm_units_stamp_data.end()");
		return true;
	}

	auto& rm_units_stamp = it_map_rm_data->second;
	auto it_player_leave_stamp = rm_units_stamp.find(role_guid.server_64);
	if (it_player_leave_stamp == rm_units_stamp.end())
	{
		CONSOLE_INFO("it_player_leave_stamp == rm_units_stamp.end()");
		return true;
	}

	uint32 now_stamp = time_helper::get_cur_time_new().second;
	int32 pass_sec = now_stamp - it_player_leave_stamp->second;
	if (pass_sec < map_template_ptr->ReenterCD)
	{
		CONSOLE_ERROR("pass_sec < map_template_ptr->ReenterCD map_template_id:{} pass_sec:{} map_template_ptr->ReenterCD:{}", map_template_id, pass_sec, map_template_ptr->ReenterCD);
		return false;
	}
	CONSOLE_INFO("end function");
	return true;
}

Entity* cs_map_mgr_system::get_map_ws_by_min(int32 map_template_id, int32 trans_unit_num)
{
	ZoneScoped;
	MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
	if (nullptr == map_template_ptr)
	{
		CONSOLE_ERROR("map_template_ptr is nullptr map_template_id:{}", map_template_id);
		return nullptr;
	}
	if (init_unit::get_map_open_new(map_template_ptr->Type) == false)
	{
		ZoneScoped;
		int32 min_num = init_player_max;
		Entity* min_map_ent = nullptr;
		g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
			//ws上地图状态默认为e_map_state_join 有人进入为e_map_state_game 当cs发消息同步为e_map_state_in_game及之后的状态后不可再加入玩家
			auto role_num = cs_map_cp->m_role_list.size();
			if (cs_map_cp->m_map_template_id == map_template_id
				&& cs_map_cp->m_map_state < e_map_state_in_game
				&& min_num > role_num)
			{
				if (role_num + trans_unit_num > cs_map_cp->m_map_template_ptr->MaxPlayerCount)
				{
					return true;
				}
				if (role_num < cs_map_cp->m_map_template_ptr->MinNextLine)
				{
					min_map_ent = ent;
					return false;
				}
				min_map_ent = ent;
				min_num = role_num;
			}
			return true;
			});
		if (nullptr == min_map_ent && init_unit::get_map_create_full(map_template_ptr->Type))
		{
			min_map_ent = init_map_by_battle(map_template_id);
		}
		return min_map_ent;
	}
	else
	{
		return init_map_by_battle(map_template_id);
	}
}
Entity* cs_map_mgr_system::get_map_ws_by_max(int32 map_template_id)
{
	ZoneScoped;
	int32 max_num = -1;
	Entity* max_map_ent = nullptr;
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
		if (map_template_id == cs_map_cp->m_map_template_id && max_num < cs_map_cp->m_role_list.size())
		{
			max_num = cs_map_cp->m_role_list.size();
			max_map_ent = ent;
		}
		return true;
	});
	return max_map_ent;
}
Entity* cs_map_mgr_system::get_map_ws_by_line(int32 map_template_id, int32 line_id)
{
	ZoneScoped;
	Entity* map_ent = nullptr;
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
		if (map_template_id == cs_map_cp->m_map_template_id && line_id == cs_map_cp->m_line_id)
		{
			map_ent = ent;
			return false;
		}
		return true;
	});
	return map_ent;
}
void cs_map_mgr_system::get_line_vec(int32 map_template_id, line_id_vec& line_vec)
{
	ZoneScoped;
	int32 cur_line_id = -1;
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {

		if (map_template_id != cs_map_cp->m_map_template_id)
		{
			return true;
		}
		if (cur_line_id == cs_map_cp->m_line_id)
		{
			return true;
		}
		cur_line_id = cs_map_cp->m_line_id;
		line_vec.push_back(cs_map_cp->m_line_id);
		line_vec.push_back(cs_map_cp->m_role_list.size());
		line_vec.push_back(cs_map_cp->m_map_template_ptr->MaxPlayerCount);
		line_vec.push_back(cs_map_cp->m_is_double_line);
		return true;
	});
}
void cs_map_mgr_system::del_line_info(int32 map_template_id, int32 line_id)
{
	ZoneScoped;
	auto it = g_cs_mgr_cp.m_line_info_map.find(map_template_id);
	if (it != g_cs_mgr_cp.m_line_info_map.end())
	{
		it->second -= 1;
	}
}

void cs_map_mgr_system::transfer_to_map(guid_64 role_guid, int32 map_template_id, guid_64 map_guid, int32 war_index)
{
	ZoneScoped;
	client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
	if (nullptr == client_session_ptr)
	{
		return;
	}
	transfer_to_map(client_session_ptr, map_template_id, map_guid, war_index);
}

void cs_map_mgr_system::transfer_to_map(client_session* client_session_ptr, int32 map_template_id, guid_64 map_guid, int32 war_index, int32 server_id)
{
	ZoneScoped;
	if (nullptr == client_session_ptr)
	{
		return;
	}

	if (false == client_session_ptr->is_self_server() && false == init_unit::is_map_other_server(map_template_id) && false == init_unit::is_map_cross_server(map_template_id, world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity)) && false == init_unit::is_map_all_server(map_template_id))
	{
		return;
	}//非本服且传送地图ID不为大世界并且不为跨服地图，则不允许传送


	ws2cs_transfer_to_map transfer_to_map_msg;
	transfer_to_map_msg.role_guid = client_session_ptr->get_role_guid();
	transfer_to_map_msg.map_template_id = map_template_id;
	transfer_to_map_msg.map_guid = map_guid;
	transfer_to_map_msg.war_idx = war_index;
	transfer_to_map_msg.server_id = server_id;
	client_session_ptr->send_to_cs(&transfer_to_map_msg, sizeof(transfer_to_map_msg));
}

void cs_map_mgr_system::send_map_error_to_client(guid_64 role_guid, e_error_code error_code)
{
	ZoneScoped;
	client_session* session = client_session_mgr::getInstance().get_session(role_guid);
	if (nullptr == session)
	{
		return;
	}
	send_map_error_to_client(session, error_code);
}

void cs_map_mgr_system::send_map_error_to_client(client_session* client_session_ptr, e_error_code error_code)
{
	ZoneScoped;
	if (nullptr == client_session_ptr)
	{
		return;
	}

	game_proto_transfer_error map_error_msg;
	map_error_msg.set_error_code((int32)error_code);
	client_session_ptr->send_to_client(&map_error_msg, e_msgindex_s2c_transfer_error);
}

void cs_map_mgr_system::send_one_map_info_to_client(guid_64 role_guid, int32 map_template_id, int64 map_info[])
{
	ZoneScoped;
	client_session* session = client_session_mgr::getInstance().get_session(role_guid);
	if (nullptr == session)
	{
		return;
	}

	game_proto_get_one_map_info_end get_one_map_info_end_msg;
	get_one_map_info_end_msg.set_map_template_id(map_template_id);

	for (int32 i = 0; i < map_info_arr_max_num; i++)
	{
		get_one_map_info_end_msg.add_map_info_arr(map_info[i]);
	}

	session->send_to_client(&get_one_map_info_end_msg, e_msgindex_s2c_get_one_map_info_end);
}

void  cs_map_mgr_system::send_message_to_all_client_by_map_id(int32 map_template_id, google::protobuf::Message* net_pro, uint32 header)
{
	ZoneScoped;
	if (map_template_id == 0|| net_pro == nullptr)
	{
		return;
	}
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
		if (cs_map_cp->m_map_template_id == map_template_id)
		{
			cs_map_system::send_message_to_all_client(cs_map_cp.get(), net_pro, header);
		}
		return true;
	});
}

void cs_map_mgr_system::send_big_player(int32 map_template_id, s_fake_player_info& fake_player_info, int32 big_type)
{
	ZoneScoped;
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
		cs_map_system::send_big_player(cs_map_cp.get(), fake_player_info, big_type);
		return true;
		});
}

void cs_map_mgr_system::send_harry_player(int32 map_template_id, s_fake_player_info& fake_player_info, int32 idex)
{
	ZoneScoped;
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
		if (cs_map_cp->m_map_template_id == map_template_id)
		{
			cs_map_system::send_harry_player(cs_map_cp.get(), fake_player_info, idex);
		}
		return true;
		});
}
void cs_map_mgr_system::show_big_map_unit_num()
{
	ZoneScoped;
	g_ecs->each<cs_map_component>([&](Entity* ent, ComponentHandle<cs_map_component> cs_map_cp) -> bool {
		CONSOLE_INFO("map unit num:{} max:{} line id:{}", cs_map_cp->m_role_list.size(), cs_map_cp->m_map_template_ptr->MaxPlayerCount, cs_map_cp->m_line_id);
		return true;
	});
}

