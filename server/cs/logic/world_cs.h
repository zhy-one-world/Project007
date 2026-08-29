/********************************************************************
	created:	2015/03/21
	created:	21:3:2015   21:31
	file base:	world_cs
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

#include "scene/map_object.h"
#include "Logic/fake_player_def.hpp"


namespace faith
{
	class world_cs
	{
	public:
		static void heart_tick(const int64& new_time);
	public:
		static void create_map_by_template(const guid_64& map_guid, int32 map_template_id, int32 line_id, int32 map_all_time);
		static void destroy_map(const guid_64& map_guid);
		template<typename T>
		static void get_all_map_by_template_id(std::vector<T*>& ret_map_set, uint32 map_template_id);
		template<typename T>
		static void get_all_map_by_type(std::vector<T*>& ret_map_set, uint32 map_type);
		template<typename T>
		static T* get_map_by_template_id(uint32 map_template_id);
		template<typename T>
		static T* get_map_by_guid(const guid_64& map_guid);
		template<typename T>
		static T* get_map_by_index(const int32 map_array_index);

	public:
		static bool add_player(Entity* map_ent, player* player_ptr);
		static void del_player(Entity* map_ent, player* player_ptr);
		static void player_dead(Entity* map_ent, player* player_ptr);
		static void player_load_data_over(Entity* map_ent, player* player_ptr);
		static void player_enter_scene(Entity* map_ent, player* player_ptr);
		static void player_leave_scene(Entity* map_ent, player* player_ptr);
		static void player_reconnect(Entity* map_ent, player* player_ptr);
		static void add_npc(Entity* map_ent, npc* npc_ptr);
		static void del_npc(Entity* map_ent, npc* npc_ptr, bool is_dead);
		static void npc_dead(Entity* map_ent, npc* npc_ptr);
		static void unit_be_damage(Entity* map_ent, int32 attack_index, int32 be_attack_index, double damage_value);
	public:
		static npc*	spawn_npc(int32 npc_spawn_template_id, Entity* map_ent, player* player_ptr = nullptr);
		static npc*	spawn_npc(int32 npc_template_id, const s_map_pos& born_pos, Entity* map_ent, player* player_ptr = nullptr, int32 npc_spawn_template_id = -1);
		static npc*	spawn_npc(int32 npc_template_id, const s_map_pos& born_pos, const s_unit_identifier& unit_identifier, Entity* map_ent, player* player_ptr = nullptr, int32 show_wing_id = 0);
		static void	create_drop_box(Entity* map_ent, std::vector< s_item_template_info >& item_list, s_map_pos& init_pos, s_map_pos& init_target_pos, player* player_ptr, std::vector<int32>& item_id_array, int32 drop_npc_temp_id, int32 drop_id, bool cost_time = false, bool is_need_show_get_new = false);
		static int32 get_npc_init_level(int32 npc_id, const map_object& create_map);
		static npc*	spawn_fake_player_npc(const guid_64& map_guid, int32 npc_template_id, const s_fake_player_info& fake_data_ptr, s_map_pos const& position, int32 npc_type = e_monster_type_player_data);
		static npc*	spawn_player_data_robot_npc(Entity* map_ent, int32 npc_template_id, const s_fake_player_info& fake_data_ptr, s_map_pos const& position, int32 npc_type = e_monster_type_player_data);

		static void	c2cs_req_map_tigger_volume(uint32 connindex, const void*data_ptr, size_t data_len);
		static void	c2cs_jump_raid_animation(uint32 connindex, const void*data_ptr, size_t data_len);
		static void	add_aoi(Entity* map_ent, const int32& unit_index, const fvector& location);
		static void	del_aoi(Entity* map_ent, const fvector& unit_location, const int32 unit_index, bool is_dead);
		static void	update_aoi_pos(Entity* map_ent, const int32& unit_index, const fvector& old_location, const fvector& new_location);
		static void	create_big_player_statue(const s_fake_player_info& player_info, const int32& big_type, const guid_64& map_guid);
		static void	create_harry_player_statue(const s_fake_player_info& player_info, const int32& big_type, const guid_64& map_guid);
		static void	send_notice_with_param(faith::guid_64 sender_guid, std::string sender_name,std::string content_text, std::vector<int32> param_array, int32 param_type, int32 chat_type, int32 sender_template_id = 0, int32 notice_id = 0);
		static void ws2cs_get_map_info_from_cs_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void set_fake_player_idex(int32 map_line_id, int32 fakse_type, int32 idex);
	};
	template<typename T>
	void world_cs::get_all_map_by_template_id(std::vector<T*>& ret_map_set, uint32 map_template_id)
	{
		//for (int32 i = 0; i < init_player_max; ++i)
		//{
		//	auto map_object_ptr = get_map_by_index<T>(i);
		//	if (map_object_ptr && map_object_ptr->get_map_template_id() == map_template_id)
		//	{
		//		ret_map_set.push_back(map_object_ptr);
		//	}
		//}
	}
	template<typename T>
	void world_cs::get_all_map_by_type(std::vector<T*>& ret_map_set, uint32 map_type)
	{
		//for (int32 i = 0; i < init_player_max; ++i)
		//{
		//	auto map_object_ptr = get_map_by_index<T>(i);
		//	if (map_object_ptr && map_object_ptr->get_map_type() == map_type)
		//	{
		//		ret_map_set.push_back(map_object_ptr);
		//	}
		//}
	}
	template<typename T>
	T* world_cs::get_map_by_template_id(uint32 map_template_id)
	{
		//for (int32 i = 0; i < init_player_max; ++i)
		//{
		//	auto map_object_ptr = get_map_by_index<T>(i);
		//	if (map_object_ptr && map_object_ptr->get_map_template_id() == map_template_id)
		//	{
		//		return map_object_ptr;
		//	}
		//}
		return nullptr;
	}
	template<typename T>
	T* world_cs::get_map_by_guid(const guid_64& map_guid)
	{
		//return get_map_by_index<T>(map_guid.B);
		return nullptr;

	}
	template<typename T>
	T* world_cs::get_map_by_index(const int32 map_array_index)
	{
		//if (map_array_index <= 0 || map_array_index >= init_player_max)
		//{
		//	return nullptr;
		//}
		//return   dynamic_cast<T*>(m_map_cs_array[map_array_index]);
		return nullptr;
	}
}
