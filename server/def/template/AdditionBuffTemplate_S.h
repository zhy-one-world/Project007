#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void AdditionBuffTemplateSize();
#pragma pack(push, 1)
    struct AdditionBuffTemplate : public template_base
    {
        int32 BuffName;
        int32 BuffDescription;
        int32 AdditionBuffType;
        int32 AdditionBuffSubType;
        int32 AdditionBuffLevel;
        int32 EquipNum;
        int32 UpgradeNum;
        int32 EquipQuality;
        int32 AchievementID;
        int32 FeatherNum;
        int32 FeatherUpgradeNum;
        int32 RankID;
        int32 AddOnLevel;
        int32 SuccinctFullQualityLevel;
        std::vector<float> AdditionValue;
        std::vector<int32> BuffArray;
        int32 Icon;
        int32 IsCanShow;
        int32 ShowNoticeId;
        int32 SuitSkillID;
    };
#pragma pack(pop)
}
