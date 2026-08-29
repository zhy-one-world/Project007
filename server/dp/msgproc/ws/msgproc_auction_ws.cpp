/********************************************************************
created:	2017年10月17日13:04:37
file base:	msgproc_auction_ws
file ext:	cpp
author:		luoxingyu

purpose:	
*********************************************************************/
#include "msgproc_auction_ws.hpp"
#include "../../dbproxy_service.hpp"
#include "Internal/core.hpp"
#include "time.hpp"
#include "template/StringConst_S.h"
#include "utility/parse_msg.h"

namespace faith
{
	void auction_add_item_into_db_proc_lua(uint32 connindex, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_add_item_into_db request;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&request, data_ptr, data_len);
		if (false == is_sucess)
		{
			return;
		}			
		
		s_auction_info		info;
		s_unit_info			role_info;
		login_fixed_data	third_info;
		info.proto_set_info(request.info());
		role_info.proto_set_info(request.role_info());
		third_info.proto_set_info(request.third_info());
		if (request.info().item_info().item_guid() <= 0)
		{
			return;
		}

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_add_item(");

		_sql << request.info().item_info().item_guid() << "," << "'(";
		_sql << request.info().seller_guid() << ",";
		_sql << "\\\'" << request.info().seller_name() << "\\\',";
		_sql << request.info().buyer_guid() << ",";
		_sql << "\\\'" << request.info().buyer_name() << "\\\',";
		_sql << request.info().trade_type() << ",";
		_sql << request.info().seller_vip_level() << ",";
		_sql << request.info().auction_type() << ",";
		_sql << request.info().auction_sub_type() << ",";
		_sql << request.info().item_color() << ",";
		_sql << request.info().quality_level() << ",";
		_sql << request.info().class_limit() << ",";
		_sql << "\\\'" << request.info().item_name() << "\\\',";
		_sql << request.info().sell_time() << ",";
		_sql << request.info().deal_time() << ",";
		_sql << request.info().sell_money_type() << ",";
		_sql << request.info().sell_total_money() << ",";
		_sql << request.info().sell_single_money() << ",";
		_sql << request.info().cur_bid_money() << ",";
		_sql << request.info().is_show() << ",";
		_sql << request.info().add_time() << ",";
		_sql << request.info().is_sky_order() << ",";
		_sql << request.info().show_guid() << ",";

		_sql << request.info().item_info().item_guid() << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			_sql << request.info().item_info().data_ary(i) << ",";
		}
		_sql << "\\\'" << request.info().item_info().param_str() << "\\\'";
		_sql << ")');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_add_item_into_db_end_proc, _1, connindex, info, request.old_item_guid(), role_info, request.is_add_to_log(), third_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}
	void auction_add_item_into_db_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		packet_s2s* pPacket = (packet_s2s*)data_ptr;
		auction_add_item_into_db_proc_lua(connindex, (char*)pPacket->google_data, pPacket->google_data_len);
	}

	void auction_add_item_into_db_end_proc(db_result_type result, uint32 connindex, s_auction_info auction_info, guid_64 old_item_guid, s_unit_info role_info, bool is_add_to_log, login_fixed_data third_info)
	{
		dp2ws_auction_sell_end msg;
		msg.info = auction_info;
		msg.role_info = role_info;
		msg.is_add_to_log = is_add_to_log;
		msg.third_info = third_info;
		if (result.error)
		{
			msg.operate_result = e_auction_sell_failed;
		}
		else
		{
			msg.operate_result = e_auction_sell_success;

			{
				//安全防错，防止在极限情况下，服务器挂掉，导致物品上架了，但是玩家上线后还有这个东西
				sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
				_sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_info.role_guid.server_64) << _XTEXT(".sp_role_item_del_one(");

				_sql << old_item_guid.server_64 << ",";
				_sql << role_info.role_guid.server_64 << ");";


				db_query_type query;
				query.sql_str.assign(_sql.c_str(), _sql.size());
				query.role_guid = auction_info.seller_guid;
				query.logic_id = e_data_flag_item;
				db_manager::getInstance().get_db_link().game_db.add_query(query);

			}

		}
		dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_auction_sell_end));
	}

	void auction_find_buy_target_proc_lua(uint32 connindex, int64 role_guid, int64 item_guid)
	{
		if (item_guid <= 0)
		{
			return;
		}

		guid_64 _role_guid(role_guid);
		guid_64 _item_guid(item_guid);
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find(");
		_sql << item_guid << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_find_buy_target_end, _1, connindex, _role_guid, _item_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_find_buy_target_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_find_buy_target request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_find_buy_target_proc_lua(connindex, request.role_guid(), request.item_guid());

	}
	void auction_find_buy_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, guid_64 item_guid)
	{
		//必须先 选 再 删
		//并且等删除执行结束后再回复 购买成功
		//否则会有删除语句延迟过长造成的统一物品反复被买的隐患

		if (result.query.data_select.row_count < 1
			|| result.query.data_select.row_size != sizeof(s_auction_info))
		{
			//相当于购买失败
			dp2ws_auction_find_buy_target_end msg;
			msg.role_guid = role_guid;
			msg.item_guid = item_guid;
			msg.auction_info.clear_data();
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_find_buy_target_end));
			return;
		}
		s_auction_info auction_info;
		db_read_data(&result, &auction_info, sizeof(s_auction_info));

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_del(");
		_sql << item_guid.server_64 << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_del_buy_target_end, _1, connindex,role_guid, item_guid, auction_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_del_buy_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, guid_64 item_guid, s_auction_info item_info)
	{
		dp2ws_auction_find_buy_target_end msg;
		msg.role_guid = role_guid;
		msg.item_guid = item_guid;
		msg.auction_info = item_info;
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_find_buy_target_end));
	}

	void auction_find_cancel_target_proc_lua(uint32 connindex, int64 role_guid, int64 item_guid)
	{
		if (item_guid <= 0)
		{
			return;
		}

		guid_64 _role_guid(role_guid);
		guid_64 _item_guid(item_guid);

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find(");
		_sql << item_guid << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_find_cancel_target_end, _1, connindex, _role_guid, _item_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_find_cancel_target_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_cancel_sell request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_find_cancel_target_proc_lua(connindex, request.role_guid(), request.item_guid());
	}

	void auction_find_cancel_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, guid_64 item_guid)
	{
		s_auction_info auction_info;
		int32	data_row_len = result.query.data_select.row_count;
		if (result.query.data_select.row_count == 1
			&& result.query.data_select.row_size == sizeof(s_auction_info))
		{
			db_read_data(&result, &auction_info, sizeof(s_auction_info));
		}

		if (auction_info.item_info.item_guid.is_valid() && false == auction_info.buyer_guid.is_valid())
		{
			sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
			_sql << _XTEXT("call sp_auction_del(");
			_sql << auction_info.item_info.item_guid.server_64 << ");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			query.handler = boost::bind(&auction_del_cancel_target_end, _1, connindex, role_guid, auction_info);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
		else
		{
			dp2ws_auction_cancel_sell msg;
			msg.role_guid = role_guid;
			msg.auction_info.item_info.item_guid = item_guid;
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_cancel_sell));
		}
	}

	void auction_del_cancel_target_end(db_result_type result, uint32 connindex, guid_64 role_guid, s_auction_info auction_info)
	{
		dp2ws_auction_cancel_sell msg;
		msg.role_guid = role_guid;
		msg.auction_info = auction_info;
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_cancel_sell));
	}

	void auction_find_bid_target_proc_lua(uint32 connindex, int64 role_guid, int64 item_guid, int32 money_type, int32 money_value)
	{
		if (item_guid <= 0)
		{
			return;
		}

		guid_64 _role_guid(role_guid);
		guid_64 _item_guid(item_guid);

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find(");
		_sql << item_guid << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_find_bid_target_end_proc, _1, connindex, _role_guid, money_type, money_value, _item_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_find_bid_target_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_find_bid_target request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_find_bid_target_proc_lua(connindex, request.role_guid(), request.item_guid(), request.price_money_type(), request.price_money_value());
	}

	void auction_find_bid_target_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid, int32 price_money_type, int32 price_money_value, guid_64 item_guid)
	{
		dp2ws_auction_find_bid_target_end msg;
		msg.role_guid = role_guid;
		msg.item_guid = item_guid;
		msg.price_money_type = price_money_type;
		msg.price_money_value = price_money_value;

		if (result.query.data_select.row_count == 1
			&& result.query.data_select.row_size == sizeof(s_auction_info))
		{
			db_read_data(&result, &msg.auction_info, sizeof(s_auction_info));
		}
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_find_bid_target_end));
	}
	s_auction_info	bid_auction_info;
	void auction_bid_success_proc_lua(uint32 connindex, int64 new_record_id, int64 old_buyer_guid, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_s_auction_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}
		if (msg.item_info().item_guid() <= 0)
		{
			return;
		}
		bid_auction_info.clear_data();
		bid_auction_info.proto_set_info(msg);


		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_add_item(");

		_sql << bid_auction_info.item_info.item_guid.server_64 << "," << "'(";
		_sql << bid_auction_info.seller_guid.server_64 << ",";
		_sql << "\\\'" << bid_auction_info.seller_name << "\\\',";
		_sql << bid_auction_info.buyer_guid.server_64 << ",";
		_sql << "\\\'" << bid_auction_info.buyer_name << "\\\',";
		_sql << bid_auction_info.trade_type << ",";
		_sql << bid_auction_info.seller_vip_level << ",";
		_sql << bid_auction_info.auction_type << ",";
		_sql << bid_auction_info.auction_sub_type << ",";
		_sql << bid_auction_info.item_color << ",";
		_sql << bid_auction_info.quality_level << ",";
		_sql << bid_auction_info.class_limit << ",";
		_sql << "\\\'" << bid_auction_info.item_name << "\\\',";
		_sql << bid_auction_info.sell_time << ",";
		_sql << bid_auction_info.deal_time << ",";
		_sql << bid_auction_info.sell_money_type << ",";
		_sql << bid_auction_info.sell_total_money << ",";
		_sql << bid_auction_info.sell_single_money << ",";
		_sql << bid_auction_info.cur_bid_money << ",";
		_sql << bid_auction_info.is_show << ",";
		_sql << bid_auction_info.add_time << ",";
		_sql << bid_auction_info.is_sky_order << ",";
		_sql << bid_auction_info.show_guid.server_64 << ",";

		_sql << bid_auction_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			_sql << bid_auction_info.item_info.data_ary[i] << ",";
			//if (i != last_index)
			//{
			//	_sql << ",";
			//}
		}
		_sql << "\\\'" << bid_auction_info.item_info.param_str << "\\\'";
		_sql << ")');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_bid_success_end_proc, _1, connindex, bid_auction_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	
	void auction_bid_success_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_bid_success request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		server2dp_proto_s_auction_info msg = request.auction_info();
		if (msg.item_info().item_guid() <= 0)
		{
			return;
		}
		bid_auction_info.clear_data();
		bid_auction_info.proto_set_info(msg);


		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_add_item(");

		_sql << bid_auction_info.item_info.item_guid.server_64 << "," << "'(";
		_sql << bid_auction_info.seller_guid.server_64 << ",";
		_sql << "\\\'" << bid_auction_info.seller_name << "\\\',";
		_sql << bid_auction_info.buyer_guid.server_64 << ",";
		_sql << "\\\'" << bid_auction_info.buyer_name << "\\\',";
		_sql << bid_auction_info.trade_type << ",";
		_sql << bid_auction_info.seller_vip_level << ",";
		_sql << bid_auction_info.auction_type << ",";
		_sql << bid_auction_info.auction_sub_type << ",";
		_sql << bid_auction_info.item_color << ",";
		_sql << bid_auction_info.quality_level << ",";
		_sql << bid_auction_info.class_limit << ",";
		_sql << "\\\'" << bid_auction_info.item_name << "\\\',";
		_sql << bid_auction_info.sell_time << ",";
		_sql << bid_auction_info.deal_time << ",";
		_sql << bid_auction_info.sell_money_type << ",";
		_sql << bid_auction_info.sell_total_money << ",";
		_sql << bid_auction_info.sell_single_money << ",";
		_sql << bid_auction_info.cur_bid_money << ",";
		_sql << bid_auction_info.is_show << ",";
		_sql << bid_auction_info.add_time << ",";
		_sql << bid_auction_info.is_sky_order << ",";
		_sql << bid_auction_info.show_guid.server_64 << ",";

		_sql << bid_auction_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			_sql << bid_auction_info.item_info.data_ary[i] << ",";
			//if (i != last_index)
			//{
			//	_sql << ",";
			//}
		}
		_sql << "\\\'" << bid_auction_info.item_info.param_str << "\\\'";
		_sql << ")');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_bid_success_end_proc, _1, connindex, bid_auction_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_bid_success_end_proc(db_result_type result, uint32 connindex, s_auction_info auction_item_info)
	{
		if (auction_item_info.buyer_guid.is_valid())
		{
			sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
			_sql << _XTEXT("call sp_auction_bid_record_add(");

			_sql << auction_item_info.buyer_guid.server_64 << ",";
			_sql << auction_item_info.item_info.item_guid.server_64 << ",";
			_sql << auction_item_info.deal_time << ");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			db_manager::getInstance().get_db_link().game_db.add_query(query);

			//auction_bid_success_add_replace_info(msg_info);
			//auction_bid_success_add_notice_info(msg_info);
		}

		dp2ws_auction_bid_db_sucess   wsmsg;
		wsmsg.buy_guid = auction_item_info.buyer_guid;
		wsmsg.item_guid = auction_item_info.item_info.item_guid;
		dbproxy_service::getInstance().send_message(connindex, &wsmsg, sizeof(dp2ws_auction_bid_db_sucess));

		//std::cout << "-----auction---request--insert----" << utility::get_tick_count() << std::endl;

		dp2ws_auction_clear_selling_lock msg;
		msg.item_guid = auction_item_info.item_info.item_guid;
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_clear_selling_lock));

		
	}
	void auction_bid_success_add_replace_info(const ws2dp_auction_bid_success &info)
	{
		if (!info.old_buyer_guid.is_valid())
		{
			return;
		}

		int32 max_record_num = auction_bid_record_failure_num;
		int32 cur_time = utility::get_tick_count() / 1000;

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_bid_replace_add(");

		_sql << info.old_buyer_guid.server_64 << ",";
		_sql << max_record_num << "," << "'(";
		_sql << info.new_record_id.server_64 << ",";
		_sql << info.old_buyer_guid.server_64 << ",";
		_sql << cur_time << ",";
		_sql << 0 << ",";
		_sql << info.auction_info.seller_guid.server_64 << ",";
		_sql << "\\\'" << info.auction_info.seller_name << "\\\',";
		_sql << info.auction_info.buyer_guid.server_64 << ",";
		_sql << "\\\'" << info.auction_info.buyer_name << "\\\',";
		_sql << info.auction_info.trade_type << ",";
		_sql << info.auction_info.seller_vip_level << ",";
		_sql << info.auction_info.auction_type << ",";
		_sql << info.auction_info.auction_sub_type << ",";
		_sql << info.auction_info.item_color << ",";
		_sql << info.auction_info.quality_level << ",";
		_sql << info.auction_info.class_limit << ",";
		_sql << "\\\'" << info.auction_info.item_name << "\\\',";
		_sql << info.auction_info.sell_time << ",";
		_sql << info.auction_info.deal_time << ",";
		_sql << info.auction_info.sell_money_type << ",";
		_sql << info.auction_info.sell_total_money << ",";
		_sql << info.auction_info.sell_single_money << ",";
		_sql << info.auction_info.cur_bid_money << ",";
		_sql << info.auction_info.is_show << ",";
		_sql << info.auction_info.add_time << ",";
		_sql << info.auction_info.is_sky_order << ",";
		_sql << info.auction_info.show_guid.server_64 << ",";

		_sql << info.auction_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			_sql << info.auction_info.item_info.data_ary[i] << ",";
		}
		_sql << "\\\'" << info.auction_info.item_info.param_str << "\\\'";
		_sql << ")');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_bid_success_add_notice_info(const ws2dp_auction_bid_success &info)
	{
		int32 max_notice_num = auction_bid_notice_max_num;
		int32 cur_time = utility::get_tick_count() / 1000;
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_bid_notice_add(");

		_sql << info.auction_info.buyer_guid.server_64 << ",";
		_sql << info.auction_info.item_info.item_guid.server_64 << ",";
		_sql << cur_time << ",";
		_sql << info.auction_info.item_info.data_ary[e_item_info_info_id] << ",";
		_sql << info.auction_info.item_info.data_ary[e_item_info_stack_count] << ",";
		_sql << info.auction_info.sell_money_type << ",";
		_sql << info.auction_info.cur_bid_money << ",";
		_sql << max_notice_num << ")";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());

		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void auction_get_and_clear_old_item_proc_lua(uint32 connindex, int32 trade_type, int64 cur_time)
	{
		std::string param_string;
		std::stringstream temp_stringstream;

		temp_stringstream << "(";
		temp_stringstream << trade_type << ",";
		temp_stringstream << cur_time << ",";
		temp_stringstream << auction_clear_item_num_per_time << ");";
		temp_stringstream >> param_string;
		auction_get_old_item_proc(param_string, connindex);
	}

	void auction_get_and_clear_old_item_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_clear_old_item request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_get_and_clear_old_item_proc_lua(connindex, request.trade_type(), request.cur_time());
	}

	void auction_del_no_buyer_old_item_proc(std::string param_string, uint32 connindex)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_del_no_buyer_old_item");
		_sql << param_string;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_get_old_item_proc, param_string, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_get_old_item_proc(const std::string& param_string, uint32 connindex)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_get_old_item");
		_sql << param_string;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_get_old_item_end_proc, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	dp2ws_auction_clear_old_item_end g_msg;
	void auction_get_old_item_end_proc(db_result_type result, uint32 connindex)
	{
		//dp2ws_auction_clear_old_item_end msg;
		g_msg.clear_data();
		if (result.query.data_select.row_size == sizeof(s_auction_info))
		{
			int32 temp_data_num = result.query.data_select.row_count;
			if (temp_data_num > auction_clear_item_num_per_time)
			{
				temp_data_num = auction_clear_item_num_per_time;
			}
			db_read_data(&result, &g_msg.data_info, sizeof(s_auction_info) * temp_data_num);
			g_msg.data_num = temp_data_num;


			sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
			_sql << _XTEXT("call sp_auction_del_batch(\"");
			int32 last_index = temp_data_num - 1;
			for (int32 i = 0; i < temp_data_num; ++i)
			{
				_sql << g_msg.data_info[i].item_info.item_guid.server_64;

				if (i != last_index)
				{
					_sql << ",";
				}
			}
			_sql << "\");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			query.handler = boost::bind(&auction_send_old_item_proc, _1, g_msg, connindex);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void auction_send_old_item_proc(db_result_type result, const dp2ws_auction_clear_old_item_end& msg, uint32 connindex)
	{
		dbproxy_service::getInstance().send_message(connindex,  &msg, msg.get_pak_length());
	}

	void auction_add_share_list_proc_lua(uint32 connindex, int64 item_guid[auction_share_list_max_num], int64 role_guid[auction_share_list_max_num], float share_rate[auction_share_list_max_num], xstring share_mail_title[auction_share_list_max_num], xstring share_mail_content[auction_share_list_max_num])
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_add_share_list(") << item_guid[0] << ",'";
		int32 tempLen = 0;
		for (int32 i = 0; i < auction_share_list_max_num; ++i)
		{
			if (item_guid[i] > 0)
			{
				tempLen++;
			}
		}
		for (int32 i = 0; i < auction_share_list_max_num; ++i)
		{
			if (role_guid[i] > 0)
			{
				std::cout << "\n Insert To Sql, ItemGuid = " << item_guid[i] << " ,RoleGuid = " << role_guid[i] << "\n";

				_sql << "(" << item_guid[i] << ",";
				_sql << role_guid[i] << ",";
				_sql << share_rate[i] << ",";
				_sql << "\\\'" << share_mail_title[i] << "\\\',";
				_sql << "\\\'" << share_mail_content[i] << "\\\'";
				tempLen--;
				if (tempLen > 0)
				{
					_sql << "),";
				}
			}
			if (tempLen <= 0)
			{
				_sql << ")";
				break;
			}
		}
		_sql << "');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_add_share_list_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_add_share_list request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int64 item_guid[auction_share_list_max_num];
		int64 role_guid[auction_share_list_max_num];
		float share_rate[auction_share_list_max_num];
		xstring share_mail_title[auction_share_list_max_num];
		xstring share_mail_content[auction_share_list_max_num];
		for (int32 i = 0; i < auction_share_list_max_num; ++i)
		{
			item_guid[i] = request.item_guid(i);
			role_guid[i] = request.role_guid(i);
			share_rate[i] = request.share_rate(i);
			share_mail_title[i] = request.share_mail_title(i);
			share_mail_content[i] = request.share_mail_content(i);
		}
		auction_add_share_list_proc_lua(connindex, item_guid, role_guid, share_rate, share_mail_title, share_mail_content);
	}

	void auction_add_trade_record_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_add_purchase_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		server2dp_proto_s_auction_info msg = request.auction_info();

		s_auction_info	auction_info;
		auction_info.proto_set_info(msg);
		if (false == auction_info.buyer_guid.is_valid())
		{
			return;
		}

		int32 max_record_num = 0;
		switch (auction_info.trade_type)
		{
		case e_trading_type_normal_sell:
		{
			max_record_num = auction_trade_record_num_personal;
		}
		break;
		case e_trading_type_legion:
		{
			max_record_num = auction_trade_record_num_legion;
		}
		break;
		case e_trading_type_world:
		{
			max_record_num = auction_trade_record_num_world;
		}
		break;
		default:
			break;
		}

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_add_record(");

		_sql << request.role_guid() << ",";
		_sql << auction_info.trade_type << ",";
		_sql << max_record_num << "," << "'(";
		_sql << request.record_guid() << ",";
		_sql << request.role_guid() << ",";
		_sql << request.is_sell() << ",";
		_sql << auction_info.seller_guid.server_64 << ",";
		_sql << "\\\'" << auction_info.seller_name << "\\\',";
		_sql << auction_info.buyer_guid.server_64 << ",";
		_sql << "\\\'" << auction_info.buyer_name << "\\\',";
		_sql << auction_info.trade_type << ",";
		_sql << auction_info.seller_vip_level << ",";
		_sql << auction_info.auction_type << ",";
		_sql << auction_info.auction_sub_type << ",";
		_sql << auction_info.item_color << ",";
		_sql << auction_info.quality_level << ",";
		_sql << auction_info.class_limit << ",";
		_sql << "\\\'" << auction_info.item_name << "\\\',";
		_sql << auction_info.sell_time << ",";
		_sql << auction_info.deal_time << ",";
		_sql << auction_info.sell_money_type << ",";
		_sql << auction_info.sell_total_money << ",";
		_sql << auction_info.sell_single_money << ",";
		_sql << auction_info.cur_bid_money << ",";
		_sql << auction_info.is_show << ",";
		_sql << auction_info.add_time << ",";
		_sql << auction_info.is_sky_order << ",";
		_sql << auction_info.show_guid.server_64 << ",";
		_sql << auction_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			_sql << auction_info.item_info.data_ary[i] << ",";
		}
		_sql << "\\\'" << auction_info.item_info.param_str << "\\\'";
		_sql << ")');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_add_trade_record_proc_lua(uint32 connindex, int64 record_guid, int64 role_guid, bool is_sell, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_s_auction_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}
		s_auction_info	auction_info;
		auction_info.proto_set_info(msg);
		if (false == auction_info.buyer_guid.is_valid())
		{
			return;
		}
		
		int32 max_record_num = 0;
		switch (auction_info.trade_type)
		{
		case e_trading_type_normal_sell:
		{
			max_record_num = auction_trade_record_num_personal;
		}
		break;
		case e_trading_type_legion:
		{
			max_record_num = auction_trade_record_num_legion;
		}
		break;
		case e_trading_type_world:
		{
			max_record_num = auction_trade_record_num_world;
		}
		break;
		default:
			break;
		}

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_add_record(");

		_sql << role_guid << ",";
		_sql << auction_info.trade_type << ",";
		_sql << max_record_num << "," << "'(";
		_sql << record_guid << ",";
		_sql << role_guid << ",";
		_sql << is_sell << ",";
		_sql << auction_info.seller_guid.server_64 << ",";
		_sql << "\\\'" << auction_info.seller_name << "\\\',";
		_sql << auction_info.buyer_guid.server_64 << ",";
		_sql << "\\\'" << auction_info.buyer_name << "\\\',";
		_sql << auction_info.trade_type << ",";
		_sql << auction_info.seller_vip_level << ",";
		_sql << auction_info.auction_type << ",";
		_sql << auction_info.auction_sub_type << ",";
		_sql << auction_info.item_color << ",";
		_sql << auction_info.quality_level << ",";
		_sql << auction_info.class_limit << ",";
		_sql << "\\\'" << auction_info.item_name << "\\\',";
		_sql << auction_info.sell_time << ",";
		_sql << auction_info.deal_time << ",";
		_sql << auction_info.sell_money_type << ",";
		_sql << auction_info.sell_total_money << ",";
		_sql << auction_info.sell_single_money << ",";
		_sql << auction_info.cur_bid_money << ",";
		_sql << auction_info.is_show << ",";
		_sql << auction_info.add_time << ",";
		_sql << auction_info.is_sky_order << ",";
		_sql << auction_info.show_guid.server_64 << ",";
		_sql << auction_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			_sql << auction_info.item_info.data_ary[i] << ",";
		}
		_sql << "\\\'" << auction_info.item_info.param_str << "\\\'";
		_sql << ")');";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_add_notice_is_sell(const ws2dp_auction_add_purchase_record & info)
	{
		if (info.auction_record_info.auction_info.trade_type != e_trading_type_legion
			&& info.auction_record_info.auction_info.trade_type != e_trading_type_world)
		{
			return;
		}
		if (!info.auction_record_info.auction_info.buyer_guid.is_valid())
		{
			return;
		}

		int32  cur_time = utility::get_tick_count() / 1000;

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_notice_del(");

		_sql << info.auction_record_info.auction_info.item_info.item_guid.server_64 << ",";
		_sql << info.auction_record_info.auction_info.buyer_guid.server_64 << ",";
		_sql << info.auction_record_info.auction_info.cur_bid_money << ",";
		_sql << cur_time << ")";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_req_share_list_proc_lua(uint32 connindex, int64 item_guid, int32 item_template_id, int64 buyer_guid, int32 money_type, int32 money_value)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find_share_list(");
		_sql << item_guid << ");";

		guid_64 _buyer_guid(buyer_guid);
		guid_64 _item_guid(item_guid);

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_req_share_list_end_proc, _1, connindex, money_type, money_value, _buyer_guid, _item_guid, item_template_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void auction_req_share_list_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_req_share_list request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_req_share_list_proc_lua(connindex, request.item_guid(), request.item_template_id(), request.buyer_guid(), request.money_type(), request.money_value());
	}

	void auction_req_share_list_end_proc(db_result_type result, uint32 connindex, int32 money_type, int32 money_value, guid_64 buyer_guid, guid_64 item_guid, int32 item_template_id)
	{
		if (result.error
			|| result.query.data_select.row_size != sizeof(s_auction_share_info))
		{
			return;
		}

		dp2ws_auction_req_share_list_end msg;
		msg.buyer_guid = buyer_guid;
		msg.money_type = money_type;
		msg.money_value = money_value;
		msg.item_template_id = item_template_id;
		int32 real_data_num = result.query.data_select.row_count;
		if (real_data_num > auction_share_list_max_num)
		{
			real_data_num = auction_share_list_max_num;
		}

		msg.data_num = real_data_num;
		db_read_data(&result, &msg.data_info, sizeof(s_auction_share_info) * real_data_num);

		dbproxy_service::getInstance().send_message(connindex,  &msg, msg.get_pak_length());

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_del_share_list(");
		_sql << item_guid.server_64 << ");";
		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_check_legion_selling_proc_lua(uint32 connindex, int64 legion_guid, int64 cur_time)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_req_legion_selling_num(");
		_sql << legion_guid << ",";
		_sql << cur_time;
		_sql << ");";

		guid_64 _legion_guid(legion_guid);
		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_check_legion_selling_end_proc, _1, connindex, _legion_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_check_legion_selling_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_check_legion_selling request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_check_legion_selling_proc_lua(connindex, request.legion_guid(), request.cur_time());
	}

	void auction_check_legion_selling_end_proc(db_result_type result, uint32 connindex, guid_64 legion_guid)
	{
		dp2ws_auction_check_legion_selling_end msg;
		msg.legion_guid = legion_guid;
		if (!result.error)
		{
			db_read_data(&result, &msg.selling_num, sizeof(int32));
		}
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_auction_check_legion_selling_end));
	}

	void auction_del_bid_replace_proc(int64 item_guid)
	{

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_bid_replace_del(");
		_sql << item_guid << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_del_bid_record_proc_lua(uint32 connindex, int64 item_guid)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_bid_record_del(");
		_sql << item_guid << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);

		auction_del_bid_replace_proc(item_guid);
	}

	void auction_del_bid_record_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_del_bid_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_del_bid_record_proc_lua(connindex, request.item_guid());
	}

	void auction_clear_old_bid_record_proc_lua(uint32 connindex)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_bid_record_clear_old(");
		_sql << utility::get_tick_count() << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_clear_old_bid_record_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_clear_old_bid_record request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_clear_old_bid_record_proc_lua(connindex);
	}

	void ws2dp_req_check_world_auction_sell(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);

		_sql << _XTEXT("call sp_auction_check_wrold_sell(");
		_sql << e_trading_type_world;
		_sql << ", ";
		_sql << utility::get_tick_count();
		_sql << ");";
		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&check_world_auction_sell_end, _1, connindex);

		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void check_world_auction_sell_end(db_result_type result, uint32 connindex)
	{
		dp2ws_auction_check_world_sell_end msg;
		if (!result.error)
		{
			db_read_data(&result, &msg.sell_num, sizeof(int32));
		}

		dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_auction_check_world_sell_end));
	}

	void auction_update_sell_show_proc_lua(uint32 connindex, int32 end_time, const char * data_ptr, size_t data_len)
	{
		server2dp_proto_s_auction_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}
		s_auction_info	auction_info;
		auction_info.proto_set_info(msg);

		int64 cur_time = utility::get_tick_count();
		int64 _end_time = cur_time + end_time * 1000;

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_show(");
		_sql << cur_time,
			_sql << ",";
		_sql << _end_time;
		_sql << ",";
		_sql << auction_info.item_info.item_guid.server_64 << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_update_sell_show_proc_end, _1, connindex, auction_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_update_sell_show_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_show_sell request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		server2dp_proto_s_auction_info msg = request.data_info();
		s_auction_info	auction_info;
		auction_info.proto_set_info(msg);

		int64 cur_time = utility::get_tick_count();
		int64 _end_time = cur_time + request.end_time() * 1000;

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_show(");
		_sql << cur_time,
			_sql << ",";
		_sql << _end_time;
		_sql << ",";
		_sql << auction_info.item_info.item_guid.server_64 << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_update_sell_show_proc_end, _1, connindex, auction_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_update_sell_show_proc_end(db_result_type result, uint32 connindex, s_auction_info auction_info)
	{
		dp2ws_auction_show_sell_end msg;
		msg.data_info = auction_info;
		if (!result.error)
		{
			db_read_data(&result, &msg.result, sizeof(int32));
		}
		dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_auction_show_sell_end));
	}
	void auction_first_show_proc(int32 is_first, int64 sell_time, int64 deal_time)
	{
		if (is_first != 1)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_show_all(");
		_sql << sell_time << ",";
		_sql << deal_time;
		_sql << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_time_out_proc_lua(uint32 connindex, int64 cur_mil_sec, int64 sell_time, int64 deal_time, int32 is_first)
	{
		if (is_first == 1)
		{
			auction_first_show_proc(is_first, sell_time, deal_time);
		}

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_get_time_out(");
		_sql << cur_mil_sec << ",";
		_sql << auction_time_out_sell_num,
			_sql << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&auction_time_out_proc_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void auction_time_out_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_auction_time_out request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		auction_time_out_proc_lua(connindex, request.cur_mil_sec(), request.sell_time(), request.deal_time(), request.is_first());
	}

	dp2ws_aution_time_out_end  g_time_out_msg;

	void auction_time_out_proc_end(db_result_type result, uint32 connindex)
	{
		//dp2ws_aution_time_out_end msg;

		g_time_out_msg.clear();

		if (result.query.data_select.row_size == sizeof(s_auction_info))
		{
			int32 temp_data_num = result.query.data_select.row_count;
			if (temp_data_num > auction_time_out_sell_num)
			{
				temp_data_num = auction_time_out_sell_num;
			}
			db_read_data(&result, &g_time_out_msg.data_info, sizeof(s_auction_info) * temp_data_num);
			g_time_out_msg.data_num = temp_data_num;


			sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
			_sql << _XTEXT("call sp_auction_del_batch(\"");
			int32 last_index = temp_data_num - 1;
			for (int32 i = 0; i < temp_data_num; ++i)
			{
				_sql << g_time_out_msg.data_info[i].item_info.item_guid.server_64;

				if (i != last_index)
				{
					_sql << ",";
				}
			}
			_sql << "\");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			query.handler = boost::bind(&auction_send_time_out_proc, _1, g_time_out_msg, connindex);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}
	void auction_send_time_out_proc(db_result_type result, const dp2ws_aution_time_out_end& msg, uint32 connindex)
	{
		dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());
	}

}
