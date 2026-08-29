
#include "web_client.hpp"
#include "game_cfg/servers_config.h"
#include "Internal/internal.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include "server_log.hpp"
#include "utility/init_unit.h"
#include <char_msg.hpp>
#include "utility/guid_gen.h"
#include "utility/cs_date.hpp"
#include "mail_msg.hpp"
#include "ws_client.hpp"
#include "mail/event_ws_mgr.h"
#include "Logic/time_def.hpp"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "activity/time_limit_activity_ws_mgr.h"
#include "activity/time_limit_activity_temp_ws_mgr.h"
#include "activity/cloud_shop_mgr.h"
#include "connection/daemon_client.hpp"
#include "lua/script_mgr.h"
#include "md5/aes.h"
#include "md5/Base64.h"
#include "md5/md5.h"
#include "md5/AESDataConvert.h"
#include "globle_data.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "server/legion/legion_ws_mgr.h"
#include "web.pb.h"
#include "mail/mail_event_ws.h"


namespace faith
{
	web_client::web_client()
	{
		m_gmcode_map["BANCHAT"] = e_web_json_type_send_chat;
		m_gmcode_map["KICK"] = e_web_json_type_kickout;
		m_gmcode_map["EMAIL"] = e_web_json_type_send_mail;
		m_gmcode_map["MARQUEE"] = e_web_json_type_marquee;
		m_gmcode_map["COMMAND"] = e_web_json_type_textcommand;
		m_gmcode_map["RECHARGE"] = e_web_json_type_recharge;
		m_gmcode_map["SERVER_OPEN_UPDATE"] = e_web_json_type_open_time;
		m_gmcode_map["SETNEWPASSWARD"] = e_web_json_type_new_password;
		m_gmcode_map["BANLOGIN"] = e_web_json_type_ban_login;
		clear_data();
	}

	void web_client::clear_data()
	{
		m_all_role_info_map.clear();
	}

	void web_client::tick(int64& new_time)
	{
		static int64 req_srv_info_gm_stamp = 0;
		if (new_time >= req_srv_info_gm_stamp)
		{
			send_get_server_info_gm();
			//int32 online_num = client_session_mgr::getInstance().get_session_num();
			//int32 almost_full = init_player_max * 0.8;
			//if (online_num > almost_full)
			//{
			req_srv_info_gm_stamp = new_time + minute_tick_time;
			//}
			//else
			//{
			//	req_srv_info_gm_stamp = new_time + minute_tick_time * 5;
			//}
		}
		if (new_time >= m_role_info_map_tick)
		{
			m_role_info_map_tick = init_unit::get_end_time(10);
			for (gm_all_info_map_it it = m_all_role_info_map.begin(); it != m_all_role_info_map.end();)
			{
				if (it->second.time < new_time)
				{
					it = m_all_role_info_map.erase(it++);
				}
				else
				{
					++it;
				}
			}
		}
	}
	void web_client::handler_http_onrecv(const s_http_receive_info& receive_info)
	{
		CONSOLE_INFO(" http data:{}", receive_info.m_req_body);
		CONSOLE_INFO(" http path:{}", receive_info.m_path);

		if (receive_info.m_path == "/recharge")
		{
			handler_http_on_recv_recharge_end(receive_info);
		}
		else if (receive_info.m_path == "/enrecharge")
		{
			handler_http_on_en_recharge_end(receive_info);
		}
		else if (receive_info.m_path == "/umisdkrecharge")
		{
			handler_http_on_recv_umisdkrecharge_end(receive_info);
		}
		else if (receive_info.m_path == xstring("/gmtool/operate"))
		{//中文gm指令
			s_daemon_callback_info temp_info;
			temp_info.listen_port = receive_info.m_req_listen_port;
			temp_info.handle_index = receive_info.m_req_handle_index;
			Json::Reader reader;
			Json::Value  json_value;
			Json::FastWriter json_writer;
			if (reader.parse(receive_info.m_req_body, json_value) == false || json_value.isObject() == false
				|| json_value["gmCode"].isNull() || json_value["gmCode"].isString() == false
				|| json_value["data"].isNull() || json_value["data"].isObject() == false)
			{
				send_order_fail(e_web_json_type_textcommand, "hanlde_json string error " + receive_info.m_req_body, receive_info.m_req_handle_index);
				return;
			}
			std::map<xstring, int32>::iterator it = m_gmcode_map.find(json_value["gmCode"].asString());
			if (it == m_gmcode_map.end())
			{
				send_order_fail(e_web_json_type_textcommand, "hanlde_json string error " + receive_info.m_req_body, receive_info.m_req_handle_index);
				return;
			}
			do_gm_order(it->second, json_value["data"], temp_info, true);
		}
		else if (receive_info.m_path == xstring("/recharge/operate"))
		{//中文充值

		}
		else if (receive_info.m_path == xstring("/Aes"))
		{
			Json::Value json_value;
			Json::Reader json_reader;
			Json::FastWriter json_writer;

			s_daemon_callback_info temp_callback_info;
			temp_callback_info.handle_index = receive_info.m_req_handle_index;
			temp_callback_info.listen_port = receive_info.m_req_listen_port;

			xstring data_str = AESDataConvert::DecryptionAES(receive_info.m_req_body);
			CONSOLE_INFO(" Aes End path:{}", data_str);
			if (json_reader.parse(data_str, json_value) == false || json_value.isObject() == false)
			{
				gm_order_end_new(0, "json is error", temp_callback_info);
				return;
			}
			if (json_value["gmCode"].isNull() == false)
			{
				if (json_value["data"].isNull() || !json_value["data"].isObject())
				{
					gm_order_end_new(0, "data is error", temp_callback_info);
					return;
				}
				new_do_gm_order(json_value["gmCode"].asString(), json_value["data"], temp_callback_info);
			}
		}
		else
		{
			Json::Value json_value;
			Json::Reader json_reader;
			Json::FastWriter json_writer;
			if (json_reader.parse(receive_info.m_req_body, json_value) == false || json_value.isObject() == false)
			{
				http_access_mgr::get_instance().repose_client_req(receive_info.m_req_handle_index, "json is error");
				return;
			}
			if (json_value["gmCode"].isNull() == false)
			{//{"gmCode":13,"data":{"openDate":1581762300000},"serverName":"hy","serverId":10200}
				if (!json_value["gmCode"].isInt()
					|| json_value["data"].isNull()
					|| !json_value["data"].isObject())
				{
					http_access_mgr::get_instance().repose_client_req(receive_info.m_req_handle_index, "data is error");
					return;
				}
				s_daemon_callback_info temp_callback_info;
				temp_callback_info.handle_index = receive_info.m_req_handle_index;
				temp_callback_info.listen_port = receive_info.m_req_listen_port;
				do_gm_order(json_value["gmCode"].asInt(), json_value["data"], temp_callback_info, false);
			}
			else if (json_value["serverType"].isNull() == false)
			{//{"serverType":3,"isForce":0}
				if (!json_value["serverType"].isInt())
				{
					http_access_mgr::get_instance().repose_client_req(receive_info.m_req_handle_index, "data is error");
					return;
				}
				int32 server_type = json_value["serverType"].asInt();
				req_stop server_stop_msg;
				server_stop_msg.server_type = e_server_type(server_type);
				switch (server_type)
				{
				case e_server_type_gate:
				{
					ws_client::getInstance().only_send_to_gate(&server_stop_msg, sizeof(server_stop_msg));
				}
				break;
				case e_server_type_ws:
				{
					server_stop_msg.server_type = e_server_type_cs;
					world_server::getInstance().send_to_cs(&server_stop_msg, sizeof(server_stop_msg));
					server_stop_msg.server_type = e_server_type_dp;
					ws_client::getInstance().send_to_dp(&server_stop_msg, sizeof(server_stop_msg));
					daemon_client::getInstance().set_server_close(true);
					server_stop_msg.server_type = e_server_type_fep;
					world_server::getInstance().send_to_fep_all(&server_stop_msg, sizeof(server_stop_msg));
					CONSOLE_INFO("FaithEye Stop Game!");
				}
				break;
				case e_server_type_fep:
				{
					world_server::getInstance().send_to_fep_all(&server_stop_msg, sizeof(server_stop_msg));
				}
				break;
				case e_server_type_cs:
				{
					world_server::getInstance().send_to_cs(&server_stop_msg, sizeof(server_stop_msg));
				}
				break;
				case e_server_type_dp:
				{
					ws_client::getInstance().send_to_dp(&server_stop_msg, sizeof(server_stop_msg));
				}
				break;
				default:
					break;
				}
				http_access_mgr::get_instance().repose_client_req(receive_info.m_req_handle_index, "result is ok");
			}
		}
	}

	void web_client::hanlde_login(const char* data_ptr, uint32 length)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		faith::web_proto::login msg;
		msg.ParseFromArray(data_ptr, length);
		int32 test = msg.biggroupid();
		int32 test2 = msg.servergroupid();
		CONSOLE_INFO("{} {}", test, test2);
	}

	void web_client::hanlde_json(const char* data_ptr, uint32 length)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		faith::web_proto::json_str_http_to_ws msg;
		msg.ParseFromArray(data_ptr, length);
		CONSOLE_INFO(" gm_json_string : {}", msg.datastr());
		Json::Reader reader;
		Json::Value  json_value;
		Json::FastWriter json_writer;
		int64 order_num = msg.ordernum();
		if (msg.datastr().size() <= 0 || reader.parse(msg.datastr(), json_value) == false || json_value.isObject() == false)
		{
			send_order_fail(e_web_json_type_textcommand, "hanlde_json string error " + msg.datastr(), order_num);
			return;
		}

		s_daemon_callback_info temp_info;
		temp_info.listen_port = 0;
		temp_info.handle_index = order_num;

		do_gm_order(msg.jsontype(), json_value, temp_info, true);
	}

	void web_client::do_gm_order(int32 gm_type, Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		if (json_value.isObject() == false)
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(gm_type, "hanlde_json string error ", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "", callback_info);
			}
			return;
		}

		switch (gm_type)
		{
		case e_web_json_type_role_info:
		{
			//已弃用
			//parse_json_get_role_info(json_value, order_guid);
		}
		break;
		case e_web_json_type_send_chat:
		{
			parse_json_set_send_chat(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_kickout:
		{
			parse_json_kickout_player(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_send_mail:
		{
			parse_json_set_send_mail(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_marquee:
		{
			parse_json_set_send_delay_marquee(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_textcommand:
		{
			parse_json_do_text_command(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_activate_code:
		{
			parse_json_activate_code(json_value);
		}
		break;
		case e_web_json_type_robot_op:
		{
			parse_json_robot_op(json_value, callback_info.handle_index);
		}
		break;
		case e_web_json_type_recharge:
		{
			parse_json_recharge(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_open_time:
		{
			parse_json_open_time(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_new_password:
		{
			parse_json_change_password(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		case e_web_json_type_ban_login:
		{
			parse_json_set_ban_login(json_value, callback_info, is_send_to_old_gmt);
		}
		break;
		default:
			break;
		}
	}

	void web_client::gm_order_end(int32 result_code, xstring result_msg, s_daemon_callback_info callback_info)
	{
		http_access_mgr::get_instance().repose_client_req(callback_info.handle_index, result_msg);
	}
	void web_client::gm_order_end_new(int32 result_code, xstring result_msg, s_daemon_callback_info callback_info)
	{
		http_access_mgr::get_instance().new_repose_client_req(callback_info.handle_index, result_code, result_msg);
	}
	void web_client::new_do_gm_order(xstring gm_type, Json::Value& json_value, s_daemon_callback_info callback_info)
	{
		if (json_value.isObject() == false)
		{
			gm_order_end_new(e_http_error_code_json_parse_failed, "json_value nil", callback_info);
			return;
		}

		if (gm_type == "MAIL_SERVER" || gm_type == "MAIL_PLAYER")
		{
			parse_json_set_send_mail_new(json_value, callback_info);
		}
		else if (gm_type == "FORBIDDEN_WORDS")
		{
			parse_json_set_send_chat_new(json_value, callback_info);
		}
		else if (gm_type == "KICK")
		{
			parse_json_kickout_player_new(json_value, callback_info);
		}
	}
	void web_client::set_role_info(const s_unit_info* role_info, int32 data_num, s_daemon_callback_info callback_info, int32 check_type)
	{
		if (m_all_role_info_map.find(callback_info.handle_index) == m_all_role_info_map.end())
		{
			return;
		}

		if (data_num == 0 || role_info == nullptr)
		{
			std::string error_str;
			switch (check_type)
			{
			case e_check_type_ch:
				error_str = "{\"code\": 1, \"msg\" : \"no role\", \"roleInfoList\" : []}";
				break;
			case e_check_type_tw:
				error_str = "{\"ret\": 1001, \"msg\" : \"no role\"}";
				break;
			default:
				error_str = "{\"code\": 1, \"msg\" : \"no role\", \"roleInfoList\" : []}";
			}
			http_access_mgr::get_instance().repose_client_req(callback_info.handle_index, error_str);
			m_all_role_info_map.erase(callback_info.handle_index);
			return;
		}

		s_gm_all_info& temp_role_info_map = m_all_role_info_map[callback_info.handle_index];
		temp_role_info_map.check_type = check_type;
		for (int32 i = 0; i < data_num; ++i)
		{
			guid_64 temp_guid = role_info[i].role_guid;
			gm_info_map::iterator it = temp_role_info_map.info_map.find(temp_guid);
			if (it == temp_role_info_map.info_map.end())
			{
				temp_role_info_map.info_map[temp_guid].role_info = role_info[i];
			}
			else
			{
				it->second.role_info = role_info[i];
			}

		}
	}

	void web_client::set_role_time(const guid_64& role_guid, const s_time_info& role_time, int64 order_num)
	{
		if (m_all_role_info_map.empty())
		{
			return;
		}
		gm_all_info_map_it allit = m_all_role_info_map.find(order_num);
		if (allit == m_all_role_info_map.end())
		{
			return;
		}
		if (allit->second.info_map.empty())
		{
			return;
		}

		gm_info_map_it it = m_all_role_info_map[order_num].info_map.find(role_guid.server_64);
		if (it == m_all_role_info_map[order_num].info_map.end())
		{
			return;
		}

		it->second.time_info = role_time;
		it->second.time_ok = true;
		data_load_over(order_num);
	}

	void web_client::set_role_money(const guid_64& role_guid, const s_money_info& role_money, int64 order_num)
	{
		if (m_all_role_info_map.empty())
		{
			return;
		}
		gm_all_info_map_it allit = m_all_role_info_map.find(order_num);
		if (allit == m_all_role_info_map.end())
		{
			return;
		}
		if (allit->second.info_map.empty())
		{
			return;
		}
		gm_info_map_it it = m_all_role_info_map[order_num].info_map.find(role_guid.server_64);
		if (it == m_all_role_info_map[order_num].info_map.end())
		{
			return;
		}
		it->second.money_info = role_money;
		it->second.money_ok = true;
		data_load_over(order_num);
	}

	void web_client::set_role_lock(const guid_64& role_guid, int32 is_login, int32 is_send_chat, int64 order_num)
	{
		if (m_all_role_info_map.empty())
		{
			return;
		}
		gm_all_info_map_it allit = m_all_role_info_map.find(order_num);
		if (allit == m_all_role_info_map.end())
		{
			return;
		}
		if (allit->second.info_map.empty())
		{
			return;
		}
		gm_info_map_it it = m_all_role_info_map[order_num].info_map.find(role_guid.server_64);
		if (it == m_all_role_info_map[order_num].info_map.end())
		{
			return;
		}
		it->second.is_login = is_login;
		it->second.is_send_chat = is_send_chat;
		it->second.lock_ok = true;
		data_load_over(order_num);
	}

	void web_client::data_load_over(int64 order_num)
	{
		if (m_all_role_info_map.empty())
		{
			return;
		}
		gm_all_info_map_it allit = m_all_role_info_map.find(order_num);
		if (allit == m_all_role_info_map.end())
		{
			return;
		}
		if (allit->second.info_map.empty())
		{
			return;
		}
		for (gm_info_map_it it = m_all_role_info_map[order_num].info_map.begin(); it != m_all_role_info_map[order_num].info_map.end(); ++it)
		{
			if (it->second.is_send() == false)
			{
				return;
			}
		}

		std::string role_str;
		switch (m_all_role_info_map[order_num].check_type)
		{
		case e_check_type_ch:

			role_str = "{\"code\": 0, \"msg\" : \"find success\", \"roleInfoList\" : [";
			for (gm_info_map_it it = allit->second.info_map.begin(); it != allit->second.info_map.end(); ++it)
			{
				s_gm_info& gm_info = it->second;
				std::string info_str;

				info_str = "{";

				info_str += "\"account\":\"";
				info_str += gm_info.role_info.account;
				info_str += "\",";

				info_str += "\"role_guid\":\"";
				info_str += to_string(gm_info.role_info.role_guid.server_64);
				info_str += "\",";

				info_str += "\"role_name\":\"";
				info_str += gm_info.role_info.role_name;
				info_str += "\",";

				info_str += "\"role_template_id\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_template_id]);
				info_str += "\",";

				info_str += "\"create_time\":\"";
				info_str += to_string(gm_info.role_info.data_i64_ary[e_role_i64_info_create_time]);
				info_str += "\",";

				info_str += "\"login_time_all\":\"";
				info_str += to_string(gm_info.time_info.data_ary[e_time_type_login_time_all]);
				info_str += "\",";

				info_str += "\"exp_level\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_exp_level]);
				info_str += "\",";

				info_str += "\"exp_num\":\"";
				info_str += to_string(gm_info.money_info.cur_data_ary[e_money_type_exp]);
				info_str += "\",";

				info_str += "\"gender\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_gender]);
				info_str += "\",";

				info_str += "\"class_type\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_class_type]);
				info_str += "\",";

				info_str += "\"jewel\":\"";
				info_str += to_string(gm_info.money_info.cur_data_ary[e_money_type_jewel]);
				info_str += "\",";

				info_str += "\"jewel_bind\":\"";
				info_str += to_string(gm_info.money_info.cur_data_ary[e_money_type_jewel_bind]);
				info_str += "\",";

				info_str += "\"treasure_pic\":\"";
				info_str += to_string(gm_info.money_info.cur_data_ary[e_money_type_treasure_pic]);
				info_str += "\",";

				info_str += "\"jewel_all\":\"";
				info_str += to_string(gm_info.money_info.all_data_ary[e_money_type_jewel]);
				info_str += "\",";

				info_str += "\"gs_value\":\"";
				info_str += to_string(gm_info.role_info.get_gs_value());
				info_str += "\",";

				info_str += "\"vip_level\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_vip_level]);
				info_str += "\",";

				info_str += "\"vip_point\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_recharge_num]);
				info_str += "\",";

				info_str += "\"jew_total_buy_num\":\"";
				info_str += to_string(0);
				info_str += "\",";

				info_str += "\"login_time\":\"";
				info_str += to_string(gm_info.time_info.data_ary[e_time_type_login_time]);
				info_str += "\",";

				info_str += "\"logout_time\":\"";
				info_str += to_string(gm_info.time_info.data_ary[e_time_type_logout_time]);
				info_str += "\",";

				client_session* session = client_session_mgr::getInstance().get_session(gm_info.role_info.role_guid);
				if (session)
				{
					info_str += "\"on_line\":\"";
					info_str += to_string(1);
					info_str += "\",";
				}
				else
				{
					info_str += "\"on_line\":\"";
					info_str += to_string(0);
					info_str += "\",";
				}
				info_str += "\"send_chat\":\"";
				info_str += to_string(gm_info.is_send_chat);
				info_str += "\",";

				info_str += "\"login\":\"";
				info_str += to_string(gm_info.is_login);
				info_str += "\"";

				info_str += "},";

				role_str += info_str;
			}
			break;
		case e_check_type_tw:
			role_str = "{\"ret\": 0, \"msg\" : \"ok\", \"data\" : [";
			for (gm_info_map_it it = allit->second.info_map.begin(); it != allit->second.info_map.end(); ++it)
			{
				s_gm_info& gm_info = it->second;
				std::string info_str;
				info_str = "{";

				info_str += "\"roleId\":\"";
				info_str += to_string(gm_info.role_info.role_guid.server_64);
				info_str += "\",";

				info_str += "\"level\":\"";
				info_str += to_string(gm_info.role_info.data_ary[e_role_info_exp_level]);
				info_str += "\",";

				info_str += "\"nick\":\"";
				info_str += gm_info.role_info.role_name;
				info_str += "\"";

				info_str += "},";

				role_str += info_str;
			}
			break;
		default:
			return;
		}
		role_str = role_str.substr(0, role_str.length() - 1);
		role_str = role_str + "]}";

		http_access_mgr::get_instance().repose_client_req(order_num, role_str);
		m_all_role_info_map.erase(order_num);
	}

	void web_client::db_role_info_modified(const guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		send_get_role_info(e_check_type_ch, init_unit::change_i64_to_string(role_guid.server_64).c_str(), e_unit_guid_role_guid, callback_info);
	}

	void web_client::check_gm_modify_role_info(const guid_64& role_guid)
	{
		gm_mod_role_info_map_it ite = m_modify_role_info_sql_map.find(role_guid.server_64);
		if (ite == m_modify_role_info_sql_map.end())
		{
			return;
		}
		send_modify_role_info(role_guid, ite->second, s_daemon_callback_info());
		m_modify_role_info_sql_map.erase(role_guid.server_64);
	}

	void web_client::set_role_items_info(const guid_64& role_guid, const s_item_info* items_info, int32 item_num, int64 order_num)
	{
		if (item_num == 0 || false == role_guid.is_valid())
		{
			std::string error_str = "{\"code\": 1, \"msg\" : \"no role\", \"itemInfoList\" : []}";
			http_access_mgr::get_instance().repose_client_req(order_num, error_str);
			return;
		}

		Json::Value items_list_json;
		for (int32 i = 0; i < item_num; ++i)
		{
			const s_item_info& item_info = items_info[i];

			Json::Value item_json;
			item_json["itemGuid"] = Json::Value(init_unit::change_i64_to_string(item_info.item_guid.server_64));
			item_json["itemId"] = Json::Value(init_unit::change_i64_to_string(item_info.data_ary[e_item_info_info_id]));
			item_json["itemStackCnt"] = Json::Value(init_unit::change_i64_to_string(item_info.data_ary[e_item_info_stack_count]));
			items_list_json[i] = item_json;
		}

		Json::Value ret_json;
		ret_json["playerGuid"] = Json::Value(init_unit::change_i64_to_string(role_guid.server_64));
		ret_json["items"] = items_list_json;
		Json::FastWriter fastWriter;
		std::string ret_json_str = fastWriter.write(ret_json);

		http_access_mgr::get_instance().repose_client_req(order_num, ret_json_str);
	}

	void web_client::send_server_state_to_gm(const server_info_gm& srv_info)
	{
		//faith::web_proto::json_str_ws_to_http msg;
		//msg.set_jsontype(-1);

		//Json::Value server_info_json;
		//server_info_json["serverId"] = world_server::getInstance().get_server_id();
		//server_info_json["serverUserNum"] = Json::Value(srv_info.acc_num);
		//server_info_json["serverRoleNum"] = Json::Value(srv_info.role_num);
		//server_info_json["serverRoleOnlineNum"] = Json::Value(client_session_mgr::getInstance().get_game_num());

		//Json::FastWriter fastWriter;
		//std::string server_info_json_str = fastWriter.write(server_info_json);

		//msg.set_datastr(server_info_json_str);
		//msg.set_biggroupid(world_server::getInstance().get_big_group_id());
		//msg.set_servergroupid(world_server::getInstance().get_server_group_id());
		//msg.set_ordernum(0);
		//serialze_and_send(&msg, faith::e_ws2w_pi_server_state);

		Json::Value server_info_json;
		server_info_json["serverId"] = world_server::getInstance().get_server_id();
		server_info_json["serverUserNum"] = Json::Value(srv_info.acc_num);
		server_info_json["serverRoleNum"] = Json::Value(srv_info.role_num);
		server_info_json["serverRoleOnlineNum"] = Json::Value(client_session_mgr::getInstance().get_game_num());

		Json::FastWriter fastWriter;
		std::string server_info_json_str = fastWriter.write(server_info_json);

		s_client_uid temp_uid;
		temp_uid.fep_uid_64 = world_server::getInstance().get_server_id();
		xstring target_url = SERVERCONFIG->server_state_url;
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type: application/json");
		http_access_mgr::get_instance().async_request
		(
			temp_uid,
			target_url,
			head_list,
			server_info_json_str,
			boost::bind(&web_client::send_server_state_to_gm_end, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}

	void web_client::send_server_state_to_gm_end(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result)
	{
		Json::Value end_info;

		Json::Reader reader;
		reader.parse(http_result, end_info);
	}

	void web_client::send_role_info_to_gm(const s_client_uid client_uid, const s_unit_info& role_info)
	{
		Json::Value server_info_json;
		server_info_json["userCode"] = role_info.account;
		server_info_json["serverId"] = role_info.data_ary[e_role_info_server_id];
		server_info_json["templateId"] = role_info.data_ary[e_role_info_template_id];
		server_info_json["expLevel"] = role_info.data_ary[e_role_info_exp_level];
		server_info_json["roleName"] = role_info.role_name;
		server_info_json["roleGuid"] = role_info.role_guid.server_64;

		Json::FastWriter fastWriter;
		std::string server_info_json_str = fastWriter.write(server_info_json);

		s_client_uid temp_uid;
		temp_uid.fep_uid_64 = world_server::getInstance().get_server_id();
		xstring target_url = SERVERCONFIG->account_info_url;
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type: application/json");
		http_access_mgr::get_instance().async_request
		(
			temp_uid,
			target_url,
			head_list,
			server_info_json_str,
			boost::bind(&web_client::send_role_info_to_to_gm_end, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}

	void web_client::send_role_info_to_to_gm_end(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result)
	{
		Json::Value end_info;

		Json::Reader reader;
		reader.parse(http_result, end_info);
	}

	void web_client::send_order_win(int32 json_type, int64 order_num)
	{
		Json::Value ret_json;
		//ret_json["ok"] = true;
		ret_json["code"] = 0; //0为成功 不用json_type
		ret_json["msg"] = "Success";
		ret_json["serverId"] = world_server::getInstance().get_server_id();
		Json::FastWriter fastWriter;
		std::string ret_json_str = fastWriter.write(ret_json);
		http_access_mgr::get_instance().repose_client_req(order_num, ret_json_str);
	}

	void web_client::send_order_fail(int32 json_type, std::string error_type, int64 order_num)
	{
		Json::Value ret_json;
		//ret_json["ok"] = false;
		ret_json["code"] = json_type; //json_type是从1开始的不会为0
		ret_json["msg"] = error_type;
		ret_json["serverId"] = world_server::getInstance().get_server_id();
		Json::FastWriter fastWriter;
		std::string ret_json_str = fastWriter.write(ret_json);
		http_access_mgr::get_instance().repose_client_req(order_num, ret_json_str);
	}


	void web_client::send_taiwan_order_fail(int32 json_type, std::string error_type, int64 order_num)
	{
		Json::Value ret_json;
		//ret_json["ok"] = false;
		ret_json["ret"] = json_type; //json_type是从1开始的不会为0
		ret_json["msg"] = error_type;
		//ret_json["serverId"] = world_server::getInstance().get_cross_group_id();
		Json::FastWriter fastWriter;
		std::string ret_json_str = fastWriter.write(ret_json);
		http_access_mgr::get_instance().repose_client_req(order_num, ret_json_str);
	}


	void web_client::send_activate_code(client_session* session, std::string activate_code, int32 login_type)
	{
		if (nullptr == session)
		{
			return;
		}

		guid_64 player_guid = session->get_role_guid();
		xstring account_id = session->m_account;
		xstring channel_id = session->m_login_third_data.param7;

		Json::Value activate_code_json;
		activate_code_json["giftCode"] = Json::Value(activate_code);
		activate_code_json["roleId"] = Json::Value(init_unit::change_i64_to_string(player_guid.server_64));
		activate_code_json["userId"] = Json::Value(account_id);
		activate_code_json["serverId"] = Json::Value(world_server::getInstance().get_server_id());
		activate_code_json["channelId"] = Json::Value(channel_id);
		activate_code_json["loginType"] = Json::Value(login_type);

		Json::FastWriter fastWriter;
		std::string activate_code_str = fastWriter.write(activate_code_json);
		xstring sdk_url = SERVERCONFIG->gift_url;
		// 自定义请求头
		xstring url_para = activate_code_str;
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type:application/json");
		head_list.push_back("Content-Length:" + init_unit::change_i64_to_string(url_para.size()));
		head_list.push_back("gameId:" + init_unit::change_i64_to_string(SERVERCONFIG->game_id));
		head_list.push_back("channelId:" + channel_id);
		// 异步请求
		http_access_mgr::get_instance().async_request
		(
			player_guid.server_64,
			sdk_url,
			head_list,
			url_para,
			boost::bind(&web_client::send_activate_code_end, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}
	void web_client::send_activate_code_end(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result)
	{
		CONSOLE_INFO("http_error_info:{}", http_error_info);
		CONSOLE_INFO("http_result:{}", http_result);
		guid_64 role_guid(http_id);

		Json::Reader reader;
		Json::Value  json_value;
		if (http_error_code > 0 || http_result.size() == 0 || false == reader.parse(http_result, json_value))
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (json_value["msg"].isNull())
		{
			if (session)
			{
				xstring msg_str = "error code";
				session->send_notice(msg_str);
			}
			return;
		}
		if (json_value["data"].isNull())
		{
			if (session)
			{
				xstring msg_str = json_value["msg"].asString();
				session->send_notice(msg_str);
			}
			return;
		}
		if (session)
		{
			const Json::Value& jdata_msg = json_value["msg"];
			if (!jdata_msg.isNull()
				&& jdata_msg.isString()
				&& jdata_msg.asString().length() <= 0)
			{
				return;
			}

			xstring msg_str = jdata_msg.asString();
			session->send_notice(msg_str);
		}

		Json::Value jdata_dataobj;
		const Json::Value& jdata_data = json_value["data"];
		Json::ValueType _data_type = jdata_data.type();
		switch (_data_type)
		{
		case Json::stringValue:
		{
			Json::Reader reader;
			reader.parse(jdata_data.asString(), jdata_dataobj);
			break;
		}
		case Json::objectValue:
		{
			jdata_dataobj = jdata_data;
			break;
		}
		default:
			return;
		}

		if (false == jdata_dataobj.isObject())
		{
			return;
		}

		const Json::Value& jdata_data_gift = jdata_dataobj["giftGoods"];
		if (false == jdata_data_gift.isArray())
		{
			return;
		}

		std::vector<s_item_template_info> drop_item_list;
		for (uint32 idx = 0; idx < jdata_data_gift.size(); idx++)
		{
			const Json::Value& goods_id = jdata_data_gift[idx]["goodsCode"];
			const Json::Value& goods_num = jdata_data_gift[idx]["goodsNumber"];
			if (goods_id.isNull() || goods_num.isNull())
			{
				continue;
			}
			int32 item_id = init_unit::change_string_to_i64(goods_id.asString());
			int32 item_num = init_unit::change_string_to_i64(goods_num.asString());
			drop_item_list.push_back({ item_id ,item_num });
		}

		std::string title_text = "";
		std::string content_text = "";
		if (!jdata_dataobj["mailTitle"].isNull() && jdata_dataobj["mailTitle"].isString())
		{
			title_text = jdata_dataobj["mailTitle"].asString();
		}
		else
		{
			std::string title_gift_name_string = "";
			if (!jdata_dataobj["giftName"].isNull() && jdata_dataobj["giftName"].isString())
			{
				title_gift_name_string = jdata_dataobj["giftName"].asString();
			}
			std::vector<std::string> title_params;
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_active_code_gift_title));
			title_params.push_back(title_gift_name_string);
			title_text = init_unit::implode(title_params);
		}

		if (!jdata_dataobj["mailContent"].isNull() && jdata_dataobj["mailContent"].isString())
		{
			content_text = jdata_dataobj["mailContent"].asString();
		}
		else
		{
			content_text = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_active_code_gift_context);
		}
		mail_ws_mgr::get_instance().send_mail_system(role_guid, 0, drop_item_list, title_text, content_text);
	}
	void web_client::parse_json_do_text_command(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		if (json_value["command"].isNull() || json_value["command"].isString() == false || json_value["command"].asString().length() <= 0)
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_textcommand, "parse_json_set_server_game parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "parse_json_set_server_game parameter error", callback_info);
			}
			return;
		}
		xstring command = json_value["command"].asString();
		if (command == "radioHost")
		{
			xchar role_mark[max_account_length + 1];
			memset(role_mark, 0, sizeof(role_mark));
			if (json_value["userId"].isNull() == false && json_value["userId"].asString().length() > 0)
			{
				xstring account = json_value["userId"].asString();
				int32 cpy_num = account.size() > max_account_length ? max_account_length : account.size();
				memcpy(role_mark, account.c_str(), cpy_num);
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "userId parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "userId parameter error", callback_info);
				}
				return;
			}
			if (json_value["radioIndex"].isNull() == false && json_value["radioIndex"].isString() && json_value["radioIndex"].asString().length() > 0)
			{
				int32 radioIndex;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["radioIndex"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> radioIndex;
				send_role_right(role_mark, e_role_right_radio_host, radioIndex, callback_info);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "radioIndex parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "radioIndex parameter error", callback_info);
				}
			}
		}
		else if (command == "cross")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross, IsOpen, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross", callback_info);
				}
			}
		}
		else if (command == "crossPK")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_pk, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross_pk parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_pk parameter error", callback_info);
				}
			}
		}
		else if (command == "crossActvity")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_activity, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross_activity parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_activity parameter error", callback_info);
				}
			}
		}
		else if (command == "crossHarry")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_harry, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross_harry parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_harry parameter error", callback_info);
				}
			}
		}
		else if (command == "crossCityWar")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_city_war, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross_city_war parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_city_war parameter error", callback_info);
				}
			}
		}
		else if (command == "crossWorldBoss")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_boss, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross_boss parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_boss parameter error", callback_info);
				}
			}
		}
		else if (command == "crossLadder")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_ladder, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_cross_ladder parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_boss parameter error", callback_info);
				}
			}
		}
		else if (command == "elementWar")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_element_war, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " is_begin_elementWar parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_elementWar parameter error", callback_info);
				}
			}
		}
		else if (command == "kickOutChief")
		{
			if (json_value["legionGuid"].isNull() == false && json_value["legionGuid"].isString() == true)
			{
				xstring legion_guid_str = json_value["legionGuid"].asString();
				guid_64 legion_guid = init_unit::change_string_to_i64(legion_guid_str);
				send_server_kick_out_legion_chief(legion_guid);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " kickOutChief Error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "kickOutChief Error", callback_info);
				}
			}
		}
		else if (command == "delItem")
		{
			int32 bag_type = 0;
			int32 bag_slot_begin = 0;
			int32 bag_slot_end = 0;
			xstring role_guid_str = "";
			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
			if (json_value["bagType"].isNull() == false && json_value["bagType"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["bagType"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> bag_type;
			}
			if (json_value["bagSlotBegin"].isNull() == false && json_value["bagSlotBegin"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["bagSlotBegin"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> bag_slot_begin;
			}
			if (json_value["bagSlotEnd"].isNull() == false && json_value["bagSlotEnd"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["bagSlotEnd"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> bag_slot_end;
			}
			client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (client_session_ptr && client_session_ptr->m_status == client_session::e_ss_ingame)
			{
				send_del_unit_item(client_session_ptr->m_cs_array_index, bag_type, bag_slot_begin, bag_slot_end);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "unit is offline", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "unit is offline", callback_info);
				}
			}
		}
		else if (command == "changeVip")
		{
			xstring role_guid_str = "";
			int32 change_value = 0;
			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
			if (json_value["changeValue"].isNull() == false && json_value["changeValue"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring value_str = json_value["changeValue"].asString();
				temp_stream.clear();
				temp_stream << value_str;
				temp_stream >> change_value;
			}
			client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (client_session_ptr == nullptr
				|| client_session_ptr->m_status != client_session::e_ss_ingame)
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "unit is offline", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "unit is offline", callback_info);
				}
				return;
			}

			send_change_unit_vip(client_session_ptr->m_cs_array_index, change_value);

			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "stopLogin")
		{
			int32 stop_login = 0;
			if (json_value["isStop"].isNull() == false && json_value["isStop"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isStop"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> stop_login;
				send_stop_login(stop_login, callback_info);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				//else
				//{
				//	gm_order_end(e_http_error_code_no_error, "", order_num);
				//}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "no isStop param", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "no isStop param", callback_info);
				}
			}
		}
		else if (command == "reloadCsv")
		{
			send_server_reload_csv();
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "roleReward")
		{
			xstring mail_title = "";
			xstring mail_Content = "";
			if (!json_value["emailTitle"].isNull())
			{
				mail_title = json_value["emailTitle"].asString();
			}
			if (!json_value["emailContent"].isNull())
			{
				mail_Content = json_value["emailContent"].asString();
			}
			send_mail_by_role_reward(mail_title, mail_Content);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "changeAccount")
		{
			xstring change_name = "";
			xstring target_name = "";
			if (!json_value["changeName"].isNull())
			{
				change_name = json_value["changeName"].asString();
			}
			if (!json_value["targetName"].isNull())
			{
				target_name = json_value["targetName"].asString();
			}
			if (change_name.size() < min_name_size || change_name.size() >= max_name_size)
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "change name size is error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "change name size is error", callback_info);
				}
			}
			if (target_name.size() < min_name_size || target_name.size() >= max_name_size)
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "target name size is error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "target name size is error", callback_info);
				}
			}
			send_change_account(change_name, target_name, callback_info);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			//else
			//{
			//	gm_order_end(e_http_error_code_no_error, "", order_num);
			//}
		}
		else if (command == "delCache")
		{
			xstring role_guid_str = "";
			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
			send_del_role_cache(role_guid);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "delRole")
		{
			xstring role_guid_str = "";
			int32 is_del = 0;
			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			if (json_value["isDel"].isNull() == false && json_value["isDel"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isDel"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> is_del;
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
			send_del_role(role_guid, is_del, callback_info);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			//else
			//{
			//	gm_order_end(e_http_error_code_no_error, "", order_num);
			//}
		}
		else if (command == "createNpc")
		{
			int32 map_id = 0;
			int32 npc_id = 0;
			int32 location_x = 0;
			int32 location_y = 0;
			int32 location_z = 0;

			if (json_value["mapId"].isNull() == false && json_value["mapId"].isString() == true && json_value["mapId"].asString().length() > 0)
			{
				std::stringstream temp_stream;
				xstring map_id_str = json_value["mapId"].asString();
				temp_stream.clear();
				temp_stream << map_id_str;
				temp_stream >> map_id;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "mapId parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "mapId parameter error", callback_info);
				}
				return;
			}
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_id);
			if (map_template_ptr == nullptr)
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "mapId is nullptr", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "mapId is nullptr", callback_info);
				}
				return;
			}
			if (json_value["npcId"].isNull() == false && json_value["npcId"].isString() == true && json_value["npcId"].asString().length() > 0)
			{
				std::stringstream temp_stream;
				xstring npc_id_str = json_value["npcId"].asString();
				temp_stream.clear();
				temp_stream << npc_id_str;
				temp_stream >> npc_id;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "npcId parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "npcId parameter error", callback_info);
				}
				return;
			}
			if (json_value["locationX"].isNull() == false && json_value["locationX"].isString() == true && json_value["locationX"].asString().length() > 0)
			{
				std::stringstream temp_stream;
				xstring location_x_str = json_value["locationX"].asString();
				temp_stream.clear();
				temp_stream << location_x_str;
				temp_stream >> location_x;
				if (location_x < map_template_ptr->MapStartX || location_x > map_template_ptr->MapEndX)
				{
					if (is_send_to_old_gmt)
					{
						send_order_fail(e_web_json_type_textcommand, "locationX parameter error", callback_info.handle_index);
					}
					else
					{
						gm_order_end(e_http_error_code_operate_failed, "locationX parameter error", callback_info);
					}
					return;
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "locationX parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "locationX parameter error", callback_info);
				}
				return;
			}
			if (json_value["locationY"].isNull() == false && json_value["locationY"].isString() == true && json_value["locationY"].asString().length() > 0)
			{
				std::stringstream temp_stream;
				xstring location_y_str = json_value["locationY"].asString();
				temp_stream.clear();
				temp_stream << location_y_str;
				temp_stream >> location_y;
				if (location_y < map_template_ptr->MapStartY || location_y > map_template_ptr->MapEndY)
				{
					if (is_send_to_old_gmt)
					{
						send_order_fail(e_web_json_type_textcommand, "locationY parameter error", callback_info.handle_index);
					}
					else
					{
						gm_order_end(e_http_error_code_json_parse_failed, "locationY parameter error", callback_info);
					}
					return;
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "locationY parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "locationY parameter error", callback_info);
				}
				return;
			}
			if (json_value["locationZ"].isNull() == false && json_value["locationZ"].isString() == true && json_value["locationZ"].asString().length() > 0)
			{
				std::stringstream temp_stream;
				xstring location_z_str = json_value["locationZ"].asString();
				temp_stream.clear();
				temp_stream << location_z_str;
				temp_stream >> location_z;
				if (location_z < 0 || location_z > 10000)
				{
					if (is_send_to_old_gmt)
					{
						send_order_fail(e_web_json_type_textcommand, "locationZ parameter error", callback_info.handle_index);
					}
					else
					{
						gm_order_end(e_http_error_code_operate_failed, "locationZ parameter error", callback_info);
					}
					return;
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "locationZ parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "locationZ parameter error", callback_info);
				}
				return;
			}

			auto map_ent = cs_map_mgr_system::get_map_ws_by_line(map_id, 1);
			if (map_ent == nullptr)
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "map_ent is nullptr", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "map_ent is nullptr", callback_info);
				}
				return;
			}
			ws2cs_spawn_npc	ws2cs_spawn_npc_msg;
			ws2cs_spawn_npc_msg.map_guid = map_ent->getEntityId();
			ws2cs_spawn_npc_msg.npc_template_id = npc_id;
			ws2cs_spawn_npc_msg.pos.unit_location.X = location_x;
			ws2cs_spawn_npc_msg.pos.unit_location.Y = location_y;
			ws2cs_spawn_npc_msg.pos.unit_location.Z = location_z;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_spawn_npc);
			if (is_use == false)
			{
				cs_map_system::send_message_to_cs(map_ent, &ws2cs_spawn_npc_msg, sizeof(ws2cs_spawn_npc_msg));
			}
			else
			{
				faith::ws2cs_proto::spawn_npc pro_msg;
				ws2cs_spawn_npc_msg.to_proto(pro_msg);
				cs_map_system::send_message_to_cs_lua(map_ent, &pro_msg, e_msgindex_ws2cs_spawn_npc);
			}

			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "addGuideRecord")
		{
			int32 guide_trigger_id = 0;
			xstring role_guid_str = "";
			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);

			if (json_value["guideTriggerId"].isNull() == false && json_value["guideTriggerId"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["guideTriggerId"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> guide_trigger_id;
			}

			client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (client_session_ptr && client_session_ptr->m_status == client_session::e_ss_ingame)
			{
				ws2cs_add_guide_record req;
				req.cs_unit_index = client_session_ptr->m_cs_array_index;
				req.guide_trigger_id = guide_trigger_id;

				bool is_use = proto_by_lua(e_msgindex_ws2cs_add_guide_record);
				if (is_use == false)
				{
					client_session_ptr->send_to_cs(&req, sizeof(req));
				}
				else
				{
					faith::ws2cs_proto::add_guide_record msg;
					msg.set_cs_unit_index(client_session_ptr->m_cs_array_index);
					msg.set_guide_trigger_id(guide_trigger_id);
					client_session_ptr->send_to_cs_lua(&msg, e_msgindex_ws2cs_add_guide_record);
				}

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "unit is offline", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "unit is offline", callback_info);
				}
			}
		}
		else if (command == "roleInfo")
		{
			if (m_all_role_info_map.size() >= ROLE_INFO_MAP_MAX_SIZE)
			{
				if (is_send_to_old_gmt)
				{
					send_taiwan_order_fail(1004, "too much request ", callback_info.handle_index);
				}
				else
				{
					gm_order_end(1004, "too much request", callback_info);
				}
				return;
			}
			if (json_value["checkType"].isNull() || json_value["checkType"].isString() == false)
			{
				if (is_send_to_old_gmt)
				{
					send_taiwan_order_fail(1002, "checkType error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(1002, "checkType error", callback_info);
				}
				return;
			}
			int32 checkType = 0;
			std::stringstream temp_stream;
			xstring checkType_str = json_value["checkType"].asString();
			temp_stream.clear();
			temp_stream << checkType_str;
			temp_stream >> checkType;
			if (checkType < 0 || checkType > e_check_type_max)
			{
				if (is_send_to_old_gmt)
				{
					send_taiwan_order_fail(1002, "checkType error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(1002, "checkType error", callback_info);
				}
				return;
			}
			e_unit_guid get_type = e_unit_guid_account;
			xchar role_mark[max_account_length + 1];
			memset(role_mark, 0, sizeof(role_mark));
			if (json_value["userId"].isNull() == false && json_value["userId"].isString() == true && json_value["userId"].asString().length() > 0)
			{
				xstring account = json_value["userId"].asString();
				int32 cpy_num = account.size() > max_account_length ? max_account_length : account.size();
				memcpy(role_mark, account.c_str(), cpy_num);
			}
			else if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true && json_value["roleId"].asString().length() > 0)
			{
				xstring role_guid_str = json_value["roleId"].asString();
				int32 cpy_num = role_guid_str.size() > max_account_length ? max_account_length : role_guid_str.size();
				memcpy(role_mark, role_guid_str.c_str(), cpy_num);
				get_type = e_unit_guid_role_guid;
			}
			else if (json_value["roleName"].isNull() == false && json_value["roleName"].isString() == true && json_value["roleName"].asString().length() > 0)
			{
				xstring role_name = json_value["roleName"].asString();
				int32 cpy_num = role_name.size() > max_account_length ? max_account_length : role_name.size();
				memcpy(role_mark, role_name.c_str(), cpy_num);
				get_type = e_unit_guid_role_name;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_taiwan_order_fail(1003, "parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(1003, "parameter error", callback_info);
				}
				return;
			}
			//m_all_role_info_map[order_num].time = utility::get_tick_count() + 10000;// init_unit::get_end_time(second_tick_time);

			m_all_role_info_map[callback_info.handle_index].time = init_unit::get_end_time(10);
			send_get_role_info(checkType, role_mark, get_type, callback_info);
		}
		else if (command == "modifyMoney")
		{
			xstring role_guid_str = "";
			xstring money_command = "";
			e_money_type money_type = faith::e_money_type_exp;
			int64 money_value = 0;
			bool is_add_money = false;

			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);

			if (json_value["moneyCommand"].isNull() == false && json_value["moneyCommand"].isString() == true)
			{
				money_command = json_value["moneyCommand"].asString();
				if (money_command == "addMoney")
				{
					is_add_money = true;
				}
				else if (money_command == "cutMoney")
				{
					is_add_money = false;
				}
				else
				{
					if (is_send_to_old_gmt)
					{
						send_taiwan_order_fail(e_web_json_type_textcommand, "Please enter the correct operation!", callback_info.handle_index);
					}
					else
					{
						gm_order_end(e_http_error_code_operate_failed, "Please enter the correct operation!", callback_info);
					}
					return;
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "Please enter the correct operation!", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "Please enter the correct operation!", callback_info);
				}
				return;
			}

			if (json_value["moneyType"].isNull() == false && json_value["moneyType"].isString() == true)
			{
				int32 temp_money_type;
				std::stringstream temp_stream;
				xstring money_type_str = json_value["moneyType"].asString();
				temp_stream.clear();
				temp_stream << money_type_str;
				temp_stream >> temp_money_type;
				money_type = (e_money_type)temp_money_type;
			}
			if (json_value["moneyValue"].isNull() == false && json_value["moneyValue"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring money_value_str = json_value["moneyValue"].asString();
				temp_stream.clear();
				temp_stream << money_value_str;
				temp_stream >> money_value;
			}

			client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (client_session_ptr && client_session_ptr->m_status == client_session::e_ss_ingame)
			{
				ws2cs_modify_money req;
				req.cs_unit_index = client_session_ptr->m_cs_array_index;
				req.is_add_money = is_add_money;
				req.money_type = money_type;
				req.money_value = money_value;
				bool is_use = proto_by_lua(e_msgindex_ws2cs_modify_money);
				if (is_use == false)
				{
					client_session_ptr->send_to_cs(&req, sizeof(req));
				}
				else
				{
					faith::ws2cs_proto::modify_money pro_msg;
					req.to_proto(pro_msg);
					client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_modify_money);
				}

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "unit is offline", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "unit is offline", callback_info);
				}
			}

		}
		else if (command == "delSpecifiedItem")
		{
			int32 bag_type = 0;
			int32 item_id = 0;
			xstring role_guid_str = "";
			if (json_value["roleId"].isNull() == false && json_value["roleId"].isString() == true)
			{
				role_guid_str = json_value["roleId"].asString();
			}
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
			if (json_value["bagType"].isNull() == false && json_value["bagType"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["bagType"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> bag_type;
			}
			if (json_value["itemId"].isNull() == false && json_value["itemId"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["itemId"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> item_id;
			}
			client_session* client_session_ptr = client_session_mgr::getInstance().get_session(role_guid);
			if (client_session_ptr && client_session_ptr->m_status == client_session::e_ss_ingame)
			{
				send_del_unit_specified_item(client_session_ptr->m_cs_array_index, bag_type, item_id);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "unit is offline", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_operate_failed, "unit is offline", callback_info);
				}
			}
		}
		else if (command == "crossLegionTerritoryWar")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_cross_begin_cross_legion_territory_war, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "is_begin_cross_pk parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_cross_pk parameter error", callback_info);
				}
			}
		}
		else if (command == "attackCity")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 IsOpen;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> IsOpen;
				send_server_need_begin_cross_with_type(e_need_server_attack_city, IsOpen != 0, callback_info.handle_index);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "is_begin_attack_city parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "is_begin_attack_city parameter error", callback_info);
				}
			}
		}
		else if (command == "crossServerCommon")
		{
			int32 gm_type = 0;
			int32 is_begin_cross = 0;
			if (json_value["crossType"].isNull() == false && json_value["crossType"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["crossType"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> gm_type;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "crossType cant parse", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "crossType cant parse", callback_info);
				}
				return;
			}
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> is_begin_cross;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "isOpen cant parse", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "isOpen cant parse", callback_info);
				}
				return;
			}
			send_server_need_begin_cross_with_type(gm_type, is_begin_cross, callback_info.handle_index);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "pauseActivity")
		{
			if (json_value["isOpen"].isNull() == false && json_value["isOpen"].isString() == true)
			{
				int32 pause;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isOpen"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> pause;
				//time_limit_activity_ws_mgr::get_instance().set_pause_flag(pause != 0);

				if (is_send_to_old_gmt)
				{
					send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_no_error, "", callback_info);
				}
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, " pauseActivity parameter error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "pauseActivity parameter error", callback_info);
				}
			}
		}
		else if (command == "endActivity")
		{
			int32 act_type = 0;
			if (json_value["actType"].isNull() == false && json_value["actType"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["actType"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> act_type;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "acttype is error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "acttype is error", callback_info);
				}
				return;
			}
			time_limit_activity_ws_mgr::get_instance().end_act_with_gm(act_type);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "endCloudShop")
		{
			int32 is_end = 0;
			if (json_value["isEnd"].isNull() == false && json_value["isEnd"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["isEnd"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> is_end;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "endCloudShop is error", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "endCloudShop is error", callback_info);
				}
				return;
			}
			if (is_end > 0)
			{
				cloud_shop_mgr::get_instance().send_rewards_and_clear_info(time_limit_activity_ws_mgr::get_instance().get_can_begin_cloud_shop_type_in_this_server());
			}
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}

		else if (command == "serverState")
		{
			int32 server_info_type = 0;
			int32 server_info_value = 0;
			//if (json_value["stateType"].isNull() == false && json_value["stateType"].isString() == true)
			//{
			//	std::stringstream temp_stream;
			//	xstring radioIndex_str = json_value["stateType"].asString();
			//	temp_stream.clear();
			//	temp_stream << radioIndex_str;
			//	temp_stream >> server_info_type;
			//}
			//else
			//{
			//	if (is_send_to_old_gmt)
			//	{
			//		send_order_fail(e_web_json_type_textcommand, "stateType cant parse", order_num);
			//	}
			//	else
			//	{
			//		gm_order_end(e_http_error_code_json_parse_failed, "stateType cant parse", order_num);
			//	}
			//	return;
			//}
			server_info_type = e_server_info_type_server_state;
			if (json_value["stateValue"].isNull() == false && json_value["stateValue"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["stateValue"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> server_info_value;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "stateValue cant parse", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "stateValue cant parse", callback_info);
				}
				return;
			}
			send_server_info_arr_with_type(server_info_type, server_info_value, callback_info);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			//else
			//{
			//	gm_order_end(e_http_error_code_no_error, "", order_num);
			//}
		}

		else if (command == "crossServerState")
		{
			int32 server_info_value = 0;
			if (json_value["stateValue"].isNull() == false && json_value["stateValue"].isString() == true)
			{
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["stateValue"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> server_info_value;
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "stateValue cant parse", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "stateValue cant parse", callback_info);
				}
				return;
			}
			send_server_info_arr_with_type(e_server_info_type_server_cross_state, server_info_value, callback_info);
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
		}
		else if (command == "deleteActivity")
		{
			if (json_value["actType"].isNull() == false && json_value["actType"].isString() == true)
			{
				int32 time_limit_activity_type;
				std::stringstream temp_stream;
				xstring radioIndex_str = json_value["actType"].asString();
				temp_stream.clear();
				temp_stream << radioIndex_str;
				temp_stream >> time_limit_activity_type;
				time_limit_activity_temp_ws_mgr::get_instance().delete_time_limit_activity_by_type(time_limit_activity_type);
			}
			else
			{
				if (is_send_to_old_gmt)
				{
					send_order_fail(e_web_json_type_textcommand, "stateValue cant parse", callback_info.handle_index);
				}
				else
				{
					gm_order_end(e_http_error_code_json_parse_failed, "stateValue cant parse", callback_info);
				}
				return;
			}
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else if (command == "refreshServerList")
		{
			send_server_end_refresh_server_list();
			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}
		}
		else
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_textcommand, _XTEXT("parameter error : ") + command, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, _XTEXT("parameter error : ") + command, callback_info);
			}
		}
	}


	void web_client::send_role_right(const xchar* role_mark, int32 right_type, int32 right_value, s_daemon_callback_info callback_info, int32 right_time)
	{
		//ws2dp_set_role_right req;
		//memcpy(req.role_mark, role_mark, max_account_length);
		//req.right_type = right_type;
		//req.right_value = right_value;
		//req.right_time = right_time;
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));
		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_set_role_right_lua", 0, false, "%s%d%d%d%d%l", role_mark, right_type, right_value, right_time, callback_info.listen_port, callback_info.handle_index);

	}

	void web_client::send_get_role_info(int32 check_type, const xchar* role_mark, e_unit_guid get_type, s_daemon_callback_info callback_info)
	{
		//ws2dp_gm_get_role_info req;
		//int32 role_mark_len = strlen(role_mark);
		//memcpy(req.role_mark, role_mark, role_mark_len < max_account_length ? role_mark_len : max_account_length);
		//req.get_type = get_type;
		//req.callback_info = callback_info;
		//req.check_type = check_type;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_get_role_info_lua", 0, false, "%s%d%d%l%d", role_mark, (int32)get_type, callback_info.listen_port, callback_info.handle_index, check_type);

	}

	void web_client::send_modify_role_info(const guid_64& role_guid, xstring sql_str, s_daemon_callback_info callback_info)
	{
		if (sql_str.length() > max_gm_sql_str_length)
		{
			return;
		}

		//ws2dp_gm_modify_role_info req;
		//req.role_guid = role_guid;
		//req.callback_info = callback_info;
		//memcpy(req.sql_str, sql_str.c_str(), sql_str.length());
		//
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_modify_role_info_lua", 0, false, "%l%s%d%l", role_guid.server_64, sql_str.c_str(), callback_info.listen_port, callback_info.handle_index);

	}

	void web_client::send_get_role_items_info(const guid_64& role_guid, s_daemon_callback_info callback_info)
	{
		//ws2dp_gm_get_role_items_info req;
		//req.role_guid = role_guid;
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_get_role_items_info_lua", 0, false, "%l%d%l", role_guid.server_64, callback_info.listen_port, callback_info.handle_index);

	}

	void web_client::send_get_server_info_gm()
	{
		ws2dp_query_server_info_gm req;
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}

	void web_client::send_recharge_end_result(int32 result_type, int64 order_num, int32 payment_type)
	{
		/*if (e_payment_method_en_ios == payment_type || e_payment_method_en_android == payment_type)
		{
			recv_recharge_en_end(order_num, result_type);
			return;
		}*/

		Json::Value activate_code_json;
		activate_code_json["code"] = Json::Value(result_type);
		if (result_type == e_recharge_error_none || result_type == 200)
		{
			const std::vector<std::string>& temp_array_list = template_manager::get_instance().get_string_list(90095302);
			if (temp_array_list.size() > 0)
			{
				activate_code_json["msg"] = Json::Value(temp_array_list[0]);
			}
		}
		else
		{
			const std::vector<std::string>& temp_array_list = template_manager::get_instance().get_string_list(90095303);
			if (temp_array_list.size() > 0)
			{
				activate_code_json["msg"] = Json::Value(temp_array_list[0]);
			}
		}
		Json::FastWriter fastWriter;
		std::string activate_code_str = fastWriter.write(activate_code_json);
		http_access_mgr::get_instance().repose_client_req(order_num, activate_code_str);
	}

	void web_client::parse_json_kickout_player(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		xstring user_id_acc = "";
		if (!json_value["userId"].isNull())
		{
			user_id_acc = json_value["userId"].asString();
		}

		if (user_id_acc.empty())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_kickout, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "no user id", callback_info);
			}
			return;
		}

		bool kick_ret = client_session_mgr::getInstance().kickout_account(user_id_acc, e_logout_result_ban_account);
		if (false == kick_ret)
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_kickout, "player not online", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_operate_failed, "player not online", callback_info);
			}
			return;
		}
		//legion_ws_mgr::get_instance().kick_out_chief_with_chief_account(user_id_acc);提玩家不需要从军团长踢出去
		//send_role_right(role_mark, e_role_right_allow_login, operation_int, 99999);

		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_kickout, callback_info.handle_index);
		}
		else
		{
			gm_order_end(e_http_error_code_no_error, "", callback_info);
		}
	}

	void web_client::parse_json_get_role_info(Json::Value& json_value, s_daemon_callback_info callback_info)
	{
		xchar role_mark[max_account_length + 1];
		memset(role_mark, 0, sizeof(role_mark));
		e_unit_guid get_type = e_unit_guid_account;
		if (json_value["userId"].isNull() == false && json_value["userId"].asString().length() > 0)
		{
			xstring account = json_value["userId"].asString();
			int32 cpy_num = account.size() > max_account_length ? max_account_length : account.size();
			memcpy(role_mark, account.c_str(), cpy_num);
		}
		else if (json_value["roleId"].isNull() == false && json_value["roleId"].asString().length() > 0)
		{
			xstring role_guid_str = json_value["roleId"].asString();
			int32 cpy_num = role_guid_str.size() > max_account_length ? max_account_length : role_guid_str.size();
			memcpy(role_mark, role_guid_str.c_str(), cpy_num);
			get_type = e_unit_guid_role_guid;
		}
		else if (json_value["roleName"].isNull() == false && json_value["roleName"].asString().length() > 0)
		{
			xstring role_name = json_value["roleName"].asString();
			int32 cpy_num = role_name.size() > max_account_length ? max_account_length : role_name.size();
			memcpy(role_mark, role_name.c_str(), cpy_num);
			get_type = e_unit_guid_role_name;
		}
		else
		{
			send_order_fail(e_web_json_type_role_info, "parameter error", callback_info.handle_index);
			return;
		}
		send_get_role_info(e_check_type_ch, role_mark, get_type, callback_info);
	}

	void web_client::parse_json_modify_role_info(Json::Value& json_value, xstring sql_table_name, s_daemon_callback_info callback_info)
	{
		if (json_value["roleId"].isNull() || json_value["roleId"].asString().length() <= 0)
		{
			send_order_fail(e_web_json_type_char_info_modify, "parameter error", callback_info.handle_index);
			return;
		}

		if (json_value["modify_sql"].isNull() || json_value["modify_sql"].isObject() == false)
		{
			send_order_fail(e_web_json_type_char_info_modify, "parameter error", callback_info.handle_index);
			return;
		}
		xstring role_guid_str = json_value["roleId"].asString();
		guid_64 role_guid = init_unit::change_string_to_i64(role_guid_str);

		xstring sql_str = json_value["modify_data"].asString();
		if (sql_str.find("drop") != xstring::npos
			|| sql_str.find("DROP") != xstring::npos
			|| sql_str.find("truncate") != xstring::npos
			|| sql_str.find("TRUNCATE") != xstring::npos)
		{
			send_order_fail(e_web_json_type_char_info_modify, "sql parameter error", callback_info.handle_index);
			return;
		}
		if ((sql_str.find("delete") != xstring::npos || sql_str.find("DELETE") != xstring::npos)
			&& (sql_str.find("where") == xstring::npos || sql_str.find("WHERE") != xstring::npos))
		{
			send_order_fail(e_web_json_type_char_info_modify, "sql parameter error", callback_info.handle_index);
			return;
		}

		if (sql_str.length() > max_gm_sql_str_length)
		{
			send_order_fail(e_web_json_type_char_info_modify, "modify too many", callback_info.handle_index);
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (session)
		{
			m_modify_role_info_sql_map[role_guid.server_64] = sql_str;
			client_session_mgr::getInstance().kickout_account(session->m_role_info.account, e_logout_result_ban_account);

			gm_order_end(e_http_error_code_role_online, "role online,do kickout", callback_info);
		}
		else
		{
			send_modify_role_info(role_guid, sql_str, callback_info);
		}
	}
	//void web_client::parse_json_modify_role_info(Json::Value& json_value, xstring sql_table_name)
	//{
	//	if (json_value["roleId"].isNull() || json_value["roleId"].asString().length() <= 0)
	//	{
	//		send_order_fail(e_web_json_type_char_info_modify, "parameter error");
	//		return;
	//	}
	//	
	//	if (json_value["modify_data"].isNull() || json_value["modify_data"].isObject() == false)
	//	{
	//		send_order_fail(e_web_json_type_char_info_modify, "parameter error");
	//		return;
	//	}

	//	xstring role_guid_str = json_value["roleId"].asString();
	//	guid_64 role_guid = init_unit::change_string_to_i64(role_guid_str);

	//	xstring sql_str = "'UPDATE " + sql_table_name + " SET ";
	//	const Json::Value& json_modify_data = json_value["modify_data"];
	//	Json::Value::Members mod_keys = json_modify_data.getMemberNames();        
	//	for (int32 mod_idx = 0; mod_idx < mod_keys.size(); mod_idx++)
	//	{
	//		std::string mod_key = mod_keys[mod_idx];
	//		if (json_modify_data[mod_key].type() != Json::intValue)
	//		{
	//			continue;
	//		}
	//		const Json::Value& mod_val = json_modify_data[mod_key];
	//		if (mod_val.asInt() <= 0)
	//		{
	//			continue;
	//		}
	//		sql_str += mod_key + " = " + mod_val.asString();
	//		if (mod_idx < mod_keys.size() - 1)
	//		{
	//			sql_str += ",";
	//		}
	//	}
	//	sql_str += " WHERE role_guid = " + role_guid_str;
	//	if (sql_table_name == "role_money")
	//	{
	//		sql_str += " AND e_money_save_type = 0";
	//	}
	//	else if (sql_table_name == "role_mission")
	//	{
	//		xstring mission_id_str = json_value["missionId"].asString();
	//		sql_str += " AND emid_id = " + mission_id_str;
	//	}

	//	sql_str += ";'";

	//	if (sql_str.length() > max_gm_sql_str_length)
	//	{
	//		send_order_fail(e_web_json_type_char_info_modify, "modify too many");
	//		return;
	//	}

	//	client_session* session = client_session_mgr::getInstance().get_session(role_guid);
	//	if (session)
	//	{
	//		m_modify_role_info_sql_map[role_guid] = sql_str;
	//		client_session_mgr::getInstance().kickout_account(session->m_role_info.account, e_logout_result_ban_account);
	//	}
	//	else
	//	{
	//		send_modify_role_info(role_guid, sql_str);
	//	}
	//}

	void web_client::parse_json_get_role_items_info(Json::Value& json_value, s_daemon_callback_info callback_info)
	{
		if (json_value["roleId"].isNull() || json_value["roleId"].asString().length() <= 0)
		{
			send_order_fail(e_web_json_type_role_item_info, "parameter error", callback_info.handle_index);
			return;
		}

		xstring role_guid_str = json_value["roleId"].asString();
		guid_64 role_guid = init_unit::change_string_to_i64(role_guid_str);

		send_get_role_items_info(role_guid, callback_info);
	}

	void web_client::parse_json_set_send_chat(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		xchar role_mark[max_account_length + 1];
		memset(role_mark, 0, sizeof(role_mark));

		if (json_value["operation"].isNull())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_send_chat, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "", callback_info);
			}
			return;
		}
		//直接asInt会崩溃
		xstring operation_str = json_value["operation"].asString();
		std::stringstream temp_stream;
		temp_stream << operation_str;
		int32 operation_int;
		temp_stream >> operation_int;

		int32 muteMinute_int;
		if (json_value["muteMinute"].isNull())
		{
			muteMinute_int = 999999999;
		}
		else
		{
			xstring muteMinute_str = json_value["muteMinute"].asString();
			temp_stream.clear();
			temp_stream << muteMinute_str;
			temp_stream >> muteMinute_int;
		}

		if (operation_int == 1)
		{
			operation_int = INT_FAST32_MAX;
		}

		if (json_value["userId"].isNull() == false && json_value["userId"].asString().length() > 0)
		{
			xstring account = json_value["userId"].asString();
			int32 cpy_num = account.size() > max_account_length ? max_account_length : account.size();
			memcpy(role_mark, account.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session(role_mark);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_chat, operation_int, muteMinute_int);
			}

		}
		else if (json_value["roleId"].isNull() == false && json_value["roleId"].asString().length() > 0)
		{
			xstring role_guid_str = json_value["roleId"].asString();
			int64 role_guid = init_unit::change_string_to_i64(role_guid_str);
			int32 cpy_num = role_guid_str.size() > max_account_length ? max_account_length : role_guid_str.size();
			memcpy(role_mark, role_guid_str.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session(role_guid);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_chat, operation_int, muteMinute_int);
			}
		}
		else if (json_value["roleName"].isNull() == false && json_value["roleName"].asString().length() > 0)
		{
			xstring role_name = json_value["roleName"].asString();
			int32 cpy_num = role_name.size() > max_name_size ? max_name_size : role_name.size();
			memcpy(role_mark, role_name.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session_by_role_name(role_mark);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_chat, operation_int, muteMinute_int);
			}
		}
		else
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_send_chat, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "", callback_info);
			}
			return;
		}
		send_role_right(role_mark, e_role_right_allow_chat, operation_int, callback_info, muteMinute_int);


		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_send_chat, callback_info.handle_index);
		}
		//else
		//{
		//	gm_order_end(e_http_error_code_no_error, "", req_http_id);
		//}
	}

	void web_client::parse_json_set_ban_login(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		xchar role_mark[max_account_length + 1];
		memset(role_mark, 0, sizeof(role_mark));

		if (json_value["operation"].isNull())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_ban_login, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "parameter error", callback_info);
			}
			return;
		}

		//直接asInt会崩溃
		xstring operation_str = json_value["operation"].asString();
		std::stringstream temp_stream;
		temp_stream << operation_str;
		int32 operation_int;
		temp_stream >> operation_int;

		int32 muteMinute_int;
		if (json_value["muteMinute"].isNull())
		{
			muteMinute_int = 999999999;
		}
		else
		{
			xstring muteMinute_str = json_value["muteMinute"].asString();
			temp_stream.clear();
			temp_stream << muteMinute_str;
			temp_stream >> muteMinute_int;
		}

		if (json_value["userId"].isNull() == false && json_value["userId"].asString().length() > 0)
		{
			xstring account = json_value["userId"].asString();
			int32 cpy_num = account.size() > max_account_length ? max_account_length : account.size();
			memcpy(role_mark, account.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session(role_mark);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_login, operation_int, muteMinute_int);
				if (operation_int > 0)
				{
					client_session_mgr::getInstance().kickout_account(account, e_logout_result_ban_account);
				}
			}
		}
		else if (json_value["roleId"].isNull() == false && json_value["roleId"].asString().length() > 0)
		{
			xstring role_guid_str = json_value["roleId"].asString();
			int64 role_guid = init_unit::change_string_to_i64(role_guid_str);
			int32 cpy_num = role_guid_str.size() > max_account_length ? max_account_length : role_guid_str.size();
			memcpy(role_mark, role_guid_str.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session(role_guid);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_login, operation_int, muteMinute_int);
				if (operation_int > 0)
				{
					client_session_mgr::getInstance().kickout_account(role_guid, e_logout_result_ban_account);
				}
			}
		}
		else if (json_value["roleName"].isNull() == false && json_value["roleName"].asString().length() > 0)
		{
			xstring role_name = json_value["roleName"].asString();
			int32 cpy_num = role_name.size() > max_name_size ? max_name_size : role_name.size();
			memcpy(role_mark, role_name.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session_by_role_name(role_mark);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_login, operation_int, muteMinute_int);
				if (operation_int > 0)
				{
					client_session_mgr::getInstance().kickout_account(session->get_client_uid(), e_logout_result_ban_account);
				}
			}
		}
		else
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_ban_login, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "parameter error", callback_info);
			}
			return;
		}

		send_role_right(role_mark, e_role_right_allow_login, operation_int, callback_info, muteMinute_int);

		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_ban_login, callback_info.handle_index);
		}
		//else
		//{
		//	gm_order_end(e_http_error_code_no_error, "", order_num);
		//}
	}

	void web_client::get_mail_data(s_mail_info& mail_info, s_item_info* item_array, int32& item_num, const std::vector<int32>& item_list, const xstring& title, const xstring& content, int32 money_type_1, int32 money_num_1, int32 money_type_2, int32 money_num_2)
	{
		item_num = item_num > max_item_per_mail ? max_item_per_mail : item_num;
		mail_info.mail_guid = guid_gen::make_guid();
		int32 title_size = title.size();
		int32 content_size = content.size();
		if (title_size > max_mail_title_length)
		{
			title_size = max_mail_title_length;
		}
		if (content_size > max_text_contents_length)
		{
			content_size = max_text_contents_length;
		}
		memcpy(mail_info.mail_title, title.c_str(), title_size);
		memcpy(mail_info.text_contents, content.c_str(), content_size);
		mail_info.data_ary[EMailInfo_SendTime] = time_helper::get_cur_time_new().second;

		for (uint32 i = 0; i < item_num; ++i)
		{
			if (init_unit::init_item_data(item_array[i], item_list[i * 3], 0, item_list[i * 3 + 1], e_bag_type_mail, 1, item_list[i * 3 + 2]))
			{
				mail_info.data_ary[EMailInfo_ItemGuid11 + i * 2] = item_array[i].item_guid.A;
				mail_info.data_ary[EMailInfo_ItemGuid11 + i * 2 + 1] = item_array[i].item_guid.B;
			}
		}

		mail_info.data_ary[EMailInfo_MoneyTyp1] = money_type_1;
		mail_info.data_ary[EMailInfo_MoneyNum1] = money_num_1;
		mail_info.data_ary[EMailInfo_MoneyTyp2] = money_type_2;
		mail_info.data_ary[EMailInfo_MoneyNum2] = money_num_2;
	}

	void web_client::send_mail_proc(client_session* target_session, s_mail_info& mail_info, s_item_info* item_array, int32& item_num,
		xstring role_guid_str, xstring role_name_str, xstring role_acount_str)
	{
		if ((target_session != nullptr) && (target_session->get_cs_conn_index() >= 0))
		{
			ws2cs_send_mail request;
			request.addressee_guid = target_session->get_role_guid();
			request.item_num = item_num;
			request.mail_info = mail_info;
			int32 temp_item_count = 0;
			for (int32 i = 0; i < max_item_per_mail; ++i)
			{
				if (i >= item_num)
				{
					break;
				}
				if (item_array[i].data_ary[e_item_info_info_id] > 0)
				{
					request.item_list[temp_item_count] = item_array[i];
					temp_item_count++;
				}
			}
			bool is_use = proto_by_lua(e_msgindex_ws2cs_recive_mail);
			if (is_use == false)
			{
				target_session->send_to_cs(&request, sizeof(request));
			}
			else
			{
				faith::ws2cs_proto::send_mail pro_msg;
				request.to_proto(pro_msg);
				target_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_recive_mail);
			}
			//target_session->send_to_cs(&request, sizeof(request));
		}


		ws2dp_add_mail_to_sql msg;
		msg.mail_info = mail_info;
		int32 user_id_size = role_acount_str.length();
		if (user_id_size > max_account_length)
		{
			return;
		}
		int32 user_name_size = role_name_str.length();
		if (user_name_size > max_account_length)
		{
			return;
		}
		else if (user_name_size > 0
			&& user_name_size < min_name_size)
		{
			return;
		}
		if (role_acount_str.length() > 0)
		{
			memcpy(msg.role_mark, role_acount_str.c_str(), role_acount_str.size());
			msg.mark_type = e_unit_guid_account;
		}
		else if (role_guid_str.length() > 0)
		{
			memcpy(msg.role_mark, role_guid_str.c_str(), role_guid_str.size());
			msg.mark_type = e_unit_guid_role_guid;
		}
		else if (role_name_str.length() > 0)
		{
			memcpy(msg.role_mark, role_name_str.c_str(), role_name_str.size());
			msg.mark_type = e_unit_guid_role_name;
		}

		msg.item_num = item_num;
		int32 temp_item_count = 0;
		for (int32 i = 0; i < max_item_per_mail; ++i)
		{
			if (item_array[i].data_ary[e_item_info_info_id] > 0)
			{
				msg.item_list[temp_item_count] = item_array[i];
				temp_item_count++;
			}
		}
		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}
	void  web_client::parse_json_set_send_mail(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		s_item_info final_item_array[max_item_per_mail];
		s_mail_info final_mail_info;

		xstring mail_title = "";
		xstring mail_Content = "";
		xstring param_string = "";
		if (!json_value["emailTitle"].isNull())
		{
			mail_title = json_value["emailTitle"].asString();
		}
		if (!json_value["emailContent"].isNull())
		{
			mail_Content = json_value["emailContent"].asString();
		}
		if (!json_value["supplyId"].isNull())
		{
			param_string = json_value["supplyId"].asString();
		}

		int32 money_type_1 = 0;
		int32 money_num_1 = 0;
		int32 money_type_2 = 0;
		int32 money_num_2 = 0;

		if (!json_value["moneyId_1"].isNull()
			&& !json_value["moneyNum_1"].isNull())
		{
			if (json_value["moneyId_1"].isInt()
				&& json_value["moneyNum_1"].isInt())
			{
				money_type_1 = json_value["moneyId_1"].asInt();
				money_num_1 = json_value["moneyNum_1"].asInt();
			}
		}

		if (!json_value["moneyId_2"].isNull()
			&& !json_value["moneyNum_2"].isNull())
		{
			if (json_value["moneyId_2"].isInt()
				&& json_value["moneyNum_2"].isInt())
			{
				money_type_2 = json_value["moneyId_2"].asInt();
				money_num_2 = json_value["moneyNum_2"].asInt();
			}
		}


		Json::Value item_json_value = json_value["emailGoods"];
		std::vector<int32> item_list;
		int32 item_num = 0;
		if (!item_json_value.isNull()
			&& item_json_value.isArray())
		{
			item_num = item_json_value.size() > max_item_per_mail ? max_item_per_mail : item_json_value.size();
			for (int32 i = 0; i < item_num; ++i)
			{
				Json::Value temp_item = item_json_value[i];
				xstring item_id_string;
				int32 temp_item_num = 0;
				int32 temp_item_lock = 1;
				if (temp_item["goodsCode"].isString())
				{
					item_id_string = temp_item["goodsCode"].asString();

				}
				if (temp_item["goodsNumber"].isInt())
				{
					temp_item_num = temp_item["goodsNumber"].asInt();
				}

				if (temp_item["goodsBinding"].isInt())
				{
					temp_item_lock = temp_item["goodsBinding"].asInt();
				}

				std::stringstream temp_stream;
				temp_stream << item_id_string;
				int32 temp_item_id = 0;
				temp_stream >> temp_item_id;
				item_list.push_back(temp_item_id);
				item_list.push_back(temp_item_num);
				item_list.push_back(temp_item_lock);
			}
		}
		get_mail_data(final_mail_info, final_item_array, item_num, item_list, mail_title, mail_Content, money_type_1, money_num_1, money_type_2, money_num_2);
		for (int32 i = 0; i < max_item_per_mail; ++i)
		{
			if (final_item_array[i].data_ary[e_item_info_info_id] <= 0)
			{
				continue;
			}
			int32 str_len = param_string.length();
			if (str_len > max_recharge_order_length)
			{
				str_len = max_recharge_order_length;
			}
			memcpy(final_item_array[i].param_str, param_string.c_str(), str_len);
		}

		int32 valid_sec = 0;
		const Json::Value& valid_sec_json = json_value["emailExpires"];
		if (valid_sec_json.isInt64())
		{
			valid_sec = valid_sec_json.asInt64() / 1000;
		}
		final_mail_info.data_ary[EMailInfo_IsNeedDelete] = time_helper::get_cur_time_new().second + valid_sec;

		xstring user_id = "";
		//if (!json_value["userId"].isNull() && json_value["userId"].asString().length())
		//{
		//	user_id = json_value["userId"].asString();
		//}
		xstring role_guid_str = "";
		if (!json_value["roleId"].isNull())
		{
			role_guid_str = json_value["roleId"].asString();
		}
		guid_64 role_guid;
		role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
		xstring role_name = "";
		if (!json_value["roleName"].isNull())
		{
			role_name = json_value["roleName"].asString();
		}

		if (role_guid.is_valid())
		{
			client_session* temp_session = client_session_mgr::getInstance().get_session(role_guid);
			send_mail_proc(temp_session, final_mail_info, final_item_array, item_num,
				role_guid_str, role_name, user_id);
		}
		else
		{
			event_ws_mgr::get_instance().add_globel_mail(final_mail_info, final_item_array, item_num);
		}

		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_send_mail, callback_info.handle_index);
		}
		else
		{
			gm_order_end(e_http_error_code_no_error, "", callback_info);
		}
	}

	void web_client::parse_json_set_send_delay_marquee(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		if (false == json_value["marqueeId"].isInt64() || false == json_value["enable"].isBool())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_marquee, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "", callback_info);
			}
			return;
		}

		if (false == json_value["enable"].asBool())
		{
			//删除
			event_ws_mgr::get_instance().del_delay_globle_notice(json_value["marqueeId"].asInt64());

			if (is_send_to_old_gmt)
			{
				send_order_win(e_web_json_type_marquee, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_no_error, "", callback_info);
			}

			return;
		}

		if (json_value["marqueeContent"].isNull() || json_value["marqueeContent"].asString().length() <= 0
			|| false == json_value["startDate"].isInt64() || false == json_value["endDate"].isInt64()
			|| false == json_value["marqueeInterval"].isInt64())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_marquee, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "", callback_info);
			}

			return;
		}
		int64 startStamp = json_value["startDate"].asInt64();
		int64 endStamp = json_value["endDate"].asInt64();
		int64 marqueeInterval = json_value["marqueeInterval"].asInt64();
		if (marqueeInterval < 20000)
		{
			marqueeInterval = 20000;
		}
		xstring marquee_content = json_value["marqueeContent"].asString();
		event_ws_mgr::get_instance().add_delay_globle_notice(json_value["marqueeId"].asInt64(), marquee_content, startStamp, endStamp, marqueeInterval);

		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_marquee, callback_info.handle_index);
		}
		else
		{
			gm_order_end(e_http_error_code_no_error, "", callback_info);
		}
	}

	void web_client::parse_json_activate_code(Json::Value& json_value)
	{
		const Json::Value& jdata_roleId = json_value["role_guid"];
		if (jdata_roleId.isNull() || jdata_roleId.asString().length() <= 0)
		{
			return;
		}

		int64 guid_srv64 = init_unit::change_string_to_i64(jdata_roleId.asString());
		guid_64 role_guid(guid_srv64);

		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (session)
		{
			const Json::Value& jdata_msg = json_value["msg"];
			if (!jdata_msg.isNull()
				&& jdata_msg.isString()
				&& jdata_msg.asString().length() <= 0)
			{
				return;
			}

			xstring msg_str = jdata_msg.asString();
			session->send_notice(msg_str);
		}

		Json::Value jdata_dataobj;
		const Json::Value& jdata_data = json_value["data"];
		Json::ValueType _data_type = jdata_data.type();
		switch (_data_type)
		{
		case Json::stringValue:
		{
			Json::Reader reader;
			reader.parse(jdata_data.asString(), jdata_dataobj);
			break;
		}
		case Json::objectValue:
		{
			jdata_dataobj = jdata_data;
			break;
		}
		default:
			return;
		}

		if (false == jdata_dataobj.isObject())
		{
			return;
		}

		const Json::Value& jdata_data_gift = jdata_dataobj["giftGoods"];
		if (false == jdata_data_gift.isArray())
		{
			return;
		}

		std::vector<s_item_template_info> drop_item_list;
		for (uint32 idx = 0; idx < jdata_data_gift.size(); idx++)
		{
			const Json::Value& goods_id = jdata_data_gift[idx]["goodsCode"];
			const Json::Value& goods_num = jdata_data_gift[idx]["goodsNumber"];
			if (goods_id.isNull() || goods_num.isNull())
			{
				continue;
			}
			int32 item_id = init_unit::change_string_to_i64(goods_id.asString());
			int32 item_num = init_unit::change_string_to_i64(goods_num.asString());
			drop_item_list.push_back({ item_id , item_num });
		}

		std::string title_text = "";
		std::string content_text = "";
		if (!jdata_dataobj["mailTitle"].isNull() && jdata_dataobj["mailTitle"].isString())
		{
			title_text = jdata_dataobj["mailTitle"].asString();
		}
		else
		{
			std::string title_gift_name_string = "";
			if (!jdata_dataobj["giftName"].isNull() && jdata_dataobj["giftName"].isString())
			{
				title_gift_name_string = jdata_dataobj["giftName"].asString();
			}
			std::vector<std::string> title_params;
			title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_active_code_gift_title));
			title_params.push_back(title_gift_name_string);
			title_text = init_unit::implode(title_params);
		}

		if (!jdata_dataobj["mailContent"].isNull() && jdata_dataobj["mailContent"].isString())
		{
			content_text = jdata_dataobj["mailContent"].asString();
		}
		else
		{
			content_text = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_active_code_gift_context);
		}
		mail_ws_mgr::get_instance().send_mail_system(role_guid, 0, drop_item_list, title_text, content_text);
	}

	void web_client::parse_json_recharge(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		const Json::Value& jdata_orderId = json_value["billingOrderId"];
		const Json::Value& jdata_roleId = json_value["roleId"];
		const Json::Value& jdata_gameGoodsId = json_value["gameGoodsId"];
		const Json::Value& jdata_payPrice = json_value["payPrice"];
		if (jdata_orderId.isNull() || jdata_orderId.asString().length() <= 0)
		{
			if (is_send_to_old_gmt)
			{
				send_recharge_end_result(e_recharge_error_begin_order_id_is_null, callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "id is null", callback_info);
			}
			return;
		}
		int64 role_id_srv64 = init_unit::change_string_to_i64(jdata_roleId.asString());
		guid_64 role_guid(role_id_srv64);
		int32 game_goods_id = init_unit::change_string_to_i32(jdata_gameGoodsId.asString());
		float pay_price = jdata_payPrice.asFloat();
		int32 direct_diamond = 0;
		if (!json_value["gameCurrency"].isNull())
		{
			direct_diamond = init_unit::change_string_to_i32(json_value["gameCurrency"].asString());
		}
		xstring order_id_str = jdata_orderId.asString();
		recv_recharge(order_id_str, role_guid, game_goods_id, pay_price, direct_diamond, callback_info);
	}
	void web_client::recv_recharge(xstring& order_id_str, guid_64& role_guid, int32& game_goods_id, float& pay_price, int32& direct_diamond, s_daemon_callback_info& callback_info)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		login_fixed_data temp_third_data;
		int32 login_type = e_login_type_new_account_and_password;
		s_unit_info* role_info_ptr = nullptr;
		if (session)
		{
			temp_third_data = session->m_login_third_data;
			login_type = session->m_login_type;
			role_info_ptr = &session->m_role_info;
		}
		server_log::recharge_step_log(order_id_str, role_guid, game_goods_id, e_recharge_step_begin, temp_third_data, login_type, role_info_ptr);

		xchar order_id[max_recharge_order_length + 1];
		memset(order_id, 0, sizeof(order_id));
		int32 cpy_num = order_id_str.size() > max_recharge_order_length ? max_recharge_order_length : order_id_str.size();
		memcpy(order_id, order_id_str.c_str(), cpy_num);

		int32 payment_type = -1;
		{
			RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, game_goods_id);
			if (recharge_template_ptr == nullptr)
			{
				if (direct_diamond <= 0)
				{
					recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_recharge_by_template_id_recharge_template_ptr_is_null, callback_info.handle_index, e_payment_method_test, role_guid, game_goods_id, order_id);
					return;
				}
				else
				{
					payment_type = e_payment_method_tw_third;
				}
			}
			else
			{
				payment_type = recharge_template_ptr->PaymentType;
			}
		}

		recharge_mgr_ws::get_instance().check_and_save_recharge(order_id, role_guid, game_goods_id, pay_price, callback_info.handle_index, payment_type, faith::s_client_uid(), "", 0, direct_diamond);
	}
	void web_client::parse_json_robot_op(Json::Value& json_value, int64 order_num)
	{
		const Json::Value& jdata_op_type = json_value["op_type"];
		if (jdata_op_type.isNull() || jdata_op_type.asString().length() <= 0)
		{
			return;
		}

		if (jdata_op_type.asString() == "chat_close")
		{
		}
	}

	void web_client::send_server_kick_out_legion_chief(guid_64 legion_guid)
	{
		legion_ws_mgr::get_instance().kick_out_chief_with_legion_guid(legion_guid);
	}

	void web_client::send_server_need_begin_cross_with_type(int32 gm_type, int32 need_begin, int64 req_http_id)
	{
		if (gm_type <= e_need_server_cross_none || gm_type >= e_need_server_cross_max)
		{
			return;
		}
		if (gm_type == e_need_server_cross_begin_cross)
		{
			world_server::getInstance().func_when_change_gate_server(need_begin);
		}
		world_server::getInstance().set_need_begin_cross_gm_common(need_begin, (e_need_server_cross)gm_type);
		world_server::getInstance().send_need_server_cross_to_other_server();
		world_server::getInstance().save_gm_common_to_db(req_http_id);
		world_server::getInstance().send_begin_cross_gm_to_client((e_need_server_cross)gm_type);
		world_server::getInstance().send_begin_cross_gm_to_cs((e_need_server_cross)gm_type);
	}

	void web_client::send_del_unit_item(int32 cs_unit_index, int32 bag_type, int32 bag_slot_begin, int32 bag_slot_end)
	{
		ws2cs_del_unit_item req;
		req.cs_unit_index = cs_unit_index;
		req.bag_type = bag_type;
		req.bag_slot_begin = bag_slot_begin;
		req.bag_slot_end = bag_slot_end;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_del_unit_item);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&req, sizeof(req), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::del_unit_item msg;
			msg.set_cs_unit_index(cs_unit_index);
			msg.set_bag_type(bag_type);
			msg.set_bag_slot_begin(bag_slot_begin);
			msg.set_bag_slot_end(bag_slot_end);

			world_server::getInstance().broadcast_lua(&msg, e_msg_index_ws2cs_del_unit_item, e_server_type_cs);
		}
	}

	void web_client::send_change_unit_vip(int32 cs_unit_index, int32 vip_value)
	{
		ws2cs_change_unit_vip req;
		req.cs_unit_index = cs_unit_index;
		req.vip_value = vip_value;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_change_unit_vip);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&req, sizeof(req), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::change_unit_vip msg;
			msg.set_cs_unit_index(cs_unit_index);
			msg.set_vip_value(vip_value);
			world_server::getInstance().broadcast_lua(&msg, e_msg_index_ws2cs_change_unit_vip, e_server_type_cs);
		}
	}

	void web_client::send_server_info_arr_with_type(int32 server_info_type, int32 server_info_value, s_daemon_callback_info callback_info)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			CONSOLE_INFO("send_server_info_arr_with_type error: gm type begin_cross_server_time");
			return;//禁止GM指令修改开始跨服时间
		}
		world_server::getInstance().set_server_info_arr(server_info_value, server_info_type);
		world_server::getInstance().save_server_info_to_db(server_info_type, callback_info);
		world_server::getInstance().send_server_info_arr_to_cs(server_info_type);
		if (e_server_info_type_server_cross_state == server_info_type && world_server::getInstance().is_sky_island_server())
		{
			world_server::getInstance().send_server_info_by_type_to_other_server(e_server_info_type_server_cross_state);
		}
	}

	void web_client::send_del_unit_specified_item(int32 cs_unit_index, int32 bag_type, int32 item_id)
	{
		ws2cs_del_unit_specified_item req;
		req.cs_unit_index = cs_unit_index;
		req.bag_type = bag_type;
		req.item_id = item_id;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_del_unit_specified_item);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&req, sizeof(req), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::del_unit_specified_item msg;
			msg.set_cs_unit_index(cs_unit_index);
			msg.set_bag_type(bag_type);
			msg.set_item_id(item_id);
			world_server::getInstance().broadcast_lua(&msg, e_msg_index_ws2cs_del_unit_specified_item, e_server_type_cs);
		}
	}
	void web_client::send_stop_login(int32 stop_login, s_daemon_callback_info callback_info)
	{
		//ws2dp_gm_stop_login req;
		//req.is_stop = stop_login;
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_stop_login_lua", 0, false, "%d%d%l", stop_login, callback_info.listen_port, callback_info.handle_index);
	}
	void web_client::send_server_reload_csv()
	{
		world_server::getInstance().reload_csv_func();
	}

	void web_client::send_server_end_refresh_server_list()
	{
		world_server::getInstance().end_refresh_server_list();
	}

	void web_client::send_mail_by_role_reward(const xstring& title, const xstring& content)
	{
		template_manager::template_type* role_reward_template_table = template_manager::get_instance().get_templates(e_RoleRewardTemplate);
		if (nullptr == role_reward_template_table)
		{
			return;
		}

		for (auto ite = role_reward_template_table->begin(); ite != role_reward_template_table->end(); ++ite)
		{
			RoleRewardTemplate* role_reward_template_ptr = (RoleRewardTemplate*)(ite->second);
			if (nullptr == role_reward_template_ptr || role_reward_template_ptr->ServerId != world_server::getInstance().get_server_id())
			{
				continue;
			}
			int32 mail_item_num = role_reward_template_ptr->MailItem.size() / 3;
			int32 item_num = mail_item_num > max_item_per_mail ? max_item_per_mail : mail_item_num;
			s_item_info final_item_array[max_item_per_mail];
			s_mail_info final_mail_info;
			get_mail_data(final_mail_info, final_item_array, item_num, role_reward_template_ptr->MailItem, title, content, 0, 0, 0, 0);
			final_mail_info.data_ary[EMailInfo_IsNeedDelete] = time_helper::get_cur_time_new().second + 15 * day_time_second;
			guid_64 role_guid;
			role_guid.server_64 = init_unit::change_string_to_i64(role_reward_template_ptr->RoleId);
			if (role_guid.is_valid())
			{
				client_session* temp_session = client_session_mgr::getInstance().get_session(role_guid);
				send_mail_proc(temp_session, final_mail_info, final_item_array, item_num,
					role_reward_template_ptr->RoleId, "", "");
			}
			else if (role_reward_template_ptr->RoleId.size() <= 1)
			{
				event_ws_mgr::get_instance().add_globel_mail(final_mail_info, final_item_array, item_num);
			}
		}
		template_manager::get_instance().remove_template(e_RoleRewardTemplate);
	}

	void web_client::send_del_role_cache(guid_64 role_guid)
	{
		ws2dp_gm_del_cache req;
		req.role_guid = role_guid;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));
		bool is_use = proto_by_lua(e_msgindex_ws2dp_gm_del_cache);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&req, sizeof(req), e_server_type_cs);
		}
		else
		{
			faith::ws2cs_proto::gm_del_cache msg;
			msg.set_role_guid(role_guid.server_64);
			world_server::getInstance().broadcast_lua(&msg, e_msgindex_ws2dp_gm_del_cache, e_server_type_cs);
		}

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_del_role_cache_lua", 0, false, "%l", role_guid.server_64);

	}
	void web_client::send_del_role(guid_64 role_guid, int32 is_del, s_daemon_callback_info callback_info)
	{
		//ws2dp_gm_del_role req;
		//req.role_guid = role_guid;
		//req.is_del = is_del;
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_del_role_lua", 0, false, "%l%d%d%l", role_guid.server_64, is_del, callback_info.listen_port, callback_info.handle_index);

	}
	void web_client::send_change_account(const xstring& change_name, const xstring& target_name, s_daemon_callback_info callback_info)
	{
		//ws2dp_gm_change_account req;
		//memcpy(req.change_name, change_name.c_str(), change_name.size());
		//memcpy(req.target_name, target_name.c_str(), target_name.size());
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));
		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_change_account_lua", 0, false, "%s%s%d%l", change_name.c_str(), target_name.c_str(), callback_info.listen_port, callback_info.handle_index);

	}
	void web_client::send_change_account_end(int32 change_res, s_daemon_callback_info callback_info)
	{
		if (change_res == 0)
		{
			send_order_win(e_web_json_type_textcommand, callback_info.handle_index);
			gm_order_end(e_http_error_code_no_error, "", callback_info);
		}
		else
		{
			xstring res = "error code : " + change_res;
			send_order_fail(e_web_json_type_textcommand, res, callback_info.handle_index);
		}
	}
	void web_client::parse_json_open_time(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		const Json::Value& jdata_open_time = json_value["openDate"];
		if (jdata_open_time.isNull() || false == jdata_open_time.isInt64())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_open_time, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "parameter error", callback_info);
			}
			return;
		}
		int64 open_time = jdata_open_time.asInt64() / 1000;
		if (open_time < time_helper::get_time())
		{
			open_time = time_helper::get_time();
		}
		world_server::getInstance().set_begin_time(open_time);
		//ws2dp_gm_open_time req;
		//req.open_time = open_time;
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_open_time_lua", 0, false, "%d%d%l", open_time, callback_info.listen_port, callback_info.handle_index);

		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_open_time, callback_info.handle_index);
		}
		//else
		//{
		//	gm_order_end(e_http_error_code_no_error, "", req_http_id);
		//}
	}

	void web_client::parse_json_change_password(Json::Value& json_value, s_daemon_callback_info callback_info, bool is_send_to_old_gmt)
	{
		const Json::Value& jdata_account = json_value["userId"];
		const Json::Value& jdata_new_password = json_value["newPassword"];
		if (jdata_account.isNull()
			|| jdata_new_password.isNull()
			|| false == jdata_account.isString()
			|| false == jdata_new_password.isString())
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_new_password, "parameter error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_json_parse_failed, "parameter error", callback_info);
			}
			return;
		}
		std::string account = jdata_account.asString();
		std::string new_password = jdata_new_password.asString();
		if (account.length() < 0
			|| account.length() > max_account_length)
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_new_password, "account size error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_operate_failed, "account size error", callback_info);
			}
			return;
		}

		if (new_password.length() < 0
			|| new_password.length() > max_password_length)
		{
			if (is_send_to_old_gmt)
			{
				send_order_fail(e_web_json_type_new_password, "new password size error", callback_info.handle_index);
			}
			else
			{
				gm_order_end(e_http_error_code_operate_failed, "new password size error", callback_info);
			}
			return;
		}

		//ws2dp_gm_change_password req;
		//memcpy(req.account, account.c_str(), account.length());
		//memcpy(req.new_password, new_password.c_str(), new_password.length());
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_set_new_password_lua", 0, false, "%s%s%d%d", account.c_str(), new_password.c_str(), callback_info.listen_port, callback_info.handle_index);


		if (is_send_to_old_gmt)
		{
			send_order_win(e_web_json_type_new_password, callback_info.handle_index);
		}
		//else
		//{
		//	gm_order_end(e_http_error_code_no_error, "", order_num);
		//}
	}

	void web_client::parse_json_set_send_mail_new(Json::Value& json_value, s_daemon_callback_info callback_info)
	{
		s_item_info final_item_array[max_item_per_mail];
		s_mail_info final_mail_info;

		xstring mail_title = "";
		xstring mail_Content = "";
		xstring param_string = "";
		if (!json_value["mailTitle"].isNull())
		{
			mail_title = json_value["mailTitle"].asString();
		}
		if (!json_value["mailContent"].isNull())
		{
			mail_Content = json_value["mailContent"].asString();
		}
		if (!json_value["supplyId"].isNull())
		{
			param_string = json_value["supplyId"].asString();
		}

		int32 money_type_1 = 0;
		int32 money_num_1 = 0;
		int32 money_type_2 = 0;
		int32 money_num_2 = 0;

		if (!json_value["moneyId_1"].isNull()
			&& !json_value["moneyNum_1"].isNull())
		{
			if (json_value["moneyId_1"].isInt()
				&& json_value["moneyNum_1"].isInt())
			{
				money_type_1 = json_value["moneyId_1"].asInt();
				money_num_1 = json_value["moneyNum_1"].asInt();
			}
		}

		if (!json_value["moneyId_2"].isNull()
			&& !json_value["moneyNum_2"].isNull())
		{
			if (json_value["moneyId_2"].isInt()
				&& json_value["moneyNum_2"].isInt())
			{
				money_type_2 = json_value["moneyId_2"].asInt();
				money_num_2 = json_value["moneyNum_2"].asInt();
			}
		}


		Json::Value item_json_value = json_value["goodsJson"];
		std::vector<int32> item_list;
		int32 item_num = 0;
		if (!item_json_value.isNull()
			&& item_json_value.isArray())
		{
			item_num = item_json_value.size() > max_item_per_mail ? max_item_per_mail : item_json_value.size();
			for (int32 i = 0; i < item_num; ++i)
			{
				Json::Value temp_item = item_json_value[i];
				xstring item_id_string;
				int32 temp_item_num = 0;
				int32 temp_item_lock = 1;
				if (temp_item["goodsCode"].isString())
				{
					item_id_string = temp_item["goodsCode"].asString();

				}
				if (temp_item["goodsNumber"].isInt())
				{
					temp_item_num = temp_item["goodsNumber"].asInt();
				}

				if (temp_item["goodsBinding"].isInt())
				{
					temp_item_lock = temp_item["goodsBinding"].asInt();
				}

				std::stringstream temp_stream;
				temp_stream << item_id_string;
				int32 temp_item_id = 0;
				temp_stream >> temp_item_id;
				item_list.push_back(temp_item_id);
				item_list.push_back(temp_item_num);
				item_list.push_back(temp_item_lock);
			}
		}
		get_mail_data(final_mail_info, final_item_array, item_num, item_list, mail_title, mail_Content, money_type_1, money_num_1, money_type_2, money_num_2);
		for (int32 i = 0; i < max_item_per_mail; ++i)
		{
			if (final_item_array[i].data_ary[e_item_info_info_id] <= 0)
			{
				continue;
			}
			int32 str_len = param_string.length();
			if (str_len > max_recharge_order_length)
			{
				str_len = max_recharge_order_length;
			}
			memcpy(final_item_array[i].param_str, param_string.c_str(), str_len);
		}

		int32 valid_sec = 0;
		const Json::Value& valid_sec_json = json_value["mailExpire"];
		if (valid_sec_json.isInt64())
		{
			valid_sec = valid_sec_json.asInt64() / 1000;
		}
		final_mail_info.data_ary[EMailInfo_IsNeedDelete] = time_helper::get_cur_time_new().second + valid_sec;

		xstring user_id = "";
		xstring role_guid_str = "";
		if (!json_value["roleId"].isNull())
		{
			role_guid_str = json_value["roleId"].asString();
		}
		guid_64 role_guid;
		role_guid.server_64 = init_unit::change_string_to_i64(role_guid_str);
		xstring role_name = "";
		if (!json_value["roleName"].isNull())
		{
			role_name = json_value["roleName"].asString();
		}

		if (role_guid.is_valid())
		{
			client_session* temp_session = client_session_mgr::getInstance().get_session(role_guid);
			send_mail_proc(temp_session, final_mail_info, final_item_array, item_num,
				role_guid_str, role_name, user_id);
		}
		else
		{
			event_ws_mgr::get_instance().add_globel_mail(final_mail_info, final_item_array, item_num);
		}
		gm_order_end_new(e_http_error_code_no_error, "succeed", callback_info);
	}

	void web_client::parse_json_set_send_chat_new(Json::Value& json_value, s_daemon_callback_info callback_info)
	{
		if (json_value["chatDate"].isNull())
		{
			gm_order_end_new(e_http_error_code_json_parse_failed, "parameter error", callback_info);
			return;
		}

		xchar role_mark[max_account_length + 1];
		memset(role_mark, 0, sizeof(role_mark));

		//直接asInt会崩溃
		xstring operation_str = json_value["chatDate"].asString();
		std::stringstream temp_stream;
		temp_stream << operation_str;
		int64 operation_int;
		temp_stream >> operation_int;

		int32 is_operation = 0;
		if (operation_int > 0)
		{
			operation_int /= 1000;
			is_operation = 1;
		}
		else
		{
			operation_int = 0;
		}
		if (json_value["userId"].isNull() == false && json_value["userId"].asString().length() > 0)
		{
			xstring account = json_value["userId"].asString();
			int32 cpy_num = account.size() > max_account_length ? max_account_length : account.size();
			memcpy(role_mark, account.c_str(), cpy_num);
			client_session* session = client_session_mgr::getInstance().get_session(role_mark);
			if (session && session->get_cs_conn_index() >= 0)
			{
				session->send_role_right(e_role_right_allow_chat, operation_int, is_operation);
			}

		}
		send_role_right(role_mark, e_role_right_allow_chat, is_operation, callback_info, operation_int);
		gm_order_end_new(e_http_error_code_no_error, "succeed", callback_info);
	}

	void web_client::parse_json_kickout_player_new(Json::Value& json_value, s_daemon_callback_info callback_info)
	{
		xstring user_id_acc = "";
		if (!json_value["userId"].isNull())
		{
			user_id_acc = json_value["userId"].asString();
		}

		if (user_id_acc.empty())
		{
			gm_order_end_new(e_http_error_code_json_parse_failed, "no user id", callback_info);
			return;
		}

		bool kick_ret = client_session_mgr::getInstance().kickout_account(user_id_acc, e_logout_result_ban_account);
		if (false == kick_ret)
		{
			gm_order_end_new(e_http_error_code_no_error, "fail", callback_info);
			return;
		}
		gm_order_end_new(e_http_error_code_no_error, "succeed", callback_info);
	}

	void web_client::handler_http_on_en_recharge_end(const s_http_receive_info& receive_info)
	{
		s_daemon_callback_info callback_info;
		callback_info.listen_port = receive_info.m_req_listen_port;
		callback_info.handle_index = receive_info.m_req_handle_index;
		Json::Value json_value;
		Json::Reader json_reader;
		Json::FastWriter json_writer;
		if (json_reader.parse(receive_info.m_req_body, json_value))
		{//en
			if (json_value["product_id"].isNull() || json_value["product_id"].empty() || !json_value["product_id"].isString()
				|| json_value["buy_type"].isNull() || json_value["buy_type"].empty() || !json_value["buy_type"].isInt()
				|| json_value["order_id"].isNull() || json_value["order_id"].empty() || !json_value["order_id"].isString()
				|| json_value["pay_type"].isNull() || json_value["pay_type"].empty() || !json_value["pay_type"].isString()
				|| json_value["order_id"].isNull() || json_value["order_id"].empty() || !json_value["order_id"].isString()
				|| json_value["game_id"].isNull() || json_value["game_id"].empty() || !json_value["game_id"].isString()
				|| json_value["server_id"].isNull() || json_value["server_id"].empty() || !json_value["server_id"].isString()
				|| json_value["currency"].isNull() || json_value["currency"].empty() || !json_value["currency"].isString()
				|| json_value["pay_channel"].isNull() || json_value["pay_channel"].empty() || !json_value["pay_channel"].isString()
				|| json_value["extra_gift"].isNull() || json_value["extra_gift"].empty() || !json_value["extra_gift"].isString()
				|| json_value["game_extension"].isNull() || json_value["game_extension"].empty() || !json_value["game_extension"].isString())
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_failed);
				return;
			}
			if (json_value["user_id"].isNull() || json_value["user_id"].empty() || !json_value["user_id"].isString())
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_user_error);
				return;
			}
			if (json_value["sign"].isNull() || json_value["sign"].empty() || !json_value["sign"].isString())
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_sign_error);
				return;
			}
			if (json_value["money"].isNull() || json_value["money"].empty() || !json_value["money"].isString())
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_price_error);
				return;
			}
			if (json_value["timestamp"].isNull() || json_value["timestamp"].empty() || !json_value["timestamp"].isString())
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_order_timeout);
				return;
			}

			int32 payment_type;
			if (json_value["pay_channel"].asString() == "appstore")
			{
				payment_type = e_payment_method_en_ios;
			}
			else if (json_value["pay_channel"].asString() == "googleplay")
			{
				payment_type = e_payment_method_en_android;
			}
			else
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_failed);
				return;
			}


			if (json_value["platform"].isNull() || json_value["platform"].empty() || !json_value["platform"].isString()
				|| (json_value["platform"].asString() != "tw_iOS" && json_value["platform"].asString() != "tw_googleplay" && json_value["platform"].asString() != "tw_top1game"))
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_platform_error);
				return;
			}

			RechargeTemplate* custom_recharge_template_ptr = recharge_mgr_ws::get_instance().get_recharge_template_ptr(json_value["product_id"].asString());
			if (custom_recharge_template_ptr == nullptr)
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_failed);
				return;
			}

			float money = init_unit::change_string_to_float(json_value["money"].asString());
			if (money - custom_recharge_template_ptr->Price > 0.000001)
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_price_error);
				return;
			}

			int32 buy_type = json_value["buy_type"].asInt();
			int32 time_stamp = init_unit::change_string_to_i32(json_value["timestamp"].asString());
			int32 cur_time_stamp = time_helper::get_cur_time_new().second;
			if (buy_type == e_en_recharge_buy_type_normal)
			{
				if (cur_time_stamp - time_stamp > day_time_second)
				{
					recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_order_timeout);
					return;
				}
			}
			else if (buy_type == e_en_recharge_buy_type_manual_supplement)
			{
				if (cur_time_stamp - time_stamp > day_time_second * 7)
				{
					recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_order_timeout);
					return;
				}
			}
			else
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_order_timeout);
				return;
			}

			std::string temp_sign = json_value["currency"].asString() + json_value["extra_gift"].asString() + json_value["game_extension"].asString() + json_value["game_id"].asString()
				+ json_value["money"].asString() + json_value["order_id"].asString() + json_value["pay_channel"].asString() + json_value["pay_type"].asString()
				+ json_value["product_id"].asString() + json_value["server_id"].asString() + json_value["timestamp"].asString() + json_value["user_id"].asString() + en_secr_key;
			MD5 md5;
			md5.update(temp_sign.c_str(), temp_sign.size());
			if (md5.toString() != json_value["sign"].asString())
			{
				recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_sign_error);
				return;
			}

			recharge_mgr_ws::get_instance().check_and_save_recharge(json_value["order_id"].asString().c_str(), guid_64(init_unit::change_string_to_i64(json_value["user_id"].asString()))
				, custom_recharge_template_ptr->attribute_id, money, receive_info.m_req_handle_index, payment_type, faith::s_client_uid(), json_value["sign"].asString(), 0, 0);
		}
		else
		{
			recv_recharge_en_end(receive_info.m_req_handle_index, e_recharge_end_msg_error_code_sign_error);
		}
	}
	void web_client::handler_http_on_recv_recharge_end(const s_http_receive_info& receive_info)
	{//amount=30&proItemId=104000503&sign=002353fa3beb9134a3827b313132f59e&timeStamp=1584600564&userId=344226087&serverCode=11001&gameCurrency=60&orderId=926702496&payment_type=ios&roleId=6805828696482523897
		s_daemon_callback_info callback_info;
		callback_info.listen_port = receive_info.m_req_listen_port;
		callback_info.handle_index = receive_info.m_req_handle_index;
		std::vector<xstring> recharge_vector;
		init_unit::explode(receive_info.m_req_body, recharge_vector, "&");
		xstring jdata_orderId = "";
		xstring jdata_roleId = "";
		xstring jdata_proItemId = "";
		xstring jdata_amount = "";
		xstring jdata_gameCurrency = "";
		int32 recharge_size = recharge_vector.size();
		if (recharge_size < 9)
		{
			send_recharge_end_result(e_recharge_error_http_verify_failed, receive_info.m_req_handle_index);
			return;
		}

		for (int32 i = 0; i < recharge_size; ++i)
		{
			std::vector<xstring> data_vector;
			init_unit::explode(recharge_vector[i], data_vector, "=");
			if (data_vector.size() < 2)
			{
				send_recharge_end_result(e_recharge_error_http_verify_failed, receive_info.m_req_handle_index);
				return;
			}
			if (data_vector[0] == "orderId")
			{
				jdata_orderId = data_vector[1];
			}
			else if (data_vector[0] == "roleId")
			{
				jdata_roleId = data_vector[1];
			}
			else if (data_vector[0] == "proItemId")
			{
				jdata_proItemId = data_vector[1];
				if (recharge_size < 10)
				{
					send_recharge_end_result(e_recharge_error_http_verify_failed, receive_info.m_req_handle_index);
					return;
				}
			}
			else if (data_vector[0] == "amount")
			{
				jdata_amount = data_vector[1];
			}
			else if (data_vector[0] == "gameCurrency")
			{
				jdata_gameCurrency = data_vector[1];
			}
		}
		int64 role_id_srv64 = init_unit::change_string_to_i64(jdata_roleId);
		guid_64 role_guid(role_id_srv64);
		int32 game_goods_id = init_unit::change_string_to_i32(jdata_proItemId);
		float pay_price = init_unit::change_string_to_float(jdata_amount);
		int32 direct_diamond = init_unit::change_string_to_i32(jdata_gameCurrency);
		recv_recharge(jdata_orderId, role_guid, game_goods_id, pay_price, direct_diamond, callback_info);
	}

	void web_client::handler_http_on_recv_umisdkrecharge_end(const s_http_receive_info& receive_info)
	{
		Json::Value json_value;
		Json::Reader json_reader;

		xstring base_end = Base64::Decode(receive_info.m_req_body.c_str(), receive_info.m_req_body.size());
		int32 base_len = base_end.size();
		if (base_len > 1000)
		{
			return;
		}
		xstring key_str = SERVERCONFIG->http_aes;//如果key为空，解析aes后字符串概率崩溃
		if (key_str.size() <= 0)
		{
			return;
		}
		xstring aes_end = AES::aes_128_ecb_decrypt(base_end, key_str);

		CONSOLE_INFO("http ase:{}", aes_end);

		if (!json_reader.parse(aes_end, json_value) || json_value.empty() || !json_value.isObject())
		{
			send_recharge_end_result(e_recharge_error_http_verify_failed, receive_info.m_req_handle_index);
			return;
		}

		if (json_value["roleId"].isNull() || json_value["roleId"].empty() || !json_value["roleId"].isString()
			|| json_value["billingOrderId"].isNull() || json_value["billingOrderId"].empty() || !json_value["billingOrderId"].isString()
			|| json_value["gameProductId"].isNull() || json_value["gameProductId"].empty() || !json_value["gameProductId"].isString()
			|| json_value["payPrice"].isNull() || json_value["payPrice"].empty() || !json_value["payPrice"].isDouble())
		{
			send_recharge_end_result(e_recharge_error_http_verify_failed, receive_info.m_req_handle_index);
			return;
		}
		float money = json_value["payPrice"].asFloat();
		xstring order_id = json_value["billingOrderId"].asString();
		guid_64 role_guid(init_unit::change_string_to_i64(json_value["roleId"].asString()));
		int32 goods_id = init_unit::change_string_to_i32(json_value["gameProductId"].asString());
		recharge_mgr_ws::get_instance().check_and_save_recharge(order_id.c_str(), role_guid, goods_id, money, receive_info.m_req_handle_index, e_payment_method_tl_umisdk_google);
		//recv_recharge_end(receive_info.m_req_listen_port, receive_info.m_req_handle_index, e_recharge_end_msg_error_code_success);

	}

	void web_client::recv_recharge_en_end(int64 handle_index, int32 code_index)
	{
		int32 real_code = code_index;
		if (code_index == 0)
		{
			real_code = e_recharge_end_msg_error_code_success;
		}
		else if (code_index < e_recharge_end_msg_error_code_success)
		{
			real_code = e_recharge_end_msg_error_code_failed;
		}
		Json::Value server_info_json;
		server_info_json["code"] = real_code;
		switch (real_code)
		{
		case faith::e_recharge_end_msg_error_code_success:
		{
			server_info_json["msg"] = "SUCCESS";
		}
		break;
		case faith::e_recharge_end_msg_error_code_perpeat_order:
		{
			server_info_json["msg"] = "PERPEAT_ORDER";
		}
		break;
		case faith::e_recharge_end_msg_error_code_sign_error:
		{
			server_info_json["msg"] = "SIGN_ERROR";
		}
		break;
		case faith::e_recharge_end_msg_error_code_price_error:
		{
			server_info_json["msg"] = "PRICE_ERROR";
		}
		break;
		case faith::e_recharge_end_msg_error_code_user_error:
		{
			server_info_json["msg"] = "USER_ERROR";
		}
		break;
		case faith::e_recharge_end_msg_error_code_platform_error:
		{
			server_info_json["msg"] = "PLATFORM_ERROR";
		}
		break;
		case faith::e_recharge_end_msg_error_code_order_timeout:
		{
			server_info_json["msg"] = "ORDER_TIMEOUT";
		}
		break;
		case faith::e_recharge_end_msg_error_code_failed:
		{
			server_info_json["msg"] = "FAILED";
		}
		break;
		default:
			return;
		}
		Json::FastWriter fast_writer;
		std::string server_info_json_str = fast_writer.write(server_info_json);
		http_access_mgr::get_instance().repose_client_req(handle_index, server_info_json_str);
	}
}