/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   14:56
	file base:	client_session_mgr
	file ext:	hpp
	author:		lucifer~yu
	
	purpose:	
*********************************************************************/
#ifndef _WS_CLIENT_SESSION_MANAGER_H_
#define _WS_CLIENT_SESSION_MANAGER_H_

#include <singleton.hpp>
#include "server_def.hpp"
#include "client_session.hpp"
#include "Logic/count_def.hpp"
#include "core.hpp"

namespace hld
{
	//	
	//	客户端会话管理器
	//

	typedef	std::unordered_map<ui64, client_session*>	player_session_map;
	typedef player_session_map::iterator	player_session_map_it;

	class client_session_mgr : public singleton<client_session_mgr>
	{
		friend class singleton<client_session_mgr>;
	private:
		client_session_mgr(void);
	public:	
		~client_session_mgr(void);
	public:
		void							tick(const int64& new_time);
		void							init();
		void							clear_data();
	public:
		void							remove_session(s_client_uid client_uid);
		void							remove_session(xstring account);
		void							remove_session(const guid_64& role_guid);
		void							remove_session(int32 session_index);
		client_session*					get_session(s_client_uid client_uid);
		client_session*					get_session(const xstring account);
		client_session*					get_session(const guid_64& role_guid);
		client_session*					get_session_by_role_name(xstring p_role_name);
		client_session*					get_session_by_array_index(int32 array_index);


		void							logout_by_fep( uint32 fep_uid );					//当与fep的连接断开时，登出所有相关的帐号
		void							logout_by_cs( uint32 conn_index);						//当与cs的连接断开时，登出所有相关的帐号
		void							logout_client( client_session* client_session_ptr);
		void							logout_complete( client_session* client_session_ptr );
		bool							kickout_account(xstring role_mark, e_logout_result reason);
		bool							kickout_account(guid_64 role_mark, e_logout_result reason);
		bool							kickout_account(s_client_uid role_mark, e_logout_result reason);
		
		void							login_client(const void* data);
		int32							get_session_num() { return m_session_array_num; }
		int32							get_game_num() { return m_session_game_num; }
		channel_cout_map				get_channel_num();

		client_session*					get_all_session() { return m_session_array; };
		int32							get_session_array_used() { return m_session_array_use; };
		void							send_message_to_all_client(google::protobuf::Message* net_pro, uint32 header);
		void							send_message_to_all_client_data(const void* data_package, size_t data_len, uint32 header);
		void							send_message_to_all_client_data_lua(const char * msg, int32 msg_len, uint32 header);
		void							send_message_to_all_cs(const void* data_package, size_t data_len);
		void							send_message_to_map(int32 map_template_id, google::protobuf::Message* net_pro, uint32 header);
		void                            send_message_to_map_lua(int32 map_template_id, const char *msg, int32 msg_len, uint32 header);

		//给对应军团GUID的成员发消息，目前主要用于跨服
		void							send_message_to_cur_server_legion(guid_64 send_guid, google::protobuf::Message* net_pro, uint32 header);
		void							send_message_to_legion(guid_64 send_guid, google::protobuf::Message* net_pro, uint32 header);
		void							send_message_to_legion_data(guid_64 legion_guid, const void* data_package, size_t data_len, uint32 header);
		void							send_message_to_all_server_client(google::protobuf::Message* net_pro, uint32 header, uint32 gate_header);
		void							send_message_to_all_area_server_client(google::protobuf::Message* net_pro, uint32 header, uint32 gate_header);

		void							set_cross_player_legion(const s_cross_player_legion_info& legion_info, guid_64& role_guid);
		s_player_legion_info&			get_cross_player_legion(guid_64 role_guid);

		void							send_msg_to_cross_player(const google::protobuf::Message* proto_ptr, int32 server_id, guid_64 role_guid, uint32 header);//跨服发送直接传给客户端的消息包

		void							send_temp_to_all_session();
	public:
		client_session*					get_empty_session();
		//rename
		void							change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步

		int32							is_can_transfer_other_line();
		void							send_transfer_state_to_client(client_session* cur_session = nullptr);
	public:
		client_session*                 get_session_by_use_index(int32 index);
	private:
		client_session					m_session_array[init_session_max];
		int32							m_session_array_use;
		int32							m_session_array_num;
		int32							m_session_game_num;
		ws2fep_broadcast_msg			m_broadcast_msg;
		player_session_map				m_guid_session_map;
		s_player_legion_info			m_empty_legion_info;

		int32							m_last_transfer_state;

		//军团数据缓存，对应玩家guid
		std::map<int64, s_cross_player_legion_info>	m_cross_player_legion_info_map;
	};
}


#endif