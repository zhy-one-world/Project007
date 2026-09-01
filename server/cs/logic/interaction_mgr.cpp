/********************************************************************
created: 2018年3月15日
file base: interaction_mgr
file ext: cpp
author: wucun

purpose: about role's interaction_mgr
*********************************************************************/

#include "interaction_mgr.hpp"
#include "internet/interaction.pb.h"
#include "internet/legion.pb.h"
#include "internet/net.pb.h"
#include "logic/buff_man.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "template/template_manager.h"
#include "utility/init_unit.h"

namespace faith
{
	cinteraction_mgr::cinteraction_mgr()
	{
		m_unit_array_index = 0;
		clear_data();
	}
	cinteraction_mgr::~cinteraction_mgr()
	{
	}
	void cinteraction_mgr::clear_data()
	{
		m_handle_invite_cd_time = -1;
		m_interaction_target_guid.clear_data();
		m_interaction_type = faith::e_interaction_type_none;
		m_wait_real_interaction_time = -1;
		m_old_tick_time = 0;
		m_interaction_state = e_interaction_state_none;
		m_interaction_cdtime = 0;
		m_is_inviter = false;
	}
	void cinteraction_mgr::heart_tick(const int64& new_time)
	{
		//处理交互请求时间
		//if (m_interaction_state == e_interaction_state_wait_reply)
		{
			if (m_handle_invite_cd_time > 0)
			{
				m_handle_invite_cd_time -= ((new_time - m_old_tick_time) / second_tick_time);
				if (m_handle_invite_cd_time <= 0)
				{
					if (m_interaction_state == e_interaction_state_wait_reply)
					{
						if (m_is_inviter == true)
						{
							player& player_ref = unit_man::get_player(m_unit_array_index);
							if (player_ref.is_valid())
							{
								send_interaction_answer_to_two_side(player_ref.get_unit_guid(), m_interaction_type, e_interaction_reply_timeout);
							}
						}
						reset_interaction_info();
					}
				}
			}
		}
		
		//保护代码
		if (m_interaction_state == e_interaction_state_wait_start_real_interaction)
		{
			if (m_wait_real_interaction_time > 0)
			{
				m_wait_real_interaction_time -= ((new_time - m_old_tick_time) / second_tick_time);
				if (m_wait_real_interaction_time <= 0)
				{
					stop_interaction();
				}
			}
		}

		if (new_time > m_old_tick_time)
		{
			m_old_tick_time = new_time;
		}
	}

	void cinteraction_mgr::set_interaction_info(guid_64 target_guid, faith::e_interaction_type interaction_type)
	{
		m_interaction_target_guid = target_guid;
		m_interaction_type = interaction_type;

		m_handle_invite_cd_time = GAMECONFIG->InteractionCDTime;
	}

	void cinteraction_mgr::reset_interaction_info()
	{
		m_interaction_target_guid = faith::guid_64();
		m_interaction_type = faith::e_interaction_type_none;
		m_interaction_state = e_interaction_state_none;
		m_wait_real_interaction_time = -1;
	}

	faith::guid_64 cinteraction_mgr::get_interaction_guid()
	{
		return m_interaction_target_guid;
	}

	faith::e_interaction_type cinteraction_mgr::get_interaction_type()
	{
		return m_interaction_type;
	}

	faith::e_interaction_type cinteraction_mgr::get_real_interaction_type()
	{
		if (get_is_inviter() == true)
		{
			return m_interaction_type;
		}
		else
		{
			bool target_is_man = get_interaction_player_if_sex_is_man();
			return get_target_interaction_type(m_interaction_type, target_is_man);
		}
	}

	e_interaction_type cinteraction_mgr::get_target_interaction_type(faith::e_interaction_type interaction_type, bool target_is_man)
	{
		if (interaction_type == e_interaction_type_marry_raid_back_target_man || interaction_type == e_interaction_type_marry_raid_back_target_woman)
		{
			return get_marry_raid_interaction_type(false, target_is_man);
		}
		else if (interaction_type == e_interaction_type_marry_raid_front_target_man || interaction_type == e_interaction_type_marry_raid_front_target_woman)
		{
			return get_marry_raid_interaction_type(true, target_is_man);;
		}

		faith::template_manager::template_type* table = template_manager::get_instance().get_templates(e_InteractionTemplate);
		if (nullptr == table)
		{
			return e_interaction_type_none;
		}
		faith::template_manager::template_type::iterator ite;
		InteractionTemplate* interaction_template_ptr = nullptr;
		for (ite = table->begin(); ite != table->end(); ++ite)
		{
			interaction_template_ptr = (InteractionTemplate*)(ite->second);
			if (interaction_template_ptr == nullptr)
			{
				continue;
			}
			if (interaction_template_ptr->InteractionType == interaction_type)
			{
				return (e_interaction_type)interaction_template_ptr->CorrespondInteractionType;
			}
		}
		return faith::e_interaction_type_none;
	}

	faith::e_interaction_type cinteraction_mgr::get_marry_raid_interaction_type(bool is_back, bool is_man)
	{
		if (is_back)
		{
			if (is_man)
			{
				return e_interaction_type_marry_raid_back_target_man;
			}
			else
			{
				return e_interaction_type_marry_raid_back_target_woman;
			}
		}
		else
		{
			if (is_man)
			{
				return e_interaction_type_marry_raid_front_target_man;
			}
			else
			{
				return e_interaction_type_marry_raid_front_target_woman;
			}
		}
	}

	bool cinteraction_mgr::get_cur_player_if_sex_is_man()
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == false)
		{
			return false;
		}
		if ((player_ref.get_unit_info(e_role_info_class_type) == e_class_type_barserker) || (player_ref.get_unit_info(e_role_info_class_type) == e_class_type_guardian))
		{
			return true;
		}
		return false;
	}

	bool cinteraction_mgr::get_interaction_player_if_sex_is_man()
	{
		bool target_is_man = false;
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid() == true)
		{
			player& target_ref = unit_man::get_player(player_ref.get_interaction_mgr().get_interaction_guid());
			if (target_ref.is_valid() == true)
			{
				target_is_man = target_ref.get_interaction_mgr().get_cur_player_if_sex_is_man();
			}
		}
		return target_is_man;
	}

	void cinteraction_mgr::set_is_inviter(bool is_inviter)
	{
		m_is_inviter = is_inviter;
	}

	bool cinteraction_mgr::get_is_inviter()
	{
		return m_is_inviter;
	}

	void cinteraction_mgr::set_interaction_state(e_interaction_state state)
	{
		m_interaction_state = state;
	}

	void cinteraction_mgr::recv_interaction_invite(guid_64 invite_guid, faith::e_interaction_type interaction_type)
	{
		//当前是被邀请者

		player& invite_player_ref = unit_man::get_player(invite_guid);
		if (invite_player_ref.is_valid() == false)
		{
			return;
		}

		if (m_interaction_cdtime > utility::get_tick_count())
		{
			return;
		}

		m_interaction_cdtime = init_unit::get_end_time(GAMECONFIG->InteractionCDTime);

		//正在被邀请交互 交互中 则不能交互 直接返回交互应答
		if (m_handle_invite_cd_time > 0)
		{
			send_interaction_answer_to_two_side(invite_guid, interaction_type, faith::e_interaction_reply_be_invited);
			return;
		}
		if (m_interaction_target_guid.is_valid())
		{
			send_interaction_answer_to_two_side(invite_guid, interaction_type, faith::e_interaction_reply_is_interacting);
			return;
		}

		send_interaction_invite(invite_guid, interaction_type);
	}

	void cinteraction_mgr::send_interaction_invite(guid_64 invite_guid, faith::e_interaction_type interaction_type)
	{
		//当前是被邀请者

		player& be_invited_ref = unit_man::get_player(m_unit_array_index);
		if (be_invited_ref.is_valid() == false)
		{
			return;
		}
		player& invite_ref = unit_man::get_player(invite_guid);
		if (invite_ref.is_valid() == false)
		{
			return;
		}
		set_interaction_info(invite_guid, interaction_type);
		invite_ref.get_interaction_mgr().set_interaction_info(be_invited_ref.get_unit_guid(), interaction_type);

		set_is_inviter(false);
		invite_ref.get_interaction_mgr().set_is_inviter(true);

		set_interaction_state(e_interaction_state_wait_reply);
		invite_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_wait_reply);

		faith::interaction_proto_s_c_interaction_invite interaction_invite_proto;
		interaction_invite_proto.set_invite_guid(invite_guid.server_64);
		interaction_invite_proto.set_interaction_type(interaction_type);
		be_invited_ref.send_message_to_self(&interaction_invite_proto, e_msgindex_s2c_s_c_interaction_invite);
	}

	void cinteraction_mgr::recv_interaction_answer(guid_64 invite_guid, faith::e_interaction_type interaction_type, faith::e_interaction_reply result)
	{
		//当前是被邀请者

		if (result == faith::e_interaction_reply_agree)
		{
			//如果同意 则设置交互目标信息
			player& be_invited_ref = unit_man::get_player(m_unit_array_index);
			if (be_invited_ref.is_valid() == false)
			{
				return;
			}
			player& invite_ref = unit_man::get_player(invite_guid);
			if (invite_ref.is_valid() == false)
			{
				return;
			}
			if (m_interaction_type != interaction_type || m_interaction_target_guid != invite_guid)
			{
				reset_interaction_info();
				return;
			}

			guid_64 temp_target_guid = invite_ref.get_interaction_mgr().get_interaction_guid();
			faith::e_interaction_type temp_interaction_type = invite_ref.get_interaction_mgr().get_interaction_type();
			if (temp_target_guid != be_invited_ref.get_unit_guid() || temp_interaction_type != interaction_type)
			{
				invite_ref.get_interaction_mgr().reset_interaction_info();
			}
		}
		else
		{
			//拒绝或超时 重置交互信息
			guid_64 target_guid = m_interaction_target_guid;
			reset_interaction_info();
			player& target_ref = unit_man::get_player(target_guid);
			if (target_ref.is_valid())
			{
				target_ref.get_interaction_mgr().reset_interaction_info();
			}
		}
		send_interaction_answer_to_two_side(invite_guid, interaction_type, result);
	}

	void cinteraction_mgr::send_interaction_answer_to_two_side(guid_64 invite_guid, faith::e_interaction_type interaction_type, faith::e_interaction_reply result)
	{
		//当前是被邀请者

		player& be_invited_ref = unit_man::get_player(m_unit_array_index);
		if (be_invited_ref.is_valid() == false)
		{
			return;
		}
		player& invited_ref = unit_man::get_player(invite_guid);
		if (invited_ref.is_valid() == false)
		{
			return;
		}
		faith::interaction_proto_s_c_interaction_invite_reply interaction_invite_reply_proto;
		interaction_invite_reply_proto.set_invite_guid(invite_guid.server_64);
		interaction_invite_reply_proto.set_be_invited_guid(be_invited_ref.get_unit_guid().server_64);
		interaction_invite_reply_proto.set_interaction_type(interaction_type);
		interaction_invite_reply_proto.set_reply_result(result);
		//如果同意 则发给双方 否则只发给邀请者
		if (result == faith::e_interaction_reply_agree)
		{
			faith::template_manager::template_type* table = template_manager::get_instance().get_templates(e_InteractionTemplate);
			if (nullptr == table)
			{
				return;
			}
			int32 real_time = 10;
			faith::template_manager::template_type::iterator ite;
			InteractionTemplate* interaction_template_ptr = nullptr;
			for (ite = table->begin(); ite != table->end(); ++ite)
			{
				interaction_template_ptr = (InteractionTemplate*)(ite->second);
				if (interaction_template_ptr == nullptr)
				{
					continue;
				}
				if (interaction_template_ptr->InteractionType == interaction_type)
				{
					real_time = interaction_template_ptr->InviteTimeLimit;
				}
			}
			invited_ref.get_interaction_mgr().set_wait_real_interaction_time(real_time);
			be_invited_ref.get_interaction_mgr().set_wait_real_interaction_time(real_time);

			invited_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_wait_start_real_interaction);
			be_invited_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_wait_start_real_interaction);

			invited_ref.send_message_to_self(&interaction_invite_reply_proto, e_msgindex_s2c_s_c_interaction_invite_reply);
			be_invited_ref.send_message_to_self(&interaction_invite_reply_proto, e_msgindex_s2c_s_c_interaction_invite_reply);

			if (interaction_type == e_interaction_type_marry_dance_accord)
			{
				invited_ref.get_mission_mgr().target_check(e_mission_end_type_marry_dance);
				be_invited_ref.get_mission_mgr().target_check(e_mission_end_type_marry_dance);
			}
		}
		else
		{
			
			if (invited_ref.get_interaction_mgr().m_interaction_state == e_interaction_state_wait_reply)
			{
				invited_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_none);
				be_invited_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_none);

				invited_ref.send_message_to_self(&interaction_invite_reply_proto, e_msgindex_s2c_s_c_interaction_invite_reply);
			}
		}			
	}

	void cinteraction_mgr::recv_real_start_interaction(guid_64 be_invited_guid, faith::e_interaction_type interaction_type)
	{
		//当前为邀请方

		player& invited_ref = unit_man::get_player(m_unit_array_index);
		if (invited_ref.is_valid() == false)
		{
			return;
		}

		player& be_invited_ref = unit_man::get_player(be_invited_guid);
		if (be_invited_ref.is_valid() == false)
		{
			return;
		}
		if (m_interaction_target_guid != be_invited_guid || m_interaction_type != interaction_type)
		{
			return;
		}

		guid_64 temp_target_guid = be_invited_ref.get_interaction_mgr().get_interaction_guid();
		faith::e_interaction_type temp_interaction_type = be_invited_ref.get_interaction_mgr().get_interaction_type();
		if (temp_target_guid != invited_ref.get_unit_guid() || temp_interaction_type != interaction_type)
		{
			return;
		}
		invited_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_interacting);
		be_invited_ref.get_interaction_mgr().set_interaction_state(e_interaction_state_interacting);

		//如果是双骑并且双方都是骑乘状态 则被邀请方下马
		if (((invited_ref.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_mount_walk) || (invited_ref.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_mount_fly))
			&& ((be_invited_ref.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_mount_walk) || (be_invited_ref.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_mount_fly)))
		{
			buff_man::add_buff_inst(be_invited_ref.get_array_index(), be_invited_ref.get_array_index(), 10090000);
			buff_man::add_buff_inst(be_invited_ref.get_array_index(), be_invited_ref.get_array_index(), 10090012);
			buff_man::add_buff_inst(be_invited_ref.get_array_index(), be_invited_ref.get_array_index(), 10090014);
		}

		//首先设置交互属性再发送开始真正交互消息 因为客户端收到消息后需要根据交互属性处理
		set_interaction_type_attribute(be_invited_guid, interaction_type);
		send_real_start_interaction_to_aoi(invited_ref.get_unit_guid(), be_invited_guid, interaction_type);
		
		//如果是军团共舞需要发送共舞开始提示
		if (interaction_type == e_interaction_type_yao_legion_dance)
		{
			legion_proto_legion_dance_interaction_begin msg;
			invited_ref.send_message_to_self(&msg, e_mgsindex_s2c_legion_dance_interaction_begin);
			be_invited_ref.send_message_to_self(&msg, e_mgsindex_s2c_legion_dance_interaction_begin);
		}
	}

	void cinteraction_mgr::send_real_start_interaction_to_aoi(guid_64 invite_guid, guid_64 be_invited_guid, faith::e_interaction_type interaction_type)
	{
		//当前为邀请方

		player& invited_ref = unit_man::get_player(m_unit_array_index);
		if (invited_ref.is_valid() == false)
		{
			return;
		}
		faith::interaction_proto_s_c_real_start_interaction real_start_interaction_proto;
		real_start_interaction_proto.set_invite_guid(invite_guid.server_64);
		real_start_interaction_proto.set_be_invited_guid(be_invited_guid.server_64);
		real_start_interaction_proto.set_interaction_type(interaction_type);
		
		invited_ref.send_message_to_aoi(&real_start_interaction_proto, e_msgindex_s2c_s_c_real_start_interaction, true);
	}

	void cinteraction_mgr::set_interaction_type_attribute(guid_64 be_invited_guid, faith::e_interaction_type interaction_type)
	{
		//当前为邀请方
		player& invited_ref = unit_man::get_player(m_unit_array_index);
		if (invited_ref.is_valid() == false)
		{
			return;
		}
		player& be_invited_ref = unit_man::get_player(be_invited_guid);
		if (be_invited_ref.is_valid() == false)
		{
			return;
		}
		
		invited_ref.get_pawn_att().set_game_att(faith::e_unit_game_att_interaction, interaction_type, true);
		e_interaction_type target_interaction_type = get_target_interaction_type(interaction_type, get_cur_player_if_sex_is_man());
		be_invited_ref.get_pawn_att().set_game_att(faith::e_unit_game_att_interaction, target_interaction_type, true);
	}

	void cinteraction_mgr::stop_interaction()
	{
		player& cur_player_ref = unit_man::get_player(m_unit_array_index);
		if (cur_player_ref.is_valid() == false)
		{
			return;
		}
		stop_interaction_by_guid(cur_player_ref.get_unit_guid());
		stop_interaction_by_guid(m_interaction_target_guid);

		guid_64 target_guid = m_interaction_target_guid;
		reset_interaction_info();
		player& target_ref = unit_man::get_player(target_guid);
		if (target_ref.is_valid())
		{
			target_ref.get_interaction_mgr().reset_interaction_info();
		}
	}

	void cinteraction_mgr::stop_interaction_by_guid(guid_64 guid)
	{
		player& player_ref = unit_man::get_player(guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		int32 interaction_type = m_interaction_type;
		if (player_ref.get_interaction_mgr().get_is_inviter() == false)
		{
			bool target_is_man = player_ref.get_interaction_mgr().get_interaction_player_if_sex_is_man();
			interaction_type = get_target_interaction_type((faith::e_interaction_type)interaction_type, target_is_man);
		}

		faith::interaction_proto_s_c_stop_interaction stop_interaction_proto;
		stop_interaction_proto.set_role_guid(guid.server_64);
		stop_interaction_proto.set_interaction_type(interaction_type);
		player_ref.send_message_to_aoi(&stop_interaction_proto, e_msgindex_s2c_s_c_stop_interaction, true);

		player_ref.get_pawn_att().set_game_att(faith::e_unit_game_att_interaction, faith::e_interaction_type_none, true);
	}


	//保护代码
	void cinteraction_mgr::set_wait_real_interaction_time(float duration_time)
	{
		m_wait_real_interaction_time = duration_time;
	}

}
