#include "team_ws.h"
#include "team_ws_mgr.h"
#include "../legion/legion_ws_mgr.h"
#include "utility/guid_gen.h"
#include "utility/init_unit.h"
#include "time.hpp"
#include "logic/time_def.hpp"
#include "template/template_manager.h"
#include "../mail/event_ws_mgr.h"
#include "server_log.hpp"
#include "../activity/element_war_ws_mgr.h"
#include "system/scene/cs_map_mgr_system.h"
#include "base/ecs_world.h"
#include "system/scene/cs_map_system.h"
#include "components/scene/cs_map_component.h"
#include "chat.pb.h"
#include "net.pb.h"

namespace faith
{
	team_ws::team_ws(e_team_type tm_type)
	{
		clear_data();
		m_team_guid = guid_gen::make_guid();
		m_team_type = tm_type;
		init_team_state(m_team_type);
		m_is_dissolved = false;

		debug_log_team_operate("CR TM");
	}

	team_ws::~team_ws()
	{
		clear_data();
	}

	void team_ws::clear_data()
	{
		m_team_guid.clear_data();
		m_team_type = e_team_type_none; //kero
		m_team_sub_type_id = -1;
		m_team_map_guid.clear_data();
		m_captain_guid.clear_data();
		m_team_state = e_team_state_none;
		m_required_fighting_power = 0;
		m_required_level = 0;
		m_members_list.clear();
		m_member_sessions.clear();
		m_recruit_type = e_recurit_type_none;
		m_ready_transfer_info.clear_data();
		m_is_auto_matching = false;
		m_checked_ready_mem_num = -1;
		m_check_captain_offline_stamp = 0;
		m_one_stop_flags = -1;
		m_captain_invite_guid.clear();
		m_is_need_save_team = false;
	}

	void team_ws::tick(const uint64& time_now)
	{
		// 准备传送的tick逻辑
		ready_transfer_tick(time_now);
	}

	void team_ws::tick_1_sec(const uint64& time_now)
	{
		check_team_captain_offline(time_now);
	}
	
	void team_ws::tick_3_sec(const uint64& time_now)
	{
		send_all_team_member_position();
	}
	
	void team_ws::tick_1_min(const uint64& time_now)
	{
	}
	
	void team_ws::init_team_state(e_team_type team_type)
	{
		if (init_unit::is_team_type_raid_for_team(m_team_type))
		{
			set_team_state(e_team_state_waiting_join);
		}
		else
		{
			set_team_state(e_team_state_none);
		}
	}

	void team_ws::notice_add_member(FString member_name)
	{
		int32 notice_id = e_team_add_member_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(member_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_captain_guid, notice_str);
	}

	void team_ws::notice_del_member(FString member_name)
	{
		int32 notice_id = e_team_del_member_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(member_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_captain_guid, notice_str);
	}

	void team_ws::notice_change_captain(FString member_name)
	{
		int32 notice_id = e_team_change_captain_notice_id;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(member_name);
		std::string notice_str = init_unit::implode(vec_notice_str);
		int32 create_time = time_helper::get_cur_time_new().second;
		event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, m_captain_guid, notice_str);
	}

	bool team_ws::update_member(client_session* mem_session, s_team_member_info member_info)
	{
		for (s_team_member_info& member_info_ : m_members_list)
		{
			if (member_info_.role_guid == member_info.role_guid)
			{
				member_info_.data_ary[ETeamMemberInfo_one_stop_flags] = member_info.data_ary[ETeamMemberInfo_one_stop_flags];
			}
		}
		return true;
	}

	bool team_ws::add_member(client_session* mem_session, s_team_member_info member_info, bool check_condition)
	{
		if (can_team_state_add_member() == false)
		{
			return false;
		}

		guid_64 member_guid = member_info.role_guid;
		if (get_member(member_guid) != nullptr || team_ws_mgr::get_instance().get_unit_team(member_guid) != nullptr)
		{
			return false;
		}

		if (check_condition)
		{
			if (member_info.gs_value < get_required_fighting_power())
			{
				return false;
			}
			if (member_info.data_ary[ETeamMemberInfo_level] < get_required_level())
			{
				return false;
			}
		}

		m_members_list.push_back(member_info);
		team_ws_mgr::get_instance().set_unit_team(member_guid, m_team_guid);
		mem_session->set_team_guid(m_team_guid);
		m_member_sessions[member_guid.server_64] = mem_session;

		// 更新队伍人数(例如搜索附近的队伍和组队经验分享这类功能需要用到这个信息)
		send_all_member_aoi_team_info();
		update_recruit_info_to_client();
		
		//这里member_info里的ETeamMemberInfo_check_ret都已经是cs上检测后的结果 不用再检测
		//if (member_guid != m_captain_guid)
		//{
		//	client_session* session = client_session_mgr::getInstance().get_session(member_guid);
		//	if (nullptr == session)
		//	{
		//		return false;
		//	}
		//	team_ws_mgr::get_instance().req_condition_check_to_cs(session, e_team_common_check_invoker_up_mem_check_ret, m_team_type, m_team_sub_type_id, m_team_guid);
		//}

		if (get_member_num() >= max_team_member_num)
		{
			stop_auto_match();
			add_assist_fight_notice();
		}
		
		debug_log_team_operate("ADD M");

		server_log::join_team_log(m_captain_guid, get_member_num(), get_team_type(), member_guid);


		return true;
	}

	void team_ws::update_recruit_info_to_client(bool is_delete)
	{
		chat_proto_recruit_team_member_num msg;
		msg.set_team_guid(get_team_guid().server_64);
		if (is_delete)
		{
			msg.set_cur_member_num(0);
		}
		else
		{
			msg.set_cur_member_num(get_member_num());
		}
		switch (m_recruit_type)
		{
		case e_recurit_type_legion_recurit:
		{
			guid_64 captain_guid = get_captain_guid();
			legion_ws* temp_legion = legion_ws_mgr::get_instance().get_unit_legion(captain_guid);
			if (temp_legion != nullptr)
			{
				//const std::list<s_legion_member_info>& temp_member_list = temp_legion->get_member_list();
				//for (const s_legion_member_info& legion_member : temp_member_list)
				//{
				//	guid_64 temp_addressee_guid = legion_member.role_guid;
				//	client_session* temp_session = client_session_mgr::getInstance().get_session(temp_addressee_guid);
				//	if (temp_session != nullptr)
				//	{
				//		temp_session->send_to_client(&msg, e_msgindex_s2c_receive_recruit_message);
				//	}
				//}
				temp_legion->send_message_to_all_member(&msg, e_msgindex_s2c_update_recruit_member_num);
			}
		}
		break;
		case e_recurit_type_world_recurit:
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_update_recruit_member_num);
		}
		break;
		default:
			break;
		}
	}

	bool team_ws::del_member(guid_64 player_guid)
	{
		if (m_members_list.size() <= 0)
		{
			return false;
		}

		std::list<s_team_member_info>::iterator ite;
		for (ite = m_members_list.begin(); ite != m_members_list.end();)
		{
			if (ite->role_guid != player_guid)
			{
				++ite;
				continue;
			}
			s_team_member_info*	team_member_info_ptr = get_member(player_guid);
			if (nullptr == team_member_info_ptr)
			{
				return false;
			}
			xstring member_name = xstring(team_member_info_ptr->role_name);

			if (get_captain_guid() == player_guid)
			{
				if (m_members_list.size() > 1 && is_all_other_member_offline(player_guid) == false)
				{
					for (s_team_member_info& team_member_info : m_members_list)
					{
						if (team_member_info.role_guid == player_guid)
							continue;
						if (team_member_info.data_ary[ETeamMemberInfo_is_online] == 0)
							continue;

						guid_64& new_captain_guid = team_member_info.role_guid;
						set_captain(new_captain_guid);
						send_all_member_aoi_team_info();
						debug_log_team_operate("CH CP");
						break;
					}
				}
			}

			//先删掉这个玩家数据
			ite = m_members_list.erase(ite);
			del_member_common_op(player_guid);

			server_log::quit_team_log(m_captain_guid, get_member_num(), get_team_type(), player_guid, 1);
			

			// 如果队伍已经没人了就可以解散队伍,把队伍从内存中清理掉了
			if (m_members_list.size() <= 0 || is_all_member_offline())
			{
				dissolve_team();
				return true;
			}
			
			// 更新队伍人数
			send_all_member_aoi_team_info();
			update_recruit_info_to_client();
			notice_del_member(member_name);
			
			clear_all_member_ready_state();
			
			debug_log_team_operate("DEL M");

			return true;
		}
		
		return false;
	}

	void team_ws::del_member_common_op(guid_64 player_guid)
	{
		client_session* mem_session = client_session_mgr::getInstance().get_session(player_guid);
		if (nullptr != mem_session)
		{
			mem_session->clear_team_guid();
		}
		m_member_sessions.erase(player_guid.server_64);

		team_ws_mgr::get_instance().clear_unit_team(player_guid);

		send_del_team_member_message(player_guid);
	}

	void team_ws::clear_member()
	{
		for (s_team_member_info& team_member_info : m_members_list)
		{
			del_member_common_op(team_member_info.role_guid);
		}
		m_members_list.clear();
		m_captain_invite_guid.clear();
	}

	s_team_member_info* team_ws::get_member(guid_64 player_guid)
	{
		for (s_team_member_info& team_member_info : m_members_list)
		{
			if (team_member_info.role_guid == player_guid)
			{
				return &(team_member_info);
			}
		}

		return nullptr;
	}

	int32 team_ws::get_member_num() const
	{
		return m_members_list.size();
	}

	void team_ws::check_team_captain_offline(const uint64& time_now)
	{
		if (m_check_captain_offline_stamp == 0 || time_now < m_check_captain_offline_stamp)
		{
			return;
		}
		
		if (is_all_member_offline() && !m_is_need_save_team)
		{
			dissolve_team();
			return;
		}
		if (!is_all_member_offline())
		{
			m_is_need_save_team = false;
		}

		change_captain();
		
		m_check_captain_offline_stamp = 0;
	}

	void team_ws::set_member_online(client_session* mem_session, bool is_online)
	{
		guid_64 player_guid = mem_session->get_role_guid();

		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return;
		}
		
		if (true == is_online)
		{
			member_info->data_ary[ETeamMemberInfo_is_online] = 1;
		}
		else
		{
			member_info->data_ary[ETeamMemberInfo_is_online] = 0;
		}

		send_team_member_info_one(player_guid, ETeamMemberInfo_is_online);
	}

	bool team_ws::get_member_online(guid_64 player_guid)
	{
		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return false;
		}
		return member_info->data_ary[ETeamMemberInfo_is_online] > 0 ? true : false;
	}

	bool team_ws::is_all_member_offline()
	{
		if (m_members_list.size() <= 0)
		{
			return true;
		}
		for (s_team_member_info& member_info : m_members_list)
		{
			if (member_info.data_ary[ETeamMemberInfo_is_online] > 0)
			{
				return false;
			}
		}
		
		return true;
	}

	bool team_ws::is_all_other_member_offline(guid_64 player_guid)
	{
		if (m_members_list.size() <= 0)
		{
			return true;
		}
		for (s_team_member_info& member_info : m_members_list)
		{
			if (member_info.role_guid == player_guid)
				continue;
			if (member_info.data_ary[ETeamMemberInfo_is_online] > 0)
			{
				return false;
			}
		}

		return true;
	}
	
	void team_ws::set_member_check_ret(guid_64 player_guid, e_error_code check_ret)
	{
		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return;
		}

		if (member_info->data_ary[ETeamMemberInfo_check_ret] == check_ret)
		{
			return;
		}
		
		member_info->data_ary[ETeamMemberInfo_check_ret] = check_ret;
		// 由于现在客户端依赖one_stop_flags 计算check_ret了 不用同步这个了
		//send_team_member_info_one(player_guid, ETeamMemberInfo_check_ret);
	}

	e_error_code team_ws::get_member_check_ret(guid_64 player_guid)
	{
		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return e_error_code_success;
		}
		return (e_error_code)member_info->data_ary[ETeamMemberInfo_check_ret];
	}

	void team_ws::reset_mems_check_ret()
	{
		if (m_members_list.size() <= 0)
		{
			return;
		}
		for (s_team_member_info& member_info : m_members_list)
		{
			member_info.data_ary[ETeamMemberInfo_check_ret] = e_error_code_success;
		}
	}
	
	void team_ws::set_member_one_stop_flags(guid_64 player_guid, int32 _flags)
	{
		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return;
		}
		
		if (member_info->data_ary[ETeamMemberInfo_one_stop_flags] == _flags)
		{
			return;
		}

		member_info->data_ary[ETeamMemberInfo_one_stop_flags] = _flags;
		send_team_member_info_one(player_guid, ETeamMemberInfo_one_stop_flags);
	}

	int32 team_ws::get_member_one_stop_flags(guid_64 player_guid)
	{
		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr == member_info)
		{
			return -1;
		}
		return member_info->data_ary[ETeamMemberInfo_one_stop_flags];
	}

	void team_ws::dissolve_team()
	{
		//team_ws_mgr::get_instance().del_team(m_team_guid);
		m_is_dissolved = true;
	}

	void team_ws::on_team_delete()
	{
		clear_member();
		update_recruit_info_to_client(true);
		
		debug_log_team_operate("DE TM");

		server_log::quit_team_log(m_captain_guid, get_member_num(), get_team_type(), 0, 0);
	}

	void team_ws::on_team_mem_online(client_session* session)
	{
		guid_64 player_guid = session->get_role_guid();
		if (player_guid == m_captain_guid)
		{
			m_check_captain_offline_stamp = 0;
		}
		else
		{
			m_check_captain_offline_stamp = utility::get_tick_count();
		}
		m_is_need_save_team = false;
		set_member_online(session, true);
		send_team_info_to_session(session);
		send_aoi_team_info(session);
	}

	void team_ws::on_team_mem_offline(client_session* session)
	{
		guid_64 player_guid = session->get_role_guid();
		if (player_guid == m_captain_guid)
		{
			m_check_captain_offline_stamp = utility::get_tick_count() + cap_offline_check_sec * second_tick_time;
		}
			
		set_member_online(session, false);
		//member_info->data_ary[ETeamMemberInfo_follow_state] = 0;
	}

	void team_ws::on_team_mem_login(client_session* session)
	{
		guid_64 player_guid = session->get_role_guid();
		session->set_team_guid(m_team_guid);
		m_member_sessions[player_guid.server_64] = session;

		on_team_mem_online(session);
	}

	void team_ws::on_team_mem_logout(client_session* session)
	{
		guid_64 player_guid = session->get_role_guid();
		session->clear_team_guid();
		m_member_sessions.erase(player_guid.server_64);

		on_team_mem_offline(session);
	}

	void team_ws::update_member_map(client_session* session, guid_64 map_guid)
	{
		ZoneScoped;
		auto map_ent = get_entity(map_guid);
		if (nullptr == session || nullptr == map_ent)
		{
			CONSOLE_ERROR("update_member_map failed, session or map_ent is nullptr");
			return;
		}
		auto map_cp = map_ent->get_component<cs_map_component>();
		guid_64 player_guid = session->get_role_guid();
		s_team_member_info* member_info = get_member(player_guid);
		if (nullptr != member_info)
		{
			member_info->data_ary[ETeamMemberInfo_cur_map] = map_cp->m_map_template_id;
			member_info->data_ary[ETeamMemberInfo_line_id] = map_cp->m_line_id;
			send_team_member_info_one(player_guid, ETeamMemberInfo_cur_map);
			send_team_member_info_one(player_guid, ETeamMemberInfo_line_id);

			member_info->data_ary[ETeamMemberInfo_PosX] = session->get_role_info_data(e_role_info_move_pos_x);
			member_info->data_ary[ETeamMemberInfo_PosY] = session->get_role_info_data(e_role_info_move_pos_y);
			member_info->data_ary[ETeamMemberInfo_PosZ] = session->get_role_info_data(e_role_info_move_pos_z);

			send_all_team_member_position();
		}
	}

	bool team_ws::set_captain(guid_64 player_guid)
	{
		// 如果指定的成员已经是队长就不必重复设置为队长
		if (player_guid == m_captain_guid)
		{
			return false;
		}

		// 如果指定的成员不在队伍中就返回
		if (get_member(player_guid) == nullptr)
		{
			return false;
		}
		
		m_captain_guid = player_guid;
		clear_all_member_ready_state();
		
		if (is_one_stop_team())
		{
			e_team_type new_team_type;
			int32 new_sub_type_id;
			int32 cap_os_flags = get_member_one_stop_flags(player_guid);
			set_one_stop_flags(cap_os_flags);
			team_ws_mgr::get_instance().get_team_aims_by_os_flags(cap_os_flags, new_team_type, new_sub_type_id);
			change_team_aim(new_team_type, new_sub_type_id);
			sync_team_attribute_to_mems();
		}

		send_set_captain_message(player_guid);
		send_all_member_aoi_team_info();

		return true;
	}

	bool team_ws::change_captain()
	{
		for (s_team_member_info& team_member_info : m_members_list)
		{
			//if (ite.role_guid == m_captain_guid)
			//{
			//	continue;
			//}
			if (team_member_info.data_ary[ETeamMemberInfo_is_online] == 0)
			{
				continue;
			}
			guid_64& new_captain_guid = team_member_info.role_guid;
			set_captain(new_captain_guid);
			
			debug_log_team_operate("CH CP");
			return true;
		}

		return false;
	}

	void team_ws::set_member_ready_status(guid_64 member_guid, bool mem_ready, e_error_code check_ret)
	{
		s_team_member_info* member_info = get_member(member_guid);
		if (nullptr == member_info)
		{
			return;
		}
		
		if (member_guid == m_captain_guid)
		{
			// 队长发起准备或放弃
			clear_all_member_ready_state();
			if (init_unit::is_team_type_raid_for_team(get_team_type()))
			{
				stop_auto_match();
			}

			if (mem_ready)
			{
				if (false == is_mem_cur_map_can_ready_trans(*member_info))
				{
					return;
				}
				// 上一版逻辑 排除掉已经在其他副本的成员 队长和目前可以准备的成员一起进本
				// check_and_set_mem_ready_state函数里会先把不能传送的玩家的状态发给他们 保证比下面的队长准备消息先到客户端
				//m_checked_ready_mem_num = check_and_set_mem_ready_state();
				check_and_set_members_ready_state();
				m_checked_ready_mem_num = 1;
				set_team_state(e_team_state_waiting_ready);
			}
		}
		else
		{
			// 如果队伍当前的状态不能让队员改变准备状态就返回
			if (can_team_state_ready() == false)
			{
				return;
			}
			
			if (member_info->data_ary[ETeamMemberInfo_ready_state] != e_team_mem_ready_state_waiting_ready)
			{
				return;
			}

			if (mem_ready)
			{
				m_checked_ready_mem_num++;
			}
		}

		int32 mem_ready_code = e_team_mem_ready_state_none;
		if (mem_ready && (check_ret == e_error_code_success || check_ret == e_error_code_map_lack_of_times))
		{
			if (is_mem_cur_map_can_ready_trans(*member_info))
			{
				mem_ready_code = e_team_mem_ready_state_ready;
			}
			else
			{
				check_ret = e_error_code_map_cur_map_cant_ready;
			}
		}
		member_info->data_ary[ETeamMemberInfo_ready_state] = mem_ready_code;
		member_info->data_ary[ETeamMemberInfo_check_ret] = check_ret;
		
		int32 ready_state_code = (true == mem_ready ? 1 : 0);
		
		team_proto_ready_end ready_end_msg;
		ready_end_msg.set_team_guid(m_team_guid.server_64);
		ready_end_msg.set_member_guid(member_guid.server_64);
		ready_end_msg.set_is_ready(ready_state_code);
		ready_end_msg.set_trans_check_ret(check_ret);
		send_message_to_all_member(&ready_end_msg, e_msgindex_s2c_ready_end);
		
		try_start_team_aim();
	}

	void team_ws::try_start_team_aim()
	{
		if (false == can_team_state_start_transfer())
		{
			return;
		}

		if (m_checked_ready_mem_num < get_member_num())
		{
			return;
		}
		
		// 有队员没有全部准备就不能开始
		if (is_all_member_ready() == false)
		{
			return;
		}
		if (get_team_type() == e_team_type_element_war)
		{
			element_war_ws_mgr::get_instance().send_team_ready_end(get_team_guid());
		}
		else if (team_ws_mgr::get_instance().is_team_type_raid(m_team_type))
		{
			ready_transfer_all_member_to_scene();
		}
		else
		{
			sync_to_cs_start_team_aim();
			clear_all_member_ready_state();
		}
	}

	void team_ws::sync_to_cs_start_team_aim()
	{
		if (m_members_list.size() <= 0)
		{
			return;
		}

		ws2cs_sync_team_start_aim sync_team_start_aim_msg;
		
		for (const s_team_member_info& member_info : m_members_list)
		{
			client_session* session = client_session_mgr::getInstance().get_session(member_info.role_guid);
			if (nullptr == session)
			{
				continue;
			}
			sync_team_start_aim_msg.role_guid = member_info.role_guid;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_sync_team_start_aim);
			if (is_use == false)
			{
				session->send_to_cs(&sync_team_start_aim_msg, sizeof(sync_team_start_aim_msg));
			}
			else
			{
				faith::ws2cs_proto::sync_team_start_aim pro_msg;
				sync_team_start_aim_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_sync_team_start_aim);
			}
		}
	}

	bool team_ws::is_all_member_ready()
	{
		for (const s_team_member_info& member_info : m_members_list)
		{
			// 上一版逻辑 排除掉已经在其他副本的成员 队长和目前可以准备的成员一起进本
			//if (member_info.data_ary[ETeamMemberInfo_ready_state] == e_team_mem_ready_state_waiting_ready)
			//{
			//	return false;
			//}

			// 这一版逻辑 必须等所有在队伍中的成员可以进本才算
			if (member_info.data_ary[ETeamMemberInfo_ready_state] != e_team_mem_ready_state_ready)
			{
				return false;
			}

			if (false == is_mem_cur_map_can_ready_trans(member_info))
			{
				return false;
			}
		}
		return true;
	}

	void team_ws::clear_all_member_ready_state()
	{
		if (m_members_list.size() <= 0)
		{
			return;
		}
		for (s_team_member_info& member_info : m_members_list)
		{
			member_info.data_ary[ETeamMemberInfo_ready_state] = e_team_mem_ready_state_none;
		}
		
		init_team_state(m_team_type);
		m_checked_ready_mem_num = -1;
	}

	int32 team_ws::check_and_set_members_ready_state()
	{
		int32 valid_mem = 0;

		if (m_members_list.size() <= 0)
		{
			return valid_mem;
		}
		
		// 上一版逻辑 排除掉已经在其他副本的成员 队长和目前可以准备的成员一起进本
		//team_proto_ready_end ready_end_msg;
		//ready_end_msg.set_team_guid(m_team_guid.server_64);
		//ready_end_msg.set_is_ready(1);
		//ready_end_msg.set_trans_check_ret(e_error_code_map_cur_map_cant_ready);
		//for (s_team_member_info& member_info : member_list)
		//{
		//	if (is_mem_cur_map_can_ready_trans(member_info))
		//	{
		//		member_info.data_ary[ETeamMemberInfo_ready_state] = e_team_mem_ready_state_waiting_ready;
		//		valid_mem++;
		//	}
		//	else
		//	{
		//		member_info.data_ary[ETeamMemberInfo_ready_state] = e_team_mem_ready_state_none;
		//		ready_end_msg.set_member_guid(member_info.role_guid.server_64);
		//		send_message_to_all_member(&ready_end_msg, e_msgindex_s2c_ready_end);
		//	}
		//}

		for (s_team_member_info& member_info : m_members_list)
		{
			member_info.data_ary[ETeamMemberInfo_ready_state] = e_team_mem_ready_state_waiting_ready;
			valid_mem++;
		}

		return valid_mem;
	}

	bool team_ws::is_mem_cur_map_can_ready_trans(const s_team_member_info& mem_info)
	{
		MapTemplate const* map_template = GET_TEMPLATE(MapTemplate, mem_info.data_ary[ETeamMemberInfo_cur_map]);
		if (nullptr == map_template)
		{
			return false;
		}

		bool is_hold_map = init_unit::get_map_public(map_template->Type);
		return is_hold_map;
	}

	void team_ws::ready_transfer_all_member_to_scene()
	{
		if (can_team_state_start_transfer() == false)
		{
			return;
		}

		auto team_guid = get_team_guid();
		if (team_guid.is_valid() == false)
		{
			return;
		}

		if (!team_ws_mgr::get_instance().is_team_type_raid(m_team_type))
		{
			return;
		}

		// 一旦发起传送请求后就要设置新的队伍状态以避免有其他玩家加入或退出退伍
		set_team_state(e_team_state_game);
		set_ready_transfer_info(get_team_map_tempalte_id());

		faith::team_proto_ready_to_transfer_team_end ready_transfer_team_end_msg;
		ready_transfer_team_end_msg.set_team_guid(team_guid.server_64);
		ready_transfer_team_end_msg.set_sec(ready_transfer_sec);
		send_message_to_all_member(&ready_transfer_team_end_msg, e_msgindex_s2c_ready_to_transfer_team_end);
	}

	void team_ws::set_ready_transfer_info(int32 map_template_id)
	{
		if (is_ready_transfer_now() == true)
		{
			return;
		}

		// 检查地图ID是否合法
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}

		m_ready_transfer_info.is_active = true;
		m_ready_transfer_info.ready_time = utility::get_tick_count() + ready_transfer_sec * second_tick_time;
		m_ready_transfer_info.member_num = get_member_num();
		m_ready_transfer_info.map_template_id = map_template_id;
	}

	void team_ws::ready_transfer_tick(const uint64& time_now)
	{
		if (is_ready_transfer_now() == false)
		{
			return;
		}

		if (m_ready_transfer_info.ready_time <= time_now)
		{
			m_ready_transfer_info.is_active = false;
			
			if (false == check_can_start_transfer() || 
				e_team_error_none != transfer_all_member_to_scene(m_ready_transfer_info.map_template_id))
			{
				clear_all_member_ready_state();
			}
		}
	}

	bool team_ws::check_can_start_transfer()
	{
		if (get_member_num() != m_ready_transfer_info.member_num)
		{
			send_team_error_to_all_member(e_team_error_member_change_during_transfer);
			return false;
		}

		//if (false == is_all_member_ready())
		//{
		//	send_team_error_to_all_member(e_team_error_member_change_during_transfer);
		//	return false;
		//}

		//switch (m_team_type)
		//{
		//case faith::e_team_type_act_broken_sky:
		//{
		//	if (e_error_code_success != broken_sky_mgr::get_instance().is_can_join())
		//	{
		//		send_team_error_to_all_member(e_team_error_raid_condition_less);
		//		return false;
		//	}
		//	break;
		//}
		//default:
		//	break;
		//}
		return true;
	}

	e_team_error team_ws::transfer_all_member_to_scene(int32 map_template_id, bool need_ready)
	{
		if (m_members_list.size() <= 0)
		{
			CONSOLE_ERROR("team_ws::transfer_all_member_to_scene m_members_list.size() <= 0");
			return e_team_error_none;
		}

		Entity* map_ent = nullptr;
		if (init_unit::is_team_type_raid_for_team(m_team_type))
		{
			map_ent = cs_map_mgr_system::init_map_by_battle(map_template_id);
			if (map_ent)
			{
				cs_map_system::set_own_team_guid(map_ent, m_team_guid);
			}
		}
		else
		{
			map_ent = cs_map_mgr_system::get_map_ws_by_min(map_template_id, get_member_num());
		}
		
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("team_ws::transfer_all_member_to_scene map_ent is nullptr");
			return e_team_error_create_raid_failed;
		}

		guid_64 new_map_guid = map_ent->getEntityId();
		if (false == new_map_guid.is_valid())
		{
			return e_team_error_invalid_raid_instance;
		}
		set_team_map_guid(new_map_guid);

		for (s_team_member_info& member_info : m_members_list)
		{
			if (member_info.is_valid() == true)
			{
				guid_64 role_guid = member_info.role_guid;
				client_session* session = client_session_mgr::getInstance().get_session(role_guid);
				if (nullptr == session)
				{
					continue;
				}
				if (need_ready && member_info.data_ary[ETeamMemberInfo_ready_state] != e_team_mem_ready_state_ready)
				{
					continue;
				}

				ws2cs_transfer_team_member_to_scene transfer_team_to_scene_msg;
				transfer_team_to_scene_msg.team_guid = get_team_guid();
				transfer_team_to_scene_msg.role_guid = role_guid;
				transfer_team_to_scene_msg.map_template_id = map_template_id;
				transfer_team_to_scene_msg.map_guid = new_map_guid;
				bool is_use = proto_by_lua(e_msg_index_ws2cs_transfer_team_member_to_scene);
				if (is_use == false)
				{
					session->send_to_cs(&transfer_team_to_scene_msg, sizeof(transfer_team_to_scene_msg));
				}
				else
				{
					faith::ws2cs_proto::transfer_team_member_to_scene pro_msg;
					transfer_team_to_scene_msg.to_proto(pro_msg);
					session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_transfer_team_member_to_scene);
				}
			}
		}

		return e_team_error_none;
	}

	void team_ws::on_team_map_recycle()
	{
		clear_all_member_ready_state();
		m_team_map_guid.clear_data();
	}

	void team_ws::on_team_game_over()
	{
		//sync_all_member_check_ret(); //这个检测放到cs上玩家扣次数时
	}

	void team_ws::change_team_type(cs2ws_change_team_type msg)
	{
		e_team_type team_type_id = msg.team_type;
		int32 team_sub_type_id = msg.sub_temp_id;
		bool is_auto_match = msg.is_auto_match;
		int32 limit_fight_power = msg.limit_fight_power;
		int32 limit_level = msg.limit_level;

		if (false == msg.has_team_sub_type_id)
		{
			team_sub_type_id = get_team_sub_type_id();
		}

		team_proto_update_team_attribute_end update_team_attr_end_msg;
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_unit_team(msg.player_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		bool aoi_info_changed = false;
		if (true == msg.has_team_type)
		{
			if (e_team_type_one_dragon == team_type_id)
			{
				if (is_one_stop_team())
				{
					//一条龙不可以手动改目标
					return;
				}
				else
				{
					int32 cap_os_flags = get_member_one_stop_flags(get_captain_guid());
					set_one_stop_flags(cap_os_flags);
					team_ws_mgr_ref.get_team_aims_by_os_flags(cap_os_flags, team_type_id, team_sub_type_id);
				}
			}
			else
			{
				set_one_stop_flags(-1); //一条龙修改目标后就不是一条龙了
			}

			update_team_attr_end_msg.set_n_one_stop_flags(get_one_stop_flags());
			aoi_info_changed = true;
		}
		else
		{
			team_type_id = get_team_type();
		}		

		bool attr_changed = change_team_aim(team_type_id, team_sub_type_id);
		if (attr_changed)
		{
			update_team_attr_end_msg.set_n_team_type_id(get_team_type());
			update_team_attr_end_msg.set_n_team_sub_type_id(get_team_sub_type_id());
			aoi_info_changed = true;
		}
		
		if (false == msg.has_auto_match)
		{
			is_auto_match = is_auto_matching();
		}

		// modify by wangsonghao : 之前的逻辑是，如果队伍目标是"全部"，就无法将队伍的自动匹配功能打开
		// 现在根据需求修改为队伍目标为"全部"也可以打开自动匹配功能
// 		if (get_team_type() == e_team_type_none)
// 		{
// 			is_auto_match = false;
// 		}
		if (is_auto_match != is_auto_matching())
		{
			set_auto_matching(is_auto_match);
			update_team_attr_end_msg.set_n_is_auto_match(is_auto_matching());
			attr_changed = true;
		}

		if (limit_fight_power >= 0 && limit_fight_power != get_required_fighting_power())
		{
			set_required_fighting_power(limit_fight_power);
			update_team_attr_end_msg.set_n_limit_fight_power(get_required_fighting_power());
			attr_changed = true;
		}

		if (limit_level >= 0 && limit_level != get_required_level())
		{
			set_required_level(limit_level);
			update_team_attr_end_msg.set_n_limit_level(get_required_level());
			attr_changed = true;
		}

		if (attr_changed)
		{
	
			on_team_attribute_changed();
			team_ws_mgr_ref.notice_all_player_team_created(*this);
		}
		if (aoi_info_changed)
		{
			send_all_member_aoi_team_info();
		}

		//team_ws_ptr->sync_team_attribute_to_mems();
		send_message_to_all_member(&update_team_attr_end_msg, e_msgindex_s2c_update_team_attribute_end);
	}

	bool team_ws::set_one_stop_flags(int32 flags_val)
	{
		if (m_one_stop_flags == flags_val)
		{
			return false;
		}
		m_one_stop_flags = flags_val;
		return true;
	}

	int32 team_ws::get_team_map_tempalte_id()
	{
		if (team_ws_mgr::get_instance().is_team_type_raid(m_team_type))
		{
			return m_team_sub_type_id;
		}
		s_team_member_info* captain_info = get_member(m_captain_guid);
		if (nullptr == captain_info)
		{
			return -1;
		}
		return captain_info->data_ary[ETeamMemberInfo_cur_map];
	}

	bool team_ws::change_team_aim(e_team_type new_team_type, int32 new_sub_type_id)
	{
		if (new_team_type >= e_team_type_max)
		{
			return false;
		}
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		if (false == team_ws_mgr_ref.is_team_type_valid(new_team_type, new_sub_type_id))
		{
			return false;
		}
		
		bool aim_changed = false;
		bool need_check_mem_ret = false;
		if (new_team_type != get_team_type())
		{
			set_team_type(new_team_type);
			aim_changed = true;
			need_check_mem_ret = true;
		}

		if (new_sub_type_id <= 0)
		{
			new_sub_type_id = -1;
		}
		if (new_sub_type_id != get_team_sub_type_id())
		{
			set_team_sub_type_id(new_sub_type_id);
			aim_changed = true;
		}

		if (need_check_mem_ret)
		{
			// 现在每个成员有了 one_stop_flags 直接就知道助战状态了 不用算了
			//sync_all_member_check_ret();

			//切换目标了重置下
			reset_mems_check_ret();
		}

		return aim_changed;
	}

	void team_ws::sync_team_attribute_to_mems()
	{
		team_proto_update_team_attribute_end update_team_attr_end_msg;
		update_team_attr_end_msg.set_n_team_type_id(get_team_type());
		update_team_attr_end_msg.set_n_team_sub_type_id(get_team_sub_type_id());
		update_team_attr_end_msg.set_n_is_auto_match(is_auto_matching());
		update_team_attr_end_msg.set_n_limit_fight_power(get_required_fighting_power());
		update_team_attr_end_msg.set_n_limit_level(get_required_level());
		update_team_attr_end_msg.set_n_one_stop_flags(get_one_stop_flags());
		send_message_to_all_member(&update_team_attr_end_msg, e_msgindex_s2c_update_team_attribute_end);
	}

	bool team_ws::is_element_war()
	{
		if (m_team_type == e_team_type_element_war)
		{
			return true;
		}
		return false;
	}

	bool team_ws::can_team_state_add_member() const
	{
		if (get_member_num() >= max_team_member_num)
		{
			return false;
		}

		e_team_state team_state = get_team_state();
		switch (team_state)
		{
		case e_team_state_none:
		case e_team_state_waiting_join:
			return true;

		default:
			return false;
		}
	}
	
	bool team_ws::can_team_state_ready()
	{
		e_team_state team_state = get_team_state();
		switch (team_state)
		{
		case e_team_state_waiting_ready:
			return true;

		default:
			return false;
		}
	}

	bool team_ws::can_team_state_start_transfer()
	{
		e_team_state team_state = get_team_state();
		switch (team_state)
		{
		case e_team_state_waiting_ready:
			return true;

		default:
			return false;
		}
	}

	bool team_ws::can_team_state_be_searched() const
	{
		return can_team_state_add_member();
	}

	void team_ws::get_team_brief_info_pkg(team_proto_team_brief_info* brief_info_msg_ptr)
	{
		if (nullptr == brief_info_msg_ptr)
		{
			return;
		}
		s_team_member_info* captain_info = get_member(m_captain_guid);
		if (nullptr == captain_info)
		{
			return;
		}
		
		team_proto_team_brief_info& team_brief_info = *brief_info_msg_ptr;
		team_brief_info.set_team_guid(m_team_guid.server_64);
		team_brief_info.set_captain_guid(m_captain_guid.server_64);
		team_brief_info.set_player_template_id(captain_info->data_ary[ETeamMemberInfo_player_template_id]);
		team_brief_info.set_captain_name(captain_info->role_name);
		team_brief_info.set_captain_level(captain_info->data_ary[ETeamMemberInfo_level]);
		team_brief_info.set_member_num(get_member_num());
		team_brief_info.set_cur_map(captain_info->data_ary[ETeamMemberInfo_cur_map]);
		team_brief_info.set_required_fighting_power(get_required_fighting_power());
		team_brief_info.set_required_level(m_required_level);
		team_brief_info.set_team_type(m_team_type);
		team_brief_info.set_team_sub_type_id(m_team_sub_type_id);
		team_brief_info.set_one_stop_flags(m_one_stop_flags);
	}

	void team_ws::update_team_member_position(guid_64 role_guid, int32 pos_x, int32 pos_y, int32 pos_z)					 //刷新队伍玩家位置信息
	{
		if (m_members_list.size() <= 0)
		{
			return;
		}
		for (s_team_member_info& member_info : m_members_list)
		{
			if (false == member_info.is_valid()) // || member_info.data_ary[ETeamMemberInfo_is_online] <= 0
			{
				continue;
			}
			if (member_info.role_guid == role_guid)
			{
				member_info.data_ary[ETeamMemberInfo_PosX] = pos_x;
				member_info.data_ary[ETeamMemberInfo_PosY] = pos_y;
				member_info.data_ary[ETeamMemberInfo_PosZ] = pos_z;
			}
		}
	}

	void team_ws::send_all_team_member_position()					 //刷新队伍玩家位置信息
	{
		team_proto_team_pos_info team_pos_info_msg;
		int32 member_num = 0;
		for (s_team_member_info& member_info : m_members_list)
		{
			if (false == member_info.is_valid()) // || member_info.data_ary[ETeamMemberInfo_is_online] <= 0
			{
				continue;
			}
			team_proto_team_member_pos_info* member_pos_info_msg = team_pos_info_msg.add_member_list();
			if (nullptr == member_pos_info_msg)
			{
				continue;
			}
			member_pos_info_msg->set_role_guid(member_info.role_guid.server_64);
			member_pos_info_msg->set_pos_x(member_info.data_ary[ETeamMemberInfo_PosX]);
			member_pos_info_msg->set_pos_y(member_info.data_ary[ETeamMemberInfo_PosY]);
			member_pos_info_msg->set_pos_z(member_info.data_ary[ETeamMemberInfo_PosZ]);
			member_num++;
		}
		team_pos_info_msg.set_member_num(member_num);
		send_message_to_all_member(&team_pos_info_msg, e_msgindex_s2c_team_pos_info);
	}

	void team_ws::send_cs_msg_to_all_member(const void* data_ptr, size_t data_len)
	{
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* team_member_session = ite->second;
			if (nullptr == team_member_session || team_member_session->get_team_guid() != get_team_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			team_member_session->send_to_cs(data_ptr, data_len);
			++ite;
		}
	}

	void team_ws::send_message_to_all_member(google::protobuf::Message* net_pro, uint32 header)
	{
		//for (s_team_member_info& member_info : m_members_list)
		//{
		//	if (false == member_info.is_valid())
		//	{
		//		continue;
		//	}

		//	client_session* team_member_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
		//	if (nullptr == team_member_session)
		//	{
		//		continue;
		//	}

		//	team_member_session->send_to_client(net_pro, header);
		//}
		
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* team_member_session = ite->second;
			if (nullptr == team_member_session || team_member_session->get_team_guid() != get_team_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			team_member_session->send_to_client(net_pro, header);
			++ite;
		}
	}

	void team_ws::send_message_to_all_member_except(google::protobuf::Message* net_pro, uint32 header, guid_64 except_role_guid)
	{
		//for (s_team_member_info& member_info : m_members_list)
		//{
		//	if (false == member_info.is_valid())
		//	{
		//		continue;
		//	}

		//	if (except_role_guid == member_info.role_guid)
		//	{
		//		continue;
		//	}
		//	client_session* team_member_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
		//	if (nullptr == team_member_session)
		//	{
		//		continue;
		//	}

		//	team_member_session->send_to_client(net_pro, header);
		//}
		
		player_session_map_it ite = m_member_sessions.begin();
		for (; ite != m_member_sessions.end(); )
		{
			client_session* team_member_session = ite->second;
			if (nullptr == team_member_session)
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			if (except_role_guid == team_member_session->get_role_guid())
			{
				++ite;
				continue;
			}
			if (team_member_session->get_team_guid() != get_team_guid())
			{
				ite = m_member_sessions.erase(ite);
				continue;
			}
			team_member_session->send_to_client(net_pro, header);
			++ite;
		}
	}

	bool team_ws::fill_team_info_msg(team_proto_team_info& team_info_msg)
	{
		team_info_msg.set_team_type(m_team_type);
		team_info_msg.set_team_sub_type_id(m_team_sub_type_id);
		team_info_msg.set_team_guid(m_team_guid.server_64);
		team_info_msg.set_captain_guid(m_captain_guid.server_64);
		team_info_msg.set_required_fighting_power(m_required_fighting_power);
		team_info_msg.set_required_level(m_required_level);
		team_info_msg.set_auto_matching(m_is_auto_matching);
		team_info_msg.set_one_stop_flags(m_one_stop_flags);

		int32 member_info_num = 0;
		for (s_team_member_info& team_member_info : m_members_list)
		{
			team_proto_member_info* member_info_msg = team_info_msg.add_member_list();
			if (nullptr == member_info_msg)
			{
				return false;
			}

			fill_team_member_info_msg(*member_info_msg, team_member_info);
			++member_info_num;
		}
		team_info_msg.set_member_num(member_info_num);

		return true;
	}

	void team_ws::fill_team_member_info_msg(team_proto_member_info& team_member_info_msg, const s_team_member_info& member_info)
	{
		team_member_info_msg.set_role_guid(member_info.role_guid.server_64);
		team_member_info_msg.set_role_name(member_info.role_name);
		team_member_info_msg.set_gs_value(init_unit::change_i64_to_string(member_info.gs_value));
		for (int32 i = 0; i < ETeamMemberInfo_max; ++i)
		{
			team_member_info_msg.add_data(member_info.data_ary[i]);
		}
	}

	void team_ws::send_team_info_to_session(client_session* session, bool is_when_join)
	{
		if (nullptr == session)
		{
			return;
		}

		team_proto_team_info team_info_msg;
		if (!fill_team_info_msg(team_info_msg))
		{
			return;
		}
		team_info_msg.set_is_when_join(is_when_join);
		session->send_to_client(&team_info_msg, e_msgindex_s2c_update_team_info);
	}

	void team_ws::send_team_info_to_all_member()
	{
		team_proto_team_info team_info_msg;
		if (!fill_team_info_msg(team_info_msg))
		{
			return;
		}

		send_message_to_all_member(&team_info_msg, e_msgindex_s2c_update_team_info);
	}

	void team_ws::send_add_team_member_message(const s_team_member_info& new_member_info)
	{
		guid_64 new_member_guid = new_member_info.role_guid;
		if (new_member_guid.is_valid() == false)
		{
			return;
		}

		client_session* new_member_session = client_session_mgr::getInstance().get_session(new_member_guid);
		if (new_member_session)
		{
			send_team_info_to_session(new_member_session, true);
		}

		team_proto_add_team_member add_team_member_msg;
		add_team_member_msg.set_team_guid(get_team_guid().server_64);

		team_proto_member_info* member_info_msg = add_team_member_msg.mutable_member_info();
		if (nullptr == member_info_msg)
		{
			return;
		}
		fill_team_member_info_msg(*member_info_msg, new_member_info);

		send_message_to_all_member_except(&add_team_member_msg, e_msgindex_s2c_add_team_member, new_member_guid);
		notice_add_member(std::string(new_member_info.role_name));
	}

	void team_ws::send_claer_team_member_message(const guid_64& leave_member_guid)
	{
		if (leave_member_guid.is_valid() == false)
		{
			return;
		}

		client_session* leave_member_session = client_session_mgr::getInstance().get_session(leave_member_guid);
		if (leave_member_session)
		{
			team_proto_leave_team_end leave_team_end_msg;
			leave_team_end_msg.set_team_guid(get_team_guid().server_64);
			leave_member_session->send_to_client(&leave_team_end_msg, e_msgindex_s2c_leave_team_end);
		}
	}

	void team_ws::send_del_team_member_message(const guid_64& leave_member_guid)
	{
		if (leave_member_guid.is_valid() == false)
		{
			return;
		}

		client_session* leave_member_session = client_session_mgr::getInstance().get_session(leave_member_guid);
		if (leave_member_session)
		{
			team_proto_leave_team_end leave_team_end_msg;
			leave_team_end_msg.set_team_guid(get_team_guid().server_64);
			leave_member_session->send_to_client(&leave_team_end_msg, e_msgindex_s2c_leave_team_end);
			
			ws2cs_clear_player_team_info clear_player_team_info_msg;
			clear_player_team_info_msg.role_guid = leave_member_guid;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_clear_player_team_info);
			if (is_use == false)
			{
				leave_member_session->send_to_cs(&clear_player_team_info_msg, sizeof(clear_player_team_info_msg));
			}
			else
			{
				faith::ws2cs_proto::clear_player_team_info pro_msg;
				clear_player_team_info_msg.to_proto(pro_msg);
				leave_member_session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_clear_player_team_info);
			}
		}

		team_proto_del_team_member del_team_member_msg;
		del_team_member_msg.set_team_guid(get_team_guid().server_64);
		del_team_member_msg.set_role_guid(leave_member_guid.server_64);
		send_message_to_all_member_except(&del_team_member_msg, e_msgindex_s2c_del_team_member, leave_member_guid);
	}

	void team_ws::gen_player_team_info(s_player_team_info& team_info)
	{
		int32 member_num = get_member_num();
		team_info.team_guid = get_team_guid();
		team_info.captain_guid = get_captain_guid();
		team_info.member_num = member_num;
		team_info.team_map_id = get_team_map_tempalte_id();
		team_info.team_type = get_team_type();
		team_info.team_sub_type_id = get_team_sub_type_id();
		team_info.team_one_stop_flags = get_one_stop_flags();

		team_info.clear_mems_guid();
		int32 mem_idx = 0;
		for (s_team_member_info& team_member_info : m_members_list)
		{
			team_info.team_mems_guid[mem_idx] = team_member_info.role_guid;
			mem_idx++;
		}
	}
	void team_ws::send_aoi_team_info(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}

		guid_64 role_guid = session->get_role_guid();
		s_team_member_info* member_info = get_member(role_guid);
		if (nullptr == member_info)
		{
			return;
		}
		
		ws2cs_set_player_team_info set_player_team_info_msg;
		set_player_team_info_msg.role_guid = role_guid;
		gen_player_team_info(set_player_team_info_msg.player_team_info);
		bool is_use = proto_by_lua(e_msg_index_ws2cs_set_player_team_info);
		if (is_use == false)
		{
			session->send_to_cs(&set_player_team_info_msg, sizeof(set_player_team_info_msg));
		}
		else
		{
			faith::ws2cs_proto::set_player_team_info pro_msg;
			set_player_team_info_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_set_player_team_info);
		}
	}

	void team_ws::send_all_member_aoi_team_info()
	{
		if (m_members_list.size() <= 0)
		{
			return;
		}
		for (s_team_member_info& member_info : m_members_list)
		{
			if (false == member_info.is_valid())
			{
				continue;
			}
			client_session* team_mem_session = client_session_mgr::getInstance().get_session(member_info.role_guid);
			if (nullptr == team_mem_session)
			{
				continue;
			}

			send_aoi_team_info(team_mem_session);
		}
	}

	void team_ws::update_team_member_info_one(guid_64 role_guid, int32 info_index, int32 info_value, bool need_sync2c)
	{
		if (info_index < 0 || info_index >= ETeamMemberInfo_max)
		{
			return;
		}
		s_team_member_info* member_info_ptr = get_member(role_guid);
		if (member_info_ptr == nullptr)
		{
			return;
		}
		if (member_info_ptr->data_ary[info_index] == info_value)
		{
			return;
		}
		member_info_ptr->data_ary[info_index] = info_value;
		send_team_member_info_one(role_guid, info_index);
	}

	void team_ws::send_team_member_info_one(guid_64 role_guid, int32 info_index)
	{
		if (info_index < 0 || info_index >= ETeamMemberInfo_max)
		{
			return;
		}

		s_team_member_info* member_info = get_member(role_guid);
		if (nullptr == member_info)
		{
			return;
		}

		team_proto_update_team_member_info_one update_team_member_info_one;
		update_team_member_info_one.set_team_guid(get_team_guid().server_64);
		update_team_member_info_one.set_role_guid(role_guid.server_64);
		update_team_member_info_one.set_info_index(info_index);
		update_team_member_info_one.set_info_value(member_info->data_ary[info_index]);

		send_message_to_all_member(&update_team_member_info_one, e_msgindex_s2c_update_team_member_info_one);
	}

	void team_ws::send_team_gs_value(guid_64 role_guid)
	{
		s_team_member_info* member_info = get_member(role_guid);
		if (nullptr == member_info)
		{
			return;
		}
		team_proto_update_team_gs_value req;
		req.set_team_guid(get_team_guid().server_64);
		req.set_role_guid(role_guid.server_64);
		req.set_gs_value(init_unit::change_i64_to_string(member_info->gs_value));
		send_message_to_all_member(&req, e_msgindex_s2c_update_team_gs_value);
	}

	void team_ws::send_set_captain_message(guid_64 new_captain_guid)
	{
		team_proto_set_captain_end set_captain_end;
		set_captain_end.set_team_guid(get_team_guid().server_64);
		set_captain_end.set_new_captain_guid(new_captain_guid.server_64);
		send_message_to_all_member(&set_captain_end, e_msgindex_s2c_set_captain_end);

		s_team_member_info*	team_member_info_ptr = get_member(new_captain_guid);
		if (nullptr == team_member_info_ptr)
		{
			return;
		}
		xstring member_name = xstring(team_member_info_ptr->role_name);
		notice_change_captain(member_name);
	}

	void team_ws::send_team_error_to_all_member(e_team_error error_type)
	{
		team_proto_team_error team_error_msg;
		team_error_msg.set_error_type((int32)error_type);
		send_message_to_all_member(&team_error_msg, e_msgindex_s2c_team_error);
	}

	void team_ws::set_recruit_type(e_recurit_type new_type)
	{
		if (new_type >= e_recurit_type_max)
		{
			return;
		}
		m_recruit_type = new_type;
	}

	void team_ws::req_add_member_to_cs(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		ws2cs_apply_to_join_team app_join_team_msg;
		app_join_team_msg.role_guid = session->get_role_guid();
		app_join_team_msg.team_guid = m_team_guid;
		app_join_team_msg.team_type_id = m_team_type;
		app_join_team_msg.team_sub_type_id = m_team_sub_type_id;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_apply_to_join_team);
		if (is_use == false)
		{
			session->send_to_cs(&app_join_team_msg, sizeof(app_join_team_msg));
		}
		else
		{
			faith::ws2cs_proto::apply_to_join_team pro_msg;
			app_join_team_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_apply_to_join_team);
		}
	}
	
	void team_ws::set_team_type(e_team_type t_type)
	{
		m_team_type = t_type;
		if (m_team_type == e_team_type_none)
		{
			set_auto_matching(false);
		}
		if (m_team_state < e_team_state_game)
		{
			init_team_state(m_team_type);
		}
	}

	bool team_ws::is_check_sub_id_for_platform()
	{
		if (m_team_sub_type_id <= 0)
		{
			//元素争霸不需要子类型
			if (m_team_type == e_team_type_element_war)
			{
				return true;
			}
			return false;
		}
		switch (m_team_type)
		{
		case faith::e_team_type_mission_daily:
		case faith::e_team_type_mission_against:
			return false;
		default:
			break;
		}
		return true;
	}

	void team_ws::set_auto_matching(bool auto_matching)
	{
		m_is_auto_matching = auto_matching;
	}

	void team_ws::on_team_attribute_changed()
	{
		if (m_team_type == e_team_type_none || !m_is_auto_matching)
		{
			return;
		}
		match_waiting_players();
	}

	void team_ws::match_waiting_players()
	{
		if (!get_team_guid().is_valid())
		{
			return;
		}
		if (!can_team_state_be_searched())
		{
			return;
		}

		int32 cur_team_mem_num = get_member_num();
		player_auto_match_map& players_map = team_ws_mgr::get_instance().get_auto_matching_players();
		for (player_auto_match_map::iterator iter = players_map.begin(); iter != players_map.end();)
		{
			const guid_64& player_guid = iter->first;
			e_team_type need_team_type = (e_team_type)iter->second.aim_type;
			int32 need_aim_sub_type = iter->second.aim_sub_type;
			bool need_one_stop = iter->second.os_flags >= 0;
			
			player_auto_match_map::iterator cur_iter = iter;
			++iter;

			client_session* session = client_session_mgr::getInstance().get_session(player_guid);
			if (nullptr == session)
			{
				players_map.erase(cur_iter);
				continue;
			}
			team_ws* player_team = team_ws_mgr::get_instance().get_unit_team(player_guid);
			if (nullptr != player_team)
			{
				players_map.erase(cur_iter);
				continue;
			}
			if (get_team_type() != need_team_type)
			{
				continue;
			}
			if (need_aim_sub_type > 0 && need_aim_sub_type != get_team_sub_type_id())
			{
				continue;
			}
			if (need_one_stop != is_one_stop_team())
			{
				continue;
			}
			if (session->get_role_gs_value() < get_required_fighting_power())
			{
				continue;
			}
			if (session->m_role_info.data_ary[e_role_info_exp_level] < get_required_level())
			{
				continue;
			}
			if (!is_auto_matching())
			{
				continue;
			}
			players_map.erase(cur_iter);
			req_add_member_to_cs(session);
			cur_team_mem_num++;
			if (cur_team_mem_num >= max_team_member_num)
			{
				break;
			}
		}
	}

	void team_ws::stop_auto_match()
	{
		if (false == m_is_auto_matching)
		{
			return;
		}
		set_auto_matching(false);
		
		client_session* cap_session = client_session_mgr::getInstance().get_session(m_captain_guid);
		if (nullptr == cap_session)
		{
			return;
		}

		//sync_team_attribute_to_mems();
		team_proto_update_team_attribute_end update_team_attr_end_msg;
		update_team_attr_end_msg.set_n_is_auto_match(is_auto_matching());
		send_message_to_all_member(&update_team_attr_end_msg, e_msgindex_s2c_update_team_attribute_end);
	}

	void team_ws::sync_all_member_check_ret()
	{
		for (s_team_member_info& team_member_info : m_members_list)
		{
			client_session* session = client_session_mgr::getInstance().get_session(team_member_info.role_guid);
			if (nullptr == session)
			{
				return;
			}
			team_ws_mgr::get_instance().req_condition_check_to_cs(session, e_team_common_check_invoker_up_mem_check_ret, m_team_type, m_team_sub_type_id, m_team_guid);
		}
	}

	void team_ws::add_invite_guid(guid_64 invite_guid)
	{
		m_captain_invite_guid.insert(invite_guid);
	}

	void team_ws::del_invite_guid(guid_64 invite_guid)
	{
		if (m_captain_invite_guid.empty())
		{
			return;
		}
		m_captain_invite_guid.erase(invite_guid);
	}

	bool team_ws::is_have_invite_guid(guid_64 invite_guid)
	{
		if (m_captain_invite_guid.empty())
		{
			return false;
		}
		auto ite = m_captain_invite_guid.find(invite_guid);
		return ite != m_captain_invite_guid.end();
	}

	void team_ws::debug_log_team_operate(xstring log_str)
	{
//#ifndef IN_DEBUG_MODE
//		return;
//#endif
//		log_str += " T" + init_unit::change_i64_to_string(m_team_guid.server_64) + " [" + init_unit::change_i64_to_string(m_captain_guid.server_64) + "]";
//		for (int32 i = 0; i < m_members_list.size(); i++)
//		{
//			log_str += " * " + string(m_members_list[i].role_name) + "(" + init_unit::change_i64_to_string(m_members_list[i].role_guid.server_64) + ")";
//		}
//		server_log::server_debug_log(log_str);
	}

	void team_ws::join_voice_channel(guid_64 role_guid)
	{
		bool is_first_join = true;
		for (s_team_member_info& team_member_info : m_members_list)
		{
			if (team_member_info.data_ary[ETeamMemberInfo_agora_id] != 0)
			{
				is_first_join = false;
			}
		}

		update_team_member_info_one(role_guid, ETeamMemberInfo_agora_id, 1, true);

		if (is_first_join == true)
		{
			faith::chat_proto_invite_join_voice_channel msg;
			msg.set_role_guid(role_guid.server_64);
			msg.set_channel_type(faith::e_chat_type_team);
			send_message_to_all_member_except(&msg, e_msgindex_s2c_invite_join_voice_channel, role_guid);
		}
		else
		{
			s_team_member_info* member_info = get_member(role_guid);
			if (member_info == nullptr)
			{
				return;
			}
			faith::chat_proto_join_or_leave_voice_channel_notice msg;
			msg.set_channel_type(faith::e_chat_type_team);
			msg.set_role_name(member_info->role_name);
			msg.set_is_join(true);
			send_message_to_all_member_except(&msg, e_msgindex_s2c_join_or_leave_voice_channel_notice, role_guid);
		}
	}

	void team_ws::leave_voie_channel(guid_64 role_guid)
	{
		s_team_member_info* member_info = get_member(role_guid);
		if (member_info == nullptr)
		{
			return;
		}
		int32 agora_id = member_info->data_ary[ETeamMemberInfo_agora_id];
		if (agora_id == 0)
		{
			return;
		}

		update_team_member_info_one(role_guid, ETeamMemberInfo_agora_id, 0, true);

		faith::chat_proto_join_or_leave_voice_channel_notice msg;
		msg.set_channel_type(faith::e_chat_type_team);
		msg.set_role_name(member_info->role_name);
		msg.set_is_join(false);
		send_message_to_all_member_except(&msg, e_msgindex_s2c_join_or_leave_voice_channel_notice, role_guid);
	}
	bool team_ws::check_all_commmon_legion()
	{
		const std::list<s_team_member_info>& vec_team_mem = get_member_list();
		client_session* captian_session = client_session_mgr::getInstance().get_session(m_captain_guid);
		if (nullptr == captian_session)
		{
			return false;
		}
		if (!captian_session->get_legion_guid().is_valid())
		{
			captian_session->send_notice("90204162");
			return false;
		}
		for (const s_team_member_info& team_mem : vec_team_mem)
		{
			client_session* mem_session = client_session_mgr::getInstance().get_session(team_mem.role_guid);
			if (nullptr != mem_session)
			{
				if (mem_session->get_legion_guid() != captian_session->get_legion_guid())
				{
					captian_session->send_notice("90204160");
					return false;
				}
			}
		}
		return true;
	}
	void team_ws::add_assist_fight_notice()
	{
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, get_team_sub_type_id());
		if (nullptr != map_template_ptr && map_template_ptr->Order == e_map_order_type_multiplayer_raid)
		{
			client_session* captian_session = client_session_mgr::getInstance().get_session(m_captain_guid);
			if (nullptr == captian_session)
			{
				return;
			}
			captian_session->send_notice("90204159");
		}
	}

	xstring team_ws::get_member_list_name()
	{
		xstring player_name_str = "";
		int32 plyaer_idx = 0;
		int32 player_num = m_members_list.size();
		for (const s_team_member_info& team_mem : m_members_list)
		{
			if (plyaer_idx != (player_num - 1))
			{
				player_name_str = player_name_str + team_mem.role_name + " ";
			}
			else
			{
				player_name_str = player_name_str + team_mem.role_name;
			}
			plyaer_idx++;
		}
		return player_name_str;
	}
}