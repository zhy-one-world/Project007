#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void DailyActivitiesTemplateSize();
#pragma pack(push, 1)
    struct DailyActivitiesTemplate : public template_base
    {
        int32 ActivityTyp;
        int32 TargetValue;
        int32 AddValue;
        std::vector<int32> VIPAddNum;
        int32 TagType;
        std::vector<int32> SubTabArray;
        int32 ActivityOrder;
        std::vector<int32> ShowFlagArray;
        int32 DesStringTemplateId;
        int32 OpenTimeDescription;
        int32 ExpTitleIcon;
        int32 ExpTitleIconNum;
        int32 MustDoRewardClass;
        int32 PeopleTitleIcon;
        std::vector<int32> RewardClassArray;
        std::vector<float> DynamicMoneyShow;
        std::vector<int32> StaticMoneyShow;
        std::vector<int32> ItemShow;
        std::vector<int32> WidgetJumpArray;
        int32 IsCanFindBack;
        std::vector<int32> EffectLevelRange;
        std::vector<float> DynamicFindBackInfo;
        std::vector<int32> StaticFindBackInfo;
        int32 SiliverBackCost;
        int32 DiamondBackCost;
        std::vector<int32> ItemFindBackArray;
        int32 MapTemplateId;
        std::string FuncUnlockName;
        int32 SubscribeRewardId;
        std::vector<int32> InMapList;
    };
#pragma pack(pop)
}
