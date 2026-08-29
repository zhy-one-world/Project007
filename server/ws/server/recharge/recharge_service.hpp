/********************************************************************
	created:	2014/06/03
	created:	3:6:2014   13:35
	file base:	recharge_service
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _RECHARGE_SERVICE_H_
#define _RECHARGE_SERVICE_H_

#include "server_def.hpp"
#include <singleton.hpp>
#include "http/http_access_mgr.hpp"
#include "../recharge_mgr_ws.hpp"

namespace faith
{
	struct s_recharge_info
	{
		guid_64 role_guid;
		xstring goods_token;
		int32 goods_id;
		xstring order_id;
		int32 payment_type;
		xstring packge_name;
		s_recharge_info()
		{
			role_guid.clear_data();
			goods_token = "";
			goods_id = 0;
			order_id = "";
			payment_type = 0;
			packge_name = "";
		}
	};
	class recharge_service : public singleton<recharge_service>
	{
		friend class singleton<recharge_service>;
	public:
		void req_recharge(const guid_64& role_guid, const xstring& goods_token, const int32 goods_id, const xstring order_id, const int32 payment_type, const xstring& packge_name);
	};
}

#endif
