#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void FeatherTemplateSize();
#pragma pack(push, 1)
    struct FeatherTemplate : public template_base
    {
        int32 NameStringId;
        int32 UpgradeMax;
        int32 AddStarMax;
        int32 UpgradeNow;
        int32 AddStarNow;
        std::vector<float> AttArray;
        int32 AddStarMatirialId;
        int32 StarShopId;
        int32 AddStarMatirialNum;
        std::vector<int32> AddStarMoneyList;
        int32 UpgradeMatirialId;
        int32 UpgradeShopId;
        int32 UpgradeMatirialNum;
        std::vector<int32> UpgradeMoneyList;
        int32 ExhibitionNpcId;
        std::vector<int32> ExhibitionBuffArray;
        int32 UnlockMaterialId;
        int32 UnlockMaterialNum;
        int32 SkillID;
        std::vector<int32> EventCondition;
        std::vector<int32> BuffArray;
        std::vector<float> PurenessBasicAtt;
    };
#pragma pack(pop)
}
