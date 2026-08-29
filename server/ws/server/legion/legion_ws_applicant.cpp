#include "legion_ws_applicant.h"
#include "../client_session_mgr.hpp"
#include "ws_client.hpp"
#include <legion_msg.hpp>
#include "Logic/time_def.hpp"
#include "time.hpp"
#include "utility/init_unit.h"
#include "net.pb.h"

namespace faith
{
	legion_ws_applicant::legion_ws_applicant() {}

	legion_ws_applicant::legion_ws_applicant(guid_64 applicant_guid, guid_64 legion_guid)
	{
		init_by_applicant_guid(applicant_guid, legion_guid);
	}

	legion_ws_applicant::legion_ws_applicant(s_legion_applicant_info applicant_info)
	{
		m_applicant_info = applicant_info;
	}

	void legion_ws_applicant::save_to_db()
	{
		//ws2dp_save_legion_applicant save_legion_applicant_msg;
		//save_legion_applicant_msg.applicant_info = m_applicant_info;
		//ws_client::getInstance().send_to_dp(&save_legion_applicant_msg, sizeof(save_legion_applicant_msg));

		server2dp_proto_ws2dp_save_legion_applicant msg;
		msg.set_legion_guid(m_applicant_info.legion_guid.server_64);
		msg.set_role_guid(m_applicant_info.role_guid.server_64);
		msg.set_role_name(m_applicant_info.role_name);
		msg.set_exist_time(m_applicant_info.exist_time);
		msg.set_gs_value(m_applicant_info.gs_value);
		for (int32 i = 0; i < ELegionApplicantInfo_max; i++)
		{
			msg.add_data_ary(m_applicant_info.data_ary[i]);
		}
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_legion_applicant);
	}

	bool legion_ws_applicant::init_by_applicant_guid(guid_64 applicant_guid, guid_64 legion_guid)
	{
		client_session* session = client_session_mgr::getInstance().get_session(applicant_guid);
		if (nullptr == session)
		{
			return false;
		}

		m_applicant_info.legion_guid = legion_guid;
		m_applicant_info.role_guid = session->get_role_guid();
		m_applicant_info.set_applicant_name(session->m_role_info.role_name);
		m_applicant_info.exist_time = utility::get_tick_count() + legion_apply_out_of_data_time;
		m_applicant_info.gs_value = session->get_role_gs_value();
		m_applicant_info.data_ary[ELegionApplicantInfo_level] = session->m_role_info.data_ary[e_role_info_exp_level];
		m_applicant_info.data_ary[ELegionApplicantInfo_player_template_id] = session->m_role_info.data_ary[e_role_info_template_id];
		m_applicant_info.data_ary[ELegionApplicantInfo_vip_level] = session->m_role_info.data_ary[e_role_info_vip_level];
		return true;
	}

	void legion_ws_applicant::clear_data()
	{
		m_applicant_info.clear_data();
	}

	bool legion_ws_applicant::is_out_of_data(const uint64& tick_time)
	{
		return m_applicant_info.exist_time <= tick_time;
	}

	void legion_ws_applicant::generate_player_legion_info(s_legion_member_info& member_info)
	{
		member_info.role_guid = get_applicant_guid();
		member_info.set_role_name(get_applicant_name());
		member_info.gs_value = get_applicant_gs_value();
		member_info.data_ary[e_legion_member_info_level] = get_applicant_info(ELegionApplicantInfo_level);
		member_info.data_ary[e_legion_member_info_player_template_id] = get_applicant_info(ELegionApplicantInfo_player_template_id);
		member_info.data_ary[e_legion_member_info_vip_level] = get_applicant_info(ELegionApplicantInfo_vip_level);
	}

	bool legion_ws_applicant::fill_legion_application_info_msg(legion_proto_applicant_info& applicant_info_msg)
	{
		guid_64& applicant_guid = get_applicant_guid();
		if (applicant_guid.is_valid() == false)
		{
			return false;
		}

		applicant_info_msg.set_applicant_guid(applicant_guid.server_64);
		applicant_info_msg.set_applicant_name(m_applicant_info.role_name);
		applicant_info_msg.set_gs_value(init_unit::change_i64_to_string(m_applicant_info.gs_value));
		for (int32 i = 0; i < ELegionApplicantInfo_vip_level; ++i)
		{
			applicant_info_msg.add_applicant_data(m_applicant_info.data_ary[i]);
		}

		return true;
	}

	void legion_ws_applicant::set_applicant_name(xstring role_name)
	{
		memset(m_applicant_info.role_name, 0, sizeof(role_name));
		memcpy(m_applicant_info.role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
	}
}
