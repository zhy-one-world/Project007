/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 19:49
  file base: base
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _BASE_HPP_
#define _BASE_HPP_

#include "logic/type_def.hpp"
#include "server_def.hpp"

namespace faith 
{
#pragma pack(push,1)

	const int32	e_protocol_version_major = 14;		//	协议主版本号
	const int32	e_protocol_version_minor = 0;		//	协议次版本号

	const ui16	server_header_verstion = e_protocol_version_major * 100 + e_protocol_version_minor;
	enum
	{
		/************************************************************************/
		/*                      FAITH 网络层保留的协议号区间                    */
		/************************************************************************/
		e_protocol_reserved_low = 0,		//	net 保留的协议号，最小值
		e_protocol_reserved_high = 999,		//	net 保留的协议号，最大值
		e_msg_applayer_base = 1000,			//	应用层协议起始号码
		/************************************************************************/
		/*                          FAITH 应用层协议分段                        */
		/************************************************************************/
		e_msg_base_internal = e_msg_applayer_base,
		// log related
		e_msg_base_all2dp = e_msg_applayer_base + 98,
		e_msg_base_dp2all = e_msg_applayer_base + 99,

		e_msg_base_gate = e_msg_applayer_base + 100,
		// logic related
		e_msg_base_login = e_msg_applayer_base + 200,
		e_msg_base_character = e_msg_applayer_base + 300,
		e_msg_base_entergame = e_msg_applayer_base + 500, // 这个是core.h里面的消息包用的 需要预留多一点 500~800
		e_msg_base_item = e_msg_applayer_base + 800,
		e_msg_base_skill = e_msg_applayer_base + 900,
		e_msg_base_att = e_msg_applayer_base + 1000,
		e_msg_base_buff = e_msg_applayer_base + 1100,
		e_msg_base_pvp = e_msg_applayer_base + 1200,
		e_msg_base_map = e_msg_applayer_base + 1400,
		e_msg_base_pay = e_msg_applayer_base + 1500,
		e_msg_base_shop = e_msg_applayer_base + 1600,
		e_msg_base_sas_eye_basic = e_msg_applayer_base + 1700,//sas与eye通信协议号段1，基础功能
		e_msg_base_sas_daemon = e_msg_applayer_base + 1800,//sas与daemon通信协议号段
		e_msg_base_sas_eye_popedom = e_msg_applayer_base + 1900,//sas与eye通信协议号段2，管理员功能
		e_msg_base_daemon_ws = e_msg_applayer_base + 2000,//daemon与ws通信协议号段
		//e_msg_base_mine = e_msg_applayer_base + 2000,
		e_msg_base_spirit = e_msg_applayer_base + 2200,
		e_msg_c2s_s2c_begin = e_msg_applayer_base + 9000,
		e_msg_c2s_s2c_end = e_msg_applayer_base + 29000,
		e_msg_base_belief = e_msg_applayer_base + 31100,
		e_msg_base_mission = e_msg_applayer_base + 31200,
		e_msg_base_goods = e_msg_applayer_base + 31300,
		e_msg_base_element_heart = e_msg_applayer_base + 31400,
		e_msg_base_special_name = e_msg_applayer_base + 31500,
		e_msg_base_achievement = e_msg_applayer_base + 31600,
		e_msg_base_pokedex = e_msg_applayer_base + 31700,
		e_msg_base_mail = e_msg_applayer_base + 31800,
		e_msg_base_arena = e_msg_applayer_base + 31900,
		e_msg_base_event = e_msg_applayer_base + 32000,
		e_msg_base_ranking = e_msg_applayer_base + 32100,
		e_msg_base_tinder = e_msg_applayer_base + 32200,
		e_msg_base_chat = e_msg_applayer_base + 32300,
		e_msg_base_convert = e_msg_applayer_base + 32400,
		e_msg_base_legion = e_msg_applayer_base + 32500,
		e_msg_base_welfare = e_msg_applayer_base + 32600,
		e_msg_base_role_pk = e_msg_applayer_base + 32700,
		e_msg_base_world_boss = e_msg_applayer_base + 32800,
		e_msg_base_best_record = e_msg_applayer_base + 32900,
		e_msg_big_player = e_msg_applayer_base + 33000,
		e_msg_base_title = e_msg_applayer_base + 33100,
		e_msg_base_guide = e_msg_applayer_base + 33200,
		e_msg_base_first_time_do = e_msg_applayer_base + 33300,
		e_msg_base_talent = e_msg_applayer_base + 33400,
		e_msg_base_lucky_draw_record = e_msg_applayer_base + 33500,
		e_msg_base_service_goal = e_msg_applayer_base + 33600,
		e_msg_base_auction = e_msg_applayer_base + 33700,
		e_msg_base_save_log = e_msg_applayer_base + 33800,
		e_msg_base_marry = e_msg_applayer_base + 33900,
		e_msg_base_service_rank = e_msg_applayer_base + 34000,
		e_msg_base_gain_treasure = e_msg_applayer_base + 34100,
		e_msg_base_treasure = e_msg_applayer_base + 34200,
		e_msg_base_cloud_shop = e_msg_applayer_base + 34300,
		e_msg_base_time_limit_activity = e_msg_applayer_base + 34400,
		e_msg_base_server_merge = e_msg_applayer_base + 34500,
		e_msg_base_boss_island = e_msg_applayer_base + 34600,
		e_msg_base_belief_rune = e_msg_applayer_base + 34700,
		e_msg_base_cross_server_pk = e_msg_applayer_base + 34800,
		e_msg_base_seven_day_goal = e_msg_applayer_base + 34900,
		e_msg_base_cross_server_harry = e_msg_applayer_base + 35000,
		e_msg_base_cross_ladder = e_msg_applayer_base + 35100,
		e_msg_base_cross_territory_war = e_msg_applayer_base + 35200,
		e_msg_base_cross_server_world_boss = e_msg_applayer_base + 35300,
		e_msg_base_phantom = e_msg_applayer_base + 35400,
		e_msg_oracle_trial = e_msg_applayer_base + 35500,
		e_msg_base_recycle = e_msg_applayer_base + 35600,
		e_msg_base_world_server = e_msg_applayer_base + 35700,
		e_msg_legion_station = e_msg_applayer_base + 35800,
		e_msg_appearance_info = e_msg_applayer_base + 35900,
		e_msg_base_pk_king = e_msg_applayer_base + 36000,
		e_msg_base_element_war = e_msg_applayer_base + 36100,
		e_msg_base_attack_city = e_msg_applayer_base + 36200,
		e_msg_base_max = 65535
	};

	#define MAX_C2S_S2C_PACKAGE_SIZE 12*1024
	#define MAX_S2W_W2S_PACKAGE_SIZE 16*1024
	
	#define IN_DEBUG_MODE //非debug模式注释掉这行?


	struct s_client_uid
	{
		s_client_uid(ui32 server_uid, ui16 session_uid) : fepserver_uid(server_uid), fepsession_uid(session_uid)
		{
			rand_num_1 = rand();
		}
		s_client_uid() : fepserver_uid(0), fepsession_uid(0)
		{
			rand_num_1 = rand();
		}
		explicit s_client_uid(ui64 InValue) : fep_uid_64(InValue)
		{
		}
		bool operator == (const s_client_uid& o) const
		{
			return fep_uid_64 == o.fep_uid_64;
		}
		bool operator != (const s_client_uid& o) const
		{
			return fep_uid_64 != o.fep_uid_64;;
		}
		operator ui64() const
		{
			return fep_uid_64;
		}
		bool is_valid()
		{
			return fep_uid_64 > 0;
		}
		void clear_data()
		{
			fep_uid_64 = 0;
		}
		union
		{
			struct
			{
				ui32	fepserver_uid;
				ui16	fepsession_uid;
				ui16	rand_num_1;
			};
			ui64 fep_uid_64;
		};
	};


	//	base struct for all packets
	struct packet_base
	{
		packet_base()
		{
			wheader		 = 0;
			wheader_plus = 0;
			header_verstion = server_header_verstion;
		}
		ui16	wheader;
		ui8		wheader_plus;
		ui16	header_verstion;
	};

	struct packet_c2s_s2c : public packet_base
	{
		packet_c2s_s2c()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			header_verstion = server_header_verstion;
		}
		int32 get_packet_len()
		{
			return sizeof(packet_base) + sizeof(google_data_len)+ google_data_len + sizeof(s_client_uid);
		}

		int32 get_google_data_len()
		{
			return google_data_len < MAX_C2S_S2C_PACKAGE_SIZE ? google_data_len : MAX_C2S_S2C_PACKAGE_SIZE;
		}		
		s_client_uid	client_uid;
		int32 google_data_len;
		ui8 google_data[MAX_C2S_S2C_PACKAGE_SIZE];
	};

	struct packet_s2s : public packet_base
	{
		packet_s2s()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			header_verstion = server_header_verstion;
		}
		int32 get_packet_len()
		{
			const int32 basic_len = (ULONG_PTR)&google_data - (ULONG_PTR)&wheader;
			return (basic_len + google_data_len);
		}

		int32 get_google_data_len()
		{
			return google_data_len < INTERNAL_SERVER_MAX_PACKET_SIZE ? google_data_len : INTERNAL_SERVER_MAX_PACKET_SIZE;
		}
		guid_64 logic_guid;
		int32 logic_index;
		int32 logic_ex;
		int32 data_flag;
		int32 google_data_len;
		unsigned char google_data[INTERNAL_SERVER_MAX_PACKET_SIZE];
	};
#pragma pack(pop)
}

#endif
