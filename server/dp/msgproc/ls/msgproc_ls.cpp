/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:52
	file base:	msgproc_ls
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "msgproc_ls.hpp"
#include <char_msg.hpp>
#include <item_msg.hpp>
#include <Utility/guid_gen.h>
#include "server_log.hpp"
#include "utility/random.h"
#include  "template/template_manager.h"
#include "dbproxy_service.hpp"
#include "dp/db_manager.hpp"
#include "data_manager/data_manager.hpp"
#include "parse_msg.h"
#include "md5/Base64.h"
#include "buff_def.hpp"

namespace hld
{
	void ls2dp_client_login_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const ls2dp_client_login* pdata = static_cast<const ls2dp_client_login*>(data_ptr);
		if ( NULL == pdata )
			return;
		int32 is_new_account = data_manager::get_instance().is_new_account(pdata->account);
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call sp_client_login_main_db_check('")
			<< pdata->account
			<< "', '" << pdata->password
			<< "', '" << pdata->create_ip
			<< "', '" //<< pdata->device_type
			<< "' ," << pdata->login_type
			<< ", " << pdata->login_type_plus
			<< ", " << dbproxy_service::getInstance().get_server_id()
			<< ", '" << db_manager::getInstance().get_game_db_name()
			<< "' ," << is_new_account
			<< _XTEXT(");");

		db_query_type query;
		query.sql_str.assign(sql.c_str(),sql.size());
		query.handler = boost::bind(&ls2dp_client_main_check_login_end, _1, connindex, *pdata);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ls2dp_client_main_check_login_end(db_result_type result, uint32 connindex, const ls2dp_client_login msg_data)
	{
		dp2ls_client_login reply;
		reply.client_uid = msg_data.client_uid;
		reply.login_type = msg_data.login_type;
		reply.login_type_plus = msg_data.login_type_plus;
		memcpy(reply.account, msg_data.account, sizeof(reply.account));
		memcpy(reply.server_msg, msg_data.server_msg, sizeof(reply.server_msg));
		memcpy(reply.ban_role_array, msg_data.ban_role_array, sizeof(reply.ban_role_array));
		memcpy(reply.ban_chat_array, msg_data.ban_chat_array, sizeof(reply.ban_chat_array));
		reply.age = msg_data.age;
		if (result.error)
		{
			reply.e_result = e_error_code_db_error;
			dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));
		}
		else
		{
#pragma pack(push,1)
			struct  db_result
			{
				int32				account_id;
				int32				jewel_num;
				int32				radio_host;
				int32				is_create;
				int32				online_duration;
				int32				online_time;
				int32				month_recharge_num;
				int32				month_recharge_time;
				db_result()
				{
					memset(this, 0, sizeof(*this));
				}
			};
#pragma pack (pop)
			db_result data;
			db_read_data(&result, &data, sizeof(data));
			if (data.account_id > 0)
			{
				int32 is_new_account = data_manager::get_instance().is_new_account(msg_data.account);
				if (is_new_account > 0)
				{
					if (db_manager::getInstance().is_have_this_db_name_in_map(msg_data.server_id))
					{
						data_manager::get_instance().add_data_server(msg_data.account, msg_data.server_id);						
					}
					else
					{
						data_manager::get_instance().add_data_server(msg_data.account, SERVERCONFIG->game_id);
					}
				}
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(msg_data.account) << _XTEXT(".sp_client_login('")
					<< msg_data.account
					<< "', '" << msg_data.password
					<< "', '" << msg_data.create_ip
					<< "', '" //<< pdata->device_type
					<< "' ," << msg_data.login_type
					<< ", " << msg_data.login_type_plus
					<< ", " << dbproxy_service::getInstance().get_server_id()
					<< ", '" << db_manager::getInstance().get_game_db_name()
					<< "' ," << is_new_account
					<< _XTEXT(");");

				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				query.handler = boost::bind(&ls2dp_client_login_end, _1, connindex, msg_data);
				db_manager::getInstance().get_db_link().game_db.add_query(query);
				return;
			}
			else if (data.account_id == 0)
			{
				reply.e_result = e_error_code_login_password_error;
			}
			else if (data.account_id == -1)
			{
				reply.e_result = e_error_code_login_ban;
			}
			else if (data.account_id == -2)
			{
				reply.e_result = e_error_code_login_login_full;
			}

			dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));
		}
	}

	void ls2dp_client_login_end(db_result_type result, uint32 connindex, const ls2dp_client_login msg_data)
	{
		dp2ls_client_login reply;
		reply.client_uid = msg_data.client_uid;
		reply.login_type = msg_data.login_type;
		reply.login_type_plus = msg_data.login_type_plus;
		memcpy(reply.account, msg_data.account, sizeof(reply.account));
		memcpy(reply.server_msg, msg_data.server_msg, sizeof(reply.server_msg));
		memcpy(reply.ban_role_array, msg_data.ban_role_array, sizeof(reply.ban_role_array));
		memcpy(reply.ban_chat_array, msg_data.ban_chat_array, sizeof(reply.ban_chat_array));
		reply.age = msg_data.age;
		if (result.error)
		{
			reply.e_result= e_error_code_db_error;
			dbproxy_service::getInstance().send_message(connindex,&reply,sizeof(reply));
		}
	 	else
		{
	#pragma pack(push,1)
			struct  db_result
			{
				int32				account_id;
				int32				jewel_num;
				int32				radio_host;
				int32				is_create;
				int32				online_duration;
				int32				online_time;
				int32				month_recharge_num;
				int32				month_recharge_time;
				db_result()
				{
					memset(this,0,sizeof(*this));
				}
			};
	#pragma pack (pop)
			db_result data;

			db_read_data(&result, &data, sizeof(data));

			reply.jewel_num = data.jewel_num;
			reply.radio_host = data.radio_host;
			reply.online_duration = data.online_duration;
			reply.online_time = data.online_time;
			reply.month_recharge_num = data.month_recharge_num;
			reply.month_recharge_time = data.month_recharge_time;
			reply.is_create_new = data.is_create;
			if (data.account_id > 0)
			{
				reply.e_result = e_error_code_success;

			}
			else if (data.account_id == 0)
			{
				reply.e_result = e_error_code_login_password_error;
			}
			else if (data.account_id == -1)
			{
				reply.e_result = e_error_code_login_ban;
			}
			else if (data.account_id == -2)
			{
				reply.e_result = e_error_code_login_login_full;
			}

			dbproxy_service::getInstance().send_message(connindex,&reply,sizeof(reply));
		}
		return;
	}
	// d. zhao
	void ls2dp_req_createchar_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if( data_len != sizeof( ls2dp_create_character ) )
		{
			return;
		}
		const ls2dp_create_character* pdata = static_cast<const ls2dp_create_character*>(data_ptr);
		if (nullptr == pdata)
		{
			return;
		}
		int32 real_server_id = pdata->server_id;
		if (!db_manager::getInstance().is_have_this_db_name_in_map(real_server_id))
		{
			real_server_id = SERVERCONFIG->game_id;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_login_create_role(")
			<< pdata->init_role.role_guid.server_64 << ","
			<< "'" << pdata->init_role.role_name << "',"
			<< "'" << pdata->init_role.account << "',"
			<< "'" << pdata->create_level_requirement << "',"
			<< "'" << pdata->init_role.role_guid.server_64 << ","
			<< "\\\'" << pdata->init_role.account << "\\\',"
			<< "\\\'" << pdata->init_role.role_name << "\\\',"
			<< "\\\'" << pdata->init_role.role_appearance << "\\\',"
			<< "\\\'"<< "" << "\\\',"
			<< 0 << ",";
		int32 i64_info_len = e_role_i64_info_max;
		for (int32 i = 0; i < i64_info_len; ++i)
		{
			sql << pdata->init_role.data_i64_ary[i] << ",";
		}

		int32 info_len = e_role_info_max - 1;
		int32 msg_len = 0;
		for (int32 i = 0; i < info_len; ++i)
		{
				sql << pdata->init_role.data_ary[i] << ",";
		}
		sql << pdata->init_role.data_ary[info_len] << "',";
		//info_len = e_item_info_max - 1;
		msg_len = pdata->item_num - 1;
		sql << "'";
		for (int32 i = 0; i < pdata->item_num; i++)
		{
			sql << "("
				<<pdata->init_role.role_guid.server_64 << ","
				<<pdata->init_item[i].item_guid.server_64 << ",";

			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				sql << pdata->init_item[i].data_ary[j] << ",";
			}
			sql << "\\\'" << pdata->init_item[i].param_str << "\\\'";
			//sql << pdata->init_item[i].data_ary[info_len];
			if (i != msg_len)
			{
				sql << "),";
			}
			else
			{
				sql << ")";
			}
		}
		sql << "','";
		info_len = e_skill_info_max - 1;
		msg_len = pdata->skill_num - 1;
		for (int32 i = 0; i < pdata->skill_num; i++)
		{
			sql << "("
				<< pdata->init_role.role_guid.server_64 << ",";
			for (int32 j = 0; j < info_len; ++j)
			{
				sql << pdata->init_skill[i].data_ary[j] << ",";
			}
			sql << pdata->init_skill[i].data_ary[info_len];
			if (i != msg_len)
			{
				sql << "),";
			}
			else
			{
				sql << ")";
			}
		}

		sql << "','";
		info_len = pdata->record_num - 1;
		for (int32 i = 0; i < pdata->record_num; i++)
		{
			sql << "("
				<< pdata->init_role.role_guid.server_64 << ",";
			sql << pdata->record_info_list[i].guide_trigger_id << ",";
			sql << pdata->record_info_list[i].state;
			if (i != info_len)
			{
				sql << "),";
			}
			else
			{
				sql << ")";
			}
		}

		sql << "')";

		db_query_type query;
		query.sql_str.assign(sql.c_str(),sql.size());
		query.handler = boost::bind(&ls2dp_req_createchar_end, _1, pdata->init_role.role_guid, connindex, pdata->client_uid, pdata->init_role, pdata->login_proto_msg, pdata->login_msg_len, real_server_id);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ls2dp_req_createchar_end(db_result_type result,guid_64 charid,uint32 connindex, s_client_uid client_uid, s_unit_info role_info, const ui8* login_proto_msg, int32 login_msg_len, int32 server_id)
	{
		if (!result.error && result.query.data_select.row_count == 1 )
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
			if ( -1 == nres )
			{
				// 数据库操作错误
				dp2ls_create_character reply;
				reply.eresult = e_failed_internal_error;
				reply.client_uid = client_uid;
				dbproxy_service::getInstance().send_message( connindex, &reply, sizeof( reply )) ;

				return;
			}
			else if ( 0 == nres)
			{
				// 创建角色失败
				dp2ls_create_character reply;
				reply.eresult = e_failed_create_error;
				reply.client_uid = client_uid;
				dbproxy_service::getInstance().send_message( connindex, &reply, sizeof( reply ) );

				return;
			}
			else if (nres == -2)
			{
				// 角色名重复
				dp2ls_create_character reply;
				reply.eresult = e_failed_role_name_duplicate;
				reply.client_uid = client_uid;
				dbproxy_service::getInstance().send_message( connindex, &reply, sizeof( reply ) );

				return;
			}
			else if (nres == -3)
			{
				// 角色过多
				dp2ls_create_character reply;
				reply.eresult = e_failed_role_num_max;
				reply.client_uid = client_uid;
				dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));

				return;
			}
			else if (nres == -4)
			{
				// 没有达到创建等级要求
				dp2ls_create_character reply;
				reply.eresult = e_failed_role_create_level_limit;
				reply.client_uid = client_uid;
				dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));
			}
			else
			{
				dp2ls_create_character reply;
				reply.eresult = e_create_role_success;
				reply.role_guid = charid;
				reply.client_uid = client_uid;
				reply.role_info = role_info;
				memcpy(reply.login_proto_msg, login_proto_msg, login_msg_len);
				reply.login_msg_len = login_msg_len;
				dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));
				data_manager::get_instance().add_data_server(role_info.role_guid.server_64, server_id);
			}
		}
		else
		{
			dp2ls_create_character reply;
			reply.eresult=e_failed_internal_error;
			reply.client_uid = client_uid;
			dbproxy_service::getInstance().send_message(connindex,&reply,sizeof(reply));

		}
	}

	// d. zhao
	void ls2dp_req_deletechar_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if( data_len != sizeof( ls2dp_delete_character ) )
		{
			return;
		}

		const ls2dp_delete_character* pdata = static_cast<const ls2dp_delete_character*>(data_ptr);
		if ( NULL == pdata )
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_login_del_role('") << pdata->account << _XTEXT("',") << pdata->role_guid.server_64 << _XTEXT(",") << pdata->del_type << _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(),sql.size());
		query.handler = boost::bind(&ls2dp_req_deletechar_end, _1, connindex, pdata->client_uid, pdata->role_guid, pdata->del_type);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void ls2dp_req_deletechar_end(db_result_type result,uint32 connindex, s_client_uid client_uid, guid_64 role_guid, e_del_char_type del_type)
	{
		dp2ls_delete_character_end reply;
		reply.client_uid = client_uid;
		reply.role_guid = role_guid;
		reply.del_type = e_del_char_none;
		if (!result.error && result.query.data_select.row_count == 1 )
		{
	#pragma pack(push,1)
			struct _delete_char_ret
			{
				int32 del_time;		
			};
	#pragma pack(pop)

			_delete_char_ret data;
			memset(&data, 0, sizeof(data));

			if (sizeof(data) < result.query.data_select.row_count * result.query.data_select.row_size)
			{
				return;
			}

			db_read_data(&result, &data, result.query.data_select.row_count * result.query.data_select.row_size);

			reply.del_type = del_type;
			reply.del_time = data.del_time;
			dbproxy_service::getInstance().send_message(connindex, &reply, sizeof(reply));

			return;
		}
		else
		{
			reply.del_time = 0;
			dbproxy_service::getInstance().send_message(connindex,&reply,sizeof(reply));
		}
	}

	void ls2dp_req_enum_char(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if( data_len != sizeof( ls2dp_enum_character ) )
		{
			return;
		}

		const ls2dp_enum_character* pdata = static_cast<const ls2dp_enum_character*>(data_ptr);

		if ( pdata->page_index <= 0 )
		{

			dp2ls_enum_character reply;
			reply.client_uid = pdata->client_uid;
			reply.char_count = 0;
			dbproxy_service::getInstance().send_message(connindex, &reply, reply.get_pak_length());
			return;
		}

		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << it->second << _XTEXT(".sp_login_enum_role_info('")
				<< pdata->account << "',"
				<< random_gen::get_random(1, 10000000)
				<< _XTEXT(")");
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&ls2dp_req_enumchar_end, _1, connindex, pdata->client_uid, pdata->page_index, it->first, pdata->account);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void ls2dp_req_enumchar_end(db_result_type result,uint32 connindex, s_client_uid client_uid, ui16 ucurrentpage, int32 server_id, xstring account)
	{
		if (!result.error && result.query.data_select.row_count >= 0 )
		{
			s_unit_info data[hld::max_character_num];
			memset(&data, 0, sizeof(data));
			int32 data_num = max_character_num > result.query.data_select.row_count ? result.query.data_select.row_count : max_character_num;
			db_read_data(&result, &data, sizeof(s_unit_info)*data_num);
			for (int32 i = 0; i < data_num; ++i)
			{
				s_unit_info_db_check unit_info_db;
				unit_info_db.set_unit_info(data[i]);
				unit_info_db.server_id = server_id;
				data_manager::get_instance().add_unit_info_with_client_uid(account, unit_info_db);
			}		
		}
		else
		{

		}
		if (data_manager::get_instance().mark_enum_character(account))
		{
			const std::vector<s_unit_info_db_check>& unit_info_db_arr = data_manager::get_instance().get_unit_info_arr_with_client_uid(account);
			dp2ls_enum_character reply;
			reply.client_uid = client_uid;
			for (int32 i = 0; i < unit_info_db_arr.size(); i++)
			{
				reply.char_data[i].role_guid = unit_info_db_arr[i].role_guid;
				reply.char_data[i].set_role_name(unit_info_db_arr[i].role_name);
				reply.char_data[i].set_appearance(unit_info_db_arr[i].role_appearance);
				reply.char_data[i].set_role_account(unit_info_db_arr[i].account);
				reply.char_data[i].set_first_kill_npc_id_arr(unit_info_db_arr[i].role_first_kill_npc_id_arr);
				reply.char_data[i].map_guid = unit_info_db_arr[i].map_guid;
				for (int32 y = 0; y < e_role_i64_info_max; y++)
				{
					reply.char_data[i].data_i64_ary[y] = unit_info_db_arr[i].data_i64_ary[y];
				}
				for (int32 z = 0; z < e_role_info_max; z++)
				{
					reply.char_data[i].data_ary[z] = unit_info_db_arr[i].data_ary[z];
				}
				guid_64 cur_role_guid = unit_info_db_arr[i].role_guid;
				int32 cur_server_id = unit_info_db_arr[i].server_id;
				load_enum_item(cur_server_id, cur_role_guid, unit_info_db_arr[i].data_ary[e_role_info_wing_showd_template_id], unit_info_db_arr[i].data_ary[e_role_info_mount_showd_template_id], connindex, client_uid);
				load_enum_buff(cur_server_id, cur_role_guid, connindex, client_uid);
				load_enum_spirit(cur_server_id, cur_role_guid, connindex, client_uid);
				load_enum_time(cur_server_id, cur_role_guid, connindex, client_uid);
			}
			reply.char_count = unit_info_db_arr.size();
			dbproxy_service::getInstance().send_message(connindex, &reply, reply.get_pak_length());
			data_manager::get_instance().clear_all_with_client_uid(account);
		}
	}


	void load_enum_item(int32 server_id, guid_64& role_guid, int32 wing_id, int32 mount_id, uint32 connindex, s_client_uid client_uid, int32 wing_illusion_range_id, int32 mount_illusion_range_id)
	{

		if (wing_id != 0)
		{
			ItemTemplate* item_wing_template_ptr = GET_TEMPLATE(ItemTemplate, wing_id);
			if (item_wing_template_ptr != nullptr)
			{
				WingTemplate* wing_template_ptr = GET_TEMPLATE(WingTemplate, item_wing_template_ptr->logic_id);
				if (wing_template_ptr != nullptr)
				{
					if (wing_template_ptr->IllusionOriginalIdRange.size() == hld::e_wing_illusion_originalid_range_max)
					{
						ItemTemplate* item_wing_real_template_ptr = template_manager::get_instance().get_item_template_ptr_by_logic_id(wing_template_ptr->IllusionOriginalIdRange[hld::e_wing_illusion_originalid_range_low]);
						if (item_wing_real_template_ptr != nullptr)
						{
							wing_illusion_range_id = item_wing_real_template_ptr->attribute_id;
						}
					}
				}
			}
		}

		if (mount_id != 0)
		{
			ItemTemplate* item_mount_template_ptr = GET_TEMPLATE(ItemTemplate, mount_id);
			if (item_mount_template_ptr != nullptr)
			{
				MountTemplate* mount_template_ptr = GET_TEMPLATE(MountTemplate, item_mount_template_ptr->logic_id);
				if (mount_template_ptr!=nullptr)
				{
					if (mount_template_ptr->IllusionOriginalIdRange.size() == 2)
					{
						ItemTemplate* item_mount_real_template_ptr = template_manager::get_instance().get_item_template_ptr_by_logic_id(mount_template_ptr->IllusionOriginalIdRange[0]);
						if (item_mount_real_template_ptr != nullptr)
						{
							mount_illusion_range_id = item_mount_real_template_ptr->attribute_id;
						}
					}
				}
			}
		}

		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_login_enum_role_item(")
			<< role_guid.server_64 << _XTEXT(",")
			<< wing_id << _XTEXT(",")
			<< mount_id << _XTEXT(",")
			<< wing_illusion_range_id << _XTEXT(",")
			<< mount_illusion_range_id
			<< _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_enum_item_end, _1, role_guid, connindex, client_uid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void load_enum_item_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid)
	{
		s_item_info_db sql_data[role_show_item_num];
		int32	table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);

		dp2ls_load_enum_item reply_item;
		reply_item.client_uid = client_uid;
		reply_item.role_guid = role_guid;
		if (result.error || table_len > role_show_item_num || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_item, reply_item.get_pak_length());
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			reply_item.data_num = table_len;
			for (int32 i = 0; i < table_len; ++i)
			{
				memcpy(&(reply_item.item_data[i]), &(sql_data[i].data_info), sizeof(s_item_info));
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_item, reply_item.get_pak_length());
		}
	}
	void load_enum_buff(int32 server_id, guid_64& role_guid, uint32 connindex, s_client_uid client_uid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_login_enum_role_buff(")
			<< role_guid.server_64
			<< _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_enum_buff_end, _1, role_guid, connindex, client_uid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);

	}

	void load_enum_buff_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid)
	{
		s_buff_save_db sql_data[MAX_LS_ENUM_BUFF];
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_buff_save_db);

		dp2ls_load_enum_buff reply_buff;
		reply_buff.client_uid = client_uid;
		reply_buff.role_guid = role_guid;
		if (result.error || table_len > MAX_LS_ENUM_BUFF || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_buff, reply_buff.get_pak_length());
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			reply_buff.data_num = table_len * e_enum_buff_info_max;
			for (int32 i = 0; i < table_len; ++i)
			{
				reply_buff.buff_data[i * e_enum_buff_info_max + e_enum_buff_info_id] = sql_data[i].data_info.data_ary[EBuffInfo_template_id];
				reply_buff.buff_data[i * e_enum_buff_info_max + e_enum_buff_info_effect_index] = sql_data[i].data_info.data_ary[EBuffInfo_effect_index];
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_buff, reply_buff.get_pak_length());
			return;
		}
	}
	void load_enum_spirit(int32 server_id, guid_64& role_guid, uint32 connindex, s_client_uid client_uid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_login_enum_role_spirit(")
			<< role_guid.server_64
			<< _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_enum_spirit_end, _1, role_guid, connindex, client_uid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_enum_spirit_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid)
	{
		s_item_info_db sql_data[MAX_LS_ENUM_SPIRIT];
		int32 table_len = result.query.data_select.row_count;
		int32 data_size = sizeof(s_item_info_db);

		dp2ls_load_enum_spirit reply_spirit;
		reply_spirit.client_uid = client_uid;
		reply_spirit.role_guid = role_guid;
		if (result.error || table_len > MAX_LS_ENUM_SPIRIT || data_size != result.query.data_select.row_size)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_spirit, sizeof(reply_spirit));
			return;
		}
		else
		{
			memset(sql_data, 0, sizeof(sql_data));
			db_read_data(&result, &sql_data, data_size * table_len);

			reply_spirit.temp_sprite_info.item_guid = sql_data[0].data_info.item_guid;
			reply_spirit.temp_sprite_info.data_ary[e_item_info_prototype_id] = sql_data[0].data_info.data_ary[e_item_info_prototype_id];
			reply_spirit.temp_sprite_info.data_ary[e_item_info_showing_illusion_idex] = sql_data[0].data_info.data_ary[e_item_info_showing_illusion_idex];
			reply_spirit.temp_sprite_info.data_ary[e_item_info_upgrade_count] = sql_data[0].data_info.data_ary[e_item_info_upgrade_count];
			reply_spirit.temp_sprite_info.data_ary[e_item_info_stargrade_count] = sql_data[0].data_info.data_ary[e_item_info_stargrade_count];
			dbproxy_service::getInstance().send_message(connindex, &reply_spirit, sizeof(reply_spirit));
			return;
		}
	}
	void load_enum_time(int32 server_id, guid_64& role_guid, uint32 connindex, s_client_uid client_uid)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << data_manager::get_instance().get_db_name(role_guid.server_64) << _XTEXT(".sp_role_data_load(") << role_guid.server_64 << ",'" << data_manager::get_table_name(e_data_flag_time).c_str() << _XTEXT("');");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&load_enum_time_end, _1, role_guid, connindex, client_uid);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void load_enum_time_end(db_result_type result, guid_64 role_guid, uint32 connindex, s_client_uid client_uid)
	{
		dp2ls_load_enum_time reply_time;
		reply_time.client_uid = client_uid;
		reply_time.role_guid = role_guid;

		int32 table_len = result.query.data_select.row_count;
		int32 table_size = result.query.data_select.row_size;
		if (result.error || table_len > 1)
		{
			dbproxy_service::getInstance().send_message(connindex, &reply_time, sizeof(reply_time));
			return;
		}
		else
		{
			static s_db_data_array sql_data;
			sql_data.clear_data();
			db_read_data(&result, &sql_data, table_size);
			auto str_len = strlen(sql_data.data_array);

			xstring str_2 = Base64::Decode(sql_data.data_array, str_len);
			character_s2s_sl_time_data msg;
			msg.ParseFromArray(str_2.c_str(), str_2.size());
			if (msg.data_ary_size() > e_time_type_logout_time)
			{
				reply_time.login_out_time = msg.data_ary(e_time_type_logout_time);
			}
			dbproxy_service::getInstance().send_message(connindex, &reply_time, sizeof(reply_time));
		}
	}
}
