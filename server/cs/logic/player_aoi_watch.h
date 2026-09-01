
#ifndef __PLAYER_AOI_WATCH_H__
#define __PLAYER_AOI_WATCH_H__

#include "logic/type_def.hpp"
#include "Logic/team_def.hpp"

namespace faith
{
	const int32 aoi_watch_num = 30;
	const int32 aoi_watch_max = aoi_watch_num + max_team_member_num;//最大30人 + 队员
	class  player_aoi_watch
	{
	private:
		int32 m_aoi_watch[aoi_watch_max];
		int32 m_unit_index; 
	public:
		player_aoi_watch();
		void clear_data();
		void set_unit_index(const int32& unit_index);
		void end_team(const int32& unit_index);
		bool can_join_aoi(const int32& unit_index);
		void add_aoi_index(const int32& unit_index);
		void del_aoi_index(const int32& unit_index);
		bool is_in_aoi(const int32& unit_index);
		void send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self, bool use_length);
		void send_message_to_aoi(const char* msg, int32 msg_len, uint32 header, bool to_self, bool use_length);
	};
}

#endif //__PLAYER_AOI_WATCH_H__
