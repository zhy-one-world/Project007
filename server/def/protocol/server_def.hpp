/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 19:30
  file base: serverdef
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _SERVER_DEF_HPP_
#define _SERVER_DEF_HPP_

#include "logic/type_def.hpp"
#include <net/tcp_client.hpp>
#include <net/tcp_server.hpp>
#include "logic/char_def.hpp"
#include "internet/st.pb.h"

namespace faith 
{
#pragma pack(push,1)

	const int32 server_group_level_num = 10000;
	const int32 server_uid_level_num = 100000000;
	const uint32 max_ip_address_length = 30;						// ip地址的长度
	const uint32 max_ip_port_length = 10;							// ip端口的长度
	const int32 max_db_user_name = 100;								// 数据库用户名
	const int32 max_db_password_name = 100;							// 数据库密码
	const int32 max_db_schema_name = 50;							// 数据库名
	const int32 max_server_num = 11;								// 一个跨服群最多多少个服务器
	const int32 max_robot_num = 2000;								// 一个exe最多启动多少机器人
	const int32	max_gate_server_num = 500;							// 服务器最大记录数量

	//common
	#define MAX_ERVER_STATUS_LOG_BUFFER 256
	#define SERVER_RES_PATH "../res"
	#define SERVER_SCHEDULER_THREAD_COUNT 18
	#define MYSQL_CONNECT_SCHEDULER_THREAD_COUNT 60
	#define INTERNAL_SERVER_MAX_PACKET_SIZE 1024*1024
	//ws
	#define WS_SERVER_SCHEDULER_THREAD_COUNT 18
	#define WS_SERVER_SEND_BUFF_SIZE	40*1024*1024
	#define WS_SERVER_RECV_BUFF_SIZE	40*1024*1024
	#define WS_CLIENT_SEND_BUFF_SIZE	40*1024*1024
	#define WS_CLIENT_RECV_BUFF_SIZE	40*1024*1024

	//cs
	#define CS_SERVER_SCHEDULER_THREAD_COUNT 18
	#define CS_SERVER_SEND_BUFF_SIZE	64*1024*1024
	#define CS_SERVER_RECV_BUFF_SIZE	64*1024*1024
	#define CS_CLIENT_SEND_BUFF_SIZE	32*1024*1024
	#define CS_CLIENT_RECV_BUFF_SIZE	32*1024*1024
	//dp
	#define DP_SERVER_SCHEDULER_THREAD_COUNT 18
	#define DP_SERVER_SEND_BUFF_SIZE	64*1024*1024
	#define DP_SERVER_RECV_BUFF_SIZE	64*1024*1024
	#define DP_CLIENT_SEND_BUFF_SIZE	64*1024*1024
	#define DP_CLIENT_RECV_BUFF_SIZE	64*1024*1024
	//fep
	#define FEP_SERVER_SCHEDULER_THREAD_COUNT 255
	#define FEP_NETWORK_SCHEDULER_THREAD_ID 1
	#define FEP_ACCEPTOR_SCHEDULER_THREAD_ID 0
	#define FEP_CLIENT_SEND_BUFF_SIZE	32*1024*1024
	#define FEP_CLIENT_RECV_BUFF_SIZE	32*1024*1024
	#define FEP_INTERNAL_SERVER_SEND_BUFF_SIZE	128*1024*1024
	#define FEP_INTERNAL_SERVER_RECV_BUFF_SIZE	128*1024*1024
	#define FEP_INTERNET_MAX_PACKAGE_SIZE	16*1024
	#define FEP_INTERNET_SERVER_SEND_BUFF_SIZE	1024*1024
	#define FEP_INTERNET_SERVER_RECV_BUFF_SIZE	1024*1024
	//daemon
	#define DAEMON_SERVER_SEND_BUFF_SIZE	16*1024*1024
	#define DAEMON_SERVER_RECV_BUFF_SIZE	16*1024*1024
	//gate
	#define GATE_SERVER_SCHEDULER_THREAD_COUNT 18
	#define GATE_CLIENT_SEND_BUFF_SIZE	16*1024*1024
	#define GATE_CLIENT_RECV_BUFF_SIZE	16*1024*1024
	#define	GATE_SEVER_RECV_BUFFER_SIZE 16*1024*1024
	#define	GATE_SEVER_SEND_BUFFER_SIZE 16*1024*1024

	enum
	{
		e_invalid_server_uid = 0xFFFFFFFF,
		e_invalid_server_pid = 0x0,
	};

	//	server type define
	enum e_server_type
	{
		e_server_type_invalid,		//无效
		e_server_type_gate,			//gate服
		e_server_type_ws,		//Ws
		e_server_type_fep,			//Fep
		e_server_type_ls,		//Ls
		e_server_type_cs,			//Cs
		e_server_type_dp,		//Db
		e_server_type_deamon,
		e_server_type_gmt,
		e_server_type_billing,
		e_server_type_max
	};

	const xchar* const e_server_type_str[] =
	{
		"invalid",
		"gate",
		"ws",
		"fep",
		"ls",
		"cs",
		"dp",
		"deamon",			
		"gmt",				
		"billing",			
	};
	const int32  SERVER_WS_COUNT = 1;
	const int32  SERVER_CS_COUNT = 1;
	const int32  SERVER_DP_COUNT = 1;
	const int32  SERVER_DAEMON_COUNT = 1;
	const int32  SERVER_GATE_COUNT = 4;
	const int32  SERVER_MAIN_GATE_COUNT = 1;
	const int32  SERVER_FEP_COUNT = 2;
	const int32  SERVER_GMT_COUNT = 1;
	const int32  SERVER_BILLING_COUNT = 1;
	// ws
	const int32  WS_NEED_SERVER_COUNT = SERVER_FEP_COUNT + SERVER_CS_COUNT;
	const int32  WS_NEED_CLIENT_COUNT = SERVER_DP_COUNT + SERVER_DAEMON_COUNT + SERVER_GMT_COUNT + SERVER_GATE_COUNT;
	// cs
	const int32 CS_NEED_SERVER_COUNT = SERVER_FEP_COUNT;
	const int32 CS_NEED_CLIENT_COUNT = SERVER_WS_COUNT + SERVER_DP_COUNT + SERVER_DAEMON_COUNT + SERVER_GATE_COUNT;
	// dp
	const int32 DP_NEED_SERVER_COUNT = SERVER_WS_COUNT + SERVER_CS_COUNT;
	const int32 DP_NEED_CLIENT_COUNT = SERVER_DAEMON_COUNT + SERVER_GATE_COUNT;
	// fep
	const int32 FEP_NEED_CLIENT_COUNT = SERVER_WS_COUNT + SERVER_CS_COUNT + SERVER_DAEMON_COUNT;
	// daemon
	const int32 DAEMON_NEED_SERVER_COUNT = SERVER_WS_COUNT + SERVER_DP_COUNT + SERVER_CS_COUNT + SERVER_FEP_COUNT + SERVER_GMT_COUNT;
	// gate
	const int32 GATE_NEED_SERVER_COUNT = (SERVER_WS_COUNT + SERVER_DP_COUNT + SERVER_CS_COUNT) * 200 + SERVER_GATE_COUNT;
	const int32 GATE_NEED_CLIENT_COUNT = SERVER_DAEMON_COUNT + SERVER_MAIN_GATE_COUNT;



	const int32 DAEMON_SERVER_STATE_PARAM_LENTH = 512;

	enum e_server_status
	{
		e_serverstatus_created = 0,
		e_serverstatus_initialized = 1,
		e_serverstatus_working = 2,
	};

	struct  s_server_info
	{
		s_server_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		e_server_type  server_type;
		int32 server_index;
		xchar ip_addr[max_ip_address_length + 1];
		uint32 port;
	};
	struct s_game_info
	{
		s_game_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		int32 server_id;
		xchar out_ip[max_ip_address_length + 1];
		int32 out_port;
		xchar server_name[max_name_size + 1];
		int32 cross_id;
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		bool to_proto(faith::st_proto::st_game_info *st_game_ptr)
		{
			if (st_game_ptr == nullptr)
			{
				return false;
			}
			st_game_ptr->set_server_id(server_id);
			st_game_ptr->set_out_ip(out_ip);
			st_game_ptr->set_out_port(out_port);
			st_game_ptr->set_server_name(server_name);
			st_game_ptr->set_cross_id(cross_id);
			return true;

		}
		void from_proto(const faith::st_proto::st_game_info &st_game_ref)
		{
			server_id = st_game_ref.server_id();
			my_memcopy_string(out_ip, max_ip_address_length, st_game_ref.out_ip());

			out_port = st_game_ref.out_port();
			my_memcopy_string(server_name, max_name_size, st_game_ref.server_name());
			cross_id = st_game_ref.cross_id();
		}
	};
	struct s_db_conn_info
	{
		s_db_conn_info()
		{
			memset(this, 0, sizeof(s_db_conn_info));
		}

		xchar game_name[max_db_schema_name + 1];
		xchar game_ip[max_ip_address_length + 1];
		int32 game_port;
		xchar game_user_name[max_db_user_name + 1];
		xchar game_user_password[max_db_password_name + 1];
		xchar log_name[max_db_schema_name + 1];
		xchar log_ip[max_ip_address_length + 1];
		int32 log_port;
		xchar log_user_name[max_db_user_name + 1];
		xchar log_user_password[max_db_password_name + 1];
	};
	struct s_server_config
	{
		s_server_config()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_server_config));
		}
		bool is_vaild() const
		{
			return server_id > 0;
		}
		xchar server_name[max_name_size + 1];
		int32 server_id;
		xchar server_ip[max_ip_address_length + 1];
		xchar out_ip[max_ip_address_length + 1];
		xchar out_port[max_ip_port_length + 1];
		bool is_cross_server;
		xchar db_ip[max_ip_address_length + 1];
		xchar dp_port[max_ip_port_length + 1];
	};

	enum server_attr_val_type
	{
		e_srv_attr_val_refresh_zero, //0点刷新时间戳
		e_srv_attr_val_refresh_five, //5点刷新时间戳
		e_srv_attr_val_world_lv_cur, //当前世界等级
		e_srv_attr_val_world_lv_last, //上一次世界等级
		e_srv_attr_val_service_rank_check, //全民冲榜需要检测的类型
		e_srv_attr_val_total_recharge_rank_refresh_time, //累计充值排行刷新时间
		e_srv_attr_val_total_cost_rank_refresh_time, //累计消费排行刷新时间
		e_srv_attr_val_total_recharge_rank_branch_template_id, //累计充值排行分支id
		e_srv_attr_val_total_cost_rank_branch_template_id, //累计消费排行分支id
		e_srv_attr_val_server_harry_refresh_time,//跨服掠夺刷新时间戳
		e_srv_attr_val_is_refresh_cross_city_war,//是否已经刷新最新的霸主战
		e_srv_attr_val_total_time_limit_activity_recharge,//限时活动全服累计充值数量
		e_srv_attr_val_gs_up_rank_refresh_time,					//战力提升排行刷新时间
		e_srv_attr_val_gs_up_rank_branch_template_id,			//战力提升排行分支id
		e_srv_attr_val_mount_up_rank_refresh_time,				//坐骑提升排行刷新时间
		e_srv_attr_val_mount_up_rank_branch_template_id,		//坐骑提升排行分支id
		e_srv_attr_val_wing_up_rank_refresh_time,				//翅膀提升排行刷新时间
		e_srv_attr_val_wing_up_rank_branch_template_id,			//翅膀提升排行分支id
		e_srv_attr_val_equip_up_rank_refresh_time,				//装备提升排行刷新时间
		e_srv_attr_val_equip_up_rank_branch_template_id,		//装备提升排行分支id
		e_srv_attr_val_spirit_up_rank_refresh_time,				//精灵提升排行刷新时间
		e_srv_attr_val_spirit_up_rank_branch_template_id,		//精灵提升排行分支id
		e_srv_attr_val_cross_gs_up_rank_refresh_time,			//跨服战力提升排行刷新时间
		e_srv_attr_val_cross_gs_up_rank_branch_template_id,		//跨服壮丽提升排行分支id
		e_srv_attr_val_cross_mount_up_rank_refresh_time,		//跨服坐骑提升排行刷新时间
		e_srv_attr_val_cross_mount_up_rank_branch_template_id,	//跨服坐骑提升排行分支id
		e_srv_attr_val_cross_wing_up_rank_refresh_time,			//跨服翅膀提升排行刷新时间
		e_srv_attr_val_cross_wing_up_rank_branch_template_id,	//跨服翅膀提升排行分支id
		e_srv_attr_val_cross_equip_up_rank_refresh_time,		//跨服装备提升排行刷新时间
		e_srv_attr_val_cross_equip_up_rank_branch_template_id,	//跨服装备提升排行分支id
		e_srv_attr_val_cross_spirit_up_rank_refresh_time,		//跨服精灵提升排行刷新时间
		e_srv_attr_val_cross_spirit_up_rank_branch_template_id,	//跨服精灵提升排行分支id
		e_srv_attr_val_broken_sky_boss_level_last,				//上次没被击杀的破碎虚空boss等级
		e_srv_attr_val_week_contribution_refresh_time,			//周贡献刷新时间点 暂时不使用
		e_srv_attr_val_clear_city_war_bid_time,					//城战清空刷新时间带你
		e_srv_attr_val_week_welfare_refresh_time,				//军团福利刷新时间点
		e_srv_attr_val_ladder_world_level,						//天梯记录等级
		e_srv_attr_val_max
	};

	enum srv_refresh_type
	{
		e_refresh_type_zero,//0点刷新
		e_refresh_type_five,//5点刷新
	};

	struct  s_time_count
	{
		s_time_count()
		{
			memset(this, 0, sizeof(*this));
		}
		int32 m_time_role_count[max_time_role_count];
	};
	typedef std::map<xstring, s_time_count> time_cout_map;
	typedef time_cout_map::iterator time_cout_map_it;
	typedef std::map<xstring, int32> channel_cout_map;
	typedef channel_cout_map::iterator channel_cout_map_it;
#pragma pack(pop)
}

#endif
