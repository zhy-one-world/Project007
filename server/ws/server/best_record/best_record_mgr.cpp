#include "best_record_mgr.h"
#include "template/template_manager.h"
#include "utility/cs_date.hpp"
#include "server/mail/event_ws_mgr.h"
#include "utility/init_unit.h"
#include "logic/activity_def.hpp"
#include "lua/script_mgr.h"
#include "server/team/team_ws_mgr.h"
#include "net.pb.h"

namespace faith
{
	void best_record_mgr::save_best_record_to_db(const s_best_record& best_record)
	{
		if (best_record.map_template_id <= 0)
		{
			return;
		}
		server2dp_proto_ws2dp_save_best_record msg;
		server2dp_proto_s_best_record* record_info = msg.mutable_record_info();
		record_info->set_map_template_id(best_record.map_template_id);
		for (int32 i = 0; i < faith::single_map_record_num; i++)
		{
			record_info->add_role_guid(best_record.single_map_best_record[i].role_guid.server_64);
			record_info->add_role_name(best_record.single_map_best_record[i].role_name);
			record_info->add_role_value(best_record.single_map_best_record[i].value);
		}
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_best_record);

		//ws2dp_save_best_record save_record_msg;
		//save_record_msg.record_info = best_record;
		//ws_client::getInstance().send_to_dp(&save_record_msg, sizeof(save_record_msg));
	}

	void best_record_mgr::load_all_best_record_from_db()
	{
		m_activity_record_list.clear();
		ws2dp_load_all_best_record load_all_record_msg;
		ws_client::getInstance().send_to_dp(&load_all_record_msg, sizeof(load_all_record_msg));
	}


	s_best_record* best_record_mgr::get_record_by_template_id(int32 map_template_id)
	{
		int32 best_record_index = -1;
		for (int32 i = 0; i < m_activity_record_list.size(); i++)
		{
			if (m_activity_record_list[i].map_template_id == map_template_id)
			{
				best_record_index = i;
				break;
			}
		}
		if (best_record_index < 0)
		{
			return create_record_by_template_id(map_template_id);
		}
		return &(m_activity_record_list[best_record_index]);
	}

	s_best_record* best_record_mgr::create_record_by_template_id(int32 map_template_id)
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return nullptr;
		}
		int32 order_type = map_template_ptr->BestRecordOrderType;
		if (order_type <= 0)
		{
			return nullptr;
		}
		s_best_record new_best_record;
		new_best_record.map_template_id = map_template_id;
		m_activity_record_list.push_back(new_best_record);
		int32 size_activity_record_list = m_activity_record_list.size();
		return &(m_activity_record_list[size_activity_record_list - 1]);
	}

	int32	best_record_mgr::get_order_type_by_template_id(int32 map_template_id)
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return -1;
		}
		return map_template_ptr->BestRecordOrderType;
	}

	int32	best_record_mgr::get_convert_map_template_id(int32 map_template_id)	//活动副本中最好记录改为世界统一,统一设置map_template_id
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return -1;
		}
		if (map_template_ptr->Type == faith::e_map_type_broken_sky || map_template_ptr->Type == faith::e_map_type_king_of_pk || map_template_ptr->Type == e_map_type_temple || map_template_ptr->Type == e_map_type_empire_treasure)
		{
			return (map_template_id / 100 * 100 + 1);
		}
		return map_template_id;
	}
	int32	best_record_mgr::get_insert_index(s_best_record* best_record, const s_single_map_record& new_record, int32 order_type)
	{
		int32 insert_index = 0;
		if (best_record == nullptr)
		{
			return -1;
		}
		if (order_type <= 0 || order_type >= faith::e_best_record_order_max)
		{
			return -1;
		}
		for (int32 i = faith::single_map_record_num - 1; i >= 0; i--)
		{
			if (best_record->single_map_best_record[i].role_guid.is_valid() == false)
			{
				continue;
			}
			if (order_type == faith::e_best_record_order_by_bigger)
			{
				if (best_record->single_map_best_record[i].value >= new_record.value)
				{
					insert_index = i + 1;
					break;
				}
			}
			else
			{
				if (best_record->single_map_best_record[i].value <= new_record.value)
				{
					insert_index = i + 1;
					break;
				}
			}
		}
		return insert_index;
	}
	bool best_record_mgr::judget_if_can_add(s_best_record* best_record, const s_single_map_record& new_record)
	{
		if (best_record == nullptr)
		{
			return false;
		}
		if (new_record.role_guid.is_valid() == false)
		{
			return false;
		}
		int32 order_type = get_order_type_by_template_id(best_record->map_template_id);
		if (order_type <= 0 || order_type >= faith::e_best_record_order_max)
		{
			return false;
		}
		int32 insert_index = get_insert_index(best_record, new_record, order_type);
		if (insert_index < 0 || insert_index >= faith::single_map_record_num)
		{
			return false;
		}
		for (int32 i = 0; i < faith::single_map_record_num; i++)
		{
			if (best_record->single_map_best_record[i].role_guid == new_record.role_guid)
			{
				if (order_type == faith::e_best_record_order_by_bigger)
				{
					if (new_record.value <= best_record->single_map_best_record[i].value)
					{
						return false;
					}
					else
					{
						return true;
					}
				}
				else if (order_type == faith::e_best_record_order_by_smaller)
				{
					if (new_record.value >= best_record->single_map_best_record[i].value)
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
		return true;
	}

	void best_record_mgr::insert_record(s_best_record* best_record, const s_single_map_record& new_record, bool is_from_db)
	{
		if (best_record == nullptr)
		{
			return;
		}
		if (new_record.role_guid.is_valid() == false)
		{
			return;
		}
		int32 order_type = get_order_type_by_template_id(best_record->map_template_id);
		int32 insert_index = get_insert_index(best_record, new_record, order_type);
		if (insert_index < 0 || insert_index >= single_map_record_num)
		{
			return;
		}
		for (int32 i = faith::single_map_record_num - 1; i > insert_index; i--)
		{
			best_record->single_map_best_record[i] = best_record->single_map_best_record[i - 1];
		}
		best_record->single_map_best_record[insert_index] = new_record;
		if (!is_from_db)
		{
			save_best_record_to_db(*best_record);
		}
	}
	void best_record_mgr::del_record(s_best_record* best_record, const s_single_map_record& new_record)
	{
		int32 del_index = -1;
		if (best_record == nullptr)
		{
			return;
		}
		if (new_record.role_guid.is_valid() == false)
		{
			return;
		}
		for (int32 i = 0; i < faith::single_map_record_num; i++)
		{
			if (best_record->single_map_best_record[i].role_guid == new_record.role_guid)
			{
				del_index = i;
				break;
			}
		}
		if (del_index < 0)
		{
			return;
		}
		if (del_index < faith::single_map_record_num - 1)
		{
			for (int32 i = del_index; i < faith::single_map_record_num - 1; i++)
			{
				best_record->single_map_best_record[i] = best_record->single_map_best_record[i + 1];
			}
		}
		best_record->single_map_best_record[faith::single_map_record_num - 1].clear_data();
	}

	void best_record_mgr::set_best_record(int32 map_template_id, s_single_map_record new_record, bool is_from_db)
	{
		team_best_record_role_name_replace(map_template_id, new_record);
		s_best_record* best_record = get_record_by_template_id(map_template_id);
		if (!judget_if_can_add(best_record, new_record))
		{
			return;
		}
		if (!is_from_db)
		{
			del_record(best_record, new_record);
		}
		insert_record(best_record, new_record, is_from_db);

		if (!is_from_db)
		{//更新副本最快纪录的公告
			notice_best_record(map_template_id, new_record);
		}
		
	}

	void best_record_mgr::notice_best_record(int32 map_template_id, const s_single_map_record& best_record)
	{
		s_best_record* best_record_str = get_record_by_template_id(map_template_id);
		if (nullptr == best_record_str)
		{
			return;
		}
		if (best_record_str->single_map_best_record[0].role_guid != best_record.role_guid)//不是第一名不发公告
		{
			return;
		}
		MapTemplate* map_template_ptr = template_manager::get_instance().get_map_template_map_id(map_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}
		std::string map_name = "";
		if (!template_manager::get_instance().get_str_by_attribute_id(map_name, map_template_ptr->MapName))
		{
			return;
		}
		int32 notice_id = map_template_ptr->Noticeld;		//93000034
		float best_value = (float)best_record.value;
		if (map_template_ptr->Type == e_map_type_exp_fuben && best_value >= 10000)
		{
			notice_id = exp_best_record_hundred_million_notice_id;
			//保留两位小数
			int32 temp_best_value = best_value / 100;
			best_value = (float)temp_best_value / 100;
		}
		if (notice_id <= 0)
		{
			return;
		}
		
		std::string player_name_str = "";
		team_ws* team = team_ws_mgr::get_instance().get_team(best_record.role_guid);
		if (nullptr != team  &&  is_multiple_raid((e_map_order_type)map_template_ptr->Order))
		{
			const std::list<s_team_member_info>& vec_team_mem = team->get_member_list();
			int32 player_num = vec_team_mem.size();
			int32 plyaer_idx = 0;
			for (const s_team_member_info& team_mem : vec_team_mem)
			{
				if (plyaer_idx != (player_num - 1))
				{
					player_name_str = player_name_str + team_mem.role_name + " ";
				}
				else
				{
					player_name_str = player_name_str + team_mem.role_name;
				}
				plyaer_idx++;
			}
		}
		else
		{
			player_name_str = best_record.role_name;
		}
		std::vector<std::string> vec_str_time;
		if ((e_map_order_type)map_template_ptr->Order == e_map_order_type_story_raid || (e_map_order_type)map_template_ptr->Order == e_map_order_type_multiplayer_raid)
		{
			vec_str_time.push_back(get_map_difficulty_notice_id(map_template_id));
		}
		else
		{
			vec_str_time.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		}
		vec_str_time.push_back(player_name_str);
		vec_str_time.push_back(map_name);
		if (map_template_ptr->BestRecordOrderType == 2)
		{
			vec_str_time.push_back(template_manager::get_instance().int_to_string(best_record.value / 60));
			vec_str_time.push_back(template_manager::get_instance().int_to_string(best_record.value % 60));
		}
		else if (map_template_ptr->BestRecordOrderType == 1)
		{
			vec_str_time.push_back(template_manager::get_instance().float_to_string(best_value));
		}
		std::string notice_str = init_unit::implode(vec_str_time);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, best_record.role_guid, notice_str);
	}

	std::string best_record_mgr::get_map_difficulty_notice_id(int32 map_template_id)
	{
		MapTemplate* map_template_ptr = template_manager::get_instance().get_map_template_map_id(map_template_id);
		if (nullptr == map_template_ptr)
		{
			return std::string("");
		}
		switch ((e_map_order_type)map_template_ptr->Order)
		{
		case faith::e_map_order_type_story_raid:
			{
				switch ((e_map_difficulty)map_template_ptr->Difficulty)
				{
				case e_map_difficulty_easy:
					{
						return template_manager::get_instance().int_to_string(90300098);//普通
					}
					break;
				case e_map_difficulty_normal:
					{
						return template_manager::get_instance().int_to_string(90300099);//精英
					}
					break;
				case e_map_difficulty_heard:
					{
						return template_manager::get_instance().int_to_string(90300100);;//炼狱
					}
					break;
				default:
					{
						return std::string("");
					}
					break;
				}
			}
			break;
		case faith::e_map_order_type_multiplayer_raid:
			{
				switch ((e_map_difficulty_for_multiplayer)map_template_ptr->Difficulty)
				{
				case e_map_difficulty_for_multiplayer_easy:
					{
						return template_manager::get_instance().int_to_string(90300101);//简单
					}
					break;
				case e_map_difficulty_for_multiplayer_normal:
					{
						return template_manager::get_instance().int_to_string(90300102);//普通
					}
					break;
				case e_map_difficulty_for_multiplayer_hard:
					{
						return template_manager::get_instance().int_to_string(90300103);//困难
					}
					break;
				case e_map_difficulty_for_multiplayer_heroic:
					{
						return template_manager::get_instance().int_to_string(90300104);//英雄
					}
					break;
				case e_map_difficulty_for_multiplayer_hell:
					{
						return template_manager::get_instance().int_to_string(90300105);//地狱
					}
					break;
				default:
					{
						return std::string("");
					}
					break;
				}
			}
			break;
		default:
			{
				return std::string("");
			}
			break;
		}
	}
	bool best_record_mgr::is_multiple_raid(e_map_order_type map_type)
	{
		switch (map_type)
		{
		case e_map_order_type_multiplayer_raid:
			return true;
		default:
			return false;
		}
	}

	void best_record_mgr::team_best_record_role_name_replace(int32 map_template_id, s_single_map_record& best_record)
	{
		MapTemplate* map_template_ptr = template_manager::get_instance().get_map_template_map_id(map_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}
		team_ws* team = team_ws_mgr::get_instance().get_unit_team(best_record.role_guid);
		if (nullptr != team  &&  is_multiple_raid((e_map_order_type)map_template_ptr->Order))
		{
			const std::list<s_team_member_info>& vec_team_mem = team->get_member_list();
			std::vector<std::string> role_name_vec;
			for (const s_team_member_info& team_member : vec_team_mem)
			{
				role_name_vec.push_back(team_member.role_name);
			}
			best_record.role_guid = team->get_team_guid();
			best_record.set_role_name(init_unit::implode(role_name_vec));
		}
	}

	void best_record_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "best_record_mgr_change_player_name_func", 0, false, "%l%s", role_guid.server_64, role_name);
			return;

		}
		auto iter_record = m_activity_record_list.begin();
		for (; iter_record != m_activity_record_list.end(); ++iter_record)
		{
			for (int32 i = 0; i < faith::single_map_record_num; ++i)
			{
				if (iter_record->single_map_best_record[i].role_guid == role_guid)
				{
					iter_record->single_map_best_record[i].set_role_name(role_name);
					break;
				}
			}
		}
	}
	bool best_record_mgr::is_use_lua()
	{
		static int is_get = 0;
		static bool getRet = false;
		if (is_get == 0)
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "is_best_record_use_lua", 1, false, ">%b", &bRet);
			getRet = bRet;
			is_get = 1;
			return bRet;
		}
		return getRet;

	}
}