#pragma once
#include "logic/type_def.hpp"
#include "base/ecs_world.h"
#include "google/protobuf/message.h"
namespace faith
{
	enum e_aoi_move_type;
	struct MapTemplate;

	class aoi_system
	{
	public:
		static void start_up(Entity* map_ent, MapTemplate* map_template);
		static void shut_down(Entity* map_ent);
		static void send_message_aoi_all(unit_index_map& player_map, google::protobuf::Message* net_pro, uint32 header);
		static void send_message_aoi_by_type(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos_old, const fvector& map_pos_new,e_aoi_move_type move_type, int32 array_index);
		static void send_message_aoi_by_type(const guid_64& map_guid, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos_old, const fvector& map_pos_new,e_aoi_move_type move_type, int32 array_index);
		static void send_message_aoi_all(Entity* map_ent, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos, const int32& array_index, bool use_length = false);
		static void send_message_aoi_all(const guid_64& map_guid, google::protobuf::Message* net_pro, uint32 header, const fvector& map_pos, const int32& array_index, bool use_length = false);
	public:
		static const unit_index_map& get_watch_all(Entity* map_ent, const fvector& map_pos);
		static const unit_index_map& get_watch_all(const guid_64& map_guid, const fvector& map_pos);
		static void get_watch_by_type(Entity* map_ent, const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32* unit_array, int32& watch_array_num);
		static void get_watch_by_type(const guid_64& map_guid, const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32* unit_array, int32& watch_array_num);
	public:
		//把物体添加到aoi管理中
		static bool add_object(Entity* map_ent, const fvector& unit_location, const int32 unit_index);
		//把物体从aoi管理中删除
		static bool remove_object(Entity* map_ent, const fvector& unit_location, const int32 unit_index, bool is_dead);
		//更新物体aoi状态
		static bool update_object(Entity* map_ent, const fvector& map_pos_old, const fvector& map_pos_new, const int32& unit_index);
	};
}

