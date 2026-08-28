/********************************************************************
  created: 2020/07/29
  file base: login_service_appstore
  file ext: cpp
  author: wangdi
  
  purpose: 
*********************************************************************/
#include "login_service_appstore.hpp"
#include "login_service_mgr.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <login_msg.hpp>
#include <json/json.h>
#include <Utility/cs_date.hpp>
#include "server_log.hpp"
#include "jwtcpp/jwt.h"
#include "Base64.h"

namespace hld
{
	const xstring sdk_password = "googlesdk";		// 固定密码
	const xstring apple_login_private_key =	"-----BEGIN PRIVATE KEY-----\nMIGTAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBHkwdwIBAQQgMMKLzlrbVd9ezpLR\nnOiLE3iiB7ucbWVw6F4OwYi1JHygCgYIKoZIzj0DAQehRANCAASO/33LDPplMKcE\nORrE38AGiX67hlFRcvjgWJcKM0Hlmj1vSzhpqUEvJZR+bE0HXcKPWPodZa51PWvG\nrhYgsWKy\n-----END PRIVATE KEY-----";

	login_service_appstore::login_service_appstore()
	{
		m_time_count = 0;
		m_last_client_secret_time = 0;
	}
	void login_service_appstore::tick(float deltaseconds)
	{
		m_time_count += deltaseconds;
	}

	void login_service_appstore::req_login(const s_client_uid client_uid, const login_proto_login proto_data)
	{
		if (proto_data.login_type() != e_login_type_new_appstore)
		{
			return;
		}

		xstring sdk_url = "https://appleid.apple.com/auth/token";

		// 自定义请求头
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type:application/x-www-form-urlencoded");
		
		xstring url_para = xstring("client_id=com.gameland.arka.ios")
			+ "&client_secret=" + get_client_secret()
			+ "&code=" + proto_data.sdk_data().data()
			+ "&grant_type=authorization_code";

		// 异步请求
		http_access_mgr::get_instance().async_request
			(
				client_uid,
				sdk_url,
				head_list,
				url_para,
				boost::bind(&login_service_appstore::on_login_result_handle, this, _1, _2, _3, _4),
				e_http_request_type_post
				);
	}

	void login_service_appstore::end_login(const dp2ls_client_login* pdata)
	{
		if (!pdata)
		{
			return;
		}
		//CONSOLE_INFO(" sdk : dp to ls login end : state = " << pdata->e_result << time_helper::get_current_time() << " , " << hld::utility::get_tick_count() );

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

	void login_service_appstore::on_login_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result)
	{
		//CONSOLE_INFO(" login_result: " << http_result << "\n");


		//CONSOLE_INFO(" on_login_result_handle: " << http_error_code << ","
		//	<< http_error_info << "," << http_result << "," << time_helper::get_current_time()
		//	<< " , " << hld::utility::get_tick_count());
		s_client_uid client_uid(uid);
		http_access_mgr::get_instance().remove_http(client_uid);

		if (http_result.size() > 0)
		{
			read_json_data(http_result, client_uid);
		}
	}

	bool login_service_appstore::read_json_data(const xstring& json_data,const s_client_uid client_uid)
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
				else if (!value["error"].isNull())
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
				if (data_value["id_token"].isNull() || data_value["id_token"].empty() || !data_value["id_token"].isString())
				{
					CONSOLE_INFO("billing data format error:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}

				xstring id_token = data_value["id_token"].asString();
				int64 ban_role_array[hld::max_character_num];
				int64 ban_chat_array[hld::max_character_num];
				memset(ban_role_array, 0, sizeof(ban_role_array));
				memset(ban_chat_array, 0, sizeof(ban_chat_array));

				auto decoded = jwt::decode(id_token);
				xstring account = decoded.get_subject();

				// 检查登陆状态
				if (account.size() > 0)
				{
					//CONSOLE_INFO("sdk read json data : " << json_data.c_str() << " ," << time_helper::get_current_time() << " , " << hld::utility::get_tick_count());
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

	void login_service_appstore::save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array)
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
			request.login_type = e_login_type_new_appstore;
			request.login_type_plus = 0;
			memcpy(request.ban_role_array, ban_role_array, sizeof(request.ban_role_array));
			memcpy(request.ban_chat_array, ban_chat_array, sizeof(request.ban_chat_array));
			ws_client::getInstance().send_to_dp( &request, sizeof(request));
		}

	}

	void login_service_appstore::refresh_client_secret()
	{
		time_info cur_time = time_helper::get_cur_time_new();
		m_client_secret = jwt::create()
			.set_key_id("RVK28A558B")	//key id
			.set_issuer("BH3438NA8P")	//team id
			.set_issued_at(std::chrono::system_clock::from_time_t(cur_time.second))
			.set_expires_at(std::chrono::system_clock::from_time_t(cur_time.second + 3600))
			.set_subject("com.gameland.arka.ios")
			.set_audience("https://appleid.apple.com")
			.sign(jwt::algorithm::es256{ "", apple_login_private_key });
		m_last_client_secret_time = cur_time.second + 3300;
	}

	const xstring& login_service_appstore::get_client_secret()
	{
		if (time_helper::get_cur_time_new().second > m_last_client_secret_time)
		{
			refresh_client_secret();
		}
		return m_client_secret;
	}

}
