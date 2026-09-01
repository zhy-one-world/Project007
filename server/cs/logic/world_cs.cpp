/********************************************************************
	created:	2015/03/21
	created:	21:3:2015   21:34
	file base:	world_cs
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "aoi/aoi_system.h"
#include "base/ecs_world.h"
#include "cell_server.hpp"
#include "internet/character.pb.h"
#include "components/scene/base_map_component.h"
#include "connection_mgr.hpp"
#include "logic/item_set.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "npc.hpp"
#include "scene/map_cs.h"
#include "server_log.hpp"
#include "system/scene/base_map_system.h"
#include "system/scene/battle_map_system.h"
#include "system/scene/box_map_system.h"
#include "system/scene/element_map_system.h"
#include "system/scene/npc_refrush_system.h"
#include "system/scene/wave_monster_system.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include <Utility/parse_msg.h>
#include "internal/chat_msg.hpp"
#include "internal/core.hpp"


namespace faith
{
	static int32 m_pk_player_idex_array[map_lind_max_num][e_fake_type_max];

	void world_cs::heart_tick(const int64& new_time)
	{
		ZoneScoped;
		box_map_system::heart_tick(new_time);
		element_map_system::heart_tick(new_time);
		battle_map_system::heart_tick(new_time);
		npc_refrush_system::heart_tick(new_time);
		wave_monster_system::heart_tick(new_time);
	}
	void world_cs::create_map_by_template(const guid_64& map_guid, int32 map_template_id, int32 line_id, int32 map_all_time)
	{
		ZoneScoped;
		CONSOLE_INFO("map_guid = {} map_template_id = {} line_id = {} map_all_time = {}", map_guid.server_64, map_template_id, line_id, map_all_time);
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			CONSOLE_ERROR("map_template_ptr is nullptr map_template_id = {}", map_template_id);
			return;
		}
		auto map_ent = get_entity(map_guid);
		if (map_ent)
		{
			CONSOLE_ERROR("map_guid = {} map_template_id = {} line_id = {} map_all_time = {}", map_guid.server_64, map_template_id, line_id, map_all_time);
			return;
		}
		map_ent = create_entity(map_guid);
		aoi_system::start_up(map_ent, map_template_ptr);
		base_map_system::start_up(map_ent, line_id, map_template_ptr);
		box_map_system::start_up(map_ent, map_template_ptr);
		element_map_system::start_up(map_ent, map_template_ptr);
		battle_map_system::start_up(map_ent, map_template_ptr);
	}
	void world_cs::destroy_map(const guid_64& map_guid)
	{
		ZoneScoped;
		CONSOLE_INFO("map_guid = {}", map_guid.server_64);
		auto map_ent = get_entity(map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("map_ent is nullptr map_guid = {}", map_guid.server_64);
			return;
		}
		npc_refrush_system::shut_down(map_ent);
		box_map_system::shut_down(map_ent);
		element_map_system::shut_down(map_ent);
		battle_map_system::shut_down(map_ent);
		base_map_system::shut_down(map_ent);
		aoi_system::shut_down(map_ent);
		destroy_entity(map_ent);
	}
	
	void world_cs::create_drop_box(Entity* map_ent, std::vector< s_item_template_info >& item_list, s_map_pos& init_pos, s_map_pos& init_target_pos, player* player_ptr, std::vector<int32>& item_id_array, int32 drop_npc_temp_id, int32 drop_id, bool cost_time, bool is_need_show_get_new)
	{
		ZoneScoped;
		if (nullptr == player_ptr)
		{
			CONSOLE_ERROR("create_drop_box player_ptr is invalid");
			return;
		}

		auto base_map_cp = map_ent->get_component<base_map_component>();

		std::vector<s_item_template_info> item_tuple_array;
		item_tuple_array.clear();

		int32 list_size = item_list.size();
		for (int32 i = 0; i < list_size; i++)
		{
			int32 temp_item_id = item_list[i].m_item_id;
			int32 lock_state = item_list[i].m_item_num;
			if (temp_item_id <= 0)
				continue;

			npc& temp_npc = unit_man::get_empty_npc();
			if (temp_npc.is_valid())
			{
				s_map_pos temp_pos = init_pos;
				init_unit::range_pos(temp_pos);
				int32 DropModeID = 0;
				ItemTemplate* line_ptr = GET_TEMPLATE(ItemTemplate, temp_item_id);
				if (nullptr != line_ptr)
				{
					if (line_ptr->DropNpcMesh > 0)
					{
						DropModeID = line_ptr->DropNpcMesh;
					}
					else
					{
						continue;
					}
				}
				temp_pos.unit_location.z += 50;
				if (temp_npc.init_by_template_data(DropModeID, temp_pos, map_ent, base_map_cp->m_map_id, base_map_cp->m_line_id, 0))
				{
					temp_npc.set_owner_type(e_dropbox_owner_player);
					temp_npc.set_owner(player_ptr->get_identifier());
					temp_npc.set_item_id(temp_item_id);//设置掉落物品id
					temp_npc.set_item_lock(lock_state);
					temp_npc.set_spawn_point_id(0);
					temp_npc.set_drop_npc_temp_id(drop_npc_temp_id);
					add_npc(map_ent, &temp_npc);

					item_id_array.push_back(temp_item_id);
					item_tuple_array.push_back({ temp_item_id, 1, lock_state });

					if (player_ptr->get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_added_drop))
					{
						time_limit_activity_object& activity_obj_ptr = player_ptr->get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_added_drop);
						if (activity_obj_ptr.is_valid()
							&& time_limit_activity_added_drop_drop_template_id == drop_id)
						{
							server_log::time_limit_activity_log(player_ptr->get_unit_info_inst(), player_ptr->get_third_info(), activity_obj_ptr.get_branch_template_id(), e_time_limit_activity_type_added_drop, 0, 0, temp_item_id, 1, player_ptr->get_login_type(), 0, 0, activity_obj_ptr.get_time_limit_activity_branch_temp().AttributeId);
						}
					}
				}
				else
				{
					unit_man::remove_npc(temp_npc.get_array_index(), false);
				}
			}
		}

		NpcTemplate *m_npc_template_ptr = GET_TEMPLATE(NpcTemplate, drop_npc_temp_id);
		if (m_npc_template_ptr == nullptr)
			return;
		if (m_npc_template_ptr->NpcType == e_unit_type_monster && (m_npc_template_ptr->SubType == e_monster_type_boss || m_npc_template_ptr->SubType == e_monster_type_world_boss || m_npc_template_ptr->SubType == e_monster_type_gold_army))
		{
			//扣除挑战次数并且兼顾活跃度等数据的修改
			if (cost_time)
				player_ptr->delete_boss_challenge_times_after_drop();

			//boss掉落日志
			server_log::boss_kill_drop_log(player_ptr->get_third_info(), player_ptr->get_unit_info_inst(), item_list, drop_npc_temp_id, player_ptr->get_login_type());

			set_log_var(log_head);
			player_ptr->get_log_common_head_info(log_head);
			server_log::serverBossKillDrop(log_head, drop_npc_temp_id, item_list);
		}
		if ((m_npc_template_ptr->ShowDropList && m_npc_template_ptr->IsUseDamageList == 0) || is_need_show_get_new)
			player_ptr->get_item_set().get_item_send_promp_msg_to_client(item_tuple_array, false, drop_npc_temp_id);
	}
	void world_cs::add_aoi(Entity* map_ent, const int32& unit_index, const fvector& location)
	{
		ZoneScoped;
		aoi_system::add_object(map_ent, location, unit_index);
	}
	void world_cs::del_aoi(Entity* map_ent, const fvector& unit_location, const int32 unit_index, bool is_dead)
	{
		ZoneScoped;
		aoi_system::remove_object(map_ent, unit_location, unit_index, is_dead);
	}
	void world_cs::update_aoi_pos(Entity* map_ent, const int32& unit_index, const fvector& old_location, const fvector& new_location)
	{
		ZoneScoped;
		aoi_system::update_object(map_ent, old_location, new_location, unit_index);
	}

	bool world_cs::add_player(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;
		base_map_system::on_player_add(map_ent, player_ptr);
		box_map_system::on_player_add(map_ent, player_ptr);
		element_map_system::on_player_add(map_ent, player_ptr);
		battle_map_system::on_player_add(map_ent, player_ptr);
		return true;
	}

	void world_cs::del_player(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;
		base_map_system::on_player_delete(map_ent, player_ptr);
		box_map_system::on_player_delete(map_ent, player_ptr);
		element_map_system::on_player_delete(map_ent, player_ptr);
		battle_map_system::on_player_delete(map_ent, player_ptr);
	}

	void world_cs::player_dead(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;
		base_map_system::on_player_dead(map_ent, player_ptr);
		box_map_system::on_player_dead(map_ent, player_ptr);
		element_map_system::on_player_dead(map_ent, player_ptr);
		battle_map_system::on_player_dead(map_ent, player_ptr);
	}

	void world_cs::player_load_data_over(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;
		base_map_system::on_player_load_data_over(map_ent, player_ptr);
		box_map_system::on_player_load_data_over(map_ent, player_ptr);
		element_map_system::on_player_load_data_over(map_ent, player_ptr);
		battle_map_system::on_player_load_data_over(map_ent, player_ptr);
	}
	void world_cs::player_enter_scene(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;

		aoi_system::add_object(map_ent, player_ptr->get_new_map_pos().unit_location, player_ptr->get_array_index());

		player_ptr->summon_all_pet();

		base_map_system::on_player_enter_scene(map_ent, player_ptr);
		box_map_system::on_player_enter_scene(map_ent, player_ptr);
		element_map_system::on_player_enter_scene(map_ent, player_ptr);
		battle_map_system::on_player_enter_scene(map_ent, player_ptr);
	}
	void world_cs::player_leave_scene(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;
		player_ptr->map_in_out_hp_process();
		player_ptr->kill_all_pet();

		base_map_system::on_player_leave_scene(map_ent, player_ptr);
		box_map_system::on_player_leave_scene(map_ent, player_ptr);
		element_map_system::on_player_leave_scene(map_ent, player_ptr);
		battle_map_system::on_player_leave_scene(map_ent, player_ptr);

		aoi_system::remove_object(map_ent, player_ptr->get_new_map_pos().unit_location, player_ptr->get_array_index(), false);

		{
			//set_log_var(log_head);
			//player_ptr->get_log_common_head_info(log_head);
			//server_log::serverMapChange(log_head, get_map_template_id(), map_type, map_sub_type, faith::e_map_change_leave, m_map_template_ptr->Difficulty);
		}
	}
	void world_cs::player_reconnect(Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;

		aoi_system::add_object(map_ent, player_ptr->get_new_map_pos().unit_location, player_ptr->get_array_index());

		base_map_system::on_player_reconnect(map_ent, player_ptr);
		box_map_system::on_player_reconnect(map_ent, player_ptr);
		element_map_system::on_player_reconnect(map_ent, player_ptr);
		battle_map_system::on_player_reconnect(map_ent, player_ptr);
	}
	void  world_cs::add_npc(Entity* map_ent, npc* npc_ptr)
	{
		ZoneScoped;
		npc_ptr->set_show_map(true);
		const s_map_pos& unit_pos = npc_ptr->get_new_map_pos();
		aoi_system::add_object(map_ent, unit_pos.unit_location, npc_ptr->get_array_index());

		base_map_system::on_npc_add(map_ent, npc_ptr);
		box_map_system::on_npc_add(map_ent, npc_ptr);
		element_map_system::on_npc_add(map_ent, npc_ptr);
		battle_map_system::on_npc_add(map_ent, npc_ptr);
	}
	void world_cs::del_npc(Entity* map_ent, npc* npc_ptr, bool is_dead)
	{
		ZoneScoped;
		wave_monster_system::on_npc_delete(map_ent, npc_ptr);
		base_map_system::on_npc_delete(map_ent, npc_ptr, is_dead);
		box_map_system::on_npc_delete(map_ent, npc_ptr, is_dead);
		element_map_system::on_npc_delete(map_ent, npc_ptr, is_dead);
		battle_map_system::on_npc_delete(map_ent, npc_ptr, is_dead);
	}
	void  world_cs::unit_be_damage(Entity* map_ent, int32 attack_index, int32 be_attack_index, double damage_value)
	{
		ZoneScoped;
		base_map_system::on_unit_be_damage(map_ent, attack_index, be_attack_index, damage_value);
		box_map_system::on_unit_be_damage(map_ent, attack_index, be_attack_index, damage_value);
		element_map_system::on_unit_be_damage(map_ent, attack_index, be_attack_index, damage_value);
		battle_map_system::on_unit_be_damage(map_ent, attack_index, be_attack_index, damage_value);
	}

	void world_cs::npc_dead(Entity* map_ent, npc* npc_ptr)
	{
		ZoneScoped;	
		npc_refrush_system::on_npc_dead(map_ent, npc_ptr);
		base_map_system::on_npc_dead(map_ent, npc_ptr);
		box_map_system::on_npc_dead(map_ent, npc_ptr);
		element_map_system::on_npc_dead(map_ent, npc_ptr);
		battle_map_system::on_npc_dead(map_ent, npc_ptr);
	}
	npc* world_cs::spawn_npc(int32 npc_spawn_template_id, Entity* map_ent, player* player_ptr)
	{
		ZoneScoped;
		NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_template_id);
		if (npc_spawn_point_template_ptr == nullptr)
		{
			CONSOLE_ERROR("npc_spawn_point_template_ptr is nullptr npc_spawn_template_id = {}", npc_spawn_template_id);
			return nullptr;
		}
		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
		if (npc_template_ptr == nullptr)
		{
			CONSOLE_ERROR("npc_template_ptr is nullptr npc_spawn_template_id = {} npc_id:{}", npc_spawn_template_id, npc_spawn_point_template_ptr->TemplateId);
			return nullptr;
		}
		auto base_map_cp = map_ent->get_component<base_map_component>();
		//map_object_ptr->caclu_now_player_avg_level();
		//if (dynamic_lv < 0)
		//{
		//	dynamic_lv = get_npc_init_level(npc_spawn_point_template_ptr->TemplateId, *map_object_ptr);
		//}

		npc& temp_npc = unit_man::get_empty_npc();
		if (temp_npc.is_valid())
		{
			fvector location(npc_spawn_point_template_ptr->PosX, npc_spawn_point_template_ptr->PosY, npc_spawn_point_template_ptr->PosZ);
			frotator rotation(npc_spawn_point_template_ptr->Roll, npc_spawn_point_template_ptr->Pitch, npc_spawn_point_template_ptr->Yaw);
			s_map_pos bornPos(location, rotation);

			fvector location_n(npc_spawn_point_template_ptr->NextPosX, npc_spawn_point_template_ptr->NextPosY, npc_spawn_point_template_ptr->NextPosZ);
			s_map_pos patrol_pos(location_n);

			int32 npc_index = temp_npc.get_array_index();
			if (temp_npc.init_by_template_data(npc_spawn_point_template_ptr->TemplateId, bornPos, map_ent, base_map_cp->m_map_id, base_map_cp->m_line_id, player_ptr, patrol_pos))
			{
				temp_npc.set_line_id(base_map_cp->m_line_id);
				temp_npc.set_spawn_point_id(npc_spawn_template_id);
				add_npc(map_ent, &temp_npc);
				return &temp_npc;
			}
			else
			{
				unit_man::remove_npc(npc_index, false);
			}
		}
		return nullptr;
	}

	npc * world_cs::spawn_npc(int32 npc_temp_id, const s_map_pos & born_pos, Entity* map_ent, player* player_ptr, int32 npc_spawn_template_id)
	{
		ZoneScoped;
		if (npc_temp_id <= 0)
		{
			CONSOLE_ERROR("npc_temp_id is invalid npc_temp_id = {}", npc_temp_id);
			return nullptr;			
		}
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("spawn_npc map_ent is nullptr");
			return nullptr;
		}
		auto npc_template = GET_TEMPLATE(NpcTemplate, npc_temp_id);
		if (nullptr == npc_template)
		{
			CONSOLE_ERROR("npc_template is nullptr npc_temp_id = {}", npc_temp_id);
			return nullptr;
		}
		auto base_map_cp = map_ent->get_component<base_map_component>();

		npc& temp_npc = unit_man::get_empty_npc();
		if (temp_npc.is_valid())
		{
			int32 npc_index = temp_npc.get_array_index();
			if (temp_npc.init_by_template_data(npc_temp_id, born_pos, map_ent, base_map_cp->m_map_id, base_map_cp->m_line_id, player_ptr, born_pos))
			{
				temp_npc.set_line_id(base_map_cp->m_line_id);
				if (npc_spawn_template_id != -1)
				{
					temp_npc.set_spawn_point_id(npc_spawn_template_id);
				}
				else
				{
					temp_npc.set_spawn_point_id(npc_temp_id);
				}
				add_npc(map_ent, &temp_npc);
				return &temp_npc;
			}
			else
			{
				unit_man::remove_npc(npc_index, false);
			}
		}
		return nullptr;
	}

	npc* world_cs::spawn_npc(int32 npc_template_id, const s_map_pos& born_pos, const s_unit_identifier& unit_identifier, Entity* map_ent, player* player_ptr, int32 show_wing_id)
	{
		ZoneScoped;
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("spawn_npc map_ent is nullptr");
			return nullptr;
		}
		auto base_map_cp = map_ent->get_component<base_map_component>();
		npc& temp_npc = unit_man::get_empty_npc();
		if (temp_npc.is_valid())
		{
			int32 npc_index = temp_npc.get_array_index();
			if (temp_npc.init_by_template_data(npc_template_id, born_pos, map_ent, base_map_cp->m_map_id, base_map_cp->m_line_id, player_ptr, s_map_pos(), show_wing_id))
			{
				if (unit_identifier.is_valid())
				{
					temp_npc.set_owner_type(e_dropbox_owner_player);
					temp_npc.set_owner(unit_identifier);
				}
				add_npc(map_ent, &temp_npc);
				return &temp_npc;
			}
			else
			{
				unit_man::remove_npc(npc_index, false);
			}
		}
		return nullptr;
	}

	int32 world_cs::get_npc_init_level(int32 npc_id, const map_object& create_map)
	{
		ZoneScoped;
		int32 init_lv = 0;
		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_id);
		if (nullptr == npc_template_ptr)
		{
			return init_lv;
		}
		if (npc_template_ptr->InitLevelInfo.size() < 2)
		{
			return init_lv;
		}
		
		switch (npc_template_ptr->InitLevelInfo[0])
		{
		case e_npc_level_type_player_lv:
			init_lv = create_map.get_player_avg_lv() + npc_template_ptr->InitLevelInfo[1];
			break;
		case e_npc_level_type_server_lv:
			init_lv = cell_server::getInstance().get_server_avg_level() + npc_template_ptr->InitLevelInfo[1];
			break;
		case e_npc_level_type_legion_average_lv:
			init_lv = create_map.get_legion_average_lv() + npc_template_ptr->InitLevelInfo[1];
			break;
		case e_npc_level_type_cur_player_lv:
			init_lv = create_map.get_now_player_avg_level() + npc_template_ptr->InitLevelInfo[1];
			break;
		default:
			break;
		}

		return init_lv;
	}
	
	npc* world_cs::spawn_fake_player_npc(const guid_64& map_guid, 
		int32 npc_template_id, 
		const s_fake_player_info& fake_data_ptr, 
		s_map_pos const& position,
		int32 npc_type)
	{
		ZoneScoped;
		auto map_ent = get_entity(map_guid);
		if (nullptr == map_ent)
		{
			return nullptr;
		}
		auto base_map_cp = map_ent->get_component<base_map_component>();
		npc& temp_npc = unit_man::get_empty_npc();
		if (temp_npc.is_valid())
		{
			int32 npc_index = temp_npc.get_array_index();
			if (temp_npc.init_fake_player_data(npc_template_id, fake_data_ptr, map_ent, position, base_map_cp->m_map_id, npc_type))
			{
				add_npc(map_ent, &temp_npc);
				return &temp_npc;
			}
			else
			{
				unit_man::remove_npc(npc_index, false);
			}
		}

		return nullptr;
	}

	npc * world_cs::spawn_player_data_robot_npc(Entity* map_ent, int32 npc_template_id, const s_fake_player_info& fake_data_ptr, s_map_pos const & position, int32 npc_type)
	{
		ZoneScoped;
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("spawn_player_data_robot_npc map_ent is nullptr");
			return nullptr;
		}
		auto base_map_cp = map_ent->get_component<base_map_component>();
		npc& temp_npc = unit_man::get_empty_npc();
		if (temp_npc.is_valid())
		{
			int32 npc_index = temp_npc.get_array_index();
			if (temp_npc.init_player_data_robot(npc_template_id, fake_data_ptr, map_ent, position, base_map_cp->m_map_id, npc_type))
			{
				add_npc(map_ent, &temp_npc);
				return &temp_npc;
			}
			else
			{
				unit_man::remove_npc(npc_index, false);
			}
		}
		return nullptr;
	}

	void world_cs::c2cs_req_map_tigger_volume(uint32 connindex, const void*data_ptr, size_t data_len)
	{
		ZoneScoped;
		character_proto_map_tigger_volume msg;
		parse_msg::getInstance().parse_message_new(&msg, data_ptr, data_len);

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);

		player& player_ptr = unit_man::get_player(unit_index);
		if (false == player_ptr.is_valid())
		{
			return;
		}

		auto map_object_ptr = get_map_by_guid<map_object>(player_ptr.get_map_guid());
		if (nullptr == map_object_ptr)
		{
			return;
		}
		//if (map_object_ptr->get_map_type() == e_map_type_arena)
		//{
		//	arena_map_cs* arena_map_cs_ptr = dynamic_cast<arena_map_cs*>(map_object_ptr);
		//	if (nullptr == arena_map_cs_ptr)
		//	{
		//		return;
		//	}
		//	arena_map_cs_ptr->req_create_enemy();
		//	return;
		//}
		//raid_map_cs* raid_map_cs_ptr =dynamic_cast<raid_map_cs*>(map_object_ptr);
		//if (nullptr == raid_map_cs_ptr)
		//{
		//	return;
		//}
		//raid_map_cs_ptr->map_tigger_volume(msg.tigger_volume_id(), 0, true, unit_index);
	}

	void world_cs::c2cs_jump_raid_animation(uint32 connindex, const void*data_ptr, size_t data_len)
	{
		ZoneScoped;
		character_proto_jump_raid_animation msg;
		parse_msg::getInstance().parse_message_new(&msg, data_ptr, data_len);

		int32 unit_index = 0;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, unit_index);

		player& player_ptr = unit_man::get_player(unit_index);
		if (false == player_ptr.is_valid())
		{
			return;
		}

		//auto map_object_ptr = get_map_by_guid<raid_map_cs>(player_ptr.get_map_guid());
		//if (nullptr == map_object_ptr)
		//{
		//	return;
		//}
		//map_object_ptr->map_refresh_wave_immediately();
	}

	void world_cs::create_big_player_statue(const s_fake_player_info& player_info, const int32& big_type, const guid_64& map_guid)
	{
		ZoneScoped;
		//if (big_type < 0 || big_type >= e_big_player_type_max)
		//{
		//	return;
		//}
		//if (GAMECONFIG->LordOfWarWorshipPositionArray.size() != 6
		//	|| GAMECONFIG->LordOfCityWorshipPositionArray.size() != 6
		//	|| GAMECONFIG->LordOfSanctuaryWorshipPositionArray.size() != 6
		//	|| GAMECONFIG->LordOfGodWorshipPositionArray.size() != 6)
		//{
		//	return;
		//}


		//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(map_guid);
		//if (nullptr == map_object_ptr)
		//{
		//	return;
		//}
		//s_map_pos temp_pos;
		//int32 npc_template_id = 0;
		//switch (big_type)
		//{
		//case e_big_player_type_lord_of_city:
		//	temp_pos.unit_location.x = GAMECONFIG->LordOfCityWorshipPositionArray[0];
		//	temp_pos.unit_location.y = GAMECONFIG->LordOfCityWorshipPositionArray[1];
		//	temp_pos.unit_location.z = GAMECONFIG->LordOfCityWorshipPositionArray[2];
		//	temp_pos.unit_rotation.roll = GAMECONFIG->LordOfCityWorshipPositionArray[3];
		//	temp_pos.unit_rotation.pitch = GAMECONFIG->LordOfCityWorshipPositionArray[4];
		//	temp_pos.unit_rotation.yaw = GAMECONFIG->LordOfCityWorshipPositionArray[5];
		//	npc_template_id = GAMECONFIG->LordOfCityWorshipNpcTemplateId;
		//	break;
		//case e_big_player_type_lord_or_war:
		//	temp_pos.unit_location.x = GAMECONFIG->LordOfWarWorshipPositionArray[0];
		//	temp_pos.unit_location.y = GAMECONFIG->LordOfWarWorshipPositionArray[1];
		//	temp_pos.unit_location.z = GAMECONFIG->LordOfWarWorshipPositionArray[2];
		//	temp_pos.unit_rotation.roll = GAMECONFIG->LordOfWarWorshipPositionArray[3];
		//	temp_pos.unit_rotation.pitch = GAMECONFIG->LordOfWarWorshipPositionArray[4];
		//	temp_pos.unit_rotation.yaw = GAMECONFIG->LordOfWarWorshipPositionArray[5];
		//	npc_template_id = GAMECONFIG->LordOfWarWorshipNpcTemplateId;
		//	break;
		//case e_big_player_type_lord_or_sanctuary:
		//	temp_pos.unit_location.x = GAMECONFIG->LordOfSanctuaryWorshipPositionArray[0];
		//	temp_pos.unit_location.y = GAMECONFIG->LordOfSanctuaryWorshipPositionArray[1];
		//	temp_pos.unit_location.z = GAMECONFIG->LordOfSanctuaryWorshipPositionArray[2];
		//	temp_pos.unit_rotation.roll = GAMECONFIG->LordOfSanctuaryWorshipPositionArray[3];
		//	temp_pos.unit_rotation.pitch = GAMECONFIG->LordOfSanctuaryWorshipPositionArray[4];
		//	temp_pos.unit_rotation.yaw = GAMECONFIG->LordOfSanctuaryWorshipPositionArray[5];
		//	npc_template_id = GAMECONFIG->LordOfSanctuaryWorshipNpcTemplateId;
		//	break;
		//case e_big_player_type_lord_or_god:
		//	temp_pos.unit_location.x = GAMECONFIG->LordOfGodWorshipPositionArray[0];
		//	temp_pos.unit_location.y = GAMECONFIG->LordOfGodWorshipPositionArray[1];
		//	temp_pos.unit_location.z = GAMECONFIG->LordOfGodWorshipPositionArray[2];
		//	temp_pos.unit_rotation.roll = GAMECONFIG->LordOfGodWorshipPositionArray[3];
		//	temp_pos.unit_rotation.pitch = GAMECONFIG->LordOfGodWorshipPositionArray[4];
		//	temp_pos.unit_rotation.yaw = GAMECONFIG->LordOfGodWorshipPositionArray[5];
		//	npc_template_id = GAMECONFIG->LordOfGodWorshipNpcTemplateId;
		//	break;
		//default:
		//	return;
		//	break;
		//}

		//if (m_big_player_statue_index_array[big_type] > 0)
		//{
		//	//移除旧雕像
		//	//del_npc(map_guid, m_big_player_statue_index_array[big_type], false);
		//	unit_man::remove_npc(m_big_player_statue_index_array[big_type], false);
		//	m_big_player_statue_index_array[big_type] = -1;
		//}

		//npc* temp_npc = spawn_fake_player_npc(map_guid,
		//	npc_template_id,
		//	&player_info,
		//	temp_pos);
		//if (temp_npc)
		//{
		//	m_big_player_statue_index_array[big_type] = temp_npc->get_array_index();

		//	guid_64 statue_guid = temp_npc->get_unit_guid();
		//	cs2ws_create_big_player_success success_msg;
		//	success_msg.big_player_guid = statue_guid;
		//	success_msg.big_type = big_type;
		//	connection_mgr::getInstance().send_to_ws(&success_msg, sizeof(cs2ws_create_big_player_success));
		//}
	}

	void world_cs::send_notice_with_param(faith::guid_64 sender_guid, std::string sender_name, 
		std::string content_text, std::vector<int32> param_array, int32 param_type,
		int32 chat_type, int32 sender_template_id, int32 notice_id)
	{
		ZoneScoped;
		if (param_array.size() > e_item_info_max)
		{
			return;
		}

		cs2ws_send_notice_with_param msg;
		msg.sender_guid = sender_guid;
		msg.param_type = param_type;
		msg.chat_typ = chat_type;
		msg.template_id = sender_template_id;
		msg.notice_id = notice_id;

		int32 name_len = sender_name.length();
		if (name_len > max_name_size)
		{
			name_len = max_name_size;
		}
		memcpy(msg.sender_name, sender_name.c_str(), name_len);

		int32 content_len = content_text.length();
		if (content_len > chat_max_chat_size)
		{
			content_len = chat_max_chat_size;
		}
		memcpy(msg.content_text, content_text.c_str(), content_len);
		msg.param_len = 0;
		for (int32 i = 0; i < param_array.size(); ++i)
		{
			msg.param_array[i] = param_array[i];
			msg.param_len++;
		}

		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_send_notice_with_param));
	}

	void world_cs::ws2cs_get_map_info_from_cs_fun(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_get_map_info_from_cs* packet = static_cast<const ws2cs_get_map_info_from_cs*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_get_map_info_from_cs))
		{
			return;
		}

		auto map_object_ptr = get_map_by_guid<map_object>(packet->map_guid);
		if (nullptr == map_object_ptr)
		{
			return;
		}
		MapTemplate* map_template_ptr = map_object_ptr->get_map_template_ptr();
		if (nullptr == map_template_ptr)
		{
			return;
		}
		e_map_type operate_map_type = (e_map_type)map_object_ptr->get_map_type();
		switch (operate_map_type)
		{
		case faith::e_map_type_legion_station:
		{
			//auto map_cs_ptr = dynamic_cast<legion_station_map_cs*>(map_object_ptr);
			//if (nullptr == map_cs_ptr)
			//{
			//	return;
			//}
			//guid_64 boss_guid = map_cs_ptr->get_boss_guid();
			//npc& legion_boss_npc_ptr = unit_man::get_npc(boss_guid);
			//if (legion_boss_npc_ptr.is_valid() == false || legion_boss_npc_ptr.is_dead() == true)
			//{
			//	return;
			//}
			//pawn_att& unit_att_ref = legion_boss_npc_ptr.get_pawn_att();
			//double boss_cur_hp = unit_att_ref.get_unit_base_att(e_base_att_info_hp_cur);
			//double boss_max_hp = unit_att_ref.get_attack_att_value(e_unit_attack_att_hp_max);

			//cs2ws_get_map_info_from_cs_end get_map_info_from_cs_end_msg;
			//get_map_info_from_cs_end_msg.role_guid = packet->role_guid;
			//get_map_info_from_cs_end_msg.map_template_id = packet->map_template_id;
			//get_map_info_from_cs_end_msg.map_info[e_map_info_player_num] = packet->map_info[0];
			//get_map_info_from_cs_end_msg.map_info[e_map_info_boss_cur_hp] = boss_cur_hp;
			//get_map_info_from_cs_end_msg.map_info[e_map_info_boss_max_hp] = boss_max_hp;

			//connection_mgr::getInstance().send_to_ws(&get_map_info_from_cs_end_msg, sizeof(get_map_info_from_cs_end_msg));
			break;
		}
		default:
			break;
		}
	}

	void world_cs::create_harry_player_statue(const s_fake_player_info& player_info, const int32& big_type, const guid_64& map_guid)
	{
		ZoneScoped;
		CONSOLE_INFO("create_harry_player_statue guid = {} big_type = {}", player_info.guid.server_64, big_type);
		if (big_type >= e_fake_type_first_harry_player && big_type <= e_fake_type_third_harry_player)
		{
			int32 npc_spawn_id = harry_map_first_spawn_id + big_type;
			NpcSpawnPointTemplate* npc_spawn_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_id);
			if (nullptr == npc_spawn_ptr)
			{
				return;
			}
			if (npc_spawn_ptr->MapId != harry_map_id)
			{
				return;
			}
			s_map_pos temp_pos;
			temp_pos.set_location(npc_spawn_ptr->PosX, npc_spawn_ptr->PosY, npc_spawn_ptr->PosZ);
			temp_pos.set_rotation(npc_spawn_ptr->Roll, npc_spawn_ptr->Pitch, npc_spawn_ptr->Yaw);
			int32 npc_template_id = npc_spawn_ptr->TemplateId;

			npc* temp_npc = spawn_fake_player_npc(map_guid,
				npc_template_id,
				player_info,
				temp_pos,
				e_npc_type_server_harry_chests);

			if (nullptr == temp_npc)
			{
				CONSOLE_INFO("create_harry_player_statue npc guid = {} big_type = {}", player_info.guid.server_64, big_type);
			}
		}
		if (big_type >= e_fake_type_best_barserker && big_type < e_fake_type_max)//通用根据类型创建雕像逻辑，可直接调用
		{
			auto map_cs_ptr = get_map_by_guid<map_cs>(map_guid);
			if (nullptr == map_cs_ptr)
			{
				return;
			}

			int32 map_line = map_cs_ptr->get_line_id() - 1;
			if (map_line >= map_lind_max_num)
			{
				return;
			}
			else if (map_line < 0)
			{
				map_line = 0;
			}
			int32 npc_spawn_id = pk_winner_first_spawn_id + big_type - e_fake_type_best_barserker;
			NpcSpawnPointTemplate* npc_spawn_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_id);
			if (nullptr == npc_spawn_ptr)
			{
				return;
			}
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, npc_spawn_ptr->MapId);
			if (map_template_ptr == nullptr)
			{
				return;
			}
			if (map_template_ptr->Type != faith::e_map_type_big_map)
			{
				return;
			}
			if (m_pk_player_idex_array[map_line][big_type] > 0)
			{
				npc& last_unit_ref = unit_man::get_npc(m_pk_player_idex_array[map_line][big_type]);
				if (last_unit_ref.is_valid() && init_unit::is_fake_player(last_unit_ref.get_unit_type(), last_unit_ref.get_unit_sub_type())
					&& last_unit_ref.get_npc_template_id() == npc_spawn_ptr->TemplateId)//防止误删
				{
					unit_man::remove_npc(m_pk_player_idex_array[map_line][big_type], false);
				}
				m_pk_player_idex_array[map_line][big_type] = -1;
			}
			s_fake_player_info fake_player_info = player_info;
			if (!player_info.guid.is_valid())//不存在玩家，创建普通NPC
			{
				if (npc_spawn_ptr->SpawnRate < 0)
				{
					return;
				}
				NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_ptr->TemplateId);
				if (npc_template_ptr == nullptr)
				{
					return;
				}
				if (npc_template_ptr->NpcType == e_unit_type_monster
					&& init_unit::is_world_boss(npc_template_ptr->SubType))
				{
					//世界Boss啥的归WS上的某个管理器处理
					return;
				}

				npc* new_monster_ptr = nullptr;
				if (npc_spawn_ptr->IsCreateByMapCreate > 0
					&& map_cs_ptr->is_in_refresh_time(npc_spawn_ptr))
				{
					new_monster_ptr = map_cs_ptr->spawn_npc(npc_spawn_id);
				}
				if (new_monster_ptr != nullptr)
				{
					m_pk_player_idex_array[map_line][big_type] = new_monster_ptr->get_array_index();
				}
			}
			else
			{
				s_map_pos temp_pos;
				temp_pos.set_location(npc_spawn_ptr->PosX, npc_spawn_ptr->PosY, npc_spawn_ptr->PosZ);
				temp_pos.set_rotation(npc_spawn_ptr->Roll, npc_spawn_ptr->Pitch, npc_spawn_ptr->Yaw);
				int32 npc_template_id = npc_spawn_ptr->TemplateId;

				npc* temp_npc = spawn_fake_player_npc(map_guid,
					npc_template_id,
					fake_player_info,
					temp_pos,
					e_npc_type_lord_city);
				if (nullptr != temp_npc)
				{
					m_pk_player_idex_array[map_line][big_type] = temp_npc->get_array_index();
				}
			}
		}
	}

	void world_cs::set_fake_player_idex(int32 map_line_id, int32 fakse_type, int32 idex)
	{
		ZoneScoped;
		if (fakse_type < 0 || fakse_type >= e_fake_type_max || map_line_id < 0 || map_line_id >= map_lind_max_num)
		{
			return;
		}
		m_pk_player_idex_array[map_line_id][fakse_type] = idex;
	}
}
