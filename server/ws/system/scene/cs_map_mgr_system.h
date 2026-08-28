#pragma once
#include "logic/type_def.hpp"
#include "google/protobuf/message.h"
#include "base/ecs_world.h"

namespace hld
{
	struct s_fake_player_info;
	typedef	std::vector<int32> line_id_vec;
	class client_session;
	enum e_error_code;

	class cs_map_mgr_system
	{
	public:
		static bool start_up();
		static void shut_down();

	public:
		static void	tick(float tick_time);
	public:
		static void	init_map_by_world(int32 conn_index);//创建世界上所有的地图
		static Entity*	init_map_by_main(int32 map_template_id, int32 conn_index);//创建主城地图
		static Entity*	init_map_by_battle(int32 map_template_id, int32 conn_index = -1);//创建副本战斗地图
		static void	init_map_by_cross_world(int32 conn_index);
		static void	remove_map_by_uid(uint32 conn_index);//移除指定server的地图数据
		static void	remove_map_by_guid(const guid_64& map_guid);
		static void	remove_map(Entity* map_ent);
	public:
		static int32 remove_unit_from_map(const guid_64& map_guid, const guid_64& role_guid);//把角色从指定地图里面移除
		static void	on_unit_leave_map(int32 map_template_id, const guid_64& role_guid);
		static bool	check_unit_can_enter_map(int32 map_template_id, const guid_64& role_guid);
	public:
		static Entity* get_map_ws_by_min(int32 map_template_id, int32 trans_unit_num = 1);
		static Entity* get_map_ws_by_max(int32 map_template_id);
		static Entity* get_map_ws_by_line(int32 map_template_id, int32 line_id);
	public:
		static void	get_line_vec(int32 map_template_id, line_id_vec& line_vec);
		static void del_line_info(int32 map_template_id, int32 line_id);

		static void	transfer_to_map(guid_64 role_guid, int32 map_template_id, guid_64 map_guid, int32 war_index = 0);
		static void	transfer_to_map(client_session* session, int32 map_template_id, guid_64 map_guid, int32 war_index = 0, int32 server_id = 0);
		static void	send_map_error_to_client(guid_64 role_guid, e_error_code error_code);
		static void	send_map_error_to_client(client_session* session, e_error_code error_code);
		static void	send_one_map_info_to_client(guid_64 role_guid, int32 map_template_id, int64 map_info[]);
		static void	send_message_to_all_client_by_map_id(int32 map_template_id, google::protobuf::Message* net_pro, uint32 header);
		static void	send_big_player(int32 map_template_id, s_fake_player_info& fake_player_info, int32 big_type);
		static void	send_harry_player(int32 map_temp_id, s_fake_player_info& fake_player_info, int32 idex);
	public:
		static void	show_big_map_unit_num();
	private:
		static Entity* add_cs_map(int32 map_template_id, int32 line_id, int32 cs_uid);
		static void del_cs_map(Entity* map_ent);
	};
}