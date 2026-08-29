#include "rename_mgr.h"
#include "../client_session.hpp"
#include "../client_session_mgr.hpp"
#include "ws_client.hpp"
#include "template/template_manager.h"
#include "server/mail/mail_event_ws.h"
#include <char_msg.hpp>
#include "lua/script_mgr.h"
#include "invalid_word/invalid_ansi_word.h"
#include "net.pb.h"
#include "character.pb.h"

#define CHANGE_NAME_CARD_ID				31000159
namespace faith
{
	rename_mgr::rename_mgr()
	{
		clear_data();
	}
	rename_mgr::~rename_mgr()
	{
		clear_data();
	}

	void rename_mgr::clear_data(void)
	{

	}

	void rename_mgr::heart_tick(const int64& new_time)
	{

	}

	void rename_mgr::add_want_use_name(const xstring& name, uint64 role_guid)
	{
		m_want_use_names.insert(std::make_pair(name, role_guid));
	}

	void rename_mgr::del_want_use_name(const xstring& name)
	{
		auto iter = m_want_use_names.find(name);
		if (m_want_use_names.end() != iter)
		{
			m_want_use_names.erase(iter);
		}
	}

	bool rename_mgr::is_in_want_use_name(const xstring& name)
	{
		if (m_want_use_names.count(name) > 0)
		{
			return true;
		}

		return false;
	}

	void rename_mgr::get_name_in_want_use_name(const uint64& role_guid, xstring& name)
	{
		auto iter = m_want_use_names.begin();
		for (; iter != m_want_use_names.end(); ++iter)
		{
			if (role_guid == iter->second)
			{
				name = iter->first;
				return;
			}
		}
	}

	void rename_mgr::check_name_is_avaliable(const xstring& role_name, uint32& check_ret, const uint64& role_guid )
	{
		if (false == invalid_ansi_word::is_valid_ansi_str(role_name))
		{
			check_ret = e_change_failed_role_name_invalid;
		}

		if (invalid_ansi_word::include_invalid_ansi_str(role_name))
		{//存在非法字
			check_ret = e_change_failed_role_name_invalid;
		}

		//判断角色名长度是否合法
		if ((role_name.size() < min_name_size))
		{
			check_ret = e_change_failed_role_name_size_too_short;
		}
		if ((role_name.size() > max_name_size))
		{
			check_ret = e_change_failed_role_name_size_too_long;
		}

		if (is_in_want_use_name(role_name))
		{
			check_ret = e_change_failed_role_name_duplicate;
		}

		xstring want_name = "";
		get_name_in_want_use_name(role_guid, want_name);
		if (want_name.length() > 0)
		{
			check_ret = e_change_failed_common_error;
		}

		return;
	}

	bool rename_mgr::change_player_name(xstring name, guid_64 role_guid)
	{
		if (is_use_lua())
		{
			bool bRet = false;
			script_mgr::get_instance().call_func(nullptr, "rename_change_player_name", 1, false, "%s%l>%b", name.c_str(), role_guid.server_64, &bRet);
			return bRet;
		}
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
		{
			return false;
		}
		uint32 check_ret = e_change_role_success;

		check_name_is_avaliable(name, check_ret, role_guid);

		if (check_ret != e_change_role_success)
		{
			character_proto_change_player_name_end result_msg;
			result_msg.set_ret(check_ret);
			player_session->send_to_client(&result_msg, e_msgindex_s2c_change_player_name_end);
			return false;
		}

		server2dp_proto::ws2dp_check_player_name pro_msg;


		ws2dp_check_player_name msg;
		msg.role_guid = role_guid;
		msg.set_role_name(name);

		msg.to_proto(pro_msg);


		ws_client::getInstance().send_to_dp_new(&pro_msg, e_msgindex_ws2dp_check_player_name, player_session->m_role_info.data_ary[e_role_info_server_id]);

       

		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), player_session->m_role_info.data_ary[e_role_info_server_id]);

		return true;
	}
	void rename_mgr::sub_rename_item_end(int32 ret, guid_64 role_guid, xstring& want_name, xstring& original_name)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "rename_sub_rename_item_end", 2, false, "%d%l>%s%s", ret, role_guid.server_64, &want_name, &original_name);
			return;
		}
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
		{
			give_back_rename_card(role_guid);
			return;
		}
		get_name_in_want_use_name(role_guid, want_name);
	
		if (want_name.length() == 0)
		{
			return;
		}

		if (ret != e_change_role_success)
		{
			rename_mgr::del_want_use_name(want_name);

			character_proto_change_player_name_end result_msg;
			result_msg.set_ret(ret);
			player_session->send_to_client(&result_msg, e_msgindex_s2c_change_player_name_end);
			return;
		}

		ws2dp_change_player_name msg;
		msg.role_guid = role_guid;
		msg.set_role_name(want_name);

		server2dp_proto::ws2dp_change_player_name pro_msg;

		msg.to_proto(pro_msg);

		ws_client::getInstance().send_to_dp_new(&pro_msg, e_msgindex_ws2dp_change_player_name, player_session->m_role_info.data_ary[e_role_info_server_id]);

		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), player_session->m_role_info.data_ary[e_role_info_server_id]);




		original_name = player_session->get_role_name();

		char char_role_name[max_name_size + 1] = { 0 };
		memcpy(char_role_name, want_name.c_str(), want_name.size() > max_name_size ? max_name_size : want_name.size());
		player_session->set_role_name(char_role_name);

		ws2cs_change_player_name req_msg;
		req_msg.role_guid = player_session->get_role_guid();
		req_msg.set_role_name(want_name);
		bool is_use = proto_by_lua(e_msgindex_ws2cs_change_player_name);
		if (is_use == false)
		{
			player_session->send_to_cs(&req_msg, sizeof(ws2cs_change_player_name));
		}
		else
		{
			ws2cs_proto::change_player_name pro_msg;
			req_msg.to_proto(pro_msg);
			player_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_change_player_name);
		}
	}

	void rename_mgr::check_role_name_end(uint64 role_guid, xstring role_name, int32 result)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "rename_check_role_name_end", 0, false, "%l%s%d", role_guid, role_name.c_str(), result);
			return;

		}
		uint32 check_ret = e_change_role_success;

		if (result != e_change_role_success)
		{
			rename_mgr::del_want_use_name(role_name);

			check_ret = e_change_failed_role_name_duplicate;
		}
		else
		{
			check_ret = e_change_role_name_available;
		}

		xstring want_name = "";
		get_name_in_want_use_name(role_guid, want_name);
		if (want_name.length() == 0 || check_ret != e_change_role_name_available)
		{
			character_proto_change_player_name_end result_msg;
			result_msg.set_ret(check_ret);
			client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr != player_session)
			{
				player_session->send_to_client(&result_msg, e_msgindex_s2c_change_player_name_end);
			}
		}
		else
		{
			ws2cs_sub_player_rename_item msg;
			msg.role_guid = role_guid;
			msg.item_templete_id = CHANGE_NAME_CARD_ID;

			client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
			if (nullptr == player_session)
			{
				return;
			}
			bool is_use = proto_by_lua(e_msgindex_ws2cs_sub_player_rename_item);
			if (is_use == false)
			{
				player_session->send_to_cs(&msg, sizeof(msg));
			}
			else
			{
				ws2cs_proto::sub_player_rename_item pro_msg;
				msg.to_proto(pro_msg);
				player_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_sub_player_rename_item);
			}
		}
		
		return;
	}

	void rename_mgr::confirm_change_name(bool confirm_type, guid_64 role_guid, const xstring& role_name )
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "rename_confirm_change_name", 0, false, "%b%l%s", confirm_type, role_guid.server_64, role_name.c_str());
			return;
		}
		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
		{
			return;
		}
		uint32 check_ret = e_change_role_success;
		if (!confirm_type)
		{	
			return;
		}

		check_name_is_avaliable(role_name, check_ret, role_guid);

		if (check_ret != e_change_role_success)
		{
			character_proto_change_player_name_end result_msg;
			result_msg.set_ret(check_ret);

			player_session->send_to_client(&result_msg, e_msgindex_s2c_change_player_name_end);
			return;
		}

		rename_mgr::add_want_use_name(role_name, role_guid);

		ws2dp_check_player_name msg;
		msg.role_guid = role_guid;
		msg.set_role_name(role_name);

		server2dp_proto::ws2dp_check_player_name pro_msg;
		msg.to_proto(pro_msg);

		ws_client::getInstance().send_to_dp_new(&pro_msg, e_msgindex_ws2dp_check_player_name, player_session->m_role_info.data_ary[e_role_info_server_id]);

		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), player_session->m_role_info.data_ary[e_role_info_server_id]);

	}

	void rename_mgr::change_player_name_end(uint64 role_guid, xstring role_name)
	{
		if (is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "rename_change_player_name_end", 0, false, "%l%s", role_guid, role_name.c_str());
			return;
		}
		rename_mgr::del_want_use_name(role_name);

		uint32 check_ret = e_change_role_success;
		character_proto_change_player_name_end result_msg;
		result_msg.set_ret(check_ret);

		client_session* player_session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == player_session)
		{
			return;
		}
		player_session->send_to_client(&result_msg, e_msgindex_s2c_change_player_name_end);
	}

	void rename_mgr::give_back_rename_card(uint64 role_guid)
	{
		const char* mail_title_char = nullptr;
		const char* mail_content_char = nullptr;
 
		mail_title_char = "90303013";
		mail_content_char = "90303013";

		std::vector<s_item_template_info> item_list;
		item_list.push_back({ CHANGE_NAME_CARD_ID , 1});
		mail_ws_mgr::get_instance().send_mail_system(role_guid, 0, item_list, mail_title_char, mail_content_char, 0, 0);
	}

	bool rename_mgr::is_use_lua()
	{
		bool bRet = false;
		script_mgr::get_instance().call_func(nullptr, "is_rename_use_lua", 1, false, ">%b", &bRet);
		return bRet;
	}
}

