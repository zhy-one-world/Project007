/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_google
  file ext: cpp
  author: lxy
  
  purpose: 
*********************************************************************/
#include "recharge_google.hpp"
#include "game_cfg/servers_config.h"
#include <Utility/init_unit.h>
#include "md5/md5.h"
#include <json/json.h>
#include "server_log.hpp"
namespace faith
{
	void recharge_google::req_recharge(const s_recharge_info& recharge_info)
	{
		xstring sdk_url = "https://accounts.google.com/o/oauth2/token";
		if (SERVERCONFIG->gm_state)
		{
			sdk_url = "https://accounts.google.com/o/oauth2/token";
		}
		// 自定义请求头
		xstring url_para = xstring("grant_type=refresh_token");
		url_para += "&client_id=1038072218930-05iud14ihnm7rbrd8pn2uivk51chgqou.apps.googleusercontent.com";
		url_para += "&client_secret=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";
		url_para += "&refresh_token=1//0eU1AvXNm5_S5CgYIARAAGA4SNwF-L9IrFMxAco3N9NXoohsB_rxxiGLyS0g9ywY_1K-KWdqgeXZQldyuy9gSmvH7XZku3s6EL54";
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
			boost::bind(&recharge_google::on_login_result_handle, this, _1, _2, _3, _4, recharge_info),
			e_http_request_type_post
		);
	}

	void recharge_google::on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info)
	{
		CONSOLE_INFO("{}", http_result.c_str());


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
	void recharge_google::on_login_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info)
	{
		CONSOLE_INFO("{}", http_result.c_str());
		//{
		//	"access_token": "ya29.a0Adw1xeXyK0OaxIeKTR3XJPPc2JjMGMCqJsIBINiN7PWYgyuxMvR6LOOOA4ZK_h6ui1bAOhh9KpJg-a_ZP_-2nogCR5iyU2stG9WLbW2sYAJnw3t7WtUgGK93WOmftqlCX_7DSnIXpyUUu_SD7NMBdu8-5jv34v27ImtwCA",
		//	"expires_in" : 3599,
		//	"scope" : "https://www.googleapis.com/auth/androidpublisher",
		//	"token_type" : "Bearer"
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
		if (reader.parse(http_result, value) == false || value.isNull() || value.empty() || value["access_token"].isNull())
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_json_parse_failed, 0, recharge_info.payment_type, recharge_info.role_guid, recharge_info.goods_id, "");
			return;
		}

		xstring sdk_url = "https://www.googleapis.com";
		if (SERVERCONFIG->gm_state)
		{
			sdk_url = "https://www.googleapis.com";
		}
		// /androidpublisher/v3/applications/com.ulugame.faithkr.google/purchases/products/104000103/tokens/niapkjjopmojcbnofhkpgppo.AO-J1Ox_3P58LQwjE2aXO5OHFYrk7icgpqBNmapeK_JNqgSeKY8j6_Xq02Bz1AY6BDoDEOuONGk9TRPrLcnmmI1WUFRDyNOpY0RI6tVQXfMOOjUq-ay7p5voyCaEBB16IZ-I1BDILSqZ?access_token=ya29.a0Adw1xeXeBte9xkMGm7eZnsFFY8NzCi0osJXQod5gR3x7gnniBRpoKMynYlb6DqttrE08eEyXTwZGrA3v3eldJ1AgZ8Z49Kh7jaXzECgROiZDnsZNHnthMPhLy-SzSgxz0tWYQTM4904wDszJ4_yuXO1DKqTpMrQlVtxBbA
		// 自定义请求头
		xstring url_para = "";
		url_para += "/androidpublisher/v3/applications/" + recharge_info.packge_name;
		url_para += "/purchases/products/" + init_unit::change_i64_to_string(recharge_info.goods_id);
		url_para += "/tokens/" + recharge_info.goods_token;
		url_para += "?access_token=" + value["access_token"].asString();
		std::vector<xstring> head_list;
		head_list.push_back("application/x-www-form-urlencoded");


		// 异步请求
		http_access_mgr::get_instance().async_request
		(
			recharge_info.role_guid.server_64,
			sdk_url,
			head_list,
			url_para,
			boost::bind(&recharge_google::on_recharge_result_handle, this, _1, _2, _3, _4, recharge_info),
			e_http_request_type_get
		);
	}

}
