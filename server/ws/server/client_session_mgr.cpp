/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   14:57
	file base:	client_session_mgr
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "client_session_mgr.hpp"
#include "queued_login_mgr.h"
#include "team/team_ws_mgr.h"
#include "assist_fight/assist_fight_mgr.h"
#include "legion/legion_ws_mgr.h"
#include "server_log.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "csv_synchronization_mgr.h"
#include "game_cfg/servers_config.h"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "login_msg.hpp"
#include "net.pb.h"

namespace faith
{
	client_session_mgr::client_session_mgr(void)
	{
		clear_data();
	}

	client_session_mgr::~client_session_mgr(void)
	{
	}
	void client_session_mgr::init()
	{
		for (int32 i = 0; i < init_session_max; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			client_session_ref.clear_data();
			client_session_ref.set_array_index(i);
		}
		m_last_transfer_state = -1;
	}
	void client_session_mgr::clear_data()
	{
		for (int32 i = 0; i < init_session_max; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			client_session_ref.clear_data();
		}
		m_session_array_use = 0;
		m_session_array_num = 0;
		m_session_game_num = 0;
		m_empty_legion_info.clear_data();
	}
	client_session* client_session_mgr::get_empty_session()
	{
		int32 empty_index = -1;
		for (int32 i = 0; i < init_session_max; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			if (client_session_ref.is_vaild() == false)
			{
				client_session_ref.set_data_use(true);
				m_session_array_num++;
				if (i > m_session_array_use)
				{
					m_session_array_use = i;
				}
				return &client_session_ref;
			}
		}
		return nullptr;
	}
	void client_session_mgr::remove_session(int32 session_index)
	{
		if (session_index < 0 || session_index >= init_session_max)
		{
			return;
		}
		if (m_session_array[session_index].is_vaild())
		{

			m_session_array[session_index].clear_data();
			m_session_array_num--;
			if (session_index >= m_session_array_use)
			{
				m_session_array_use = session_index - 1;
			}
		}
	}

	client_session* client_session_mgr::get_session(s_client_uid client_uid)
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			if (client_session_ref.is_vaild() && client_session_ref.get_client_uid() == client_uid)
			{
				return &client_session_ref;
			}
		}
		return nullptr;
	}

	client_session* client_session_mgr::get_session(const xstring account )
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			if (client_session_ref.is_vaild() && account == client_session_ref.m_account)
			{
				return &client_session_ref;
			}
		}
		return nullptr;
	}

	client_session* client_session_mgr::get_session(const guid_64& role_guid)
	{ 
		if (role_guid.is_valid() == false)
		{
			return nullptr;
		}

		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session& client_session_ref = m_session_array[i]; 
			if (client_session_ref.is_vaild() && client_session_ref.get_role_guid() == role_guid)
			{
				return &client_session_ref;
			}
		}
		return nullptr;
	}

	client_session* client_session_mgr::get_session_by_role_name( xstring p_role_name )
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			if (client_session_ref.is_vaild() && client_session_ref.m_role_info.role_name == p_role_name)
			{
				return &client_session_ref;
			}
		}
		return nullptr;
	}

	client_session* client_session_mgr::get_session_by_array_index(int32 array_index)
	{
		if (array_index < 0 || array_index >= init_session_max)
		{
			return nullptr;
		}
		return &(m_session_array[array_index]);
	}

	void client_session_mgr::remove_session(s_client_uid client_uid)
	{
		client_session* client_session_ptr = get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		remove_session(client_session_ptr->get_array_index());
	}

	void client_session_mgr::remove_session(xstring account)
	{
		client_session* client_session_ptr = get_session(account);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		remove_session(client_session_ptr->get_array_index());
	}

	void client_session_mgr::remove_session(const guid_64& role_guid)
	{
		client_session* client_session_ptr = get_session(role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		remove_session(client_session_ptr->get_array_index());
	}
	//	无条件的，保存、清除此玩家在整个游戏服务器组内的所有数据
	void client_session_mgr::logout_client(client_session* client_session_ptr)
	{
		if( !client_session_ptr )
			return;

		switch(client_session_ptr->m_status)
		{
		case client_session::e_ss_created:
		case client_session::e_ss_login_ok:
		case client_session::e_ss_ingame:
		case client_session::e_ss_queue:
		case client_session::e_ss_map_transfer:
		case client_session::e_ss_cross_transfer:
		case client_session::e_ss_check_token:
			{
				if(client_session_ptr->get_cs_array_index() >= 0)
				{
					client_session_ptr->m_status = client_session::e_ss_logout;
				}
				else
				{
					logout_complete(client_session_ptr);
				}
			}
			break;
		default:
			return;
		}

		client_session_ptr->m_status = client_session::e_ss_logout;
	}

	void client_session_mgr::logout_complete(client_session* client_session_ptr)
	{
		if (nullptr == client_session_ptr)
		{
			return;
		}	

		queued_login_mgr::getInstance().on_session_logout(client_session_ptr);
		if (client_session_ptr->m_online_state != client_session::e_os_online)
		{
			remove_session(client_session_ptr->m_client_uid);
			return;
		}

		guid_64 map_guid = client_session_ptr->get_map_guid();

		guid_64 role_guid = client_session_ptr->get_role_guid();
		cs_map_mgr_system::remove_unit_from_map(map_guid, role_guid);

		//从带结婚列表中删除
		marry_mgr_ws::get_instance().session_offline_marry_handle(role_guid);
		// 玩家下线的时候要通知队伍
		team_ws_mgr::get_instance().on_player_logout(client_session_ptr);

		assist_fight_mgr::get_instance().on_player_logout(role_guid);

		// 玩家下线时候要把军团中的在线状态置为离线
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr != legion_ws_ptr)
		{
			legion_ws_ptr->on_legion_member_logout(client_session_ptr);
		}

		//玩家下线时候保存玩家好友列表
		client_session_ptr->get_relation_list_mgr().save_relation_to_db(false);
		remove_session(client_session_ptr->m_client_uid);
	}

	void client_session_mgr::login_client(const void* data)
	{
		const fep2ws_client_logined* packet= static_cast<const fep2ws_client_logined*>(data);
		if(nullptr == packet)
		{
			CONSOLE_ERROR("packet is nullptr");
			return;
		}
		// 创建session
		client_session *  client_session_ptr = get_empty_session();
		if(client_session_ptr == NULL)
		{
			CONSOLE_ERROR("get empty session fail");
			//发送错误消息到FEP
			ws2fep_client_logined rep;
			rep.client_uid = packet->client_uid;
			rep.login_type = packet->login_type;
			rep.login_type_plus = packet->login_type_plus;
			rep.eResult = e_error_code_login_login_queue_full;
			world_server::getInstance().broadcast(&rep, sizeof(rep), e_server_type_fep);
			return;
		}
		int32 server_id = packet->server_id;
		if (server_id <= 0)
		{
			server_id = world_server::getInstance().get_server_id();
		}

		client_session_ptr->m_client_uid = packet->client_uid;
		client_session_ptr->m_login_type = packet->login_type;
		client_session_ptr->m_login_type_plus = packet->login_type_plus;
		client_session_ptr->m_step_num = client_session::e_session_step_login;
		if (packet->role_guid.is_valid())
		{
			client_session_ptr->set_role_guid(packet->role_guid);
		}
		if (packet->map_guid.is_valid())
		{
			client_session_ptr->m_transfer_info.map_guid = packet->map_guid;
			client_session_ptr->m_transfer_info.war_index = packet->war_index;
		}
		memcpy(client_session_ptr->m_account, packet->account, max_account_length);
		memcpy(client_session_ptr->m_ban_role_array, packet->ban_role_array, sizeof(client_session_ptr->m_ban_role_array));
		memcpy(client_session_ptr->m_ban_chat_array, packet->ban_chat_array, sizeof(client_session_ptr->m_ban_chat_array));
		memcpy(&client_session_ptr->m_login_third_data,&packet->fixed_data,sizeof(login_fixed_data));
		client_session_ptr->m_logintime = time_helper::get_cur_time_new().second;
		client_session_ptr->set_is_already_login(false);
		client_session_ptr->m_jewel_num = packet->jewel_num;
		client_session_ptr->m_role_info.data_ary[e_role_info_server_id] = server_id;
		client_session_ptr->check_is_robot();
		queued_login_mgr::getInstance().on_session_login(client_session_ptr, (e_login_type_new)packet->login_type);
	}

	void client_session_mgr::tick(const int64& new_time)
	{
		m_session_game_num = 0;
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			if (client_session_ref.is_vaild())
			{
				client_session_ref.tick(new_time);
				if (client_session_ref.m_cs_array_index > 0)
				{
					m_session_game_num++;
				}
			}
		}
		static int32 tick_min_last = 0;
		int32 tick_min_cur = new_time / minute_tick_time;
		if (tick_min_cur > tick_min_last)
		{
			tick_min_last = tick_min_cur;

			for (auto ite = m_cross_player_legion_info_map.begin(); ite != m_cross_player_legion_info_map.end(); )
			{
				// tick里可能删掉 要用这种方式防止迭代器失效
				auto  temp_it = ite++;
				if (temp_it->second.save_time_stamp + cross_server_player_legion_save_time <= time_helper::get_cur_time_new().second)
				{
					m_cross_player_legion_info_map.erase(temp_it);
				}
			}

			int32 cur_transfer_state = is_can_transfer_other_line();
			if (cur_transfer_state != m_last_transfer_state)
			{
				m_last_transfer_state = cur_transfer_state;
				send_transfer_state_to_client();
			}
		}
	}
	channel_cout_map client_session_mgr::get_channel_num()
	{
		channel_cout_map channel_map;
		channel_map["111092021901"] = 0;
		channel_map["131092101103"] = 0;
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session& client_session_ref = m_session_array[i];
			if (client_session_ref.is_vaild() && client_session_ref.m_cs_array_index > 0)
			{
				channel_map[client_session_ref.m_login_third_data.param7] += 1;
			}
		}
		return channel_map;
	}
	void client_session_mgr::logout_by_fep(uint32 fep_uid)
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session* session = get_session_by_array_index(i);
			if (session->m_client_uid.fepserver_uid == fep_uid)
				logout_client(session);
		}
	}

	void client_session_mgr::logout_by_cs(uint32 conn_index)
	{
		cs_map_mgr_system::remove_map_by_uid(conn_index);

		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			client_session* session = get_session_by_array_index(i);
			if (session->get_cs_conn_index() == conn_index)
			{
				session->set_cs_conn_index(-1);
				kickout_account(session->m_account,e_logout_result_cs_crash);
			}
		}
	}

	bool client_session_mgr::kickout_account(xstring role_mark, e_logout_result reason)
	{
		client_session* client_session_ptr = get_session(role_mark);
		if (client_session_ptr != NULL)
		{
			ws2fep_kickout_account msg;
			msg.client_uid = client_session_ptr->m_client_uid;
			msg.reason = reason;
			client_session_ptr->send_to_fep(&msg,sizeof(msg));
			logout_client(client_session_ptr);
			return true;
		}
		return false;
	}

	bool client_session_mgr::kickout_account(guid_64 role_mark, e_logout_result reason)
	{
		client_session* client_session_ptr = get_session(role_mark);
		if (client_session_ptr != NULL)
		{
			ws2fep_kickout_account msg;
			msg.client_uid = client_session_ptr->m_client_uid;
			msg.reason = reason;
			client_session_ptr->send_to_fep(&msg, sizeof(msg));
			logout_client(client_session_ptr);
			return true;
		}
		return false;
	}
	bool client_session_mgr::kickout_account(s_client_uid role_mark, e_logout_result reason)
	{
		client_session* client_session_ptr = get_session(role_mark);
		if (client_session_ptr != NULL)
		{
			ws2fep_kickout_account msg;
			msg.client_uid = client_session_ptr->m_client_uid;
			msg.reason = reason;
			client_session_ptr->send_to_fep(&msg, sizeof(msg));
			logout_client(client_session_ptr);
			return true;
		}
		return false;
	}

	void client_session_mgr::send_message_to_all_server_client(google::protobuf::Message* net_pro, uint32 header, uint32 gate_header)
	{
		send_message_to_all_client(net_pro, header);
		cross::send_msg_to_ws(guid_64(), 0, gate_header, net_pro);

	}
	void client_session_mgr::send_message_to_all_area_server_client(google::protobuf::Message* net_pro, uint32 header, uint32 gate_header)
	{
		send_message_to_all_client(net_pro, header);
		cross::send_msg_to_ws(guid_64(), -1, gate_header, net_pro);
	}

	void client_session_mgr::send_message_to_all_client(google::protobuf::Message* net_pro, uint32 header)
	{
		m_broadcast_msg.wheader = e_msg_index_ws2fep_broadcast_msg;
		m_broadcast_msg.header = header;
		m_broadcast_msg.data_size = net_pro->ByteSize();
		if (net_pro->SerializeToArray(m_broadcast_msg.data, sizeof(m_broadcast_msg.data)))
		{
			world_server::getInstance().broadcast(&m_broadcast_msg, sizeof(m_broadcast_msg), e_server_type_fep);
		}
	}



	void client_session_mgr::send_message_to_all_client_data(const void* data_package, size_t data_len, uint32 header)
	{
		m_broadcast_msg.wheader = e_msg_index_ws2fep_broadcast_msg;
		m_broadcast_msg.header = header;
		m_broadcast_msg.data_size = data_len;
		memcpy(m_broadcast_msg.data, data_package, sizeof(m_broadcast_msg.data) > data_len ? data_len : sizeof(m_broadcast_msg.data));
		world_server::getInstance().broadcast(&m_broadcast_msg, sizeof(m_broadcast_msg), e_server_type_fep);
	
	}

	void client_session_mgr::send_message_to_all_client_data_lua(const char * msg, int32 msg_len, uint32 header)
	{
		send_message_to_all_client_data(msg, msg_len, header);
	}

	void client_session_mgr::send_message_to_all_cs(const void* data_package, size_t data_len)
	{
		world_server::getInstance().broadcast(data_package, data_len, e_server_type_cs);
	}

	void client_session_mgr::send_message_to_map(int32 map_template_id, google::protobuf::Message* net_pro, uint32 header)
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			if (!m_session_array[i].is_vaild())
			{
				continue;
			}
			if (m_session_array[i].m_role_info.data_ary[e_role_info_move_map_id] != map_template_id)
			{
				continue;
			}
			m_session_array[i].send_to_client(net_pro, header);
		}
	}

	void client_session_mgr::send_message_to_map_lua(int32 map_template_id, const char * msg, int32 msg_len, uint32 header)
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			if (!m_session_array[i].is_vaild())
			{
				continue;
			}
			if (m_session_array[i].m_role_info.data_ary[e_role_info_move_map_id] != map_template_id)
			{
				continue;
			}
			m_session_array[i].send_to_client(msg, msg_len, header);
		}
	}

	void client_session_mgr::send_temp_to_all_session()
	{
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			if (!m_session_array[i].is_vaild())
			{
				continue;
			}
			if (m_session_array[i].get_role_info_data(e_role_info_server_id) != SERVERCONFIG->game_id)
			{
				continue;
			}
			csv_synchronization_mgr::get_instance().send_template(&m_session_array[i]);
		}
	}

	void client_session_mgr::send_message_to_legion(guid_64 send_guid, google::protobuf::Message* net_pro, uint32 header)
	{
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross))
		{
			return;
		}
		client_session* client_session_ptr = get_session(send_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		guid_64 legion_guid = client_session_ptr->get_legion_guid();
		cross::send_msg_to_ws(legion_guid, 0, e_msgindex_ws2ws_chat_server_legion, net_pro);
	}

	void client_session_mgr::send_message_to_cur_server_legion(guid_64 send_guid, google::protobuf::Message* net_pro, uint32 header)
	{
		client_session* client_session_ptr = get_session(send_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		guid_64 legion_guid = client_session_ptr->get_legion_guid();
		if (false == legion_guid.is_valid())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr != legion_ws_ptr)
		{
			legion_ws_ptr->send_message_to_all_member(net_pro, header);
			return;
		}
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross))
		{
			return;
		}
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			if (!m_session_array[i].is_vaild())
			{
				continue;
			}
			if (m_session_array[i].get_legion_guid() != legion_guid)
			{
				continue;
			}
			m_session_array[i].send_to_client(net_pro, header);
		}
	}

	void client_session_mgr::send_message_to_legion_data(guid_64 legion_guid, const void* data_package, size_t data_len, uint32 header)
	{
		if (false == legion_guid.is_valid())
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr != legion_ws_ptr)
		{
			legion_ws_ptr->send_message_to_all_member_data(data_package, data_len, header);
			return;
		}
		for (int32 i = 0; i <= m_session_array_use; ++i)
		{
			if (!m_session_array[i].is_vaild())
			{
				continue;
			}
			if (m_session_array[i].get_legion_guid() != legion_guid)
			{
				continue;
			}
			m_session_array[i].send_to_client(data_package, data_len, header);
		}
	}

	int32 client_session_mgr::is_can_transfer_other_line()
	{
		auto map_ent = cs_map_mgr_system::get_map_ws_by_line(big_world_map_id, 2);
		if (nullptr != map_ent)
		{
			if (cs_map_system::get_unit_count(map_ent) > 0)
			{
				return 1;
			}
		}
		auto new_map_ent = cs_map_mgr_system::get_map_ws_by_line(big_world_map_id, 2);
		if (nullptr != new_map_ent)
		{
			if (cs_map_system::get_unit_count(new_map_ent) > 0)
			{
				return 1;
			}
		}
		if (m_session_array_num > can_show_other_line_session_num)
		{
			return 1;
		}
		return 0;
	}

	void client_session_mgr::send_transfer_state_to_client(client_session* cur_session)
	{
		game_proto_is_can_transfer_other_line msg;
		msg.set_transfer_state(m_last_transfer_state);
		if (nullptr == cur_session)
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_can_transfer_other_line);
			return;
		}
		cur_session->send_to_client(&msg, e_msgindex_s2c_can_transfer_other_line);
	}

	

	void client_session_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{

		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (player_session != nullptr)
		{
			xchar char_role_name[max_name_size + 1] = { 0 };
			memcpy(char_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
			player_session->set_role_name(char_role_name);
			legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
			if (city_war_mgr.get_city_master_guid() == role_guid)
			{
				city_war_mgr.set_city_master_name(char_role_name, true);
				city_war_mgr.save_city_info_into_db(city_war_mgr.get_overlord_legion(), char_role_name);
			}
		}

		client_session* all_session_map = client_session_mgr::getInstance().get_all_session();
		for (int32 i = 0; i <= client_session_mgr::getInstance().get_session_array_used(); ++i)
		{
			client_session& client_session_ref = all_session_map[i];
			if (client_session_ref.is_vaild() == false)
			{
				continue;
			}
			client_session_ref.get_relation_list_mgr().change_player_name_func(role_guid, role_name);
		}
	}

	void client_session_mgr::set_cross_player_legion(const s_cross_player_legion_info& legion_info, guid_64& role_guid)
	{
		if (m_cross_player_legion_info_map.find(role_guid.server_64) != m_cross_player_legion_info_map.end())
		{
			m_cross_player_legion_info_map[role_guid.server_64] = legion_info;
			return;
		}
		m_cross_player_legion_info_map.insert({ role_guid.server_64, legion_info });
	}

	s_player_legion_info& client_session_mgr::get_cross_player_legion(guid_64 role_guid)
	{
		if (m_cross_player_legion_info_map.find(role_guid.server_64) != m_cross_player_legion_info_map.end())
		{
			return m_cross_player_legion_info_map[role_guid.server_64].legion_info;
		}
		return m_empty_legion_info;
	}

	void client_session_mgr::send_msg_to_cross_player(const google::protobuf::Message* proto_ptr, int32 server_id, guid_64 role_guid, uint32 header)
	{
		if (nullptr == proto_ptr)
		{
			return;
		}
		cross::send_msg_to_ws(role_guid, server_id, e_msgindex_ws2ws_send_cross_proto_msg, proto_ptr, header);
	}

	client_session * client_session_mgr::get_session_by_use_index(int32 index)
	{
		if (index < 0 || index > m_session_array_use)
		{
			return nullptr;
		}
		return m_session_array + index;
	}

}

