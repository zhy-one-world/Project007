/********************************************************************
created: 2018Äê9ÔÂ6ÈÕ
file base: msgproc_boss_island
file ext: cpp
author: zhangshuo

purpose: ÓÀºãµº
*********************************************************************/

#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_oracle_trial.hpp"
#include "msgproc_cs.hpp"
#include "data_manager/data_manager.hpp"
#include <oracle_trial_msg.hpp>
#include "Logic/oracle_trial_def.hpp"
#include <char_msg.hpp>
#include <template/template_manager.h>
#include "Logic/activity_def.hpp"
#include "dp2cs.pb.h"
#include "cs2dp.pb.h"


namespace hld
{
	static void cs2dp_load_oracle_trial_send_lua(uint32 conindex, const dp2cs_load_oracle_trial_all_info &msgData)
	{
		hld::dp2cs_proto::load_role_db_data msg;
		msg.set_role_guid(msgData.role_guid.server_64);
		msg.set_unit_array_index(msgData.unit_array_index);

		hld::cs2dp_proto::role_oracle_trial_db msg_db;
		msg_db.set_row_count(msgData.data_num);
		for (int32 i = 0; i < msg_db.row_count(); i++)
		{
			hld::cs2dp_proto::role_oracle_trial_row * db_row = msg_db.add_row_data();
			if (db_row == nullptr)
			{
				return;
			}
			db_row->set_role_guid(msgData.data_list[i].role_guid.server_64);
			db_row->set_tier_num(msgData.data_list[i].tier_num);
			db_row->set_customs_state(msgData.data_list[i].customs_state);
			db_row->set_reward_stat(msgData.data_list[i].reward_stat);
		}

		packet_s2s *p_db = parse_msg::getInstance().serialze_buffer(&msg_db);
		if (p_db == nullptr)
		{
			return;
		}

		msg.set_db_data(p_db->google_data, p_db->google_data_len);

		dbproxy_service::getInstance().send_message_lua(conindex, &msg, e_msgindex_dp2cs_load_oracle_trial_all_info);
	}
	void cs2dp_req_load_oracle_trial_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index)
	{
			data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
			dp2cs_load_oracle_trial_all_info msgData;
			int32 data_num = 1;
			if (data_set_ptr && data_set_ptr->get_data_info(e_data_flag_oracle_trial, &msgData.data_list, msgData.data_num))
			{
				msgData.role_guid = role_guid;
				msgData.unit_array_index = unit_array_index;
				bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_oracle_trial_all_info);
				if (is_use == false)
				{
					dbproxy_service::getInstance().send_message(connindex, &msgData, sizeof(msgData));
				}
				else
				{
					cs2dp_load_oracle_trial_send_lua(connindex, msgData);
				}
			}
			else
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(up_role_guid.server_64)<<_XTEXT(".sp_role_oracle_trial_info_load(") << role_guid.server_64 << _XTEXT(");");

				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				query.role_guid = role_guid;
				query.logic_id = e_data_flag_oracle_trial;
				query.handler = boost::bind(&cs2dp_req_load_oracle_trial_info_end, _1, connindex, role_guid, unit_array_index);
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
	}

	void cs2dp_req_load_oracle_trial_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index)
	{	
		s_oracle_trial_info_db sql_data[max_tier_num];
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size || table_len > max_tier_num)
		{
			dp2cs_load_data_error error;
			error.role_guid = role_guid;
			error.array_index = unit_array_index;
			error.error_code = e_data_flag_oracle_trial;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			dp2cs_load_oracle_trial_all_info reply_data;
			reply_data.role_guid = role_guid;
			reply_data.unit_array_index = unit_array_index;
			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.data_list[i]), &(sql_data[i]), sizeof(s_oracle_trial_info_db));
			}
			bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_dp2cs_load_oracle_trial_all_info);
			if (is_use == false)
			{
				dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(dp2cs_load_oracle_trial_all_info));
			}
			else
			{
				cs2dp_load_oracle_trial_send_lua(connindex, reply_data);
			}
			if (table_len > 0)
			{
				data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
				if (data_set_ptr)
				{
					data_set_ptr->add_data_info(e_data_flag_oracle_trial, &reply_data.data_list, sizeof(s_oracle_trial_info_db) * reply_data.data_num, table_len);
				}
			}
		}
		return;
	}
	bool cs2dp_save_role_oracle_trial_cache_lua(int64 role_id, const char* data_ptr, size_t data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		guid_64 role_guid;
		role_guid.server_64 = role_id;

		hld::cs2dp_proto::role_oracle_trial_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_oracle_trial_info_db));
		if (p_data == nullptr)
		{
			return false;
		}

		s_oracle_trial_info_db *p_row = (s_oracle_trial_info_db *)p_data;

		for (int32 i = 0; i < row_count; i++)
		{
			hld::cs2dp_proto::role_oracle_trial_row one_row = msg.row_data(i);
			p_row->role_guid.server_64 = one_row.role_guid();
			p_row->tier_num = one_row.tier_num();
			p_row->customs_state = one_row.customs_state();
			p_row->reward_stat = one_row.reward_stat();
			p_row++;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(role_guid);
		if (data_set_ptr == nullptr)
		{
			return false;
		}
		data_set_ptr->add_data_info(e_data_flag_oracle_trial, p_data, row_count * sizeof(s_oracle_trial_info_db), row_count);
		return true;
	}

	void cs2dp_req_save_oracle_trial_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_save_oracle_trial_info* pdata = static_cast<const cs2dp_save_oracle_trial_info*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		data_set* data_set_ptr = data_manager::get_instance().get_data_set(pdata->role_guid);
		if (data_set_ptr)
		{
			data_set_ptr->add_data_info(e_data_flag_oracle_trial, &pdata->info_data, sizeof(s_oracle_trial_info_db) * pdata->data_num, pdata->data_num);
			if (pdata->save_type != e_save_data_type_exit_game)
			{
				cs2dp_req_send_info_end(true, connindex, pdata->role_guid, pdata->unit_array_index, e_data_flag_oracle_trial, pdata->save_type);
			}
		}
		for (int32 i = 0; i < pdata->data_num; ++i)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(pdata->role_guid.server_64) << _XTEXT(".sp_oracle_trial_player_info_save(")  << pdata->role_guid.server_64 << ",";
			sql << pdata->info_data[i].tier_num << ",";
			sql << "'(";
			sql << pdata->info_data[i].role_guid.server_64 << ",";
			sql << pdata->info_data[i].tier_num << ",";
			sql << pdata->info_data[i].customs_state << ",";
			sql << pdata->info_data[i].reward_stat << ")'";
			sql << ");";
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
		
	}
}