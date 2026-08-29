/********************************************************************
  created: 2014/08/20
  created: 20:8:2014 9:50
  file base: lua_call_handle
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "lua_globle_data.h"
#include "../logic/unit_man.h"
#include "../logic/skill_manager.h"
#include "../logic/buff_man.h"
#include "../Logic/item_manager.h"
#include "../logic/world_cs.h"
#include "logic/npc.hpp"


namespace faith
{
     /////////////////////////////////////////////////////////////////////////////////////
	 //ÁÙÊ±lua guid±äÁ¿
	 ////////////////////////////////////////////////////////////////////////////////////
	guid_64 g_lua_guid_array[10240];
	int32   g_idx = 0;


	guid_64* get_guid()
	{
		int32 cur_idx = g_idx++;
		if (g_idx == 10240) g_idx = 0;
		guid_64* guid_ptr = g_lua_guid_array + cur_idx;
		return guid_ptr;

	}

	guid_64* get_guid_64(int64 value)
	{
	
		guid_64* guid_ptr = get_guid();
		guid_ptr->server_64 = value;
		return guid_ptr;
	}

	guid_64* get_guid_64(int32 a, int32 b)
	{
		guid_64* guid_ptr = get_guid();
		guid_ptr->A = a;
		guid_ptr->B = b;
		return guid_ptr;
	}

	guid_64* get_guid_64()
	{
		guid_64* guid_ptr = get_guid();
		guid_ptr->server_64 = 0;
		return guid_ptr;
	}



	////////////////////////////////////////////////////////////////////////////////////
	unit*	get_unit_by_guid(guid_64 unit_guid)
	{
		unit& temp_unit = unit_man::get_unit(unit_guid);

		if (temp_unit.is_valid() == false)
			return NULL;
		return &temp_unit;
	}
	npc*	get_npc_by_guid(guid_64 unit_guid)
	{
		npc& temp_npc = unit_man::get_npc(unit_guid);

		if (temp_npc.is_valid() == false)
			return NULL;
		return &temp_npc;
	}
	player*	get_player_by_guid(guid_64 unit_guid)
	{
		player& temp_player = unit_man::get_player(unit_guid);

		if (temp_player.is_valid() == false)
			return NULL;
		return &temp_player;
	}
	unit*	get_unit_by_guid(int32 unit_guid_A, int32 unit_guid_B)
	{
		guid_64 unit_id(unit_guid_A, unit_guid_B);

		unit& temp_unit = unit_man::get_unit(unit_id);

		if (temp_unit.is_valid() == false)
			return NULL;
		return &temp_unit;
	}
	npc*	get_npc_by_guid(int32 unit_guid_A, int32 unit_guid_B)
	{
		guid_64 unit_id(unit_guid_A, unit_guid_B);

		npc& temp_npc = unit_man::get_npc(unit_id);

		if (temp_npc.is_valid() == false)
			return NULL;
		return &temp_npc;
	}
	player*	get_player_by_guid(int32 unit_guid_A, int32 unit_guid_B)
	{
		guid_64 unit_id(unit_guid_A, unit_guid_B);

		player& temp_player = unit_man::get_player(unit_id);

		if (temp_player.is_valid() == false)
			return NULL;
		return &temp_player;
	}
	unit*	get_unit_by_index(int32 unit_index)
	{
		unit& temp_unit = unit_man::get_unit(unit_index);

		if (temp_unit.is_valid() == false)
			return NULL;
		return &temp_unit;
	}
	npc*	get_npc_by_index(int32 unit_index)
	{
		npc& temp_npc = unit_man::get_npc(unit_index);

		if (temp_npc.is_valid() == false)
			return NULL;
		return &temp_npc;
	}
	player*	get_player_by_index(int32 unit_index)
	{
		player& temp_player = unit_man::get_player(unit_index);

		if (temp_player.is_valid() == false)
			return NULL;
		return &temp_player;
	}

	player*	get_player_by_name(std::string name)
	{
		player& temp_player = unit_man::get_player(name);

		if (temp_player.is_valid() == false)
			return NULL;
		return &temp_player;
	}


	skill_set* get_skill_set_by_index(int32 unit_index)
	{
		skill_set& skill_set_ref = skill_manager::get_skill_set(unit_index);

		return &skill_set_ref;

	}
	buff_set* get_buff_set_by_index(int32 unit_index)
	{
		buff_set& buff_set_ref = buff_man::get_buff_set_by_index(unit_index);

		return &buff_set_ref;
	}
	item_set* get_item_set_by_index(int32 unit_index)
	{
		item_set& item_set_ref = item_manager::get_item_set(unit_index);

		return &item_set_ref;
	}
	map_object*	get_map(guid_64 map_guid)
	{
		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(map_guid);
		if (nullptr == map_object_ptr)
		{
			return nullptr;
		}
		return map_object_ptr;
	}

	raid_map_cs* get_raid_map_cs(guid_64 map_guid)
	{
		auto map_object_ptr = world_cs::get_map_by_guid<raid_map_cs>(map_guid);
		if (nullptr == map_object_ptr)
		{
			return nullptr;
		}
		return map_object_ptr;
	}

	std::string guid_transfer_to_string(int32 guid_A, int32 guid_B)
	{
		guid_64 tmp_guid(guid_A, guid_B);
		return tmp_guid.to_string();
	}

	int32 get_guid_A(std::string guid_value)
	{
		guid_64 tmp_guid;
		tmp_guid.parse_guid(guid_value);
		return tmp_guid.A;
	}
	int32 get_guid_B(std::string guid_value)
	{
		guid_64 tmp_guid;
		tmp_guid.parse_guid(guid_value);
		return tmp_guid.B;
	}
	std::string get_str_by_string_template_id(int32 template_id, int32 index)
	{
		return template_manager::get_instance().get_str_by_string_template_id(template_id, index);
	}

	map_record_set *get_map_record_set_by_index(int32 unit_index)
	{
		auto& player_ref = unit_man::get_player(unit_index);
		if (player_ref.is_valid() == false)
		{
			return nullptr;
		}
		return  player_ref.get_map_record_mgr().get();
	}

	cell_server * get_cell_server()
	{
		cell_server& tmp = cell_server::getInstance();
		return &tmp;
	}
}