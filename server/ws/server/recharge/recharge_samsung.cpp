/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_samsung
  file ext: cpp
  author: lxy
  
  purpose: 
*********************************************************************/
#include "recharge_samsung.hpp"
#include "game_cfg/servers_config.h"
#include <Utility/init_unit.h>
#include "md5/md5.h"
#include <json/json.h>
#include "server_log.hpp"
namespace hld
{
	void recharge_samsung::req_recharge(const s_recharge_info& recharge_info)
	{
		xstring sdk_url = "https://iap.samsungapps.com";
		if (SERVERCONFIG->gm_state)
		{
			sdk_url = "https://iap.samsungapps.com";
		}
		// 自定义请求头
		xstring url_para = "";
		url_para += xstring("/iap/getPurchaseReceipt.as?protocolVersion=") + "5.0";
		url_para += "&purchaseID=" + recharge_info.goods_token;
		std::vector<xstring> head_list;
		head_list.push_back("Content-Type:application/x-www-form-urlencoded");
		//head_list.push_back("Content-Length:" + init_unit::change_i64_to_string(url_para.size()));


		// 异步请求
		http_access_mgr::get_instance().async_request
			(
				recharge_info.role_guid.server_64,
				sdk_url,
				head_list,
				url_para,
				boost::bind(&recharge_samsung::on_recharge_result_handle,this,_1,_2,_3,_4, recharge_info),
				e_http_request_type_get
			);
	}

	void recharge_samsung::on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info)
	{
		CONSOLE_INFO("{}", http_result.c_str());

		//{ itemId: '104001005',
		//	paymentId : '20200305112039G54669726X',
		//	orderId : 'P20200305KR33324030',
		//	itemName : '250다이아',
		//	itemDesc : '구매후 즉시 250다이아 획득\n',
		//	purchaseDate : '2020-03-05 11:22:03',
		//	paymentAmount : '7500.000',
		//	status : 'true',
		//	paymentMethod : 'Credit Card',
		//	mode : 'PRODUCTION',
		//	consumeYN : 'Y',
		//	consumeDate : '2020-03-05 11:22:10',
		//	consumeDeviceModel : 'SM-N935S',
		//	currencyCode : 'KRW',
		//	currencyUnit : '￦' }

		if (http_error_code != 0 || http_result.size() == 0)
		{
			CONSOLE_INFO("http error:{}", http_error_info);
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_http_verify_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		Json::Reader reader;
		Json::Value  value;
		Json::Value  data_value;
		if (reader.parse(http_result, value)== false || value.isNull() || value.empty() || value["status"].isNull())
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_json_parse_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		if (value["status"].asString() != "true")
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_http_verify_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		if (SERVERCONFIG->gm_state == false && value["mode"].asString() != "PRODUCTION")
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_http_verify_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		if (value["consumeYN"].asString() != "Y")
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_http_verify_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}
		MD5 md5;
		md5.update(recharge_info.goods_token.c_str(), recharge_info.goods_token.size());
		recharge_mgr_ws::get_instance().check_and_save_recharge(recharge_info.order_id.c_str(), recharge_info.role_guid, recharge_info.goods_id, 0, 0, recharge_info.payment_type, s_client_uid(), md5.toString(), 0, 0);
	}
}
