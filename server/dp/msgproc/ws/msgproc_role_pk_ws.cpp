/********************************************************************
created:	2016年12月12日12:47:39
file base:	msgproc_role_pk_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_role_pk_ws.hpp"
#include <role_pk_msg.hpp>
#include "dbproxy_service.hpp"

namespace faith
{
	

	void load_role_pk_top(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		load_role_pk_top_lua(connindex);
	}
	void load_role_pk_top_end(db_result_type result, uint32 connindex)
	{
#pragma pack ( push, 1 )
		struct _ranking_info
		{
			int32				ranking_id;
			faith::guid_64		role_guid;
			xchar				role_name[max_name_size + 1];
			int32				template_id;
			int32				role_gs;
			int32				ladder_score;

			_ranking_info()
			{
				memset(this, 0, sizeof(*this));
			}
		};
#pragma pack( pop )

		dp2ws_load_role_pk_top msg;

		if (result.error || result.query.data_select.row_size != sizeof(_ranking_info))
		{
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(msg));
			return;
		}

		_ranking_info ranking_array[PK_RANKING_TOP_NUM];
		memset(ranking_array, 0, sizeof(ranking_array));
		db_read_data(&result, &ranking_array, sizeof(ranking_array));

		for (int32 i = 0; i < PK_RANKING_TOP_NUM; ++i)
		{
			_ranking_info& temp_db_info = ranking_array[i];
			s_role_pk_info& temp_ranking_info = msg.pk_top_array[i];
			temp_ranking_info.role_guid = temp_db_info.role_guid;
			memcpy(temp_ranking_info.role_name, temp_db_info.role_name, sizeof(temp_db_info.role_name));
			temp_ranking_info.data_array[e_role_pk_info_template_id] = temp_db_info.template_id;
			temp_ranking_info.data_array[e_role_pk_info_gs] = temp_db_info.role_gs;
			temp_ranking_info.data_array[e_role_pk_info_ladder_score] = temp_db_info.ladder_score;
			temp_ranking_info.data_array[e_role_pk_info_ranking_num] = temp_db_info.ranking_id;
		}
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(msg));
	}
	void load_role_pk_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_role_pk_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_role_pk_info_lua(connindex, request.role_guid(), request.template_id(), request.role_gs(), request.role_name());
	}
	void load_role_pk_info_end(db_result_type result, uint32 connindex)
	{	
		dp2ws_load_role_pk_info msg;
		if (result.error || result.query.data_select.row_count != 1 || result.query.data_select.row_size != sizeof(s_role_pk_info))
		{
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(msg));
			return;
		}
		db_read_data(&result, &(msg.role_pk_info), sizeof(s_role_pk_info));
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(msg));
	}
	void save_role_pk_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_role_pk_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_role_pk_info_lua(connindex, request.role_guid(), request.target_name_1(), request.target_name_2(), request.target_name_3(), request.ladder_score(), request.win_result(), request.cross_honor());
	}
	void save_role_pk_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error)
		{

		}
	}
	void save_role_pk_honor_refresh(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_role_pk_honor_refresh request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_role_pk_honor_refresh_lua(connindex, request.role_guid(), request.refresh_time());
	
	}
	void save_role_pk_honor_refresh_end(db_result_type result, uint32 connindex)
	{

	}
	void load_role_pk_log(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_role_pk_log request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_role_pk_log_lua(connindex, request.role_guid(), request.server_id());
	}
	void load_role_pk_log_end(db_result_type result, uint32 connindex, guid_64 role_guid, int32 server_id)
	{
		dp2ws_load_role_pk_log msg;
		msg.server_id = server_id;
		msg.role_guid = role_guid;
		if (result.error || result.query.data_select.row_count > CROSS_WAR_INFO_NUM || result.query.data_select.row_size != sizeof(s_cross_war_db))
		{
			dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(msg));
			return;
		}

		msg.data_len = result.query.data_select.row_count;
		db_read_data(&result, &(msg.data_array), sizeof(msg.data_array));
		dbproxy_service::getInstance().send_message(connindex,  &msg, sizeof(msg));
	}
	void load_role_pk_top_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cross_get_new_ranking()");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_role_pk_top_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_role_pk_info_lua(uint32 connindex, int64 role_guid, int32 tempalte_id, int32 role_gs, xstring role_name)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_cross_load_role_info(")
			<< role_guid
			<< _XTEXT(",'")
			<< role_name << _XTEXT("',")
			<< tempalte_id << _XTEXT(",")
			<< role_gs << _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_role_pk_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_role_pk_info_lua(uint32 connindex, int64 role_guid, xstring target_name1, xstring target_name2, xstring target_name3, int32 laddet_score, int32 win_result, int32 cross_honor)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_save_role_info(")
			<< role_guid
			<< _XTEXT(",")
			<< laddet_score
			<< _XTEXT(",'")
			<< target_name1
			<< _XTEXT("','")
			<< target_name2
			<< _XTEXT("','")
			<< target_name3
			<< _XTEXT("',")
			<< win_result
			<< _XTEXT(",")
			<< cross_honor
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_role_pk_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_role_pk_honor_refresh_lua(uint32 connindex, int64 role_guid, int32 refresh_time)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("CALL sp_cross_save_role_info_honor_refresh(")
			<< role_guid
			<< _XTEXT(",")
			<< refresh_time
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_role_pk_honor_refresh_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_role_pk_log_lua(uint32 connindex, int64 role_guid, int32 server_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("CALL sp_cross_war_info_load(") << role_guid << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_role_pk_log_end, _1, connindex, role_guid, server_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}
