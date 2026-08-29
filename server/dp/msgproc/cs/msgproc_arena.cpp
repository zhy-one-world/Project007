/********************************************************************
created:	2016年8月25日
file base : msgproc_arena
file ext : hpp
author : zhangminghai

purpose :
*********************************************************************/
#include "logic/type_def.hpp"
#include "../../dbproxy_service.hpp"
#include "msgproc_arena.hpp"
#include <char_msg.hpp>
#include <arena_msg.hpp>

namespace faith
{
	//竞技场
	
	void cs2dp_req_add_arena_log(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const cs2dp_add_arena_log* pdata = static_cast<const cs2dp_add_arena_log*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_add_arena_log))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		int32 result = pdata->log_data.result;
		sql << _XTEXT("call sp_rank_arena_challenge_log_save(") 
			<< pdata->log_data.role_guid.server_64 << _XTEXT(",\'") 
			<< pdata->log_data.role_name<< _XTEXT("\',")
			<< pdata->log_data.role_rank<<_XTEXT(",")
			<< pdata->log_data.target_guid.server_64 << _XTEXT(",\'") 
			<< pdata->log_data.target_name <<_XTEXT("\',")
			<< pdata->log_data.target_rank << _XTEXT(",")
			<< result << _XTEXT(",")
			<< pdata->log_data.date
			<< _XTEXT(");"); 

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = pdata->log_data.role_guid;
		query.handler = boost::bind(&cs2dp_req_add_arena_log_end, _1);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	
	void cs2dp_req_add_arena_log_end(db_result_type result)
	{

	}

	void cs2dp_req_load_arena_log(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		
		const cs2dp_load_arena_log* pdata = static_cast<const cs2dp_load_arena_log*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}

		if (data_len != sizeof(cs2dp_load_arena_log))
		{
			return;
		}
		guid_64 role_guid = pdata->role_guid;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_rank_arena_challenge_log_load(")
			<< role_guid.server_64
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_log_end, _1, connindex, role_guid, pdata->array_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_arena_log_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_index)
	{

		static s_chanllenge_log_info sql_data[challenge_log_max];
		g_clear_data(sql_data, challenge_log_max);

		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_chanllenge_log_info);
		if (result.error || table_len > challenge_log_max || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_arena_data_error error;
			error.role_guid = role_guid;
			error.error_code = e_msgindex_dp2cs_resp_load_arena_log;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size * table_len);

			static dp2cs_resp_load_arena_log reply_challenge_log;

			reply_challenge_log.clear_data();
			reply_challenge_log.data_num = table_len;
			reply_challenge_log.role_guid = role_guid;
			reply_challenge_log.array_index = unit_index;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_challenge_log.role_challenge_log[i]), &(sql_data[i]), sizeof(s_chanllenge_log_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_challenge_log, sizeof(dp2cs_resp_load_arena_log));
			return;
		}
	}

	void cs2dp_req_load_arena_char_role_info(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_info_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_role_info_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_arena_char_role_info_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		//s_unit_info sql_data;
		int32	table_len = result.query.data_select.row_count;
		//int32 data_size = sizeof(s_unit_info);
		if (result.error || table_len > 1 || sizeof(s_unit_info) != result.query.data_select.row_size)
		{
			dp2cs_load_arena_char_error error;
			error.map_guid = map_guid;
			error.error_code = e_data_flag_info;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			dp2cs_arena_load_char_info ret_msg;
			ret_msg.map_guid = map_guid;
			db_read_data(&result, &(ret_msg.role_info), sizeof(ret_msg.role_info));
			dbproxy_service::getInstance().send_message(connindex, &ret_msg, sizeof(ret_msg));
		}
	}

	void cs2dp_req_load_arena_char_att(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_att_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_att_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_arena_char_att_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		s_att_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_att_info_db);
		if (result.error || table_len > 1 || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_arena_char_error error;
			//error.role_guid = role_guid;
			error.map_guid = map_guid;
			error.error_code = e_data_flag_att;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			db_read_data(&result, &sql_data, data_size);

			dp2cs_arena_load_char_att reply_att;
			//reply_att.role_guid = role_guid;
			reply_att.map_guid = map_guid;
			reply_att.data_info = sql_data.data_info;
			dbproxy_service::getInstance().send_message(connindex, &reply_att, sizeof(dp2cs_arena_load_char_att));
			return;
		}
	}

	void cs2dp_req_load_arena_char_skill(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_arena_skill_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_skill_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_arena_char_skill_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		static s_skill_info_db sql_data[MAX_SKILL_NUM];
		g_clear_data(sql_data, MAX_SKILL_NUM);

		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_skill_info_db);
		if (result.error || table_len > MAX_SKILL_NUM || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_arena_char_error error;
			//error.role_guid = role_guid;
			error.map_guid = map_guid;
			error.error_code = e_data_flag_skill;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			static dp2cs_arena_load_char_skill reply_data;
			reply_data.clear_data();
			//reply_data.role_guid = role_guid;
			reply_data.map_guid = map_guid;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.skill_data[i].data_ary), &(sql_data[i].data_ary), sizeof(sql_data[i].data_ary));
			}
			reply_data.data_num = table_len;
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void cs2dp_req_load_arena_char_item(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_item_load(") << role_guid.server_64 <<","<<e_bag_type_equip<<_XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_item_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void cs2dp_req_load_arena_char_item_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		static s_item_info_db sql_data[fake_item_num];
		g_clear_data(sql_data, fake_item_num);

		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);
		if (result.error || table_len > fake_item_num || data_size != result.query.data_select.row_size)
		{
			dp2cs_load_arena_char_error error;
			error.map_guid = map_guid;
			error.error_code = e_data_flag_item;
			error.error_result = result.error;
			dbproxy_service::getInstance().send_message(connindex, &error, sizeof(error));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			static dp2cs_arena_load_char_item reply_item;
			reply_item.clear_data();
			reply_item.map_guid = map_guid;
			reply_item.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_item.item_data[i]), &(sql_data[i].data_info), sizeof(s_item_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_item, reply_item.get_pak_length());
			return;
		}
	}



	void cs2dp_req_load_arena_char_spirit(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_fighting_spirit_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_spirit_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_load_arena_char_spirit_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		s_spirit_info_db sql_data;
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_spirit_info_db);
		dp2cs_arena_load_char_spirit reply_spirit;
		reply_spirit.map_guid = map_guid;
		if (table_len == 0)
		{//可以没有精灵
			reply_spirit.is_has_fighting_spirit = false;
			dbproxy_service::getInstance().send_message(connindex, &reply_spirit, sizeof(dp2cs_arena_load_char_spirit));
		}
		else if(result.error || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else if(table_len == 1)
		{
			//只能出战一个精灵
			db_read_data(&result, &sql_data, data_size);
			reply_spirit.map_guid = map_guid;
			reply_spirit.spirit_data = sql_data.data_info;
			reply_spirit.is_has_fighting_spirit = true;
			dbproxy_service::getInstance().send_message(connindex, &reply_spirit, sizeof(dp2cs_arena_load_char_spirit));
			return;
		}
	}

	void cs2dp_req_load_arena_char_fight_att(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_fight_att_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_fight_att_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_load_arena_char_fight_att_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		dp2cs_arena_load_char_fight_att reply_att;
		reply_att.map_guid = map_guid;

		int32 table_len = result.query.data_select.row_count;
		int32 table_size = result.query.data_select.row_size;
		//此值可能为0，比如旧号没存过战斗属性，在后续逻辑处理，正式上线不会有这种情况
		int32 data_size = sizeof(s_arena_char_fight_att);
		if (table_len == 0 || result.error || table_size != data_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_att, sizeof(dp2cs_arena_load_char_fight_att));
		}
		else
		{
			db_read_data(&result, &reply_att.m_fight_att, data_size);
			dbproxy_service::getInstance().send_message(connindex, &reply_att, sizeof(dp2cs_arena_load_char_fight_att));
			return;
		}
	}

	void cs2dp_req_load_arena_char_buff(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_buff_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_buff_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_load_arena_char_buff_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		static s_buff_save_db sql_data[MAX_BUFF_DESC];
		g_clear_data(sql_data, MAX_BUFF_DESC);

		int32	iNumber = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < iNumber * result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, iNumber * result.query.data_select.row_size);
			static dp2cs_arena_load_char_buff reply_data;
			reply_data.clear_data();

			reply_data.map_guid = map_guid;
			reply_data.data_num = iNumber;
			for (int32 i = 0; i < iNumber; ++i)
			{
				memcpy(&(reply_data.buff_data[i]), &(sql_data[i].data_info), sizeof(s_buff_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}
	void cs2dp_req_load_arena_char_belief(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ")<<data_manager::get_instance().get_db_name(role_guid.server_64)<<_XTEXT(".sp_role_belief_load(") << role_guid.server_64 << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.role_guid = role_guid;
		query.handler = boost::bind(&cs2dp_req_load_arena_char_belief_end, _1, connindex, map_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void cs2dp_req_load_arena_char_belief_end(db_result_type result, uint32 connindex, const guid_64& map_guid)
	{
		static s_belief_info_db sql_data[e_belief_series_type_max];
		g_clear_data(sql_data, e_belief_series_type_max);

		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(sql_data);
		if (result.error || data_size < table_len * result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, data_size);
			db_read_data(&result, &sql_data, table_len * result.query.data_select.row_size);
			static dp2cs_arena_load_char_belief reply_data;
			reply_data.clear_data();

			reply_data.map_guid = map_guid;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_data.belief_data[i] = sql_data[i].data_info;
				reply_data.data_num++;
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
		}
	}
}