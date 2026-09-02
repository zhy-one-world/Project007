#include "legion_ws.h"
#include "legion_ws_mgr.h"
#include "utility/guid_gen.h"
#include "template/template_manager.h"
#include "utility/globle_data.h"
#include "ws_client.hpp"
#include <legion_msg.hpp>
#include "utility/cs_date.hpp"
#include "Logic/time_def.hpp"
#include "Logic/legion_def.hpp"
#include "Logic/activity_def.hpp"
#include "city_war_territory_mgr.h"
#include "ws/world_server.hpp"
#include "time.hpp"
#include "../mail/event_ws_mgr.h"
#include "utility/init_unit.h"
#include "invalid_word/invalid_ansi_word.h"
#include "../auction/auction_mgr_ws.h"
#include "server_log.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "../ranking/ranking_mgr_ws.h"
#include "../world_boss/world_boss_ws_mgr.h"
#include "../activity/cross_server_city_war_ws_mgr.h"

#include "Logic/attack_city_def.hpp"
#include "base/ecs_world.h"
#include "system/scene/cs_map_system.h"
#include "system/scene/cs_map_mgr_system.h"
#include "server/mail/mail_event_ws.h"
#include "chat.pb.h"
#include "game.pb.h"
#include "net.pb.h"
#include <rlog.hpp>

namespace faith
{
	legion_ws::legion_ws(guid_64 legion_guid)
	{
		m_creat_legion_boss_stamp = 0;
		m_selling_num = 0;
		m_legion_info.legion_guid = legion_guid;
		init();
	}

	legion_ws::legion_ws()
	{
		m_creat_legion_boss_stamp = 0;
		m_selling_num = 0;
		m_legion_info.legion_guid = guid_gen::make_guid();
		init();
	}

	legion_ws::legion_ws(const s_legion_info& legion_info)
	{
		m_creat_legion_boss_stamp = 0;
		m_selling_num = 0;
		m_legion_info = legion_info;
		init();
	}

	void legion_ws::init()
	{
		m_chief_guid.clear_data();
		m_boss_record_set.init(get_legion_guid());
		m_last_ordered_rank_stamp = 0;
		m_legion_rank = 1;
		m_bonfire_add_fuel_times = 0;
		m_last_recruit_in_world_stamp = 0;
		m_recalcu_fighting_power_stamp = 0;
		m_auto_appoint_chief_stamp = 0;
		m_auto_save_bound_to_db_stamp = 0;
		m_city_war_territories.clear();
		m_event_logger.init(m_legion_info.legion_guid);
		m_bonus_info.init(m_legion_info.legion_guid);
		m_is_auction_selling = true;
		m_next_auction_check_time = 0;
		m_last_stimulate_bonus_stamp.clear();
		m_member_sessions.clear();
		m_tick_one_sec_stamp = 0;

		for (int32 i = 0; i < e_legion_act_type_max; i++)
		{
			m_attend_act_mems_map[i].clear();
		}
		memset(m_last_attend_act_stamp, 0, sizeof(m_last_attend_act_stamp));
		m_is_legion_have_change = false;
		m_cross_city_territories_common.clear();
		m_cross_city_territories_prior.clear();
		m_last_caclu_rank_gs_value = 0;
		m_legion_answer_info.clear_data();
		m_legion_station_map_guid.clear_data();
		for (int32 i = 0; i < max_legion_member_num; i++)
		{
			m_legion_boss_damage_player_arr[i].clear_data();
		}
		m_legion_boss_damage_player_num = 0;
		m_is_load_legion_member_end = false;
		m_next_bid_time = 0;
		memset(m_strategy_world_boss, 0, sizeof(m_strategy_world_boss));
	}

	void legion_ws::tick(const uint64& tick_time)
	{
		if (false == get_legion_guid().is_valid() || false == m_chief_guid.is_valid() || m_member_list.size() <= 0)
		{
			return;
		}

		if (tick_time - m_recalcu_fighting_power_stamp >= recalcu_legion_fighting_power_interval)
		{
			m_recalcu_fighting_power_stamp = tick_time;
			recalcu_legion_fighting_power();
		}

		if (tick_time - m_auto_appoint_chief_stamp >= half_hour_tick_time)
		{
			auto_appoint_chief();
			m_auto_appoint_chief_stamp = tick_time;
		}

		if (tick_time - m_auto_save_bound_to_db_stamp >= second_tick_time * 10)
		{
			get_bonus_info_ins().save_legion_bonus_info_into_db();
			m_auto_save_bound_to_db_stamp = tick_time;
		}

		if (tick_time - m_tick_one_sec_stamp >= second_tick_time)
		{
			m_tick_one_sec_stamp = tick_time;

		}

		if (m_is_auction_selling
			&& tick_time > m_next_auction_check_time)
		{
			m_next_auction_check_time = init_unit::get_end_time(auction_legion_sell_check_interval);

			server2dp_proto_ws2dp_auction_check_legion_selling auction_selling_check_msg;
			auction_selling_check_msg.set_legion_guid(get_legion_guid().server_64);
			auction_selling_check_msg.set_cur_time(utility::get_tick_count());
			ws_client::getInstance().send_to_dp_new(&auction_selling_check_msg, e_msg_ws2dp_auction_check_legion_selling);

			//ws2dp_auction_check_legion_selling auction_selling_check_msg;
			//auction_selling_check_msg.legion_guid = get_legion_guid();
			//auction_selling_check_msg.cur_time = utility::get_tick_count();
			//ws_client::getInstance().send_to_dp(&auction_selling_check_msg, sizeof(ws2dp_auction_check_legion_selling));
		}

		m_applicant_list.tick(tick_time);

		refresh_weekly();
	}

	void legion_ws::tick_1_min(const uint64& tick_time)
	{
		check_chief_valid_and_change();
		if (m_is_legion_have_change)
		{
			m_is_legion_have_change = false;
			save_legion_info_to_db();
		}
	}

	void legion_ws::tick_5_min(const uint64& tick_time)
	{
		if (!m_is_legion_have_change)
		{
			save_legion_info_to_db();
		}
	}

	void legion_ws::tick_1_day(const uint64& tick_time)
	{

	}

	void legion_ws::refresh_week_for_zero()
	{

	}

	void legion_ws::refresh_week_for_twenty_three()
	{
		for (s_legion_member_info& legion_member_data : m_member_list)
		{
			update_member_info_one(legion_member_data.role_guid, e_legion_member_info_week_contribution, 0, true);
		}

	}

	void legion_ws::refresh_day_for_zero()
	{
		legion_ws_mgr::get_instance().get_boss_award_get_log().clear_role_legion_boss_award_get_log();
		send_legion_boss_award_get_log_to_all_member();
		//maintain_territories();
		set_legion_info(ELegionInfo_act_score_raid, 0);
		set_legion_info(ELegionInfo_act_score_bonfire, 0);

		set_log_var(log_head);
		set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count());
		server_log::serverLegionLog(log_head, get_legion_name(), get_legion_level(), get_legion_gs_value(), get_member_count());

	}

	void legion_ws::refresh_day_for_five()
	{
		m_bonus_info.clear_legion_bonus_info_map();
		legion_proto_get_legion_bonus_info_end get_legion_bonus_info_end_msg;
		m_bonus_info.fill_legion_bonus_info_list_all(get_legion_bonus_info_end_msg);
		send_message_to_all_member(&get_legion_bonus_info_end_msg, e_msgindex_s2c_legion_bonus_info);

		if (m_is_load_legion_member_end)
		{
			bool is_all_logout = is_all_member_logout_long_time();
			if (is_all_logout)
			{
				legion_ws_mgr::get_instance().del_legion(get_legion_guid());
			}
		}

	}

	static int32 get_fresh_time()
	{
		tm* now_time = time_helper::get_localtime();
		if (nullptr == now_time)
		{
			return 0;
		}

		int32 refresh_time = now_time->tm_year * 1000 + now_time->tm_yday;
		if (1 == now_time->tm_wday)
		{
			return refresh_time;
		}
		else
		{
			return 0;
		}
	}

	void legion_ws::refresh_weekly()
	{
		int32 refresh_time = get_fresh_time();
		if (refresh_time <= 0)
		{
			return;
		}
		int32 last_refresh_time = get_legion_info(ELegionInfo_last_refresh_time);
		if (refresh_time == last_refresh_time)
		{
			return;
		}

		if (false == cost_maintain_money())
		{
			return;
		}

		//if (last_refresh_time > 0)
		//m_boss_record_set.refresh_data();
		//send_boss_record_list_to_all_member();
		//legion_ws_mgr::get_instance().get_boss_award_get_log().clear_role_legion_boss_award_get_log();
		//send_legion_boss_award_get_log_to_all_member();

		set_legion_info(ELegionInfo_last_refresh_time, refresh_time);
		send_legion_attr_one(ELegionInfo_last_refresh_time);
	}

	bool legion_ws::cost_maintain_money()
	{
		const LegionConstructionsTemplate* legion_tmpl_ptr = get_legion_cfg();
		if (nullptr == legion_tmpl_ptr)
		{
			return false;
		}

		int32 maintain_cost_monget = legion_tmpl_ptr->LegionMaintainMoney;
		if (maintain_cost_monget <= 0)
		{
			return true;
		}

		int32 cur_legion_asset_money = get_legion_info(ELegionInfo_asset_money);
		if (cur_legion_asset_money < maintain_cost_monget)
		{
			legion_ws_mgr::get_instance().del_legion(get_legion_guid());
			return false;
		}

		add_asset_money(-maintain_cost_monget);
		return true;
	}

	void legion_ws::add_asset_money(int32 add_val)
	{
		if (add_val == 0)
		{
			return;
		}
		int64 cur_legion_asset_money = get_legion_info(ELegionInfo_asset_money);
		cur_legion_asset_money += add_val;
		if (cur_legion_asset_money > max_glory_glow)
		{
			cur_legion_asset_money = max_glory_glow;
		}

		set_legion_info(ELegionInfo_asset_money, cur_legion_asset_money);
		send_legion_attr_one(ELegionInfo_asset_money);
	}

	void legion_ws::add_glory_glow(int32 add_val)
	{
		//if (add_val == 0)
		//{
		//	return;
		//}
		//int64 cur_glory_glow = get_legion_info(ELegionInfo_glory_glow);
		//cur_glory_glow += add_val;
		//if (cur_glory_glow > max_glory_glow)
		//{
		//	cur_glory_glow = max_glory_glow;
		//}
		//set_legion_info(ELegionInfo_glory_glow, cur_glory_glow);
		//send_legion_attr_one(ELegionInfo_glory_glow);
	}

	void legion_ws::save_legion_info_to_db()
	{
		if (!m_legion_info.legion_guid.is_valid())
		{
			return;
		}
		server2dp_proto_ws2dp_save_legion_info msg;
		server2dp_proto_s_legion_info* legion_info = msg.mutable_legion_info();
		if (nullptr == legion_info)
		{
			return;
		}
		legion_info->set_legion_guid(m_legion_info.legion_guid.server_64);
		legion_info->set_legion_name(m_legion_info.legion_name);
		legion_info->set_announcement(m_legion_info.announcement);
		legion_info->set_gs_value(m_legion_info.gs_value);
		for (int32 i = 0; i < ELegionInfo_max; i++)
		{
			legion_info->add_data_ary(m_legion_info.data_ary[i]);
		}
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_legion_info);

		//ws2dp_save_legion_info save_legion_info_msg;
		//save_legion_info_msg.legion_info = m_legion_info;
		//ws_client::getInstance().send_to_dp(&save_legion_info_msg, sizeof(save_legion_info_msg));
	}

	void legion_ws::save_legion_member_info_to_db(guid_64 member_guid)
	{
		s_legion_member_info* member_info = get_member(member_guid);
		if (nullptr == member_info)
		{
			return;
		}
		save_legion_member_info_to_db(*member_info);
	}

	void legion_ws::save_legion_member_info_to_db(const s_legion_member_info& member_info)
	{
		//ws2dp_save_legion_member_info save_legion_member_info_msg;
		//save_legion_member_info_msg.legion_guid = get_legion_guid();
		//save_legion_member_info_msg.legion_member_info = member_info;
		//ws_client::getInstance().send_to_dp(&save_legion_member_info_msg, sizeof(save_legion_member_info_msg));

		server2dp_proto_ws2dp_save_legion_member_info msg;
		server2dp_proto_s_legion_member_info* member_info_msg = msg.mutable_legion_member_info();
		if (nullptr == member_info_msg)
		{
			return;
		}
		msg.set_legion_guid(get_legion_guid().server_64);
		member_info_msg->set_role_guid(member_info.role_guid);
		member_info_msg->set_role_name(member_info.role_name);
		member_info_msg->set_gs_value(member_info.gs_value);
		for (int32 i = 0; i < e_legion_member_info_max; i++)
		{
			member_info_msg->add_data_ary(member_info.data_ary[i]);
		}
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_legion_member_info);
	}

	void legion_ws::del_legion_member_info_to_db(guid_64 member_guid)
	{
		//s_legion_member_info temp_member_info;
		//temp_member_info.role_guid = member_guid;

		//ws2dp_save_legion_member_info save_legion_member_info_msg;
		//save_legion_member_info_msg.legion_member_info = temp_member_info;
		//ws_client::getInstance().send_to_dp(&save_legion_member_info_msg, sizeof(save_legion_member_info_msg));

		//ws2dp_delete_legion_member_info del_msg;
		//del_msg.role_guid = member_guid;
		//ws_client::getInstance().send_to_dp(&del_msg, sizeof(del_msg));

		script_mgr::get_instance().call_func("ws2dp_send", "send_delete_legion_member_info_lua", 0, false, "%l", member_guid.server_64);
	}
	void legion_ws::set_legion_info(ELegionInfo info_index, int64 info_data, bool sv_to_db)
	{
		m_legion_info.data_ary[info_index] = info_data;
		m_is_legion_have_change = true;
	}

	void legion_ws::set_legion_gs_value(int64 gs_value)
	{
		m_legion_info.gs_value = gs_value;
		m_is_legion_have_change = true;
	}

	s_legion_member_info* legion_ws::get_member(guid_64 player_guid)
	{
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			if (player_guid == legion_member_info.role_guid)
			{
				return &(legion_member_info);
			}
		}

		return nullptr;
	}

	const s_legion_member_info* legion_ws::get_member(guid_64 player_guid) const
	{
		for (const s_legion_member_info& legion_member_info : m_member_list)
		{
			if (player_guid == legion_member_info.role_guid)
			{
				return &(legion_member_info);
			}
		}

		return nullptr;
	}

	//s_legion_member_info* legion_ws::get_member(int32 legion_member_index)
	//{
	//	int32 len = m_member_list.size();
	//	if (legion_member_index < 0 || legion_member_index >= len)
	//	{
	//		return nullptr;
	//	}

	//	return &(m_member_list[legion_member_index]);
	//}

	void legion_ws::refresh_city_war_can_award()
	{
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			update_member_info_one(legion_member_info.role_guid, e_legion_member_info_last_get_city_award, 0);
		}
	}

	void legion_ws::refresh_master_city_war_can_award()
	{
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			update_member_info_one(legion_member_info.role_guid, e_legion_member_info_get_master_city_award, 1);
		}
	}

	bool legion_ws::add_member(s_legion_member_info& member_info, bool is_from_db, bool is_need_save_db, const login_fixed_data& third_info, const int32& login_type)
	{
		const guid_64& member_guid = member_info.role_guid;
		if (member_guid.is_valid() == false)
		{
			return false;
		}

		if (is_legion_full())
		{
			return false;
		}

		if (get_member(member_guid) != nullptr)
		{
			return false;
		}

		m_member_list.push_back(member_info);
		legion_ws_mgr::get_instance().set_unit_legion(member_guid, m_legion_info.legion_guid);

		bool is_chief = member_info.data_ary[e_legion_member_info_job_title] == e_legion_job_title_chief;
		if (is_chief)
		{
			m_chief_guid = member_info.role_guid;
		}

		if (false == is_from_db)
		{
			on_member_add(member_info, is_need_save_db);
		}

		return true;
	}

	void legion_ws::clear_all_city_war_member()
	{
		clear_all_city_war_member_by_type(e_legion_use_contribution_type_city_war);
		send_city_war_member_info_msg();
	}

	void legion_ws::clear_all_city_war_member_by_type(int32 use_type)
	{
		std::list<s_legion_city_war_member_info>::iterator ite;
		for (ite = m_city_war_member_list.begin(); ite != m_city_war_member_list.end();)
		{
			if (ite->use_type == use_type)
			{
				ite = m_city_war_member_list.erase(ite);
			}
			else
			{
				++ite;
			}
		}
	}

	void legion_ws::save_city_war_member_to_db(s_legion_city_war_member_info& member_info)
	{
		ws2dp_save_legion_city_war_member_info c_member_info_msg;
		//c_member_info_msg.legion_guid = get_legion_guid();
		//c_member_info_msg.legion_member_info = member_info;
		//ws_client::getInstance().send_to_dp(&c_member_info_msg, sizeof(c_member_info_msg));

		server2dp_proto_ws2dp_save_legion_city_war_member_info msg;
		server2dp_proto_s_legion_city_war_member_info* member_info_ptr = msg.mutable_legion_member_info();
		if (nullptr == member_info_ptr)
		{
			return;
		}
		msg.set_legion_guid(get_legion_guid().server_64);
		member_info_ptr->set_role_guid(member_info.role_guid.server_64);
		member_info_ptr->set_use_type(member_info.use_type);
		member_info_ptr->set_role_name(member_info.role_name);
		member_info_ptr->set_player_temp_id(member_info.player_temp_id);
		member_info_ptr->set_week_contribution(member_info.week_contribution);
		member_info_ptr->set_job_title(member_info.job_title);
		ws_client::getInstance().send_to_dp_new(&msg, e_msgindex_ws2dp_save_legion_city_war_member_info);
	}

	void legion_ws::add_city_war_member_from_db(s_legion_city_war_member_info& c_member_info)
	{
		if (!c_member_info.is_valid())
		{
			return;
		}
		m_city_war_member_list.push_back(c_member_info);
	}

	void legion_ws::send_city_war_reward_with_terr_id(int32 terr_id)
	{
		CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, terr_id);
		if (nullptr == territory_cfg)
		{
			return;
		}
		const LegionConstructionsTemplate* legion_con_temp_ptr = get_legion_cfg(ELegionInfo_construction_level_main);
		if (nullptr == legion_con_temp_ptr)
		{
			return;
		}
		greater <s_legion_member_info> gt;
		m_member_list.sort(gt);
		int32 rule_max_num = legion_con_temp_ptr->CityWarRewardNum;
		int32 cur_num = 0;
		std::vector<int32> reward_arr;
		reward_arr.clear();
		reward_arr = territory_cfg->DailyRewards;
		if (reward_arr.size() <= 0 || reward_arr.size() % 2 != 0)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();
		for (int32 i = 0; i < reward_arr.size() / 2; i++)
		{
			drop_item_list.push_back({ reward_arr[i * 2] ,reward_arr[i * 2 + 1] ,1});
		}
		const std::string& terr_name = template_manager::get_instance().get_str_by_string_template_id(territory_cfg->TerrName);
		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_city_war_win_reward_title));
		title_params.push_back(terr_name);
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_city_war_win_reward_content));

		std::string content_text = init_unit::implode(content_params);
		std::list<s_legion_member_info>::iterator ite;
		for (ite = m_member_list.begin(); ite != m_member_list.end(); ++ite)
		{
			if (cur_num >= rule_max_num)
			{
				break;
			}
			mail_ws_mgr::get_instance().send_mail_system(ite->role_guid, 0, drop_item_list, title_text, content_text);
			add_city_war_member(ite->role_guid, e_legion_use_contribution_type_city_war);
			cur_num++;
		}
		send_city_war_member_info_msg();
	}

	void legion_ws::add_city_war_member(guid_64 role_guid, int32 use_type)
	{
		s_legion_city_war_member_info c_member_info;

		s_legion_member_info* member_info = get_member(role_guid);
		if (nullptr == member_info)
		{
			return;
		}
		c_member_info.role_guid = role_guid;
		c_member_info.use_type = use_type;
		c_member_info.set_role_name(member_info->role_name);
		c_member_info.week_contribution = member_info->data_ary[e_legion_member_info_week_contribution];
		c_member_info.job_title = member_info->data_ary[e_legion_member_info_job_title];
		c_member_info.player_temp_id = member_info->data_ary[e_legion_member_info_player_template_id];
		m_city_war_member_list.push_back(c_member_info);

		save_city_war_member_to_db(c_member_info);
	}

	void legion_ws::on_member_add(const s_legion_member_info& member_info, bool is_need_save_db)
	{
		const guid_64& member_guid = member_info.role_guid;
		legion_ws_mgr::get_instance().on_player_join(get_legion_guid(), member_info.role_guid, get_legion_name());

		get_event_logger().add_member_join_event(member_info.role_name);

		int64 cur_time = time_helper::get_cur_time_new().second;
		if (member_info.data_ary[e_legion_member_info_last_logout_stamp] <= 0 || cur_time < member_info.data_ary[e_legion_member_info_last_logout_stamp] + 7 * day_time_second)
		{
			int64 new_power = get_legion_gs_value() + member_info.gs_value;
			set_legion_gs_value(new_power);
		}

		client_session* member_session = client_session_mgr::getInstance().get_session(member_guid);
		if (member_session)
		{
			member_session->set_legion_guid(get_legion_guid());
			member_session->set_legion_name(get_legion_name());
			send_aoi_legion_info(member_session, true);
			//send_player_bonfire_info(member_session);
			send_legion_city_war_info(member_session);

			legion_proto_auction_is_selling msg;
			msg.set_is_selling(m_selling_num > 0);
			member_session->send_to_client(&msg, e_msgindex_s2c_update_legion_auction_selling);
		}

		send_add_member_message(member_info);
		if (member_session)
		{
			m_member_sessions[member_guid.server_64] = member_session;
		}
		save_legion_member_info_to_db(member_info);
	}

	void legion_ws::del_member(guid_64 player_guid, const login_fixed_data& third_info, const int32& login_type)
	{
		bool is_refresh = false;
		std::list<s_legion_member_info>::iterator ite;
		for (ite = m_member_list.begin(); ite != m_member_list.end();)
		{
			if (ite->role_guid == player_guid)
			{
				on_member_delete(*ite);
				ite = m_member_list.erase(ite);
				legion_ws_mgr::get_instance().clear_unit_legion(player_guid);
				is_refresh = true;
				break;
			}
			else
			{
				++ite;
			}
		}
		if (is_refresh)
		{
			recalcu_legion_fighting_power();
		}
	}

	void legion_ws::on_member_delete(const s_legion_member_info& member_info)
	{
		const guid_64& member_guid = member_info.role_guid;
		legion_ws_mgr::get_instance().on_player_leave(get_legion_guid(), member_info.role_guid, get_legion_name());

		get_event_logger().add_member_leave_event(member_info.role_name);

		client_session* member_session = client_session_mgr::getInstance().get_session(member_guid);
		if (member_session)
		{
			member_session->clear_legion_info();

			legion_proto_auction_is_selling msg;
			msg.set_is_selling(false);
			member_session->send_to_client(&msg, e_msgindex_s2c_update_legion_auction_selling);
		}

		m_member_sessions.erase(member_info.role_guid.server_64);
		send_del_member_message(member_info);
		del_legion_member_info_to_db(member_info.role_guid);
	}

	void legion_ws::clear_member()
	{
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			on_member_delete(legion_member_info);
			legion_ws_mgr::get_instance().clear_unit_legion(legion_member_info.role_guid);
		}

		m_member_list.clear();
	}

	void legion_ws::send_mail_to_all_member_with_master_city_war(bool is_winner)
	{
		int32 wintext_id = 0;
		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();

		ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (nullptr == activity_template_ptr)
		{
			return;
		}

		if (true == is_winner)
		{
			if (activity_template_ptr->ParamIntArr1.size() % 2 != 0 && activity_template_ptr->ParamIntArr1.size() <= 0)
			{
				return;
			}
			wintext_id = 90200673;
			int32 item_type_num = activity_template_ptr->ParamIntArr1.size() / 2;

			for (int32 i = 0; i < item_type_num; i++)
			{
				drop_item_list.push_back({ activity_template_ptr->ParamIntArr1[i * 2] , activity_template_ptr->ParamIntArr1[i * 2 + 1] });
			}
		}
		else
		{
			if (activity_template_ptr->ParamIntArr2.size() % 2 != 0 && activity_template_ptr->ParamIntArr2.size() <= 0)
			{
				return;
			}
			wintext_id = 90200672;
			int32 item_type_num = activity_template_ptr->ParamIntArr2.size() / 2;

			for (int32 i = 0; i < item_type_num; i++)
			{
				drop_item_list.push_back({ activity_template_ptr->ParamIntArr2[i * 2] ,activity_template_ptr->ParamIntArr2[i * 2 + 1] });
			}
		}

		const std::string& wintext = template_manager::get_instance().get_str_by_string_template_id(wintext_id);
		//const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_master_city_war_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_master_city_war_context));
		content_params.push_back(wintext);
		std::string contenttext = init_unit::implode(content_params);

		for (s_legion_member_info& member_info : m_member_list)
		{
			if (member_info.role_guid.is_valid() == false)
			{
				continue;
			}
			mail_ws_mgr::get_instance().send_mail_system(member_info.role_guid, 0, drop_item_list, title, contenttext);
		}
	}

	void legion_ws::send_mail_to_officer_with_city_bid(int32 ter_id)
	{
		CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, ter_id);
		if (nullptr == territory_cfg)
		{
			return;
		}
		const std::string& city_name = template_manager::get_instance().get_str_by_string_template_id(territory_cfg->TerrName);
		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		std::vector<s_item_template_info> drop_item_list;
		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_city_bid_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_city_bid_context));
		content_params.push_back(city_name);
		std::string contenttext = init_unit::implode(content_params);

		s_mail_info temp_mail_info;

		std::vector<guid_64> temp_item_guid_array;
		init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);

		temp_mail_info.set_sender_name(sender_name);
		temp_mail_info.set_mail_title(title);
		temp_mail_info.set_mail_content_text(contenttext);

		for (s_legion_member_info& member_info : m_member_list)
		{
			if (member_info.role_guid.is_valid() == false)
			{
				continue;
			}
			if (false == is_assistant_chief(member_info.role_guid) && false == is_chief(member_info.role_guid))
			{
				continue;
			}
			temp_mail_info.mail_guid = guid_gen::make_guid();
			mail_ws_mgr::get_instance().send_mail(member_info.role_guid, 0, temp_mail_info, nullptr, 0);
		}
	}

	void legion_ws::send_all_legion_boss_notice()
	{
		int32 notice_id = legion_boss_all_legion_notice_id;
		score_indicator* score_list = world_boss_ws_mgr::get_instance().get_legion_score_all();
		if (nullptr == score_list)
		{
			return;
		}
		std::string empty_string = template_manager::get_instance().get_str_by_string_template_id(90200229);
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		for (int32 i = 0; i < 3; i++)
		{
			score_record* cur_score = score_list->get_score_record_at(i);
			if (nullptr == cur_score)
			{
				vec_str_up_item.push_back(empty_string);
			}
			else
			{
				vec_str_up_item.push_back(cur_score->role_name);
			}
		}
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::send_legion_boss_notice()
	{
		int32 notice_id = legion_boss_cur_legion_notice_id;
		int64 damage_value = world_boss_ws_mgr::get_instance().get_legion_damage_with_legion_boss(get_legion_guid());
		if (damage_value <= 0)
		{
			return;
		}
		guid_64 first_player_guid = m_legion_boss_damage_player_arr[0];
		std::string player_name = "";
		if (first_player_guid.is_valid())
		{
			s_legion_member_info* player_info = get_member(first_player_guid);
			if (nullptr != player_info)
			{
				player_name = player_info->role_name;
			}
		}
		xstring gold_num_str = init_unit::change_i64_to_string(damage_value);
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(gold_num_str);
		vec_str_up_item.push_back(player_name);
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::fill_one_auction_item_msg(game_proto_legion_aunction_item_one& one_msg, int32 item_temp_id, int32 item_num, int32 item_lock)
	{
		one_msg.set_item_temp_id(item_temp_id);
		one_msg.set_item_num(item_num);
		one_msg.set_item_lock(item_lock);
	}

	void legion_ws::send_legion_welfare_by_activity_and_rank(int32 activity_type, int32 rank_index)
	{
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (nullptr == config_ptr)
		{
			return;
		}
		RegionTemplate* region_ptr = globle_data::get_instance().get_region_template_ptr();
		if (nullptr == region_ptr)
		{
			return;
		}
		const LegionConstructionsTemplate* legion_con_temp_ptr = get_legion_cfg(ELegionInfo_construction_level_main);
		if (nullptr == legion_con_temp_ptr)
		{
			return;
		}
		int32 notice_id = legion_welfare_activity_award_notice_id;

		std::vector<int32> rank_reward_list;
		switch (activity_type)
		{
		case e_activity_type_legion_boss:
		{
			int32 notice_id = legion_welfare_kill_boss_notice_id;
			rank_reward_list = legion_con_temp_ptr->LegionBossRankWelfareNum;
		}
		break;
		case e_activity_type_world_boss:
		{
			int32 notice_id = legion_welfare_kill_boss_notice_id;
			rank_reward_list = legion_con_temp_ptr->WorldBossRankWelfareNum;
		}
		break;
		case e_activity_type_cross_server_world_boss:
		{
			int32 notice_id = legion_welfare_kill_boss_notice_id;
			rank_reward_list = legion_con_temp_ptr->CrossBossRankWelfareNum;
		}
		break;
		case e_activity_type_overlord_city_war:
		{
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				rank_reward_list = legion_con_temp_ptr->CrossOverlordRankWelfareNum;
			}
			else
			{
				rank_reward_list = legion_con_temp_ptr->OverlordRankWelfareNum;
			}
		}
		break;
		case e_activity_type_attack_city:
		{
			rank_reward_list = legion_con_temp_ptr->AttackCityWelfareNum;
		}
		break;
		}
		int32 reward_num = globle_data::get_instance().get_rwd_num_by_rank_rwd(rank_index, rank_reward_list);
		if (reward_num <= 0)
		{
			return;
		}
		int32 now_num = get_legion_info(ELegionInfo_legion_welfare_num);
		int32 max_num = legion_con_temp_ptr->LegionWelfareMaxNum;
		int32 add_num = 0;
		if (now_num < max_num)
		{
			add_num = now_num + reward_num;
			if (add_num > max_num)
			{
				add_num = max_num;
			}
			set_legion_info(ELegionInfo_legion_welfare_num, add_num, true);
			send_legion_attr_one(ELegionInfo_legion_welfare_num);
		}
		_RLOG_(MINFO, ::faith::log_detail::format_message("LegionWelfareReceiveAwardActivity LegionGuid:{} ActivityType:{} Rank:{} RewardNum:{} NowNum:{} MaxNum:{} AddNum:{}",  get_legion_guid().server_64,  activity_type,  rank_index,  reward_num,  now_num,  max_num,  add_num));

		int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> notice_str_params;
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		if (region_ptr->RegionCode == e_version_region_type_en)
		{
			notice_str_params.push_back(init_unit::change_i32_to_string(rank_index));
			notice_str_params.push_back(template_manager::get_instance().int_to_string(config_ptr->ActivityName));
		}
		else
		{
			notice_str_params.push_back(template_manager::get_instance().int_to_string(config_ptr->ActivityName));
			notice_str_params.push_back(init_unit::change_i32_to_string(rank_index));
		}
		notice_str_params.push_back(init_unit::change_i32_to_string(reward_num));
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, get_legion_guid(), award_notice_str);
	}

	void legion_ws::send_legion_welfare_by_npc(int32 npc_temid, int32 rank_index)
	{
		NpcTemplate* npc_ptr = GET_TEMPLATE(NpcTemplate, npc_temid);
		if (nullptr == npc_ptr)
		{
			return;
		}
		int32 reward_num = globle_data::get_instance().get_rwd_num_by_rank_rwd(rank_index, npc_ptr->LegionWelferaRewards);
		if (reward_num <= 0)
		{
			return;
		}

		const LegionConstructionsTemplate* legion_con_temp_ptr = get_legion_cfg(ELegionInfo_construction_level_main);
		if (nullptr == legion_con_temp_ptr)
		{
			return;
		}
		int32 now_num = get_legion_info(ELegionInfo_legion_welfare_num);
		int32 max_num = legion_con_temp_ptr->LegionWelfareMaxNum;
		int32 add_num = 0;
		if (now_num < max_num)
		{
			add_num = now_num + reward_num;
			if (add_num > max_num)
			{
				add_num = max_num;
			}
			set_legion_info(ELegionInfo_legion_welfare_num, add_num, true);
			send_legion_attr_one(ELegionInfo_legion_welfare_num);
		}
		_RLOG_(MINFO, ::faith::log_detail::format_message("LegionWelfareReceiveAwardNpc LegionGuid:{} NpcId:{} Rank:{} RewardNum:{} NowNum:{} MaxNum:{} AddNum:{}",  get_legion_guid().server_64,  npc_temid,  rank_index,  reward_num,  now_num,  max_num,  add_num));


		int32 notice_id = legion_welfare_kill_boss_notice_id;
		int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> notice_str_params;
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		notice_str_params.push_back(template_manager::get_instance().int_to_string(npc_ptr->NpcName));
		notice_str_params.push_back(init_unit::change_i32_to_string(rank_index));
		notice_str_params.push_back(init_unit::change_i32_to_string(reward_num));
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, get_legion_guid(), award_notice_str);
	}

	void legion_ws::legion_welfare_account()
	{
		const LegionConstructionsTemplate* legion_con_temp_ptr = get_legion_cfg(ELegionInfo_construction_level_main);
		if (nullptr == legion_con_temp_ptr)
		{
			return;
		}
		//std::vector<std::string> content_params;
		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_legion_welfare_reward_title);
		const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_legion_welfare_reward_context);
		if (nullptr == title || nullptr == content)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.push_back({ legion_con_temp_ptr->LegioWelfareItemId ,legion_welfare_grant_num });

		greater <s_legion_member_info> gt;
		std::list<s_legion_member_info> member_list = get_member_list();
		member_list.sort(gt);

		int32 active_num = 0;
		int32 grant_num = 0;
		int32 welfare_num = get_legion_info(ELegionInfo_legion_welfare_num);
		for (auto iter = member_list.begin(); iter != member_list.end(); ++iter)
		{
			if (iter->data_ary[e_legion_member_info_week_contribution] > 0)
			{
				active_num++;
			}
			if (welfare_num <= 0)
			{
				continue;
			}
			if (iter->data_ary[e_legion_member_info_week_contribution] < legion_con_temp_ptr->LegionWelfareLowestGiveNum)
			{
				continue;
			}
			grant_num++;
			add_city_war_member(iter->role_guid, e_legion_use_contribution_type_legion_welfare);
			//content_params.clear();
			//content_params.push_back(std::string(content));
			//content_params.push_back(std::string(role_name));
			//std::string contenttext = init_unit::implode(content_params);
			mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title, content);
		}

		send_legion_welfare_member_info_msg();
		set_legion_info(ELegionInfo_legion_welfare_num, 0);
		send_legion_attr_one(ELegionInfo_legion_welfare_num);

		set_log_var(log_head);
		set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count());
		set_log_common_head_part4(log_head, get_legion_guid(), get_legion_name(), get_legion_info(ELegionInfo_construction_level_main), 0);

		server_log::serverLegionWelfareGrantNumLog(log_head, get_legion_guid(), active_num, grant_num);
	}

	bool legion_ws::week_is_have_player_active()
	{
		std::list<s_legion_member_info> member_list = get_member_list();
		for (auto iter = member_list.begin(); iter != member_list.end(); ++iter)
		{
			if (iter->data_ary[e_legion_member_info_week_contribution] > 0)
			{
				return true;
			}
		}
		return false;
	}

	void legion_ws::send_legion_welfare_member_info_msg()
	{
		legion_proto_legion_welfare_members_info_list c_members_info_list_msg;
		c_members_info_list_msg.set_is_first(true);
		for (s_legion_city_war_member_info& c_legion_member_data : m_city_war_member_list)
		{
			if (c_legion_member_data.is_valid() == false || c_legion_member_data.use_type != e_legion_use_contribution_type_legion_welfare)
			{
				continue;
			}

			legion_proto_legion_welfare_member_info* c_legion_member_info_msg = c_members_info_list_msg.add_mems_info_arr();
			if (nullptr == c_legion_member_info_msg)
			{
				continue;
			}

			fill_legion_welfare_member_info_msg(c_legion_member_info_msg, c_legion_member_data);
			if (c_members_info_list_msg.mems_info_arr_size() >= 30)
			{
				send_message_to_all_member(&c_members_info_list_msg, e_msgindex_s2c_legion_welfare_members_info_list_end);
				c_members_info_list_msg.Clear();
				c_members_info_list_msg.set_is_first(false);
			}
		}
		if (c_members_info_list_msg.mems_info_arr_size() > 0)
		{
			send_message_to_all_member(&c_members_info_list_msg, e_msgindex_s2c_legion_welfare_members_info_list_end);
		}
	}

	void legion_ws::send_legion_boss_auction_item(std::vector<s_item_template_info>& _item_list, int32 act_type)
	{
		if (_item_list.size() <= 0)
		{
			return;
		}
		game_proto_legion_auction_item_show msg;

		for (auto& item_ite : _item_list)
		{
			game_proto_legion_aunction_item_one* one_msg = msg.add_item_all();
			fill_one_auction_item_msg(*one_msg, item_ite.m_item_id, item_ite.m_item_num, item_ite.m_lock);
		}
		if (act_type == e_activity_type_legion_boss)
		{
			send_message_to_all_member_with_map_type(&msg, e_msgindex_s2c_legion_auction_item_tip, e_map_type_legion_station);
		}
	}

	void legion_ws::send_legion_reward_with_legion_boss_rank(int32 rank)
	{
		if (rank <= 0 || m_legion_boss_damage_player_num <= 0)
		{
			return;
		}
		ActivityCommonConfigTemplate* cross_server_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_boss);
		if (nullptr == cross_server_boss_config_ptr)
		{
			return;
		}
		int32 cur_avg_level = ranking_mgr_ws::get_world_level_last();

		std::vector<s_item_template_info> _item_list;
		std::vector<int32> rwd_data;
		rwd_data.clear();

		rwd_data = cross_server_boss_config_ptr->RankRewards;

		globle_data::get_instance().get_item_list_by_rank_rwd(rank, rwd_data, _item_list, cur_avg_level);
		send_legion_boss_auction_item(_item_list, (int32)e_activity_type_legion_boss);
		for (auto item_ite : _item_list)
		{
			give_activity_reward(cross_server_boss_config_ptr, m_legion_boss_damage_player_arr, m_legion_boss_damage_player_num, item_ite.m_item_id, item_ite.m_item_num);
		}
	}

	void legion_ws::set_legion_boss_damage_player(const guid_64* player_guid_arr, int32 player_num)
	{
		for (int32 i = 0; i < max_legion_member_num; i++)
		{
			m_legion_boss_damage_player_arr[i].clear_data();
		}
		m_legion_boss_damage_player_num = 0;
		if (nullptr == player_guid_arr || player_num == 0)
		{
			return;
		}
		int32  real_num = 0;
		for (int32 i = 0; i < player_num; i++)
		{
			if (get_member(player_guid_arr[i]) == nullptr)
			{
				continue;
			}
			m_legion_boss_damage_player_arr[real_num] = player_guid_arr[i];
			real_num++;
		}
		m_legion_boss_damage_player_num = real_num;
	}

	bool legion_ws::is_all_member_logout_long_time()
	{
		bool all_logout = true;
		int64 cur_time = time_helper::get_cur_time_new().second;
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			if (legion_member_info.data_ary[e_legion_member_info_is_online] > 0)
			{
				all_logout = false;
				break;
			}
			if (legion_member_info.data_ary[e_legion_member_info_last_logout_stamp] > 0 && cur_time < legion_member_info.data_ary[e_legion_member_info_last_logout_stamp] + 15 * day_time_second)
			{
				all_logout = false;
				break;
			}
		}
		return all_logout;
	}

	void legion_ws::recalcu_legion_fighting_power()
	{
		int64 fighting_power = 0;
		int64 cur_time = time_helper::get_cur_time_new().second;
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			if (legion_member_info.data_ary[e_legion_member_info_last_logout_stamp] > 0 && cur_time >= legion_member_info.data_ary[e_legion_member_info_last_logout_stamp] + 7 * day_time_second)
			{
				continue;
			}
			fighting_power += legion_member_info.gs_value;
		}

		int64 old_fighting_power = m_last_caclu_rank_gs_value;
		if (fighting_power == old_fighting_power)
		{
			return;
		}
		m_last_caclu_rank_gs_value = fighting_power;
		set_legion_gs_value(fighting_power);
		send_legion_gs_value();

		legion_rank_list* _rank_list = legion_ws_mgr::get_instance().get_legion_rank(e_legion_rank_type_fighting_power);
		if (_rank_list)
		{
			legion_ws_mgr::get_instance().update_legion_rank_one(*_rank_list, this, fighting_power, e_legion_rank_type_fighting_power);
		}
	}

	const LegionConstructionsTemplate* legion_ws::get_legion_cfg(faith::ELegionInfo construction_index) const
	{
		int32 cur_legion_level = get_legion_info(construction_index);
		int32 legion_template_id = first_legion_template_id + cur_legion_level - 1;
		return GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
	}

	int32 legion_ws::appoint(guid_64 player_guid, e_legion_job_title job_title, const login_fixed_data& third_info, const int32& login_type)
	{
		if (job_title >= e_legion_job_title_max)
		{
			return -1;
		}

		s_legion_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return -2;
		}

		if (e_legion_job_title_chief == job_title)
		{
			//if (legion_ws_mgr::get_instance().get_legion_city_war().get_overlord_legion() == get_legion_guid())
			//{
			//	legion_ws_mgr::get_instance().get_legion_city_war().send_del_city_master_stuff(m_chief_guid);
			//	legion_ws_mgr::get_instance().get_legion_city_war().ws2dp_del_overlord_legion_stuff_proc(e_del_overlord_stuff_event_only_master, m_chief_guid);
			//	guid_64 new_city_master_guid = player_guid;
			//	client_session* new_city_master_session = client_session_mgr::getInstance().get_session(new_city_master_guid);
			//	if (new_city_master_session)
			//	{
			//		big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_lord_of_city, new_city_master_guid);
			//		legion_ws_mgr::get_instance().get_legion_city_war().send_get_city_master_stuff(new_city_master_session);
			//	}
			//}
		}

		const LegionConstructionsTemplate* legion_con_temp_ptr = get_legion_cfg(ELegionInfo_construction_level_main);
		if (legion_con_temp_ptr == nullptr)
		{
			return -1;
		}
		if (legion_con_temp_ptr->MaxJobNum.size() < 2)
		{
			return -1;
		}
		if (job_title > e_legion_job_title_none && job_title < e_legion_job_title_chief)
		{
			if (legion_con_temp_ptr->MaxJobNum[job_title - 1] <= get_job_title_num(job_title))
			{
				return -3;
			}

		}

		FString old_job_title = get_job_title_string(player_guid);
		update_member_info_one(player_guid, e_legion_member_info_job_title, (int32)job_title);
		FString new_job_title = get_job_title_string(player_guid);
		notice_appoint(FString(member_info->role_name), old_job_title, new_job_title);
		m_event_logger.add_member_post_change_event(member_info->role_name, old_job_title, new_job_title);
		if (e_legion_job_title_chief == job_title && player_guid != m_chief_guid)
		{
			appoint(m_chief_guid, e_legion_job_title_none);
			m_chief_guid = player_guid;
			cross_server_city_war_ws_mgr::get_instance().send_to_gate_change_legion_msg(get_legion_guid(), e_change_cross_city_type_change_chief, member_info->role_name, player_guid);
		}

		client_session* player_session = client_session_mgr::getInstance().get_session(player_guid);
		if (player_session)
		{
			send_aoi_legion_info(player_session);
		}


		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		std::vector<s_item_template_info> drop_item_list;
		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_set_legion_job_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_set_legion_job_context));
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(legion_job_name[job_title]);
		content_params.push_back(temp_string);
		std::string contenttext = init_unit::implode(content_params);

		s_mail_info temp_mail_info;

		std::vector<guid_64> temp_item_guid_array;
		init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);

		temp_mail_info.set_sender_name(sender_name);
		temp_mail_info.set_mail_title(title);
		temp_mail_info.set_mail_content_text(contenttext);

		mail_ws_mgr::get_instance().send_mail(player_guid, 0, temp_mail_info, nullptr, 0);

		xchar title_id[8];
		memset(title_id, 0, 8);
		printf(title_id, "%d", job_title);
		return 1;
	}

	e_legion_job_title legion_ws::get_job_title(guid_64 player_guid) const
	{
		const s_legion_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return e_legion_job_title_max;
		}
		return (e_legion_job_title)(member_info->data_ary[e_legion_member_info_job_title]);
	}

	xstring legion_ws::get_job_title_string(guid_64 player_guid)
	{
		int32 legion_job_title_index = get_job_title(player_guid);
		const std::vector<std::string>& temp_string_array_list = template_manager::get_instance().get_string_list(90090431);
		if (temp_string_array_list.empty())
		{
			return "";
		}
		if (e_legion_job_title_max > temp_string_array_list.size() || legion_job_title_index >= e_legion_job_title_max)
		{
			return "";
		}
		return temp_string_array_list[legion_job_title_index];
	}

	bool legion_ws::is_chief(guid_64 player_guid)
	{
		return get_job_title(player_guid) == e_legion_job_title_chief;
	}

	bool legion_ws::is_assistant_chief(guid_64 player_guid)
	{
		return get_job_title(player_guid) == e_legion_job_title_assistant_chief;
	}

	void legion_ws::set_member_online(client_session* legion_member_session, bool online)
	{
		if (nullptr == legion_member_session)
		{
			return;
		}
		guid_64 player_guid = legion_member_session->get_role_guid();
		s_legion_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return;
		}
		int32 new_data_val = (online ? 1 : 0);
		update_member_info_one(player_guid, e_legion_member_info_is_online, new_data_val);
	}

	void legion_ws::on_legion_member_reconnect(client_session* mem_session)
	{
		send_legion_all_info(mem_session);
		send_legion_city_war_info(mem_session);
		send_aoi_legion_info(mem_session);
	}

	void legion_ws::update_member_info_login(client_session* mem_session)
	{
		if (nullptr == mem_session)
		{
			return;
		}
		guid_64 role_guid = mem_session->get_role_guid();
		update_member_gs_value(role_guid, mem_session->get_role_gs_value());
		update_member_info_one(role_guid, e_legion_member_info_vip_level, mem_session->get_role_info_data(e_role_info_vip_level));
		update_member_info_one(role_guid, e_legion_member_info_level, mem_session->get_role_info_data(e_role_info_exp_level));
		update_member_info_one(role_guid, e_legion_member_info_is_show_vip, mem_session->get_role_info_data(e_role_info_is_show_vip));

	}

	void legion_ws::update_member_gs_value(guid_64 player_guid, int64 gs_value)
	{
		s_legion_member_info* member_info_ptr = get_member(player_guid);
		if (nullptr == member_info_ptr)
		{
			return;
		}

		if (member_info_ptr->gs_value == gs_value)
		{
			return;
		}
		member_info_ptr->gs_value = gs_value;

		send_legion_member_gs_value(player_guid);
	}

	void legion_ws::update_member_contribution(guid_64 player_guid, int64 contribution_value)
	{
		s_legion_member_info* member_info_ptr = get_member(player_guid);
		if (nullptr == member_info_ptr)
		{
			return;
		}

		if (member_info_ptr->data_ary[e_legion_member_info_donate_contribution] == contribution_value)
		{
			return;
		}
		member_info_ptr->data_ary[e_legion_member_info_donate_contribution] = contribution_value;
		save_legion_member_info_to_db(*member_info_ptr);
		send_legion_member_contribution_value(player_guid);
	}

	void legion_ws::on_legion_member_login(client_session* mem_session)
	{
		guid_64 role_guid = mem_session->get_role_guid();

		m_member_sessions[role_guid.server_64] = mem_session;
		mem_session->set_legion_guid(get_legion_guid());
		mem_session->set_legion_name(get_legion_name());
		update_member_info_one(role_guid, e_legion_member_info_last_logout_stamp, 0);
		update_member_info_login(mem_session);
		set_member_online(mem_session, true);
		auto_appoint_chief();

		send_legion_all_info(mem_session);
		send_aoi_legion_info(mem_session);

		legion_proto_kill_legion_boss kill_legion_boss_to_client_msg;
		kill_legion_boss_to_client_msg.set_boss_map_template_id(globle_data::get_instance().get_legion_boss_temp_id());
		kill_legion_boss_to_client_msg.set_killer_name(get_kill_legion_boss_name());
		send_message_to_all_member(&kill_legion_boss_to_client_msg, e_msgindex_s2c_kill_legion_boss);

		legion_proto_get_legion_bonus_info_end get_legion_bonus_info_end_msg;
		get_bonus_info_ins().fill_legion_bonus_info_list_all(get_legion_bonus_info_end_msg);
		send_message_to_all_member(&get_legion_bonus_info_end_msg, e_msgindex_s2c_legion_bonus_info);

		send_legion_city_war_info(mem_session);
		//legion_ws_mgr::get_instance().get_legion_city_war().check_and_send_city_master_online_acconcement(role_guid);
		legion_ws_mgr::get_instance().get_boss_award_get_log().send_role_legion_boss_award_get_log(mem_session);

		legion_proto_auction_is_selling msg;
		msg.set_is_selling(m_selling_num > 0);
		mem_session->send_to_client(&msg, e_msgindex_s2c_update_legion_auction_selling);

		send_applicant_info_list_to_have_job_member();
	}

	void legion_ws::on_legion_member_logout(client_session* mem_session)
	{
		guid_64 role_guid = mem_session->get_role_guid();

		m_member_sessions.erase(role_guid.server_64);
		mem_session->clear_legion_info();

		set_member_online(mem_session, false);
		update_member_info_one(role_guid, e_legion_member_info_last_logout_stamp, time_helper::get_cur_time_new().second);
	}

	void legion_ws::add_member_contribution(guid_64 player_guid, int32 add_contri_val)
	{
		s_legion_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return;
		}
		int64 new_data_val = member_info->data_ary[e_legion_member_info_donate_contribution] + add_contri_val;
		int64 new_week_val = member_info->data_ary[e_legion_member_info_week_contribution] + add_contri_val;
		update_member_contribution(player_guid, new_data_val);
		update_member_info_one(player_guid, e_legion_member_info_week_contribution, new_week_val);
	}

	void legion_ws::update_member_info_one(guid_64 player_guid, int32 info_index, int64 info_value, bool need_sync2c)
	{
		if (info_index < 0 || info_index >= e_legion_member_info_max)
		{
			return;
		}

		s_legion_member_info* member_info_ptr = get_member(player_guid);
		if (nullptr == member_info_ptr)
		{
			return;
		}

		if (member_info_ptr->data_ary[info_index] == info_value)
		{
			return;
		}
		member_info_ptr->data_ary[info_index] = info_value;
		save_legion_member_info_to_db(*member_info_ptr);
		send_legion_member_info_one(player_guid, (e_legion_member_info)info_index);
	}

	void legion_ws::set_legion_name(const xchar* legion_name, int32 legion_name_len)
	{
		memcpy(m_legion_info.legion_name, legion_name, legion_name_len >= max_name_size ? max_name_size : legion_name_len);
	}

	xchar* legion_ws::get_legion_name()
	{
		return m_legion_info.legion_name;
	}

	xchar* legion_ws::get_chief_name()
	{
		if (m_chief_guid.is_valid() == false)
		{
			return empty_char_array;
		}
		s_legion_member_info* member_info = get_member(m_chief_guid);
		if (nullptr == member_info)
		{
			return empty_char_array;
		}
		return member_info->role_name;
	}

	void legion_ws::set_announcement(const xchar* content, int32 content_len, bool need_send_mail, bool need_save_db, guid_64 do_change_role_guid, const login_fixed_data& third_info, const int32& login_type)
	{
		memset(m_legion_info.announcement, 0, max_legion_announcement_size);
		memcpy(m_legion_info.announcement, content, content_len >= max_legion_announcement_size ? max_legion_announcement_size : content_len);
		//if (true == need_save_db)
		//{
		//	save_legion_info_to_db();
		//}

		if (true == need_send_mail)
		{
			if (content_len == 0)
			{
				return;
			}
			const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			std::vector<s_item_template_info> drop_item_list;
			const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_set_legion_announcement_title);
			std::vector<std::string> content_params;
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_set_legion_announcement_context));
			content_params.push_back(m_legion_info.announcement);
			std::string contenttext = init_unit::implode(content_params);

			s_mail_info temp_mail_info;

			std::vector<guid_64> temp_item_guid_array;
			init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);

			temp_mail_info.set_sender_name(sender_name);
			temp_mail_info.set_mail_title(title);
			temp_mail_info.set_mail_content_text(contenttext);

			for (s_legion_member_info& member_info : m_member_list)
			{
				if (member_info.role_guid.is_valid() == false)
				{
					continue;
				}
				temp_mail_info.mail_guid = guid_gen::make_guid(0);
				mail_ws_mgr::get_instance().send_mail(member_info.role_guid, 0, temp_mail_info, nullptr, 0);
			}
		}
	}

	int32 legion_ws::get_legion_rank()
	{
		uint64 cur_ordered_stamp = legion_ws_mgr::get_instance().get_order_legion_list_stamp();
		if (cur_ordered_stamp <= m_last_ordered_rank_stamp)
		{
			return m_legion_rank;
		}

		int32 cur_legion_rank = legion_ws_mgr::get_instance().get_legion_rank_in_list(get_legion_guid());
		set_legion_rank(cur_legion_rank);
		m_last_ordered_rank_stamp = cur_ordered_stamp;

		return cur_legion_rank;
	}

	int32 legion_ws::get_max_member_count() const
	{
		const LegionConstructionsTemplate* legion_tmpl_ptr = get_legion_cfg();
		if (nullptr == legion_tmpl_ptr)
		{
			return -1;
		}

		return legion_tmpl_ptr->MaxMemNum;
	}

	bool legion_ws::is_legion_full() const
	{
		return get_member_count() >= get_max_member_count();
	}

	bool legion_ws::is_auto_accept_new_member()
	{
		return get_legion_info(ELegionInfo_auto_accept_new_member) > 0 ? true : false;
	}

	static int32 get_construction_level_up_need_money(const LegionConstructionsTemplate* legion_template_ptr, int32 construction_index)
	{
		if (nullptr == legion_template_ptr)
		{
			return -1;
		}

		switch ((ELegionInfo)construction_index)
		{
		case ELegionInfo_construction_level_main:
			return legion_template_ptr->MainConstructionLevelUpCostMoney;

		case ELegionInfo_construction_level_1:
			return legion_template_ptr->Construction_1_LevelUpCostMoney;

		case ELegionInfo_construction_level_2:
			return legion_template_ptr->Construction_2_LevelUpCostMoney;

		case ELegionInfo_construction_level_3:
			return legion_template_ptr->Construction_3_LevelUpCostMoney;
		}

		return -1;
	}

	static std::vector<int32> get_construction_level_up_need_items(const LegionConstructionsTemplate* legion_template_ptr, int32 construction_index)
	{
		if (nullptr == legion_template_ptr)
		{
			std::vector<int32> empty_arr;
			return empty_arr;
		}

		switch ((ELegionInfo)construction_index)
		{
		case ELegionInfo_construction_level_main:
			return legion_template_ptr->MainConstructionLevelUpCostItems;

		case ELegionInfo_construction_level_1:
			return legion_template_ptr->Construction_1_LevelUpCostItems;

		case ELegionInfo_construction_level_2:
			return legion_template_ptr->Construction_2_LevelUpCostItems;

		case ELegionInfo_construction_level_3:
			return legion_template_ptr->Construction_3_LevelUpCostItems;
		}

		std::vector<int32> empty_arr;
		return empty_arr;
	}

	int32 legion_ws::construction_level_up(int32 construction_index)
	{
		if (construction_index < ELegionInfo_construction_level_main || construction_index > ELegionInfo_construction_level_3)
		{
			return e_legion_construction_level_up_error_invalid_index;
		}

		int32 cur_construction_level = get_legion_info((ELegionInfo)construction_index);
		if ((int32)ELegionInfo_construction_level_main == construction_index)
		{
			//if (get_legion_info(ELegionInfo_construction_level_1) != cur_construction_level ||
			//	get_legion_info(ELegionInfo_construction_level_2) != cur_construction_level ||
			//	get_legion_info(ELegionInfo_construction_level_3) != cur_construction_level)
			//{
			//	return e_legion_construction_level_up_error_sub_constr_level_not_enough;
			//}
		}
		else
		{
			if (cur_construction_level >= get_legion_info(ELegionInfo_construction_level_main))
			{
				return e_legion_construction_level_up_error_sub_constr_level_too_high;
			}
		}

		const LegionConstructionsTemplate* legion_template_ptr = get_legion_cfg((faith::ELegionInfo)construction_index);
		if (nullptr == legion_template_ptr)
		{
			return e_legion_construction_level_up_error_table_data_error;
		}

		int32 next_legion_template_id = cur_construction_level + first_legion_template_id;
		LegionConstructionsTemplate* next_level_legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, next_legion_template_id);
		if (nullptr == next_level_legion_template_ptr)
		{
			return e_legion_construction_level_up_error_already_top_level;
		}

		int32 level_up_need_money = get_construction_level_up_need_money(legion_template_ptr, construction_index);
		if (level_up_need_money < 0)
		{
			return e_legion_construction_level_up_error_table_data_error;
		}

		std::vector<int32> level_up_need_items = get_construction_level_up_need_items(legion_template_ptr, construction_index);
		if (level_up_need_items.size() < 4)
		{
			return e_legion_construction_level_up_error_table_data_error;
		}

		int32 cur_legion_money = get_legion_info(ELegionInfo_asset_money);
		if (cur_legion_money < level_up_need_money)
		{
			return e_legion_construction_level_up_error_money_not_enough;
		}

		int32 cost_items_arr_len = level_up_need_items.size();
		for (int32 i = 0; i < cost_items_arr_len; ++i)
		{
			ELegionInfo info_index = (ELegionInfo)((int32)ELegionInfo_asset_item_1 + i);
			int32 legion_item_asset_num = get_legion_info(info_index);
			if (legion_item_asset_num < level_up_need_items[i])
			{
				return e_legion_construction_level_up_error_item_not_enougn;
			}
		}

		set_legion_info(ELegionInfo_asset_money, cur_legion_money - level_up_need_money);
		for (int32 i = 0; i < cost_items_arr_len; ++i)
		{
			ELegionInfo info_index = (ELegionInfo)((int32)ELegionInfo_asset_item_1 + i);
			int32 cur_legion_item_asset_num = get_legion_info(info_index);
			set_legion_info(info_index, cur_legion_item_asset_num - level_up_need_items[i]);
		}

		set_legion_info((ELegionInfo)construction_index, cur_construction_level + 1);
		send_aoi_legion_info_to_all_member();

		send_legion_attr_all();
		send_construction_level_up_message(construction_index);

		m_event_logger.add_legion_construction_level_up_event((ELegionInfo)construction_index, cur_construction_level + 1);

		return e_legion_construction_level_up_error_none;
	}

	void legion_ws::fill_city_war_member_info_msg(legion_proto_city_war_member_info* member_info_msg, const s_legion_city_war_member_info& member_info_ref)
	{
		member_info_msg->set_job_title(member_info_ref.job_title);
		member_info_msg->set_role_guid(member_info_ref.role_guid.server_64);
		member_info_msg->set_role_name(member_info_ref.role_name);
		member_info_msg->set_week_contribution(init_unit::change_i64_to_string(member_info_ref.week_contribution));
		member_info_msg->set_player_temp_id(member_info_ref.player_temp_id);
	}

	void legion_ws::fill_legion_welfare_member_info_msg(legion_proto_legion_welfare_member_info* member_info_msg, const s_legion_city_war_member_info& member_info_ref)
	{
		if (nullptr == member_info_msg)
		{
			return;
		}
		member_info_msg->set_role_guid_a(member_info_ref.role_guid.A);
		member_info_msg->set_role_guid_b(member_info_ref.role_guid.B);
		member_info_msg->set_job_title(member_info_ref.job_title);
		member_info_msg->set_role_name(member_info_ref.role_name);
		member_info_msg->set_week_contribution(init_unit::change_i64_to_string(member_info_ref.week_contribution));
		member_info_msg->set_player_temp_id(member_info_ref.player_temp_id);
	}

	void legion_ws::fill_member_info_msg(legion_proto_member_info* member_info_msg, const s_legion_member_info& member_info_ref)
	{
		member_info_msg->set_role_guid(member_info_ref.role_guid.server_64);
		member_info_msg->set_role_name(member_info_ref.role_name);
		member_info_msg->set_gs_value(init_unit::change_i64_to_string(member_info_ref.gs_value));
		member_info_msg->set_contribution_value(init_unit::change_i64_to_string(member_info_ref.data_ary[e_legion_member_info_donate_contribution]));
		member_info_msg->set_week_contribution_value(init_unit::change_i64_to_string(member_info_ref.data_ary[e_legion_member_info_week_contribution]));
		for (int32 i = 0; i < e_legion_member_info_max; ++i)
		{
			member_info_msg->add_member_data(member_info_ref.data_ary[i]);
		}


		if (m_bonfire_add_fuel_times > 0)
		{
			unit_guid_map_it it = m_member_add_fuel_times.find(member_info_ref.role_guid.server_64);
			if (it != m_member_add_fuel_times.end())
			{
				member_info_msg->set_add_fuel_times(it->second);
			}
		}

	}

	bool legion_ws::is_in_this_legion(guid_64 target_guid)
	{
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			if (legion_member_info.role_guid == target_guid)
			{
				return true;
			}
		}
		return false;
	}

	bool legion_ws::fill_legion_info_msg(legion_proto_legion_info& legion_info_msg, client_session* client_session_ptr)
	{
		guid_64 legion_guid = get_legion_guid();
		legion_info_msg.set_legion_guid(legion_guid.server_64);
		legion_info_msg.set_legion_name(m_legion_info.legion_name);
		legion_info_msg.set_announcement(m_legion_info.announcement);
		legion_info_msg.set_legion_gs_value(init_unit::change_i64_to_string(m_legion_info.gs_value));
		for (int32 i = 0; i < ELegionInfo_max; ++i)
		{
			legion_info_msg.add_legion_data(m_legion_info.data_ary[i]);
		}

		int32 member_num = 0;
		for (s_legion_member_info& member_info : m_member_list)
		{
			if (member_info.role_guid.is_valid() == false)
			{
				continue;
			}

			if (nullptr != client_session_ptr)
			{
				if (member_num >= 1)
				{
					break;
				}
				if (member_info.role_guid != client_session_ptr->get_role_guid()) //&& member_info.role_guid != m_chief_guid
				{
					continue;
				}
			}

			legion_proto_member_info* member_info_msg = legion_info_msg.add_member_list();
			if (nullptr == member_info_msg)
			{
				return false;
			}
			fill_member_info_msg(member_info_msg, member_info);
			++member_num;
		}
		legion_info_msg.set_member_num(member_num);

		const LegionConstructionsTemplate* legion_tmpl_ptr = get_legion_cfg();
		if (legion_tmpl_ptr)
		{
			int32 next_recruit_stamp_2c = m_last_recruit_in_world_stamp / second_tick_time + legion_tmpl_ptr->RecruitInWorldCD;
			legion_info_msg.set_next_recruit_stamp(next_recruit_stamp_2c);
		}
		return true;
	}

	void legion_ws::fill_legion_brief_info_msg(legion_proto_legion_brief_info& legion_brief_info_msg)
	{
		const guid_64& legion_guid = get_legion_guid();
		if (legion_guid.is_valid() == false)
		{
			return;
		}

		legion_brief_info_msg.set_legion_guid(legion_guid.server_64);

		legion_brief_info_msg.set_legion_name(get_legion_name());

		guid_64 chief_guid = get_chief_guid();
		legion_brief_info_msg.set_chief_guid(chief_guid.server_64);

		s_legion_member_info* chief_info = get_member(chief_guid);
		if (nullptr == chief_info)
		{
			legion_brief_info_msg.set_chief_name("");
		}
		else
		{
			legion_brief_info_msg.set_chief_name(chief_info->role_name);
		}

		legion_brief_info_msg.set_cur_member_num(get_member_count());
		legion_brief_info_msg.set_max_member_num(get_max_member_count());
		legion_brief_info_msg.set_legion_fighting_power(init_unit::change_i64_to_string(get_legion_gs_value()));
		legion_brief_info_msg.set_legion_level(get_legion_level());
	}

	void legion_ws::send_cs_msg_to_all_member(const void* data_ptr, size_t data_len)
	{
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* legion_member_session = ite->second;
			if (nullptr == legion_member_session || legion_member_session->get_legion_guid() != get_legion_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			legion_member_session->send_to_cs(data_ptr, data_len);
			++ite;
		}
	}

	void legion_ws::send_message_to_all_member_data(const void* data_ptr, int32 data_len, uint32 header)
	{
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* legion_member_session = ite->second;
			if (nullptr == legion_member_session || legion_member_session->get_legion_guid() != get_legion_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			legion_member_session->send_to_client(data_ptr, data_len, header);
			++ite;
		}
	}

	void legion_ws::send_message_to_all_member_data_lua(const char* msg, int32 msg_len, uint32 header)
	{
		send_message_to_all_member_data(msg, msg_len, header);
	}

	void legion_ws::send_message_to_all_member_with_map_type(google::protobuf::Message* net_pro, uint32 header, int32 map_type)
	{
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* legion_member_session = ite->second;
			if (nullptr == legion_member_session || legion_member_session->get_legion_guid() != get_legion_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			MapTemplate* map_temp_ptr = legion_member_session->get_map_temp_ptr();
			if (map_temp_ptr != nullptr && map_temp_ptr->Type == map_type)
			{
				legion_member_session->send_to_client(net_pro, header);
			}
			++ite;
		}
	}

	void legion_ws::send_message_to_all_member(google::protobuf::Message* net_pro, uint32 header)
	{
		//for (s_legion_member_info& member_info : m_member_list)
		//{
		//	if (false == member_info.role_guid.is_valid() || member_info.data_ary[e_legion_member_info_is_online] <= 0)
		//	{
		//		continue;
		//	}

		//	client_session* legion_member_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
		//	if (nullptr == legion_member_session)
		//	{
		//		continue;
		//	}
		//	legion_member_session->send_to_client(net_pro, header);
		//}
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* legion_member_session = ite->second;
			if (nullptr == legion_member_session || legion_member_session->get_legion_guid() != get_legion_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			legion_member_session->send_to_client(net_pro, header);
			++ite;
		}
	}

	void legion_ws::send_message_to_all_member_except(google::protobuf::Message* net_pro, uint32 header, guid_64 except_member)
	{
		//for (s_legion_member_info& member_info : m_member_list)
		//{
		//	if (false == member_info.role_guid.is_valid()
		//		|| member_info.data_ary[e_legion_member_info_is_online] <= 0
		//		|| member_info.role_guid == except_member)
		//	{
		//		continue;
		//	}

		//	client_session* legion_member_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
		//	if (nullptr == legion_member_session)
		//	{
		//		continue;
		//	}
		//	legion_member_session->send_to_client(net_pro, header);
		//}
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			if (ite->first == except_member.server_64)
			{
				++ite;
				continue;
			}
			client_session* legion_member_session = ite->second;
			if (nullptr == legion_member_session || legion_member_session->get_legion_guid() != get_legion_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			legion_member_session->send_to_client(net_pro, header);
			++ite;
		}
	}

	//void legion_ws::send_message_to_all_cs(const void* data_ptr, size_t data_len)
	//{
	//	if (nullptr == data_ptr || data_len == 0)
	//	{
	//		return;
	//	}

	//	cs_uid_cacher.reserve(10);

	//	uint32 cs_uid;
	//	int32 len;
	//	bool already_sended;
	//	for (s_legion_member_info& member_info : m_member_list)
	//	{
	//		if (false == member_info.role_guid.is_valid() || member_info.data_ary[e_legion_member_info_is_online] <= 0)
	//		{
	//			continue;
	//		}

	//		client_session* legion_member_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
	//		if (nullptr == legion_member_session)
	//		{
	//			continue;
	//		}
	//		cs_uid = legion_member_session->m_cs_uid;
	//		len = cs_uid_cacher.size();
	//		already_sended = false;
	//		for (int32 i = 0; i < len; ++i)
	//		{
	//			if (cs_uid_cacher[i] == cs_uid)
	//			{
	//				already_sended = true;
	//				break;
	//			}
	//		}

	//		if (!already_sended)
	//		{
	//			cs_uid_cacher.push_back(cs_uid);
	//			world_server::getInstance().send_by_uid(cs_uid, data_ptr, data_len);
	//		}
	//	}
	//}

	void legion_ws::send_legion_all_info(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		legion_proto_legion_info legion_info_msg;
		if (!fill_legion_info_msg(legion_info_msg, session))
		{
			return;
		}

		session->send_to_client(&legion_info_msg, e_msgindex_s2c_update_legion_info);

		legion_proto_members_info_list members_info_list_msg;

		for (s_legion_member_info& legion_member_data : m_member_list)
		{
			if (legion_member_data.is_valid() == false || legion_member_data.role_guid == session->get_role_guid())
			{
				continue;
			}

			legion_proto_member_info* legion_member_info_msg = members_info_list_msg.add_mems_info_arr();
			if (nullptr == legion_member_info_msg)
			{
				continue;
			}

			fill_member_info_msg(legion_member_info_msg, legion_member_data);
			if (members_info_list_msg.mems_info_arr_size() >= 30)
			{
				session->send_to_client(&members_info_list_msg, e_msgindex_s2c_members_info_list_end);
				members_info_list_msg.Clear();
			}
		}
		if (members_info_list_msg.mems_info_arr_size() > 0)
		{
			session->send_to_client(&members_info_list_msg, e_msgindex_s2c_members_info_list_end);
		}
		legion_proto_city_war_members_info_list c_members_info_list_msg;
		c_members_info_list_msg.set_is_first(true);
		legion_proto_legion_welfare_members_info_list c_legion_welfare_list_msg;
		c_legion_welfare_list_msg.set_is_first(true);
		for (s_legion_city_war_member_info& c_legion_member_data : m_city_war_member_list)
		{
			if (c_legion_member_data.is_valid() == false)
			{
				continue;
			}

			switch (c_legion_member_data.use_type)
			{
			case e_legion_use_contribution_type_city_war:
			{
				legion_proto_city_war_member_info* c_legion_member_info_msg;
				c_legion_member_info_msg = c_members_info_list_msg.add_mems_info_arr();
				if (nullptr == c_legion_member_info_msg)
				{
					continue;
				}
				fill_city_war_member_info_msg(c_legion_member_info_msg, c_legion_member_data);
				if (c_members_info_list_msg.mems_info_arr_size() >= 30)
				{
					session->send_to_client(&c_members_info_list_msg, e_msgindex_s2c_city_war_members_info_list_end);
					c_members_info_list_msg.Clear();
					c_members_info_list_msg.set_is_first(false);
				}
			}
			break;
			case e_legion_use_contribution_type_legion_welfare:
			{
				legion_proto_legion_welfare_member_info* c_legion_member_info_msg;
				c_legion_member_info_msg = c_legion_welfare_list_msg.add_mems_info_arr();
				if (nullptr == c_legion_member_info_msg)
				{
					continue;
				}
				fill_legion_welfare_member_info_msg(c_legion_member_info_msg, c_legion_member_data);
				if (c_legion_welfare_list_msg.mems_info_arr_size() >= 30)
				{
					session->send_to_client(&c_members_info_list_msg, e_msgindex_s2c_legion_welfare_members_info_list_end);
					c_legion_welfare_list_msg.Clear();
					c_legion_welfare_list_msg.set_is_first(false);
				}
			}
			break;
			default:
				break;
			}
		}

		if (c_members_info_list_msg.mems_info_arr_size() > 0)
		{
			session->send_to_client(&c_members_info_list_msg, e_msgindex_s2c_city_war_members_info_list_end);
		}
		if (c_legion_welfare_list_msg.mems_info_arr_size() > 0)
		{
			session->send_to_client(&c_legion_welfare_list_msg, e_msgindex_s2c_legion_welfare_members_info_list_end);
		}
	}

	void legion_ws::send_city_war_member_info_msg()
	{
		legion_proto_city_war_members_info_list c_members_info_list_msg;
		c_members_info_list_msg.set_is_first(true);
		for (s_legion_city_war_member_info& c_legion_member_data : m_city_war_member_list)
		{
			if (c_legion_member_data.is_valid() == false || c_legion_member_data.use_type != e_legion_use_contribution_type_city_war)
			{
				continue;
			}

			legion_proto_city_war_member_info* c_legion_member_info_msg = c_members_info_list_msg.add_mems_info_arr();
			if (nullptr == c_legion_member_info_msg)
			{
				continue;
			}

			fill_city_war_member_info_msg(c_legion_member_info_msg, c_legion_member_data);
			if (c_members_info_list_msg.mems_info_arr_size() >= 30)
			{
				send_message_to_all_member(&c_members_info_list_msg, e_msgindex_s2c_city_war_members_info_list_end);
				c_members_info_list_msg.Clear();
				c_members_info_list_msg.set_is_first(false);
			}
		}
		if (c_members_info_list_msg.mems_info_arr_size() > 0)
		{
			send_message_to_all_member(&c_members_info_list_msg, e_msgindex_s2c_city_war_members_info_list_end);
		}
	}

	//void legion_ws::send_legion_all_info_to_all_member()
	//{
	//	legion_proto_legion_info legion_info_msg;
	//	if (!fill_legion_info_msg(legion_info_msg))
	//	{
	//		return;
	//	}

	//	send_message_to_all_member(&legion_info_msg, e_msgindex_s2c_update_legion_info);
	//}

	void legion_ws::send_legion_attr_all()
	{
		legion_proto_update_legion_info_all update_legion_info_all_to_client_msg;

		int32 legion_info_value = 0;
		for (int32 i = 0; i < ELegionInfo_max; ++i)
		{
			legion_info_value = m_legion_info.data_ary[i];
			update_legion_info_all_to_client_msg.add_legion_info_data_arr(legion_info_value);
		}
		send_legion_gs_value();
		send_message_to_all_member(&update_legion_info_all_to_client_msg, e_msgindex_s2c_update_legion_info_all);
	}

	void legion_ws::send_legion_attr_one(ELegionInfo legion_info_index)
	{
		int32 legion_info_value = get_legion_info(legion_info_index);

		legion_proto_update_legion_info_one update_legion_info_one_to_client_msg;
		update_legion_info_one_to_client_msg.set_legion_info_index(legion_info_index);
		update_legion_info_one_to_client_msg.set_legion_info_value(legion_info_value);
		send_message_to_all_member(&update_legion_info_one_to_client_msg, e_msgindex_s2c_update_legion_info_one);
	}

	void legion_ws::send_legion_gs_value()
	{
		legion_proto_update_legion_gs_value msg;
		msg.set_gs_value(init_unit::change_i64_to_string(get_legion_gs_value()));
		send_message_to_all_member(&msg, e_msgindex_s2c_update_legion_gs_value);
	}

	void legion_ws::send_legion_member_info_all(guid_64 member_guid)
	{
		s_legion_member_info* member_info = get_member(member_guid);
		if (nullptr == member_info)
		{
			return;
		}

		legion_proto_update_legion_member_info_all update_legion_member_info_all_to_client_msg;
		update_legion_member_info_all_to_client_msg.set_member_guid(member_guid.server_64);
		update_legion_member_info_all_to_client_msg.set_gs_value(init_unit::change_i64_to_string(member_info->gs_value));
		update_legion_member_info_all_to_client_msg.set_contribution_value(init_unit::change_i64_to_string(member_info->data_ary[e_legion_member_info_donate_contribution]));
		update_legion_member_info_all_to_client_msg.set_week_contribution_value(init_unit::change_i64_to_string(member_info->data_ary[e_legion_member_info_week_contribution]));
		int64 mid_int = 0;
		for (int32 i = 0; i < e_legion_member_info_max; ++i)
		{
			mid_int = member_info->data_ary[i];
			if (mid_int > INT32_MAX - 1)
			{
				mid_int = INT32_MAX - 1;
			}
			update_legion_member_info_all_to_client_msg.add_data_arr(mid_int);
		}
		send_message_to_all_member(&update_legion_member_info_all_to_client_msg, e_msgindex_s2c_update_legion_member_info_all);
	}

	void legion_ws::send_legion_member_info_one(guid_64 member_guid, e_legion_member_info info_index)
	{
		s_legion_member_info* member_info = get_member(member_guid);
		if (nullptr == member_info)
		{
			return;
		}
		if (info_index >= e_legion_member_info_max)
		{
			return;
		}
		int64 info_value = member_info->data_ary[info_index];
		if (info_value >= INT32_MAX - 1)
		{
			info_value = INT32_MAX - 1;
		}

		legion_proto_update_legion_member_info_one update_legion_member_info_one_to_client_msg;
		update_legion_member_info_one_to_client_msg.set_member_guid(member_guid.server_64);
		update_legion_member_info_one_to_client_msg.set_info_index(info_index);
		update_legion_member_info_one_to_client_msg.set_info_value(info_value);
		send_message_to_all_member(&update_legion_member_info_one_to_client_msg, e_msgindex_s2c_update_legion_member_info_one);
	}

	void legion_ws::send_legion_member_gs_value(guid_64 member_guid)
	{
		s_legion_member_info* member_info = get_member(member_guid);
		if (nullptr == member_info)
		{
			return;
		}
		legion_proto_update_legion_member_gs_value msg;
		msg.set_member_guid(member_guid);
		msg.set_gs_value(init_unit::change_i64_to_string(member_info->gs_value));
		send_message_to_all_member(&msg, e_mgsindex_s2c_legion_pdate_legion_member_gs_value);
	}

	void legion_ws::send_legion_member_contribution_value(guid_64 member_guid)
	{
		s_legion_member_info* member_info = get_member(member_guid);
		if (nullptr == member_info)
		{
			return;
		}
		legion_proto_update_legion_member_contribution_value msg;
		msg.set_member_guid(member_guid);
		msg.set_contribution_value(init_unit::change_i64_to_string(member_info->data_ary[e_legion_member_info_donate_contribution]));
		msg.set_week_contribution_value(init_unit::change_i64_to_string(member_info->data_ary[e_legion_member_info_week_contribution]));
		send_message_to_all_member(&msg, e_mgsindex_s2c_legion_pdate_legion_member_contribution);
	}

	void legion_ws::send_legion_announcement()
	{
		xchar* legion_announcement = get_announcement();
		if (nullptr == legion_announcement)
		{
			return;
		}
		std::string announcement_str = legion_announcement;
		int32 len = announcement_str.size();
		if (len <= 0)
		{
			return;
		}

		legion_proto_update_announcement_end update_announcement_to_client_msg;
		update_announcement_to_client_msg.set_content(legion_announcement);
		send_message_to_all_member(&update_announcement_to_client_msg, e_msgindex_s2c_update_legion_announcement_end);
	}

	void legion_ws::send_add_member_message(const s_legion_member_info& new_member_info)
	{
		guid_64 new_member_guid = new_member_info.role_guid;

		client_session* new_member_session = client_session_mgr::getInstance().get_session(new_member_guid);
		if (nullptr != new_member_session)
		{
			send_legion_all_info(new_member_session);
			legion_proto_kill_legion_boss kill_legion_boss_to_client_msg;
			kill_legion_boss_to_client_msg.set_boss_map_template_id(globle_data::get_instance().get_legion_boss_temp_id());
			kill_legion_boss_to_client_msg.set_killer_name(m_kill_legion_boss_name);
			send_message_to_all_member(&kill_legion_boss_to_client_msg, e_msgindex_s2c_kill_legion_boss);
			legion_proto_get_legion_bonus_info_end get_legion_bonus_info_end_msg;
			m_bonus_info.fill_legion_bonus_info_list_all(get_legion_bonus_info_end_msg);
			send_message_to_all_member(&get_legion_bonus_info_end_msg, e_msgindex_s2c_legion_bonus_info);
		}

		legion_proto_add_member_end add_member_end_to_client_msg;
		legion_proto_member_info* legion_member_info = add_member_end_to_client_msg.mutable_new_member_info();
		if (nullptr != legion_member_info)
		{
			fill_member_info_msg(legion_member_info, new_member_info);
			send_message_to_all_member_except(&add_member_end_to_client_msg, e_msgindex_s2c_add_legion_member_end, new_member_guid);
		}

		notice_add_member(std::string(new_member_info.role_name));
	}

	void legion_ws::send_del_member_message(const s_legion_member_info& del_member_info)
	{
		const guid_64& del_member_guid = del_member_info.role_guid;
		legion_proto_del_member_end del_legion_member_end_to_client_msg;
		del_legion_member_end_to_client_msg.set_del_member_guid(del_member_guid.server_64);
		send_message_to_all_member_except(&del_legion_member_end_to_client_msg, e_msgindex_s2c_del_legion_member_end, del_member_guid);

		notice_del_member(std::string(del_member_info.role_name));
	}

	void legion_ws::send_construction_level_up_message(int32 construction_index)
	{
		legion_proto_construction_level_up_end level_up_end_msg_to_client_msg;
		level_up_end_msg_to_client_msg.set_construction_index(construction_index);
		send_message_to_all_member(&level_up_end_msg_to_client_msg, e_msgindex_s2c_legion_construction_level_up_end);
	}

	void legion_ws::send_boss_record_list_to_all_member()
	{
		legion_proto_kill_legion_boss kill_legion_boss_to_client_msg;
		kill_legion_boss_to_client_msg.set_boss_map_template_id(globle_data::get_instance().get_legion_boss_temp_id());
		kill_legion_boss_to_client_msg.set_killer_name(m_kill_legion_boss_name);
		send_message_to_all_member(&kill_legion_boss_to_client_msg, e_msgindex_s2c_kill_legion_boss);
	}

	void legion_ws::send_legion_boss_award_get_log_to_all_member()
	{
		legion_ws_boss_award_get_log& legion_ws_mgr_ref = legion_ws_mgr::get_instance().get_boss_award_get_log();

		for (s_legion_member_info& member_info : m_member_list)
		{
			if (false == member_info.role_guid.is_valid() || member_info.data_ary[e_legion_member_info_is_online] <= 0)
			{
				continue;
			}

			client_session* legion_member_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
			if (nullptr == legion_member_session)
			{
				continue;
			}

			legion_ws_mgr_ref.send_role_legion_boss_award_get_log(legion_member_session);
		}
	}

	void legion_ws::send_legion_city_war_info(client_session* session)
	{
		if (m_city_war_territories.size() <= 0)
		{
			return;
		}

		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		//legion_proto_get_city_war_info_end get_city_war_info_end_msg;
		//if (city_war_mgr.fill_get_city_war_info_msg(get_city_war_info_end_msg) == false)
		//{
		//	return;
		//}
		//session->send_to_client(&get_city_war_info_end_msg, e_msgindex_s2c_get_city_war_info_end);
		city_war_mgr.send_city_war_info_all_msg(session);
	}

	void legion_ws::get_player_legion_info(s_player_legion_info& legion_info, guid_64& role_guid)
	{
		guid_64 legion_guid = get_legion_guid();
		legion_info.legion_guid = legion_guid;
		legion_info.set_legion_name(get_legion_name());
		legion_info.legion_level = get_legion_info(ELegionInfo_construction_level_main);
		legion_info.job_title = get_job_title(role_guid);
		legion_info.legion_role_num = get_member_list().size();
		legion_info.legion_power = get_legion_gs_value();

		std::vector<int32> occupied_territorys;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			occupied_territorys = get_legion_occupied_territorys_cross_city();
		}
		else
		{
			occupied_territorys = get_legion_occupied_territorys();
		}
		int32 terr_size = occupied_territorys.size();
		for (int32 i = 0; i < terr_size; i++)
		{
			if (i >= max_territory_num)
			{
				break;
			}
			legion_info.occupied_territorys[i] = occupied_territorys[i];
		}
		legion_info.legion_construction_level[ELegionInfo_construction_level_main] = m_legion_info.data_ary[ELegionInfo_construction_level_main];
		legion_info.legion_construction_level[ELegionInfo_construction_level_1] = m_legion_info.data_ary[ELegionInfo_construction_level_1];
		legion_info.legion_construction_level[ELegionInfo_construction_level_2] = m_legion_info.data_ary[ELegionInfo_construction_level_2];
		legion_info.legion_construction_level[ELegionInfo_construction_level_3] = m_legion_info.data_ary[ELegionInfo_construction_level_3];
	}

	void legion_ws::send_aoi_legion_info(client_session* mem_session, bool is_new_join)
	{
		if (nullptr == mem_session)
		{
			return;
		}

		guid_64 role_guid = mem_session->get_role_guid();
		ws2cs_set_player_legion_info set_player_legion_info_msg;
		set_player_legion_info_msg.role_guid = role_guid;
		set_player_legion_info_msg.is_new_join_legion = is_new_join;
		get_player_legion_info(set_player_legion_info_msg.player_legion_info, role_guid);
		bool is_use = proto_by_lua(e_msg_index_ws2cs_set_player_legion_info);
		if (is_use == false)
		{
			mem_session->send_to_cs(&set_player_legion_info_msg, sizeof(set_player_legion_info_msg));
		}
		else
		{
			faith::ws2cs_proto::set_player_legion_info pro_msg;
			set_player_legion_info_msg.to_proto(pro_msg);
			mem_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_set_player_legion_info);

		}
	}

	void legion_ws::send_aoi_legion_info_to_all_member()
	{
		ws2cs_set_player_legion_info set_player_legion_info_msg;
		for (s_legion_member_info& member_info : m_member_list)
		{
			if (member_info.is_valid() == false || member_info.data_ary[e_legion_member_info_is_online] <= 0)
			{
				continue;
			}

			guid_64 member_guid = member_info.role_guid;
			set_player_legion_info_msg.role_guid = member_guid;
			get_player_legion_info(set_player_legion_info_msg.player_legion_info, member_guid);

			client_session* session = client_session_mgr::getInstance().get_session(member_guid);
			if (nullptr == session)
			{
				continue;
			}
			bool is_use = proto_by_lua(e_msg_index_ws2cs_set_player_legion_info);
			if (is_use == false)
			{
				session->send_to_cs(&set_player_legion_info_msg, sizeof(set_player_legion_info_msg));
			}
			else
			{
				faith::ws2cs_proto::set_player_legion_info pro_msg;
				set_player_legion_info_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_set_player_legion_info);
			}
		}
	}

	void legion_ws::gm_kick_out_chief()
	{
		guid_64 old_chief_guid = m_chief_guid;
		s_legion_member_info* chief_info_ptr = get_member(m_chief_guid);
		if (nullptr == chief_info_ptr)
		{
			return;
		}
		guid_64 new_chief = get_next_chief_guid();
		if (new_chief.is_valid() == false)
		{
			legion_ws_mgr::get_instance().del_legion(m_legion_info.legion_guid, true);
			return;
		}
		appoint(new_chief, e_legion_job_title_chief);

		client_session* leaver_session = client_session_mgr::getInstance().get_session(old_chief_guid);
		if (nullptr != leaver_session)
		{
			legion_proto_kickout_legion_member_end kickout_legion_member_end_msg;
			kickout_legion_member_end_msg.set_leaver_guid(old_chief_guid.server_64);
			leaver_session->send_to_client(&kickout_legion_member_end_msg, e_msgindex_s2c_kickout_legion_member_end);
		}
		del_member(old_chief_guid);
	}

	void legion_ws::auto_appoint_chief()
	{
		s_legion_member_info* chief_info_ptr = get_member(m_chief_guid);
		if (nullptr == chief_info_ptr)
		{
			return;
		}
		if (!is_member_offline_too_long_to_Dismiss(*chief_info_ptr))
		{
			return;
		}
		guid_64 new_chief = get_next_chief_guid();
		if (new_chief.is_valid() == false)
		{
			return;
		}
		appoint(new_chief, e_legion_job_title_chief);
	}

	guid_64 legion_ws::get_next_chief_guid()
	{
		guid_64 new_chief(0);
		int32 title_tmp = e_legion_job_title_none;
		int32 contribution_tmp = -1;
		for (s_legion_member_info& member_info : m_member_list)
		{
			if (member_info.is_valid() == false)
			{
				continue;
			}
			guid_64 member_guid = member_info.role_guid;
			if (member_guid == m_chief_guid)
			{
				continue;
			}
			int64 mem_contribution = member_info.data_ary[e_legion_member_info_donate_contribution];
			if (mem_contribution < 0)
			{
				continue;
			}
			if (is_member_offline_too_long_to_Dismiss(member_info))
			{
				continue;
			}
			int32 mem_title = member_info.data_ary[e_legion_member_info_job_title];
			if (mem_title < title_tmp)
			{
				continue;
			}
			if (mem_title == title_tmp && mem_contribution <= contribution_tmp)
			{
				continue;
			}
			title_tmp = mem_title;
			contribution_tmp = mem_contribution;
			new_chief = member_guid;
		}
		return new_chief;
	}

	void legion_ws::check_chief_valid_and_change()
	{
		if (m_chief_guid.is_valid())
		{
			return;
		}
		guid_64 new_chief = get_next_chief_guid();
		if (new_chief.is_valid() == false)
		{
			return;
		}
		appoint(new_chief, e_legion_job_title_chief);
	}

	bool legion_ws::is_member_offline_too_long_to_Dismiss(s_legion_member_info& member_info)
	{
		if (!member_info.is_valid())
		{
			return false;
		}

		int64 now_stamp = time_helper::get_cur_time_new().second;
		int64 logout_stamp = member_info.data_ary[e_legion_member_info_last_logout_stamp];
		if (logout_stamp <= 0)
		{
			return false;
		}

		const LegionConstructionsTemplate* legion_tmpl_ptr = get_legion_cfg();;
		if (nullptr == legion_tmpl_ptr)
		{
			return false;
		}
		int32 need_dis_hour = legion_tmpl_ptr->ChiefDismissedOfflineHour;
		int64 offline_sec = now_stamp - member_info.data_ary[e_legion_member_info_last_logout_stamp];

		int32 server_open_days = globle_data::get_instance().get_server_on_days();
		if (server_open_days <= 7)
		{
			need_dis_hour = 24;
		}

		if (offline_sec < need_dis_hour * 3600)
		{
			return false;
		}

		return true;
	}

	void legion_ws::on_create_bonfire_npc()
	{
		//if (m_bonfire_add_fuel_times <= 1)
		//{
		m_bonfire_add_fuel_times = 1;
		m_member_add_fuel_times.clear();
		m_got_bonfire_reward_members.clear();

		notice_on_create_bonfire();
		m_event_logger.add_on_create_bonfire_event();
		//}
		//else
		//{
		//	auto	map_ws_ptr = get_entity(m_legion_station_map_guid);
		//	if (nullptr == map_ws_ptr || false == map_ws_ptr->is_vaild())
		//	{
		//		return;
		//	}
		//	ws2cs_send_bonfire_level msg;
		//	msg.bonfire_level = (m_bonfire_add_fuel_times - 1);
		//	msg.map_guid = m_legion_station_map_guid;
		//	map_ws_ptr->send_message_to_cs(&msg, sizeof(msg));
		//}
	}

	void legion_ws::send_player_bonfire_info(client_session* p_session)
	{
		//if (!is_bonfire_valid())
		//{
		//	return;
		//}
		//auto map_ptr = get_entity(m_bonfire_map_guid);
		//if (nullptr == map_ptr)
		//{
		//	return;
		//}
		//legion_proto_bonfire_info bonfire_info_msg;
		//bonfire_info_msg.set_map_line_id(map_ptr->get_line_id());
		//bonfire_info_msg.set_create_stamp(m_bonfire_begin_stamp);
		//bonfire_info_msg.set_end_stamp(m_bonfire_end_stamp);
		//bonfire_info_msg.add_postion(m_bonfire_pos.x);
		//bonfire_info_msg.add_postion(m_bonfire_pos.y);
		//bonfire_info_msg.add_postion(m_bonfire_pos.z);
		//bonfire_info_msg.set_bonfire_guid(m_bonfire_guid.server_64);
		//p_session->send_to_client(&bonfire_info_msg, e_msgindex_s2c_bonfire_info);
	}

	void legion_ws::add_fuel_to_bonfire_succeed(guid_64 mem_guid)
	{
		s_legion_member_info* member_info = get_member(mem_guid);
		if (nullptr == member_info)
		{
			return;
		}
		int32 add_fuel_times_all = member_info->data_ary[e_legion_member_info_add_fuel_times] + 1;
		update_member_info_one(mem_guid, e_legion_member_info_add_fuel_times, add_fuel_times_all, false);

		int32 add_times_cur = m_member_add_fuel_times[mem_guid.server_64] + 1;
		m_member_add_fuel_times[mem_guid.server_64] = add_times_cur;
		m_bonfire_add_fuel_times++;

		legion_proto_member_add_fuel_to_fire member_add_fuel_to_fire_msg;
		member_add_fuel_to_fire_msg.set_fire_member_guid(mem_guid.server_64);
		member_add_fuel_to_fire_msg.set_fire_times(add_times_cur);
		member_add_fuel_to_fire_msg.set_fire_times_history(add_fuel_times_all);

		send_message_to_all_member(&member_add_fuel_to_fire_msg, e_msgindex_s2c_member_add_fuel_info);

		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, first_legion_template_id + get_legion_info(ELegionInfo_construction_level_main) - 1);
		if (nullptr == legion_template_ptr || legion_template_ptr->GetContributionScale.size() <= e_legion_get_contribution_type_fuel_bonfire)
		{
			return;
		}
		add_member_contribution(mem_guid, legion_template_ptr->GetContributionScale[e_legion_get_contribution_type_fuel_bonfire]);

		ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_bonfire);
		if (activity_template_ptr == nullptr)
		{
			return;
		}
		notice_add_bonfire_successfull(member_info->role_name, activity_template_ptr->ParamInt3);
	}

	void legion_ws::req_bonfire_rank_reward(guid_64 mem_guid)
	{
		s_legion_member_info* member_info = get_member(mem_guid);
		if (nullptr == member_info)
		{
			return;
		}
		if (m_got_bonfire_reward_members.find(mem_guid) != m_got_bonfire_reward_members.end())
		{
			return;
		}
		unit_guid_map_it it_find = m_member_add_fuel_times.find(mem_guid.server_64);
		if (it_find == m_member_add_fuel_times.end())
		{
			return;
		}
		int32 rank = 0;
		int32 mem_add_times = it_find->second;
		unit_guid_map_it ite;
		for (ite = m_member_add_fuel_times.begin(); ite != m_member_add_fuel_times.end(); ++ite)
		{
			if (ite->second > mem_add_times)
			{
				rank++;
			}
		}
		m_got_bonfire_reward_members.insert(mem_guid);
	}

	void legion_ws::add_city_war_territory(int32 terr_id)
	{
		m_city_war_territories.insert(terr_id);
		if (terr_id == overlord_war_territory_flag)
		{
			send_one_territory_to_all_mem(terr_id);
		}
		else
		{
			if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				send_one_territory_to_all_mem(terr_id);
			}
		}
	}

	void legion_ws::del_city_war_territory(int32 terr_id)
	{
		m_city_war_territories.erase(terr_id);
		if (terr_id == overlord_war_territory_flag)
		{
			send_one_territory_to_all_mem(terr_id);
		}
		else
		{
			if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
			{
				send_one_territory_to_all_mem(terr_id);
			}
		}
	}
	void legion_ws::add_cross_city_territories_prior(int32 terr_id)
	{
		if (m_cross_city_territories_prior.find(terr_id) == m_cross_city_territories_prior.end())
		{
			m_cross_city_territories_prior.insert(terr_id);
		}
	}
	void legion_ws::del_city_war_territory_prior(int32 terr_id)
	{

		if (m_cross_city_territories_prior.find(terr_id) != m_cross_city_territories_prior.end())
		{
			m_cross_city_territories_prior.erase(terr_id);
		}
	}
	void legion_ws::add_cross_city_territories_common(int32 terr_id)
	{
		if (m_cross_city_territories_common.find(terr_id) == m_cross_city_territories_common.end())
		{
			m_cross_city_territories_common.insert(terr_id);
		}
	}
	void legion_ws::del_city_war_territory_common(int32 terr_id)
	{
		if (m_cross_city_territories_common.find(terr_id) != m_cross_city_territories_common.end())
		{
			m_cross_city_territories_common.erase(terr_id);
		}
	}
	void legion_ws::send_one_territory_to_all_mem(int32 terr_id)
	{
		s_city_war_territory_info* terr_info_ptr = city_war_territory_mgr::get_instance().get_territory_info(terr_id);
		if (nullptr == terr_info_ptr)
		{
			return;
		}
		legion_proto_req_territory_info_end req_territory_info_end_msg;
		legion_proto_city_war_territory_info* territory_info_msg = req_territory_info_end_msg.mutable_terr_info();
		legion_ws_mgr::get_instance().get_legion_city_war().fill_territory_info_msg(territory_info_msg, *terr_info_ptr);
		send_message_to_all_member(&req_territory_info_end_msg, e_msgindex_s2c_get_city_war_territory_info_end);
	}

	void legion_ws::clear_legion_city_war()
	{
		guid_64 legion_guid = get_legion_guid();
		for (std::set<int32>::iterator iter = m_city_war_territories.begin(); iter != m_city_war_territories.end(); ++iter)
		{
			city_war_territory_mgr::get_instance().clear_territory_legion_bid_info(*iter, legion_guid);
		}
		m_city_war_territories.clear();
	}
	void legion_ws::maintain_territories_cross_city()
	{
		return;
		city_war_territory_mgr& city_war_terr_mgr_ins = city_war_territory_mgr::get_instance();
		bool is_sync_cs_aoi = false;
		int32 need_cost_asset = 0;
		int32 need_cost_glory_glow = 0;
		int32 cur_legion_asset = get_legion_info(ELegionInfo_asset_money);
		//int32 cur_legion_glory_glow = get_legion_info(ELegionInfo_glory_glow);
		std::set<int32>::iterator prior_iter = m_cross_city_territories_prior.begin();
		while (prior_iter != m_cross_city_territories_prior.end())
		{
			int32 terr_id = *prior_iter;
			++prior_iter;
			CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, terr_id);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			need_cost_asset += territory_cfg->MaintainGold;
			if (cur_legion_asset < need_cost_asset)// || cur_legion_glory_glow < need_cost_glory_glow)
			{
				need_cost_asset -= territory_cfg->MaintainGold;
				//need_cost_glory_glow -= territory_cfg->MaintainGlow;
				send_to_gate_clear_bid_info(terr_id);
				city_war_terr_mgr_ins.add_abstention_city_war_legion(terr_id, get_legion_name());
				//m_city_war_territories.erase(terr_id);
				del_city_war_territory_common(terr_id);
				del_city_war_territory_prior(terr_id);
				m_event_logger.add_lose_city_by_maintain_money_not_enough_event(terr_id);
				is_sync_cs_aoi = true;
			}
		}

		std::set<int32>::iterator common_iter = m_cross_city_territories_common.begin();
		while (common_iter != m_cross_city_territories_common.end())
		{
			int32 terr_id = *common_iter;
			++common_iter;
			CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, terr_id);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			need_cost_asset += territory_cfg->MaintainGold;
			if (cur_legion_asset < need_cost_asset)// || cur_legion_glory_glow < need_cost_glory_glow)
			{
				need_cost_asset -= territory_cfg->MaintainGold;
				//need_cost_glory_glow -= territory_cfg->MaintainGlow;
				send_to_gate_clear_bid_info(terr_id);
				city_war_terr_mgr_ins.add_abstention_city_war_legion(terr_id, get_legion_name());
				//m_city_war_territories.erase(terr_id);
				del_city_war_territory_common(terr_id);
				del_city_war_territory_prior(terr_id);

				m_event_logger.add_lose_city_by_maintain_money_not_enough_event(terr_id);
				is_sync_cs_aoi = true;
			}
		}
		add_asset_money(-need_cost_asset);
		//add_glory_glow(-need_cost_glory_glow);
		if (is_sync_cs_aoi)
		{
			send_aoi_legion_info_to_all_member();
		}
	}
	void legion_ws::send_to_gate_clear_bid_info(int terr_id) {
		ws2ws_clear_bid_info_to_gate msg;
		msg.legion_guid = get_legion_guid();
		msg.territory_id = terr_id;
		int32 server_id = world_server::getInstance().get_cross_id();
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_clear_bid_info_to_gate, &msg, sizeof(msg));
	}
	void legion_ws::maintain_territories()
	{
		return;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
		{
			return;
		}
		city_war_territory_mgr& city_war_terr_mgr_ins = city_war_territory_mgr::get_instance();
		guid_64 legion_guid = get_legion_guid();
		std::set<int32> prior_maintain_terr;
		std::set<int32> common_maintain_terr;
		std::set<int32>::iterator iter = m_city_war_territories.begin();
		int32 now_time_stamp = time_helper::get_cur_time_new().second;
		for (; iter != m_city_war_territories.end(); ++iter)
		{
			int32 terr_id = *iter;
			if (overlord_war_territory_flag == terr_id)
			{
				continue;
			}
			guid_64 occ_leigon = city_war_terr_mgr_ins.get_occupy_legion_guid(terr_id);
			if (occ_leigon != legion_guid)
			{
				continue;
			}
			s_city_war_territory_info* terr_info_ptr = city_war_terr_mgr_ins.get_territory_info(terr_id);
			if (nullptr == terr_info_ptr)
			{
				continue;
			}
			const s_city_war_bid_info* bid_info_ptr = terr_info_ptr->get_bid_info(territory_occupier_bid_idx);
			if (nullptr == bid_info_ptr || bid_info_ptr->legion_guid != legion_guid || bid_info_ptr->cur_bid_price != territory_occupier_price_flag) //|| !time_helper::is_diff_day(now_time_stamp, bid_info_ptr->occupy_time_stamp + sec_day_tick_time)
			{
				continue;
			}
			if (bid_info_ptr->is_prior_maintain)
			{
				prior_maintain_terr.insert(terr_id);
			}
			else
			{
				common_maintain_terr.insert(terr_id);
			}
		}

		bool is_sync_cs_aoi = false;
		int32 need_cost_asset = 0;
		int32 need_cost_glory_glow = 0;
		int32 cur_legion_asset = get_legion_info(ELegionInfo_asset_money);
		//int32 cur_legion_glory_glow = get_legion_info(ELegionInfo_glory_glow);
		std::set<int32>::iterator prior_iter = prior_maintain_terr.begin();
		for (; prior_iter != prior_maintain_terr.end(); ++prior_iter)
		{
			int32 terr_id = *prior_iter;
			CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, terr_id);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			need_cost_asset += territory_cfg->MaintainGold;
			if (cur_legion_asset < need_cost_asset)// || cur_legion_glory_glow < need_cost_glory_glow)
			{
				need_cost_asset -= territory_cfg->MaintainGold;
				//need_cost_glory_glow -= territory_cfg->MaintainGlow;
				city_war_terr_mgr_ins.clear_territory_legion_bid_info(terr_id, legion_guid);
				city_war_terr_mgr_ins.add_abstention_city_war_legion(terr_id, get_legion_name());
				//m_city_war_territories.erase(terr_id);
				del_city_war_territory(terr_id);

				m_event_logger.add_lose_city_by_maintain_money_not_enough_event(terr_id);
				is_sync_cs_aoi = true;
			}
		}

		std::set<int32>::iterator common_iter = common_maintain_terr.begin();
		for (; common_iter != common_maintain_terr.end(); ++common_iter)
		{
			int32 terr_id = *common_iter;
			CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, terr_id);
			if (nullptr == territory_cfg)
			{
				continue;
			}
			need_cost_asset += territory_cfg->MaintainGold;
			if (cur_legion_asset < need_cost_asset)// || cur_legion_glory_glow < need_cost_glory_glow)
			{
				need_cost_asset -= territory_cfg->MaintainGold;
				//need_cost_glory_glow -= territory_cfg->MaintainGlow;
				city_war_terr_mgr_ins.clear_territory_legion_bid_info(terr_id, legion_guid);
				city_war_terr_mgr_ins.add_abstention_city_war_legion(terr_id, get_legion_name());
				//m_city_war_territories.erase(terr_id);
				del_city_war_territory(terr_id);

				m_event_logger.add_lose_city_by_maintain_money_not_enough_event(terr_id);
				is_sync_cs_aoi = true;
			}
		}
		add_asset_money(-need_cost_asset);
		//add_glory_glow(-need_cost_glory_glow);
		if (is_sync_cs_aoi)
		{
			send_aoi_legion_info_to_all_member();
		}
	}
	void legion_ws::notice_get_bonus_reward(guid_64 send_guid, xstring member_name, int32 gold_num)
	{
		int32 notice_id = legion_get_bonus_reward_notice_id;
		xstring gold_num_str = init_unit::change_i32_to_string(gold_num);
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(member_name);
		vec_str_up_item.push_back(gold_num_str);
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, send_guid, notice_str);
	}

	void legion_ws::notice_add_member(FString member_name)
	{
		int32 notice_id = legion_add_member_notice_id;
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(member_name);
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::notice_del_member(FString member_name)
	{
		int32 notice_id = legion_del_member_notice_id;
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(member_name);
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::notice_appoint(FString member_name, FString old_job_title, FString new_job_title)
	{
		int32 notice_id = legion_change_job_title_notice_id;
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(member_name);
		vec_str_up_item.push_back(old_job_title);
		vec_str_up_item.push_back(new_job_title);
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::notice_on_join_overload_city_war()
	{
		int32 notice_id = 93000410;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			notice_id = 93000412;
		}
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::mail_on_attack_city_group_up(int32 rank_level)
	{
		int32 _level = rank_level / e_attack_city_group_level_max;
		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_attack_city_group_up_title));
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_attack_city_group_up_context));
		content_params.push_back(template_manager::get_instance().get_str_by_string_template_id(90306332, _level));
		std::string content_text = init_unit::implode(content_params);

		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();
		auto iter = m_member_list.begin();
		for (; iter != m_member_list.end(); ++iter)
		{
			mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title_text, content_text);
		}
	}

	void legion_ws::mail_on_attack_city_single_reward(int32 rank_level, const guid_64* guid_list, int32 data_num)
	{
		int32 _level = rank_level / e_attack_city_group_level_max;
		int32 _rank = rank_level % attack_city_group_max_num;
		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_attack_city_singl_reward_title));

		send_legion_welfare_by_activity_and_rank(e_activity_type_attack_city, rank_level + 1);
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_time_attack_city_singl_reward_context));
		content_params.push_back(template_manager::get_instance().get_str_by_string_template_id(90306332, _level));
		content_params.push_back(template_manager::get_instance().int_to_string((_rank + 1)));
		std::string content_text = init_unit::implode(content_params);

		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();

		for (int32 i = 0; i < data_num; ++i)
		{
			set_attack_city_single_reward_by_rank(drop_item_list, i + 1, rank_level);
			mail_ws_mgr::get_instance().send_mail_system(guid_list[i], 0, drop_item_list, title_text, content_text);
		}
	}

	void legion_ws::set_attack_city_single_reward_by_rank(std::vector<s_item_template_info>& drop_list, int32 rank, int32 legion_level)
	{
		drop_list.clear();
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_attack_city);
		if (nullptr == act_com_cfg || act_com_cfg->ParamIntArr4.size() / 3 < 0 || act_com_cfg->ParamIntArr4.size() % 3 != 0)
		{
			return;
		}
		int32 reward_id = 0;
		for (int32 i = 0; i < act_com_cfg->ParamIntArr4.size() / 3; ++i)
		{
			if (rank >= act_com_cfg->ParamIntArr4[i * 3] && rank <= act_com_cfg->ParamIntArr4[i * 3 + 1])
			{
				reward_id = act_com_cfg->ParamIntArr4[i * 3 + 2];
				reward_id += legion_level;
				break;
			}
		}
		if (reward_id > 0)
		{
			std::vector<int32> reward_list = init_unit::reward_regional_diff(reward_id);
			for (int32 i = 0; i < reward_list.size() / 2; ++i)
			{
				drop_list.push_back({ reward_list[i * 2] ,reward_list[i * 2 + 1] });
			}
		}
	}

	void legion_ws::notice_on_begin_overload_city_war(int32 rank_idex)
	{
		int32 notice_id = 93000299;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			notice_id = 93000407;
		}
		int32 group_num = rank_idex / max_cross_server_one_group_city_num + 1;
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(template_manager::get_instance().int_to_string(group_num));
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::mail_on_begin_overload_city_war(int32 rank_idex)
	{
		int32 group_num = rank_idex / max_cross_server_one_group_city_num + 1;
		std::vector<std::string> title_params;
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_overload_city_war_begin_title));
		}
		else
		{
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_overload_city_war_begin_title));
		}
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_overload_city_war_begin_context));
		}
		else
		{
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_overload_city_war_begin_context));
		}
		content_params.push_back(template_manager::get_instance().int_to_string(group_num));
		std::string content_text = init_unit::implode(content_params);
		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();
		auto iter = m_member_list.begin();
		for (; iter != m_member_list.end(); ++iter)
		{
			mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title_text, content_text);
		}
	}

	void legion_ws::notice_on_create_bonfire()
	{
		faith::template_manager::template_type* map_table = template_manager::get_instance().get_templates(e_MapTemplate);
		if (nullptr == map_table)
		{
			return;
		}
		int32 bornfire_map_id = 0;
		faith::template_manager::template_type::iterator ite;
		for (ite = map_table->begin(); ite != map_table->end(); ++ite)
		{
			MapTemplate* map_template_ptr = (MapTemplate*)(ite->second);
			if (map_template_ptr == nullptr)
			{
				continue;
			}
			if (map_template_ptr->Type == faith::e_map_type_legion_bonfire)
			{
				bornfire_map_id = map_template_ptr->attribute_id;
				break;
			}
		}

		int32 notice_id = legion_on_create_bonfire_notice_id;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		std::vector<int32> param_array;
		fvector bonfire_pos = fvector();
		param_array.push_back(bornfire_map_id);
		param_array.push_back(bonfire_pos.x);
		param_array.push_back(bonfire_pos.y);
		param_array.push_back(bonfire_pos.z);
		event_ws_mgr::get_instance().send_notice_with_param(m_chief_guid, get_chief_name(), notice_str.c_str(), param_array, e_notice_param_go_to_location, e_chat_type_legion);
	}
	void legion_ws::notice_occupied_treeitory(int32 territory_id)
	{
		CityWarTerritoryTemplate* territory_cfg = GET_TEMPLATE(CityWarTerritoryTemplate, territory_id);
		if (nullptr == territory_cfg)
		{
			return;
		}
		int32 notice_id = legion_occupied_territory_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(get_legion_name());
		vec_notice_str.push_back(template_manager::get_instance().int_to_string(territory_cfg->TerrName));
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}
	void legion_ws::notice_on_legion_boss()
	{
		int32 notice_id = legion_on_legion_boss_notice_id;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::notice_world_mistress()
	{
		int32 notice_id = legion_world_mistress_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(get_legion_name());
		vec_notice_str.push_back(get_chief_name());
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::notice_add_bonfire_successfull(std::string add_bonfire_player_name, int32 get_contribution)
	{
		int32 notice_id = legion_add_bonfire_successfull_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(add_bonfire_player_name);
		vec_notice_str.push_back(template_manager::get_instance().int_to_string(get_contribution));
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}


	void legion_ws::req_send_legion_recruit_in_world(guid_64 player_guid)
	{
		s_legion_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return;
		}
		if (member_info->data_ary[e_legion_member_info_job_title] < e_legion_job_title_assistant_chief)
		{
			return;
		}

		if (false == can_world_recruit_now())
		{
			return;
		}
		m_last_recruit_in_world_stamp = utility::get_tick_count();
		send_legion_recruit(player_guid, member_info->role_name);
	}

	bool legion_ws::can_world_recruit_now()
	{
		const LegionConstructionsTemplate* legion_tmpl_ptr = get_legion_cfg();
		if (nullptr == legion_tmpl_ptr)
		{
			return false;
		}
		int32 req_cd = legion_tmpl_ptr->RecruitInWorldCD;
		uint64 now_stamp = utility::get_tick_count();
		int32 interval_sec = (now_stamp - m_last_recruit_in_world_stamp) / second_tick_time;
		if (interval_sec < req_cd)
		{
			return false;
		}
		return true;
	}

	void legion_ws::send_legion_recruit(guid_64 sender_guid, std::string sender_name, bool is_send_when_create)
	{
		chat_proto_receive_legion_recruit_info msg;
		client_session* session = client_session_mgr::getInstance().get_session(sender_guid);
		if (session == NULL
			|| session->m_status == client_session::e_ss_map_transfer)
		{
			return;
		}
		msg.set_legion_guid(m_legion_info.legion_guid.server_64);
		msg.set_legion_name(m_legion_info.legion_name);
		msg.set_sender_guid(sender_guid.server_64);
		msg.set_sender_name(sender_name);
		msg.set_is_when_create(is_send_when_create);
		msg.set_legion_master_name(get_chief_name());
		msg.set_legion_level(get_legion_info(ELegionInfo_construction_level_main));
		msg.set_sender_template_id(session->get_role_info_data(faith::e_role_info_template_id));
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_reveive_legion_recruit);
	}

	void legion_ws::save_legion_applicant_to_db()
	{
		m_need_save_db_applicant_list.save_to_db();
	}

	void legion_ws::send_legion_boss_map_info(guid_64 role_guid)
	{
		ZoneScoped;
		if (legion_ws_mgr::get_instance().is_in_legion_boss_game_time() == false)
		{
			CONSOLE_ERROR("send_legion_boss_map_info is_in_legion_boss_game_time false");
			return;
		}
		if (!m_legion_station_map_guid.is_valid())
		{
			CONSOLE_ERROR("send_legion_boss_map_info m_legion_station_map_guid is invalid");
			return;
		}
		int32 player_num = 0;
		auto map_ent = get_entity(m_legion_station_map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("send_legion_boss_map_info map_ent is nullptr");
			return;
		}
		player_num = cs_map_system::get_unit_count(map_ent);

		int32 legion_station_map_template_id = GAMECONFIG->LegionStationMapId;
		ws2cs_get_map_info_from_cs get_map_info_from_cs_msg;
		get_map_info_from_cs_msg.map_guid = m_legion_station_map_guid;
		get_map_info_from_cs_msg.role_guid = role_guid;
		get_map_info_from_cs_msg.map_template_id = legion_station_map_template_id;
		get_map_info_from_cs_msg.map_info[0] = player_num;

		bool is_use = proto_by_lua(e_msg_index_ws2cs_get_map_info_from_cs);
		if (is_use == false)
		{
			cs_map_system::send_message_to_cs(map_ent, &get_map_info_from_cs_msg, sizeof(get_map_info_from_cs_msg));
		}
		else
		{
			faith::ws2cs_proto::get_map_info_from_cs pro_msg;

			get_map_info_from_cs_msg.to_proto(pro_msg);
			cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msg_index_ws2cs_get_map_info_from_cs);
		}
	}

	void legion_ws::send_legion_average_lv_to_map_cs(Entity* map_ent)
	{
		int32 legion_sum_lv = 0;
		int32 legion_sum_num = 0;
		std::list<s_legion_member_info> member_list = m_member_list;

		member_list.sort([](const s_legion_member_info& temp_info_a, const s_legion_member_info& temp_info_b)
			{
				return temp_info_a.data_ary[e_legion_member_info_level] > temp_info_b.data_ary[e_legion_member_info_level];
			});

		for (auto member_info : member_list)
		{
			if (member_info.role_guid.is_valid())
			{
				legion_sum_lv += member_info.data_ary[e_legion_member_info_level];
				++legion_sum_num;
				if (legion_sum_num >= calculate_legion_average_lv_need_member_num)
				{
					break;
				}
			}
		}
		if (legion_sum_num <= 0)
		{
			return;
		}
		ws2cs_set_legion_average_lv_to_map msg;
		msg.map_guid = map_ent->getEntityId();
		msg.legion_average_lv = legion_sum_lv / legion_sum_num;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_set_legion_average_lv_to_map);
		if (is_use == false)
		{
			cs_map_system::send_message_to_cs(map_ent ,&msg, msg.get_pak_length());
		}
		else
		{
			faith::ws2cs_proto::set_legion_average_lv_to_map pro_msg;
			msg.to_proto(pro_msg);
			cs_map_system::send_message_to_cs_lua(map_ent ,&pro_msg, e_msg_index_ws2cs_set_legion_average_lv_to_map);
		}
	}

	void legion_ws::level_up_legion_skill(guid_64 player_guid, int32 skill_template_id)
	{
		LegionSkillTemplate* legion_skill_ptr = GET_TEMPLATE(LegionSkillTemplate, skill_template_id);
		if (nullptr == legion_skill_ptr)
		{
			return;
		}

		if (get_legion_info(ELegionInfo_construction_level_1) < legion_skill_ptr->UnlockLevel)
		{
			return;
		}

		s_legion_member_info* player_mem_info = get_member(player_guid);
		if (player_mem_info == nullptr)
		{
			return;
		}
		int64 donate_contribution = player_mem_info->data_ary[e_legion_member_info_donate_contribution];

		ws2cs_level_up_legion_skill level_up_legion_skill_msg;
		level_up_legion_skill_msg.role_guid = player_guid;
		level_up_legion_skill_msg.skill_template_id = skill_template_id;
		level_up_legion_skill_msg.donate_contribution = donate_contribution;

		client_session* player_session = client_session_mgr::getInstance().get_session(player_guid);
		if (nullptr == player_session)
		{
			return;
		}
		bool is_use = proto_by_lua(e_msg_index_ws2cs_level_up_legion_skill);
		if (is_use == false)
		{
			player_session->send_to_cs(&level_up_legion_skill_msg, sizeof(level_up_legion_skill_msg));
		}
		else
		{
			faith::ws2cs_proto::level_up_legion_skill pro_msg;
			level_up_legion_skill_msg.to_proto(pro_msg);
			player_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_level_up_legion_skill);
		}
	}

	void legion_ws::check_legion_selling_end(int32 selling_num)
	{
		if (selling_num == 0)
		{
			set_auction_selling_check_start(false);
		}

		m_selling_num = selling_num;

		legion_proto_auction_is_selling msg;
		msg.set_is_selling(selling_num > 0);
		send_message_to_all_member(&msg, e_msgindex_s2c_update_legion_auction_selling);
	}

	void legion_ws::set_auction_selling_check_start(bool is_start)
	{
		m_is_auction_selling = is_start;
		m_next_auction_check_time = 0;
	}

	void legion_ws::send_legion_attack_city_call(guid_64 role_guid)
	{
		const s_legion_member_info* legion_member_info_ptr = get_member(role_guid);
		if (nullptr == legion_member_info_ptr)
		{
			return;
		}

		std::vector<std::string> content_text_arr;
		std::vector<int32> param_arr;
		param_arr.reserve(e_widget_jump_max);
		std::string content_text;


		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, 71220801);
		if (nullptr == map_template_ptr)
		{
			return;
		}

		content_text_arr.push_back("90306358");
		content_text = init_unit::implode(content_text_arr);
		param_arr.push_back(-1);
		param_arr.push_back(0);
		param_arr.push_back(92810245);
		param_arr.push_back(-1);
		param_arr.push_back(-1);

		event_ws_mgr::get_instance().send_notice_with_param(role_guid, legion_member_info_ptr->role_name, content_text.c_str(), param_arr, e_notice_param_open_widget, e_chat_type_legion, 0, 93000435);
	}

	void legion_ws::check_reset_act_info(e_legion_act_type act_type)
	{
		if (false == time_helper::is_diff_day(m_last_attend_act_stamp[act_type]))
		{
			return;
		}
		m_last_attend_act_stamp[act_type] = time_helper::get_cur_time_new().second;
		m_attend_act_mems_map[act_type].clear();
		set_legion_info((ELegionInfo)(ELegionInfo_act_score_raid + act_type), 0);
	}

	void legion_ws::set_act_got_score(e_legion_act_type act_type, int64 score_val)
	{
		check_reset_act_info(act_type);
		//int32 cur_score = get_legion_info((ELegionInfo)(ELegionInfo_act_score_raid + act_type));
		set_legion_info((ELegionInfo)(ELegionInfo_act_score_raid + act_type), score_val);
	}

	void legion_ws::add_act_attend_mems_info(e_legion_act_type act_type, const s_legion_member_attend_activity* attend_mem_arr, int32 attend_mem_num)
	{
		check_reset_act_info(act_type);
		for (int32 mem_idx = 0; mem_idx < attend_mem_num; mem_idx++)
		{
			const s_legion_member_attend_activity& mem_act_info = attend_mem_arr[mem_idx];
			add_act_attend_mem_info(act_type, mem_act_info);
		}
	}

	void legion_ws::add_act_attend_mem_info(e_legion_act_type act_type, const s_legion_member_attend_activity& attend_mem)
	{
		if (false == attend_mem.mem_guid.is_valid())
		{
			return;
		}
		check_reset_act_info(act_type);
		legion_act_attend_mems_map& mem_list = m_attend_act_mems_map[act_type];
		guid_64 _guid = attend_mem.mem_guid;
		mem_list[_guid].mem_legion_guid = attend_mem.mem_legion_guid;
		mem_list[_guid].mem_guid = attend_mem.mem_guid;
		mem_list[_guid].mem_score += attend_mem.mem_score;
	}

	void legion_ws::give_activity_reward(ActivityCommonConfigTemplate* act_config_ptr, const legion_act_attend_mems_map& mem_list, int32 item_id, int32 item_num)
	{
		if (item_num <= 0 || nullptr == act_config_ptr)
		{
			return;
		}
		/////////////////////////////////////////////////////////////////////////////////////////
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			int32 config_id = act_config_ptr->attribute_id;
			string mem_list_str = "";
			std::stringstream ss;
			for (legion_act_attend_mems_map::const_iterator ite = mem_list.begin(); ite != mem_list.end(); ++ite)
			{
				ss << ite->second.mem_guid.server_64;
				ss << ",";
			}
			ss >> mem_list_str;
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_give_activity_reward", 0, false, "%d%s%d%d%l", config_id, mem_list_str.c_str(), item_id, item_num, get_legion_guid().server_64);
			return;
		}

		//int64 score_mem_all = 0;
		//for (legion_act_attend_mems_map::iterator ite = mem_list.begin(); ite != mem_list.end(); ++ite)
		//{
		//	score_mem_all += ite->second.mem_score;
		//}
		//if (score_mem_all <= 0)
		//{
		//	return;
		//}
		int32 attend_num = init_unit::change_string_to_i64(act_config_ptr->ParamString1);
		if (attend_num < mem_list.size())
		{
			attend_num = mem_list.size();
		}
		if (attend_num <= 0)
		{
			return;
		}
		float _share_rate = 1 / (float)attend_num;

		std::string sell_item_mail_t = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_legion_rank_rwd_title);
		std::string sell_item_mail_c = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_legion_rank_rwd_content);
		std::vector<std::string> content_params;
		content_params.push_back(sell_item_mail_c);
		content_params.push_back(template_manager::get_instance().int_to_string(act_config_ptr->ActivityName));
		sell_item_mail_c = init_unit::implode(content_params);

		int32 rwd_relate_mem_num = 0;
		s_auction_share_info rwd_relate_mems[max_legion_member_num];
		for (legion_act_attend_mems_map::const_iterator ite = mem_list.begin(); ite != mem_list.end(); ++ite)
		{
			rwd_relate_mems[rwd_relate_mem_num].role_guid = ite->second.mem_guid;
			//float _share_rate = ite->second.mem_score / (float)score_mem_all;
			rwd_relate_mems[rwd_relate_mem_num].share_rate = _share_rate > 0.2f ? 0.2f : _share_rate;
			memcpy(rwd_relate_mems[rwd_relate_mem_num].share_mail_title, sell_item_mail_t.c_str(), sell_item_mail_t.length() > max_mail_title_length ? max_mail_title_length : sell_item_mail_t.length());
			memcpy(rwd_relate_mems[rwd_relate_mem_num].share_mail_content, sell_item_mail_c.c_str(), sell_item_mail_c.length() > max_text_contents_length ? max_text_contents_length : sell_item_mail_c.length());
			rwd_relate_mem_num++;
			if (rwd_relate_mem_num >= max_legion_member_num)
			{
				break;
			}
		}

		s_item_info rwd_item_info;
		init_unit::init_item_data(rwd_item_info, item_id, -1, item_num);
		s_auction_info rwd_auction_info;
		for (int32 i = 0; i < item_num; i++)
		{
			rwd_auction_info.clear_data();
			globle_data::get_instance().make_auction_info(rwd_auction_info, rwd_item_info, 1, e_trading_type_legion, get_legion_guid());
			if (false == rwd_auction_info.item_info.item_guid.is_valid())
			{
				continue;
			}
			//auction_mgr_ws::get_instance().add_wait_time(rwd_auction_info);
			auction_mgr_ws::get_instance().add_auction_info_to_db(rwd_auction_info, guid_64(), s_unit_info(), true);
			auction_mgr_ws::get_instance().send_up_notice(rwd_auction_info);

			for (int32 mem_idx = 0; mem_idx < rwd_relate_mem_num; mem_idx++)
			{
				rwd_relate_mems[mem_idx].item_guid = rwd_auction_info.item_info.item_guid;
			}
			auction_mgr_ws::get_instance().add_share_list_to_db(rwd_relate_mems, rwd_relate_mem_num);
		}

		legion_proto_notice_new_item_in_legion_auction msg;
		send_message_to_all_member(&msg, e_msgindex_s2c_notice_new_item_in_legion_auction);
	}

	void legion_ws::give_activity_reward(const ActivityCommonConfigTemplate* act_config_ptr, const guid_64* mem_list, int32 play_num, int32 item_id, int32 item_num)
	{

		if (item_num <= 0 || nullptr == act_config_ptr || mem_list == nullptr)
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			int32 config_id = act_config_ptr->attribute_id;
			string mem_list_str = "";
			std::stringstream ss;
			for (int32 i = 0; i < play_num; i++)
			{
				ss << mem_list[i].server_64;
				ss << ",";
			}
			ss >> mem_list_str;
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_give_activity_reward", 0, false, "%d%s%d%d%l", config_id, mem_list_str.c_str(), item_id, item_num, get_legion_guid().server_64);

			return;
		}
		int32 attend_num = init_unit::change_string_to_i64(act_config_ptr->ParamString1);
		if (attend_num < play_num)
		{
			attend_num = play_num;
		}
		if (attend_num <= 0)
		{
			return;
		}
		float _share_rate = 1 / (float)attend_num;

		std::string sell_item_mail_t = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_legion_rank_rwd_title);
		std::string sell_item_mail_c = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_legion_rank_rwd_content);
		std::vector<std::string> content_params;
		content_params.push_back(sell_item_mail_c);
		content_params.push_back(template_manager::get_instance().int_to_string(act_config_ptr->ActivityName));
		sell_item_mail_c = init_unit::implode(content_params);

		int32 rwd_relate_mem_num = 0;
		s_auction_share_info rwd_relate_mems[max_legion_member_num];
		for (int32 i = 0; i < play_num; i++)
		{
			rwd_relate_mems[rwd_relate_mem_num].role_guid = mem_list[i];
			rwd_relate_mems[rwd_relate_mem_num].share_rate = _share_rate > 0.2f ? 0.2f : _share_rate;
			memcpy(rwd_relate_mems[rwd_relate_mem_num].share_mail_title, sell_item_mail_t.c_str(), sell_item_mail_t.length() > max_mail_title_length ? max_mail_title_length : sell_item_mail_t.length());
			memcpy(rwd_relate_mems[rwd_relate_mem_num].share_mail_content, sell_item_mail_c.c_str(), sell_item_mail_c.length() > max_text_contents_length ? max_text_contents_length : sell_item_mail_c.length());
			rwd_relate_mem_num++;
			if (rwd_relate_mem_num >= max_legion_member_num)
			{
				break;
			}
			_RLOG_(MINFO, ::faith::log_detail::format_message("roleguid:{} sharerate:{} itemid:{} itemnum:{} activittype:{}",  mem_list[i].server_64,  _share_rate,  item_id,  item_num,  act_config_ptr->ActivityType));

		}

		s_item_info rwd_item_info;
		init_unit::init_item_data(rwd_item_info, item_id, -1, item_num);
		s_auction_info rwd_auction_info;
		for (int32 i = 0; i < item_num; i++)
		{
			rwd_auction_info.clear_data();
			globle_data::get_instance().make_auction_info(rwd_auction_info, rwd_item_info, 1, e_trading_type_legion, get_legion_guid());
			if (false == rwd_auction_info.item_info.item_guid.is_valid())
			{
				continue;
			}
			//auction_mgr_ws::get_instance().add_wait_time(rwd_auction_info);
			auction_mgr_ws::get_instance().add_auction_info_to_db(rwd_auction_info, guid_64(), s_unit_info(), true);
			auction_mgr_ws::get_instance().send_up_notice(rwd_auction_info);

			for (int32 mem_idx = 0; mem_idx < rwd_relate_mem_num; mem_idx++)
			{
				rwd_relate_mems[mem_idx].item_guid = rwd_auction_info.item_info.item_guid;
			}
			auction_mgr_ws::get_instance().add_share_list_to_db(rwd_relate_mems, rwd_relate_mem_num);
		}

		legion_proto_notice_new_item_in_legion_auction msg;
		send_message_to_all_member(&msg, e_msgindex_s2c_notice_new_item_in_legion_auction);
	}


	int32 legion_ws::get_ranking_list_type(e_legion_rank_type legion_type, int32 sub_id)
	{
		switch (legion_type)
		{
		case faith::e_legion_rank_type_fighting_power:
			return e_RankingIndex_legion_core;
			break;
		case faith::e_legion_rank_type_city_war:
			return e_RankingIndex_legion_territory;
			break;
		case faith::e_legion_rank_type_world_elite:
		case faith::e_legion_rank_type_world_boss:
		{
			if (-1 != sub_id)
			{
				RankingShowTemplate* template_ptr = template_manager::get_instance().get_ranking_template_by_npc_id(sub_id);
				if (nullptr != template_ptr)
				{
					return template_ptr->RankingType;
				}
			}
		}
		break;
		case faith::e_legion_rank_type_raid:
		{
			return e_RankingIndex_legion_boss;
		}
		break;
		case faith::e_legion_rank_type_bonfire:
		{
			return e_RankingIndex_legion_bonfire;
		}
		break;
		case faith::e_legion_rank_type_max:
			break;
		default:
			break;
		}
		return -1;
	}


	void legion_ws::gen_info_in_rank_list(s_ranking_player_info& new_rank_info, int64 rank_score, e_legion_rank_type rank_type, int32 sub_id)
	{
		new_rank_info.role_guid = get_legion_guid();
		new_rank_info.set_role_name(m_legion_info.legion_name);
		int64 rangking_type = get_ranking_list_type(rank_type, sub_id);
		if (-1 != rangking_type)
		{
			new_rank_info.ranking_type = rangking_type;
		}
		s_legion_member_info* chief_info = get_member(m_chief_guid);
		if (nullptr != chief_info)
		{
			new_rank_info.supporting_guid = m_chief_guid;
			new_rank_info.set_supporting_name(chief_info->role_name);
			new_rank_info.vip_level = chief_info->data_ary[e_legion_member_info_vip_level];
		}
		new_rank_info.ranking_value = rank_score;
		new_rank_info.save_time = time_helper::get_cur_time_new().second;
		new_rank_info.server_id = world_server::getInstance().get_server_id();
	}

	std::vector<int32> legion_ws::get_legion_occupied_territorys()
	{
		std::vector<int32> occupied_territorys;

		for (std::set<int32>::iterator iter = m_city_war_territories.begin(); iter != m_city_war_territories.end(); ++iter)
		{
			int32 terr_id = *iter;
			if (city_war_territory_mgr::get_instance().get_occupy_legion_guid(terr_id) == get_legion_guid())
			{
				occupied_territorys.push_back(terr_id);
			}
		}
		return occupied_territorys;
	}
	std::vector<int32> legion_ws::get_legion_occupied_territorys_cross_city()
	{
		std::vector<int32> occupied_territorys;

		for (std::set<int32>::iterator iter = m_cross_city_territories_prior.begin(); iter != m_cross_city_territories_prior.end(); ++iter)
		{
			int32 terr_id = *iter;
			occupied_territorys.push_back(terr_id);
		}
		for (std::set<int32>::iterator iter = m_cross_city_territories_common.begin(); iter != m_cross_city_territories_common.end(); ++iter)
		{
			int32 terr_id = *iter;
			occupied_territorys.push_back(terr_id);
		}

		return occupied_territorys;
	}

	void legion_ws::del_legion_station_map()
	{
		auto map_ent = get_entity(m_legion_station_map_guid);
		if (nullptr == map_ent)
		{
			return;
		}
		ws2cs_transfer_this_map_all_player msg;
		msg.map_guid = m_legion_station_map_guid;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_transfer_all_player_this_map);
		if (is_use == false)
		{
			cs_map_system::send_message_to_cs(map_ent, &msg, sizeof(msg));
		}
		else
		{
			faith::ws2cs_proto::transfer_this_map_all_player pro_msg;
			msg.to_proto(pro_msg);
			cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msg_index_ws2cs_transfer_all_player_this_map);
		}

		cs_map_mgr_system::remove_map_by_guid(m_legion_station_map_guid);
	}

	void legion_ws::create_legion_station_map()
	{
		cs_map_mgr_system::remove_map_by_guid(m_legion_station_map_guid);
		m_legion_station_map_guid.clear_data();

		auto map_ent = cs_map_mgr_system::init_map_by_battle(GAMECONFIG->LegionStationMapId);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("create_legion_station_map failed LegionStationMapId:", GAMECONFIG->LegionStationMapId);
			return;
		}
		m_legion_station_map_guid = map_ent->getEntityId();

		send_legion_average_lv_to_map_cs(map_ent);

		ws2cs_set_legion_guid_to_map msg;
		msg.map_guid = map_ent->getEntityId();
		msg.legion_guid = get_legion_guid();
		bool is_use = proto_by_lua(e_msg_index_ws2cs_set_legion_guid_to_map);
		if (is_use == false)
		{
			cs_map_system::send_message_to_cs(map_ent, &msg, sizeof(msg));
		}
		else
		{
			faith::ws2cs_proto::set_legion_guid_to_map pro_msg;
			msg.to_proto(pro_msg);
			cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msg_index_ws2cs_set_legion_guid_to_map);
		}
	}

	void legion_ws::join_legion_station_map(guid_64 role_guid)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		if (!is_in_this_legion(role_guid))
		{
			return;
		}
		guid_64 map_guid = get_valid_legion_station_map();
		if (!map_guid.is_valid())
		{
			cs_map_mgr_system::send_map_error_to_client(session, e_error_code_map_init_map_err);
			return;
		}

		int32 legion_station_map_template_id = GAMECONFIG->LegionStationMapId;

		cs_map_mgr_system::transfer_to_map(session, legion_station_map_template_id, map_guid);
	}

	void legion_ws::legion_brofire_end()
	{
		m_bonfire_add_fuel_times = 0;
		m_member_add_fuel_times.clear();
		m_got_bonfire_reward_members.clear();
	}

	void legion_ws::set_legion_boss_create_time(int64 creat_time)
	{
		m_creat_legion_boss_stamp = creat_time;
	}

	guid_64 legion_ws::get_valid_legion_station_map()
	{
		return m_legion_station_map_guid;
	}

	void legion_ws::set_kill_legion_boss_name(const xchar* _killer_name)
	{
		m_kill_legion_boss_name = _killer_name;
	}

	int32 faith::legion_ws::get_job_title_num(int32 job_title)
	{
		int32 job_num = 0;
		for (s_legion_member_info& legion_member_data : m_member_list)
		{
			if (legion_member_data.data_ary[e_legion_member_info_job_title] == job_title)
			{
				job_num++;
			}
		}
		return job_num;
	}

	int64 faith::legion_ws::get_last_stimulated_stamp_by_guid(guid_64 player_guid)
	{
		unit_guid_map_it ite;
		ite = m_last_stimulate_bonus_stamp.find(player_guid.server_64);
		if (ite == m_last_stimulate_bonus_stamp.end())
		{
			return 0;
		}
		return ite->second;
	}

	void faith::legion_ws::insert_last_stimulated_stamp(guid_64 player_guid, int64 stmp)
	{
		m_last_stimulate_bonus_stamp[player_guid.server_64] = stmp;
	}

	void faith::legion_ws::join_voice_channel(guid_64 role_guid)
	{
		bool is_first_join = true;
		for (s_legion_member_info& legion_member_info : m_member_list)
		{
			if (legion_member_info.data_ary[faith::e_legion_member_info_agora_id] != 0)
			{
				is_first_join = false;
			}
		}

		update_member_info_one(role_guid, faith::e_legion_member_info_agora_id, 1, true);

		if (is_first_join == true)
		{
			faith::chat_proto_invite_join_voice_channel msg;
			msg.set_role_guid(role_guid.server_64);
			msg.set_channel_type(faith::e_chat_type_legion);
			send_message_to_all_member_except(&msg, e_msgindex_s2c_invite_join_voice_channel, role_guid);
		}
		else
		{
			s_legion_member_info* member_info = get_member(role_guid);
			if (member_info == nullptr)
			{
				return;
			}
			faith::chat_proto_join_or_leave_voice_channel_notice msg;
			msg.set_channel_type(faith::e_chat_type_legion);
			msg.set_role_name(member_info->role_name);
			msg.set_is_join(true);
			send_message_to_all_member_except(&msg, e_msgindex_s2c_join_or_leave_voice_channel_notice, role_guid);
		}
	}

	void faith::legion_ws::leave_voie_channel(guid_64 role_guid)
	{
		s_legion_member_info* member_info = get_member(role_guid);
		if (member_info == nullptr)
		{
			return;
		}

		int32 agora_id = member_info->data_ary[faith::e_legion_member_info_agora_id];
		if (agora_id == 0)
		{
			return;
		}
		update_member_info_one(role_guid, faith::e_legion_member_info_agora_id, 0, true);

		faith::chat_proto_join_or_leave_voice_channel_notice msg;
		msg.set_channel_type(faith::e_chat_type_legion);
		msg.set_role_name(member_info->role_name);
		msg.set_is_join(false);
		send_message_to_all_member_except(&msg, e_msgindex_s2c_join_or_leave_voice_channel_notice, role_guid);
	}

	std::string legion_ws::get_kill_legion_boss_name()
	{
		if (time_helper::is_diff_day(m_creat_legion_boss_stamp))
		{
			m_kill_legion_boss_name = "";
		}
		return m_kill_legion_boss_name;
	}

	void legion_ws::send_applicant_info_list(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		if (get_job_title(session->get_role_guid()) <= e_legion_job_title_none)
		{
			return;
		}

		legion_ws_applicant_list& applicant_list = get_applicant_list();
		legion_proto_get_applicant_info_list_end get_legion_applicant_list_end_msg;
		int32 ret = applicant_list.fill_get_legion_applicant_list_msg(get_legion_applicant_list_end_msg);
		if (ret != 1)
		{
			return;
		}
		session->send_to_client(&get_legion_applicant_list_end_msg, e_msgindex_s2c_get_legion_applicant_info_list_end);
	}

	void legion_ws::send_applicant_info_list_to_have_job_member()
	{
		for (s_legion_member_info& member_info : m_member_list)
		{
			if (member_info.role_guid.is_valid() == false)
			{
				continue;
			}
			if (get_job_title(member_info.role_guid) < e_legion_job_title_commander)
			{
				continue;
			}
			client_session* session = client_session_mgr::getInstance().get_session(member_info.role_guid);
			if (nullptr == session)
			{
				continue;
			}
			send_applicant_info_list(session);
		}
	}

	void legion_ws::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		s_legion_member_info* legion_member_ptr = get_member(role_guid);
		if (nullptr != legion_member_ptr)
		{
			xchar char_role_name[max_name_size + 1] = { 0 };
			memcpy(char_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());

			if (legion_member_ptr->role_name == m_kill_legion_boss_name)
			{
				m_kill_legion_boss_name = role_name;
			}

			xstring original_name = legion_member_ptr->role_name;
			legion_member_ptr->set_role_name(char_role_name);
			if (role_guid == m_chief_guid)
			{
				cross_server_city_war_ws_mgr::get_instance().send_to_gate_change_legion_msg(get_legion_guid(), e_change_cross_city_type_change_chief, role_name, role_guid);
			}
			send_legion_info_to_all_member();
			send_change_name_mail_to_all_member(original_name, role_name);
			m_event_logger.add_member_change_name_event(original_name, role_name);
			notice_member_change_name(original_name, role_name);
		}

		legion_ws_applicant* legion_ws_applicant_ptr = m_applicant_list.get_applicant(role_guid);
		if (nullptr != legion_ws_applicant_ptr)
		{
			legion_ws_applicant_ptr->set_applicant_name(role_name);
		}
		legion_ws_applicant* legion_ws_applicant_other_ptr = m_need_save_db_applicant_list.get_applicant(role_guid);
		if (nullptr != legion_ws_applicant_other_ptr)
		{
			legion_ws_applicant_other_ptr->set_applicant_name(role_name);
		}

	}

	void legion_ws::send_legion_info_to_all_member()
	{
		auto iter = m_member_list.begin();
		for (; iter != m_member_list.end(); ++iter)
		{
			client_session* player_session = client_session_mgr::getInstance().get_session(iter->role_guid);
			if (nullptr == player_session)
			{
				continue;
			}

			send_legion_all_info(player_session);
		}
	}

	void legion_ws::send_change_name_mail_to_all_member(const xstring& original_name, const xstring& current_name)
	{
		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_change_name_inform_legion_member_title));
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_change_name_inform_legion_member_content));
		content_params.push_back(original_name);
		content_params.push_back(current_name);
		std::string content_text = init_unit::implode(content_params);
		std::vector<s_item_template_info> drop_item_list;

		auto iter = m_member_list.begin();
		for (; iter != m_member_list.end(); ++iter)
		{
			mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title_text, content_text);
		}
	}

	void legion_ws::notice_on_end_overload_city_war(int32 rank_idex)
	{
		int32 notice_id = 93000301;
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			notice_id = 93000409;
		}
		int32 group_num = rank_idex / max_cross_server_one_group_city_num + 1;
		int32 group_rank_num = rank_idex % max_cross_server_one_group_city_num + 1;
		std::vector<std::string> vec_str_up_item;
		vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_up_item.push_back(template_manager::get_instance().int_to_string(group_num));
		vec_str_up_item.push_back(template_manager::get_instance().int_to_string(group_rank_num));
		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::mail_on_end_overload_city_war(int32 rank_idex)
	{
		int32 group_num = rank_idex / max_cross_server_one_group_city_num + 1;
		int32 group_rank_num = rank_idex % max_cross_server_one_group_city_num + 1;
		std::vector<std::string> title_params;
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_overload_city_war_end_title));
		}
		else
		{
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_overload_city_war_end_title));
		}
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		if (!world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_overload_city_war_end_context));
		}
		else
		{
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_overload_city_war_end_context));
		}
		content_params.push_back(template_manager::get_instance().int_to_string(group_num));
		content_params.push_back(template_manager::get_instance().int_to_string(group_rank_num));
		std::string content_text = init_unit::implode(content_params);
		std::vector<s_item_template_info> drop_item_list;
		drop_item_list.clear();
		auto iter = m_member_list.begin();
		for (; iter != m_member_list.end(); ++iter)
		{
			mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title_text, content_text);
		}
	}

	void legion_ws::send_mail_to_all_member_with_city_rank(int32 rank_idex, guid_64* play_guid_list, int32 play_num)
	{
		notice_on_end_overload_city_war(rank_idex);
		mail_on_end_overload_city_war(rank_idex);
		send_legion_welfare_by_activity_and_rank(e_activity_type_overlord_city_war, rank_idex + 1);
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_overlord_city_war);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		int32 world_level = ranking_mgr_ws::get_world_level_last();
		std::vector<int32> reward_arr;
		reward_arr.clear();
		std::vector<int32> chief_reward_arr;
		chief_reward_arr.clear();
		for (int32 i = 0; i < max_overload_city_war; i++)
		{
			OverloadCityWarRewarTemplate* over_com_cfg = GET_TEMPLATE(OverloadCityWarRewarTemplate, overload_city_war_reward_first_temp_id + i);
			if (nullptr == over_com_cfg)
			{
				break;
			}
			if (over_com_cfg->IsCross == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				std::vector<int32> level_limit = over_com_cfg->LevelLimit;
				if (level_limit.size() < 2)
				{
					continue;
				}
				if (world_level >= level_limit[0] && world_level <= level_limit[1])
				{
					reward_arr = over_com_cfg->RewardItemArray;
					chief_reward_arr = over_com_cfg->ChiefRewardArray;
					break;
				}
			}
		}

		if (reward_arr.size() <= rank_idex || chief_reward_arr.size() <= rank_idex)
		{
			return;
		}
		int32 drop_id = reward_arr[rank_idex];
		int32 chief_drop_id = chief_reward_arr[rank_idex];
		std::vector<s_item_template_info> drop_item_list;
		std::vector<s_item_template_info> chief_drop_item_list;
		globle_data::get_instance().get_item_list_by_drop_bg(drop_id, drop_item_list, 0, world_level);
		globle_data::get_instance().get_item_list_by_drop_bg(chief_drop_id, chief_drop_item_list, 0, world_level);

		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_city_rank_award_title));
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_city_rank_award_context));
		int32 group_num = rank_idex / 3 + 1;
		int32 count_num = rank_idex % 3 + 1;
		content_params.push_back(init_unit::change_i32_to_string(group_num));
		content_params.push_back(init_unit::change_i32_to_string(count_num));
		std::string content_text = init_unit::implode(content_params);

		//auto iter = m_member_list.begin();
		//for (; iter != m_member_list.end(); ++iter)
		//{
		//	mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title_text, content_text);
		//}
		if (drop_item_list.size() > 0)
		{
			for (int32 i = 0; i < drop_item_list.size(); i++)
			{
				give_activity_reward(act_com_cfg, play_guid_list, play_num, drop_item_list[i].m_item_id, drop_item_list[i].m_item_num);
			}
		}


		if (m_chief_guid.is_valid())
		{
			std::vector<std::string> chief_title_params;
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				chief_title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_city_rank_award_title_chief));
			}
			else
			{
				chief_title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_city_rank_award_title_chief));
			}

			std::string chief_title_text = init_unit::implode(chief_title_params);
			std::vector<std::string> chief_content_params;
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
			{
				chief_content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cross_city_rank_award_context_chief));
			}
			else
			{
				chief_content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_city_rank_award_context_chief));
			}
			chief_content_params.push_back(init_unit::change_i32_to_string(group_num));
			chief_content_params.push_back(init_unit::change_i32_to_string(count_num));
			std::string chief_content_text = init_unit::implode(chief_content_params);
			mail_ws_mgr::get_instance().send_mail_system(m_chief_guid, 0, chief_drop_item_list, chief_title_text, chief_content_text);
		}

	}

	void legion_ws::notice_member_change_name(const xstring& original_name, const xstring& current_name)
	{
		int32 notice_id = legion_member_change_name_notice_id;
		std::vector<std::string> vec_str_notice;
		vec_str_notice.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_notice.push_back(original_name);
		vec_str_notice.push_back(current_name);
		std::string notice_str = init_unit::implode(vec_str_notice);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::change_legion_name(const guid_64& role_guid, const xchar* legion_name, int32 legion_name_len)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}

		if (!is_chief(role_guid))
		{
			change_legion_name_end(role_guid, e_legion_name_error_not_chief);
			return;
		}

		if (legion_name_len < legion_name_size_min_limit || legion_name_len > max_name_size)
		{
			change_legion_name_end(role_guid, e_legion_name_error_formal_error);
			return;
		}

		if (invalid_ansi_word::is_valid_ansi_str(legion_name) == false)
		{
			change_legion_name_end(role_guid, e_legion_name_error_contains_illegal_characters);
			return;
		}

		if (invalid_ansi_word::include_invalid_ansi_str(legion_name))
		{
			change_legion_name_end(role_guid, e_legion_name_error_contains_illegal_characters);
			return;
		}

		if (legion_ws_mgr::get_instance().exist_name_in_legion_name_searcher(legion_name))
		{
			change_legion_name_end(role_guid, e_legion_name_error_repetition);
			return;
		}

		change_legion_name_end(role_guid, e_legion_name_error_available);
	}

	void legion_ws::change_legion_name_end(const guid_64& role_guid, int32 num)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		legion_proto_req_change_legion_name_end msg;
		msg.set_ret(num);
		session->send_to_client(&msg, e_msgindex_s2c_change_legion_name_end);
	}

	void legion_ws::confirm_change_legion_name(const guid_64& role_guid, const xchar* legion_name, int32 legion_name_len)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}

		if (legion_ws_mgr::get_instance().exist_name_in_legion_name_searcher(legion_name))
		{
			change_legion_name_end(role_guid, e_legion_name_error_repetition);
			return;
		}

		set_m_new_legion_name(legion_name);

		ws2cs_rem_item_change_legion_name_care msg;
		msg.role_guid = session->get_role_guid();

		bool is_use = proto_by_lua(e_msg_index_ws2cs_rem_item_change_legion_name_care);
		if (is_use == false)
		{
			session->send_to_cs(&msg, e_msg_index_ws2cs_rem_item_change_legion_name_care);
		}
		else
		{
			faith::ws2cs_proto::rem_item_change_legion_name_care pro_msg;
			msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_rem_item_change_legion_name_care);
		}
	}
	void legion_ws::ref_all_about_legion_name(const guid_64& role_guid)
	{
		legion_ws_mgr::get_instance().clear_name_in_legion_name_searcher(m_legion_info.legion_name);

		xstring old_legion_name = m_legion_info.legion_name;
		memset(m_legion_info.legion_name, 0, sizeof(m_legion_info.legion_name));
		set_legion_name(m_new_legion_name.c_str(), m_new_legion_name.size());
		m_is_legion_have_change = true;

		legion_ws_mgr::get_instance().add_name_in_legion_name_searcher(m_legion_info.legion_name);

		send_change_legion_name_mail_to_all_member(m_legion_info.legion_name);
		notice_member_change_legion_name(m_legion_info.legion_name);

		world_boss_ws_mgr::get_instance().world_boss_change_legion_name_func(m_legion_info.legion_guid, m_legion_info.legion_name);
		cross_server_city_war_ws_mgr::get_instance().send_to_gate_change_legion_msg(get_legion_guid(), e_change_cross_city_type_change_legion_name, get_legion_name());
		legion_ws_mgr::get_instance().change_gate_legion_name(get_legion_guid(), get_legion_name());
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		std::list<s_legion_member_info>::iterator ite;
		for (ite = m_member_list.begin(); ite != m_member_list.end(); ++ite)
		{
			ranking_mgr_ws::change_ranking_player_legion_info(ite->role_guid, m_legion_info.legion_guid, m_legion_info.legion_name);
			ranking_mgr_ws::change_ranking_legion_name_info(ite->role_guid, m_legion_info.legion_guid, m_legion_info.legion_name);

			client_session* legion_session = client_session_mgr::getInstance().get_session(ite->role_guid);
			if (nullptr == legion_session)
			{
				continue;
			}
			ws2cs_confirm_change_legion_name msg;
			msg.role_guid = ite->role_guid;
			msg.set_legion_name(m_legion_info.legion_name);
			msg.is_need_change = (ite->role_guid == m_chief_guid);

			bool is_use = proto_by_lua(e_msg_index_ws2cs_confirm_change_legion_name);
			if (is_use == false)
			{
				legion_session->send_to_cs(&msg, sizeof(ws2cs_confirm_change_legion_name));
			}
			else
			{
				faith::ws2cs_proto::confirm_change_legion_name pro_msg;
				msg.to_proto(pro_msg);
				legion_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_confirm_change_legion_name);
			}

			legion_session->set_legion_name(m_legion_info.legion_name);
			send_legion_all_info(legion_session);
			world_boss_ws_mgr::get_instance().send_damage_list(legion_session);
		}
	}

	void legion_ws::set_m_new_legion_name(const	xchar* new_legion_name)
	{
		if (nullptr == new_legion_name)
		{
			return;
		}
		m_new_legion_name = new_legion_name;
	}

	void legion_ws::notice_member_change_legion_name(const xstring& original_name)
	{
		int32 notice_id = legion_member_change_legion_name_notice_id;
		std::vector<std::string> vec_str_notice;
		vec_str_notice.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_str_notice.push_back(original_name);
		std::string notice_str = init_unit::implode(vec_str_notice);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_chief_guid, notice_str);
	}

	void legion_ws::send_change_legion_name_mail_to_all_member(const xstring& original_name)
	{
		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_change_legion_name_inform_legion_member_title));
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_change_legion_name_inform_legion_member_content));
		content_params.push_back(original_name);
		std::string content_text = init_unit::implode(content_params);
		std::vector<s_item_template_info> drop_item_list;

		auto iter = m_member_list.begin();
		for (; iter != m_member_list.end(); ++iter)
		{
			mail_ws_mgr::get_instance().send_mail_system(iter->role_guid, 0, drop_item_list, title_text, content_text);
		}
	}

	void legion_ws::set_next_bid_time()
	{
		m_next_bid_time = utility::get_tick_count() + second_tick_time / 2;
	}

	bool legion_ws::is_can_bid()
	{
		int64 cur_time = utility::get_tick_count();
		if (cur_time >= m_next_bid_time)
		{
			return true;
		}

		return false;
	}

	void legion_ws::set_strategy_world_boss_arr(int32 index, int32 value)
	{
		if (0 <= index && index < max_world_boss_arr_num)
		{
			m_strategy_world_boss[index] = value;
		}
	}

	int32 legion_ws::get_strategy_world_boss_arr(int32 index)
	{
		if (0 <= index && index < max_world_boss_arr_num)
		{
			return m_strategy_world_boss[index];
		}
		else
		{
			return 0;
		}
	}


	void legion_ws::send_legion_strategy_world_boss()
	{
		legion_proto_legion_strategy_world_boss_info msg;
		msg.set_low_boss_id(m_strategy_world_boss[0]);
		msg.set_intermediate_boss_id(m_strategy_world_boss[1]);
		msg.set_senior_boss_id(m_strategy_world_boss[2]);

		send_message_to_all_member(&msg, e_msgindex_s2c_legion_strategy_world_boss_info);
	}

	void legion_ws::send_one_legion_strategy_world_boss(guid_64 role_guid)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}

		for (int32 i = 0; i < legion_strategy_world_boss_num; i++)
		{
			if (m_strategy_world_boss[i] == 0)
			{
				world_boss_ws_mgr::get_instance().assign_world_boss_to_one_legion(get_legion_guid());
			}
		}

		legion_proto_legion_strategy_world_boss_info msg;
		msg.set_low_boss_id(m_strategy_world_boss[0]);
		msg.set_intermediate_boss_id(m_strategy_world_boss[1]);
		msg.set_senior_boss_id(m_strategy_world_boss[2]);

		session->send_to_client(&msg, e_msgindex_s2c_send_legion_strategy_world_boss_info);
	}

	xstring legion_ws::get_all_member_guid()
	{

		int32 count = m_member_list.size();
		std::string res_value = "";
		std::stringstream ss;
		int32 pos = 0;
		std::list<s_legion_member_info>::iterator ite;
		for (ite = m_member_list.begin(); ite != m_member_list.end(); ++ite)
		{

			int64 value_64 = 0;
			if (ite->role_guid.is_valid() == false || ite->data_ary[e_legion_member_info_is_online] <= 0)
			{
				value_64 = 0;
			}
			else
			{
				value_64 = ite->role_guid.server_64;
			}
			ss << value_64;

			pos++;
			if (pos != count)
			{
				ss << ",";
			}
		}
		ss >> res_value;
		return res_value;
	}
}
