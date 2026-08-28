/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_service
  file ext: cpp
  author: lxy
  
  purpose: 
*********************************************************************/
#include "recharge_service.hpp"
#include "recharge_ios.hpp"
#include "recharge_google.hpp"
#include "recharge_one_store.hpp"
#include "recharge_samsung.hpp"
namespace hld
{
	void recharge_service::req_recharge(const guid_64& role_guid, const xstring& goods_token, const int32 goods_id, const xstring order_id, const int32 payment_type, const xstring& packge_name)
	{
		s_recharge_info recharge_info;
		recharge_info.role_guid = role_guid;
		recharge_info.goods_token = goods_token;
		recharge_info.goods_id = goods_id;
		recharge_info.order_id = order_id;
		recharge_info.payment_type = payment_type;
		recharge_info.packge_name = packge_name;

		switch (payment_type)
		{
		case e_payment_method_test:
		{

		}
		break;
		case e_payment_method_zw:
		{

		}
		break;
		case e_payment_method_google:
		{
			recharge_google::getInstance().req_recharge(recharge_info);
		}
		break;
		case e_payment_method_one_store:
		{
			recharge_one_store::getInstance().req_recharge(recharge_info);
		}
		break;
		case e_payment_method_app_store:
		{
			recharge_ios::getInstance().req_recharge(recharge_info);
		}
		break;
		case e_payment_method_tw_android:
		{

		}
		break;
		case e_payment_method_tw_ios:
		{

		}
		break;
		case e_payment_method_tw_third:
		{

		}
		break;
		case e_payment_method_samsung:
		{
			recharge_samsung::getInstance().req_recharge(recharge_info);
		}
		break;
		case e_payment_method_en_ios:
		{

		}
		break;
		case e_payment_method_en_android:
		{

		}
		break;
		case e_payment_method_kr_gameloop:
		{

		}
		break;
		default:
			break;
		}
	}
}
