/********************************************************************
created:	2016年12月12日12:51:28
file base:	msgproc_ranking_ws
file ext:	cpp
author:		zhy

purpose:	
*********************************************************************/
#include "msgproc_ranking_ws.hpp"
#include <ranking_msg.hpp>
#include <arena_msg.hpp>
#include "dbproxy_service.hpp"
#include "server_log.hpp"
#include "utility/parse_msg.h"
#include "dp2s.pb.h"
#include "net.pb.h"

namespace hld
{
	

	///////////////////////////////////ranking/////////////////////////////////////////////
	


	void save_arena_rank(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		hld::server2dp_proto::ws2dp_save_arena_rank request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
                       
		ws2dp_save_arena_rank packet;
		packet.from_proto(request);         
		save_arena_rank_(connindex, &packet, sizeof(ws2dp_save_arena_rank));
	}
	void save_arena_rank_(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		const hld::ws2dp_save_arena_rank* pdata = static_cast<const hld::ws2dp_save_arena_rank*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_rank_arena_save(" << pdata->is_first << ",'");

		int32 last_index = rank_save_per_time_max - 1;
		for (int32 i = 0; i < rank_save_per_time_max; ++i)
		{
			sql << "(" << pdata->data_list[i].rank_pos << ","
				<< pdata->data_list[i].role_guid.server_64 << ","
				<< "\\'" << pdata->data_list[i].role_name << "\\'" << ","
				<< pdata->data_list[i].gs_value << ","
				<< pdata->data_list[i].role_template_id << ","
				<< pdata->data_list[i].robot_template_id << ","
				<< pdata->data_list[i].military_rank_level << ","
				<< pdata->data_list[i].vip_level << ",\\'"
				<< pdata->data_list[i].game_channel << "\\'";
			if (i != last_index)
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
		query.logic_id = e_data_flag_type_role_ranking;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_arena_rank_end, _1);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void save_arena_rank_end(db_result_type result)
	{

	}

	void save_arena_rank_first_lua(uint32 connindex, uint32 t_rank_pos, int64 t_role_guid, xstring t_role_name, int64 t_gs_value, uint32 t_role_temp_id, uint32 t_robot_temp_id, uint32 t_military_rank_level, uint32 t_vip_level, xstring t_game_channel, uint32 n_rank_pos, int64 n_role_guid, xstring n_role_name, int64 n_gs_value, uint32 n_role_temp_id, uint32 n_robot_temp_id, uint32 n_military_rank_level, uint32 n_vip_level, xstring n_game_channel)
	{
		s_arena_rank my_new_data;
		my_new_data.rank_pos = n_rank_pos;
		my_new_data.role_guid = n_role_guid;
		my_new_data.gs_value = n_gs_value;
		my_new_data.role_template_id = n_role_temp_id;
		my_new_data.robot_template_id = n_robot_temp_id;
		my_new_data.military_rank_level = n_military_rank_level;
		my_new_data.vip_level = n_vip_level;
		my_new_data.set_role_name(n_role_name);
		my_new_data.set_game_channel(n_game_channel);

		s_arena_rank target_new_data;
		target_new_data.rank_pos = t_rank_pos;
		target_new_data.role_guid = t_role_guid;
		target_new_data.gs_value = t_gs_value;
		target_new_data.role_template_id = t_role_temp_id;
		target_new_data.robot_template_id = t_robot_temp_id;
		target_new_data.military_rank_level = t_military_rank_level;
		target_new_data.vip_level = t_vip_level;
		target_new_data.set_role_name(t_role_name);
		target_new_data.set_game_channel(t_game_channel);

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_rank_arena_save_one(");

		sql << my_new_data.role_guid.server_64 << ",'";

		sql << "(" << my_new_data.rank_pos << ","
			<< my_new_data.role_guid.server_64 << ","
			<< "\\'" << my_new_data.role_name << "\\'" << ","
			<< my_new_data.gs_value << ","
			<< my_new_data.role_template_id << ","
			<< my_new_data.robot_template_id << ","
			<< my_new_data.military_rank_level << ","
			<< my_new_data.vip_level << ",\\'"
			<< my_new_data.game_channel << "\\')";
		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_type_role_ranking;
		query.handler = boost::bind(&save_arena_rank_second, _1, connindex, target_new_data);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_arena_rank_first(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_arena_rank_change request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		save_arena_rank_first_lua(connindex, 
			request.target_new_data().rank_pos(),
			request.target_new_data().role_guid(),
			request.target_new_data().role_name(),
			request.target_new_data().gs_value(),
			request.target_new_data().role_template_id(),
			request.target_new_data().robot_template_id(),
			request.target_new_data().military_rank_level(),
			request.target_new_data().vip_level(),
			request.target_new_data().game_channel(),
			request.my_new_data().rank_pos(),
			request.my_new_data().role_guid(),
			request.my_new_data().role_name(),
			request.my_new_data().gs_value(),
			request.my_new_data().role_template_id(),
			request.my_new_data().robot_template_id(),
			request.my_new_data().military_rank_level(),
			request.my_new_data().vip_level(),
			request.my_new_data().game_channel());
	}

	void save_arena_rank_second(db_result_type result, uint32 connindex, const s_arena_rank target_new_info)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		
		if (target_new_info.rank_pos >= 0
			&& target_new_info.rank_pos < rank_max)
		{
			sql << _XTEXT("call sp_rank_arena_save_one(");

			sql << target_new_info.role_guid.server_64 << ",'";

			sql << "(" << target_new_info.rank_pos << ","
				<< target_new_info.role_guid.server_64 << ","
				<< "\\'" << target_new_info.role_name << "\\'" << ","
				<< target_new_info.gs_value << ","
				<< target_new_info.role_template_id << ","
				<< target_new_info.robot_template_id << ","
				<< target_new_info.military_rank_level << ","
				<< target_new_info.vip_level << ",\\'"
				<< target_new_info.game_channel << "\\')";
			sql << "');";
		}
		else
		{
			sql << _XTEXT("call sp_rank_arena_del_one(") << target_new_info.role_guid.server_64 << ");";
		}

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.logic_id = e_data_flag_type_role_ranking;

		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_arena_rank(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		//server2dp_proto_test_ws2dp_load_arena_rank load_data;
		////packet_s2s* load_data_ptr = parse_msg::getInstance().parse_message_server(&load_data, data_ptr, data_len);
		////if (nullptr == load_data_ptr)
		////{
		////	return;
		////}
		//parse_msg::getInstance().parse_message_new(&load_data, data_ptr, data_len);
		//int32 test_guid = load_data.role_guid();
		//CONSOLE_INFO("main(): testguid =====" << test_guid));
		//do_load_arena_rank(test_guid, 0);
		do_load_arena_rank(connindex, 0);
	}

	void do_load_arena_rank(uint32 connindex, int32 start_index)
	{
		int32 load_num = rank_save_per_time_max;
		if (rank_max - start_index < load_num)
		{
			load_num = rank_max - start_index;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_rank_arena_load(" << start_index << "," << load_num << ");");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_arena_rank_end, _1, connindex, start_index);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_arena_rank_end(db_result_type result, uint32 connindex, int32 old_start_from)
	{
		int32 table_len = result.query.data_select.row_count;
		if (table_len == 0)
		{
			if (old_start_from == 0)
			{
				//0行就是要创建机器人
				//dp2ws_create_robot create_msg;
				//dbproxy_service::getInstance().send_message(connindex, &create_msg, sizeof(dp2ws_create_robot));
				//return;
				dp2s_proto_dp2ws_create_robot msg;
				msg.set_is_create(0);
				dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_create_robot);
				return;
			}
			else
			{
				//load完了
				dp2s_proto_dp2ws_arena_load_finish msg;
				dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_load_finish);
				return;
			}
		}

		int32 data_size = sizeof(s_arena_rank);
		int32 temp_size = result.query.data_select.row_size;
		if (result.error || table_len > rank_save_per_time_max || data_size != temp_size)
		{
			return;
		}

		size_t db_reult_stream_size = result.query.data_select.row_count * result.query.data_select.row_size;

		dp2ws_load_arena_rank msg;
		msg.start_index = old_start_from;
		msg.data_num = table_len;
		//可能不为rank_max

		db_read_data(&result, msg.load_data, db_reult_stream_size);
		//dbproxy_service::getInstance().send_message(connindex, &msg, msg.get_pak_length());

		dp2s_proto_dp2ws_load_arena_rank all_msg;
		all_msg.set_start_index(old_start_from);
		dp2s_proto_s_arena_rank_all* rank_all_msg = all_msg.mutable_load_data_all();
		if (nullptr == rank_all_msg)
		{
			return;
		}
		rank_all_msg->set_row_count(table_len);
		for (int32 i = 0; i < table_len; i++)
		{
			dp2s_proto_s_arena_rank* rank_msg = rank_all_msg->add_load_data();
			if (nullptr == rank_msg || false == msg.load_data[i].role_guid.is_valid())
			{
				continue;
			}
			rank_msg->set_rank_pos(msg.load_data[i].rank_pos);
			rank_msg->set_role_guid(msg.load_data[i].role_guid.server_64);
			rank_msg->set_role_name(msg.load_data[i].role_name);
			rank_msg->set_gs_value(msg.load_data[i].gs_value);
			rank_msg->set_role_template_id(msg.load_data[i].role_template_id);
			rank_msg->set_robot_template_id(msg.load_data[i].robot_template_id);
			rank_msg->set_military_rank_level(msg.load_data[i].military_rank_level);
			rank_msg->set_vip_level(msg.load_data[i].vip_level);
			rank_msg->set_game_channel(msg.load_data[i].game_channel);
		}
		dbproxy_service::getInstance().send_message(connindex, &all_msg, e_msg_dp2ws_load_arena_rank);

		int32 new_start_index = old_start_from + table_len;
		do_load_arena_rank(connindex, new_start_index);
	}

	void load_all_ranking_info_lua(uint32 connindex, uint32 ranking_type)
	{
		load_all_ranking_type(connindex, ranking_type);
	}

	void load_all_ranking_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_all_ranking_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_all_ranking_info_lua(connindex, request.ranking_type());
		
	}
	void load_all_ranking_type(uint32 connindex, int32 ranking_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		if (ranking_type != e_RankingIndex_arena)
		{
			sql << _XTEXT("call sp_ranking_info_load(");
			sql << ranking_type << ","
				<< TopMaxNum << _XTEXT(");");
		}
		else
		{
			sql << _XTEXT("call sp_ranking_arena_info_load(");
			sql << ranking_type << ","
				<< TopMaxNum << _XTEXT(");");
		}

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_ranking_info_end, _1, connindex, ranking_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_all_ranking_info_end(db_result_type result, uint32 connindex, int32 ranking_type)
	{
		s_ranking_info_db sql_data[TopMaxNum];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_ranking_info_db);
		
		if (result.error || table_len > TopMaxNum || result.query.data_select.row_size != data_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2s_proto_dp2ws_load_all_ranking_info msg;
			dp2s_proto_s_ranking_top_info* all_rank_msg = msg.mutable_data_info();
			if (nullptr == all_rank_msg)
			{
				return;
			}
			msg.set_ranking_type(ranking_type);
			all_rank_msg->set_data_num(table_len);

			for (int32 i = 0; i < table_len; i++)
			{
				dp2s_proto_s_ranking_player_info* player_info = all_rank_msg->add_data_ary();
				if (nullptr == player_info)
				{
					break;
				}
				player_info->set_role_guid(sql_data[i].role_guid.server_64);
				player_info->set_role_name(sql_data[i].role_name);
				player_info->set_ranking_type(sql_data[i].ranking_type);
				player_info->set_ranking_value(sql_data[i].ranking_value);
				player_info->set_save_time(sql_data[i].time);
				player_info->set_supporting_guid(sql_data[i].supporting_guid.server_64);
				player_info->set_supporting_name(sql_data[i].supporting_name);
				player_info->set_vip_level(sql_data[i].vip_level);
				player_info->set_peak(sql_data[i].Peak);
				player_info->set_server_id(sql_data[i].server_id);
				player_info->set_game_channel(sql_data[i].game_channel);
				player_info->set_standby_parameter(sql_data[i].standby_parameter);
			}

			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_load_all_ranking_info);
			return;
		}
	}

	void save_ranking_info_one_lua(uint32 connindex, int64 role_guid, xstring role_name, int64 ranking_type, int64 ranking_value,
		int64 save_time, int64 supporting_guid, xstring supporting_name, int32 vip_level, int32 peak, int32 server_id, xstring game_channel, int64 standby_parameter)
	{
		xchar	t_role_name[max_name_size + 1];
		memset(t_role_name, 0, sizeof(t_role_name));
		memcpy(t_role_name, role_name.c_str(), role_name.size() > sizeof(t_role_name) ? sizeof(t_role_name) : role_name.size());

		xchar	t_supporting_name[max_name_size + 1];
		memset(t_supporting_name, 0, sizeof(t_supporting_name));
		memcpy(t_supporting_name, supporting_name.c_str(), supporting_name.size() > sizeof(t_supporting_name) ? sizeof(t_supporting_name) : supporting_name.size());
		
		xchar	t_game_channel[e_login_fixed_data_lenght + 1];
		memset(t_game_channel, 0, sizeof(t_game_channel));
		memcpy(t_game_channel, game_channel.c_str(), game_channel.size() > sizeof(t_game_channel) ? sizeof(t_game_channel) : game_channel.size());


		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid) << _XTEXT(".sp_role_ranking_save(")
			<< role_guid << ",'"
			<< t_role_name << "',"
			<< ranking_type << ","
			<< ranking_value << ","
			<< save_time << ","
			<< supporting_guid << ",'"
			<< t_supporting_name << "',"
			<< vip_level << ","
			<< peak << ","
			<< server_id << ",'"
			<< t_game_channel << "',"
			<< standby_parameter
			<< ");";
		db_query_type query;
		query.logic_id = e_data_flag_type_role_ranking;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_ranking_info_one_end, _1, connindex, (guid_64)role_guid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_ranking_info_one(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_ranking_info_one request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		server2dp_proto_s_ranking_player_info msg = request.ranking_info();
		save_ranking_info_one_lua(connindex, msg.role_guid(), msg.role_name(), msg.ranking_type(), msg.ranking_value(), msg.save_time(), msg.supporting_guid(),
			msg.supporting_name(), msg.vip_level(), msg.peak(), msg.server_id(), msg.game_channel(), msg.standby_parameter());
	}

	void save_ranking_info_one_end(db_result_type result, uint32 connindex, const guid_64& target_guid)
	{
	}

	void save_one_ranking_list(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const hld::ws2dp_save_one_total_ranking_info* pdata = static_cast<const hld::ws2dp_save_one_total_ranking_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (pdata->data_num < 0)
			return;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_role_ranking_save_one_list('") << pdata->data_type << "','";
		for (int32 index = 0; index < pdata->data_num; index++)
		{
			int64 test_info = pdata->data_info.data_ary[index].ranking_value;

			sql << "(";
			sql << pdata->data_info.data_ary[index].role_guid.server_64 << ",\\\'"
				<< pdata->data_info.data_ary[index].role_name << "\\\',"
				<< pdata->data_info.data_ary[index].ranking_type << ","
				<< test_info << ","
				<< pdata->data_info.data_ary[index].save_time << ","
				<< pdata->data_info.data_ary[index].supporting_guid.server_64 << ",\\\'"
				<< pdata->data_info.data_ary[index].supporting_name << "\\\',"
				<< pdata->data_info.data_ary[index].vip_level << ","
				<< pdata->data_info.data_ary[index].Peak << ","
				<< pdata->data_info.data_ary[index].server_id << ",\\\'"
				<< pdata->data_info.data_ary[index].game_channel << "\\\',"
				<< pdata->data_info.data_ary[index].standby_parameter;
			if (index + 1 >= pdata->data_num)
			{
				sql << ")";
			}
			else
			{
				sql << "),";
			} 
		}
		sql << "');"; 

		db_query_type query;
		query.logic_id = e_data_flag_type_role_ranking;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_one_ranking_list_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void save_one_ranking_list_end(db_result_type result, uint32 connindex)
	{

	}

	void save_one_service_rank_list(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		const hld::ws2dp_save_one_service_rank_info* pdata = static_cast<const hld::ws2dp_save_one_service_rank_info*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}

		if (pdata->data_num <= 0)
			return;
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_service_ranking_save_one_list('") << pdata->data_info.data_ary[0].rank_type << "','";
		for (int32 index = 0; index < pdata->data_num; index++)
		{
			sql << "(";
			sql << pdata->data_info.data_ary[index].role_guid.server_64 << ","
				<< pdata->data_info.data_ary[index].rank_type << ","
				<< pdata->data_info.data_ary[index].role_num;

			if (index + 1 >= pdata->data_num)
			{
				sql << ")";
			}
			else
			{
				sql << "),";
			}
		}
		sql << "');";

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_one_service_rank_list_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_one_service_rank_list_end(db_result_type result, uint32 connindex)
	{

	}

	void load_all_service_rank_info_lua(uint32 connindex, uint32 rank_type)
	{
		load_all_service_rank_type(connindex, rank_type);
	}


	void load_all_service_rank_info(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_load_all_service_rank_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_all_service_rank_info_lua(connindex, request.rank_type());
	}
	void load_all_service_rank_type(uint32 connindex, int32 rank_type)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_role_service_rank_info_load(");
		sql << rank_type << ","
			<< ShowTopMaxNum << _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_all_service_rank_info_end, _1, connindex);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_all_service_rank_info_end(db_result_type result, uint32 connindex)
	{
		s_service_rank_player_info sql_data[ShowTopMaxNum];
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_service_rank_player_info);

		if (result.error || table_len > ShowTopMaxNum || table_len == 0 || result.query.data_select.row_size != data_size)
		{
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			dp2s_proto_dp2ws_load_all_service_rank_info all_msg;
			dp2s_proto_s_service_rank_info* msg = all_msg.mutable_all_data();

			msg->set_data_num(table_len);
			for (int32 i = 0; i < table_len; i++)
			{
				dp2s_proto_s_service_rank_player_info* data_ary_one = msg->add_data_ary();
				data_ary_one->set_role_guid(sql_data[i].role_guid.server_64);
				data_ary_one->set_rank_type(sql_data[i].rank_type);
				data_ary_one->set_role_num(sql_data[i].role_num);
			}

			dbproxy_service::getInstance().send_message(connindex, &all_msg, e_msg_dp2ws_load_all_service_rank_info);
			return;
		}
	}

	void save_ranking_worship_lua(uint32 connindex, int64 sender_guid, int64 role_guid, xstring role_name, int64 ranking_type, int64 ranking_value, int64 save_time, int64 supporting_guid, xstring supporting_name, int32 vip_level, int32 peak, int32 server_id, xstring game_channel, int64 standby_parameter, int64 worship_add_value, int32 worship_type)
	{
		xchar	t_role_name[max_name_size + 1];
		memset(t_role_name, 0, sizeof(t_role_name));
		memcpy(t_role_name, role_name.c_str(), role_name.size() > sizeof(t_role_name) ? sizeof(t_role_name) : role_name.size());

		xchar	t_supporting_name[max_name_size + 1];
		memset(t_supporting_name, 0, sizeof(t_supporting_name));
		memcpy(t_supporting_name, supporting_name.c_str(), supporting_name.size() > sizeof(t_supporting_name) ? sizeof(t_supporting_name) : supporting_name.size());

		xchar	t_game_channel[e_login_fixed_data_lenght + 1];
		memset(t_game_channel, 0, sizeof(t_game_channel));
		memcpy(t_game_channel, game_channel.c_str(), game_channel.size() > sizeof(t_game_channel) ? sizeof(t_game_channel) : game_channel.size());

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		if (ranking_type == e_RankingIndex_legion_core
			|| (ranking_type >= e_RankingIndex_legion_territory &&
				ranking_type <= e_RankingIndex_world_elite_eight))
		{
			sql << _XTEXT("call sp_rank_worship_player_add(")
				<< supporting_guid << ",'"
				<< t_supporting_name << "',"
				<< e_RankingIndex_worship << ","
				<< worship_add_value << ","
				<< save_time << ","
				<< role_guid << ",'"
				<< t_role_name << "',"
				<< vip_level << ","
				<< peak << ","
				<< server_id << ",'"
				<< t_game_channel << "',"
				<< standby_parameter
				<< ");";
		}
		else
		{
			sql << _XTEXT("call sp_rank_worship_player_add(")
				<< role_guid << ",'"
				<< t_role_name << "',"
				<< e_RankingIndex_worship << ","
				<< worship_add_value << ","
				<< save_time << ","
				<< supporting_guid << ",'"
				<< t_supporting_name << "',"
				<< vip_level << ","
				<< peak << ","
				<< server_id << ",'"
				<< t_game_channel << "',"
				<< standby_parameter
				<< ");";
		}


		db_query_type query;
		query.logic_id = e_data_flag_type_role_ranking;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&save_ranking_worship_end, _1, connindex, (guid_64)sender_guid, ranking_type, worship_add_value, worship_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void save_ranking_worship(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_save_ranking_worship request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const server2dp_proto_s_ranking_player_info& msg = request.target_data();
		save_ranking_worship_lua(connindex, request.sender_guid(), msg.role_guid(), msg.role_name(), msg.ranking_type(), msg.ranking_value(), msg.save_time(), msg.supporting_guid(),
			msg.supporting_name(), msg.vip_level(), msg.peak(), msg.server_id(), msg.game_channel(), msg.standby_parameter(), request.worship_add_value(), request.worship_type());

	}

	void save_ranking_worship_end(db_result_type result, uint32 connindex,const guid_64& sender_guid, int32  from_ranking_type, int32 worship_add_value, int32 worship_type)
	{
		s_ranking_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_ranking_info_db);

		if (result.error || table_len > 1 || table_len == 0 || result.query.data_select.row_size != data_size)
		{
			//dp2ws_load_one_ranking_info reply_data;
			//reply_data.sender_guid = sender_guid;
			//reply_data.ranking_type = from_ranking_type;
			//reply_data.worship_add_value = worship_add_value;
			//reply_data.worship_type = worship_type;
			//reply_data.end_result = EWorshipEndType_unknown;
			//reply_data.data_info.set_data(sql_data);
			//dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));

			dp2s_proto_dp2ws_load_one_ranking_info msg;
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_ranking_type(from_ranking_type);
			msg.set_worship_add_value(worship_add_value);
			msg.set_worship_type(worship_type);
			msg.set_end_result((int32)EWorshipEndType_unknown);
			dp2s_proto_s_ranking_player_info* player_info = msg.mutable_data_info();
			if (nullptr == player_info)
			{
				return;
			}
			player_info->set_role_guid(sql_data.role_guid.server_64);
			player_info->set_role_name(sql_data.role_name);
			player_info->set_ranking_type(sql_data.ranking_type);
			player_info->set_ranking_value(sql_data.ranking_value);
			player_info->set_save_time(sql_data.time);
			player_info->set_supporting_guid(sql_data.supporting_guid.server_64);
			player_info->set_supporting_name(sql_data.supporting_name);
			player_info->set_vip_level(sql_data.vip_level);
			player_info->set_peak(sql_data.Peak);
			player_info->set_server_id(sql_data.server_id);
			player_info->set_game_channel(sql_data.game_channel);
			player_info->set_standby_parameter(sql_data.standby_parameter);
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_load_ranking_info_one);
		}
		else
		{
			memset(&sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			//dp2ws_load_one_ranking_info reply_data;
			//reply_data.sender_guid = sender_guid;
			//reply_data.ranking_type = from_ranking_type;
			//reply_data.worship_add_value = worship_add_value;
			//reply_data.worship_type = worship_type;
			//reply_data.end_result = EWorshipEndType_success;
			//reply_data.data_info.set_data(sql_data);
			//dbproxy_service::getInstance().send_message(connindex, &reply_data, sizeof(reply_data));

			dp2s_proto_dp2ws_load_one_ranking_info msg;
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_ranking_type(from_ranking_type);
			msg.set_worship_add_value(worship_add_value);
			msg.set_worship_type(worship_type);
			msg.set_end_result((int32)EWorshipEndType_success);
			dp2s_proto_s_ranking_player_info* player_info = msg.mutable_data_info();
			if (nullptr == player_info)
			{
				return;
			}
			player_info->set_role_guid(sql_data.role_guid.server_64);
			player_info->set_role_name(sql_data.role_name);
			player_info->set_ranking_type(sql_data.ranking_type);
			player_info->set_ranking_value(sql_data.ranking_value);
			player_info->set_save_time(sql_data.time);
			player_info->set_supporting_guid(sql_data.supporting_guid.server_64);
			player_info->set_supporting_name(sql_data.supporting_name);
			player_info->set_vip_level(sql_data.vip_level);
			player_info->set_peak(sql_data.Peak);
			player_info->set_server_id(sql_data.server_id);
			player_info->set_game_channel(sql_data.game_channel);
			player_info->set_standby_parameter(sql_data.standby_parameter);
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_load_ranking_info_one);
		}
	}

	void load_player_worship_value_lua(uint32 connindex, int64 sender, int64 target)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_ranking_info_load_one(");
		sql << target << ","
			<< e_RankingIndex_worship << _XTEXT(");");
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_player_worship_value_end, _1, connindex, (guid_64)sender);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void load_player_worship_value(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		server2dp_proto_ws2dp_get_worship_value request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		load_player_worship_value_lua(connindex, request.sender(), request.target());
	}

	void load_player_worship_value_end(db_result_type result, uint32 connindex, const guid_64& sender_guid)
	{
		s_ranking_info_db sql_data;
		int32	table_len = result.query.data_select.row_count;
		int32	data_size = sizeof(s_ranking_info_db);


		dp2s_proto_dp2ws_get_worship_value_end msg;
		msg.set_sender(sender_guid.server_64);


		if (result.error || table_len > 1 || table_len == 0 || result.query.data_select.row_size != data_size)
		{
			msg.set_is_load(false);
			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_get_worship_value_end);
			return;
		}
		else
		{
			dp2s_proto_s_ranking_player_info* player_info = msg.mutable_target();
			if (nullptr == player_info)
			{
				return;
			}
			memset(&sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);
			msg.set_is_load(true);

			player_info->set_role_guid(sql_data.role_guid.server_64);
			player_info->set_role_name(sql_data.role_name);
			player_info->set_ranking_type(sql_data.ranking_type);
			player_info->set_ranking_value(sql_data.ranking_value);
			player_info->set_save_time(sql_data.time);
			player_info->set_supporting_guid(sql_data.supporting_guid.server_64);
			player_info->set_supporting_name(sql_data.supporting_name);
			player_info->set_vip_level(sql_data.vip_level);
			player_info->set_peak(sql_data.Peak);
			player_info->set_server_id(sql_data.server_id);
			player_info->set_game_channel(sql_data.game_channel);
			player_info->set_standby_parameter(sql_data.standby_parameter);

			dbproxy_service::getInstance().send_message(connindex, &msg, e_msg_dp2ws_get_worship_value_end);
			return;
		}


	}


	void add_robot_worship(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		//以下注释部分是 当竞技场排行无数据时创建机器人 ，新需求是 机器人不加入排行榜
		//const hld::ws2dp_add_robot_worship* pdata = static_cast<const hld::ws2dp_add_robot_worship*>(data_ptr);
		//if (nullptr == pdata)
		//{
		//	return;
		//}

		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);

		//int32 last_index = pdata->data_num - 1;
		//sql << _XTEXT("call sp_rank_worship_add('");

		//for (int32 i = 0; i < pdata->data_num; ++i)
		//{
		//	sql << "(";
		//	sql << pdata->data_array[i].role_guid.server_64 << ",";
		//	sql << pdata->data_array[i].worship_value << ",";
		//	sql << pdata->data_array[i].save_time;

		//	if (i != last_index)
		//	{
		//		sql << "),";
		//	}
		//	else
		//	{
		//		sql << ")";
		//	}
		//}
		//sql << "');";

		//db_query_type query;
		//query.sql_str.assign(sql.c_str(), sql.size());
		//query.handler = boost::bind(&add_robot_worship_end, _1, connindex);
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void add_robot_worship_end(db_result_type result, uint32 connindex)
	{//存完机器人的威望，一定要再load一遍，否则内存中的数据会有问题

		//以下注释部分是 当竞技场排行无数据时创建机器人 ，新需求是 机器人不加入排行榜
		//但此功能不完全，不能正确读出 机器人数据

		//sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		//sql << _XTEXT("call sp_ranking_info_load(");
		//sql << ERankingIndex_worship << _XTEXT(");");
		//db_query_type query;
		//query.sql_str.assign(sql.c_str(), sql.size());
		//query.handler = boost::bind(&load_all_ranking_info_end, _1, connindex,ERankingIndex_worship);
		//db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
}
