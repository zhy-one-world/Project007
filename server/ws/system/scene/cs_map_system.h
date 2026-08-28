#pragma once
#include "logic/type_def.hpp"
#include "base/ecs_world.h"
#include "google/protobuf/message.h"

namespace hld
{
	struct cs_map_component;
	struct s_fake_player_info;
	struct MapTemplate;

	class cs_map_system
	{
	public:
		static void start_up(Entity* map_ent, MapTemplate* map_template, int32 map_template_id, int32 line_id, int32 server_uid);
		static void shut_down(Entity* map_ent);
		static void tick(float tick_time);
	public:
		static void add_unit(Entity* map_ent, const guid_64& role_guid);
		static int32 del_unit(Entity* map_ent, const guid_64& role_guid);
		static void set_map_state(Entity* map_ent, e_map_state map_state);
		static void set_own_team_guid(Entity* map_ent, guid_64 team_guid);
	public:
		static void send_to_cs_create_map(const cs_map_component& map_cp);
		static void send_to_cs_delete_map(const cs_map_component& map_cp);
		static void send_to_cs_double_line(const cs_map_component& map_cp);
		static void send_message_to_cs(Entity* map_ent, const void* data_ptr, size_t data_len);
		static void send_message_to_cs(const cs_map_component& map_cp, const void* data_ptr, size_t data_len);
		static void send_message_to_cs_lua(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header);
		static void send_message_to_cs_lua(const cs_map_component& map_cp, google::protobuf::Message* net_pro, uint32 header);
		static void send_message_to_all_client(const cs_map_component& map_cp, google::protobuf::Message* net_pro, uint32 header);
		static void send_big_player(const cs_map_component& map_cp, const s_fake_player_info& fake_player_info, int32 big_type);
		static void send_harry_player(const cs_map_component& map_cp, const s_fake_player_info& fake_player_info, int32 big_type);
	public:
		static MapTemplate* get_map_template(const guid_64& map_guid);
		static int32 get_map_template_id(const guid_64& map_guid);
		static int32 get_map_template_id(Entity* map_ent);
		static int32 get_unit_count(Entity* map_ent);
		static bool is_full(const guid_64& map_guid, int32 check_num = 1);
		static bool is_full(Entity* map_ent, int32 check_num = 1);
	};

}
