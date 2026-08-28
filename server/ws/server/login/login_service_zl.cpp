/********************************************************************
  created: 2014/08/05
  created: 5:8:2014 18:49
  file base: login_service_zl
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "login_service_zl.hpp"
#include "login_service_mgr.hpp"
#include "game_cfg/servers_config.h"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <login_msg.hpp>
#include <Utility/parse_msg.h>
#include <json/json.h>
#include <Utility/cs_date.hpp>
#include <time.hpp>
#include "server_log.hpp"
namespace hld
{
	const xstring sdk_password = "sdk";		// 固定密码

	void login_service_zl::tick(float deltaseconds)
	{
		m_time_count += deltaseconds;
	}

	void login_service_zl::req_login(const s_client_uid client_uid, const login_proto_login proto_data)
	{
		return;
		//if(data_ptr == NULL)
		//	return;

		//s_client_uid client_uid;
		//login_proto_login login;
		//parse_msg::getInstance().parse_message_new(&login, data_ptr, data_len);
		//parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		//if (login.login_type() != e_type_sdk_zl)
		//	return;

		//// URL
		//xstring sdk_url = SERVERCONFIG->login_url;

		//// channel_id
		//xstring channel_id = login.sdk_data().channel_id();

		//xstring opcode = login.sdk_data().opcode();

		//xchar tag_buff[32];
		//sprintf(tag_buff,"%d%d",client_uid.fepserver_uid,client_uid.fepsession_uid);
		//xstring tag = tag_buff;
		//tag = tag.substr(0,8);
		////sign
		//xstring sign = create_sign(SIGN_PARAM_NUM,opcode,login.sdk_data().data(),login.sdk_data().app_key(),login.sdk_data().app_secret(),tag,channel_id);

		//// 自定义请求头
		//std::vector<xstring> head_list;
		//xstring appkey = "appkey:" + login.sdk_data().app_key();
		//xstring sign_str = "sign:" + sign;
		//xstring tag_str = "tag:" + tag;
		//xstring opcode_str = "opcode:" + opcode;
		//xstring channel_id_str = "channelId:" + channel_id;
		//xstring client_ip_str = "clientIp:" + login.ip();
		//create_http_head_list(head_list,HTTP_HEAD_PARAM_NUM,appkey,sign_str,tag_str,opcode_str,channel_id_str, client_ip_str);

		//// GET/POST 参数
		//xstring url_para = create_post_data(login.sdk_data().data());

 	//	CONSOLE_INFO("sdk login data : " << time_helper::get_current_time() << " , " << hld::utility::get_tick_count()
 	//		<< ","<< appkey << "," << sign_str << "," << tag_str << "," << opcode_str << "," << channel_id_str << "," << client_ip_str << ","
 	//		<< login.sdk_data().app_secret() << "," << url_para);

		//// 异步请求
		//std::vector<xstring> url_vec;
		//url_vec.push_back(sdk_url);
		//login_proto_login_block temp_data_block;
		//temp_data_block.data = login;
		//temp_data_block.create_time = cs_date::get_local_time();
		//client_data_map::iterator temp_it = m_datas.find(client_uid);
		//if (temp_it != m_datas.end())
		//{
		//	m_datas.erase(temp_it);
		//}
		//m_datas.insert(std::make_pair(client_uid, temp_data_block));

		//http_access_mgr::get_instance().create_http(client_uid,CONNNECT_CURRENCY_MAX,url_para);
		//http_access_mgr::get_instance().async_request
		//	(
		//		client_uid,
		//		sdk_url,
		//		head_list,
		//		url_para,
		//		boost::bind(&login_service_zl::on_login_result_handle,this,_1,_2,_3,_4),
		//		e_request_type_post
		//	);
	}

	void login_service_zl::end_login(const dp2ls_client_login* pdata)
	{
		if (!pdata)
		{
			return;
		}
		CONSOLE_INFO(" sdk : dp to ls login end : state:{}", pdata->e_result);

		ls2fep_client_login request;
		request.eResult = pdata->e_result;
		request.client_uid = pdata->client_uid;
		request.login_type = pdata->login_type;
		request.login_type_plus = pdata->login_type_plus;
		request.jewel_num = pdata->jewel_num;
		request.is_create_new = pdata->is_create_new;
		memcpy(request.account, pdata->account, sizeof(pdata->account));
		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &request, sizeof(request));
	}

	void login_service_zl::on_login_result_handle(ui64 uid,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result)
	{
		CONSOLE_INFO("error_code:{} error_info:{} http_result:{}", http_error_code, http_error_info, http_result);
		s_client_uid client_uid(uid);
		http_access_mgr::get_instance().remove_http(client_uid);

		if(http_result.size() > 0)
		{
			// 先剥离http头
			if ( std::string::npos == http_result.find_first_of("{") || std::string::npos == http_result.find_last_of("}") )
			{
				CONSOLE_INFO("error_code:{} error_info:{} http_result:{}", http_error_code, http_error_info, http_result);
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



	xstring login_service_zl::write_json_data()
	{
		return "";
	}

	bool login_service_zl::read_json_data(const xstring& json_data,const s_client_uid client_uid)
	{
		Json::Reader reader;
		Json::Value  value;
		Json::Value  data_value;
		Json::Value  default_value;

		CONSOLE_INFO("sdk read json data:{}", json_data.c_str());

		try
		{
			if(json_data.size() > 0 && reader.parse(json_data,value))
			{
				if(value.isNull() || value.empty()  || value["data"].isNull() || value["data"].empty())
				{
					CONSOLE_INFO("billing respose format error:{}", json_data);
					return false;
				}
				else if(!value["state"].isInt() || value["state"].asInt() != 200 )
				{
					CONSOLE_INFO("billing repose op failed:{}", json_data);
					return false;
				}

				data_value = value.get("data",default_value);
				// 解析data数据
				if(data_value.empty() || data_value["status"].isNull() || data_value["status"].empty() || !data_value["status"].isString()
					|| data_value["userid"].isNull() || data_value["userid"].empty()  || !data_value["userid"].isString())
				{
					CONSOLE_INFO("billing data format error:{}", json_data);
					return false;
				}

				xstring status = data_value["status"].asString();
				xstring account = data_value["userid"].asString();

				// 检查登陆状态
				if(status == "1" && account.size() > 0)
				{ 
					// 存盘
					save_account(account,client_uid);
					return true;
				}
				else
				{
					CONSOLE_INFO("billing check failed:{}", json_data);
				}
			}
			else
			{
				CONSOLE_INFO("billing respose format error:{}", json_data);
			}
		}
		catch (...)
		{
			CONSOLE_INFO("  catch-exception:{}", json_data);
		}
		return false;
	}

	void login_service_zl::save_account(const xstring& account, const s_client_uid client_uid)
	{
		const login_proto_login* login_data = login_service_mgr::getInstance().get_login_info(client_uid.fep_uid_64);
		if (login_data == nullptr)
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
			request.login_type = -1;
			request.login_type_plus = 0;
			ws_client::getInstance().send_to_dp( &request, sizeof(request));
		}

	}

	xstring login_service_zl::create_post_data(const xstring& data)
	{
		return xstring("data=") + data;
	}

	xstring login_service_zl::create_sign(i16 param_num,...)
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

	void login_service_zl::create_http_head_list(std::vector<xstring>& head_list,i16 param_num,...)
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
