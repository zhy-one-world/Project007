
#ifndef __SERVERS_CONFIG_H__
#define __SERVERS_CONFIG_H__

#include "server_def.hpp"
#include <json/json.h>
#include <singleton.hpp>

namespace faith 
{
	class server_base_config
	{
	public:
		server_base_config()
		{
			clear_data();
		};

		virtual ~server_base_config() {};
		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);

		std::string app_key;
		int32 game_id;
		std::string game_name;
		int32 init_player_num;
		bool gm_state;
		std::string robot_account;
		std::string web_address;
		int32 web_port;
		std::string daemon_address;
		int32 daemon_port;
		std::string billing_url;
		std::string recharge_url;
		std::string one_store_recharge_url;
		std::string login_url;
		std::string server_state_url;
		std::string account_info_url;
		std::string http_aes;
		std::string gift_url;
		std::string new_log_file_root;
		bool not_need_db_log;
	};

	class net_config_base
	{
	public:
		net_config_base()
		{
			clear_data();
		};

		virtual ~net_config_base() {};
		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);

		e_server_type server_type;//服务器类型
		std::string internal_host;//内网IP
		int32 internal_port;//内网端口
		std::string external_host;//外网IP
		int32 external_port;//外网端口
	};

	class fep_config : public net_config_base {
	public:
		fep_config() { clear_data(); };
		virtual ~fep_config() {};

		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);

		int32 client_send_limit = 20;//每秒客户端请求次数限制
		int32 init_socket_num = 20;//初始链接数量
	};

	struct gate_config : public net_config_base {
	public:
		gate_config() { clear_data(); };
		virtual ~gate_config() {};
		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);
		int32 gate_id;//每秒客户端请求次数限制
	};

	class cs_config : public net_config_base 
	{
	public:
		cs_config() { clear_data(); };
		virtual ~cs_config() {};
		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);

		int32 role_num_limit = 2400;//最大承载人数
		int32 npc_num_limit = 10000;//最大承载Npc数量
		int32 drop_num_limit = 60000;//最大承载掉落数
		int32 role_data_save_time = 60000;//自动存盘间隔
		int32 role_data_send_time = 60000;//延迟同步间隔
		int32 process_stat_interval = 600;
	};

	struct ws_config : public net_config_base {
	public:
		ws_config() { clear_data(); };
		virtual ~ws_config() {};

		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);

		xstring sdk_url;
		xstring ds_url;
		int32 http_port;
		int32 max_team_user_num;
		int32 max_room_team_num;
		int32 max_room_user_num;
	};

	class dp_config : public net_config_base 
	{
	public:
		struct db_info
		{
			int32 mysql_id;
			std::string ip_addr;
			int32 ip_port;
			std::string user_name;
			std::string user_password;
			std::string schema_name;
			int32 thread_num;
			db_info()
			{
				clear();
			}
			void clear()
			{
				mysql_id = 0;
				ip_addr = "";
				ip_port = 0;
				user_name = "";
				user_password = "";
				schema_name = "";
				thread_num = 1;
			}
		};
	public:
		dp_config() { clear_data(); };
		virtual ~dp_config() {};

		virtual void clear_data();
		virtual bool parse_by_json(Json::Value& json_obj);

		std::map<int32, db_info> m_db_info_map;
		int32 db_error_limit = 10;
		int32 db_thread_num = 18;
		int32 char_byte_size = 1;
		int32 save_open = 0;
		int32 save_interval = 0;
	};

	class servers_config_manager :  public  singleton<servers_config_manager>
	{
		typedef std::map<int32, net_config_base*> server_config_map;
	public:
		servers_config_manager()
		{
			m_self_config = nullptr;
			m_server_config = nullptr;
			memset(m_server_config_array, 0, sizeof(m_self_config));
		};
		bool init_config();
		server_base_config const* get_server_config() { return m_server_config; }
		net_config_base const* get_self_config();
		net_config_base* get_server_config(e_server_type server_type);
	private:
		bool parse_by_json(Json::Value& json_obj);
		bool parse_by_json(Json::Value& json_obj, e_server_type server_type);
		net_config_base* create_server_config(e_server_type server_type);
	private:
		server_base_config* m_server_config;
		net_config_base* m_self_config;
		net_config_base* m_server_config_array[e_server_type_max];
	};

}
#define SERVERCONFIG faith::servers_config_manager::getInstance().get_server_config()
#define FEPCONFIG ((fep_config*)(faith::servers_config_manager::getInstance().get_server_config(e_server_type_fep)))
#define GATECONFIG ((gate_config*)(faith::servers_config_manager::getInstance().get_server_config(e_server_type_gate)))
#define CSCONFIG ((cs_config*)(faith::servers_config_manager::getInstance().get_server_config(e_server_type_cs)))
#define WSCONFIG ((ws_config*)(faith::servers_config_manager::getInstance().get_server_config(e_server_type_ws)))
#define DPCONFIG ((dp_config*)(faith::servers_config_manager::getInstance().get_server_config(e_server_type_dp)))

#endif //! __SERVERS_CONFIG_H__
