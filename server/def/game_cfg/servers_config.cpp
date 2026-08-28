#include "servers_config.h"
#include <iostream>
#include <fstream>
#include "server_log.hpp"

namespace hld 
{

	void server_base_config::clear_data()
	{
		app_key = "";
		game_id = 0;
		game_name = "";
		init_player_num = 0;
		gm_state = false;
		robot_account = "";
		web_address = "";
		web_port = 0;
		daemon_address = "";
		daemon_port = 0;
		billing_url = "";
		recharge_url = "";
		one_store_recharge_url = "";
		login_url = "";
		server_state_url = "";
		account_info_url = "";
		http_aes = "";
		gift_url = "";
		new_log_file_root = "";
		not_need_db_log = false;
	}
	bool server_base_config::parse_by_json(Json::Value& json_obj)
	{
		if (!json_obj.isObject())
		{
			return false;
		}
		if (json_obj["app_key"].isString())
		{
			app_key = json_obj["app_key"].asString();
		}
		if (json_obj["game_id"].isInt())
		{
			game_id = json_obj["game_id"].asInt();
		}
		if (json_obj["game_name"].isString())
		{
			game_name = json_obj["game_name"].asString();
		}
		if (json_obj["init_player_num"].isInt())
		{
			init_player_num = json_obj["init_player_num"].asInt();
		}
		if (json_obj["gm_state"].isBool())
		{
			gm_state = json_obj["gm_state"].asBool();
		}
		if (json_obj["robot_account"].isString())
		{
			robot_account = json_obj["robot_account"].asString();
		}
		if (json_obj["web_address"].isString())
		{
			web_address = json_obj["web_address"].asString();
		}
		if (json_obj["web_port"].isInt())
		{
			web_port = json_obj["web_port"].asInt();
		}
		if (json_obj["daemon_address"].isString())
		{
			daemon_address = json_obj["daemon_address"].asString();
		}
		if (json_obj["daemon_port"].isInt())
		{
			daemon_port = json_obj["daemon_port"].asInt();
		}
		if (json_obj["billing_url"].isString())
		{
			billing_url = json_obj["billing_url"].asString();
		}
		if (json_obj["recharge_url"].isString())
		{
			recharge_url = json_obj["recharge_url"].asString();
		}
		if (json_obj["one_store_recharge_url"].isString())
		{
			one_store_recharge_url = json_obj["one_store_recharge_url"].asString();
		}
		if (json_obj["login_url"].isString())
		{
			login_url = json_obj["login_url"].asString();
		}
		if (json_obj["server_state_url"].isString())
		{
			server_state_url = json_obj["server_state_url"].asString();
		}
		if (json_obj["account_info_url"].isString())
		{
			account_info_url = json_obj["account_info_url"].asString();
		}
		if (json_obj["http_aes"].isString())
		{
			http_aes = json_obj["http_aes"].asString();
		}
		if (json_obj["gift_url"].isString())
		{
			gift_url = json_obj["gift_url"].asString();
		}
		if (json_obj["new_log_file_root"].isString())
		{
			new_log_file_root = json_obj["new_log_file_root"].asString();
		}
		if (json_obj["not_need_db_log"].isBool())
		{
			not_need_db_log = json_obj["not_need_db_log"].asBool();
		}
		return true;
	}
	void net_config_base::clear_data()
	{
		server_type = e_server_type_invalid;
		internal_host = "";
		internal_port = 0;
		external_host = "";
		external_port = 0;
	}
	bool net_config_base::parse_by_json(Json::Value& json_obj)
	{
		if (!json_obj.isObject())
		{
			return false;
		}
		if (json_obj["server_type"].isInt())
		{
			server_type = (e_server_type)json_obj["server_type"].asInt();
		}
		if (json_obj["internal_host"].isString()) 
		{
			internal_host = json_obj["internal_host"].asString();
		}
		if (json_obj["internal_port"].isInt()) 
		{
			internal_port = json_obj["internal_port"].asInt();
		}
		if (json_obj["external_host"].isString())
		{
			external_host = json_obj["external_host"].asString();
		}
		if (json_obj["external_port"].isInt())
		{
			external_port = json_obj["external_port"].asInt();
		}
		return true;
	}
	void fep_config::clear_data()
	{
		net_config_base::clear_data();
		client_send_limit = 20;
		init_socket_num = 20;
	}
	bool fep_config::parse_by_json(Json::Value& json_obj)
	{
		if (json_obj.isObject() == false)
		{
			return false;
		}
		if (net_config_base::parse_by_json(json_obj) == false)
		{
			return false;
		}
		if (json_obj["client_send_limit"].isInt())
		{
			client_send_limit = json_obj["client_send_limit"].asInt();
		}
		if (json_obj["init_socket_num"].isInt())
		{
			init_socket_num = json_obj["init_socket_num"].asInt();
		}
		return true;
	}
	void gate_config::clear_data()
	{
		net_config_base::clear_data();
		gate_id = 0;
	}
	bool gate_config::parse_by_json(Json::Value& json_obj)
	{
		if (!json_obj.isObject())
		{
			return false;
		}
		if (!net_config_base::parse_by_json(json_obj))
		{
			return false;
		}
		if (json_obj["gate_id"].isInt());
		{
			gate_id = json_obj["gate_id"].asInt();
		}
		return true;
	}

	void ws_config::clear_data()
	{
		net_config_base::clear_data();
		sdk_url = "";
		ds_url = "";
		http_port = 0;
		max_team_user_num = 0;
		max_room_team_num = 0;
		max_room_user_num = 0;
	}
	bool ws_config::parse_by_json(Json::Value& json_obj)
	{
		if (json_obj.isObject() == false)
		{
			return false;
		}
		if (net_config_base::parse_by_json(json_obj) == false)
		{
			return false;
		}
		if (json_obj["sdk_url"].isString())
		{
			sdk_url = json_obj["sdk_url"].asString();
		}
		if (json_obj["ds_url"].isString())
		{
			ds_url = json_obj["ds_url"].asString();
		}
		if (json_obj["http_port"].isInt());
		{
			http_port = json_obj["http_port"].asInt();
		}
		if (json_obj["max_team_user_num"].isInt());
		{
			max_team_user_num = json_obj["max_team_user_num"].asInt();
		}
		if (json_obj["max_room_team_num"].isInt());
		{
			max_room_team_num = json_obj["max_room_team_num"].asInt();
		}
		if (json_obj["max_room_user_num"].isInt());
		{
			max_room_user_num = json_obj["max_room_user_num"].asInt();
		}
		return true;
	}	
	void cs_config::clear_data()
	{
		net_config_base::clear_data();
		role_num_limit = 2400;//最大承载人数
		npc_num_limit = 10000;//最大承载Npc数量
		drop_num_limit = 60000;//最大承载掉落数
		role_data_save_time = 60000;//自动存盘间隔
		role_data_send_time = 60000;//延迟同步间隔
		process_stat_interval = 600;//
	}
	bool cs_config::parse_by_json(Json::Value& json_obj)
	{
		if (json_obj.isObject() == false)
		{
			return false;
		}
		if (net_config_base::parse_by_json(json_obj) == false)
		{
			return false;
		}
		if (json_obj["role_num_limit"].isInt())
		{
			role_num_limit = json_obj["role_num_limit"].asInt();
		}
		if (json_obj["npc_num_limit"].isInt())
		{
			npc_num_limit = json_obj["npc_num_limit"].asInt();
		}
		if (json_obj["drop_num_limit"].isInt())
		{
			drop_num_limit = json_obj["drop_num_limit"].asInt();
		}
		if (json_obj["role_data_save_time"].isInt())
		{
			role_data_save_time = json_obj["role_data_save_time"].asInt();
		}
		if (json_obj["role_data_send_time"].isInt())
		{
			role_data_send_time = json_obj["role_data_send_time"].asInt();
		}
		if (json_obj["process_stat_interval"].isInt())
		{
			process_stat_interval = json_obj["process_stat_interval"].asInt();
		}
		return true;
	}
	void dp_config::clear_data()
	{
		net_config_base::clear_data();
		m_db_info_map.clear();
		db_error_limit = 10;
		db_thread_num = 18; 
		char_byte_size = 1;
	}
	bool dp_config::parse_by_json(Json::Value& json_obj)
	{
		if (json_obj.isObject() == false)
		{
			return false;
		}
		if (net_config_base::parse_by_json(json_obj) == false)
		{
			return false;
		}
		if (json_obj["mysql_conn"].isNull() || json_obj["mysql_conn"].isArray() == false)
		{
			return false;
		}
		for (int32 i = 0; i < json_obj["mysql_conn"].size(); ++i)
		{
			Json::Value& db_obj = json_obj["mysql_conn"][i];
			if (db_obj["mysql_id"].isNull() || db_obj["ip_addr"].isNull() || db_obj["ip_port"].isNull() || db_obj["user_name"].isNull() || db_obj["user_password"].isNull() || db_obj["schema_name"].isNull() || db_obj["thread_num"].isNull())
			{
				return false;
			}
			db_info temp_info;
			temp_info.mysql_id = db_obj["mysql_id"].asInt();
			temp_info.ip_addr = db_obj["ip_addr"].asString();
			temp_info.ip_port = db_obj["ip_port"].asInt();
			temp_info.user_name = db_obj["user_name"].asString();
			temp_info.user_password = db_obj["user_password"].asString();
			temp_info.schema_name = db_obj["schema_name"].asString();
			temp_info.thread_num = db_obj["thread_num"].asInt();
			m_db_info_map[temp_info.mysql_id] = temp_info;
		}
		if (json_obj["save_open"].isInt())
		{
			save_open = json_obj["save_open"].asInt();
		}
		if (json_obj["save_interval"].isInt())
		{
			save_interval = json_obj["save_interval"].asInt();
		}
		return true;
	}
	net_config_base const* servers_config_manager::get_self_config() 
	{ 
		return nullptr;
		//return get_server_config(g_worker->get_server_type());
	}
	net_config_base* servers_config_manager::get_server_config(e_server_type server_type)
	{
		if (server_type < 0 || server_type >= e_server_type_max)
		{
			return nullptr;
		}
		return m_server_config_array[server_type];
	}
	bool servers_config_manager::init_config() 
	{
		memset(m_server_config_array, 0, sizeof(m_server_config_array));
		Json::Reader reader;
		Json::Value  json_value;
		std::ifstream in("./servers.json");
		if (reader.parse(in, json_value) == false)
		{
			return false;
		}
		if (parse_by_json(json_value) == false)
		{
			return false;
		}
		return true;
	}

	bool servers_config_manager::parse_by_json(Json::Value& json_obj)
	{
		if (json_obj.isObject() == false)
		{
			std::cout << "servers_config_manager::parse_by_json fail!! no object" << std::endl;
			return false;
		}
		auto& base_obj = json_obj["server_base"];
		if (!base_obj.isObject())
		{
			std::cout << "servers_config_manager::parse_by_json fail!! no base_obj object" << std::endl;
			return false;
		}
		m_server_config = new server_base_config();
		if (m_server_config->parse_by_json(base_obj) == false) {
			std::cout << "servers_config_manager::parse_by_json fail!! server_base config fail" << std::endl;
			return false;
		}
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			if (json_obj[e_server_type_str[i]].isObject())
			{
				if (parse_by_json(json_obj[e_server_type_str[i]], (e_server_type)i) == false)
				{
					std::cout << "servers_config_manager::parse_by_json fail!! server:" << e_server_type_str[i] << std::endl;
					return false;
				}
			}
		}
		return true;
	}
	bool servers_config_manager::parse_by_json(Json::Value& json_obj, e_server_type server_type)
	{
		if (json_obj.isObject() == false)
		{
			std::cout << "servers_config_manager::parse_by_json fail!! no object server:" << server_type << std::endl;
			return false;
		}

		net_config_base* config_ptr = create_server_config(server_type);
		if (nullptr == config_ptr)
		{
			std::cout << "servers_config_manager::parse_by_json fail!! create_server_config fail!!! server:" << server_type << std::endl;
			return false;
		}
		if (config_ptr->parse_by_json(json_obj) == false)
		{
			delete config_ptr;
			config_ptr = nullptr;
			std::cout << "servers_config_manager::parse_by_json fail!! config json fail!!! server:" << server_type << std::endl;
			return false;
		}
		m_server_config_array[server_type] = config_ptr;
		return true;
	}
	net_config_base* servers_config_manager::create_server_config(e_server_type server_type)
	{
		switch (server_type)
		{
		case hld::e_server_type_gate:
			return new gate_config();
			break;
		case hld::e_server_type_ws:
			return new ws_config();
			break;
		case hld::e_server_type_fep:
			return new fep_config();
			break;
		case hld::e_server_type_cs:
			return new cs_config();
			break;
		case hld::e_server_type_dp:
			return new dp_config();
			break;
		case hld::e_server_type_max:
			break;
		default:
			break;
		}
		return nullptr;
	}
}
