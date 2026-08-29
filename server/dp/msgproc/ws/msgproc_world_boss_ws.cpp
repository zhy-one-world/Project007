/********************************************************************
created:	2016Äê12ÔÂ12ÈÕ13:09:22
file base:	msgproc_world_boss_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_world_boss_ws.hpp"
#include "dbproxy_service.hpp"
#include <world_boss_msg.hpp>
#include <gain_treasure_msg.hpp>
#include "utility/parse_msg.h"

namespace faith
{
	
	void save_world_boss_to_sql(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_world_boss_to_db request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		int32	npc_index[max_world_boss_and_gold_army_num];
		int32	template_id[max_world_boss_and_gold_army_num];
		int32	is_alive[max_world_boss_and_gold_army_num];
		int32	line_id[max_world_boss_and_gold_army_num];
		int64	last_respawn_time[max_world_boss_and_gold_army_num];
		int32	respawn_phase[max_world_boss_and_gold_army_num];
		xstring	killer_name[max_world_boss_and_gold_army_num];
		xstring	first_killer_name[max_world_boss_and_gold_army_num];
		int32	first_killed_time[max_world_boss_and_gold_army_num];
		int32	is_get_prize[max_world_boss_and_gold_army_num];
		for (int32 i = 0; i < max_world_boss_and_gold_army_num; ++i)
		{
			npc_index[i] = request.npc_index(i);
			template_id[i] = request.template_id(i);
			is_alive[i] = request.is_alive(i);
			line_id[i] = request.line_id(i);
			last_respawn_time[i] = request.last_respawn_time(i);
			respawn_phase[i] = request.respawn_phase(i);
			killer_name[i] = request.killer_name(i);
			first_killer_name[i] = request.first_killer_name(i);
			first_killed_time[i] = request.first_killed_time(i);
			is_get_prize[i] = request.is_get_prize(i);
		}
		save_world_boss_to_sql_lua(connindex, npc_index, template_id, is_alive, line_id, last_respawn_time, respawn_phase, killer_name, first_killer_name, first_killed_time, is_get_prize);
	}
	void load_world_boss(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_world_boss request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_world_boss_lua(connindex);
	}
	void load_world_boss_end(db_result_type result, uint32 connindex)
	{
		s_world_boss_info sql_data[max_world_boss_and_gold_army_num];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_world_boss_info);
		if (result.error || table_len > max_world_boss_and_gold_army_num || data_size != result.query.data_select.row_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2ws_load_world_boss_end reply_data;

			reply_data.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_data.data_info[i]), &(sql_data[i]), sizeof(s_world_boss_info));
			}

			dbproxy_service::getInstance().send_message(connindex, &reply_data, reply_data.get_pak_length());
			return;
		}
	}

	void load_all_gain_treasure_record_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_all_gain_treasure_record_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_all_gain_treasure_record_info_lua(connindex, request.boss_spawn_point_id());
	}
	
	void load_all_gain_treasure_record_info_end(db_result_type result, uint32 connindex)
	{
		if (result.error || result.query.data_select.row_count <= 0)
		{
			return;
		}
		else
		{
			dp2ws_load_all_gain_treasure_record_info all_record_info_msg;
			int32 data_num = result.query.data_select.row_count;
			if (data_num > gain_treasure_killed_record_one_boss_max_size)
			{
				data_num = gain_treasure_killed_record_one_boss_max_size;
			}
			int32 data_size = result.query.data_select.row_size * data_num;
			int32 data_size2 = sizeof(s_gain_treasure_record_info_one_db) * data_num;
			db_read_data(&result, all_record_info_msg.record_infos, data_size2);
			all_record_info_msg.data_num = data_num;

			dbproxy_service::getInstance().send_message(connindex, &all_record_info_msg, all_record_info_msg.get_pak_length());
			return;
		}
	}

	void save_gain_treasure_record_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_gain_treasure_record_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		xstring role_name[max_team_member_num];
		xstring gain_item_template_id[max_team_member_num];
		int64 role_guid[max_team_member_num];

		for (int32 i = 0; i < max_team_member_num; ++i)
		{
			role_guid[i] = request.role_guid(i);
			role_name[i] = request.role_name(i);
			gain_item_template_id[i] = request.gain_item_template_id(i);
		}
		save_gain_treasure_record_info_lua(connindex, request.del_stamp(), request.boss_spawn_point_template_id(), request.killed_stamp(), role_name, gain_item_template_id, role_guid);
	}
	void load_all_gain_treasure_record_info_lua(uint32 connindex, uint32 boss_spawn_point_id)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_gain_treasure_record_info_load_all('");
		sql << boss_spawn_point_id << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_gain_treasure_record_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_gain_treasure_record_info_lua(uint32 connindex, uint32 del_stamp, uint32 boss_spawn_point_template_id, uint32 killed_stamp, xstring role_name[max_team_member_num], xstring gain_item_template_id[max_team_member_num], int64 role_guid[max_team_member_num])
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_gain_treasure_record_info_save('");
		sql << boss_spawn_point_template_id << "','";
		sql << del_stamp << "','";
		sql << killed_stamp << "','(";
		sql << boss_spawn_point_template_id << ",";
		sql << killed_stamp << ",";

		xstring gain_role_name[gain_treasure_gain_team_num];
		xstring item_template_id[gain_treasure_gain_team_num];
		int64 gain_role_guid[gain_treasure_gain_team_num];

		for (int32 i = 0; i < max_team_member_num; ++i)
		{
			gain_role_name[i] = role_name[i];
			item_template_id[i] = gain_item_template_id[i];
			gain_role_guid[i] = role_guid[i];
		}

		int32 tempLen = gain_treasure_gain_team_num - 1;
		for (int32 i = 0; i < gain_treasure_gain_team_num; i++)
		{
			sql << "\\\'" << gain_role_name[i] << "\\\',";
			sql << "\\\'" << item_template_id[i] << "\\\',";
			sql << gain_role_guid[i];
			if (tempLen == i)
			{
				sql << ")";
			}
			else
			{
				sql << ",";
			}
		}
		sql << "');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_world_boss_to_sql_lua(uint32 connindex, int32 npc_index[max_world_boss_and_gold_army_num], int32 template_id[max_world_boss_and_gold_army_num], int32 is_alive[max_world_boss_and_gold_army_num], int32 line_id[max_world_boss_and_gold_army_num], int64 last_respawn_time[max_world_boss_and_gold_army_num], int32 respawn_phase[max_world_boss_and_gold_army_num], xstring killer_name[max_world_boss_and_gold_army_num], xstring first_killer_name[max_world_boss_and_gold_army_num], int32 first_killed_time[max_world_boss_and_gold_army_num], int32 is_get_prize[max_world_boss_and_gold_army_num])
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_world_boss_save(") << "'";

		int32 data_num = 0;
		for (int32 i = 0; i < max_world_boss_and_gold_army_num - 1; i++)
		{
			if (template_id[i] > 0)
			{
				data_num++;
			}
		}
		int32 tempLen = data_num - 1;
		for (int32 i = 0; i < max_world_boss_and_gold_army_num - 1; i++)
		{
			if (template_id[i] <= 0)
			{
				continue;
			}
			sql << "(" << npc_index[i] << ",";
			sql << template_id[i] << ",";
			sql << is_alive[i] << ",";
			sql << line_id[i] << ",";
			sql << last_respawn_time[i] << ",";
			sql << respawn_phase[i] << ",";
			sql << "\\\'" << killer_name[i] << "\\\',";
			sql << "\\\'" << first_killer_name[i] << "\\\',";
			sql << first_killed_time[i] << ",";
			sql << is_get_prize[i];
			tempLen--;
			if (tempLen != 0)
			{
				sql << "),";
			}
			else
			{
				sql << ")";
				break;
			}
		}
		sql << "');";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}
	void load_world_boss_lua(uint32 connindex)
	{
		sql_builder _sql(db_manager::getInstance().get_db_link().game_db);
		_sql << _XTEXT("call sp_world_boss_load();");

		db_query_type query;
		query.sql_str.assign(_sql.c_str(), _sql.size());
		query.handler = boost::bind(&load_world_boss_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}
