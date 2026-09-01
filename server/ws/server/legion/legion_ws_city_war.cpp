#include "utility/guid_gen.h"
#include "legion_ws_city_war.h"
#include "ws_client.hpp"
#include "legion_ws_mgr.h"
#include "../big_player_ws_mgr.h"
#include "utility/globle_data.h"
#include "../mail/event_ws_mgr.h"
#include <legion_msg.hpp>
#include "Logic/time_def.hpp"
#include "city_war_territory_mgr.h"
#include "utility/init_unit.h"
#include "Logic/activity_def.hpp"
#include "world_server.hpp"

#include "base/ecs_world.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "net.pb.h"

namespace faith
{
	legion_ws_city_war::legion_ws_city_war()
	{
		m_cross_server_territory_war_state = false;
		m_participant_max = 0;
		m_overlord_legion.clear_data();
		m_is_start_city_war = false;
		m_is_banquet_holding = false;
		m_request_hold_banquet = false;
		m_banquet_level = e_banquet_level_max;
		m_banquet_holding_time = 0.0f;
		m_participant_count = 0;
		m_need_maintain_city_war = true;
		m_city_activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_city_war);
		if (nullptr == m_city_activity_cfg_ptr)
		{
			return;
		}
	}

	void legion_ws_city_war::tick(const uint64& tick_time)
	{
		static uint64 tick_time_1sec = 0;
		if (tick_time >= tick_time_1sec)
		{
			tick_time_1sec = tick_time + second_tick_time;
			tick_1_sec(tick_time);
		}
	}

	void legion_ws_city_war::tick_1_sec(const uint64& tick_time)
	{
	}

	void legion_ws_city_war::tick_1_min(const uint64& tick_time)
	{
		if (nullptr == GAMECONFIG)
		{
			return;
		}
		if (nullptr == m_city_activity_cfg_ptr)
		{
			return;
		}


		check_cross_server_territory_war_begin();
		tick_notice(tick_time);
		begin_all_city_war(tick_time);

		
		if (!is_city_war_hold_day())
		{
			if (is_in_banquet_time()
				&& m_request_hold_banquet == true
				&& is_banquet_holding() == false)
			{
				start_hold_banquet(e_banquet_level_ordinary);
			}
			else if (is_banquet_holding() == true)
			{
				stop_hold_banquet();
			}
		}
	}

	void legion_ws_city_war::check_cross_server_territory_war_begin()
	{
		if (!m_cross_server_territory_war_state)
		{
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war) && !world_server::getInstance().is_sky_island_server()) 
			{
				city_war_territory_mgr::get_instance().territory_war_clear();
				m_cross_server_territory_war_state = true;
			}
		}
		if (m_cross_server_territory_war_state) 
		{
			if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				m_cross_server_territory_war_state = false;
			}
		}
	}

	void legion_ws_city_war::save_city_info_into_db(guid_64 overlord_legion, xchar* city_master_name)
	{
		//ws2dp_save_city_info save_city_info_msg;
		//save_city_info_msg.occupation_legion_guid = overlord_legion;
		//memcpy(save_city_info_msg.city_master_name, city_master_name, max_name_size);
		//ws_client::getInstance().send_to_dp(&save_city_info_msg, sizeof(save_city_info_msg));

		xstring t_city_master_name = city_master_name;
		script_mgr::get_instance().call_func("ws2dp_send", "send_save_city_info_lua", 0, false, "%l%s", overlord_legion.server_64, t_city_master_name.c_str());

	}

	void legion_ws_city_war::set_city_master_name(const xchar* city_master_name, bool is_need_save)
	{
		memset(m_city_master_name, 0, sizeof(m_city_master_name));
		memcpy(m_city_master_name, city_master_name, max_name_size);

		if (true == is_need_save)
		{
			save_city_info_into_db(get_overlord_legion(), m_city_master_name);
		}
	}

	void legion_ws_city_war::set_overlord_legion(guid_64 legion_guid, bool is_from_db, bool is_from_cross_city)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		//guid_64 old_city_master_guid = get_city_master_guid();
		//send_del_city_master_stuff(old_city_master_guid);
		//send_del_city_war_winner_title(m_overlord_legion);
		m_overlord_legion = legion_guid;
		
		if (!is_from_cross_city)
		{
			city_war_territory_mgr::get_instance().set_occupy_legion_guid(overlord_war_territory_flag, legion_guid);
			big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_overload_legion, legion_guid);
		}
		if (is_from_db)
		{
			//s_city_war_bid_info bid_info_db;
			//bid_info_db.territory_id = overlord_war_territory_flag;
			//bid_info_db.index_in_terr = territory_occupier_bid_idx;
			//bid_info_db.legion_guid = legion_guid;
			//bid_info_db.cur_bid_price = territory_occupier_price_flag;
			//city_war_territory_mgr::get_instance().load_bid_info(bid_info_db);
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		guid_64 new_city_master_guid = guid_64();
		if (!is_from_cross_city)
		{
			new_city_master_guid = legion_ws_ptr->get_chief_guid();
			big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_lord_of_city, new_city_master_guid);
		}
		else
		{
			new_city_master_guid = big_player_ws_mgr::get_instance().get_big_player_guid(e_big_player_type_lord_of_city);
		}



		client_session* new_city_master_session = client_session_mgr::getInstance().get_session(new_city_master_guid);
		if (new_city_master_session)
		{
			send_get_city_master_stuff(new_city_master_session);
		}

		send_add_city_war_winner_title(m_overlord_legion);

		if (!is_from_cross_city)
		{
			legion_proto_get_city_master_info_end get_city_master_info_end_msg;
			get_city_master_info_end_msg.set_legion_guid(m_overlord_legion.server_64);
			client_session_mgr::getInstance().send_message_to_all_client(&get_city_master_info_end_msg, e_msgindex_s2c_get_city_master_info_end);
		}
	}

	xchar* legion_ws_city_war::get_overlord_legion_name()
	{
		const guid_64& overlord_legion = get_overlord_legion();
		if (false == overlord_legion.is_valid())
		{
			return empty_char_array;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(overlord_legion);
		if (nullptr == legion_ws_ptr)
		{
			return empty_char_array;
		}

		char* legion_name = legion_ws_ptr->get_legion_name();
		if (nullptr == legion_name)
		{
			return empty_char_array;
		}
		return legion_name;
	}

	void legion_ws_city_war::send_city_war_end_notice(guid_64 legion_guid)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_legion(legion_guid);
		if (legion_ws_ptr == nullptr)
		{
			return;
		}
		s_legion_member_info* member_info = legion_ws_ptr->get_member(legion_ws_ptr->get_chief_guid());
		if (nullptr == member_info)
		{
			return;
		}
		int32 notice_id = GAMECONFIG->LegionCityWarWinNotice;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(legion_ws_ptr->get_legion_name());
		vec_notice_str.push_back(member_info->role_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, legion_ws_ptr->get_chief_guid(), notice_str);
	}

	guid_64 legion_ws_city_war::get_occupy_legion_guid(int32 terr_id)
	{
		return city_war_territory_mgr::get_instance().get_occupy_legion_guid(terr_id);
	}

	char* legion_ws_city_war::get_occupy_legion_name(int32 terr_id)
	{
		guid_64 occupation_legion_guid = get_occupy_legion_guid(terr_id);
		if (false == occupation_legion_guid.is_valid())
		{
			return empty_char_array;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(occupation_legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return empty_char_array;
		}

		char* legion_name = legion_ws_ptr->get_legion_name();
		if (nullptr == legion_name)
		{
			return empty_char_array;
		}
		return legion_name;
	}

	guid_64 legion_ws_city_war::get_city_master_guid()
	{
		//guid_64 overlord_legion = get_overlord_legion();
		//if (false == overlord_legion.is_valid())
		//{
		//	return guid_64();
		//}
		//legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(overlord_legion);
		//if (nullptr == legion_ws_ptr)
		//{
		//	return guid_64();
		//}
		//return legion_ws_ptr->get_chief_guid();
		return big_player_ws_mgr::get_instance().get_big_player_guid(e_big_player_type_lord_of_city);
	}

	xchar* legion_ws_city_war::get_city_master_name()
	{
		return m_city_master_name;
	}

	xchar* legion_ws_city_war::get_city_master_name_old()
	{
		guid_64 overlord_legion = get_overlord_legion();
		if (false == overlord_legion.is_valid())
		{
			return empty_char_array;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(overlord_legion);
		if (nullptr == legion_ws_ptr)
		{
			return empty_char_array;
		}
		guid_64 city_master_guid = legion_ws_ptr->get_chief_guid();
		if (city_master_guid.is_valid() == false)
		{
			return empty_char_array;
		}
		s_legion_member_info* member_info = legion_ws_ptr->get_member(city_master_guid);
		if (nullptr == member_info)
		{
			return empty_char_array;
		}
		return member_info->role_name;
	}

	void legion_ws_city_war::check_and_send_city_master_online_acconcement(const guid_64& online_guid)
	{
		if (online_guid.is_valid() == false)
		{
			return;
		}
		guid_64 city_master_guid = get_city_master_guid();
		if (city_master_guid.is_valid() == false)
		{
			return;
		}
		if (online_guid != city_master_guid)
		{
			return;
		}

		xchar* city_master_name = nullptr;
		client_session* online_client_ptr = client_session_mgr::getInstance().get_session(online_guid);
		if (nullptr != online_client_ptr)
		{
			city_master_name = online_client_ptr->get_role_name();
		}

		if (nullptr == city_master_name)
		{
			return;
		}
		
		int32 notice_id = legion_city_master_online_notice_id;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			notice_id = new_legion_city_master_online_notice_id;
		}
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(city_master_name);
		xstring notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}

	void legion_ws_city_war::send_get_city_master_stuff(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		ws2cs_get_city_master_stuff get_city_master_stuff_msg;
		get_city_master_stuff_msg.role_guid = session->get_role_guid();
		bool is_use = proto_by_lua(e_msg_index_ws2cs_get_city_master_stuff);
		if (is_use == false)
		{
			session->send_to_cs(&get_city_master_stuff_msg, sizeof(get_city_master_stuff_msg));
		}
		else
		{
			faith::ws2cs_proto::get_city_master_stuff pro_msg;
			get_city_master_stuff_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_get_city_master_stuff);
		}
	}

	void legion_ws_city_war::send_del_city_master_stuff(const guid_64& city_master_guid)
	{
		client_session* session = client_session_mgr::getInstance().get_session(city_master_guid);
		if (nullptr == session)
		{
			return;
		}

		ws2cs_del_city_master_stuff del_city_master_stuff_msg;
		del_city_master_stuff_msg.role_guid = city_master_guid;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_del_city_master_stuff);
		if (is_use == false)
		{
			session->send_to_cs(&del_city_master_stuff_msg, sizeof(del_city_master_stuff_msg));
		}
		else
		{
			faith::ws2cs_proto::del_city_master_stuff pro_msg;
			del_city_master_stuff_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_del_city_master_stuff);
		}
	}
	void legion_ws_city_war::send_one_add_city_war_winner_title(client_session* member_session)
	{
		if (nullptr == member_session)
		{
			return;
		}
		ws2cs_add_city_war_winner_stuff add_pak;
		add_pak.role_guid = member_session->get_role_guid();
		bool is_use = proto_by_lua(e_msg_index_ws2cs_add_city_war_winner_stuff);
		if (is_use == false)
		{
			member_session->send_to_cs(&add_pak, sizeof(add_pak));
		}
		else
		{
			faith::ws2cs_proto::add_city_war_winner_stuff pro_msg;
			add_pak.to_proto(pro_msg);
			member_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_add_city_war_winner_stuff);
		}
	}

	void legion_ws_city_war::send_one_del_city_war_winner_title(const guid_64& member_guid)
	{
		client_session* member_session = client_session_mgr::getInstance().get_session(member_guid);
		if (nullptr == member_session)
		{
			return;
		}
		ws2cs_del_city_war_winner_stuff add_pak;
		add_pak.role_guid = member_guid;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_del_city_war_winner_stuff);
		if (is_use == false)
		{
			member_session->send_to_cs(&add_pak, sizeof(add_pak));
		}
		else
		{
			faith::ws2cs_proto::del_city_war_winner_stuff pro_msg;
			add_pak.to_proto(pro_msg);
			member_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_del_city_war_winner_stuff);
		}
	}

	void legion_ws_city_war::send_add_city_war_winner_title(const guid_64& new_legion_winner_guid)
	{
		if (new_legion_winner_guid.is_valid() == false)
		{
			return;
		}
		
		legion_ws* new_winner_legion = legion_ws_mgr::get_instance().get_legion(new_legion_winner_guid);
		if (nullptr == new_winner_legion)
		{
			return;
		}

		int32 member_count = new_winner_legion->get_member_count();
		std::list<s_legion_member_info>& member_list = new_winner_legion->get_member_list();
		auto iter = member_list.begin();
		for (; iter != member_list.end(); ++iter)
		{
			client_session* member_session = client_session_mgr::getInstance().get_session(iter->role_guid);
			if (nullptr == member_session)
			{
				continue;
			}
			send_one_add_city_war_winner_title(member_session);
		}	
	}

	void legion_ws_city_war::send_del_city_war_winner_title(const guid_64& old_legion_winner_guid)
	{
		if (old_legion_winner_guid.is_valid() == false)
		{
			return;
		}
		
		legion_ws* old_winner_legion = legion_ws_mgr::get_instance().get_legion(old_legion_winner_guid);
		if (nullptr == old_winner_legion)
		{
			return;
		}
		int32 member_count = old_winner_legion->get_member_count();
		std::list<s_legion_member_info>& member_list = old_winner_legion->get_member_list();
		auto iter = member_list.begin();
		for (; iter != member_list.end(); ++iter)
		{
			client_session* member_session = client_session_mgr::getInstance().get_session(iter->role_guid);
			if (nullptr == member_session)
			{
				continue;
			}
			send_one_del_city_war_winner_title(iter->role_guid);
		}

	}


	void legion_ws_city_war::ws2dp_del_overlord_legion_stuff_proc(e_del_overlord_stuff_event del_type, const guid_64& target_guid)
	{
		//ws2dp_clear_overlord_legion_stuff pak;
		//pak.del_stuff_type = del_type;
		//pak.target_guid = target_guid;
		//ws_client::getInstance().send_to_dp(&pak, sizeof(ws2dp_clear_overlord_legion_stuff));

		script_mgr::get_instance().call_func("ws2dp_send", "send_clear_city_war_reward_stuff_lua", 0, false, "%d%l", (int32)del_type, target_guid.server_64);
	}

	bool legion_ws_city_war::is_city_war_hold_day(int32 terr_id)
	{
		time_info cur_time_info = time_helper::get_cur_time_new();
		if (terr_id == overlord_war_territory_flag)
		{
			//ActivityCommonConfigTemplate* _activity_type_overlord_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
			//if (_activity_type_overlord_ptr == nullptr || _activity_type_overlord_ptr->OpenDate.size() < 1)
			//{
			//	return false;
			//}
			//if (_activity_type_overlord_ptr->ParamInt1 > 0)
			//{
			//	int32 server_open_days = globle_data::get_instance().get_server_on_days();
			//	if (server_open_days < _activity_type_overlord_ptr->ParamInt1)
			//	{
			//		return false;
			//	}
			//	else if (server_open_days == _activity_type_overlord_ptr->ParamInt1)
			//	{
			//		return true;
			//	}
			//}

			//return cur_time_info.day_in_week == _activity_type_overlord_ptr->OpenDate[0];
			return false;
		}
		int32 activityId = 0;
		if (world_server::getInstance().is_sky_island_server() || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			activityId = faith::e_activity_type_city_war_cross_server;
		}
		else
		{
			activityId = faith::e_activity_type_city_war;
		}

		std::vector<int32> open_data = world_server::getInstance().get_activity_open_data(activityId);
		for (int32 i = 0; i < open_data.size(); ++i)
		{
			if (cur_time_info.day_in_week == open_data[i])
			{
				return true;
			}
		}

		return false;
	}

	bool legion_ws_city_war::is_city_war_begin(int32 terr_id)
	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return false;
		}
		return terr_info_ptr->city_war_map_guid.is_valid();
	}

	bool legion_ws_city_war::is_city_war_end_today(int32 terr_id)
	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return false;
		}

		return !time_helper::is_diff_day(terr_info_ptr->city_war_finish_stamp);
	}

  	int32 legion_ws_city_war::get_legion_bid_index(int32 terr_id, guid_64 legion_guid)
  	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return -1;
		}
		s_city_war_bid_info* bid_info_ptr = terr_info_ptr->get_bid_info(legion_guid);
		if (nullptr == bid_info_ptr)
		{
			return -1;
		}
  		return bid_info_ptr->index_in_terr;
  	}
  
  	bool legion_ws_city_war::exist_bid_info(int32 terr_id, guid_64 legion_guid)
  	{
  		return get_legion_bid_index(terr_id, legion_guid) > -1;
  	}

	bool legion_ws_city_war::exist_bid_info(guid_64 map_guid, guid_64 legion_guid)
	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info_by_map(map_guid);
		if (nullptr == terr_info_ptr)
		{
			return false;
		}
		return exist_bid_info(terr_info_ptr->territory_id, legion_guid);
	}
	
	bool legion_ws_city_war::is_during_city_war(guid_64 legion_guid)
	{
		template_manager::template_type* territory_cfgs = template_manager::get_instance().get_templates(e_CityWarTerritoryTemplate);
		if (nullptr == territory_cfgs)
		{
			return false;
		}
		template_manager::template_type::iterator ite;
		for (ite = territory_cfgs->begin(); ite != territory_cfgs->end(); ++ite)
		{
			CityWarTerritoryTemplate* territory_cfg = (CityWarTerritoryTemplate*)(ite->second);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			if (is_city_war_begin(territory_cfg->attribute_id) && exist_bid_info(territory_cfg->attribute_id, legion_guid))
			{
				return true;
			}
		}


		return false;
	}

	void legion_ws_city_war::begin_all_city_war(const uint64& tick_time)
	{	
		template_manager::template_type* territory_cfgs = template_manager::get_instance().get_templates(e_CityWarTerritoryTemplate);
		if (nullptr == territory_cfgs)
		{
			return;
		}
		
		struct tm* cur_date = time_helper::get_localtime();
		if (nullptr == cur_date)
		{
			return;
		}
		struct tm cur_time = *cur_date;
		tm begin_maintain_tm, end_maintain_tm;
		get_maintain_time(begin_maintain_tm, end_maintain_tm);
		if (cur_time.tm_hour < begin_maintain_tm.tm_hour || cur_time.tm_hour > end_maintain_tm.tm_hour)
		{
			m_need_maintain_city_war = true;
		}
		else if (cur_time.tm_hour == begin_maintain_tm.tm_hour && cur_time.tm_min < begin_maintain_tm.tm_min)
		{
			m_need_maintain_city_war = true;
		}
		//else if (cur_time.tm_hour == end_maintain_tm.tm_hour && cur_time.tm_min >= end_maintain_tm.tm_min)
		//{
		//	m_need_maintain_city_war = true;
		//}
		else if (cur_time.tm_min >= begin_maintain_tm.tm_min && cur_time.tm_min <= end_maintain_tm.tm_min && m_need_maintain_city_war == true)
		{
			m_need_maintain_city_war = false;

			if (world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				city_war_territory_mgr::get_instance().send_cross_territory_war_maintain_terrories_to_other_server(0);
			}
			if (!world_server::getInstance().is_sky_island_server()) 
			{
				legion_ws_mgr& legion_ws_msg_ref = legion_ws_mgr::get_instance();
				legion_ws_map& legion_map = legion_ws_msg_ref.get_legion_map();
				legion_ws_map_it ite;
				for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
				{
					legion_ws& legion_ws_ref = ite->second;
					legion_ws_ref.maintain_territories();
				}
			}
		}
		bool is_war_begun = false;
		bool is_overload_war_begun = false;
		tm begin_tm, end_tm;
		template_manager::template_type::iterator ite;
		for (ite = territory_cfgs->begin(); ite != territory_cfgs->end(); ++ite)
		{
			CityWarTerritoryTemplate* territory_cfg = (CityWarTerritoryTemplate*)(ite->second);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			if (!is_city_war_hold_day(territory_cfg->attribute_id))
			{
				continue;
			}
			if (territory_cfg->attribute_id == overlord_war_territory_flag && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				continue;
			}
			if (!world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war) && territory_cfg->attribute_id != overlord_war_territory_flag)
			{
				continue;
			}
			get_begin_and_end_tm(territory_cfg->attribute_id, begin_tm, end_tm);
			if (cur_time.tm_hour < begin_tm.tm_hour || cur_time.tm_hour > end_tm.tm_hour)
			{
				continue;
			}
			if (cur_time.tm_hour == begin_tm.tm_hour && cur_time.tm_min < begin_tm.tm_min)
			{
				continue;
			}
			if (cur_time.tm_hour == end_tm.tm_hour && cur_time.tm_min >= end_tm.tm_min)
			{
				continue;
			}


			//if (cur_time.tm_hour != begin_tm.tm_hour || cur_time.tm_min != begin_tm.tm_min)
			//{
			//	continue;
			//}
			begin_war(territory_cfg->attribute_id);

			is_war_begun = true;
			if (territory_cfg->attribute_id == overlord_war_territory_flag)
			{
				is_overload_war_begun = true;
			}
		}

		if (is_war_begun)
		{
			if (!is_overload_war_begun)
			{
				add_legion_bouns_count_city_war();
			}
		}
	}
	void legion_ws_city_war::add_legion_bouns_count_city_war()
	{
		legion_ws_mgr& legion_ws_msg_ref = legion_ws_mgr::get_instance();
		legion_ws_map& legion_map = legion_ws_msg_ref.get_legion_map();
		legion_ws_map_it ite;
		for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
		{
			legion_ws& legion_ws_ref = ite->second;
			int32 finish_count = legion_ws_ref.get_city_war_territories().size();
			s_legion_bonus_info& bonus_info = legion_ws_ref.get_bonus_info_ins().get_bonus_one(e_legion_bonus_city_war_count);
			bonus_info.finish_count = finish_count;
			legion_ws_ref.get_bonus_info_ins().add_legion_bonus_info_map(bonus_info);
		}
	}
	int32 legion_ws_city_war::get_bid_info_count(int32 terr_id)
	{
		int32 bid_info_count = 0;
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return bid_info_count;
		}
		for (int32 i = 0; i < max_apply_city_war_legion_num; ++i)
		{
			s_city_war_bid_info* bid_info_ptr = terr_info_ptr->get_bid_info(i);
			if (bid_info_ptr && bid_info_ptr->is_valid())
			{
				++bid_info_count;
			}
		}

		return bid_info_count;
	}

	void legion_ws_city_war::begin_war(int32 terr_id)
	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return;
		}

		if (is_city_war_begin(terr_id) || is_city_war_end_today(terr_id))
		{
			return;
		}

		//if (overlord_war_territory_flag == terr_id)
		//{
		//	del_overlord_legion();
		//}

		//int32 notice_id = legion_city_war_open_notice_id;
		//std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		//int32 create_time = time_helper::get_time();
		//event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);

		if (get_bid_info_count(terr_id) <= 1)
		{
			guid_64 win_legion_guid = guid_64();
			int32 legion_bid_price = -1;
			int32 server_id = 0;
			for (int32 i = 0; i < max_apply_city_war_legion_num; ++i)
			{
				s_city_war_bid_info* bid_info_ptr = terr_info_ptr->get_bid_info(i);
				if (bid_info_ptr && bid_info_ptr->is_valid())
				{
					win_legion_guid = bid_info_ptr->legion_guid;
					legion_bid_price = bid_info_ptr->cur_bid_price;
					server_id = bid_info_ptr->server_id;
					break;
				}
			}

			if (win_legion_guid.is_valid() && legion_bid_price == territory_occupier_price_flag)
			{
				end_war(terr_id, win_legion_guid, false);
				return;
			}
		}

		auto map_ent = cs_map_mgr_system::init_map_by_battle(city_war_map_id);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("begin_war map_ent is nullptr city_war_map_id:{}", city_war_map_id);
			return;
		}
		city_war_territory_mgr::get_instance().on_begin_war(terr_id, map_ent->getEntityId());

		ws2cs_begin_city_war begin_war_msg;
		begin_war_msg.cur_terr_id = terr_id;
		begin_war_msg.city_war_map_guid = map_ent->getEntityId();
		for (int32 i = 0; i < max_apply_city_war_legion_num; ++i)
		{
			s_city_war_bid_info* bid_info_ptr = terr_info_ptr->get_bid_info(i);
			if (nullptr == bid_info_ptr || bid_info_ptr->is_valid() == false)
			{
				continue;
			}
			guid_64& legion_guid = bid_info_ptr->legion_guid;
			if (!world_server::getInstance().is_sky_island_server())
			{
				legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
				if (nullptr == legion_ws_ptr)
				{
					continue;
				}
			}
			begin_war_msg.join_war_legions[i].legion_guid = legion_guid;
			memcpy(begin_war_msg.join_war_legions[i].legion_name, bid_info_ptr->legion_name, max_name_size);
			begin_war_msg.join_war_legions[i].bid_price = bid_info_ptr->cur_bid_price;
		}


		bool is_use = proto_by_lua(e_msg_index_ws2cs_begin_city_war);
		if (is_use == false)
		{
			cs_map_system::send_message_to_cs(map_ent, &begin_war_msg, sizeof(begin_war_msg));
		}
		else
		{
			faith::ws2cs_proto::begin_city_war pro_msg;
			begin_war_msg.to_proto(pro_msg);
			cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msg_index_ws2cs_begin_city_war);
		}

		//new_map_ws_ptr->send_message_to_cs(&begin_war_msg, sizeof(begin_war_msg));
	}

	void legion_ws_city_war::del_overlord_legion(bool is_legion_del, bool is_cross_city)
	{
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(m_overlord_legion); 
		if (legion_ptr == nullptr)
		{
			return;
		}
		guid_64 city_master = get_city_master_guid();
		if (false == is_legion_del)
		{
			send_del_city_master_stuff(city_master);
			if (!is_cross_city)
			{
				big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_lord_of_city, 0);
			}
			save_city_info_into_db(0, empty_char_array);
		}
		else
		{
			save_city_info_into_db(0,get_city_master_name());
		}
		ws2dp_del_overlord_legion_stuff_proc(e_del_overlord_stuff_event_all_legion, guid_64());
		send_del_city_war_winner_title(m_overlord_legion);
		if (!is_cross_city)
		{
			big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_overload_legion, 0);
			legion_proto_get_city_master_info_end get_city_master_info_end_msg;
			client_session_mgr::getInstance().send_message_to_all_client(&get_city_master_info_end_msg, e_msgindex_s2c_get_city_master_info_end);
		}
		m_overlord_legion.clear_data();
	}

	void legion_ws_city_war::end_war(int32 terr_id, guid_64 winner_legion_guid, bool need_send_notice)
	{
		if (terr_id == overlord_war_territory_flag)
		{
			legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(winner_legion_guid);
			if (nullptr != legion_ptr)
			{
				legion_ptr->refresh_master_city_war_can_award();
			}
			set_overlord_legion(winner_legion_guid, false);
			set_city_master_name(get_city_master_name_old(), false);
			save_city_info_into_db(winner_legion_guid, get_city_master_name_old());
			send_city_war_end_notice(winner_legion_guid);
		}
		city_war_territory_mgr::get_instance().on_end_war(terr_id, winner_legion_guid, need_send_notice);
	}

	void legion_ws_city_war::end_war(guid_64 map_guid, guid_64 winner_legion_guid)
	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info_by_map(map_guid);
		if (nullptr == terr_info_ptr)
		{
			return;
		}
		end_war(terr_info_ptr->territory_id, winner_legion_guid);
	}

	guid_64 legion_ws_city_war::get_city_war_map_guid(int32 terr_id)
	{
		ZoneScoped;
		guid_64 city_war_map_guid = guid_64();
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			CONSOLE_ERROR("get_city_war_map_guid terr_info_ptr is nullptr");
			return city_war_map_guid;
		}
		city_war_map_guid = terr_info_ptr->city_war_map_guid;
		auto map_ent = get_entity(city_war_map_guid);
		if (map_ent)
		{
			city_war_map_guid = map_ent->getEntityId();
		}
		else
		{
			city_war_map_guid.clear_data();
		}
		return city_war_map_guid;
	}

	void legion_ws_city_war::get_maintain_time(tm& begin_tm, tm& end_tm)
	{
		begin_tm = *time_helper::get_localtime();
		begin_tm.tm_sec = 0;
		end_tm = *time_helper::get_localtime();
		end_tm.tm_sec = 0;
		int32 activityId = 0;
		if (world_server::getInstance().is_sky_island_server() || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			activityId = faith::e_activity_type_city_war_cross_server;
		}
		else
		{
			activityId = faith::e_activity_type_city_war;
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(activityId);
		if (open_time.size() < once_activity_open_time_array_size)
		{
			return;
		}
		begin_tm.tm_hour = open_time[2];
		begin_tm.tm_min = open_time[3];
		end_tm.tm_hour = open_time[4];
		end_tm.tm_min = open_time[5];
	
	}

	void legion_ws_city_war::get_begin_and_end_tm(int32 terr_id, tm& begin_tm, tm& end_tm)
	{
		begin_tm = *time_helper::get_localtime();
		begin_tm.tm_sec = 0;
		end_tm = *time_helper::get_localtime();
		end_tm.tm_sec = 0;

		int32 activityId = 0;
		if (world_server::getInstance().is_sky_island_server() || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			activityId = faith::e_activity_type_city_war_cross_server;
		}
		else
		{
			activityId = faith::e_activity_type_city_war;
		}

		ActivityCommonConfigTemplate* act_com_ptr =GET_TEMPLATE(ActivityCommonConfigTemplate, faith::first_activity_common_template_id + activityId);
		if (terr_id == faith::overlord_war_territory_flag)
		{
			int32 ActTemplateID = faith::first_activity_common_template_id + faith::e_activity_type_overlord_city_war;
			act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, ActTemplateID);
		}

		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(activityId);
		if (nullptr == act_com_ptr || open_time.size() < once_activity_open_time_array_size)
		{
			return;
		}
		
		begin_tm.tm_hour = open_time[4];
		begin_tm.tm_min  = open_time[5];
		end_tm.tm_hour = open_time[6];
		end_tm.tm_min  = open_time[7];
	}

	bool legion_ws_city_war::is_during_city_war_time(int32 terr_id)
	{
		int32 activityId = 0;
		if (world_server::getInstance().is_sky_island_server() || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			activityId = faith::e_activity_type_city_war_cross_server;
		}
		else
		{
			activityId = faith::e_activity_type_city_war;
		}
		ActivityCommonConfigTemplate* act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, faith::first_activity_common_template_id + activityId);
		if (terr_id == faith::overlord_war_territory_flag)
		{
			int32 ActTemplateID = faith::first_activity_common_template_id + faith::e_activity_type_overlord_city_war;
			act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, ActTemplateID);
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(activityId);
		if (nullptr == act_com_ptr || open_time.size() < once_activity_open_time_array_size)
		{
			return false;
		}
		if (!is_city_war_hold_day(terr_id))
		{
			return false;
		}

		uint32 begin_stamp = time_helper::get_stamp_by_hour_min(open_time[4], open_time[5]);
		uint32 end_stamp = time_helper::get_stamp_by_hour_min(open_time[6], open_time[7]);
		uint32 now_stamp = time_helper::get_cur_time_new().second;
		return now_stamp > begin_stamp && now_stamp <= end_stamp;
	}

	bool legion_ws_city_war::is_during_city_war_prepare_time(int32 terr_id)
	{
		int32 activityId = 0;
		if (world_server::getInstance().is_sky_island_server() || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			activityId = faith::e_activity_type_city_war_cross_server;
		}
		else
		{
			activityId = faith::e_activity_type_city_war;
		}
		ActivityCommonConfigTemplate* act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, faith::first_activity_common_template_id + activityId);
		if (terr_id == faith::overlord_war_territory_flag)
		{
			int32 ActTemplateID = faith::first_activity_common_template_id + faith::e_activity_type_overlord_city_war;
			act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, ActTemplateID);
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(activityId);
		if (nullptr == act_com_ptr || open_time.size() < once_activity_open_time_array_size)
		{
			return false;
		}
		if (!is_city_war_hold_day(terr_id))
		{
			return false;
		}

		uint32 begin_stamp = time_helper::get_stamp_by_hour_min(open_time[2], open_time[3]);
		uint32 end_stamp = time_helper::get_stamp_by_hour_min(open_time[4], open_time[5]);
		
		uint32 now_stamp = time_helper::get_cur_time_new().second;
		return now_stamp > begin_stamp && now_stamp <= end_stamp;
	}

	bool legion_ws_city_war::is_during_bid_time(int32 terr_id)
	{
		if (!is_city_war_hold_day(terr_id))
		{
			return false;
		}
		if (is_during_city_war_prepare_time(terr_id) || is_during_city_war_time(terr_id))
		{
			return false;
		}
		int32 activityId = 0;
		if (world_server::getInstance().is_sky_island_server() || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			activityId = faith::e_activity_type_city_war_cross_server;
		}
		else
		{
			activityId = faith::e_activity_type_city_war;
		}

		ActivityCommonConfigTemplate* act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, faith::first_activity_common_template_id + activityId);
		if (terr_id == faith::overlord_war_territory_flag)
		{
			int32 ActTemplateID = faith::first_activity_common_template_id + faith::e_activity_type_overlord_city_war;
			act_com_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, ActTemplateID);
		}
		std::vector<int32> open_time = world_server::getInstance().get_activity_open_time(activityId);
		if (nullptr == act_com_ptr || open_time.size() < once_activity_open_time_array_size)
		{
			return false;
		}
		uint32 begin_stamp = time_helper::get_stamp_by_hour_min(legion_city_war_bid_time_begin, 0);
		uint32 end_stamp = time_helper::get_stamp_by_hour_min(open_time[2], open_time[3]);
		uint32 now_stamp = time_helper::get_cur_time_new().second;
		if (now_stamp > begin_stamp && now_stamp < end_stamp)
		{
			return true;
		}
		return false;
	}

	bool legion_ws_city_war::is_banquet_holding()
	{
		if (m_is_banquet_holding)
		{
			return true;
		}

		return false;
	}

	e_legion_error legion_ws_city_war::request_hold_banquet(e_banquet_level banquet_level)
	{
		if (banquet_level >= e_banquet_level_max)
		{
			return e_legion_error_invalid_banquet_type;
		}

		if (is_city_war_hold_day() == true)
		{
			return e_legion_error_can_not_hold_banquet_in_legion_war_day;
		}

		if (is_in_banquet_time())
		{
			return e_legion_error_invalid_hold_banquet_time;
		}

		if (true == m_request_hold_banquet)
		{
			return e_legion_error_request_hold_banquet_again;
		}

		m_request_hold_banquet = true;
		m_banquet_level = banquet_level;

		return e_legion_error_none;
	}

	bool legion_ws_city_war::is_in_banquet_time()
	{
		if (nullptr == GAMECONFIG || GAMECONFIG->CityBanquetBeginTime.size() < 4)
		{
			return false;
		}
		int32 begin_banquet_hour = GAMECONFIG->CityBanquetBeginTime[0];
		int32 begin_banquet_min  = GAMECONFIG->CityBanquetBeginTime[1];

		int32 end_banquet_hour = GAMECONFIG->CityBanquetBeginTime[2];
		int32 end_banquet_min  = GAMECONFIG->CityBanquetBeginTime[3];

		int32 begin_banquet_time_in_min = begin_banquet_hour * 60 + begin_banquet_min;
		int32 end_banquet_time_in_min = end_banquet_hour * 60 + end_banquet_min;
		
		tm cur_time_struct = *time_helper::get_localtime();
		int32 cur_time_in_min = cur_time_struct.tm_hour * 60 + cur_time_struct.tm_min;
		return cur_time_in_min >= begin_banquet_time_in_min && cur_time_in_min < end_banquet_time_in_min;
	}

	void legion_ws_city_war::start_hold_banquet(e_banquet_level banquet_level)
	{
		switch (m_banquet_level)
		{
		case e_banquet_level_ordinary:
			m_participant_max = GAMECONFIG->MaxParticipantNumOrdinary;
			break;
		case e_banquet_level_hearty:
			m_participant_max = GAMECONFIG->MaxParticipantNumHearty;
			break;
		case e_banquet_level_luxury:
			m_participant_max = GAMECONFIG->MaxParticipantNumLuxury;
			break;

		default:
			return;
		}

		m_banquet_holding_time = 0.0f;
		m_participant_count = 0;
		m_participant_record.clear();

		m_is_banquet_holding = true;

		int32 notice_id = legion_city_dance_open_notice_id;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}

	void legion_ws_city_war::stop_hold_banquet()
	{
		m_is_banquet_holding = false;

		if (GAMECONFIG->TakePartInOrdinaryBanquetCostMoney.size() < 2
			|| GAMECONFIG->TakePartInHeartyBanquetCostMoney.size() < 2
			|| GAMECONFIG->TakePartInLuxuryBanquetCostMoney.size() < 2
			|| GAMECONFIG->BanquetCityMasterEarnRate <= 0)
		{
			return;
		}
		int32 take_part_in_banquet_cost = 0;
		switch (get_banquet_level())
		{
		case e_banquet_level_ordinary:
			take_part_in_banquet_cost = GAMECONFIG->TakePartInOrdinaryBanquetCostMoney[1];
			break;
		case e_banquet_level_hearty:
			take_part_in_banquet_cost = GAMECONFIG->TakePartInHeartyBanquetCostMoney[1];
			break;
		case e_banquet_level_luxury:
			take_part_in_banquet_cost = GAMECONFIG->TakePartInLuxuryBanquetCostMoney[1];
			break;
		}
		int32 city_master_earn_money_num = ((take_part_in_banquet_cost * m_participant_count) / GAMECONFIG->BanquetCityMasterEarnRate);
		
		std::vector<s_item_info> empty_vec;
		const std::vector<std::string>& temp_array_list = template_manager::get_instance().get_string_list(90090451);
		if (temp_array_list.size() < 3)
		{
			return;
		}

		event_ws_mgr::get_instance().send_mail_to_player_by_system(
			get_city_master_guid(),
			0,
			temp_array_list[0],
			temp_array_list[1],
			temp_array_list[2],
			e_money_type_jewel,
			city_master_earn_money_num,
			0,
			0,
			empty_vec
		);

		m_request_hold_banquet = false;
	}

	void legion_ws_city_war::take_part_in_banquet(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		guid_64 role_guid = session->get_role_guid();

		if (m_participant_count >= m_participant_max)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_banquet_already_over);
			return;
		}

		unit_guid_map_it ite = m_participant_record.find(role_guid.server_64);
		if (ite == m_participant_record.end())
		{
			m_participant_record.insert({ role_guid.server_64, 0 });
			ite = m_participant_record.find(role_guid.server_64);
			if (ite == m_participant_record.end())
			{
				return;
			}
		}

		if (ite->second >= max_take_part_in_banquet_per_role)
		{
			legion_ws_mgr::get_instance().send_legion_error_to_session(session, e_legion_error_used_all_take_part_in_banquet_times);
			return;
		}
		
		ite->second += 1;
		m_participant_count += 1;

		ws2cs_get_take_part_in_banquet_award get_take_part_in_banquet_award_msg;
		get_take_part_in_banquet_award_msg.role_guid = role_guid;
		get_take_part_in_banquet_award_msg.banquet_level = get_banquet_level();
		session->send_to_cs(&get_take_part_in_banquet_award_msg, sizeof(get_take_part_in_banquet_award_msg));

		legion_proto_take_part_in_banquet_end take_part_in_banquet_end_msg;
		take_part_in_banquet_end_msg.set_participant_num(m_participant_count);
		take_part_in_banquet_end_msg.set_participant_max(m_participant_max);
		take_part_in_banquet_end_msg.set_role_take_part_in_num(ite->second);
		session->send_to_client(&take_part_in_banquet_end_msg, e_msgindex_s2c_take_part_in_banquet_end);

		if (m_participant_count >= m_participant_max)
		{
			stop_hold_banquet();
		}
	}

	bool legion_ws_city_war::fill_get_city_war_info_msg(legion_proto_get_city_war_info_end& get_city_war_info_msg, int32 begin_idx, int32 end_idx)
	{
//		territory_lists_type& territories = city_war_territory_mgr::get_instance().get_territory_lists();
// 		for (territory_lists_type::iterator iter = territories.begin(); iter != territories.end(); ++iter)
// 			s_city_war_territory_info& terr_info = iter->second;
		template_manager::template_type* territory_cfgs = template_manager::get_instance().get_templates(e_CityWarTerritoryTemplate);
		if (nullptr == territory_cfgs)
		{
			return false;
		}
		int32 terr_idx = 0;
		template_manager::template_type::iterator ite;
		for (ite = territory_cfgs->begin(); ite != territory_cfgs->end(); ++ite)
		{
			CityWarTerritoryTemplate* territory_cfg = (CityWarTerritoryTemplate*)(ite->second);
			if (nullptr == territory_cfg || overlord_war_territory_flag == territory_cfg->attribute_id)
			{
				continue;
			}
			terr_idx++;
			if (begin_idx >= 0 && terr_idx < begin_idx)
			{
				continue;
			}
			if (end_idx >= 0 && terr_idx > end_idx)
			{
				break;
			}
			s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(territory_cfg->attribute_id);
			if (nullptr == terr_info_ptr)
			{
				continue;
			}
			s_city_war_territory_info& terr_info_ref = *terr_info_ptr;
			legion_proto_city_war_territory_info* territory_info_msg = get_city_war_info_msg.add_territory_info();
			fill_territory_info_msg(territory_info_msg, terr_info_ref);
		}

		return true;
	}

	void legion_ws_city_war::fill_territory_info_msg(legion_proto_city_war_territory_info* territory_info_msg, s_city_war_territory_info& terr_info_ref)
	{
		if (nullptr == territory_info_msg)
		{
			return;
		}
		territory_info_msg->set_territory_id(terr_info_ref.territory_id);
		territory_info_msg->set_is_in_war(terr_info_ref.city_war_map_guid.is_valid());

		for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
		{
			s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
 			legion_proto_city_war_bid_info* bid_info_msg = territory_info_msg->add_bid_infos();
			fill_bid_info_msg(bid_info_msg, *bid_info);
		}
	}

	void legion_ws_city_war::fill_bid_info_msg(legion_proto_city_war_bid_info* bid_info_msg, s_city_war_bid_info& bid_info_ref)
	{
		if (nullptr == bid_info_msg)
		{
			return;
		}
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		bid_info_msg->set_territory_id(bid_info_ref.territory_id);
		bid_info_msg->set_index_in_terr(bid_info_ref.index_in_terr);
		legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_legion(bid_info_ref.legion_guid);
		if (legion_ws_ptr)
		{
			bid_info_msg->set_legion_guid(bid_info_ref.legion_guid.server_64);
			bid_info_msg->set_legion_name(legion_ws_ptr->get_legion_name());
			s_legion_member_info* chief_member_info = legion_ws_ptr->get_member(legion_ws_ptr->get_chief_guid());
			if (chief_member_info)
			{
				bid_info_msg->set_legion_chief_name(chief_member_info->role_name);
				bid_info_msg->set_legion_chief_template_id(chief_member_info->data_ary[e_legion_member_info_player_template_id]);
			}
		}
		bid_info_msg->set_cur_bid_price(bid_info_ref.cur_bid_price);
		bid_info_msg->set_is_prior_maintain(bid_info_ref.is_prior_maintain);
		bid_info_msg->set_occupy_time_stamp(bid_info_ref.occupy_time_stamp);
	}

	void legion_ws_city_war::fill_territory_info_msg_cross(legion_proto_city_war_territory_info* territory_info_msg, s_city_war_territory_info& terr_info_ref)
	{
		if (nullptr == territory_info_msg)
		{
			return;
		}
		territory_info_msg->set_territory_id(terr_info_ref.territory_id);
		territory_info_msg->set_is_in_war(terr_info_ref.city_war_map_guid.is_valid());

		for (int32 idx = 0; idx < max_apply_city_war_legion_num; idx++)
		{
			s_city_war_bid_info* bid_info = terr_info_ref.get_bid_info(idx);
			legion_proto_city_war_bid_info* bid_info_msg = territory_info_msg->add_bid_infos();
			fill_bid_info_msg_cross(bid_info_msg, *bid_info);
		}
	}
	void legion_ws_city_war::fill_bid_info_msg_cross(legion_proto_city_war_bid_info* bid_info_msg, s_city_war_bid_info& bid_info_ref)
	{
		if (nullptr == bid_info_msg)
		{
			return;
		}
		bid_info_msg->set_territory_id(bid_info_ref.territory_id);
		bid_info_msg->set_index_in_terr(bid_info_ref.index_in_terr);
		bid_info_msg->set_legion_guid(bid_info_ref.legion_guid.server_64);
		bid_info_msg->set_legion_name(bid_info_ref.legion_name);
		bid_info_msg->set_cur_bid_price(bid_info_ref.cur_bid_price);
		bid_info_msg->set_is_prior_maintain(bid_info_ref.is_prior_maintain);
		bid_info_msg->set_occupy_time_stamp(bid_info_ref.occupy_time_stamp);
	}

	void legion_ws_city_war::send_territory_info_one(client_session* session, int32 terr_id)
	{
		if (!world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			return;
		}
		if (nullptr == session)
		{
			return;
		}
		legion_proto_req_territory_info_end req_territory_info_end_msg;
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return;
		}
		legion_proto_city_war_territory_info* territory_info_msg = req_territory_info_end_msg.mutable_terr_info();
		fill_territory_info_msg(territory_info_msg, *terr_info_ptr);
		session->send_to_client(&req_territory_info_end_msg, e_msgindex_s2c_get_city_war_territory_info_end);
	}
	
	void legion_ws_city_war::fill_rank_info_msg(legion_proto_req_city_war_rank_info_end& req_rank_infos_msg)
	{
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		const city_war_rank_type& rank_infos_ref = city_war_territory_mgr::get_instance().get_legion_city_war_rank();

		for (city_war_rank_type::const_iterator iter = rank_infos_ref.begin(); iter != rank_infos_ref.end(); ++iter)
		{
			const s_city_war_legion_rank_info& rank_info = *iter;
			legion_proto_city_war_rank_info* rank_info_msg = req_rank_infos_msg.add_rank_info();
			legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_legion(rank_info.legion_guid);
			if (nullptr == legion_ws_ptr || nullptr == rank_info_msg)
			{
				continue;
			}
			rank_info_msg->set_legion_guid(rank_info.legion_guid.server_64);
			rank_info_msg->set_legion_name(legion_ws_ptr->get_legion_name());
			rank_info_msg->set_glory_glow(legion_ws_ptr->get_legion_info(ELegionInfo_glory_glow));
			rank_info_msg->set_occupy_terr_num(rank_info.occupy_terr_num);
			rank_info_msg->set_occupy_terr_score(rank_info.occupy_terr_score);
			rank_info_msg->set_occupy_terr_highest_score(rank_info.occupy_terr_highest_score);
		}
	}

	void legion_ws_city_war::send_city_war_info_all_msg(client_session* session)
	{
		if (!world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		if (nullptr == session)
		{
			return;
		}
		int32 terr_2c_offset = max_territory_num / 2;
		legion_proto_get_city_war_info_end get_city_war_info_end_msg;
		if (fill_get_city_war_info_msg(get_city_war_info_end_msg, 1, terr_2c_offset))
		{
			session->send_to_client(&get_city_war_info_end_msg, e_msgindex_s2c_get_city_war_info_end);
		}

		get_city_war_info_end_msg.Clear();
		if (fill_get_city_war_info_msg(get_city_war_info_end_msg, terr_2c_offset + 1, max_territory_num))
		{
			session->send_to_client(&get_city_war_info_end_msg, e_msgindex_s2c_get_city_war_info_end);
		}
		legion_proto_get_abstention_city_war_end get_abstention_msg;
		if (fill_get_abstention_city_war_legion(get_abstention_msg))
		{
			session->send_to_client(&get_abstention_msg, e_msgindex_s2c_abstention_city_msg);
		}
	}

	bool legion_ws_city_war::fill_get_abstention_city_war_legion(legion_proto_get_abstention_city_war_end& get_abstention_msg)
	{
		std::unordered_map<int32, xstring>& abstention_msg = city_war_territory_mgr::get_instance().get_absabstention_city_war_legion();
		if (abstention_msg.size() <= 0)
		{
			return false;
		}
		std::unordered_map<int32, xstring>::iterator ite;
		for (ite = abstention_msg.begin(); ite != abstention_msg.end(); ++ite)
		{
			get_abstention_msg.add_territory_id(ite->first);
			get_abstention_msg.add_legion_name(ite->second);
		}
		return true;
	}

	void legion_ws_city_war::fill_get_banquet_info_msg(legion_proto_get_banquet_info_end& get_banquet_info_msg, guid_64 role_guid)
	{
		get_banquet_info_msg.set_occupy_legion_guid(get_overlord_legion().server_64);
		get_banquet_info_msg.set_is_banquet_holding(is_banquet_holding());
		get_banquet_info_msg.set_request_hold_banquet(m_request_hold_banquet);
		get_banquet_info_msg.set_banquet_level(get_banquet_level());
		get_banquet_info_msg.set_participant_num(m_participant_count);
		get_banquet_info_msg.set_participant_max(m_participant_max);

		int32 role_take_part_in_count = 0;
		auto ite = m_participant_record.find(role_guid.server_64);
		if (ite != m_participant_record.end())
		{
			role_take_part_in_count = ite->second;
		}
		get_banquet_info_msg.set_role_take_part_in_num(role_take_part_in_count);
	}

	void legion_ws_city_war::send_overlord_notice(std::string overlord_name, guid_64 overlord_guid, std::string city_name)
	{
		int32 notice_id = legion_send_overload_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(overlord_name);
		vec_notice_str.push_back(city_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, overlord_guid, notice_str);
	}

	void legion_ws_city_war::check_send_city_war_begin_notice(int32 territory_id)
	{
		if (!is_city_war_hold_day(territory_id))
		{
			return;
		}
		
		CityWarTerritoryTemplate* terr_template_ptr = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == terr_template_ptr)
		{
			return;
		}
		struct tm* cur_date = time_helper::get_localtime();
		if (nullptr == cur_date)
		{
			return;
		}
		tm begin_tm, end_tm;
		get_begin_and_end_tm(terr_template_ptr->attribute_id, begin_tm, end_tm);
		if (cur_date->tm_hour == begin_tm.tm_hour && cur_date->tm_min == begin_tm.tm_min)
		{
			if (territory_id == overlord_war_territory_flag)
			{
				send_overlord_war_begin_notice();
			}
			if (territory_id == overlord_war_territory_flag + 1)
			{
				send_common_war_begin_notice();
			}
			if (territory_id == cross_server_territory_war_flag) {
				city_war_territory_mgr::get_instance().send_cross_territory_war_begin_notice_to_other_server(0);
			}
		}
	}

	void legion_ws_city_war::send_overlord_war_begin_notice()
	{
		int32 notice_id = legion_world_mistress_open_notice_id;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			notice_id = new_begin_cross_city_war_notice_id;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			std::string notice_str = init_unit::implode(vec_notice_str);
			int32 create_time = time_helper::get_cur_time_new().second;
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
			return;
		}
		s_city_war_territory_info* info = city_war_territory_mgr::get_instance().get_territory_info(overlord_war_territory_flag);
		if (nullptr == info)
		{
			return;
		}
		std::string overlord_war_legion_name = "";
		s_city_war_bid_info* bid_info = info->bid_legions;
		for (int i = 0; bid_info[i].is_valid() && i < max_apply_city_war_legion_num; i++)
		{
			legion_ws* legion_unit = legion_ws_mgr::get_instance().get_legion(bid_info[i].legion_guid);
			if (nullptr == legion_unit)
			{
				continue;
			}
			overlord_war_legion_name = overlord_war_legion_name + legion_unit->get_legion_name() + " ";
		}
		if (overlord_war_legion_name.empty())
		{
			return;
		}
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(overlord_war_legion_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}

	void legion_ws_city_war::send_common_war_begin_notice()
	{
		int32 notice_id = legion_city_war_open_notice_id;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}
	void legion_ws_city_war::send_cross_server_war_begin_notice()
	{
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		int32 notice_id = legion_cross_server_city_war_open_notice_id;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}
	void legion_ws_city_war::tick_notice(const uint64& tick_time) 
	{
		check_send_city_war_begin_notice(overlord_war_territory_flag);

		if (!world_server::getInstance().is_sky_island_server() && !world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			check_send_city_war_begin_notice(overlord_war_territory_flag + 1);
		}

		if (world_server::getInstance().is_sky_island_server() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war)) 
		{
			check_send_city_war_begin_notice(cross_server_territory_war_flag);
		}
	}

	void legion_ws_city_war::send_city_war_info_to_all_attended()
	{
		unit_guid_map legion_guids_map;
		city_war_territory_mgr::get_instance().get_all_attended_legions(legion_guids_map);
		if (legion_guids_map.size() <= 0)
		{
			return;
		}
		
		//legion_proto_get_city_war_info_end get_city_war_info_end_msg;
		//if (fill_get_city_war_info_msg(get_city_war_info_end_msg) == false)
		//{
		//	return;
		//}
		
		client_session* all_session_map = client_session_mgr::getInstance().get_all_session();
		for (int32 i = 0; i <= client_session_mgr::getInstance().get_session_array_used(); ++i)
		{
			client_session& client_session_ref = all_session_map[i];
			if (client_session_ref.is_vaild() == false)
			{
				continue;
			}
			
			unit_guid_map_it it_find = legion_guids_map.find(client_session_ref.get_legion_guid().server_64);
			if (it_find == legion_guids_map.end())
			{
				continue;
			}
			
			//client_session_ref.send_to_client(&get_city_war_info_end_msg, e_msgindex_s2c_get_city_war_info_end);
			send_city_war_info_all_msg(&client_session_ref);
		}
	}
}