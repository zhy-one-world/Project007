#pragma once

#include "logic/type_def.hpp"
#include "template/ServerTemplateHead.h"
#include "time.hpp"

namespace faith
{
	class csv_row;

    class template_manager
    {
    public:
        static template_manager& get_instance()
        {
            static template_manager instance;
            return instance;
        }
    public:
        void init();
		void init_for_dp();
        typedef std::map<int32, ui8*> template_type;

    public:
        template<class T>
        T* get_template(e_template template_name, int32 id)
		{
			ZoneScoped;
			if (template_name < 0 || template_name >= e_template_max)
			{
				return nullptr;
			}
			if (id <= 0)
			{
				return nullptr;
			}

			int64 time_now = utility::get_tick_count();
			int64 time_old = 0;
			template_type& template_data = template_map[template_name];
            auto item_iter = template_data.find(id);
            if (item_iter == template_data.end())
            {
                return nullptr;
			}
			time_old = utility::get_tick_count();
			if (time_now - time_old > 0)
			{
				std::cout << __FUNCTION__ << " " << __LINE__ << time_now - time_old << " "<< template_name  <<""<< id << std::endl;
			}
            return (T*)(item_iter->second);
        }

        template_type* get_templates(e_template template_name)
		{
			ZoneScoped;
			if (template_name < 0 || template_name >= e_template_max)
			{
				return nullptr;
			}
            return &(template_map[template_name]);
        }

		size_t get_template_row_size(e_template template_name)
		{
			ZoneScoped;
			if (template_name < 0 || template_name >= e_template_max)
			{
				return 0;
			}
			return template_map[template_name].size();
		}
		void										remove_template(e_template template_name);

	public:
		GameConfigTemplate*							get_game_config_template() { return m_game_config_template; }
		SpecialNameTemplate*						get_template_by_type_and_level_and_star(int32 name_type, int32 name_level, int32 name_star);
		StoreTemplate*								get_store_template_by_id(int32 store_id);
		DailyActivitiesTemplate*					get_template_by_daily_activity_type(int32 activity_typ, int32 player_exp_level);
		std::vector<DailyActivitiesTemplate*>		get_template_array_by_daily_activity_type(int32 activity_typ, int32 player_exp_level);
		VIPTemplate*								get_template_by_vip_level(int32 vip_level);
		MissionChapterTemplate*						get_template_by_mission_chapter_index(int32 index);
		MissionChapterTemplate*						get_cur_chapter_template_by_main_mission_id(int32 cur_main_main_mission_id);
		RechargeTemplate*							get_template_by_recharge_type(int32 recharge_type);
		RechargeTemplate*							get_template_by_recharge_offline_item(int32 offline_item);
		ActiveDegreeTemplate*						get_template_by_active_degree_type(int32 active_degree_type);
		int32										get_template_id_by_active_degree_type(int32 active_degree_type);
		LadderTemplate*								get_template_by_ladder_score(int32 ladder_score, int32 season = 0);
		ElementLadderTemplate*						get_template_by_element_score(int32 element_score, int32 season = 0);
		std::vector<LadderTemplate*>                get_template_array_by_ladder_score(int32 ladder_score);
		std::vector<ElementLadderTemplate*>			get_template_array_by_element_score(int32 element_score);
		std::vector<ElementLadderTemplate*>			get_template_array_by_element_min_and_max_score(int32 min_score, int32 max_score, int32 season = 0);
		std::vector<ElementLadderTaskTemplate*>     get_all_element_task_template_array();
		std::vector<ElementLadderTaskTemplate*>     get_all_element_task_template_array_by_mission_type(int32 mission_type);
		RewardSettlementTemplate*					get_template_by_activity_score(e_activity_type type, int64  npc_score);
		PlayerUpgradeTemplate*						get_player_upgrade_by_level(int32 unit_type, int32 exp_level);
		MoneyTemplate*								get_money_template_by_type(int32 money_type);
		ArenaRankRewardTemplate*					get_arena_reward_by_arena_pos(int32 arena_pos);
		MapTemplate*								get_map_template_map_id(const int32 map_template_id);
		WelfareTemplate*							get_welfare_template_ptr_by_type_and_target(const int32 welfare_type, const int32 welfare_target_value);
		std::vector<WelfareTemplate*>				get_welfare_template_ptr_array_by_type(const int32 welfare_type, const int32 welfare_target_value);
		ItemTemplate*								get_item_template_ptr_by_template_id(const int32 template_id);
		ItemTemplate*								get_item_template_ptr_by_logic_id(const int32 logic_id);
		ConvertTemplate*							get_convert_template_by_type(int32 convert_type);
		FuncUnlockTemplate*							get_func_unlock_template(const int32 func_template_id);
		DailyActivitiesTemplate*					get_template_by_daily_active_degree_type(int32 active_degree_type);
		RankingShowTemplate*						get_ranking_template_by_npc_id(int32 ranking_npc_id);
		RankingShowTemplate*						get_ranking_template_by_ranking_type(e_RankingIndex ranking_type);
		InteractionTemplate*						get_interaction_template(int32 interaction_type);
		WaveTemplate*								get_wave_template(BrushStrangeTemplate* brush_temp_ptr, int32 wave_id);
		std::vector<int32>&							get_excellent_target_color_array(EquipTemplate* equip_template_ptr, int32 target_color);
		std::vector<int32>&							get_spirit_excellent_target_color_array(SpiritTemplate* spirit_template_ptr, int32 target_color);
		TimeLimitActivityTemplate*					get_time_limit_activity_template_ptr(int32 template_id);						
		TimeLimitActivityBranchTemplate*			get_time_limit_activity_branch_template_ptr(int32 template_id);					
		EquipEnchantTemplate*                       get_equip_enchant_template_ptr(int32 level, int32 type, int32 slot);
		CompetitionSeasonTemplate*					get_template_by_competition_level(int32 level, int64 world_level);
		ElementCompetitionTemplate*					get_template_by_element_competition_level(int32 level, int64 world_level);
		std::vector<BreakWillTemplate*>				get_break_will_template_ptr_type_array();
	public:
		std::string									get_str_id_by_notice_id(int32 notice_id);		
		bool										get_vec_str_by_notice_id(std::vector<std::string>& str_list, int32 notice_id);		
		bool										get_str_by_attribute_id(std::string& str, int32 attributed_id);					
		const std::string&							get_str_by_string_template_id(const int32 string_template_id, int32 index=0);
		std::string									get_second_str_by_string_template_id(const int32 string_template_id);				
		std::string									float_to_string(float num);
		std::vector<std::pair<int32, ui8*>>&		get_grade_qiyuan_templates();
		std::vector<std::pair<int32, ui8*>>&		get_flair_qiyuan_templates();
		const std::vector<std::string>&				get_string_list(const int32 string_template_id);
		const std::string&							get_target_sensitive_text(const int32 word_template_id);
		int32										get_competition_first_id(int64 world_level);
		int32										get_element_competition_first_id(int64 world_level);
		std::string									get_all_string_context(const int32 string_template_id, std::string separative_sign = ",");
	public:
		bool                                        is_message_use_lua(int32 head);
	private:
		template_manager();
		template<class T>
		void										register_template(e_template template_name, std::string&& file_name);
		template <class T>
		void										re_struct_in_memory(template_type& template_file, e_template template_name, std::shared_ptr<csv_row> row_content, const std::vector<std::string>& result_types, const std::vector<std::string>& key_types, int32 data_size);
		template_type								template_map[e_template_max];
		std::vector<int32>							m_empty_int_array;
		std::vector<float>							m_empty_float_array;
		std::vector<std::pair<int32, ui8*>>			vec_grade_qiyuan_templates;
		std::vector<std::pair<int32, ui8*>>			vec_flair_qiyuan_templates;
		GameConfigTemplate*							m_game_config_template;	
	public:
		std::string return_empty_string;
		std::vector<std::string> return_string_list;
	};

#define proto_by_lua(Head)  template_manager::get_instance().is_message_use_lua(Head)
#define GAMECONFIG  template_manager::get_instance().get_game_config_template()
#define GET_TEMPLATE(TableName, Id)  template_manager::get_instance().get_template<##TableName>(e_##TableName,Id)

}
