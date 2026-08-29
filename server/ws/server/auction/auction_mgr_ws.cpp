/********************************************************************
file name:	relation_mgr_ws.cpp
created:	2017/06/20 20:46
author:		zhy
purpose:
*********************************************************************/
#include "auction_mgr_ws.h"
#include "Logic/time_def.hpp"
#include "../mail/event_ws_mgr.h"
#include "utility/random.h"
#include "../client_session_mgr.hpp"
#include "ws_client.hpp"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/guid_gen.h"
#include "server_log.hpp"
#include "world_server.hpp"
#include "game_cfg/servers_config.h"

#include "utility/parse_msg.h"
#include "../ranking/ranking_mgr_ws.h"
#include "globle_data.h"
#include "auction.pb.h"
#include "server/mail/mail_event_ws.h"
#include "server/legion/legion_ws_mgr.h"
#include "net.pb.h"

namespace faith
{
	auction_mgr_ws auction_mgr_ws::s_auction_mgr_ws;

	auction_mgr_ws::auction_mgr_ws()
	{
		clear_data();
	}

	auction_mgr_ws::~auction_mgr_ws()
	{

	}

	void auction_mgr_ws::clear_data()
	{
		m_selling_goods_map.clear();
		m_direct_purchase_map.clear();
		m_old_tick_time = 0;
		m_next_clear_old_legion_item_time = 0;

		m_is_clearing_old_item = false;
		m_is_force_clear_old_item = false;

		m_need_check_world_auction = true;
		m_next_check_world_auction_time = 0;
		m_world_sell_num = 0;

		m_wait_show_vec.clear();

		m_time_tip_vec.clear();
		while (!m_person_info_queue.empty())
		{
			m_person_info_queue.pop();
		}
		
	}

	auction_mgr_ws&	auction_mgr_ws::get_instance()
	{
		return s_auction_mgr_ws;
	}

	void auction_mgr_ws::tick(const int64& new_time)
	{
		if (is_use_lua())
		{
			return;
		}
		if (m_is_force_clear_old_item)
		{
			if (!m_is_clearing_old_item)
			{
				clear_old_item();
			}
		}
		else
		{
			if (new_time - m_old_tick_time > auction_check_time)
			{
				m_old_tick_time = new_time;
				clear_old_item();
			}
		}

		if (new_time > m_next_check_world_auction_time )
		{
			if (m_need_check_world_auction)
			{
				check_world_auction_sell();
			}

			m_next_check_world_auction_time = init_unit::get_end_time(auction_legion_sell_check_interval);
		}
		wait_tick();
		time_out_tick(new_time);
		time_tip_tick();

		person_tick();
	}
	
	void auction_mgr_ws::add_auction_info_to_db(const s_auction_info& info, const guid_64& old_item_guid, const s_unit_info& role_info, bool is_add_to_log, const login_fixed_data& third_info)
	{
		if (is_use_lua())
		{
			return;
		}
		if (!info.item_info.item_guid.is_valid())
		{
			return;
		}

		server2dp_proto_ws2dp_auction_add_item_into_db msg;
		msg.set_is_add_to_log(is_add_to_log);
		msg.set_old_item_guid(old_item_guid.server_64);
		info.set_proto_info(msg.mutable_info());
		role_info.set_proto_info(msg.mutable_role_info());
		third_info.set_proto_info(msg.mutable_third_info());
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_add_auction_into_db);

		//ws2dp_auction_add_item_into_db msg;
		//msg.info = info;
		//msg.role_info = role_info;
		//msg.is_add_to_log = is_add_to_log;
		//msg.old_item_guid = old_item_guid;
		//msg.third_info = third_info;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_add_item_into_db));
	}

	void auction_mgr_ws::add_share_list_to_db(const s_auction_share_info* share_list, int32 list_num)
	{
		if (share_list == nullptr
			|| list_num == 0)
		{
			return;
		}
		s_auction_share_info data_info[auction_share_list_max_num];
		for (int32 i = 0; i < list_num; ++i)
		{
			if (i >= auction_share_list_max_num)
			{
				break;
			}
			data_info[i] = share_list[i];
		}

		server2dp_proto_ws2dp_auction_add_share_list msg;
		for (int32 i = 0; i < auction_share_list_max_num; ++i)
		{
			msg.add_item_guid(data_info[i].item_guid.server_64);
			msg.add_role_guid(data_info[i].role_guid.server_64);
			msg.add_share_rate(data_info[i].share_rate);
			msg.add_share_mail_title(data_info[i].share_mail_title);
			msg.add_share_mail_content(data_info[i].share_mail_content);
		}
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_add_share_list);

		//ws2dp_auction_add_share_list msg;
		//for (int32 i = 0; i < list_num; ++i)
		//{
		//	if (i >= auction_share_list_max_num)
		//	{
		//		break;
		//	}
		//	msg.data_info[i] = share_list[i];
		//	msg.data_num++;
		//}
		//ws_client::getInstance().send_to_dp(&msg, msg.get_pak_length());
	}

	void auction_mgr_ws::send_auction_puchase_success_info(const s_auction_info& info)
	{
		if (is_use_lua())
		{
			return;
		}
		if (!info.item_info.item_guid.is_valid())
		{
			return;
		}
		client_session* target_session = client_session_mgr::getInstance().get_session(info.seller_guid);
		if (target_session == nullptr
			|| target_session->get_cs_conn_index() < 0)
		{
			return;
		}

		ws2cs_auction_send_sell_success_info success_msg;
		success_msg.auction_info = info;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_auction_sell_success_info);
		if (is_use == false)
		{
			target_session->send_to_cs(&success_msg, sizeof(ws2cs_auction_send_sell_success_info));
		}
		else
		{
			faith::ws2cs_proto::auction_send_sell_success_info pro_msg;
			success_msg.to_proto(pro_msg);
			target_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_auction_sell_success_info);
		}
	}

	void auction_mgr_ws::buy(const guid_64& role_guid, const guid_64& item_guid)
	{
		if (is_use_lua())
		{
			return;
		}
		if (is_item_locked(item_guid))
		{
			auction_proto_operate_end buy_failed_msg;
			buy_failed_msg.set_operate_type(e_auction_operate_type_purchase);
			buy_failed_msg.set_operate_result(e_auction_purchase_already_purchased);

			//�����⼸����ûʲô��
			buy_failed_msg.set_item_guid(item_guid.server_64);
			buy_failed_msg.set_item_num(0);	
			buy_failed_msg.set_money_type(0);
			buy_failed_msg.set_money_num(0);

			client_session* target_session = client_session_mgr::getInstance().get_session(role_guid);
			if (target_session != nullptr)
			{
				target_session->send_to_client(&buy_failed_msg, e_msgindex_s2c_auction_operate_end);
			}
			return;
		}
		operate_selling_lock(item_guid, true);

		server2dp_proto_ws2dp_find_buy_target buy_msg;
		buy_msg.set_role_guid(role_guid.server_64);
		buy_msg.set_item_guid(item_guid.server_64);
		ws_client::getInstance().send_to_dp_new(&buy_msg, e_msg_ws2dp_auction_find_buy_target);

		//ws2dp_find_buy_target buy_msg;
		//buy_msg.role_guid = role_guid;
		//buy_msg.item_guid = item_guid;
		//ws_client::getInstance().send_to_dp(&buy_msg, sizeof(ws2dp_find_buy_target));
	}

	void auction_mgr_ws::find_buy_target_end(const s_auction_info& info, guid_64 role_guid, guid_64 item_guid)
	{
		if (is_use_lua())
		{
			return;
		}
		client_session* session_player = client_session_mgr::getInstance().get_session(role_guid);

		if (!info.item_info.item_guid.is_valid())
		{
			auction_proto_operate_end buy_failed_msg;
			buy_failed_msg.set_operate_type(e_auction_operate_type_purchase);
			buy_failed_msg.set_operate_result(e_auction_purchase_already_purchased);

			//�����⼸����ûʲô��
			buy_failed_msg.set_item_guid(0);
			buy_failed_msg.set_item_num(0);
			buy_failed_msg.set_money_type(0);
			buy_failed_msg.set_money_num(0);

			if (session_player != nullptr
				&& session_player->get_cs_conn_index() >= 0)
			{
				session_player->send_to_client(&buy_failed_msg, e_msgindex_s2c_auction_operate_end);
			}
		}
		else
		{
			if (nullptr == session_player
				|| session_player->get_cs_conn_index() < 0)
			{
				//�Է��������� ����֪���۶���Ǯ
				auction_mgr_ws::get_instance().add_auction_info_to_db(info);
			}
			else
			{
				ws2cs_auction_purchase_success success_msg;
				success_msg.auction_info = info;
				success_msg.role_guid = role_guid;
				bool is_use = proto_by_lua(e_msgindex_ws2cs_auction_purchase_success);
				if (is_use == false)
				{
					session_player->send_to_cs(&success_msg, sizeof(ws2cs_auction_purchase_success));
				}
				else
				{
					faith::ws2cs_proto::auction_purchase_success pro_msg;
					success_msg.to_proto(pro_msg);
					session_player->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_auction_purchase_success);
				}
			}
		}
		operate_selling_lock(item_guid, false);
	}

	void auction_mgr_ws::cancel_sell(const guid_64& role_guid, const guid_64& item_guid)
	{
		if (is_use_lua())
		{
			return;
		}
		if (is_item_locked(item_guid))
		{
			auction_proto_operate_end cancel_sell_end_msg;
			cancel_sell_end_msg.set_item_guid(item_guid.server_64);
			cancel_sell_end_msg.set_item_num(0);
			cancel_sell_end_msg.set_operate_type(e_auction_operate_type_cancel_sell);
			cancel_sell_end_msg.set_money_type(0);
			cancel_sell_end_msg.set_money_num(0);
			cancel_sell_end_msg.set_operate_result(e_auction_cancel_sell_already_sell);

			client_session* target_session = client_session_mgr::getInstance().get_session(role_guid);
			if (target_session != nullptr)
			{
				target_session->send_to_client(&cancel_sell_end_msg, e_msgindex_s2c_auction_operate_end);
			}
			return;
		}

		operate_selling_lock(item_guid, true);

		server2dp_proto_ws2dp_auction_cancel_sell cancel_msg;
		cancel_msg.set_role_guid(role_guid.server_64);
		cancel_msg.set_item_guid(item_guid.server_64);
		ws_client::getInstance().send_to_dp_new(&cancel_msg, e_msg_ws2dp_auction_cancel_sell);

		//ws2dp_auction_cancel_sell cancel_msg;
		//cancel_msg.role_guid = role_guid;
		//cancel_msg.item_guid = item_guid;
		//ws_client::getInstance().send_to_dp(&cancel_msg, sizeof(ws2dp_auction_cancel_sell));
	}

	void auction_mgr_ws::cancel_sell_end(const s_auction_info& info, guid_64 role_guid)
	{
		if (is_use_lua())
		{
			return;
		}
		//guidһ���У���ID�ж��Ƿ��ҵ�
		if (info.item_info.data_ary[e_item_info_info_id] == 0)
		{
			auction_proto_operate_end buy_failed_msg;
			buy_failed_msg.set_operate_type(e_auction_operate_type_cancel_sell);
			buy_failed_msg.set_operate_result(e_auction_cancel_sell_already_sell);

			//�����⼸����ûʲô��
			buy_failed_msg.set_item_guid(0);
			buy_failed_msg.set_item_num(0);
			buy_failed_msg.set_money_type(0);
			buy_failed_msg.set_money_num(0);

			client_session* session_player = client_session_mgr::getInstance().get_session(role_guid);
			if (session_player != nullptr
				&& session_player->get_cs_conn_index() >= 0)
			{
				session_player->send_to_client(&buy_failed_msg, e_msgindex_s2c_auction_operate_end);
			}
		}
		else
		{
			client_session* session_player = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr == session_player
				|| session_player->get_cs_conn_index() < 0)
			{
				const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
				const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_cancel_sell_title);
				const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_cancel_sell_content);

				std::vector<s_item_info> temp_item_info_array;
				temp_item_info_array.push_back(info.item_info);
				event_ws_mgr::get_instance().send_mail_to_player_by_system(role_guid, 0, sender_name, mail_title, mail_content,
					0,0,0,0, temp_item_info_array);
			}
			else
			{
				cancel_del_show(info);
				ws2cs_auction_cancel_sell success_msg;
				success_msg.auction_info = info;
				success_msg.role_guid = role_guid;
				bool is_use = proto_by_lua(e_msgindex_ws2cs_auction_cancel_sell);
				if (is_use == false)
				{
					session_player->send_to_cs(&success_msg, sizeof(ws2cs_auction_cancel_sell));
				}
				else
				{
					faith::ws2cs_proto::auction_cancel_sell pro_msg;
					success_msg.to_proto(pro_msg);
					session_player->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_auction_cancel_sell);
				}

			}
		}
		operate_selling_lock(info.item_info.item_guid, false);
	}

	void auction_mgr_ws::operate_selling_lock(guid_64 item_guid, bool is_add)
	{
		if (is_use_lua())
		{
			return;
		}
		if (!item_guid.is_valid())
		{
			return;
		}

		selling_goods_map::iterator temp_it = m_selling_goods_map.find(item_guid.server_64);
		if (is_add)
		{
			if (!is_item_locked(item_guid))
			{
				m_selling_goods_map.insert({ item_guid.server_64 ,true });
			}
		}
		else
		{
			if (is_item_locked(item_guid))
			{
				m_selling_goods_map.erase(temp_it);
			}
		}
	}

	bool auction_mgr_ws::is_item_locked(guid_64 item_guid)
	{
		selling_goods_map::iterator temp_it = m_selling_goods_map.find(item_guid.server_64);
		return temp_it != m_selling_goods_map.end();
	}

	void auction_mgr_ws::sell_end(const s_auction_info& info, int32 sell_result, const s_unit_info& role_info, bool is_add_to_log, const login_fixed_data& third_info)
	{
		if (is_use_lua())
		{
			return;
		}
		int32 temp_login_type = e_login_type_new_account_and_password;
		switch (info.trade_type)
		{
		case e_trading_type_world:
		case e_trading_type_normal_sell:
		{//��ͨ����Ҫ��CS�ذ�
			client_session* session_player = client_session_mgr::getInstance().get_session(info.seller_guid);
			if (session_player != nullptr
				&& session_player->get_cs_conn_index() >= 0)
			{
				ws2cs_auction_sell_end end_msg;
				end_msg.info = info;
				end_msg.operate_result = sell_result;
				bool is_use = proto_by_lua(e_msgindex_ws2cs_auction_sell_end);
				if (is_use == false)
				{
					session_player->send_to_cs(&end_msg, sizeof(ws2cs_auction_sell_end));
				}
				else
				{
					faith::ws2cs_proto::auction_sell_end pro_msg;
					end_msg.to_proto(pro_msg);
					session_player->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_auction_sell_end);
				}
				temp_login_type = session_player->m_login_type;
			}
			if (sell_result == e_auction_sell_success && info.show_guid.is_valid() == false)
			{
				s_auction_timer_info temp;
				temp.auction_info = info;
				if (session_player != nullptr)
				{
					temp.template_id = session_player->get_role_info_data(e_role_info_template_id);
				}
				send_up_notice(info);
				//add_wait_show(temp);	
			}
			
			//send_prepare_notice(info);

			/*	if (info.item_color >= e_item_color_orange
					&& sell_result == e_auction_sell_success)
				{
					sell_rare_item_proc(info);
				}*/
		}
			break;
		case e_trading_type_legion:
		{
			legion_ws* temp_legion = legion_ws_mgr::get_instance().get_legion(info.seller_guid);
			if (sell_result == e_auction_sell_success
				&& temp_legion != nullptr)
			{
				temp_legion->set_auction_selling_check_start(true);
			}
		}
		break;
		default:
			break;
		}

		if (is_add_to_log)
		{
			int32 item_num = info.item_info.data_ary[e_item_info_stack_count];
			int32 money_type = info.sell_money_type;
			int32 money_count = info.sell_total_money;
			int32 item_id = info.item_info.data_ary[e_item_info_info_id];
			server_log::auction_sell_log(item_id, item_num, money_type, money_count);
			server_log::auction_db_log(info, e_auction_state_start_sell, role_info, third_info, temp_login_type);

			set_log_var(log_head)
			if (info.trade_type != e_trading_type_normal_sell || info.show_guid.is_valid() == true)
			{
				set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count())
				server_log::serverAuction(log_head, "null", "null", "null", 0,
				info.item_info.item_guid.to_string(), info.item_info.data_ary[e_item_info_info_id], info.item_info.data_ary[e_item_info_stack_count],
				info.trade_type, "null", "null", "null", 0, info.sell_money_type, globle_data::get_instance().get_money_str_by_type(info.sell_money_type), info.sell_total_money, e_auction_state_start_sell);
			}
			else
			{
				set_log_common_head_part1(log_head, SERVERCONFIG->app_key, third_info.param7, third_info.param4 , third_info.param5);
				set_log_common_head_part2(log_head, third_info.param2, init_unit::change_string_to_i32(third_info.param15));
				set_log_common_head_part3(log_head, role_info.account, role_info.data_ary[e_role_info_server_id], utility::get_tick_count())
				set_log_common_head_part4(log_head, role_info.role_guid, role_info.role_name, role_info.data_ary[e_role_info_exp_level], role_info.data_ary[e_role_info_gender])
				set_log_common_head_part5(log_head, role_info.data_i64_ary[e_role_i64_info_gs_value],role_info.data_ary[e_role_info_class_type], role_info.data_ary[e_role_info_vip_level])

				server_log::serverAuction(log_head, role_info.account, init_unit::change_i64_to_string(role_info.role_guid.server_64), role_info.role_name, 0,
				info.item_info.item_guid.to_string(), info.item_info.data_ary[e_item_info_info_id], info.item_info.data_ary[e_item_info_stack_count],
				info.trade_type, "null", "null", "null", 0, info.sell_money_type, globle_data::get_instance().get_money_str_by_type(info.sell_money_type), info.sell_total_money, e_auction_state_start_sell);

			}


		}
	}

	void auction_mgr_ws::sell_rare_item_proc(const s_auction_info& info)
	{
		client_session* session_player = client_session_mgr::getInstance().get_session(info.seller_guid);
		if (session_player == nullptr
			|| session_player->get_cs_conn_index() < 0)
		{
			return;
		}
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return;
		}
		if (item_template_ptr->AuctionNotice <= 0)
		{
			return;
		}

		std::vector<int32> notice_param_array;
		notice_param_array.reserve(e_item_info_max);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			notice_param_array.push_back(info.item_info.data_ary[i]);
		}

		//�����Ƶ�
		std::vector<std::string> vec_notice;
		vec_notice.push_back(template_manager::get_instance().get_str_id_by_notice_id(93000232));
		vec_notice.push_back(info.seller_name);

		//�ṹ���е�nameû����ɫ����
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
		vec_notice.push_back(temp_string);

		std::string notice_str = init_unit::implode(vec_notice);
		event_ws_mgr::get_instance().send_notice_to_all(93000232, time_helper::get_cur_time_new().second, info.seller_guid, notice_str, session_player->get_role_info_data(e_role_info_template_id));

		//����Ƶ����
		event_ws_mgr::get_instance().send_notice_with_param(
			info.seller_guid, info.seller_name, 
			"90089566", 
			notice_param_array, e_notice_param_item_info, 
			e_chat_type_world, session_player->get_role_info_data(e_role_info_template_id), 93000233);

	}

	void auction_mgr_ws::trade_bid(const guid_64& role_guid, const guid_64& item_guid, const int32& price_type, const int32& price_value)
	{
		if (is_use_lua())
		{
			return;
		}
		if (is_item_locked(item_guid))
		{
			auction_proto_operate_end bid_failed_msg;
			bid_failed_msg.set_operate_type(e_auction_operate_type_bid);
			bid_failed_msg.set_operate_result(e_auction_bid_money_changed);

			//�����⼸����ûʲô��
			bid_failed_msg.set_item_guid(item_guid.server_64);
			bid_failed_msg.set_item_num(0);
			bid_failed_msg.set_money_type(0);
			bid_failed_msg.set_money_num(0);

			client_session* target_session = client_session_mgr::getInstance().get_session(role_guid);
			if (target_session != nullptr)
			{
				target_session->send_to_client(&bid_failed_msg, e_msgindex_s2c_trade_operate_end);
			}
			return;
		}
		operate_selling_lock(item_guid, true);

		server2dp_proto_ws2dp_auction_find_bid_target bid_msg;
		bid_msg.set_role_guid(role_guid.server_64);
		bid_msg.set_item_guid(item_guid.server_64);
		bid_msg.set_price_money_type(price_type);
		bid_msg.set_price_money_value(price_value);
		ws_client::getInstance().send_to_dp_new(&bid_msg, e_msg_ws2dp_auction_find_bid_target);

		//ws2dp_auction_find_bid_target bid_msg;
		//bid_msg.role_guid = role_guid;
		//bid_msg.item_guid = item_guid;
		//bid_msg.price_money_type = price_type;
		//bid_msg.price_money_value = price_value;
		//ws_client::getInstance().send_to_dp(&bid_msg, sizeof(ws2dp_auction_find_bid_target));
	}

	void auction_mgr_ws::find_bid_end_proc(const guid_64& role_guid, const guid_64& item_guid, const s_auction_info& info, const int32& price_type, const int32& price_value)
	{
		if (is_use_lua())
		{
			return;
		}
		client_session* session_player = client_session_mgr::getInstance().get_session(role_guid);

		if (!info.item_info.item_guid.is_valid())
		{
			auction_proto_operate_end buy_failed_msg;
			buy_failed_msg.set_operate_type(e_auction_operate_type_bid);
			buy_failed_msg.set_operate_result(e_auction_purchase_already_purchased);

			//�����⼸����ûʲô��
			buy_failed_msg.set_item_guid(0);
			buy_failed_msg.set_item_num(0);
			buy_failed_msg.set_money_type(0);
			buy_failed_msg.set_money_num(0);

			if (session_player != nullptr
				&& session_player->get_cs_conn_index() >= 0)
			{
				session_player->send_to_client(&buy_failed_msg, e_msgindex_s2c_auction_operate_end);
			}
			operate_selling_lock(item_guid, false);
		}
		else
		{
			if (info.sell_money_type != price_type
				|| info.cur_bid_money != price_value)
			{
				auction_proto_operate_end buy_failed_msg;
				buy_failed_msg.set_operate_type(e_auction_operate_type_bid);
				buy_failed_msg.set_operate_result(e_auction_bid_money_changed);

				//�����⼸����ûʲô��
				buy_failed_msg.set_item_guid(0);
				buy_failed_msg.set_item_num(0);
				buy_failed_msg.set_money_type(0);
				buy_failed_msg.set_money_num(0);

				if (session_player != nullptr
					&& session_player->get_cs_conn_index() >= 0)
				{
					session_player->send_to_client(&buy_failed_msg, e_msgindex_s2c_auction_operate_end);
				}
				operate_selling_lock(item_guid, false);
			}
			if (nullptr == session_player
				|| session_player->get_cs_conn_index() < 0)
			{
				operate_selling_lock(item_guid, false);
			}
			else
			{
				//�Է��������� ����֪���۶���Ǯ
				ws2cs_auction_find_bid_target_end success_msg;
				success_msg.auction_info = info;
				success_msg.role_guid = role_guid;
				bool is_use = proto_by_lua(e_msgindex_ws2cs_auction_find_bid_target_end);
				if (is_use == false)
				{
					session_player->send_to_cs(&success_msg, sizeof(ws2cs_auction_find_bid_target_end));
				}
				else
				{
					faith::ws2cs_proto::auction_find_bid_target_end pro_msg;
					success_msg.to_proto(pro_msg);
					session_player->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_auction_find_bid_target_end);
				}
			}
		}
	}

	void auction_mgr_ws::bid_success_proc(const s_auction_info& auction_info, const guid_64& old_buyer)
	{
		if (is_use_lua())
		{
			return;
		}
		server2dp_proto_ws2dp_auction_bid_success success_msg;
		success_msg.set_new_record_id(guid_gen::make_guid().server_64);
		success_msg.set_old_buyer_guid(old_buyer);
		s_auction_info new_auction_info = auction_info;
		update_auction_time(new_auction_info);
		new_auction_info.set_proto_info(success_msg.mutable_auction_info());
		ws_client::getInstance().send_to_dp_new(&success_msg, e_msg_ws2dp_bid_success);

		//ws2dp_auction_bid_success success_msg;
		//success_msg.new_record_id = guid_gen::make_guid();
		//success_msg.old_buyer_guid = old_buyer;
		//success_msg.auction_info = auction_info;
		//update_auction_time(success_msg.auction_info);
		//ws_client::getInstance().send_to_dp(&success_msg, sizeof(ws2dp_auction_bid_success));

	
		/*auction_proto_operate_end purchase_end_msg;
		purchase_end_msg.set_item_guid(auction_info.item_info.item_guid.server_64);
		purchase_end_msg.set_item_num(0);
		purchase_end_msg.set_operate_type(e_auction_operate_type_bid);
		purchase_end_msg.set_money_type(0);
		purchase_end_msg.set_money_num(0);
		purchase_end_msg.set_operate_result(e_auction_bid_success);
		client_session* new_session_player = client_session_mgr::getInstance().get_session(auction_info.buyer_guid);
		if (new_session_player != nullptr &&  new_session_player->get_cs_conn_index() >= 0)
		{
			new_session_player->send_to_client(&purchase_end_msg, e_msgindex_s2c_auction_operate_end);
		}
		std::cout << "-----auction-----request---success----" << utility::get_tick_count() << std::endl;*/

		if (old_buyer.is_valid()
			&& old_buyer != auction_info.buyer_guid)
		{
			client_session* session_player = client_session_mgr::getInstance().get_session(old_buyer);
			if (session_player != nullptr
				&& session_player->get_cs_conn_index() >= 0)
			{
				auction_proto_bid_are_overtaken msg;
				msg.set_item_id(auction_info.item_info.data_ary[e_item_info_info_id]);
				msg.set_item_num(auction_info.item_info.data_ary[e_item_info_stack_count]);
				session_player->send_to_client(&msg, e_msgindex_s2c_bid_are_overtaken);

			}
		}
	}

	void auction_mgr_ws::clear_old_item()
	{
		server2dp_proto_ws2dp_auction_clear_old_item msg;
		msg.set_cur_time(utility::get_tick_count());
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_clear_old_item);

		//ws2dp_auction_clear_old_item msg;
		//msg.cur_time = utility::get_tick_count();
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_clear_old_item));

		m_is_clearing_old_item = true;
	}

	void auction_mgr_ws::buy_set_bind(s_auction_info * auction_info_ptr)
	{
		if (auction_info_ptr == nullptr)
		{
			return;
		}
		auction_info_ptr->item_info.data_ary[e_item_info_locked] = 1;
	}

	void auction_mgr_ws::get_old_item_end_proc(const s_auction_info* auction_info, const int32& data_num)
	{
		if (is_use_lua())
		{
			return;
		}

		for (int32 i = 0; i < data_num; ++i)
		{
			if (auction_info[i].buyer_guid.is_valid())
			{
				buy_set_bind((s_auction_info *)(auction_info + i));
				send_reward_to_buyer(auction_info[i]);
			}

			share_reward_to_seller(auction_info[i]);

			switch (auction_info[i].trade_type)
			{
			case e_trading_type_world:
			{
				add_record_to_db(auction_info[i], auction_info[i].buyer_guid, 0, false);
				add_record_to_db(auction_info[i], auction_info[i].seller_guid, 0, true);
				// ��������ֻ�г�������Ч�������˳��� ����ʧ����Ҫ����Ʒ���ظ�������
				if (auction_info[i].seller_guid.is_valid())
				{
					if (auction_info[i].buyer_guid.is_valid() == false)
					{
						cancel_sell_end(auction_info[i], auction_info[i].seller_guid);
					}
					else
					{
						give_money_to_seller(auction_info[i]);
					}
				}
			}
			break;
			case e_trading_type_normal_sell:
			{
				add_record_to_db(auction_info[i], auction_info[i].buyer_guid, 0, false);
				add_record_to_db(auction_info[i], auction_info[i].seller_guid, 0, true);
			}
			break;
			case e_trading_type_legion:
			{
				//�������� ��¼���ھ��� �� �����˾��Ǿ���guid
				add_record_to_db(auction_info[i], auction_info[i].seller_guid);
				if (!auction_info[i].buyer_guid.is_valid()
					&& !m_is_force_clear_old_item)
				{
					if (auction_info[i].is_sky_order != 1)
					{
						s_auction_info temp_info = auction_info[i];
						temp_info.trade_type = e_trading_type_world;
						temp_info.seller_guid.clear_data();
						//temp_info.sell_total_money = 0;

						temp_info.sell_time = temp_info.deal_time;
						temp_info.deal_time = init_unit::get_end_time(GAMECONFIG->WorldAuctionItemLiveTime) + get_config_param(e_auction_param_wait_bid) * second_tick_time;

						add_auction_info_to_db(temp_info, guid_64(), s_unit_info(), true);

						send_up_notice(temp_info);

						set_need_world_acution_check(true);
					}
				}
			}
			break;
			default:
				break;
			}
		}

		m_is_clearing_old_item = false;

		if (m_is_force_clear_old_item
			&& data_num < auction_clear_item_num_per_time)
		{
			m_is_force_clear_old_item = false;
			CONSOLE_INFO("Server Merge Auction Clear Success, Please Wait 2 Minutes And Do Next Step");
		}
	}

	void auction_mgr_ws::send_reward_to_buyer(const s_auction_info& auction_info)
	{
		switch (auction_info.trade_type)
		{
		case e_trading_type_normal_sell:
		{
			//��ͨû���� ����Ҳ�������
		}
		break;
		case e_trading_type_world:
		case e_trading_type_legion:
		{
			if (!auction_info.buyer_guid.is_valid())
			{
				return;
			}
			const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_purchase_title);
			const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_purchase_content);

			if (sender_name == nullptr
				|| mail_title == nullptr
				|| mail_content == nullptr)
			{
				return;
			}
			s_mail_info temp_mail_info;

			std::vector<guid_64> temp_item_guid_array;
			temp_item_guid_array.push_back(auction_info.item_info.item_guid);

			init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);

			temp_mail_info.set_sender_name(sender_name);
			temp_mail_info.set_mail_title(mail_title);
			temp_mail_info.set_mail_content_text(mail_content);

			mail_ws_mgr::get_instance().send_mail(auction_info.buyer_guid, 0, temp_mail_info, &auction_info.item_info, 1);

			server_log::auction_db_log(auction_info, e_auction_state_deal);

			set_log_var(log_head)

			set_log_common_head_part3(log_head, "null", world_server::getInstance().get_server_id(), utility::get_tick_count())

			server_log::serverAuction(log_head, "null",auction_info.seller_guid.to_string(), auction_info.seller_name,0,
				auction_info.item_info.item_guid.to_string(), auction_info.item_info.data_ary[e_item_info_info_id], auction_info.item_info.data_ary[e_item_info_stack_count],
				auction_info.trade_type, "null", auction_info.buyer_guid.to_string(), auction_info.buyer_name, 0, auction_info.sell_money_type, globle_data::get_instance().get_money_str_by_type(auction_info.sell_money_type), auction_info.sell_total_money, e_auction_state_deal);
		}
		break;
		default:
			break;
		}

		del_bid_record(auction_info.item_info.item_guid);
	}
	void auction_mgr_ws::share_reward_to_seller(const s_auction_info& auction_info)
	{
		if (is_use_lua())
		{
			return;
		}
		switch (auction_info.trade_type)
		{
		case e_trading_type_normal_sell:
		case e_trading_type_world:
		{
			//���粻�ֺ� ��ͨû�о���û�ֺ�
		}
			break;
		case e_trading_type_legion:
		{
			server2dp_proto_ws2dp_auction_req_share_list msg;
			msg.set_item_guid(auction_info.item_info.item_guid.server_64);
			msg.set_item_template_id(auction_info.item_info.data_ary[e_item_info_info_id]);
			msg.set_buyer_guid(auction_info.buyer_guid.server_64);
			if (!auction_info.buyer_guid.is_valid()
				&& auction_info.sell_money_type == e_money_type_jewel)
			{
				msg.set_money_type((int32)e_money_type_jewel_bind);
			}
			else
			{
				msg.set_money_type(auction_info.sell_money_type);
			}
			msg.set_money_value(auction_info.cur_bid_money);
			if (!auction_info.buyer_guid.is_valid())
			{
				int32 item_id = auction_info.item_info.data_ary[e_item_info_info_id];
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
				if (item_template_ptr != nullptr
					&& item_template_ptr->AuctionAutoRecoverMoneyArray.size() > msg.money_type())
				{
					msg.set_money_value(item_template_ptr->AuctionAutoRecoverMoneyArray[msg.money_type()]);
				}
			}

			ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_req_share_list);
		}
			break;
		default:
			break;
		}
	}
	void auction_mgr_ws::add_record_to_db(const s_auction_info& auction_info, guid_64 role_guid, guid_64 record_guid, bool is_sell)
	{
		if (is_use_lua())
		{
			return;
		}
		if (auction_info.trade_type == e_trading_type_world
			&& !auction_info.buyer_guid.is_valid())
		{
			return;
		}

		server2dp_proto_ws2dp_auction_add_purchase_record msg;
		if (record_guid.is_valid())
		{
			msg.set_record_guid(record_guid);
		}
		else
		{
			msg.set_record_guid(guid_gen::make_guid(role_guid));
		}
		msg.set_role_guid(role_guid);
		msg.set_is_sell(is_sell);
		auction_info.set_proto_info(msg.mutable_auction_info());
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_add_purchase_record);

		//ws2dp_auction_add_purchase_record msg;
		//if (record_guid.is_valid())
		//{
		//	msg.auction_record_info.record_guid = record_guid;
		//}
		//else
		//{
		//	msg.auction_record_info.record_guid = guid_gen::make_guid(role_guid);
		//}
		//msg.auction_record_info.auction_info = auction_info;
		//msg.auction_record_info.role_guid = role_guid;
		//msg.auction_record_info.is_sell = is_sell;

		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_add_purchase_record));
	}

	void auction_mgr_ws::del_bid_record(const guid_64& item_guid)
	{
		if (is_use_lua())
		{
			return;
		}
		if (!item_guid.is_valid())
		{
			return;
		}

		server2dp_proto_ws2dp_auction_del_bid_record msg;
		msg.set_item_guid(item_guid.server_64);

		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_del_bid_record);

		//ws2dp_auction_del_bid_record msg;
		//msg.item_guid = item_guid;

		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_del_bid_record));
	}

	void auction_mgr_ws::clear_old_bid_record()
	{
		server2dp_proto_ws2dp_auction_clear_old_bid_record msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_clear_old_bid_record);

		//ws2dp_auction_clear_old_bid_record msg;

		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_clear_old_bid_record));
	}


	void auction_mgr_ws::req_share_list_end_proc(const int32& money_type, const int32& money_value, const int32& item_template_id,
		const s_auction_share_info* share_info, const int32& data_num, 
		const guid_64& buyer_guid)
	{
		if (share_info == nullptr
			|| data_num == 0
			|| money_value == 0)
		{
			return;
		}
		if (is_use_lua())
		{
			return;
		}

		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		s_mail_info temp_mail_info;
		std::vector<guid_64> temp_item_guid_array;

		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		if (sender_name == nullptr)
		{
			return;
		}
		int32 sender_name_len = std::strlen(sender_name);
		if (sender_name_len > max_name_size)
		{
			sender_name_len = max_name_size;
		}
		init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);
		memcpy(temp_mail_info.sender_name, sender_name, sender_name_len);

		int32 share_money_value = money_value;
		if (share_money_value > get_config_param(e_auction_param_share_money_limit))
		{
			share_money_value = get_config_param(e_auction_param_share_money_limit);
		}
		float deduct_num = 0;
		if (GAMECONFIG->AuctionShareRate.size() >= 5 && GAMECONFIG->AuctionShareRate.size() % 5 == 0)
		{
			int32 world_level = ranking_mgr_ws::get_world_level_cur();
			std::vector<int32> share_rate = GAMECONFIG->AuctionShareRate;
			int32 cur_region_code = region_template_ptr->RegionCode;
			int32 cur_server_cross = world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross);
			int32 arr_len = GAMECONFIG->AuctionShareRate.size();
			for (int32 i = 0; i < (arr_len / 5); i++)
			{
				int32 need_code = share_rate[i * 5];
				int32 level_max = share_rate[i * 5 + 1];
				int32 level_min = share_rate[i * 5 + 2];
				int32 need_cross = share_rate[i * 5 + 3];
				if (need_code != -1 && need_code != cur_region_code)
				{
					continue;
				}
				if (need_cross != -1 && cur_server_cross != need_cross)
				{
					if (need_cross == 0 && cur_server_cross != 0)
					{
						continue;
					}
					else if (need_cross > 0 && cur_server_cross == 0)
					{
						continue;
					}
				}
				if (level_max != -1 && level_min != -1 && (world_level > level_max || world_level < level_min))
				{
					continue;
				}
				deduct_num = share_money_value * (float)share_rate[i * 5 + 4] / (float)100 - 0.5f;
				break;
			}
		}

		share_money_value -= deduct_num;

		temp_mail_info.data_ary[EMailInfo_MoneyTyp1] = money_type;
		for (int32 i = 0; i < data_num; ++i)
		{
			//if (share_info[i].role_guid == buyer_guid)
			//{
			//	continue;
			//}
			temp_mail_info.mail_guid = guid_gen::make_guid(share_info[i].role_guid);
			int32 mail_title_len = std::strlen(share_info[i].share_mail_title);
			if (mail_title_len > max_mail_title_length)
			{
				mail_title_len = max_mail_title_length;
			}
			memcpy(temp_mail_info.mail_title, share_info[i].share_mail_title, mail_title_len);

			int32 mail_content_len = std::strlen(share_info[i].share_mail_content);
			if (mail_content_len > max_text_contents_length)
			{
				mail_content_len = max_text_contents_length;
			}
			memcpy(temp_mail_info.text_contents, share_info[i].share_mail_content, mail_content_len);

			float temp_money_value = share_money_value * share_info[i].share_rate + 0.5f;
			//+0.5 ��������

			int32 share_money_num = temp_money_value;
			if (share_money_num <= 0)
			{
				share_money_num = 1;
			}
			temp_mail_info.data_ary[EMailInfo_MoneyNum1] = share_money_num;
			mail_ws_mgr::get_instance().send_mail(share_info[i].role_guid, 0, temp_mail_info, nullptr, 0);
		}

		if (buyer_guid.is_valid())
		{
			client_session* session_player = client_session_mgr::getInstance().get_session(buyer_guid);
			if (session_player == nullptr)
			{
				return;
			}
			server_log::auctionbid_log(session_player->m_login_third_data, session_player->m_role_info, money_type, money_value, item_template_id, data_num, buyer_guid);
		}
	}


	void auction_mgr_ws::force_clear_all_old_item()
	{
		m_is_clearing_old_item = false;
		m_is_force_clear_old_item = true;
	}

	void auction_mgr_ws::check_world_auction_sell()
	{
		//ws2dp_auction_check_world_sell msg;

		faith::server2dp_proto::ws2dp_auction_check_world_sell pro_msg;

		ws_client::getInstance().send_to_dp_new(&pro_msg, e_msgindex_ws2dp_auction_check_world_sell);


		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_check_world_sell));
	}

	void auction_mgr_ws::set_world_auction_sell_num(int32 sell_num)
	{

		m_world_sell_num = sell_num;
	}

	void auction_mgr_ws::set_need_world_acution_check(bool is_need)
	{

		m_need_check_world_auction = is_need;
	}

	void auction_mgr_ws::dp_check_world_auction_sell_end(int32 sell_num)
	{
		if (is_use_lua())
		{
			return;
		}
		set_world_auction_sell_num(sell_num);

		bool is_world_sell = sell_num > 0 ? true : false;
		set_need_world_acution_check(is_world_sell);

		auction_proto_auction_sell_flag msg;
		msg.set_sell_flag(is_world_sell);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_auction_sell_flag);
	}

	void auction_mgr_ws::set_is_show(s_auction_info &auction_info)
	{
		auction_info.is_show = 1;
	}

	void auction_mgr_ws::add_wait_show(s_auction_timer_info &auctin_timer_info)
	{
		int32 start = get_config_param(e_auction_param_private_delay_start);
		int32 end = get_config_param(e_auction_param_private_delay_end);
		auctin_timer_info.show_time = time_helper::get_time() + random_gen::get_random(start, end);
		m_wait_show_vec.push_back(auctin_timer_info);
	}
	void auction_mgr_ws::wait_tick()
	{
		if (m_wait_show_vec.size() == 0)
		{
			return;
		}
		int32 cur_sec = time_helper::get_time();
		for (wait_show_vector_it it = m_wait_show_vec.begin(); it != m_wait_show_vec.end(); it++)
		{
			if (it->show_time < cur_sec && it->is_update == 0)
			{
				send_to_dp_show(it->auction_info);
				it->is_update = 1;
			}
		}
	}
	s_auction_timer_info& auction_mgr_ws::find_timer(guid_64 item_guid)
	{
		for (wait_show_vector_it it = m_wait_show_vec.begin(); it != m_wait_show_vec.end(); it++)
		{
			if (it->auction_info.item_info.item_guid == item_guid)
			{
				return *it;
			}
		}
		return m_empty;

	}
	void auction_mgr_ws::del_timer(guid_64 item_guid)
	{
		wait_show_vector_it res = m_wait_show_vec.end();
		for (wait_show_vector_it it = m_wait_show_vec.begin(); it != m_wait_show_vec.end(); it++)
		{
			if (it->auction_info.item_info.item_guid == item_guid)
			{
				res = it;
				break;
			}
		}
		if (res != m_wait_show_vec.end())
		{
			m_wait_show_vec.erase(res);
		}
	}
	void auction_mgr_ws::send_to_dp_show(s_auction_info auction_info)
	{
		server2dp_proto_ws2dp_auction_show_sell msg;
		auction_info.set_proto_info(msg.mutable_data_info());
		msg.set_end_time(get_config_param(e_auction_param_private_del));
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_show_sell);

		//ws2dp_auction_show_sell msg;
		//msg.data_info = auction_info;
		//msg.endTime = get_config_param(e_auction_param_private_del);
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_show_sell));
	}

	void auction_mgr_ws::send_to_dp_show_end(int32 result, s_auction_info auction_info)
	{
		if (is_use_lua())
		{
			return;
		}
		s_auction_timer_info& timer_info = find_timer(auction_info.item_info.item_guid);
		if (!timer_info.is_vaild())
		{
			return;
		}
		if (result != 1)
		{
			timer_info.is_update = 0;
		}
		if (timer_info.auction_info.item_color >= e_item_color_orange)
		{
			sell_rare_item_proc_ex(timer_info);
		}

		del_timer(auction_info.item_info.item_guid);

	}
	void auction_mgr_ws::sell_rare_item_proc_ex(const s_auction_timer_info& info)
	{

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, info.auction_info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return;
		}
		if (item_template_ptr->AuctionNotice <= 0)
		{
			return;
		}
		std::vector<int32> notice_param_array;
		notice_param_array.reserve(e_item_info_max);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			notice_param_array.push_back(info.auction_info.item_info.data_ary[i]);
		}

		//�����Ƶ�
		std::vector<std::string> vec_notice;
		vec_notice.push_back(template_manager::get_instance().get_str_id_by_notice_id(93000232));
		vec_notice.push_back(info.auction_info.seller_name);

		//�ṹ���е�nameû����ɫ����
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
		vec_notice.push_back(temp_string);

		std::string notice_str = init_unit::implode(vec_notice);
		event_ws_mgr::get_instance().send_notice_to_all(93000232, time_helper::get_cur_time_new().second, info.auction_info.seller_guid, notice_str, info.template_id);

		//����Ƶ����
		event_ws_mgr::get_instance().send_notice_with_param(
			info.auction_info.seller_guid, info.auction_info.seller_name,
			"90089566",
			notice_param_array, e_notice_param_item_info,
			e_chat_type_world, info.template_id, 93000233);
	}

	void auction_mgr_ws::time_out_tick(const int64& new_time)
	{
		
		if (m_time_out_last == 0)
		{
			m_time_out_last = new_time;
	    }
		if (is_request_time_out == true)
		{
			return;
		}
		if (new_time - m_time_out_last > auction_time_out_interval)
		{
			m_time_out_last = new_time;
			send_to_dp_time_out(m_is_first);
			m_is_first = 0;
			is_request_time_out = true;
		}

	}
	void auction_mgr_ws::send_to_dp_time_out(int32 is_first)
	{
		if (is_first == 1)
		{
			m_wait_show_vec.clear();
		}
		int32 cur_time = time_helper::get_time();

		server2dp_proto_ws2dp_auction_time_out msg;
		msg.set_cur_mil_sec((int64)(time_helper::get_time() - get_config_param(e_auction_param_private_del)) * (int64)second_tick_time);
		msg.set_sell_time((int64)time_helper::get_time() * (int64)second_tick_time);
		msg.set_deal_time((int64)(time_helper::get_time() + get_config_param(e_auction_param_private_del)) * (int64)second_tick_time);
		msg.set_is_first(is_first);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_auction_time_out);

		//ws2dp_auction_time_out msg;

		//msg.cur_mil_sec = (time_helper::get_time() - get_config_param(e_auction_param_private_del));
		//msg.cur_mil_sec *= second_tick_time;

		//msg.sell_time = time_helper::get_time();
		//msg.sell_time *= second_tick_time;
		//msg.deal_time = (time_helper::get_time() + get_config_param(e_auction_param_private_del));
		//msg.deal_time *= second_tick_time;
		//msg.is_first = is_first;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_auction_time_out));
	}

	void auction_mgr_ws::send_to_dp_time_out_end(const s_auction_info *aution_info_array, int32 num)
	{
		if (is_use_lua())
		{
			return;
		}
		is_request_time_out = false;
		if (nullptr == aution_info_array || num == 0)
		{
			return;
		}
		for (int32 i = 0; i < num; ++i)
		{
			send_to_owner(aution_info_array[i]);
			add_record_to_db(aution_info_array[i], aution_info_array[i].seller_guid, 0, true);
			sync_to_cs_del(aution_info_array[i]);
		}
	}

	void auction_mgr_ws::cancel_del_show(const s_auction_info& auction_info)
	{
		if (auction_info.is_show == 1)
		{
			return;
		}
		del_timer(auction_info.item_info.item_guid);
	}

	void auction_mgr_ws::send_to_owner(const s_auction_info& auction_info)
	{
		give_item_to_seller(auction_info);
	}
	
	void auction_mgr_ws::give_item_to_seller(const s_auction_info& auction_info)
	{
		if (!auction_info.seller_guid.is_valid())
		{
			return;
		}
		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_time_out_title);
		const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_time_out_content);

		if (sender_name == nullptr
			|| mail_title == nullptr
			|| mail_content == nullptr)
		{
			return;
		}
		s_mail_info temp_mail_info;

		std::vector<guid_64> temp_item_guid_array;
		temp_item_guid_array.push_back(auction_info.item_info.item_guid);

		init_unit::init_mail_data_array(temp_mail_info, temp_item_guid_array, 0, 0, 0, 0, 0);

		temp_mail_info.set_sender_name(sender_name);
		temp_mail_info.set_mail_title(mail_title);
		temp_mail_info.set_mail_content_text(mail_content);
		mail_ws_mgr::get_instance().send_mail(auction_info.seller_guid, 0, temp_mail_info, &auction_info.item_info, 1);

	}
	void auction_mgr_ws::give_money_to_seller(const s_auction_info& auction_info)
	{
		VIPTemplate* seller_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(auction_info.seller_vip_level);
		if (seller_vip_template_ptr == nullptr)
		{
			return;
		}

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, auction_info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return;
		}

		int32 after_tax_reward = (float)auction_info.cur_bid_money * seller_vip_template_ptr->AuctionRewardPercentage;

		std::vector<s_item_info> item_list;
		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		const xchar* seller_mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_seller_title);
		const xchar* seller_mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_seller_content);
		if (sender_name == nullptr
			|| seller_mail_title == nullptr
			|| seller_mail_content == nullptr)
		{
			return;
		}

		std::vector<string> sell_mail_content_array;
		sell_mail_content_array.push_back(seller_mail_content);
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
		sell_mail_content_array.push_back(temp_string);

		event_ws_mgr::get_instance().send_mail_to_player_by_system(auction_info.seller_guid, 0, sender_name, seller_mail_title, init_unit::implode(sell_mail_content_array),
			auction_info.sell_money_type, after_tax_reward, 0, 0, item_list);


		//globle_data::get_instance().send_mail_to_another_player_by_system(
		//	auction_info.seller_guid,
		//	auction_info.seller_guid,
		//	0,
		//	sender_name,
		//	seller_mail_title, init_unit::implode(sell_mail_content_array),
		//	auction_info.sell_money_type, after_tax_reward, 0, 0, nullptr, 0);

	}
	void auction_mgr_ws::sync_to_cs_del(const s_auction_info& auction_info)
	{
		client_session* session_player = client_session_mgr::getInstance().get_session(auction_info.seller_guid);
		if (session_player != nullptr
			&& session_player->get_cs_conn_index() >= 0)
		{
			ws2cs_auction_time_out_del msg;
			msg.role_guid = auction_info.seller_guid;
			msg.auction_info = auction_info;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_time_out_del);
			if (is_use == false)
			{
				session_player->send_to_cs(&msg, sizeof(ws2cs_auction_time_out_del));
			}
			else
			{
				faith::ws2cs_proto::auction_time_out_del pro_msg;
				msg.to_proto(pro_msg);
				session_player->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_time_out_del);
			}
		}
	}

	void auction_mgr_ws::add_tip(const s_auction_info &aution_info)
	{
		m_time_tip_vec.push_back(aution_info);
	}

	bool auction_mgr_ws::is_time_tip(int64 sell_time, int32 cur_time)
	{
		int32 delta_time = cur_time - sell_time / second_tick_time;
		if (delta_time < get_config_param(e_auction_param_wait_notice))
		{
			return false;
		}
		return true;	 
	}

	void auction_mgr_ws::time_tip_tick()
	{ 
		if (m_time_tip_vec.size() == 0)
		{
			return;
		}

		int32 cur_sec = time_helper::get_time();
		for (time_tip_vector_it it = m_time_tip_vec.begin(); it != m_time_tip_vec.end();)
		{
			if (is_time_tip(it->sell_time, cur_sec))
			{
				send_prepare_notice(*it);
				it = m_time_tip_vec.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	void auction_mgr_ws::add_wait_time(s_auction_info &info)
	{
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr != nullptr)
		{
			info.deal_time += item_template_ptr->TradingAwaitTime * second_tick_time;
		}
		else
		{
			info.deal_time += get_config_param(e_auction_param_wait_bid) * second_tick_time;
		}
	}

	void auction_mgr_ws::send_up_notice(const s_auction_info &info)
	{
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return;
		}
		if (item_template_ptr->AuctionNotice <= 0)
		{
			return;
		}
		std::vector<int32> notice_param_array;
		notice_param_array.reserve(e_item_info_max);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			notice_param_array.push_back(info.item_info.data_ary[i]);
		}

		//�����Ƶ�
		std::vector<std::string> vec_notice;
		vec_notice.push_back(template_manager::get_instance().get_str_id_by_notice_id(93000416));

		//�ṹ���е�nameû����ɫ����
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
		vec_notice.push_back(temp_string);

		std::string notice_str = init_unit::implode(vec_notice);
		event_ws_mgr::get_instance().send_notice_to_all(93000416, time_helper::get_cur_time_new().second, guid_64(), notice_str, 0);

		//����Ƶ����
		event_ws_mgr::get_instance().send_notice_with_param(
			guid_64(), "",
			"90089577",
			notice_param_array, e_notice_param_item_info,
			e_chat_type_system, 0, 93000417);

		add_tip(info); // wait_tip
	}
	void auction_mgr_ws::send_prepare_notice(const s_auction_info &info)
	{
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr)
		{
			return;
		}
		if (item_template_ptr->AuctionNotice <= 0)
		{
			return;
		}
		std::vector<int32> notice_param_array;
		notice_param_array.reserve(e_item_info_max);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			notice_param_array.push_back(info.item_info.data_ary[i]);
		}

		//�����Ƶ�
		std::vector<std::string> vec_notice;
		vec_notice.push_back(template_manager::get_instance().get_str_id_by_notice_id(93000418));
	
		//�ṹ���е�nameû����ɫ����
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
		vec_notice.push_back(temp_string);

		std::string notice_str = init_unit::implode(vec_notice);
		event_ws_mgr::get_instance().send_notice_to_all(93000418, time_helper::get_cur_time_new().second, guid_64(), notice_str, 0);

		//����Ƶ����
		event_ws_mgr::get_instance().send_notice_with_param(
			guid_64(), "",
			"90089578",
			notice_param_array, e_notice_param_item_info,
			e_chat_type_system, 0, 93000419);
	}

	void auction_mgr_ws::update_auction_time(s_auction_info &auction_info)
	{
		int64 end_time = auction_info.deal_time / second_tick_time;
		int64 cur_time = time_helper::get_time();
		int64 delta_time = end_time - cur_time;
		if (delta_time < 0)
		{
			return;
		}
		if (delta_time > get_config_param(e_auction_param_add_time))
		{
			auction_info.add_time = 0;
			return;
		}
		if (auction_info.add_time >= get_config_param(e_auction_param_add_n))
		{
			return;
		}
		int32 interval_time = 30;
		if (get_config_param(e_auction_param_add_n) > 0)
		{
			interval_time = get_config_param(e_auction_param_add_time) / get_config_param(e_auction_param_add_n);
		}
		auction_info.add_time += 1;
		auction_info.deal_time += interval_time * second_tick_time;
		delta_time += interval_time;
		if (delta_time > get_config_param(e_auction_param_add_time))
		{
			auction_info.add_time = 0;
		}
	}
	int32 auction_mgr_ws::get_config_param(e_auction_param param_type)
	{
		int32 config_init[e_auction_param_max] = {86400, 60, 600, 600, 540, 180, 6,100,39600,4};
		if (GAMECONFIG->AuctionArray.size() < e_auction_param_max)
		{
			return config_init[param_type];
		}
		return GAMECONFIG->AuctionArray[param_type];
	}

	void auction_mgr_ws::on_login(client_session* session)
	{
		if (nullptr == session)
			return;
		/////////////////////////////////////////////////
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_on_login", 0, false, "%l", session->get_role_guid().server_64);
			return;

		}
		/////////////////////////////////////////

		if (!session->is_self_server())
		{
			return;
		}
		bool is_world_sell = m_world_sell_num > 0 ? true : false;

		auction_proto_auction_sell_flag msg;
		msg.set_sell_flag(is_world_sell);
		session->send_to_client(&msg, e_msgindex_s2c_auction_sell_flag);
	}

	void auction_mgr_ws::auction_bid_db_sucess(const guid_64& buyer_guid, const guid_64& item_guid)
	{
		if (is_use_lua())
		{
			return;
	
		}
		auction_proto_operate_end purchase_end_msg;
		purchase_end_msg.set_item_guid(item_guid.server_64);
		purchase_end_msg.set_item_num(0);
		purchase_end_msg.set_operate_type(e_auction_operate_type_bid);
		purchase_end_msg.set_money_type(0);
		purchase_end_msg.set_money_num(0);
		purchase_end_msg.set_operate_result(e_auction_bid_success);
		client_session* new_session_player = client_session_mgr::getInstance().get_session(buyer_guid);
		if (new_session_player != nullptr &&  new_session_player->get_cs_conn_index() >= 0)
		{
			new_session_player->send_to_client(&purchase_end_msg, e_msgindex_s2c_auction_operate_end);
		}
		//std::cout << "-----auction-----request---success-111---" << utility::get_tick_count() << std::endl;
	}

	void auction_mgr_ws::add_person_info(const cs2ws_auction_request_refresh_person *p_data)
	{
		if (p_data == nullptr)
		{
			return;
		}
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_add_person_info", 0, false, "%l%d%d%l", p_data->role_guid.server_64, p_data->role_class_type, p_data->role_lv, p_data->item_time);
			return;
		}

		s_auction_person_info temp; 
		temp.role_guid = p_data->role_guid;
		temp.role_lv = p_data->role_lv;
		temp.role_class_type = p_data->role_class_type;
		temp.item_time = p_data->item_time;
		m_person_info_queue.push(temp);
	}


	void auction_mgr_ws::person_tick()
	{
		int32 size = m_person_info_queue.size();
		if (size == 0)
		{
			return;
		}
		int32 enter_count = 0;

		while (!m_person_info_queue.empty())
		{
			s_auction_person_info temp = m_person_info_queue.front();
			m_person_info_queue.pop();
			process_person_info(temp);
			enter_count++;
			if (enter_count == auction_one_frame_person_num)
			{
				break;
			}
		}
	}

	void auction_mgr_ws::process_person_info(s_auction_person_info& person_info)
	{
		 std::vector<s_item_template_info> _item_list;
		 globle_data::get_instance().get_item_list_by_drop_bg((70022100 + person_info.role_class_type), _item_list, 0, person_info.role_lv);

		 for (auto& it : _item_list)
		 {
			 make_person_auction(person_info, it.m_item_id);
		 }
	}
	void auction_mgr_ws::make_person_auction(s_auction_person_info& person_info, int32 item_id)
	{
		// ϵͳ�ϼ�
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (item_template_ptr == nullptr)
		{
			return;
		}

		s_item_info new_item_info;
		init_unit::init_item_data(new_item_info, item_id, -1, 1, e_bag_type_bag, 0, 0);
		s_auction_info new_auction_info;
		new_auction_info.clear_data();
		if (globle_data::get_instance().make_auction_info(new_auction_info, new_item_info, 1, e_trading_type_world, guid_64()) == false)
		{
			return;
		}
		/////////////////////////////////////////////
		new_auction_info.is_show = 1;
		new_auction_info.show_guid = person_info.role_guid;
		
		//////////////////////////////////////

		add_auction_info_to_db(new_auction_info, guid_64(), s_unit_info(), true);

	}

	int32 auction_mgr_ws::get_area_index(int32 role_lv)
	{
		static int32  range[12] = { 0, 199, 200, 299, 300, 399, 400, 499, 500, 599, 600,9999};
		for (int32 i = 0 ; i < 6; i++)
		{
			if (range[i * 2] <= role_lv &&  role_lv <= range[i * 2 + 1])
			{
				return i;
			}
		}
		return -1;
	}
	const std::vector<int32> * auction_mgr_ws::get_item_config(int32 role_class_type)
	{
		const std::vector<int32> *p_vec = nullptr;

		switch (role_class_type)
		{
		case e_class_type_barserker:
			p_vec = &GAMECONFIG->BarserkerAuctionEquRandomSet;
			break;
		case e_class_type_wizard:
			p_vec = &GAMECONFIG->WizardAuctionEquRandomSet;
			break;
		case e_class_type_guardian:
			p_vec = &GAMECONFIG->GuardianAuctionEquRandomSet;
			break;
		case e_class_type_assassinator:
			p_vec = &GAMECONFIG->AssassinatorAuctionEquRandomSet;
			break;
		default:
			break;
		}
		return p_vec;
	}
	int32 auction_mgr_ws::get_item_id(const std::vector<int32> *p_vec, int32 idx)
	{
		if (p_vec == nullptr || p_vec->size() != 54)
		{
			return 0;
		}
		if (idx < 0 || idx >= 6)
		{
			return 0;
		}
		int32 random_pos = random_gen::get_random(0, 8);

		int32 item_id = (*p_vec)[idx * 9 + random_pos];

		return item_id;

	}
	bool auction_mgr_ws::is_use_lua()
	{
		static int is_get = 0;
		static bool getRet = false;
		if (is_get == 0)
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "is_auciton_use_lua", 1, false, ">%b", &bRet);
			getRet = bRet;
			is_get = 1;
			return bRet;
		}
		return getRet;
	}

	packet_s2s *auction_mgr_ws::get_auction_buffer( s_auction_info& auction_info)
	{
		faith::st_proto::st_auction_info pro_msg;
		auction_info.to_proto(&pro_msg);
		packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
		return p_s2s;
	}

}
