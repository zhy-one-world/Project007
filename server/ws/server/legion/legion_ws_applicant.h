#ifndef _WS_LEGION_WS_APPLICANT_H_
#define _WS_LEGION_WS_APPLICANT_H_


#include "legion.pb.h"
#include "Logic/legion_def.hpp"

namespace hld
{
	class legion_ws_applicant
	{
	public:
		explicit legion_ws_applicant();
		explicit legion_ws_applicant(guid_64 applicant_guid, guid_64 legion_guid);
		explicit legion_ws_applicant(s_legion_applicant_info applicant_info);

		void		save_to_db();

		bool		init_by_applicant_guid(guid_64 applicant_guid, guid_64 legion_guid);
		bool		is_valid() { return m_applicant_info.is_valid(); }
		void		clear_data();

		bool		is_out_of_data(const uint64& tick_time);

		void		set_applicant_info(ELegionApplicantInfo info_index, int32 value) { m_applicant_info.data_ary[info_index] = value; }
		int32		get_applicant_info(ELegionApplicantInfo info_index) { return m_applicant_info.data_ary[info_index]; }
		void		set_applicant_gs_value(int64 gs_value) { m_applicant_info.gs_value = gs_value; }
		int64		get_applicant_gs_value() { return m_applicant_info.gs_value; }
		guid_64&	get_applicant_guid() { return m_applicant_info.role_guid; }
		char*		get_applicant_name() { return m_applicant_info.role_name; }
		void		set_applicant_name(xstring role_name);
		guid_64&	get_legion_guid() { return m_applicant_info.legion_guid; }
		void		generate_player_legion_info(s_legion_member_info& member_info);

		bool		fill_legion_application_info_msg(legion_proto_applicant_info& applicant_info_msg);

		s_legion_applicant_info get_legion_applicant_info() { return m_applicant_info; };
		void		set_legion_applicant_info(s_legion_applicant_info info) { m_applicant_info = info; };

	private:
		s_legion_applicant_info m_applicant_info;
	};
}

#endif