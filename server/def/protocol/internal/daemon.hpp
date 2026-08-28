#ifndef _FAITH_SHOP_H
#define  _FAITH_SHOP_H

#include "Logic/att_def.hpp"
#include "Logic/char_def.hpp"
#include "logic/type_def.hpp"
#include "base.hpp"

namespace hld
{
#pragma pack(push,1)

	enum {
		e_msg_index_ws2dp_request_query_role_list_match = hld::e_msg_base_daemon_ws,
		e_msg_index_dp2ws_response_query_role_list_match,
		e_msg_index_ws2dp_req_query_server_info_gm,
		e_msg_index_dp2ws_rep_query_server_info_gm,
	};

	const int32 max_string_lenth = 128;

	/************************************************************************/
	/*                           shop                                       */
	/************************************************************************/
	struct ws2dp_request_query_role_list_match : public hld::packet_base
	{
		xchar	name_mode[max_name_size + 1];

		void set_name_mode(xstring p_role_name)
		{
			memcpy(name_mode, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}

		ws2dp_request_query_role_list_match()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2dp_request_query_role_list_match;
		}
	};

	struct role_info_in_list_match
	{
		xchar		info[max_string_lenth + 1];
		xchar		role_name[max_name_size + 1];
		guid_64		role_guid;
		xchar		account[max_account_length + 1];		
		uint32		money;
		uint32		jewel;
		uint32		exp;
		uint32		exp_level;
		uint32		born_time;
		uint32		last_login_time;
		bool		online;

		void set_info(xstring p_info)
		{
			memcpy(info, p_info.c_str(), p_info.size() > max_string_lenth ? max_string_lenth : p_info.size());
		}

		void set_role_name(xstring p_role_name)
		{
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}

		void set_account(xstring p_account){
			memcpy(account, p_account.c_str(), p_account.size() > max_account_length ? max_account_length : p_account.size());
		}

		role_info_in_list_match()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct dp2ws_response_query_role_list_match : public hld::packet_base
	{
		ui8 role_num;
		role_info_in_list_match role_arr[10];  //最多返回10个角色信息

		dp2ws_response_query_role_list_match()
		{
			memset(this,0,sizeof(*this));
			wheader = e_msg_index_dp2ws_response_query_role_list_match;
		}
	};
	
	//struct ws2cs_gmtool_req_kickout_player : public packet_base
	//{
	//	guid_64	player_guid;

	//	ws2cs_gmtool_req_kickout_player()
	//	{
	//		memset(this, 0, sizeof(this));
	//		wheader = e_msg_index_ws2cs_gmtool_req_kickout_player;
	//	}
	//};

	struct server_info_gm
	{
		server_info_gm ()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		
		int32			acc_num;
		int32			role_num;
	};
	
	struct ws2dp_query_server_info_gm : public hld::packet_base
	{
		ws2dp_query_server_info_gm()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2dp_req_query_server_info_gm;
		}
	};
	
	struct dp2ws_query_server_info_gm : public hld::packet_base
	{
		server_info_gm srv_gm_info;
		dp2ws_query_server_info_gm()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_dp2ws_rep_query_server_info_gm;
		}
	};



#pragma pack(pop)

}
#endif