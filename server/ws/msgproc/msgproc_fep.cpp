/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   15:00
	file base:	msgproc_fep
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "server/big_player_ws_mgr.h"
#include "server/client_session.hpp"
#include "server/client_session_mgr.hpp"
#include "server/login/login_service_mgr.hpp"
#include "server/lucky_draw_record_ws_mgr.h"
#include "server/queued_login_mgr.h"
#include "server/re_login_mgr.h"
#include "server/recharge/recharge_service.hpp"
#include "server/red_package/red_package_ws_mgr.h"
#include "server/team/team_ws_mgr.h"
#include "server/world_boss/world_boss_ws_mgr.h"
#include "character.pb.h"
#include "chat.pb.h"
#include "game.pb.h"
#include "invalid_word/CCharLib.hpp"
#include "invalid_word/invalid_ansi_word.h"
#include "msgproc_fep.hpp"
#include "net.pb.h"
#include "server_log.hpp"
#include "time.hpp"
#include "utility/init_unit.h"
#include "ws_client.hpp"
#include <char_msg.hpp>
#include <core.hpp>
#include <login_msg.hpp>
#include <Utility/parse_msg.h>

namespace faith
{

	void fep2ls_request_client_login(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		login_service_mgr::getInstance().req_login(data_ptr, data_len);
	}

	void fep2ls_enum_character_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		character_proto_enum_character pdata;
		parse_msg::getInstance().parse_message_new(&pdata, data_ptr, data_len);

		ls2dp_enum_character req;
		req.server_id = pdata.area_id();
		req.client_uid = client_uid;
		req.page_index = 1;
		XSTRNCPY(req.account, pdata.account().c_str(), ELEMENTOF(req.account));
		req.account[ELEMENTOF(req.account) - 1] = '\0';
		ws_client::getInstance().send_to_dp(&req, sizeof(req));
	}
	void fep2ls_create_character_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		fep2ls_create_character* pdata = (fep2ls_create_character*)(data_ptr);
		if (!pdata || data_len != sizeof(fep2ls_create_character))
		{
			return;
		}
		s_client_uid client_uid = pdata->client_uid;
		character_proto_create_character create_proto_msg;
		create_proto_msg.ParseFromArray(pdata->create_character_msg, pdata->create_character_len);

		e_create_role_result check_ret = e_create_role_success;
		//if (false == invalid_ansi_word::is_valid_ansi_str(create_proto_msg.name()))
		//{
		//	check_ret = e_failed_role_name_invalid;
		//}
		//if (invalid_ansi_word::include_invalid_ansi_str(create_proto_msg.name()))
		//{//存在非法字
		//	check_ret = e_failed_role_name_invalid;
		//}

		//判断角色名长度是否合法final
		if ((create_proto_msg.name().size() < faith::min_name_size))
		{
			check_ret = e_failed_role_name_size_too_short;
		}
		if ((create_proto_msg.name().size() > faith::max_name_size))
		{
			check_ret = e_failed_role_name_size_too_long;
		}
		if (check_ret != e_create_role_success)
		{
			ls2fep_create_role msg;
			msg.client_uid = client_uid;
			msg.eresult = check_ret;
			world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &msg, sizeof(msg));
			return;
		}

		int32 create_index = create_proto_msg.template_id();
		if (create_index >= GAMECONFIG->InitRoleAry.size())
		{
			return;
		}
		int32 player_template_id = GAMECONFIG->InitRoleAry[create_index];
		PlayerInitTemplate* template_data_ptr = GET_TEMPLATE(PlayerInitTemplate, player_template_id);
		if (template_data_ptr == NULL
			|| GAMECONFIG->PatronSaintSkillStartId.size() < template_data_ptr->Class)
		{
			return;
		}
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, GAMECONFIG->InitMapId);
		if (map_template_ptr == NULL || map_template_ptr->BornLocation.size() < 3 || map_template_ptr->BornRotation.size() < 3)
		{
			return;
		}

		ls2dp_create_character reqeust_db;
		reqeust_db.server_id = create_proto_msg.area_id();
		memcpy(reqeust_db.login_proto_msg, pdata->login_proto_msg, pdata->login_msg_len);
		reqeust_db.login_msg_len = pdata->login_msg_len;
		reqeust_db.client_uid = client_uid;
		size_t account_name_size_dst = sizeof(reqeust_db.init_role.account);
		size_t account_name_size_src = create_proto_msg.account().length();
		size_t account_name_size_cp = (account_name_size_dst > account_name_size_src ? account_name_size_src : account_name_size_dst);
		memcpy(reqeust_db.init_role.account, create_proto_msg.account().c_str(), account_name_size_cp);

		size_t player_name_size_dst = sizeof(reqeust_db.init_role.role_name);
		size_t player_name_size_src = create_proto_msg.name().length();
		size_t player_name_size_cp = (player_name_size_dst > player_name_size_src ? player_name_size_src : player_name_size_dst);
		memcpy(reqeust_db.init_role.role_name, create_proto_msg.name().c_str(), player_name_size_cp);

		size_t appearance_size_dst = sizeof(reqeust_db.init_role.role_appearance);
		size_t appearance_size_src = create_proto_msg.appearance().length();
		size_t appearance_size_cp = (appearance_size_dst > appearance_size_src ? appearance_size_src : appearance_size_dst);
		memcpy(reqeust_db.init_role.role_appearance, create_proto_msg.appearance().c_str(), appearance_size_cp);

		// 等级限制
		reqeust_db.create_level_requirement = template_data_ptr->CreateLevelRequirement;
		reqeust_db.init_role.data_ary[e_role_info_exp_level] = template_data_ptr->InitLevel;
		guid_64 player_id = guid_gen::make_guid();
		reqeust_db.init_role.role_guid.A = player_id.A;
		reqeust_db.init_role.role_guid.B = player_id.B;
		reqeust_db.init_role.data_ary[e_role_info_template_id] = player_template_id;
		reqeust_db.init_role.data_ary[e_role_info_server_id] = world_server::getInstance().get_server_id();
		reqeust_db.init_role.data_ary[e_role_info_move_server_id] = world_server::getInstance().get_server_id();
		reqeust_db.init_role.data_ary[e_role_info_class_type] = template_data_ptr->Class;
		reqeust_db.init_role.data_ary[e_role_info_img_id] = create_proto_msg.img_id();
		reqeust_db.init_role.data_ary[e_role_info_gender] = create_proto_msg.gender();
		reqeust_db.init_role.data_i64_ary[e_role_i64_info_create_time] = utility::get_tick_count();
		reqeust_db.init_role.data_i64_ary[e_role_i64_info_gs_value] = 298;
		reqeust_db.init_role.data_i64_ary[e_role_i64_info_history_best_gs_value] = 298;
		reqeust_db.init_role.data_ary[e_role_info_main_map_id] = GAMECONFIG->InitMapId;
		reqeust_db.init_role.data_ary[e_role_info_main_pos_x] = map_template_ptr->BornLocation[0];
		reqeust_db.init_role.data_ary[e_role_info_main_pos_y] = map_template_ptr->BornLocation[1];
		reqeust_db.init_role.data_ary[e_role_info_main_pos_z] = map_template_ptr->BornLocation[2];
		reqeust_db.init_role.data_ary[e_role_info_main_pos_yaw] = map_template_ptr->BornRotation[2];
		reqeust_db.init_role.data_ary[e_role_info_move_map_id] = GAMECONFIG->InitMapId;
		reqeust_db.init_role.data_ary[e_role_info_leave_map_id] = GAMECONFIG->InitMapId;
		reqeust_db.init_role.data_ary[e_role_info_move_pos_x] = map_template_ptr->BornLocation[0];
		reqeust_db.init_role.data_ary[e_role_info_move_pos_y] = map_template_ptr->BornLocation[1];
		reqeust_db.init_role.data_ary[e_role_info_move_pos_z] = map_template_ptr->BornLocation[2];
		reqeust_db.init_role.data_ary[e_role_info_move_pos_yaw] = map_template_ptr->BornRotation[2];
		reqeust_db.init_role.data_ary[e_role_info_upgrade_id] = template_data_ptr->UpgradeId;
		reqeust_db.init_role.data_ary[e_role_info_arena_times] = GAMECONFIG->ArenaTimes;
		reqeust_db.init_role.data_ary[e_role_info_watch_count] = 3;
		reqeust_db.init_role.data_ary[e_role_info_is_show_vip] = 1;
		reqeust_db.init_role.data_ary[e_role_info_push_map_id] = 0;
		if (GAMECONFIG->AoiWatchCountArr.size() == 3)
		{
			if (create_proto_msg.frame_quality() == 0)
			{
				reqeust_db.init_role.data_ary[e_role_info_watch_count] = GAMECONFIG->AoiWatchCountArr[0];
			}
			else if (create_proto_msg.frame_quality() == 1)
			{
				reqeust_db.init_role.data_ary[e_role_info_watch_count] = GAMECONFIG->AoiWatchCountArr[1];
			}
			else if (create_proto_msg.frame_quality() == 2)
			{
				reqeust_db.init_role.data_ary[e_role_info_watch_count] = GAMECONFIG->AoiWatchCountArr[2];
			}
		}
		reqeust_db.init_role.data_ary[e_role_info_show_fashion] = default_show_fashion;

		int32 protect_mark_slot_open_num = 0;
		for (int32 i = 0; i < GAMECONFIG->ProtectMarkSlotOpenGradeReqArray.size(); ++i)
		{
			if (GAMECONFIG->ProtectMarkSlotOpenGradeReqArray[i] == 0)
			{
				protect_mark_slot_open_num++;
			}
		}
		reqeust_db.init_role.data_ary[e_role_info_protect_mark_slot_open_num] = protect_mark_slot_open_num;

		int32 element_heart_slot_open_num = 0;
		for (int32 i = 0; i < GAMECONFIG->ElementHeartSlotOpenGradeReqArray.size(); ++i)
		{
			if (GAMECONFIG->ElementHeartSlotOpenGradeReqArray[i] == 0)
			{
				element_heart_slot_open_num++;
			}
		}
		reqeust_db.init_role.data_ary[e_role_info_element_heart_slot_open_num] = element_heart_slot_open_num;

		int32 sprite_equip_slot_open_num = 1;
		reqeust_db.init_role.data_ary[e_role_info_sprite_equip_slot_open_num] = sprite_equip_slot_open_num;


		// 初始化初始装备信息
		int32 item_count = 0;
		for (int32 i = 0; i < template_data_ptr->InitEquips.size(); ++i)
		{
			if (item_count >= init_item_num)
			{
				break;
			}
			if (init_unit::init_item_data(reqeust_db.init_item[item_count], template_data_ptr->InitEquips[i], 0, 1, e_bag_type_equip, 1))
			{
				++item_count;
			}
		}

		// 初始化背包物品信息
		if (template_data_ptr->InitItems.size() > 0
			&& template_data_ptr->InitItems.size() % 2 == 0)
		{
			int32 bag_slot = 0;
			int32 fashion_slot = 0;
			int32 temp_item_num = template_data_ptr->InitItems.size() / 2;

			for (int32 i = 0; i < temp_item_num; ++i)
			{
				if (item_count >= init_item_num)
				{
					break;
				}
				int32 item_id = template_data_ptr->InitItems[i * 2];
				int32 item_num = template_data_ptr->InitItems[i * 2 + 1];

				ItemTemplate* item_template = GET_TEMPLATE(ItemTemplate, item_id);
				if (nullptr == item_template)
				{
					continue;
				}

				switch (item_template->item_type)
				{
				case e_item_type_weapon:
				case e_item_type_armor:
					if (bag_slot < MAX_ITEM_BAG_NUM)
					{
						if (init_unit::init_item_data(reqeust_db.init_item[item_count], item_id, bag_slot, 1, e_bag_type_bag, 0))
						{
							++item_count;
							++bag_slot;
						}
					}
					break;
				case e_item_type_fashion:
					if (fashion_slot < max_fasion_item_bag_size)
						if (bag_slot < MAX_ITEM_BAG_NUM)
						{
							if (init_unit::init_item_data(reqeust_db.init_item[item_count], item_id, fashion_slot, 1, e_bag_type_fasion, 0))
							{
								++item_count;
								++fashion_slot;
							}
						}
					break;
				default:
					if (bag_slot < MAX_ITEM_BAG_NUM)
					{
						if (init_unit::init_item_data(reqeust_db.init_item[item_count], item_id, bag_slot, item_num, e_bag_type_bag, 0))
						{
							++item_count;
							++bag_slot;
						}
					}
					break;
				}


			}
		}

		int32 skill_count = 0;
		//创建初始化技能

		//for (int32 i = 0; i < template_data_ptr->InstSkills.size(); i++)
		//{
		//	if (init_unit::init_skill_data(reqeust_db.init_skill[skill_count], template_data_ptr->InstSkills[i]))
		//	{
		//		//创建角色时设为不可用，由新手引导（任务）激活
		//		//下面废弃了，因为在init时，等级限制为0的，就是一上来不给的技能了
		//		//reqeust_db.init_skill[skill_count].data_ary[e_skill_info_activated] = 0;
		//		++skill_count;
		//	}
		//}
		for (int32 i = 0; i < template_data_ptr->AssistSkills.size(); i++)
		{
			if (init_unit::init_skill_data(reqeust_db.init_skill[skill_count], template_data_ptr->AssistSkills[i], 1))
			{
				//创建角色时设为不可用，由新手引导（任务）激活
				//下面废弃了，因为在init时，等级限制为0的，就是一上来不给的技能了
				//reqeust_db.init_skill[skill_count].data_ary[e_skill_info_activated] = 0;
				++skill_count;
			}
		}
		for (int32 i = 0; i < GAMECONFIG->PatronSaintSkillStartId.size(); i++)
		{
			if (init_unit::init_skill_data(reqeust_db.init_skill[skill_count], GAMECONFIG->PatronSaintSkillStartId[i]))
			{
				++skill_count;
			}
		}


		//初始化引导
		faith::template_manager::template_type* table = template_manager::get_instance().get_templates(e_GuideTriggerTemplate);
		if (nullptr == table)
		{
			return;
		}
		faith::template_manager::template_type::iterator ite;
		GuideTriggerTemplate* guide_trigger_template_ptr = nullptr;
		reqeust_db.record_num = 0;
		for (ite = table->begin(); ite != table->end(); ++ite)
		{
			guide_trigger_template_ptr = (GuideTriggerTemplate*)(ite->second);
			reqeust_db.record_info_list[reqeust_db.record_num].guide_trigger_id = guide_trigger_template_ptr->attribute_id;
			reqeust_db.record_info_list[reqeust_db.record_num].state = 0;
			reqeust_db.record_num++;
		}

		reqeust_db.item_num = item_count;
		reqeust_db.skill_num = skill_count;
		ws_client::getInstance().send_to_dp(&reqeust_db, sizeof(reqeust_db));
	}
	void fep2ls_delete_character_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		character_proto_delete_character delete_char;
		parse_msg::getInstance().parse_message_new(&delete_char, data_ptr, data_len);

		faith::ls2dp_delete_character request;
		request.client_uid = client_uid;
		request.server_id = delete_char.server_id();
		request.role_guid.server_64 = delete_char.role_guid();
		request.del_type = (e_del_char_type)delete_char.del_type();
		int32 cpy_len = delete_char.account().size() > max_account_length ? max_account_length : delete_char.account().size();
		memcpy(request.account, delete_char.account().c_str(), cpy_len);
		ws_client::getInstance().send_to_dp(&request, sizeof(request));
	}

	void fep2ws_client_reconnect_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const fep2ws_client_reconnect* packet = static_cast<const fep2ws_client_reconnect*>(data_ptr);
		if (!packet || data_len != sizeof(fep2ws_client_reconnect))
		{
			string str = " the param is err";
			server_log::reconnect_game_log("ws", __FUNCTION__, str.c_str());

			return;
		}
		client_session* old_session_ptr = client_session_mgr::getInstance().get_session(packet->account);
		if (nullptr == old_session_ptr)
		{
			ws2fep_reconnect_game fep_msg;
			fep_msg.client_uid = packet->client_uid;
			fep_msg.reconnect_res = e_reconnect_result_ws_no_find;
			memcpy(fep_msg.account, packet->account, sizeof(fep_msg.account));
			world_server::getInstance().send_to_fep(packet->client_uid.fepserver_uid, &fep_msg, sizeof(fep_msg));

			string str = " the session is null";
			server_log::reconnect_game_log("ws", __FUNCTION__, str.c_str());

			return;
		}

		ws2fep_kickout_account msg;
		msg.client_uid = old_session_ptr->m_client_uid;
		msg.reason = e_logout_result_replaced_by_new_login;
		world_server::getInstance().send_to_fep(old_session_ptr->m_client_uid.fepserver_uid, &msg, sizeof(msg));

		old_session_ptr->m_client_uid = packet->client_uid;
		if (old_session_ptr->get_cs_conn_index() >= 0)
		{
			ws2cs_reconnect_game cs_msg;
			cs_msg.client_uid = packet->client_uid;
			cs_msg.role_guid = old_session_ptr->get_role_guid();
			cs_msg.cur_map_id = packet->cur_map_id;
			cs_msg.cur_map_line_id = packet->cur_map_line_id;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_reconnect_game);
			if (is_use == false)
			{
				old_session_ptr->send_to_cs(&cs_msg, sizeof(cs_msg));
			}
			else
			{
				faith::ws2cs_proto::reconnect_game msg;
				msg.set_client_uid(packet->client_uid.fep_uid_64);
				msg.set_role_guid(old_session_ptr->get_role_guid().server_64);
				msg.set_cur_map_id(packet->cur_map_id);
				msg.set_cur_map_line_id(packet->cur_map_line_id);
				old_session_ptr->send_to_cs_lua(&msg, e_msg_index_ws2cs_reconnect_game);
			}
		}
		else if (old_session_ptr->m_status == client_session::e_ss_login_ok)
		{
			old_session_ptr->m_logout_begin = false;
			old_session_ptr->m_logout_time = 0;
			ws2fep_reconnect_game fep_msg;
			fep_msg.client_uid = packet->client_uid;
			fep_msg.cellserver_id = old_session_ptr->get_cs_conn_index();
			fep_msg.array_index = old_session_ptr->get_cs_array_index();
			fep_msg.reconnect_res = e_reconnect_result_win;
			memcpy(fep_msg.account, old_session_ptr->m_account, sizeof(fep_msg.account));
			old_session_ptr->send_to_fep(&fep_msg, sizeof(fep_msg));
		}
		else
		{
			ws2fep_reconnect_game fep_msg;
			fep_msg.client_uid = packet->client_uid;
			fep_msg.reconnect_res = e_reconnect_result_ws_no_find;
			memcpy(fep_msg.account, packet->account, sizeof(fep_msg.account));
			world_server::getInstance().send_to_fep(packet->client_uid.fepserver_uid, &fep_msg, sizeof(fep_msg));
		}
	}
	void fep2ws_client_leave_queued(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const fep2ws_leave_queued* packet = static_cast<const fep2ws_leave_queued*>(data_ptr);
		if (!packet || data_len != sizeof(fep2ws_leave_queued))
			return;
		client_session* old_session_ptr = client_session_mgr::getInstance().get_session(packet->account);
		if (nullptr == old_session_ptr)
		{
			return;
		}
		if (old_session_ptr->m_online_state == client_session::e_os_inqueue)
		{
			queued_login_mgr::getInstance().on_session_logout(old_session_ptr);
		}
	}

	void fep2ws_client_loginned(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const fep2ws_client_logined* packet = static_cast<const fep2ws_client_logined*>(data_ptr);
		if (!packet || data_len != sizeof(fep2ws_client_logined))
			return;

		client_session* old_session_ptr = client_session_mgr::getInstance().get_session(packet->account);
		if (old_session_ptr != NULL)
		{//有账号已经登录了,干掉前一个登陆者,加入到等待列表
			if (old_session_ptr->m_status != client_session::e_ss_logout)
			{
				client_session_mgr::getInstance().kickout_account(old_session_ptr->m_client_uid, e_logout_result_replaced_by_new_login);
			}
			s_re_login_info temp_info;
			memcpy(temp_info.m_account, packet->account, sizeof(temp_info.m_account));
			memcpy(&(temp_info.m_login_data), packet, sizeof(fep2ws_client_logined));
			re_login_mgr::getInstance().add_re_login(packet->account, temp_info);
		}
		else
		{
			re_login_mgr::getInstance().remove_re_login(packet->account);
			client_session_mgr::getInstance().login_client(data_ptr);
		}
	}

	void c2ws_req_enter_game(uint32 conn_index,const void* data_ptr,size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_enter_game packet;
		parse_msg::getInstance().parse_message_new(&packet,data_ptr,data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if(session == NULL)
		{
			CONSOLE_ERROR("c2ws_req_enter_game session == NULL client_uid = {}", client_uid.fepsession_uid);
			return;
		}
		if(session->m_status == client_session::e_ss_map_transfer)
		{
			CONSOLE_ERROR("c2ws_req_enter_game session->m_status = {}", (int32)session->m_status);
			return;
		}
		
		guid_64 role_guid(packet.role_guid());
		session->player_enter_game_transfer(role_guid, 0);
	}

	void c2ws_enter_scene(uint32 conn_indexer,const void* data_ptr,size_t data_len)
	{
		game_proto_enter_scene enter_scene;
		parse_msg::getInstance().parse_message_new(&enter_scene,data_ptr,data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr,client_uid);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if(nullptr == session)
		{
			CONSOLE_INFO("c2ws_enter_scene session is null  client_uid = {}", client_uid.fepsession_uid);
			return;
		}
		if (session->get_role_guid().is_valid() == false)
		{
			CONSOLE_INFO("c2ws_enter_scene session guid is null ");
		}
		if(session->m_status != client_session::e_ss_ingame)
		{
			CONSOLE_INFO("c2ws_enter_scene session m_status:{}", (int32)session->m_status);
			return;
		}
		session->m_step_num = client_session::e_session_step_send_enter_scene;
		ws2cs_enter_scene send_msg;
		send_msg.role_guid = session->get_role_guid();
		send_msg.cs_array_index = session->get_cs_array_index();

		bool is_use = proto_by_lua(e_msg_index_ws2cs_enter_scene);
		if (is_use == false)
		{
			session->send_to_cs(&send_msg, sizeof(send_msg));
		}
		else
		{
			faith::ws2cs_proto::enter_scene pro_msg;
			pro_msg.set_role_guid(session->get_role_guid().server_64);
			pro_msg.set_cs_array_index(session->get_cs_array_index());
			session->send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_enter_scene);

		}

		//load世界boss状态
		world_boss_ws_mgr::get_instance().send_world_boss_all(session, false);

		//load膜拜雕像的数据
		big_player_ws_mgr::get_instance().load_statue_by_ws_proc(session->get_role_guid());

		//load红包
		red_package_ws_mgr::get_instance().send_info_to_one(session->get_role_guid());

		//load抽奖信心
		lucky_draw_record_ws_mgr::get_instance().send_record_all_to_req_player(session->get_role_guid());

		//载入ws_info
		session->send_ws_info_all();

	}

	void c2ws_send_recruit_proc(uint32 conn_indexer, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		chat_proto_recruit_info packet;
		parse_msg::getInstance().parse_message_new(&packet, data_ptr, data_len);
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (session == NULL
			|| session->m_status == client_session::e_ss_map_transfer)
		{
			return;
		}
		guid_64 team_guid = packet.team_guid();
		team_guid.server_64 = packet.team_guid();
		int32 send_result = team_ws_mgr::get_instance().send_recruit_proc(team_guid, packet.aim_type(), packet.aim_sub_id(), packet.recruit_type(), session->get_role_guid(), session->m_role_info.role_name, session->get_role_info_data(faith::e_role_info_template_id), packet.one_dragon_flag());

		chat_proto_recruit_end_info end_msg;
		end_msg.set_recruit_result(send_result);
		end_msg.set_recruit_type(packet.recruit_type());
		session->send_to_client(&end_msg, e_msgindex_s2c_send_recruit_result);
	}

	void c2ws_recharge_success_by_sdk_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		character_proto_recharge_success_by_sdk packet;
		parse_msg::getInstance().parse_message_new(&packet, data_ptr, data_len);
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (client_session_ptr == NULL)
		{
			return;
		}
		server_log::recharge_step_log(packet.order_id().c_str(), packet.role_guid(), packet.goods_id(), e_recharge_step_begin, client_session_ptr->m_login_third_data, client_session_ptr->m_login_type, &client_session_ptr->m_role_info);
		int64 now_tick = utility::get_tick_count();
		if (now_tick < client_session_ptr->m_recharge_time)
		{
			return;
		}
		client_session_ptr->m_recharge_time = now_tick + second_tick_time;

		recharge_service::getInstance().req_recharge(packet.role_guid(), packet.token(), packet.goods_id(), packet.order_id(), packet.payment_type(), packet.package_name());
	}

	void fep2ws_req_logout_client(uint32 conn_indexer,const void* data_ptr,size_t data_len)
	{
		const fep2ws_client_logout* packet= static_cast<const fep2ws_client_logout*>(data_ptr);
		if(packet == NULL)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(packet->client_uid);
		if(client_session_ptr == NULL)
		{
			return;
		}

		if (packet->need_save_online_time == true)
		{
			ws2dp_save_account_online_time req;
			memcpy(req.account, packet->account, sizeof(req.account));
			req.online_duration = packet->online_duration;
			req.online_time = packet->online_time;
			req.month_recharge_num = packet->month_recharge_num;
			req.month_recharge_time = packet->month_recharge_time;
			ws_client::getInstance().send_to_dp(&req, sizeof(req));
		}

		//if (client_session_ptr->m_step_num == client_session::e_session_step_cs_enter_scene)
		//{
		//	client_session_ptr->set_client_logout_begin();
		//	marry_mgr_ws::get_instance().session_offline_marry_handle(client_session_ptr->get_role_guid());
		//	team_ws_mgr::get_instance().on_player_offline(client_session_ptr);
		//	ws2cs_time_out msg;
		//	msg.role_guid = client_session_ptr->get_role_guid();
		//	msg.cs_array_index = client_session_ptr->get_cs_array_index();
		//	bool is_use = proto_by_lua(e_msg_index_ws2cs_time_out);
		//	if (is_use == false)
		//	{
		//		world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
		//	}
		//	else
		//	{
		//		faith::ws2cs_proto::time_out pro_msg;
		//		pro_msg.set_role_guid(client_session_ptr->get_role_guid().server_64);
		//		pro_msg.set_cs_unit_index(client_session_ptr->get_cs_array_index());
		//		world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_time_out, e_server_type_cs);
		//	}
		//}
		//else
		{
			queued_login_mgr::getInstance().on_session_logout(client_session_ptr, false);
			client_session_mgr::getInstance().kickout_account(packet->client_uid, e_logout_result_connect_dis);
		}
	}
}
