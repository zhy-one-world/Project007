/********************************************************************
created: 2018Äê9ÔÂ6ÈÕ
file base: boss_island_mgr
file ext: cpp
author: zhangshuo

purpose: ÓÀºãµº
*********************************************************************/


#include "boss_island_mgr.h"
#include "connection_mgr.hpp"
#include "Logic/activity_def.hpp"
#include "logic/player.hpp"
#include "utility/parse_msg.h"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "internet/cs2dp.pb.h"
#include "internal/boss_island_msg.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"

namespace hld
{
	boss_island_mgr::boss_island_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	boss_island_mgr::~boss_island_mgr()
	{

	}

	void boss_island_mgr::clear_data()
	{
		for (int32 i = 0; i < e_island_record_max; i++)
		{
			m_boss_island_info[i] = 0;
		}
	}

	void boss_island_mgr::set_player_ptr(player* player_ptr)
	{
		m_player_ptr = player_ptr;
	}

	void boss_island_mgr::load_boss_island_info_from_db(const s_boss_island_info& boss_island_info)
	{
		for (int32 i = 0; i < e_island_record_max; i++)
		{
			m_boss_island_info[i] = boss_island_info.data_ary[i];
		}
		send_player_boss_island_info_all();
	}

	bool boss_island_mgr::load_boss_island_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::cs2dp_proto::role_boss_island_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_boss_island_info db_data;

		for (int32 i = 0 ; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}

		load_boss_island_info_from_db(db_data);
		return true;

	}

	void boss_island_mgr::save_boss_island_to_db(int32 type)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_boss_island_info);
		if (is_use == false)
		{
			cs2dp_save_boss_island_info req;
			req.save_type_ex = type;
			req.role_guid = m_player_ptr->get_unit_guid();
			req.unit_array_index = m_player_ptr->get_array_index();
			for (int32 i = 0; i < e_island_record_max; i++)
			{
				req.info_data.data_ary[i] = m_boss_island_info[i];
			}
			m_player_ptr->send_message_to_dp(&req, req.get_pak_length());
		}
		else
		{
			hld::cs2dp_proto::save_role_boss_island msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			msg.set_unit_array_index(m_player_ptr->get_array_index());
			msg.set_save_type_ex(type);

			hld::cs2dp_proto::role_boss_island_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_island_record_max; i++)
			{
				db_data->add_data_ary(m_boss_island_info[i]);
			}
			m_player_ptr->send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_boss_island_info);	
		}
	}

	void boss_island_mgr::set_boss_island_info(int32 type, int32 value)
	{
		if (type < e_island_record_killed_island_boss || type >= e_island_record_max)
		{
			return;
		}
		m_boss_island_info[type] = value;
		if (e_island_record_killed_island_boss == type && m_player_ptr)
		{
			m_player_ptr->set_logic_data(e_role_logic_info_boss_island_cur_kill_times, value);
		}
	}

	int32 boss_island_mgr::get_boss_island_info(int32 type)
	{
		if (type < e_island_record_killed_island_boss || type >= e_island_record_max)
		{
			return -1;
		}
		return m_boss_island_info[type];
	}

	int32 boss_island_mgr::get_left_cross_pk_chest()
	{
		ActivityCommonConfigTemplate* temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_pk);
		if (temp_ptr == nullptr)
		{
			return -1;
		}
		int32 max_chest_num = temp_ptr->ParamInt6;
		int32 cur_gather_count = m_boss_island_info[e_island_record_cross_pk_collect_count];
		return (max_chest_num - cur_gather_count);
	}

	int32 boss_island_mgr::get_left_boss_island_chest()
	{
		int32 cur_gather_count = m_boss_island_info[e_island_record_island_collect_count];
		return (GAMECONFIG->BossIslandMaxChest - cur_gather_count);
	}

	int32 boss_island_mgr::get_left_boss_island_kill()
	{
		int32 cur_gather_count = m_boss_island_info[e_island_record_killed_island_boss];
		return (GAMECONFIG->BossIslandMaxKillNum - cur_gather_count);
	}

	void boss_island_mgr::refresh_all_boss_island_info()
	{
		for (int32 i = 0; i < e_island_record_max; i++)
		{
			set_boss_island_info(i, 0);
		}
		send_player_boss_island_info_all();
		if (m_player_ptr && m_player_ptr->is_valid())
		{
			m_player_ptr->set_logic_data(e_role_logic_info_boss_island_cur_kill_times, 0);
			m_player_ptr->get_oracle_trial_mgr().send_player_oracle_trial_info_all();
		}
	}

	bool boss_island_mgr::is_finish_today_boss_island()
	{
		return get_left_boss_island_kill() <= 0;
	}

	bool boss_island_mgr::can_have_item_in_island()
	{
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		auto map_type = base_map_system::get_map_type(m_player_ptr->get_map_ent());
		if (map_type != e_map_type_boss_island)
		{
			return true;
		}
		return get_left_boss_island_kill() > 0;
	}

	void boss_island_mgr::send_boss_island_info_to_ws(int32 boss_spawn_point_template_id, guid_64 boss_guid, int32 next_refresh_stamp, const std::vector<s_gain_treasure_player_info>& player_info_arr)
	{
		cs2ws_update_boss_island_info boss_info_msg;
		boss_info_msg.boss_info.boss_spawn_point_template_id = boss_spawn_point_template_id;
		boss_info_msg.boss_info.boss_guid = boss_guid;
		boss_info_msg.boss_info.next_refresh_stamp = next_refresh_stamp;
		for (int32 i = 0; i < player_info_arr.size(); i++)
		{
			if (i >= max_team_member_num)
			{
				break;
			}
			boss_info_msg.boss_info.player_info[i] = player_info_arr[i];
		}
		connection_mgr::getInstance().send_to_ws( &boss_info_msg, sizeof(boss_info_msg));
	}

	void boss_island_mgr::send_player_boss_island_info_all()
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		game_proto_boss_island_info_all msg;
		for (int32 i = 0; i < e_island_record_max; i++)
		{
			msg.add_info_value(m_boss_island_info[i]);
		}
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_player_boss_island_info_all);
		m_player_ptr->send_logic_one(e_role_logic_info_boss_island_cur_kill_times);
	}

	void boss_island_mgr::send_player_boss_island_info_one(int32 type)
	{
		if (type < e_island_record_killed_island_boss || type >= e_island_record_max || nullptr == m_player_ptr)
		{
			return;
		}
		game_proto_boss_island_info_one msg;
		msg.set_info_type(type);
		msg.set_info_value(m_boss_island_info[type]);
		m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_player_boss_island_info_one);
		if(e_island_record_killed_island_boss == type)
			m_player_ptr->send_logic_one(e_role_logic_info_boss_island_cur_kill_times);
	}
}
