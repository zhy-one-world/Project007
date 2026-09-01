#include "cloud_shop_mgr.h"

#include "Logic/time_def.hpp"

#include "../ranking/ranking_mgr_ws.h"
#include <cloud_shop_msg.hpp> 
#include "utility/random.h"
#include "../mail/event_ws_mgr.h"
#include "world_server.hpp"
#include "cross_group/gate_proxy.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "../activity/time_limit_activity_temp_ws_mgr.h"
#include "../activity/time_limit_activity_ws_mgr.h"
#include "globle_data.h"
#include "cloud_shop.pb.h"
#include "server/mail/mail_event_ws.h"
#include "net.pb.h"

namespace faith
{
	cloud_shop_mgr::cloud_shop_mgr()
	{
		clear_data();
	}

	void cloud_shop_mgr::clear_data()
	{	
		m_cloud_shop_info_map.clear();
		cloud_shop_one_player_info_map temp_cloud_shop_player_info_map;
		cloud_shop_one_player_info_map temp_cross_cloud_shop_player_info_map;
		m_cloud_shop_info_map.insert({ e_time_limit_activity_type_cloud_shop, temp_cloud_shop_player_info_map });
		m_cloud_shop_info_map.insert({ e_time_limit_activity_type_cross_cloud_shop, temp_cross_cloud_shop_player_info_map });
	}

	void cloud_shop_mgr::init_manager()
	{
		clear_data();
		load_cloud_shop_players_info();
	}

	void cloud_shop_mgr::load_cloud_shop_players_info()
	{
		//从数据库获取玩家关于此活动的数据
		ws2dp_load_cloud_shop_players_info req_roles;
		ws_client::getInstance().send_to_dp(&req_roles, sizeof(req_roles));
	}

	void cloud_shop_mgr::load_role_record_data(const cloud_shop_role_record_data* load_data, int32 load_num, bool is_end)
	{
		for (int32 i = 0; i < load_num; i++)
		{
			switch (load_data[i].get_data(e_cloud_shop_role_info_activity_type))
			{
			case e_time_limit_activity_type_cloud_shop:
			{
				m_cloud_shop_info_map[e_time_limit_activity_type_cloud_shop].insert({ load_data[i].role_guid, load_data[i] });
			}
				break;
			case e_time_limit_activity_type_cross_cloud_shop:
			{
				m_cloud_shop_info_map[e_time_limit_activity_type_cross_cloud_shop].insert({ load_data[i].role_guid, load_data[i] });
			}
				break;
			default:
				break;
			}
		}
		if (is_end)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_load_cloud_shop_info);
		}
	}

	void cloud_shop_mgr::tick(const int64& new_time)
	{
		static int64 last_tick_time = new_time;
		static float update_internal = 0;
		float elapse_sec = (float)(new_time - last_tick_time) / second_tick_time;
		last_tick_time = new_time;

		update_internal += elapse_sec;
		if (update_internal >= 1)
		{
			check_state_need_change_by_time(new_time);
			update_internal -= 1;
		}
	}

	void cloud_shop_mgr::recv_other_ws_buy_msg(guid_64 role_guid, int32 server_id, int32 req_type, const xstring& role_name, int32 class_type, int32 sender_server_id)
	{
		e_time_limit_activity_type activity_type = time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server();
		switch (req_type)
		{
		case faith::e_client_req_type_info:
		{
			cloud_shop_proto_cloud_shop_resp resp_msg;
			get_activity_info(activity_type, resp_msg, role_guid);
			client_session_mgr::getInstance().send_msg_to_cross_player(&resp_msg, sender_server_id, role_guid, e_msgindex_s2c_cloud_shop_resp);
		}
		break;
		case faith::e_client_req_type_buy:
		case faith::e_client_req_type_robber:
		{	
			if (is_in_sell_time((e_client_req_type)req_type, activity_type))
			{
				player_buy(activity_type, role_guid, role_name, class_type, req_type, server_id, sender_server_id);
			}
			else
			{
				cloud_shop_proto_cloud_shop_resp resp_msg;
				get_activity_info(activity_type, resp_msg, role_guid);
				resp_msg.set_req_type(req_type);
				resp_msg.set_error_id(e_cloud_shop_communicate_error_define_state_error);
				client_session_mgr::getInstance().send_msg_to_cross_player(&resp_msg, sender_server_id, role_guid, e_msgindex_s2c_cloud_shop_resp);
			}
		}
		break;
		default:
			break;
		}
	}

	void cloud_shop_mgr::handler_client_request(client_session* const pSession, e_client_req_type req_type)
	{
		if (pSession == nullptr) 
		{
			return;
		}
		guid_64 role_guid = pSession->get_role_guid();

		if (!pSession->is_self_server())
		{
			return;
		}

		//开跨服后只存在跨服云购 不走原逻辑
		if (world_server::getInstance().get_cross_id() > 0)
		{
			ws2gws_send_bug_cloud cross_msg;
			cross_msg.sender_server_id = world_server::getInstance().get_server_id();
			cross_msg.req_type = req_type;
			cross_msg.role_guid = pSession->get_role_guid();
			cross_msg.server_id = pSession->get_role_info_data(e_role_info_server_id);
			cross_msg.set_role_name(pSession->get_role_name());
			cross_msg.role_class_type = pSession->get_role_info_data(e_role_info_class_type);
			cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2gws_send_buy_cloud, &cross_msg, sizeof(cross_msg));
		}
		else
		{
			e_time_limit_activity_type activity_type = time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server();
			switch (req_type)
			{
				case faith::e_client_req_type_info:
				{
					cloud_shop_proto_cloud_shop_resp resp_msg;
					get_activity_info(activity_type, resp_msg, role_guid);
					pSession->send_to_client(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
				}
					break;
				case faith::e_client_req_type_buy:
				case faith::e_client_req_type_robber:
				{
					if (is_in_sell_time((e_client_req_type)req_type, activity_type))
					{
						player_buy(activity_type, role_guid, pSession->get_role_name(), pSession->get_role_info_data(e_role_info_class_type), req_type, pSession->get_role_info_data(e_role_info_server_id));
					}
					else
					{
						cloud_shop_proto_cloud_shop_resp resp_msg;
						get_activity_info(activity_type, resp_msg, role_guid);
						resp_msg.set_req_type(req_type);
						resp_msg.set_error_id(e_cloud_shop_communicate_error_define_state_error);
						pSession->send_to_client(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
					}
				}
					break;
				default:
					break;
			}
		}
	}

	void cloud_shop_mgr::send_buy_result_to_player(const guid_64& player_uid, const int32 error_id, const int32 buy_type, int32 server_id, int32 sender_server_id)
	{
		e_time_limit_activity_type activity_type = time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server();
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return;
		}
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end())
		{
			return;
		}

		int32 final_error_id = error_id;
		if (error_id == e_cloud_shop_communicate_error_define_no_error)
		{
			const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
			if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.Condition.size() < 4)
			{
				return;
			}

			//算钱
			int32 need_cost_money_value = 0;
			int32 need_cost_money_type = -1;
			cloud_shop_ite->second[player_uid].role_guid = player_uid;

			if (buy_type == e_cloud_shop_buy_type_buy)
			{
				need_cost_money_type = time_limit_act_branch_temp.Condition[0];
				need_cost_money_value = time_limit_act_branch_temp.Condition[1];
			}
			else if (buy_type == e_cloud_shop_buy_type_robber)
			{
				need_cost_money_type = time_limit_act_branch_temp.Condition[2];
				need_cost_money_value = time_limit_act_branch_temp.Condition[3];
			}
			else
			{
				return;
			}

			//有必要进行二次判断
			if (get_left_count(activity_type) > 0)
			{
				cur_cloud_shop_activity.add_activity_info(e_time_limit_ws_act_info_schedule, 1);

				if (buy_type == e_cloud_shop_buy_type_buy)
				{
					cloud_shop_ite->second[player_uid].add_data(e_cloud_shop_role_info_buy_number, 1);
				}
				else if (buy_type == e_cloud_shop_buy_type_robber)
				{
					cloud_shop_ite->second[player_uid].add_data(e_cloud_shop_role_info_robber_number, 1);
				}
				else
				{
					return;
				}

				int32 award_index = calc_player_basic_award(activity_type);
				if (award_index >= 0)
				{
					add_basic_award_to_player(activity_type, player_uid, award_index);
				}
			}
			else
			{
				final_error_id = e_cloud_shop_communicate_error_define_not_enough_product;
			}

			//如果发现已买又数量满了，这时通过邮件退回消耗钻石
			if (final_error_id != e_cloud_shop_communicate_error_define_no_error)
			{
				std::string mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cloud_shop_big_buy_failed_title);
				std::string mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cloud_shop_big_buy_failed_context);
				std::vector<s_item_template_info> item_list;
				mail_ws_mgr::get_instance().send_mail_system(player_uid, server_id, item_list, mail_title, mail_context, need_cost_money_type, need_cost_money_value);
			}
			time_limit_activity_ws_mgr::get_instance().save_ws_time_limit_act_to_db((e_time_limit_activity_type)cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_type));
			save_activity_role_content(activity_type, player_uid);
		}
		cloud_shop_proto_cloud_shop_resp resp;
		get_activity_info(activity_type, resp, player_uid);
		resp.set_req_type(buy_type + 1);
		resp.set_error_id(final_error_id);

		if (activity_type == e_time_limit_activity_type_cross_cloud_shop)
		{
			client_session_mgr::getInstance().send_msg_to_cross_player(&resp, sender_server_id, player_uid, e_msgindex_s2c_cloud_shop_resp);
		}
		else if (activity_type == e_time_limit_activity_type_cloud_shop)
		{
			client_session *pSession = client_session_mgr::getInstance().get_session(player_uid);
			if (pSession == nullptr)
			{
				return;
			}
			pSession->send_to_client(&resp, e_msgindex_s2c_cloud_shop_resp);
		}
	}

	int32 cloud_shop_mgr::get_left_count(e_time_limit_activity_type activity_type)
	{
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return 0;
		}
		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr3.size() < 1)
		{
			return 0;
		}
		int32 left = time_limit_act_branch_temp.ParamArr3[0] - cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_schedule);
		return left > 0 ? left : 0;
	}

	void cloud_shop_mgr::get_activity_info(e_time_limit_activity_type activity_type, cloud_shop_proto_cloud_shop_resp &msg, const guid_64 &player_uid)
	{
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			msg.set_activity_state(e_cloud_shop_not_open);
			return;
		}
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end())
		{
			msg.set_activity_state(e_cloud_shop_not_open);
			return;
		}
		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr3.size() < 1)
		{
			msg.set_activity_state(e_cloud_shop_not_open);
			return;
		}
		time_info now_time = time_helper::get_cur_time_new();
		int32 left_time_to_next_step = 0;
		int32 activity_state = 0;
		if (time_limit_act_branch_temp.ParamArr5.size() > 0 && time_limit_act_branch_temp.ParamArr5[0] > 0)
		{
			if (now_time.second_in_day <= cloud_shop_award_opening_time)
			{
				activity_state = e_cloud_shop_robber;
				left_time_to_next_step = cloud_shop_award_opening_time - now_time.second_in_day;
			}
			else
			{
				activity_state = e_cloud_shop_finish;
			}
		}
		else
		{
			if (now_time.second_in_day <= cloud_shop_robbery_start_time)
			{
				activity_state = e_cloud_shop_buy;
				left_time_to_next_step = cloud_shop_robbery_start_time - now_time.second_in_day;
			}
			else if (now_time.second_in_day <= cloud_shop_award_opening_time)
			{
				activity_state = e_cloud_shop_robber;
				left_time_to_next_step = cloud_shop_award_opening_time - now_time.second_in_day;
			}
			else
			{
				activity_state = e_cloud_shop_finish;
			}
		}

		msg.set_error_id(e_cloud_shop_communicate_error_define_no_error);
		msg.set_day_index(time_limit_act_branch_temp.CycleIndex);
		msg.set_req_type(e_client_req_type_info);
		msg.set_activity_state(activity_state);
		msg.set_total_num(time_limit_act_branch_temp.ParamArr3[0]);
		msg.set_left_for_sell_num(get_left_count(activity_type));
		msg.set_left_time_to_next_step(left_time_to_next_step);
		msg.set_award_info(get_big_reward_string().c_str());
		msg.set_template_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (player_uid.is_valid())
		{
			auto cloud_shop_player_ite = cloud_shop_ite->second.find(player_uid);
			if (cloud_shop_player_ite != cloud_shop_ite->second.end())
			{
				msg.set_have_buy_count(cloud_shop_player_ite->second.get_data(e_cloud_shop_role_info_buy_number));
				msg.set_have_robber_count(cloud_shop_player_ite->second.get_data(e_cloud_shop_role_info_robber_number));
			}
		}
	}

	xstring cloud_shop_mgr::make_today_reward_string(const cloud_shop_big_rewards& big_reward_player_vec)
	{
		xstring	today_reward_info = "";
		for (int32 i = 0; i < big_reward_player_vec.size(); i++)
		{
			today_reward_info.append(boost::lexical_cast<xstring>(big_reward_player_vec[i].role_guid.server_64));
			today_reward_info.append(_XTEXT(","));
			today_reward_info.append(boost::lexical_cast<xstring>(big_reward_player_vec[i].role_name));
			today_reward_info.append(_XTEXT(","));
			today_reward_info.append(boost::lexical_cast<xstring>(big_reward_player_vec[i].big_item_id));
			today_reward_info.append(_XTEXT(","));
			today_reward_info.append(boost::lexical_cast<xstring>(big_reward_player_vec[i].big_item_count));
			today_reward_info.append(_XTEXT("|"));
		}
		return today_reward_info;
	}

	xstring cloud_shop_mgr::get_big_reward_string()
	{
		xstring reward_str = "";
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_cur_cloud_shop_activity();
		if (cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_act_guid) <= 0 || cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id) <= 0)
		{
			return reward_str;
		}
		reward_str.append(cur_cloud_shop_activity.get_activity_string_param(e_time_limit_activity_string_param_index_1));
		reward_str.append("&");
		reward_str.append(cur_cloud_shop_activity.get_activity_string_param(e_time_limit_activity_string_param_index_2));
		return reward_str;
	}

	void cloud_shop_mgr::player_buy(e_time_limit_activity_type activity_type, const guid_64& player_uid, const xstring& player_name, int32 class_type, int32 req_type, int32 server_id, int32 sender_server_id)
	{
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return;
		}
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end())
		{
			return;
		}

		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.Condition.size() < 4)
		{
			return;
		}
		if (get_left_count(activity_type) <= 0)
		{
			cloud_shop_proto_cloud_shop_resp resp_msg;
			get_activity_info(activity_type, resp_msg, player_uid);
			resp_msg.set_error_id(e_cloud_shop_communicate_error_define_not_enough_product);
			resp_msg.set_req_type(req_type);

			if (activity_type == e_time_limit_activity_type_cross_cloud_shop)
			{
				client_session_mgr::getInstance().send_msg_to_cross_player(&resp_msg, sender_server_id, player_uid, e_msgindex_s2c_cloud_shop_resp);
			}
			else
			{
				client_session* pSession = client_session_mgr::getInstance().get_session(player_uid);
				if (pSession != nullptr)
				{
					pSession->send_to_client(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
				}
			}
		}
		else
		{
			cloud_shop_ite->second[player_uid].role_guid = player_uid;
			cloud_shop_ite->second[player_uid].set_role_name(player_name);
			cloud_shop_ite->second[player_uid].set_data(e_cloud_shop_role_info_activity_type, activity_type);
			cloud_shop_ite->second[player_uid].set_data(e_cloud_shop_role_info_class_type, class_type);
			cloud_shop_ite->second[player_uid].set_data(e_cloud_shop_role_info_server_id, server_id);

			ws2cs_cloud_shop_buy_req req;
			req.role_guid = player_uid;
			if (req_type == e_client_req_type_buy)
			{
				req.buy_type = e_cloud_shop_buy_type_buy;
				req.have_buy_count = cloud_shop_ite->second[player_uid].get_data(e_cloud_shop_role_info_buy_number);
				req.need_money_type = time_limit_act_branch_temp.Condition[0];
				req.need_money_count = time_limit_act_branch_temp.Condition[1];
			}
			else if (req_type == e_client_req_type_robber)
			{
				req.buy_type = e_cloud_shop_buy_type_robber;
				req.have_buy_count = cloud_shop_ite->second[player_uid].get_data(e_cloud_shop_role_info_robber_number);
				req.need_money_type = time_limit_act_branch_temp.Condition[2];
				req.need_money_count = time_limit_act_branch_temp.Condition[3];
			}
			else
			{
				return;
			}

			if (activity_type == e_time_limit_activity_type_cross_cloud_shop)
			{
				cross::send_msg_to_ws(player_uid, sender_server_id, e_msgindex_gws2ws_cloud_shop_buy, &req, sizeof(req));
			}
			else
			{
				client_session* pSession = client_session_mgr::getInstance().get_session(player_uid);
				if (pSession != nullptr)
				{
					bool is_use = proto_by_lua(e_msgindex_ws2cs_cloud_shop_buy_req);
					if (is_use == false)
					{
						pSession->send_to_cs(&req, sizeof(ws2cs_cloud_shop_buy_req));
					}
					else
					{
						faith::ws2cs_proto::cloud_shop_buy_req pro_msg;
						req.to_proto(pro_msg);
						pSession->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_cloud_shop_buy_req);
					}
				}
			}
		}
	}

	void cloud_shop_mgr::send_open_msg_to_all_client()
	{
		e_time_limit_activity_type activity_type = time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server();

		if (activity_type == e_time_limit_activity_type_cloud_shop)
		{
			cloud_shop_proto_cloud_shop_resp resp_msg;
			get_activity_info(activity_type, resp_msg, guid_64());
			client_session_mgr::getInstance().send_message_to_all_client(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
			
		}
		else if (activity_type == e_time_limit_activity_type_cross_cloud_shop)
		{
			cloud_shop_proto_cloud_shop_resp resp_msg;
			get_activity_info(activity_type, resp_msg, guid_64());
			client_session_mgr::getInstance().send_message_to_all_client(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
			ws_client::getInstance().send_to_gate(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
		}

	}

	void cloud_shop_mgr::send_open_msg_to_other_server(guid_64 role_guid, int32 server_id) 
	{
		cloud_shop_proto_cloud_shop_resp resp_msg;
		get_activity_info(time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server(), resp_msg, role_guid);
		client_session_mgr::getInstance().send_msg_to_cross_player(&resp_msg, server_id, role_guid, e_msgindex_s2c_cloud_shop_resp);
	}

	void cloud_shop_mgr::send_open_msg_to_client(client_session* session_ptr)
	{
		if (nullptr == session_ptr)
		{
			return;
		}
		
		if (world_server::getInstance().get_cross_id() > 0)
		{
			ws2gws_get_cloud_shop msg;
			msg.sender_server_id = world_server::getInstance().get_server_id();
			msg.role_guid = session_ptr->get_role_guid();
			cross::send_msg_to_ws(session_ptr->get_role_guid(), world_server::getInstance().get_cross_id(), e_msgindex_ws2gws_get_cloud_shop, &msg, sizeof(msg));
		}
		else
		{
			cloud_shop_proto_cloud_shop_resp resp_msg;
			get_activity_info(time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server(), resp_msg, session_ptr->get_role_guid());
			session_ptr->send_to_client(&resp_msg, e_msgindex_s2c_cloud_shop_resp);
		}
	}

	void cloud_shop_mgr::check_state_need_change_by_time(const int64& new_time)
	{
		e_time_limit_activity_type activity_type = time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server();
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return;
		}

		time_info now_time = time_helper::get_time_by_stamp_new(new_time, true);

		if (now_time.second_in_day > cloud_shop_award_opening_time && cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_flag) == 0)
		{
			send_rewards_and_clear_info(activity_type);
		}
	}

	void cloud_shop_mgr::add_basic_award_to_player(e_time_limit_activity_type activity_type, const guid_64& player_uid, const int32 award_index)
	{
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end() || award_index < 0 || award_index >= cloud_shop_basic_award_size)
		{
			return;
		}

		cloud_shop_ite->second[player_uid].role_guid = player_uid;
		cloud_shop_ite->second[player_uid].add_data(e_cloud_shop_role_info_basic_award_count1 + award_index, 1);
	}

	int32 cloud_shop_mgr::calc_player_basic_award(e_time_limit_activity_type activity_type)
	{
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return -1;
		}
		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr1.size() < cloud_shop_basic_award_size)
		{
			return -1;
		}
		int32 base_award_total_weight = 0;
		for (int32 i = 0; i < cloud_shop_basic_award_size; i++)
		{
			base_award_total_weight += time_limit_act_branch_temp.ParamArr1[i];
		}

		if (base_award_total_weight <= 0)
		{
			return -1;
		}

		int32 rand_number = random_gen::get_random(0, base_award_total_weight - 1);
		int32 tmp_number = 0;
		for (int32 i = 0; i < cloud_shop_basic_award_size; i++)
		{
			tmp_number += time_limit_act_branch_temp.ParamArr1[i];

			if (rand_number <= tmp_number)
			{
				return i;
			}
		}

		return -1;
	}

	cloud_shop_big_rewards cloud_shop_mgr::calc_big_reward(e_time_limit_activity_type activity_type, int32 big_reward_num, int32 basic_reward_num, const std::vector<int32>& rewards_array)
	{
		cloud_shop_big_rewards cloud_shop_big_rewards_vec;
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end() || big_reward_num > basic_reward_num)
		{
			return cloud_shop_big_rewards_vec;
		}

		std::vector<int32> big_reward_random_list;
		for (int32 i = 0; i < basic_reward_num; i++)
		{
			big_reward_random_list.push_back(i);
		}
		random_array(big_reward_random_list);

		std::vector<guid_64> cloud_shop_calc_player;
		for (auto iter = cloud_shop_ite->second.begin(); iter != cloud_shop_ite->second.end(); iter++)
		{
			for (int32 i = 0; i < iter->second.get_data(e_cloud_shop_role_info_buy_number); i++)
			{
				cloud_shop_calc_player.push_back(iter->second.role_guid);
			}
			for (int32 i = 0; i < iter->second.get_data(e_cloud_shop_role_info_robber_number); i++)
			{
				cloud_shop_calc_player.push_back(iter->second.role_guid);
			}
		}

		auto big_reward_random_list_ite = big_reward_random_list.begin();
		for (int32 i = 0; i < big_reward_num; i++)
		{
			if (*big_reward_random_list_ite < cloud_shop_calc_player.size())
			{
				guid_64 player_guid = cloud_shop_calc_player[*big_reward_random_list_ite];
				auto player_data_ptr = cloud_shop_ite->second.find(player_guid);
				if (player_data_ptr != cloud_shop_ite->second.end() && rewards_array.size() > (player_data_ptr->second.get_data(e_cloud_shop_role_info_class_type) - 1) * 4 + 2)//用需要用到的最大长度判空
				{
					cloud_shop_big_reward_content player_content;
					player_content.role_guid = player_guid;
					player_content.set_role_name(player_data_ptr->second.role_name);
					player_content.big_item_id = rewards_array[(player_data_ptr->second.get_data(e_cloud_shop_role_info_class_type) - 1) * 4];
					player_content.big_item_count = rewards_array[(player_data_ptr->second.get_data(e_cloud_shop_role_info_class_type) - 1) * 4 + 1];
					player_content.big_item_lock = rewards_array[(player_data_ptr->second.get_data(e_cloud_shop_role_info_class_type) - 1) * 4 + 2];
					player_content.server_id = player_data_ptr->second.get_data(e_cloud_shop_role_info_server_id);
					cloud_shop_big_rewards_vec.push_back(player_content);
				}
			}
			big_reward_random_list_ite++;
		}

		return cloud_shop_big_rewards_vec;
	}

	void cloud_shop_mgr::send_cloud_shop_awards(e_time_limit_activity_type activity_type)
	{
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end())
		{
			return;
		}
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return;
		}
		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr3.size() < 2 || time_limit_act_branch_temp.Reward.size() < cloud_shop_basic_award_size * 3)
		{
			return;
		}

		int32 basic_reward_num = 0;
		if (time_limit_act_branch_temp.ParamArr3.size() >= 3 && time_limit_act_branch_temp.ParamArr3[2] <= cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_schedule))
		{
			basic_reward_num = cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_schedule);
		}
		else
		{
			basic_reward_num = time_limit_act_branch_temp.ParamArr3[0];
		}

		cloud_shop_big_rewards big_reward_player_vec = calc_big_reward(activity_type, time_limit_act_branch_temp.ParamArr3[1], basic_reward_num, time_limit_act_branch_temp.ParamArr2);
			
		//保底奖励
		for (auto iter = cloud_shop_ite->second.begin(); iter != cloud_shop_ite->second.end(); iter++)
		{
			guid_64 player_guid = iter->second.role_guid;
			int32 server_id = iter->second.get_data(e_cloud_shop_role_info_server_id);
			std::vector<s_item_template_info> item_list;
			for (int32 i = 0; i < cloud_shop_basic_award_size; i++)
			{
				s_item_template_info item;
				item.m_item_num = iter->second.get_data(e_cloud_shop_role_info_basic_award_count1 + i) * time_limit_act_branch_temp.Reward[i * 3 + 1];
				if (item.m_item_num > 0)
				{
					item.m_item_id = time_limit_act_branch_temp.Reward[i * 3];
					item.m_lock = time_limit_act_branch_temp.Reward[i * 3 + 2];
					item_list.push_back(item);
				}
			}
			if (item_list.size() > 0)
			{
				std::string mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cloud_shop_basic_award_title);
				std::string mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cloud_shop_basic_award_context);
				mail_ws_mgr::get_instance().send_mail_system(player_guid, server_id, item_list, mail_title, mail_context);
			}
		}	
		
		//大奖
		for (int32 i = 0; i < big_reward_player_vec.size(); i++)
		{
			guid_64 player_guid = big_reward_player_vec[i].role_guid;
			int32 server_id = big_reward_player_vec[i].server_id;
			if (big_reward_player_vec[i].big_item_id > 0)
			{
				std::vector<s_item_template_info> item_list;
				item_list.push_back(s_item_template_info(big_reward_player_vec[i].big_item_id, big_reward_player_vec[i].big_item_count, big_reward_player_vec[i].big_item_lock));
				
				std::string mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cloud_shop_big_award_title);
				std::string mail_context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_cloud_shop_big_award_context);
				mail_ws_mgr::get_instance().send_mail_system(player_guid, server_id, item_list, mail_title, mail_context);
			}
		}
		cur_cloud_shop_activity.set_activity_string_param(make_today_reward_string(big_reward_player_vec), e_time_limit_activity_string_param_index_1);
		cur_cloud_shop_activity.set_activity_info(e_time_limit_ws_act_info_flag, 1);
		time_limit_activity_ws_mgr::get_instance().save_ws_time_limit_act_to_db(activity_type);
	}

	void cloud_shop_mgr::save_activity_role_content(e_time_limit_activity_type activity_type, const guid_64 role_id)
	{
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end())
		{
			return;
		}
		ws2dp_save_cloud_shop_role_info req_save;
		req_save.player_role_info = cloud_shop_ite->second[role_id];
		ws_client::getInstance().send_to_dp(&req_save, sizeof(req_save));
	}

	void cloud_shop_mgr::clear_cloud_shop_player_info(e_time_limit_activity_type activity_type)
	{
		auto cloud_shop_ite = m_cloud_shop_info_map.find(activity_type);
		if (cloud_shop_ite == m_cloud_shop_info_map.end())
		{
			return;
		}
		cloud_shop_ite->second.clear();
		ws2dp_clear_cloud_shop_player_info req_clear;
		req_clear.clear_type = activity_type;
		ws_client::getInstance().send_to_dp(&req_clear, sizeof(req_clear));
	}

	bool cloud_shop_mgr::is_in_sell_time(e_client_req_type req_type, e_time_limit_activity_type activity_type)
	{
		time_info cur_time = time_helper::get_cur_time_new();
		switch (req_type)
		{
		case e_client_req_type_buy:
		{
			return cur_time.second_in_day <= cloud_shop_robbery_start_time;
		}
			break;
		case e_client_req_type_robber:
		{
			if (get_is_liberty_buy(activity_type))
			{
				return cur_time.second_in_day <= cloud_shop_award_opening_time;
			}
			return cur_time.second_in_day <= cloud_shop_award_opening_time && cur_time.second_in_day > cloud_shop_robbery_start_time;
		}
			break;
		default:
			break;
		}
		return false;
	}

	bool cloud_shop_mgr::get_is_liberty_buy(e_time_limit_activity_type activity_type)
	{
		s_time_limit_activity_ws_info& cur_cloud_shop_activity = time_limit_activity_ws_mgr::get_instance().get_activity_info_ref(activity_type);
		if (!cur_cloud_shop_activity.is_valid() || !cur_cloud_shop_activity.is_open())
		{
			return false;
		}
		const s_time_limit_activity_branch_temp& time_limit_act_branch_temp = time_limit_activity_temp_ws_mgr::get_instance().get_time_limit_act_branch_with_id(cur_cloud_shop_activity.get_activity_info(e_time_limit_ws_act_info_branch_temp_id));
		if (!time_limit_act_branch_temp.is_valid() || time_limit_act_branch_temp.ParamArr5.size() < 1)
		{
			return false;
		}
		if (time_limit_act_branch_temp.ParamArr5[0] > 0)
		{
			return true;
		}
		return  false;
	}

	void cloud_shop_mgr::send_rewards_and_clear_info(e_time_limit_activity_type activity_type)
	{
		send_cloud_shop_awards(activity_type);
		clear_cloud_shop_player_info(activity_type);
	}

	void cloud_shop_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		for (auto ite_info_map = m_cloud_shop_info_map.begin(); ite_info_map != m_cloud_shop_info_map.end(); ite_info_map++)
		{
			auto ite_player_info = ite_info_map->second.find(role_guid);
			if (ite_player_info != ite_info_map->second.end())
			{
				ite_player_info->second.set_role_name(role_name);
			}
		}
	}
}