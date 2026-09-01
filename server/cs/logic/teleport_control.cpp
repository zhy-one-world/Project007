/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 20:16
  file base: teleport_control
  file ext: cpp
  author: zhy
  
  purpose: 
*********************************************************************/

/************************************************************************/
/*                            File Include                              */
/************************************************************************/
#include "base/ecs_world.h"
#include "components/scene/base_map_component.h"
#include "connection_mgr.hpp"
#include "internal/core.hpp"
#include "internet/error.pb.h"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "logic/player.hpp"
#include "server_log.hpp"
#include "teleport_control.hpp"
#include "template/template_manager.h"
#include "utility/init_unit.h"
#include "utility/random.h"

namespace faith
{
	/************************************************************************/
	/*                           Class Implement                            */
	/************************************************************************/
	teleport_control::teleport_control()
	{
		ZoneScoped;
		m_player_ptr = nullptr;
		clear_data();
	}
	void teleport_control::clear_data()
	{
		ZoneScoped;
		m_teleport_dest.clear_data();
		m_teleporting = true;
		m_delay_teleport = false;
		m_teleport_param.clear_data();
	}

	void teleport_control::teleport_by_template_pos(int32 map_tempate_id, int32 line_id, s_map_pos& teleport_pos, guid_64 map_guid)
	{
		ZoneScoped;
		if (nullptr == m_player_ptr)
		{
			CONSOLE_ERROR("m_player_ptr is nullptr");
			return;
		}
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, map_tempate_id);
		if (map_ptr == NULL)
		{
			CONSOLE_ERROR("map_ptr is nullptr map_tempate_id:{}", map_tempate_id);
			send_transfer_error_message(faith::e_error_code_template_param);
			return;
		}
		
		if (is_teleport_to_same_map(map_guid, map_tempate_id, line_id))
		{
			//同地图传送
			CONSOLE_INFO("teleport_to_same_map map_tempate_id:{} line_id:{}", map_tempate_id, line_id);
			teleport_to_local(teleport_pos);
			return;
		}

		req_start_teleport(e_scene_type_low_map, map_tempate_id, line_id, teleport_pos, map_guid);
	}

	void teleport_control::teleport_by_template(int32 map_tempate_id, int32 line_id, int32 group_id, guid_64& map_guid, int32 war_index)
	{
		ZoneScoped;
		if (nullptr == m_player_ptr)
		{
			return;
		}
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, map_tempate_id);
		if (map_ptr == NULL)
		{
			send_transfer_error_message(faith::e_error_code_template_param);
			return;
		}
		s_map_pos temp_pos;
		if (map_ptr->RandomBormLocation.size() >= 6	)
		{
			temp_pos = get_random_map_pos(map_ptr->RandomBormLocation);
		}
		else
		{
			if (map_ptr->BornLocation.size() < 3 || map_ptr->BornRotation.size() < 3)
			{
				send_transfer_error_message(faith::e_error_code_template_param);
				return;
			}

			if (war_index > 0)
			{
				if (map_ptr->BornLocation.size() < war_index * 3 || map_ptr->BornRotation.size() < war_index * 3)
				{
					send_transfer_error_message(faith::e_error_code_template_param);
					return;
				}
			}

			if (war_index > 0)
			{
				temp_pos.set_location(map_ptr->BornLocation[(war_index - 1) * 3 + 0], map_ptr->BornLocation[(war_index - 1) * 3 + 1], map_ptr->BornLocation[(war_index - 1) * 3 + 2]);
				temp_pos.set_rotation(map_ptr->BornRotation[(war_index - 1) * 3 + 0], map_ptr->BornRotation[(war_index - 1) * 3 + 1], map_ptr->BornRotation[(war_index - 1) * 3 + 2]);
			}
			else
			{
				temp_pos.set_location(map_ptr->BornLocation[0], map_ptr->BornLocation[1], map_ptr->BornLocation[2]);
				temp_pos.set_rotation(map_ptr->BornRotation[0], map_ptr->BornRotation[1], map_ptr->BornRotation[2]);
			}
		}
		if (group_id != 0 && is_teleport_to_same_map(map_guid, map_tempate_id, line_id))
		{
			// 此接口不支持 同地图传送
			//return;跨服传送，支持同地图传送
		}
		if (map_ptr->Type == faith::e_map_type_king_of_pk && map_ptr->SubType == e_pk_king_map_sub_type_combat)
		{
			m_player_ptr->get_pk_king_bron_pos(map_guid, temp_pos);
		}

		//auto map_obj_ptr = world_cs::get_map_by_guid<map_object>(map_guid);
		//if (nullptr != map_obj_ptr)
		//{
		//	map_obj_ptr->get_transfer_map_pos(m_player_ptr->get_array_index(), temp_pos, war_index);
		//}

		if (map_ptr->Type == faith::e_map_type_big_map && map_tempate_id == m_player_ptr->get_unit_info(e_role_info_main_map_id))
		{
			temp_pos = m_player_ptr->get_main_pos();//从副本传回大世界会走到这
		}
		req_start_teleport(e_scene_type_low_map, map_tempate_id, line_id, temp_pos, map_guid, war_index, false, group_id);
	}

	void teleport_control::teleport_to_local(const s_map_pos& map_pos, bool send_client)
	{
		ZoneScoped;
		if (nullptr == m_player_ptr)
		{
			CONSOLE_ERROR("m_player_ptr is nullptr");
			return;
		}
		
		m_player_ptr->set_new_map_pos(map_pos);
		m_player_ptr->get_pawn_att().set_last_move_time(utility::get_tick_count() + second_tick_time);
		m_player_ptr->set_time_stamp(utility::get_tick_count() + second_tick_time);
		//moveTo?
		m_player_ptr->kill_all_pet();
		m_player_ptr->summon_all_pet();
		if (send_client)
		{
			game_proto_trans_local_scene_end msg;
			msg.set_role_guid(m_player_ptr->get_unit_guid().server_64);
			auto new_pos = msg.mutable_position();
			new_pos->set_x(map_pos.unit_location.x);
			new_pos->set_y(map_pos.unit_location.y);
			new_pos->set_z(map_pos.unit_location.z);
			auto new_angle = msg.mutable_rotator_angle();
			new_angle->set_x(0);
			new_angle->set_y(0);
			new_angle->set_z(map_pos.unit_rotation.yaw);
			m_player_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_transfer_local_scene_end);
		}
	}

	bool teleport_control::is_teleport_to_same_map(guid_64& map_guid, int32 map_template_id, int32 line_id)
	{
		ZoneScoped;
		if (nullptr == m_player_ptr)
		{
			return false;
		}
		
		auto base_map_cp = m_player_ptr->get_map_ent()->get_component<base_map_component>();

		if (map_guid.is_valid() && map_guid == m_player_ptr->get_map_guid())
		{
			return true;
		}

		return init_unit::is_goto_same_map(map_template_id, line_id, base_map_cp->m_map_id, base_map_cp->m_line_id);
	}
	
	void teleport_control::re_teleport_to_local()
	{
		ZoneScoped;
		if (nullptr == m_player_ptr)
		{
			CONSOLE_ERROR("m_player_ptr is nullptr");
			return;
		}
		auto base_map_cp = m_player_ptr->get_map_ent()->get_component<base_map_component>();
		
		req_start_teleport(e_scene_type_low_map, base_map_cp->m_map_id, base_map_cp->m_line_id, m_player_ptr->get_new_map_pos(), m_player_ptr->get_map_guid());
	}

	void teleport_control::send_transfer_error_message(i8 result)
	{
		ZoneScoped;
		s_map_pos temp_pos;
		send_transfer_message(result, 0, temp_pos, 0);
	}

	void teleport_control::send_transfer_message(i8 result, const world_location& location, uint32 pos_id)
	{
		ZoneScoped;
		send_transfer_message(result, location.instance_type_id, location.map_pos, pos_id);
	}

	void teleport_control::send_transfer_message(i8 result, int32 map_id, s_map_pos map_pos, uint32 pos_id)
	{
		ZoneScoped;
	}

	s_map_pos teleport_control::get_random_map_pos(std::vector<int32> map_location_arr)
	{
		ZoneScoped;
		s_map_pos temp_pos;
		if (map_location_arr.size() < 6)
		{
			return temp_pos;
		}
		int32 begin_x = map_location_arr[0];
		int32 begin_y = map_location_arr[1];
		int32 begin_z = map_location_arr[2];
		int32 end_x = map_location_arr[3];
		int32 end_y = map_location_arr[4];
		int32 end_z = map_location_arr[5];

		int32 random_x = random_gen::get_random(begin_x, end_x);
		int32 random_y = random_gen::get_random(begin_y, end_y);
		int32 random_z = random_gen::get_random(begin_z, end_z);

		temp_pos.set_location(random_x, random_y, random_z);
		temp_pos.set_rotation(0, 0, 0);
	
		return temp_pos;
	}

	// 用于副本中强制退出
	void teleport_control::teleport_world_update(uint32 elapse_time)
	{
		ZoneScoped;
	}

	void teleport_control::teleport_world(uint32 delay_time)
	{
		ZoneScoped;
		m_delay_teleport	= true;
	}
	void teleport_control::set_teleport_dest_info(faith::e_scene_type desc_type, int32 desc_map_template_id, s_map_pos map_pos)
	{
		ZoneScoped;
		m_teleport_dest.transfer_type_id = desc_type;
		m_teleport_dest.instance_type_id = desc_map_template_id;
		m_teleport_dest.map_pos = map_pos;
	}

	void teleport_control::req_start_teleport(faith::e_scene_type desc_type, int32 desc_map_template_id, int32 line_id, s_map_pos map_pos, guid_64 map_guid, int32 war_index, bool force, uint32 dest_group_id)
	{
		ZoneScoped;
		auto map_template_ptr = GET_TEMPLATE(MapTemplate, desc_map_template_id);
		if (nullptr == map_template_ptr)
		{
			CONSOLE_ERROR("map_template_ptr is nullptr desc_map_template_id:{}", desc_map_template_id);
			return;
		}
		if (m_teleporting == true)
		{
			CONSOLE_ERROR("m_teleporting is true");
			return;
		}
		m_teleporting = true;

		if (nullptr == m_player_ptr)
		{
			CONSOLE_ERROR("m_player_ptr is nullptr");
			return;
		}

		if (m_teleport_param.bOk == true)
		{
			CONSOLE_ERROR("m_teleport_param.bOk is true");
			return;
		}

		server_log::map_transfer_log(m_player_ptr->get_third_info(), m_player_ptr->get_unit_info_inst(), desc_type, desc_map_template_id, line_id, map_pos, map_guid, war_index, force, dest_group_id);
		m_teleport_param.desc_type		= desc_type;
		m_teleport_param.desc_map_template_id	= desc_map_template_id;
		m_teleport_param.desc_line_id	= line_id;
		m_teleport_param.desc_map_pos	= map_pos;
		m_teleport_param.desc_map_guid = map_guid;
		m_teleport_param.desc_war_index = war_index;
		m_teleport_param.force			= force;
		m_teleport_param.bOk			= true;
		m_teleport_param.dest_group_id	= dest_group_id;
		m_teleport_param.transfer_type	= e_transfer_type_scene;
		set_teleport_dest_info(desc_type, desc_map_template_id, map_pos);

		m_player_ptr->set_unit_info(e_role_info_leave_map_id, m_player_ptr->get_unit_info(e_role_info_move_map_id));
		m_player_ptr->set_unit_info(e_role_info_move_line_id, line_id);
		m_player_ptr->set_unit_info(e_role_info_move_map_id, desc_map_template_id);

		if (map_template_ptr->Type == e_map_type_big_map)
		{
			m_player_ptr->set_unit_info(e_role_info_main_map_id, desc_map_template_id);
			m_player_ptr->set_main_pos(map_pos);
		}
		m_player_ptr->set_move_pos(map_pos);
		if (dest_group_id > 0)
		{
			m_player_ptr->set_unit_info(e_role_info_move_server_id, dest_group_id);
			m_player_ptr->set_unit_info(e_role_info_move_war_idex, war_index);
			m_player_ptr->m_move_map_guid = map_guid;
			m_player_ptr->save_role_all_data(e_save_data_type_cross);
		}
		else
		{
			m_player_ptr->save_role_all_data(e_save_data_type_leave_scene);
		}

	}

	void teleport_control::start_teleport()
	{
		ZoneScoped;
		if (m_teleport_param.bOk == false)
		{
			CONSOLE_ERROR("m_teleport_param.bOk is false");
			return;
		}
		m_teleport_param.bOk = false;

		if (NULL == m_player_ptr)
		{
			CONSOLE_ERROR("m_player_ptr is nullptr");
			return;
		}

		faith::e_scene_type desc_type				= m_teleport_param.desc_type;
		int32 desc_map_template_id					= m_teleport_param.desc_map_template_id;
		int32 desc_line_id							= m_teleport_param.desc_line_id;
		s_map_pos temp_pos							= m_teleport_param.desc_map_pos;
		guid_64 map_guid							= m_teleport_param.desc_map_guid;
		int32 war_index								= m_teleport_param.desc_war_index;

		set_teleport_dest_info(desc_type, desc_map_template_id, temp_pos);

		faith::cs2ws_tansfer_player send_msg;
		send_msg.role_guid = m_player_ptr->get_unit_guid();
		send_msg.transfer_info.scene_type = desc_type;
		send_msg.transfer_info.map_id = desc_map_template_id;
		send_msg.transfer_info.line_id = desc_line_id;
		send_msg.transfer_info.map_pos = temp_pos;
		send_msg.transfer_info.map_guid = map_guid;
		send_msg.transfer_info.war_index = war_index;
		send_msg.dest_group_id = m_teleport_param.dest_group_id;
		memcpy(send_msg.transfer_info.dynamic_ints, m_teleport_param.dynamic_ints, sizeof(send_msg.transfer_info.dynamic_ints));
		memcpy(&send_msg.role_unit_info, &(m_player_ptr->get_unit_info_inst()), sizeof(send_msg.role_unit_info));

		connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(send_msg));
		m_teleporting = false;
	}

	void teleport_control::set_dynamic_param_int(int32 idx, int32 value)
	{
		ZoneScoped;
		if (idx >= 0 && idx < kmax_transfer_dynamic_int_num)
		{
			m_teleport_param.dynamic_ints[idx] = value;
		}
	}

	bool teleport_control::get_dynamic_param_int(int32 idx, int32& value) const
	{
		ZoneScoped;
		if (idx >= 0 && idx < kmax_transfer_dynamic_int_num)
		{
			value = m_teleport_param.dynamic_ints[idx];
			return true;
		}
		return false;
	}
}
