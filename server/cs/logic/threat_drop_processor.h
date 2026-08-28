
#ifndef __THREAT_DROP_HELPER_H__
#define __THREAT_DROP_HELPER_H__

#include "logic/type_def.hpp"
#include "Logic/ai_def.hpp"
#include "logic/player.hpp"
#include "logic/npc.hpp"

namespace hld
{
	class player_score_info
	{
	public:
		player_score_info() {
			clear_data();
		}

		void clear_data() {
			m_player_ptr = nullptr;
			m_score = 0;
		}

		void init_data(player* player_ptr, int32 player_score) {
			set_player(player_ptr);
			set_score(player_score);
		}

		bool is_non_nullptr() {
			if (nullptr == m_player_ptr)
			{
				return false;
			}
			return true;
		}

		player* get_player_ptr() {
			return m_player_ptr;
		}

		int32 get_score() {
			return m_score;
		}

	private:
		void set_player(player* player_ptr) {
			m_player_ptr = player_ptr;
		}

		void set_score(int32 player_score) {
			m_score = player_score;
		}

	private:
		player* m_player_ptr;
		int32   m_score;
	};

	class team_score_info
	{
	public:
		team_score_info() {
			clear_data();
		}

		void clear_data() {
			m_team_guid = 0;
			m_team_score = 0;
			m_team_member_array.clear();
		}

		void init_data(guid_64 team_guid) {
			m_team_guid = team_guid;
		}

		bool	is_non_nullptr() {
			return m_team_guid.is_valid();
		}

		int32	get_team_score() {
			return	m_team_score;
		}

		guid_64 get_team_guid() {
			return m_team_guid;
		}

		int32	get_member_num() {
			return m_team_member_array.size();
		}

		player* get_member_ptr(int32 member_index) {
			if (member_index < 0 || member_index >= get_member_num())
			{
				return nullptr;
			}
			return m_team_member_array[member_index].get_player_ptr();
		}

		void add_player_member(player_score_info player_info) {
			push_back_player_info(player_info);
			add_score(player_info.get_score());
		}

		bool has_this_player(guid_64& player_guid) {
			int32 size = get_member_num();
			for (int32 i = 0; i < size; i++)
			{
				player* temp_player_ptr = get_member_ptr(i);
				if (nullptr == temp_player_ptr)
				{
					continue;
				}
				if (player_guid == temp_player_ptr->get_unit_guid())
				{
					return true;
				}
			}
			return false;
		}

	private:
		void push_back_player_info(player_score_info player_info) {
			m_team_member_array.push_back(player_info);
		}

		void add_score(int32 player_score) {
			m_team_score += player_score;
		}

	private:
		guid_64	m_team_guid;
		int32	m_team_score;
		std::vector<player_score_info>	m_team_member_array;
	};

	class drop_manager
	{
	private:
		static void clear_data();
	public:
		static void create_drop(NpcTemplate* npc_template_ptr,
			Entity* map_ent,
			const s_map_pos& init_pos,
			const int32& first_attacker_index,
			const int32& last_attacker_index,
			const hate_info_vector& hate_list,
			const damage_info_vector& damage_list,
			const int32& npc_array_index,
			const bool& is_use_all_player_drop = false);
		static void init_team_score_info_ex(const hate_info_vector& hate_list, std::vector<team_score_info>&);

	private:
		static void init_drop(std::vector<int32>& drop_list, const hate_info_vector& hate_list, const damage_info_vector& damage_list);
		static void init_player_score_info(const hate_info_vector& hate_list, std::vector<player_score_info>&);
		static void init_team_score_info(const hate_info_vector& hate_list, std::vector<team_score_info>&);

		static void init_player_score_info(const damage_info_vector& damage_list, std::vector<player_score_info>&);
		static void init_team_score_info(const damage_info_vector& damage_list, std::vector<team_score_info>&);
		static void init_team_score_info_ex(const damage_info_vector& damage_list, std::vector<team_score_info>&);


		static void add_score_info(player* player_ptr, const int32& player_score,std::vector<player_score_info>& player_score_info_array, std::vector<team_score_info>& team_score_info_array);
		static void add_player_score_info_array(player_score_info& new_player_score_info, std::vector<player_score_info>& player_score_info_array);
		static void add_team_score_info_array(player_score_info& new_player_score_info, std::vector<team_score_info>& team_score_info_array);
		static void team_score_info_array_sort(std::vector<team_score_info>& team_score_info_array);
		static void clear_zero_score_team(std::vector<team_score_info>& team_score_info_array);
		static void find_other_team_member_in_aoi(std::vector<team_score_info>& team_score_info_array);

		static void distribution_by_player(int32& drop_template_id, player* player_ptr, const hate_info_vector& hate_list);
		static void distribution_by_list(int32& drop_template_id, int32 rule_type, const hate_info_vector& hate_list, const damage_info_vector& damage_list);

		static void distribution_to_personal(int32& drop_template_id, player* player_ptr, int32& drop_type);
		static void distribution_to_team(int32& drop_template_id, team_score_info& team_info, int32& distribute_type, int32& drop_type);
		static void distribution_to_team_ex(int32& drop_template_id, team_score_info& team_info, int32& distribute_type, int32& drop_type);

		static bool is_same_class(int32& drop_template_id, player* player_ptr);
	private:
		static void create_drop_box(int32& drop_template_id, player* player_ptr, int32& drop_type);
		static void create_drop_box(std::vector< s_item_template_info >& item_list, player* player_ptr, int32& drop_type, int32& drop_template_id, bool cost_time = false);
	public:
		static void create_drop_for_boss_damage_ranking(npc& boss_ref, int32 ranking, int32 drop_template_id, std::vector<guid_64>& team_info);
		static void create_drop_box(int32& drop_template_id, player* player_ptr, s_map_pos& drop_pos, bool is_need_show_get_item_new = false);
		static std::vector< s_item_template_info > create_drop_box_by_drop_template_id(int32& drop_template_id, player* player_ptr, Entity* map_ent, s_map_pos& drop_pos, int32 npc_template_id, bool cost_time = false);
	private:
		static team_score_info get_team_score_info(guid_64& team_guid);
		static void send_notice();
		static void clear_score_array();
	};

}

#endif //__THREAT_DROP_HELPER_H__
