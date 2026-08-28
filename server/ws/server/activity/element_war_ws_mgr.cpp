
/***********************************************
* created	: 2020年8月18日
* file base : element_war_ws_mgr
* file ext  : cpp
* author    : 赵玉明
* purpose   : 元素争霸
************************************************/


#include "element_war_ws_mgr.h"
#include "Logic/char_def.hpp"
#include "template/template_manager.h"
#include "server/mail/mail_event_ws.h"
#include "server/mail/event_ws_mgr.h"
#include "ws_client.hpp"
#include "world_server.hpp"
#include "server/ranking/ranking_mgr_ws.h"
#include "utility/guid_gen.h"
#include "utility/init_unit.h"
#include "utility/random.h"
#include "game_cfg/servers_config.h"
#include "globle_data.h"
#include "system/scene/cs_map_mgr_system.h"
#include "server/team/team_ws_mgr.h"
#include "game.pb.h"
#include "net.pb.h"

namespace hld
{
	c_element_war_role_info::c_element_war_role_info()
	{
		m_role_state = e_element_war_role_no_sign_up;
		m_sign_up_time = 0;
		m_previous_opponent = 0;
		m_server_out_port = 0;
	}

	c_element_war_role_info::~c_element_war_role_info()
	{

	}

	void c_element_war_role_info::set_role_info(const s_element_war_role_info& info)
	{
		memcpy(&m_role_info, &info, sizeof(m_role_info));
	}

	void c_element_war_role_info::set_team_guid(const guid_64 & temp_guid)
	{
		m_team_guid = temp_guid;
	}

	void c_element_war_role_info::clear_team_guid()
	{
		m_team_guid.clear_data();
	}

	void c_element_war_role_info::set_role_guid(const guid_64& temp_guid)
	{
		m_role_info.role_guid = temp_guid;
	}

	void c_element_war_role_info::set_role_name(const xchar* name)
	{
		memset(m_role_info.role_name, 0, sizeof(m_role_info.role_name));
		int32 copy_size = strlen(name);
		copy_size = copy_size > max_name_size ? max_name_size : copy_size;
		memcpy(m_role_info.role_name, name, copy_size);
	}

	void c_element_war_role_info::set_role_name(const xstring& name)
	{
		memset(m_role_info.role_name, 0, sizeof(m_role_info.role_name));
		int32 copy_size = name.size();
		copy_size = copy_size > max_name_size ? max_name_size : copy_size;
		memcpy(m_role_info.role_name, name.c_str(), copy_size);
	}

	void c_element_war_role_info::set_server_name(const xchar* name)
	{
		memset(m_role_info.server_name, 0, max_name_size);
		memcpy(m_role_info.server_name, name, max_name_size);
	}

	void c_element_war_role_info::set_data_array_info(e_element_war_role_info info_type, int32 info_value)
	{
		if (info_type >= e_element_war_role_info_max)
		{
			return;
		}
		m_role_info.data_array[info_type] = info_value;
	}

	void c_element_war_role_info::set_gs_value(int64 gs_value)
	{
		m_role_info.gs_value = gs_value;
	}

	void c_element_war_role_info::set_segment_value(int32 segment_value)
	{
		m_cur_segment_value = segment_value;
	}

	void c_element_war_role_info::check_reset_pk_state()
	{
		if (m_role_state == e_element_war_role_sign_up && (m_sign_up_time + 600 * second_tick_time) < utility::get_tick_count())
		{
			m_role_state = e_element_war_role_no_sign_up;
		}
		else if (m_role_state == e_element_war_role_join_war)
		{
			m_role_state = e_element_war_role_no_sign_up;
		}
	}

	void c_element_war_role_info::reduce_ticket()
	{
		if (m_role_info.data_array[e_element_war_role_info_ticket_num])
		{
			m_role_info.data_array[e_element_war_role_info_ticket_num] -= 1;
			update_info_to_dp();
		}
	}

	void c_element_war_role_info::add_ticket_num()
	{
		m_role_info.data_array[e_element_war_role_info_ticket_num] += 1;
		update_info_to_dp();
	}

	int32 c_element_war_role_info::get_data_array_info(e_element_war_role_info info_type) const
	{
		if (info_type >= e_element_war_role_info_max || info_type < e_element_war_role_info_template_id)
		{
			return 0;
		}
		return m_role_info.data_array[info_type];
	}
	int64 c_element_war_role_info::get_gs_value()
	{
		return m_role_info.gs_value;
	}

	void c_element_war_role_info::set_server_info(const xstring& out_ip, int32 out_port)
	{
		m_server_out_ip = out_ip;
		m_server_out_port = out_port;
	}

	void c_element_war_role_info::update_info_to_dp()
	{
		ws2dp_element_war_update_info dp_msg;
		dp_msg.role_guid = m_role_info.role_guid;
		dp_msg.total_win_num = m_role_info.data_array[e_element_war_role_info_total_win_num];
		dp_msg.last_win_num = m_role_info.data_array[e_element_war_role_info_last_win_num];
		dp_msg.score_num = m_role_info.data_array[e_element_war_role_info_score_num];
		dp_msg.ticket_num = m_role_info.data_array[e_element_war_role_info_ticket_num];
		dp_msg.have_buy_num = m_role_info.data_array[e_element_war_role_info_have_buy_num];
		dp_msg.history_score = m_role_info.data_array[e_element_war_role_info_history_score];
		dp_msg.cur_season_join_num = m_role_info.data_array[e_element_war_role_info_cur_season_join_num];
		dp_msg.join_num = m_role_info.data_array[e_element_war_role_info_total_join_num];
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void c_element_war_role_info::refresh_season_info()
	{
		ws2dp_element_war_refresh_season_info dp_msg;
		dp_msg.role_guid = m_role_info.role_guid;
		dp_msg.score_num = m_role_info.data_array[e_element_war_role_info_score_num];
		dp_msg.last_season = m_role_info.data_array[e_element_war_role_info_last_season];
		dp_msg.history_score = m_role_info.data_array[e_element_war_role_info_history_score];
		dp_msg.last_score = m_role_info.data_array[e_element_war_role_info_last_score];
		dp_msg.season_reward = m_role_info.data_array[e_element_war_role_info_season_reward];
		dp_msg.cur_season_join_num = m_role_info.data_array[e_element_war_role_info_cur_season_join_num];
		dp_msg.last_season_join_num = m_role_info.data_array[e_element_war_role_info_last_season_join_num];
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	std::vector<int32> c_element_war_role_info::get_robot_array()
	{
		std::vector<int32> robot_array;
		ElementLadderTemplate* template_ptr = template_manager::get_instance().get_template_by_element_score(m_role_info.data_array[e_element_war_role_info_score_num]);
		if (nullptr == template_ptr)
		{
			return robot_array;
		}
		return template_ptr->RobotTemplateID;
	}

	void c_element_war_role_info::save_mission_info()
	{
		ws2dp_element_war_refresh_mission_info dp_msg;
		dp_msg.role_guid = m_role_info.role_guid;
		dp_msg.mission1 = m_role_info.data_array[e_element_war_role_info_mission1];
		dp_msg.mission2 = m_role_info.data_array[e_element_war_role_info_mission2];
		dp_msg.mission3 = m_role_info.data_array[e_element_war_role_info_mission3];
		dp_msg.mission4 = m_role_info.data_array[e_element_war_role_info_mission4];
		dp_msg.mission1_have_num = m_role_info.data_array[e_element_war_role_info_mission1_have_num];
		dp_msg.mission2_have_num = m_role_info.data_array[e_element_war_role_info_mission2_have_num];
		dp_msg.mission3_have_num = m_role_info.data_array[e_element_war_role_info_mission3_have_num];
		dp_msg.mission4_have_num = m_role_info.data_array[e_element_war_role_info_mission4_have_num];
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void c_element_war_role_info::buy_ticket()
	{
		m_role_info.data_array[e_element_war_role_info_ticket_num] += 1;
		m_role_info.data_array[e_element_war_role_info_can_buy_num] -= 1;
		m_role_info.data_array[e_element_war_role_info_have_buy_num] += 1;
		update_info_to_dp();
	}

	void c_element_war_role_info::check_player_mission(e_element_war_mission_type mission_type, int32 add_num)
	{
		for (int32 i = 0; i < 4; ++i)
		{
			if (get_data_array_info((e_element_war_role_info)(e_element_war_role_info_mission1_have_num + i)) < 0)
			{
				continue;
			}
			ElementLadderTaskTemplate* template_ptr = GET_TEMPLATE(ElementLadderTaskTemplate, m_role_info.data_array[(e_element_war_role_info)(e_element_war_role_info_mission1 + i)]);
			if (nullptr == template_ptr)
			{
				continue;
			}
			if (mission_type != template_ptr->TaskType)
			{
				continue;
			}
			int32 new_num = add_num + get_data_array_info((e_element_war_role_info)(e_element_war_role_info_mission1_have_num + i));
			set_data_array_info((e_element_war_role_info)(e_element_war_role_info_mission1_have_num + i), new_num);
		}
	}

	void c_element_war_role_info::game_end(bool is_win)
	{
		m_role_info.data_array[e_element_war_role_info_total_join_num]++;
		m_role_info.data_array[e_element_war_role_info_cur_season_join_num]++;
		if (is_win)
		{
			m_role_info.data_array[e_element_war_role_info_total_win_num]++;
			m_role_info.data_array[e_element_war_role_info_last_win_num]++;
		}
		else
		{
			m_role_info.data_array[e_element_war_role_info_last_win_num] = 0;
		}
	}

	void c_element_war_role_info::send_last_season_score(int32 server_id)
	{
		if (m_role_info.data_array[e_element_war_role_info_last_score] == -1)
		{
			return;
		}
		ws2ws_element_ladder_send_last_score msg;
		msg.role_guid = m_role_info.role_guid;
		msg.last_score = m_role_info.data_array[e_element_war_role_info_last_score];
		if (server_id)
		{
			cross::send_msg_to_ws(m_role_info.role_guid, server_id, e_msgindex_ws2ws_element_ladder_send_last_score, &msg, sizeof(msg));
		}
		else
		{
			cross::send_msg_to_ws(m_role_info.role_guid, m_role_info.data_array[e_element_war_role_info_server_id], e_msgindex_ws2ws_element_ladder_send_last_score, &msg, sizeof(msg));
		}		
	}

	void c_element_war_role_info::send_math_tip()
	{
		ws2ws_element_war_send_math_tip msg;
		msg.role_guid = m_role_info.role_guid;
		cross::send_msg_to_ws(m_role_info.role_guid, m_role_info.data_array[e_element_war_role_info_server_id], e_msgindex_ws2ws_element_war_send_math_tip, &msg, sizeof(msg));
		set_is_send_tip(false);
	}

	element_war_ws_mgr::element_war_ws_mgr()
	{
		clear_data();
	}

	element_war_ws_mgr::~element_war_ws_mgr()
	{
	}

	void element_war_ws_mgr::clear_data()
	{
		m_transfer_list.clear();
		m_notify_map.clear();
		m_config_level_limit = 0;
		m_break_line_login_list.clear();
		m_all_player_info.clear();
		m_all_sign_up_map.clear();
		m_config_match_time_limit = 0;
		m_config_transfer_time = 0;
		m_is_load_all = false;
		m_config_daily_ticket_num = 0;
		m_get_reward_need_num = 0;
		m_is_in_activity_time = false;
		m_config_match_server_id = 0;
		m_cur_season = 0;
		m_send_notify_flag = 0;
		m_activity_timer = 0;
		m_sec_timer = 0;
		m_refresh_flag = false;
		m_activity_time_vec.clear();
		m_transfer_info_map.clear();
		m_role_server_info_map.clear();
	}

	void element_war_ws_mgr::init_manager(bool need_load_dp)
	{
		m_activity_timer = utility::get_tick_count() / second_tick_time;
 		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == config_ptr)
		{
			return;
		}

		init_activity_season_list(config_ptr->ParamStringArr1);	//赛季时间信息
		m_config_match_server_id = GATECONFIG->gate_id;

		m_config_daily_ticket_num = config_ptr->ParamInt1;		//每次免费次数
		m_get_reward_need_num = config_ptr->ParamInt2;			//领取赛季奖励需要次数
		m_config_level_limit = config_ptr->LimitLevel;			//等级限制
		m_config_match_time_limit = config_ptr->ParamInt4 * second_tick_time;	//匹配最大时长
		m_config_transfer_time = config_ptr->ParamInt5 * second_tick_time;		//传送延时时间

		if (need_load_dp)
		{
			load_info_from_db();
		}

	}

	void element_war_ws_mgr::tick(int64 time_new)
	{
		if (!m_is_load_all && is_match_server())
		{
			return;
		}
		int64 time_new_sec = time_new / second_tick_time;
		for (auto iter = m_activity_time_vec.begin(); iter != m_activity_time_vec.end(); ++iter)
		{
			if (iter->begin_time <= time_new_sec && time_new_sec < iter->end_time)
			{
				m_is_in_activity_time = true;
				m_cur_season = iter->season;
			}
			if (iter->begin_time > m_activity_timer && iter->begin_time <= time_new_sec)
			{
				begin_one_season(iter->season);
			}
			if (iter->end_time > m_activity_timer  && iter->end_time <= time_new_sec)
			{
				m_is_in_activity_time = false;
				end_one_season();
			}
		}
		m_activity_timer = time_new_sec;

		if (m_refresh_flag)
		{
			refresh_daily_ticket_info();
		}

		if (m_is_in_activity_time == false)
		{
			return;
		}
			
		if (is_match_server())
		{
			match(time_new);
		}
		else
		{
			if (time_new > m_sec_timer)
			{
				m_sec_timer = time_new + second_tick_time;
				transfer_player(time_new);
				check_break_line_player();
			}
			if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
			{
				if (world_server::getInstance().get_activity_sec_left(e_activity_type_element_war, e_activity_time_get_ready) > 0
					|| world_server::getInstance().get_activity_sec_left(e_activity_type_element_war, e_activity_time_get_notice) > 0)
				{
					m_send_notify_flag = 1;
				}
				else if (world_server::getInstance().get_activity_sec_left(e_activity_type_element_war, e_activity_time_get_gaming) > 0)
				{
					if (m_send_notify_flag == 1)
					{
						send_activity_begin_notice();
						m_send_notify_flag = 0;
					}
				}
			}
		}
	}

	bool element_war_ws_mgr::is_in_game_time()
	{
		return m_is_in_activity_time && world_server::getInstance().get_activity_sec_left(e_activity_type_element_war, e_activity_time_get_gaming) > 0;;
	}

	bool element_war_ws_mgr::is_in_activity_time()
	{
		return m_is_in_activity_time && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war);
	}

	bool element_war_ws_mgr::is_match_server()
	{
		return get_match_server_id() == world_server::getInstance().get_server_id();
	}

	int32 element_war_ws_mgr::get_match_server_id()
	{
		return m_config_match_server_id;
	}

	void element_war_ws_mgr::init_activity_season_list(std::vector<std::string> time_list)
	{
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		m_activity_time_vec.clear();
		for (auto iter = time_list.begin(); iter != time_list.end(); ++iter)
		{
			s_element_war_activity_time item;
			int64 beginTime = 0;
			int64 endTime = 0;
			int32 region_type = -1;
			sscanf_s((*iter).c_str(), "%d-%d-%I64d-%I64d", &region_type, &item.season, &beginTime, &endTime);
			if (region_type != region_template_ptr->RegionCode)
			{
				continue;
			}
			item.begin_time = transfer_date_to_stamp(beginTime);
			item.end_time = transfer_date_to_stamp(endTime);
			m_activity_time_vec.push_back(item);
		}
	}

	int64 element_war_ws_mgr::transfer_date_to_stamp(int64 date)
	{
		tm base_time;
		base_time.tm_year = date / 10000000000 - 1900;
		base_time.tm_mon = date / 100000000 % 100 - 1;
		base_time.tm_mday = date / 1000000 % 100;
		base_time.tm_hour = date / 10000 % 100;
		base_time.tm_min = date / 100 % 100;
		base_time.tm_sec = date % 100;
		return mktime(&base_time);
	}

	int32 element_war_ws_mgr::get_segment_value(const c_element_war_role_info & info)
	{
		ElementLadderTemplate* element_template_ptr = template_manager::get_instance().get_template_by_element_score(info.get_data_array_info(e_element_war_role_info_score_num));
		if (nullptr != element_template_ptr)
		{
			return element_template_ptr->attribute_id;
		}
		return 0;
	}

	int32 element_war_ws_mgr::get_can_buy_num(int32 vip_level)
	{
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
		if (vip_template_ptr == nullptr)
		{
			return 0;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return 0;
		}
		for (int32 i = 0; (i + e_element_war_buy_limit_type_max) < vip_template_ptr->ElementWarlimit.size(); i += e_element_war_buy_limit_type_max)
		{
			if (vip_template_ptr->ElementWarlimit[i + e_element_war_buy_limit_type_region] == region_template_ptr->RegionCode)
			{
				return vip_template_ptr->ElementWarlimit[i + e_element_war_buy_limit_type_num];
			}
		}
		return 0;
	}

	void element_war_ws_mgr::begin_one_season(int32 cur_season_num)
	{
		m_transfer_list.clear();
		m_notify_map.clear();
		m_transfer_info_map.clear();
		m_role_server_info_map.clear();
		m_all_sign_up_map.clear();
		m_break_line_login_list.clear();
		

		if (is_match_server() == false && world_server::getInstance().is_sky_island_server() == false && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			std::vector<std::string> title_params;
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_element_war_season_begin_title));
			title_params.push_back(init_unit::change_i32_to_string(cur_season_num));
			std::string titletext = init_unit::implode(title_params);
			std::vector<std::string> content_params;
			content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_element_war_season_begin_context));
			content_params.push_back(init_unit::change_i32_to_string(cur_season_num));
			std::string contenttext = init_unit::implode(content_params);

			s_item_info final_item_array[max_item_per_mail];
			s_mail_info final_mail_info;
			final_mail_info.mail_guid = guid_gen::make_guid();
			final_mail_info.set_mail_title(titletext);
			final_mail_info.set_mail_content_text(contenttext);
			final_mail_info.data_ary[EMailInfo_IsNeedDelete] = time_helper::get_cur_time_new().second + day_time_second * 15;//15天删除
			final_mail_info.data_ary[EMailInfo_SendTime] = time_helper::get_cur_time_new().second;
			event_ws_mgr::get_instance().add_globel_mail(final_mail_info, final_item_array, 0);
		}
	}

	void element_war_ws_mgr::end_one_season()
	{
		if (is_match_server() == false)
		{
			ranking_mgr_ws::clear_data_by_ranking_type(e_RankingIndex_element_rank_list);
			return;
		}

		ranking_mgr_ws::replace_element_war_ranking();

		for (auto iter = m_all_player_info.begin(); iter != m_all_player_info.end(); ++iter)
		{
			auto & role_info = iter->second;
			int32 old_score = role_info.get_data_array_info(e_element_war_role_info_score_num);
			ElementLadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_element_score(old_score);
			if (nullptr == ladder_template_ptr || old_score == 0)
			{
				role_info.set_data_array_info(e_element_war_role_info_score_num, 0);
				role_info.set_data_array_info(e_element_war_role_info_history_score, 0);
				role_info.set_data_array_info(e_element_war_role_info_last_season, m_cur_season);
				role_info.set_data_array_info(e_element_war_role_info_last_score, 0);
				role_info.set_data_array_info(e_element_war_role_info_season_reward, 0);
				role_info.set_data_array_info(e_element_war_role_info_last_season_join_num, role_info.get_data_array_info(e_element_war_role_info_cur_season_join_num));
				role_info.set_data_array_info(e_element_war_role_info_cur_season_join_num, 0);
				continue;
			}
			role_info.set_data_array_info(e_element_war_role_info_score_num, ladder_template_ptr->ReservedScore);
			role_info.set_data_array_info(e_element_war_role_info_last_season, m_cur_season);
			role_info.set_data_array_info(e_element_war_role_info_history_score, 0);
			role_info.set_data_array_info(e_element_war_role_info_last_score, old_score);
			if (role_info.get_data_array_info(e_element_war_role_info_cur_season_join_num) >= m_get_reward_need_num)
			{
				role_info.set_data_array_info(e_element_war_role_info_season_reward, m_cur_season);
			}
			role_info.set_data_array_info(e_element_war_role_info_last_season_join_num, role_info.get_data_array_info(e_element_war_role_info_cur_season_join_num));
			role_info.set_data_array_info(e_element_war_role_info_cur_season_join_num, 0);
			role_info.refresh_season_info();
		}

		ws2ws_element_war_notify_season_reward notify_msg;
		cross::send_msg_to_ws(0, -1, e_msgindex_ws2ws_element_war_notify_get_season_reward, &notify_msg, sizeof(notify_msg));
	}

	void element_war_ws_mgr::refresh_daily_ticket_info()
	{
		if (is_match_server() == false)
		{
			return;
		}
		if (m_is_load_all == false)
		{
			m_refresh_flag = true;
			return;
		}
		m_refresh_flag = false;

		//ws2dp_element_war_clear_mission_info clear_msg;
		//ws_client::getInstance().send_to_dp(&clear_msg, sizeof(clear_msg));

		std::vector<int32> mission_id = get_mission_list();

		for (auto iter = m_all_player_info.begin(); iter != m_all_player_info.end(); ++iter)
		{
			auto & role_info = iter->second;
			role_info.set_data_array_info(e_element_war_role_info_ticket_num, m_config_daily_ticket_num);
			role_info.set_data_array_info(e_element_war_role_info_have_buy_num, 0);
			role_info.set_data_array_info(e_element_war_role_info_mission1_have_num, 0);
			role_info.set_data_array_info(e_element_war_role_info_mission2_have_num, 0);
			role_info.set_data_array_info(e_element_war_role_info_mission3_have_num, 0);
			role_info.set_data_array_info(e_element_war_role_info_mission4_have_num, 0);

			for (int32 i = 0; i < mission_id.size(); ++i)
			{
				role_info.set_data_array_info(e_element_war_role_info(e_element_war_role_info_mission1 + i), mission_id[i]);
			}
	
			role_info.save_mission_info();
		}

		ws2dp_element_war_refresh_ticket_info dp_msg;
		dp_msg.ticket_num = m_config_daily_ticket_num;
		dp_msg.have_buy_num = 0;
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void element_war_ws_mgr::send_activity_begin_notice()
	{
		int32 notice_id = 93000415;
		std::string notice_str = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
	}

	bool element_war_ws_mgr::get_transfer_game_info(const guid_64 & role_guid, const int32 & server_id, s_game_info & game_info)
	{
		auto iter = m_transfer_info_map.find(role_guid.server_64);
		if (iter != m_transfer_info_map.end())
		{
			if (iter->second.server_id == server_id)
			{
				game_info = iter->second;
				m_transfer_info_map.erase(iter);
				return true;
			}
			return false;
		}
		//回到原区
		auto it = m_role_server_info_map.find(role_guid.server_64);
		if (it != m_role_server_info_map.end())
		{
			if (it->second.server_id == server_id)
			{
				game_info = it->second;
				m_role_server_info_map.erase(it);
				return true;
			}
			return false;
		}
		return false;
	}

	void element_war_ws_mgr::gm_send_add_ticket(guid_64 role_guid)
	{
		if (gate_proxy::getInstance().is_gate_run() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			cs2ws_element_war_gm_send_add_ticket msg;
			msg.role_guid = role_guid;
			cross::send_msg_to_ws(role_guid, get_match_server_id(), e_msgindex_cs2ws_element_war_gm_send_add_ticket, &msg, sizeof(msg));
		}
	}

	void element_war_ws_mgr::gm_add_ticket(guid_64 role_guid)
	{
		auto role_it = m_all_player_info.find(role_guid);
		if (role_it == m_all_player_info.end())
		{
			return;
		}
		auto& first_role_it = role_it->second;
		first_role_it.set_data_array_info(e_element_war_role_info_ticket_num, m_config_daily_ticket_num);
		first_role_it.set_data_array_info(e_element_war_role_info_have_buy_num, 0);
	}

	void element_war_ws_mgr::match(int64 time_new)
	{
		//遍历循环列表
		for (auto iter = m_all_sign_up_map.begin(); iter != m_all_sign_up_map.end(); ++iter)
		{
			//当前段位列表
			auto & cur_list = iter->second;
			for (auto role_iter = cur_list.begin(); role_iter != cur_list.end();)
			{
				//判断当前角色数据是否有效
				if (*role_iter == nullptr)
				{
					cur_list.erase(role_iter++);
					continue;
				}
				if ((time_new - (*role_iter)->get_sign_up_time()) >= m_config_match_time_limit)
				{
					(*role_iter)->set_pk_state(e_element_war_role_no_sign_up);
					send_sign_up_result((*role_iter)->get_role_guid(), e_element_war_sign_up_time_out, (*role_iter)->get_data_array_info(e_element_war_role_info_server_id));
					cur_list.erase(role_iter++);
				}
				else if (((time_new - (*role_iter)->get_sign_up_time()) >= (m_config_match_time_limit - (ElEMENT_WAR_SHOW_TIP_TIME * second_tick_time))) && (*role_iter)->get_is_send_tip())
				{
					(*role_iter)->send_math_tip();
				}
				else
				{
					role_iter++;
					continue;		//break;
				}
			}
			if (cur_list.size() == 0)
			{
				continue;
			}
			//段位列表中人数满足逻辑
		/*	if (cur_list.size() >= ELEMENT_WAR_MATCH_NUM)
			{
				bool complete = false;
				std::vector<guid_64> first_role_guid_array;
				std::vector<guid_64> second_role_guid_array;
				guid_64 first_time_guid;
				guid_64 second_time_guid;
				first_time_guid.clear_data();
				second_time_guid.clear_data();
				auto cur_role_ptr = cur_list.begin();

				//先遍历组队玩家
				while (cur_role_ptr != cur_list.end())
				{
					guid_64 role_team_guid = (*cur_role_ptr)->get_team_guid();
					if (role_team_guid.is_valid())
					{
						if (!first_time_guid.is_valid())
						{
							first_time_guid = role_team_guid;
							first_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (first_time_guid == role_team_guid)
						{
							first_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (!second_time_guid.is_valid())
						{
							second_time_guid = role_team_guid;
							second_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (first_time_guid == role_team_guid)
						{
							second_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
					}
					cur_role_ptr++;
				}
				cur_role_ptr = cur_list.begin();
				//再遍历是非组队玩家
				while (cur_role_ptr != cur_list.end())
				{
					if (first_role_guid_array.size() == ELEMENT_WAR_PLAY_NUM && second_role_guid_array.size() == ELEMENT_WAR_PLAY_NUM)
					{
						break;
					}
					guid_64 role_team_guid = (*cur_role_ptr)->get_team_guid();
					if (!role_team_guid.is_valid())
					{
						if (first_role_guid_array.size() < ELEMENT_WAR_PLAY_NUM && first_role_guid_array.size() <= second_role_guid_array.size())
						{
							first_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (second_role_guid_array.size() < ELEMENT_WAR_PLAY_NUM)
						{
							second_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
					}
					cur_role_ptr++;
				}
				//检查人数是否满足
				if (first_role_guid_array.size() == ELEMENT_WAR_PLAY_NUM && second_role_guid_array.size() == ELEMENT_WAR_PLAY_NUM)
				{
					complete = true;
					match_success(first_role_guid_array, second_role_guid_array);
					erase_match_by_list(first_role_guid_array);
					erase_match_by_list(second_role_guid_array);
				}

				if (complete)
				{
					cur_role_ptr = cur_list.begin();
					//再遍历是非组队玩家
					while (cur_role_ptr != cur_list.end())
					{
						bool is_erase = false;
						guid_64 success_guid = (*cur_role_ptr)->get_role_guid();
						for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
						{
							if (success_guid == first_role_guid_array[i] || success_guid == second_role_guid_array[i])
							{
								cur_list.erase(cur_role_ptr++);
								is_erase = true;
								break;
							}
						}
						if (!is_erase)
						{
							cur_role_ptr++;
						}
					}
					continue;
				}
			}*/

			ElementLadderTemplate* template_ptr = GET_TEMPLATE(ElementLadderTemplate, iter->first);
			if (nullptr == template_ptr)
			{
				continue;
			}

			auto first_role_iter = cur_list.begin();
			if (*first_role_iter == nullptr)
			{
				continue;
			}
			std::vector<guid_64> first_role_guid_array;
			std::vector<guid_64> second_role_guid_array;
			get_valid_role_list(**first_role_iter, time_new, first_role_guid_array, second_role_guid_array);

			if (first_role_guid_array.size() == ELEMENT_WAR_PLAY_NUM && second_role_guid_array.size() == ELEMENT_WAR_PLAY_NUM)
			{
				match_success(first_role_guid_array, second_role_guid_array);
				erase_match_by_list(first_role_guid_array);
				erase_match_by_list(second_role_guid_array);
			}
			else if ((time_new - (*first_role_iter)->get_sign_up_time()) >= (m_config_match_time_limit - (ElEMENT_WAR_MATH_TIME * second_tick_time)))
			{
				if (match_with_robot(*first_role_iter, first_role_guid_array, second_role_guid_array))
				{
					erase_match_by_list(first_role_guid_array);
					erase_match_by_list(second_role_guid_array);
				}
			}
		}

	}
	void element_war_ws_mgr::match_success(std::vector<guid_64> first_team_list, std::vector<guid_64> secord_team_list)
	{
		ws2ws_element_war_create_map msg;
		int32 target_server_id = 0;
		int32 player_level = 0;
		int32 player_num = 0;
		for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
		{
			if (i >= first_team_list.size())
			{
				continue;
			}
			auto role_it = m_all_player_info.find(first_team_list[i]);
			if (role_it == m_all_player_info.end())
			{
				continue;
			}
			auto& first_role_it = role_it->second;
			s_element_war_match_role_info role_info;
			if (target_server_id == 0)
			{
				target_server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
			}
			first_role_it.reduce_ticket();
			first_role_it.set_pk_state(e_element_war_role_join_war);
	
			memcpy(role_info.role_name, first_role_it.get_role_name(), max_name_size);
			memcpy(role_info.server_name, first_role_it.get_server_name(), max_name_size);
			role_info.role_guid = first_role_it.get_role_guid();
			role_info.server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
			role_info.template_id = first_role_it.get_data_array_info(e_element_war_role_info_template_id);
			role_info.score_num = first_role_it.get_data_array_info(e_element_war_role_info_score_num);
			role_info.total_join_num = first_role_it.get_data_array_info(e_element_war_role_info_total_join_num);
			role_info.total_win_num = first_role_it.get_data_array_info(e_element_war_role_info_total_win_num);
			role_info.last_win_num = first_role_it.get_data_array_info(e_element_war_role_info_last_win_num);
			role_info.vip_level = first_role_it.get_data_array_info(e_element_war_role_info_vip_level);
			memcpy(role_info.role_server_out_ip, first_role_it.get_server_out_ip().c_str(), max_ip_address_length);
			role_info.role_server_out_port = first_role_it.get_server_out_port();
			msg.match_info.first_role_info[i] = role_info;
			player_level += first_role_it.get_data_array_info(e_element_war_role_info_role_level);
			player_num++;
		}

		for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
		{
			if (i >= secord_team_list.size())
			{
				continue;
			}
			auto role_it = m_all_player_info.find(secord_team_list[i]);
			if (role_it == m_all_player_info.end())
			{
				continue;
			}
			auto& first_role_it = role_it->second;
			s_element_war_match_role_info role_info;
			if (target_server_id == 0)
			{
				target_server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
			}
			first_role_it.reduce_ticket();
			first_role_it.set_pk_state(e_element_war_role_join_war);

			memcpy(role_info.role_name, first_role_it.get_role_name(), max_name_size);
			memcpy(role_info.server_name, first_role_it.get_server_name(), max_name_size);
			role_info.role_guid = first_role_it.get_role_guid();
			role_info.server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
			role_info.template_id = first_role_it.get_data_array_info(e_element_war_role_info_template_id);
			role_info.role_level = first_role_it.get_data_array_info(e_element_war_role_info_role_level);
			role_info.score_num = first_role_it.get_data_array_info(e_element_war_role_info_score_num);
			role_info.total_join_num = first_role_it.get_data_array_info(e_element_war_role_info_total_join_num);
			role_info.total_win_num = first_role_it.get_data_array_info(e_element_war_role_info_total_win_num);
			role_info.last_win_num = first_role_it.get_data_array_info(e_element_war_role_info_last_win_num);
			role_info.vip_level = first_role_it.get_data_array_info(e_element_war_role_info_vip_level);
			memcpy(role_info.role_server_out_ip, first_role_it.get_server_out_ip().c_str(), max_ip_address_length);
			role_info.role_server_out_port = first_role_it.get_server_out_port();
			msg.match_info.second_role_info[i] = role_info;
			player_level += first_role_it.get_data_array_info(e_element_war_role_info_role_level);
			player_num++;
		}
		msg.match_info.map_level = ((player_level + player_num - 1) / player_num);
		cross::send_msg_to_ws(guid_64(), target_server_id, e_msgindex_ws2ws_element_war_req_create_map, &msg, sizeof(msg));
	}

	void element_war_ws_mgr::get_expend_range(const c_element_war_role_info & info, int64 time_new, int32 expend_time, int32 & upper_limit, int32 & lower_limit)
	{
		int32 cur_segment_value = info.get_segment_value();
		int32 expand_value = ((time_new - info.get_sign_up_time()) / expend_time) * 2;
		int32 cur_last_win = info.get_data_array_info(e_element_war_role_info_last_win_num);
		if (cur_last_win > 0)
		{
			upper_limit = cur_segment_value + (expand_value + 1) / 2;
			lower_limit = cur_segment_value - expand_value / 2;
		}
		else
		{
			upper_limit = cur_segment_value + expand_value / 2;
			lower_limit = cur_segment_value - (expand_value + 1) / 2;
		}
	}

	e_element_war_sign_up_ret element_war_ws_mgr::single_sign_up(guid_64 role_guid)
	{
		auto role_it = m_all_player_info.find(role_guid.server_64);
		if (role_it == m_all_player_info.end())
		{
			return e_element_war_sign_up_failed;
		}
		auto & role_info = role_it->second;
		role_info.check_reset_pk_state();
		//检查排队状态
		if (role_info.get_pk_state() != e_element_war_role_no_sign_up)
		{
			return e_element_war_sign_up_already;
		}
		//检查剩余次数
		if (role_info.get_data_array_info(e_element_war_role_info_ticket_num) <= 0)
		{
			return e_element_war_sign_up_no_ticket;
		}
		//检查段位
		int32 segment_value = get_segment_value(role_info);
		if (segment_value == 0)
		{
			return e_element_war_sign_up_failed;
		}
		//设置报名状态	
		role_info.set_pk_state(e_element_war_role_sign_up);
		//清空组队id
		role_info.clear_team_guid();
		//设置报名时间
		role_info.set_sign_up_time(utility::get_tick_count());
		role_info.set_is_send_tip(true);
		//设置匹配队伍
		role_info.set_segment_value(segment_value);
		auto sign_list_it = m_all_sign_up_map.find(segment_value);
		if (sign_list_it == m_all_sign_up_map.end())
		{
			element_war_sign_up_list new_sign_lsit;
			new_sign_lsit.push_back(&role_info);
			m_all_sign_up_map[segment_value] = new_sign_lsit;
		}
		else
		{
			element_war_sign_up_list& temp_sign_list = sign_list_it->second;
			temp_sign_list.push_back(&role_info);
		}
		return e_element_war_sign_up_success;
	}

	e_element_war_sign_up_ret element_war_ws_mgr::team_sign_up(std::vector<guid_64> role_guid_list, guid_64 team_guid, guid_64& error_guid)
	{
		if (role_guid_list.size() > ELEMENT_WAR_PLAY_NUM)
		{
			return e_element_war_sign_up_failed;
		}
		int32 max_segment_value = 0;
		for (int32 i = 0; i < role_guid_list.size(); ++i)
		{
			auto role_it = m_all_player_info.find(role_guid_list[i].server_64);
			error_guid = role_guid_list[i].server_64;
			if (role_it == m_all_player_info.end())
			{
				return e_element_war_sign_up_failed;
			}
			auto & role_info = role_it->second;
			role_info.check_reset_pk_state();
			//检查排队状态
			if (role_info.get_pk_state() != e_element_war_role_no_sign_up)
			{
				return e_element_war_sign_up_already;
			}
			//检查剩余次数
			if (role_info.get_data_array_info(e_element_war_role_info_ticket_num) <= 0)
			{
				return e_element_war_sign_up_no_ticket;
			}
			//检查段位
			int32 segment_value = get_segment_value(role_info);
			if (segment_value == 0)
			{
				return e_element_war_sign_up_failed;
			}
			if (max_segment_value < segment_value)
			{
				max_segment_value = segment_value;
			}
			//清除组队id
			role_info.clear_team_guid();
		}

		for (int32 i = 0; i < role_guid_list.size(); ++i)
		{
			auto role_it = m_all_player_info.find(role_guid_list[i].server_64);
			error_guid = role_guid_list[i].server_64;
			if (role_it == m_all_player_info.end())
			{
				return e_element_war_sign_up_failed;
			}
			auto & role_info = role_it->second;
			//设置报名状态	
			role_info.set_pk_state(e_element_war_role_sign_up);
			//设置队伍GUid
			role_info.set_team_guid(team_guid);
			//设置队伍列表
			role_info.set_segment_value(max_segment_value);
			//设置报名时间
			role_info.set_sign_up_time(utility::get_tick_count());
			role_info.set_is_send_tip(true);
			auto sign_list_it = m_all_sign_up_map.find(max_segment_value);
			if (sign_list_it == m_all_sign_up_map.end())
			{
				element_war_sign_up_list new_sign_lsit;
				new_sign_lsit.push_back(&role_info);
				m_all_sign_up_map[max_segment_value] = new_sign_lsit;
			}
			else
			{
				element_war_sign_up_list& temp_sign_list = sign_list_it->second;
				temp_sign_list.push_back(&role_info);
			}
		}
		return e_element_war_sign_up_success;
	}

	void element_war_ws_mgr::sign_up_leave(guid_64  role_guid)
	{
		auto role_it = m_all_player_info.find(role_guid.server_64);
		if (role_it == m_all_player_info.end())
		{
			return;
		}
		auto& role_info = role_it->second;
		if (role_info.get_pk_state() == e_element_war_role_no_sign_up)
		{
			return;
		}
		game_proto_element_war_send_close_sign_up_end msg;
		msg.set_ret_index(e_element_war_sign_up_leave_success);
		msg.set_guid_a(role_guid.A);
		msg.set_guid_b(role_guid.B);

		bool is_send = false;
		int32 segment_value = role_info.get_segment_value();
		for (auto iter = m_all_sign_up_map.begin(); iter != m_all_sign_up_map.end(); ++iter)
		{
			element_war_sign_up_list& temp_sign_list = iter->second;
			for (auto it = temp_sign_list.begin(); it != temp_sign_list.end();)
			{
				if ((*it)->get_role_guid() == role_guid || (*it)->get_team_guid() == role_info.get_team_guid())
				{
					is_send = true;
					cross::send_msg_to_ws((*it)->get_role_guid(), (*it)->get_data_array_info(e_element_war_role_info_server_id), e_mgsindex_s2c_element_war_send_close_sign_up_end, &msg);
					(*it)->set_pk_state(e_element_war_role_no_sign_up);
					temp_sign_list.erase(it++);
				}
				else
				{
					++it;
				}
			}
		}
		if (false == is_send)
		{
			role_info.set_pk_state(e_element_war_role_no_sign_up);
			cross::send_msg_to_ws(role_info.get_role_guid(), role_info.get_data_array_info(e_element_war_role_info_server_id), e_mgsindex_s2c_element_war_send_close_sign_up_end, &msg);
		}
	}

	void element_war_ws_mgr::send_sign_up_result(const guid_64 & role_guid, e_element_war_sign_up_ret result_type, int32 server_id)
	{
		auto role_it = m_all_player_info.find(role_guid.server_64);
		if (role_it == m_all_player_info.end())
		{
			return;
		}
		auto& role_info = role_it->second;
		game_proto_element_war_send_sign_up_end msg;
		msg.set_ret_index(result_type);
		msg.set_guid_a(role_guid.A);
		msg.set_guid_b(role_guid.B);
		cross::send_msg_to_ws(role_guid, server_id, e_mgsindex_s2c_element_war_send_sign_up_end, &msg);
	}

	void element_war_ws_mgr::get_valid_role_list(const c_element_war_role_info & info, int64 time_new, std::vector<guid_64>& first_role_guid_array, std::vector<guid_64>& second_role_guid_array)
	{
		int32 cur_segment = info.get_segment_value();
		int32 upper_limit = 0;
		int32 lower_limit = 0;
		ElementLadderTemplate* template_ptr = GET_TEMPLATE(ElementLadderTemplate, cur_segment);
		if (nullptr == template_ptr)
		{
			return;
		}
		get_expend_range(info, time_new, template_ptr->ExpandTime * second_tick_time, upper_limit, lower_limit);
		guid_64 first_time_guid;
		guid_64 second_time_guid;
		first_time_guid.clear_data();
		second_time_guid.clear_data();
		first_role_guid_array.clear();
		second_role_guid_array.clear();
		if (info.get_team_guid().is_valid())
		{
			first_time_guid = info.get_team_guid();
		}
		for (int32 min_key = lower_limit; min_key <= upper_limit; ++min_key)
		{
			auto iter = m_all_sign_up_map.find(min_key);
			if (iter != m_all_sign_up_map.end())
			{
				auto & cur_list = iter->second;
				if (cur_list.size() == 0)
				{
					continue;
				}
				//检查匹配列表
				for (auto role_iter = cur_list.begin(); role_iter != cur_list.end();)
				{
					if (*role_iter == nullptr)
					{
						cur_list.erase(role_iter++);
					}
					else
					{
						role_iter++;
					}
				}
				auto cur_role_ptr = cur_list.begin();
				//遍历组队玩家
				while (cur_role_ptr != cur_list.end())
				{
					guid_64 role_team_guid = (*cur_role_ptr)->get_team_guid();
					if (role_team_guid.is_valid())
					{
						if (!first_time_guid.is_valid())
						{
							first_time_guid = role_team_guid;
							first_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (first_time_guid == role_team_guid)
						{
							first_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (!second_time_guid.is_valid())
						{
							second_time_guid = role_team_guid;
							second_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (second_time_guid == role_team_guid)
						{
							second_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
					}
					cur_role_ptr++;
				}
			}
		}
		for (int32 min_key = lower_limit; min_key <= upper_limit; ++min_key)
		{
			auto iter = m_all_sign_up_map.find(min_key);
			if (iter != m_all_sign_up_map.end())
			{
				auto & cur_list = iter->second;
				if (cur_list.size() == 0)
				{
					continue;
				}
				//检查匹配列表
				for (auto role_iter = cur_list.begin(); role_iter != cur_list.end();)
				{
					if (*role_iter == nullptr)
					{
						cur_list.erase(role_iter++);
					}
					else
					{
						role_iter++;
					}
				}
				auto cur_role_ptr = cur_list.begin();
				//再遍历是非组队玩家
				while (cur_role_ptr != cur_list.end())
				{
					guid_64 role_team_guid = (*cur_role_ptr)->get_team_guid();
					if (!role_team_guid.is_valid())
					{
						if (first_role_guid_array.size() < ELEMENT_WAR_PLAY_NUM && first_role_guid_array.size() <= second_role_guid_array.size())
						{
							first_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
						else if (second_role_guid_array.size() < ELEMENT_WAR_PLAY_NUM)
						{
							second_role_guid_array.push_back((*cur_role_ptr)->get_role_guid());
						}
					}
					cur_role_ptr++;
				}
			}
		}
	}

	void element_war_ws_mgr::erase_match_by_list(std::vector<guid_64>& role_guid_array)
	{
		for (int32 i = 0; i < role_guid_array.size(); ++i)
		{
			auto role_it = m_all_player_info.find(role_guid_array[i]);
			if (role_it == m_all_player_info.end())
			{
				continue;
			}
			auto& role_info = role_it->second;
			auto iter = m_all_sign_up_map.find(role_info.get_segment_value());
			if (iter != m_all_sign_up_map.end())
			{
				auto & cur_list = iter->second;
				//检查匹配列表
				for (auto role_iter = cur_list.begin(); role_iter != cur_list.end();)
				{
					if ((*role_iter)->get_role_guid() == role_guid_array[i])
					{
						cur_list.erase(role_iter++);
						break;
					}
					else
					{
						role_iter++;
					}
				}
			}
		}
	}

	void element_war_ws_mgr::add_element_war_info(const s_element_war_role_info & info)
	{
		m_all_player_info[info.role_guid].set_role_info(info);
	}

	void element_war_ws_mgr::req_element_war_info(const guid_64 & role_guid, const ws2ws_element_war_req_role_info * msg)
	{
		if (!m_is_load_all || nullptr == msg)
		{
			return;
		}	
		auto iter = m_all_player_info .find(role_guid);
		if (iter != m_all_player_info.end())
		{
			if (strcmp(iter->second.get_role_name(), msg->role_name) != 0
				|| strcmp(iter->second.get_server_name(), msg->server_name) != 0
				|| iter->second.get_data_array_info(e_element_war_role_info_server_id) != msg->server_id
				|| iter->second.get_data_array_info(e_element_war_role_info_template_id) != msg->template_id
				|| iter->second.get_data_array_info(e_element_war_role_info_vip_level) != msg->vip_level
				|| iter->second.get_data_array_info(e_element_war_role_info_role_level) != msg->role_level
				|| iter->second.get_gs_value() != msg->role_gs)
			{
				iter->second.set_role_name(msg->role_name);
				iter->second.set_server_name(msg->server_name);
				iter->second.set_gs_value(msg->role_gs);
				iter->second.set_data_array_info(e_element_war_role_info_server_id, msg->server_id);
				iter->second.set_data_array_info(e_element_war_role_info_template_id, msg->template_id);
				iter->second.set_data_array_info(e_element_war_role_info_vip_level, msg->vip_level);
				iter->second.set_data_array_info(e_element_war_role_info_can_buy_num, get_can_buy_num(msg->vip_level));
				iter->second.set_data_array_info(e_element_war_role_info_role_level, msg->role_level);
				ws2dp_element_war_update_role_info dp_msg;
				dp_msg.role_guid = role_guid;
				dp_msg.role_gs = msg->role_gs;
				dp_msg.vip_level = msg->vip_level;
				dp_msg.server_id = msg->server_id;
				dp_msg.template_id = msg->template_id;
				dp_msg.role_level = msg->role_level;
				memcpy(dp_msg.role_name, msg->role_name, max_name_size);
				memcpy(dp_msg.server_name, msg->server_name, max_name_size);
				ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
				if (strcmp(iter->second.get_role_name(), msg->role_name) != 0)
				{
					ranking_mgr_ws::change_player_name_func(role_guid, msg->role_name);
				}
			}
			send_element_war_info(iter->second, msg->is_online);
			if (iter->second.get_data_array_info(e_element_war_role_info_season_reward) > 0)
			{
				send_season_reward_to_player(role_guid);
			}
			send_season_upreward_to_player(role_guid);		
		}
		else
		{
			c_element_war_role_info new_role_info;
			new_role_info.set_role_guid(role_guid);
			new_role_info.set_role_name(msg->role_name);
			new_role_info.set_server_name(msg->server_name);
			new_role_info.set_gs_value(msg->role_gs);
			new_role_info.set_data_array_info(e_element_war_role_info_template_id, msg->template_id);
			new_role_info.set_data_array_info(e_element_war_role_info_vip_level, msg->vip_level);
			new_role_info.set_data_array_info(e_element_war_role_info_server_id, msg->server_id);
			new_role_info.set_data_array_info(e_element_war_role_info_role_level, msg->role_level);
			new_role_info.set_data_array_info(e_element_war_role_info_ticket_num, m_config_daily_ticket_num);
			new_role_info.set_data_array_info(e_element_war_role_info_can_buy_num, get_can_buy_num(msg->vip_level));
			new_role_info.set_data_array_info(e_element_war_role_info_have_buy_num, 0);
			new_role_info.set_data_array_info(e_element_war_role_info_last_score, -1);
			bool is_set = false;
			for (auto iter = m_all_player_info.begin(); iter != m_all_player_info.end(); ++iter)
			{
				auto & role_info = iter->second;
				new_role_info.set_data_array_info(e_element_war_role_info_mission1, role_info.get_data_array_info(e_element_war_role_info_mission1));
				new_role_info.set_data_array_info(e_element_war_role_info_mission2, role_info.get_data_array_info(e_element_war_role_info_mission2));
				new_role_info.set_data_array_info(e_element_war_role_info_mission3, role_info.get_data_array_info(e_element_war_role_info_mission3));
				new_role_info.set_data_array_info(e_element_war_role_info_mission4, role_info.get_data_array_info(e_element_war_role_info_mission4));
				is_set = true;
				break;
			}
			if (is_set == false)
			{
				std::vector<int32> mission_id = get_mission_list();
				for (int32 i = 0; i < mission_id.size(); ++i)
				{
					new_role_info.set_data_array_info(e_element_war_role_info(e_element_war_role_info_mission1 + i), mission_id[i]);
				}
			}
			m_all_player_info[role_guid.server_64] = new_role_info;

			ws2dp_element_war_add_role_info dp_msg;
			memcpy(&dp_msg.role_info, &new_role_info.get_role_info(), sizeof(s_element_war_role_info));
			ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));

			send_element_war_info(new_role_info, msg->is_online);
		}
	}

	void element_war_ws_mgr::load_info_from_db()
	{
		if (is_match_server() == false)
		{
			return;
		}
		ws2dp_element_war_load_info dp_msg;
		ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
	}

	void element_war_ws_mgr::set_is_load_all(bool is_load)
	{
		m_is_load_all = is_load;
	}

	void element_war_ws_mgr::set_role_server_info(const guid_64 & role_guid, const xstring & server_ip, int32 server_port)
	{
		auto role_it = m_all_player_info.find(role_guid.server_64);
		if (role_it != m_all_player_info.end())
		{
			role_it->second.set_server_info(server_ip, server_port);
		}
	}

	void element_war_ws_mgr::reset_role_state(const guid_64 & role_guid)
	{
		if (role_guid.is_valid() == false)
		{
			return;
		}
		auto iter = m_all_player_info.find(role_guid);
		if (iter != m_all_player_info.end())
		{
			iter->second.set_pk_state(e_element_war_role_no_sign_up);
			iter->second.add_ticket_num();
		}
	}

	void element_war_ws_mgr::send_element_war_info(c_element_war_role_info & info, bool is_online)
	{
		game_proto_element_war_role_info msg;
		msg.set_cross_result(1);
		int32 ranking_level = -1;
		int64 ranking_value = 0;
		ranking_mgr_ws::get_player_rank_level(e_RankingIndex_element_war_last_season, info.get_role_guid(), ranking_level, ranking_value);
		if (ranking_level < 0)
		{
			ranking_level = 0;
		}
		msg.set_last_score(ranking_level);
		msg.set_is_online(is_online);
		for (int32 i = 0; i < e_element_war_role_info_max; ++i)
		{
			msg.add_info_data(info.get_data_array_info(e_element_war_role_info(i)));
		}
		bool is_sign = (info.get_pk_state() == e_element_war_role_sign_up);
		msg.set_max_index(e_element_war_role_info_max);
		msg.set_is_sign(is_sign);
		cross::send_msg_to_ws(info.get_role_guid(), info.get_data_array_info(e_element_war_role_info_server_id), e_mgsindex_s2c_element_war_send_role_info, &msg);
	}

	void element_war_ws_mgr::send_season_reward_to_player(const guid_64 & role_guid)
	{
		auto role_iter = m_all_player_info.find(role_guid);
		if (role_iter == m_all_player_info.end())
			return;

		int32 old_score = role_iter->second.get_data_array_info(e_element_war_role_info_last_score);
		int32 server_id = role_iter->second.get_data_array_info(e_element_war_role_info_server_id);

		send_season_ranking_reward_to_player(role_guid, server_id);

		std::vector<std::string> content_params;
		const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_element_war_season_reward_title);
		const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_element_war_season_reward_context);
		if (title == nullptr || content == nullptr)
		{
			return;
		}
		ElementLadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_element_score(old_score, role_iter->second.get_data_array_info(e_element_war_role_info_season_reward));
		if (ladder_template_ptr)
		{
			content_params.clear();
			content_params.push_back(std::string(content));
			content_params.push_back(init_unit::change_i32_to_string(ladder_template_ptr->RankName));
			std::string contenttext = init_unit::implode(content_params);

			std::vector<int32>& reward_vec = ladder_template_ptr->SeasonReward;
			int32 vec_size = reward_vec.size();
			if (vec_size && vec_size % 2 == 0)
			{
				int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
				std::vector<s_item_template_info> drop_item_list;
				drop_item_list.clear();

				for (int32 i = 0; i < vec_size / 2; ++i)
				{
					drop_item_list.push_back(s_item_template_info(reward_vec[i * 2], reward_vec[i * 2 + 1], 1));
				}
				mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, title, contenttext, money_type_1, money_num_1, money_type_2, money_num_2);
				role_iter->second.set_data_array_info(e_element_war_role_info_season_reward, 0);
				role_iter->second.refresh_season_info();
			}
		}
		CONSOLE_INFO("element_war_ws_mgr::send_season_reward_to_player RoleGuid:{} ServerId:{} OldScore:{}",role_guid.server_64,role_iter->second.get_data_array_info(e_element_war_role_info_server_id),old_score);
	}

	void element_war_ws_mgr::send_season_upreward_to_player(const guid_64 & role_guid)
	{
		auto role_iter = m_all_player_info.find(role_guid);
		if (role_iter == m_all_player_info.end())
		{
			return;
		}
		
		int32 reserved_score = role_iter->second.get_data_array_info(e_element_war_role_info_score_num);
		int32 history_score = role_iter->second.get_data_array_info(e_element_war_role_info_history_score);
		int32 server_id = role_iter->second.get_data_array_info(e_element_war_role_info_server_id);

		if (history_score >= reserved_score)
		{
			return;
		}
		if (!m_is_in_activity_time)
		{
			return;
		}
		if (GATECONFIG->gate_id != world_server::getInstance().get_server_id())
		{
			return;
		}

		//4.当前所达到段位所有的奖励
		std::vector<ElementLadderTemplate*> ladder_template_ptr_arr = template_manager::get_instance().get_template_array_by_element_score(reserved_score);
		for (int32 i = 0; i < ladder_template_ptr_arr.size(); i++)
		{
			if (ladder_template_ptr_arr[i] == nullptr)
			{
				return;
			}
			const auto & reward_vec = ladder_template_ptr_arr[i]->UpgradeReward;
			int32 vec_size = reward_vec.size();
			if (vec_size && vec_size % 2 == 0)
			{
				const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_element_war_upgrade_title);
				const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_element_war_upgrade_context);
				if (title != nullptr && content != nullptr)
				{
					std::vector<std::string> content_params;
					content_params.clear();
					content_params.push_back(std::string(content));
					content_params.push_back(init_unit::change_i32_to_string(ladder_template_ptr_arr[i]->RankName));
					std::string contenttext = init_unit::implode(content_params);

					int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
					std::vector<s_item_template_info> drop_item_list;
					drop_item_list.clear();

					for (int32 i = 0; i < vec_size / 2; ++i)
					{
						drop_item_list.push_back(s_item_template_info(reward_vec[i * 2], reward_vec[i * 2 + 1], 1));
					}
					mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, title, contenttext, money_type_1, money_num_1, money_type_2, money_num_2);
				}
			}
		}
		role_iter->second.set_data_array_info(e_element_war_role_info_history_score, reserved_score);
		role_iter->second.refresh_season_info();
	}

	std::vector<int32> element_war_ws_mgr::get_mission_list()
	{
		std::vector<int32> mission_list;
		std::vector<int32> mission_type_list;

		int32 while_num = 0;

		//防止任务类型不足死循环
		while (mission_list.size() < 4 && while_num < 100)
		{
			while_num++;
			int32 mission_type = random_gen::get_random(e_element_war_mission_type_join_map_num, e_element_war_mission_type_kill_first_score_num);
			int32 is_find = false;
			for (int32 i = 0; i < mission_type_list.size(); ++i)
			{
				if (mission_type_list[i] == mission_type)
				{
					is_find = true;
					break;
				}
			}
			if (false == is_find)
			{
				std::vector<ElementLadderTaskTemplate*> all_template = template_manager::get_instance().get_all_element_task_template_array_by_mission_type(mission_type);
				if (all_template.size() > 0)
				{
					ElementLadderTaskTemplate* ladder_tem = all_template[random_gen::get_random(0, all_template.size() - 1)];
					if (ladder_tem != nullptr)
					{
						mission_list.push_back(ladder_tem->attribute_id);
						mission_type_list.push_back(mission_type);
						while_num = 0;
					}
				}
			}
		}
		return mission_list;
	}

	void element_war_ws_mgr::map_game_end(const cs2ws_element_war_map_game_end_role_info* end_info)
	{
		if (nullptr == end_info)
		{
			return;
		}
		auto role_iter = m_all_player_info.find(end_info->role_guid);
		if (role_iter == m_all_player_info.end())
		{
			return;
		}
		int32 old_score = role_iter->second.get_data_array_info(e_element_war_role_info_score_num);
		int32 new_score = old_score + end_info->add_score;
		if (new_score < 0)
		{
			new_score = 0;
		}
		role_iter->second.set_data_array_info(e_element_war_role_info_score_num, new_score);
		int32 create_time = time_helper::get_cur_time_new().second;

		CONSOLE_INFO("element_war_ws_mgr::map_game_end  RoleGuid:{} ServerId:{} NewScore:{} OldScore:{} HistoryScore:{} IsWin:{} RankNum:{} IsEscape:{}", end_info->role_guid.server_64, end_info->role_guid.server_64, new_score, old_score, role_iter->second.get_data_array_info(e_element_war_role_info_history_score), end_info->is_win, end_info->score_rank, end_info->is_escape)

		role_iter->second.game_end(end_info->is_win);

		s_ranking_player_info role_info;
		role_info.role_guid = end_info->role_guid;
		role_info.set_role_name(role_iter->second.get_role_name());
		role_info.ranking_type = e_RankingIndex_element_war_cur_season;
		role_info.ranking_value = new_score;
		role_info.set_supporting_name(role_iter->second.get_server_name());
		role_info.supporting_guid.B = role_iter->second.get_data_array_info(e_element_war_role_info_template_id);
		role_info.standby_parameter = role_iter->second.get_gs_value();
		role_info.server_id = role_iter->second.get_data_array_info(e_element_war_role_info_server_id);
		role_info.vip_level = role_iter->second.get_data_array_info(e_element_war_role_info_vip_level);
		role_info.save_time = create_time;
		ranking_mgr_ws::sync_player_info(role_info);
		role_iter->second.check_player_mission(e_element_war_mission_type_join_map_num, 1);
		if (end_info->is_win)
		{
			role_iter->second.check_player_mission(e_element_war_mission_type_win_num, 1);
		}
		if (end_info->score_rank == 1)
		{
			role_iter->second.check_player_mission(e_element_war_mission_type_kill_first_score_num, 1);
		}
		role_iter->second.check_player_mission(e_element_war_mission_type_kill_monster_num, end_info->kill_monster);
		role_iter->second.check_player_mission(e_element_war_mission_type_kill_player_num, end_info->kill_player);
		role_iter->second.check_player_mission(e_element_war_mission_type_kill_elite_num, end_info->kill_elite);
		role_iter->second.check_player_mission(e_element_war_mission_type_kill_boss_num, end_info->kill_boss);
		role_iter->second.save_mission_info();
		//role_iter->second.refresh_season_info();

		send_element_war_info(role_iter->second, false);

		ws2ws_element_war_sync_game_result sync_msg;
		sync_msg.role_guid = end_info->role_guid;
		memcpy(sync_msg.role_name, role_iter->second.get_role_name(), max_name_size);
		sync_msg.new_score = new_score;
		sync_msg.hight_score = role_iter->second.get_data_array_info(e_element_war_role_info_history_score);
		sync_msg.server_id = role_iter->second.get_data_array_info(e_element_war_role_info_server_id);
		sync_msg.vip_level = role_iter->second.get_data_array_info(e_element_war_role_info_vip_level);
		
		cross::send_msg_to_ws(sync_msg.role_guid, sync_msg.server_id, e_msgindex_ws2ws_element_war_sync_game_result, &sync_msg, sizeof(sync_msg));
		if (role_iter->second.get_data_array_info(e_element_war_role_info_history_score) < new_score)
		{
			role_iter->second.set_data_array_info(e_element_war_role_info_history_score, new_score);
		}
		role_iter->second.update_info_to_dp();
	}

	void element_war_ws_mgr::check_can_get_mission_reward(const guid_64 & role_guid, int32 mission_index)
	{
		ws2ws_element_war_check_can_get_mission_reward_end msg;
		msg.ret_index = e_element_war_get_mission_reward_succeed;
		msg.mission_index = mission_index;
		auto role_iter = m_all_player_info.find(role_guid);
		if (role_iter == m_all_player_info.end())
		{
			return;
		}
		ElementLadderTaskTemplate* template_ptr = GET_TEMPLATE(ElementLadderTaskTemplate, role_iter->second.get_data_array_info((e_element_war_role_info)(e_element_war_role_info_mission1 + mission_index)));
		if (nullptr == template_ptr)
		{
			return;
		}
		msg.mission_id = template_ptr->attribute_id;
		msg.role_guid = role_guid;
		if (role_iter->second.get_data_array_info((e_element_war_role_info)(e_element_war_role_info_mission1_have_num + mission_index)) < template_ptr->Value)
		{
			msg.ret_index = e_element_war_get_mission_reward_not_num;
		}
		cross::send_msg_to_ws(role_guid, role_iter->second.get_data_array_info(e_element_war_role_info_server_id), e_msgindex_ws2ws_element_war_check_can_get_mission_reward_end, &msg, sizeof(msg));
	}

	void element_war_ws_mgr::send_season_ranking_reward_to_player(const guid_64 & role_guid, int32 server_id)
	{
		int32 ranking_level = -1;
		int64 ranking_value = 0;
		ranking_mgr_ws::get_player_rank_level(e_RankingIndex_element_war_last_season, role_guid, ranking_level, ranking_value);

		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		std::vector<s_item_template_info> drop_item_list;
		std::vector<int32>& rwd_data = act_com_cfg->RankRewards;
		globle_data::get_instance().get_item_list_by_rank_rwd(ranking_level, rwd_data, drop_item_list);

		if (drop_item_list.size() <= 0)
		{
			return;
		}
		std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_element_war_season_ranking_reward_title);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_element_war_season_ranking_reward_context));
		content_params.push_back(init_unit::change_i32_to_string(ranking_level));
		std::string contenttext = init_unit::implode(content_params);

		mail_ws_mgr::get_instance().send_mail_system(role_guid, server_id, drop_item_list, title, contenttext, 0, 0, 0, 0);
	}

	void element_war_ws_mgr::deal_with_create_map_msg(s_element_war_match_info & match_info)
	{
		auto map_ent = cs_map_mgr_system::init_map_by_battle(ELEMENT_WAR_MAP_ID);
		if (map_ent)
		{
			guid_64 map_guid = map_ent->getEntityId();
			//m_war_map_list[map_guid.server_64] = match_info;
			//发送匹配成功
			send_match_success_msg(map_guid, 1, match_info);
			create_robot_init_map(map_guid, match_info);
			/*for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
			{
				//记录传送信息
				if (match_info.first_role_info[i].server_id != world_server::getInstance().get_server_id())
				{
					s_game_info tmp_game_info;
					tmp_game_info.server_id = match_info.first_role_info[i].server_id;
					memcpy(tmp_game_info.out_ip, match_info.first_role_info[i].role_server_out_ip, max_ip_address_length);
					tmp_game_info.out_port = match_info.first_role_info[i].role_server_out_port;
					m_role_server_info_map[match_info.first_role_info[i].role_guid.server_64] = tmp_game_info;
				}
				if (match_info.second_role_info[i].server_id != world_server::getInstance().get_server_id())
				{
					s_game_info tmp_game_info;
					tmp_game_info.server_id = match_info.second_role_info[i].server_id;
					memcpy(tmp_game_info.out_ip, match_info.second_role_info[i].role_server_out_ip, max_ip_address_length);
					tmp_game_info.out_port = match_info.second_role_info[i].role_server_out_port;
					m_role_server_info_map[match_info.second_role_info[i].role_guid.server_64] = tmp_game_info;
				}
			}*/
		}
		else
		{
			//发送地图创建错误
			ws2ws_element_war_create_map_failed msg;
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
			{
				if (match_info.first_role_info[i].role_guid.is_valid() && match_info.first_role_info[i].is_robor == false)
				{
					msg.first_role_guid[i] = match_info.first_role_info[i].role_guid;
					send_sign_up_result(match_info.first_role_info[i].role_guid, e_element_war_sign_up_map_limit, match_info.first_role_info[i].server_id);
				}
				if (match_info.second_role_info[i].role_guid.is_valid() && match_info.second_role_info[i].is_robor == false)
				{
					msg.second_role_guid[i] = match_info.second_role_info[i].role_guid;
					send_sign_up_result(match_info.second_role_info[i].role_guid, e_element_war_sign_up_map_limit, match_info.second_role_info[i].server_id);
				}
			}
			cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_element_war_create_map_failed, &msg, sizeof(msg));
		}
	}

	void element_war_ws_mgr::send_match_success_msg(guid_64 map_id, int32 war_index, const s_element_war_match_info & opponent_info)
	{
		ws2ws_element_war_send_match_success msg;
		msg.map_id = map_id;
		msg.server_id = world_server::getInstance().get_server_id();
		msg.war_index = war_index;
		for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
		{
			msg.match_info.first_role_info[i].role_guid = opponent_info.first_role_info[i].role_guid;
			msg.match_info.first_role_info[i].server_id = opponent_info.first_role_info[i].server_id;
			msg.match_info.first_role_info[i].template_id = opponent_info.first_role_info[i].template_id;
			msg.match_info.first_role_info[i].score_num = opponent_info.first_role_info[i].score_num;
			msg.match_info.first_role_info[i].total_join_num = opponent_info.first_role_info[i].total_join_num;
			msg.match_info.first_role_info[i].total_win_num = opponent_info.first_role_info[i].total_win_num;
			msg.match_info.first_role_info[i].last_win_num = opponent_info.first_role_info[i].last_win_num;
			msg.match_info.first_role_info[i].is_robor = opponent_info.first_role_info[i].is_robor;
			msg.match_info.first_role_info[i].robor_template_id = opponent_info.first_role_info[i].robor_template_id;
			msg.match_info.first_role_info[i].vip_level = opponent_info.first_role_info[i].vip_level;
			memcpy(msg.match_info.first_role_info[i].role_name, opponent_info.first_role_info[i].role_name, max_name_size);
			memcpy(msg.match_info.first_role_info[i].server_name, opponent_info.first_role_info[i].server_name, max_name_size);

			msg.match_info.second_role_info[i].role_guid = opponent_info.second_role_info[i].role_guid;
			msg.match_info.second_role_info[i].server_id = opponent_info.second_role_info[i].server_id;
			msg.match_info.second_role_info[i].template_id = opponent_info.second_role_info[i].template_id;
			msg.match_info.second_role_info[i].score_num = opponent_info.second_role_info[i].score_num;
			msg.match_info.second_role_info[i].total_join_num = opponent_info.second_role_info[i].total_join_num;
			msg.match_info.second_role_info[i].total_win_num = opponent_info.second_role_info[i].total_win_num;
			msg.match_info.second_role_info[i].last_win_num = opponent_info.second_role_info[i].last_win_num;
			msg.match_info.second_role_info[i].is_robor = opponent_info.second_role_info[i].is_robor;
			msg.match_info.second_role_info[i].robor_template_id = opponent_info.second_role_info[i].robor_template_id;
			msg.match_info.second_role_info[i].vip_level = opponent_info.second_role_info[i].vip_level;
			memcpy(msg.match_info.second_role_info[i].role_name, opponent_info.second_role_info[i].role_name, max_name_size);
			memcpy(msg.match_info.second_role_info[i].server_name, opponent_info.second_role_info[i].server_name, max_name_size);
		}
		for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
		{
			if (opponent_info.first_role_info[i].server_id == world_server::getInstance().get_server_id())
			{
				deal_with_match_success_msg(opponent_info.first_role_info[i].role_guid, &msg);
			}
			else
			{
				auto tmp_server_config = world_server::getInstance().get_server_config_by_server_id(world_server::getInstance().get_server_id());
				if (nullptr != tmp_server_config)
				{
					msg.match_info.first_role_info[i].target_game_info = *tmp_server_config;
					cross::send_msg_to_ws(opponent_info.first_role_info[i].role_guid, opponent_info.first_role_info[i].server_id, e_msgindex_ws2ws_element_war_send_match_success, &msg, sizeof(msg));
				}
			}
			if (opponent_info.second_role_info[i].server_id == world_server::getInstance().get_server_id())
			{
				deal_with_match_success_msg(opponent_info.second_role_info[i].role_guid, &msg);
			}
			else
			{
				auto tmp_server_config = world_server::getInstance().get_server_config_by_server_id(world_server::getInstance().get_server_id());
				if (nullptr != tmp_server_config)
				{
					msg.match_info.second_role_info[i].target_game_info = *tmp_server_config;
					cross::send_msg_to_ws(opponent_info.second_role_info[i].role_guid, opponent_info.second_role_info[i].server_id, e_msgindex_ws2ws_element_war_send_match_success, &msg, sizeof(msg));
				}
			}
		}
	}

	void element_war_ws_mgr::create_robot_init_map(guid_64 map_guid, s_element_war_match_info & match_info)
	{
		ws2cs_element_war_send_create_robot msg;
		msg.map_guid = map_guid;
		msg.match_info = match_info;

		bool is_use = proto_by_lua(e_msgindex_ws2cs_element_war_send_create_robot);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::element_war_send_create_robot pro_msg;
			msg.to_proto(pro_msg);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_element_war_send_create_robot, e_server_type_cs);
		}
	}

	void element_war_ws_mgr::session_online(client_session * session)
	{
		if (nullptr == session)
		{
			return;
		}
			
		if (session->is_self_server())
		{
			send_req_element_war_role_info(session, true);
			send_upgrade_notify(session);
		}

		game_proto_send_server_name msg;
		msg.set_is_self_server(session->is_self_server());
		msg.set_server_name(world_server::getInstance().get_server_name());
		session->send_to_client(&msg, e_msgindex_s2c_send_server_name);
	}

	bool element_war_ws_mgr::send_req_element_war_role_info(client_session * session, bool is_online)
	{
		if (nullptr == session || session->is_self_server() == false)
		{
			return false;
		}
		if (gate_proxy::getInstance().is_gate_run() && world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war))
		{
			ws2ws_element_war_req_role_info msg;
			memcpy(msg.role_name, session->m_role_info.role_name, max_name_size);
			memcpy(msg.server_name, world_server::getInstance().get_server_name().c_str(), max_name_size);
			msg.server_id = world_server::getInstance().get_server_id();
			msg.template_id = session->get_role_info_data(e_role_info_template_id);
			msg.role_gs = session->get_role_gs_value();
			msg.role_level = session->get_role_info_data(e_role_info_exp_level);
			msg.vip_level = session->get_role_info_data(e_role_info_vip_level);
			msg.is_online = is_online;
			cross::send_msg_to_ws(session->get_role_guid(), get_match_server_id(), e_msgindex_ws2ws_element_war_req_role_info, &msg, sizeof(msg));
			return true;
		}
		return false;
	}

	void element_war_ws_mgr::send_upgrade_notify(client_session * session)
	{
		if (nullptr == session || nullptr == session->get_map_temp_ptr() || session->get_map_temp_ptr()->Type != e_map_type_big_map)
		{
			return;
		}	
		auto iter = m_notify_map.find(session->get_role_guid());
		if (iter != m_notify_map.end())
		{
			game_proto_element_war_send_upgrade_notify notify;
			notify.set_upgrade_id(iter->second);
			session->send_to_client(&notify, e_mgsindex_s2c_element_war_send_upgrade_notify);
			m_notify_map.erase(iter);
		}
	}

	e_element_war_sign_up_ret element_war_ws_mgr::check_can_sign_up(client_session * session)
	{
		if (nullptr == session || false == session->is_vaild() || false == session->is_self_server())
		{
			return e_element_war_sign_up_failed;
		}
		if (is_in_activity_time() == false)
		{
			return e_element_war_sign_up_not_open;
		}
		if (world_server::getInstance().get_activity_sec_left(e_activity_type_element_war, e_activity_time_get_gaming) <= 0)
		{
			return e_element_war_sign_up_not_in_time;
		}
		if (false == gate_proxy::getInstance().is_gate_run())
		{
			return e_element_war_sign_up_cross_server_fail;
		}
		if (session->get_role_info_data(e_role_info_exp_level) < m_config_level_limit)
		{
			return e_element_war_sign_up_level_limit;
		}
		return e_element_war_sign_up_success;
	}

	void element_war_ws_mgr::send_team_ready_end(guid_64 team_guid)
	{
		team_ws* team_ref = team_ws_mgr::get_instance().get_team(team_guid);
		if (team_ref == nullptr)
		{
			return;
		}
		ws2ws_element_war_sign_up msg;
		game_proto_element_war_send_sign_up_end erroe_msg;
		const std::list<s_team_member_info>& vec_team_mem = team_ref->get_member_list();
		int32 num_index = 0;
		e_element_war_sign_up_ret error_ret = e_element_war_sign_up_success;
		for (const s_team_member_info& team_mem : vec_team_mem)
		{
			if (num_index < 0 || num_index >= ELEMENT_WAR_PLAY_NUM)
			{
				break;
			}
			client_session* mem_session = client_session_mgr::getInstance().get_session(team_mem.role_guid);
			if (nullptr != mem_session)
			{
				error_ret = check_can_sign_up(mem_session);
				if (error_ret != e_element_war_sign_up_success)
				{
					erroe_msg.set_guid_a(team_mem.role_guid.A);
					erroe_msg.set_guid_b(team_mem.role_guid.B);
					break;
				}
			}
			else
			{
				error_ret = e_element_war_sign_up_is_not_line;
				erroe_msg.set_guid_a(team_mem.role_guid.A);
				erroe_msg.set_guid_b(team_mem.role_guid.B);
				break;
			}
			msg.role_guid_list[num_index] = team_mem.role_guid;
			num_index++;
		}

		
		if (error_ret == e_element_war_sign_up_success)
		{
			msg.team_guid = team_guid;
			msg.server_id = world_server::getInstance().get_server_id();
			memcpy(msg.server_ip, world_server::getInstance().get_out_ip().c_str(), max_ip_address_length);
			msg.server_port = world_server::getInstance().get_out_port();
			msg.online_num = client_session_mgr::getInstance().get_game_num();
			cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_element_war_send_sign_up, &msg, sizeof(msg));
		}
		else
		{
			erroe_msg.set_ret_index(error_ret);
			team_ref->send_message_to_all_member(&erroe_msg, e_mgsindex_s2c_element_war_send_sign_up_end);
		}
	}

	void element_war_ws_mgr::send_team_sign_up(client_session* session)
	{
		if (nullptr == session || session->is_self_server() == false || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war) == false)
		{
			return;
		}
		team_ws* team_ref = team_ws_mgr::get_instance().get_team(session->get_team_guid());
		if (team_ref == nullptr)
		{
			return;
		}
		game_proto_element_war_send_sign_up_end msg;
		msg.set_ret_index(e_element_war_sign_up_success);
		if (session->get_role_guid() != team_ref->get_captain_guid())
		{
			//文字待添加  非队长无法开启准备
			session->send_notice("");
			return;
		}
		e_element_war_sign_up_ret error_ret = e_element_war_sign_up_success;
		guid_64 not_can_guid;

		const std::list<s_team_member_info>& vec_team_mem = team_ref->get_member_list();
		for (const s_team_member_info& team_mem : vec_team_mem)
		{
			client_session* mem_session = client_session_mgr::getInstance().get_session(team_mem.role_guid);
			if (nullptr != mem_session)
			{
				error_ret = check_can_sign_up(mem_session);
				if (error_ret != e_element_war_sign_up_success)
				{
					msg.set_guid_a(team_mem.role_guid.A);
					msg.set_guid_b(team_mem.role_guid.B);
					break;
				}
			}
			else
			{
				error_ret = e_element_war_sign_up_is_not_line;
				msg.set_guid_a(team_mem.role_guid.A);
				msg.set_guid_b(team_mem.role_guid.B);
				break;
			}
		}

		if (error_ret == e_element_war_sign_up_success)
		{
			team_ref->change_team_aim(e_team_type_element_war, 0);
			team_ref->sync_team_attribute_to_mems();
			team_ref->send_all_member_aoi_team_info();
			team_ref->set_member_ready_status(session->get_role_guid(), true);

			/*game_proto_element_war_send_create_ready_ui ready_msg;
			for (const s_team_member_info& team_mem : vec_team_mem)
			{
				client_session* mem_session = client_session_mgr::getInstance().get_session(team_mem.role_guid);
				if (nullptr != mem_session)
				{
					mem_session->send_to_client(&ready_msg, e_mgsindex_s2c_element_war_send_create_ready_ui);
				}
			}*/
		}
		else
		{
			msg.set_ret_index(error_ret);
			team_ref->send_message_to_all_member(&msg, e_mgsindex_s2c_element_war_send_sign_up_end);
		}
		
	}

	void element_war_ws_mgr::send_single_sign_up(client_session* session)
	{
		if (nullptr == session || session->is_self_server() == false || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war) == false)
		{
			return;
		}
		e_element_war_sign_up_ret ret = element_war_ws_mgr::get_instance().check_can_sign_up(session);
		if (e_element_war_sign_up_success != ret)
		{
			game_proto_element_war_send_sign_up_end msg;
			msg.set_ret_index(ret);
			msg.set_guid_a(session->get_role_guid().A);
			msg.set_guid_b(session->get_role_guid().B);
			session->send_to_client(&msg, e_mgsindex_s2c_element_war_send_sign_up_end);
		}
		else
		{
			ws2ws_element_war_sign_up msg;
			msg.role_guid_list[0] = session->get_role_guid();
			msg.server_id = world_server::getInstance().get_server_id();
			memcpy(msg.server_ip, world_server::getInstance().get_out_ip().c_str(), max_ip_address_length);
			msg.server_port = world_server::getInstance().get_out_port();
			msg.online_num = client_session_mgr::getInstance().get_game_num();
			cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_element_war_send_sign_up, &msg, sizeof(msg));
		}
	}

	void element_war_ws_mgr::send_close_sign_up(client_session * session)
	{
		ws2ws_element_war_close_sign_up msg;
		msg.role_guid = session->get_role_guid();
		msg.server_id = world_server::getInstance().get_server_id();
		cross::send_msg_to_ws(guid_64(), get_match_server_id(), e_msgindex_ws2ws_element_war_close_sign_up, &msg, sizeof(msg));
	}

	void element_war_ws_mgr::transfer_player(int64 time_new)
	{
		for (auto iter = m_transfer_list.begin(); iter != m_transfer_list.end();)
		{
			if (time_new >= iter->m_transfer_time + m_config_transfer_time)
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(iter->m_role_guid);
				if (nullptr != session_ptr)
				{
					if (iter->m_server_id == world_server::getInstance().get_server_id())
					{
						cs_map_mgr_system::transfer_to_map(session_ptr, ELEMENT_WAR_MAP_ID, iter->m_map_guid, iter->m_war_index);
					}		
					else
					{
						cs_map_mgr_system::transfer_to_map(session_ptr, ELEMENT_WAR_MAP_ID, iter->m_map_guid, iter->m_war_index, iter->m_server_id);

					}
				}
				m_transfer_list.erase(iter++);
			}
			else
			{
				break;
			}		
		}
	}

	void element_war_ws_mgr::deal_with_match_success_msg(const guid_64 & role_guid, const ws2ws_element_war_send_match_success * msg)
	{
		if (msg == nullptr)
		{
			return;
		}
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr != session_ptr && msg != nullptr)
		{
			m_transfer_list.push_back(s_element_war_transfer_info(role_guid, msg->server_id, msg->map_id, msg->war_index, utility::get_tick_count()));

			game_proto_element_war_send_match_success_msg match_msg;
			for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
			{
				game_proto_element_match_info *first_team_info = match_msg.add_my_team_info();
				game_proto_element_match_info *secord_team_info = match_msg.add_other_team_info();
				if (nullptr == first_team_info || nullptr == secord_team_info)
				{
					break;
				}
				first_team_info->set_guid_a(msg->match_info.first_role_info[i].role_guid.A);
				first_team_info->set_guid_b(msg->match_info.first_role_info[i].role_guid.B);
				first_team_info->set_role_name(msg->match_info.first_role_info[i].role_name);
				first_team_info->set_is_robot(msg->match_info.first_role_info[i].is_robor);
				first_team_info->set_template_id(msg->match_info.first_role_info[i].template_id);
				first_team_info->set_score_num(msg->match_info.first_role_info[i].score_num);
				first_team_info->set_vip_level(msg->match_info.first_role_info[i].vip_level);
				
				if (msg->match_info.first_role_info[i].role_guid == role_guid)
				{
					if (msg->server_id != world_server::getInstance().get_server_id())
					{
						m_transfer_info_map[role_guid] = msg->match_info.first_role_info[i].target_game_info;
					}
				}

				secord_team_info->set_guid_a(msg->match_info.second_role_info[i].role_guid.A);
				secord_team_info->set_guid_b(msg->match_info.second_role_info[i].role_guid.B);
				secord_team_info->set_role_name(msg->match_info.second_role_info[i].role_name);
				secord_team_info->set_is_robot(msg->match_info.second_role_info[i].is_robor);
				secord_team_info->set_template_id(msg->match_info.second_role_info[i].template_id);
				secord_team_info->set_score_num(msg->match_info.second_role_info[i].score_num);
				secord_team_info->set_vip_level(msg->match_info.second_role_info[i].vip_level);

				if (msg->match_info.second_role_info[i].role_guid == role_guid)
				{
					if (msg->server_id != world_server::getInstance().get_server_id())
					{
						m_transfer_info_map[role_guid] = msg->match_info.second_role_info[i].target_game_info;
					}
				}
			}
			session_ptr->send_to_client(&match_msg, e_msgindex_s2c_element_war_send_match_success);
		}
	}

	void element_war_ws_mgr::add_break_line_player(s_break_line_login_info break_info)
	{
		int64 game_time = utility::get_tick_count();
		break_info.expiry_time = game_time + minute_tick_time;
		bool is_change = true;
		for (int32 i = 0; i < m_break_line_login_list.size(); ++i)
		{
			if (m_break_line_login_list[i].role_guid == break_info.role_guid)
			{
				m_break_line_login_list[i] = break_info;
				is_change = false;
				break;
			}
		}
		if (is_change)
		{
			m_break_line_login_list.push_back(break_info);
		}
	}

	void element_war_ws_mgr::check_break_line_player()
	{
		int64 game_time = utility::get_tick_count();
		vector<s_break_line_login_info>::iterator ite = m_break_line_login_list.begin();
		for (; ite != m_break_line_login_list.end();)
		{
			s_break_line_login_info& other_info = *ite;
			if (game_time >= other_info.expiry_time)
			{
				//不要删除防止接近时间的时候重新连接导致获取不到数据
				other_info.is_send_msg = true;
				if (other_info.remove_time > 0 && game_time >= other_info.remove_time)
				{
					ite = m_break_line_login_list.erase(ite);
					continue;
				}
				else if (other_info.remove_time <= 0)
				{
					other_info.remove_time = game_time + 60 * second_tick_time;
				}
				ite++;
				continue;
			}
			if (other_info.is_send_msg)
			{
				ite++;
				continue;
			}
			client_session* session_ptr = client_session_mgr::getInstance().get_session(other_info.role_guid);
			if (nullptr == session_ptr || session_ptr->m_online_state != client_session::e_os_online || session_ptr->m_status != client_session::e_ss_ingame)
			{
				ite++;
				continue;
			}
			other_info.is_send_msg = true;
			send_break_line_msg(other_info.role_guid, other_info.map_template_id);
			ite++;
		}
	}

	void element_war_ws_mgr::del_break_line_player(guid_64 role_guid)
	{
		vector<s_break_line_login_info>::iterator ite = m_break_line_login_list.begin();
		for (; ite != m_break_line_login_list.end(); ite++)
		{
			s_break_line_login_info& other_info = *ite;

			if (other_info.role_guid == role_guid)
			{
				m_break_line_login_list.erase(ite);
				break;
			}
		}
	}

	void element_war_ws_mgr::send_break_line_msg(guid_64 role_guid, int32 map_template_id)
	{
		client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		game_proto_send_need_break_line_login msg;
		msg.set_map_template_id(map_template_id);
		msg.set_active_type(e_activity_type_element_war);
		session_ptr->send_to_client(&msg, e_mgsindex_s2c_send_need_break_line_login);
	}

	void element_war_ws_mgr::break_line_login_msg(guid_64 role_guid, bool is_login)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		if (is_login)
		{
			s_break_line_login_info role_login_info = get_player_break_login_info(role_guid);
			if (false == role_login_info.role_guid.is_valid())
			{
				//发送活动已结束
				session->send_notice("90201845");
				return;
			}
			cs_map_mgr_system::transfer_to_map(session, ELEMENT_WAR_MAP_ID, role_login_info.map_guid, 1, role_login_info.server_id);
		}
		else
		{
			del_break_line_player(role_guid);
		}
	}

	s_break_line_login_info element_war_ws_mgr::get_player_break_login_info(guid_64 role_guid)
	{
		s_break_line_login_info login_info;
		for (int32 i = 0; i < m_break_line_login_list.size(); ++i)
		{
			if (m_break_line_login_list[i].role_guid == role_guid)
			{
				login_info = m_break_line_login_list[i];
				break;
			}
		}
		return login_info;
	}

	void element_war_ws_mgr::deal_with_pk_result_msg(const guid_64 & role_guid, const ws2ws_element_war_sync_game_result * msg)
	{
		if (nullptr == msg)
		{
			return;
		}

		int32 old_score = msg->hight_score;
		int32 new_score = msg->new_score;

		int32 create_time = time_helper::get_cur_time_new().second;

		//1.同步排行榜
		s_ranking_player_info role_info;
		role_info.role_guid = msg->role_guid;
		role_info.set_role_name(msg->role_name);
		role_info.ranking_type = e_RankingIndex_element_rank_list;
		role_info.ranking_value = new_score;
		role_info.server_id = msg->server_id;
		role_info.vip_level = msg->vip_level;
		role_info.save_time = create_time;
		ranking_mgr_ws::sync_player_info(role_info);

		//2. 发送段位提升奖励
		bool is_update = false;
		if (new_score > old_score)
		{
			std::vector<ElementLadderTemplate*> ladder_template_ptr = template_manager::get_instance().get_template_array_by_element_min_and_max_score(old_score, new_score);
			if (ladder_template_ptr.size() > 0)
			{
				is_update = true;
				for (int32 i = 0; i < ladder_template_ptr.size(); i++)
				{
					if (ladder_template_ptr[i] == nullptr)
					{
						continue;;
					}
					const auto & reward_vec = ladder_template_ptr[i]->UpgradeReward;
					int32 vec_size = reward_vec.size();
					if (vec_size && vec_size % 3 == 0)
					{
						const xchar* title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_element_war_upgrade_title);
						const xchar* content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_element_war_upgrade_context);
						if (title != nullptr && content != nullptr)
						{
							std::vector<std::string> content_params;
							content_params.clear();
							content_params.push_back(std::string(content));
							content_params.push_back(init_unit::change_i32_to_string(ladder_template_ptr[i]->RankName));
							std::string contenttext = init_unit::implode(content_params);

							int32 money_type_1 = 0, money_num_1 = 0, money_type_2 = 0, money_num_2 = 0;
							std::vector<s_item_template_info> drop_item_list;
							drop_item_list.clear();

							for (int32 i = 0; i < vec_size / 2; ++i)
							{
								drop_item_list.push_back({ reward_vec[i * 2], reward_vec[i * 2 + 1], 1 });
							}
							mail_ws_mgr::get_instance().send_mail_system(msg->role_guid, msg->server_id, drop_item_list, title, contenttext, money_type_1, money_num_1, money_type_2, money_num_2);
						}
					}
				}
			}
		}
		if (is_update)
		{
			ElementLadderTemplate* update_template_ptr = template_manager::get_instance().get_template_by_element_score(msg->new_score);
			if (nullptr == update_template_ptr)
			{
				return;
			}
			m_notify_map[role_guid] = update_template_ptr->attribute_id;
		}
		del_break_line_player(role_guid);
	}

	void element_war_ws_mgr::get_mission_reward(guid_64 role_guid, int32 mission_idex)
	{
		ws2ws_element_war_check_can_get_mission_reward msg;
		msg.role_guid = role_guid;
		msg.mission_index = mission_idex;
		cross::send_msg_to_ws(msg.role_guid, get_match_server_id(), e_msgindex_ws2ws_element_war_check_can_get_mission_reward, &msg, sizeof(msg));
	}

	void element_war_ws_mgr::buy_ticket_num(guid_64 role_guid)
	{
		auto role_it = m_all_player_info.find(role_guid.server_64);
		if (role_it == m_all_player_info.end())
		{
			return;
		}
		auto& role_info = role_it->second;
		role_info.buy_ticket();
		send_element_war_info(role_info, false);
	}

	void element_war_ws_mgr::req_element_ladder_last_score(const guid_64 & role_guid, int32 server_id)
	{
		auto iter = m_all_player_info.find(role_guid);
		if (iter != m_all_player_info.end())
		{
			iter->second.send_last_season_score(server_id);
		}
	}

	bool element_war_ws_mgr::match_with_robot(c_element_war_role_info* play_role_info, std::vector<guid_64> first_team_list, std::vector<guid_64> secord_team_list)
	{
		if (nullptr == play_role_info)
		{
			return false;
		}
		if (GAMECONFIG->InitRoleAry.size() < 4)
		{
			return false;
		}
		std::vector<int32> robor_tem_array = play_role_info->get_robot_array();
		int32 robor_size = robor_tem_array.size();
		if (robor_size <= 0)
		{
			return false;
		}
		int32 robor_index = 0;
		ws2ws_element_war_create_map msg;
		int32 target_server_id = 0;
		int32 player_level = 0;
		int32 player_num = 0;
		for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
		{
			if (i >= first_team_list.size())
			{
				s_element_war_match_role_info role_info;
				role_info.set_role_name(init_unit::load_random_role_name());
				role_info.set_server_name(play_role_info->get_server_name());
				role_info.server_id = play_role_info->get_data_array_info(e_element_war_role_info_server_id);
				if ((rand() % 2) == 1 || play_role_info->get_data_array_info(e_element_war_role_info_score_num) < 1000)
				{
					role_info.score_num = (play_role_info->get_data_array_info(e_element_war_role_info_score_num) + rand() % 1000);
				}
				else
				{
					role_info.score_num = (play_role_info->get_data_array_info(e_element_war_role_info_score_num) - rand() % 1000);
				}
				role_info.total_join_num = play_role_info->get_data_array_info(e_element_war_role_info_total_join_num);
				role_info.total_win_num = play_role_info->get_data_array_info(e_element_war_role_info_total_win_num);
				role_info.last_win_num = play_role_info->get_data_array_info(e_element_war_role_info_last_win_num);
				role_info.is_robor = true;
				role_info.robor_template_id = robor_tem_array[rand() % robor_size];
				NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, role_info.robor_template_id);
				if (npc_template_ptr == nullptr)
				{
					continue;
				}
				int32 npc_class = npc_template_ptr->Class;
				if (npc_class < 1 || npc_class > 4)
				{
					continue;
				}
				role_info.template_id = GAMECONFIG->InitRoleAry[npc_class - 1];
				msg.match_info.first_role_info[i] = role_info;		
			}
			else
			{
				auto role_it = m_all_player_info.find(first_team_list[i]);
				if (role_it == m_all_player_info.end())
				{
					continue;
				}
				auto& first_role_it = role_it->second;
				s_element_war_match_role_info role_info;
				if (target_server_id == 0)
				{
					target_server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
				}
				first_role_it.reduce_ticket();
				first_role_it.set_pk_state(e_element_war_role_join_war);

				memcpy(role_info.role_name, first_role_it.get_role_name(), max_name_size);
				memcpy(role_info.server_name, first_role_it.get_server_name(), max_name_size);
				role_info.role_guid = first_role_it.get_role_guid();
				role_info.server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
				role_info.template_id = first_role_it.get_data_array_info(e_element_war_role_info_template_id);
				role_info.role_level = first_role_it.get_data_array_info(e_element_war_role_info_role_level);
				role_info.score_num = first_role_it.get_data_array_info(e_element_war_role_info_score_num);
				role_info.total_join_num = first_role_it.get_data_array_info(e_element_war_role_info_total_join_num);
				role_info.total_win_num = first_role_it.get_data_array_info(e_element_war_role_info_total_win_num);
				role_info.last_win_num = first_role_it.get_data_array_info(e_element_war_role_info_last_win_num);
				memcpy(role_info.role_server_out_ip, first_role_it.get_server_out_ip().c_str(), max_ip_address_length);
				role_info.role_server_out_port = first_role_it.get_server_out_port();
				msg.match_info.first_role_info[i] = role_info;
				player_level += first_role_it.get_data_array_info(e_element_war_role_info_role_level);
				player_num++;
			}
		}

		for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
		{
			if (i >= secord_team_list.size())
			{
				s_element_war_match_role_info role_info;
				role_info.set_role_name(init_unit::load_random_role_name());
				role_info.set_server_name(play_role_info->get_server_name());
				role_info.server_id = play_role_info->get_data_array_info(e_element_war_role_info_server_id);
				if ((rand() % 2) == 1 || play_role_info->get_data_array_info(e_element_war_role_info_score_num) < 1000)
				{
					role_info.score_num = (play_role_info->get_data_array_info(e_element_war_role_info_score_num) + rand() % 1000);
				}
				else
				{
					role_info.score_num = (play_role_info->get_data_array_info(e_element_war_role_info_score_num) - rand() % 1000);
				}
				role_info.role_level = play_role_info->get_data_array_info(e_element_war_role_info_role_level);
				role_info.total_join_num = play_role_info->get_data_array_info(e_element_war_role_info_total_join_num);
				role_info.total_win_num = play_role_info->get_data_array_info(e_element_war_role_info_total_win_num);
				role_info.last_win_num = play_role_info->get_data_array_info(e_element_war_role_info_last_win_num);
				role_info.is_robor = true;
				role_info.robor_template_id = robor_tem_array[rand() % robor_size];
				NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, role_info.robor_template_id);
				if (npc_template_ptr == nullptr)
				{
					continue;
				}
				int32 npc_class = npc_template_ptr->Class;
				if (npc_class < 1 || npc_class > 4)
				{
					continue;
				}
				role_info.template_id = GAMECONFIG->InitRoleAry[npc_class - 1];
				msg.match_info.second_role_info[i] = role_info;
			}
			else
			{
				auto role_it = m_all_player_info.find(secord_team_list[i]);
				if (role_it == m_all_player_info.end())
				{
					continue;
				}
				auto& first_role_it = role_it->second;
				s_element_war_match_role_info role_info;
				if (target_server_id == 0)
				{
					target_server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
				}
				first_role_it.reduce_ticket();
				first_role_it.set_pk_state(e_element_war_role_join_war);

				memcpy(role_info.role_name, first_role_it.get_role_name(), max_name_size);
				memcpy(role_info.server_name, first_role_it.get_server_name(), max_name_size);
				role_info.role_guid = first_role_it.get_role_guid();
				role_info.server_id = first_role_it.get_data_array_info(e_element_war_role_info_server_id);
				role_info.template_id = first_role_it.get_data_array_info(e_element_war_role_info_template_id);
				role_info.role_level = first_role_it.get_data_array_info(e_element_war_role_info_role_level);
				role_info.score_num = first_role_it.get_data_array_info(e_element_war_role_info_score_num);
				role_info.total_join_num = first_role_it.get_data_array_info(e_element_war_role_info_total_join_num);
				role_info.total_win_num = first_role_it.get_data_array_info(e_element_war_role_info_total_win_num);
				role_info.last_win_num = first_role_it.get_data_array_info(e_element_war_role_info_last_win_num);
				memcpy(role_info.role_server_out_ip, first_role_it.get_server_out_ip().c_str(), max_ip_address_length);
				role_info.role_server_out_port = first_role_it.get_server_out_port();
				msg.match_info.second_role_info[i] = role_info;
				player_level += first_role_it.get_data_array_info(e_element_war_role_info_role_level);
				player_num++;
			}
		}
		msg.match_info.map_level = ((player_level + player_num - 1) / player_num);
		cross::send_msg_to_ws(guid_64(), target_server_id, e_msgindex_ws2ws_element_war_req_create_map, &msg, sizeof(msg));
		return true;
	}
	
}