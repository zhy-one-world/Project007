#ifndef _WS_PERSON_INFOR_MGR_H_
#define _WS_PERSON_INFOR_MGR_H_

#include "Logic/chat_def.hpp"

namespace hld
{
	class person_infor_ws_mgr
	{
	public:
		explicit person_infor_ws_mgr();
		~person_infor_ws_mgr();

		static person_infor_ws_mgr& get_instance()
		{
			static person_infor_ws_mgr instance;
			return instance;
		}
		void sync_person_info(guid_64 role_guid, s_role_person_information person_info);

		std::vector<guid_64> get_relation_person(guid_64 role_guid);

		guid_64 get_one_relation_person(guid_64 role_guid);

		std::vector<guid_64>     random_relation_person(guid_64 role_guid);
		void	remove_person_info(guid_64 role_guid);

		void send_lucky_info_by_type(const guid_64& role_guid, e_lucky_info_req_type type);

	private:
		std::map<guid_64, s_role_person_information> person_info_map;
	};
}

#endif