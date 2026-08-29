#ifndef _WS_LUCKY_DRAW_RECORD_WS_MGR_H_
#define _WS_LUCKY_DRAW_RECORD_WS_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/item_def.hpp"

namespace faith
{
	class lucky_draw_record_ws_mgr
	{
		lucky_draw_record_ws_mgr();
		~lucky_draw_record_ws_mgr();
	public:
		static				lucky_draw_record_ws_mgr& get_instance()
		{
			static lucky_draw_record_ws_mgr instance;
			return instance;
		}

		void							clear_data();
		void							tick(int32 dt);

		void							save_data_to_db();
		void							load_data_by_db();
		void							load_data_by_db_end(const s_hope_hunt_record_info* record_info, int32 data_num);

		void							send_record_all_to_req_player(guid_64 target_guid);

		void							add_record(s_hope_hunt_record_info record_info);
		//rename
		void	change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
	private:
		std::list<s_hope_hunt_record_info> m_sprite_record_array;
		std::list<s_hope_hunt_record_info> m_hope_item_record_array;
	};
}
#endif
