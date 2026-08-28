/********************************************************************
created:	2016年8月9日13:11:56
file base:	globle_data
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/

#include "activity_def.hpp"
#include "template/template_manager.h"
#include "time.hpp"
#include "log/server_log.hpp"
#include "globle_data.h"
#include "char_def.hpp"
#include "time_def.hpp"
#include "init_unit.h"
#include "guid_gen.h"
#include "random.h"
#include "internal/core.hpp"


namespace hld
{
	void globle_data::init_manager()
	{
		m_region_template_ptr = GET_TEMPLATE(RegionTemplate, 150000000);
		m_version_template_ptr = GET_TEMPLATE(VersionTemplate, 148000000);
		clear_char_notice_info();
		init_map_safe_point();
		init_att_gs();
		init_crc();
		init_char(90000, 90000, m_char_can_use_vector);
		init_char(90000, 90000, m_char_no_use_vector);
		init_community_group_relation();
	}
	void globle_data::init_for_client()
	{
		init_community_group_relation();
	}

	int32 globle_data::get_region_code() 
	{ 
		return m_region_template_ptr == nullptr ? -1 : m_region_template_ptr->RegionCode;
	}
	map_safe_point_vec* globle_data::get_map_save_point_ary(int32 map_template_id)
	{
		auto it = m_map_safe_point_map.find(map_template_id);
		if (it == m_map_safe_point_map.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	bool comp_safe_vec(const s_map_block& a, const s_map_block& b)
	{
		return a.priority > b.priority;
	}
	void globle_data::init_map_safe_point()
	{
		template_manager::template_type* map_template_ptr_ary = template_manager::get_instance().get_templates(e_MapTemplate);
		if (nullptr == map_template_ptr_ary)
		{
			return;
		}
		template_manager::template_type::iterator ite;
		for (ite = map_template_ptr_ary->begin(); ite != map_template_ptr_ary->end(); ++ite)
		{
			MapTemplate* map_template_ptr = (MapTemplate*)(ite->second);
			if (map_template_ptr && map_template_ptr->IsAllowPK != e_pk_mode_type_cant && map_template_ptr->SafePoint.size() > 0)
			{
				map_safe_point_vec temp_safe_vec;
				for (int32 i = 0; i < map_template_ptr->SafePoint.size(); ++i)
				{
					MapSafeTemplate* map_safe_template_ptr = GET_TEMPLATE(MapSafeTemplate, map_template_ptr->SafePoint[i]);
					//if (map_safe_template_ptr && map_safe_template_ptr->IsAllowPK == e_pk_mode_type_cant)
					if (map_safe_template_ptr)
					{
						s_map_block tmp_block;
						tmp_block.region_id = map_template_ptr->SafePoint[i];
						tmp_block.priority = map_safe_template_ptr->Priority;
						tmp_block.region_type = map_safe_template_ptr->AreaType;
						for (int32 j = 0; j + 1 < map_safe_template_ptr->PointAry.size(); j += 2)
						{
							//if (map_safe_template_ptr->PointAry[j] > 0)
							//{
							tmp_block.points.push_back(fvector(map_safe_template_ptr->PointAry[j], map_safe_template_ptr->PointAry[j + 1]));
							//}
						}
						temp_safe_vec.push_back(tmp_block);
					}
				}
				std::sort(temp_safe_vec.begin(), temp_safe_vec.end(), comp_safe_vec);
				m_map_safe_point_map[map_template_ptr->attribute_id] = temp_safe_vec;
			}
		}
	}
	void globle_data::init_att_gs()
	{
		for (int32 ers_index = e_unit_attack_att_strength; ers_index < e_unit_attack_att_max; ers_index++)
		{
			AttributeTemplate* att_template_ptr = GET_TEMPLATE(AttributeTemplate, ers_index + attribute_template_id_start);
			if (nullptr == att_template_ptr)
			{
				return;
			}
			m_unit_gs_value[ers_index] = att_template_ptr->AttGs;
		}
	}

#define CRC32_POLY 0x04c11db7

	void globle_data::init_crc()
	{
		for (int32 iCRC = 0; iCRC < 256; iCRC++)
		{
			for (int32 c = iCRC << 24, j = 8; j != 0; j--)
			{
				m_GCRCTable[iCRC] = c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
			}
		}
	}
	uint32 globle_data::app_ansi_str_crc(const char* Data)
	{
		int32 Length = strlen(Data);
		uint32 CRC = 0xFFFFFFFF;
		for (int32 i = 0; i < Length; i++)
		{
			char C = Data[i];
			int32   CL = (C & 255);
			CRC = (CRC << 8) ^ m_GCRCTable[(CRC >> 24) ^ CL];;
			int32   CH = (C >> 8) & 255;
			CRC = (CRC << 8) ^ m_GCRCTable[(CRC >> 24) ^ CH];;
		}
		return ~CRC;
	}
	bool char_sort_function(const std::string& str1, const std::string& str2)
	{
		return str1.size() < str2.size();
	}
	void globle_data::init_char(int32 begin_id, int32 end_id, word_vector& char_vector)
	{
		//		for (int32 string_id = begin_id; string_id < end_id; string_id++)
		//		{
		//			StringTemplate* string_template_ptr = GET_TEMPLATE(StringTemplate, string_id);
		//			if (nullptr == string_template_ptr)
		//			{
		//				continue;
		//			}
		//			if (string_template_ptr->StrList.empty() == false && string_template_ptr->StrList[0].empty() == false)
		//			{
		//				char_vector.push_back(string_template_ptr->StrList[0]);
		//			}
		//		}
		//		std::sort(char_vector.begin(), char_vector.end(), char_sort_function);
	}


	void globle_data::init_server_open_time()
	{
		static bool is_open_tm_inited = false;
		if (is_open_tm_inited)
		{
			return;
		}

		m_server_open_time_str.clear();
		m_server_open_state_end_time_str.clear();
		tm* t = time_helper::get_localtime();
		if (t)
		{
			m_server_open_time_struct = *t;
		}
		is_open_tm_inited = true;
	}

	void globle_data::init_server_open_time(std::string server_open_str)
	{
		init_server_open_time();

		if (server_open_str.empty())
		{
			server_open_str = time_helper::get_current_time();
		}
		m_server_open_time_str = server_open_str;
		time_helper::get_time_by_string(m_server_open_time_str.c_str(), &m_server_open_time_struct);

		uint32 open_time_stamp = mktime(&m_server_open_time_struct); //time_t
		open_time_stamp += GAMECONFIG->ServerOpenStateDay * 24 * 3600;
		tm* temp_tm = time_helper::get_tm_by_time(open_time_stamp);
		if (nullptr == temp_tm)
		{
			return;
		}
		tm temp_end_tm = *temp_tm;
		temp_end_tm.tm_hour = 0;
		temp_end_tm.tm_min = 0;
		temp_end_tm.tm_sec = 0;
		m_server_open_state_end_time_str = time_helper::get_str_time_by_tm(&temp_end_tm);
		set_server_days();
	}

	void globle_data::init_server_open_time(uint32 server_open_stamp)
	{
		std::string server_open_str = time_helper::get_str_time_by_stamp(server_open_stamp);
		init_server_open_time(server_open_str);
	}

	bool globle_data::is_ancient_battle(int32 map_template_id)
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return false;
		}
		return map_template_ptr->Type == e_map_type_field;
	}

	bool globle_data::is_belief_cloister_battle(int32 map_template_id)
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return false;
		}
		return map_template_ptr->Type == e_map_type_belief_cloister;
	}

	int32 globle_data::get_community_group_relation_by_type(int32 type1, int32 type2)
	{
		if (type1 < 0 || type1 >= e_community_type_max)
		{
			return 0;
		}
		if (type2 < 0 || type2 >= e_community_type_max)
		{
			return 0;
		}
		return m_community_group_relation.data_ary[type1][type2];
	}

	void globle_data::init_community_group_relation()
	{
		for (int32 i = 0; i < e_community_type_max; i++)
		{
			for (int32 j = 0; j < e_community_type_max; j++)
			{
				if (i == j)
				{
					m_community_group_relation.data_ary[i][j] = 1;
				}
				else
				{
					m_community_group_relation.data_ary[i][j] = 0;
				}
			}
		}
		for (int32 i = 0; i < e_community_type_max; i++)
		{
			m_community_group_relation.data_ary[e_community_type_common_friend][i] = 1;
			m_community_group_relation.data_ary[i][e_community_type_common_friend] = 1;
		}

		m_community_group_relation.data_ary[e_community_type_player_group1][e_community_type_friend_npc_group1] = 1;
		m_community_group_relation.data_ary[e_community_type_friend_npc_group1][e_community_type_player_group1] = 1;

		m_community_group_relation.data_ary[e_community_type_player_group2][e_community_type_friend_npc_group2] = 1;
		m_community_group_relation.data_ary[e_community_type_friend_npc_group2][e_community_type_player_group2] = 1;

		m_community_group_relation.data_ary[e_community_type_npc_pet][e_community_type_friend_npc_group1] = 1;
		m_community_group_relation.data_ary[e_community_type_friend_npc_group1][e_community_type_npc_pet] = 1;

		m_community_group_relation.data_ary[e_community_type_npc_pet][e_community_type_friend_npc_group1] = 1;



		m_community_group_relation.data_ary[e_community_type_attack_player][e_community_type_attack_car] = 1;
		m_community_group_relation.data_ary[e_community_type_attack_car][e_community_type_attack_player] = 1;

		m_community_group_relation.data_ary[e_community_type_guard_player][e_community_type_guard_car] = 1;
		m_community_group_relation.data_ary[e_community_type_guard_player][e_community_type_guard_boss] = 1;
		m_community_group_relation.data_ary[e_community_type_guard_car][e_community_type_guard_player] = 1;
		m_community_group_relation.data_ary[e_community_type_guard_car][e_community_type_guard_boss] = 1;
		m_community_group_relation.data_ary[e_community_type_guard_boss][e_community_type_guard_player] = 1;
		m_community_group_relation.data_ary[e_community_type_guard_boss][e_community_type_guard_car] = 1;

		m_community_group_relation.data_ary[e_community_type_guard_player][e_community_type_attack_car] = 1;
		m_community_group_relation.data_ary[e_community_type_attack_car][e_community_type_guard_player] = 1;
		m_community_group_relation.data_ary[e_community_type_attack_player][e_community_type_guard_car] = 1;
		m_community_group_relation.data_ary[e_community_type_guard_car][e_community_type_attack_player] = 1;
	}
	void globle_data::send_mail_to_another_player_by_system(
		guid_64 target_player_guid,
		guid_64 sender_guid,
		int32 receiver_server_id,
		e_item_mail_type mail_type,
		int32 money_typ1,
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2,
		const s_item_info* item_list,
		int32 item_num)
	{
		std::string SenderName = "";
		std::string title = "";
		std::string contentText = "";
		switch (mail_type)
		{
		case e_item_mail_type_nomal:
		{
			SenderName = "90303013";
			title = "90009004";
			contentText = "90303016";
			break;
		}
		break;
		case e_item_mail_type_auction_item:
		{
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_purchase_title);
			contentText = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_purchase_content);
			break;
		}
		break;
		case e_item_mail_type_auction_cancel_sell_item:
		{
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_cancel_sell_title);
			contentText = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_cancel_sell_bag_full_content);
			break;
		}
		break;
		case e_item_mail_type_feather_heart_put_bag:
		{
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_skill_bag_full_send_mail_title);
			contentText = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_skill_bag_full_send_mail_title);
			break;
		}
		break;
		case e_item_mail_type_blessing:
		{
			title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_blessing_title);
			contentText = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_blessing_context);
			break;
		}
		break;
		case e_item_mail_type_arena_reward:
		{
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_arena_reward_mail_title);
			contentText = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_arena_reward_mail_content);
			break;
		}
		break;
		case e_item_mail_type_jewel_send_mail:
		{
			title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_jewel_send_mail_title);
			contentText = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_jewel_send_mail_content);
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			break;
		}
		break;
		case e_item_mail_type_item_recovery:
		{
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			title = "90303033";
			contentText = "90303034";
			break;
		}
		break;
		case e_item_mail_type_hope_item:
		{
			title = template_manager::get_instance().get_str_by_string_template_id(90090915);
			contentText = template_manager::get_instance().get_str_by_string_template_id(90090916);
			SenderName = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			break;
		}
		break;
		default:
			break;
		}


		send_mail_to_another_player_by_system(
			target_player_guid,
			sender_guid,
			receiver_server_id,
			SenderName,
			title,
			contentText,
			money_typ1,
			money_num1,
			money_typ2,
			money_num2,
			item_list,
			item_num);
	}
	void globle_data::send_mail_to_another_player_by_system(
		guid_64 target_player_guid,
		guid_64 sender_guid,
		int32 receiver_server_id,
		std::string SenderName,
		std::string title,
		std::string contentText,
		int32 money_typ1,
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2,
		const s_item_info* item_list,
		int32 item_num)
	{
		if (!target_player_guid.is_valid())
		{
			return;
		}
		if (item_num > max_item_per_mail)
		{
			return;
		}

		cs2ws_send_mail send_mail_info;
		send_mail_info.receiver_server_id = receiver_server_id;
		std::vector<guid_64> temp_item_guid_array;
		if (item_list)
		{
			for (int32 i = 0; i < item_num; ++i)
			{
				if (item_list[i].item_guid.is_valid())
				{
					temp_item_guid_array.push_back(item_list[i].item_guid);
				}
			}
		}

		init_unit::init_mail_data_array(send_mail_info.mail_info, temp_item_guid_array, money_typ1, money_num1, money_typ2, money_num2, sender_guid);

		int32 string_len = 0;
		if (contentText.length() > max_text_contents_length)
		{
			string_len = max_text_contents_length;
		}
		else
		{
			string_len = contentText.length();
		}
		memcpy(send_mail_info.mail_info.text_contents, contentText.c_str(), string_len);

		if (title.length() > max_mail_title_length)
		{
			string_len = max_mail_title_length;
		}
		else
		{
			string_len = title.length();
		}
		memcpy(send_mail_info.mail_info.mail_title, title.c_str(), string_len);

		if (SenderName.length() > max_name_size)
		{
			string_len = max_name_size;
		}
		else
		{
			string_len = SenderName.length();
		}
		memcpy(send_mail_info.mail_info.sender_name, SenderName.c_str(), string_len);



		//物品数据
		if (item_list)
		{
			for (int32 i = 0; i < item_num; ++i)
			{
				if (!item_list[i].item_guid.is_valid())
				{
					continue;
				}
				send_mail_info.item_list[i].item_guid = item_list[i].item_guid;
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					send_mail_info.item_list[i].data_ary[j] = item_list[i].data_ary[j];

					//设置为 邮件包/-1格子
					if (j == e_item_info_container_type)
					{
						send_mail_info.item_list[i].data_ary[j] = e_bag_type_mail;
					}
					else if (j == e_item_info_slot)
					{
						send_mail_info.item_list[i].data_ary[j] = -1;
					}
				}
				send_mail_info.item_num++;
			}
		}

		send_mail_info.addressee_guid = target_player_guid;

		server_log::send_mail_log(target_player_guid.server_64, sender_guid.server_64, title, contentText, money_typ1, money_num1, money_typ2, money_num2, item_list, item_num);


		m_send_ws(&send_mail_info, sizeof(send_mail_info), receiver_server_id);
	}
	void globle_data::send_mail_with_item(
		guid_64 player_guid,
		int32 receiver_server_id,
		const std::vector<s_item_template_info>& item_list,
		std::string title,
		std::string contentText,
		int32 money_typ1,
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2)
	{
		if (item_list.empty() && money_num1 <= 0)
		{
			CONSOLE_ERROR("send_mail_with_item error: drop_item_size <= 0 && money_num1 <= 0");
			return;
		}
		if (item_list.empty())
		{
			send_mail_to_another_player_by_system(player_guid, player_guid, receiver_server_id, "", title, contentText, money_typ1, money_num1, money_typ2, money_num2, nullptr, 0);
			return;
		}
		s_item_info mail_item_info_array[max_item_per_mail];
		int32 item_in_mail_index = 0;
		for (auto& it : item_list)
		{
			s_item_info& temp_item_info = mail_item_info_array[item_in_mail_index];
			init_unit::init_item_data(temp_item_info, it.m_item_id, -1, it.m_item_num, e_bag_type_bag, 0, it.m_lock, 0, 0);
			if (item_in_mail_index == max_item_per_mail - 1)
			{
				//一封邮件满了，先发一下
				send_mail_to_another_player_by_system(player_guid, player_guid, receiver_server_id, "", title, contentText, money_typ1, money_num1, money_typ2, money_num2, mail_item_info_array, max_item_per_mail);
				memset(mail_item_info_array, 0, sizeof(mail_item_info_array));
				item_in_mail_index = 0;
			}
			else
			{
				item_in_mail_index++;
			}
		}

		//循环下来，邮件物品数组中还有东西，就再发一下
		if (item_in_mail_index > 0)
		{
			send_mail_to_another_player_by_system(player_guid, player_guid, receiver_server_id, "", title, contentText, money_typ1, money_num1, money_typ2, money_num2, mail_item_info_array, max_item_per_mail);
		}
	}

	const xchar* globle_data::get_mail_common_text_id(e_mail_common_text id_index)
	{
		if (id_index < 0 || id_index >= e_mail_common_text_max)
		{
			return "";
		}
		return mail_common_text_array[id_index];
	}
	const xchar* globle_data::get_mail_common_text(int32 id_index)
	{
		if (id_index >= e_mail_common_text_max)
		{
			return "";
		}
		return mail_common_text_array[id_index];
	}

	void globle_data::merge_money_to_array(std::vector<int32>& money_array, int32 new_money_id, int32 new_money_value)
	{
		if (money_array.size() % 2 != 0
			|| new_money_value == 0)
		{
			return;
		}
		int32 cur_money_num = money_array.size() / 2;
		int32 merge_index = -1;
		for (int32 i = 0; i < cur_money_num; ++i)
		{
			if (money_array[i * 2] == new_money_id)
			{
				merge_index = i;
			}
		}
		if (merge_index >= 0
			&& merge_index < cur_money_num)
		{
			money_array[merge_index * 2 + 1] += new_money_value;
		}
		else
		{
			money_array.push_back(new_money_id);
			money_array.push_back(new_money_value);
		}
	}

	void globle_data::add_to_drop_list(std::vector<s_drop_list>& drop_list, const guid_64& ref_player_guid, s_item_template_info& _drop_list)
	{
		for (auto& it : drop_list)
		{
			if (it.drop_owner_guid == ref_player_guid)
			{
				std::vector<s_item_template_info>& drop_list_num = it.drop_list_vec;
				drop_list_num.push_back(_drop_list);
				return;
			}
		}

		std::vector<s_item_template_info> new_drop_list_with_num;
		new_drop_list_with_num.push_back(_drop_list);
		s_drop_list new_drop_list;
		new_drop_list.drop_owner_guid = ref_player_guid;
		new_drop_list.drop_list_vec = new_drop_list_with_num;

		drop_list.push_back(new_drop_list);
	}

	void globle_data::add_to_drop_list(std::vector<s_item_template_info>& drop_list, s_item_template_info& drop_info)
	{
		for (auto& it : drop_list)
		{
			if (it.m_item_id == drop_info.m_item_id)
			{
				it.m_item_num += drop_info.m_item_num;
				return;
			}
		}
		drop_list.push_back(drop_info);
	}

	void globle_data::get_item_list_by_drop_bg(int32 drop_bg_id, std::vector<s_item_template_info>& _drop_list_with_num, int32 recursion_times, int32 drop_level)
	{
		recursion_times++;
		if (recursion_times >= 100)
		{
			return;
		}
		//_drop_list_with_num.clear();
		DropTemplate* drop_ptr = GET_TEMPLATE(DropTemplate, drop_bg_id);
		if (nullptr == drop_ptr)
		{
			return;
		}

		if (drop_ptr->LevelChangeId.size() > 0 && drop_ptr->LevelChangeId.size() % 3 == 0 && drop_level >= 0)
		{
			std::vector<int32> level_change_id_arr = drop_ptr->LevelChangeId;
			for (int32 i = 0; i < level_change_id_arr.size() / 3; i++)
			{
				if (level_change_id_arr[i * 3] <= drop_level && level_change_id_arr[i * 3 + 1] >= drop_level)
				{
					drop_ptr = GET_TEMPLATE(DropTemplate, level_change_id_arr[i * 3 + 2]);
					break;
				}
			}
			if (nullptr == drop_ptr)
			{
				return;
			}
		}
		if (drop_ptr->IDs.size() <= 0)
		{
			return;
		}
		int32 size_id = drop_ptr->IDs.size();
		int32 size_idtype = drop_ptr->IDTypes.size();
		int32 size_count = drop_ptr->Counts.size();
		int32 size_bound = drop_ptr->Bounds.size();
		int32 size_weight = drop_ptr->Weights.size();
		bool size_equ = (size_id == size_idtype) && (size_id * 2 == size_count) && (size_id == size_bound) && (size_id == size_weight);
		if (false == size_equ)
		{
			CONSOLE_ERROR("drop_ptr->IDs.size():{}, drop_ptr->IDTypes.size():{}, drop_ptr->Counts.size():{}, drop_ptr->Bounds.size():{}, drop_ptr->Weights.size():{}", size_id, size_idtype, size_count, size_bound, size_weight);
			return;//表填错了 列的数量对不上
		}

		std::vector<int32> need_drop_id_types;
		std::vector<int32> need_drop_ids;
		std::vector<int32> need_drop_cnts;
		std::vector<int32> need_drop_bounds;
		std::vector<int32> need_drop_weights;

		int32 drop_type = drop_ptr->DropType;
		switch (drop_type)
		{
		case e_dropbox_ran_type_all:
		{
			need_drop_id_types = drop_ptr->IDTypes;
			need_drop_ids = drop_ptr->IDs;
			need_drop_cnts = drop_ptr->Counts;
			need_drop_bounds = drop_ptr->Bounds;
			need_drop_weights = drop_ptr->Weights;
			break;
		}
		case e_dropbox_ran_type_one:
		{
			int32 choosed_item_index = -1;
			int32 total_weight = 0;
			for (int32 i = 0; i < size_weight; i++)
			{
				total_weight += drop_ptr->Weights[i];
			}
			if (total_weight < DROP_MAX_WEIGHT)
			{
				total_weight = DROP_MAX_WEIGHT;
			}

			int32 random_value = random_gen::get_random(0, total_weight);
			int32 cur_weight = 0;
			for (int32 i = 0; i < size_weight; i++)
			{
				cur_weight += drop_ptr->Weights[i];
				if (cur_weight >= random_value)
				{
					choosed_item_index = i;
					break;
				}
			}
			if (choosed_item_index < 0)
			{
				return;
			}
			need_drop_id_types.push_back(drop_ptr->IDTypes[choosed_item_index]);
			need_drop_ids.push_back(drop_ptr->IDs[choosed_item_index]);
			need_drop_cnts.push_back(drop_ptr->Counts[2 * choosed_item_index]);
			need_drop_cnts.push_back(drop_ptr->Counts[2 * choosed_item_index + 1]);
			need_drop_bounds.push_back(drop_ptr->Bounds[choosed_item_index]);
			break;
		}
		default:
			break;
		}
		if (drop_type == e_dropbox_ran_type_one)
		{
			int32 drop_num = need_drop_ids.size();
			for (int32 i = 0; i < drop_num; i++)
			{
				if (i >= need_drop_id_types.size() || i >= need_drop_bounds.size() || i * 2 + 1 >= need_drop_cnts.size())
				{
					break;
				}
				int32 drop_item_num = random_gen::get_random(need_drop_cnts[i * 2], need_drop_cnts[i * 2 + 1]);
				int32 temp_type = need_drop_id_types[i];
				int32 drop_item_lock = need_drop_bounds[i];
				if (e_dropbox_typ_drop_box == temp_type)
				{
					for (int32 j = 0; j < drop_item_num; j++)
					{
						get_item_list_by_drop_bg(need_drop_ids[i], _drop_list_with_num, recursion_times, drop_level);
					}
				}
				else if (e_dropbox_typ_item == temp_type)
				{
					_drop_list_with_num.push_back(s_item_template_info(need_drop_ids[i], drop_item_num, drop_item_lock));
				}
				else if (e_dropbox_typ_money == temp_type)
				{
					_drop_list_with_num.push_back(s_item_template_info(need_drop_ids[i], drop_item_num, drop_item_lock));
				}
			}
		}
		if (drop_type == e_dropbox_ran_type_all)
		{
			int32 drop_num = need_drop_ids.size();
			for (int32 i = 0; i < drop_num; i++)
			{
				if (i >= need_drop_id_types.size() || i >= need_drop_bounds.size() || i * 2 + 1 >= need_drop_cnts.size() || i >= need_drop_weights.size())
				{
					break;
				}

				int32 temp_weight = need_drop_weights[i];
				int32 random = random_gen::get_random(1, DROP_MAX_WEIGHT);
				if (temp_weight < random)
				{
					continue;
				}

				int32 drop_item_num = random_gen::get_random(need_drop_cnts[i * 2], need_drop_cnts[i * 2 + 1]);
				int32 temp_type = need_drop_id_types[i];
				int32 drop_item_lock = need_drop_bounds[i];
				if (e_dropbox_typ_drop_box == temp_type)
				{
					for (int32 j = 0; j < drop_item_num; j++)
					{
						get_item_list_by_drop_bg(need_drop_ids[i], _drop_list_with_num, recursion_times, drop_level);
					}
				}
				else if (e_dropbox_typ_item == temp_type)
				{
					_drop_list_with_num.push_back(s_item_template_info(need_drop_ids[i], drop_item_num, drop_item_lock));
				}
				else if (e_dropbox_typ_money == temp_type)
				{
					_drop_list_with_num.push_back(s_item_template_info(need_drop_ids[i], drop_item_num, drop_item_lock));
				}
			}
		}
	}

	void globle_data::get_item_list_by_rank_rwd(int32 _rank, std::vector<int32>& rank_rwd_list, std::vector<s_item_template_info>& _drop_list_with_num, int32 drop_level)
	{
		_drop_list_with_num.clear();
		int32 rwd_data_len = rank_rwd_list.size();
		if (rwd_data_len <= 0 || rwd_data_len % 3 != 0)
		{
			return;
		}

		int32 rank_low = 0;
		int32 rank_high = 0;
		int32 drop_bg = 0;
		for (int32 i = 0; i < rwd_data_len; i += 3)
		{
			rank_low = rank_rwd_list[i];
			rank_high = rank_rwd_list[i + 1];
			if (_rank < rank_low || _rank > rank_high)
			{
				continue;
			}
			drop_bg = rank_rwd_list[i + 2];
			break;
		}

		if (drop_bg <= 0)
		{
			return;
		}
		get_item_list_by_drop_bg(drop_bg, _drop_list_with_num, 0, drop_level);
	}

	int32 globle_data::get_drop_bag_by_rank_rwd(int32 _rank, std::vector<int32>& rank_rwd_list)
	{
		int32 rwd_data_len = rank_rwd_list.size();
		if (rwd_data_len <= 0 || rwd_data_len % 3 != 0)
		{
			return 0;
		}

		int32 rank_low = 0;
		int32 rank_high = 0;
		int32 drop_bg = 0;
		for (int32 i = 0; i < rwd_data_len; i += 3)
		{
			rank_low = rank_rwd_list[i];
			rank_high = rank_rwd_list[i + 1];
			if (_rank < rank_low || _rank > rank_high)
			{
				continue;
			}
			drop_bg = rank_rwd_list[i + 2];
			break;
		}

		return drop_bg;
	}

	std::vector<int32> globle_data::get_item_by_class_type(const e_class_type class_type, const std::vector<int32>& item_array, const int32& item_locked_state)
	{
		std::vector<int32> result_item_array;
		const int32 item_array_size = item_array.size();
		if (0 != item_array_size % 2)
		{
			return result_item_array;
		}

		for (int32 item_array_index = 0; item_array_index < item_array_size; item_array_index += 2)
		{
			const int32 item_id = item_array[item_array_index];
			const int32 item_num = item_array[item_array_index + 1];
			ItemTemplate* item_template_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(item_id);
			if (nullptr == item_template_ptr)
			{
				continue;
			}
			const int32 class_type_array_size = item_template_ptr->class_type.size();
			if (0 == class_type_array_size)
			{
				result_item_array.push_back(item_id);
				result_item_array.push_back(item_num);
				result_item_array.push_back(item_locked_state);
				continue;
			}
			for (int32 class_type_array_index = 0; class_type_array_index < class_type_array_size; class_type_array_index++)
			{
				if ((e_class_type)item_template_ptr->class_type[class_type_array_index] == class_type)
				{
					result_item_array.push_back(item_id);
					result_item_array.push_back(item_num);
					result_item_array.push_back(item_locked_state);
					break;
				}
			}
		}
		return result_item_array;
	}

	void globle_data::get_exp_add_percent(int32 player_level, int32 down_level, int32 up_level, float& add_percent, int32& buff_template_id)
	{
		add_percent = 0;
		buff_template_id = 0;

		if (GAMECONFIG->ExpDownServerLevel.size() % 3 != 0 || GAMECONFIG->ExpDownServerLevel.size() < 6)
		{
			return;
		}
		if (GAMECONFIG->ExpUpServerLevel.size() % 3 != 0 || GAMECONFIG->ExpUpServerLevel.size() < 6)
		{
			return;
		}

		//先给个默认值
		add_percent = GAMECONFIG->ExpUpServerLevel[1] / 100.f;
		buff_template_id = GAMECONFIG->ExpUpServerLevel[2];
		if (player_level <= GAMECONFIG->ExpServerEndLevel)
		{
			return;
		}

		if (player_level < down_level)
		{
			int32 diff_level = down_level - player_level;
			for (int32 i = 3; i < GAMECONFIG->ExpDownServerLevel.size(); i = i + 3)
			{
				if (diff_level >= GAMECONFIG->ExpDownServerLevel[i - 3] && diff_level < GAMECONFIG->ExpDownServerLevel[i])
				{
					add_percent = GAMECONFIG->ExpDownServerLevel[i + 1] / 100.f;
					buff_template_id = GAMECONFIG->ExpDownServerLevel[i + 2];
					return;
				}
			}
		}
		else if (player_level > up_level)
		{
			int32 diff_level = player_level - up_level;
			for (int32 i = 3; i < GAMECONFIG->ExpUpServerLevel.size(); i = i + 3)
			{
				if (diff_level >= GAMECONFIG->ExpUpServerLevel[i - 3] && diff_level < GAMECONFIG->ExpUpServerLevel[i])
				{
					add_percent = -GAMECONFIG->ExpUpServerLevel[i + 1] / 100.f;
					buff_template_id = GAMECONFIG->ExpUpServerLevel[i + 2];
					return;
				}
			}
		}
		else
		{
		}
	}

	int32 globle_data::get_rwd_num_by_rank_rwd(int32 _rank, std::vector<int32>& rank_rwd_list)
	{
		int32 rwd_data_len = rank_rwd_list.size();
		if (rwd_data_len <= 0 || rwd_data_len % 3 != 0)
		{
			return 0;
		}

		int32 rank_low = 0;
		int32 rank_high = 0;
		int32 rank_num = 0;
		for (int32 i = 0; i < rwd_data_len; i += 3)
		{
			rank_low = rank_rwd_list[i];
			rank_high = rank_rwd_list[i + 1];
			if (_rank < rank_low || _rank > rank_high)
			{
				continue;
			}
			rank_num = rank_rwd_list[i + 2];
			break;
		}

		return rank_num;
	}

	void globle_data::set_server_days()
	{
		m_on_server_day = get_server_on_days();
	}

	int32 globle_data::get_server_on_days(int32 time_stamp)
	{
		const tm& server_on_time = globle_data::get_instance().get_server_open_tm();
		int32 server_on_year = server_on_time.tm_year;
		int32 server_on_yday = server_on_time.tm_yday;

		tm* local_time = nullptr;
		if (time_stamp < 0)
		{
			local_time = time_helper::get_localtime();
		}
		else
		{
			local_time = time_helper::get_tm_by_time(time_stamp);
			local_time->tm_year -= 1900;
		}
		if (nullptr == local_time)
		{
			return 0;
		}
		int32 local_year = local_time->tm_year;
		int32 local_yday = local_time->tm_yday;

		int32 days = 0;
		if (server_on_year == local_year)
		{
			days = local_yday - server_on_yday;
		}
		else if (server_on_year > local_year)
		{
			return 0;
		}
		else {

			days += time_helper::get_days_of_year(server_on_year) - server_on_yday; //开服年剩余天数
			days += local_yday;	//当前年已过天数
			for (int32 year = server_on_year + 1; year < local_year; year++)
			{
				days += time_helper::get_days_of_year(year);	// 中间年的天数
			}
		}
		if (days < 0)
		{
			days = 0;
		}
		return days + 1;
	}

	bool globle_data::make_auction_info(s_auction_info& target_info, s_item_info item_info, int32 sell_num, int32 trade_type,
		guid_64 seller_guid, std::string seller_name, int32 seller_vip_level,
		int32 sell_money_type, int64 sell_total_money)
	{
		if (GAMECONFIG == nullptr
			|| m_region_template_ptr == nullptr)
		{
			return false;
		}

		if (!item_info.item_guid.is_valid()
			|| seller_name.length() >= max_name_size)
		{
			return false;
		}

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return false;
		}
		if (sell_num <= 0)
		{
			sell_num = item_info.data_ary[e_item_info_stack_count];
		}
		if (item_info.data_ary[e_item_info_stack_count] < sell_num)
		{
			return false;
		}

		target_info.clear_data();

		target_info.seller_guid = seller_guid;
		target_info.trade_type = trade_type;

		int32 real_name_len = seller_name.length();
		memcpy(target_info.seller_name, seller_name.c_str(), real_name_len);
		target_info.seller_vip_level = seller_vip_level;

		target_info.item_color = item_template_ptr->item_color;
		{
			EquipTemplate* equip_template_ptr = GET_TEMPLATE(EquipTemplate, item_template_ptr->logic_id);
			if (equip_template_ptr == nullptr)
			{
				target_info.quality_level = 0;
			}
			else
			{
				//target_info.quality_level = equip_template_ptr->quality_level;
			}
		}
		if (item_template_ptr->class_type.size() > 0)
		{
			target_info.class_limit = item_template_ptr->class_type[0];
		}
		else
		{
			target_info.class_limit = e_class_type_none;
		}
		const std::vector<std::string>& temp_array_list = template_manager::get_instance().get_string_list(item_template_ptr->ItemName);
		if (temp_array_list.empty())
		{
			return false;
		}
		std::string temp_item_name;
		if (temp_array_list.size() == 2)
		{
			temp_item_name = temp_array_list[1];
		}
		else
		{
			temp_item_name = temp_array_list[0];
		}
		int32 real_item_name_len = temp_item_name.length();
		if (real_item_name_len > max_item_name_size)
		{
			real_item_name_len = max_item_name_size;
		}
		memcpy(target_info.item_name, temp_item_name.c_str(), real_item_name_len);

		target_info.sell_time = utility::get_tick_count();
		target_info.deal_time = 0;

		int32 is_sky_order = 0;


		if (item_template_ptr->item_type == e_item_type_expendable &&
			item_template_ptr->sub_type == e_prop_sub_type_gife &&
			item_template_ptr->item_color == e_item_color_red)
		{
			is_sky_order = 1;
	    }
		if (item_template_ptr->item_type == e_item_type_expendable &&
			item_template_ptr->sub_type == e_prop_sub_type_grade_up &&
			item_template_ptr->item_color == e_item_color_orange)
		{
			is_sky_order = -1;
		}

		switch (trade_type)
		{
		case e_trading_type_normal_sell:
		{

			target_info.sell_money_type = sell_money_type;
			target_info.sell_total_money = sell_total_money;
			target_info.sell_single_money = round(double(sell_total_money) / sell_num);
			target_info.auction_type = item_template_ptr->AuctionLabType;
			target_info.auction_sub_type = item_template_ptr->AuctionLabSubType;
			target_info.is_sky_order = is_sky_order;
		}
		break;
		case e_trading_type_legion:
		{
			int32 region_money_type_index = m_region_template_ptr->RegionCode * 2;
			int32 region_money_num_index = m_region_template_ptr->RegionCode * 2 + 1;

			if (item_template_ptr->TradingBaseMoneyArray.size() < region_money_num_index
				|| item_template_ptr->TradingFinalMoneyArray.size() < region_money_num_index)
			{
				return false;
			}
			target_info.sell_money_type = item_template_ptr->TradingFinalMoneyArray[region_money_type_index];
			target_info.sell_total_money = item_template_ptr->TradingFinalMoneyArray[region_money_num_index] * sell_num;
			target_info.sell_single_money = 0;
			target_info.auction_type = item_template_ptr->TradingLabType;
			target_info.auction_sub_type = item_template_ptr->TradingSubLabType;
			target_info.cur_bid_money = item_template_ptr->TradingBaseMoneyArray[region_money_num_index] * sell_num;
			target_info.deal_time = init_unit::get_end_time(GAMECONFIG->LegionAuctionItemLiveTime) + (300 * second_tick_time);
			target_info.is_show = 1;
			target_info.is_sky_order = is_sky_order;
		}
		break;
		case e_trading_type_world:
		{
			int32 region_money_type_index = m_region_template_ptr->RegionCode * 2;
			int32 region_money_num_index = m_region_template_ptr->RegionCode * 2 + 1;

			if (item_template_ptr->TradingBaseMoneyArray.size() < region_money_num_index
				|| item_template_ptr->TradingFinalMoneyArray.size() < region_money_num_index)
			{
				return false;
			}
			target_info.sell_money_type = item_template_ptr->TradingFinalMoneyArray[region_money_type_index];
			target_info.sell_total_money = item_template_ptr->TradingFinalMoneyArray[region_money_num_index] * sell_num;
			target_info.sell_single_money = 0;
			target_info.auction_type = item_template_ptr->TradingLabType;
			target_info.auction_sub_type = item_template_ptr->TradingSubLabType;
			target_info.cur_bid_money = item_template_ptr->TradingBaseMoneyArray[region_money_num_index] * sell_num;
			target_info.deal_time = init_unit::get_end_time(item_template_ptr->TradingAuctionTime + item_template_ptr->TradingAwaitTime);
			target_info.is_show = 1;
			target_info.is_sky_order = is_sky_order;
		}
		break;
		default:
			break;
		}

		target_info.item_info.item_guid = guid_gen::make_guid(seller_guid);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			if (i == e_item_info_stack_count)
			{
				target_info.item_info.data_ary[i] = sell_num;
			}
			else if (i == e_item_info_container_type)
			{
				target_info.item_info.data_ary[i] = e_bag_type_none;
			}
			else
			{
				target_info.item_info.data_ary[i] = item_info.data_ary[i];
			}
		}
		return true;
	}
	std::vector<int32> globle_data::get_extra_buy_money_arr(int32 map_template_id)
	{
		std::vector<int32> money_arr;
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		RegionTemplate* region_ptr = get_region_template_ptr();
		if (nullptr == map_ptr || nullptr == region_ptr || map_ptr->ExtraBuyCountNeedMoneyStr.size() <= 0)
		{
			return money_arr;
		}
		std::vector<std::string>  money_str = map_ptr->ExtraBuyCountNeedMoneyStr;
		int32 str_len = map_ptr->ExtraBuyCountNeedMoneyStr.size();
		for (int32 i = 0; i < str_len; ++i)
		{
			std::string map_str = money_str[i];
			if (map_str.length() > 0)
			{
				if (init_unit::change_string_to_i32(map_str.substr(0, 1)) == region_ptr->RegionCode)
				{
					money_arr = get_int_arr_by_str_arr(map_str.assign(map_str.begin() + 2, map_str.end()));
					break;
				}
			}
		}
		return money_arr;
	}

	std::vector<int32> globle_data::get_int_arr_by_str_arr(std::string str_arr)
	{
		std::vector<int32> num_arr;
		int32 pos = 0;
		int32 end_index = 0;
		int32 str_len = str_arr.length();
		for (int32 i = 0 ; i < str_len; ++i)
		{
			pos = str_arr.find("-", i);
			if (pos == 0)
			{
				continue;
			}
			if (pos == -1)
			{
				num_arr.push_back(init_unit::change_string_to_i32(str_arr.substr(i, str_len - i)));
				break;
			}
			if (pos > 0)
			{
				num_arr.push_back(init_unit::change_string_to_i32(str_arr.substr(i, pos - i)));
				i = pos;
			}
		}
		return num_arr;
	}
	xstring globle_data::get_money_str_by_type(int32 money_type)
	{
		xstring money_str;
		MoneyTemplate* money_ptr = template_manager::get_instance().get_money_template_by_type(money_type);
		if (nullptr == money_ptr)
		{
			return money_str;
		}
		money_str = template_manager::get_instance().get_str_by_string_template_id(money_ptr->NameId);
		return money_str;
	}
	xstring globle_data::get_class_name_by_type(int32 class_type)
	{
		xstring class_name = "";
		if (class_type < 1)
		{
			return class_name;
		}
		if (GAMECONFIG->CreateCareerNameId.size() < class_type * 5)
		{
			return class_name;
		}
		class_name = template_manager::get_instance().get_str_by_string_template_id(GAMECONFIG->CreateCareerNameId[(class_type - 1) * 5]);

		return class_name;
	}
	int32 globle_data::get_legion_boss_temp_id()
	{
		ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_boss);
		if (nullptr == activity_template_ptr)
		{
			return -1;
		}
		if (activity_template_ptr->ParamIntArr4.size() < 7)
		{
			return -1;
		}
		int32 wdays = time_helper::get_today_zero_time_info().day_in_week;
		int32 bosstemplateID = -1;

		if (wdays >= 0 && wdays < 7)
		{
			bosstemplateID = activity_template_ptr->ParamIntArr4[wdays];
			return bosstemplateID;
		}
		return -1;
	}
}