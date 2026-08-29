#include "Logic/activity_def.hpp"
#include "Logic/arena_def.hpp"
#include "server_log.hpp"
#include "template/template_base.h"
#include "template/template_manager.h"
#include "utility/csv_parser.hpp"
#include "utility/globle_data.h"
#include "utility/init_unit.h"

namespace faith
{

    template_manager::template_manager()
    {
		m_game_config_template = nullptr;
    }

	void template_manager::init_for_dp()
	{
		template_size::get_instance().init_template_size_for_dp();
		std::string template_dir = R"(../res/csv/)";
		register_template<LuaMessageTemplate>(e_LuaMessageTemplate, template_dir + "LuaMessageTemplate.csv");
	}

    void template_manager::init()    
	{
		template_size::get_instance().init_template_size();

		std::string version_dir = R"(../res/)";
		register_template<VersionTemplate>(e_VersionTemplate, version_dir + "VersionTemplate.csv");
		std::string template_dir = R"(../res/csv/)";
		register_template<RegionTemplate>(e_RegionTemplate, template_dir + "RegionTemplate.csv");
		register_template<LuaMessageTemplate>(e_LuaMessageTemplate, template_dir + "LuaMessageTemplate.csv");
		register_template<VIPTemplate>(e_VIPTemplate, template_dir + "VIPTemplate.csv");
		//register_template<StringTemplate>(e_StringTemplate, template_dir + "StringTemplate.csv");
		register_template<RechargeTemplate>(e_RechargeTemplate, template_dir + "RechargeTemplate.csv");
		register_template<MapTemplate>(e_MapTemplate, template_dir + "MapTemplate.csv");
		register_template<MapPlayTemplate>(e_MapPlayTemplate, template_dir + "MapPlayTemplate.csv");
		register_template<MapAwardTemplate>(e_MapAwardTemplate, template_dir + "MapAwardTemplate.csv");
		register_template<FlipBoxTemplate>(e_FlipBoxTemplate, template_dir + "FlipBoxTemplate.csv");
		register_template<BoxMapTemplate>(e_BoxMapTemplate, template_dir + "BoxMapTemplate.csv");
		register_template<TimeActivityTemplate>(e_TimeActivityTemplate, template_dir + "TimeActivityTemplate.csv");
		register_template<PlayAddActivityTemplate>(e_PlayAddActivityTemplate, template_dir + "PlayAddActivityTemplate.csv");
		register_template<WheelPlateTemplate>(e_WheelPlateTemplate, template_dir + "WheelPlateTemplate.csv");
		register_template<RaidCommonTemplate>(e_RaidCommonTemplate, template_dir + "RaidCommonTemplate.csv");
		register_template<SearchLevelTemplate>(e_SearchLevelTemplate, template_dir + "SearchLevelTemplate.csv");
		register_template<MapSafeTemplate>(e_MapSafeTemplate, template_dir + "MapSafeTemplate.csv");
		register_template<PlayerInitTemplate>(e_PlayerInitTemplate, template_dir + "PlayerInitTemplate.csv");
		register_template<PlayerUpgradeTemplate>(e_PlayerUpgradeTemplate, template_dir + "PlayerUpgradeTemplate.csv");
		register_template<NpcTemplate>(e_NpcTemplate, template_dir + "NpcTemplate.csv");
		register_template<NpcPlayTemplate>(e_NpcPlayTemplate, template_dir + "NpcPlayTemplate.csv");
		register_template<NpcSpawnPointTemplate>(e_NpcSpawnPointTemplate, template_dir + "NpcSpawnPointTemplate.csv");
		register_template<TreasurePicTemplate>(e_TreasurePicTemplate, template_dir + "TreasurePicTemplate.csv");
		register_template<BuffDataTemplate>(e_BuffDataTemplate, template_dir + "BuffDataTemplate.csv");
		register_template<BuffActionTemplate>(e_BuffActionTemplate, template_dir + "BuffActionTemplate.csv");
		register_template<BuffResourceTemplate>(e_BuffResourceTemplate, template_dir + "BuffResourceTemplate.csv");
		register_template<BeliefTemplate>(e_BeliefTemplate, template_dir + "BeliefTemplate.csv");
		register_template<ItemTemplate>(e_ItemTemplate, template_dir + "ItemTemplate.csv");
		register_template<SkillTemplate>(e_SkillTemplate, template_dir + "SkillTemplate.csv");
		register_template<SkillResTemplate>(e_SkillResTemplate, template_dir + "SkillResTemplate.csv");
		register_template<SkillBulletTemplate>(e_SkillBulletTemplate, template_dir + "SkillBulletTemplate.csv");
		register_template<BrushStrangeTemplate>(e_BrushStrangeTemplate, template_dir + "BrushStrangeTemplate.csv");
		register_template<WaveTemplate>(e_WaveTemplate, template_dir + "WaveTemplate.csv");
		register_template<ItemUpgradeTemplate>(e_ItemUpgradeTemplate, template_dir + "ItemUpgradeTemplate.csv");
		register_template<ItemAddonTemplate>(e_ItemAddonTemplate, template_dir + "ItemAddonTemplate.csv");
		register_template<MissionTemplate>(e_MissionTemplate, template_dir + "MissionTemplate.csv");
		register_template<MissionLibraryTemplate>(e_MissionLibraryTemplate, template_dir + "MissionLibraryTemplate.csv");
		register_template<WingAddSoulTemplate>(e_WingAddSoulTemplate, template_dir + "WingAddSoulTemplate.csv");
		register_template<MountTemplate>(e_MountTemplate, template_dir + "MountTemplate.csv");
		register_template<WingAddSpiritTemplate>(e_WingAddSpiritTemplate, template_dir + "WingAddSpiritTemplate.csv");
		register_template<FeatherTemplate>(e_FeatherTemplate, template_dir + "FeatherTemplate.csv");
		register_template<DropTemplate>(e_DropTemplate, template_dir + "DropTemplate.csv");
		register_template<EquipTemplate>(e_EquipTemplate, template_dir + "EquipTemplate.csv");
		register_template<MoneyTemplate>(e_MoneyTemplate, template_dir + "MoneyTemplate.csv");
		register_template<GameConfigTemplate>(e_GameConfigTemplate, template_dir + "GameConfigTemplate.csv");
		register_template<BagExtendTemplate>(e_BagExtendTemplate, template_dir + "BagExtendTemplate.csv");
		register_template<SpiritTemplate>(e_SpiritTemplate, template_dir + "SpiritTemplate.csv");
		register_template<SpiritGradeQiYuanTemplate>(e_SpiritGradeQiYuanTemplate, template_dir + "SpiritGradeQiYuanTemplate.csv");
		register_template<SpiritWeightTemplate>(e_SpiritWeightTemplate, template_dir + "SpiritWeightTemplate.csv");
		register_template<ArenaRankRewardTemplate>(e_ArenaRankRewardTemplate, template_dir + "ArenaRankRewardTemplate.csv");
		register_template<SpecialNameTemplate>(e_SpecialNameTemplate, template_dir + "SpecialNameTemplate.csv");
		register_template<SpiritFlairQiYuanTemplate>(e_SpiritFlairQiYuanTemplate, template_dir + "SpiritFlairQiYuanTemplate.csv");
		register_template<StoreTemplate>(e_StoreTemplate, template_dir + "StoreTemplate.csv");
		register_template<GoodsTemplate>(e_GoodsTemplate, template_dir + "GoodsTemplate.csv");
		register_template<WordTemplate>(e_WordTemplate, template_dir + "WordTemplate.csv");
		register_template<LadderTemplate>(e_LadderTemplate, template_dir + "LadderTemplate.csv");
		register_template<TalentTemplate>(e_TalentTemplate, template_dir + "TalentTemplate.csv");
		register_template<RobotTalkTemplate>(e_RobotTalkTemplate, template_dir + "RobotTalkTemplate.csv");
		register_template<TalentListTemplate>(e_TalentListTemplate, template_dir + "TalentListTemplate.csv");
		register_template<LadderRewarPreviewTemplate>(e_LadderRewarPreviewTemplate, template_dir + "LadderRewarPreviewTemplate.csv");
		register_template<RewardSettlementTemplate>(e_RewardSettlementTemplate, template_dir + "RewardSettlementTemplate.csv");
		register_template<AdditionBuffTemplate>(e_AdditionBuffTemplate, template_dir + "AdditionBuffTemplate.csv");
		register_template<AttributeTemplate>(e_AttributeTemplate, template_dir + "AttributeTemplate.csv");
		register_template<SpiritJiBanTemplate>(e_SpiritJiBanTemplate, template_dir + "SpiritJiBanTemplate.csv");
		register_template<PropBasicAttributeTemplate>(e_PropBasicAttributeTemplate, template_dir + "PropBasicAttributeTemplate.csv");
		register_template<CompositTemplate>(e_CompositTemplate, template_dir + "CompositTemplate.csv");
		register_template<ItemSuccinctTemplate>(e_ItemSuccinctTemplate, template_dir + "ItemSuccinctTemplate.csv");
		register_template<ElementHeartTemplate>(e_ElementHeartTemplate, template_dir + "ElementHeartTemplate.csv");
		register_template<ElementHeartGroupTemplate>(e_ElementHeartGroupTemplate, template_dir + "ElementHeartGroupTemplate.csv");
		register_template<ElementHeartUpgradeTemplate>(e_ElementHeartUpgradeTemplate, template_dir + "ElementHeartUpgradeTemplate.csv");
		register_template<AchievementTemplate>(e_AchievementTemplate, template_dir + "AchievementTemplate.csv");
		register_template<WingTemplate>(e_WingTemplate, template_dir + "WingTemplate.csv");
		register_template<PokedexTemplate>(e_PokedexTemplate, template_dir + "PokedexTemplate.csv");
		register_template<PsycheTemplate>(e_PsycheTemplate, template_dir + "PsycheTemplate.csv");
		register_template<NoticeTemplate>(e_NoticeTemplate, template_dir + "NoticeTemplate.csv");
		register_template<ProtectTemplate>(e_ProtectTemplate, template_dir + "ProtectTemplate.csv");
		register_template<ProtectSpiritTemplate>(e_ProtectSpiritTemplate, template_dir + "ProtectSpiritTemplate.csv");
		register_template<DailyActivitiesTemplate>(e_DailyActivitiesTemplate, template_dir + "DailyActivitiesTemplate.csv");
		register_template<TitleTemplate>(e_TitleTemplate, template_dir + "TitleTemplate.csv");
		register_template<EquipEnchantTemplate>(e_EquipEnchantTemplate, template_dir + "EquipEnchantTemplate.csv");
		register_template<DeityTemplate>(e_DeityTemplate, template_dir + "DeityTemplate.csv");
		register_template<ActiveDegreeTemplate>(e_ActiveDegreeTemplate, template_dir + "ActiveDegreeTemplate.csv");
		register_template<WelfareTemplate>(e_WelfareTemplate, template_dir + "WelfareTemplate.csv");
		register_template<GrowUpFundTemplate>(e_GrowUpFundTemplate, template_dir + "GrowUpFundTemplate.csv");
		register_template<LegionConstructionsTemplate>(e_LegionConstructionsTemplate, template_dir + "LegionConstructionsTemplate.csv");
		register_template<ArenaRobotTemplate>(e_ArenaRobotTemplate, template_dir + "ArenaRobotTemplate.csv");
		register_template<OutboardTemplate>(e_OutboardTemplate, template_dir + "OutboardTemplate.csv");
		register_template<RedPackageTemplate>(e_RedPackageTemplate, template_dir + "RedPackageTemplate.csv");
		register_template<UIHideTemplate>(e_UIHideTemplate, template_dir + "UIHideTemplate.csv");
		register_template<FuncUnlockTemplate>(e_FuncUnlockTemplate, template_dir + "FuncUnlockTemplate.csv");
		register_template<GuideTriggerTemplate>(e_GuideTriggerTemplate, template_dir + "GuideTriggerTemplate.csv");
		register_template<GuideTemplate>(e_GuideTemplate, template_dir + "GuideTemplate.csv");
		register_template<MissionChapterTemplate>(e_MissionChapterTemplate, template_dir + "MissionChapterTemplate.csv");
		register_template<ActivityCommonConfigTemplate>(e_ActivityCommonConfigTemplate, template_dir + "ActivityCommonConfigTemplate.csv");
		register_template<CityWarTerritoryTemplate>(e_CityWarTerritoryTemplate, template_dir + "CityWarTerritoryTemplate.csv");
		register_template<TeamAimTypeTemplate>(e_TeamAimTypeTemplate, template_dir + "TeamAimTypeTemplate.csv");
		register_template<ServiceGoalTemplate>(e_ServiceGoalTemplate, template_dir + "ServiceGoalTemplate.csv");
		register_template<RobotItemsTemplate>(e_RobotItemsTemplate, template_dir + "RobotItemsTemplate.csv");
		register_template<ConvertTemplate>(e_ConvertTemplate, template_dir + "ConvertTemplate.csv");
		register_template<LegionSkillTemplate>(e_LegionSkillTemplate, template_dir + "LegionSkillTemplate.csv");
		register_template<StopPlaceTemplate>(e_StopPlaceTemplate, template_dir + "StopPlaceTemplate.csv");
		register_template<RankingShowTemplate>(e_RankingShowTemplate, template_dir + "RankingShowTemplate.csv");
		register_template<FasionTemplate>(e_FasionTemplate, template_dir + "FasionTemplate.csv");
		register_template<InteractionTemplate>(e_InteractionTemplate, template_dir + "InteractionTemplate.csv");
		register_template<LegionSalaryTemplate>(e_LegionSalaryTemplate, template_dir + "LegionSalaryTemplate.csv");
		register_template<TinderTemplate>(e_TinderTemplate, template_dir + "TinderTemplate.csv");
		register_template<ServiceRankTemplate>(e_ServiceRankTemplate, template_dir + "ServiceRankTemplate.csv");
		register_template<BeastSpiritTemplate>(e_BeastSpiritTemplate, template_dir + "BeastSpiritTemplate.csv");
		register_template<JewelTemplate>(e_JewelTemplate, template_dir + "JewelTemplate.csv");
		register_template<RoleRewardTemplate>(e_RoleRewardTemplate, template_dir + "RoleRewardTemplate.csv");
		register_template<TimeLimitActivityTemplate>(e_TimeLimitActivityTemplate, template_dir + "TimeLimitActivityTemplate.csv");
		register_template<TimeLimitActivityBranchTemplate>(e_TimeLimitActivityBranchTemplate, template_dir + "TimeLimitActivityBranchTemplate.csv");
		register_template<OperatorUseActivityTemplate>(e_OperatorUseActivityTemplate, template_dir + "OperatorUseActivityTemplate.csv");
		//register_template<TLATotalCostTemplate>(e_TLATotalCostTemplate, template_dir + "TLATotalCostTemplate.csv");
		//register_template<TLADailyTimeWelfareTemplate>(e_TLADailyTimeWelfareTemplate, template_dir + "TLADailyTimeWelfareTemplate.csv");
		//register_template<TLATotalRechargeTemplate>(e_TLATotalRechargeTemplate, template_dir + "TLATotalRechargeTemplate.csv");
		//register_template<TLASuperEverydayTemplate>(e_TLASuperEverydayTemplate, template_dir + "TLASuperEverydayTemplate.csv");
		register_template<PassiveSkillTemplate>(e_PassiveSkillTemplate, template_dir + "PassiveSkillTemplate.csv");
		register_template<MailTemplate>(e_MailTemplate, template_dir + "MailTemplate.csv");
		register_template<BeliefRunesTemplate>(e_BeliefRunesTemplate, template_dir + "BeliefRunesTemplate.csv");
		register_template<BeliefRunesUpgradeTemplate>(e_BeliefRunesUpgradeTemplate, template_dir + "BeliefRunesUpgradeTemplate.csv");
		register_template<QuestionBankTemplate>(e_QuestionBankTemplate, template_dir + "QuestionBankTemplate.csv");
		//register_template<FromWhatPeriodTemplate>(e_FromWhatPeriodTemplate, template_dir + "FromWhatPeriodTemplate.csv");
		register_template<PromptBoxTemplate>(e_PromptBoxTemplate, template_dir + "PromptBoxTemplate.csv");
		//register_template<TLABehaviorAwardTemplate>(e_TLABehaviorAwardTemplate, template_dir + "TLABehaviorAwardTemplate.csv");
		//register_template<TLABehaviorTemplate>(e_TLABehaviorTemplate, template_dir + "TLABehaviorTemplate.csv");
		//register_template<TLATreasureAwardTemplate>(e_TLATreasureAwardTemplate, template_dir + "TLATreasureAwardTemplate.csv");
		register_template<GradeGodHoodTemplate>(e_GradeGodHoodTemplate, template_dir + "GradeGodHoodTemplate.csv");
		register_template<GodHeartTemplate>(e_GodHeartTemplate, template_dir + "GodHeartTemplate.csv");
		register_template<GodHeartUpgradeTemplate>(e_GodHeartUpgradeTemplate, template_dir + "GodHeartUpgradeTemplate.csv");
		register_template<IllusionTemplate>(e_IllusionTemplate, template_dir + "IllusionTemplate.csv");
		register_template<WeddingRingTemplate>(e_WeddingRingTemplate, template_dir + "WeddingRingTemplate.csv");
		register_template<QuestionMarryTemplate>(e_QuestionMarryTemplate, template_dir + "QuestionMarryTemplate.csv");
		register_template<PersonalSignatureTemplate>(e_PersonalSignatureTemplate, template_dir + "PersonalSignatureTemplate.csv");
		register_template<CompetitionSeasonTemplate>(e_CompetitionSeasonTemplate, template_dir + "CompetitionSeasonTemplate.csv");
		register_template<RecycleTemplate>(e_RecycleTemplate, template_dir + "RecycleTemplate.csv");
		register_template<OverloadCityWarRewarTemplate>(e_OverloadCityWarRewarTemplate, template_dir + "OverloadCityWarRewarTemplate.csv");
		register_template<AwakenTemplate>(e_AwakenTemplate, template_dir + "AwakenTemplate.csv");
		register_template<DragonTripTemplate>(e_DragonTripTemplate, template_dir + "DragonTripTemplate.csv");
		register_template<MountEquipTemplate>(e_MountEquipTemplate, template_dir + "MountEquipTemplate.csv");
		register_template<MountEquipTemplate>(e_ElementLadderTemplate, template_dir + "ElementLadderTemplate.csv");
		register_template<MountEquipTemplate>(e_ElementLadderTaskTemplate, template_dir + "ElementLadderTaskTemplate.csv");
		register_template<JewelCarveTemplate>(e_JewelCarveTemplate, template_dir + "JewelCarveTemplate.csv");
		register_template<JewelCarveTemplate>(e_ElementCompetitionTemplate, template_dir + "ElementCompetitionTemplate.csv");
		register_template<RewardByRegionTemplate>(e_RewardByRegionTemplate, template_dir + "RewardByRegionTemplate.csv");
		register_template<ChangeShapeTemplate>(e_ChangeShapeTemplate, template_dir + "ChangeShapeTemplate.csv");
		register_template<BreakWillTemplate>(e_BreakWillTemplate, template_dir + "BreakWillTemplate.csv");
		register_template<TimeLimitGiftTemplate>(e_TimeLimitGiftTemplate, template_dir + "TimeLimitGiftTemplate.csv");
		register_template<DevelopOpenTemplate>(e_DevelopOpenTemplate, template_dir + "DevelopOpenTemplate.csv");
		register_template<ResultsPreviewTemplate>(e_ResultsPreviewTemplate, template_dir + "ResultsPreviewTemplate.csv");
		register_template<ResultsPreviewTemplate>(e_AwakenFetterTemplate, template_dir + "AwakenFetterTemplate.csv");

		m_game_config_template = get_template<GameConfigTemplate>(e_GameConfigTemplate, 99000001);
		if (nullptr == m_game_config_template)
		{
			CONSOLE_ERROR("m_game_config_template is null");
			return;
		}
    }

    static std::string get_item_type(std::string& item)
    {
        if (item == "int32" || item == "int64" || item == "float" || item == "string" || item == "vector<int32>" || item == "vector<float>" || item == "vector<string>")
        {
            return item;
        }
        else
        {
            return "";
        }
    }

    static std::vector<std::string> get_struct_type(e_template template_name)
    {
		std::vector<std::string> result_types;
		if (template_name < 0 || template_name >= e_template_max)
		{
			return result_types;
		}
        auto iter = template_size::get_instance().template_size_map[template_name];
        
        for (auto& item : iter)
        {
            std::string type_string = get_item_type(item);
			if (type_string == "")
			{
				if (item == "template_base")
				{
					result_types.push_back("int32");
				}
			}
			else
			{
				result_types.push_back(type_string);
			}
        }
        return result_types;
    }

    template <class T>
    void template_manager::register_template(e_template template_name, std::string&& file_name)
   {
        faith::csv_parser parser;
        parser.load(file_name);
        int32 row_num = parser.get_row_count();
		if (row_num <= 0)
		{
			CONSOLE_ERROR("template_manager::register_template row_num <= 0 file_name:{}", file_name);
			return;
		}
        template_type& template_item = template_map[template_name];
		std::vector<std::string> result_types = get_struct_type(template_name);
		const std::vector<std::string>& key_types = template_size::get_instance().template_key_map[template_name];
		if (result_types.size() != key_types.size())
		{
			CONSOLE_ERROR("template_manager::register_template result_types.size() != key_types.size() file_name:{} result_types:{} key_types:{}", file_name, result_types.size(), key_types.size());
			return;
		}

		int32 data_size = sizeof(T);
        for (int32 i = 0; i < row_num; ++i)
		{
			auto row_content = parser.get_row(i);
			re_struct_in_memory<T>(template_item, template_name, row_content, result_types, key_types, data_size);
        }
    }

	void template_manager::remove_template(e_template template_name)
	{
		if (template_name < 0 || template_name >= e_template_max)
		{
			return;
		}
		template_type& template_data = template_map[template_name];
		for (template_type::iterator it = template_data.begin(); it != template_data.end(); ++it)
		{
			delete[] it->second;
		}
		template_data.clear();
	}
    template <class T>
    static void  parse_string(const std::string& str, std::vector<T>* result)
    {
        if (result == nullptr)
        {
            return;
        }
        auto word_begin = str.begin();
        std::string sub_string;
		bool is_set_begin = false;
        for (auto iter = str.begin(); iter != str.end(); ++iter)
        {
            if (*iter == '(')
                continue;

			if (*iter != ',' && *iter != ')')
			{
				if (is_set_begin)
					continue;

				word_begin = iter;
				is_set_begin = true;
			}
			else
			{
				if (!is_set_begin)
					break;
				sub_string = std::string(word_begin, iter);
				T TValue;
				std::stringstream ss;
				ss << sub_string;
				ss >> TValue;
				result->push_back(TValue);
				is_set_begin = false;

				if (*iter == ')')
					break;
			}
        }
    }
	static void  parse_string_ex(const std::string& str, std::vector<std::string>* result)
	{
		if (result == nullptr)
		{
			return;
		}
		auto word_begin = str.begin();
		std::string sub_string;
		bool is_set_begin = false;
		for (auto iter = str.begin(); iter != str.end(); ++iter)
		{
			if (*iter == '(')
				continue;

			if (*iter != ',' && *iter != ')')
			{
				if (is_set_begin)
					continue;

				word_begin = iter;
				is_set_begin = true;
			}
			else
			{
				if (!is_set_begin)
					break;
				sub_string = std::string(word_begin, iter);
				int32 size_sub_string = sub_string.size();
				result->push_back(sub_string.substr(1, size_sub_string - 2));
				is_set_begin = false;

				if (*iter == ')')
					break;
			}
		}
	}

	template <class T>
	void template_manager::re_struct_in_memory(template_type& template_file, e_template template_name, std::shared_ptr<csv_row> row_content, const std::vector<std::string>& result_types, const std::vector<std::string>& key_types, int32 data_size)
	{
		static i8 memory[20000];
		memset(memory, 0, sizeof(memory));
		int32 offset = 0;
		int32 id = 0;
		int32 row_len = result_types.size();
		for (int32 i = 0; i < row_len; ++i)
		{
			const std::string& key = key_types[i];
			const std::string str = row_content->get_value(key);
			if (result_types[i] == "int32")
			{
				int32 ivalue = 0;
				//if (str != "")
				//{
				ivalue = std::atoi(str.c_str());
				//}
				memcpy(memory + offset, &ivalue, sizeof(int32));
				offset += sizeof(int32);
			}
			else if (result_types[i] == "int64")
			{
				i64 ivalue = 0;
				//if (str != "")
				//{
					ivalue = atol(str.c_str());
				//}
				memcpy(memory + offset, &ivalue, sizeof(i64));
				offset += sizeof(i64);
			}
			else if (result_types[i] == "float")
			{
				float f = 0.f;
				//if (str != "")
				//{
					f = std::atof(str.c_str());
				//}
				memcpy(memory + offset, &f, sizeof(float));
				offset += sizeof(float);
			}
			else if (result_types[i] == "string")
			{
				std::string* str_item = new(memory + offset) std::string;
				*str_item = str;
				offset += sizeof(std::string);
			}
			else if (result_types[i] == "vector<int32>")
			{
				std::vector<int32>* vect_item = new(memory + offset) std::vector<int32>;
				parse_string<int32>(str, vect_item);
				offset += sizeof(std::vector<int32>);
			}
			else if (result_types[i] == "vector<float>")
			{
				std::vector<float>* vect_item = new(memory + offset) std::vector<float>;
				parse_string<float>(str, vect_item);
				offset += sizeof(std::vector<float>);
			}
			else if (result_types[i] == "vector<string>")
			{
				std::vector<std::string>* vect_item = new(memory + offset) std::vector<std::string>;
				parse_string_ex(str, vect_item);
				offset += sizeof(std::vector<std::string>);
			}
		}
		int32 template_line_key = std::atoi(row_content->get_value(key_types[0]).c_str());
		template_type::iterator ite_find = template_file.find(template_line_key);
		if (ite_find == template_file.end())
		{

			template_file[template_line_key] = new i8[data_size + data_size / 2];
		}

		memcpy(template_file[template_line_key], memory, data_size);
	}

	SpecialNameTemplate*  template_manager::get_template_by_type_and_level_and_star(int32 name_type, int32 name_level, int32 name_star)
	{
		template_type* special_name_tmpl_table = get_templates(e_SpecialNameTemplate);
		if (nullptr == special_name_tmpl_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = special_name_tmpl_table->begin(); ite != special_name_tmpl_table->end(); ++ite)
		{
			SpecialNameTemplate* special_name_tmpl_ptr = (SpecialNameTemplate*)(ite->second);
			if (name_type == special_name_tmpl_ptr->NameType && name_level == special_name_tmpl_ptr->NameLevel && name_star == special_name_tmpl_ptr->NameStar)
			{
				return special_name_tmpl_ptr;
			}
		}
		return nullptr;
	}

	DailyActivitiesTemplate* template_manager::get_template_by_daily_activity_type(int32 activity_typ, int32 player_exp_level)
	{
		template_type* daily_template_table = get_templates(e_DailyActivitiesTemplate);
		if (nullptr == daily_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = daily_template_table->begin(); ite != daily_template_table->end(); ++ite)
		{
			DailyActivitiesTemplate* template_ptr = (DailyActivitiesTemplate*)(ite->second);
			if (template_ptr->EffectLevelRange.size() < 2)
			{
				continue;
			}
			if (template_ptr->ActivityTyp == activity_typ)
			{
				if (player_exp_level >= template_ptr->EffectLevelRange[0]
					&& player_exp_level <= template_ptr->EffectLevelRange[1])
				{
					return template_ptr;
				}
			}
		}
		return nullptr;
	}

	std::vector<DailyActivitiesTemplate*> template_manager::get_template_array_by_daily_activity_type(int32 activity_typ, int32 player_exp_level)
	{
		std::vector<DailyActivitiesTemplate*> return_vector;

		template_type* daily_template_table = get_templates(e_DailyActivitiesTemplate);
		if (nullptr == daily_template_table)
		{
			return return_vector;
		}
		template_type::iterator ite;
		for (ite = daily_template_table->begin(); ite != daily_template_table->end(); ++ite)
		{
			DailyActivitiesTemplate* template_ptr = (DailyActivitiesTemplate*)(ite->second);
			if (template_ptr->EffectLevelRange.size() < 2)
			{
				continue;
			}
			if (template_ptr->ActivityTyp == activity_typ)
			{
				if (player_exp_level >= template_ptr->EffectLevelRange[0]
					&& player_exp_level <= template_ptr->EffectLevelRange[1])
				{
					return_vector.push_back(template_ptr);
				}
			}
		}
		return return_vector;
	}

	MoneyTemplate* template_manager::get_money_template_by_type(int32 money_type)
	{
		template_type* money_template_table = get_templates(e_MoneyTemplate);
		if (nullptr == money_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = money_template_table->begin(); ite != money_template_table->end(); ++ite)
		{
			MoneyTemplate* template_ptr = (MoneyTemplate*)(ite->second);
			if (template_ptr->MoneyType == money_type)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	ArenaRankRewardTemplate* template_manager::get_arena_reward_by_arena_pos(int32 arena_pos)
	{
		int32 real_template_id = 0;
		template_type* arena_reward_template_table = get_templates(e_ArenaRankRewardTemplate);
		if (nullptr == arena_reward_template_table)
		{
			return nullptr;
		}
		if (arena_pos < 0
			|| arena_pos >= arena_reward_template_table->size())
		{
			real_template_id = arena_reward_template_start_id + arena_reward_template_table->size() - 1;
		}
		else
		{
			real_template_id = arena_reward_template_start_id + arena_pos;
		}
		ArenaRankRewardTemplate* template_ptr = get_template<ArenaRankRewardTemplate>(e_ArenaRankRewardTemplate, real_template_id);
		return template_ptr;
	}

	faith::MapTemplate* template_manager::get_map_template_map_id(const int32 map_template_id)
	{
		if (0 > map_template_id)
		{
			return nullptr;
		}
		template_type* map_template = get_templates(e_MapTemplate);
		if (nullptr == map_template)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = map_template->begin(); ite != map_template->end(); ++ite)
		{
			MapTemplate* template_ptr = (MapTemplate*)(ite->second);
			if (template_ptr->attribute_id == map_template_id)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	WelfareTemplate* template_manager::get_welfare_template_ptr_by_type_and_target(const int32 welfare_type, const int32 welfare_target_value)
	{
		template_type* welfare_templatee_table = get_templates(e_WelfareTemplate);
		if (nullptr == welfare_templatee_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = welfare_templatee_table->begin(); ite != welfare_templatee_table->end(); ++ite)
		{
			WelfareTemplate* template_ptr = (WelfareTemplate*)(ite->second);
			if (nullptr == template_ptr)
			{
				continue;
			}
			if (welfare_type == template_ptr->Type && welfare_target_value == template_ptr->TargetValue)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	std::vector<WelfareTemplate*> template_manager::get_welfare_template_ptr_array_by_type(const int32 welfare_type, const int32 welfare_target_value)
	{
		std::vector<WelfareTemplate*> return_vector;
		template_type* welfare_templatee_table = get_templates(e_WelfareTemplate);
		if (nullptr == welfare_templatee_table)
		{
			return return_vector;
		}
		template_type::iterator ite;
		for (ite = welfare_templatee_table->begin(); ite != welfare_templatee_table->end(); ++ite)
		{
			WelfareTemplate* template_ptr = (WelfareTemplate*)(ite->second);
			if (nullptr == template_ptr)
			{
				continue;
			}
			if (welfare_type == template_ptr->Type && welfare_target_value >= template_ptr->TargetValue)
			{
				return_vector.push_back(template_ptr);
			}
		}
		return return_vector;
	}

	ItemTemplate* template_manager::get_item_template_ptr_by_template_id(const int32 template_id)
	{
		if(0 > template_id)
		{
			return nullptr;
		}
		template_type* item_template = get_templates(e_ItemTemplate);
		if (nullptr == item_template)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = item_template->begin(); ite != item_template->end(); ++ite)
		{
			ItemTemplate* template_ptr = (ItemTemplate*)(ite->second);
			if (template_ptr->attribute_id == template_id)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}
	
	ItemTemplate* template_manager::get_item_template_ptr_by_logic_id(const int32 logic_id)
	{
		if (0 > logic_id)
		{
			return nullptr;
		}
		template_type* item_template = get_templates(e_ItemTemplate);
		if (nullptr == item_template)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = item_template->begin(); ite != item_template->end(); ++ite)
		{
			ItemTemplate* template_ptr = (ItemTemplate*)(ite->second);
			if (template_ptr->logic_id == logic_id)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}


	std::string template_manager::get_str_id_by_notice_id(int32 notice_id)
	{
		NoticeTemplate* notice_template_ptr = GET_TEMPLATE(NoticeTemplate, notice_id);
		if (nullptr == notice_template_ptr)
		{
			return "";
		}
		if (notice_template_ptr->ContentText.size() <= 0)
		{
			return "";
		}
		
		return int_to_string(notice_template_ptr->ContentText[0]);
	}
	
	bool template_manager::get_vec_str_by_notice_id(std::vector<std::string>& str_list, int32 notice_id)
	{
		NoticeTemplate* notice_template_ptr = GET_TEMPLATE(NoticeTemplate, notice_id);
		if (nullptr == notice_template_ptr)
		{
			return false;
		}
		if (0 >= notice_template_ptr->ContentText.size())
		{
			return false;
		}
		const std::vector<std::string>& temp_array = get_string_list(notice_template_ptr->ContentText[0]);
		str_list = temp_array;
		return true;
	}

	bool template_manager::get_str_by_attribute_id(std::string& str, int32 attributed_id)
	{
		std::string temp_string = template_manager::get_instance().get_str_by_string_template_id(attributed_id);
		str = temp_string;
		return true;
	}

	const std::string& template_manager::get_str_by_string_template_id(const int32 string_template_id, int32 index)
	{
		if (index <= -1)
		{
			return return_empty_string;
		}
		const std::vector<std::string>& temp_array_list = get_string_list(string_template_id);
		if (temp_array_list.size() <= index)
		{
			return return_empty_string;
		}
		return temp_array_list[index];

	}
	std::string template_manager::get_second_str_by_string_template_id(const int32 string_template_id)
	{
		const std::vector<std::string>& temp_array = get_string_list(string_template_id);
		if (temp_array.size() <= 0)
		{
			return "";
		}
		if (temp_array.size() < 2)
		{
			return "";
		}
		return temp_array[1];
	}
	std::string template_manager::int_to_string(int64 num)
	{
		std::string temp_string;
		std::stringstream temp_string_stream;
		temp_string_stream << num;
		temp_string_stream >> temp_string;
		return temp_string;
	}

	std::string template_manager::float_to_string(float num)
	{
		std::string temp_string;
		std::stringstream temp_string_stream;
		temp_string_stream << num;
		temp_string_stream >> temp_string;
		return temp_string;
	}

	std::vector<std::pair<int32, i8*>>& template_manager::get_grade_qiyuan_templates()
	{
		if (vec_grade_qiyuan_templates.empty())
		{
			template_type* p_table = get_templates(e_SpiritGradeQiYuanTemplate);
			if (nullptr == p_table)
			{
				return vec_grade_qiyuan_templates;
			}
			if (vec_grade_qiyuan_templates.size() == p_table->size())
			{
				return vec_grade_qiyuan_templates;
			}
			template_type::iterator grade_qiyuan_it_;
			for (grade_qiyuan_it_ = p_table->begin(); grade_qiyuan_it_ != p_table->end(); ++grade_qiyuan_it_)
			{
				vec_grade_qiyuan_templates.push_back(std::make_pair(grade_qiyuan_it_->first, grade_qiyuan_it_->second));
			}
			sort(vec_grade_qiyuan_templates.begin(), vec_grade_qiyuan_templates.end());
		}
		return vec_grade_qiyuan_templates;
	}

	std::vector<std::pair<int32, i8*>>& template_manager::get_flair_qiyuan_templates()
	{
		if (vec_flair_qiyuan_templates.empty())
		{
			template_type* p_table = get_templates(e_SpiritFlairQiYuanTemplate);
			if (nullptr == p_table)
			{
				return vec_flair_qiyuan_templates;
			}
			if (vec_flair_qiyuan_templates.size() == p_table->size())
			{
				return vec_flair_qiyuan_templates;
			}
			template_type::iterator flair_qiyuan_it_;
			for (flair_qiyuan_it_ = p_table->begin(); flair_qiyuan_it_ != p_table->end(); ++flair_qiyuan_it_)
			{
				vec_flair_qiyuan_templates.push_back(std::make_pair(flair_qiyuan_it_->first, flair_qiyuan_it_->second));
			}
			sort(vec_flair_qiyuan_templates.begin(), vec_flair_qiyuan_templates.end());
		}
		return vec_flair_qiyuan_templates;
	}

	const std::vector<std::string>& template_manager::get_string_list(const int32 string_template_id)
	{
		if (string_template_id <= 0)
		{
			return return_string_list;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return return_string_list;
		}
		int32 region_code = region_template_ptr->RegionCode;
		StringTemplate* string_template_ptr = GET_TEMPLATE(StringTemplate, string_template_id);
		if (string_template_ptr == nullptr || region_code < 0)
		{
			return return_string_list;
		}
		switch (region_code)
		{
		case e_version_region_type::e_version_region_type_china_main:
			return string_template_ptr->StrListCH;
			break;
		case e_version_region_type::e_version_region_type_kr:
			return string_template_ptr->StrListKR;
			break;
		case e_version_region_type::e_version_region_type_china_tw:
			return string_template_ptr->StrListTW;
			break;
		case e_version_region_type::e_version_region_type_en:
			return string_template_ptr->StrListEN;
			break;
		case e_version_region_type::e_version_region_type_tha:
			return string_template_ptr->StrListTHA;
			break;
		case e_version_region_type::e_version_region_type_vn:
			return string_template_ptr->StrListVN;
			break;
		case e_version_region_type::e_version_region_type_jp:
			return string_template_ptr->StrListJP;
			break;
		case e_version_region_type::e_version_region_type_rus:
			return string_template_ptr->StrListRUS;
			break;
		case e_version_region_type::e_version_region_type_sg:
			return string_template_ptr->StrListSG;
			break;
		default:
			return return_string_list;
			break;
		}
		return return_string_list;
	}

	const std::string& template_manager::get_target_sensitive_text(const int32 word_template_id)
	{
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return return_empty_string;
		}
		int32 region_code = region_template_ptr->RegionCode;
		WordTemplate* word_template_ptr = GET_TEMPLATE(WordTemplate, word_template_id);
		if (word_template_ptr == nullptr || region_code < 0)
		{
			return return_empty_string;
		}
		switch (region_code)
		{
		case e_version_region_type::e_version_region_type_china_main:
			return word_template_ptr->SensitiveTextCH;
			break;
		case e_version_region_type::e_version_region_type_kr:
			return word_template_ptr->SensitiveTextKR;
			break;
		case e_version_region_type::e_version_region_type_china_tw:
			return word_template_ptr->SensitiveTextTW;
			break;
		case e_version_region_type::e_version_region_type_en:
			return word_template_ptr->SensitiveTextEN;
			break;
		case e_version_region_type::e_version_region_type_tha:
			return word_template_ptr->SensitiveTextTHA;
			break;
		case e_version_region_type::e_version_region_type_vn:
			return word_template_ptr->SensitiveTextVN;
			break;
		case e_version_region_type::e_version_region_type_jp:
			return word_template_ptr->SensitiveTextJP;
			break;
		case e_version_region_type::e_version_region_type_rus:
			return word_template_ptr->SensitiveTextRus;
			break;
		case e_version_region_type::e_version_region_type_sg:
			return word_template_ptr->SensitiveTextSG;
			break;
		default:
			return return_empty_string;
			break;
		}
		return return_empty_string;
	}

	VIPTemplate* template_manager::get_template_by_vip_level(int32 vip_level)
	{
		template_type* vip_template_table = get_templates(e_VIPTemplate);
		if (nullptr == vip_template_table)
		{
			return nullptr;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = vip_template_table->begin(); ite != vip_template_table->end(); ++ite)
		{
			VIPTemplate* template_ptr = (VIPTemplate*)(ite->second);
			if (template_ptr->Level == vip_level)
			{
				for (int32 index = 0 ; index < template_ptr->RegionArr.size(); ++index)
				{
					if (template_ptr->RegionArr[index] == region_template_ptr->RegionCode)
					{
						return template_ptr;
						break;
					}
				}
			}
		}
		return nullptr;
	}

	MissionChapterTemplate* template_manager::get_template_by_mission_chapter_index(int32 index)
	{
		template_type* chapter_template_table = get_templates(e_MissionChapterTemplate);
		if (nullptr == chapter_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = chapter_template_table->begin(); ite != chapter_template_table->end(); ++ite)
		{
			MissionChapterTemplate* template_ptr = (MissionChapterTemplate*)(ite->second);
			if (template_ptr != nullptr
				&& template_ptr->ChapterNum == index)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	MissionChapterTemplate* template_manager::get_cur_chapter_template_by_main_mission_id(int32 cur_main_main_mission_id)
	{
		template_type* chapter_template_table = get_templates(e_MissionChapterTemplate);
		if (nullptr == chapter_template_table)
		{
			return nullptr;
		}
		MissionChapterTemplate* return_template_ptr = nullptr;
		template_type::iterator ite;
		for (ite = chapter_template_table->begin(); ite != chapter_template_table->end(); ++ite)
		{
			MissionChapterTemplate* template_template_ptr = (MissionChapterTemplate*)(ite->second);
			if (template_template_ptr == nullptr)
			{
				continue;
			}
			if (cur_main_main_mission_id >= template_template_ptr->FirstMissionId)
			{
				return_template_ptr = template_template_ptr;
			}
		}
		return return_template_ptr;
	}

	RechargeTemplate* template_manager::get_template_by_recharge_type(int32 recharge_type)
	{
		template_type* recharge_template_table = get_templates(e_RechargeTemplate);
		if (nullptr == recharge_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = recharge_template_table->begin(); ite != recharge_template_table->end(); ++ite)
		{
			RechargeTemplate* template_ptr = (RechargeTemplate*)(ite->second);
			if (template_ptr->Type == recharge_type)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	faith::RechargeTemplate* template_manager::get_template_by_recharge_offline_item(int32 offline_item)
	{
		template_type* recharge_template_table = get_templates(e_RechargeTemplate);
		if (nullptr == recharge_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = recharge_template_table->begin(); ite != recharge_template_table->end(); ++ite)
		{
			RechargeTemplate* template_ptr = (RechargeTemplate*)(ite->second);
			if (template_ptr->OfflineItem == offline_item)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	ActiveDegreeTemplate* template_manager::get_template_by_active_degree_type(int32 active_degree_type)
	{
		template_type* recharge_template_table = get_templates(e_ActiveDegreeTemplate);
		if (nullptr == recharge_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = recharge_template_table->begin(); ite != recharge_template_table->end(); ++ite)
		{
			ActiveDegreeTemplate* template_ptr = (ActiveDegreeTemplate*)(ite->second);
			if (template_ptr->Type == active_degree_type)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	int32 template_manager::get_template_id_by_active_degree_type(int32 active_degree_type)
	{
		template_type* recharge_template_table = get_templates(e_ActiveDegreeTemplate);
		if (nullptr == recharge_template_table)
		{
			return 0;
		}
		template_type::iterator ite;
		for (ite = recharge_template_table->begin(); ite != recharge_template_table->end(); ++ite)
		{
			ActiveDegreeTemplate* template_ptr = (ActiveDegreeTemplate*)(ite->second);
			if (template_ptr->Type == active_degree_type)
			{
				return ite->first;
			}
		}
		return 0;
	}

	StoreTemplate*  template_manager::get_store_template_by_id(int32 store_id)
	{
		template_type* store_tmpl_table = get_templates(e_StoreTemplate);
		if (nullptr == store_tmpl_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = store_tmpl_table->begin(); ite != store_tmpl_table->end(); ++ite)
		{
			StoreTemplate* store_tmpl_ptr = (StoreTemplate*)(ite->second);
			if (store_id == store_tmpl_ptr->StoreType)
			{
				return store_tmpl_ptr;
			}
		}
		return nullptr;
	}	
	
	LadderTemplate * template_manager::get_template_by_ladder_score(int32 ladder_score, int32 season)
	{
		if (season == 0)
		{
			season = init_unit::get_ladder_season();
		}
		//int32 season =
		template_type* ladder_tmpl_table = get_templates(e_LadderTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return nullptr;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return nullptr;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr)
		{
			return nullptr;
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			LadderTemplate* ladder_tmpl_ptr = (LadderTemplate*)(ite->second);
			if (ladder_score >= ladder_tmpl_ptr->LowScore && ladder_score < ladder_tmpl_ptr->HighScore && ladder_tmpl_ptr->Region == region_code &&  ladder_tmpl_ptr->SeasonNum == season)
			{
				return ladder_tmpl_ptr;
			}
		}
		return nullptr;
	}

	ElementLadderTemplate * template_manager::get_template_by_element_score(int32 element_score, int32 season)
	{
		if (season == 0)
		{
			season = init_unit::get_element_season();
		}
		template_type* element_tmpl_table = get_templates(e_ElementLadderTemplate);
		if (nullptr == element_tmpl_table)
		{
			return nullptr;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return nullptr;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == config_ptr)
		{
			return nullptr;
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = element_tmpl_table->begin(); ite != element_tmpl_table->end(); ++ite)
		{
			ElementLadderTemplate* ladder_tmpl_ptr = (ElementLadderTemplate*)(ite->second);
			if (element_score >= ladder_tmpl_ptr->LowScore && element_score < ladder_tmpl_ptr->HighScore && ladder_tmpl_ptr->RegionType == region_code &&  ladder_tmpl_ptr->SeasonNum == season)
			{
				return ladder_tmpl_ptr;
			}
		}
		return nullptr;
	}

	std::vector<LadderTemplate*>  template_manager::get_template_array_by_ladder_score(int32 ladder_score)
	{
		std::vector<LadderTemplate*> temp_vector;
		int32 season = init_unit::get_ladder_season();
		template_type* ladder_tmpl_table = get_templates(e_LadderTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return temp_vector;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return temp_vector;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr)
		{
			return temp_vector;
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			LadderTemplate* ladder_tmpl_ptr = (LadderTemplate*)(ite->second);
			if (ladder_score >= 0 && ladder_score >= ladder_tmpl_ptr->LowScore && ladder_tmpl_ptr->Region == region_code &&  ladder_tmpl_ptr->SeasonNum == season)
			{
				temp_vector.push_back(ladder_tmpl_ptr);
			}
		}
		return temp_vector;
	}

	std::vector<ElementLadderTaskTemplate*> template_manager::get_all_element_task_template_array()
	{
		std::vector<ElementLadderTaskTemplate*> temp_vector;
		template_type* ladder_tmpl_table = get_templates(e_ElementLadderTaskTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return temp_vector;
		}
		template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			ElementLadderTaskTemplate* ladder_tmpl_ptr = (ElementLadderTaskTemplate*)(ite->second);
			temp_vector.push_back(ladder_tmpl_ptr);
		}
		return temp_vector;
	}

	std::vector<ElementLadderTaskTemplate*> template_manager::get_all_element_task_template_array_by_mission_type(int32 mission_type)
	{
		std::vector<ElementLadderTaskTemplate*> temp_vector;
		template_type* ladder_tmpl_table = get_templates(e_ElementLadderTaskTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return temp_vector;
		}
		template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			ElementLadderTaskTemplate* ladder_tmpl_ptr = (ElementLadderTaskTemplate*)(ite->second);
			if (ladder_tmpl_ptr->TaskType == mission_type)
			{
				temp_vector.push_back(ladder_tmpl_ptr);
			}
		}
		return temp_vector;
	}

	std::vector<ElementLadderTemplate*> template_manager::get_template_array_by_element_score(int32 element_score)
	{
		std::vector<ElementLadderTemplate*> temp_vector;
		int32 season = init_unit::get_element_season();
		template_type* ladder_tmpl_table = get_templates(e_ElementLadderTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return temp_vector;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return temp_vector;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == config_ptr)
		{
			return temp_vector;
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			ElementLadderTemplate* ladder_tmpl_ptr = (ElementLadderTemplate*)(ite->second);
			if (element_score >= 0 && element_score >= ladder_tmpl_ptr->LowScore && ladder_tmpl_ptr->RegionType == region_code &&  ladder_tmpl_ptr->SeasonNum == season)
			{
				temp_vector.push_back(ladder_tmpl_ptr);
			}
		}
		return temp_vector;
	}

	std::vector<ElementLadderTemplate*> template_manager::get_template_array_by_element_min_and_max_score(int32 min_score, int32 max_score, int32 season)
	{
		std::vector<ElementLadderTemplate*> temp_vector;
		if (season == 0)
		{
			season = init_unit::get_element_season();
		}
		template_type* ladder_tmpl_table = get_templates(e_ElementLadderTemplate);
		if (nullptr == ladder_tmpl_table)
		{
			return temp_vector;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return temp_vector;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == config_ptr)
		{
			return temp_vector;
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = ladder_tmpl_table->begin(); ite != ladder_tmpl_table->end(); ++ite)
		{
			ElementLadderTemplate* ladder_tmpl_ptr = (ElementLadderTemplate*)(ite->second);
			if (ladder_tmpl_ptr->RegionType != region_code || ladder_tmpl_ptr->SeasonNum != season)
			{
				continue;
			}
			//最高分小于或是最低分小于等于的跳过
			if (ladder_tmpl_ptr->HighScore < min_score || ladder_tmpl_ptr->LowScore <= min_score)
			{
				//第一个段位会在第一次升段的时候发放
				if (min_score != 0 || ladder_tmpl_ptr->LowScore != 0)
				{
					continue;
				}	
			}
			if (ladder_tmpl_ptr->LowScore <= max_score)
			{
				temp_vector.push_back(ladder_tmpl_ptr);
			}
		}
		return temp_vector;
	}

	RewardSettlementTemplate* template_manager::get_template_by_activity_score(e_activity_type type, int64 npc_score)
	{
		if (type >= e_activity_type_max)
		{
			return nullptr;
		}
		template_type* template_table = get_templates(e_RewardSettlementTemplate);
		if (nullptr == template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		template_type::iterator ite_end = template_table->end();
		for (ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			RewardSettlementTemplate*  template_ptr = (RewardSettlementTemplate*)(ite->second);
			if (template_ptr == nullptr
				|| template_ptr->Type != type)
			{
				continue;
			}
			ite_end = ite;
			if (npc_score >= template_ptr->LowScore && npc_score < template_ptr->HighScore)
			{
				return template_ptr;
			}
		}
		if (ite_end != template_table->end())
		{
			RewardSettlementTemplate*  template_ptr = (RewardSettlementTemplate*)(ite_end->second);
			if (template_ptr)
			{
				if (npc_score >= template_ptr->HighScore)
				{
					return template_ptr;
				}
			}
		}
		return nullptr;
	}
	PlayerUpgradeTemplate* template_manager::get_player_upgrade_by_level(int32 unit_type, int32 exp_level)
	{
		template_type* template_table = get_templates(e_PlayerUpgradeTemplate);
		if (nullptr == template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			PlayerUpgradeTemplate*  template_ptr = (PlayerUpgradeTemplate*)(ite->second);
			if (unit_type == template_ptr->ClassType && exp_level == template_ptr->Level)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	ConvertTemplate* template_manager::get_convert_template_by_type(int32 convert_type)
	{
		template_type* template_table = get_templates(e_ConvertTemplate);
		if (nullptr == template_table)
		{
			return nullptr;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (nullptr == region_template_ptr)
		{
			return nullptr;
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			ConvertTemplate*  template_ptr = (ConvertTemplate*)(ite->second);
			if (template_ptr->ConvertType == convert_type)
			{
				for (int32 i = 0; i < template_ptr->RegionArr.size(); ++i)
				{
					if (template_ptr->RegionArr[i] == region_code)
					{
						return template_ptr;
					}
				}		
			}
		}
		return nullptr;
	}

	FuncUnlockTemplate* template_manager::get_func_unlock_template(const int32 func_template_id)
	{
		template_type* template_table = get_templates(e_FuncUnlockTemplate);
		if (nullptr == template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			FuncUnlockTemplate*  template_ptr = (FuncUnlockTemplate*)(ite->second);
			if (nullptr == template_ptr)
			{
				continue;
			}
			if (template_ptr->attribute_id == func_template_id)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	DailyActivitiesTemplate * template_manager::get_template_by_daily_active_degree_type(int32 active_degree_type)
	{
		template_type* recharge_template_table = get_templates(e_DailyActivitiesTemplate);
		if (nullptr == recharge_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = recharge_template_table->begin(); ite != recharge_template_table->end(); ++ite)
		{
			DailyActivitiesTemplate* template_ptr = (DailyActivitiesTemplate*)(ite->second);
			if (template_ptr->ActivityTyp == active_degree_type)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}


	RankingShowTemplate* template_manager::get_ranking_template_by_npc_id(int32 ranking_npc_id)
	{
		template_type* ranking_template_table = get_templates(e_RankingShowTemplate);
		if (nullptr == ranking_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = ranking_template_table->begin(); ite != ranking_template_table->end(); ++ite)
		{
			RankingShowTemplate* template_ptr = (RankingShowTemplate*)(ite->second);
			if (template_ptr->RankingBossID == ranking_npc_id)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}
	RankingShowTemplate* template_manager::get_ranking_template_by_ranking_type(e_RankingIndex ranking_type)
	{
		template_type* ranking_template_table = get_templates(e_RankingShowTemplate);
		if (nullptr == ranking_template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = ranking_template_table->begin(); ite != ranking_template_table->end(); ++ite)
		{
			RankingShowTemplate* template_ptr = (RankingShowTemplate*)(ite->second);
			if (template_ptr->RankingType == ranking_type)
			{
				return template_ptr;
			}
		}
		return nullptr;


	}

	InteractionTemplate* template_manager::get_interaction_template(int32 interaction_type)
	{
		template_type* template_table = get_templates(e_InteractionTemplate);
		if (nullptr == template_table)
		{
			return nullptr;
		}
		template_type::iterator ite;
		for (ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			InteractionTemplate* template_ptr = (InteractionTemplate*)(ite->second);
			if (template_ptr == nullptr)
			{
				continue;
			}
			if (template_ptr->InteractionType == interaction_type)
			{
				return template_ptr;
			}
		}
		return nullptr;
	}

	WaveTemplate* template_manager::get_wave_template(BrushStrangeTemplate* brush_temp_ptr, int32 wave_id)
	{
		if (brush_temp_ptr &&
			wave_id >= 0 &&
			wave_id < brush_temp_ptr->WaveId.size())
		{
			return GET_TEMPLATE(WaveTemplate, brush_temp_ptr->WaveId[wave_id]);
		}
		return nullptr;
	}
	std::vector<int32>& template_manager::get_excellent_target_color_array(EquipTemplate* equip_template_ptr, int32 target_color)
	{
		if (equip_template_ptr == nullptr || target_color <= e_item_color_white || target_color >= e_item_color_max)
		{
			return m_empty_int_array;
		}

		switch (target_color)
		{
		//case faith::e_item_color_green:
		//	return equip_template_ptr->GreenExcellentRatio;
		//	break;
		//case faith::e_item_color_blue:
		//	return equip_template_ptr->BlueExcellentRatio;
		//	break;
		//case faith::e_item_color_purple:
		//	return equip_template_ptr->PurpleExcellentRatio;
		//	break;
		//case faith::e_item_color_orange:
		//	return equip_template_ptr->OrangeExcellentRatio;
		//	break;
		//case faith::e_item_color_red:
		//	return equip_template_ptr->RedExcellentRatio;
		//	break;
		//case faith::e_item_color_pink:
		//	return equip_template_ptr->PinkExcellentRatio;
		//	break;
		default:
			return m_empty_int_array;
			break;
		}
		return m_empty_int_array;
	}

	std::vector<int32>& template_manager::get_spirit_excellent_target_color_array(SpiritTemplate* spirit_template_ptr, int32 target_color)
	{
		if (spirit_template_ptr == nullptr
			|| target_color <= e_item_color_white
			|| target_color >= e_item_color_max)
		{
			return m_empty_int_array;
		}

		switch (target_color)
		{
		case faith::e_item_color_green:
			return spirit_template_ptr->GreenExcellentRatio;
			break;
		case faith::e_item_color_blue:
			return spirit_template_ptr->BlueExcellentRatio;
			break;
		case faith::e_item_color_purple:
			return spirit_template_ptr->PurpleExcellentRatio;
			break;
		case faith::e_item_color_orange:
			return spirit_template_ptr->OrangeExcellentRatio;
			break;
		case faith::e_item_color_red:
			return spirit_template_ptr->RedExcellentRatio;
			break;
		case faith::e_item_color_pink:
			return spirit_template_ptr->PinkExcellentRatio;
			break;
		default:
			return m_empty_int_array;
			break;
		}
		return m_empty_int_array;
	}

	TimeLimitActivityTemplate* template_manager::get_time_limit_activity_template_ptr(int32 template_id)
	{
		TimeLimitActivityTemplate* time_limit_activity_template_ptr = get_template<TimeLimitActivityTemplate>(e_OperatorUseActivityTemplate, template_id);
		if (nullptr == time_limit_activity_template_ptr)
		{
			time_limit_activity_template_ptr = get_template<TimeLimitActivityTemplate>(e_TimeLimitActivityTemplate, template_id);
		}
		return time_limit_activity_template_ptr;
	}

	TimeLimitActivityBranchTemplate* template_manager::get_time_limit_activity_branch_template_ptr(int32 template_id)
	{
		TimeLimitActivityBranchTemplate* time_limit_activity_template_branch_ptr = get_template<TimeLimitActivityBranchTemplate>(e_OperatorUseActivityBranchTemplate, template_id);
		if (nullptr == time_limit_activity_template_branch_ptr)
		{
			time_limit_activity_template_branch_ptr = get_template<TimeLimitActivityBranchTemplate>(e_TimeLimitActivityBranchTemplate, template_id);
		}
		return time_limit_activity_template_branch_ptr;
	}

	EquipEnchantTemplate* template_manager::get_equip_enchant_template_ptr(int32 level, int32 type, int32 slot)
	{
		int32 enchant_template_id = -1;
		faith::template_manager::template_type* enchant_table = get_templates(e_EquipEnchantTemplate);
		if (nullptr == enchant_table)
		{
			return nullptr;
		}
		faith::template_manager::template_type::iterator ite;
		for (ite = enchant_table->begin(); ite != enchant_table->end(); ++ite)
		{
			EquipEnchantTemplate* temp_enchant_template_ptr = (EquipEnchantTemplate*)(ite->second);
			if (temp_enchant_template_ptr == nullptr)
			{
				continue;
			}
			if ((temp_enchant_template_ptr->Level == level) && (temp_enchant_template_ptr->Type == type) && (temp_enchant_template_ptr->Slot == slot))
			{
				enchant_template_id = temp_enchant_template_ptr->attribute_id;
				break;
			}
		}
		if (enchant_template_id < 0)
		{
			return nullptr;
		}
		EquipEnchantTemplate* enchant_template_ptr = GET_TEMPLATE(EquipEnchantTemplate, enchant_template_id);
		if (enchant_template_ptr == nullptr)
		{
			return nullptr;
		}

		return enchant_template_ptr;
	}

	CompetitionSeasonTemplate* template_manager::get_template_by_competition_level(int32 level, int64 world_level)
	{
		int32 season = init_unit::get_ladder_season();
		template_type* competition_tmpl_table = get_templates(e_CompetitionSeasonTemplate);
		if (nullptr == competition_tmpl_table)
		{
			return nullptr;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return nullptr;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr || (config_ptr->ParamIntArr6.size() % 3) != 0 || config_ptr->ParamIntArr6.size() <= 0)
		{
			return nullptr;
		}
		int64 competion_level = 0;
		for (int32 i = 0; i < config_ptr->ParamIntArr6.size() / 3; ++i)
		{
			if (world_level >= config_ptr->ParamIntArr6[i * 3] && world_level <= config_ptr->ParamIntArr6[i * 3 + 1])
			{
				competion_level = config_ptr->ParamIntArr6[i * 3 + 2];
				break;
			}
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = competition_tmpl_table->begin(); ite != competition_tmpl_table->end(); ++ite)
		{
			CompetitionSeasonTemplate* competition_tmpl_ptr = (CompetitionSeasonTemplate*)(ite->second);
			if (level == competition_tmpl_ptr->Level && competition_tmpl_ptr->Region == region_code &&  competition_tmpl_ptr->SeasonNum == competion_level)
			{
				return competition_tmpl_ptr;
			}
		}
		return nullptr;
	}

	ElementCompetitionTemplate * template_manager::get_template_by_element_competition_level(int32 level, int64 world_level)
	{
		int32 season = init_unit::get_element_season();
		template_type* competition_tmpl_table = get_templates(e_ElementCompetitionTemplate);
		if (nullptr == competition_tmpl_table)
		{
			return nullptr;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return nullptr;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr || (config_ptr->ParamIntArr6.size() % 3) != 0 || config_ptr->ParamIntArr6.size() <= 0)
		{
			return nullptr;
		}
		int64 competion_level = 0;
		for (int32 i = 0; i < config_ptr->ParamIntArr6.size() / 3; ++i)
		{
			if (world_level >= config_ptr->ParamIntArr6[i * 3] && world_level <= config_ptr->ParamIntArr6[i * 3 + 1])
			{
				competion_level = config_ptr->ParamIntArr6[i * 3 + 2];
				break;
			}
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = competition_tmpl_table->begin(); ite != competition_tmpl_table->end(); ++ite)
		{
			ElementCompetitionTemplate* competition_tmpl_ptr = (ElementCompetitionTemplate*)(ite->second);
			if (level == competition_tmpl_ptr->Level && competition_tmpl_ptr->Region == region_code &&  competition_tmpl_ptr->SeasonNum == competion_level)
			{
				return competition_tmpl_ptr;
			}
		}
		return nullptr;
	}

	std::vector<BreakWillTemplate*> template_manager::get_break_will_template_ptr_type_array()
	{
		std::vector<BreakWillTemplate*> return_vector;
		template_type* break_will_templatee_table = get_templates(e_BreakWillTemplate);
		if (nullptr == break_will_templatee_table)
		{
			return return_vector;
		}
		template_type::iterator ite;
		for (ite = break_will_templatee_table->begin(); ite != break_will_templatee_table->end(); ++ite)
		{
			BreakWillTemplate* template_ptr = (BreakWillTemplate*)(ite->second);
			if (nullptr == template_ptr)
			{
				continue;
			}
			if (template_ptr->Type > -1)
			{
				return_vector.push_back(template_ptr);
			}
		}
		return return_vector;
	}

	int32 template_manager::get_competition_first_id(int64 world_level)
	{
		int32 season = init_unit::get_ladder_season();
		template_type* competition_tmpl_table = get_templates(e_CompetitionSeasonTemplate);
		if (nullptr == competition_tmpl_table)
		{
			return 0;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return 0;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr || (config_ptr->ParamIntArr6.size() % 3) != 0 || config_ptr->ParamIntArr6.size() <= 0)
		{
			return 0;
		}
		int64 competion_level = 0;
		for (int32 i = 0; i < config_ptr->ParamIntArr6.size() / 3; ++i)
		{
			if (world_level >= config_ptr->ParamIntArr6[i * 3] && world_level <= config_ptr->ParamIntArr6[i * 3 + 1])
			{
				competion_level = config_ptr->ParamIntArr6[i * 3 + 2];
				break;
			}
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = competition_tmpl_table->begin(); ite != competition_tmpl_table->end(); ++ite)
		{
			CompetitionSeasonTemplate* competition_tmpl_ptr = (CompetitionSeasonTemplate*)(ite->second);
			if (competition_tmpl_ptr->Region == region_code && competition_tmpl_ptr->SeasonNum == competion_level)
			{
				return competition_tmpl_ptr->attribute_id;
			}
		}
		return 0;
	}

	int32 template_manager::get_element_competition_first_id(int64 world_level)
	{
		int32 season = init_unit::get_element_season();
		template_type* competition_tmpl_table = get_templates(e_ElementCompetitionTemplate);
		if (nullptr == competition_tmpl_table)
		{
			return 0;
		}
		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return 0;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == config_ptr || (config_ptr->ParamIntArr6.size() % 3) != 0 || config_ptr->ParamIntArr6.size() <= 0)
		{
			return 0;
		}
		int64 competion_level = 0;
		for (int32 i = 0; i < config_ptr->ParamIntArr6.size() / 3; ++i)
		{
			if (world_level >= config_ptr->ParamIntArr6[i * 3] && world_level <= config_ptr->ParamIntArr6[i * 3 + 1])
			{
				competion_level = config_ptr->ParamIntArr6[i * 3 + 2];
				break;
			}
		}
		int32 region_code = region_template_ptr->RegionCode;
		template_type::iterator ite;
		for (ite = competition_tmpl_table->begin(); ite != competition_tmpl_table->end(); ++ite)
		{
			ElementCompetitionTemplate* competition_tmpl_ptr = (ElementCompetitionTemplate*)(ite->second);
			if (competition_tmpl_ptr->Region == region_code && competition_tmpl_ptr->SeasonNum == competion_level)
			{
				return competition_tmpl_ptr->attribute_id;
			}
		}
		return 0;
	}

	std::string	template_manager::get_all_string_context(const int32 string_template_id, std::string separative_sign)
	{
		std::string return_string = "";
		const std::vector<std::string>& temp_array_list = get_string_list(string_template_id);
		int32 array_size = temp_array_list.size();
		if (array_size <= 0)
		{
			return return_string;
		}

		for (int32 i = 0; i < array_size; i++)
		{
			return_string += temp_array_list[i];
			if (i < array_size - 1)
			{
				return_string += separative_sign;
			}
		}

		return return_string;
	}
	bool template_manager::is_message_use_lua(int32 head)
	{
		LuaMessageTemplate* LuaMessageTemplatePtr = GET_TEMPLATE(LuaMessageTemplate, head);
		if (LuaMessageTemplatePtr == nullptr)
		{
			return false;
		}
		return true;
	}
}
