#ifndef _WS_RED_PACKAGE_WS_MGR_H_
#define _WS_RED_PACKAGE_WS_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/event_def.hpp"
#include "red_package_ws.h"

namespace hld
{
	class character_proto_update_red_pack_info;

	class red_package_ws_mgr
	{
		red_package_ws_mgr();
		~red_package_ws_mgr();
		static red_package_ws_mgr*	s_red_package_ws_mgr_ptr;
	public:
		static red_package_ws_mgr& get_instance()
		{
			static red_package_ws_mgr instance;
			return instance;
		}
		void	clear_data();
		void	tick(float dt);
		int32	add_red_package(int32 red_package_template_id, guid_64 sender_guid, xstring sender_name, xstring text);
		void	get_red_package(guid_64 red_package_guid, guid_64 role_guid, xstring role_name);
		int32	find_empty_slot();
		int32	find_package_index(guid_64 red_package_guid);
		void	send_info_to_all(int32 index);
		void	send_info_to_one(guid_64 target_guid);
		void	send_new_draw_info(int32 index, guid_64 role_guid, xstring role_name, int32 money_num);
		void	add_info_to_msg(character_proto_update_red_pack_info& msg, int32 index);
		void	save_info_to_db();
		void	load_info_by_db();  //无调用
		void	load_receiver_by_db(guid_64 red_package_guid);
		void	load_red_package_end(const s_red_package_base* data_list, int32 data_num);
		void	load_red_package_receiver_end(const s_red_package_receiver* data_list, int32 data_num);
		int32   get_suit_slot();
		int32   get_oldest_red_package_slot();
		//rename																																		
		void	change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
		void	set_red_package_base_proto(server2dp_proto_s_red_package_base * proto_info, s_red_package_base data_info);
		void	set_red_package_receiver_proto(server2dp_proto_s_red_package_receiver * proto_info, s_red_package_receiver data_info);
		void	set_red_package_proto(server2dp_proto_s_red_package * proto_info, s_red_package data_info);
	public:
		bool    is_use_lua();
	private:
		red_package_ws	red_package_list[red_package_total_num];
		red_package_ws*	red_package_arr[red_package_total_num];
		float	tick_count;
		float	auto_save_count;
	};
}



#endif
