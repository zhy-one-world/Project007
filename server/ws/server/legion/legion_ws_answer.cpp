#include "legion_ws_answer.h"
#include "../client_session_mgr.hpp"
#include "world_server.hpp"
#include <legion_msg.hpp>
#include "Logic/time_def.hpp"
#include "time.hpp"
#include "template/template_manager.h"
#include "Logic/activity_def.hpp"
#include "legion_ws_mgr.h"
#include "../mail/event_ws_mgr.h"
#include "utility/init_unit.h"
#include "net.pb.h"

namespace faith
{
	legion_ws_answer::legion_ws_answer()
	{
		clear_data();
	}

	legion_ws_answer::~legion_ws_answer()
	{

	}

	void legion_ws_answer::tick(const int64& new_time)
	{
		if (is_in_legion_answer_read_time())
		{
			check_prepare_notice();	//发送活动准备公告
		}
		if (m_legion_answer_activity_begin == false && is_in_legion_answer_time())//初始化军团数据
		{
			m_legion_answer_activity_begin = true;
			m_legion_answer_activity_begin_time = time_helper::get_cur_time_new().second;;
			legion_ws_map& legion_map = legion_ws_mgr::get_instance().get_legion_map();
			legion_ws_map_it ite;
			for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
			{
				legion_ws& legion_ws_ref = ite->second;
				s_legion_answer_info* answer_info = legion_ws_ref.get_legion_answer_info();
				answer_info->clear_data();
				m_legion_answer_rank_list.add_score_record(legion_ws_ref.get_legion_guid(), legion_ws_ref.get_legion_name(), 0);
			}
			m_answer_index = 0;
			refresh_question_answer();
		}
		if (is_in_legion_answer_time() && m_legion_answer_activity_end_time == 0)
		{
			if (new_time >= m_next_answer_time )
			{
				refresh_question_answer();
			}
			if (new_time > m_next_rank_list_refresh_time)
			{
				m_next_rank_list_refresh_time = new_time + (3 * second_tick_time);
				send_legion_answer_rank_list_all();
			}
		}
		if (m_legion_answer_activity_end_time > 0 && is_in_legion_answer_time() == false)
		{
			clear_data();
		}
	}

	//清理数据
	void legion_ws_answer::clear_data()
	{
		m_legion_answer_rank_list.clear_data();
		m_legion_answer_activity_begin_time = 0;
		m_legion_answer_activity_begin = false;
		m_notice_index = 0;
		m_answer_index = 0;
		m_next_answer_time = 0;
		m_topic_id = 0;
		m_answer_id = 0;
		m_old_answer.clear();
		m_next_rank_list_refresh_time = 0;
		m_legion_answer_activity_end_time = 0;
	}

	void legion_ws_answer::refresh_question_answer()
	{
		if (m_answer_index >= max_legion_answer_num)
		{
			//如果军团题目索引已经是最后一题了就结束
			legion_answer_end();
			return;
		}

		int32 tbale_size = template_manager::get_instance().get_template_row_size(e_QuestionBankTemplate);
		std::vector<int32> temp_vec;
		for (int32 i = 0; i < tbale_size; ++i)
		{
			if (std::count(m_old_answer.begin(), m_old_answer.end(), i) > 0)
			{
				continue;
			}
			temp_vec.push_back(i);
		}
		if (temp_vec.size() <= 0)
		{
			return;
		}

		int32 rand_index = rand() % temp_vec.size();

		QuestionBankTemplate* question_template_ptr = GET_TEMPLATE(QuestionBankTemplate, first_legion_answer_template_id + temp_vec[rand_index]);
		if (nullptr == question_template_ptr)
		{
			return;
		}
		++m_answer_index;								//题目索引加一
		m_old_answer.push_back(temp_vec[rand_index]);	//将题目加到题目类表中
		//记录新题和答案
		int32 answer_id = m_answer_id;
		m_topic_id = question_template_ptr->attribute_id;
		m_answer_id = question_template_ptr->answer;
		int64 _time = time_helper::get_cur_time_new().millisecond;
		//计算下次刷新题目的时间
		m_next_answer_time = _time + legion_answer_interval_time;
		//向客户端发送刷新题目
		legion_proto_get_legion_answer_question_info_end msg;
		msg.set_question_template_id(question_template_ptr->attribute_id);
		msg.set_time_left(legion_answer_interval_time / second_tick_time);
		msg.set_question_index(m_answer_index);
		msg.set_is_answer(false);
		legion_ws_map& legion_map = legion_ws_mgr::get_instance().get_legion_map();
		legion_ws_map_it ite;
		for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
		{
			legion_ws& legion_ws_ref = ite->second;
			s_legion_answer_info* answer_info = legion_ws_ref.get_legion_answer_info();	
			//清除本题数据
			if (answer_info->is_answer == false && m_answer_index > 1)
			{
				notice_on_answer_failure_notice(legion_ws_ref.get_legion_guid(), answer_id);
			}
			answer_info->is_answer = false;
			answer_info->the_answer_play_num = 0;
			for (int32 i = 0; i < max_legion_member_num; ++i)
			{
				answer_info->the_answer_play_guid[i].clear_data();
			}
			//更新题目
			msg.set_succeed_num(answer_info->answer_successfully_num);
			legion_ws_ref.send_message_to_all_member(&msg, e_msgindex_s2c_get_legion_answer_question_info_end);

			legion_proto_legion_answer_award_refresh req;
			req.set_award_num(answer_info->the_answer_play_num);
			legion_ws_ref.send_message_to_all_member(&req, e_mgsindex_s2c_send_legion_answer_box_reward);

		}
	}

	//是否在活动期间
	bool legion_ws_answer::is_in_legion_answer_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_legion_answer, e_activity_time_get_gaming);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	//是否在活动准备期间
	bool legion_ws_answer::is_in_legion_answer_read_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_legion_answer, e_activity_time_get_ready);
		if (act_left < 0)
		{
			return false;
		}
		return true;
	}

	bool legion_ws_answer::is_in_legion_answer_all_time()
	{
		int32 act_left = world_server::getInstance().get_activity_sec_left(e_activity_type_legion_answer, e_activity_time_get_all);
		if (act_left < 0)
		{
			return false;
		}
		return true; return false;
	}

	//回答问题判断
	void legion_ws_answer::answer_legion_question(uint64 role_guid, std::vector<xstring> answer_vec)
	{
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);

		if (nullptr == player_session)
		{
			return;
		}

		if (!player_session->is_self_server())
		{
			return;
		}//跨服不答题

		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(player_session->get_legion_guid());
		if (nullptr == legion_ptr)
		{
			return;
		}
		s_legion_answer_info* answer_info = legion_ptr->get_legion_answer_info();
		if (nullptr == answer_info)
		{
			return;
		}


		if (is_in_legion_answer_time() == false)
		{
			return;
		}

		std::vector<std::string> temp_array_list = template_manager::get_instance().get_string_list(m_answer_id);
		if (temp_array_list.empty())
		{
			return;
		}
		xstring answer_content = "";
		for (auto iter = answer_vec.begin(); iter != answer_vec.end(); ++iter)
		{
			answer_content += *iter;
		}

		//比较答案
		if (std::count(temp_array_list.begin(), temp_array_list.end(), answer_content) <= 0)
		{
			return;
		}
		answer_correctly(role_guid);

		legion_proto_legion_answer_result result_msg;
		player_session->send_to_client(&result_msg, e_msgindex_s2c_legion_answer_result);
	}

	//发送准备和开始公告
	void legion_ws_answer::notice_on_legion_prepare_answer(uint32 prepare_time)
	{
		int32 notice_id = 0;
		std::vector<std::string> vec_str_up_item;
		if ( 0 == prepare_time)
		{
			notice_id = start_legion_answer_notice_id;
			vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		}
		else
		{
			notice_id = prepare_legion_answer_notice_id;
			vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			xstring prepare_time_str = init_unit::change_i32_to_string(prepare_time);
			vec_str_up_item.push_back(prepare_time_str);
		}

		std::string notice_str = init_unit::implode(vec_str_up_item);
		int32 create_time = time_helper::get_cur_time_new().second;

		legion_ws_map& all_legions = legion_ws_mgr::get_instance().get_legion_map();
		for ( auto iter = all_legions.begin(); iter != all_legions.end(); ++iter)
		{
			legion_ws& legion_ws_ref = iter->second;
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, legion_ws_ref.get_legion_guid(), notice_str);
		}
	}

	//活动军团答题信息
	void legion_ws_answer::get_legion_question_info(guid_64 role_guid)
	{
		if (m_legion_answer_activity_begin_time <= 0)
		{
			return;
		}
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
		{
			return;
		}
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(player_session->get_legion_guid());
		if (nullptr == legion_ptr)
		{
			return;
		}
		s_legion_answer_info* answer_info = legion_ptr->get_legion_answer_info();
		if (nullptr == answer_info)
		{
			return;
		}

		legion_proto_get_legion_answer_question_info_end msg;
		msg.set_question_template_id(m_topic_id);
		int64 now_stamp = time_helper::get_cur_time_new().millisecond;
		msg.set_time_left((m_next_answer_time - now_stamp) / second_tick_time);
		msg.set_question_index(m_answer_index);
		msg.set_succeed_num(answer_info->answer_successfully_num);

		msg.set_is_answer(answer_info->is_answer);
		if (answer_info->is_answer)
		{
			if (m_answer_index > 0 && m_answer_index <= max_legion_answer_num)
			{
				s_legion_member_info* player_info = legion_ptr->get_member(answer_info->play_guid[m_answer_index - 1]);
				if (nullptr != player_info)
				{
					msg.set_play_name(player_info->role_name);
				}
			}
		}
		player_session->send_to_client(&msg, e_msgindex_s2c_get_legion_answer_question_info_end);

		legion_proto_legion_answer_award_refresh req;
		req.set_award_num(answer_info->the_answer_play_num);
		player_session->send_to_client(&req, e_mgsindex_s2c_send_legion_answer_box_reward);
	}

	void legion_ws_answer::send_legion_question_info_by_legion(guid_64 legion_guid)
	{
		if (m_legion_answer_activity_begin_time <= 0)
		{
			return;
		}
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ptr)
		{
			return;
		}
		s_legion_answer_info* answer_info = legion_ptr->get_legion_answer_info();
		if (nullptr == answer_info)
		{
			return;
		}
		legion_proto_get_legion_answer_question_info_end msg;
		msg.set_question_template_id(m_topic_id);
		int64 now_stamp = time_helper::get_cur_time_new().millisecond;
		msg.set_time_left((m_next_answer_time - now_stamp) / second_tick_time);
		msg.set_question_index(m_answer_index);
		msg.set_succeed_num(answer_info->answer_successfully_num);
		msg.set_is_answer(answer_info->is_answer);
		if (answer_info->is_answer)
		{
			if (m_answer_index > 0 && m_answer_index <= max_legion_answer_num)
			{
				s_legion_member_info* player_info = legion_ptr->get_member(answer_info->play_guid[m_answer_index - 1]);
				if (nullptr != player_info)
				{
					msg.set_play_name(player_info->role_name);
				}
			}
		}
		legion_ptr->send_message_to_all_member(&msg, e_msgindex_s2c_get_legion_answer_question_info_end);
	}

	void legion_ws_answer::answer_correctly(guid_64 role_guid)
	{
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
		{
			return;
		}

		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(player_session->get_legion_guid());
		if (nullptr == legion_ptr)
		{
			return;
		}

		s_legion_answer_info* answer_info = legion_ptr->get_legion_answer_info();
		if (nullptr == answer_info)
		{
			return;
		}

		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_answer);
		if (nullptr == config_ptr)
		{
			return;
		}

		for (int32 i = 0; i < max_legion_member_num; ++i)
		{
			if (answer_info->the_answer_play_guid[i] == role_guid)
			{
				return;
			}
			if (false == answer_info->the_answer_play_guid[i].is_valid())
			{
				answer_info->the_answer_play_guid[i] = role_guid;
				answer_info->the_answer_play_num++;
				break;
			}
		}
		//发送可以获得奖励的玩家列表
		ws2cs_legion_answer_question_right msg;
		msg.role_guid = role_guid;
		msg.question_template_id = m_topic_id;
		msg.is_answer = false;
		
		//发送回答正确公告
		if (answer_info->is_answer == false && m_answer_index > 0 && m_answer_index - 1 < max_legion_answer_num)
		{
			answer_info->play_guid[m_answer_index - 1] = role_guid;
			++answer_info->answer_successfully_num;
			answer_info->is_answer = true;
			msg.is_answer = true;
			notice_on_first_answer_true_notice(player_session->get_legion_guid(), player_session->get_role_name(), m_answer_id);
			send_legion_question_info_by_legion(player_session->get_legion_guid());//返回当前答题进度单信息

			for (int32 i = 0; i < config_ptr->ParamIntArr1.size(); ++i)
			{
				if (answer_info->get_play_answer_num(role_guid) == config_ptr->ParamIntArr1[i])
				{
					notice_on_play_answer_num_notice(player_session->get_legion_guid(), player_session->get_role_name(), answer_info->get_play_answer_num(role_guid));
				}
			}
		}

		//更新排行榜
		int64 add_rank = m_legion_answer_rank_list.add_score(legion_ptr->get_legion_guid(), 1);
		if (add_rank == -1)
		{
			m_legion_answer_rank_list.add_score_record(legion_ptr->get_legion_guid(), legion_ptr->get_legion_name(), 0);
			m_legion_answer_rank_list.add_score(legion_ptr->get_legion_guid(), 1);
		}
		msg.data_num = 0;
		std::list<s_legion_member_info>& member_list = legion_ptr->get_member_list();
		for (auto iter = member_list.begin(); iter != member_list.end(); ++iter)
		{
			client_session* member_session = client_session_mgr::getInstance().get_session(iter->role_guid);
			if (nullptr == member_session)
			{
				continue;
			}
			if (msg.data_num < max_legion_member_num)
			{
				msg.legion_play_guid[msg.data_num] = iter->role_guid;
				msg.data_num++;
			}
		}
		bool is_use = proto_by_lua(e_msgindex_ws2cs_legion_answer_question_right);
		if (is_use == false)
		{
			player_session->send_to_cs(&msg, sizeof(msg));
		}
		else
		{
			faith::ws2cs_proto::legion_answer_question_right pro_msg;
			msg.to_proto(pro_msg);
			player_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_legion_answer_question_right);
		}


		legion_proto_legion_answer_award_refresh req;
		req.set_award_num(answer_info->the_answer_play_num);
		legion_ptr->send_message_to_all_member(&req, e_mgsindex_s2c_send_legion_answer_box_reward);	
	}


	//发送抢答成功的公告
	void legion_ws_answer::notice_on_first_answer_true_notice(guid_64 legion_guid, const xstring& role_name, int32 answer_id)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		int32 notice_str_id = first_answer_true_notice_id;
		std::vector<std::string> notice_str_params;
		int32 create_time = time_helper::get_cur_time_new().second;
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
		notice_str_params.push_back(role_name);
		notice_str_params.push_back(template_manager::get_instance().get_all_string_context(answer_id));
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, legion_guid, award_notice_str);
	}

	//发送无人抢答成功的公告
	void legion_ws_answer::notice_on_answer_failure_notice(guid_64 legion_guid, int32 answer_id)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		int32 notice_str_id = answer_failure_notice_id;
		std::vector<std::string> notice_str_params;
		int32 create_time = time_helper::get_cur_time_new().second;
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
		notice_str_params.push_back(template_manager::get_instance().get_all_string_context(answer_id));
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, legion_guid, award_notice_str);
	}

	//发送抢答到指定数量的公告
	void legion_ws_answer::notice_on_play_answer_num_notice(guid_64 legion_guid, const xstring& role_name, int32 question_num)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}
		int32 notice_str_id = play_answer_num_notice_id;
		std::vector<std::string> notice_str_params;
		int32 create_time = time_helper::get_cur_time_new().second;
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
		notice_str_params.push_back(role_name);
		notice_str_params.push_back(template_manager::get_instance().int_to_string(question_num));
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, legion_guid, award_notice_str);
	}

	//发送本军团答题结束的公告
	void legion_ws_answer::notice_on_legion_answer_end_notice(guid_64 legion_guid, const xstring& role_name, int32 question_num)
	{
		if (legion_guid.is_valid() == false)
		{
			return;
		}		
		int32 notice_str_id = legion_answer_end_notice_id;
		std::vector<std::string> notice_str_params;
		int32 create_time = time_helper::get_cur_time_new().second;
		int32 minute_time = 0;
		if ((create_time - m_legion_answer_activity_begin_time) > 60)
		{
			minute_time = (create_time - m_legion_answer_activity_begin_time) / 60;
		}
		int32 second_time = 0;
		if ((create_time - m_legion_answer_activity_begin_time) > 0)
		{
			second_time = (create_time - m_legion_answer_activity_begin_time) % 60;
		}
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
		notice_str_params.push_back(template_manager::get_instance().int_to_string(minute_time));
		notice_str_params.push_back(template_manager::get_instance().int_to_string(second_time));
		notice_str_params.push_back(template_manager::get_instance().int_to_string(question_num));
		notice_str_params.push_back(role_name);
		std::string award_notice_str = init_unit::implode(notice_str_params);
		event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, legion_guid, award_notice_str);
	}

	//发送活动结束的全服公告
	void legion_ws_answer::send_activity_end_notice()
	{
		int32 notice_str_id = 0;
		int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> legion_name;
		std::vector<std::string> notice_str_params;
		std::vector<score_record> rank_list = m_legion_answer_rank_list.get_score_record_list_all();
		for (int32 i = 0; i < rank_list.size(); ++i)
		{
			if (i <= rank_show_num)
			{
				if (rank_list[i].score > 0)//只要完成1道题以上进入排名
				{
					notice_str_id = all_legion_answer_end_one_play_notice_id + i;
					legion_name.push_back(rank_list[i].role_name);
				}
			}
		}
		
		if (notice_str_id == 0 || legion_name.size() <= 0)
		{
			return;
		}
		notice_str_params.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_str_id));
		for (int32 i = 0; i < legion_name.size(); ++i)
		{
			notice_str_params.push_back(legion_name[i]);
		}
		std::string award_notice_str = init_unit::implode(notice_str_params);

		legion_ws_map& all_legions = legion_ws_mgr::get_instance().get_legion_map();
		for (auto iter = all_legions.begin(); iter != all_legions.end(); ++iter)
		{
			legion_ws& legion_ws_ref = iter->second;
			event_ws_mgr::get_instance().send_notice_to_all(notice_str_id, create_time, legion_ws_ref.get_legion_guid(), award_notice_str);
		}
	}
	
	//本军团答题结束
	void legion_ws_answer::legion_answer_end()
	{
		legion_ws_map& legion_map = legion_ws_mgr::get_instance().get_legion_map();
		legion_ws_map_it ite;
		for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
		{
			legion_ws& legion_ptr = ite->second;
			s_legion_answer_info* answer_info = legion_ptr.get_legion_answer_info();
			if (nullptr == answer_info)
			{
				return;
			}

			//发送本次军团答题结束公告
			const s_legion_member_info* play_info = legion_ptr.get_member(answer_info->get_max_num_play());
			if (play_info != nullptr)
			{
				xstring role_name = play_info->role_name;
				notice_on_legion_answer_end_notice(legion_ptr.get_legion_guid(), role_name, answer_info->answer_successfully_num);
			}
			legion_proto_legion_answer_end msg;
			legion_ptr.send_message_to_all_member(&msg, e_msgindex_s2c_legion_answer_end);
		}
		//记录结束时间
		//answer_info->answer_end_time = time_helper::get_cur_time_new().second;
		send_activity_end_notice();
		//clear_data();
		m_legion_answer_activity_end_time = time_helper::get_cur_time_new().second;
	}

	//发送排名列表给单个人
	void legion_ws_answer::send_legion_answer_rank_list(guid_64 role_guid)
	{
		client_session* play_ptr = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == play_ptr)
		{
			return;
		}
		if (play_ptr->get_legion_guid().is_valid() == false)
		{
			return;
		}
		legion_ws* _legion_ptr = legion_ws_mgr::get_instance().get_legion(play_ptr->get_legion_guid());
		if (nullptr == _legion_ptr)
		{
			return;
		}
		legion_proto_legion_answer_info_list msg;
		m_legion_answer_rank_list.sort();
		std::vector<score_record> score_info = m_legion_answer_rank_list.get_score_record_list_all();
		for (int32 i = 0; i < score_info.size(); ++i)
		{
			if (i < rank_show_num)
			{
				msg.add_legion_name(score_info[i].role_name);
				msg.add_score(score_info[i].score);
			}
			if (score_info[i].role_guid == _legion_ptr->get_legion_guid())
			{
				msg.set_m_rank(i + 1);
				msg.set_m_score(score_info[i].score);
			}
		}
		play_ptr->send_to_client(&msg, e_msgindex_s2c_send_legion_answer_rank_list);
	}

	//发送排名列表给全部军团
	void legion_ws_answer::send_legion_answer_rank_list_all()
	{
		m_legion_answer_rank_list.sort();
		legion_ws_map& legion_map = legion_ws_mgr::get_instance().get_legion_map();
		legion_ws_map_it ite;
		for (ite = legion_map.begin(); ite != legion_map.end(); ++ite)
		{
			legion_ws& _legion_ptr = ite->second;
			legion_proto_legion_answer_info_list msg;
			std::vector<score_record> score_info = m_legion_answer_rank_list.get_score_record_list_all();
			for (int32 i = 0; i < score_info.size(); ++i)
			{
				if (i < rank_show_num)
				{
					msg.add_legion_name(score_info[i].role_name);
					msg.add_score(score_info[i].score);
				}
				if (score_info[i].role_guid == _legion_ptr.get_legion_guid())
				{
					msg.set_m_rank(i + 1);
					msg.set_m_score(score_info[i].score);
				}
			}
			_legion_ptr.send_message_to_all_member(&msg, e_msgindex_s2c_send_legion_answer_rank_list);
		}
	}

	//发送准备公告
	void legion_ws_answer::check_prepare_notice()
	{
		int32 prepare_time = world_server::getInstance().get_activity_sec_left(e_activity_type_legion_answer, e_activity_time_get_ready);
		if (prepare_time <= legion_answer_fisrt_notice_time && prepare_time > legion_answer_second_notice_time && m_notice_index == 0)
		{
			m_notice_index = 1;
			notice_on_legion_prepare_answer(legion_answer_fisrt_notice_time);
		}
		if (prepare_time <= legion_answer_second_notice_time && prepare_time > legion_answer_third_notice_time && m_notice_index == 1)
		{
			m_notice_index = 2;
			notice_on_legion_prepare_answer(legion_answer_second_notice_time);
		}
		if (prepare_time <= legion_answer_third_notice_time && prepare_time > 0 && m_notice_index == 2)
		{
			m_notice_index = 3;
			notice_on_legion_prepare_answer(legion_answer_third_notice_time);
		}
		if (prepare_time == 0 && m_notice_index == 3)
		{
			m_notice_index = 4;
			notice_on_legion_prepare_answer(0);
		}
	}

}
