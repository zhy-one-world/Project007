#ifndef _WS_CLIENT_ACTIVITY_RECORD_H_
#define _WS_CLIENT_ACTIVITY_RECORD_H_

#include "Logic/best_record_def.hpp"
#include <activity_msg.hpp>
#include "ws_client.hpp"


namespace faith
{
	class best_record_mgr
	{
	public:
		static best_record_mgr& get_instance()
		{
			static best_record_mgr instance;
			return instance;
		}

		void					save_best_record_to_db(const s_best_record& best_record);
		void					load_all_best_record_from_db();

		s_best_record*			get_record_by_template_id(int32 map_template_id);
		s_best_record*			create_record_by_template_id(int32 map_template_id);
		int32					get_order_type_by_template_id(int32 map_template_id);
		int32					get_convert_map_template_id(int32 map_template_id);	//活动副本中最好记录改为世界统一,统一设置map_template_id

		int32					get_insert_index(s_best_record* best_record, const s_single_map_record& new_record, int32 order_type);
		bool					judget_if_can_add(s_best_record* best_record, const s_single_map_record& new_record);
		void					insert_record(s_best_record* best_record, const s_single_map_record& new_record, bool is_from_db = false);
		void					del_record(s_best_record* best_record, const s_single_map_record& new_record);

		void					set_best_record(int32 map_template_id, s_single_map_record new_record, bool is_from_db = false);
		void					notice_best_record(int32 map_template_id, const s_single_map_record& best_record);
		bool					is_multiple_raid(e_map_order_type map_type);
		void				    team_best_record_role_name_replace(int32 map_template_id, s_single_map_record& best_record);
		std::string				get_map_difficulty_notice_id(int32 map_template_id);

		//rename
		void					change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步

		bool                    is_use_lua();

	private:
		std::vector<s_best_record>			m_activity_record_list;
		float								m_save_interval = 0.0f;
		float								m_sec_tick = 0.0f;
		bool								m_is_saving = false;
		int32								m_cur_saving_index = -1;
		int32								m_max_saving_count = -1;
	};
}

#endif