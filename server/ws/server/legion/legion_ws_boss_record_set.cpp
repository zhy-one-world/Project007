#include "legion_ws_boss_record_set.h"
#include <legion_msg.hpp>
#include "legion_ws_mgr.h"
#include "template/template_manager.h"
#include "Logic/legion_def.hpp"
#include "Logic/activity_def.hpp"
#include "utility/guid_gen.h"
#include "ws_client.hpp"
#include "lua/script_mgr.h"
#include "system/scene/cs_map_mgr_system.h"
#include "net.pb.h"

namespace hld
{
	//void legion_ws_boss_record_set::add_boss_record(const s_legion_boss_record& boss_record)
	//{
		//m_boss_record_set.push_back(boss_record);
	//}

	void legion_ws_boss_record_set::add_boss_record(int32 template_id)
	{
		s_legion_boss_record new_boss_record;
		new_boss_record.legion_guid = m_legion_guid;
		new_boss_record.data_ary[ELegionBossRecord_template_id] = template_id;
		new_boss_record.data_ary[ELegionBossRecord_unlock] = 0;
		auto map_ent = cs_map_mgr_system::init_map_by_battle(template_id);
		if (nullptr != map_ent)
		{
			new_boss_record.boss_map_guid = map_ent->getEntityId();
		}
		else
		{
			CONSOLE_ERROR("init_map_by_battle failed, template_id = {}", template_id);
		}
		m_boss_record_set.push_back(new_boss_record);
	}

	s_legion_boss_record* legion_ws_boss_record_set::get_boss_record(int32 template_id)
	{
		int32 set_size = m_boss_record_set.size();
		if (set_size <= 0)
		{
			return nullptr;
		}

		for (int32 i = 0; i < set_size; ++i)
		{
			if (template_id == m_boss_record_set[i].data_ary[ELegionBossRecord_template_id])
			{
				return &(m_boss_record_set[i]);
			}
		}

		return nullptr;
	}

	void legion_ws_boss_record_set::init(guid_64 legion_guid)
	{
		m_legion_guid = legion_guid;
	}

	//void legion_ws_boss_record_set::refresh_data()
	//{
	//	for (s_legion_boss_record& record : m_boss_record_set)
	//	{
	//		record.data_ary[ELegionBossRecord_killed] = 0;
	//		record.clear_killer_name();
	//		record.boss_map_guid.clear_data();

	//		save_boss_record_to_db(record.data_ary[ELegionBossRecord_template_id]);
	//	}
	//}

	//void legion_ws_boss_record_set::save_boss_record_to_db(int32 boss_map_id)
	//{
	//	int32 record_num = m_boss_record_set.size();
	//	for (int32 i = 0; i < record_num; ++i)
	//	{
	//		if (m_boss_record_set[i].data_ary[ELegionBossRecord_template_id] == boss_map_id)
	//		{
	//			ws2dp_save_legion_boss_record save_boss_record_msg;
	//			save_boss_record_msg.boss_record = m_boss_record_set[i];
	//			ws_client::getInstance().send_to_dp(&save_boss_record_msg, sizeof(save_boss_record_msg));
	//			return;
	//		}
	//	}
	//}

	//void legion_ws_boss_record_set::clear_boss_record_to_db()
	//{
		//m_boss_record_set.clear();
		//ws2dp_clear_legion_boss_record clear_boss_record_msg;
		//clear_boss_record_msg.legion_guid = m_legion_guid;
		//ws_client::getInstance().send_to_dp(&clear_boss_record_msg, sizeof(clear_boss_record_msg));
	//}

	//void legion_ws_boss_record_set::load_from_db(const s_legion_boss_record* records_in_db, int32 records_num)
	//{
	//	if (records_num <= 0 || records_num > max_legion_boss_record_num)
	//	{
	//		return;
	//	}

	//	for (int32 i = 0; i < records_num; ++i)
	//	{
	//		add_boss_record(records_in_db[i]);
	//	}
	//}

	//void legion_ws_boss_record_set::set_boss_killed(int32 template_id, const xchar* killer_name)
	//{
	//	if (nullptr == killer_name)
	//	{
	//		return;
	//	}

	//	s_legion_boss_record* boss_record = get_boss_record(template_id);
	//	if (nullptr == boss_record)
	//	{
	//		add_boss_record(template_id);
	//		boss_record = get_boss_record(template_id);
	//	}

	//	boss_record->set_killer_name(killer_name);
	//	boss_record->data_ary[ELegionBossRecord_killed] = 1;
	//	boss_record->data_ary[ELegionBossRecord_unlock] = 1;
	//	boss_record->boss_map_guid.clear_data();

		//save_boss_record_to_db(template_id);
	//}

	//bool legion_ws_boss_record_set::is_boss_killed(int32 template_id)
	//{
	//	s_legion_boss_record* boss_record = get_boss_record(template_id);
	//	if (nullptr == boss_record)
	//	{
	//		return false;
	//	}

	//	return boss_record->data_ary[ELegionBossRecord_killed] > 0 ? true : false;
	//}

	//xchar* legion_ws_boss_record_set::get_killer_name(int32 template_id)
	//{
	//	s_legion_boss_record* boss_record = get_boss_record(template_id);
	//	if (nullptr == boss_record)
	//	{
	//		return nullptr;
	//	}

	//	return boss_record->killer_name;
	//}

	//bool legion_ws_boss_record_set::fill_legion_boss_record_list_msg(legion_proto_legion_boss_record_list& legion_boss_record_lisg_msg)
	//{
	//	/*	int32 record_num = m_boss_record_set.size();
	//		legion_boss_record_lisg_msg.set_record_num(record_num);
	//		for (int32 i = 0; i < record_num; ++i)
	//	  {
	//			if (nullptr == boss_record_msg)
	//			{
	//				return false;
	//			}*/
	//		legion_proto_legion_boss_record* boss_record_msg = legion_boss_record_lisg_msg.add_records();
	//		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(m_legion_guid);
	//		if (nullptr == legion_ws_ptr)
	//		{
	//			return false;
	//		}
	//		
	//		boss_record_msg->set_killer_name(legion_ws_ptr->get_kill_legion_boss_name());
	//		std::vector<int32> nothing = { 0,0,0,0 };
	//		for (int32 j = 0; j < ELegionBossRecord_max; ++j)
	//		{
	//			boss_record_msg->add_data_ary(nothing[j]);
	//		}
	//		//const s_legion_boss_record& boss_record = m_boss_record_set[i];
	//		//boss_record_msg->set_killer_name(boss_record.killer_name);
	//		//for (int32 j = 0; j < ELegionBossRecord_max; ++j)
	//		//{
	//			//boss_record_msg->add_data_ary(boss_record.data_ary[j]);
	//		//}
	//	

	//	return true;
	//}

	//bool legion_ws_boss_record_set::send_legion_boss_record_list(client_session* session)
	//{
	//	if (nullptr == session)
	//	{
	//		return false;
	//	}
	//	legion_proto_legion_boss_record_list legion_boss_record_list_msg;
	//	if (fill_legion_boss_record_list_msg(legion_boss_record_list_msg) == false)
	//	{
	//		return false;
	//	}
	//	session->send_to_client(&legion_boss_record_list_msg, e_msgindex_s2c_legion_boss_record_list);

	//	return true;
	//}


	///////////////////////// methods of legion_ws_boss_award_get_log //////////////////////////////////////////////

	void legion_ws_boss_award_get_log::save_legion_boss_award_get_log(guid_64 role_guid, int32 boss_map_id)
	{
		//ws2dp_save_legion_boss_award_get_log save_legion_boss_award_get_log_msg;
		//save_legion_boss_award_get_log_msg.role_guid = role_guid;
		//save_legion_boss_award_get_log_msg.boss_map_id = boss_map_id;
		//ws_client::getInstance().send_to_dp(&save_legion_boss_award_get_log_msg, sizeof(save_legion_boss_award_get_log_msg));	
		script_mgr::get_instance().call_func("ws2dp_send", "send_save_legion_boss_award_get_log_lua", 0, false, "%l%d", role_guid.server_64, boss_map_id);
	}

	void legion_ws_boss_award_get_log::load_legion_boss_award_get_log_all(guid_64 role_guid)
	{
		//ws2dp_load_all_legion_boss_award_get_log load_all_legion_boss_award_get_log_msg;
		//load_all_legion_boss_award_get_log_msg.role_guid = role_guid;
		//ws_client::getInstance().send_to_dp(&load_all_legion_boss_award_get_log_msg, sizeof(load_all_legion_boss_award_get_log_msg));
		script_mgr::get_instance().call_func("ws2dp_send", "send_load_all_legion_boss_award_get_log_lua", 0, false, "%l", role_guid.server_64);
	}

	void legion_ws_boss_award_get_log::set_role_legion_boss_award_get_log_from_db(const s_legion_boss_award_get_log& legion_boss_award_get_log)
	{
		guid_64 role_guid = legion_boss_award_get_log.role_guid;
		legion_boss_award_get_log_map::iterator ite = m_legion_boss_award_get_log_map.find(role_guid.server_64);
		if (ite == m_legion_boss_award_get_log_map.end())
		{
			m_legion_boss_award_get_log_map.insert({ role_guid.server_64, legion_boss_award_get_log });
		}
		else
		{
			ite->second = legion_boss_award_get_log;
		}
	}

	void legion_ws_boss_award_get_log::add_role_legion_boss_award_get_log(guid_64 role_guid, int32 boss_map_id)
	{
		legion_boss_award_get_log_map::iterator ite = m_legion_boss_award_get_log_map.find(role_guid.server_64);
		if (ite == m_legion_boss_award_get_log_map.end())
		{
			s_legion_boss_award_get_log legion_boss_award_get_log;
			legion_boss_award_get_log.role_guid = role_guid;
			legion_boss_award_get_log.get_logs[0] = boss_map_id;
			legion_boss_award_get_log.get_logs_num = 1;
			m_legion_boss_award_get_log_map.insert({ role_guid.server_64, legion_boss_award_get_log });
		}
		else
		{
			s_legion_boss_award_get_log& legion_boss_award_get_log = ite->second;
			if (legion_boss_award_get_log.get_logs_num >= max_legion_boss_record_num)
			{
				return;
			}
			legion_boss_award_get_log.get_logs[legion_boss_award_get_log.get_logs_num] = boss_map_id;
			++legion_boss_award_get_log.get_logs_num;
		}

		save_legion_boss_award_get_log(role_guid, boss_map_id);
	}

	bool legion_ws_boss_award_get_log::exist_role_legion_boss_award_get_log(guid_64 role_guid, int32 boss_map_id)
	{
		legion_boss_award_get_log_map::iterator ite = m_legion_boss_award_get_log_map.find(role_guid.server_64);
		if (ite == m_legion_boss_award_get_log_map.end())
		{
			return false;
		}
		
		s_legion_boss_award_get_log& legion_boss_award_get_log = ite->second;
		for (int32 i = 0; i < legion_boss_award_get_log.get_logs_num; ++i)
		{
			if (boss_map_id == legion_boss_award_get_log.get_logs[i])
			{
				return true;
			}
		}

		return false;
	}

	void legion_ws_boss_award_get_log::clear_role_legion_boss_award_get_log()
	{
		m_legion_boss_award_get_log_map.clear();
	}

	void legion_ws_boss_award_get_log::send_role_legion_boss_award_get_log(client_session* session)
	{
		guid_64 role_guid = session->get_role_guid();
		s_legion_boss_award_get_log* get_log = get_role_legion_boss_award_get_log(role_guid);
		if (nullptr == get_log)
		{
			return;
		}

		ws2cs_set_get_legion_boss_award_log set_get_legion_boss_award_log_msg;
		set_get_legion_boss_award_log_msg.legion_boss_award_get_log = *get_log;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_set_get_legion_boss_award_log);
		if (is_use == false)
		{
			session->send_to_cs(&set_get_legion_boss_award_log_msg, sizeof(set_get_legion_boss_award_log_msg));
		}
		else
		{
			hld::ws2cs_proto::set_get_legion_boss_award_log pro_msg;
			set_get_legion_boss_award_log_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_set_get_legion_boss_award_log);
		}

		legion_proto_set_player_legion_boss_award_get_log msg;
		msg.set_role_guid(role_guid.server_64);
		msg.set_get_logs_num(get_log->get_logs_num);
		for (int32 i = 0; i < get_log->get_logs_num; ++i)
		{
			msg.add_get_logs(get_log->get_logs[i]);
		}
		session->send_to_client(&msg, e_msgindex_s2c_set_player_legion_boss_award_get_log);
	}

	s_legion_boss_award_get_log* legion_ws_boss_award_get_log::get_role_legion_boss_award_get_log(guid_64 role_guid)
	{
		legion_boss_award_get_log_map::iterator ite = m_legion_boss_award_get_log_map.find(role_guid.server_64);
		if (ite == m_legion_boss_award_get_log_map.end())
		{
			return nullptr;
		}
		else
		{
			return &(ite->second);
		}
	}
}
