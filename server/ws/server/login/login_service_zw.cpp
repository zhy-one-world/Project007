/********************************************************************
  created: 2014/08/05
  created: 5:8:2014 18:49
  file base: login_service_zw
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "login_service_zw.hpp"
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
#include "template/template_manager.h"
#include "server_log_msg.hpp"

namespace faith
{
	const xstring sdk_password = "zwsdk";		// 固定密码

	void login_service_zw::tick(float deltaseconds)
	{
		m_time_count += deltaseconds;
	}

	void login_service_zw::req_login(const s_client_uid client_uid, const login_proto_login proto_data)
	{
		if (proto_data.login_type() != e_login_type_new_zw)
		{
			return;
		}
		// URL
		xstring sdk_url = SERVERCONFIG->login_url;

		// sign
		xstring channel_id = proto_data.sdk_data().channel_id();

		xstring opcode = proto_data.sdk_data().opcode();

		xchar tag_buff[32];
		sprintf(tag_buff,"%d%d",client_uid.fepserver_uid,client_uid.fepsession_uid);
		xstring tag = tag_buff;
		tag = tag.substr(0,8);

		xstring sign = create_sign(SIGN_PARAM_NUM,opcode, 
			proto_data.sdk_data().data(), 
			proto_data.sdk_data().app_key(), 
			proto_data.sdk_data().app_secret(),
			tag,channel_id);

		// 自定义请求头
		std::vector<xstring> head_list;
		xstring gameId = "gameId:" + opcode;
		xstring channelId = "channelId:" + channel_id;
		create_http_head_list(head_list,HTTP_HEAD_PARAM_NUM, gameId, channelId/*,tag_str,opcode_str,channel_id_str, client_ip_str*/);

		// GET/POST 参数
		xstring url_para = create_post_data(proto_data.sdk_data().data(), world_server::getInstance().get_server_id(), opcode, channel_id);

		// 异步请求
		http_access_mgr::get_instance().async_request
			(
				client_uid,
				sdk_url,
				head_list,
				url_para,
				boost::bind(&login_service_zw::on_login_result_handle,this,_1,_2,_3,_4),
				e_http_request_type_post
			);
	}

	void login_service_zw::end_login(const dp2ls_client_login* pdata)
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
		request.online_duration = pdata->online_duration;
		request.online_time = pdata->online_time;
		request.month_recharge_num = pdata->month_recharge_num;
		request.month_recharge_time = pdata->month_recharge_time;
		if (SERVERCONFIG->gm_state)
		{
			request.age = 30;
		}
		else
		{
			request.age = pdata->age;
		}
		memcpy(request.account, pdata->account, sizeof(pdata->account));
		memcpy(request.server_msg, pdata->server_msg, sizeof(pdata->server_msg));
		memcpy(request.ban_role_array, pdata->ban_role_array, sizeof(request.ban_role_array));
		memcpy(request.ban_chat_array, pdata->ban_chat_array, sizeof(request.ban_chat_array));

		if (request.eResult == e_error_code_success && request.age < 18)
		{
			if (GAMECONFIG->AntiIndulgenceTimeLimit.size() == 4)
			{
				time_info cur_time_info = time_helper::get_cur_time_new();
				if ((cur_time_info.minute_in_day < GAMECONFIG->AntiIndulgenceTimeLimit[0] * 60 + GAMECONFIG->AntiIndulgenceTimeLimit[1])
					|| (cur_time_info.minute_in_day > GAMECONFIG->AntiIndulgenceTimeLimit[2] * 60 + GAMECONFIG->AntiIndulgenceTimeLimit[3]))
				{
					request.eResult = e_error_code_login_online_time_limit;
				}
				else
				{
					if (time_helper::is_diff_day(request.online_time, time_helper::get_time()) == true)
					{
						request.online_duration = 0;
						request.online_time = time_helper::get_time();
					}
					else
					{
						int32 DurationLimit = GAMECONFIG->AntiIndulgenceDurationLimit * 3600;
						for (int32 i = 0; i < GAMECONFIG->AntiIndulgenceHoliday.size() / 6; i++)
						{
							time_info start_time = time_helper::get_time_by_info(GAMECONFIG->AntiIndulgenceHoliday[i * 6], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 1], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 2]);
							time_info end_time = time_helper::get_time_by_info(GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 3], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 4], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 5]);
							if (start_time.second < cur_time_info.second && cur_time_info.second < end_time.second + 3600 * 24)
							{
								DurationLimit = GAMECONFIG->AntiIndulgenceHolidayDurationLimit * 3600;
								break;
							}
						}
						if (request.online_duration >= DurationLimit)
						{
							request.eResult = e_error_code_login_online_duration_over;
						}
					}
				}

				//充值额度
				if (time_helper::is_diff_month(request.month_recharge_time, time_helper::get_time()) == true)
				{
					request.month_recharge_num = 0;
					request.month_recharge_time = time_helper::get_time();
				}
			}
		}

		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &request, sizeof(request));
	}

	void login_service_zw::on_login_result_handle(ui64 uid,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result)
	{
		//CONSOLE_INFO(" on_login_result_handle: " << http_error_code << ","
		//	<< http_error_info << "," << http_result << "," << time_helper::get_current_time()
		//	<< " , " << faith::utility::get_tick_count());
		s_client_uid client_uid(uid);
		http_access_mgr::get_instance().remove_http(client_uid);

		if(http_result.size() > 0)
		{
			// 先剥离http头
			if ( std::string::npos == http_result.find_first_of("{") || std::string::npos == http_result.find_last_of("}") )
			{
				CONSOLE_INFO("error_code:{} error_info:{}", http_error_code, http_error_info);
				return;
			}
			xstring result = http_result.substr(http_result.find_first_of("{"),http_result.find_last_of("}") + 1);
			if(result.size() > 0)
			{
				// 再解析
				read_json_data(result, client_uid);
			}
		}
	}



	xstring login_service_zw::write_json_data()
	{
		return "";
	}

	bool login_service_zw::read_json_data(const xstring& json_data,const s_client_uid client_uid)
	{
		Json::Reader reader;
		Json::Value  value;
		Json::Value  data_value;
		Json::Value  default_value;


		try
		{
			if(json_data.size() > 0 && reader.parse(json_data,value))
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
				else if(value["data"].isNull() || value["data"].empty())
				{
					CONSOLE_INFO("billing respose format error:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}
				else if(!value["code"].isInt() || value["code"].asInt() != 0 )
				{
					CONSOLE_INFO("billing repose op failed:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}

				data_value = value.get("data",default_value);
				// 解析data数据
				if(data_value.empty() || data_value["userId"].isNull() || data_value["userId"].empty()  || !data_value["userId"].isString())
				{
					CONSOLE_INFO("billing data format error:{}", json_data);
					ls2fep_client_login msg;
					msg.client_uid = client_uid;
					msg.eResult = e_error_code_login_invalid_bi;
					memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
					world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
					return false;
				}

				xstring account = data_value["userId"].asString();
				int64 ban_role_array[faith::max_character_num];
				int64 ban_chat_array[faith::max_character_num];
				memset(ban_role_array, 0, sizeof(ban_role_array));
				memset(ban_chat_array, 0, sizeof(ban_chat_array));
				Json::Value& ban_role = data_value["banRoles"];
				Json::Value& ban_chat = data_value["banChats"];
				int32 age = data_value["age"].asInt();
				//age = 15;
				if (ban_role.isNull() == false && ban_role.isArray())
				{
					int32 array_index = 0;
					for (int32 i = 0; i < ban_role.size(); ++i)
					{
						Json::Value& ban_value = ban_role[i];
						if (ban_value["roleId"].isNull() == false && ban_value["roleId"].isString())
						{
							ban_role_array[array_index++] = init_unit::change_string_to_i64(ban_value["roleId"].asString());
						}
						if (array_index >= faith::max_character_num)
						{
							break;
						}
					}
				}
				if (ban_chat.isNull() == false && ban_chat.isArray())
				{
					int32 array_index = 0;
					for (int32 i = 0; i < ban_chat.size(); ++i)
					{
						Json::Value& ban_value = ban_chat[i];
						if (ban_value["roleId"].isNull() == false && ban_value["roleId"].isString())
						{
							ban_chat_array[array_index++] = init_unit::change_string_to_i64(ban_value["roleId"].asString());
						}
						if (array_index >= faith::max_character_num)
						{
							break;
						}
					}
				}
				// 检查登陆状态
				if(account.size() > 0)
				{
					//CONSOLE_INFO("sdk read json data : " << json_data.c_str() << " ," << time_helper::get_current_time() << " , " << faith::utility::get_tick_count());
					// 存盘
					save_account(account, json_data, client_uid, ban_role_array, ban_chat_array, age);
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
			CONSOLE_INFO("catch-exception:{}", json_data);
			ls2fep_client_login msg;
			msg.client_uid = client_uid;
			msg.eResult = e_error_code_login_invalid_bi;
			memcpy(msg.server_msg, json_data.c_str(), json_data.size() > max_server_msg_length ? max_server_msg_length : json_data.size());
			world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
		}
		return false;
	}

	void login_service_zw::save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array, int32 age)
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
			request.login_type = e_login_type_new_zw;
			request.login_type_plus = 0;
			memcpy(request.ban_role_array, ban_role_array, sizeof(request.ban_role_array));
			memcpy(request.ban_chat_array, ban_chat_array, sizeof(request.ban_chat_array));
			request.age = age;
			ws_client::getInstance().send_to_dp( &request, sizeof(request));
			//login日志
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


	void login_service_zw::ls2dp_save_server_log(const xstring & log_sql_name, const xstring & sql_param_string)
	{
		faith::server2dp_save_log req;
		int32 cpy_size = log_sql_name.size() > max_name_size ? max_name_size : log_sql_name.size();
		memcpy(req.sql_table_name, log_sql_name.c_str(), cpy_size);
		cpy_size = sql_param_string.size() > max_log_char_num ? max_log_char_num : sql_param_string.size();
		memcpy(req.save_string, sql_param_string.c_str(), cpy_size);
		ws_client::getInstance().send_to_dp( &req, sizeof(server2dp_save_log));
	}

	xstring login_service_zw::create_post_data(const xstring& data, int32 server_id, const xstring& game_id, const xstring& channel_id)
	{
		//return xstring("billingToken=") + data + xstring("&serverId=") + to_string(server_id) 
		//	+ xstring("&gameId=") + game_id
		//	+ xstring("&channelId=") + channel_id;

		return xstring("login_type=") + to_string(e_login_type_new_zw) + "&"
			+ "billingToken=" + data + "&serverId=" + to_string(server_id) + "&"
			+ "gameId=" + game_id + "&"
			+ "channelId=" + channel_id;
	}

	xstring login_service_zw::create_sign(i16 param_num,...)
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

	void login_service_zw::create_http_head_list(std::vector<xstring>& head_list,i16 param_num,...)
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
