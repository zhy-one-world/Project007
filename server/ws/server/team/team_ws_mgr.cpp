#include "team_ws_mgr.h"
#include "ws/server/legion/legion_ws_mgr.h"
#include "team_ws_mgr.h"
#include "template/StringConst_S.h"
#include "Logic/time_def.hpp"
#include "utility/init_unit.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "chat.pb.h"
#include "net.pb.h"

namespace hld
{
	team_ws_mgr::team_ws_mgr()
	{
		m_team_ws_map.clear();
		m_team_create_stamp_map.clear();
		m_role_team_pair_map.clear();
		m_timer = 0;
		m_auto_matching_players.clear();
	}
	team_ws_mgr::~team_ws_mgr()
	{

	}
	void team_ws_mgr::tick(const uint64& time_now)
	{
		static uint64 team_mgr_stamp_next_sec_tick = 0;
		static uint64 team_mgr_stamp_next_3_sec_tick = 0;
		static uint64 team_mgr_stamp_next_min_tick = 0;
		bool do_1_sec_tick = false;
		bool do_3_sec_tick = false;
		bool do_1_min_tick = false;
		if (time_now >= team_mgr_stamp_next_sec_tick)
		{
			team_mgr_stamp_next_sec_tick = time_now + second_tick_time;
			do_1_sec_tick = true;
		}
		if (time_now >= team_mgr_stamp_next_3_sec_tick)
		{
			team_mgr_stamp_next_3_sec_tick = time_now + (second_tick_time * 3);
			do_3_sec_tick = true;
		}
		if (time_now >= team_mgr_stamp_next_min_tick)
		{
			team_mgr_stamp_next_min_tick = time_now + minute_tick_time;
			do_1_min_tick = true;
		}

		team_ws_map::iterator ite;
		for (ite = m_team_ws_map.begin(); ite != m_team_ws_map.end(); )
		{
			team_ws_map::iterator temp_ite = ite++;
			team_ws& team_ws_ref = temp_ite->second;

			if (team_ws_ref.is_dissolved())
			{
				del_team(team_ws_ref.get_team_guid());
				continue;
			}

			team_ws_ref.tick(time_now);

			if (do_1_sec_tick)
			{
				team_ws_ref.tick_1_sec(time_now);
			}
			if (do_3_sec_tick)
			{
				team_ws_ref.tick_3_sec(time_now);
			}
			if (do_1_min_tick)
			{
				team_ws_ref.tick_1_min(time_now);
			}
		}
	}

	team_ws* team_ws_mgr::get_team(guid_64 team_guid)
	{
		if (false == team_guid.is_valid())
		{
			return nullptr;
		}
		auto ite = m_team_ws_map.find(team_guid.server_64);
		if (ite == m_team_ws_map.end())
		{
			return nullptr;
		}
		
		team_ws& team_ref = ite->second;
		if (team_ref.is_dissolved())
		{
			return nullptr;
		}

		return &team_ref;
	}

	team_ws* team_ws_mgr::add_team(team_ws& new_team)
	{
		guid_64 team_guid = new_team.get_team_guid();
		if (team_guid.is_valid() == false)
		{
			return nullptr;
		}

		// 如果队伍已经在组队管理器中则不再重复插入
		if (get_team(team_guid) != nullptr)
		{
			return nullptr;
		}

		m_team_ws_map.insert({ team_guid.server_64, new_team });
		m_team_create_stamp_map.insert({ utility::get_tick_count(), team_guid });

		return &new_team;
	}

	void team_ws_mgr::del_team(guid_64 team_guid)
	{
		auto ite = m_team_ws_map.find(team_guid.server_64);
		if (ite == m_team_ws_map.end())
		{
			return;
		}

		for (auto stamp_ite = m_team_create_stamp_map.begin(); stamp_ite != m_team_create_stamp_map.end(); ++stamp_ite)
		{
			if (stamp_ite->second == team_guid)
			{
				m_team_create_stamp_map.erase(stamp_ite);
				break;
			}
		}

		team_ws& team_ws_ref = ite->second;
		team_ws_ref.on_team_delete();
		m_team_ws_map.erase(ite);
	}

	void team_ws_mgr::set_unit_team(const guid_64& unit_guid, const guid_64& team_guid)
	{
		m_role_team_pair_map.insert({ unit_guid.server_64, team_guid });
	}

	team_ws* team_ws_mgr::get_unit_team(const guid_64& unit_guid)
	{
		auto ite = m_role_team_pair_map.find(unit_guid.server_64);
		if (ite == m_role_team_pair_map.end())
		{
			return nullptr;
		}

		return get_team(ite->second);
	}

	void team_ws_mgr::clear_unit_team(const guid_64& unit_guid)
	{
		m_role_team_pair_map.erase(unit_guid.server_64);
	}

	team_ws* team_ws_mgr::create_new_team(client_session* session, const s_team_member_info& captain_info, e_team_type team_type, int32 sub_type_id, bool is_auto_match, int32 one_stop_val, int32 required_fp, int32 required_lv)
	{
		// 创建队伍 加入创建人 并将创建人设置为队长 任何一步运行异常都会导致队伍创建失败
		team_ws new_team_ws(team_type);
		new_team_ws.set_captain_guid(captain_info.role_guid);
		new_team_ws.set_team_sub_type_id(sub_type_id);
		new_team_ws.set_auto_matching(is_auto_match);
		new_team_ws.set_required_fighting_power(required_fp);
		new_team_ws.set_required_level(required_lv);
		new_team_ws.set_one_stop_flags(one_stop_val);
		if (new_team_ws.add_member(session, captain_info) == false)
		{
			return nullptr;
		}
		team_ws* new_team_ws_ptr = add_team(new_team_ws);
		if (new_team_ws_ptr == nullptr)
		{
			return nullptr;
		}

		new_team_ws.send_team_info_to_session(session, true);
		// new_team_ws.send_aoi_team_info(role_guid); //上面的add_member已经发了

		team_proto_create_team_end create_team_end_msg;
		session->send_to_client(&create_team_end_msg, e_msgindex_s2c_create_team_end);

		// 所有的创建流程走完之后 搜素下有没有符合条件的玩家
		new_team_ws.match_waiting_players();
		notice_all_player_team_created(new_team_ws);

		server_log::create_team_log(captain_info.role_guid, team_type);
		return new_team_ws_ptr;
	}
	
	void team_ws_mgr::gen_player_team_mem_info_ws(client_session* session, s_team_member_info& team_mem_info)
	{
		if (nullptr == session)
		{
			return;
		}

		team_mem_info.role_guid										= session->get_role_guid();
		team_mem_info.role_index									= -1;
		team_mem_info.set_role_name(session->m_role_info.role_name);
		team_mem_info.gs_value										= session->get_role_gs_value();
		team_mem_info.data_ary[ETeamMemberInfo_is_online]			= 1;
		team_mem_info.data_ary[ETeamMemberInfo_line_id]				= session->get_line_id();
		team_mem_info.data_ary[ETeamMemberInfo_player_template_id]	= session->get_role_info_data(e_role_info_template_id);
		team_mem_info.data_ary[ETeamMemberInfo_cur_hp]				= session->m_role_data_ws_ex[e_role_data_ws_ex_cur_hp];
		team_mem_info.data_ary[ETeamMemberInfo_max_hp]				= session->m_role_data_ws_ex[e_role_data_ws_ex_max_hp];
		team_mem_info.data_ary[ETeamMemberInfo_level]				= session->get_role_info_data(e_role_info_exp_level);
		team_mem_info.data_ary[ETeamMemberInfo_cur_map]				= session->get_role_info_data(e_role_info_move_map_id);
		team_mem_info.data_ary[ETeamMemberInfo_damage]				= 0;
		team_mem_info.data_ary[ETeamMemberInfo_check_ret]			= e_error_code_success;
		team_mem_info.data_ary[ETeamMemberInfo_one_stop_flags]		= -1;
	}
	
	void team_ws_mgr::get_team_ws_list_by_map_id(int32 map_template_id, std::vector<team_ws*>& ret_team_list)
	{
		team_ws_map::iterator ite;
		for (ite = m_team_ws_map.begin(); ite != m_team_ws_map.end(); ++ite)
		{
			team_ws& team_ws_ref = ite->second;
			if (team_ws_ref.get_team_guid().is_valid() == false)
			{
				continue;
			}
			if (team_ws_ref.can_team_state_be_searched() == false)
			{
				continue;
			}
			if (team_ws_ref.get_team_map_tempalte_id() == map_template_id)
			{
				ret_team_list.push_back(&team_ws_ref);
			}
		}
	}

	bool team_ws_mgr::is_two_player_in_same_team(guid_64 player1, guid_64 player2)
	{
		team_ws* player1_team = team_ws_mgr::get_instance().get_unit_team(player1);
		if (nullptr == player1_team)
		{
			return false;
		}

		team_ws* player2_team = team_ws_mgr::get_instance().get_unit_team(player2);
		if (nullptr == player2_team)
		{
			return false;
		}

		if (player1_team->get_team_guid() == player2_team->get_team_guid())
		{
			return true;
		}

		return false;
	}
	void team_ws_mgr::send_msg_to_all_member(guid_64 team_guid, google::protobuf::Message* net_pro, uint32 header)
	{
		team_ws* team_ptr = get_team(team_guid);
		if (nullptr == team_ptr)
		{
			return;
		}
		team_ptr->send_message_to_all_member(net_pro, header);
	}
	void team_ws_mgr::send_team_error_to_session(client_session* session, e_team_error err_type)
	{
		team_proto_team_error team_error_msg;
		team_error_msg.set_error_type((int32)err_type);
		session->send_to_client(&team_error_msg, e_msgindex_s2c_team_error);
	}

	int32 team_ws_mgr::send_recruit_proc(guid_64 team_guid, int32 aim_tpye, int32 aim_sub_id, int32 recruit_type, guid_64 sender_guid, xstring sender_name, int32 role_template_id, int32 one_dragon_flag)
	{
		if (recruit_type < 0
			|| recruit_type >= e_recurit_type_max)
		{
			return e_item_string_unkown;
		}
		team_ws* team_ptr = get_team(team_guid);
		if (team_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		team_ptr->set_recruit_type(e_recurit_type(recruit_type));
		team_ptr->get_required_fighting_power();
		chat_proto_receive_recruit_info msg;
		msg.set_team_guid(team_guid.server_64);
		msg.set_aim_type(aim_tpye);
		msg.set_aim_sub_id(aim_sub_id);
		msg.set_recruit_type(recruit_type);
		msg.set_request_fighting_power(team_ptr->get_required_fighting_power());
		msg.set_request_level(team_ptr->get_required_level());
		msg.set_cur_member_num(team_ptr->get_member_num());
		msg.set_sender_guid(sender_guid.server_64);
		msg.set_sender_name(sender_name);
		msg.set_sender_template_id(role_template_id);
		msg.set_one_dragon_flag(one_dragon_flag);
		switch (recruit_type)
		{
		case e_recurit_type_legion_recurit:
			{
				guid_64 captain_guid = team_ptr->get_captain_guid();
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
					temp_legion->send_message_to_all_member(&msg, e_msgindex_s2c_receive_recruit_message);
				}
			}
			break;

		case e_recurit_type_world_recurit:
			{
				client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_receive_recruit_message);
			}
			break;

		default:
			break;
		}
		return e_item_string_succeed;
	}

	bool team_ws_mgr::team_condition_common_check(team_ws& team_ws_ref, client_session* session)
	{
		if (!team_ws_ref.get_team_guid().is_valid())
		{
			return false;
		}
		if (!team_ws_ref.can_team_state_be_searched())
		{
			return false;
		}
		
		//if (session)
		//{
		//	if (session->m_role_info.data_ary[e_role_info_GS_value] < team_ws_ref.get_required_fighting_power())
		//	{
		//		return false;
		//	}
		//	if (session->m_role_info.data_ary[e_role_info_exp_level] < team_ws_ref.get_required_level())
		//	{
		//		return false;
		//	}
		//	
		//	if (is_team_type_raid(team_ws_ref.get_team_type()))
		//	{
		//		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, team_ws_ref.get_team_sub_type_id());
		//		if (nullptr == map_template_ptr)
		//		{
		//			return false;
		//		}
		//		if (session->m_role_info.data_ary[e_role_info_exp_level] < map_template_ptr->EnterLevel)
		//		{
		//			return false;
		//		}
		//	}
		//}
		
		return true;
	}

	void team_ws_mgr::fill_team_list_msg_by_aim(team_proto_get_team_list_by_aim_end& get_team_list_msg, e_team_type team_type, int32 sub_type_id, client_session* session)
	{
		int32 got_team_num = 0;
		get_team_list_msg.set_team_type_id(team_type);
		for (team_create_stamp_map::reverse_iterator riter = m_team_create_stamp_map.rbegin(); riter != m_team_create_stamp_map.rend(); ++riter)
		{
			team_ws* team_ws_ptr = get_team(riter->second);
			if (nullptr == team_ws_ptr)
			{
				continue;
			}
			team_ws& team_ws_ref = *team_ws_ptr;
			if (!team_condition_common_check(team_ws_ref, session))
			{
				continue;
			}
			
			if (team_type == e_team_type_one_dragon)
			{
				if (false == team_ws_ref.is_one_stop_team())
				{
					continue;
				}
			}
			else
			{
				if (team_ws_ref.get_team_type() != team_type)
				{
					continue;
				}
				// modify by wangsonghao : sub_type_id 不一致的队伍也同步给客户端显示
// 				if (team_ws_ref.is_check_sub_id_for_platform() && team_ws_ref.get_team_sub_type_id() != sub_type_id)
// 				{
// 					continue;
// 				}
				if (false == team_ws_ref.is_check_sub_id_for_platform())
				{
					continue;
				}
			}
			
			team_proto_team_brief_info* team_brief_info = get_team_list_msg.add_team_list();
			team_ws_ref.get_team_brief_info_pkg(team_brief_info);
			
			got_team_num++;
			if (got_team_num >= platform_get_team_num)
			{
				return;
			}
		}
	}

	void team_ws_mgr::fill_team_list_msg_all(team_proto_get_team_list_by_aim_end& get_team_list_msg, const unit_index_map& can_transfer_sub_ids, client_session* session)
	{
		int32 got_team_num = 0;
		get_team_list_msg.set_team_type_id(-1);
		for (team_create_stamp_map::reverse_iterator riter = m_team_create_stamp_map.rbegin(); riter != m_team_create_stamp_map.rend(); ++riter)
		{
			team_ws* team_ws_ptr = get_team(riter->second);
			if (nullptr == team_ws_ptr)
			{
				continue;
			}
			team_ws& team_ws_ref = *team_ws_ptr;
			if (!team_condition_common_check(team_ws_ref, session))
			{
				continue;
			}
			
			if (team_ws_ref.get_team_type() == e_team_type_none)
			{
				continue;
			}
			
			if (is_team_type_raid(team_ws_ref.get_team_type()))
			{
				// 暂时去掉 让玩家都能看见
				//auto ite = can_transfer_sub_ids.find(team_ws_ref.get_team_sub_type_id());
				//if (ite == can_transfer_sub_ids.end())
				//{
				//	continue;
				//}
			}

			team_proto_team_brief_info* team_brief_info = get_team_list_msg.add_team_list();
			team_ws_ref.get_team_brief_info_pkg(team_brief_info);
			got_team_num++;
			if (got_team_num >= platform_get_team_num)
			{
				return;
			}
		}

		for (team_create_stamp_map::reverse_iterator riter = m_team_create_stamp_map.rbegin(); riter != m_team_create_stamp_map.rend(); ++riter)
		{
			team_ws* team_ws_ptr = get_team(riter->second);
			if (nullptr == team_ws_ptr)
			{
				continue;
			}
			team_ws& team_ws_ref = *team_ws_ptr;
			if (!team_condition_common_check(team_ws_ref, session))
			{
				continue;
			}
			
			if (team_ws_ref.get_team_type() != e_team_type_none)
			{
				continue;
			}

			team_proto_team_brief_info* team_brief_info = get_team_list_msg.add_team_list();
			team_ws_ref.get_team_brief_info_pkg(team_brief_info);

			got_team_num++;
			if (got_team_num >= platform_get_team_num)
			{
				return;
			}
		}
	}

	e_player_match_team_ret team_ws_mgr::player_auto_match_team(e_team_type team_type, int32 sub_type, client_session* session, e_error_code map_check_ret, int32 self_one_stop_flags, bool is_one_stop)
	{
		if (nullptr == session)
		{
			return e_player_match_team_none;
		}
		guid_64 player_guid = session->get_role_guid();
		team_ws* player_team = get_unit_team(player_guid);
		if (nullptr != player_team)
		{
			return e_player_match_team_none;
		}
		
		if (team_type == e_team_type_one_dragon)
		{
			// 一条龙会选择具体目标 所有目标次数都用完的话 前面就返回错误了
			return e_player_match_team_none;
		}

		for (team_ws_map::iterator iter = m_team_ws_map.begin(); iter != m_team_ws_map.end(); ++iter)
		{
			team_ws& team_ws_ref = iter->second;
			if (!team_ws_ref.get_team_guid().is_valid())
			{
				continue;
			}
			if (!team_ws_ref.can_team_state_be_searched())
			{
				continue;
			}
			if (is_one_stop != team_ws_ref.is_one_stop_team())
			{
				continue;
			}
			if (team_ws_ref.get_team_type() != team_type)
			{
				continue;
			}
			if (sub_type > 0 && sub_type != team_ws_ref.get_team_sub_type_id())
			{
				continue;
			}
			if (session->get_role_gs_value() < team_ws_ref.get_required_fighting_power())
			{
				continue;
			}
			if (session->m_role_info.data_ary[e_role_info_exp_level] < team_ws_ref.get_required_level())
			{
				continue;
			}
			if (!team_ws_ref.is_auto_matching())
			{
				continue;
			}
			team_ws_ref.req_add_member_to_cs(session);
			return e_player_match_team_succeed;
		}
		
		// 没有匹配到队伍 检查有没有匹配一样的队伍目标的其他玩家
		player_auto_match_map& matching_players_map = get_auto_matching_players();
		for (player_auto_match_map::iterator iter = matching_players_map.begin(); iter != matching_players_map.end(); ++iter)
		{
			const guid_64& matching_guid = iter->first;
			e_team_type need_team_type = (e_team_type)iter->second.aim_type;
			int32 need_aim_sub_type = iter->second.aim_sub_type;
			bool need_one_stop = iter->second.os_flags >= 0;

			if (need_team_type != team_type)
			{
				continue;
			}
			if (need_aim_sub_type != sub_type)
			{
				continue;
			}
			if (need_one_stop != is_one_stop)
			{
				continue;
			}

			if (matching_guid == player_guid)
			{
				continue;
			}
			
			s_team_member_info captain_mem_info;
			gen_player_team_mem_info_ws(session, captain_mem_info);
			captain_mem_info.data_ary[ETeamMemberInfo_check_ret] = map_check_ret;
			captain_mem_info.data_ary[ETeamMemberInfo_one_stop_flags] = self_one_stop_flags;
			int32 new_team_os = -1;
			if (is_one_stop)
			{
				new_team_os = self_one_stop_flags;
			}
			create_new_team(session, captain_mem_info, need_team_type, need_aim_sub_type, true, new_team_os);
			return e_player_match_team_succeed;
		}
		
		m_auto_matching_players[player_guid] = s_team_auto_match_condition(team_type, sub_type);
		if (is_one_stop)
		{
			m_auto_matching_players[player_guid].os_flags = self_one_stop_flags;
		}
		
		return e_player_match_team_waiting;
	}

	void team_ws_mgr::player_stop_auto_match_team(guid_64 player_guid)
	{
		player_auto_match_map::iterator it = m_auto_matching_players.find(player_guid);
		if (it == m_auto_matching_players.end())
		{
			return;
		}
		m_auto_matching_players.erase(player_guid);
	}

	e_team_type team_ws_mgr::get_team_type_by_sub_id(int32 sub_type_id)
	{
		template_manager::template_type* team_type_cfgs = template_manager::get_instance().get_templates(e_TeamAimTypeTemplate);
		if (nullptr == team_type_cfgs)
		{
			return e_team_type_none;
		}
		template_manager::template_type::iterator ite;
		for (ite = team_type_cfgs->begin(); ite != team_type_cfgs->end(); ++ite)
		{
			TeamAimTypeTemplate* team_type_cfg = (TeamAimTypeTemplate*)(ite->second);
			if (nullptr == team_type_cfg)
			{
				continue;
			}
			int32 map_num = team_type_cfg->SubTypeId.size();
			for (std::vector<int32>::iterator sub_type_ids_it = team_type_cfg->SubTypeId.begin(); sub_type_ids_it != team_type_cfg->SubTypeId.end(); ++sub_type_ids_it)
			{
				if (sub_type_id == *sub_type_ids_it)
				{
					return (e_team_type)team_type_cfg->AimType;
				}
			}
		}
		return e_team_type_none;
	}
	
	bool team_ws_mgr::is_team_type_raid(e_team_type team_type)
	{
		return init_unit::is_team_type_raid_map(team_type);
	}
	
	bool team_ws_mgr::is_team_type_have_sub_id(e_team_type team_type)
	{
		if (team_type == e_team_type_one_dragon)
		{
			return false;
		}
		TeamAimTypeTemplate* team_type_cfg = GET_TEMPLATE(TeamAimTypeTemplate, first_team_aim_template_id + team_type);
		if (nullptr == team_type_cfg)
		{
			return false;
		}
		return team_type_cfg->SubTypeId.size() > 0;
	}

	bool team_ws_mgr::is_team_type_valid(e_team_type team_type, int32 sub_type_id)
	{
		TeamAimTypeTemplate* team_type_cfg = GET_TEMPLATE(TeamAimTypeTemplate, first_team_aim_template_id + team_type);
		if (nullptr == team_type_cfg)
		{
			return false;
		}

		if (team_type_cfg->AimTypeValid <= 0)
		{
			return false;
		}

		if (is_team_type_have_sub_id(team_type) && team_type != get_team_type_by_sub_id(sub_type_id))
		{
			return false;
		}

		return true;
	}

	void team_ws_mgr::get_team_aims_by_os_flags(int32 os_flags, e_team_type& team_type, int32& sub_type_id)
	{
		team_type = e_team_type_one_dragon;
		sub_type_id = -1;
		
		TeamAimTypeTemplate* one_stop_cfg = GET_TEMPLATE(TeamAimTypeTemplate, first_team_aim_template_id + e_team_type_one_dragon);
		if (nullptr == one_stop_cfg)
		{
			return;
		}
		
		int32 sub_id_size = one_stop_cfg->SubTypeId.size();
		for (int32 idx = 0; idx < sub_id_size; idx++)
		{
			TeamAimTypeTemplate* _aim_cfg = GET_TEMPLATE(TeamAimTypeTemplate, one_stop_cfg->SubTypeId[idx]);
			if (nullptr == _aim_cfg)
			{
				continue;
			}

			if (e_team_one_stop_aim_ok != init_unit::get_flag_data_2bit(os_flags, idx + 1))
			{
				continue;
			}

			team_type = (e_team_type)_aim_cfg->AimType;
			if (_aim_cfg->SubTypeId.size() > 0)
			{
				sub_type_id = _aim_cfg->SubTypeId[0];
			}
			break;
		}
	}

	void team_ws_mgr::on_unit_leave_map(client_session* session, int32 unit_num_after_rm)
	{
		//之前队伍状态重置的逻辑 这样实现副本内唯一队员 退队会有bug 改为副本回收时候重置
		//if (nullptr == session)
		//{
		//	return;
		//}
		//guid_64 unit_guid = session->m_role_info.role_guid;
		//guid_64 from_map_guid = session->get_map_guid();
		//uint32 tar_map_template_id = session->m_transfer_info.map_id;
		//guid_64 tar_map_guid = session->m_transfer_info.map_guid;

		//team_ws*  unit_cur_team = get_unit_team(unit_guid);
		//if (nullptr == unit_cur_team)
		//{
		//	return;
		//}
		//if (from_map_guid != unit_cur_team->get_team_map_guid())
		//{
		//	return;
		//}

		//if (unit_num_after_rm <= 0)
		//{
		//	unit_cur_team->on_team_game_over();
		//}
	}

	void team_ws_mgr::on_team_map_recycle(guid_64 team_guid, guid_64 recycling_map_guid)
	{
		team_ws*  map_cur_team = get_team(team_guid);
		if (nullptr == map_cur_team)
		{
			return;
		}
		if (recycling_map_guid != map_cur_team->get_team_map_guid())
		{
			return;
		}
		map_cur_team->on_team_map_recycle();
	}

	void team_ws_mgr::on_player_online(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		guid_64 player_guid = session->get_role_guid();
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(player_guid);
		if (team_ws_ptr == nullptr)
		{
			//防止断线过程中被踢掉
			team_proto_leave_team_end leave_team_end_msg;
			leave_team_end_msg.set_team_guid(0);
			session->send_to_client(&leave_team_end_msg, e_msgindex_s2c_leave_team_end);
			return;
		}
		team_ws_ptr->on_team_mem_online(session);
	}

	void team_ws_mgr::on_player_offline(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		guid_64 player_guid = session->get_role_guid();
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(player_guid);
		if (team_ws_ptr == nullptr)
		{
			return;
		}
		team_ws_ptr->on_team_mem_offline(session);
	}

	void team_ws_mgr::on_player_login(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		guid_64 player_guid = session->get_role_guid();
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(player_guid);
		if (team_ws_ptr == nullptr)
		{
			return;
		}
		team_ws_ptr->on_team_mem_login(session);
	}

	void team_ws_mgr::on_player_logout(client_session* session)
	{
		if (nullptr == session)
		{
			return;
		}
		guid_64 player_guid = session->get_role_guid();
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(player_guid);
		if (team_ws_ptr == nullptr)
		{
			return;
		}
		team_ws_ptr->on_team_mem_logout(session);
	}

	void team_ws_mgr::req_condition_check_to_cs(client_session* session, e_team_common_check_invoker invoker, e_team_type team_type, int32 team_sub_type_id, guid_64 team_guid)
	{
		if (nullptr == session)
		{
			return;
		}
		ws2cs_team_common_check_in_cs team_common_check_in_cs_msg;
		team_common_check_in_cs_msg.role_guid = session->get_role_guid();
		team_common_check_in_cs_msg.team_guid = team_guid;
		team_common_check_in_cs_msg.team_type_id = team_type;
		team_common_check_in_cs_msg.team_sub_type_id = team_sub_type_id;
		team_common_check_in_cs_msg.check_invoker = invoker;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_team_common_check_in_cs);
		if (is_use == false)
		{
			session->send_to_cs(&team_common_check_in_cs_msg, sizeof(team_common_check_in_cs_msg));
		}
		else
		{
			hld::ws2cs_proto::team_common_check_in_cs pro_msg;
			team_common_check_in_cs_msg.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_team_common_check_in_cs);
		}
	}

	void team_ws_mgr::recv_condition_check_from_cs(int32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_team_common_check_in_cs* packet = static_cast<const cs2ws_team_common_check_in_cs*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_team_common_check_in_cs))
		{
			return;
		}
		
		guid_64 role_guid = packet->role_guid;
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}

		e_team_common_check_invoker invoker = packet->check_invoker;
		e_error_code map_check_ret = packet->map_check_ret;
		e_team_type type_id = packet->team_type_id;
		int32 sub_type_id = packet->team_sub_type_id;
		switch (invoker)
		{
		case e_team_common_check_invoker_auto_match_team:
			{
				int32 match_ret = e_player_match_team_none;
				if (e_error_code_success == map_check_ret || e_error_code_map_lack_of_times == map_check_ret)
				{
					match_ret = player_auto_match_team(type_id, sub_type_id, session, map_check_ret, packet->own_one_stop_flags, packet->is_match_one_stop);
				}
				else
				{
					send_team_error_to_session(session, e_team_error_raid_condition_less);
				}
				team_proto_auto_match_team_end match_team_end_msg;
				match_team_end_msg.set_match_ret(match_ret);
				session->send_to_client(&match_team_end_msg, e_msgindex_s2c_auto_match_team_end);
			}
			break;
		case e_team_common_check_invoker_mem_ready:
			{
				team_ws* team_ws_ptr = get_unit_team(role_guid);
				if (nullptr == team_ws_ptr)
				{
					break;
				}
				team_ws_ptr->set_member_ready_status(role_guid, true, map_check_ret);
			}
			break;
		case e_team_common_check_invoker_up_mem_check_ret:
			{
				team_ws* team_ws_ptr = get_unit_team(role_guid);
				if (nullptr == team_ws_ptr)
				{
					break;
				}
				team_ws_ptr->set_member_check_ret(role_guid, map_check_ret);
			}
			break;
		default:
			break;
		}
	}

	void team_ws_mgr::req_no_team_mate_list(client_session* session, e_team_req_mate_type mate_type)
	{
		if (nullptr == session)
		{
			return;
		}
		guid_64 role_guid = session->get_role_guid();
		
		team_proto_get_no_team_mate_list_end get_no_team_mate_list_end_msg;
		int32 record_count = 0;
		switch (mate_type)
		{
		case hld::e_team_req_mate_type_friend:
			{
			}
			break;
		case hld::e_team_req_mate_type_legion:
			{
				legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
				if (nullptr == legion_ws_ptr)
				{
					break;;
				}
				const std::list<s_legion_member_info>& member_list = legion_ws_ptr->get_member_list();
				for (const s_legion_member_info& member_info : member_list)
				{
					if (team_ws_mgr::get_instance().get_unit_team(member_info.role_guid) != nullptr)
					{
						continue;
					}

					get_no_team_mate_list_end_msg.add_mates_guid_arr(member_info.role_guid.server_64);
					++record_count;
				}
			}
			break;
		default:
			break;
		}
		if (record_count <= 0)
		{
			return;
		}
		session->send_to_client(&get_no_team_mate_list_end_msg, e_msgindex_s2c_get_no_team_legionmate_list_end);
	}

	//目前这个消息包是个机器人功能用的
	void team_ws_mgr::notice_all_player_team_created(team_ws& team_ref)
	{
		if (!team_ref.can_team_state_be_searched())
		{
			return;
		}

		team_proto_notice_create_new_team noti_new_team_msg;
		noti_new_team_msg.set_team_guid(team_ref.get_team_guid().server_64);
		noti_new_team_msg.set_required_fighting_power(team_ref.get_required_fighting_power());
		noti_new_team_msg.set_required_level(team_ref.get_required_level());
		noti_new_team_msg.set_team_type(team_ref.get_team_type());
		noti_new_team_msg.set_team_sub_type_id(team_ref.get_team_sub_type_id());
		noti_new_team_msg.set_is_auto_matching(team_ref.is_auto_matching());
		noti_new_team_msg.set_captain_guid(team_ref.get_captain_guid().server_64);

		client_session_mgr::getInstance().send_message_to_all_client(&noti_new_team_msg, e_msgindex_s2c_notice_create_new_team);
	}

	void team_ws_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		auto iter_team_ws =  m_team_ws_map.begin();
		for (; iter_team_ws != m_team_ws_map.end(); ++iter_team_ws)
		{
			team_ws& team = iter_team_ws->second;
			s_team_member_info* team_member_ptr = team.get_member(role_guid);
			if (nullptr == team_member_ptr)
			{
				continue;
			}

			xchar char_role_name[max_name_size + 1] = { 0 };
			memcpy(char_role_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
			team_member_ptr->set_role_name(char_role_name);

			team.send_team_info_to_all_member();
		}
	}
}
