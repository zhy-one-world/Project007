/********************************************************************
created:	2014/08/20
created:	20:8:2014   19:58
file base:	msgproc_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_ws.hpp"
#include "msgproc_relation.hpp"
#include "msgproc_legion_ws.hpp"
#include "msgproc_best_record_ws.hpp"
#include "msgproc_world_boss_ws.hpp"
#include "msgproc_lucky_draw_record_ws.hpp"
#include "msgproc_big_player_ws.hpp"
#include "msgproc_red_package_ws.hpp"
#include "msgproc_ranking_ws.hpp"
#include "msgproc_marry.hpp"
#include "msgproc_mail_ws.hpp"
#include <time_limit_activity_msg.hpp>
#include "../../dbproxy_service.hpp"
#include "data_manager/data_manager.hpp"
#include <item_msg.hpp>
#include <daemon.hpp>
#include <core.hpp>
#include "server_log.hpp"
#include "time.hpp"
#include "db_manager.hpp"
#include "template/template_manager.h"
#include "utility/parse_msg.h"
#include "net.pb.h"
#include <rlog.hpp>

namespace faith
{
	void get_game_info_ws(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_server_info_load(") << dbproxy_service::getInstance().get_server_id()<<_XTEXT(")");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_get_game_info_ws_proc, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void handler_get_game_info_ws_proc(db_result_type result, uint32 connindex)
	{
		dp2s_proto_dp2ws_get_game_info msg;
		// dp2ws_get_game_info msgData;
		if (result.error || result.query.data_select.row_count != 1)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("handler_get_game_info_ws_proc result.error:{}",  result.error));
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_get_game_info);
			return;
		}
#pragma pack ( push, 1 )
		struct _game_info
		{
			int32			big_group_id;
			int32			server_group_id;
			int32			begin_time;
			int32			server_info_arr[e_server_info_type_max];
			int32			is_close_login;
			int32			gm_commond_arr[e_need_server_cross_max - 1];
			_game_info()
			{
				clear_data();
			}
			void clear_data()
			{
				memset(this, 0, sizeof(*this));
			}
		};
#pragma pack( pop )
		static _game_info game_info;
		game_info.clear_data();

		db_read_data(&result, &game_info, sizeof(game_info));
		msg.set_server_id(game_info.big_group_id * server_group_level_num + game_info.server_group_id);
		msg.set_begin_time(game_info.begin_time);
		for (int32 i = 0; i < e_server_info_type_max; i++)
		{
			msg.add_server_info_arr(game_info.server_info_arr[i]);
		}
		for (int32 i = 0; i < e_need_server_cross_max - 1; i++)
		{
			msg.add_gm_commond_arr(game_info.gm_commond_arr[i]);
		}
		dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_get_game_info);
	}
	void gm_set_open_time_lua(uint32 connindex, uint32 open_time, uint32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_server_info_begin_time(") << dbproxy_service::getInstance().get_server_id() << "," << open_time << _XTEXT(")");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_reload_csv(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		template_manager::get_instance().init_for_dp();
	}

	void gm_set_open_time(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_open_time request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_set_open_time_lua(connindex, request.open_time(), request.listen_port(), request.handle_index());
	}

	void gm_set_cross_open_time_lua(uint32 connindex, uint32 server_info_value, uint32 server_info_type,uint32 listen_port, int64 handle_index)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return;
		}
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		xstring lind_name = "";
		switch (server_info_type)
		{
		case e_server_info_type_begin_cross_server_time:
			lind_name = "begin_cross_server_time = ";
			break;
		case e_server_info_type_server_state:
			lind_name = "server_state = ";
			break;
		case e_server_info_type_server_cross_state:
			lind_name = "server_cross_state = ";
			break;
		case e_server_info_type_begin_first_cross_server_time:
			lind_name = "begin_first_cross_server_time = ";
			break;
		case e_server_info_type_begin_first_merge_server_time:
			lind_name = "begin_first_merge_server_time = ";
			break;
		case e_server_info_type_time_limit_ranking_level:
			lind_name = "time_limit_ranking_level = ";
			break;
		case e_server_info_type_close_exchange:
			lind_name = "is_close_exchange = ";
			break;
		case e_server_info_type_only_create_role:
			lind_name = "is_only_create_role";
			break;
		default:
			return;
			break;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_server_info_need_begin_cross(");
		sql << _XTEXT("'UPDATE server_info SET ");
		sql << lind_name << server_info_value;
		sql << " WHERE big_group_id =" << (dbproxy_service::getInstance().get_server_id() / server_group_level_num) << " AND server_group_id = " << (dbproxy_service::getInstance().get_server_id() % server_group_level_num) << ";');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_set_cross_open_time(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_set_server_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_set_cross_open_time_lua(connindex, request.server_info_value(), request.server_info_type(), request.listen_port(), request.handle_index());
	}

	void gm_set_new_password_lua(uint32 connindex, xstring account, xstring new_password, uint32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		xchar	t_account[max_account_length + 1];
		memset(t_account, 0, sizeof(t_account));
		memcpy(t_account, account.c_str(), account.size() > sizeof(t_account) ? sizeof(t_account) : account.size());


		xchar	t_new_password[max_password_length + 1];
		memset(t_new_password, 0, sizeof(t_new_password));
		memcpy(t_new_password, new_password.c_str(), new_password.size() > sizeof(t_new_password) ? sizeof(t_new_password) : new_password.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call " << data_manager::get_instance().get_db_name(t_account) << ".sp_gm_set_new_password(";
		sql << "\'" << t_account << "\',";
		sql << "\'" << t_new_password << "\'";
		sql << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_set_new_password(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_change_password request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_set_new_password_lua(connindex, request.account(), request.new_password(), request.listen_port(), request.handle_index());
	}

	void gm_set_begin_cross_lua(uint32 connindex, uint32 need_begin_cross, uint32 gm_type, int64 req_http_id)
	{
		xstring lind_name = "";
		switch (gm_type)
		{
		case e_need_server_cross_begin_cross:
			lind_name = "is_begin_cross = ";
			break;
		case e_need_server_cross_begin_cross_pk:
			lind_name = "is_begin_cross_pk = ";
			break;
		case e_need_server_cross_begin_cross_activity:
			lind_name = "is_begin_cross_activity = ";
			break;
		case e_need_server_cross_begin_cross_harry:
			lind_name = "is_begin_cross_harry = ";
			break;
		case e_need_server_cross_begin_cross_city_war:
			lind_name = "is_begin_cross_city_war = ";
			break;
		case e_need_server_cross_begin_cross_legion_territory_war:
			lind_name = "is_begin_cross_territory_war = ";
			break;
		case e_need_server_cross_begin_cross_boss:
			lind_name = "is_begin_cross_world_boss = ";
			break;
		case e_need_server_cross_begin_cross_ladder:
			lind_name = "is_begin_cross_ladder = ";
			break;
		case e_need_server_cross_begin_cross_cloud:
			lind_name = "is_begin_cross_cloud = ";
			break;
		case e_need_server_element_war:
			lind_name = "is_begin_element_war = ";
			break;
		case e_need_server_attack_city:
			lind_name = "is_begion_attack_city = ";
			break;
		default:
			return;
			break;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_server_info_need_begin_cross(");
		sql << _XTEXT("'UPDATE server_info SET ");
		sql << lind_name << need_begin_cross;
		sql << " WHERE big_group_id =" << (dbproxy_service::getInstance().get_server_id() / server_group_level_num) << " AND server_group_id = " << (dbproxy_service::getInstance().get_server_id() % server_group_level_num) << ";');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());

		s_daemon_callback_info temp_info;
		temp_info.handle_index = req_http_id;

		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, temp_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_set_begin_cross(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_need_begin_cross request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_set_begin_cross_lua(connindex, request.need_begin_cross(), request.gm_type(), request.req_http_id());
	}
	//获取游戏角色信息给ws
	void get_char_info_ws_lua(uint32 connindex, int64 role_guid, xstring role_account)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_ws_role_info_load(") << role_guid << _XTEXT(")");
		db_query_type query;
		query.logic_id = e_data_flag_info;
		query.role_guid = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_get_char_info_ws_proc, _1, connindex, role_guid, role_account);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void get_char_info_ws(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_character request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		get_char_info_ws_lua(connindex, request.role_guid(), request.role_account());
	}

	void handler_get_char_info_ws_proc(db_result_type result, uint32 connindex, guid_64 role_guid, xstring role_account)
	{
		dp2s_proto_dp2ws_load_character msg;
		auto p_unit_info = msg.mutable_role_info();
		p_unit_info->set_role_guid(role_guid.server_64);

		if (result.error)
		{
			msg.set_eresult(e_error_code_db_error);
			CONSOLE_ERROR("handler_get_char_info_ws_proc result.error:{}", result.error);
		}
		else if (result.query.data_select.row_count == 1)
		{
#pragma pack ( push, 1 )
			struct _character_info
			{
				s_unit_info		role_info;
				s_unit_ws_info	role_ws_info;
				int32			is_login;
				int32			is_send_chat;
				int64			worship_value;

				_character_info()
				{
					memset(this, 0, sizeof(*this));
				}
			};
#pragma pack( pop )
			_character_info sql_info;
			db_read_data(&result, &sql_info, sizeof(sql_info));
			if (0 != strcmp(sql_info.role_info.account, role_account.c_str()))
			{
				msg.set_eresult(e_error_code_db_error);
				CONSOLE_ERROR("handler_get_char_info_ws_proc result. wrong guid:{}", role_guid.server_64);
			}
			else
			{
				p_unit_info->set_account(sql_info.role_info.account);
				p_unit_info->set_role_name(sql_info.role_info.role_name);
				p_unit_info->set_role_appearance(sql_info.role_info.role_appearance);
				p_unit_info->set_role_first_kill_npc_id_arr(sql_info.role_info.role_first_kill_npc_id_arr);
				p_unit_info->set_map_guid(sql_info.role_info.map_guid.server_64);
				for (int32 i = 0; i < e_role_i64_info_max; i++)
				{
					p_unit_info->add_data_i64_ary(sql_info.role_info.data_i64_ary[i]);
				}
				for (int32 i = 0; i < e_role_info_max; i++)
				{
					if (i == e_role_info_server_id)
					{
						p_unit_info->add_data_ary(SERVERCONFIG->game_id);
						continue;
					}
					p_unit_info->add_data_ary(sql_info.role_info.data_ary[i]);
				}
				msg.set_eresult(e_error_code_success);
				for (int32 i = 0; i < e_role_ws_info_max; i++)
				{
					msg.add_ws_data_ary(sql_info.role_ws_info.data_ary[i]);
				}
				msg.set_is_login(sql_info.is_login);
				msg.set_is_send_chat(sql_info.is_send_chat);
				msg.set_worship_value(sql_info.worship_value);

			}
		}
		else
		{
			msg.set_eresult(e_error_code_db_error);
			CONSOLE_ERROR("handler_get_char_info_ws_proc result.query.data_select.row_count:{} role_guid = {}", result.query.data_select.row_count, role_guid.server_64);
		}
		dbproxy_service::getInstance().send_message(connindex,  &msg, e_msg_dp2ws_load_character);
	}

	void gm_set_role_right_lua(uint32 connindex, xstring role_mark, uint32 right_type, uint32 right_value, uint32 right_time, uint32 listen_port, int64 handle_index)
	{
		xchar	t_role_mark[max_account_length + 1];
		memset(t_role_mark, 0, sizeof(t_role_mark));
		memcpy(t_role_mark, role_mark.c_str(), role_mark.size() > sizeof(t_role_mark) ? sizeof(t_role_mark) : role_mark.size());

		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_gm_set_role_right('")
			<< t_role_mark << _XTEXT("',")
			<< right_type << _XTEXT(",")
			<< right_value << _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_set_role_right(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_set_role_right request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_set_role_right_lua(connindex, request.role_mark(), request.right_type(), request.right_value(), request.right_time(), request.listen_port(), request.handle_index());
	}
	void handler_gm_set_proc(db_result_type result, uint32 connindex, s_daemon_callback_info callback_info)
	{
		dp2s_proto_dp2ws_gm_set_result msg;

		msg.set_listen_port(callback_info.listen_port);
		msg.set_handle_index(callback_info.handle_index);
		msg.set_result_code((int32)e_http_error_code_no_error);

		xstring error_msg = "result is ok";
		xchar	result_msg[gm_set_result_len + 1];
		memset(result_msg, 0, sizeof(result_msg));
		if (result.error > 0)
		{
			msg.set_result_code((int32)e_http_error_code_db_error);
			error_msg = "db error code ";
			error_msg += result.error;
		}
		int32 msg_len = error_msg.length();
		if (msg_len > gm_set_result_len)
		{
			msg_len = gm_set_result_len;
		}
		memcpy(result_msg, error_msg.c_str(), msg_len);
		msg.set_result_msg(result_msg);

		dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_gm_set_result);
	}

	void save_recharge(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_save_recharge* pdata = static_cast<const ws2dp_save_recharge*>(data_ptr);
		if (nullptr == pdata)
		{
			//dp2ws_send_recharge_end_result msgData;
			//msgData.result_type = e_recharge_error_sp_save_recharge_pData_is_null;
			//dbproxy_service::getInstance().send_message(connindex,  &msgData, sizeof(msgData));

			dp2s_proto_dp2ws_send_recharge_end_result msgData;
			msgData.set_result_type((int32)e_recharge_error_sp_save_recharge_pData_is_null);
			dbproxy_service::getInstance().send_message(connindex, &msgData, e_msg_dp2ws_send_recharge_end);
			return;
		}

		login_fixed_data temp_third_info = pdata->third_info;
		xstring order_id_str = xstring(pdata->order_id);
		server_log::recharge_step_log(order_id_str, pdata->role_guid, pdata->game_goods_id, e_recharge_step_sp_save_recharge, temp_third_info, pdata->login_type);

		int64 now_time = utility::get_tick_count();
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_save_recharge('")
			<< pdata->order_id << _XTEXT("','")
			<< pdata->order_token << _XTEXT("',")
			<< pdata->role_guid.server_64 << _XTEXT(",")
			<< pdata->game_goods_id << _XTEXT(",")
			<< pdata->payment_type << _XTEXT(",")
			<< pdata->direct_diamond << _XTEXT(",")
			<< now_time << _XTEXT(")");

		db_query_type query;
		query.role_guid = pdata->role_guid;
		query.logic_id = e_msgindex_ws2dp_save_recharge;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_save_recharge_end, _1, connindex, *pdata);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void handler_save_recharge_end(db_result_type result, uint32 connindex, ws2dp_save_recharge recharge_info)
	{
		login_fixed_data temp_third_info = recharge_info.third_info;
		server_log::recharge_step_log(recharge_info.order_id, recharge_info.role_guid, recharge_info.game_goods_id, e_recharge_step_sp_save_recharge_end, temp_third_info, recharge_info.login_type);

		//dp2ws_save_recharge msgData;
		//strcpy(msgData.order_id, recharge_info.order_id);
		//msgData.role_guid = recharge_info.role_guid;
		//msgData.game_goods_id = recharge_info.game_goods_id;
		//msgData.pay_price = recharge_info.pay_price;
		//msgData.order_num = recharge_info.order_num;
		//msgData.payment_type = recharge_info.payment_type;
		//msgData.client_uid = recharge_info.client_uid;
		//msgData.direct_diamond = recharge_info.direct_diamond;
		//strcpy(msgData.order_token, recharge_info.order_token);

		dp2s_proto_dp2ws_save_recharge rechargeMsg;
		rechargeMsg.set_order_id(recharge_info.order_id);
		rechargeMsg.set_role_guid(recharge_info.role_guid.server_64);
		rechargeMsg.set_game_goods_id(recharge_info.game_goods_id);
		rechargeMsg.set_pay_price(recharge_info.pay_price);
		rechargeMsg.set_order_num(recharge_info.order_num);
		rechargeMsg.set_payment_type(recharge_info.payment_type);
		rechargeMsg.set_direct_diamond(recharge_info.direct_diamond);
		rechargeMsg.set_client_uid(recharge_info.client_uid);
		rechargeMsg.set_order_token(recharge_info.order_token);

		if (result.error
			|| recharge_info.force_error_code != e_recharge_error_none)
		{
			dp2s_proto_dp2ws_send_recharge_end_result endmsg;

			if (recharge_info.force_error_code != e_recharge_error_none)
			{
				endmsg.set_result_type(recharge_info.force_error_code);
			}
			else
			{
				endmsg.set_result_type((int32)e_recharge_error_sp_save_recharge_end_error);
			}
			endmsg.set_order_num(recharge_info.order_num);
			endmsg.set_payment_type(recharge_info.payment_type);
			endmsg.set_role_guid(recharge_info.role_guid.server_64);
			endmsg.set_goods_id(recharge_info.game_goods_id);
			endmsg.set_direct_diamond(recharge_info.direct_diamond);
			endmsg.set_order_id(recharge_info.order_id);

			dbproxy_service::getInstance().send_message(connindex,  &endmsg, e_msg_dp2ws_send_recharge_end);
			return;
		}

		int32 t_sucess = 0;
		db_read_data(&result, &t_sucess, sizeof(int32));

		rechargeMsg.set_success(t_sucess);

		dbproxy_service::getInstance().send_message(connindex, &rechargeMsg, e_msg_dp2ws_save_recharge);
	}

	void save_recharge_end(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_save_recharge_end* pdata = static_cast<const ws2dp_save_recharge_end*>(data_ptr);
		if (nullptr == pdata)
		{
			//dp2ws_send_recharge_end_result error_msgData;
			//error_msgData.result_type = e_recharge_error_save_recharge_end_pData_is_null;
			//dbproxy_service::getInstance().send_message(connindex,  &error_msgData, sizeof(error_msgData));
			//return;

			dp2s_proto_dp2ws_send_recharge_end_result error_msgData;
			error_msgData.set_result_type((int32)e_recharge_error_save_recharge_end_pData_is_null);
			dbproxy_service::getInstance().send_message(connindex, &error_msgData, e_msg_dp2ws_send_recharge_end);
			return;
		}
		int64 now_time = utility::get_tick_count();
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_recharge_process_sucess('")
			<< pdata->order_id << _XTEXT("',")
			<< pdata->payment_type << _XTEXT(",")
			<< now_time << _XTEXT(")");

		db_query_type query;
		query.role_guid = pdata->role_guid;
		//query.logic_id = e_msgindex_ws2dp_save_recharge;
		query.sql_str.assign(sql.c_str(), sql.size());
		//query.handler = boost::bind(&handler_save_recharge_end, _1, pdata->role_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_change_account_lua(uint32 connindex, xstring change_name, xstring target_name, uint32 listen_port, int64 handle_index)
	{
		xchar	t_change_name[max_account_length + 1];
		memset(t_change_name, 0, sizeof(t_change_name));
		memcpy(t_change_name, change_name.c_str(), change_name.size() > sizeof(t_change_name) ? sizeof(t_change_name) : change_name.size());

		xchar	t_target_name[max_account_length + 1];
		memset(t_target_name, 0, sizeof(t_target_name));
		memcpy(t_target_name, target_name.c_str(), target_name.size() > sizeof(t_target_name) ? sizeof(t_target_name) : target_name.size());

		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_gm_change_account('" << t_change_name << "','" << t_target_name << "');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_change_account, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_change_account(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_change_account request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_change_account_lua(connindex, request.change_name(), request.target_name(), request.listen_port(), request.handle_index());
	}
	void handler_gm_change_account(db_result_type result, uint32 connindex, s_daemon_callback_info callback_info)
	{
		dp2s_proto_dp2ws_gm_change_account msg;

		msg.set_listen_port(callback_info.listen_port);
		msg.set_handle_index(callback_info.handle_index);
		if (result.error)
		{
			msg.set_change_res(1);
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_gm_change_name);
		}
		else
		{
			int32 data_num = result.query.data_select.row_count;
			if (data_num > 0)
			{
				int32 t_change_res = 0;
				int32 data_size = result.query.data_select.row_size * data_num;
				db_read_data(&result, &t_change_res, data_size);
				msg.set_change_res(t_change_res);
			}
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_gm_change_name);
		}
	}

	void gm_del_role_cache_lua(uint32 connindex, int64 role_guid)
	{
		data_manager::get_instance().del_data_set(role_guid);
	}

	void gm_del_role_cache(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_del_cache request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_del_role_cache_lua(connindex, request.role_guid());
	}

	void gm_del_role_lua(uint32 connindex, int64 role_guid, uint32 is_del, uint32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_gm_del_role(" << role_guid << "," << is_del << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_del_role(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_del_role request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_del_role_lua(connindex, request.role_guid(), request.is_del(), request.listen_port(), request.handle_index());
	}

	void gm_stop_login_lua(uint32 connindex, uint32 is_stop, uint32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_gm_stop_login(" << is_stop << "," << dbproxy_service::getInstance().get_server_id() << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_set_proc, _1, connindex, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_stop_login(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_stop_login request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_stop_login_lua(connindex, request.is_stop(), request.listen_port(), request.handle_index());
	}

	void save_server_attr_val_lua(uint32 connindex, uint32 attr_type, int64 attr_value)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_server_refresh_save(" << attr_type << ",";
		sql << "'(";
		sql << attr_type << ",";
		sql << attr_value << "";
		sql << ")');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_server_attr_val_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_server_attr_val(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_server_attr_val request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_server_attr_val_lua(connindex, request.attr_type(), request.attr_val());
	}

	void save_server_attr_val_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}

	void load_server_attr_val_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_server_refresh_load();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_server_attr_val_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void load_server_attr_val(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		/*server2dp_proto_ws2dp_load_server_attr_val request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}*/
		load_server_attr_val_lua(connindex);
	}

	void load_server_attr_val_end(db_result_type result, uint32 connindex)
	{
		if (result.error)
		{
			dp2ws_load_server_attr_val_end load_server_msg;
			dbproxy_service::getInstance().send_message(connindex,  &load_server_msg, sizeof(load_server_msg));
		}
		else
		{
			dp2ws_load_server_attr_val_end load_server_msg;
			int32 data_num = result.query.data_select.row_count;
			load_server_msg.attr_num = data_num;
			if (data_num > 0)
			{
				int32 data_size = result.query.data_select.row_size * data_num;
				db_read_data(&result, load_server_msg.attr_int_val, data_size);
			}
			dbproxy_service::getInstance().send_message(connindex,  &load_server_msg, sizeof(load_server_msg));
		}

		load_all_legion_info(connindex, nullptr, 0);
		load_all_best_record(connindex, nullptr, 0);
		//load_world_boss(connindex, nullptr, 0);
		load_draw_info_by_db(connindex, nullptr, 0);
		load_red_package_by_sql(connindex, nullptr, 0);
		load_arena_rank(connindex, nullptr, 0);
		ws2dp_req_load_marry_info(connindex, nullptr, 0);
		load_big_player(connindex, nullptr, 0);
		//clear_spirit_info();
		for (int32 ranking_type = 0; ranking_type < e_RankingIndex_max; ++ranking_type)
		{
			load_all_ranking_type(connindex, ranking_type);
		}
		for (int32 rank_type = 0; rank_type < e_service_rank_type_max; ++rank_type)
		{
			load_all_service_rank_type(connindex, rank_type);
		}
		load_other_server_mail_record(connindex, nullptr, 0);
		load_mail_event(connindex, nullptr, 0);
		load_mail_event_item(connindex, nullptr, 0);

		set_all_player_check_guide_info(connindex);
	}

	void ws2dp_req_check_player_name(uint32 connindex, const void* data_ptr, size_t data_len)
	{

		faith::server2dp_proto::ws2dp_check_player_name request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		ws2dp_check_player_name  packet;
		packet.from_proto(request);
		ws2dp_req_check_player_name_(connindex, &packet, sizeof(ws2dp_check_player_name));


	}


	void ws2dp_req_check_player_name_(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_check_player_name *pdata = static_cast<const ws2dp_check_player_name*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		xstring role_name = pdata->role_name;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_check_role_name('")
			<< db_manager::getInstance().get_db_names().c_str()
			<< "',"
			<< db_manager::getInstance().get_data_name_map_size()
			<< ",'"
			<< role_name.c_str()
			<< _XTEXT("')");

			db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&ws2dp_req_check_player_name_end, _1, connindex, pdata->role_guid, role_name);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_check_player_name_end(db_result_type result, uint32 connindex, guid_64 role_guid, std::string role_name)
	{
		if (!result.error)
		{
			dp2ws_check_player_name_end msg;
			db_read_data(&result, &msg.result, sizeof(int32));
			msg.set_role_name(role_name);
			msg.role_guid = role_guid;
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_check_player_name_end));
		}
		else
		{
			dp2ws_check_player_name_end msg;
			msg.result = e_change_failed_role_name_invalid;
			msg.set_role_name(role_name);
			msg.role_guid = role_guid;
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(dp2ws_check_player_name_end));
		}
	}

	void ws2dp_req_change_player_name(uint32 connindex, const void* data_ptr, size_t data_len)
	{

		faith::server2dp_proto::ws2dp_change_player_name request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		ws2dp_change_player_name  packet;
		packet.from_proto(request);
		ws2dp_req_change_player_name_(connindex, &packet, sizeof(ws2dp_change_player_name));

	}

	void ws2dp_req_change_player_name_(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_change_player_name *pdata = static_cast<const ws2dp_change_player_name*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		xstring role_name = pdata->role_name;

		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")
				<< it->second.c_str()
				<< _XTEXT(".sp_change_role_name(")
				<< pdata->role_guid.server_64
				<< ",'"
				<< role_name.c_str()
				<< _XTEXT("')");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.role_guid = pdata->role_guid;
			query.handler = boost::bind(&ws2dp_req_change_player_name_end, _1, connindex, pdata->role_guid, role_name);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}
	void ws2dp_req_change_player_name_end(db_result_type result, uint32 connindex, guid_64 role_guid, std::string role_name)
	{
		if (!result.error)
		{
			dp2ws_change_player_name_end result_msg;
			result_msg.role_guid = role_guid;
			result_msg.set_role_name(role_name);

			dbproxy_service::getInstance().send_message(connindex,  &result_msg, sizeof(dp2ws_change_player_name_end));
		}
	}
	void send_to_cs_dp_connect_success(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		dp2cs_connect_success req;
		dbproxy_service::getInstance().send_message(connindex, &req, sizeof(req));
	}

	void load_server_last_server_list_lua(uint32 connindex, uint32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_last_server_list_load(");
		sql << server_id << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_server_last_server_list_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_server_last_server_list(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_last_server_list request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_server_last_server_list_lua(connindex, request.server_id());
	}

	void load_server_last_server_list_end(db_result_type result, uint32 connindex)
	{
		s_last_server_list_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_last_server_list_db);
		if (result.error || result.query.data_select.row_size != data_size)
		{
			return;
		}
		else
		{
			sql_data.clear_data();
			db_read_data(&result, &sql_data, data_size * table_len);
			//dp2ws_load_last_server_list msg;
			//memcpy(&(msg.last_server_list), &(sql_data), sizeof(s_last_server_list_db));
			//dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(msg));

			dp2s_proto_dp2ws_load_last_server_list proto_msg;
			proto_msg.set_server_id(sql_data.server_id);
			proto_msg.set_last_server_list(sql_data.last_server_list);
			dbproxy_service::getInstance().send_message(connindex, &proto_msg, e_msg_dp2ws_load_server_list_end);

			return;
		}
	}

	void save_server_last_server_list_lua(uint32 connindex, uint32 server_id, xstring last_server_list)
	{
		xchar	t_last_server_list[last_server_list_max_length];
		memset(t_last_server_list, 0, sizeof(t_last_server_list));
		memcpy(t_last_server_list, last_server_list.c_str(), last_server_list.size() > sizeof(t_last_server_list) ? sizeof(t_last_server_list) : last_server_list.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_last_server_list_save(";
		sql << server_id << ",";
		sql << "'(";
		sql << server_id << ",";
		sql << "\\\'" << t_last_server_list << "\\\')'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_last_server_list_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_server_last_server_list(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_last_server_list request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_server_last_server_list_lua(connindex, request.server_id(), request.last_server_list());
	}

	void save_last_server_list_end(db_result_type result, uint32 connindex)
	{

	}

	void load_ws_time_limit_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_load_time_limit_ws_info* pdata = static_cast<const ws2dp_load_time_limit_ws_info*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		int32 cur_data_num = 0;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_time_limit_ws_activity_load(")
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_ws_time_limit_func_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_ws_time_limit_func_end(db_result_type result, uint32 connindex)
	{
		static s_time_limit_ws_info sql_data[e_time_limit_activity_type_max];
		g_clear_data(sql_data, e_time_limit_activity_type_max);

		int32 data_size = sizeof(s_time_limit_ws_info);
		int32 data_num = result.query.data_select.row_count;
		if (result.error || data_num > e_time_limit_activity_type_max)
		{
			return;
		}
		db_read_data(&result, &sql_data, data_size * data_num);
		static dp2ws_load_time_limit_ws_info_end msg;
		msg.clear_data();

		msg.data_num = 0;
		msg.is_end = 0;
		int32 send_num = 0;
		for (int32 i = 0; i < data_num; i++)
		{
			memcpy(&(msg.act_info[send_num]), &(sql_data[i]), sizeof(s_time_limit_ws_info));
			send_num++;
			if (send_num >= time_limit_activity_dp_send_size)
			{
				msg.data_num = time_limit_activity_dp_send_size;
				dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_load_time_limit_ws_info_end));
				send_num = 0;
				msg.clear_data();
			}
		}
		msg.is_end = 1;
		msg.data_num = send_num;
		dbproxy_service::getInstance().send_message(connindex, &msg, sizeof(dp2ws_load_time_limit_ws_info_end));
		return;
	}

	void save_ws_time_limit_func(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_save_time_limit_ws_info* pdata = static_cast<const ws2dp_save_time_limit_ws_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (data_len != sizeof(ws2dp_save_time_limit_ws_info))
		{
			return;
		}
		if (pdata->data_num > e_time_limit_activity_type_max)
		{
			return;
		}
		for (int32 i = 0; i < pdata->data_num; i++)
		{
			const s_time_limit_ws_info& time_limit_info = pdata->act_info[i];

			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << "call sp_time_limit_ws_activity_save(";
			sql << time_limit_info.act_info[e_time_limit_ws_act_info_type] << ",";
			sql << "'(";
			for (int32 j = e_time_limit_ws_act_info_type; j < e_time_limit_ws_act_info_max; j++)
			{
				sql << time_limit_info.act_info[j] << ",";
			}
			sql << "\\\'" << time_limit_info.param_string_1 << "\\\',";
			sql << "\\\'" << time_limit_info.param_string_2 << "\\\'";
			sql << ")');";
			db_query_type query;
			query.logic_id = e_data_flag_type_time_limit_activity_ws_info;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&save_ws_time_limit_func_end, _1, connindex);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void save_ws_time_limit_func_end(db_result_type result, uint32 connindex)
	{

	}

	void save_account_online_time(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_save_account_online_time *pdata = static_cast<const ws2dp_save_account_online_time*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		xstring account = pdata->account;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_save_account_online_time('")
			<< account.c_str() << _XTEXT("','")
			<< pdata->online_duration << _XTEXT("','")
			<< pdata->online_time << _XTEXT("','")
			<< pdata->month_recharge_num << _XTEXT("','")
			<< pdata->month_recharge_time
			<< _XTEXT("')");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_account_online_time_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_account_online_time_end(db_result_type result, uint32 connindex)
	{
		db_result_type a = result;
	}

	void set_all_player_check_guide_info(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_set_all_player_check_guid(")
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&set_all_player_check_guide_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void set_all_player_check_guide_info_end(db_result_type result, uint32 connindex)
	{
	}

	void gm_get_role_info_lua(uint32 connindex, xstring role_mark, uint32 get_type, uint32 listen_port, int64 handle_index, uint32 check_type)
	{
		xchar	t_role_mark[max_account_length + 1];
		memset(t_role_mark, 0, sizeof(t_role_mark));
		memcpy(t_role_mark, role_mark.c_str(), role_mark.size() > sizeof(t_role_mark) ? sizeof(t_role_mark) : role_mark.size());


		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		data_manager::get_instance().reset_query_data(role_mark);

		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (auto iter = db_map.begin(); iter != db_map.end(); iter++)
		{
			std::string db_name = iter->second;
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << db_name.c_str() << _XTEXT(".sp_gm_get_role_info('")
				<< t_role_mark << _XTEXT("',")
				<< get_type << _XTEXT(")");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&handler_gm_get_role_info_proc, _1, connindex, callback_info, check_type, role_mark);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void gm_get_role_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_get_role_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_get_role_info_lua(connindex, request.role_mark(), request.get_type(), request.listen_port(), request.handle_index(), request.check_type());
	}

	void handler_gm_get_role_info_proc(db_result_type result, uint32 connindex, s_daemon_callback_info callback_info, int32 check_type, std::string role_mark)
	{
		if (result.error == 0 && result.query.data_select.row_count > 0 && sizeof(s_unit_info) == result.query.data_select.row_size)
		{
			s_unit_info		role_info[faith::max_character_num];
			int32 count_num = result.query.data_select.row_count > faith::max_character_num ? faith::max_character_num : result.query.data_select.row_count;
			db_read_data(&result, role_info, sizeof(s_unit_info) *  count_num );
		
			for (int32 i = 0; i < count_num; ++i)
			{
				s_unit_info& role_info_data = role_info[i];
				data_manager::get_instance().add_unit_info_gm_qeury(role_mark, role_info_data);		
			}		
		}

		if (data_manager::get_instance().check_query_all_db(role_mark))
		{
			std::vector<s_unit_info> unit_info_vec = data_manager::get_instance().get_gm_query_data(role_mark);
			
			dp2s_proto_dp2ws_gm_get_role_info msg;
			msg.set_listen_port(callback_info.listen_port);
			msg.set_handle_index(callback_info.handle_index);
			msg.set_check_type(check_type);
			dp2s_proto_dp2ws_gm_get_role_info_arr* arr_msg = msg.mutable_role_info_arr();
			if (nullptr == arr_msg)
			{
				return;
			}
			int32 data_num = unit_info_vec.size() > faith::max_character_num * 10 ? faith::max_character_num * 10 : unit_info_vec.size();
			arr_msg->set_data_num(data_num);
			for (int32 i = 0; i < data_num; i++)
			{
				dp2s_proto_s_unit_info* p_unit_info = arr_msg->add_role_info();
				if (nullptr == p_unit_info)
				{
					return;
				}
				p_unit_info->set_role_guid(unit_info_vec[i].role_guid.server_64);
				p_unit_info->set_account(unit_info_vec[i].account);
				p_unit_info->set_role_name(unit_info_vec[i].role_name);
				p_unit_info->set_role_appearance(unit_info_vec[i].role_appearance);
				p_unit_info->set_role_first_kill_npc_id_arr(unit_info_vec[i].role_first_kill_npc_id_arr);
				p_unit_info->set_map_guid(unit_info_vec[i].map_guid.server_64);
				for (int32 i = 0; i < e_role_i64_info_max; i++)
				{
					p_unit_info->add_data_i64_ary(unit_info_vec[i].data_i64_ary[i]);
				}
				for (int32 i = 0; i < e_role_info_max; i++)
				{
					p_unit_info->add_data_ary(unit_info_vec[i].data_ary[i]);
				}
			}
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_gm_get_role_info);

			for (int32 i = 0; i < data_num; ++i)
			{
				s_unit_info& role_info = unit_info_vec[i];
				gm_get_role_time(connindex, role_info.role_guid, callback_info);
				gm_get_role_money(connindex, role_info.role_guid, callback_info);
				gm_get_role_lock(connindex, role_info.account, role_info.role_guid, role_info.role_name, callback_info);
			}
			return;
		}
	}

	void gm_modify_role_info_lua(uint32 connindex, int64 role_guid, xstring sql_str, uint32 listen_port, int64 handle_index)
	{
		xchar	t_sql_str[max_gm_sql_str_length + 1];
		memset(t_sql_str, 0, sizeof(t_sql_str));
		memcpy(t_sql_str, sql_str.c_str(), sql_str.size() > sizeof(t_sql_str) ? sizeof(t_sql_str) : sql_str.size());


		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_ws_role_info_save(";
		sql << t_sql_str;
		sql << ");";

		db_query_type query;
		query.role_guid = role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_modify_role_info_end, _1, connindex, (guid_64)role_guid, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_modify_role_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_modify_role_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		gm_modify_role_info_lua(connindex, request.role_guid(), request.sql_str(), request.listen_port(), request.handle_index());
	}
	void handler_gm_modify_role_info_end(db_result_type result, uint32 connindex, guid_64 role_guid, s_daemon_callback_info callback_info)
	{
		dp2s_proto_dp2ws_gm_modify_role_info msg;
		msg.set_role_guid(role_guid.server_64);
		msg.set_listen_port(callback_info.listen_port);
		msg.set_handle_index(callback_info.handle_index);
		dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_gm_modify_role_info);
	}

	void gm_get_role_time(uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_role_time_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_time;
		query.role_guid = role_guid;
		query.handler = boost::bind(&handler_gm_get_role_time_proc, _1, connindex, role_guid, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void handler_gm_get_role_time_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		dp2cs_load_time_info reply_data;
		reply_data.callback_info = callback_info;
		reply_data.role_guid = role_guid;
		s_time_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_time_info_db);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);
			for (int32 i = 0; i < e_time_type_max; ++i)
			{
				reply_data.data_info.data_ary[i] = sql_data.data_ary[i];
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}
	}
	void gm_get_role_money(uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_role_money_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&handler_gm_get_role_money_proc, _1, connindex, role_guid, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void handler_gm_get_role_money_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		dp2cs_load_money_info reply_data;
		reply_data.callback_info = callback_info;
		reply_data.role_guid = role_guid;
		s_money_info_db sql_data[3];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_money_info_db);
		if (result.error || table_len > 3 || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);
			for (int32 i = 0; i < e_money_type_max; ++i)
			{
				reply_data.data_info.cur_data_ary[i] = sql_data[0].data_ary[i];
				reply_data.data_info.all_data_ary[i] = sql_data[1].data_ary[i];
				reply_data.data_info.cost_data_ary[i] = sql_data[2].data_ary[i];
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}
	}
	void gm_get_role_lock(uint32 connindex, xchar* account, guid_64& role_guid, xchar* role_name, s_daemon_callback_info callback_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_gm_get_role_right('")
			<< account << _XTEXT("',")
			<< role_guid.server_64 << _XTEXT(",'")
			<< role_name << _XTEXT("')");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_get_role_lock_proc, _1, connindex, role_guid, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void handler_gm_get_role_lock_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		dp2ws_gm_get_role_lock reply_data;
		reply_data.callback_info = callback_info;
		reply_data.role_guid = role_guid;
		int32	sql_data[2];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(int32);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);
			reply_data.is_login = sql_data[0];
			reply_data.is_send_chat = sql_data[1];
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}
	}

	void gm_get_role_items_info_lua(uint32 connindex, int64 role_guid, uint32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_role_item_load(") << role_guid << "," << callback_info.handle_index << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_item;
		query.role_guid = role_guid;
		query.handler = boost::bind(&handler_gm_get_role_items_info_proc, _1, connindex, (guid_64)role_guid, callback_info);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void gm_get_role_items_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_gm_get_role_items_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		gm_get_role_items_info_lua(connindex,request.role_guid(), request.listen_port(), request.handle_index());
	}

	void handler_gm_get_role_items_info_proc(db_result_type result, uint32 connindex, guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		static dp2cs_load_char_item reply_data;
		reply_data.clear_data();
		reply_data.callback_info = callback_info;
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len > max_item_num || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));
			return;
		}

		s_item_info_db sql_data[max_item_num];
		memset(sql_data, 0, sizeof(sql_data));
		db_read_data(&result, &sql_data, data_size * table_len);

		reply_data.role_guid = role_guid;
		reply_data.data_num = table_len;
		for (int32 i = 0; i < table_len; ++i)
		{
			memcpy(&(reply_data.data_list[i]), &(sql_data[i].data_info), sizeof(s_item_info));
		}
		dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
	}

	void gm_get_server_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_query_server_info_gm* pData = static_cast<const ws2dp_query_server_info_gm*>(data_ptr);
		if (nullptr == pData)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_server_info_query_gm()";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&handler_gm_get_server_info_proc, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void handler_gm_get_server_info_proc(db_result_type result, uint32 connindex)
	{
		dp2s_proto_dp2ws_query_server_info_gm msgData;

		//dp2ws_query_server_info_gm msgData;
		if (result.error || result.query.data_select.row_count == 0)
		{
			dbproxy_service::getInstance().send_message(connindex, &msgData, e_msg_dp2ws_rep_query_server_info_gm);
			return;
		}
		server_info_gm t_gm_info;
		db_read_data(&result, &t_gm_info, sizeof(t_gm_info));
		msgData.set_acc_num(t_gm_info.acc_num);
		msgData.set_role_num(t_gm_info.role_num);
		dbproxy_service::getInstance().send_message(connindex, &msgData, e_msg_dp2ws_rep_query_server_info_gm);
	}

}
