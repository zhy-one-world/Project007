/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 10:58
  file base: teleport_control
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _TELEPORT_CONTROL_HPP_
#define _TELEPORT_CONTROL_HPP_
/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include "Logic/transfer_def.hpp"
/************************************************************************/
/*                           Struct Define                              */
/************************************************************************/
namespace faith
{
	enum e_transfer_type
	{
		e_transfer_type_login,//登录
		e_transfer_type_scene//场景间
	};
	struct teleport_param
	{
		teleport_param()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		faith::e_scene_type					desc_type;
		int32 								desc_map_template_id;
		int32								desc_line_id;
		s_map_pos							desc_map_pos;
		guid_64								desc_map_guid;
		int32								desc_war_index;
		bool								force;
		bool								bOk;
		uint32								dest_group_id;
		e_transfer_type						transfer_type;
		int32								dynamic_ints[kmax_transfer_dynamic_int_num];
	};
	/************************************************************************/
	/*                           Class Declare                              */
	/************************************************************************/
	class teleport_control
	{
		friend class player;

	public:
		teleport_control();

	public:
		void 						set_player_ptr(player* player_ptr) { m_player_ptr = player_ptr; }
		void						clear_data();
		/************************************************************************/
		/*                              传送操作                                */
		/************************************************************************/
		void                        teleport_by_template(int32 map_tempate_id, int32 line_id, int32 group_id, guid_64& map_guid, int32 war_index);
        void                        teleport_by_template_pos(int32 map_tempate_id, int32 line_id, s_map_pos& teleport_pos, guid_64 map_guid);//用于特殊的传送需求,比如回城复活.//Steven.Han
		void 						teleport_to_local(const s_map_pos& map_pos, bool send_client = true); //本地图传送
		bool 						is_teleport_to_same_map(guid_64& map_guid, int32 map_template_id, int32 line_id);
		void						re_teleport_to_local();

		bool 						is_teleporting						( )	const { return m_teleporting; }
		void 						send_transfer_error_message			(i8 result);
		void 						send_transfer_message				(i8 result, const world_location& location, uint32 pos_id);
		void 						send_transfer_message				(i8 result, int32 map_id, s_map_pos map_pos, uint32 pos_id);
			
		s_map_pos					get_random_map_pos(std::vector<int32> map_location_arr);
		/************************************************************************/
		/*							 用于副本中强制退出                         */
		/************************************************************************/
		void 						teleport_world_update				(uint32 elapse_time);
		void 						teleport_world						(uint32 delay_time);
		void 						unteleport_world					()					{ m_delay_teleport = false; }
		bool 						is_teleport_world					() 					{ return m_delay_teleport; }
		teleport_param&				get_teleport_param()					{ return m_teleport_param; }
		void						start_teleport();
		void						set_dynamic_param_int(int32 idx, int32 value);
		bool						get_dynamic_param_int(int32 idx, int32& value) const;
	private:
		player&						get_player()						{ return *m_player_ptr; }
		void						set_teleport_dest_info(faith::e_scene_type desc_type, int32 desc_map_type_id, s_map_pos map_pos);
		void						req_start_teleport(faith::e_scene_type desc_type, int32 desc_map_type_id, int32 line_id, s_map_pos map_pos, guid_64 map_guid, int32 war_index = 0, bool force = false, uint32 dest_group_id = 0);
	

	private:
		world_location				m_teleport_dest;
		bool						m_teleporting;
		bool						m_delay_teleport;
		teleport_param				m_teleport_param;
		player*						m_player_ptr;
	};

}

#endif
