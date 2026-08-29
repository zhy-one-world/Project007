/********************************************************************
  created: 2017-7-17 17:36:24
  file base: lua_globle_data
  file ext: h
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_LUA_GLOBLE_DATA_H_
#define _FAITH_LUA_GLOBLE_DATA_H_
#include "logic/type_def.hpp"
#include "cell_server.hpp"

namespace faith
{
	class unit;
	class npc;
	class player;
	class skill_set;
	class buff_set;
	class item_set;
	class map_object;
	class raid_map_cs;
	class map_record_set;

	guid_64* get_guid_64(int64 value);
	guid_64* get_guid_64(int32 a, int32 b);
	guid_64* get_guid_64();
	unit*	get_unit_by_guid(guid_64 unit_guid);
	npc*	get_npc_by_guid(guid_64 unit_guid);
	player*	get_player_by_guid(guid_64 unit_guid);
	unit*	get_unit_by_guid(int32 unit_guid_A, int32 unit_guid_B);
	npc*	get_npc_by_guid(int32 unit_guid_A, int32 unit_guid_B);
	player*	get_player_by_guid(int32 unit_guid_A, int32 unit_guid_B);
	unit*	get_unit_by_index(int32 unit_index);
	npc*	get_npc_by_index(int32 unit_index);
	player*	get_player_by_index(int32 unit_index);
	player* get_player_by_name(std::string name);
	skill_set* get_skill_set_by_index(int32 unit_index);
	buff_set* get_buff_set_by_index(int32 unit_index);
	item_set* get_item_set_by_index(int32 unit_index);
	map_object*	get_map(guid_64 map_guid);
	raid_map_cs* get_raid_map_cs(guid_64 map_guid);
	std::string get_str_by_string_template_id(int32 template_id, int32 index);

	std::string guid_transfer_to_string(int32 guid_A, int32 guid_B);
	int32	get_guid_A(std::string guid_value);
	int32   get_guid_B(std::string guid_value);


	map_record_set* get_map_record_set_by_index(int32 unit_index);

	cell_server * get_cell_server();




}

#endif