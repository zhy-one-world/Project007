#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void LegionConstructionsTemplateSize();
#pragma pack(push, 1)
    struct LegionConstructionsTemplate : public template_base
    {
        int32 Level;
        int32 MaxMemNum;
        std::vector<int32> MaxJobNum;
        int32 CreateNeedLevel;
        std::vector<int32> CreateNeedMoney;
        int32 LegionMaintainMoney;
        int32 RecruitInWorldCD;
        std::vector<int32> RemoveRecruitCDCost;
        int32 ChiefDismissedOfflineHour;
        int32 MainConstructionIcon;
        int32 MainConstructionName;
        int32 MainConstructionLevelUpCostMoney;
        std::vector<int32> MainConstructionLevelUpCostItems;
        int32 MainConstructionDescribe;
        int32 Construction_1_Icon;
        int32 Construction_1_Name;
        int32 Construction_1_LevelUpCostMoney;
        std::vector<int32> Construction_1_LevelUpCostItems;
        int32 Construction_1_Describe;
        int32 Construction_2_Icon;
        int32 Construction_2_Name;
        int32 Construction_2_LevelUpCostMoney;
        std::vector<int32> Construction_2_LevelUpCostItems;
        int32 Construction_2_Describe;
        int32 Construction_3_Icon;
        int32 Construction_3_Name;
        int32 Construction_3_LevelUpCostMoney;
        std::vector<int32> Construction_3_LevelUpCostItems;
        int32 Construction_3_Describe;
        std::vector<int32> DonateMoneyCost;
        int32 DonateMoneyGet;
        int32 BattleAchievementGet;
        int32 BattleAchievementGetLimit;
        int32 DonateMoneyGetContribution;
        std::vector<int32> DonateItemsID;
        std::vector<int32> DonateItemGetMoney;
        std::vector<int32> DonateItemGetBattleAchievement;
        int32 DonateItemGetBattleAchievementLimit;
        std::vector<int32> DonateItemGetContribution;
        std::vector<float> GetContributionScale;
        int32 BonfireNpcId;
        std::vector<int32> BonfireNpcFireBuff;
        int32 BonfireAdditionNum;
        std::vector<int32> MemDailyAwards;
        std::vector<int32> LegionBossGetAwardList;
        int32 CreateNeedVipLevel;
        int32 CityWarRewardNum;
        int32 LegionWelfareMaxNum;
        int32 LegioWelfareItemId;
        std::vector<int32> LegioWelfareShowItemIdList;
        int32 LegionWelfareLowestGiveNum;
        std::vector<int32> WorldBossRankWelfareNum;
        std::vector<int32> CrossBossRankWelfareNum;
        std::vector<int32> LegionBossRankWelfareNum;
        std::vector<int32> OverlordRankWelfareNum;
        std::vector<int32> CrossOverlordRankWelfareNum;
        std::vector<int32> AttackCityWelfareNum;
    };
#pragma pack(pop)
}
