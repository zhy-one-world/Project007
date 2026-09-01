/********************************************************************
created: 2014/07/14
created: 14:7:2014 19:40
file base: item_set
file ext: cpp
author: zhy

purpose:
*********************************************************************/

#include "buff_item.h"
#include "cache_mgr.hpp"
#include "char_def.hpp"
#include "connection_mgr.hpp"
#include "goods_def.hpp"
#include "internet/item.pb.h"
#include "item_def.hpp"
#include "logic/buff_man.h"
#include "logic/drop.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"

#include "internet/net.pb.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "time.hpp"
#include "utility/random.h"
#include "internet/error.pb.h"
#include "internet/character.pb.h"
#include "npc.hpp"
#include "utility/parse_msg.h"
#include "internal/item_msg.hpp"
#include "internet/goods.pb.h"
#include "internal/char_msg.hpp"
#include "internal/lucky_draw_record_msg.hpp"

using namespace faith;

item_set::item_set()
{
	m_unit_array_index = 0;
	m_item_bag_length[e_bag_type_none] = 0;
	m_item_bag_length[e_bag_type_equip] = e_role_equip_slot_max;
	m_item_bag_length[e_bag_type_bag] = MAX_ITEM_BAG_NUM;
	m_item_bag_length[e_bag_type_wing] = MAX_WING_ELEMENTS_NUM;
	m_item_bag_length[e_bag_type_mount] = MAX_MOUNT_NUM;
	m_item_bag_length[e_bag_type_protect] = max_proctect_mark_bag_slot_num;
	m_item_bag_length[e_bag_type_equip_protect] = max_proctect_mark_equip_slot_num;
	m_item_bag_length[e_bag_type_mail] = 0;
	m_item_bag_length[e_bag_type_hope_item] = max_hope_item_bag_size;
	m_item_bag_length[e_bag_type_elemenet] = element_heart_bag_num;
	m_item_bag_length[e_bag_type_equip_elemenet] = element_heart_equip_num;
	m_item_bag_length[e_bag_type_fasion] = max_fasion_item_bag_size;
	m_item_bag_length[e_bag_type_equip_fasion] = e_fashion_equip_slot_max;
	m_item_bag_length[e_bag_type_mount_beast_spirit] = MAX_BEAST_SPIRIT_NUM;
	m_item_bag_length[e_bag_type_spirit_beast_spirit] = MAX_BEAST_SPIRIT_NUM;
	m_item_bag_length[e_bag_type_wing_beast_spirit] = MAX_BEAST_SPIRIT_NUM;
	m_item_bag_length[e_bag_type_treasure] = MAX_ITEM_TREASURE_NUM;
	m_item_bag_length[e_bag_type_storage] = max_item_storage_num;
	m_item_bag_length[e_bag_type_belief_rune] = belief_rune_bag_num;
	m_item_bag_length[e_bag_type_belief_rune_equip] = belief_rune_equip_bag_num;
	m_item_bag_length[e_bag_type_equip_spirit] = sprite_bag_num;
	m_item_bag_length[e_bag_type_equip_tinder_prop] = tinder_prop_bag_num;
	m_item_bag_length[e_bag_type_skill_book] = max_feather_heart_bag_num;
	m_item_bag_length[e_bag_type_goddess_equip] = max_goddess_equip_num;
	m_item_bag_length[e_bag_type_wedding_equip] = max_wedding_equip_num;
	m_item_bag_length[e_bag_type_core_element] = core_element_num;
	m_item_bag_length[e_bag_type_sky_equip] = sky_equip_slot_num;
	m_item_bag_length[e_bag_type_skygod_equip] = skygod_equip_slot_num;
	m_item_bag_length[e_bag_type_supreme_equip] = supreme_equip_slot_num;
	m_item_bag_length[e_bag_type_spirit_upgrade_promote] = upgrade_promote_spirit_max;
	m_item_bag_length[e_bag_type_break_will] = break_will_max;
	m_item_bag_length[e_bag_type_awaken_item] = awaken_fetter_bag_max;
	m_item_bag_length[e_bag_type_online_award] = MAX_ITEM_BAG_NUM;
	clear_data();
}

item_set::~item_set(void)
{
	clear_data();
}

void item_set::heart_tick(const int64& new_time)
{
	if (m_unit_array_index >= npc_arrary_index_begin)
	{
		return;
	}
	if (m_item_use_cd.empty() == false)
	{
		std::map<int32, int64>::iterator cd_map_ite;
		for (cd_map_ite = m_item_use_cd.begin(); cd_map_ite != m_item_use_cd.end();)
		{
			std::map<int32, int64>::iterator temp_it = cd_map_ite++;
			if (new_time >= temp_it->second)
			{
				m_item_use_cd.erase(temp_it);
			}
		}
	}
	check_time_limit_item();
	if (m_tidy_type_list.size() > 0)
	{
		if (m_item_tidy_cd < new_time)
		{
			int32 bag_type = m_tidy_type_list[0];
			if (tidy_item((e_bag_type)bag_type))
			{
				add_and_del_tidy_type((e_bag_type)bag_type, false);
			}
		}
	}
}

void item_set::set_unit_ptr(int32 unit_array_index)
{
	m_unit_array_index = unit_array_index;
}

void item_set::clear_data()
{
	m_item_load_flag = 0;
	m_item_tidy_cd = 0.0f;
	m_fashion_star_num = 0;
	m_bind_material_num = 0;
	m_no_bind_material_num = 0;
	for (auto it = m_item_map.begin(); it != m_item_map.end(); ++it)
	{
		cache_mgr::get_instance().back_item_ptr(it->second);
	}
	m_item_map.clear();
	m_quick_call_mount_array.clear();
	for (int32 i = e_bag_type_none; i < e_bag_type_max; ++i)
	{
		m_item_load_flag += i;
		m_item_bag_array[i].clear();
	}
	m_wing_att_change = false;
	m_mount_att_change = false;
	m_show_wing_item_ptr = nullptr;
	m_show_mount_item_ptr = nullptr;
}


void item_set::save_item_to_db(int32 save_type)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}
	item_proto_item_save_load request_save;
	for (int32 i = e_bag_type_none + 1; i < e_bag_type_max; ++i)
	{
		request_save.Clear();
		auto& guid_array = GET_BAG(i);
		request_save.set_bag_type(i);
		for (auto& it : guid_array)
		{
			citem* item_ptr = get_item(it.second);
			if (item_ptr)
			{
				const s_item_info& item_info = item_ptr->get_item_inst();
				request_save.add_item_guid(item_info.item_guid.server_64);
				request_save.add_param_str(item_info.param_str);
				for (int32 k = 0; k < e_item_info_max; ++k)
				{
					request_save.add_data_ary(item_info.data_ary[k]);
				}
			}
		}
		player_ref.send_message_to_dp(&request_save, e_msgindex_cs2dp_save_char_item, save_type);
	}
	request_save.Clear();
	request_save.set_bag_type(e_bag_type_none);
	player_ref.send_message_to_dp(&request_save, e_msgindex_cs2dp_save_char_item, save_type);
}

void item_set::load_item_by_db(const item_proto_item_save_load& load_proto)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	int32 item_num = load_proto.item_guid_size();
	for (int32 i = 0; i < item_num; i++)
	{
		s_item_info item_info;
		item_info.item_guid = load_proto.item_guid(i);
		for (int32 j = 0; j < e_item_info_max; ++j)
		{
			item_info.data_ary[j] = load_proto.data_ary(i * e_item_info_max + j);
		}
		memcpy(item_info.param_str, load_proto.param_str(i).c_str(), load_proto.param_str(i).size());
		create_item_by_info(item_info, e_server_log_add_item_load_db, 0, false);
	}
	PlayerInitTemplate* player_init_template_ptr = nullptr;
	if (player_ref.is_valid())
	{
		player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, player_ref.get_unit_info(e_role_info_template_id));
		if (player_init_template_ptr)
		{
			int32* init_data = nullptr;
			e_bag_type init_bag_type = (e_bag_type)load_proto.bag_type();
			int32 init_len = 0;
			switch (init_bag_type)
			{
			case faith::e_bag_type_wing:
				init_data = player_init_template_ptr->InitWing.data();
				init_len = player_init_template_ptr->InitWing.size();
				break;
			case faith::e_bag_type_mount:
				init_data = player_init_template_ptr->InitMount.data();
				init_len = player_init_template_ptr->InitMount.size();
				break;
			case faith::e_bag_type_spirit_upgrade_promote:
				init_data = player_init_template_ptr->InitStarMapSpirit.data();
				init_len = player_init_template_ptr->InitStarMapSpirit.size();
				break;
			case faith::e_bag_type_equip_protect:
				init_data = player_init_template_ptr->InitProtectMark.data();
				init_len = player_init_template_ptr->InitProtectMark.size();
				init_bag_type = e_bag_type_protect;
				break;
			case faith::e_bag_type_equip_fasion:
				init_data = player_init_template_ptr->InitFashion.data();
				init_len = player_init_template_ptr->InitFashion.size();
				init_bag_type = e_bag_type_fasion;
				break;
			case faith::e_bag_type_goddess_equip:
				init_data = player_init_template_ptr->InitGoddess.data();
				init_len = player_init_template_ptr->InitGoddess.size();
				init_bag_type = e_bag_type_goddess_equip;
				break;
			case faith::e_bag_type_wedding_equip:
				init_data = player_init_template_ptr->InitWedding.data();
				init_len = player_init_template_ptr->InitWedding.size();
				init_bag_type = e_bag_type_wedding_equip;
				break;
			case faith::e_bag_type_core_element:
				init_data = player_init_template_ptr->InitCoreElement.data();
				init_len = player_init_template_ptr->InitCoreElement.size();
				init_bag_type = e_bag_type_core_element;
				break;
			case faith::e_bag_type_break_will:
				init_data = player_init_template_ptr->InitBreakWill.data();
				init_len = player_init_template_ptr->InitBreakWill.size();
				init_bag_type = e_bag_type_break_will;
				break;
			case faith::e_bag_type_awaken_item:
				init_data = player_init_template_ptr->InitAwakenItem.data();
				init_len = player_init_template_ptr->InitAwakenItem.size();
				init_bag_type = e_bag_type_awaken_item;
				break;
			}
			if (init_data)
			{
				bool not_need_load = false;
				int32 region_code = (int32)e_version_region_type_china_main;
				RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
				if (region_template_ptr != nullptr)
				{
					region_code = region_template_ptr->RegionCode;
				}
				std::vector<int32> region_limit_arr;
				for (int32 i = 0; i < init_len; ++i)
				{
					not_need_load = false;
					region_limit_arr.clear();
					ItemTemplate* item_temp_ptr = GET_TEMPLATE(ItemTemplate, init_data[i]);
					if (nullptr != item_temp_ptr)
					{

						if (init_bag_type == faith::e_bag_type_wing)
						{
							WingTemplate* wing_temp_ptr = GET_TEMPLATE(WingTemplate, item_temp_ptr->logic_id);
							if (nullptr != wing_temp_ptr)
							{
								region_limit_arr = wing_temp_ptr->RegionArr;
							}
						}
						else if (init_bag_type == faith::e_bag_type_mount)
						{
							MountTemplate* mount_temp_ptr = GET_TEMPLATE(MountTemplate, item_temp_ptr->logic_id);
							if (nullptr != mount_temp_ptr)
							{
								region_limit_arr = mount_temp_ptr->RegionArr;
							}
						}
						else if (init_bag_type == faith::e_bag_type_fasion)
						{
							FasionTemplate* fasion_temp_ptr = GET_TEMPLATE(FasionTemplate, item_temp_ptr->logic_id);
							if (nullptr != fasion_temp_ptr)
							{
								region_limit_arr = fasion_temp_ptr->RegionArr;
							}
						}
						else if (init_bag_type == faith::e_bag_type_protect)
						{
							ProtectSpiritTemplate* protect_temp_ptr = GET_TEMPLATE(ProtectSpiritTemplate, item_temp_ptr->logic_id);
							if (nullptr != protect_temp_ptr)
							{
								region_limit_arr = protect_temp_ptr->RegionArr;
							}
						}
					}

					if (region_limit_arr.size() > 0)
					{
						for (int32 j = 0; j < region_limit_arr.size(); j++)
						{
							if (region_code == region_limit_arr[j])
							{
								not_need_load = true;
								break;
							}
						}
					}
					if (not_need_load)
					{
						continue;
					}
					citem* item_ptr = get_item_by_template_id(init_data[i]);
					if (item_ptr == nullptr)
					{
						int32 empty_slot = get_empty_slot(init_bag_type);
						if (empty_slot < 0)
						{
							break;
						}
						s_item_info item_info;
						if (init_unit::init_item_data(item_info, init_data[i], empty_slot, 1, init_bag_type, 0))
						{
							create_item_by_info(item_info, e_server_log_add_item_load_create, 0, false);
						}
						if (init_bag_type == faith::e_bag_type_fasion)
						{
							citem* new_item_ptr = get_item_by_template_id(init_data[i]);
							if (new_item_ptr != nullptr)
							{
								FasionTemplate* fasion_temp_ptr = GET_TEMPLATE(FasionTemplate, item_temp_ptr->logic_id);
								if (nullptr != fasion_temp_ptr)
								{
									if (fasion_temp_ptr->NeedFasionId.size() > 0)
									{
										for (int32 fasion_num = 0; fasion_num < fasion_temp_ptr->NeedFasionId.size(); ++fasion_num)
										{
											citem* need_item_ptr = get_item_by_template_id(fasion_temp_ptr->NeedFasionId[fasion_num]);
											if (need_item_ptr == nullptr)
											{
												continue;
											}
											if (need_item_ptr->get_data_info(e_item_info_activate) > 0)
											{
												new_item_ptr->set_data_info(e_item_info_activate, 1);
												new_item_ptr->set_data_info(e_item_info_upgrade_count, 1);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	m_item_load_flag -= load_proto.bag_type();
	if (m_item_load_flag <= 0)
	{
		all_wing_sort();
		send_item_all();
		set_showed_mount();
		set_showed_wing();
		player_ref.set_loading_flag(e_data_flag_item);
	}
}

bool item_set::load_item_by_db_lua(const char* data_ptr, int32 data_len)
{

	item_proto_item_save_load load_proto;

	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&load_proto, data_ptr, data_len, false);

	if (!is_sucess)
	{
		return false;
	}

	load_item_by_db(load_proto);
	return true;

}

void item_set::init_skill_and_item_gs()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid())
	{
		set_equip_skill_buff_all();
		set_equip_att_all();
		calcu_addition(e_addition_buff_upgrade, false);
		calcu_addition(e_addition_buff_quality, false);
		calcu_addition(e_addition_buff_feather, false);
		calcu_addition(e_addition_buff_addon, false);
		calcu_addition(e_addition_buff_succinct, false);
		calcu_addition(e_addition_buff_sky_equip, false);
		calcu_addition(e_addition_buff_skygod_equip, false);
		calcu_addition(e_addition_buff_supreme_equip, false);
		calcu_addition(e_addition_buff_enchant, false);
		calcu_addition(e_addition_buff_goddess, false);
		if (player_ref.is_valid())
		{
			//player_ref.get_element_heart_mgr().set_element_heart_all_att(true);
			player_ref.get_belief_rune_mgr().set_belief_rune_all_att(true);
			player_ref.get_feather_heart_mgr().set_feather_heart_attr_all(true);
		}

	}
}

void item_set::send_item_all()
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	item_proto_update_character_item item_msg;
	item_msg.add_role_guid(temp_unit.get_unit_guid().A);
	item_msg.add_role_guid(temp_unit.get_unit_guid().B);
	item_msg.set_update_mode(e_update_item_info_mode_update_all);

	for (auto ite = m_item_map.begin(); ite != m_item_map.end(); ++ite)
	{
		citem* item_ptr = ite->second;
		if (nullptr == item_ptr->get_item_info_ptr())
		{
			continue;
		}
		if (item_ptr->get_item_info_ptr()->item_type == e_item_type_spirit && item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip_spirit)
		{
			send_spirit_msg(item_ptr);
		}
		item_ptr->get_item_msg(item_msg);
		if (item_msg.item_list_size() > send_item_max_num)
		{
			temp_unit.send_message_to_self(&item_msg, e_msgindex_s2c_item_update_character);
			item_msg.clear_item_list();
		}
	}
	if (item_msg.item_list_size() > 0)
	{
		temp_unit.send_message_to_self(&item_msg, e_msgindex_s2c_item_update_character);
	}
}

void item_set::change_buff_time()
{
	citem* item_ptr = get_item_by_template_id(special_mission_wing_id);
	if (nullptr != item_ptr)
	{
		int32 item_over_time = item_ptr->get_data_info(e_item_info_over_time);
		int64 cur_time = time_helper::get_cur_time_new().second;
		if (0 == item_ptr->get_data_info(e_item_info_activate))
		{
			buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, special_wing_buff_id);
			return;
		}
		if (cur_time < item_over_time)
		{
			buff_item* buff_ptr = buff_man::get_buff_item_by_template_id(m_unit_array_index, m_unit_array_index, special_wing_buff_id);
			if (nullptr != buff_ptr)
			{
				buff_ptr->change_life_time(item_over_time - cur_time);
				return;
			}
		}
		else
		{
			buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, special_wing_buff_id);
			return;
		}
	}

}

s_item_template_info item_set::get_const_att_item_by_id(int32 item_tem_id, int32 item_num, int32 item_const_att, int32 item_lock, int32 class_type, int32 exp_level)
{
	s_item_template_info item_info(item_tem_id, item_num, item_lock);

	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_tem_id);
	if (item_template_ptr == nullptr)
	{
		return item_info;
	}

	if (item_template_ptr->item_type == e_item_type_expendable && item_template_ptr->sub_type == e_prop_sub_type_const_att_equip)
	{
		PropBasicAttributeTemplate* prop_basic_temp_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, item_template_ptr->logic_id);
		if (nullptr == prop_basic_temp_ptr)
		{
			return item_info;
		}
		int32 drop_temp_id = prop_basic_temp_ptr->ItemGet;
		std::vector<s_item_template_info> item_drop_list_with_att;
		cdrop::gen_drop_list_by_drop_id(drop_temp_id, item_drop_list_with_att, class_type, exp_level);

		std::vector<s_item_template_info> items_with_num_list;
		cdrop::merge_drop_bag(item_drop_list_with_att, items_with_num_list, class_type);
		if (items_with_num_list.size() <= 0)
		{
			return item_info;
		}
		item_info = items_with_num_list[0];
	}
	return item_info;
}


void item_set::all_wing_sort()
{
	citem* temp_item = nullptr;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		if (GET_BAG(e_bag_type_wing)[i].is_valid())
		{
			temp_item = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_item != nullptr)
			{
				temp_item->set_data_info(e_item_info_slot, -1);
			}
		}
	}

	int32 wing_slot = 0;
	temp_item = nullptr;
	for (int32 index = 0; index < MAX_WING_ELEMENTS_NUM; index++)
	{
		citem* choosed_item = nullptr;
		int32 choosed_view_order = 99999;
		for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
		{
			if (!GET_BAG(e_bag_type_wing)[i].is_valid())
			{
				continue;
			}
			temp_item = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_item == nullptr || temp_item->get_data_info(e_item_info_slot) != -1)
			{
				continue;
			}
			if (choosed_item == nullptr)
			{
				choosed_item = temp_item;
				WingTemplate* temp_wing_template_ptr = choosed_item->get_wing_template_ptr();
				if (nullptr == temp_wing_template_ptr)
				{
					continue;
				}
				choosed_view_order = temp_wing_template_ptr->ViewOrder;
				continue;
			}
			else
			{
				WingTemplate* temp_wing_template_ptr = temp_item->get_wing_template_ptr();
				if (nullptr == temp_wing_template_ptr)
				{
					continue;
				}
				int32 temp_view_order = temp_wing_template_ptr->ViewOrder;
				if (temp_view_order < choosed_view_order)
				{
					choosed_item = temp_item;
					choosed_view_order = temp_view_order;
				}
			}
		}
		if (choosed_item == nullptr)
		{

		}
		else
		{
			choosed_item->set_data_info(e_item_info_slot, wing_slot);
			wing_slot++;
		}
	}
}

void item_set::send_wing_item_all()
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}

	item_proto_update_character_item item_msg;
	item_msg.add_role_guid(temp_unit.get_unit_guid().A);
	item_msg.add_role_guid(temp_unit.get_unit_guid().B);

	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		if (GET_BAG(e_bag_type_wing)[i].is_valid())
		{
			citem* temp_wing_ptr = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_wing_ptr != nullptr)
			{
				temp_wing_ptr->get_item_msg(item_msg);
			}
			if (item_msg.item_list_size() > send_item_max_num)
			{
				temp_unit.send_message(&item_msg, e_msgindex_s2c_item_update_character);
				item_msg.clear_item_list();
			}
		}
	}
	if (item_msg.item_list_size() > 0)
	{
		temp_unit.send_message(&item_msg, e_msgindex_s2c_item_update_character);
	}
}

void item_set::send_item_tidy_guid_array(const std::unordered_map<int32, guid_64>& guid_arr)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	item_proto_item_operation_tidy tidy_msg;

	for (auto& it : guid_arr)
	{
		tidy_msg.add_item_guid_list(it.second.A);
		tidy_msg.add_item_guid_list(it.second.B);
	}

	temp_unit.send_message(&tidy_msg, e_msgindex_s2c_item_tidy);
}

void item_set::send_item_sell_earn_info(int32 money_id, int32 money_num)
{
	item_proto_item_sell_earn_info sell_earn_info;
	sell_earn_info.set_earn_money_id(money_id);
	sell_earn_info.set_earn_money(money_num);

	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	temp_unit.send_message_to_self(&sell_earn_info, e_msgindex_s2c_item_sell_earn_info);
}

void item_set::send_item_composit_response(int32 composit_item_template_id, vector<int32>& result_code)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	item_proto_item_composit_end item_composit_response;
	item_composit_response.add_role_guid(temp_unit.get_unit_guid().A);
	item_composit_response.add_role_guid(temp_unit.get_unit_guid().B);
	item_composit_response.set_composit_item_template_id(composit_item_template_id);
	for (int32 i = 0; i < result_code.size(); i++)
	{
		item_composit_response.add_result_code(result_code[i]);
	}

	temp_unit.send_message(&item_composit_response, e_msgindex_s2c_item_composit);
}

void item_set::send_item_equip(unit& temp_unit)
{
	unit& self_unit = unit_man::get_unit(m_unit_array_index);
	if (self_unit.is_valid() == false)
	{
		return;
	}
	item_proto_update_character_item item_msg;
	item_msg.add_role_guid(self_unit.get_unit_guid().A);
	item_msg.add_role_guid(self_unit.get_unit_guid().B);
	for (int32 equip_slot = e_role_equip_slot_weapon_1; equip_slot <= e_role_equip_slot_magic_2; equip_slot++)
	{
		citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[equip_slot]);
		if (item_ptr)
		{
			item_proto_item_info* item_info_ptr = item_msg.add_item_list();
			item_info_ptr->set_item_guid(item_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
			{
				item_info_ptr->add_item_data(item_ptr->get_data_info((e_item_info)i));
			}
		}
	}
	citem* mount_ptr = get_showed_mount();
	if (mount_ptr)
	{
		item_proto_item_info* item_info_ptr = item_msg.add_item_list();
		item_info_ptr->set_item_guid(mount_ptr->get_item_guid().server_64);
		for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
		{
			item_info_ptr->add_item_data(mount_ptr->get_data_info((e_item_info)i));
		}
	}
	citem* wing_ptr = get_showed_wing();
	if (wing_ptr)
	{
		item_proto_item_info* item_info_ptr = item_msg.add_item_list();
		item_info_ptr->set_item_guid(wing_ptr->get_item_guid().server_64);
		for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
		{
			item_info_ptr->add_item_data(wing_ptr->get_data_info((e_item_info)i));
		}
	}
	temp_unit.send_message_to_self(&item_msg, e_msgindex_s2c_item_update_character);
}
void item_set::send_item_one(citem* item_ref, bool to_self, e_update_item_info_mode update_mode)
{
	if (nullptr == item_ref)
	{
		return;
	}
	if (nullptr == item_ref->get_item_info_ptr())
	{
		return;
	}
	if (item_ref->get_item_info_ptr()->item_type == e_item_type_spirit && item_ref->get_data_info(e_item_info_container_type) == e_bag_type_equip_spirit)
	{
		send_spirit_msg(item_ref);
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (false == temp_unit.is_valid())
	{
		return;
	}
	if (to_self)
	{
		send_item_one_self(item_ref, update_mode);
	}
	item_proto_update_character_item item_msg;
	item_msg.add_role_guid(temp_unit.get_unit_guid().A);
	item_msg.add_role_guid(temp_unit.get_unit_guid().B);
	item_msg.set_update_mode(update_mode);
	item_ref->get_item_msg(item_msg, false);
	temp_unit.send_message_to_aoi(&item_msg, e_msgindex_s2c_item_update_character, false);
}
void item_set::send_item_one_self(citem* item_ref, e_update_item_info_mode update_mode)
{
	unit& unit_ref = unit_man::get_unit(m_unit_array_index);
	item_proto_update_character_item item_msg;
	item_msg.add_role_guid(unit_ref.get_unit_guid().A);
	item_msg.add_role_guid(unit_ref.get_unit_guid().B);
	item_msg.set_update_mode(update_mode);
	item_ref->get_item_msg(item_msg);
	unit_ref.send_message_to_self(&item_msg, e_msgindex_s2c_item_update_character);
}

void item_set::set_equip_skill_buff_all()
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	for (int32 i = 0; i < e_role_equip_slot_max; ++i)
	{
		if (GET_BAG(e_bag_type_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i].server_64);
			if (item_ptr)
			{
				item_ptr->set_unit_skill(m_unit_array_index, true);
				item_ptr->set_unit_buff(m_unit_array_index, true);
				item_ptr->set_unit_equip_passive_skill(m_unit_array_index, true);
			}
		}
	}
	reset_fashion_effect();
	if (temp_unit.get_unit_info(e_role_info_show_sky_suit_shape) == 1)
	{
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, sky_equip_buff_id);
	}
	if (temp_unit.get_unit_info(e_role_info_show_sky_suit_shape) == 2)
	{
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, skygod_equip_buff_id);
	}
	if (temp_unit.get_unit_info(e_role_info_show_sky_suit_shape) == 3)
	{
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, supreme_equip_buff_id);
	}
	citem* showed_wing_item = get_showed_wing();
	citem* showed_mount_item = get_showed_mount();

	if (showed_wing_item)
	{
		showed_wing_item->set_unit_skill(m_unit_array_index, true);
		showed_wing_item->set_unit_buff(m_unit_array_index, true);
	}

	if (showed_mount_item)
	{
		showed_mount_item->set_unit_skill(m_unit_array_index, true);
		showed_mount_item->set_unit_buff(m_unit_array_index, true);
	}
}

void item_set::set_equip_att_all()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (true == player_ref.is_valid())
	{
		player_ref.set_is_have_cant_equip_item(false);
	}
	for (int32 i = 0; i < e_role_equip_slot_mount; ++i)
	{
		if (GET_BAG(e_bag_type_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i].server_64);
			if (item_ptr)
			{
				bool can_add = is_equipment_effective(item_ptr->get_item_guid());
				if (true == can_add)
				{
					set_equip_att(item_ptr, true);
					item_ptr->set_add_att(true);
				}
				else if (false == can_add)
				{
					item_ptr->set_add_att(false);
					if (true == player_ref.is_valid())
					{
						player_ref.set_is_have_cant_equip_item(true);
					}
				}
			}
		}
	}


	m_fashion_star_num = 0;
	for (int32 i = 0; i < e_fashion_equip_slot_max; ++i)
	{
		if (GET_BAG(e_bag_type_equip_fasion)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_equip_fasion)[i].server_64);
			if (item_ptr)
			{
				citem* item_fashion = get_item(GET_BAG(e_bag_type_equip_fasion)[i]);
				int32 star_num = item_fashion->get_data_info(e_item_info_upgrade_count);
				if (item_ptr->get_data_info(e_item_info_activate) > 0 && star_num > 0)
				{
					m_fashion_star_num += star_num;
					set_fashion_att(item_ptr, true, star_num);
				}
			}
		}
	}


	for (int32 i = 0; i < max_fasion_item_bag_size; ++i)
	{
		if (GET_BAG(e_bag_type_fasion)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_fasion)[i].server_64);
			if (item_ptr)
			{
				citem* item_fashion = get_item(GET_BAG(e_bag_type_fasion)[i]);
				int32 star_num = item_fashion->get_data_info(e_item_info_upgrade_count);
				if (item_ptr->get_data_info(e_item_info_activate) > 0 && star_num > 0)
				{
					m_fashion_star_num += star_num;
					set_fashion_att(item_ptr, true, star_num);
				}
			}
		}
	}

	for (int32 i = 0; i < sky_equip_slot_num; i++)
	{
		if (GET_BAG(e_bag_type_sky_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_sky_equip)[i].server_64);
			if (item_ptr)
			{
				if (item_ptr->get_data_info(e_item_info_activate) > 0)
				{
					set_sky_equip_att(item_ptr, true);
				}
			}
		}
	}

	for (int32 i = 0; i < skygod_equip_slot_num; i++)
	{
		if (GET_BAG(e_bag_type_skygod_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_skygod_equip)[i].server_64);
			if (item_ptr)
			{
				if (item_ptr->get_data_info(e_item_info_activate) > 0)
				{
					set_skygod_equip_att(item_ptr, true);
				}
			}
		}
	}

	for (int32 i = 0; i < supreme_equip_slot_num; i++)
	{
		if (GET_BAG(e_bag_type_supreme_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_supreme_equip)[i].server_64);
			if (item_ptr)
			{
				if (item_ptr->get_data_info(e_item_info_activate) > 0)
				{
					set_supreme_equip_att(item_ptr, true);
				}
			}
		}
	}

	for (int32 i = 0; i < max_goddess_equip_num; i++)
	{
		if (GET_BAG(e_bag_type_goddess_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_goddess_equip)[i].server_64);
			if (item_ptr)
			{
				ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
				if (item_template_ptr)
				{
					if (item_template_ptr->sub_type == 0)
					{
						if (item_ptr->get_data_info(e_item_info_is_first) > 0)
						{
							set_goddess_equip_att(item_ptr, true);
							player_ref.get_goddess_mgr().set_star_is_open(item_ptr, true);
						}
						if (item_ptr->get_data_info(e_item_info_activate) > 0)
						{
							player_ref.get_goddess_mgr().calcu_cooperate_buff(item_ptr, true);
						}
					}
					else
					{
						if (item_ptr->get_data_info(e_item_info_activate) > 0)
						{
							set_goddess_equip_att(item_ptr, true);
						}
					}
				}
			}
		}
	}

	for (int32 i = 0; i < max_wedding_equip_num; i++)
	{
		if (GET_BAG(e_bag_type_wedding_equip)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_wedding_equip)[i].server_64);
			if (item_ptr)
			{
				ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
				if (item_template_ptr)
				{
					if (item_template_ptr->item_type != e_item_type_wedding_equip)
					{
						return;
					}
					if (item_ptr->get_data_info(e_item_info_activate) > 0)
					{
						set_wedding_equip_att(item_ptr, true);
					}
				}
			}
		}
	}
	set_wing_att(true);
	set_mount_att(true);
	change_beast_spirit_att(e_beast_spirit_type_mount, true);
	change_beast_spirit_att(e_beast_spirit_type_wing, true);
	change_beast_spirit_att(e_beast_spirit_type_spirit, true);
}

void item_set::set_show_fashion_buff()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (!player_ref.is_valid())
		return;
	int32 show_fashion = player_ref.get_unit_info(faith::e_role_info_show_fashion);
	citem* normal_weapon_ptr = get_item_by_slot(e_bag_type_equip, e_role_equip_slot_weapon_1);
	if (normal_weapon_ptr == nullptr)
	{
		citem* weapon_effect_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon_effect);
		if (weapon_effect_ptr != nullptr)
		{
			weapon_effect_ptr->set_unit_buff(m_unit_array_index, false);
		}

		citem* weapon_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon);
		if (weapon_mesh_ptr != nullptr)
		{
			weapon_mesh_ptr->set_unit_buff(m_unit_array_index, false);
		}
	}
	else
	{
		citem* weapon_effect_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon_effect);
		if (weapon_effect_ptr != nullptr)
		{
			if (show_fashion == 0)
			{
				weapon_effect_ptr->set_unit_buff(m_unit_array_index, false);
				normal_weapon_ptr->set_unit_buff(m_unit_array_index, true);
			}
			else
			{
				weapon_effect_ptr->set_unit_buff(m_unit_array_index, true);
			}
		}

		citem* weapon_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon);
		if (weapon_mesh_ptr != nullptr)
		{
			if (show_fashion == 0)
			{
				weapon_mesh_ptr->set_unit_buff(m_unit_array_index, false);
				normal_weapon_ptr->set_unit_buff(m_unit_array_index, true);
			}
			else
			{
				weapon_mesh_ptr->set_unit_buff(m_unit_array_index, true);
				normal_weapon_ptr->set_unit_buff(m_unit_array_index, false);
			}
		}
		if ((weapon_effect_ptr == nullptr) && (weapon_mesh_ptr == nullptr))
		{
			normal_weapon_ptr->set_unit_buff(m_unit_array_index, true);
		}
	}

	citem* cloth_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_cloth_effect);
	if (cloth_mesh_ptr != nullptr)
	{
		if (show_fashion == 0)
		{
			cloth_mesh_ptr->set_unit_buff(m_unit_array_index, false);
		}
		else
		{
			cloth_mesh_ptr->set_unit_buff(m_unit_array_index, true);
		}
	}
	citem* foot_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_foot_effect);
	if (foot_mesh_ptr != nullptr)
	{
		if (show_fashion == 0)
		{
			foot_mesh_ptr->set_unit_buff(m_unit_array_index, false);
		}
		else
		{
			foot_mesh_ptr->set_unit_buff(m_unit_array_index, true);
		}
	}
	citem* fashion_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_cloth);
	if (fashion_mesh_ptr != nullptr)
	{
		if (show_fashion == 0)
		{
			fashion_mesh_ptr->set_unit_buff(m_unit_array_index, false);
		}
		else
		{
			fashion_mesh_ptr->set_unit_buff(m_unit_array_index, true);
		}
	}


}

void item_set::item_change_att(std::vector<f32> const& att_array, f32 add_count, bool is_add)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	temp_unit.get_pawn_att().apply_att_change_by_array(att_array, is_add, add_count);
}

int32 item_set::get_element_heart_equip_num()
{
	int32 result_num = 0;
	for (int32 item_slot = 0; item_slot < element_heart_equip_num; item_slot++)
	{
		if (GET_BAG(e_bag_type_equip_elemenet)[item_slot].is_valid())
		{
			result_num++;
		}
	}
	return result_num;
}

int32 item_set::get_total_element_heart_level()
{
	int32 total_level = 0;
	for (int32 item_slot = 0; item_slot < element_heart_equip_num; item_slot++)
	{
		citem* temp_item_ptr = get_item(GET_BAG(e_bag_type_equip_elemenet)[item_slot]);
		if (nullptr == temp_item_ptr)
		{
			continue;
		}
		total_level = total_level + temp_item_ptr->get_data_info(e_item_info_upgrade_count) + 1;
	}
	return total_level;
}

void item_set::protect_spirit_change_att(std::vector<f32> const& att_array, int32 add_count, float radio, bool is_add)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	temp_unit.get_pawn_att().apply_att_change_by_array(att_array, is_add, add_count * radio);
}
void item_set::set_equip_att(citem* equip_ptr, bool is_add)
{
	ZoneScoped;
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	auto item_template_ptr = equip_ptr->get_item_info_ptr();

	if (item_template_ptr->item_type != e_item_type_weapon && item_template_ptr->item_type != e_item_type_armor)
	{
		CONSOLE_ERROR("item_set::set_equip_att item_type:{}", item_template_ptr->item_type);
		return;
	}

	auto equip_template_ptr = equip_ptr->get_equip_info_ptr();
	if (nullptr == equip_template_ptr)
	{
		CONSOLE_ERROR("item_set::set_equip_att equip_template_ptr is nullptr item_id:{}", item_template_ptr->attribute_id);
		return;
	}
	int32 upgrade_num = equip_ptr->get_data_info(e_item_info_upgrade_count);
	int32 add_on_num = equip_ptr->get_data_info(e_item_info_add_on);
	int32 enchant_num = equip_ptr->get_data_info(e_item_info_illusion_had_byte);
	item_change_att(equip_template_ptr->AttArray, 1, is_add);

	set_equip_upgrade_att(equip_template_ptr, upgrade_num, is_add);
	set_equip_addon_att(equip_template_ptr, add_on_num, is_add);
	set_equip_enchant_att(*equip_ptr, enchant_num, is_add);

	ItemSuccinctTemplate* item_succinct_template = GET_TEMPLATE(ItemSuccinctTemplate, equip_template_ptr->SuccinctTemplateID);
	if (item_succinct_template != nullptr)
	{
		set_equip_succinct_att(item_succinct_template, *equip_ptr, is_add);
	}

	set_equip_excellent_att(equip_template_ptr, *equip_ptr, is_add);
	set_equip_jewel_att(*equip_ptr, is_add);

	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	temp_player.get_awaken_mgr().set_awaken_attr(equip_ptr, is_add);
}

void item_set::set_sky_equip_att(citem* equip_ptr, bool is_add)
{
	if (nullptr == equip_ptr)
	{
		return;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	ItemTemplate* item_template_ptr = equip_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return;
	}
	if (item_template_ptr->item_type != e_item_type_sky_equip)
	{
		return;
	}
	EquipTemplate* equip_template_ptr = equip_ptr->get_equip_info_ptr();
	if (nullptr == equip_template_ptr)
	{
		return;
	}
	item_change_att(equip_template_ptr->AttArray, 1, is_add);
}

void item_set::set_skygod_equip_att(citem* equip_ptr, bool is_add)
{
	if (nullptr == equip_ptr)
	{
		return;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	ItemTemplate* item_template_ptr = equip_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return;
	}
	if (item_template_ptr->item_type != e_item_type_skygod_equip)
	{
		return;
	}
	EquipTemplate* equip_template_ptr = equip_ptr->get_equip_info_ptr();
	if (nullptr == equip_template_ptr)
	{
		return;
	}
	item_change_att(equip_template_ptr->AttArray, 1, is_add);
}

void item_set::set_supreme_equip_att(citem* equip_ptr, bool is_add)
{
	if (nullptr == equip_ptr)
	{
		return;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	ItemTemplate* item_template_ptr = equip_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return;
	}
	if (item_template_ptr->item_type != e_item_type_supreme_equip)
	{
		return;
	}
	EquipTemplate* equip_template_ptr = equip_ptr->get_equip_info_ptr();
	if (nullptr == equip_template_ptr)
	{
		return;
	}
	item_change_att(equip_template_ptr->AttArray, 1, is_add);
}

bool item_set::can_show_sky_shape(int32 suit_type)
{
	int32 same_suit_type_num = 0;
	for (int32 j = 0; j < sky_equip_slot_num; j++)
	{
		if (GET_BAG(e_bag_type_sky_equip)[j].is_valid() == false)
		{
			return false;
		}
		citem* temp_item = get_item(GET_BAG(e_bag_type_sky_equip)[j]);
		if (temp_item == nullptr)
		{
			return false;
		}
		EquipTemplate* temp_equip_template = temp_item->get_equip_info_ptr();
		if (temp_equip_template == nullptr)
		{
			return false;
		}
		if (temp_equip_template->SuitType == suit_type)
		{
			same_suit_type_num++;
		}
	}
	if (same_suit_type_num == sky_equip_slot_num)
	{
		return true;
	}
	return false;
}

bool item_set::can_show_skygod_shape(int32 suit_type)
{
	int32 same_suit_type_num = 0;
	for (int32 j = 0; j < skygod_equip_slot_num; j++)
	{
		if (GET_BAG(e_bag_type_skygod_equip)[j].is_valid() == false)
		{
			return false;
		}
		citem* temp_item = get_item(GET_BAG(e_bag_type_skygod_equip)[j]);
		if (temp_item == nullptr)
		{
			return false;
		}
		EquipTemplate* temp_equip_template = temp_item->get_equip_info_ptr();
		if (temp_equip_template == nullptr)
		{
			return false;
		}
		if (temp_equip_template->SuitType == suit_type)
		{
			same_suit_type_num++;
		}
	}
	if (same_suit_type_num == skygod_equip_slot_num)
	{
		return true;
	}
	return false;
}

bool item_set::can_show_supreme_shape(int32 suit_type)
{
	int32 same_suit_type_num = 0;
	for (int32 j = 0; j < supreme_equip_slot_num; j++)
	{
		if (GET_BAG(e_bag_type_supreme_equip)[j].is_valid() == false)
		{
			return false;
		}
		citem* temp_item = get_item(GET_BAG(e_bag_type_supreme_equip)[j]);
		if (temp_item == nullptr)
		{
			return false;
		}
		EquipTemplate* temp_equip_template = temp_item->get_equip_info_ptr();
		if (temp_equip_template == nullptr)
		{
			return false;
		}
		if (temp_equip_template->SuitType == suit_type)
		{
			same_suit_type_num++;
		}
	}
	if (same_suit_type_num == supreme_equip_slot_num)
	{
		return true;
	}
	return false;
}

void item_set::set_equip_upgrade_att(const EquipTemplate* equip_template_ptr, const int32& upgrade_num, bool is_add)
{
	if (equip_template_ptr == nullptr || upgrade_num <= 0)
	{
		return;
	}
	item_change_att(init_unit::get_equip_upgrade_att_array(equip_template_ptr->attribute_id, upgrade_num), 1, is_add);
}
void item_set::set_equip_addon_att(const EquipTemplate* equip_template_ptr, const int32& addon_num, bool is_add)
{
	if (equip_template_ptr == nullptr
		|| addon_num <= 0)
	{
		return;
	}

	item_change_att(equip_template_ptr->EquipAddonAtt, addon_num, is_add);
}
void item_set::set_equip_succinct_att(const ItemSuccinctTemplate* succinct_template_ptr, const citem& equip_ptr, bool is_add)
{
	if (succinct_template_ptr == nullptr)
	{
		return;
	}

	std::vector<f32> succinct_property_value;
	int32 property_size = succinct_template_ptr->PropertyID.size() > item_temp_succinct_property_max ? item_temp_succinct_property_max : succinct_template_ptr->PropertyID.size();
	succinct_property_value.reserve(property_size * e_att_one_max);
	for (int32 i = 0; i < property_size; i++)
	{
		int32 real_item_info_index = e_item_info_succinct_property1 + i;
		if (real_item_info_index > e_item_info_succinct_property6)
		{
			break;
		}

		succinct_property_value.push_back(4.0);											//level
		succinct_property_value.push_back(succinct_template_ptr->PropertyID[i]);		//id
		succinct_property_value.push_back(equip_ptr.get_data_info(e_item_info(real_item_info_index)));				//value
		succinct_property_value.push_back(0.0);											//percentag
		succinct_property_value.push_back(1.0);											//is_show
	}
	item_change_att(succinct_property_value, 1, is_add);
}
void item_set::set_equip_excellent_att(const EquipTemplate* equip_template_ptr, const citem& equip_ptr, bool is_add)
{
	if (equip_template_ptr == nullptr)
	{
		return;
	}
	//item_change_att(init_unit::get_excellent_att_array(equip_template_ptr->BaseExcellentAtt, equip_ptr.get_item_inst()), 1, is_add);
}
void item_set::set_equip_jewel_att(citem& equip_ptr, bool is_add)
{
	for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
	{
		int32 jewel_id = equip_ptr.get_data_info((e_item_info)i);
		if (jewel_id <= 0)
		{
			continue;
		}
		ItemTemplate* jewel_item_ptr = GET_TEMPLATE(ItemTemplate, jewel_id);
		if (jewel_item_ptr == nullptr)
		{
			continue;
		}
		JewelTemplate* jewel_ptr = GET_TEMPLATE(JewelTemplate, jewel_item_ptr->logic_id);
		if (jewel_ptr == nullptr)
		{
			continue;
		}
		item_change_att(jewel_ptr->JewelAtt, 1, is_add);
	}

	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	temp_player.get_jewel_carve_mgr().set_skill(&equip_ptr, is_add);


}

void item_set::set_equip_enchant_att(citem& equip_ptr, const int32& enchant_num, bool is_add)
{
	ItemTemplate* item_ptr = equip_ptr.get_item_info_ptr();
	if (item_ptr == nullptr)
	{
		return;
	}
	int32 enchant_level = enchant_num / faith::max_enchant_type_num;
	int32 enchant_type = enchant_num % faith::max_enchant_type_num;
	int32 enchant_slot = equip_ptr.get_data_info(e_item_info_slot) - 1;
	EquipEnchantTemplate* enchant_template_ptr = template_manager::get_instance().get_equip_enchant_template_ptr(enchant_level, enchant_type, enchant_slot);
	if (enchant_template_ptr == nullptr)
	{
		return;
	}
	if (enchant_template_ptr->ItemGet.size() % 2 != 0)
	{
		return;
	}

	item_change_att(enchant_template_ptr->AttArray, 1, is_add);

}

void item_set::set_goddess_equip_att(citem* equip_ptr, bool is_add)
{
	if (nullptr == equip_ptr)
	{
		return;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}
	ItemTemplate* item_template_ptr = equip_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return;
	}
	if (item_template_ptr->item_type != e_item_type_goddess_equip)
	{
		return;
	}
	DeityTemplate* deity_template_ptr = equip_ptr->get_deity_info_ptr();
	if (nullptr == deity_template_ptr)
	{
		return;
	}
	item_change_att(deity_template_ptr->AttArray, 1, is_add);
	if (item_template_ptr->sub_type != 0)
	{
		int32 array_index = m_unit_array_index;
		int32 npc_array_index = -1;
		bool Is_goddess_npc_exist = false;
		citem* goddess_item = player_ref.get_goddess_mgr().get_select_type_goddess(deity_template_ptr->Type);
		if (nullptr == goddess_item)
		{
			return;
		}
		if (goddess_item->get_data_info(e_item_info_is_first) == 0)
		{
			return;
		}
		DeityTemplate* template_ptr = goddess_item->get_deity_info_ptr();
		if (nullptr == template_ptr)
		{
			return;
		}
		npc* goddess_npc = player_ref.get_pet_ptr_by_type(e_summon_pet_type_goddess);
		if (goddess_npc)
		{
			if (template_ptr->NpcId == goddess_npc->get_npc_template_id())
			{
				npc_array_index = goddess_npc->get_array_index();
				Is_goddess_npc_exist = true;
			}
		}
		if (item_template_ptr->sub_type == 1 || item_template_ptr->sub_type == 2 || item_template_ptr->sub_type == 6)
		{
			if (Is_goddess_npc_exist)
			{
				array_index = goddess_npc->get_array_index();
			}
			else
			{
				return;
			}
		}
		if (is_add)
		{
			for (int32 i = 0; i < deity_template_ptr->AdditionBuff.size(); i++)
			{
				buff_man::add_buff_inst(array_index, array_index, deity_template_ptr->AdditionBuff[i]);
			}
			if (Is_goddess_npc_exist && template_ptr->AdditionBuff.size() >= per_goddess_type_equip_num)
			{
				buff_man::add_buff_inst(npc_array_index, npc_array_index, template_ptr->AdditionBuff[item_template_ptr->sub_type - 1]);
			}
		}
		else
		{
			for (int32 i = 0; i < deity_template_ptr->AdditionBuff.size(); i++)
			{
				buff_man::del_buff_by_template_id(array_index, array_index, deity_template_ptr->AdditionBuff[i]);
			}
			if (Is_goddess_npc_exist && template_ptr->AdditionBuff.size() >= per_goddess_type_equip_num)
			{
				buff_man::del_buff_by_template_id(npc_array_index, npc_array_index, template_ptr->AdditionBuff[item_template_ptr->sub_type - 1]);
			}
		}
	}
	else
	{
		if (equip_ptr->get_data_info(e_item_info_succinct_level) == 0 || equip_ptr->get_data_info(e_item_info_is_first) != 1)
		{
			return;
		}
		DeityTemplate* star_deity_template_ptr = GET_TEMPLATE(DeityTemplate, deity_template_ptr->StarID + equip_ptr->get_data_info(e_item_info_stargrade_count));
		if (star_deity_template_ptr == nullptr)
		{
			return;
		}
		item_change_att(star_deity_template_ptr->AttArray, 1, is_add);


		int32 npc_array_index = -1;

		npc* goddess_npc = player_ref.get_pet_ptr_by_type(e_summon_pet_type_goddess);
		if (goddess_npc)
		{
			if (deity_template_ptr->NpcId == goddess_npc->get_npc_template_id())
			{
				npc_array_index = goddess_npc->get_array_index();
			}
		}
		if (npc_array_index == -1)
		{
			return;
		}
		if (is_add)
		{
			buff_man::add_buff_inst(npc_array_index, npc_array_index, star_deity_template_ptr->ShowBuff);

		}
		else
		{
			buff_man::del_buff_by_template_id(npc_array_index, npc_array_index, star_deity_template_ptr->ShowBuff);

		}

	}
}

int32 item_set::get_goddess_equip_slot(citem* equip_ptr)
{
	if (nullptr == equip_ptr)
	{
		return -1;
	}
	for (int32 i = 0; i < max_goddess_equip_num; i++)
	{
		citem* temp_item = get_item(GET_BAG(e_bag_type_goddess_equip)[i]);
		if (nullptr == temp_item)
		{
			continue;
		}
		if (temp_item->get_data_info(e_item_info_info_id) == equip_ptr->get_data_info(e_item_info_info_id))
		{
			return i;
		}
	}
	return -1;
}

void item_set::set_fashion_att(citem* fashion_ptr, bool is_add, int32 star_num)
{
	if (nullptr == fashion_ptr)
	{
		return;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	ItemTemplate* item_template_ptr = fashion_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return;
	}
	if (item_template_ptr->item_type != e_item_type_fashion)
	{
		return;
	}

	FasionTemplate* fashion_template_ptr = GET_TEMPLATE(FasionTemplate, item_template_ptr->logic_id + star_num);
	if (nullptr == fashion_template_ptr)
	{
		return;
	}
	item_change_att(fashion_template_ptr->AttArray, 1, is_add);
}

void item_set::set_wing_att(bool is_add)
{
	if (m_wing_att_change == is_add)
	{
		return;
	}
	m_wing_att_change = is_add;
	citem* wing_ptr = get_cur_level_wing();
	if (wing_ptr)
	{
		auto item_template_ptr = wing_ptr->get_item_info_ptr();
		if (item_template_ptr)
		{
			int32 real_id = item_template_ptr->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
			//MountTemplate* mount_att_ptr = GET_TEMPLATE(MountTemplate, real_id);
			WingTemplate* wing_att_ptr = GET_TEMPLATE(WingTemplate, real_id);
			if (wing_att_ptr)
			{
				item_change_att(wing_att_ptr->AttArray, 1, is_add);
			}
		}
	}

	std::vector<citem*> shape_wing_block = get_all_shape_wing();
	int32 array_size = shape_wing_block.size();
	for (int32 shape_wing_index = 0; shape_wing_index < array_size; shape_wing_index++)
	{
		citem* wing_item_ptr = shape_wing_block[shape_wing_index];
		if (wing_item_ptr != nullptr)
		{
			if (0 == wing_item_ptr->get_data_info(e_item_info_activate))
			{
				continue;
			}
			auto item_template_ptr = wing_item_ptr->get_item_info_ptr();
			int32 real_id = wing_item_ptr->get_item_info_ptr()->logic_id + wing_item_ptr->get_data_info(e_item_info_upgrade_count);
			int32 real_star_id = wing_item_ptr->get_item_info_ptr()->logic_id + wing_item_ptr->get_data_info(e_item_info_stargrade_count) + 1000;
			int32 real_zhuling_id = wing_item_ptr->get_item_info_ptr()->logic_id + wing_item_ptr->get_data_info(e_item_info_forge_property1) + 2000;

			WingTemplate* shape_wing_ptr = GET_TEMPLATE(WingTemplate, real_id);
			WingTemplate* shape_wing_star_ptr = GET_TEMPLATE(WingTemplate, real_star_id);
			WingTemplate* shape_wing_zhuling_ptr = GET_TEMPLATE(WingTemplate, real_zhuling_id);
			if (shape_wing_ptr)
			{
				item_change_att(shape_wing_ptr->AttArray, 1, is_add);
			}
			if (item_template_ptr)
			{
				if (item_template_ptr->sub_type == 1)
				{
					if (shape_wing_star_ptr)
					{
						item_change_att(shape_wing_star_ptr->AttArray, 1, is_add);
					}
					if (shape_wing_zhuling_ptr)
					{
						item_change_att(shape_wing_zhuling_ptr->AttArray, 1, is_add);
						if (wing_item_ptr->get_data_info(e_item_info_forge_property1) >= 101)
						{
							item_change_att(shape_wing_zhuling_ptr->ExcellentAttArray, 1, is_add);
						}
					}
				}
			}
		}
	}


	citem* spirit_ptr = get_wing_add_spirit();
	if (spirit_ptr != nullptr)
	{
		ItemTemplate* item_template_ptr = spirit_ptr->get_item_info_ptr();
		if (item_template_ptr != nullptr)
		{
			int32 real_id = item_template_ptr->logic_id + spirit_ptr->get_data_info(e_item_info_upgrade_count);
			WingAddSpiritTemplate* add_spirit_att_ptr = GET_TEMPLATE(WingAddSpiritTemplate, real_id);
			if (add_spirit_att_ptr != nullptr)
			{
				item_change_att(add_spirit_att_ptr->AttArray, 1, is_add);
			}
		}
	}

	citem* soul_ptr = get_wing_add_soul();
	if (soul_ptr != nullptr)
	{
		ItemTemplate* item_template_ptr = soul_ptr->get_item_info_ptr();
		if (item_template_ptr)
		{
			int32 real_id = item_template_ptr->logic_id + soul_ptr->get_data_info(e_item_info_upgrade_count);
			WingAddSoulTemplate* add_soul_att_ptr = GET_TEMPLATE(WingAddSoulTemplate, real_id);
			if (add_soul_att_ptr != nullptr)
			{
				item_change_att(add_soul_att_ptr->AttArray, 1, is_add);
			}
		}
	}

	std::vector<citem*> temp_feather_array = get_wing_feather();
	for (int32 i = 0; i < temp_feather_array.size(); ++i)
	{
		citem* temp_wing_feather = temp_feather_array[i];
		if (temp_wing_feather != nullptr && temp_wing_feather->get_data_info(e_item_info_activate) > 0)
		{
			set_feather_att(*temp_wing_feather, is_add);
		}
	}
}

void item_set::set_feather_att(const citem& feather_ptr, bool is_add)
{
	auto feather_template_ptr = feather_ptr.get_item_info_ptr();
	if (nullptr == feather_template_ptr)
	{
		return;
	}
	if (feather_template_ptr->item_type != e_item_type_wing)
	{
		return;
	}
	if (feather_template_ptr->sub_type != e_wing_sub_type_feather)
	{
		return;
	}

	int32 real_id = feather_template_ptr->logic_id + feather_ptr.get_data_info(e_item_info_upgrade_count);
	FeatherTemplate* feather_att_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
	if (nullptr == feather_att_ptr)
	{
		return;
	}
	item_change_att(feather_att_ptr->AttArray, 1, is_add);
	item_change_att(feather_att_ptr->PurenessBasicAtt, 1, is_add);
}

citem* item_set::get_equip_item_by_slot(e_role_equip_slot item_slot)
{
	if (item_slot <= 0 || item_slot >= e_role_equip_slot_max)
	{
		return nullptr;
	}

	if (item_slot == e_role_equip_slot_mount)
	{
		return get_showed_mount();
	}
	else if (item_slot == e_role_equip_slot_wing)
	{
		return get_showed_wing();
	}
	return get_item(GET_BAG(e_bag_type_equip)[item_slot]);
}

citem* item_set::get_equip_sprite_by_slot(int32 equip_slot)
{
	auto& guid_array = GET_BAG(e_bag_type_equip_spirit);
	for (auto& it : guid_array)
	{
		auto item = get_item(it.second);
		if (item && item->get_data_info(e_item_info_equip_slot) == equip_slot)
		{
			return item;
		}
	}
	return nullptr;
}
citem* item_set::get_item_by_slot(int32 bag_type, int32 item_slot)
{
	if (bag_type <= e_bag_type_none || bag_type >= e_bag_type_max)
	{
		return nullptr;
	}
	auto& guid_array = GET_BAG(bag_type);
	auto it = guid_array.find(item_slot);
	if (it != guid_array.end())
	{
		return get_item(it->second);
	}
	return nullptr;
}

int32 item_set::get_lingyu_total_grade()
{
	int32 return_value = 0;
	citem* wing_ptr = get_cur_level_wing();
	if (wing_ptr != nullptr)
	{
		std::vector<citem*> temp_feather_array = get_wing_feather();
		for (int32 i = 0; i < temp_feather_array.size(); ++i)
		{
			citem* temp_wing_feather = temp_feather_array[i];
			if (temp_wing_feather != nullptr)
			{
				ItemTemplate* item_template_ptr = temp_wing_feather->get_item_info_ptr();
				if (item_template_ptr != nullptr)
				{
					int32 real_id = item_template_ptr->logic_id + temp_wing_feather->get_data_info(e_item_info_upgrade_count);
					FeatherTemplate* feather_template_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
					if (feather_template_ptr != nullptr)
					{
						return_value += feather_template_ptr->UpgradeNow;
					}
				}
			}
		}
	}
	return return_value;
}

int32 item_set::star_skill_unlock(guid_64 choosed_guid, int32 choosed_index, bool use_item, bool is_star)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	int32 result = 0;
	citem* temp_item = get_item(choosed_guid);
	if (temp_item != nullptr)
	{
		result = item_star_skill_unlock(choosed_guid, choosed_index, use_item, is_star);
	}
	return result;
}

int32 item_set::item_star_skill_unlock(guid_64 choosed_guid, int32 choosed_index, bool use_item, bool is_star)
{
	int32 result = 0;
	int32 matirial_count = 0;
	int32 passive_skill_template_id = 0;
	int32 unlock_skill_grade = 0;
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* temp_item = get_item(choosed_guid);
	if (temp_item == nullptr)
	{
		return e_item_string_unkown;
	}
	ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
	if (item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 logic_id = item_template_ptr->logic_id;
	int32 item_type = item_template_ptr->item_type;
	int32 item_sub_type = item_template_ptr->sub_type;
	int32 cur_grade = 0;
	bool is_normal_upgrade = false;
	switch (item_type)
	{
	case e_item_type_mount:
	{
		MountTemplate* mount_template_ptr = nullptr;
		if (is_star)
		{
			mount_template_ptr = GET_TEMPLATE(MountTemplate, logic_id + 1000 + choosed_index);
			if (mount_template_ptr == nullptr)
			{
				return e_item_string_unkown;
			}
			unlock_skill_grade = mount_template_ptr->StarGradeNow;
			cur_grade = temp_item->get_data_info(e_item_info_stargrade_count);
		}
		else
		{
			mount_template_ptr = GET_TEMPLATE(MountTemplate, logic_id + choosed_index);
			if (mount_template_ptr == nullptr)
			{
				return e_item_string_unkown;
			}
			if (mount_template_ptr->Type == mount_type_illusion)
			{
				unlock_skill_grade = mount_template_ptr->GradeNow;
			}
			else
			{
				unlock_skill_grade = mount_template_ptr->ClassNow * 11 + mount_template_ptr->GradeNow;
				is_normal_upgrade = true;
			}
			cur_grade = temp_item->get_data_info(e_item_info_upgrade_count);
		}
		passive_skill_template_id = mount_template_ptr->StarBuffId;
	}
	break;
	case e_item_type_wing:
	{
		if (item_sub_type == 2)
		{
			FeatherTemplate* feather_template_ptr = GET_TEMPLATE(FeatherTemplate, logic_id + temp_item->get_data_info(e_item_info_upgrade_count));
			if (nullptr == feather_template_ptr)
			{
				return e_item_string_unkown;
			}
			unlock_skill_grade = (feather_template_ptr->UpgradeNow - 1) * 11 + feather_template_ptr->AddStarNow;
			is_normal_upgrade = true;
			cur_grade = temp_item->get_data_info(e_item_info_upgrade_count);
			passive_skill_template_id = feather_template_ptr->SkillID;
		}
		else
		{
			WingTemplate* wing_template_ptr = nullptr;
			if (is_star)
			{
				wing_template_ptr = GET_TEMPLATE(WingTemplate, logic_id + 1000 + choosed_index);
				if (wing_template_ptr == nullptr)
				{
					return e_item_string_unkown;
				}
				unlock_skill_grade = wing_template_ptr->StarGradeNow;
				cur_grade = temp_item->get_data_info(e_item_info_stargrade_count);
			}
			else
			{
				wing_template_ptr = GET_TEMPLATE(WingTemplate, logic_id + choosed_index);
				if (wing_template_ptr == nullptr)
				{
					return e_item_string_unkown;
				}
				if (wing_template_ptr->Type == e_item_wing_type_illusion)
				{
					unlock_skill_grade = wing_template_ptr->UpgradeNow;
				}
				else
				{
					unlock_skill_grade = (wing_template_ptr->UpgradeNow - 1) * 11 + wing_template_ptr->StarNow;
					is_normal_upgrade = true;
				}

				cur_grade = temp_item->get_data_info(e_item_info_upgrade_count);
			}
			passive_skill_template_id = wing_template_ptr->StarBuffId;
		}
	}
	break;
	case e_item_type_spirit:
	{
		return 0;
		//return player_ref.get_spirit_mgr().spirit_star_skill_unlock(choosed_guid, choosed_index, use_item);
	}
	break;
	default:
		break;
	}
	PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, passive_skill_template_id);
	if (passive_skill_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 unlock_flag = temp_item->get_data_info(e_item_info_starskill_flag);
	if (cur_grade >= unlock_skill_grade)
	{
		if (passive_skill_template_ptr->OpenItem.size() >= 2 && use_item)
		{
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, passive_skill_template_ptr->OpenItem[0], passive_skill_template_ptr->OpenItem[1]) == false)
			{
				return e_item_string_matirial_not_enough;
			}
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, passive_skill_template_ptr->OpenItem[0], passive_skill_template_ptr->OpenItem[1]);
		}
		int32 open_time = 0;
		if (is_normal_upgrade)
		{
			temp_item->set_data_info(e_item_info_succinct_property2, passive_skill_template_id);
			if (temp_item->get_data_info(e_item_info_succinct_property5) <= 0)
			{
				open_time = time_helper::get_time() - time_helper::get_today_time_in_sec();
				temp_item->set_data_info(e_item_info_succinct_property5, open_time);
			}
			open_time = temp_item->get_data_info(e_item_info_succinct_property5);
		}
		else
		{
			temp_item->set_data_info(e_item_info_succinct_property1, passive_skill_template_id);
			if (temp_item->get_data_info(e_item_info_succinct_property4) <= 0)
			{
				open_time = time_helper::get_time() - time_helper::get_today_time_in_sec();
				temp_item->set_data_info(e_item_info_succinct_property4, open_time);
			}

			open_time = temp_item->get_data_info(e_item_info_succinct_property4);
		}
		player_ref.get_passive_skill().passive_skill_special(passive_skill_template_id, true);

		result = 1;
		//unlock_flag = (1 << choosed_index) | unlock_flag;
		//temp_item->set_data_info(e_item_info_starskill_flag, unlock_flag);
		send_item_one(temp_item);
	}
	return result;
}

int32 item_set::wing_psychic_skill_unlock(int32 passive_skill_template_id, bool is_add)
{
	int32 result = 0;
	int32 open_time = time_helper::get_time() - time_helper::get_today_time_in_sec();
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return result;
	}
	player_ref.get_passive_skill().passive_skill_special(passive_skill_template_id, is_add);

	result = 1;
	return result;
}

int32 item_set::get_mark_activated_num()
{
	int32 activated_num = 0;
	for (int32 i = 0; i < max_proctect_mark_bag_slot_num; ++i)
	{
		if (GET_BAG(e_bag_type_protect)[i].is_valid())
		{
			citem* temp_ptr = get_item(GET_BAG(e_bag_type_protect)[i]);
			if (temp_ptr != nullptr && temp_ptr->get_data_info(e_item_info_activate) == 1)
			{
				activated_num++;
			}
		}
	}
	for (int32 i = 0; i < max_proctect_mark_equip_slot_num; ++i)
	{
		if (GET_BAG(e_bag_type_equip_protect)[i].is_valid())
		{
			citem* temp_ptr = get_item(GET_BAG(e_bag_type_equip_protect)[i]);
			if (temp_ptr != nullptr && temp_ptr->get_data_info(e_item_info_activate) == 1)
			{
				activated_num++;
			}
		}
	}
	return activated_num;
}

int32 item_set::get_double_att_element_num()
{
	int32 total_num = 0;
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return total_num;
	}
	for (int32 slot = 0; slot < element_heart_equip_num; slot++)
	{
		citem* temp_element_heart_ptr = get_item_by_slot(e_bag_type_equip_elemenet, slot);
		if (temp_element_heart_ptr != nullptr)
		{
			//ElementHeartTemplate* element_heart_temp_ptr = player_ref.get_element_heart_mgr().get_element_heart_template(*temp_element_heart_ptr);
			//{
			//	total_num++;
			//}
		}
	}
	return total_num;
}

void item_set::init_break_will_skill_att()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}

	std::map<int32, int32> lock_item_list;
	lock_item_list.clear();
	for (int32 i = 0; i < break_will_max; ++i)
	{
		if (GET_BAG(e_bag_type_break_will)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_break_will)[i]);
			if (temp_item && temp_item->get_data_info(e_item_info_activate) > 0)
			{
				ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
				if (item_template_ptr == nullptr)
				{
					continue;
				}
				BreakWillTemplate* break_will_ptr = GET_TEMPLATE(BreakWillTemplate, item_template_ptr->logic_id);
				if (break_will_ptr == nullptr)
				{
					continue;
				}
				lock_item_list[item_template_ptr->logic_id] = 1;
				item_change_att(break_will_ptr->AttArray, 1, true);
			}
		}
	}
	std::vector<BreakWillTemplate*> temp_template_array = template_manager::get_instance().get_break_will_template_ptr_type_array();
	for (int32 i = 0; i < temp_template_array.size(); ++i)
	{
		BreakWillTemplate* break_will_ptr = temp_template_array[i];
		if (break_will_ptr == nullptr)
		{
			continue;
		}
		if (break_will_ptr->Type < 0 || break_will_ptr->SkillId <= 0 || break_will_ptr->SubList.size() <= 0)
		{
			continue;
		}
		bool is_all_have = true;
		for (int32 j = 0; j < break_will_ptr->SubList.size(); ++j)
		{
			auto entry = lock_item_list.find(break_will_ptr->SubList[j]);
			if (entry == lock_item_list.end())
			{
				is_all_have = false;
				break;
			}
		}
		if (is_all_have)
		{
			player_ref.get_passive_skill().passive_skill_special(break_will_ptr->SkillId, true);
		}
	}
	player_ref.get_mission_mgr().target_check(e_mission_end_type_active_break_will, 0, 0);
}

void item_set::refresh_break_will_skill_by_id(int32 tem_id)
{
	BreakWillTemplate* break_will_ptr = GET_TEMPLATE(BreakWillTemplate, tem_id);
	if (break_will_ptr == nullptr)
	{
		return;
	}

	if (break_will_ptr->Type < 0 || break_will_ptr->SkillId <= 0 || break_will_ptr->SubList.size() <= 0)
	{
		return;
	}

	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}

	bool is_all_have = true;
	for (int32 j = 0; j < break_will_ptr->SubList.size(); ++j)
	{
		bool is_have = false;
		for (int32 i = 0; i < break_will_max; ++i)
		{
			if (GET_BAG(e_bag_type_break_will)[i].is_valid())
			{
				citem* temp_item = get_item(GET_BAG(e_bag_type_break_will)[i]);
				if (temp_item && temp_item->get_data_info(e_item_info_activate) > 0)
				{
					ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
					if (item_template_ptr == nullptr)
					{
						continue;
					}
					if (item_template_ptr->logic_id == break_will_ptr->SubList[j])
					{
						is_have = true;
						break;
					}
				}
			}
		}
		if (is_have == false)
		{
			is_all_have = false;
			break;
		}
	}
	if (is_all_have)
	{
		player_ref.get_passive_skill().passive_skill_special(break_will_ptr->SkillId, true);
	}
}

int32 item_set::get_break_will_level()
{
	int32 all_level = 0;
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return all_level;
	}

	for (int32 i = 0; i < break_will_max; ++i)
	{
		if (GET_BAG(e_bag_type_break_will)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_break_will)[i]);
			if (temp_item && temp_item->get_data_info(e_item_info_activate) > 0)
			{
				all_level++;
			}
		}
	}
	return all_level;
}

void item_set::init_awaken_fetter_att()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}

	for (int32 i = 0; i < awaken_fetter_bag_max; ++i)
	{
		if (GET_BAG(e_bag_type_awaken_item)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_awaken_item)[i]);
			if (temp_item && temp_item->get_data_info(e_item_info_upgrade_count) > 0)
			{
				ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
				if (item_template_ptr == nullptr)
				{
					continue;
				}
				for (int32 j = 0; j < temp_item->get_data_info(e_item_info_upgrade_count); ++j)
				{
					AwakenFetterTemplate* template_ptr = GET_TEMPLATE(AwakenFetterTemplate, item_template_ptr->logic_id + j);
					if (template_ptr == nullptr)
					{
						continue;
					}
					item_change_att(template_ptr->AttArray, 1, true);
					int32 skill_level = temp_item->get_data_info((e_item_info)(e_item_info_random_property1 + template_ptr->SubIndex));
					if (template_ptr->Type == 1 && template_ptr->UnLockSkillList.size() > skill_level - 1 && skill_level > 0)
					{
						AwakenFetterTemplate* skill_template_ptr = GET_TEMPLATE(AwakenFetterTemplate, template_ptr->UnLockSkillList[skill_level - 1]);
						if (skill_template_ptr == nullptr || skill_template_ptr->UnLockSkillList.size() <= 0)
						{
							continue;
						}
						player_ref.get_passive_skill().passive_skill_special(skill_template_ptr->UnLockSkillList[0], true);
					}
				}
			}
		}
	}
}

citem* item_set::get_item(const guid_64& item_guid)
{
	if (item_guid.is_valid() == false)
	{
		return nullptr;
	}
	auto entry = m_item_map.find(item_guid.server_64);
	if (entry != m_item_map.end())
	{
		return entry->second;
	}

	return nullptr;
}

citem* item_set::get_item(const int32& item_guid_A, const int32& item_guid_B)
{
	guid_64 item_guid(item_guid_A, item_guid_B);
	return get_item(item_guid);
}
citem* item_set::get_item(std::string item_guid_str)
{
	guid_64 unit_id;
	unit_id.parse_guid(item_guid_str);
	return get_item(unit_id);
}
std::vector<citem*> item_set::get_item_array(e_bag_type bag_type)
{
	std::vector<citem*> item_array;
	auto& guid_array = GET_BAG(bag_type);
	for (auto& it : guid_array)
	{
		auto item = get_item(it.second);
		if (item)
		{
			item_array.push_back(item);
		}
	}
	return item_array;
}

citem* item_set::create_item_by_template(e_server_log_add_item add_type, int32 param, int32 item_template_id, int32 item_num, int32 locked, int32 item_over_time, e_bag_type bag_type, int32 item_const_att)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (!player_ref.is_valid())
	{
		CONSOLE_ERROR("player is not valid");
		return nullptr;
	}
	s_item_info temp_info;
	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
	if (item_template_ptr == nullptr)
	{
		CONSOLE_ERROR("item_template_ptr is nullptr item_template_id:{}", item_template_id);
		return nullptr;
	}

	if (item_template_ptr->item_type == e_item_type_expendable && item_template_ptr->sub_type == e_prop_sub_type_const_att_equip)
	{
		PropBasicAttributeTemplate* prop_basic_temp_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, item_template_ptr->logic_id);
		if (nullptr == prop_basic_temp_ptr)
		{
			CONSOLE_ERROR("prop_basic_temp_ptr is nullptr item_template_id:{}", item_template_id);
			return nullptr;
		}
		int32 drop_temp_id = prop_basic_temp_ptr->ItemGet;
		int32 cur_class = player_ref.get_unit_info(e_role_info_class_type);
		std::vector<s_item_template_info> item_drop_list_with_att;
		cdrop::gen_drop_list_by_drop_id(drop_temp_id, item_drop_list_with_att, player_ref.get_unit_info(e_role_info_class_type), player_ref.get_unit_info(e_role_info_exp_level));

		std::vector<s_item_template_info> items_with_num_list;
		cdrop::merge_drop_bag(item_drop_list_with_att, items_with_num_list, cur_class);
		if (items_with_num_list.size() <= 0)
		{
			CONSOLE_ERROR("items_with_num_list is empty item_template_id:{}", item_template_id);
			return nullptr;
		}
		item_template_id = items_with_num_list[0].m_item_id;
		item_num = items_with_num_list[0].m_item_num;
		item_const_att = 0;
		item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
		if (nullptr == item_template_ptr)
		{
			CONSOLE_ERROR("item_template_ptr is nullptr item_template_id:{}", item_template_id);
			return nullptr;
		}
	}
	if (item_template_ptr->item_type == e_item_type_time_limit)
	{
		item_template_id = item_template_ptr->logic_id;
		item_over_time = time_helper::get_cur_time_new().second + item_template_ptr->EffectiveTime;
		locked = 1;
		temp_info.data_ary[e_item_info_prototype_id] = item_template_id;
		item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_ptr->logic_id);
		if (nullptr == item_template_ptr)
		{
			CONSOLE_ERROR("item_template_ptr is nullptr item_template_id:{}", item_template_id);
			return nullptr;
		}
	}

	if (item_template_ptr->EffectiveTime > 0)
	{
		item_over_time = time_helper::get_cur_time_new().second + item_template_ptr->EffectiveTime;
	}

	if (item_template_ptr->item_type == e_item_type_expendable && item_template_ptr->sub_type == e_prop_sub_type_wing_experience)
	{
		int32 class_type = player_ref.get_unit_info(e_role_info_class_type);
		int32 item_wing_template_id = 0;
		switch (class_type)
		{
		case e_class_type_barserker:
			item_wing_template_id = 31040001;
			break;
		case e_class_type_wizard:
			item_wing_template_id = 31040011;
			break;
		case e_class_type_guardian:
			item_wing_template_id = 31040021;
			break;
		case e_class_type_assassinator:
			item_wing_template_id = 31040031;
			break;
		}
		ItemTemplate* item_ptr_lock = GET_TEMPLATE(ItemTemplate, item_wing_template_id);
		if (nullptr != item_ptr_lock)
		{
			bool is_unlock = player_ref.get_func_unlock_mgr().is_func_unlock(item_ptr_lock->FuncUnlockid);
			if (true == is_unlock)
			{
				CONSOLE_ERROR("item is unlock item_template_id:{}", item_template_id);
				return nullptr;
			}
		}
		if (nullptr != get_showed_wing())
		{
			CONSOLE_ERROR("item is showed wing item_template_id:{}", item_template_id);
			return nullptr;
		}
		PropBasicAttributeTemplate* prop_basic_temp_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, item_template_ptr->logic_id);
		if (nullptr == prop_basic_temp_ptr)
		{
			CONSOLE_ERROR("prop_basic_temp_ptr is nullptr item_template_id:{}", item_template_id);
			return nullptr;
		}
		int32 cur_time = time_helper::get_cur_time_new().second;
		int32 item_time = prop_basic_temp_ptr->EffectiveTime;
		int32 over_time = item_time + cur_time;
		item_template_id = prop_basic_temp_ptr->ItemGet;
		int32 wing_item_slot = get_empty_slot(e_bag_type_wing);
		if (wing_item_slot < 0)
		{
			CONSOLE_ERROR("wing_item_slot is invalid item_template_id:{}", item_template_id);
			return nullptr;
		}
		citem* temp = get_item_by_template_id(item_template_id);
		if (nullptr == temp)
		{
			//if (init_unit::init_item_data(temp_info, item_template_id, wing_item_slot, item_num, e_bag_type_wing, 0, locked))
			//{
			//	citem* temp = create_item_by_info(temp_info, add_type, param, true);
			//	if (nullptr == temp)
			//	{
			//		return temp;
			//	}
			//}
			CONSOLE_ERROR("temp is nullptr item_template_id:{}", item_template_id);
			return temp;
		}
		else
		{
			int32 empty_slot = get_use_time_item_empty_slot();
			if (empty_slot < 0 || empty_slot >= MAX_MOUNT_NUM)
			{
				CONSOLE_ERROR("empty_slot is invalid item_template_id:{}", item_template_id);
				return nullptr;
			}
			m_used_has_time_item_array[empty_slot] = temp->get_item_guid();
			temp->set_data_info(e_item_info_activate, 1);
			temp->set_data_info(e_item_info_over_time, over_time);
			send_item_one(temp);

			WingTemplate* wing_template_ptr = temp->get_wing_template_ptr();
			if (wing_template_ptr != nullptr)
			{
				item_change_att(wing_template_ptr->AttArray, 1, true);
				show_this_wing(temp, -1, true);
			}
			if (item_template_ptr->buff_trigger_id.size() > 0)
			{
				int32 buff_id = item_template_ptr->buff_trigger_id[0];
				buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, buff_id);
			}
		}

	}

	else if (item_template_ptr->item_type == e_item_type_wing)
	{
		if (item_template_ptr->sub_type == e_wing_sub_type_feather)
		{
			if (get_item_by_template_id(item_template_id) != nullptr)
			{
				CONSOLE_ERROR("item is not nullptr item_template_id:{}", item_template_id);
				return nullptr;
			}

		}
		int32 wing_item_slot = get_empty_slot(e_bag_type_wing);
		if (wing_item_slot < 0)
		{
			CONSOLE_ERROR("wing_item_slot is invalid item_template_id:{}", item_template_id);
			return nullptr;
		}
		else if (init_unit::init_item_data(temp_info, item_template_id, wing_item_slot, item_num, e_bag_type_wing, 0, locked, item_over_time))
		{
			citem* temp = create_item_by_info(temp_info, add_type, param, true);
			return temp;
		}
	}
	else if (item_template_ptr->item_type == e_item_type_mount)
	{
		int32 mount_item_slot = get_empty_slot(e_bag_type_mount);
		if (mount_item_slot < 0)
		{
			CONSOLE_ERROR("mount_item_slot is invalid item_template_id:{}", item_template_id);
			return nullptr;
		}
		else if (init_unit::init_item_data(temp_info, item_template_id, mount_item_slot, item_num, e_bag_type_mount, 0, locked, item_over_time))
		{
			citem* temp = create_item_by_info(temp_info, add_type, param, true);
			return temp;
		}
	}
	else if (item_template_ptr->item_type == e_item_type_protect)
	{
		int32 protect_item_slot = get_empty_slot(e_bag_type_protect);
		if (protect_item_slot < 0)
		{
			CONSOLE_ERROR("protect_item_slot is invalid item_template_id:{}", item_template_id);
			return nullptr;
		}
		else if (init_unit::init_item_data(temp_info, item_template_id, protect_item_slot, item_num, e_bag_type_protect, 0, locked, item_over_time))
		{
			citem* temp = create_item_by_info(temp_info, add_type, param, true);
			return temp;
		}
	}
	else if (item_template_ptr->item_type == e_item_type_fashion)
	{
		int32 fashion_item_slot = get_empty_slot(e_bag_type_fasion);
		if (fashion_item_slot < 0)
		{
			CONSOLE_ERROR("fashion_item_slot is invalid item_template_id:{}", item_template_id);
			return nullptr;
		}
		else if (init_unit::init_item_data(temp_info, item_template_id, fashion_item_slot, item_num, e_bag_type_fasion, 1, locked, item_over_time))
		{
			citem* temp = create_item_by_info(temp_info, add_type, param, true);
			return temp;
		}
	}
	else if (item_template_ptr->item_type == e_item_type_spirit)
	{
		int32 item_slot = get_empty_slot(e_bag_type_equip_spirit);
		if (item_slot < 0)
		{
			CONSOLE_ERROR("item_slot is invalid item_template_id:{}", item_template_id);
			return nullptr;
		}
		else if (init_unit::init_item_data(temp_info, item_template_id, item_slot, item_num, e_bag_type_equip_spirit, 1, locked, item_over_time))
		{
			citem* temp = create_item_by_info(temp_info, add_type, param, true);
			return temp;
		}
	}
	else
	{
		if (init_unit::init_item_data(temp_info, item_template_id, -1, item_num, bag_type, 0, locked, item_over_time, item_const_att))
		{
			citem* new_citem_ptr = create_item_by_info(temp_info, add_type, param, true);
			if (new_citem_ptr && item_template_ptr->item_type == e_item_type_spirit)
			{
				player_ref.get_phantom_mgr().on_item_oper(new_citem_ptr, new_citem_ptr->get_data_info(e_item_info_info_id));
			}
			return new_citem_ptr;
			//return create_item_by_info(temp_info, add_type, param, true);
		}
	}
	CONSOLE_ERROR("create item failed item_template_id:{}", item_template_id);
	return nullptr;
}

citem* item_set::create_item_by_info(const s_item_info& temp_info, e_server_log_add_item add_type, int32 param, bool write_log)
{
	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, temp_info.data_ary[e_item_info_info_id]);
	if (nullptr == item_template_ptr)
	{
		CONSOLE_ERROR("item_template_ptr is nullptr item_template_id:{}", temp_info.data_ary[e_item_info_info_id]);
		return nullptr;
	}

	guid_64 item_guid = temp_info.item_guid;
	e_bag_type	item_container = (e_bag_type)temp_info.data_ary[e_item_info_container_type];
	int32	item_slot = temp_info.data_ary[e_item_info_slot];
	if (item_slot >= 0)
	{
		guid_64 old_guid = get_target_slot_guid(item_container, item_slot);
		if (old_guid.is_valid())
		{
			int32 empty_slot = get_empty_slot(item_container);
			if (empty_slot < 0)
			{
				CONSOLE_ERROR("empty_slot is invalid item_template_id:{}", temp_info.data_ary[e_item_info_info_id]);
				return nullptr;
			}
			item_slot = empty_slot;
		}
	}
	citem* item_ptr = cache_mgr::get_instance().get_item_ptr();
	if (item_guid.is_valid())
	{
		item_ptr->set_item_data(temp_info);
		item_ptr->set_data_info(e_item_info_slot, item_slot);
		auto item_inserted = m_item_map.insert({ item_guid.server_64, item_ptr });
		if (false == item_inserted.second)
		{
			int32 have_info_id = 0;
			citem* have_item = get_item(item_guid);
			if (have_item)
			{
				have_info_id = have_item->get_data_info(e_item_info_info_id);
			}
			CONSOLE_ERROR("item input error! insert guid {} insert info_id {} old_info_id {} add_type {}", item_guid.server_64, temp_info.data_ary[e_item_info_info_id], have_info_id, (int32)add_type);
			guid_64 new_guid = guid_gen::make_guid();
			item_ptr->set_item_guid(new_guid);
			item_guid = new_guid;
			item_inserted = m_item_map.insert({ item_guid.server_64, item_ptr });
			if (false == item_inserted.second)
			{
				cache_mgr::get_instance().back_item_ptr(item_ptr);
				CONSOLE_ERROR("item input error! new guid {} info_id {} info_num {}"
					, item_guid.server_64
					, temp_info.data_ary[e_item_info_info_id]
					, temp_info.data_ary[e_item_info_stack_count]);
				return nullptr;
			}
		}
		set_target_slot_guid((e_bag_type)item_container, item_slot, item_guid);
		if (item_container == e_bag_type_equip)
		{
			if (temp_info.data_ary[e_item_info_over_time] > 0)
			{
				m_time_limit_item_in_equip[item_slot] = item_guid;
			}
		}
		else if (item_container == e_bag_type_wing || item_container == e_bag_type_mount)
		{
			int32 empty_slot = get_use_time_item_empty_slot();
			if (empty_slot >= 0 && empty_slot < MAX_TIME_LIMIT_ITEM_NUM)
			{
				m_used_has_time_item_array[empty_slot] = item_guid;
			}
		}

		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() && player_ref.get_write_log() && write_log)
		{
			server_log::item_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), 1, temp_info.data_ary[e_item_info_info_id], temp_info.data_ary[e_item_info_stack_count], add_type, param, item_guid, temp_info.data_ary[e_item_info_stack_count], player_ref.get_login_type());

			set_log_var(log_head);

			player_ref.get_log_common_head_info(log_head);

			server_log::serverPropChange(log_head, init_unit::change_i32_to_string(temp_info.data_ary[e_item_info_info_id]), 1, add_type, temp_info.data_ary[e_item_info_stack_count], temp_info.data_ary[e_item_info_stack_count], param);
		}
		return item_ptr;
	}
	CONSOLE_ERROR("item_guid is invalid item_template_id:{}", temp_info.data_ary[e_item_info_info_id]);
	return nullptr;
}

bool item_set::put_item_into_bag(citem* item_ptr, e_bag_type bag_type, bool merge, e_update_item_info_mode update_mode, bool is_show_go_to_bag, bool is_continue_put)
{
	ZoneScoped;
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_ptr is nullptr");
		return false;
	}
	if (bag_type <= e_bag_type_none || bag_type >= e_bag_type_max)
	{
		CONSOLE_ERROR("bag_type is invalid bag_type:{}", (int32)bag_type);
		return false;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player is invalid m_unit_array_index:{}", m_unit_array_index);
		return false;
	}
	auto item_template_ptr = item_ptr->get_item_info_ptr();
	if (item_template_ptr->max_pile_num <= 0)
	{
		CONSOLE_ERROR("item max_pile_num is invalid max_pile_num:{}", item_template_ptr->max_pile_num);
		return false;
	}
	if (item_template_ptr->item_type == faith::e_item_type_stone && item_template_ptr->sub_type == faith::e_stone_sub_type_psyche)
	{
		update_mode = e_update_item_info_mode_default;
	}

	if (item_ptr->get_data_info(e_item_info_container_type) == bag_type)
	{
		send_item_one(item_ptr);
		return true;
	}

	int32 item_template_id = item_ptr->get_data_info(e_item_info_info_id);
	int32 old_bag_typ = item_ptr->get_data_info(e_item_info_container_type);
	int32 old_slot = item_ptr->get_data_info(e_item_info_slot);
	int32 item_lock = item_ptr->get_data_info(e_item_info_locked);
	int32 total_item_num = item_ptr->get_data_info(e_item_info_stack_count);
	int32 put_count = item_ptr->get_data_info(e_item_info_put_in_bag_count);
	int32 over_time = item_ptr->get_data_info(e_item_info_over_time);

	int32 left_item_num = total_item_num;

	if (total_item_num <= 0)
	{
		del_item(e_server_log_del_item_bag_zero, old_slot, item_ptr, 1);
		return true;
	}

	if (item_template_ptr->AutoUse > 0)
	{
		citem* end_item = nullptr;
		return item_use(item_ptr->get_item_guid(), end_item, left_item_num, 0, is_continue_put);
	}

	auto& guid_array = GET_BAG(bag_type);

	if (merge)
	{
		for (auto& it : guid_array)
		{
			auto exist_item = get_item(it.second);
			if (exist_item == nullptr || exist_item->get_data_info(e_item_info_info_id) != item_template_id || exist_item->get_data_info(e_item_info_locked) != item_lock)
			{
				continue;
			}
			int32 exist_num = exist_item->get_data_info(e_item_info_stack_count);
			if (exist_num >= item_template_ptr->max_pile_num)
			{
				continue;
			}
			int32 sum_num = exist_num + left_item_num;
			if (sum_num > item_template_ptr->max_pile_num)
			{
				exist_item->set_data_info(e_item_info_stack_count, item_template_ptr->max_pile_num);
				left_item_num = sum_num - item_template_ptr->max_pile_num;
				item_ptr->set_data_info(e_item_info_stack_count, left_item_num);
				send_item_one(exist_item);
			}
			else
			{
				exist_item->set_data_info(e_item_info_stack_count, sum_num);
				left_item_num = 0;
				item_ptr->set_data_info(e_item_info_stack_count, left_item_num);
				send_item_one(exist_item);
				break;
			}
		}
	}

	if (left_item_num > 0)
	{
		if (left_item_num > item_template_ptr->max_pile_num)
		{
			int32 need_slot_num = left_item_num / item_template_ptr->max_pile_num;
			for (int32 i = 0; i < need_slot_num; ++i)
			{
				int32 item_slot = get_empty_slot(bag_type);
				if (item_slot < 0)
				{
					break;
				}
				citem* new_item = create_item_by_template(e_server_log_add_item_bag_split, 0, item_template_ptr->attribute_id, item_template_ptr->max_pile_num, item_ptr->get_data_info(e_item_info_locked));
				if (nullptr == new_item)
				{
					break;
				}

				new_item->set_data_info(e_item_info_container_type, bag_type);
				new_item->set_data_info(e_item_info_slot, item_slot);
				guid_array[item_slot] = new_item->get_item_guid();
				send_item_one(new_item, true, update_mode);

				left_item_num -= item_template_ptr->max_pile_num;
				item_ptr->set_data_info(e_item_info_stack_count, left_item_num);
			}
		}
		if (left_item_num > 0)
		{
			int32 item_slot = get_empty_slot(bag_type);
			if (item_slot < 0)
			{
				item_ptr->set_data_info(e_item_info_stack_count, left_item_num);
				send_item_one(item_ptr, true, update_mode);
				CONSOLE_ERROR("item item_slot is invalid item_slot:{} bag_type:{}", item_slot, (int32)bag_type);
				return false;
			}
			else
			{
				item_ptr->set_data_info(e_item_info_container_type, bag_type);
				item_ptr->set_data_info(e_item_info_slot, item_slot);
				item_ptr->set_data_info(e_item_info_stack_count, left_item_num);
				guid_array[item_slot] = item_ptr->get_item_guid();
				send_item_one(item_ptr, true, update_mode);
			}
		}
	}
	clear_target_slot((e_bag_type)old_bag_typ, old_slot);

	if (is_show_go_to_bag && bag_type == e_bag_type_bag)
	{
		item_proto_simple_item_info show_go_to_bag_info;
		show_go_to_bag_info.set_item_template_id(item_template_id);
		show_go_to_bag_info.set_item_num(total_item_num);
		player_ref.send_message_to_self(&show_go_to_bag_info, e_msgindex_s2c_item_show_goto_bag);
	}

	if (put_count == 0)
	{
		item_ptr->set_data_info(e_item_info_put_in_bag_count, 1);
		if (item_template_ptr->item_type == e_item_type_weapon
			|| item_template_ptr->item_type == e_item_type_armor)
		{
			if (item_template_ptr->item_color >= e_item_color_red)
			{
				const std::vector<std::string>& temp_string_array_list = template_manager::get_instance().get_string_list(e_item_rear_item_get_notice);
				const std::vector<std::string>& temp_item_string_array_list = template_manager::get_instance().get_string_list(item_template_ptr->ItemName);
				if (temp_string_array_list.size() >= 2
					&& temp_item_string_array_list.size() >= 1)
				{
					std::string first_string = temp_string_array_list[0];
					std::string second_string = temp_string_array_list[1];

					std::string final_string = first_string;
					final_string += player_ref.get_name();
					final_string += second_string;
					final_string += temp_item_string_array_list[0];
					player_ref.get_chat_mgr().send_globel_message(final_string);
				}
			}
		}
	}

	if (left_item_num <= 0)
	{
		del_item(e_server_log_del_item_bag_merge, 0, item_ptr, total_item_num);
	}
	return true;
}

bool item_set::put_in_bag(citem* item_ptr, e_bag_type bag_type, e_item_mail_type mail_type)
{
	if (item_ptr == nullptr)
	{
		return false;
	}
	std::vector<citem*> item_inst_array;
	item_inst_array.push_back(item_ptr);
	return put_in_bag(item_inst_array, bag_type, mail_type);
}
bool item_set::put_in_bag(std::vector<citem*>& item_inst_array, e_bag_type bag_type, e_item_mail_type mail_type)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return false;
	}
	const int32 item_inst_array_size = item_inst_array.size();
	if (item_inst_array_size <= 0)
	{
		return false;
	}
	vector<citem*> will_send_email_item_array;

	for (int32 item_array_index = 0; item_array_index < item_inst_array_size; item_array_index++)
	{
		citem* item_ptr = item_inst_array[item_array_index];
		if (item_ptr == nullptr)
		{
			continue;
		}
		int32 item_template_id = item_ptr->get_data_info(e_item_info_info_id);
		int32 item_num = item_ptr->get_data_info(e_item_info_stack_count);
		int32 is_item_locked = item_ptr->get_data_info(e_item_info_locked);
		auto in_bag_type = bag_type == e_bag_type_none ? init_unit::get_item_bag_type(item_template_id) : bag_type;
		if (put_item_into_bag(item_ptr, in_bag_type, true, e_update_item_info_mode_pick_up_new, true, true) == false)
		{
			will_send_email_item_array.push_back(item_ptr);
		}
	}
	bool all_success = will_send_email_item_array.empty();
	if (will_send_email_item_array.empty() == false)
	{
		if (mail_type > 0)
		{
			send_item_by_mail(will_send_email_item_array, mail_type);
			player_ref.get_chat_mgr().receive_top_tip(90096108);
		}
		else
		{
			std::vector<s_del_item_guid> item_guids;
			for (auto& it : will_send_email_item_array)
			{
				item_guids.push_back(it->get_item_guid());
			}
			del_item(e_server_log_del_item_full_bag, bag_type, item_guids);
		}
	}
	return all_success;
}
int32 item_set::get_item_from_treasure_to_bag()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (!temp_player.is_valid())
	{
		return false;
	}
	bool is_bag_full = false;
	for (int32 i = 0; i < MAX_ITEM_TREASURE_NUM; ++i)
	{
		citem* item_ptr = get_item(GET_BAG(e_bag_type_treasure)[i]);
		if (nullptr == item_ptr)
		{
			continue;
		}
		if (put_in_bag(item_ptr, e_bag_type_bag) == false)
		{
			is_bag_full = true;
		}
	}
	tidy_item(e_bag_type_treasure);

	if (is_bag_full)
	{
		std::string show_string = template_manager::get_instance().get_str_id_by_notice_id(bag_is_full_notice_id);
		temp_player.get_chat_mgr().send_notice(bag_is_full_notice_id, show_string);
	}
	return true;
}
void item_set::init_item_from_treasure_to_mail()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (!temp_player.is_valid())
	{
		return;
	}
	bool is_bag_full = false;
	for (int32 i = 0; i < MAX_ITEM_TREASURE_NUM; ++i)
	{
		citem* item_ptr = get_item(GET_BAG(e_bag_type_treasure)[i]);
		if (nullptr == item_ptr)
		{
			continue;
		}

		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_treasure_clear_title);
		const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_treasure_clear_context);
		if (sender_name == nullptr
			|| mail_title == nullptr
			|| mail_content == nullptr)
		{
			return;
		}
		globle_data::get_instance().send_mail_to_another_player_by_system(
			temp_player.get_unit_guid(),
			temp_player.get_unit_guid(),
			temp_player.get_unit_info(e_role_info_server_id),
			"", mail_title, mail_content,
			0, 0, 0, 0, &item_ptr->get_item_inst(), 1);

		del_item(e_server_log_del_item_treasure_clear, 0, item_ptr, item_ptr->get_data_info(e_item_info_stack_count));
	}
}

void item_set::send_item_by_mail(std::vector<citem*>& item_array, e_item_mail_type mail_type)
{
	std::vector<s_item_info> item_info_array;
	item_info_array.reserve(item_array.size());

	for (int32 i = 0; i < item_array.size(); ++i)
	{
		citem* item_ptr = item_array[i];
		if (item_ptr == nullptr)
		{
			continue;
		}
		item_info_array.push_back(item_ptr->get_item_inst());
		del_item(e_server_log_del_item_send_mail, 0, item_ptr, item_ptr->get_data_info(e_item_info_stack_count));
	}
	send_item_by_mail(item_info_array, mail_type);
}

void item_set::send_item_by_mail(std::vector<s_item_info>& item_array, e_item_mail_type mail_type)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	s_item_info mail_item_info_array[max_item_per_mail];
	int32 item_in_mail_index = 0;
	const int32 item_inst_array_size = item_array.size();
	for (int32 item_inst_array_index = 0; item_inst_array_index < item_inst_array_size; item_inst_array_index++)
	{
		if (item_in_mail_index < max_item_per_mail)
		{
			mail_item_info_array[item_in_mail_index] = item_array[item_inst_array_index];
			if (item_in_mail_index == max_item_per_mail - 1)
			{
				globle_data::get_instance().send_mail_to_another_player_by_system(
					temp_unit.get_unit_guid(),
					temp_unit.get_unit_guid(),
					temp_unit.get_unit_info(e_role_info_server_id),
					mail_type,
					0,
					0,
					0,
					0,
					mail_item_info_array,
					max_item_per_mail);
				memset(mail_item_info_array, 0, sizeof(mail_item_info_array));
				item_in_mail_index = 0;
			}
			else
			{
				item_in_mail_index++;
			}
		}
	}
	if (item_in_mail_index > 0)
	{
		globle_data::get_instance().send_mail_to_another_player_by_system(
			temp_unit.get_unit_guid(),
			temp_unit.get_unit_guid(),
			temp_unit.get_unit_info(e_role_info_server_id),
			mail_type,
			0,
			0,
			0,
			0,
			mail_item_info_array,
			max_item_per_mail);
	}
}

std::vector<s_item_template_info> item_set::get_item_template_info_array(const std::vector<int32>& item_array, e_item_transform_type item_transform_type)
{
	std::vector<s_item_template_info> item_template_info_array;
	const int32 item_array_size = item_array.size();
	switch (item_transform_type)
	{
	case faith::e_item_transform_type_two:
	{
		if (item_array_size % 2 != 0)
		{
			break;
		}
		for (int32 item_array_index = 0; item_array_index < item_array_size; item_array_index += 2)
		{
			item_template_info_array.push_back(s_item_template_info(item_array[item_array_index], item_array[item_array_index + 1], 1));
		}
		break;
	}
	case faith::e_item_transform_type_three:
	{
		if (item_array_size % 3 != 0)
		{
			break;
		}
		for (int32 item_array_index = 0; item_array_index < item_array_size; item_array_index += 3)
		{
			item_template_info_array.push_back(s_item_template_info(item_array[item_array_index], item_array[item_array_index + 1], item_array[item_array_index + 2]));
		}
		break;
	}
	case faith::e_item_transform_type_four:
	{
		if (item_array_size % 4 != 0)
		{
			break;
		}
		for (int32 item_array_index = 0; item_array_index < item_array_size; item_array_index += 4)
		{
			item_template_info_array.push_back(s_item_template_info(item_array[item_array_index], item_array[item_array_index + 1], item_array[item_array_index + 2]));
		}
		break;
	}
	default:
		CONSOLE_ERROR("item_transform_type error! item_transform_type {}", (int32)item_transform_type);
		break;
	}
	return item_template_info_array;
}

std::vector<s_item_info> item_set::get_item_info_array(std::vector<citem*>& item_ptr_array)
{
	std::vector<s_item_info> item_info_array;
	const int32 item_ptr_array_size = item_ptr_array.size();
	for (int32 item_ptr_array_index = 0; item_ptr_array_index < item_ptr_array_size; item_ptr_array_index++)
	{
		citem* item_ptr = item_ptr_array[item_ptr_array_index];
		if (nullptr == item_ptr)
		{
			continue;
		}
		item_info_array.push_back(item_ptr->get_item_inst());
	}
	return item_info_array;
}

s_item_info item_set::filter_items_with_notice(std::vector<citem*> item_ptr_array, std::string& item_names_str, int32 notices_id)
{
	int32 notice_id = 93000204;
	//std::vector<citem*> filter_item_ptr_array;
	s_item_info item_data;
	const int32 item_ptr_array_size = item_ptr_array.size();
	for (int32 item_ptr_array_index = 0; item_ptr_array_index < item_ptr_array_size; item_ptr_array_index++)
	{
		citem* new_item_ptr = item_ptr_array[item_ptr_array_index];
		if (nullptr == new_item_ptr)
		{
			continue;
		}
		if (notices_id == notice_id)
		{
			if (new_item_ptr->get_item_color() >= e_item_color_orange)
			{
				ItemTemplate* item_template_ptr = new_item_ptr->get_item_info_ptr();
				if (nullptr != item_template_ptr)
				{
					if (item_template_ptr->item_type == e_item_type_weapon
						|| item_template_ptr->item_type == e_item_type_armor)
					{
						item_data = new_item_ptr->get_item_inst();
						//filter_item_ptr_array.push_back(new_item_ptr);
						item_names_str += " ";
						item_names_str += "%d";
					}
				}
			}
		}
		else
		{
			if (new_item_ptr->get_item_color() >= e_item_color_orange)
			{
				//filter_item_ptr_array.push_back(new_item_ptr);
				item_data = new_item_ptr->get_item_inst();
				item_names_str += " ";
				item_names_str += "%d";
			}
			else if (new_item_ptr->get_item_color() == e_item_color_purple)
			{
				ItemTemplate* item_template_ptr = new_item_ptr->get_item_info_ptr();
				if (nullptr != item_template_ptr)
				{
					if (item_template_ptr->item_type == e_item_type_weapon
						|| item_template_ptr->item_type == e_item_type_armor)
					{
						item_data = new_item_ptr->get_item_inst();
						//filter_item_ptr_array.push_back(new_item_ptr);
						item_names_str += " ";
						item_names_str += "%d";
					}
				}
			}
		}
	}
	return item_data;
}

void item_set::set_quick_hp_guid(guid_64 new_guid)
{
	m_quick_hp_guid = new_guid;
}

int32 item_set::against_buy_item(guid_64 item_guid, int32 goods_id)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return false;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return false;
	}

	if (item_ptr->get_data_info(e_item_info_activate) > 0 && item_ptr->get_data_info(e_item_info_over_time) == 0)
	{
		return false;
	}

	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return false;
	}
	int32 money_type_index = region_template_ptr->RegionCode * 2;
	int32 money_num_index = money_type_index + 1;

	GoodsTemplate* goods_template = GET_TEMPLATE(GoodsTemplate, goods_id);
	if (goods_template == nullptr
		|| goods_template->NewPrice.size() <= money_num_index)
	{
		return false;
	}

	if (temp_player.can_cut_money((e_money_type)goods_template->NewPrice[money_type_index], goods_template->NewPrice[money_num_index]) == false)
	{
		return false;
	}

	temp_player.cut_money((e_money_type)goods_template->NewPrice[money_type_index], goods_template->NewPrice[money_num_index], e_server_log_cut_money_item_against_buy, goods_id);

	int32 cur_second_time = time_helper::get_cur_time_new().second;
	int32 item_info_over_time = 0;
	bool is_equip = true;

	if (cur_second_time >= item_ptr->get_data_info(e_item_info_over_time))
	{
		if (goods_template->EffectiveTime == 0)
		{
			item_ptr->set_data_info(e_item_info_over_time, 0);
		}
		else
		{
			if (item_ptr->get_data_info(e_item_info_over_time) < 0)
			{
				item_info_over_time = item_ptr->get_data_info(e_item_info_over_time) - goods_template->EffectiveTime;
				item_ptr->set_data_info(e_item_info_over_time, item_info_over_time);
			}
			else
			{
				item_info_over_time = cur_second_time + goods_template->EffectiveTime;
				item_ptr->set_data_info(e_item_info_over_time, item_info_over_time);
			}
		}
	}
	else
	{
		if (goods_template->EffectiveTime == 0)
		{
			item_ptr->set_data_info(e_item_info_over_time, 0);
		}
		else
		{
			item_info_over_time = item_ptr->get_data_info(e_item_info_over_time) + goods_template->EffectiveTime;
			item_ptr->set_data_info(e_item_info_over_time, item_info_over_time);
		}
		is_equip = false;
	}

	item_ptr->set_data_info(e_item_info_activate, 1);
	send_item_one(item_ptr);

	int32  in_which_slot = item_ptr->get_data_info(e_item_info_slot);
	if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip_fasion && is_equip)
	{
		equip_on(item_guid, in_which_slot, false, true);
	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_mount)
	{
		temp_player.refresh_service_goal(e_service_goal_type_mount_num);
		temp_player.get_mission_mgr().target_check(e_mission_end_type_mount_unlock);
		temp_player.get_achievement_mgr().item_changed(item_ptr, e_item_operation_mount_unlock);
		add_new_quick_mount_call(item_ptr->get_item_guid());
		temp_player.get_mission_mgr().target_check(e_mission_end_type_mount_total_grade);
		if (is_equip)
		{
			MountTemplate* item_mount_template_ptr = item_ptr->get_mount_template_ptr();
			if (item_mount_template_ptr != nullptr)
			{
				item_change_att(item_mount_template_ptr->AttArray, 1, true);
				show_this_mount(item_ptr, -1);
				int32 empty_slot = get_use_time_item_empty_slot();
				if (empty_slot < 0 || empty_slot >= MAX_TIME_LIMIT_ITEM_NUM)
				{
					return false;
				}
				m_used_has_time_item_array[empty_slot] = item_guid;
			}
		}
	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_wing)
	{
		WingTemplate* wing_template_ptr = item_ptr->get_wing_template_ptr();
		if (wing_template_ptr)
		{
			if (wing_template_ptr->StarBuffId > 0)
			{
				PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, wing_template_ptr->StarBuffId);
				if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
				{
					star_skill_unlock(item_guid, item_ptr->get_data_info(e_item_info_upgrade_count), false, false);
				}
			}
			if (is_equip)
			{
				item_change_att(wing_template_ptr->AttArray, 1, true);
				show_this_wing(item_ptr, -1);
				int32 empty_slot = get_use_time_item_empty_slot();
				if (empty_slot < 0 || empty_slot >= MAX_TIME_LIMIT_ITEM_NUM)
				{
					return false;
				}
				m_used_has_time_item_array[empty_slot] = item_guid;
			}
		}
	}
	temp_player.get_phantom_mgr().on_item_oper(item_ptr, item_ptr->get_data_info(e_item_info_info_id));

	return true;
}

void  item_set::check_time_limit_item()
{
	//check_time_limit_fashion_item();
	check_time_limit_mount_and_wing_item();
	check_time_limit_equip_item();
}

void  item_set::check_time_limit_fashion_item()
{
	for (int32 i = 0; i < e_fashion_equip_slot_max; ++i)
	{
		if (GET_BAG(e_bag_type_equip_fasion)[i].is_valid())
		{
			int32 cur_second_time = time_helper::get_cur_time_new().second;
			citem* item_fashion_ptr = get_item(GET_BAG(e_bag_type_equip_fasion)[i]);
			if (nullptr == item_fashion_ptr)
			{
				GET_BAG(e_bag_type_equip_fasion)[i].clear_data();
				continue;
			}
			int32 item_fashion_over_time = item_fashion_ptr->get_data_info(e_item_info_over_time);
			if (item_fashion_over_time <= 0)
			{
				continue;
			}
			if (cur_second_time >= item_fashion_over_time)
			{
				if (0 == item_fashion_ptr->get_data_info(e_item_info_activate))
				{
					continue;
				}

				item_fashion_ptr->set_data_info(e_item_info_activate, 0);
				send_item_one(item_fashion_ptr);
				equip_off_upgrade_show_buff();
				//set_fashion_att(item_fashion_ptr, false);
				equip_on_upgrade_show_buff();
				player& temp_player_ref = unit_man::get_player(m_unit_array_index);
				if (temp_player_ref.is_valid())
				{
					temp_player_ref.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_fashion_ptr);
				}
				item_fashion_ptr->set_unit_buff(m_unit_array_index, false);
			}
		}
	}
	reset_weapon_fashion_effect();
}

void  item_set::check_time_limit_mount_and_wing_item()
{
	for (int32 i = 0; i < MAX_TIME_LIMIT_ITEM_NUM; ++i)
	{
		if (m_used_has_time_item_array[i].is_valid())
		{
			citem* item_ptr = get_item(m_used_has_time_item_array[i]);
			if (nullptr == item_ptr)
			{
				m_used_has_time_item_array[i].clear_data();
				continue;
			}
			if (item_ptr->get_data_info(e_item_info_activate) <= 0)
			{
				m_used_has_time_item_array[i].clear_data();
				continue;
			}
			if (item_ptr->get_data_info(e_item_info_over_time) > 0)
			{
				int32 cur_second_time = time_helper::get_cur_time_new().second;
				if (cur_second_time >= item_ptr->get_data_info(e_item_info_over_time))
				{
					if (item_ptr->get_data_info(e_item_info_activate) == 0)
					{
						m_used_has_time_item_array[i].clear_data();
						continue;
					}
					item_ptr->set_data_info(e_item_info_activate, 0);
					send_item_one(item_ptr);

					player& temp_player_ref = unit_man::get_player(m_unit_array_index);
					if (!temp_player_ref.is_valid())
					{
						continue;
					}

					if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_mount)
					{
						MountTemplate* temp_mount_template_ptr = item_ptr->get_mount_template_ptr();
						if (temp_mount_template_ptr == nullptr)
						{
							continue;
						}
						item_change_att(temp_mount_template_ptr->AttArray, 1, false);
						if (temp_player_ref.get_unit_info(e_role_info_mount_showd_template_id) != item_ptr->get_data_info(e_item_info_info_id))
						{
							continue;
						}
						show_this_init_mount();

					}
					else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_wing)
					{
						WingTemplate* wing_template_ptr = item_ptr->get_wing_template_ptr();
						if (wing_template_ptr == nullptr)
						{
							continue;
						}
						item_change_att(wing_template_ptr->AttArray, 1, false);
						if (temp_player_ref.get_unit_info(e_role_info_wing_showd_template_id) != item_ptr->get_data_info(e_item_info_info_id))
						{
							continue;
						}

						show_this_init_max_wing(item_ptr);
					}
					m_used_has_time_item_array[i].clear_data();
				}
			}
			else
			{
				m_used_has_time_item_array[i].clear_data();
			}
		}
	}

}

citem* item_set::get_can_auto_use_hp_item()
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return nullptr;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return nullptr;
	}
	int32 money_type_index = region_template_ptr->RegionCode * 2;
	int32 money_num_index = money_type_index + 1;


	const int32 len = 5;
	int32 hp_item_type[len] = {
		e_prop_sub_type_hp_essence,
		e_prop_sub_type_hp_shop,
		e_prop_sub_type_hp_max,
		e_prop_sub_type_hp_mld,
		e_prop_sub_type_hp_min
	};
	citem* hp_item_ptr = nullptr;
	guid_64 hp_item_guid = guid_64();
	int32 cur_player_level = temp_player_ref.get_unit_info(e_role_info_exp_level);
	for (int32 i = 0; i < len; i++)
	{
		int32 item_sub_type = hp_item_type[i];
		hp_item_guid = get_item_by_type(e_item_type_expendable, item_sub_type);
		hp_item_ptr = get_item(hp_item_guid);
		if (nullptr != hp_item_ptr)
		{
			int32 item_temp_id = hp_item_ptr->get_item_template_id();
			ItemTemplate* item_temp_ptr = GET_TEMPLATE(ItemTemplate, item_temp_id);
			if (nullptr == item_temp_ptr || item_temp_ptr->level_limit.size() < 2)
			{
				continue;
			}
			int32 item_use_level_limit = item_temp_ptr->level_limit[0];
			int32 item_use_level_limit_max = item_temp_ptr->level_limit[1];
			if (cur_player_level < item_use_level_limit || cur_player_level > item_use_level_limit_max)
			{
				continue;
			}
			return hp_item_ptr;
		}
	}
	if (temp_player_ref.is_auto_by_hp_pot() == false)
	{
		return nullptr;
	}
	int64 player_bind_money_num = temp_player_ref.get_money_data(e_money_type_silver_bind);
	s_store_goods* goods_arr = temp_player_ref.get_goods_helper_mgr().get_store_goods_by_id(e_store_type_medicine);
	if (nullptr == goods_arr || goods_arr->is_valid() == false)
	{
		return nullptr;
	}
	for (int32 i = 0; i < goods_max_num; i++)
	{
		cgoods* goods_ref = goods_arr->get_goods_by_idex(i);
		if (nullptr == goods_ref)
		{
			continue;
		}
		GoodsTemplate* goods_temp_ptr = goods_ref->get_goods_template_ptr();
		if (nullptr == goods_temp_ptr)
		{
			continue;
		}
		ItemTemplate* item_template_ptr = goods_ref->get_item_template_ptr();
		if (nullptr == item_template_ptr)
		{
			continue;
		}
		int32 item_use_level_limit = item_template_ptr->level_limit[0];
		int32 item_use_level_limit_max = item_template_ptr->level_limit[1];
		if (cur_player_level >= item_use_level_limit && cur_player_level < item_use_level_limit_max)
		{
			int32 good_num = max_auto_buy_medicine_num;
			int32 good_price = 0;
			if (goods_temp_ptr->NewPrice.size() > money_num_index)
			{
				good_price = goods_temp_ptr->NewPrice[money_num_index];
			}
			if (player_bind_money_num / good_price < max_auto_buy_medicine_num)
			{
				good_num = player_bind_money_num / good_price;
			}
			if (good_num <= 0)
			{
				return nullptr;
			}
			temp_player_ref.buy_goods(goods_temp_ptr->attribute_id, good_num, e_store_type_medicine, true);
			return nullptr;
		}
	}
	return nullptr;
}

void item_set::check_time_limit_equip_item()
{
	for (int32 i = e_role_equip_slot_body; i < e_role_equip_slot_max; i++)
	{
		if (!m_time_limit_item_in_equip[i].is_valid())
		{
			continue;
		}
		citem* item_ptr = get_item(m_time_limit_item_in_equip[i]);
		if (nullptr == item_ptr)
		{
			m_time_limit_item_in_equip[i].clear_data();
			continue;
		}
		guid_64 item_guid = item_ptr->get_item_guid();
		if (!is_in_duration(item_guid))
		{
			if (item_ptr->get_add_att())
			{
				change_att_for_equip(item_guid, false);
				item_ptr->set_add_att(false);
			}
			m_time_limit_item_in_equip[i].clear_data();
		}
	}
}

int32  item_set::show_this_init_mount()
{
	citem* temp_item_mount = get_mount_by_mount_template_id(init_mount_template_id);
	if (nullptr == temp_item_mount)
	{
		return -1;
	}
	if (temp_item_mount->get_data_info(e_item_info_activate) < 1)
	{
		temp_item_mount->set_data_info(e_item_info_activate, 1);
		MountTemplate* temp_item_mount_template_ptr = temp_item_mount->get_mount_template_ptr();
		if (nullptr == temp_item_mount_template_ptr)
		{
			return -1;
		}
		item_change_att(temp_item_mount_template_ptr->AttArray, 1, true);
	}

	return show_this_mount(temp_item_mount, -1);
}

int32  item_set::show_this_init_max_wing(citem* item_ptr)
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return -1;
	}
	citem* temp_item_wing_ptr = get_cur_level_wing();
	if (nullptr == temp_item_wing_ptr)
	{
		int32 class_type = temp_player_ref.get_unit_info(e_role_info_class_type);
		int32 item_wing_template_id = 0;
		switch (class_type)
		{
		case e_class_type_barserker:
			item_wing_template_id = 31040001;
			break;
		case e_class_type_wizard:
			item_wing_template_id = 31040011;
			break;
		case e_class_type_guardian:
			item_wing_template_id = 31040021;
			break;
		case e_class_type_assassinator:
			item_wing_template_id = 31040031;
			break;
		}
		ItemTemplate* item_ptr_lock = GET_TEMPLATE(ItemTemplate, item_wing_template_id);
		if (nullptr != item_ptr_lock)
		{
			bool is_unlock = temp_player_ref.get_func_unlock_mgr().is_func_unlock(item_ptr_lock->FuncUnlockid);
			if (false == is_unlock && nullptr != item_ptr)
			{
				item_ptr->set_data_info(e_item_info_activate, 1);
				show_this_wing(item_ptr, -1, true, true);
				item_ptr->set_data_info(e_item_info_activate, 0);
				return -1;
			}
		}

		temp_item_wing_ptr = get_item_by_template_id(item_wing_template_id);
		if (nullptr == temp_item_wing_ptr)
		{
			return -1;
		}
		if (temp_item_wing_ptr->get_data_info(e_item_info_activate) < 1)
		{
			temp_item_wing_ptr->set_data_info(e_item_info_activate, 1);
			WingTemplate* temp_wing_template_ptr = temp_item_wing_ptr->get_wing_template_ptr();
			if (temp_wing_template_ptr == nullptr)
			{
				return -1;
			}
			item_change_att(temp_wing_template_ptr->AttArray, 1, true);
		}
	}

	ItemTemplate* item_ptr_lock = GET_TEMPLATE(ItemTemplate, temp_item_wing_ptr->get_item_template_id());
	if (nullptr != item_ptr_lock)
	{
		bool is_unlock = temp_player_ref.get_func_unlock_mgr().is_func_unlock(item_ptr_lock->FuncUnlockid);
		if (false == is_unlock && nullptr != item_ptr)
		{
			item_ptr->set_data_info(e_item_info_activate, 1);
			show_this_wing(item_ptr, -1, true, true);
			item_ptr->set_data_info(e_item_info_activate, 0);
			return -1;
		}
	}


	if (temp_item_wing_ptr->get_data_info(e_item_info_activate) < 1)
	{
		temp_item_wing_ptr->set_data_info(e_item_info_activate, 1);
		WingTemplate* temp_item_wing_template_ptr = temp_item_wing_ptr->get_wing_template_ptr();
		if (temp_item_wing_template_ptr == nullptr)
		{
			return -1;
		}
		item_change_att(temp_item_wing_template_ptr->AttArray, 1, true);
	}

	return show_this_wing(temp_item_wing_ptr, -1);

}

bool item_set::has_spirit_on_equip()
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return false;
	}
	//int32 cur_spirit_slot_open_num = temp_player_ref.get_spirit_mgr().get_sprite_slot_effect_num();
	//for (int32 spirit_slot = 0; spirit_slot < cur_spirit_slot_open_num; spirit_slot++)
	//{
	//	if (GET_BAG(e_bag_type_equip_spirit)[spirit_slot].is_valid())
	//	{
	//		return true;
	//	}
	//}
	return false;
}
void item_set::send_spirit_msg(citem* temp_item, bool is_del)
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return;
	}
	if (nullptr == temp_item)
	{
		return;
	}
	character_proto_update_spirit spirit_msg;
	spirit_msg.set_role_guid(temp_player_ref.get_unit_guid().server_64);
	character_proto_spirit_info* spirit_info_ptr = spirit_msg.add_spirit_list();
	spirit_info_ptr->set_spirit_guid(temp_item->get_item_guid().server_64);
	//s_spirit_info temp_spirit_info = temp_player_ref.get_spirit_mgr().make_fake_spirit_info(temp_item, is_del);
	//int32 tem_info = 0;
	//for (int32 i = 0; i < e_spirit_info_max; ++i)
	//{
	//	tem_info = temp_spirit_info.data_ary[(e_spirit_info)i];
	//	spirit_info_ptr->add_spirit_data(tem_info);
	//}
	temp_player_ref.send_message_to_aoi(&spirit_msg, e_msgindex_s2c_spirit_update);
}

bool item_set::put_in_bag(e_server_log_add_item add_type, int32 param, const std::vector<int32>& item_data_array, e_bag_type bag_type, e_item_transform_type item_transform_type, e_item_mail_type mail_type, int32 notice_id)
{
	std::vector<s_item_template_info> item_array = get_item_template_info_array(item_data_array, item_transform_type);
	player& player_ref = unit_man::get_player(m_unit_array_index);
	player_ref.send_promp_msg_to_client(item_array);
	return put_in_bag(add_type, param, item_array, bag_type, mail_type, notice_id);
}
bool item_set::put_in_bag(e_server_log_add_item add_type, int32 param, const std::vector<s_item_template_info>& item_array, e_bag_type bag_type, e_item_mail_type mail_type, int32 notice_id)
{
	if (item_array.empty())
	{
		CONSOLE_ERROR("item_array is empty");
		return false;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		CONSOLE_ERROR("player is not valid m_unit_array_index:{}", m_unit_array_index);
		return false;
	}
	bool b_can_put_item_into_bag = can_put_items(item_array);

	if (true == b_can_put_item_into_bag || mail_type > 0)
	{
		std::string item_names_str;
		item_names_str.reserve(100);
		s_item_info item_data[chat_max_item];
		int32 item_data_size = 0;
		std::vector<xstring> item_name_array;
		std::vector<citem*> item_ptr_array = create_items(add_type, param, item_array);
		if (item_data_size < chat_max_item)
		{
			item_data[item_data_size] = filter_items_with_notice(item_ptr_array, item_names_str);
			if (item_data[item_data_size].item_guid.is_valid())
			{
				item_data_size++;
			}
		}
		if (0 < notice_id && 0 < item_data_size)
		{
			std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
			std::vector<std::string> notice_str_params_vec;
			notice_str_params_vec.push_back(notice_str_id);
			notice_str_params_vec.push_back(player_ref.get_name());
			notice_str_params_vec.push_back(item_names_str);
			std::string notice_str = init_unit::implode(notice_str_params_vec);
			player_ref.get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_data_size - 1);
		}

		if (false == b_can_put_item_into_bag)
		{
			std::string show_string = template_manager::get_instance().get_str_id_by_notice_id(mail_with_item_notice_id);
			player_ref.get_chat_mgr().send_notice(mail_with_item_notice_id, show_string);
		}

		put_in_bag(item_ptr_array, bag_type, mail_type);
		return true;
	}
	else
	{
		std::string show_string = template_manager::get_instance().get_str_id_by_notice_id(bag_is_full_notice_id);
		player_ref.get_chat_mgr().send_notice(bag_is_full_notice_id, show_string);
	}
	return false;
}

bool item_set::can_put_items(const std::vector<s_item_template_info>& item_array, e_bag_type bag_type)
{
	int32 len = item_array.size();
	std::map <e_bag_type, std::vector<s_item_template_info>> item_array_map;
	for (int32 i = 0; i < len; ++i)
	{
		s_item_template_info template_info = item_array[i];
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, template_info.m_item_id);
		if (nullptr == item_template_ptr)
		{
			continue;
		}
		if (item_template_ptr->item_type == e_item_type_time_limit)
		{
			template_info.m_item_id = item_template_ptr->logic_id;
		}
		e_bag_type item_bag_type = bag_type == e_bag_type_none ? init_unit::get_item_bag_type(template_info.m_item_id) : bag_type;
		if (item_bag_type <= e_bag_type_none || item_bag_type >= e_bag_type_max)
		{
			return false;
		}
		item_array_map[item_bag_type].push_back(template_info);
	}
	std::map <e_bag_type, std::vector<s_item_template_info>>::iterator it = item_array_map.begin();
	for (; it != item_array_map.end(); ++it)
	{
		if (can_put_items_into_bag(it->first, it->second) == false)
		{
			return false;
		}
	}
	return true;
}
bool item_set::can_put_items_into_bag(e_bag_type bag_type, const std::vector<s_item_template_info>& item_array)
{
	if (bag_type <= 0 || bag_type >= e_bag_type_max)
	{
		return false;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return false;
	}
	std::map<int32, int32> bag_item_num;
	auto& guid_array = GET_BAG(bag_type);

	for (auto& it : guid_array)
	{
		citem* item_ptr = get_item(it.second);
		if (item_ptr)
		{
			int32 template_id = item_ptr->get_data_info(e_item_info_info_id) + item_ptr->get_data_info(e_item_info_locked) * 100000000;
			bag_item_num[template_id] += item_ptr->get_data_info(e_item_info_stack_count);
		}
	}
	int32 len = item_array.size();
	for (int32 i = 0; i < len; ++i)
	{
		const s_item_template_info& item_info = item_array[i];
		int32 template_id = item_info.m_item_id + item_info.m_lock * 100000000;
		bag_item_num[template_id] += item_info.m_item_num;
	}
	int32 need_slot = 0;
	for (auto it = bag_item_num.begin(); it != bag_item_num.end(); ++it)
	{
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, it->first % 100000000);
		if (item_template_ptr)
		{
			if (it->second % item_template_ptr->max_pile_num > 0)
			{
				need_slot += it->second / item_template_ptr->max_pile_num + 1;
			}
			else
			{
				need_slot += it->second / item_template_ptr->max_pile_num;
			}
		}
	}
	switch (bag_type)
	{
	case faith::e_bag_type_bag:
		if (need_slot > player_ref.get_logic_data(e_role_logic_info_bag_open))
		{
			return false;
		}
		break;
	case faith::e_bag_type_storage:
		if (need_slot > player_ref.get_logic_data(e_role_logic_info_storage_open))
		{
			return false;
		}
		break;
	default:
		if (need_slot > m_item_bag_length[bag_type])
		{
			return false;
		}
		break;
	}
	return true;
}
bool item_set::can_put_items(const std::vector<citem*>& item_array, e_bag_type bag_type)
{
	int32 len = item_array.size();
	std::vector<s_item_template_info> item_data_array;
	for (int32 i = 0; i < len; ++i)
	{
		citem* item_ptr = item_array[i];
		if (item_ptr)
		{
			item_data_array.push_back(s_item_template_info(item_ptr->get_data_info(e_item_info_info_id), item_ptr->get_data_info(e_item_info_stack_count), item_ptr->get_data_info(e_item_info_locked)));
		}
	}
	return can_put_items(item_data_array, bag_type);
}

std::vector<citem*> item_set::create_items(e_server_log_add_item add_type, int32 param, const std::vector<s_item_template_info>& item_template_info_array)
{
	std::vector<citem*> items_array;
	const int32 item_template_info_array_size = item_template_info_array.size();
	for (int32 item_array_index = 0; item_array_index < item_template_info_array_size; item_array_index++)
	{
		s_item_template_info item_template_info = item_template_info_array[item_array_index];
		const int32 item_template_id = item_template_info.m_item_id;
		const int32 item_num = item_template_info.m_item_num;
		const int32 item_is_lock = item_template_info.m_lock;
		const int32 item_const_att = 0;
		citem* new_item_ptr = create_item_by_template(add_type, param, item_template_id, item_num, item_is_lock, 0, e_bag_type_none, item_const_att);
		if (nullptr == new_item_ptr)
		{
			continue;
		}
		items_array.push_back(new_item_ptr);
	}
	return items_array;
}

std::vector<citem*> item_set::get_items_by_template_id(int32 item_template_id, int32 locked, bool only_bag)
{
	std::vector<citem*> item_array;

	for (auto ite : m_item_map)
	{
		citem* item = ite.second;
		if (nullptr != item && item->get_item_info_ptr()->attribute_id == item_template_id)
		{
			if (only_bag && item->get_data_info(e_item_info_container_type) != e_bag_type_bag)
			{
				continue;
			}
			if (e_item_lock_status_ignore != locked && item->get_data_info(e_item_info_locked) != locked)
			{
				continue;
			}
			item_array.push_back(item);
		}
	}

	return item_array;
}

int32 item_set::del_item_instance(e_server_log_del_item causeid, int32 location, const guid_64& item_guid, int32 del_num)
{
	auto item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_ptr is nullptr item_guid:{}", item_guid.server_64);
		return 0;
	}
	return del_item_instance(causeid, location, item_ptr, del_num);
}
int32 item_set::del_item_instance(e_server_log_del_item causeid, int32 location, citem* item_ptr, int32 del_num)
{
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_ptr is nullptr");
		return 0;
	}
	if (item_ptr->get_item_info_ptr() == nullptr)
	{
		return 0;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return 0;
	}
	int32 item_container = item_ptr->get_data_info(e_item_info_container_type);
	int32 item_slot = item_ptr->get_data_info(e_item_info_slot);
	int32 item_cur_stack_count = item_ptr->get_data_info(e_item_info_stack_count);
	int32 item_id = item_ptr->get_data_info(e_item_info_info_id);
	guid_64 item_guid = item_ptr->get_item_guid();
	if (del_num <= 0)
	{
		del_num = item_cur_stack_count;
	}
	check_target_mission(item_ptr->get_item_template_id(), del_num, causeid);
	int32 left_stack_count = item_cur_stack_count - del_num;
	if (left_stack_count < 0)
	{
		left_stack_count = 0;
	}
	item_ptr->set_data_info(e_item_info_stack_count, left_stack_count);

	if (left_stack_count <= 0)
	{
		if (item_ptr->get_item_info_ptr()->item_type == e_item_type_spirit && causeid != e_server_log_del_item_equip_on_spirit)
		{
			for (int32 i = e_item_info_succinct_property2; i < e_item_info_succinct_property4; ++i)
			{
				int32 cur_passive_id = item_ptr->get_data_info(e_item_info(i));
				if (cur_passive_id > 0)
				{
					player_ref.get_passive_skill().passive_skill_special(cur_passive_id, false);
				}
			}
		}

		if (item_container == e_bag_type_wing)
		{
			set_wing_att(false);
			reset_wings_order();
		}
		clear_target_slot((e_bag_type)item_container, item_slot);
		m_item_map.erase(item_ptr->get_item_guid().server_64);
		cache_mgr::get_instance().back_item_ptr(item_ptr);

		if (item_container == e_bag_type_wing)
		{
			set_wing_att(true);
			send_wing_item_all();
		}
	}
	server_log::item_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), -1, item_id, del_num, causeid, location, item_guid, left_stack_count, player_ref.get_login_type());

	set_log_var(log_head);

	player_ref.get_log_common_head_info(log_head);

	server_log::serverPropChange(log_head, init_unit::change_i32_to_string(item_id), 0, causeid, del_num, left_stack_count, location);

	return left_stack_count;
}

int32 item_set::get_item_count(e_bag_type bag_type, int32 item_template_id, int32 lock)
{
	ZoneScoped;
	if (bag_type <= 0 || bag_type >= e_bag_type_max)
	{
		CONSOLE_ERROR("bag_type is error bag_type:{}", (int32)bag_type);
		return 0;
	}
	auto& guid_array = GET_BAG(bag_type);
	int32 template_item_sum = 0;

	for (auto& it : guid_array)
	{
		citem* item_ptr = get_item(it.second);
		if (nullptr == item_ptr)
		{
			continue;
		}
		if (item_ptr->get_data_info(e_item_info_info_id) == item_template_id)
		{
			if (lock == -1 || item_ptr->get_data_info(e_item_info_locked) == lock)
			{
				template_item_sum += item_ptr->get_data_info(e_item_info_stack_count);
			}
		}
	}
	return template_item_sum;
}

void item_set::clear_bag(e_bag_type bag_type)
{
	if (bag_type <= e_bag_type_none || bag_type >= e_bag_type_max)
	{
		return;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}

	item_proto_item_delete item_delete_msg;
	item_delete_msg.add_role_guid(player_ref.get_unit_guid().A);
	item_delete_msg.add_role_guid(player_ref.get_unit_guid().B);
	item_delete_msg.set_del_item_arr_len(0);

	auto guid_array = GET_BAG(bag_type);

	int32 del_item_num = 0;
	for (auto& it : guid_array)
	{
		citem* item_ptr = get_item(it.second);
		if (nullptr == item_ptr)
		{
			continue;
		}

		if (del_item_instance(e_server_log_del_item_gm_order, 0, item_ptr) <= 0)
		{
			item_delete_msg.add_del_item_guid_arr(it.second.A);
			item_delete_msg.add_del_item_guid_arr(it.second.B);
			++del_item_num;
		}
	}
	m_item_bag_array[bag_type].clear();
	item_delete_msg.set_del_item_arr_len(del_item_num);
	player_ref.send_message_to_self(&item_delete_msg, e_msgindex_s2c_item_delete);
}

void item_set::del_item(e_server_log_del_item causeid, int32 location, e_bag_type bag_type, int32 item_template_id, int32 del_num)
{
	ZoneScoped;
	std::vector<s_del_item_info> del_info;
	del_info.push_back(s_del_item_info(item_template_id, del_num));
	del_item(causeid, location, bag_type, del_info);
}
void item_set::del_item(e_server_log_del_item causeid, int32 location, e_bag_type bag_type, const std::vector<s_del_item_info>& del_info_array)
{
	ZoneScoped;
	if (bag_type <= 0 || bag_type >= e_bag_type_max)
	{
		CONSOLE_ERROR("bag_type is error bag_type:{}", (int32)bag_type);
		return;
	}
	if (del_info_array.empty())
	{
		CONSOLE_ERROR("del_info_array is empty");
		return;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player_ref is null m_unit_array_index:{}", m_unit_array_index);
		return;
	}
	std::vector<guid_64> del_guid;
	for (auto& it : del_info_array)
	{
		int32 del_item_id = it.m_item_id;
		int32 del_num = it.m_del_num;
		if (del_num <= 0)
		{
			del_num = get_item_count(bag_type, del_item_id);
		}
		auto bag_array = GET_BAG(bag_type);
		for (auto& it : bag_array)
		{
			if (del_num <= 0)
			{
				break;
			}
			citem* item_ptr = get_item(it.second);
			if (nullptr == item_ptr)
			{
				clear_target_slot(bag_type, it.first);
				continue;
			}
			if (item_ptr->get_item_template_id() == del_item_id)
			{
				auto item_count = item_ptr->get_data_info(e_item_info_stack_count);
				if (item_count <= del_num)
				{
					del_num -= item_count;
					if (del_item_instance(causeid, location, item_ptr) <= 0)
					{
						del_guid.push_back(it.second);
					}
				}
				else
				{
					del_item_instance(causeid, location, item_ptr, del_num);
					send_item_one(item_ptr);
					break;
				}
			}
		}
	}
	if (del_guid.empty() == false)
	{
		item_proto_item_delete item_delete_msg;
		item_delete_msg.add_role_guid(player_ref.get_unit_guid().A);
		item_delete_msg.add_role_guid(player_ref.get_unit_guid().B);
		item_delete_msg.set_del_item_arr_len(del_guid.size());
		for (auto& it : del_guid)
		{
			item_delete_msg.add_del_item_guid_arr(it.A);
			item_delete_msg.add_del_item_guid_arr(it.B);
		}
		player_ref.send_message_to_self(&item_delete_msg, e_msgindex_s2c_item_delete);
	}
}

void item_set::del_item(e_server_log_del_item causeid, int32 location, const guid_64& item_guid, int32 del_num)
{
	ZoneScoped;
	std::vector<s_del_item_guid> del_array;
	del_array.push_back(s_del_item_guid(item_guid, del_num));
	del_item(causeid, location, del_array);
}
void item_set::del_item(e_server_log_del_item causeid, int32 location, const std::vector<s_del_item_guid>& del_guid_array)
{
	ZoneScoped;
	int32 len = del_guid_array.size();
	if (len <= 0)
	{
		CONSOLE_ERROR("del_guid_array is empty");
		return;
	}

	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player_ref is nullptr");
		return;
	}
	std::vector<guid_64> del_guid;
	for (auto& it : del_guid_array)
	{
		citem* item_ptr = get_item(it.m_item_guid);
		if (nullptr == item_ptr)
		{
			del_guid.push_back(it.m_item_guid);
			continue;
		}
		if (del_item_instance(causeid, location, item_ptr, it.m_del_num) <= 0)
		{
			del_guid.push_back(it.m_item_guid);
		}
		else
		{
			send_item_one(item_ptr);
		}
	}
	if (del_guid.empty() == false)
	{
		item_proto_item_delete item_delete_msg;
		item_delete_msg.add_role_guid(player_ref.get_unit_guid().A);
		item_delete_msg.add_role_guid(player_ref.get_unit_guid().B);
		item_delete_msg.set_del_item_arr_len(del_guid.size());
		for (auto& guid_it : del_guid)
		{
			item_delete_msg.add_del_item_guid_arr(guid_it.A);
			item_delete_msg.add_del_item_guid_arr(guid_it.B);
		}
		player_ref.send_message_to_self(&item_delete_msg, e_msgindex_s2c_item_delete);
	}
}
void item_set::del_item(e_server_log_del_item causeid, int32 location, citem* item_ptr, int32 del_num)
{
	ZoneScoped;
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_ptr is nullptr");
		return;
	}
	del_item(causeid, location, item_ptr->get_item_guid(), del_num);
}

bool item_set::tidy_item(e_bag_type tidy_bag_typ, bool is_not_tidy_time)
{
	unit& unit_ref = unit_man::get_unit(m_unit_array_index);
	if (unit_ref.is_valid() == false)
	{
		return false;
	}
	if (m_item_tidy_cd > utility::get_tick_count())
	{
		if (is_not_tidy_time)
		{
			add_and_del_tidy_type(tidy_bag_typ, true);
		}
		return false;
	}
	m_item_tidy_cd = init_unit::get_end_time(item_tidy_interval);

	item_merge(tidy_bag_typ);

	auto& guid_array = GET_BAG(tidy_bag_typ);
	std::vector<guid_64> tidy_guid_array;
	for (auto& it : guid_array)
	{
		tidy_guid_array.push_back(it.second);
	}
	int32 tidy_guid_array_size = tidy_guid_array.size();
	for (int32 i = 0; i < tidy_guid_array_size; ++i)
	{
		guid_64& item_guid = tidy_guid_array[i];

		citem* item_ptr = unit_ref.get_item_set().get_item(item_guid);
		ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();

		for (int32 j = i + 1; j < tidy_guid_array_size; ++j)
		{
			guid_64& next_item_guid = tidy_guid_array[j];
			citem* next_item_ptr = unit_ref.get_item_set().get_item(next_item_guid);
			ItemTemplate* next_item_template_ptr = next_item_ptr->get_item_info_ptr();
			if (item_template_ptr->ExtraTidyWeight < next_item_template_ptr->ExtraTidyWeight || (item_template_ptr->ExtraTidyWeight == next_item_template_ptr->ExtraTidyWeight && item_template_ptr->attribute_id < next_item_template_ptr->attribute_id))
			{
				guid_64 temp_guid = item_guid;
				item_guid = next_item_guid;
				next_item_guid = temp_guid;
				item_ptr = next_item_ptr;
				item_template_ptr = next_item_template_ptr;
			}
		}
	}
	guid_array.clear();
	for (int32 i = 0; i < tidy_guid_array_size; ++i)
	{
		guid_64& item_guid = tidy_guid_array[i];
		citem* item_ptr = unit_ref.get_item_set().get_item(item_guid);
		item_ptr->set_data_info(e_item_info_slot, i);
		guid_array[i] = item_guid;
	}
	unit_ref.get_item_set().send_item_tidy_guid_array(guid_array);
	return true;
}

void item_set::refresh_element_heart_faker_player_buff()
{
	npc& temp_npc = unit_man::get_npc(m_unit_array_index);
	if (temp_npc.is_valid() == false || !init_unit::is_fake_player(temp_npc.get_unit_type(), temp_npc.get_unit_sub_type()))
	{
		return;
	}

	if (GAMECONFIG->RuneBuffLevelArray.empty())
	{
		return;
	}

	int32 total_rune_level = 0;
	for (int32 i = 0; i < element_heart_equip_num; ++i)
	{
		citem* temp_rune = get_item_by_slot(e_bag_type_equip_elemenet, i);
		if (temp_rune == nullptr)
		{
			continue;
		}
		total_rune_level += temp_rune->get_data_info(e_item_info_upgrade_count) + 1;
	}
	int32 buff_phase = -1;

	for (int32 i = 0; i < GAMECONFIG->RuneBuffLevelArray.size(); ++i)
	{
		if (total_rune_level >= GAMECONFIG->RuneBuffLevelArray[i])
		{
			buff_phase = i;
		}
		else
		{
			break;
		}
	}
	if (buff_phase < 0)
	{
		return;
	}
	for (int32 i = 0; i < element_heart_equip_num; ++i)
	{
		citem* temp_equip_rune = get_item_by_slot(e_bag_type_equip_elemenet, i);
		if (temp_equip_rune == nullptr)
		{
			continue;
		}
		ElementHeartTemplate* temp_template_ptr = GET_TEMPLATE(ElementHeartTemplate, temp_equip_rune->get_item_logic_id());
		if (temp_template_ptr == nullptr
			|| buff_phase >= temp_template_ptr->BuffIdArray.size())
		{
			continue;
		}

		int32 temp_buff_id = temp_template_ptr->BuffIdArray[buff_phase];
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, temp_buff_id, nullptr, false, i);
	}

}

int32 item_set::init_fake_player_item_info(s_item_info* item_info_list, int32 max_num)
{
	int32 item_index = 0;
	for (auto ite = m_item_map.begin(); ite != m_item_map.end(); ++ite)
	{
		citem* item_ptr = ite->second;
		if (nullptr == item_ptr->get_item_info_ptr())
		{
			continue;
		}
		if (item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_equip && item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_equip_fasion && item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_wedding_equip)
		{
			continue;
		}
		if (item_index >= max_num)
		{
			break;
		}
		item_info_list[item_index] = item_ptr->get_item_inst();
		item_index++;
	}
	return item_index;
}

void item_set::calcu_addition_with_fake_player(e_addition_buff addition_buff_type)
{
	npc& temp_npc = unit_man::get_npc(m_unit_array_index);
	if (temp_npc.is_valid() == false || !init_unit::is_fake_player(temp_npc.get_unit_type(), temp_npc.get_unit_sub_type()))
	{
		return;
	}
	faith::template_manager::template_type* addition_buff_tmpl_table = template_manager::get_instance().get_templates(e_AdditionBuffTemplate);
	if (nullptr == addition_buff_tmpl_table)
	{
		return;
	}

	faith::template_manager::template_type::iterator ite;
	AdditionBuffTemplate* addition_tmpl_ptr = nullptr;
	std::vector<int32> new_buff_id_arr;
	std::vector<int32> new_buff_equip_num_arr;
	std::vector<int32> new_buff_need_num_arr;
	std::vector<int32> new_buff_sub_type_arr;
	std::vector<int32> new_buff_level_arr;

	for (ite = addition_buff_tmpl_table->begin(); ite != addition_buff_tmpl_table->end(); ++ite)
	{
		addition_tmpl_ptr = (AdditionBuffTemplate*)(ite->second);
		if (nullptr == addition_tmpl_ptr || addition_tmpl_ptr->AdditionBuffType != addition_buff_type + 1)
		{
			continue;
		}
		calcu_addition_by_one(addition_tmpl_ptr, new_buff_id_arr, new_buff_equip_num_arr, new_buff_need_num_arr, new_buff_sub_type_arr, new_buff_level_arr);
	}
	for (int32 i = 0; i < new_buff_id_arr.size(); i++)
	{
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, new_buff_id_arr[i], nullptr, true);
	}
}

void item_set::calcu_addition(e_addition_buff addition_buff_type, bool need_send_notice)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	std::vector<int32> old_buff_id_arr = temp_player.get_addition_buff_id_arr(addition_buff_type);

	faith::template_manager::template_type* addition_buff_tmpl_table = template_manager::get_instance().get_templates(e_AdditionBuffTemplate);
	if (nullptr == addition_buff_tmpl_table)
	{
		return;
	}

	faith::template_manager::template_type::iterator ite;
	AdditionBuffTemplate* addition_tmpl_ptr = nullptr;
	std::vector<int32> new_buff_id_arr;
	std::vector<int32> new_buff_equip_num_arr;
	std::vector<int32> new_buff_need_num_arr;
	std::vector<int32> new_buff_sub_type_arr;
	std::vector<int32> new_buff_level_arr;
	for (ite = addition_buff_tmpl_table->begin(); ite != addition_buff_tmpl_table->end(); ++ite)
	{
		addition_tmpl_ptr = (AdditionBuffTemplate*)(ite->second);
		if (nullptr == addition_tmpl_ptr || addition_tmpl_ptr->AdditionBuffType != addition_buff_type + 1)
		{
			continue;
		}
		calcu_addition_by_one(addition_tmpl_ptr, new_buff_id_arr, new_buff_equip_num_arr, new_buff_need_num_arr, new_buff_sub_type_arr, new_buff_level_arr);
	}

	bool buff_is_change = false;
	if (old_buff_id_arr.size() != new_buff_id_arr.size())
	{
		buff_is_change = true;
	}
	else
	{
		for (int32 i = 0; i < old_buff_id_arr.size(); i++)
		{
			if (old_buff_id_arr[i] != new_buff_id_arr[i])
			{
				buff_is_change = true;
				break;
			}
		}
	}
	if (buff_is_change == false)
	{
		return;
	}
	temp_player.reset_addition_buff_id_by_type(addition_buff_type);

	int32 old_addtion_level = 0;
	for (int32 i = 0; i < old_buff_id_arr.size(); i++)
	{
		if (old_buff_id_arr[i] > 0)
		{
			AdditionBuffTemplate* cur_addition_tmpl_ptr = GET_TEMPLATE(AdditionBuffTemplate, old_buff_id_arr[i]);
			if (cur_addition_tmpl_ptr != nullptr)
			{
				old_addtion_level = cur_addition_tmpl_ptr->AdditionBuffLevel;
				item_change_att(cur_addition_tmpl_ptr->AdditionValue, 1, false);
				for (int32 j = 0; j < cur_addition_tmpl_ptr->BuffArray.size(); j++)
				{
					buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, cur_addition_tmpl_ptr->BuffArray[j]);
				}
				if (cur_addition_tmpl_ptr->SuitSkillID > 0)
				{
					temp_player.get_passive_skill().passive_skill_special(cur_addition_tmpl_ptr->SuitSkillID, false);
				}
			}
		}
	}

	for (int32 i = 0; i < new_buff_id_arr.size(); i++)
	{
		int32 new_addtion_level = 0;
		if (new_buff_id_arr[i] > 0)
		{
			AdditionBuffTemplate* cur_addition_tmpl_ptr = GET_TEMPLATE(AdditionBuffTemplate, new_buff_id_arr[i]);
			if (cur_addition_tmpl_ptr)
			{
				new_addtion_level = cur_addition_tmpl_ptr->AdditionBuffLevel;
				temp_player.add_addition_buff_id_arr(addition_buff_type, new_buff_id_arr[i]);
				item_change_att(cur_addition_tmpl_ptr->AdditionValue, 1, true);
				for (int32 j = 0; j < cur_addition_tmpl_ptr->BuffArray.size(); j++)
				{
					buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, cur_addition_tmpl_ptr->BuffArray[j]);
				}
				if (cur_addition_tmpl_ptr->SuitSkillID > 0)
				{
					temp_player.get_passive_skill().passive_skill_special(cur_addition_tmpl_ptr->SuitSkillID, true);
				}

				temp_player.send_addition_buff_info(temp_player.get_unit_guid(), addition_buff_type, new_buff_id_arr[i]);
				if (true == need_send_notice && new_addtion_level > old_addtion_level)
				{
					temp_player.send_addition_notice_with_type(addition_buff_type, new_buff_id_arr[i]);
				}
			}
		}
		else
		{
			temp_player.add_addition_buff_id_arr(addition_buff_type, 0);
			temp_player.send_addition_buff_info(temp_player.get_unit_guid(), addition_buff_type, 0);
		}
	}

	if (addition_buff_type == faith::e_addition_buff_enchant)
	{
		std::vector<int32> buff_id_arr = temp_player.get_addition_buff_id_arr(faith::e_addition_buff_enchant);
		temp_player.send_addition_buff_info_arr(temp_player.get_unit_guid(), faith::e_addition_buff_enchant, buff_id_arr);
	}
}

void item_set::calcu_addition_by_one(AdditionBuffTemplate* addition_tmpl_ptr, std::vector<int32>& buff_id_arr, std::vector<int32>& buff_equip_num_arr, std::vector<int32>& buff_need_num_arr, std::vector<int32>& buff_sub_type_arr, std::vector<int32>& buff_level_arr)
{
	if (buff_id_arr.size() != buff_equip_num_arr.size() || buff_id_arr.size() != buff_need_num_arr.size() || buff_id_arr.size() != buff_sub_type_arr.size() || buff_id_arr.size() != buff_level_arr.size())
	{
		return;
	}

	switch (addition_tmpl_ptr->AdditionBuffType - 1)
	{
	case e_addition_buff_upgrade:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_magic_2; ++i)
			{
				if (GET_BAG(e_bag_type_equip)[i].is_valid())
				{
					citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
					if (item_ptr)
					{
						if (item_ptr->get_data_info(e_item_info_upgrade_count) >= addition_tmpl_ptr->UpgradeNum)
						{
							if (true == is_equipment_effective(item_ptr->get_item_guid()))
							{
								++equip_num;
							}
						}
					}
				}
			}

			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (buff_id_arr.size() <= 0)
				{
					buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
					buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
					buff_need_num_arr.push_back(addition_tmpl_ptr->UpgradeNum);
					buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
					buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
				}
				else
				{
					if (addition_tmpl_ptr->EquipNum > buff_equip_num_arr[0] || addition_tmpl_ptr->UpgradeNum > buff_need_num_arr[0])
					{
						buff_id_arr[0] = addition_tmpl_ptr->attribute_id;
						buff_equip_num_arr[0] = addition_tmpl_ptr->EquipNum;
						buff_need_num_arr[0] = addition_tmpl_ptr->UpgradeNum;
						buff_sub_type_arr[0] = addition_tmpl_ptr->AdditionBuffSubType;
						buff_level_arr[0] = addition_tmpl_ptr->AdditionBuffLevel;
					}
				}
			}
		}
	}
	break;
	case e_addition_buff_quality:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = 0; i < faith::e_role_equip_slot_amulet; ++i)
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
				if (item_ptr)
				{
					if (item_ptr->get_item_info_ptr()->item_color >= addition_tmpl_ptr->EquipQuality)
					{
						if (true == is_equipment_effective(item_ptr->get_item_guid()))
						{
							++equip_num;
						}
					}
				}
			}

			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (buff_id_arr.size() <= 0)
				{
					buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
					buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
					buff_need_num_arr.push_back(addition_tmpl_ptr->EquipQuality);
					buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
					buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
				}
				else
				{
					if (addition_tmpl_ptr->EquipNum > buff_equip_num_arr[0] || addition_tmpl_ptr->EquipQuality > buff_need_num_arr[0])
					{
						buff_id_arr[0] = addition_tmpl_ptr->attribute_id;
						buff_equip_num_arr[0] = addition_tmpl_ptr->EquipNum;
						buff_need_num_arr[0] = addition_tmpl_ptr->EquipQuality;
						buff_sub_type_arr[0] = addition_tmpl_ptr->AdditionBuffSubType;
						buff_level_arr[0] = addition_tmpl_ptr->AdditionBuffLevel;
					}
				}
			}
		}
	}
	break;
	case e_addition_buff_feather:
	{
		if (addition_tmpl_ptr != nullptr && addition_tmpl_ptr->AdditionBuffType == e_addition_buff_feather + 1)
		{
			int32 feather_num = 0;
			for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; ++i)
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_wing)[i]);
				if (item_ptr)
				{
					ItemTemplate* temp_template = item_ptr->get_item_info_ptr();
					if (nullptr == temp_template)
					{
						return;
					}

					if (temp_template->item_type == e_item_type_wing && temp_template->sub_type == e_wing_sub_type_feather)
					{
						int32 now_feather_count = item_ptr->get_data_info(e_item_info_upgrade_count);
						int32 real_id = item_ptr->get_item_info_ptr()->logic_id + now_feather_count;
						FeatherTemplate* feather_upgrade_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
						if (nullptr == feather_upgrade_ptr)
						{
							return;
						}
						if (feather_upgrade_ptr->UpgradeNow >= addition_tmpl_ptr->FeatherUpgradeNum)
						{
							feather_num++;
						}
					}
				}
			}

			if (feather_num >= addition_tmpl_ptr->FeatherNum)
			{
				if (buff_id_arr.size() <= 0)
				{
					buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
					buff_equip_num_arr.push_back(addition_tmpl_ptr->FeatherNum);
					buff_need_num_arr.push_back(addition_tmpl_ptr->FeatherUpgradeNum);
					buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
					buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
				}
				else
				{
					if (addition_tmpl_ptr->FeatherNum > buff_equip_num_arr[0] || addition_tmpl_ptr->FeatherUpgradeNum > buff_need_num_arr[0])
					{
						buff_id_arr[0] = addition_tmpl_ptr->attribute_id;
						buff_equip_num_arr[0] = addition_tmpl_ptr->FeatherNum;
						buff_need_num_arr[0] = addition_tmpl_ptr->FeatherUpgradeNum;
						buff_sub_type_arr[0] = addition_tmpl_ptr->AdditionBuffSubType;
						buff_level_arr[0] = addition_tmpl_ptr->AdditionBuffLevel;
					}
				}
			}
		}
	}
	break;
	case e_addition_buff_addon:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_magic_2; ++i)
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
				if (item_ptr)
				{
					if (item_ptr->get_data_info(e_item_info_add_on) >= addition_tmpl_ptr->AddOnLevel)
					{
						if (true == is_equipment_effective(item_ptr->get_item_guid().server_64))
						{
							++equip_num;
						}
					}
				}
			}

			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (buff_id_arr.size() <= 0)
				{
					buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
					buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
					buff_need_num_arr.push_back(addition_tmpl_ptr->AddOnLevel);
					buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
					buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
				}
				else
				{
					if (addition_tmpl_ptr->EquipNum > buff_equip_num_arr[0] || addition_tmpl_ptr->AddOnLevel > buff_need_num_arr[0])
					{
						buff_id_arr[0] = addition_tmpl_ptr->attribute_id;
						buff_equip_num_arr[0] = addition_tmpl_ptr->EquipNum;
						buff_need_num_arr[0] = addition_tmpl_ptr->AddOnLevel;
						buff_sub_type_arr[0] = addition_tmpl_ptr->AdditionBuffSubType;
						buff_level_arr[0] = addition_tmpl_ptr->AdditionBuffLevel;
					}
				}
			}
		}
	}
	break;
	case e_addition_buff_succinct:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_magic_2; ++i)
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
				if (item_ptr)
				{
					if (item_ptr->get_item_succinct_current_buff_level() >= addition_tmpl_ptr->SuccinctFullQualityLevel)
					{
						if (true == is_equipment_effective(item_ptr->get_item_guid()))
						{
							++equip_num;
						}
					}
				}
			}

			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (buff_id_arr.size() <= 0)
				{
					buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
					buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
					buff_need_num_arr.push_back(addition_tmpl_ptr->SuccinctFullQualityLevel);
					buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
					buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
				}
				else
				{
					if (addition_tmpl_ptr->EquipNum > buff_equip_num_arr[0] || addition_tmpl_ptr->SuccinctFullQualityLevel > buff_need_num_arr[0])
					{
						buff_id_arr[0] = addition_tmpl_ptr->attribute_id;
						buff_equip_num_arr[0] = addition_tmpl_ptr->EquipNum;
						buff_need_num_arr[0] = addition_tmpl_ptr->SuccinctFullQualityLevel;
						buff_sub_type_arr[0] = addition_tmpl_ptr->AdditionBuffSubType;
						buff_level_arr[0] = addition_tmpl_ptr->AdditionBuffLevel;
					}
				}
			}
		}
	}
	break;
	case e_addition_buff_sky_equip:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = 0; i < sky_equip_slot_num; i++)
			{
				citem* temp_item = get_item(GET_BAG(e_bag_type_sky_equip)[i]);
				if (temp_item)
				{
					equip_num++;
				}
			}
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
				buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
				buff_need_num_arr.push_back(addition_tmpl_ptr->UpgradeNum);
				buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
				buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
			}
		}
	}
	break;
	case e_addition_buff_skygod_equip:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = 0; i < skygod_equip_slot_num; i++)
			{
				citem* temp_item = get_item(GET_BAG(e_bag_type_skygod_equip)[i]);
				if (temp_item)
				{
					equip_num++;
				}
			}
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
				buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
				buff_need_num_arr.push_back(addition_tmpl_ptr->UpgradeNum);
				buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
				buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
			}
		}
	}
	break;
	case e_addition_buff_supreme_equip:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = 0; i < supreme_equip_slot_num; i++)
			{
				citem* temp_item = get_item(GET_BAG(e_bag_type_supreme_equip)[i]);
				if (temp_item)
				{
					equip_num++;
				}
			}
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
				buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
				buff_need_num_arr.push_back(addition_tmpl_ptr->UpgradeNum);
				buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
				buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
			}
		}
	}
	break;
	case e_addition_buff_enchant:
	{
		if (addition_tmpl_ptr != nullptr)
		{
			int32 equip_num = 0;
			for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_magic_2; ++i)
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
				if (item_ptr)
				{
					int32 enchant_level = item_ptr->get_data_info(e_item_info_illusion_had_byte) / faith::max_enchant_type_num;
					int32 enchant_type = item_ptr->get_data_info(e_item_info_illusion_had_byte) % faith::max_enchant_type_num;

					if (enchant_level >= addition_tmpl_ptr->AdditionBuffLevel && enchant_type == addition_tmpl_ptr->AdditionBuffSubType)
					{
						if (true == is_equipment_effective(item_ptr->get_item_guid().server_64))
						{
							++equip_num;
						}
					}
				}
			}

			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				int32 need_replace_index = -1;
				bool need_replace = false;
				for (int32 i = 0; i < buff_id_arr.size(); i++)
				{
					if (buff_equip_num_arr[i] == addition_tmpl_ptr->EquipNum && buff_sub_type_arr[i] == addition_tmpl_ptr->AdditionBuffSubType)
					{
						if (buff_level_arr[i] < addition_tmpl_ptr->AdditionBuffLevel)
						{
							need_replace = true;
							need_replace_index = i;
						}
						break;
					}
				}
				if (need_replace == true && need_replace_index >= 0 && need_replace_index < buff_id_arr.size())
				{
					buff_id_arr[need_replace_index] = addition_tmpl_ptr->attribute_id;
					buff_equip_num_arr[need_replace_index] = addition_tmpl_ptr->EquipNum;
					buff_need_num_arr[need_replace_index] = addition_tmpl_ptr->UpgradeNum;
					buff_sub_type_arr[need_replace_index] = addition_tmpl_ptr->AdditionBuffSubType;
					buff_level_arr[need_replace_index] = addition_tmpl_ptr->AdditionBuffLevel;
				}
				else
				{
					buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
					buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
					buff_need_num_arr.push_back(addition_tmpl_ptr->UpgradeNum);
					buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
					buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
				}
			}
		}
	}
	break;
	case e_addition_buff_goddess:
	{
		int32 equip_num = 0;
		for (int32 j = 0; j < max_goddess_equip_num; j++)
		{
			if (GET_BAG(e_bag_type_goddess_equip)[j].is_valid())
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_goddess_equip)[j]);
				if (item_ptr)
				{
					ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
					DeityTemplate* deity_template_ptr = item_ptr->get_deity_info_ptr();
					if (item_template_ptr && deity_template_ptr)
					{
						if (item_template_ptr->sub_type != 0 && deity_template_ptr->Type == addition_tmpl_ptr->AdditionBuffSubType &&
							deity_template_ptr->CurLevel + 1 >= addition_tmpl_ptr->AdditionBuffLevel)
						{
							equip_num++;
						}
					}
				}
			}
		}
		if (equip_num >= addition_tmpl_ptr->EquipNum)
		{
			int32 need_replace_index = -1;
			bool need_replace = false;
			for (int32 i = 0; i < buff_id_arr.size(); i++)
			{
				if (buff_equip_num_arr[i] == addition_tmpl_ptr->EquipNum && buff_sub_type_arr[i] == addition_tmpl_ptr->AdditionBuffSubType)
				{
					if (buff_level_arr[i] < addition_tmpl_ptr->AdditionBuffLevel)
					{
						need_replace = true;
						need_replace_index = i;
					}
					break;
				}
			}
			if (need_replace == true && need_replace_index >= 0 && need_replace_index < buff_id_arr.size())
			{
				buff_id_arr[need_replace_index] = addition_tmpl_ptr->attribute_id;
				buff_equip_num_arr[need_replace_index] = addition_tmpl_ptr->EquipNum;
				buff_need_num_arr[need_replace_index] = addition_tmpl_ptr->UpgradeNum;
				buff_sub_type_arr[need_replace_index] = addition_tmpl_ptr->AdditionBuffSubType;
				buff_level_arr[need_replace_index] = addition_tmpl_ptr->AdditionBuffLevel;
			}
			else
			{
				buff_id_arr.push_back(addition_tmpl_ptr->attribute_id);
				buff_equip_num_arr.push_back(addition_tmpl_ptr->EquipNum);
				buff_need_num_arr.push_back(addition_tmpl_ptr->UpgradeNum);
				buff_sub_type_arr.push_back(addition_tmpl_ptr->AdditionBuffSubType);
				buff_level_arr.push_back(addition_tmpl_ptr->AdditionBuffLevel);
			}
		}
	}
	break;
	default:
		break;
	}
}
void item_set::sync_whole_euip_fighting_power()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	int64 ranking_value = 0;
	ranking_value = calcu_whole_equip_fighting_power();
	temp_player.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_equipment_main, ranking_value);

	if (temp_player.check_and_set_highest_record(e_role_history_highest_record_equip_value, ranking_value) == true)
	{
		temp_player.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_equip_up);
		temp_player.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_cross_equip_up);
	}
}

int64 item_set::calcu_feather_total_fighting_power()
{
	int64 total_value = 0;
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return 0;
	}
	const double* att_array = player_ref.get_pawn_att().get_attack_att_all();
	std::vector<citem*> temp_feather_array = get_wing_feather();
	for (int32 i = 0; i < temp_feather_array.size(); ++i)
	{
		citem* temp_wing_feather = temp_feather_array[i];
		if (temp_wing_feather != nullptr)
		{
			if (temp_wing_feather->get_data_info(e_item_info_activate) <= 0)
			{
				continue;
			}
			ItemTemplate* item_template_ptr = temp_wing_feather->get_item_info_ptr();
			if (item_template_ptr != nullptr)
			{
				int32 real_id = item_template_ptr->logic_id + temp_wing_feather->get_data_info(e_item_info_upgrade_count);
				FeatherTemplate* feather_template_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
				if (feather_template_ptr != nullptr)
				{
					total_value += init_unit::calcu_fighting_power_by_att_array(feather_template_ptr->AttArray, att_array);
				}
			}
		}
	}

	return total_value;
}

int64 item_set::calcu_whole_equip_fighting_power()
{
	int64 ranking_value = 0;
	for (int32 slot_index = e_role_equip_slot_body; slot_index <= e_role_equip_slot_magic_2; slot_index++)
	{
		citem* equip_ptr = get_equip_item_by_slot((e_role_equip_slot)slot_index);
		if (nullptr != equip_ptr && is_equipment_effective(equip_ptr->get_item_guid()))
		{
			ranking_value += equip_ptr->calcu_single_item_fighting_power(m_unit_array_index);
		}
	}
	return ranking_value;
}

bool item_set::item_operate(const std::vector<guid_64>& item_guid, int32 item_slot, int32 operation_type, int32 use_up_item, int32 first_use_lock, int32 item_num, int32 force_do)
{
	int32 bSuccess = false;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		CONSOLE_ERROR("item_set::item_operate temp_player.is_valid() == false");
		return false;
	}
	server_log::item_operate_log(temp_player.get_third_info(), temp_player.get_unit_info_inst(), item_guid, item_slot, operation_type, use_up_item, first_use_lock, item_num, force_do);
	switch (operation_type)
	{
	case e_item_operation_tidy:
		bSuccess = tidy_item(e_bag_type_bag);
		break;
	case e_item_operation_equipon:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = equip_on(item_guid[0], item_slot);
			if (bSuccess)
			{
				sync_whole_euip_fighting_power();
				sync_ranking_spirit();
			}
		}
	}
	break;
	case e_item_operation_equipoff:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = equip_off(item_guid[0]);
			if (bSuccess)
			{
				sync_whole_euip_fighting_power();
			}
		}
	}
	break;
	case e_item_operation_weaponactivate:
	{
		bSuccess = activate_weapon(item_slot);
		if (bSuccess)
		{
			sync_whole_euip_fighting_power();
		}
	}
	break;
	case e_item_operation_weaponfirst:
	{
		bSuccess = activate_weapon_first(item_slot);
		if (bSuccess)
		{
			sync_whole_euip_fighting_power();
		}
	}
	break;
	case e_item_operation_synthesis:
		break;
	case e_item_operation_sell:
		if (item_guid.size() > 0)
		{
			bSuccess = item_sell(item_guid[0], item_num);
		}
		break;
	case e_item_operation_recovery:
	case e_item_operation_spirit_recovery:
		bSuccess = item_recovery(item_guid, use_up_item > 0);
		break;
	case e_item_operation_spirit_send_out:
	{
		//bSuccess = sprite_system::activate_spirit(&temp_player, item_guid[0]);
	}
	break;
	case e_item_operation_spirit_had:
	{
		//bSuccess = sprite_system::set_scene_show(&temp_player, item_guid[0], item_slot);
	}
	break;
	case e_item_operation_spirit_upgrade:
	{
		for (auto& it : item_guid)
		{
			//sprite_system::spirit_level_up(&temp_player, it);
		}
		bSuccess = e_item_string_succeed;
	}
	break;
	case e_item_operation_use:
		if (item_guid.size() > 0)
		{
			citem* end_item = nullptr;
			bSuccess = item_use(item_guid[0], end_item, item_num, force_do, false, item_slot);
		}
		break;
	case e_item_operation_one_key_use:
	{
		bSuccess = item_one_key_use(item_guid);
	}
	break;
	case e_item_operation_wing_shape_upgrade:
	case e_item_operation_wing_upgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_upgrade(item_guid[0], use_up_item);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_wing_levelup);
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_starupgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_starupgrade(item_guid[0], use_up_item);
			if (bSuccess)
			{
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_zhuling:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_zhuling(item_guid[0], use_up_item, item_num);
			if (bSuccess)
			{
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_lengque:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_lengque(item_guid[0], use_up_item);
		}
	}
	break;
	case e_item_operation_wing_xiling:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_xiling(item_guid[0], use_up_item);
			if (bSuccess)
			{
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_illusionupgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_illusionupgrade(item_guid[0], use_up_item);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_wing_illusion);
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_addsoul:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_add_soul(item_guid[0]);
			if (bSuccess)
			{
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_addspirit:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = wing_add_spirit(item_guid[0]);
			if (bSuccess)
			{
				sync_ranking_wing();
			}
		}
	}
	break;
	case e_item_operation_wing_shape_unlock:
		if (item_guid.size() > 0)
		{
			citem* temp_item = get_item(item_guid[0]);
			if (nullptr != temp_item && nullptr != temp_item->get_item_info_ptr())
			{
				if (temp_item->get_item_info_ptr()->sub_type == 2)
				{
					bSuccess = wing_feather_unlock(item_guid[0], item_slot);
				}
				else
				{
					bSuccess = wing_shape_unlock(item_guid[0], item_slot);
				}
			}
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_wing_levelup);
			}
		}
		break;
	case e_item_operation_wing_shape_illusion_unlock:
		if (item_guid.size() > 0)
		{
			bSuccess = wing_shape_illusion_unlock(item_guid[0], item_slot);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_wing_levelup);
			}
		}
		break;
	case e_item_operation_featherupgrade:
		if (item_guid.size() > 0)
		{
			bSuccess = feather_upgrade(item_guid[0], use_up_item);
		}
		break;
	case e_item_operation_inherit:
		bSuccess = item_inherit(item_guid, false);
		break;
	case e_item_operation_advanced:
		bSuccess = item_advance(item_guid, use_up_item);
		if (bSuccess)
		{
			temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_equip_upgrade);
		}
		//script_mgr::get_instance().call_func("advanced_mgr", "advanced_operate", 1, false, "%d%s%d>%d", m_unit_array_index, temp_player.get_awaken_mgr().item_guid_array_to_string(item_guid), use_up_item, &bSuccess);
		break;
	case e_item_operation_godassembly:
		if (item_guid.size() > 0)
		{
			bSuccess = item_assembly(item_guid[0], first_use_lock);
		}
		break;
	case e_item_operation_godassembled:
		if (item_guid.size() > 0)
		{
			bSuccess = item_assembled(item_guid[0], first_use_lock);
		}
		break;

	case e_item_operation_save_succinct_result:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = item_save_succinct(item_guid[0]);
			{
				if (bSuccess)
				{
					sync_whole_euip_fighting_power();

					citem* temp_item = get_item(item_guid[0]);
					if (temp_item != nullptr)
					{
						server_log::succinct_equip_save_log(temp_player.get_third_info(), temp_player.get_unit_info_inst(), temp_item->get_data_info(e_item_info_info_id));
						//	temp_player.check_and_set_highest_record(e_role_history_highest_record_weapon_full_succinct_grade);

					}
				}
			}
		}
	}
	break;
	case e_item_operation_show_this_wing:
	{
		if (item_guid.size() > 0)
		{
			citem* item_ptr = get_item(item_guid[0]);
			if (item_ptr)
			{
				bSuccess = show_this_wing(item_ptr, item_slot, true);
			}
		}
	}
	break;
	case e_item_operation_mount_unlock:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = mount_unlock(item_guid[0]);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_mount_unlock);
				sync_ranking_mount();
			}
		}
		break;
	}
	case e_item_operation_mount_upgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = mount_upgrade(item_guid[0], use_up_item);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_mount_levelup);
				sync_ranking_mount();
			}
		}
	}
	break;

	case e_item_operation_mount_starupgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = mount_starupgrade(item_guid[0], use_up_item);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_mount_star_grade);
				sync_ranking_mount();
			}
		}
	}
	break;
	case e_item_operation_mount_illusionupgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = mount_illusionupgrade(item_guid[0], use_up_item);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_mount_illusion);
				sync_ranking_mount();
			}
		}
	}
	break;
	case e_item_operation_mount_show_this_mount:
	{
		if (item_guid.size() > 0)
		{
			citem* item_ptr = get_item(item_guid[0]);
			if (item_ptr)
			{
				bSuccess = show_this_mount(item_ptr, item_slot);
			}
		}
	}
	break;
	case e_item_operation_mount_show_this_mount_illusion:
		if (item_guid.size() > 0)
		{
			bSuccess = show_this_mount_illusion(item_guid[0], item_slot);
		}
		break;
	case e_item_operation_mount_get_illusion:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = get_mount_illusion(item_guid[0], item_slot);
			{
				if (bSuccess)
				{
					sync_ranking_mount();
				}
			}
		}
	}
	break;
	case e_item_operation_retrieve_psyche:
		if (item_guid.size() > 0)
		{
			bSuccess = get_mount_illusion(item_guid[0], item_slot);
		}
		break;
	case e_item_operation_put_into_bag:
		if (item_guid.size() > 0)
		{
			bSuccess = put_item_into_bag_from_another_bag(item_guid[0]);
		}
		break;
	case e_item_operation_tidy_hope_item:
		bSuccess = tidy_item(e_bag_type_hope_item);
		break;
	case e_item_operation_quickly_hp:
		if (item_guid.size() > 0)
		{
			set_quick_hp_guid(item_guid[0]);
		}
		break;
	case e_item_operation_against_buy:
		if (item_guid.size() > 0)
		{
			bSuccess = against_buy_item(item_guid[0], item_slot);
			if (bSuccess)
			{
				sync_whole_euip_fighting_power();
			}
		}
		break;
	case e_item_operation_buy_and_use:
		if (item_guid.size() > 0)
		{
			bSuccess = item_buy_and_use(item_slot);
		}
		break;
	case e_item_operation_use_beast_spirit:
		if (item_guid.size() > 0)
		{
			bSuccess = use_beast_spirit(item_guid[0], item_num);
		}
		break;
	case e_item_operation_set_jewel_on:
	{
		if (item_guid.size() > 1)
		{
			//bSuccess = set_jewel_on(item_guid[0], item_slot, item_guid[1]);
			script_mgr::get_instance().call_func("jewel_mgr", "set_jewel_on", 1, false, "%d%s%d%s>%d", m_unit_array_index, item_guid[0].to_string().c_str(), item_slot, item_guid[1].to_string().c_str(), &bSuccess);
		}
	}
	break;
	case e_item_operation_set_jewel_off:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = set_jewel_off(item_guid[0], item_slot);
		}
	}
	break;
	case e_item_operation_jewel_upgrade:
	{
		if (item_guid.size() > 0)
		{
			bSuccess = jewel_upgrade(item_guid[0], item_slot);
			if (bSuccess)
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_jewel_composit);
			}
		}
	}
	break;
	case e_item_operation_tidy_treasure_item:
		bSuccess = tidy_item(e_bag_type_treasure);
		break;
	case e_item_operation_get_treasure_item:
		bSuccess = get_item_from_treasure_to_bag();
		break;
	case e_item_operation_tidy_storage_item:
		bSuccess = tidy_item(e_bag_type_storage);
		break;
	case e_item_operation_fashion_upgrade:
		if (item_guid.size() > 0)
		{
			bSuccess = fashion_upgrade(item_guid[0], use_up_item, item_num);
		}
		break;
	case e_item_operation_n_succinct_gold:
		if (item_guid.size() > 0)
		{
			bSuccess = item_succinct_n_times(item_guid[0], item_succinct_money_cost_gold, first_use_lock, use_up_item, item_num, force_do);
			operation_type = e_item_operation_succinct_gold;
		}
		break;
	case e_item_operation_n_succinct_diamond:
		if (item_guid.size() > 0)
		{
			bSuccess = item_succinct_n_times(item_guid[0], item_succinct_money_cost_diamond, first_use_lock, use_up_item, item_num, force_do);
			operation_type = e_item_operation_succinct_diamond;
		}
		break;
	case e_item_operation_enchant:
		if (item_guid.size() > 0)
		{
			bSuccess = item_enchant(item_guid[0], item_slot, first_use_lock);
			if (bSuccess)
			{
				sync_whole_euip_fighting_power();
			}
		}
		break;
	case e_item_operation_unenchant:
		if (item_guid.size() > 0)
		{
			bSuccess = item_unenchant(item_guid[0]);
			if (bSuccess)
			{
				sync_whole_euip_fighting_power();
			}
		}
		break;
	default:
		break;
	}
	item_proto_item_operation_end item_msg;

	item_msg.set_operation_state(bSuccess);
	item_msg.set_operation_type(operation_type);
	item_msg.set_item_slot(item_slot);

	item_msg.add_role_guid(temp_player.get_unit_guid().A);
	item_msg.add_role_guid(temp_player.get_unit_guid().B);
	for (int32 i = 0; i < item_guid.size(); ++i)
	{
		item_msg.add_item_guid(item_guid[i].A);
		item_msg.add_item_guid(item_guid[i].B);
	}

	temp_player.send_message_to_self(&item_msg, e_msgindex_s2c_item_operation);
	if (bSuccess > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool item_set::is_class_enable(citem* target_item)
{
	if (nullptr == target_item)
	{
		return false;
	}
	ItemTemplate const* item_template_ptr = target_item->get_item_info_ptr();
	if (NULL == item_template_ptr)
	{
		return false;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return false;
	}

	if (item_template_ptr->class_type.empty())
	{
		return true;
	}
	int32 real_class_type = temp_unit.get_unit_info(e_role_info_class_type);
	for (int32 i = 0; i < item_template_ptr->class_type.size(); ++i)
	{
		if (item_template_ptr->class_type[i] == 0
			|| real_class_type == item_template_ptr->class_type[i])
		{
			return true;
		}
	}
	return false;
}
// get current highest upgrade level 
int32	item_set::get_current_highest_upgrade_on_character()
{
	int32 max_upgrade_level = 0;
	for (int32 slot = e_role_equip_slot_weapon_1; slot < e_role_equip_slot_amulet; ++slot) // equipments that can be upgraded
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[slot]);
		if (nullptr != temp_equip_item_ptr)
		{
			int32 one_level = temp_equip_item_ptr->get_data_info(e_item_info_upgrade_count);
			if (one_level > max_upgrade_level)
			{
				max_upgrade_level = one_level;
			}
		}


	}
	return max_upgrade_level;
}
//get current highest addon level
int32 item_set::get_current_highest_addon_on_character()
{
	int32 max_addon_level = 0;
	for (int32 slot = e_role_equip_slot_weapon_1; slot < e_role_equip_slot_amulet; ++slot)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[slot]);
		if (nullptr != temp_equip_item_ptr)
		{
			int32 one_level = temp_equip_item_ptr->get_data_info(e_item_info_add_on);
			if (one_level > max_addon_level)
			{
				max_addon_level = one_level;
			}
		}
	}
	return max_addon_level;
}

int32 item_set::get_highest_mount_level()
{
	int32 max_level = 0;
	for (int32 item_slot = 0; item_slot < MAX_MOUNT_NUM; item_slot++)
	{
		citem* temp_item_ptr = get_item(GET_BAG(e_bag_type_mount)[item_slot]);

		if (nullptr == temp_item_ptr)
		{
			continue;
		}
		MountTemplate* real_mount_template_ptr = temp_item_ptr->get_mount_template_ptr();
		if (real_mount_template_ptr)
		{
			int32 temp_level = real_mount_template_ptr->ClassNow;
			if (temp_level > max_level)
			{
				max_level = temp_level;
			}
		}
	}
	return max_level;
}

bool item_set::equip_on(const guid_64& item_guid, int32& item_slot, bool is_without_auto_inherit, bool is_against_buy_time)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		CONSOLE_ERROR("item_set::equip_on temp_unit.is_valid() == false");
		return false;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_set::equip_on nullptr == item_ptr");
		return false;
	}

	if (!is_against_buy_time)
	{
		if (item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_storage
			&& item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_bag
			&& item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_fasion
			&& item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_goddess_equip
			&& item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_equip_spirit
			)
		{
			CONSOLE_ERROR("bag_type:{}", item_ptr->get_data_info(e_item_info_container_type));
			return false;
		}
	}

	if (false == is_can_add_att(item_ptr->get_item_guid()))
	{
		CONSOLE_ERROR("item_set::equip_on is_can_add_att false");
		return false;
	}
	item_ptr->set_add_att(true);
	ItemTemplate const* item_template_ptr = item_ptr->get_item_info_ptr();
	if (NULL == item_template_ptr)
	{
		CONSOLE_ERROR("item_set::equip_on NULL == item_template_ptr");
		return false;
	}

	if (!is_class_enable(item_ptr))
	{
		CONSOLE_ERROR("item_set::equip_on is_class_enable false");
		return false;
	}
	if (item_template_ptr->level_limit.size() >= 2)
	{
		if (temp_unit.get_unit_info(e_role_info_exp_level) < item_template_ptr->level_limit[0])
		{
			CONSOLE_ERROR("item_set::equip_on level_limit[0] = {} player level:{}", item_template_ptr->level_limit[0], temp_unit.get_unit_info(e_role_info_exp_level));
			return false;
		}
	}

	int32 in_which_item_slot = item_ptr->get_data_info(e_item_info_slot);
	e_bag_type in_which_bag_type = e_bag_type(item_ptr->get_data_info(e_item_info_container_type));
	citem* old_equip_item_ptr = nullptr;

	switch (item_template_ptr->item_type)
	{
	//case e_item_type_spirit:
	//{
	//	player& temp_player = unit_man::get_player(m_unit_array_index);
	//	return sprite_system::equip_on_spirit(&temp_player, item_ptr, item_slot) == e_item_string_succeed;
	//}
	//break;
	case e_item_type_fashion:
	{
		FasionTemplate* fashion_template_ptr = GET_TEMPLATE(FasionTemplate, item_template_ptr->logic_id);
		if (nullptr == fashion_template_ptr)
		{
			CONSOLE_ERROR("item_set::equip_on nullptr == fashion_template_ptr");
			return false;
		}
		if (item_template_ptr->sub_type < e_fashion_show_type_all
			|| item_template_ptr->sub_type >= e_fashion_show_type_max)
		{
			CONSOLE_ERROR("item_set::equip_on item_template_ptr->sub_type = {}", item_template_ptr->sub_type);
			return false;
		}

		guid_64 old_item_guid = GET_BAG(e_bag_type_equip_fasion)[item_template_ptr->sub_type];
		if (!is_against_buy_time)
		{
			if (old_item_guid.is_valid() && old_item_guid != item_guid)
			{
				old_equip_item_ptr = get_item(old_item_guid);
				if (!equip_off(old_item_guid, in_which_item_slot))
				{
					CONSOLE_ERROR("item_set::equip_on equip_off false");
					return false;
				}
			}
			else
			{
				GET_BAG(in_which_bag_type)[in_which_item_slot].clear_data();
			}
		}

		equip_off_equip_show_buff();

		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_equip_fasion);
		item_ptr->set_data_info(e_item_info_slot, item_template_ptr->sub_type);

		GET_BAG(e_bag_type_equip_fasion)[item_template_ptr->sub_type] = item_ptr->get_item_guid();

		item_ptr->set_data_info(e_item_info_locked, 1);
		send_item_one(item_ptr);

		equip_on_equip_show_buff();

		player& player_ref = unit_man::get_player(m_unit_array_index);

		if (item_template_ptr->sub_type == e_fashion_show_type_weapon || item_template_ptr->sub_type == e_fashion_show_type_cloth)
		{
			item_proto_show_fashion show_request_end;
			show_request_end.set_is_show_fashion(player_ref.get_unit_info(faith::e_role_info_show_fashion));
			show_request_end.add_role_guid(player_ref.get_unit_guid().A);
			show_request_end.add_role_guid(player_ref.get_unit_guid().B);
			player_ref.send_message_to_aoi(&show_request_end, e_msgindex_s2c_show_fashion, true);
		}

		//reset_weapon_fashion_effect();
		if (false == player_ref.is_valid())
		{
			return false;
		}
		if (item_template_ptr->sub_type == e_fashion_show_type_weapon_effect
			|| item_template_ptr->sub_type == e_fashion_show_type_cloth
			|| item_template_ptr->sub_type == e_fashion_show_type_cloth_effect
			|| item_template_ptr->sub_type == e_fashion_show_type_foot_effect
			|| item_template_ptr->sub_type == e_fashion_show_type_weapon)
		{
			set_show_fashion_buff();
		}
		else
		{
			item_ptr->set_unit_buff(m_unit_array_index, true);
		}

		if (item_template_ptr->sub_type == e_fashion_show_type_head_frame)
		{
			player_ref.set_unit_info(e_role_info_head_frame, item_template_ptr->attribute_id);
		}

		return true;
	}
	break;
	case e_item_type_sky_equip:
	{
		if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_sky_equip)
		{
			return false;
		}
		int32 should_in_which_equip_slot = item_template_ptr->sub_type;
		if (should_in_which_equip_slot < 0 || should_in_which_equip_slot >= sky_equip_slot_num)
		{
			return false;
		}
		guid_64 old_item_guid = GET_BAG(e_bag_type_sky_equip)[should_in_which_equip_slot];
		if (old_item_guid.is_valid() && old_item_guid != item_guid)
		{
			old_equip_item_ptr = get_item(old_item_guid);
			if (!equip_off(old_item_guid, in_which_item_slot, true))
			{
				return false;
			}
		}
		else
		{
			clear_target_slot(in_which_bag_type, in_which_item_slot);
		}


		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_sky_equip);
		item_ptr->set_data_info(e_item_info_slot, should_in_which_equip_slot);
		item_ptr->set_data_info(e_item_info_activate, 1);

		GET_BAG(e_bag_type_sky_equip)[should_in_which_equip_slot] = item_ptr->get_item_guid();
		send_item_one(item_ptr);

		set_sky_equip_att(item_ptr, true);
		calcu_addition(e_addition_buff_sky_equip);
		return true;
	}
	break;
	case e_item_type_skygod_equip:
	{
		if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_skygod_equip)
		{
			return false;
		}
		int32 should_in_which_equip_slot = item_template_ptr->sub_type;
		if (should_in_which_equip_slot < 0 || should_in_which_equip_slot >= skygod_equip_slot_num)
		{
			return false;
		}
		guid_64 old_item_guid = GET_BAG(e_bag_type_skygod_equip)[should_in_which_equip_slot];
		if (old_item_guid.is_valid() && old_item_guid != item_guid)
		{
			old_equip_item_ptr = get_item(old_item_guid);
			if (!equip_off(old_item_guid, in_which_item_slot, true))
			{
				return false;
			}
		}
		else
		{
			clear_target_slot(in_which_bag_type, in_which_item_slot);
		}


		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_skygod_equip);
		item_ptr->set_data_info(e_item_info_slot, should_in_which_equip_slot);
		item_ptr->set_data_info(e_item_info_activate, 1);

		GET_BAG(e_bag_type_skygod_equip)[should_in_which_equip_slot] = item_ptr->get_item_guid();
		send_item_one(item_ptr);

		set_skygod_equip_att(item_ptr, true);
		calcu_addition(e_addition_buff_skygod_equip);
		return true;
	}
	break;
	case e_item_type_supreme_equip:
	{
		if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_supreme_equip)
		{
			return false;
		}
		int32 should_in_which_equip_slot = item_template_ptr->sub_type;
		if (should_in_which_equip_slot < 0 || should_in_which_equip_slot >= supreme_equip_slot_num)
		{
			return false;
		}
		guid_64 old_item_guid = GET_BAG(e_bag_type_supreme_equip)[should_in_which_equip_slot];
		if (old_item_guid.is_valid() && old_item_guid != item_guid)
		{
			old_equip_item_ptr = get_item(old_item_guid);
			if (!equip_off(old_item_guid, in_which_item_slot, true))
			{
				return false;
			}
		}
		else
		{
			clear_target_slot(in_which_bag_type, in_which_item_slot);
		}


		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_supreme_equip);
		item_ptr->set_data_info(e_item_info_slot, should_in_which_equip_slot);
		item_ptr->set_data_info(e_item_info_activate, 1);

		GET_BAG(e_bag_type_supreme_equip)[should_in_which_equip_slot] = item_ptr->get_item_guid();
		send_item_one(item_ptr);

		set_supreme_equip_att(item_ptr, true);
		calcu_addition(e_addition_buff_supreme_equip);
	}
	break;
	case e_item_type_goddess_equip:
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return false;
		}
		if (item_template_ptr->sub_type == 0)
		{
			return temp_player.get_goddess_mgr().goddess_gods_on(item_ptr, item_slot);
		}
		DeityTemplate* deity_template_ptr = item_ptr->get_deity_info_ptr();
		if (nullptr == deity_template_ptr)
		{
			return false;
		}
		if (item_ptr->get_data_info(e_item_info_container_type) == e_item_type_goddess_equip)
		{
			return false;
		}
		int32 should_in_which_equip_slot = e_branch_type_dark + (deity_template_ptr->Type - 1) * per_goddess_type_equip_num + item_template_ptr->sub_type - 1;
		if (should_in_which_equip_slot < 0 || should_in_which_equip_slot >= max_goddess_equip_num)
		{
			return false;
		}
		guid_64 old_item_guid = GET_BAG(e_bag_type_goddess_equip)[should_in_which_equip_slot];
		if (old_item_guid.is_valid() && old_item_guid != item_guid)
		{
			if (!equip_off(old_item_guid, in_which_item_slot, true))
			{
				return false;
			}
		}
		else
		{
			clear_target_slot(in_which_bag_type, in_which_item_slot);
		}


		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_goddess_equip);
		item_ptr->set_data_info(e_item_info_slot, should_in_which_equip_slot);
		item_ptr->set_data_info(e_item_info_activate, 1);

		GET_BAG(e_bag_type_goddess_equip)[should_in_which_equip_slot] = item_ptr->get_item_guid();
		send_item_one(item_ptr);


		set_goddess_equip_att(item_ptr, true);
		calcu_addition(e_addition_buff_goddess);
		player& player_ref = unit_man::get_player(m_unit_array_index);

		citem* goddess_item = player_ref.get_goddess_mgr().get_select_type_goddess(deity_template_ptr->Type);
		if (nullptr == goddess_item)
		{
			return false;
		}
		player_ref.get_goddess_mgr().set_star_is_open(goddess_item, true);

		return true;
	}
	break;
	default:
	{
		//auto& unit_ref = unit_man::get_unit(m_unit_array_index);
		//return equipment_system::equip_on_equip(&unit_ref, item_ptr, item_slot);
	}
	}
	return false;
}

bool item_set::is_can_add_att(const guid_64& item_guid)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (!temp_player.is_valid())
	{
		return true;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return true;
	}
	ItemTemplate const* item_template_ptr = item_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return true;
	}
	if (item_template_ptr->item_type != e_item_type_weapon && item_template_ptr->item_type != e_item_type_armor)
	{
		return true;
	}
	//if (item_template_ptr->EquipOnNeedAtt.size() != 8)
	//{
	//	return true;
	//}
	//std::vector<int32> player_att(4, 0);
	//std::vector<int32> equip_need_att(4, 0);
	//for (int32 i = 0; i < 4; i++)
	//{
	//	player_att[i] = temp_player.get_pawn_att().get_unit_base_att(i + e_base_att_info_strength) + temp_player.get_pawn_att().get_unit_base_att(i + e_base_att_info_body_strength);
	//}
	//for (int32 i = 0; i < item_template_ptr->EquipOnNeedAtt.size(); i += 2)
	//{
	//	int32 need_att_index = 0;
	//	need_att_index = item_template_ptr->EquipOnNeedAtt[i] - 1;
	//	if (need_att_index >= equip_need_att.size())
	//	{
	//		continue;
	//	}
	//	equip_need_att[need_att_index] = item_template_ptr->EquipOnNeedAtt[i + 1];
	//}
	bool can_add_att = true;
	//for (int32 i = 0; i < 4; i++)
	//{
	//	if (player_att[i] < equip_need_att[i])
	//	{
	//		can_add_att = false;
	//	}
	//}
	return can_add_att;
}

bool item_set::is_in_duration(const guid_64& item_guid)
{
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return true;
	}
	if (item_ptr->get_data_info(e_item_info_over_time) > 0 && item_ptr->get_data_info(e_item_info_over_time) <= time_helper::get_cur_time_new().second)
	{
		return false;
	}
	return true;
}

bool item_set::is_equipment_effective(const guid_64& item_guid)
{
	return is_can_add_att(item_guid) && is_in_duration(item_guid);
}

void item_set::change_att_for_equip(const guid_64& item_guid, bool is_add)
{
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return;
	}
	equip_off_equip_show_buff();
	calcu_addition(e_addition_buff_upgrade);
	calcu_addition(e_addition_buff_quality);
	calcu_addition(e_addition_buff_addon);
	calcu_addition(e_addition_buff_succinct);
	calcu_addition(e_addition_buff_enchant);
	equip_on_equip_show_buff();
	set_equip_att(item_ptr, is_add);
}


bool item_set::equip_off(const guid_64& item_guid, int32 item_slot, bool is_auto_equip_off, bool is_change_equipment)
{
	ZoneScoped;
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_set::equip_off nullptr == item_ptr");
		return false;
	}

	ItemTemplate const* item_template_ptr = item_ptr->get_item_info_ptr();
	if (NULL == item_template_ptr)
	{
		CONSOLE_ERROR("item_set::equip_off NULL == item_template_ptr");
		return false;
	}

	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		CONSOLE_ERROR("item_set::equip_off temp_unit.is_valid() == false");
		return false;
	}
	player& temp_player = unit_man::get_player(m_unit_array_index);

	/*	if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip_spirit)
		{
			return sprite_system::equip_off_spirit(&temp_player, item_ptr);
		}
		else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip)
		{
			return equipment_system::equip_off_equip(&temp_unit, item_ptr);
		}
		else */if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_sky_equip)
	{
		int32 empty_slot = (item_slot >= 0 ? item_slot : get_empty_slot(e_bag_type_bag));
		if (empty_slot < 0)
		{
			return false;
		}

		int32 old_slot = item_ptr->get_data_info(e_item_info_slot);
		if (true == is_equipment_effective(item_ptr->get_item_guid()))
		{
			set_sky_equip_att(item_ptr, false);
			item_ptr->set_add_att(true);
		}

		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_bag);
		item_ptr->set_data_info(e_item_info_slot, empty_slot);
		item_ptr->set_data_info(e_item_info_activate, 0);

		GET_BAG(e_bag_type_bag)[empty_slot] = item_ptr->get_item_guid();
		clear_target_slot(e_bag_type_sky_equip, old_slot);

		if (false == is_auto_equip_off)
		{
			calcu_addition(e_addition_buff_sky_equip);
		}

		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid())
		{
			temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		}

		send_item_one(item_ptr);

		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, sky_equip_buff_id);
		temp_player.set_unit_info(e_role_info_show_sky_suit_shape, 0);
		temp_player.send_info_one(e_role_info_show_sky_suit_shape, true);


		faith::item_proto_change_sky_suit_shape request;
		request.set_operate_state(0);
		temp_player.send_message_to_self(&request, e_msgindex_s2c_change_sky_suit_shape_end);
		return true;

	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_skygod_equip)
	{
		int32 empty_slot = (item_slot >= 0 ? item_slot : get_empty_slot(e_bag_type_bag));
		if (empty_slot < 0)
		{
			return false;
		}

		int32 old_slot = item_ptr->get_data_info(e_item_info_slot);
		if (true == is_equipment_effective(item_ptr->get_item_guid()))
		{
			set_skygod_equip_att(item_ptr, false);
			item_ptr->set_add_att(true);
		}

		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_bag);
		item_ptr->set_data_info(e_item_info_slot, empty_slot);
		item_ptr->set_data_info(e_item_info_activate, 0);

		GET_BAG(e_bag_type_bag)[empty_slot] = item_ptr->get_item_guid();
		clear_target_slot(e_bag_type_skygod_equip, old_slot);

		if (false == is_auto_equip_off)
		{
			calcu_addition(e_addition_buff_skygod_equip);
		}

		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid())
		{
			temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		}

		send_item_one(item_ptr);

		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, skygod_equip_buff_id);

		temp_player.set_unit_info(e_role_info_show_sky_suit_shape, 0);
		temp_player.send_info_one(e_role_info_show_sky_suit_shape, true);


		faith::item_proto_change_sky_suit_shape request;
		request.set_operate_state(0);
		temp_player.send_message_to_self(&request, e_msgindex_s2c_change_sky_suit_shape_end);
		return true;

	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_supreme_equip)
	{
		int32 empty_slot = (item_slot >= 0 ? item_slot : get_empty_slot(e_bag_type_bag));
		if (empty_slot < 0)
		{
			return false;
		}

		int32 old_slot = item_ptr->get_data_info(e_item_info_slot);
		if (true == is_equipment_effective(item_ptr->get_item_guid()))
		{
			set_supreme_equip_att(item_ptr, false);
			item_ptr->set_add_att(true);
		}

		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_bag);
		item_ptr->set_data_info(e_item_info_slot, empty_slot);
		item_ptr->set_data_info(e_item_info_activate, 0);

		GET_BAG(e_bag_type_bag)[empty_slot] = item_ptr->get_item_guid();
		clear_target_slot(e_bag_type_supreme_equip, old_slot);

		if (false == is_auto_equip_off)
		{
			calcu_addition(e_addition_buff_supreme_equip);
		}

		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid())
		{
			temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		}

		send_item_one(item_ptr);

		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, supreme_equip_buff_id);
		temp_player.set_unit_info(e_role_info_show_sky_suit_shape, 0);
		temp_player.send_info_one(e_role_info_show_sky_suit_shape, true);


		faith::item_proto_change_sky_suit_shape request;
		request.set_operate_state(0);
		temp_player.send_message_to_self(&request, e_msgindex_s2c_change_sky_suit_shape_end);
		return true;

	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_goddess_equip)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			return false;
		}
		if (item_template_ptr->sub_type == 0)
		{
			return temp_player.get_goddess_mgr().goddess_gods_off(item_ptr, item_slot, is_auto_equip_off);
		}
		int32 empty_slot = (item_slot >= 0 ? item_slot : get_empty_slot(e_bag_type_bag));
		if (empty_slot < 0)
		{
			return false;
		}

		int32 old_slot = item_ptr->get_data_info(e_item_info_slot);
		//int32 real_slot = get_goddess_equip_slot(item_ptr);
		set_goddess_equip_att(item_ptr, false);
		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_bag);
		item_ptr->set_data_info(e_item_info_slot, empty_slot);
		item_ptr->set_data_info(e_item_info_activate, 0);

		GET_BAG(e_bag_type_bag)[empty_slot] = item_ptr->get_item_guid();
		clear_target_slot(e_bag_type_goddess_equip, old_slot);

		if (false == is_auto_equip_off)
		{
			calcu_addition(e_addition_buff_goddess);
		}

		temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		send_item_one(item_ptr);
		return true;
	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip_fasion)
	{
		FasionTemplate* fashion_template_ptr = GET_TEMPLATE(FasionTemplate, item_template_ptr->logic_id);
		if (nullptr == fashion_template_ptr
			|| item_template_ptr->sub_type < 0
			|| item_template_ptr->sub_type >= e_fashion_show_type_max)
		{
			return false;
		}
		int32 empty_slot = (item_slot >= 0 ? item_slot : get_empty_slot(e_bag_type_fasion));
		if (empty_slot < 0)
		{
			return false;
		}
		if (item_ptr->get_data_info(e_item_info_activate) > 0)
		{
			equip_off_upgrade_show_buff();
			//set_fashion_att(item_ptr, false);
		}
		int32 old_slot = item_ptr->get_data_info(e_item_info_slot);
		item_ptr->set_data_info(e_item_info_container_type, e_bag_type_fasion);
		item_ptr->set_data_info(e_item_info_slot, empty_slot);

		GET_BAG(e_bag_type_fasion)[empty_slot] = item_ptr->get_item_guid();
		GET_BAG(e_bag_type_equip_fasion)[old_slot].clear_data();

		if (item_ptr->get_data_info(e_item_info_activate) == 0)
		{
			return true;
		}

		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid())
		{
			temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);
		}
		item_ptr->set_unit_buff(m_unit_array_index, false);
		if (item_template_ptr->sub_type == e_fashion_show_type_weapon)
		{
			reset_weapon_fashion_effect();
		}
		if (item_template_ptr->sub_type == e_fashion_show_type_head_frame)
		{
			temp_player.set_unit_info(e_role_info_head_frame, 0);
		}
		send_item_one(item_ptr);

		equip_on_upgrade_show_buff();
		return true;
	}
	return false;
}
void item_set::reset_fashion_effect()
{
	reset_weapon_fashion_effect();

	for (int32 i = 0; i < e_fashion_show_type_max; i++)
	{
		if (i == e_fashion_show_type_weapon || i == e_fashion_show_type_weapon_effect)
		{
			continue;
		}
		citem* fashion_ptr = get_item_by_slot(e_bag_type_equip_fasion, i);
		if (fashion_ptr != nullptr)
		{
			fashion_ptr->set_unit_buff(m_unit_array_index, true);
		}
	}
}
void item_set::reset_weapon_fashion_effect()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (!player_ref.is_valid())
		return;
	int32 show_fashion = player_ref.get_unit_info(faith::e_role_info_show_fashion);
	citem* normal_weapon_ptr = get_item_by_slot(e_bag_type_equip, e_role_equip_slot_weapon_1);
	if (normal_weapon_ptr == nullptr)
	{
		citem* weapon_effect_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon_effect);
		if (weapon_effect_ptr != nullptr)
		{
			weapon_effect_ptr->set_unit_buff(m_unit_array_index, false);
		}

		citem* weapon_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon);
		if (weapon_mesh_ptr != nullptr)
		{
			weapon_mesh_ptr->set_unit_buff(m_unit_array_index, false);
		}
	}
	else
	{
		citem* weapon_effect_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon_effect);
		if (weapon_effect_ptr != nullptr)
		{
			if (weapon_effect_ptr->get_data_info(e_item_info_activate) < 1 || show_fashion == 0)
			{
				weapon_effect_ptr->set_unit_buff(m_unit_array_index, false);
			}
			else
			{
				weapon_effect_ptr->set_unit_buff(m_unit_array_index, true);
			}
		}
		citem* weapon_mesh_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon);
		if (weapon_mesh_ptr != nullptr)
		{
			if (weapon_mesh_ptr->get_data_info(e_item_info_activate) < 1 || show_fashion == 0)
			{
				weapon_mesh_ptr->set_unit_buff(m_unit_array_index, false);
				normal_weapon_ptr->set_unit_buff(m_unit_array_index, true);
			}
			else
			{
				normal_weapon_ptr->set_unit_buff(m_unit_array_index, false);
				weapon_mesh_ptr->set_unit_buff(m_unit_array_index, true);
			}
		}
		else
		{
			normal_weapon_ptr->set_unit_buff(m_unit_array_index, true);
		}
	}
}

void item_set::reset_cloth_fashion_effect()
{
	citem* cloth_effect_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_cloth_effect);
	if (cloth_effect_ptr != nullptr)
	{
		cloth_effect_ptr->set_unit_buff(m_unit_array_index, true);
	}
}

void item_set::reset_foot_fashion_effect()
{
	citem* foot_effect_ptr = get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_foot_effect);
	if (foot_effect_ptr != nullptr)
	{
		foot_effect_ptr->set_unit_buff(m_unit_array_index, true);
	}
}


bool item_set::is_can_add_fashion_buff(citem* item_fashion)
{
	if (nullptr == item_fashion || item_fashion->get_data_info(e_item_info_activate) <= 0)
	{
		return false;
	}

	if (item_fashion->get_data_info(e_item_info_container_type) == e_bag_type_equip_fasion)
	{
		ItemTemplate* item_fashion_template_ptr = item_fashion->get_item_info_ptr();
		if (nullptr == item_fashion_template_ptr)
		{
			return false;
		}

		if (item_fashion_template_ptr->item_type == e_item_type_fashion)
		{
			FasionTemplate* fashion_template_ptr = GET_TEMPLATE(FasionTemplate, item_fashion_template_ptr->logic_id);
			if (nullptr == fashion_template_ptr
				|| item_fashion_template_ptr->sub_type <= e_fashion_equip_slot_non
				|| item_fashion_template_ptr->sub_type >= e_fashion_equip_slot_max)
			{
				return false;
			}

			if (item_fashion_template_ptr->sub_type == e_fashion_show_type_weapon_effect
				|| item_fashion_template_ptr->sub_type == e_fashion_show_type_weapon)
			{
				citem* normal_weapon_ptr = get_item_by_slot(e_bag_type_equip, e_role_equip_slot_weapon_1);
				if (nullptr == normal_weapon_ptr)
				{
					return false;
				}
				else
				{
					return true;
				}

			}
		}
	}

	return false;
}

void item_set::equip_off_upgrade_show_buff()
{
	int32 weapon_buff_id = get_equipment_upgrade_buff_id(true);
	buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, weapon_buff_id);

	int32 equip_buff_id = get_equipment_upgrade_buff_id(false);
	buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_on_upgrade_show_buff()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}

	citem* weapon_ptr = get_equip_item_by_slot(e_role_equip_slot_weapon_1);
	if (weapon_ptr != nullptr)
	{
		int32 cur_upgrade_level = init_unit::get_upgrade_phase_level(weapon_ptr->get_data_info(e_item_info_upgrade_count));

		int32 weapon_buff_id = get_equipment_upgrade_buff_id(true);
		bool can_add_att = is_equipment_effective(weapon_ptr->get_item_guid());
		if (false == can_add_att)
		{
			weapon_buff_id = 0;
		}
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, weapon_buff_id);
		temp_player.upgrade_highest_record(e_role_history_highest_record_higest_weapon_upgrade_buff, cur_upgrade_level);
	}

	int32 eight_item_smallest_level = get_equip_smallest_upgrade_level();
	int32 cur_upgrade_level = init_unit::get_upgrade_phase_level(eight_item_smallest_level);

	int32 equip_buff_id = get_equipment_upgrade_buff_id(false);
	buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, equip_buff_id);
	temp_player.upgrade_highest_record(e_role_history_highest_record_higest_equip_upgrade_buff, cur_upgrade_level);
}

bool item_set::activate_weapon(int32 item_slot)
{
	/*if (item_slot < e_role_equip_slot_weapon_1 || item_slot > e_role_equip_slot_weapon_3)
	{
	return false;
	}
	auto it = m_unit_item_map.find(m_equip_ary[item_slot]);
	if (it == m_unit_item_map.end())
	{
	return false;
	}
	citem& activate_item_ptr = it->second;
	int32 activate_slot = activate_item_ptr.get_data_info(e_item_info_activate);
	if (e_role_equip_slot_weapon_1 == activate_slot)
	{
	return false;
	}
	citem* old_item_ptr = get_weapon_by_activate_slot(e_role_equip_slot_weapon_1);
	if (NULL == old_item_ptr )
	{
	activate_item_ptr.set_data_info(e_item_info_activate, e_role_equip_slot_weapon_1);
	send_item_one(activate_item_ptr);
	return true;
	}
	old_item_ptr->set_data_info(e_item_info_activate, activate_slot);
	activate_item_ptr.set_data_info(e_item_info_activate, e_role_equip_slot_weapon_1);
	activate_item_ptr.set_data_info(e_item_info_is_first, 1);
	send_item_one(old_item_ptr);
	send_item_one(activate_item_ptr);*/
	return true;
}

bool item_set::activate_weapon_first(int32 item_slot)
{
	//if (item_slot < e_role_equip_slot_weapon_1 || item_slot > e_role_equip_slot_weapon_3)
	//{
	//	return false;
	//}
	//auto it = m_unit_item_map.find(m_equip_ary[item_slot]);
	//if (it == m_unit_item_map.end())
	//{
	//	return false;
	//}
	//citem& activate_item_ptr = it->second;
	//activate_item_ptr.set_data_info(e_item_info_is_first, 1);
	//send_item_one(activate_item_ptr);
	return true;
}

bool item_set::check_equip_by_item_id(int32 item_id)
{
	if (item_id <= 0)
	{
		return false;
	}
	for (int32 i = e_role_equip_slot_weapon_1; i < e_role_equip_slot_amulet; ++i)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
		if (temp_equip_item_ptr == nullptr)
		{
			continue;
		}
		if (temp_equip_item_ptr->get_item_template_id() == item_id)
		{
			return true;
		}
	}
	return false;
}

faith::int32 item_set::item_enchant(const guid_64& item_guid, int32 enchant_template_id, int32 first_use_lock)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return e_item_string_unkown;
	}
	ItemTemplate const* item_template_ptr = item_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return e_item_string_unkown;
	}
	EquipTemplate* equip_template_ptr = item_ptr->get_equip_info_ptr();
	if (nullptr == equip_template_ptr)
	{
		return e_item_string_unkown;
	}

	EquipEnchantTemplate* enchant_template_ptr = GET_TEMPLATE(EquipEnchantTemplate, enchant_template_id);
	if (enchant_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	int32 enchant_num = item_ptr->get_data_info(e_item_info_illusion_had_byte);
	if (enchant_num / faith::max_enchant_type_num != (enchant_template_ptr->Level - 1))
	{
		return e_item_string_unkown;
	}
	if (enchant_num != 0 && enchant_num % faith::max_enchant_type_num != enchant_template_ptr->Type)
	{
		return e_item_string_unkown;
	}
	if (item_template_ptr->sub_type != enchant_template_ptr->Slot)
	{
		return e_item_string_unkown;
	}

	if (item_template_ptr->item_color < faith::e_item_color_purple)
	{
		return e_item_string_unkown;
	}
	if (equip_template_ptr->equip_enchant_num < enchant_template_ptr->Level)
	{
		return e_item_string_unkown;
	}

	if (enchant_template_ptr->ItemConsume.size() % 2 != 0)
	{
		return e_item_string_unkown;
	}
	for (int32 i = 0; i < enchant_template_ptr->ItemConsume.size() / 2; i++)
	{
		if (enchant_template_ptr->ItemConsume[i * 2] == 0)
		{
			break;
		}
		if (enchant_template_ptr->ItemConsume[i * 2 + 1] > get_item_count(e_bag_type_bag, enchant_template_ptr->ItemConsume[i * 2]))
		{
			return e_item_string_matirial_not_enough;
		}
	}

	for (int32 i = 0; i < enchant_template_ptr->ItemConsume.size() / 2; i++)
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_enchant, 0, enchant_template_ptr->ItemConsume[i * 2], enchant_template_ptr->ItemConsume[i * 2 + 1], first_use_lock);
	}

	faith::template_manager::template_type* enchant_table = template_manager::get_instance().get_templates(e_EquipEnchantTemplate);
	if (nullptr == enchant_table)
	{
		return e_item_string_unkown;
	}
	faith::template_manager::template_type::iterator ite;
	int32 title_count = 0;
	for (ite = enchant_table->begin(); ite != enchant_table->end(); ++ite)
	{
		EquipEnchantTemplate* temp_enchant_template_ptr = (EquipEnchantTemplate*)(ite->second);
		if (temp_enchant_template_ptr == nullptr)
		{
			continue;
		}
		if ((temp_enchant_template_ptr->Level == enchant_template_ptr->Level - 1) && (temp_enchant_template_ptr->Type == enchant_template_ptr->Type) && (temp_enchant_template_ptr->Slot == enchant_template_ptr->Slot))
		{
			item_change_att(temp_enchant_template_ptr->AttArray, 1, false);
		}
	}
	item_ptr->set_data_info(e_item_info_illusion_had_byte, enchant_template_ptr->Level * faith::max_enchant_type_num + enchant_template_ptr->Type);

	calcu_addition(e_addition_buff_enchant);
	item_change_att(enchant_template_ptr->AttArray, 1, true);

	send_item_one(item_ptr);

	temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);

	return e_item_string_succeed;
}

faith::int32 item_set::item_unenchant(const guid_64& item_guid)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return e_item_string_unkown;
	}
	ItemTemplate const* item_template_ptr = item_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return e_item_string_unkown;
	}
	int32 enchant_value = item_ptr->get_data_info(e_item_info_illusion_had_byte);
	if (enchant_value <= 0)
	{
		return e_item_string_unkown;
	}
	EquipTemplate* equip_template_ptr = item_ptr->get_equip_info_ptr();
	if (nullptr == equip_template_ptr)
	{
		return e_item_string_unkown;
	}
	int32 enchant_level = enchant_value / faith::max_enchant_type_num;
	int32 enchant_type = enchant_value % faith::max_enchant_type_num;
	int32 enchant_slot = equip_template_ptr->EquipSlot - 22000002;

	int32 enchant_template_id = -1;
	faith::template_manager::template_type* enchant_table = template_manager::get_instance().get_templates(e_EquipEnchantTemplate);
	if (nullptr == enchant_table)
	{
		return e_item_string_unkown;
	}
	faith::template_manager::template_type::iterator ite;
	for (ite = enchant_table->begin(); ite != enchant_table->end(); ++ite)
	{
		EquipEnchantTemplate* temp_enchant_template_ptr = (EquipEnchantTemplate*)(ite->second);
		if (temp_enchant_template_ptr == nullptr)
		{
			continue;
		}
		if ((temp_enchant_template_ptr->Level == enchant_level) && (temp_enchant_template_ptr->Type == enchant_type) && (temp_enchant_template_ptr->Slot == enchant_slot))
		{
			enchant_template_id = temp_enchant_template_ptr->attribute_id;
			break;
		}
	}
	if (enchant_template_id < 0)
	{
		return e_item_string_unkown;
	}
	EquipEnchantTemplate* enchant_template_ptr = GET_TEMPLATE(EquipEnchantTemplate, enchant_template_id);
	if (enchant_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	if (enchant_template_ptr->ItemGet.size() % 2 != 0)
	{
		return e_item_string_unkown;
	}
	if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip)
	{
		item_change_att(enchant_template_ptr->AttArray, 1, false);
	}
	item_ptr->set_data_info(e_item_info_illusion_had_byte, 0);
	send_item_one(item_ptr);

	std::vector<s_item_template_info> data_array;
	for (int32 i = 0; i < enchant_template_ptr->ItemGet.size() / 2; i++)
	{
		data_array.push_back({ enchant_template_ptr->ItemGet[i * 2],enchant_template_ptr->ItemGet[i * 2 + 1], 1 });
	}
	put_in_bag(e_server_log_add_item_unenchant, 0, data_array);
	calcu_addition(e_addition_buff_enchant);

	temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(item_ptr);

	return e_item_string_succeed;
}

int32 item_set::item_advance(const std::vector<guid_64>& item_guids, int32 use_up_item)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false || item_guids.size() < max_item_advance_num)
	{
		return e_error_code_item_advance;
	}
	//the items is not exist;
	vector<citem*> vec_item_ptr;
	vector<int32> vec_item_color;
	vector<EquipTemplate*> vec_item_equip_template_ptr;
	vector<int32> vec_item_excellent_num;
	vector<int32> vec_item_upgrade_level;
	vector<int32> vec_item_addon_level;
	vector<int32> vec_item_succinct_level;
	vector<float> vec_item_culturing_percent;

	vector<int32> vec_item_awaken_level;

	vector<int32> vec_item_jewel_skill_id;
	int32  open_jewel_carve_num = 0;


	vec_item_ptr.clear();
	vec_item_color.clear();
	vec_item_equip_template_ptr.clear();
	vec_item_excellent_num.clear();
	vec_item_upgrade_level.clear();
	vec_item_addon_level.clear();
	vec_item_succinct_level.clear();
	vec_item_culturing_percent.clear();


	vec_item_jewel_skill_id.clear();
	vec_item_awaken_level.clear();
	for (int32 i = 0; i < max_item_advance_num; i++)
	{
		citem* item_ptr = get_item(item_guids[i]);
		if (nullptr == item_ptr || item_ptr->get_data_info(e_item_info_over_time) > 0)
		{
			//return e_error_code_item_advance_over_time;
			return e_error_code_item_advance;
		}
		if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_storage)
		{
			return e_error_code_item_advance;
		}
		vec_item_ptr.push_back(item_ptr);
		vec_item_color.push_back(0);
		vec_item_equip_template_ptr.push_back(0);
		vec_item_excellent_num.push_back(0);
		vec_item_upgrade_level.push_back(0);
		vec_item_addon_level.push_back(0);
		vec_item_culturing_percent.push_back(0);
		vec_item_awaken_level.push_back(0);
	}
	for (int32 j = e_item_info_succinct_property1; j <= e_item_info_succinct_property6; ++j)
	{
		vec_item_succinct_level.push_back(0);
	}
	for (int32 i = 0; i < vec_item_color.size(); i++)
	{
		if (i > 0 && vec_item_ptr[i]->get_data_info(e_item_info_info_id) != vec_item_ptr[0]->get_data_info(e_item_info_info_id))
		{
			//return e_error_code_item_advance_three_quality;
			return e_error_code_item_advance;
		}
		vec_item_color[i] = vec_item_ptr[i]->get_item_color();
	}

	for (int32 i = 0; i < vec_item_equip_template_ptr.size(); i++)
	{
		EquipTemplate* equip_template_ptr = vec_item_ptr[i]->get_equip_info_ptr();
		if (nullptr == equip_template_ptr)
		{
			//return e_error_code_item_advance_three_template;
			return e_error_code_item_advance;
		}
		vec_item_equip_template_ptr[i] = equip_template_ptr;
	}

	for (int32 i = 0; i < vec_item_ptr.size(); i++)
	{
		vec_item_excellent_num[i] = vec_item_ptr[i]->get_excellent_att_num();
		vec_item_upgrade_level[i] = vec_item_ptr[i]->get_data_info(e_item_info_upgrade_count);
		vec_item_addon_level[i] = vec_item_ptr[i]->get_data_info(e_item_info_add_on);
		vec_item_awaken_level[i] = vec_item_ptr[i]->get_data_info(e_item_info_wing_exp);



		int32 skill_id = vec_item_ptr[i]->get_data_info(e_item_info_jewel_carve_1);
		int32 skill_num = 0;

		for (int32 j = e_item_info_jewel_carve_1; j <= e_item_info_jewel_carve_2; j++)
		{
			skill_id = vec_item_ptr[i]->get_data_info((e_item_info)j);
			if (skill_id != 0)
			{
				vec_item_jewel_skill_id.push_back(skill_id);
				skill_num++;
			}
		}
		if (skill_num > open_jewel_carve_num)
		{
			open_jewel_carve_num = skill_num;
		}
	}

	for (int32 i = 0; i < vec_item_culturing_percent.size(); i++)
	{
		vector<int32> item_property_max_limit;
		vec_item_ptr[i]->get_property_max_limit(item_property_max_limit);
		if (item_property_max_limit.size() > 0)
		{
			int32 cur_succinct_num = 0;
			int32 Max_succinct_num = 0;
			for (int32 j = e_item_info_succinct_property1; j <= e_item_info_succinct_property6; ++j)
			{
				int32 succinct_num = j - e_item_info_succinct_property1;
				if (vec_item_succinct_level[succinct_num] < vec_item_ptr[i]->get_data_info((e_item_info)j))
				{
					vec_item_succinct_level[succinct_num] = vec_item_ptr[i]->get_data_info((e_item_info)j);
				}
				if (vec_item_ptr[i]->get_data_info((e_item_info)j) > 0)
				{
					cur_succinct_num += vec_item_ptr[i]->get_data_info((e_item_info)j);
				}
			}

			for (int32 j = 0; j < item_property_max_limit.size(); ++j)
			{
				if (item_property_max_limit[i] > 0)
				{
					Max_succinct_num += item_property_max_limit[j];
				}
			}

			if (Max_succinct_num > 0)
			{
				vec_item_culturing_percent[i] = cur_succinct_num / Max_succinct_num;
			}
			else
			{
				vec_item_culturing_percent[i] = 0;
			}
		}
	}

	ItemTemplate* item_template_ptr = vec_item_ptr[0]->get_item_info_ptr();
	if (nullptr == item_template_ptr || item_template_ptr->advanced_type != e_item_advance_normal)
	{
		//return e_error_code_item_advance_normal;
		return e_error_code_item_advance;
	}

	//if (vec_item_equip_template_ptr[0]->AdvanceRadio < 1)
	//{
	//	int32 temp_random_value = random_gen::get_random(0, 100);
	//	if (temp_random_value > vec_item_equip_template_ptr[0]->AdvanceRadio * 100)
	//	{
	//		del_item(e_server_log_del_item_advance_cost, 0, item_guids[1], 1);
	//		del_item(e_server_log_del_item_advance_cost, 0, item_guids[2], 1);
	//		//return e_error_code_item_advance_random;
	//		return e_error_code_item_advance;
	//	}
	//}
	//if (vec_item_equip_template_ptr[0]->AdvanceSpend.size() % 2 != 0)
	//{
	//	return e_error_code_item_advance;
	//}
	//for (int32 i = 0; i < vec_item_equip_template_ptr[0]->AdvanceSpend.size(); i += 2)
	//{
	//	if (temp_player.can_cut_money((e_money_type)vec_item_equip_template_ptr[0]->AdvanceSpend[i], vec_item_equip_template_ptr[0]->AdvanceSpend[i + 1]) == false)
	//	{
	//		//	//return e_error_code_item_advance_money;
	//		return e_error_code_item_advance;
	//	}

	//}
	//for (int32 i = 0; i < vec_item_equip_template_ptr[0]->AdvanceSpend.size(); i += 2)
	//{
	//	temp_player.cut_money((e_money_type)vec_item_equip_template_ptr[0]->AdvanceSpend[i], vec_item_equip_template_ptr[0]->AdvanceSpend[i + 1], e_server_log_cut_money_item_advance, vec_item_ptr[0]->get_data_info(e_item_info_info_id));
	//}

	//if (vec_item_equip_template_ptr[0]->quality_level >= EQUIP_QUALITY_MAX)
	//{
	//	if (vec_item_equip_template_ptr[0]->AdvanceItemSpend.size() % 2 != 0)
	//	{
	//		return e_error_code_item_advance;
	//	}
	//	for (int32 i = 0; i < vec_item_equip_template_ptr[0]->AdvanceItemSpend.size(); i += 2)
	//	{
	//		if (vec_item_equip_template_ptr[0]->AdvanceItemSpend[i + 1] > get_item_count(e_bag_type_bag, vec_item_equip_template_ptr[0]->AdvanceItemSpend[i]))
	//		{
	//			return e_error_code_item_advance;
	//		}


	//	}
	//	for (int32 i = 0; i < vec_item_equip_template_ptr[0]->AdvanceItemSpend.size(); i += 2)
	//	{
	//		cost_item_by_id_with_lock_states(e_server_log_del_item_advance_cost, 0, vec_item_equip_template_ptr[0]->AdvanceItemSpend[i], vec_item_equip_template_ptr[0]->AdvanceItemSpend[i + 1], false);
	//	}
	//}

	//get the biggest upgrade level
	int32 upgradeLevel_advanceed = vec_item_upgrade_level[0] >= vec_item_upgrade_level[1] ? vec_item_upgrade_level[0] : vec_item_upgrade_level[1];
	if (vec_item_upgrade_level[2] >= upgradeLevel_advanceed)
	{
		upgradeLevel_advanceed = vec_item_upgrade_level[2];
	}


	//get the biggest addon_level
	int32 addonLevel_advanced = vec_item_addon_level[0] >= vec_item_addon_level[1] ? vec_item_addon_level[0] : vec_item_addon_level[1];
	if (vec_item_addon_level[2] >= addonLevel_advanced)
	{
		addonLevel_advanced = vec_item_addon_level[2];
	}

	//get the biggest addon_level
	int32 awakenLevel_advanced = vec_item_awaken_level[0] >= vec_item_awaken_level[1] ? vec_item_awaken_level[0] : vec_item_awaken_level[1];
	if (vec_item_awaken_level[2] >= awakenLevel_advanced)
	{
		awakenLevel_advanced = vec_item_awaken_level[2];
	}


	//if the any one suply_items is locked ,then the result one is locked.
	std::vector<int32> succinct_property;

	int32 property_start = 0;
	int32 property_end = int32(e_item_info_succinct_property6) - int32(e_item_info_succinct_property1);
	for (; property_start <= property_end; property_start++)
	{
		succinct_property.push_back(vec_item_succinct_level[property_start]);
	}

	citem temp_item;
	temp_item.clear_data();

	citem* new_item_ptr = create_item_by_template(e_server_log_add_item_item_advance, 0, vec_item_ptr[0]->get_item_info_ptr()->advanced_id, 1, 0);
	if (nullptr == new_item_ptr)
	{
		//return e_error_code_item_advance_create;
		return e_error_code_item_advance;
	}
	{
		EquipTemplate* inherit_equip_template_ptr = new_item_ptr->get_equip_info_ptr();
		if (nullptr == inherit_equip_template_ptr)
		{
			return e_error_code_item_advance;
		}

		std::vector<int32> waiting_inherit_jewer_array;
		waiting_inherit_jewer_array.reserve((e_item_info_jewel_vip_slot_1 - e_item_info_jewel_slot_0) * max_item_advance_num);
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
		{
			new_item_ptr->set_data_info((e_item_info)i, 0);
			for (int32 j = 0; j < vec_item_ptr.size(); j++)
			{
				int32 item_jewel_id = vec_item_ptr[j]->get_data_info((e_item_info)i);
				vec_item_ptr[j]->set_data_info((e_item_info)i, 0);
				if (item_jewel_id > 0)
				{
					waiting_inherit_jewer_array.push_back(item_jewel_id);
				}
			}
		}
		jewel_array_set_to_item(new_item_ptr, waiting_inherit_jewer_array, true);
	}

	temp_item = *new_item_ptr;
	s_item_info main_item_data;
	main_item_data = vec_item_ptr[0]->get_item_inst();

	ItemTemplate* new_advance_ptr = new_item_ptr->get_item_info_ptr();
	if (nullptr == new_advance_ptr)
	{
		//return e_error_code_item_advance_item_data;
		return e_error_code_item_advance;
	}

	for (int32 i = 0; i < vec_item_ptr.size(); i++)
	{
		if (e_item_locked == vec_item_ptr[i]->get_data_info(e_item_info_locked))
		{
			new_item_ptr->set_data_info(e_item_info_locked, e_item_locked);
			break;
		}
	}

	new_item_ptr->set_data_info(e_item_info_upgrade_count, upgradeLevel_advanceed);
	new_item_ptr->set_data_info(e_item_info_add_on, addonLevel_advanced);
	new_item_ptr->set_data_info(e_item_info_wing_exp, awakenLevel_advanced);

	int32 skill_id1 = 0, skill_id2 = 0;

	temp_player.get_jewel_carve_mgr().get_random_skill(vec_item_jewel_skill_id, skill_id1, skill_id2, open_jewel_carve_num);

	new_item_ptr->set_data_info(e_item_info_jewel_carve_1, skill_id1);
	new_item_ptr->set_data_info(e_item_info_jewel_carve_2, skill_id2);

	new_item_ptr->set_data_info(e_item_info_illusion_had_byte, main_item_data.data_ary[e_item_info_illusion_had_byte]);
	e_role_equip_slot equip_slot = init_unit::get_slot_by_item_type(item_template_ptr->item_type, item_template_ptr->sub_type);
	for (int32 i = e_item_info_random_had_flag; i <= e_item_info_random_property6; ++i)
	{
		if (equip_slot == e_role_equip_slot_magic_1)
		{
			break;
		}
		new_item_ptr->set_data_info(e_item_info(i), main_item_data.data_ary[i]);
	}

	//count the attribute of main item after upgrading the upgrade_level and addon_level
	new_item_ptr->set_data_info((e_item_info)e_item_info_succinct_level, main_item_data.data_ary[e_item_info_succinct_level]);
	for (int32 i = 0; i < succinct_property.size(); i++)
	{
		new_item_ptr->set_data_info((e_item_info)(int32(e_item_info_succinct_property1) + i), succinct_property[i]);
	}


	int32 item_color = new_item_ptr->get_item_color();
	s_chat_notice_info 	notice_data;
	notice_data.item_data.push_back(new_item_ptr->get_item_inst());
	put_in_bag(new_item_ptr);

	if (e_item_color_orange == item_color)
	{
		int32 notice_id = 93000153;
		temp_player.get_chat_mgr().send_notice_new(notice_id, notice_data);
	}
	else if (e_item_color_purple == item_color)
	{
		int32 notice_id = 93000152;
		temp_player.get_chat_mgr().send_notice_new(notice_id, notice_data);
	}

	//succeess get new item to send promp to client
	std::vector<s_item_template_info> temp_item_data_array;
	temp_item_data_array.push_back({ temp_item.get_data_info(e_item_info_info_id),temp_item.get_data_info(e_item_info_stack_count),temp_item.get_data_info(e_item_info_locked) });
	get_item_send_promp_msg_to_client(temp_item_data_array);

	int32 finish_item_quality_level = 0;
	int32 finish_item_upgrade_level = 0;
	int32 finish_item_addon_level = 0;
	EquipTemplate* new_equip_template_ptr = temp_item.get_equip_info_ptr();
	if (nullptr != new_equip_template_ptr)
	{
		//finish_item_quality_level = new_equip_template_ptr->quality_level;
	}
	ItemUpgradeTemplate* new_upgrade_template_ptr = temp_item.get_upgrade_info_ptr();
	if (nullptr != new_upgrade_template_ptr)
	{
		//finish_item_upgrade_level = new_upgrade_template_ptr->level;
	}
	ItemAddonTemplate* new_addon_template_ptr = temp_item.get_add_on_info_ptr();
	if (nullptr != new_upgrade_template_ptr)
	{
		finish_item_addon_level = new_addon_template_ptr->level;
	}
	int32 cur_succinct_num = 0;
	int32 Max_succinct_num = 0;
	float finish_item_culturing_percent = 0;
	vector<int32> new_property_max_limit;
	temp_item.get_property_max_limit(new_property_max_limit);
	if (new_property_max_limit.size() > 0)
	{
		for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property6; ++i)
		{
			if (temp_item.get_data_info((e_item_info)i) > 0)
			{
				cur_succinct_num += temp_item.get_data_info((e_item_info)i);
			}
		}
		for (int32 i = 0; i < new_property_max_limit.size(); ++i)
		{
			if (new_property_max_limit[i] > 0)
			{
				Max_succinct_num += new_property_max_limit[i];
			}
		}
	}
	if (Max_succinct_num > 0)
	{
		finish_item_culturing_percent = cur_succinct_num / Max_succinct_num;
	}
	//server_log::advance_equip_log(temp_player.get_third_info(), temp_player.get_unit_info_inst(), item_guids[0].server_64, vec_item_equip_template_ptr[0]->quality_level, vec_item_color[0], vec_item_excellent_num[0], vec_item_upgrade_level[0], vec_item_addon_level[0], vec_item_culturing_percent[0],
	//	item_guids[1].server_64, vec_item_equip_template_ptr[1]->quality_level, vec_item_color[1], vec_item_excellent_num[1], vec_item_upgrade_level[1], vec_item_addon_level[1], vec_item_culturing_percent[1],
	//	item_guids[2].server_64, vec_item_equip_template_ptr[2]->quality_level, vec_item_color[2], vec_item_excellent_num[2], vec_item_upgrade_level[2], vec_item_addon_level[2], vec_item_culturing_percent[2],
	//	temp_item.get_item_guid().server_64, finish_item_quality_level, temp_item.get_item_color(), temp_item.get_excellent_att_num(), finish_item_upgrade_level, finish_item_addon_level, finish_item_culturing_percent);

	temp_player.get_mission_mgr().target_check(e_mission_end_type_equip_levelup);

	for (int32 i = 1; i < max_item_advance_num; i++)
	{
		citem* item_ptr = get_item(item_guids[i]);
		if (item_ptr->get_data_info(e_item_info_illusion_had_byte) > 0)
		{
			item_unenchant(item_ptr->get_item_guid());
		}
	}

	del_item(e_server_log_del_item_advance_cost, 0, item_guids[0], 1);
	del_item(e_server_log_del_item_advance_cost, 0, item_guids[1], 1);
	del_item(e_server_log_del_item_advance_cost, 0, item_guids[2], 1);

	return e_error_code_success;
}
bool item_set::is_can_inherit(citem* strip_item_ptr, citem* inherited_item_ptr)
{
	if (strip_item_ptr == nullptr
		|| inherited_item_ptr == nullptr)
	{
		return false;
	}
	EquipTemplate* equip_template_ptr = inherited_item_ptr->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return false;
	}

	//bool upgrade_can = false;
	//bool add_on_can = false;
	//bool succinct_can = false;

	int32 strip_upgrade_num = strip_item_ptr->get_data_info(e_item_info_upgrade_count);
	int32 inherited_upgrade_num = inherited_item_ptr->get_data_info(e_item_info_upgrade_count);
	//if (inherited_upgrade_num < equip_template_ptr->limit_activity_equip_upgrade_num
	//	&& strip_upgrade_num > inherited_upgrade_num)
	//{
	//	//upgrade_can = true;
	//	return true;
	//}

	int32 strip_add_on_num = strip_item_ptr->get_data_info(e_item_info_add_on);
	int32 inherited_add_on_num = inherited_item_ptr->get_data_info(e_item_info_add_on);
	//if (inherited_add_on_num < equip_template_ptr->equip_addon_num
	if (strip_add_on_num > inherited_add_on_num)
	{
		//add_on_can = true;
		return true;
	}


	vector<int32> vec_property_max_limit;
	inherited_item_ptr->get_property_max_limit(vec_property_max_limit);
	if (vec_property_max_limit.size() > 0)
	{
		for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property4; ++i)
		{
			int32 temp_index = i - e_item_info_succinct_property1;
			if (temp_index >= vec_property_max_limit.size())
			{
				break;
			}
			int32 strip_succinct_num = strip_item_ptr->get_data_info(e_item_info(i));
			int32 inherited_succinct_num = inherited_item_ptr->get_data_info(e_item_info(i));
			if (inherited_succinct_num < vec_property_max_limit[temp_index]
				&& strip_succinct_num > inherited_succinct_num)
			{
				//succinct_can = true;
				//break;
				return true;
			}
		}
	}

	for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
	{
		if (strip_item_ptr->get_data_info(e_item_info(i)) > 0)
		{
			return true;
		}
	}


	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() &&
		temp_player.get_awaken_mgr().is_can_inherit(strip_item_ptr, inherited_item_ptr))
	{
		return true;
	}


	if (temp_player.is_valid() &&
		temp_player.get_jewel_carve_mgr().is_can_inherit(strip_item_ptr, inherited_item_ptr))
	{
		return true;
	}
	//if (upgrade_can || add_on_can || succinct_can)
	//{
	//	return true;
	//}
	return false;
}

bool item_set::is_will_lose_point(citem* strip_item_ptr, citem* inherited_item_ptr)
{
	if (strip_item_ptr == nullptr
		|| inherited_item_ptr == nullptr)
	{
		return false;
	}
	EquipTemplate* equip_template_ptr = inherited_item_ptr->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return false;
	}
	vector<int32> vec_property_max_limit;
	inherited_item_ptr->get_property_max_limit(vec_property_max_limit);
	if (vec_property_max_limit.size() <= 0)
	{
		return false;
	}

	//int32 strip_upgrade_num = strip_item_ptr->get_data_info(e_item_info_upgrade_count);
	//if (strip_upgrade_num > equip_template_ptr->limit_activity_equip_upgrade_num)
	//{
	//	return true;
	//}

	int32 strip_add_on_num = strip_item_ptr->get_data_info(e_item_info_add_on);
	if (strip_add_on_num > equip_template_ptr->equip_addon_num)
	{
		return true;
	}


	for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property4; ++i)
	{
		int32 temp_index = i - e_item_info_succinct_property1;
		if (temp_index >= vec_property_max_limit.size())
		{
			break;
		}
		int32 strip_succinct_num = strip_item_ptr->get_data_info(e_item_info(i));
		if (strip_succinct_num > vec_property_max_limit[temp_index])
		{
			return true;
		}
	}
	return false;
}

int64 item_set::get_inherit_money_num(citem* strip_item_ptr, int32 cost_type)
{
	int64 return_value = 0;
	if (strip_item_ptr == nullptr)
	{
		return return_value;
	}
	ItemUpgradeTemplate* upgrade_template_ptr = strip_item_ptr->get_upgrade_info_ptr();
	if (upgrade_template_ptr == nullptr)
	{
		return return_value;
	}
	EquipTemplate* equip_template_ptr = strip_item_ptr->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return return_value;
	}
	ItemSuccinctTemplate* succinct_template_ptr = strip_item_ptr->get_succinc_info_ptr();
	if (succinct_template_ptr == nullptr)
	{
		return return_value;
	}
	ItemAddonTemplate* add_on_template_ptr = strip_item_ptr->get_add_on_info_ptr();
	if (add_on_template_ptr == nullptr)
	{
		return return_value;
	}

	int32 money_num_get_index = 1;
	if (cost_type > 0)
	{
		money_num_get_index = 3;
	}
	//if (money_num_get_index >= equip_template_ptr->InheritUpgradeSpend.size()
	//	|| money_num_get_index >= equip_template_ptr->InheritAddonSpend.size()
	//	|| money_num_get_index >= equip_template_ptr->InheritSuccinctSpend.size())
	//{
	//	return return_value;
	//}

	//int64 base_upgrade_cost = equip_template_ptr->InheritUpgradeSpend[money_num_get_index];
	//int64 base_add_on_cost = equip_template_ptr->InheritAddonSpend[money_num_get_index];
	//int64 base_succinct_cost = 0;
	//if (upgrade_template_ptr->InheritUpgradeRatio.empty()
	//	|| money_num_get_index >= upgrade_template_ptr->InheritUpgradeRatio.size())
	//{
	//	base_upgrade_cost = 0;
	//}
	//else
	//{
	//	base_upgrade_cost *= upgrade_template_ptr->InheritUpgradeRatio[money_num_get_index];
	//}
	//if (add_on_template_ptr->InheritAddonRatio.empty()
	//	|| money_num_get_index >= add_on_template_ptr->InheritAddonRatio.size())
	//{
	//	base_add_on_cost = 0;
	//}
	//else
	//{
	//	base_add_on_cost *= add_on_template_ptr->InheritAddonRatio[money_num_get_index];
	//}

	//if (money_num_get_index < equip_template_ptr->InheritSuccinctSpend.size())
	//{
	//	for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property4; ++i)
	//	{
	//		if (strip_item_ptr->get_data_info(e_item_info(i)) > 0)
	//		{
	//			base_succinct_cost = equip_template_ptr->InheritSuccinctSpend[money_num_get_index];
	//			break;
	//		}
	//	}
	//}

	//return_value = base_upgrade_cost + base_add_on_cost + base_succinct_cost;

	return return_value;
}

int32 item_set::item_inherit(const std::vector<guid_64>& item_guids, bool is_auto)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_error_code_item_inherit;
	}
	if (item_guids.size() < 2)
	{
		return e_error_code_item_inherit;
	}
	citem* item_strip_ptr = get_item(item_guids[0]);
	citem* item_inherit_ptr = get_item(item_guids[1]);
	if (nullptr == item_strip_ptr)
	{
		return e_error_code_item_inherit;
	}
	if (nullptr == item_inherit_ptr)
	{
		return e_error_code_item_inherit;
	}

	if (item_inherit_ptr->get_item_info_ptr()->item_type != item_strip_ptr->get_item_info_ptr()->item_type)
	{
		return e_error_code_item_inherit;
	}

	if (item_inherit_ptr->get_item_info_ptr()->item_type != e_item_type_weapon && item_inherit_ptr->get_item_info_ptr()->sub_type != item_strip_ptr->get_item_info_ptr()->sub_type)
	{
		return e_error_code_item_inherit;
	}

	EquipTemplate* inherit_equip_template_ptr = item_inherit_ptr->get_equip_info_ptr();
	if (nullptr == inherit_equip_template_ptr)
	{
		return e_error_code_item_inherit;
	}
	vector<int32> vec_property_max_limit;
	item_inherit_ptr->get_property_max_limit(vec_property_max_limit);

	if (vec_property_max_limit.size() <= 0)
	{
		return e_error_code_item_inherit;
	}

	if (!is_can_inherit(item_strip_ptr, item_inherit_ptr))
	{
		return e_error_code_item_inherit;
	}

	//int32 money_cost_type = e_money_type_silver_bind;
	//if (use_up_item > 0)
	//{
	//	money_cost_type = e_money_type_jewel;
	//}
	//int32 money_cost_num = get_inherit_money_num(item_strip_ptr, use_up_item);
	//if (temp_player.can_cut_money((e_money_type)money_cost_type, money_cost_num) == false)
	//{
	//	return e_error_code_item_inherit;
	//}

	bool is_equiped = false;
	int32 should_in_which_equip_slot = init_unit::get_slot_by_item_type(item_inherit_ptr->get_item_info_ptr()->item_type, item_inherit_ptr->get_item_info_ptr()->sub_type);
	guid_64 old_item_guid = GET_BAG(e_bag_type_equip)[should_in_which_equip_slot];
	if (old_item_guid == item_inherit_ptr->get_item_guid()
		|| old_item_guid == item_strip_ptr->get_item_guid())
	{
		if (is_auto == false)
		{
			equip_off(old_item_guid);
			is_equiped = true;
		}
	}

	int32 inherit_upgrade_num = item_inherit_ptr->get_data_info(e_item_info_upgrade_count);
	int32 strip_upgrade_num = item_strip_ptr->get_data_info(e_item_info_upgrade_count);
	int32 final_upgrade_num = inherit_upgrade_num;
	if (strip_upgrade_num > inherit_upgrade_num)
	{
		//if (strip_upgrade_num <= inherit_equip_template_ptr->limit_activity_equip_upgrade_num)
		//{
		//	item_inherit_ptr->set_data_info(e_item_info_upgrade_count, strip_upgrade_num);
		//	item_strip_ptr->set_data_info(e_item_info_upgrade_count, inherit_upgrade_num);
		//}
	}

	int32 inherit_add_on_num = item_inherit_ptr->get_data_info(e_item_info_add_on);
	int32 strip_add_on_num = item_strip_ptr->get_data_info(e_item_info_add_on);
	int32 final_add_on_num = inherit_add_on_num;
	if (strip_add_on_num > inherit_add_on_num)
	{
		//if (strip_add_on_num <= inherit_equip_template_ptr->equip_addon_num)
		//{
		item_inherit_ptr->set_data_info(e_item_info_add_on, strip_add_on_num);
		item_strip_ptr->set_data_info(e_item_info_add_on, inherit_add_on_num);
		//}
	}


	for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property4; ++i)
	{
		int32 temp_index = i - e_item_info_succinct_property1;
		if (temp_index >= vec_property_max_limit.size())
		{
			item_strip_ptr->set_data_info((e_item_info)i, 0);
		}
		int32 inherit_succinct_num = item_inherit_ptr->get_data_info((e_item_info)i);
		int32 strip_succinct_num = item_strip_ptr->get_data_info((e_item_info)i);
		int32 final_succinct_num = inherit_succinct_num;
		if (strip_succinct_num > inherit_succinct_num)
		{
			if (strip_succinct_num <= vec_property_max_limit[temp_index])
			{
				item_inherit_ptr->set_data_info((e_item_info)i, strip_succinct_num);
				item_strip_ptr->set_data_info((e_item_info)i, inherit_succinct_num);
			}
		}
	}
	item_inherit_ptr->set_data_info(e_item_info_succinct_level, item_strip_ptr->get_data_info(e_item_info_succinct_level));
	item_strip_ptr->set_data_info(e_item_info_succinct_level, 0);
	if (item_strip_ptr->get_data_info(e_item_info_locked) > 0
		|| item_inherit_ptr->get_data_info(e_item_info_locked) > 0)
	{
		item_inherit_ptr->set_data_info(e_item_info_locked, 1);
		item_strip_ptr->set_data_info(e_item_info_locked, 1);
	}

	{
		std::vector<int32> waiting_inherit_jewer_array;
		waiting_inherit_jewer_array.reserve((e_item_info_jewel_vip_slot_1 - e_item_info_jewel_slot_0) * 2);
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
		{
			int32 strip_item_jewel_id = item_strip_ptr->get_data_info((e_item_info)i);
			int32 inherit_item_jewel_id = item_inherit_ptr->get_data_info((e_item_info)i);

			item_strip_ptr->set_data_info((e_item_info)i, 0);
			item_inherit_ptr->set_data_info((e_item_info)i, 0);

			if (strip_item_jewel_id > 0)
			{
				waiting_inherit_jewer_array.push_back(strip_item_jewel_id);
			}

			if (inherit_item_jewel_id > 0)
			{
				waiting_inherit_jewer_array.push_back(inherit_item_jewel_id);
			}
		}

		if (waiting_inherit_jewer_array.size() > 0)
		{
			//int32 slot_open_num_by_equip = init_unit::get_jewel_slot_open_num_by_equip(inherit_equip_template_ptr->quality_level);
			//int32 slot_open_num_by_vip = init_unit::get_jewel_slot_open_num_by_vip(temp_player.get_vip_level());

			//for (int32 i = 0; i < slot_open_num_by_equip; ++i)
			//{
			//	int32 target_jewel_slot = e_item_info_jewel_slot_0 + i;
			//	if (target_jewel_slot >= e_item_info_jewel_vip_slot_0)
			//	{
			//		break;
			//	}
			//	int32 maximum_index = -1;
			//	int32 maximum_id = 0;
			//	for (int32 j = 0; j < waiting_inherit_jewer_array.size(); ++j)
			//	{
			//		if (waiting_inherit_jewer_array[j] > maximum_id)
			//		{
			//			maximum_index = j;
			//			maximum_id = waiting_inherit_jewer_array[j];
			//		}
			//	}
			//	if (maximum_id > 0)
			//	{
			//		waiting_inherit_jewer_array.erase(waiting_inherit_jewer_array.begin() + maximum_index);
			//		item_inherit_ptr->set_data_info((e_item_info)target_jewel_slot, maximum_id);
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}

			//for (int32 i = 0; i < slot_open_num_by_vip; ++i)
			//{
			//	int32 target_jewel_slot = e_item_info_jewel_vip_slot_0 + i;
			//	if (target_jewel_slot > e_item_info_jewel_vip_slot_1)
			//	{
			//		break;
			//	}
			//	int32 maximum_index = -1;
			//	int32 maximum_id = 0;
			//	for (int32 j = 0; j < waiting_inherit_jewer_array.size(); ++j)
			//	{
			//		if (waiting_inherit_jewer_array[j] > maximum_id)
			//		{
			//			maximum_index = j;
			//			maximum_id = waiting_inherit_jewer_array[j];
			//		}
			//	}
			//	if (maximum_id > 0)
			//	{
			//		waiting_inherit_jewer_array.erase(waiting_inherit_jewer_array.begin() + maximum_index);
			//		item_inherit_ptr->set_data_info((e_item_info)target_jewel_slot, maximum_id);
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}

			std::vector<citem*> need_send_mail_array;
			need_send_mail_array.reserve(waiting_inherit_jewer_array.size());
			for (int32 i = 0; i < waiting_inherit_jewer_array.size(); ++i)
			{
				citem* temp_item = create_item_by_template(e_server_log_add_item_set_jewel_off, 0, waiting_inherit_jewer_array[i], 1, 1);
				if (temp_item == nullptr)
				{
					continue;
				}
				need_send_mail_array.push_back(temp_item);
			}

			if (need_send_mail_array.size() > 0)
			{
				put_in_bag(need_send_mail_array, e_bag_type_none, e_item_mail_type_jewel_send_mail);
				temp_player.get_chat_mgr().receive_top_tip(90091120);
			}
			else
			{
				temp_player.get_chat_mgr().receive_top_tip(90091119);
			}
		}
	}

	temp_player.get_awaken_mgr().set_inherit(item_strip_ptr, item_inherit_ptr);

	temp_player.get_jewel_carve_mgr().set_inherit(item_strip_ptr, item_inherit_ptr);


	if (is_equiped)
	{
		equip_on(old_item_guid, should_in_which_equip_slot, true);
	}
	send_item_one(item_inherit_ptr);
	send_item_one(item_strip_ptr);
	return e_error_code_success;
}

int32 item_set::item_assembly(const guid_64& item_guid, int32 first_use_lock)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_error_code_item_godassembly;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return e_error_code_item_godassembly;
	}

	if (item_ptr->get_data_info(e_item_info_over_time) > 0)
	{
		return e_error_code_item_godassembly;
	}

	if (temp_player.get_unit_info(e_role_info_exp_level) < 2)
	{
		return e_error_code_item_godassembly;
	}

	EquipTemplate* equiptemplate_ptr = item_ptr->get_equip_info_ptr();
	if (!equiptemplate_ptr)
	{
		return e_error_code_item_godassembly;
	}
	//if (EQUIP_QUALITY_MAX > equiptemplate_ptr->quality_level)
	//{
	//	return e_error_code_item_godassembly;
	//}

	if (item_ptr->get_item_info_ptr()->advanced_type != e_item_advance_godassembly)
	{
		return e_error_code_item_godassembly;
	}
	//std::vector<int32>& materialList = equiptemplate_ptr->AdvanceSpend;
	//int32   materialTypeInt = materialList.size();
	//bool is_use_lock_material = false;
	//if (materialTypeInt >= 4 && materialTypeInt % 2 == 0)
	//{
	//	if (!temp_player.can_cut_money((e_money_type)materialList[0], materialList[1]))
	//	{
	//		return e_error_code_item_godassembly;
	//	}

	//	if (!temp_player.can_cut_money((e_money_type)materialList[2], materialList[3]))
	//	{
	//		return e_error_code_item_godassembly;
	//	}

	//	materialTypeInt = materialTypeInt / 2;
	//	for (int32 i = 2; i < materialTypeInt; i++)
	//	{
	//		if (get_item_count(e_bag_type_bag, materialList[i * 2]) < materialList[i * 2 + 1])
	//		{
	//			return e_error_code_item_godassembly;
	//		}
	//	}

	//	temp_player.cut_money((e_money_type)materialList[0], materialList[1], e_server_log_cut_money_item_assembly, item_ptr->get_data_info(e_item_info_info_id));
	//	temp_player.cut_money((e_money_type)materialList[2], materialList[3], e_server_log_cut_money_item_assembly, item_ptr->get_data_info(e_item_info_info_id));

	//	for (int32 i = 2; i < materialTypeInt; i++)
	//	{
	//		if (first_use_lock == 1 && get_item_count(e_bag_type_bag, materialList[i * 2], 1) > 0)
	//		{
	//			is_use_lock_material = true;
	//		}
	//		else if (get_item_count(e_bag_type_bag, materialList[i * 2], 0) < materialList[i * 2 + 1])
	//			is_use_lock_material = true;
	//		}
	//		cost_item_by_id_with_lock_states(e_server_log_del_item_assembly, 0, materialList[i * 2], materialList[i * 2 + 1], first_use_lock);
	//	}
	//}
	//else
	//{
	//	return e_error_code_item_godassembly;
	//}

	//if (rand() % 100 < (equiptemplate_ptr->AdvanceRadio * 100))
	//{
	//	int32 upgrade = item_ptr->get_data_info(e_item_info_upgrade_count);
	//	int32 addon = item_ptr->get_data_info(e_item_info_add_on);
	//	int32 islock = item_ptr->get_data_info(e_item_info_locked);

	//	std::vector<int32> succinct_property;

	//	int32 property_start = int32(e_item_info_succinct_property1);
	//	int32 property_end = int32(e_item_info_succinct_property6);
	//	{
	//		succinct_property.push_back(int32(item_ptr->get_data_info((e_item_info)property_start)));
	//	}

	//	int32 advanced_id = item_ptr->get_item_info_ptr()->advanced_id;
	//	del_item(e_server_log_del_item_assembly_cost, 0, item_guid, 1);

	//	int32 temp_id = 0;
	//	int32 temp_num = 0;
	//	int32 temp_locked = 0;
	//	{
	//		citem* new_item = create_item_by_template(e_server_log_add_item_item_assembly, 0, advanced_id, 1, 0);
	//		if (!new_item)
	//		{
	//			return e_error_code_item_godassembly;
	//		}
	//		new_item->set_data_info(e_item_info_upgrade_count, upgrade);
	//		new_item->set_data_info(e_item_info_add_on, addon);
	//		new_item->set_data_info(e_item_info_locked, islock);
	//		if (is_use_lock_material)
	//		{
	//			new_item->set_data_info(e_item_info_locked, 1);
	//		}
	//		for (int32 i = 0; i < succinct_property.size(); i++)
	//		{
	//			new_item->set_data_info((e_item_info)(int32(e_item_info_succinct_property1) + i), succinct_property[i]);
	//		}

	//		temp_id = new_item->get_data_info(e_item_info_info_id);
	//		temp_num = new_item->get_data_info(e_item_info_stack_count);
	//		temp_locked = new_item->get_data_info(e_item_info_locked);
	//		put_in_bag(new_item);
	//	}
	//	std::vector<s_item_template_info> item_data;
	//	item_data.push_back({ temp_id ,temp_num ,temp_locked });
	//	get_item_send_promp_msg_to_client(item_data);
	//	return e_error_code_success;
	//}
	return e_error_code_item_godassembly;
}
int32 item_set::item_assembled(const guid_64& item_guid, int32 first_use_lock)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_error_code_item_godassembled;
	}
	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr)
	{
		return e_error_code_item_godassembled;
	}
	ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
	if (item_template_ptr == nullptr)
	{
		return e_error_code_item_godassembled;
	}
	if (item_template_ptr->god_assembled <= 0)
	{
		return e_error_code_item_godassembled;
	}
	EquipTemplate* equiptemplate_ptr = item_ptr->get_equip_info_ptr();
	if (equiptemplate_ptr == nullptr)
	{
		return e_error_code_item_godassembled;
	}
	std::vector<int32> need_money_list;
	std::vector<int32> need_material_list;
	for (int32 j = 0; j < equiptemplate_ptr->GodAssembledSpend.size() / 2; j++)
	{
		if ((equiptemplate_ptr->GodAssembledSpend[j * 2] >= 0) && (equiptemplate_ptr->GodAssembledSpend[j * 2] < faith::e_money_type_max))
		{
			need_money_list.push_back(equiptemplate_ptr->GodAssembledSpend[j * 2]);
			need_money_list.push_back(equiptemplate_ptr->GodAssembledSpend[j * 2 + 1]);
		}
		else
		{
			need_material_list.push_back(equiptemplate_ptr->GodAssembledSpend[j * 2]);
			need_material_list.push_back(equiptemplate_ptr->GodAssembledSpend[j * 2 + 1]);
		}
	}
	if (need_money_list.size() >= 2)
	{
		if (!temp_player.can_cut_money((e_money_type)need_money_list[0], need_money_list[1]))
		{
			return e_error_code_item_godassembled;
		}
	}

	int32   materialTypeInt = need_material_list.size();
	if (materialTypeInt < 2 || materialTypeInt % 2 != 0)
	{
		return e_error_code_item_godassembled;
	}
	materialTypeInt = materialTypeInt / 2;
	for (int32 i = 0; i < materialTypeInt; i++)
	{
		if (get_item_count(e_bag_type_bag, need_material_list[i * 2]) < need_material_list[i * 2 + 1])
		{
			return e_error_code_item_godassembled;
		}
	}
	if (need_money_list.size() >= 2)
	{
		temp_player.cut_money((e_money_type)need_money_list[0], need_money_list[1], e_server_log_cut_money_item_assembled, item_ptr->get_data_info(e_item_info_info_id));
	}

	bool is_use_lock_material = false;
	for (int32 i = 0; i < materialTypeInt; i++)
	{
		if (first_use_lock == 1 && get_item_count(e_bag_type_bag, need_material_list[i * 2], 1) > 0)
		{
			is_use_lock_material = true;
		}
		else if (get_item_count(e_bag_type_bag, need_material_list[i * 2], 0) < need_material_list[i * 2 + 1])
		{
			is_use_lock_material = true;
		}
		cost_item_by_id_with_lock_states(e_server_log_del_item_assembled, 0, need_material_list[i * 2], need_material_list[i * 2 + 1], first_use_lock);
	}
	if (rand() % 100 < (equiptemplate_ptr->GodAssembledRadio * 100))
	{
		int32 upgrade = item_ptr->get_data_info(e_item_info_upgrade_count);
		int32 addon = item_ptr->get_data_info(e_item_info_add_on);
		int32 islock = item_ptr->get_data_info(e_item_info_locked);

		std::vector<int32> succinct_property;
		int32 property_start = int32(e_item_info_succinct_property1);
		int32 property_end = int32(e_item_info_succinct_property6);
		for (; property_start <= property_end; property_start++)
		{
			succinct_property.push_back(int32(item_ptr->get_data_info((e_item_info)property_start)));
		}
		std::vector<int32> jewel_array;
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
		{
			int32 temp_jewel_id = item_ptr->get_data_info((e_item_info)i);
			if (temp_jewel_id > 0)
			{
				jewel_array.push_back(temp_jewel_id);
			}
		}

		int32 enchant_level = item_ptr->get_data_info(e_item_info_illusion_had_byte);
		int32 awaken_level = item_ptr->get_data_info(e_item_info_wing_exp);
		int32 succinct_level = item_ptr->get_data_info(e_item_info_succinct_level);

		std::vector<int32> jewel_carve_vec;

		for (int32 i = e_item_info_jewel_carve_0; i <= e_item_info_jewel_carve_2; i++)
		{
			int32 temp_skill_id = item_ptr->get_data_info((e_item_info)i);
			jewel_carve_vec.push_back(temp_skill_id);
		}


		int32 temp_id = 0;
		int32 temp_num = 0;
		int32 temp_locked = 0;
		citem* new_item = create_item_by_template(e_server_log_add_item_item_assembled, 0, item_template_ptr->god_assembled_id, 1, 0);
		if (new_item == nullptr)
		{
			return e_error_code_item_godassembled;
		}
		new_item->set_data_info(e_item_info_upgrade_count, upgrade);
		new_item->set_data_info(e_item_info_add_on, addon);
		new_item->set_data_info(e_item_info_locked, islock);
		jewel_array_set_to_item(new_item, jewel_array, true);
		if (is_use_lock_material)
		{
			new_item->set_data_info(e_item_info_locked, 1);
		}
		for (int32 i = 0; i < succinct_property.size(); i++)
		{
			new_item->set_data_info((e_item_info)(int32(e_item_info_succinct_property1) + i), succinct_property[i]);
		}
		for (int32 i = e_item_info_random_had_flag; i <= e_item_info_random_property6; ++i)
		{
			new_item->set_data_info(e_item_info(i), item_ptr->get_item_inst().data_ary[i]);
		}

		for (int32 i = 0; i < succinct_property.size(); i++)
		{
			new_item->set_data_info((e_item_info)(int32(e_item_info_succinct_property1) + i), succinct_property[i]);
		}

		new_item->set_data_info(e_item_info_illusion_had_byte, enchant_level);
		new_item->set_data_info(e_item_info_wing_exp, awaken_level);
		new_item->set_data_info(e_item_info_succinct_level, succinct_level);


		for (int32 i = 0; i < jewel_carve_vec.size(); i++)
		{
			new_item->set_data_info((e_item_info)(int32(e_item_info_jewel_carve_0) + i), jewel_carve_vec[i]);
		}

		temp_id = new_item->get_data_info(e_item_info_info_id);
		temp_num = new_item->get_data_info(e_item_info_stack_count);
		temp_locked = new_item->get_data_info(e_item_info_locked);
		put_in_bag(new_item);
		std::vector<s_item_template_info> item_data;
		item_data.push_back({ temp_id ,temp_num,temp_locked });
		get_item_send_promp_msg_to_client(item_data);
		ItemTemplate* new_item_template_ptr = new_item->get_item_info_ptr();
		if (nullptr != new_item_template_ptr)
		{
			s_chat_notice_info 	notice_data;
			notice_data.item_data.push_back(new_item->get_item_inst());
			int32 notice_id = 93000251;
			temp_player.get_chat_mgr().send_notice_new(notice_id, notice_data);
		}

		del_item(e_server_log_del_item_assembled_cost, 0, item_guid, 1);

		return e_error_code_success;
	}
	return e_error_code_item_godassembled;
}

e_error_code item_set::item_use_check(citem& item_ref, int32& use_num)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_error_code_no_template;
	}

	if (!is_in_duration(item_ref.get_item_guid()))
	{
		return e_error_code_item_over_time;
	}

	if (use_num <= 0 || use_num > item_ref.get_data_info(e_item_info_stack_count))
	{
		return e_error_code_item_invalid_use_num;
	}

	ItemTemplate* item_template_ptr = item_ref.get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return e_error_code_item_invalid_item_template;
	}


	int32 player_grade_num = temp_player.get_grade_num();
	if (player_grade_num < item_template_ptr->grade_level_limit)
	{
		return	e_error_code_item_invalid_player_state;
	}

	int32 player_class_type = temp_player.get_unit_info(e_role_info_class_type);
	if (item_ref.can_used_by_class_type((e_class_type)player_class_type) == false)
	{
		return e_error_code_item_class_type_limit;
	}


	int32 player_level = temp_player.get_unit_info(e_role_info_exp_level);
	if (item_template_ptr->level_limit.size() >= 2)
	{
		if (player_level < item_template_ptr->level_limit[0] || player_level > item_template_ptr->level_limit[1])
		{
			return e_error_code_item_level_limit;
		}
	}

	PropBasicAttributeTemplate* prop_template_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, item_template_ptr->logic_id);
	if (nullptr == prop_template_ptr)
	{
		return e_error_code_item_invalid_prop_template;
	}
	if (prop_template_ptr->RoleAttributeChange.size() % e_att_one_max != 0
		|| prop_template_ptr->MoneyChange.size() % 2 != 0)
	{
		return e_error_code_item_invalid_prop_template;
	}

	if (prop_template_ptr->OnlyUseInSelfServer && temp_player.is_self_server() == false)
	{
		temp_player.send_notice("90305139");
		return	e_error_code_item_invalid_player_state;
	}

	if (prop_template_ptr->AddHeartValue && temp_player.had_couple() == false)
	{
		temp_player.send_notice("90096889");
		return	e_error_code_item_invalid_player_state;
	}

	int32 title_template_id = prop_template_ptr->TitleGet;
	if (title_template_id > 0)
	{
		ctitle_mgr& title_mgr = temp_player.get_title_mgr();
		TitleTemplate* title_template_ptr = GET_TEMPLATE(TitleTemplate, title_template_id);
		if (title_template_ptr == nullptr)
		{
			return e_error_code_item_invalid_prop_template;
		}

		if ((title_mgr.is_have_title(title_template_id) == true) && (title_template_ptr->LastTime > 0))
		{
			return e_error_code_item_readd_title;
		}
	}

	if (m_item_use_cd.find(item_template_ptr->attribute_id) != m_item_use_cd.end())
	{
		return e_error_code_item_in_cd;
	}


	e_fruit_type fruit_type = item_ref.is_fruit();
	if (e_fruit_type_none != fruit_type)
	{
		int32 fruit_cur_eated, fruid_can_eat_max;
		temp_player.get_fruit_eating_info(fruit_type, fruit_cur_eated, fruid_can_eat_max);

		int32 can_eat_now = fruid_can_eat_max - fruit_cur_eated;
		if (can_eat_now < 0)
		{
			can_eat_now = 0;
		}
		if (use_num > can_eat_now)
		{
			use_num = can_eat_now;
			return e_error_code_item_fruit_upper_limit;
		}
	}

	return e_error_code_success;
}

bool item_set::item_use(const guid_64& item_guid, citem*& end_item, int32 item_num, int32 force_use, bool is_cotinue_use, int32 item_slot)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false || temp_player.m_cur_map_template_ptr == nullptr)
	{
		CONSOLE_ERROR("m_unit_array_index:{}", m_unit_array_index);
		return false;
	}
	/*if (temp_player.is_dead())
	{
	return false;
	}*/
	if (!item_guid.is_valid() || 0 == item_num)
	{
		CONSOLE_ERROR("item_num:{}", item_num);
		return false;
	}
	citem* item_ptr = get_item(item_guid.server_64);
	if (nullptr == item_ptr)
	{
		CONSOLE_ERROR("item_guid:{}", item_guid.server_64);
		return false;
	}
	int32 item_id = item_ptr->get_item_info_ptr()->attribute_id;
	ItemTemplate* real_item_template_ptr = item_ptr->get_item_info_ptr();
	if (real_item_template_ptr == nullptr)
	{
		CONSOLE_ERROR("item_id:{}", item_id);
		return false;
	}
	PropBasicAttributeTemplate* prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, real_item_template_ptr->logic_id);
	if (prop_template == nullptr)
	{
		CONSOLE_ERROR("item_id:{}", item_id);
		return false;
	}
	if (prop_template->UseType == e_item_use_type_can_not_use
		|| prop_template->UseType == e_item_use_type_only_jump)
	{
		CONSOLE_ERROR("item_id:{}", item_id);
		return false;
	}
	for (int32 i = 0; i < real_item_template_ptr->buff_trigger_id.size(); ++i)
	{
		BuffDataTemplate* temp_buff_template = GET_TEMPLATE(BuffDataTemplate, real_item_template_ptr->buff_trigger_id[i]);
		if (temp_buff_template == nullptr)
		{
			continue;
		}
		for (int32 j = 0; j < temp_buff_template->InvalidMapTypeArray.size(); ++j)
		{
			if (temp_buff_template->InvalidMapTypeArray[j] == temp_player.m_cur_map_template_ptr->Type)
			{
				CONSOLE_ERROR("item_id:{}", item_id);
				return false;
			}
		}
	}

	e_error_code item_use_check_ret = item_use_check(*item_ptr, item_num);

	item_proto_item_use_end item_use_end_msg;
	item_use_end_msg.set_result(item_use_check_ret);
	item_use_end_msg.set_item_template_id(item_id);

	switch (item_use_check_ret)
	{
	case e_error_code_success:
		break;

	case e_error_code_item_fruit_upper_limit:
		if (item_num <= 0)
		{
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		break;

	default:
		temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
		CONSOLE_ERROR("item_id:{}", item_id);
		return false;
	}

	int32 drop_template_id = prop_template->ItemGet;
	int32 has_opened_package_num = 0;
	vector<s_item_template_info> get_item_list;
	get_item_list.clear();
	e_error_code item_use_check_result = e_error_code_success;
	switch (real_item_template_ptr->sub_type)
	{
	case e_prop_sub_type_gife:
	{
		has_opened_package_num = open_package_bag(prop_template->ItemGet, item_num, item_use_check_result, get_item_list, real_item_template_ptr->Notice_id, real_item_template_ptr);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	}
	case e_prop_sub_type_choose_gife:
	{
		has_opened_package_num = open_choose_package_bag(real_item_template_ptr->logic_id, item_num, item_use_check_result, get_item_list, real_item_template_ptr->Notice_id, item_slot, real_item_template_ptr);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	}
	case e_prop_sub_type_element_package:
	{
		has_opened_package_num = open_package_elementbag(prop_template->ItemGet, item_num, item_use_check_result, get_item_list, real_item_template_ptr->Notice_id);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		temp_player.get_item_set().tidy_item(e_bag_type_elemenet);
		break;
	}
	case e_prop_sub_type_mount_illusion_unlock:
	case e_prop_sub_type_mount_shap_card:
	{
		int32 over_time = 0;
		if (prop_template->EffectiveTime > 0)
		{
			time_info cur_time = time_helper::get_cur_time_new();
			int32 cur_second_time = cur_time.second;
			over_time = cur_second_time + prop_template->EffectiveTime;
		}
		has_opened_package_num = open_package_mountbag(prop_template->ItemGet, item_use_end_msg, over_time);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	}
	case e_prop_sub_type_wing_illusion_unlock:
	case e_prop_sub_type_wing_shap_card:
	{
		has_opened_package_num = open_package_wingbag(prop_template->ItemGet, item_use_end_msg);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	}
	case e_prop_sub_type_rand_equip:
	{
		has_opened_package_num = open_rand_equip_package_bag(prop_template->ItemGet, item_num, item_use_check_result, get_item_list);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	}
	case e_prop_sub_type_diamonds_package:
	{
		has_opened_package_num = item_buy_and_use(item_id, item_num, item_use_check_result, get_item_list);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
	}
	break;
	case e_prop_sub_type_time_limit_prop:
		has_opened_package_num = open_package_time_limit_prop(prop_template->ItemGet, item_use_end_msg, prop_template->EffectiveTime);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	case e_prop_sub_type_recharge_package:
	{
		RechargeTemplate* RechargeTemplatePtr = template_manager::get_instance().get_template_by_recharge_offline_item(item_id);
		if (RechargeTemplatePtr == nullptr)
		{
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		if (item_num > item_ptr->get_data_info(e_item_info_stack_count))
		{
			item_num = item_ptr->get_data_info(e_item_info_stack_count);
		}
		int32 temp_payment_type = atoi(temp_player.get_third_info().param14);
		temp_player.recharge_by_template_id(RechargeTemplatePtr->attribute_id, item_ptr->get_item_inst().param_str, 0, temp_payment_type, RechargeTemplatePtr->Price, item_num);
	}
	break;
	case e_prop_sub_type_vip_experience_card:
	{
		int32 cur_vip_experience_level = temp_player.get_unit_info(e_role_info_vip_experience_level);
		int32 cur_vip_level = temp_player.get_vip_level();
		if (cur_vip_experience_level > prop_template->ExperienceVipLevel)
		{
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		else if (cur_vip_experience_level == prop_template->ExperienceVipLevel)
		{
			int64 cur_end_time = temp_player.get_time_data(e_time_type_vip_experience_level_time);
			int64 time_add_value = prop_template->EffectiveTime * second_tick_time;
			cur_end_time += time_add_value;
			temp_player.set_time_data(e_time_type_vip_experience_level_time, cur_end_time);
			temp_player.send_time_one(e_time_type_vip_experience_level_time);
		}
		else
		{
			temp_player.set_unit_info(e_role_info_vip_experience_level, prop_template->ExperienceVipLevel);
			temp_player.send_info_one(e_role_info_vip_experience_level);

			temp_player.set_time_data(e_time_type_vip_experience_level_time, init_unit::get_end_time(prop_template->EffectiveTime));
			temp_player.send_time_one(e_time_type_vip_experience_level_time);

			if (cur_vip_level < prop_template->ExperienceVipLevel)
			{
				temp_player.refresh_vip_title();
				temp_player.vip_buff_change(cur_vip_experience_level, false);
				temp_player.vip_buff_change(prop_template->ExperienceVipLevel, true);
			}
		}
		open_package_bag(prop_template->ItemGet, item_num, item_use_check_result, get_item_list, real_item_template_ptr->Notice_id, real_item_template_ptr);
	}
	break;
	case e_prop_sub_type_const_att_equip:
	{
		has_opened_package_num = open_const_att_equip_bag(prop_template->ItemGet, item_num, item_use_check_result, get_item_list, end_item);
		item_use_end_msg.set_result(item_use_check_result);
		item_num = has_opened_package_num < item_num ? has_opened_package_num : item_num;
		break;
	}
	break;
	default:
		break;
	}

	//if (get_item_list.empty() == false)
	//{
	//	get_item_send_promp_msg_to_client(get_item_list, is_cotinue_use);
	//}

	int32 len = prop_template->RoleAttributeChange.size();
	if (len > 0 && len % e_att_one_max == 0)
	{
		for (int32 i = 0; i < len; i += e_att_one_max)
		{
			int32 att_id = prop_template->RoleAttributeChange[i + e_att_one_att_id];
			int32 att_value = prop_template->RoleAttributeChange[i + e_att_one_value] * item_num;
			temp_player.add_att_permanent(att_id, att_value);
		}
	}

	item_ptr->set_unit_buff(m_unit_array_index, true);

	len = prop_template->MoneyChange.size();
	vector<int32> get_money_list;
	if (len > 0)
	{
		add_money_by_card(prop_template->MoneyChange, item_num, get_money_list);
	}

	temp_player.get_title_mgr().add_title_by_template_id(prop_template->TitleGet);

	if (prop_template->UseCD > 0)
	{
		m_item_use_cd.insert({ item_id, init_unit::get_end_time(prop_template->UseCD) });
	}

	if (e_prop_sub_type_add_exp_with_num == real_item_template_ptr->sub_type)
	{
		float exp_fix_num = prop_template->ExpNum;
		if (exp_fix_num >= .0f)
		{
			//if (true == temp_player.can_add_exp())
			{
				int32 upgrade_temp_id = temp_player.get_unit_info(e_role_info_upgrade_id);
				PlayerUpgradeTemplate* player_upgrade_temp = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_temp_id);
				if (nullptr == player_upgrade_temp)
				{
					item_use_end_msg.set_result(e_error_code_item_invalid_prop_template);
					CONSOLE_ERROR("item_id:{}", item_id);
					return false;
				}
				int64 exp_num = (int64)(player_upgrade_temp->BaseExp) * exp_fix_num * (int64)item_num;

				exp_num = temp_player.add_exp(exp_num, true);

				character_proto_character_get_money character_get_money;
				std::vector<int32> money_type_list;
				std::vector<xstring> money_num_list;
				money_type_list.clear();
				money_type_list.push_back((int32)e_money_type_exp);
				money_num_list.clear();
				money_num_list.push_back(init_unit::change_i64_to_string(exp_num));
				if (money_type_list.size() == money_num_list.size())
				{
					for (int32 i = 0; i < money_type_list.size(); i++)
					{
						character_get_money.add_money_type_array(money_type_list[i]);
						character_get_money.add_money_num_array(money_num_list[i]);
					}
					temp_player.send_message_to_self(&character_get_money, e_msgindex_s2c_character_get_money);
					item_use_end_msg.set_result(e_error_code_success);
				}
			}
			//else
			//{
			//	item_use_end_msg.set_result(e_error_code_item_level_limit);
			//	return false;
			//}
		}
		else
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_prop_template);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
	}

	if (e_prop_sub_type_max_level_up_medicine == real_item_template_ptr->sub_type
		|| e_prop_sub_type_mid_level_up_medicine == real_item_template_ptr->sub_type
		|| e_prop_sub_type_min_level_up_medicine == real_item_template_ptr->sub_type)
	{
		int32 level_array_size = prop_template->LevelUp.size();
		if (3 == level_array_size)
		{
			//if (true == temp_player.can_add_exp())
			{
				int32 old_level = temp_player.get_unit_info(e_role_info_exp_level);
				//int64 exp_num = 0;
				if (old_level < prop_template->LevelUp[0])
				{
					if (old_level >= init_unit::get_max_player_level())
					{
						CONSOLE_ERROR("item_id:{}", item_id);
						return false;
					}
					if (temp_player.set_level_up_exp() == false)
					{
						CONSOLE_ERROR("item_id:{}", item_id);
						return false;
					}
					//temp_player.gm_set_level_and_grade(old_level + (prop_template->LevelUp[1] * item_num));
					//temp_player.set_money_data(e_money_type_exp, 0);

				}
				else
				{
					int64 exp_num = (int64)prop_template->LevelUp[2] * (int64)item_num;

					exp_num = temp_player.add_exp(exp_num, true);

					character_proto_character_get_money character_get_money;
					std::vector<int32> money_type_list;
					std::vector<xstring> money_num_list;
					money_type_list.clear();
					money_type_list.push_back((int32)e_money_type_exp);
					money_num_list.clear();
					money_num_list.push_back(init_unit::change_i64_to_string(exp_num));
					if (money_type_list.size() == money_num_list.size())
					{
						for (int32 i = 0; i < money_type_list.size(); i++)
						{
							character_get_money.add_money_type_array(money_type_list[i]);
							character_get_money.add_money_num_array(money_num_list[i]);
						}
						temp_player.send_message_to_self(&character_get_money, e_msgindex_s2c_character_get_money);
					}
				}
				//exp_num = temp_player.add_exp(exp_num, true);

				//character_proto_character_get_money character_get_money;
				//std::vector<int32> get_money_list;
				//get_money_list.clear();
				//get_money_list.push_back((int32)e_money_type_exp);
				//get_money_list.push_back(exp_num);
				//for (int32 i = 0; i < get_money_list.size(); i++)
				//{
				//	character_get_money.add_money_array(get_money_list[i]);
				//}
				//temp_player.send_message_to_self(&character_get_money, e_msgindex_s2c_character_get_money);
				//}
				//}
				item_use_end_msg.set_result(e_error_code_success);
			}
			//else
			//{
			//	item_use_end_msg.set_result(e_error_code_item_level_limit);
			//	return false;
			//}
		}
		else
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_prop_template);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
	}

	if (e_prop_sub_type_wash_talent_water == real_item_template_ptr->sub_type)
	{
		player& ref_player = unit_man::get_player(m_unit_array_index);
		if (false == ref_player.is_valid())
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		item_use_end_msg.set_result(e_error_code_success);
		ref_player.get_talent_mgr().talent_reset();
	}

	if (e_prop_sub_type_refresh_body_att == real_item_template_ptr->sub_type)
	{
		player& ref_player = unit_man::get_player(m_unit_array_index);
		if (false == ref_player.is_valid())
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		item_use_end_msg.set_result(e_error_code_success);
		std::vector<int32> init_body_att = { 0,0,0,0 };
		ref_player.get_pawn_att().change_base_body_att(init_body_att, true);
	}

	if (e_prop_sub_type_add_map_count == real_item_template_ptr->sub_type)
	{
		player& ref_player = unit_man::get_player(m_unit_array_index);
		if (false == ref_player.is_valid())
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		bool use_result = ref_player.add_map_count_with_things(prop_template->AddMapCount, item_num);
		if (false == use_result)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		else
		{
			item_use_end_msg.set_result(e_error_code_success);
		}

	}

	if (e_prop_sub_type_add_map_time == real_item_template_ptr->sub_type)
	{
		player& ref_player = unit_man::get_player(m_unit_array_index);
		if (false == ref_player.is_valid())
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		bool use_result = ref_player.add_map_time_with_things(prop_template->AddMapTimeArr, item_num);
		if (false == use_result)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		else
		{
			item_use_end_msg.set_result(e_error_code_success);
		}
	}

	//ϴpkֵ
	if (e_prop_sub_type_reduce_Pk_value == real_item_template_ptr->sub_type)
	{
		int32 reduce_array_size = prop_template->ReducePkValue.size();
		if (reduce_array_size == 2)
		{
			bool is_cut_down = false;
			if (temp_player.get_pk_community_mgr().get_pk_value() > 0)
			{
				int32 reduce_value = temp_player.get_pk_community_mgr().cut_pk_value(prop_template->ReducePkValue[1]);
				if (reduce_value > 0)
				{
					item_use_end_msg.set_result(e_error_code_success);
					item_use_end_msg.set_effect_value(reduce_value);
					is_cut_down = true;
				}
			}
			if (false == is_cut_down)
			{
				item_use_end_msg.set_effect_value(0);
				item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
				temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
				CONSOLE_ERROR("item_id:{}", item_id);
				return false;
			}
		}
		else
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_prop_template);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
	}

	if (e_prop_sub_type_meditation_medicine == real_item_template_ptr->sub_type)
	{
		player& ref_player = unit_man::get_player(m_unit_array_index);
		if (false == ref_player.is_valid())
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_player_state);
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}

		int64 MeditationTime = ref_player.get_time_data(e_time_type_meditation_remaining_time) + (prop_template->MeditationMedicineTime * second_tick_time * item_num);
		int32 meditation_time_max = GAMECONFIG->MeditationTimeMax * second_tick_time;
		MeditationTime = MeditationTime > meditation_time_max ? meditation_time_max : MeditationTime;
		ref_player.set_time_data(e_time_type_meditation_remaining_time, MeditationTime);
		ref_player.send_time_one(e_time_type_meditation_remaining_time);
		item_use_end_msg.set_result(e_error_code_success);
	}
	if (e_prop_sub_type_add_already_time == real_item_template_ptr->sub_type)
	{
		int32 cur_kill_num = temp_player.get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
		if (cur_kill_num <= 0)
		{
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		temp_player.get_gain_treasure_mgr().set_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss, cur_kill_num - 1);
		temp_player.get_gain_treasure_mgr().send_gain_treasure_info_one(e_gain_treasure_info_type_killed_field_boss);
		item_use_end_msg.set_result(e_error_code_success);
	}
	if (e_prop_sub_type_add_single_boss_times == real_item_template_ptr->sub_type)
	{
		int32 cur_kill_num = temp_player.get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss);
		if (cur_kill_num <= 0)
		{
			CONSOLE_ERROR("item_id:{}", item_id);
			return false;
		}
		temp_player.get_gain_treasure_mgr().set_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss, cur_kill_num - 1);
		temp_player.get_gain_treasure_mgr().send_gain_treasure_info_one(e_gain_treasure_info_type_killed_simple_boss);
		item_use_end_msg.set_result(e_error_code_success);
	}
	if (prop_template->AddHeartValue)
	{
		temp_player.get_marry_mgr().add_heart_value(prop_template->AddHeartValue * item_num);
	}

	if (prop_template->SummonNpcID)
	{
		world_cs::spawn_npc(prop_template->SummonNpcID, temp_player.get_new_map_pos(), temp_player.get_map_ent(), &temp_player);
	}

	int32 vip_card_add_point = prop_template->GetRechargeNum * item_num;
	if (vip_card_add_point > 0)
	{
		int32 cur_vip_level = temp_player.get_vip_level(false);
		int32 cur_vip_buff_level = temp_player.get_vip_level(true);
		int32 cur_vip_recharge_num = temp_player.get_unit_info(e_role_info_recharge_num);
		int32 cur_daily_recharge_num = temp_player.get_unit_info(e_role_info_daily_recharge_num);
		//int64 cur_vip_point_num = temp_player.get_unit_info(e_role_info_total_diamond_cost_num);
		//temp_player.set_unit_info(e_role_info_total_diamond_cost_num, cur_vip_point_num + vip_card_add_point);
		//temp_player.send_info_one(e_role_info_total_diamond_cost_num);
		temp_player.set_unit_info(e_role_info_recharge_num, vip_card_add_point + cur_vip_recharge_num);
		temp_player.set_unit_info(e_role_info_daily_recharge_num, cur_daily_recharge_num + vip_card_add_point);

		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_charge, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().reset_ranking_value(e_time_limit_activity_type_total_charge);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_cross_total_charge, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().reset_ranking_value(e_time_limit_activity_type_cross_total_charge);

		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge2, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge3, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge4, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge5, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge6, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_charge2, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_charge_1, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_charge_2, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_charge_3, vip_card_add_point);

		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_1, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_2, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_3, vip_card_add_point);
		temp_player.get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_4, vip_card_add_point);


		temp_player.calculate_vip_level();
		int32 new_vip_level = temp_player.get_vip_level(false);
		if (new_vip_level != cur_vip_level)
		{
			if (new_vip_level > cur_vip_buff_level)
			{
				temp_player.vip_buff_change(cur_vip_buff_level, false);
				temp_player.vip_buff_change(new_vip_level, true);
				temp_player.refresh_vip_title();
			}
			temp_player.broadcast_info_one(e_role_info_vip_level);
			temp_player.sync_data_to_ws(e_sync_cs2ws_data_vip_level, new_vip_level);
		}

		temp_player.send_info_one(e_role_info_recharge_num);
		temp_player.send_info_one(e_role_info_daily_recharge_num);
	}

	del_item(e_server_log_del_item_item_use, 0, item_ptr, item_num);
	item_use_end_msg.set_use_num(item_num);
	temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);

	return item_use_check_result == e_error_code_success;
}

int32 item_set::item_buy_and_use(int32 item_id, int32 item_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (!temp_player.is_valid())
	{
		return -1;
	}

	ItemTemplate* real_item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);;
	if (real_item_template_ptr == nullptr)
	{
		return -1;
	}

	PropBasicAttributeTemplate* prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, real_item_template_ptr->logic_id);
	if (prop_template == nullptr)
	{
		return -1;
	}

	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	int32 open_package_result = open_package_bag(prop_template->ItemGet, item_num, item_use_end_result, get_item_list, real_item_template_ptr->Notice_id, real_item_template_ptr);
	if (open_package_result == 0)
	{
		return -1;
	}
	vector<int32> goods_money_list;
	for (int32 i = 0; i < real_item_template_ptr->goods_id.size(); i++)
	{
		GoodsTemplate* temp_goods_template = GET_TEMPLATE(GoodsTemplate, real_item_template_ptr->goods_id[i]);
		if (temp_goods_template != nullptr
			&& temp_goods_template->NewPrice.size() > region_money_num_index)
		{
			goods_money_list.push_back(temp_goods_template->NewPrice[region_money_type_index]);
			goods_money_list.push_back(temp_goods_template->NewPrice[region_money_num_index] * item_num);
		}
	}

	if (goods_money_list.size() == 0 || goods_money_list.size() % 2 != 0)
	{
		return -1;
	}

	for (int32 i = 0; i < goods_money_list.size(); i += 2)
	{
		if (temp_player.can_cut_money((e_money_type)goods_money_list[i], goods_money_list[i + 1]) == false)
		{
			return -1;
		}
	}

	for (int32 i = 0; i < goods_money_list.size(); i += 2)
	{
		temp_player.cut_money((e_money_type)goods_money_list[i], goods_money_list[i + 1], e_server_log_cut_money_item_package_buy, item_id, item_num);
	}

	return open_package_result;
}

int32 item_set::item_buy_and_use(int32 goods_id)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (!temp_player.is_valid())
	{
		return false;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	GoodsTemplate* goods_template = GET_TEMPLATE(GoodsTemplate, goods_id);
	if (goods_template == nullptr
		|| goods_template->NewPrice.size() < region_money_num_index)
	{
		return false;
	}
	ItemTemplate* temp_item_ptr = GET_TEMPLATE(ItemTemplate, goods_template->ItemId);
	if (temp_item_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	int32 player_level = temp_player.get_unit_info(e_role_info_exp_level);
	if (goods_template->Levellimit.size() < 2)
	{
		return  e_item_string_level;
	}

	if (player_level < goods_template->Levellimit[0] || player_level > goods_template->Levellimit[1])
	{
		if (temp_item_ptr->item_type == e_item_type_expendable &&
			(temp_item_ptr->sub_type == e_prop_sub_type_hp_min || temp_item_ptr->sub_type == e_prop_sub_type_hp_mld || temp_item_ptr->sub_type == e_prop_sub_type_hp_max
				|| temp_item_ptr->sub_type == e_prop_sub_type_hp_shop || temp_item_ptr->sub_type == e_prop_sub_type_hp_essence))
		{
			return e_item_string_not_belong_level;
		}
		else
		{
			return e_item_string_level;
		}

	}


	if (!temp_player.can_cut_money(e_money_type(goods_template->NewPrice[region_money_type_index]), goods_template->NewPrice[region_money_num_index]))
	{
		return false;
	}
	temp_player.cut_money((e_money_type)goods_template->NewPrice[region_money_type_index], goods_template->NewPrice[region_money_num_index], e_server_log_cut_money_item_medical_buy, goods_template->NewPrice[region_money_type_index]);

	citem* temp_item = temp_player.get_item_set().create_item_by_template(e_server_log_add_item_buy_goods, goods_template->NewPrice[region_money_type_index], goods_template->ItemId, goods_template->GoodsNum, goods_template->IsLock);
	if (temp_item == nullptr)
	{
		return e_item_string_unkown;
	}


	faith::goods_proto_goods_operate_end msg;
	msg.set_goods_id(goods_id);
	msg.set_goods_num(goods_template->GoodsNum);
	msg.set_store_id(e_store_type_medicine);
	msg.set_operation_end_state(0);
	msg.set_is_auto_buy(false);
	temp_player.send_message_to_self(&msg, e_msgindex_s2c_goods_operate);
	citem* end_item = nullptr;
	put_in_bag(temp_item);

	return true;
}

bool item_set::use_beast_spirit(const guid_64& item_guid, int32 item_num)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("beast_spirit", "use_beast_spirit", 1, false, "%d%s%d>%d", m_unit_array_index, item_guid.to_string().c_str(), item_num, &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (!temp_player.is_valid())
	{
		return false;
	}

	citem* item_ptr = get_item(item_guid);
	if (nullptr == item_ptr || item_ptr->get_data_info(e_item_info_container_type) != e_bag_type_bag)
	{
		return false;
	}

	if (item_ptr->get_data_info(e_item_info_stack_count) < item_num || item_num <= 0)
	{
		return false;
	}

	BeastSpiritTemplate* beast_spirit_template_ptr = item_ptr->get_beast_spirit_template_ptr();
	if (nullptr == beast_spirit_template_ptr)
	{
		return false;
	}

	bool is_upgrade = false;
	for (int32 i = 0; i < beast_spirit_template_ptr->UnlockConditionArray.size(); i += 2)
	{
		citem* item_mount_or_wing_ptr = get_item_by_template_id(beast_spirit_template_ptr->UnlockConditionArray[i]);
		if (nullptr == item_mount_or_wing_ptr)
		{
			continue;
		}

		if (is_class_enable(item_mount_or_wing_ptr))
		{
			if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_mount)
			{
				MountTemplate* mount_template_ptr = item_mount_or_wing_ptr->get_mount_template_ptr();
				if (nullptr == mount_template_ptr)
				{
					continue;
				}

				if (mount_template_ptr->ClassNow >= beast_spirit_template_ptr->UnlockConditionArray[i + 1])
				{
					is_upgrade = true;
					break;
				}
			}
			else if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_wing)
			{
				WingTemplate* wing_template_ptr = item_mount_or_wing_ptr->get_wing_template_ptr();
				if (nullptr == wing_template_ptr)
				{
					continue;
				}

				if (wing_template_ptr->UpgradeNow >= beast_spirit_template_ptr->UnlockConditionArray[i + 1])
				{
					is_upgrade = true;
					break;
				}
			}
		}
	}
	if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_spirit)
	{
		is_upgrade = true;
	}
	if (!is_upgrade)
	{
		return false;
	}

	change_beast_spirit_att(beast_spirit_template_ptr->BeastSpiritType, false);
	if (set_mount_or_wing_beast_spirit_bag_data(item_ptr->get_data_info(e_item_info_info_id), beast_spirit_template_ptr->BeastSpiritType, item_num))
	{
		change_beast_spirit_att(beast_spirit_template_ptr->BeastSpiritType, true);
		if (item_ptr->get_data_info(e_item_info_stack_count) - item_num <= 0)
		{
			del_item(e_server_log_del_item_use_beast, 0,*item_ptr, item_ptr->get_data_info(e_item_info_stack_count));
		}
		else
		{
			item_ptr->set_data_info(e_item_info_stack_count, item_ptr->get_data_info(e_item_info_stack_count) - item_num);
			send_item_one(item_ptr);
		}

		if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_mount)
		{
			sync_ranking_mount();
		}
		else if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_wing)
		{
			sync_ranking_wing();
		}
		else if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_spirit)
		{
			sync_ranking_spirit();
		}
		return true;
	}

	if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_mount)
	{
		sync_ranking_mount();
	}
	else if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_wing)
	{
		sync_ranking_wing();
	}
	else if (beast_spirit_template_ptr->BeastSpiritType == e_beast_spirit_type_spirit)
	{
		sync_ranking_spirit();
	}
	change_beast_spirit_att(beast_spirit_template_ptr->BeastSpiritType, true);
	return false;
	*/
}

bool item_set::item_one_key_use(const std::vector<guid_64>& item_guid_array)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false
		|| temp_player.m_cur_map_template_ptr == nullptr)
	{
		return false;
	}
	if (temp_player.is_dead())
	{
		return false;
	}

	const int32 item_guid_array_size = item_guid_array.size();
	item_proto_item_one_key_use_end item_one_key_use_end;
	item_one_key_use_end.set_result(e_error_code_success);
	std::vector<s_item_template_info> get_item_list;
	std::vector<int32> get_money_list;
	character_proto_character_get_money character_get_money;

	for (int32 item_guid_array_index = 0; item_guid_array_index < item_guid_array_size; item_guid_array_index++)
	{
		const guid_64 item_guid = item_guid_array[item_guid_array_index];
		if (false == item_guid.is_valid())
		{
			return false;
		}
		citem* item_ptr = get_item(item_guid);
		if (nullptr == item_ptr)
		{
			return false;
		}
		int32 item_num = item_ptr->get_data_info(e_item_info_stack_count);
		int32 item_gid = item_ptr->get_item_info_ptr()->attribute_id;

		e_error_code item_use_check_ret = item_use_check(*item_ptr, item_num);

		item_proto_item_use_end item_use_end_msg;
		item_use_end_msg.set_result(item_use_check_ret);
		item_use_end_msg.set_item_template_id(item_gid);

		switch (item_use_check_ret)
		{
		case e_error_code_success:
			break;

		case e_error_code_item_fruit_upper_limit:
			if (item_num <= 0)
			{
				temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
				break;
			}
			break;

		default:
			temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
			break;
		}


		if (e_error_code_success != item_use_check(*item_ptr, item_num))
		{
			continue;
		}

		int32 item_id = item_ptr->get_item_info_ptr()->attribute_id;
		ItemTemplate* real_item_template_ptr = item_ptr->get_item_info_ptr();
		if (real_item_template_ptr == nullptr)
		{
			return false;
		}
		PropBasicAttributeTemplate* prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, real_item_template_ptr->logic_id);
		if (prop_template == nullptr)
		{
			return false;
		}

		const e_prop_sub_type prop_sub_type = (e_prop_sub_type)real_item_template_ptr->sub_type;
		e_error_code item_use_end_result;
		const int32 item_use_num = open_package(prop_sub_type,
			prop_template,
			item_num,
			item_use_end_result,
			get_money_list,
			get_item_list,
			real_item_template_ptr->Notice_id,
			real_item_template_ptr);

		item_num = item_use_num < item_num ? item_use_num : item_num;

		del_item(e_server_log_del_item_item_use, 0, item_ptr, item_num);

		if (e_error_code_success != item_use_end_result)
		{
			item_one_key_use_end.set_result(item_use_end_result);
		}
	}

	get_item_send_promp_msg_to_client(get_item_list);
	const int32 get_money_array_size = get_money_list.size();
	if (0 < get_money_array_size)
	{
		for (int32 get_money_array_index = 0; get_money_array_index + 1 < get_money_array_size; get_money_array_index += 2)
		{
			character_get_money.add_money_type_array(get_money_list[get_money_array_index]);
			character_get_money.add_money_num_array(init_unit::change_i32_to_string(get_money_list[get_money_array_index + 1]));
		}
		temp_player.send_message_to_self(&character_get_money, e_msgindex_s2c_character_get_money);
	}

	temp_player.send_message_to_self(&item_one_key_use_end, e_msgindex_s2c_item_one_key_use_end);
	return true;
}

bool item_set::item_sell(const guid_64& item_guid, int32 item_num)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return false;
	}
	if (!item_guid.is_valid())
	{
		return false;
	}
	citem* item_ptr = get_item(item_guid.server_64);
	if (nullptr == item_ptr)
	{
		return false;
	}
	int32 item_num_now = item_ptr->get_data_info(e_item_info_stack_count);

	if (item_num > item_num_now || item_num < 1)
	{
		return false;
	}

	if (item_ptr->get_item_info_ptr()->SellMoney.size() < 2)
	{
		return false;
	}

	if (item_ptr->get_item_info_ptr()->item_type == e_item_type_spirit)
	{
		int32 spirit_templateid = item_ptr->get_item_info_ptr()->logic_id + item_ptr->get_data_info(e_item_info_upgrade_count);

		SpiritTemplate* temp_template = GET_TEMPLATE(SpiritTemplate, spirit_templateid);
		//int32 earn_money_num = temp_template->Recycle;
		if (temp_template->RecoveryMoney.size() % e_money_tuple_max != 0)
		{
			return false;
		}

		int32 temp_money_id = 0;
		int64 temp_money_num = 0;
		int32 money_list_num = temp_template->RecoveryMoney.size() / e_money_tuple_max;
		for (int32 i = 0; i < money_list_num; i += e_money_tuple_max)
		{
			temp_money_id = temp_template->RecoveryMoney[i + e_money_tuple_id];
			temp_money_num = temp_template->RecoveryMoney[i + e_money_tuple_num];

			temp_player.add_money_or_exp(e_money_type(temp_money_id), temp_money_num, e_server_log_add_money_item_sell);
		}

		temp_player.send_money_all();
		send_item_sell_earn_info(temp_money_id, temp_money_num);
		del_item(e_server_log_del_item_item_sell, 0, item_ptr, item_num);
		return true;
	}

	if (item_ptr->get_item_info_ptr()->item_type == e_item_type_stone
		&& item_ptr->get_item_info_ptr()->sub_type == e_stone_sub_type_psyche)
	{
		PsycheTemplate* psyche_template_ptr = GET_TEMPLATE(PsycheTemplate, item_ptr->get_item_info_ptr()->logic_id);
		if (psyche_template_ptr == nullptr)
		{
			return false;
		}
		cpokedex* pokedex_ptr = temp_player.get_pokedex_mgr().get_pokedex_by_templateid(psyche_template_ptr->PokedexTemplateId);
		if (pokedex_ptr != nullptr)
		{
			if (!pokedex_ptr->is_psyche_submit_finish(item_ptr->get_data_info(e_item_info_info_id)))
			{
				return false;
			}
		}
	}

	int32 len = item_ptr->get_item_info_ptr()->SellMoney.size();
	if (len >= 2)
	{
		int32 earn_money_id = item_ptr->get_item_info_ptr()->SellMoney[0];
		int64 earn_money_num = item_ptr->get_item_info_ptr()->SellMoney[1] * item_num;

		if (temp_player.add_money_or_exp((e_money_type)earn_money_id, earn_money_num, e_server_log_add_money_item_sell, item_ptr->get_data_info(e_item_info_info_id)) == false)
		{
			return false;
		}
		temp_player.send_money_one((e_money_type)earn_money_id);
		send_item_sell_earn_info(earn_money_id, earn_money_num);

		item_ptr->set_data_info(e_item_info_stack_count, item_num_now - item_num);
		del_item(e_server_log_del_item_item_sell, 0, item_ptr, item_num);
		return true;
	}
	return false;
}

void item_set::item_merge(e_bag_type merge_bag)
{
	if (merge_bag >= e_bag_type_max)
	{
		return;
	}
	unit& unit_ref = unit_man::get_unit(m_unit_array_index);
	if (false == unit_ref.is_valid())
	{
		return;
	}
	auto& guid_array = GET_BAG(merge_bag);
	static s_merge_stat merge_array[MAX_ITEM_BAG_NUM];
	std::vector<guid_64> merge_guid_array;
	for (auto& it : guid_array)
	{
		merge_guid_array.push_back(it.second);
	}
	int32 merge_num = 0;
	int32 merge_guid_array_size = merge_guid_array.size();
	for (int32 i = 0; i < merge_guid_array_size; ++i)
	{
		auto& item_guid = merge_guid_array[i];
		citem* item_ptr = get_item(item_guid);
		if (nullptr == item_ptr)
		{
			item_guid.clear_data();
			continue;
		}
		ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
		if (nullptr == item_template_ptr)
		{
			del_item(e_server_log_del_item_template_null, 0, item_guid, item_ptr->get_data_info(e_item_info_stack_count));
			item_guid.clear_data();
			continue;
		}
		if (item_ptr->can_merge() == false)
		{
			continue;
		}
		for (int32 j = i + 1; j < merge_guid_array_size; ++j)
		{
			auto& next_item_guid = merge_guid_array[j];
			citem* next_item_ptr = get_item(next_item_guid);
			if (nullptr == next_item_ptr)
			{
				next_item_guid.clear_data();
				continue;
			}
			ItemTemplate* next_item_template_ptr = next_item_ptr->get_item_info_ptr();
			if (nullptr == next_item_template_ptr)
			{
				del_item(e_server_log_del_item_template_null, 0, next_item_guid, next_item_ptr->get_data_info(e_item_info_stack_count));
				next_item_guid.clear_data();
				continue;
			}
			if (next_item_ptr->can_merge() == false)
			{
				continue;
			}
			if (item_ptr->get_item_template_id() == next_item_ptr->get_item_template_id() && item_ptr->get_data_info(e_item_info_locked) == next_item_ptr->get_data_info(e_item_info_locked))
			{
				int32 item_count = item_ptr->get_data_info(e_item_info_stack_count);
				int32 next_item_count = next_item_ptr->get_data_info(e_item_info_stack_count);
				int32 sum_count = item_count + next_item_count;
				if (sum_count > item_template_ptr->max_pile_num)
				{
					item_ptr->set_data_info(e_item_info_stack_count, item_template_ptr->max_pile_num);
					next_item_ptr->set_data_info(e_item_info_stack_count, sum_count - item_template_ptr->max_pile_num);
					set_merge_array(merge_array, merge_num, item_guid, item_template_ptr->max_pile_num);
					set_merge_array(merge_array, merge_num, next_item_guid, sum_count - item_template_ptr->max_pile_num);
				}
				else
				{
					item_ptr->set_data_info(e_item_info_stack_count, sum_count);
					del_item_instance(e_server_log_del_item_bag_merge, 0, next_item_ptr, next_item_count);
					set_merge_array(merge_array, merge_num, item_guid, sum_count);
					set_merge_array(merge_array, merge_num, next_item_guid, 0);
				}
			}
		}
	}
	if (merge_num <= 0)
	{
		return;
	}
	guid_array.clear();
	int32 slot = 0;
	for (int32 i = 0; i < merge_guid_array_size; ++i)
	{
		citem* item_ptr = get_item(merge_guid_array[i]);
		if (item_ptr)
		{
			item_ptr->set_data_info(e_item_info_slot, slot);
			guid_array.insert({ slot++, merge_guid_array[i] });
		}
	}
	item_proto_item_merge_end item_merge_end_msg;
	item_merge_end_msg.add_role_guid(unit_ref.get_unit_guid().A);
	item_merge_end_msg.add_role_guid(unit_ref.get_unit_guid().B);
	item_merge_end_msg.set_merge_item_count(merge_num);

	for (int32 i = 0; i < merge_num; ++i)
	{
		s_merge_stat& merge_stat = merge_array[i];
		item_proto_item_merge_entry* item_merge_entry_msg = item_merge_end_msg.add_merge_item_list();
		if (nullptr == item_merge_entry_msg)
		{
			return;
		}
		item_merge_entry_msg->add_item_guid(merge_stat.item_guid.A);
		item_merge_entry_msg->add_item_guid(merge_stat.item_guid.B);
		item_merge_entry_msg->set_item_num(merge_stat.total_num);
	}
	unit_ref.send_message_to_self(&item_merge_end_msg, e_msgindex_s2c_item_merge_end);
}
void item_set::set_merge_array(s_merge_stat* merge_array, int32& merge_num, const guid_64& item_guid, const int32& item_count)
{
	if (nullptr == merge_array)
	{
		return;
	}
	bool repeate = false;
	for (int32 i = 0; i < merge_num; ++i)
	{
		if (merge_array[i].item_guid == item_guid)
		{
			merge_array[i].total_num = item_count;
			repeate = true;
			break;
		}
	}
	if (repeate == false)
	{
		merge_array[merge_num].item_guid = item_guid;
		merge_array[merge_num].total_num = item_count;
		++merge_num;
	}
}

s_money_tuple item_set::get_item_recovery_earn_money(int32 item_template_id, int32 excellent_att_num)
{
	s_money_tuple earn_money;

	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
	if (nullptr == item_template_ptr)
	{
		return earn_money;
	}
	int32 logic_template_id = item_template_ptr->logic_id;
	if (logic_template_id <= 0)
	{
		return earn_money;
	}

	if (init_unit::is_equip_type(item_template_ptr->item_type))
	{
		EquipTemplate* equip_template_ptr = GET_TEMPLATE(EquipTemplate, logic_template_id);
		if (nullptr == equip_template_ptr)
		{
			return earn_money;
		}
		//if (equip_template_ptr->RecoveryMoney.size() > 0 && equip_template_ptr->RecoveryMoney.size() % 8 == 0)
		//{
		//	earn_money.money_id = equip_template_ptr->RecoveryMoney[0];
		//	earn_money.money_num = equip_template_ptr->RecoveryMoney[excellent_att_num + 1];
		//}
	}
	else if (init_unit::is_spirit_type(item_template_ptr->item_type))
	{
		SpiritTemplate* spirit_tmpl_ptr = GET_TEMPLATE(SpiritTemplate, logic_template_id);
		if (nullptr == spirit_tmpl_ptr)
		{
			return earn_money;
		}
		if (spirit_tmpl_ptr->RecoveryMoney.size() < e_money_tuple_max)
		{
			return earn_money;
		}
		earn_money.money_id = spirit_tmpl_ptr->RecoveryMoney[e_money_tuple_id];
		earn_money.money_num = spirit_tmpl_ptr->RecoveryMoney[e_money_tuple_num];
	}

	return earn_money;
}

bool item_set::item_recovery(const std::vector<guid_64>& item_guid, bool is_sprite_use_diamond)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (false == temp_player.is_valid())
	{
		return false;
	}
	int32 size = item_guid.size();
	if (size <= 0)
	{
		return false;
	}

	std::map<guid_64, bool> processed_map;

	std::vector<s_del_item_guid> del_guid_array;
	del_guid_array.reserve(item_guid.size());

	std::vector<s_item_template_info> money_get_array;
	std::vector<s_item_template_info> item_get_array;
	std::vector<s_item_template_info> money_cost_array;
	std::vector<int64> equip_random_array;
	money_cost_array.reserve(10);
	equip_random_array.reserve(6);
	std::vector<citem*>  item_array;
	for (int32 i = 0; i < item_guid.size(); ++i)
	{
		if (processed_map.find(item_guid[i]) != processed_map.end())
		{
			continue;
		}
		processed_map.insert({ item_guid[i],true });

		citem* temp_item = get_item(item_guid[i]);
		if (temp_item == nullptr)
		{
			continue;
		}
		ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
		if (nullptr == item_template_ptr)
		{
			continue;
		}
		if (judge_can_recovery(temp_item) == false)
		{
			temp_player.send_notice("90096654");
			continue;
		}

		if (item_template_ptr->item_type == faith::e_item_type_spirit)
		{
			if (temp_item->get_data_info(e_item_info_container_type) == e_bag_type_equip_spirit)
			{
				continue;
			}
			s_item_info temp_item_info = temp_item->get_item_inst();
			int32 upgrade_num = temp_item->get_data_info(e_item_info_upgrade_count);
			if (upgrade_num > 1)
			{
				temp_item->set_data_info(e_item_info_upgrade_count, 1);
			}
			int32 star_grade = temp_item->get_data_info(e_item_info_stargrade_count);
			if (star_grade > 0)
			{
				temp_item->set_data_info(e_item_info_stargrade_count, 0);
				SpiritTemplate* temp_spirit_template_ptr = GET_TEMPLATE(SpiritTemplate, item_template_ptr->logic_id);
				if (temp_spirit_template_ptr == nullptr)
				{
					continue;
				}
				int32 RecoveryMaterialNum = 0;
				int32 temp_spirit_template_id = temp_spirit_template_ptr->StarGradeTemplateID;
				SpiritTemplate* temp_spirit_star_template_ptr = GET_TEMPLATE(SpiritTemplate, temp_spirit_template_id);
				if (temp_spirit_star_template_ptr == nullptr)
				{
					continue;
				}
				for (int32 i = 0; i < star_grade; i++)
				{
					SpiritTemplate* spirit_template_ptr = GET_TEMPLATE(SpiritTemplate, temp_spirit_template_id + i);
					if (spirit_template_ptr == nullptr)
					{
						continue;
					}
					RecoveryMaterialNum += spirit_template_ptr->UpgradeMatirialNum;
				}
				int32 use_lock_first = 0;
				if (temp_item->get_data_info(e_item_info_locked) > 0)
				{
					use_lock_first = 1;
				}
				citem* recovery_item_material = create_item_by_template(e_server_log_add_item_item_recovery, 0, temp_spirit_star_template_ptr->UpgradeMatirialId, RecoveryMaterialNum, use_lock_first);
				if (recovery_item_material == nullptr)
				{
					continue;
				}
				item_array.push_back(recovery_item_material);
			}
			temp_item->set_data_info(e_item_info_wing_exp, 0);
			send_item_one(temp_item);
			if (upgrade_num == 1 && star_grade == 0)
			{
				del_guid_array.push_back(item_guid[i]);
			}
			if (upgrade_num > 1 || star_grade == 0)
			{
				init_unit::get_item_recovery_money_info(temp_item_info, money_get_array, money_cost_array, false, is_sprite_use_diamond);
			}
		}
		else if (item_template_ptr->item_type == faith::e_item_type_goddess_equip)
		{
			del_guid_array.push_back(item_guid[i]);
			if (temp_item->get_data_info(e_item_info_container_type) == e_bag_type_goddess_equip)
			{
				continue;
			}
			DeityTemplate* deity_template_ptr = temp_item->get_deity_info_ptr();
			if (nullptr == deity_template_ptr)
			{
				continue;
			}
			if (deity_template_ptr->ItemGet.size() > 0 && deity_template_ptr->ItemGet.size() % 2 == 0)
			{
				citem* recovery_item_material = create_item_by_template(e_server_log_add_item_item_recovery, 0, deity_template_ptr->ItemGet[0], deity_template_ptr->ItemGet[1]);
				if (recovery_item_material == nullptr)
				{
					continue;
				}
				item_array.push_back(recovery_item_material);
			}

		}
		else if (item_template_ptr->item_type == faith::e_item_type_mount_equip)
		{
			del_guid_array.push_back(item_guid[i]);

			MountEquipTemplate* equ_template_ptr = temp_item->get_mount_equip_template_ptr();
			if (nullptr == equ_template_ptr)
			{
				continue;
			}
			if (equ_template_ptr->ItemGet.size() > 0 && equ_template_ptr->ItemGet.size() % 2 == 0)
			{
				for (int32 data_num = 0; data_num < equ_template_ptr->ItemGet.size() / 2; ++data_num)
				{
					citem* recovery_item_material = create_item_by_template(e_server_log_add_item_item_recovery, 0, equ_template_ptr->ItemGet[data_num * 2], equ_template_ptr->ItemGet[(data_num * 2) + 1]);
					if (recovery_item_material == nullptr)
					{
						continue;
					}
					item_array.push_back(recovery_item_material);
				}
			}

		}
		else if (temp_item->is_equip_type())
		{
			del_guid_array.push_back(item_guid[i]);
			EquipTemplate* equip_template_ptr = temp_item->get_equip_info_ptr();
			if (equip_template_ptr == nullptr)
			{
				continue;
			}
			std::vector<s_item_template_info> item_drop_list_with_arr;
			//cdrop::gen_drop_list_by_drop_id(equip_template_ptr->RecoveryItem, item_drop_list_with_arr, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));
			for (int32 j = 0; j < item_drop_list_with_arr.size(); ++j)
			{
				init_unit::merge_item(item_get_array, item_drop_list_with_arr[j].m_item_id, 1);
			}
			for (int32 i = e_item_info_jewel_slot_0; i < e_item_info_jewel_vip_slot_1; ++i)
			{
				if (temp_item->get_data_info((e_item_info)i) > 0)
				{
					init_unit::merge_item(item_get_array, temp_item->get_data_info((e_item_info)i), 1);
				}
			}
			init_unit::get_item_recovery_money_info(temp_item->get_item_inst(), money_get_array, money_cost_array, false, is_sprite_use_diamond);
			if (e_item_color_red == item_template_ptr->item_color)
			{
				for (int32 i = e_item_info_random_had_flag; i < e_item_info_random_property6; i++)
				{
					equip_random_array.push_back(temp_item->get_data_info((e_item_info)i));
				}
			}
		}
		else
		{
			del_guid_array.push_back(item_guid[i]);
			init_unit::get_item_recovery_money_info(temp_item->get_item_inst(), money_get_array, money_cost_array, false, is_sprite_use_diamond);
		}
	}

	for (auto& it : money_cost_array)
	{
		if (!temp_player.can_cut_money(e_money_type(it.m_item_id), it.m_item_num))
		{
			return false;
		}
	}
	del_item(e_server_log_del_item_item_sell, 0, del_guid_array);
	for (auto& it : money_cost_array)
	{
		temp_player.cut_money(e_money_type(it.m_item_id), it.m_item_num, e_server_log_cut_money_item_recovery);
	}

	for (auto& it : money_get_array)
	{
		temp_player.add_money(e_money_type(it.m_item_id), it.m_item_num, e_server_log_add_money_item_recovery, -1, false);
	}


	put_in_bag(e_server_log_add_item_item_recovery, 0, item_get_array, e_bag_type_none, e_item_mail_type_item_recovery);

	temp_player.send_promp_msg_to_client(item_get_array, money_get_array);
	return true;
}
void item_set::cost_item_by_id_with_lock_states(e_server_log_del_item causeid, int32 location, int32 item_template_id, int32 item_cost_num, int32 first_use_lock)
{
	int32 temp_item_count = item_cost_num;
	m_bind_material_num = 0;
	m_no_bind_material_num = 0;

	//if (first_use_lock > 0)
	{
		for (int32 i = 0; i < MAX_ITEM_BAG_NUM; ++i)
		{
			if (temp_item_count <= 0)
			{
				break;
			}
			citem* item_ptr = get_item(GET_BAG(e_bag_type_bag)[i]);
			if (item_ptr && item_ptr->get_data_info(e_item_info_info_id) == item_template_id && item_ptr->get_data_info(e_item_info_locked) == first_use_lock)
			{
				int32 item_num = item_ptr->get_data_info(e_item_info_stack_count);
				if (item_num > temp_item_count)
				{
					m_bind_material_num += temp_item_count;
					del_item(causeid, location, item_ptr, temp_item_count);
					temp_item_count = 0;
					break;
				}
				else
				{
					m_bind_material_num += item_num;
					temp_item_count -= item_num;
					del_item(causeid, location, item_ptr, item_num);
				}
			}
		}
	}

	if (temp_item_count > 0)
	{
		for (int32 i = 0; i < MAX_ITEM_BAG_NUM; ++i)
		{
			if (temp_item_count <= 0)
			{
				break;
			}
			citem* item_ptr = get_item(GET_BAG(e_bag_type_bag)[i]);
			if (item_ptr && item_ptr->get_data_info(e_item_info_info_id) == item_template_id)
			{
				int32 item_num = item_ptr->get_data_info(e_item_info_stack_count);
				if (item_num > temp_item_count)
				{
					m_no_bind_material_num += temp_item_count;
					del_item(causeid, location, item_ptr, temp_item_count);
					temp_item_count = 0;
					break;
				}
				else
				{
					m_no_bind_material_num += item_num;
					temp_item_count -= item_num;
					del_item(causeid, location, item_ptr, item_num);
				}
			}
		}
	}
}

int32 item_set::upgrade_protect_item_can_use(const ItemUpgradeTemplate* item_upgrade_ptr, int32 cur_upgrade_num, int32 protect_item_index)
{
	if (protect_item_index < 0)
	{
		return e_error_code_item_upgrade;
	}
	//if (item_upgrade_ptr == nullptr
	//	|| item_upgrade_ptr->ProtectItemInfo.size() % 2 != 0)
	//{
	//	return e_error_code_item_upgrade;
	//}
	//int32 data_num = item_upgrade_ptr->ProtectItemInfo.size() / 2;
	//if (protect_item_index >= data_num)
	//{
	//	return e_error_code_item_upgrade;
	//}
	//int32 item_id = item_upgrade_ptr->ProtectItemInfo[protect_item_index * 2];
	//int32 item_num = item_upgrade_ptr->ProtectItemInfo[protect_item_index * 2 + 1];
	//if (item_id <= 0
	//	|| item_num <= 0)
	//{
	//	return e_error_code_item_upgrade;
	//}

	//if (get_item_count(e_bag_type_bag, item_id) < item_num)
	//{
	//	return e_error_code_item_matirial_not_enough;
	//}

	//ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
	//if (nullptr == item_template_ptr)
	//{
	//	return e_error_code_item_upgrade;
	//}
	//PropBasicAttributeTemplate* props_template_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, item_template_ptr->logic_id);
	//if (nullptr == props_template_ptr)
	//{
	//	return e_error_code_item_upgrade;
	//}
	//if (props_template_ptr->FuncLevelLimit.size() >= 2)
	//{
	//	if (cur_upgrade_num < props_template_ptr->FuncLevelLimit[0] || cur_upgrade_num > props_template_ptr->FuncLevelLimit[1])
	//	{
	//		return e_error_code_item_upgrade;
	//	}
	//}
	return e_error_code_success;
}

int32 item_set::set_jewel_on(const guid_64& target_guid, const int32& target_slot, const guid_64& jewel_guid)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}

	if (target_slot < e_item_info_jewel_slot_0
		|| target_slot > e_item_info_jewel_vip_slot_1)
	{
		return e_item_string_unkown;
	}

	citem* target_equip_ptr = get_item(target_guid);
	if (target_equip_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	EquipTemplate* equip_template_ptr = target_equip_ptr->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	if (target_slot < e_item_info_jewel_vip_slot_0)
	{
		//if (!init_unit::is_can_use_this_jewel_slot_on_this_equip_level(equip_template_ptr->quality_level, target_slot))
		//{
		//	return e_item_string_unkown;
		//}
	}
	else
	{
		if (!init_unit::is_can_use_this_jewel_slot_on_this_vip_level(temp_player.get_vip_level(), target_slot))
		{
			return e_item_string_unkown;
		}
	}

	citem* jewel_ptr = get_item(jewel_guid);
	if (jewel_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	ItemTemplate* jewel_item_template_ptr = jewel_ptr->get_item_info_ptr();
	if (jewel_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	JewelTemplate* jewel_template_ptr = GET_TEMPLATE(JewelTemplate, jewel_item_template_ptr->logic_id);
	if (jewel_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	//if (equip_template_ptr->CanSettingJewelType != jewel_template_ptr->JewelType)
	//{
	//	return e_item_string_unkown;
	//}

	int32 old_jewel_id = target_equip_ptr->get_data_info((e_item_info)target_slot);
	if (old_jewel_id > 0)
	{
		citem* old_jewel_ptr = create_item_by_template(e_server_log_add_item_set_jewel_off, 0, old_jewel_id, 1, 1);
		if (old_jewel_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		put_in_bag(old_jewel_ptr, e_bag_type_none, e_item_mail_type_jewel_send_mail);
	}

	set_equip_jewel_att(*target_equip_ptr, false);
	target_equip_ptr->set_data_info(e_item_info(target_slot), jewel_ptr->get_data_info(e_item_info_info_id));
	set_equip_jewel_att(*target_equip_ptr, true);
	send_item_one(target_equip_ptr);
	temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(target_equip_ptr);
	del_item(e_server_log_del_item_set_jewel, old_jewel_id, jewel_ptr, 1);



	return e_item_string_succeed;
}

int32 item_set::set_jewel_off(const guid_64& target_guid, const int32& target_slot)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	if (target_slot < e_item_info_jewel_slot_0
		|| target_slot > e_item_info_jewel_vip_slot_1)
	{
		return e_item_string_unkown;
	}
	citem* target_equip_ptr = get_item(target_guid);
	if (target_equip_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 old_jewel_id = target_equip_ptr->get_data_info((e_item_info)target_slot);
	if (old_jewel_id <= 0)
	{
		return e_item_string_unkown;
	}

	citem* old_jewel_ptr = create_item_by_template(e_server_log_add_item_set_jewel_off, 0, old_jewel_id, 1, 1);
	if (old_jewel_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	std::vector<citem*> item_inst_array;
	item_inst_array.push_back(old_jewel_ptr);

	put_in_bag(item_inst_array, e_bag_type_none, e_item_mail_type_jewel_send_mail);

	set_equip_jewel_att(*target_equip_ptr, false);
	target_equip_ptr->set_data_info(e_item_info(target_slot), 0);
	set_equip_jewel_att(*target_equip_ptr, true);
	send_item_one(target_equip_ptr);

	temp_player.get_chat_mgr().receive_top_tip(90091118);

	temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(target_equip_ptr);

	temp_player.get_mission_mgr().target_check(e_mission_end_type_inset_genstone);

	return e_item_string_succeed;
}

int32 item_set::jewel_upgrade(const guid_64& target_guid, const int32& target_slot)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	if (target_slot < e_item_info_jewel_slot_0
		|| target_slot > e_item_info_jewel_vip_slot_1)
	{
		return e_item_string_unkown;
	}

	citem* target_equip_ptr = get_item(target_guid);
	if (target_equip_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 target_jewel_id = target_equip_ptr->get_data_info(e_item_info(target_slot));
	if (target_jewel_id <= 0)
	{
		return e_item_string_unkown;
	}

	ItemTemplate* jewel_item_template_ptr = GET_TEMPLATE(ItemTemplate, target_jewel_id);
	if (jewel_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	JewelTemplate* jewel_template_ptr = GET_TEMPLATE(JewelTemplate, jewel_item_template_ptr->logic_id);
	if (jewel_template_ptr == nullptr
		|| jewel_template_ptr->JewelValue.size() < 2)
	{
		return e_item_string_unkown;
	}
	if (jewel_template_ptr->NextLevelItemId <= 0)
	{
		return e_item_string_unkown;
	}

	int32 cost_money_type = jewel_template_ptr->JewelValue[0];
	int32 cost_money_value = jewel_template_ptr->JewelValue[1] * 2;

	if (!temp_player.can_cut_money(e_money_type(cost_money_type), cost_money_value))
	{
		return e_item_string_money;
	}

	set_equip_jewel_att(*target_equip_ptr, false);
	target_equip_ptr->set_data_info(e_item_info(target_slot), jewel_template_ptr->NextLevelItemId);
	set_equip_jewel_att(*target_equip_ptr, true);

	temp_player.cut_money(e_money_type(cost_money_type), cost_money_value, e_server_log_cut_money_jewel_upgrade);

	send_item_one(target_equip_ptr);

	temp_player.get_ranking_mgr().set_and_sync_single_equip_ranking_data(target_equip_ptr);

	return e_item_string_succeed;
}

int32 item_set::get_equipment_upgrade_buff_id(bool is_weapon)
{
	int32 buff_id = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return buff_id;
	}
	int32 class_type = temp_player.get_unit_info(e_role_info_class_type);
	int32 branch_type = temp_player.get_unit_info(e_role_info_class_branch);
	int32 grade_num = 0;

	int32 phase_level = 0;
	if (is_weapon)
	{
		citem* weapon_ptr = get_equip_item_by_slot(e_role_equip_slot_weapon_1);
		if (weapon_ptr == nullptr)
		{
			return buff_id;
		}
		ItemTemplate* item_template_ptr = weapon_ptr->get_item_info_ptr();
		if (item_template_ptr == nullptr)
		{
			return buff_id;
		}
		EquipTemplate* equip_template_ptr = weapon_ptr->get_equip_info_ptr();
		if (nullptr == equip_template_ptr)
		{
			return buff_id;
		}
		phase_level = init_unit::get_upgrade_phase_level(weapon_ptr->get_data_info(e_item_info_upgrade_count));
		//grade_num = equip_template_ptr->quality_level - 1; //init_unit::get_player_grade_num(item_template_ptr->level_limit);
	}
	else
	{
		int32 eight_item_smallest_level = get_equip_smallest_upgrade_level();
		phase_level = init_unit::get_upgrade_phase_level(eight_item_smallest_level);
		grade_num = temp_player.get_grade_num();
	}

	if (phase_level <= 0)
	{
		return buff_id;
	}

	ItemUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(ItemUpgradeTemplate,
		item_upgrade_default_start_id + phase_level);

	if (upgrade_template_ptr == nullptr)
	{
		return buff_id;
	}
	buff_id = init_unit::get_upgrade_buff_id(upgrade_template_ptr, class_type, branch_type, grade_num, is_weapon);
	return buff_id;
}

int32 item_set::wing_upgrade(const guid_64& wing_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("wing", "wing_upgrade", 1, false, "%d%s%d>%d", m_unit_array_index, wing_guid.to_string().c_str(), use_up_item, &result);
	return result;
	/*
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return e_item_string_unkown;
	}
	citem* wing_ptr = get_item(wing_guid);
	if (nullptr == wing_ptr
		|| nullptr == wing_ptr->get_item_info_ptr())
	{
		return e_item_string_no_item;
	}

	if (wing_ptr->get_data_info(e_item_info_activate) == 0 || wing_ptr->get_data_info(e_item_info_over_time) != 0)
	{
		return e_item_string_unkown;
	}

	if (wing_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;

	int32 real_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	if (nullptr == wing_template_ptr)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	bool is_cost_money = false;
	if (wing_template_ptr->UpgradeMoney.size() > 1)
	{
		is_cost_money = true;
	}
	if (is_cost_money
		&& wing_template_ptr->UpgradeMoney.size() % 2 != 0)
	{
		return e_item_string_unkown;
	}

	if (wing_template_ptr->GradeLimit > temp_player_ref.get_grade_num())
	{
		return e_item_string_unkown;
	}


	int32 cur_wing_star_num = wing_template_ptr->StarNow;
	int32 max_wing_star_num = wing_template_ptr->WingAddStarMax;
	int32 cur_wing_level = wing_template_ptr->UpgradeNow;
	int32 max_wing_level = wing_template_ptr->WingUpgradeMax;

	if (wing_template_ptr->Type == e_item_wing_type_normal
		&& wing_ptr != get_cur_level_wing())
	{
		return e_item_string_full_level;
	}
	if (wing_template_ptr->Type == e_item_wing_type_illusion || wing_template_ptr->Type == e_item_wing_type_special_santo)
	{
		return e_item_string_unkown;
	}

	if ((cur_wing_star_num >= max_wing_star_num) && (cur_wing_level >= max_wing_level))
	{
		return e_item_string_full_level;
	}

	if (cur_wing_star_num == max_wing_star_num)
	{
		if (wing_template_ptr->Type == e_item_wing_type_normal)
		{
			if (temp_player_ref.get_unit_info(e_role_info_exp_level) < wing_template_ptr->WingOpenNeedRoleClass)
			{
				return e_item_string_level;
			}
		}
		else
		{
			citem* cur_max_level_wing_ptr = get_cur_level_wing();
			if (nullptr == cur_max_level_wing_ptr)
			{
				return e_item_string_unkown;
			}

			WingTemplate* cur_max_level_wing_template_ptr = cur_max_level_wing_ptr->get_wing_template_ptr();
			if (nullptr == cur_max_level_wing_template_ptr)
			{
				return e_item_string_unkown;
			}

			if (wing_template_ptr->UpgradeNow >= cur_max_level_wing_template_ptr->UpgradeNow)
			{
				return e_item_string_level;
			}
		}

		if (get_item_count(wing_template_ptr->UpgradeMatirialId, e_bag_type_bag) < wing_template_ptr->UpgradeMatirialNum)
		{
			if (use_up_item == 0)
			{
				return e_item_string_matirial_not_enough;
			}
			else
			{
				use_money_instead_matirial = true;
			}
		}
		if (use_money_instead_matirial)
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, wing_template_ptr->UpgradeShopId);
			if (goods_ptr != nullptr
				&& goods_ptr->NewPrice.size() > region_money_num_index)
			{
				int32 money_id = goods_ptr->NewPrice[region_money_type_index];
				int32 money_num = goods_ptr->NewPrice[region_money_num_index] * wing_template_ptr->UpgradeMatirialNum;
				if (!temp_player_ref.can_cut_money((e_money_type)money_id, money_num))
				{
					return e_item_string_auto_use_money_not_enough;
				}
				temp_player_ref.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_upgrade_auto_buy, wing_template_ptr->UpgradeShopId);
			}
			else
			{
				return e_item_string_no_item;
			}
		}
		else
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_wing_upgrade, wing_ptr->get_data_info(e_item_info_info_id) , wing_template_ptr->UpgradeMatirialId, wing_template_ptr->UpgradeMatirialNum, e_item_locked);
		}

		if (wing_template_ptr->UpgradeMoney.size() >= e_money_tuple_max && temp_player_ref.can_cut_money((e_money_type)wing_template_ptr->UpgradeMoney[e_money_tuple_id], wing_template_ptr->UpgradeMoney[e_money_tuple_num]) == false)
		{
			return e_item_string_money;
		}
		int32 upgrade_bless_now = wing_ptr->get_data_info(e_item_info_wing_bless);

		if (is_cost_money)
		{
			if (wing_template_ptr->UpgradeMoney.size() >= e_money_tuple_max)
			{
				temp_player_ref.cut_money((e_money_type)wing_template_ptr->UpgradeMoney[e_money_tuple_id], wing_template_ptr->UpgradeMoney[e_money_tuple_num], e_server_log_cut_money_wing_upgrade, real_id);
			}
			else
			{
				return e_item_string_unkown;
			}
		}

		wing_ptr->set_data_info(e_item_info_wing_bless, upgrade_bless_now + wing_template_ptr->EachUpgradeBless);
		upgrade_bless_now = wing_ptr->get_data_info(e_item_info_wing_bless);
		if (wing_upgrade_rate(upgrade_bless_now, wing_template_ptr) == true)
		{
			set_wing_att(false);

			wing_ptr->set_data_info(e_item_info_wing_exp, 0);
			wing_ptr->set_data_info(e_item_info_wing_bless, 0);

			if (wing_template_ptr->Type != e_item_wing_type_normal)
			{
				wing_ptr->set_data_info(e_item_info_upgrade_count, wing_ptr->get_data_info(e_item_info_upgrade_count) + 1);
				set_wing_att(true);
				item_proto_show_item_upgrade_effect effect_msg;
				effect_msg.set_show_type(e_item_upgrade_effect_type_wing_upgrade);
				effect_msg.set_show_value(cur_wing_level + 1);
				temp_player_ref.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);
				WingTemplate* new_wing_template_ptr = get_wing_template_ptr(*wing_ptr);
				if (new_wing_template_ptr != nullptr)
				{
					if (new_wing_template_ptr->StarBuffId > 0)
					{
						PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, new_wing_template_ptr->StarBuffId);
						if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
						{
							star_skill_unlock(wing_guid, wing_ptr->get_data_info(e_item_info_upgrade_count), false, false);
						}
					}
					if (new_wing_template_ptr->NoticeId > 0)
					{
						s_chat_notice_info notice_data;
						const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(new_wing_template_ptr->WingNameId);
						notice_data.item_name.push_back(temp_string);
						notice_data.num_type_data.push_back(new_wing_template_ptr->UpgradeNow);
						temp_player_ref.get_chat_mgr().send_notice_new(new_wing_template_ptr->NoticeId, notice_data);
					}
				}
			}
			else
			{
				reset_wings_order();
				citem* next_level_wing = get_item_by_slot(e_bag_type_wing, wing_ptr->get_data_info(e_item_info_slot) + 1);
				if (next_level_wing == nullptr)
				{
					return e_item_string_unkown;
				}

				ItemTemplate* next_level_wing_item_template = next_level_wing->get_item_info_ptr();
				if (next_level_wing_item_template == nullptr)
				{
					return e_item_string_unkown;
				}

				WingTemplate* next_level_wing_wing_template = GET_TEMPLATE(WingTemplate, next_level_wing_item_template->logic_id);
				if (next_level_wing_wing_template == nullptr)
				{
					return e_item_string_unkown;
				}

				if (next_level_wing_wing_template->Type == e_item_wing_type_normal)
				{
					next_level_wing->set_data_info(e_item_info_activate, 1);
					next_level_wing->set_data_info(e_item_info_upgrade_count, 0);
					next_level_wing->set_data_info(e_item_info_wing_exp, 0);
					next_level_wing->set_data_info(e_item_info_wing_bless, 0);
					send_item_one(next_level_wing);
				}
				citem* cur_level_wing = get_cur_level_wing();
				if (cur_level_wing == nullptr)
				{
					return e_item_string_unkown;
				}
				//sync_ranking_wing();
				set_wing_att(true);

				item_proto_show_item_upgrade_effect effect_msg;
				effect_msg.set_show_type(e_item_upgrade_effect_type_wing_upgrade);
				effect_msg.set_show_value(next_level_wing_wing_template->UpgradeNow);
				temp_player_ref.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);

				//mission check
				temp_player_ref.get_mission_mgr().target_check(e_mission_end_type_wing_upgrade);
				temp_player_ref.get_achievement_mgr().item_changed(wing_ptr, e_item_operation_featherupgrade_grade);

				WingTemplate* current_wing_template_ptr = cur_level_wing->get_wing_template_ptr();
				if (current_wing_template_ptr && current_wing_template_ptr->NoticeId > 0)
				{
					s_item_info item_data[chat_max_item];
					int32 item_num = 0;

					int32 notice_id = current_wing_template_ptr->NoticeId;

					std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
					std::vector<std::string> notice_str_params_vec;
					notice_str_params_vec.push_back(notice_str_id);
					notice_str_params_vec.push_back(temp_player_ref.get_name());
					notice_str_params_vec.push_back("%d");  //(template_manager::get_instance().int_to_string(current_wing_template_ptr->WingNameId));
					notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(current_wing_template_ptr->UpgradeNow));
					std::string notice_str = init_unit::implode(notice_str_params_vec);
					item_data[item_num] = next_level_wing->get_item_inst();
					temp_player_ref.get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_num);

					temp_player_ref.refresh_service_goal(e_service_goal_type_wing_level);
				}
			}
		}

		send_item_one(wing_ptr);
		int32 temp_cur_value = temp_player_ref.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_wing);
		temp_player_ref.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_wing, temp_cur_value + 1);
		//sync_ranking_wing();
		return e_error_code_success;
	}
	else
	{
		if (get_item_count(wing_template_ptr->AddStarMatirialId, e_bag_type_bag) < wing_template_ptr->AddStarMatirialNum)
		{
			if (use_up_item == 0)
			{
				return e_item_string_matirial_not_enough;
			}
			else
			{
				use_money_instead_matirial = true;
			}
		}
		if (use_money_instead_matirial)
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, wing_template_ptr->StarShopId);
			if (goods_ptr != nullptr
				&& goods_ptr->NewPrice.size() > region_money_num_index)
			{
				int32 money_id = goods_ptr->NewPrice[region_money_type_index];
				int32 money_num = goods_ptr->NewPrice[region_money_num_index] * wing_template_ptr->AddStarMatirialNum;
				if (!temp_player_ref.can_cut_money((e_money_type)money_id, money_num))
				{
					return e_item_string_auto_use_money_not_enough;
				}
				temp_player_ref.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_add_star_auto_buy, wing_template_ptr->StarShopId);
			}
			else
			{
				return e_item_string_no_item;
			}
		}
		else
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_wing_upgrade, wing_ptr->get_data_info(e_item_info_info_id) , wing_template_ptr->AddStarMatirialId, wing_template_ptr->AddStarMatirialNum, e_item_locked);
		}
		int32 add_star_exp_now = wing_ptr->get_data_info(e_item_info_wing_exp);
		int32 add_star_exp_max = wing_template_ptr->AddStarMaxEXP;
		bool crirical_flag;
		if (rand() % 100 < wing_template_ptr->AddStarCriticalRate)
		{
			add_star_exp_now += wing_template_ptr->EachAddStarExp * 2;
			crirical_flag = true;
		}
		else
		{
			add_star_exp_now += wing_template_ptr->EachAddStarExp;
			crirical_flag = false;
		}

		int32 temp_star = 0;
		while (add_star_exp_now >= add_star_exp_max)
		{
			add_star_exp_now -= add_star_exp_max;
			temp_star++;
		}
		if (temp_star > 0)
		{
			set_wing_att(false);
			if (cur_wing_star_num + temp_star > max_wing_star_num)
			{
				temp_star = max_wing_star_num - cur_wing_star_num;
			}
			wing_ptr->set_data_info(e_item_info_upgrade_count, wing_ptr->get_data_info(e_item_info_upgrade_count) + temp_star);
			int32 wing_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
			WingTemplate* wing_temp_ptr = GET_TEMPLATE(WingTemplate, wing_id);
			if (wing_temp_ptr != nullptr)
			{
				//sync_ranking_wing();
				set_wing_att(true);
				if (wing_temp_ptr->StarBuffId > 0)
				{

					PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, wing_temp_ptr->StarBuffId);
					if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
					{
						star_skill_unlock(wing_guid, wing_ptr->get_data_info(e_item_info_upgrade_count), false, false);
					}
				}
				item_proto_show_item_upgrade_effect effect_msg;
				effect_msg.set_show_type(e_item_upgrade_effect_type_wing_add_star);
				effect_msg.set_show_value(wing_temp_ptr->StarNow);
				temp_player_ref.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);
			}
		}
		wing_ptr->set_data_info(e_item_info_wing_exp, add_star_exp_now);
		send_item_one(wing_ptr);
		//sync_ranking_wing();
		temp_player_ref.refresh_service_goal(e_service_goal_type_wing_level);
		if (crirical_flag)
		{
			return e_error_code_item_add_star_critical;
		}

		int32 temp_cur_value = temp_player_ref.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_wing);
		temp_player_ref.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_wing, temp_cur_value + 1);
		return e_error_code_success;
	}
	*/
}

int32 item_set::wing_starupgrade(const guid_64& wing_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("wing", "wing_starupgrade", 1, false, "%d%s%d>%d", m_unit_array_index, wing_guid.to_string().c_str(), use_up_item, &result);
	return result;
	/*
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return e_item_string_unkown;
	}
	citem* wing_ptr = get_item(wing_guid);
	if (nullptr == wing_ptr
		|| nullptr == wing_ptr->get_item_info_ptr())
	{
		return e_item_string_no_item;
	}

	if (wing_ptr->get_data_info(e_item_info_activate) == 0 || wing_ptr->get_data_info(e_item_info_over_time) != 0)
	{
		return e_item_string_unkown;
	}

	if (wing_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;

	int32 real_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_stargrade_count) + 1000;
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	if (nullptr == wing_template_ptr)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	bool is_cost_money = false;
	if (wing_template_ptr->UpgradeMoney.size() > 1)
	{
		is_cost_money = true;
	}
	if (is_cost_money
		&& wing_template_ptr->UpgradeMoney.size() % 2 != 0)
	{
		return e_item_string_unkown;
	}
	//////////////////////
	int32 offset_upgrade_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* offset_template_ptr = GET_TEMPLATE(WingTemplate, offset_upgrade_id);
	if (nullptr == offset_template_ptr)
	{
		return e_item_string_unkown;
	}

	if (wing_template_ptr->UpgradeLimit > offset_template_ptr->UpgradeNow)
	{
		return e_item_string_unkown;
	}
	////////////////////////

	int32 cur_wing_star_num = wing_template_ptr->StarGradeNow;
	int32 max_wing_star_num = wing_template_ptr->StarGradeMax;

	if (wing_template_ptr->Type == e_item_wing_type_shape_illusion && (cur_wing_star_num >= max_wing_star_num))
	{
		return e_item_string_full_level;
	}

	if (get_item_count(wing_template_ptr->AddStarMatirialId, e_bag_type_bag) < wing_template_ptr->AddStarMatirialNum)
	{
		if (use_up_item == 0)
		{
			return e_item_string_matirial_not_enough;
		}
		else
		{
			use_money_instead_matirial = true;
		}
	}
	if (use_money_instead_matirial)
	{
		GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, wing_template_ptr->StarShopId);
		if (goods_ptr != nullptr
			&& goods_ptr->NewPrice.size() > region_money_num_index)
		{
			int32 money_id = goods_ptr->NewPrice[region_money_type_index];
			int32 money_num = goods_ptr->NewPrice[region_money_num_index] * wing_template_ptr->AddStarMatirialNum;
			if (!temp_player_ref.can_cut_money((e_money_type)money_id, money_num))
			{
				return e_item_string_auto_use_money_not_enough;
			}
			temp_player_ref.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_add_star_auto_buy, wing_template_ptr->StarShopId);
		}
		else
		{
			return e_item_string_no_item;
		}
	}
	else
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_wing_starupgrade , 0 ,wing_template_ptr->AddStarMatirialId, wing_template_ptr->AddStarMatirialNum, e_item_locked);
	}

	item_change_att(wing_template_ptr->AttArray, 1, false);
	wing_ptr->set_data_info(e_item_info_stargrade_count, wing_ptr->get_data_info(e_item_info_stargrade_count) + 1);
	int32 wing_next_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_stargrade_count) + 1000;
	WingTemplate* wing_next_temp_ptr = GET_TEMPLATE(WingTemplate, wing_next_id);
	if (wing_next_temp_ptr != nullptr)
	{
		item_change_att(wing_next_temp_ptr->AttArray, 1, true);

		item_proto_show_item_upgrade_effect effect_msg;
		effect_msg.set_show_type(e_item_upgrade_effect_type_wing_add_star);
		effect_msg.set_show_value(wing_next_temp_ptr->StarGradeNow);
		temp_player_ref.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);

		// wangsonghao
		if (wing_next_temp_ptr->StarBuffId > 0)
		{
			PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, wing_next_temp_ptr->StarBuffId);
			if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
			{
				star_skill_unlock(wing_guid, wing_ptr->get_data_info(e_item_info_stargrade_count), false);
			}
		}

	}

	send_item_one(wing_ptr);

	//sync_ranking_wing();
	//temp_player_ref.refresh_service_goal(e_service_goal_type_wing_level);

	int32 temp_cur_value = temp_player_ref.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_wing);
	temp_player_ref.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_wing, temp_cur_value + 1);
	return e_error_code_success;
	*/
}

int32 item_set::wing_zhuling(const guid_64& wing_guid, int32 use_up_item, int32 item_num)
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return e_error_code_no_template;
	}
	citem* wing_ptr = get_item(wing_guid);
	if (nullptr == wing_ptr
		|| nullptr == wing_ptr->get_item_info_ptr())
	{
		return e_error_code_success;
	}

	if (wing_ptr->get_data_info(e_item_info_activate) == 0 || wing_ptr->get_data_info(e_item_info_over_time) != 0)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_error_code_template_param;
	}

	int32 upgrade_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* wing_upgrade_template_ptr = GET_TEMPLATE(WingTemplate, upgrade_id);
	if (nullptr == wing_upgrade_template_ptr)
	{
		return e_error_code_template_param;
	}
	if (wing_upgrade_template_ptr->UpgradeNow < GAMECONFIG->WingPsychicUnlockNum)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) < 0 || wing_ptr->get_data_info(e_item_info_forge_property1) >= 110)
	{
		return e_error_code_template_param;
	}

	int32 real_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_forge_property1) + 2000;
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	if (nullptr == wing_template_ptr)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) < 50)
	{

		if (wing_ptr->get_data_info(e_item_info_forge_property2) != 0 && time_helper::get_cur_time_new().second < wing_ptr->get_data_info(e_item_info_forge_property2))
		{
			return e_error_code_template_param;
		}

		if (get_item_count(e_bag_type_bag, wing_template_ptr->AddStarMatirialId) < wing_template_ptr->AddStarMatirialNum)
		{
			return e_error_code_template_param;
		}
		else
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_wing_zhuling, 0, wing_template_ptr->AddStarMatirialId, wing_template_ptr->AddStarMatirialNum, e_item_locked);
		}
		set_wing_att(false);
		wing_ptr->set_data_info(e_item_info_forge_property1, wing_ptr->get_data_info(e_item_info_forge_property1) + 10);
		wing_ptr->set_data_info(e_item_info_forge_property2, time_helper::get_cur_time_new().second + wing_template_ptr->LengQueTime * 3600);
	}
	else
	{
		if (GAMECONFIG->WingZhuLingFrequency.size() < 2 || GAMECONFIG->WingZhuLingRandomRange.size() < 9)
		{
			return e_error_code_template_param;
		}

		if (wing_ptr->get_data_info(e_item_info_forge_property1) < 80)
		{
			if (wing_ptr->get_data_info(e_item_info_forge_property4) > GAMECONFIG->WingZhuLingFrequency[0] - 1)
			{
				return e_error_code_template_param;
			}
		}
		else
		{
			if (wing_ptr->get_data_info(e_item_info_forge_property4) > GAMECONFIG->WingZhuLingFrequency[1] - 1)
			{
				return e_error_code_template_param;
			}
		}

		if (use_up_item == 0 || item_num == 0)
		{
			return e_error_code_template_param;
		}

		if (get_item_count(e_bag_type_bag, use_up_item) < item_num)
		{
			return e_error_code_template_param;
		}
		else
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_wing_zhuling, 0, use_up_item, item_num, e_item_locked);
		}

		int32 result = 0;
		for (int32 i = 0; i + 2 < GAMECONFIG->WingZhuLingRandomRange.size(); i += 3)
		{
			if (GAMECONFIG->WingZhuLingRandomRange[i] == use_up_item)
			{
				result = random_gen::get_random(GAMECONFIG->WingZhuLingRandomRange[i + 1], GAMECONFIG->WingZhuLingRandomRange[i + 2]);
				break;
			}
		}
		set_wing_att(false);
		wing_ptr->set_data_info(e_item_info_forge_property1, wing_ptr->get_data_info(e_item_info_forge_property1) + result);
		wing_ptr->set_data_info(e_item_info_forge_property4, wing_ptr->get_data_info(e_item_info_forge_property4) + 1);
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) > 110)
	{
		wing_ptr->set_data_info(e_item_info_forge_property1, 110);
	}

	int32 wing_next_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_forge_property1) + 2000;
	WingTemplate* wing_next_temp_ptr = GET_TEMPLATE(WingTemplate, wing_next_id);
	if (wing_next_temp_ptr != nullptr)
	{
		set_wing_att(true);
		//item_change_att(wing_next_temp_ptr->AttArray, 1, true);

		/*item_proto_show_item_upgrade_effect effect_msg;
		effect_msg.set_show_type(e_item_upgrade_effect_type_wing_add_star);
		effect_msg.set_show_value(wing_ptr->get_data_info(e_item_info_forge_property1));
		temp_player_ref.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);*/
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) >= 80 && wing_ptr->get_data_info(e_item_info_forge_property5) < 80)
	{
		if (wing_next_temp_ptr->SkillPool.size() % 2 != 0)
		{
			return e_error_code_template_param;
		}
		int32 index = 0;
		int32 temp_value = 0;
		int32 random_number = random_gen::get_random(0, 99);
		for (int32 j = 0; j < wing_next_temp_ptr->SkillPool.size(); j += 2)
		{
			if (temp_value <= random_number && random_number < temp_value + wing_next_temp_ptr->SkillPool[j + 1])
			{
				index = j;
				break;
			}
			temp_value += wing_next_temp_ptr->SkillPool[j + 1];
		}

		if (!wing_psychic_skill_unlock(wing_next_temp_ptr->SkillPool[index], true))
		{
			return e_error_code_template_param;
		}
		wing_ptr->set_data_info(e_item_info_forge_property3, wing_next_temp_ptr->SkillPool[index]);
		wing_ptr->set_data_info(e_item_info_forge_property4, 0);
		wing_ptr->set_data_info(e_item_info_forge_property5, wing_ptr->get_data_info(e_item_info_forge_property1));
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) >= 110 && wing_ptr->get_data_info(e_item_info_forge_property6) == 0)
	{
		if (!wing_psychic_skill_unlock(wing_ptr->get_data_info(e_item_info_forge_property3), false))
		{
			return e_error_code_template_param;
		}
		if (!wing_psychic_skill_unlock(wing_ptr->get_data_info(e_item_info_forge_property3) + 1, true))
		{
			return e_error_code_template_param;
		}

		wing_ptr->set_data_info(e_item_info_forge_property3, wing_ptr->get_data_info(e_item_info_forge_property3) + 1);
		wing_ptr->set_data_info(e_item_info_forge_property6, 1);
	}

	send_item_one(wing_ptr);
	return e_error_code_success;
}

int32 item_set::wing_lengque(const guid_64& wing_guid, int32 use_up_item)
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return e_error_code_template_param;
	}
	citem* wing_ptr = get_item(wing_guid);
	if (nullptr == wing_ptr
		|| nullptr == wing_ptr->get_item_info_ptr())
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_activate) == 0 || wing_ptr->get_data_info(e_item_info_over_time) != 0)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_error_code_template_param;
	}

	int32 upgrade_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* wing_upgrade_template_ptr = GET_TEMPLATE(WingTemplate, upgrade_id);
	if (nullptr == wing_upgrade_template_ptr)
	{
		return e_error_code_template_param;
	}
	if (wing_upgrade_template_ptr->UpgradeNow < GAMECONFIG->WingPsychicUnlockNum)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) < 10 || wing_ptr->get_data_info(e_item_info_forge_property1) >= 50)
	{
		return e_error_code_template_param;
	}

	int32 real_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_forge_property1) + 2000;
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	if (nullptr == wing_template_ptr)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property2) == 0 || time_helper::get_cur_time_new().second - wing_ptr->get_data_info(e_item_info_forge_property2) >= 0)
	{
		return e_error_code_template_param;
	}

	int32 money_times = (wing_ptr->get_data_info(e_item_info_forge_property1) / 10 - 1) * 3;

	if (GAMECONFIG->WingZhuLingMoneyCost.size() < 12 || money_times < 0 || money_times > 9)
	{
		return e_error_code_template_param;
	}

	int32 money_id = GAMECONFIG->WingZhuLingMoneyCost[money_times + 1];
	int32 money_num = GAMECONFIG->WingZhuLingMoneyCost[money_times + 2];
	if (!temp_player_ref.can_cut_money((e_money_type)money_id, money_num))
	{
		return e_error_code_template_param;
	}
	temp_player_ref.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_lengque);
	wing_ptr->set_data_info(e_item_info_forge_property2, 0);

	send_item_one(wing_ptr);
	return e_error_code_success;
}

int32 item_set::wing_xiling(const guid_64& wing_guid, int32 use_up_item)
{
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (!temp_player_ref.is_valid())
	{
		return e_error_code_template_param;
	}
	citem* wing_ptr = get_item(wing_guid);
	if (nullptr == wing_ptr
		|| nullptr == wing_ptr->get_item_info_ptr())
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_activate) == 0 || wing_ptr->get_data_info(e_item_info_over_time) != 0)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_error_code_template_param;
	}

	int32 upgrade_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* wing_upgrade_template_ptr = GET_TEMPLATE(WingTemplate, upgrade_id);
	if (nullptr == wing_upgrade_template_ptr)
	{
		return e_error_code_template_param;
	}
	if (wing_upgrade_template_ptr->UpgradeNow < GAMECONFIG->WingPsychicUnlockNum)
	{
		return e_error_code_template_param;
	}

	if (wing_ptr->get_data_info(e_item_info_forge_property1) < 50 || wing_ptr->get_data_info(e_item_info_forge_property1) > 110)
	{
		return e_error_code_template_param;
	}

	int32 real_id = wing_ptr->get_item_info_ptr()->logic_id + wing_ptr->get_data_info(e_item_info_forge_property1) + 2000;
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	if (nullptr == wing_template_ptr)
	{
		return e_error_code_template_param;
	}

	if (GAMECONFIG->WingXiLingMoneyCost.size() < 6)
	{
		return e_error_code_template_param;
	}

	int32 money_times = 0;
	if (wing_ptr->get_data_info(e_item_info_forge_property1) >= 50 && wing_ptr->get_data_info(e_item_info_forge_property1) < 80)
	{
		money_times = 0;
		int32 money_id = GAMECONFIG->WingXiLingMoneyCost[money_times + 1];
		int32 money_num = GAMECONFIG->WingXiLingMoneyCost[money_times + 2];
		if (!temp_player_ref.can_cut_money((e_money_type)money_id, money_num))
		{
			return e_error_code_template_param;
		}
		set_wing_att(false);
		temp_player_ref.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_xiling);
		wing_ptr->set_data_info(e_item_info_forge_property4, 0);
		wing_ptr->set_data_info(e_item_info_forge_property1, 50);
		set_wing_att(true);
	}
	else
	{
		money_times = 3;
		int32 money_id = GAMECONFIG->WingXiLingMoneyCost[money_times + 1];
		int32 money_num = GAMECONFIG->WingXiLingMoneyCost[money_times + 2];
		if (!temp_player_ref.can_cut_money((e_money_type)money_id, money_num))
		{
			return e_error_code_template_param;
		}

		if (use_up_item == 0)
		{
			if (wing_template_ptr->SkillPool.size() % 2 != 0)
			{
				return e_error_code_template_param;
			}
			int32 index = 0;
			int32 temp_value = 0;
			int32 random_number = random_gen::get_random(0, 99);
			for (int32 j = 0; j < wing_template_ptr->SkillPool.size(); j += 2)
			{
				if (temp_value <= random_number && random_number < temp_value + wing_template_ptr->SkillPool[j + 1])
				{
					index = j;
					break;
				}
				temp_value += wing_template_ptr->SkillPool[j + 1];
			}

			if (!wing_psychic_skill_unlock(wing_ptr->get_data_info(e_item_info_forge_property3), false))
			{
				return e_error_code_template_param;
			}
			if (!wing_psychic_skill_unlock(wing_template_ptr->SkillPool[index], true))
			{
				return e_error_code_template_param;
			}
			wing_ptr->set_data_info(e_item_info_forge_property3, wing_template_ptr->SkillPool[index]);
		}
		else
		{
			if (wing_ptr->get_data_info(e_item_info_forge_property1) >= 110)
			{
				if (!wing_psychic_skill_unlock(wing_ptr->get_data_info(e_item_info_forge_property3), false))
				{
					return e_error_code_template_param;
				}
				if (!wing_psychic_skill_unlock(wing_ptr->get_data_info(e_item_info_forge_property3) - 1, true))
				{
					return e_error_code_template_param;
				}
				wing_ptr->set_data_info(e_item_info_forge_property3, wing_ptr->get_data_info(e_item_info_forge_property3) - 1);
			}
		}

		set_wing_att(false);
		temp_player_ref.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_xiling);
		wing_ptr->set_data_info(e_item_info_forge_property4, 0);
		wing_ptr->set_data_info(e_item_info_forge_property6, 0);
		wing_ptr->set_data_info(e_item_info_forge_property1, wing_ptr->get_data_info(e_item_info_forge_property5));
		set_wing_att(true);
	}

	send_item_one(wing_ptr);
	return e_error_code_success;
}

int32 item_set::wing_illusionupgrade(const guid_64& wing_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("wing", "wing_illusionupgrade", 1, false, "%d%s%d>%d", m_unit_array_index, wing_guid.to_string().c_str(), use_up_item, &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;
	int32 matirial_count = 0;
	if (!wing_guid.is_valid())
	{
		return e_item_string_unkown;
	}

	citem* wing_ptr = get_item(wing_guid);
	if (wing_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	if (wing_ptr->get_data_info(e_item_info_activate) <= 0)
	{
		return e_item_string_mount_unlock;
	}
	ItemTemplate* temp_item_template_ptr = wing_ptr->get_item_info_ptr();
	if (temp_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	int32 wing_real_id = temp_item_template_ptr->logic_id + wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* real_wing_template_ptr = GET_TEMPLATE(WingTemplate, wing_real_id);
	if (real_wing_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	if (real_wing_template_ptr->Type != e_item_wing_type_illusion)
	{
		return e_item_string_auto_use_money_not_enough;
	}

	if (real_wing_template_ptr->UpgradeNow >= real_wing_template_ptr->WingUpgradeMax)
	{
		return e_item_string_full_level;
	}

	int32 temp_cur_value = temp_player.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_mount);
	temp_player.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1);

	matirial_count = get_item_count(real_wing_template_ptr->UpgradeMatirialId, e_bag_type_bag);
	if (matirial_count < real_wing_template_ptr->UpgradeMatirialNum)
	{
		if (use_up_item == 0)
		{
			return e_item_string_matirial_not_enough;
		}
		else
		{
			use_money_instead_matirial = true;
		}

	}

	if (use_money_instead_matirial)
	{
		GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, real_wing_template_ptr->UpgradeShopId);
		if (goods_ptr != nullptr
			&& goods_ptr->NewPrice.size() > region_money_num_index)
		{
			int32 money_id = goods_ptr->NewPrice[region_money_type_index];
			int32 money_num = goods_ptr->NewPrice[region_money_num_index] * real_wing_template_ptr->UpgradeMatirialNum;
			if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
			{
				return e_item_string_auto_use_money_not_enough;
			}
			temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_mount_upgrade_auto_buy, real_wing_template_ptr->UpgradeMatirialNum);
		}
		else
		{
			return e_item_string_no_item;
		}
	}
	else
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_wing_illusionupgrade , 0, real_wing_template_ptr->UpgradeMatirialId, real_wing_template_ptr->UpgradeMatirialNum, e_item_locked);
	}

	item_change_att(real_wing_template_ptr->AttArray, 1, false);
	int32 wing_upgrade_now = wing_ptr->get_data_info(e_item_info_upgrade_count);
	wing_ptr->set_data_info(e_item_info_upgrade_count, wing_upgrade_now + 1);
	if (wing_ptr->get_wing_template_ptr())
	{
		item_change_att(wing_ptr->get_wing_template_ptr()->AttArray, 1, true);

		// wangsonghao
		if (wing_ptr->get_wing_template_ptr()->StarBuffId > 0)
		{
			star_skill_unlock(wing_guid, wing_ptr->get_data_info(e_item_info_upgrade_count), false, false);
		}
	}
	else
	{
		wing_ptr->set_data_info(e_item_info_upgrade_count, wing_upgrade_now);
		item_change_att(wing_ptr->get_wing_template_ptr()->AttArray, 1, true);
	}
	send_item_one(wing_ptr);

	item_proto_show_item_upgrade_effect effect_msg;
	WingTemplate* final_template_ptr = wing_ptr->get_wing_template_ptr();
	if (final_template_ptr != nullptr)
	{
		effect_msg.set_show_type(e_item_upgrade_effect_type_wing_upgrade);
		effect_msg.set_show_value(final_template_ptr->UpgradeNow);
		temp_player.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);
	}
	return e_error_code_success;
	*/
}

int32 item_set::wing_shape_upgrade(citem& ref_wing_item_shape, int32 use_up_item)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}

	const WingTemplate* wing_template_ptr = get_wing_template_ptr(ref_wing_item_shape);
	if (nullptr == wing_template_ptr)
	{
		return e_item_string_unkown;
	}

	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	const int32 cur_wing_star_num = wing_template_ptr->StarNow;
	const int32 max_wing_star_num = wing_template_ptr->WingAddStarMax;
	const int32 cur_wing_upgrade_num = wing_template_ptr->UpgradeNow;
	const int32 max_wing_upgrade_num = wing_template_ptr->WingUpgradeMax;

	if ((cur_wing_star_num == max_wing_star_num) && (cur_wing_upgrade_num == max_wing_upgrade_num))
	{
		return e_item_string_full_level;
	}

	bool money_cost = false;
	bool use_money_instead_matirial = false;
	bool matirial_unlock = false;

	const int32 active_state = ref_wing_item_shape.get_data_info(e_item_info_activate);
	set_wing_att(false);

	if (0 == active_state)
	{
		if (wing_template_ptr->UnlockNeedMoney.size() >= 2)
		{
			if (!temp_player.can_cut_money((e_money_type)wing_template_ptr->UnlockNeedMoney[0], wing_template_ptr->UnlockNeedMoney[1]))
			{
				return e_item_string_money;
			}
			money_cost = true;
		}

		if (wing_template_ptr->UnlockNeedMatirial.size() >= 2)
		{
			if (get_item_count(e_bag_type_bag, wing_template_ptr->UnlockNeedMatirial[0]) < wing_template_ptr->UnlockNeedMatirial[1])
			{
				return e_item_string_matirial_not_enough;
			}
			matirial_unlock = true;
		}

		ref_wing_item_shape.set_data_info(e_item_info_activate, 1);
	}
	else
	{
		if (cur_wing_star_num == max_wing_star_num)
		{
			if (get_item_count(e_bag_type_bag, wing_template_ptr->UpgradeMatirialId) < wing_template_ptr->UpgradeMatirialNum)
			{
				if (use_up_item == 0)
				{
					return e_item_string_matirial_not_enough;
				}
				else
				{
					use_money_instead_matirial = true;
				}
			}
			if (use_money_instead_matirial)
			{
				GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, wing_template_ptr->UpgradeShopId);
				if (goods_ptr != nullptr
					&& goods_ptr->NewPrice.size() > region_money_num_index)
				{
					int32 money_id = goods_ptr->NewPrice[region_money_type_index];
					int32 money_num = goods_ptr->NewPrice[region_money_num_index] * wing_template_ptr->UpgradeMatirialNum;
					if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
					{
						return e_item_string_auto_use_money_not_enough;
					}
					temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_upgrade_auto_buy, wing_template_ptr->UpgradeShopId);
				}
				else
				{
					return e_item_string_no_item;
				}
			}
			else
			{
				cost_item_by_id_with_lock_states(e_server_log_del_item_wing_shape_upgrade, 0, wing_template_ptr->UpgradeMatirialId, wing_template_ptr->UpgradeMatirialNum, e_item_locked);
			}

			ref_wing_item_shape.set_data_info(e_item_info_upgrade_count, ref_wing_item_shape.get_data_info(e_item_info_upgrade_count) + 1);

			item_proto_show_item_upgrade_effect effect_msg;
			effect_msg.set_show_type(e_item_upgrade_effect_type_wing_upgrade);
			effect_msg.set_show_value(cur_wing_upgrade_num + 1);
			temp_player.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);

		}
		else
		{
			if (get_item_count(e_bag_type_bag, wing_template_ptr->AddStarMatirialId) < wing_template_ptr->AddStarMatirialNum)
			{
				if (use_up_item == 0)
				{
					return e_item_string_matirial_not_enough;
				}
				else
				{
					use_money_instead_matirial = true;
				}
			}
			if (use_money_instead_matirial)
			{
				GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, wing_template_ptr->StarShopId);
				if (goods_ptr != nullptr
					&& goods_ptr->NewPrice.size() > region_money_num_index)
				{
					int32 money_id = goods_ptr->NewPrice[region_money_type_index];
					int32 money_num = goods_ptr->NewPrice[region_money_num_index] * wing_template_ptr->AddStarMatirialNum;
					if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
					{
						return e_item_string_auto_use_money_not_enough;
					}
					temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_wing_add_star_auto_buy, wing_template_ptr->StarShopId);
				}
				else
				{
					return e_item_string_no_item;
				}
			}
			else
			{
				cost_item_by_id_with_lock_states(e_server_log_del_item_wing_shape_upgrade, 0, wing_template_ptr->AddStarMatirialId, wing_template_ptr->AddStarMatirialNum, e_item_locked);
			}

			ref_wing_item_shape.set_data_info(e_item_info_upgrade_count, ref_wing_item_shape.get_data_info(e_item_info_upgrade_count) + 1);

			item_proto_show_item_upgrade_effect effect_msg;
			effect_msg.set_show_type(e_item_upgrade_effect_type_wing_add_star);
			effect_msg.set_show_value(cur_wing_star_num + 1);
			temp_player.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);

		}
	}
	if (money_cost)
	{
		if (wing_template_ptr->UnlockNeedMoney.size() >= 2)
		{
			temp_player.cut_money((e_money_type)wing_template_ptr->UnlockNeedMoney[0], wing_template_ptr->UnlockNeedMoney[1], e_server_log_cut_money_wing_upgrade, ref_wing_item_shape.get_data_info(e_item_info_info_id));
		}
		else
		{
			return e_item_string_unkown;
		}
	}
	if (matirial_unlock)
	{
		if (wing_template_ptr->UnlockNeedMatirial.size() >= 2)
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_wing_shape_upgrade, 0, wing_template_ptr->UnlockNeedMatirial[0], wing_template_ptr->UnlockNeedMatirial[1], e_item_locked);
		}
		else
		{
			return e_item_string_unkown;
		}
	}
	set_wing_att(true);
	send_item_one(&ref_wing_item_shape);

	WingTemplate* new_wing_template_ptr = get_wing_template_ptr(ref_wing_item_shape);
	if (new_wing_template_ptr != nullptr)
	{
		if (new_wing_template_ptr->NoticeId > 0)
		{
			s_chat_notice_info notice_data;
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(new_wing_template_ptr->WingNameId);
			notice_data.item_name.push_back(temp_string);
			notice_data.num_type_data.push_back(new_wing_template_ptr->UpgradeNow);
			temp_player.get_chat_mgr().send_notice_new(new_wing_template_ptr->NoticeId, notice_data);
		}
	}

	return e_error_code_success;
}

int32 item_set::wing_shape_unlock(const guid_64& wing_guid, int32 item_slot)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("wing", "wing_shape_unlock", 1, false, "%d%s%d>%d", m_unit_array_index, wing_guid.to_string().c_str(), item_slot, &result);
	return result;
	/*
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (temp_player_ref.is_valid() == false)
	{
		return e_item_string_unkown;
	}

	citem* item_wing_ptr = get_item(wing_guid);
	if (nullptr == item_wing_ptr)
	{
		return e_item_string_no_item;
	}

	bool money_cost = false;
	bool matirial_unlock = false;

	WingTemplate* wing_template_ptr = item_wing_ptr->get_wing_template_ptr();
	if (nullptr == wing_template_ptr)
	{
		return e_item_string_no_item;
	}

	if (0 == item_wing_ptr->get_data_info(e_item_info_activate))
	{
		if (wing_template_ptr->UnlockNeedMoney.size() >= e_money_tuple_max && wing_template_ptr->UnlockNeedMoney.size() % e_money_tuple_max == 0)
		{
			if (!temp_player_ref.can_cut_money((e_money_type)wing_template_ptr->UnlockNeedMoney[e_money_tuple_id], wing_template_ptr->UnlockNeedMoney[e_money_tuple_num]))
			{
				return e_item_string_money;
			}
			money_cost = true;
		}

		if (wing_template_ptr->UnlockNeedMatirial.size() >= 2 && wing_template_ptr->UnlockNeedMatirial.size() % 2 == 0)
		{
			if (get_item_count(wing_template_ptr->UnlockNeedMatirial[0], e_bag_type_bag) < wing_template_ptr->UnlockNeedMatirial[1])
			{
				return e_item_string_matirial_not_enough;
			}
			matirial_unlock = true;
		}
	}
	else
	{
		return e_item_string_cant_unlock_again;
	}

	if (money_cost)
	{
		if (wing_template_ptr->UnlockNeedMoney.size() >= e_money_tuple_max)
		{
			temp_player_ref.cut_money((e_money_type)wing_template_ptr->UnlockNeedMoney[e_money_tuple_id], wing_template_ptr->UnlockNeedMoney[e_money_tuple_num], e_server_log_cut_money_wing_upgrade, item_wing_ptr->get_data_info(e_item_info_info_id));
		}
		else
		{
			return e_item_string_unkown;
		}
	}

	if (matirial_unlock)
	{
		if (wing_template_ptr->UnlockNeedMatirial.size() >= e_money_tuple_max)
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_wing_unlock , 0, wing_template_ptr->UnlockNeedMatirial[e_money_tuple_id], wing_template_ptr->UnlockNeedMatirial[e_money_tuple_num], e_item_locked);
		}
		else
		{
			return e_item_string_unkown;
		}
	}

	set_wing_att(false);
	item_wing_ptr->set_data_info(e_item_info_activate, 1);
	set_wing_att(true);
	if (wing_template_ptr->StarBuffId > 0)
	{
		PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, wing_template_ptr->StarBuffId);
		if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
		{
			star_skill_unlock(wing_guid, item_wing_ptr->get_data_info(e_item_info_upgrade_count), false, false);
		}
	}
	send_item_one(item_wing_ptr);
	show_this_wing(item_wing_ptr, item_slot);

	temp_player_ref.get_phantom_mgr().on_item_oper(item_wing_ptr, item_wing_ptr->get_data_info(e_item_info_info_id));


	return e_error_code_success;
	*/
}

int32 item_set::wing_feather_unlock(const guid_64& wing_guid, int32 item_slot)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("feather_heart", "wing_feather_unlock", 1, false, "%d%s%d>%d", m_unit_array_index, wing_guid.to_string().c_str(), item_slot, &result);
	return result;
	/*player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (temp_player_ref.is_valid() == false)
	{
		return e_item_string_unkown;
	}

	citem* item_ptr = get_item(wing_guid);
	if (nullptr == item_ptr)
	{
		return e_item_string_no_item;
	}
	if (item_ptr->get_data_info(e_item_info_activate) > 0)
	{
		return e_item_string_unkown;
	}
	ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return e_item_string_unkown;
	}
	FeatherTemplate* feather_template_ptr = GET_TEMPLATE(FeatherTemplate, item_template_ptr->logic_id);
	if (nullptr == feather_template_ptr)
	{
		return e_item_string_unkown;
	}
	int32 unlock_need_material_num = feather_template_ptr->UnlockMaterialNum;
	int32 cur_owned_material_num = get_item_count(feather_template_ptr->UnlockMaterialId, e_bag_type_bag);
	if (cur_owned_material_num < unlock_need_material_num)
	{
		return e_item_string_matirial_not_enough;
	}
	cost_item_by_id_with_lock_states(e_server_log_del_item_wing_feather_unlock, 0, feather_template_ptr->UnlockMaterialId, unlock_need_material_num, e_item_locked);
	set_wing_att(false);
	item_ptr->set_data_info(e_item_info_activate, 1);
	set_wing_att(true);
	if (feather_template_ptr->SkillID > 0)
	{
		PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, feather_template_ptr->SkillID);
		if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
		{
			star_skill_unlock(wing_guid, item_ptr->get_data_info(e_item_info_upgrade_count), false, false);
		}
	}
	send_item_one(item_ptr);
	temp_player_ref.get_feather_heart_mgr().sync_ranking_feather_heart_data();
	return e_error_code_success;*/
}

int32 item_set::wing_shape_illusion_unlock(const guid_64& wing_guid, const int32 illusion_slot)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("wing", "wing_shape_illusion_unlock", 1, false, "%d%s%d>%d", m_unit_array_index, wing_guid.to_string().c_str(), illusion_slot, &result);
	return result;
	/*
	player& temp_player_ref = unit_man::get_player(m_unit_array_index);
	if (temp_player_ref.is_valid() == false)
	{
		return e_item_string_unkown;
	}

	citem* temp_wing_ptr = get_item(wing_guid);

	if (temp_wing_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	WingTemplate* temp_wing_template_ptr = temp_wing_ptr->get_wing_template_ptr();
	if (temp_wing_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	const int32 current_wing_basic_level = temp_wing_ptr->get_data_info(e_item_info_upgrade_count);
	const int32 current_wing_basic_actived = temp_wing_ptr->get_data_info(e_item_info_activate);

	if (current_wing_basic_actived == 0)
	{
		return e_item_string_unkown;
	}

	if (illusion_slot >= temp_wing_template_ptr->IIllusionList.size())
	{
		return e_item_string_have_no_illusion;
	}

	const int32 wing_illusion_item_template_id = temp_wing_template_ptr->IIllusionList[illusion_slot];

	if (get_item_by_template_id(wing_illusion_item_template_id))
	{
		return e_item_string_cant_unlock_again;
	}

	ItemTemplate* temp_wing_illusion_item_ptr = GET_TEMPLATE(ItemTemplate, wing_illusion_item_template_id);
	if (temp_wing_illusion_item_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	const int32 illusion_wing_template = temp_wing_illusion_item_ptr->logic_id;
	WingTemplate* temp_illusion_wing_template_ptr = GET_TEMPLATE(WingTemplate, illusion_wing_template);
	if (temp_illusion_wing_template_ptr == nullptr)
	{
		return e_item_string_have_no_illusion;
	}

	WingTemplate* real_wing_template_ptr = GET_TEMPLATE(WingTemplate, temp_wing_template_ptr->attribute_id);
	if (real_wing_template_ptr == nullptr)
	{
		return e_item_string_have_no_illusion;
	}
	if (temp_illusion_wing_template_ptr->PreWingRequirement.size() >= 2)
	{
		if (real_wing_template_ptr->UpgradeNow < temp_illusion_wing_template_ptr->PreWingRequirement[1])
		{
			return e_item_string_unkown;
		}
	}

	//const int32 wing_illusion_open_class = temp_illusion_wing_template_ptr->IllusionOpenClass;
	//if (current_wing_basic_actived == 0 || current_wing_basic_level + 1 < wing_illusion_open_class)
	//{
	//return e_item_string_unkown;
	//}

	if (temp_illusion_wing_template_ptr->UnlockNeedMatirial.size() < e_item_tuple_max
		|| temp_illusion_wing_template_ptr->UnlockNeedMatirial.size() % e_item_tuple_max != 0)
	{
		return e_item_string_unkown;
	}

	bool is_cut_money = false;
	if (temp_illusion_wing_template_ptr->UnlockNeedMoney.size() >= e_money_tuple_max)
	{
		if (!temp_player_ref.can_cut_money((e_money_type)temp_illusion_wing_template_ptr->UnlockNeedMoney[e_money_tuple_id], temp_illusion_wing_template_ptr->UnlockNeedMoney[e_money_tuple_num]))
		{
			return e_item_string_money;
		}
		is_cut_money = true;
	}

	const bool has_enough_item = get_item_count(temp_illusion_wing_template_ptr->UnlockNeedMatirial[e_item_tuple_id], e_bag_type_bag) >= temp_illusion_wing_template_ptr->UnlockNeedMatirial[e_money_tuple_num];
	if (false == has_enough_item)
	{
		return e_item_string_matirial_not_enough;
	}

	citem* illusion_item = create_item_by_template(e_server_log_add_item_wing_iilusion, 0, wing_illusion_item_template_id, 1, 0);
	if (illusion_item == nullptr)
	{
		return e_item_string_unkown;
	}

	cost_item_by_id_with_lock_states(e_server_log_del_item_wing_iilusion , 0, temp_illusion_wing_template_ptr->UnlockNeedMatirial[0], temp_illusion_wing_template_ptr->UnlockNeedMatirial[1], e_item_locked);
	if (is_cut_money)
	{
		temp_player_ref.cut_money((e_money_type)temp_illusion_wing_template_ptr->UnlockNeedMoney[e_money_tuple_id], temp_illusion_wing_template_ptr->UnlockNeedMoney[e_money_tuple_num], e_server_log_cut_money_get_wing_illusion);
	}
	set_wing_att(false);
	illusion_item->set_data_info(e_item_info_activate, 1);
	set_wing_att(true);
	send_item_one(illusion_item);
	show_this_wing(temp_wing_ptr, illusion_slot);

	return e_error_code_success;
	*/
}

WingTemplate* item_set::get_wing_template_ptr(citem& ref_wing_item_shape)
{
	ItemTemplate* item_template_ptr = ref_wing_item_shape.get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return nullptr;
	}
	int32 real_id = item_template_ptr->logic_id + ref_wing_item_shape.get_data_info(e_item_info_upgrade_count);
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	return wing_template_ptr;
}

void  item_set::sync_ranking_wing()
{
	player& m_player_ptr = unit_man::get_player(m_unit_array_index);
	if (false == m_player_ptr.is_valid())
	{
		return;
	}
	int64 wing_value_sum = 0;
	int64 max_fight_value = 0;
	int64 max_normal_num = 0;
	citem* highest_wing_item = nullptr;
	for (int32 index = 0; index < MAX_WING_ELEMENTS_NUM; index++)
	{
		citem* temp = get_item_by_slot(e_bag_type_wing, index);
		if (temp != nullptr)
		{
			if (temp->get_data_info(e_item_info_activate) > 0)
			{
				if (temp->get_wing_template_ptr() != nullptr)
				{
					int32 cur_value = temp->calcu_single_item_fighting_power(m_unit_array_index);
					switch ((e_item_wing_type)temp->get_wing_template_ptr()->Type)
					{
					case e_item_wing_type_normal:
					{
						if (cur_value > max_normal_num)
						{
							max_normal_num = cur_value;
						}
					}
					break;
					case e_item_wing_type_shape_no_illusion:
					case e_item_wing_type_shape_illusion:
					{
						wing_value_sum += cur_value;
						if (temp->get_wing_template_ptr()->WingAddStarMax > 1)
						{
							if (cur_value >= max_fight_value)
							{
								max_fight_value = cur_value;
								highest_wing_item = temp;
							}
						}
					}
					break;
					case e_item_wing_type_illusion:
					case e_item_wing_type_special_santo:
					{
						wing_value_sum += cur_value;
					}
					break;
					default:
						break;
					}
				}
			}
		}
	}
	wing_value_sum += max_normal_num;

	wing_value_sum += calcu_beast_sprite_fighting_power(e_bag_type_wing_beast_spirit);

	m_player_ptr.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_wing, wing_value_sum);

	if (m_player_ptr.check_and_set_highest_record(e_role_history_highest_record_wing_value, wing_value_sum) == true)
	{
		m_player_ptr.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_wing_up);
		m_player_ptr.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_cross_wing_up);
	}
	if (highest_wing_item != nullptr)
	{
		m_player_ptr.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_wing_train, max_fight_value, highest_wing_item->get_item_template_id());
	}
}

bool item_set::wing_upgrade_rate(int32 bless, WingTemplate* wing_template_ptr)
{
	if (wing_template_ptr == nullptr)
	{
		return false;
	}
	if (bless < wing_template_ptr->MinUpgradeBless)
	{
		return false;
	}
	if (bless >= wing_template_ptr->MaxUpgradeBless)
	{
		return true;
	}

	//if (rand() % 100 >(((bless - wing_template_ptr->MinUpgradeBless) * wing_template_ptr->RateChange + wing_template_ptr->BasicRate) * 100))
	//{
	//	return true;
	//}
	if (wing_template_ptr->EachUpgradeBless <= 0)
	{
		return false;
	}
	if ((rand() % 100)
		<= (100 * (wing_template_ptr->BasicRate + ((bless - wing_template_ptr->MinUpgradeBless) / wing_template_ptr->EachUpgradeBless) * wing_template_ptr->RateChange)))
	{
		return true;
	}
	return false;
}

int32 item_set::wing_add_soul(const guid_64& add_soul_guid)
{

	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* item_ptr = get_item(add_soul_guid);
	if (nullptr == item_ptr)
	{
		return e_item_string_unkown;
	}
	if (nullptr == item_ptr->get_item_info_ptr())
	{
		return e_item_string_unkown;
	}
	if (item_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_item_string_unkown;
	}

	if (item_ptr->get_item_info_ptr()->sub_type != e_wing_sub_type_soul)
	{
		return e_item_string_unkown;
	}

	int32 now_soul_count = item_ptr->get_data_info(e_item_info_upgrade_count);
	int32 real_id = item_ptr->get_item_info_ptr()->logic_id + now_soul_count;
	WingAddSoulTemplate* wing_add_soul_ptr = GET_TEMPLATE(WingAddSoulTemplate, real_id);
	if (nullptr == wing_add_soul_ptr)
	{
		return e_item_string_unkown;
	}
	WingAddSoulTemplate* next_wing_add_soul_ptr = GET_TEMPLATE(WingAddSoulTemplate, real_id + 1);
	if (nullptr == next_wing_add_soul_ptr)
	{
		return e_item_string_full_level;
	}

	citem* wing_item_ptr = get_cur_level_wing();
	if (nullptr == wing_item_ptr)
	{
		return e_item_string_unkown;
	}
	//if ((wing_item_ptr->get_data_info(e_item_info_slot)+1) < ADD_SOUL_OPEN)
	//	return e_item_string_unkown;
	//}
	if (get_item_count(e_bag_type_bag, wing_add_soul_ptr->AddSoulMatirialId) < wing_add_soul_ptr->AddSoulMatirialNum)
	{
		return e_item_string_matirial_not_enough;
	}
	if (wing_add_soul_ptr->AddSoulMoney.size() < 2)
	{
		return e_item_string_unkown;
	}
	if (false == temp_player.can_cut_money((e_money_type)wing_add_soul_ptr->AddSoulMoney[0], wing_add_soul_ptr->AddSoulMoney[1]))
	{
		return e_item_string_money;
	}

	cost_item_by_id_with_lock_states(e_server_log_del_item_add_soul, 0, wing_add_soul_ptr->AddSoulMatirialId, wing_add_soul_ptr->AddSoulMatirialNum, e_item_locked);
	temp_player.cut_money((e_money_type)wing_add_soul_ptr->AddSoulMoney[0], wing_add_soul_ptr->AddSoulMoney[1], e_server_log_cut_money_add_soul, real_id);
	set_wing_att(false);
	item_ptr->set_data_info(e_item_info_upgrade_count, now_soul_count + 1);
	send_item_one(item_ptr);
	set_wing_att(true);
	return e_error_code_success;
}

int32 item_set::wing_add_spirit(const guid_64& add_spirit_guid)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* item_ptr = get_item(add_spirit_guid);
	if (nullptr == item_ptr)
	{
		return e_item_string_unkown;
	}
	if (nullptr == item_ptr->get_item_info_ptr())
	{
		return e_item_string_unkown;
	}
	if (item_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_item_string_unkown;
	}

	if (item_ptr->get_item_info_ptr()->sub_type != e_wing_sub_type_spirit)
	{
		return e_item_string_unkown;
	}

	int32 now_spirit_count = item_ptr->get_data_info(e_item_info_upgrade_count);
	int32 real_id = item_ptr->get_item_info_ptr()->logic_id + now_spirit_count;
	WingAddSpiritTemplate* wing_add_spirit_ptr = GET_TEMPLATE(WingAddSpiritTemplate, real_id);
	if (nullptr == wing_add_spirit_ptr)
	{
		return e_item_string_unkown;
	}

	WingAddSpiritTemplate* next_wing_add_spirit_ptr = GET_TEMPLATE(WingAddSpiritTemplate, real_id + 1);
	if (nullptr == next_wing_add_spirit_ptr)
	{
		return e_item_string_full_level;
	}

	citem* wing_item_ptr = get_cur_level_wing();
	if (nullptr == wing_item_ptr)
	{
		return e_item_string_unkown;
	}
	//if ((wing_item_ptr->get_data_info(e_item_info_slot) + 1) < ADD_SPIRIT_OPEN)
	//	return e_item_string_unkown;
	//}

	if (get_item_count(e_bag_type_bag, wing_add_spirit_ptr->AddSpiritMatirialId) < wing_add_spirit_ptr->AddSpiritMatirialNum)
	{
		return e_item_string_matirial_not_enough;
	}
	if (wing_add_spirit_ptr->AddSpiritMoney.size() < 2)
	{
		return e_item_string_unkown;
	}
	if (false == temp_player.can_cut_money((e_money_type)wing_add_spirit_ptr->AddSpiritMoney[0], wing_add_spirit_ptr->AddSpiritMoney[1]))
	{
		return e_item_string_money;
	}

	cost_item_by_id_with_lock_states(e_server_log_del_item_add_sprite, 0, wing_add_spirit_ptr->AddSpiritMatirialId, wing_add_spirit_ptr->AddSpiritMatirialNum, e_item_locked);

	temp_player.cut_money((e_money_type)wing_add_spirit_ptr->AddSpiritMoney[0], wing_add_spirit_ptr->AddSpiritMoney[1], e_server_log_cut_money_add_sprite, real_id);
	set_wing_att(false);
	item_ptr->set_data_info(e_item_info_upgrade_count, now_spirit_count + 1);
	send_item_one(item_ptr);
	set_wing_att(true);
	return e_error_code_success;
}

int32 item_set::feather_upgrade(const guid_64& feather_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("feather_heart", "feather_upgrade", 1, false, "%d%s%d>%d", m_unit_array_index, feather_guid.to_string().c_str(), use_up_item, &result);
	return result;

	/*player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* feather_ptr = get_item(feather_guid);
	if (nullptr == feather_ptr)
	{
		return e_item_string_unkown;
	}
	if (feather_ptr->get_data_info(e_item_info_activate) != 1)
	{
		return e_item_string_unkown;
	}
	if (nullptr == feather_ptr->get_item_info_ptr())
	{
		return e_item_string_unkown;
	}
	if (feather_ptr->get_item_info_ptr()->item_type != e_item_type_wing)
	{
		return e_item_string_unkown;
	}
	if (feather_ptr->get_item_info_ptr()->sub_type != e_wing_sub_type_feather)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	int32 now_feather_count = feather_ptr->get_data_info(e_item_info_upgrade_count);
	int32 real_id = feather_ptr->get_item_info_ptr()->logic_id + now_feather_count;
	FeatherTemplate* feather_upgrade_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
	if (nullptr == feather_upgrade_ptr)
	{
		return e_item_string_unkown;
	}
	int32 cur_star_num = feather_upgrade_ptr->AddStarNow;
	int32 max_star_num = feather_upgrade_ptr->AddStarMax;
	int32 cur_level_num = feather_upgrade_ptr->UpgradeNow;
	int32 max_level_num = feather_upgrade_ptr->UpgradeMax;

	if ((cur_star_num == max_star_num) && (cur_level_num == max_level_num))
	{
		return e_item_string_full_level;
	}

	FeatherTemplate* next_feather_upgrade_ptr = GET_TEMPLATE(FeatherTemplate, real_id + 1);
	if (nullptr == next_feather_upgrade_ptr)
	{
		return e_item_string_unkown;
	}


	citem* cur_wing_ptr = get_cur_level_wing();
	if (nullptr == cur_wing_ptr)
	{
		return e_item_string_unkown;
	}
	//int32 mount_upgrade_now = wing_ptr->get_data_info(e_item_info_upgrade_count);
	//if ((cur_wing_ptr->get_data_info(e_item_info_slot) + 1) < FEATHER_EQUIP_OPEN)
	//	return e_item_string_unkown;
	//}

	bool is_use_jewel_instead_matirial = false;

	if (cur_star_num == max_star_num)
	{
		if (get_item_count(feather_upgrade_ptr->UpgradeMatirialId, e_bag_type_bag) < feather_upgrade_ptr->UpgradeMatirialNum)
		{
			if (use_up_item == 0)
			{
				return e_item_string_matirial_not_enough;
			}
			else
			{
				is_use_jewel_instead_matirial = true;
			}

		}

		//if (false == temp_player.can_cut_money((e_money_type)feather_upgrade_ptr->UpgradeMoneyList[0], feather_upgrade_ptr->UpgradeMoneyList[1]))
		//	return e_item_string_money;
		//}
		if (is_use_jewel_instead_matirial == false)
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_feather_add_star , 0, feather_upgrade_ptr->UpgradeMatirialId, feather_upgrade_ptr->UpgradeMatirialNum, e_item_locked);
		}
		else
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, feather_upgrade_ptr->UpgradeShopId);
			if (goods_ptr != nullptr
				&& goods_ptr->NewPrice.size() > region_money_num_index)
			{
				int32 money_id = goods_ptr->NewPrice[region_money_type_index];
				int32 money_num = goods_ptr->NewPrice[region_money_num_index] * feather_upgrade_ptr->UpgradeMatirialNum;
				if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
				{
					return e_item_string_auto_use_money_not_enough;
				}
				temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_feather_upgrade_auto_buy, feather_upgrade_ptr->UpgradeShopId);
			}
			else
			{
				return e_item_string_no_item;
			}
		}
		//temp_player.cut_money((e_money_type)feather_upgrade_ptr->UpgradeMoneyList[0], feather_upgrade_ptr->UpgradeMoneyList[1], e_server_log_cut_money_feather_upgrade, real_id);
		set_feather_att(*feather_ptr, false);
		feather_ptr->set_data_info(e_item_info_upgrade_count, now_feather_count + 1);
		send_item_one(feather_ptr);
		set_feather_att(*feather_ptr, true);
		calcu_addition(e_addition_buff_feather);

		//mission check
		temp_player.get_mission_mgr().target_check(e_mission_end_type_wing_upgrade);
		temp_player.get_achievement_mgr().item_changed(feather_ptr, e_item_operation_featherupgrade_star);
	}
	else
	{
		if (get_item_count(feather_upgrade_ptr->AddStarMatirialId, e_bag_type_bag) < feather_upgrade_ptr->AddStarMatirialNum)
		{
			if (use_up_item == 0)
			{
				return e_item_string_matirial_not_enough;
			}
			else
			{
				is_use_jewel_instead_matirial = true;
			}
		}
		//if (false == temp_player.can_cut_money((e_money_type)feather_upgrade_ptr->AddStarMoneyList[0], feather_upgrade_ptr->AddStarMoneyList[1]))
		//	return e_item_string_money;
		//}


		if (is_use_jewel_instead_matirial == false)
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_feather_add_star , 0, feather_upgrade_ptr->AddStarMatirialId, feather_upgrade_ptr->AddStarMatirialNum, e_item_locked);
		}
		else
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, feather_upgrade_ptr->StarShopId);
			if (goods_ptr != nullptr
				&& goods_ptr->NewPrice.size() > region_money_num_index)
			{
				int32 money_id = goods_ptr->NewPrice[region_money_type_index];
				int32 money_num = goods_ptr->NewPrice[region_money_num_index] * feather_upgrade_ptr->AddStarMatirialNum;
				if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
				{
					return e_item_string_auto_use_money_not_enough;
				}
				temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_feather_add_star_auto_buy, feather_upgrade_ptr->AddStarMatirialNum);
			}
			else
			{
				return e_item_string_no_item;
			}
		}
		//temp_player.cut_money((e_money_type)feather_upgrade_ptr->AddStarMoneyList[0], feather_upgrade_ptr->AddStarMoneyList[1], e_server_log_cut_money_feather_add_star, real_id);
		set_feather_att(*feather_ptr, false);
		feather_ptr->set_data_info(e_item_info_upgrade_count, now_feather_count + 1);
		send_item_one(feather_ptr);
		set_feather_att(*feather_ptr, true);
	}
	if (next_feather_upgrade_ptr->SkillID > 0)
	{
		PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, next_feather_upgrade_ptr->SkillID);
		if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
		{
			star_skill_unlock(feather_guid, feather_ptr->get_data_info(e_item_info_upgrade_count), false, false);
		}
	}
	temp_player.get_feather_heart_mgr().sync_ranking_feather_heart_data();
	return e_error_code_success;*/
}
void item_set::equip_all_feather_and_spirit_and_soul()
{
	for (int32 i = e_feather_sub_type_feather1; i < e_feather_sub_type_max; i++)
	{
		if (get_item_by_template_id(GAMECONFIG->FirstFeatherTemplateId + i - 1))
		{
			continue;
		}
		s_item_info temp_info;
		if (init_unit::init_item_data(temp_info, GAMECONFIG->FirstFeatherTemplateId + i - 1, get_empty_slot(e_bag_type_wing), 1, e_bag_type_wing, 0))
		{
			citem* temp_feather = create_item_by_info(temp_info, e_server_log_add_item_gm_order, 0, true);
			if (temp_feather)
			{
				send_item_one(temp_feather);
			}

		}
	}
	if (!get_item_by_template_id(GAMECONFIG->WingAddSoulTemplateId))
	{
		s_item_info temp_info;
		if (init_unit::init_item_data(temp_info, GAMECONFIG->WingAddSoulTemplateId, get_empty_slot(e_bag_type_wing), 1, e_bag_type_wing, 0))
		{
			citem* temp_wing_soul = create_item_by_info(temp_info, e_server_log_add_item_gm_order, 0, true);
			if (temp_wing_soul)
			{
				send_item_one(temp_wing_soul);
			}
		}
	}
	if (!get_item_by_template_id(GAMECONFIG->WingAddSpiritTemplateId))
	{
		s_item_info temp_info;
		if (init_unit::init_item_data(temp_info, GAMECONFIG->WingAddSpiritTemplateId, get_empty_slot(e_bag_type_wing), 1, e_bag_type_wing, 0))
		{
			citem* temp_wing_spirit = create_item_by_info(temp_info, e_server_log_add_item_gm_order, 0, true);
			if (temp_wing_spirit)
			{
				send_item_one(temp_wing_spirit);
			}

		}
	}
}

void item_set::equip_all_spirit()
{
	if (!get_item_by_template_id(GAMECONFIG->WingAddSpiritTemplateId))
	{
		s_item_info temp_info;
		if (init_unit::init_item_data(temp_info, GAMECONFIG->WingAddSpiritTemplateId, get_empty_slot(e_bag_type_wing), 1, e_bag_type_wing, 0))
		{
			citem* temp_wing_spirit = create_item_by_info(temp_info, e_server_log_add_item_func_lock, 0, true);
			if (temp_wing_spirit)
			{
				send_item_one(temp_wing_spirit);
			}

		}
	}
}

void item_set::equip_all_feather()
{
	for (int32 i = e_feather_sub_type_feather1; i < e_feather_sub_type_max; i++)
	{
		if (get_item_by_template_id(GAMECONFIG->FirstFeatherTemplateId + i - 1))
		{
			continue;
		}
		s_item_info temp_info;
		if (init_unit::init_item_data(temp_info, GAMECONFIG->FirstFeatherTemplateId + i - 1, get_empty_slot(e_bag_type_wing), 1, e_bag_type_wing, 0))
		{
			citem* temp_feather = create_item_by_info(temp_info, e_server_log_add_item_func_lock, 0, true);
			if (temp_feather)
			{
				send_item_one(temp_feather);

				//set_feather_att(*temp_feather, true);
			}

		}
	}

}

void item_set::equip_all_soul()
{
	s_item_info temp_info;
	if (!get_item_by_template_id(GAMECONFIG->WingAddSoulTemplateId))
	{
		if (init_unit::init_item_data(temp_info, GAMECONFIG->WingAddSoulTemplateId, get_empty_slot(e_bag_type_wing), 1, e_bag_type_wing, 0))
		{
			citem* temp_wing_soul = create_item_by_info(temp_info, e_server_log_add_item_func_lock, 0, true);
			if (temp_wing_soul)
			{
				send_item_one(temp_wing_soul);
			}
		}
	}

}
bool item_set::judge_can_recovery(citem* item_ptr)
{
	if (nullptr == item_ptr)
	{
		return false;
	}
	ItemTemplate* item_template_ptr = item_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr)
	{
		return false;
	}
	if (item_template_ptr->item_type == e_item_type_weapon || item_template_ptr->item_type == e_item_type_armor)
	{
		if (item_ptr->get_data_info(e_item_info_upgrade_count) > 0)
		{
			return false;
		}
		if (item_ptr->get_data_info(e_item_info_add_on) > 0)
		{
			return false;
		}
		for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property4; ++i)
		{
			if (item_ptr->get_data_info((e_item_info)i) > 0)
			{
				return false;
			}
		}
		for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
		{
			if (item_ptr->get_data_info((e_item_info)i) > 0)
			{
				return false;
			}
		}
	}
	return true;
}
citem* item_set::creat_spirit_stone_by_spirit_template_id(int32 spirit_templateid)
{
	faith::template_manager::template_type* p_table = template_manager::get_instance().get_templates(e_ItemTemplate);
	SpiritTemplate* spirit_template = GET_TEMPLATE(SpiritTemplate, spirit_templateid);
	if (nullptr == p_table)
	{
		return nullptr;
	}
	if (nullptr == spirit_template)
	{
		return nullptr;
	}

	template_manager::template_type::iterator it;
	citem* item_ptr = nullptr;
	for (it = p_table->begin(); it != p_table->end(); ++it)
	{
		ItemTemplate* temp_template = (ItemTemplate*)it->second;
		if (nullptr == temp_template)
		{
			continue;
		}

		if (temp_template->item_type == e_item_type_spirit)
		{
			if (temp_template->logic_id == spirit_template->Type)
			{
				item_ptr = create_item_by_template(e_server_log_add_item_create_sprite_stone, 0, temp_template->attribute_id, 1, 0);
				if (nullptr == item_ptr)
				{
					return nullptr;
				}
				put_in_bag(item_ptr);
				break;
			}
		}

	}
	return item_ptr;
}

void item_set::put_item_into_protect_spirit_bag(const guid_64& item_guid)
{

	for (int32 i = 0; i < max_proctect_mark_bag_slot_num; ++i)
	{
		if (!GET_BAG(e_bag_type_protect)[i].is_valid())
		{
			GET_BAG(e_bag_type_protect)[i] = item_guid;
			citem* temp_ptr = get_item(item_guid);
			temp_ptr->set_data_info(e_item_info_container_type, e_bag_type_protect);
			send_item_one(temp_ptr);
			break;
		}
	}


}

citem* item_set::get_item_in_bag_with_template_id(int32 item_template_id)
{
	for (auto ite = m_item_map.begin(); ite != m_item_map.end(); )
	{
		auto temp_ite = ite++;
		if (temp_ite->second->get_data_info(e_item_info_info_id) == item_template_id
			&& temp_ite->second->get_data_info(e_item_info_container_type) == e_bag_type_bag
			)
		{
			return temp_ite->second;
		}
	}

	return nullptr;
}

int32 item_set::get_equip_smallest_upgrade_level()
{
	int32 return_value = -1;
	for (int32 i = e_role_equip_slot_hat; i < e_role_equip_slot_amulet; ++i)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
		if (temp_equip_item_ptr == nullptr)
		{
			return 0;
		}
		bool is_can_att = is_equipment_effective(temp_equip_item_ptr->get_item_guid());
		if (false == is_can_att)
		{
			continue;
		}
		if (return_value == -1
			|| temp_equip_item_ptr->get_data_info(e_item_info_upgrade_count) < return_value)
		{
			return_value = temp_equip_item_ptr->get_data_info(e_item_info_upgrade_count);
		}
	}
	return return_value;
}

int32 item_set::get_equip_num_by_color(int32 target_color)
{
	int32 return_value = 0;
	if (target_color < e_item_color_white
		|| target_color >= e_item_color_max)
	{
		return return_value;
	}

	for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_magic_2; ++i)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
		if (temp_equip_item_ptr == nullptr)
		{
			continue;
		}
		int32 item_color = temp_equip_item_ptr->get_item_color();
		if (item_color < e_item_color_white
			|| item_color >= e_item_color_max)
		{
			continue;
		}

		if (item_color >= target_color)
		{
			return_value++;
		}
	}
	return return_value;
}

guid_64 item_set::get_item_by_type(e_item_type time_type, int32 sub_type)
{
	//bool flag = false;
	guid_64 item_guid;
	citem* temp_item = nullptr;
	if (sub_type < 0)
	{
		for (auto it = m_item_map.begin(); it != m_item_map.end(); ++it)
		{
			temp_item = it->second;
			if (temp_item == nullptr)
			{
				continue;
			}
			if (temp_item->get_item_info_ptr() != nullptr)
			{
				if (temp_item->get_item_info_ptr()->item_type == time_type)
				{
					item_guid = temp_item->get_item_guid();
					break;
				}
			}
		}
	}
	else
	{
		for (auto it = m_item_map.begin(); it != m_item_map.end(); ++it)
		{
			temp_item = it->second;
			if (temp_item == nullptr)
			{
				continue;
			}
			if (temp_item->get_item_info_ptr() != nullptr)
			{
				if (temp_item->get_item_info_ptr()->item_type == time_type)
				{
					if (temp_item->get_item_info_ptr()->sub_type == sub_type)
					{
						item_guid = temp_item->get_item_guid();
						break;
					}
				}
			}
		}
	}
	return item_guid;
}

e_error_code item_set::item_succinct_is_fight_add(const guid_64& item_guid)
{
	player& temp_unit = unit_man::get_player(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return e_error_code_template_param;
	}
	citem* temp_item = get_item(item_guid);
	if (temp_item == nullptr)
	{
		return e_error_code_template_param;
	}
	EquipTemplate* equip_template_ptr = temp_item->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return e_error_code_template_param;
	}

	ItemSuccinctTemplate* temp_succinct_template = temp_item->get_succinc_info_ptr();
	if (temp_succinct_template == nullptr)
	{
		return e_error_code_template_param;
	}
	int32 gs = 0;
	for (int32 i = 0; i < item_temp_succinct_property_max && i < temp_succinct_template->PropertyID.size(); i++)
	{
		int32 property_idx = temp_succinct_template->PropertyID[i];
		int32 property_value = temp_item->get_temp_succinct_att((item_temp_succinct_property)i);
		gs += globle_data::get_instance().get_att_gs(property_idx) * property_value;
	}
	if (gs <= 0)
		return e_error_code_no_param;
	return e_error_code_success;
}
int32 item_set::item_succinct_get_effect_num(const guid_64& item_guid, int32 money_cost_type)
{
	citem* temp_item = get_item(item_guid);
	if (temp_item == nullptr)
	{
		return e_error_code_template_param;
	}
	EquipTemplate* equip_template_ptr = temp_item->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return e_error_code_template_param;
	}

	ItemSuccinctTemplate* temp_succinct_template = temp_item->get_succinc_info_ptr();
	if (temp_succinct_template == nullptr)
	{
		return e_error_code_template_param;
	}

	const vector<float>* vec_chance = &temp_succinct_template->GoldSuccinctEffectChance;
	if (money_cost_type != item_succinct_money_cost_gold)
		vec_chance = &temp_succinct_template->DiamondSuccinctEffectChance;

	int32 total_chance = 0;
	for (int32 i = 0; i < vec_chance->size(); ++i)
	{
		total_chance += (*vec_chance)[i];
	}
	int32 effect_random_value = random_gen::get_random(1, total_chance);
	int32 effect_num = 0;

	total_chance = 0;

	for (; effect_num < vec_chance->size(); effect_num++)
	{
		if ((*vec_chance)[effect_num] == 0)
		{
			continue;
		}
		total_chance += (*vec_chance)[effect_num];
		if (effect_random_value <= total_chance)
		{
			break;
		}
	}
	return effect_num;
}
int32 item_set::item_succinct_random_property(const std::vector<float>* p_vec_att_up_change, const std::vector<int32>* p_vec_change_range, int32 offset_index, int32 cur_property_value)
{
	if (p_vec_att_up_change == nullptr || p_vec_change_range == nullptr)
		return 0;

	int32 property_value = 0;
	if (p_vec_att_up_change->size() > offset_index &&
		p_vec_change_range->size() > offset_index * item_succinct_change_max + item_succinct_change_ins_max)
	{
		if (cur_property_value == 0)
		{
			property_value = random_property_value(
				100,
				0,
				0,
				(*p_vec_change_range)[offset_index * item_succinct_change_max + item_succinct_change_ins_min],
				(*p_vec_change_range)[offset_index * item_succinct_change_max + item_succinct_change_ins_max]);

		}
		else
		{
			property_value = random_property_value(
				(*p_vec_att_up_change)[offset_index],
				(*p_vec_change_range)[offset_index * item_succinct_change_max + item_succinct_change_des_max],
				(*p_vec_change_range)[offset_index * item_succinct_change_max + item_succinct_change_des_min],
				(*p_vec_change_range)[offset_index * item_succinct_change_max + item_succinct_change_ins_min],
				(*p_vec_change_range)[offset_index * item_succinct_change_max + item_succinct_change_ins_max]);
		}

	}
	return  property_value;

}
int32 item_set::item_succinct_n_times(const guid_64& item_guid, int32 money_cost_type, int32 first_use_lock, int32 is_auto, int32 level, int32 is_use_jewel)
{


	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_error_code_template_param;
	}
	citem* temp_item = get_item(item_guid);
	if (temp_item == nullptr)
	{
		return e_error_code_template_param;
	}

	//int32 cur_level = temp_item->get_item_succinct_level();

	int32 is_suceess = item_succinct_one_times(item_guid, money_cost_type, first_use_lock, is_auto, level, is_use_jewel);

	return is_suceess;
}

int32 item_set::item_succinct_one_times(const guid_64& item_guid, int32 money_cost_type, int32 first_use_lock, int32 is_auto, int32 level, int32 is_use_jewel)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_error_code_template_param;
	}
	citem* temp_item = get_item(item_guid);
	if (temp_item == nullptr)
	{
		return e_error_code_template_param;
	}
	EquipTemplate* equip_template_ptr = temp_item->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return e_error_code_template_param;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_error_code_template_param;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	ItemSuccinctTemplate* temp_succinct_template = temp_item->get_succinc_info_ptr();
	if (temp_succinct_template == nullptr)
	{
		return e_error_code_template_param;
	}
	if (temp_succinct_template->GoldPropertyChangeRange.size() != temp_succinct_template->PropertyID.size() * item_succinct_change_max)
	{
		return e_error_code_template_param;
	}
	if (temp_succinct_template->DiamondPropertyChangeRange.size() != temp_succinct_template->PropertyID.size() * item_succinct_change_max)
	{
		return e_error_code_template_param;
	}
	if (money_cost_type != item_succinct_money_cost_gold && money_cost_type != item_succinct_money_cost_diamond)
	{
		return e_error_code_template_param;
	}
	int32 cost_type = (money_cost_type == item_succinct_money_cost_gold) ? e_money_type_silver_bind : e_money_type_jewel;
	int32 cost_num = (money_cost_type == item_succinct_money_cost_gold) ? temp_succinct_template->GoldCost : temp_succinct_template->DiamondCost;
	int32 material_id = 0;
	int32 material_num = 0;
	//int32 shop_id = 35030008;
	bool is_change = false;


	if (!temp_player.can_cut_money((e_money_type)cost_type, cost_num))
	{
		return e_error_code_item_money;
	}
	if (get_item_count(e_bag_type_bag, temp_succinct_template->MaterialID) < temp_succinct_template->MaterialCost)
	{
		if (!is_use_jewel)
			return e_error_code_item_matirial_not_enough;
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, temp_succinct_template->MaterialShopID);
			if (goods_ptr == nullptr)
				return e_error_code_template_param;

			material_id = goods_ptr->NewPrice[region_money_type_index];
			material_num = goods_ptr->NewPrice[region_money_num_index] * temp_succinct_template->MaterialCost;
			is_change = true;
		}
	}
	if (is_change)
	{
		if (cost_type == material_id && !temp_player.can_cut_money((e_money_type)cost_type, cost_num + material_num))
			return e_error_code_item_money;
		if (cost_type != material_id && !temp_player.can_cut_money((e_money_type)material_id, material_num))
			return e_error_code_item_money;
	}
	std::vector<int32> vec_property_max_limit;
	temp_item->get_property_max_limit(vec_property_max_limit);

	std::vector<int32> effect_block;
	for (int32 i = 0; i < vec_property_max_limit.size(); ++i)
	{
		if (temp_item->get_data_info(e_item_info(i + e_item_info_succinct_property1)) < vec_property_max_limit[i])
		{
			effect_block.push_back(i);
		}
	}
	if (effect_block.size() <= 0)
	{
		return e_error_code_item_full_level;
	}

	int effect_num = item_succinct_get_effect_num(item_guid, money_cost_type);

	if (effect_num == 0)
	{
		return e_error_code_template_param;
	}
	for (int32 i = 0; i < item_temp_succinct_property_max; i++)
	{
		temp_item->set_temp_succinct_att((item_temp_succinct_property)i, 0);
	}

	const std::vector<float>* p_vec_att_up_change = &temp_succinct_template->GoldSuccinctAttUpChance;
	const std::vector<int32>* p_vec_change_range = &temp_succinct_template->GoldPropertyChangeRange;
	if (money_cost_type != item_succinct_money_cost_gold)
	{
		p_vec_att_up_change = &temp_succinct_template->DiamondSuccinctAttUpChance;
		p_vec_change_range = &temp_succinct_template->DiamondPropertyChangeRange;
	}

	for (int32 i = 0; i < effect_num; i++)
	{
		if (effect_block.empty())
		{
			break;
		}
		int32 size_effect_block = effect_block.size();
		int32 effect_block_index = random_gen::get_random(0, size_effect_block - 1);
		int32 offset_index = effect_block[effect_block_index];

		int32 property_value = 0;
		int32 cur_property_value = temp_item->get_data_info(e_item_info(offset_index + e_item_info_succinct_property1));

		property_value = item_succinct_random_property(p_vec_att_up_change, p_vec_change_range, offset_index, cur_property_value);
		if (property_value + cur_property_value < 0)
		{
			property_value = -cur_property_value;
		}
		int32 max_value = vec_property_max_limit[offset_index];
		if ((property_value + cur_property_value) > max_value)
		{
			property_value = max_value - cur_property_value;
		}
		temp_item->set_temp_succinct_att((item_temp_succinct_property)offset_index, property_value);
		effect_block.erase(effect_block.begin() + effect_block_index);

	}
	if (first_use_lock)
	{
		temp_item->set_data_info(e_item_info_locked, 1);
	}
	temp_player.cut_money((e_money_type)cost_type, cost_num, e_server_log_cut_money_item_succinct, temp_item->get_data_info(e_item_info_info_id));
	if (is_change)
	{
		temp_player.cut_money((e_money_type)material_id, material_num, e_server_log_cut_money_item_succinct, temp_item->get_data_info(e_item_info_info_id));
	}
	else
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_succinct, 0, temp_succinct_template->MaterialID, temp_succinct_template->MaterialCost, first_use_lock);
	}
	if (item_succinct_is_fight_add(item_guid) == e_error_code_success)
	{
		item_save_succinct(item_guid);
		item_proto_item_operation_end item_msg;

		item_msg.set_operation_state(e_error_code_success);
		item_msg.set_operation_type(e_item_operation_save_succinct_result);
		item_msg.set_item_slot(0);

		item_msg.add_role_guid(temp_player.get_unit_guid().A);
		item_msg.add_role_guid(temp_player.get_unit_guid().B);

		item_msg.add_item_guid(item_guid.A);
		item_msg.add_item_guid(item_guid.B);
		temp_player.send_message_to_self(&item_msg, e_msgindex_s2c_item_operation);
	}
	else
	{
		item_proto_item_succinct_temp_value msg;
		msg.add_item_guid(item_guid.A);
		msg.add_item_guid(item_guid.B);
		msg.set_is_save_operate(false);
		msg.add_property_value(temp_item->get_temp_succinct_att(item_temp_succinct_property_1));
		msg.add_property_value(temp_item->get_temp_succinct_att(item_temp_succinct_property_2));
		msg.add_property_value(temp_item->get_temp_succinct_att(item_temp_succinct_property_3));
		msg.add_property_value(temp_item->get_temp_succinct_att(item_temp_succinct_property_4));
		temp_player.send_message(&msg, e_msgindex_s2c_item_succinct_temp_property_value);
	}

	int32 cur_succinct_num = 0;
	for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property6; ++i)
	{
		if (temp_item->get_data_info((e_item_info)i) > 0)
		{
			cur_succinct_num += temp_item->get_data_info((e_item_info)i);
		}
	}

	int32 Max_succinct_num = 0;
	for (int32 i = 0; i < vec_property_max_limit.size(); ++i)
	{
		if (vec_property_max_limit[i] > 0)
		{
			Max_succinct_num += vec_property_max_limit[i];
		}
	}

	float culturing_percent = 0;
	if (Max_succinct_num > 0)
	{
		culturing_percent = cur_succinct_num / Max_succinct_num;
	}
	//server_log::succinct_equip_log(temp_player.get_third_info(), temp_player.get_unit_info_inst(), equip_template_ptr->EquipSlot,
	//	temp_item->get_data_info(e_item_info_info_id), equip_template_ptr->quality_level, temp_item->get_item_info_ptr()->item_color, culturing_percent,
	//	cost_type, cost_num, m_bind_material_num, m_no_bind_material_num, temp_succinct_template->MaterialID, temp_succinct_template->MaterialCost);


	return e_error_code_success;
}


int32 item_set::item_save_succinct(const guid_64& item_guid)
{
	player& temp_unit = unit_man::get_player(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return e_error_code_template_param;
	}
	citem* temp_item = get_item(item_guid);
	if (temp_item == nullptr)
	{
		return e_error_code_template_param;
	}
	ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
	if (item_template_ptr == nullptr)
	{
		return e_error_code_template_param;
	}


	ItemSuccinctTemplate* temp_succinct_template = temp_item->get_succinc_info_ptr();
	if (temp_succinct_template == nullptr)
	{
		return e_error_code_template_param;
	}
	if (is_equipment_effective(temp_item->get_item_guid()))
	{
		set_equip_succinct_att(temp_succinct_template, *temp_item, false);
		refresh_succinct_buff(false);
	}
	vector<int32> vec_property_max_limit;
	temp_item->get_property_max_limit(vec_property_max_limit);
	int32 property_size = vec_property_max_limit.size() > item_temp_succinct_property_max ? item_temp_succinct_property_max : vec_property_max_limit.size();
	for (int32 i = 0; i < property_size; i++)
	{
		int32 temp_succinct_property_value = 0;
		temp_succinct_property_value = temp_item->get_temp_succinct_att((item_temp_succinct_property)i);

		int32 cur_succinct_property_value = 0;
		cur_succinct_property_value = temp_item->get_data_info(e_item_info(e_item_info_succinct_property1 + i));

		temp_succinct_property_value += cur_succinct_property_value;
		if (temp_succinct_property_value < 0)
		{
			temp_succinct_property_value = 0;
		}
		if (temp_succinct_property_value > vec_property_max_limit[i])
		{
			temp_succinct_property_value = vec_property_max_limit[i];
		}
		temp_item->set_temp_succinct_att((item_temp_succinct_property)i, 0);
		temp_item->set_data_info(e_item_info(e_item_info_succinct_property1 + i), temp_succinct_property_value);
	}

	send_item_one(temp_item);
	if (is_equipment_effective(temp_item->get_item_guid()))
	{
		set_equip_succinct_att(temp_succinct_template, *temp_item, true);
	}
	item_proto_item_succinct_temp_value msg;
	msg.add_item_guid(item_guid.A);
	msg.add_item_guid(item_guid.B);
	msg.set_is_save_operate(true);
	temp_unit.send_message(&msg, e_msgindex_s2c_item_succinct_temp_property_value);

	if (temp_item->is_succinct_full())
	{
		s_item_info item_data[chat_max_item];
		int32 item_num = 0;

		temp_unit.check_and_set_highest_record(e_role_history_highest_record_item_succinct_reach_mark_level, temp_item->get_item_grade(), item_template_ptr->item_color);

		int32 notice_id = temp_succinct_template->Noticeld;
		if (notice_id > 0)
		{
			s_chat_notice_info notice_data;
			notice_data.item_data.push_back(temp_item->get_item_inst());
			temp_unit.get_chat_mgr().send_notice_new(notice_id, notice_data);
			//std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
			//std::vector<std::string> notice_str_params_vec;
			//notice_str_params_vec.push_back(notice_str_id);
			//notice_str_params_vec.push_back(temp_unit.get_name());
			//notice_str_params_vec.push_back("%d"); 
			//std::string notice_str = init_unit::implode(notice_str_params_vec);
			//item_data[item_num] = temp_item->get_item_inst();
			//temp_unit.get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_num);
		}
	}
	temp_item->set_data_info(e_item_info_succinct_level, temp_item->get_item_succinct_level());
	if (is_equipment_effective(temp_item->get_item_guid()))
	{
		calcu_addition(e_addition_buff_succinct);
		refresh_succinct_buff(true);
	}
	temp_unit.get_ranking_mgr().set_and_sync_single_equip_ranking_data(temp_item);

	if (item_template_ptr->item_type == e_item_type_weapon)
	{
		temp_unit.check_and_set_highest_record(e_role_history_highest_record_weapon_full_succinct_grade);
	}
	if (item_template_ptr->item_type == e_item_type_armor)
	{
		temp_unit.check_and_set_highest_record(e_role_history_highest_record_equip_full_succinct_grade);
	}
	temp_unit.get_mission_mgr().target_check(e_mission_end_type_succinct_level_num);

	return e_error_code_success;
}
int32 item_set::rand_excellent_att_color(EquipTemplate* equip_template_ptr)
{
	if (equip_template_ptr == nullptr)
	{
		return e_item_color_max;
	}

	int32 color_rand_value = random_gen::get_random(1, 100);

	int32 color_rand_target_value = 0;
	int32 final_excellent_color = 0;
	for (int32 i = 0; i < equip_template_ptr->ForgeExcellentAttQualityWeight.size(); ++i)
	{
		color_rand_target_value += equip_template_ptr->ForgeExcellentAttQualityWeight[i];
		if (color_rand_value <= color_rand_target_value)
		{
			return i;
		}
	}
	return e_item_color_max;
}

int32 item_set::random_property_value(int32 up_chance, int32 max_des, int32 min_des, int32 min_ins, int32 max_ins)
{
	int32 result = 0;

	if (random_gen::get_random(0, 100) <= up_chance)
	{
		result = random_gen::get_random(min_ins, max_ins);
	}
	else
	{
		result = random_gen::get_random(max_des, min_des);
	}

	return result;
}
int32 item_set::rand_spirit_excellent_att_color(SpiritTemplate* spirit_template_ptr)
{
	if (spirit_template_ptr == nullptr)
	{
		return e_item_color_max;
	}
	int32 color_rand_value = random_gen::get_random(1, 100);
	int32 color_rand_target_value = 0;
	int32 final_excellent_color = 0;
	for (int32 i = 0; i < spirit_template_ptr->ForgeExcellentAttQualityWeight.size(); ++i)
	{
		color_rand_target_value += spirit_template_ptr->ForgeExcellentAttQualityWeight[i];
		if (color_rand_value <= color_rand_target_value)
		{
			return i;
		}
	}
	return e_item_color_max;
}
void item_set::refresh_succinct_buff(bool is_add)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}

	int32 weapon_buff_id = get_succinct_weapon_buff();
	int32 equip_buff_id = get_succinct_equip_buff();

	if (is_add)
	{
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, weapon_buff_id);
		buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, equip_buff_id);
	}
	else
	{
		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, weapon_buff_id);
		buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, equip_buff_id);
	}

}

citem* item_set::get_item_by_template_id(int32 target_template_id)
{
	for (auto it = m_item_map.begin(); it != m_item_map.end(); ++it)
	{
		int32 temp_template_id = it->second->get_data_info(e_item_info_info_id);
		if (target_template_id == temp_template_id)
		{
			return it->second;
		}
	}
	return nullptr;
}
citem* item_set::get_item_by_template_id(int32 item_template_id, e_bag_type bag_type)
{
	auto& guid_array = GET_BAG(bag_type);
	for (auto& it : guid_array)
	{
		citem* temp_item = get_item(it.second);
		if (temp_item != nullptr)
		{
			if (temp_item->get_data_info(e_item_info_info_id) == item_template_id)
			{
				return temp_item;
			}
		}
	}
	return nullptr;
}

citem* item_set::get_cur_level_wing()
{
	citem* cur_item_ptr = nullptr;
	//reset_wings_order();
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		citem* temp_item_ptr = get_item_by_slot(e_bag_type_wing, i);
		if (nullptr == temp_item_ptr)
		{
			break;
		}

		if (temp_item_ptr->get_data_info(e_item_info_activate) <= 0)
		{
			continue;
		}
		if (temp_item_ptr->get_wing_template_ptr())
		{
			if (temp_item_ptr->get_wing_template_ptr()->Type == faith::e_item_wing_type_normal)
			{
				cur_item_ptr = temp_item_ptr;
			}
		}
	}
	return cur_item_ptr;
}

citem* item_set::get_wing_add_spirit()
{
	citem* return_item = nullptr;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_wing)[i];
		if (temp_guid.is_valid())
		{
			citem* temp_item = get_item(temp_guid);
			if (temp_item != nullptr)
			{
				if (temp_item->get_item_info_ptr()->item_type == e_item_type_wing && temp_item->get_item_info_ptr()->sub_type == e_wing_sub_type_spirit)
				{
					return_item = temp_item;
					break;
				}
			}
		}
	}
	return return_item;
}
citem* item_set::get_wing_add_soul()
{
	citem* return_item = nullptr;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_wing)[i];
		if (temp_guid.is_valid())
		{
			citem* temp_item = get_item(temp_guid);
			if (temp_item != nullptr)
			{
				if (temp_item->get_item_info_ptr()->item_type == e_item_type_wing && temp_item->get_item_info_ptr()->sub_type == e_wing_sub_type_soul)
				{
					return_item = temp_item;
					break;
				}
			}
		}
	}
	return return_item;
}

std::vector<citem*> item_set::get_wing_feather()
{
	std::vector<citem*> return_item;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_wing)[i];
		if (temp_guid.is_valid())
		{
			citem* temp_item = get_item(temp_guid);
			if (temp_item != nullptr)
			{
				if (temp_item->get_item_info_ptr()->item_type == e_item_type_wing && temp_item->get_item_info_ptr()->sub_type == e_wing_sub_type_feather)
				{
					return_item.push_back(temp_item);

				}
			}
		}
	}
	return return_item;
}


int32 item_set::reset_wings_order()
{
	/*	citem* temp_item = nullptr;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
	if (item_in_wing[i].is_valid())
	{
	temp_item = get_item(item_in_wing[i]);
	if (temp_item != nullptr)
	{
	temp_item->set_data_info(e_item_info_slot,-1);
	}
	}
	}

	int32 wing_slot = 0;
	temp_item = nullptr;
	for (int32 index = 0 ; index < MAX_WING_ELEMENTS_NUM; index++)
	{
	citem* choosed_item = nullptr;
	int32 choosed_view_order = 99999;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM;i++)
	{
	if (!item_in_wing[i].is_valid())
	{
	continue;
	}
	temp_item = get_item(item_in_wing[i]);
	if (temp_item == nullptr || temp_item->get_data_info(e_item_info_slot) != -1)
	{
	continue;
	}
	if (choosed_item == nullptr)
	{
	choosed_item = temp_item;
	WingTemplate* temp_wing_template_ptr = choosed_item->get_wing_info_ptr();
	if (nullptr == temp_wing_template_ptr)
	{
	continue;
	}
	choosed_view_order = temp_wing_template_ptr->ViewOrder;
	continue;
	}
	else
	{
	WingTemplate* temp_wing_template_ptr = temp_item->get_wing_info_ptr();
	if (nullptr == temp_wing_template_ptr)
	{
	continue;
	}
	int32 temp_view_order = temp_wing_template_ptr->ViewOrder;
	if (temp_view_order < choosed_view_order)
	{
	choosed_item = temp_item;
	choosed_view_order = temp_view_order;
	}
	}
	}
	if (choosed_item == nullptr)
	{

	}
	else
	{
	choosed_item->set_data_info(e_item_info_slot, wing_slot);
	wing_slot++;
	}
	}
	*/
	//citem* wing_add_spirit = get_wing_add_spirit();
	//citem* wing_add_soul = get_wing_add_soul();
	//std::vector<citem*> wing_feathers = get_wing_feather();

	//if ( wing_add_spirit!=nullptr && wing_slot < MAX_WING_ELEMENTS_NUM )
	//{
	//	wing_add_spirit->set_data_info(e_item_info_slot, wing_slot);
	//	wing_slot++;
	//}

	//if (wing_add_soul != nullptr && wing_slot < MAX_WING_ELEMENTS_NUM)
	//{
	//	wing_add_soul->set_data_info(e_item_info_slot, wing_slot);
	//	wing_slot++;
	//}

	//for (int32 i = 0 ; i < wing_feathers.size() ; i++)
	//{
	//	if (wing_feathers[i] != nullptr && wing_slot < MAX_WING_ELEMENTS_NUM)
	//	{
	//		wing_feathers[i]->set_data_info(e_item_info_slot, wing_slot);
	//		wing_slot++;
	//	}
	//}

	return 0;
}

int32 item_set::calculate_total_wing_upgrade_num()
{
	int32 upgrade_count = 0;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		citem* temp_wing = get_item_by_slot(e_bag_type_wing, i);
		if (temp_wing != nullptr && temp_wing->get_item_info_ptr()->item_type == e_item_type_wing && temp_wing->get_item_info_ptr()->sub_type == e_wing_sub_type_wing)
		{
			if (temp_wing->get_data_info(e_item_info_activate) > 0)
			{
				upgrade_count += temp_wing->get_data_info(e_item_info_upgrade_count);
			}
		}
	}
	return upgrade_count;
}

int32 item_set::get_cur_wing_level()
{
	int32 wing_level = 0;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		citem* temp_wing = get_item_by_slot(e_bag_type_wing, i);
		if (temp_wing != nullptr && temp_wing->get_item_info_ptr()->item_type == e_item_type_wing
			&& temp_wing->get_item_info_ptr()->sub_type == e_wing_sub_type_wing)
		{
			WingTemplate* temp_wing_template = GET_TEMPLATE(WingTemplate, temp_wing->get_item_info_ptr()->logic_id);
			if (temp_wing_template != nullptr && temp_wing_template->Type == e_item_wing_type_normal)
			{
				if (temp_wing->get_data_info(e_item_info_activate) > 0)
				{
					wing_level++;
				}
			}
		}
	}
	return wing_level;
}

std::vector<citem*> item_set::get_all_shape_wing()
{
	std::vector<citem*> shape_wing_block;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		if (GET_BAG(e_bag_type_wing)[i].is_valid())
		{
			citem* temp_wing_ptr = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_wing_ptr != nullptr)
			{
				WingTemplate* temp_wing_template = GET_TEMPLATE(WingTemplate, temp_wing_ptr->get_item_info_ptr()->logic_id);
				if (temp_wing_template != nullptr && temp_wing_template->Type != e_item_wing_type_normal)
				{
					shape_wing_block.push_back(temp_wing_ptr);
				}
			}
		}
	}
	return shape_wing_block;
}

std::vector<citem*> item_set::get_all_wing()
{
	std::vector<citem*> return_item;
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_wing)[i];
		if (temp_guid.is_valid())
		{
			citem* temp_item = get_item(temp_guid);
			if (temp_item != nullptr)
			{
				if (temp_item->get_item_info_ptr()->item_type == e_item_type_wing && temp_item->get_item_info_ptr()->sub_type == e_wing_sub_type_wing)
				{
					return_item.push_back(temp_item);

				}
			}
		}
	}
	return return_item;
}

void item_set::cost_star_map_spirit_item_states(e_server_log_del_item causeid, int32 location, int32 item_template_id, int32 item_cost_num)
{
	int32 temp_item_count = item_cost_num;
	for (int32 i = 0; i < MAX_ITEM_BAG_NUM; ++i)
	{
		if (temp_item_count <= 0)
		{
			break;
		}
		citem* item_ptr = get_item(GET_BAG(e_bag_type_bag)[i]);
		if (item_ptr && item_ptr->get_data_info(e_item_info_info_id) == item_template_id && item_ptr->get_data_info(e_item_info_upgrade_count) <= 1 && item_ptr->get_data_info(e_item_info_stargrade_count) == 0)
		{
			int32 item_num = item_ptr->get_data_info(e_item_info_stack_count);
			if (item_num > temp_item_count)
			{
				del_item(causeid, location, item_ptr, temp_item_count);
				temp_item_count = 0;
				break;
			}
			else
			{
				temp_item_count -= item_num;
				del_item(causeid, location, item_ptr, item_num);
			}
		}
	}
}

int32 item_set::count_star_map_spirit(int32 item_template_id)
{
	if (item_template_id <= 0)
	{
		return 0;
	}

	auto& guid_array = GET_BAG(e_bag_type_bag);
	int32 template_item_sum = 0;

	for (auto& it : guid_array)
	{
		citem* item_ptr = get_item(it.second);
		if (nullptr == item_ptr)
		{
			continue;
		}
		if (item_ptr->get_data_info(e_item_info_info_id) == item_template_id && item_ptr->get_data_info(e_item_info_upgrade_count) <= 1 && item_ptr->get_data_info(e_item_info_stargrade_count) == 0)
		{
			template_item_sum += item_ptr->get_data_info(e_item_info_stack_count);
		}
	}
	return template_item_sum;
}

int32 item_set::show_this_wing(citem* wing_ptr, int32 item_slot, bool is_mission_wing, bool is_need_send_notice)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("wing", "show_this_wing", 1, false, "%d%s%d%b%b>%d", m_unit_array_index, wing_ptr->get_item_guid().to_string().c_str(), item_slot, is_mission_wing, is_need_send_notice, &result);
	return result;
	/*
	if (wing_ptr == nullptr)
	{
		return e_error_code_item_no_item;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return e_error_code_template_param;
	}
	if (wing_ptr->get_data_info(e_item_info_activate) <= 0)
	{
		//return e_error_code_item_chant_show_unactivate_wing;
		return e_item_string_chant_show_unactivate_wing;
	}


	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, wing_ptr->get_item_info_ptr()->logic_id);
	if (wing_template_ptr == nullptr)
	{
		return e_error_code_template_param;
	}

	int32 wing_template_id = 0;
	if (item_slot < 0)
	{
		wing_template_id = wing_ptr->get_data_info(e_item_info_info_id);
		wing_ptr->set_data_info(e_item_info_showing_illusion_idex, item_slot);
	}
	else
	{
		if (item_slot >= wing_template_ptr->IIllusionList.size())
		{
			return e_error_code_template_param;
		}

		citem* wing_illusion_ptr = get_item_by_template_id(wing_template_ptr->IIllusionList[item_slot]);
		if (wing_illusion_ptr == nullptr)
		{
			return e_item_string_no_item;
		}
		if (wing_illusion_ptr->get_data_info(e_item_info_activate) <= 0)
		{
			return e_item_string_chant_show_unactivate_wing;
		}

		wing_template_id = wing_illusion_ptr->get_data_info(e_item_info_info_id);
		wing_ptr->set_data_info(e_item_info_showing_illusion_idex, item_slot);
	}

	int32 cur_showing_wing = temp_unit.get_unit_info(e_role_info_wing_showd_template_id);
	if (cur_showing_wing == wing_template_id)
	{
		wing_template_id = 0;
	}
	temp_unit.set_unit_info(e_role_info_wing_showd_template_id, wing_template_id);
	temp_unit.send_info_one(e_role_info_wing_showd_template_id);
	temp_unit.get_item_set().set_showed_wing();
	if (wing_template_id > 0)
	{
		if (temp_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_walk)
		{
			if (true == is_mission_wing)
			{
				temp_unit.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_template_ptr->MaxWalkSpeed, send_gs_to_client, true);
			}
			temp_unit.get_pawn_att().set_game_att(e_unit_game_att_movement, e_move_ment_wing_walk, true);
		}
		send_item_one(wing_ptr);
	}
	else
	{
		player& temp_player_ref = unit_man::get_player(m_unit_array_index);
		if (true == temp_player_ref.is_valid() && true == is_need_send_notice)
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90096106);
			temp_player_ref.send_notice(notice_str);
		}
		if (temp_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_wing_walk)
		{
			if (true == is_mission_wing)
			{
				temp_unit.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_template_ptr->MaxWalkSpeed, send_gs_to_client, false);
			}
			temp_unit.get_pawn_att().set_game_att(e_unit_game_att_movement, e_move_ment_walk, true);
		}
		else if (temp_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_wing_fly)
		{
			temp_unit.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_template_ptr->MaxFlySpeed, send_gs_to_client, false);
			citem* mount_ptr = get_showed_mount();
			if (nullptr != mount_ptr)
			{
				MountTemplate* mount_temp_ptr = mount_ptr->get_mount_template_ptr();
				if (nullptr != mount_temp_ptr)
				{
					use_skill_param param;
					param.sender_unit_idf = temp_unit.get_identifier();
					param.target_unit_idf = temp_unit.get_identifier();
					param.skill_template_id = skill_wing_fly_to_mount_fly_id;
					param.pos = temp_unit.get_new_map_pos().unit_location;
					param.yaw = temp_unit.get_new_map_pos().unit_location.y;
					param.skill_order = 1;
					skill_manager::skill_cast(param);
				}
			}

			else
			{
				temp_unit.get_pawn_att().set_game_att(e_unit_game_att_movement, e_move_ment_walk, true);
			}
		}
	}


	item_proto_show_this_wing msg;
	guid_64 role_guid = temp_unit.get_unit_guid();

	msg.add_role_guid(role_guid.A);
	msg.add_role_guid(role_guid.B);

	msg.set_wing_template_id(wing_template_id);

	temp_unit.send_message_to_aoi(&msg, e_msgindex_s2c_show_this_whig);
	return e_error_code_success;
	*/

}

void item_set::add_and_del_tidy_type(e_bag_type tidy_bag_typ, bool is_add)
{
	bool is_have = false;

	for (auto iter = m_tidy_type_list.begin(); iter != m_tidy_type_list.end(); iter++)
	{
		if (*iter == (int32)tidy_bag_typ)
		{
			if (false == is_add)
			{
				m_tidy_type_list.erase(iter);
				break;
			}
			is_have = true;
			break;
		}
	}

	if (false == is_have && is_add)
	{
		m_tidy_type_list.push_back((int32)tidy_bag_typ);
	}
}

int32 item_set::get_use_time_item_empty_slot()
{
	int32 slot_idex = -1;
	for (int32 i = 0; i < MAX_TIME_LIMIT_ITEM_NUM; ++i)
	{
		if (!m_used_has_time_item_array[i].is_valid())
		{
			slot_idex = i;
			break;
		}
	}

	return slot_idex;
}

bool item_set::set_mount_or_wing_beast_spirit_bag_data(int32 item_id, int32 item_type, int32 upgrade_num)
{
	if (e_beast_spirit_type_mount == item_type)
	{
		for (int32 i = 0; i < MAX_BEAST_SPIRIT_NUM; i++)
		{
			if (GET_BAG(e_bag_type_mount_beast_spirit)[i].is_valid())
			{
				citem* item_mount_beast_spirit = get_item(GET_BAG(e_bag_type_mount_beast_spirit)[i]);
				if (nullptr != item_mount_beast_spirit && item_mount_beast_spirit->get_data_info(e_item_info_info_id) == item_id)
				{
					BeastSpiritTemplate* beast_spirit_template = item_mount_beast_spirit->get_beast_spirit_template_ptr();
					if (nullptr == beast_spirit_template || beast_spirit_template->UseNumArray.size() <= 0)
					{
						return false;
					}

					upgrade_num += item_mount_beast_spirit->get_data_info(e_item_info_upgrade_count);
					if (beast_spirit_template->UseNumArray.size() == 1)
					{
						if (upgrade_num <= beast_spirit_template->UseNumArray[0])
						{
							item_mount_beast_spirit->set_data_info(e_item_info_upgrade_count, upgrade_num);
							send_item_one(item_mount_beast_spirit);
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						player& temp_player_ref = unit_man::get_player(m_unit_array_index);
						if (!temp_player_ref.is_valid())
						{
							return false;
						}

						int32 player_class_type = temp_player_ref.get_unit_info(e_role_info_class_type) - 1;
						if (upgrade_num <= beast_spirit_template->UseNumArray[player_class_type])
						{
							item_mount_beast_spirit->set_data_info(e_item_info_upgrade_count, upgrade_num);
							send_item_one(item_mount_beast_spirit);
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
			else
			{
				int32 empty_slot = get_empty_slot(e_bag_type_mount_beast_spirit);
				if (empty_slot < 0)
				{
					return false;
				}
				s_item_info temp_item_info;
				temp_item_info.item_guid = guid_gen::make_guid();
				temp_item_info.data_ary[e_item_info_info_id] = item_id;
				temp_item_info.data_ary[e_item_info_stack_count] = 1;
				temp_item_info.data_ary[e_item_info_container_type] = e_bag_type_mount_beast_spirit;
				temp_item_info.data_ary[e_item_info_upgrade_count] = upgrade_num;
				temp_item_info.data_ary[e_item_info_slot] = empty_slot;
				citem* item_mount_beast_spirit = create_item_by_info(temp_item_info, e_server_log_add_item_spirit_lock, 0, true);
				if (nullptr != item_mount_beast_spirit)
				{
					send_item_one(item_mount_beast_spirit);
					return true;
				}
			}
		}
	}
	else if (e_beast_spirit_type_wing == item_type)
	{
		for (int32 i = 0; i < MAX_BEAST_SPIRIT_NUM; i++)
		{
			if (GET_BAG(e_bag_type_wing_beast_spirit)[i].is_valid())
			{
				citem* item_wing_beast_spirit = get_item(GET_BAG(e_bag_type_wing_beast_spirit)[i]);
				if (nullptr != item_wing_beast_spirit && item_wing_beast_spirit->get_data_info(e_item_info_info_id) == item_id)
				{
					BeastSpiritTemplate* beast_spirit_template = item_wing_beast_spirit->get_beast_spirit_template_ptr();
					if (nullptr == beast_spirit_template || beast_spirit_template->UseNumArray.size() <= 0)
					{
						return false;
					}

					upgrade_num += item_wing_beast_spirit->get_data_info(e_item_info_upgrade_count);
					if (beast_spirit_template->UseNumArray.size() == 1)
					{
						if (upgrade_num <= beast_spirit_template->UseNumArray[0])
						{
							item_wing_beast_spirit->set_data_info(e_item_info_upgrade_count, upgrade_num);
							send_item_one(item_wing_beast_spirit);
							return true;
						}
						else
						{
							return false;
						}

					}
					else
					{
						player& temp_player_ref = unit_man::get_player(m_unit_array_index);
						if (!temp_player_ref.is_valid())
						{
							return false;
						}

						int32 player_class_type = temp_player_ref.get_unit_info(e_role_info_class_type) - 1;
						if (upgrade_num <= beast_spirit_template->UseNumArray[player_class_type])
						{
							item_wing_beast_spirit->set_data_info(e_item_info_upgrade_count, upgrade_num);
							send_item_one(item_wing_beast_spirit);
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
			else
			{
				int32 empty_slot = get_empty_slot(e_bag_type_wing_beast_spirit);
				if (empty_slot < 0)
				{
					return false;
				}
				s_item_info temp_item_info;
				temp_item_info.item_guid = guid_gen::make_guid();
				temp_item_info.data_ary[e_item_info_info_id] = item_id;
				temp_item_info.data_ary[e_item_info_stack_count] = 1;
				temp_item_info.data_ary[e_item_info_container_type] = e_bag_type_wing_beast_spirit;
				temp_item_info.data_ary[e_item_info_upgrade_count] = upgrade_num;
				temp_item_info.data_ary[e_item_info_slot] = empty_slot;

				citem* item_wing_beast_spirit = create_item_by_info(temp_item_info, e_server_log_add_item_spirit_lock, 0, true);
				if (nullptr != item_wing_beast_spirit)
				{
					send_item_one(item_wing_beast_spirit);
					return true;
				}
			}
		}
	}
	else if (e_beast_spirit_type_spirit == item_type)
	{
		for (int32 i = 0; i < MAX_BEAST_SPIRIT_NUM; i++)
		{
			if (GET_BAG(e_bag_type_spirit_beast_spirit)[i].is_valid())
			{
				citem* item_spirit_beast_spirit = get_item(GET_BAG(e_bag_type_spirit_beast_spirit)[i]);
				if (nullptr != item_spirit_beast_spirit && item_spirit_beast_spirit->get_data_info(e_item_info_info_id) == item_id)
				{
					BeastSpiritTemplate* beast_spirit_template = item_spirit_beast_spirit->get_beast_spirit_template_ptr();
					if (nullptr == beast_spirit_template || beast_spirit_template->UseNumArray.size() <= 0)
					{
						return false;
					}

					upgrade_num += item_spirit_beast_spirit->get_data_info(e_item_info_upgrade_count);
					if (beast_spirit_template->UseNumArray.size() == 1)
					{
						if (upgrade_num <= beast_spirit_template->UseNumArray[0])
						{
							item_spirit_beast_spirit->set_data_info(e_item_info_upgrade_count, upgrade_num);
							send_item_one(item_spirit_beast_spirit);
							return true;
						}
						else
						{
							return false;
						}

					}
					else
					{
						player& temp_player_ref = unit_man::get_player(m_unit_array_index);
						if (!temp_player_ref.is_valid())
						{
							return false;
						}

						int32 player_class_type = temp_player_ref.get_unit_info(e_role_info_class_type) - 1;
						if (upgrade_num <= beast_spirit_template->UseNumArray[player_class_type])
						{
							item_spirit_beast_spirit->set_data_info(e_item_info_upgrade_count, upgrade_num);
							send_item_one(item_spirit_beast_spirit);
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
			else
			{
				int32 empty_slot = get_empty_slot(e_bag_type_spirit_beast_spirit);
				if (empty_slot < 0)
				{
					return false;
				}
				s_item_info temp_item_info;
				temp_item_info.item_guid = guid_gen::make_guid();
				temp_item_info.data_ary[e_item_info_info_id] = item_id;
				temp_item_info.data_ary[e_item_info_stack_count] = 1;
				temp_item_info.data_ary[e_item_info_container_type] = e_bag_type_spirit_beast_spirit;
				temp_item_info.data_ary[e_item_info_upgrade_count] = upgrade_num;
				temp_item_info.data_ary[e_item_info_slot] = empty_slot;

				citem* item_wing_beast_spirit = create_item_by_info(temp_item_info, e_server_log_add_item_spirit_lock, 0, true);
				if (nullptr != item_wing_beast_spirit)
				{
					send_item_one(item_wing_beast_spirit);
					return true;
				}
			}
		}
	}
	return false;
}

void item_set::change_beast_spirit_att(int32 beast_spirit_type, bool is_add_att)
{
	if (beast_spirit_type == e_beast_spirit_type_mount)
	{
		for (int32 i = 0; i < MAX_BEAST_SPIRIT_NUM; i++)
		{
			if (GET_BAG(e_bag_type_mount_beast_spirit)[i].is_valid())
			{
				citem* item_mount_beast_spirit = get_item(GET_BAG(e_bag_type_mount_beast_spirit)[i]);
				if (nullptr != item_mount_beast_spirit)
				{
					BeastSpiritTemplate* beast_spirit_template_ptr = item_mount_beast_spirit->get_beast_spirit_template_ptr();
					if (nullptr != beast_spirit_template_ptr)
					{
						item_change_att(beast_spirit_template_ptr->GetAttributes, item_mount_beast_spirit->get_data_info(e_item_info_upgrade_count), is_add_att);
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (beast_spirit_type == e_beast_spirit_type_wing)
	{
		for (int32 i = 0; i < MAX_BEAST_SPIRIT_NUM; i++)
		{
			if (GET_BAG(e_bag_type_wing_beast_spirit)[i].is_valid())
			{
				citem* item_wing_beast_spirit = get_item(GET_BAG(e_bag_type_wing_beast_spirit)[i]);
				if (nullptr != item_wing_beast_spirit)
				{
					BeastSpiritTemplate* beast_spirit_template_ptr = item_wing_beast_spirit->get_beast_spirit_template_ptr();
					if (nullptr != beast_spirit_template_ptr)
					{
						item_change_att(beast_spirit_template_ptr->GetAttributes, item_wing_beast_spirit->get_data_info(e_item_info_upgrade_count), is_add_att);
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	else if (beast_spirit_type == e_beast_spirit_type_spirit)
	{
		for (int32 i = 0; i < MAX_BEAST_SPIRIT_NUM; i++)
		{
			if (GET_BAG(e_bag_type_spirit_beast_spirit)[i].is_valid())
			{
				citem* item_spirit_beast_spirit = get_item(GET_BAG(e_bag_type_spirit_beast_spirit)[i]);
				if (nullptr != item_spirit_beast_spirit)
				{
					BeastSpiritTemplate* beast_spirit_template_ptr = item_spirit_beast_spirit->get_beast_spirit_template_ptr();
					if (nullptr != beast_spirit_template_ptr)
					{
						item_change_att(beast_spirit_template_ptr->GetAttributes, item_spirit_beast_spirit->get_data_info(e_item_info_upgrade_count), is_add_att);
					}
				}
			}
			else
			{
				break;
			}
		}
	}
}

void item_set::set_mount_att(bool is_add)
{
	if (m_mount_att_change == is_add)
	{
		return;
	}
	player& temp_player = unit_man::get_player(m_unit_array_index);
	m_mount_att_change = is_add;
	citem* temp_mount_item = nullptr;
	MountTemplate* temp_mount_template_ptr = nullptr;
	MountTemplate* temp_mount_star_template_ptr = nullptr;
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		if (GET_BAG(e_bag_type_mount)[i].is_valid())
		{
			temp_mount_item = get_item(GET_BAG(e_bag_type_mount)[i]);
			if (temp_mount_item != nullptr)
			{
				if (temp_mount_item->get_data_info(e_item_info_activate) > 0)
				{
					temp_mount_template_ptr = temp_mount_item->get_mount_template_ptr();
					int32 mount_real_id = temp_mount_item->get_item_logic_id() + temp_mount_item->get_data_info(e_item_info_stargrade_count) + 1000;
					temp_mount_star_template_ptr = GET_TEMPLATE(MountTemplate, mount_real_id);
					if (temp_mount_template_ptr)
					{
						item_change_att(temp_mount_template_ptr->AttArray, 1, is_add);
					}
					if (temp_mount_item->get_item_info_ptr())
					{
						if (temp_mount_item->get_item_info_ptr()->sub_type == 0)
						{
							if (temp_mount_star_template_ptr)
							{
								item_change_att(temp_mount_star_template_ptr->AttArray, 1, is_add);
							}
						}
					}

				}
				if (temp_player.is_valid() == false)
				{
					continue;
				}
				temp_player.get_mount_power_mgr().set_item_attr_all(temp_mount_item, is_add);
			}
		}
	}

}

int32 item_set::mount_upgrade(const guid_64& mount_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("mount", "mount_upgrade", 1, false, "%d%s%d>%d", m_unit_array_index, mount_guid.to_string().c_str(), use_up_item, &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;
	int32 matirial_count = 0;
	if (!mount_guid.is_valid())
	{
		return e_item_string_unkown;
	}

	citem* mount_ptr = get_item(mount_guid);
	if (mount_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	if (mount_ptr->get_data_info(e_item_info_activate) <= 0)
	{
		return e_item_string_mount_unlock;
	}
	ItemTemplate* temp_item_template_ptr = mount_ptr->get_item_info_ptr();
	if (temp_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	MountTemplate* temp_mount_template_ptr = GET_TEMPLATE(MountTemplate, temp_item_template_ptr->logic_id);
	if (temp_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	int32 mount_real_id = temp_item_template_ptr->logic_id + mount_ptr->get_data_info(e_item_info_upgrade_count);
	MountTemplate* real_mount_template_ptr = GET_TEMPLATE(MountTemplate, mount_real_id);

	if (real_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	if (real_mount_template_ptr->Type == mount_type_warlord || real_mount_template_ptr->Type == mount_type_lordlegionmem)
	{
		return e_item_string_auto_use_money_not_enough;
	}

	if ((real_mount_template_ptr->ClassNow >= real_mount_template_ptr->MountClassMax) && (real_mount_template_ptr->GradeNow >= real_mount_template_ptr->MountGradeMax))
	{
		return e_item_string_full_level;
	}

	if (real_mount_template_ptr->GradeLimit > temp_player.get_grade_num())
	{
		return e_item_string_unkown;
	}

	int32 temp_cur_value = temp_player.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_mount);
	temp_player.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1);

	bool is_big_upgrade = false;
	if (real_mount_template_ptr->GradeNow != real_mount_template_ptr->MountGradeMax)
	{
		matirial_count = get_item_count(real_mount_template_ptr->UpgradeMatirialId, e_bag_type_bag);
		if (matirial_count < real_mount_template_ptr->UpgradeMatirialNum)
		{
			if (use_up_item == 0)
			{
				return e_item_string_matirial_not_enough;
			}
			else
			{
				use_money_instead_matirial = true;
			}

		}
		//if ((real_mount_template_ptr->UpgradeMoney.size() > 1)
		//	&& (temp_player.can_cut_money((e_money_type)real_mount_template_ptr->UpgradeMoney[0], real_mount_template_ptr->UpgradeMoney[1]) == false))
		//{
		//	return e_item_string_money;
		//}

		if (use_money_instead_matirial)
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, real_mount_template_ptr->UpgradeMatirialShopId);
			if (goods_ptr != nullptr
				&& goods_ptr->NewPrice.size() > region_money_num_index)
			{
				int32 money_id = goods_ptr->NewPrice[region_money_type_index];
				int32 money_num = goods_ptr->NewPrice[region_money_num_index] * real_mount_template_ptr->UpgradeMatirialNum;
				if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
				{
					return e_item_string_auto_use_money_not_enough;
				}
				temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_mount_upgrade_auto_buy, real_mount_template_ptr->UpgradeMatirialNum);
			}
			else
			{
				return e_item_string_no_item;
			}
		}
		else
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_mount_upgrade , 0, real_mount_template_ptr->UpgradeMatirialId, real_mount_template_ptr->UpgradeMatirialNum, e_item_locked);
		}

		//set_mount_att(false);
		//if (real_mount_template_ptr->UpgradeMoney.size() > 1)
		//{
		//	temp_player.cut_money((e_money_type)real_mount_template_ptr->UpgradeMoney[0], real_mount_template_ptr->UpgradeMoney[1]);
		//}
		if (mount_ptr->get_mount_template_ptr())
		{
			item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, false);
			int32 mount_upgrade_now = mount_ptr->get_data_info(e_item_info_upgrade_count);
			mount_ptr->set_data_info(e_item_info_upgrade_count, mount_upgrade_now + 1);
			if (mount_ptr->get_mount_template_ptr())
			{
				item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, true);
				if (mount_ptr->get_mount_template_ptr()->StarBuffId > 0)
				{
					PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, mount_ptr->get_mount_template_ptr()->StarBuffId);
					if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
					{
						star_skill_unlock(mount_guid, mount_ptr->get_data_info(e_item_info_upgrade_count), false, false);
					}
				}
			}
			else
			{
				mount_ptr->set_data_info(e_item_info_upgrade_count, mount_upgrade_now);
				item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, true);
			}
		}
		send_item_one(mount_ptr);
		//set_mount_att(true);

	}
	else
	{
		is_big_upgrade = true;
		matirial_count = get_item_count(real_mount_template_ptr->LiftingClassMatirialId, e_bag_type_bag);
		if (matirial_count < real_mount_template_ptr->LiftingClassMatirialNum)
		{
			if (use_up_item == 0)
			{
				return e_item_string_matirial_not_enough;
			}
			else
			{
				use_money_instead_matirial = true;
			}

		}
		//if (temp_player.can_cut_money((e_money_type)real_mount_template_ptr->UpgradeMoney[0], real_mount_template_ptr->UpgradeMoney[1]) == false)
		//{
		//	return e_item_string_money;
		//}

		if (use_money_instead_matirial)
		{
			GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, real_mount_template_ptr->LiftingClassShopId);
			if (goods_ptr != nullptr
				&& goods_ptr->NewPrice.size() > region_money_num_index)
			{
				int32 money_id = goods_ptr->NewPrice[region_money_type_index];
				int32 money_num = goods_ptr->NewPrice[region_money_num_index] * real_mount_template_ptr->LiftingClassMatirialNum;
				if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
				{
					return e_item_string_auto_use_money_not_enough;
				}
				temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_mount_upgrade, mount_real_id);
			}
			else
			{
				return e_item_string_no_item;
			}
		}
		else
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_mount_upgrade , 0, real_mount_template_ptr->LiftingClassMatirialId, real_mount_template_ptr->LiftingClassMatirialNum, e_item_locked);
		}
		if (mount_ptr->get_mount_template_ptr())
		{
			item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, false);
			int32 mount_upgrade_now = mount_ptr->get_data_info(e_item_info_upgrade_count);
			mount_ptr->set_data_info(e_item_info_upgrade_count, mount_upgrade_now + 1);
			if (mount_ptr->get_mount_template_ptr())
			{
				item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, true);
				if (mount_ptr->get_mount_template_ptr()->StarBuffId > 0)
				{
					PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, mount_ptr->get_mount_template_ptr()->StarBuffId);
					if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
					{
						star_skill_unlock(mount_guid, mount_ptr->get_data_info(e_item_info_upgrade_count), false, false);
					}
				}
			}
			else
			{
				mount_ptr->set_data_info(e_item_info_upgrade_count, mount_upgrade_now);
				item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, true);
			}
		}
		send_item_one(mount_ptr);
	}

	item_proto_show_item_upgrade_effect effect_msg;
	MountTemplate* final_template_ptr = mount_ptr->get_mount_template_ptr();
	if (final_template_ptr != nullptr)
	{
		if (is_big_upgrade)
		{
			effect_msg.set_show_type(e_item_upgrade_effect_type_mount_upgrade_class);
			effect_msg.set_show_value(final_template_ptr->ClassNow);
		}
		else
		{
			effect_msg.set_show_type(e_item_upgrade_effect_type_mount_upgrade);
			effect_msg.set_show_value(final_template_ptr->GradeNow);
		}
		temp_player.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);
	}


	//sync_ranking_mount();

	if (is_big_upgrade)
	{
		int32 current_mount_id = temp_item_template_ptr->logic_id + mount_ptr->get_data_info(e_item_info_upgrade_count);
		MountTemplate* current_mount_template_ptr = GET_TEMPLATE(MountTemplate, current_mount_id);
		if (current_mount_template_ptr && current_mount_template_ptr->NoticeId > 0)
		{
			s_chat_notice_info notice_data;
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(current_mount_template_ptr->MountNameId);
			notice_data.item_name.push_back(temp_string);
			notice_data.num_type_data.push_back(current_mount_template_ptr->ClassNow);
			temp_player.get_chat_mgr().send_notice_new(current_mount_template_ptr->NoticeId, notice_data);
		}

		temp_player.get_achievement_mgr().player_changed(eat_mount_grade_count);
		temp_player.get_mission_mgr().target_check(e_mission_end_type_mount_total_grade);
	}
	temp_player.refresh_service_goal(e_service_goal_type_mount_level);
	return e_error_code_success;
	*/
}

int32 item_set::fashion_upgrade(const guid_64& fashion_guid, int32 use_up_item, int32 use_num)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;
	int32 matirial_count = 0;
	if (!fashion_guid.is_valid())
	{
		return e_item_string_unkown;
	}

	citem* fashion_ptr = get_item(fashion_guid);
	if (fashion_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	ItemTemplate* temp_item_template_ptr = fashion_ptr->get_item_info_ptr();
	if (temp_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	FasionTemplate* temp_fashion_template_ptr = GET_TEMPLATE(FasionTemplate, temp_item_template_ptr->logic_id);

	if (temp_fashion_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	if (temp_player.get_owner_unit_level() < GAMECONFIG->FashionNextUpgradeLevel)
	{
		return e_item_string_level;
	}

	int32 fashion_real_id = temp_item_template_ptr->logic_id + fashion_ptr->get_data_info(e_item_info_upgrade_count);
	FasionTemplate* real_fashion_template_ptr = GET_TEMPLATE(FasionTemplate, fashion_real_id);

	if (real_fashion_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	if (real_fashion_template_ptr->GradeLimit > temp_player.get_grade_num())
	{
		return e_item_string_unkown;
	}

	bool is_big_upgrade = false;
	if (real_fashion_template_ptr->UpgradeMaterial.size() != 0 && real_fashion_template_ptr->UpgradeMaterial[0] > 0)
	{
		int32 item_id = real_fashion_template_ptr->UpgradeMaterial[0];
		int32 need_num = real_fashion_template_ptr->UpgradeMaterial[1];
		int32 have_num = get_item_count(e_bag_type_bag, real_fashion_template_ptr->UpgradeMaterial[0]);
		if (have_num < use_num)
		{
			return e_item_string_matirial_not_enough;
		}
		cost_item_by_id_with_lock_states(e_server_log_del_item_mount_upgrade, 0, item_id, use_num, e_item_locked);
		int32 new_exp = fashion_ptr->get_data_info(e_item_info_wing_exp) + use_num;
		fashion_ptr->set_data_info(e_item_info_wing_exp, new_exp);
		if (new_exp >= need_num)
		{
			fashion_ptr->set_data_info(e_item_info_wing_exp, new_exp - need_num);
			if (fashion_ptr->get_fashion_template_ptr())
			{
				item_change_att(fashion_ptr->get_fashion_template_ptr()->AttArray, 1, false);
				int32 fashion_upgrade_now = fashion_ptr->get_data_info(e_item_info_upgrade_count);
				fashion_ptr->set_data_info(e_item_info_upgrade_count, fashion_upgrade_now + 1);
				if (fashion_ptr->get_fashion_template_ptr())
				{
					item_change_att(fashion_ptr->get_fashion_template_ptr()->AttArray, 1, true);
				}
				else
				{
					fashion_ptr->set_data_info(e_item_info_upgrade_count, fashion_upgrade_now);
					item_change_att(fashion_ptr->get_fashion_template_ptr()->AttArray, 1, true);
				}
			}


			AdditionBuffTemplate* temp_addition_buff_template_ptr = GET_TEMPLATE(AdditionBuffTemplate, 58080001);
			int32 i = 0;
			m_fashion_star_num++;
			while (temp_addition_buff_template_ptr != nullptr)
			{
				if (temp_addition_buff_template_ptr->UpgradeNum >= m_fashion_star_num)
				{
					if (temp_addition_buff_template_ptr->UpgradeNum == m_fashion_star_num)
					{
						item_change_att(temp_addition_buff_template_ptr->AdditionValue, 1, true);
						if (i > 0)
						{
							temp_addition_buff_template_ptr = GET_TEMPLATE(AdditionBuffTemplate, 58080000 + i);
							item_change_att(temp_addition_buff_template_ptr->AdditionValue, 1, false);
						}
					}
					break;
				}

				temp_addition_buff_template_ptr = GET_TEMPLATE(AdditionBuffTemplate, 58080001 + ++i);
			}
		}

		if (fashion_ptr->get_data_info(e_item_info_activate) == 0)
		{
			fashion_ptr->set_data_info(e_item_info_activate, 1);
		}
		temp_player.get_phantom_mgr().on_item_oper(fashion_ptr, fashion_ptr->get_data_info(e_item_info_info_id));
		send_item_one(fashion_ptr);
	}
	else
	{
		is_big_upgrade = true;
		return e_item_string_full_level;
	}

	item_proto_show_item_upgrade_effect effect_msg;
	FasionTemplate* final_template_ptr = fashion_ptr->get_fashion_template_ptr();
	if (final_template_ptr != nullptr)
	{
		effect_msg.set_show_type(e_item_upgrade_effect_type_fashion_upgrade);
		effect_msg.set_show_value(fashion_ptr->get_data_info(e_item_info_upgrade_count));

		temp_player.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);
	}
	return e_error_code_success;
}

int32 item_set::mount_starupgrade(const guid_64& mount_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("mount", "mount_starupgrade", 1, false, "%d%s%d>%d", m_unit_array_index, mount_guid.to_string().c_str(), use_up_item, &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;
	int32 matirial_count = 0;
	if (!mount_guid.is_valid())
	{
		return e_item_string_unkown;
	}

	citem* mount_ptr = get_item(mount_guid);
	if (mount_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	if (mount_ptr->get_data_info(e_item_info_activate) <= 0)
	{
		return e_item_string_mount_unlock;
	}
	ItemTemplate* temp_item_template_ptr = mount_ptr->get_item_info_ptr();
	if (temp_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	int32 mount_real_id = temp_item_template_ptr->logic_id + mount_ptr->get_data_info(e_item_info_stargrade_count) + 1000;
	MountTemplate* real_mount_template_ptr = GET_TEMPLATE(MountTemplate, mount_real_id);
	if (real_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	if (real_mount_template_ptr->Type == mount_type_warlord || real_mount_template_ptr->Type == mount_type_lordlegionmem)
	{
		return e_item_string_auto_use_money_not_enough;
	}

	if ((real_mount_template_ptr->StarGradeNow >= real_mount_template_ptr->StarGradeMax))
	{
		return e_item_string_full_level;
	}

	int32 mount_grade_id = temp_item_template_ptr->logic_id + mount_ptr->get_data_info(e_item_info_upgrade_count) ;
	MountTemplate * grade_ptr = GET_TEMPLATE(MountTemplate, mount_grade_id);
	if (grade_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	if (real_mount_template_ptr->UpgradeLimit > grade_ptr->ClassNow)
	{
		return e_item_string_unkown;
	}

	int32 temp_cur_value = temp_player.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_mount);
	temp_player.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1);

	matirial_count = get_item_count(real_mount_template_ptr->UpgradeMatirialId, e_bag_type_bag);
	if (matirial_count < real_mount_template_ptr->UpgradeMatirialNum)
	{
		if (use_up_item == 0)
		{
			return e_item_string_matirial_not_enough;
		}
		else
		{
			use_money_instead_matirial = true;
		}

	}

	if (use_money_instead_matirial)
	{
		GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, real_mount_template_ptr->UpgradeMatirialShopId);
		if (goods_ptr != nullptr
			&& goods_ptr->NewPrice.size() > region_money_num_index)
		{
			int32 money_id = goods_ptr->NewPrice[region_money_type_index];
			int32 money_num = goods_ptr->NewPrice[region_money_num_index] * real_mount_template_ptr->UpgradeMatirialNum;
			if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
			{
				return e_item_string_auto_use_money_not_enough;
			}
			temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_mount_upgrade_auto_buy, real_mount_template_ptr->UpgradeMatirialNum);
		}
		else
		{
			return e_item_string_no_item;
		}
	}
	else
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_mount_starupgrade , 0, real_mount_template_ptr->UpgradeMatirialId, real_mount_template_ptr->UpgradeMatirialNum, e_item_locked);
	}

	int32 mount_starupgrade_now = mount_ptr->get_data_info(e_item_info_stargrade_count);
	mount_ptr->set_data_info(e_item_info_stargrade_count, mount_starupgrade_now + 1);

	item_change_att(real_mount_template_ptr->AttArray, 1, false);

	int32 next_mount_real_id = temp_item_template_ptr->logic_id + mount_ptr->get_data_info(e_item_info_stargrade_count) + 1000;
	MountTemplate* next_real_mount_template_ptr = GET_TEMPLATE(MountTemplate, next_mount_real_id);
	if (next_real_mount_template_ptr)
	{
		item_change_att(next_real_mount_template_ptr->AttArray, 1, true);

		// wangsonghao
		if (next_real_mount_template_ptr->StarBuffId > 0)
		{
			PassiveSkillTemplate* passive_skill_template_ptr = GET_TEMPLATE(PassiveSkillTemplate, next_real_mount_template_ptr->StarBuffId);
			if (passive_skill_template_ptr && passive_skill_template_ptr->OpenItem.size() < 2)
			{
				star_skill_unlock(mount_guid, mount_ptr->get_data_info(e_item_info_stargrade_count), false);
			}
		}
	}
	else
	{
		mount_ptr->set_data_info(e_item_info_stargrade_count, mount_starupgrade_now);
		item_change_att(real_mount_template_ptr->AttArray, 1, true);
	}
	send_item_one(mount_ptr);

	return e_error_code_success;
	*/
}

int32 item_set::mount_illusionupgrade(const guid_64& mount_guid, int32 use_up_item)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("mount", "mount_upgrade", 1, false, "%d%s%d>%d", m_unit_array_index, mount_guid.to_string().c_str(), use_up_item, &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	bool use_money_instead_matirial = false;
	int32 matirial_count = 0;
	if (!mount_guid.is_valid())
	{
		return e_item_string_unkown;
	}

	citem* mount_ptr = get_item(mount_guid);
	if (mount_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	if (mount_ptr->get_data_info(e_item_info_activate) <= 0)
	{
		return e_item_string_mount_unlock;
	}
	ItemTemplate* temp_item_template_ptr = mount_ptr->get_item_info_ptr();
	if (temp_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	int32 region_money_type_index = region_template_ptr->RegionCode * 2;
	int32 region_money_num_index = region_money_type_index + 1;

	int32 mount_real_id = temp_item_template_ptr->logic_id + mount_ptr->get_data_info(e_item_info_upgrade_count);
	MountTemplate* real_mount_template_ptr = GET_TEMPLATE(MountTemplate, mount_real_id);
	if (real_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	if (real_mount_template_ptr->Type != mount_type_illusion)
	{
		return e_item_string_auto_use_money_not_enough;
	}

	if (real_mount_template_ptr->GradeNow >= real_mount_template_ptr->MountGradeMax)
	{
		return e_item_string_full_level;
	}

	if (real_mount_template_ptr->PreMountRequirement.size() > 1)
	{
		citem* pre_mount = get_item_by_template_id(real_mount_template_ptr->PreMountRequirement[0]);
		if (pre_mount == nullptr)
		{
			return e_item_string_unkown;
		}
		if (pre_mount->get_mount_template_ptr()->ClassNow < real_mount_template_ptr->PreMountRequirement[1])
		{
			return e_item_string_unkown;
		}
	}
	int32 temp_cur_value = temp_player.get_welfare_mgr().get_active_degree_info(e_daily_active_degree_type_strengthing_one_mount);
	temp_player.get_welfare_mgr().set_active_degree_info(e_daily_active_degree_type_strengthing_one_mount, temp_cur_value + 1);

	matirial_count = get_item_count(real_mount_template_ptr->UpgradeMatirialId, e_bag_type_bag);
	if (matirial_count < real_mount_template_ptr->UpgradeMatirialNum)
	{
		if (use_up_item == 0)
		{
			return e_item_string_matirial_not_enough;
		}
		else
		{
			use_money_instead_matirial = true;
		}

	}

	if (use_money_instead_matirial)
	{
		GoodsTemplate* goods_ptr = GET_TEMPLATE(GoodsTemplate, real_mount_template_ptr->UpgradeMatirialShopId);
		if (goods_ptr != nullptr
			&& goods_ptr->NewPrice.size() > region_money_num_index)
		{
			int32 money_id = goods_ptr->NewPrice[region_money_type_index];
			int32 money_num = goods_ptr->NewPrice[region_money_num_index] * real_mount_template_ptr->UpgradeMatirialNum;
			if (!temp_player.can_cut_money((e_money_type)money_id, money_num))
			{
				return e_item_string_auto_use_money_not_enough;
			}
			temp_player.cut_money((e_money_type)money_id, money_num, e_server_log_cut_money_mount_upgrade_auto_buy, real_mount_template_ptr->UpgradeMatirialNum);
		}
		else
		{
			return e_item_string_no_item;
		}
	}
	else
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_mount_illusionupgrade , 0, real_mount_template_ptr->UpgradeMatirialId, real_mount_template_ptr->UpgradeMatirialNum, e_item_locked);
	}

	item_change_att(real_mount_template_ptr->AttArray, 1, false);
	int32 mount_upgrade_now = mount_ptr->get_data_info(e_item_info_upgrade_count);
	mount_ptr->set_data_info(e_item_info_upgrade_count, mount_upgrade_now + 1);
	if (mount_ptr->get_mount_template_ptr())
	{
		item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, true);

		// wangsonghao
		if (mount_ptr->get_mount_template_ptr()->StarBuffId > 0)
		{
			star_skill_unlock(mount_guid, mount_ptr->get_data_info(e_item_info_upgrade_count), false, false);
		}
	}
	else
	{
		mount_ptr->set_data_info(e_item_info_upgrade_count, mount_upgrade_now);
		item_change_att(mount_ptr->get_mount_template_ptr()->AttArray, 1, true);
	}
	send_item_one(mount_ptr);

	item_proto_show_item_upgrade_effect effect_msg;
	MountTemplate* final_template_ptr = mount_ptr->get_mount_template_ptr();
	if (final_template_ptr != nullptr)
	{
		effect_msg.set_show_type(e_item_upgrade_effect_type_mount_upgrade);
		effect_msg.set_show_value(final_template_ptr->GradeNow);
		temp_player.send_message_to_self(&effect_msg, e_msgindex_s2c_show_item_upgrade_effect);
	}

	return e_error_code_success;
	*/
}

int32 item_set::mount_unlock(const guid_64& mount_guid)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("mount", "mount_unlock", 1, false, "%d%s>%d", m_unit_array_index, mount_guid.to_string().c_str(), &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* temp_mount_item = get_item(mount_guid);
	if (temp_mount_item == nullptr)
	{
		return e_item_string_no_item;
	}
	if (temp_mount_item->get_data_info(e_item_info_activate) > 0 && temp_mount_item->get_data_info(e_item_info_over_time) == 0)
	{
		return e_item_string_cant_unlock_again;
	}

	MountTemplate* temp_mount_template_ptr = temp_mount_item->get_mount_template_ptr();
	if (temp_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	bool matirial_done = false;
	bool money_done = false;
	bool pre_mount_done = false;

	bool matirial_cost = false;
	bool money_cost = false;

	if (temp_mount_template_ptr->UnlockMoneyRequirement.size() > 1)
	{
		money_cost = true;
		if (!temp_player.can_cut_money((e_money_type)temp_mount_template_ptr->UnlockMoneyRequirement[0], temp_mount_template_ptr->UnlockMoneyRequirement[1]))
		{
			return e_item_string_money;
		}
		money_done = true;
	}
	else
	{
		money_done = true;
	}

	if (temp_mount_template_ptr->UnlockMatirialRequirement.size() > 1)
	{
		matirial_cost = true;
		int32 cur_matirial_num = get_item_count(temp_mount_template_ptr->UnlockMatirialRequirement[0], e_bag_type_bag);
		if (cur_matirial_num<temp_mount_template_ptr->UnlockMatirialRequirement[1])
		{
			return e_item_string_matirial_not_enough;
		}

		matirial_done = true;
	}
	else
	{
		matirial_done = true;
	}

	if (temp_mount_template_ptr->PreMountRequirement.size() > 1)
	{
		citem* pre_mount = get_item_by_template_id(temp_mount_template_ptr->PreMountRequirement[0]);
		if (pre_mount == nullptr)
		{
			return e_item_string_unkown;
		}
		if (pre_mount->get_mount_template_ptr()->ClassNow >= temp_mount_template_ptr->PreMountRequirement[1])
		{
			pre_mount_done = true;
		}
	}
	else
	{
		pre_mount_done = true;
	}



	if (matirial_done && money_done && pre_mount_done)
	{
		if (matirial_cost)
		{
			cost_item_by_id_with_lock_states(e_server_log_del_item_mount_unlock , 0, temp_mount_template_ptr->UnlockMatirialRequirement[0], temp_mount_template_ptr->UnlockMatirialRequirement[1], e_item_locked);
		}
		if (money_cost)
		{
			temp_player.cut_money((e_money_type)temp_mount_template_ptr->UnlockMoneyRequirement[0], temp_mount_template_ptr->UnlockMoneyRequirement[1], e_server_log_cut_money_mount_unlock, temp_mount_item->get_data_info(e_item_info_info_id));
		}

		temp_mount_item->set_data_info(e_item_info_activate, 1);
		temp_mount_item->set_data_info(e_item_info_over_time, 0);
		item_change_att(temp_mount_item->get_mount_template_ptr()->AttArray, 1, true);
		send_item_one(temp_mount_item);

		show_this_mount(temp_mount_item, -1);

		//sync_ranking_mount();

		if (temp_mount_template_ptr->NoticeId > 0)
		{
			s_chat_notice_info notice_data;
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(temp_mount_template_ptr->MountNameId);
			notice_data.item_name.push_back(temp_string);
			temp_player.get_chat_mgr().send_notice_new(temp_mount_template_ptr->NoticeId, notice_data);
		}

		//mission check
		temp_player.get_mission_mgr().target_check(e_mission_end_type_mount_unlock);
		temp_player.get_achievement_mgr().item_changed(temp_mount_item, e_item_operation_mount_unlock);
		temp_player.refresh_service_goal(e_service_goal_type_mount_num);
		add_new_quick_mount_call(temp_mount_item->get_item_guid());
		temp_player.get_mission_mgr().target_check(e_mission_end_type_mount_total_grade);
		temp_player.get_phantom_mgr().on_item_oper(temp_mount_item, temp_mount_item->get_data_info(e_item_info_info_id));
		return e_error_code_success;
	}
	return e_item_string_unlock_requirement_not_reach;
	*/
}


void  item_set::sync_ranking_mount()
{
	int32	sum_fight_num = 0;
	int32	max_fight_num = 0;
	citem* highest_mount_item = nullptr;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	for (int32 index = 0; index < MAX_MOUNT_NUM; index++)
	{
		citem* temp_mount_item = get_item(GET_BAG(e_bag_type_mount)[index]);
		if (temp_mount_item != nullptr)
		{
			if (temp_mount_item->get_item_inst().data_ary[e_item_info_activate] > 0)
			{
				MountTemplate* mount_template_ptr = temp_mount_item->get_mount_template_ptr();
				if (nullptr != mount_template_ptr)
				{
					if (mount_template_ptr->Type != faith::mount_type_non)
					{
						int32 temp = temp_mount_item->calcu_single_item_fighting_power(m_unit_array_index);
						sum_fight_num += temp;
						if (mount_template_ptr->Type == faith::mount_type_normal || mount_template_ptr->Type == faith::mount_type_special)
						{
							if (temp >= max_fight_num)
							{
								max_fight_num = temp;
								highest_mount_item = temp_mount_item;
							}
						}
					}
				}
			}
		}
	}

	sum_fight_num += calcu_beast_sprite_fighting_power(e_bag_type_mount_beast_spirit);

	temp_player.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_mount, sum_fight_num);
	if (temp_player.check_and_set_highest_record(e_role_history_highest_record_mount_value, sum_fight_num) == true)
	{
		temp_player.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_mount_up);
		temp_player.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_cross_mount_up);
	}
	if (highest_mount_item != nullptr)
	{
		temp_player.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_mount_train, max_fight_num, highest_mount_item->get_item_template_id());
	}
}

void  item_set::sync_ranking_spirit()
{
	int32	sum_fight_num = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}

	//sum_fight_num = temp_player.get_spirit_mgr().get_total_spirit_fight_power();
	sum_fight_num += calcu_beast_sprite_fighting_power(e_bag_type_spirit_beast_spirit);
	temp_player.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_elves_gs, sum_fight_num);

	if (temp_player.check_and_set_highest_record(e_role_history_highest_record_spirit_value, sum_fight_num) == true)
	{
		temp_player.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_spirit_up);
		temp_player.get_time_limit_activity_mgr().set_up_rank_activity_schedule(e_time_limit_activity_type_cross_spirit_up);
	}
}

int32 item_set::show_this_mount(citem* mount_ptr, int32 item_slot, bool is_buff)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("mount", "show_this_mount", 1, false, "%d%s%d%b>%d", m_unit_array_index, mount_ptr->get_item_guid().to_string().c_str(), item_slot, is_buff, &result);
	return result;
	/*
	if (mount_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return e_item_string_unkown;
	}

	ItemTemplate* temp_mount_item_template_ptr = mount_ptr->get_item_info_ptr();
	if (temp_mount_item_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}

	MountTemplate* temp_mount_template_ptr = GET_TEMPLATE(MountTemplate, temp_mount_item_template_ptr->logic_id);
	if (temp_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	item_proto_change_mount_shape msg;
	guid_64 role_guid = temp_unit.get_unit_guid();

	msg.add_role_guid(role_guid.A);
	msg.add_role_guid(role_guid.B);
	msg.set_is_buff(is_buff);
	if (item_slot < 0)
	{
		if (mount_ptr->get_data_info(e_item_info_activate) <= 0)
		{
			return e_item_string_cant_show_unlock_mount;
		}

		temp_unit.set_unit_info(e_role_info_mount_showd_template_id, mount_ptr->get_data_info(e_item_info_info_id));
		temp_unit.send_info_one(e_role_info_mount_showd_template_id);
		temp_unit.get_item_set().set_showed_mount();
		mount_ptr->set_data_info(e_item_info_showing_illusion_idex, item_slot);
		send_item_one(mount_ptr);
		msg.set_mount_template_id(mount_ptr->get_data_info(e_item_info_info_id));
		temp_unit.send_message_to_aoi(&msg, e_msgindex_s2c_change_mount_shape);
	}
	else
	{
		if (item_slot >= temp_mount_template_ptr->IllusionList.size())
		{
			return e_item_string_unkown;
		}
		citem* mount_illusion_ptr = get_item_by_template_id(temp_mount_template_ptr->IllusionList[item_slot]);
		if (mount_illusion_ptr == nullptr)
		{
			return e_item_string_no_item;
		}
		if (mount_illusion_ptr->get_data_info(e_item_info_activate) <= 0)
		{
			return e_item_string_cant_show_unlock_mount;
		}

		temp_unit.set_unit_info(e_role_info_mount_showd_template_id, mount_illusion_ptr->get_data_info(e_item_info_info_id));
		temp_unit.send_info_one(e_role_info_mount_showd_template_id);
		temp_unit.get_item_set().set_showed_mount();
		mount_ptr->set_data_info(e_item_info_showing_illusion_idex, item_slot);
		send_item_one(mount_ptr);
		msg.set_mount_template_id(mount_illusion_ptr->get_data_info(e_item_info_info_id));
		temp_unit.send_message_to_aoi(&msg, e_msgindex_s2c_change_mount_shape);
	}

	return e_error_code_success;
	*/
}

int32 item_set::show_this_mount_illusion(const guid_64& mount_guid, int32 item_slot)
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* temp_mount_ptr = get_item(mount_guid);
	if (temp_mount_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	if (temp_mount_ptr->get_data_info(e_item_info_activate) <= 0)
	{
		return e_item_string_mount_unlock;
	}
	MountTemplate* mount_template_ptr = temp_mount_ptr->get_mount_template_ptr();
	if (mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}
	if (item_slot >= 0)
	{
		if (mount_template_ptr->IllusionList.empty()
			|| item_slot >= mount_template_ptr->IllusionList.size())
		{
			return e_item_string_have_no_illusion;
		}
		citem* temp_illusion_ptr = get_item_by_template_id(mount_template_ptr->IllusionList[item_slot]);
		if (temp_mount_ptr == nullptr)
		{
			return e_item_string_mount_illusion_unlock;
		}
		if (item_slot == temp_mount_ptr->get_data_info(e_item_info_showing_illusion_idex))
		{
			if (temp_mount_ptr->get_data_info(e_item_info_showing_illusion_idex) == -1)
			{
				return e_item_string_unkown;
			}
			//item_slot = -1;
		}
	}

	temp_mount_ptr->set_data_info(e_item_info_showing_illusion_idex, item_slot);
	send_item_one(temp_mount_ptr);

	bool is_need_change_ride_mount = false;
	int32 cur_ride_mount_id = temp_unit.get_unit_info(e_role_info_mount_showd_template_id);
	if (cur_ride_mount_id == temp_mount_ptr->get_data_info(e_item_info_info_id))
	{
		is_need_change_ride_mount = true;
	}
	else
	{
		for (int32 i = 0; i < mount_template_ptr->IllusionList.size(); ++i)
		{
			if (mount_template_ptr->IllusionList[i] == cur_ride_mount_id)
			{
				is_need_change_ride_mount = true;
				break;
			}
		}
	}
	//if (is_need_change_ride_mount)
	{
		show_this_mount(temp_mount_ptr, item_slot);
		//temp_unit.set_unit_info(e_role_info_mount_showd_template_id, temp_illusion_ptr->get_data_info(e_item_info_info_id));
		//temp_unit.send_info_one(e_role_info_mount_showd_template_id);
		//temp_unit.get_item_set().set_showed_mount();
	}
	return e_error_code_success;
}

int32 item_set::get_mount_illusion(const guid_64& mount_guid, int32 illusion_slot)
{
	int32 result = 0;
	//script_mgr::get_instance().call_func(nullptr, "reload", 0, false, "");
	script_mgr::get_instance().call_func("mount", "get_mount_illusion", 1, false, "%d%s%d>%d", m_unit_array_index, mount_guid.to_string().c_str(), illusion_slot, &result);
	return result;
	/*
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return e_item_string_unkown;
	}
	citem* temp_mount_ptr = get_item(mount_guid);

	if (temp_mount_ptr == nullptr)
	{
		return e_item_string_no_item;
	}
	MountTemplate* temp_mount_template_ptr = temp_mount_ptr->get_mount_template_ptr();
	if (temp_mount_template_ptr == nullptr)
	{
		return e_item_string_unkown;
	}


	if (illusion_slot >= temp_mount_template_ptr->IllusionList.size())
	{
		return e_item_string_have_no_illusion;
	}

	if (get_item_by_template_id(temp_mount_template_ptr->IllusionList[illusion_slot]))
	{
		return e_item_string_cant_unlock_again;
	}

	ItemTemplate* temp_illusion_mount_item_ptr = GET_TEMPLATE(ItemTemplate, temp_mount_template_ptr->IllusionList[illusion_slot]);
	if (temp_illusion_mount_item_ptr == nullptr)
	{
		return e_item_string_unkown;
	}


	MountTemplate* temp_illusion_template_ptr = GET_TEMPLATE(MountTemplate, temp_illusion_mount_item_ptr->logic_id);
	if (temp_illusion_template_ptr == nullptr)
	{
		return e_item_string_have_no_illusion;
	}
	if (temp_illusion_template_ptr->PreMountRequirement.size() > 1)
	{
		if (temp_mount_template_ptr->ClassNow < temp_illusion_template_ptr->PreMountRequirement[1])
		{
			return e_error_code_item_mount_class_not_reach;
		}
	}

	if (temp_illusion_template_ptr->UnlockMoneyRequirement.size() >= 2)
	{
		if (!temp_player.can_cut_money((e_money_type)temp_illusion_template_ptr->UnlockMoneyRequirement[0], temp_illusion_template_ptr->UnlockMoneyRequirement[1]))
		{
			return e_item_string_money;
		}
	}
	if (temp_illusion_template_ptr->UnlockMatirialRequirement.size() >= 2)
	{
		int32 cur_matirial_num = get_item_count(temp_illusion_template_ptr->UnlockMatirialRequirement[0], e_bag_type_bag);
		if (cur_matirial_num < temp_illusion_template_ptr->UnlockMatirialRequirement[1])
		{
			return e_item_string_matirial_not_enough;
		}
	}

	citem* illusion_item = create_item_by_template(e_server_log_add_item_mount_illusion, 0, temp_mount_template_ptr->IllusionList[illusion_slot], 1, 0);
	if (illusion_item == nullptr)
	{
		return e_item_string_unkown;
	}

	if (temp_illusion_template_ptr->UnlockMoneyRequirement.size() >= 2)
	{
		temp_player.cut_money((e_money_type)temp_illusion_template_ptr->UnlockMoneyRequirement[0], temp_illusion_template_ptr->UnlockMoneyRequirement[1], e_server_log_cut_money_get_mount_illusion, temp_mount_template_ptr->IllusionList[illusion_slot]);
	}
	if (temp_illusion_template_ptr->UnlockMatirialRequirement.size() >= 2)
	{
		cost_item_by_id_with_lock_states(e_server_log_del_item_mount_illusion , 0, temp_illusion_template_ptr->UnlockMatirialRequirement[0], temp_illusion_template_ptr->UnlockMatirialRequirement[1], e_item_locked);
	}

	illusion_item->set_data_info(e_item_info_activate, 1);
	item_change_att(temp_illusion_template_ptr->AttArray, 1, 1);
	send_item_one(illusion_item);

	show_this_mount(temp_mount_ptr, illusion_slot);
	temp_player.get_achievement_mgr().item_changed(temp_mount_ptr, e_item_operation_mount_get_illusion);
	return e_error_code_success;
	*/
}
void item_set::set_showed_wing()
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; i++)
	{
		if (GET_BAG(e_bag_type_wing)[i].is_valid())
		{
			citem* temp_wing_item = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_wing_item)
			{
				if (temp_wing_item->get_data_info(e_item_info_info_id) == temp_unit.get_unit_info(e_role_info_wing_showd_template_id))
				{
					m_show_wing_item_ptr = temp_wing_item;
					return;
				}
			}
		}
	}
	m_show_wing_item_ptr = nullptr;
}
citem* item_set::get_showed_wing()
{
	return m_show_wing_item_ptr;
}
void item_set::set_showed_mount()
{
	unit& temp_unit = unit_man::get_unit(m_unit_array_index);
	if (temp_unit.is_valid() == false)
	{
		return;
	}
	for (int32 i = 0; i < MAX_MOUNT_NUM; i++)
	{
		if (GET_BAG(e_bag_type_mount)[i].is_valid())
		{
			citem* temp_mount_item = get_item(GET_BAG(e_bag_type_mount)[i]);
			if (temp_mount_item)
			{
				if (temp_mount_item->get_data_info(e_item_info_info_id) == temp_unit.get_unit_info(e_role_info_mount_showd_template_id))
				{
					m_show_mount_item_ptr = temp_mount_item;
					return;
				}
			}
		}
	}
	m_show_mount_item_ptr = nullptr;
}
citem* item_set::get_showed_mount()
{
	return m_show_mount_item_ptr;
}

citem* item_set::get_illusion_range_mount_or_wing(citem* item_mount_or_wing)
{
	if (nullptr == item_mount_or_wing)
	{
		return nullptr;
	}

	if (item_mount_or_wing->get_data_info(e_item_info_container_type) == e_bag_type_mount)
	{
		MountTemplate* mount_template_ptr = item_mount_or_wing->get_mount_template_ptr();
		if (nullptr == mount_template_ptr)
		{
			return nullptr;
		}

		if (mount_template_ptr->IllusionOriginalIdRange.size() >= e_mount_illusion_originalid_range_max)
		{
			ItemTemplate* item_mount_illusion_template_ptr = template_manager::get_instance().get_item_template_ptr_by_logic_id(mount_template_ptr->IllusionOriginalIdRange[faith::e_mount_illusion_originalid_range_low]);
			if (nullptr == item_mount_illusion_template_ptr)
			{
				return nullptr;
			}
			citem* item_mount_illusion = get_item_by_template_id(item_mount_illusion_template_ptr->attribute_id);
			if (nullptr == item_mount_illusion)
			{
				return nullptr;
			}
			else
			{
				return item_mount_illusion;
			}
		}
		else
		{
			return nullptr;
		}

	}
	else if (item_mount_or_wing->get_data_info(e_item_info_container_type) == e_bag_type_wing)
	{
		WingTemplate* wing_template_ptr = item_mount_or_wing->get_wing_template_ptr();
		if (nullptr == wing_template_ptr)
		{
			return nullptr;
		}

		if (wing_template_ptr->IllusionOriginalIdRange.size() >= e_wing_illusion_originalid_range_max)
		{
			ItemTemplate* item_wing_illusion_template_ptr = template_manager::get_instance().get_item_template_ptr_by_logic_id(wing_template_ptr->IllusionOriginalIdRange[faith::e_wing_illusion_originalid_range_low]);
			if (nullptr == item_wing_illusion_template_ptr)
			{
				return nullptr;
			}
			citem* item_wing_illusion = get_item_by_template_id(item_wing_illusion_template_ptr->attribute_id);
			if (nullptr == item_wing_illusion)
			{
				return nullptr;
			}
			else
			{
				return item_wing_illusion;
			}
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

citem* item_set::get_mount_by_mount_template_id(int32 mount_template_id)
{
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		if (GET_BAG(e_bag_type_mount)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_mount)[i]);
			if (temp_item)
			{
				if (temp_item->get_item_info_ptr()->logic_id == mount_template_id)
				{
					return temp_item;

				}
			}
		}
	}
	return nullptr;
}

void item_set::unlock_all_mount()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}

	set_mount_att(false);
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		if (GET_BAG(e_bag_type_mount)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_mount)[i]);
			if (temp_item
				&& temp_item->get_data_info(e_item_info_info_id) != city_master_mount_id
				&& temp_item->get_data_info(e_item_info_info_id) != city_master_union_mount_id)
			{
				temp_item->set_data_info(e_item_info_activate, 1);
				send_item_one(temp_item);

				temp_player.get_phantom_mgr().on_item_oper(temp_item, temp_item->get_data_info(e_item_info_info_id));
			}
		}
	}
	temp_player.refresh_service_goal(e_service_goal_type_mount_num);
	show_this_init_mount();
	set_mount_att(true);
}

void item_set::init_mount_skill_att()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		if (GET_BAG(e_bag_type_mount)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_mount)[i]);
			if (temp_item && temp_item->get_data_info(e_item_info_activate) > 0)
			{
				for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property3; ++i)
				{
					temp_player.get_passive_skill().passive_skill_special(temp_item->get_data_info((e_item_info)i), true);
				}
			}

			temp_player.get_mount_power_mgr().set_core_skill(temp_item);

		}

	}
}

void item_set::unlock_all_wing()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	set_wing_att(false);
	for (int32 i = 0; i < MAX_WING_NUM; ++i)
	{
		if (GET_BAG(e_bag_type_wing)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_item
				&& temp_item->get_data_info(e_item_info_activate) <= 0
				&& temp_item->get_data_info(e_item_info_info_id) != city_master_wing_id)
			{
				ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
				if (item_template_ptr != nullptr
					&& item_template_ptr->sub_type == e_wing_sub_type_wing)
				{
					temp_item->set_data_info(e_item_info_activate, 1);
					WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, item_template_ptr->logic_id);
					if (wing_template_ptr == nullptr)
					{
						continue;
					}
					if (wing_template_ptr->UpgradeNow != wing_template_ptr->WingUpgradeMax)
					{
						temp_item->set_data_info(e_item_info_upgrade_count, wing_template_ptr->WingAddStarMax);
					}
					if (wing_template_ptr->StarBuffId > 0)
					{
						temp_item->set_data_info(e_item_info_succinct_property2, wing_template_ptr->StarBuffId);
						temp_player.get_passive_skill().passive_skill_special(wing_template_ptr->StarBuffId, true);
					}
					temp_player.get_phantom_mgr().on_item_oper(temp_item, temp_item->get_data_info(e_item_info_info_id));
					if (temp_item->get_wing_template_ptr() != nullptr && temp_item->get_wing_template_ptr()->Type == e_item_wing_type_normal)
					{
						temp_player.get_achievement_mgr().item_changed(temp_item, e_item_operation_featherupgrade_grade);
					}
				}
			}
			else
			{
				break;
			}
			send_item_one(temp_item);
		}
	}
	equip_all_feather_and_spirit_and_soul();
	reset_wings_order();
	send_wing_item_all();
	show_this_init_max_wing();
	set_wing_att(true);
}

void item_set::init_wing_skill_att()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}
	for (int32 i = 0; i < MAX_WING_ELEMENTS_NUM; ++i)
	{
		if (GET_BAG(e_bag_type_wing)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_wing)[i]);
			if (temp_item && temp_item->get_data_info(e_item_info_activate) > 0)
			{
				for (int32 i = e_item_info_succinct_property1; i <= e_item_info_succinct_property3; ++i)
				{
					player_ref.get_passive_skill().passive_skill_special(temp_item->get_data_info((e_item_info)i), true);
				}

				wing_psychic_skill_unlock(temp_item->get_data_info(e_item_info_forge_property3), true);
			}
		}
	}
}

void item_set::save_show_info_to_db(int32 save_type)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return;
	}
	cs2dp_save_char_show msg;
	msg.show_info.role_guid = player_ref.get_unit_guid();
	for (int32 i = 0; i < char_quick_call_mount_max; ++i)
	{
		msg.show_info.quick_call_mount_guid_array[i] = m_quick_call_mount_array[i];
	}

	msg.show_info.showed_quickly_hp_guid = m_quick_hp_guid;
	player_ref.send_message_to_dp(&msg, sizeof(cs2dp_save_char_show));
}

void item_set::load_char_show_info(const s_char_show_info* show_data)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	item_proto_item_show_info msg;
	//item_proto_quick_mount_array msg;
	for (int32 i = 0; i < char_quick_call_mount_max; ++i)
	{
		m_quick_call_mount_array[i] = show_data->quick_call_mount_guid_array[i];
		msg.add_mount_guid(m_quick_call_mount_array[i].A);
		msg.add_mount_guid(m_quick_call_mount_array[i].B);
	}
	temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_quick_call_mount);

	item_proto_item_show_info_quickly_hp msg_data;
	msg_data.set_quickly_hp_guid(show_data->showed_quickly_hp_guid.server_64);
	temp_player.send_message_to_self(&msg_data, e_msgindex_s2c_update_quickly_hp);


}

void item_set::set_quick_mount_call_array(guid_64* guid_array)
{
	if (guid_array == nullptr)
	{
		return;
	}
	for (int32 i = 0; i < char_quick_call_mount_max; ++i)
	{
		citem* temp_item = get_item(guid_array[i]);

		if (temp_item == nullptr || temp_item->get_item_inst().data_ary[e_item_info_activate] <= 0)
		{
			m_quick_call_mount_array[i] = guid_64(0, 0);
			continue;
		}
		ItemTemplate* temp_template = temp_item->get_item_info_ptr();
		if (temp_template == nullptr)
		{
			m_quick_call_mount_array[i] = guid_64(0, 0);
			continue;
		}

		if (temp_template->item_type != e_item_type_mount)
		{
			m_quick_call_mount_array[i] = guid_64(0, 0);
			continue;
		}

		m_quick_call_mount_array[i] = guid_array[i];
	}
}

void item_set::add_new_quick_mount_call(const guid_64& new_mount_guid)
{
	for (int32 char_quick_call_mount_index = 0; char_quick_call_mount_index < char_quick_call_mount_max; char_quick_call_mount_index++)
	{
		guid_64& mount_guid = m_quick_call_mount_array[char_quick_call_mount_index];
		if (false == mount_guid.is_valid())
		{
			m_quick_call_mount_array[char_quick_call_mount_index] = new_mount_guid;
			break;
		}
	}
	send_quick_mount_call();
}

void item_set::send_quick_mount_call()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	item_proto_item_show_info msg;
	//item_proto_quick_mount_array msg;
	for (int32 i = 0; i < char_quick_call_mount_max; ++i)
	{
		msg.add_mount_guid(m_quick_call_mount_array[i].A);
		msg.add_mount_guid(m_quick_call_mount_array[i].B);
	}
	temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_quick_call_mount);
}

void item_set::clear_quick_mount_call_element(const guid_64& mount_guid)
{
	if (mount_guid.is_valid() == false)
	{
		return;
	}
	for (int32 index = 0; index < char_quick_call_mount_max; ++index)
	{
		if (m_quick_call_mount_array[index] == mount_guid)
		{
			m_quick_call_mount_array[index] = guid_64(0, 0);
			break;
		}
	}
	return;
}

void item_set::get_hope_item(int32 get_typ)
{
	item_proto_item_hope_item_get_end msg;
	msg.set_get_typ(get_typ);

	msg.set_get_result(0);

	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	int32 player_class_type = temp_player.get_unit_info(e_role_info_class_type);
	if (player_class_type >= GAMECONFIG->HopeItemGetOneFreeDropID.size()
		|| player_class_type >= GAMECONFIG->HopeItemGetOneCostDropID.size()
		|| player_class_type >= GAMECONFIG->HopeItemGetTenDropID.size()
		|| player_class_type >= GAMECONFIG->HopeItemGetRareDropID.size()
		|| player_class_type >= GAMECONFIG->HopeItemExchangeVoucherGetTenDropID.size())
	{
		return;
	}

	msg.add_role_guid(temp_player.get_unit_guid().A);
	msg.add_role_guid(temp_player.get_unit_guid().B);

	int32 drop_template_id = 0;
	int32 get_result_id = 0;

	int32 cur_hope_value = temp_player.get_unit_info(e_role_info_hope_value);
	int32 max_hope_value = GAMECONFIG->HopeItemGetHopeValueMax;
	int32 rare_drop_id = GAMECONFIG->HopeItemGetRareDropID[player_class_type];

	std::vector<s_item_template_info> final_list;
	bool is_first_time_get_hope = false;
	switch (get_typ)
	{
	case e_item_get_hope_item_typ_one_free:
	{
		int32 last_get_time = temp_player.get_time_data(e_time_type_hope_item_free_get_time);
		int32 max_interval_time = GAMECONFIG->HopeItemFreeGetTime;
		int32 cur_time = time_helper::get_cur_time_new().second;
		if (max_interval_time > (cur_time - last_get_time))
		{
			msg.set_get_result(e_item_string_cant_free_get_hope_item);
			temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
			return;
		}
		else
		{
			if (temp_player.get_first_time_do_mgr().is_first_time_do(e_first_time_do_type_lucky_draw_item))
			{
				drop_template_id = GAMECONFIG->HopeItemGetFirstTimeDropID[player_class_type];
				temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_lucky_draw_item);
				is_first_time_get_hope = true;
			}
			else if (cur_hope_value >= max_hope_value)
			{
				drop_template_id = rare_drop_id;
				temp_player.set_unit_info(e_role_info_hope_value, 0);
				temp_player.send_info_one(e_role_info_hope_value);
			}
			else
			{
				drop_template_id = cdrop::get_drop_id(GAMECONFIG->HopeItemGetOneFreeDropID[player_class_type]);
				if (drop_template_id != rare_drop_id)
				{
					temp_player.set_unit_info(e_role_info_hope_value, cur_hope_value + 1);
					temp_player.send_info_one(e_role_info_hope_value);
				}
				else
				{
					temp_player.set_unit_info(e_role_info_hope_value, 0);
					temp_player.send_info_one(e_role_info_hope_value);
				}
			}
			std::vector<s_item_template_info> item_drop_list_with_att;
			s_item_template_info temp_item_with_num;
			cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

			if (item_drop_list_with_att.empty())
			{
				msg.set_get_result(e_item_string_unkown);
				temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
				return;
			}
			temp_item_with_num.m_item_id = item_drop_list_with_att[0].m_item_id;
			temp_item_with_num.m_item_num = item_drop_list_with_att.size();
			final_list.push_back(temp_item_with_num);

			int32 cur_get_time = time_helper::get_cur_time_new().second;
			temp_player.set_time_data(e_time_type_hope_item_free_get_time, cur_get_time);
			msg.set_free_get_time(cur_get_time);
		}
	}
	break;
	case e_item_get_hope_item_typ_one_voucher:
	{
		int32 hop_item_get_voucher_id = GAMECONFIG->HopeItemExchangeVoucherID;
		int32 voucher_num = get_item_count(e_bag_type_bag, hop_item_get_voucher_id);
		if (voucher_num > 0)
		{
			if (temp_player.get_first_time_do_mgr().is_first_time_do(e_first_time_do_type_lucky_draw_item))
			{
				drop_template_id = GAMECONFIG->HopeItemGetFirstTimeDropID[player_class_type];
				temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_lucky_draw_item);
				is_first_time_get_hope = true;
			}
			else if (cur_hope_value >= max_hope_value)
			{
				drop_template_id = rare_drop_id;
				temp_player.set_unit_info(e_role_info_hope_value, 0);
				temp_player.send_info_one(e_role_info_hope_value);
			}
			else
			{
				drop_template_id = cdrop::get_drop_id(GAMECONFIG->HopeItemGetOneFreeDropID[player_class_type]);
				if (drop_template_id != rare_drop_id)
				{
					temp_player.set_unit_info(e_role_info_hope_value, cur_hope_value + 1);
					temp_player.send_info_one(e_role_info_hope_value);
				}
				else
				{
					temp_player.set_unit_info(e_role_info_hope_value, 0);
					temp_player.send_info_one(e_role_info_hope_value);
				}
			}
			std::vector<s_item_template_info> item_drop_list_with_att;
			s_item_template_info temp_item_with_num;
			cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

			if (item_drop_list_with_att.empty())
			{
				msg.set_get_result(e_item_string_unkown);
				temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
				return;
			}
			temp_item_with_num.m_item_id = item_drop_list_with_att[0].m_item_id;
			temp_item_with_num.m_item_num = item_drop_list_with_att.size();
			final_list.push_back(temp_item_with_num);

			cost_item_by_id_with_lock_states(e_server_log_del_item_hope_item_one, 0, hop_item_get_voucher_id, 1, 1);
		}
		else
		{
		}
	}
	break;
	case e_item_get_hope_item_typ_ten_voucher:
	{
		int32 hop_item_get_voucher_id = GAMECONFIG->HopeItemExchangeVoucherID;
		int32 voucher_num = get_item_count(e_bag_type_bag, hop_item_get_voucher_id);
		if (voucher_num > 9)
		{
			for (int32 i = 0; i < 10; ++i)
			{
				cur_hope_value = temp_player.get_unit_info(e_role_info_hope_value);
				if (temp_player.get_first_time_do_mgr().is_first_time_do(e_first_time_do_type_lucky_draw_item))
				{
					drop_template_id = GAMECONFIG->HopeItemGetFirstTimeDropID[player_class_type];
					temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_lucky_draw_item);
					is_first_time_get_hope = true;
				}
				else if (cur_hope_value >= max_hope_value)
				{
					drop_template_id = rare_drop_id;
					temp_player.set_unit_info(e_role_info_hope_value, 0);
					temp_player.send_info_one(e_role_info_hope_value);
				}
				else
				{
					drop_template_id = cdrop::get_drop_id(GAMECONFIG->HopeItemExchangeVoucherGetTenDropID[player_class_type]);
					if (drop_template_id != rare_drop_id)
					{
						temp_player.set_unit_info(e_role_info_hope_value, cur_hope_value + 1);
						temp_player.send_info_one(e_role_info_hope_value);
					}
					else
					{
						temp_player.set_unit_info(e_role_info_hope_value, 0);
						temp_player.send_info_one(e_role_info_hope_value);
					}
				}
				std::vector<s_item_template_info> item_drop_list_with_att;
				s_item_template_info temp_item_with_num;
				cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

				if (item_drop_list_with_att.empty())
				{
					msg.set_get_result(e_item_string_unkown);
					temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
					return;
				}
				temp_item_with_num.m_item_id = item_drop_list_with_att[0].m_item_id;
				temp_item_with_num.m_item_num = item_drop_list_with_att.size();
				final_list.push_back(temp_item_with_num);
			}
			cost_item_by_id_with_lock_states(e_server_log_del_item_hope_item_ten, 0, hop_item_get_voucher_id, 10, 1);
		}
		else
		{
		}
	}
	break;
	case e_item_get_hope_item_typ_one_cost:
	{
		int32 cost = GAMECONFIG->HopeItemOneCost;
		int64 hope_point_get = GAMECONFIG->HopeItemOneTimesScore;
		if (!temp_player.can_cut_money(e_money_type_jewel, cost))
		{
			msg.set_get_result(e_item_string_jewel);
			temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
			return;
		}
		if (temp_player.get_first_time_do_mgr().is_first_time_do(e_first_time_do_type_lucky_draw_item))
		{
			drop_template_id = GAMECONFIG->HopeItemGetFirstTimeDropID[player_class_type];
			temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_lucky_draw_item);
			is_first_time_get_hope = true;
		}
		else if (cur_hope_value >= max_hope_value)
		{
			drop_template_id = rare_drop_id;
			temp_player.set_unit_info(e_role_info_hope_value, 0);
			temp_player.send_info_one(e_role_info_hope_value);
		}
		else
		{
			drop_template_id = cdrop::get_drop_id(GAMECONFIG->HopeItemGetOneCostDropID[player_class_type]);
			if (drop_template_id != rare_drop_id)
			{
				temp_player.set_unit_info(e_role_info_hope_value, cur_hope_value + 1);
				temp_player.send_info_one(e_role_info_hope_value);
			}
			else
			{
				temp_player.set_unit_info(e_role_info_hope_value, 0);
				temp_player.send_info_one(e_role_info_hope_value);
			}
		}
		std::vector<s_item_template_info> item_drop_list_with_att;
		s_item_template_info temp_item_with_num;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

		if (item_drop_list_with_att.empty())
		{
			msg.set_get_result(e_item_string_unkown);
			temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
			return;
		}
		temp_item_with_num.m_item_id = item_drop_list_with_att[0].m_item_id;
		temp_item_with_num.m_item_num = item_drop_list_with_att.size();
		final_list.push_back(temp_item_with_num);

		temp_player.cut_money(e_money_type_jewel, cost, e_server_log_cut_money_hope_item_one);

		temp_player.add_money_or_exp(e_money_type_hope_point, hope_point_get, e_server_log_add_money_hope_item);
	}
	break;
	case e_item_get_hope_item_typ_ten_cost:
	{
		int32 cost = GAMECONFIG->HopeItemTenTimesCost;
		int64 hope_point_get = GAMECONFIG->HopeItemTenTimesScore;
		if (!temp_player.can_cut_money(e_money_type_jewel, cost))
		{
			msg.set_get_result(e_item_string_jewel);
			temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
			return;
		}
		for (int32 i = 0; i < 10; ++i)
		{
			cur_hope_value = temp_player.get_unit_info(e_role_info_hope_value);
			if (temp_player.get_first_time_do_mgr().is_first_time_do(e_first_time_do_type_lucky_draw_item))
			{
				drop_template_id = GAMECONFIG->HopeItemGetFirstTimeDropID[player_class_type];
				temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_lucky_draw_item);
				is_first_time_get_hope = true;
			}
			else if (cur_hope_value >= max_hope_value)
			{
				drop_template_id = rare_drop_id;
				temp_player.set_unit_info(e_role_info_hope_value, 0);
				temp_player.send_info_one(e_role_info_hope_value);
			}
			else
			{
				drop_template_id = cdrop::get_drop_id(GAMECONFIG->HopeItemGetTenDropID[player_class_type]);
				if (drop_template_id != rare_drop_id)
				{
					temp_player.set_unit_info(e_role_info_hope_value, cur_hope_value + 1);
					temp_player.send_info_one(e_role_info_hope_value);
				}
				else
				{
					temp_player.set_unit_info(e_role_info_hope_value, 0);
					temp_player.send_info_one(e_role_info_hope_value);
				}
			}
			std::vector<s_item_template_info> item_drop_list_with_att;
			s_item_template_info temp_item_with_num;
			cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

			if (item_drop_list_with_att.empty())
			{
				msg.set_get_result(e_item_string_unkown);
				temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);
				return;
			}
			temp_item_with_num.m_item_id = item_drop_list_with_att[0].m_item_id;
			temp_item_with_num.m_item_num = item_drop_list_with_att.size();
			final_list.push_back(temp_item_with_num);
		}

		temp_player.cut_money(e_money_type_jewel, cost, e_server_log_cut_money_hope_item_ten);

		temp_player.add_money_or_exp(e_money_type_hope_point, hope_point_get, e_server_log_add_money_hope_item);
	}
	break;

	default:
		break;
	}

	int32 send_var_mail = 0;
	s_item_info mail_item_info_array[max_item_per_mail];

	std::vector<citem*> item_array;
	for (int32 i = 0; i < final_list.size(); ++i)
	{
		citem* temp_item = temp_player.get_item_set().create_item_by_template(e_server_log_add_item_luck_draw, 0, final_list[i].m_item_id, final_list[i].m_item_num, 0);
		if (temp_item)
		{
			item_array.push_back(temp_item);
		}
	}
	temp_player.get_item_set().put_in_bag(item_array, e_bag_type_none, e_item_mail_type_hope_item);

	for (int32 i = 0; i < final_list.size(); ++i)
	{
		msg.add_get_item_template_id(final_list[i].m_item_id);
		msg.add_get_item_num(final_list[i].m_item_num);
	}
	msg.set_send_mail_num(send_var_mail);
	temp_player.send_message_to_self(&msg, e_msgindex_s2c_get_hope_item_end);

	s_item_info item_data[chat_max_item];
	int32 item_num = 0;

	if (msg.get_result() == 0 && final_list.size() > 0)
	{
		bool if_notice = false;
		std::string item_names_str = "";

		for (int32 index = 0; index < final_list.size(); index++)
		{
			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, final_list[index].m_item_id);
			if (nullptr == item_template_ptr || item_template_ptr->item_color < faith::e_item_color_orange)
			{
				continue;
			}

			citem* temp_item = temp_player.get_item_set().create_item_by_template(e_server_log_add_item_luck_draw, 0, final_list[index].m_item_id, final_list[index].m_item_num, 0);
			if (temp_item == nullptr)
			{
				continue;
			}

			if_notice = true;
			item_names_str += " ";
			item_names_str += "%d";
			if (item_num >= 0 && item_num < chat_max_item)
			{
				item_data[item_num] = temp_item->get_item_inst();
				++item_num;
			}


			cs2ws_add_new_draw_record add_record_msg;
			add_record_msg.record_info.info_guid = guid_gen::make_guid(temp_player.get_unit_guid());
			add_record_msg.record_info.role_guid = temp_player.get_unit_guid();
			add_record_msg.record_info.item_template_id = final_list[index].m_item_id;
			add_record_msg.record_info.get_type = get_typ;
			memcpy(add_record_msg.record_info.role_name, temp_player.get_name(), max_name_size);
			add_record_msg.record_info.record_time = utility::get_tick_count();
			add_record_msg.record_info.record_type = e_luck_draw_record_type_hope_item;
			connection_mgr::getInstance().send_to_ws(&add_record_msg, sizeof(add_record_msg));
		}
		if (if_notice)
		{
			std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(GAMECONFIG->HopeNotice);
			std::vector<std::string> notice_str_params_vec;
			notice_str_params_vec.push_back(notice_str_id);
			notice_str_params_vec.push_back(temp_player.get_name());
			notice_str_params_vec.push_back(item_names_str);
			std::string notice_str = init_unit::implode(notice_str_params_vec);
			temp_player.get_chat_mgr().send_notice(GAMECONFIG->HopeNotice, notice_str, item_data, item_num - 1);
		}
	}
}

int32 item_set::put_item_into_bag_from_another_bag(const guid_64& item_guid)
{

	citem* temp_item = get_item(item_guid);
	if (temp_item == nullptr)
	{
		return e_item_string_no_item;
	}

	int32 temp_result = -1;
	int32 storage_full = 0;
	if (temp_item->get_data_info(e_item_info_container_type) == e_bag_type_bag)
	{

		if (put_in_bag(temp_item, e_bag_type_storage) == false)
		{
			return e_item_string_storage_full;
		}
	}
	else
	{
		if (put_in_bag(temp_item, e_bag_type_bag) == false)
		{
			return e_item_string_bag_full;
		}
	}
	return e_item_string_succeed;
}

int32 item_set::get_item_num(e_bag_type temp_bag_type)
{
	if (temp_bag_type <= e_bag_type_none || temp_bag_type >= e_bag_type_max)
	{
		return 0;
	}
	int32 item_num = 0;
	auto& guid_array = GET_BAG(temp_bag_type);
	for (auto& it : guid_array)
	{
		if (it.second.is_valid())
		{
			item_num++;
		}
	}
	return item_num;
}

int32 item_set::get_empty_slot_num(e_bag_type temp_bag_type)
{
	if (temp_bag_type <= e_bag_type_none || temp_bag_type >= e_bag_type_max)
	{
		return 0;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return 0;
	}
	auto& guid_array = GET_BAG(temp_bag_type);
	int32 bag_len = m_item_bag_length[temp_bag_type];

	switch (temp_bag_type)
	{
	case e_bag_type_bag:
	{
		bag_len = player_ref.get_logic_data(e_role_logic_info_bag_open);
		if (bag_len > MAX_ITEM_BAG_NUM)
		{
			bag_len = MAX_ITEM_BAG_NUM;
		}
	}
	break;
	case e_bag_type_storage:
	{
		bag_len = player_ref.get_logic_data(e_role_logic_info_storage_open);
		if (bag_len > max_item_storage_num)
		{
			bag_len = max_item_storage_num;
		}
	}
	break;
	case e_bag_type_equip_protect:
	{
		bag_len = player_ref.get_unit_info(e_role_info_protect_mark_slot_open_num);
		if (bag_len > max_proctect_mark_equip_slot_num)
		{
			bag_len = max_proctect_mark_equip_slot_num;
		}
	}
	break;
	case e_bag_type_equip_elemenet:
	{
		bag_len = player_ref.get_unit_info(e_role_info_element_heart_slot_open_num);
		if (bag_len > element_heart_equip_num)
		{
			bag_len = element_heart_equip_num;
		}
	}
	}
	int32 empty_slot_count = 0;
	for (int32 i = 0; i < bag_len; ++i)
	{
		auto it = guid_array.find(i);
		if (it == guid_array.end())
		{
			empty_slot_count++;
		}
	}
	return empty_slot_count;
}

int32 item_set::get_empty_slot(e_bag_type temp_bag_type)
{
	if (temp_bag_type <= e_bag_type_none || temp_bag_type >= e_bag_type_max)
	{
		return -1;
	}
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
	{
		return -1;
	}
	auto& guid_array = GET_BAG(temp_bag_type);
	int32 bag_len = m_item_bag_length[temp_bag_type];

	switch (temp_bag_type)
	{
	case e_bag_type_bag:
	{
		bag_len = player_ref.get_logic_data(e_role_logic_info_bag_open);
		if (bag_len > MAX_ITEM_BAG_NUM)
		{
			bag_len = MAX_ITEM_BAG_NUM;
		}
	}
	break;
	case e_bag_type_storage:
	{
		bag_len = player_ref.get_logic_data(e_role_logic_info_storage_open);
		if (bag_len > max_item_storage_num)
		{
			bag_len = max_item_storage_num;
		}
	}
	break;
	case e_bag_type_equip_protect:
	{
		bag_len = player_ref.get_unit_info(e_role_info_protect_mark_slot_open_num);
		if (bag_len > max_proctect_mark_equip_slot_num)
		{
			bag_len = max_proctect_mark_equip_slot_num;
		}
	}
	break;
	case e_bag_type_equip_elemenet:
	{
		bag_len = player_ref.get_unit_info(e_role_info_element_heart_slot_open_num);
		if (bag_len > element_heart_equip_num)
		{
			bag_len = element_heart_equip_num;
		}
	}
	break;
	}
	for (int32 i = 0; i < bag_len; ++i)
	{
		auto it = guid_array.find(i);
		if (it == guid_array.end())
		{
			return i;
		}
	}
	return -1;
}

bool item_set::is_empty_slot_enough(e_bag_type temp_bag_typ, int32 request_num)
{
	if (request_num <= 0)
	{
		return true;
	}
	if (get_empty_slot_num(temp_bag_typ) >= request_num)
	{
		return true;
	}
	return false;
}

int32 item_set::get_total_mount_grade()
{
	int32 return_value = 0;
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_mount)[i];
		citem* temp_mount_ptr = get_item(temp_guid);
		if (temp_mount_ptr != nullptr
			&& temp_mount_ptr->get_data_info(e_item_info_activate) > 0)
		{
			MountTemplate* mount_template_ptr = temp_mount_ptr->get_mount_template_ptr();
			if (mount_template_ptr != nullptr
				&& mount_template_ptr->Type != mount_type_illusion)
			{
				return_value += mount_template_ptr->ClassNow;
			}
		}
	}
	return return_value;
}

int32 item_set::get_total_mount_activate_num()
{
	int32 return_value = 0;
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_mount)[i];
		citem* temp_mount_ptr = get_item(temp_guid);
		if (temp_mount_ptr != nullptr)
		{
			if (temp_mount_ptr->get_data_info(e_item_info_activate) > 0)
			{
				MountTemplate* mount_template_ptr = temp_mount_ptr->get_mount_template_ptr();
				if (mount_template_ptr != nullptr)
				{
					if (mount_template_ptr->Type != mount_type_illusion)
					{
						return_value++;
					}
				}
			}
		}
	}
	return return_value;
}

int32 item_set::get_highest_level_activate_mount_template_id()
{
	int32 return_value = 0;
	int32 level_count = 0;
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_mount)[i];
		citem* temp_mount_ptr = get_item(temp_guid);
		if (temp_mount_ptr == nullptr)
		{
			continue;
		}
		if (temp_mount_ptr->get_data_info(e_item_info_activate) > 0)
		{
			MountTemplate* mount_template_ptr = temp_mount_ptr->get_mount_template_ptr();
			if (mount_template_ptr != nullptr)
			{
				if (mount_template_ptr->Type != mount_type_illusion)
				{
					return_value = temp_mount_ptr->get_item_template_id();
					return return_value;
				}
			}

		}
	}
	return return_value;
}

int32 item_set::get_highest_level_activate_wing_template_id()
{
	int32 return_value = 0;
	int32 level_count = 0;
	for (int32 index = 0; index < MAX_WING_ELEMENTS_NUM; index++)
	{
		if (GET_BAG(e_bag_type_wing)[index].is_valid())
		{
			citem* temp_wing_ptr = get_item(GET_BAG(e_bag_type_wing)[index]);
			if (temp_wing_ptr != nullptr)
			{
				if (temp_wing_ptr->get_data_info(e_item_info_activate) <= 0)
				{
					continue;
				}
				WingTemplate* wing_ptr = temp_wing_ptr->get_wing_template_ptr();
				if (wing_ptr != nullptr)
				{
					if (e_item_wing_type_normal == wing_ptr->Type)
					{
						if (level_count <= temp_wing_ptr->get_data_info(e_item_info_upgrade_count))
						{
							level_count = temp_wing_ptr->get_data_info(e_item_info_upgrade_count);
							return_value = temp_wing_ptr->get_data_info(e_item_info_info_id);
						}
					}
				}
			}
		}
	}
	return return_value;
}


int32 item_set::get_wing_and_total_feather_grade_num()
{
	int32 return_value = 0;
	citem* wing_ptr = get_cur_level_wing();
	if (wing_ptr != nullptr)
	{
		WingTemplate* wing_template_ptr = wing_ptr->get_wing_template_ptr();
		if (wing_template_ptr != nullptr)
		{
			return_value += wing_template_ptr->UpgradeNow;
		}

		std::vector<citem*> temp_feather_array = get_wing_feather();
		for (int32 i = 0; i < temp_feather_array.size(); ++i)
		{
			citem* temp_wing_feather = temp_feather_array[i];
			if (temp_wing_feather != nullptr)
			{
				ItemTemplate* item_template_ptr = temp_wing_feather->get_item_info_ptr();
				if (item_template_ptr != nullptr)
				{
					int32 real_id = item_template_ptr->logic_id + temp_wing_feather->get_data_info(e_item_info_upgrade_count);
					FeatherTemplate* feather_template_ptr = GET_TEMPLATE(FeatherTemplate, real_id);
					if (feather_template_ptr != nullptr)
					{
						return_value += feather_template_ptr->UpgradeNow;
					}
				}
			}
		}
	}
	return return_value;
}

int32 item_set::get_wing_and_total_feather_star_num()
{
	int32 return_value = 0;
	citem* wing_ptr = get_cur_level_wing();
	if (wing_ptr != nullptr)
	{
		/*WingTemplate* wing_template_ptr = wing_ptr->get_wing_template_ptr();
		if (wing_template_ptr != nullptr)
		{
			return_value += (wing_template_ptr->UpgradeNow - 1) * wing_template_ptr->WingAddStarMax + wing_template_ptr->StarNow;
		}*/

		std::vector<citem*> temp_feather_array = get_all_wing();
		for (int32 i = 0; i < temp_feather_array.size(); ++i)
		{
			citem* temp_wing_feather = temp_feather_array[i];
			if (temp_wing_feather != nullptr)
			{
				ItemTemplate* item_template_ptr = temp_wing_feather->get_item_info_ptr();
				if (item_template_ptr != nullptr)
				{
					int32 real_id = item_template_ptr->logic_id + temp_wing_feather->get_data_info(e_item_info_upgrade_count);
					WingTemplate* wing_ptr = GET_TEMPLATE(WingTemplate, real_id);
					if (wing_ptr != nullptr)
					{
						if (wing_ptr->Type == e_item_wing_type_normal)
						{
							return_value += wing_ptr->StarNow;
						}
						else if (wing_ptr->Type != e_item_wing_type_special_santo)
						{
							return_value += (wing_ptr->UpgradeNow - 1) * wing_ptr->WingAddStarMax + wing_ptr->StarNow;
						}
					}
				}
			}
		}
	}
	return return_value;
}

int32 item_set::calcu_beast_sprite_fighting_power(int32 bag_type)
{
	int32 return_value = 0;
	player& self_ref = unit_man::get_player(m_unit_array_index);
	if (false == self_ref.is_valid())
	{
		return return_value;
	}


	switch (bag_type)
	{
	case faith::e_bag_type_mount_beast_spirit:
	case faith::e_bag_type_wing_beast_spirit:
	case faith::e_bag_type_spirit_beast_spirit:
	{
		auto& guid_array = GET_BAG(bag_type);
		const double* att_array = self_ref.m_pawn_att.get_attack_att_all();

		for (auto& it : guid_array)
		{
			citem* item_ptr = get_item(it.second);
			if (item_ptr == nullptr)
			{
				continue;
			}
			BeastSpiritTemplate* beast_template_ptr = item_ptr->get_beast_spirit_template_ptr();
			if (beast_template_ptr == nullptr)
			{
				continue;
			}
			int32 upgrade_num = item_ptr->get_data_info(e_item_info_upgrade_count);
			return_value += init_unit::calcu_fighting_power_by_att_array(beast_template_ptr->GetAttributes, att_array, upgrade_num);
		}

	}
	default:
		break;
	}

	return return_value;
}
guid_64 item_set::get_target_slot_guid(e_bag_type target_bag, int32 target_slot)
{
	if (target_bag <= e_bag_type_none || target_bag >= e_bag_type_max)
	{
		return guid_64();
	}
	auto& guid_array = GET_BAG(target_bag);
	auto it = guid_array.find(target_slot);
	if (it == guid_array.end())
	{
		return guid_64();
	}
	return it->second;
}
void item_set::set_target_slot_guid(e_bag_type target_bag, int32 target_slot, guid_64 new_guid)
{
	ZoneScoped;
	if (target_bag <= e_bag_type_none || target_bag >= e_bag_type_max)
	{
		//CONSOLE_ERROR("item_set::set_target_slot_guid target_bag:{}", (int32)target_bag);
		return;
	}
	auto& guid_array = GET_BAG(target_bag);
	int32 bag_len = m_item_bag_length[target_bag];
	if (target_slot < 0 || target_slot >= bag_len)
	{
		CONSOLE_ERROR("item_set::set_target_slot_guid target_slot:{}", target_slot);
		return;
	}
	guid_array[target_slot] = new_guid;
}

void item_set::clear_target_slot(e_bag_type target_bag, int32 target_slot)
{
	ZoneScoped;
	if (target_bag <= e_bag_type_none || target_bag >= e_bag_type_max)
	{
		//CONSOLE_ERROR("item_set::clear_target_slot target_bag:{}", (int32)target_bag);
		return;
	}
	auto& guid_array = GET_BAG(target_bag);
	auto it = guid_array.find(target_slot);
	if (it != guid_array.end())
	{
		guid_array.erase(it);
	}
}

int32 item_set::get_equip_num_by_strength_limit(int32 limit_level)
{
	int32 item_num = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_magic_2; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		if (limit_level <= temp_equip_ptr->get_data_info(e_item_info_upgrade_count))
		{
			item_num++;
		}
	}
	return item_num;
}

int32 item_set::get_equip_num_by_addition_limie(int32 limit_level)
{
	int32 item_num = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_magic_2; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		if (limit_level <= temp_equip_ptr->get_data_info(e_item_info_add_on))
		{
			item_num++;
		}
	}
	return item_num;
}
int32 item_set::get_equip_num_by_awaken_limit(int32 limit_level)
{
	int32 item_num = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_magic_2; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		if (limit_level <= temp_equip_ptr->get_data_info(e_item_info_wing_exp))
		{
			item_num++;
		}
	}
	return item_num;

}

int32 item_set::get_equip_num_by_succinct_limit(int32 limit_level)
{
	int32 item_num = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_magic_2; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		if (limit_level <= temp_equip_ptr->get_data_info(e_item_info_succinct_level))
		{
			item_num++;
		}
	}
	return item_num;
}

void item_set::get_mount_grade_and_level_by_order_num(int32 order_num, int32& grade, int32& level)
{
	for (int32 i = 0; i < MAX_MOUNT_NUM; ++i)
	{
		guid_64 temp_guid = GET_BAG(e_bag_type_mount)[i];
		citem* temp_mount_ptr = get_item(temp_guid);
		if (temp_mount_ptr != nullptr
			&& temp_mount_ptr->get_data_info(e_item_info_activate) > 0)
		{
			MountTemplate* mount_template_ptr = temp_mount_ptr->get_mount_template_ptr();
			if (mount_template_ptr != nullptr
				&& mount_template_ptr->MountOrderNum == order_num)
			{
				grade = mount_template_ptr->ClassNow;
				level = mount_template_ptr->GradeNow;
			}
		}
	}
}

void item_set::get_cur_wing_grade_and_level(int32& grade, int32& level)
{
	citem* temp_wing_ptr = get_cur_level_wing();
	if (nullptr == temp_wing_ptr)
	{
		return;
	}
	int32 real_id = temp_wing_ptr->get_item_info_ptr()->logic_id + temp_wing_ptr->get_data_info(e_item_info_upgrade_count);
	WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, real_id);
	if (nullptr == wing_template_ptr)
	{
		return;
	}
	grade = wing_template_ptr->UpgradeNow;
	level = wing_template_ptr->StarNow;
}

int32 item_set::add_money_by_card(const std::vector<int32>& money_card, const int32 money_card_num, std::vector<int32>& get_money_list)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (false == temp_player.is_valid())
	{
		return 0;
	}

	const int32 money_card_size = money_card.size();
	if (0 != money_card_size % 2 || 0 == money_card_size)
	{
		return 0;
	}
	for (int32 money_card_index = 0; money_card_index < money_card_size; money_card_index += 2)
	{
		const int32 money_id = money_card[money_card_index];
		int64 money_num = money_card[money_card_index + 1] * money_card_num;
		temp_player.add_money_or_exp((e_money_type)money_id, money_num, e_server_log_add_money_money_card);
		if (money_id == e_money_type_jewel)
		{
			int32 cur_real_recharge_jewel_num = temp_player.get_unit_info(e_role_info_real_recharge_jewel_num);
			if (cur_real_recharge_jewel_num == 0)
			{
				int32 cur_time = time_helper::get_today_zero_time_info().second;
				if (cur_time > 0)
				{
					temp_player.set_time_data(e_time_type_frist_recharge_time, cur_time);
					temp_player.get_welfare_mgr().get_first_recharge_welfare_by_id();
				}
			}
			temp_player.set_unit_info(e_role_info_real_recharge_jewel_num, cur_real_recharge_jewel_num + money_num);

			//int32 cur_daily_recharge_num = temp_player.get_unit_info(e_role_info_daily_recharge_num);
			//temp_player.set_unit_info(e_role_info_daily_recharge_num, cur_daily_recharge_num + money_num);

			temp_player.get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_recharge_rmb, temp_player.get_unit_info(e_role_info_real_recharge_jewel_num));

			//temp_player.send_info_one(e_role_info_daily_recharge_num);
			temp_player.send_info_one(e_role_info_real_recharge_jewel_num);
		}
		get_money_list.push_back(money_id);
		get_money_list.push_back(money_num);
	}

	return money_card_num;
}


int32 item_set::open_fruit_bag(const PropBasicAttributeTemplate* prop_template, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id)
{

	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false
		|| temp_player.m_cur_map_template_ptr == nullptr)
	{
		return 0;
	}

	int32 has_prop_num = 0;
	for (; has_prop_num < package_num; has_prop_num++)
	{
		int32 len = prop_template->RoleAttributeChange.size();
		if (len > 0 && len % e_att_one_max == 0)
		{
			for (int32 i = 0; i < len; i += e_att_one_max)
			{
				int32 att_id = prop_template->RoleAttributeChange[i + e_att_one_att_id];
				int32 att_value = prop_template->RoleAttributeChange[i + e_att_one_value] * 1;
				temp_player.add_att_permanent(att_id, att_value);
			}
		}
	}

	return has_prop_num;
}

int32 item_set::open_package(const e_prop_sub_type item_sub_type, const PropBasicAttributeTemplate* prop_template, const int32 package_num, e_error_code& item_use_end_result, std::vector<int32>& get_money_list, std::vector<s_item_template_info>& get_item_list, int32 notice_id, ItemTemplate* parent_item_ptr)
{
	int32 has_opened_package_num = 0;
	if (nullptr == prop_template)
	{
		return has_opened_package_num;
	}
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false
		|| temp_player.m_cur_map_template_ptr == nullptr)
	{
		return has_opened_package_num;
	}
	switch (item_sub_type)
	{
	case e_prop_sub_type_fruit:
	{
		has_opened_package_num = open_fruit_bag(prop_template, package_num, item_use_end_result, get_item_list, notice_id);
		break;
	}
	case e_prop_sub_type_gife:
	{
		has_opened_package_num = open_package_bag(prop_template->ItemGet, package_num, item_use_end_result, get_item_list, notice_id, parent_item_ptr);
		break;
	}
	case e_prop_sub_type_element_package:
	{
		has_opened_package_num = open_package_elementbag(prop_template->ItemGet, package_num, item_use_end_result, get_item_list, notice_id);
		break;
	}
	case e_prop_sub_type_money:
	{
		has_opened_package_num = add_money_by_card(prop_template->MoneyChange, package_num, get_money_list);
	}
	default:
		break;
	}
	return has_opened_package_num;
}

int32 item_set::open_package_bag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id, ItemTemplate* parent_item_ptr)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (false == temp_player.is_valid())
	{
		item_use_end_result = e_error_code_item_invalid_player_state;
		return 0;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		item_use_end_result = e_error_code_item_invalid_player_state;
		return 0;
	}
	std::vector<s_item_info> filter_items_array;
	s_item_info item_data[chat_max_item];
	std::string item_names_str;
	item_names_str.reserve(100);
	int32 package_index = 0;
	int32 item_data_num = 0;
	for (; package_index < package_num; package_index++)
	{
		std::vector<s_item_template_info> drop_list;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, drop_list, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

		if (false == can_put_items(drop_list, e_bag_type_bag))
		{
			item_use_end_result = e_error_code_item_bag_full;
			return package_index;
		}
		std::vector<citem*> items_ptr_array = create_items(e_server_log_add_item_open_package_bag, drop_template_id, drop_list);
		if (item_data_num < chat_max_item)
		{
			item_data[item_data_num] = filter_items_with_notice(items_ptr_array, item_names_str, notice_id);
			if (item_data[item_data_num].item_guid.is_valid())
			{
				item_data_num++;
			}
		}
		if (0 < notice_id && chat_max_item <= item_data_num)
		{
			player& temp_player = unit_man::get_player(m_unit_array_index);
			if (temp_player.is_valid() == false)
			{
				item_use_end_result = e_error_code_item_invalid_item_template;
				return package_index;
			}

			std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
			std::vector<std::string> notice_str_params_vec;
			notice_str_params_vec.push_back(notice_str_id);
			notice_str_params_vec.push_back(temp_player.get_name());
			if (region_template_ptr->RegionCode == e_version_region_type_en)
			{
				notice_str_params_vec.push_back(item_names_str);
				if (nullptr != parent_item_ptr)
				{
					notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(parent_item_ptr->ItemName));
				}
			}
			else
			{
				if (nullptr != parent_item_ptr)
				{
					notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(parent_item_ptr->ItemName));
				}
				notice_str_params_vec.push_back(item_names_str);
			}
			std::string notice_str = init_unit::implode(notice_str_params_vec);
			temp_player.get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_data_num - 1);
			item_data_num = 0;
			item_names_str.clear();
			memset(item_data, 0, sizeof(item_data));
		}
		put_in_bag(items_ptr_array);

		for (int32 result_items_list_index = 0; result_items_list_index < drop_list.size(); result_items_list_index += 4)
		{
			const int32 item_template_id = drop_list[result_items_list_index].m_item_id;
			const int32 item_num = drop_list[result_items_list_index].m_item_num;
			const int32 item_is_lock = drop_list[result_items_list_index].m_lock;
			get_item_list.push_back({ item_template_id ,item_num ,item_is_lock });
		}
	}

	if (0 < notice_id && 0 < item_data_num)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			item_use_end_result = e_error_code_item_invalid_item_template;
			return package_index;
		}

		std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(temp_player.get_name());
		if (region_template_ptr->RegionCode == e_version_region_type_en)
		{
			notice_str_params_vec.push_back(item_names_str);
			if (nullptr != parent_item_ptr)
			{
				notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(parent_item_ptr->ItemName));
			}
		}
		else
		{
			if (nullptr != parent_item_ptr)
			{
				notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(parent_item_ptr->ItemName));
			}
			notice_str_params_vec.push_back(item_names_str);
		}
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		temp_player.get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_data_num - 1);
		/*s_chat_notice_info notice_data;
		notice_data.item_data = filter_items_array;
		notice_data.item_name = filter_item_name_array;
		temp_player.get_chat_mgr().send_notice_new(notice_id, notice_data);*/
	}

	item_use_end_result = e_error_code_success;
	return package_index;
}

int32 item_set::open_choose_package_bag(const int32 logic_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id, int32 item_slot, ItemTemplate* parent_item_ptr)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (false == temp_player.is_valid())
	{
		item_use_end_result = e_error_code_item_invalid_player_state;
		return 0;
	}
	RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
	if (region_template_ptr == nullptr)
	{
		item_use_end_result = e_error_code_item_invalid_player_state;
		return 0;
	}
	PropBasicAttributeTemplate* line_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, logic_id);
	if (nullptr == line_ptr)
	{
		item_use_end_result = e_error_code_item_invalid_item_template;
		return 0;
	}

	int32 choose_item_index = 0;
	int32 choose_item_num = 0;
	for (; choose_item_index < line_ptr->ChooseItem.size(); choose_item_index += 2)
	{
		if (line_ptr->ChooseItem[choose_item_index] == item_slot)
		{
			choose_item_num = line_ptr->ChooseItem[choose_item_index + 1];
			break;
		}
	}
	if (0 == item_slot || 0 == choose_item_num)
	{
		item_use_end_result = e_error_code_item_invalid_item_template;
		return 0;
	}
	std::vector<s_item_info> filter_items_array;
	s_item_info item_data[chat_max_item];
	std::string item_names_str;
	item_names_str.reserve(100);
	int32 package_index = 0;
	int32 item_data_num = 0;
	int32 has_opened_package_num = 0;
	for (; package_index < package_num; package_index++)
	{
		std::vector<int32> result_items_list;
		ItemTemplate* line_ptr = GET_TEMPLATE(ItemTemplate, item_slot);
		if (nullptr == line_ptr)
		{
			item_use_end_result = e_error_code_item_invalid_item_template;
			return 0;
		}
		result_items_list.push_back(item_slot);
		result_items_list.push_back(choose_item_num);
		result_items_list.push_back(line_ptr->locked);
		result_items_list.push_back(0);

		std::vector<s_item_template_info> items_template_info_array = get_item_template_info_array(result_items_list, e_item_transform_type_four);
		if (false == can_put_items(items_template_info_array, e_bag_type_bag))
		{
			item_use_end_result = e_error_code_item_bag_full;
			return package_index;
		}
		std::vector<citem*> items_ptr_array = create_items(e_server_log_add_item_open_package_bag, logic_id, items_template_info_array);
		if (item_data_num < chat_max_item)
		{
			item_data[item_data_num] = filter_items_with_notice(items_ptr_array, item_names_str, notice_id);
			if (item_data[item_data_num].item_guid.is_valid())
			{
				item_data_num++;
			}
		}

		put_in_bag(items_ptr_array);
		has_opened_package_num++;
		//for (int32 result_items_list_index = 0; result_items_list_index < result_items_list_size; result_items_list_index += 4)
		{
			const int32 item_template_id = result_items_list[0];
			const int32 item_num = result_items_list[1];
			const int32 item_is_lock = result_items_list[2];
			get_item_list.push_back({ item_template_id ,item_num ,item_is_lock });
		}
	}

	if (0 < notice_id && 0 < item_data_num)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (temp_player.is_valid() == false)
		{
			item_use_end_result = e_error_code_item_invalid_item_template;
			return package_index;
		}

		std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(temp_player.get_name());
		if (region_template_ptr->RegionCode == e_version_region_type_en)
		{
			notice_str_params_vec.push_back(item_names_str);
			if (nullptr != parent_item_ptr)
			{
				notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(parent_item_ptr->ItemName));
			}
		}
		else
		{
			if (nullptr != parent_item_ptr)
			{
				notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(parent_item_ptr->ItemName));
			}
			notice_str_params_vec.push_back(item_names_str);
		}
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		temp_player.get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_data_num - 1);
		/*s_chat_notice_info notice_data;
		notice_data.item_data = filter_items_array;
		notice_data.item_name = filter_item_name_array;
		temp_player.get_chat_mgr().send_notice_new(notice_id, notice_data);*/
	}

	item_use_end_result = e_error_code_success;
	return has_opened_package_num;
}

int32 item_set::open_package_elementbag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, int32 notice_id)
{
	int32 has_opened_package_num = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return has_opened_package_num;
	}
	s_chat_notice_info notice_data;
	std::vector<citem*> item_array;
	for (int32 package_index = 0; package_index < package_num; package_index++)
	{
		std::vector<s_item_template_info> item_drop_list_with_att;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

		std::vector<s_item_template_info> items_with_num_list;
		cdrop::merge_drop_bag(item_drop_list_with_att, items_with_num_list);
		const int32 items_list_size = items_with_num_list.size();

		for (int32 items_list_index = 0; items_list_index < items_list_size; items_list_index++)
		{
			const s_item_template_info& item_drop = items_with_num_list[items_list_index];
			const int32 item_template_id = item_drop.m_item_id;
			const int32 item_num = item_drop.m_item_num;
			const int32 item_const_att_lev = 0;

			citem* new_item = create_item_by_template(e_server_log_add_item_item_use, 0, item_template_id, item_num, 0, 0, faith::e_bag_type_none, item_const_att_lev);
			if (nullptr == new_item)
			{
				item_use_end_result = e_error_code_item_invalid_item_template;
				return has_opened_package_num;
			}
			get_item_list.push_back({ item_template_id ,item_num ,new_item->get_data_info(e_item_info_locked) });
			if (new_item->get_item_color() >= e_item_color_purple)
			{
				notice_data.item_data.push_back(new_item->get_item_inst());
			}
			item_array.push_back(new_item);
		}
		has_opened_package_num++;
	}
	put_in_bag(item_array);
	item_use_end_result = e_error_code_success;

	if (notice_id > 0)
	{
		if (notice_data.item_data.size() > 0)
		{
			player& m_player_ref = unit_man::get_player(m_unit_array_index);
			if (m_player_ref.is_valid())
			{
				m_player_ref.get_chat_mgr().send_notice_new(notice_id, notice_data);
			}
		}
	}

	return has_opened_package_num;
}

int32 item_set::open_package_wingbag(const int32 drop_template_id, item_proto_item_use_end& item_use_end_msg)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return 0;
	}
	std::string add_item_cause_id;
	std::stringstream add_item_cause_id_stream;
	add_item_cause_id_stream << int32(e_server_log_add_item_item_use);
	add_item_cause_id_stream >> add_item_cause_id;

	std::vector<s_item_template_info> item_drop_list_with_att;
	cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

	const int32 items_list_size = item_drop_list_with_att.size();
	if (1 != items_list_size)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}

	const int32 item_template_id = item_drop_list_with_att[0].m_item_id;
	const int32 item_num = 1;

	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
	if (nullptr == item_template_ptr)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}
	if (item_template_ptr->sub_type == e_wing_sub_type_illusion)
	{
		const int32 item_wing_illustion_template_id = item_template_ptr->logic_id;
		item_use_end_msg.set_illusion_unlock_teamplate_id(item_wing_illustion_template_id);
		WingTemplate* item_wing_illustion_template_ptr = GET_TEMPLATE(WingTemplate, item_wing_illustion_template_id);
		if (nullptr == item_wing_illustion_template_ptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}

		if (item_wing_illustion_template_ptr->PreWingRequirement.size() <= 0)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
		const int32 item_wing_origin_template_id = item_wing_illustion_template_ptr->PreWingRequirement[0];
		const citem* item_wing_origin_template_ptr = get_item_by_template_id(item_wing_origin_template_id);
		if (nullptr == item_wing_origin_template_ptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
		if (0 == item_wing_origin_template_ptr->get_data_info(e_item_info_activate))
		{
			item_use_end_msg.set_result(e_error_code_item_cant_unlock_wing_illusion);
			return 0;
		}
		/*				if (item_wing_illustion_template_ptr->IllusionOpenClass > (item_wing_origin_template_ptr->get_data_info(e_item_info_upgrade_count) + 1))
		{
		item_use_end_msg.set_result(e_error_code_item_cant_unlock_illusion);
		temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
		return false;
		}*/
	}
	int32 wing_illusion_item_template_id = item_template_id;
	citem* new_wing_item = get_item_by_template_id(wing_illusion_item_template_id);
	if (nullptr == new_wing_item)
	{
		new_wing_item = create_item_by_template(e_server_log_add_item_wing_iilusion, 0, wing_illusion_item_template_id, 1, 0);
		if (new_wing_item == nullptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
	}
	if (1 == new_wing_item->get_data_info(e_item_info_activate))
	{
		item_use_end_msg.set_result(e_error_code_item_has_unlocked);
		return 0;
	}

	set_wing_att(false);
	new_wing_item->set_data_info(e_item_info_activate, 1);
	set_wing_att(true);
	send_item_one(new_wing_item);
	show_this_wing(new_wing_item, -1);

	item_use_end_msg.set_result(e_error_code_success);
	return 1;
}

faith::int32 item_set::open_package_mountbag(const int32 drop_template_id, item_proto_item_use_end& item_use_end_msg, int32 over_time)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return 0;
	}
	std::string add_item_cause_id;
	std::stringstream add_item_cause_id_stream;
	add_item_cause_id_stream << int32(e_server_log_add_item_item_use);
	add_item_cause_id_stream >> add_item_cause_id;

	std::vector<s_item_template_info> item_drop_list_with_att;
	cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));


	const int32 items_list_size = item_drop_list_with_att.size();
	if (1 != items_list_size)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}

	const int32 item_template_id = item_drop_list_with_att[0].m_item_id;
	const int32 item_num = 1;

	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
	if (nullptr == item_template_ptr)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}

	bool is_mount_illusion = false;
	citem* item_mount_origin_ptr = nullptr;
	if (item_template_ptr->sub_type == e_mount_sub_type_illusion)
	{
		is_mount_illusion = true;
		const int32 item_mount_illustion_template_id = item_template_ptr->logic_id;
		item_use_end_msg.set_illusion_unlock_teamplate_id(item_mount_illustion_template_id);
		MountTemplate* item_mount_illustion_template_ptr = GET_TEMPLATE(MountTemplate, item_mount_illustion_template_id);
		if (nullptr == item_mount_illustion_template_ptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}

		if (item_mount_illustion_template_ptr->PreMountRequirement.size() <= 1)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
		const int32 item_mount_origin_template_id = item_mount_illustion_template_ptr->PreMountRequirement[0];
		item_mount_origin_ptr = get_item_by_template_id(item_mount_origin_template_id);
		if (nullptr == item_mount_origin_ptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
		if (0 == item_mount_origin_ptr->get_data_info(e_item_info_activate))
		{
			item_use_end_msg.set_result(e_error_code_item_cant_unlock_mount_illusion);
			return 0;
		}
		//if (item_mount_illustion_template_ptr->PreMountRequirement[1] > item_mount_origin_ptr->get_data_info(e_item_info_upgrade_count))
		//{
		//	item_use_end_msg.set_result(e_error_code_item_cant_unlock_illusion);
		//	temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
		//	return false;
		//}
	}

	citem* new_mount_item = get_item_by_template_id(item_template_id);
	if (nullptr == new_mount_item)
	{
		new_mount_item = create_item_by_template(e_server_log_add_item_mount_illusion, 0, item_template_id, 1, 0);
		if (new_mount_item == nullptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
	}
	if (1 == new_mount_item->get_data_info(e_item_info_activate))
	{
		item_use_end_msg.set_result(e_error_code_item_has_unlocked);
		temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
		return 0;
	}
	new_mount_item->set_data_info(e_item_info_activate, 1);
	new_mount_item->set_data_info(e_item_info_over_time, over_time);
	MountTemplate* new_mount_template_ptr = new_mount_item->get_mount_template_ptr();
	if (nullptr == new_mount_template_ptr)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}
	item_change_att(new_mount_template_ptr->AttArray, 1, true);
	send_item_one(new_mount_item);
	temp_player.refresh_service_goal(e_service_goal_type_mount_num);
	int32 empty_slot = get_use_time_item_empty_slot();
	if (empty_slot < 0 || empty_slot >= MAX_MOUNT_NUM)
	{
		return false;
	}
	m_used_has_time_item_array[empty_slot] = new_mount_item->get_item_guid();
	if (true == is_mount_illusion)
	{
		const int32 illusion_slot = get_mount_illusion_slot(item_template_ptr->logic_id);
		if (-1 == illusion_slot)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
		show_this_mount(item_mount_origin_ptr, illusion_slot);
	}
	else
	{
		show_this_mount(new_mount_item, -1);
	}
	temp_player.refresh_service_goal(e_service_goal_type_mount_num);
	item_use_end_msg.set_result(e_error_code_success);
	return 1;
}

int32 item_set::open_package_time_limit_prop(const int32 drop_template_id, item_proto_item_use_end& item_use_end_msg, int32 over_time)
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return 0;
	}

	std::string add_item_cause_id;
	std::stringstream add_item_cause_id_stream;
	add_item_cause_id_stream << int32(e_server_log_add_item_item_use);
	add_item_cause_id_stream >> add_item_cause_id;

	std::vector<s_item_template_info> item_drop_list_with_att;
	cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));

	const int32 items_list_size = item_drop_list_with_att.size();
	if (1 != items_list_size)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}

	const int32 item_template_id = item_drop_list_with_att[0].m_item_id;
	const int32 item_num = 1;

	ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_template_id);
	if (nullptr == item_template_ptr)
	{
		item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
		return 0;
	}

	citem* item_ptr = get_item_by_template_id(item_template_id);
	if (nullptr == item_ptr)
	{
		item_ptr = create_item_by_template(e_server_log_add_item_mount_illusion, 0, item_template_id, 1, 0);
		if (item_ptr == nullptr)
		{
			item_use_end_msg.set_result(e_error_code_item_invalid_item_template);
			return 0;
		}
	}

	if (1 == item_ptr->get_data_info(e_item_info_activate) && 0 == item_ptr->get_data_info(e_item_info_over_time))
	{
		item_use_end_msg.set_result(e_error_code_item_has_unlocked);
		temp_player.send_message_to_self(&item_use_end_msg, e_msgindex_s2c_item_use_end);
		return 0;
	}

	item_ptr->set_data_info(e_item_info_activate, 1);

	bool is_equip = true;
	int32 cur_second_time = time_helper::get_cur_time_new().second;
	if (cur_second_time >= item_ptr->get_data_info(e_item_info_over_time))
	{
		if (over_time == 0)
		{
			item_ptr->set_data_info(e_item_info_over_time, 0);
		}
		else
		{
			if (item_ptr->get_data_info(e_item_info_over_time) < 0)
			{
				over_time = item_ptr->get_data_info(e_item_info_over_time) - over_time;
				item_ptr->set_data_info(e_item_info_over_time, over_time);
			}
			else
			{
				over_time = cur_second_time + over_time;
				item_ptr->set_data_info(e_item_info_over_time, over_time);
			}
		}
	}
	else
	{
		if (over_time == 0)
		{
			item_ptr->set_data_info(e_item_info_over_time, 0);
		}
		else
		{
			over_time = item_ptr->get_data_info(e_item_info_over_time) + over_time;
			item_ptr->set_data_info(e_item_info_over_time, over_time);
		}
		is_equip = false;
	}
	send_item_one(item_ptr);

	int32  in_which_slot = item_ptr->get_data_info(e_item_info_slot);
	if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_mount && is_equip)
	{
		MountTemplate* mount_template_ptr = item_ptr->get_mount_template_ptr();
		if (mount_template_ptr != nullptr)
		{
			item_change_att(mount_template_ptr->AttArray, 1, true);
			show_this_mount(item_ptr, -1);
		}
	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_wing && is_equip)
	{
		WingTemplate* wing_template_ptr = item_ptr->get_wing_template_ptr();
		if (wing_template_ptr != nullptr)
		{
			item_change_att(wing_template_ptr->AttArray, 1, true);
			show_this_wing(item_ptr, -1);
		}
	}
	else if (item_ptr->get_data_info(e_item_info_container_type) == e_bag_type_equip_fasion && is_equip)
	{
		equip_on(item_ptr->get_item_guid(), in_which_slot, false, true);
	}

	if (item_template_ptr->item_type != e_item_type_fashion)
	{
		int32 empty_slot = get_use_time_item_empty_slot();
		if (empty_slot < 0 || empty_slot >= MAX_TIME_LIMIT_ITEM_NUM)
		{
			return false;
		}
		m_used_has_time_item_array[empty_slot] = item_ptr->get_item_guid();
	}

	item_use_end_msg.set_result(e_error_code_success);
	return 1;
}

int32 faith::item_set::open_const_att_equip_bag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list, citem*& end_item)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		return 0;
	}
	int32 cur_class = player_ref.get_unit_info(e_role_info_class_type);
	int32 has_opened_package_num = 0;
	for (int32 package_index = 0; package_index < package_num; package_index++)
	{
		std::vector<s_item_template_info> item_drop_list_with_att;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, player_ref.get_unit_info(e_role_info_class_type), player_ref.get_unit_info(e_role_info_exp_level));

		std::vector<s_item_template_info> items_with_num_list;
		cdrop::merge_drop_bag(item_drop_list_with_att, items_with_num_list, cur_class);

		const int32 items_list_size = items_with_num_list.size();
		if (false == is_empty_slot_enough(e_bag_type_bag, items_list_size))
		{
			item_use_end_result = e_error_code_item_bag_full;
			return has_opened_package_num;
		}

		for (int32 items_list_index = 0; items_list_index < items_list_size; items_list_index++)
		{
			const s_item_template_info& item_drop = items_with_num_list[items_list_index];
			const int32 item_template_id = item_drop.m_item_id;
			const int32 item_num = item_drop.m_item_num;
			const int32 item_const_att_lev = 0;

			citem* new_item = create_item_by_template(e_server_log_add_item_item_use, 0, item_template_id, item_num, 1, 0, faith::e_bag_type_none, item_const_att_lev);
			if (nullptr == new_item)
			{
				item_use_end_result = e_error_code_item_invalid_item_template;
				return has_opened_package_num;
			}
			end_item = new_item;

			has_opened_package_num++;
			get_item_list.push_back({ item_template_id , item_num, 0});
		}
	}
	item_use_end_result = e_error_code_success;
	return has_opened_package_num;
}

int32 item_set::open_rand_equip_package_bag(const int32 drop_template_id, const int32 package_num, e_error_code& item_use_end_result, std::vector<s_item_template_info>& get_item_list)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		return 0;
	}
	int32 notice_id = 0;
	s_chat_notice_info notice_data;
	int32 has_opened_package_num = 0;
	for (int32 package_index = 0; package_index < package_num; package_index++)
	{
		std::vector<s_item_template_info> item_drop_list_with_att;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, player_ref.get_unit_info(e_role_info_class_type), player_ref.get_unit_info(e_role_info_exp_level));

		std::vector<s_item_template_info> items_with_num_list;
		cdrop::merge_drop_bag(item_drop_list_with_att, items_with_num_list);

		const int32 items_list_size = items_with_num_list.size();
		if (false == is_empty_slot_enough(e_bag_type_bag, items_list_size))
		{
			item_use_end_result = e_error_code_item_bag_full;
			return has_opened_package_num;
		}

		for (int32 items_list_index = 0; items_list_index < items_list_size; items_list_index++)
		{
			const s_item_template_info& item_drop = items_with_num_list[items_list_index];
			const int32 item_template_id = item_drop.m_item_id;
			const int32 item_num = item_drop.m_item_num;
			const int32 item_const_att_lev = 0;

			citem* new_item = create_item_by_template(e_server_log_add_item_item_use, 0, item_template_id, item_num, 1, 0, faith::e_bag_type_none, item_const_att_lev);
			if (nullptr == new_item)
			{
				item_use_end_result = e_error_code_item_invalid_item_template;
				return has_opened_package_num;
			}

			if (new_item->get_item_color() >= e_item_color_orange)
			{
				notice_id = 93000188;
				notice_data.item_data.push_back(new_item->get_item_inst());
			}
			else if (new_item->get_item_color() == e_item_color_purple)
			{
				notice_id = 93000187;
				notice_data.item_data.push_back(new_item->get_item_inst());
			}

			put_in_bag(new_item);

			has_opened_package_num++;
			get_item_list.push_back({ item_template_id ,item_num ,0});
		}
	}
	item_use_end_result = e_error_code_success;

	if (notice_id > 0)
	{
		if (notice_data.item_data.size() > 0)
		{
			player_ref.get_chat_mgr().send_notice_new(notice_id, notice_data);
		}
	}

	return has_opened_package_num;
}

int32 item_set::get_equip_num_by_limit(int32 limit_item_grade, int32 limit_item_color)
{
	int32 item_num = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_magic_2; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		if (temp_equip_ptr->get_item_color() < limit_item_color)
		{
			continue;
		}
		EquipTemplate* equip_template_ptr = temp_equip_ptr->get_equip_info_ptr();
		if (nullptr == equip_template_ptr)
		{
			continue;
		}
		//const int32 quality_level = equip_template_ptr->quality_level;
		//if (quality_level < limit_item_grade)
		//{
		//	continue;
		//}
		item_num++;
	}
	return item_num;
}

int32 item_set::get_equip_num_by_spirit_baptism_color(int32 limit_level)
{
	int32 item_num = 0;
	for (int32 i = 0; i < sprite_bag_num; ++i)
	{
		citem* equiping_spirit_ptr = get_item_by_slot(e_bag_type_equip_spirit, i);
		if (equiping_spirit_ptr == nullptr)
		{
			continue;
		}
		SpiritTemplate* spirit_template_ptr = equiping_spirit_ptr->get_sprite_info_ptr();
		if (nullptr == spirit_template_ptr)
		{
			continue;
		}
		std::vector<int32> spirit_color_arr = init_unit::get_excellent_color_array(
			spirit_template_ptr->BaseExcellentAtt,
			spirit_template_ptr->GreenExcellentRatio,
			spirit_template_ptr->BlueExcellentRatio,
			spirit_template_ptr->PurpleExcellentRatio,
			spirit_template_ptr->OrangeExcellentRatio,
			spirit_template_ptr->RedExcellentRatio,
			spirit_template_ptr->PinkExcellentRatio,
			equiping_spirit_ptr->get_item_inst());

		for (int32 j = 0; j < spirit_color_arr.size(); ++j)
		{
			if (spirit_color_arr[j] >= limit_level)
			{
				item_num++;
			}
		}
	}
	return item_num;
}

int32 item_set::get_equip_num_by_spirit_color(int32 limit_level)
{
	int32 item_num = 0;
	for (int32 i = 0; i < sprite_bag_num; ++i)
	{
		citem* equiping_spirit_ptr = get_item_by_slot(e_bag_type_equip_spirit, i);
		if (equiping_spirit_ptr == nullptr)
		{
			continue;
		}

		if (equiping_spirit_ptr->get_item_color() < limit_level)
		{
			continue;
		}

		SpiritTemplate* spirit_template_ptr = equiping_spirit_ptr->get_sprite_info_ptr();
		if (nullptr == spirit_template_ptr)
		{
			continue;
		}

		int32 quality_level = spirit_template_ptr->Color;

		if (quality_level < limit_level)
		{
			continue;
		}
		item_num++;
	}
	return item_num;
}

int32 item_set::get_equip_num_by_inset_genstone()
{
	int32 item_num = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_badge; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		ItemTemplate* item_template_ptr = temp_equip_ptr->get_item_info_ptr();
		if (nullptr == item_template_ptr)
		{
			continue;
		}
		if (item_template_ptr->item_type == e_item_type_weapon || item_template_ptr->item_type == e_item_type_armor)
		{
			for (int32 i = e_item_info_jewel_slot_0; i <= e_item_info_jewel_vip_slot_1; ++i)
			{
				if (temp_equip_ptr->get_data_info((e_item_info)i) > 0)
				{
					item_num++;
				}
			}
		}
	}
	return item_num;
}

int32 item_set::get_equip_num_awaken_level()
{
	int32 cur_level = 0;
	for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index <= e_role_equip_slot_magic_2; ++temp_equip_index)
	{
		citem* temp_equip_ptr = get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
		if (temp_equip_ptr == nullptr)
		{
			continue;
		}
		if (temp_equip_ptr->get_data_info(e_item_info_wing_exp) > 0)
		{
			cur_level += temp_equip_ptr->get_data_info(e_item_info_wing_exp);
		}
	}
	return cur_level;
}

void item_set::check_target_mission(int32 item_id, int32 item_num, e_server_log_del_item del_type)
{
	player& self_player = unit_man::get_player(m_unit_array_index);
	bool can_check = false;
	switch (del_type)
	{
	case e_server_log_del_item_advance_cost:
	case e_server_log_del_item_assembly_cost:
	case e_server_log_del_item_item_use:
	case e_server_log_del_item_clearn_talent:
	case e_server_log_del_item_rename_cost:
	case e_server_log_del_item_add_on:
	case e_server_log_del_item_treasure:
	case e_server_log_del_item_map_mopping:
	case e_server_log_del_item_teleport:
	case e_server_log_del_item_friend_gift:
	case e_server_log_del_item_submit_psyche:
	case e_server_log_del_item_spirit_illusion:
	case e_server_log_del_item_spirit_voucher:
	case e_server_log_del_item_upgrade:
	case e_server_log_del_item_wing_upgrade:
	case e_server_log_del_item_wing_unlock:
	case e_server_log_del_item_wing_shape_upgrade:
	case e_server_log_del_item_wing_illusionupgrade:
	case e_server_log_del_item_wing_starupgrade:
	case e_server_log_del_item_hope_item_one:
	case e_server_log_del_item_hope_item_ten:
	case e_server_log_del_item_mount_illusion:
	case e_server_log_del_item_mount_unlock:
	case e_server_log_del_item_mount_illusionupgrade:
	case e_server_log_del_item_mount_starupgrade:
	case e_server_log_del_item_mount_upgrade:
	case e_server_log_del_item_succinct:
	case e_server_log_del_item_feather_add_star:
	case e_server_log_del_item_add_soul:
	case e_server_log_del_item_add_sprite:
	case e_server_log_del_item_wing_iilusion:
	case e_server_log_del_item_grade_up:
	case e_server_log_del_item_mainpalace_upgrade:
	case e_server_log_del_item_oracle_activate:
	case e_server_log_del_item_change_class:
	case e_server_log_del_item_back_life:
	case e_server_log_del_item_marry:
	case e_server_log_del_item_red_package:
	case e_server_log_del_item_use_beast:
	case e_server_log_del_item_pokedex_upgrade:
	case e_server_log_del_item_activate_tinder_skill:
	case e_server_log_del_item_spirit_upgrade:
	case e_server_log_del_item_wing_feather_unlock:
	case e_server_log_del_item_forge:
	case e_server_log_del_item_re_legion_name:
	case e_server_log_del_item_enchant:
	case e_server_log_del_item_upgrade_goddess_equip:
	case e_server_log_del_item_goddess_slot_unlock:
	case e_server_log_del_item_wedding_ring_upgrade:
	case e_server_log_del_item_element_merge:
	case e_server_log_del_item_element_unlock:
	case e_server_log_del_item_chat_far_speak:
	case e_server_log_del_item_new_tinder_active:
	case e_server_log_del_item_awaken:
	case e_server_log_del_item_unlock_spirit_slot:
	case e_server_log_del_item_star_upgrade_goddess:
	case e_server_log_del_item_dragontrip:
	{
		can_check = true;
	}
	break;
	default:
		break;
	}
	if (can_check)
	{
		self_player.get_mission_mgr().target_check(e_mission_end_type_use_item, item_id, item_num);
	}
}

void item_set::get_item_send_promp_msg_to_client(const std::vector<s_item_template_info>& item_tuple_array, bool is_continue_get, int32 boss_id)
{
	if (item_tuple_array.size() <= 0)
	{
		return;
	}
	player& self_player = unit_man::get_player(m_unit_array_index);
	if (false == self_player.is_valid())
	{
		return;
	}
	faith::item_proto_item_get_item_msg client_pak;
	client_pak.add_role_guid(self_player.get_unit_guid().A);
	client_pak.add_role_guid(self_player.get_unit_guid().B);
	client_pak.set_is_continue_add(is_continue_get);
	client_pak.set_boss_id(boss_id);
	for (auto& it : item_tuple_array)
	{
		int32 item_id = it.m_item_id;
		int32 item_num = it.m_item_num;
		int32 item_locked = it.m_lock;
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (item_template_ptr && item_template_ptr->AutoUse)
		{
			continue;
		}
		int32 cur_item_num = client_pak.item_data_size() / e_item_get_info_max;
		bool is_merge = false;
		for (int32 j = 0; j < cur_item_num; ++j)
		{
			int32 this_id = client_pak.item_data(j * e_item_get_info_max + e_item_get_info_item_template_id);
			int32 this_num = client_pak.item_data(j * e_item_get_info_max + e_item_get_info_item_num);
			int32 this_locked = client_pak.item_data(j * e_item_get_info_max + e_item_get_info_locked_state);

			if (this_id == item_id && this_locked == item_locked)
			{
				is_merge = true;
				client_pak.set_item_data(j * e_item_get_info_max + e_item_get_info_item_num, this_num + item_num);
				break;
			}
		}

		if (!is_merge)
		{
			client_pak.add_item_data(item_id);
			client_pak.add_item_data(item_num);
			client_pak.add_item_data(item_locked);
		}
	}

	self_player.send_message(&client_pak, e_msgindex_s2c_get_item);
}

faith::int32 item_set::get_mount_illusion_slot(const int32 mount_illusion_template_id)
{
	MountTemplate* item_mount_illustion_template_ptr = GET_TEMPLATE(MountTemplate, mount_illusion_template_id);
	if (nullptr == item_mount_illustion_template_ptr)
	{
		return -1;
	}

	if (item_mount_illustion_template_ptr->PreMountRequirement.size() <= 1)
	{
		return -1;
	}
	const int32 item_mount_origin_template_id = item_mount_illustion_template_ptr->PreMountRequirement[0];
	citem* item_mount_origin_ptr = get_item_by_template_id(item_mount_origin_template_id);
	if (nullptr == item_mount_origin_ptr)
	{
		return -1;
	}

	MountTemplate* item_mount_origin_template_ptr = item_mount_origin_ptr->get_mount_template_ptr();
	if (nullptr == item_mount_origin_template_ptr)
	{
		return -1;
	}

	const int32 item_mount_illusion_array_size = item_mount_origin_template_ptr->IllusionList.size();
	for (int32 item_mount_illusion_array_index = 0; item_mount_illusion_array_index < item_mount_illusion_array_size; item_mount_illusion_array_index++)
	{
		const int32 item_template_id = item_mount_origin_template_ptr->IllusionList[item_mount_illusion_array_index];
		ItemTemplate* item_template_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(item_template_id);
		if (nullptr == item_template_ptr)
		{
			continue;
		}
		if (item_template_ptr->logic_id == mount_illusion_template_id)
		{
			return item_mount_illusion_array_index;
		}
	}
	return -1;
}

e_item_succinct_buff_level item_set::get_succinct_equip_buff_level()
{
	for (int32 j = e_item_succinct_buff_level_SSS; j > e_item_succinct_buff_level_none; --j)
	{
		int32 cur_buff_level_num = 0;
		int32 non_equip_num = 0;
		for (int32 i = e_role_equip_slot_hat; i <= e_role_equip_slot_magic_2; ++i)
		{
			if (non_equip_num >= e_role_equip_slot_magic_2 - add_buff_need_equip_num)
			{
				return e_item_succinct_buff_level_none;
			}
			citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
			if (temp_equip_item_ptr == nullptr)
			{
				non_equip_num++;
				continue;
			}

			bool can_add_att = is_equipment_effective(temp_equip_item_ptr->get_item_guid());
			if (false == can_add_att)
			{
				continue;
			}

			int32 current_level = temp_equip_item_ptr->get_item_succinct_current_buff_level();
			if (current_level >= j)
			{
				cur_buff_level_num++;
			}
		}
		if (cur_buff_level_num >= add_buff_need_equip_num)
		{
			return (e_item_succinct_buff_level)j;
		}
	}
	return e_item_succinct_buff_level_none;
}

int32 item_set::get_succinct_weapon_buff()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return 0;
	}

	citem* weapon_item_ptr = get_item_by_slot(e_bag_type_equip, e_role_equip_slot_weapon_1);
	if (weapon_item_ptr == nullptr)
	{
		return 0;
	}

	bool can_add_att = is_equipment_effective(weapon_item_ptr->get_item_guid());
	if (false == can_add_att)
	{
		return 0;
	}

	std::vector<int32>& buff_array = GAMECONFIG->SuccinctWeaponBuffIdArray;

	int32 max_num = buff_array.size() / 5;
	int32 buff_level = weapon_item_ptr->get_item_succinct_current_buff_level();
	if (buff_level <= 0)
	{
		return 0;
	}
	if (buff_level > max_num)
	{
		buff_level = max_num;
	}
	int32 buff_offect = max_num * temp_player.get_unit_info(e_role_info_class_type) + buff_level - 1;

	if (buff_offect < 0 || buff_offect >= buff_array.size())
	{
		return 0;
	}
	return buff_array[buff_offect];
}
int32 item_set::get_succinct_equip_buff()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return 0;
	}

	std::vector<int32>& buff_array = GAMECONFIG->SuccinctEquipBuffIdArray;
	int32 max_num = buff_array.size() / 5;
	int32 buff_level = get_succinct_equip_buff_level();
	if (buff_level <= 0)
	{
		return 0;
	}
	if (buff_level > max_num)
	{
		buff_level = max_num;
	}
	int32 buff_offect = max_num * temp_player.get_unit_info(e_role_info_class_type) + buff_level - 1;

	if (buff_offect < 0 || buff_offect >= buff_array.size())
	{
		return 0;
	}
	return buff_array[buff_offect];
}

void item_set::jewel_array_set_to_item(citem* target_item, std::vector<int32> jewel_array, bool is_send_mail)
{
	if (target_item == nullptr
		|| jewel_array.size() <= 0)
	{
		return;
	}
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return;
	}
	EquipTemplate* equip_template_ptr = target_item->get_equip_info_ptr();
	if (equip_template_ptr == nullptr)
	{
		return;
	}


	//int32 slot_open_num_by_equip = init_unit::get_jewel_slot_open_num_by_equip(equip_template_ptr->quality_level);
	//int32 slot_open_num_by_vip = init_unit::get_jewel_slot_open_num_by_vip(temp_player.get_vip_level());

	//for (int32 i = 0; i < slot_open_num_by_equip; ++i)
	//{
	//	int32 target_jewel_slot = e_item_info_jewel_slot_0 + i;
	//	if (target_jewel_slot >= e_item_info_jewel_vip_slot_0
	//		|| target_item->get_data_info((e_item_info)target_jewel_slot) > 0)
	//	{
	//		break;
	//	}
	//	int32 maximum_index = -1;
	//	int32 maximum_id = 0;
	//	for (int32 j = 0; j < jewel_array.size(); ++j)
	//	{
	//		if (jewel_array[j] > maximum_id)
	//		{
	//			maximum_index = j;
	//			maximum_id = jewel_array[j];
	//		}
	//	}
	//	if (maximum_id > 0)
	//	{
	//		jewel_array.erase(jewel_array.begin() + maximum_index);
	//		target_item->set_data_info((e_item_info)target_jewel_slot, maximum_id);
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}

	//for (int32 i = 0; i < slot_open_num_by_vip; ++i)
	//{
	//	int32 target_jewel_slot = e_item_info_jewel_vip_slot_0 + i;
	//	if (target_jewel_slot > e_item_info_jewel_vip_slot_1
	//		|| target_item->get_data_info((e_item_info)target_jewel_slot) > 0)
	//	{
	//		break;
	//	}
	//	int32 maximum_index = -1;
	//	int32 maximum_id = 0;
	//	for (int32 j = 0; j < jewel_array.size(); ++j)
	//	{
	//		if (jewel_array[j] > maximum_id)
	//		{
	//			maximum_index = j;
	//			maximum_id = jewel_array[j];
	//		}
	//	}
	//	if (maximum_id > 0)
	//	{
	//		jewel_array.erase(jewel_array.begin() + maximum_index);
	//		target_item->set_data_info((e_item_info)target_jewel_slot, maximum_id);
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}

	std::vector<citem*> need_send_mail_array;
	if (is_send_mail)
	{
		need_send_mail_array.reserve(jewel_array.size());
		for (int32 i = 0; i < jewel_array.size(); ++i)
		{
			citem* temp_item = create_item_by_template(e_server_log_add_item_set_jewel_off, 0, jewel_array[i], 1, 1);
			if (temp_item == nullptr)
			{
				continue;
			}
			need_send_mail_array.push_back(temp_item);
		}
	}

	if (need_send_mail_array.size() > 0)
	{
		put_in_bag(need_send_mail_array, e_bag_type_none, e_item_mail_type_jewel_send_mail);
	}
	else
	{
		temp_player.get_chat_mgr().receive_top_tip(90091119);
	}
}

void item_set::refresh_spirit_att(int32 old_spirit_id, int32 new_spirit_id)
{

	SpiritTemplate* old_temp_template = GET_TEMPLATE(SpiritTemplate, old_spirit_id);
	SpiritTemplate* new_temp_template = GET_TEMPLATE(SpiritTemplate, new_spirit_id);
	if (nullptr == old_temp_template || nullptr == new_temp_template)
	{
		return;
	}
	item_change_att(old_temp_template->BasicProperty, 1, false);
	item_change_att(new_temp_template->BasicProperty, 1, true);
}

citem* item_set::get_wedding_equip_item()
{
	citem* equip_ptr = get_item(GET_BAG(e_bag_type_wedding_equip)[0]);
	if (nullptr != equip_ptr && nullptr != equip_ptr->get_item_info_ptr() && equip_ptr->get_item_info_ptr()->item_type == e_item_type_wedding_equip)
		return equip_ptr;
	return nullptr;
}

void item_set::set_wedding_equip_att(citem* equip_ptr, bool is_add)
{
	if (nullptr == equip_ptr)
		return;

	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (player_ref.is_valid() == false)
		return;
	ItemTemplate* item_template_ptr = equip_ptr->get_item_info_ptr();
	if (nullptr == item_template_ptr || item_template_ptr->item_type != e_item_type_wedding_equip)
		return;
	if (is_add && equip_ptr->get_data_info(e_item_info_activate) == 0)
		return;
	WeddingRingTemplate* template_ptr = equip_ptr->get_wedding_ring_info_ptr();
	if (template_ptr)
	{
		item_change_att(template_ptr->AttArray, 1, is_add);
	}

	WeddingRingTemplate* mate_template_ptr = equip_ptr->get_wedding_ring_info_ptr_by_mate_ring_level();
	if (mate_template_ptr)
	{
		item_change_att(mate_template_ptr->AttArray, mate_template_ptr->mateGetAttFloat, is_add);
	}

	AdditionBuffTemplate* heart_value_template_ptr = equip_ptr->get_heart_value_template_ptr();
	if (heart_value_template_ptr)
	{
		item_change_att(heart_value_template_ptr->AdditionValue, 1, is_add);
	}
}

void item_set::set_wedding_init_fashion_state(bool activate)
{
	if (false == activate)
	{
		for (int32 i = 0; i < e_fashion_equip_slot_max; ++i)
		{
			if (GET_BAG(e_bag_type_equip_fasion)[i].is_valid())
			{
				citem* item_ptr = get_item(GET_BAG(e_bag_type_equip_fasion)[i].server_64);
				if (item_ptr && item_ptr->get_data_info(e_item_info_info_id) == init_wedding_fashion)
				{
					equip_off(item_ptr->get_item_guid());
				}
			}
		}
	}
	for (int32 i = 0; i < max_fasion_item_bag_size; ++i)
	{
		if (GET_BAG(e_bag_type_fasion)[i].is_valid())
		{
			citem* item_ptr = get_item(GET_BAG(e_bag_type_fasion)[i].server_64);
			if (item_ptr && item_ptr->get_data_info(e_item_info_info_id) == init_wedding_fashion)
			{
				if (item_ptr->get_data_info(e_item_info_activate) && false == activate)
				{
					int32 star_num = item_ptr->get_data_info(e_item_info_upgrade_count);
					item_ptr->set_data_info(e_item_info_activate, 0);
					item_ptr->set_data_info(e_item_info_upgrade_count, 0);
					set_fashion_att(item_ptr, false, star_num);
					send_item_one(item_ptr, true);
				}
				else if (0 == item_ptr->get_data_info(e_item_info_activate) && true == activate)
				{
					item_ptr->set_data_info(e_item_info_activate, 1);
					item_ptr->set_data_info(e_item_info_upgrade_count, 1);
					set_fashion_att(item_ptr, true, 1);
					send_item_one(item_ptr, true);
				}
			}
		}
	}
}

int32 item_set::get_equip_smallest_addon_level()
{
	int32 return_value = -1;
	for (int32 i = e_role_equip_slot_weapon_1; i < e_role_equip_slot_amulet; ++i)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
		if (temp_equip_item_ptr == nullptr)
		{
			return 0;
		}
		if (false == is_equipment_effective(temp_equip_item_ptr->get_item_guid()))
		{
			return 0;
		}
		if (return_value == -1 || temp_equip_item_ptr->get_data_info(e_item_info_add_on) < return_value)
		{
			return_value = temp_equip_item_ptr->get_data_info(e_item_info_add_on);
		}
	}
	return return_value;
}

int32 item_set::get_equip_smallest_awaken_level()
{
	int32 return_value = -1;
	for (int32 i = e_role_equip_slot_weapon_1; i < e_role_equip_slot_amulet; ++i)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
		if (temp_equip_item_ptr == nullptr)
		{
			return 0;
		}
		if (false == is_equipment_effective(temp_equip_item_ptr->get_item_guid()))
		{
			return 0;
		}
		if (return_value == -1 || temp_equip_item_ptr->get_data_info(e_item_info_wing_exp) < return_value)
		{
			return_value = temp_equip_item_ptr->get_data_info(e_item_info_wing_exp);
		}
	}
	return return_value;
}
int32 item_set::get_equip_smallest_forge_level()
{
	int32 return_value = -1;
	for (int32 i = e_role_equip_slot_weapon_1; i < e_role_equip_slot_amulet; ++i)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[i]);
		if (temp_equip_item_ptr == nullptr)
		{
			return 0;
		}
		if (false == is_equipment_effective(temp_equip_item_ptr->get_item_guid()))
		{
			return 0;
		}
		EquipTemplate* equip_item_ptr = temp_equip_item_ptr->get_equip_info_ptr();
		//if (equip_item_ptr == nullptr || equip_item_ptr->quality_level < 5)
		//{
		//	return 0;
		//}
		int32 temp_color_level = 0;
		{
			EquipTemplate* equip_template_ptr = temp_equip_item_ptr->get_equip_info_ptr();
			if (equip_template_ptr == nullptr)
			{
				return 0;
			}
			//auto tmp_vec = init_unit::get_excellent_color_array(equip_template_ptr->BaseExcellentAtt,
			//	equip_template_ptr->GreenExcellentRatio,
			//	equip_template_ptr->BlueExcellentRatio,
			//	equip_template_ptr->PurpleExcellentRatio,
			//	equip_template_ptr->OrangeExcellentRatio,
			//	equip_template_ptr->RedExcellentRatio,
			//	equip_template_ptr->PinkExcellentRatio,
			//	temp_equip_item_ptr->get_item_inst());
			//for (auto iter = tmp_vec.begin(); iter != tmp_vec.end(); ++iter)
			//{
			//	if (temp_color_level == 0 || temp_color_level > *iter)
			//		temp_color_level = *iter;
			//}
		}
		if (temp_color_level == 0)
			return 0;
		if (return_value == -1 || temp_color_level < return_value)
		{
			return_value = temp_color_level;
		}
	}
	return return_value;
}
int32 item_set::get_equip_smallest_enchant_level(bool is_jewelry)
{
	int32 return_value = -1;

	std::vector<int32> check_equip_slot;
	if (is_jewelry)
	{
		check_equip_slot.push_back(e_role_equip_slot_earring);
		check_equip_slot.push_back(e_role_equip_slot_necklace);
		check_equip_slot.push_back(e_role_equip_slot_ring);
	}
	else
	{
		check_equip_slot.push_back(e_role_equip_slot_hat);
		check_equip_slot.push_back(e_role_equip_slot_armor);
		check_equip_slot.push_back(e_role_equip_slot_pants);
		check_equip_slot.push_back(e_role_equip_slot_glove);
		check_equip_slot.push_back(e_role_equip_slot_boot);
	}

	int32 level_number[faith::max_enchant_type_num + 1];
	memset(level_number, 0, sizeof(level_number));
	for (auto iter = check_equip_slot.begin(); iter != check_equip_slot.end(); ++iter)
	{
		citem* temp_equip_item_ptr = get_item(GET_BAG(e_bag_type_equip)[*iter]);
		if (temp_equip_item_ptr == nullptr)
		{
			return 0;
		}
		if (false == is_equipment_effective(temp_equip_item_ptr->get_item_guid()))
		{
			return 0;
		}

		int32 enchant_value = temp_equip_item_ptr->get_data_info(e_item_info_illusion_had_byte);
		if (enchant_value <= 0)
			return 0;
		int32 enchant_type = enchant_value % faith::max_enchant_type_num;
		level_number[enchant_type] += 1;
	}

	for (int32 i = 0; i <= faith::max_enchant_type_num; ++i)
	{
		if (level_number[i] >= 2)
		{
			return_value = i;
			break;
		}
	}

	return return_value;
}

int32 item_set::get_equip_smallest_enchant_level_by_type(int32 show_type)
{
	faith::template_manager::template_type* enchant_table = template_manager::get_instance().get_templates(e_EquipEnchantTemplate);
	if (nullptr == enchant_table)
	{
		return 0;
	}
	std::vector<int32> check_equip_slot;
	check_equip_slot.clear();
	faith::template_manager::template_type::iterator ite;
	for (ite = enchant_table->begin(); ite != enchant_table->end(); ++ite)
	{
		EquipEnchantTemplate* temp_enchant_template_ptr = (EquipEnchantTemplate*)(ite->second);
		if (temp_enchant_template_ptr == nullptr)
		{
			continue;
		}
		if (temp_enchant_template_ptr->Level == 1 && show_type == temp_enchant_template_ptr->Type)
		{
			check_equip_slot.push_back(temp_enchant_template_ptr->Slot);
		}
	}
	if (check_equip_slot.size() <= 0)
	{
		return 0;
	}

	for (auto iter = check_equip_slot.begin(); iter != check_equip_slot.end(); ++iter)
	{
		int32 item_slot = (*iter + 1);
		citem* temp_equip_item_ptr = get_equip_item_by_slot(e_role_equip_slot(item_slot));
		if (temp_equip_item_ptr == nullptr)
		{
			return 0;
		}
		if (false == is_equipment_effective(temp_equip_item_ptr->get_item_guid()))
		{
			return 0;
		}

		int32 enchant_value = temp_equip_item_ptr->get_data_info(e_item_info_illusion_had_byte);
		if (enchant_value <= 0)
		{
			return 0;
		}

		if ((enchant_value % faith::max_enchant_type_num) != show_type)
		{
			return 0;
		}

	}

	return 1;
}

int32 item_set::get_equipment_addon_buff_id()
{
	int32 buff_id = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return buff_id;
	}
	int32 class_type = temp_player.get_unit_info(e_role_info_class_type);
	if (class_type < 0)
	{
		return buff_id;
	}

	int32 smallest_level = get_equip_smallest_addon_level();
	int32 phase_level = init_unit::get_addon_phase_level(smallest_level);

	ItemAddonTemplate* addon_template_ptr = GET_TEMPLATE(ItemAddonTemplate, item_addon_default_start_id + phase_level);

	if (addon_template_ptr == nullptr || addon_template_ptr->EquipBuffArray.size() < class_type)
	{
		return buff_id;
	}
	buff_id = addon_template_ptr->EquipBuffArray[class_type];

	return buff_id;
}

int32 item_set::get_equipment_awaken_buff_id()
{
	int32 buff_id = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return buff_id;
	}
	int32 class_type = temp_player.get_unit_info(e_role_info_class_type);
	if (class_type < 0)
	{
		return buff_id;
	}
	int32 smallest_level = get_awaken_fetter_max_num();
	if (smallest_level <= 0)
	{
		return buff_id;
	}

	int32 max_length = GAMECONFIG->AwakenBuffArray.size() / 6;
	if (smallest_level > max_length)
	{
		return buff_id;
	}

	int32 index = (smallest_level - 1) * 6 + class_type + 1;
	buff_id = GAMECONFIG->AwakenBuffArray[index];
	return buff_id;
}

int32 item_set::get_equipment_forge_buff_id()
{
	int32 buff_id = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return buff_id;
	}
	int32 class_type = temp_player.get_unit_info(e_role_info_class_type);
	if (class_type < 0)
	{
		return buff_id;
	}
	int32 smallest_level = get_equip_smallest_forge_level();
	if (smallest_level <= 0)
		return buff_id;

	int32 index = (smallest_level - 1) * 4 + (class_type - 1);
	if (GAMECONFIG->ForgeBuffArray.size() < index)
	{
		return 0;
	}

	buff_id = GAMECONFIG->ForgeBuffArray[index];

	return buff_id;
}
int32 item_set::get_equipment_enchant_buff_id()
{
	int32 buff_id = 0;
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return buff_id;
	}
	int32 class_type = temp_player.get_unit_info(e_role_info_class_type);
	if (class_type < 0)
	{
		return buff_id;
	}
	int32 show_type = temp_player.get_unit_info(e_role_info_enchant_show_type);
	if (0 == show_type)
	{
		show_type = refresh_enchant_buff_show_type();
		if (0 == show_type)
		{
			return 0;
		}
	}
	int32 smallest_level = get_equip_smallest_enchant_level_by_type(show_type);
	if (smallest_level <= 0)
	{
		show_type = refresh_enchant_buff_show_type();
		if (0 == show_type)
		{
			temp_player.set_unit_info(e_role_info_enchant_show_type, 0);
			temp_player.send_info_one(e_role_info_enchant_show_type);
			return 0;
		}
		smallest_level = get_equip_smallest_enchant_level_by_type(show_type);
		if (smallest_level <= 0)
		{
			temp_player.set_unit_info(e_role_info_enchant_show_type, 0);
			temp_player.send_info_one(e_role_info_enchant_show_type);
			return 0;
		}
	}

	int32 index = (show_type - 1) * 4 + (class_type - 1);
	if (GAMECONFIG->EnchantEquipBuffArray.size() < index)
	{
		return 0;
	}

	buff_id = GAMECONFIG->EnchantEquipBuffArray[index];

	return buff_id;
}

int32 item_set::refresh_enchant_buff_show_type()
{
	player& temp_player = unit_man::get_player(m_unit_array_index);
	if (temp_player.is_valid() == false)
	{
		return 0;
	}
	int32 max_type = 0;
	int32 max_level = get_enchant_max_level();
	for (int32 i = 1; i <= max_level; ++i)
	{
		int32 smallest_level = get_equip_smallest_enchant_level_by_type(i);
		if (smallest_level <= 0)
		{
			continue;
		}
		max_type = i;
	}
	temp_player.set_unit_info(e_role_info_enchant_show_type, max_type);
	temp_player.send_info_one(e_role_info_enchant_show_type);
	return max_type;
}

int32 item_set::get_enchant_max_level()
{
	int32 max_level = 0;
	faith::template_manager::template_type* enchant_table = template_manager::get_instance().get_templates(e_EquipEnchantTemplate);
	if (nullptr == enchant_table)
	{
		return max_level;
	}
	faith::template_manager::template_type::iterator ite;
	for (ite = enchant_table->begin(); ite != enchant_table->end(); ++ite)
	{
		EquipEnchantTemplate* temp_enchant_template_ptr = (EquipEnchantTemplate*)(ite->second);
		if (temp_enchant_template_ptr == nullptr)
		{
			continue;
		}
		if (temp_enchant_template_ptr->Type > max_level)
		{
			max_level = temp_enchant_template_ptr->Type;
		}
	}
	return max_level;
}

int32 item_set::get_awaken_fetter_max_num()
{
	int32 max_num = 0;
	for (int32 i = 0; i < awaken_fetter_bag_max; ++i)
	{
		if (GET_BAG(e_bag_type_awaken_item)[i].is_valid())
		{
			citem* temp_item = get_item(GET_BAG(e_bag_type_awaken_item)[i]);
			if (temp_item && temp_item->get_data_info(e_item_info_upgrade_count) > 0)
			{
				ItemTemplate* item_template_ptr = temp_item->get_item_info_ptr();
				if (item_template_ptr == nullptr)
				{
					continue;
				}
				AwakenFetterTemplate* template_ptr = GET_TEMPLATE(AwakenFetterTemplate, item_template_ptr->logic_id);
				if (template_ptr == nullptr)
				{
					continue;
				}
				if (temp_item->get_data_info(e_item_info_upgrade_count) >= template_ptr->MaxNum)
				{
					max_num++;
				}
			}
		}
	}
	return max_num;
}

void item_set::equip_off_addon_show_buff()
{
	int32 equip_buff_id = get_equipment_addon_buff_id();
	buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_on_addon_show_buff()
{
	int32 equip_buff_id = get_equipment_addon_buff_id();
	buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, equip_buff_id);
}

void item_set::equip_off_awaken_show_buff()
{
	int32 equip_buff_id = get_equipment_awaken_buff_id();
	buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_on_awaken_show_buff()
{
	int32 equip_buff_id = get_equipment_awaken_buff_id();
	buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_off_forge_show_buff()
{
	int32 equip_buff_id = get_equipment_forge_buff_id();
	buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_on_forge_show_buff()
{
	int32 equip_buff_id = get_equipment_forge_buff_id();
	buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_off_enchant_show_buff()
{
	int32 equip_buff_id = get_equipment_enchant_buff_id();
	buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, equip_buff_id);
}
void item_set::equip_on_enchant_show_buff()
{
	int32 equip_buff_id = get_equipment_enchant_buff_id();
	buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, equip_buff_id);
}

void item_set::equip_off_equip_show_buff()
{
	equip_off_upgrade_show_buff();
	equip_off_addon_show_buff();
	equip_off_awaken_show_buff();
	equip_off_forge_show_buff();
	equip_off_enchant_show_buff();
	refresh_succinct_buff(false);
}

void item_set::equip_on_equip_show_buff()
{
	equip_on_upgrade_show_buff();
	equip_on_addon_show_buff();
	equip_on_awaken_show_buff();
	equip_on_forge_show_buff();
	equip_on_enchant_show_buff();
	refresh_succinct_buff(true);
}