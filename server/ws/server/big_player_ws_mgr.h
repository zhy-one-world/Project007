#ifndef _WS_BIG_PLAYER_WS_MGR_H_
#define _WS_BIG_PLAYER_WS_MGR_H_

#include <core.hpp>
#include "logic/type_def.hpp"
#include "template/template_manager.h"
#include "Logic/fake_player_def.hpp"
#include "utility/cs_date.hpp"
#include "big_player_msg.hpp"
#include "ws_client.hpp"


namespace hld
{
	class big_player_ws_mgr
	{
		big_player_ws_mgr();
		~big_player_ws_mgr();
	public:
		static				big_player_ws_mgr& get_instance()
		{
			static big_player_ws_mgr instance;
			return instance;
		}

		void				clear_data();
		void				tick(int32 dt);
		void				set_big_player_guid(e_big_player_type big_type,guid_64 player_guid,int32 from = 0); //外
		guid_64				get_big_player_guid(e_big_player_type big_type); //外
		void				save_data_to_db();
		void				load_data_by_db();
		void				load_detail_data(int32 index);
		void				load_data_by_db_end(const s_big_player_db* data_info, int32 data_num = e_big_player_type_max); //外

		void				handle_base_detail_info(int32 big_type,const s_unit_info role_info);
		void				handle_item_info(int32 big_type,const s_item_info* item_info);
		void				handle_att_info(int32 big_type, const s_base_att_info& att_info);
		void				handle_skill_info(int32 big_type,const s_skill_info* skill_info);
		void				handle_buff_info(int32 big_type, const s_buff_info* buff_info);

		bool				crate_big_player_statue_by_big_type(int32 big_type);
		void				load_end(int32 big_type);
		void				crate_big_player_statue_success(int32 big_type, guid_64 statue_guid); //外
		void				send_big_player_statue_info();
		void				load_statue_by_ws_proc(guid_64 request_player_guid); //外

		void				send_big_player_msg_to_other_server(bool is_load_flag, int32 server_id = 0); //外
		void				set_title_session_login(const guid_64& role_guid, bool(&loading_title_flag)[hld::e_title_type_max]);
		int32				get_big_player_title_type(int32 player_type);

		void				clear_title_with_big_player_clear(int32 big_player_type); //外
		int32				get_big_player_type_with_ranking_type(int32 ranking_type); //外

		bool                is_use_lua();

	private:
		guid_64				player_guid_array[e_big_player_type_max];
		s_fake_player_info	player_info_array[e_big_player_type_max];
		guid_64				statue_guid_array[e_big_player_type_max];
		int32				create_list[e_big_player_type_max];
		int32				m_last_save_time;
		int32				m_timer;
		int32				m_last_tick_time;

	};
}



#endif
