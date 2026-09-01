/********************************************************************
	file name:	relation.cpp
	created:	2017/05/26 13:49
	author:		zhy
	purpose:	
*********************************************************************/
#include "world_server.hpp"
#include "relation_mgr.h"
#include "../client_session.hpp"
#include "../client_session_mgr.hpp"
#include <char_msg.hpp>
#include <chat_msg.hpp>
#include "template/template_manager.h"
#include "time.hpp"
#include "ws_client.hpp"
#include "utility/init_unit.h"
#include "../ranking/ranking_mgr_ws.h"
#include "server/mail/event_ws_mgr.h"
#include "server/mail/mail_event_ws.h"
#include "utility/guid_gen.h"
#include "Logic/chat_def.hpp"
#include "server_log.hpp"

#include "cross_group/cross_transfer_logic.hpp"
#include "globle_data.h"
#include "relation.pb.h"
#include "chat.pb.h"
#include "net.pb.h"

namespace faith
{
//=========== relation_mgr : code here =========================================

	relation_mgr::relation_mgr(client_session* client_session_ptr)
	{
		m_client_session_ptr = client_session_ptr;
		clear_data();
	}

	relation_mgr::~relation_mgr()
	{
	}

	void relation_mgr::clear_data()
	{
		for (int32 listtype = 0; listtype < e_relationlist_type_max; listtype++)
		{
			relation_set* relation_list_ptr = get_relation_list((e_relationlist_type)listtype);
			if (nullptr == relation_list_ptr)
			{
				continue;
			}
			relation_list_ptr->set_client_session(m_client_session_ptr);
			relation_list_ptr->clear_data();
			relation_list_ptr->set_relation_type(listtype);
		}
		m_marriage.clean_up_data();
		m_last_update_friend_user_info_time = 0;
		m_last_find_friend_user_info_time = 0;
		m_load_data = false;
	}

	void relation_mgr::clear_relation_list()
	{
		for (int32 listtype = 0; listtype < e_relationlist_type_max; listtype++)
		{
			relation_set* relation_list_ptr = get_relation_list((e_relationlist_type)listtype);
			if (nullptr == relation_list_ptr)
			{
				continue;
			}
			relation_list_ptr->clear_data();
			relation_list_ptr->set_relation_type(listtype);
		}
	}


	void	relation_mgr::send_operate_result_to_client(e_relation_operate_result result_id)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		relation_proto_s2c_ret_operate_relation_result pak;
		pak.set_result_id(result_id);
		m_client_session_ptr->send_to_client(&pak, e_msgindex_s2c_ret_operate_relation_result);
	}
	void relation_mgr::add_chat(client_session* client_session_ptr)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (nullptr == client_session_ptr)
		{
			return;
		}
		relation_set& chat_relation_ref = m_releation_list[e_relationlist_type_chat];
		if (chat_relation_ref.get_relation_num() > FRIEND_CHAT_NUM)
		{
			chat_relation_ref.get_relation_list().pop_back();
		}
		add_relation(e_relationlist_type_chat, client_session_ptr, false, false);
	}

	void relation_mgr::recv_chat(guid_64 send_guid)
	{
		relation_set* relation_set_ptr = get_relation_list(e_relationlist_type_chat);
		if (nullptr == relation_set_ptr)
		{
			return;
		}

		if (nullptr == relation_set_ptr->get_relation(send_guid))
		{
			return;
		}

		relation relation_info;
		relation_list& relation_list_temp = relation_set_ptr->get_relation_list();
		for (auto iter = relation_list_temp.begin(); iter != relation_list_temp.end();)
		{
			relation relation_temp = *iter;
			if (relation_temp.get_data_guid() == send_guid)
			{
				s_relation_info relation_info_temp = relation_temp.get_data_all();
				relation_info.set_data_guid(send_guid);
				relation_info.set_data_name(relation_temp.get_data_name());
				for (int32 i = 0; i < e_relation_data_max; ++i)
				{
					relation_info.set_data_value((e_relation_data)i, relation_info_temp.data_array[i]);
				}

				if (iter == relation_list_temp.begin())
				{
					return;
				}
				else
				{
					relation_list_temp.erase(iter);
					relation_list_temp.push_front(relation_info);
					return;
				}
			}
			else
			{
				++iter;
			}
		}

	}
	void relation_mgr::add_relation(e_relationlist_type relation_type, client_session* client_session_ptr, bool send_client, bool remove_other)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (nullptr == client_session_ptr)
		{
			return;
		}
		if (is_relation_full(relation_type))
		{
			if (send_client)
			{
				send_operate_result_to_client(e_relation_operate_result_friend_list_full);
			}
			return;
		}

		if (is_relation_exist(relation_type, client_session_ptr->get_role_guid()))
		{
			if (send_client)
			{
				send_operate_result_to_client(e_relation_operate_result_friend_exist);
			}
			return;
		}

		s_relation_info relation_info;
		relation_info.relation_guid = client_session_ptr->get_role_guid();
		memcpy(relation_info.relation_name, client_session_ptr->m_role_info.role_name, max_name_size);
		relation_info.gs_value = client_session_ptr->get_role_gs_value();
		relation_info.data_array[e_relation_data_type] = relation_type;
		relation_info.data_array[e_relation_data_level] = client_session_ptr->m_role_info.data_ary[e_role_info_exp_level];
		relation_info.data_array[e_relation_data_template_id] = client_session_ptr->m_role_info.data_ary[e_role_info_template_id];
		relation_info.data_array[e_relation_data_relation_state] = e_relation_online;
		relation_info.data_array[e_relation_data_last_login_time] = client_session_ptr->m_logintime;
		relation_info.data_array[e_relation_data_friendliness] = 0;
		relation_info.data_array[e_relation_data_vip_title_template_id] = get_vip_title_template_id(client_session_ptr);

		if (relation_info.data_array[e_relation_data_type] == e_relationlist_type_chat || relation_info.data_array[e_relation_data_type] == e_relationlist_type_other_req_add_lucky)
		{
			relation_set* relation_set_ptr = get_relation_list(e_relationlist_type_friend);
			if (nullptr != relation_set_ptr)
			{
				relation* relation_ptr = relation_set_ptr->get_relation(client_session_ptr->get_role_guid());
				if (nullptr != relation_ptr)
				{
					relation_info.data_array[e_relation_data_friendliness] += relation_ptr->get_data_all().data_array[e_relation_data_friendliness];
				}	
			}
		}

		if (relation_info.data_array[e_relation_data_type] == e_relationlist_type_chat)
		{
			remove_other = false;
		}

		bool req_add_friend = false;
		if (relation_info.data_array[e_relation_data_type] == e_relationlist_type_friend)
		{
			req_add_friend = true;
		}

		if (relation_info.data_array[e_relation_data_type] == e_relationlist_type_lucky)
		{
			req_add_friend = true;
		}

		bool result = add_unit_to_relation_list(relation_info, remove_other, req_add_friend);
		if (result)
		{
			if (relation_type == e_relationlist_type_black || relation_type == e_relationlist_type_hate)
			{
				client_session_ptr->get_relation_list_mgr().del_relation(m_client_session_ptr->get_role_guid());
			}
		}

		if (result && send_client)
		{
			if (relation_type == e_relationlist_type_other_req_add || relation_type == e_relationlist_type_other_req_add_lucky)
			{
				send_operate_result_to_client(e_relation_operate_result_other_add_friend);
				return;
			}
			if (req_add_friend)
			{
				send_operate_result_to_client(e_relation_operate_result_req_friend_success);
			}
			else
			{
				send_operate_result_to_client(e_relation_operate_result_add_friend_success);
			}		
		}
	}
	void relation_mgr::add_relation(e_relationlist_type relation_type, const guid_64& guid, int32 server_id)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}

		if (relation_type == e_relationlist_type_black || relation_type == e_relationlist_type_hate)
		{
			CONSOLE_INFO("add_relation guid:{} target:{} type:{}", m_client_session_ptr->get_role_guid().server_64, guid.server_64, (int32)relation_type);
		}
		if (is_relation_full(relation_type))
		{
			send_operate_result_to_client(e_relation_operate_result_friend_list_full);
			return;
		}

		if (is_relation_exist(relation_type, guid))
		{
			send_operate_result_to_client(e_relation_operate_result_friend_exist);
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(guid);
		if (client_session_ptr)
		{
			if (false == client_session_ptr->is_self_server() && e_relationlist_type_black != relation_type)
			{
				send_operate_result_to_client(e_relation_operate_result_other_is_cross_server);
				return;
			}
			add_relation(relation_type, client_session_ptr, true, true);
		}
		else
		{
			if (server_id > 0 && e_relationlist_type_black == relation_type)
			{
				ws2ws_load_offline_other_relation ws_msg;
				ws_msg.sender_guid = m_client_session_ptr->get_role_guid();
				ws_msg.sender_server_id = m_client_session_ptr->get_role_info_data(e_role_info_server_id);
				ws_msg.list_type = e_relationlist_type_black;
				ws_msg.other_guid = guid;

				cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_load_offline_other_relation, &ws_msg, sizeof(ws_msg));
			}
			else
			{
				req_relation_info_from_db(relation_type, guid);
			}
		}

		ws2cs_req_relation_end to_cs_msg;
		to_cs_msg.self_index = m_client_session_ptr->get_cs_array_index();
		to_cs_msg.relation_type = relation_type;
		to_cs_msg.target_guid = guid;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_req_relation_end);
		if (is_use == false)
		{
			m_client_session_ptr->send_to_cs(&to_cs_msg, sizeof(ws2cs_req_relation_end));
		}
		else
		{
			faith::ws2cs_proto::req_relation_end pro_msg;
			to_cs_msg.to_proto(pro_msg);
			m_client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_req_relation_end);
		}
	}

	void relation_mgr::del_relation(const guid_64& unit_guid, e_relationlist_type type)
	{
		if (type != e_relationlist_type_friend)
		{
			del_unit_from_relation_list(type, unit_guid);
			return;
		}
		for (int32 i = 0; i < e_relationlist_type_max; ++i)
		{
			del_unit_from_relation_list((e_relationlist_type)i, unit_guid);
		}
	}
	void relation_mgr::add_ret_friend_userinfo(relation_proto_s2c_ret_friend_userinfo& pak, client_session* add_session)
	{
		if (nullptr == add_session)
		{
			return;
		}
		relation_proto_friend_user_info* user_info = pak.add_friend_list();
		if (nullptr == user_info)
		{
			return;
		}
		user_info->set_guid(add_session->get_role_guid().server_64);
		user_info->set_name(add_session->m_role_info.role_name);
		user_info->set_gs_value(init_unit::change_i64_to_string(add_session->get_role_gs_value()));
		user_info->add_data_array(0);
		user_info->add_data_array(add_session->m_role_info.data_ary[e_role_info_exp_level]);
		user_info->add_data_array(add_session->m_role_info.data_ary[e_role_info_template_id]);
		user_info->add_data_array(add_session->get_role_gs_value());
		user_info->add_data_array(e_relation_online);				
		user_info->add_data_array(add_session->m_logintime);
		user_info->add_data_array(0);
	}


	void relation_mgr::find_player_to_add_friend()
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (false == m_client_session_ptr->is_vaild())
		{
			return;
		}
		
		int64 now = utility::get_tick_count();
		if (get_last_find_user_time() != 0 && ((now - get_last_find_user_time())<= FIND_PLAYER_INTERVAL))
		{
			return;
		}
		set_last_find_user_time(now);
		int32 level_limit = GAMECONFIG->PushFriendLevelLimit;
		relation_proto_s2c_ret_friend_userinfo push_pak;
		push_pak.set_is_search(false);
		static int32 begin_index = 0;
		bool return_begin = false;

		for (int32 push_index = begin_index; push_index < init_session_max; push_index++)
		{
			if (push_pak.friend_list_size() >= RECOMMENDED_PLAYER_NUM)
			{
				begin_index = push_index;
				break;
			}
			client_session* push_session = client_session_mgr::getInstance().get_session_by_array_index(push_index);
			if (nullptr == push_session || push_session->is_vaild() == false)
			{
				if (push_index >= init_session_max - 1 && return_begin == false)
				{
					push_index = -1;
					return_begin = true;
				}

				continue;
			}
			if (push_session->get_role_guid() == m_client_session_ptr->get_role_guid())
			{
				if (push_index >= init_session_max - 1 && return_begin == false)
				{
					push_index = -1;
					return_begin = true;
				}
				continue;

			}
			//if level is low
			if (push_session->m_role_info.data_ary[e_role_info_exp_level] < level_limit)
			{
				if (push_index >= init_session_max - 1 && return_begin == false)
				{
					push_index = -1;
					return_begin = true;
				}
				continue;
			}
			//Is relation
			if (is_relation_exist(push_session->get_role_guid()))
			{
				if (push_index >= init_session_max - 1 && return_begin == false)
				{
					push_index = -1;
					return_begin = true;
				}
				continue;
			}
			add_ret_friend_userinfo(push_pak, push_session);
			if (push_index >= init_session_max - 1 && return_begin == false)
			{
				push_index = -1;
				return_begin = true;
			}
		}
		//push relation to client
		m_client_session_ptr->send_to_client(&push_pak, e_msgindex_s2c_ret_friend_userinfo);
	}


	void relation_mgr::find_player_to_add_friend_by_name(const xstring& szname)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (false == m_client_session_ptr->is_vaild())
		{
			return;
		}

		int64 now = utility::get_tick_count();
		if (get_last_find_user_time() != 0 && ((now - get_last_find_user_time()) <= FIND_PLAYER_INTERVAL ))
		{
			send_operate_result_to_client(e_relation_operate_result_too_fast);
			return;
		}
		set_last_find_user_time(now);

		//search the relation in client session mgr
		relation_proto_s2c_ret_friend_userinfo pak;
		pak.set_is_search(true);
		client_session* target_seesion = client_session_mgr::getInstance().get_session_by_role_name(szname);
		if (nullptr != target_seesion)
		{
			add_ret_friend_userinfo(pak, target_seesion);
		}
		m_client_session_ptr->send_to_client(&pak, e_msgindex_s2c_ret_friend_userinfo);
	}

	void relation_mgr::find_player_to_add_friend_by_pattern(const xstring & pattern)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (false == m_client_session_ptr->is_vaild())
		{
			return;
		}

		int64 now = utility::get_tick_count();
		if (get_last_find_user_time() != 0 && ((now - get_last_find_user_time()) <= FIND_PLAYER_INTERVAL))
		{
			send_operate_result_to_client(e_relation_operate_result_too_fast);
			return;
		}
		set_last_find_user_time(now);
		int32 level_limit = GAMECONFIG->PushFriendLevelLimit;

		int32 use_session_num = client_session_mgr::getInstance().get_session_array_used();

		relation_proto_s2c_ret_friend_userinfo search_pak;
		search_pak.set_is_search(true);


		int32 get_count = 0;

		for (int32 idx = 0; idx <= use_session_num; idx++ )
		{
			client_session* push_session = client_session_mgr::getInstance().get_session_by_array_index(idx);
			if (push_session == nullptr || push_session->is_vaild() == false)
			{
				continue;
			}
			if (push_session->get_role_guid() == m_client_session_ptr->get_role_guid())
			{
				continue;
			}
			if (push_session->m_role_info.data_ary[e_role_info_exp_level] < level_limit)
			{
				continue;
			}
			if (is_relation_exist(push_session->get_role_guid()))
			{
				continue;
			}
			xstring full_name = push_session->get_role_name();
			xstring::size_type pos = full_name.find(pattern);
			if (pos == full_name.npos)
			{
				continue;
			}
			add_ret_friend_userinfo(search_pak, push_session);

			get_count++;
			if (get_count >= SearchMax)
			{
				break;
			}
		}
		m_client_session_ptr->send_to_client(&search_pak, e_msgindex_s2c_ret_friend_userinfo);
	}

	void relation_mgr::req_marrage(int32 req_type, client_session& req, client_session& obj)
	{
		if (req_type == e_marry_packet_askmarry)
		{
			req.get_relation_list_mgr().set_promosing(obj.get_role_guid());
		}
		else if (req_type == e_marry_packet_accept)
		{
			if (req.get_relation_list_mgr().can_marry(obj,false))
			{
				req.get_relation_list_mgr().marry(obj);
			}
		}
		else if (req_type == e_marry_packet_refuse)
		{
			//req.get_relation_list_mgr().on_clear_proming();
		}
		else if (req_type == e_marry_packet_divorce)
		{
			if (req.get_relation_list_mgr().can_divorce(true))
			{
				req.get_relation_list_mgr().divorce();
			}
		}
		else if (req_type == e_marry_packet_parade)
		{
			if (req.get_relation_list_mgr().can_parade(obj, true))
			{
				req.get_relation_list_mgr().marry_parade(obj);
			}
		}
		else
		{
			// do nothing.
		}
	}

	relation_set*		relation_mgr::get_relation_list(e_relationlist_type relation_list_type)
	{
		if (relation_list_type>= e_relationlist_type_max)
		{
			return nullptr;
		}
		return &(m_releation_list[relation_list_type]);
	}
	void	relation_mgr::set_last_find_user_time(int64 time)
	{
		m_last_find_friend_user_info_time = time;
	}

	int64	relation_mgr::get_last_find_user_time()
	{
		return m_last_find_friend_user_info_time;
	}

	void	relation_mgr::update_relation_state(bool is_send_to_client, bool is_send_friend_online_notice)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		faith::relation_proto_s2c_ret_relation_state send_pak;
		bool have_change = false;
		for (int32 list_type = 0; list_type < e_relationlist_type_max; list_type++)
		{
			relation_set* list_ptr = get_relation_list((e_relationlist_type)list_type);
			if (nullptr != list_ptr)
			{
				relation_list& relation_list_ref = list_ptr->get_relation_list();
				for (relation_list_it it = relation_list_ref.begin(); it != relation_list_ref.end(); ++it)
				{
					relation& relation_ref = *it;
					client_session* session_ptr = client_session_mgr::getInstance().get_session(relation_ref.get_data_guid());
					bool it_have_change = false;
					if (nullptr != session_ptr)
					{
						if (list_type == e_relationlist_type_friend)
						{
							if (is_send_friend_online_notice)
							{
								if (relation_ref.get_data_value(e_relation_data_relation_state) == e_relation_offline
									&& relation_ref.get_data_value(e_relation_data_friendliness) >= 0)
								{
									send_friend_online_notice(session_ptr->get_role_name(), m_client_session_ptr);
								}

							}
						}
						if (relation_ref.get_data_value(e_relation_data_level) != session_ptr->m_role_info.data_ary[e_role_info_exp_level])
						{
							relation_ref.set_data_value(e_relation_data_level, session_ptr->m_role_info.data_ary[e_role_info_exp_level]);
							have_change = true;
							it_have_change = true;
						}
						
						relation_ref.set_data_value(e_relation_data_template_id, session_ptr->m_role_info.data_ary[e_role_info_template_id]);
						relation_ref.set_data_gs_value(session_ptr->get_role_gs_value());
						if (relation_ref.get_data_value(e_relation_data_level) != e_relation_online)
						{
							relation_ref.set_data_value(e_relation_data_relation_state, e_relation_online);
							have_change = true;
							it_have_change = true;
						}
						
						relation_ref.set_data_value(e_relation_data_last_login_time, session_ptr->m_logintime);
						relation_ref.set_data_value(e_relation_data_vip_title_template_id, get_vip_title_template_id(session_ptr));
					}
					else
					{
						if (relation_ref.get_data_value(e_relation_data_relation_state) == e_relation_online)
						{
							relation_ref.set_data_value(e_relation_data_last_login_time, time_helper::get_cur_time_new().second);
							have_change = true;
							it_have_change = true;
						}
						relation_ref.set_data_value(e_relation_data_relation_state, e_relation_offline);
					}
					if (is_send_to_client && it_have_change)
					{
						faith::relation_proto_ret_relation_state* ret_pak = send_pak.add_full_relation_state();
						if (nullptr != ret_pak)
						{
							ret_pak->set_relation_guid(relation_ref.get_data_guid().server_64);
							ret_pak->set_relation_state(relation_ref.get_data_value(e_relation_data_relation_state));
							ret_pak->set_relation_login_time(relation_ref.get_data_value(e_relation_data_last_login_time));
							if (nullptr != session_ptr)
								ret_pak->set_exp_level(session_ptr->m_role_info.data_ary[e_role_info_exp_level]);
						}
						if (send_pak.ByteSize() > packege_length_max)
						{
							m_client_session_ptr->send_to_client(&send_pak, e_msgindex_s2c_ret_relation_state);
							send_pak.clear_full_relation_state();
						}
					}
				}

			}
		}
		if (is_send_to_client && have_change)
		{
			m_client_session_ptr->send_to_client(&send_pak, e_msgindex_s2c_ret_relation_state);
		}
	}

	void	relation_mgr::load_relation_from_db(const s_relation_info(&datas)[MAX_FRIEND_NUM * e_relationlist_type_max], int32 data_num)
	{
		m_load_data = true;
		for (int32 index = 0; index < data_num; ++index)
		{
			if (!datas[index].relation_guid.is_valid())
			{
				break;
			}
			relation_set* list_ptr = get_relation_list((e_relationlist_type)datas[index].data_array[e_relation_data_type]);
			if (nullptr == list_ptr)
			{
				return;
			}
			if (list_ptr->is_full())
			{
				continue;
			}
			list_ptr->serialize_from_db(datas[index]);
		}
	}

	void	relation_mgr::save_relation_to_db(bool is_tick)
	{
		if (m_load_data == false)
		{
			return;
		}
		if (is_tick == false)
		{
			m_load_data = false;
		}
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		static ws2dp_save_player_relation req;
		req.clear_data();

		req.role_guid = m_client_session_ptr->get_role_guid();
		req.data_num = 0;
		for (int32 relation_type = 0; relation_type < e_relationlist_type_max; relation_type++)
		{
			relation_set* relation_list_ptr = get_relation_list((e_relationlist_type)relation_type);
			if (nullptr != relation_list_ptr)
			{
				if (relation_list_ptr->get_relation_num() > 0)
				{
					relation_list_ptr->serialize_to_db(req.data_list, req.data_num);
				}
			}
		}
		ws_client::getInstance().send_to_dp(&req, req.get_pak_length());
	}

	void	relation_mgr::req_load_relation_from_db()
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		server2dp_proto_ws2dp_req_load_relation_list req_pak;
		req_pak.set_sender_guid(m_client_session_ptr->get_role_guid().server_64);
		ws_client::getInstance().send_to_dp_new(&req_pak, e_msg_ws2dp_req_load_relation_list);

		//script_mgr::get_instance().call_func("ws2dp_send", "send_ws2dp_req_load_player_relation_lua", 0, false, "%l", m_client_session_ptr->get_role_guid().server_64);

	}

	void	relation_mgr::send_friend_online_notice(const xstring& friend_name,client_session* frined_session)
	{
		if (nullptr == frined_session)
		{
			return;
		}
		int32 notice_id = 93000071;
		//guid_64 sender_guid = m_client_session_ptr->get_role_guid();
		//int32 create_time = time_helper::get_cur_time_new().second;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(friend_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		frined_session->send_notice(notice_str);
		//event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, sender_guid, notice_str);
	}
	//-------------------------------------------------
	bool	relation_mgr::send_other_add_msg_to_self(relation* req_player)
	{
		if (nullptr == m_client_session_ptr)
		{
			return false;
		}
		if (nullptr == req_player)
		{
			return false;
		}
		if (req_player->get_data_guid() == m_client_session_ptr->get_role_guid())
		{
			return false;
		}

		relation_proto_s2c_otheraddfriend pak;
		pak.set_guid(req_player->get_data_guid().server_64);
		pak.set_name(req_player->get_data_name());
		pak.set_gs_value(init_unit::change_i64_to_string(req_player->get_data_gs_value()));
		for (int32 data_index = 0; data_index < e_relation_data_max; data_index++)
		{
			pak.add_data_array(req_player->get_data_value((e_relation_data)data_index));
		}
		m_client_session_ptr->send_to_client(&pak, e_msgindex_s2c_otheraddfriend);
		return true;
	}


	int32	relation_mgr::get_relation_num(e_relationlist_type relation_type)
	{
		if (relation_type>= e_relationlist_type_max)
		{
			return 0;
		}
		return m_releation_list[relation_type].get_relation_num();
	}

	bool	relation_mgr::is_relation_full(e_relationlist_type relation_type)
	{
		if (relation_type >= e_relationlist_type_max)
		{
			return true;
		}
		return m_releation_list[relation_type].is_full();
	}

	bool	relation_mgr::is_relation_exist(e_relationlist_type relation_type, const guid_64& friendGuid)
	{
		if (relation_type >= e_relationlist_type_max)
		{
			return false;
		}
		if (!friendGuid.is_valid())
		{
			return false;
		}

		return m_releation_list[relation_type].is_relation(friendGuid);
	}
	bool	relation_mgr::is_relation_exist(const guid_64& friendGuid)
	{
		if (!friendGuid.is_valid())
		{
			return false;
		}
		for (int i = 0; i < e_relationlist_type_max; i++)
		{
			if (i == e_relationlist_type_chat)
				continue;
			if (m_releation_list[i].is_relation(friendGuid))
			{
				return true;
			}
		}
		return false;
	}
	bool	relation_mgr::add_unit_to_relation_list(const s_relation_info& realtion_info, bool remove_other, bool req_add_friend)
	{
		int32 relation_type = realtion_info.data_array[e_relation_data_type];
		if (relation_type < 0 || relation_type >= e_relationlist_type_max)
		{
			return false;
		}
		if (remove_other)
		{
			for (int i = 0; i < e_relationlist_type_max; i++)
			{
				if (e_relationlist_type_chat == i)
					continue;
				if (e_relationlist_type_friend == relation_type && e_relationlist_type_lucky == i)
					continue;
				if (e_relationlist_type_lucky == relation_type && e_relationlist_type_other_req_add_lucky != i)
					continue;
				if (e_relationlist_type_other_req_add == relation_type && i >= e_relationlist_type_chat)
					continue;
				if (e_relationlist_type_other_req_add_lucky == relation_type && i != e_relationlist_type_other_req_add)
					continue;
				m_releation_list[i].del_relation(realtion_info.relation_guid);
			}
		}
		bool add_result = true;
		if (!req_add_friend || (relation_type != e_relationlist_type_friend && relation_type != e_relationlist_type_lucky))
		{
			if (!m_releation_list[relation_type].is_relation(realtion_info.relation_guid))
			{
				add_result = m_releation_list[relation_type].add_relation(realtion_info, true);
			}
		}
		
		if (add_result && (relation_type == e_relationlist_type_friend || relation_type == e_relationlist_type_lucky))
		{
			if (req_add_friend)
			{
				client_session* other = client_session_mgr::getInstance().get_session(realtion_info.relation_guid);
				if (nullptr != other)
				{
					if (relation_type == e_relationlist_type_friend)
					{
						if (other->get_relation_list_mgr().is_relation_exist(m_client_session_ptr->get_role_guid()) == false)
							other->get_relation_list_mgr().add_relation(e_relationlist_type_other_req_add, m_client_session_ptr, true, true);
					}
					else
					{
						guid_64 my_guid = m_client_session_ptr->get_role_guid();
						if(other->get_relation_list_mgr().is_relation_exist(e_relationlist_type_lucky, my_guid) == false
							&& other->get_relation_list_mgr().is_relation_exist(e_relationlist_type_other_req_add_lucky, my_guid) == false
							&& other->get_relation_list_mgr().is_relation_exist(e_relationlist_type_black, my_guid) == false
							&& other->get_relation_list_mgr().is_relation_exist(e_relationlist_type_hate, my_guid) == false)
							other->get_relation_list_mgr().add_relation(e_relationlist_type_other_req_add_lucky, m_client_session_ptr, true, true);
					}
				}
				else
				{
					//target off online
					s_relation_info req_add_relation_info;

					req_add_relation_info.relation_guid = m_client_session_ptr->get_role_guid();
					memcpy(req_add_relation_info.relation_name, m_client_session_ptr->m_role_info.role_name, max_name_size);
					req_add_relation_info.gs_value = m_client_session_ptr->get_role_gs_value();
					if (relation_type == e_relationlist_type_friend)
						req_add_relation_info.data_array[e_relation_data_type] = e_relationlist_type_other_req_add;
					else
						req_add_relation_info.data_array[e_relation_data_type] = e_relationlist_type_other_req_add_lucky;
					req_add_relation_info.data_array[e_relation_data_level] = m_client_session_ptr->m_role_info.data_ary[e_role_info_exp_level];
					req_add_relation_info.data_array[e_relation_data_template_id] = m_client_session_ptr->m_role_info.data_ary[e_role_info_template_id];
					req_add_relation_info.data_array[e_relation_data_relation_state] = e_relation_online;
					req_add_relation_info.data_array[e_relation_data_last_login_time] = m_client_session_ptr->m_logintime;
					req_add_relation_info.data_array[e_relation_data_friendliness] = 0;
					req_add_relation_info.data_array[e_relation_data_vip_title_template_id] = get_vip_title_template_id(m_client_session_ptr);

					add_relation_to_dp(realtion_info.relation_guid, req_add_relation_info);
				}
			}
			else
			{
				sync_full_relation_list_to_cs(true);
			}
		}
		return add_result;
	}

	bool	relation_mgr::del_unit_from_relation_list(e_relationlist_type relation_type, const guid_64& friend_guid)
	{
		relation_set* relation_set_ptr = get_relation_list(relation_type);
		if (nullptr == relation_set_ptr)
		{
			return false;
		}
		return relation_set_ptr->del_relation(friend_guid);
	}


	void relation_mgr::sync_relation_list_to_client()
	{
		for (int32 i = 0; i < e_relationlist_type_max; i++)
		{
			m_releation_list[i].send_sync_relation_list_to_client(e_msgindex_s2c_syc_full_friend_list);
		}
	}

	bool	relation_mgr::add_player_from_other_req(const guid_64& guid)
	{
		if (nullptr == m_client_session_ptr)
		{
			return false;
		}

		e_relationlist_type relation_type;
		relation* operate_relation = m_releation_list[e_relationlist_type_other_req_add].get_relation(guid);
		if (nullptr != operate_relation)
		{
			relation_type = e_relationlist_type_friend;
		}
		else
		{
			operate_relation = m_releation_list[e_relationlist_type_other_req_add_lucky].get_relation(guid);
			if (nullptr == operate_relation)
				return false;
			relation_type = e_relationlist_type_lucky;
		}

		client_session* target_session_ptr = client_session_mgr::getInstance().get_session(guid);
		if (relation_type == e_relationlist_type_lucky && nullptr == target_session_ptr)
		{
			m_client_session_ptr->send_notice("90090421");
			m_releation_list[e_relationlist_type_other_req_add_lucky].del_relation(guid);
			return false;
		}

		s_relation_info relation_info = operate_relation->get_data_all();
		relation_info.data_array[e_relation_data_type] = relation_type;
		if (nullptr != target_session_ptr)
		{
			if (target_session_ptr->get_relation_list_mgr().is_relation_full(relation_type))
			{
				return false;
			}

			if (target_session_ptr->get_relation_list_mgr().is_relation_exist(relation_type, m_client_session_ptr->get_role_guid()))
			{
				return false;
			}
		}
	    bool result = add_unit_to_relation_list(relation_info, true, false);

		if (!result)
		{
			return result;
		}

		if (relation_type == e_relationlist_type_lucky)
		{
			if (is_relation_full(e_relationlist_type_friend) == false && is_relation_exist(e_relationlist_type_friend, guid) == false)
			{
				relation_info.data_array[e_relation_data_type] = e_relationlist_type_friend;
				add_unit_to_relation_list(relation_info, true, false);
			}
		}

		s_relation_info target_relation_info;
		target_relation_info.relation_guid = m_client_session_ptr->get_role_guid();
		memcpy(target_relation_info.relation_name, m_client_session_ptr->m_role_info.role_name, max_name_size);
		target_relation_info.gs_value = m_client_session_ptr->get_role_gs_value();
		target_relation_info.data_array[e_relation_data_type] = relation_type;
		target_relation_info.data_array[e_relation_data_level] = m_client_session_ptr->m_role_info.data_ary[e_role_info_exp_level];
		target_relation_info.data_array[e_relation_data_template_id] = m_client_session_ptr->m_role_info.data_ary[e_role_info_template_id];
		target_relation_info.data_array[e_relation_data_relation_state] = e_relation_online;
		target_relation_info.data_array[e_relation_data_last_login_time] = m_client_session_ptr->m_logintime;
		target_relation_info.data_array[e_relation_data_friendliness] = 0;
		target_relation_info.data_array[e_relation_data_vip_title_template_id] = get_vip_title_template_id(m_client_session_ptr);
		
		if (relation_type == e_relationlist_type_lucky && nullptr != target_session_ptr)
		{
			relation_set* relation_set_ptr = target_session_ptr->get_relation_list_mgr().get_relation_list(e_relationlist_type_friend);
			if (nullptr != relation_set_ptr)
			{
				relation* relation_ptr = relation_set_ptr->get_relation(m_client_session_ptr->get_role_guid());
				if (nullptr != relation_ptr)
				{
					target_relation_info.data_array[e_relation_data_friendliness] += relation_ptr->get_data_all().data_array[e_relation_data_friendliness];
				}
			}
		}
		
		if (nullptr != target_session_ptr)
		{
			result = target_session_ptr->get_relation_list_mgr().add_unit_to_relation_list(target_relation_info, true, false);
			if (result)
			{
				target_session_ptr->get_relation_list_mgr().send_operate_result_to_client(e_relation_operate_result_pass_req_friend);
			}
		}
		else
		{
			add_relation_to_dp(guid, target_relation_info);
			result = true;
		}

		if (relation_type == e_relationlist_type_lucky && nullptr != target_session_ptr)
		{
			target_relation_info.data_array[e_relation_data_type] = e_relationlist_type_friend;
			if (target_session_ptr->get_relation_list_mgr().is_relation_full(e_relationlist_type_friend) == false
				&& target_session_ptr->get_relation_list_mgr().is_relation_exist(e_relationlist_type_friend, m_client_session_ptr->get_role_guid()) == false)
			{
				target_session_ptr->get_relation_list_mgr().add_unit_to_relation_list(target_relation_info, true, false);
			}
		}

		add_friend_success_chat_and_send_advices(guid, target_relation_info);

		if (result && relation_type == e_relationlist_type_friend)
		{
			ws2cs_req_relation_end to_cs_msg;
			to_cs_msg.self_index = m_client_session_ptr->get_cs_array_index();
			to_cs_msg.relation_type = e_relationlist_type_friend;
			to_cs_msg.target_guid = guid;

			bool is_use = proto_by_lua(e_msgindex_ws2cs_req_relation_end);
			if (is_use == false)
			{
				m_client_session_ptr->send_to_cs(&to_cs_msg, sizeof(ws2cs_req_relation_end));
			}
			else
			{
				faith::ws2cs_proto::req_relation_end pro_msg;
				to_cs_msg.to_proto(pro_msg);
				m_client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_req_relation_end);
			}
		}
		return result;
	}

	void relation_mgr::operate_player_from_other_req(const faith::relation_proto_c2s_operate_other_add_req& packet)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		int32 array_size = packet.operate_guid_size();
		if (array_size <= 0)
		{
			return;
		}
		if (packet.is_add() == false)
		{
			for (int32 index = 0; index < array_size; index++)
			{
				m_releation_list[e_relationlist_type_other_req_add].del_relation(packet.operate_guid(index));
				m_releation_list[e_relationlist_type_other_req_add_lucky].del_relation(packet.operate_guid(index));
			}
			send_operate_result_to_client(e_relation_operate_result_success);
		}
		else
		{
			int32 success_times = 0;
			for (int32 index = 0; index < array_size; index++)
			{
				bool result = add_player_from_other_req(packet.operate_guid(index));
				if (result)
				{
					success_times++;
				}
			}

			if (success_times > 0)
			{
				if (1 == array_size)
				{
					send_operate_result_to_client(e_relation_operate_result_add_friend_success);
				}
				else
				{
					if (array_size == success_times)
					{
						send_operate_result_to_client(e_relation_operate_result_add_friend_all_once);
					}
					else
					{
						send_operate_result_to_client(e_relation_operate_result_friend_list_full);
					}
				}
			}
		}
	}

	void	relation_mgr::req_relation_info_from_db(int32 relation_type, const guid_64& other_guid)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (other_guid.is_valid() == false)
		{
			return;
		}
		if (relation_type < e_relationlist_type_friend || relation_type >= e_relationlist_type_max)
		{
			return;
		}
		//ws2dp_load_relation_info req_msg;
		//req_msg.list_type = relation_type;
		//req_msg.sender_guid = m_client_session_ptr->get_role_guid();
		//req_msg.other_guid = other_guid;
		//ws_client::getInstance().send_to_dp(&req_msg, req_msg.get_pak_length());

		script_mgr::get_instance().call_func("ws2dp_send", "send_ws2dp_req_load_offline_other_relation_lua", 0, false, "%l%d%l", m_client_session_ptr->get_role_guid().server_64, relation_type, other_guid.server_64);

	}

	void	relation_mgr::recv_other_relation_info_from_db(const s_relation_info& other_realtion_info)
	{
		if (other_realtion_info.relation_guid.is_valid() == false)
		{
			return;
		}
		s_relation_info other_realtion = other_realtion_info;
		
		if (nullptr == m_client_session_ptr)
		{
			return;
		}

		bool req_add_friend = false;
		if (other_realtion.data_array[e_relation_data_type] == e_relationlist_type_friend || other_realtion.data_array[e_relation_data_type] == e_relationlist_type_lucky)
		{
			req_add_friend = true;
		}

		bool remove_other = true;
		if (other_realtion.data_array[e_relation_data_type] == e_relationlist_type_chat)
		{
			remove_other = false;
		}

		if (other_realtion.data_array[e_relation_data_type] == e_relationlist_type_chat || other_realtion.data_array[e_relation_data_type] == e_relationlist_type_lucky)
		{
			relation_set* relation_set_ptr = get_relation_list(e_relationlist_type_friend);
			if (nullptr != relation_set_ptr)
			{
				relation* relation_ptr = relation_set_ptr->get_relation(other_realtion.relation_guid);
				if (nullptr != relation_ptr)
				{
					other_realtion.data_array[e_relation_data_friendliness] += relation_ptr->get_data_all().data_array[e_relation_data_friendliness];
				}
			}
		}

		bool result = false;
		if (!is_relation_exist((e_relationlist_type)other_realtion.data_array[e_relation_data_type], other_realtion.relation_guid))
		{
			result = add_unit_to_relation_list(other_realtion, remove_other, req_add_friend);
		}
		if (false == result)
		{
			return;
		}

		if (other_realtion.data_array[e_relation_data_type] == e_relationlist_type_black || other_realtion.data_array[e_relation_data_type] == e_relationlist_type_hate)
		{
			del_relation_to_dp(other_realtion.relation_guid,m_client_session_ptr->get_role_guid(), e_relationlist_type_friend);
			del_relation_to_dp(other_realtion.relation_guid, m_client_session_ptr->get_role_guid(), e_relationlist_type_lucky);
		}

		if (req_add_friend)
		{
			send_operate_result_to_client(e_relation_operate_result_req_friend_success);
		}
		else
		{
			send_operate_result_to_client(e_relation_operate_result_add_friend_success);
		}
	}
	void	relation_mgr::sync_full_relation_list_to_cs(bool is_add)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		ws2cs_full_friend_guid full_friend_pak;
		full_friend_pak.is_add = is_add;
		full_friend_pak.player_index = m_client_session_ptr->get_cs_array_index();
		int32 friend_num = 0;
		full_friend_pak.highest_firendliness_value = m_releation_list[e_relationlist_type_friend].get_max_firendliness();
		full_friend_pak.friend_num = m_releation_list[e_relationlist_type_friend].get_relation_num();
		bool is_use = proto_by_lua(e_msgindex_ws2cs_full_friend_guid);
		if (is_use == false)
		{
			m_client_session_ptr->send_to_cs(&full_friend_pak, sizeof(ws2cs_full_friend_guid));
		}
		else
		{
			faith::ws2cs_proto::full_friend_guid pro_msg;
			full_friend_pak.to_proto(pro_msg);
			m_client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_full_friend_guid);
		}
	}

	//============================================================================================

	void	relation_mgr::update_lover_name(const guid_64& rGuid, const std::string& rName)
	{
		if (get_lover_guid() != rGuid || rName.size() <= 0)
		{
			return;
		}
	}

	void relation_mgr::add_friendliness(const guid_64& relation_guid, int32 add_value)
	{
		if (false == relation_guid.is_valid())
		{
			return;
		}

		relation* relation_ptr = m_releation_list[e_relationlist_type_friend].get_relation(relation_guid);
		if (nullptr == relation_ptr)
		{
			return;
		}
		relation_ptr->set_data_value(e_relation_data_friendliness, add_value);
	}


	void relation_mgr::send_relation_friendliness_to_self(const guid_64& relation_guid)
	{
		if (false == relation_guid.is_valid())
		{
			return;
		}
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		relation* relation_ptr = m_releation_list[e_relationlist_type_friend].get_relation(relation_guid);
		if (nullptr == relation_ptr)
		{
			return;
		}
		relation_proto_s2c_ret_friendliness_value pak;
		pak.set_relation_guid(relation_guid.server_64);
		pak.set_relation_friendliness(relation_ptr->get_data_value(e_relation_data_friendliness));
		m_client_session_ptr->send_to_client(&pak, e_msgindex_s2c_ret_friendliness_value);
	}


	void	relation_mgr::fuben_add_friendliness_func(const guid_64 team_member_guid[max_team_member_num], int32 team_num)
	{
		if (team_num <= 0)
		{
			return;
		}
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		std::vector<std::string> friend_member_name;
		bool is_change_value = false;
		for (int32 friend_count = 0; friend_count < team_num; friend_count++)
		{
			if (team_member_guid[friend_count] == m_client_session_ptr->get_role_guid())
			{
				continue;
			}
			relation* friend_relation = m_releation_list[e_relationlist_type_friend].get_relation(team_member_guid[friend_count]);
			if (nullptr == friend_relation)
			{
				continue;
			}

			friend_member_name.push_back(friend_relation->get_data_name());
			int32 old_friendliness = friend_relation->get_data_value(e_relation_data_friendliness);
			friend_relation->set_data_value(e_relation_data_friendliness, old_friendliness + GAMECONFIG->MissionFriendlinessValue);
			faith::relation_proto_s2c_ret_friendliness_value ret_pak;
			ret_pak.set_relation_guid(friend_relation->get_data_guid().server_64);
			ret_pak.set_relation_friendliness(friend_relation->get_data_value(e_relation_data_friendliness));
			m_client_session_ptr->send_to_client(&ret_pak, e_msgindex_s2c_ret_friendliness_value);

			relation* chat_relation = m_releation_list[e_relationlist_type_chat].get_relation(team_member_guid[friend_count]);
			if (nullptr != chat_relation)
			{
				chat_relation->set_data_value(e_relation_data_friendliness, old_friendliness + GAMECONFIG->MissionFriendlinessValue);
			}
			relation* lucky_relation = m_releation_list[e_relationlist_type_lucky].get_relation(team_member_guid[friend_count]);
			if (nullptr != lucky_relation)
			{
				lucky_relation->set_data_value(e_relation_data_friendliness, old_friendliness + GAMECONFIG->MissionFriendlinessValue);
			}

			is_change_value = true;
		}

		if (is_change_value)
		{
			sync_full_relation_list_to_cs(false);
			int32 notice_id = 93000070;
			int32 create_time = time_helper::get_cur_time_new().second;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			std::string total_friend_name = "";
			int32 member_num = friend_member_name.size();
			for (int32 index = 0; index < member_num; index++)
			{
				total_friend_name.append(friend_member_name[index]);
				total_friend_name.append(" ");
			}
			vec_notice_str.push_back(total_friend_name);
			std::string add_value_str = init_unit::change_i64_to_string(GAMECONFIG->MissionFriendlinessValue);
			vec_notice_str.push_back(add_value_str);
			std::string notice_str = init_unit::implode(vec_notice_str);
			event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_client_session_ptr->get_role_guid(), notice_str);

		}
	}


	void	relation_mgr::send_gift_add_friendliness_func(const guid_64& addreessee_guid, int32 gift_id, int32 gift_count)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		if (addreessee_guid.is_valid() == false || gift_id <= 0 || gift_count <= 0)
		{
			return;
		}
		relation* addreesee_relation = m_releation_list[e_relationlist_type_friend].get_relation(addreessee_guid);
		if (nullptr == addreesee_relation)
		{
			s_item_info gift_item;
			bool result = faith::init_unit::init_item_data(gift_item, gift_id, -1, gift_count, e_bag_type_bag, 1);
			if (false == result)
			{
				return;
			}
			int32 title_id = 90095140;
			int32 context_id = 90095141;
			std::vector<s_item_info> item_list;
			item_list.push_back(gift_item);
			StringTemplate* mail_title_text_string = GET_TEMPLATE(StringTemplate, title_id);
			const std::vector<std::string>& temp_title_string_array_list = template_manager::get_instance().get_string_list(title_id);
			std::string mail_title_name = template_manager::get_instance().get_str_by_string_template_id(90303013);
			if (temp_title_string_array_list.size() > 0)
			{
				StringTemplate* mail_content_text_string = GET_TEMPLATE(StringTemplate, context_id);
				const std::vector<std::string>& temp_content_string_array_list = template_manager::get_instance().get_string_list(context_id);
				if (temp_content_string_array_list.size() > 0)
				{
					event_ws_mgr::get_instance().send_mail_to_player_by_system(m_client_session_ptr->get_role_guid(), m_client_session_ptr->get_cs_conn_index(),mail_title_name,
						temp_title_string_array_list[0],
						temp_content_string_array_list[0],
						0, 0, 0, 0, item_list);
				}
			}
			return;
		}
		ItemTemplate* gift_ptr = GET_TEMPLATE(ItemTemplate, gift_id);
		if (nullptr == gift_ptr)
		{
			return;
		}
		PropBasicAttributeTemplate* gift_prop_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, gift_ptr->logic_id);
		if (nullptr == gift_prop_ptr)
		{
			return;
		}
		int32 old_friendliness = addreesee_relation->get_data_value(e_relation_data_friendliness);
		addreesee_relation->set_data_value(e_relation_data_friendliness, old_friendliness + gift_prop_ptr->FriendlinessAdd * gift_count);

		relation* chat_relation = m_releation_list[e_relationlist_type_chat].get_relation(addreessee_guid);
		if (nullptr != chat_relation)
		{
			chat_relation->set_data_value(e_relation_data_friendliness, old_friendliness + gift_prop_ptr->FriendlinessAdd * gift_count);
		}
		relation* lucky_relation = m_releation_list[e_relationlist_type_lucky].get_relation(addreessee_guid);
		if (nullptr != lucky_relation)
		{
			lucky_relation->set_data_value(e_relation_data_friendliness, old_friendliness + gift_prop_ptr->FriendlinessAdd * gift_count);
		}
		faith::relation_proto_s2c_ret_friendliness_value ret_pak;
		ret_pak.set_relation_guid(addreesee_relation->get_data_guid().server_64);
		ret_pak.set_relation_friendliness(addreesee_relation->get_data_value(e_relation_data_friendliness));
		ret_pak.set_gift_id(gift_id);
		m_client_session_ptr->send_to_client(&ret_pak, e_msgindex_s2c_ret_friendliness_value);
		sync_full_relation_list_to_cs(false);
	}


	void	relation_mgr::sync_lover_to_client()
	{
		if (m_client_session_ptr == nullptr)
		{
			return;
		}
		relation_proto_s2c_syn_loverinfo pak;
		pak.set_loverguid(get_lover_guid().server_64);
		m_client_session_ptr->send_to_client(&pak, e_msgindex_s2c_syn_loverinfo);
	}
	guid_64 relation_mgr::get_lover_guid()
	{
		return m_marriage.m_lover_guid;
	}

	bool	relation_mgr::set_lover_guid(guid_64 loverguid)
	{
		if (!loverguid.is_valid() || m_marriage.m_lover_guid.is_valid())
		{
			return false;
		}
		m_marriage.m_lover_guid = loverguid;
		return true;
	}

	int8	relation_mgr::get_lover_status()
	{
		return m_marriage.m_lover_status;
	}

	void	relation_mgr::set_lover_status(int8 nValue)
	{
		if (nValue > marriage_data::e_lover_nostatus && nValue < marriage_data::e_lover_maxstatus)
		{
			m_marriage.m_lover_status = nValue;
		}
	}

	bool	relation_mgr::can_marry(client_session& ruser, bool btips)
	{
		//level based
		if (m_client_session_ptr->m_role_info.data_ary[e_role_info_exp_level] < MARRY_MIN_LEVEL
			|| ruser.m_role_info.data_ary[e_role_info_exp_level] < MARRY_MIN_LEVEL)
		{
			return false;
		}
		//lover based
		if (!ruser.get_relation_list_mgr().get_lover_guid().is_valid())
		{
			return false;
		}
		if (!get_lover_guid().is_valid())
		{
			return false;
		}
		if (ruser.get_relation_list_mgr().get_lover_status() == marriage_data::e_lover_break && ruser.get_relation_list_mgr().is_breaking_heart())
		{
			return false;
		}
		if (get_lover_status() == marriage_data::e_lover_break && is_breaking_heart())
		{
			return false;
		}
		return true;
	}

	void	relation_mgr::marry(client_session& ruser)
	{
		if (can_marry(ruser, false))
		{
			int32 nitemcount1 = 0/*getbackpack().getcountbydataid(marry_item_normal)*/;
			int32 nitemcount2 = 0/*getbackpack().getcountbydataid(marry_item_luxury)*/;
			int32 nitemid = 0;
			int8  ndeststauts = marriage_data::e_lover_nostatus;
			if (nitemcount2 > 0)
			{
				nitemid = MARRY_ITEM_LUXURY;
				ndeststauts = marriage_data::e_lover_luxury;
			}
			else if (nitemcount1 > 0)
			{
				nitemid = MARRY_ITEM_NORMAL;
				ndeststauts = marriage_data::e_lover_normal;
			}
			else
			{
				return;
			}
			//del

			//relation
			if (set_lover_guid(ruser.get_role_guid()) && ruser.get_relation_list_mgr().set_lover_guid(m_client_session_ptr->get_role_guid()))
			{
				//status
				set_lover_status(ndeststauts);
				ruser.get_relation_list_mgr().set_lover_status(ndeststauts);

				sync_lover_to_client();
				ruser.get_relation_list_mgr().sync_lover_to_client();
			}

		}
	}

	bool	relation_mgr::is_parade_expired()
	{
		if (m_marriage.m_lover_status != marriage_data::e_lover_luxury)
		{
			return true;
		}
		if (m_marriage.m_marrage_time <= 0)
		{
			return true;
		}
		time_t curTime/* = gTimeManager.GetANSITime()*/;
		time(&curTime);
		if (curTime - m_marriage.m_marrage_time < PARADE_VALIDTIME)
		{
			return false;
		}
		return true;
	}

	bool	relation_mgr::can_parade(client_session& rUser, bool bTips)
	{
		//Lover Based
		if (rUser.get_relation_list_mgr().get_lover_guid() != m_client_session_ptr->get_role_guid() || get_lover_guid() != rUser.get_role_guid())
		{
			return false;
		}
		if (rUser.get_relation_list_mgr().get_lover_status() == marriage_data::e_lover_luxury_over
			|| get_lover_status() == marriage_data::e_lover_luxury_over)
		{
			return false;
		}
		if (rUser.get_relation_list_mgr().get_lover_status() != marriage_data::e_lover_luxury
			|| get_lover_status() != marriage_data::e_lover_luxury)
		{
			return false;
		}
		if (rUser.get_relation_list_mgr().is_parade_expired() || is_parade_expired())
		{
			return false;
		}
		return true;
	}

	void	relation_mgr::marry_parade(client_session& rUser)
	{
		if (can_parade(rUser, false))
		{
			set_lover_status(marriage_data::e_lover_luxury_over);
			rUser.get_relation_list_mgr().set_lover_status(marriage_data::e_lover_luxury_over);
		}
	}

	bool	relation_mgr::is_breaking_heart()
	{
		if (m_marriage.m_lover_status != marriage_data::e_lover_break)
		{
			return false;
		}
		if (m_marriage.m_marrage_time <= 0)
		{
			return false;
		}
		time_t curTime/* = gTimeManager.GetANSITime()*/;
		time(&curTime);
		if (curTime - m_marriage.m_marrage_time < MARRY_BREAK_TIME)
		{
			return true;
		}
		return false;
	}

	bool	relation_mgr::can_divorce(bool bTips)
	{
		if (!get_lover_guid().is_valid())
		{
			return false;
		}
		return true;
	}

	void	relation_mgr::divorce()
	{
		if (can_divorce(false))
		{
			guid_64 guidLover = get_lover_guid();
			on_divorce();
		}
	}

	void	relation_mgr::on_divorce()
	{
		//delete title
		guid_64 guidLover = get_lover_guid();
		m_marriage.clean_up_data();

		m_marriage.m_lover_status = marriage_data::e_lover_break;
		sync_lover_to_client();

		//if (IsSceneValid())
		{
			client_session* lover = client_session_mgr::getInstance().get_session(guidLover);
			if (lover && lover->is_vaild())
			{
				lover->get_relation_list_mgr().sync_lover_to_client();
			}
		}
	}


	///////////////// Code Here : class marry_process ////////////////////////////////////////////////////

	//marry_process::marry_process(map_cs& rscene)
	//	:m_rscene(rscene)
	//{
	//	cleanup();
	//}

	void marry_process::cleanup()
	{
		m_operator1.clear_data();
		m_operator2.clear_data();
		m_step = e_marry_process_invalid;
		m_overtime = 0;
	}

	void marry_process::heart_tick(const int64& new_time)
	{
		if ( /*rinfo.m_bdiffsecond &&*/ isdoing())
		{
			nextstep();
		}
	}

	void marry_process::start(const guid_64& op1, const guid_64& op2)
	{
		if (isdoing())
		{
			return;
		}
		// 		if ( !_gameconfig().m_bisopenmarrage )
		// 		{
		// 			return;
		// 		}

		client_session* puser1 = client_session_mgr::getInstance().get_session(op1);
		if (puser1 == nullptr)
		{
			return;
		}
		client_session* puser2 = client_session_mgr::getInstance().get_session(op2);
		if (puser2 == nullptr)
		{
			return;
		}
		if (puser1->is_vaild() && puser2->is_vaild())
		{
			m_operator1 = puser1->get_role_guid();
			m_operator2 = puser2->get_role_guid();
			m_step = e_marry_process_story;

			//sendstory
// 			userrefcont cont;
// 			m_rscene.scan_user_all(cont);
// 			for (int32 i = 0; i < cont.size(); i++)
// 			{
// 				cont[i].playstory(marry_process_story);
// 			}

			//unmount
// 			puser1->singlemount_unmount();
// 			puser1->markautomount(0);
// 			puser2->singlemount_unmount();
// 			puser2->markautomount(0);
// 			puser1->hideactivefellow();
// 			puser2->hideactivefellow();
			//begin to move
// 			puser1->forcesetscenepos(marry_process_pos1);
// 			puser2->forcesetscenepos(marry_process_pos2);

// 			cachelog(logdef_inst(marrage),
// 				"startpro,u1[%08x,%08x],u2[%08x,%08x]",
// 				puser1->get_unit_guid().gethigh32value(),puser1->get_unit_guid().getlow32value(),
// 				puser2->get_unit_guid().gethigh32value(),puser2->get_unit_guid().getlow32value());
// 			auditlog(logdef_inst(audit_marrage), "pros",puser1->get_unit_guid(), "playerlevel=%d \1 viplevel=%d", 
// 				puser1->getlevel(),puser1->getviplv());
// 			auditlog(logdef_inst(audit_marrage), "pros",puser2->get_unit_guid(), "playerlevel=%d \1 viplevel=%d", 
// 				puser2->getlevel(),puser2->getviplv());
		}
	}

	void marry_process::nextstep()
	{
		client_session* puser1 = client_session_mgr::getInstance().get_session(m_operator1);
		if (puser1 == nullptr)
		{
			return;
		}
		client_session* puser2 = client_session_mgr::getInstance().get_session(m_operator2);
		if (puser2 == nullptr)
		{
			return;
		}
		if (!puser1->is_vaild() || !puser2->is_vaild())
		{
			cleanup();

			// 			cachelog(logdef_inst(marrage),
			// 				"nouser,u1[%08x,%08x],u2[%08x,%08x]",
			// 				m_operator1.gethigh32value(),m_operator1.getlow32value(),
			// 				m_operator2.gethigh32value(),m_operator2.getlow32value());

			return;
		}
		client_session* ruser1 = puser1;
		client_session* ruser2 = puser2;
		if (ruser1->get_relation_list_mgr().get_lover_guid() != ruser2->get_role_guid()
			|| ruser2->get_relation_list_mgr().get_lover_guid() != ruser1->get_role_guid())
		{
			cleanup();

			// 			cachelog(logdef_inst(marrage),
			// 				"nofit,u1[%08x,%08x],u2[%08x,%08x]",
			// 				ruser1.get_unit_guid().gethigh32value(),ruser1.get_unit_guid().getlow32value(),
			// 				ruser2.get_unit_guid().gethigh32value(),ruser2.get_unit_guid().getlow32value());
			return;
		}
		switch (m_step)
		{
		case e_marry_process_invalid:
			break;
		case e_marry_process_story:
		{
			m_overtime++;
			if (m_overtime >= MARRY_PROCESS_MAXTIME)
			{
				cleanup();
				return;
			}
			// 				if (ruser1.getscenepos().distancesquare(marry_process_pos1) < 2 
			// 					&& ruser2.getscenepos().distancesquare(marry_process_pos2) < 2 )
			{
				m_step = e_marry_process_action1;
				m_overtime = 0;

				// 					cachelog(logdef_inst(marrage),
				// 						"story,u1[%08x,%08x],u2[%08x,%08x]",
				// 						ruser1.get_unit_guid().gethigh32value(),ruser1.get_unit_guid().getlow32value(),
				// 						ruser2.get_unit_guid().gethigh32value(),ruser2.get_unit_guid().getlow32value());
			}
		}
		break;
		case e_marry_process_action1:
		{
			m_overtime++;
			if (m_overtime >= MARRY_STEP_INTERVAL)
			{
				// 					//skill action
				// 					packets::gc_ret_use_skill_pak retpak1;
				// 					retpak1.m_packetdata.set_skillid(marry_process_action1);
				// 					retpak1.m_packetdata.set_senderid(ruser1.getid());
				// 					retpak1.m_packetdata.set_targetid(0);
				// 					m_rscene.broadcast_insight_include(retpak1,ruser1.getid());
				// 					packets::gc_ret_use_skill_pak retpak2;
				// 					retpak2.m_packetdata.set_skillid(marry_process_action1);
				// 					retpak2.m_packetdata.set_senderid(ruser2.getid());
				// 					retpak2.m_packetdata.set_targetid(0);
				// 					m_rscene.broadcast_insight_include(retpak2,ruser2.getid());

				m_overtime = 0;
				m_step = e_marry_process_action2;

				// 					cachelog(logdef_inst(marrage),
				// 						"s1,u1[%08x,%08x],u2[%08x,%08x]",
				// 						ruser1.get_unit_guid().gethigh32value(),ruser1.get_unit_guid().getlow32value(),
				// 						ruser2.get_unit_guid().gethigh32value(),ruser2.get_unit_guid().getlow32value());
			}
		}
		break;
		case e_marry_process_action2:
		{
			m_overtime++;
			if (m_overtime >= MARRY_STEP_INTERVAL)
			{
				//skill action
// 					packets::gc_ret_use_skill_pak retpak1;
// 					retpak1.m_packetdata.set_skillid(marry_process_action2);
// 					retpak1.m_packetdata.set_senderid(ruser1.getid());
// 					retpak1.m_packetdata.set_targetid(0);
// 					m_rscene.broadcast_insight_include(retpak1,ruser1.getid());
// 					packets::gc_ret_use_skill_pak retpak2;
// 					retpak2.m_packetdata.set_skillid(marry_process_action2);
// 					retpak2.m_packetdata.set_senderid(ruser2.getid());
// 					retpak2.m_packetdata.set_targetid(0);
// 					m_rscene.broadcast_insight_include(retpak2,ruser2.getid());

				m_overtime = 0;
				m_step = e_marry_process_action3;

				// 					cachelog(logdef_inst(marrage),
				// 						"s2,u1[%08x,%08x],u2[%08x,%08x]",
				// 						ruser1.get_unit_guid().gethigh32value(),ruser1.get_unit_guid().getlow32value(),
				// 						ruser2.get_unit_guid().gethigh32value(),ruser2.get_unit_guid().getlow32value());
			}
		}
		break;
		case e_marry_process_action3:
		{
			m_overtime++;
			if (m_overtime >= MARRY_STEP_INTERVAL)
			{
				//skill action
// 					packets::gc_ret_use_skill_pak retpak1;
// 					retpak1.m_packetdata.set_skillid(marry_process_action3);
// 					retpak1.m_packetdata.set_senderid(ruser1.getid());
// 					retpak1.m_packetdata.set_targetid(ruser2.getid());
// 					m_rscene.broadcast_insight_include(retpak1,ruser1.getid());
// 					packets::gc_ret_use_skill_pak retpak2;
// 					retpak2.m_packetdata.set_skillid(marry_process_action3);
// 					retpak2.m_packetdata.set_senderid(ruser2.getid());
// 					retpak2.m_packetdata.set_targetid(ruser1.getid());
// 					m_rscene.broadcast_insight_include(retpak2,ruser2.getid());

				m_overtime = 0;
				m_step = e_marry_process_over;

				// 					cachelog(logdef_inst(marrage),
				// 						"s3,u1[%08x,%08x],u2[%08x,%08x]",
				// 						ruser1.get_unit_guid().gethigh32value(),ruser1.get_unit_guid().getlow32value(),
				// 						ruser2.get_unit_guid().gethigh32value(),ruser2.get_unit_guid().getlow32value());
			}
		}
		break;
		case e_marry_process_over:
		{
			m_overtime++;
			if (m_overtime >= MARRY_STEP_INTERVAL)
			{
				//send effect
// 					ruser2.sendimpacttounit(ruser1,marry_process_over,0);
// 					ruser1.sendimpacttounit(ruser2,marry_process_over,0);

					//trans out
// 					ruser2.forcesetscenepos(marry_process_over_pos1);
// 					ruser1.forcesetscenepos(marry_process_over_pos2);

// 					ruser1.showactivefellow(call_aftermarry);
// 					ruser2.showactivefellow(call_aftermarry);

					//effect obj
// 					m_rscene.createyanhuaobj(marry_fireworks_id,0,marry_fireworks_pos1);
// 					m_rscene.createyanhuaobj(marry_fireworks_id,0,marry_fireworks_pos2);
// 					m_rscene.createyanhuaobj(marry_fireworks_id,0,marry_fireworks_pos3);
// 					m_rscene.createyanhuaobj(marry_fireworks_id,0,marry_fireworks_pos4);

					//sendbuff
// 					userrefcont cont;
// 					m_rscene.scan_user_all(cont);
// 					for (int32 i = 0; i < cont.size(); i++)
// 					{
// 						int32 nsharetimes =cont[i].getcommondata(user_commondata::cd_sharemarryexpandcoin);
// 						if (nsharetimes>=maxsharemarrycoinandexp)
// 						{
// 							cont[i].sendnotice("#{3175}");
// 							continue;
// 						}
// 						ruser1.sendimpacttounit(cont[i],marry_process_buff,0);
// 						cont[i].setcommondata(user_commondata::cd_sharemarryexpandcoin,nsharetimes+1);
// 					}

// 					cachelog(logdef_inst(marrage),
// 						"so,u1[%08x,%08x],u2[%08x,%08x]",
// 						ruser1.get_unit_guid().gethigh32value(),ruser1.get_unit_guid().getlow32value(),
// 						ruser2.get_unit_guid().gethigh32value(),ruser2.get_unit_guid().getlow32value());

					//roll notice
				xchar szmsg[256] = { 0 };
				if (ruser1->get_relation_list_mgr().get_lover_status() == marriage_data::e_lover_normal &&
					ruser2->get_relation_list_mgr().get_lover_status() == marriage_data::e_lover_normal)
				{
					// 						if ( ruser1.getcurprofession()%2 == 0 )
					// 						{
					// 							dictionaryformat::formatdictionary(szmsg, sizeof(szmsg), "#{3105}",
					// 								ruser1.getname().getctext(), ruser2.getname().getctext());
					// 						}
					// 						else
					// 						{
					// 							dictionaryformat::formatdictionary(szmsg, sizeof(szmsg), "#{3105}",
					// 								ruser2.getname().getctext(), ruser1.getname().getctext());
					// 						}
				}
				else if (ruser1->get_relation_list_mgr().get_lover_status() == marriage_data::e_lover_luxury &&
					ruser2->get_relation_list_mgr().get_lover_status() == marriage_data::e_lover_luxury)
				{
					// 						if ( ruser1.getcurprofession()%2 == 0 )
					// 						{
					// 							dictionaryformat::formatdictionary(szmsg, sizeof(szmsg), "#{3106}",
					// 								ruser1.getname().getctext(), ruser2.getname().getctext());
					// 						}
					// 						else
					// 						{
					// 							dictionaryformat::formatdictionary(szmsg, sizeof(szmsg), "#{3106}",
					// 								ruser2.getname().getctext(), ruser1.getname().getctext());
					// 						}
				}
				//sceneinterface::broadcastrollnotice(szmsg);

				cleanup();
			}
		}
		break;
		default:
			break;
		}
	}

	bool marry_process::isdoing()
	{
		if (m_step != e_marry_process_invalid)
		{
			return true;
		}
		return false;
	}

	void marriage_data::clean_up_data()
	{
		m_lover_guid.clear_data();
		m_lover_status = marriage_data::e_lover_nostatus;
		m_promising_id.clear_data();
		m_marrage_time = 0;
	}

	void marriage_data::serialize_from_db(s_relation_info(&datas)[MAX_FRIEND_NUM * e_relationlist_type_max])
	{
// 		m_lover_guid = rsour.m_dbloverguid;
// 		m_lover_status = rsour.m_dbloverstatus;
// 		m_marrage_time = rsour.m_dbtime;
	}

	void marriage_data::serialize_to_db(s_relation_info(&datas)[MAX_FRIEND_NUM * e_relationlist_type_max])
	{
// 		rdest.m_dbloverguid = m_lover_guid;
// 		rdest.m_dbloverstatus = m_lover_status;
// 		rdest.m_dbtime = m_marrage_time;
	}

	//////////////////////////////////////////////////////////////////////////

	//marry_parade::marry_parade(map_cs& rscene)
	//	:m_rscene(rscene)
	//{
	//	clean_up();
	//}

	void marry_parade::clean_up()
	{
		m_operator1.clear_data();
		m_operator2.clear_data();
		m_overtime = 0;
		//		std::vector<int32>::iterator it = m_npcids.begin();
		// 		for ( ; it != m_npcids.end(); ++it )
		// 		{
		// 			int32 nID =  *it;
		// 			if ( m_rscene.GetNpcByID(nID) )
		// 			{
		// 				m_rscene.DelNpcByID(nID);
		// 			}
		// 		}
		m_npcids.clear();
	}

	void marry_parade::heart_tick(const int64& new_time)
	{
		if ( /*rInfo.m_bDiffSecond &&*/ m_overtime >= 0)
		{
			m_overtime++;
			if (m_overtime > MARRY_PARADE_MAXTIME)
			{
				// 				CacheLog(LOGDEF_INST(Marrage),"TiEnd,U1(%08X,%08X),U2(%08X,%08X)",
				// 					m_operator1.GetHigh32Value(),m_operator1.GetLow32Value(),
				// 					m_operator2.GetHigh32Value(),m_operator2.GetLow32Value());
				end();
				clean_up();
			}
			std::vector<int32>::iterator it = m_npcids.begin();
			// 			for ( ; it != m_npcids.end(); ++it )
			// 			{
			// 				int32 nID =  *it;
			// 				if ( m_rscene.GetNpcByID(nID) )
			// 				{
			// 					break;
			// 				}
			// 			}
			if (it == m_npcids.end())
			{
				// 				CacheLog(LOGDEF_INST(Marrage),"ClrEnd,U1(%08X,%08X),U2(%08X,%08X)",
				// 					m_operator1.GetHigh32Value(),m_operator1.GetLow32Value(),
				// 					m_operator2.GetHigh32Value(),m_operator2.GetLow32Value());
				end();
				clean_up();
			}
		}
	}

	bool marry_parade::is_doing()
	{
		if (m_overtime >= 0)
		{
			return true;
		}
		return false;
	}

	void marry_parade::end()
	{
		client_session* pUser1 = client_session_mgr::getInstance().get_session(m_operator1);
		if (pUser1 == nullptr)
		{
			return;
		}
		client_session* pUser2 = client_session_mgr::getInstance().get_session(m_operator2);
		if (pUser2 == nullptr)
		{
			return;
		}
		// 		if ( pUser1.is_valid() )
		// 		{
		// 			pUser1.ShowActiveFellow(CALL_AFTERMARRY);
		// 		}
		// 		if ( pUser2.is_valid())
		// 		{
		// 			pUser2.ShowActiveFellow(CALL_AFTERMARRY);
		// 		}
	}

	void marry_parade::start(const guid_64& op1, const guid_64& op2)
	{
		if (is_doing())
		{
			return;
		}
		// 		if ( !_GameConfig().m_bIsOpenMarrage )
		// 		{
		// 			return;
		// 		}
		client_session* pUser1 = client_session_mgr::getInstance().get_session(op1);
		if (pUser1 == nullptr)
		{
			return;
		}
		client_session* pUser2 = client_session_mgr::getInstance().get_session(op2);
		if (pUser2 == nullptr)
		{
			return;
		}
		if (pUser1->is_vaild() && pUser2->is_vaild())
		{
			m_operator1 = pUser1->get_role_guid();
			m_operator2 = pUser2->get_role_guid();

			// 			CacheLog(LOGDEF_INST(Marrage),"StartPar,U1(%08X,%08X),U2(%08X,%08X)",
			// 				m_operator1.GetHigh32Value(),m_operator1.GetLow32Value(),
			// 				m_operator2.GetHigh32Value(),m_operator2.GetLow32Value());

			// 			player* obj1 = m_rscene.CreateNpc(MARRY_PARADE_FRONTNPC,	pUser1->GetScenePos(),	false);
			// 			if (obj1)
			// 			{
			// 				m_npcids.push_back(obj1->GetID());
			// 				obj1->StartPatrol(*obj1,MARRY_PARADE_FRONTWAY);
			// 			}
			// 			player* obj2 = m_rscene.CreateNpc(MARRY_PARADE_BUSNPC,	pUser1->GetScenePos(),	false);
			// 			if (obj2)
			// 			{
			// 				m_npcids.push_back(obj2->GetID());
			// 				obj2->StartPatrol(*obj2,MARRY_PARADE_BUSWAY);
			// 				obj2->BindObj(op1);
			// 				obj2->BindObj(op2);
			// 				pUser1->PlayStory(MARRY_PARADE_STORY);
			// 				pUser2->PlayStory(MARRY_PARADE_STORY);
			// 				pUser1->SingleMount_UnMount();
			// 				pUser1->MarkAutoMount(0);
			// 				pUser2->SingleMount_UnMount();
			// 				pUser2->MarkAutoMount(0);
			// 				pUser1->HideActiveFellow();
			// 				pUser2->HideActiveFellow();

			// 				CacheLog(LOGDEF_INST(Marrage),"Bind,U1(%08X,%08X),U2(%08X,%08X)",
			// 					m_operator1.GetHigh32Value(),m_operator1.GetLow32Value(),
			// 					m_operator2.GetHigh32Value(),m_operator2.GetLow32Value());
			//			}
			// 			player* obj3 = m_rscene.CreateNpc(MARRY_PARADE_BACKNPC,	pUser1->GetScenePos(),	false);
			// 			if (obj3)
			// 			{
			// 				m_npcids.push_back(obj3->GetID());
			// 				obj3->StartPatrol(*obj3,MARRY_PARADE_BACKWAY);
			// 			}
			m_overtime = 0;
		}
	}

	void relation_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		bool is_change = false;
		for (int32 i = 0; i < e_relationlist_type_max; ++i)
		{
			relation* relation_ptr = m_releation_list[i].get_relation(role_guid);
			if (nullptr == relation_ptr)
			{
				continue;
			}

			is_change = true;
			xchar char_role_name[max_name_size + 1] = { 0 };
			memcpy(char_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
			relation_ptr->set_data_name(char_role_name);
		}

		if (is_change)
		{
			sync_relation_list_to_client();
		}	
	}

	void relation_mgr::send_change_name_mail_to_all_friend(const xstring& original_name, const xstring& current_name)
	{
		std::vector<std::string> title_params;
		title_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_change_name_inform_friend_title));
		std::string title_text = init_unit::implode(title_params);
		std::vector<std::string> content_params;
		content_params.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_change_name_inform_friend_content));
		content_params.push_back(original_name);
		content_params.push_back(current_name);
		std::string content_text = init_unit::implode(content_params);
		std::vector<s_item_template_info> drop_item_list;

		relation_list rel_list = m_releation_list[e_relationlist_type_friend].get_relation_list();
		auto iter = rel_list.begin();
		for (; iter != rel_list.end(); ++iter)
		{
			mail_ws_mgr::get_instance().send_mail_system(iter->get_data_guid(), 0, drop_item_list, title_text, content_text);
		}
			
		
	}

	void relation_mgr::add_relation_to_dp(guid_64 role_guid, s_relation_info relation_info )
	{
		//ws2dp_add_relation msg;
		//msg.role_guid = role_guid;
		//msg.relation_guid = relation_info.relation_guid;
		//msg.set_relation_name(relation_info.relation_name);
		//msg.relation_gs_value = relation_info.gs_value;
		//msg.data_array[e_relation_data_type] = relation_info.data_array[e_relation_data_type];
		//msg.data_array[e_relation_data_level] = relation_info.data_array[e_relation_data_level];
		//msg.data_array[e_relation_data_template_id] = relation_info.data_array[e_relation_data_template_id];
		//msg.data_array[e_relation_data_relation_state] = relation_info.data_array[e_relation_data_relation_state];
		//msg.data_array[e_relation_data_last_login_time] = relation_info.data_array[e_relation_data_last_login_time];
		//msg.data_array[e_relation_data_friendliness] = relation_info.data_array[e_relation_data_friendliness];
		//msg.data_array[e_relation_data_vip_title_template_id] = relation_info.data_array[e_relation_data_vip_title_template_id];

		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));

		server2dp_proto_ws2dp_add_relation req;
		req.set_role_guid(role_guid.server_64);
		req.set_relation_guid(relation_info.relation_guid.server_64);
		req.set_relation_name(relation_info.relation_name);
		req.set_relation_gs_value(relation_info.gs_value);
		for (int32 i = e_relation_data_type; i < e_relation_data_max; i++)
		{
			req.add_data_array(relation_info.data_array[i]);
		}
		ws_client::getInstance().send_to_dp_new(&req, e_msg_ws2dp_add_relation, 0);

	}

	void relation_mgr::del_relation_to_dp(guid_64 role_guid, guid_64 target_role_guid, int32 relation_type)
	{
		//ws2dp_del_relation msg;
		//msg.role_guid = role_guid;
		//msg.target_role_guid = target_role_guid;
		//msg.relation_type = relation_type;

		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));

		script_mgr::get_instance().call_func("ws2dp_send", "send_ws2dp_req_del_relation_lua", 0, false, "%l%l%d", role_guid.server_64, target_role_guid.server_64, relation_type);

	}

	void relation_mgr::add_friend_success_chat_and_send_advices(guid_64 target_guid, s_relation_info relation_info)
	{
		if (false == target_guid.is_valid())
		{
			return;
		}
		if (nullptr == m_client_session_ptr)
		{
			return;
		}

		relation_info.data_array[e_relation_data_type] = e_relationlist_type_chat;
		client_session* target_client_ptr = client_session_mgr::getInstance().get_session(target_guid);
		if (nullptr == target_client_ptr)
		{
			add_relation_to_dp(target_guid, relation_info);

			ws2dp_save_chat_with_offline offline_msg;
			offline_msg.sender_guid = m_client_session_ptr->get_role_guid();
			offline_msg.sender_templete_id = m_client_session_ptr->get_role_info_data(e_role_info_template_id);
			offline_msg.chat_type = e_chat_type_private;
			offline_msg.address_guid = target_guid;
			offline_msg.set_sender_name(m_client_session_ptr->get_role_name());
			offline_msg.relation_template_id = m_client_session_ptr->get_role_info_data(e_role_info_template_id);
			offline_msg.relation_level = m_client_session_ptr->get_role_info_data(e_role_info_exp_level);
			offline_msg.chat_guid = guid_gen::make_guid().server_64;
			offline_msg.vip_title_template_id = get_vip_title_template_id(m_client_session_ptr);
			offline_msg.head_frame_id = m_client_session_ptr->get_role_info_data(e_role_info_head_frame);
			const std::vector<std::string>& temp_string_array_list_first = template_manager::get_instance().get_string_list(add_friend_success_content_id);
			if (temp_string_array_list_first.empty())
			{
				return;
			}
			if (0 >= temp_string_array_list_first.size())
			{
				return;
			}
			xstring chat_content = init_unit::implode(temp_string_array_list_first);
			offline_msg.set_content_text(chat_content);

			ws_client::getInstance().send_to_dp(&offline_msg, sizeof(offline_msg));
			return;
		}
		if ( false == target_client_ptr->get_relation_list_mgr().is_relation_exist(e_relationlist_type_chat, relation_info.relation_guid))
		{
			target_client_ptr->get_relation_list_mgr().add_unit_to_relation_list(relation_info, false, false);
		}
		

		chat_proto_chat_content msg;
		msg.set_chat_typ(e_chat_type_private);
		msg.set_sender_guid(m_client_session_ptr->get_role_guid());
		msg.set_sender_template_id(m_client_session_ptr->get_role_info_data(e_role_info_template_id));
		msg.set_sender_name(m_client_session_ptr->get_role_name());
		msg.set_relation_template_id(m_client_session_ptr->get_role_info_data(e_role_info_template_id));
		msg.set_relation_level(m_client_session_ptr->get_role_info_data(e_role_info_exp_level));
		msg.set_chat_guid(guid_gen::make_guid().server_64);
		msg.set_vip_title_template_id(get_vip_title_template_id(m_client_session_ptr));
		const std::vector<std::string>& temp_string_array_list = template_manager::get_instance().get_string_list(add_friend_success_content_id);
		if (temp_string_array_list.empty())
		{
			return;
		}
		if (0 >= temp_string_array_list.size())
		{
			return;
		}
	
		for (int32 j = 0; j < temp_string_array_list.size(); ++j)
		{
			chat_proto_chat_content_info* temp_info_msg = msg.add_content_info_array();

			temp_info_msg->set_commontext(temp_string_array_list[j]);
		}
		msg.set_is_finish_send(true);
		msg.add_addressee_guid(target_guid.A);
		msg.add_addressee_guid(target_guid.B);

		target_client_ptr->send_to_client(&msg, e_msgindex_s2c_receive_chat_new);
	}

	faith::int32 relation_mgr::get_vip_title_template_id(client_session* client_session_ptr)
	{
		for (int32 vip_level = client_session_ptr->m_role_info.data_ary[e_role_info_vip_level]; vip_level >= 0; vip_level--)
		{
			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
			if (nullptr == vip_template_ptr || (vip_template_ptr->ItemGetArray.size() % 2 != 0))
			{
				continue;
			}
			int32 total_item_num = vip_template_ptr->ItemGetArray.size() / 2;
			for (int32 i = 0; i < total_item_num; ++i)
			{
				int32 item_id = vip_template_ptr->ItemGetArray[i * 2];
				ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
				if (temp_template_ptr == nullptr)
				{
					continue;
				}
				PropBasicAttributeTemplate* prop_template = GET_TEMPLATE(PropBasicAttributeTemplate, temp_template_ptr->logic_id);
				if (prop_template == nullptr)
				{
					continue;
				}
				if (prop_template->TitleGet > 0)
				{
					return prop_template->TitleGet;
				}
			}
		}
		return 0;
	}
	//void relation_mgr::send_relation_info_one(guid_64 member_guid, e_relation_data info_index)
	//{
		//s_legion_member_info* member_info = get_member(member_guid);
		//if (nullptr == member_info)
		//{
		//	return;
		//}
		//if (info_index < 0 || info_index >= e_legion_member_info_max)
		//{
		//	return;
		//}
		//int32 info_value = member_info->data_ary[info_index];

		//legion_proto_update_legion_member_info_one update_legion_member_info_one_to_client_msg;
		//update_legion_member_info_one_to_client_msg.set_member_guid(member_guid.server_64);
		//update_legion_member_info_one_to_client_msg.set_info_index(info_index);
		//update_legion_member_info_one_to_client_msg.set_info_value(info_value);
		//send_message_to_all_member(&update_legion_member_info_one_to_client_msg, e_msgindex_s2c_update_legion_member_info_one);
	//}
}