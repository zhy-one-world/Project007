/********************************************************************
  created: 2014/08/09
  created: 9:8:2014 11:48
  file base: http_send
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "http_send.hpp"
#include "json/json.h"
#include "md5/md5.h"
#include "game_cfg/servers_config.h"
#include "utility/cs_date.hpp"
#include "utility/init_unit.h"
#include "http/http_access_mgr.hpp"
#include "internal/daemon.hpp"

namespace hld
{
	http_send& http_send::get_instance()
	{
		static http_send g_instance;
		return g_instance;
	}
	void http_send::send_http_handle(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result)
	{
		std::cout << http_error_info << std::endl;
		std::cout << http_result << std::endl;
	}
	void http_send::send_role_count(xstring app_key, int32 server_id, time_cout_map& time_count)
	{
		if (app_key != xstring("1092"))
		{
			return;
		}
		xstring sdk_url = "https://game.apesgame.com/outapi/cu_counts";
		xstring secr_key = "af8143bf48ad60ccad6484ee7e3213c4";
		xstring user_data = "";
		for (time_cout_map_it it = time_count.begin(); it != time_count.end(); ++it)
		{
			s_time_count& channel_time_count = it->second; 
			xstring channel_data = "";
			int32 max_count = 0;
			int32 cur_count = 0;
			for (int32 i = 0; i < max_time_role_count; ++i)
			{
				max_count = channel_time_count.m_time_role_count[i] > max_count ? channel_time_count.m_time_role_count[i] : max_count;
				cur_count += channel_time_count.m_time_role_count[i];
				channel_data += "|" + init_unit::change_i64_to_string(channel_time_count.m_time_role_count[i]);
			}
			channel_data = "|" + init_unit::change_i64_to_string(max_count) + channel_data;
			channel_data = "|" + init_unit::change_i64_to_string(cur_count / max_time_role_count) + channel_data;
			channel_data = it->first + channel_data;
			user_data += channel_data + ",";
		}
		// 自定义请求头
		std::vector<xstring> head_list;
		//head_list.push_back("application/json");

		int32 time_now = time_helper::get_time();
		xstring post_content = "data=" + user_data + "&";
		post_content += "gameid=" + app_key + "&";
		post_content += "serverid=" + init_unit::change_i64_to_string(server_id) + "&";
		post_content += "time=" + init_unit::change_i64_to_string(time_now) + "&";
		xstring md5_str = user_data + app_key;
		md5_str += init_unit::change_i64_to_string(server_id);
		md5_str += init_unit::change_i64_to_string(time_now);
		md5_str += secr_key;
		MD5 md5;
		md5.update(md5_str.c_str(), md5_str.size());
		post_content += "signature=" + md5.toString();


		// 异步请求
		http_access_mgr::get_instance().async_request
		(
			0,
			sdk_url,
			head_list,
			post_content,
			boost::bind(&http_send::send_http_handle, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}
	void http_send::send_role_level(xstring app_key, xstring channel_id, xstring lang, xstring account, int32 server_id, guid_64 role_guid, int32 exp_level)
	{
		if (app_key != xstring("1092"))
		{
			return;
		}
		if (exp_level < 90)
		{
			if (exp_level % 10 != 0)
			{
				return;
			}
		}
		xstring secr_key = "af8143bf48ad60ccad6484ee7e3213c4";
		xstring sdk_url = "https://game.apesgame.com/outapi/notify_level";

		// 自定义请求头
		std::vector<xstring> head_list;
		head_list.push_back("GAME-ID:" + app_key);
		head_list.push_back("CHANNEL:" + channel_id);
		head_list.push_back("LANG:");
		//head_list.push_back("application/json");
		int32 time_now = time_helper::get_time();
		xstring post_content = "level=" + init_unit::change_i64_to_string(exp_level);
		post_content += "&rid=" + init_unit::change_i64_to_string(role_guid.server_64);
		post_content += "&sid=" + init_unit::change_i64_to_string(server_id);
		post_content += "&time=" + init_unit::change_i64_to_string(time_now);
		post_content += "&uid=top1game_" + account;
		xstring md5_str = "";
		md5_str += init_unit::change_i64_to_string(exp_level);
		md5_str += init_unit::change_i64_to_string(role_guid.server_64);
		md5_str += init_unit::change_i64_to_string(server_id);
		md5_str += init_unit::change_i64_to_string(time_now);
		md5_str += "top1game_"+account;
		md5_str += secr_key;
		MD5 md5;
		md5.update(md5_str.c_str(), md5_str.size());
		post_content += "&signature=" + md5.toString();

		// 异步请求
		http_access_mgr::get_instance().async_request
		(
			0,
			sdk_url,
			head_list,
			post_content,
			boost::bind(&http_send::send_http_handle, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}
	void http_send::send_server_state(const server_info_gm& srv_info, int32 player_num)
	{
		Json::Value server_info_json;
		server_info_json["serverId"] = SERVERCONFIG->game_id;
		server_info_json["serverUserNum"] = Json::Value(srv_info.acc_num);
		server_info_json["serverRoleNum"] = Json::Value(srv_info.role_num);
		server_info_json["serverRoleOnlineNum"] = Json::Value(player_num);

		Json::FastWriter fastWriter;
		std::string server_info_json_str = fastWriter.write(server_info_json);

		xstring target_url = SERVERCONFIG->server_state_url;
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type: application/json");
		http_access_mgr::get_instance().async_request
		(
			0,
			target_url,
			head_list,
			server_info_json_str,
			boost::bind(&http_send::send_http_handle, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}
	void http_send::send_role_info(const s_unit_info& role_info)
	{
		Json::Value server_info_json;
		server_info_json["userCode"] = role_info.account;
		server_info_json["serverId"] = role_info.data_ary[e_role_info_server_id];
		server_info_json["templateId"] = role_info.data_ary[e_role_info_template_id];
		server_info_json["expLevel"] = role_info.data_ary[e_role_info_exp_level];
		server_info_json["roleName"] = role_info.role_name;
		server_info_json["roleGuid"] = role_info.role_guid.server_64;

		Json::FastWriter fastWriter;
		std::string server_info_json_str = fastWriter.write(server_info_json);

		xstring target_url = SERVERCONFIG->account_info_url;
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type: application/json");
		http_access_mgr::get_instance().async_request
		(
			0,
			target_url,
			head_list,
			server_info_json_str,
			boost::bind(&http_send::send_http_handle, this, _1, _2, _3, _4),
			e_http_request_type_post
		);
	}
}
