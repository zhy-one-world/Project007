#include "aoi/aoi_system.h"
#include "logic/drop.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "utility/random.h"
#include "server_log.hpp"
#include "system/scene/base_map_system.h"
#include "threat_drop_processor.h"

using namespace hld;
Entity* m_map_ent;
std::vector<player_score_info> m_hate_player_score_info_array;
std::vector<team_score_info>   m_hate_team_score_info_array;
std::vector<player_score_info> m_damage_player_score_info_array;
std::vector<team_score_info>   m_damage_team_score_info_array;
s_map_pos					   m_init_pos;
int32						   m_first_attacker_index;
int32						   m_last_attacker_index;
std::vector<int32>			   m_item_id_array;
NpcTemplate* m_npc_template_ptr;
int32						   m_npc_array_index;

void drop_manager::clear_data()
{
	m_hate_player_score_info_array.clear();
	m_hate_team_score_info_array.clear();
	m_damage_player_score_info_array.clear();
	m_damage_team_score_info_array.clear();
	m_map_ent = nullptr;
	m_init_pos.clear_data();
	m_first_attacker_index = 0;
	m_last_attacker_index = 0;
	m_item_id_array.clear();
	m_npc_template_ptr = nullptr;
	m_npc_array_index = 0;
}

void drop_manager::create_drop(NpcTemplate*  npc_template_ptr,
	Entity* map_ent,
	const s_map_pos& init_pos,
	const int32& first_attacker_index,
	const int32& last_attacker_index,
	const hate_info_vector& hate_list,
	const damage_info_vector& damage_list,
	const int32& npc_array_index,
	const bool& is_use_all_player_drop)
{
	clear_data(); // 工具单例类 使用前需要清空
	if (nullptr == npc_template_ptr)
	{
		return;
	}
	player& player_ref = unit_man::get_player(last_attacker_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}

	m_map_ent = map_ent;
	m_init_pos = init_pos;
	m_first_attacker_index = first_attacker_index;
	m_last_attacker_index = last_attacker_index;
	m_npc_template_ptr = npc_template_ptr;
	m_npc_array_index = npc_array_index;

	if (npc_template_ptr->NpcType == e_unit_type_monster && npc_template_ptr->SubType == e_monster_type_gold_army)//如果为世界BOSS，则掉落包直接生成到玩家脚下，尝试修复掉落包不见得问题
	{
		m_init_pos = player_ref.get_new_map_pos();
	}

	vector<int32> temp_drop(npc_template_ptr->DropID);
	if (is_use_all_player_drop == true && npc_template_ptr->WorldBossDropID.size() > 0)
	{
		temp_drop.clear();
		temp_drop = npc_template_ptr->WorldBossDropID;
	}

	if (player_ref.get_time_limit_activity_mgr().is_activity_open(e_time_limit_activity_type_added_drop))
	{
		player_ref.get_time_limit_activity_mgr().change_added_drop_item();
		temp_drop.push_back(time_limit_activity_added_drop_drop_template_id);
	}
	init_drop(temp_drop,hate_list, damage_list);
	send_notice();

	//军团boss被击杀后添加军团boss击杀事件
	legion_cs_mgr legion_cs_mgr_ref = player_ref.get_legion_cs_mgr();
	if (legion_cs_mgr_ref.is_legion_boss_map(m_map_ent->getEntityId()))
	{
		legion_cs_mgr_ref.add_kill_boss_event(player_ref.get_name(), npc_template_ptr->attribute_id, m_item_id_array);
	}
}

void drop_manager::clear_score_array()
{
	m_hate_player_score_info_array.clear();
	m_hate_team_score_info_array.clear();
	m_damage_player_score_info_array.clear();
	m_damage_team_score_info_array.clear();
}

void drop_manager::init_drop(vector<int32>& drop_list, const hate_info_vector& hate_list, const damage_info_vector& damage_list)
{
	int32 list_size = drop_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		clear_score_array();//清理一次缓存的分数信息，否则会导致同一个玩家多次加入到数组中--比如同一个NPC存在多个掉落包时
		int32 drop_template_id = drop_list[list_index];
		DropTemplate* temp_drop_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
		if (nullptr == temp_drop_ptr)
		{
			continue;
		}

		switch (temp_drop_ptr->BelongID)
		{
		case e_dropbox_attribution_rule_first:
		{
			player& ref_player = unit_man::get_player(m_first_attacker_index);
			if (false == ref_player.is_valid())
			{
				break;
			}
			distribution_by_player(drop_template_id, &ref_player, hate_list);
			break;
		}
		case e_dropbox_attribution_rule_killer:
		{
			player& ref_player = unit_man::get_player(m_last_attacker_index);
			if (false == ref_player.is_valid())
			{
				break;
			}
			distribution_by_player(drop_template_id, &ref_player, hate_list);
			break;
		}
		case e_dropbox_attribution_rule_threat:
		{
			distribution_by_list(drop_template_id, (int32)e_dropbox_attribution_rule_threat, hate_list, damage_list);
			break;
		}
		case e_dropbox_attribution_rule_dmg:
		{
			distribution_by_list(drop_template_id,(int32)e_dropbox_attribution_rule_dmg, hate_list, damage_list);
			break;
		}
		default:
			break;
		}
	}
}

void drop_manager::init_player_score_info(const hate_info_vector& hate_list, std::vector<player_score_info>& player_score_info_array)
{
	int32 list_size = hate_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		const s_hate_info& ref_hate_info = hate_list[list_index];
			player& ref_player = unit_man::get_player(ref_hate_info.unit_identifier);
			if (false == ref_player.is_valid())
			{
				continue;
			}
		if (ref_hate_info.hate_value <= 0)
		{
			continue;
		}
			
		player_score_info new_player_score_info;
		new_player_score_info.init_data(&ref_player, ref_hate_info.hate_value);

		add_player_score_info_array(new_player_score_info, player_score_info_array);
	}
}

void drop_manager::init_team_score_info(const hate_info_vector& hate_list, std::vector<team_score_info>& team_score_info_array)
{
	int32 list_size = hate_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		const s_hate_info& ref_hate_info = hate_list[list_index];
		player& ref_player = unit_man::get_player(ref_hate_info.unit_identifier);
		if (false == ref_player.is_valid())
		{
			continue;
		}
		if (ref_hate_info.hate_value <= 0)
		{
			continue;
		}

		player_score_info new_player_score_info;
		new_player_score_info.init_data(&ref_player, ref_hate_info.hate_value);

		add_team_score_info_array(new_player_score_info, team_score_info_array);
	}
	team_score_info_array_sort(team_score_info_array);
	find_other_team_member_in_aoi(team_score_info_array);
}

	void drop_manager::init_team_score_info_ex(const hate_info_vector& hate_list, std::vector<team_score_info>& team_score_info_array)
{
	int32 list_size = hate_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		const s_hate_info& ref_hate_info = hate_list[list_index];
		player& ref_player = unit_man::get_player(ref_hate_info.unit_identifier);
		if (false == ref_player.is_valid())
		{
			continue;
		}
		if (ref_hate_info.hate_value <= 0)
		{
			continue;
		}

		player_score_info new_player_score_info;
		new_player_score_info.init_data(&ref_player, ref_hate_info.hate_value);

		add_team_score_info_array(new_player_score_info, team_score_info_array);
	}
	team_score_info_array_sort(team_score_info_array);
}

void drop_manager::init_player_score_info(const damage_info_vector& damage_list, std::vector<player_score_info>& player_score_info_array)
{
	int32 list_size = damage_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		const s_damage_info& ref_damage_info = damage_list[list_index];
		player& ref_player = unit_man::get_player(ref_damage_info.unit_identifier);
		if (false == ref_player.is_valid())
		{
			continue;
		}
		if (ref_damage_info.damage_value <= 0)
		{
			continue;
		}


		player_score_info new_player_score_info;
		new_player_score_info.init_data(&ref_player, ref_damage_info.damage_value);

		add_player_score_info_array(new_player_score_info, player_score_info_array);
	}
}

void drop_manager::init_team_score_info(const damage_info_vector& damage_list, std::vector<team_score_info>& team_score_info_array)
{
	int32 list_size = damage_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		const s_damage_info& ref_damage_info = damage_list[list_index];
		player& ref_player = unit_man::get_player(ref_damage_info.unit_identifier);
		if (false == ref_player.is_valid())
		{
			continue;
		}
		if (ref_damage_info.damage_value <= 0)
		{
			continue;
		}
		player_score_info new_player_score_info;
		new_player_score_info.init_data(&ref_player, ref_damage_info.damage_value);

		add_team_score_info_array(new_player_score_info, team_score_info_array);
	}
	team_score_info_array_sort(team_score_info_array);
	find_other_team_member_in_aoi(team_score_info_array);
}

void drop_manager::init_team_score_info_ex(const damage_info_vector& damage_list, std::vector<team_score_info>&team_score_info_array)
{
	int32 list_size = damage_list.size();
	for (int32 list_index = 0; list_index < list_size; list_index++)
	{
		const s_damage_info& ref_damage_info = damage_list[list_index];
		player& ref_player = unit_man::get_player(ref_damage_info.unit_identifier);
		if (false == ref_player.is_valid())
		{
			continue;
		}
		if (ref_damage_info.damage_value <= 0)
		{
			continue;
		}
		player_score_info new_player_score_info;
		new_player_score_info.init_data(&ref_player, ref_damage_info.damage_value);
		add_team_score_info_array(new_player_score_info, team_score_info_array);
	}
	team_score_info_array_sort(team_score_info_array);
}

void drop_manager::add_score_info( player* player_ptr,
	const int32& player_score,
	std::vector<player_score_info>& player_score_info_array,
	std::vector<team_score_info>& team_score_info_array)
{
}

void drop_manager::add_player_score_info_array(player_score_info& new_player_score_info, std::vector<player_score_info>& player_score_info_array)
{
	int32 player_score = new_player_score_info.get_score();

	std::vector<player_score_info>::iterator iterator = player_score_info_array.begin();
	for (; iterator != player_score_info_array.end(); ++iterator)
	{
		if (player_score > iterator->get_score())
		{
			player_score_info_array.insert(iterator, new_player_score_info);
			return;
		}
	}

	player_score_info_array.insert(iterator, new_player_score_info);
}

void drop_manager::add_team_score_info_array(player_score_info& new_player_score_info, std::vector<team_score_info>& team_score_info_array)
{
	player* temp_player_ptr = new_player_score_info.get_player_ptr();
	if (nullptr == temp_player_ptr)
	{
		return;
	}
	guid_64 team_guid = temp_player_ptr->get_team_guid();

	std::vector<team_score_info>::iterator iterator = team_score_info_array.begin();
	for (; iterator != team_score_info_array.end(); ++iterator)
	{
		if (false == team_guid.is_valid())
		{
			team_guid = temp_player_ptr->get_unit_guid();
			break;
		}
		if (team_guid == iterator->get_team_guid())
		{
			iterator->add_player_member(new_player_score_info);
			return;
		}
	}

	team_score_info new_team_score_info;
	new_team_score_info.init_data(team_guid);
	new_team_score_info.add_player_member(new_player_score_info);
	team_score_info_array.push_back(new_team_score_info);
}

void drop_manager::team_score_info_array_sort(std::vector<team_score_info>& team_score_info_array)
{
	int32 score_info_array_size = team_score_info_array.size();
	for (int32 score_info_array_index = 0; score_info_array_index < score_info_array_size - 1; score_info_array_index++)
	{
		for (int32 temp_index = score_info_array_index; temp_index < score_info_array_size; temp_index++)
		{
			if (team_score_info_array[score_info_array_index].get_team_score() < team_score_info_array[temp_index].get_team_score())
			{
				swap(team_score_info_array[score_info_array_index], team_score_info_array[temp_index]);
			}
		}
	}
}

void drop_manager::clear_zero_score_team(std::vector<team_score_info>& team_score_info_array)
{
	int32 array_size = team_score_info_array.size();
	std::vector<team_score_info>::iterator iterator = team_score_info_array.begin();
	for (; iterator != team_score_info_array.end();)
	{
		if (0 == iterator->get_team_score())
		{
			team_score_info_array.erase(iterator);
		}
		else 
		{
			++iterator;
		}
	}
}

void drop_manager::find_other_team_member_in_aoi(std::vector<team_score_info>& team_score_info_array)
{
	const int32 array_size = team_score_info_array.size();

	const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(m_map_ent, m_init_pos.unit_location);
	if (aoi_tower_watch.empty())
	{
		return;
	}
	for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
	{
		const int32& unit_index = it->first;
		player& aoi_player_ref = unit_man::get_player(unit_index);
		if (aoi_player_ref.is_valid() == false)
		{
			continue;
		}
		const guid_64& team_guid = aoi_player_ref.get_team_guid();
		for (int32 array_index = 0; array_index < array_size; array_index++)
		{
			team_score_info& team_info = team_score_info_array[array_index];
			if (team_guid == team_info.get_team_guid())
			{
				guid_64 ref_player_guid = aoi_player_ref.get_unit_guid();
				if (false == ref_player_guid.is_valid())
				{
					break;
				}
				if (false == team_info.has_this_player(ref_player_guid))
				{
					player_score_info new_player_score_info;
					new_player_score_info.init_data(&aoi_player_ref, 0);
					team_info.add_player_member(new_player_score_info);
				}
				break;
			}
		}
	}
}

void drop_manager::distribution_by_player(int32& drop_template_id, player* player_ptr, const hate_info_vector& hate_list)
{
	DropTemplate* temp_drop_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
	if (nullptr == temp_drop_ptr)
	{
		return;
	}
	int32& belong_type = temp_drop_ptr->BelongType;
	int32& distribute_type = temp_drop_ptr->Distribute;
	int32& drop_type = temp_drop_ptr->DropType;
	switch (belong_type)
	{
	case e_dropbox_owner_team:
	{
		guid_64 temp_guid = player_ptr->get_team_guid();
		if (false == temp_guid.is_valid())
		{
			distribution_to_personal(drop_template_id, player_ptr, drop_type);
		}
		else
		{
			m_hate_team_score_info_array.clear();
			init_team_score_info(hate_list, m_hate_team_score_info_array);
			team_score_info temp_team_score_info = get_team_score_info(temp_guid);
			distribution_to_team(drop_template_id, temp_team_score_info, distribute_type, drop_type);
		}
		break;
	}
	case e_dropbox_owner_player:
	{
		distribution_to_personal(drop_template_id, player_ptr, drop_type);
		break;
	}
	default:
		break;
	}
}

void drop_manager::distribution_by_list(int32& drop_template_id, int32 rule_type, const hate_info_vector& hate_list, const damage_info_vector& damage_list)
{
	DropTemplate* temp_drop_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
	if (nullptr == temp_drop_ptr)
	{
		return;
	}
	int32& belong_type = temp_drop_ptr->BelongType;
	int32& distribute_type = temp_drop_ptr->Distribute;
	int32& drop_type = temp_drop_ptr->DropType;
	std::vector<int32>& belong_id_param = temp_drop_ptr->BelongIDParam;
	if (2 != belong_id_param.size())
	{
		return;
	}
	int32 min_rank = belong_id_param[0] - 1;
	int32 max_rank = belong_id_param[1] - 1;

	switch (belong_type)
	{
	case e_dropbox_owner_team:
	{
		std::vector<team_score_info> team_list;
		if (e_dropbox_attribution_rule_threat == rule_type)
		{
			m_hate_team_score_info_array.clear();
			init_team_score_info(hate_list, m_hate_team_score_info_array);
			team_list = m_hate_team_score_info_array;
		}
		else if (e_dropbox_attribution_rule_dmg == rule_type)
		{
			m_damage_team_score_info_array.clear();
			init_team_score_info(damage_list, m_damage_team_score_info_array);
			team_list = m_damage_team_score_info_array;
		}
		int32 size_team_list = team_list.size();
		if (min_rank < 0 || max_rank < 0)
		{
			min_rank = 0;
			max_rank = size_team_list - 1;
		}
		int32 list_size = size_team_list - 1;
		if (list_size < min_rank)
		{
			return;
		}
		max_rank = max_rank > list_size ? list_size : max_rank;

		for (int32 index = min_rank; index <= max_rank; index++)
		{
			team_score_info temp_team_score_info = team_list[index];
			distribution_to_team(drop_template_id, temp_team_score_info, distribute_type, drop_type);
		}
		break;
	}
	case e_dropbox_owner_player:
	{
		std::vector<player_score_info> player_list;
		if (e_dropbox_attribution_rule_threat == rule_type)
		{
			m_hate_player_score_info_array.clear();
			init_player_score_info(hate_list, m_hate_player_score_info_array);
			player_list = m_hate_player_score_info_array;
		}
		else if (e_dropbox_attribution_rule_dmg == rule_type)
		{
			m_damage_player_score_info_array.clear();
			init_player_score_info(damage_list, m_damage_player_score_info_array);
			player_list = m_damage_player_score_info_array;
		}
		int32 size_player_list = player_list.size();
		if (min_rank < 0 || max_rank < 0)
		{
			min_rank = 0;
			max_rank = size_player_list - 1;
		}
		int32 list_size = size_player_list - 1;
		if (list_size < min_rank)
		{
			return;
		}
		max_rank = max_rank > list_size ? list_size : max_rank;

		for (int32 index = min_rank; index <= max_rank; index++)
		{
			player_score_info temp_player_score_info = player_list[index];
			player* player_ptr = temp_player_score_info.get_player_ptr();
			if (nullptr == player_ptr)
			{
				continue;
			}
			distribution_to_personal(drop_template_id, player_ptr, drop_type);
		}
		break;
	}
	case e_dropbox_owner_team_ex:
	{
		std::vector<team_score_info> team_list;
		if (e_dropbox_attribution_rule_threat == rule_type)
		{
			m_hate_team_score_info_array.clear();
			init_team_score_info_ex(hate_list, m_hate_team_score_info_array);
			team_list = m_hate_team_score_info_array;
		}
		else if (e_dropbox_attribution_rule_dmg == rule_type)
		{
			m_damage_team_score_info_array.clear();
			init_team_score_info_ex(damage_list, m_damage_team_score_info_array);
			team_list = m_damage_team_score_info_array;
		}
		int32 size_team_list = team_list.size();
		if (min_rank < 0 || max_rank < 0)
		{
			min_rank = 0;
			max_rank = size_team_list - 1;
		}
		int32 list_size = size_team_list - 1;
		if (list_size < min_rank)
		{
			return;
		}
		max_rank = max_rank > list_size ? list_size : max_rank;

		for (int32 index = min_rank; index <= max_rank; index++)
		{
			team_score_info temp_team_score_info = team_list[index];
			distribution_to_team_ex(drop_template_id, temp_team_score_info, distribute_type, drop_type);
		}
		break;
	}
	default:
		break;
	}
}

void drop_manager::distribution_to_personal(int32& drop_template_id, player* player_ptr, int32& drop_type)
{
	if (m_npc_template_ptr->FirstKillDropId.size() > 0)
	{
		if (!player_ptr->get_is_have_cur_first_kill_npc_id(m_npc_template_ptr->attribute_id))
		{
			player_ptr->insert_first_npc_kill_id_str(m_npc_template_ptr->attribute_id);
			for (int32 i = 0; i < m_npc_template_ptr->FirstKillDropId.size(); i++)
			{
				create_drop_box( m_npc_template_ptr->FirstKillDropId[i], player_ptr, drop_type);
			}
			return;
		}
	}
	create_drop_box(drop_template_id, player_ptr, drop_type);
}

void drop_manager::distribution_to_team(int32& drop_template_id, team_score_info& team_info, int32& distribute_type, int32& drop_type)
{
	int32 team_member_num = team_info.get_member_num();
	switch (distribute_type)
	{
	case e_dropbox_distribute_every_one:
	{
		for (int32 player_index = 0; player_index < team_member_num; player_index++)
		{
			player* player_ptr = team_info.get_member_ptr(player_index);
			if (nullptr == player_ptr)
			{
				continue;
			}
			if (m_npc_template_ptr->FirstKillDropId.size() > 0)
			{
				if (!player_ptr->get_is_have_cur_first_kill_npc_id(m_npc_template_ptr->attribute_id))
				{
					player_ptr->insert_first_npc_kill_id_str(m_npc_template_ptr->attribute_id);
					for (int32 i = 0; i < m_npc_template_ptr->FirstKillDropId.size(); i++)
					{
						create_drop_box(m_npc_template_ptr->FirstKillDropId[i], player_ptr, drop_type);
					}
					return;
				}
			}
			create_drop_box(drop_template_id, player_ptr, drop_type);
		}
		break;
	}
	case e_dropbox_distribute_random:
	{
		int32 player_index = random_gen::get_random(0, team_member_num - 1);
		player* player_ptr = team_info.get_member_ptr(player_index);
		if (nullptr == player_ptr)
		{
			return;
		}
		if (m_npc_template_ptr->FirstKillDropId.size() > 0)
		{
			if (!player_ptr->get_is_have_cur_first_kill_npc_id(m_npc_template_ptr->attribute_id))
			{
				player_ptr->insert_first_npc_kill_id_str(m_npc_template_ptr->attribute_id);
				for (int32 i = 0; i < m_npc_template_ptr->FirstKillDropId.size(); i++)
				{
					create_drop_box(m_npc_template_ptr->FirstKillDropId[i], player_ptr, drop_type);
				}
				return;
			}
		}
		create_drop_box(drop_template_id, player_ptr, drop_type);
		break;
	}
	default:
		break;
	}
}

void drop_manager::distribution_to_team_ex(int32& drop_template_id, team_score_info& team_info, int32& distribute_type, int32& drop_type)
{
	npc& npc_ref = unit_man::get_npc(m_npc_array_index);
	if (false == npc_ref.is_valid())
	{
		return;
	}
	int32 class_type = -1;
	int32 exp_level = -1;
	int32 team_member_num = team_info.get_member_num();
	if (team_member_num <= 0)
	{
		return;
	}
	if (drop_template_id <= 0)
	{
		return;
	}
	std::set<int32> player_index_arr;
	//将该队伍中玩家下标做一个数组，便于乱序
	std::vector<int32> player_ptr_index;
	player_ptr_index.clear();
	for (int32 i = 0; i < team_member_num; i++)
	{
		player_ptr_index.push_back(i);
		player* player_ptr = team_info.get_member_ptr(i);
		if (nullptr == player_ptr || player_ptr->get_session_state() != e_session_status_in_gaming)
		{
			continue;
		}
		if (team_member_num == 1 || player_ptr->get_team_cs_mgr().get_unit_team_info().captain_guid == player_ptr->get_unit_guid())
		{
			class_type = player_ptr->get_unit_info(e_role_info_class_type);
			exp_level = player_ptr->get_unit_info(e_role_info_exp_level);
		}
		if (false == player_ptr->check_can_get_boss_drop(&npc_ref))
		{
			continue;
		}	

		auto map_type = base_map_system::get_map_type(player_ptr->get_map_ent());
		if (true == init_unit::is_map_gain_treasure(map_type) || true == init_unit::is_map_boss_island(map_type))
		{
			player_index_arr.insert(player_ptr->get_array_index());
		}

	}
	//分配逻辑
	std::vector<s_item_template_info> item_drop_list_with_arr;
	cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_arr, class_type, exp_level);


	int32 list_num = item_drop_list_with_arr.size();
	std::map<int32, int32> list_and_lock;
	list_and_lock.clear();
	for (int32 i = 0; i < list_num; i++)
	{
		list_and_lock.insert({ item_drop_list_with_arr[i].m_item_id,item_drop_list_with_arr[i].m_lock });
	}
	random_array(item_drop_list_with_arr);
	random_array(player_ptr_index);
	int32 player_index = 0;
	std::vector<s_gain_treasure_record_player_info_one_things> player_info_arr;

	s_gain_treasure_record_player_info_one_things one_player_info;
	player_info_arr.clear();
	one_player_info.clear_data();

	std::vector< s_item_template_info > temp_list_arr[max_team_member_num];

	for (int32 i = 0; i < list_num; i++)
	{
		if (player_index >= team_member_num)
		{
			player_index = 0;
		}
		int32 cur_player_index = player_index;
		player* player_ptr = team_info.get_member_ptr(player_ptr_index[player_index]);
		player_index++;
		if (nullptr == player_ptr || player_ptr->get_session_state() != e_session_status_in_gaming)
		{
			continue;
		}
		if (false == player_ptr->check_can_get_boss_drop(&npc_ref))
		{
			continue;
		}
		auto cur_map_type = base_map_system::get_map_type(player_ptr->get_map_ent());
		if (false == init_unit::is_map_gain_treasure(cur_map_type) && false == init_unit::is_map_boss_island(cur_map_type))
		{
			continue;
		}
		temp_list_arr[cur_player_index].push_back({ item_drop_list_with_arr[i].m_item_id , 1, list_and_lock[item_drop_list_with_arr[i].m_item_id] });

		ItemTemplate* item_temp_ptr = GET_TEMPLATE(ItemTemplate, item_drop_list_with_arr[i].m_item_id);
		if (nullptr == item_temp_ptr)
		{
			continue;
		}
		auto map_type = base_map_system::get_map_type(m_map_ent);
		if (true == init_unit::is_map_gain_treasure(map_type))
		{
			if (item_temp_ptr->DropRecordModel > e_drop_record_model_type_no)
			{
				one_player_info.clear_data();
				std::string player_name = player_ptr->get_name();
				memcpy(one_player_info.role_name, player_name.c_str(), player_name.size() >= max_name_size ? max_name_size : player_name.size());
				one_player_info.gain_item_template_id = item_drop_list_with_arr[i].m_item_id;
				one_player_info.role_guid = player_ptr->get_unit_guid();
				player_info_arr.push_back(one_player_info);
			}
		}
	}

	for (int32 index = 0; index < max_team_member_num && index < team_member_num; ++index)
	{
		player* player_ptr = team_info.get_member_ptr(player_ptr_index[index]);
		if(player_ptr != nullptr)
			create_drop_box(temp_list_arr[index], player_ptr, drop_type, drop_template_id, true);
	}
}

bool drop_manager::is_same_class(int32& drop_template_id, player* player_ptr)
{
	DropTemplate* temp_drop_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
	if (nullptr == temp_drop_ptr)
	{
		return false;
	}
	int32& drop_class = temp_drop_ptr->ClassType;
	if (hld::e_class_type_none == drop_class)
	{
		return true;
	}
	int32 player_class = player_ptr->get_unit_info(e_role_info_class_type);
	if (drop_class == player_class)
	{
		return true;
	}
	return false;
}

void drop_manager::create_drop_box(int32& drop_template_id, player* player_ptr, int32& drop_type)
{
	if (false == is_same_class(drop_template_id, player_ptr))
	{
		return;
	}
	if (false == player_ptr->can_get_drop_from_monster())
	{
		return;
	}
	if (drop_template_id <= 0)
	{
		return;
	}
	std::vector<s_item_template_info> item_drop_list_with_arr;
	cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_arr, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));

	if (m_npc_template_ptr == nullptr)
	{
		return;
	}
	s_map_pos target_pos = s_map_pos();
	world_cs::create_drop_box(player_ptr->get_map_ent(),
		item_drop_list_with_arr,
		m_init_pos,
		target_pos,
		player_ptr,
		m_item_id_array,
		m_npc_template_ptr->attribute_id,
		drop_template_id);
}

void  drop_manager::create_drop_box(std::vector< s_item_template_info >& item_list, player* player_ptr, int32& drop_type, int32& drop_template_id, bool cost_time)
{

	if (false == player_ptr->can_get_drop_from_monster())
	{
		return;
	}

	if (m_npc_template_ptr == nullptr)
	{
		return;
	}
	s_map_pos target_pos = s_map_pos();
	world_cs::create_drop_box(player_ptr->get_map_ent(),
		item_list,
		m_init_pos,
		target_pos,
		player_ptr,
		m_item_id_array,
		m_npc_template_ptr->attribute_id,
		drop_template_id,
		cost_time);
}

void drop_manager::create_drop_box(int32& drop_template_id, player* player_ptr, s_map_pos& drop_pos, bool is_need_show_get_item_new)
{
	ZoneScoped;		
	if (drop_template_id <= 0)
	{
		CONSOLE_ERROR("drop_template_id:{}", drop_template_id);
		return;
	}
	if (false == player_ptr->is_valid())
	{
		CONSOLE_ERROR("player is invalid");
		return;
	}
	if (false == is_same_class(drop_template_id, player_ptr))
	{
		CONSOLE_ERROR("is_same_class is false");
		return;
	}
	if (false == player_ptr->can_get_drop_from_monster())
	{
		CONSOLE_ERROR("can_get_drop_from_monster is false");
		return;
	}
	if (true == drop_pos.is_zero())
	{
		CONSOLE_ERROR("drop_pos is zero");
		return;
	}
	std::vector<s_item_template_info> item_drop_list_with_arr;
	cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_arr, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));

	s_map_pos target_pos = s_map_pos();
	world_cs::create_drop_box(player_ptr->get_map_ent(),
		item_drop_list_with_arr,
		drop_pos,
		target_pos,
		player_ptr,
		m_item_id_array,
		0,
		drop_template_id,
		false,
		is_need_show_get_item_new);
}

std::vector< s_item_template_info > drop_manager::create_drop_box_by_drop_template_id(int32& drop_template_id, player* player_ptr, Entity* map_ent, s_map_pos& drop_pos, int32 npc_template_id, bool cost_time)
{
	std::vector< s_item_template_info > temp_list;
	temp_list.clear();
	if (drop_template_id <= 0)
	{
		return temp_list;
	}
	if (false == player_ptr->is_valid())
	{
		return temp_list;
	}
	if (false == is_same_class(drop_template_id, player_ptr))
	{
		return temp_list;
	}
	if (false == player_ptr->can_get_drop_from_monster())
	{
		return temp_list;
	}
	if (true == drop_pos.is_zero())
	{
		return temp_list;
	}

	cdrop::gen_drop_list_by_drop_id(drop_template_id, temp_list, player_ptr->get_unit_info(e_role_info_class_type), player_ptr->get_unit_info(e_role_info_exp_level));

	s_map_pos target_pos = s_map_pos();
	world_cs::create_drop_box(map_ent,
		temp_list,
		drop_pos,
		target_pos,
		player_ptr,
		m_item_id_array,
		npc_template_id,
		drop_template_id,
		cost_time);
	return temp_list;
}

hld::team_score_info drop_manager::get_team_score_info(guid_64& team_guid)
{
	int32 score_info_array_size = m_hate_team_score_info_array.size();
	for (int32 score_info_array_index = 0; score_info_array_index < score_info_array_size; score_info_array_index++)
	{
		if (team_guid == m_hate_team_score_info_array[score_info_array_index].get_team_guid())
		{
			return m_hate_team_score_info_array[score_info_array_index];
		}
	}
	return team_score_info();
}

void drop_manager::send_notice()
{
	player& last_attacker = unit_man::get_player(m_last_attacker_index);
	if (false == last_attacker.is_valid())
	{
		return;
	}
	std::string last_attacker_name = last_attacker.get_name();

	if (nullptr == m_npc_template_ptr)
	{
		return;
	}
		
	std::string notice_str = "";				// 公告拼接
	int32 notice_id = m_npc_template_ptr->Noticeld;		
	if (notice_id_broken_sky == notice_id)
	{
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(last_attacker_name);
		notice_str = init_unit::implode(vec_notice_str);
		last_attacker.get_chat_mgr().send_notice(notice_id, notice_str);
	}	
}

void drop_manager::create_drop_for_boss_damage_ranking(npc& boss_ref, int32 ranking, int32 drop_template_id, std::vector<guid_64>& team_info)
{
	if (boss_ref.is_valid() == false || team_info.empty())
		return;

	m_map_ent = boss_ref.get_map_ent();;
	m_init_pos = boss_ref.get_new_map_pos();
	m_first_attacker_index = boss_ref.get_first_attacker();;
	m_last_attacker_index = boss_ref.get_last_attack_idex();;
	m_npc_array_index = boss_ref.get_array_index();;
	m_npc_template_ptr = boss_ref.get_npc_template();

	DropTemplate* temp_drop_ptr = GET_TEMPLATE(DropTemplate, drop_template_id);
	if (nullptr == temp_drop_ptr)
		return;

	std::vector<s_gain_treasure_record_player_info_one_things> player_info_arr;
	std::set<int32> player_index_arr;
	player_info_arr.clear();
	player_index_arr.clear();

	if (temp_drop_ptr->Distribute != e_dropbox_distribute_random || team_info.size() == 1)
	{
		for (auto iter = team_info.begin(); iter != team_info.end(); ++iter)
		{
			player& member_ref = unit_man::get_player(*iter);
			if (member_ref.is_valid())
			{
				s_map_pos tmp_pos = member_ref.get_new_map_pos();
				const auto& item_list = drop_manager::create_drop_box_by_drop_template_id(drop_template_id, &member_ref, boss_ref.get_map_ent(), tmp_pos, boss_ref.get_npc_template()->attribute_id, true);
				boss_ref.send_boss_damage_helper_reward(*iter, ranking, item_list);

				boss_ref.create_gain_treasure_drop_record(member_ref, item_list, player_info_arr);

				member_ref.get_gain_treasure_mgr().set_map_rank(ranking);
				player_index_arr.insert(member_ref.get_array_index());
			}
		}
	}
	else
	{
		int32 class_type = -1;
		int32 exp_level = -1;
		int32 team_member_num = team_info.size();

		//将该队伍中玩家下标做一个数组，便于乱序
		std::vector<int32> player_ptr_index;
		player_ptr_index.clear();
		for (int32 i = 0; i < team_member_num; i++)
		{
			player& member_ref = unit_man::get_player(team_info[i]);
			if (member_ref.is_valid() == false)
				continue;

			if (i == 0 || member_ref.is_player_team_captain())
			{
				class_type = member_ref.get_unit_info(e_role_info_class_type);
				exp_level = member_ref.get_unit_info(e_role_info_exp_level);
			}
			player_ptr_index.push_back(i);
			player_index_arr.insert(member_ref.get_array_index());
		}

		s_gain_treasure_record_player_info_one_things one_player_info;
		one_player_info.clear_data();
		//分配逻辑
		std::vector<s_item_template_info> drop_list;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, drop_list, class_type, exp_level);

		std::map<int32, int32> list_and_lock;
		list_and_lock.clear();
		for (auto& it : drop_list)
		{
			list_and_lock.insert({ it.m_item_id,it.m_lock });
		}
		random_array(drop_list);
		random_array(player_ptr_index);
		int32 player_index = 0;

		std::vector< s_item_template_info > temp_list_arr[max_team_member_num];

		for (auto& it : drop_list)
		{
			if (player_index >= team_member_num)
			{
				player_index = 0;
			}
			int32 cur_player_index = player_index;
			player& member_ref = unit_man::get_player(team_info[player_ptr_index[player_index]]);
			player_index++;
			if (member_ref.is_valid() == false)
				continue;
			if (boss_ref.get_map_guid() != member_ref.get_map_guid())
			{
				continue;;
			}
			temp_list_arr[cur_player_index].push_back({ it.m_item_id, 1, list_and_lock[it.m_item_id] });

			ItemTemplate* item_temp_ptr = GET_TEMPLATE(ItemTemplate, it.m_item_id);
			if (nullptr == item_temp_ptr)
			{
				continue;
			}
			member_ref.get_gain_treasure_mgr().set_map_rank(ranking);
			auto map_type = base_map_system::get_map_type(boss_ref.get_map_ent());
			if (true == init_unit::is_map_gain_treasure(map_type))
			{
				if (item_temp_ptr->DropRecordModel > e_drop_record_model_type_no)
				{
					one_player_info.clear_data();
					std::string player_name = member_ref.get_name();
					memcpy(one_player_info.role_name, player_name.c_str(), player_name.size() >= max_name_size ? max_name_size : player_name.size());
					one_player_info.gain_item_template_id = it.m_item_id;
					one_player_info.role_guid = member_ref.get_unit_guid();
					player_info_arr.push_back(one_player_info);
				}
			}
		}

		for (int32 index = 0; index < max_team_member_num && index < team_member_num; ++index)
		{
			player& member_ref = unit_man::get_player(team_info[index]);
			if (member_ref.is_valid())
			{
				create_drop_box(temp_list_arr[index], &member_ref, temp_drop_ptr->DropType, drop_template_id, true);
				boss_ref.send_boss_damage_helper_reward(member_ref.get_unit_guid(), ranking, temp_list_arr[index]);
			}
		}
	}

	//map_object_ptr->send_gain_treasure_record_to_ws_when_npc_dead(boss_ref, player_info_arr, player_index_arr);
}
