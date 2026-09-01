/********************************************************************
created: 2016年9月13日17:46:11
file base: rank_mgr
file ext: h
author: zhy

purpose: about rankings
*********************************************************************/

#include "connection_mgr.hpp"
#include "ranking_mgr.h"
#include "utility/cs_date.hpp"
#include "logic/unit_man.h"
#include "logic/item_set.h"
#include "cell_server.hpp"
#include "time_def.hpp"
#include "logic/player.hpp"
#include "internal/core.hpp"
#include "utility/init_unit.h"

using namespace faith;
cranking_mgr::cranking_mgr()
{
	m_array_index = 0;
	clear_data();
}


cranking_mgr::~cranking_mgr(void)
{

}

void cranking_mgr::clear_data()
{
	memset(m_ranking_data, 0, e_RankingIndex_max * sizeof(*m_ranking_data));
	m_player_class_type = e_class_type_none;
	m_worship_value = 0;
	m_next_tick_value = 0;
	m_bflag_has_init = false;
}

void	cranking_mgr::tick(const int64& tick_time)
{
	if (m_next_tick_value > tick_time)
	{
		return;
	}

	m_next_tick_value = tick_time + minute_tick_time;

	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ranking_type++)
	{
		if (get_is_wait_tick_update((e_RankingIndex)ranking_type))
		{
			if (get_ranking_data_update_flag((e_RankingIndex)ranking_type))
			{
				req_sync_player_rank_data_one((e_RankingIndex)ranking_type);
			}
		}
	}

}

void cranking_mgr::init_ranking_time_and_update()
{
	int64 ranking_time = time_helper::get_cur_time_new().second;
	for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ranking_type++)
	{
		m_ranking_data[ranking_type].ranking_time = ranking_time;
		switch ((e_RankingIndex)ranking_type)
		{
		case e_RankingIndex_gs://战力总榜
		case e_RankingIndex_gs_barserker: //战力-战士
		case e_RankingIndex_gs_wizard: //战力-法师
		case e_RankingIndex_gs_guardian: //战力-骑士
		case e_RankingIndex_gs_assassinator: //战力-刺客
		case e_RankingIndex_equipment_main: //装备榜-总战力
		case e_RankingIndex_box_map_level: //主地图等级
		case e_RankingIndex_wing: //羽翼总战力
		case e_RankingIndex_elves_gs://精灵总战力
		case e_RankingIndex_mount://坐骑总战力
		case e_RankingIndex_belief: //个人榜-信仰
		case e_RankingIndex_element: //个人榜-符文
		case e_RankingIndex_feather_heart:
		case e_RankingIndex_couple:
		{
			m_ranking_data[ranking_type].bflag_is_tick_update = true;
		}
		break;
		default:
			m_ranking_data[ranking_type].bflag_is_tick_update = false;
			break;
		}
	}
}

void	cranking_mgr::init_ranking_data()
{
	if (get_has_init() == true)
	{
		return;
	}
	player& m_player_ref = unit_man::get_player(m_array_index);
	if (m_player_ref.is_valid() == false)
	{
		return;
	}
	m_bflag_has_init = true;
	m_player_class_type = m_player_ref.get_unit_info(e_role_info_class_type);
	init_ranking_time_and_update();
	//初始化cs断rangking的数据备份
	int64 temp_ranking_value = m_player_ref.get_unit_gs_value();
	set_and_sync_ranking_data(e_RankingIndex_gs, temp_ranking_value);

	// 等级排行榜 巅峰
	int32 GradeNum = m_player_ref.get_grade_num();
	if (GradeNum >= faith::player_grade_up_peakedness_num)
	{
		m_ranking_data[e_RankingIndex_level].Peak = 1;
		int32 sub_ranking_type = -1;
		switch ((e_class_type)m_player_class_type)
		{
		case e_class_type_barserker:
			sub_ranking_type = e_RankingIndex_level_barserker;
			break;
		case e_class_type_wizard:
			sub_ranking_type = e_RankingIndex_level_wizard;
			break;
		case e_class_type_guardian:
			sub_ranking_type = e_RankingIndex_level_guardian;
			break;
		case e_class_type_assassinator:
			sub_ranking_type = e_RankingIndex_level_assassinator;
			break;
		default:
			break;
		}
		if (sub_ranking_type >= 0)
		{
			m_ranking_data[sub_ranking_type].Peak = 1;
		}
	}

	temp_ranking_value = m_player_ref.get_unit_info(e_role_info_exp_level);		
	//set_and_sync_ranking_data(e_RankingIndex_level, temp_ranking_value);

	temp_ranking_value = m_player_ref.get_money_data(e_money_type_silver_bind);// +m_player_ref.get_money_data(e_money_type_silver);
	set_and_sync_ranking_data(e_RankingIndex_box_map_level, temp_ranking_value);
	m_player_ref.get_item_set().sync_whole_euip_fighting_power();
	m_player_ref.get_item_set().sync_ranking_mount();
	m_player_ref.get_item_set().sync_ranking_wing();
	m_player_ref.get_item_set().sync_ranking_spirit();
	m_player_ref.get_belief_mgr().sync_ranking_belief_data();
	m_player_ref.get_feather_heart_mgr().sync_ranking_feather_heart_data();
	m_player_ref.get_patron_saint_mgr().sync_tinder_data();
	m_player_ref.get_server_harry_mgr().sync_defend_data_ranking_data();
	m_player_ref.get_special_name_mgr().sync_ranking_special_name(e_special_name_type_military_rank);
	m_player_ref.get_special_name_mgr().sync_ranking_special_name(e_special_name_type_acheivement);
	//map_record_set& record_set = map_record_mgr::get_instance().get_map_record_set(m_player_ref.get_array_index());
	//if (record_set.is_valid() == false)
	//{
	//	return;
	//}
	//set_and_sync_ranking_data(e_RankingIndex_demonstower, record_set.get_demons_tower_info().get_data_info(e_map_record_info_score_record));
}

void	cranking_mgr::set_is_wait_tick_update(e_RankingIndex ranking_type, bool is_wait_tick)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return;
	}
	m_ranking_data[ranking_type].bflag_is_tick_update = is_wait_tick;
}

bool	cranking_mgr::get_is_wait_tick_update(e_RankingIndex ranking_type)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return false;
	}
	//m_bflag_is_tick_update[ranking_type];
	return   m_ranking_data[ranking_type].bflag_is_tick_update;
}

void    cranking_mgr::set_ranking_data_update_flag(e_RankingIndex ranking_type, bool bflag)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return;
	}
	m_ranking_data[ranking_type].is_update = bflag;
}

bool	cranking_mgr::get_ranking_data_update_flag(e_RankingIndex ranking_type)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return false;
	}
	return m_ranking_data[ranking_type].is_update;
}


void cranking_mgr::set_player_ptr(int32 array_index)
{
	m_array_index = array_index;
}

void cranking_mgr::set_ranking_value(e_RankingIndex ranking_type, const  int64& ranking_value)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return;
	}

	m_ranking_data[ranking_type].ranking_value = ranking_value;
	m_ranking_data[ranking_type].ranking_time = time_helper::get_cur_time_new().second;
}

void	cranking_mgr::save_ranking_to_db()
{
	////防止下线前 tick更新的数据没有更新
	//for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max;ranking_type++)
	//{
	//	if (m_ranking_data[ranking_type].bflag_is_tick_update)
	//	{
	//		if (m_ranking_data[ranking_type].is_update)
	//		{
	//			req_sync_player_rank_data_one((e_RankingIndex)ranking_type);
	//		}
	//	}
	//}

}

void cranking_mgr::sync_vip_level_to_ws_ranking()
{
	player& temp_player = unit_man::get_player(m_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}

	cs2ws_sync_vip_level_data req;
	req.role_guid = temp_player.get_unit_guid();
	req.vip_level = temp_player.get_vip_level(false);
	connection_mgr::getInstance().send_to_ws( &req, sizeof(req));
}


void	cranking_mgr::req_sync_player_rank_data_one(e_RankingIndex ranking_type, int32 supporting_id, int32 Peak)
{
	if (ranking_type >= e_RankingIndex_max)
	{
		return;
	}
	player& temp_player = unit_man::get_player(m_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	m_ranking_data[ranking_type].is_update = false;
	s_ranking_player_info ranking_data;
	if (false == make_ranking_data(ranking_data, ranking_type, temp_player, supporting_id, Peak))
	{
		return;
	}
	int32 server_id = temp_player.get_unit_info(e_role_info_server_id);
	cs2ws_sync_ranking_info req;
	req.role_info = ranking_data;
	req.server_id = server_id;
	int64 test_info = req.role_info.ranking_value;

	if ((ranking_type >= e_RankingIndex_time_limit_activity_cross_gs_up && ranking_type <= e_RankingIndex_time_limit_activity_cross_spirit_up) || e_RankingIndex_cross_time_limit_activity_total_recharge == ranking_type)
	{
		req.server_id = cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross);
	}

	connection_mgr::getInstance().send_to_ws( &req, sizeof(req));
}

bool	cranking_mgr::make_ranking_data(s_ranking_player_info& ranking_data, e_RankingIndex ranking_type, player& player_ref, int32& template_id, int32 Peak)
{
	if (player_ref.is_valid() == false)
	{
		return false;
	}
	if (ranking_type >= e_RankingIndex_max)
	{
		return false;
	}
	ranking_data.role_guid = player_ref.get_unit_guid();
	ranking_data.set_role_name(player_ref.get_name());
	ranking_data.save_time = m_ranking_data[ranking_type].ranking_time;
	ranking_data.ranking_type = ranking_type;
	ranking_data.ranking_value = m_ranking_data[ranking_type].ranking_value;
	ranking_data.vip_level = player_ref.get_vip_level(false);
	ranking_data.set_game_channel(player_ref.get_third_info().param14);
	if (Peak == -1)
	{
		ranking_data.Peak = m_ranking_data[ranking_type].Peak;
	}
	else 
	{
		m_ranking_data[ranking_type].Peak = Peak;
		ranking_data.Peak = Peak;
	}
	ranking_data.server_id = player_ref.get_unit_info(e_role_info_server_id);
		
		
	switch (ranking_type)
	{
	case e_RankingIndex_gs:
	case e_RankingIndex_level:
	case e_RankingIndex_box_map_level:
	case e_RankingIndex_demonstower:
	case e_RankingIndex_wing:
	case e_RankingIndex_mount:
	case e_RankingIndex_worship:
	case e_RankingIndex_arena:
	case e_RankingIndex_legion_core:
	case e_RankingIndex_gs_barserker:
	case e_RankingIndex_gs_wizard:
	case e_RankingIndex_gs_guardian:
	case e_RankingIndex_gs_assassinator:
	case e_RankingIndex_level_barserker:
	case e_RankingIndex_level_wizard:
	case e_RankingIndex_level_guardian:
	case e_RankingIndex_level_assassinator:
	case e_RankingIndex_equipment_main:
	case e_RankingIndex_belief:
	case e_RankingIndex_element:
	case e_RankingIndex_military_rank:
	case e_RankingIndex_achievement:
	case e_RankingIndex_illustrated:
	case e_RankingIndex_elves_gs:
	case e_RankingIndex_vip_level_one:
	case e_RankingIndex_vip_level_five:
	case e_RankingIndex_vip_level_seven:
	case e_RankingIndex_recharge_rmb:
	case e_RankingIndex_inspiration:
	case e_RankingIndex_time_limit_activity_total_recharge:
	case e_RankingIndex_time_limit_activity_total_cost:
	case e_RankingIndex_time_limit_activity_treasure:
	case e_RankingIndex_cross_server_plinder_guard_value:
	case e_RankingIndex_feather_heart:
	case e_RankingIndex_time_limit_activity_gs_up:
	case e_RankingIndex_time_limit_activity_mount_up:
	case e_RankingIndex_time_limit_activity_wing_up:
	case e_RankingIndex_time_limit_activity_equip_up:
	case e_RankingIndex_time_limit_activity_spirit_up:
	case e_RankingIndex_time_limit_activity_cross_gs_up:
	case e_RankingIndex_time_limit_activity_cross_mount_up:
	case e_RankingIndex_time_limit_activity_cross_wing_up:
	case e_RankingIndex_time_limit_activity_cross_equip_up:
	case e_RankingIndex_time_limit_activity_cross_spirit_up:
	case e_RankingIndex_cross_time_limit_activity_total_recharge:
	{
		ranking_data.set_supporting_name(player_ref.get_player_legion_name());
		return true;
	}
	break;
	case e_RankingIndex_equipment_weapon:
	case e_RankingIndex_equipment_necklace:
	case e_RankingIndex_equipment_armor:
	case e_RankingIndex_equipment_pants:
	case e_RankingIndex_equipment_hat:
	case e_RankingIndex_equipment_shoes:
	case e_RankingIndex_equipment_glove:
	case e_RankingIndex_equipment_ring:
	case e_RankingIndex_equipment_earring:
	case e_RankingIndex_mount_train:
	case e_RankingIndex_wing_train:
	{
		if (-1 == template_id)
		{
			return false;
		}
		ranking_data.supporting_guid.A = template_id;				//装备id
		return true;
	}
	break;
	default:
		break;
	}
	return false;
}

bool cranking_mgr::check_is_need_set(e_RankingIndex ranking_type, const int64& ranking_value)
{
	if (m_ranking_data[ranking_type].ranking_value == ranking_value || ranking_value < 0)
	{
		return false;
	}

	return true;
}
void cranking_mgr::set_and_sync_ranking_data_str(e_RankingIndex ranking_type, xstring ranking_value_str, int32 supporting_data_id, int32 Peak)
{
	int64 ranking_value = init_unit::change_string_to_i64(ranking_value_str);
	set_and_sync_ranking_data(ranking_type, ranking_value, supporting_data_id, Peak);
}
void cranking_mgr::set_and_sync_ranking_data(e_RankingIndex ranking_type, const int64& ranking_value, int32 supporting_data_id,int32 Peak)
{
	player& temp_player = unit_man::get_player(m_array_index);
	if (!temp_player.is_valid() /*|| cell_server::getInstance().get_is_self_server(temp_player.get_unit_info(e_role_info_server_id)) == false*/)
	{
		return;
	}
	if (false == check_is_need_set(ranking_type, ranking_value) && Peak == -1)
	{
		return;
	}
	set_ranking_value(ranking_type, ranking_value);
	set_sub_class_type_ranking_data(ranking_type, ranking_value, Peak);
	if (false == m_bflag_has_init)
	{
		return;
	}
	temp_player.get_service_rank_mgr().check_receive_reward_state(ranking_type, ranking_value);
	m_ranking_data[ranking_type].is_update = true;
	if (get_is_wait_tick_update(ranking_type))
	{
		return;
	}
	req_sync_player_rank_data_one(ranking_type, supporting_data_id, Peak);
}
void cranking_mgr::set_and_sync_single_equip_ranking_data(citem* item_ptr)
{
	if (nullptr == item_ptr)
	{
		return;
	}
	int64 ranking_value = item_ptr->calcu_single_item_fighting_power(m_array_index);
	ItemTemplate* equip_ptr = item_ptr->get_item_info_ptr();
	if (nullptr == equip_ptr)
	{
		return;
	}
	switch ((e_item_type)equip_ptr->item_type)
	{
	case e_item_type_weapon:
	{
		set_and_sync_ranking_data(e_RankingIndex_equipment_weapon, ranking_value, equip_ptr->attribute_id);
	}
	break;
	case e_item_type_armor:
	{
		switch (equip_ptr->sub_type)
		{
		case e_armor_sub_type_hat:
			set_and_sync_ranking_data(e_RankingIndex_equipment_hat, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_earring:
			set_and_sync_ranking_data(e_RankingIndex_equipment_earring, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_necklace:
			set_and_sync_ranking_data(e_RankingIndex_equipment_necklace, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_armor:
			set_and_sync_ranking_data(e_RankingIndex_equipment_armor, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_pants:
			set_and_sync_ranking_data(e_RankingIndex_equipment_pants, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_ring:
			set_and_sync_ranking_data(e_RankingIndex_equipment_ring, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_glove:
			set_and_sync_ranking_data(e_RankingIndex_equipment_glove, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_boot:
			set_and_sync_ranking_data(e_RankingIndex_equipment_shoes, ranking_value, equip_ptr->attribute_id);
			break;
		case e_armor_sub_type_amulet:
		case e_armor_sub_type_fashion:
		case e_armor_sub_type_wedding:
		case e_armor_sub_type_spirit:
		case e_armor_sub_type_mantle:
			break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}


void cranking_mgr::set_sub_class_type_ranking_data(e_RankingIndex ranking_type, const int64& ranking_value, int32 Peak)
{
	int32 sub_ranking_type = -1;
	switch (ranking_type)
	{
	case faith::e_RankingIndex_gs:
	{
		switch ((e_class_type)m_player_class_type)
		{
		case e_class_type_barserker:
			sub_ranking_type = e_RankingIndex_gs_barserker;
			break;
		case e_class_type_wizard:
			sub_ranking_type = e_RankingIndex_gs_wizard;
			break;
		case e_class_type_guardian:
			sub_ranking_type = e_RankingIndex_gs_guardian;
			break;
		case e_class_type_assassinator:
			sub_ranking_type = e_RankingIndex_gs_assassinator;
			break;
		default:
			break;
		}
	}
	break;
	case faith::e_RankingIndex_level:
	{
		switch ((e_class_type)m_player_class_type)
		{
		case e_class_type_barserker:
			sub_ranking_type = e_RankingIndex_level_barserker;
			break;
		case e_class_type_wizard:
			sub_ranking_type = e_RankingIndex_level_wizard;
			break;
		case e_class_type_guardian:
			sub_ranking_type = e_RankingIndex_level_guardian;
			break;
		case e_class_type_assassinator:
			sub_ranking_type = e_RankingIndex_level_assassinator;
			break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
	if (-1 != sub_ranking_type)
	{
		set_and_sync_ranking_data((e_RankingIndex)sub_ranking_type, ranking_value, -1, Peak);
	}
	return;
}
