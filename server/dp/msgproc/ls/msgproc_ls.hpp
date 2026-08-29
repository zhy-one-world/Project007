/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:52
	file base:	msgproc_ls
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_LS_H_
#define _MSGPROC_LS_H_

#include "db_config.hpp"
#include "logic/type_def.hpp"
#include "server_def.hpp"
#include <login_msg.hpp>
#include "Internet/character.pb.h"

namespace faith
{
	class db_object;

	//获得账号登录数据
	void ls2dp_client_login_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ls2dp_client_main_check_login_end(db_result_type result, uint32 connindex, const ls2dp_client_login msg_data);
	void ls2dp_client_login_end(db_result_type result, uint32 connindex, const ls2dp_client_login msg_data);

	//处理老服问题，当查询不到玩家所在服时，则直接去主服查找

	void ls2dp_req_createchar_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ls2dp_req_createchar_end(db_result_type result,guid_64 charid,uint32 connindex, s_client_uid client_uid, s_unit_info role_info, const ui8* login_proto_msg, int32 login_msg_len, int32 server_id);

	void ls2dp_req_deletechar_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ls2dp_req_deletechar_end(db_result_type result,uint32 connindex, s_client_uid client_uid, guid_64 role_guid, e_del_char_type del_type);

	void ls2dp_req_enum_char(uint32 connindex, const void* data_ptr, size_t data_len);
	void ls2dp_req_enumchar_end(db_result_type result,uint32 connindex, s_client_uid client_uid, ui16 uCurrentPage, int32 server_id, xstring account);

	void load_enum_item(int32 server_id, guid_64& role_guid, int32 wing_id, int32 mount_id, uint32 connindex, s_client_uid client_uid, int32 wing_illusion_range_id = 0, int32 mount_illusion_range_id = 0);
	void load_enum_item_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid);

	void load_enum_buff(int32 server_id, guid_64& role_guid, uint32 connindex, s_client_uid client_uid);
	void load_enum_buff_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid);

	void load_enum_spirit(int32 server_id, guid_64& role_guid, uint32 connindex, s_client_uid client_uid);
	void load_enum_spirit_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid);

	void load_enum_time(int32 server_id, guid_64& role_guid, uint32 connindex, s_client_uid client_uid);
	void load_enum_time_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid);

}

#endif