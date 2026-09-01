/********************************************************************
created: 2019年03月27日
file base: csv_synchronization_mgr
file ext: h
author: wangdi

purpose: csv同步信息管理器
*********************************************************************/

#ifndef _CSV_SYNCHRONIZATION_MGR_H_
#define _CSV_SYNCHRONIZATION_MGR_H_
#include "template/template_manager.h"
#include "game.pb.h"
#include "client_session_mgr.hpp"
#include "client_session.hpp"


namespace faith
{
	class csv_synchronization_mgr
	{
	public:
		csv_synchronization_mgr();
		~csv_synchronization_mgr();
		static csv_synchronization_mgr& get_instance()
		{
			static csv_synchronization_mgr instance;
			return instance;
		}
		void								clear_data();
		void								send_template(client_session* session_ptr);
		void								send_oracle_trial_template(xstring table_name, client_session* session_ptr);
		void								send_title_template(xstring table_name, client_session* session_ptr);
		void								fill_title_template_one_msg(TitleTemplate* template_line, game_proto_template_table_data* activity_template_data);

	};
}

#endif
