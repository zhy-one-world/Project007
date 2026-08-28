/********************************************************************
created:	2016年8月16日14
	file base : msgproc_mail
	file ext : hpp
	author : lxy

	purpose :
*********************************************************************/
#ifndef _DP_MSGPROC_MAIL_HPP_
#define _DP_MSGPROC_MAIL_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	//角色邮件
	void cs2dp_req_load_char_mail(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_mail_item(guid_64* item_guid, int32 item_num, int32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_mail_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_mail_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
}

#endif