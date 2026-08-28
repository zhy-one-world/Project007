#include "base_map_system.h"
#include "components/scene/base_map_component.h"
#include "base/ecs_world.h"
#include "server_log.hpp"
#include "logic/unit_man.h"
#include "logic/buff_man.h"
#include "server_log_def.hpp"
#include "logic/player.hpp"
#include "logic/npc.hpp"
#include "buff_def.hpp"
#include "gm_order_def.hpp"

using namespace hld;

void base_map_system::start_up(Entity* map_ent, int32 line_id, MapTemplate* map_template)
{
	ZoneScoped;
	auto base_map_cp = map_ent->add_component<base_map_component>();
	base_map_cp->m_map_id = map_template->attribute_id;
	base_map_cp->m_map_template = map_template;
	base_map_cp->m_line_id = line_id;
	return;
}
void base_map_system::shut_down(Entity* entity)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	auto npc_list = base_map_cp->m_npc_list;
	for (auto& npc_index : npc_list)
	{
		unit_man::remove_npc(npc_index, false);
	}
	entity->remove_component<base_map_component>();
}
int32 base_map_system::get_map_template_id(Entity* entity)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return -1;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return -1;
	}
	return base_map_cp->m_map_id;
}
int32 base_map_system::get_map_type(Entity* entity)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return -1;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return -1;
	}
	return base_map_cp->m_map_template != nullptr ? base_map_cp->m_map_template->Type : 0;
}
int32 base_map_system::get_map_sub_type(Entity* entity)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return -1;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return -1;
	}
	return base_map_cp->m_map_template != nullptr ? base_map_cp->m_map_template->SubType : 0;

}
MapTemplate* base_map_system::get_map_template(Entity* entity)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return nullptr;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return nullptr;
	}
	return base_map_cp->m_map_template;

}

int32 base_map_system::get_map_template_id(const guid_64& map_guid)
{
	auto entity = get_entity(map_guid);
	return get_map_template_id(entity);
}
int32 base_map_system::get_map_type(const guid_64& map_guid)
{
	auto entity = get_entity(map_guid);
	return get_map_type(entity);

}
int32 base_map_system::get_map_sub_type(const guid_64& map_guid)
{
	auto entity = get_entity(map_guid);

	return get_map_sub_type(entity);

}
MapTemplate* base_map_system::get_map_template(const guid_64& map_guid)
{
	auto entity = get_entity(map_guid);

	return get_map_template(entity);

}
void base_map_system::on_player_add(Entity* entity, player* player_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	base_map_cp->m_player_list.insert(player_ptr->get_array_index());
}
void base_map_system::on_player_delete(Entity* entity, player* player_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	base_map_cp->m_player_list.erase(player_ptr->get_array_index());
}
void base_map_system::on_player_load_data_over(Entity* map_ent, player* player_ptr)
{

}
void base_map_system::on_player_enter_scene(Entity* entity, player* player_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();

	add_map_buff(base_map_cp.get(), player_ptr->get_array_index());
	add_vip_map_buff(base_map_cp.get(), player_ptr->get_array_index());
	set_default_pk_mode(base_map_cp.get(), player_ptr->get_array_index());
	map_change_log(base_map_cp.get(), player_ptr->get_array_index(), hld::e_map_change_enter);
}
void base_map_system::on_player_leave_scene(Entity* entity, player* player_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	map_change_log(base_map_cp.get(), player_ptr->get_array_index(), hld::e_map_change_leave);
}
void base_map_system::on_player_dead(Entity* entity, player* player_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
}
void base_map_system::on_player_reconnect(Entity* entity, player* player_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
}
void base_map_system::on_npc_add(Entity* entity, npc* npc_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	base_map_cp->m_npc_list.insert(npc_ptr->get_array_index());
}
void base_map_system::on_npc_dead(Entity* entity, npc* npc_ptr)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
}
void base_map_system::on_npc_delete(Entity* entity, npc* npc_ptr, bool is_dead)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	base_map_cp->m_npc_list.erase(npc_ptr->get_array_index());
}
void base_map_system::on_unit_be_damage(Entity* entity, const int32& attack_index, const int32& be_attack_index, double damage_value)
{
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
}

void base_map_system::map_settlement(Entity* entity, MapTemplate* map_template)
{
	ZoneScoped;
	auto base_map_cp = entity->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	auto npc_list = base_map_cp->m_npc_list;
	for (auto& it : npc_list)
	{
		unit_man::remove_npc(it, false);
	}
	base_map_cp->m_npc_list.clear();
	base_map_cp->m_map_id = map_template->attribute_id;
	base_map_cp->m_map_template = map_template;
}
void base_map_system::transfer_all_player(Entity* entity, int32 map_template_id)
{
	ZoneScoped;
	auto base_map_cp = entity->get_component<base_map_component>();
	auto player_list = base_map_cp->m_player_list;
	for (auto it : player_list)
	{
		auto& temp_player = unit_man::get_player(it);
		int32 map_line_id = 0;
		if (0 == map_template_id)
		{
			map_template_id = temp_player.get_unit_info(e_role_info_main_map_id);
			map_line_id = temp_player.get_main_line_id();
		}
		// 如果是个人打宝地图离开时复原Pk模式
		if (base_map_cp->m_map_template->Type == e_map_type_single_land_boss)
		{
			int32 main_pk_mode = temp_player.get_logic_data(e_role_logic_info_main_pk_mode);
			temp_player.get_pk_community_mgr().set_entire_pk_mode(main_pk_mode);
		}
		temp_player.transfer_by_template(map_template_id, map_line_id, temp_player.get_unit_info(e_role_info_server_id), guid_64(), 0);
	}
}
void base_map_system::remove_all_monster(Entity* entity)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	auto npc_list = base_map_cp->m_npc_list;
	for (auto& it : npc_list)
	{
		//友方NPC不删除
		auto& temp_npc = unit_man::get_npc(it);

		if (temp_npc.get_unit_type() == e_unit_type_friend_npc || e_unit_type_drop_bag == temp_npc.get_unit_type()
			|| (temp_npc.get_unit_type() == e_unit_type_npc && temp_npc.get_unit_sub_type() == e_npc_type_cross_pk_chests)
			|| (temp_npc.get_unit_type() == e_unit_type_npc && temp_npc.get_unit_sub_type() == e_npc_type_world_boss_chests))//防止掉落包被清掉
		{
			continue;
		}

		unit_man::remove_npc(it, false);
	}
}
void base_map_system::kill_all_monster(Entity* entity, int32 player_index)
{
	ZoneScoped;
	if (nullptr == entity)
	{
		CONSOLE_ERROR("entity is nullptr");
		return;
	}
	auto base_map_cp = entity->get_component<base_map_component>();
	auto npc_list = base_map_cp->m_npc_list;
	buff_ex_env_param penv;
	penv.damage_num = int32_MAX_NUM;
	penv.buff_level = 1;
	for (auto it : npc_list)
	{
		buff_man::add_buff_inst(player_index, it, GM_DAMAGE_BUFF_ID, &penv);
	}
}
void base_map_system::add_map_buff(base_map_component& base_map_cp, int32 unit_index)
{
	ZoneScoped;
	auto& player_ref = unit_man::get_player(unit_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player_ref is invalid");
		return;
	}
	for (int32 i = 0; i < base_map_cp.m_map_template->MapBuffIdArray.size(); i += 2)
	{
		if (i + 1 < base_map_cp.m_map_template->MapBuffIdArray.size())
		{
			if (base_map_cp.m_map_template->MapBuffIdArray[i + 1] > e_class_type_none && base_map_cp.m_map_template->MapBuffIdArray[i + 1] != player_ref.get_unit_info(e_role_info_class_type))
			{
				continue;
			}
		}
		buff_man::add_buff_inst(unit_index, unit_index, base_map_cp.m_map_template->MapBuffIdArray[i]);
	}
}
void  base_map_system::add_vip_map_buff(base_map_component& base_map_cp, int32 unit_index)
{
	ZoneScoped;
	auto& player_ref = unit_man::get_player(unit_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player_ref is invalid");
		return;
	}
	int32 cur_player_vip = player_ref.get_vip_level(false);
	if (base_map_cp.m_map_template->VipEnterSceneBuff.size() > cur_player_vip)
	{
		buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), base_map_cp.m_map_template->VipEnterSceneBuff[cur_player_vip]);
	}
}
void base_map_system::set_default_pk_mode(base_map_component& base_map_cp, int32 unit_index)
{
	ZoneScoped;
	//现在的逻辑需要部分进入地图改pk模式 比如打宝地图
	if (e_map_type_big_map == base_map_cp.m_map_template->Type)
	{//非大世界的逻辑
		return;
	}
	if (base_map_cp.m_map_template->DefaultPkMode < 0 || base_map_cp.m_map_template->DefaultPkMode >= e_pk_mode_max)
	{
		CONSOLE_ERROR("DefaultPkMode is invalid PkMode:{}", base_map_cp.m_map_template->DefaultPkMode);
		return;
	}
	if (base_map_cp.m_map_template->DefaultPkMode == e_pk_mode_family)
	{
		CONSOLE_ERROR("DefaultPkMode is family PkMode:{}", base_map_cp.m_map_template->DefaultPkMode);
		return;
	}
	player& player_ref = unit_man::get_player(unit_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player_ref is nullptr");
		return;
	}
	player_ref.get_pk_community_mgr().set_pk_mode(base_map_cp.m_map_template->DefaultPkMode);
}
void base_map_system::send_map_message(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header)
{
	ZoneScoped;
	if (nullptr == map_ent)
	{
		CONSOLE_ERROR("map_ent is nullptr");
		return;
	}
	auto base_map_cp = map_ent->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_ERROR("base_map_cp is nullptr");
		return;
	}
	auto player_list = base_map_cp->m_player_list;
	for (auto& it : player_list)
	{
		auto& player_ref = unit_man::get_player(it);
		player_ref.send_message_to_self(net_pro, header);
	}
}
void base_map_system::map_change_log(base_map_component& base_map_cp, int32 unit_index, e_map_change_type chang_type)
{
	ZoneScoped;
	player& player_ref = unit_man::get_player(unit_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player_ref is nullptr");
		return;
	}

	server_log::map_change_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_login_type(), base_map_cp.m_map_id, base_map_cp.m_map_template->Type, base_map_cp.m_map_template->SubType, chang_type, base_map_cp.m_map_template->Difficulty);

}
void base_map_system::gm_show_all_npc(Entity* map_ent)
{
	auto base_map_cp = map_ent->get_component<base_map_component>();
	if (false == base_map_cp.isValid())
	{
		CONSOLE_INFO("base_map_cp is null");
		return;
	}
	for (auto& it : base_map_cp->m_npc_list)
	{
		auto& npc = unit_man::get_npc(it);
		auto pos = npc.get_new_map_pos();
		CONSOLE_INFO("npc_index:{} npc_id:{} spawn_point_id:{} x:{} y:{} z:{}", it, npc.get_npc_template_id(), npc.get_spawn_point_id(), pos.unit_location.x, pos.unit_location.y, pos.unit_location.z);
	}
}