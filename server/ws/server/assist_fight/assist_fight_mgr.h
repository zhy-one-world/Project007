#ifndef _WS_ASSIST_FIGHT_MGR_H_
#define _WS_ASSIST_FIGHT_MGR_H_

#include "Logic/char_def.hpp"

namespace hld
{
	class assist_fight_mgr
	{
	public:
		explicit assist_fight_mgr();
		~assist_fight_mgr();

		static assist_fight_mgr& get_instance()
		{
			static assist_fight_mgr instance;
			return instance;
		}
		void start_assist_fight(const s_assist_fight_info & m_in_info);//外
		void req_assist_fight_end(guid_64 assist_fight_guid, guid_64 role_guid, bool check_result);//外
		void cancel_assist_fight(int32 cancel_type, guid_64 assist_fight_guid, guid_64 role_guid);//外
		void sync_assist_fight_info(guid_64 role_guid, bool is_login = false, guid_64 create_guid = guid_64()); //外
		void broadcast_to_server();
		void broadcast_to_legion(guid_64 legion_guid, guid_64 create_guid = guid_64());
		void sync_assist_fight_info_to_cs(guid_64 role_guid, guid_64 assist_fight_guid);
		void req_assist_fight(guid_64 role_guid, guid_64 assist_fight_guid); //外
		bool check_can_assist_fight(guid_64 role_guid, int32 assist_fight_tag, int32 map_template_id, guid_64 map_guid);
		void sync_assist_fight_info_to_cs_login(guid_64 role_guid); //外
		void check_assist_fight_legion_team(guid_64 role_guid); //外
		void on_player_logout(guid_64 role_guid);//外

		void send_create_assist_fight_notic(const s_assist_fight_info & m_in_info);
	public:
		bool is_use_lua();
	private:
		std::map<guid_64, s_assist_fight_info> m_assist_fight_list;
	};
}

#endif