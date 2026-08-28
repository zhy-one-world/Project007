#ifndef _WS_GAIN_TREASURE_WS_MGR_H_
#define _WS_GAIN_TREASURE_WS_MGR_H_

#include "Logic/gain_treasure_def.hpp"
#include "../client_session_mgr.hpp"
namespace hld
{
	class game_proto_gain_treasure_kill_record_end;
	class game_proto_gain_treasure_gain_item_record_end;
	class game_proto_gain_treasure_boss_info_one;

	class gain_treasure_ws_mgr
	{
	public:
		explicit gain_treasure_ws_mgr();
		static gain_treasure_ws_mgr& get_instance()
		{
			static gain_treasure_ws_mgr instance;
			return instance;
		}
		void								init_manager(); //外部
		void								save_gain_treasure_record_info_into_db(int32 boss_spawn_point_template_id, int32 del_stamp);//默认存第一条 删除最后一条
		void								req_load_gain_treasure_record_info_from_db();
		void								load_gain_treasure_record_info_from_db_proc(const s_gain_treasure_record_info_one_db* record_infos, int32 data_num); //dp-ws外部
		void								load_gain_treasure_record_info(const s_gain_treasure_record_info_one_db& record_info_db);

		s_gain_treasure_record_info_all*	get_gain_treasure_record_one_boss_info(int32 boss_spawn_point_template_id);
		int32								add_gain_treasure_record(const s_gain_treasure_record_info_one& record_info);//返回需要删除记录的时间戳
		void								add_gain_treasure_record_and_save(const s_gain_treasure_record_info_one& record_info); //cs-ws外部

		void								add_gain_item_record(s_gain_treasure_gain_item_record_info& record_info);
		void								add_gain_item_reocrd_by_one_boss(const s_gain_treasure_record_info_one& record_info);
		void								add_gain_item_reocrd_by_dorp_record_model(const s_gain_treasure_record_info_one& record_info, e_drop_record_model_type record_model);//找到该条中对应模式的物品并记录

		void								req_kill_record_end(client_session* session, int32 boss_spawn_point_template_id); //cs-ws外部
		int32								fill_kill_record_end_msg(game_proto_gain_treasure_kill_record_end& kill_record_end_msg, int32 boss_spawn_point_template_id);
		void								req_gain_item_record(client_session* session, int32 record_index);//index小于0表示第一次请求记录 //cs-ws外部
		int32								fill_gain_item_record_end_msg(game_proto_gain_treasure_gain_item_record_end& gain_item_record_end_msg, int32 record_index);
		std::set<int32>						get_npc_spawn_point_template_id_by_map_id(int32 map_template_id);
		std::set<int32>						get_npc_spawn_point_template_id_by_map_type(e_map_type map_type);

		s_gain_treasure_boss_info*			get_gain_treasure_boss_info_by_id(int32 boss_spawn_point_template_id);
		void								update_gain_treasure_boss_info(const s_gain_treasure_boss_info& boss_info); //cs-ws外部
		void								send_gain_treasure_boss_info_one(client_session* session, int32 boss_spawn_point_template_id);
		void								send_gain_treasure_boss_info_all(client_session* session); //外部
		int32								fill_boss_info_one_msg(game_proto_gain_treasure_boss_info_one& boss_info_one_msg, int32 boss_spawn_point_template_id);
		void								send_gain_treasure_boss_to_all(int32 boss_spawn_point_template_id);
		void								send_gain_treasure_boss_to_map(int32 boss_spawn_point_template_id);
		//rename
		void								change_player_name_func(const guid_64& role_guid, const xstring& role_name, const xstring& original_name);	//角色改名同步外部
		bool                                is_use_lua();

	private:
		gain_treasure_record_lists_type						m_record_map; 
		std::list<s_gain_treasure_gain_item_record_info>	m_gain_item_record_list;
		gain_treasure_boss_infos_type						m_boss_info_map;
	};
}

#endif