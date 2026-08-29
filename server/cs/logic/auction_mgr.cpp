/********************************************************************
	file name:	auction_cs.cpp
	created:	2017/06/11 0:14
	author:		zhy
	purpose:	
*********************************************************************/

#include "time.hpp"
#include "connection_mgr.hpp"
#include "auction_mgr.h"
#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "template/StringConst_S.h"
#include "server_log.hpp"
#include "invalid_word/invalid_ansi_word.h"
#include "lua/script_mgr.h"
#include "system/item/item_system.h"
#include "components/item/item_base_component.h"
#include "utility/parse_msg.h"
#include "internet/net.pb.h"
#include "utility/globle_data.h"
#include "utility/init_unit.h"


namespace faith
{
	cauction_mgr::cauction_mgr()
	{
		m_unit_array_index = 0;
		clear_data();
	}

	cauction_mgr::~cauction_mgr()
	{

	}

	void cauction_mgr::clear_data()
	{
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			m_selling_array[i].clear_data();
		}
		m_find_info.clear_data();
		m_req_record_info.clear_data();

		is_load = 0;
	/*	m_bid_record_vec.clear();*/
		m_next_interval = 0;

		m_refresh_time = 0;
		m_refresh_info.second = 0;

	}

	void cauction_mgr::heart_tick(int64 new_time)
	{
		if (m_find_info.is_waiting_find
			&& new_time > m_find_info.next_can_find_time)
		{
			start_find_list();
		}

		if (m_req_record_info.is_waiting_find
			&& new_time > m_req_record_info.next_can_find_time)
		{
			start_req_record_list();
		}
		//if (is_load == 0)
		//{
		//	//req_bid_record_failure(0);
		//	is_load = 1;
		//}
		//person_tick(new_time);
	}

	int32 cauction_mgr::get_empty_sell_slot()
	{
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			if (!m_selling_array[i].item_info.item_guid.is_valid())
			{
				return i;
			}
		}
		return -1;
	}

	void cauction_mgr::operate(int32 operate_type, guid_64 item_guid, int32 sell_num, int32 price_type, int32 price_value)
	{


		int32 ret = 0;

		script_mgr::get_instance().call_func("auction_lua_mgr", "operate_cpp", 1, false, "%d%d%d%d%d%d%d>%d", m_unit_array_index, operate_type, item_guid.A, item_guid.B, sell_num, price_type, price_value, &ret);
		

		/*	switch (operate_type)
			{
			case e_auction_operate_type_non:
				break;
			case e_auction_operate_type_sell:
				sell_item(item_guid, sell_num, price_type, price_value);
				break;
			case e_auction_operate_type_purchase:
				buy_item(item_guid);
				break;
			case e_auction_operate_type_cancel_sell:
				cancel_sell_item(item_guid);
				break;
			case e_auction_operate_type_bid:
				bid_item(item_guid, price_type, price_value);
				break;
			case e_auction_operate_type_read_record:
				read_record(item_guid,price_type);
			default:
				break;
			}*/
	}

	void cauction_mgr::sell_item(guid_64 item_guid, int32 sell_num, int32 price_type, int32 price_value)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			CONSOLE_ERROR("temp_player is invalid");
			return;
		}
		auto item_ent =get_entity(item_guid);
		if (item_ent == nullptr)
		{
			CONSOLE_ERROR("item_ent is nullptr");
			return;
		}
		auto item_base_cp = item_ent->get_component<item_base_component>();
		if (item_base_cp->m_data_array[e_item_base_locked] > 0)
		{
			CONSOLE_ERROR("item is locked");
			return;
		}
		auto item_template = item_base_cp->m_item_template;
		//////////////////////////////////////////////////////////////
		/*if (item_template->AuctionLabType > 4)
		{*/
			if (item_template->AuctionCanSellMoneyArray.size() <= 0)
			{
				return;
			}

			bool is_price_money_type_can_use = false;
			for (int32 i = 0; i < item_template->AuctionCanSellMoneyArray.size(); ++i)
			{
				if (price_type == item_template->AuctionCanSellMoneyArray[i])
				{
					is_price_money_type_can_use = true;
					break;
				}
			}

			if (!is_price_money_type_can_use)
			{
				return;
			}

			if (sell_num <= 0 || sell_num > item_base_cp->m_data_array[e_item_base_count])
			{
				sell_num = item_base_cp->m_data_array[e_item_base_count];
			}
			int32 tolal_sell_price_min = 0;
			if (price_type < item_template->AuctionMinMoneyArray.size())
			{
				tolal_sell_price_min = item_template->AuctionMinMoneyArray[price_type] * sell_num;
			}
			if (tolal_sell_price_min < GAMECONFIG->AuctionSellTotalPriceMin)
			{
				tolal_sell_price_min = GAMECONFIG->AuctionSellTotalPriceMin;
			}

			if (price_value < tolal_sell_price_min)
			{
				return;
			}
		//}
		/////////////////////////////////////////////////////

		int32 empty_selling_index = get_empty_sell_slot();
		if (empty_selling_index < 0 || empty_selling_index >= max_auction_sell_num)
		{
			return;
		}
		//////////////////////////////
		s_auction_info temp_info;
		//if (!globle_data::get_instance().make_auction_info(temp_info, temp_item->get_item_inst(), sell_num, e_trading_type_world,
		//	temp_player.get_unit_guid(), temp_player.get_name(), temp_player.get_vip_level(),
		//	price_type, price_value))
		//{
		//	return;
		//}

		set_is_show(temp_info);

		m_selling_array[empty_selling_index] = temp_info;
		item_system::cost_item_by_ent(item_ent, sell_num);

		cs2ws_auction_add_item_into_db sell_msg;
		sell_msg.role_info = temp_player.get_unit_info_inst();
		sell_msg.auction_info = temp_info;
		sell_msg.old_item_guid = item_guid;
		sell_msg.third_info = temp_player.get_third_info();
		connection_mgr::getInstance().send_to_ws( &sell_msg, sizeof(cs2ws_auction_add_item_into_db));
	}

	void cauction_mgr::cancel_sell_item(guid_64 item_guid)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		bool is_in_selling_array = false;
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			if (m_selling_array[i].item_info.item_guid == item_guid)
			{
				if (m_selling_array[i].buyer_guid.is_valid())
				{
					return;
				}
				is_in_selling_array = true;
				break;
			}
		}
		if (!is_in_selling_array)
		{
			auction_proto_operate_end cancel_sell_end_msg;
			cancel_sell_end_msg.set_item_guid(item_guid.server_64);
			cancel_sell_end_msg.set_item_num(0);
			cancel_sell_end_msg.set_operate_type(e_auction_operate_type_cancel_sell);
			cancel_sell_end_msg.set_money_type(0);
			cancel_sell_end_msg.set_money_num(0);
			cancel_sell_end_msg.set_operate_result(e_auction_cancel_sell_already_sell);
			temp_player.send_message_to_self(&cancel_sell_end_msg, e_msgindex_s2c_auction_operate_end);
			return;
		}

		cs2ws_auction_cancel_sell cancel_sell_msg;
		cancel_sell_msg.role_guid = temp_player.get_unit_guid();
		cancel_sell_msg.item_guid = item_guid;
		connection_mgr::getInstance().send_to_ws( &cancel_sell_msg, sizeof(cs2ws_auction_cancel_sell));
	}

	void cauction_mgr::cancel_sell_item_end(s_auction_info sell_info)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (!sell_info.item_info.item_guid.is_valid())
		{
			return;
		}
		remove_from_selling_list(sell_info.item_info.item_guid);
		//citem* temp_item = temp_player.get_item_set().create_item_by_info(sell_info.item_info, e_server_log_add_item_cancel_sell, 0, true);
		//if (temp_item == nullptr)
		//{
		//	return;
		//}
		//guid_64 item_guid = temp_item->get_item_guid();
		//temp_player.get_item_set().put_in_bag(temp_item, e_bag_type_bag, e_item_mail_type_auction_cancel_sell_item);

		auction_proto_operate_end cancel_sell_end_msg;
		cancel_sell_end_msg.set_item_guid(sell_info.item_info.item_guid.server_64);
		cancel_sell_end_msg.set_item_num(0);
		cancel_sell_end_msg.set_operate_type(e_auction_operate_type_cancel_sell);
		cancel_sell_end_msg.set_money_type(0);
		cancel_sell_end_msg.set_money_num(0);
		cancel_sell_end_msg.set_operate_result(e_auction_cancel_sell_success);
		temp_player.send_message_to_self(&cancel_sell_end_msg, e_msgindex_s2c_auction_operate_end);

		//return_money_to_old_buyer(sell_info.buyer_guid, sell_info.item_info.data_ary[e_item_info_info_id], sell_info.sell_money_type, sell_info.cur_bid_money);

		server_log::auction_db_log(sell_info, e_auction_state_cancel, temp_player.get_unit_info_inst(), temp_player.get_third_info(), temp_player.get_login_type());

		set_log_var(log_head);

		temp_player.get_log_common_head_info(log_head);

		server_log::serverAuction(log_head, temp_player.get_account(),init_unit::change_i64_to_string(temp_player.get_unit_guid().server_64), temp_player.get_name(), temp_player.get_unit_info(e_role_info_exp_level),
			init_unit::change_i64_to_string(sell_info.item_info.item_guid.server_64), sell_info.item_info.data_ary[e_item_info_info_id], sell_info.item_info.data_ary[e_item_info_stack_count],
			sell_info.trade_type, "null", "null", "null", 0, sell_info.sell_money_type, globle_data::get_instance().get_money_str_by_type(sell_info.sell_money_type), sell_info.sell_total_money, e_auction_state_cancel);
	}

	void cauction_mgr::sell_item_end(s_auction_info sell_info, int32 sell_result)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		auction_proto_operate_end sell_end_msg;
		sell_end_msg.set_item_guid(sell_info.item_info.item_guid.server_64);
		sell_end_msg.set_item_num(sell_info.item_info.data_ary[e_item_info_stack_count]);
		sell_end_msg.set_operate_type(e_auction_operate_type_sell);
		sell_end_msg.set_money_type(sell_info.sell_money_type);
		sell_end_msg.set_money_num(sell_info.sell_total_money);
		sell_end_msg.set_operate_result(sell_result);

		if (sell_result != e_auction_sell_success)
		{
			const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_sell_failed_title);
			const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_sell_failed_content);

			if (sender_name == nullptr
				|| mail_title == nullptr
				|| mail_content == nullptr)
			{
				return;
			}

			globle_data::get_instance().send_mail_to_another_player_by_system(
				temp_player.get_unit_guid(), 
				temp_player.get_unit_guid(), 
				temp_player.get_unit_info(e_role_info_server_id), 
				sender_name, mail_title, mail_content,
				0, 0, 0, 0, &sell_info.item_info, 1);

			remove_from_selling_list(sell_info.item_info.item_guid);
		}
		else
		{
			send_selling_list_one(sell_info.item_info.item_guid);
		}

		temp_player.send_message_to_self(&sell_end_msg, e_msgindex_s2c_auction_operate_end);



		temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_auction_sell);

		temp_player.get_mission_mgr().target_check(e_mission_end_type_auction_goods);


	}

	void cauction_mgr::sell_item_success(s_auction_info sell_info)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		remove_from_selling_list(sell_info.item_info.item_guid);
		faith::auction_proto_receieve_sell_success_info msg;
		msg.set_item_template_id(sell_info.item_info.data_ary[e_item_info_info_id]);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_receieve_sell_success);
	}

	void cauction_mgr::make_package_info(auction_proto_auction_item_info* target_info, s_auction_info source_info)
	{
		target_info->set_seller_guid(source_info.seller_guid.server_64);
		target_info->set_seller_name(source_info.seller_name);
		target_info->set_buyer_guid(source_info.buyer_guid.server_64);
		target_info->set_buyer_name(source_info.buyer_name);
		target_info->set_trade_type(source_info.trade_type);
		target_info->set_seller_vip_level(source_info.seller_vip_level);
		target_info->set_auction_tab(source_info.auction_type);
		target_info->set_auction_sub_tab(source_info.auction_sub_type);
		target_info->set_item_color(source_info.item_color);
		target_info->set_quality_level(source_info.quality_level);
		target_info->set_class_limit(source_info.class_limit);
		target_info->set_item_name(source_info.item_name);
		target_info->set_sell_time(source_info.sell_time);
		target_info->set_deal_time(source_info.deal_time);
		target_info->set_sell_money_type(source_info.sell_money_type);
		target_info->set_sell_total_money(source_info.sell_total_money);
		target_info->set_sell_single_money(source_info.sell_single_money);
		target_info->set_cur_bid_money(source_info.cur_bid_money);

		target_info->set_item_guid(source_info.item_info.item_guid.server_64);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			target_info->add_item_info(source_info.item_info.data_ary[i]);
		}
		target_info->set_is_show(source_info.is_show);
	}

	void cauction_mgr::send_selling_list_one(int32 list_index)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (list_index < 0 || list_index >= max_auction_sell_num)
		{
			return;
		}
		if (!m_selling_array[list_index].item_info.item_guid.is_valid())
		{
			return;
		}
		auction_proto_update_selling_auction_list msg;
		msg.set_is_need_clean(false);

		auction_proto_auction_item_info* item_info = msg.add_item_list();
		make_package_info(item_info, m_selling_array[list_index]);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_selling_info);
	}

	void cauction_mgr::send_selling_list_all()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		auction_proto_update_selling_auction_list msg;
		msg.set_is_need_clean(true);
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			if (!m_selling_array[i].item_info.item_guid.is_valid())
			{
				continue;
			}
			auction_proto_auction_item_info* item_info = msg.add_item_list();
			make_package_info(item_info, m_selling_array[i]);
			int32 pak_size = msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_selling_info);
				msg.set_is_need_clean(false);
				msg.clear_item_list();
			}
		}
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_selling_info);
	}

	void cauction_mgr::send_selling_list_one(guid_64 item_guid)
	{
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			if (m_selling_array[i].item_info.item_guid == item_guid)
			{
				send_selling_list_one(i);
				break;
			}
		}
	}

	void cauction_mgr::load_self_selling_from_db(const s_auction_info* auction_info, int32 data_num)
	{
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			m_selling_array[i].clear_data();
		}

		for (int32 i = 0; i < data_num; ++i)
		{
			int32 temp_slot = get_empty_sell_slot();
			if (temp_slot < 0
				|| temp_slot >= max_auction_sell_num)
			{
				break;
			}
			m_selling_array[i] = auction_info[i];
		}
		send_selling_list_all();
	}
	bool cauction_mgr::load_self_selling_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::db_proto::auction_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_auction_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_auction_info *p_row = (s_auction_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::db_proto::auction_row db_row = msg.row_data(i);

			p_row->seller_guid.server_64 = db_row.seller_guid();
			parse_msg::getInstance().my_memcopy_string(p_row->seller_name, max_name_size, db_row.seller_name());
			
			p_row->buyer_guid.server_64 = db_row.buyer_guid();
			parse_msg::getInstance().my_memcopy_string(p_row->buyer_name, max_name_size, db_row.buyer_name());

			p_row->trade_type = db_row.trade_type();
			p_row->seller_vip_level = db_row.seller_vip_level();
			p_row->auction_type = db_row.auction_type();
			p_row->auction_sub_type = db_row.auction_sub_type();
			p_row->item_color = db_row.item_color();
			p_row->quality_level = db_row.quality_level();
			p_row->class_limit = db_row.class_limit();
			parse_msg::getInstance().my_memcopy_string(p_row->item_name, max_item_name_size, db_row.item_name());

			p_row->sell_time = db_row.sell_time();
			p_row->deal_time = db_row.deal_time();
			p_row->sell_money_type = db_row.sell_money_type();
			p_row->sell_total_money = db_row.sell_total_money();
			p_row->sell_single_money = db_row.sell_single_money();
			p_row->cur_bid_money = db_row.cur_bid_money();
			p_row->is_show = db_row.is_show();
			p_row->add_time = db_row.add_time();
			p_row->is_sky_order = db_row.is_sky_order();
			p_row->show_guid.server_64 = db_row.show_guid();

			faith::db_proto::item_info_db item_ref = db_row.item_info();

			p_row->item_info.item_guid.server_64 = item_ref.item_guid();

			for (int32 j = 0; j < item_ref.data_ary_size(); j++)
			{
				p_row->item_info.data_ary[j] = item_ref.data_ary(j);
			}
			parse_msg::getInstance().my_memcopy_string(p_row->item_info.param_str, max_recharge_order_length, item_ref.param_str());

			p_row++;
		}

		load_self_selling_from_db((const s_auction_info *)p_data, msg.row_count());
		return true;

	}

	void cauction_mgr::req_goods_list_end(const s_auction_info* auction_info, int32 data_num, int32 cur_page, int32 total_num)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		auction_proto_req_auction_list_end msg;
		msg.set_cur_page(cur_page);
		msg.set_max_info_num(total_num);
		msg.set_is_need_clear(true);
		msg.set_is_final(false);
		for (int32 i = 0; i < data_num; ++i)
		{
			auction_proto_auction_item_info* temp_info = msg.add_item_list();
			make_package_info(temp_info, auction_info[i]);
			int32 pak_size = msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_auction_list_end);
				msg.set_is_need_clear(false);
				msg.clear_item_list();
			}
		}
		msg.set_is_final(true);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_auction_list_end);
	}

	void cauction_mgr::buy_item(guid_64 item_guid)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		temp_player.get_star_trip_mgr().target_mission(e_star_trip_target_type_bid);

		cs2ws_auction_buy buy_msg;
		buy_msg.role_guid = temp_player.get_unit_guid();
		buy_msg.item_guid = item_guid;
		connection_mgr::getInstance().send_to_ws( &buy_msg, sizeof(cs2ws_auction_buy));
	}
	void cauction_mgr::buy_set_bind(s_auction_info &auction_info)
	{
		auction_info.item_info.data_ary[e_item_info_locked] = 1;

	}

	void cauction_mgr::buy_item_end(s_auction_info item_info)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		if (item_info.deal_time > 0
			&& item_info.deal_time < utility::get_tick_count())
		{
			auction_proto_operate_end purchase_end_msg;
			purchase_end_msg.set_item_guid(item_info.item_info.item_guid.server_64);
			purchase_end_msg.set_item_num(0);
			purchase_end_msg.set_operate_type(e_auction_operate_type_purchase);
			purchase_end_msg.set_money_type(0);
			purchase_end_msg.set_money_num(0);
			purchase_end_msg.set_operate_result(e_auction_purchase_already_purchased);
			temp_player.send_message_to_self(&purchase_end_msg, e_msgindex_s2c_auction_operate_end);

			cs2ws_auction_clear_selling_lock clear_msg;
			clear_msg.item_guid = item_info.item_info.item_guid;
			connection_mgr::getInstance().send_to_ws( &clear_msg, sizeof(cs2ws_auction_clear_selling_lock));
			return;
		}

		if (!temp_player.can_cut_money(e_money_type(item_info.sell_money_type), item_info.sell_total_money))
		{
			cs2ws_auction_add_item_into_db sell_msg;
			sell_msg.auction_info = item_info;
			connection_mgr::getInstance().send_to_ws( &sell_msg, sizeof(cs2ws_auction_add_item_into_db));
			return;
		}
		if ((item_info.trade_type == e_trading_type_legion || item_info.trade_type == e_trading_type_world))
		{
			int32 delta = (utility::get_tick_count() - item_info.sell_time) / second_tick_time;
			int32 tem_deal_time = get_config_param(e_auction_param_wait_bid);
			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_info.item_info.data_ary[e_item_info_info_id]);
			if (item_template_ptr != nullptr)
			{
				tem_deal_time = item_template_ptr->TradingAwaitTime;
			}

			if (item_info.sell_total_money == 0 || delta < tem_deal_time)
			{
				cs2ws_auction_add_item_into_db sell_msg;
				sell_msg.auction_info = item_info;
				connection_mgr::getInstance().send_to_ws(&sell_msg, sizeof(cs2ws_auction_add_item_into_db));
				return;
			}
		}
		//if (item_info.trade_type == e_trading_type_normal_sell && item_info.sell_total_money == 0)
		//{
		//	cs2ws_auction_add_item_into_db sell_msg;
		//	sell_msg.auction_info = item_info;
		//	connection_mgr::getInstance().send_to_ws(&sell_msg, sizeof(cs2ws_auction_add_item_into_db));
		//	return;
		//}

		VIPTemplate* seller_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(item_info.seller_vip_level);
		if (seller_vip_template_ptr == nullptr)
		{
			return;
		}

		guid_64 old_buyer_guid = item_info.buyer_guid;
		int32 old_bid_money_type = item_info.sell_money_type;
		int32 old_bid_money_value = item_info.cur_bid_money;

		item_info.deal_time = utility::get_tick_count();
		memcpy(item_info.buyer_name, temp_player.get_name(), max_name_size);
		item_info.buyer_guid = temp_player.get_unit_guid();
		temp_player.cut_money(e_money_type(item_info.sell_money_type), item_info.sell_total_money, e_server_log_cut_money_auction_buy);
		item_info.cur_bid_money = item_info.sell_total_money;

		int32 after_tax_reward = item_info.sell_total_money * seller_vip_template_ptr->AuctionRewardPercentage;
		buy_set_bind(item_info);
		send_reward_to_buyer(item_info);
		send_reward_to_seller(item_info, after_tax_reward);
		add_new_record_to_db(item_info, after_tax_reward);

		cs2ws_auction_send_sell_success_info success_info_msg;
		success_info_msg.auction_info = item_info;
		connection_mgr::getInstance().send_to_ws( &success_info_msg, sizeof(cs2ws_auction_send_sell_success_info));

		return_money_to_old_buyer(old_buyer_guid, item_info.item_info.data_ary[e_item_info_info_id], old_bid_money_type, old_bid_money_value);

		auction_proto_operate_end purchase_end_msg;
		purchase_end_msg.set_item_guid(item_info.item_info.item_guid.server_64);
		purchase_end_msg.set_item_num(0);
		purchase_end_msg.set_operate_type(e_auction_operate_type_purchase);
		purchase_end_msg.set_money_type(0);
		purchase_end_msg.set_money_num(0);
		purchase_end_msg.set_operate_result(e_auction_purchase_success);
		temp_player.send_message_to_self(&purchase_end_msg, e_msgindex_s2c_auction_operate_end);

		temp_player.get_first_time_do_mgr().add_do_record(e_first_time_do_type_auction_buy);

		//判断拍卖行购物任务是否完成
		temp_player.get_mission_mgr().target_check(e_mission_end_type_auction_shopping);

		int32 item_num = item_info.item_info.data_ary[e_item_info_stack_count];
		int32 money_type = item_info.sell_money_type;
		int32 money_count = item_info.sell_total_money;
		int32 item_id = item_info.item_info.data_ary[e_item_info_info_id];
		server_log::auction_buy_log(temp_player.get_third_info(), temp_player.get_unit_info_inst(), item_id, item_num, money_type, money_count);
		server_log::auction_db_log(item_info, e_auction_state_deal, temp_player.get_unit_info_inst(), temp_player.get_third_info(), temp_player.get_login_type());

		set_log_var(log_head)

		temp_player.get_log_common_head_info(log_head);


		server_log::serverAuction(log_head, "null", init_unit::change_i64_to_string(item_info.seller_guid.server_64), item_info.seller_name, 0,
			init_unit::change_i64_to_string(item_info.item_info.item_guid.server_64), item_info.item_info.data_ary[e_item_info_info_id], item_info.item_info.data_ary[e_item_info_stack_count],
			item_info.trade_type, "null", "null", "null", 0, item_info.sell_money_type, globle_data::get_instance().get_money_str_by_type(item_info.sell_money_type), item_info.sell_total_money, e_auction_state_deal);






		
	}


	void cauction_mgr::req_goods_list(int32 main_type, int32 sub_type,
		int32 item_color, int32 quality_level,
		int32 single_money_sort_mode, int32 total_money_sort_mode,
		bool is_show_gold, bool is_show_diamond, int32 show_page, std::string key_word,
		int32 trade_type,int32 career_type)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		if (trade_type == e_trading_type_legion
			&& !temp_player.get_legion_guid().is_valid())
		{
			return;
		}

		m_find_info.clear_data();
		m_find_info.is_waiting_find = true;
		m_find_info.main_type = main_type;
		m_find_info.sub_type = sub_type;
		m_find_info.item_color = item_color;
		m_find_info.quality_level = quality_level;
		m_find_info.single_money_sort_mode = single_money_sort_mode;
		m_find_info.total_money_sort_mode = total_money_sort_mode;
		m_find_info.is_show_gold = is_show_gold;
		m_find_info.is_show_diamond = is_show_diamond;
		m_find_info.show_page = show_page;
		m_find_info.trade_type = trade_type;
		m_find_info.career_type = career_type;
		m_find_info.self_guid = temp_player.get_unit_guid();

		switch (trade_type)
		{
		case e_trading_type_legion:
		{
			m_find_info.seller_guid = temp_player.get_legion_guid();
		}
		break;
		case e_trading_type_normal_sell:
		case e_trading_type_world:
		default:
		{
			m_find_info.seller_guid = 0;
		}
		break;
		}

		invalid_ansi_word::replace_invalid_ansi_str(key_word, true);
		int32 real_key_word_len = key_word.length();
		if (real_key_word_len > 0)
		{
			if (real_key_word_len > auction_find_key_word_max_lenth)
			{
				real_key_word_len = auction_find_key_word_max_lenth;
			}
			memcpy(m_find_info.key_word, key_word.c_str(), real_key_word_len);
		}
	}

	void cauction_mgr::start_find_list()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		cs2dp_auction_req_goods_list msg;
		msg.role_guid = temp_player.get_unit_guid();
		msg.main_type = m_find_info.main_type;
		msg.sub_type = m_find_info.sub_type;
		msg.item_color = m_find_info.item_color;
		msg.quality_level = m_find_info.quality_level;
		msg.single_money_sort_mode = m_find_info.single_money_sort_mode;
		msg.total_money_sort_mode = m_find_info.total_money_sort_mode;
		msg.is_show_gold = m_find_info.is_show_gold;
		msg.is_show_diamond = m_find_info.is_show_diamond;
		msg.show_page = m_find_info.show_page;
		msg.trade_type = m_find_info.trade_type;
		msg.career_type = m_find_info.career_type;
		msg.seller_guid = m_find_info.seller_guid;
		msg.self_guid = m_find_info.self_guid;
		memcpy(msg.key_word, m_find_info.key_word, auction_find_key_word_max_lenth);
		temp_player.send_message_to_dp( &msg, sizeof(cs2dp_auction_req_goods_list));


		m_find_info.is_waiting_find = false;
		m_find_info.next_can_find_time = init_unit::get_end_time(auction_find_list_interval);
	}

	void cauction_mgr::start_req_record_list()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		cs2dp_auction_req_trade_record msg;
		switch (m_req_record_info.trade_type)
		{
		case e_trading_type_world:
		case e_trading_type_normal_sell:
		{
			msg.record_owner_guid = temp_player.get_unit_guid();
		}
		break;
		case e_trading_type_legion:
		{
			msg.record_owner_guid = temp_player.get_legion_guid();
		}
		break;
		default:
			break;
		}
		msg.role_guid = temp_player.get_unit_guid();
		msg.trade_type = m_req_record_info.trade_type;
		msg.page_index = m_req_record_info.page_index;
		temp_player.send_message_to_dp( &msg, sizeof(cs2dp_auction_req_trade_record));

		m_req_record_info.is_waiting_find = false;
		m_req_record_info.next_can_find_time = init_unit::get_end_time(auction_find_list_interval);
	}

	void cauction_mgr::remove_from_selling_list(guid_64 item_guid)
	{
		for (int32 i = 0; i < max_auction_sell_num; ++i)
		{
			if (m_selling_array[i].item_info.item_guid == item_guid)
			{
				m_selling_array[i].item_info.data_ary[e_item_info_stack_count] = -1;
				send_selling_list_one(i);
				m_selling_array[i].clear_data();
			}
		}
	}

	void cauction_mgr::req_trade_record(int32 trade_type, int32 page_index)
	{
		m_req_record_info.clear_data();
		m_req_record_info.is_waiting_find = true;
		m_req_record_info.trade_type = trade_type;
		m_req_record_info.page_index = page_index;
	}

	void cauction_mgr::req_trade_record_end(const s_auction_record_info* auction_info, int32 data_num, int32 cur_page, int32 total_record_num)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		auction_proto_req_trade_record_end msg;
		msg.set_cur_page_index(cur_page);
		msg.set_total_record_num(total_record_num);
		for (int32 i = 0; i < data_num; ++i)
		{
			auction_proto_auction_item_info* temp_info = msg.add_item_list();
			make_package_info(temp_info, auction_info[i].auction_info);
			temp_info->set_is_sell(auction_info[i].is_sell);
		}
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_trade_record_end);
	}

	void cauction_mgr::req_another_sell_info(int32 sell_money_type, int32 item_template_id)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		cs2dp_auction_req_other_sell_info msg;
		msg.role_guid = temp_player.get_unit_guid();
		msg.sell_money_type = sell_money_type;
		msg.item_template_id = item_template_id;
		temp_player.send_message_to_dp( &msg, sizeof(cs2dp_auction_req_other_sell_info));
	}

	void cauction_mgr::req_another_sell_info_end(const s_auction_info* auction_info, int32 data_num, int32 money_type, int32 item_template_id)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		auction_proto_req_another_sell_info_end msg;
		msg.set_item_template_id(item_template_id);
		msg.set_sell_money_type(money_type);
		msg.set_is_need_clear(true);
		msg.set_is_final(false);
		for (int32 i = 0; i < data_num; ++i)
		{
			auction_proto_auction_item_info* temp_info = msg.add_item_list();
			make_package_info(temp_info, auction_info[i]);

			int32 pak_size = msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_another_sell_info_end);

				msg.set_is_need_clear(false);
				msg.clear_item_list();
			}
		}

		msg.set_is_final(true);
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_req_another_sell_info_end);
	}

	void cauction_mgr::req_self_bid_info(int32 trade_type, int32 page_index)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		cs2dp_auction_req_self_bid_list msg;
		msg.role_guid = temp_player.get_unit_guid();
		msg.trade_type = trade_type;
		msg.page_index = page_index;
		msg.cur_time = utility::get_tick_count();
		temp_player.send_message_to_dp( &msg, sizeof(cs2dp_auction_req_self_bid_list));
	}
	void cauction_mgr::put_auction_item_bag(const s_auction_info& auction_info)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		s_item_info temp_item_info = auction_info.item_info;
		const int32 item_template_id = temp_item_info.data_ary[e_item_info_info_id];
	
		temp_item_info.data_ary[e_item_info_container_type] = 0;
		temp_item_info.data_ary[e_item_info_slot] = -1;
		temp_item_info.item_guid = guid_gen::make_guid(temp_player.get_unit_guid());
		if (temp_item_info.data_ary[e_item_info_prototype_id] > 0)
		{
			ItemTemplate* prototype_item_template_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(temp_item_info.data_ary[e_item_info_prototype_id]);
			if (nullptr != prototype_item_template_ptr)
			{
				temp_item_info.data_ary[e_item_info_over_time] = time_helper::get_cur_time_new().second + prototype_item_template_ptr->EffectiveTime;
			}
		}
		//citem* p_new_item = temp_player.get_item_set().create_item_by_info(temp_item_info, e_server_log_add_item_auction, 0, true);

		//if (p_new_item == nullptr)
		//{ 
		//	globle_data::get_instance().send_mail_to_another_player_by_system(
		//		temp_player.get_unit_guid(),
		//		temp_player.get_unit_guid(),
		//		temp_player.get_unit_info(e_role_info_server_id),
		//		e_item_mail_type_auction_item,
		//		0, 0, 0, 0, &auction_info.item_info, 1); 

		//	return;
		//
		//}
		//
		//temp_player.get_item_set().put_in_bag(p_new_item, e_bag_type_none, e_item_mail_type_auction_item);

		//vector<s_item_template_info> promp_item_data;

		//promp_item_data.push_back({ item_template_id,temp_item_info.data_ary[e_item_info_stack_count],1 });

		//temp_player.get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
	}

	void cauction_mgr::send_reward_to_buyer(const s_auction_info& auction_info)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		/*	const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_purchase_title);
			const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_purchase_content);

			if (sender_name == nullptr
				|| mail_title == nullptr
				|| mail_content == nullptr)
			{
				return;
			}


			globle_data::get_instance().send_mail_to_another_player_by_system(
				temp_player.get_unit_guid(),
				temp_player.get_unit_guid(),
				temp_player.get_unit_info(e_role_info_server_id),
				sender_name, mail_title, mail_content,
				0, 0, 0, 0, &auction_info.item_info, 1);*/

		put_auction_item_bag(auction_info);

		cs2ws_auction_del_bid_record msg;
		msg.item_guid = auction_info.item_info.item_guid;
		connection_mgr::getInstance().send_to_ws( &msg, sizeof(cs2ws_auction_del_bid_record));
	}
	void cauction_mgr::send_reward_to_seller(const s_auction_info& auction_info, const int32& after_tax_reward)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		switch (auction_info.trade_type)
		{
		case e_trading_type_world:
		case e_trading_type_normal_sell:
		{
			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, auction_info.item_info.data_ary[e_item_info_info_id]);
			if (item_template_ptr == nullptr)
			{
				return;
			}

			const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
			const xchar* seller_mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_seller_title);
			const xchar* seller_mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_seller_content);
			if (sender_name == nullptr
				|| seller_mail_title == nullptr
				|| seller_mail_content == nullptr)
			{
				return;
			}

			std::vector<std::string> sell_mail_content_array;
			sell_mail_content_array.push_back(seller_mail_content);
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
			sell_mail_content_array.push_back(temp_string);

			globle_data::get_instance().send_mail_to_another_player_by_system(
				auction_info.seller_guid, 
				auction_info.seller_guid,
				0, 
				sender_name,
				seller_mail_title, init_unit::implode(sell_mail_content_array),
				auction_info.sell_money_type, after_tax_reward, 0, 0, nullptr, 0);
		}
			break;
		case e_trading_type_legion:
		{
			cs2ws_auction_share_sell_reward msg;
			msg.data_info = auction_info;
			connection_mgr::getInstance().send_to_ws( &msg, sizeof(cs2ws_auction_share_sell_reward));
		}
		break;
		default:
			break;
		}
	}
	void cauction_mgr::add_new_record_to_db(const s_auction_info& auction_info, const int32& after_tax_reward)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		switch (auction_info.trade_type)
		{
		case e_trading_type_normal_sell:
		case e_trading_type_world:
		{
			//购买记录
			cs2ws_auction_add_purchase_record auction_record_msg;
			auction_record_msg.auction_record_info.record_guid = guid_gen::make_guid(temp_player.get_unit_guid());
			auction_record_msg.auction_record_info.role_guid = temp_player.get_unit_guid();
			auction_record_msg.auction_record_info.is_sell = false;
			auction_record_msg.auction_record_info.auction_info = auction_info;
			connection_mgr::getInstance().send_to_ws( &auction_record_msg, sizeof(cs2ws_auction_add_purchase_record));

			if (auction_info.seller_guid.is_valid())
			{
				//出售记录
				auction_record_msg.auction_record_info.record_guid = guid_gen::make_guid(temp_player.get_unit_guid());
				auction_record_msg.auction_record_info.role_guid = auction_info.seller_guid;
				auction_record_msg.auction_record_info.is_sell = true;
				auction_record_msg.auction_record_info.auction_info.sell_total_money = after_tax_reward;
				int32 item_num = auction_info.item_info.data_ary[e_item_info_stack_count];
				if (item_num == 0)
				{
					item_num = 1;
				}
				auction_record_msg.auction_record_info.auction_info.sell_single_money = after_tax_reward / item_num;
				connection_mgr::getInstance().send_to_ws( &auction_record_msg, sizeof(cs2ws_auction_add_purchase_record));
			}
		}
		break;
		case e_trading_type_legion:
		{
			cs2ws_auction_add_purchase_record auction_record_msg;
			auction_record_msg.auction_record_info.record_guid = guid_gen::make_guid(temp_player.get_unit_guid());
			auction_record_msg.auction_record_info.role_guid = auction_info.seller_guid;		//军团拍卖 guid是军团guid
			auction_record_msg.auction_record_info.is_sell = false;
			auction_record_msg.auction_record_info.auction_info = auction_info;
			connection_mgr::getInstance().send_to_ws( &auction_record_msg, sizeof(cs2ws_auction_add_purchase_record));
		}
		break;
		default:
			break;
		}
	}

	void cauction_mgr::bid_item(guid_64 item_guid, int32 price_type, int32 price_value)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		//此处的 price_value 并不是将要乘以单次加价数额的基准值

		temp_player.get_star_trip_mgr().target_mission(e_star_trip_target_type_bid);

		cs2ws_auction_bid_info bid_msg;
		bid_msg.role_guid = temp_player.get_unit_guid();
		bid_msg.item_guid = item_guid;
		bid_msg.price_money_type = price_type;
		bid_msg.price_money_value = price_value;
		connection_mgr::getInstance().send_to_ws( &bid_msg, sizeof(cs2ws_auction_bid_info));
	}

	void cauction_mgr::find_bid_target_end(s_auction_info auction_info)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid()
			||!auction_info.item_info.item_guid.is_valid())
		{
			return;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		int32 trading_base_money_num_index = region_template_ptr->RegionCode * 2 + 1;

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, auction_info.item_info.data_ary[e_item_info_info_id]);
		if (item_template_ptr == nullptr
			|| item_template_ptr->TradingBaseMoneyArray.size() <= trading_base_money_num_index)
		{
			return;
		}

		if (auction_info.deal_time > 0
			&& auction_info.deal_time < utility::get_tick_count())
		{
			auction_proto_operate_end purchase_end_msg;
			purchase_end_msg.set_item_guid(auction_info.item_info.item_guid.server_64);
			purchase_end_msg.set_item_num(0);
			purchase_end_msg.set_operate_type(e_auction_operate_type_bid);
			purchase_end_msg.set_money_type(0);
			purchase_end_msg.set_money_num(0);
			purchase_end_msg.set_operate_result(e_auction_purchase_already_purchased);
			temp_player.send_message_to_self(&purchase_end_msg, e_msgindex_s2c_auction_operate_end);

			cs2ws_auction_clear_selling_lock clear_msg;
			clear_msg.item_guid = auction_info.item_info.item_guid;
			connection_mgr::getInstance().send_to_ws( &clear_msg, sizeof(cs2ws_auction_clear_selling_lock));
			return;
		}
		guid_64 old_buyer_guid = auction_info.buyer_guid;

		int32 seller_guid = 0;
		int32 item_num = 0;
		int32 money_type = 0;
		int32 item_price = 0;


		int32 cost_money_value;
		if (old_buyer_guid.is_valid())
		{
			int32 cost_money_add_value = item_template_ptr->TradingBaseMoneyArray[trading_base_money_num_index] * GAMECONFIG->AuctionBidPercentage;
			if (cost_money_add_value <= 0)
			{
				cost_money_add_value = 1;
			}
			cost_money_value = auction_info.cur_bid_money + cost_money_add_value;
		}
		else
		{//无人竞拍（第一次拍）使用不加价的价格
			cost_money_value = auction_info.cur_bid_money;
		}

		if (cost_money_value >= auction_info.sell_total_money && auction_info.sell_total_money != 0)
		{//如果是这样 应该直接走 购买逻辑
			//以下为拍卖行埋点日志
			item_num = auction_info.item_info.data_ary[e_item_info_stack_count];
			money_type = auction_info.sell_money_type;
			item_price = auction_info.sell_total_money;
			if (auction_info.trade_type == e_trading_type_legion)
			{
				server_log::legion_auction_log(auction_info.item_info.item_guid.server_64, item_num, money_type, item_price);
				
			}
			if (auction_info.trade_type == e_trading_type_world)
			{
				server_log::world_auction_log(auction_info.item_info.item_guid.server_64, item_num, money_type, item_price);
				
			}
			return;
		}

		if (!temp_player.can_cut_money(e_money_type(auction_info.sell_money_type), cost_money_value))
		{
			cs2ws_auction_clear_selling_lock clear_msg;
			clear_msg.item_guid = auction_info.item_info.item_guid;
			connection_mgr::getInstance().send_to_ws( &clear_msg, sizeof(cs2ws_auction_clear_selling_lock));
			return;
		}
		int32 delta = (utility::get_tick_count() - auction_info.sell_time) / second_tick_time;

		if (delta < item_template_ptr->TradingAwaitTime)
		{
			cs2ws_auction_clear_selling_lock clear_msg;
			clear_msg.item_guid = auction_info.item_info.item_guid;
			connection_mgr::getInstance().send_to_ws(&clear_msg, sizeof(cs2ws_auction_clear_selling_lock));
			return;
		}

		return_money_to_old_buyer(old_buyer_guid, auction_info.item_info.data_ary[e_item_info_info_id], auction_info.sell_money_type, auction_info.cur_bid_money);

		temp_player.cut_money(e_money_type(auction_info.sell_money_type), cost_money_value, e_server_log_cut_money_auction_bid);
		auction_info.cur_bid_money = cost_money_value;
		auction_info.buyer_guid = temp_player.get_unit_guid();
		memcpy(auction_info.buyer_name, temp_player.get_name(), max_name_size);

		cs2ws_auction_bid_success success_msg;
		success_msg.auction_info = auction_info;
		success_msg.old_buyer_guid = old_buyer_guid;
		connection_mgr::getInstance().send_to_ws( &success_msg, sizeof(cs2ws_auction_bid_success));

		//auction_proto_operate_end purchase_end_msg;
		//purchase_end_msg.set_item_guid(auction_info.item_info.item_guid.server_64);
		//purchase_end_msg.set_item_num(0);
		//purchase_end_msg.set_operate_type(e_auction_operate_type_bid);
		//purchase_end_msg.set_money_type(0);
		//purchase_end_msg.set_money_num(0);
		//purchase_end_msg.set_operate_result(e_auction_bid_success);
		//temp_player.send_message_to_self(&purchase_end_msg, e_msgindex_s2c_auction_operate_end);
		
		temp_player.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_auction_bid);

		//以下为拍卖行埋点日志
		item_num = auction_info.item_info.data_ary[e_item_info_stack_count];
		money_type = auction_info.sell_money_type;
		item_price = auction_info.sell_total_money;
		if (auction_info.trade_type == e_trading_type_legion)
		{
			server_log::legion_auction_log(auction_info.item_info.item_guid.server_64, item_num, money_type, item_price);
			
		}
		if (auction_info.trade_type == e_trading_type_world)
		{
			server_log::world_auction_log(auction_info.item_info.item_guid.server_64, item_num, money_type, item_price);
			
		}
		server_log::auction_db_log(auction_info, e_auction_state_bid, temp_player.get_unit_info_inst(), temp_player.get_third_info(), temp_player.get_login_type());

		set_log_var(log_head);

		temp_player.get_log_common_head_info(log_head);

		server_log::serverAuction(log_head, "null", "null", "null", 0,
			init_unit::change_i64_to_string(auction_info.item_info.item_guid.server_64), auction_info.item_info.data_ary[e_item_info_info_id], auction_info.item_info.data_ary[e_item_info_stack_count],
			auction_info.trade_type, temp_player.get_account(), init_unit::change_i64_to_string(temp_player.get_unit_guid().server_64), temp_player.get_name(), temp_player.get_unit_info(e_role_info_exp_level),
			auction_info.sell_money_type, globle_data::get_instance().get_money_str_by_type(auction_info.sell_money_type), auction_info.cur_bid_money, e_auction_state_bid);


	}

	void cauction_mgr::return_money_to_old_buyer(const guid_64& old_buyer_guid, const int32& item_id, const int32& money_type, const int32& money_value)
	{
		if (!old_buyer_guid.is_valid()
			|| money_value <= 0)
		{
			return;
		}
		const xchar* sender_name = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_sender_system);
		const xchar* mail_title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_bid_failed_title);
		const xchar* mail_content = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_auction_bid_failed_content);

		if (sender_name == nullptr
			|| mail_title == nullptr
			|| mail_content == nullptr)
		{
			return;
		}

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (item_template_ptr == nullptr)
		{
			return;
		}

		std::vector<std::string> sell_mail_content_array;
		sell_mail_content_array.push_back(mail_content);
		const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(item_template_ptr->ItemName);
		sell_mail_content_array.push_back(temp_string);

		globle_data::get_instance().send_mail_to_another_player_by_system(
			old_buyer_guid, old_buyer_guid, 0, sender_name, mail_title, init_unit::implode(sell_mail_content_array),
			money_type, money_value, 0, 0, nullptr, 0);
	}

	void cauction_mgr::set_is_show(s_auction_info& auction_info)
	{
		auction_info.is_show = 0;
	}
	//void cauction_mgr::req_bid_record_failure(int32 in_oper_type)
	//{
	//	player& temp_player = unit_man::get_player(m_unit_array_index);
	//	if (!temp_player.is_valid())
	//	{
	//		return;
	//	}
	//	cs2dp_auction_req_bid_record_failure msg;
	//	msg.role_guid = temp_player.get_unit_guid();
	//	msg.oper_type = in_oper_type;
	//	temp_player.send_message_to_dp(&msg, sizeof(cs2dp_auction_req_bid_record_failure));
	//}

	//void cauction_mgr::req_bid_record_failure_end(const s_auction_bid_replace_info *info_array, int32 num)
	//{
	//	 for (int32 i = 0; i < num; i++)
	//	 {
	//		 m_bid_record_vec.push_back(info_array[i]);		
	//	 }
	//	 is_load = 2;
	//	 sync_client_bid_record_failure_data();
	//}

	void cauction_mgr::read_record(guid_64 record_guid, int32 type)
	{
		if (type != e_auction_req_type_read && type != e_auction_req_type_red)
		{
			return;
		}
		if (type == e_auction_req_type_red)
		{
			if (m_next_interval > time_helper::get_time())
			{
				return;
			}
			m_next_interval = time_helper::get_time() + auction_bid_red_min_interval;
		}
	
		req_bid_notice(type);
		 //for (int32 i = 0 ; i < m_bid_record_vec.size(); i++)
		 //{
			// m_bid_record_vec[i].is_read = 1;
		 //}
	}
	/*void cauction_mgr::sync_bid_record_failure(const s_auction_bid_replace_info &new_info)
	{
		if (m_bid_record_vec.size() >= auction_bid_record_failure_num)
		{
			m_bid_record_vec.pop_back();
		}
		m_bid_record_vec.insert(m_bid_record_vec.begin(), new_info);
		sync_client_bid_record_failure_data();

	}
	void cauction_mgr::sync_client_bid_record_failure_data()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		auction_proto_auction_bid_record_failure msg;
		for (bid_replace_vector_it it= m_bid_record_vec.begin(); it != m_bid_record_vec.end(); it++)
		{
			auction_proto_auction_item_single *one_msg = msg.add_auction_record_array();
			one_msg->set_trade_type(it->info.trade_type);
			one_msg->set_create_time(it->create_time);
			one_msg->set_is_read(it->is_read);
			one_msg->set_sell_time(it->info.sell_time / 1000);
			one_msg->set_deal_time(it->info.deal_time / 1000);
			one_msg->set_sell_money_type(it->info.sell_money_type);
			one_msg->set_cur_bid_money_str(init_unit::change_i64_to_string(it->info.cur_bid_money));
			one_msg->add_cur_bid_money(guid_64(it->info.cur_bid_money).A);
			one_msg->add_cur_bid_money(guid_64(it->info.cur_bid_money).B);
			for (int32 i = 0; i < e_item_info_max; ++i)
			{
				one_msg->add_item_info(it->info.item_info.data_ary[i]);
			}
		}
		temp_player.send_message_to_self(&msg, e_mgsindex_s2c_update_bid_record_failure);

	}
*/
	int32 cauction_mgr::get_config_param(e_auction_param param_type)
	{
		int32 config_init[e_auction_param_max] = { 86400, 60, 600, 600, 540, 180, 6,100,39600,4};

		if (GAMECONFIG->AuctionArray.size() < e_auction_param_max)
		{
			return config_init[param_type];
		}
		return GAMECONFIG->AuctionArray[param_type];
	}

	void cauction_mgr::time_out_del(const s_auction_info & auction_info)
	{
		remove_from_selling_list(auction_info.item_info.item_guid);
	}

	void cauction_mgr::req_bid_notice(int32 in_oper_type)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		cs2dp_auction_req_bid_notice msg;
		msg.role_guid = temp_player.get_unit_guid();
		msg.oper_type = in_oper_type;
		temp_player.send_message_to_dp(&msg, sizeof(cs2dp_auction_req_bid_notice));

	}

	void cauction_mgr::req_bid_notice_end(const s_auction_bid_notice_info * data_array, int32 num)
	{
		if (data_array == nullptr)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		auction_proto_auction_bid_notice msg;

		msg.add_role_guid(temp_player.get_unit_guid().A);
		msg.add_role_guid(temp_player.get_unit_guid().B);

		for (int32 i = 0; i < num; i ++)
		{
			const s_auction_bid_notice_info & signle_data = data_array[i];

			auction_proto_auction_bid_single *one_msg = msg.add_bid_notice_array();

			one_msg->add_item_guid(signle_data.item_guid.A);
			one_msg->add_item_guid(signle_data.item_guid.B);
			one_msg->set_create_time(signle_data.create_time);
			one_msg->set_template_id(signle_data.template_id);
			one_msg->set_template_num(signle_data.template_num);
			one_msg->set_money_type(signle_data.money_type);
			one_msg->set_bid_money(init_unit::change_i64_to_string(signle_data.bid_money));
			one_msg->set_is_sell(signle_data.is_sell);
			one_msg->add_buyer_guid(signle_data.buy_guid.A);
			one_msg->add_buyer_guid(signle_data.buy_guid.B);
			one_msg->set_update_time(signle_data.update_time);
			one_msg->set_is_read(signle_data.is_read);

		}
		temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_bid_notice);
	}
	void cauction_mgr::req_bid_notice_red_end(int32 result, int32 is_red)
	{
		if (result == 1)
		{
			return;
		}
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}

		auction_proto_auction_bid_notice_red msg;

		msg.add_role_guid(temp_player.get_unit_guid().A);
		msg.add_role_guid(temp_player.get_unit_guid().B);
		msg.add_is_red(is_red);
	    temp_player.send_message_to_self(&msg, e_msgindex_s2c_update_bid_notice_red);
	}

	void cauction_mgr::person_tick(int64 new_time) // 在线
	{
		if (m_refresh_time == 0)
		{
			calc_refresh_time(new_time);
		}
		///////////////////////////////////////////////////
		if (new_time < m_refresh_time)
		{
			return;
		}
		m_refresh_time += day_time_second * second_tick_time;
		////////////////////////////////////////////////////
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		if (temp_player.get_is_robot())
		{
			return;
		}
		if (temp_player.get_func_unlock_mgr().is_func_unlock("Auction") == false)
		{
			return;
		}
		cs2ws_auction_request_refresh_person msg;
		msg.role_guid = temp_player.get_unit_guid();
		msg.role_lv = temp_player.get_unit_info(e_role_info_exp_level);
		msg.role_class_type = temp_player.get_unit_info(e_role_info_class_type);
		int32 server_id = temp_player.get_unit_info(e_role_info_server_id);
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_auction_request_refresh_person), server_id);
	}
	void cauction_mgr::calc_refresh_time(int64 new_time)
	{
		m_refresh_time = time_helper::get_time_by_today_stamp_new(get_config_param(e_auction_param_person_refresh_time)).millisecond;
		if (new_time >= m_refresh_time)
		{
		   m_refresh_time += day_time_second * second_tick_time;
		}
	}

	void cauction_mgr::person_sec_tick()
	{

		time_info cur_time_info = time_helper::get_cur_time_new();
		if (m_refresh_info.second == 0) // 初始化当前时间
		{
			m_refresh_info = time_helper::get_time_by_today_stamp_new(get_config_param(e_auction_param_person_refresh_time));
		}


		if (cur_time_info.year != m_refresh_info.year ||
			cur_time_info.month_in_year != m_refresh_info.month_in_year ||
			cur_time_info.day_in_month != m_refresh_info.day_in_month)
		{
			m_refresh_info = time_helper::get_time_by_today_stamp_new(get_config_param(e_auction_param_person_refresh_time));
		}

		if (cur_time_info.second < m_refresh_info.second)
		{
			refresh_yesterday();
		}
		else
		{
			refresh_today();
		}
	}

	bool cauction_mgr::is_same_day(int64 dbtime)
	{
	   time_info db_time_info =	time_helper::get_time_by_stamp_new(dbtime);
	   if (db_time_info.year != m_refresh_info.year ||
		   db_time_info.month_in_year != m_refresh_info.month_in_year ||
		   db_time_info.day_in_month != m_refresh_info.day_in_month)
	   {
		   return false;
	   }
	   return true;
	   

	}

	void cauction_mgr::refresh_yesterday()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		int64 last_time = temp_player.get_time_data((e_time_type)auction_person_last_refresh_enum);

		if (is_same_day(last_time + day_time_second))
		{
			return;
		}
	
		int64 yes_sec_time = m_refresh_info.second - day_time_second;
		temp_player.set_time_data((e_time_type)auction_person_last_refresh_enum, yes_sec_time);

		bool is_refesh = request_refresh_to_ws(yes_sec_time * second_tick_time);
		if (is_refesh)
		{		
			temp_player.set_time_data((e_time_type)auction_person_last_refresh_enum, yes_sec_time);
		}



	}
	void cauction_mgr::refresh_today()
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return;
		}
		int64 last_time = temp_player.get_time_data((e_time_type)auction_person_last_refresh_enum);
		if (is_same_day(last_time))
		{
			return;
		}
		//temp_player.set_time_data((e_time_type)auction_person_last_refresh_enum, m_refresh_info.second);
		bool is_refresh = request_refresh_to_ws(m_refresh_info.second * second_tick_time);
		if (is_refresh)
		{
			temp_player.set_time_data((e_time_type)auction_person_last_refresh_enum, m_refresh_info.second);
		}


	}
	bool cauction_mgr::request_refresh_to_ws(int64 item_time)
	{
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}
		if (temp_player.get_is_robot())
		{
			return false;
		}
		if (temp_player.get_func_unlock_mgr().is_func_unlock("Auction") == false)
		{
			return false;
		}
		cs2ws_auction_request_refresh_person msg;
		msg.role_guid = temp_player.get_unit_guid();
		msg.role_lv = temp_player.get_unit_info(e_role_info_exp_level);
		msg.role_class_type = temp_player.get_unit_info(e_role_info_class_type);
		msg.item_time = item_time;
		int32 server_id = temp_player.get_unit_info(e_role_info_server_id);
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_auction_request_refresh_person), server_id);

		return true;
	}

	bool cauction_mgr::lua_make_auction_to_ws(citem *temp_item, int32 sell_num, int32 price_type, int32 price_value)
	{
		if (temp_item == nullptr)
		{
			return false;
		}
		player& temp_player = unit_man::get_player(m_unit_array_index);
		if (!temp_player.is_valid())
		{
			return false;
		}
		int32 empty_selling_index = get_empty_sell_slot();
		if (empty_selling_index < 0
			|| empty_selling_index >= max_auction_sell_num)
		{
			return false;
		}
		s_auction_info temp_info;
		if (!globle_data::get_instance().make_auction_info(temp_info, temp_item->get_item_inst(), sell_num, e_trading_type_world,
			temp_player.get_unit_guid(), temp_player.get_name(), temp_player.get_vip_level(),
			price_type, price_value))
		{
			return false;
		}

		//set_is_show(temp_info);

		m_selling_array[empty_selling_index] = temp_info;

		cs2ws_auction_add_item_into_db sell_msg;
		sell_msg.role_info = temp_player.get_unit_info_inst();
		sell_msg.auction_info = temp_info;
		sell_msg.old_item_guid = temp_item->get_item_guid();
		sell_msg.third_info = temp_player.get_third_info();
		connection_mgr::getInstance().send_to_ws(&sell_msg, sizeof(cs2ws_auction_add_item_into_db));

		return true;

	}

}

