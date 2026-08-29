#include "legion_ws_bonus_info.h"
#include "ws_client.hpp"
#include "legion_ws_mgr.h"
#include <legion_msg.hpp>
#include "net.pb.h"

namespace faith
{
	legion_ws_bonus_info::legion_ws_bonus_info()
	{
		clear_data();
	}
	legion_ws_bonus_info::~legion_ws_bonus_info()
	{
		clear_data();
	}

	void legion_ws_bonus_info::clear_data()
	{
		m_legion_guid.clear_data();
		m_bonus_info_map.clear();
		m_is_need_save_to_db = false;
	}

	void legion_ws_bonus_info::init(guid_64 cur_legion)
	{
		m_legion_guid = cur_legion;
	}

	void legion_ws_bonus_info::clear_legion_bonus_info_map()
	{
		m_bonus_info_map.clear();
		clear_legion_bonus_info_in_db();
	}

	void legion_ws_bonus_info::fill_legion_bonus_info_list_all(legion_proto_get_legion_bonus_info_end & get_legion_bonus_info_end_msg)
	{
		get_legion_bonus_info_end_msg.set_bonus_num(0);
		if (m_bonus_info_map.size() <= 0)
		{
			return;
		}
		int32 count = 0;
		std::unordered_map<int32, s_legion_bonus_info>::iterator ite;
		for (ite = m_bonus_info_map.begin(); ite != m_bonus_info_map.end(); ++ite)
		{

			legion_proto_legion_bonus_info* legion_bonus_info_msg = get_legion_bonus_info_end_msg.add_bonus_info();
			if (nullptr == legion_bonus_info_msg)
			{
				return;
			}
			fill_legion_bonus_info_list(*legion_bonus_info_msg, ite->second.mission_id, ite->second.finish_count);
			count++;
		}
		get_legion_bonus_info_end_msg.set_bonus_num(count);
	}

	void legion_ws_bonus_info::fill_legion_bonus_info_list(legion_proto_legion_bonus_info& legion_bonus_info, int32 mission_id, int32 finish_count)
	{
		legion_bonus_info.set_finish_count(finish_count);
		legion_bonus_info.set_mission_id(mission_id);
	}

	void legion_ws_bonus_info::add_legion_bonus_info_map(const s_legion_bonus_info& bonus_info)
	{
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(m_legion_guid);
		if (legion_ws_ptr == nullptr)
		{
			return;
		}
		m_is_need_save_to_db = true;
		m_bonus_info_map[bonus_info.mission_id] = bonus_info;
		legion_proto_legion_bonus_info	bonus_info_msg;
		bonus_info_msg.set_finish_count(bonus_info.finish_count);
		bonus_info_msg.set_mission_id(bonus_info.mission_id);
		legion_ws_ptr->send_message_to_all_member(&bonus_info_msg, e_msgindex_s2c_legion_bonus_info_one);
	}

	void legion_ws_bonus_info::recv_load_all_legion_bonus_info_from_db(const s_legion_bonus_info& bonus_info)
	{
		m_bonus_info_map.insert({ bonus_info.mission_id,bonus_info });
		//add_legion_bonus_info_map(*bonus_info);

	}
	s_legion_bonus_info& legion_ws_bonus_info::get_bonus_one(int32 mission_id)
	{
		s_legion_bonus_info& bonus_info = m_bonus_info_map[mission_id];
		if (!bonus_info.legion_guid.is_valid())
		{
			bonus_info.legion_guid = m_legion_guid;
			bonus_info.mission_id = mission_id;
		}
		return bonus_info;
	}

	void legion_ws_bonus_info::save_legion_bonus_info_into_db()
	{
		if (false == m_is_need_save_to_db)//±‹√‚Œﬁ”√¥Ê¥¢
		{
			return;
		}
		m_is_need_save_to_db = false;
		std::unordered_map<int32, s_legion_bonus_info>::iterator ite;
		for (ite = m_bonus_info_map.begin(); ite != m_bonus_info_map.end(); )
		{
			std::unordered_map<int32, s_legion_bonus_info>::iterator temp_ite = ite++;
			s_legion_bonus_info& bonus_info = temp_ite->second;
			if (false == bonus_info.is_valid())
			{
				continue;
			}
			//ws2dp_save_legion_bonus_info save_bonus_info_msg;
			//save_bonus_info_msg.bonus_info = bonus_info;
			//ws_client::getInstance().send_to_dp(&save_bonus_info_msg, sizeof(save_bonus_info_msg));

			server2dp_proto_ws2dp_save_legion_bonus_info msg;
			msg.set_legion_guid(bonus_info.legion_guid.server_64);
			msg.set_mission_id(bonus_info.mission_id);
			msg.set_finish_count(bonus_info.finish_count);
			ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_save_legion_bonus_info);
		}
	}

	void legion_ws_bonus_info::clear_legion_bonus_info_in_db()
	{
		//ws2dp_clear_legion_bonus_info clear_legion_bonus_info;
		//clear_legion_bonus_info.legion_guid = m_legion_guid;
		//ws_client::getInstance().send_to_dp(&clear_legion_bonus_info, sizeof(clear_legion_bonus_info));

		server2dp_proto_ws2dp_clear_legion_bonus_info msg;
		msg.set_legion_guid(m_legion_guid.server_64);
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_clear_legion_bonus_info);
	}
}