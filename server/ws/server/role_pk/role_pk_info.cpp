/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-17 13:19
	
	File Name :	c_role_pk_info.cpp
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "role_pk_info.h"
#include "../client_session_mgr.hpp"
#include "ws_client.hpp"
#include "world_server.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "utility/guid_gen.h"
#include "utility/cs_date.hpp"
#include "utility/globle_data.h"
#include "utility/init_unit.h"
#include "template/ServerTemplateHead.h"
#include "template/template_manager.h"
#include <role_pk_msg.hpp>
#include <mail_msg.hpp>
#include "../mail/mail_event_ws.h"
#include "role_pk_manager.h"
#include "cross.pb.h"
#include "net.pb.h"

//////////////////////////////////////////////////////////////////////////
//
//	Class Implement
//
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	c_role_pk_info::c_role_pk_info()
	{
		m_pk_state = e_pk_state_no_join;
		m_in_game = false;
		m_cur_war_map.clear_data();
		m_leave_game_time = 0;
	}

	c_role_pk_info::~c_role_pk_info()
	{
	}

	void c_role_pk_info::heart_tick(float deltaseconds)
	{
		refresh_honor();
	}

	void c_role_pk_info::refresh_honor()
	{
		tm* now_time = time_helper::get_localtime();
		if (now_time)
		{
			int32 now_day = now_time->tm_year * 10000 + now_time->tm_mon * 100 + now_time->tm_mday;
			if (now_day >= m_role_pk_info.data_array[e_role_pk_info_refresh_time])
			{
				m_role_pk_info.data_array[e_role_pk_info_refresh_time] = now_day;
				m_role_pk_info.data_array[e_role_pk_info_join_num] = 0;
				server2dp_proto_ws2dp_save_role_pk_honor_refresh msg;
				msg.set_role_guid(get_role_guid().server_64);
				msg.set_refresh_time(now_day);
				ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_role_pk_honor_refresh);

				//ws2dp_save_role_pk_honor_refresh dp_msg;
				//dp_msg.role_guid = get_role_guid();
				//dp_msg.refresh_time= now_day;
				//ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));
			}
		}
	}

	bool c_role_pk_info::is_remove()
	{
		if (m_in_game)
		{
			return false;
		}
		if (time_helper::get_cur_time_new().second - m_leave_game_time > ROLE_PK_WAR_TIME)
		{
			return true;
		}
		return false;
	}

	void c_role_pk_info::set_role_pk_info(s_role_pk_info& pk_info)
	{
		memcpy(&m_role_pk_info, &pk_info, sizeof(m_role_pk_info));
		refresh_honor();
	}

	const int32 c_role_pk_info::get_role_pk_info(e_role_pk_info info_index) const
	{
		if (info_index >= e_role_pk_info_max)
		{
			return 0;
		}
		return m_role_pk_info.data_array[info_index];
	}

	void c_role_pk_info::set_role_pk_info(e_role_pk_info info_index, int32 info_value)
	{
		if (info_index >= e_role_pk_info_max)
		{
			return;
		}
		m_role_pk_info.data_array[info_index] = info_value;
	}

	void c_role_pk_info::game_win(const guid_64 tar_team[PK_RANKING_TEAM_NUM])
	{
		int32 old_score = get_role_pk_info(e_role_pk_info_ladder_score);
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
		{
			return;
		}
		old_score += ladder_template_ptr->WinScore;
		set_role_pk_info(e_role_pk_info_ladder_score, old_score);

		int32 old_war_num = get_role_pk_info(e_role_pk_info_war_num);
		++old_war_num;
		set_role_pk_info(e_role_pk_info_war_num, old_war_num);

		int32 old_win_num = get_role_pk_info(e_role_pk_info_win_num);
		++old_win_num;
		set_role_pk_info(e_role_pk_info_win_num, old_win_num);
		int32 old_last_win = get_role_pk_info(e_role_pk_info_last_win);
		++old_last_win;
		set_role_pk_info(e_role_pk_info_last_win, old_last_win);
		
		game_settle_common(e_map_war_result_win, tar_team);
	}

	void c_role_pk_info::game_lost(const guid_64 tar_team[PK_RANKING_TEAM_NUM])
	{
		int32 old_score = get_role_pk_info(e_role_pk_info_ladder_score);
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
		{
			return;
		}
		//old_score -= ladder_template_ptr->LostScore;
		if (old_score < 0)
		{
			old_score = 0;
		}
		set_role_pk_info(e_role_pk_info_ladder_score, old_score);
		int32 old_war_num = get_role_pk_info(e_role_pk_info_war_num);
		++old_war_num;
		set_role_pk_info(e_role_pk_info_war_num, old_war_num);
		set_role_pk_info(e_role_pk_info_last_win, 0);

		game_settle_common(e_map_war_result_lost, tar_team);
	}

	void c_role_pk_info::game_peace(const guid_64 tar_team[PK_RANKING_TEAM_NUM])
	{
		int32 old_score = get_role_pk_info(e_role_pk_info_ladder_score);
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
		{
			return;
		}
		int32 old_war_num = get_role_pk_info(e_role_pk_info_war_num);
		++old_war_num;
		set_role_pk_info(e_role_pk_info_war_num, old_war_num);
		set_role_pk_info(e_role_pk_info_last_win, 0);

		game_settle_common(e_map_war_result_peace, tar_team);
	}

	void c_role_pk_info::game_settle_common(e_map_war_result fight_ret, const guid_64 tar_team[PK_RANKING_TEAM_NUM])
	{
		int32 old_score = get_role_pk_info(e_role_pk_info_ladder_score);
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(old_score);
		if (nullptr == ladder_template_ptr)
		{
			return;
		}

		int32 old_join_num = get_role_pk_info(e_role_pk_info_join_num);
		int32 got_honor = 0;

		if (old_join_num < GAMECONFIG->CrossDailyHonorPointsGetTime)
		{
			++old_join_num;
			set_role_pk_info(e_role_pk_info_join_num, old_join_num);
			//got_honor = ladder_template_ptr->WinHonor;
		}
		if (got_honor > 0)
		{
			std::string title;
			std::string contenttext;
			std::vector<s_item_template_info> drop_item_list;
			//std::vector<int32>& rwd_data = m_brokensky_config_ptr->ParamIntArr3;
			//globle_data::get_instance().get_item_list_by_rank_rwd(player_rank, rwd_data, drop_item_list);
			mail_ws_mgr::get_instance().send_mail_system(m_role_pk_info.role_guid, 0, drop_item_list, title, contenttext, e_money_type_cross_honor, got_honor);
		}
		
		int32 got_score = 0;
		switch (fight_ret)
		{
		case e_map_war_result_win:
			got_score = ladder_template_ptr->WinScore;
			break;
		case e_map_war_result_lost:
			//got_score = ladder_template_ptr->LostScore;
			break;
		case e_map_war_result_peace:
			break;
		default:
			break;
		}

		c_role_pk_info* role_pk_info_1 = role_pk_manager::getInstance().get_role_pk_info(tar_team[0]);
		c_role_pk_info* role_pk_info_2 = role_pk_manager::getInstance().get_role_pk_info(tar_team[0]);
		c_role_pk_info* role_pk_info_3 = role_pk_manager::getInstance().get_role_pk_info(tar_team[0]);


		server2dp_proto_ws2dp_save_role_pk_info dp_msg;
		dp_msg.set_role_guid(get_role_guid().server_64);
		dp_msg.set_ladder_score(got_score);
		dp_msg.set_win_result(fight_ret);
		dp_msg.set_cross_honor(got_honor);
		if (nullptr != role_pk_info_1)
		{
			dp_msg.set_target_name_1(role_pk_info_1->get_role_name());
		}
		if (nullptr != role_pk_info_2)
		{
			dp_msg.set_target_name_2(role_pk_info_2->get_role_name());
		}
		if (nullptr != role_pk_info_3)
		{
			dp_msg.set_target_name_3(role_pk_info_3->get_role_name());
		}
		ws_client::getInstance().send_to_dp_new(&dp_msg, e_msg_ws2dp_save_role_pk_info);
		//ws2dp_save_role_pk_info dp_msg;
		//dp_msg.role_guid = get_role_guid();
		//dp_msg.cross_war_db.ladder_score = got_score;
		//dp_msg.cross_war_db.win_result = fight_ret;
		//dp_msg.cross_war_db.cross_honor = got_honor;
		//if (role_pk_info_1)
		//{
		//	memcpy(dp_msg.cross_war_db.target_name_1, role_pk_info_1->get_role_name(), max_name_size);
		//}
		//if (role_pk_info_2)
		//{
		//	memcpy(dp_msg.cross_war_db.target_name_2, role_pk_info_2->get_role_name(), max_name_size);
		//}
		//if (role_pk_info_3)
		//{
		//	memcpy(dp_msg.cross_war_db.target_name_3, role_pk_info_3->get_role_name(), max_name_size);
		//}
		//ws_client::getInstance().send_to_dp(&dp_msg, sizeof(dp_msg));

		client_session* session_ptr = client_session_mgr::getInstance().get_session(get_role_guid());
		if (nullptr == session_ptr)
		{
			return;
		}
		cross_proto_war_result war_result_msg;
		war_result_msg.set_ladder_score(ladder_template_ptr->WinScore);
		war_result_msg.set_cross_honor(got_honor);
		war_result_msg.set_war_result(fight_ret);
		session_ptr->send_to_client(&war_result_msg, e_msgindex_s2c_cross_war_result);
	}

	void c_role_pk_info::set_role_name(const xstring& role_name)
	{
		memset(m_role_pk_info.role_name, 0, sizeof(role_name));
		memcpy(m_role_pk_info.role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
	}
}