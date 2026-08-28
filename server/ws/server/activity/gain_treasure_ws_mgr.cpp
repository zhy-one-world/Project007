#include "gain_treasure_ws_mgr.h"
#include "template/template_manager.h"
#include <gain_treasure_msg.hpp>
#include "ws_client.hpp"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "lua/script_mgr.h"
#include "utility/parse_msg.h"
#include "game.pb.h"
#include "net.pb.h"

namespace hld
{
	gain_treasure_ws_mgr::gain_treasure_ws_mgr()
	{
		m_record_map.clear();
	}

	void gain_treasure_ws_mgr::init_manager()
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_req_load_gain_treasure_record_info_from_db", 0, false, "");
			return;
		}
		req_load_gain_treasure_record_info_from_db();
	}

	void gain_treasure_ws_mgr::save_gain_treasure_record_info_into_db(int32 boss_spawn_point_template_id, int32 del_stamp)
	{
		s_gain_treasure_record_info_all* one_boss_record_all = get_gain_treasure_record_one_boss_info(boss_spawn_point_template_id);
		if (nullptr == one_boss_record_all || one_boss_record_all->record_info_list.size() <= 0)
		{
			return;
		}
		auto ite = one_boss_record_all->record_info_list.begin();
		if (ite == one_boss_record_all->record_info_list.end())
		{
			return;
		}
		s_gain_treasure_record_info_one& record_info = *ite;

		server2dp_proto_ws2dp_save_gain_treasure_record_info msg;
		msg.set_del_stamp(del_stamp);
		msg.set_boss_spawn_point_template_id(record_info.boss_spawn_point_template_id);
		msg.set_killed_stamp(record_info.killed_stamp);
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			msg.add_role_name(record_info.player_info_arr[i].role_name);
			msg.add_role_guid(record_info.player_info_arr[i].role_guid.server_64);
			std::vector<std::string> vec_gain_item_template_str;
			vec_gain_item_template_str.clear();
			for (int32 j = 0; j < gain_treasure_gain_item_max_num; j++)
			{
				if (record_info.player_info_arr[i].gain_item_template_id[j] <= 0)
				{
					continue;
				}
				vec_gain_item_template_str.push_back(template_manager::get_instance().int_to_string(record_info.player_info_arr[i].gain_item_template_id[j]));
			}
			std::string gain_items_template_id_str = init_unit::implode(vec_gain_item_template_str);
			msg.add_gain_item_template_id(gain_items_template_id_str);
		}
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_gain_treasure_record_info);
		/*ws2dp_save_gain_treasure_record_info save_record_info_msg;
		save_record_info_msg.del_stamp = del_stamp;
		save_record_info_msg.record_info.boss_spawn_point_template_id = record_info.boss_spawn_point_template_id;
		save_record_info_msg.record_info.killed_stamp = record_info.killed_stamp;
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			std::string role_name(record_info.player_info_arr[i].role_name);
			if (role_name.empty())
			{
				continue;
			}
			memcpy(save_record_info_msg.record_info.player_info_arr[i].role_name, record_info.player_info_arr[i].role_name, max_name_size);
			save_record_info_msg.record_info.player_info_arr[i].role_guid = record_info.player_info_arr[i].role_guid;
			std::vector<std::string> vec_gain_item_template_str;
			vec_gain_item_template_str.clear();
			for (int32 j = 0; j < gain_treasure_gain_item_max_num; j++)
			{
				if (record_info.player_info_arr[i].gain_item_template_id[j] <= 0)
				{
					continue;
				}
				vec_gain_item_template_str.push_back(template_manager::get_instance().int_to_string(record_info.player_info_arr[i].gain_item_template_id[j]));
			}
			std::string gain_items_template_id_str = init_unit::implode(vec_gain_item_template_str);
			memcpy(save_record_info_msg.record_info.player_info_arr[i].gain_item_template_id, gain_items_template_id_str.c_str(),
				gain_items_template_id_str.length() >= gain_treasure_gain_item_id_max_size ? gain_treasure_gain_item_id_max_size : gain_items_template_id_str.length());
		}

		ws_client::getInstance().send_to_dp(&save_record_info_msg, sizeof(save_record_info_msg));*/
	}

	void gain_treasure_ws_mgr::req_load_gain_treasure_record_info_from_db()
	{
		std::set<int32> vec_boss_spawn_point_template_id = get_npc_spawn_point_template_id_by_map_type(e_map_type_boss_home);
		for (auto it : vec_boss_spawn_point_template_id)
		{
			NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, it);
			if (nullptr == npc_spawn_point_template_ptr)
			{
				continue;
			}
			NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
			if (nullptr == npc_template_ptr || npc_template_ptr->NpcType != e_unit_type_monster ||npc_template_ptr->SubType != e_monster_type_boss)
			{
				continue;
			}
			server2dp_proto_ws2dp_load_all_gain_treasure_record_info load_all_gain_treasure_record_info_msg;
			load_all_gain_treasure_record_info_msg.set_boss_spawn_point_id(it);
			ws_client::getInstance().send_to_dp_new(&load_all_gain_treasure_record_info_msg, e_msg_ws2dp_ws2dp_load_all_gain_treasure_record_info);
			/*ws2dp_load_all_gain_treasure_record_info load_all_gain_treasure_record_info_msg;
			load_all_gain_treasure_record_info_msg.boss_spawn_point_id = it;
			ws_client::getInstance().send_to_dp(&load_all_gain_treasure_record_info_msg, sizeof(load_all_gain_treasure_record_info_msg));*/
		}

		vec_boss_spawn_point_template_id.clear();
		vec_boss_spawn_point_template_id = get_npc_spawn_point_template_id_by_map_type(e_map_type_boss_vip_home);
		for (auto it : vec_boss_spawn_point_template_id)
		{
			NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, it);
			if (nullptr == npc_spawn_point_template_ptr)
			{
				continue;
			}
			NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_spawn_point_template_ptr->TemplateId);
			if (nullptr == npc_template_ptr || npc_template_ptr->NpcType != e_unit_type_monster || npc_template_ptr->SubType != e_monster_type_boss)
			{
				continue;
			}
			server2dp_proto_ws2dp_load_all_gain_treasure_record_info load_all_gain_treasure_record_info_msg;
			load_all_gain_treasure_record_info_msg.set_boss_spawn_point_id(it);
			ws_client::getInstance().send_to_dp_new(&load_all_gain_treasure_record_info_msg, e_msg_ws2dp_ws2dp_load_all_gain_treasure_record_info);

			//ws2dp_load_all_gain_treasure_record_info load_all_gain_treasure_record_info_msg;
			//load_all_gain_treasure_record_info_msg.boss_spawn_point_id = it;
			//ws_client::getInstance().send_to_dp(&load_all_gain_treasure_record_info_msg, sizeof(load_all_gain_treasure_record_info_msg));
		}
	}

	void gain_treasure_ws_mgr::load_gain_treasure_record_info_from_db_proc(const s_gain_treasure_record_info_one_db* record_infos, int32 data_num)
	{
		for (int32 i = 0; i < data_num; i++)
		{
			if (i >= gain_treasure_record_info_max_size)
			{
				return;
			}
			load_gain_treasure_record_info(*(record_infos + i));
		}
	}

	void gain_treasure_ws_mgr::load_gain_treasure_record_info(const s_gain_treasure_record_info_one_db& record_info_db)
	{
		s_gain_treasure_record_info_one record_info;
		record_info.boss_spawn_point_template_id = record_info_db.boss_spawn_point_template_id;
		record_info.killed_stamp = record_info_db.killed_stamp;
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			std::string role_name(record_info_db.player_info_arr[i].role_name);
			if (role_name.empty())
			{
				continue;
			}
			memcpy(record_info.player_info_arr[i].role_name, record_info_db.player_info_arr[i].role_name, max_name_size);
			record_info.player_info_arr[i].role_guid = record_info_db.player_info_arr[i].role_guid;
			std::vector<std::string> vec_gain_item_template_str;
			init_unit::explode(std::string(record_info_db.player_info_arr[i].gain_item_template_id), vec_gain_item_template_str);
			for (int32 j = 0; j < vec_gain_item_template_str.size(); j++)
			{
				if (j >= gain_treasure_gain_item_max_num)
				{
					break;
				}
				record_info.player_info_arr[i].gain_item_template_id[j] = std::stoi(vec_gain_item_template_str[j].c_str());
			}
		}
		add_gain_treasure_record(record_info);
		add_gain_item_reocrd_by_one_boss(record_info);
	}

	s_gain_treasure_record_info_all* gain_treasure_ws_mgr::get_gain_treasure_record_one_boss_info(int32 boss_spawn_point_template_id)
	{
		auto ite = m_record_map.find(boss_spawn_point_template_id);
		if (ite == m_record_map.end())
		{
			return nullptr;
		}
		return &(ite->second);
	}

	void gain_treasure_ws_mgr::add_gain_treasure_record_and_save(const s_gain_treasure_record_info_one& record_info)
	{
		NpcSpawnPointTemplate* boss_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, record_info.boss_spawn_point_template_id);
		if (nullptr == boss_spawn_point_template_ptr || record_info.killed_stamp <= 0)
		{
			return;
		}
		int32 del_stamp = 0;
		del_stamp = add_gain_treasure_record(record_info);
		add_gain_item_reocrd_by_one_boss(record_info);
		save_gain_treasure_record_info_into_db(record_info.boss_spawn_point_template_id, del_stamp);
	}

	int32 gain_treasure_ws_mgr::add_gain_treasure_record(const s_gain_treasure_record_info_one& record_info)
	{
		auto ite = m_record_map.find(record_info.boss_spawn_point_template_id);
		int32 del_stamp = 0;
		if (ite == m_record_map.end())
		{
			s_gain_treasure_record_info_all one_boss_record_all_info;
			one_boss_record_all_info.clear_data();
			one_boss_record_all_info.add_record(record_info);
			m_record_map.insert({ record_info.boss_spawn_point_template_id, one_boss_record_all_info });
		}
		else
		{
			del_stamp = ite->second.add_record(record_info);
		}
		return del_stamp;
	}

	void gain_treasure_ws_mgr::add_gain_item_record(s_gain_treasure_gain_item_record_info& record_info)
	{
		int32 cur_record_num = m_gain_item_record_list.size();
		auto insert_ite = m_gain_item_record_list.end();
		for (auto ite = m_gain_item_record_list.begin(); ite != m_gain_item_record_list.end(); ite++)
		{
			if (record_info.killed_stamp >= ite->killed_stamp)
			{
				insert_ite = ite;
				record_info.record_index = ite->record_index + 1;
				if (record_info.record_index >= gain_treasure_gain_item_record_max_size)
				{
					record_info.record_index = 0;
				}
				break;
			}
			ite->record_index++;
			if (ite->record_index >= gain_treasure_gain_item_record_max_size)
			{
				ite->record_index = 0;
			}
		}

		if (insert_ite == m_gain_item_record_list.end())
		{
			if (cur_record_num == 0)
			{
				record_info.record_index = 0;
			}
			else
			{
				record_info.record_index = m_gain_item_record_list.rbegin()->record_index - 1;
				if (record_info.record_index < 0)
				{
					record_info.record_index = gain_treasure_gain_item_record_max_size - 1;
				}
			}
		}

		m_gain_item_record_list.insert(insert_ite, record_info);
		
		if (cur_record_num > gain_treasure_gain_item_record_max_size)
		{
			m_gain_item_record_list.pop_back();
		}
	}

	void gain_treasure_ws_mgr::add_gain_item_reocrd_by_one_boss(const s_gain_treasure_record_info_one& record_info)
	{
		//先放记录 再放置顶 可以让置顶的物品一直在记录上面
		add_gain_item_reocrd_by_dorp_record_model(record_info, e_drop_record_model_type_record);
		add_gain_item_reocrd_by_dorp_record_model(record_info, e_drop_record_model_type_set_top);
	}

	void gain_treasure_ws_mgr::add_gain_item_reocrd_by_dorp_record_model(const s_gain_treasure_record_info_one& record_info, e_drop_record_model_type record_model)
	{
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			std::string role_name(record_info.player_info_arr[i].role_name);
			if (role_name.empty())
			{
				continue;
			}
			for (int32 j = 0; j < gain_treasure_gain_item_max_num; j++)
			{
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, record_info.player_info_arr[i].gain_item_template_id[j]);
				if (nullptr == item_template_ptr || item_template_ptr->DropRecordModel != record_model)
				{
					continue;
				}
				s_gain_treasure_gain_item_record_info gain_item_record_info;
				gain_item_record_info.boss_spawn_point_template_id = record_info.boss_spawn_point_template_id;
				memcpy(gain_item_record_info.role_name, record_info.player_info_arr[i].role_name, max_name_size);
				gain_item_record_info.item_template_id = record_info.player_info_arr[i].gain_item_template_id[j];
				gain_item_record_info.killed_stamp = record_info.killed_stamp;
				gain_item_record_info.role_guid = record_info.player_info_arr[i].role_guid;
				add_gain_item_record(gain_item_record_info);
			}
		}
	}

	void gain_treasure_ws_mgr::req_kill_record_end(client_session* session, int32 boss_spawn_point_template_id)
	{
		if (nullptr == session)
		{
			return;
		}

		game_proto_gain_treasure_kill_record_end kill_record_end_msg;
		if (fill_kill_record_end_msg(kill_record_end_msg, boss_spawn_point_template_id) <= 0)
		{
			return;
		}
		
		session->send_to_client(&kill_record_end_msg, e_msgindex_s2c_gain_treasure_kill_record_end);
	}

	int32 gain_treasure_ws_mgr::fill_kill_record_end_msg(game_proto_gain_treasure_kill_record_end& kill_record_end_msg, int32 boss_spawn_point_template_id)
	{
		NpcSpawnPointTemplate* boss_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, boss_spawn_point_template_id);
		if (nullptr == boss_spawn_point_template_ptr)
		{
			return 0;
		}

		s_gain_treasure_record_info_all* one_boss_record_ptr = get_gain_treasure_record_one_boss_info(boss_spawn_point_template_id);
		if (nullptr == one_boss_record_ptr || !one_boss_record_ptr->is_valid())
		{
			return 0;
		}
		
		kill_record_end_msg.set_boss_spawn_point_template_id(boss_spawn_point_template_id);
		auto ite = one_boss_record_ptr->record_info_list.begin();
		for (; ite != one_boss_record_ptr->record_info_list.end(); ite++)
		{
			game_proto_gain_treasure_kill_record_one* one_record = kill_record_end_msg.add_record_arr();
			if (nullptr == one_record)
			{
				continue;
			}
			one_record->set_killed_stamp(ite->killed_stamp);
			for (int32 i = 0; i < max_team_member_num; i++)
			{
				std::string role_name(ite->player_info_arr[i].role_name);
				if (role_name.empty())
				{
					continue;
				}
				one_record->add_role_name(role_name);
			}
		}

		return 1;
	}

	void gain_treasure_ws_mgr::req_gain_item_record(client_session* session, int32 record_index)
	{
		if (nullptr == session)
		{
			return;
		}

		game_proto_gain_treasure_gain_item_record_end gain_item_record_end_msg;
		if (fill_gain_item_record_end_msg(gain_item_record_end_msg, record_index) <= 0)
		{
			return;
		}

		session->send_to_client(&gain_item_record_end_msg, e_msgindex_s2c_gain_treasure_gain_item_record_end);
	}

	int32 gain_treasure_ws_mgr::fill_gain_item_record_end_msg(game_proto_gain_treasure_gain_item_record_end& gain_item_record_end_msg, int32 record_index)
	{
		if (m_gain_item_record_list.empty())
		{
			return 0;
		}

		//先找到已经发送记录的位置
		auto ite = m_gain_item_record_list.begin();
		if (record_index >= 0)
		{
			for (ite = m_gain_item_record_list.begin(); ite != m_gain_item_record_list.end(); ite++)
			{
				if (ite->record_index == record_index)
				{
					++ite;
					break;
				}
			}
		}

		int32 record_list_len = 0;
		for (int32 i = 0; i < one_gain_item_record_list_get_num; i++)
		{
			if (ite == m_gain_item_record_list.end())
			{
				break;
			}
			game_proto_gain_treasure_gain_item_record_info_one* one_record = gain_item_record_end_msg.add_record_arr();
			if (nullptr == one_record)
			{
				continue;
			}

			std::string role_name(ite->role_name);
			if (role_name.empty())
			{
				ite++;
				continue;
			}

			one_record->set_role_name(role_name);
			one_record->set_boss_spawn_point_template_id(ite->boss_spawn_point_template_id);
			one_record->set_item_template_id(ite->item_template_id);
			one_record->set_killed_stamp(ite->killed_stamp);
			one_record->set_record_index(ite->record_index);
			one_record->set_role_guid(ite->role_guid.server_64);
			ite++;
		}

		return 1;
	}

	s_gain_treasure_boss_info* gain_treasure_ws_mgr::get_gain_treasure_boss_info_by_id(int32 boss_spawn_point_template_id)
	{
		auto ite = m_boss_info_map.find(boss_spawn_point_template_id);
		if (ite == m_boss_info_map.end())
		{
			return nullptr;
		}
		return &(ite->second);
	}

	void gain_treasure_ws_mgr::update_gain_treasure_boss_info(const s_gain_treasure_boss_info& boss_info)
	{
		if (is_use_lua())
		{
			s_gain_treasure_boss_info& tmp = const_cast<s_gain_treasure_boss_info& >(boss_info);
			hld::st_proto::st_gain_treasure_boss_info pro_msg;
			tmp.to_proto(&pro_msg);
			packet_s2s * p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}

			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_update_gain_treasure_boss_info", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;

		}
		auto ite = m_boss_info_map.find(boss_info.boss_spawn_point_template_id);
		if (ite == m_boss_info_map.end())
		{
			m_boss_info_map.insert({ boss_info.boss_spawn_point_template_id, boss_info });
			send_gain_treasure_boss_to_all(boss_info.boss_spawn_point_template_id);
		}
		else
		{
			//boss刚复活或者刚死亡 消息发送给全服玩家
			if ((!ite->second.boss_guid.is_valid() && boss_info.boss_guid.is_valid())
				|| !boss_info.boss_guid.is_valid())
			{
				ite->second = boss_info;
				send_gain_treasure_boss_to_all(boss_info.boss_spawn_point_template_id);
			}
			//boss归属改变只发给对应地图的玩家
			else
			{
				ite->second = boss_info;
				send_gain_treasure_boss_to_map(boss_info.boss_spawn_point_template_id);
			}
		}
	}

	void gain_treasure_ws_mgr::send_gain_treasure_boss_info_one(client_session* session, int32 boss_spawn_point_template_id)
	{
		game_proto_gain_treasure_boss_info_one boss_info_msg;
		if (fill_boss_info_one_msg(boss_info_msg, boss_spawn_point_template_id) <= 0)
		{
			return;
		}
		session->send_to_client(&boss_info_msg, e_msgindex_s2c_gain_treasure_boss_info_one);
	}

	void gain_treasure_ws_mgr::send_gain_treasure_boss_info_all(client_session* session)
	{ 
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_send_gain_treasure_boss_info_all", 0, false, "%l", session->get_role_guid().server_64);
			return;
		}
		game_proto_gain_treasure_boss_info_all boss_info_all_msg;
		for (auto ite = m_boss_info_map.begin(); ite != m_boss_info_map.end(); ite++)
		{
			game_proto_gain_treasure_boss_info_one* boss_info = boss_info_all_msg.add_boss_info();
			if (nullptr == boss_info)
			{
				continue;
			}
			fill_boss_info_one_msg(*boss_info, ite->second.boss_spawn_point_template_id);
			if (boss_info_all_msg.ByteSize() > packege_length_max)
			{
				session->send_to_client(&boss_info_all_msg, e_msgindex_s2c_gain_treasure_boss_info_all);
				boss_info_all_msg.clear_boss_info();
			}
		}
		session->send_to_client(&boss_info_all_msg, e_msgindex_s2c_gain_treasure_boss_info_all);
	}

	int32 gain_treasure_ws_mgr::fill_boss_info_one_msg(game_proto_gain_treasure_boss_info_one& boss_info_one_msg, int32 boss_spawn_point_template_id)
	{
		auto ite = m_boss_info_map.find(boss_spawn_point_template_id);
		if (ite == m_boss_info_map.end())
		{
			return 0;
		}
		boss_info_one_msg.set_boss_spawn_point_template_id(boss_spawn_point_template_id);
		boss_info_one_msg.set_boss_guid(ite->second.boss_guid.server_64);
		boss_info_one_msg.set_next_refresh_stamp(ite->second.next_refresh_stamp);
		boss_info_one_msg.set_first_player_name(ite->second.player_info[0].player_name);
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			if (!ite->second.player_info[i].player_guid.is_valid())
			{
				continue;
			}
			boss_info_one_msg.add_player_guid(ite->second.player_info[i].player_guid.server_64);
		}
		return 1;
	}

	void gain_treasure_ws_mgr::send_gain_treasure_boss_to_all(int32 boss_spawn_point_template_id)
	{
		game_proto_gain_treasure_boss_info_one boss_info_msg;
		if (fill_boss_info_one_msg(boss_info_msg, boss_spawn_point_template_id) <= 0)
		{
			return;
		}
		client_session_mgr::getInstance().send_message_to_all_client(&boss_info_msg, e_msgindex_s2c_gain_treasure_boss_info_one);
	}

	void gain_treasure_ws_mgr::send_gain_treasure_boss_to_map(int32 boss_spawn_point_template_id)
	{
		NpcSpawnPointTemplate* boss_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, boss_spawn_point_template_id);
		if (nullptr == boss_spawn_point_template_ptr)
		{
			return;
		}
		game_proto_gain_treasure_boss_info_one boss_info_msg;
		if (fill_boss_info_one_msg(boss_info_msg, boss_spawn_point_template_id) <= 0)
		{
			return;
		}

		auto ite = m_boss_info_map.find(boss_spawn_point_template_id);
		if (ite == m_boss_info_map.end())
		{
			return;
		}
		int32 cur_num = 0;
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			if (ite->second.player_info[i].player_guid.is_valid())
			{
				cur_num++;
			}
		}
		if (cur_num == 0)//如果为0则表示仇恨被清除，发给所有人
		{
			client_session_mgr::getInstance().send_message_to_all_client(&boss_info_msg, e_msgindex_s2c_gain_treasure_boss_info_one);
			return;
		}
		client_session_mgr::getInstance().send_message_to_map(boss_spawn_point_template_ptr->MapId, &boss_info_msg, e_msgindex_s2c_gain_treasure_boss_info_one);
	}

	std::set<int32> gain_treasure_ws_mgr::get_npc_spawn_point_template_id_by_map_id(int32 map_template_id)
	{
		std::set<int32> vec_boss_spawn_point_template_id;
		vec_boss_spawn_point_template_id.clear();

		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return vec_boss_spawn_point_template_id;
		}
		int32 npc_spawn_id = map_template_ptr->NpcPointId;
		int32 next_spawn_id = 0;

		while (true)
		{
			NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_id);
			if (nullptr == npc_spawn_point_template_ptr || npc_spawn_point_template_ptr->MapId != map_template_id)
			{
				break;
			}
			vec_boss_spawn_point_template_id.insert(npc_spawn_point_template_ptr->attribute_id);
			next_spawn_id = npc_spawn_point_template_ptr->NextSpawnNpcId;
			if (next_spawn_id <= npc_spawn_id)
			{
				break;
			}
			npc_spawn_id = next_spawn_id;
		}

		return vec_boss_spawn_point_template_id;
	}

	std::set<int32> gain_treasure_ws_mgr::get_npc_spawn_point_template_id_by_map_type(e_map_type map_type)
	{
		std::set<int32> vec_boss_spawn_point_template_id;
		vec_boss_spawn_point_template_id.clear();
		if (map_type >= e_map_type_max)
		{
			return vec_boss_spawn_point_template_id;
		}

		template_manager::template_type* table_ptr = template_manager::get_instance().get_templates(e_MapTemplate);
		if (nullptr == table_ptr)
		{
			return vec_boss_spawn_point_template_id;
		}

		std::set<int32> temp_boss_spawn_point_template_id;
		template_manager::template_type::iterator ite;
		for (ite = table_ptr->begin(); ite != table_ptr->end(); ++ite)
		{
			temp_boss_spawn_point_template_id.clear();
			MapTemplate* map_template_ptr = (MapTemplate*)(ite->second);
			if (nullptr == map_template_ptr || map_template_ptr->Type != map_type)
			{
				continue;
			}
			temp_boss_spawn_point_template_id = get_npc_spawn_point_template_id_by_map_id(ite->first);
			vec_boss_spawn_point_template_id.insert(temp_boss_spawn_point_template_id.begin(), temp_boss_spawn_point_template_id.end());
		}

		return vec_boss_spawn_point_template_id;
	}

	void gain_treasure_ws_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name, const xstring& original_name)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_change_player_name_func", 0, false, "%l%s%s", role_guid.server_64, role_name.c_str(), original_name.c_str());
			return;
		}
		auto iter_boss_info = m_boss_info_map.begin();
		for (; iter_boss_info != m_boss_info_map.end(); ++iter_boss_info)
		{
			for ( int32 i = 0; i < max_team_member_num; ++i)
			{
				if (iter_boss_info->second.player_info[i].player_guid == role_guid)
				{
					iter_boss_info->second.player_info[i].set_role_name(role_name);
					break;
				}
			}
		}

		auto iter_item_record = m_gain_item_record_list.begin();
		for (; iter_item_record != m_gain_item_record_list.end(); ++iter_item_record)
		{
			if (iter_item_record->role_guid == role_guid)
			{
				iter_item_record->set_role_name(role_name);
			}
		}
	}

	bool gain_treasure_ws_mgr::is_use_lua()
	{

		static int is_get = 0;
		static bool getRet = false;
		if (is_get == 0)
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "is_treasure_use_lua", 1, false, ">%b", &bRet);
			getRet = bRet;
			is_get = 1;
			return bRet;
		}
		return getRet;


	}

}
