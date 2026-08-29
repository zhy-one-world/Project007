#ifndef _ASSIST_FIGHT_CS_MGR_H_
#define _ASSIST_FIGHT_CS_MGR_H_

namespace faith
{
	class assist_fight_cs_mgr
	{
	public:
		assist_fight_cs_mgr();
		~assist_fight_cs_mgr();
	public:
		s_assist_fight_info&        get_assist_fight_info() { return m_assist_fight_info; }
		void						clear_assist_fight_guid() { m_assist_fight_info.clear_data(); }
		guid_64						get_assist_fight_guid() { return m_assist_fight_info.asssit_fight_guid; }
		void						set_boss_id(guid_64 boss_id) { m_assist_fight_info.boss_guid = boss_id; }
		void						clear_assist_fight_boss_id() { m_assist_fight_info.boss_guid.clear_data(); }
		guid_64						get_assist_fight_boss_id() { return m_assist_fight_info.boss_guid; }
		//bool						check_can_assist_fight(int32 assist_fight_tag, int32 map_template_id, guid_64 map_guid);
		void						go_to_assist();
		void						check_assist_fight_boss_island();
		void						update_assist_fight_info(s_assist_fight_info m_info);
		void						cancel_assist_fight(bool need_transfer = true);
		//void						req_assist_fight(guid_64 assist_fight_guid);
		bool						is_assist_helper();
		bool						is_assist_main_player();
		void						start_assist_fight(int32 assist_fight_type,int32 assist_fight_tag);
		void						assist_fight_end_to_show_reward(std::vector<guid_64>& assist_fight_helper_list, int32 reward_id1, std::vector<int32>& reward_id2, const std::vector< s_item_template_info >& drop_id_list ,xstring first_name = "");
		void                        set_unit_index(int32 array_index) { m_array_index = array_index; }
		int32						get_assist_type() { return m_assist_fight_info.assist_fight_type; }
		bool						check_cant_join_team();
		bool						is_multi_map_assist_fight() { return is_assist_main_player() && m_assist_fight_info.assist_fight_tag == 1;}
		void						clear_data();
		void						check_leave_assist_map();
		void						check_enter_assist_map();
		void						sync_assist_fight_state(bool is_login = false);
		void						remove_assist_helper(guid_64 helper_guid);
		bool						is_in_assist_fight() { return m_assist_fight_info.asssit_fight_guid.is_valid();}
		void						check_assist_fight_legion_team();
		void						sync_assist_fight_state_to_all_mem();
	private:
		int32 m_array_index;
		s_assist_fight_info m_assist_fight_info;
	};
}
#endif