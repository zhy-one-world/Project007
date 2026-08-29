/********************************************************************
  created: 2014/08/20
  created: 20:8:2014 9:50
  file base: lua_call_handle
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "lua_globle_data.h"
#include "server/client_session_mgr.hpp"
#include "world_server.hpp"
#include "logic/type_def.hpp"
#include  "utility/parse_msg.h"



namespace faith
{
#define Lua_g_var_num(type,name, num)    type  g_lua_##name##_array[num];  int32 g_##name##_idx = 0; \
       static  type*   get_##name##_var()                                 \
        {                                                        \
	       int32 cur_idx = g_##name##_idx++;                      \
	       if (g_##name##_idx == num)                              \
	       {                                                        \
	          g_##name##_idx = 0;                                   \
	       }                                                        \
	       type* tmp_ptr = g_lua_##name##_array + cur_idx;           \
	       return tmp_ptr;                                          \
         }                                                          \

	/////////////////////////////////////////////////////////////////////////////////////
	//临时lua guid变量
	////////////////////////////////////////////////////////////////////////////////////
	guid_64 g_lua_guid_array[10240];
	int32   g_idx = 0;

	guid_64* get_guid()
	{
		int32 cur_idx = g_idx++;
		if (g_idx == 10240)
		{
			g_idx = 0;
		}
		guid_64* guid_ptr = g_lua_guid_array + cur_idx;
		return guid_ptr;

	}

	guid_64* get_guid_64(int64 value)
	{

		guid_64* guid_ptr = get_guid();
		guid_ptr->server_64 = value;
		return guid_ptr;
	}

	guid_64* get_guid_64(int32 a, int32 b)
	{
		guid_64* guid_ptr = get_guid();
		guid_ptr->A = a;
		guid_ptr->B = b;
		return guid_ptr;
	}

	guid_64* get_guid_64()
	{
		guid_64* guid_ptr = get_guid();
		guid_ptr->server_64 = 0;
		return guid_ptr;
	}






	////////////////////////////////////////////////////////////////////////////////////

	client_session * get_client_session_by_index(int64 unit_index)
	{
		s_client_uid client_uid(unit_index);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return nullptr;
		}
		return session;
	}

	client_session * get_client_session_by_guid(guid_64 guid)
	{

		client_session* session = client_session_mgr::getInstance().get_session(guid);
		if (nullptr == session)
		{
			return nullptr;
		}
		return session;
	}
	
	world_server* get_world_server()
	{
		world_server& tmp = world_server::getInstance();
		return &tmp;
	}

	gate_proxy*	 get_gate_proxy()
	{
		gate_proxy& tmp = gate_proxy::getInstance();

		return &tmp;
	}

	int64  make_guid_lua(int64 in_guid)
	{
		guid_64 new_guid = guid_gen::make_guid(guid_64(in_guid));
		return  new_guid.server_64;
	}

	client_session_mgr* get_client_session_mgr()
	{
		client_session_mgr& tmp = client_session_mgr::getInstance();
		return &tmp;

	}

	std::string get_str_by_string_template_id(int32 template_id, int32 index)
	{
		return template_manager::get_instance().get_str_by_string_template_id(template_id, index);
	}


	/////////////////////////////////////////////////////////////////////////////
	             /// 临时变量 

	//////////////////////////////////////////////////////////////////////////////

	Lua_g_var_num(s_item_info, item, 100)


	s_item_info* get_item_info_by_str(const char *msg, int32 msg_len)
	{
		faith::st_proto::st_item_info pro_msg;
		bool is_suecss = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_suecss == false)
		{
			return nullptr;
		}
		s_item_info* item_info_ptr = get_item_var();
		//item_info_ptr->from_proto(pro_msg);
		return item_info_ptr;
	}


	//////////////////////////////////////////////////////////
	Lua_g_var_num(s_auction_info, auction, 100)


	s_auction_info* get_auction_info_by_str(const char* msg, int32 msg_len)
	{
		if (msg == nullptr)
		{
			return nullptr;
		}
		faith::st_proto::st_auction_info pro_msg;
		bool is_suecss = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_suecss == false)
		{
			return nullptr;
		}
		s_auction_info* tmp_ptr = get_auction_var();
		tmp_ptr->clear_data();
		tmp_ptr->from_proto(pro_msg);
		return tmp_ptr;
	}


	char * get_auction_buffer(s_auction_info *auction_ptr)
	{
		faith::st_proto::st_auction_info pro_msg;
		auction_ptr->to_proto(&pro_msg);
		packet_s2s* p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
		if (p_s2s == nullptr)
		{
			return nullptr;
		}
		char *p_hex = parse_msg::getInstance().get_cache(p_s2s->google_data_len * 2);
		if (p_hex == nullptr)
		{
			return nullptr;
		}
		int32 offset = 0;
		static char encode[17] = "0123456789ABCDEF";
		for (int32 i = 0; i < p_s2s->google_data_len; i++)
		{
			p_hex[2 * i] = encode[p_s2s->google_data[i] >> 4];
			p_hex[2 * i + 1] = encode[p_s2s->google_data[i] & 0xf];
		}
		return p_hex;
	}

	//////////////////////////////////////////////////////////////////

	Lua_g_var_num(s_unit_info,  unit, 100)

	s_unit_info* get_unit_info_by_str(const char* msg, int32 msg_len)
	{
		if (msg == nullptr)
		{
			return nullptr;
		}
		faith::st_proto::st_unit_info pro_msg;
		bool is_suecss = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_suecss == false)
		{
			return nullptr;
		}
		s_unit_info* tmp_ptr = get_unit_var();
		tmp_ptr->clear_data();
		tmp_ptr->from_proto(pro_msg);
		return tmp_ptr;
	}

	//////////////////////////////////////////////////////////////////

	Lua_g_var_num(login_fixed_data, login_fixed, 100)

	login_fixed_data *get_login_fixed_data_by_str(const char *msg, int32 msg_len)
	{
		if (msg == nullptr)
		{
			return nullptr;
		}
		faith::st_proto::st_login_fixed_data pro_msg;
		bool is_suecss = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_suecss == false)
		{
			return nullptr;
		}
		login_fixed_data* tmp_ptr = get_login_fixed_var();
		tmp_ptr->clear_data();
		tmp_ptr->from_proto(pro_msg);
		return tmp_ptr;

	}
	///////////////////////////////////////////////////
	//Lua_g_var_num(s_log_common_head, log_head, 100)

	s_log_common_head* g_lua_log_head_array = nullptr;
	int32 g_log_head_idx = 0;

	static s_log_common_head*   get_log_head_var()
    {                                                        
		int32 cur_idx = g_log_head_idx++;
		if (g_log_head_idx == 100)
		{
			g_log_head_idx = 0;
		}
		s_log_common_head* tmp_ptr = g_lua_log_head_array + cur_idx;
	    return tmp_ptr;                                          
    }                                                          


	s_log_common_head* get_log_common_head()
	{
		if (g_lua_log_head_array == nullptr)
		{
			g_lua_log_head_array = new s_log_common_head[100];
		}
		s_log_common_head* tmp_ptr = get_log_head_var();
		tmp_ptr->clear_data();
		return tmp_ptr;
	}
	void lua_set_log_common_head_part1(s_log_common_head& head, xstring game_id, xstring channel_id, xstring media_id, xstring device_id)
	{
		set_log_common_head_part1(head, game_id, channel_id, media_id, device_id)
	}
	void lua_set_log_common_head_part2(s_log_common_head& head, xstring version_name, int32  version_code)
	{
		set_log_common_head_part2(head, version_name, version_code)
	}
	void lua_set_log_common_head_part3(s_log_common_head& head, xstring user_id, int32  server_id, int64 log_time)
	{
		set_log_common_head_part3(head, user_id, server_id, log_time)
	}
	void lua_set_log_common_head_part4(s_log_common_head& head, int64 role_id, xstring role_name, int32 role_level, int32 role_gender)
	{
		guid_64 role_guid(role_id);
		set_log_common_head_part4(head, role_guid, role_name, role_level, role_gender)
	}
	void lua_set_log_common_head_part5(s_log_common_head& head, int64 battle_points, int32 role_type, int32 vip_level)
	{
		set_log_common_head_part5(head, battle_points, role_type,  vip_level)
	}

	xstring get_app_key()
	{
		return SERVERCONFIG->app_key;

	}

	Lua_g_var_num(s_mail_info, mail,100)

	s_mail_info* get_mail_info_by_str(const char *msg, int32 msg_len)
	{
		if (msg == nullptr)
		{
			return nullptr;
		}
		faith::db_proto::role_mail_row pro_msg;
		bool is_suecss = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_suecss == false)
		{
			return nullptr;
		}
		s_mail_info* tmp_ptr = get_mail_var();
		tmp_ptr->clear_data();
		tmp_ptr->from_proto(pro_msg);
		return tmp_ptr;
	}

	Lua_g_var_num(s_fake_player_info, fake_player, 20)

	s_fake_player_info* get_fake_player_info_by_str(const char *msg, int32 msg_len)
	{
		if (msg == nullptr)
		{
			return nullptr;
		}
		faith::st_proto::st_fake_player_info pro_msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_sucess == false)
		{
			return nullptr;
		}
		s_fake_player_info* tmp_ptr = get_fake_player_var();
		tmp_ptr->clear();
		tmp_ptr->from_proto(pro_msg);
		return tmp_ptr;
	}

	Lua_g_var_num(ws2ws_send_big_player_msg, send_big_player, 20)


	ws2ws_send_big_player_msg* get_send_big_player_msg(const char *msg, int32 msg_len, int32& out_len)
	{
		if (msg == nullptr)
		{
			return nullptr;
		}
		faith::ws2ws_proto::send_big_player_msg pro_msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&pro_msg, msg, msg_len, false);
		if (is_sucess == false)
		{
			return nullptr;
		}
		ws2ws_send_big_player_msg* tmp_ptr = get_send_big_player_var();
		tmp_ptr->clear_data();
		tmp_ptr->from_proto(pro_msg);

		out_len = sizeof(ws2ws_send_big_player_msg);
		return tmp_ptr;
	}





	
   
	///////////////////////////////////////////////////////////



	





}