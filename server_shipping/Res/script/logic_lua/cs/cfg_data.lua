 
local cs_t_files = {
	"common",
	"class",
	"functions",
	"template_manager",
	"init_unit",
	"proto_mgr",
	"const_define",
	"data_flag",
	"register_data",  
	"formula_calculation",  

	"message_mgr",
	"dp_msgproc",
	"ws_msgproc",
	"server_tool",
	"belief",
	"belief_rune",
	"royal_treasure",
	"marry_mgr",
	"lua_oracle_trial",
	"legion_dance_mgr",
	"awaken_mgr",
	"pawn_att",
	"welfare",
	"equip_strengthen",
	"element_heart",
	"spirit",
	"recharge",
	"feather_heart",
	"feather_def",
	"convert",
	"item_set",
	"patron_saint",
	"item_def",
	"pokedex_def",
	"meditation",
	"meditation_def",
	"mail_def",
	"recycle",
	"wing",
	"beast_spirit",
	"att_def",
	"composit",
	"mount",
	"grade_up",
	"mission",
	"goddess",
	"role_summary",
	"pokedex_mgr",
	"store",
	"seven_day_goal",
	"msgproc_client",
	"interaction_mgr",
	"dragontrip",
	"auction",
	"arena",
	"talent",
	"phantom",
	"achievement",
	"cross_ladder_map_cs",
	"daily_must_do",
	"dungeon",
	"legion",
	"activity",
	"skill",
	"time_mgr",
	"player_mgr",
}

--[[local cs_t_files = {
	"common",
	"class",
	
	"proto_mgr",
	
	"register_data",  

	"time_mgr",
	"player_mgr",
}]]

---------------------------------
 ---全局文件加载所有文件 
--------------------------------

g_t_files = cs_t_files



--------------------------------------------------------
 -- 加载一次初始化-次文件
--------------------------------------------------------

local t_one_files =    
{
	class = 1,
	register_data = 1,
}

g_one_files = t_one_files


---------------------------------------------------------
-- 多次加载初始化一次

----------------------------------------------------------

local t_one_init = 
{

	time_mgr = 1,
	player_mgr = 1,
}


g_one_init = t_one_init


local t_debug_files = 
{

}

g_debug_files = t_debug_files

-------------------------------------------------------


local m_TemplateName =
{
    "VIPTemplate",
	"WeddingRingTemplate",
	"ElementHeartTemplate",
	"ItemTemplate",
	"ElementHeartUpgradeTemplate",
	"QuestionMarryTemplate",
	"MapTemplate",
	"DropTemplate",
	"OracleTrialTemplate",
	"GameConfigTemplate",
	"PlayerUpgradeTemplate",
	"ActivityCommonConfigTemplate",
	"WelfareTemplate",
    "AwakenTemplate",
    "NoticeTemplate",
    "EquipTemplate",
	"SpiritTemplate",
    "ItemUpgradeTemplate",
	"ItemAddonTemplate",
    "PropBasicAttributeTemplate",
    "GodHeartTemplate",
    "GodHeartUpgradeTemplate",
    "FeatherTemplate",
    "PassiveSkillTemplate",
    "RegionTemplate",
    "GoodsTemplate",
	"ConvertTemplate",
	"BeliefTemplate",
	"BeliefRunesTemplate",
	"BeliefRunesUpgradeTemplate",
	"GrowUpFundTemplate",
    "ProtectSpiritTemplate",
    "TinderTemplate",
    "MissionTemplate",
    "JewelTemplate",
    "VIPTemplate",
    "RecycleTemplate",
   	"WingTemplate",
    "BeastSpiritTemplate",
	"CompositTemplate",
    "MountTemplate",
    "PlayerInitTemplate",
    "GradeGodHoodTemplate",
    "MissionChapterTemplate",
    "DeityTemplate",
    "PokedexTemplate",
    "PsycheTemplate",
    "FasionTemplate",
    "SpecialNameTemplate",
    "StoreTemplate",
    "AttributeTemplate",
    "ServiceGoalTemplate",
    "ItemSuccinctTemplate",
    "AdditionBuffTemplate",
    "EquipEnchantTemplate",
	"MoneyTemplate",
    "DragonTripTemplate",
    "ArenaRankRewardTemplate",
    "TalentTemplate",
    "TalentListTemplate",
    "AchievementTemplate",
    "DailyActivitiesTemplate",
    "LegionConstructionsTemplate",
	"BreakWillTemplate",
	"RewardByRegionTemplate",
	"AwakenFetterTemplate"
}

-----------------------------------
  --策划配表文件
------------------------------------

g_TemplateName = m_TemplateName



local define_c = {}

define_C = define_c


