/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 21:22
  file base: internal
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _INTERNAL_HPP_
#define _INTERNAL_HPP_

#include "logic/type_def.hpp"
#include "Logic/transfer_def.hpp"
#include "Logic/char_def.hpp"
#include "base.hpp"

namespace faith 
{
#pragma pack(push,1)
	//	用于服务器组内部各服务器间建立连接的协议定义
	enum
	{
		e_msg_index_req_login = e_msg_base_internal,
		e_msg_index_rep_login,
		e_msg_index_req_stop,

		e_msg_index_appserver_dataupdate,
		e_msg_index_reload_csv,
		e_msg_index_cs_reload_csv_end,
	};
				
	struct req_login : public packet_base
	{
		s_server_info		server_info;
		req_login()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_req_login;
		}
	};

	struct rep_login :public packet_base
	{
		int32 cross_server_id;
		int32 open_time;
		rep_login()
		{
			wheader=e_msg_index_rep_login;
			cross_server_id = 0;
			open_time = 0;
		}
	};
	struct req_stop : public packet_base
	{
		e_server_type server_type;
		req_stop()
		{
			server_type = e_server_type_invalid;
			wheader = e_msg_index_req_stop;
		}
	};

	struct app_server_update : public packet_base
	{
		int32 player_count;			// 当前玩家数量
		int32 max_player_count;		// 可容载最大玩家数量
		app_server_update()
		{
			wheader=e_msg_index_appserver_dataupdate;
			player_count = 0;
			max_player_count = 0;
		}
	};
	struct reload_csv : public packet_base
	{
		reload_csv()
		{
			wheader = e_msg_index_reload_csv;
		}
	};

	struct cs_reload_csv_end : public packet_base
	{
		cs_reload_csv_end()
		{
			wheader = e_msg_index_cs_reload_csv_end;
		}
	};
#pragma pack(pop)
}

#endif
