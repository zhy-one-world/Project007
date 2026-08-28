/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 20:24
  file base: msgproc_dp
  file ext: cpp
  author: lidongyang
  
  purpose: 
*********************************************************************/

#include "cell_server.hpp"
#include "components/scene/base_map_component.h"
#include "connection_mgr.hpp"
#include "internal/achievement_msg.hpp"
#include "internal/arena_msg.hpp"
#include "internal/att_msg.hpp"
#include "internal/boss_island_msg.hpp"
#include "internal/char_msg.hpp"
#include "internal/convert_msg.hpp"
#include "internal/core.hpp"
#include "internal/cross_server_harry_msg.hpp"
#include "internal/event_msg.hpp"
#include "internal/first_time_do_msg.hpp"
#include "internal/goods_msg.hpp"
#include "internal/guide_msg.hpp"
#include "internal/mail_msg.hpp"
#include "internal/map_msg.hpp"
#include "internal/mission_msg.hpp"
#include "internal/oracle_trial_msg.hpp"
#include "internal/phantom_msg.hpp"
#include "internal/pokedex_msg.hpp"
#include "internal/ranking_msg.hpp"
#include "internal/recycle_msg.h"
#include "internal/service_rank_msg.hpp"
#include "internal/seven_day_goal_msg.hpp"
#include "internal/special_name_msg.hpp"
#include "internal/spirit_msg.hpp"
#include "internal/talent_msg.hpp"
#include "internal/title_msg.hpp"
#include "internal/treasure_msg.hpp"
#include "internal/welfare_msg.hpp"
#include "internal/world_server_msg.hpp"
#include "internet/buff.pb.h"
#include "internet/character.pb.h"
#include "internet/cs2dp.pb.h"
#include "internet/item.pb.h"
#include "internet/map.pb.h"
#include "internet/net.pb.h"
#include "internet/skill.pb.h"
#include "internet/time_activity.pb.h"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/skill_manager.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "msgproc_dp.hpp"
#include "server_log.hpp"
#include "internal/service_goal_msg.hpp"
#include "system/item/item_system.h"
#include "system/item/item_upgrade_system.h"
#include "system/play/time_activity_system.h"
#include "utility/parse_msg.h"
#include "internal/chat_msg.hpp"
#include "internal/gain_treasure_msg.hpp"
#include "internal/time_limit_activity_msg.hpp"

namespace hld
{

#define RECV_DB_DATA(packet_name)  ZoneScoped;\
	packet_name load_data;\
	packet_s2s* load_data_ptr = parse_msg::getInstance().parse_message_server(&load_data, data_ptr, data_len);\
	if (nullptr == load_data_ptr)\
	{\
		CONSOLE_INFO("NULL == packet data_len:{}", data_len);\
		return;\
	}\
	player& player_ref = unit_man::get_player(load_data_ptr->logic_index);\
	if (false == player_ref.is_valid() || player_ref.get_unit_guid() != load_data_ptr->logic_guid)\
	{\
		CONSOLE_INFO("NULL == packet logic_index:{}", load_data_ptr->logic_index);\
		return;\
	}

	static std::vector<ui64> split(std::string str, std::string delimiters)
	{
		int32 current;
		int32 next = -1;
		std::vector<ui64> ids;
		do
		{
			current = next + 1;
			next = str.find_first_of( delimiters, current );
			std::stringstream strValue;
			ui64 ullValue = 0;
			strValue<<str.substr(current, next - current);
			strValue>>ullValue;
			strValue.clear();
			strValue.flush();
			ids.push_back(ullValue);
		} 
		while (next != std::string::npos);
		return ids;
	}


	static bool load_role_info(int32 unit_array_index, const s_unit_info &in_unit_info)
	{
		player& player_ptr = unit_man::get_player(unit_array_index);
		if (false == player_ptr.is_valid() || player_ptr.get_unit_guid() != in_unit_info.role_guid)
		{
			CONSOLE_ERROR("NULL == player");
			return false;
		}

		MapTemplate* move_map_template_ptr = GET_TEMPLATE(MapTemplate, in_unit_info.data_ary[e_role_info_move_map_id]);
		if (move_map_template_ptr == nullptr)
		{
			CONSOLE_ERROR("NULL == move_map_template_ptr");
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			unit_man::remove_player(player_ptr.get_array_index());
			return false;
		}
		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(player_ptr.get_map_guid());
		if (nullptr == map_object_ptr)
		{
			CONSOLE_ERROR("NULL == map_object_ptr");
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			unit_man::remove_player(player_ptr.get_array_index());
			return false;
		}

		s_unit_info unit_info = in_unit_info;
		unit_info.data_ary[e_role_info_move_map_id] = map_object_ptr->get_map_template_id();
		unit_info.data_ary[e_role_info_move_line_id] = map_object_ptr->get_line_id();
		if (unit_info.data_i64_ary[e_role_i64_info_gs_value] <= 0 && unit_info.data_ary[e_role_info_GS_value] > 0)
		{
			unit_info.data_i64_ary[e_role_i64_info_gs_value] = unit_info.data_ary[e_role_info_GS_value];
		}
		if (move_map_template_ptr->Type != hld::e_map_type_big_map && map_object_ptr->get_map_type() == hld::e_map_type_big_map)
		{
			// 从副本里退出游戏 又上线后 副本已经被回收的话 会走这里
			unit_info.data_ary[e_role_info_move_pos_x] = unit_info.data_ary[e_role_info_main_pos_x];
			unit_info.data_ary[e_role_info_move_pos_y] = unit_info.data_ary[e_role_info_main_pos_y];
			unit_info.data_ary[e_role_info_move_pos_z] = unit_info.data_ary[e_role_info_main_pos_z];
			unit_info.data_ary[e_role_info_move_pos_yaw] = unit_info.data_ary[e_role_info_main_pos_yaw];
		}
		player_ptr.set_unit_info_all(unit_info);

		player_ptr.check_is_robot();
		if (false == world_cs::add_player(player_ptr.get_map_ent(), &player_ptr))
		{
			CONSOLE_ERROR("add player to map fail");
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			unit_man::remove_player(player_ptr.get_array_index());
			return false;
		}
		player_ptr.set_loading_flag(e_data_flag_info);
		player_ptr.send_unit_begin_info();
		if (player_ptr.get_is_robot())
		{
			unit_man::add_robot_num();
		}
		return true;


	}
	bool dp2cs_load_role_info_lua(int32 unit_array_index, const char *data_ptr, int32 data_len)
	{

		if (data_ptr == nullptr || !data_len)
		{
			CONSOLE_INFO("NULL == packet");
			return false;
		}
		hld::cs2dp_proto::role_info_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			CONSOLE_INFO("parse fail");
			return false;
		}

		s_unit_info db_data;

		db_data.role_guid.server_64 = msg.role_guid();

		parse_msg::getInstance().my_memcopy_string(db_data.account, max_account_length, msg.account());
		parse_msg::getInstance().my_memcopy_string(db_data.role_name, max_name_size, msg.role_name());
		parse_msg::getInstance().my_memcopy_string(db_data.role_appearance, max_appearance_size, msg.role_appearance());
		parse_msg::getInstance().my_memcopy_string(db_data.role_first_kill_npc_id_arr, max_first_kill_npc_id_str, msg.role_first_kill_ary());
		db_data.map_guid.server_64 = msg.map_guid();
		for (int32 i = 0; i < msg.data_i64_ary_size(); i++)
		{
			db_data.data_i64_ary[i] = msg.data_i64_ary(i);
		}
		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}

		return load_role_info(unit_array_index,db_data);

	}



	// 1. 属性
	void dp2cs_rep_load_char_info(uint32 conn_index,const void* data_ptr,size_t data_len)
	{
		const dp2cs_load_role_info* packet = static_cast<const dp2cs_load_role_info*>(data_ptr);
		if ( NULL == packet )
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ptr = unit_man::get_player(packet->unit_array_index);
		if (false == player_ptr.is_valid() || player_ptr.get_unit_guid() != packet->data_info.role_guid)
		{
			CONSOLE_INFO("NULL == player_ptr");
			return;
		}

		MapTemplate* move_map_template_ptr = GET_TEMPLATE(MapTemplate, packet->data_info.data_ary[e_role_info_move_map_id]);
		if (move_map_template_ptr == nullptr)
		{
			CONSOLE_INFO("NULL == move_map_template_ptr");
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			unit_man::remove_player(player_ptr.get_array_index());
			return;
		}
		auto map_entity = player_ptr.get_map_ent();
		if (nullptr == map_entity)
		{
			CONSOLE_ERROR("NULL == map_entity guid:{}", player_ptr.get_map_guid().server_64);
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			unit_man::remove_player(player_ptr.get_array_index());
			return;
		}
		auto base_map_cp = map_entity->get_component<base_map_component>();
		if (false == base_map_cp.isValid())
		{
			CONSOLE_ERROR("NULL == base_map_cp guid:{}", player_ptr.get_map_guid().server_64);
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			return;
		}

		s_unit_info unit_info = packet->data_info;
		unit_info.data_ary[e_role_info_move_map_id] = base_map_cp->m_map_id;
		unit_info.data_ary[e_role_info_move_line_id] = base_map_cp->m_line_id;
		if (unit_info.data_i64_ary[e_role_i64_info_gs_value] <= 0 && unit_info.data_ary[e_role_info_GS_value] > 0)
		{
			unit_info.data_i64_ary[e_role_i64_info_gs_value] = unit_info.data_ary[e_role_info_GS_value];
		}
		if (move_map_template_ptr->Type != hld::e_map_type_big_map && base_map_cp->m_map_template->Type == hld::e_map_type_big_map)
		{
			// 从副本里退出游戏 又上线后 副本已经被回收的话 会走这里
			unit_info.data_ary[e_role_info_move_pos_x] = unit_info.data_ary[e_role_info_main_pos_x];
			unit_info.data_ary[e_role_info_move_pos_y] = unit_info.data_ary[e_role_info_main_pos_y];
			unit_info.data_ary[e_role_info_move_pos_z] = unit_info.data_ary[e_role_info_main_pos_z];
			unit_info.data_ary[e_role_info_move_pos_yaw] = unit_info.data_ary[e_role_info_main_pos_yaw];
		}
		player_ptr.set_unit_info_all(unit_info);

		player_ptr.check_is_robot();
		if (false == world_cs::add_player(player_ptr.get_map_ent(), &player_ptr))
		{
			CONSOLE_INFO("add player fail");
			cs2ws_enter_game	rep;
			rep.client_uid = player_ptr.get_client_uid();
			rep.char_info_to_ws.role_guid = player_ptr.get_unit_guid();
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws( &rep, sizeof(rep));
			unit_man::remove_player(player_ptr.get_array_index());
			return;
		}
		player_ptr.set_loading_flag(e_data_flag_info);
		player_ptr.send_unit_begin_info();
		if (player_ptr.get_is_robot())
		{
			unit_man::add_robot_num();
		}
	}

	void dp2cs_rep_load_char_data(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr || 0 == data_len)
		{
			CONSOLE_INFO("NULL == packet data_len:{}", data_len);
			return;
		}
		packet_s2s* serer_data_ptr = (packet_s2s*)data_ptr;
		if (nullptr == serer_data_ptr)
		{
			CONSOLE_INFO("NULL == serer_data_ptr");
			return;
		}
		player& player_ref = unit_man::get_player(serer_data_ptr->logic_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != serer_data_ptr->logic_guid)
		{
			CONSOLE_INFO("NULL == player_ref logig_index:{}, in s guid:{} msg guid:{}", serer_data_ptr->logic_index, player_ref.get_unit_guid().server_64, serer_data_ptr->logic_guid.server_64);
			return;
		}
		switch (serer_data_ptr->data_flag)
		{
		case e_data_flag_logic_info:
		{
			character_s2s_sl_logic_data msg;
			parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			player_ref.load_role_logic(msg);
		}
		break;
		case e_data_flag_time:
		{
			character_s2s_sl_time_data msg;
			parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			player_ref.load_role_time(msg);
		}
		break;
		case e_data_flag_map:
		{
			map_s2s_sl_map_record msg;
			parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			player_ref.get_map_record_mgr()->load_map_record_by_db(msg);
		}
		break;
		case e_data_flag_sprite_fetter:
		{
			//item_s2s_sl_sprite_fetter_list msg;
			//parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			//player_ref.get_spirit_mgr().load_spirit_fetter_by_db(msg);
		}
		break;
		case e_data_flag_passive_skill:
		{
			skill_s2s_sl_passive_skill_info msg;
			parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			player_ref.get_passive_skill().load_passive_skill(msg);
		}
		break;
		case e_data_flag_item_upgrade:
		{
			item_s2s_sl_item_upgrade msg;
			parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			item_upgrade_system::load_data_from_db(&player_ref, msg);
		}
		break;
		case e_data_flag_time_activity:
		{
			time_activity_s2s_sl_activity_info msg;
			parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
			time_activity_system::load_data_from_db(&player_ref, msg);
		}
		break;
		default:
		{
			if (serer_data_ptr->data_flag > e_data_flag_item && serer_data_ptr->data_flag < e_data_flag_item + e_bag_type_max)
			{
				e_bag_type bag_type = e_bag_type(serer_data_ptr->data_flag - e_data_flag_item);
				item_s2s_sl_item_info msg;
				parse_msg::getInstance().parse_message_server(&msg, data_ptr, data_len);
				item_system::load_data_from_db(&player_ref, msg, bag_type);
			}
		}
		}
		player_ref.set_loading_flag(e_data_flag(serer_data_ptr->data_flag));
	}
	void dp2cs_rep_load_char_buff(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		buff_proto_buff_save_load load_data;
		packet_s2s* load_data_ptr = parse_msg::getInstance().parse_message_server(&load_data, data_ptr, data_len);
		if (nullptr == load_data_ptr)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(load_data_ptr->logic_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != load_data_ptr->logic_guid)
		{
			return;
		}
		buff_man::load(player_ref.get_array_index(), load_data);
		player_ref.set_loading_flag(e_data_flag_buff);
	}
	void dp2cs_rep_load_char_skill(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		skill_proto_skill_save_load load_data;
		packet_s2s* load_data_ptr = parse_msg::getInstance().parse_message_server(&load_data, data_ptr, data_len);
		if (nullptr == load_data_ptr)
		{
			return;
		}
		player& player_ref = unit_man::get_player(load_data_ptr->logic_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != load_data_ptr->logic_guid)
		{
			return;
		}
		skill_manager::load_skill(player_ref.get_array_index(), load_data);
		player_ref.set_loading_flag(e_data_flag_skill);
	}

	void dp2cs_rep_load_char_legion_skill(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_legion_skill* packet = static_cast<const dp2cs_load_char_legion_skill*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_legion_skill().load_legion_skill(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_legion_skill);
	}

	void dp2cs_rep_load_char_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		item_proto_item_save_load load_proto;
		packet_s2s* load_data_ptr = parse_msg::getInstance().parse_message_server(&load_proto, data_ptr, data_len);
		if (nullptr == load_data_ptr)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(load_data_ptr->logic_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != load_data_ptr->logic_guid)
		{
			return;
		}

		player_ref.get_item_set().load_item_by_db(load_proto);
	}
	void dp2cs_rep_load_char_money(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_money_info* packet = static_cast<const dp2cs_load_money_info*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		player_ref.load_money(packet->data_info);
		player_ref.set_loading_flag(e_data_flag_money);
	}
	void dp2cs_rep_load_char_talent(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_talent* packet = static_cast<const dp2cs_load_char_talent*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_talent_mgr().load_talent_by_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_talent);
	}
	void dp2cs_rep_load_char_treasure(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_treasure* packet = static_cast<const dp2cs_load_char_treasure*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_treasure_mgr().load_treasure_by_db(packet->data_list, packet->data_num);
	}
	void dp2cs_rep_load_char_pokedex(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_pokedex* packet = static_cast<const dp2cs_load_char_pokedex*>(data_ptr);
	
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_pokedex_mgr().load_pokedex_from_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_pokedex);
	}

	void dp2cs_rep_load_char_tinder(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_tinder* packet = static_cast<const dp2cs_load_char_tinder*>(data_ptr);

		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_patron_saint_mgr().load_tinder_from_db(packet->tinder_data);
		player_ref.set_loading_flag(e_data_flag_tinder);
	}


	void dp2cs_rep_load_char_spirit(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_spirit* packet = static_cast<const dp2cs_load_char_spirit*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		//player_ref.get_spirit_mgr().load_spirit_by_db(packet->data_list, packet->data_num);
		//player_ref.set_loading_flag(e_data_flag_spirit);
	}

	void dp2cs_rep_load_char_welfare(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_welfare* packet = static_cast<const dp2cs_load_char_welfare*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
			if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_welfare_mgr().load_welfare_by_db(&packet->welfare_data);
		player_ref.set_loading_flag(e_data_flag_welfare);
	}

	void dp2cs_rep_load_char_active_degree(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_active_degree* packet = static_cast<const dp2cs_load_char_active_degree*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_welfare_mgr().load_active_degree_by_db(&packet->active_degree_data);
		player_ref.set_loading_flag(e_data_flag_active_degree);
	}
	void dp2cs_rep_load_char_welfare_random_get(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_welfare_random_get* welfare_load = static_cast<const dp2cs_load_char_welfare_random_get*>(data_ptr);
		if (nullptr == welfare_load)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(welfare_load->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != welfare_load->role_guid)
		{
			return;
		}
		player_ref.get_welfare_mgr().load_random_get_by_db(&welfare_load->random_get_data);
	}

	void dp2cs_rep_load_char_title(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_title* packet = static_cast<const dp2cs_load_char_title*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_title_mgr().load_title_by_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_title);
	}

	void dp2cs_rep_load_char_mail(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_mail* mail_load = static_cast<const dp2cs_load_char_mail*>(data_ptr);
		if (nullptr == mail_load)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(mail_load->unit_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != mail_load->role_guid)
		{
			return;
		}
		player_ref.get_mail_mgr().load_mail_by_db(mail_load->data_list, mail_load->data_num);

	}

	void dp2cs_rep_load_char_convert(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_convert* packet = static_cast<const dp2cs_load_char_convert*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
			if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_convert_mgr().load_data_from_db(packet->data_list);
		player_ref.set_loading_flag(e_data_flag_convert);
	}

	

	void dp2cs_rep_load_char_mail_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_mail_item* packet = static_cast<const dp2cs_load_char_mail_item*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_mail_mgr().load_mail_item_by_db(packet->mail_item_data, packet->data_num);
	}

	void dp2cs_rep_del_char_mail_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_del_mail_end* mail_del_end = static_cast<const dp2cs_del_mail_end*>(data_ptr);
		if (nullptr == mail_del_end)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(mail_del_end->unit_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != mail_del_end->unit_guid)
		{
			return;
		}
		player_ref.get_mail_mgr().del_mail_end();
	}

	void dp2cs_operate_load_lock_mail_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_operate_load_lock_mail* operate_load_lock_mail = static_cast<const dp2cs_operate_load_lock_mail*>(data_ptr);
		if (nullptr == operate_load_lock_mail)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(operate_load_lock_mail->unit_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != operate_load_lock_mail->unit_guid)
		{
			return;
		}
		player_ref.get_mail_mgr().operate_load_lock(operate_load_lock_mail->operate_value);
	}

	void dp2cs_rep_load_char_had_globel_mail_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_player_had_globel_mail_end* mail_load = static_cast<const dp2cs_load_player_had_globel_mail_end*>(data_ptr);
		if (nullptr == mail_load)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(mail_load->unit_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != mail_load->role_guid)
		{
			return;
		}
		player_ref.get_mail_mgr().load_had_globel_mail_guid_end(mail_load->data_info);
		player_ref.set_loading_flag(e_data_flag_globel_mail_had);
	}


	void dp2cs_rep_load_char_mission(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_mission* packet = static_cast<const dp2cs_load_char_mission*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_mission_mgr().load_mission_by_db(packet->data_list, packet->data_num);
		player_ref.get_mission_mgr().refresh_missions();
		player_ref.set_loading_flag(e_data_flag_mission);
	}
	void dp2cs_rep_load_char_att(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_att* packet = static_cast<const dp2cs_load_char_att*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
			if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		player_ref.get_pawn_att().load_base_att_by_db(packet->data_info);
		player_ref.set_loading_flag(e_data_flag_att);
	}
	void dp2cs_rep_load_char_goods(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_goods* packet = static_cast<const dp2cs_load_char_goods*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_goods_helper_mgr().load_purchase_record_from_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_goods);
	}

	void dp2cs_rep_load_worship_target(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_worship_target* packet = static_cast<const dp2cs_load_worship_target*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_worship_target().load_worship_target_from_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_worship);
	}

	void dp2cs_rep_load_char_map_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_map* packet = static_cast<const dp2cs_load_char_map*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		int32 player_unit_index = player_ref.get_array_index();
		//map_record_mgr::get_instance().load_map_record(player_unit_index, packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_map);
	}

	void dp2cs_rep_load_char_demons_tower(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_demons_tower* packet = static_cast<const dp2cs_load_char_demons_tower*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		//map_record_mgr::get_instance().load_demons_tower(player_ref.get_array_index(), &(packet->demons_tower_info));
		//player_ref.set_loading_flag(e_data_flag_demons_tower);
	}

	void dp2cs_rep_load_char_element_heart(uint32 conn_index, const void* data_ptr, size_t data_len)
	{

	}
	void dp2cs_rep_load_char_special_name(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_special_name* packet = static_cast<const dp2cs_load_char_special_name*>(data_ptr);

		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_special_name_mgr().load_data_from_db(packet->special_name_data);
		player_ref.set_loading_flag(e_data_flag_special_name);
	}
	void dp2cs_rep_load_char_achievement(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_achievement* packet = static_cast<const dp2cs_load_char_achievement*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_achievement_mgr().load_achievement_by_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_achievement);
	}

	//arena
	void dp2cs_rep_load_arena_log(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_resp_load_arena_log* arena_log = static_cast<const dp2cs_resp_load_arena_log*>(data_ptr);
		if (NULL == arena_log)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(arena_log->array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != arena_log->role_guid)
		{
			return;
		}
		std::vector<s_chanllenge_log_info> temp_challenge_log_vec;
		for (int32 i = 0; i < arena_log->data_num;++i)
		{
			temp_challenge_log_vec.push_back(arena_log->role_challenge_log[i]);
		}

		player_ref.get_arena_mgr().get_challenge_logs_end(temp_challenge_log_vec);
	}

	void dp2cs_rep_arena_data_error(uint32 conn_index, const void* data_ptr, size_t data_len)
	{

	}

	void dp2cs_rep_load_arena_char_error(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_arena_char_error* packet = static_cast<const dp2cs_load_arena_char_error*>(data_ptr);
		if (NULL == packet || data_len != sizeof(dp2cs_load_arena_char_error))
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (nullptr == map_object_ptr)
		//{
		//	return;
		//}
		//	map_object_ptr->handle_load_target_error();
	}

	void dp2cs_rep_load_arena_char_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_info* packet = static_cast<const dp2cs_arena_load_char_info*>(data_ptr);
		if (NULL == packet || data_len != sizeof(dp2cs_arena_load_char_info))
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		
		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_info(packet);
		//}
	}

	void dp2cs_rep_load_arena_char_att(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_att* packet = static_cast<const dp2cs_arena_load_char_att*>(data_ptr);
		if (NULL == packet || data_len != sizeof(dp2cs_arena_load_char_att))
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_att(packet);
		//}
	}

	void dp2cs_rep_load_arena_char_skill(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_skill* packet = static_cast<const dp2cs_arena_load_char_skill*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_skill(packet);
		//}
	}

	void dp2cs_rep_load_arena_char_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_item* packet = static_cast<const dp2cs_arena_load_char_item*>(data_ptr);
		if (packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_item(packet);
		//}
	}

	void dp2cs_rep_load_arena_char_fight_att(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_fight_att* packet = static_cast<const dp2cs_arena_load_char_fight_att*>(data_ptr);
		if (packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_fight_att(packet);
		//}
	}
	void dp2cs_rep_load_arena_char_buff(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_buff* packet = static_cast<const dp2cs_arena_load_char_buff*>(data_ptr);
		if (packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_buff(packet);
		//}
	}
	void dp2cs_rep_load_arena_char_spirit(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_spirit* packet = static_cast<const dp2cs_arena_load_char_spirit*>(data_ptr);
		if (packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_spirit(packet);
		//}
	}
	void dp2cs_rep_load_arena_char_belief(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_arena_load_char_belief* packet = static_cast<const dp2cs_arena_load_char_belief*>(data_ptr);
		if (packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		//auto map_object_ptr = world_cs::get_map_by_guid<arena_map_cs>(packet->map_guid);
		//if (map_object_ptr)
		//{
		//	map_object_ptr->handle_load_target_belief(packet);
		//}
	}

	void dp2cs_rep_load_char_data_error(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_data_error* packet = static_cast<const dp2cs_load_data_error*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		CONSOLE_INFO("load data error:role_guid:{} error_code:{} error_id:{}", packet->role_guid.server_64, packet->error_code, packet->error_result);
		player& player_ref = unit_man::get_player(packet->array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			CONSOLE_INFO("player_ref is null");
			return;
		}
		// 加载数据错误处理，session player删除
		unit_man::logout_player(player_ref.get_unit_guid(), player_ref.get_array_index(), true, e_logout_result_load_data_fail);
	}

	void dp2cs_rep_save_char_data_complete(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_save_data_result* packet = static_cast<const dp2cs_save_data_result*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			CONSOLE_ERROR("NULL == player_ref");
			return;
		}
		if (false == packet->is_ok)
		{
			CONSOLE_ERROR("packet->is_ok is false");
			// 存档数据错误处理，session player删除
			unit_man::logout_player(player_ref.get_unit_guid(),player_ref.get_array_index(), true, e_logout_result_save_data_fail);
			return;
		}
		player_ref.set_saving_flag(packet->save_type_ex, packet->save_flag);
		CONSOLE_INFO("role_guid:{} save_type_ex:{} save_flag:{}", packet->role_guid.server_64, packet->save_type_ex, int32(packet->save_flag));
		if (player_ref.get_saveing_flag_all_finish())
		{
			switch (packet->save_type_ex)
			{
			case e_save_data_type_exit_game:
			{
				unit_man::logout_player(player_ref.get_unit_guid(), player_ref.get_array_index(), false, e_logout_result_self);
			}
			break;
			case e_save_data_type_leave_scene:
			case e_save_data_type_cross:
			{
				if (player_ref.get_send_gate_save_end())
				{
					cs2ws_player_save_end end_msg;
					end_msg.role_guid = player_ref.get_unit_guid();
					connection_mgr::getInstance().send_to_ws(&end_msg, sizeof(end_msg), player_ref.get_unit_info(e_role_info_server_id));
				}
				player_ref.set_session_state(e_session_status_dp_loading);
				player_ref.get_transfer_control().start_teleport();
				unit_man::remove_player(player_ref.get_array_index());
			}
			break;
			case e_save_data_type_update:
			{
				player_ref.set_update_db_flag(false);
			}
			break;
			default:
				break;
			}
		}
	}

	void dp2cs_rep_load_char_show_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_show* packet = static_cast<const dp2cs_load_char_show*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& temp_player = unit_man::get_player(packet->unit_array_index);
		if (false == temp_player.is_valid() || temp_player.get_unit_guid() != packet->show_info.role_guid)
		{
			return;
		}

		temp_player.get_item_set().load_char_show_info(&packet->show_info);

	}

	void dp2cs_rep_load_char_yester_must_do(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_character_yesterday_remain_must_do* packet = static_cast<const dp2cs_load_character_yesterday_remain_must_do*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.load_yesterday_must_do_remain_end(packet->data_info);
	}

	void dp2cs_rep_load_char_guide_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_guide_record* packet = static_cast<const dp2cs_load_guide_record*>(data_ptr);
		if (nullptr == packet || data_len != sizeof(dp2cs_load_guide_record))
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
			if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		player_ref.get_guide_mgr().load_guide_record(packet->data_num, packet->data_list);
		player_ref.set_loading_flag(e_data_flag_guide);
	}

	void dp2cs_rep_load_char_first_time_do(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_first_time_do_record* packet = static_cast<const dp2cs_load_first_time_do_record*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
			if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		player_ref.get_first_time_do_mgr().load_first_time_do_records_from_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_fist_time_do);
	}

	//find other player info
	void dp2cs_rep_load_other_player_info_base(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_base_end* get_other_player_info_base = static_cast<const dp2cs_get_other_player_info_base_end*>(data_ptr);
		if (nullptr == get_other_player_info_base)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(get_other_player_info_base->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.find_other_player_info_base_end(get_other_player_info_base->data_info);
	}
	void dp2cs_rep_load_other_player_info_equiping(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_equiping_end* get_other_player_info_equiping = static_cast<const dp2cs_get_other_player_info_equiping_end*>(data_ptr);
		if (nullptr == get_other_player_info_equiping)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(get_other_player_info_equiping->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.find_other_player_info_equiping_end(get_other_player_info_equiping->data_info);
	}
	void dp2cs_rep_load_other_player_info_special_name(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_special_name_end* get_other_player_info_special_name = static_cast<const dp2cs_get_other_player_info_special_name_end*>(data_ptr);
		if (nullptr == get_other_player_info_special_name)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(get_other_player_info_special_name->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.find_other_player_info_special_name_end(get_other_player_info_special_name->data_info);
	}

	void dp2cs_rep_load_other_player_info_base_group(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_base_group_end* get_other_player_info_base_group = static_cast<const dp2cs_get_other_player_info_base_group_end*>(data_ptr);
		if (nullptr == get_other_player_info_base_group)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(get_other_player_info_base_group->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.find_other_player_info_base_group_end(get_other_player_info_base_group->data_info.group_job, get_other_player_info_base_group->data_info.group_name, get_other_player_info_base_group->target_guid);
	}

	void dp2cs_rep_load_other_player_info_spirit(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_spirit_end* get_other_player_info_spirit = static_cast<const dp2cs_get_other_player_info_spirit_end*>(data_ptr);
		if (nullptr == get_other_player_info_spirit)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(get_other_player_info_spirit->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.find_other_player_info_spirit_end(get_other_player_info_spirit->data_info);
	}
	void dp2cs_rep_load_other_player_info_wing_or_mount(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_wing_or_mount_end* get_other_player_info_wing_or_mount = static_cast<const dp2cs_get_other_player_info_wing_or_mount_end*>(data_ptr);
		if (nullptr == get_other_player_info_wing_or_mount)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(get_other_player_info_wing_or_mount->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		player_ref.find_other_player_info_wing_or_mount_end(get_other_player_info_wing_or_mount->data_info, get_other_player_info_wing_or_mount->is_wing);
	}
	void dp2cs_rep_load_other_player_info_buff(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_buff_end* get_other_player_info_buff = static_cast<const dp2cs_get_other_player_info_buff_end*>(data_ptr);
		if (nullptr == get_other_player_info_buff)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(get_other_player_info_buff->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.find_other_player_info_buff_end(get_other_player_info_buff->target_guid, get_other_player_info_buff->buff_info, get_other_player_info_buff->data_num);
	}

	void dp2cs_rep_load_other_player_info_feather(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_feather_end* get_other_player_info_feather  = static_cast<const dp2cs_get_other_player_info_feather_end*>(data_ptr);
		if (nullptr == get_other_player_info_feather)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(get_other_player_info_feather->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.find_other_player_info_feather_end(get_other_player_info_feather->feather_info);
	}

	void dp2cs_rep_load_other_player_info_appearance(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_player_info_appearance_end* msg_data = static_cast<const dp2cs_get_other_player_info_appearance_end*>(data_ptr);
		if (nullptr == msg_data)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(msg_data->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		character_proto_find_other_unit_appearance_info_end msg;
		msg.set_target_guid(msg_data->target_guid);
		msg.set_appearance_id(msg_data->data_info);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_character_find_other_appearance_info);
	}

	void dp2cs_load_hightest_record_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_hightest_record* packet = static_cast<const dp2cs_load_hightest_record*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
			if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.load_history_highest_record(packet->highest_info);
		player_ref.set_loading_flag(e_data_flag_highest_record);
	}

	//auction
	void dp2cs_auction_load_self_selling_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_auction_load_self_selling_end* packet = static_cast<const dp2cs_auction_load_self_selling_end*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_auction_mgr().load_self_selling_from_db(packet->auction_info, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_auction_self_selling);
	}

	void dp2cs_auction_get_auction_list_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_auction_req_goods_list_end* packet = static_cast<const dp2cs_auction_req_goods_list_end*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_auction_mgr().req_goods_list_end(packet->data_info, packet->data_num, packet->cur_page_index, packet->total_info_num);
	}

	void dp2cs_auction_get_trade_record_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_auction_req_trade_record_end* packet = static_cast<const dp2cs_auction_req_trade_record_end*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_auction_mgr().req_trade_record_end(packet->data_info, packet->data_num, packet->cur_page, packet->total_record_num);
	}

	void dp2cs_auction_get_another_sell_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_auction_req_other_sell_info_end* packet = static_cast<const dp2cs_auction_req_other_sell_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_auction_mgr().req_another_sell_info_end(packet->data_info, packet->data_num, packet->sell_money_type, packet->item_template_id);
	}

	/*void dp2cs_auction_find_bid_target_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const ws2cs_auction_find_bid_target_end* packet = static_cast<const ws2cs_auction_find_bid_target_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_auction_mgr().find_bid_target_end(packet->auction_info);
	}*/

	void dp2cs_req_bid_record_failure_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_aution_req_bid_record_failure_end* packet = static_cast<const dp2cs_aution_req_bid_record_failure_end*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		//player_ref.get_auction_mgr().req_bid_record_failure_end((const s_auction_bid_replace_info *)&packet->data_info, packet->data_num);
	}

	void dp2cs_auction_req_bid_notice_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_auction_req_bid_notice_end *packet = static_cast<const dp2cs_auction_req_bid_notice_end *>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.get_auction_mgr().req_bid_notice_end((const s_auction_bid_notice_info *)&packet->data_info, packet->data_num);
	}

	void dp2cs_auction_req_bid_notice_red_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_auction_get_red_state_end *packet = static_cast<const dp2cs_auction_get_red_state_end *>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		player_ref.get_auction_mgr().req_bid_notice_red_end(packet->result, packet->is_red);
	}


	void dp2cs_req_load_char_service_goal_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_service_goal* service_goal_load = static_cast<const dp2cs_load_char_service_goal*>(data_ptr);
		if (NULL == service_goal_load)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(service_goal_load->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != service_goal_load->role_guid)
		{
			return;
		}
		player_ref.get_service_goal_mgr().load_service_goal_by_db(service_goal_load->data_list, service_goal_load->data_num);
		player_ref.set_loading_flag(e_data_flag_service_goal);
	}

	void dp2cs_req_load_char_seven_day_goal_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_seven_day_goal* seven_day_goal_load = static_cast<const dp2cs_load_char_seven_day_goal*>(data_ptr);
		if (NULL == seven_day_goal_load)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}

		player& player_ref = unit_man::get_player(seven_day_goal_load->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != seven_day_goal_load->role_guid)
		{
			return;
		}
		player_ref.get_seven_day_goal_mgr().load_seven_day_goal_by_db(seven_day_goal_load->data_list, seven_day_goal_load->data_num);
		player_ref.set_loading_flag(e_data_flag_seven_day_goal);
	}

	void dp2cs_rep_load_char_daily_must_do_count(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_character_daily_must_do_count* packet = static_cast<const dp2cs_load_character_daily_must_do_count*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.load_daily_must_do_count(packet->data_info);
	}

	void dp2cs_req_load_service_rank_reward_state(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_service_rank_reward_state* packet = static_cast<const dp2cs_load_service_rank_reward_state*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_service_rank_mgr().load_recevice_rewards_state_from_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_service_rank);
	}

	void dp2cs_req_load_boss_island_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_boss_island_info* packet = static_cast<const dp2cs_load_boss_island_info*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_boss_island_mgr().load_boss_island_info_from_db(packet->info_data);
		player_ref.set_loading_flag(e_data_flag_boss_island);
	}

	void dp2cs_req_load_role_harry_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_role_harry_info* packet = static_cast<const dp2cs_load_role_harry_info*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_server_harry_mgr().load_role_harry_info_from_db(packet->info_data);
		player_ref.set_loading_flag(e_data_flag_harry_record);
	}
	void dp2cs_rep_load_char_phantom(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_phantom* packet = static_cast<const dp2cs_load_char_phantom*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_phantom_mgr().load_phantom_by_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_phantom);

	}
	void dp2cs_rep_load_char_recycle(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_recycle* packet = static_cast<const dp2cs_load_char_recycle*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_recycle_mgr().load_recycle_by_db(packet->data_info);
		player_ref.set_loading_flag(e_data_flag_recycle);

	}
	void dp2cs_rep_load_char_recycle_task(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_recycle_task* packet = static_cast<const dp2cs_load_char_recycle_task*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_recycle_mgr().load_recycle_task_by_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_recycle_task);

	}
	void dp2cs_rep_load_char_recycle_invited(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_recycle_invited* packet = static_cast<const dp2cs_load_char_recycle_invited*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		player_ref.get_recycle_mgr().load_recycle_invited_by_db(packet->data_list, packet->data_num);

		//player_ref.set_loading_flag(e_data_flag_recycle_task);
	}
	void dp2cs_rep_recycle_get_inviter_info_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_recycle_get_inviter_info_end *packet = static_cast<const dp2cs_recycle_get_inviter_info_end*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		player_ref.get_recycle_mgr().send_to_dp_find_player_end(packet->data_info);

	}

	void dp2cs_rep_load_oracle_trial_info(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2cs_load_oracle_trial_all_info* packet = static_cast<const dp2cs_load_oracle_trial_all_info*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		int32 data_num = packet->data_num > GAMECONFIG->OracleTrialTierNum ? GAMECONFIG->OracleTrialTierNum : packet->data_num;
		player_ref.get_oracle_trial_mgr().load_oracle_trial_info_from_db(packet->data_list, data_num);
		player_ref.set_loading_flag(e_data_flag_oracle_trial);
	}

	void dp2cs_rep_load_chat_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_chat_record* packet = static_cast<const dp2cs_load_chat_record*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_chat_mgr().load_chat_record_by_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_chat_record);

	}

	void dp2cs_req_load_gain_treasure_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_gain_treasure_info* packet = static_cast<const dp2cs_load_gain_treasure_info*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_gain_treasure_mgr().load_gain_treasure_info_from_db(packet->info_data);
		player_ref.set_loading_flag(e_data_flag_gain_treasure);
	}

	void dp2cs_time_limit_activity_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_time_limit_activity_info_end* packet = static_cast<const dp2cs_load_time_limit_activity_info_end*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}

		player_ref.get_time_limit_activity_mgr().load_data_from_db(packet->data_list, packet->data_num);
		player_ref.set_loading_flag(e_data_flag_time_limit_activity);
	}

	void dp2cs_connect_success_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_connect_success* packet = static_cast<const dp2cs_connect_success*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		cell_server::getInstance().dp_connect_cs_success();
	}

	void dp2cs_get_person_information_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{ 
		const dp2cs_get_person_information* packet = static_cast<const dp2cs_get_person_information*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_person_infor_mgr().update_person_infomation(packet->person_info);
		player_ref.set_loading_flag(e_data_flag_player_information);
	}

	void dp2cs_get_other_person_information_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_other_person_information* packet = static_cast<const dp2cs_get_other_person_information*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_person_infor_mgr().get_other_person_information_to_dp_end(packet->target_guid, packet->person_info, packet->is_ask_player_base_info);
	}

	void dp2cs_get_role_competition_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_role_competition* packet = static_cast<const dp2cs_get_role_competition*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		if (packet->competition_info.have_type == e_competition_type_ladder)
		{
			player_ref.get_competition_mgr().load_competition_info_end(packet->competition_info);
			player_ref.set_loading_flag(e_data_flag_player_competition);
		}
		else if (packet->competition_info.have_type == e_competition_type_element)
		{
			player_ref.get_element_competition_mgr().load_competition_info_end(packet->competition_info);
			player_ref.set_loading_flag(e_data_flag_element_competition);
		}
	}
	void dp2cs_get_role_dragontrip_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_role_dragontrip* packet = static_cast<const dp2cs_get_role_dragontrip*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_dragontrip_mgr().load_dragontrip_by_db(packet->dragontrip_info);

		player_ref.set_loading_flag(e_data_flag_dragontrip);
	}

	void dp2cs_get_role_skytreasure_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_role_skytreasure* packet = static_cast<const dp2cs_get_role_skytreasure*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_skytreasure_mgr().load_skytreasure_by_db(packet->data_num, packet->skytreasure_data_list);

		player_ref.set_loading_flag(e_data_flag_skytreasure);
	}
	void dp2cs_get_role_starark_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_get_role_starark* packet = static_cast<const dp2cs_get_role_starark*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->unit_array_index);
		if (false == player_ref.is_valid() || player_ref.get_unit_guid() != packet->role_guid)
		{
			return;
		}
		player_ref.get_starark_mgr().load_starark_by_db(packet->starark_info);

		player_ref.set_loading_flag(e_data_flag_starark);
	}
	void dp2cs_get_time_feed_back_to_db_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2cs_get_time_feed_back_to_db_end* packet = static_cast<const dp2cs_get_time_feed_back_to_db_end*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_time_feed_back_mgr().load_info_end(packet->info_list, packet->date_num);
		player_ref.set_loading_flag(e_data_flag_time_feed_back);
	}

	void dp2cs_get_time_limit_gift_to_db_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2cs_get_time_limit_gift_to_db_end* packet = static_cast<const dp2cs_get_time_limit_gift_to_db_end*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_time_limit_gift_mgr().load_info_end(packet->info_list, packet->date_num);
		player_ref.set_loading_flag(e_data_flag_time_limit_gift);
	}
	void dp2cs_get_subscribe_daily_info_to_db_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2cs_get_subscribe_daily_info_to_db_end* packet = static_cast<const dp2cs_get_subscribe_daily_info_to_db_end*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_INFO("NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.load_subscribe_daily_info_end(packet->info_list, packet->date_num);
		player_ref.set_loading_flag(e_data_flag_subscribe_daily_info);
	}
}
