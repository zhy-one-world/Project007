/********************************************************************
created:	2016年12月12日12:57:20
file base:	msgproc_legion_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_legion_ws.hpp"
#include <legion_msg.hpp>
#include "dbproxy_service.hpp"
#include "dbproxy_service.hpp"
#include "data_manager/data_manager.hpp"
#include "msgproc_ranking_ws.hpp"
#include "utility/parse_msg.h"

namespace hld
{
	void save_legion_info_lua(uint32 connindex, int64 legion_guid, xstring legion_name, xstring announcement, int64 gs_value, int64 data_ary[ELegionInfo_max])
	{
		xchar	t_legion_name[max_name_size + 1];
		memset(t_legion_name, 0, sizeof(t_legion_name));
		memcpy(t_legion_name, legion_name.c_str(), legion_name.size() > sizeof(t_legion_name) ? sizeof(t_legion_name) : legion_name.size());


		xchar	t_announcement[max_legion_announcement_size + 1];
		memset(t_announcement, 0, sizeof(t_announcement));
		memcpy(t_announcement, announcement.c_str(), announcement.size() > sizeof(t_announcement) ? sizeof(t_announcement) : announcement.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_info_save(";
		sql << legion_guid << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << "\\\'" << t_legion_name << "\\\',";
		sql << "\\\'" << t_announcement << "\\\',";
		sql << gs_value << ",";
		int32 last_index = ELegionInfo_max - 1;
		for (int32 i = 0; i < ELegionInfo_max; ++i)
		{
			sql << data_ary[i];
			if (i != last_index)
			{
				sql << ",";
			}
		}
		sql << ")');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_legion_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const server2dp_proto_s_legion_info& legion_info = request.legion_info();

		int64 t_data_array[ELegionInfo_max];
		for (int32 i = 0; i < legion_info.data_ary_size(); i++)
		{
			if (i >= ELegionInfo_max)
			{
				break;
			}
			t_data_array[i] = legion_info.data_ary(i);
		}

		save_legion_info_lua(connindex, legion_info.legion_guid(), legion_info.legion_name(), legion_info.announcement(), legion_info.gs_value(), t_data_array);
	}

	void save_legion_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void load_all_legion_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_info_load_all();");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_info_end, _1, connindex, SERVERCONFIG->game_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void load_all_legion_info_end(db_result_type result, uint32 connindex, int32 server_id)
	{
 		if (result.error)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_info load_all_legion_info_msg;
			int32 data_num = result.query.data_select.row_count;
			if (data_num > 0)
			{				
				memset(legion_info_arr_dp, 0, sizeof(legion_info_arr_dp));
				if (data_num > max_legion_count)
				{
					data_num = max_legion_count;
				}
				int32 data_size = result.query.data_select.row_size * data_num;
				db_read_data(&result, legion_info_arr_dp, data_size);
				int32 send_count = (data_num - 1) / db_legion_count + 1;
				for (int32 i = 0; i < send_count - 1; ++i)
				{
					load_all_legion_info_msg.is_end = false;
					load_all_legion_info_msg.legion_info_num = db_legion_count;
					memcpy(load_all_legion_info_msg.legion_info, &(legion_info_arr_dp[i * db_legion_count]), sizeof(s_legion_info) * db_legion_count);
					dbproxy_service::getInstance().send_message(connindex, &load_all_legion_info_msg, load_all_legion_info_msg.get_pak_length());
				}
				int32 end_count = data_num - db_legion_count * (send_count - 1);
				load_all_legion_info_msg.is_end = true;
				load_all_legion_info_msg.legion_info_num = end_count;
				memcpy(load_all_legion_info_msg.legion_info, &(legion_info_arr_dp[data_num - end_count]), sizeof(s_legion_info) * end_count);
				dbproxy_service::getInstance().send_message(connindex, &load_all_legion_info_msg, load_all_legion_info_msg.get_pak_length());
				load_all_legion_member_info(connindex, server_id, legion_info_arr_dp, 0, data_num);
				load_all_legion_applicant(connindex, server_id, legion_info_arr_dp, 0, data_num);
				load_all_legion_warehouse_info(connindex, server_id, legion_info_arr_dp, 0, data_num);
				load_all_legion_warehouse_log(connindex, server_id, legion_info_arr_dp, 0, data_num);
				load_all_legion_city_war_member_info(connindex, server_id, legion_info_arr_dp, 0, data_num);
				load_all_legion_event(connindex, server_id, legion_info_arr_dp, 0, data_num);

				load_legion_bonus_info(connindex);
				load_war_bid_info(connindex);
				load_city_info(connindex);
				return;
			}
			else
			{
				load_all_legion_info_msg.is_end = true;
				dbproxy_service::getInstance().send_message(connindex, &load_all_legion_info_msg, load_all_legion_info_msg.get_pak_length());
				load_legion_bonus_info(connindex);
				load_war_bid_info(connindex);
				load_city_info(connindex);
			}
			return;
		}
	}

	void delete_legion_city_war_member_info_lua(uint32 connindex, int32 del_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_city_war_member_info_clear(";
		sql << del_type << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&delete_legion_member_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void delete_legion_city_war_member_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_delete_legion_city_war_member_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		delete_legion_city_war_member_info_lua(connindex, request.del_type());
	}

	void delete_legion_city_war_member_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}

	void delete_legion_member_info_lua(uint32 connindex, int64 role_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_member_info_delete(");
		sql << role_guid << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&delete_legion_member_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void delete_legion_member_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_delete_legion_member_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		delete_legion_member_info_lua(connindex, request.role_guid());
	}

	void delete_legion_member_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}

	void delete_legion_info_lua(uint32 connindex, int64 legion_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_info_delete(");
		sql << legion_guid << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&delete_legion_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void delete_legion_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_delete_legion_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		delete_legion_info_lua(connindex, request.legion_guid());
	}

	void delete_legion_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void save_legion_city_war_member_info_lua(uint32 connindex, int64 legion_guid, int64 role_guid, int64 use_type, xstring role_name, int64 player_temp_id, int64 week_contribution, int64 job_title)
	{
		xchar	t_role_name[max_name_size + 1];
		memset(t_role_name, 0, sizeof(t_role_name));
		memcpy(t_role_name, role_name.c_str(), role_name.size() > sizeof(t_role_name) ? sizeof(t_role_name) : role_name.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_city_war_member_info_save(";
		sql << role_guid << ",";
		sql << use_type << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << role_guid << ",";
		sql <<use_type << ",";
		sql << "\"" <<	t_role_name << "\",";
		sql << player_temp_id << ",";
		sql << week_contribution << ",";
		sql << job_title << ")');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_city_war_member_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_legion_city_war_member_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_city_war_member_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const server2dp_proto_s_legion_city_war_member_info& member_info = request.legion_member_info();
		save_legion_city_war_member_info_lua(connindex, request.legion_guid(), member_info.role_guid(), member_info.use_type(), member_info.role_name(), member_info.player_temp_id(), member_info.week_contribution(), member_info.job_title());
	}

	void save_legion_city_war_member_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}

	void save_legion_member_info_lua(uint32 connindex, int64 legion_guid, int64 role_guid, xstring role_name, int64 gs_value, int64 data_ary[e_legion_member_info_max])
	{
		xchar	t_role_name[max_name_size + 1];
		memset(t_role_name, 0, sizeof(t_role_name));
		memcpy(t_role_name, role_name.c_str(), role_name.size() > sizeof(t_role_name) ? sizeof(t_role_name) : role_name.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_member_info_save(";
		sql << role_guid << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << role_guid << ",";
		sql << "\"" << t_role_name << "\",";
		sql << gs_value << ",";
		int32 last_index = e_legion_member_info_max - 1;
		for (int32 i = 0; i < e_legion_member_info_max; ++i)
		{
			sql << data_ary[i];
			if (i != last_index)
			{
				sql << ",";
			}
		}
		sql << ")');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_member_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_legion_member_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_member_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const server2dp_proto_s_legion_member_info& member_info = request.legion_member_info();

		int64 t_data_array[e_legion_member_info_max];
		for (int32 i = 0; i < member_info.data_ary_size(); i++)
		{
			if (i >= e_legion_member_info_max)
			{
				break;
			}
			t_data_array[i] = member_info.data_ary(i);
		}

		save_legion_member_info_lua(connindex, request.legion_guid(), member_info.role_guid(), member_info.role_name(), member_info.gs_value(), t_data_array);
	}

	void save_legion_member_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void load_all_legion_city_war_member_info(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (use_index >= max_index)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_legion_city_war_member_info_load_all(");
		sql << legion_array[use_index].legion_guid.server_64 << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_city_war_member_info_end, _1, connindex, server_id, legion_array, use_index, max_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_all_legion_city_war_member_info_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (result.error)
		{
			return;
		}
		dp2ws_load_all_legion_city_war_member_info load_all_legion_member_info_msg;
		int32 data_num = result.query.data_select.row_count;
		if (data_num > 0)
		{
			if (data_num > max_legion_member_num)
			{
				data_num = max_legion_member_num;
			}
			int32 data_size = result.query.data_select.row_size * data_num;
			s_legion_city_war_member_info_db legion_member_info_db[max_legion_member_num];
			db_read_data(&result, legion_member_info_db, data_size);

			load_all_legion_member_info_msg.legion_guid = legion_member_info_db[0].legion_guid;
			for (int32 i = 0; i < data_num; ++i)
			{
				load_all_legion_member_info_msg.member_info[i] = legion_member_info_db[i].legion_member_info;
			}
		}
		load_all_legion_member_info_msg.member_info_num = data_num;

		dbproxy_service::getInstance().send_message(connindex, &load_all_legion_member_info_msg, load_all_legion_member_info_msg.get_pak_length());
		load_all_legion_city_war_member_info(connindex, server_id, legion_array, use_index + 1, max_index);
		return;
	}

	void load_all_legion_member_info(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (use_index >= max_index)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_legion_member_info_load_all(");
		sql << legion_array[use_index].legion_guid.server_64 <<");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_member_info_end, _1, connindex, server_id, legion_array, use_index, max_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_all_legion_member_info_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (result.error)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_member_info load_all_legion_member_info_msg;
			int32 data_num = result.query.data_select.row_count;
			if (data_num > 0)
			{
				if (data_num > max_legion_member_num)
				{
					data_num = max_legion_member_num;
				}
				int32 data_size = result.query.data_select.row_size * data_num;
				s_legion_member_info_db legion_member_info_db[max_legion_member_num];
				db_read_data(&result, legion_member_info_db, data_size);
				
				load_all_legion_member_info_msg.legion_guid = legion_member_info_db[0].legion_guid;
				for (int32 i = 0; i < data_num; ++i)
				{
					load_all_legion_member_info_msg.member_info[i] = legion_member_info_db[i].legion_member_info;
				}
				CONSOLE_INFO("load_all_legion_member_info_end data_num:{} legion_guid :{} data_size:{}", data_num, load_all_legion_member_info_msg.legion_guid.server_64, data_size);
			}
			CONSOLE_INFO("load_all_legion_member_info_end data_num:{} legion_guid:{}", data_num, load_all_legion_member_info_msg.legion_guid.server_64);

			load_all_legion_member_info_msg.member_info_num = data_num;

			dbproxy_service::getInstance().send_message(connindex, &load_all_legion_member_info_msg, load_all_legion_member_info_msg.get_pak_length());
			load_all_legion_member_info(connindex, server_id, legion_array, use_index + 1, max_index);
			return;
		}
	}
	void delete_legion_applicant_lua(uint32 connindex, int64 role_guid)
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << it->second << ".sp_legion_applicant_del_one(";
			sql << role_guid << ");";
			db_query_type query;
			query.logic_id = e_data_flag_type_legion_applicant;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&delete_legion_applicant_end, _1, connindex);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void delete_legion_applicant(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_del_legion_applicant_one request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		delete_legion_applicant_lua(connindex, request.role_guid());
	}

	void delete_legion_applicant_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}

	void save_legion_applicant_lua(uint32 connindex, int64 legion_guid, int64 role_guid, xstring role_name, float exist_time, int64 gs_value, int32 data_ary[ELegionApplicantInfo_max])
	{
		xchar	t_role_name[max_name_size + 1];
		memset(t_role_name, 0, sizeof(t_role_name));
		memcpy(t_role_name, role_name.c_str(), role_name.size() > sizeof(t_role_name) ? sizeof(t_role_name) : role_name.size());

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_applicant_save(";
		sql << legion_guid << ",";
		sql << role_guid << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << role_guid << ",";
		sql << "\"" << t_role_name << "\",";
		sql << exist_time << ",";
		sql << gs_value << ",";
		int32 last_index = ELegionApplicantInfo_max - 1;
		for (int32 i = 0; i < ELegionApplicantInfo_max; ++i)
		{
			sql << data_ary[i];
			if (i != last_index)
			{
				sql << ",";
			}
		}
		sql << ")');";

		db_query_type query;
		query.logic_id = e_data_flag_type_legion_applicant;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_applicant_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_legion_applicant(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_applicant request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		int32 t_data_array[ELegionApplicantInfo_max];
		for (int32 i = 0; i < request.data_ary_size(); i++)
		{
			if (i >= ELegionApplicantInfo_max)
			{
				break;
			}
			t_data_array[i] = request.data_ary(i);
		}
		save_legion_applicant_lua(connindex, request.legion_guid(), request.role_guid(), request.role_name(), request.exist_time(), request.gs_value(), t_data_array);

	}

	void save_legion_applicant_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void load_all_legion_applicant(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (use_index >= max_index)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_legion_applicant_load_all(");
		sql << legion_array[use_index].legion_guid.server_64 <<");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_applicant_end, _1, connindex, server_id, legion_array, use_index, max_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_all_legion_applicant_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (result.error)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_applicant load_all_applicant_msg;
			load_all_applicant_msg.applicant_info_num = result.query.data_select.row_count;
			if (load_all_applicant_msg.applicant_info_num > max_legion_applicant_num)
			{
				return;
			}
			int32 data_size = result.query.data_select.row_size * load_all_applicant_msg.applicant_info_num;
			db_read_data(&result, load_all_applicant_msg.applicant_info, data_size);

			dbproxy_service::getInstance().send_message(connindex, &load_all_applicant_msg, sizeof(load_all_applicant_msg));
			load_all_legion_applicant(connindex, server_id, legion_array, use_index + 1, max_index);
			return;
		}
	}

	void save_legion_boss_award_get_log_lua(uint32 connindex, int64 role_guid, int32 boss_map_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call " << data_manager::get_instance().get_db_name(role_guid) << ".sp_legion_get_boss_award_log_save(";
		sql << role_guid << ",";
		sql << boss_map_id << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_boss_award_get_log_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_legion_boss_award_get_log(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_boss_award_get_log request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_legion_boss_award_get_log_lua(connindex, request.role_guid(), request.boss_map_id());
	}

	void save_legion_boss_award_get_log_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void load_all_legion_boss_award_get_log_lua(uint32 connindex, int64 role_guid)
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << it->second << ".sp_legion_get_boss_award_log_load_all(";
			sql << role_guid << ");";

			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&load_all_legion_boss_award_get_log_end, _1, connindex);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void load_all_legion_boss_award_get_log(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_all_legion_boss_award_get_log request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_all_legion_boss_award_get_log_lua(connindex, request.role_guid());

	}

	void load_all_legion_boss_award_get_log_end(db_result_type result, uint32 connindex)
	{
		if (result.error || result.query.data_select.row_count <= 0)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_boss_award_get_log load_all_legion_boss_award_get_log_msg;
			int32 data_num = result.query.data_select.row_count;
			load_all_legion_boss_award_get_log_msg.legion_boss_award_get_log.get_logs_num = data_num;
			int32 data_size = result.query.data_select.row_size * data_num;
			s_legion_boss_award_get_log_db temp_log_ary[max_legion_boss_record_num];
			db_read_data(&result, temp_log_ary, data_size);

			load_all_legion_boss_award_get_log_msg.legion_boss_award_get_log.role_guid = temp_log_ary[0].role_guid;
			for (int32 i = 0; i < data_num; ++i)
			{
				load_all_legion_boss_award_get_log_msg.legion_boss_award_get_log.get_logs[i] = temp_log_ary[i].boss_map_id;
			}

			dbproxy_service::getInstance().send_message(connindex, &load_all_legion_boss_award_get_log_msg, sizeof(load_all_legion_boss_award_get_log_msg));
			return;
		}
	}

	void save_city_info_lua(uint32 connindex, int64 occupation_legion_guid, xstring city_master_name)
	{
		xchar	t_city_master_name[max_name_size + 1];
		memset(t_city_master_name, 0, sizeof(t_city_master_name));
		memcpy(t_city_master_name, city_master_name.c_str(), city_master_name.size() > sizeof(t_city_master_name) ? sizeof(t_city_master_name) : city_master_name.size());
	

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_city_info_save(";
		sql << "'(" << occupation_legion_guid << ",";
		sql << "\\\'" << t_city_master_name << "\\\'";
		sql << ")');";


		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_city_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	
	}

	void save_city_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_city_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_city_info_lua(connindex, request.occupation_legion_guid(), request.city_master_name());
	}

	void save_city_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void load_city_info(uint32 connindex)
	{

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_city_info_load();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_city_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}
	void load_city_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error || result.query.data_select.row_count <= 0 || result.query.data_select.row_count > 1)
		{
			return;
		}
		else
		{
			dp2ws_load_city_info load_city_info_msg;
			int32 data_num = result.query.data_select.row_count;
			int32 data_size = result.query.data_select.row_size * data_num;
			db_read_data(&result, &(load_city_info_msg.overload_info), data_size);
			dbproxy_service::getInstance().send_message(connindex, &load_city_info_msg, sizeof(load_city_info_msg));
			return;
		}
	}
	
	void save_war_bid_info_lua(uint32 connindex, int32 territory_id, int32 index_in_terr, int64 legion_guid, int32 cur_bid_price, int32 is_prior_maintain, int32 occupy_time_stamp, int32 server_id, xstring legion_name)
	{
		xchar	t_role_mark[max_name_size + 1];
		memset(t_role_mark, 0, sizeof(t_role_mark));
		memcpy(t_role_mark, legion_name.c_str(), legion_name.size() > sizeof(t_role_mark) ? sizeof(t_role_mark) : legion_name.size());

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_city_bid_info_save(";
		sql << territory_id << ",";
		sql << index_in_terr << ",";
		sql << "'(";
		sql << territory_id << ",";
		sql << index_in_terr << ",";
		sql << legion_guid << ",";
		sql << cur_bid_price << ",";
		sql << is_prior_maintain << ",";
		sql << occupy_time_stamp << ",";
		sql << server_id << ",";
		sql << "\\\'" << t_role_mark << "\\\')'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_war_bid_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_war_bid_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_war_bid_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const server2dp_proto_s_city_war_bid_info& bid_info = request.bid_info();
		save_war_bid_info_lua(connindex, bid_info.territory_id(), bid_info.index_in_terr(), bid_info.legion_guid(), bid_info.cur_bid_price(),
			bid_info.is_prior_maintain(), bid_info.occupy_time_stamp(), bid_info.server_id(), bid_info.legion_name());
	}

	void save_war_bid_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {
			return;
		} else {
			
		}
	}
	
	void load_war_bid_info(uint32 connindex)
	{

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_city_bid_info_load_all();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_war_bid_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void load_war_bid_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error )//|| result.query.data_select.row_count <= 0
		{
			return;
		}
		else
		{
			dp2ws_load_all_war_bid_info load_all_war_bid_info_msg;
			int32 data_num = result.query.data_select.row_count;
			if (data_num > all_territory_bid_info_num)
			{
				data_num = all_territory_bid_info_num;
			}
			int32 data_size = result.query.data_select.row_size * data_num;
			db_read_data(&result, &load_all_war_bid_info_msg.bid_infos, data_size);
			load_all_war_bid_info_msg.data_num = data_num;
			dbproxy_service::getInstance().send_message(connindex, &load_all_war_bid_info_msg, load_all_war_bid_info_msg.get_pak_length());
			return;
		}
	}

	void clear_war_bid_info_lua(uint32 connindex, int32 clear_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_city_bid_info_clear();";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_war_bid_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_war_bid_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_clear_war_bid_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		clear_war_bid_info_lua(connindex, request.clear_type());
	}

	void clear_war_bid_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}
	
	void load_all_legion_event(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (use_index >= max_index)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_legion_event_load_all(");
		sql << legion_array[use_index].legion_guid.server_64 << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_event_end, _1, connindex, server_id, legion_array, use_index, max_index);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}

	void load_all_legion_event_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (result.error)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_event load_all_legion_event_msg;
			int32 data_num = result.query.data_select.row_count;
			if (data_num > max_legion_event_num)
			{
				data_num = max_legion_event_num;
			}
			int32 data_size = result.query.data_select.row_size * data_num;
			db_read_data(&result, &load_all_legion_event_msg.event_datas, data_size);
			load_all_legion_event_msg.data_num = data_num;
			dbproxy_service::getInstance().send_message(connindex, &load_all_legion_event_msg, load_all_legion_event_msg.get_pak_length());
			load_all_legion_event(connindex, server_id, legion_array, use_index + 1, max_index);
			return;
		}
	}

	void save_legion_event_lua(uint32 connindex, int64 legion_guid, int32 event_idx, xstring extra_data, int32 happen_time)
	{
		xchar	t_extra_data[max_once_legion_event_size + 1];
		memset(t_extra_data, 0, sizeof(t_extra_data));
		memcpy(t_extra_data, extra_data.c_str(), extra_data.size() > sizeof(t_extra_data) ? sizeof(t_extra_data) : extra_data.size());

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_event_save_one(";
		sql << legion_guid << ",";
		sql << event_idx << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << event_idx << ",";
		sql << "\"" << t_extra_data << "\",";
		sql << happen_time << ")'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_event_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void save_legion_event(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_event request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const server2dp_proto_s_legion_event& event_info = request.event_data();
		save_legion_event_lua(connindex, event_info.legion_guid(), event_info.event_idx(), event_info.extra_data(), event_info.happen_time());
	}

	void save_legion_event_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void del_legion_event(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_del_legion_event* pdata = static_cast<const ws2dp_del_legion_event*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_del_legion_event))
		{
			return;
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_event_del_one(";
		sql << pdata->legion_guid.server_64 << ",";
		sql << pdata->event_idx;
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&del_legion_event_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void del_legion_event_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void clear_legion_event_lua(uint32 connindex, int64 legion_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_event_clear(";
		sql << legion_guid << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_legion_event_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_legion_event(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_clear_legion_event request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		clear_legion_event_lua(connindex, request.legion_guid());
	}

	void clear_legion_event_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {} else {}
	}

	void clear_overlord_reward_stuff_func(uint32 connindex, const int32& mount_template_id, const int32& wing_template_id, const int32& title_id, const int32& buff_if)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_overlord_legion_clear_all(";
		sql << mount_template_id <<","
			<< wing_template_id << ","
			<< title_id << ","
			<< buff_if
			<< ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_city_war_reward_stuff_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_city_war_reward_stuff_lua(uint32 connindex, int32 del_stuff_type, int64 target_guid)
	{
		switch ((e_del_overlord_stuff_event)del_stuff_type)
		{
		case e_del_overlord_stuff_event_all_legion:
		{
			//clear_overlord_reward_stuff_func(connindex, city_master_mount_id, city_master_wing_id, city_master_title_id, city_master_buff_id);
			clear_overlord_reward_stuff_func(connindex, lord_legion_mem_mount_id, 0, city_war_winner_title_id, 0);
			clear_overlord_reward_stuff_func(connindex, lord_legion_mem_mount_id, 0, city_war_cross_winner_title_id, 0);
		}
		break;
		case e_del_overlord_stuff_event_all_member:
		{
			clear_overlord_reward_stuff_func(connindex, lord_legion_mem_mount_id, 0, city_war_winner_title_id, 0);
			clear_overlord_reward_stuff_func(connindex, lord_legion_mem_mount_id, 0, city_war_cross_winner_title_id, 0);
		}
		break;
		case e_del_overlord_stuff_event_only_master:
		{
			clear_overlord_reward_stuff_func(connindex, city_master_mount_id, city_master_wing_id, city_master_title_id, city_master_buff_id);
			clear_overlord_reward_stuff_func(connindex, city_master_mount_id, city_master_wing_id, city_war_cross_master_title_id, city_master_buff_id);
		}
		break;
		case e_del_overlord_stuff_event_only_one_member:
		{
			clear_only_one_member_title(city_war_winner_title_id, target_guid, connindex);
			clear_only_one_member_title(city_war_cross_winner_title_id, target_guid, connindex);
		}
		break;
		default:
			return;
		}
	}

	void clear_city_war_reward_stuff(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_clear_overlord_legion_stuff request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		clear_city_war_reward_stuff_lua(connindex, request.del_stuff_type(), request.target_guid());
	}

	void clear_only_one_member_title(int32 title_temp_id, guid_64 role_guid, int32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_overlord_legion_clear_one_member_stuff(";
		sql << role_guid.server_64 << ","
			<< lord_legion_mem_mount_id << ","
			<< 0 << ","
			<< title_temp_id << ","
			<< 0
			<< ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_city_war_reward_stuff_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_city_war_reward_stuff_end(db_result_type result, uint32 connindex)
	{

	}

	void load_legion_bonus_info(uint32 connindex)
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << it->second << _XTEXT(".sp_legion_bonus_info_load_all();");
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&load_legion_bonus_info_end, _1, connindex, it->first);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void load_legion_bonus_info_end(db_result_type result, uint32 connindex, int32 server_id)
	{
		if (result.error || result.query.data_select.row_count <= 0)
		{
			return;
		}
		else
		{
			dp2ws_load_legion_bonus_info load_legion_bonus_info_msg;
			int32 data_num = result.query.data_select.row_count;
			if (data_num > max_legion_bonus_mission_num*max_legion_count)
			{
				data_num = max_legion_bonus_mission_num*max_legion_count;
			}
			int32 data_size = result.query.data_select.row_size * data_num;
			db_read_data(&result, &(load_legion_bonus_info_msg.bonus_info), data_size);
			load_legion_bonus_info_msg.data_num = data_num;
			dbproxy_service::getInstance().send_message(connindex, &load_legion_bonus_info_msg, sizeof(load_legion_bonus_info_msg));
			return;
		}
	}

	void save_legion_bonus_info_lua(uint32 connindex, int64 legion_guid, int32 mission_id, int32 finish_count)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_bonus_info_save(";
		sql << legion_guid << ",";
		sql << mission_id << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << mission_id << ",";
		sql << finish_count << ")'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_legion_bonus_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_legion_bonus_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_legion_bonus_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_legion_bonus_info_lua(connindex, request.legion_guid(), request.mission_id(), request.finish_count());
	}

	void save_legion_bonus_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}
	
	void clear_legion_bonus_info_lua(uint32 connindex, int64 legion_guid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_legion_bonus_info_clear(";
		sql << legion_guid << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_legion_bonus_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_legion_bonus_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_clear_legion_bonus_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		clear_legion_bonus_info_lua(connindex, request.legion_guid());
	}

	void clear_legion_bonus_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error) {}
		else {}
	}
	void load_all_legion_warehouse_info(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (use_index >= max_index)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_legion_warehouse_load_all(");
		sql << legion_array[use_index].legion_guid.server_64 << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_warehouse_info_end, _1, connindex, server_id, legion_array, use_index, max_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_all_legion_warehouse_info_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (result.error)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_warehouse_info load_legion_warehouse_info_msg;
			int32 data_num = result.query.data_select.row_count;
			load_legion_warehouse_info_msg.legion_warehouse_num = data_num;
			int32 data_size = result.query.data_select.row_size * data_num;
			db_read_data(&result, load_legion_warehouse_info_msg.legion_warehouse, data_size);
			dbproxy_service::getInstance().send_message(connindex, &load_legion_warehouse_info_msg, load_legion_warehouse_info_msg.get_pak_length());
			load_all_legion_warehouse_info(connindex, server_id, legion_array, use_index + 1, max_index);
			return;
		}
	}

	void load_all_legion_warehouse_log(uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (use_index >= max_index)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_legion_warehouse_load_log(");
		sql << legion_array[use_index].legion_guid.server_64 << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_legion_warehouse_log_end, _1, connindex, server_id, legion_array, use_index, max_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_all_legion_warehouse_log_end(db_result_type result, uint32 connindex, int32 server_id, s_legion_info* legion_array, int32 use_index, int32 max_index)
	{
		if (result.error)
		{
			return;
		}
		else
		{
			dp2ws_load_all_legion_warehouse_log load_legion_warehouse_log_msg;
			int32 data_num = result.query.data_select.row_count;
			load_legion_warehouse_log_msg.log_num = data_num;
			int32 data_size = result.query.data_select.row_size * data_num;
			db_read_data(&result, load_legion_warehouse_log_msg.log, data_size);
			dbproxy_service::getInstance().send_message(connindex, &load_legion_warehouse_log_msg, load_legion_warehouse_log_msg.get_pak_length());
			load_all_legion_warehouse_log(connindex, server_id, legion_array, use_index + 1, max_index);
			return;
		}
	}

	// 玩家存入道具到数据库
	void store_legion_warehouse(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_store_legion_warehouse_info* pdata = static_cast<const ws2dp_store_legion_warehouse_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_store_legion_warehouse_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_warehouse_store_item(");
		sql << pdata->legion_info.item_info.item_guid.server_64 << ",";
		sql << pdata->legion_info.legion_guid.server_64 << ",'(";

		sql << pdata->legion_info.legion_guid.server_64 << ",";
		sql << pdata->legion_info.add_time << ",";
		sql << pdata->legion_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			sql << pdata->legion_info.item_info.data_ary[i] << ",";
			//if (i != last_index)
			//{
			//	sql << ",";
			//}
		}
		sql << "\\\'" << pdata->legion_info.item_info.param_str << "\\\'";

		sql << ")'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&store_legion_warehouse_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void store_legion_warehouse_end(db_result_type result, uint32 connindex) 
	{
#pragma pack(push,1)
		struct _create_char_ret
		{
			int32 nres;
		};
#pragma pack(pop)
		_create_char_ret data;
		memset(&data, 0, sizeof(data));
		if (sizeof(data) < result.query.data_select.row_count * result.query.data_select.row_size)
		{
			return;
		}
		db_read_data(&result, &data, result.query.data_select.row_count * result.query.data_select.row_size);
		int32 nres = data.nres;
		if (1 == nres)
		{
			// 数据库操作错误
			return;
		}
	}

	// 玩家存入道具到数据库
	void store_legion_warehouse_log(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_store_legion_warehouse_log* pdata = static_cast<const ws2dp_store_legion_warehouse_log*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_store_legion_warehouse_log))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_warehouse_log(");
		sql <<"'(";
		sql << pdata->log.role_guid.server_64 << ",";
		sql << pdata->log.item_guid.server_64 << ",";
		sql << pdata->log.legion_guid.server_64 << ",";
		sql << pdata->log.add_time << ",";
		sql << pdata->log.job << ",";
		sql << "\\\'" << pdata->log.role_name << "\\\',";
		sql << pdata->log.operation_type << ",";
		sql << pdata->log.item_template_id;
		sql << ")');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&store_legion_warehouse_log_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void store_legion_warehouse_log_end(db_result_type result, uint32 connindex)
	{
#pragma pack(push,1)
		struct _create_char_ret
		{
			int32 nres;
		};
#pragma pack(pop)
		_create_char_ret data;
		memset(&data, 0, sizeof(data));
		if (sizeof(data) < result.query.data_select.row_count * result.query.data_select.row_size)
		{
			return;
		}
		db_read_data(&result, &data, result.query.data_select.row_count * result.query.data_select.row_size);
		int32 nres = data.nres;
		if (1 == nres)
		{
			// 数据库操作错误
			return;
		}
	}
	//玩家从军团仓库里面获取道具
	void role_get_legion_warehouse(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_role_get_legion_warehouse_info* pdata = static_cast<const ws2dp_role_get_legion_warehouse_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_role_get_legion_warehouse_info))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_warehouse_role_get_item(");
		sql << pdata->legion_info.item_info.item_guid.server_64 << ",";
		sql << pdata->legion_info.legion_guid.server_64 << ",";
		sql << pdata->role_guid.server_64 << ",'(";

		sql << pdata->role_guid.server_64 << ",";
		sql << pdata->legion_info.item_info.item_guid.server_64 << ",";
		int32 last_index = e_item_info_max - 1;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			sql << pdata->legion_info.item_info.data_ary[i];
			if (i != last_index)
			{
				sql << ",";
			}
		}
		sql << ")','(";
		sql << pdata->log.role_guid.server_64 << ",";
		sql << pdata->log.item_guid.server_64 << ",";
		sql << pdata->log.legion_guid.server_64 << ",";
		sql << pdata->log.add_time << ",";
		sql << pdata->log.job << ",";
		sql << "\\\'" << pdata->log.role_name << "\\\',";
		sql << pdata->log.operation_type << ",";
		sql << pdata->log.item_template_id;

		sql << ")');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&role_get_legion_warehouse_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void role_get_legion_warehouse_end(db_result_type result, uint32 connindex)
	{
#pragma pack(push,1)
		struct _create_char_ret
		{
			int32 nres;
		};
#pragma pack(pop)
		_create_char_ret data;
		memset(&data, 0, sizeof(data));
		if (sizeof(data) < result.query.data_select.row_count * result.query.data_select.row_size)
		{
			return;
		}
		db_read_data(&result, &data, result.query.data_select.row_count * result.query.data_select.row_size);
		int32 nres = data.nres;
		if (1 == nres)
		{
			// 数据库操作错误
			return;
		}
	}

	void destroy_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_destroy_legion_item* pdata = static_cast<const ws2dp_destroy_legion_item*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_destroy_legion_item))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_warehouse_destroy('");

		
		int32 last_index = pdata->del_num - 1;
		for (int32 i = 0; i < pdata->del_num; i++)
		{
			sql << pdata->del_guid_array[i].server_64;
			if (i != last_index)
			{
				sql << ",";
			}
		}

		sql << "','";
		sql << " insert into legion_warehouse_log values ";
		last_index = pdata->del_log_num - 1;
		for (int32 i = 0; i < pdata->del_log_num; i++)
		{
			sql << "( ";
			sql << pdata->log.role_guid.server_64 << ",";
			sql << pdata->del_guid_array[i].server_64 << ",";
			sql << pdata->log.legion_guid.server_64 << ",";
			sql << pdata->log.add_time << ",";
			sql << pdata->log.job << ",";
			sql << "\\\'" << pdata->log.role_name << "\\\',";
			sql << pdata->log.operation_type << ",";
			sql << pdata->del_item_id[i];

			if (i != last_index)
			{
				sql << "),";
			}
			else
			{
				sql << "); ";
			}
		}
		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&destroy_legion_warehouse_item_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void destroy_legion_warehouse_item_end(db_result_type result, uint32 connindex)
	{
#pragma pack(push,1)
		struct _create_char_ret
		{
			int32 nres;
		};
#pragma pack(pop)
		_create_char_ret data;
		memset(&data, 0, sizeof(data));
		if (sizeof(data) < result.query.data_select.row_count * result.query.data_select.row_size)
		{
			return;
		}
		db_read_data(&result, &data, result.query.data_select.row_count * result.query.data_select.row_size);
		int32 nres = data.nres;
		if (1 == nres)
		{
			// 数据库操作错误
			return;
		}
	}

	void clear_legion_warehouse(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const ws2dp_clear_legion_warehouse* pdata = static_cast<const ws2dp_clear_legion_warehouse*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_clear_legion_warehouse))
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_legion_warehouse_clear(");
		sql << pdata->legion_guid.server_64;
		sql << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_legion_warehouse_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void clear_legion_warehouse_end(db_result_type result, uint32 connindex)
	{
#pragma pack(push,1)
		struct _create_char_ret
		{
			int32 nres;
		};
#pragma pack(pop)
		_create_char_ret data;
		memset(&data, 0, sizeof(data));
		if (sizeof(data) < result.query.data_select.row_count * result.query.data_select.row_size)
		{
			return;
		}
		db_read_data(&result, &data, result.query.data_select.row_count * result.query.data_select.row_size);
		int32 nres = data.nres;
		if (1 == nres)
		{
			// 数据库操作错误
			return;
		}
	}

	void load_cross_city_info_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cross_server_overload_city_load();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_cross_city_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_cross_city_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		/*server2dp_proto_ws2dp_load_cross_city_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}*/
		load_cross_city_info_lua(connindex);
	}

	void load_cross_city_info_end(db_result_type result, uint32 connindex)
	{
		s_cross_db_city_info sql_data[max_cross_server_all_city_num];
		int32 data_num = result.query.data_select.row_count;
		int32 data_size = sizeof(s_cross_city_info);
		if (result.error || data_num > max_cross_server_all_city_num)
		{
			return;
		}
		else if (data_num <= 0)
		{
			dp2ws_load_cross_city_info_all reply_msg;
			reply_msg.is_empty_msg = true;
			reply_msg.data_num = 0;
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, reply_msg.get_pak_length());
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * data_num);
			dp2ws_load_cross_city_info_all reply_msg;
			reply_msg.data_num = data_num;
			reply_msg.is_empty_msg = false;
			for (int32 i = 0; i < data_num; i++)
			{
				reply_msg.city_info_msg[i].chief_guid = sql_data[i].chief_guid;
				reply_msg.city_info_msg[i].legion_guid = sql_data[i].legion_guid;
				reply_msg.city_info_msg[i].server_id = sql_data[i].server_id;
				reply_msg.city_info_msg[i].set_chief_name(sql_data[i].chief_name);
				reply_msg.city_info_msg[i].set_legion_name(sql_data[i].legion_name);
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_msg, reply_msg.get_pak_length());
			return;
		}
	}

	void save_cross_city_info_lua(uint32 connindex, uint32 rank_idex, int64 legion_guid, uint32 server_id, int64 chief_guid, xstring legion_name, xstring chief_name)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << "call sp_cross_server_overload_city_save(";
		sql << rank_idex << ",";
		sql << "'(";
		sql << legion_guid << ",";
		sql << server_id << ",";
		sql << chief_guid << ",";
		sql << rank_idex << ",";
		sql << "\\\'" << legion_name << "\\\',";
		sql << "\\\'" << chief_name << "\\\')'";
		sql << ");";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_cross_city_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_cross_city_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_cross_city_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_cross_city_info_lua(connindex, request.rank_idex(), request.city_info_msg().legion_guid(), request.city_info_msg().server_id(),
			request.city_info_msg().chief_guid(), request.city_info_msg().legion_name(), request.city_info_msg().chief_name());
	}

	void save_cross_city_info_end(db_result_type result, uint32 connindex)
	{

	}

	void clear_cross_city_info_lua(uint32 connindex)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_cross_server_overload_city_clear();");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&clear_cross_city_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void clear_cross_city_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		/*server2dp_proto_ws2dp_clear_cross_city_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}*/
		clear_cross_city_info_lua(connindex);
	}

	void clear_cross_city_info_end(db_result_type result, uint32 connindex)
	{

	}
}
