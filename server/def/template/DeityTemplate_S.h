#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void DeityTemplateSize();
#pragma pack(push, 1)
    struct DeityTemplate : public template_base
    {
        int32 Name;
        int32 Describe;
        int32 Icon;
        int32 Type;
        std::vector<int32> ItemGet;
        int32 CurLevel;
        int32 MaxLevel;
        std::vector<int32> UpgradeCost;
        std::vector<int32> AddExpCost;
        int32 UpgradeNeedNum;
        std::vector<float> AttArray;
        int32 NpcId;
        std::vector<int32> AdditionBuff;
        std::vector<int32> AdditionBuffName;
        std::vector<int32> AdditionBuffDescribe;
        std::vector<int32> CooperateBuff;
        std::vector<int32> CooperateBuffDescribe;
        std::vector<int32> CooperateUnlockNeed;
        int32 StarID;
        int32 EquNeedLv;
        int32 ShowBuff;
    };
#pragma pack(pop)
}
