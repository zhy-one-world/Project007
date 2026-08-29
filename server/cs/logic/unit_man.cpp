/********************************************************************
	created:	2015/03/13
	created:	13:3:2015   12:32
	file base:	unit_man
	file ext:	cpp
	author:		zhy

	purpose:
	*********************************************************************/

#include "base.hpp"
#include "cache_mgr.hpp"
#include "cell_server.hpp"
#include "connection_mgr.hpp"
#include "gm_order_def.hpp"
#include "internal/login_msg.hpp"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "npc.hpp"
#include "server_log.hpp"
#include "system/play/time_activity_system.h"
#include "template/template_manager.h"

namespace faith
{
	struct s_msg_info
	{
		int32 msg_num;
		int64 msg_size_max;
		s_msg_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	typedef std::map<int32, s_msg_info> msg_info_map;
	typedef msg_info_map::iterator msg_info_map_it;

	player							m_empty_player;
	player* m_player_vec[init_player_max];//缓存的玩家管理
	std::list<int32>				m_empty_player_index;
	std::list<int32>				m_use_player_index;

	npc								m_empty_npc;
	npc* m_npc_vec[init_npc_max];//缓存的npc管理
	std::list<int32>				m_empty_npc_index;
	std::list<int32>				m_use_npc_index;

	msg_info_map					m_msg_info_map;
	msg_info_map					m_msg_info_map_client;
	bool							m_save_all_begin;
	std::map<xstring, int32>		m_harry_kill_num;
	int32							m_robot_num;

	bool unit_man::init_manager()
	{
		ZoneScoped;
		memset(m_player_vec, 0, sizeof(m_player_vec));
		memset(m_npc_vec, 0, sizeof(m_npc_vec));
		m_empty_player.clear_data();
		m_empty_npc.clear_data();
		m_robot_num = 0;
		m_save_all_begin = false;
		m_harry_kill_num.clear();
		for (int32 i = 1; i < init_player_max; ++i)
		{
			m_empty_player_index.push_back(i);
		}
		m_use_player_index.clear();
		for (int32 i = 1; i < init_npc_max; ++i)
		{
			m_empty_npc_index.push_back(i);
		}
		m_use_npc_index.clear();
		return true;
	}
	void unit_man::reload_csv()
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid())
			{
				player_ref->reload_csv();
			}
		}
	}

	void unit_man::refresh_all_player_time_limit()
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid())
			{
				player_ref->get_time_limit_activity_mgr().init_all_act();
			}
		}
	}

	void unit_man::heart_tick(const int64& new_time, const int32& tick_time)
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid())
			{
				player_ref->heart_tick(new_time, tick_time);
			}
		}
		for (auto it  = m_use_player_index.begin(); it != m_use_player_index.end();)
		{
			auto temp_it = it++;
			player* player_ref = m_player_vec[*temp_it];
			if (player_ref && player_ref->is_valid() == false)
			{
				cache_mgr::get_instance().back_player_ptr(player_ref);
				m_player_vec[*temp_it] = nullptr;

				m_empty_player_index.push_back(*temp_it);
				m_use_player_index.erase(temp_it);
			}
		}
		for (auto it : m_use_npc_index)
		{
			auto npc_ref = m_npc_vec[it];
			if (npc_ref && npc_ref->is_valid())
			{
				npc_ref->heart_tick(new_time, tick_time);
			}
		}		
		for (auto it = m_use_npc_index.begin(); it != m_use_npc_index.end();)
		{
			auto temp_it = it++;
			auto npc_ref = m_npc_vec[*temp_it];
			if (npc_ref && npc_ref->is_valid() == false)
			{
				cache_mgr::get_instance().back_npc_ptr(npc_ref);
				m_npc_vec[*temp_it] = nullptr;

				m_empty_npc_index.push_back(*temp_it);
				m_use_npc_index.erase(temp_it);
			}
		}
		if (get_player_num() <= 0)
		{
			m_save_all_begin = false;
		}
	}
	void unit_man::dump_npc_info_by_template_id(int32 npc_id)
	{
		ZoneScoped;
		for (auto it : m_use_npc_index)
		{
			npc* npc_ref = m_npc_vec[it];
			if (npc_ref && npc_ref->is_valid())
			{
				if (npc_ref->get_unit_info(e_role_info_template_id) == npc_id)
				{
					server_log::dump_npc_log(
						npc_ref->get_unit_info(e_role_info_move_map_id),
						npc_ref->get_unit_info(e_role_info_template_id),
						npc_ref->get_spawn_point_id(),
						npc_ref->get_array_index(),
						npc_ref->get_unit_guid(),
						npc_ref->get_new_map_pos().unit_location,
						npc_ref->get_line_id());
				}
			}
		}
	}
	int32 unit_man::get_player_num()
	{
		ZoneScoped;
		return m_use_player_index.size();
	}
	int32 unit_man::get_robot_num()
	{
		ZoneScoped;
		return m_robot_num;
	}
	void unit_man::add_robot_num()
	{
		ZoneScoped;
		m_robot_num++; 
	}
	player& unit_man::get_player(const guid_64& unit_guid)
	{
		ZoneScoped;
		if (unit_guid.is_valid() == false)
		{
			return m_empty_player;
		}
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid() && player_ref->get_unit_guid() == unit_guid)
			{
				return *player_ref;
			}
		}
		return m_empty_player;
	}

	player& unit_man::get_player(const s_client_uid& uid)
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid() && player_ref->get_client_uid() == uid)
			{
				return *player_ref;
			}
		}
		return m_empty_player;
	}

	player& unit_man::get_player(const int32& array_index)
	{
		ZoneScoped;
		if (array_index < 0 || array_index >= init_player_max || nullptr == m_player_vec[array_index])
		{
			return m_empty_player;
		}
		if (m_player_vec[array_index]->is_valid())
		{
			return *m_player_vec[array_index];
		}
		else
		{
			return m_empty_player;
		}
	}

	player&	unit_man::get_player(const xstring& szname)
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid() && player_ref->get_name() ==szname)
			{
				return *player_ref;
			}
		}
		return m_empty_player;
	}
	player& unit_man::get_player(const s_unit_identifier& unit_idf)
	{
		ZoneScoped;
		player& player_ref = get_player(unit_idf.runtime_id);
		if (player_ref.is_valid() && player_ref.get_unit_guid() == unit_idf.unit_guid)
		{
			return player_ref;
		}
		else
		{
			return m_empty_player;
		}
	}
	player& unit_man::get_empty_player(const guid_64 unit_guid)
	{
		ZoneScoped;
		if (unit_guid.is_valid() == false)
		{
			return m_empty_player;
		}
		if (m_empty_player_index.empty())
		{
			return m_empty_player;
		}
		player* player_ptr = cache_mgr::get_instance().get_player_ptr();
		if (nullptr == player_ptr)
		{
			return m_empty_player;
		}
		auto it = m_empty_player_index.begin();
		auto index = *it;

		m_empty_player_index.pop_front();
		m_use_player_index.push_back(index);

		player_ptr->set_array_index(index);
		player_ptr->set_unit_guid(unit_guid);
		player_ptr->set_data_use(true);
		m_player_vec[index] = player_ptr;
		return *player_ptr;
	}

	void unit_man::remove_player(const guid_64& unit_guid)
	{
		ZoneScoped;
		player& temp_player = get_player(unit_guid);
		if (temp_player.is_valid())
		{
			remove_player(temp_player.get_array_index());
		}
	}
	void unit_man::remove_player(const int32& array_index)
	{
		ZoneScoped;
		player& temp_player = get_player(array_index);
		if (temp_player.is_valid())
		{
			bool is_robot = temp_player.get_is_robot();
			world_cs::del_player(temp_player.get_map_ent(), &temp_player);
			temp_player.set_data_use(false);
			if (is_robot)
			{
				--m_robot_num;
			}
		}
	}

	void unit_man::remove_all_player()
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref)
			{
				if (player_ref->is_valid())
				{
					remove_player(it);					
				}
				cache_mgr::get_instance().back_player_ptr(player_ref);
			}
		}
		m_use_player_index.clear();
		m_empty_player_index.clear();
		memset(m_player_vec, 0, sizeof(m_player_vec));
		for (int32 i = 0; i < init_player_max; ++i)
		{
			m_empty_player_index.push_back(i);
		}
	}

	int32 unit_man::get_npc_num()
	{ 
		return m_use_npc_index.size();
	}
	npc& unit_man::get_npc(const guid_64& unit_guid)
	{
		ZoneScoped;
		if (unit_guid.is_valid() == false)
		{
			return m_empty_npc;
		}
		for (auto it : m_empty_npc_index)
		{
			npc* temp_npc = m_npc_vec[it];
			if (temp_npc && temp_npc->is_valid() && temp_npc->get_unit_guid() == unit_guid)
			{
				return *temp_npc;
			}
		}
		return m_empty_npc;
	}
	npc&  unit_man::get_npc(const s_unit_identifier& unit_idf)
	{
		ZoneScoped;
		npc& npc_ref = get_npc(unit_idf.runtime_id);
		if (npc_ref.is_valid() && npc_ref.get_unit_guid() == unit_idf.unit_guid)
		{
			return npc_ref;
		}
		else
		{
			return m_empty_npc;
		}
	}
	npc&  unit_man::get_npc(const int32& array_index)
	{
		ZoneScoped;
		if (array_index >= npc_arrary_index_begin && array_index < npc_arrary_index_max && m_npc_vec[array_index - npc_arrary_index_begin])
		{
			if (m_npc_vec[array_index - npc_arrary_index_begin]->is_valid())
			{
				return *m_npc_vec[array_index - npc_arrary_index_begin];
			}
		}
		return m_empty_npc;
	}

	npc& unit_man::get_empty_npc()
	{
		ZoneScoped;
		if (m_empty_npc_index.empty())
		{
			return m_empty_npc;
		}
		auto npc_ptr = cache_mgr::get_instance().get_npc_ptr();
		if (nullptr == npc_ptr)
		{
			return m_empty_npc;
		}
		auto it = m_empty_npc_index.begin();
		auto index = *it;

		m_empty_npc_index.pop_front();
		m_use_npc_index.push_back(index);

		npc_ptr->set_array_index(npc_arrary_index_begin + index);
		npc_ptr->set_unit_guid(guid_gen::make_guid());
		npc_ptr->set_data_use(true);
		m_npc_vec[index] = npc_ptr;
		return *npc_ptr;
	}

	void unit_man::remove_npc(const guid_64& unit_guid, bool is_dead)
	{
		ZoneScoped;
		npc& temp_npc = get_npc(unit_guid);
		if (temp_npc.is_valid())
		{
			remove_npc(temp_npc.get_array_index(), is_dead);
		}
	}

	void unit_man::remove_npc(const int32 array_index, bool is_dead)
	{
		ZoneScoped;
		npc& temp_npc = get_npc(array_index);
		if (temp_npc.is_valid())
		{
			MapTemplate* cur_map_template_ptr = temp_npc.get_cur_map_template_ptr();
			NpcTemplate* npc_template_ptr = temp_npc.get_npc_template();
			if (cur_map_template_ptr != nullptr && npc_template_ptr != nullptr && cur_map_template_ptr->Type == e_map_type_legion_station
				&& npc_template_ptr->NpcType == e_unit_type_monster && npc_template_ptr->SubType == e_monster_type_boss)
			{
				temp_npc.creat_dead_chests();
			}
			if (temp_npc.get_map_ent())
			{
				world_cs::del_npc(temp_npc.get_map_ent(), &temp_npc, is_dead);
				world_cs::del_aoi(temp_npc.get_map_ent(), temp_npc.get_new_map_pos().unit_location, array_index, is_dead);
			}
			temp_npc.set_data_use(false);
		}
		else
		{
			CONSOLE_ERROR("remove_npc error array_index = {}", array_index);
		}
	}

	void unit_man::kill_unit(const int32 killer_index, const int32 array_index)
	{
		ZoneScoped;
		buff_ex_env_param penv;
		penv.damage_num = int32_MAX_NUM;
		penv.buff_level = 1;

		buff_man::add_buff_inst(killer_index, array_index, GM_DAMAGE_BUFF_ID, &penv);
	}
	unit& unit_man::get_unit(const guid_64& unit_guid)
	{
		ZoneScoped;
		player& temp_player = get_player(unit_guid);
		if (temp_player.is_valid())
		{
			return temp_player;
		}
		npc& tempNpc = get_npc(unit_guid);
		if (tempNpc.is_valid())
		{
			return tempNpc;
		}
		return m_empty_player;
	}
	unit& unit_man::get_unit(const int32& array_index)
	{
		ZoneScoped;
		if (array_index >= npc_arrary_index_begin)
		{
			return get_npc(array_index);
		}
		else
		{
			return get_player(array_index);
		}
	}

	void unit_man::operate_unit_att(const int32& unit_array_index, const bool& is_add, const std::vector<float>& att_array)
	{
		ZoneScoped;
		unit& temp_unit = unit_man::get_unit(unit_array_index);
		if (temp_unit.is_valid() == false)
		{
			return;
		}
		temp_unit.get_pawn_att().apply_att_change_by_array(att_array, is_add);
	}

	unit& unit_man::get_unit(const s_unit_identifier& unit_idf)
	{
		ZoneScoped;
		if (unit_idf.runtime_id >= npc_arrary_index_begin)
		{
			npc& npc_ref = get_npc(unit_idf.runtime_id);
			if (npc_ref.get_unit_guid() == unit_idf.unit_guid)
			{
				return npc_ref;
			}
			else
			{
				return m_empty_npc;
			}
		}
		else
		{
			player& player_ref = get_player(unit_idf.runtime_id);
			if (player_ref.get_unit_guid() == unit_idf.unit_guid)
			{
				return player_ref;
			}
			else
			{
				return m_empty_player;
			}
		}
	}

	void unit_man::remove_unit(const int32& array_index)
	{
		ZoneScoped;
		if (array_index >= npc_arrary_index_begin)
		{
			return remove_npc(array_index, false);
		}
		else
		{
			return remove_player(array_index);
		}
	}
	void unit_man::remove_unit(const guid_64& unit_guid)
	{
		ZoneScoped;
		remove_player(unit_guid);
		remove_npc(unit_guid, false);
	}
	void unit_man::save_all_player(e_logout_result logout_result)
	{
		ZoneScoped;
		CONSOLE_INFO("unit_man::save_all_player logout_result = {}", (int32)logout_result);
		if (m_save_all_begin)
		{
			return;
		}
		m_save_all_begin = true;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			if (player_ref && player_ref->is_valid())
			{
				player_ref->clear_saving_flag(e_save_data_type_exit_game);
				player_ref->save_role_all_data(e_save_data_type_exit_game);
			}
		}
	}
	void unit_man::logout_player(const guid_64& unit_guid, const int32& array_index, bool bcs, e_logout_result logout_result)
	{
		ZoneScoped;
		player& player_ref = get_player(array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != unit_guid)
		{
			return;
		}
		cs2ws_client_logout rep_out;
		rep_out.player_guid = unit_guid;
		rep_out.bCS = bcs;
		rep_out.logout_result = logout_result;
		connection_mgr::getInstance().send_to_ws(&rep_out, sizeof(rep_out));
		if (player_ref.get_write_log())
		{
			int64 login_time = player_ref.get_login_time();
			int64 online_time = utility::get_tick_count() - login_time;
			int64 create_time = player_ref.get_unit_i64_info_data(e_role_i64_info_create_time);
			server_log::logout_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_money_data_inst(), online_time, create_time, player_ref.get_login_type());
			server_log::player_pk_role_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), player_ref.get_pk_community_mgr().get_pk_mode(), player_ref.get_pk_community_mgr().get_pk_value());
			
			int32 strength = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_strength) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_strength);
			int32 agility = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_agility) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_agility);
			int32 intellect = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_intellect) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_intellect);
			int32 stamina = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_stamina) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_stamina);
			server_log::player_mark_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), strength, agility, intellect, stamina);
			
			int32 cur_bag_total_num = player_ref.get_logic_data(e_role_logic_info_bag_open);
			server_log::bag_total_num_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), cur_bag_total_num);
			
			int32 unused_num = player_ref.get_item_set().get_empty_slot_num(e_bag_type_bag);
			server_log::bag_unused_num_log(unused_num);

			int32 pound_num = player_ref.get_pound_num();
			player_ref.set_pound_num(0);
			server_log::element_recovery_log(pound_num);
		}
		remove_player(player_ref.get_array_index());
	}
	void unit_man::set_all_player_buff(int32 buff_template_id, bool is_add)
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			if (is_add)
			{
				buff_man::add_buff_inst(it, it, buff_template_id);
			}
			else
			{
				buff_man::del_buff_by_template_id(it, it, buff_template_id);
			}
		}
	}
	void  unit_man::refresh_all_time_activity()
	{
		ZoneScoped;
		for (auto it : m_use_player_index)
		{
			player* player_ref = m_player_vec[it];
			time_activity_system::generate_new_activity(player_ref);
			time_activity_system::clear_old_activity(player_ref);
		}
	}
	void unit_man::log_msg_info(int32 msg_header, int32 msg_size)
	{
		ZoneScoped;
		msg_info_map_it it = m_msg_info_map.find(msg_header);
		if (it != m_msg_info_map.end())
		{
			it->second.msg_num++;
			it->second.msg_size_max += msg_size;
		}
		else
		{
			s_msg_info temp_info;
			temp_info.msg_num = 1;
			temp_info.msg_size_max = msg_size;
			m_msg_info_map[msg_header] = temp_info;
		}
	}
	void unit_man::log_msg_print_file()
	{
		ZoneScoped;
		for (msg_info_map_it it = m_msg_info_map.begin(); it != m_msg_info_map.end(); ++it)
		{
			server_log::dump_msg_info(it->first, it->second.msg_num, it->second.msg_size_max);
		}
	}
	void unit_man::log_msg_info_client(int32 msg_header, int32 msg_size)
	{
		ZoneScoped;
		//msg_info_map_it it = m_msg_info_map_client.find(msg_header);
		//if (it != m_msg_info_map_client.end())
		//{
		//	it->second.msg_num++;
		//	it->second.msg_size_max += msg_size;
		//}
		//else
		//{
		//	s_msg_info temp_info;
		//	temp_info.msg_num = 1;
		//	temp_info.msg_size_max = msg_size;
		//	m_msg_info_map_client[msg_header] = temp_info;
		//}
	}

	int32 unit_man::get_grade_num(int32 player_template_id, int32 class_type)
	{
		ZoneScoped;
		if (class_type <= 0 || class_type > GAMECONFIG->InitRoleAry.size())
		{
			return 0;
		}

		int32 grade_num = player_template_id - GAMECONFIG->InitRoleAry[class_type - 1];

		return grade_num;
	}
	int32 unit_man::get_world_exp_value()
	{
		ZoneScoped;
		int32 default_level = GAMECONFIG->FirstExpLevel;

		int32 world_level = cell_server::getInstance().get_world_level_last();
		if (world_level <= 0)
		{
			world_level = default_level;
		}
		int32 upgrade_id = player_upgrade_cfg_first_id + world_level - 1;
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_id);
		if (nullptr == template_ptr)
		{
			return 0;
		}
		return template_ptr->BaseExp;
	}

	bool unit_man::kill_player_num(guid_64 guid_killer, guid_64 guid_killed, int32 level_killer, int32 level_killed) //开头还差等级判定
	{
		ZoneScoped;
		if (GAMECONFIG->ServerHarryKillLimit.size() % 3 != 0)
		{
			return true;
		}
		int32 region_code = (int32)e_version_region_type_china_main;
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr != nullptr)
		{
			region_code = region_template_ptr->RegionCode;
		}
		int32 max_gap_level = 0, max_kill_num = 0;
		bool have_region_code = false;
		for (int32 index = 0; index < (GAMECONFIG->ServerHarryKillLimit.size()) - 2; index += 3)
		{
			if (GAMECONFIG->ServerHarryKillLimit[index] == region_code)
			{
				max_gap_level = GAMECONFIG->ServerHarryKillLimit[index + 1];
				max_kill_num = GAMECONFIG->ServerHarryKillLimit[index + 2];
				have_region_code = true;
			}
		}
		if (!have_region_code)
		{
			return true;
		}
		if (level_killer - level_killed > max_gap_level)
		{
			return false;
		}
		xstring killer_player = guid_killer.to_string() + guid_killed.to_string(); //把killer和killed的guid转化成字符串并拼接起来当做map的key	
		map<xstring, int32>::iterator iter; //判断map中是否存在a kill b的key，存在判断击杀次数是否小于N，小于则++value，不存在则插入key并初始化对应value为1（表示a击杀b一次）
		iter = m_harry_kill_num.find(killer_player);
		if (iter == m_harry_kill_num.end())
		{
			m_harry_kill_num.insert(make_pair(killer_player, 1));
			return true;
		}
		else if (iter->second >= max_kill_num)
		{
			return false;
		}
		++iter->second;
		return true;
	}

	void unit_man::refresh_kill_num_at_zero_hour()
	{
		ZoneScoped;
		m_harry_kill_num.clear();
	}
}
