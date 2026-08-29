#include "msgproc_recycle.h"
#include "data_manager/data_manager.hpp"
#include <recycle_msg.h>
#include "../../dbproxy_service.hpp"
#include <char_msg.hpp>
#include "msgproc_cs.hpp"
#include "cs2dp.pb.h"
#include "dp2cs.pb.h"

namespace faith
{
	static void cs2dp_load_role_recycle_send_lua(uint32 connindex, const dp2cs_load_char_recycle& msgData)
	{
		faith::dp2cs_proto::load_role_recycle msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_recycle_db msg_db;
		msg_db.set_e_start_time_1(msgData.data_info.e_start_time_1);
		msg_db.set_e_login_state_2(msgData.data_info.e_login_state_2);
		msg_db.set_e_prop_state_3(msgData.data_info.e_prop_state_3);
		msg_db.set_inviter_guid(msgData.data_info.inviter_guid.server_64);
		msg_db.set_invited_state(msgData.data_info.invited_state);

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data((const char *)p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex,&msg, e_msgindex_dp2cs_load_char_recycle);
	}
	void cs2dp_req_load_char_recycle(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_load_char_recycle msgData;
		int32 data_num = 1;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_recycle, &msgData.data_info, data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_recycle);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_recycle_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_recycle_load('") << up_role_guid.server_64 << _XTEXT("');");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_recycle;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_recycle_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}


	}
	void cs2dp_req_load_char_recycle_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_recycle_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_recycle_info_db);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_recycle;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);
			dp2cs_load_char_recycle reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.data_info = sql_data.data_info;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_recycle);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_char_recycle));
			}
			else
			{
				cs2dp_load_role_recycle_send_lua(connindex, reply_data);
			}
			//dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_char_recycle));
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_recycle, &reply_data.data_info, sizeof(s_recycle_info), 1);
			}
		}

	}
	bool cs2dp_save_role_recycle_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		s_recycle_info db_row_info;

		faith::cs2dp_proto_role_recycle_db msg;

		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}
		db_row_info.e_start_time_1 = msg.e_start_time_1();
		db_row_info.e_login_state_2 = msg.e_login_state_2();
		db_row_info.e_prop_state_3 = msg.e_prop_state_3();
		db_row_info.inviter_guid = msg.inviter_guid();
		db_row_info.invited_state = msg.invited_state();

		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}

		data_set_ptr->add_data_info(e_data_flag_recycle, &db_row_info, sizeof(s_recycle_info), 1);

		return true;


	}
	void cs2dp_req_save_char_recycle(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_recycle* pdata = static_cast<const cs2dp_save_char_recycle*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_recycle, &pdata->data_info, sizeof(pdata->data_info), 1);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_recycle, pdata->save_type_ex);

			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_recycle_save('") << pdata->role_guid.server_64 << "','";

		sql <<  "(" << pdata->role_guid.server_64 << ",";

		sql << pdata->data_info.e_start_time_1 << ",";
		sql << pdata->data_info.e_login_state_2 << ",";
		sql << pdata->data_info.e_prop_state_3 << ",";
		sql << pdata->data_info.inviter_guid.server_64 << ",";
		sql << pdata->data_info.invited_state;


		sql << ")');";

		db_query_type query;
		query.logic_id = e_data_flag_recycle;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_recycle, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	////////////////////////////////////////////////////////////////////////////////////
	void cs2dp_load_role_recycle_task_send_lua(uint32 connindex, const dp2cs_load_char_recycle_task & msgData)
	{
		faith::dp2cs_proto::load_role_recycle_task msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		faith::cs2dp_proto::role_recycle_task_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::cs2dp_proto::role_recycle_task_row *db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			for (int32 j = 0; j < e_recycle_tk_max; j++)
			{
				db_row->add_data_ary(msgData.data_list[i].data_ary[j]);
			}
		}


		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}

		msg.set_db_data((const char *)p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_recycle_task);




	}

	void cs2dp_req_load_char_recycle_task(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		dp2cs_load_char_recycle_task msgData;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_recycle_task, msgData.data_list, msgData.data_num))
		{
			msgData.role_guid = role_guid;
			msgData.unit_array_index = unit_array_index;

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_recycle_task);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_recycle_task_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_recycle_task_load('") << up_role_guid.server_64 << _XTEXT("');");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_recycle_task;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_recycle_task_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}

	}

	void cs2dp_req_load_char_recycle_task_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_recycle_task_info_db sql_data[MAX_RECYCLE_TASK_NUM];
		int32  table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_recycle_task_info_db);
		if (result.error || table_len > MAX_RECYCLE_TASK_NUM || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_recycle_task;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2cs_load_char_recycle_task reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.data_list[i] = sql_data[i].data_info;
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_recycle_task);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				cs2dp_load_role_recycle_task_send_lua(connindex, reply_data);
			}
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_recycle_task, reply_data.data_list, reply_data.data_num * sizeof(s_recycle_task_info), reply_data.data_num);
			}
		}

	}

	bool cs2dp_save_role_recycle_task_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		faith::cs2dp_proto::role_recycle_task_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_recycle_task_info));
		if (p_data == nullptr)
		{
			return false;
		}

		s_recycle_task_info *p_row = (s_recycle_task_info *)p_data;

		for (int32 i = 0 ; i < row_count; i++)
		{
			faith::cs2dp_proto::role_recycle_task_row one_row = msg.row_data(i);
			for (int32 j = 0; j < one_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = one_row.data_ary(j);
			}
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_recycle_task, p_data, row_count * sizeof(s_recycle_task_info), row_count);

		return true;


	}
	void cs2dp_req_save_char_recycle_task(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_recycle_task* pdata = static_cast<const cs2dp_save_char_recycle_task*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != (const_cast<cs2dp_save_char_recycle_task*>(pdata))->get_pak_length())
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_recycle_task, pdata->data_list, pdata->data_num * sizeof(s_recycle_task_info), pdata->data_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_recycle_task, pdata->save_type_ex);

			}
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_recycle_task_save('") << pdata->role_guid.server_64 << "','";
		int32 tempLen = pdata->data_num - 1;
		int32 sqlEnd = e_recycle_tk_max - 1;
		for (int32 i = 0; i < pdata->data_num; i++)
		{
			sql << "(" << pdata->role_guid.server_64 << ",";
			for (int32 j = 0; j < e_recycle_tk_max; ++j)
			{
				if (j != sqlEnd)
				{
					sql << pdata->data_list[i].data_ary[j] << ",";
				}
				else
				{
					sql << pdata->data_list[i].data_ary[j];
				}
			}
			if (i != tempLen)
			{
				sql << "),";
			}
			else
			{
				sql << ")";
			}
		}
		sql << "');";
		db_query_type query;
		query.logic_id = e_data_flag_recycle_task;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_recycle_task, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_char_recycle_invited(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		//data_set* data_set_ptr = data_manager::get_instance().get_data_set(up_role_guid);
		//dp2cs_load_char_recycle_invited msgData;
		//if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_recycle_invited, msgData.data_list, msgData.data_num))
		//{
		//	msgData.role_guid = role_guid;
		//	msgData.unit_array_index = unit_array_index;
		//	dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		//}
		//else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_recycle_invited_load('") << up_role_guid.server_64 << _XTEXT("');");

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.logic_id = e_data_flag_recycle_invited;
			query.role_guid = role_guid;
			query.handler = boost::bind(&cs2dp_req_load_char_recycle_invited_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	static void cs2dp_load_role_recycle_invited_send_lua(uint32 connindex, const dp2cs_load_char_recycle_invited &reply_data)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(reply_data.role_guid.server_64);
		msg.set_unit_array_index(reply_data.unit_array_index);

		faith::db_proto::role_proc_recycle_invited_db msg_db;
		msg_db.set_row_count(reply_data.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::db_proto::role_proc_recycle_invited_row* db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_role_guid(reply_data.data_list[i].role_guid.server_64);
			db_row->set_level(reply_data.data_list[i].level);
			db_row->set_head_id(reply_data.data_list[i].head_id);
			db_row->set_class_type(reply_data.data_list[i].class_type);
			db_row->set_invited_name(reply_data.data_list[i].invited_name);
		}
		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_char_recycle_invited);

	}

	void cs2dp_req_load_char_recycle_invited_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_recycle_invited_info_db sql_data[MAX_RECYCLE_INVITED_NUM];
		int32  table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_recycle_invited_info_db);
		if (result.error || table_len > MAX_RECYCLE_INVITED_NUM || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_recycle_invited;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2cs_load_char_recycle_invited reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.data_list[i] = sql_data[i].data_info;
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_char_recycle_invited);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			}
			else
			{
				cs2dp_load_role_recycle_invited_send_lua(connindex, reply_data);
			}
			//data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			//if (data_set_ptr)
			//{
			//	data_set_ptr->add_data_info(e_data_flag_recycle_invited, reply_data.data_list, reply_data.data_num * sizeof(s_recycle_invited_info), reply_data.data_num);
			//}
		}
	}
	void cs2dp_req_get_inviter_player_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_recycle_get_inviter_info* pdata = static_cast<const cs2dp_recycle_get_inviter_info*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_recycle_get_inviter_info))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->target_guid.server_64)<<_XTEXT(".sp_role_recycle_get_inviter_info(") << pdata->target_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->role_guid;
		query.handler = boost::bind(&cs2dp_req_get_inviter_player_info_end, _1, connindex, pdata->role_guid, pdata->target_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);


		////////////////////////////////////////////////////////////////////////////////////////////////

	}
	void cs2dp_req_get_inviter_player_info_end(db_result_type result,uint32 connindex, const guid_64& role_guid, const guid_64& target_guid)
	{
		dp2cs_recycle_get_inviter_info_end msgData;
		int32 data_size = sizeof(s_recycle_inviter_base_info_db);
		int32 table_len = result.query.data_select.row_count;
		if (result.error || data_size != result.query.data_select.row_size)
		{
			 msgData.role_guid = role_guid;
			 msgData.target_guid = target_guid;
			 msgData.error = 1;
		}
		else {

			msgData.role_guid = role_guid;
			msgData.target_guid = target_guid;
			db_read_data(&result, &(msgData.data_info), sizeof(msgData.data_info));
			//dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
		}
		dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));

	}


	void cs2dp_req_save_inviter_player_add(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_char_recycle_invited* pdata = static_cast<const cs2dp_save_char_recycle_invited*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_save_char_recycle_invited))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->target_guid.server_64)<<_XTEXT(".sp_role_recycle_add_invited(") << pdata->target_guid.server_64 << ",";
		sql << pdata->role_guid.server_64 << ",";
	    sql << "'(" << pdata->target_guid.server_64 << ","<< pdata->role_guid.server_64 << "," << 0 << ")'";
		sql << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->role_guid;
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	

}