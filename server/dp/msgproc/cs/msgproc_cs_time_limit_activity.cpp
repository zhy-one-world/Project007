/********************************************************************
created:	2018-7-28 15:15:40
file base:	msgproc_cs_time_limit_activity
file ext:	cpp
author:		shangxiaoliang

purpose:
*********************************************************************/
#include "../../dbproxy_service.hpp"
#include "data_manager/data_manager.hpp"
#include "msgproc_cs_time_limit_activity.hpp"
#include "msgproc_cs.hpp"
#include <time_limit_activity_msg.hpp>
#include <char_msg.hpp>
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"
#include "error.pb.h"


namespace faith
{
	static void cs2dp_load_role_time_limit_activity_send_lua(uint32 connindex, const dp2cs_load_time_limit_activity_info_end & msgData)
	{
		faith::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);
		faith::cs2dp_proto::role_time_limit_activity_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			faith::cs2dp_proto::role_time_limit_activity_row* db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			for (int32 j = 0; j < e_time_limit_activity_info_max; j++)
			{
				db_row->add_data_ary(msgData.data_list[i].data_info.data_ary[j]);
			}
			for (int32 j = 0; j < time_limit_activity_schedule_num; j++)
			{
				db_row->add_data_ary(msgData.data_list[i].data_info.activity_schedule[j]);
			}
			db_row->set_schedule_str(msgData.data_list[i].data_info.schedule_str);
			db_row->set_reward_flag(msgData.data_list[i].data_info.reward_flag);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}
		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(connindex, &msg, e_msgindex_dp2cs_load_time_limit_activity_info_end);

	}
	

	void cs2dp_req_load_char_time_limit_activity_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		dp2cs_load_time_limit_activity_info_end msgData;
		msgData.role_guid = role_guid;
		msgData.unit_array_index = unit_array_index;
		if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_time_limit_activity, &msgData.data_list, msgData.data_num))
		{
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_time_limit_activity_info_end);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
			}
			else
			{
				cs2dp_load_role_time_limit_activity_send_lua(connindex, msgData);
			}
		}
		else
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_time_limit_activity_load(") << role_guid.server_64 << _XTEXT(");");
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&cs2dp_req_load_char_time_limit_activity_info_end, _1, connindex, role_guid, unit_array_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void cs2dp_req_load_char_time_limit_activity_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{
		s_time_limit_activity_info_db sql_data[max_save_num_with_one_player];
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_time_limit_activity_info_db);
		if (result.error || table_len > max_save_num_with_one_player || data_size != result.query.data_select.row_size)
		{
			CONSOLE_ERROR("error role_guid:{} unit_array_index:{} result.error:{} table_len:{} data_size:{} row_size:{}", role_guid.server_64, unit_array_index, result.error, table_len, data_size, result.query.data_select.row_size);
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_time_limit_activity;
			error.error_result = e_error_code_db_error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, sql_data, data_size * table_len);

			dp2cs_load_time_limit_activity_info_end reply;
			reply.role_guid = role_guid;
			reply.unit_array_index = unit_array_index;
			reply.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply.data_list[i]), &(sql_data[i]), sizeof(s_time_limit_activity_info_db));
			}

			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_time_limit_activity_info_end);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply, reply.get_pak_length());
			}
			else
			{
				cs2dp_load_role_time_limit_activity_send_lua(connindex, reply);
			}

			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			if (data_set_ptr)
			{
				data_set_ptr->add_data_info(e_data_flag_time_limit_activity, reply.data_list, table_len * sizeof(s_time_limit_activity_info_db), table_len);
			}
			return;
		}
	}

	bool cs2dp_save_role_time_limit_activity_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		faith::cs2dp_proto::role_time_limit_activity_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}
		int32 row_count = msg.row_count();
		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_time_limit_activity_info_db));
		if (p_data == nullptr)
		{
			return false;
		}

		s_time_limit_activity_info_db *p_row = (s_time_limit_activity_info_db *)p_data;

		for (int32 i = 0; i < row_count; i++)
		{
			faith::cs2dp_proto::role_time_limit_activity_row one_row = msg.row_data(i);
			for (int32 j = 0; j < one_row.data_ary_size(); j++)
			{
				p_row->data_info.data_ary[j] = one_row.data_ary(j);
			}
			for (int32 j = 0; j < one_row.activity_schedule_size(); j++)
			{
				p_row->data_info.activity_schedule[j] = one_row.activity_schedule(j);
			}
			p_row->data_info.set_schedule_str(one_row.schedule_str());
			p_row->data_info.reward_flag = one_row.reward_flag();
			p_row->role_guid = role_guid;
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_time_limit_activity, p_data, row_count * sizeof(s_time_limit_activity_info_db), row_count);
		return true;
	}


	void cs2dp_req_save_char_time_limit_activity_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_time_limit_activity_info* pdata = static_cast<const cs2dp_save_time_limit_activity_info*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_time_limit_activity, pdata->activity_data_array, pdata->data_num * sizeof(s_time_limit_activity_info_db), pdata->data_num);

			if (pdata->save_type_ex != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_time_limit_activity, pdata->save_type_ex);
			}
		}
		int32 num = 0;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(pdata->role_guid.server_64)<<_XTEXT(".sp_role_time_limit_activity_save(")
			<< pdata->role_guid.server_64 << ",'";

		for (int32 i = 0; i < pdata->data_num; i++)
		{
			if (pdata->activity_data_array[i].data_info.data_ary[e_time_limit_activity_info_guid] > 0)
			{
				if (num != 0)
				{
					sql << ",";
				}
				num++;
				sql << "(" << pdata->role_guid.server_64 << ",";
				for (int32 j = 0; j < e_time_limit_activity_info_max; j++)
				{
					sql << pdata->activity_data_array[i].data_info.data_ary[j] << ",";
				}
				for (int32 j = 0; j < time_limit_activity_schedule_num; j++)
				{
					sql << pdata->activity_data_array[i].data_info.activity_schedule[j] << ",";
				}
				sql << "\\\'" << pdata->activity_data_array[i].data_info.schedule_str << "\\\',";
				sql << pdata->activity_data_array[i].data_info.reward_flag << ")"; 
			}
		}

		sql	<< _XTEXT("');");

		db_query_type query;
		query.logic_id = e_data_flag_time_limit_activity;
		query.role_guid = pdata->role_guid;
		query.sql_str.assign(sql.c_str(), sql.size());
		if (pdata->save_type_ex == e_save_data_type_exit_game)
		{
			query.handler = boost::bind(&cs2dp_req_save_char_info_cs_end, _1, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_time_limit_activity, pdata->save_type_ex, sql.c_str());
		}
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_time_limit_temp_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_load_time_limit_temp* pdata = static_cast<const ws2dp_load_time_limit_temp*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		int32 cur_data_num = 0;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_time_limit_temp_load_info(")
			<< cur_data_num << ","
			<< max_load_time_limit_temp_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_time_limit_temp_info_end, _1, connindex, cur_data_num + max_load_time_limit_temp_num);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_time_limit_temp_info_end(db_result_type result, uint32 connindex, int32 cur_num)
	{
		if (result.error)
		{
			return;
		}
		s_time_limit_activity_temp_db sql_data[max_load_time_limit_temp_num];
		memset(sql_data, 0, sizeof(sql_data));
		int32 data_num = result.query.data_select.row_count;
		dp2ws_load_time_limit_temp_end msg;
		msg.data_num = data_num;
		if (sizeof(s_time_limit_activity_temp_db) == result.query.data_select.row_size)
		{
			if (data_num > 0)
			{
				int32 data_size = result.query.data_select.row_size * data_num;
				db_read_data(&result, &sql_data, data_size);
				for (int32 i = 0; i < data_num; i++)
				{
					memcpy(&(msg.temp_info[i]), &(sql_data[i]), sizeof(s_time_limit_activity_temp_db));
				}
			}
			if (data_num < max_load_time_limit_temp_num)
			{
				msg.is_load_all = true;
				ws2dp_req_load_time_limit_branch_temp_info(connindex);
			}
			dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());
			if (data_num >= max_load_time_limit_temp_num)
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("CALL sp_time_limit_temp_load_info(")
					<< cur_num << ","
					<< max_load_time_limit_temp_num
					<< _XTEXT(");");

				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				query.handler = boost::bind(&ws2dp_req_load_time_limit_temp_info_end, _1, connindex, cur_num + max_load_time_limit_temp_num);
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
		}
	}

	void ws2dp_req_load_time_limit_branch_temp_info(uint32 connindex)
	{
		int32 cur_data_num = 0;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_time_limit_branch_temp_load_info(")
			<< cur_data_num << ","
			<< max_load_time_limit_temp_num
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_load_time_limit_branch_temp_info_end, _1, connindex, cur_data_num + max_load_time_limit_temp_num);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_load_time_limit_branch_temp_info_end(db_result_type result, uint32 connindex, int32 cur_num)
	{
		if (result.error)
		{
			return;
		}
		s_time_limit_activity_branch_temp_db sql_data[max_load_time_limit_temp_num];
		memset(sql_data, 0, sizeof(sql_data));
		int32 data_num = result.query.data_select.row_count;
		dp2ws_load_time_limit_branch_temp_end msg;
		msg.data_num = data_num;
		if (sizeof(s_time_limit_activity_branch_temp_db) == result.query.data_select.row_size)
		{
			if (data_num > 0)
			{
				int32 data_size = result.query.data_select.row_size * data_num;
				db_read_data(&result, &sql_data, data_size);
				for (int32 i = 0; i < data_num; i++)
				{
					memcpy(&(msg.temp_info[i]), &(sql_data[i]), sizeof(s_time_limit_activity_branch_temp_db));
				}
			}
			if (data_num < max_load_time_limit_temp_num)
			{
				msg.is_load_all = true;
			
			}
			dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());

			if (data_num >= max_load_time_limit_temp_num)
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("CALL sp_time_limit_branch_temp_load_info(")
					<< cur_num << ","
					<< max_load_time_limit_temp_num
					<< _XTEXT(");");

				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				query.handler = boost::bind(&ws2dp_req_load_time_limit_branch_temp_info_end, _1, connindex, cur_num + max_load_time_limit_temp_num);
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
		}
	}

	void ws2dp_req_save_time_limit_temp_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_save_time_limit_temp* pdata = static_cast<const ws2dp_save_time_limit_temp*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_save_time_limit_temp))
		{
			return;
		}
		if (pdata->temp_db_info.ActGuid <= 0)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_time_limit_temp_save(";
		sql << pdata->temp_db_info.ActGuid << ",";
		sql << "'(";
		sql << pdata->temp_db_info.ActGuid << ",";
		sql << pdata->temp_db_info.AttributeId << ",";
		sql << pdata->temp_db_info.ActivityType << ",";
		sql << pdata->temp_db_info.CycleParamType << ",";
		sql << "\\\'" << pdata->temp_db_info.CycleParamNum << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.CycleFirstId << "\\\',";
		sql << pdata->temp_db_info.OpenType << ",";
		sql << pdata->temp_db_info.OpenServerDate << ",";
		sql << "\\\'" << pdata->temp_db_info.OpenSpencificDate << "\\\',";
		sql << pdata->temp_db_info.FirstOpenDayOfWeek << ",";
		sql << pdata->temp_db_info.DurationDays << ",";
		sql << pdata->temp_db_info.CycleDays << ",";
		sql << pdata->temp_db_info.ShowType << ",";
		sql << "\\\'" << pdata->temp_db_info.TextImage << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr1 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr2 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr3 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr4 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr5 << "\\\',";
		sql << pdata->temp_db_info.IsEndAct << ",";
		sql << "\\\'" << pdata->temp_db_info.OriginalCycleFirstId << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.OldCycleFirstId << "\\\',";
		sql << pdata->temp_db_info.CreateTime << ",";
		sql << pdata->temp_db_info.EndTime << ",";
		sql << pdata->temp_db_info.TemplateFromType << ",";
		sql << pdata->temp_db_info.RefreshTime << ")'";
		sql << ");";

		db_query_type query;
		query.logic_id = e_data_flag_type_time_limit_activity_template;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_save_time_limit_temp_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_save_time_limit_temp_info_end(db_result_type result, uint32 connindex)
	{

	}

	void ws2dp_req_save_time_limit_branch_temp_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_save_time_limit_branch_temp* pdata = static_cast<const ws2dp_save_time_limit_branch_temp*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_save_time_limit_branch_temp))
		{
			return;
		}
		if (pdata->temp_db_info.ActGuid <= 0)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_time_limit_branch_temp_save(";
		sql << pdata->temp_db_info.ActGuid << ",";
		sql << "'(";
		sql << pdata->temp_db_info.ActGuid << ",";
		sql << pdata->temp_db_info.AttributeId << ",";
		sql << pdata->temp_db_info.ActivityType << ",";
		sql << pdata->temp_db_info.CycleIndex << ",";
		sql << "\\\'" << pdata->temp_db_info.Condition << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.Reward << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.TextImage << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr1 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr2 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr3 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr4 << "\\\',";
		sql << "\\\'" << pdata->temp_db_info.ParamArr5 << "\\\')'";
		sql << ");";

		db_query_type query;
		query.logic_id = e_data_flag_type_time_limit_activity_template;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_save_time_limit_branch_temp_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_save_time_limit_branch_temp_info_end(db_result_type result, uint32 connindex)
	{

	}

	void ws2dp_req_update_role_time_limit_info_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_update_role_time_limit* pdata = static_cast<const ws2dp_update_role_time_limit*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_update_role_time_limit))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_update_role_time_limit_info(")
			<< pdata->act_guid
			<< _XTEXT(",")
			<< pdata->act_type
			<< _XTEXT(");");

		db_query_type query;
		query.logic_id = e_data_flag_type_time_limit_activity_template;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_update_role_time_limit_info_func_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	
	void ws2dp_req_update_role_time_limit_info_func_end(db_result_type result, uint32 connindex)
	{
		
	}

	void ws2dp_req_update_delete_time_limit_temp(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_delete_time_limit_temp* pdata = static_cast<const ws2dp_delete_time_limit_temp*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_delete_time_limit_temp))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_time_limit_temp_delete(")
			<< pdata->act_guid
			<< _XTEXT(");");

		db_query_type query;
		query.logic_id = e_data_flag_type_time_limit_activity_template;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_update_delete_time_limit_temp_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
		
	}

	void ws2dp_req_update_delete_time_limit_temp_end(db_result_type result, uint32 connindex)
	{

	}

	void ws2dp_req_update_delete_time_limit_branch_temp(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_delete_time_limit_branch_temp* pdata = static_cast<const ws2dp_delete_time_limit_branch_temp*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_delete_time_limit_branch_temp))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_time_limit_branch_temp_delete(")
			<< pdata->act_guid
			<< _XTEXT(");");

		db_query_type query;
		query.logic_id = e_data_flag_type_time_limit_activity_template;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_update_delete_time_limit_branch_temp_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_update_delete_time_limit_branch_temp_end(db_result_type result, uint32 connindex)
	{

	}

	void ws2dp_req_refresh_role_time_limit_guid(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ws2dp_refresh_role_time_limit_guid* pdata = static_cast<const ws2dp_refresh_role_time_limit_guid*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_refresh_role_time_limit_guid))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_refresh_role_time_limit_guid(")
			<< pdata->old_act_guid << _XTEXT(",")
			<< pdata->new_act_guid
			<< _XTEXT(");");

		db_query_type query;
		query.logic_id = e_data_flag_type_time_limit_activity_template;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&ws2dp_req_refresh_role_time_limit_guid_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ws2dp_req_refresh_role_time_limit_guid_end(db_result_type result, uint32 connindex)
	{

	}
}
