/********************************************************************
	created:	2015/03/13
	created:	13:3:2015   12:31
	file base:	unit_man
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	class unit;
	class player;
	class npc;
	struct s_client_uid;
	struct s_unit_identifier;

	class unit_man
	{
	public:
		static bool init_manager();	//初始化管理
		static void reload_csv();
		static void heart_tick(const int64& new_time, const int32& tick_time);//心跳管理
		static void dump_npc_info_by_template_id(int32 npc_id);
	public:
		//player
		static player& get_player(const guid_64& unit_guid);
		static player& get_player(const s_client_uid& uid);
		static player& get_player(const int32& array_index);
		static player& get_player(const xstring& szname);
		static player& get_player(const s_unit_identifier& unit_idf);
		static int32 get_player_num();
		static int32 get_robot_num();
		static void add_robot_num();
		static player& get_empty_player(const guid_64 unit_guid);
		static void remove_player(const guid_64& unit_guid);
		static void remove_player(const int32& array_index);
		static void remove_all_player();
	public:		    
		//npc		 
		static int32 get_npc_num();
		static npc& get_npc(const s_unit_identifier& unit_idf);
		static npc& get_npc(const guid_64& unit_guid);
		static npc& get_npc(const int32& array_index);
		static npc& get_empty_npc();
		static void remove_npc(const guid_64& unit_guid, bool is_dead);
		static void remove_npc(const int32 array_index, bool is_dead);
	public:
		//all						
		static void	kill_unit(const int32 killer_index, const int32 array_index);
		static unit& get_unit(const guid_64& unit_guid);
		static unit& get_unit(const int32& array_index);
		static void	operate_unit_att(const int32& unit_array_index, const bool& is_add, const std::vector<float>& att_array);
		static unit& get_unit(const s_unit_identifier& unit_idf);
		static void	remove_unit(const guid_64& unit_guid);
		static void	remove_unit(const int32& array_index);
	public:
		static void	save_all_player(e_logout_result logout_result);
		static void	logout_player(const guid_64& unit_guid, const int32& array_index, bool bcs, e_logout_result logout_result);
		static void	set_all_player_buff(int32 buff_template_id, bool is_add);
		static void	refresh_all_time_activity();
	public:
		static int32 get_grade_num(int32 player_template_id, int32 class_type);
		static int32 get_world_exp_value();
		static bool	kill_player_num(guid_64 guid_killer, guid_64 guid_killed, int32 level_killer, int32 level_killed); //跨服掠夺击杀敌人是否有效判定;
		static void	refresh_kill_num_at_zero_hour();
	public:
		static void	log_msg_info(int32 msg_header, int32 msg_size);
		static void	log_msg_print_file();
		static void	log_msg_info_client(int32 msg_header, int32 msg_size);
		static void	refresh_all_player_time_limit();//因为gm强制结束某个活动，刷新活动状态		
	};
}