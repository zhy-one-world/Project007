/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: login_service_google
  file ext: cpp
  author: lxy
  
  purpose: 
*********************************************************************/
#include "login_service_google.hpp"
#include "login_service_mgr.hpp"
#include "game_cfg/servers_config.h"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <login_msg.hpp>
#include <Utility/parse_msg.h>
#include <Utility/init_unit.h>
#include <json/json.h>
#include <Utility/cs_date.hpp>
#include <time.hpp>
#include "server_log.hpp"
#include "server_log_msg.hpp"
namespace faith
{
	const xstring sdk_password = "googlesdk";		// 固定密码

	void login_service_google::tick(float deltaseconds)
	{
		m_time_count += deltaseconds;
	}

	void login_service_google::req_login(const s_client_uid client_uid, const login_proto_login proto_data)
	{
		if (proto_data.login_type() != e_login_type_new_google)
		{
			return;
		}

		xstring sdk_url = "https://www.googleapis.com/oauth2/v3/tokeninfo?";

		// 自定义请求头
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type:application/x-www-form-urlencoded");
		//xstring url_para = create_post_data(proto_data.sdk_data().data(), world_server::getInstance().get_server_id(), "", "");
		xstring url_para = "id_token=" + proto_data.sdk_data().data();

		// 异步请求
		http_access_mgr::get_instance().async_request
			(
				client_uid,
				sdk_url,
				head_list,
				url_para,
				boost::bind(&login_service_google::on_login_result_handle, this, _1, _2, _3, _4),
				e_http_request_type_get
				);
	}

	void login_service_google::end_login(const dp2ls_client_login* pdata)
	{
		if (!pdata)
		{
			return;
		}
		//CONSOLE_INFO(" sdk : dp to ls login end : state = " << pdata->e_result << time_helper::get_current_time() << " , " << faith::utility::get_tick_count() );

		ls2fep_client_login request;
		request.eResult = pdata->e_result;
		request.client_uid = pdata->client_uid;
		request.login_type = pdata->login_type;
		request.login_type_plus = pdata->login_type_plus;
		request.jewel_num = pdata->jewel_num;
		request.radio_host = pdata->radio_host;
		request.is_create_new = pdata->is_create_new;
		memcpy(request.account, pdata->account, sizeof(pdata->account));
		memcpy(request.server_msg, pdata->server_msg, sizeof(pdata->server_msg));
		memcpy(request.ban_role_array, pdata->ban_role_array, sizeof(request.ban_role_array));
		memcpy(request.ban_chat_array, pdata->ban_chat_array, sizeof(request.ban_chat_array));
		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &request, sizeof(request));
	}

	void login_service_google::on_login_result_handle(ui64 uid,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result)
	{
		//CONSOLE_INFO(" login_result: " << http_result << "\n");


		//CONSOLE_INFO(" on_login_result_handle: " << http_error_code << ","
		//	<< http_error_info << "," << http_result << "," << time_helper::get_current_time()
		//	<< " , " << faith::utility::get_tick_count());
		s_client_uid client_uid(uid);
		http_access_mgr::get_instance().remove_http(client_uid);

		if (http_result.size() > 0)
		{
			read_json_data(http_result, client_uid);
		}
	}



	xstring login_service_google::write_json_data()
	{
		return "";
	}

	bool login_service_google::read_json_data(const xstring& json_data,const s_client_uid client_uid)
	{
		Json::Reader reader;
		Json::Value  value;
		Json::Value  data_value;
		Json::Value  default_value;

		try
		{
			if (json_data.size() > 0 && reader.parse(json_data, value))
			{
				if (value.isNull() || value.empty())
				{
					CONSOLE_INFO("billing respose format error:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}
				else if (!value["error_description"].isNull())
				{
					CONSOLE_INFO("billing respose format error:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}

				data_value = value;

				// 解析data数据
				if (data_value["sub"].isNull() || data_value["sub"].empty() || !data_value["sub"].isString())
				{
					CONSOLE_INFO("billing data format error:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}

				xstring account = data_value["sub"].asString();
				int64 ban_role_array[faith::max_character_num];
				int64 ban_chat_array[faith::max_character_num];
				memset(ban_role_array, 0, sizeof(ban_role_array));
				memset(ban_chat_array, 0, sizeof(ban_chat_array));
				//Json::Value& ban_role = data_value["banRoles"];
				//Json::Value& ban_chat = data_value["banChats"];
				// 检查登陆状态
				if (account.size() > 0)
				{
					//CONSOLE_INFO("sdk read json data : " << json_data.c_str() << " ," << time_helper::get_current_time() << " , " << faith::utility::get_tick_count());
					// 存盘
					save_account(account, json_data, client_uid, ban_role_array, ban_chat_array);
					return true;
				}
				else
				{
					CONSOLE_INFO("billing check failed:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
				}
			}
			else
			{
				CONSOLE_INFO("billing respose format error:{}", json_data);
				ls2fep_client_login msg;
				msg.client_uid = client_uid;
				msg.eResult = e_error_code_login_invalid_bi;
				memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
				world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
			}
		}
		catch (...)
		{
			CONSOLE_INFO("  catch-exception:{}", json_data);
			ls2fep_client_login msg;
			msg.client_uid = client_uid;
			msg.eResult = e_error_code_login_invalid_bi;
			memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
			world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
		}
		return false;
	}

	void login_service_google::save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array)
	{
		const login_proto_login* login_data = login_service_mgr::getInstance().get_login_info(client_uid.fep_uid_64);
		if (login_data == nullptr
			|| nullptr == ban_role_array
			|| nullptr == ban_chat_array)
		{
			return;
		}

		ls2dp_client_login request;
		request.client_uid = client_uid;
		request.server_id = login_data->server_id();

		if(account.size() > 0 && account[0] != '\0')
		{
			memcpy(request.account, account.c_str(), sizeof(request.account) > account.size() ? account.size() : sizeof(request.account));
			memcpy(request.password, sdk_password.c_str(), sizeof(request.password) > sdk_password.size() ? sdk_password.size() : sizeof(request.password));
			memcpy(request.server_msg, server_msg.c_str(), sizeof(request.server_msg) > server_msg.size() ? server_msg.size() : sizeof(request.server_msg));
			memcpy(request.create_ip, login_data->ip().c_str(), sizeof(request.create_ip) > login_data->ip().size() ? login_data->ip().size() : sizeof(request.create_ip));
			memcpy(request.device_type, login_data->device_type().c_str(), sizeof(request.device_type) > login_data->device_type().size() ? login_data->device_type().size() : sizeof(request.device_type));
			request.login_type = e_login_type_new_google;
			request.login_type_plus = 0;
			memcpy(request.ban_role_array, ban_role_array, sizeof(request.ban_role_array));
			memcpy(request.ban_chat_array, ban_chat_array, sizeof(request.ban_chat_array));
			ws_client::getInstance().send_to_dp( &request, sizeof(request));
			////login日志
			//server_log::login_role_log(login_data->server_id(),
			//	login_data->sdk_data().app_key(),
			//	login_data->client_version(),
			//	login_data->sdk_data().channel_id(),
			//	login_data->sdk_data().app_secret(),
			//	account,
			//	login_data->sdk_data().device_id(),
			//	login_data->ip(),
			//	login_data->sdk_data().model(),
			//	login_data->sdk_data().idfa(),
			//	login_data->sdk_data().imei(),
			//	login_data->sdk_data().android_id(),
			//	login_data->sdk_data().mac(),
			//	login_data->sdk_data().sn());
		}

	}


	void login_service_google::ls2dp_save_server_log(const xstring & log_sql_name, const xstring & sql_param_string)
	{
		faith::server2dp_save_log req;
		int32 cpy_size = log_sql_name.size() > max_name_size ? max_name_size : log_sql_name.size();
		memcpy(req.sql_table_name, log_sql_name.c_str(), cpy_size);
		cpy_size = sql_param_string.size() > max_log_char_num ? max_log_char_num : sql_param_string.size();
		memcpy(req.save_string, sql_param_string.c_str(), cpy_size);
		ws_client::getInstance().send_to_dp( &req, sizeof(server2dp_save_log));
	}

	xstring login_service_google::create_post_data(const xstring& data, int32 server_id, const xstring& game_id, const xstring& channel_id)
	{
		//return xstring("login_type=") + to_string(e_login_sdk_type_google) + "&"
		//	+ "token_from_client=" + data;

		return xstring("login_type=") + to_string(e_login_type_new_google) + "&"
			+ "billingToken=" + data + "&serverId=" + to_string(server_id) + "&"
			+ "gameId=" + game_id + "&"
			+ "channelId=" + channel_id;
	}


	xstring login_service_google::create_sign(i16 param_num,...)
	{
		va_list argumentPtr;
		va_start(argumentPtr,param_num);

		xstring param;
		for(int32 i = 0;i < param_num;i++)
		{
			param += va_arg(argumentPtr,xstring);
		}
		va_end(argumentPtr);

		ui8 sign[MD5_CHAR_NUM];
		memset(sign,0,sizeof(sign));

		xstring result;
		for(uint32 i = 0;i< sizeof(sign);++i)
		{ 
			int32 x = sign[i]&0xFF; 
			int32 h = x >> 4; 
			int32 l = x & 0x0F; 
			result += (char)(h + ((h<10) ? '0' : 'a' - 10)); 
			result += (char)(l + ((l<10) ? '0' : 'a' - 10)); 
		} 

		return result.substr(8,16);
	}

	void login_service_google::create_http_head_list(std::vector<xstring>& head_list,i16 param_num,...)
	{
		va_list argumentPtr;
		va_start(argumentPtr,param_num);

		xstring param;
		for(int32 i = 0;i < param_num;i++)
		{
			head_list.push_back(va_arg(argumentPtr,xstring));
		}
		va_end(argumentPtr);
	}

}
