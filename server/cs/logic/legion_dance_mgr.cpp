/********************************************************************
created: 2019年10月15日
file base: legion_dance_mgr
file ext: cpp
author: 赵玉明

purpose: 军团传功
*********************************************************************/


#include "cell_server.hpp"
#include "utility/init_unit.h"
#include "legion_dance_mgr.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "template/template_manager.h"
#include "internet/legion.pb.h"
#include "internet/net.pb.h"

namespace hld
{
	legion_dance_mgr::legion_dance_mgr()
	{	
		clear_data();
	}

	legion_dance_mgr::~legion_dance_mgr()
	{

	}

	void legion_dance_mgr::clear_data()
	{
		m_array_index = 0;
	}

	int32 legion_dance_mgr::get_world_server_last_level()
	{
		return cell_server::getInstance().get_world_level_last();
	}

	void legion_dance_mgr::send_legion_dance_invite_error_tip(e_legion_dance_invite_error error_index, xstring role_name)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		switch (error_index)
		{
		case e_legion_dance_invite_error_player_info_error:
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305446);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_invite_error_not_in_map:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305443));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_invite_error_not_in_game:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305444));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_invite_error_in_war:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305445));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_invite_error_my_in_war:
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305447);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_invite_error_target_refuse:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305441));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		default:
			break;
		}
	}

	void legion_dance_mgr::send_legion_dance_accept_error_tip(e_legion_dance_accept_error error_index, xstring role_name)
	{
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		switch (error_index)
		{
		case e_legion_dance_accept_error_player_info_error:
		{
			player_ref.send_notice("90305457");
		}
		break;
		case e_legion_dance_accept_error_not_in_map:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305458));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_accept_error_not_in_game:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305459));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_accept_error_in_war:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305460));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_accept_error_my_in_war:
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305461);
			player_ref.send_notice(notice_str);
		}
		break;
		case e_legion_dance_accept_error_in_interactive:
		{
			std::vector<std::string> vec_str_time;
			vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305463));
			vec_str_time.push_back(role_name);
			std::string notice_str = init_unit::implode(vec_str_time);
			player_ref.send_notice(notice_str);
		}
		break;
		default:
			break;
		}
	}

	void legion_dance_mgr::send_legion_dance_invite(int32 target_index)
	{
		player& target_ref = unit_man::get_player(target_index);
		if (target_ref.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		
		//发送邀请发送成功提示
		std::vector<std::string> vec_str_time;
		vec_str_time.push_back(template_manager::get_instance().get_str_by_string_template_id(90305442));
		std::string role_name = target_ref.get_name();
		vec_str_time.push_back(role_name);
		std::string notice_str = init_unit::implode(vec_str_time);
		player_ref.send_notice(notice_str);

		//发送邀请
		legion_proto_be_invite_legion_dance msg;
		msg.set_play_guid_a(player_ref.get_unit_guid().A);
		msg.set_play_guid_b(player_ref.get_unit_guid().B);
		msg.set_play_name(player_ref.get_name());
		msg.set_play_level(player_ref.get_unit_info(e_role_info_exp_level));
		msg.set_play_template_id(player_ref.get_unit_info(e_role_info_template_id));
		target_ref.send_message_to_self(&msg, e_mgsindex_s2c_be_invite_legion_dance);
	}

	void legion_dance_mgr::legion_dance_official_start(guid_64 target_guid)
	{
		player& target_ref = unit_man::get_player(target_guid);
		if (target_ref.is_valid() == false)
		{
			return;
		}
		player& player_ref = unit_man::get_player(m_array_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		legion_proto_invite_legion_dance_end msg;
		player_ref.send_message_to_self(&msg, e_mgsindex_s2c_invite_legion_dance_end);
		target_ref.send_message_to_self(&msg, e_mgsindex_s2c_invite_legion_dance_end);
	}
}