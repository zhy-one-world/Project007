/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_one_store
  file ext: h
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _RECHARGE_ONE_STORE_HPP_
#define _RECHARGE_ONE_STORE_HPP_

#include "recharge_service.hpp"

namespace faith
{
	class recharge_one_store : public singleton<recharge_one_store>
	{
		friend class singleton<recharge_one_store>;
	public:
		recharge_one_store() {};
		~recharge_one_store()	{};

	public:
		virtual void req_recharge(const s_recharge_info& recharge_info);
		virtual void on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info);
	private:
		virtual void on_login_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info);
	};
}


#endif
