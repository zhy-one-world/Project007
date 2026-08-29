/********************************************************************
	created:	2015��12��28��16:11:23
	file base:	msgproc_client
	file ext:	cpp
	author:		zhy

	purpose:
*********************************************************************/


#include "cell_server.hpp"
#include "components/scene/base_map_component.h"
#include "connection_mgr.hpp"
#include "cross_ladder_def.hpp"
#include "game_cfg/servers_config.h"
#include "internal/core.hpp"
#include "internal/cross_ladder_msg.hpp"
#include "internal/element_war_msg.hpp"
#include "internet/achievement.pb.h"
#include "internet/aoi.pb.h"
#include "internet/arena.pb.h"
#include "internet/assist_fight.pb.h"
#include "internet/buff.pb.h"
#include "internet/character.pb.h"
#include "internet/chat.pb.h"
#include "internet/cross_ladder.pb.h"
#include "internet/dragontrip.pb.h"
#include "internet/elementheart.pb.h"
#include "internet/finger_guess.pb.h"
#include "internet/first_time_do.pb.h"
#include "internet/game.pb.h"
#include "internet/goods.pb.h"
#include "internet/interaction.pb.h"
#include "internet/legion.pb.h"
#include "internet/mail.pb.h"
#include "internet/map.pb.h"
#include "internet/map_award.pb.h"
#include "internet/mission.pb.h"
#include "internet/mountpower.pb.h"
#include "internet/net.pb.h"
#include "internet/phantom.pb.h"
#include "internet/ranking.pb.h"
#include "internet/recycle.pb.h"
#include "internet/relation.pb.h"
#include "internet/service_goal.pb.h"
#include "internet/skill.pb.h"
#include "internet/skytreasure.pb.h"
#include "internet/smashing_bottle.pb.h"
#include "internet/starark.pb.h"
#include "internet/talent.pb.h"
#include "internet/time_activity.pb.h"
#include "internet/time_limit_activity.pb.h"
#include "internet/title.pb.h"
#include "internet/welfare.pb.h"
#include "logic/aoi/aoi_system.h"
#include "logic/buff_man.h"
#include "Logic/chat_def.hpp"
#include "Logic/element_war_def.hpp"
#include "Logic/gm_order_def.hpp"
#include "logic/item_set.h"
#include "Logic/map_def.hpp"
#include "logic/npc.hpp"
#include "logic/offline_award_system.h"
#include "logic/online_award_system.h"
#include "logic/skill_manager.h"
#include "logic/skill_set.h"
#include "Logic/time_limit_activity_temp_mgr.hpp"
#include "logic/unit_man.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"

#include "msgproc_client.hpp"
#include "msgproc_gm.hpp"
#include "server_log.hpp"
#include "server_log.hpp"
#include "system/item/item_composit_system.h"
#include "system/item/item_element_system.h"
#include "system/item/item_system.h"
#include "system/item/item_upgrade_system.h"
#include "system/npc/box_award_system.h"
#include "system/npc/treasure_pic_system.h"
#include "system/play/finger_guess_system.h"
#include "system/play/smashing_bottle_system.h"
#include "system/play/wheel_plate_system.h"
#include "system/scene/battle_map_system.h"
#include "system/scene/box_map_system.h"
#include "system/scene/element_map_system.h"
#include "system/scene/flip_box_system.h"
#include "system/scene/map_search_system.h"
#include "template/template_manager.h"
#include <time.hpp>
#include <Utility/init_unit.h>
#include <Utility/parse_msg.h>
namespace faith
{
#define  UNPACKING_PROTO(ProtoName) \
	ZoneScoped;\
	ProtoName request;\
	bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);\
	if (!suc)\
	{\
		CONSOLE_ERROR("parse_message_new fail data_len:{}", data_len);\
		return;\
	}\
	s_client_uid client_uid;\
	parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);\
	player& player_ref = unit_man::get_player(client_uid.fepsession_uid);\
	if (false == player_ref.is_valid() || player_ref.get_session_state() != e_session_status_in_gaming || player_ref.get_client_uid().rand_num_1 != client_uid.rand_num_1)\
	{\
		CONSOLE_ERROR("player is null {}", client_uid.fepsession_uid);\
		return;\
	}
	void req_fep2cs_in_game(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const fep2cs_in_game* pdata = static_cast<const fep2cs_in_game*>(data_ptr);
		if (NULL == pdata)
			return;
		player& player_ref = unit_man::get_player(pdata->array_index);
		if (player_ref.get_unit_guid() != pdata->role_guid)
		{
			return;
		}
		if (pdata->be_in_game)
		{
			player_ref.set_leave_num(0);
		}
		else
		{
			int32 leave_num = player_ref.get_leave_num();
			leave_num += 1;
			if (leave_num >= diconnect_repeat_num)
			{
				unit_man::logout_player(pdata->role_guid, pdata->array_index, false, e_logout_result_time_out);
			}
			else
			{
				player_ref.set_leave_num(leave_num);
			}
		}
	}
	void c2cs_ping(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_client2cs_ping)

		game_proto_cs2client_ping msg;
		msg.set_trace_time(request.trace_time());
		player_ref.send_message_to_self(&msg, e_msgindex_cs2c_ping);
	}
	//******************************************************** npc *************************************************************************************//


	void c2cs_req_temple_gift_num(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_temple_over_gift)
		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
		if (nullptr == map_object_ptr)
		{
			return;
		}
		map_object_ptr->get_game_over_gift(player_ref.get_array_index(), request.get_over_num());
	}

	void c2cs_req_map_tigger_volume(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		world_cs::c2cs_req_map_tigger_volume(connindex, data_ptr, data_len);
	}
	void c2cs_jump_raid_animation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		world_cs::c2cs_jump_raid_animation(connindex, data_ptr, data_len);
	}
	void c2cs_req_interact_with_npc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_interact_with_npc)
		const guid_64 npc_guid = request.npc_guid();
		player_ref.begin_interact_with_npc(npc_guid);
	}

	//******************************************************** player *************************************************************************************//

	void c2cs_req_aoi_location(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		aoi_proto_unit_location req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		player& player_ref = unit_man::get_player(client_uid.fepsession_uid);
		if (false == player_ref.is_valid() || player_ref.get_session_state() != e_session_status_in_gaming || player_ref.get_client_uid().rand_num_1 != client_uid.rand_num_1
			|| player_ref.get_loading_flag_all_finish() == false
			|| player_ref.get_saveing_flag_all_finish() == false
			|| player_ref.is_during_transfer())
		{
			return;
		}
		c2cs_req_aoi_location_logic(player_ref, req);
	}
	void c2cs_req_aoi_location_logic(player& player_ref, aoi_proto_unit_location& req)
	{
		if (player_ref.is_dead())
		{
			return;
		}
		int64 time_now = utility::get_tick_count();
		if (time_now < player_ref.get_time_stamp())
		{
			return;
		}
		fvector location(req.pos_x(), req.pos_y(), req.pos_z());
		//if (player_ref.get_pawn_att().move_check(location, time_now) == false)
		//{
		//	CONSOLE_ERROR("move_check fail");
		//	player_ref.get_move_mgr().send_unit_end_location(player_ref.get_old_map_pos().unit_location);
		//	return;
		//}
		s_map_pos map_pos(location, player_ref.get_new_map_pos().unit_rotation);
		player_ref.get_meditation_mgr().break_meditation();
		player_ref.stop_interact_with_npc(true, true);
		player_ref.trigger_unit_event(e_game_event_type_unit_move);
		player_ref.set_new_map_pos(map_pos);
		player_ref.set_time_stamp(time_now + deci_second_tick_time);
		player_ref.get_move_mgr().tick_sync_pos(time_now);
	}

	void c2cs_req_aoi_location_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(aoi_proto_unit_location, c2cs_req_aoi_location)
	}
	void c2cs_req_aoi_rotation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(aoi_proto_unit_rotation)
		c2cs_req_aoi_rotation_logic(player_ref, request);
	}
	void c2cs_req_aoi_rotation_logic(player& player_ref, aoi_proto_unit_rotation& req)
	{
		if (player_ref.is_dead())
		{
			return;
		}
		player_ref.set_new_map_angle(req.yaw());
		int64 time_now = utility::get_tick_count();
		player_ref.set_time_stamp(time_now);
		player_ref.get_move_mgr().tick_sync_pos(time_now);
	}
	void c2cs_req_aoi_rotation_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(aoi_proto_unit_rotation, c2cs_req_aoi_rotation)
	}
	void c2cs_req_aoi_show_player(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(aoi_proto_show_player)
		//////////////////////////////////////////////
		player_ref.aoi_change_show_player(request.show_num());
	}
	void c2cs_req_aoi_simple_pos(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(aoi_proto_simple_unit_pos)
		if (request.unit_guid_size() != request.runtime_id_size())
		{
			return;
		}
		c2cs_req_aoi_simple_pos_logic(player_ref, request);
	}

	void c2cs_req_aoi_simple_pos_logic(player& player_ref, aoi_proto_simple_unit_pos& req)
	{
		aoi_proto_simple_unit_pos_end msg;
		int32 send_data_num = 0;
		for (int32 i = 0; i < req.unit_guid_size(); ++i)
		{
			guid_64 unit_guid(req.unit_guid(i));
			unit& unit_ref = unit_man::get_unit(req.runtime_id(i));
			if (unit_ref.is_valid() && unit_ref.get_unit_guid() == unit_guid)
			{
				msg.add_unit_guid(unit_guid.server_64);
				msg.add_pox_x(unit_ref.get_new_map_pos().unit_location.x);
				msg.add_pox_y(unit_ref.get_new_map_pos().unit_location.y);
				msg.add_pox_z(unit_ref.get_new_map_pos().unit_location.z);
				msg.set_att_info(unit_ref.get_pawn_att().get_game_att(e_unit_game_att_movement));
				msg.set_att_state(unit_ref.get_pawn_att().get_state_att(e_unit_attack_state_dead));
				send_data_num++;
			}
			if (send_data_num >= 100)
			{
				player_ref.send_message_to_self(&msg, e_msgindex_s2c_aoi_simple_pos_end);
				msg.Clear();
				send_data_num = 0;
			}
		}
		if (send_data_num > 0)
		{
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_aoi_simple_pos_end);
		}
	}

	void c2cs_req_aoi_simple_pos_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(aoi_proto_simple_unit_pos, c2cs_req_aoi_simple_pos)
	}

	void c2cs_req_transfer_scene_pos(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_transfer_scene_pos);
		c2cs_req_transfer_scene_pos_logic(player_ref, request);
	}

	void c2cs_req_transfer_scene_pos_logic(player& player_ref, game_proto_transfer_scene_pos& req)
	{
		if (player_ref.get_pawn_att().get_game_att(e_unit_game_att_war_state))
		{
			//return;
		}
		if (req.location().size() < 3)
		{
			return;
		}
		if (req.rotation().size() < 3)
		{
			return;
		}

		int32 map_template_id = req.map_template_id();
		int32 line_id = req.line_id();
		// Ϊ�˷�������ͼ�н���Ҵ������������ͼ��ֻ�õ��ô��͵�ʱ��0�Ϳ�����
		if (0 == map_template_id)
		{
			//map_template_id = player_ref.get_unit_info(e_role_info_main_map_id);
			map_template_id = big_world_map_id;
		}
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}

		auto cur_map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
		if (nullptr == cur_map_object_ptr)
		{
			return;
		}
		guid_64 map_guid(player_ref.get_map_guid());

		MapTemplate* cur_map_template_ptr = cur_map_object_ptr->get_map_template_ptr();
		if (nullptr == cur_map_template_ptr)
		{
			return;
		}

		//if (map_template_ptr->Type != e_map_type_big_map
		//	&& cur_map_template_ptr->Type != e_map_type_big_map)
		//{
		//	return;
		//}
		if ((cur_map_object_ptr->get_line_id() != line_id && cur_map_template_ptr->Type == e_map_type_big_map) || (cur_map_object_ptr->get_map_template_id() != map_template_id && map_template_ptr->Type == e_map_type_big_map))
		{
			map_guid.clear_data();
		}
		s_map_pos map_pos;
		map_pos.clear_data();
		map_pos.set_location(req.location(0), req.location(1), req.location(2));
		map_pos.set_rotation(req.rotation(0), req.rotation(1), req.rotation(2));

		player_ref.transfer_by_template_pos(map_template_id, line_id, map_pos, map_guid);
	}

	void c2cs_req_transfer_scene_pos_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(game_proto_transfer_scene_pos, c2cs_req_transfer_scene_pos)
	}

	void c2cs_back_to_self_server(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_cross_server_pk_map_transfer)
		c2cs_back_to_self_server_logic(player_ref, request);
	}

	void c2cs_back_to_self_server_logic(player& player_ref, game_proto_cross_server_pk_map_transfer& req)
	{
		auto cur_map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
		if (nullptr == cur_map_object_ptr)
		{
			return;
		}

		MapTemplate* cur_map_template_ptr = cur_map_object_ptr->get_map_template_ptr();
		if (nullptr == cur_map_template_ptr)
		{
			return;
		}
		if (!init_unit::is_map_cross_server(cur_map_template_ptr->attribute_id, cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))
			&& false == init_unit::is_map_other_server(cur_map_template_ptr->attribute_id)
			&& false == init_unit::is_map_all_server(cur_map_template_ptr->attribute_id))//Ŀǰ��ֻ�п�������õ��������
		{
			return;
		}

		int32 map_temp_id = player_ref.get_unit_info(e_role_info_main_map_id);
		int32 line_id = 0;
		int32 server_id = player_ref.get_unit_info(e_role_info_server_id);
		player_ref.transfer_by_template(map_temp_id, line_id, server_id, guid_64(), 0);
	}

	void c2cs_back_to_self_server_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(game_proto_cross_server_pk_map_transfer, c2cs_back_to_self_server)
	}

	void c2cs_req_transfer_scene(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_transfer_scene)
		if (player_ref.get_client_uid().rand_num_1 != client_uid.rand_num_1)
		{
			CONSOLE_ERROR("player_ref is null error");
			return;
		}

		if (request.map_guid().size() < guid_size)
		{
			CONSOLE_ERROR("guid is error");
			return;
		}
		int32 map_template_id = request.map_template_id();
		int32 line_id = request.line_id();
		int32 group_id = request.group_id();
		guid_64 map_guid(request.map_guid(0), request.map_guid(1));
		
		// Ϊ�˷�������ͼ�н���Ҵ������������ͼ ֻ�õ��ô��͵�ʱ��0�Ϳ�����
		if (0 == map_template_id)
		{
			map_template_id = player_ref.get_unit_info(e_role_info_main_map_id);
		}
		auto map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			CONSOLE_ERROR("map_template_ptr is error map_template_id:{}", map_template_id);
			return;
		}
		if (nullptr == player_ref.get_map_ent())
		{
			CONSOLE_ERROR("map_ent is error map_template_id:{}", map_template_id);
			return;
		}
		auto base_map_cp = player_ref.get_map_ent()->get_component<base_map_component>();

		auto cur_map_template_ptr = base_map_cp->m_map_template;

		if (group_id > 0)
		{
			if (false == init_unit::get_map_public(cur_map_template_ptr->Type))
			{
				CONSOLE_ERROR("map_ent is error map_type:{}", cur_map_template_ptr->Type);
				return;
			}
		}

		bool is_need_check_map = true;
		if (init_unit::get_map_public(map_template_ptr->Type) || init_unit::get_map_public(cur_map_template_ptr->Type))
		{
			//������ͼ���Դ����κε�ͼ �κε�ͼ���Դ���������ͼ
		}
		else if (init_unit::is_goto_same_map(map_template_id, line_id, base_map_cp->m_map_id, base_map_cp->m_line_id))
		{
			// ����ͬ��ͼ����
			CONSOLE_ERROR("same map");
			return;
		}
		else if (map_template_ptr->Type == e_map_type_demon_tower_fuben && cur_map_template_ptr->Type == e_map_type_demon_tower_fuben)
		{
			//����ħ����������ֱ�ӽ���һ����ħ������ ��һ��
		}
		else
		{
			//���ܴ�һ������ֱ�Ӵ��͵���һ������
			CONSOLE_ERROR("map other error");
			return;
		}

		if (player_ref.can_entry_fuben(map_template_id, true, cell_server::getInstance().get_is_cross_sever(group_id)) == false)
		{
			CONSOLE_ERROR("player_ref.can_entry_fuben false");
			return;
		}
		//����������������е�ͼ
		if (false == init_unit::is_map_cross_server(map_template_id, cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity)) && map_template_ptr->Type != e_map_type_big_map && false == init_unit::is_map_all_server(map_template_id))
		{
			if (false == player_ref.is_self_server())
			{
				CONSOLE_ERROR("cross server false");
				return;
			}
		}

		if (map_object::get_activity_map_sec_left(map_template_ptr->Type, player_ref.get_array_index()) < 0)
		{
			return;
		}

		player_ref.transfer_by_template(map_template_id, line_id, group_id, map_guid, request.war_index());
	}

	void c2cs_req_transfer_scene_logic(player& player_ref, game_proto_transfer_scene& req)
	{
		if (req.map_guid().size() < guid_size)
		{
			return;
		}
		int32 map_template_id = req.map_template_id();
		int32 line_id = req.line_id();
		int32 group_id = req.group_id();
		guid_64 map_guid(req.map_guid(0), req.map_guid(1));

		// Ϊ�˷�������ͼ�н���Ҵ������������ͼ ֻ�õ��ô��͵�ʱ��0�Ϳ�����
		if (0 == map_template_id)
		{
			map_template_id = player_ref.get_unit_info(e_role_info_main_map_id);
		}
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}

		if (map_template_ptr->Type == e_map_type_crystak_dreamland)
		{//�ж� ˮ���þ��Ƿ���
			bool is_open = cell_server::getInstance().get_activity_sec_left(e_activity_type_crystal_fairyland, e_activity_time_get_all) > 0;
			if (false == is_open)
			{
				return;
			}
		}

		auto cur_map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
		if (nullptr == cur_map_object_ptr)
		{
			return;
		}

		MapTemplate* cur_map_template_ptr = cur_map_object_ptr->get_map_template_ptr();
		if (nullptr == cur_map_template_ptr)
		{
			return;
		}

		if (group_id > 0)
		{
			if (false == init_unit::get_map_public(cur_map_template_ptr->Type))
			{
				return;
			}
		}

		bool is_need_check_map = true;
		if (init_unit::get_map_public(map_template_ptr->Type)
			|| init_unit::get_map_public(cur_map_template_ptr->Type))
		{
			//������ͼ���Դ����κε�ͼ �κε�ͼ���Դ���������ͼ
		}
		else if (init_unit::is_goto_same_map(map_template_id, line_id, cur_map_object_ptr->get_map_template_id(), cur_map_object_ptr->get_line_id()))
		{
			// ����ͬ��ͼ����
			return;
		}
		else if (map_template_ptr->Type == e_map_type_demon_tower_fuben
			&& cur_map_template_ptr->Type == e_map_type_demon_tower_fuben)
		{
			//����ħ����������ֱ�ӽ���һ����ħ������ ��һ��
		}
		else
		{
			//���ܴ�һ������ֱ�Ӵ��͵���һ������
			return;
		}

		if (player_ref.can_entry_fuben(map_template_id, true, cell_server::getInstance().get_is_cross_sever(group_id)) == false)
		{
			return;
		}
		//����������������е�ͼ
		if (false == init_unit::is_map_cross_server(map_template_id, cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity)) && map_template_ptr->Type != e_map_type_big_map && false == init_unit::is_map_all_server(map_template_id))
		{
			if (false == player_ref.is_self_server())
			{
				return;
			}
		}

		if (player_ref.get_pawn_att().get_game_att(e_unit_game_att_war_state))
		{
			//return;
		}

		if (map_object::get_activity_map_sec_left(map_template_ptr->Type, player_ref.get_array_index()) < 0)
		{
			return;
		}

		player_ref.transfer_by_template(map_template_id, line_id, group_id, map_guid, req.war_index());
	}

	void c2cs_req_transfer_scene_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(game_proto_transfer_scene, c2cs_req_transfer_scene)
	}

	void c2cs_req_transfer_scene_born(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_transfer_scene_born)
		c2cs_req_transfer_scene_born_logic(player_ref, request);
	}

	void c2cs_req_transfer_scene_born_logic(player& player_ref, game_proto_transfer_scene_born& req)
	{
		auto map_ent = get_entity(player_ref.get_map_guid());
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("c2cs_req_transfer_scene_born_logic map_ent is nullptr map_guid:{}", player_ref.get_map_guid().server_64);
			return;
		}
		auto base_map_cp = map_ent->get_component<base_map_component>();

		if (base_map_cp->m_map_template->BornLocation.size() < 3 || base_map_cp->m_map_template->BornRotation.size() < 3)
		{
			return;
		}

		s_map_pos temp_pos;
		temp_pos.set_location(base_map_cp->m_map_template->BornLocation[0], base_map_cp->m_map_template->BornLocation[1], base_map_cp->m_map_template->BornLocation[2]);
		temp_pos.set_rotation(base_map_cp->m_map_template->BornRotation[0], base_map_cp->m_map_template->BornRotation[1], base_map_cp->m_map_template->BornRotation[2]);
		player_ref.transfer_by_template_pos(base_map_cp->m_map_id, base_map_cp->m_line_id, temp_pos, player_ref.get_map_guid());
	}

	void c2cs_req_transfer_scene_born_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(game_proto_transfer_scene_born, c2cs_req_transfer_scene_born)
	}

	void c2cs_lucky_draw_req(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_lucky_draw)

		player_ref.lucky_draw(request.fuben_template_id());
	}
	void c2cs_unlock_bag_slot(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_bag_unlock_slot_req)
		player_ref.unlock_bag_slot(request.bag_slot_index_want_to_unlock(), request.storage_slot_index_want_to_unlock());
	}

	void c2cs_get_first_raid_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_get_first_raid_award)
		int32 map_id = request.map_temp_id();

		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}
		std::vector<int32>* award_item_arr = &(map_template_ptr->FirstWinAwardItems);
		std::vector<int32>* award_item_num_arr = &(map_template_ptr->FirstWinAwardItemsNum);

		std::vector<s_item_template_info> msg_info_array;
		msg_info_array.reserve(30);
		if (award_item_arr->size() == award_item_num_arr->size())
		{
			int32 arr_len = award_item_arr->size();
			if (arr_len > 0)
			{
				item_set& temp_item_set = player_ref.get_item_set();
				for (int32 i = 0; i < arr_len; ++i)
				{
					int32 award_template_id = (*award_item_arr)[i];
					int32 award_num = (int32)((*award_item_num_arr)[i]);
					citem* award_item = temp_item_set.create_item_by_template(e_server_log_add_item_raid_reward, map_id, award_template_id, award_num, 1);
					if (nullptr == award_item)
					{
						return;
					}
					msg_info_array.push_back({ award_template_id,award_num,1 });

					std::vector<citem*> item_list;
					item_list.push_back(award_item);
					if (temp_item_set.can_put_items(item_list))
					{
						temp_item_set.put_in_bag(item_list);
					}
					else
					{
						std::string title = "";
						title += template_manager::get_instance().get_str_by_string_template_id(map_template_ptr->MapName);
						title += template_manager::get_instance().get_str_by_string_template_id(90090511);
						player_ref.get_mail_mgr().send_mail_to_another_player_by_system(player_ref.get_unit_guid(), player_ref.get_unit_info(e_role_info_server_id), title, "", 0, 0, 0, 0, item_list);
						return;
					}
					player_ref.get_item_set().get_item_send_promp_msg_to_client(msg_info_array);
				}
			}
		}
	}

	void c2cs_auto_use_hp_pot(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_auto_use_hp_medicine)
		player_ref.set_auto_use_hp_pot(request.health_percent(), request.is_auto_buy_medicine());
	}
	void c2cs_ask_unit_dead(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_ask_unit_dead)
		c2cs_ask_unit_dead_logic(player_ref, request);
	}
	void c2cs_ask_unit_dead_logic(player& player_ref, character_proto_ask_unit_dead& req)
	{
		character_proto_unit_dead msg;
		msg.set_unit_guid(req.unit_guid());
		msg.set_is_dead(true);
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(player_ref.get_map_guid(), player_ref.get_new_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_unit_dead);
			return;
		}
		bool be_have = false;
		unit_index_map_const_it it_end = aoi_tower_watch.end();
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != it_end; ++it)
		{
			if (req.runtime_id() == it->first)
			{
				unit& unit_ref = unit_man::get_unit(it->first);
				if (unit_ref.get_unit_guid().server_64 != req.unit_guid())
				{
					player_ref.send_message_to_self(&msg, e_msgindex_s2c_unit_dead);
					return;
				}
				else
				{
					be_have = true;
				}
			}
		}
		if (be_have == false)
		{
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_unit_dead);
		}
	}
	void c2cs_ask_unit_dead_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(character_proto_ask_unit_dead, c2cs_ask_unit_dead)
	}
	void c2cs_unit_fuhuo(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_character_fuhuo)
		e_revive_type revive_mode = (e_revive_type)request.mode();

		player_ref.back_to_life(revive_mode);
	}


	void c2s_player_gmorder_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (SERVERCONFIG->gm_state == false)
		{
			CONSOLE_ERROR("GM is Close");
			return;
		}

		UNPACKING_PROTO(character_proto_character_gmorder_message)

		auto command_string = request.command_string();
		auto command_type = request.command_type();
		auto sub_command = request.sub_command_type();
		std::vector<int32> arg_int;
		for (int32 i = 0; i < request.arg_int().size(); i++)
		{
			arg_int.push_back(request.arg_int(i));
		}
		std::vector<std::string> arg_string;
		for (int32 i = 0; i < request.arg_string().size(); i++)
		{
			arg_string.push_back(request.arg_string(i));
		}
		do_gm_order_logic(player_ref, command_string, arg_int, arg_string);
		
		player_ref.send_message_to_self(&request, e_msgindex_s2c_character_gmorder_message);
	}

	void c2s_player_pick_drop_box(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_pick_drop_box)

		guid_64 drop_box_guid(request.drop_box_guid());
		s_unit_identifier unit_identifier(drop_box_guid, request.runtime_id());
		player_ref.on_req_pick_drop_box(unit_identifier);
	}
	void c2cs_mopping_up(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_mopping_up)

		//���
		if (false == player_ref.is_self_server())
		{
			return;
		}
		int32 raid_template_id = request.raid_template_id();
		int32 mopping_up_count = request.mopping_up_count();

		bool auto_convert_equip_to_magic_crystal = request.auto_convert_equip_to_magic_crystal() > 0 ? true : false;
	}

	void c2cs_mopping_up_demons_tower(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_mopping_up_demons_tower)

		//���
		if (false == player_ref.is_self_server())
		{
			return;
		}
	}

	void c2cs_mopping_up_demons_tower_get_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_mopping_up_demons_tower_get_award)
		//���
		if (false == player_ref.is_self_server())
		{
			return;
		}
		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(player_ref.get_array_index());
		//if (map_record_set_ref.is_valid() == false)
		//{
		//	return;
		//}
		//map_record_set_ref.mopping_up_demons_tower_get_award(req.is_resolve());
	}

	void c2cs_all_mopping_up(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_all_mopping_up)
		//���
		if (false == player_ref.is_self_server())
		{
			return;
		}
		std::vector<int32> raid_template_id_list;
		raid_template_id_list.clear();
		std::vector<int32> mopping_num_list;
		mopping_num_list.clear();

		for (int32 i = 0; i < request.raid_template_id_size(); ++i)
		{
			raid_template_id_list.push_back(request.raid_template_id(i));
		}

		for (int32 i = 0; i < request.mopping_up_count_size(); ++i)
		{
			mopping_num_list.push_back(request.mopping_up_count(i));
		}
		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(player_ref.get_array_index());
		//if (map_record_set_ref.is_valid() == false)
		//{
		//	return;
		//}
		//map_record_set_ref.mopping_up_all(raid_template_id_list, mopping_num_list);
	}
	void c2cs_spirit_fetter_upgrade(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_c2s_sprite_fetter_upgrade)
		//���
		if (false == player_ref.is_self_server())
		{
			return;
		}
		//auto res = player_ref.get_spirit_mgr().upgrade_spirit_fetter(request.sprite_fetter_id());
		//item_s2c_sprite_fetter_upgrade msg;
		//msg.set_res(res);
		//msg.set_sprite_fetter_id(request.sprite_fetter_id());
		//player_ref.send_message_to_self(&msg, e_msgindex_s2c_spirit_fetter_upgrade);
	}

	void c2cs_get_func_unlock_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_get_func_unlock_award)

		int32 func_unlock_id = request.func_unlock_template_id();
		if (func_unlock_id <= 0)
		{
			return;
		}
		player_ref.get_func_unlock_award(func_unlock_id);
	}

	void c2cs_leave_map(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_leave_map)

		player_ref.transfer_by_template(player_ref.get_unit_info(e_role_info_main_map_id), player_ref.get_main_line_id(), 0, guid_64(), 0);
	}

	void c2cs_req_hit_and_get_npc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_req_hit_and_get)
		guid_64 target_npc_guid;
		target_npc_guid.server_64 = request.npc_guid();
		player_ref.req_hit_and_get_npc_proc(target_npc_guid);
	}
	void c2cs_req_att_game_change(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_character_one_att_change)

		int32 change_index = request.attack_att_index();
		float change_value = request.attack_att_value();
		if (change_index < e_unit_attack_att_max)
		{//�����޸Ļ�������
			//player_ref.get_pawn_att().set_state_att(change_index, change_value);
		}
		else if (change_index < e_unit_attack_att_max + e_unit_game_att_max)
		{
			if ((change_index - e_unit_attack_att_max == e_unit_game_att_fight_lock) || (change_index - e_unit_attack_att_max == e_unit_game_att_jump) || (change_index - e_unit_attack_att_max == e_unit_game_att_interaction))
			{
				player_ref.get_pawn_att().set_game_att(change_index - e_unit_attack_att_max, change_value, true);
			}
		}
		else
		{//״̬�����޸�
			//player_ref.get_pawn_att().set_state_att(change_index - e_unit_attack_att_max - e_unit_game_att_max, change_value);
		}
	}

	void c2cs_grade_level_ani_finish_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_grade_up_level_ani_finish)


		player_ref.grade_up_level_ani_end(request.old_equip_buff_id(), request.new_equip_buff_id());
	}

	void c2cs_req_base_body_att(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_unit_info_body_att)
		c2cs_req_base_body_att_logic(player_ref, request);
	}

	void c2cs_req_base_body_att_logic(player& player_ref, character_proto_unit_info_body_att& req)
	{
		if (req.role_base_body_att_size() != 4)
		{
			return;
		}
		std::vector<int32> base_body_att_arr;
		for (int32 i = 0; i < req.role_base_body_att_size(); i++)
		{
			int32 value = req.role_base_body_att(i);
			if (value < 0)
			{
				value = 0;
			}
			base_body_att_arr.push_back(value);
		}
		player_ref.get_pawn_att().change_base_body_att(base_body_att_arr);

	}

	void c2cs_req_base_body_att_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(character_proto_unit_info_body_att, c2cs_req_base_body_att);
	}

	void c2cs_add_damage_buff_in_legion_boss(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_add_exp_raid_buff)

		//auto legion_map_cs_ptr = world_cs::get_map_by_guid<legion_station_map_cs>(player_ref.get_map_guid());
		//if (nullptr == legion_map_cs_ptr)
		//{
		//	return;
		//}

		//int32 add_time_all = legion_map_cs_ptr->add_legion_boss_damage(player_ref.get_unit_guid(), request.add_count());
		//if (add_time_all < 0)
		//{
		//	return;
		//}
		//int32 demon_count = legion_map_cs_ptr->get_player_demon_count(player_ref.get_unit_guid());
		//int32 gold_count = legion_map_cs_ptr->get_player_gold_count(player_ref.get_unit_guid());
		//game_proto_add_legion_boss_buff_end end_msg;
		//end_msg.set_add_count(add_time_all);
		//end_msg.set_demon_count(demon_count);
		//end_msg.set_gold_count(gold_count);
		//player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_add_legion_boss_damage_end);
	}

	void c2cs_add_damage_buff_in_exp_raid(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_add_exp_raid_buff)

		//auto map_object_ptr = world_cs::get_map_by_guid<raid_map_cs>(player_ref.get_map_guid());
		//if (nullptr == map_object_ptr)
		//{
		//	return;
		//}
		//int32 damage_buff_count = map_object_ptr->add_exp_raid_attack_buff(player_ref, request.add_count());
		//if (damage_buff_count < 0)
		//{
		//	return;
		//}
		//game_proto_add_exp_raid_buff_end end_msg;
		//end_msg.set_add_count(damage_buff_count);
		//player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_add_damage_buff_in_expraid_end);
	}

	void c2cs_buy_raid_count(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_buy_raid_count)

		int32 map_temp_id = request.map_temp_id();
		int32 buy_count = request.buy_count();
		bool is_success_buy_count = player_ref.buy_count_with_map_id(map_temp_id, buy_count);
		if (false == is_success_buy_count)
		{
			return;
		}
		game_proto_buy_raid_count_end end_msg;
		end_msg.set_map_temp_id(map_temp_id);
		end_msg.set_buy_count(buy_count);
		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_buy_raid_count_end);
	}

	void c2cs_change_role_class(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_change_role_class)

		if (player_ref.change_role_class(request.class_change()))
		{
			character_proto_change_role_class_end end_msg;
			end_msg.set_result(1);
			player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_change_role_class_end);
		}
	}

	//******************************************************** ��� *************************************************************************************//

	void c2cs_create_team(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		team_cs_mgr::c2cs_create_team(connindex, data_ptr, data_len);
	}

	void c2cs_join_team(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		team_cs_mgr::c2cs_join_team(connindex, data_ptr, data_len);
	}

	void c2cs_change_team_type(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		team_cs_mgr::c2cs_change_team_type(connindex, data_ptr, data_len);
	}

	//******************************************************** item *************************************************************************************//

	void item_operate_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_item_operation)

		item_system::item_operate_message(&player_ref, request);
	}
	void item_element_operate_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(element_heart_proto_element_heart_operate)

		item_element_system::element_operate_message(&player_ref, request);
	}
	void item_composit_operate_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_item_composit)

		item_composit_system::composit_operate_message(&player_ref, request);
	}

	void show_fashion(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_show_fashion)

		int32 is_show_fashion = request.is_show_fashion();
		if (is_show_fashion < 0 || is_show_fashion > 1)
		{
			player_ref.set_unit_info(e_role_info_show_fashion, 1);
		}
		else
		{
			player_ref.set_unit_info(e_role_info_show_fashion, request.is_show_fashion());
		}
		item_set& item_ref = player_ref.get_item_set();
		item_ref.equip_off_equip_show_buff();	//ȥװ����Чbuff
		player_ref.send_info_one(e_role_info_show_fashion, true);

		item_proto_show_fashion show_request_end;
		show_request_end.set_is_show_fashion(player_ref.get_unit_info(e_role_info_show_fashion));
		show_request_end.add_role_guid(player_ref.get_unit_guid().A);
		show_request_end.add_role_guid(player_ref.get_unit_guid().B);
		//player_ref.send_message_to_self(&show_request_end, e_msgindex_s2c_show_fashion);
		player_ref.send_message_to_aoi(&show_request_end, e_msgindex_s2c_show_fashion, true);

		item_ref.set_show_fashion_buff();
		item_ref.equip_on_equip_show_buff();

	}

	void star_skill_unlock(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_unlock_skill)
		guid_64 temp_guid;
		temp_guid.server_64 = request.choosed_guid();
		int32 choosed_index = request.choosed_index();

		int32 result = player_ref.get_item_set().star_skill_unlock(temp_guid, choosed_index);

		item_proto_unlock_skill_end unlock_end_msg;
		unlock_end_msg.set_result(result);
		player_ref.send_message_to_self(&unlock_end_msg, e_msgindex_s2c_unlock_skill_end);
	}
	void c2cs_worship_top_player_proc(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(ranking_proto_worship_player)
		guid_64 temp_guid;
		temp_guid.server_64 = request.target_guid();
		player_ref.get_worship_target().req_worship_player(e_RankingIndex(request.ranking_index()), request.worship_type(), temp_guid);
	}

	void c2cs_service_rank_receice_reward_proc(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(ranking_proto_service_rank_recevice_reward)

		player_ref.get_service_rank_mgr().receive_reward_by_type(request.service_rank_type());
	}

	//******************************************************** store *************************************************************************************//

	void goods_operate_message(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(goods_proto_goods_operate)

		player_ref.buy_goods(request.goods_id(), request.goods_num(), request.store_id(), request.is_auto_buy());
	}

	//******************************************************** skill *************************************************************************************//
	void skill_spell_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_skill_spell)

		guid_64 target_guid(request.target_id());
		s_unit_identifier target_unit_idf(target_guid, request.target_runtime_id());
		use_skill_param param;
		param.sender_unit_idf = player_ref.get_identifier();
		param.target_unit_idf = target_unit_idf;
		param.skill_template_id = request.skill_template_id();
		param.pos = player_ref.get_new_map_pos().unit_location;
		param.yaw = request.yaw();
		param.skill_order = request.skill_order();
		skill_manager::skill_spell(param);
	}
	void skill_cast_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_skill_cast)

		guid_64 target_guid(request.target_id());
		s_unit_identifier target_unit_idf(target_guid, request.target_runtime_id());
		use_skill_param param;
		param.sender_unit_idf = player_ref.get_identifier();
		param.target_unit_idf = target_unit_idf;
		param.skill_template_id = request.skill_template_id();
		param.pos = player_ref.get_new_map_pos().unit_location;
		param.yaw = request.yaw();
		param.skill_order = request.skill_order();
		skill_manager::skill_cast(param);
	}
	void skill_hurt_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_skill_hurt)

		if (request.postion_size() < 3)
		{
			return;
		}
		guid_64 target_guid(request.target_id());
		s_unit_identifier target_unit_idf(target_guid, request.target_runtime_id());
		use_skill_param param;
		param.sender_unit_idf = player_ref.get_identifier();
		param.target_unit_idf = target_unit_idf;
		param.skill_template_id = request.skill_template_id();
		param.effect_index = request.effect_index();
		param.skill_order = request.skill_order();
		param.be_hit = request.be_hit();
		param.be_critical = request.be_critical();
		param.hit_random = request.hit_random();
		param.pos.x = request.postion(0);
		param.pos.y = request.postion(1);
		param.pos.z = request.postion(2);
		skill_manager::skill_hurt(param);
	}
	void skill_req_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_skill_operation)

		skill_req_message_logic(player_ref, request);
	}
	void skill_req_message_logic(player& player_ref, skill_proto_skill_operation& request)
	{
		skill_manager::distribute_msg(player_ref.get_identifier(), request, 0);
	}
	void skill_req_message_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(skill_proto_skill_operation, skill_req_message)
	}
	void skill_use_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_change_skill_index)
		unit& unit_ref = unit_man::get_unit(player_ref.get_array_index());
		if (unit_ref.is_valid() == false)
		{
			return;
		}
		skill_manager::skill_change_msg(player_ref.get_array_index(), request.skill_template_id(), request.pos_index());
	}
	void passive_skill_up_leve(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_passive_skill_up_level)

		player_ref.get_passive_skill().passive_skill_up_level(request.passive_skill_id());

		skill_proto_passive_skill_up_level_end end_msg;
		end_msg.set_res(0);
		end_msg.set_passive_skill_id(request.passive_skill_id());
		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_passive_skill_up_leve);
	}
	void c2cs_passive_skill_equip(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skill_proto_passive_skill_equip)

		auto res_id = player_ref.get_passive_skill().passive_skill_equip(request.passive_skill_id(), request.slot());
		skill_proto_passive_skill_equip_end end_msg;
		end_msg.set_res(0);
		end_msg.set_old_passive_skill_id(res_id);
		end_msg.set_new_passive_skill_id(request.passive_skill_id());
		player_ref.send_message_to_self(&end_msg, e_msgindex_s2c_passive_skill_equip);
	}

	void buff_req_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(buff_proto_buff_operation)

		buff_req_message_logic(player_ref, request);
	}

	void buff_req_message_logic(player& player_ref, buff_proto_buff_operation& req)
	{
		buff_man::handle_msg(player_ref.get_array_index(), req);
	}

	void buff_req_message_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(buff_proto_buff_operation, buff_req_message)
	}
	void c2cs_change_pk_mode(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_pk_mode_change)

		player_ref.get_pk_community_mgr().change_pk_info(request.pk_info_type(), request.pk_info_value());
	}

	void c2cs_start_meditation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_start_meditation)

		player_ref.get_meditation_mgr().start_mediation(true);
	}

	void c2cs_stop_meditation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_stop_meditation)

		player_ref.get_meditation_mgr().stop_mediation();
	}
	void c2cs_talent_level_up(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(talent_proto_level_up)

		player_ref.get_talent_mgr().talent_level_up(request.talent_template_id(), request.talent_list_template_id(), request.layer_index());
	}
	void c2cs_talent_reset(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(talent_proto_reset_money)

		player_ref.get_talent_mgr().talent_reset();
	}

	void c2cs_convenient_join_legion(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		legion_cs_mgr::c2cs_convenient_join_legion(connindex, data_ptr, data_len);
	}

	void c2cs_draw_mem_daily_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		legion_cs_mgr::c2cs_draw_mem_daily_award_fun(connindex, data_ptr, data_len);
	}

	void c2cs_territory_buff_operate(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		legion_cs_mgr::c2cs_legion_territory_buff_operate(connindex, data_ptr, data_len);
	}

	void c2cs_goto_next_stage_of_bonfire_map(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		legion_cs_mgr::c2cs_goto_next_stage_of_bonfire_map(connindex, data_ptr, data_len);
	}

	void c2cs_req_apply_to_join_legion(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(legion_proto_apply_to_join_legion)

		//���״̬�²�����
		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (false == player_ref.get_func_unlock_mgr().is_func_unlock("Legion"))
		{
			player_ref.send_notice("90202450");//δ����
			return;
		}
		cs2ws_apply_to_join_legion msg;
		msg.client_uid = player_ref.get_client_uid();
		msg.legion_guid = request.legion_guid();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
	}

	void c2cs_invite_player_to_join_legion(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(legion_proto_invite_player)

		//���״̬�²�����
		if (false == player_ref.is_self_server())
		{
			return;
		}
		guid_64 invite_guid = guid_64();

		player* invite_player_ptr = nullptr;
		if (request.has_invite_guid())
		{
			invite_guid = request.invite_guid();
			if (false == invite_guid.is_valid())
			{
				return;
			}
			invite_player_ptr = &unit_man::get_player(invite_guid);
		}
		else if (request.has_new_member_name())
		{
			const std::string new_member_name = request.new_member_name();
			int32 name_len = new_member_name.length();
			if (name_len <= 0 || name_len > max_name_size)
			{
				return;
			}
			invite_player_ptr = &unit_man::get_player(new_member_name);
		}
		if (nullptr == invite_player_ptr)
		{
			return;
		}
		if (false == invite_player_ptr->get_func_unlock_mgr().is_func_unlock("Legion"))
		{
			player_ref.send_notice("90095314");//��������δ����
			return;
		}
		cs2ws_invite_to_join_legion msg;
		msg.client_uid = player_ref.get_client_uid();
		msg.invite_client_uid = invite_player_ptr->get_client_uid();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
	}

	void c2cs_legion_warehouse_operation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(legion_proto_operation_legion_warehouse_item)

		player_ref.send_notice("90096217");
		return;//���ε����Ųֿ⹦��

		//�ظ���Ϣ��ǰ��
		int32 error_id = 0;
		if (request.operation_type() == e_legion_warehouse_operation_store) {
			//����
			error_id = legion_cs_mgr::c2cs_store_legion_warehouse_item(connindex, data_ptr, data_len);
		}
		else if (request.operation_type() == e_legion_warehouse_operation_out) {
			//���
			error_id = legion_cs_mgr::c2cs_role_get_legion_warehouse_item(connindex, data_ptr, data_len);
		}
		else if (request.operation_type() == e_legion_warehouse_operation_destroy) {
			//����
			error_id = legion_cs_mgr::c2cs_destroy_legion_warehouse_item(connindex, data_ptr, data_len);
		}
		if (error_id > 0) {
			// ���ڴ��� ������ŷ���ǰ�� �ɹ��Ļ� �����ӿڻ᷵��
			legion_proto_operation_legion_warehouse_item_end resp;
			resp.set_error_code(error_id);
			player_ref.send_message_to_self(&resp, e_msgindex_s2c_operation_legion_warehouse_item_end);
		}
	}

	void c2cs_req_treasure_notice(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_treasure_notice)
		player_ref.get_treasure_mgr().send_treasure_notice(request.item_id(), request.item_quality(), request.operate_type());
	}

	void c2s_add_guide_record(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		character_proto_add_guide_record add_guide_record_msg;

		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		player& player_ref = unit_man::get_player(client_uid.fepsession_uid);
		if (false == player_ref.is_valid() || player_ref.get_client_uid().rand_num_1 != client_uid.rand_num_1 || (player_ref.get_session_state() != e_session_status_in_gaming && player_ref.get_session_state() != e_session_status_client_loading))
		{
			return;
		}

		c2s_add_guide_record_logic(player_ref, add_guide_record_msg);
	}

	void c2s_add_guide_record_logic(player& player_ref, character_proto_add_guide_record& req)
	{
		s_record_info record_info;
		record_info.guide_trigger_id = req.guide_trigger_id();
		record_info.state = req.state();
		player_ref.get_guide_mgr().save_guide_record(record_info);

		if (req.guide_trigger_id() == first_exp_fuben_guide_trigger_id)
		{
			player_ref.m_is_begin_exp_raid = true;
		}
	}

	void c2s_add_guide_record_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(character_proto_add_guide_record, c2s_add_guide_record)
	}

	void c2s_guide_trigger_start(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		character_proto_guide_trigger_start guide_trigger_start_msg;
		bool suc = parse_msg::getInstance().parse_message_new(&guide_trigger_start_msg, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		player& player_ref = unit_man::get_player(client_uid.fepsession_uid);
		if (false == player_ref.is_valid() || player_ref.get_client_uid().rand_num_1 != client_uid.rand_num_1 || (player_ref.get_session_state() != e_session_status_in_gaming && player_ref.get_session_state() != e_session_status_client_loading))
		{
			return;
		}
		player_ref.get_guide_mgr().guide_trigger_start_proc(guide_trigger_start_msg.guide_trigger_id());
	}

	//******************************************************** arena *************************************************************************************//
	void req_get_arena_rank(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_get_arena_rank)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_arena_mgr().get_arena_rank();
	}
	void req_get_match_player(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_get_match_player)
		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_arena_mgr().req_get_match_player();
	}
	void req_get_first_three_player(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_get_top_three_player)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_arena_mgr().req_get_first_three_player();
	}
	void req_do_challenge(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_req_challenge)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (request.target_guid_size() != guid_size)
		{
			return;
		}


		MapTemplate* cur_map_template_ptr = player_ref.m_cur_map_template_ptr;
		if (nullptr == cur_map_template_ptr)
		{
			return;
		}

		if (cur_map_template_ptr->Type != e_map_type_big_map)
		{
			return;//���ڴ����粻����JJC
		}

		guid_64 temp_guid;
		temp_guid.A = request.target_guid(0);
		temp_guid.B = request.target_guid(1);
		player_ref.get_arena_mgr().req_challenge(temp_guid, request.is_cost_money(), request.target_cur_pos(), request.self_cur_pos());
	}

	void get_challenge_log(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_req_get_challenge_log)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_arena_mgr().get_challenge_logs();
	}

	void get_arena_reward(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_get_arena_reward)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_arena_mgr().get_reward();
	}

	void mopping_arena_remain_times(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(arena_proto_get_arena_reward)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_arena_mgr().mopping_remain_times();
	}

	void c2s_operate_mail(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(mail_proto_mail_operation)

		c2s_operate_mail_logic(player_ref, request);
	}

	void c2s_operate_mail_logic(player& player_ref, mail_proto_mail_operation& request)
	{
		if (request.mail_guid_size() <= 0 || request.mail_guid_size() % 2 != 0)
		{
			return;
		}
		std::vector<guid_64> temp_mail_vec;
		for (int32 i = 0; i < request.mail_guid_size(); i += 2)
		{
			guid_64 mail_guid;
			mail_guid.A = request.mail_guid(0 + i);
			mail_guid.B = request.mail_guid(1 + i);
			temp_mail_vec.push_back(mail_guid);
		}
		player_ref.get_mail_mgr().mail_operate(temp_mail_vec, request.operation_typ());
	}

	void c2s_operate_mail_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(mail_proto_mail_operation, c2s_operate_mail)
	}


	void c2s_send_mail(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(mail_proto_mail_send_mail)

		std::vector<guid_64> temp_item_guid_array;
		guid_64 temp_addressee_guid;
		int32 temp_money_typ1 = 0;
		int32 temp_money_num1 = 0;
		int32 temp_money_typ2 = 0;
		int32 temp_money_num2 = 0;
		std::string temp_mail_title;

		std::string temp_mail_text;
		if (request.addressee_guid_size() != guid_size)
		{
			return;
		}


		if ((request.mail_item_guid_size() > 1) && (request.mail_item_guid_size() % guid_size == 0))
		{
			guid_64 temp_guid;
			for (int32 i = 0; i < request.mail_item_guid_size(); i += guid_size)
			{
				if (temp_item_guid_array.size() >= max_item_per_mail)
				{
					break;
				}
				temp_guid = guid_64(request.mail_item_guid(i), request.mail_item_guid(i + 1));
				temp_item_guid_array.push_back(temp_guid);
			}

		}
		temp_addressee_guid.A = request.addressee_guid(0);
		temp_addressee_guid.B = request.addressee_guid(1);

		temp_money_typ1 = request.monye_typ_1();
		temp_money_num1 = request.monye_value_1();
		temp_money_typ2 = request.monye_typ_2();
		temp_money_num2 = request.monye_value_2();

		temp_mail_title = request.mail_title();
		temp_mail_text = request.text();



		player_ref.get_mail_mgr().send_mail_to_another_player(temp_addressee_guid, 0,
			temp_mail_title, temp_mail_text,
			temp_money_typ1, temp_money_num1, temp_money_typ2, temp_money_num2, temp_item_guid_array);

	}

	void c2s_send_mail_to_all_player(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(mail_proto_mail_send_mail_to_all_player)

		std::vector<guid_64> temp_item_guid_array;
		int32 temp_money_typ1 = 0;
		int32 temp_money_num1 = 0;
		int32 temp_money_typ2 = 0;
		int32 temp_money_num2 = 0;
		std::string temp_mail_title;

		std::string temp_mail_text;


		if ((request.mail_item_guid_size() > 1) && (request.mail_item_guid_size() % 2 == 0))
		{
			guid_64 temp_guid;
			for (int32 i = 0; i < request.mail_item_guid_size(); i += 2)
			{
				if (temp_item_guid_array.size() >= max_item_per_mail)
				{
					break;
				}
				temp_guid = guid_64(request.mail_item_guid(i), request.mail_item_guid(i + 1));
				temp_item_guid_array.push_back(temp_guid);
			}
		}

		temp_money_typ1 = request.monye_typ_1();
		temp_money_num1 = request.monye_value_1();
		temp_money_typ2 = request.monye_typ_2();
		temp_money_num2 = request.monye_value_2();

		temp_mail_title = request.mail_title();
		temp_mail_text = request.text();

		//player_ref.get_mail_mgr().send_mail_to_all_player(temp_mail_title, temp_mail_text, 1, 1, 1, 1, temp_item_guid_array);

		player_ref.get_mail_mgr().send_mail_to_all_player(temp_mail_title, temp_mail_text, temp_money_typ1, temp_money_num1, temp_money_typ2, temp_money_num2, temp_item_guid_array);

	}

	void c2s_send_notice(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(chat_proto_notice_info)

		if (request.role_guid_size() != 2)
		{
			return;
		}
		player_ref.get_chat_mgr().send_notice(request.notice_id(), request.final_string());
	}

	void c2s_send_notice_with_param(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(chat_proto_notice_info_with_param)
		c2s_send_notice_with_param_logic(player_ref, request);
	}

	void c2s_send_notice_with_param_logic(player& player_ref, chat_proto_notice_info_with_param& request)
	{
		std::vector<int32> param_array;
		param_array.reserve(request.param_array_size());
		for (int32 i = 0; i < request.param_array_size(); ++i)
		{
			param_array.push_back(request.param_array(i));
		}

		world_cs::send_notice_with_param(request.sender_guid(), request.sender_name(),
			request.content_text(), param_array,
			request.param_type(), request.chat_type(),
			request.sender_template_id(), request.notice_id());
	}

	void c2s_send_notice_with_param_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(chat_proto_notice_info_with_param, c2s_send_notice_with_param)
	}

	void c2s_send_chat_new(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(chat_proto_chat_content)

		if (request.addressee_guid_size() != guid_size)
		{
			return;
		}

		c2s_send_chat_new_logic(player_ref, request);
	}

	void c2s_send_chat_new_logic(player& player_ref, chat_proto_chat_content& request)
	{

		guid_64 addressee_guid(request.addressee_guid(0), request.addressee_guid(1));

		e_chat_content_info info_array[chat_max_word_num];
		int32 chat_info_num = 0;
		for (int32 i = 0; i < request.content_info_array_size(); ++i)
		{
			if (i >= chat_max_word_num)
			{
				break;
			}
			e_chat_content_info temp_info;
			int32 str_len = request.content_info_array(i).commontext().size() > chat_content_info_max ? chat_content_info_max : request.content_info_array(i).commontext().size();
			memcpy(temp_info.common_text, request.content_info_array(i).commontext().c_str(), str_len);
			//
			temp_info.template_id = request.content_info_array(i).template_id();
			//
			if (request.content_info_array(i).item_data_size() == e_item_info_max)
			{
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					temp_info.item_info[j] = request.content_info_array(i).item_data(j);
				}
			}

			if (request.content_info_array(i).location_size() == 3
				&& request.content_info_array(i).has_map_id() && request.content_info_array(i).has_line_id())
			{
				temp_info.location[0] = request.content_info_array(i).location(0);
				temp_info.location[1] = request.content_info_array(i).location(1);
				temp_info.location[2] = request.content_info_array(i).location(2);
				temp_info.map_id = request.content_info_array(i).map_id();
				temp_info.line_id = request.content_info_array(i).line_id();
			}

			int32 audio_id_len = request.content_info_array(i).audio_id().size() > chat_audio_id_max ? chat_audio_id_max : request.content_info_array(i).audio_id().size();
			memcpy(temp_info.audio_id, request.content_info_array(i).audio_id().c_str(), audio_id_len);
			temp_info.duration_time = request.content_info_array(i).audio_duration_time();
			temp_info.face_finish_img_id = request.content_info_array(i).face_finish_img_id();

			info_array[chat_info_num] = temp_info;
			chat_info_num++;
		}
		player_ref.get_chat_mgr().send_chat_new(info_array, chat_info_num, request.common_string(), request.chat_typ(), addressee_guid);
	}

	void c2s_send_chat_new_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(chat_proto_chat_content, c2s_send_chat_new)
	}

	void c2s_send_audio(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(chat_proto_audio_info)

		player_ref.send_message_to_aoi(&request, e_msgindex_s2c_receive_audio, false);
	}
	void item_set_quick_call_mount_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_item_quick_call_mount)
		item_set_quick_call_mount_proc_logic(player_ref, request);
	}

	void item_set_quick_call_mount_proc_logic(player& player_ref, item_proto_item_quick_call_mount& request)
	{
		if (request.mount_guid_size() != char_quick_call_mount_max)
		{
			return;
		}
		guid_64 mount_guid_array[char_quick_call_mount_max];
		for (int32 i = 0; i < char_quick_call_mount_max; ++i)
		{
			mount_guid_array[i] = request.mount_guid(i);
		}

		player_ref.get_item_set().set_quick_mount_call_array(mount_guid_array);
	}

	void item_set_quick_call_mount_proc_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(item_proto_item_quick_call_mount, item_set_quick_call_mount_proc)
	}

	void item_get_hope_item(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_item_hope_item_get)

		int32 get_typ = request.get_typ();
		if ((get_typ < 0) || (get_typ >= e_item_get_hope_item_typ_max))
		{
			return;
		}
		player_ref.get_item_set().get_hope_item(get_typ);
	}
	void c2s_item_upgrade(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_c2s_item_upgrade)
		item_upgrade_system::item_upgrade(&player_ref);
	}
	void req_change_sky_suit_state(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_change_sky_suit_shape)

		int32 show_state = request.operate_state();
		int32 suit_type = request.operate_type();
		if (show_state < 0 || show_state > 3)
		{
			return;
		}
		if (show_state == 1)
		{
			if (player_ref.get_item_set().can_show_sky_shape(suit_type) == false)
			{
				return;
			}
			buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), sky_equip_buff_id);
		}
		else
		{
			buff_man::del_buff_by_template_id(player_ref.get_array_index(), player_ref.get_array_index(), sky_equip_buff_id);
		}
		if (show_state == 2)
		{
			if (player_ref.get_item_set().can_show_skygod_shape(suit_type) == false)
			{
				return;
			}
			buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), skygod_equip_buff_id);
		}
		else
		{
			buff_man::del_buff_by_template_id(player_ref.get_array_index(), player_ref.get_array_index(), skygod_equip_buff_id);
		}
		if (show_state == 3)
		{
			if (player_ref.get_item_set().can_show_supreme_shape(suit_type) == false)
			{
				return;
			}
			buff_man::add_buff_inst(player_ref.get_array_index(), player_ref.get_array_index(), supreme_equip_buff_id);
		}
		else
		{
			buff_man::del_buff_by_template_id(player_ref.get_array_index(), player_ref.get_array_index(), supreme_equip_buff_id);
		}
		player_ref.set_unit_info(e_role_info_show_sky_suit_shape, show_state);
		player_ref.send_info_one(e_role_info_show_sky_suit_shape, true);
		player_ref.send_message_to_self(&request, e_msgindex_s2c_change_sky_suit_shape_end);
	}

	void c2s_find_back_must_do_resource(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_find_back_must_do_resource)

		//�����ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (request.only_find_back_several() != -1)
		{
			player_ref.find_back_serveral_resource_process(request.must_do_typ(), request.is_diamond_find(), request.only_find_back_several());
		}
		else
		{
			player_ref.find_back_must_do_resource_process(request.must_do_typ(), request.is_diamond_find());
		}

	}

	void c2s_recharge(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_recharge)

		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (cell_server::getInstance().get_gm_state() == false && !player_ref.get_is_robot())
		{
			return;
		}
		player_ref.recharge_by_template_id(request.recharge_typ(), "", 0, e_payment_method_test);
	}

	void c2s_recharge_widget_type(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_recharge_widget_type)

		if (false == player_ref.is_self_server())
		{
			return;
		}
		if (cell_server::getInstance().get_gm_state() == false && !player_ref.get_is_robot())
		{
			return;
		}
		player_ref.set_recharget_widget_type(request.is_dead_recharge());
	}
	void c2s_get_first_recharge_time(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(welfare_proto_get_first_recharge_time)
		player_ref.get_welfare_mgr().get_first_recharge_welfare_by_id();
	}

	void c2s_title_set_title_no_need_see(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(title_proto_title_set_title_no_need_see)
		player_ref.get_title_mgr().set_title_no_need_see();
	}

	void c2s_worship_by_type(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_worship)
		player_ref.worship_by_type(request.worship_type(), request.is_cost_diamond() > 0);
	}

	void c2s_find_other_unit(uint32 connidex, const void* data_ptr, size_t data_len)
	{
		//��ȡtarget_guid

		UNPACKING_PROTO(character_proto_find_other_unit)


		int32 server_id = player_ref.get_unit_info(e_role_info_server_id);
		if (false == player_ref.is_self_server())
		{
			server_id = cell_server::getInstance().get_server_id();
		}
		guid_64 target_guid(request.target_guid());

		player& other_player_ref = unit_man::get_player(target_guid);
		if (true == other_player_ref.is_valid())
		{
			server_id = other_player_ref.get_unit_info(e_role_info_server_id);
		}
		else
		{
			server_id = request.server_id();
		}
		player_ref.find_other_player_info(target_guid, server_id);
	}


	void c2s_send_red_package(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_send_red_package)

		if (request.item_guid().size() < guid_size)
		{
			return;
		}
		guid_64 item_guid(request.item_guid(0), request.item_guid(1));

		player_ref.send_red_package(request.red_package_id(), request.send_text(), item_guid);
	}

	void c2s_get_red_package(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_get_red_package)

		if (request.red_package_guid_size() != 2)
		{
			return;
		}
		guid_64 red_package_guid;
		red_package_guid.A = request.red_package_guid(0);
		red_package_guid.B = request.red_package_guid(1);
		player_ref.get_red_package(red_package_guid);
	}

	void c2s_transfer_buff(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_transfer_buff)
		player_ref.transfer_buff(request.operate_type());
	}

	void c2cs_add_first_time_do_record(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(first_time_do_proto_add_first_time_do_record)

		e_first_time_do_type do_type = (e_first_time_do_type)(request.do_type());

		first_time_do_mgr& first_time_do_mgr_ref = player_ref.get_first_time_do_mgr();
		first_time_do_mgr_ref.add_do_record(do_type);
	}

	//auction
	void c2cs_auction_operate_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(auction_proto_operate)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_auction_mgr().operate(request.operate_type(), request.item_guid(), request.item_num(), request.money_type(), request.money_num());
	}

	void c2cs_auction_req_show_list(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(auction_proto_req_auction_list)
		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_auction_mgr().req_goods_list(request.main_type(), request.sub_type(),
			request.item_color(), request.quality_level(),
			request.single_money_sort_type(), request.total_money_sort_type(),
			request.is_show_gold(), request.is_show_diamond(), request.show_page(), request.key_word()
			, request.trade_type(), request.career_type());
	}

	void c2cs_auction_req_trade_record(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(auction_proto_req_trade_record)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_auction_mgr().req_trade_record(request.trade_type(), request.page_index());
	}

	void c2cs_auction_req_another_sell_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(auction_proto_req_another_sell_info)
		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_auction_mgr().req_another_sell_info(request.sell_money_type(), request.item_template_id());
	}

	void c2cs_auction_req_self_bid_list(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(auction_proto_req_self_bid_list)

		//���״̬�½�ֹ
		if (false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_auction_mgr().req_self_bid_info(request.trade_type(), request.page_index());
	}

	void c2s_achievement_finish(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(achievement_proto_achievement_finish)
		player_ref.get_achievement_mgr().set_achievement_success(request.achievement_id(), request.achievement_state());
	}

	void c2s_achievement_all_finish(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(achievement_proto_achievement_all_finish)
		player_ref.get_achievement_mgr().set_can_receive_achievement_success();
	}

	void c2cs_add_friendliness_value(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_add_friendliness_value)
		guid_64 addreessee_guid;
		addreessee_guid.server_64 = request.addreessee_guid();
		player_ref.send_gift_add_friend_values(addreessee_guid, request.gift_id(), request.gift_count(), request.chat_message());
	}

	void c2cs_phantom_oper(int32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(phantom_proto_phantom_operate)
		int32 phantom_id = request.phantom_id();
		player_ref.get_phantom_mgr().oper_active(phantom_id);
	}

	void c2cs_chat_record_oper(int32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(chat_proto_chat_record_oper)
		int32 pos = request.pos();
		xstring record = request.record();
		player_ref.get_chat_mgr().chat_oper(pos, record);
	}
	void c2cs_recycle_invited_get_reward(int32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(recycle_proto_recycle_invited_reward)
		xstring s_guid = request.inviter_guid();

		player_ref.get_recycle_mgr().on_process_invited_reward(s_guid);
	}

	void c2s_service_goal_operate(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(service_goal_proto_service_goal_operate)
		player_ref.get_service_goal_mgr().service_goal_operate(request.operate_type(), request.service_goal_id());
	}

	void c2s_interaction_invite(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(interaction_proto_c_s_interaction_invite)
		player& be_invite_player_ref = unit_man::get_player(guid_64(request.be_invited_guid()));
		if (false == be_invite_player_ref.is_valid() || be_invite_player_ref.get_session_state() != e_session_status_in_gaming)
		{
			return;
		}
		be_invite_player_ref.get_interaction_mgr().recv_interaction_invite(player_ref.get_unit_guid(), e_interaction_type(request.interaction_type()));

		//player_ref.prepare_interaction(guid_64(request.interaction_be_invited_guid()), guid_64(request.interaction_initiator_guid()), request.interaction_be_invited_interaction_type(), request.interaction_initiator_interaction_type());
	}

	void c2s_interaction_invite_reply(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(interaction_proto_c_s_interaction_invite_reply)
		player_ref.get_interaction_mgr().recv_interaction_answer(guid_64(request.invite_guid()), e_interaction_type(request.interaction_type()), e_interaction_reply(request.reply_result()));
	}

	void c2s_interaction_invite_start(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(interaction_proto_c_s_real_start_interaction)
		if ((request.invite_guid() != player_ref.get_unit_guid().server_64) && (request.be_invited_guid() != player_ref.get_unit_guid().server_64))
		{
			return;
		}

		player& invite_ref = unit_man::get_player(guid_64(request.invite_guid()));
		if (false == invite_ref.is_valid() || invite_ref.get_session_state() != e_session_status_in_gaming)
		{
			return;
		}
		invite_ref.get_interaction_mgr().recv_real_start_interaction(guid_64(request.be_invited_guid()), e_interaction_type(request.interaction_type()));

		//player_ref.start_interaction(guid_64(request.interaction_be_invited_guid()), guid_64(request.interaction_initiator_guid()), request.interaction_be_invited_interaction_type(), request.interaction_initiator_interaction_type());
	}

	void c2s_stop_interaction(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(interaction_proto_c_s_stop_interaction)
		player_ref.get_interaction_mgr().stop_interaction();
	}

	void c2s_first_look_legion(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_jump_raid_animation)
		player_ref.set_logic_data(e_role_logic_info_first_look_legion, 1);
		player_ref.send_logic_one(e_role_logic_info_first_look_legion);
	}

	void c2s_give_server_item_to_npc_func(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_give_server_item_msg)
		if (!player_ref.get_server_harry_mgr().is_have_item())
		{
			return;
		}
		player_ref.get_server_harry_mgr().give_server_item_to_npc();
		player_ref.change_harry_buff();
	}

	void c2s_buy_time_limit_item(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_limit_activity_proto_buy_time_limit_item)
		player_ref.get_time_limit_activity_mgr().buy_time_limit_item(request.activity_type(), request.buy_num());
	}

	void c2s_buy_diamond_shop(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_limit_activity_proto_buy_diamond_shop)

		time_limit_activity_object& activity_obj_ptr = player_ref.get_time_limit_activity_mgr().get_new_act_obj_by_act_type(e_time_limit_activity_type_diamond_shop);

		if (request.buy_index() >= 0)
		{
			activity_obj_ptr.buy_diamond_shop_by_index(request.buy_index());
		}
		else
		{
			activity_obj_ptr.refresh_diamond_shop(2);
		}
	}

	void c2s_star_trip_operate(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_limit_activity_proto_star_trip_operate)

		player_ref.get_star_trip_mgr().activity_operate(request.operate_type(), request.parame1(), request.parame2());
	}

	void c2cs_time_limit_get_act_reward(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_limit_activity_proto_time_limit_activity_send_get_award)

		const s_time_limit_activity_branch_temp& reward_template_ptr = time_limit_activity_temp_mgr::get_instance().get_time_limit_act_branch_with_id(request.activity_id());
		if (reward_template_ptr.ActGuid <= 0)
		{
			return;
		}
		if (request.act_guid() <= 0)
		{
			return;
		}
		//С���ֵ����ʾ֮·�ڷ�����ֱ�ӻ�ý��� ����Ҫ�ӿͻ���������
		if (e_time_limit_activity_type_small_charge == reward_template_ptr.ActivityType
			|| e_time_limit_activity_type_behavior_welfare == reward_template_ptr.ActivityType
			|| e_time_limit_activity_type_behavior_welfare1 == reward_template_ptr.ActivityType
			|| e_time_limit_activity_type_behavior_welfare2 == reward_template_ptr.ActivityType
			|| e_time_limit_activity_type_behavior_welfare3 == reward_template_ptr.ActivityType
			|| e_time_limit_activity_type_daily_gift_charge == reward_template_ptr.ActivityType
			|| e_time_limit_activity_type_new_behavior_welfare == reward_template_ptr.ActivityType)
		{
			return;
		}

		player_ref.get_time_limit_activity_mgr().apply_award(request.activity_id(), request.act_guid(), request.param_int_1());
	}

	void c2cs_time_limit_add_schedule(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_limit_activity_proto_add_activity_schedule)

		//���������ʱֻ���ڼ�¼��ҵ��ת��
		if (e_time_limit_activity_type_treasure == request.add_type()
			|| e_time_limit_activity_type_treasure_royal1 == request.add_type()
			|| e_time_limit_activity_type_treasure_royal2 == request.add_type()
			|| e_time_limit_activity_type_treasure_royal3 == request.add_type()
			|| e_time_limit_activity_type_treasure_sprite == request.add_type()
			|| e_time_limit_activity_type_treasure_element == request.add_type())
		{
			player_ref.get_time_limit_activity_mgr().add_activity_schedule((e_time_limit_activity_type)request.add_type(), request.add_value(), request.schedule_index());
		}
	}

	void c2s_cross_ladder_req_buy_ticket(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_request_buy_join_ticket)

		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		int32 region_code = region_template_ptr->RegionCode;
		e_cross_ladder_buy_ticket_ret ret;
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == act_cfg_ptr)
		{
			ret = e_cross_ladder_buy_ticket_failed;
		}
		else
		{
			int32 cost_money = -1;
			int32 cost_money_type = -1;
			if (act_cfg_ptr->ParamIntArr5.size() <= 0 || act_cfg_ptr->ParamIntArr5.size() % 3 != 0)
			{
				ret = e_cross_ladder_buy_ticket_failed;
			}
			else
			{
				for (int32 i = 0; i < act_cfg_ptr->ParamIntArr5.size() / 3; i++)
				{
					if (act_cfg_ptr->ParamIntArr5[i * 3] == region_code)
					{
						cost_money_type = act_cfg_ptr->ParamIntArr5[i * 3 + 1];
						cost_money = act_cfg_ptr->ParamIntArr5[i * 3 + 2];
						break;
					}
				}


				if (false == player_ref.can_cut_money((e_money_type)cost_money_type, cost_money))
				{
					ret = e_cross_ladder_buy_ticket_jewl;
				}
				else if (player_ref.get_cross_ladder_can_buy_ticket_num() <= 0)
				{

					ret = e_cross_ladder_buy_ticket_limit;
				}
				else
				{
					player_ref.cut_money((e_money_type)cost_money_type, cost_money, e_server_log_cut_money_cross_ladder_buy_ticket);
					cs2ws_cross_ladder_req_buy_ticket msg;
					msg.role_guid = player_ref.get_unit_guid();
					connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
					return;
				}
			}
		}
		cross_ladder_respond_buy_join_ticket send;
		send.set_result(ret);
		player_ref.send_message(&send, e_msgindex_s2c_cross_ladder_respond_buy_join_ticket);
	}

	void c2s_cross_ladder_req_mission_reward(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_req_mission_reward)

		if (request.index() < 0)
			return;
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == act_cfg_ptr || act_cfg_ptr->ParamIntArr4.size() % 6 != 0)
			return;
		int32 max_mission_num = act_cfg_ptr->ParamIntArr4.size() / 6;
		if (request.index() >= max_mission_num)
			return;
		int32 mission_type = act_cfg_ptr->ParamIntArr4[request.index() * 6];
		int32 mission_condition = act_cfg_ptr->ParamIntArr4[request.index() * 6 + 1];
		int32 mission_reward = act_cfg_ptr->ParamIntArr4[request.index() * 6 + 2];
		int32 mission_reward_num = act_cfg_ptr->ParamIntArr4[request.index() * 6 + 3];

		int32 my_cur_num = 0;
		int32 reward_tag = player_ref.get_logic_data(e_role_logic_info_cross_ladder_reward_tag);
		if (mission_type == e_cross_ladder_mission_join_num)
			my_cur_num = player_ref.get_logic_data(e_role_logic_info_cross_ladder_join_num);
		else
			my_cur_num = player_ref.get_logic_data(e_role_logic_info_cross_ladder_win_num);

		cross_ladder_ret_mission_reward ret;
		ret.set_index(request.index());
		if (my_cur_num >= mission_condition && (reward_tag & (1 << request.index())) == 0)
		{
			ret.set_result(1);
			player_ref.add_money((e_money_type)mission_reward, mission_reward_num, e_server_log_add_money_cross_ladder_task, -1, false);
			character_proto_character_get_money character_get_money;
			character_get_money.add_money_type_array((e_money_type)mission_reward);
			character_get_money.add_money_num_array(init_unit::change_i32_to_string(mission_reward_num));
			player_ref.send_message_to_self(&character_get_money, e_msgindex_s2c_character_get_money);
			player_ref.set_logic_data(e_role_logic_info_cross_ladder_reward_tag, reward_tag | (1 << request.index()));
			player_ref.send_logic_one(e_role_logic_info_cross_ladder_reward_tag);
		}
		else
			ret.set_result(0);
		player_ref.send_message(&ret, e_msgindex_s2c_cross_ladder_ret_mission_reward);
	}

	void c2s_cross_ladder_return_to_source_zone_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_req_mission_reward)
		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ref.get_map_guid());
		if (nullptr == map_object_ptr || nullptr == map_object_ptr->get_map_template_ptr())
			return;

		if (player_ref.is_self_server() == false && map_object_ptr->get_map_template_ptr()->Type == e_map_type_cross_ladder)
		{
			int32 map_template_id = player_ref.get_unit_info(e_role_info_main_map_id);
			int32 map_line_id = player_ref.get_main_line_id();
			player_ref.transfer_by_template(map_template_id, map_line_id, player_ref.get_unit_info(e_role_info_server_id), guid_64(), 0);
		}
	}

	void c2s_cross_boss_get_boss_is_dead(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_map_boss_is_dead)
		c2s_cross_boss_get_boss_is_dead_logic(player_ref, request);
	}

	void c2s_cross_boss_get_boss_is_dead_logic(player& player_ref, game_proto_get_map_boss_is_dead& request)
	{
		//if (request.map_template_id() <= 0)
		//{
		//	return;
		//}
		//auto map_object_ptr = world_cs::get_map_by_template_id<cross_server_world_boss_map_cs>(request.map_template_id());
		//if (nullptr == map_object_ptr || map_object_ptr->get_map_type() != e_map_type_cross_server_pk || map_object_ptr->get_map_template_ptr()->SubType != 4)
		//{
		//	return;
		//}

		//map_object_ptr->send_boss_dead_info(player_ref.get_unit_guid());
	}

	void c2s_cross_boss_get_boss_is_dead_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(game_proto_get_map_boss_is_dead, c2s_cross_boss_get_boss_is_dead)
	}

	void c2s_req_upgrade_wedding_ring_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_req_upgrade_wedding_ring)
		player_ref.get_marry_mgr().wedding_ring_upgrade(request.use_item_id());
	}
	void c2s_confirm_marry_examination(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_confirm_marry_examination)
		player_ref.get_marry_mgr().confirm_marry_dati(request.is_receive());

	}
	void c2s_get_marry_examination(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_get_marry_examination)
		player_ref.get_marry_mgr().begin_marry_dati(request.is_near());
	}
	void c2s_operate_marry_examination(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_operate_marry_examination)
		player_ref.get_marry_mgr().confirm_marry_operate(request.operate_type(), request.ext_param_1(), request.ext_param_2());
	}
	void c2s_send_oracle_trial_commpingup_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_send_oracle_trial_commpingup)
		player_ref.get_oracle_trial_mgr().send_mopping_up_award();
	}
	void c2s_get_oracle_trial_customs_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_oracle_trial_customs_award)
		int32 error_index = player_ref.get_oracle_trial_mgr().lua_get_stat_num_award(request.tier_num(), request.award_num());
		game_proto_send_oracle_trial_customs_award_end msg;
		msg.set_error_index(error_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_oracle_trial_customs_award_end);
	}
	void c2s_get_boss_god_time(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_boss_god_time)
		c2s_get_boss_god_time_logic(player_ref, request);
	}

	void c2s_get_boss_god_time_logic(player& player_ref, game_proto_get_boss_god_time& request)
	{
		//auto map_object_ptr = world_cs::get_map_by_guid<cross_server_world_boss_map_cs>(player_ref.get_map_guid());
		//if (nullptr == map_object_ptr)
		//{
		//	return;
		//}

		//npc* boss_ptr = map_object_ptr->get_boss(request.boss_id());
		//if (nullptr == boss_ptr)
		//{
		//	return;
		//}
		//boss_ptr->cross_boss_send_boss_god_time(player_ref.get_unit_guid());
	}

	void c2s_get_boss_god_time_lua(player& player_ref, const char* msg, int32 msg_len)
	{
		PROTO_C_TO_S_FUNC(game_proto_get_boss_god_time, c2s_get_boss_god_time)
	}

	void c2s_req_use_fireworks_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_req_use_fireworks)
		player_ref.get_marry_mgr().req_use_fireworks(guid_64(request.obj_guid_a(), request.obj_guid_b()), request.target_name(), request.target_blessing());
	}
	void c2s_get_person_information_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_get_main_person_information)
		player_ref.get_person_infor_mgr().sync_to_client();
	}
	void c2s_get_other_person_information_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_get_other_person_information)
		player_ref.get_person_infor_mgr().get_other_person_information_to_dp(guid_64(request.role_guid_a(), request.role_guid_b()), false);
	}
	void c2s_get_person_infor_reward_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_get_person_infor_reward)
		player_ref.get_person_infor_mgr().get_reward();
	}

	void c2s_req_lucky_info_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(relation_proto_c2s_req_lucky_info)
		player_ref.get_marry_mgr().req_lucky_operator((e_lucky_info_req_type)request.req_type());
	}
	void c2s_get_role_competition_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_get_role_competition)
		player_ref.get_competition_mgr().sync_competition_info();
	}
	void c2s_get_competition_lv_reward_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_get_competition_lv_reward)
		player_ref.get_competition_mgr().get_reward(request.manual_id(), request.manual_type());
	}

	void c2s_get_competition_lv_reward_all_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_get_competition_lv_reward_all)
		player_ref.get_competition_mgr().get_reward_all(e_competition_manual_get_reward_type_all);
	}

	void c2s_buy_competition_lv_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_buy_competition_lv)
		player_ref.get_competition_mgr().buy_competition_level(request.level());
	}
	void c2s_buy_speical_manual_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(cross_ladder_buy_speical_manual)
		//����Ӣ�ֲ�
		player_ref.get_competition_mgr().buy_speical_manual(request.manual_type());
	}

	void c2s_get_element_competition_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_element_competition)
		player_ref.get_element_competition_mgr().sync_competition_info();
	}

	void c2s_get_element_competition_lv_reward_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_competition_lv_reward)
		player_ref.get_element_competition_mgr().get_reward(request.manual_id(), request.manual_type());
	}

	void c2s_get_element_competition_lv_reward_all_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_competition_lv_reward_all)
		player_ref.get_element_competition_mgr().get_reward_all(e_competition_manual_get_reward_type_all);
	}

	void c2s_buy_element_competition_lv_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_buy_competition_lv)
		player_ref.get_element_competition_mgr().buy_competition_level(request.level());
	}

	void c2s_buy_speical_element_manual_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_buy_speical_manual)
		//����Ԫ���ֲ�
		player_ref.get_element_competition_mgr().buy_speical_manual(request.manual_type());
	}

	void c2s_get_map_legion_player_proc(uint32 connindex, const void* data_ptr, size_t data_len) 
	{
		UNPACKING_PROTO(legion_proto_get_map_legion_player)
		//auto map_object_ptr = world_cs::get_map_by_guid<legion_station_map_cs>(player_ref.get_map_guid());
		//if (nullptr == map_object_ptr || map_object_ptr->get_map_type() != e_map_type_legion_station)
		//{
		//	return;
		//}

		//map_object_ptr->send_map_all_play_info(player_ref.get_unit_guid());
	}
	void c2s_start_assist_fight_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(assist_fight_proto_start_assist_fight)
		player_ref.get_assist_fight_mgr().start_assist_fight(request.assist_fight_type(), request.assist_fight_tag());
	}
	void c2s_cancel_assist_fight_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(assist_fight_proto_cancel_assist_fight)
		player_ref.get_assist_fight_mgr().cancel_assist_fight();
	}
	void c2s_create_assist_fight_npc_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(assist_fight_proto_create_assist_fight_npc)
		//auto raid_map_cs_ptr = world_cs::get_map_by_guid<raid_map_cs>(player_ref.get_map_guid());
		//if (nullptr == raid_map_cs_ptr)
		//{
		//	return;
		//}

		//int64 time_now = utility::get_tick_count();
		//raid_map_cs_ptr->set_create_assist_fight_guid(player_ref.get_unit_guid());
		//raid_map_cs_ptr->set_create_assist_fight_time(time_now + second_tick_time * 3);
		//player_ref.set_logic_data(e_role_logic_info_help_tip_send_times, player_ref.get_logic_data(e_role_logic_info_help_tip_send_times) + 1);
		//player_ref.get_mission_mgr().target_check(e_mission_end_type_by_assis_fight_num);
	}
	void c2s_get_cumulative_sign_in_reward(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		//��ȡ�ۼ�ǩ������
		UNPACKING_PROTO(welfare_proto_get_cumulative_sign_in_reward)
		player_ref.get_welfare_mgr().get_cumulative_sign_in_reward(request.reward_index());
	}
	void c2s_retroactive_all_days(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		//��ǩ��������
		UNPACKING_PROTO(welfare_proto_retroactive_all_days)
		player_ref.get_welfare_mgr().retroactive_all_days();
	}
	void c2s_buy_simage_boss_num(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_buy_simple_boss_num)
		if (request.map_type() == e_map_type_boss_single)
		{
			player_ref.get_gain_treasure_mgr().buy_single_boss_num();
		}
	}

	void c2s_save_appearance_info(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_save_customize_appearance_info)
		bool Success = false;
		{
			guid_64 card_guid = player_ref.get_item_set().get_item_by_type(e_item_type_expendable, e_prop_sub_type_appearance_card);
			if (card_guid.is_valid())
			{
				citem* end_item = nullptr;
				Success = player_ref.get_item_set().item_use(card_guid, end_item);
			}
		}
		if (Success == false)
		{
			return;
		}

		player_ref.set_appearance(request.appearance());
	}

	void c2s_is_show_vip(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_show_vip)
		player_ref.show_vip(request.is_show_vip());
	}

	void c2s_dragontrip_oper(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(dragontrip_proto_dragontrip_operate)
		player_ref.get_dragontrip_mgr().on_operate(request.oper_type(), request.oper_param());
	}

	void c2s_mountpower_oper(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(mount_power_proto_mount_power_operate)

		std::vector<guid_64> tmp_vec;

		for (int32 i = 0; i < request.item_guid_size(); i += 2)
		{
			guid_64 tmp;
			tmp.A = request.item_guid(i);
			tmp.B = request.item_guid(i + 1);
			tmp_vec.push_back(tmp);
		}

		guid_64 d_item_guid;

		d_item_guid.A = request.item_a();
		d_item_guid.B = request.item_b();

		player_ref.get_mount_power_mgr().on_oper(tmp_vec, request.oper_type(), request.slot(), d_item_guid);
	}

	void c2s_jewel_carve_oper(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(item_proto_item_jewel_carve_oper)

		guid_64 item_guid;
		item_guid.A = request.item_guid(0);
		item_guid.B = request.item_guid(1);

		player_ref.get_jewel_carve_mgr().on_oper(item_guid, request.oper_type(), request.param());
	}

	void c2s_element_war_buy_ticket(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_element_war_buy_ticket)
		
		e_element_war_buy_ticket_ret ret = e_element_war_buy_ticket_success;
		game_proto_element_war_buy_ticket_end send_msg;
		int32 have_buy_num = player_ref.get_logic_data(e_role_logic_info_element_war_buy_ticket);
		int32 need_num = 0;			//����ϵ��
		int32 money_type = 0;		//�������� 
		int32 const_money_money = 0;//�����۸�
		int32 can_buy_max = 0;		//��������
		int32 gate_server_id = GATECONFIG->gate_id;
		VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(player_ref.get_vip_level(false));
		RegionTemplate* region_ptr = globle_data::get_instance().get_region_template_ptr();
		if (nullptr == vip_temp_ptr || nullptr == region_ptr || vip_temp_ptr->ElementWarlimit.size() < 0 || vip_temp_ptr->ElementWarlimit.size() % 5 != 0)
		{
			send_msg.set_ret_index(e_element_war_buy_ticket_failed);
			player_ref.send_message(&send_msg, e_mgsindex_s2c_element_war_buy_ticket_end);
			return;
		}

		for (int32 i = 0; i < vip_temp_ptr->ElementWarlimit.size() / 5; ++i)
		{
			if (vip_temp_ptr->ElementWarlimit[i * 5] == region_ptr->RegionCode)
			{
				can_buy_max = vip_temp_ptr->ElementWarlimit[i * 5 + 1];
				money_type = vip_temp_ptr->ElementWarlimit[i * 5 + 2];
				const_money_money = vip_temp_ptr->ElementWarlimit[i * 5 + 3];
				need_num = vip_temp_ptr->ElementWarlimit[i * 5 + 4];
				break;
			}
		}
		int32 need_buy_num = have_buy_num * need_num + const_money_money;
		if (have_buy_num >= can_buy_max)
		{
			ret = e_element_war_buy_ticket_limit;
		}
		else if (false == player_ref.can_cut_money((e_money_type)money_type, need_buy_num))
		{
			ret = e_element_war_buy_ticket_jewl;
		}
		else
		{
			player_ref.set_logic_data(e_role_logic_info_element_war_buy_ticket, have_buy_num + 1);
			player_ref.send_logic_one(e_role_logic_info_element_war_buy_ticket);
			player_ref.cut_money((e_money_type)money_type, need_buy_num, e_server_log_cut_money_element_war_buy_ticket);
			cs2ws_element_war_buy_ticket msg;
			msg.role_guid = player_ref.get_unit_guid();
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), gate_server_id);
		}
		send_msg.set_ret_index(ret);
		player_ref.send_message(&send_msg, e_mgsindex_s2c_element_war_buy_ticket_end);
	}

	void c2s_add_use_power_up(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(mission_proto_use_power_up)
		player_ref.get_mission_mgr().set_power_up_be_use(request.power_up_type());
	}

	void c2s_skytreasure_operation(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(skytreasure_proto_skytreasure_operate)
		player_ref.get_skytreasure_mgr().on_operate(request.operation_type(), request.operation_param1(), request.operation_param2());
	}

	void c2s_starark_oper(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(starark_proto_starark_operate)
		player_ref.get_starark_mgr().on_operate(request.oper_type(), request.oper_param1(), request.oper_param2(), request.oper_param3());

	}
	void c2s_lucky_card_operate(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_lucky_card_operate)
		player_ref.get_lucky_card_mgr().next_state(request.touch_index(), request.is_jump());
	}
	void c2s_change_enchant_show_type(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(character_proto_change_enchant_show_type)
		player_ref.enchant_show_type(request.show_type());
	}
	void c2s_get_attack_city_map_rank_list_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_get_attack_city_map_rank_list)
		//auto map_object_ptr = world_cs::get_map_by_guid<attack_city_map_cs>(player_ref.get_map_guid());
		//if (map_object_ptr && map_object_ptr->get_map_type() == e_map_type_attack_city)
		//{
		//	map_object_ptr->send_rank_info(player_ref.get_array_index());
		//}
	}
	void c2s_transfer_to_bron_pos_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_transfer_to_bron_pos)
		//auto map_object_ptr = world_cs::get_map_by_guid<attack_city_map_cs>(player_ref.get_map_guid());
		//if (map_object_ptr && map_object_ptr->get_map_type() == e_map_type_attack_city)
		//{
		//	map_object_ptr->transfer_to_bron_pos(player_ref.get_array_index(), true);
		//}
	}
	void c2s_attack_legion_call_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_attack_legion_call)
		//auto map_object_ptr = world_cs::get_map_by_guid<attack_city_map_cs>(player_ref.get_map_guid());
		//if (map_object_ptr && map_object_ptr->get_map_type() == e_map_type_attack_city)
		//{
		//	cs2ws_send_attack_city_legion_call msg;
		//	msg.play_guid = player_ref.get_unit_guid();
		//	msg.legion_guid = player_ref.get_legion_guid();
		//	connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), player_ref.get_unit_info(e_role_info_server_id));
		//}
	}

	void c2s_end_attack_transfer_info_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_end_attack_transfer_info)
		//auto map_object_ptr = world_cs::get_map_by_guid<attack_city_map_cs>(player_ref.get_map_guid());
		//if (map_object_ptr && map_object_ptr->get_map_type() == e_map_type_attack_city)
		//{
		//	map_object_ptr->end_one_transfer(player_ref.get_unit_guid());
		//}
	}

	void c2s_time_feed_back_operation_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_time_feed_back_operation)
		player_ref.get_time_feed_back_mgr().time_feed_back_operation(request.operation_type(), request.main_tem_id(), request.sub_tem_id());
	}

	void c2s_time_limit_gift_operation_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_time_limit_gift_operation)
		player_ref.get_time_limit_gift_mgr().operation_begin(request.operation_type(), request.template_id());
	}

	void c2s_send_subscribe_daily(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(chat_proto_send_subscribe_daily)
		player_ref.subscribe_daily(request.must_do_type());
	}

	void c2s_fuben_vip_mopping(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_fuben_vip_mopping)
		player_ref.fuben_vip_mopping(request.fuben_template_id(), request.settlement());
	}

	void c2s_attack_city_operate_proc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_attack_city_operate)
		//auto map_object_ptr = world_cs::get_map_by_guid<attack_city_map_cs>(player_ref.get_map_guid());
		//if (map_object_ptr && map_object_ptr->get_map_type() == e_map_type_attack_city)
		//{
		//	map_object_ptr->attack_city_operate(player_ref.get_unit_guid(), request.operate_type(), request.prop_npc_guid());
		//}
	}

	void c2s_fuben_kill_boss(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_kill_boss)
		//map_search_system::repeatd_kill_boss(client_uid.fepsession_uid);
		box_map_system::begin_kill_boss(player_ref.get_map_ent(), &player_ref);
		auto pos = player_ref.get_new_map_pos();
		game_proto_kill_boss_end msg;
		msg.set_result(e_error_code_success);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_kill_boss);
	}

	void c2s_get_offline_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(offline_award_proto_get_award)
		offline_award_system::gain_drop_item(&player_ref);
	}


	void c2s_get_online_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(online_award_proto_get_award)
		online_award_system::get_online_award(&player_ref);
	}

	void c2s_upgrade_search_level(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(game_proto_upgrade_searche_level)
		auto res = map_search_system::upgrade_search_level(player_ref.get_array_index());
		game_proto_upgrade_searche_level_end send_msg;
		send_msg.set_result(res);
		player_ref.send_message(&send_msg, e_msgindex_s2c_upgrade_search_level_end);
	}
	void c2s_get_npc_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_get_npc_award)
		auto& npc_ref = unit_man::get_npc(request.npc_index());
		if (npc_ref.is_valid() == false)
		{
			CONSOLE_ERROR("npc is not valid npc_id:{}", request.npc_index());
			return;
		}
		box_award_system::get_npc_award(&player_ref, &npc_ref);
		treasure_pic_system::get_npc_award(&player_ref, &npc_ref);
	}
	void c2s_kill_award_npc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_kill_award_npc)
		auto& npc_ref = unit_man::get_npc(request.npc_index());
		if (npc_ref.is_valid() == false)
		{
			CONSOLE_ERROR("npc is not valid npc_id:{}", request.npc_index());
			return;
		}
		box_map_system::kill_award_npc(player_ref.get_map_ent(), &player_ref, &npc_ref);
	}
	void c2s_open_treasure_pic(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_open_treasure_pic)
		auto npc_guid = box_map_system::open_treasure_pic(player_ref.get_map_ent(), &player_ref);
		map_s2c_search_award_npc_end msg;
		msg.set_npc_guid(npc_guid);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_search_award_npc_end);
	}
	void c2s_get_treasuer_award(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_get_treasure_award)
		auto& npc_ref = unit_man::get_npc(request.npc_index());
		if (npc_ref.is_valid() == false)
		{
			CONSOLE_ERROR("npc is not valid npc_id:{}", request.npc_index());
			return;
		}
		treasure_pic_system::get_treasure_award(&player_ref, &npc_ref, request.treasure_index());
		map_s2c_get_treasure_award_end msg;
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_get_treasuer_award);
	}
	void c2s_map_anim_end(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_map_anim_end)
		box_map_system::open_new_map(player_ref.get_map_ent(), &player_ref);
		element_map_system::open_new_map(player_ref.get_map_ent(), &player_ref);
	}
	void c2s_refrush_award_npc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_refrush_award_npc)
		//box_map_system::refresh_award(player_ref.get_map_ent(), player_ref.get_array_index());
	}
	void c2s_get_next_award_npc(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_search_award_npc)
		auto npc_guid = box_map_system::get_next_award_npc(player_ref.get_map_ent(), &player_ref);
		map_s2c_search_award_npc_end msg;
		msg.set_npc_guid(npc_guid);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_search_award_npc_end);
	}
	void c2s_flip_box_end(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_flip_box_open)
		int32 open_index = flip_box_system::open_flip_box(player_ref.get_map_ent(), &player_ref, request.open_index());
		map_s2c_flip_box_open_end msg;
		msg.set_open_index(open_index);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_flip_box_open_end);
	}
	void c2s_mop_up_map(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(map_c2s_mop_up_map)
		battle_map_system::mop_up_map(&player_ref, request.map_id());
	}
	void c2s_finger_guess_end(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(finger_guess_c2s_finger_guess_end)
		finger_guess_system::guess_end(&player_ref);
	}
	void c2s_smashing_bottle_end(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(smashing_bottle_c2s_smashing_bottle_end)
		smashing_bottle_system::guess_end(&player_ref);
	}
	void c2s_wheel_plate_begin(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_activity_c2s_get_activity);
		wheel_plate_system::wheel_plate_begin(&player_ref, request.activity_id());
	}
	void c2s_wheel_plate_end(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		UNPACKING_PROTO(time_activity_c2s_wheel_plate_end)
		wheel_plate_system::wheel_plate_end(&player_ref, request.activity_id());
	}
}
