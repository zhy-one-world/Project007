/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: recharge_samsung
  file ext: h
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _RECHARGE_SAMMSUNG_HPP_
#define _RECHARGE_SAMMSUNG_HPP_

#include "recharge_service.hpp"

namespace faith
{
	class recharge_samsung : public singleton<recharge_samsung>
	{
		friend class singleton<recharge_samsung>;
	public:
		recharge_samsung() {};
		~recharge_samsung()	{};

	public:
		virtual void req_recharge(const s_recharge_info& recharge_info);
		virtual void on_recharge_result_handle(ui64 uid, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result, const s_recharge_info& recharge_info);
	};
}


#endif
