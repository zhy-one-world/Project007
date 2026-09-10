#include "servers_config.h"
#include "xml_config_util.hpp"

#include <iostream>
#include <cstring>

#include <tinyxml/tinyxml.h>

#include "server_log.hpp"

namespace faith 
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
		config_center_host = "127.0.0.1";
		config_center_port = 19000;
	}

	bool server_base_config::parse_by_xml(TiXmlElement* element)
	{
		if (element == nullptr)
		{
			return false;
		}
		app_key = xml_child_text(element, "app_key", app_key.c_str());
		game_id = xml_child_int(element, "game_id", game_id);
		game_name = xml_child_text(element, "game_name", game_name.c_str());
		init_player_num = xml_child_int(element, "init_player_num", init_player_num);
		gm_state = xml_child_bool(element, "gm_state", gm_state);
		robot_account = xml_child_text(element, "robot_account", robot_account.c_str());
		web_address = xml_child_text(element, "web_address", web_address.c_str());
		web_port = xml_child_int(element, "web_port", web_port);
		daemon_address = xml_child_text(element, "daemon_address", daemon_address.c_str());
		daemon_port = xml_child_int(element, "daemon_port", daemon_port);
		billing_url = xml_child_text(element, "billing_url", billing_url.c_str());
		recharge_url = xml_child_text(element, "recharge_url", recharge_url.c_str());
		one_store_recharge_url = xml_child_text(element, "one_store_recharge_url", one_store_recharge_url.c_str());
		login_url = xml_child_text(element, "login_url", login_url.c_str());
		server_state_url = xml_child_text(element, "server_state_url", server_state_url.c_str());
		account_info_url = xml_child_text(element, "account_info_url", account_info_url.c_str());
		http_aes = xml_child_text(element, "http_aes", http_aes.c_str());
		gift_url = xml_child_text(element, "gift_url", gift_url.c_str());
		new_log_file_root = xml_child_text(element, "new_log_file_root", new_log_file_root.c_str());
		not_need_db_log = xml_child_bool(element, "not_need_db_log", not_need_db_log);
		config_center_host = xml_child_text(element, "config_center_host", config_center_host.c_str());
		config_center_port = xml_child_int(element, "config_center_port", config_center_port);
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

	bool net_config_base::parse_by_xml(TiXmlElement* element)
	{
		if (element == nullptr)
		{
			return false;
		}
		const int type_value = xml_child_int(element, "server_type", static_cast<int>(server_type));
		if (type_value > e_server_type_invalid && type_value < e_server_type_max)
		{
			server_type = static_cast<e_server_type>(type_value);
		}
		internal_host = xml_child_text(element, "internal_host", internal_host.c_str());
		internal_port = xml_child_int(element, "internal_port", internal_port);
		external_host = xml_child_text(element, "external_host", external_host.c_str());
		external_port = xml_child_int(element, "external_port", external_port);
		return true;
	}

	void gateway_config::clear_data()
	{
		net_config_base::clear_data();
		client_send_limit = 20;
		init_socket_num = 20;
	}

	bool gateway_config::parse_by_xml(TiXmlElement* element)
	{
		if (!net_config_base::parse_by_xml(element))
		{
			return false;
		}
		client_send_limit = xml_child_int(element, "client_send_limit", client_send_limit);
		init_socket_num = xml_child_int(element, "init_socket_num", init_socket_num);
		return true;
	}

	void gate_config::clear_data()
	{
		net_config_base::clear_data();
		gate_id = 0;
	}

	bool gate_config::parse_by_xml(TiXmlElement* element)
	{
		if (!net_config_base::parse_by_xml(element))
		{
			return false;
		}
		gate_id = xml_child_int(element, "gate_id", gate_id);
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

	bool ws_config::parse_by_xml(TiXmlElement* element)
	{
		if (!net_config_base::parse_by_xml(element))
		{
			return false;
		}
		sdk_url = xml_child_text(element, "sdk_url", "");
		ds_url = xml_child_text(element, "ds_url", "");
		http_port = xml_child_int(element, "http_port", http_port);
		max_team_user_num = xml_child_int(element, "max_team_user_num", max_team_user_num);
		max_room_team_num = xml_child_int(element, "max_room_team_num", max_room_team_num);
		max_room_user_num = xml_child_int(element, "max_room_user_num", max_room_user_num);
		return true;
	}

	void cs_config::clear_data()
	{
		net_config_base::clear_data();
		role_num_limit = 2400;
		npc_num_limit = 10000;
		drop_num_limit = 60000;
		role_data_save_time = 60000;
		role_data_send_time = 60000;
		process_stat_interval = 600;
	}

	bool cs_config::parse_by_xml(TiXmlElement* element)
	{
		if (!net_config_base::parse_by_xml(element))
		{
			return false;
		}
		role_num_limit = xml_child_int(element, "role_num_limit", role_num_limit);
		npc_num_limit = xml_child_int(element, "npc_num_limit", npc_num_limit);
		drop_num_limit = xml_child_int(element, "drop_num_limit", drop_num_limit);
		role_data_save_time = xml_child_int(element, "role_data_save_time", role_data_save_time);
		role_data_send_time = xml_child_int(element, "role_data_send_time", role_data_send_time);
		process_stat_interval = xml_child_int(element, "process_stat_interval", process_stat_interval);
		return true;
	}

	void dp_config::clear_data()
	{
		net_config_base::clear_data();
		m_db_info_map.clear();
		db_error_limit = 10;
		db_thread_num = 18;
		char_byte_size = 1;
		save_open = 0;
		save_interval = 0;
	}

	bool dp_config::parse_by_xml(TiXmlElement* element)
	{
		if (!net_config_base::parse_by_xml(element))
		{
			return false;
		}
		TiXmlElement* mysql_root = xml_child(element, "mysql_conn");
		if (mysql_root == nullptr)
		{
			return false;
		}
		for (TiXmlElement* conn = mysql_root->FirstChildElement("conn");
			conn != nullptr;
			conn = conn->NextSiblingElement("conn"))
		{
			db_info temp_info;
			temp_info.mysql_id = xml_child_int(conn, "mysql_id", 0);
			temp_info.ip_addr = xml_child_text(conn, "ip_addr", "");
			temp_info.ip_port = xml_child_int(conn, "ip_port", 0);
			temp_info.user_name = xml_child_text(conn, "user_name", "");
			temp_info.user_password = xml_child_text(conn, "user_password", "");
			temp_info.schema_name = xml_child_text(conn, "schema_name", "");
			temp_info.thread_num = xml_child_int(conn, "thread_num", 1);
			if (temp_info.ip_addr.empty() || temp_info.ip_port <= 0 ||
				temp_info.user_name.empty() || temp_info.schema_name.empty())
			{
				return false;
			}
			m_db_info_map[temp_info.mysql_id] = temp_info;
		}
		if (m_db_info_map.empty())
		{
			return false;
		}
		save_open = xml_child_int(element, "save_open", save_open);
		save_interval = xml_child_int(element, "save_interval", save_interval);
		return true;
	}

	servers_config_manager::servers_config_manager()
	{
		m_self_config = nullptr;
		m_server_config = nullptr;
		memset(m_server_config_array, 0, sizeof(m_server_config_array));
	}

	net_config_base const* servers_config_manager::get_self_config() 
	{ 
		return nullptr;
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
		return start();
	}

	bool servers_config_manager::on_start()
	{
		return load_config();
	}

	void servers_config_manager::on_stop()
	{
		clear_config();
	}

	void servers_config_manager::clear_config()
	{
		delete m_server_config;
		m_server_config = nullptr;
		m_self_config = nullptr;
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			delete m_server_config_array[i];
			m_server_config_array[i] = nullptr;
		}
	}

	bool servers_config_manager::load_config()
	{
		clear_config();
		TiXmlDocument doc;
		if (!doc.LoadFile("./servers.xml"))
		{
			std::cout << "servers_config_manager::load_config fail!! load servers.xml: "
				<< doc.ErrorDesc() << std::endl;
			return false;
		}
		TiXmlElement* root = doc.RootElement();
		if (root == nullptr || std::strcmp(root->Value(), "servers") != 0)
		{
			std::cout << "servers_config_manager::load_config fail!! root must be <servers>" << std::endl;
			return false;
		}
		return parse_by_xml(root);
	}

	bool servers_config_manager::parse_by_xml(TiXmlElement* root)
	{
		TiXmlElement* base_obj = xml_child(root, "server_base");
		if (base_obj == nullptr)
		{
			std::cout << "servers_config_manager::parse_by_xml fail!! no server_base" << std::endl;
			return false;
		}
		m_server_config = new server_base_config();
		if (!m_server_config->parse_by_xml(base_obj))
		{
			std::cout << "servers_config_manager::parse_by_xml fail!! server_base config fail" << std::endl;
			return false;
		}
		for (int32 i = 0; i < e_server_type_max; ++i)
		{
			TiXmlElement* server_elem = xml_child(root, e_server_type_str[i]);
			if (server_elem != nullptr)
			{
				if (!parse_by_xml(server_elem, static_cast<e_server_type>(i)))
				{
					std::cout << "servers_config_manager::parse_by_xml fail!! server:"
						<< e_server_type_str[i] << std::endl;
					return false;
				}
			}
		}
		return true;
	}

	bool servers_config_manager::parse_by_xml(TiXmlElement* element, e_server_type server_type)
	{
		net_config_base* config_ptr = create_server_config(server_type);
		if (nullptr == config_ptr)
		{
			std::cout << "servers_config_manager::parse_by_xml fail!! create_server_config fail!!! server:"
				<< server_type << std::endl;
			return false;
		}
		config_ptr->server_type = server_type;
		if (!config_ptr->parse_by_xml(element))
		{
			delete config_ptr;
			std::cout << "servers_config_manager::parse_by_xml fail!! config xml fail!!! server:"
				<< server_type << std::endl;
			return false;
		}
		m_server_config_array[server_type] = config_ptr;
		return true;
	}

	net_config_base* servers_config_manager::create_server_config(e_server_type server_type)
	{
		switch (server_type)
		{
		case faith::e_server_type_gate:
			return new gate_config();
		case faith::e_server_type_ws:
			return new ws_config();
		case faith::e_server_type_gateway:
			return new gateway_config();
		case faith::e_server_type_cs:
			return new cs_config();
		case faith::e_server_type_dp:
			return new dp_config();
		default:
			break;
		}
		return nullptr;
	}
}
