/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-27 13:40
	
	File Name :	gate_define.h
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/
#ifndef _GATE_DEFINE_H_
#define _GATE_DEFINE_H_

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "server_def.hpp"
#include "Logic/char_def.hpp"
#include "Logic/transfer_def.hpp"
#include "Logic/legion_def.hpp"

//////////////////////////////////////////////////////////////////////////
//	Macro And Struct Define
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	const uint32 TOKEN_LEN = 64;
	const uint32 SIGLE_CROSS_LADDER_EXP_LEVEL = 14;
	const uint32 WS_RANKING_LENGTH = 30;

	typedef std::map<int32, s_game_info> game_info_map;
	typedef game_info_map::iterator game_info_map_it;

	enum GateStatus
	{
		E_GATE_INITED,
		E_GATE_CONNECTED,
		E_GATE_REGISTERED,
		E_GATE_CLOSED,
	};

	enum e_cross_sign_up_result
	{
		e_cross_sign_up_success,
		e_cross_sign_up_leave_success,
		e_cross_sign_up_exp_level,
		e_cross_sign_up_no_in_time,
		e_cross_sign_up_no_cross,
		e_cross_sign_up_cross_server_fail,
		e_cross_sign_up_db_error,
		e_cross_sign_up_template,
		e_cross_sign_up_already,
		e_cross_sign_up_no_role,
		e_cross_sign_up_state_error,
	};

	struct TransferParam
	{
		int32					objType;
		guid_64					objID;
		int32					srcGroupID;
		int32					destGroupID;
		uint32					callbackVal[limit_call_back_val_count];
		s_transfer_info			transfer_info;
		union LogicParam
		{
			struct PlayerParam
			{
				//client_uid	client_uid;
				xchar				account[max_account_length + 1];
				xchar				ipAddr[max_ip_address_length + 1];
				int32				server_id;
			} playerParam;
		} logicParam;

		TransferParam()
		{
			memset(this, 0, sizeof(TransferParam));
		}
	};
}
#endif