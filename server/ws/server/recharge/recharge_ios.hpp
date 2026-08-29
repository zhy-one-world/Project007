/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_ios
  file ext: h
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _RECHARGE_IOS_HPP_
#define _RECHARGE_IOS_HPP_

#include "recharge_service.hpp"

namespace faith
{
	class recharge_ios : public singleton<recharge_ios>
	{
		friend class singleton<recharge_ios>;
	public:
		recharge_ios() {};
		~recharge_ios()	{};

	public:
		virtual void req_recharge(const s_recharge_info& recharge_info);
		virtual void on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info);
	};
}


#endif
