/********************************************************************
	created:	2016??12??5??12:01:46
	file base:	server
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "db_log_type_def.hpp"
#include "game_cfg/servers_config.h"
#include "internal/server_log_msg.hpp"
#include "Logic/auction_def.h"
#include "login_def.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/cs_date.hpp"
#include "utility/globle_data.h"
#include "utility/init_unit.h"
#include <rlog.hpp>


namespace faith
{
	server_log::sender_handler_type server_log::log_db_sender_handler = NULL;
#define SHOW_SERVER_LOG
	std::string server_log_get_game_server_id(int32 server_template_id)
	{
		/*ServerListTemplate* server_template_ptr = GET_TEMPLATE(ServerListTemplate, server_template_id);
		if (server_template_ptr == nullptr)
		{
			return server_invaild_string;
		}
		int32 game_server_id = server_template_ptr->GroupId * server_group_level_num + server_template_ptr->ServerId;
		std::stringstream temp_stream;
		temp_stream << game_server_id;
		std::string return_value;
		temp_stream >> return_value;
		return return_value;*/
		std::stringstream temp_stream;
		temp_stream << server_template_id;
		std::string return_value;
		temp_stream >> return_value;
		return return_value;
	}

	std::string server_log_get_money_json(s_money_info& money_info)
	{
		std::string return_value;

		return_value = "{";
		for (int32 i = 0; i < e_money_type_max; ++i)
		{
			std::stringstream money_stream;
			money_stream << "\"" << i << "\"" << ":" << money_info.all_data_ary[i];
			std::string temp_string;
			money_stream >> temp_string;
			return_value += temp_string;

			if (i != e_money_type_max - 1)
			{
				return_value += ",";
			}
		}
		return_value += "}";
		return return_value;
	}

	std::string server_log_get_item_json(s_item_info* item_info, int32 item_num)
	{
		std::string return_value;
		return_value += "{";
		for (int32 item_index = 0; item_index < item_num; ++item_index)
		{
			s_item_info temp_info = item_info[item_index];
			return_value +="[";

			return_value += "item_guid:";
			std::stringstream item_guid_stream;
			item_guid_stream << temp_info.item_guid.server_64;
			std::string item_guid_string;
			item_guid_stream >> item_guid_string;
			return_value += item_guid_string;

			return_value += ",";

			for (int32 info_index = 0; info_index < e_item_info_max; ++info_index)
			{
				std::stringstream item_info_stream;
				item_info_stream << info_index << ":" << temp_info.data_ary[info_index];
				std::string temp_string;
				item_info_stream >> temp_string;
				return_value += temp_string;
				if (info_index != e_item_info_max - 1)
				{
					return_value += ",";
				}
			}

			if (item_index == item_num - 1)
			{
				return_value += "]";
			}
			else
			{
				return_value += "],";
			}
		}
		return_value += "}";
		return return_value;
	}

	std::string server_log_get_good_item_json(int32 good_template_id)
	{
		std::string return_value;
		GoodsTemplate* good_template_ptr = GET_TEMPLATE(GoodsTemplate, good_template_id);
		if (good_template_ptr == nullptr)
		{
			return return_value;
		}

		std::stringstream good_stream;
		good_stream << "{" << "\"" << good_template_ptr->ItemId << "\"" << ":" << good_template_ptr->GoodsNum << "}";
		good_stream >> return_value;

		return return_value;
	}

	const std::string server_log_trans(const xchar* source)
	{
		if (strlen(source) > 0)
		{
			std::string return_string = "\"";
			return_string += source;
			return_string += "\"";
			return return_string;
		}
		else
		{
			std::string return_string = "\"null\"";
			return return_string;
		}
	}
	const std::string server_log_trans(std::string source)
	{
		if (source.size() > 0)
		{
			std::string return_string = "\"";
			return_string += source;
			return_string += "\"";
			return return_string;
		}
		else
		{
			std::string return_string = "\"null\"";
			return return_string;
		}
	}

	int64  server_log_trans(int64 source)
	{
		return source;
	}
	void server_log::init_new_log(int32, xstring process_name)
	{
		rlog::options options;
		options.basename = process_name;
		rlog::init(options);
	}

	void	server_log::init_log_db_sender(server_log::sender_handler_type handler)
	{
		log_db_sender_handler = handler;
	}

	void	server_log::send_log_to_db(const xstring& table_name, const xstring& log_string)
	{
		bool not_need_save_db_log = SERVERCONFIG->not_need_db_log;

		if (not_need_save_db_log)
		{
			return;
		}
		if (log_db_sender_handler)
		{
			server2dp_save_log req;
			int32 cpy_size = table_name.size() > max_name_size ? max_name_size : table_name.size();
			memcpy(req.sql_table_name, table_name.c_str(), cpy_size);
			cpy_size = log_string.size() > max_log_char_num ? max_log_char_num : log_string.size();
			memcpy(req.save_string, log_string.c_str(), cpy_size);
			log_db_sender_handler(&req, sizeof(req), 0);
		}
	}

	void 	server_log::push_log_string(xstring& des_str, const xstring& src_str, const xstring& json_key, bool is_final)
	{
		des_str += "\"";
		des_str += json_key;
		des_str += "\"";
		des_str += ":";
		des_str += server_log_trans(src_str);
		if (!is_final)
		{
			des_str += ",";
		}
		//des_str.push_back(server_log_separator);
	}

	void server_log::push_log_string(xstring& des_str, int32 src_num, const xstring& json_key, bool is_final)
	{
		xchar temp[max_log_int64_size] = "";
		sprintf(temp, "%d", src_num);
		des_str += "\"";
		des_str += json_key;
		des_str += "\"";
		des_str += ":";
		des_str += server_log_trans(temp);
		if (!is_final)
		{
			des_str += ",";
		}
	}
	void 	server_log::push_log_string(xstring& des_str, int64 src_num, const xstring& json_key, bool is_final)
	{
		xchar temp[max_log_int64_size] = "";
		sprintf(temp, "%I64d", src_num);
		des_str += "\"";
		des_str += json_key;
		des_str += "\"";
		des_str += ":";
		des_str += server_log_trans(temp);
		if (!is_final)
		{
			des_str += ",";
		}
		//des_str.push_back(server_log_separator);
	}
	void 	server_log::push_log_string(xstring& des_str, uint64 src_num, const xstring& json_key, bool is_final)
	{
		xchar temp[max_log_int64_size] = "";
		sprintf(temp, "%I64d", src_num);
		des_str += "\"";
		des_str += json_key;
		des_str += "\"";
		des_str += ":";
		des_str += server_log_trans(temp);
		if (!is_final)
		{
			des_str += ",";
		}
		//des_str.push_back(server_log_separator);
	}
	void server_log::push_log_string(xstring& des_str, double src_num, const xstring& json_key, bool is_final)
	{
		xchar temp[max_log_int64_size] = "";
		sprintf(temp, "%lf", src_num);
		des_str += "\"";
		des_str += json_key;
		des_str += "\"";
		des_str += ":";
		des_str += server_log_trans(temp);
		if (!is_final)
		{
			des_str += ",";
		}
	}
	void server_log::push_hadooplog_string(xstring & des_str, const xstring & json_key)
	{
		des_str += "\"";
		des_str += json_key;
		des_str += "\"";
		des_str += ":";
	}
	xstring server_log::change_i32_to_string(int32 value_i32)
	{	
		std::string res_value = "";
		std::stringstream ss;
		ss << value_i32;
		ss >> res_value;
		return res_value;
	}
	int32 server_log::change_string_to_i32(std::string str_i32)
	{
		int32 res_value = 0;
		std::stringstream ss;
		ss << str_i32;
		ss >> res_value;
		return res_value;
	}
	void server_log::login_role_log(int32 server_id, std::string app_key, std::string client_version, xstring gamechannel, xstring adchannel, std::string account, std::string device_id, std::string ip, xstring model, xstring idfa, xstring imei, xstring android_id, xstring mac, xstring sn)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_LOGIN);
		log_string += "{";
		server_log::push_log_string(log_string, account, "userid");
		server_log::push_log_string(log_string, device_id, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, app_key, "appid");								//appid
		server_log::push_log_string(log_string, "login", "logname");
		server_log::push_log_string(log_string, client_version, "version");
		server_log::push_log_string(log_string, "2050", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, server_id, "serverid");
		server_log::push_log_string(log_string, gamechannel, "gamechannel");
		server_log::push_log_string(log_string, adchannel, "adchannel");
		server_log::push_log_string(log_string, server_invaild_string, "roleid");
		server_log::push_log_string(log_string, server_invaild_string, "rolelevel");
		server_log::push_log_string(log_string, server_invaild_string, "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, account, "accname");
		server_log::push_log_string(log_string, ip, "ip");										//ip
		server_log::push_log_string(log_string, server_invaild_string, "valueamount");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, model, "model");
		server_log::push_log_string(log_string, idfa, "idfa"); 									//idfa
		server_log::push_log_string(log_string, imei, "imei"); 									//imei
		server_log::push_log_string(log_string, android_id, "androidId"); 									//androidId
		server_log::push_log_string(log_string, mac, "mac"); 									//mac
		server_log::push_log_string(log_string, sn, "sn", true); 									//sn

		log_string += "}}";





		
		

		//server_log::push_log_string(log_string, server_log_trans(app_key));										//app key
		//PROJECT_RECORD_LOG(g_login_role_logger, log_string);
#endif
	}

	void server_log::login_create_role_log(int32 server_id, std::string app_key, std::string client_version, xstring gamechannel, xstring adchannel, std::string account, std::string device_id, std::string ip, xstring model, xstring idfa, xstring imei, xstring android_id, xstring mac, xstring sn, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ACCOUNT_CREATE);
		log_string += "{";
		server_log::push_log_string(log_string, account, "userid");
		server_log::push_log_string(log_string, device_id, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, app_key, "appid");								//appid
		server_log::push_log_string(log_string, "login", "logname");
		server_log::push_log_string(log_string, client_version, "version");
		server_log::push_log_string(log_string, "2050", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, server_id, "serverid");
		server_log::push_log_string(log_string, gamechannel, "gamechannel");
		server_log::push_log_string(log_string, adchannel, "adchannel");
		server_log::push_log_string(log_string, server_invaild_string, "roleid");
		server_log::push_log_string(log_string, server_invaild_string, "rolelevel");
		server_log::push_log_string(log_string, server_invaild_string, "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, account, "accname");
		server_log::push_log_string(log_string, ip, "ip");										//ip
		server_log::push_log_string(log_string, server_invaild_string, "valueamount");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, model, "model");
		server_log::push_log_string(log_string, idfa, "idfa"); 									//idfa
		server_log::push_log_string(log_string, imei, "imei"); 									//imei
		server_log::push_log_string(log_string, android_id, "androidId"); 									//androidId
		server_log::push_log_string(log_string, mac, "mac"); 									//mac
		server_log::push_log_string(log_string, sn, "sn", true); 									//sn
		
		log_string += "}}";
		//server_log::push_log_string(log_string, server_log_trans(app_key));										//app key
		//PROJECT_RECORD_LOG(g_login_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		{
			Json::Value json;
			Json::FastWriter writer;
			json["server_id"] = SERVERCONFIG->game_id;
			json["log_time"] = log_time;
			json["account_id"] = account;
			json["device_id"] = device_id;

			json["login_type"] = login_type;
			json["appid"] = app_key;
			json["version"] = client_version;
			json["stepnumid"] = "2050";
			json["normversion"] = server_log_normversion;
			json["gamechannel"] = gamechannel;
			json["ip"] = ip;
			json["adchannel"] = adchannel;
			json["cross_service"] = "";
			json["os_version"] = server_invaild_string;
			json["model"] = model;
			json["idfa"] = idfa;
			json["imei"] = imei;
			json["androidId"] = android_id;
			json["mac"] = mac;
			json["sn"] = sn;

			xstring str_db_log = writer.write_no_enter(json);
			send_log_to_db(TABLE_NAME_ACCOUNT_CREATE, str_db_log);
		}
#endif
	}

	void server_log::create_role_log(int32 server_template_id, xstring deviceid, xstring appid, xstring version, xstring gamechannel, xstring adchannel, xstring model, xstring idfa, xstring imei, xstring android_id, xstring mac, xstring sn, const s_unit_info& role_info, xstring ip, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		std::string game_server = server_log_get_game_server_id(server_template_id);

		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;

		std::stringstream temp_template_id;
		temp_template_id << role_info.data_ary[e_role_info_template_id];
		std::string return_template_id;
		temp_template_id >> return_template_id;

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CREATE);
		log_string += "{";

		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, deviceid, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, appid, "appid");											//appid
		server_log::push_log_string(log_string, "rolebuild", "logname");
		server_log::push_log_string(log_string, version, "version");
		server_log::push_log_string(log_string, "3025", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, gamechannel, "gamechannel");
		server_log::push_log_string(log_string, adchannel, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, role_info.account, "accname");
		server_log::push_log_string(log_string, role_info.role_name, "rolename");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "roletypeid");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, model, "model");
		server_log::push_log_string(log_string, idfa, "idfa"); 												//idfa
		server_log::push_log_string(log_string, imei, "imei"); 												//imei
		server_log::push_log_string(log_string, android_id, "androidId"); 									//androidId
		server_log::push_log_string(log_string, mac, "mac"); 												//mac
		server_log::push_log_string(log_string, ip, "ip"); 													//ip
		server_log::push_log_string(log_string, sn, "sn", true); 											//sn

		log_string += "}}";
	
		//PROJECT_RECORD_LOG(g_create_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = deviceid;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["career"] = return_template_id;

		json["login_type"] = login_type;
		json["appid"] = appid;
		json["version"] = version;
		json["stepnumid"] = "3025";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = gamechannel;
		json["ip"] = ip;
		json["adchannel"] = adchannel;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = model;
		json["idfa"] = idfa;
		json["imei"] = imei;
		json["androidId"] = android_id;
		json["mac"] = mac;
		json["sn"] = sn;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_CREATE, str_db_log);
		
#endif
	}
	void server_log::enter_game_role_log(login_fixed_data& third_info, s_unit_info& role_info, s_money_info& money_info, int64 online_time, int64 create_time, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		//std::string money_json = server_log_get_money_json(money_info);
 
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();
		xstring create_times = time_helper::get_str_time_by_stamp(create_time);

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_LOGOUT);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, third_info.param9, "idfa"); 									//idfa
		//server_log::push_log_string(log_string, third_info.param10, "imei"); 									//imei
		//server_log::push_log_string(log_string, third_info.param11, "androidId"); 									//androidId
		//server_log::push_log_string(log_string, third_info.param12, "mac"); 									//mac
		//server_log::push_log_string(log_string, third_info.param13, "sn", true); 									//sn


		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_enter_game_role_logger, log_string);

		online_time = online_time / 1000;
		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["channel_id"] = third_info.param7;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;



		json["ip"] = third_info.param6;

		if (login_type == e_login_type_new_token)
		{
			json["op_type"] = 3;
		}
		else
		{
			json["op_type"] = 0;
		}

		json["create_time"] = create_times;
		json["online_time"] = online_time;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "6010";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_LOGOUT, str_db_log);
#endif
	}

	void server_log::logout_role_log(login_fixed_data& third_info, s_unit_info& role_info, s_money_info& money_info, int64 online_time, int64 create_time, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		//std::string money_json = server_log_get_money_json(money_info);
		xstring create_times = time_helper::get_str_time_by_stamp(create_time);

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();
		online_time = online_time / 1000;

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_ACCOUNT_CREATE);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_logout_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;

		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["channel_id"] = third_info.param7;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["op_type"] = 1;
		json["create_time"] = create_times;
		json["online_time"] = online_time;
		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "6010";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_LOGOUT, str_db_log);
#endif
	}
	void server_log::across_day_log(login_fixed_data& third_info, s_unit_info& role_info, int64 online_time, int64 create_time, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring create_times = time_helper::get_str_time_by_stamp(create_time);

		online_time = online_time / 1000;
		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_LOGOUT);

		//log_string += "{";
		//server_log::push_log_string(log_string, SERVERCONFIG->game_id, "server_id");
		//server_log::push_log_string(log_string, log_time, "log_time");
		//server_log::push_log_string(log_string, role_info.account, "account_id");
		//server_log::push_log_string(log_string, third_info.param5, "device_id");
		//server_log::push_log_string(log_string, third_info.param7, "channel_id");										
		//server_log::push_log_string(log_string, return_value, "role_id");
		//server_log::push_log_string(log_string, role_info.role_name, "role_name");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "role_level");
		//server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "role_power");
		//server_log::push_log_string(log_string, login_type, "login_type");
		//server_log::push_log_string(log_string, third_info.param1, "appid");								
		//server_log::push_log_string(log_string, third_info.param2, "version");
		//server_log::push_log_string(log_string, "6010", "stepnumid");
		//server_log::push_log_string(log_string, server_log_normversion, "normversion");
		//server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		//server_log::push_log_string(log_string, third_info.param6, "ip");
		//server_log::push_log_string(log_string, third_info.param4, "adchannel");
		//server_log::push_log_string(log_string, "", "cross_service");
		//server_log::push_log_string(log_string, server_invaild_string, "os_version");
		//server_log::push_log_string(log_string, third_info.param8, "model");
		//server_log::push_log_string(log_string, third_info.param9, "idfa");
		//server_log::push_log_string(log_string, third_info.param10, "imei");
		//server_log::push_log_string(log_string, third_info.param11, "androidId");
		//server_log::push_log_string(log_string, third_info.param12, "mac");
		//server_log::push_log_string(log_string, third_info.param13, "sn", true);

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_across_day_logger, log_string);

				// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["channel_id"] = third_info.param7;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "6010";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_LOGOUT, str_db_log);
#endif
	}

	void server_log::level_up_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 old_level, int32 cur_level, int32 cur_level_time, int32 old_gs, int64 online_time, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
 
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_LEVELCHANGE);
		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");


		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_level_up_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;

		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;

		json["old_level"] = old_level;
		json["new_level"] = cur_level;
		json["online_time"] = cur_level_time / 1000;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "6010";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_LEVELCHANGE, str_db_log);
#endif
	}


	void server_log::money_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 action, int32 money_id, int64 change_value, int64 remain_value, int32 causeid, int32 location, int64 old_money, int64 new_money, int32 param, int32 param2, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		if (change_value == 0)
		{
			return;
		}

		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_MONEYCHANGE);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");				//vip


		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_money_role_logger, log_string);

		if (money_id != e_money_type_exp)
		{
			xstring log_time = time_helper::get_current_time();
			std::stringstream temp_stream;
			temp_stream << role_info.role_guid.server_64;
			std::string return_value;
			temp_stream >> return_value;
			// send log to db
			Json::Value json;
			Json::FastWriter writer;
			json["server_id"] = SERVERCONFIG->game_id;
			json["log_time"] = log_time;
			json["account_id"] = role_info.account;
			json["device_id"] = third_info.param5;
			json["role_id"] = return_value;
			json["role_name"] = role_info.role_name;
			json["role_level"] = role_info.data_ary[e_role_info_exp_level];
			json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;

			json["role_vip"] = role_info.data_ary[e_role_info_vip_level];
			json["money_type"] = money_id;
			json["old_money"] = old_money;
			json["new_money"] = new_money;
			json["op_type"] = action;
			json["cause_id"] = causeid;
			json["location"] = location;
			json["action"] = action;
			json["param2"] = param2;

			json["login_type"] = login_type;
			json["appid"] = third_info.param1;
			json["version"] = third_info.param2;
			json["stepnumid"] = "6010";
			json["normversion"] = server_log_normversion;
			json["gamechannel"] = third_info.param14;
			json["ip"] = third_info.param6;
			json["adchannel"] = third_info.param4;
			json["cross_service"] = "";
			json["os_version"] = server_invaild_string;
			json["model"] = third_info.param8;
			json["idfa"] = third_info.param9;
			json["imei"] = third_info.param10;
			json["androidId"] = third_info.param11;
			json["mac"] = third_info.param12;
			json["sn"] = third_info.param13;

			xstring str_db_log = writer.write_no_enter(json);
			send_log_to_db(TABLE_NAME_MONEYCHANGE, str_db_log);
		}
#endif
	}

	void server_log::item_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 action, int32 item_id, int32 change_value, int32 causeid, int32 location, guid_64 item_guid, int32 left_item_count, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
		if (item_template_ptr == nullptr)
		{
			return;
		}
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_ITEMCHANGE);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");				//vip
		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_item_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["item_id"] = item_id;
		json["change_num"] = change_value;
		json["left_num"] = left_item_count;
		json["op_type"] = action;
		json["cause_id"] = causeid;
		json["location"] = location;
		json["item_guid"] = item_guid.server_64;


		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_ITEMCHANGE, str_db_log);
#endif
	}
	void server_log::item_operate_log(login_fixed_data& third_info, s_unit_info& role_info, const std::vector<guid_64>& item_guid, int32 item_slot, int32 operation_type, int32 use_up_item, int32 first_use_lock, int32 item_num, int32 force_do)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_OPERATE_ITEM);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "itemoperate", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B2110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, item_slot, "itemslot");
		server_log::push_log_string(log_string, operation_type, "operationtype");
		server_log::push_log_string(log_string, use_up_item, "useupitem");
		server_log::push_log_string(log_string, first_use_lock, "firstuselock");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");				//vip
		server_log::push_log_string(log_string, item_num, "itemnum");
		for (int i =0; i < item_guid.size(); ++i)
		{
			server_log::push_log_string(log_string, item_guid[i].server_64, "itemguid");
		}
		server_log::push_log_string(log_string, force_do, "forcedo", true);
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_item_operate_logger, log_string);
#endif
	}
	void server_log::buy_goods_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 shop_template_id, int32 good_template_id, int32 buy_num, int32 login_type,int32 last_login_time)
	{
#ifdef SHOW_SERVER_LOG
		GoodsTemplate* good_template_ptr = GET_TEMPLATE(GoodsTemplate, good_template_id);
		if (good_template_ptr == nullptr)
		{
			return ;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		int32 money_type_index = region_template_ptr->RegionCode * 2;
		int32 money_num_index = money_type_index + 1;

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, good_template_ptr->ItemId);
		if (item_template_ptr == nullptr)
		{
			return ;
		}

		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		std::string item_json = server_log_get_good_item_json(good_template_id);

		int32 money_id = 0;
		int32 money_num = 0;
		if (good_template_ptr->NewPrice.size() > money_num_index)
		{
			money_id = good_template_ptr->NewPrice[money_type_index];
			money_num = good_template_ptr->NewPrice[money_num_index] * buy_num;
		}
		int32 need_item_id = 0;
		int32 need_item_num = 0;
		if (good_template_ptr->NeedItemId.size() > 0)
		{
			need_item_id = good_template_ptr->NeedItemId[0];
			need_item_num = good_template_ptr->NeedItemId[1];
		}

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_GOODS_LOG);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "shoptrade", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "8010", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, last_login_time, "lastlogintime");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "viplevel");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, server_invaild_string, "sn");
		server_log::push_log_string(log_string, third_info.param6, "ip");										//ip
		server_log::push_log_string(log_string, item_template_ptr->item_type, "itemtypeid");
		server_log::push_log_string(log_string, good_template_ptr->ItemId, "itemid");
		server_log::push_log_string(log_string, server_invaild_string, "itemvalid");
		server_log::push_log_string(log_string, buy_num * good_template_ptr->GoodsNum, "itemcount");
		server_log::push_log_string(log_string, money_id, "moneytypeid");
		server_log::push_log_string(log_string, money_num, "moneycount");
		server_log::push_log_string(log_string, need_item_id, "cost_item_id");
		server_log::push_log_string(log_string, need_item_num, "cost_item_num");
		server_log::push_log_string(log_string, shop_template_id, "shopid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_buy_goods_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["goods_id"] = good_template_id;
		json["buy_num"] = buy_num;
		json["item_id"] = good_template_ptr->ItemId;
		json["item_num_per_goods"] = good_template_ptr->GoodsNum;
		json["cost_money_type"] = money_id;
		json["cost_money_num"] = money_num;
		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_GOODS_LOG, str_db_log);
#endif																												
	}	

	void server_log::role_mall_log(login_fixed_data& third_info, s_unit_info& role_info, int32 consumed_currency_type, int32 consumed_num, int32 store_id, int32 sold_num, int32 sold_id, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		GoodsTemplate* good_template_ptr = GET_TEMPLATE(GoodsTemplate, sold_id);
		if (good_template_ptr == nullptr)
		{
			return;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		int32 money_type_index = region_template_ptr->RegionCode * 2;
		int32 money_num_index = money_type_index + 1;

		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, good_template_ptr->ItemId);
		if (item_template_ptr == nullptr)
		{
			return;
		}

		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		std::string item_json = server_log_get_good_item_json(sold_id);

		int32 money_id = 0;
		int32 money_num = 0;
		int32 single_price = 0;
		if (good_template_ptr->NewPrice.size() > money_num_index)
		{
			money_id = good_template_ptr->NewPrice[money_type_index];
			money_num = good_template_ptr->NewPrice[money_num_index] * sold_num;
			single_price = good_template_ptr->NewPrice[money_num_index];
		}
		int32 need_item_id = 0;
		int32 need_item_num = 0;
	
		if (good_template_ptr->NeedItemId.size() > 0)
		{
			need_item_id = good_template_ptr->NeedItemId[0];
			need_item_num = good_template_ptr->NeedItemId[1];
		}

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_MALL);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "shoptrade", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "8010", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "viplevel");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, server_invaild_string, "sn");
		server_log::push_log_string(log_string, third_info.param6, "ip");										//ip
		server_log::push_log_string(log_string, item_template_ptr->item_type, "itemtypeid");
		server_log::push_log_string(log_string, good_template_ptr->ItemId, "itemid");
		server_log::push_log_string(log_string, server_invaild_string, "itemvalid");
		server_log::push_log_string(log_string, sold_num * good_template_ptr->GoodsNum, "itemcount");
		server_log::push_log_string(log_string, money_id, "moneytypeid");
		server_log::push_log_string(log_string, money_num, "moneycount");
		server_log::push_log_string(log_string, need_item_id, "cost_item_id");
		server_log::push_log_string(log_string, need_item_num, "cost_item_num");
		server_log::push_log_string(log_string, store_id, "shopid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_role_mall_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["deviceid"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = role_info.data_i64_ary[e_role_i64_info_gs_value];
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["reward_num"] = sold_num * good_template_ptr->GoodsNum;
		json["reward_id"] = good_template_ptr->ItemId;
		json["store_id"] = store_id;
		json["consumed_currency_type"] = money_id;
		json["consumed_num"] = money_num;
		json["sold_num"] = sold_num;
		json["sold_id"] = sold_id;

		json["login_type"] = login_type;
		json["channel_id"] = third_info.param7;

		xstring str_db_log = writer.write_no_enter(json);

		set_log_var(log_head)
		set_log_common_head_part1(log_head, SERVERCONFIG->app_key, third_info.param7, third_info.param4, third_info.param5);
		set_log_common_head_part2(log_head, third_info.param2, init_unit::change_string_to_i32(third_info.param15));
		set_log_common_head_part3(log_head, role_info.account, SERVERCONFIG->game_id, utility::get_tick_count());
		set_log_common_head_part4(log_head, role_info.role_guid, role_info.role_name, role_info.data_ary[e_role_info_exp_level], role_info.data_ary[e_role_info_gender])
		set_log_common_head_part5(log_head, role_info.data_i64_ary[e_role_i64_info_gs_value], role_info.data_ary[e_role_info_class_type], role_info.data_ary[e_role_info_vip_level]);


		server_log::serverRoleMallLog(log_head, 0, good_template_ptr->ItemId, sold_num * good_template_ptr->GoodsNum, money_id,
			money_num, "",change_i32_to_string(store_id), sold_num, change_i32_to_string(money_id), single_price);

#endif																												
	}





	void server_log::gettask_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 mission_template_id, int32 get_result, int32 task_type, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id); 

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_TASK);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_gettask_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;

		json["op_type"] = 0;
		json["task_type"] = task_type;
		json["task_id"] = mission_template_id;
		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "6010";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_TASK, str_db_log);
#endif
	}

	void server_log::finishtask_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 mission_template_id, int32 finish_result, int32 task_type, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_MONEYCHANGE);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_finishtask_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;

		json["op_type"] = 1;
		json["task_type"] = task_type;
		json["task_id"] = mission_template_id;
		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "6010";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_TASK, str_db_log);

#endif
	}

	void server_log::raid_fight_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 raid_template_id, int32 type, int32 result, int32 map_id, int32 star, int32 diffcult, int32 map_type, int32 relive_times)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
 
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_FIGHT_RAID);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "pvefight", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B4110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, raid_template_id, "stageid");
		server_log::push_log_string(log_string, type, "type");
		server_log::push_log_string(log_string, server_invaild_string, "npcid");								//npcid
		server_log::push_log_string(log_string, result, "result");
		server_log::push_log_string(log_string, map_id, "mapid");
		server_log::push_log_string(log_string, star, "star");
		server_log::push_log_string(log_string, diffcult, "stagetype");
		server_log::push_log_string(log_string, map_type, "maptype");
		server_log::push_log_string(log_string, relive_times, "relive", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_raid_fight_role_logger, log_string);
		 

#endif
	}

	void server_log::achievement_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 achievement_template_id)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		 
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_ACHIEVEMENT);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "achievement", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B5110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, achievement_template_id, "achievementid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_achievement_role_logger, log_string);
		 
#endif
	}

	void server_log::new_stages_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 new_stages_id, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		 
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_NEWGUIDE);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_new_stages_role_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string return_value;
		temp_stream >> return_value;

		std::stringstream temp_stages_id;
		temp_stages_id << new_stages_id;
		std::string return_stages_id;
		temp_stages_id >> return_stages_id;
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = return_value;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["step_id"] = return_stages_id;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_NEWGUIDE, str_db_log);
#endif
	}

	void server_log::activity_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 must_do_type, int32 activity_id)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		 
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_ACTIVITY);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "activity", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B6110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, must_do_type, "mustdotype");
		server_log::push_log_string(log_string, activity_id, "activityid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_activity_role_logger, log_string);
		 
#endif
	}

	void server_log::heart_beat_role_log(const xstring& appkey, int32 server_version, int32 server_id, int32 online_user, int32 queue_user)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();
		std::string game_server = server_log_get_game_server_id(server_id);

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_HEART_BEAT);

		log_string += "{";
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, appkey, "appid");												//APPID
		server_log::push_log_string(log_string, server_version, "version");
		server_log::push_log_string(log_string, "heart", "logname");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, "B9990", "stepnumid");
		server_log::push_log_string(log_string, server_id, "serverid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, online_user, "onlineuser", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_heart_beat_role_logger, log_string);

		
#endif
	}
	void server_log::player_count_log(int32 server_id, int32 online_user)
	{
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();
		std::string game_server = server_log_get_game_server_id(server_id);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["player_count"] = online_user;

		login_fixed_data third_info;
		json["login_type"] = 0;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_PLAYERCOUNT, str_db_log);
	}
	void server_log::recharge_role_log(login_fixed_data& third_info, s_unit_info& role_info, s_money_info& money_info, int32 add_value, int32 recharge_id)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_RECHARGE);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "recharge", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "5050", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, server_invaild_string, "gameorderid");
		server_log::push_log_string(log_string, server_invaild_string, "gamechannelorderid");
		server_log::push_log_string(log_string, server_invaild_string, "orderamount");
		server_log::push_log_string(log_string, server_invaild_string, "shareamount");
		server_log::push_log_string(log_string, server_invaild_string, "noshareamount");
		server_log::push_log_string(log_string, server_invaild_string, "payid");
		server_log::push_log_string(log_string, server_invaild_string, "rechargechannel");
		server_log::push_log_string(log_string, add_value, "valuequantity");
		server_log::push_log_string(log_string, server_invaild_string, "add_value");
		server_log::push_log_string(log_string, third_info.param6, "ip");										//IP
		server_log::push_log_string(log_string, money_info.cur_data_ary[e_money_type_jewel], "valueamount");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");
		server_log::push_log_string(log_string, recharge_id, "itemid");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, third_info.param8, "model");
		server_log::push_log_string(log_string, third_info.param9, "idfa"); 									//idfa
		server_log::push_log_string(log_string, third_info.param10, "imei"); 									//imei
		server_log::push_log_string(log_string, third_info.param11, "androidId"); 									//androidId
		server_log::push_log_string(log_string, third_info.param12, "mac"); 									//mac
		server_log::push_log_string(log_string, third_info.param13, "sn", true); 									//sn

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_recharge_role_logger, log_string);
		

#endif
	}

	void server_log::server_event_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 event_id)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_SERVER_EVENT);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "serverevent", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "7010", "stepnumid");
		server_log::push_log_string(log_string, "v1.2", "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, server_invaild_string, "sdkversion");
		server_log::push_log_string(log_string, server_invaild_string, "system");
		server_log::push_log_string(log_string, event_id, "code");
		server_log::push_log_string(log_string, server_invaild_string, "os_version", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_server_event_role_logger, log_string);
		

#endif
	}

	void server_log::upgrade_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 equip_slot, int32 item_id, int32 quality_level,
		int32 item_color, int32 upgrade_total_num, int32 is_success, int32 money_type, int32 money_num, int32 bind_material_num, int32 no_bind_material_num,
		int32 material_one_id, int32 material_one_num, int32 material_two_id,int32 material_two_num, int32 material_three_id, int32 material_three_num, int32 level)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_UPGRADE_EQUIP);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "upgradeequip", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0410", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "viplevel");
		server_log::push_log_string(log_string, equip_slot, "equipslot");
		server_log::push_log_string(log_string, item_id, "itemid");
		server_log::push_log_string(log_string, quality_level, "qualitylevel");
		server_log::push_log_string(log_string, item_color, "itemcolor");
		server_log::push_log_string(log_string, level, "level");
		server_log::push_log_string(log_string, upgrade_total_num, "upgradetotalnum");
		server_log::push_log_string(log_string, is_success, "issuccess");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, money_num, "moneynum");
		server_log::push_log_string(log_string, bind_material_num, "bindmaterialnum");
		server_log::push_log_string(log_string, no_bind_material_num, "nobindmaterialnum");
		server_log::push_log_string(log_string, material_one_id, "materialoneid");
		server_log::push_log_string(log_string, material_one_num, "materialonenum");
		server_log::push_log_string(log_string, material_two_id, "materialtwoid");
		server_log::push_log_string(log_string, material_two_num, "materialtwonum");
		server_log::push_log_string(log_string, material_three_id, "materialthreeid");
		server_log::push_log_string(log_string, material_three_num, "materialthreenum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_upgrade_equip_logger, log_string);
		

#endif
	}

	void server_log::addon_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 equip_slot, int32 item_id, int32 quality_level, int32 item_color, int32 cur_addon_level,
		int32 is_success, int32 money_type, int32 money_num, int32 bind_material_num, int32 no_bind_material_num, int32 material_id, int32 material_num) 
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ADDON_EQUIP);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "addonequip", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0420", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "viplevel");
		server_log::push_log_string(log_string, equip_slot, "equipslot");
		server_log::push_log_string(log_string, item_id, "itemid");
		server_log::push_log_string(log_string, quality_level, "qualitylevel");
		server_log::push_log_string(log_string, item_color, "itemcolor");
		server_log::push_log_string(log_string, cur_addon_level, "curaddonlevel");
		server_log::push_log_string(log_string, is_success, "issuccess");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, money_num, "moneynum");
		server_log::push_log_string(log_string, bind_material_num, "bindmaterialnum");
		server_log::push_log_string(log_string, no_bind_material_num, "nobindmaterialnum");
		server_log::push_log_string(log_string, material_id, "materialid");
		server_log::push_log_string(log_string, material_num, "materialnum", true);

		log_string += "}}";
		
		//PROJECT_RECORD_LOG(g_addon_equip_logger, log_string);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
		

#endif
	}

	void server_log::succinct_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 equip_slot, int32 item_id, int32 quality_level, int32 item_color, float culturing_percent,
		int32 money_type, int32 money_num, int32 bind_material_num, int32 no_bind_material_num, int32 material_id, int32 material_num)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_SUCCINCT_EQUIP);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "succinctequip", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0430", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "viplevel");
		server_log::push_log_string(log_string, equip_slot, "equipslot");
		server_log::push_log_string(log_string, item_id, "itemid");
		server_log::push_log_string(log_string, quality_level, "qualitylevel");
		server_log::push_log_string(log_string, item_color, "itemcolor");
		server_log::push_log_string(log_string, culturing_percent, "culturingpercent");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, money_num, "moneynum");
		server_log::push_log_string(log_string, bind_material_num, "bindmaterialnum");
		server_log::push_log_string(log_string, no_bind_material_num, "nobindmaterialnum");
		server_log::push_log_string(log_string, material_id, "materialid");
		server_log::push_log_string(log_string, material_num, "materialnum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_succinct_equip_logger, log_string);
		

#endif
	}

	void server_log::succinct_equip_save_log(login_fixed_data& third_info, s_unit_info& role_info, int32 item_id)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_SUCCINCT_EQUIP_SAVE);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "succinctequipsave", "logname");
		server_log::push_log_string(log_string, "C0440", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, item_id, "itemid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_succinct_equip_save_logger, log_string);
		
#endif
	}

	void server_log::advance_equip_log(login_fixed_data& third_info, s_unit_info& role_info, int32 item_id, int32 quality_level, int32 item_color, int32 excellence_num, int32 upgrade_level, int32 addon_level, float culturing_percent,
		int32 first_material_id, int32 first_material_level, int32 first_material_color, int32 first_material_excellence_num, int32 first_material_upgrade_level, int32 first_material_addon_level, float first_material_culturing_percent,
		int32 second_material_id, int32 second_material_level, int32 second_material_color, int32 second_material_excellence_num, int32 second_material_upgrade_level, int32 second_material_addon_level, float second_material_culturing_percent,
		int32 finish_item_id, int32 finish_item_quality_level, int32 finish_item_color, int32 finish_item_excellence_num, int32 finish_item_upgrade_level, int32 finish_item_addon_level, float finish_item_culturing_percent)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ADVANCE_EQUIP);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "advanceequip", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "viplevel");
		server_log::push_log_string(log_string, item_id, "itemid");
		server_log::push_log_string(log_string, quality_level, "qualitylevel");
		server_log::push_log_string(log_string, item_color, "itemcolor");
		server_log::push_log_string(log_string, excellence_num, "excellencenum");
		server_log::push_log_string(log_string, upgrade_level, "upgradelevel");
		server_log::push_log_string(log_string, addon_level, "addonlevel");
		server_log::push_log_string(log_string, culturing_percent, "culturingpercent");
		server_log::push_log_string(log_string, first_material_id, "firstmaterialid");
		server_log::push_log_string(log_string, first_material_level, "firstmateriallevel");
		server_log::push_log_string(log_string, first_material_color, "first_materialcolor");
		server_log::push_log_string(log_string, first_material_excellence_num, "firstmaterialexcellencenum");
		server_log::push_log_string(log_string, first_material_upgrade_level, "firstmaterialupgradelevel");
		server_log::push_log_string(log_string, first_material_addon_level, "firstmaterialaddonlevel");
		server_log::push_log_string(log_string, first_material_culturing_percent, "firstmaterialculturingpercent");
		server_log::push_log_string(log_string, second_material_id, "secondmaterialid");
		server_log::push_log_string(log_string, second_material_level, "secondmateriallevel");
		server_log::push_log_string(log_string, second_material_color, "secondmaterialcolor");
		server_log::push_log_string(log_string, second_material_excellence_num, "secondmaterialexcellencenum");
		server_log::push_log_string(log_string, second_material_upgrade_level, "secondmaterialupgradelevel");
		server_log::push_log_string(log_string, second_material_addon_level, "secondmaterialaddonlevel");
		server_log::push_log_string(log_string, second_material_culturing_percent, "secondmaterialculturingpercent");
		server_log::push_log_string(log_string, finish_item_id, "finishitemid");
		server_log::push_log_string(log_string, finish_item_quality_level, "finishitemqualitylevel");
		server_log::push_log_string(log_string, finish_item_color, "finishitemcolor");
		server_log::push_log_string(log_string, finish_item_excellence_num, "finishitemexcellencenum");
		server_log::push_log_string(log_string, finish_item_upgrade_level, "finishitemupgradelevel");
		server_log::push_log_string(log_string, finish_item_addon_level, "finishitemaddonlevel");
		server_log::push_log_string(log_string, finish_item_culturing_percent, "finishitemculturingpercent", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_advance_equip_logger, log_string);
		

#endif
	}

	void server_log::resurrection_log(int32 map_id, int32 mode, int32 old_jewel_num, int32 new_jewel_num)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_RESURRECTION);

		log_string += "{";
		server_log::push_log_string(log_string, "resurrection", "logname");
		server_log::push_log_string(log_string, map_id, "mapid");
		server_log::push_log_string(log_string, cur_time, "time");
		server_log::push_log_string(log_string, mode, "mode");
		server_log::push_log_string(log_string, old_jewel_num, "oldjewelnum");
		server_log::push_log_string(log_string, new_jewel_num, "newjewelnum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_resurrection_logger, log_string);
		

#endif
	}

	void server_log::send_mail_log(int64 target_player_guid, int64 sender_player_guid, std::string title, std::string content_text, int32 money_typ1, int32 money_num1, int32 money_typ2, int32 money_num2, const s_item_info* item_list, int32 item_type_num)
	{
#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_SEND_MAIL);

		log_string += "{";
		server_log::push_log_string(log_string, "sendmail", "logname");
		server_log::push_log_string(log_string, target_player_guid, "targetguid");
		server_log::push_log_string(log_string, sender_player_guid, "senderguid");
		server_log::push_log_string(log_string, title, "title");
		server_log::push_log_string(log_string, content_text, "contenttext");
		server_log::push_log_string(log_string, money_typ1, "moneytyp1");
		server_log::push_log_string(log_string, money_num1, "moneynum1");
		server_log::push_log_string(log_string, money_typ2, "moneytyp2");
		server_log::push_log_string(log_string, money_num2, "moneynum2");
		int32 num = 1;
		for (int32 i = 0; i < item_type_num; i++)
		{
			if (item_list[i].item_guid.is_valid())
			{
				std::string itemid = "itemguid";
				std::string itemnum = "itemnum";
				itemid += std::to_string(num);
				itemnum += std::to_string(num);
				server_log::push_log_string(log_string, item_list[i].item_guid.server_64, itemid);
				server_log::push_log_string(log_string, item_list[i].data_ary[e_item_info_stack_count], itemnum);
				num++;
			}
		}
		server_log::push_log_string(log_string, item_type_num, "itemtypenum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_send_mail_logger, log_string);
		

#endif
	}

	void server_log::first_recharge_log(int64 player_guid, bool is_first_recharge)
	{
#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_FIRST_RECHARGE);

		log_string += "{";
		server_log::push_log_string(log_string, "firstrecharge", "logname");
		server_log::push_log_string(log_string, player_guid, "isfirstrecharge");
		server_log::push_log_string(log_string, is_first_recharge, "isfirstrecharge", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_first_recharge_logger, log_string);
		

#endif
	}

	void server_log::recharge_log(login_fixed_data& third_info, s_unit_info& role_info, int64 jewel_num, std::string order_id_str, float add_money_value, int32 add_jewel_value, int32 recharge_template_id, bool is_first_recharge, int32 fixed_payment_type, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_RECHARGE_SUCCESS);

		//log_string += "{";
		//server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, third_info.param9, "idfa"); 									//idfa
		//server_log::push_log_string(log_string, third_info.param10, "imei"); 									//imei
		//server_log::push_log_string(log_string, third_info.param11, "androidId"); 									//androidId
		//server_log::push_log_string(log_string, third_info.param12, "mac"); 									//mac
		//server_log::push_log_string(log_string, third_info.param13, "sn", true); 									//sn

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_recharge_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_role_id;
		temp_role_id << role_info.role_guid.server_64;
		std::string return_role_id;
		temp_role_id >> return_role_id;

		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = return_role_id;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["ip"] = third_info.param6;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];
		json["game_order_id"] = "";
		json["game_channel_order_id"] = order_id_str;
		json["order_amount"] = add_money_value;
		json["share_amount"] = 0;
		json["no_share_amount"] = 0;
		json["pay_id"] = 0;

		if (fixed_payment_type >= 0)
		{
			json["recharge_channel"] = to_string(fixed_payment_type).c_str();
		}
		else
		{
			json["recharge_channel"] = third_info.param14;
		}

		json["add_jewel"] = add_jewel_value;
		json["total_jewel"] = jewel_num;
		json["currency_type"] = 1;
		json["item_id"] = recharge_template_id;
		json["is_first_recharge"] = is_first_recharge ? 1 : 0;
		json["device_model"] = third_info.param8;
		json["android_id"] = third_info.param11;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_RECHARGE_SUCCESS, str_db_log);
#endif
	}

	void server_log::today_recharge_log(int32 recharge_num, int32 total_num, int32 vip_level)
	{
#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_TODAY_RECHARGE);

		log_string += "{";
		server_log::push_log_string(log_string, "recharge", "logname");
		server_log::push_log_string(log_string, recharge_num, "rechargenum");
		server_log::push_log_string(log_string, total_num, "totalnum");
		server_log::push_log_string(log_string, vip_level, "viplevel", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_today_recharge_logger, log_string);
		

#endif
	}

	void server_log::recharge_step_log(xstring order_id, guid_64 role_id, int32 goods_id, int32 step_num, login_fixed_data& third_info, int32 login_type, const s_unit_info * role_info_ptr)
	{

#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_RECHARGE_STEP);

		log_string += "{";
		server_log::push_log_string(log_string, "rechargestep", "logname");
		server_log::push_log_string(log_string, order_id, "order_id");
		server_log::push_log_string(log_string, role_id.server_64, "role_id");
		server_log::push_log_string(log_string, goods_id, "goods_id");
		server_log::push_log_string(log_string, step_num, "setp_num", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_recharge_step_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_role_id;
		temp_role_id << role_id.server_64;
		std::string return_role_id;
		temp_role_id >> return_role_id;

		std::stringstream temp_goods_id;
		temp_goods_id << goods_id;
		std::string return_goods_id;
		temp_goods_id >> return_goods_id;
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = log_time;
		json["order_id"] = order_id;
		json["role_id"] = return_role_id;
		json["goods_id"] = return_goods_id;
		json["step_num"] = step_num;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_RECHARGE_STEP, str_db_log);


		set_log_var(log_head)
		if (role_info_ptr == nullptr)
		{
		set_log_common_head_part3(log_head, "", SERVERCONFIG->game_id, utility::get_tick_count())
		set_log_common_head_part4(log_head, role_id, "null", 0, 0)
		}
		else
		{
			set_log_common_head_part1(log_head, SERVERCONFIG->app_key, third_info.param7, third_info.param4, third_info.param5);
			set_log_common_head_part2(log_head, third_info.param2, init_unit::change_string_to_i32(third_info.param15));
			set_log_common_head_part3(log_head, role_info_ptr->account, SERVERCONFIG->game_id, utility::get_tick_count());
			set_log_common_head_part4(log_head, role_info_ptr->role_guid, role_info_ptr->role_name, role_info_ptr->data_ary[e_role_info_exp_level], role_info_ptr->data_ary[e_role_info_gender])
			set_log_common_head_part5(log_head, role_info_ptr->data_i64_ary[e_role_i64_info_gs_value], role_info_ptr->data_ary[e_role_info_class_type], role_info_ptr->data_ary[e_role_info_vip_level]);

		}

		server_log::serverRechargeStep(log_head, order_id, return_goods_id, step_num);

		

#endif
	}

	void server_log::recharge_error_log(xstring order_id, guid_64 role_id, int32 goods_id, int32 error_num)
	{

#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_RECHARGE_ERROR);

		log_string += "{";
		server_log::push_log_string(log_string, "rechargeerror", "logname");
		server_log::push_log_string(log_string, order_id, "order_id");
		server_log::push_log_string(log_string, role_id.server_64, "goods_id");
		server_log::push_log_string(log_string, goods_id, "goods_id");
		server_log::push_log_string(log_string, error_num, "error_num", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_recharge_error_logger, log_string);
		

#endif
	}

	void server_log::vip_log(int32 old_vip_level, bool is_get_old_welfare, int32 cur_vip_level)
	{
#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_VIP);

		log_string += "{";
		server_log::push_log_string(log_string, "recharge", "logname");
		server_log::push_log_string(log_string, old_vip_level, "rechargenum");
		server_log::push_log_string(log_string, is_get_old_welfare, "totalnum");
		server_log::push_log_string(log_string, cur_vip_level, "viplevel", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_vip_logger, log_string);
		

#endif
	}

	void server_log::jewel_consume_log(int32 consume_num, int32 consume_type, int32 total_consume_num)
	{
#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_JEWEL_CONSUME_LOGGER);

		log_string += "{";
		server_log::push_log_string(log_string, "jewelconsume", "logname");
		server_log::push_log_string(log_string, consume_num, "consumenum");
		server_log::push_log_string(log_string, consume_type, "causeid");
		server_log::push_log_string(log_string, total_consume_num, "totalconsumenum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_jewel_consume_logger, log_string);
		

#endif
	}

	void server_log::spawn_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos)
	{
#ifdef SHOW_SERVER_LOG
		_RLOG_(MINFO, ::faith::log_detail::format_message("{} {} {} {} {} {} {} {} {} {} {} {} {}", 
			server_common_head, "npc spawn", server_log_normversion, map_id, "tempid", npc_id, spawn_id, array_idx, guid.A, guid.B, bornpos.x, bornpos.y,  bornpos.z));
#endif
	}

	void server_log::remove_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos)
	{
#ifdef SHOW_SERVER_LOG
		_RLOG_(MINFO, ::faith::log_detail::format_message("{} {} {} {} {} {} {} {} {} {} {} {} {}", 
			server_common_head, "npcremove", server_log_normversion, map_id, "tempid", npc_id, spawn_id, array_idx, guid.A, guid.B, bornpos.x, bornpos.y, bornpos.z));
#endif
	}
	void server_log::find_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos)
	{
#ifdef SHOW_SERVER_LOG
		_RLOG_(MINFO, ::faith::log_detail::format_message("{} {} {} {} {} {} {} {} {} {} {} {}", server_common_head, "npcaoierror", server_log_normversion, map_id, "tempid", npc_id, spawn_id, array_idx, guid.server_64, bornpos.x, bornpos.y, bornpos.z));
#endif
	}

	void server_log::dump_npc_log(int32 map_id, int32 npc_id, int32 spawn_id, int32 array_idx, guid_64 guid, fvector bornpos, int32 line_id)
	{
#ifdef SHOW_SERVER_LOG
		_RLOG_(MINFO, ::faith::log_detail::format_message("{} {} {} {} {} {} {} {} {} {} {} {}", server_common_head, "npcdump", server_log_normversion, map_id, npc_id, spawn_id, array_idx, guid.server_64, bornpos.x, bornpos.y, bornpos.z, line_id));
#endif
	}

	void server_log::arena_challenge_log(login_fixed_data& third_info, s_unit_info& role_info)

	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ARENA_CHALLENGE);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "arenachallenge", "logname");
		server_log::push_log_string(log_string, "C1010", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime", true);

		log_string += "}}";
		
		//PROJECT_RECORD_LOG(g_arena_challenge_logger, log_string);
		
#endif
	}

	void server_log::chat_log(login_fixed_data& third_info, s_unit_info& sender_info, guid_64& addressee_guid, int32 chat_type, std::string chat_text, s_item_info* item_info, int32 item_num, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();

		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		std::string item_json = server_log_get_item_json(item_info, item_num);

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CHAT);

		log_string += "{";
		server_log::push_log_string(log_string, sender_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "chat", "logname");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, sender_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, sender_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, sender_info.data_ary[e_role_info_vip_level], "vip");			//vip	
		server_log::push_log_string(log_string, chat_type, "chat_type");
		server_log::push_log_string(log_string, addressee_guid.server_64, "addressee");
		server_log::push_log_string(log_string, chat_text, "chat_text");
		server_log::push_log_string(log_string, item_json, "allitemnum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_chat_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		{
			if (log_db_sender_handler)
			{
				server2dp_save_chat_log req;
				req.server_id = server_template_id;
				req.role_guid = sender_info.role_guid.server_64;
				req.third_info = third_info;
				req.login_type = login_type;

				int32 time_str_len = log_time.size();
				if (time_str_len > log_time_str_len)
				{
					time_str_len = log_time_str_len;
				}
				memcpy(req.log_time, log_time.c_str(), time_str_len);

				memcpy(req.role_account, sender_info.account, max_account_length);
				memcpy(req.role_name, sender_info.role_name, max_name_size);
				req.role_level = sender_info.data_ary[e_role_info_exp_level];
				req.target_guid = addressee_guid.server_64;
				req.chat_type = chat_type;

				int32 chat_len = chat_text.size();
				if (chat_len > chat_max_chat_size_with_hyper_link)
				{
					chat_len = chat_max_chat_size_with_hyper_link;
				}
				memcpy(req.chat_content, chat_text.c_str(), chat_len);

				log_db_sender_handler(&req, sizeof(req), 0);
			}


			//Json::Value json;
			//Json::FastWriter writer;
			//json["server_id"] = log_db_server_id;
			//json["log_time"] = log_time;
			//json["account_id"] = sender_info.account;
			//json["role_id"] = init_unit::change_i64_to_string(sender_info.role_guid.server_64);
			//json["role_name"] = sender_info.role_name;
			//json["role_level"] = sender_info.data_ary[e_role_info_exp_level];
			//json["target_id"] = init_unit::change_i64_to_string(addressee_guid.server_64);
			//json["chat_type"] = chat_type;
			//json["chat_content"] = chat_text;

			//xstring str_db_log = writer.write_no_enter(json);
			//send_log_to_db(TABLE_NAME_CHAT_LOG, str_db_log);
		}
#endif
	}

	void server_log::lucky_draw_log(login_fixed_data& third_info, s_unit_info& role_info, 
		std::vector<int32>& item_get_array, int32 treasure_type, int32 cost_type, int32 is_first_time_do, 
		int32 old_lucky_value, int32 new_lucky_value, int32 is_free, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		if (item_get_array.size() <= 0
			|| item_get_array.size() % 2 !=0)
		{
			return;
		}


		xstring cur_time = time_helper::get_current_time();


		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_LUCKY_DRAW_LOG);

		log_string += "{";
		server_log::push_log_string(log_string, cur_time, "cur_time");
		server_log::push_log_string(log_string, role_info.account, "userid");									
		server_log::push_log_string(log_string, third_info.param5, "deviceid");							
		server_log::push_log_string(log_string, third_info.param1, "appid");																				
		server_log::push_log_string(log_string, third_info.param2, "version");																				
		server_log::push_log_string(log_string, server_log_normversion, "normversion");							 
		server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.role_name, "role_name");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, treasure_type, "treasure_type");
		server_log::push_log_string(log_string, cost_type, "cost_type");
		server_log::push_log_string(log_string, is_first_time_do, "is_first_time_do");
		server_log::push_log_string(log_string, old_lucky_value, "old_lucky_value");
		server_log::push_log_string(log_string, new_lucky_value, "new_lucky_value");
		server_log::push_log_string(log_string, is_free, "is_free");
		server_log::push_log_string(log_string, third_info.param6, "ip");											
		server_log::push_log_string(log_string, login_type, "login_type");
		server_log::push_log_string(log_string, "B2110", "stepnumid");
		server_log::push_log_string(log_string, "", "cross_service");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, third_info.param8, "model");							
		server_log::push_log_string(log_string, third_info.param9, "idfa"); 									
		server_log::push_log_string(log_string, third_info.param10, "imei"); 									
		server_log::push_log_string(log_string, third_info.param11, "androidId"); 								
		server_log::push_log_string(log_string, third_info.param12, "mac"); 									
		server_log::push_log_string(log_string, third_info.param13, "sn"); 								
		
		
		for (int32 i = 0; i < item_get_array.size(); i += 2)
		{
			int32 item_id = item_get_array[i];
			int32 item_num = item_get_array[i + 1];
			server_log::push_log_string(log_string, item_id, "item_id");
			server_log::push_log_string(log_string, item_num, "item_num", true);
			log_string += "}}";
			//PROJECT_RECORD_LOG(g_lucky_draw_logger, log_string);
		}

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];

		json["treasure_type"] = treasure_type;
		json["cost_type"] = cost_type;
		json["is_first_time_do"] = is_first_time_do;
		json["old_lucky_value"] = old_lucky_value;
		json["new_lucky_value"] = new_lucky_value;
		json["is_free"] = is_free;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		for (int32 i = 0; i < item_get_array.size(); i += 2)
		{
			int32 item_id = item_get_array[i];
			int32 item_num = item_get_array[i + 1];
			json["item_id"] = item_id;
			json["item_num"] = item_num;

			xstring str_db_log = writer.write_no_enter(json);
			send_log_to_db(TABLE_NAME_LUCKY_DRAW_LOG, str_db_log);
		}
#endif
	}

	void server_log::player_mark_log(login_fixed_data& third_info, s_unit_info& role_info, int32 strength, int32 agility, int32 intellect, int32 stamina)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_PLAYER_MARK);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "playermark", "logname");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");				//vip		
		server_log::push_log_string(log_string, strength, "strengthlevel");
		server_log::push_log_string(log_string, agility, "agilitylevel");
		server_log::push_log_string(log_string, intellect, "intellectlevel");
		server_log::push_log_string(log_string, stamina, "staminalevel", true);

		log_string += "}}";
	
		//PROJECT_RECORD_LOG(g_player_mark_logger, log_string);
		
#endif
	}

	void server_log::player_pk_role_log(login_fixed_data& third_info, s_unit_info& role_info, int32 cur_pk_mode, int32 cur_pk_value)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_PLAYER_PK);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "playerpk", "logname");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");				//vip			
		server_log::push_log_string(log_string, cur_pk_mode, "pk_mode");
		server_log::push_log_string(log_string, cur_pk_value, "pkvalue", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_player_pk_role_logger, log_string);
		
#endif
	}

	void server_log::player_dead_role_log(login_fixed_data& third_info, s_unit_info& killer_role_info, s_unit_info& dead_role_info, fvector dead_pos, int32 killer_pk_mode, int32 dead_pk_mode, int32 map_type, int32 dead_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ROLE_PLAYER_DEAD);

		log_string += "{";
		server_log::push_log_string(log_string, dead_role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");												//appid
		server_log::push_log_string(log_string, "playerdead", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0300", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, dead_role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, dead_role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, dead_role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, killer_role_info.role_name, "killername");
		server_log::push_log_string(log_string, killer_role_info.data_ary[e_role_info_template_id], "killertemplateid");
		server_log::push_log_string(log_string, killer_role_info.data_i64_ary[e_role_i64_info_gs_value], "killertgs");
		server_log::push_log_string(log_string, dead_role_info.role_guid.server_64, "deadguid");
		server_log::push_log_string(log_string, dead_role_info.role_name, "deadname");
		server_log::push_log_string(log_string, dead_role_info.data_ary[e_role_info_template_id], "deadtemplateid");
		server_log::push_log_string(log_string, dead_role_info.data_i64_ary[e_role_i64_info_gs_value], "deadgs");
		server_log::push_log_string(log_string, dead_pos.X, "deathlocationx");
		server_log::push_log_string(log_string, dead_pos.Y, "deathlocationy");
		server_log::push_log_string(log_string, dead_pos.Z, "deathlocationz");
		server_log::push_log_string(log_string, killer_pk_mode, "killerpkmode");
		server_log::push_log_string(log_string, dead_pk_mode, "deadpkmode");
		server_log::push_log_string(log_string, map_type, "scenemap");
		server_log::push_log_string(log_string, dead_type, "deadtype", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_player_dead_role_logger, log_string);
		
#endif
	}
	void server_log::bag_total_num_log(login_fixed_data& third_info, s_unit_info& role_info, int32 cur_bag_total_num)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_NUM_BAG_TOTAL);

		log_string += "{";
		server_log::push_log_string(log_string, "bagtotalnum", "logname");
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");				//vip			
		server_log::push_log_string(log_string, cur_bag_total_num, "curbagtotalnum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_bag_total_num_logger, log_string);
		
#endif
	}

	void server_log::bag_unused_num_log(int32 cur_bag_unused_num)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_NUM_BAG_UNUSED);

		log_string += "{";
		server_log::push_log_string(log_string, "bagunusednum", "logname");
		server_log::push_log_string(log_string, cur_bag_unused_num, "curbagunusednum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_bag_unused_num_logger, log_string);
		
#endif
	}

	void server_log::month_card_log(xstring buy_time, int32 buy_num)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_MONTH_CARD);

		log_string += "{";
		server_log::push_log_string(log_string, "monthcard", "logname");
		server_log::push_log_string(log_string, buy_time, "buytime");
		server_log::push_log_string(log_string, buy_num, "buynum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_month_card_logger, log_string);
		
#endif
	}

	void server_log::element_recovery_log(int32 pound_num)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ELEMENT_RECOVERY);

		log_string += "{";
		server_log::push_log_string(log_string, "elementrecovery", "logname");
		server_log::push_log_string(log_string, pound_num, "pound_num", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_element_recovery_logger, log_string);
		
#endif
	}

	void server_log::world_boss_battle_from_player_log(int32 boss_id, int32 hurt_value, int64 battle_begin_time, int64 battle_end_time, int64 battle_continued_time, int32 boss_is_dead, int32 is_team, int32 pk_mode)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_WORLD_BOSS_BATTLE_FROM_PLAYER);

		log_string += "{";
		server_log::push_log_string(log_string, "worldbossbattlefromplayer", "logname");
		server_log::push_log_string(log_string, boss_id, "bossid");									// bossID
		server_log::push_log_string(log_string, hurt_value, "damagevalue");
		server_log::push_log_string(log_string, battle_begin_time, "battlebegintime");
		server_log::push_log_string(log_string, battle_end_time, "battleendtime");
		server_log::push_log_string(log_string, battle_continued_time, "battlecontinuoustime");
		server_log::push_log_string(log_string, boss_is_dead, "bossisdead");
		server_log::push_log_string(log_string, is_team, "isteam");
		server_log::push_log_string(log_string, pk_mode, "pkmode", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_world_boss_battle_from_player_logger, log_string);
		
#endif
	}

	void server_log::world_boss_battle_from_boss_log(login_fixed_data& third_info, s_unit_info& role_info ,int32 boss_id, int32 map_id, int32 player_num, int32 battle_begin_time, int32 battle_end_time, int32 battle_continued_time,int32 login_type)
	{ 
#ifdef SHOW_SERVER_LOG
		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_BOSS_KILL_LOG);

		//log_string += "{";
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, boss_id, "bossid");											//bossID

		//log_string += "}}";
		
		//PROJECT_RECORD_LOG(g_world_boss_battle_from_boss_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["boss_id"] = boss_id;
		json["map_id"] = map_id;
		json["player_num"] = player_num;
		json["battle_begin_time"] = time_helper::get_str_time_by_stamp(battle_begin_time);
		json["battle_end_time"] = time_helper::get_str_time_by_stamp(battle_end_time);;
		json["battle_continued_time"] = battle_continued_time;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_BOSS_KILL_LOG, str_db_log);
#endif
	}
	void server_log::cross_server_boss_kill_log(int32 boss_id, guid_64 legion_id, xstring legion_name,int32 server_id, guid_64 legion_id_top1,xstring legion_name_top1, int32 legion_server_id_top1, guid_64 legion_id_top2, xstring legion_name_top2, int32 legion_server_id_top2, guid_64 legion_id_top3, xstring legion_name_top3, int32 legion_server_id_top3)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG);

		log_string += "{";
		server_log::push_log_string(log_string, "crossserverworldbosskill", "logname");
		server_log::push_log_string(log_string, boss_id, "bossid");								//boss_id
		server_log::push_log_string(log_string, legion_id.server_64, "legionid");
		server_log::push_log_string(log_string, server_id, "serverid");
		server_log::push_log_string(log_string, legion_name, "legionname");
		server_log::push_log_string(log_string, legion_id_top1.server_64, "legionidtop1");
		server_log::push_log_string(log_string, legion_name_top1, "legionnametop1");
		server_log::push_log_string(log_string, legion_server_id_top1, "legionserveridtop1");
		server_log::push_log_string(log_string, legion_id_top2.server_64, "legionidtop2");
		server_log::push_log_string(log_string, legion_name_top2, "legionnametop2");
		server_log::push_log_string(log_string, legion_server_id_top2, "legionserveridtop2");
		server_log::push_log_string(log_string, legion_id_top3.server_64, "legionidtop3");
		server_log::push_log_string(log_string, legion_name_top3, "legionnametop3");
		server_log::push_log_string(log_string, legion_server_id_top3, "legionserveridtop3", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_cross_server_world_boss_kill, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = server_id;
		json["log_time"] = log_time;
		json["boss_id"] = boss_id;
		json["kill_legion_id"] = legion_id.server_64;
		json["kill_legion_name"] = legion_name;

		json["top1_id"] = legion_id_top1.server_64;
		json["top1_name"] = legion_name_top1;
		json["top1_server_id"] = legion_server_id_top1;
		json["top2_id"] = legion_id_top2.server_64;
		json["top2_name"] = legion_name_top2;
		json["top2_server_id"] = legion_server_id_top2;
		json["top3_id"] = legion_id_top3.server_64;
		json["top3_name"] = legion_name_top3;
		json["top3_server_id"] = legion_server_id_top3;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_CROSS_SERVER_BOSS_KILL_LOG, str_db_log);
#endif
	}

	void server_log::boss_kill_drop_log(login_fixed_data& third_info, s_unit_info& role_info, std::vector< s_item_template_info >& item_list, int32 boss_id, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_BOSS_KILL_DROP_LOG);

		log_string += "{";
		server_log::push_log_string(log_string, "boss_kill_drop_log", "logname");
		server_log::push_log_string(log_string, role_info.role_guid, "killerguid");
		server_log::push_log_string(log_string, role_info.role_name, "rolename");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelv");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "rolepower");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "rolevip");
		server_log::push_log_string(log_string, boss_id, "bossid");											//bossID

		xstring item_list_str = "";
		int32 item_size = item_list.size();
		std::map<int32, int32> item_map;
		for (int32 i = 0 ; i < item_size;i++)
		{
			item_list_str.append(to_string(item_list[i].m_item_id));
			item_list_str.append(",");
			int32 m_id = item_list[i].m_item_id;
			if (item_map.find(m_id) == item_map.end())
			{
				item_map.insert(std::pair<int32, int32>(m_id, 1));
			}
			else 
			{
				int32 cur_num = item_map[m_id];
				item_map[m_id] = cur_num + 1;
			}
		}
		server_log::push_log_string(log_string, item_list_str, "drop_item_list", true);
		
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_world_boss_kill_drop, log_string);

		xstring log_time = time_helper::get_current_time();
		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];
		json["boss_id"] = boss_id;

		int32 m_idx = 1;
		for (map<int32, int32>::iterator iter = item_map.begin(); iter != item_map.end(); iter++)
		{
			if (m_idx > 20)
			{
				break;
			}
			json[std::string("drop_item_id") + to_string(m_idx)] = iter->first;
			json[std::string("drop_item_num") + to_string(m_idx)] = iter->second;
			m_idx++;
		}
		//for (int32 i = m_idx;i <= 20;i++)
		//{
		//	json[std::string("drop_item_id") + to_string(i)] = 0;
		//	json[std::string("drop_item_num") + to_string(i)] = 0;
		//}

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_BOSS_KILL_DROP_LOG, str_db_log);
#endif
	}
	void server_log::elite_boss_battle_from_player_log(int32 boss_id, int32 hurt_value, int64 battle_begin_time, int64 battle_end_time, int64 battle_continued_time, int32 boss_is_dead, int32 is_team, int32 pk_mode)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ELITE_BOSS_BATTLE_FROM_PLAYER);

		log_string += "{";
		server_log::push_log_string(log_string, "elitebossbattlefromplayer", "logname");
		server_log::push_log_string(log_string, boss_id, "bossid");								// bossID
		server_log::push_log_string(log_string, hurt_value, "hurtvalue");
		server_log::push_log_string(log_string, battle_begin_time, "battlebegintime");
		server_log::push_log_string(log_string, battle_end_time, "battleendtime");
		server_log::push_log_string(log_string, battle_continued_time, "battlecontinuoustime");
		server_log::push_log_string(log_string, boss_is_dead, "bossisdead");
		server_log::push_log_string(log_string, is_team, "isteam");
		server_log::push_log_string(log_string, pk_mode, "pkmode", true);

		log_string += "}}";
	
		//PROJECT_RECORD_LOG(g_elite_boss_battle_from_player_logger, log_string);
		
#endif
	}

	void server_log::elite_boss_battle_from_boss_log(int32 boss_id, int32 player_num, int64 battle_begin_time, int64 battle_end_time, int64 battle_continued_time)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ELITE_BOSS_BATTLE_FROM_BOSS);

		log_string += "{";
		server_log::push_log_string(log_string, "elitebossbattlefromboss", "logname");
		server_log::push_log_string(log_string, boss_id, "bossid");								// bossID
		server_log::push_log_string(log_string, player_num, "playernum");
		server_log::push_log_string(log_string, battle_begin_time, "battlebegintime");
		server_log::push_log_string(log_string, battle_end_time, "battleendtime");
		server_log::push_log_string(log_string, battle_continued_time, "battlecontinuoustime", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_elite_boss_battle_from_boss_logger, log_string);
		
#endif
	}

	void server_log::boss_home_battle_from_boss_log(const s_gain_treasure_record_info_one& record_info)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ELITE_BOSS_BATTLE_FROM_BOSS);

		log_string += "{";
		server_log::push_log_string(log_string, "bosshome", "logname");
		server_log::push_log_string(log_string, record_info.boss_spawn_point_template_id, "bossspawnid");			// bossID
		server_log::push_log_string(log_string, record_info.killed_stamp, "killedtime");
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			if (!record_info.player_info_arr[i].role_guid.is_valid())
			{
				break;
			}
			server_log::push_log_string(log_string, record_info.player_info_arr[i].role_guid, "roleguid");		
		}
		server_log::push_log_string(log_string, "onerecordend", "bosshomerecord", true);	
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_elite_boss_battle_from_boss_logger, log_string);

#endif
	}
	void server_log::activity_degree_log(s_unit_info& role_info, int32 new_activity_degree_value)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_ACTIVITY_DEGREE);

		log_string += "{";
		server_log::push_log_string(log_string, "activitydegree", "logname");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleguid");
		server_log::push_log_string(log_string, new_activity_degree_value, "activitydegreevalue", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_activity_degree_logger, log_string);
		
#endif
	}

	void server_log::resource_find_back_log(login_fixed_data& third_info, s_unit_info& role_info, int32 must_do_type, int32 find_back_type, int32 find_back_value)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_RESOURCE_FIND_BACK);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "resourcefindback", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0700", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, must_do_type, "mustdotype");
		server_log::push_log_string(log_string, find_back_type, "findbacktype");
		server_log::push_log_string(log_string, find_back_value, "findbackvalue", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_resource_find_back_logger, log_string);
		
#endif
	}

	void server_log::legion_burn_fire_over_log(guid_64 legion_id, int32 participate_number)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_LEGION_BURN_FIRE_OVER);

		log_string += "{";
		server_log::push_log_string(log_string, "legionburnfireover", "logname");
		server_log::push_log_string(log_string, legion_id, "legionid");
		server_log::push_log_string(log_string, participate_number, "participatenumber", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_legion_burn_fire_over_logger, log_string);
#endif
	}

	void server_log::legion_boss_over_log(guid_64 legion_id, int32 participate_number)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_LEGION_BOSS_OVER);

		log_string += "{";
		server_log::push_log_string(log_string, "legionbossover", "logname");
		server_log::push_log_string(log_string, legion_id, "legionid");
		server_log::push_log_string(log_string, participate_number, "participatenumber", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_legion_boss_over_logger, log_string);
		
#endif
	}

	void server_log::legion_war_over_log(guid_64 legion_id, int32 participate_number)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_LEGION_WAR_OVER);

		log_string += "{";
		server_log::push_log_string(log_string, "legionwarover", "logname");
		server_log::push_log_string(log_string, legion_id, "legionid");
		server_log::push_log_string(log_string, participate_number, "participatenumber", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_legion_war_over_logger, log_string);
		
#endif
	}

	void server_log::legion_info_log(guid_64 legion_id, xstring legion_name, int32 legion_level, int32 legion_role_num)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_LEGION_INFO);

		log_string += "{";
		server_log::push_log_string(log_string, "legioninfo", "logname");
		server_log::push_log_string(log_string, legion_id, "legionid");
		server_log::push_log_string(log_string, legion_name, "legionname");
		server_log::push_log_string(log_string, legion_level, "legionlevel");
		server_log::push_log_string(log_string, legion_role_num, "legionrolenum", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_legion_info_logger, log_string);
		
#endif
	}

	void server_log::create_team_log(guid_64 creater_role_guid, int32 team_aim)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CREATE_TEAM);

		log_string += "{";
		server_log::push_log_string(log_string, "createteam", "logname");
		server_log::push_log_string(log_string, creater_role_guid.server_64, "createrroleguid");
		server_log::push_log_string(log_string, team_aim, "teamaim", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_create_team_logger, log_string)
		
#endif
	}

	void server_log::join_team_log(guid_64 creater_role_guid, int32 cur_member_num, int32 team_aim, guid_64 joiner_role_guid)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CREATE_TEAM);

		log_string += "{";
		server_log::push_log_string(log_string, "jointeam", "logname");
		server_log::push_log_string(log_string, creater_role_guid.server_64, "createrroleguid");
		server_log::push_log_string(log_string, cur_member_num, "curmembernum");
		server_log::push_log_string(log_string, team_aim, "teamaim");
		server_log::push_log_string(log_string, joiner_role_guid.server_64, "joinerroleguid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_join_team_logger, log_string)
			
#endif
	}

	void server_log::quit_team_log(guid_64 creater_role_guid, int32 cur_member_num, int32 team_aim, guid_64 joiner_role_guid, int32 is_exist)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_QUIT_TEAM);

		log_string += "{";
		server_log::push_log_string(log_string, "quitteam", "logname");
		server_log::push_log_string(log_string, creater_role_guid.server_64, "createrroleguid");
		server_log::push_log_string(log_string, cur_member_num, "curmembernum");
		server_log::push_log_string(log_string, team_aim, "teamaim");
		server_log::push_log_string(log_string, joiner_role_guid.server_64, "joinerroleguid");
		server_log::push_log_string(log_string, is_exist, "isexist", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_quit_team_logger, log_string)
			
#endif
	}

	void server_log::chat_channel_log(int32 channel_id, xstring chat_content, guid_64 sender_guid, guid_64 receiver_guid, xstring voice_id, int32 voice_time)
	{
#ifdef SHOW_SERVER_LOG
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CHAT_CHANNEL);

		log_string += "{";
		server_log::push_log_string(log_string, "chatchannel", "logname");
		server_log::push_log_string(log_string, channel_id, "channelid");
		server_log::push_log_string(log_string, chat_content, "chatcontent");
		server_log::push_log_string(log_string, sender_guid.server_64, "senderguid");
		server_log::push_log_string(log_string, receiver_guid.server_64, "receiverguid");
		server_log::push_log_string(log_string, voice_id, "voiceid");
		server_log::push_log_string(log_string, voice_time, "voicetime", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_chat_channel_logger, log_string)
			
#endif
	}
	void server_log::grow_up_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 grow_up_id, int32 grow_up_cost)
	{

#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_GROW_UP);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "growup", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C5050", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "combatpower");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "vip");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, grow_up_id, "growupid");
		server_log::push_log_string(log_string, grow_up_cost, "growupcost");
		server_log::push_log_string(log_string, 1, "buynum");
		server_log::push_log_string(log_string, third_info.param7, "rechargechannel");
		server_log::push_log_string(log_string, third_info.param6, "ip");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, third_info.param8, "model");
		server_log::push_log_string(log_string, third_info.param9, "idfa"); 									//idfa
		server_log::push_log_string(log_string, third_info.param10, "imei"); 									//imei
		server_log::push_log_string(log_string, third_info.param11, "androidId"); 								//androidId
		server_log::push_log_string(log_string, third_info.param12, "mac"); 									//mac
		server_log::push_log_string(log_string, third_info.param13, "sn", true); 								//sn


		log_string += "}}";

		//PROJECT_RECORD_LOG(g_grow_up_logger, log_string);

		xstring log_time = time_helper::get_current_time();
		std::stringstream temp_role_id;
		temp_role_id << role_info.role_guid.server_64;
		std::string return_role_id;
		temp_role_id >> return_role_id;

		// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = log_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = return_role_id;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];
		json["purchase_growth_fund_id"] = grow_up_id;
		json["purchase_cost"] = grow_up_cost;
		json["purchase_quantity"] = 1;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_GROW_UP, str_db_log);
#endif
	}

	void server_log::grade_up_log(s_unit_info& role_info, int32 old_class, int32 new_class)
	{
#ifdef SHOW_SERVER_LOG

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_GRADE_UP);

		log_string += "{";
		server_log::push_log_string(log_string, "gradeup", "logname");
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, old_class, "oldclass");
		server_log::push_log_string(log_string, new_class, "newclass", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_grade_up_logger, log_string);
		
#endif
	}

	void server_log::getmeditation_log(login_fixed_data& third_info, s_unit_info& role_info, int32 meditation_type, int32 meditation_time)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_GETMEDITATION);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "meditation", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0600", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, meditation_type, "meditationtype");
		server_log::push_log_string(log_string, meditation_time, "meditationtime", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_getmeditation_logger, log_string)
		
#endif
	}

	void server_log::getwelfare_log(login_fixed_data& third_info, s_unit_info& role_info, int32 in_template_type, int32 in_template_index)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_GETWELFARE);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "succinctequip", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C0500", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, in_template_type, "intemplatetype");
		server_log::push_log_string(log_string, in_template_index, "intemplateindex", true);

		log_string += "}}";
		
		//PROJECT_RECORD_LOG(g_getwelfare_logger, log_string);
		
#endif
	}

	void server_log::map_transfer_log(login_fixed_data& third_info, s_unit_info& role_info, int32 desc_type, int32 desc_map_template_id, int32 line_id, s_map_pos map_pos, guid_64 map_guid, int32 war_index, bool force, int32 dest_group_id)
	{
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_MAP_TRANSFER);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "maptransfer", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C1000", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, desc_type, "desctype");
		server_log::push_log_string(log_string, desc_map_template_id, "mapid");
		server_log::push_log_string(log_string, line_id, "lineid");
		server_log::push_log_string(log_string, map_pos.unit_location.x, "mapx");
		server_log::push_log_string(log_string, map_pos.unit_location.y, "mapy");
		server_log::push_log_string(log_string, map_pos.unit_location.z, "mapz");
		server_log::push_log_string(log_string, map_guid.server_64, "mapguid");
		server_log::push_log_string(log_string, war_index, "warindex");
		server_log::push_log_string(log_string, force, "force");
		server_log::push_log_string(log_string, dest_group_id, "destgroupid", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_map_transfer_logger, log_string);
	}
	void server_log::map_change_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 map_id, int32 map_type, int32 map_sub_type, int32 change_type, int32 stage_type)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_MAP_CHANGE);

		//log_string += "{";
		//server_log::push_log_string(log_string,		third_info.param1,							"appid");			//appid
		//server_log::push_log_string(log_string,   role_info.data_ary[e_role_info_template_id], "career");


		//server_log::push_log_string(log_string,		third_info.param9,							"idfa"); 			//idfa
		//server_log::push_log_string(log_string,		third_info.param10,							"imei"); 			//imei
		//server_log::push_log_string(log_string,		third_info.param11,							"androidId"); 		//androidId
		//server_log::push_log_string(log_string,		third_info.param12,							"mac"); 			//mac
		//server_log::push_log_string(log_string,		third_info.param13,							"sn"); 				//sn
		//server_log::push_log_string(log_string,		third_info.param6,							"ip", true);		//ip

		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_map_change_logger, log_string);

				// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["map_template_id"] = map_id;
		json["map_type"] = map_type;
		json["map_sub_type"] = map_sub_type;
		json["change_type"] = change_type;
		json["stage_type"] = stage_type;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_MAP_CHANGE, str_db_log);
#endif
	}

#if 0
	struct s_auction_share_info
	{
		guid_64			item_guid;
		guid_64			role_guid;
		float			share_rate;
		xchar			share_mail_title[max_mail_title_length + 1];
		xchar			share_mail_content[max_text_contents_length + 1];
#endif


	void server_log::time_limit_activity_log_zw(login_fixed_data& third_info, s_unit_info& role_info, int32 activity_id, int32 activity_template_id, int32 activity_cycle, int32 activity_day, int32 activity_index, int32 money_type, int32 money_value, std::vector<int32> item_array)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_TIME_LIMIT_ACTIVITY);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "timelimitactivity", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");

		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");

		server_log::push_log_string(log_string, activity_id, "activityid");
		server_log::push_log_string(log_string, activity_template_id, "activitytemplateid");
		server_log::push_log_string(log_string, activity_cycle, "activitycycle");
		server_log::push_log_string(log_string, activity_day, "activityday");
		server_log::push_log_string(log_string, activity_index, "activityindex");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, money_value, "moneyvalue");
		if (item_array.size() > 0)
		{
			int32 num = 1;
			for (int32 i = 0; i < item_array.size() - 1; i = i + 2)
			{
				std::string itemid = "itemid";
				std::string itemcount = "itemcount";
				itemid += std::to_string(num);
				itemcount += std::to_string(num);
				server_log::push_log_string(log_string, item_array[i], itemid);
				server_log::push_log_string(log_string, item_array[i + 1], itemcount);
				num++;
			}
		}
		
		log_string.pop_back();
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_time_limit_activity_logger, log_string);

#endif
	}

	void server_log::rank_list_log(ranking_list* rank_list, int32 rank_list_type)
	{
#ifdef SHOW_SERVER_LOG
		if (nullptr == rank_list)
		{
			return;
		}

		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		
		std::string f_table_name = TABLE_NAME_RANK_LIST + "_";
		f_table_name = f_table_name + init_unit::change_i32_to_string(rank_list_type);

		server_log::push_hadooplog_string(log_string, f_table_name);

		log_string += "{";
		
		server_log::push_log_string(log_string, "ranklist", "logname");
		int32 role_num = 0;

		server_log::push_log_string(log_string, rank_list_type);
		for (ranking_list_ite ite = rank_list->begin(); ite != rank_list->end(); ++ite,++role_num)
		{
			server_log::push_log_string(log_string, ite->role_guid.server_64, "roleid");
			server_log::push_log_string(log_string, ite->role_name, "rolename");
			server_log::push_log_string(log_string, ite->ranking_value, "rankingvalue", true);
			if (role_num >= 100)
			{
				break;
			}
		}
	
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_rank_list_logger, log_string);
		int32 ranking_index = 1;
		// send log to db
		std::string table_name = TABLE_NAME_RANK_LIST + "_";
		table_name = table_name + init_unit::change_i32_to_string(rank_list_type);
		for (ranking_list_ite ite = rank_list->begin(); ite != rank_list->end(); ++ite, ++ranking_index)
		{
			Json::Value json;
			Json::FastWriter writer;
			json["server_id"] = ite->server_id;
			json["log_time"] = cur_time;
			json["role_id"] = ite->role_guid.server_64;
			json["role_name"] = ite->role_name;
			json["role_vip"] = ite->vip_level;
			json["ranking_type"] = ite->ranking_type;
			json["ranking_value"] = ite->ranking_value;
			json["ranking_time"] = ite->save_time;
			json["supporting_guid"] = ite->supporting_guid.server_64;
			json["supporting_name"] = ite->supporting_name;
			json["vip_level"] = ite->vip_level;
			json["peak"] = ite->Peak;
			json["gamechannel"] = ite->game_channel;
			json["ranking_index"] = ranking_index;

			xstring str_db_log = writer.write_no_enter(json);
			send_log_to_db(table_name, str_db_log);

			
			set_log_var(log_head);
			serverRankList(log_head, *ite, ranking_index);
			if (ranking_index >= 100)
				break;
		}
#endif
	}

	void server_log::dump_location_unit(int32* unit_array)
	{
#ifdef SHOW_SERVER_LOG
		std::stringstream ss;
		if (unit_array[0] > 0)
		{
			for (int32 i = 0; i <= unit_array[0]; ++i)
			{
				if (unit_array[i] > 0)
				{
					ss << server_log_trans(unit_array[i]) << "|";
				}
			}
		}
		std::string log_string;
		ss >> log_string;

		PROJECT_RECORD_LOG(g_game_logger, log_string);
#endif
	}
	void server_log::dump_location_watch(const unit_index_map& aoi_tower_watch)
	{
#ifdef SHOW_SERVER_LOG
		if (aoi_tower_watch.size() <= 0)
		{
			return;
		}
		std::stringstream ss;

		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			if (unit_index > 0)
			{
				ss << server_log_trans(unit_index) << "|";
			}
		}
		std::string log_string;
		ss >> log_string;
		PROJECT_RECORD_LOG(g_game_logger, log_string);
#endif
	}
	void server_log::dump_msg_info(int32 msg_header, int32 msg_num, int64 msg_size)
	{
#ifdef SHOW_SERVER_LOG

		_RLOG_(MINFO, ::faith::log_detail::format_message("msgheader:{} msg_num:{} msg_size:{}",  msg_header,  msg_num,  msg_size));
#endif
	}
	void server_log::dump_msg_info_client(int32 msg_header, int32 msg_num, int64 msg_size)
	{
#ifdef SHOW_SERVER_LOG
		_RLOG_(MINFO, ::faith::log_detail::format_message("msgheader:{} msg_num:{} msg_size:{}",  msg_header,  msg_num,  msg_size));
#endif
	}
	void server_log::message_minor_header_verstion_error(int32 server_verstion, int32 client_verstion)
	{
#ifdef SHOW_SERVER_LOG
		_RLOG_(MINFO, ::faith::log_detail::format_message("serververstion:{} clientverstion:{}",  server_verstion,  client_verstion));
#endif
	}

	void server_log::auctionbid_log(login_fixed_data& third_info, s_unit_info& role_info, const int32& money_type, const int32& money_value, const int32& item_template_id, const int32& data_num, const guid_64& buyer_guid)
	{
#ifdef SHOW_SERVER_LOG

		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_AUCTIONBID);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "auctionbidlog", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C1020", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, buyer_guid.server_64, "buyerid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, item_template_id, "itemid");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, money_value, "moneyvalue");
		server_log::push_log_string(log_string, data_num, "datanum");
		server_log::push_log_string(log_string, 0, "systemdeduction", true);
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_auctionbid_logger, log_string);
#endif
	}

	void server_log::auction_buy_log(login_fixed_data& third_info, s_unit_info& role_info, int32 item_id, int32 item_count, int32 money_type, int32 money_count)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		xstring cur_time = time_helper::get_cur_time();
		int64 cur_time_int = utility::get_tick_count();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_AUTION_BUY);

		log_string += "{";
		server_log::push_log_string(log_string, role_info.account, "userid");
		server_log::push_log_string(log_string, third_info.param5, "deviceid");
		server_log::push_log_string(log_string, cur_time_int, "msec");
		server_log::push_log_string(log_string, third_info.param1, "appid");									//appid
		server_log::push_log_string(log_string, "auctionbuy", "logname");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "C1000", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, game_server, "serverid");
		server_log::push_log_string(log_string, third_info.param7, "gamechannel");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "roleid");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "rolelevel");
		server_log::push_log_string(log_string, "0", "combatpower");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, item_id, "itemid");
		server_log::push_log_string(log_string, item_count, "itemcount");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, money_count, "moneycount", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_auction_buy_logger, log_string);
#endif
	}

	void server_log::auction_sell_log(int32 item_id, int32 item_num, int32 money_type, int32 item_price)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_AUCTION_SELL);

		log_string += "{";
		server_log::push_log_string(log_string, "auctionsell", "logname");
		server_log::push_log_string(log_string, item_id, "itemid");
		server_log::push_log_string(log_string, item_num, "itemnum");
		server_log::push_log_string(log_string, cur_time, "dtime");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, item_price, "itemprice", true);

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_auction_sell_logger, log_string);
#endif
	}

	void server_log::legion_auction_log(int64 item_guid, int32 item_num, int32 money_type, int32 item_price)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_LEGION_AUCTION);

		log_string += "{";
		server_log::push_log_string(log_string, "legionauction", "logname");
		server_log::push_log_string(log_string, item_guid, "itemguid");
		server_log::push_log_string(log_string, item_num, "itemnum");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, item_price, "itemprice");
		server_log::push_log_string(log_string, cur_time, "dtime", true);

		log_string += "}}";
		
		//PROJECT_RECORD_LOG(g_legion_auction_logger, log_string);
#endif
	}

	void server_log::world_auction_log(int64 item_guid, int32 item_num, int32 money_type, int32 item_price)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_WORLD_AUCTION);

		log_string += "{";
		server_log::push_log_string(log_string, "worldauction", "logname");
		server_log::push_log_string(log_string, item_guid, "itemguid");
		server_log::push_log_string(log_string, item_num, "itemnum");
		server_log::push_log_string(log_string, money_type, "moneytype");
		server_log::push_log_string(log_string, item_price, "itemprice");
		server_log::push_log_string(log_string, cur_time, "dtime", true);
		
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_world_auction_logger, log_string);
#endif
	}

	void server_log::auction_db_log(const s_auction_info& auction_info, int32 auction_state, const s_unit_info& role_info, const login_fixed_data& third_info, const int32& login_type)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_current_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_AUCTION_LOG);

		log_string += "{";
		server_log::push_log_string(log_string, SERVERCONFIG->game_id, "server_id");
		server_log::push_log_string(log_string, cur_time, "log_time");
		server_log::push_log_string(log_string, auction_info.seller_guid.server_64, "seller_role_guid");
		server_log::push_log_string(log_string, auction_info.seller_name, "seller_role_name");
		server_log::push_log_string(log_string, auction_info.item_info.item_guid.server_64, "item_guid");
		server_log::push_log_string(log_string, auction_info.item_info.data_ary[e_item_info_info_id], "item_id");
		server_log::push_log_string(log_string, auction_info.item_info.data_ary[e_item_info_stack_count], "item_num");
		server_log::push_log_string(log_string, auction_info.trade_type, "auction_type");
		server_log::push_log_string(log_string, auction_info.buyer_guid.server_64, "buyer_role_guid");
		server_log::push_log_string(log_string, auction_info.buyer_name, "buyer_role_name");
		server_log::push_log_string(log_string, auction_info.sell_money_type, "money_type");
		server_log::push_log_string(log_string, auction_state, "auction_state");
		server_log::push_log_string(log_string, login_type, "login_type");
		server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		server_log::push_log_string(log_string, third_info.param6, "ip");

		switch (auction_state)
		{
		case faith::e_auction_state_start_sell:
		case faith::e_auction_state_cancel:
		{
			server_log::push_log_string(log_string, role_info.account, "seller_account_id");
			server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "seller_role_level");
			if (auction_info.trade_type == e_trading_type_normal_sell)
			{
				server_log::push_log_string(log_string, auction_info.sell_total_money, "money_value");
			}
			else
			{
				server_log::push_log_string(log_string, auction_info.cur_bid_money, "money_value");
			}
		}
		break;
		case faith::e_auction_state_deal:
		case faith::e_auction_state_bid:
		{
			server_log::push_log_string(log_string, role_info.account, "buyer_account_id");
			server_log::push_log_string(log_string, auction_info.cur_bid_money, "money_value");
			server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "buyer_role_level");
		}
		break;
		default:
			break;
		}
		server_log::push_log_string(log_string, login_type, "login_type");
		server_log::push_log_string(log_string, third_info.param1, "appid");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B2110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		server_log::push_log_string(log_string, third_info.param6, "ip");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, "", "cross_service");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, third_info.param8, "model");
		server_log::push_log_string(log_string, third_info.param9, "idfa");
		server_log::push_log_string(log_string, third_info.param10, "imei");
		server_log::push_log_string(log_string, third_info.param11, "androidId");
		server_log::push_log_string(log_string, third_info.param12, "mac");
		server_log::push_log_string(log_string, third_info.param13, "sn", true);
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_auction_logger, log_string);

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["seller_role_guid"] = auction_info.seller_guid.server_64;
		json["seller_role_name"] = auction_info.seller_name;
		json["item_guid"] = auction_info.item_info.item_guid.server_64;
		json["item_id"] = auction_info.item_info.data_ary[e_item_info_info_id];
		json["item_num"] = auction_info.item_info.data_ary[e_item_info_stack_count];
		json["auction_type"] = auction_info.trade_type;
		json["buyer_role_guid"] = auction_info.buyer_guid.server_64;
		json["buyer_role_name"] = auction_info.buyer_name;
		json["money_type"] = auction_info.sell_money_type;
		json["auction_state"] = auction_state;
		json["login_type"] = login_type;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;

		switch (auction_state)
		{
		case faith::e_auction_state_start_sell:
		case faith::e_auction_state_cancel:
		{
			json["seller_account_id"] = role_info.account;
			json["seller_role_level"] = role_info.data_ary[e_role_info_exp_level];
			if (auction_info.trade_type == e_trading_type_normal_sell)
			{
				json["money_value"] = auction_info.sell_total_money;
			}
			else
			{
				json["money_value"] = auction_info.cur_bid_money;
			}
		}
		break;
		case faith::e_auction_state_deal:
		case faith::e_auction_state_bid:
		{
			json["buyer_account_id"] = role_info.account;
			json["money_value"] = auction_info.cur_bid_money;
			json["buyer_role_level"] = role_info.data_ary[e_role_info_exp_level];
		}
		break;
		default:
			break;
		}

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_AUCTION_LOG, str_db_log);
#endif
	}
	void server_log::mail_get_content_log(const s_unit_info& role_info, const login_fixed_data& third_info, guid_64 mail_guid, int32 reward_id, int32 reward_num, std::string mail_title, std::string text_contents, int32 login_type)
	{
#ifdef SHOW_SERVER_LOG
		std::string log_mail_title = template_manager::get_instance().get_str_by_string_template_id(std::atoi(mail_title.c_str()));
		if (log_mail_title.empty())
		{
			log_mail_title = mail_title;
		}
		std::string log_mail_content = template_manager::get_instance().get_str_by_string_template_id(std::atoi(text_contents.c_str()));
		if (log_mail_content.empty())
		{
			log_mail_content = text_contents;
		}
		xstring cur_time = time_helper::get_current_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_MAIL_GET_CONTENT_LOG);

		log_string += "{";
		server_log::push_log_string(log_string, SERVERCONFIG->game_id, "server_id");
		server_log::push_log_string(log_string, cur_time, "log_time");
		server_log::push_log_string(log_string, role_info.account, "account_id");
		server_log::push_log_string(log_string, role_info.role_guid.server_64, "role_id");
		server_log::push_log_string(log_string, role_info.role_name, "role_name");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "role_level");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, reward_id, "reward_id");
		server_log::push_log_string(log_string, reward_num, "reward_num");
		server_log::push_log_string(log_string, mail_guid.server_64, "mail_guid");
		server_log::push_log_string(log_string, log_mail_title.c_str(), "mail_title");
		server_log::push_log_string(log_string, log_mail_content.c_str(), "text_contents");
		server_log::push_log_string(log_string, login_type, "login_type");
		server_log::push_log_string(log_string, third_info.param1, "appid");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B2110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		server_log::push_log_string(log_string, third_info.param6, "ip");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, "", "cross_service");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, third_info.param8, "model");
		server_log::push_log_string(log_string, third_info.param9, "idfa");
		server_log::push_log_string(log_string, third_info.param10, "imei");
		server_log::push_log_string(log_string, third_info.param11, "androidId");
		server_log::push_log_string(log_string, third_info.param12, "mac");
		server_log::push_log_string(log_string, third_info.param13, "sn");
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_mail_content_logger, log_string);

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["reward_id"] = reward_id;
		json["reward_num"] = reward_num;
		json["mail_guid"] = mail_guid.server_64;
		json["mail_title"] = log_mail_title.c_str();
		json["text_contents"] = log_mail_content.c_str();

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_MAIL_GET_CONTENT_LOG, str_db_log);
#endif
	}

	void server_log::jewel_count_log(const s_unit_info& role_info, const s_logic_info& logic_info, const login_fixed_data& third_info, const s_money_info& money_info, const int32& login_type)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_current_time();

		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string role_guid_str;
		temp_stream >> role_guid_str;
		
		//xstring log_string = "{";
		//server_log::push_hadooplog_string(log_string, TABLE_NAME_JEWEL_COUNT_LOG);

		//log_string += "{";
		//server_log::push_log_string(log_string, SERVERCONFIG->game_id, "server_id");
		//server_log::push_log_string(log_string, cur_time, "log_time");
		//server_log::push_log_string(log_string, role_info.account, "account_id");
		//server_log::push_log_string(log_string, third_info.param5, "device_id");
		//server_log::push_log_string(log_string, role_guid_str, "role_id");
		//server_log::push_log_string(log_string, role_info.role_name, "reward_name");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "role_level");
		//server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "role_power");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		//server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "role_vip");

		//server_log::push_log_string(log_string, logic_info.data_ary[e_role_logic_info_jewel_total_recharge], "total_recharge_jewel");
		//server_log::push_log_string(log_string, logic_info.data_ary[e_role_logic_info_jewel_total_consume], "total_consume_jewel");

		//server_log::push_log_string(log_string, login_type, "login_type");
		//server_log::push_log_string(log_string, third_info.param1, "appid");
		//server_log::push_log_string(log_string, third_info.param2, "version");
		//server_log::push_log_string(log_string, "B2110", "stepnumid");
		//server_log::push_log_string(log_string, server_log_normversion, "normversion");
		//server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		//server_log::push_log_string(log_string, third_info.param6, "ip");
		//server_log::push_log_string(log_string, third_info.param4, "adchannel");
		//server_log::push_log_string(log_string, "", "cross_service");
		//server_log::push_log_string(log_string, server_invaild_string, "os_version");
		//server_log::push_log_string(log_string, third_info.param8, "model");
		//server_log::push_log_string(log_string, third_info.param9, "idfa");
		//server_log::push_log_string(log_string, third_info.param10, "imei");
		//server_log::push_log_string(log_string, third_info.param11, "androidId");
		//server_log::push_log_string(log_string, third_info.param12, "mac");
		//server_log::push_log_string(log_string, third_info.param13, "sn");
		//log_string += "}}";

		//PROJECT_RECORD_LOG(g_jewel_count_logger, log_string);

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_guid_str;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["total_recharge_jewel"] = logic_info.data_ary[e_role_logic_info_jewel_total_recharge];
		json["total_consume_jewel"] = logic_info.data_ary[e_role_logic_info_jewel_total_consume];

		json["total_recharge_bind_jewel"] = int32(money_info.all_data_ary[e_money_type_jewel_bind]);
		json["total_consume_bind_jewel"] = int32(money_info.cost_data_ary[e_money_type_jewel_bind]);

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_JEWEL_COUNT_LOG, str_db_log);
#endif
	}

	void server_log::time_limit_activity_log(const s_unit_info& role_info, const login_fixed_data& third_info, const int32& activity_id, const int32& activity_type, const int32& cur_value, const int32& target_value, const int32& reward_id, const int32& reward_num, int32 login_type, int32 cost_money_type, int32 cost_money_num, int32 limit_act_branch_temp_id)
	{
#ifdef SHOW_SERVER_LOG
		xstring cur_time = time_helper::get_current_time();

		std::stringstream temp_stream;
		temp_stream << role_info.role_guid.server_64;
		std::string role_guid_str;
		temp_stream >> role_guid_str;

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_TIME_LIMIT_ACTIVITY);


		TimeLimitActivityBranchTemplate* activity_temp = template_manager::get_instance().get_time_limit_activity_branch_template_ptr(limit_act_branch_temp_id);
		if (activity_temp == nullptr)
		{
			return;
		}

		int32 cost_item_info[10] = {0};
		if (activity_type == e_time_limit_activity_type_item_exchange || activity_type == e_time_limit_activity_type_treasure_sprite || 
			activity_type == e_time_limit_activity_type_treasure_element || activity_type == e_time_limit_activity_type_treasure ||
			activity_type == e_time_limit_activity_type_treasure_royal1 || activity_type == e_time_limit_activity_type_treasure_royal2 ||
			activity_type == e_time_limit_activity_type_treasure_royal3 || activity_type == e_time_limit_activity_type_special_exchange)
		{
			if (activity_temp->Condition.size() > 0 || activity_temp->Condition.size() % 2 == 0)
			{
				for (int32 i = 0; i < 5; i++)
				{
					if (i * 2 + 1 >= activity_temp->Condition.size())
					{
						break;
					}
					cost_item_info[i * 2] = activity_temp->Condition[i * 2];
					cost_item_info[i * 2 + 1] = activity_temp->Condition[i * 2 + 1];
				}
			}
		}
		log_string += "{";
		server_log::push_log_string(log_string, SERVERCONFIG->game_id, "server_id");
		server_log::push_log_string(log_string, cur_time, "log_time");
		server_log::push_log_string(log_string, role_info.account, "account_id");
		server_log::push_log_string(log_string, third_info.param5, "device_id");
		server_log::push_log_string(log_string, role_guid_str, "role_id");
		server_log::push_log_string(log_string, role_info.role_name, "reward_name");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_exp_level], "role_level");
		server_log::push_log_string(log_string, role_info.data_i64_ary[e_role_i64_info_gs_value], "role_power");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_vip_level], "role_vip");

		server_log::push_log_string(log_string, activity_id, "activity_id");
		server_log::push_log_string(log_string, activity_type, "activity_type");
		server_log::push_log_string(log_string, cur_value, "cur_value");
		server_log::push_log_string(log_string, target_value, "target_value");
		server_log::push_log_string(log_string, reward_id, "reward_id");
		server_log::push_log_string(log_string, reward_num, "reward_num");

		server_log::push_log_string(log_string, cost_money_type, "cost_money_type");
		server_log::push_log_string(log_string, cost_money_num, "cost_money_num");

		server_log::push_log_string(log_string, cost_item_info[0], "cost_item_id1");
		server_log::push_log_string(log_string, cost_item_info[1], "cost_item_num1");
		server_log::push_log_string(log_string, cost_item_info[2], "cost_item_id2");
		server_log::push_log_string(log_string, cost_item_info[3], "cost_item_num2");
		server_log::push_log_string(log_string, cost_item_info[4], "cost_item_id3");
		server_log::push_log_string(log_string, cost_item_info[5], "cost_item_num3");
		server_log::push_log_string(log_string, cost_item_info[6], "cost_item_id4");
		server_log::push_log_string(log_string, cost_item_info[7], "cost_item_num4");
		server_log::push_log_string(log_string, cost_item_info[8], "cost_item_id5");
		server_log::push_log_string(log_string, cost_item_info[9], "cost_item_num5");

		server_log::push_log_string(log_string, login_type, "login_type");
		server_log::push_log_string(log_string, third_info.param1, "appid");
		server_log::push_log_string(log_string, third_info.param2, "version");
		server_log::push_log_string(log_string, "B2110", "stepnumid");
		server_log::push_log_string(log_string, server_log_normversion, "normversion");
		server_log::push_log_string(log_string, third_info.param14, "gamechannel");
		server_log::push_log_string(log_string, third_info.param6, "ip");
		server_log::push_log_string(log_string, third_info.param4, "adchannel");
		server_log::push_log_string(log_string, "", "cross_service");
		server_log::push_log_string(log_string, server_invaild_string, "os_version");
		server_log::push_log_string(log_string, third_info.param8, "model");
		server_log::push_log_string(log_string, third_info.param9, "idfa");
		server_log::push_log_string(log_string, third_info.param10, "imei");
		server_log::push_log_string(log_string, third_info.param11, "androidId");
		server_log::push_log_string(log_string, third_info.param12, "mac");
		server_log::push_log_string(log_string, third_info.param13, "sn");
		log_string += "}}";

		//PROJECT_RECORD_LOG(g_activity_limit_time_logger, log_string);

		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = SERVERCONFIG->game_id;
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_guid_str;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["activity_id"] = activity_id;
		json["activity_type"] = activity_type;
		json["cur_value"] = cur_value;
		json["target_value"] = target_value;
		json["reward_id"] = reward_id;
		json["reward_num"] = reward_num;
		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;

		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_TIME_LIMIT_ACTIVITY_LOG, str_db_log);

		set_log_var(log_head);
		set_log_common_head_part1(log_head, SERVERCONFIG->app_key, third_info.param7, third_info.param4 , third_info.param5);
		set_log_common_head_part2(log_head, third_info.param2, init_unit::change_string_to_i32(third_info.param15));
		set_log_common_head_part3(log_head, role_info.account, SERVERCONFIG->game_id, utility::get_tick_count());
		set_log_common_head_part4(log_head, role_info.role_guid, role_info.role_name, role_info.data_ary[e_role_info_exp_level], role_info.data_ary[e_role_info_gender])
		set_log_common_head_part5(log_head,  role_info.data_i64_ary[e_role_i64_info_gs_value], role_info.data_ary[e_role_info_class_type], role_info.data_ary[e_role_info_vip_level]);

		server_log::serverTimeLimitActivity(log_head, cur_value, target_value, reward_id, reward_num, activity_type);


#endif
	}

	void server_log::cross_server_harry_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 oper_type, int32 harry_type, int32 harry_server_id, int32 normal_harry_count, int32 sepcail_harry_count)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_CROSS_SERVER_HARRY);

		log_string += "{";
		server_log::push_log_string(log_string,		role_info.account,								"userid");
		server_log::push_log_string(log_string,		third_info.param5,								"deviceid");
		server_log::push_log_string(log_string,		cur_time_int,									"msec");
		server_log::push_log_string(log_string,		third_info.param1,								"appid");			//appid
		server_log::push_log_string(log_string,		"map_change",									"logname");
		server_log::push_log_string(log_string,		third_info.param2,								"version");
		server_log::push_log_string(log_string,		"C1000",										"stepnumid");
		server_log::push_log_string(log_string,		server_log_normversion,							"normversion");
		server_log::push_log_string(log_string,		game_server,									"serverid");
		server_log::push_log_string(log_string,		third_info.param7,								"gamechannel");
		server_log::push_log_string(log_string,		third_info.param4,								"adchannel");
		server_log::push_log_string(log_string,		role_info.role_guid.server_64,					"roleid");
		server_log::push_log_string(log_string,		role_info.data_ary[e_role_info_exp_level],		"rolelevel");
		server_log::push_log_string(log_string,		"0",											"combatpower");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string,		cur_time,										"dtime");

		server_log::push_log_string(log_string,		oper_type,										"opertype");
		server_log::push_log_string(log_string,		harry_type,										"harrytype");
		server_log::push_log_string(log_string,		harry_server_id,								"harryserverid");
		server_log::push_log_string(log_string,		normal_harry_count,								"normalharrycount");
		server_log::push_log_string(log_string,		sepcail_harry_count,							"sepcailharrycount");

		server_log::push_log_string(log_string,		server_invaild_string,							"os_version");
		server_log::push_log_string(log_string,		third_info.param8,								"model");
		server_log::push_log_string(log_string,		third_info.param9,								"idfa"); 			//idfa
		server_log::push_log_string(log_string,		third_info.param10,								"imei"); 			//imei
		server_log::push_log_string(log_string,		third_info.param11,								"androidId"); 		//androidId
		server_log::push_log_string(log_string,		third_info.param12,								"mac"); 			//mac
		server_log::push_log_string(log_string,		third_info.param13,								"sn"); 				//sn
		server_log::push_log_string(log_string,		login_type,										"logintype");
		server_log::push_log_string(log_string,		third_info.param6,								"ip", true);		//ip

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_cross_server_harry_logger, log_string);

				// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["oper_type"] = oper_type;
		json["harry_type"] = harry_type;
		json["harry_server_id"] = harry_server_id;
		json["normal_harry_count"] = normal_harry_count;
		json["sepcail_harry_count"] = sepcail_harry_count;

		json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_CROSS_SERVER_HARRY, str_db_log);
#endif
	}

#define fill_item_to_json(index, index2) \
	if (item_list.size() > index) \
	{ \
		json[std::string("item_id_")  + #index2] = item_list[index].m_item_id;\
		json[std::string("item_num_") + #index2] = item_list[index].m_item_num;\
	}

	void server_log::occupation_pk_log(login_fixed_data& third_info, s_unit_info& role_info, int32 login_type, int32 class_type, int32 state_value, guid_64 oppo_guid, int32 pk_result, std::vector<s_item_template_info>& item_list)
	{
#ifdef SHOW_SERVER_LOG
		int32 server_template_id = atoi(third_info.param3);
		std::string game_server = server_log_get_game_server_id(server_template_id);
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_OCCUPATION_PK);

		log_string += "{";
		server_log::push_log_string(log_string,		role_info.account,								"userid");
		server_log::push_log_string(log_string,		third_info.param5,								"deviceid");
		server_log::push_log_string(log_string,		cur_time_int,									"msec");
		server_log::push_log_string(log_string,		third_info.param1,								"appid");			//appid
		server_log::push_log_string(log_string,		"map_change",									"logname");
		server_log::push_log_string(log_string,		third_info.param2,								"version");
		server_log::push_log_string(log_string,		"C1000",										"stepnumid");
		server_log::push_log_string(log_string,		server_log_normversion,							"normversion");
		server_log::push_log_string(log_string,		game_server,									"serverid");
		server_log::push_log_string(log_string,		third_info.param7,								"gamechannel");
		server_log::push_log_string(log_string,		third_info.param4,								"adchannel");
		server_log::push_log_string(log_string,		role_info.role_guid.server_64,					"roleid");
		server_log::push_log_string(log_string,		role_info.data_ary[e_role_info_exp_level],		"rolelevel");
		server_log::push_log_string(log_string,		"0",											"combatpower");
		server_log::push_log_string(log_string, role_info.data_ary[e_role_info_template_id], "career");
		server_log::push_log_string(log_string,		cur_time,										"dtime");

		server_log::push_log_string(log_string,		class_type,										"classtype");
		server_log::push_log_string(log_string,		state_value,									"statevalue");
		server_log::push_log_string(log_string,		oppo_guid,										"oppoguid");
		server_log::push_log_string(log_string,		pk_result,										"pkresult");

		for (int32 i = 0; i < item_list.size(); i++)
		{
			std::string itemid  =  "itemid";
			std::string itemnum = "itemnum";
			itemid  += std::to_string(i+1);
			itemnum += std::to_string(i+1);
			server_log::push_log_string(log_string, item_list[i].m_item_id,	itemid);
			server_log::push_log_string(log_string, item_list[i].m_item_num, itemnum);
		}

		server_log::push_log_string(log_string,		server_invaild_string,							"os_version");
		server_log::push_log_string(log_string,		third_info.param8,								"model");
		server_log::push_log_string(log_string,		third_info.param9,								"idfa"); 			//idfa
		server_log::push_log_string(log_string,		third_info.param10,								"imei"); 			//imei
		server_log::push_log_string(log_string,		third_info.param11,								"androidId"); 		//androidId
		server_log::push_log_string(log_string,		third_info.param12,								"mac"); 			//mac
		server_log::push_log_string(log_string,		third_info.param13,								"sn"); 				//sn
		server_log::push_log_string(log_string,		login_type,										"logintype");
		server_log::push_log_string(log_string,		third_info.param6,								"ip", true);		//ip

		log_string += "}}";

		//PROJECT_RECORD_LOG(g_occupation_pk_logger, log_string);

				// send log to db
		Json::Value json;
		Json::FastWriter writer;
		json["server_id"] = role_info.data_ary[e_role_info_server_id];
		json["log_time"] = cur_time;
		json["account_id"] = role_info.account;
		json["device_id"] = third_info.param5;
		json["role_id"] = role_info.role_guid.server_64;
		json["role_name"] = role_info.role_name;
		json["role_level"] = role_info.data_ary[e_role_info_exp_level];
		json["role_power"] = (role_info.data_i64_ary[e_role_i64_info_gs_value]) / 10;
		json["role_vip"] = role_info.data_ary[e_role_info_vip_level];

		json["class_type"] = class_type;
		json["state_value"] = state_value;
		json["oppo_guid"] = oppo_guid.server_64;
		json["pk_result"] = pk_result;

		fill_item_to_json(0, 1)
			fill_item_to_json(1, 2)
			fill_item_to_json(2, 3)
			fill_item_to_json(3, 4)
			fill_item_to_json(4, 5)
			fill_item_to_json(5, 6)
			fill_item_to_json(6, 7)
			fill_item_to_json(7, 8)
			fill_item_to_json(8, 9)
			fill_item_to_json(9, 10)

			json["login_type"] = login_type;
		json["appid"] = third_info.param1;
		json["version"] = third_info.param2;
		json["stepnumid"] = "B2110";
		json["normversion"] = server_log_normversion;
		json["gamechannel"] = third_info.param14;
		json["ip"] = third_info.param6;
		json["adchannel"] = third_info.param4;
		json["cross_service"] = "";
		json["os_version"] = server_invaild_string;
		json["model"] = third_info.param8;
		json["idfa"] = third_info.param9;
		json["imei"] = third_info.param10;
		json["androidId"] = third_info.param11;
		json["mac"] = third_info.param12;
		json["sn"] = third_info.param13;
		xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_OCCUPATION_PK, str_db_log);

		if (item_list.size() > 10)
		{
			fill_item_to_json(10, 1)
				fill_item_to_json(11, 2)
				fill_item_to_json(12, 3)
				fill_item_to_json(13, 4)
				fill_item_to_json(14, 5)
				fill_item_to_json(15, 6)
				fill_item_to_json(16, 7)
				fill_item_to_json(17, 8)
				fill_item_to_json(18, 9)
				fill_item_to_json(19, 10)

				json["login_type"] = login_type;
			json["appid"] = third_info.param1;
			json["version"] = third_info.param2;
			json["stepnumid"] = "B2110";
			json["normversion"] = server_log_normversion;
			json["gamechannel"] = third_info.param14;
			json["ip"] = third_info.param6;
			json["adchannel"] = third_info.param4;
			json["cross_service"] = "";
			json["os_version"] = server_invaild_string;
			json["model"] = third_info.param8;
			json["idfa"] = third_info.param9;
			json["imei"] = third_info.param10;
			json["androidId"] = third_info.param11;
			json["mac"] = third_info.param12;
			json["sn"] = third_info.param13;
			xstring str_db_log = writer.write_no_enter(json);
			send_log_to_db(TABLE_NAME_OCCUPATION_PK, str_db_log);
		}
#endif
	}

	void server_log::occupation_pk_log(guid_64 player_guid, int32 class_type, int32 state_value, guid_64 oppo_guid, int32 pk_result, std::vector<s_item_template_info>& item_list)
	{
#ifdef SHOW_SERVER_LOG
		int64 cur_time_int = utility::get_tick_count();
		xstring cur_time = time_helper::get_cur_time();

		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_OCCUPATION_PK);

		log_string += "{";
		server_log::push_log_string(log_string,		server_invaild_string,		"userid");
		server_log::push_log_string(log_string,		server_invaild_string,		"deviceid");
		server_log::push_log_string(log_string,		cur_time_int,				"msec");
		server_log::push_log_string(log_string,		server_invaild_string,		"appid");			//appid
		server_log::push_log_string(log_string,		"map_change",				"logname");
		server_log::push_log_string(log_string,		server_invaild_string,		"version");
		server_log::push_log_string(log_string,		"C1000",					"stepnumid");
		server_log::push_log_string(log_string,		server_log_normversion,		"normversion");
		server_log::push_log_string(log_string,		server_invaild_string,		"serverid");
		server_log::push_log_string(log_string,		server_invaild_string,		"gamechannel");
		server_log::push_log_string(log_string,		server_invaild_string,		"adchannel");
		server_log::push_log_string(log_string,		player_guid.server_64,		"roleid");
		server_log::push_log_string(log_string,		server_invaild_string,		"rolelevel");
		server_log::push_log_string(log_string,		"0",						"combatpower");
	
		server_log::push_log_string(log_string,		cur_time,					"dtime");


		server_log::push_log_string(log_string,		class_type,					"classtype");
		server_log::push_log_string(log_string,		state_value,				"statevalue");
		server_log::push_log_string(log_string,		oppo_guid,					"oppoguid");
		server_log::push_log_string(log_string,		pk_result,					"pkresult");

		for (int32 i = 0; i < item_list.size(); i++)
		{
			std::string itemid  = "itemid";
			std::string itemnum = "itemnum";
			itemid  += std::to_string(i + 1);
			itemnum += std::to_string(i + 1);
			server_log::push_log_string(log_string, item_list[i].m_item_id, itemid);
			server_log::push_log_string(log_string, item_list[i].m_item_num, itemnum);
		}
		
		server_log::push_log_string(log_string,		server_invaild_string,		"os_version");
		server_log::push_log_string(log_string,		server_invaild_string,		"model");
		server_log::push_log_string(log_string,		server_invaild_string,		"idfa"); 			//idfa
		server_log::push_log_string(log_string,		server_invaild_string,		"imei"); 			//imei
		server_log::push_log_string(log_string,		server_invaild_string,		"androidId"); 		//androidId
		server_log::push_log_string(log_string,		server_invaild_string,		"mac"); 			//mac
		server_log::push_log_string(log_string,		server_invaild_string,		"sn"); 				//sn
		server_log::push_log_string(log_string,		server_invaild_string,		"logintype");
		server_log::push_log_string(log_string,		server_invaild_string,		"ip", true);		//ip

		log_string += "}";

		//PROJECT_RECORD_LOG(g_occupation_pk_logger, log_string);

				// send log to db
		Json::Value json;
		Json::FastWriter writer;
		//json["server_id"]		= role_info.data_ary[e_role_info_server_id];
		json["log_time"] = cur_time;
		//json["account_id"]	= role_info.account;
		//json["device_id"]		= third_info.param5;
		json["role_id"] = player_guid.server_64;
		//json["role_name"]		= role_info.role_name;
		//json["role_level"]	= role_info.data_ary[e_role_info_exp_level];
		//json["role_power"]	= role_info.data_i64_ary[e_role_i64_info_gs_value];
		//json["role_vip"]		= role_info.data_ary[e_role_info_vip_level];

		json["class_type"] = class_type;
		json["state_value"] = state_value;
		json["oppo_guid"] = oppo_guid.server_64;
		json["pk_result"] = pk_result;

		fill_item_to_json(0, 1)
			fill_item_to_json(1, 2)
			fill_item_to_json(2, 3)
			fill_item_to_json(3, 4)
			fill_item_to_json(4, 5)
			fill_item_to_json(5, 6)
			fill_item_to_json(6, 7)
			fill_item_to_json(7, 8)
			fill_item_to_json(8, 9)
			fill_item_to_json(9, 10)

			xstring str_db_log = writer.write_no_enter(json);
		send_log_to_db(TABLE_NAME_OCCUPATION_PK, str_db_log);

		if (item_list.size() > 10)
		{
			fill_item_to_json(10, 1)
				fill_item_to_json(11, 2)
				fill_item_to_json(12, 3)
				fill_item_to_json(13, 4)
				fill_item_to_json(14, 5)
				fill_item_to_json(15, 6)
				fill_item_to_json(16, 7)
				fill_item_to_json(17, 8)
				fill_item_to_json(18, 9)
				fill_item_to_json(19, 10)

				xstring str_db_log = writer.write_no_enter(json);
			send_log_to_db(TABLE_NAME_OCCUPATION_PK, str_db_log);
		}
#endif
	}

#define  fill_log_comon_head(json, lg_common_head)              \
    json["gameId"] = lg_common_head.gameId;            \
	json["channelId"] = lg_common_head.channelId;       \
	json["mediaId"] = lg_common_head.mediaId;            \
	json["deviceId"] = lg_common_head.deviceId;           \
	json["versionName"] = lg_common_head.versionName;     \
	json["versionCode"] = lg_common_head.versionCode;     \
	json["userId"] = lg_common_head.userId;                \
	json["serverId"] = lg_common_head.serverId;           \
	json["logTime"] = lg_common_head.logTime;             \
	json["roleId"] = lg_common_head.roleId;               \
	json["roleName"] = lg_common_head.roleName;           \
	json["roleLevel"] = lg_common_head.roleLevel;          \
	json["roleGender"] = lg_common_head.roleGender;        \
	json["battlePoints"] = lg_common_head.battlePoints;     \
	json["roleType"] = lg_common_head.roleType;             \
	json["vipLevel"] = lg_common_head.vipLevel;             
       



	void server_log::serverAccountCreate(s_log_common_head &lg_common_head)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverAccountCreate";

	/*	json["gameId"] = lg_common_head.gameId;
		json["channelId"] = lg_common_head.channelId;
		json["mediaId"] = lg_common_head.mediaId;
		json["deviceId"] = lg_common_head.deviceId;
		json["versionName"] = lg_common_head.versionName;
		json["versionCode"] = lg_common_head.versionCode;
		json["userId"] = lg_common_head.userId;
		json["serverId"] = lg_common_head.serverId;
		json["logTime"] = lg_common_head.logTime;
		json["roleId"] = lg_common_head.roleId;
		json["roleName"] = lg_common_head.roleName;
		json["roleLevel"] = lg_common_head.roleLevel;
		json["roleGender"] = lg_common_head.roleGender;
		json["battlePoints"] = lg_common_head.battlePoints;
		json["roleType"] = lg_common_head.roleType;
		json["vipLevel"] = lg_common_head.vipLevel;*/

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverAccountCreate, str_json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverRoleLogin(s_log_common_head &lg_common_head)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRoleLogin";

	/*	json["gameId"] = lg_common_head.gameId;
		json["channelId"] = lg_common_head.channelId;
		json["mediaId"] = lg_common_head.mediaId;
		json["deviceId"] = lg_common_head.deviceId;
		json["versionName"] = lg_common_head.versionName;
		json["versionCode"] = lg_common_head.versionCode;
		json["userId"] = lg_common_head.userId;
		json["serverId"] = lg_common_head.serverId;
		json["logTime"] = lg_common_head.logTime;
		json["roleId"] = lg_common_head.roleId;
		json["roleName"] = lg_common_head.roleName;
		json["roleLevel"] = lg_common_head.roleLevel;
		json["roleGender"] = lg_common_head.roleGender;
		json["battlePoints"] = lg_common_head.battlePoints;
		json["roleType"] = lg_common_head.roleType;
		json["vipLevel"] = lg_common_head.vipLevel;*/

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverRoleLogin, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverRoleCreate(s_log_common_head &lg_common_head)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRoleCreate";

	/*	json["gameId"] = lg_common_head.gameId;
		json["channelId"] = lg_common_head.channelId;
		json["mediaId"] = lg_common_head.mediaId;
		json["deviceId"] = lg_common_head.deviceId;
		json["versionName"] = lg_common_head.versionName;
		json["versionCode"] = lg_common_head.versionCode;
		json["userId"] = lg_common_head.userId;
		json["serverId"] = lg_common_head.serverId;
		json["logTime"] = lg_common_head.logTime;
		json["roleId"] = lg_common_head.roleId;
		json["roleName"] = lg_common_head.roleName;
		json["roleLevel"] = lg_common_head.roleLevel;
		json["roleGender"] = lg_common_head.roleGender;
		json["battlePoints"] = lg_common_head.battlePoints;
		json["roleType"] = lg_common_head.roleType;
		json["vipLevel"] = lg_common_head.vipLevel;*/

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverRoleCreate, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverRoleUpgrade(s_log_common_head &lg_common_head, int32 newRoleLevel, int32 spendSecond)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRoleUpgrade";

		json["newRoleLevel"] = newRoleLevel;
		json["spendSecond"] = spendSecond;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverRoleUpgrade, str_json);
		PROJECT_RECORD_LOG("", str_json);
	}
	void server_log::serverVipUpgrade(s_log_common_head &lg_common_head, int32 vipLevel, int32 spendSecond)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverVipUpgrade";

		json["vipLevel"] = vipLevel;
		json["spendSecond"] = spendSecond;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverVipUpgrade, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}
	void server_log::serverNewbie(s_log_common_head &lg_common_head, xstring step)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverNewbie";
		json["step"] = step;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverNewbie, str_json);
		PROJECT_RECORD_LOG("", str_json);
	}
	void server_log::serverTask(s_log_common_head &lg_common_head, xstring taskId, xstring taskType, int32 taskStatus, int32 spendSecond)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverTask";
		json["taskId"] = taskId;
		json["taskType"] = taskType;
		json["taskStatus"] = taskStatus;
		json["spendSecond"] = spendSecond;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverTask, str_json);
		PROJECT_RECORD_LOG("", str_json);



	}

	void server_log::serverDungeon(s_log_common_head &lg_common_head, xstring dungeonId, xstring  dungeonType, int32 dungeonLevel, int32 battleType, int32 dungeonResult, int32 resultStar, int32 spendSecond)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverDungeon";

		json["dungeonId"] = dungeonId;
		json["dungeonType"] = dungeonType;
		json["dungeonLevel"] = dungeonLevel;
		json["battleType"] = battleType;
		json["dungeonResult"] = dungeonResult;
		json["resultStar"] = resultStar;
		json["spendSecond"] = spendSecond;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverDungeon, str_json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverCurrencyChange(s_log_common_head &lg_common_head, xstring currencyId, int32 changeType, int32 recharge, int32 changeDescribe, int32 changeCount, int32 residueCount, int32 changeLocation)
	{

		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverCurrencyChange";
		json["currencyId"] = currencyId;
		json["changeType"] = changeType;
		json["recharge"] = recharge;
		json["changeDescribe"] = changeDescribe;
		json["changeCount"] = changeCount;
		json["residueCount"] = residueCount;
		json["changeLocation"] = changeLocation;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverCurrencyChange, str_json);
		PROJECT_RECORD_LOG("", str_json);


	}

	void server_log::serverPropChange(s_log_common_head &lg_common_head, xstring propId, int32 changeType, int32 changeDescribe, int32 changeCount, int32 residueCount, int32 changeLocation)
	{

		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverPropChange";
		json["propId"] = propId;
		json["changeType"] = changeType;
		json["changeDescribe"] = changeDescribe;
		json["changeCount"] = changeCount;
		json["residueCount"] = residueCount;
		json["changeLocation"] = changeLocation;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverPropChange, str_json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverPropUpgrade(s_log_common_head &lg_common_head, xstring propId, int32 changeType, xstring propQuality, xstring propStart, xstring propLevel)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverPropUpgrade";
		json["propId"] = propId;
		json["changeType"] = changeType;
		json["propQuality"] = propQuality;
		json["propStart"] = propStart;
		json["propLevel"] = propLevel;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverPropUpgrade, str_json);
		PROJECT_RECORD_LOG("", str_json);


	}
	void  server_log::serverRechargeStep(s_log_common_head &lg_common_head, xstring orderId, xstring goodsId, int32 stepNum)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRechargeStep";
		json["orderId"] = orderId;
		json["goodsId"] = goodsId;
		json["stepNum"] = stepNum;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverRechargeStep, str_json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverPlayerCount(s_log_common_head &lg_common_head, int32 playerCount)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverPlayerCount";
		json["playerCount"] = playerCount;

		xstring str_json =writer.write_no_enter(json);

		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverRechargeSuccess(s_log_common_head &lg_common_head, int32 roleVip, xstring gameOrderId, xstring gameChannelOrderId,
		float orderAmount, float shareaMount, float noShareaMount, int32 payId, int32 loginType, xstring rechargeChannel,
		int32 addJewel, int32 totalJewel, int32 currencyType, int32 itemId, int32 isFirstRecharge, xstring deviceModel)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRechargeSuccess";
		json["roleVip"] = roleVip;
		json["gameOrderId"] = gameOrderId;
		json["gameChannelOrderId"] = gameChannelOrderId;
		json["orderAmount"] = orderAmount;
		json["shareaMount"] = shareaMount;
		json["noShareaMount"] = noShareaMount;
		json["payId"] = payId;
		json["loginType"] = loginType;
		json["rechargeChannel"] = rechargeChannel;
		json["addJewel"] = addJewel;
		json["totalJewel"] = totalJewel;
		json["currencyType"] = currencyType;
		json["itemId"] = itemId;
		json["isFirstRecharge"] = isFirstRecharge;
		json["deviceModel"] = deviceModel;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverRechargeSuccess, str_json);
		PROJECT_RECORD_LOG("", str_json);
		


	}

	void server_log::serverLuckyDraw(s_log_common_head &lg_common_head, int32 itemId, int32 itemNum, int32 treasureType, int32 costType,
		int32 isFirstTimeDo, int32 oldLuckyValue, int32 newLuckyValue, int32 isFree)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverLuckyDraw";
		json["itemId"] = itemId;
		json["itemNum"] = itemNum;
		json["treasureType"] = treasureType;
		json["costType"] = costType;
		json["isFirstTimeDo"] = isFirstTimeDo;
		json["oldLuckyValue"] = oldLuckyValue;
		json["newLuckyValue"] = newLuckyValue;
		json["isFree"] = isFree;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverLuckyDraw, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverAuction(s_log_common_head &lg_common_head, xstring sellerAccountId, xstring sellerRoleGuid, xstring sellerRoleName, int32 sellerRoleLevel,
		xstring itemGuid, int32 itemId, int32 itemNum, int32 auctionType,
		xstring buyerAccountId, xstring buyerRoleGuid, xstring buyerRoleName, int32 buyerRoleLevel,
		int32 moneyType, xstring moneyName, int32 moneyValue, int32 auctionState)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverAuction";
		json["sellerAccountId"] = sellerAccountId;
		json["sellerRoleGuid"] = sellerRoleGuid;
		json["sellerRoleName"] = sellerRoleName;
		json["sellerRoleLevel"] = sellerRoleLevel;
		json["itemGuid"] = itemGuid;
		json["itemId"] = itemId;
		json["itemNum"] = itemNum;
		json["auctionType"] = auctionType;
		json["buyerAccountId"] = buyerAccountId;
		json["buyerRoleGuid"] = buyerRoleGuid;
		json["buyerRoleName"] = buyerRoleName;
		json["buyerRoleLevel"] = buyerRoleLevel;
		json["moneyType"] = moneyType;
		json["moneyName"] = moneyName;
		json["moneyValue"] = moneyValue;
		json["auctionState"] = auctionState;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverAuction, str_json);

		PROJECT_RECORD_LOG("", str_json);



	}

	void server_log::serverChat(s_log_common_head &lg_common_head, xstring targetId, int32 chatType, xstring chatContent)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverChat";
		json["targetId"] = targetId;
		json["chatType"] = chatType;
		json["chatContent"] = chatContent;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverChat, str_json);

		PROJECT_RECORD_LOG("", str_json);
		

	}
	void server_log::serverBuyGoods(s_log_common_head &lg_common_head, int32 goodsId, xstring goodsName, int32 buyNum, int32 itemId,
		xstring itemName, int32 itemNumPerGoods, int32 costMoneyType, xstring constMoneyName, int32 costMoneyNum)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverBuyGoods";
		json["goodsId"] = goodsId;
		json["goodsName"] = goodsName;
		json["buyNum"] = buyNum;
		json["itemId"] = itemId;
		json["itemName"] = itemName;
		json["itemNumPerGoods"] = itemNumPerGoods;
		json["costMoneyType"] = costMoneyType;
		json["costMoneyName"] = constMoneyName;
		json["costMoneyNum"] = costMoneyNum;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverBuyGoods, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverGetMailContent(s_log_common_head &lg_common_head, int32 itemId, int32 itemNum, int64 mailGuid, xstring mailTitle)
	{
		std::string log_mail_title = "";
		std::vector<xstring> title_list;
		init_unit::explode(mailTitle, title_list);
		if (title_list.size() > 0)
		{
			log_mail_title = template_manager::get_instance().get_str_by_string_template_id(std::atoi(title_list[0].c_str()));
			if (log_mail_title.empty())
			{
				log_mail_title = mailTitle;
			}
			int32 last_index = 0;
			for (int32 i = 1; i < title_list.size(); ++i)
			{
				last_index = log_mail_title.find("%s", last_index);
				if (last_index < 0)
				{
					break;
				}
				log_mail_title.replace(last_index, string("%s").length(), title_list[i]);
			}
		}
		
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverGetMailContent";
		json["itemId"] = itemId;
		json["itemNum"] = itemNum;
		json["mailGuid"] = mailGuid;
		json["mailTitle"] = log_mail_title;
		
		xstring str_json = writer.write_no_enter(json);



		//PROJECT_RECORD_LOG(g_serverGetMailContent, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverGrowthFund(s_log_common_head &lg_common_head, int32 purchaseGrowthFundId, int32 purchaseCost, int32 purchaseQuantity)
	{
		Json::Value json;
		Json::FastWriter writer;

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverGrowthFund";
		json["purchaseGrowthFundId"] = purchaseGrowthFundId;
		json["purchaseCost"] = purchaseCost;
		json["purchaseQuantity"] = purchaseQuantity;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverGrowthFund, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverCrossServerHarry(s_log_common_head &lg_common_head, int32 operType, int32  harryType, int32 harryServerId, int32 normalHarryCount, int32 sepcailHarryCount)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverCrossServerHarry";
		json["operType"] = operType;
		json["harryType"] = harryType;
		json["harryServerId"] = harryServerId;
		json["normalHarryCount"] = normalHarryCount;
		json["sepcailHarryCount"] = sepcailHarryCount;

		xstring str_json =writer.write_no_enter(json);

		//PROJECT_RECORD_LOG(g_serverCrossServerHarry, str_json);
		PROJECT_RECORD_LOG("", str_json);

	}

	 void server_log::serverCrossServerBossKill(s_log_common_head &lg_common_head, int32 bossId, int64 killLegionId, xstring killLegionName, int64 top1Id, xstring top1Name,
		int64 top2Id, xstring top2Name, int64 top3Id, xstring top3Name)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverCrossServerHarry";
		json["bossId"] = bossId;
		json["killLegionId"] = killLegionId;
		json["killLegionName"] = killLegionName;
		json["top1Id"] = top1Id;
		json["top1Name"] = top1Name;
		json["top2Id"] = top2Id;
		json["top2Name"] = top2Name;
		json["top3Id"] = top3Id;
		json["top3Name"] = top3Name;

		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverOccupationPk(s_log_common_head &lg_common_head, int32 classType, int32 stateValue, int64 oppoGuid, int32 pkResult, std::vector<s_item_template_info>& item_list)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverOccupationPk";
		json["classType"] = classType;
		json["stateValue"] = stateValue;
		json["oppoGuid"] = oppoGuid;
		json["pkResult"] = pkResult;

		for (int32 i = 0 ; i < 10; i++)
		{
			xstring item_id_key = "itemId" + std::to_string(i + 1);
			xstring item_num_key = "itemNum" + std::to_string(i + 1);
			int32  id = 0;
			int32  num = 0;
			if (item_list.size() > i)
			{
				id = item_list[i].m_item_id;
				num = item_list[i].m_item_num;
			}
			json[item_id_key] = id;
			json[item_num_key] = num;
		}

		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverBossKillDrop(s_log_common_head &lg_common_head, int32 bossId, std::vector< s_item_template_info >& item_list)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverBossKillDrop";
		json["bossId"] = bossId;

		for (int32 i = 0; i < 20; i++)
		{
			xstring item_id_key = "dropItemId" + std::to_string(i + 1);
			xstring item_num_key = "dropItemNum" + std::to_string(i + 1);
			int32  id = 0;
			int32  num = 0;
			if (item_list.size() > i)
			{
				id = item_list[i].m_item_id;
				num = 1;
			}
			json[item_id_key] = id;
			json[item_num_key] = num;
		}
		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}


	void server_log::serverBossKill(s_log_common_head &lg_common_head, int32 bossId, int32 mapId, int32 playerNum, int64 battleBeginTime, int64 battleEndTime, int64 battleContinuedTime)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);

		json["logName"] = "serverBossKill";
		json["bossId"] = bossId;
		json["mapId"] = mapId;
		json["playerNumn"] = playerNum;
		json["battleBeginTime"] = battleBeginTime;
		json["battleEndTime"] = battleEndTime;
		json["battleContinuedTime"] = battleContinuedTime;


		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);




	}

	void server_log::serverMapChange(s_log_common_head &lg_common_head, int32 mapTemplateId, int32 mapType, int32 mapSubType, int32 changeType, int32 stateType)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);

		json["logName"] = "serverMapChange";
		json["mapTemplateId"] = mapTemplateId;
		json["mapType"] = mapType;
		json["mapSubType"] = mapSubType;
		json["changeType"] = changeType;
		json["stateType"] = stateType;


		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverTimeLimitActivity(s_log_common_head &lg_common_head, int32 curValue, int32 targetValue, int32 rewardId, int32 rewardNum, int32 activityType)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverTimeLimitActivity";
		json["curValue"] = curValue;
		json["targetValue"] = targetValue;
		json["rewardId"] = rewardId;
		json["rewardNum"] = rewardNum;
		json["activityType"] = activityType;


		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverRoleMallLog(s_log_common_head &lg_common_head, int32 activityId, int32 rewardId, int32 rewardNum, int32 consumedCurrencyType, int32 consumedNum, xstring storeName, xstring storeId, int32 soldNum, xstring soldCurrecyId, int32 soldNumAmount)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRoleMallLog";
		json["activityId"] = activityId;
		json["rewardId"] = rewardId;
		json["rewardNum"] = rewardNum;
		json["consumedCurrencyType"] = consumedCurrencyType;
		json["consumedNum"] = consumedNum;
		json["storeName"] = storeName;
		json["storeId"] = storeId;
		json["soldNum"] = soldNum;
		json["soldCurrecyId"] = soldCurrecyId;
		json["soldNumAmount"] = soldNumAmount;

		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverLegionLog(s_log_common_head &lg_common_head, xstring legionName, int32 legionLevel, int64  legionPower, int32 legionNum)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverLegionLog";
		json["legionName"] = legionName;
		json["legionLevel"] = legionLevel;
		json["legionPower"] = legionPower;
		json["legionNum"] = legionNum;

		xstring str_json =writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::reconnect_game_log(std::string str_server_type, std::string func_name, std::string context_)
	{
		xstring log_string = "{";
		server_log::push_hadooplog_string(log_string, TABLE_NAME_RECONNECT_GAME);
		log_string += "{";
		server_log::push_log_string(log_string, str_server_type, "server_type");
		server_log::push_log_string(log_string, func_name, "function_name");
		server_log::push_log_string(log_string, context_, "context");

		log_string += "}";

		//PROJECT_RECORD_LOG(g_reconnect_logger, log_string);
	}

	void server_log::serverLegionWelfareGrantNumLog(s_log_common_head & lg_common_head, guid_64 legion_guid, int32 rewardNum, int32 activeNum)
	{
#ifdef SHOW_SERVER_LOG
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverLegionWelfareGrantNumLog";
		json["legionRewardNum"] = rewardNum;
		json["legionActiveNum"] = activeNum;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

		xstring cur_time = time_helper::get_cur_time();
		Json::Value db_json;
		Json::FastWriter db_writer;
		db_json["server_id"] = SERVERCONFIG->game_id;
		db_json["log_time"] = cur_time;
		db_json["legion_id"] = legion_guid.server_64;
		db_json["legionRewardNum"] = rewardNum;
		db_json["legionActiveNum"] = activeNum;
		xstring str_db_log = db_writer.write_no_enter(db_json);
//		send_log_to_db(TABLE_NAME_LEGION_GRANT, str_db_log);
#endif
	}

	void server_log::serverLegionActivrNumLog(s_log_common_head & lg_common_head, int32 activeNum)
	{
#ifdef SHOW_SERVER_LOG
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverLegionActivrNumLog";
		json["legionActiveNum"] = activeNum;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

		xstring cur_time = time_helper::get_cur_time();

		Json::Value db_json;
		Json::FastWriter db_writer;
		db_json["server_id"] = SERVERCONFIG->game_id;
		db_json["log_time"] = cur_time;
		db_json["legionActiveNum"] = activeNum;
		xstring str_db_log = db_writer.write_no_enter(db_json);
//		send_log_to_db(TABLE_NAME_LEGION_ACTIVE, str_db_log);
#endif
	}

	void server_log::serverAssistantLog(s_log_common_head & lg_common_head, int32 mapType)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverAssistantLog";
		json["mapType"] = mapType;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	
	}

	void server_log::serverRankList(s_log_common_head & lg_common_head, s_ranking_player_info rank_info, int32 rank_index)
	{
		int64 cur_time_int = utility::get_tick_count();

		Json::Value json;
		Json::FastWriter writer;
		set_log_common_head_part3(lg_common_head, "null",  SERVERCONFIG->game_id, cur_time_int);
		set_log_common_head_part4(lg_common_head, rank_info.role_guid, rank_info.role_name, 0, 0);
		set_log_common_head_part5(lg_common_head, 0, 0, rank_info.vip_level);

		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverRankListLog";
		json["rankingType"] = rank_info.ranking_type;
		json["rankingValue"] = rank_info.ranking_value;
		json["rankingTime"] = rank_info.save_time;
		json["supportingGuid"] = rank_info.supporting_guid.server_64;
		json["supportingName"] = rank_info.supporting_name;
		json["peak"] = rank_info.Peak;
		json["gameChannel"] = rank_info.game_channel;
		json["rankingIndex"] = rank_index;

		xstring str_db_log = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_db_log);
	}

	void server_log::serverTreasureLog(s_log_common_head & lg_common_head, int32 type, int32 bossId, int32 num)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverTreasureLog";
		json["type"] = type;
		json["bossId"] = bossId;
		json["num"] = num;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverBeatBoss(s_log_common_head & lg_common_head, int32 activeType, int32 type, int32 bossId)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverBeatBoss";
		json["activeType"] = activeType;
		json["type"] = type;
		json["bossId"] = bossId;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverManorLog(s_log_common_head & lg_common_head, int32 activeType, int32 type)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverManorLog";
		json["activeType"] = activeType;
		json["type"] = type;
		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverCrystalLog(s_log_common_head & lg_common_head, int32 type)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverCrystalLog";
		json["type"] = type;
		
		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverLadderLog(s_log_common_head & lg_common_head, int32 type)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverLadderLog";
		json["type"] = type;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverPkKingLog(s_log_common_head & lg_common_head, int32 type)
	{
		Json::Value json;
		Json::FastWriter  writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverPkKingLog";
		json["type"] = type;

		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}

	void server_log::serverGodnessStar(s_log_common_head &lg_common_head, int32 level)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverGodnessStar";
		json["level"] = level;
		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);

	}

	void server_log::serverEarlyWarning(s_log_common_head &lg_common_head, int32 onlineTime)
	{
		Json::Value json;
		Json::FastWriter writer;
		fill_log_comon_head(json, lg_common_head);
		json["logName"] = "serverEarlyWarning";
		json["onlineTime"] = onlineTime;
		xstring str_json = writer.write_no_enter(json);
		PROJECT_RECORD_LOG("", str_json);
	}



	

	


}