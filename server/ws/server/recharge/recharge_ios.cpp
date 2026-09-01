/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_ios
  file ext: cpp
  author: lxy
  
  purpose: 
*********************************************************************/
#include "recharge_ios.hpp"
#include "game_cfg/servers_config.h"
#include <Utility/init_unit.h>
#include "md5/md5.h"
#include <json/json.h>
#include "server_log.hpp"
namespace faith
{
	void recharge_ios::req_recharge(const s_recharge_info& recharge_info)
	{
		xstring sdk_url = "https://buy.itunes.apple.com/verifyReceipt";
		if (SERVERCONFIG->gm_state)
		{
			sdk_url = "https://sandbox.itunes.apple.com/verifyReceipt";
		}
		// 自定义请求头
		Json::Value root;
		root["receipt-data"] = recharge_info.goods_token;
		Json::FastWriter fast_writer;
		xstring url_para = fast_writer.write(root);
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type:application/json");
		head_list.push_back("Content-Length:" + init_unit::change_i64_to_string(url_para.size()));


		// 异步请求
		http_access_mgr::get_instance().async_request
			(
				recharge_info.role_guid.server_64,
				sdk_url,
				head_list,
				url_para,
				boost::bind(&recharge_ios::on_recharge_result_handle,this,_1,_2,_3,_4, recharge_info),
				e_http_request_type_post
			);
	}

	void recharge_ios::on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info)
	{
		CONSOLE_INFO("{}", http_result.c_str());

		//{"receipt":
		//			{"receipt_type":"Production", 
		//			"adam_id" : 1462632992,
		//			"app_item_id" : 1462632992, 
		//			"bundle_id" : "com.gameland.arka.ios",
		//			"application_version" : "121818",
		//			"download_id" : 97046569384523, 
		//			"version_external_identifier" : 834323470,
		//			"receipt_creation_date" : "2020-03-09 03:30:27 Etc/GMT",
		//			"receipt_creation_date_ms" : "1583724627000", 
		//			"receipt_creation_date_pst" : "2020-03-08 20:30:27 America/Los_Angeles",
		//			"request_date" : "2020-03-09 03:30:28 Etc/GMT", 
		//			"request_date_ms" : "1583724628837",
		//			"request_date_pst" : "2020-03-08 20:30:28 America/Los_Angeles",
		//			"original_purchase_date" : "2020-03-08 14:52:57 Etc/GMT", 
		//			"original_purchase_date_ms" : "1583679177000",
		//			"original_purchase_date_pst" : "2020-03-08 07:52:57 America/Los_Angeles",
		//			"original_application_version" : "121818",
		//			"in_app" : [
		//							{"quantity":"1", "product_id" : "104000307",
		//							"transaction_id" : "570000471483995",
		//							"original_transaction_id" : "570000471483995",
		//							"purchase_date" : "2020-03-09 03:30:27 Etc/GMT",
		//							"purchase_date_ms" : "1583724627000",
		//							"purchase_date_pst" : "2020-03-08 20:30:27 America/Los_Angeles",
		//							"original_purchase_date" : "2020-03-09 03:30:27 Etc/GMT",
		//							"original_purchase_date_ms" : "1583724627000", 
		//							"original_purchase_date_pst" : "2020-03-08 20:30:27 America/Los_Angeles", 
		//							"is_trial_period" : "false"
		//							}
		//						]
		//			},
		//"status" : 0,
		//"environment" : "Production" 
		//}

		if (http_error_code != 0 || http_result.size() == 0)
		{
			CONSOLE_INFO(" http error:{}", http_error_info);
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
		if (value["status"].asInt() != 0)
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(value["status"].asInt() + 1000000, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		MD5 md5;
		md5.update(recharge_info.goods_token.c_str(), recharge_info.goods_token.size());
		recharge_mgr_ws::get_instance().check_and_save_recharge(recharge_info.order_id.c_str(), recharge_info.role_guid, recharge_info.goods_id, 0, 0, recharge_info.payment_type, s_client_uid(), md5.toString(), 0, 0);
	}
}
