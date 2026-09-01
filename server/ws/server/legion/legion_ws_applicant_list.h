#ifndef _WS_LEGION_WS_APPLICANT_LIST_H_
#define _WS_LEGION_WS_APPLICANT_LIST_H_

#include "legion_ws_applicant.h"

namespace faith
{
	class legion_ws_applicant_list
	{
	public:
		explicit legion_ws_applicant_list();
		~legion_ws_applicant_list();
		int32					get_empty_pos();
		bool					is_full();
		bool					exist_applicant(guid_64 applicant_guid);
		bool					can_add_member_applicant(guid_64 applicant_guid);
		e_add_applicant_ret		add_applicant(legion_ws_applicant& applicant);
		legion_ws_applicant*	get_applicant(guid_64 applicant_guid);
		void					del_applicant(guid_64 applicant_guid);
		int32					get_applicant_list_size();
		void					del_applicant_to_dp(guid_64 role_guid);

		void					save_to_db();

		void					tick(const uint64& tick_time);

		int32					fill_get_legion_applicant_list_msg(legion_proto_get_applicant_info_list_end& get_legion_applicant_list_end_msg);

		void					del_applicant_with_join_legion(guid_64 role_guid);//当玩家加入军团后，删除掉其所有的申请

	private:
		legion_ws_applicant*	m_applicant_ptr_arr[max_legion_applicant_num];

		uint64					m_tick_time;
	};
}

#endif