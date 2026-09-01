/********************************************************************
	created:	2014/05/17
	created:	17:5:2014   16:31
	file base:	gate
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _FAITH_PROTOCOL_GATE_H_
#define _FAITH_PROTOCOL_GATE_H_

//	protocol define
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/transfer_def.hpp"
#include "Logic/gate_def.hpp"
#include "Logic/role_pk_def.hpp"
#include "Logic/legion_def.hpp"


namespace faith 
{
#pragma pack(push,1)
	//-----------------------------------------------------------------------------------
	//	Declaration of packet index(identifier) value
	//-----------------------------------------------------------------------------------
	enum
	{
		e_msg_server2gate_register			= e_msg_base_gate,
		e_msg_gate2server_rep_register,
		e_msg_gate2server_off_line,
		e_msg_gate2ws_rep_off_line,
		e_msg_index_fep2ws_check_token,
		e_msg_gate2ws_rep_token,
		e_msg_server2gate_send_to_server,
		e_msg_gate2ws_rep_check_from_ws,
		e_msg_ws2gate_cross_info,
		e_msg_gate2ws_server_id,
		e_msg_ws2gate_is_begin_cross,
		e_msg_ws2gate_all_gs_ranking,
		e_msg_ws2gate_gs_ranking,
		e_msg_gate2ws_gs_ranking,
		e_msg_ws2gate_my_rank,
		e_msg_gate2ws_other_ws_connect_gatews,
		e_msg_ws2gate_cross_boss_ranking,
		e_msg_ws2gate_time_limit_get_rank_info,
		e_msg_ws2gate_change_cross_id,
		e_msg_gate2ws_change_cross_id,
		e_msg_gate2ws_all_server_arr,
		e_msg_gate2ws_server_info,
		e_msg_gate2gate_new,
		e_msg_ws2gate_flag_load_end,
		e_msg_gate2ws_end_refresh_list,
		e_msg_ws2gate_server_hide_act,
		e_msg_gate2ws_server_hide_act,
		e_msg_server2gate_reload_csv,
		e_msg_gate2ws_reload_csv,
		e_msg_server2gate_reload_csv_end,
	};

	//-----------------------------------------------------------------------------------
	//	const value defines
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	//	Uniform result code
	//-----------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------
	//	可传送的对象类型枚举
	//-----------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------
	//	Define of packet struct
	//-----------------------------------------------------------------------------------	
	struct server2gate_reload_csv : public packet_base
	{
		int32 need_reload;
		server2gate_reload_csv()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_server2gate_reload_csv;
		}
	};
	struct server2gate_reload_csv_end : public packet_base
	{
		int32 server_id;
		xchar out_ip[max_ip_address_length + 1];
		void set_out_ip(xstring _out_ip)
		{
			memset(out_ip, 0, sizeof(out_ip));
			memcpy(out_ip, _out_ip.c_str(), _out_ip.size() > max_ip_address_length ? max_ip_address_length : _out_ip.size());
		}
		server2gate_reload_csv_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_server2gate_reload_csv_end;
		}
	};
	struct gate2server_reload_csv : public packet_base
	{
		int32 need_reload;
		gate2server_reload_csv()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_reload_csv;
		}
	};
	struct server2gate_req_register : public packet_base
	{
		s_game_info game_info;
		e_server_type server_type;
		int32 server_index;
		int32 recv_server_id;
		server2gate_req_register()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_server2gate_register;
		}
	};
	struct gate2server_req_off_line : public packet_base
	{
		int32 server_id;
		gate2server_req_off_line()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2server_off_line;
		}
	};

	struct server2gate_to_server : public packet_base
	{
		int32			recv_server_id;
		e_server_type	recv_server_type;
		int32			recv_server_index;
		int32			send_server_id;
		e_server_type	send_server_type;
		int32			send_server_index;
		uint32			header;
		uint32			dataLen;
		xchar			data[INTERNAL_SERVER_MAX_PACKET_SIZE + 1];
		server2gate_to_server()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_server2gate_send_to_server;
		}

		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&data - (ULONG_PTR)&wheader;
			return basic_len + dataLen;
		}
	};

	struct gate2gate_req_new : public packet_base
	{
		bool is_create;
		s_server_info server_info;
		gate2gate_req_new()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2gate_new;
		}
	};

	struct gate2server_to_server : public packet_base
	{
		guid_64			role_guid;
		int32			server_id;
		uint32			header;
		uint32			dataType;
		uint32			dataLen;
		xchar			data[INTERNAL_SERVER_MAX_PACKET_SIZE + 1];
		gate2server_to_server()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_rep_check_from_ws;
		}

		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&data - (ULONG_PTR)&wheader;
			return basic_len + dataLen;
		}
	};
	struct gate2ws_server_id : public packet_base
	{
		int32					server_id;

		gate2ws_server_id()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_server_id;
		}
	};

	struct ws2gate_all_gs_ranking : public packet_base
	{
		int32					rank_index;
		int32					server_id;
		guid_64					role_guid;
		ws2gate_all_gs_ranking()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_all_gs_ranking;
		}
	};

	struct ws2gate_gs_ranking : public packet_base
	{
		int32							ranking_index;
		int32							data_num;
		int32							server_id;
		int32							if_first_send;	//是否是最后一次发送
		int32							if_last_send;	//是否是最后一次发送	

		s_ranking_player_info			data_ary[WS_RANKING_LENGTH];
		ws2gate_gs_ranking()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_gs_ranking;
		}
		void clear_data_array()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};

	struct ws2gate_cross_boss_ranking : public packet_base
	{
		int32							ranking_index;
		int32							data_num;
		int32							server_id;
		s_ranking_player_info			data_ary[WS_RANKING_LENGTH];
		int32							if_first_send;
		ws2gate_cross_boss_ranking()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_cross_boss_ranking;
		}
	};
	struct gate2ws_gs_ranking : public packet_base
	{
		int32							server_index;
		gate2ws_gs_ranking()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_gs_ranking;
		}
	};
	struct ws2gate_my_rank : public packet_base
	{
		guid_64							player_guid;
		guid_64							sender_guid;
		int32							server_id;
		int32							rank_type;
		ws2gate_my_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_my_rank;
		}
	};

	struct ws2gate_time_limit_get_rank_info : public packet_base
	{
		int32					activity_type;
		int32					server_id;
		guid_64					role_guid;
		ws2gate_time_limit_get_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_time_limit_get_rank_info;
		}
	};

	struct ws2gate_change_cross_id : public packet_base
	{
		int32					cur_server_id;
		int32					cross_id;
		int32					old_cross_id;
		ws2gate_change_cross_id()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_change_cross_id;
		}
	};

	struct ws2gate_server_act_hide: public packet_base
	{
		bool					is_need_hide;
		ws2gate_server_act_hide()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_server_hide_act;
		}
	};

	struct gate2ws_server_act_hide : public packet_base
	{
		bool					is_need_hide;
		gate2ws_server_act_hide()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_server_hide_act;
		}
	};


	struct ws2gate_flag_load_end : public packet_base
	{
		int32					cur_server_id;
		ws2gate_flag_load_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2gate_flag_load_end;
		}
	};

	struct gate2ws_end_refresh_list : public packet_base
	{
		gate2ws_end_refresh_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_end_refresh_list;
		}
	};

	struct gate2ws_change_server_id : public packet_base
	{
		s_game_info				server_info;
		bool					is_add;
		gate2ws_change_server_id()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_change_cross_id;
		}
	};

	struct gate2ws_all_server_arr : public packet_base
	{
		s_game_info				server_info[max_server_num];
		int32					server_num;
		gate2ws_all_server_arr()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_all_server_arr;
		}
	};

	struct gate2ws_server_info : public packet_base
	{
		int32					server_info_type;
		int32					server_info_value;
		gate2ws_server_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_server_info;
		}
	};
#pragma pack(pop)
}

#endif
