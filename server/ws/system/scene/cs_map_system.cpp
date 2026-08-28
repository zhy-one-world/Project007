
#include "cs_map_system.h"
#include "components/scene/cs_map_component.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "init_unit.h"
#include "time_def.hpp"
#include "server/client_session_mgr.hpp"
#include "world_server.hpp"
#include "parse_msg.h"
#include "server/team/team_ws_mgr.h"
#include "core.hpp"
#include "cs_map_mgr_system.h"
#include "big_player_msg.hpp"
#include "cross_server_harry_msg.hpp"

using namespace hld;

void cs_map_system::start_up(Entity* map_ent, MapTemplate* map_template, int32 map_template_id, int32 line_id, int32 server_uid)
{
	ZoneScoped;
	auto map_cp = map_ent->add_component<cs_map_component>();
	map_cp->m_map_guid = map_ent->getEntityId();
	map_cp->m_map_state = e_map_state_join;
	map_cp->m_map_template_id = map_template_id;
	map_cp->m_map_template_ptr = map_template;
	map_cp->m_line_id = line_id;
	map_cp->m_cs_uid = server_uid;
	map_cp->m_begin_time = time_helper::get_cur_time_new().second;
	if (init_unit::get_map_hold(map_template->Type) || map_template->Type == e_map_type_single_monster)
	{
	}
	else
	{
		auto map_tick_cp = map_ent->add_component<cs_map_tick_component>();
		map_tick_cp->m_empty_end_time = map_cp->m_begin_time + map_in_null_time;
	}
	send_to_cs_create_map(map_cp.get());
}
void cs_map_system::shut_down(Entity* map_ent)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return;
	}
	team_ws_mgr::get_instance().on_team_map_recycle(map_cp->m_own_team_guid, map_cp->m_map_guid);
	cs_map_mgr_system::del_line_info(map_cp->m_map_template_id, map_cp->m_line_id);
	send_to_cs_delete_map(map_cp.get());
}
void cs_map_system::tick(float tick_time)
{
	ZoneScoped;
	g_ecs->each<cs_map_tick_component>([&](Entity* ent, ComponentHandle<cs_map_tick_component> map_tick_cp) -> bool {
		if (time_helper::get_cur_time_new().second < map_tick_cp->m_empty_end_time)
		{
			return true;
		}
		cs_map_mgr_system::remove_map(ent);
		return true;
	});
}

void cs_map_system::send_message_to_all_client(const cs_map_component& map_cp, google::protobuf::Message* net_pro, uint32 header)
{
	ZoneScoped;
	if (net_pro == nullptr)
	{
		CONSOLE_ERROR("net_pro is null header:{}", header);
		return;
	}
	for (auto& it : map_cp.m_role_list)
	{
		auto session_ptr = client_session_mgr::getInstance().get_session(it);
		if (session_ptr == nullptr)
		{
			CONSOLE_ERROR("session_ptr is null role_guid:{}", it);
			continue;
		}
		session_ptr->send_to_client(net_pro, header);
	}
}

void cs_map_system::send_message_to_cs(Entity* map_ent, const void* data_ptr, size_t data_len)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return;
	}
	world_server::getInstance().send_to_cs(map_cp->m_cs_uid, data_ptr, data_len);
}
void cs_map_system::send_message_to_cs(const cs_map_component& map_cp, const void* data_ptr, size_t data_len)
{
	ZoneScoped;
	world_server::getInstance().send_to_cs(map_cp.m_cs_uid, data_ptr, data_len);
}

void cs_map_system::send_message_to_cs_lua(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header)
{
	ZoneScoped;
	packet_s2s* p_s2s = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
	if (p_s2s == nullptr)
	{
		CONSOLE_ERROR("p_s2s is null header:{}", header);
		return;
	}
	send_message_to_cs(map_ent, p_s2s, p_s2s->get_packet_len());
}
void cs_map_system::send_message_to_cs_lua(const cs_map_component& map_cp, google::protobuf::Message* net_pro, uint32 header)
{
	ZoneScoped;
	packet_s2s* p_s2s = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
	if (p_s2s == nullptr)
	{
		CONSOLE_ERROR("p_s2s is null header:{}", header);
		return;
	}
	send_message_to_cs(map_cp, p_s2s, p_s2s->get_packet_len());
}

void cs_map_system::add_unit(Entity* map_ent, const guid_64& role_guid)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return;
	}
	map_cp->m_role_list.insert(role_guid.server_64);
	if (map_cp->m_map_template_ptr->BreakLineLogin > 0)
	{
		map_cp->m_enter_role_list.insert(role_guid.server_64);
	}
	map_cp->m_map_state = e_map_state_game;
	map_ent->remove_component<cs_map_tick_component>();
}
int32 cs_map_system::del_unit(Entity* map_ent, const guid_64& role_guid)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return 0;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return 0;
	}
	map_cp->m_role_list.erase(role_guid.server_64);

	if (map_cp->m_role_list.empty())
	{
		map_cp->m_map_state = e_map_state_return_data;
		if (init_unit::get_map_hold(map_cp->m_map_template_ptr->Type))
		{
			return 0;
		}
		auto map_tick_cp = map_ent->add_component<cs_map_tick_component>();
		if (map_cp->m_map_template_ptr->Type == e_map_type_cross_ladder)
		{
			//当玩家在等待期间退出时，会在等待时间结束结算所以不能直接回收
			map_tick_cp->m_empty_end_time = time_helper::get_cur_time_new().second + minute_tick_time / second_tick_time;
		}
		else
		{
			map_tick_cp->m_empty_end_time = time_helper::get_cur_time_new().second + map_in_null_time;
		}
	}
	return map_cp->m_role_list.size();
}

void cs_map_system::set_map_state(Entity* map_ent, e_map_state map_state)
{
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return;
	}
	map_cp->m_map_state = map_state;
	if (map_state >= e_map_state_over)
	{
		auto map_tick_cp = map_ent->add_component<cs_map_tick_component>();
		map_tick_cp->m_empty_end_time = time_helper::get_cur_time_new().second + map_in_null_time;
	}
}
void cs_map_system::set_own_team_guid(Entity* map_ent, guid_64 team_guid)
{
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return;
	}
	map_cp->m_own_team_guid = team_guid;
}
void cs_map_system::send_to_cs_create_map(const cs_map_component& map_cp)
{
	ZoneScoped;
	ws2cs_map_create req;
	req.map_guid = map_cp.m_map_guid;
	req.line_id = map_cp.m_line_id;
	req.map_template_id = map_cp.m_map_template_id;
	req.map_all_time = map_cp.m_map_template_ptr->TimeRequire;


	send_message_to_cs(map_cp, &req, sizeof(req));
}
void cs_map_system::send_to_cs_delete_map(const cs_map_component& map_cp)
{
	ZoneScoped;
	ws2cs_map_delete req;
	req.map_guid = map_cp.m_map_guid;
	send_message_to_cs(map_cp, &req, sizeof(req));
}	
void cs_map_system::send_to_cs_double_line(const cs_map_component& map_cp)
{
	ZoneScoped;
	ws2cs_map_double_line req;
	req.map_guid = map_cp.m_map_guid;
	req.is_double_line = map_cp.m_is_double_line;
	send_message_to_cs(map_cp, &req, sizeof(req));
}
void cs_map_system::send_big_player(const cs_map_component& map_cp, const s_fake_player_info& fake_player_info, int32 big_type)
{
	ws2cs_create_big_player_statue msg;
	msg.big_player_info = fake_player_info;
	msg.big_type = big_type;

	msg.map_guid = map_cp.m_map_guid;
	send_message_to_cs(map_cp, &msg, sizeof(msg));
}

void cs_map_system::send_harry_player(const cs_map_component& map_cp, const s_fake_player_info& fake_player_info, int32 big_type)
{
	ws2cs_create_harry_player_statue msg;
	msg.big_player_info = fake_player_info;
	msg.big_type = big_type;
	msg.map_guid = map_cp.m_map_guid;
	send_message_to_cs(map_cp, &msg, sizeof(msg));
}
MapTemplate* cs_map_system::get_map_template(const guid_64& map_guid)
{
	ZoneScoped;
	auto map_ent = get_entity(map_guid);
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null map_guid:{}", map_guid.server_64);
		return nullptr;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return nullptr;
	}
	return map_cp->m_map_template_ptr;
}
int32 cs_map_system::get_map_template_id(const guid_64& map_guid)
{
	ZoneScoped;
	auto map_ent = get_entity(map_guid);
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null map_guid:{}", map_guid.server_64);
		return 0;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return 0;
	}
	return map_cp->m_map_template_id;
}
int32 cs_map_system::get_map_template_id(Entity* map_ent)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return 0;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return 0;
	}
	return map_cp->m_map_template_id;
}
int32 cs_map_system::get_unit_count(Entity* map_ent)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return 0;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return 0;
	}
	return map_cp->m_role_list.size();
}	
bool cs_map_system::is_full(const guid_64& map_guid, int32 check_num)
{
	ZoneScoped;
	auto map_ent = get_entity(map_guid);
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null map_guid:{}", map_guid.server_64);
		return true;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return true;
	}
	return (map_cp->m_role_list.size() + check_num) > map_cp->m_map_template_ptr->MaxPlayerCount;
}
bool cs_map_system::is_full(Entity* map_ent, int32 check_num)
{
	ZoneScoped;
	if (map_ent == nullptr)
	{
		CONSOLE_ERROR("map_ent is null");
		return true;
	}
	auto map_cp = map_ent->get_component<cs_map_component>();
	if (map_cp.isValid() == false)
	{
		CONSOLE_ERROR("map_cp is null");
		return true;
	}
	return (map_cp->m_role_list.size() + check_num) > map_cp->m_map_template_ptr->MaxPlayerCount;
}