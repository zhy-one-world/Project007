/********************************************************************
	file name:	msgproc_auction.cpp
	created:	2017/06/11 0:45
	author:		zhy
	purpose:	
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_auction.hpp"
#include "time.hpp"
#include <char_msg.hpp>
#include <auction_msg.hpp>
#include "utility/parse_msg.h"
#include "template/template_manager.h"
#include "dp2cs.pb.h"

namespace faith
{
	

	void cs2dp_auction_load_self_selling(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_auction_load_self_selling(") << role_guid.server_64 <<  "," << max_auction_sell_num << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_auction_self_selling;
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_auction_load_self_selling_end, _1, connindex, role_guid, unit_array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	static void cs2dp_load_auction_self_selling_send_lua(uint32 connindex, const dp2cs_auction_load_self_selling_end& reply_data)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(reply_data.role_guid);
		msg.set_unit_array_index(reply_data.unit_array_index);

		faith::db_proto::auction_db  msg_db;
		msg_db.set_row_count(reply_data.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::db_proto::auction_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_seller_guid(reply_data.auction_info[i].seller_guid.server_64);
			db_row->set_seller_name(reply_data.auction_info[i].seller_name);
			db_row->set_buyer_guid(reply_data.auction_info[i].buyer_guid.server_64);
			db_row->set_buyer_name(reply_data.auction_info[i].buyer_name);
			db_row->set_trade_type(reply_data.auction_info[i].trade_type);
			db_row->set_seller_vip_level(reply_data.auction_info[i].seller_vip_level);
			db_row->set_auction_type(reply_data.auction_info[i].auction_type);
			db_row->set_auction_sub_type(reply_data.auction_info[i].auction_sub_type);
			db_row->set_item_color(reply_data.auction_info[i].item_color);
			db_row->set_quality_level(reply_data.auction_info[i].quality_level);
			db_row->set_class_limit(reply_data.auction_info[i].class_limit);
			db_row->set_item_name(reply_data.auction_info[i].item_name);
			db_row->set_sell_time(reply_data.auction_info[i].sell_time);
			db_row->set_deal_time(reply_data.auction_info[i].deal_time);
			db_row->set_sell_money_type(reply_data.auction_info[i].sell_money_type);
			db_row->set_sell_total_money(reply_data.auction_info[i].sell_total_money);
			db_row->set_sell_single_money(reply_data.auction_info[i].sell_single_money);
			db_row->set_cur_bid_money(reply_data.auction_info[i].cur_bid_money);
			db_row->set_is_show(reply_data.auction_info[i].is_show);
			db_row->set_add_time(reply_data.auction_info[i].add_time);
			db_row->set_is_sky_order(reply_data.auction_info[i].is_sky_order);
			db_row->set_show_guid(reply_data.auction_info[i].show_guid.server_64);

			faith::db_proto::item_info_db* item_info = db_row->mutable_item_info();
			if (item_info == nullptr)
			{
				return;
			}
			item_info->set_item_guid(reply_data.auction_info[i].item_info.item_guid.server_64);
			for (int32 j = 0 ; j < e_item_info_max; j++)
			{
				item_info->add_data_ary(reply_data.auction_info[i].item_info.data_ary[j]);
			}
			item_info->set_param_str(reply_data.auction_info[i].item_info.param_str);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}

		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_auction_load_self_selling_end);


	}

	void cs2dp_auction_load_self_selling_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		static s_auction_info sql_data[max_auction_sell_num];
		g_clear_data(sql_data, max_auction_sell_num);

		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size || table_len > max_auction_sell_num)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_auction_self_selling;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			static dp2cs_auction_load_self_selling_end reply_data;
			reply_data.clear_data();

			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.auction_info[i] = sql_data[i];
				reply_data.data_num++;
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_auction_load_self_selling_end);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				cs2dp_load_auction_self_selling_send_lua(connindex, reply_data);
			}
			return;
		}
	}

	void cs2dp_auction_req_auction_list_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auction_req_goods_list* pdata = static_cast<const cs2dp_auction_req_goods_list*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_req_auction_data_num");

		std::string param_string;
		param_string += "(";

		param_string += std::to_string(pdata->main_type);
		param_string += ",";

		param_string += std::to_string(pdata->sub_type);
		param_string += ",";

		param_string += std::to_string(pdata->item_color);
		param_string += ",";

		param_string += std::to_string(pdata->quality_level);
		param_string += ",";

		param_string += std::to_string(pdata->single_money_sort_mode);
		param_string += ",";

		param_string += std::to_string(pdata->total_money_sort_mode);
		param_string += ",";

		param_string += std::to_string(pdata->is_show_gold);
		param_string += ",";

		param_string += std::to_string(pdata->is_show_diamond);
		param_string += ",";

		param_string += std::to_string(pdata->show_page * auction_show_num_per_page);
		param_string += ",";

		param_string += std::to_string(auction_show_num_per_page);
		param_string += ",";

		param_string += std::to_string(pdata->trade_type);
		param_string += ",";

		param_string += std::to_string(pdata->career_type);
		param_string += ",";

		param_string += std::to_string(utility::get_tick_count());
		param_string += ",";

		param_string += std::to_string(pdata->seller_guid.server_64);
		param_string += ",";

		param_string += std::to_string(pdata->self_guid.server_64);
		param_string += ",";

		param_string += "\'";
		param_string += "%";
		param_string += pdata->key_word;
		param_string += "%";
		param_string += "\'";

		param_string += ")";


		_sql << param_string;
		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_auction_data_num_end, _1, connindex, param_string, pdata->role_guid, pdata->show_page);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_auction_req_auction_data_num_end(db_result_type result, uint32 connindex, std::string param, guid_64 role_guid, int32 cur_page_index)
	{
		if (result.error)
		{
			return;
		}
		int32 all_info_num = 0;
		db_read_data(&result, &all_info_num, sizeof(int32));

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_req_auction_list");
		_sql << param;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_auction_list_end_proc, _1, connindex, all_info_num, role_guid, cur_page_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_auction_req_auction_list_end_proc(db_result_type result, uint32 connindex, int32 all_data_num, guid_64 role_guid, int32 cur_page_index)
	{
		int32	table_len = result.query.data_select.row_count;
		int32	single_data_size = sizeof(s_auction_info);
		if (result.error
			|| single_data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 real_data_num = table_len;
			if (real_data_num > auction_show_num_per_page)
			{
				real_data_num = auction_show_num_per_page;
			}
			dp2cs_auction_req_goods_list_end reply_data;
			int32 data_size = real_data_num * single_data_size;
			db_read_data(&result, &reply_data.data_info, data_size);
			reply_data.data_num = real_data_num;
			reply_data.role_guid = role_guid;
			reply_data.cur_page_index = cur_page_index;
			reply_data.total_info_num = all_data_num;
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void cs2dp_auction_req_trade_record_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auction_req_trade_record* pdata = static_cast<const cs2dp_auction_req_trade_record*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find_trade_record_num");

		std::string param_string;
		std::stringstream temp_stringstream;

		temp_stringstream << "(";
		temp_stringstream << pdata->record_owner_guid.server_64 << ",";
		temp_stringstream << pdata->trade_type << ",";
		temp_stringstream << pdata->page_index * auction_record_show_num_per_page << ",";
		temp_stringstream << auction_record_show_num_per_page;
		temp_stringstream << ");";
		temp_stringstream >> param_string;
		_sql << param_string;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_trade_record_num_end, _1, connindex, param_string, pdata->role_guid, pdata->page_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_auction_req_trade_record_num_end(db_result_type result, uint32 connindex, std::string param, guid_64 role_guid, int32 cur_page_index)
	{
		if (result.error)
		{
			return;
		}
		int32 all_info_num = 0;
		db_read_data(&result, &all_info_num, sizeof(int32));

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find_trade_record");
		_sql << param;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_trade_record_proc_end, _1, connindex, all_info_num, role_guid, cur_page_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_auction_req_trade_record_proc_end(db_result_type result, uint32 connindex, int32 all_record_num, guid_64 role_guid, int32 cur_page_index)
	{
		int32	table_len = result.query.data_select.row_count;
		int32	single_data_size = sizeof(s_auction_record_info);
		if (result.error
			|| single_data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 real_data_num = table_len;
			if (real_data_num > auction_record_show_num_per_page)
			{
				real_data_num = auction_record_show_num_per_page;
			}
			dp2cs_auction_req_trade_record_end reply_data;
			int32 data_size = real_data_num * single_data_size;
			db_read_data(&result, &reply_data.data_info, data_size);
			reply_data.role_guid = role_guid;
			reply_data.cur_page = cur_page_index;
			reply_data.total_record_num = all_record_num;
			reply_data.data_num = real_data_num;
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void cs2dp_auction_req_self_bid_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auction_req_self_bid_list* pdata = static_cast<const cs2dp_auction_req_self_bid_list*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find_self_bid_num");

		std::string param_string;
		std::stringstream temp_stringstream;

		temp_stringstream << "(";
		temp_stringstream << pdata->role_guid.server_64 << ",";
		temp_stringstream << pdata->trade_type << ",";
		temp_stringstream << pdata->cur_time << ",";
		temp_stringstream << pdata->page_index * auction_show_num_per_page << ",";
		temp_stringstream << auction_show_num_per_page;
		temp_stringstream << ");";
		temp_stringstream >> param_string;
		_sql << param_string;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_self_bid_num_end, _1, connindex, param_string, pdata->role_guid, pdata->page_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_auction_req_self_bid_num_end(db_result_type result, uint32 connindex, std::string param, guid_64 role_guid, int32 cur_page_index)
	{
		if (result.error)
		{
			return;
		}
		int32 all_info_num = 0;
		db_read_data(&result, &all_info_num, sizeof(int32));

		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find_self_bid");
		_sql << param;

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_self_bid_end_proc, _1, connindex, all_info_num, role_guid, cur_page_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_auction_req_self_bid_end_proc(db_result_type result, uint32 connindex, int32 all_data_num, guid_64 role_guid, int32 cur_page_index)
	{
		int32	table_len = result.query.data_select.row_count;
		int32	single_data_size = sizeof(s_auction_info);
		if (result.error
			|| single_data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 real_data_num = table_len;
			if (real_data_num > auction_show_num_per_page)
			{
				real_data_num = auction_show_num_per_page;
			}

			int32 data_size = real_data_num * single_data_size;
			dp2cs_auction_req_goods_list_end reply_data;
			reply_data.data_num = real_data_num;
			reply_data.role_guid = role_guid;
			reply_data.cur_page_index = cur_page_index;
			reply_data.total_info_num = all_data_num;
			db_read_data(&result, reply_data.data_info, data_size);
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void cs2dp_auction_req_another_sell_info_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auction_req_other_sell_info* pdata = static_cast<const cs2dp_auction_req_other_sell_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_auction_find_other_sell_info(");
		_sql << pdata->sell_money_type << ",";
		_sql << pdata->item_template_id;
		_sql << ");";

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&cs2dp_auction_req_another_sell_info_end_proc, _1, connindex, pdata->sell_money_type, pdata->role_guid, pdata->item_template_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_auction_req_another_sell_info_end_proc(db_result_type result, uint32 connindex, int32 check_money_type, guid_64 role_guid, int32 item_template_id)
	{
		int32	table_len = result.query.data_select.row_count;
		int32	single_data_size = sizeof(s_auction_info);
		if (result.error
			|| single_data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 real_data_num = table_len;
			if (real_data_num > auction_show_another_sell_info_num)
			{
				real_data_num = auction_show_another_sell_info_num;
			}
			dp2cs_auction_req_other_sell_info_end reply_data;
			int32 data_size = real_data_num * single_data_size;
			db_read_data(&result, &reply_data.data_info, data_size);
			reply_data.data_num = real_data_num;
			//reply_data.sell_money_type = check_money_type;
			reply_data.sell_money_type = e_money_type_jewel;
			reply_data.role_guid = role_guid;
			reply_data.item_template_id = item_template_id;
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void cs2dp_auction_req_bid_record_failure_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auction_req_bid_record_failure* pdata = static_cast<const cs2dp_auction_req_bid_record_failure*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		if (pdata->oper_type == 1)	
		{
			_sql << _XTEXT("call sp_auction_find_read_bid_record_failure(");
			_sql << pdata->role_guid.server_64;
			_sql << ");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			db_manager::getInstance().get_db_link().game_db.add_query(query);			
		}
		else
		{
			_sql << _XTEXT("call sp_auction_find_bid_record_failure(");
			_sql << pdata->role_guid.server_64;
			_sql << ");";
			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			query.role_guid = pdata->role_guid;
			query.handler = boost::bind(&cs2dp_auction_req_bid_record_failure_end_proc, _1, connindex,pdata->role_guid);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_auction_req_bid_record_failure_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid)
	{
		int32	table_len = result.query.data_select.row_count;
		int32	single_data_size = sizeof(s_auction_bid_replace_info);
		if (result.error
			|| single_data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 real_data_num = table_len;
			if (real_data_num > auction_bid_record_failure_num)
			{
				real_data_num = auction_bid_record_failure_num;
			}
			static dp2cs_aution_req_bid_record_failure_end reply_data;
			reply_data.clear_data();

			int32 data_size = real_data_num * single_data_size;
			db_read_data(&result, &reply_data.data_info, data_size);
			reply_data.data_num = real_data_num;
			reply_data.role_guid = role_guid;
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}

	}

	void cs2dp_auction_req_bid_notice_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_auction_req_bid_notice* pdata = static_cast<const cs2dp_auction_req_bid_notice*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		if (pdata->oper_type == e_auction_req_type_read)
		{
			_sql << _XTEXT("call sp_auction_bid_notice_read(");
			_sql << pdata->role_guid.server_64;
			_sql << ");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
		else if (pdata->oper_type == e_auction_req_type_red)
		{
			_sql << _XTEXT("call sp_auction_bid_notice_red(");
			_sql << pdata->role_guid.server_64;
			_sql << ");";

			db_query_type query;
			query.sql_str.assign(_sql.c_str(), _sql.size());
			query.handler = boost::bind(&cs2dp_auction_req_bid_notice_red_end_proc, _1, connindex, pdata->role_guid);
			db_manager::getInstance().get_db_link().game_db.add_query(query);

		}
		else
		{
			/*		_sql << _XTEXT("call sp_auction_find_bid_notice(");
					_sql << pdata->role_guid.server_64;
					_sql << ");";
					db_query_type query;
					query.sql_str.assign(_sql.c_str(), _sql.size());
					query.role_guid = pdata->role_guid;
					query.handler = boost::bind(&cs2dp_auction_req_bid_notice_end_proc, _1, connindex, pdata->role_guid);
					db_manager::getInstance().get_db_link().game_db.add_query(query);*/
		}
	}
	
	void cs2dp_auction_req_bid_notice_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid)
	{
		int32	table_len = result.query.data_select.row_count;
		int32	single_data_size = sizeof(s_auction_bid_notice_info);
		if (result.error
			|| single_data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			int32 real_data_num = table_len;
			if (real_data_num > auction_bid_notice_max_num)
			{
				real_data_num = auction_bid_notice_max_num;
			}
			dp2cs_auction_req_bid_notice_end reply_data;
			int32 data_size = real_data_num * single_data_size;
			db_read_data(&result, &reply_data.data_info, data_size);
			reply_data.data_num = real_data_num;
			reply_data.role_guid = role_guid;
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}


	void cs2dp_auction_req_bid_notice_red_end_proc(db_result_type result, uint32 connindex, guid_64 role_guid)
	{
		dp2cs_auction_get_red_state_end reply_data;
		reply_data.role_guid = role_guid;
		if (result.error)
		{
			reply_data.result = 1;
		}
		else
		{
			int32 is_red = 0;
			db_read_data(&result, &is_red, sizeof(int32));

			reply_data.is_red = is_red;
		}
		dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_auction_get_red_state_end));
	}
	

}