/********************************************************************
created: 2018/09/08
created: 8:9:2018 18:48
file base: recharge_one_store
file ext: cpp
author: lxy

purpose:
*********************************************************************/
#include "recharge_one_store.hpp"
#include "game_cfg/servers_config.h"
#include <Utility/init_unit.h>
#include "md5/md5.h"
#include <json/json.h>
#include "server_log.hpp"
namespace hld
{
	void recharge_one_store::req_recharge(const s_recharge_info& recharge_info)
	{
		xstring sdk_url = "https://apis.onestore.co.kr/v2/oauth/token";
		if (SERVERCONFIG->gm_state)
		{
			sdk_url = "https://sbpp.onestore.co.kr/v2/oauth/token";
		}
		// 自定义请求头
		xstring url_para = xstring("client_id=") + "com.ulugames.faithkr.onestore";
		url_para += "&client_secret=0IyDZGyaFSwtolft0cz6gZyOnzvQ9Nf34y4/LShie+8=";
		url_para += "&grant_type=client_credentials";
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type:application/x-www-form-urlencoded");
		head_list.push_back("Content-Length:" + init_unit::change_i64_to_string(url_para.size()));


		// 异步请求
		http_access_mgr::get_instance().async_request
		(
			recharge_info.role_guid.server_64,
			sdk_url,
			head_list,
			url_para,
			boost::bind(&recharge_one_store::on_login_result_handle, this, _1, _2, _3, _4, recharge_info),
			e_http_request_type_post
		);
	}

	void recharge_one_store::on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info)
	{
		CONSOLE_INFO("{}", http_result.c_str());

		//{
		//	"consumptionState" : 1,
		//		"developerPayload" : "qinmian_onestore",
		//		"purchaseState" : 0,
		//		"purchaseTime" : 1584015997000
		//}
		if (http_error_code != 0 || http_result.size() == 0)
		{
			CONSOLE_INFO("http error:{}", http_error_info);
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_http_verify_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		Json::Reader reader;
		Json::Value  value;
		Json::Value  data_value;
		if (reader.parse(http_result, value) == false || value.isNull() || value.empty() || value["consumptionState"].isNull())
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_json_parse_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		if (value["consumptionState"].asInt() != 1)
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(value["consumptionState"].asInt() + 1000000, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}

		MD5 md5;
		md5.update(recharge_info.goods_token.c_str(), recharge_info.goods_token.size());
		recharge_mgr_ws::get_instance().check_and_save_recharge(recharge_info.order_id.c_str(), recharge_info.role_guid, recharge_info.goods_id, 0, 0, recharge_info.payment_type, s_client_uid(), md5.toString(), 0, 0);
	}
	void recharge_one_store::on_login_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info)
	{
		CONSOLE_INFO("{}", http_result.c_str());
		//{
		//	"status" : "SUCCESS",
		//		"client_id" : "com.ulugames.faithkr.onestore",
		//		"access_token" : "e8ad7411-c69d-4092-ac1f-1f37615818cb",
		//		"token_type" : "bearer",
		//		"expires_in" : 2718,
		//		"scope" : "DEFAULT"
		//}
		if (http_error_code != 0 || http_result.size() == 0)
		{
			CONSOLE_INFO("http error:{}", http_error_info);
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_http_verify_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		Json::Reader reader;
		Json::Value  value;
		Json::Value  data_value;
		if (reader.parse(http_result, value) == false || value.isNull() || value.empty() || value["status"].isNull())
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_json_parse_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		if (value["status"].asString() != "SUCCESS")
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_json_parse_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		xstring sdk_url = "https://apis.onestore.co.kr";
		if (SERVERCONFIG->gm_state)
		{
			sdk_url = "https://sbpp.onestore.co.kr";
		}
		// 自定义请求头
		xstring url_para = "";
		sdk_url += "/v2/purchase/details-by-productid/" + recharge_info.goods_token + "/com.ulugames.faithkr.onestore/" + init_unit::change_i64_to_string(recharge_info.goods_id);
		std::vector<xstring> head_list;
		head_list.push_back("Authorization:Bearer " + value["access_token"].asString());

		// 异步请求
		http_access_mgr::get_instance().async_request
		(
			recharge_info.role_guid.server_64,
			sdk_url,
			head_list,
			url_para,
			boost::bind(&recharge_one_store::on_recharge_result_handle, this, _1, _2, _3, _4, recharge_info),
			e_http_request_type_get
		);
	}

}
