#ifndef _WS_LEGION_WS_BOSS_RECORD_SET_H_
#define _WS_LEGION_WS_BOSS_RECORD_SET_H_


#include "legion.pb.h"
#include "Logic/legion_def.hpp"
#include "../client_session.hpp"


namespace hld
{
	typedef std::unordered_map<ui64, s_legion_boss_award_get_log> legion_boss_award_get_log_map;

	class legion_ws_boss_record_set
	{
	public:
		void								init(guid_64 legion_guid);

		s_legion_boss_record*				get_boss_record(int32 template_id);

		void								add_boss_record(int32 template_id);



	private:
		guid_64								m_legion_guid;
		std::vector<s_legion_boss_record>	m_boss_record_set;
	};

	class legion_ws_boss_award_get_log
	{
	public:
		void								save_legion_boss_award_get_log(guid_64 role_guid, int32 boss_map_id);
		void								load_legion_boss_award_get_log_all(guid_64 role_guid);

		void								set_role_legion_boss_award_get_log_from_db(const s_legion_boss_award_get_log& legion_boss_award_get_log);
		void								add_role_legion_boss_award_get_log(guid_64 role_guid, int32 boss_map_id);
		bool								exist_role_legion_boss_award_get_log(guid_64 role_guid, int32 boss_map_id);
		void								clear_role_legion_boss_award_get_log();

		void								send_role_legion_boss_award_get_log(client_session* session);

	private:
		s_legion_boss_award_get_log*		get_role_legion_boss_award_get_log(guid_64 role_guid);

	private:
		legion_boss_award_get_log_map		m_legion_boss_award_get_log_map;
	};
}

#endif