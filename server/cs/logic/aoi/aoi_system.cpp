
#include "aoi_system.h"
#include "aoi_set.h"
#include "server_log.hpp"
#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "utility/init_unit.h"

using namespace hld;

//static cs2fep_aoi_msg g_aoi_msg;
static int32 g_aoi_tower_ary[init_player_max];

void aoi_system::start_up(Entity* map_ent, MapTemplate* map_template)
{
	ZoneScoped;
	auto aoi_cp = map_ent->add_component<aoi_set>();
	aoi_cp->create_aoi(map_template->MapStartX, map_template->MapEndX, map_template->MapStartY, map_template->MapEndY, map_template->TowerWidth, map_template->TowerLength);
}
void aoi_system::shut_down(Entity* map_ent)
{
	ZoneScoped;
	map_ent->remove_component<aoi_set>();
}
 const unit_index_map& aoi_system::get_watch_all(Entity* map_ent, const fvector& map_pos)
{
	auto aoi_cp = map_ent->get_component<aoi_set>();
	return aoi_cp->get_watcher_all(map_pos);
}
 const unit_index_map& aoi_system::get_watch_all(const guid_64& map_guid, const fvector& map_pos)
 {
	 ZoneScoped;
	 Entity* map_ent = get_entity(map_guid);
	 return get_watch_all(map_ent, map_pos);
 }
void aoi_system::get_watch_by_type(Entity* map_ent, const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32* unit_array, int32& watch_array_num)
{
	ZoneScoped;
	if (nullptr == unit_array)
	{
		return;
	}
	auto aoi_cp = map_ent->get_component<aoi_set>();
	//memset(unit_array, 0, sizeof(int32) * AOI_TOWER_UNIT_MAX);
	aoi_cp->get_watcher_by_type(map_pos_old, map_pos_new, move_type, unit_array, watch_array_num);
}
void aoi_system::get_watch_by_type(const guid_64& map_guid, const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32* unit_array, int32& watch_array_num)
{
	ZoneScoped;
	Entity* map_ent = get_entity(map_guid);
	get_watch_by_type(map_ent, map_pos_old, map_pos_new, move_type, unit_array, watch_array_num);
}
void  aoi_system::send_message_aoi_all(unit_index_map& player_map, google::protobuf::Message* net_pro, uint32 header)
{
	ZoneScoped;

	unit_index_map_const_it it_end = player_map.end();
	for (unit_index_map_const_it it = player_map.begin(); it != it_end; ++it)
	{
		int32 playr_num = 0;
		const int32& aoi_unit_index = it->first;
		player& player_ref = unit_man::get_player(aoi_unit_index);
		if (player_ref.get_client_uid().is_valid())
		{
			player_ref.send_message_to_self(net_pro, header);
			//g_aoi_msg.client_uid[playr_num++] = player_ref.get_client_uid();
		}
		//if (playr_num > 0)
		//{
		//	g_aoi_msg.data_num = playr_num;
		//	g_aoi_msg.wheader = e_msg_index_cs2fep_aoi_msg;
		//	g_aoi_msg.header = header;
		//	g_aoi_msg.data_size = net_pro->ByteSize();
		//	if (g_aoi_msg.data_size > packege_length_max)
		//	{
		//		CONSOLE_INFO("aoi_system::send_message_aoi_by_type packge is big data_size {}, header {}", g_aoi_msg.data_size, header);
		//		return;
		//	}
		//	if (net_pro->SerializeToArray(g_aoi_msg.data, sizeof(g_aoi_msg.data)))
		//	{
		//		cell_server::getInstance().broadcast(&g_aoi_msg, g_aoi_msg.get_pak_length(), e_server_type_fep);
		//	}
		//	else
		//	{
		//		CONSOLE_INFO("header {} ", header);
		//	}
		//}
	}
}
void aoi_system::send_message_aoi_all(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos, const int32& array_index, bool use_length)
{
	ZoneScoped;
	unit& unit_ref = unit_man::get_unit(array_index);
	if (unit_ref.is_valid() == false)
	{
		return;
	}
	const unit_index_map& aoi_tower_watch = get_watch_all(map_ent, map_pos);
	if (aoi_tower_watch.empty())
	{
		return;
	}

	int32 playr_num = 0;
	unit_index_map_const_it it_end = aoi_tower_watch.end();
	for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != it_end; ++it)
	{
		const int32& aoi_unit_index = it->first;
		if (aoi_unit_index == array_index || aoi_unit_index >= npc_arrary_index_begin)
		{
			continue;
		}
		player& player_ref = unit_man::get_player(aoi_unit_index);
		if (player_ref.aoi_is_watch_all_data(array_index) == false)
		{
			continue;
		}
		if (use_length)
		{
			if (init_unit::is_in_distance(player_ref.get_new_map_pos().unit_location, unit_ref.get_new_map_pos().unit_location, 2500) == false)
			{
				continue;
			}
		}
		if (player_ref.get_client_uid().is_valid())
		{
			player_ref.send_message_to_self(net_pro, header);
			//g_aoi_msg.client_uid[playr_num++] = player_ref.get_client_uid();
		}
	}
	//if (playr_num > 0)
	//{
	//	g_aoi_msg.data_num = playr_num;
	//	g_aoi_msg.wheader = e_msg_index_cs2fep_aoi_msg;
	//	g_aoi_msg.header = header;
	//	g_aoi_msg.data_size = net_pro->ByteSize();
	//	if (g_aoi_msg.data_size > packege_length_max)
	//	{
	//		CONSOLE_INFO("aoi_system::send_message_aoi_by_type packge is big data_size = {} header = {}", g_aoi_msg.data_size, header);
	//		return;
	//	}
	//	if (net_pro->SerializeToArray(g_aoi_msg.data, sizeof(g_aoi_msg.data)))
	//	{
	//		cell_server::getInstance().broadcast(&g_aoi_msg, g_aoi_msg.get_pak_length(), e_server_type_fep);
	//		//unit_man::log_msg_info(header, net_pro->ByteSize());
	//	}
	//	else
	//	{
	//		CONSOLE_INFO("header : {}", header);
	//	}
	//}
}
void aoi_system::send_message_aoi_all(const guid_64& map_guid, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos, const int32& array_index, bool use_length)
{
	ZoneScoped;
	Entity* map_ent = get_entity(map_guid);
	send_message_aoi_all(map_ent, net_pro, header, map_pos, array_index, use_length);
}
void aoi_system::send_message_aoi_by_type(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32 array_index)
{
	ZoneScoped;
	unit& unit_ref = unit_man::get_unit(array_index);
	if (unit_ref.is_valid() == false)
	{
		return;
	}
	int32 watch_array_num = 0;
	get_watch_by_type(map_ent, map_pos_old, map_pos_new, move_type, g_aoi_tower_ary, watch_array_num);
	if (watch_array_num == 0)
	{
		return;
	}
	//memset(&g_aoi_msg, 0, sizeof(g_aoi_msg));
	int32 playr_num = 0;
	for (int32 i = 0; i < watch_array_num; i++)
	{
		int32& aoi_unit_index = g_aoi_tower_ary[i];
		if (aoi_unit_index == array_index || aoi_unit_index >= npc_arrary_index_begin)
		{
			continue;
		}
		player& player_ref = unit_man::get_player(aoi_unit_index);
		if (player_ref.is_valid())
		{
			if (player_ref.aoi_is_watch_all_data(array_index) == false)
			{
				continue;
			}
			if (player_ref.get_client_uid().is_valid())
			{
				player_ref.send_message_to_self(net_pro, header);
				//g_aoi_msg.client_uid[playr_num++] = player_ref.get_client_uid();
			}
		}
	}
	//if (playr_num > 0)
	//{
	//	g_aoi_msg.data_num = playr_num;
	//	g_aoi_msg.wheader = e_msg_index_cs2fep_aoi_msg;
	//	g_aoi_msg.header = header;
	//	g_aoi_msg.data_size = net_pro->ByteSize();
	//	if (g_aoi_msg.data_size > packege_length_max)
	//	{
	//		CONSOLE_INFO("aoi_system::send_message_aoi_by_type packge is big data_size = {} header = {}", g_aoi_msg.data_size, header);
	//		return;
	//	}
	//	if (net_pro->SerializeToArray(g_aoi_msg.data, sizeof(g_aoi_msg.data)))
	//	{
	//		cell_server::getInstance().broadcast(&g_aoi_msg, g_aoi_msg.get_pak_length(), e_server_type_fep);
	//		//unit_man::log_msg_info(header, net_pro->ByteSize());
	//	}
	//	else
	//	{
	//		CONSOLE_INFO("header : {}", header);
	//	}
	//}
}
void aoi_system::send_message_aoi_by_type(const guid_64& map_guid, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32 array_index)
{
	ZoneScoped;
	Entity* map_ent = get_entity(map_guid);
	send_message_aoi_by_type(map_ent, net_pro, header, map_pos_old, map_pos_new, move_type, array_index);
}
bool aoi_system::add_object(Entity* map_ent, const fvector& unit_location, const int32 unit_index)
{
	ZoneScoped;
	auto aoi_cp = map_ent->get_component<aoi_set>();
	if (aoi_cp.isValid())
	{
		return aoi_cp->add_object(unit_location, unit_index);
	}
	return false;
}
bool aoi_system::remove_object(Entity* map_ent, const fvector& unit_location, const int32 unit_index, bool is_dead)
{
	ZoneScoped;
	auto aoi_cp = map_ent->get_component<aoi_set>();
	if (aoi_cp.isValid())
	{
		return aoi_cp->remove_object(unit_location, unit_index, is_dead);
	}
	return false;
}
bool aoi_system::update_object(Entity* map_ent, const fvector& map_pos_old, const fvector& map_pos_new, const int32& unit_index)
{
	ZoneScoped;
	auto aoi_cp = map_ent->get_component<aoi_set>();
	if (aoi_cp.isValid())
	{
		return aoi_cp->update_object(map_pos_old, map_pos_new, unit_index);
	}
	return false;
}