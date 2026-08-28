/********************************************************************
	created:	2016Äê12ÔÂ12ÈÕ12:53:46
	file base:	msgproc_mail_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_RED_PACKAGE_WS_H_
#define _MSGPROC_RED_PACKAGE_WS_H_

#include "db_manager.hpp"
#include <mail_msg.hpp>
#include <event_msg.hpp>
namespace hld
{
	void save_red_package_to_sql(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_red_package_by_sql(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_red_package_receiver_by_sql(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_red_package_end(db_result_type result, uint32 connindex);
	void load_red_package_receiver_end(db_result_type result, uint32 connindex);
	void sub_save_red_package_base_info(server2dp_proto_s_red_package data_list[red_package_total_num], int32 data_num);
	void sub_save_red_package_receiver(server2dp_proto_s_red_package data_list[red_package_total_num], int32 data_num);
	void add_red_package_to_sql_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void add_red_package_receiver_to_sql_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void del_red_package_by_sql_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	void save_red_package_to_sql_lua(uint32 connindex, const char *data_ptr, size_t data_len);
	void load_red_package_by_sql_lua(uint32 connindex);
	void load_red_package_receiver_by_sql_lua(uint32 connindex, int64 role_guid);
	void add_red_package_to_sql_proc_lua(uint32 connindex, int64 role_guid, int32 template_id, int64 sender_guid, int32 create_time, xstring sender_name,
			xstring red_text, int32 sender_vip_level, int32 remain_times, int32 remain_money_num);
	void add_red_package_receiver_to_sql_proc_lua(uint32 connindex, int64 role_guid, int64 receiver_guid, xstring role_name, int32 money_num);
	void del_red_package_by_sql_proc_lua(uint32 connindex, int64 red_package_guid);
}

#endif