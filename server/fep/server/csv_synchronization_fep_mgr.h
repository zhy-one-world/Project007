/********************************************************************
created: 2020年01月07日
file base: csv_synchronization_fep_mgr
file ext: h
author: wangdi

purpose: csv同步信息管理器
*********************************************************************/

#ifndef _CSV_SYNCHRONIZATION_FEP_MGR_H_
#define _CSV_SYNCHRONIZATION_FEP_MGR_H_
#include "game.pb.h"
#include "client_session.hpp"


namespace hld
{
	class csv_synchronization_fep_mgr
	{
	public:
		csv_synchronization_fep_mgr();
		~csv_synchronization_fep_mgr();
		static csv_synchronization_fep_mgr& get_instance()
		{
			static csv_synchronization_fep_mgr instance;
			return instance;
		}
		void													clear_data();
		void													heart_tick(const int64& time_new);
		void													send_time_limit_template(client_session* session_ptr);
		void													add_time_limit_activity_template(xstring table_name);
		void													add_time_limit_branch_activity_template(xstring table_name);
		void													erase_from_send_csv_msg_map_by_session_uid(ui64 session_uid);
		bool													send_csv_msg_to_client(client_session* session_ptr, int32 msg_index);
		void													refresh_csv_template_msg_array();
	private:
		std::map<ui64, int32>									m_send_csv_player_map;
		std::vector<game_proto_set_template_table>				m_send_csv_template_msg_array;
	};
}

#endif
